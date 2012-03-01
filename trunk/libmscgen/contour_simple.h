#if !defined(CONTOUR_CONTOUR_H)
#define CONTOUR_CONTOUR_H

#include <vector>
#include <list>
#include "contour_edge.h"


namespace contour {

//This is a template that allows storing of state along a 1-dimensional axis.
//The state can change at any real (double) value. 
//The template parameter contains the type of state.
//We have constructs to set the state at a double walue and onwards or between two doubles
//We also have members to query the state at any point and ask where will the next change be.
//Finally, if the element supports the += operator we can add changes to the state.

template <class element>
class DoubleMap : public std::map<double, element>
{
public:
    using std::map<double, element>::operator[];   //indicate to two-stage (dependent) name lookup
    using std::map<double, element>::upper_bound;  //that we want these names (not dependent on "element")
    using std::map<double, element>::lower_bound;  //looked up only at instantiation time (from base class)
    using std::map<double, element>::begin;        //and not during template compilation (searched in global scope).
    using std::map<double, element>::end;

    DoubleMap() {};
    DoubleMap(const element &e) {insert(typename std::map<double, element>::value_type(-CONTOUR_INFINITY, e));
                                 insert(typename std::map<double, element>::value_type(CONTOUR_INFINITY, e));}
    void Set(double pos, const element&e) {operator[](pos) = e;}
    void Set(const Range &r, const element &e);
    void Add(double pos, const element&e);     //assumes element has operator +=
    void Add(const Range &r, const element&e); //assumes element has operator +=
    const element* Get(double pos) const {auto i=upper_bound(pos); return i==begin()?NULL:&(--i)->second;}
    double Till(double pos) const {auto i=upper_bound(pos); return i==end()?CONTOUR_INFINITY:i->first;}
    double From(double pos) const {auto i=--lower_bound(pos); return i==end()?-CONTOUR_INFINITY:i->first;}
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



class Contour;

//inside of contour is to the right as we go through: counterclockwise pointlists contain the "outside"
//never degenerate - always has actual area
//never flopped - edges never cross each-other
//always contagious - one single surface
//always goes in clockwise direction - except internally
class SimpleContour
{
    friend class Contour;
protected:
    typedef enum {OVERLAP=0, A_IS_EMPTY, B_IS_EMPTY, BOTH_EMPTY, A_INSIDE_B, B_INSIDE_A, 
                  SAME, APART, A_IN_HOLE_OF_B, B_IN_HOLE_OF_A, IN_HOLE_APART} result_t;
    static bool result_overlap(result_t t) {return t==OVERLAP || t==A_INSIDE_B || t==B_INSIDE_A || t==SAME;}
    static result_t switch_side(result_t t);
    std::vector<Edge> edges;
public:
    typedef std::vector<Edge>::size_type size_type;
private:
    Block  boundingBox;
    bool   clockwise;
    mutable std::pair<bool, double> area_cache;

    result_t CheckContainmentHelper(const SimpleContour &b) const;
    double do_offsetbelow(const SimpleContour &below, double &touchpoint, double offset=CONTOUR_INFINITY) const;

    const Block &CalculateBoundingBox();

    friend class ContoursHelper; //For walk related memebers
    void CalculateClockwise();
    double CalcualteArea() const;
    void AppendDuringWalk(const Edge &);
    bool PostWalk();
    bool Sanitize();

    size_type next(size_type vertex) const {return size()<=1 ? 0 : (vertex+1)%size();}
    size_type prev(size_type vertex) const {return size()<=1 ? 0 : (vertex-1+size())%size();}
    Edge       &at(size_type i)       {return edges[i];}
    const Edge &at(size_type i) const {return edges[i];}
    Edge       &at_next(size_type i)       {return at(next(i));}
    const Edge &at_next(size_type i) const {return at(next(i));}
    Edge       &at_prev(size_type i)       {return at(prev(i));}
    const Edge &at_prev(size_type i) const {return at(prev(i));}

