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
#include <map>
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


class Path
{
    friend void contour_test(void);
    friend class Contour;
protected:
    std::vector<Edge> edges; ///<An ordered list of (directed) edges.
private:
    Edge       &at(size_t i) { return edges[i]; }          ///<Returns reference to edge at index `i`.
public:
    Path() = default;  ///<Create an empty path with no edges.
    Path(const XY &c, double radius_x, double radius_y = 0, double tilt_deg = 0, double s_deg = 0, double d_deg = 360);
    Path(const Path &p) = default; ///<Create a copy of another path
    Path(Path &&p) { swap(p); }  ///<Move content of another path. The other path gets undefined.
    Path(const std::vector<XY> &v) { assign(v); }      
    Path(const XY v[], size_t size) { assign(v, size); }
    template<size_t size> Path(const XY(&v)[size]) { assign(v, size); } 
    Path(const std::vector<Edge> &v) : edges(v) {}    
    Path(std::vector<Edge> &&v) : edges(std::move(v)) {}  
    Path(const Edge v[], size_t size) { assign(v, size); } 
    template<size_t size> Path(const Edge(&v)[size]) { assign(v, size); } ///<Set path content to `v`. 

    Path &operator =(Path &&p) { if (this!=&p) swap(p);  return *this; }
    Path &operator =(const Path &p) { if (this!=&p) edges = p.edges; return *this; }
    Edge &operator[](size_t edge) { return at(edge); }  ///<Returns reference to edge at index `i`.
    //Edge *operator[](const XY &p) { size_t edge; EPointRelationType r = IsWithin(p, &edge); return r==WI_ON_EDGE||r==WI_ON_VERTEX ? &at(edge) : NULL; }  ///<Returns pointer to edge `p` is at, NULL if `p` is not on the contour.
    //const Edge *operator[](const XY &p) const { size_t edge; EPointRelationType r = IsWithin(p, &edge); return r==WI_ON_EDGE||r==WI_ON_VERTEX ? &at(edge) : NULL; } ///<Returns const pointer to edge `p` is at, NULL if `p` is not on the contour.

    //bool operator < (const Path &b) const;
    //bool operator ==(const Path &b) const;
    const Edge &at(size_t i) const { return edges[i]; }          ///<Returns const reference to edge at index `i`.
    const Edge &operator[](size_t edge) const { return at(edge); } ///<Returns const reference to edge at index `i`.
    void swap(Path &b) { edges.swap(b.edges); };
    void clear() { edges.clear(); } ///<Empty the shape by deleting all edges.

    Path &append(const std::vector<XY> &v, bool ensure_connect = false);      ///<Append `v` to path. 
    Path &append(const XY v[], size_t size, bool ensure_connect = false);  ///<Append `v` of size `size` to path. 
    template<size_t size> Path &append(const XY(&v)[size]) { return append(v, size, ensure_connect); } ///<Append `v` to path. 
    Path &append(const std::vector<Edge> &v, bool ensure_connect = false);    ///<Append `v` to path. 
    Path &append(const Edge v[], size_t size, bool ensure_connect = false);///<Append `v` of size `size` to path. 
    template<size_t size> Path &append(const Edge(&v)[size]) { return append(v, size, ensure_connect); } ///<Append `v` to path. 
    Path &append(const Path &p, bool ensure_connect = false);      ///<Append `p` to path. 

    Path &assign(const std::vector<XY> &v) { clear(); append(v); return *this; }      ///<Set path content to `v`. 
    Path &assign(const XY v[], size_t size) { clear(); append(v, size); return *this; } ///<Set path content to `v` of size `size`. 
    template<size_t size> Path &assign(const XY(&v)[size]) { assign(v, size); return *this; } ///<Set path content to `v`. 
    Path &assign(const std::vector<Edge> &v) { edges = v; return *this; };    ///<Set path content to `v`. 
    Path &assign(std::vector<Edge> &&v) { edges.swap(v); return *this; };       ///<Set path content to `v`. 
    Path &assign(const Edge v[], size_t size) { clear(); append(v, size); return *this; } ///<Set path content to `v` of size `size`. 
    template<size_t size> Path &assign(const Edge(&v)[size]) { assign(v, size); return *this; } ///<Set path content to `v`. 
    Path &assign(const Path &p) { return *this = p; return *this; }      ///<Make us equal to p 
    Path &assign(Path &&p) { swap(p); return *this; return *this; }      ///<Make us equal to p 

