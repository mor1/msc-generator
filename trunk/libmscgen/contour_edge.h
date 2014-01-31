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

/** @file contour_edge.h Declares class Edge.
 * @ingroup contour_files
 */

#if !defined(CONTOUR_H)
#define CONTOUR_H

#include <map>
#include <vector>
#include "cairo.h"
#include "contour_distance.h"

namespace contour {

/** @addtogroup contour_internal
 * @{
 */


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
struct RayAngle {
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
    explicit RayAngle(double a, double b=0) : angle(a), curve(b) {}
    bool IsSimilar(const RayAngle&o) const {return test_equal(angle, o.angle) && test_equal(curve, o.curve);}  ///<Compares two RayAngles
    bool Smaller(const RayAngle&o) const {return test_equal(angle, o.angle) ? test_smaller(curve, o.curve) : angle < o.angle;} ///<True we are smaller than `o`, that is, `o` is clockwise after us.
};

/** @} */ //addtogroup contour_internal

/** Describew how edge joints shall be handled at an Expand operation.
 * @ingroup contour
 *
 * This has effect only if the join is expansive, that is for
 * - a convex vertex (with inner angle <180 degrees) at expansion (with a positive value).
 * - a concave vertex (with inner angle >180 degrees) at shrinkage (negative value).
 */
enum EExpandType {
    /** The simplest: continue edges until they meet, if they dont: add a direct segment.
     *
     * This method uses `miter_limit`, to limit the length of the tip for very
     * narrow angle vertices. `miter_limit` is understood as a multiple of the expansion
     * gap: the length of edge increases at most `miter_limit*gap`.
     *
     * If we encounter parallel edges, which do not cross even if linearly extended,
     * we still produce a very long miter (at 5 degrees), see the right example below.
     * Below see two edges with dots, and their and expanded version in solid lines.
     * The right example shows a straight edge and a half circle meeting. (The
     * straight edge being a tangent of the half circle.)
     * @verbatim
        -->-----+         ----->-----------
                |                          \------------
        .>..+   |          ..>...+                      \----+
            :   |               :                 ----------/
            :   |              :      +----------/
            :   |             :      /
            :   |             :      |
       @endverbatim
     * If there is a miter_limit in effect the right figure becomes like this
     * @verbatim
                          ----->-----------
                                           \--------+
                           ..>...+                  |
                                :                 --+
                               :      +----------/
                              :      /
                              :      |
       @endverbatim     */
    EXPAND_MITER,
    /** Same as EXPAND_MITER, but for parallel edges add a round join.
     *
     * Below see two edges with dots, and their and expanded version in solid lines.
     * The right example shows a straight edge and a half circle meeting.
     * @verbatim
        -->-----+         ----->------+_
                |                       \
        .>..+   |          ..>...+       |
            :   |               :      _/
            :   |              :      +
            :   |             :      /
            :   |             :      |
       @endverbatim */
    EXPAND_MITER_ROUND,
    /** Same as EXPAND_MITER, but for parallel edges add a flat/direct join.
     *
     * Below see two edges with dots, and their and expanded version in solid lines.
     * The right example shows a straight edge and a half circle meeting.
     * @verbatim
        -->-----+         ----->------+
                |                     |
        .>..+   |          ..>...+    |
            :   |               :     |
            :   |              :      +
            :   |             :      /
            :   |             :      |
       @endverbatim */
    EXPAND_MITER_BEVEL,
    /** Same as EXPAND_MITER, but for parallel edges add a square.
     *
     * Below see two edges with dots, and their and expanded version in solid lines.
     * The right example shows a straight edge and a half circle meeting.
     * @verbatim
        -->-----+         ----->----------+
                |                         |
        .>..+   |          ..>...+        |
            :   |               :         |
            :   |              :      +---+
            :   |             :      /
            :   |             :      |
       @endverbatim */
    EXPAND_MITER_SQUARE, ///<Same as EXPAND_MITER, but for parallel edges add a square.
    /** At the line join add a circle to connect the two edges.
     *
     * Below see two edges with dots, and their and expanded version in solid lines.
     * The right example shows a straight edge and a half circle meeting.
     * @verbatim
        -->-+-           ----->------+_
               \                        \
        .>..+   +          ..>...+       |
            :   |               :      _/
            :   |              :      +
            :   |             :      /
            :   |             :      |
       @endverbatim */
    EXPAND_ROUND,
    /** <At the line join add a direct line to connect the two edges.
     *
     * Below see two edges with dots, and their and expanded version in solid lines.
     * The right example shows a straight edge and a half circle meeting.
     * @verbatim
        -->-+            ----->-------+
              \                       |
        .>..+   +          ..>...+    |
            :   |               :     |
            :   |              :      +
            :   |             :      /
            :   |             :      |
       @endverbatim */
    EXPAND_BEVEL
};

inline bool IsMiter(EExpandType t) { return t==EXPAND_MITER || t==EXPAND_MITER_BEVEL || t==EXPAND_MITER_ROUND|| t==EXPAND_MITER_SQUARE; }
inline bool IsRound(EExpandType t) { return t==EXPAND_ROUND || t==EXPAND_MITER_ROUND; }
inline bool IsBevel(EExpandType t) { return t==EXPAND_BEVEL || t==EXPAND_MITER_BEVEL; }

/** Return which of two points are clockwise if seen from a third point.
*
* @param [in] from Seen from this point...
* @param [in] A ...is this or ...
* @param [in] B ...this point is in the clockwise direction.
* @param [in] clockwise If false we return the one in counterclockwise direction.
* @return `A` or `B` depending on which we selected.
*/
inline const XY &minmax_clockwise(const XY &from, const XY &A, const XY &B, bool clockwise)
{
    return (clockwise == (CLOCKWISE == triangle_dir(from, A, B))) ? B : A;
}


enum ELineCrossingType
{
    LINE_CROSSING_PARALLEL,  ///< No crossing, the two sections are parallel.
    LINE_CROSSING_INSIDE,    ///< Real crossing, the crosspoint is inside both sections.
    LINE_CROSSING_OUTSIDE_FW,///< The line of the two lines cross, but the crosspoint is outside at least one of the sections - towards the right end. (beyond B and/or M)
    LINE_CROSSING_OUTSIDE_BK ///< The line of the two lines cross, but the crosspoint is outside at least one of the sections - towards the wrong end. (beyond A and/or N)
};

ELineCrossingType crossing_line_line(const XY &A, const XY &B, const XY &M, const XY &N, XY &r);


inline XY Mid(const XY &A, const XY &B, double t)
{
    return A+t*(B-A);
}

class Edge
{
    friend void contour_test(void);
public:
    /** Describes, how two expanded edges can relate to each other.
    */
    enum EExpandCPType
    {
        DEGENERATE,     ///<One of the edges have `start==end`.
        TRIVIAL,        ///<The crosspoint is exactly at the end of the edge - used between segments of an edge when an edge needed to be split to several segments at expansion

