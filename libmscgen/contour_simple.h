#if !defined(CONTOUR_CONTOUR_H)
#define CONTOUR_CONTOUR_H

#include <vector>
#include <list>
#include "contour_edge.h"


namespace contour {


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
    typedef enum {OVERLAP, A_IS_EMPTY, B_IS_EMPTY, BOTH_EMPTY, A_INSIDE_B, B_INSIDE_A, SAME, APART} result_t;
    std::vector<Edge> edges;
public:
    typedef std::vector<Edge>::size_type size_type;
private:
    Block  boundingBox;
    bool   clockwise;

    result_t CheckContainmentHelper(const SimpleContour &b) const;
    double do_offsetbelow(const SimpleContour &below, double &touchpoint, double offset=CONTOUR_INFINITY) const;

    const Block &CalculateBoundingBox();

    friend class ContoursHelper; //For walk related memebers
    void CalculateClockwise();
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
    SimpleContour() : clockwise(true) {boundingBox.MakeInvalid();}
    SimpleContour(SimpleContour &&p) {swap(p);}
    SimpleContour(const SimpleContour &p) : edges(p.edges), boundingBox(p.boundingBox), clockwise(p.clockwise) {}
    SimpleContour(double sx, double dx, double sy, double dy) {operator = (Block(sx,dx,sy,dy));}
    SimpleContour(const Block &b) {operator =(b);}
    SimpleContour(XY a, XY b, XY c);
    SimpleContour(double ax, double ay, double bx, double by, double cx, double cy) {operator =(SimpleContour(XY(ax,ay), XY(bx,by), XY(cx,cy)));}
    SimpleContour(const XY &c, double radius_x, double radius_y=0, double tilt_deg=0, double s_deg=0, double d_deg=360);
    SimpleContour &operator =(const Block &b);
    SimpleContour &operator =(SimpleContour &&p) {if (this!=&p) swap(p);  return *this;}
    SimpleContour &operator =(const SimpleContour &p) {if (this!=&p) {if (p.size()) {edges=p.edges; boundingBox=p.boundingBox;} else clear();} return *this;}
    Edge &operator[](size_type edge) {return at(edge);}
    Edge *operator[](const XY &p) {size_type edge; is_within_t r = IsWithin(p, &edge); return r==WI_ON_EDGE||r==WI_ON_VERTEX ? &at(edge) : NULL;}
    const Edge *operator[](const XY &p) const {size_type edge; is_within_t r = IsWithin(p, &edge); return r==WI_ON_EDGE||r==WI_ON_VERTEX ? &at(edge) : NULL;}

    bool AddAnEdge(const Edge &edge);
    void Invert();

    result_t CheckContainment(const SimpleContour &b) const;

    is_within_t IsWithin(XY p, size_type *edge=NULL, double *pos=NULL, bool strict=true) const;
    void Shift(const XY &xy) {boundingBox.Shift(xy); for (size_type i=0; i<size(); i++) at(i).Shift(xy);}
    void SwapXY() {_ASSERT(IsSane()); boundingBox.SwapXY(); for (size_type i=0; i<size(); i++) at(i).SwapXY(); Invert(); clockwise=!clockwise;}
    void Rotate(double cos, double sin, double radian) {for (size_type i=0; i<size(); i++) at(i).Rotate(cos, sin, radian); CalculateBoundingBox();}
    void RotateAround(const XY&c, double cos, double sin, double radian) {for (size_type i=0; i<size(); i++) at(i).RotateAround(c, cos, sin, radian); CalculateBoundingBox();}

    static Edge CreateRoundForExpand(const XY &start, const XY &end, const XY& old, bool clockwise);

public:
    bool operator < (const SimpleContour &b) const;
    bool operator ==(const SimpleContour &b) const;
    const Edge &operator[](size_type edge) const {return at(edge);}
    void swap(SimpleContour &b) {edges.swap(b.edges); std::swap(boundingBox, b.boundingBox); std::swap(clockwise, b.clockwise);}
    void clear() {edges.clear(); boundingBox.MakeInvalid();}

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
    double GetArea() const;
    double GetCircumference(bool include_hidden=false) const;

    void VerticalCrossSection(double x, DoubleMap<bool> &section) const;
    double OffsetBelow(const SimpleContour &below, double &touchpoint, double offset=CONTOUR_INFINITY) const;
    void Expand(EExpandType type, double gap, Contour &res, double miter_limit/*=DBL_MAX*/) const;

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

} //namespace

#endif //CONTOUR_CONTOUR_H