    size_t size() const { return edges.size(); }             ///<Returns the number of edges.
    Block CalculateBoundingBox() const;
    bool IsEmpty() const { return edges.size()==0; }            ///<Returns if the shape is empty (no edges).
    void CairoPath(cairo_t *cr, bool show_hidden) const;
    void CairoPathDashed(cairo_t *cr, const double pattern[], unsigned num, bool show_hidden) const;

    std::list<std::vector<Edge>> ConvertToClosed(bool close_everything=false) const;

    Path &Invert(); ///<Invert the direction of the path
    Path &Shift(const XY &xy) { for (auto &e : edges) e.Shift(xy); return *this; } ///<Translates the path.
    Path &Scale(double sc) { for (auto &e : edges) e.Scale(sc); return *this;} ///<Scale the path.
    Path &Scale(const XY &sc) { for (auto &e : edges) e.Scale(sc); return *this;} ///<Scale the path.
    Path &SwapXY() { for (auto &e : edges) e.SwapXY(); return *this;} ///<Transpose the path: swap XY coordinates 
    Path &Rotate(double degree) { return Rotate(cos(degree/180*M_PI), sin(degree/180*M_PI)); }
    Path &Rotate(double cos, double sin) { for (auto &e : edges) e.Rotate(cos, sin); return *this;} ///<Rotate the path by `radian`. `sin` and `cos` are pre-computed values.
    Path &RotateAround(const XY&c, double degree) { return RotateAround(c, cos(degree/180*M_PI), sin(degree/180*M_PI)); }
    Path &RotateAround(const XY&c, double cos, double sin) { for (auto &e : edges) e.RotateAround(c, cos, sin); return *this; } ///<Rotate the path by `radian` around `c`. `sin` and `cos` are pre-computed values.

    Path CreateInverted() const { Path tmp(*this); return tmp.Invert(); } ///<Invert the direction of the path
    Path CreateShifted(const XY &xy) const { Path tmp(*this); return tmp.Shift(xy); } ///<Translates the path.
    Path CreateScaled(double sc) const { Path tmp(*this); return tmp.Scale(sc); } ///<Scale the path.
    Path CreateScaled(const XY &sc) const { Path tmp(*this); return tmp.Scale(sc); } ///<Scale the path.
    Path CreateSwapXYd() const { Path tmp(*this); return tmp.SwapXY(); } ///<Transpose the path: swap XY coordinates 
    Path CreateRotated(double degree) const { Path tmp(*this); return tmp.Rotate(degree); } ///<Rotate the path by `radian`. `sin` and `cos` are pre-computed values.
    Path CreateRotated(double cos, double sin) const { Path tmp(*this); return tmp.Rotate(cos, sin); } ///<Rotate the path by `radian`. `sin` and `cos` are pre-computed values.
    Path CreateRotatedAround(const XY&c, double degree) const { Path tmp(*this); return tmp.RotateAround(c, degree); } ///<Rotate the path by `radian` around `c`. `sin` and `cos` are pre-computed values.
    Path CreateRotatedAround(const XY&c, double cos, double sin) const { Path tmp(*this); return tmp.RotateAround(c, cos, sin); } ///<Rotate the path by `radian` around `c`. `sin` and `cos` are pre-computed values.

