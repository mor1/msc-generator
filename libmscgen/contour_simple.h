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

/** @file contour_simple.h Declares class SimpleContour and class DoubleMap.
 * @ingroup contour_files
 */

#if !defined(CONTOUR_CONTOUR_H)
#define CONTOUR_CONTOUR_H

#include <vector>
#include <list>
#include "contour_edge.h"


namespace contour {

/** A template that allows storing of state along a 1 dimensional axis.
 * @ingroup contour_internal
 *
 * The state can change at any real (double) value.
 * The template parameter contains the type of state.
 * We have constructs to set the state at a walue and onwards or between two points only.
 * We also have members to query the state at any point and ask where will the next change is.
 * Finally, if the element supports the += operator we can add changes to the state.
 */

template <class element>
class DoubleMap : public std::map<double, element>
{
public:
    using std::map<double, element>::operator[];   //indicate to two-stage (dependent) name lookup
    using std::map<double, element>::upper_bound;  //that we want these names (not dependent on "element")
    using std::map<double, element>::lower_bound;  //looked up only at instantiation time (from base class)
    using std::map<double, element>::begin;        //and not during template compilation (searched in global scope).
    using std::map<double, element>::end;
    using std::map<double, element>::insert;
    using std::map<double, element>::erase;

    DoubleMap() {};
    DoubleMap(const element &e) {insert(typename std::map<double, element>::value_type(-CONTOUR_INFINITY, e));
                                 insert(typename std::map<double, element>::value_type(CONTOUR_INFINITY, e));}
    void clear(const element &e) {std::map<double, element>::clear();
                                  insert(typename std::map<double, element>::value_type(-CONTOUR_INFINITY, e));
                                  insert(typename std::map<double, element>::value_type(CONTOUR_INFINITY, e));}
    void clear() {if (this->size()>2) erase(++begin(), --end());} ///<Clear all of the state
    void Set(double pos, const element&e) {operator[](pos) = e;}  ///<Set state to `e` from `pos` till any existing state change after pos.
    void Set(const Range &r, const element &e);                   ///<Set state to `e` in the range `r`.
    void Add(double pos, const element&e);                        ///<Add the element `e` to the state from `pos` till any existing state change after pos. Assumes element has operator +=
    void Add(const Range &r, const element&e);                    ///<Add state to `e` in the range `r`. Assumes element has operator +=
    template <typename T> void Add(double pos, const element&e, T &combine);     ///<Add the element `e` to the state from `pos` till any existing state change after pos. Uses `combine(a, b)` to combine elements
    template <typename T> void Add(const Range &r, const element&e, T &combine); ///<Add state to `e` in the range `r`. Uses `combine(a, b)` to combine elements
    const element* Get(double pos) const {auto i=upper_bound(pos); return i==begin()?NULL:&(--i)->second;} ///<Return state at `pos` if any.
    double Till(double pos) const {auto i=upper_bound(pos); return i==end()?CONTOUR_INFINITY:i->first;}    ///<Returns the location of any state change strictly before `pos`. Returns `-CONTOUR_INFINITY` if none.
    double From(double pos) const {auto i=--lower_bound(pos); return i==end()?-CONTOUR_INFINITY:i->first;} ///<Returns the location of any state change strictly after `pos`. Returns `CONTOUR_INFINITY` if none.
    void Prune();      ///<Kill redundant elements (no-op state changes) to reduce size.
};

template <class element>
void DoubleMap<element>::Add(double pos, const element&e)
{
    auto i = --upper_bound(pos);
    if (i==begin()) Set(pos, e);
    else if (i->first == pos)
        i->second += e;
    else insert(i, typename std::map<double, element>::value_type(pos, i->second))->second += e;
}

template <class element>
template <typename T>
void DoubleMap<element>::Add(double pos, const element&e, T &combine)
{
    auto i = --upper_bound(pos);
    if (i==begin()) Set(pos, e);
    else if (i->first == pos)
        i->second = combine(i->second, e);
    else insert(i, typename std::map<double, element>::value_type(pos, combine(i->second, e)));
}

template <class element>
void DoubleMap<element>::Set(const Range &r, const element& e)
{
    if (r.till <= r.from) return;
    auto i = --upper_bound(r.till);
    if (i!=end()) {//avoid the case when whole range is before first element
        if (i->first == r.till) {
            erase(upper_bound(r.from), i);
        } else {
            operator[](r.till) = i->second;
            erase(upper_bound(r.from), ++i);
        }
    }
    operator[](r.from) = e;
}

template <class element>
void DoubleMap<element>::Add(const Range &r, const element& e)
{
    if (r.till <= r.from) return;
    auto i = --upper_bound(r.till);
    if (i==end())
        operator[](r.from) = e; //if the whole range is before the first element
    else {
        if (i->first != r.till) //i points to a place before r.till
            i = insert(i, typename std::map<double, element>::value_type(r.till, i->second));
        //now i points to the element at r.till
        auto h = --upper_bound(r.from);
        if (h==end())
            h = insert(begin(), typename std::map<double, element>::value_type(r.from, e))++;
        else if (h->first != r.from) //j points to an element before r.from
            h = insert(h, typename std::map<double, element>::value_type(r.from, h->second));
        //now h points to the first element to add e to
        for (; h!=i; h++)
            h->second += e;
    }
}

template <class element>
template <typename T>
void DoubleMap<element>::Add(const Range &r, const element& e, T &combine)
{
    if (r.till <= r.from) return;
    auto i = --upper_bound(r.till);
    if (i==end())
        operator[](r.from) = e; //if the whole range is before the first element
    else {
        if (i->first != r.till) //i points to a place before r.till
            i = insert(i, typename std::map<double, element>::value_type(r.till, i->second));
        //now i points to the element at r.till
        auto h = --upper_bound(r.from);
        if (h==end())
            h = insert(begin(), typename std::map<double, element>::value_type(r.from, e))++;
        else if (h->first != r.from) //j points to an element before r.from
            h = insert(h, typename std::map<double, element>::value_type(r.from, h->second));
        //now h points to the first element to add e to
        for (; h!=i; h++)
            h->second = combine(h->second, e);
    }
}
template <class element>
void DoubleMap<element>::Prune()
{
    auto i = begin();
    while(i!=end()) {
        auto j = i;
        j++;
        if (j==end()) return;
        if (i->second == j->second)
            erase(j);
        else
            i=j;
    }
}


class Contour;

/** Contains a single, contigous 2D shape with no holes. Essentially a list of edges.
 * @ingroup contour_internal
 *
 * - Never degenerate - always has actual area
 * - Never flopped - edges never cross each-other (except joining at vertices)
 * - Always contagious - one single surface
 * - Always goes in clockwise direction - except internally, when we store holes.
 *
 * The constructors are all protected, since you are not supposed to instantiate 
 * a copy from this class. This is used by class Contour to store parts.
 */
class SimpleContour
{
    friend class Contour;
protected:
    std::vector<Edge> edges;   ///<An ordered list of (directed) edges.
private:
    Block  boundingBox;        ///<The bounding box for the shape.
    bool   clockwise;          ///<Tells if the shape is clockwise or counterclockwise
    mutable std::pair<bool, double> area_cache; ///<A cache: if `first` is true, `second` tells the surface area of the shape. Negative for counterclockwise shapes.

