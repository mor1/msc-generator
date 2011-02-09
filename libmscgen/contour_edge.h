#if !defined(CONTOUR_EDGE_H)
#define CONTOUR_EDGE_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <map>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "contour_ellipse.h"

//An edge of a contour. (edges are directed)
//We distinguish two types: straight or curvy.
//Curvy edge is a part of an ellipse, we call it tilted if the axes of the ellipse
//is not parallel with the x and y axises. We make a distinction between tilted and
//non-tilted curvy edges, because you need more calculation for curvy ones.
//
//For straight edges we merely store the startpoint ("start"), the endpoint is
//  the startpoint of the next edge
//For non-tilted curvy edges, we store also store the centerpoint of the ellipsis,
//  the two radiuses (x and y axis in radius1 and radius2), the starting and ending
//  radian of the arc (the ending radian can be calculated from the endpoint coming
//  from the next edge, but we cache it here for performance) and also the notion
//  if the curve is clockwise or counterclockwise. (normal or negative in
//  cairo's terms, with the addition that in msc space y grows downward, and I mean
//  clockwiseness in that space).
//For tilted curvy edges we also store the tilt in radians around the centerpoint.
//  We also store the sin and cos of tilt for increased efficiency

//A straight edge, or a section of an ellipse
//For straight edges, we do not store the endpoint, just the start
//(the endpoint is given by the next edge in the contour)
class Edge
{
    friend class Contour;
    friend void test_geo(cairo_t *cr, int x, int y, bool clicked); ///XXX
protected:
    bool    straight;
    XY      start;
    EllipseData ell;
    bool    full_circle;    //if true, s==e (but can take a value)
    double  s, e;           //supposedly between [0..2pi), if s==e, either empty or full circle
    bool    clockwise_arc;  //this is clocwise if y is downwards
    Block   boundingBox;

    //Convert between pos (0..1) and coordinates
    double pos2radian(double r) const;
    double radian2pos(double r) const;
    //helper, checks if r is between s and e
    bool   radianbetween(double r) const;
    //remove the part of an edge before an after point p lying on the edge
    void   removebeforepoint_curvy(const XY &p);
    void   removeafterpoin_curvy(const XY &p);

    //functions for straight edges
    static int crossing_straight_straight(const XY A, const XY B, const XY M, const XY N,
                                          XY *r, double *pos_in_ab, double *pos_in_mn);
public:
	//No constructor sets the bounding Box!!!
    Edge() : straight(true) {}
    explicit Edge(const XY &xy) : start(xy), straight(true) {}
    Edge(const XY &c, double radius_x, double radius_y=0, double tilt_deg=0, double s_deg=0, double d_deg=360);

    //But all methods assume bounding Box is OK!!!
    void Shift(XY wh) {start+=wh; boundingBox.Shift(wh); if (!straight) ell.Shift(wh);}
    void Rotate(double cos, double sin, double radian, const XY&B);
    void RotateAround(const XY&c, double cos, double sin, double radian, const XY&B);
    void SwapXY();

    bool   operator ==(const Edge& p) const;
    bool   operator < (const Edge& p) const;

    bool IsStraight() const {return straight;}
    const XY & GetStart() const {return start;}
    const Block &GetBoundingBox() const {return boundingBox;}
    const EllipseData &GetEllipseData() const {_ASSERT(!straight); return ell;}
    void CopyInverseToMe(const Edge &B, const XY &next);
    void SetEllipseToFull() {_ASSERT(!straight); e=s; full_circle=true; start=ell.Radian2Point(0);}
    bool IsFullCircle() const {_ASSERT(!straight); return full_circle;}
	double GetSpan() const;
    double GetRadianS() const {_ASSERT(!straight); return s;}
    double GetRadianE() const {_ASSERT(!straight); return e;}
    bool GetClockWise() const {_ASSERT(!straight); return clockwise_arc;}

    //Gives the intersecting points of AB and MN
    static int Crossing(const Edge &A, const XY &B, const Edge &M, const XY &N,
                        XY r[], double pos_ab[], double pos_mn[]);
    //Tells at what x pos this->B crosses the horizontal line at y, rets the number of crosses
    int    CrossingVertical(double x, const XY &B, double y[], double pos[], bool forward[]) const;
    //Removes the part of the edge or curve before point p. Assumes p lies on us. pos must match p
    Edge&  SetStart(const XY &p, double pos);
    //Removes the part of the edge or curve after point p. Assumes p lies on us.
    Edge&  SetEnd(const XY &p, bool keep_full_circle);
    //calculates bb without the endpoint (Contour will add that).
    const Block& CalculateBoundingBox(const XY &next);
    //returns a point on the line of a tangent at "pos", the point being towards the start of curve/edge.
    XY     PrevTangentPoint(double pos, const Edge &prev_vertex) const;
    //returns a point on the line of a tangent at "pos", the point being towards the end of curve/edge.
    XY     NextTangentPoint(double pos, const Edge &next_vertex) const;
    //check if next->after is a direct continuation of this->next and (if a curve) combine it into this, if yes
    bool   CheckAndCombine(const Edge &next, const XY &after);
    //assumes cairo position is at start
    void   Path(cairo_t *cr, const XY &next, bool reverse=false) const;