    void AddAnEdge(const Edge &edge, bool ensure_connect = false);
};

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
    friend class ContourWithHoles;
    friend class ContourList;
    friend class ContoursHelper; //For walk related memebers
    friend struct node;
    friend void contour_test(void);

    struct ExpandMetaData : public Edge
    {
        const int           original_edge;
        Edge::EExpandCPType cross_type; //The type of crossing between edge #i and #i+1 (wrapped around)
        XY                  original_point; // The original end vertex (for creating the round circle)
        XY                  cross_point;//The cp between edge #i and #i+1 (wrapped around)
        double              us_end_pos;
        double              next_start_pos;

        ExpandMetaData(const Edge &e, int o=-1, Edge::EExpandCPType a = Edge::CP_TRIVIAL, const XY &b = XY(), const XY &c = XY(), double ue=double(), double ns=double()) :
            Edge(e), original_edge(o), cross_type(a), original_point(b), cross_point(c), us_end_pos(ue), next_start_pos(ns) {}
        ExpandMetaData(const XY&s, const XY &d, bool v = true, Edge::EExpandCPType a = Edge::CP_TRIVIAL, const XY &b = XY(), const XY &c = XY(), double ue = double(), double ns = double()) :
            Edge(s, d, v), original_edge(-1), cross_type(a), original_point(b), cross_point(c), us_end_pos(ue), next_start_pos(ns) {}
        ExpandMetaData(const XY&s, const XY &d, const XY&c1, const XY &c2, bool v = true, Edge::EExpandCPType a = Edge::CP_TRIVIAL, const XY &b = XY(), const XY &c = XY(), double ue = double(), double ns = double()) :
            Edge(s, d, c1, c2, v), original_edge(-1), cross_type(a), original_point(b), cross_point(c), us_end_pos(ue), next_start_pos(ns) {}
    };

protected:
    std::vector<Edge> edges; ///<An ordered list of (directed) edges.
private:
    mutable bool clockwise_fresh : 1;
    mutable bool boundingBox_fresh : 1;
    mutable bool area_fresh : 1;
    mutable bool clockwise : 1;
    mutable Block  boundingBox;      ///<The bounding box for the shape.
    mutable double area;     ///<if nonzero, it tells the surface area of the shape. Negative for counterclockwise shapes.

    EContourRelationType CheckContainmentHelper(const SimpleContour &b) const;
    double do_offsetbelow(const SimpleContour &below, double &touchpoint, double offset=CONTOUR_INFINITY) const;

    const Block &CalculateBoundingBox() const;

    double CalculateArea() const;
    void CalculateClockwise() const { if (!clockwise_fresh) { if (!area_fresh) CalculateArea(); clockwise = area>=0; clockwise_fresh = true; } }
    void AppendDuringWalk(const Edge &, const XY *prev_xy);
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

    static void CreateRoundForExpand(const XY &center, const XY &start, const XY &end, bool clockwise,
                                     std::list<ExpandMetaData> &insert_to, std::list<ExpandMetaData>::iterator here,
                                     bool visible);
    void Expand2DHelper(const XY &gap, std::vector<Edge> &a,
                        unsigned original_last, unsigned next, unsigned my_index,
                        int last_type, int stype) const;
