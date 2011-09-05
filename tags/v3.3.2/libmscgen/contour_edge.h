#if !defined(CONTOUR_EDGE_H)
#define CONTOUR_EDGE_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <map>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "contour_ellipse.h"

namespace contour {

//TODO: XXX redo comment below
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

typedef enum {ALL_EQUAL, A_EQUAL_B, A_EQUAL_C, B_EQUAL_C, IN_LINE, CLOCKWISE, COUNTERCLOCKWISE} triangle_dir_t;
triangle_dir_t triangle_dir(XY a, XY b, XY c);
double angle(XY base, XY A, XY B);
inline double angle_degrees(double angle) {
    return (angle>=2) ? 360 - acos(angle-3)*(180./M_PI) : acos(1-angle)*(180./M_PI);
}

typedef enum {EDGE_STRAIGHT, EDGE_FULL_CIRCLE, EDGE_ARC} EEdgeType;
typedef enum {EXPAND_MITER, EXPAND_ROUND, EXPAND_BEVEL, EXPAND_MITER_ROUND, EXPAND_MITER_BEVEL, EXPAND_MITER_SQUARE} EExpandType;

struct RayAngle {
    double angle;  //the false angle [0..4], each integer corresponds to 90 degrees
    double curve;  //the curvature of the angle, 0 is straight
    RayAngle() {};
    explicit RayAngle(double a, double b=0) : angle(a), curve(b) {}
    bool IsSimilar(const RayAngle&o) const {return test_equal(angle, o.angle) && test_equal(curve, o.curve);}
    bool Smaller(const RayAngle&o) const {return test_equal(angle, o.angle) ? test_smaller(curve, o.curve) : angle < o.angle;}
};

class EdgeStraight
{
    friend class SimpleContour;
    friend class EdgeArc;
    friend void test_geo(cairo_t *cr, int x, int y, bool clicked); ///TODO: remove
protected:
    EEdgeType type;
    XY        start;
    XY        end;
    Block     boundingBox;

    EdgeStraight() : type(EDGE_STRAIGHT) {boundingBox.MakeInvalid();}
    EdgeStraight(const XY &s, const XY &e) : type(EDGE_STRAIGHT), start(s), end(e), boundingBox(s, e) {}
    void Shift(XY wh) {start+=wh; end+=wh; boundingBox.Shift(wh);}
    void Rotate(double cos, double sin, double radian);
    void RotateAround(const XY&c, double cos, double sin, double radian);
    void SwapXY();

    bool operator ==(const EdgeStraight& p) const {return type==p.type && start==p.start && end==p.end;}
    bool operator < (const EdgeStraight& p) const {return type!=p.type ? type<p.type : start==p.start ? end<p.end : start<p.start;}

    void Invert() {std::swap(start, end);}
    RayAngle Angle(bool incoming) const {return RayAngle(incoming ? angle(end, XY(end.x+100, end.y), start) : angle(start, XY(start.x+100, start.y), end));}

    //Gives the intersecting points of me and another straight edge
    unsigned Crossing(const EdgeStraight &A, XY r[], double pos_my[], double pos_other[]) const;
    //Tells at what x pos this edge crosses the horizontal line at y, rets the number of crosses
    int CrossingVertical(double x, double y[], double pos[], bool forward[]) const;
    //calculates bb
    const Block& CalculateBoundingBox() {boundingBox.MakeInvalid(); boundingBox += start; boundingBox += end; return boundingBox;}
    //check if "next" is a direct continuation of "this". If yes combine "next" into "this" and return true
    bool   CheckAndCombine(const EdgeStraight &next);
    //assumes cairo position is at start (or at end if reverse is true)
    void   PathTo(cairo_t *cr) const {cairo_line_to(cr, end.x, end.y);}
    void   PathDashed(cairo_t *cr, const double pattern[], unsigned num, int &pos, double &offset, bool reverse=false) const;

    //helpers for offsetbelow
    double OffsetBelow(const EdgeStraight &M, double &touchpoint) const;
public:
    //Only those in public, which will not be upgraded
    EEdgeType GetType() const {return type;}
    const XY & GetStart() const {return start;}
    const XY & GetEnd() const {return end;}
    const Block &GetBoundingBox() const {return boundingBox;}
};

class EdgeFullCircle : public EdgeStraight {
protected:
    EllipseData ell;
    double      s;
    bool        clockwise_arc;  //this is clocwise if y is downwards

