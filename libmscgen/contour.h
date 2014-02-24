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

/** @file contour.h Declares class ContourList, ContourWithHoles and Contour.
 * @ingroup contour_files
 */

#if !defined(CONTOUR_CONTOURS_H)
#define CONTOUR_CONTOURS_H

#include <list>
#include "contour_simple.h"

namespace contour {


class ContourWithHoles;

/** A list of non-overlapping ContourWithHoles 
 * @ingroup contour_internal
 *
 * A building block toward the overall Contour class.
 * Essentially functions working on a list of ContourWithHoles objects.
 * Should only contain contours with the same clockwiseness. It can be clockwise,
 * in which case we are a collection of shapes (possibly with holes inside them),
 * or it can be counterclockwise, in which case we are a list of holes inside a
 * shape (with possibly each hole containing some positive shapes in them).
 *
 * When we say "non-overlapping" we mean that no two shapes have intersections
 * and they do not even touch by edge. However, it is possible that they touch
 * by vertex. That is, a vertex of one shape may lie on a vertex or edge of
 * another.
 */

class ContourList : protected std::list<ContourWithHoles>
{
    friend class ContourWithHoles;
    friend class Contour;
    friend class ContoursHelper;
    friend void contour_test(void);

    Block boundingBox; ///< Bounding box containing all of the ContourWithHole shapes.

    ContourList() {boundingBox.MakeInvalid();}

    void append(const ContourWithHoles &p);  ///<Append a shape & update bounding box. Assumes the shape not overlapping with us.
    void append(ContourWithHoles &&p);       ///<Append (move) a shape & update bounding box. Assumes the shape not overlapping with us.
    void append(const ContourList &p) {_ASSERT(p.size()==0 || size()==0 || GetClockWise() == p.GetClockWise()); boundingBox += p.GetBoundingBox(); insert(end(), p.begin(), p.end());} ///<Append a list of shapes & update bounding box. Assumes none of the shapes is overlapping with us.
    void append(ContourList &&p) {_ASSERT(p.size()==0 || size()==0 || GetClockWise() == p.GetClockWise()); boundingBox += p.GetBoundingBox(); splice(end(), p);} ///<Append (move) a list of shapes & update bounding box. Assumes none of the shapes is overlapping with us.

    void Invert();

    void Shift(const XY &xy);
    void Scale(double sc);
    void Scale(const XY &sc);
    void SwapXY();
    void Rotate(double cos, double sin);
    void RotateAround(const XY&c, double cos, double sin);

    EContourRelationType RelationTo(const ContourWithHoles &c, bool ignore_holes) const;
    EContourRelationType RelationTo(const ContourList &c, bool ignore_holes) const;
    void Distance(const ContourWithHoles &c, DistanceType &dist_so_far) const;
    void Distance(const ContourList &cl, DistanceType &dist_so_far) const;
public:
    const ContourWithHoles & operator[](size_type i) const;
    void swap(ContourList &a) {std::list<ContourWithHoles>::swap(a); std::swap(boundingBox, a.boundingBox);}  ///<Swap content with another ContourList object
    void clear() {std::list<ContourWithHoles>::clear(); boundingBox.MakeInvalid();} ///<Clear content, making the list empty
    size_type size() const {return std::list<ContourWithHoles>::size();} ///<Return the number of elements in the list
    bool operator < (const ContourList &p) const;
    bool operator ==(const ContourList &p) const;
    bool IsEmpty() const {return size()==0;} ///<True if the list is empty
    const Block &GetBoundingBox(void) const {return boundingBox;} ///<Return the bounding box of the list. Invalid if the list is empty.
    bool GetClockWise() const;
    double GetArea(bool consider_holes=true) const;
    double GetCircumference(bool consider_holes=true, bool include_hidden=false) const;
    XY CentroidUpscaled() const;

    EPointRelationType IsWithin(const XY &p) const;
    EPointRelationType Tangents(const XY &p, XY &t1, XY &t2) const;
    void VerticalCrossSection(double x, DoubleMap<bool> &section) const;
    double OffsetBelow(const SimpleContour &below, double &touchpoint, double offset=CONTOUR_INFINITY) const;
    double OffsetBelow(const Contour &below, double &touchpoint, double offset=CONTOUR_INFINITY) const;
    double OffsetBelow(const ContourList &below, double &touchpoint, double offset=CONTOUR_INFINITY) const;

    void CairoPath(cairo_t *cr, bool show_hidden) const;
    void CairoPath(cairo_t *cr, bool show_hidden, bool clockwiseonly) const;
    void CairoPathDashed(cairo_t *cr, const double pattern[], unsigned num, bool show_hidden) const;
    void CairoPathDashed(cairo_t *cr, const double pattern[], unsigned num, bool show_hidden, bool clockwiseonly) const;

    double Distance(const XY &o, XY &ret) const;
    double DistanceWithTangents(const XY &o, XY &ret, XY &t1, XY &t2) const;
    Range Cut(const Edge &e) const;
    Range CutWithTangent(const Edge &e, std::pair<XY, XY> &from, std::pair<XY, XY> &till) const;
    void Cut(const XY &A, const XY &B, DoubleMap<bool> &map) const;
    bool TangentFrom(const XY &from, XY &clockwise, XY &cclockwise) const;
};

/** A single contigous shape with potentially a list of non-overlapping holes.
 * @ingroup contour_intermal
 *
 * Or the opposite: a single contigous hole with potentially a list of non-overlapping
 * shapes in it. This is decided by the clockwiseness of the `outline` member.
 * The `holes` member shall have opposite clockwisedness than `outline`.
 *
 * For simplicity of description we will assume the outline is clockwise.
 *
 * The holes must be fully inside the outline. By we mean that no part of a hole
 * lies outside the outline and they do not even touch by edge. 
 * However, it is possible that they touch
 * by vertex. That is, a vertex of a hole or the outline may lie on a vertex or edge of
 * the shape or a hole, respectively.
 */
class ContourWithHoles
{
    friend class SimpleContour;
    friend class ContourList;
    friend class Contour;
    friend class ContoursHelper;
    friend void contour_test(void);

protected:
    SimpleContour outline; ///<The outer shape
    ContourList   holes;   ///<The list of holes in it - inside `outline`

    ContourWithHoles() {}; ///<Create an empty shape.
    ContourWithHoles(double sx, double dx, double sy, double dy) : outline(sx, dx, sy, dy) {} ///<Create a rectangular shape
    ContourWithHoles(const Block &b) : outline(b) {} ///<Create a rectangular shape
    ContourWithHoles(XY a, XY b, XY c) : outline(a,b,c) {} ///<Create a triangle
    ContourWithHoles(double ax, double ay, double bx, double by, double cx, double cy) : outline(ax, ay, bx, by, cx, cy) {} ///<Create a triangle
    /** Create an ellipse (or ellipse slice) shape.
     *
     * @param [in] c Centerpoint
     * @param [in] radius_x Radius in the x direction.
     * @param [in] radius_y Radius in the y direction (same as `radius_x` if omitted = circle)
     * @param [in] tilt_deg The tilt of the ellipse in degrees. 0 if omitted.
     * @param [in] s_deg The startpoint of the arc.
     * @param [in] d_deg The endpoint of the arc. If unequal to `s_deg` a straight line is added to close the arc.
    */
    ContourWithHoles(const XY &c, double radius_x, double radius_y=0, double tilt_deg=0, double s_deg=0, double d_deg=360) :
        outline(c, radius_x, radius_y, tilt_deg, s_deg, d_deg) {} 

    void assign(const SimpleContour &a) {outline = a; holes.clear();} ///<Assign a SimpleContour to us (no holes).
    void assign(SimpleContour &&a) {outline.swap(a); holes.clear(); } ///<Move a SimpleContour to us (no holes). `a` becomes undefined
    void assign(const ContourWithHoles &a) {operator=(a);}            ///<Copy a ContourWithHoles to us.
    void assign(ContourWithHoles &&a) {if (this!=&a) swap(a);}        ///<Move a ContourWithHoles to us. `a` becomes undefined.

    void clear() {outline.clear(); holes.clear();} ///<Delete all content.

    void Invert() {outline.Invert(); if (holes.size()) holes.Invert();} ///<Reverse the direction (clockwise/counterclockwise) of both `outline` and `holes`.
    void Shift(const XY &xy) {outline.Shift(xy); if (holes.size()) holes.Shift(xy);} ///<Translate the shape.
    void Scale(double sc) { outline.Scale(sc); if (holes.size()) holes.Scale(sc); } ///<Scale the shape.
    void Scale(const XY& sc) { outline.Scale(sc); if (holes.size()) holes.Scale(sc); } ///<Scale the shape.
    void SwapXY() { outline.SwapXY(); if (holes.size()) holes.SwapXY(); } ///<Transpose the shape: swap X and Y coordinates (but keep clockwisedness). 
    void Rotate(double cos, double sin)
        {outline.Rotate(cos, sin); if (holes.size()) holes.Rotate(cos, sin);} ///<Rotate the shape by `radian`. `sin` and `cos` are pre-computed values.
    void RotateAround(const XY&c, double cos, double sin)
        {outline.RotateAround(c, cos, sin); if (holes.size()) holes.RotateAround(c, cos, sin);} ///<Rotate the shape around 'c' by `radian`. `sin` and `cos` are pre-computed values.
    void Expand(EExpandType type4positive, EExpandType type4negative, double gap, Contour &res,
                double miter_limit_positive, double miter_limit_negative) const;
    void Expand2D(const XY &gap, Contour &res) const;
    EContourRelationType RelationTo(const ContourWithHoles &c, bool ignore_holes) const;
    EContourRelationType RelationTo(const ContourList &c, bool ignore_holes) const {return switch_side(c.RelationTo(*this, ignore_holes));} ///<Retermine a relation to a list of shapes.

