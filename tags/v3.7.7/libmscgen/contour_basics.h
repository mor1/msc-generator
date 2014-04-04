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

/** @defgroup contour The contour library 
 *
 * The contour library contains functionality for handling 2D vector shapes. Intersection,
 * union, expansion, area/circumference calculation, distance computation are the key 
 * fetures.
 *
 * What is a *contour*?
 * --------------------
 *
 * We define the concept of a *contour* (embodied in the Contour class) as a
 * closed 2D shape. It has vertices and edges, where edges can be straight sections
 * or cubic bezier curves. Contours are stored as lists (well, vectors) of their edges.
 *
 * The library can handle contours that have *holes* in them, or that have multiple
 * disjoint parts (even inside a hole). We always try to normalize contours
 * such that their edges do not cross any other edge. 
 * Edges are ordered in the contour - *clockwise* for positive contour outlines, 
 * while counerclockwise for outlines representing holes. Clockwiseness is defined
 * such that the y coordinate grows _downward_. This convention is used everywhere
 * as throughout the comments.
 *
 * Features
 * --------
 *
 * - The main feature of the library is to be able to create union and intersection
 * of contours; substraction and xor operation is also possible. 
 * - A number of auxiliary operations
 * are defined, such as area, centroid location and circumference length calculation, 
 * tangent fitting and various cuts. 
 * - Naturally linear transformations (shift, rotate, scale, etc.) are supported too.
 * - The library uses the C++11 move schemantics extensively.
 * - The library can *expand* a contory by a given pixels, that is generate
 * a contour that is this many pixels wider (or narrrower for negative values), 
 * such that the resulting contour is sane - its edges do not cross each other.
 * (In general this is called curve offsetting, but for closed curves this really 
 * becomes expansion or shrinking.)
 * - Another way to expand a shape (called Expand2D) takes two parameters (x and y) 
 * and creates a shape around the original such that an x-y rectangle just fits 
 * in between at any given place. In other words, placing a 2*x - 2*y rectangle
 * centered anywhere outside the expanded shape will not overlap with the original.
 * - The distance of two contours (along with the closest points) can be determined.
 * - Especially important for layout tipography, the library can determine, how much a
 * one of two shapes has to be moved up or down to touch anothe shape.
 *
 * See the definition of the Contour class for a detailed description of the
 * features.
 *
 * The library also contains a Path class which is represents a set of
 * (not necessarily connected) edges, which may or may not be closed.
 * It supports basic transformations and drawing.
 * You can also convert it to a Contour (assuming it is a closed path).
 *
 * Terminology
 * -----------
 * 
 * - *pos* usually refers to a location on the edge and is between [0..1). 
 *   The start of the edge is position zero, the end of it is position 1, but this
 *   is not used, as we refer to the end of an edge as the start of the next edge.
 * - *Crosspoints* are points of edges meeting. 
 * - *rays* are defined in the context of a crosspoint. A ray is a part of an edge
 *   coming to or leaving from a crosspoint (called incoming/outgoing rays, respectively).
 *   For example, two straight crossing edges have four rays, two incoming and two outgoing.
 *   If three edges cross at the same point, we have 6 rays. If an edge `E` crosses a vertex `V`
 *   of the other polygon, we still have 4 rays: one incoming and outgoing for the edge; 
 *   an incoming ray for the edge leading to `V` and an outgoing ray for the edge after `V`.
 *   A ray can be curvy, if its edge is a bezier.
 * - *Walking* is the procedure used to calculate the union/intersection of two shapes.
 *   We calculate all crosspoints and start walking around one of the contours. During the walk
 *   we stop by each crosspoint and see if we have to switch to the other contour. Finally we get
 *   back to the staring point. Then we can start an additional walk if relevant crosspoints have
 *   not been visited - in which case the resulting contour will not be contigous, that is it will
 *   contain multiple disjoint shapes. (Some of which may turn out to be holes inside other 
 *   shapes.)
 * - A *tangent* for our purposes is a line that touches a shape (or edge) only at a single point
 *   (Except for straight lines, where the tangent _lies on_ the edge.)
 *   Class Contour and Edge have functions that take that single point as input and provide another point on the 
 *   tangent line some distance away. We can speak of a *forward* point, which is when we start on the
 *   tangent line towards the end of the edge or a *backward* point which is towards the start of the edge.
 *   We also call these 'next' and 'prev' tangent points, respectively.
 *   Now, a few more details here
 *       one point.
 *     - If we take a tangent at a point on a concave edge (say the inner circle of a crescent moon)
 *       the tangent will even locally be inside the shape.
 *     - If we take a tangent at a point that is on a straight edge, naturally the tangent line will
 *       lien on the straight edge.
 *     - If we take a tangent at a vertex, which is not smooth (has an angle), there are actually two
 *       different tangent lines, one for each edge. In this case the forward and backward tangent points 
 *       are returned form the edge subsequent or preceeding to the vertex, respectively.
 * - *Untangling* means the process of creating a shape, whose edges do not cross each other. Normally
 *   contours in class Contour have no edges crossing. There are two rules of what part of a tangled
 *   list of edges shall we keep during untangling, called winding rules. 
 *   For both fill rules, whether or not a point is included 
 *   in the resulting shape is determined by taking a ray from that point to infinity and looking at intersections 
 *   with the original shape's contour. The ray can be in any direction, as long as it doesn't pass through the 
 *   end point of an edge or have a tricky intersection such as intersecting tangent to the path.
 *     - *Nonzero* rule: If the original shape crosses the ray from left-to-right, counts +1. 
 *       If the path crosses the ray from right to left, counts -1. (Left and right are determined from the 
 *       perspective of looking along the ray from the starting point.) 
 *       If the total count is non-zero, the point will be included in the untangled shape.
 *     - *Evenodd* rule: Counts the total number of intersections, without regard to the orientation of the contour. 
 *       If the total number of intersections is odd, the point will be included in the untangled shape.
 */

