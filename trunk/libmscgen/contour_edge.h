/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011,2012,2013,2014 Zoltan Turanyi
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
#include <list>
#include "cairo.h"
#include "contour_distance.h"

namespace contour {


#define CP_PRECISION (1e-8)

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
     * This is actually 1/radius.
     * 0 is used for straight edges, positive values if the edge curves toward
     * clockwise, larger positive values the smaller the turn radius is.
     */
    double curve;
    RayAngle() = default;
    explicit RayAngle(double a, double b=0) : angle(a), curve(b) {}
    bool IsSimilar(const RayAngle&o) const {return test_equal(angle, o.angle) && test_equal(curve, o.curve);}  ///<Compares two RayAngles
    bool Smaller(const RayAngle&o) const {return test_equal(angle, o.angle) ? test_smaller(curve, o.curve) : angle < o.angle;} ///<True we are smaller than `o`, that is, `o` is clockwise after us. We do not define operator < since this is not strict, but approximate.
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

/** Returns true if t is one of EXPAND_MITER_XXX */
inline bool IsMiter(EExpandType t) { return t==EXPAND_MITER || t==EXPAND_MITER_BEVEL || t==EXPAND_MITER_ROUND|| t==EXPAND_MITER_SQUARE; }
/** Returns if t dictates a round join for parallel edge endings. */
inline bool IsRound(EExpandType t) { return t==EXPAND_ROUND || t==EXPAND_MITER_ROUND; }
/** Returns if t dictates a bevelled join for parallel edge endings. */
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

/** Describes how two sections can cross each other.*/
enum ELineCrossingType
{
    LINE_CROSSING_PARALLEL,  ///< No crossing, the two sections are parallel.
    LINE_CROSSING_INSIDE,    ///< Real crossing, the crosspoint is inside both sections.
    LINE_CROSSING_OUTSIDE_FW,///< The line of the two lines cross, but the crosspoint is outside at least one of the sections - towards the right end. (beyond B and/or M)
    LINE_CROSSING_OUTSIDE_BK ///< The line of the two lines cross, but the crosspoint is outside at least one of the sections - towards the wrong end. (beyond A and/or N)
};

ELineCrossingType crossing_line_line(const XY &A, const XY &B, const XY &M, const XY &N, XY &r);


/** Returns the point on position 't' of the section A->B. 
 * 't' must be [0..1], 0 returns 'A', 1 returns 'B', 0.5 returns their midpoint.*/
inline XY Mid(const XY &A, const XY &B, double t)
{
    return A+t*(B-A);
}

/** Holds either a straight line or a cubic bezier. 
 * May be visible or invisible. */
class Edge
{
    friend void contour_test(void);
public:
    /** Describes, how two expanded edges can relate to each other.
    */
    enum EExpandCPType
    {
        CP_DEGENERATE,     ///<One of the edges have `start==end`.
        CP_TRIVIAL,        ///<The crosspoint is exactly at the end of the edge - used between segments of an edge when an edge needed to be split to several segments at expansion

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
         /** They do not have a crosspoint, but if we extend them, they have
          *
          * Note that "extension" for a straight edge means its a continuation in a straight line,
          * while for a curvy edge it is the continuation of the circle/ellipse.
          * This is the most common case for expansion.
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
           * In this case there is no crosspoint not even if linearly extrapolated.
           * The main example is below. A straight edge meets a half-circle.
           * (The straight edge being a tangent of the half circle.)
           * In this case linear extension of the half circle is still parallel to the
           * straight edge.
           * No potential crosspoint can be identified here, so what we return is a
           * point in between the two edges, currently 5 times further than the distance
           * between the two `x`s. See `o` below. * `x` shows the end of the expanded edges
           * and solid lines show linear extensions.
           * @verbatim
            ----->-x------------------

             ..>...+                 o
                  :
                 :      -x----------------
                :      /
                :      |
           @endverbatim */
           NO_CP_PARALLEL,
           /** The two edges do not meet, but their extension do - but at the opposite end
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
    /** True if two expanded edges or their extrension has crosspoints.*/
    static bool HasCP(EExpandCPType t) { return t==CP_REAL || t==CP_EXTENDED || t==CP_TRIVIAL; }
    enum { MAX_CP = 9 ///<The maximum number of crosspoints two Edges may have.
    };

protected:
    friend class SimpleContour;
    friend class Path;
    friend class ContoursHelper;
    friend void contour_test_bezier(unsigned num);
protected:
    bool straight : 1;  ///<True if the Edge is a straight line (members c1 and c2 are to be ignored), false if a cubic bezier.
public:
    mutable bool visible : 1; ///<True if the edge is visible.
protected:
    XY start; ///<The startpoint of the Edge (first control point for cubic beziers)
    XY end;   ///<The endpoint of the Edge (fourth control point for cubic beziers)
    XY c1;    ///<If a cubic bezier, then the second control point - the one associated with 'start'
    XY c2;    ///<If a cubic bezier, then the third control point - the one associated with 'end'
public:
    /** Contsruct a straight edge. */
    Edge(const XY &A, const XY &B, bool v = true)
        :straight(true), visible(v), start(A), end(B) {}
    /** Construct a cubic bezier. Will be converted to straight line if too flat. */
    Edge(const XY &A, const XY &B, const XY &C, const XY &D, bool v = true)
        :straight(false), visible(v), start(A), end(B), c1(C), c2(D) { MakeStraightIfNeeded(); }
    Edge() = default;
    Edge(const Edge &) = default;
    /** Construct the Edge as a section of another one. 
     * Will be converted to straight line if too flat. 
     * @param [in] e The other edge.
     * @param [in] t The starting parameter of the section to cut out of 'e'. Must be in [0..s).
     * @param [in] s The ending parameter of the section to cut out of 'e'. Must be in (t..1].*/
    Edge(const Edge &e, double t, double s) : Edge(e) { Chop(t, s); MakeStraightIfNeeded(); }
    Edge & operator = (const Edge &) = default;
    /** Only checks shape, not visibility */
    bool operator ==(const Edge& p) const { return start==p.start && end==p.end && straight==p.straight && (!straight || (c1==p.c1 && c2==p.c2)); }
    /** Only checks shape, not visibility */
    bool operator < (const Edge& p) const { return start!=p.start ? start<p.start : end!=p.end ? end<p.end : straight!=p.straight ? straight ? true : p.straight ? false : c1!=p.c1 ? c1<p.c1 : c2<p.c2 : false; }

