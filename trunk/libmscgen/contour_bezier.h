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

/** @file contour_bezier.h Declares Bezier calculation code
* @ingroup contour_files
*/

#if !defined(CONTOUR_BEZIER_H)
#define CONTOUR_BEZIER_H

#include "cairo.h"
#include "contour_distance.h"

namespace contour2 {

    using contour::XY;
    using contour::Block;
    using contour::Range;
    using contour::DistanceType;
    using contour::test_zero;
    using contour::test_positive;
    using contour::test_equal;
    using contour::test_smaller;

/** A helper class describing how an edge arrives/leaves a crosspoint.
*
* This is used to decide, which edge to continue the walk at contour
* intersect/union operations. For straight edges this is just a direction,
* but for curvy edges, we also need the radius of the curve, to differentiate
* between curvy edges that have the same tangent, but different radius.
* We do not store a very precise angle here, this is just used to sort
* rays at a crosspoint in clockwise order.
* Note that the direction of the ray (incoming or outgoing) plays no
* role in determining this angle. (This is since each edge crossing the crosspoint
* will have 2 rays added for it, one incoming one outgoing and both will
* have an angle (each as if the were outgoing rays, so to speak.)
*/
struct RayAngle
{
    /** The false angle [0..4], each integer corresponds to 90 degrees.
    *
    * In order to save computation we do not store the radian of the direction
    * merely its sine, since we only do sorting. We call this *fake angle*.
    * Values between [0..1] thus correspond to radians [0..PI/2], values
    * between [1..2] to radians [PI/2..PI], etc.
    * Note that the radian 0 is towards growing x axis ("right" you can say)
    * and increasing radian is clockwise (in a space where y grows "downwards").
    */
    double angle;
    /** The curvature of the angle.
    *
    * 0 is used for straight edges, positive values if the edge curves toward
    * clockwise, larger positive values the smaller the turn radius is.
    */
    double curve;
    RayAngle() {};
    explicit RayAngle(double a, double b = 0) : angle(a), curve(b) {}
    bool IsSimilar(const RayAngle&o) const { return test_equal(angle, o.angle) && test_equal(curve, o.curve); }  ///<Compares two RayAngles
    bool Smaller(const RayAngle&o) const { return test_equal(angle, o.angle) ? test_smaller(curve, o.curve) : angle < o.angle; } ///<True we are smaller than `o`, that is, `o` is clockwise after us.
};



class Edge
{
public:

    struct CrossResult {
        XY r;
        double pos_my;
        double pos_other;
    };

    bool straight : 1;
    bool visible : 1;
    XY start;
    XY end;
    XY c1;
    XY c2;
    Edge(const XY &A, const XY &B)
        :straight(true), visible(true), start(A), end(B) {}
    Edge(const XY &A, const XY &B, const XY &C, const XY &D)
        :straight(false), visible(true), start(A), end(B), c1(C), c2(D) {}
    Edge() = default;
    Edge(const Edge &) = default;
    Edge & operator = (const Edge &) = default;
    /** Only checks shape, not visibility */
    bool operator ==(const Edge& p) const { return start==p.start && end==p.end && straight==p.straight && (!straight || (c1==p.c1 && c2==p.c2)); }
    /** Only checks shape, not visibility */
    bool operator < (const Edge& p) const { return start!=p.start ? start<p.start : end!=p.end ? end<p.end : straight!=p.straight ? straight ? true : p.straight ? false : c1!=p.c1 ? c1<p.c1 : c2<p.c2 : false; }

    const XY & GetStart() const { return start; } ///<Returns the startpoint.
    const XY & GetEnd() const { return end; }     ///<Returns the endpoint.

//protected:
    XY Split(double t) const;
    XY Split(double t, XY &r1, XY &r2) const;
    XY Split() const { XY a, b; return Split(a, b); }
    XY Split(XY &r1, XY &r2) const;
    void Split(Edge &r1, Edge &r2) const;
    void Split(double t, Edge &r1, Edge &r2) const;
    double Flatness() const;
    bool HullOverlap(const Edge &) const;

    unsigned CrossingBezier(const Edge &A, XY r[], double pos_my[], double pos_other[], 
                            double pos_my_mul, double pos_other_mul, 
                            double pos_my_offset, double pos_other_offset) const;
    unsigned CrossingVerticalBezier(double x, double y[], double pos[], bool forward[],
                                    double pos_mul, double pos_offset) const;


public:
    Edge &Shift(const XY&p) { start += p; end += p; if (!straight) { c1 += p; c2 += p; } return *this; }
    Edge CreateShifted(const XY&p) const { return Edge(*this).Shift(p); }
    Edge &Scale(double sc) { start *= sc; end *= sc; if (!straight) { c1 *= sc; c2 *= sc; } return *this; }    ///<Scale the edge.
    Edge CreateScaled(double sc) { return Edge(*this).Scale(sc); }    ///<Scale the edge.
    Edge &Rotate(double cos, double sin) { start.Rotate(cos, sin); end.Rotate(cos, sin); if (!straight) { c1.Rotate(cos, sin); c2.Rotate(cos, sin); } return *this; }
    Edge CreateRotated(double cos, double sin) { return Edge(*this).Rotate(cos, sin); }
    Edge &RotateAround(const XY&c, double cos, double sin) { start.RotateAround(c, cos, sin); end.RotateAround(c, cos, sin); if (!straight) { c1.RotateAround(c, cos, sin); c2.RotateAround(c, cos, sin); } return *this; }
    Edge CreateRotatedAround(const XY&c, double cos, double sin) { return Edge(*this).RotateAround(c, cos, sin); }
    Edge &SwapXY() { start.SwapXY(); end.SwapXY(); if (!straight) { c1.SwapXY(); c2.SwapXY(); } return *this; }
    Edge CreateSwapXYd() { return Edge(*this).SwapXY(); }
    Edge &Invert() { std::swap(start, end); if (!straight) std::swap(c1, c2); return *this; } ///<Reverses the direction of the edge.

    void   PathTo(cairo_t *cr) const { if (straight) cairo_line_to(cr, end.x, end.y); else cairo_curve_to(cr, c1.x, c1.y, c2.x, c2.y, end.x, end.y); } ///<Adds the edge to a cairo path. * It assumes cairo position is at `start`.

    unsigned Crossing(const Edge &A, XY r[], double pos_my[], double pos_other[]) const;
    int CrossingVertical(double x, double y[], double pos[], bool forward[]) const;
    RayAngle Angle(bool incoming, double pos) const;
    Block CreateBoundingBox() const; ///<Returns a copy of the bounding box of the edge

    double Distance(const XY &, XY &point, double &pos) const; //always nonnegative
    DistanceType Distance(const Edge &) const;    //always nonnegative




    double GetAreaAboveAdjusted() const;
    double GetLength() const; ///<Returns the length of the arc.
    //returns the centroid of the area above multipled by the (signed) area above
    XY GetCentroidAreaAboveUpscaled() const;

    bool Expand(double gap);
//    void CreateExpand2D(const XY &gap, std::vector<Edge> &ret, int &stype, int &etype) const;

    void   PathDashed(cairo_t *cr, const double pattern[], unsigned num, int &pos, double &offset, bool reverse = false) const;

    //helpers for offsetbelow
    double OffsetBelow(const Edge &M, double &touchpoint) const;

    //tangential toucher from a point
    bool TangentFrom(const XY &from, XY &clockwise, XY &cclockwise) const;
    bool TangentFrom(const Edge &from, XY clockwise[2], XY cclockwise[2]) const;
};



}; //namespace

#endif //CONTOUR_BEZIER_H