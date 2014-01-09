/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011,2012,2013 Zoltan Turanyi
    Distributed under GNU Affero General Public License.

    Msc-generator is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Msc-generator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with Msc-generator.  If not, see <http://www.gnu.org/licenses/>.
*/

/** @file contour_ellipse.h Declares class EllipseData.
 * @ingroup contour_files
 */


#if !defined(CONTOUR_ELLIPSE_H)
#define CONTOUR_ELLIPSE_H

#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "cairo.h"
#include "contour_distance.h"

namespace contour_standard_edge {

    using namespace contour;

/** @addtogroup contour_internal
 * @{
 */

struct quadratic_xy_t;

/**A helper class implementing an ellipse.
 *
 * The class implements crosspoints with ellipses and lines.
 * The ellipse can be tilted (rotated, making its axises not align with the X and Y axises,
 * but it always represents a full ellipse, not just a section of it.
 *
 * Radian values are often used to describe a point on the circumference
 * of the ellipse. These values are meant
 * - in radians, between 0 and 2*PI (not taking the latter value);
 * - clockwise growing from the zero value at the leftmost extreme (if not tilted). Bear in mind
 *   that since y coordinate grows downward, clockwise starts toward growing y coordinate from zero
 *   radian; and
 * - irrespective of tilt. Thus if a radian value marks a point on the circumference, if the
 *   ellipse is tilted, the point tilts with it.
 */
class EllipseData
{
    friend struct quadratic_xy_t;
protected:
    XY     center;     ///< Center of the ellipse.
    double radius1;    ///< x coordinate radius.
    double radius2;    ///< y coordinate radius.
    bool   tilted;     ///< True if the ellipse is tilted and `tilt` shall be considered.
    double tilt;       ///< The (clockwise) tilt of the ellipse in radians. Shall be between [0..pi/2).
    double sintilt;    ///< Pre-computed values of `sin(tilt)`, if `tilted` is set.
    double costilt;    ///< Pre-computed values of `cos(tilt)`, if `tilted` is set.
    XY     extreme[4]; ///< Pre-computed values of the leftmost, rightmost, topmost, bottommost point of the ellipse, respectively.
    /** Radian values corresponding to the extreme points (same order as `extreme`).
     *
     * These are to be understood compared to the main axes of the ellipse.
     * Thus to calculate the extreme point from `extreme_radian` you can calculate
     * the point first as if the ellipse were not tilted, and then simply rotate
     * the point around `center` by `tilt`.
     */
    double extreme_radian[4];
    mutable double circumference_cache; ///< If nonnegative it caches the length of the cirumference.

    XY   conv_to_circle_space(const XY &p) const;
    XY   conv_to_real_space(const XY &p) const;
    int  refine_crosspoints(int num_y, double y[], const EllipseData &b,
                            const quadratic_xy_t &one, const quadratic_xy_t &two, XY p[]) const;
    bool refine_point(const EllipseData &B, XY &p) const;
    bool refine_point(const XY &A, const XY &B, XY &p) const;
    void transpose_curvy_non_tilted();
	void calculate_extremes();
	double add_to_tilt(double cos, double sin, double radian);
    double CircumferenceHelper(double to) const;   //circumference from 0 radian to "to"
    void CalcCircumferenceEllipse() const; //Calculate full circumference for ellipses & cache it

public:
    EllipseData() : circumference_cache(-1) {};
    EllipseData(const XY &c, double radius1, double radius2=0, double tilt_degree=0);
    bool operator ==(const EllipseData& p) const;
    bool operator <(const EllipseData& p) const;
    bool IsTilted() const {return tilted;}
    const XY & GetCenter() const {return center;}      ///<Return the center of the ellipse.
    double GetRadius1() const {return radius1;}        ///<Return the x-axis radius 
    double GetRadius2() const {return radius2;}        ///<Return the y-axis radius 
    double GetTilt() const {return tilted ? tilt : 0;} ///<Return the tilt of the ellipse
    double GetExtreme(unsigned n, XY &xy) const {xy = extreme[n]; return extreme_radian[n];}
    double FindExtreme(double rad, bool after, XY &p) const;
    double Distance(const XY &p, XY &point, double &rad) const;
    double Distance(const XY &start, const XY &end, XY p[2]) const;
    void Shift(const XY &xy);
    void Scale(double sc);
    double Rotate(double cos, double sin, double radian);
    double RotateAround(const XY&c, double cos, double sin, double radian);
    void SwapXY();