        /** They have a crosspoint
         *
         * Usually this happens at concave vertices (or when we srink).
         * Below see two edges with dots, the expanded edges with solid lines. The `o` marks the crosspoint.
         * Note the edge direction!
         * for clarification you can see the whole shape to the right in small. The edges in question
         * are in dots there.
         * @verbatim
            ..<.....+
                |   :
            -<--o---:         +-------+
                |   :         |       |
                |   :         +...+   |
                |   :             :   |
                |   :             +---+
           @endverbatim */
        CP_REAL,
        /** They do not have a crosspoint, but if we extend both of them, they have.
         *
         * Note that "extension" for means continuation in a straight line along the tangent.
         *
         * Below see two edges with dots, the expanded edges with solid lines and their extension
         * also using dots. The `o` marks the crosspoint of the extensions.
         * @verbatim
            -->--...o
                    :
            .>..+   :
                :   |
                :   |
                :   |
                :   |
           @endverbatim */
        CP_EXTENDED,
        /** They are parallel and hence have no crosspoints.
         *
         * In this case there is no crosspoint neither if extended nor if linearly extrapolated.
         * The best example is below: a straight edge meets a half-circle.
         * (The straight edge being a tangent of the half circle.)
         * In this case linear extension of the half circle is still parallel to the
         * straight edge, while normal extension (not shown) actually diverges.
         * No potential crosspoint can be identified here, so what we return is a
         * point in between the two edges, currently 5 times further than the distance
         * between the two `x`s. See `o` below.
         * `x` shows the end of the expanded edges and solid lines show linear
         * extensions.
         * @verbatim
            ----->-x------------------

             ..>...+                              o
                  :
                 :      -x----------------
                :      /
                :      |
           @endverbatim */
        NO_CP_PARALLEL,
        /** The two edges fo not meet, but their extension do - but at the opposite end
         * This may usually happen either at expanding a concave vertex or shrinking a convex one.
         * 
         * On the figure below we see 4 edges (with dashes and pipe symbols, vertices
         * use + signs). We shrink considerably. The shrinked version of edges 1 and 2
         * do not cross and only their extension do. However, this extended cp (marked
         * with an X) is beyond the wrong (bottom) end of edge 2. Originally the cp 
         * between edges 1 and 2 was at the top end of edge 2, but after expansion it 
         * got to the other end - this makes the expanded edge of 2 sort of 
         * unnecessary to construct the expanded (shrunken) contour. So we mark this
         * case separately.
         *
         * @verbatim
           -->-1---------+
                  :      |2
                  :      |
                  :      +->---------3----+
                                  :       |
           .......X......         :       V
                                  :       |
                        ..........:.......|
                                  :       4
                                  :       |
                                
           @endverbatim */
        CP_INVERSE
        };
    ///<True if two expanded edges or their extrension has crosspoints.
    static bool HasCP(EExpandCPType t) { return t==CP_REAL || t==CP_EXTENDED || t==TRIVIAL; }