    void Distance(const ContourWithHoles &c, DistanceType &ret) const;

public:
    ContourWithHoles(const SimpleContour &p) : outline(p) {} ///<Create a shape with no holes.
    ContourWithHoles(const ContourWithHoles &p) : outline(p.outline), holes(p.holes) {}  ///<Copy constructor, must be added explicitly for g++
    ContourWithHoles(ContourWithHoles &&p) : outline(std::move(p.outline)), holes(std::move(p.holes)) {}  ///<Move constructor. Must be public for list<ContourWithHoles>
    void swap(ContourWithHoles &b) {holes.swap(b.holes); outline.swap(b.outline);} ///<Swap content with another ContourWithHoles.
    bool operator < (const ContourWithHoles &p) const {return outline==p.outline ? holes < p.holes : outline<p.outline;} ///<Comparision operator for container ordering.
    bool operator ==(const ContourWithHoles &p) const {return outline==p.outline && holes == p.holes;} ///<Tests equality. Returns true only if even the order of vertices and holes is identical.
    size_t size() const {return outline.size();} ///<Returns the number of edges in `outline`
    const Edge &operator[](size_t edge) const {return outline[edge];} ///<Returns edge number `edge`. Throws exception if out-of range.
    ContourWithHoles &operator = (const Block &a) {outline = a; holes.clear(); return *this;} ///<Assigns a rectangular shape with no holes.
    ContourWithHoles &operator = (const SimpleContour &a) {outline=a; holes.clear(); return *this;} ///<Assigns a shape with no holes.
    ContourWithHoles &operator = (SimpleContour &&a) {outline.swap(a); holes.clear(); return *this;} ///<Moves a shape with no holes. `a` becomes undefined.
    ContourWithHoles &operator = (const ContourWithHoles &a) {if (this!=&a) outline = a.outline; holes=a.holes; return *this;} ///<Copy a shape.
    ContourWithHoles &operator = (ContourWithHoles &&a) {if (this!=&a) swap(a); return *this;} ///<Move a shape, `a` becomes undefined.

    bool IsSane(bool shouldbehole=false) const;
    bool IsEmpty() const {return outline.IsEmpty();} ///<True if shape has no outline (and holes).

    EPointRelationType IsWithin(const XY &p) const;
    EPointRelationType Tangents(const XY &p, XY &t1, XY &t2) const;
    bool HasHoles() const {return !holes.IsEmpty();} ///<True if shape has holes.
    const ContourList &Holes() const {return holes;} ///<Return the holes
    const Block &GetBoundingBox(void) const {return outline.GetBoundingBox();} ///<Return the bounding box.
    bool GetClockWise() const {return outline.GetClockWise();} ///<Ture if the `outline` is clockwise.
    double GetArea(bool consider_holes=true) const {return consider_holes && !holes.IsEmpty() ? outline.GetArea() + holes.GetArea(true) : outline.GetArea();} ///<Return surface area. Holes are excluded. Negative if `outline` is counterclockwise.
    double GetCircumference(bool consider_holes=true, bool include_hidden=false) const {return consider_holes ? outline.GetCircumference(include_hidden) + holes.GetCircumference(true, include_hidden) : outline.GetCircumference(include_hidden);} ///<Return the length of the circumference. Holes are added in.
    XY CentroidUpscaled() const {return holes.IsEmpty() ? outline.CentroidUpscaled() : outline.CentroidUpscaled() + holes.CentroidUpscaled();} ///<Returns the centroid of the shape, scaled by its (potentially negative) area.

    /** Creates a cross-section of the ContourWithHoles along a vertical line.
     *
     * @param [in] x The x coordinate of the vertical line
     * @param section A DoubleMap containing true for points inside the shape, false for outside. May contain entries already.
     */
    void VerticalCrossSection(double x, DoubleMap<bool> &section) const {outline.VerticalCrossSection(x, section); if (holes.size()) holes.VerticalCrossSection(x, section);}
    /** Determine the relative vertical distance between two shapes.
     *
     * For detailed explanation and parameters, see 
     * Contour::OffsetBelow(const Contour &below, double &touchpoint, double offset).
     */
    double OffsetBelow(const SimpleContour &below, double &touchpoint, double offset=CONTOUR_INFINITY) const {return outline.OffsetBelow(below, touchpoint, offset);}

    void CairoPath(cairo_t *cr, bool show_hidden) const { outline.CairoPath(cr, show_hidden); if (holes.size()) holes.CairoPath(cr, show_hidden); } ///< Draw the shape as a path to a cairo context. If `show_hidden` is false, we skip edges marked not visible.
    void CairoPath(cairo_t *cr, bool show_hidden, bool clockwiseonly) const { outline.CairoPath(cr, show_hidden, clockwiseonly); if (holes.size()) holes.CairoPath(cr, show_hidden, clockwiseonly); } ///<Draw the shape to the path of a cairo context, but only if `clockwiseonly` equals to the clockwisedness of this shape. If `show_hidden` is false, we skip edges marked not visible.
    void CairoPathDashed(cairo_t *cr, const double pattern[], unsigned num, bool show_hidden) const { outline.CairoPathDashed(cr, pattern, num, show_hidden); if (holes.size()) holes.CairoPathDashed(cr, pattern, num, show_hidden); } ///<Draw the shape in dashed lines to the path of a cairo context. Needed for backends not supporting dashed lines.
    void CairoPathDashed(cairo_t *cr, const double pattern[], unsigned num, bool show_hidden, bool clockwiseonly) const { outline.CairoPathDashed(cr, pattern, num, show_hidden, clockwiseonly); if (holes.size()) holes.CairoPathDashed(cr, pattern, num, show_hidden, clockwiseonly); } ///<Draw the shape in dashed lines to the path of a cairo context. Needed for backends not supporting dashed lines.

    double Distance(const XY &o, XY &ret) const;
    double DistanceWithTangents(const XY &o, XY &ret, XY &t1, XY &t2) const;
    Range Cut(const Edge &e) const {return outline.Cut(e);} ///< Returns a cut of the shape along an edge
    Range CutWithTangent(const Edge &e, std::pair<XY, XY> &from, std::pair<XY, XY> &till) const {return outline.CutWithTangent(e, from, till);} ///<Returns a cut of the shape along an edge (with tangents).
    void Cut(const XY &A, const XY &B, DoubleMap<bool> &map) const {outline.Cut(A, B, map); holes.Cut(A, B, map);} ///< Returns a cut of the shape along an infinite line
};

/** The main class in the library - an arbitrary shape (non-contigous, may have holes).
 * @ingroup contour
 *
 * It contains a set of disjoint positive shapes with potentially holes in them. A wide variety of operations 
 * are defined, see the public members.
 */
class Contour
{
    friend void contour_test(void);
    friend class SimpleContour;
    friend class ContourWithHoles;
    friend class ContourList;
    friend class ContoursHelper;
    template <typename LT> friend void Distance(const LT &list, const Contour &c, DistanceType &dist_so_far);
    /** A list of possible operations on contours. 
     *
     * Used internally. There are operations for union, intersection and xor
     * (substraction is implemeted via union). There are two sets of these operations
     * depending on whether we expect to get a clockwise or counterclockwise shape.
     * If we take the union of two clockwise surface, we should use POSITIVE_UNION,
     * if we take the union of two holes (counterclockwise), we should use NEGATIVE_UNION.
     * If the input shapes differ in clockwiseness, man, think about what you really want....
     * (The only legitimate situation I can think of is substraction, like A-B, whern 
     * we take the union of A and the inverse of B. But in this case we use POSITIVE or
     * NEGATIVE depending on the clockwiseness of 'A'.)
     * This is important, for in case of a positive expectation we discard standalone parts 
     * of the result if they are counterclockwise (a hole that is not inside something); 
     * and vice versa for negative expectations.
     *
     * Then there are two rules for untangling, one for each winding rule.
     *
     * Finally there are two rules for untangling after expansion, depending again on
     * whether we want a clockwise or counterclockwise surface.
     */

    enum EOperationType {
        POSITIVE_UNION=0,     ///<Take the union of two (or more) positive shapes. Include places covered by any of the shapes.
        POSITIVE_INTERSECT,   ///<Take the intersection of two (or more) positive shapes. Include places covered by all of the shapes.
        POSITIVE_XOR,         ///<Take the xor of two (or more) positive shapes. Include places covered by odd number of the shapes. 
        WINDING_RULE_NONZERO, ///<Untangle a set of edges using the nonzero winding rule.
        WINDING_RULE_EVENODD, ///<Untangle a set of edges using the evenodd winding rule.
        EXPAND_POSITIVE,      ///<Untangle a contour resulting from expanding a positive SimpleContour.
        NEGATIVE_UNION,       ///<Take the union of two (or more) negative (counterclockwise) shapes (holes). The resulting hole will include places included in any of the holes.
        NEGATIVE_INTERSECT,   ///<Take the intersection of two (or more) negative (counterclockwise) shapes (holes). The resulting hole will include places included in all of the holes.
        NEGATIVE_XOR,         ///<Take the xor of two (or more) negative (counterclockwise) shapes (holes). The resulting hole will include places included in an odd number of the holes.
        EXPAND_NEGATIVE       ///<Untangle a contour resulting from expanding a counterclockwise SimpleContour.
    };
    static bool is_positive(EOperationType t) {return t<=EXPAND_POSITIVE;} ///<True if the operation works on and hence produces clockwise shapes.
    /** The first contigous shape in the list of disjoint shapes.
     * 
     * The first such shape is a separate member. This is to reduce heap operations, 
     * when the Contour consists only of a single contigous member (most common case).
     * In this case the ContourWithHoles associated with that single member will not 
     * be allocated on the heap as the sole member of a list. 
     * 
     * In the most common case of the shape containing no holes, the only heap 
     * allocation will be for the vector of edges.
     */
    ContourWithHoles first; 
    ContourList further; ///< If the contour has multiple disjoint shapes, this contains all of them but the first.
    Block boundingBox;   ///< The bounding box of the contour (combining that of `first` and `further`).

