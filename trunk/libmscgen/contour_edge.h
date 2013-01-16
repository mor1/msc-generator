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
#include "contour_ellipse.h"

namespace contour {

/** @addtogroup contour_internal
 * @{
 */


/** Describes how three points can relate to each other.
 */
typedef enum {
    ALL_EQUAL,       ///<All three are identical.
    A_EQUAL_B,       ///<Two of them are identical. 
    A_EQUAL_C,       ///<Two of them are identical.
    B_EQUAL_C,       ///<Two of them are identical. 
    IN_LINE,         ///<Three separate points on a line.
    CLOCKWISE,       ///<`A`->`B`->`C` define a non-degenerate clockwise triangle.
    COUNTERCLOCKWISE ///<`A`->`B`->`C` define a non-degenerate counterclockwise triangle.
} ETriangleDirType;

ETriangleDirType triangle_dir(XY a, XY b, XY c);

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
typedef enum {
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
} EExpandType;

/** A directed edge of a contour.
 * @ingroup contour_internal
 *
 * We distinguish two types: straight or curvy.
 * Curvy edge is a part of an ellipse. We call it tilted if the axes of the ellipse
 * are not parallel with the x and y axises. We make a distinction between tilted and
 * non-tilted curvy edges, because you need more calculations for tilted ones.
 *
 * For straight edges we only use the two endpoints from the class. 
 * For curvy ones, we also use the EllipseData member.
 */
class Edge
{
    friend class SimpleContour;
    friend class ContoursHelper;
public:
    /** Describes what type of an edge it is. */
    typedef enum {
        STRAIGHT=0,   ///<A straight edge.
        FULL_CIRCLE=1,///<A curvy edge that is a full circle or ellipse. 
        ARC=2         ///<A curvy edge that is a section of a circle or ellipse.
    } EEdgeType;
    /** Describes, how two expanded edges can relate to each other.
     */
    typedef enum {
        DEGENERATE,     ///<One of the edges have `start==end`.
        SAME_ELLIPSIS,  ///<Theyare different segments of the same ellipse. (Should be merged).
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
        /** They do not have a crosspoint, neither if extended, but their linear extension have.
         *
         * A difference exists from CP_EXTENDED but only for curvy edges, since for straight edges
         * regular extension is linear. In the below example you can see a straight edge and
         * a concave arc (the arc is smaller than a half-circle, so they meet at a non-zero angle).
         * In the below figure `x` marks the end of the two expanded edges, and straigght lines
         * represent their linear extension. The dotted extension shows the regular extension for the 
         * circular edge and we can see it will never cross the extended straight edge.
         * The crosspoint of the linear extensions is marked by `o`.
         * @verbatim
            ----->-x--------o
                           /
             ..>...+      |
                  :      /...
                 :      x:   :.
                :      /       :
                :      |        :
           @endverbatim */
        NO_CP_CONVERGE,
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
        NO_CP_PARALLEL 
    } EExpandCPType;
    ///<True if two expanded edges or their extrension has crosspoints.
    static bool HasCP(EExpandCPType t) {return t==CP_REAL || t==CP_EXTENDED;}

protected:
    EEdgeType   type;          ///<Type of the edge.
    XY          start;         ///<Startpoint of the edge.
    XY          end;           ///<Endpoint of the edge, shall be different from `start`.
    double      s;             ///<Starting radian of a curvy edge supposedly between [0..2pi). Unused for straight edges. For FULL_CIRCLE, it still has meaning.
    double      e;             ///<Ending radian of a curvy edge supposedly between [0..2pi). Unused for straight and full circle edges.
    bool        clockwise_arc; ///<True if a curvy edge goes clockwise from `s` to `e`. Unused for straight edges.
    EllipseData ell;           ///<Descriptor of the ellipse for curvy edges.
    Block       boundingBox;   ///< A bounding box for the edge.

public:
    mutable bool visible;      ///<True if the edge shall be shown/drawn.
    Edge() : type(STRAIGHT), visible(true) {boundingBox.MakeInvalid();}
    Edge(const XY &s, const XY &e) : type(STRAIGHT), start(s), end(e), boundingBox(s, e), visible(true) {}
    Edge(const XY &c, double radius_x, double radius_y=0, double tilt_deg=0, double s_deg=0, double d_deg=360);