protected:
    SimpleContour() : clockwise_fresh(true), boundingBox_fresh(true), area_fresh(true), clockwise(true), boundingBox(true), area(0)  {}  ///<Create an empty shape with no edges.
    SimpleContour(SimpleContour &&p) {swap(p);}  ///<Move content of another shape. The other shape gets undefined.
    SimpleContour(const SimpleContour &p) : edges(p.edges), clockwise_fresh(p.clockwise_fresh), boundingBox_fresh(p.boundingBox_fresh), area_fresh(p.area_fresh), clockwise(p.clockwise), boundingBox(p.boundingBox), area(p.area)  {} ///<Create a copy of another shape
    SimpleContour(double sx, double dx, double sy, double dy) {operator = (Block(sx,dx,sy,dy));} ///<Create a rectangle
    SimpleContour(const Block &b) {operator =(b);} ///<Create a rectangle
    SimpleContour(XY a, XY b, XY c); ///<Create an (eventually clockwise) triangle. Empty if degenerate.
    SimpleContour(double ax, double ay, double bx, double by, double cx, double cy) : SimpleContour(XY(ax,ay), XY(bx,by), XY(cx,cy)) {} ///<Create an (eventually clockwise) triangle. Empty if degenerate.
    SimpleContour(const XY &c, double radius_x, double radius_y=0, double tilt_deg=0, double s_deg=0, double d_deg=360);
    SimpleContour &operator =(const Block &b);
    SimpleContour &operator =(SimpleContour &&p) {if (this!=&p) swap(p);  return *this;}
    SimpleContour &operator =(const SimpleContour &p) { if (this!=&p) { if (p.size()) { edges = p.edges; boundingBox = p.boundingBox; area = p.area; clockwise = p.clockwise; clockwise_fresh=p.clockwise_fresh; area_fresh=p.area_fresh; boundingBox_fresh=p.boundingBox_fresh; } else clear(); } return *this; }
    Edge &operator[](size_t edge) {return at(edge);}  ///<Returns reference to edge at index `i`.
    Edge *operator[](const XY &p) {size_t edge; EPointRelationType r = IsWithin(p, &edge); return r==WI_ON_EDGE||r==WI_ON_VERTEX ? &at(edge) : NULL;}  ///<Returns pointer to edge `p` is at, NULL if `p` is not on the contour.
    const Edge *operator[](const XY &p) const {size_t edge; EPointRelationType r = IsWithin(p, &edge); return r==WI_ON_EDGE||r==WI_ON_VERTEX ? &at(edge) : NULL;} ///<Returns const pointer to edge `p` is at, NULL if `p` is not on the contour.

    bool AddAnEdge(const Edge &edge);
    SimpleContour &Invert();
    SimpleContour CreateInvert() const { return SimpleContour(*this).Invert(); }

    EContourRelationType CheckContainment(const SimpleContour &b) const;

    EPointRelationType IsWithin(XY p, size_t *edge=NULL, double *pos=NULL, bool strict=true) const;
    void Shift(const XY &xy) { boundingBox.Shift(xy); for (auto &e : edges) e.Shift(xy); } ///<Translates the shape.
    void Scale(double sc) { boundingBox.Scale(sc); for (auto &e : edges) e.Scale(sc); area *= sc*sc; } ///<Scale the shape.
    void Scale(const XY &sc) { boundingBox.Scale(sc); for (auto &e : edges) e.Scale(sc); area_fresh=false; } ///<Scale the shape.
    void SwapXY() { _ASSERT(IsSane()); boundingBox.SwapXY(); for (auto &e : edges) e.SwapXY(); Invert(); area *= -1; clockwise = !clockwise; } ///<Transpose the shape: swap XY coordinates (but keep clockwisedness).
    void Rotate(double cos, double sin) { for (auto &e : edges) e.Rotate(cos, sin); boundingBox_fresh = false; } ///<Rotate the shape by `radian`. `sin` and `cos` are pre-computed values.
    void RotateAround(const XY&c, double cos, double sin) { for (auto &e : edges) e.RotateAround(c, cos, sin); boundingBox_fresh = false; } ///<Rotate the shape by `radian` around `c`. `sin` and `cos` are pre-computed values.

    EContourRelationType RelationTo(const SimpleContour &c) const;