    friend Contour operator + (const Contour &a, const Contour &b);
    friend Contour operator * (const Contour &a, const Contour &b);
    friend Contour operator - (const Contour &a, const Contour &b);
    friend Contour operator ^ (const Contour &a, const Contour &b);
    friend Contour operator + (const Contour &a, Contour &&b);
    friend Contour operator * (const Contour &a, Contour &&b);
    friend Contour operator - (const Contour &a, Contour &&b);
    friend Contour operator ^ (const Contour &a, Contour &&b);
    friend Contour operator + (Contour &&a, const Contour &b);
    friend Contour operator * (Contour &&a, const Contour &b);
    friend Contour operator - (Contour &&a, const Contour &b);
    friend Contour operator ^ (Contour &&a, const Contour &b);
    friend Contour operator + (Contour &&a, Contour &&b);
    friend Contour operator * (Contour &&a, Contour &&b);
    friend Contour operator - (Contour &&a, Contour &&b);
    friend Contour operator ^ (Contour &&a, Contour &&b);

    /** @name Private constructors
     * @{ */
    /** Create a contour by executing an operation on two contours */
    Contour(EOperationType type, const Contour &c1, const Contour &c2) {Operation(type, c1, c2);} 
    Contour(EOperationType type, const Contour &c1, Contour &&c2) {Operation(type, c1, std::move(c2));}
    Contour(EOperationType type, Contour &&c1, Contour &&c2) {Operation(type, std::move(c1), std::move(c2));}
    //@} 
protected:
    void append(const ContourWithHoles &p) {if (p.IsEmpty()) return; if (IsEmpty()) {boundingBox = p.GetBoundingBox(); first.assign(p);} else {boundingBox+=p.GetBoundingBox(); further.append(p);}} ///<Append a shape assuming it is disjoint.
    void append(ContourWithHoles &&p)  {if (p.IsEmpty()) return; if (IsEmpty()) {boundingBox = p.GetBoundingBox(); first.assign(std::move(p));} else {boundingBox+=p.GetBoundingBox(); further.append(std::move(p));}} ///<Append (move) a shape assuming it is disjoint.

    void Invert(); ///<Reverse the contour. Used internally for substraction. Untangles the resulting shape.
    Contour& invert_dont_check() {first.Invert(); if (further.size()) further.Invert(); return *this;} ///<Reverse the contour. Used internally for substraction. Does not untangle the resulting shape.
    Contour CreateInverse() const {Contour tmp(*this); tmp.Invert(); return tmp;} ///<Creates a Contour that is the invese of us.
    Contour create_inverse_dont_check() const { Contour tmp(*this); tmp.invert_dont_check(); return tmp; } ///<Creates a Contour that is the invese of us. Used internally for substraction. Does not untangle the resulting shape.
    void Rotate(double cos, double sin) {first.Rotate(cos, sin); boundingBox = first.GetBoundingBox(); if (further.size()) {further.Rotate(cos, sin); boundingBox += further.GetBoundingBox();}} ///<Helper: Rotate the shape by `radian`. `sin` and `cos` are pre-computed values.
    void RotateAround(const XY&c, double cos, double sin) {first.RotateAround(c, cos, sin); boundingBox = first.GetBoundingBox(); if (further.size()) {further.RotateAround(c, cos, sin); boundingBox += further.GetBoundingBox();}} ///<Helper: Rotate the shape around `c` by `radian`. `sin` and `cos` are pre-computed values.

    /** @name Operation() varianta. 
     * Performs an operation on one or two contours and stores the result in `this`. Used internally.*/
    /** @{ */
    void Operation(EOperationType type, const Contour &c1);
    void Operation(EOperationType type, Contour &&c1);
    void Operation(EOperationType type, const Contour &c1, const Contour &c2);
    void Operation(EOperationType type, const Contour &c1, Contour &&c2);
    void Operation(EOperationType type, Contour &&c1, Contour &&c2);
    //@} 

    /** Helper: Expands the shape and stores the result in res.
     *
     * @param [in] type4positive Speciies how to handle joins for clockwise contours.
     * @param [in] type4negative Speciies how to handle joins for counterclockwise contours (that is, holes).
     * @param [in] gap The amount to expand (shrink if <0).
     * @param [out] res The restult is stored here.
     * @param [in] miter_limit_positive The miter limit for the clockwise contours.
     * @param [in] miter_limit_negative The miter limit for the counterclockwise contours.
     */
    void Expand(EExpandType type4positive, EExpandType type4negative, double gap, Contour &res,
                double miter_limit_positive, double miter_limit_negative) const;
    void Expand2D(const XY &gap, Contour &res) const; ///<Helper: perform an Expand2D operation.
    void Distance(const Contour &c, DistanceType &dist_so_far) const; ///<Helper: calculates the distance
public:
    /** @name Constructors */
    /** @{ */
    Contour() {boundingBox.MakeInvalid();} ///<Creates an empty Contour.
    /** Creates a rectangle.
     *
     * @param [in] sx X coordinate of the left side.
     * @param [in] dx X coordinate of the right side.
     * @param [in] sy Y coordinate of the upper side.
     * @param [in] dy Y coordinate of the lower side.
     *
     * Coordinates are reversed if they are in the wrong order, e.g., dx<sx or dy<sy. 
     * An empty contour is created if sx==dx or sy==dy. */
    Contour(double sx, double dx, double sy, double dy) : first(sx, dx, sy, dy) {boundingBox = first.GetBoundingBox();} 
    Contour(const Block &b) : first(b), boundingBox(b) {} ///<Creates a rectangle specified by Block.
    /** Creates a triangle. 
     *
     * Irrespective of the order and arrangement of the points a clockwise triangle is created.
     * An empty contour is created if two of the points equal or they fall on a line. */
    Contour(XY a, XY b, XY c) : first(a,b,c) {boundingBox = first.GetBoundingBox();} 
    /** Creates a triangle. 
     *
     * Irrespective of the order and arrangement of the points a clockwise triangle is created.
     * An empty contour is created if two of the points equal or they fall on a line. */
    Contour(double ax, double ay, double bx, double by, double cx, double cy) : first(ax, ay, bx, by, cx, cy) {boundingBox = first.GetBoundingBox();} ///<Creates a triangle
    /** Create an ellipse (or ellipse slice) shape.
     *
     * @param [in] c Centerpoint
     * @param [in] radius_x Radius in the x direction. Its absolute value is used.
     * @param [in] radius_y Radius in the y direction (same as `radius_x` if omitted = circle) Its absolute value is used.
     * @param [in] tilt_deg The tilt of the ellipse in degrees. 0 if omitted.
     * @param [in] s_deg The startpoint of the arc.
     * @param [in] d_deg The endpoint of the arc. If equal to `s_deg` a full ellipse results, else a straight line is added to close the arc.
     *
     * If `radius_x` is zero, we return an empty shape. If `radius_y` is zero, we assume it to be the same as `radius_x` (circle). */
    Contour(const XY &c, double radius_x, double radius_y=0, double tilt_deg=0, double s_deg=0, double d_deg=360) :
        first (c, radius_x, radius_y, tilt_deg, s_deg, d_deg) {boundingBox = first.GetBoundingBox();} 
    /** Creates a polygon from an ordered list of points. Untangles them using the winding rule if `winding` is true, using evenodd rule otherwise. */
    Contour(const std::vector<XY> &v, bool winding=true) {assign(v, winding);} 
    /** Creates a polygon from an ordered list of points. Untangles them using the winding rule if `winding` is true, using evenodd rule otherwise. */
    Contour(const XY v[], size_t size, bool winding=true) {assign (v, size, winding);} 
    /** Creates a polygon from an ordered list of points. Untangles them using the winding rule if `winding` is true, using evenodd rule otherwise. */
    template<size_t SIZE> Contour(const XY (&v)[SIZE], bool winding=true) {assign (v, SIZE, winding);} 
    /** Creates a shape from an ordered list of edges. Untangles them using the winding rule if `winding` is true, using evenodd rule otherwise. */
    Contour(const std::vector<Edge> &v, bool winding=true) {assign (v, winding);} 
    /** Creates a shape from an ordered list of edges. Untangles them using the winding rule if `winding` is true, using evenodd rule otherwise. */
    Contour(const Edge v[], size_t size, bool winding=true) {assign (v, size, winding);}
    /** Creates a shape from an ordered list of edges. Untangles them using the winding rule if `winding` is true, using evenodd rule otherwise. */
    template<size_t SIZE> Contour(const Edge (&v)[SIZE], bool winding=true) {assign (v, SIZE, winding);}

    Contour(const ContourWithHoles &p) : first(p) {boundingBox = first.GetBoundingBox();}       ///<Create a contour of a single shape by copying a ContourWithHoles object.
    Contour(const SimpleContour &p) : first(p) {boundingBox = first.GetBoundingBox();}          ///<Create a contour of a single shape with no holes by copying a SimpleContour object.
    Contour(ContourWithHoles &&p) : first(std::move(p)) {boundingBox = first.GetBoundingBox();} ///<Create a contour of a single shape by moving a ContourWithHoles object. `p` will become undefined.
    Contour(SimpleContour &&p) : first(std::move(p)) {boundingBox = first.GetBoundingBox();}    ///<Create a contour of a single shape with no holes by moving a SimpleContour object. `p` will become undefined.
    Contour(Contour &&a) : first(std::move(a.first)), further(std::move(a.further)), boundingBox(a.GetBoundingBox()) {}    ///<Standard move constructor `p` will become undefined.
    Contour(const Contour &a) : first(a.first), further(a.further), boundingBox(a.GetBoundingBox()) {}                     ///<Standard copy constructor.
    explicit Contour(const Path &p, bool close_everything = false, bool winding = true) { assign(p, close_everything, winding); }
    explicit Contour(Path &&p, bool close_everything = false, bool winding = true) { assign(std::move(p), close_everything, winding); }
    /** @} */ //Constructors