    EEdgeType GetType() const {return type;}    ///<Returns the type of the edge.
    const XY & GetStart() const {return start;} ///<Returns the startpoint.
    const XY & GetEnd() const {return end;}     ///<Returns the endpoint.
    const Block &GetBoundingBox() const {return boundingBox;} ///<Returns the bounding box.
    double Distance(const XY &, XY &point, double &pos) const; //always nonnegative
    DistanceType Distance(const Edge &) const;    //always nonnegative

    const EllipseData &GetEllipseData() const {_ASSERT(type!=STRAIGHT); return ell;} ///<Returns the ellipse data. Asserts for straight edges.
    bool GetClockWise() const {_ASSERT(type!=STRAIGHT); return clockwise_arc;} ///<Returns if the edge goes clockwise around the ellipse. Asserts for straight edges.
    double GetSpan() const;
    double GetRadianE() const {_ASSERT(type!=STRAIGHT); return type==ARC ? e : s;} ///<Returns the ending radian. Asserts for straight edges.
    double GetRadianS() const {_ASSERT(type!=STRAIGHT); return s;} ///<Returns the starting radian. Asserts for straight edges.
    double GetRadianMidPoint() const;
    XY Pos2Point(double pos) const;
    void SetFullCircle() {_ASSERT(type!=STRAIGHT); type = FULL_CIRCLE; end=start; e=s; CalculateBoundingBox();} ///<Makes the edge a full circle starting at 's`. Asserts for straight edges.

    bool IsSane() const;
    bool IsSaneNoBoundingBox() const;
    /** Helper for area calculation. 
     * 
     * Return the (directed) area between the y axis and the edge times 2, 
     * minus start.x*start.y, plus end.x*end.y.
     */
    double GetAreaAboveAdjusted() const {return type==STRAIGHT ? start.x*end.y-start.y*end.x : getAreaAboveAdjusted_curvy();}
    double GetLength() const {return type==STRAIGHT ? (end-start).length() : type == FULL_CIRCLE ? ell.FullCircumference() : ell.SectorCircumference(s,e);} ///<Returns the length of the arc.
    //returns the centroid of the area above multipled by the (signed) area above
    XY GetCentroidAreaAboveUpscaled() const;

    bool Expand(double gap);
    void CreateExpand2D(const XY &gap, std::vector<Edge> &ret, int &stype, int &etype) const;

    void Shift(const XY &wh) {start+=wh; end+=wh; boundingBox.Shift(wh); if (type!=STRAIGHT) ell.Shift(wh);} ///<Translate the edge.
    void Scale(double sc) {start*=sc; end*=sc; boundingBox.Scale(sc); if (type!=STRAIGHT) ell.Scale(sc);}    ///<Scale the edge.
    void Rotate(double cos, double sin, double radian);
    void RotateAround(const XY&c, double cos, double sin, double radian);
    void SwapXY();

    bool operator ==(const Edge& p) const {return start==p.start && end==p.end && type==p.type && (type==STRAIGHT || equal_curvy(p));}
    bool operator < (const Edge& p) const {return start!=p.start ? start<p.start : end!=p.end ? end<p.end : type!=p.type ? type<p.type : type==STRAIGHT ? false : smaller_curvy(p);}

    void Invert() {std::swap(start, end); if (type!=STRAIGHT) {clockwise_arc = !clockwise_arc; std::swap(s, e);}} ///<Reverses the direction of the edge.
    /** Calculates the angle of the edge at point `p`.
     * 
     * @param [in] incoming If true the angle of the incoming segment is calculated (as if it were outgoing), if false the outgoing part.
     * @param [in] p The angle is calculated at this point (indifferent for straight edges)
     * @param [in] pos Should be the pos value corresponding to `p`.
     * @returns The angle of the edge
     */
    RayAngle Angle(bool incoming, const XY &p, double pos) const {return type==STRAIGHT ? RayAngle(incoming ? angle(end, XY(end.x+100, end.y), start) : angle(start, XY(start.x+100, start.y), end)) : angle_curvy(incoming, p, pos);} 