    const XY & GetStart() const { return start; } ///<Returns the startpoint.
    const XY & GetEnd() const { return end; }     ///<Returns the endpoint.
    const XY &GetC1() const { _ASSERT(!straight); return c1; } ///<Returns the second control point for cubic beziers.
    const XY &GetC2() const { _ASSERT(!straight); return c2; } ///<Returns the third control point for cubic beziers.
    bool IsStraight() const { return straight; } ///<True if the Edge is a straight line.
    bool IsDot() const { return start.test_equal(end) && (straight || (start.test_equal(c1) && end.test_equal(c2))); } ///<Ture if the edge is degenerated to a single point.

    /** Returns the point at position 'pos' on the Edge. 'pos' must be in [0..1]*/
    XY Pos2Point(double pos) const { return straight ? Mid(start, end, pos) : Split(pos); }

protected:
    /** Returns the point at position 'pos' of a bezier curve. 'pos' must be in [0..1]*/
    XY Split(double t) const
    {
        return pow(1-t, 3) * start
            + 3 * pow(1-t, 2) * t * c1
            + 3 * (1-t) * pow(t, 2) * c2
            + pow(t, 3) * end;
    }
    XY Split(double t, XY &r1, XY &r2) const;
    XY Split() const { XY a, b; return Split(a, b); } ///<Returns the midpoint of a bezier curve
    XY Split(XY &r1, XY &r2) const;
    void Split(Edge &r1, Edge &r2) const;
    void Split(double t, Edge &r1, Edge &r2) const;
    bool Chop(double t, double s);
    bool MakeStraightIfNeeded(double maximum_distance_sqr = 0.001);
    int WhichSide(const XY &A, const XY &B) const;
    bool HullOverlap(const Edge &, bool is_next) const;
    double HullDistance(const XY &A, const XY &B) const;
    double HullDistance(const XY &A, const XY &B, const XY &C, const XY &D) const;
    unsigned SolveForDistance(const XY &p, double ret[5]) const;
    double FindBezierParam(const XY &p) const;
    Block GetBezierHullBlock() const;
    Range GetHullXRange() const;
    Range GetHullYRange() const;
    template <bool use_y_coord>
    unsigned atXorY(double v, double roots[3], double precision = 0) const;
    template <bool use_y_coord>
    bool RefineXorY(double v, double &root, double precision = CP_PRECISION) const;