    //calculate one point on the tangent at pos (between 0..1) either in the
    //  forward direction (next==true) or in the backward direction
    XY Tangent(double radian, bool next) const;
    double Point2Radian(const XY &p) const;
    XY Radian2Point(double r) const {return conv_to_real_space(XY(cos(r), sin(r)));} ///< Returns the ccordinates of the point on the contour of the ellipse for `radian`.
    //this returns positive always and assumes clockwise arc
    double SectorCircumference(double from, double to) const;
    double FullCircumference() const {if (circumference_cache<0) {if (radius1 == radius2) circumference_cache = 2*M_PI*radius1; else CalcCircumferenceEllipse();} return circumference_cache;} ///< Return the length of circumference of the whole ellipsys.
    double SectorArea(double from, double to) const {return radius1*radius2*fmod_negative_safe(to-from, 2*M_PI)/2;} ///< Return the area between the two radians `from` and `to` considering warping.
    double FullArea() const {return radius1*radius2*M_PI;}
    XY     SectorCentroidTimesArea(double from, double to) const;

    //return the number of common points, coordinates in "r"
    //also the radian or relative position inside the straight edge
    int CrossingEllipse(const EllipseData &B,
                        XY r[], double radian_us[], double radian_b[]) const;
    int CrossingStraight(const XY &A, const XY &B, XY *r, bool want_closest) const; //the infinite line of A-B
    int CrossingStraight(const XY &A, const XY &B,
                         XY r[], double radian_us[], double pos_b[]) const;
    int CrossingVertical(double x, double y[], double radian[]) const;

    int Expand(double gap); //returns -1 if completely disappear, 0 if degrades to line, 1 if OK
    void TransformForDrawing(cairo_t *cr) const;

    //return a positive number if the two can be moved closer together
    //return Infinity() if the two do not overlap in x coordinates
    double OffsetBelow(const EllipseData&) const;
    double OffsetBelow(const XY&A, const XY&B) const;
    double OffsetAbove(const XY&A, const XY&B) const;

