#if !defined(CONTOUR_CONTOUR_H)
#define CONTOUR_CONTOUR_H

#include <vector>
#include <list>
#include "contour_edge.h"

class Contour;

//inside of contour is to the right as we go through: counterclockwise pointlists contain the "outside"
//never degenerate - always has actual area
//never flopped - edges never cross each-other
//always contagious - one single surface
//always goes in clockwise direction - except internally
class SimpleContour : protected std::vector<Edge>
{
    friend class Contour;
protected:
    typedef enum {OVERLAP, A_IS_EMPTY, B_IS_EMPTY, BOTH_EMPTY, A_INSIDE_B, B_INSIDE_A, SAME, APART} result_t;
private:
    Block  boundingBox;
    bool   clockwise;

    result_t CheckContainmentHelper(const SimpleContour &b) const;
    double do_offsetbelow(const SimpleContour &below, double &touchpoint) const;

    SimpleContour CreateWithLastEdge(unsigned i) const;
    const Block &CalculateBoundingBox();

    friend class ContoursHelper; //For walk related memebers
    void CalculateClockwise();
    void AppendDuringWalk(const Edge &);
    bool PostWalk();
    bool Sanitize();

    int next(int vertex) const {return (vertex+1)%size();}
    int prev(int vertex) const {return (vertex-1+size())%size();}
    Edge       &at_next(int i)       {return at(next(i));}
    const Edge &at_next(int i) const {return at(next(i));}
    Edge       &at_prev(int i)       {return at(prev(i));}
    const Edge &at_prev(int i) const {return at(prev(i));}

    //returns a point on the line of a tangent at "pos", the point being towards the start of curve/edge.
	XY PrevTangentPoint(int edge, double pos) const {return at(edge).PrevTangentPoint(pos, at_prev(edge));}
    //returns a point on the line of a tangent at "pos", the point being towards the end of curve/edge.
    XY NextTangentPoint(int edge, double pos) const {return at(edge).NextTangentPoint(pos, at_next(edge));}

protected:
    friend class ContourList;
    friend struct node;
    SimpleContour() : clockwise(true) {boundingBox.MakeInvalid();}
    SimpleContour(SimpleContour &&p) {swap(p);}
	SimpleContour(const SimpleContour &p) : std::vector<Edge>(p), boundingBox(p.boundingBox), clockwise(p.clockwise) {}
    SimpleContour(double sx, double dx, double sy, double dy) {operator = (Block(sx,dx,sy,dy));}
    SimpleContour(const Block &b) {operator =(b);}
    SimpleContour(XY a, XY b, XY c);
    SimpleContour(double ax, double ay, double bx, double by, double cx, double cy) {operator =(SimpleContour(XY(ax,ay), XY(bx,by), XY(cx,cy)));}
    SimpleContour(const XY &c, double radius_x, double radius_y=0, double tilt_deg=0, double s_deg=0, double d_deg=360);
    SimpleContour &operator =(const Block &b);
    SimpleContour &operator =(SimpleContour &&p) {if (this!=&p) swap(p);  return *this;}
    SimpleContour &operator =(const SimpleContour &p) {if (this!=&p) {if (p.size()) {std::vector<Edge>::operator=(p); boundingBox=p.boundingBox;} else clear();} return *this;}

    bool operator < (const SimpleContour &b) const;
    bool operator ==(const SimpleContour &b) const;
    void swap(SimpleContour &b) {std::vector<Edge>::swap(b); std::swap(boundingBox, b.boundingBox); std::swap(clockwise, b.clockwise);}
    void clear() {std::vector<Edge>::clear(); boundingBox.MakeInvalid();}

    bool AddAnEdge(const Edge &edge);
    void Invert();

    result_t CheckContainment(const SimpleContour &b) const;

    is_within_t IsWithin(XY p, int *edge=NULL, double *pos=NULL, bool strict=true) const;
    void Shift(const XY &xy) {boundingBox.Shift(xy); for (unsigned i=0; i<size(); i++) at(i).Shift(xy);}
    void SwapXY() {_ASSERT(IsSane()); boundingBox.SwapXY(); for (unsigned i=0; i<size(); i++) at(i).SwapXY(); Invert(); clockwise=!clockwise;}
    void Rotate(double cos, double sin, double radian) {for (unsigned i=0; i<size(); i++) at(i).Rotate(cos, sin, radian); CalculateBoundingBox();}
    void RotateAround(const XY&c, double cos, double sin, double radian) {for (unsigned i=0; i<size(); i++) at(i).RotateAround(c, cos, sin, radian); CalculateBoundingBox();}

    void DoVerticalCrossSection(double x, DoubleMap<bool> &section) const;
    double OffsetBelow(const SimpleContour &below, double &touchpoint, double offset) const;
    void Expand(EExpandType type, double gap, Contour &res) const;

public:
    unsigned size() const {return std::vector<Edge>::size();}
    const Block &GetBoundingBox() const {return boundingBox;}
    bool GetClockWise() const {return clockwise;}
    bool IsEmpty() const {return std::vector<Edge>::size()==0;}
    bool IsSane() const;
    const Edge &GetEdge(int edge) const {return at(edge);}
    void Path(cairo_t *cr) const;
    void Path(cairo_t *cr, bool clockwiseonly) const {
        if (clockwise==clockwiseonly) 
            Path(cr);
    }
    void PathOpen(cairo_t *cr) const;
};

inline bool SimpleContour::operator <(const SimpleContour &b) const
{
    if (boundingBox != b.boundingBox) return boundingBox < b.boundingBox;
    if (size() != b.size()) return size() < b.size();
    for (unsigned i=0; i<size(); i++)
        if (at(i) != b[i]) return at(i)<b[i];
    return false; //equal
}

inline bool SimpleContour::operator ==(const SimpleContour &b) const
{
    if (boundingBox != b.boundingBox || size() != b.size()) return false;
    for (unsigned i=0; i<size(); i++)
        if (at(i) != b[i]) return false;
    return true; //equal
}

inline double SimpleContour::OffsetBelow(const SimpleContour &below, double &touchpoint, double offset) const
{
    if (offset < below.boundingBox.y.from - boundingBox.y.till) return offset;
    if (!boundingBox.x.Overlaps(below.boundingBox.x)) return offset;
    return do_offsetbelow(below, touchpoint);
}

#endif //CONTOUR_CONTOUR_H