    unsigned CrossingSegments(const Edge &o, XY r[], double pos_my[], double pos_other[]) const;
    unsigned CrossingSegments_NoSnap(const Edge &o, XY r[], double pos_my[], double pos_other[]) const;
    unsigned CrossingLine(const XY &A, const XY &B, double pos_my[], double pos_segment[]) const;
    unsigned CrossingBezier(const Edge &A, XY r[], double pos_my[], double pos_other[],
                            double pos_my_mul, double pos_other_mul,
                            double pos_my_offset, double pos_other_offset, unsigned alloc_size) const;

    Edge& SetStart(const XY &p, double pos);
    Edge& SetEnd(const XY &p, double pos);
    Edge& SetStartIgn(const XY &p, double pos);
    Edge& SetEndIgn(const XY &p, double pos);
    Edge& SetStartEndIgn(const XY &s, const XY &d, double spos, double dpos);

    //Helpers for expand
    EExpandCPType FindExpandedEdgesCP(const Edge &M, XY &newcp,
                                      const XY &my_next_tangent, const XY &Ms_prev_tangent,
                                      bool is_my_origin_bezier, bool is_Ms_origin_bezier,
                                      double &my_pos, double &M_pos) const;
    template <typename E, typename Iterator>
    static void RemoveLoop(std::list<E> &edges, Iterator first, Iterator last, bool self=false,
                           std::vector<Edge>*original = NULL, size_t orig_offset = 0);
    void TangentFromRecursive(const XY &from, XY &clockwise, XY &cclockwise) const;

public:
    Edge &Shift(const XY&p) { start += p; end += p; if (!straight) { c1 += p; c2 += p; } return *this; }  ///<Shift the egde
    Edge CreateShifted(const XY&p) const { return Edge(*this).Shift(p); } ///<Create a shifted version of the edge
    Edge &Scale(double sc) { start *= sc; end *= sc; if (!straight) { c1 *= sc; c2 *= sc; } return *this; }    ///<Scale the edge.
    Edge CreateScaled(double sc) const { return Edge(*this).Scale(sc); }    ///<Scale the edge.
    Edge &Scale(const XY &sc) { start.Scale(sc); end.Scale(sc); if (!straight) { c1.Scale(sc); c2.Scale(sc); } return *this; }    ///<Scale the edge.
    Edge CreateScaled(const XY &sc) const { return Edge(*this).Scale(sc); }    ///<Scale the edge.
    Edge &Rotate(double cos, double sin) { start.Rotate(cos, sin); end.Rotate(cos, sin); if (!straight) { c1.Rotate(cos, sin); c2.Rotate(cos, sin); } return *this; } ///<Rotates the edge around the origin
    Edge CreateRotated(double cos, double sin) const { return Edge(*this).Rotate(cos, sin); } ///<Creates a rotated version of the edge
    Edge &RotateAround(const XY&c, double cos, double sin) { start.RotateAround(c, cos, sin); end.RotateAround(c, cos, sin); if (!straight) { c1.RotateAround(c, cos, sin); c2.RotateAround(c, cos, sin); } return *this; } ///<Rotates the edge around a point
    Edge CreateRotatedAround(const XY&c, double cos, double sin) const { return Edge(*this).RotateAround(c, cos, sin); } ///<Creates a version of the edge rotated around a point
    Edge &SwapXY() { start.SwapXY(); end.SwapXY(); if (!straight) { c1.SwapXY(); c2.SwapXY(); } return *this; } ///<Swaps XY coordinates
    Edge CreateSwapXYd() const { return Edge(*this).SwapXY(); } ///<Created a version of the edge with swapped x and y coordinates.
    Edge &Invert() { std::swap(start, end); if (!straight) std::swap(c1, c2); return *this; } ///<Reverses the direction of the edge effectively swapping start and end.