    /** @name Assignment */
    /** @{ */
    /** Set the Contour to a rectangle */
    Contour &operator = (const Block &a) {first = a; further.clear(); boundingBox = a; return *this;}
    /** Set the Contour to a simple shape with no holes */
    Contour &operator = (const SimpleContour &a) {first.operator=(a); further.clear(); boundingBox = first.GetBoundingBox(); return *this;}
    /** Set the Contour to a simple shape with no holes by moving it. `a` becomes undefined.*/
    Contour &operator = (SimpleContour &&a) {first.operator=(std::move(a)); boundingBox = first.GetBoundingBox(); return *this;}
    /** Set the Contour to a contigous shape.*/
    Contour &operator = (const ContourWithHoles &a) {first.operator=(a); further.clear(); boundingBox = first.GetBoundingBox(); return *this;}
    /** Set the Contour to a contigous shape by moving it. `a` becomes undefined.*/
    Contour &operator = (ContourWithHoles &&a) {first.operator=(std::move(a)); further.clear(); boundingBox = first.GetBoundingBox(); return *this;}
    /** Standard assignment move operation. `a` becomes undefined. */
    Contour &operator = (Contour &&a) { first.swap(a.first); further.swap(a.further); boundingBox = a.GetBoundingBox(); return *this; }
    /** Standard assignment operation. */
    Contour &operator = (const Contour &a) { first = a.first; further = a.further; boundingBox = a.GetBoundingBox(); return *this; }
    /** Sets the Contour to the enclosed area(s) of a path. Untangles them.*/
    Contour &operator = (const Path &p) { assign(p); return *this; }
    /** Sets the Contour to the enclosed area(s) of a path. Untangles them.*/
    Contour &operator = (Path &&p) { assign(std::move(p)); return *this; }

    /** Sets the Contour to a polygon from an ordered list of points. Untangles them using the winding rule if `winding` is true, using evenodd rule otherwise. */
    Contour &assign(const std::vector<XY> &v, bool winding=true);
    /** Sets the Contour to a polygon from an ordered list of points. Untangles them using the winding rule if `winding` is true, using evenodd rule otherwise. */
    Contour &assign(const XY v[], size_t size, bool winding=true);
    /** Sets the Contour to a polygon from an ordered list of points. Untangles them using the winding rule if `winding` is true, using evenodd rule otherwise. */
    template<size_t SIZE> Contour &assign(const XY (&v)[SIZE], bool winding=true) {assign (v, SIZE, winding);return *this;}
    /** Sets the Contour to a shape with no holes from an ordered list of edges. Untangles them using the winding rule if `winding` is true, using evenodd rule otherwise. */
    Contour &assign(const std::vector<Edge> &v, bool winding = true);
    /** Sets the Contour to a shape with no holes from an ordered list of edges. Untangles them using the winding rule if `winding` is true, using evenodd rule otherwise. */
    Contour &assign(std::vector<Edge> &&v, bool winding = true);
    /** Sets the Contour to a shape with no holes from an ordered list of edges. Untangles them using the winding rule if `winding` is true, using evenodd rule otherwise. */
    Contour &assign(const Edge v[], size_t size, bool winding=true);
    /** Sets the Contour to a shape with no holes from an ordered list of edges. Untangles them using the winding rule if `winding` is true, using evenodd rule otherwise. */
    template<size_t SIZE> Contour &assign(const Edge (&v)[SIZE], bool winding=true) {assign (v, SIZE, winding);return *this;}
    /** Sets the Contour to the enclosed area(s) of a path. Untangles them using the winding rule if `winding` is true, using evenodd rule otherwise. */
    Contour &assign(const Path &p, bool close_everything = false, bool winding = true);
    /** Sets the Contour to the enclosed area(s) of a path. Untangles them using the winding rule if `winding` is true, using evenodd rule otherwise. */
    Contour &assign(Path &&p, bool close_everything = false, bool winding = true);

    /** Sets the Contour to a polygon from an ordered list of points. Assumes the poins specify an untangled polygon. */
    Contour &assign_dont_check(const std::vector<XY> &v) {clear(); first.outline.assign_dont_check(v); boundingBox = first.GetBoundingBox();return *this;}
    /** Sets the Contour to a polygon from an ordered list of points. Assumes the poins specify an untangled polygon. */
    Contour &assign_dont_check(const XY v[], size_t size) {clear(); first.outline.assign_dont_check(v, size); boundingBox = first.GetBoundingBox(); return *this;}
    /** Sets the Contour to a polygon from an ordered list of points. Assumes the poins specify an untangled polygon. */
    template<size_t SIZE> Contour &assign_dont_check(const XY (&v)[SIZE]) {assign_dont_check (v, SIZE);return *this;}
    /** Sets the Contour to a shape with no holes from an ordered list of edges. Assumes the poins specify an untangled polygon. */
    Contour &assign_dont_check(const std::vector<Edge> &v) {clear();  first.outline.assign_dont_check(v); boundingBox = first.GetBoundingBox(); return *this;}
    /** Sets the Contour to a shape with no holes from an ordered list of edges. Assumes the poins specify an untangled polygon. */
    Contour &assign_dont_check(std::vector<Edge> &&v) {clear();  first.outline.assign_dont_check(std::move(v)); boundingBox = first.GetBoundingBox(); return *this;}
    /** Sets the Contour to a shape with no holes from an ordered list of edges. Assumes the poins specify an untangled polygon. */
    Contour &assign_dont_check(const Edge v[], size_t size) {clear(); first.outline.assign_dont_check(v, size); boundingBox = first.GetBoundingBox(); return *this;}
    /** Sets the Contour to a shape with no holes from an ordered list of edges. Assumes the poins specify an untangled polygon. */
    template<size_t SIZE> Contour &assign_dont_check(const Edge (&v)[SIZE]) {assign_dont_check (v, SIZE);return *this;}
    Contour &assign_dont_check(const Path &p, bool close_everything = false) { clear(); for (auto &e : p.ConvertToClosed(close_everything)) append_dont_check(e); return *this;}
    Contour &assign_dont_check(Path &&p, bool close_everything = false);

    /** Appends polygon created from an ordered list of points. Assumes the poins specify an untangled polygon that does not overlap with the existing Contour. */
    Contour &append_dont_check(const std::vector<XY> &v) {ContourWithHoles tmp; tmp.outline.assign_dont_check(v); if (!tmp.IsEmpty()) append(std::move(tmp));return *this;}
    /** Appends polygon created from an ordered list of points. Assumes the poins specify an untangled polygon that does not overlap with the existing Contour. */
    Contour &append_dont_check(const XY v[], size_t size) {ContourWithHoles tmp; tmp.outline.assign_dont_check(v, size); if (!tmp.IsEmpty()) append(std::move(tmp));return *this;}
    /** Appends polygon created from an ordered list of points. Assumes the poins specify an untangled polygon that does not overlap with the existing Contour. */
    template<size_t SIZE> Contour &append_dont_check(const XY (&v)[SIZE]) {append_dont_check (v, SIZE);return *this;}
    /** Appends shape created from an ordered list of edges. Assumes the poins specify an untangled polygon that does not overlap with the existing Contour. */
    Contour &append_dont_check(const std::vector<Edge> &v) {ContourWithHoles tmp; tmp.outline.assign_dont_check(v); if (!tmp.IsEmpty()) append(std::move(tmp));return *this;}
    /** Appends shape created from an ordered list of edges. Assumes the poins specify an untangled polygon that does not overlap with the existing Contour. */
    Contour &append_dont_check(std::vector<Edge> &&v) {ContourWithHoles tmp; tmp.outline.assign_dont_check(std::move(v)); if (!tmp.IsEmpty()) append(std::move(tmp)); return *this;}
    /** Appends shape created from an ordered list of edges. Assumes the poins specify an untangled polygon that does not overlap with the existing Contour. */
    Contour &append_dont_check(const Edge v[], size_t size) {ContourWithHoles tmp; tmp.outline.assign_dont_check(v, size); if (!tmp.IsEmpty()) append(std::move(tmp));return *this;}
    /** Appends shape created from an ordered list of edges. Assumes the poins specify an untangled polygon that does not overlap with the existing Contour. */
    template<size_t SIZE> Contour &append_dont_check(const Edge (&v)[SIZE]) {append_dont_check (v, SIZE);return *this;}
    /** @return *this;} */ //assignment


    /** @name Basic information 
     * @{ */
    size_t size() const {if (first.outline.size()) return further.size()+1; return 0;} ///< Returns 
    bool IsEmpty() const {return first.IsEmpty();}
    const Block &GetBoundingBox(void) const {return boundingBox;}
    bool GetClockWise() const {return first.GetClockWise();}
    double GetArea(bool consider_holes=true) const {return further.size() ? further.GetArea(consider_holes) + first.GetArea(consider_holes) : first.GetArea(consider_holes);}
    double GetCircumference(bool consider_holes=true, bool include_hidden=false) const {return further.size() ? further.GetCircumference(consider_holes, include_hidden) + first.GetCircumference(consider_holes, include_hidden) : first.GetCircumference(consider_holes, include_hidden);}
    XY Centroid() const {return (further.IsEmpty() ? first.CentroidUpscaled() : first.CentroidUpscaled()+further.CentroidUpscaled())/GetArea();}
    //bool AddAnEdge(const Edge &edge) {SimpleContour::AddAnEdge(edge);}
    //bool AddPoint(const XY &xy) {return AddAnEdge(Edge(xy, at(0).GetStart()));}
    const ContourWithHoles &operator[](size_t n) const {return n==0 ? first : further[n-1];}
    bool IsSane() const;
    bool operator < (const Contour &p) const {return first==p.first ? further < p.further: first < p.first;}
    bool operator ==(const Contour &p) const {return first==p.first && further == p.further;}
    /** @} */ //Basic information