    EContourRelationType CheckContainmentHelper(const SimpleContour &b) const;
    double do_offsetbelow(const SimpleContour &below, double &touchpoint, double offset=CONTOUR_INFINITY) const;

    const Block &CalculateBoundingBox();

    friend class ContoursHelper; //For walk related memebers
    void CalculateClockwise();
    double CalcualteArea() const;
    void AppendDuringWalk(const Edge &);
    bool PostWalk();
    bool Sanitize();

    size_t     next(size_t vertex) const {return size()<=1 ? 0 : (vertex+1)%size();}         ///<Returns index of subsequent edge, warps around to 0 at last one.
    size_t     prev(size_t vertex) const {return size()<=1 ? 0 : (vertex-1+size())%size();}  ///<Returns index of preceeding edge, warps around at 0 to last edge.
    Edge       &at(size_t i)       {return edges[i];}          ///<Returns reference to edge at index `i`.
    const Edge &at(size_t i) const {return edges[i];}          ///<Returns const reference to edge at index `i`.
    Edge       &at_next(size_t i)       {return at(next(i));}  ///<Returns reference to edge after index `i`. Wraps around at last edge.
    const Edge &at_next(size_t i) const {return at(next(i));}  ///<Returns const reference to edge after index `i`. Wraps around at last edge.
    Edge       &at_prev(size_t i)       {return at(prev(i));}  ///<Returns reference to edge before index `i`. Wraps around at first edge.
    const Edge &at_prev(size_t i) const {return at(prev(i));}  ///<Returns const reference to edge before index `i`. Wraps around at first edge.

