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

/** @file contour_distance.h Declares class DistanceType and some helpers.
 * @ingroup contour_files
 */

#if !defined(CONTOUR_DISTANCE_H)
#define CONTOUR_DISTANCE_H

#define _USE_MATH_DEFINES
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include "contour_basics.h"

namespace contour {

/** @addtogroup contour_internal
 * @{
 */


/** Returns true if `r` is in [0..1]
 */
inline bool between01(double r)
{
    return r>=0 && r<=1;
}

/** Returns true if `r` is in [0..1] or just outside by max SMALL_NUM.
 */
inline bool between01_approximate_inclusive(double r)
{
    return !(test_smaller(r, 0) || test_smaller(1, r));
}

/** Returns true if `n` is not significantly outside [0,1] (by max SMALL_NUM). 
  If n is close to 0 or 1 we return exactly 0 or 1.
 */
inline bool between01_adjust(double &n)
{
    if (test_smaller(1,n) || test_smaller(n,0)) //if n>1 or n~=1 or n<<0
        return false;
    if (!test_smaller(0, n))
        n = 0; //if n~=0;
    else if (!test_smaller(n, 1))
        n = 1; //if n~=1;
    return true;
}

/** An `fmod(a, b)`, that always return a value in `[0..b)` even if `a<0`
*/
template<typename real> real fmod_negative_safe(real a, real b) { _ASSERT(b>0); return a>=0 ? fmod(a, b) : b - fmod(-a, b); }

/** Converts between degrees and radians.
 */
inline double deg2rad(double degree)
{
    return fmod_negative_safe(degree, 360.)*(M_PI/180);
}

/** Converts between radians and degrees.
 */
inline double rad2deg(double degree)
{
	return fmod_negative_safe(degree, 2*M_PI)*(180/M_PI);
}

/** Describes how three points can relate to each other.
*/
enum ETriangleDirType
{
    ALL_EQUAL,       ///<All three are identical.
    A_EQUAL_B,       ///<Two of them are identical.
    A_EQUAL_C,       ///<Two of them are identical.
    B_EQUAL_C,       ///<Two of them are identical.
    IN_LINE,         ///<Three separate points on a line.
    CLOCKWISE,       ///<`A`->`B`->`C` define a non-degenerate clockwise triangle.
    COUNTERCLOCKWISE ///<`A`->`B`->`C` define a non-degenerate counterclockwise triangle.
};

ETriangleDirType triangle_dir(XY a, XY b, XY c);

double angle(XY base, XY A, XY B);  //defined in contour_ellipse.cpp
double angle_to_horizontal(XY base, XY A); //defined in contour_ellipse.cpp

/** Convert from a fake angle to degrees.
 */
inline double angle2degree(double angle) {
    return (angle>=2) ? 360 - acos(angle-3)*(180./M_PI) : acos(1-angle)*(180./M_PI);
}
/** Convert from a fake angle to radians.
 */
inline double angle2radian(double angle) {
    return (angle>=2) ? 2*M_PI - acos(angle-3) : acos(1-angle);
}
/** Convert from degrees to fake angle.
 */
inline double degree2angle(double degree) {
    const double rad = fmod_negative_safe(degree, 360.)/(180./M_PI);
    return (rad>=M_PI) ? 3 + cos(2*M_PI-rad) : 1 - cos(rad);
}
/** Convert from radians to fake angle. 
 */
inline double radian2angle(double rad) {
    rad = fmod_negative_safe(rad, 2*M_PI);
    return (rad>=M_PI) ? 3 + cos(2*M_PI-rad) : 1 - cos(rad);
}

/** Defines how two (finite length) sections can cross each other.
*/
double point2pos_straight(const XY &M, const XY&N, const XY &p);

/** Returns +1 if P is on the right side of A->B, -1 on left side
* and 0 if on the line. */
inline int WhichSectionSide(const XY&A, const XY&B, const XY&P)
{
    return fsign((B-A).Rotate90CW().DotProduct(P-A));
}



/** Helper class to collect data during distance calculation.
 * 
 * We distinguish between case when an element is inside a contour or outside.
 * The former is represented by a negative distance.
 * We use this class to store the smallest distance so far during a pairwise comparison
 * of contour edges.
 * We store two flags to see if we had points inside a contour or outside.
 * If both were seen, the two contours may cross each other.
 * Here 'inside' does not mean 'in a hole', that counts as outside.
 */
struct DistanceType
{
private:
    //friend class SimpleContour;
    //friend class ContourWithHoles;
    bool was_inside;    ///<True if there was one point of the other shape inside us.
    bool was_outside;   ///<True if there was one point of the other shape inside us.
    double distance;    ///<The smallest distance so far. (negative if one inside the other)
    XY point_on_me;     ///<This of my points was closest to the other shape.
    XY point_on_other;  ///<This of the other's points was closest to us.
public:
    DistanceType() {Reset();}
    void Reset() {was_inside = was_outside; distance = MaxVal(distance);}  ///<Make the distance infinite (and invalid)
    DistanceType &SwapPoints() { std::swap(point_on_me, point_on_other); return *this; }            ///<Swap `point_on_me` and `point_on_other`
    void Merge(double d, const XY &point_on_me, const XY &point_on_other); ///<Check if `d` is smaller than `distance` and if so, replace. Set inside or outside flag depending on the sign of `d`.
    void Merge(const DistanceType &o);                                     ///<Check if `o` is smaller and replace if so.
    bool ConsiderBB(double bbdist) const {return fabs(distance) > fabs(bbdist);} ///<Return true if `bbdist` is smaller in absolute value.
    const XY &OnMe() const {return point_on_me;}                 ///<Return the point on me
    const XY &OnOther() const {return point_on_other;}           ///<Return the point on the other shape
    double Distance() const {return distance;}                   ///<Return the distance.
    bool IsValid() const {return distance != MaxVal(distance);}  ///<Return true if any distance is merged in.
    bool IsZero() const {return distance==0;}                    ///<Return if distance is zero.
    void ClearInOut() {was_inside = was_outside = false;}        ///<Clear flags of previous history.
    void MergeInOut(double dist) {(dist<0 ? was_inside : was_outside) = true;} ///<Set the inside or outside flag depending on `dist`. Do not change `distance`.
    bool WasIn() const {return was_inside;}     ///<Did we have someone inside.
    bool WasOut() const {return was_outside;}   ///<Did we have someone outside.
    void MakeAllInside() {was_outside = false; was_inside = true; distance = -fabs(distance);}  ///<Force all history to be in the inside.
    void MakeAllOutside() {was_outside = true; was_inside = false; distance = fabs(distance);}  ///<Force all history to be in the inside.
    void SwapInOut() {std::swap(was_inside, was_outside); distance *= -1;}  ///<Change in/outside history.
};

/** @} */ //addtogroup contour_internal 

inline void DistanceType::Merge(double d, const XY &p_on_me, const XY &p_on_other)
{
    if (IsZero() || fabs(d) == MaxVal(d)) return;
    if (d>0) was_outside = true;
    else if (d<0) was_inside = true;
    if (fabs(distance) <= fabs(d)) return;
    distance = d;
    point_on_me = p_on_me;
    point_on_other = p_on_other;
}

inline void DistanceType::Merge(const DistanceType &o)
{
    if (o.was_inside) was_inside = true;
    if (o.was_outside) was_outside = true;
    Merge(o.distance, o.point_on_me, o.point_on_other);
}



} //namespace

#endif //CONTOUR_DISTANCE_H