    //returns a point on the line of a tangent at "pos", the point being towards the start of curve/edge.
    XY PrevTangentPoint(size_type edge, double pos) const
        {return (test_smaller(0, pos) ? at(edge) : at_prev(edge)).PrevTangentPoint(test_smaller(0, pos) ? pos : 1);}
    //returns a point on the line of a tangent at "pos", the point being towards the end of curve/edge.
    XY NextTangentPoint(size_type edge, double pos) const
        {return (test_smaller(pos, 1) ? at(edge) : at_next(edge)).NextTangentPoint(test_smaller(pos, 1) ? pos : 0);}

protected:
    friend class ContourWithHoles;
    friend class ContourList;
    friend struct node;
    SimpleContour() : clockwise(true), area_cache(true, 0.) {boundingBox.MakeInvalid();}
    SimpleContour(SimpleContour &&p) {swap(p);}
    SimpleContour(const SimpleContour &p) : edges(p.edges), boundingBox(p.boundingBox), clockwise(p.clockwise), area_cache(p.area_cache) {}
    SimpleContour(double sx, double dx, double sy, double dy) {operator = (Block(sx,dx,sy,dy));}
    SimpleContour(const Block &b) {operator =(b);}
    SimpleContour(XY a, XY b, XY c);
    SimpleContour(double ax, double ay, double bx, double by, double cx, double cy) {operator =(SimpleContour(XY(ax,ay), XY(bx,by), XY(cx,cy)));}
    SimpleContour(const XY &c, double radius_x, double radius_y=0, double tilt_deg=0, double s_deg=0, double d_deg=360);
    SimpleContour &operator =(const Block &b);
    SimpleContour &operator =(SimpleContour &&p) {if (this!=&p) swap(p);  return *this;}
    SimpleContour &operator =(const SimpleContour &p) {if (this!=&p) {if (p.size()) {edges=p.edges; boundingBox=p.boundingBox; clockwise=p.clockwise; area_cache=p.area_cache;} else clear();} return *this;}
    Edge &operator[](size_type edge) {return at(edge);}
    Edge *operator[](const XY &p) {size_type edge; is_within_t r = IsWithin(p, &edge); return r==WI_ON_EDGE||r==WI_ON_VERTEX ? &at(edge) : NULL;}
    const Edge *operator[](const XY &p) const {size_type edge; is_within_t r = IsWithin(p, &edge); return r==WI_ON_EDGE||r==WI_ON_VERTEX ? &at(edge) : NULL;}

    bool AddAnEdge(const Edge &edge);
    void Invert();

    result_t CheckContainment(const SimpleContour &b) const;

    is_within_t IsWithin(XY p, size_type *edge=NULL, double *pos=NULL, bool strict=true) const;
    void Shift(const XY &xy) {boundingBox.Shift(xy); for (size_type i=0; i<size(); i++) at(i).Shift(xy);}
    void Scale(double sc) {boundingBox.Scale(sc); for (size_type i=0; i<size(); i++) at(i).Scale(sc); area_cache.second*=sc*sc;}
    void SwapXY() {_ASSERT(IsSane()); boundingBox.SwapXY(); for (size_type i=0; i<size(); i++) at(i).SwapXY(); Invert(); clockwise=!clockwise; area_cache.second*=-1;}
    void Rotate(double cos, double sin, double radian) {for (size_type i=0; i<size(); i++) at(i).Rotate(cos, sin, radian); CalculateBoundingBox();}
    void RotateAround(const XY&c, double cos, double sin, double radian) {for (size_type i=0; i<size(); i++) at(i).RotateAround(c, cos, sin, radian); CalculateBoundingBox();}


    static Edge CreateRoundForExpand(const XY &start, const XY &end, const XY& old, bool clockwise);
    result_t RelationTo(const SimpleContour &c) const;
public:
    bool operator < (const SimpleContour &b) const;
    bool operator ==(const SimpleContour &b) const;
    const Edge &operator[](size_type edge) const {return at(edge);}
    void swap(SimpleContour &b) {edges.swap(b.edges); std::swap(boundingBox, b.boundingBox); std::swap(clockwise, b.clockwise); std::swap(area_cache, b.area_cache);}
    void clear() {edges.clear(); boundingBox.MakeInvalid(); area_cache.first = true; area_cache.second = 0.;}