/** @defgroup contour_files Files of the contour library 
 * @ingroup contour
 * @brief These files constitute the contour library.
 * @file contour_basics.h The basic definitions for geometry
 * calculations (the contour library).
 * @ingroup contour_files
 */

/** @defgroup contour_internal Internal elements of the contour library.
 * @ingroup contour
 * @brief These elements are internal to the contour library and are presented for understanding the inner workings.
 */

#if !defined(CONTOUR_BASICS_H)
#define CONTOUR_BASICS_H

#include <cfloat>
#include <utility>
#include <cassert>
#include <cstddef>
#include <limits> 
#include <algorithm>

using namespace std::rel_ops;  //so that we have != and <= and >= etc from only == and <

#ifndef _ASSERT
#define  _ASSERT(A)
#endif

/** The `contour` namespace holds all the contour library
 */
namespace contour {

/** Enum describing the relation of two closed contours.
* @ingroup contour
*/
enum EContourRelationType {
    REL_OVERLAP=0,          ///< They overlap, but each has parts not covered by the other.
    REL_A_IS_EMPTY,         ///< The first contour is empty, the second is not.
    REL_B_IS_EMPTY,         ///< The second contour is empty, the first is not.
    REL_BOTH_EMPTY,         ///< Both contours are empty.
    REL_A_INSIDE_B,         ///< The first contour is fully inside the second, which has parts not overlapping with the first.
    REL_B_INSIDE_A,         ///< The second contour is fully inside the first, which has parts not overlapping with the first.
    REL_SAME,               ///< The two contours are identical (cover exactly the same area).
    REL_APART,              ///< Two non-empty contours that has no intersecting area, and none is around the other.
    REL_A_IN_HOLE_OF_B,     ///< The first contour is fully inside a hole of the second.
    REL_B_IN_HOLE_OF_A,     ///< The second contour is fully inside a hole of the first.
    REL_IN_HOLE_APART       ///< The two contours have no overlapping area, but each as a part in a hole of the other.
};

/** @addtogroup contour_internal
 * @{
 */

/** Gives the maximum value for a variable
 * \param real The variable we want the maximum value of
 * \return The maximum value for `real`
 */
#define MaxVal(real) (std::numeric_limits<decltype(real)>::max())
/** The maximum coordinate for contour library
 */
#define CONTOUR_INFINITY (std::numeric_limits<double>::max())

//other helpers
/** We quantize to this granularity to avoid underflow.
 *
 * The contour library cannot distinguish between distances smaller than this.
 * Also, any numerical result will be treated as equal if within this distance.
 * test_xxx() routines all rely on this constant for precision.
 */
static const double SMALL_NUM = 1e-5;
inline bool test_zero(double n) {return fabs(n)<SMALL_NUM;} ///< True if `n` is sufficiently close to zero, uses SMALL_NUM.
inline bool test_equal(double n, double m) {return test_zero(n-m);} ///< True if `n` and `m` are sufficiently close to each other, uses SMALL_NUM
inline bool test_smaller(double n, double m) {return n<m-SMALL_NUM;} ///< True if `n` is *really* smaller than `m`, not just by epsilon, uses SMALL_NUM
inline bool test_positive(double n) {return n >= SMALL_NUM;} ///< True if `n` is sufficiently larger than zero, uses SMALL_NUM

/** Returns the value closer to zero.
 */
template <typename real> real minabs(real a, real b) {return fabs(a)<fabs(b) ? a : b;}

/** Returns -1, 0 or +1 indicating the sign of `a`.
 */
template <typename real> int fsign(real a) {return a>0 ? +1 : a<0 ? -1 : 0;}

/** Returns true if the relation means that the two contours
 * have non empty overlapping area.
 */
inline bool result_overlap(EContourRelationType t) {return t==REL_OVERLAP || t==REL_A_INSIDE_B || t==REL_B_INSIDE_A || t==REL_SAME;}
/** Returns the relation with the role of the two contours
 * swapped.
 */
inline EContourRelationType switch_side(EContourRelationType t)
{
    switch (t) {
    default: _ASSERT(0); //fallthrough
    case REL_IN_HOLE_APART:
    case REL_BOTH_EMPTY:
    case REL_OVERLAP:
    case REL_APART:
    case REL_SAME:            return t;

    case REL_A_IS_EMPTY:      return REL_B_IS_EMPTY;
    case REL_B_IS_EMPTY:      return REL_A_IS_EMPTY;
    case REL_B_INSIDE_A:      return REL_A_INSIDE_B;
    case REL_A_INSIDE_B:      return REL_B_INSIDE_A;;
    case REL_B_IN_HOLE_OF_A:  return REL_A_IN_HOLE_OF_B;
    case REL_A_IN_HOLE_OF_B:  return REL_B_IN_HOLE_OF_A;
    }
}

/** @} addgroup contour_internal*/



/** The basic class for a (2D, floating point) coordinate or vector.
* @ingroup contour
 *
 * Note that in all cases when we talk about clockwiseness in this space,
 * we envision it such that the y coordinate grows _downward_, which is
 * the opposite of mathematical definition.
 * Also for angles, direction zero is thus to the *right*, a degree of 45,
 * is down-right, a degree of 90 is downward, etc.
 */

class XY {
public:
    double x; ///< The x coordinate.
    double y; ///< The y coordinate.
    XY() {}
    XY(double a, double b) : x(a), y(b) {}
    XY &   operator +=(const XY &wh)      {x+=wh.x; y+=wh.y; return *this;}    ///< Shifts the point by `wh`.
    XY &   operator -=(const XY &wh)      {x-=wh.x; y-=wh.y; return *this;}    ///< Shifts the point by `-wh`.
    XY     operator +(const XY &wh) const {return XY(x+wh.x, y+wh.y);}         ///< Returns the point shifted by `wh`, keeping the original.
    XY     operator -(const XY &wh) const {return XY(x-wh.x, y-wh.y);}         ///< Returns the point shifted by `-wh`, keeping the original.
    double DotProduct(const XY &B) const  {return x*B.x+y*B.y;}                ///< Returns dot product of the point as a vector, with `B`.
    XY     operator *(double scale) const {return XY(x*scale, y*scale);}       ///< Returns a scaled version of the point.
    XY     operator *=(double scale)      {x*=scale; y*=scale; return *this;}  ///< Scales the point.
    XY     operator /(double scale) const {return XY(x/scale, y/scale);}       ///< Returns a scaled version of the point.
    XY     operator /=(double scale)      {x/=scale; y/=scale; return *this;}  ///< Scales the point.
    double PerpProduct(const XY &B) const {return x*B.y - y*B.x;}              ///< This the dot product of transpose(point) and `B`.
    double length(void) const             {return sqrt(x*x+y*y);}              ///< Length of vector or distance of point from origin.
    double length_sqr(void) const         {return x*x+y*y;}                    ///< Square of the length of vector.
    bool   operator ==(const XY& p) const {return x==p.x && y==p.y;}
    XY	   operator -() const             {return XY(-x, -y);}
    bool   operator <(const XY& p) const  {return x!=p.x ? x<p.x : y<p.y;}     ///< Cheap comparison so `XY` can be inserted into ordered sets.
    bool   test_equal(const XY& p) const  {return contour::test_equal(x, p.x) && contour::test_equal(y, p.y);} ///< Tests if point is close to `p`, uses SMALL_NUM.
    bool   test_smaller(const XY& p) const{return contour::test_equal(x, p.x) ? contour::test_smaller(y, p.y) : contour::test_smaller(x, p.x);} ///< Tests if point is significantly smaller than `p`, uses SMALL_NUM.
    XY     Rotate90CW() const             {return XY(-y, x);}  ///< Rotate 90 degrees clockwise (in a space where y grows "downwards").
    XY     Rotate90CCW() const            {return XY(y, -x);}  ///< Rotate 90 degrees counter-clockwise (in a space where y grows "downwards").
    XY &   Rotate(double cos, double sin) {double X=x; x=X*cos-y*sin; y=X*sin+y*cos; return *this;} ///< Free rotate with user-supplied `sin` and `cos` of the angle (in a space where y grows "downwards").
    XY &   RotateAround(const XY&c, double cos, double sin)
	    {double X=x-c.x, Y=y-c.y; x=X*cos-Y*sin+c.x; y=X*sin+Y*cos+c.y; return *this;} ///< Free rotate around `c` with user-supplied `sin` and `cos` of the angle (in a space where y grows "downwards").
    XY &   SwapXY()                       {std::swap(x,y); return *this;}
    XY &   Round()                        {x = floor(x+.5); y=floor(y+.5); return *this;} ///< Round coordinates to closest integer.
    XY &   RoundUp()                      {x = ceil(x); y=ceil(y); return *this;}         ///< Round coordinates to integer (away from zero).
    XY &   RoundDown()                    {x = floor(x); y=floor(y); return *this;}       ///< Round coordinates to integer (towards zero).
    XY &   Scale(const XY &sc)            {x*=sc.x; y*=sc.y; return *this;}               ///< Scale the two coordinates separately.
    XY &   Normalize()                    {const double l = length(); if (l) {x/=l; y/=l;} return *this;}  ///< Return a unit-length scaled version.
    double Distance(const XY &p) const    {return sqrt((x-p.x)*(x-p.x)+(y-p.y)*(y-p.y));}  ///< Distance of us to `p`.
    double DistanceSqr(const XY &p) const {return (x-p.x)*(x-p.x)+(y-p.y)*(y-p.y);}        ///< Square of distance to `p`.
    XY     ProjectOntoLine(const XY&A, const XY&B) const {if (A.test_equal(B)) return A; const XY d = A-B; return B + d*d.DotProduct(*this-B)/d.length_sqr();} ///< Return closest point of the (infinite) line (defined by `A` and `B`) to us.
    XY     Transform(double xx, double xy, double yx, double yy, double dx, double dy)
                                          {return XY(x*xx + y*xy + dx, x*yx + y*yy + dy);} ///< Transform via the transformation linear matrix.
};

inline XY operator*(double a, const XY &xy) {return xy*a;} ///< Return a scaled version of the point `xy`. Enables writing the scalar before the point.

/** Enum describing the relation of a contour and a point.
 * @ingroup contour
 */
enum EPointRelationType {
    WI_OUTSIDE=0, ///< The point is fully outside the contour.
    WI_INSIDE,    ///< The point is fully inside the contour.
    WI_ON_EDGE,   ///< The point is on (the middle of) an edge.
    WI_ON_VERTEX, ///< The point is exactly at a vertex.
    WI_IN_HOLE    ///< The point is in a hole of the contour.
};

/** Returns true, if the point is inside or at the edge of the contour.
 * @ingroup contour_internal
*/
inline bool inside(EPointRelationType t) {return t!=WI_OUTSIDE && t!=WI_IN_HOLE;}

/** The structure for a one-dimensional range.
 * @ingroup contour
 *
 * Note that if `from` is larger than `till`, we consider that an "invalid"
 * range. If they are the same, the range is "empty".
 */
struct Range {
    double from;  ///< The lower (smaller) end of the range.
    double till;  ///< The higher (larger) end of the range.
    Range() {}
    Range(bool) {MakeInvalid();}
    Range(double s, double d) : from(s), till(d) {}
    void MakeInvalid() {from = MaxVal(from); till = -MaxVal(till);}
    bool IsInvalid() const {return from == CONTOUR_INFINITY && till == -CONTOUR_INFINITY;}
    bool Overlaps(const struct Range &r, double gap=0) const   ///< True if the two ranges at least touch.
        {return from<=r.till+gap && r.from <= till+gap;}
    Range &operator+=(double a)
        {if (from>a) from=a; if (till<a) till=a; return *this;}  ///< Expands the range to include the value `a`. Note, this is _not_ a shift operation.
    Range &operator+=(const Range &a)
        {if (from>a.from) from=a.from; if (till<a.till) till=a.till; return *this;}  ///<Expands the range to include all points of the range `a`. Note, this is _not_ a shift operation.
    Range &operator-=(const Range &a) {
        if (a.IsWithin(from)) from=a.till;
        if (a.IsWithin(till)) till=a.from;
        return *this;}                                          ///< Cuts range `a` out of the range. May render the range invalid. Note, this is _not_ a shift operation.
    Range &operator*=(const Range &a)
        {if (from<a.from) from=a.from; if (till>a.till) till=a.till; return *this;} ///< Takes the intersection with range `a`. Note, this is _not_ a scale operation.
    Range operator+(const Range &a) const
        {return Range(std::min(a.from, from), std::max(a.till, till));}  ///< Returns the range expanded to include `a`. Note, this is _not_ a shift operation.
    Range operator*(const Range &a) const
        {return Range(std::max(a.from, from), std::min(a.till, till));}  ///< Returns the intersection with range `a`. Note, this is _not_ a scale operation.