    /** @name Basic Operations
     * @{ */
    void swap(Contour &a) {first.swap(a.first); further.swap(a.further); std::swap(boundingBox, a.boundingBox);}
    void clear() {first.clear(); further.clear(); boundingBox.MakeInvalid();}
    Contour &ClearHoles() {first.holes.clear(); for (auto i = further.begin(); i!=further.end(); i++) i->holes.clear(); return *this;}
    /** @} */ //Basic Operations

    /** @name Transformations and expansion
     * A set of linear tranformations and two expansion operation.
     * Members starting with 'Create...' create a new copy of the contour. The others modify the contour itself.
     * @{ */
    /** Translate the contour */
    Contour &Shift(const XY &xy) {first.Shift(xy); if (further.size()) further.Shift(xy); boundingBox.Shift(xy); return *this;} 
    /** Scale the contour */
    Contour &Scale(double sc) { first.Scale(sc); if (further.size()) further.Scale(sc); boundingBox.Scale(sc); return *this; }
    /** Scale the contour */
    Contour &Scale(const XY &sc) { first.Scale(sc); if (further.size()) further.Scale(sc); boundingBox.Scale(sc); return *this; }
    /** Transpose the contour by swapping x and y coordinate. The contour, however, remains clockwise. */
    Contour &SwapXY() {first.SwapXY(); if (further.size()) further.SwapXY(); boundingBox.SwapXY(); return *this;}
    /** Rotate the contour around the origin by `degrees` degrees. */
    Contour &Rotate(double degrees) {if (degrees) {double r=deg2rad(degrees); Rotate(cos(r), sin(r));} return *this;}
    /** Rotate the contour around the `c` by `degrees` degrees. */
    Contour &RotateAround(const XY&c, double degrees) {if (degrees) {double r=deg2rad(degrees); RotateAround(c, cos(r), sin(r));} return *this;}

    /** Create a translated version of the contour. */
    Contour CreateShifted(const XY & xy) const {Contour a(*this); a.Shift(xy); return a;}
    /** Create a scaled version of the contour */
    Contour CreateScaled(double sc) const { Contour a(*this); a.Scale(sc); return a; }
    /** Create a scaled version of the contour */
    Contour CreateScaled(const XY &sc) const { Contour a(*this); a.Scale(sc); return a; }
    /** Create a clockwise, transposed version of the contour by swapping x and y coordinates. */
    Contour CreateSwapXYd() const {Contour a(*this); a.SwapXY(); return a;}
    /** Create a rotated version of the contour */
    Contour CreateRotated(double degrees) const {Contour a(*this); a.Rotate(degrees); return a;}
    /** Create a version of the contour rotated around `c`. */
    Contour CreateRotatedAround(const XY&c, double degrees) const {Contour a(*this); a.RotateAround(c, degrees); return a;}

    /** Expands a Contour.
     *
     * Expansion aims to create a shape whose edges run parallel along the edges of the 
     * original, but at `gap` pixels away. Positive `gap` values result in actual expansion
     * negative values in shrinkage (the new shape is inside the original). 
     * 
     * After the expansion, we check if there are edges crossing each other and 
     * remove them by untangling the result. In this step we also may end up in multiple separate 
     * disjoint shapes. (E.g., shrinking a dog-bone shape may lead us to two circles, 
     * like `O=O` becomes `o o`.) 
     *
     * There are multiple typical ways to handle edge joins, see EExpandType. With this function
     * we can specify the join type separately for positive contour outlines and hole outlines.
     * It is also possible to specify respective miter limits. These tells us that with 
     * EXPAND_MITER_* join types how long the miter edges can be in terms of the `gap`. 
     * E.g., miter_limit=2 says they can be as long as `2*gap`. The rest is cut away with a bevel-like edge.
     *
     * @param [in] gap The amount of expansion/shrinkage. No-op if zero.
     * @param [in] et4pos The edge join type for shape outlines.
     * @param [in] et4neg The edge join type for hole outlines.
     * @param [in] miter_limit_positive The miter limit for edge joins of shape outlines.
     * @param [in] miter_limit_negative The miter limit for edge joins of hole outlines.
     * @returns A reference to `*this`. */
    Contour& Expand(double gap, EExpandType et4pos=EXPAND_MITER_ROUND, EExpandType et4neg=EXPAND_MITER_ROUND,
                    double miter_limit_positive=CONTOUR_INFINITY, double miter_limit_negative=CONTOUR_INFINITY);
    /** Creates an expanded version of the contour; for parameters, see Expand. */
    Contour CreateExpand(double gap, EExpandType et4pos=EXPAND_MITER_ROUND, EExpandType et4neg=EXPAND_MITER_ROUND,
                         double miter_limit_positive=CONTOUR_INFINITY, double miter_limit_negative=CONTOUR_INFINITY) const;
    /** Perform 2D expansion.
     * 
     * For positive x and y `gap` values this function expands the shape such that placing a rectangle
     * 2*x wide and 2*y tall centered anywhere on or outside the expanded shape, the rectangle will not
     * overlap with the original shape.
     * This operation can be visualized as follows. Take an x*y rectangle touch the shape with it. Use
     * the rectangle as a brush and shift it along the contours of the shape while painting with it.
     * Outline of the the resulting, painted shape will be the result of the Expand2D operation.
     * @param [in] gap The x and y size of the rectangle used.
     * @returns A reference to `*this`.*/
    Contour& Expand2D(const XY &gap);
    /** Creates a 2D expanded version of the contour; for more details, see Expand2D. */
    Contour CreateExpand2D(const XY &gap) const;
    /** @} */ //Transformations

    /** @name Combination operations
     * A set of operations combining two contours.
     * Both copy and move versions available. Using the move version may result in less memory allocation.
     * For example, at taking the union of two disjoint shapes the move version need not create a separate copy, 
     * but can simply append one of the shapes to the other.
     * The result of the operation is stored in `this` as is usually the case with `+=`, `*=`, etc operators.
     * Also, the returned reference is always to `*this`.
     * Note that separate `+`, `*`, `-` and `^` operators are defined outside class contour.
     * @{ */
    /** Take the union with `a`.*/
    Contour &operator += (const Contour &a) {Operation(GetClockWise() || a.GetClockWise() ? POSITIVE_UNION : NEGATIVE_UNION, std::move(*this), a); return *this;}
    /** Take the union with `a`.*/
    Contour &operator += (Contour &&a)      {Operation(GetClockWise() || a.GetClockWise() ? POSITIVE_UNION : NEGATIVE_UNION, std::move(*this), std::move(a)); return *this;}
    /** Take the intersection with `a`.*/
    Contour &operator *= (const Contour &a) {Operation(GetClockWise() || a.GetClockWise() ? POSITIVE_INTERSECT : NEGATIVE_INTERSECT, std::move(*this), a); return *this;}
    /** Take the intersection with `a`.*/
    Contour &operator *= (Contour &&a)      {Operation(GetClockWise() || a.GetClockWise() ? POSITIVE_INTERSECT : NEGATIVE_INTERSECT, std::move(*this), std::move(a)); return *this;}
    /** Substract `a` from us, that is keep only areas of us not covered by `a`.*/
    Contour &operator -= (const Contour &a) { Operation(GetClockWise() || a.GetClockWise() ? POSITIVE_UNION : NEGATIVE_UNION, std::move(*this), a.create_inverse_dont_check()); return *this; }
    /** Substract `a` from us, that is keep only areas of us not covered by `a`.*/
    Contour &operator -= (Contour &&a)      {a.invert_dont_check(); Operation(GetClockWise() || !a.GetClockWise() ? POSITIVE_UNION : NEGATIVE_UNION, std::move(*this), std::move(a)); return *this;}
    /** Take `a` xor us, that is keep areas covered by strictly one of `a` or us.*/
    Contour &operator ^= (const Contour &a) {Operation(GetClockWise() || a.GetClockWise() ? POSITIVE_XOR : NEGATIVE_XOR, std::move(*this), a); return *this;}
    /** Take `a` xor us, that is keep areas covered by strictly one of `a` or us.*/
    Contour &operator ^= (Contour &&a)      {Operation(GetClockWise() || a.GetClockWise() ? POSITIVE_XOR : NEGATIVE_XOR, std::move(*this), std::move(a)); return *this;}
    /** @}*/ // Combination operations