    enum {MAX_CP=9};

protected:
    friend class SimpleContour;
    friend class ContoursHelper;
    friend void contour_test_bezier(unsigned num);
    struct CrossResult
    {
        XY r;
        double pos_my;
        double pos_other;
    };
protected:
    bool straight : 1;
public:
    mutable bool visible : 1;
protected:
    XY start;
    XY end;
    XY c1;
    XY c2;
public:
    Edge(const XY &A, const XY &B, bool v=true)
        :straight(true), visible(v), start(A), end(B) {}
    Edge(const XY &A, const XY &B, const XY &C, const XY &D, bool v=true)
        :straight(false), visible(v), start(A), end(B), c1(C), c2(D) {}
    Edge() = default;
    Edge(const Edge &) = default;
    Edge(const Edge &e, double t, double s) : Edge(e) { Chop(t, s); }
    Edge & operator = (const Edge &) = default;
    /** Only checks shape, not visibility */
    bool operator ==(const Edge& p) const { return start==p.start && end==p.end && straight==p.straight && (!straight || (c1==p.c1 && c2==p.c2)); }
    /** Only checks shape, not visibility */
    bool operator < (const Edge& p) const { return start!=p.start ? start<p.start : end!=p.end ? end<p.end : straight!=p.straight ? straight ? true : p.straight ? false : c1!=p.c1 ? c1<p.c1 : c2<p.c2 : false; }

    const XY & GetStart() const { return start; } ///<Returns the startpoint.
    const XY & GetEnd() const { return end; }     ///<Returns the endpoint.