    EPointRelationType IsWithin(double p) const {
		if (p==from || p == till) return WI_ON_VERTEX;
                return from<p && p<till ? WI_INSIDE : WI_OUTSIDE;
    }                                                                    ///< Checks if a value is within, at the end or outside of a range.
    bool IsWithinBool(double p) const {return from<=p && p<=till;}       ///< Checks if a value is inside/at edge of the range or outside.
    Range &Shift(double a) {from+=a; till+=a; return *this;}             ///< Shift the range by `a`.
    Range &Scale(double a) {from*=a; till*=a; if (a<0) std::swap(from, till); return *this;} ///<Scale the range by `a`.
    Range &Expand(double a) {from-=a; till+=a; return *this;}            ///< Expand the range at both ends by a.
    bool HasValidFrom() const {return from != CONTOUR_INFINITY;}         ///< Checks if the `from` member is valid.
    bool HasValidTill() const {return till != -CONTOUR_INFINITY;}        ///< Checks if the `till` member is valid.
    double Spans() const {return till-from;}                             ///< Calculates the width of the range.
    double MidPoint() const {return (from+till)/2;}                      ///< Returns the midpoint of the range - the average of `from` and `till`.
    bool operator <(const Range &r) const {
        if (till==r.till) return from<r.from;
        return till<r.till;
    }                                                                    ///< Cheap comparison for sorting.
    bool operator ==(const Range &r) const {return from==r.from && till==r.till;}
    Range & Round()       {from = floor(from+.5); till=floor(till+.5); return *this;}  ///< Round both ends to closest integer.
    Range & RoundUp()     {from = ceil(from); till=ceil(till); return *this;}          ///< Round both ends upward (towards larger integer)
    Range & RoundDown()   {from = floor(from); till=floor(till); return *this;}        ///< Round both ends downward (towards smaller integer)
    Range & RoundWider()  {from = floor(from); till=ceil(till); return *this;}         ///< Round ends outward (`from` downward, `till` upward)
    Range & RoundCloser() {from = ceil(from); till=floor(till); return *this;}         ///< Round ends inward (`from` upward, `till` downward)