    //Gives the intersecting points of me and another straight edge
    unsigned Crossing(const Edge &A, XY r[], double pos_my[], double pos_other[]) const;
    //Tells at what x pos this edge crosses the horizontal line at y, rets the number of crosses
    int CrossingVertical(double x, double y[], double pos[], bool forward[]) const;
    const Block& CalculateBoundingBox() {boundingBox.MakeInvalid(); boundingBox += start; boundingBox += end; if (type!=STRAIGHT) calcbb_curvy(); return boundingBox;} ///<Calculates & returns the bounding box

    
    void   PathTo(cairo_t *cr) const {if (type==STRAIGHT) cairo_line_to(cr, end.x, end.y); else pathto_curvy(cr);} ///<Adds the edge to a cairo path. * It assumes cairo position is at `start`.
    void   PathDashed(cairo_t *cr, const double pattern[], unsigned num, int &pos, double &offset, bool reverse=false) const;

    //helpers for offsetbelow
    double OffsetBelow(const Edge &M, double &touchpoint) const;

    //tangential toucher from a point
    bool TangentFrom(const XY &from, XY &clockwise, XY &cclockwise) const;
    bool TangentFrom(const Edge &from, XY clockwise[2], XY cclockwise[2]) const;

protected:
    bool equal_curvy(const Edge &p) const;
    bool smaller_curvy(const Edge &p) const;
    void pathto_curvy(cairo_t *cr) const;
    void calcbb_curvy();
    RayAngle angle_curvy(bool incoming, const XY &p, double pos) const;

    unsigned CrossingStraightStraight(const Edge &A, XY r[], double pos_my[], double pos_other[]) const;

    //Convert between pos (0..1) and coordinates
    double pos2radian_full_circle(double r) const {return fmod_negative_safe(clockwise_arc ? s+r*2*M_PI : s-r*2*M_PI, 2*M_PI);} ///<Convert from a pos value of [0..1] to a radian value assuming full circle
    double radian2pos_full_circle(double r) const {return fmod_negative_safe(clockwise_arc ? (r-s)/(2*M_PI) : (s-r)/(2*M_PI), 1.);} ///<Convert from a radian value to a pos value of [0..1] value assuming full circle

    //Convert between pos (0..1) and coordinates
    double pos2radian(double r) const;
    double radian2pos(double r) const;
    //helper, checks if r is between s and e
    bool   radianbetween(double r) const;
    //remove the part of an edge before an after point p lying on the edge
    void   removebeforepoint_curvy(const XY &p);
    void   removeafterpoin_curvy(const XY &p);
    double getAreaAboveAdjusted_curvy() const;
    static XY getcentroidareaaboveupscaled_straight(const XY&start, const XY&end);
    XY getcentroidareaaboveupscaled_curvy() const;
    double offsetbelow_curvy_straight(const Edge &M_straight, bool straight_is_up, double &touchpoint) const;

    double FindRadianOfClosestPos(unsigned num, double pos[], double rad);
    bool UpdateClockWise(double new_s, double new_e);
    void SwapXYcurvy();
    int CrossingVerticalCurvy(double x, double y[], double pos[], bool forward[]) const;

    //returns a point on the line of a tangent at "pos", the point being towards the start of curve/edge.
    XY     PrevTangentPoint(double pos) const;
    //returns a point on the line of a tangent at "pos", the point being towards the end of curve/edge.
    XY     NextTangentPoint(double pos) const;

    //Removes the part of the edge before point p. Assumes p lies on us. Invalidates BoundingBox!!!
    Edge& SetStartStrict(const XY &p, double pos, bool keep_full_circle=false);
    Edge& SetStartLiberal(const XY &p, bool keep_full_circle=false);
    //Removes the part of the edge after point p. Assumes p lies on us. Invalidates BoundingBox!!!
    Edge& SetEndLiberal(const XY &p, bool keep_full_circle=false);
    //check if "next" is a direct continuation of "this". If yes combine "next" into "this" and return true
    bool   CheckAndCombine(const Edge &next);
    //Helpers for expand
    EExpandCPType FindExpandedEdgesCP(const Edge &M, XY &newcp) const;
    void SetStartEndForExpand(const XY &S, const XY &E);
    bool IsOpposite(const XY &S, const XY &E) const;

