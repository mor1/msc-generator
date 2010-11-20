#if !defined(GEOMETRY_POLYGON_H)
#define GEOMETRY_POLYGON_H

#include <vector>
#include <list>
#include "geometry_edge.h"

namespace geometry {

class PolygonList;
class MscCrossPointStore;

//inside of polygon is to the right as we go through: counterclockwise pointlists contain the "outside"
//never degenerate - always has actual area
//never flopped - edges never cross each-other
//always contagious - one single surface
//always goes in clockwise direction - except internally
class Polygon : protected std::vector<Edge>
{
    friend void test_geo(cairo_t *cr, int x, int y, bool clicked); ///XXX
    friend class MscCrossPointStore;
    friend class PolygonList; //to access the (std::vector<Edge> &&) constructor & GetInverse
    explicit Polygon(std::vector<Edge> &&v) {std::vector<Edge>::swap(v);} //leave boundingBox!!
    explicit Polygon(const std::vector<Edge> &v) : std::vector<Edge>(v) {} //leave boundingBox!!
protected:
    Block  boundingBox;

    Polygon() {boundingBox.MakeInvalid();}
    Polygon GetInverse() const;

public:
    typedef enum {OK, A_IS_EMPTY, B_IS_EMPTY, BOTH_EMPTY, A_INSIDE_B, B_INSIDE_A, SAME, APART} poly_result_t;
    typedef enum {UNION, INTERSECT, SUBSTRACT} poly_action_t;
protected:
    is_within_t   IsWithin(XY p, int *edge=NULL, double *pos=NULL) const;
    poly_result_t CheckContainmentHelper(const Polygon &b) const;
    poly_result_t CheckContainment(const Polygon &b) const;
    bool CalculateClockwise() const;
    static void PolyProcessWalk(MscCrossPointStore *poly, MscCrossPointStore *poly_o, PolygonList &surfaces, PolygonList &holes);
    poly_result_t PolyProcess(const Polygon &b, PolygonList &result, poly_action_t action) const;
    poly_result_t PolyProcessSelf(PolygonList &surfaces, bool add_separate_holes=true, bool check_clockwise=true) const;

    int next(int vertex) const {return (vertex+1)%size();}
    int prev(int vertex) const {return (vertex-1+size())%size();}
    Edge       &at_next(int i)       {return at(next(i));}
    const Edge &at_next(int i) const {return at(next(i));}
    Edge       &at_prev(int i)       {return at(prev(i));}
    const Edge &at_prev(int i) const {return at(prev(i));}
public:
    Polygon(Polygon &&p) {swap(p);}
    Polygon(double sx, double dx, double sy, double dy) {operator = (Block(sx,dx,sy,dy));}
    Polygon(const Block &b) {operator =(b);}
    Polygon(XY a, XY b, XY c);
    Polygon(double ax, double ay, double bx, double by, double cx, double cy);
    Polygon(const XY &c, double radius_x, double radius_y=0, double tilt_degree=0);
    Polygon &operator =(const Block &b);

    bool operator < (const Polygon &b) const;
    bool operator ==(const Polygon &b) const;
    Polygon &operator=(Polygon &&p) {swap(p);  return *this;}

    const Block &GetBoundingBox() const {return boundingBox;}
    const Block &CalculateBoundingBox();
    void swap(Polygon &b) {std::vector<Edge>::swap(b); std::swap(boundingBox, b.boundingBox);}
    void clear() {std::vector<Edge>::clear(); boundingBox.MakeInvalid();}

    void Shift(XY xy) {boundingBox.Shift(xy); for (int i=0; i<size(); i++) at(i).Shift(xy);}
    void Rotate(double cos, double sin, double radian);
    void RotateAround(const XY&c, double cos, double sin, double radian);
    void Rotate(double degrees) {Rotate(cos(degrees*M_PI/180.), sin(degrees*M_PI/180.), degrees*M_PI/180.);}
    void RotateAround(const XY&c, double degrees) {RotateAround(c, cos(degrees*M_PI/180.), sin(degrees*M_PI/180.), degrees*M_PI/180.);}

    bool Overlaps(const Polygon &b) const;
    void Expand(double gap, PolygonList &res) const;
    void Path(cairo_t *cr, bool inverse=false) const;
};

inline bool Polygon::operator <(const Polygon &b) const
{
    if (boundingBox != b.boundingBox) return boundingBox < b.boundingBox;
    if (size() != b.size()) return size() < b.size();
    for (int i=0; i<size(); i++) if (at(i) != b[i]) return at(i)<b[i];
    return false; //equal
}

inline bool Polygon::operator ==(const Polygon &b) const
{
    if (boundingBox != b.boundingBox || size() != b.size()) return false;
    for (int i=0; i<size(); i++) if (at(i) != b[i]) false;
    return true; //equal
}

}; //namespace
#endif //GEOMETRY_POLYGON_H