    double Distance(double a) const {_ASSERT(!IsInvalid()); return minabs(from-a,a-till);}  ///< Returns the distance between a range and a point. Negative result if point is inside the range.
    EContourRelationType RelationTo(const Range &c) const {
        if (IsInvalid()) return c.IsInvalid() ? REL_BOTH_EMPTY : REL_A_IS_EMPTY;
        if (c.IsInvalid()) return REL_B_IS_EMPTY;
        if (from>=c.till || c.from>=till) return REL_APART;
        if (from<=c.from && till>=c.till) return REL_B_INSIDE_A;
        if (c.from<=from && c.till>=till) return REL_A_INSIDE_B;
        return REL_OVERLAP;
    }  ///< Returns the relation of the two ranges, e.g., overlap, inside, apart, etc.
};

/** A basic rectangle structure.
 * @ingroup contour
 *
 * Note that this rectangle is composed of two ranges, hence all time 4 double values
 * are used to describe a rectangle, we start with the 2 x coordinates followed by the two
 * y coordinates, unlike to many other systems, which start with the upper left corner and
 * either continue by width/height or the lower-right coordinate.
 *
 * An empty Block is called *invalid* and is represented by at least one of the ranges
 * being invalid (that is `till < from`).
 */
struct Block {
    struct Range x;
    struct Range y;
    Block() {}
    Block(bool) { MakeInvalid(); }
    Block(double sx, double dx, double sy, double dy) :
        x(std::min(sx, dx),std::max(sx,dx)), y(std::min(sy, dy),std::max(sy,dy)) {}
    Block(const XY &ul, const XY &dr) :
        x(std::min(ul.x, dr.x),std::max(ul.x, dr.x)), y(std::min(ul.y, dr.y),std::max(ul.y, dr.y)) {}
    Block(const Range &X, const Range &Y) : x(X), y(Y) {}
    void MakeInvalid() {x.MakeInvalid(); y.MakeInvalid();}           ///< Empty the Block.
    bool IsInvalid() const {return x.IsInvalid() || y.IsInvalid();}  ///< Check if Block is empty or not.
    /** Comparison operator required for set ordering.
     *
     * Blocks whoose y coordinate is larger are counted as larger here.
     * Note that this definition improves performance when checking overlaps
     * between a large number of Blocks (lower blocks come later).
     */
    bool operator <(const struct Block &b) const {
        if (y.till != b.y.till) return y.till < b.y.till;
        if (x.till != b.x.till) return x.till < b.x.till;
        if (x.from != b.x.from) return x.from < b.x.from;
        return y.from < b.y.from;
    }
    bool operator == (const struct Block &b) const {return x==b.x && y==b.y;}
    bool Overlaps(const struct Block &b, double gap=0) const
        {return x.Overlaps(b.x, gap) && y.Overlaps(b.y, gap);} ///< Sees if the two blocks have at least `gap` vertical and horizontal distance between them.
    XY UpperLeft(void) const
        {return XY(x.from, y.from);}  ///< Returns the coordinates of the upper-left corner. The one with the smaller y coordinate.
    XY LowerRight(void) const
        {return XY(x.till, y.till);}  ///< Returns the coordinates of the lower-right corner. The one with the larger y coordinate.
    XY UpperRight(void) const
        {return XY(x.till, y.from);}  ///< Returns the coordinates of the upper-right corner. The one with the smaller y coordinate.
    XY LowerLeft(void) const
        {return XY(x.from, y.till);}  ///< Returns the coordinates of the lower-left corner. The one with the larger y coordinate.
    XY Centroid(void) const
        {return XY(x.MidPoint(), y.MidPoint());}  ///< Returns the geometric center of the rectangle.
    XY Spans(void) const
        {return XY(x.Spans(), y.Spans());}        ///< Returns the witdth and the height of the rectangle.
    double GetArea() const {return IsInvalid() ? 0 : x.Spans()*y.Spans();}  ///< Returns the size of the area occupied by the rectangle.
    double GetCircumference() const {return IsInvalid() ? 0 : 2*(x.Spans()+y.Spans());} ///< Returns the length of the circumference of the rectangle.
    EPointRelationType IsWithin(const XY &p) const {
        if (x.IsWithin(p.x) == WI_OUTSIDE   || y.IsWithin(p.y) == WI_OUTSIDE)   return WI_OUTSIDE;
        if (x.IsWithin(p.x) == WI_INSIDE    && y.IsWithin(p.y) == WI_INSIDE)    return WI_INSIDE;
        if (x.IsWithin(p.x) == WI_ON_VERTEX && y.IsWithin(p.y) == WI_ON_VERTEX) return WI_ON_VERTEX;
        return WI_ON_EDGE;
    } ///< Checks if a point is inside the block, on its edges, vertices or outside.
    bool IsWithinBool(const XY &p) const {return x.IsWithinBool(p.x) && y.IsWithinBool(p.y);} ///< Returns true if the point is inside or on an edge/vertex of the Block.
    Block & operator +=(const XY &p)
        {x += p.x; y += p.y; return *this;}  ///< Expands the rectangle to include `p`. Note this is _not_ a shift operation.
    Block & operator +=(const Block &b)
        {x += b.x; y += b.y; return *this;}  ///< Expands the rectangle to include `b`. Note this is _not_ a shift operation.
    Block operator+(const Block &b) const
        {return Block(x+b.x, y+b.y);}        ///< Return the smallest rectangle containing both `this` and `b`.
    Block &Shift(const XY &a)
        {x.Shift(a.x); y.Shift(a.y); return *this;}                                         ///< Shift the rectangle.
    Block &Scale(const XY &sc) {x.Scale(sc.x); y.Scale(sc.y); return *this;}                ///< Scale the rectangle differently in the two dimensions.
    Block &Scale(double sc) {x.Scale(sc); y.Scale(sc); return *this;}                       ///< Scale the rectangle uniformly in both dimensions.
    Block &Expand(double a) {x.Expand(a); y.Expand(a); return *this;}                       ///< Expand the rectangle in all 4 directions by `a`. Sides grow by `2*a`.
    Block CreateExpand(double a) const {Block b(*this); b.Expand(a); return b;}             ///< Return an expanded version, keeping the original const.
    Block &Expand2D(const XY &gap) {x.Expand(gap.x); y.Expand(gap.y); return *this;}        ///< Expand the Block differently in both directions. Horizontal sides grow by `2*gap.x', vertical sides grow by `2*gap.y`.
    Block CreateExpand2D(const XY &gap) const {Block b(*this); b.Expand2D(gap); return b;}  ///< Return a version of the Block expanded differently in the two dimensions.
    Block &SwapXY() {std::swap(x,y); return *this;}                          ///< Swap x and y coordinates.
    Block & Round()       {x.Round(); y.Round(); return *this;}              ///< Round all corners to closest integer coordinates.
    Block & RoundUp()     {x.RoundUp(); y.RoundUp(); return *this;}          ///< Round all corners  upward (towards larger integers)
    Block & RoundDown()   {x.RoundDown(); y.RoundDown(); return *this;}      ///< Round all corners downward (towards smaller integers)
    Block & RoundWider()  {x.RoundWider(); y.RoundWider(); return *this;}    ///< Round corners outward.
    Block & RoundCloser() {x.RoundCloser(); y.RoundCloser(); return *this;}  ///< Round corners inward.