    /** Take tangent of `edge` at `pos` and return a point on the line of a tangent towards the start of curve/edge. */
    XY PrevTangentPoint(size_t edge, double pos) const
        {return (test_smaller(0, pos) ? at(edge) : at_prev(edge)).PrevTangentPoint(test_smaller(0, pos) ? pos : 1);}
    /** Take tangent of `edge` at `pos` and return a point on the line of a tangent towards the end of curve/edge. */
    XY NextTangentPoint(size_t edge, double pos) const
        {return (test_smaller(pos, 1) ? at(edge) : at_next(edge)).NextTangentPoint(test_smaller(pos, 1) ? pos : 0);}

    void Expand2DHelper(const XY &gap, std::vector<Edge> &a,
                        unsigned original_last, unsigned next, unsigned my_index,
                        int last_type, int stype) const;
protected:
    friend class ContourWithHoles;
    friend class ContourList;
    friend struct node;
    SimpleContour() : clockwise(true), area_cache(true, 0.) {boundingBox.MakeInvalid();}  ///<Create an empty shape with no edges.
    SimpleContour(SimpleContour &&p) {swap(p);}  ///<Move content of another shape. The other shape gets undefined.
    SimpleContour(const SimpleContour &p) : edges(p.edges), boundingBox(p.boundingBox), clockwise(p.clockwise), area_cache(p.area_cache) {} ///<Create a copy of another shape
    SimpleContour(double sx, double dx, double sy, double dy) {operator = (Block(sx,dx,sy,dy));} ///<Create a rectangle
    SimpleContour(const Block &b) {operator =(b);} ///<Create a rectangle
    SimpleContour(XY a, XY b, XY c); ///<Create an (eventually clockwise) triangle. Empty if degenerate.
    SimpleContour(double ax, double ay, double bx, double by, double cx, double cy) {operator =(SimpleContour(XY(ax,ay), XY(bx,by), XY(cx,cy)));} ///<Create an (eventually clockwise) triangle. Empty if degenerate.
    SimpleContour(const XY &c, double radius_x, double radius_y=0, double tilt_deg=0, double s_deg=0, double d_deg=360); 
    SimpleContour &operator =(const Block &b);
    SimpleContour &operator =(SimpleContour &&p) {if (this!=&p) swap(p);  return *this;}
    SimpleContour &operator =(const SimpleContour &p) {if (this!=&p) {if (p.size()) {edges=p.edges; boundingBox=p.boundingBox; clockwise=p.clockwise; area_cache=p.area_cache;} else clear();} return *this;}
    Edge &operator[](size_t edge) {return at(edge);}  ///<Returns reference to edge at index `i`.
    Edge *operator[](const XY &p) {size_t edge; EPointRelationType r = IsWithin(p, &edge); return r==WI_ON_EDGE||r==WI_ON_VERTEX ? &at(edge) : NULL;}  ///<Returns pointer to edge `p` is at, NULL if `p` is not on the contour.
    const Edge *operator[](const XY &p) const {size_t edge; EPointRelationType r = IsWithin(p, &edge); return r==WI_ON_EDGE||r==WI_ON_VERTEX ? &at(edge) : NULL;} ///<Returns const pointer to edge `p` is at, NULL if `p` is not on the contour.