    EdgeFullCircle() : EdgeStraight() {};
    EdgeFullCircle(const XY &s, const XY &e) : EdgeStraight(s, e) {}
    EdgeFullCircle(const XY &c, double radius_x, double radius_y=0, double tilt_deg=0, double s_deg=0);

    //Convert between pos (0..1) and coordinates
    double pos2radian(double r) const {return fmod_negative_safe(clockwise_arc ? s+r*2*M_PI : s-r*2*M_PI, 2*M_PI);}
    double radian2pos(double r) const {return fmod_negative_safe(clockwise_arc ? (r-s)/(2*M_PI) : (s-r)/(2*M_PI), 1.);}

    void Rotate(double cos, double sin, double radian);
    void RotateAround(const XY&c, double cos, double sin, double radian);
    void Invert() {EdgeStraight::Invert(); clockwise_arc = !clockwise_arc;}

    bool operator ==(const EdgeFullCircle& p) const;
    bool operator < (const EdgeFullCircle& p) const;

    //Gives the intersecting points of me and another straight edge
    unsigned Crossing(const EdgeFullCircle &A, XY r[], double pos_my[], double pos_other[]) const;
    //Tells at what x pos this edge crosses the horizontal line at y, rets the number of crosses
    int CrossingVertical(double x, double y[], double pos[], bool forward[]) const;

    //calculates bb
    const Block& CalculateBoundingBox();
    //assumes cairo position is at start (or at end if reverse is true)
    void   PathTo(cairo_t *cr) const;
public:
    const EllipseData &GetEllipseData() const {_ASSERT(type!=EDGE_STRAIGHT); return ell;}
    bool GetClockWise() const {_ASSERT(type!=EDGE_STRAIGHT); return clockwise_arc;}
    double GetRadianS() const {_ASSERT(type!=EDGE_STRAIGHT); return s;}
    void Shift(XY wh) {EdgeStraight::Shift(wh); if (type!=EDGE_STRAIGHT) ell.Shift(wh);}
};

class EdgeArc : public EdgeFullCircle {
public:
    typedef enum {DEGENERATE, SAME_ELLIPSIS,
                  CP_REAL, CP_EXTENDED,
                  NO_CP_CONVERGE, NO_CP_PARALLEL} EExpandCPType;
    static bool HasCP(EExpandCPType t) {return t==CP_REAL || t==CP_EXTENDED;}
    mutable bool visible;

protected:
    friend class SimpleContour;
    friend class ContoursHelper;
    double  e;           //supposedly between [0..2pi), if s==e, either empty or full circle

    //Convert between pos (0..1) and coordinates
    double pos2radian(double r) const;
    double radian2pos(double r) const;
    //helper, checks if r is between s and e
    bool   radianbetween(double r) const;
    //remove the part of an edge before an after point p lying on the edge
    void   removebeforepoint_curvy(const XY &p);
    void   removeafterpoin_curvy(const XY &p);
    double offsetbelow_curvy_straight(const EdgeStraight &M, bool straight_is_up, double &touchpoint) const;

    double FindRadianOfClosestPos(unsigned num, double pos[], double rad);
    bool UpdateClockWise(double new_s, double new_e);
    void SwapXYcurvy();
    int CrossingVerticalCurvy(double x, double y[], double pos[], bool forward[]) const;
    void PathDashedCurvy(cairo_t *cr, const double pattern[], unsigned num, int &pos, double &offset, bool reverse=false) const;

    void Rotate(double cos, double sin, double radian);
    void RotateAround(const XY&c, double cos, double sin, double radian);
    void SwapXY() {EdgeStraight::SwapXY(); if (type!=EDGE_STRAIGHT) SwapXYcurvy();}
    void Invert() {EdgeFullCircle::Invert(); if (type==EDGE_ARC) std::swap(s, e);}
    RayAngle Angle(bool incoming, const XY &p, double pos) const;
    void SetFullCircle() {_ASSERT(type!=EDGE_STRAIGHT); type = EDGE_FULL_CIRCLE; end=start; e=s; CalculateBoundingBox();}

    //returns a point on the line of a tangent at "pos", the point being towards the start of curve/edge.
    XY     PrevTangentPoint(double pos) const;
    //returns a point on the line of a tangent at "pos", the point being towards the end of curve/edge.
    XY     NextTangentPoint(double pos) const;