    XY Pos2Point(double pos) const { return straight ? Mid(start, end, pos) : Split(pos); }
    int WhichSide(const XY &A, const XY &B) const;
    bool OverlapConvexHull(const XY&A, const XY&B, const XY&C, const XY&D) const;

protected:
    bool IsStraight() const { return straight; }
    XY Split(double t) const;
    XY Split2(double t) const
    {
        return pow(1-t, 3) * start
            + 3 * pow(1-t, 2) * t * c1
            + 3 * (1-t) * pow(t, 2) * c2
            + pow(t, 3) * end;
    }
    XY Split(double t, XY &r1, XY &r2) const;
    XY Split() const { XY a, b; return Split(a, b); }
    XY Split(XY &r1, XY &r2) const;
    void Split(Edge &r1, Edge &r2) const;
    void Split(double t, Edge &r1, Edge &r2) const;
    bool Chop(double t, double s);
    double Flatness() const;
    bool HullOverlap(const Edge &) const;
    bool HullOverlap2(const Edge &) const;

    unsigned CrossingSegments(const Edge &o, XY r[], double pos_my[], double pos_other[]) const;
    unsigned CrossingBezier(const Edge &A, XY r[], double pos_my[], double pos_other[],
                            double pos_my_mul, double pos_other_mul,
                            double pos_my_offset, double pos_other_offset) const;
    unsigned CrossingVerticalBezier(double x, double y[], double pos[], bool forward[],
                                    double pos_mul, double pos_offset) const;
    double HullDistance(const XY &A, const XY &B) const;
    double HullDistance(const XY &A, const XY &B, const XY &C, const XY &D) const;

    unsigned SolveForDistance(const XY &p, double ret[5]) const;
    unsigned SolveForDistance1(const XY &p, double[5]) const;
    unsigned SolveForDistance2(const XY &p, double[5]) const;

    double FindBezierParam(const XY &p) const;

    Edge& SetStart(const XY &p, double pos);
    Edge& SetEnd(const XY &p, double pos);
    Edge& SetStartIgn(const XY &p, double pos);
    Edge& SetEndIgn(const XY &p, double pos);
    Edge& SetStartEndIgn(const XY &s, const XY &d, double spos, double dpos);

    //Helpers for expand
    EExpandCPType FindExpandedEdgesCP(const Edge &M, XY &newcp, double &my_pos, double &M_pos) const;

public:
    Edge &Shift(const XY&p) { start += p; end += p; if (!straight) { c1 += p; c2 += p; } return *this; }
    Edge CreateShifted(const XY&p) const { return Edge(*this).Shift(p); }
    Edge &Scale(double sc) { start *= sc; end *= sc; if (!straight) { c1 *= sc; c2 *= sc; } return *this; }    ///<Scale the edge.
    Edge CreateScaled(double sc) { return Edge(*this).Scale(sc); }    ///<Scale the edge.
    Edge &Scale(const XY &sc) { start.Scale(sc); end.Scale(sc); if (!straight) { c1.Scale(sc); c2.Scale(sc); } return *this; }    ///<Scale the edge.
    Edge CreateScaled(const XY &sc) { return Edge(*this).Scale(sc); }    ///<Scale the edge.
    Edge &Rotate(double cos, double sin) { start.Rotate(cos, sin); end.Rotate(cos, sin); if (!straight) { c1.Rotate(cos, sin); c2.Rotate(cos, sin); } return *this; }
    Edge CreateRotated(double cos, double sin) { return Edge(*this).Rotate(cos, sin); }
    Edge &RotateAround(const XY&c, double cos, double sin) { start.RotateAround(c, cos, sin); end.RotateAround(c, cos, sin); if (!straight) { c1.RotateAround(c, cos, sin); c2.RotateAround(c, cos, sin); } return *this; }
    Edge CreateRotatedAround(const XY&c, double cos, double sin) { return Edge(*this).RotateAround(c, cos, sin); }
    Edge &SwapXY() { start.SwapXY(); end.SwapXY(); if (!straight) { c1.SwapXY(); c2.SwapXY(); } return *this; }
    Edge CreateSwapXYd() { return Edge(*this).SwapXY(); }
    Edge &Invert() { std::swap(start, end); if (!straight) std::swap(c1, c2); return *this; } ///<Reverses the direction of the edge.