    /** Returns a point on the line of a tangent at "pos", the point being towards the start of curve/edge.*/
    XY PrevTangentPoint(double pos) const { return straight ? 2*start-end : pos ? Mid(Mid(start, c1, pos), Mid(c1, c2, pos), pos) : 2*start-c1; }
    /** Returns a point on the line of a tangent at "pos", the point being towards the end of curve/edge. */
    XY NextTangentPoint(double pos) const { return straight ? 2*end-start : pos<1 ? Mid(Mid(c1, c2, pos), Mid(c2, end, pos), pos) : 2*end-c2; }

    bool CheckAndCombine(const Edge &next, double *pos = NULL);

    unsigned Crossing(const Edge &A, bool is_next, XY r[Edge::MAX_CP], double pos_my[Edge::MAX_CP], double pos_other[Edge::MAX_CP]) const;
    unsigned SelfCrossing(XY r[Edge::MAX_CP], double pos1[Edge::MAX_CP], double pos2[Edge::MAX_CP]) const;
    int CrossingVertical(double x, double y[3], double pos[3], int cross_dir[3]) const;
    int CrossingHorizontalCPEvaluate(const XY &xy, bool self) const;
    RayAngle Angle(bool incoming, double pos) const;
    Block CreateBoundingBox() const; ///<Returns a copy of the bounding box of the edge
    XY XMaxExtreme(double &pos) const;

    double Distance(const XY &, XY &point, double &pos) const; //always nonnegative
    DistanceType Distance(const Edge &o) const { DistanceType ret; Distance(o, ret); return ret; } ///<Returns the distance of 'o' from 'this'.
    void Distance(const Edge &, DistanceType &ret) const; 

    double GetAreaAboveAdjusted() const; ///<The area above the edge (adjusted).
    double GetLength() const; ///<Returns the length of the arc.
    //returns the centroid of the area above multipled by the (signed) area above
    XY GetCentroidAreaAboveUpscaled() const;

    void CreateExpand2D(const XY &gap, std::vector<Edge> &ret, int &stype, int &etype) const;

    //helpers for offsetbelow
    double OffsetBelow(const Edge &M, double &touchpoint, double min_so_far) const;

    //tangential toucher from a point
    void TangentFrom(const XY &from, XY &clockwise, XY &cclockwise) const;
    void TangentFrom(const Edge &from, XY clockwise[2], XY cclockwise[2]) const;

    void   PathTo(cairo_t *cr) const { if (straight) cairo_line_to(cr, end.x, end.y); else cairo_curve_to(cr, c1.x, c1.y, c2.x, c2.y, end.x, end.y); } ///<Adds the edge to a cairo path. * It assumes cairo position is at `start`.
    void PathDashed(cairo_t *cr, const double pattern[], unsigned num, int &pos, double &offset, bool reverse = false) const;

    static void GenerateEllipse(std::vector<Edge> &append_to, const XY &c, double radius_x, double radius_y = 0,
                                double tilt_deg = 0, double s_deg = 0, double d_deg = 0, bool clockwise = true);
    bool CreateExpand(double gap, std::list<Edge> &expanded, XY &prev_tangent, XY &next_tangent, std::vector<Edge> *original = NULL) const;
    bool CreateExpandOneSegment(double gap, std::list<Edge> &expanded, std::vector<Edge> *original) const;