    //Helpers for expand
	bool   ExpandEdge(double gap, const XY&next, Edge &r1, XY &r2) const;
	int    CombineExpandedEdges(const XY&B, const Edge&M, const XY&N, Edge &res, Edge &res_prev) const;
	int    IsOppositeDir(const XY &B, const Edge &M, const XY &N) const;

    //helpers for offsetbelow
    static double offsetbelow_straight_straight(const XY &A, const XY &B, const XY &M, const XY &N, double &touchpoint);
    double offsetbelow_curvy_straight(const XY &A, const XY &B, bool straight_is_up, double &touchpoint) const;
    double offsetbelow_curvy_curvy(const Edge &o, double &touchpoint) const;
};


inline bool Edge::operator ==(const Edge& p) const
{
    if (start!=p.start || straight!=p.straight) return false;
    if (straight) return true;
    if (full_circle != p.full_circle) return false;
    if (s!=p.s || e!=p.e) return false;
    if (clockwise_arc!=p.clockwise_arc) return false;
    return ell==p.ell;
}

inline bool Edge::operator < (const Edge& p) const
{
    if (start!=p.start) return start<p.start;
    if (straight!= p.straight) return straight;
    if (straight) return false; //they are equal
    if (full_circle != p.full_circle) return !full_circle; //full circle is bigger
    if (s!=p.s) return s < p.s;
    if (e!=p.e) return e < p.e;
    if (clockwise_arc!=p.clockwise_arc) return clockwise_arc;
    return ell < p.ell;
}

inline double Edge::GetSpan() const
{
	_ASSERT(!straight);
    if (full_circle) return 2*M_PI;
	if (clockwise_arc) {
		if (s<e) return e-s;
		else return e-s+2*M_PI;
	} else {
		if (s>e) return s-e;
		else return s-e+2*M_PI;
	}
}

inline XY Edge::PrevTangentPoint(double pos, const Edge &prev_vertex) const
{
    //we are at a vertex, or at a crosspoint at a vertex
    if (pos <= 0)
        return prev_vertex.PrevTangentPoint(1, prev_vertex); //second parameter will be ignored
    if (straight)
        return start;
    return ell.Tangent(pos2radian(pos), !clockwise_arc);
}

inline XY Edge::NextTangentPoint(double pos, const Edge &next_vertex) const
{
    if (straight)
        return next_vertex.start;
    //calc tangent for curvy edges
    return ell.Tangent(pos2radian(pos), clockwise_arc);
}

//Removes the part of the edge or curve before point p. Assumes p lies on us.
inline Edge& Edge::SetStart(const XY &p, double pos)
{
    start = p;
    if (straight) return *this;
    const double r = pos2radian(pos);
    _ASSERT(radianbetween(r));
    full_circle = full_circle && test_equal(s,r);
    s = r;
    return *this;
}
//Removes the part of the edge or curve after point p. Assumes p lies on us.
inline Edge& Edge::SetEnd(const XY &p, bool keep_full_circle)
{
    if (straight) return *this;
    const double r = ell.Point2Radian(p);
    //_ASSERT(radianbetween(r));
    full_circle = full_circle && test_equal(e,r) && keep_full_circle;
    e = r;
    return *this;
}

typedef enum {ALL_EQUAL, A_EQUAL_B, A_EQUAL_C, B_EQUAL_C, IN_LINE, CLOCKWISE, COUNTERCLOCKWISE} triangle_dir_t;
triangle_dir_t triangle_dir(XY a, XY b, XY c);
double angle(XY base, XY A, XY B);
inline double angle_degrees(double angle) {
    return (angle>=2) ? 360 - acos(angle-3)*(180./M_PI) : acos(1-angle)*(180./M_PI);
}

template <class element>
class DoubleMap : public std::map<double, element>
{
public:
    using std::map<double, element>::operator[];   //indicate to two-stage (dependent) name lookup
    using std::map<double, element>::upper_bound;  //that we want these names (not dependent on "element")
    using std::map<double, element>::begin;        //looked up only at instantiation time (from base class)
    using std::map<double, element>::end;          //and not during template compilation (searched in global scope).

    DoubleMap() {};
    DoubleMap(const element &e) {insert(typename std::map<double, element>::value_type(-CONTOUR_INFINITY, e));
                                 insert(typename std::map<double, element>::value_type(CONTOUR_INFINITY, e));}
    void Set(double pos, const element&e) {operator[](pos) = e;}
    void Set(const Range &r, const element &e);
    void Add(double pos, const element&e);     //assumes element has operator +=
    void Add(const Range &r, const element&e); //assumes element has operator +=
    const element* Get(double pos) const {auto i=upper_bound(pos); return i==begin()?NULL:&(--i)->second;}
    double Till(double pos) const {auto i=upper_bound(pos); return i==end()?CONTOUR_INFINITY:i->first;}
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

#endif //CONTOUR_EDGE_H
