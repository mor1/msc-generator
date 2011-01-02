#if !defined(GEOMETRY_POLYGON_H)
#define GEOMETRY_POLYGON_H

#include <vector>
#include <list>
#include "geometry_edge.h"

namespace geometry {

class ContourList;
struct CPOnEdge;

//inside of contour is to the right as we go through: counterclockwise pointlists contain the "outside"
//never degenerate - always has actual area
//never flopped - edges never cross each-other
//always contagious - one single surface
//always goes in clockwise direction - except internally
class Contour : protected std::vector<Edge>
{
    friend void test_geo(cairo_t *cr, int x, int y, bool clicked); ///XXX
    friend class Ray;
    friend class CPsByContour;
	friend class CPSet;
    friend class ContourList; //to access the (std::vector<Edge> &&) constructor & GetInverse
	friend class node_list;   //to access CheckContainment
    explicit Contour(std::vector<Edge> &&v) {std::vector<Edge>::swap(v);} //leave boundingBox!!
    explicit Contour(const std::vector<Edge> &v) : std::vector<Edge>(v) {} //leave boundingBox!!
protected:
    Block  boundingBox;

    Contour() {boundingBox.MakeInvalid();}
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
	void Walk4Combine(const CPOnEdge &startpoint);
	static void Walk4Untangle(const CPOnEdge &startpoint, ContourList &surfaces, ContourList &holes);
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

	void Path(cairo_t *cr, bool inverse=false) const;
public:
    Contour(Contour &&p) {swap(p);}
	Contour(const Contour &p) : std::vector<Edge>(p), boundingBox(p.boundingBox) {}
    Contour(double sx, double dx, double sy, double dy) {operator = (Block(sx,dx,sy,dy));}
    Contour(const Block &b) {operator =(b);}
    Contour(XY a, XY b, XY c);
    Contour(double ax, double ay, double bx, double by, double cx, double cy);
    Contour(const XY &c, double radius_x, double radius_y=0, double tilt_degree=0);
    Contour &operator =(const Block &b);

    bool operator < (const Contour &b) const;
    bool operator ==(const Contour &b) const;
    Contour &operator=(Contour &&p) {swap(p);  return *this;}
	Contour &operator=(const Contour &p) {std::vector<Edge>::operator=(p); boundingBox=p.boundingBox; return *this;}

    //returns a point on the line of a tangent at "pos", the point being towards the start of curve/edge.
	XY PrevTangentPoint(int edge, double pos) const {return at(edge).PrevTangentPoint(pos, at_prev(edge));}
    //returns a point on the line of a tangent at "pos", the point being towards the end of curve/edge.
    XY NextTangentPoint(int edge, double pos) const {return at(edge).NextTangentPoint(pos, at_next(edge));}

    const Block &GetBoundingBox() const {return boundingBox;}
    const Block &CalculateBoundingBox();
    void swap(Contour &b) {std::vector<Edge>::swap(b); std::swap(boundingBox, b.boundingBox);}
    void clear() {std::vector<Edge>::clear(); boundingBox.MakeInvalid();}
	int  size() const {return std::vector<Edge>::size();}
	const Edge &GetEdge(int edge) const {return at(edge);}

    void Shift(XY xy) {boundingBox.Shift(xy); for (int i=0; i<size(); i++) at(i).Shift(xy);}
    void Rotate(double cos, double sin, double radian);
    void RotateAround(const XY&c, double cos, double sin, double radian);
    void Rotate(double degrees) {double r=deg2rad(degrees); Rotate(cos(r), sin(r), r);}
    void RotateAround(const XY&c, double degrees) {double r=deg2rad(degrees); RotateAround(c, cos(r), sin(r), r);}

    void Expand(double gap, ContourList &res) const;

    double OffsetBelow(const Contour &below) const;
};

inline bool Contour::operator <(const Contour &b) const
{
    if (boundingBox != b.boundingBox) return boundingBox < b.boundingBox;
    if (size() != b.size()) return size() < b.size();
    for (int i=0; i<size(); i++) if (at(i) != b[i]) return at(i)<b[i];
    return false; //equal
}

inline bool Contour::operator ==(const Contour &b) const
{
    if (boundingBox != b.boundingBox || size() != b.size()) return false;
    for (int i=0; i<size(); i++) if (at(i) != b[i]) false;
    return true; //equal
}

}; //namespace
#endif //GEOMETRY_POLYGON_H