    /** Return a series of parameter values where the x coordinate of the curve is 'x'
    * @param[in] x The x coordinate in question.
    * @param[out] roots The parameter values returned.All will be in[0..1]
    * @param[in] precision The precision (in pixels) to apply when refining the position values.
    *            When it is zero or negative, no refinement happens(default).
    * @returns the number of crosses found. **/
    unsigned atX(double x, double roots[3], double precision = 0) const
    { return atXorY<false>(x, roots, precision); }
    /** Return a series of parameter values where the y coordinate of the curve is 'y'
    * @param[in] y The y coordinate in question.
    * @param[out] roots The parameter values returned.All will be in[0..1]
    * @param[in] precision The precision (in pixels) to apply when refining the position values.
    *            When it is zero or negative, no refinement happens(default).
    * @returns the number of crosses found. **/
    unsigned atY(double y, double roots[3], double precision = 0) const
    { return atXorY<true>(y, roots, precision); }
};

/** Changes a bezier curve to straight if it is too flat.
 * @param [in] maximum_distance_sqr The sum of the square distance of c1 and c2 from the start->end segment.
 *                                  If the actual distance is smaller than this, we switch to straight.
 * @returns true if we have switched from bezier to straight line.*/
inline bool Edge::MakeStraightIfNeeded(double maximum_distance_sqr) 
{
    _ASSERT(!isnan(start.x) && !isnan(start.y));
    _ASSERT(!isnan(end.x) && !isnan(end.y));
    _ASSERT(!isnan(c1.x) && !isnan(c1.y));
    _ASSERT(!isnan(c2.x) && !isnan(c2.y));

    //see http://antigrain.com/research/adaptive_bezier/
    if (straight) return false;
    const double dx = end.x-start.x;
    const double dy = end.y-start.y;
    const double d2 = fabs(((c1.x - end.x) * dy - (c1.y - end.y) * dx));
    const double d3 = fabs(((c2.x - end.x) * dy - (c2.y - end.y) * dx));

    return straight = (d2 + d3)*(d2 + d3) < maximum_distance_sqr * (dx*dx + dy*dy);
}

/** Set the start of the Edge thereby effectively chopping its head away.
 * We assume 'p' and 'pos' correspond to the same point and use the one resulting
 * in faster operation. This is very small processing when compiled in optimized mode. 
 * For straight edges, only an assignment and "pos" need not be calculated in the
 * caller context (as it is not used) due to inlining.*/
inline Edge& Edge::SetStart(const XY &p, double pos)
{
    if (!straight) {
#ifdef _DEBUG
        XY dummy;
        double t;
        //test that p and pos correspond - compiles to NOP in release mode
        _ASSERT(fabs(Distance(p, dummy, t))<0.1);
        //_ASSERT(fabs(t-pos)<0.001);
#endif
        Chop(pos, 1);
    } else {
        //test that p and pos correspond - compiles to NOP in release mode
        if (fabs(start.x-end.x)<fabs(start.y-end.y))
            _ASSERT(fabs((p.y-start.y)/(end.y-start.y) - pos)<0.0001);
        else {
            _ASSERT(fabs((p.x-start.x)/(end.x-start.x) - pos)<0.0001);
        }
    }
    start = p;
    return *this;
}

/** Set the end of the Edge thereby effectively chopping its tail away.
 * We assume 'p' and 'pos' correspond to the same point and use the one resulting
 * in faster operation. This is very small processing when compiled in optimized mode. 
 * For straight edges, only an assignment and "pos" need not be calculated in the
 * caller context (as it is not used) due to inlining.*/
inline Edge& Edge::SetEnd(const XY &p, double pos)
{
    if (!straight) {
#ifdef _DEBUG
        XY dummy;
        double t;
        //test that p and pos correspond - compiles to NOP in release mode
        _ASSERT(fabs(Distance(p, dummy, t))<0.1);
        //_ASSERT(fabs(t-pos)<0.001);
#endif
        Chop(0, pos);
    } else {
        //test that p and pos correspond - compiles to NOP in release mode
        if (fabs(start.x-end.x)<fabs(start.y-end.y))
            _ASSERT(fabs((p.y-start.y)/(end.y-start.y) - pos)<0.0001);
        else {
            _ASSERT(fabs((p.x-start.x)/(end.x-start.x) - pos)<0.0001);
        }
    }
    end = p;
    return *this;
}

/** Set the start of the Edge thereby effectively chopping its head away.
 * We assume 'p' and 'pos' correspond to the same point and use the one resulting
 * in faster operation. This is very small processing when compiled in optimized mode. 
 * For straight edges, only an assignment and "pos" need not be calculated in the
 * caller context (as it is not used) due to inlining.
 * This version ignores the "pos" completely for staright lines even in debug mode*/
inline Edge& Edge::SetStartIgn(const XY &p, double pos)
{
    if (!straight) {
#ifdef _DEBUG
        XY dummy;
        double t;
        //test that p and pos correspond - compiles to NOP in release mode
        _ASSERT(fabs(Distance(p, dummy, t))<0.1);
        _ASSERT(fabs(t-pos)<0.01);
#endif
        Chop(pos, 1);
    }
    start = p;
    return *this;
}

/** Set the end of the Edge thereby effectively chopping its tail away.
 * We assume 'p' and 'pos' correspond to the same point and use the one resulting
 * in faster operation. This is very small processing when compiled in optimized mode. 
 * For straight edges, only an assignment and "pos" need not be calculated in the
 * caller context (as it is not used) due to inlining.
 * This version ignores the "pos" completely for staright lines even in debug mode*/
inline Edge& Edge::SetEndIgn(const XY &p, double pos)
{
    if (!straight) {
#ifdef _DEBUG
        XY dummy;
        double t;
        //test that p and pos correspond - compiles to NOP in release mode
        _ASSERT(fabs(Distance(p, dummy, t))<0.1);
        _ASSERT(fabs(t-pos)<0.01);
#endif
        Chop(0, pos);
    }
    end = p;
    return *this;
}

/** Set the start and end of the Edge thereby effectively chopping its head and tail away.
 * We assume 'p' and 'pos' correspond to the same point and use the one resulting
 * in faster operation. This is very small processing when compiled in optimized mode.
 * For straight edges, only an assignment and "pos" need not be calculated in the
 * caller context (as it is not used) due to inlining. Ignores the "pos" completely 
 * for staright lines even in debug mode.*/
inline Edge& Edge::SetStartEndIgn(const XY &s, const XY &d, double spos, double dpos)
{
    if (!straight) {
#ifdef _DEBUG
        XY dummy;
        double tt, ss;
        //test that p and pos correspond - compiles to NOP in release mode
        _ASSERT(fabs(Distance(s, dummy, tt))<0.1);
        _ASSERT(fabs(Distance(d, dummy, ss))<0.1);
        _ASSERT(fabs(tt-spos)<0.01);
        _ASSERT(fabs(ss-dpos)<0.01);
#endif
        Chop(spos, dpos);
    }
    start = s;
    end = d;
    return *this;
}

/** Returns the bounding box of the hull of the bezier curve.*/
inline Block Edge::GetBezierHullBlock() const
{
    _ASSERT(!straight);
    return Block(std::min(std::min(start.x, end.x), std::min(c1.x, c2.x)),
                 std::max(std::max(start.x, end.x), std::max(c1.x, c2.x)),
                 std::min(std::min(start.y, end.y), std::min(c1.y, c2.y)),
                 std::max(std::max(start.y, end.y), std::max(c1.y, c2.y)));
}

/** Returns the x range of the hull's bounding box.
 * (Cheaper than GetBezierHullBlock() & works for straight edges, too.)*/
inline Range Edge::GetHullXRange() const
{
    if (straight)
        return Range(std::min(start.x, end.x), std::max(start.x, end.x));
    return Range(std::min(std::min(start.x, end.x), std::min(c1.x, c2.x)),
                 std::max(std::max(start.x, end.x), std::max(c1.x, c2.x)));
}

/** Returns the y range of the hull's bounding box.
* (Cheaper than GetBezierHullBlock() & works for straight edges, too.)*/
inline Range Edge::GetHullYRange() const
{
    if (straight)
        return Range(std::min(start.y, end.y), std::max(start.y, end.y));
    return Range(std::min(std::min(start.y, end.y), std::min(c1.y, c2.y)),
                 std::max(std::max(start.y, end.y), std::max(c1.y, c2.y)));
}


/** Find and eliminate any potential loop in the segment [first,last)
 * does not work with vectors (iterators must survive insert+delete ops)
 */
template <typename E, typename Iterator>
void Edge::RemoveLoop(std::list<E> &edges, Iterator first, Iterator last, bool self,
                             std::vector<Edge>*original, size_t orig_offset)
{
    XY r[9];
    double pos_one[9], pos_two[9];
    unsigned orig_u = orig_offset, orig_v;
    //Start by removing self-intersections
    if (self)
        for (auto i = first; i!=last; i++,orig_u++)
            if (!i->IsStraight()) {
                unsigned num = i->SelfCrossing(r, pos_one, pos_two);
                if (num==0) continue;
                if (num==2) {
                    if (pos_one[0]<pos_one[1] && pos_two[0]>pos_two[1])
                        num--;
                    else if (pos_one[0]>pos_one[1] && pos_two[0]<pos_two[1]) {
                        num--;
                        pos_one[0] = pos_one[1];
                        pos_two[0] = pos_two[1];
                    } else
                        //a strange crossing system - give up
                        continue;
                }
                const double p1 = std::min(pos_one[0], pos_two[0]);
                const double p2 = std::max(pos_one[0], pos_two[0]);
                //Chop to remove the part between the two positions
                auto next_i = i;
                next_i++;
                edges.emplace(next_i, *i)->Chop(p2, 1);
                i->Chop(0, p1);
                if (original) {
                    original->emplace(original->begin()+orig_u+1, (*original)[orig_u], p2, 1);
                    (*original)[orig_u].Chop(0, p1);
                }
            }
    //Now find the two outermost crosspoints.
    //If crosspoints do not happen like that, we will be unsuccessful here.
    auto prev_last = last;
    prev_last--;
    orig_u = orig_offset;
    for (auto u = first; u!=last; u++, orig_u++) {
        orig_v = original ? original->size()-1 : 0;
        for (auto v = prev_last; v!=u; v--, orig_v--) {
            auto next_u = u; next_u++;
            unsigned num = u->Crossing(*v, next_u==v, r, pos_one, pos_two);
            if (!num) continue;
            //Check if in case of several crosspoints the one with smallest pos in "pos_one"
            //corresponds to the one with largest pos in "pos_two"
            unsigned small = std::min_element(pos_one, pos_one+num) - pos_one;
            unsigned large = std::max_element(pos_two, pos_two+num) - pos_two;
            //If they do not - we could not easily remove the loop - we give up
            if (small!=large) {
                //_ASSERT(0);
                return;
            }
            //Loop found. Chop these edges and remove all edges in-between
            //Note that This is not perfect. We may have a bad loop still or even several
            //But I give up coding here.
            u->Chop(0, pos_one[small]);
            v->Chop(pos_two[large], 1);
            //make the two edges connect. If one is straight, use that, if both are bezier, use midpoint
            if (u->IsStraight()) {
                if (v->IsStraight()) {
                    //both are straight - stive to keep horizontal & vertical edges so
                    if (u->start.x==u->end.x) v->start.x = u->start.x;
                    else if (v->start.x==v->end.x) u->end.x = v->start.x;
                    else u->end.x = v->start.x = (u->end.x + v->start.x)/2;
                    if (u->start.y==u->end.y) v->start.y = u->start.y;
                    else if (v->start.y==v->end.y) u->end.y = v->start.y;
                    else u->end.y = v->start.y = (u->end.y + v->start.y)/2;
                } else
                    // v is bezier
                    v->start = u->end;
            } else if (v->IsStraight())
                // u is bezier
                u->end = v->start;
            else
                //both are beziers
                u->end = v->start = (u->end + v->start)/2;
            edges.erase(++u, v); //we modify u here, but use it no longer
            if (original) {
                (*original)[orig_u].Chop(0, pos_one[small]);
                (*original)[orig_v].Chop(pos_two[large], 1);
                original->erase(original->begin()+orig_u+1, original->begin()+orig_v);
            }
            return;
        }
    }
}

} //namespace contour bezier



#endif //CONTOUR_H