    double Distance(const XY &xy) const;
    double Distance(const Block &b) const;
    Range Cut(const XY &A, const XY &B) const;
    EContourRelationType RelationTo(const Block &c) const {
        if (IsInvalid()) return c.IsInvalid() ? REL_BOTH_EMPTY : REL_A_IS_EMPTY;
        if (c.IsInvalid()) return REL_B_IS_EMPTY;
        const EContourRelationType rx = x.RelationTo(c.x), ry = y.RelationTo(c.y);
        if (rx == ry) return rx;
        if (rx == REL_APART || ry == REL_APART) return REL_APART;
        return REL_OVERLAP;
    } ///> Returns the relation of the two Blocks.
};

/** Returns the distance of `xy` from the Block.
 *
 * This is calculated as the smallest distance between `xy` and any point on the Block edge.
 * If the point is inside, a negative value will result.
 */
inline double Block::Distance(const XY &xy) const
{
    const double xd = x.Distance(xy.x);
    const double yd = y.Distance(xy.y);
    //if both outside we return the distance
    if (fsign(xd)>0 && fsign(yd)>0) return sqrt(xd*xd + yd*yd);
    //if both inside, we are fully inside the box, return the smaller abs value
    if (fsign(xd)<0 && fsign(yd)<0) return std::max(xd, yd);
    //else we return the one with bigger sign (positive or zero)
    return fsign(xd) > fsign(yd) ? xd : yd;
}

/** Returns the distance of Block `b` from the Block.
 *
 * This is calculated as the smallest distance between any point on `B`'s edge
 * and any point on the Block edge.
 * If the two block have crossing edges, the distance is zero.
 * If one block is fully inside the other, a negative value will result.
 */
inline double Block::Distance(const Block &b) const
{
    const double uld = Distance(b.UpperLeft());
    const double urd = Distance(b.UpperRight());
    const double lld = Distance(b.LowerLeft());
    const double lrd = Distance(b.LowerRight());
    //if some squares of b are inside and some are outside (or there is a touch), return 0
    if (fsign(uld) != fsign(lrd) || fsign(uld) != fsign(urd) || fsign(uld) != fsign(lld))
        return 0;
    //if all are either inside or outside, return the one with smallest abs value
    return minabs(minabs(uld, urd), minabs(lld, lrd));
}

} //namespace

#endif //CONTOUR_BASICS_H