    void assign_dont_check(const std::vector<XY> &v);
    void assign_dont_check(const XY v[], size_type size);
    template<size_type size> void assign_dont_check(const XY (&v)[size]) {assign_dont_check (v, size);}
    void assign_dont_check(const std::vector<Edge> &v);
    void assign_dont_check(const Edge v[], size_type size);
    template<size_type size> void assign_dont_check(const Edge (&v)[size]) {assign_dont_check (v, size);}

    size_type size() const {return edges.size();}
    const Block &GetBoundingBox() const {return boundingBox;}
    bool GetClockWise() const {return clockwise;}
    bool IsEmpty() const {return edges.size()==0;}
    bool IsSane() const;
    double GetArea() const {return area_cache.first ? area_cache.second : CalcualteArea();}
    double GetCircumference(bool include_hidden=false) const;
    XY CentroidUpscaled() const;

    void VerticalCrossSection(double x, DoubleMap<bool> &section) const;
    double OffsetBelow(const SimpleContour &below, double &touchpoint, double offset=CONTOUR_INFINITY) const;
    void Expand(EExpandType type, double gap, Contour &res, double miter_limit/*=MaxVal(miter_limit)*/) const;

    void Path(cairo_t *cr, bool show_hidden) const;
    void Path(cairo_t *cr, bool show_hidden, bool clockwiseonly) const {
        if (clockwise==clockwiseonly)
            Path(cr, show_hidden);
    }
    void PathDashed(cairo_t *cr, const double pattern[], unsigned num, bool show_hidden) const;
    void PathDashed(cairo_t *cr, const double pattern[], unsigned num, bool show_hidden, bool clockwiseonly) const {
        if (clockwise==clockwiseonly)
            PathDashed(cr, pattern, num, show_hidden);
    }
    template<unsigned D> void Distance(const SimpleContour &o, DistanceType<D> &ret) const;
};

inline bool SimpleContour::operator <(const SimpleContour &b) const
{
    if (boundingBox != b.boundingBox) return boundingBox < b.boundingBox;
    if (size() != b.size()) return size() < b.size();
    for (size_type i=0; i<size(); i++)
        if (at(i) != b[i]) return at(i)<b[i];
    return false; //equal
}

inline bool SimpleContour::operator ==(const SimpleContour &b) const
{
    if (boundingBox != b.boundingBox || size() != b.size()) return false;
    for (size_type i=0; i<size(); i++)
        if (at(i) != b[i]) return false;
    return true; //equal
}

inline double SimpleContour::OffsetBelow(const SimpleContour &below, double &touchpoint, double offset) const
{
    if (offset < below.boundingBox.y.from - boundingBox.y.till) return offset;
    if (!boundingBox.x.Overlaps(below.boundingBox.x)) return offset;
    return do_offsetbelow(below, touchpoint, offset);
}

//The distance of the two contours and two points on them
//returns negative one is inside the other, zero if partial overlap only (two points equal)
//"inside" here ignores clockwiseness
//ret can contain the result of previous searches
//we do not check bb, assume caller calls us if needed
template<unsigned D>
void SimpleContour::Distance(const SimpleContour &o, DistanceType<D> &ret) const
{
    if (IsEmpty() || o.IsEmpty()) return;
    DistanceType<D> running = ret, tmp;
    running.MakeAllOutside();
    //both running and tmp are positive throught this call, except at the very end
    for (unsigned u = 0; u<size(); u++) 
        for (unsigned v=0; v<o.size(); v++) {
            if (at(u).GetType() == Edge::STRAIGHT && o[v].GetType() == Edge::STRAIGHT) 
                at(u).Distance(o[v], tmp);
            else if (running.ConsiderBB(fabs(at(u).GetBoundingBox().Distance(o[v].GetBoundingBox())))) 
                //if not both are straight try to avoid calculation if bbs are far enough
                at(u).Distance(o[v], tmp);
            else continue;
            running.Merge(tmp);
            if (running.IsZero()) goto final_merge;
        }
    //now check if one is in the other - they cannot cross each other or else d would be 0
    _ASSERT(RelationTo(o)!=OVERLAP);
    if (IsWithin(o[0].GetStart()) == WI_INSIDE || o.IsWithin(at(0).GetStart()) == WI_INSIDE) 
        running.MakeAllInside();
final_merge:
    ret.Merge(running);
}

} //namespace

#endif //CONTOUR_CONTOUR_H