    /** @name Graphics related operations
     * @{ */
    /** Establish the shape as a path(s) to a cairo context. 
     *
     * This operation adds a number of closed paths to the cairo context. Holes are added counterclockwise
     * resulting in holes if you use cairo_fill() on the path.
     * If any of the closed contours has non-visible edges and `show_hidden` is false, those edges are 
     * omitted abd the path will contain open sub-paths.
     * @param [in] cr The cairo context.
     * @param [in] show_hidden If false, we skip edges marked not visible.*/
    void CairoPath(cairo_t *cr, bool show_hidden) const { first.CairoPath(cr, show_hidden); if (further.size()) further.CairoPath(cr, show_hidden); }
    /** Establish parts of the shape as path(s) to a cairo context. 
     *
     * This operation adds a number of closed paths to the cairo context, either the clockwise
     * outlines or the counterclockwise hole outlines (but not both).
     * If any of the closed contours has non-visible edges and `show_hidden` is false, those edges are 
     * omitted abd the path will contain open sub-paths.
     * @param [in] cr The cairo context.
     * @param [in] show_hidden If false, we skip edges marked not visible.
     * @param [in] clockwiseonly If true only the clockwise oultines are added, if false, only the hole outlines (if any).*/
    void CairoPath(cairo_t *cr, bool show_hidden, bool clockwiseonly) const { first.CairoPath(cr, show_hidden, clockwiseonly); if (further.size()) further.CairoPath(cr, show_hidden, clockwiseonly); }
    /** Establish the shape as a path(s) to a cairo context using a series of sub-paths forming a dashed line. 
     *
     * This operation is useful to emulate dashed lines for backends having no such support (Windows Metafiles).
     * This operation adds a number of open sub-paths, one for each dash segment.
     * If any of the closed contours has non-visible edges and `show_hidden` is false, those edges are 
     * omitted.
     * @param [in] cr The cairo context.
     * @param [in] pattern Contains lengths of alternating on/off segments. Even indices contain the length of 
                           on segments, odd indices the length of off segments.
     * @param [in] num contains the number of elements in `pattern`.
     * @param [in] show_hidden If false, we skip edges marked not visible. */
    void CairoPathDashed(cairo_t *cr, const double pattern[], unsigned num, bool show_hidden) const { first.CairoPathDashed(cr, pattern, num, show_hidden); if (further.size()) further.CairoPathDashed(cr, pattern, num, show_hidden); }
    /** Establish the shape as a path(s) to a cairo context using a series of sub-paths forming a dashed line. 
     *
     * This operation is useful to emulate dashed lines for backends having no such support (Windows Metafiles).
     * This operation adds a number of open sub-paths, one for each dash segment.
     * Either the clockwise outlines or the counterclockwise hole outlines are added (but not both).
     * If any of the closed contours has non-visible edges and `show_hidden` is false, those edges are 
     * omitted.
     * @param [in] cr The cairo context.
     * @param [in] pattern Contains lengths of alternating on/off segments. Even indices contain the length of 
                           on segments, odd indices the length of off segments.
     * @param [in] num contains the number of elements in `pattern`.
     * @param [in] show_hidden If false, we skip edges marked not visible. 
     * @param [in] clockwiseonly If true only the clockwise oultines are added, if false, only the hole outlines (if any).*/
    void CairoPathDashed(cairo_t *cr, const double pattern[], unsigned num, bool show_hidden, bool clockwiseonly) const { first.CairoPathDashed(cr, pattern, num, show_hidden, clockwiseonly); if (further.size()) further.CairoPathDashed(cr, pattern, num, show_hidden, clockwiseonly); }
    /** Strokes the shape on a cairo context. Line with, color, dash, operation, transformations and mask are taken from `cr`.*/
    void Line(cairo_t *cr) const { Contour::CairoPath(cr, false); cairo_stroke(cr); }
    /** Strokes the shape on a cairo context, stroking holes with a dashed line.*/
    void Line2(cairo_t *cr) const {cairo_save(cr); double dash[]={2,2}; cairo_set_dash(cr, dash, 2, 0); CairoPath(cr, true); cairo_stroke(cr); cairo_set_dash(cr, NULL, 0, 0); CairoPath(cr, false); cairo_stroke(cr); cairo_restore(cr);}
    /** Fills the shape on a cairo context with the source set in the context.*/
    void Fill(cairo_t *cr) const { Contour::CairoPath(cr, true); cairo_fill(cr); }
    /** @} */ //name Graphics related operations
    