    //Removes the part of the edge before point p. Assumes p lies on us. Invalidates BoundingBox!!!
    EdgeArc& SetStartStrict(const XY &p, double pos, bool keep_full_circle=false);
    EdgeArc& SetStartLiberal(const XY &p, bool keep_full_circle=false);
    //Removes the part of the edge after point p. Assumes p lies on us. Invalidates BoundingBox!!!
    EdgeArc& SetEndLiberal(const XY &p, bool keep_full_circle=false);
    //calculates bb
    const Block& CalculateBoundingBox();
    //check if "next" is a direct continuation of "this". If yes combine "next" into "this" and return true
    bool   CheckAndCombine(const EdgeArc &next);
    void   PathToCurvy(cairo_t *cr) const;
    //Helpers for expand
    EExpandCPType FindExpandedEdgesCP(const EdgeArc&M, XY &newcp) const;
    void SetStartEndForExpand(const XY &S, const XY &E);
    bool IsOpposite(const XY &S, const XY &E) const;

    //Helper for offsetbelow
    double OffsetBelow(const EdgeArc &M, double &touchpoint) const;


public:
    EdgeArc() : EdgeFullCircle(), visible(true) {};
    EdgeArc(const XY &s, const XY &e) : EdgeFullCircle(s, e), visible(true) {}
    EdgeArc(const XY &c, double radius_x, double radius_y=0, double tilt_deg=0, double s_deg=0, double d_deg=360);
    bool operator ==(const EdgeArc& p) const;
    bool operator < (const EdgeArc& p) const;
    bool IsSane() const;
    bool IsSaneNoBoundingBox() const;

    double GetSpan() const;
    double GetRadianE() const {_ASSERT(type!=EDGE_STRAIGHT); return type==EDGE_ARC ? e : s;}
    double GetRadianMidPoint() const;
    XY Pos2Point(double pos) const;

    bool Expand(double gap);
    //Gives the intersecting points of me and another straight edge
    unsigned Crossing(const EdgeArc &o, XY r[], double pos_my[], double pos_other[]) const;
    //Tells at what x pos this edge crosses the horizontal line at y, rets the number of crosses
    int CrossingVertical(double x, double y[], double pos[], bool forward[]) const {
        if (type == EDGE_STRAIGHT) return EdgeStraight::CrossingVertical(x, y, pos, forward);
        if (test_smaller(x, boundingBox.x.from) || test_smaller(boundingBox.x.till, x)) return 0;
        return CrossingVerticalCurvy(x, y, pos, forward);
    }
    void   PathTo(cairo_t *cr) const
        {if (type==EDGE_STRAIGHT) EdgeStraight::PathTo(cr);
        else if (type==EDGE_FULL_CIRCLE) EdgeFullCircle::PathTo(cr); else PathToCurvy(cr);}
    void   PathDashed(cairo_t *cr, const double pattern[], unsigned num, int &pos, double &offset, bool reverse=false) const
        {if (type==EDGE_STRAIGHT) EdgeStraight::PathDashed(cr, pattern, num, pos, offset, reverse);
        else PathDashedCurvy(cr, pattern, num, pos, offset, reverse);}

};

typedef EdgeArc Edge;

inline double EdgeArc::GetSpan() const
{
    _ASSERT(type!=EDGE_STRAIGHT);
    if (type==EDGE_FULL_CIRCLE) return 2*M_PI;
    if (clockwise_arc) {
        if (s<e) return e-s;
        else return e-s+2*M_PI;
    } else {
        if (s>e) return s-e;
        else return s-e+2*M_PI;
    }
}

inline XY EdgeArc::PrevTangentPoint(double pos) const
{
    if (type!=EDGE_STRAIGHT)
        return ell.Tangent(pos2radian(pos), !clockwise_arc);
    if (pos<=0.5)
        return start*2-end;
    return start;
}

inline XY EdgeArc::NextTangentPoint(double pos) const
{
    if (type!=EDGE_STRAIGHT)
        return ell.Tangent(pos2radian(pos), clockwise_arc);
    if (pos>0.5)
        return end*2-start;
    return end;
}

//Removes the part of the edge or curve after point p. Assumes p lies on us.
inline EdgeArc& EdgeArc::SetEndLiberal(const XY &p, bool keep_full_circle)
{
    end = p;
    if (type==EDGE_STRAIGHT) {
        EdgeStraight::CalculateBoundingBox();
        return *this;
    }
    const double r = ell.Point2Radian(p);
    if (type==EDGE_FULL_CIRCLE && !(test_equal(s,r) && keep_full_circle))
        type = EDGE_ARC;
    e = r;
    CalculateBoundingBox();
    return *this;
}

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

} //namespace

#endif //CONTOUR_EDGE_H