    //returns a point on the line of a tangent at "pos", the point being towards the start of curve/edge.
    XY PrevTangentPoint(double pos) const { return straight ? 2*start-end : pos ? Mid(Mid(start, c1, pos), Mid(c1, c2, pos), pos) : 2*start-c1; }
    //returns a point on the line of a tangent at "pos", the point being towards the end of curve/edge.
    XY NextTangentPoint(double pos) const { return straight ? 2*end-start : pos<1 ? Mid(Mid(c1, c2, pos), Mid(c2, end, pos), pos) : 2*end-c2; }

    bool CheckAndCombine(const Edge &next, double *pos=NULL); 

    unsigned Crossing(const Edge &A, XY r[Edge::MAX_CP], double pos_my[Edge::MAX_CP], double pos_other[Edge::MAX_CP]) const;
    int CrossingVertical(double x, double y[3], double pos[3], int cross_dir[3]) const;
    RayAngle Angle(bool incoming, double pos) const;
    Block CreateBoundingBox() const; ///<Returns a copy of the bounding box of the edge

    double Distance(const XY &, XY &point, double &pos) const; //always nonnegative
    DistanceType Distance(const Edge &o) const { DistanceType ret; Distance(o, ret); return ret; }    //always nonnegative
    void Distance(const Edge &, DistanceType &ret) const;    //always nonnegative

    double GetAreaAboveAdjusted() const;
    double GetLength() const; ///<Returns the length of the arc.
    //returns the centroid of the area above multipled by the (signed) area above
    XY GetCentroidAreaAboveUpscaled() const;

    void CreateExpand2D(const XY &gap, std::vector<Edge> &ret, int &stype, int &etype) const;

    //helpers for offsetbelow
    double OffsetBelow(const Edge &M, double &touchpoint) const;

    //tangential toucher from a point
    void TangentFrom(const XY &from, XY &clockwise, XY &cclockwise) const;
    void TangentFrom(const Edge &from, XY clockwise[2], XY cclockwise[2]) const;

    void   PathTo(cairo_t *cr) const { if (straight) cairo_line_to(cr, end.x, end.y); else cairo_curve_to(cr, c1.x, c1.y, c2.x, c2.y, end.x, end.y); } ///<Adds the edge to a cairo path. * It assumes cairo position is at `start`.
    void PathDashed(cairo_t *cr, const double pattern[], unsigned num, int &pos, double &offset, bool reverse = false) const;

    static void GenerateEllipse(std::vector<Edge> &append_to, const XY &c, double radius_x, double radius_y=0, 
                                double tilt_deg=0, double s_deg=0, double d_deg=0, bool clockwise=true);
    template<class Container>
    void CreateExpand(double gap, Container &expanded, std::vector<Edge> *original = NULL) const;