    bool AddAnEdge(const Edge &edge);
    void Invert();

    EContourRelationType CheckContainment(const SimpleContour &b) const;

    EPointRelationType IsWithin(XY p, size_t *edge=NULL, double *pos=NULL, bool strict=true) const;
    void Shift(const XY &xy) {boundingBox.Shift(xy); for (size_t i=0; i<size(); i++) at(i).Shift(xy);} ///<Translates the shape.
    void Scale(double sc) {boundingBox.Scale(sc); for (size_t i=0; i<size(); i++) at(i).Scale(sc); area_cache.second*=sc*sc;} ///<Scale the shape.
    void SwapXY() {_ASSERT(IsSane()); boundingBox.SwapXY(); for (size_t i=0; i<size(); i++) at(i).SwapXY(); Invert(); clockwise=!clockwise; area_cache.second*=-1;} ///<Transpose the shape: swap XY coordinates (but keep clockwisedness).
    void Rotate(double cos, double sin, double radian) {for (size_t i=0; i<size(); i++) at(i).Rotate(cos, sin, radian); CalculateBoundingBox();} ///<Rotate the shape by `radian`. `sin` and `cos` are pre-computed values.
    void RotateAround(const XY&c, double cos, double sin, double radian) {for (size_t i=0; i<size(); i++) at(i).RotateAround(c, cos, sin, radian); CalculateBoundingBox();} ///<Rotate the shape by `radian` around `c`. `sin` and `cos` are pre-computed values.

    static Edge CreateRoundForExpand(const XY &start, const XY &end, const XY& old, bool clockwise);
    EContourRelationType RelationTo(const SimpleContour &c) const;
public:
    bool operator < (const SimpleContour &b) const;
    bool operator ==(const SimpleContour &b) const;
    const Edge &operator[](size_t edge) const {return at(edge);} ///<Returns const reference to edge at index `i`.
    void swap(SimpleContour &b) {edges.swap(b.edges); std::swap(boundingBox, b.boundingBox); std::swap(clockwise, b.clockwise); std::swap(area_cache, b.area_cache);} ///<Swap data with `b`.
    void clear() {edges.clear(); boundingBox.MakeInvalid(); area_cache.first = true; area_cache.second = 0.;} ///<Empty the shape by deleting all edges.

    void assign_dont_check(const std::vector<XY> &v);      ///<Set shape content to `v`. Assume edges in `v` connect and do not cross.
    void assign_dont_check(const XY v[], size_t size);  ///<Set shape content to `v` of size `size`. Assume edges in `v` connect and do not cross.
    template<size_t size> void assign_dont_check(const XY (&v)[size]) {assign_dont_check (v, size);} ///<Set shape content to `v`. Assume edges in `v` connect and do not cross.
    void assign_dont_check(const std::vector<Edge> &v);    ///<Set shape content to `v`. Assume edges in `v` connect and do not cross.
    void assign_dont_check(const Edge v[], size_t size);///<Set shape content to `v` of size `size`. Assume edges in `v` connect and do not cross.
    template<size_t size> void assign_dont_check(const Edge (&v)[size]) {assign_dont_check (v, size);} ///<Set shape content to `v`. Assume edges in `v` connect and do not cross.