    /** @name Advanced information functions
     * These functions keep the contour intact, merely provide information about it.
     * @{ */
    /** Returns the relation of a point and the shape. */
    EPointRelationType IsWithin(const XY &p) const {EPointRelationType ret = first.IsWithin(p); if (ret==WI_OUTSIDE) ret = further.IsWithin(p); return ret;}
    /** Returns the touchpoint of tangents drawn from a point to the shape.
     * 
     * @param [in] p The point to draw the tangents from.
     * @param [out] t1 The point where the first tangent touches the shape. 
     * @param [out] t2 The point where the second tangent touches the shape. 
     * @returns The relation of the point to the shape. Meaningful tangents can only be drawn if this is WI_OUTSIDE.*/
    EPointRelationType Tangents(const XY &p, XY &t1, XY &t2) const {EPointRelationType ret = first.Tangents(p, t1, t2); if (ret==WI_OUTSIDE) ret = further.Tangents(p, t1, t2); return ret;}
    /** Returns the relation of two shapes.
     *
     * If `ignore holes` is true we consider only the outline of the shapes. In this case only
     * A_IS_EMPTY, B_IS_EMPTY, BOTH_EMPTY, A_INSIDE_B, B_INSIDE_A, SAME, APART, OVERLAP can be returned.
     * If false, these additional values may come:
     * A_IN_HOLE_OF_B, B_IN_HOLE_OF_A, IN_HOLE_APART.
     * (Latter meaning no overlap, but some parts of A is in holes and outside of B.) */
    EContourRelationType RelationTo(const Contour &c, bool ignore_holes) const;
    /** Returns if a relation means overlap */
    bool Overlaps(EContourRelationType t) const {return result_overlap(t);}
    /** Returns true if the two contours overlap (surface area of intersection is nonzero) */
    bool Overlaps(const Contour &c) const {return Overlaps(RelationTo(c, true));}
    /** Not implemented on purpose. Declaration here to prevent unintended use (similar function of Block exist). */
    bool Overlaps(const Contour &c, double gap) const; 
    double OffsetBelow(const SimpleContour &below, double &touchpoint, double offset=CONTOUR_INFINITY) const;
    double OffsetBelow(const Contour &below, double &touchpoint, double offset=CONTOUR_INFINITY) const;
    /** Returns distance of and two points on the two contours.
     *
     * May return invalid distance (infinity) if one or the other contour is empty.
     * Negative distance is returned if one is inside another (but positive if in a hole).
     * Zero returned if they cross. If the two points are equal that is a crosspoint, if not they are to be ignored.
     * (Latter may happen if one of the contours have two pieces and one is inside and one is outside the other.) */
    DistanceType Distance(const Contour &c) const {DistanceType r; Distance(c, r); return r;}
    /** Calculates the distance between a point and us by finding our closest point.
     *
     * @param [in] o The point to take the distance from.
     * @param [out] ret We return the point on our contour closes to `o`.
     * @return The distance, negative if `o` is inside us. `CONTOUR_INFINITY` if we are empty. */
    double Distance(const XY &o, XY &ret) const {XY tmp; double d = first.Distance(o, ret), dd=further.Distance(o, tmp); if (fabs(dd)<fabs(d)) {ret=tmp; d=dd;} return d;}
    /** Calculates the distance between a point and us by finding our closest point and returns two tangent points.
     *
     * Same as Distance(const XY &o, XY &ret), but in addition we return two tangent points 
     * from the tangent of the shape at `ret`. See @ref contour for a description of tangents.
     *
     * @param [in] o The point to take the distance from.
     * @param [out] ret We return the point on our contour closes to `o`.
     * @param [out] t1 The forward tangent point.
     * @param [out] t2 The backward tangent point.
     * @return The distance, negative if `o` is inside us. `CONTOUR_INFINITY` if we are empty. */
    double DistanceWithTangents(const XY &o, XY &ret, XY &t1, XY &t2) const {XY tmp, _1, _2; double d = first.DistanceWithTangents(o, ret, t1, t2), dd=further.DistanceWithTangents(o, tmp, _1, _2); if (fabs(dd)<fabs(d)) {ret=tmp; t1=_1; t2=_2; d=dd;} return d;}
    /** Returns a cut of the shape along an (infinite) line.
     *
     * Takes all crosspoints of the infinite line defined by `A` and `B` and the shape
     * and returns the two outermost. We return `pos` values corresponding to the 
     * finite section `A`->`B`,
     * thus both values are between 0 and 1. It is possible that a single value is
     * returned if the line touches the shape.
     * If the line does not cross or touch the shape, an invalid
     * range is returned. 
     * @param [in] A One point on the line.
     * @param [in] B Another point on the line.
     * @return The `pos` value corresponding to the two outmost crosspoints. */
    Range Cut(const XY &A, const XY &B) const;
    /** Returns a cut of the shape along an edge.
     *
     * Takes all crosspoints of a (finite length, possibly curvy) edge and the shape
     * and returns the two outermost. We return `pos` values corresponding to the edge,
     * thus both values are between 0 and 1. It is possible that a single value is
     * returned if the edge touches the shape or if its start is inside the shape, but
     * its end is outside. If the edge does not cross or touch the shape, an invalid
     * range is returned. */
    Range Cut(const Edge &e) const {Range ret = first.Cut(e); if (!further.IsEmpty()) ret += further.Cut(e); return ret;}
    /** Creates a cross-section (or cut) of the Contour along a straight line.
     *
     * We return all crosspoints of an infinite line specified by `A` and `B`.
     * We return them in `pos` terms, that is, at `A` we return 0, at `B` we return 1 and
     * linearly intrapolate and extrapolate between them and outside, respectively.
     *
     * @param [in] A One point of the infinite line.
     * @param [in] B Another point of the infinite line.
     * @param [out] map A DoubleMap containing true for points inside the shape, false for outside. */
    void Cut(const XY &A, const XY &B, DoubleMap<bool> &map) const {first.Cut(A, B, map); if (!further.IsEmpty()) further.Cut(A, B, map); map.Prune();}
    /** Returns a cut of the shape along an (infinite) line.
     *
     * Same as Cut(const XY &A, const XY &B), but also returns the coordinates of the positions
     * returned and a forward tangent point for them. In case the line cuts the shape
     * at a non-smooth vertex, then we take the tangent line of both edges of the vertex
     * average them and return a point from this middle line in the forward direction.
     * 
     * @param [in] A One point on the line.
     * @param [in] B Another point on the line.
     * @param [out] from Values correspond to the position returned in `from`. `first` is the coordinate of the point, `second` is the forward tangent point.
     * @param [out] till Values correspond to the position returned in `till`. `first` is the coordinate of the point, `second` is the forward tangent point.
     * @return The `pos` value corresponding to the two outmost crosspoints. */
    Range CutWithTangent(const XY &A, const XY &B, std::pair<XY, XY> &from, std::pair<XY, XY> &till) const;
    /** Returns a cut of the shape along an edge.
     *
     * Same as Cut(const Edge &s), but also returns the coordinates of the positions
     * returned and a forward tangent point for them. In case the edge cuts the shape
     * at a non-smooth vertex, then we take the tangent line of both edges of the vertex
     * average them and return a point from this middle line in the forward direction.
     * 
     * @param [in] e The edge to cut with.
     * @param [out] from Values correspond to the position returned in `from`. `first` is the coordinate of the point, `second` is the forward tangent point.
     * @param [out] till Values correspond to the position returned in `till`. `first` is the coordinate of the point, `second` is the forward tangent point.
     * @return The `pos` value corresponding to the two outmost crosspoints. */
    Range CutWithTangent(const Edge &e, std::pair<XY, XY> &from, std::pair<XY, XY> &till) const {Range ret = first.CutWithTangent(e, from, till); if (!further.IsEmpty()) ret += further.CutWithTangent(e,from, till); return ret;}
    /** Calculates the touchpoint of tangents drawn from a given point.
     * 
     * Given the point `from` draw tangents to the shape (two can be drawn)
     * and calculate where these tangents touch the shape.
     * Tangent is anything touching the shape (at a vertex, at the middle of a curvy edge
     * or going along, in parallel with a full straight edge).
     * In this context the *clockwise tangent* is the one which is traversed from 
     * `from` towards the shape touches the shape in the clockwise direction.
     * @param [in] from The point from which the tangents are drawn.
     * @param [out] clockwise The point where the clockwise tangent touches the shape.
     * @param [out] cclockwise The point where the counterclockwise tangent touches the shape.
     * @returns True if success, false if `from` is inside or on the shape. */
    bool TangentFrom(const XY &from, XY &clockwise, XY &cclockwise) const;
    /** Calculates the touchpoint of tangents drawn to touch two shapes.
     * 
     * Given the two shapes, four such tangents can be drawn, here we focus on the two 
     * outer ones, the ones that touch either both shapes clockwise or both of them
     * counterclockwise, but not mixed.
     * Tangent is anything touching the shape (at a vertex, at the middle of a curvy edge
     * or going along, in parallel with a full straight edge).
     * @param [in] from The other shape to the tangents are drawn.
     * @param [out] clockwise The points where the clockwise tangent touches our shape
     *                        (clockwise[0]) and `from` (clockwise[1]).
     * @param [out] cclockwise The points where the counterclockwise tangent touches our shape
     *                         (cclockwise[0]) and `from` (cclockwise[1]).
     * @returns True if success, false if `from` is inside us. */
    bool TangentFrom(const Contour &from, XY clockwise[2], XY cclockwise[2]) const;
    /** Creates a cross-section of the Contour along a vertical line.
     *
     * @param [in] x The x coordinate of the vertical line
     * @param section A DoubleMap containing true for points inside the shape, false for outside. May contain entries already, when calling this function. */
    void VerticalCrossSection(double x, DoubleMap<bool> &section) const {first.VerticalCrossSection(x, section); if (further.size()) further.VerticalCrossSection(x, section);}
    /** @} */ //Advanced information functions
};

inline Contour operator + (const Contour &a, const Contour &b) {return Contour(a.GetClockWise() || b.GetClockWise() ? Contour::POSITIVE_UNION : Contour::NEGATIVE_UNION,         a, b);}
inline Contour operator * (const Contour &a, const Contour &b) {return Contour(a.GetClockWise() || b.GetClockWise() ? Contour::POSITIVE_INTERSECT : Contour::NEGATIVE_INTERSECT, a, b);}
inline Contour operator - (const Contour &a, const Contour &b) {return Contour(a.GetClockWise() || b.GetClockWise() ? Contour::POSITIVE_UNION : Contour::NEGATIVE_UNION,         a, b.create_inverse_dont_check());}
inline Contour operator ^ (const Contour &a, const Contour &b) {return Contour(a.GetClockWise() || b.GetClockWise() ? Contour::POSITIVE_XOR : Contour::NEGATIVE_XOR,             a, b);}
inline Contour operator + (const Contour &a, Contour &&b)      {return Contour(a.GetClockWise() || b.GetClockWise() ? Contour::POSITIVE_UNION : Contour::NEGATIVE_UNION,         a, std::move(b));}
inline Contour operator * (const Contour &a, Contour &&b)      {return Contour(a.GetClockWise() || b.GetClockWise() ? Contour::POSITIVE_INTERSECT : Contour::NEGATIVE_INTERSECT, a, std::move(b));}
inline Contour operator - (const Contour &a, Contour &&b)      {return Contour(a.GetClockWise() || b.GetClockWise() ? Contour::POSITIVE_UNION : Contour::NEGATIVE_UNION,         a, std::move(b.invert_dont_check()));}
inline Contour operator ^ (const Contour &a, Contour &&b)      {return Contour(a.GetClockWise() || b.GetClockWise() ? Contour::POSITIVE_XOR : Contour::NEGATIVE_XOR,             a, std::move(b));}
inline Contour operator + (Contour &&a, const Contour &b)      {return Contour(a.GetClockWise() || b.GetClockWise() ? Contour::POSITIVE_UNION : Contour::NEGATIVE_UNION,         std::move(a), b);}
inline Contour operator * (Contour &&a, const Contour &b)      {return Contour(a.GetClockWise() || b.GetClockWise() ? Contour::POSITIVE_INTERSECT : Contour::NEGATIVE_INTERSECT, std::move(a), b);}
inline Contour operator - (Contour &&a, const Contour &b)      {return Contour(a.GetClockWise() || b.GetClockWise() ? Contour::POSITIVE_UNION : Contour::NEGATIVE_UNION,         std::move(a), Contour(b).invert_dont_check());}
inline Contour operator ^ (Contour &&a, const Contour &b)      {return Contour(a.GetClockWise() || b.GetClockWise() ? Contour::POSITIVE_XOR : Contour::NEGATIVE_XOR,             std::move(a), b);}
inline Contour operator + (Contour &&a, Contour &&b)           {return Contour(a.GetClockWise() || b.GetClockWise() ? Contour::POSITIVE_UNION : Contour::NEGATIVE_UNION,         std::move(a), std::move(b));}
inline Contour operator * (Contour &&a, Contour &&b)           {return Contour(a.GetClockWise() || b.GetClockWise() ? Contour::POSITIVE_INTERSECT : Contour::NEGATIVE_INTERSECT, std::move(a), std::move(b));}
inline Contour operator - (Contour &&a, Contour &&b)           {return Contour(a.GetClockWise() || b.GetClockWise() ? Contour::POSITIVE_UNION : Contour::NEGATIVE_UNION,         std::move(a), std::move(b.invert_dont_check()));}
inline Contour operator ^ (Contour &&a, Contour &&b)           {return Contour(a.GetClockWise() || b.GetClockWise() ? Contour::POSITIVE_XOR : Contour::NEGATIVE_XOR,             std::move(a), std::move(b));}



template <typename LT> void Distance(const LT &list, const Contour &c, DistanceType &dist_so_far)
{
    if (list.size()==0 || c.IsEmpty()) return;
    for (auto i = list.begin(); i!=list.end(); i++) {
        const double bbdist = (*i)->GetBoundingBox().Distance(c.GetBoundingBox());
        if (dist_so_far.ConsiderBB(bbdist))
            (*i)->Distance(c, dist_so_far);
        else
            dist_so_far.MergeInOut(bbdist);
        if (dist_so_far.IsZero())
            break;
    }
}



inline bool ContourList::GetClockWise() const
{
    return size() ? begin()->GetClockWise() : true;
}

inline double ContourList::GetArea(bool consider_holes) const
{
    register double ret = 0;
    for (auto i = begin(); i!=end(); i++)
        ret += i->GetArea(consider_holes);
    return ret;
}

inline double ContourList::GetCircumference(bool consider_holes, bool include_hidden) const
{
    register double ret = 0;
    for (auto i = begin(); i!=end(); i++)
        ret += i->GetCircumference(consider_holes, include_hidden);
    return ret;
}

inline XY ContourList::CentroidUpscaled() const
{
    XY ret(0,0);
    for (auto i = begin(); i!=end(); i++)
        ret += i->CentroidUpscaled();
    return ret;
}


inline void ContourList::append(const ContourWithHoles &p)
{
    if (p.IsEmpty()) return;
    //_ASSERT(size()==0 || GetClockWise() == p.GetClockWise()); These trigger when a Path is converted
    boundingBox += p.GetBoundingBox();
    std::list<ContourWithHoles>::push_back(p);
}

inline void ContourList::append(ContourWithHoles &&p)
{
    if (p.IsEmpty()) return;
    //_ASSERT(size()==0 || GetClockWise() == p.GetClockWise()); These trigger when a Path is converted
    boundingBox += p.GetBoundingBox();
    std::list<ContourWithHoles>::push_back(std::move(p));
}


inline const ContourWithHoles & ContourList::operator[](size_type i) const
{
    const_iterator itr = begin();
    while (i>0) i--, itr++;
    return *itr;
}


inline EPointRelationType ContourList::IsWithin(const XY &p) const
{
    EPointRelationType ret = boundingBox.IsWithin(p);
    if (ret==WI_OUTSIDE) return WI_OUTSIDE;
    for (auto i = begin(); i!=end(); i++)
        if (WI_OUTSIDE != (ret=i->IsWithin(p))) return ret; //we also return IN_HOLE
    return WI_OUTSIDE;
}

inline EPointRelationType ContourList::Tangents(const XY &p, XY &t1, XY &t2) const
{
    EPointRelationType ret = boundingBox.IsWithin(p);
    if (ret==WI_OUTSIDE) return WI_OUTSIDE;
    for (auto i = begin(); i!=end(); i++)
        if (WI_OUTSIDE != (ret=i->Tangents(p, t1, t2))) return ret; //we also return IN_HOLE
    return WI_OUTSIDE;
}


inline void ContourList::VerticalCrossSection(double x, DoubleMap<bool> &section) const
{
    for (auto i=begin(); i!=end(); i++)
        i->VerticalCrossSection(x, section);
}


inline void ContourList::Shift(const XY &xy)
{
    for (auto i = begin(); i!=end(); i++)
        i->Shift(xy);
    boundingBox.Shift(xy);
}

inline void ContourList::Scale(double sc)
{
    for (auto i = begin(); i!=end(); i++)
        i->Scale(sc);
    boundingBox.Scale(sc);
}

inline void ContourList::Scale(const XY &sc)
{
    for (auto i = begin(); i!=end(); i++)
        i->Scale(sc);
    boundingBox.Scale(sc);
}


inline void ContourList::Rotate(double cos, double sin)
{
    boundingBox.MakeInvalid();
    for (auto i = begin(); i!=end(); i++) {
        i->Rotate(cos, sin);
        boundingBox += i->GetBoundingBox();
    }
}

inline void ContourList::RotateAround(const XY&c, double cos, double sin)
{
    boundingBox.MakeInvalid();
    for (auto i = begin(); i!=end(); i++) {
        i->RotateAround(c, cos, sin);
        boundingBox += i->GetBoundingBox();
    }
}

/** Determine the relative vertical distance between our shapes and another one.
 *
 * For detailed explanation and parameters, see 
 * Contour::OffsetBelow(const Contour &below, double &touchpoint, double offset).
 */
inline double ContourList::OffsetBelow(const Contour &below, double &touchpoint, double offset) const
{
    if (below.further.size())
        offset = OffsetBelow(below.further, touchpoint, offset);
    return OffsetBelow(below.first.outline, touchpoint, offset);
}

inline double ContourWithHoles::Distance(const XY &o, XY &ret) const
{
    double d = outline.Distance(o, ret);
    if (d>=0) return d;
    XY tmp;
    double dd = -holes.Distance(o, tmp);
    if (fabs(dd)<fabs(d)) {
        ret = tmp;
        return dd;
    }
    return d;
}


inline double ContourWithHoles::DistanceWithTangents(const XY &o, XY &ret, XY &t1, XY &t2) const
{
    double d = outline.DistanceWithTangents(o, ret, t1, t2);
    if (d>=0) return d;
    XY tmp, _1, _2;
    double dd = -holes.DistanceWithTangents(o, tmp, _1, _2);
    if (fabs(dd)<fabs(d)) {
        ret = tmp;
        t1 = _1;
        t2 = _2;
        return dd;
    }
    return d;
}


inline Range ContourList::Cut(const Edge &e) const
{
    Range ret;
    ret.MakeInvalid();
    for (auto i = begin(); i!=end(); i++)
        ret += i->Cut(e);
    return ret;
}

inline Range ContourList::CutWithTangent(const Edge &e, std::pair<XY, XY> &from, std::pair<XY, XY> &till) const
{
    Range ret;
    ret.MakeInvalid();
    std::pair<XY, XY> f, t;
    for (auto i = begin(); i!=end(); i++) {
        Range r = i->CutWithTangent(e, f, t);
        if (ret.from > r.from) {
            ret.from = r.from;
            from = f;
        }
        if (ret.till < r.till) {
            ret.till = r.till;
            till = t;
        }
    }
    return ret;
}

inline void ContourList::Cut(const XY &A, const XY &B, DoubleMap<bool> &map) const
{
    for (auto i = begin(); i!=end(); i++)
        i->Cut(A, B, map);
}


//returns true only if all is true
inline bool ContourList::TangentFrom(const XY &from, XY &clockwise, XY &cclockwise) const
{
    if (size()==0) return false;
    if (!begin()->outline.TangentFrom(from, clockwise, cclockwise))
        return false;
    XY c, cc;
    for (auto i=++begin(); i!=end(); i++) {
        if (!i->outline.TangentFrom(from, c, cc)) return false;
        clockwise =  minmax_clockwise(from, clockwise,  c,  true);
        cclockwise = minmax_clockwise(from, cclockwise, cc, false);
    }
    return true;
}
///////Contour///////////////////////////////////////

inline EPointRelationType ContourWithHoles::IsWithin(const XY &p) const
{
    EPointRelationType ret = outline.IsWithin(p);
    if (ret!=WI_INSIDE || holes.size()==0) return ret;
    ret = holes.IsWithin(p);
    switch (ret) {
    case WI_INSIDE: return WI_IN_HOLE;
    case WI_IN_HOLE:
    case WI_OUTSIDE: return WI_INSIDE;
    default: return ret;
    }
}

inline EPointRelationType ContourWithHoles::Tangents(const XY &p, XY &t1, XY &t2) const
{
    size_t edge;
    double pos;
    EPointRelationType ret = outline.IsWithin(p, &edge, &pos);
    if (ret==WI_ON_EDGE || ret==WI_ON_VERTEX) {
        t1 = outline.PrevTangentPoint(edge, pos);
        t2 = outline.NextTangentPoint(edge, pos);
        return ret;
    }
    if (ret!=WI_INSIDE || holes.size()==0) return ret;
    ret = holes.Tangents(p, t1, t2);
    switch (ret) {
    case WI_INSIDE: return WI_IN_HOLE;
    case WI_IN_HOLE:
    case WI_OUTSIDE: return WI_INSIDE;
    default: return ret;
    }
}

/** Determine the relative vertical distance between two shapes.
 *
 * For detailed explanation and parameters, see 
 * Contour::OffsetBelow(const Contour &below, double &touchpoint, double offset).
 */
inline double Contour::OffsetBelow(const SimpleContour &below, double &touchpoint, double offset) const
{
    if (further.size())
        offset = further.OffsetBelow(below, touchpoint, offset);
    return first.outline.OffsetBelow(below, touchpoint, offset);
}

/** Determine the relative vertical distance between two shapes. 
 *
 * Assuming we are higher than `below` (lower y coordinates), how much can `below`
 * be shifted upwards before it collides with us (called *offset*).
 * If `below` and us do not share any poins with the same x coordinate, the offset is `CONTOUR_INFINITY`
 * (`o` can pass infinitely up besides us without collision).
 * If `o` and us share such points, but `o` is above us, the offset is negative, since
 * `o` needs to be pulled down to be below us. This is not viewed as an error case but as normal operation.
 *
 * This function is constructed to allow merging of the result of multiple OffsetBelow calls. Assume two 
 * Contours are made of multiple SimpleContours. In this case we call this function multiple times, pairwise
 * for constitutent SimpleContours. The smallest of the returned offsets (and the touchpoint associated) 
 * will be the ultimate offset between the two Contours. Therefore we can supply a previous `offset` value
 * and this function will return the offset between the two SimpleContours only if it is smaller than the
 * `offset` parameter. Also, `touchpoint` is changed only in this case.
 *
 * @param [in] below The other shape below us.
 * @param [out] touchpoint After moving `below` up, we return the point on us where we touched. However,
 *                         if the offset determined for us and `below` as descibed above is not smaller than 
 *                         the `offset` parameter, we leave `touchpoint` intact.
 * @param [in] offset The offset calculated from some previous operation. Set to `CONTOUR_INFINITY` if no such op.
 * @return The smaller of `offset` and how much `below` was moved up before it hit us. 
 */
inline double Contour::OffsetBelow(const Contour &below, double &touchpoint, double offset) const
{
    if (further.size())
        offset = further.OffsetBelow(below, touchpoint, offset);
    if (below.further.size())
        for (auto i = below.further.begin(); i!=below.further.end(); i++)
            offset = OffsetBelow(*i, touchpoint, offset);
    return first.outline.OffsetBelow(below.first.outline, touchpoint, offset);
}

inline Contour &Contour::Expand(double gap, EExpandType et4pos, EExpandType et4neg,
                                double miter_limit_positive, double miter_limit_negative)
{
    if (!gap) return *this;
    Contour res;
    Expand(et4pos, et4neg, gap, res, miter_limit_positive, miter_limit_negative);
    swap(res);
    return *this;
}

inline Contour Contour::CreateExpand(double gap, EExpandType et4pos, EExpandType et4neg,
                                     double miter_limit_positive, double miter_limit_negative) const
{
    if (!gap) return *this;
    Contour res;
    Expand(et4pos, et4neg, gap, res, miter_limit_positive, miter_limit_negative);
    return res;
}

inline Contour& Contour::Expand2D(const XY &gap)
{
    if (test_zero(gap.x) && test_zero(gap.y)) return *this;
    Contour res;
    Expand2D(gap, res);
    swap(res);
}

inline Contour Contour::CreateExpand2D(const XY &gap) const
{
    if (test_zero(gap.x) && test_zero(gap.y)) return *this;
    Contour res;
    Expand2D(gap, res);
    return res;
}


inline void Contour::Distance(const Contour &c, DistanceType &dist_so_far) const
{
    if (IsEmpty() || c.IsEmpty() || dist_so_far.IsZero()) return;
    first.Distance(c.first, dist_so_far);
    if (dist_so_far.IsZero()) return;
    if (!c.further.IsEmpty()) {
        dist_so_far.SwapPoints();
        c.further.Distance(first, dist_so_far);
        if (dist_so_far.IsZero()) return;
        dist_so_far.SwapPoints();
    }
    if (!further.IsEmpty()) {
        further.Distance(c.first, dist_so_far);
        if (dist_so_far.IsZero()) return;
    }
    if (!c.further.IsEmpty() && !further.IsEmpty())
        further.Distance(c.further, dist_so_far);
}

inline bool Contour::TangentFrom(const XY &from, XY &clockwise, XY &cclockwise) const
{
    if (first.outline.TangentFrom(from, clockwise, cclockwise)) {
        XY c, cc;
        if (further.TangentFrom(from, c, cc)) {
            clockwise =  minmax_clockwise(from, clockwise,  c,  true);
            cclockwise = minmax_clockwise(from, cclockwise, cc, false);
        }
        return true;
    }
    return false;
}

#ifdef _DEBUG
extern int expand_debug;
extern std::map<size_t, XY> expand_debug_cps;
extern std::list<std::vector<Edge>> expand_debug_contour;
#endif



} //namespace


#endif //CONTOUR_CONTOURS_H