    unsigned atX(double x, double roots[3]) const;
};

//this is very small in release mode. If straight, only an assignment and "pos" need not be calculated
inline Edge& Edge::SetStart(const XY &p, double pos)
{
    if (!straight) {
        XY dummy;
        double t;
        //test that p and pos correspond - compiles to NOP in release mode
        _ASSERT(fabs(Distance(p, dummy, t))<0.01);
        _ASSERT(fabs(t-pos)<0.001);
        Chop(pos, 1);
    } else {
        //test that p and pos correspond - compiles to NOP in release mode
        if (fabs(start.x-end.x)<fabs(start.y-end.y))
            _ASSERT(fabs((p.y-start.y)/(end.y-start.y) - pos)<0.0001);
        else
            _ASSERT(fabs((p.x-start.x)/(end.x-start.x) - pos)<0.0001);
    }
    start = p;
    return *this;
}

//this is very small in release mode. If straight, only an assignment and "pos" need not be calculated
inline Edge& Edge::SetEnd(const XY &p, double pos)
{
    if (!straight) {
        XY dummy;
        double t;
        //test that p and pos correspond - compiles to NOP in release mode
        _ASSERT(fabs(Distance(p, dummy, t))<0.01);
        _ASSERT(fabs(t-pos)<0.001);
        Chop(0, pos);
    } else {
        //test that p and pos correspond - compiles to NOP in release mode
        if (fabs(start.x-end.x)<fabs(start.y-end.y))
            _ASSERT(fabs((p.y-start.y)/(end.y-start.y) - pos)<0.0001);
        else
            _ASSERT(fabs((p.x-start.x)/(end.x-start.x) - pos)<0.0001);
    }
    end = p;
    return *this;
}

//this is very small in release mode. If straight, only an assignment and "pos" need not be calculated
//This version ignores the "pos" completely for staright lines even in debug mode
inline Edge& Edge::SetStartIgn(const XY &p, double pos)
{
    if (!straight) {
        XY dummy;
        double t;
        //test that p and pos correspond - compiles to NOP in release mode
        _ASSERT(fabs(Distance(p, dummy, t))<0.01);
        _ASSERT(fabs(t-pos)<0.001);
        Chop(pos, 1);
    }
    start = p;
    return *this;
}

//this is very small in release mode. If straight, only an assignment and "pos" need not be calculated
//This version ignores the "pos" completely for staright lines even in debug mode
inline Edge& Edge::SetEndIgn(const XY &p, double pos)
{
    if (!straight) {
        XY dummy;
        double t;
        //test that p and pos correspond - compiles to NOP in release mode
        _ASSERT(fabs(Distance(p, dummy, t))<0.01);
        _ASSERT(fabs(t-pos)<0.001);
        Chop(0, pos);
    }
    end = p;
    return *this;
}

inline Edge& Edge::SetStartEndIgn(const XY &s, const XY &d, double spos, double dpos)
{
    if (!straight) {
        XY dummy;
        double tt, ss;
        //test that p and pos correspond - compiles to NOP in release mode
        _ASSERT(fabs(Distance(s, dummy, tt))<0.01);
        _ASSERT(fabs(Distance(d, dummy, ss))<0.01);
        _ASSERT(fabs(tt-spos)<0.001);
        _ASSERT(fabs(ss-dpos)<0.001);
        Chop(spos, dpos);
    }
    start = s;
    end = d;
    return *this;
}



/** Expands the edge.
*
* This takes the direction of the edge to determine the 'outside' side of the
* edge to expand towards (or away from in case of a negative `gap`).
* Destroys bounding box!
* @param [in] gap The amount to expand (or shrink if <0)
* @param [out] expanded Append the expanded edges to this
* @param [out] original Append the original edge to this. If the expansion
*                       requires to split the edge to several pieces append
*                       the split chunks of the original here.
*/
template<class Container>
void Edge::CreateExpand(double gap, Container &expanded, std::vector<Edge> *original) const
{
    if (straight) {
        const double length = start.Distance(end);
        const XY wh = (end-start).Rotate90CCW()/length*gap;
        expanded.emplace_back(start+wh, end+wh, !!visible); //Visual Studio complains for bitfields in such templates
        if (original)
            original->push_back(*this);
        return;
    }
    //Tiller-Hansson: expand the start,c1,c2,end polygon.
    const double l1 = start.Distance(c1);
    const double l2 = c1.Distance(c2);
    const double l3 = c2.Distance(end);
    const XY wh1 = (c1-start).Rotate90CCW()/l1*gap;
    const XY wh2 = (c2-c1).Rotate90CCW()/l2*gap;
    const XY wh3 = (end-c2).Rotate90CCW()/l3*gap;
    XY new_c1, new_c2;
    if (LINE_CROSSING_PARALLEL == crossing_line_line(start+wh1, c1+wh1, c1+wh2, c2+wh2, new_c1))
        _ASSERT(0);
    if (LINE_CROSSING_PARALLEL == crossing_line_line(c1+wh2, c2+wh2, c2+wh3, end+wh3, new_c2))
        _ASSERT(0);
    expanded.emplace_back(start+wh1, end+wh3, new_c1, new_c2, !!visible); //Visual Studio complains for bitfields in such templates
    if (original)
        original->push_back(*this);
}

} //namespace contour bezier



#endif //CONTOUR_H