public:
    bool operator < (const SimpleContour &b) const;
    bool operator ==(const SimpleContour &b) const;
    const Edge &operator[](size_t edge) const {return at(edge);} ///<Returns const reference to edge at index `i`.
    void swap(SimpleContour &b);
    void clear() { edges.clear(); clockwise_fresh=true; area_fresh = true; boundingBox_fresh = true;  boundingBox.MakeInvalid(); area = 0.; clockwise = true; } ///<Empty the shape by deleting all edges.

    void assign_dont_check(const std::vector<XY> &v);      ///<Set shape content to `v`. Assume edges in `v` connect and do not cross.
    void assign_dont_check(const XY v[], size_t size);  ///<Set shape content to `v` of size `size`. Assume edges in `v` connect and do not cross.
    template<size_t size> void assign_dont_check(const XY (&v)[size]) {assign_dont_check (v, size);} ///<Set shape content to `v`. Assume edges in `v` connect and do not cross.
    void assign_dont_check(const std::vector<Edge> &v);    ///<Set shape content to `v`. Assume edges in `v` connect and do not cross.
    void assign_dont_check(std::vector<Edge> &&v);       ///<Set shape content to `v`. Assume edges in `v` connect and do not cross.
    void assign_dont_check(const Edge v[], size_t size);///<Set shape content to `v` of size `size`. Assume edges in `v` connect and do not cross.
    template<size_t size> void assign_dont_check(const Edge (&v)[size]) {assign_dont_check (v, size);} ///<Set shape content to `v`. Assume edges in `v` connect and do not cross.

    size_t size() const {return edges.size();}             ///<Returns the number of edges.
    const Block &GetBoundingBox() const { if (!boundingBox_fresh) CalculateBoundingBox(); return boundingBox; } ///<Returns the bounding box.
    bool GetClockWise() const { if (!clockwise_fresh) CalculateClockwise(); return clockwise; }              ///<Returns if the shape is clockwise.
    bool IsEmpty() const {return edges.size()==0;}            ///<Returns if the shape is empty (no edges).
    bool IsSane() const;                                      ///<Checks if all edges connect, none degenerate, etc.
    double GetArea() const {return area_fresh ? area : CalculateArea();}  ///<Return surface area of shape.
    double GetCircumference(bool include_hidden=false) const;
    XY CentroidUpscaled() const;

    void VerticalCrossSection(double x, DoubleMap<bool> &section) const;
    double OffsetBelow(const SimpleContour &below, double &touchpoint, double offset=CONTOUR_INFINITY) const;
    void Expand(EExpandType type, double gap, Contour &res, double miter_limit/*=MaxVal(miter_limit)*/) const;
    void Expand2D(const XY &gap, Contour &res) const;

    void CairoPath(cairo_t *cr, bool show_hidden) const;
    void CairoPath(cairo_t *cr, bool show_hidden, bool clockwiseonly) const {
        if (GetClockWise()==clockwiseonly)
            CairoPath(cr, show_hidden);
    } ///<Draw the shape to the path of a cairo context, but only if `clockwiseonly` equals to the clockwisedness of this shape
    void CairoPathDashed(cairo_t *cr, const double pattern[], unsigned num, bool show_hidden) const;
    void CairoPathDashed(cairo_t *cr, const double pattern[], unsigned num, bool show_hidden, bool clockwiseonly) const {
        if (GetClockWise()==clockwiseonly)
            CairoPathDashed(cr, pattern, num, show_hidden);
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
    if (boundingBox_fresh && b.boundingBox_fresh && boundingBox != b.boundingBox)
        return boundingBox < b.boundingBox;
    if (size() != b.size()) return size() < b.size();
    for (size_t i=0; i<size(); i++)
        if (at(i) != b[i]) return at(i)<b[i];
    return false; //equal
}

/** Tests equality. If all edges and vertices are the same, it can still return false if different edges have index 0. */
inline bool SimpleContour::operator ==(const SimpleContour &b) const
{
    if (boundingBox_fresh && b.boundingBox_fresh && boundingBox != b.boundingBox)
        return false;
    if (size() != b.size())
        return false;
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
    if (offset < below.GetBoundingBox().y.from - GetBoundingBox().y.till) return offset;
    if (!GetBoundingBox().x.Overlaps(below.GetBoundingBox().x)) return offset;
    return do_offsetbelow(below, touchpoint, offset);
}


} //namespace contour



#endif //CONTOUR_CONTOUR_H