    size_t size() const {return edges.size();}             ///<Returns the number of edges.
    const Block &GetBoundingBox() const {return boundingBox;} ///<Returns the bounding box.
    bool GetClockWise() const {return clockwise;}             ///<Returns if the shape is clockwise.
    bool IsEmpty() const {return edges.size()==0;}            ///<Returns if the shape is empty (no edges).
    bool IsSane() const;                                      ///<Checks if all edges connect, none degenerate, etc.
    double GetArea() const {return area_cache.first ? area_cache.second : CalcualteArea();}  ///<Return surface area of shape.
    double GetCircumference(bool include_hidden=false) const;   
    XY CentroidUpscaled() const;

    void VerticalCrossSection(double x, DoubleMap<bool> &section) const;
    double OffsetBelow(const SimpleContour &below, double &touchpoint, double offset=CONTOUR_INFINITY) const;
    void Expand(EExpandType type, double gap, Contour &res, double miter_limit/*=MaxVal(miter_limit)*/) const;
    void Expand2D(const XY &gap, Contour &res) const;

    void Path(cairo_t *cr, bool show_hidden) const;
    void Path(cairo_t *cr, bool show_hidden, bool clockwiseonly) const {
        if (clockwise==clockwiseonly)
            Path(cr, show_hidden);
    } ///<Draw the shape to the path of a cairo context, but only if `clockwiseonly` equals to the clockwisedness of this shape
    void PathDashed(cairo_t *cr, const double pattern[], unsigned num, bool show_hidden) const;
    void PathDashed(cairo_t *cr, const double pattern[], unsigned num, bool show_hidden, bool clockwiseonly) const {
        if (clockwise==clockwiseonly)
            PathDashed(cr, pattern, num, show_hidden);
    } ///<Draw the shape in dashed lines to the path of a cairo context, but only if `clockwiseonly` equals to the clockwisedness of this shape. 
    void Distance(const SimpleContour &o, DistanceType &ret) const;
    double Distance(const XY &o, XY &ret) const;
    double DistanceWithTangents(const XY &o, XY &ret, XY &t1, XY &t2) const;
    Range Cut(const Edge &e) const;
    Range CutWithTangent(const Edge &e, std::pair<XY, XY> &from, std::pair<XY, XY> &till) const;
    void Cut(const XY &A, const XY &B, DoubleMap<bool> &map) const;
    bool TangentFrom(const XY &from, XY &clockwise, XY &cclockwise) const;
    bool TangentFrom(const SimpleContour &from, XY clockwise[2], XY cclockwise[2]) const;
    EPointRelationType Tangents(const XY &p, XY &t1, XY &t2) const;
};

/** Simple comparison operator for container ordering */
inline bool SimpleContour::operator <(const SimpleContour &b) const
{
    if (boundingBox != b.boundingBox) return boundingBox < b.boundingBox;
    if (size() != b.size()) return size() < b.size();
    for (size_t i=0; i<size(); i++)
        if (at(i) != b[i]) return at(i)<b[i];
    return false; //equal
}

/** Tests equality. If all edges and vertices are the same, it can still return false if different edges have index 0. */
inline bool SimpleContour::operator ==(const SimpleContour &b) const
{
    if (boundingBox != b.boundingBox || size() != b.size()) return false;
    for (size_t i=0; i<size(); i++)
        if (at(i) != b[i]) return false;
    return true; //equal
}

/** Determine the relative vertical distance between two shapes.
 *
 * For detailed explanation and parameters, see 
 * Contour::OffsetBelow(const Contour &below, double &touchpoint, double offset).
 */
inline double SimpleContour::OffsetBelow(const SimpleContour &below, double &touchpoint, double offset) const
{
    if (offset < below.boundingBox.y.from - boundingBox.y.till) return offset;
    if (!boundingBox.x.Overlaps(below.boundingBox.x)) return offset;
    return do_offsetbelow(below, touchpoint, offset);
}


} //namespace

#endif //CONTOUR_CONTOUR_H