    void CreateExpand2DCurvy(const XY &gap, std::vector<Edge> &ret, int &stype, int &etype) const;
};

/** Returns the span of an ARC in radians. Handles case when `e < s`.
 */
inline double Edge::GetSpan() const
{
    _ASSERT(type!=STRAIGHT);
    if (type==FULL_CIRCLE) return 2*M_PI;
    if (clockwise_arc) {
        if (s<e) return e-s;
        else return e-s+2*M_PI;
    } else {
        if (s>e) return s-e;
        else return s-e+2*M_PI;
    }
}

/** Returns a point on a tangent in counterclockwise (previous) direction
 * @param pos The place where the tangent touches the edge.
 * @returns A point on the tangent, which is in counterclockwise direction.
 */
inline XY Edge::PrevTangentPoint(double pos) const
{
    if (type!=STRAIGHT)
        return ell.Tangent(pos2radian(pos), !clockwise_arc);
    if (pos<=0.5)
        return start*2-end;
    return start;
}

/** Returns a point on a tangent in clockwise (next) direction
 * @param pos The place where the tangent touches the edge.
 * @returns A point on the tangent, which is in clockwise direction.
 */
inline XY Edge::NextTangentPoint(double pos) const
{
    if (type!=STRAIGHT)
        return ell.Tangent(pos2radian(pos), clockwise_arc);
    if (pos>0.5)
        return end*2-start;
    return end;
}

/** Removes the part of the edge after point p.
 * @param p A point that is assumed to lie on the edge.
 * @param keep_full_circle If true, we do not chop a full circle.
 */
inline Edge& Edge::SetEndLiberal(const XY &p, bool keep_full_circle)
{
    end = p;
    if (type==STRAIGHT) {
        CalculateBoundingBox();
        return *this;
    }
    const double r = ell.Point2Radian(p);
    if (type==FULL_CIRCLE && !(test_equal(s,r) && keep_full_circle))
        type = ARC;
    e = r;
    CalculateBoundingBox();
    return *this;
}

/** Return the centroid of the area below the edge multiplied by the area.
 * 
 * We take the endpoints of the edge and draw a line from them to the x axis. 
 * The area bounded by these two lines, the edge itself and the x axis is
 * the subject area of this function. If the `start.x < end.s` the return is
 * negated. The return is also negated (perhaps 2nd time) if the edge is above 
 * the x axis (negative y values). 
 * The function returns the centroid of this area, scaled by the (signed)
 * area of this area.
 *
 * In more visual terms, showing the x axis and a straight edge and the area.
 * @verbatim
 ==========> =========>    
  +++++        ------      
  __+++        ------      
  |\+++        \-----      
    \++         \ ---      
     \+          \---     
      \           \--      
                  _\|      
 * @endverbatim
 * (Since in contour we treat y coordinates as growing downwards, we call these functions
 * "xxAreaAbove", meaning the area between edge & X axis.) 
 * The dotted area is the area above an edge. It is counted as positive in the first example;
 * and negative in the second. If we sum all such areas, we get the area of the contour 
 * (positive for clockwise and negative for counterclockwise), as it should be.
 *
 * Now, the formula for the above for an edge (X1,Y1)->(X2,Y2) is
 * AreaAbove = (X1-X2)*(Y1+Y2)/2 = (X1*Y1 - X2*Y2)/2 + (X1*Y2 - X2*Y1)/2
 * If we sum this for all edges of a connected contour the first term will
 * cancel out. Thus in the edge::xxAreaAboveAdjusted functions we return
 * the second term only (twice 2), thus we return
 * AreaAbove*2 - (X1*Y1 - X2*Y2).
 */
inline XY Edge::GetCentroidAreaAboveUpscaled() const
{
    switch (type) {
    default:
    case STRAIGHT:    return getcentroidareaaboveupscaled_straight(start, end);
    case FULL_CIRCLE: return ell.GetCenter() * ell.FullArea();
    case ARC:         return getcentroidareaaboveupscaled_curvy();
    }
}

namespace Edge_CreateExpand2D {
inline int comp_int(const double &a, const double &b) {
    return a<b ? -1 : a==b ? 0 : 1;
}
inline double comp_dbl(const double &a, const double &b, double g) {
    return a<b ? -g : a==b ? 0 : g;
}
}

/** Returns a list of edges resulting in expanding "this" in 2D.
 *
 * See overall documentation of what a 2D expansion is.
 * We return values describing the direction in which the edge
 * ends and starts as below.
 * @verbatim
   +4  _+3 _  +2
      |\ ^ /|           
        \|/             
   +1  <-0->  -1        
        /|\             
      |/ v \|           
   -2   -3    -4
 * @endverbatim
 * The above picture describes, how the end is described,
 * the start is described in reverse, thus for lines
 * `stype==dtype`. For degenerate edges `type==0`.
 *
 * @param [in] gap The size of the expansion in the x and y directions.
 * @param [out] ret The edges resulting from the expansion (perhaps more than one).
 * @param [out] stype The type of the starting of the edge.
 * @param [out] etype The type of the ending of the edge.
 */
inline void Edge::CreateExpand2D(const XY &gap, std::vector<Edge> &ret, int &stype, int &etype) const
{
    if (type!=STRAIGHT) return CreateExpand2DCurvy(gap, ret, stype, etype);
    ret.resize(ret.size()+1);
    Edge &e = *ret.rbegin();
    const XY off(Edge_CreateExpand2D::comp_dbl(end.y, start.y, gap.x),
                 Edge_CreateExpand2D::comp_dbl(start.x, end.x, gap.y));
    e.start = start + off;
    e.end   = end   + off;
    //expand for horizontal and vertical edges
    if (start.x == end.x) {
        e.start.y += Edge_CreateExpand2D::comp_dbl(start.y, end.y, gap.y);
        e.end.y -= Edge_CreateExpand2D::comp_dbl(start.y, end.y, gap.y);
    }
    if (start.y == end.y) {
        e.start.x += Edge_CreateExpand2D::comp_dbl(start.x, end.x, gap.x);
        e.end.x -= Edge_CreateExpand2D::comp_dbl(start.x, end.x, gap.x);
    }
    etype = stype = Edge_CreateExpand2D::comp_int(start.x, end.x) +
                    Edge_CreateExpand2D::comp_int(start.y, end.y)*3;
}

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

/** Calculates the touchpoint of tangents drawn from a given point.
 * 
 * For curvy edges:
 *     Given the point `from` draw tangents to the edge (two can be drawn)
 *     and calculate where these tangents touch the it.
 *     In this context the *clockwise tangent* is the one which is traversed from 
 *     `from` towards the ellipse touches the ellipse in the clockwise direction.
 * For straight edges:
 *     We return the `start` in both clockwise and counterclockwise.
 *     This is because we ignore the endpoint - after all it will be the startpoint
 *     of the next edge and will be considered there.
 * @param [in] from The point from which the tangents are drawn.
 * @param [out] clockwise The point where the clockwise tangent touches the ellipse.
 * @param [out] cclockwise The point where the counterclockwise tangent touches the ellipse.
 * @returns True if success, false if `from` is inside or on the ellipse.
 */
inline bool Edge::TangentFrom(const XY &from, XY &clockwise, XY &cclockwise) const
{
    if (type==FULL_CIRCLE) return ell.TangentFrom(from, clockwise, cclockwise);
    if (type==ARC && ell.TangentFrom(from, clockwise, cclockwise)) {
        if (radianbetween(ell.Point2Radian(clockwise)))
            clockwise  = minmax_clockwise(from, start, clockwise, true);
        else 
            clockwise = start;
        if (radianbetween(ell.Point2Radian(cclockwise)))
            cclockwise = minmax_clockwise(from, start, cclockwise, false);
        else 
            cclockwise = start;
    } else
        clockwise = cclockwise = start;
    return true;
}

} //namespace

#endif //CONTOUR_H