    bool TangentFrom(const XY &from, XY &clockwise, XY &cclockwise) const;
    bool TangentFrom(const EllipseData &from, XY clockwise[2], XY cclockwise[2]) const;
};

/** Defines how two (finite length) sections can cross each other.
 */
enum ELineCrossingType {
    LINE_CROSSING_PARALLEL, ///< No crossing, the two sections are parallel.
    LINE_CROSSING_INSIDE,   ///< Real crossing, the crosspoint is inside both sections.
    LINE_CROSSING_OUTSIDE   ///< The line of the two lines cross, but the crosspoint is outside at least one of the sections.
};

ELineCrossingType crossing_line_line(const XY &A, const XY &B, const XY &M, const XY &N,  XY &r);
double point2pos_straight(const XY &M, const XY&N, const XY &p);

/** @} */ //addtogroup contour_internal 

inline bool EllipseData::operator ==(const EllipseData& p) const
{
    if (center!=p.center || tilted!=p.tilted ||
        radius1!=p.radius1 || radius2!=p.radius2 ) return false;
    if (tilted) return tilt==p.tilt;
    return true;
}

inline bool EllipseData::operator < (const EllipseData& p) const
{
    if (center!=p.center) return center<p.center;
    if (tilted != p.tilted) return tilted;
    if (radius1!=p.radius1) return radius1 < p.radius1;
    if (radius2!=p.radius2) return radius2 < p.radius2;
    if (tilted) return tilt < p.tilt;
    return false; //they are equal
}

/** Translate from chart space to a space, where the ellipse is a non-tilted unit circle.
 */
inline XY EllipseData::conv_to_real_space(const XY &p) const
{
    if (!tilted)
        return center + XY(radius1 * p.x, radius2 * p.y);
    return center +
        XY(radius1 * p.x * costilt - radius2 * p.y * sintilt,
           radius1 * p.x * sintilt + radius2 * p.y * costilt);
}

/** Translate from space, where the ellipse is a non-tilted unit circle back to chart space.
 */
inline XY EllipseData::conv_to_circle_space(const XY &p) const
{
    if (!tilted)
        return XY((p.x-center.x)/radius1, (p.y-center.y)/radius2);
    return XY(( (p.x-center.x)*costilt + (p.y-center.y)*sintilt)/radius1,
              (-(p.x-center.x)*sintilt + (p.y-center.y)*costilt)/radius2);
}

/** Finds the radian value for a point on the circumference of the unit circle.
 *
 * If the point is not on the circumference, then we take the intersection
 * of the unit circle and a line between the center of the ellipse and `p`.
 * If `p` is at the center of the ellipse, we throw a divide by zero exception.
 */
inline double circle_space_point2radian_curvy(XY p)
{
    double r = asin(p.y/p.length()); //calc angle normalized to 1
    if (p.x<0) r = M_PI - r;         //between 90 and 270 degrees
    if (test_zero(r)) r=0;           //round very small negative values
    if (r<0) r += 2*M_PI;            //if between -90 and 0 degrees
    return r;
}

/** Finds the radian value for a point on the circumference of the ellipse.
 *
 * If the point is not on the circumference, then we take the intersection
 * of the ellipse and a line between the center of the ellipse and `p`.
 * If `p` is at the center of the ellipse, we throw a divide by zero exception.
 */
inline double EllipseData::Point2Radian(const XY &p) const
{
    XY q = conv_to_circle_space(p);
    const double d = q.length();
    const double r = circle_space_point2radian_curvy(q/d);
    return r;
}

/** Calculate the length of the Circumference between two radians `from` and `to`.
 *
 * Note that if `from` is larger than `to`, we calculate for a wrapped section.
 * That is the result will be the same as the total length of the circumference of
 * the full ellipse minus `SectorCircumference(to, from)`.
 */
inline double EllipseData::SectorCircumference(double from, double to) const
{
    if (radius1 == radius2) return fmod_negative_safe(to-from, 2*M_PI)*radius1;
    const double zero2to = CircumferenceHelper(fmod(to, 2*M_PI));
    const double zero2from = CircumferenceHelper(fmod(from, 2*M_PI));
    if (from < to) return zero2to - zero2from;
    return zero2to - zero2from + circumference_cache; //PerimiterHelper calls Perimiter() so cache is up-to-date
}


/** Transform a cairo context, so that this ellipse becomes the unit circle
 */
inline void EllipseData::TransformForDrawing(cairo_t *cr) const
{
    cairo_translate(cr, center.x, center.y);
    if (tilted)
        cairo_rotate(cr, tilt);
    cairo_scale(cr, radius1, radius2);
}


/** Finds the radian of the extreme close to the radian `rad`.
 *
 * A specialty of this helper function is that if `rad` is exactly
 * on an extreme (well, within SMALL_NUM), we return another extreme,
 * not the one `rad` is on.
 *
 * @param [in] rad The radian we start from
 * @param [in] after If true we return an extreme clockwise from `rad`, else counterclockwise
 * @param [out] p We return the coordinates of the extreme here.
 * @returns The radian of the extreme.
 *
 */
inline double EllipseData::FindExtreme(double rad, bool after, XY &p) const
{
    double maxdiff = MaxVal(maxdiff);
    unsigned ret=0;
    for (unsigned u=0; u<4; u++) {
        const double diff = fmod_negative_safe(after ? extreme_radian[u]-rad : rad-extreme_radian[u], 2*M_PI);
        if (diff<maxdiff && !test_zero(diff))
            ret = u, maxdiff = diff;
    }
    p = extreme[ret];
    return extreme_radian[ret];
}


} //namespace


#endif //CONTOUR_ELLIPSE_H
