#if !defined(CONTOUR_CONTOUR_H)
#define CONTOUR_CONTOUR_H

#include <vector>
#include <list>
#include "contour_edge.h"

class ContourList;
class ContourHelper;

//inside of contour is to the right as we go through: counterclockwise pointlists contain the "outside"
//never degenerate - always has actual area
//never flopped - edges never cross each-other
//always contagious - one single surface
//always goes in clockwise direction - except internally
class Contour : protected std::vector<Edge>
{
    friend class ContourHelper;

    friend class ContourList; //to access the (std::vector<Edge> &&) constructor & GetInverse
	friend class node_list;   //to access CheckContainment
    explicit Contour(std::vector<Edge> &&v) {std::vector<Edge>::swap(v);} //leave boundingBox!!
    explicit Contour(const std::vector<Edge> &v) : std::vector<Edge>(v) {} //leave boundingBox!!
    double do_offsetbelow(const Contour &below, double &touchpoint) const;
protected:
    Block  boundingBox;

    Contour CreateInverse() const;

public:
    typedef enum {OVERLAP, A_IS_EMPTY, B_IS_EMPTY, BOTH_EMPTY, A_INSIDE_B, B_INSIDE_A, SAME, APART} result_t;
	typedef enum {WINDING_RULE, EVENODD_RULE, EXPAND_RULE, XOR_RULE} untangle_t;
protected:
    is_within_t   IsWithin(XY p, int *edge=NULL, double *pos=NULL) const;
    result_t CheckContainmentHelper(const Contour &b) const;
    result_t CheckContainment(const Contour &b) const;
    bool CalculateClockwise() const;
    void AppendDuringWalk(const Edge &);
    bool PostWalk();
    result_t UnionIntersect(const Contour &b, ContourList &result, bool doUnion) const;
    result_t Union(const Contour &b, ContourList &result) const {return UnionIntersect(b, result, true);}
    result_t Intersect(const Contour &b, ContourList &result) const {return UnionIntersect(b, result, false);}
    result_t Substract(const Contour &b, ContourList &result) const {return UnionIntersect(b.CreateInverse(), result, false);}
    result_t DoXor(const Contour &b, ContourList &result) const;
	result_t Untangle(ContourList &result, untangle_t rule) const;

    int next(int vertex) const {return (vertex+1)%size();}
    int prev(int vertex) const {return (vertex-1+size())%size();}
    Edge       &at_next(int i)       {return at(next(i));}
    const Edge &at_next(int i) const {return at(next(i));}
    Edge       &at_prev(int i)       {return at(prev(i));}
    const Edge &at_prev(int i) const {return at(prev(i));}

    void Rotate(double cos, double sin, double radian) {for (unsigned i=0; i<size(); i++) at(i).Rotate(cos, sin, radian); CalculateBoundingBox();}
    void Rotate(double degrees) {double r=deg2rad(degrees); Rotate(cos(r), sin(r), r);}
    void RotateAround(const XY&c, double cos, double sin, double radian) {for (unsigned i=0; i<size(); i++) at(i).RotateAround(c, cos, sin, radian); CalculateBoundingBox();}
    void RotateAround(const XY&c, double degrees) {double r=deg2rad(degrees); RotateAround(c, cos(r), sin(r), r);}

    void DoVerticalCrossSection(double x, DoubleMap<bool> &section, bool add) const;
    void Expand(EExpandType type, double gap, ContourList &res) const;
public:
    Contour() {boundingBox.MakeInvalid();}
    Contour(Contour &&p) {swap(p);}
	Contour(const Contour &p) : std::vector<Edge>(p), boundingBox(p.boundingBox) {}
    Contour(double sx, double dx, double sy, double dy) {operator = (Block(sx,dx,sy,dy));}
    Contour(const Block &b) {operator =(b);}
    Contour(XY a, XY b, XY c);
    Contour(double ax, double ay, double bx, double by, double cx, double cy);
    Contour(const XY &c, double radius_x, double radius_y=0, double tilt_deg=0, double s_deg=0, double d_deg=360);
    Contour &operator =(const Block &b);

    bool operator < (const Contour &b) const;
    bool operator ==(const Contour &b) const;
    Contour &operator=(Contour &&p) {swap(p);  return *this;}
    Contour &operator=(const Contour &p) {if (p.size()) {std::vector<Edge>::operator=(p); boundingBox=p.boundingBox;} else clear();  return *this;}

    //returns a point on the line of a tangent at "pos", the point being towards the start of curve/edge.
	XY PrevTangentPoint(int edge, double pos) const {return at(edge).PrevTangentPoint(pos, at_prev(edge));}
    //returns a point on the line of a tangent at "pos", the point being towards the end of curve/edge.
    XY NextTangentPoint(int edge, double pos) const {return at(edge).NextTangentPoint(pos, at_next(edge));}

    const Block &GetBoundingBox() const {return boundingBox;}
    const Block &CalculateBoundingBox();
    void swap(Contour &b) {std::vector<Edge>::swap(b); std::swap(boundingBox, b.boundingBox);}
    void clear() {std::vector<Edge>::clear(); boundingBox.MakeInvalid();}
    unsigned  size() const {return std::vector<Edge>::size();}
    bool IsEmpty() const {return std::vector<Edge>::size()==0;}
    const Edge &GetEdge(int edge) const {return at(edge);}
    bool AddPoint(const XY &xy) {return AddAnEdge(Edge(xy, at(0).start));}
    bool AddAnEdge(const Edge &edge);
    bool OpenHere(const XY &xy);

    void Shift(const XY &xy) {boundingBox.Shift(xy); for (unsigned i=0; i<size(); i++) at(i).Shift(xy);}
    Contour CreateShifted(const XY & xy) const {Contour a(*this); a.Shift(xy); return std::move(a);}
    Contour& SwapXY() {boundingBox.SwapXY(); for (unsigned i=0; i<size(); i++) at(i).SwapXY(); *this = CreateInverse(); return *this;}
    void VerticalCrossSection(double x, DoubleMap<bool> &section) const {DoVerticalCrossSection(x, section, true);}

    ContourList CreateExpand(double gap, EExpandType type=EXPAND_MITER) const;
    Contour CreateWithLastEdge(unsigned i) const;
    void Path(cairo_t *cr, bool inverse=false) const;
    void PathOpen(cairo_t *cr) const;
    double OffsetBelow(const Contour &below, double &touchpoint, double offset=CONTOUR_INFINITY) const;
};

inline bool Contour::operator <(const Contour &b) const
{
    if (boundingBox != b.boundingBox) return boundingBox < b.boundingBox;
    if (size() != b.size()) return size() < b.size();
    for (unsigned i=0; i<size(); i++)
        if (at(i) != b[i]) return at(i)<b[i];
    return false; //equal
}

inline bool Contour::operator ==(const Contour &b) const
{
    if (boundingBox != b.boundingBox || size() != b.size()) return false;
    for (unsigned i=0; i<size(); i++)
        if (at(i) != b[i]) return false;
    return true; //equal
}

inline double Contour::OffsetBelow(const Contour &below, double &touchpoint, double offset) const
{
    if (offset < below.boundingBox.y.from - boundingBox.y.till) return offset;
    if (!boundingBox.x.Overlaps(below.boundingBox.x)) return offset;
    return do_offsetbelow(below, touchpoint);
}

#endif //CONTOUR_CONTOUR_H
