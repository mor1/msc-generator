#if !defined(CONTOUR_H)
#define CONTOUR_H

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

typedef enum {
    EXPAND_MITER, //continue edges until they meet, if they dont add two segments
    EXPAND_MITER_ROUND, //continue edges until they meet, if they dont add circle
    EXPAND_MITER_BEVEL, //continue edges until they meet, if they dont cut directly
    EXPAND_MITER_SQUARE, //continue edges until they meet, if they dont add square
    EXPAND_ROUND, //add circle
    EXPAND_BEVEL, //cut directly
} EExpandType;

struct RayAngle {
    double angle;  //the false angle [0..4], each integer corresponds to 90 degrees
    double curve;  //the curvature of the angle, 0 is straight
    RayAngle() {};
    explicit RayAngle(double a, double b=0) : angle(a), curve(b) {}
    bool IsSimilar(const RayAngle&o) const {return test_equal(angle, o.angle) && test_equal(curve, o.curve);}
    bool Smaller(const RayAngle&o) const {return test_equal(angle, o.angle) ? test_smaller(curve, o.curve) : angle < o.angle;}
};

class Edge
{
    friend class SimpleContour;
    friend class ContoursHelper;
public: 
    typedef enum {STRAIGHT=0, FULL_CIRCLE=1, ARC=2} EEdgeType;
    typedef enum {DEGENERATE, SAME_ELLIPSIS,
                  CP_REAL, CP_EXTENDED,
                  NO_CP_CONVERGE, NO_CP_PARALLEL} EExpandCPType;
    static bool HasCP(EExpandCPType t) {return t==CP_REAL || t==CP_EXTENDED;}

protected:
    EEdgeType   type;
    XY          start;
    XY          end;
    double      s;
    double      e;           //supposedly between [0..2pi), if s==e, either empty or full circle
    bool        clockwise_arc;  //this is clocwise if y is downwards
    EllipseData ell;
    Block       boundingBox;

public:
    mutable bool visible;
    Edge() : type(STRAIGHT), visible(true) {boundingBox.MakeInvalid();}
    Edge(const XY &s, const XY &e) : type(STRAIGHT), start(s), end(e), boundingBox(s, e), visible(true) {}
    Edge(const XY &c, double radius_x, double radius_y=0, double tilt_deg=0, double s_deg=0, double d_deg=360);

    EEdgeType GetType() const {return type;}
    const XY & GetStart() const {return start;}
    const XY & GetEnd() const {return end;}
    const Block &GetBoundingBox() const {return boundingBox;}

    const EllipseData &GetEllipseData() const {_ASSERT(type!=STRAIGHT); return ell;}
    bool GetClockWise() const {_ASSERT(type!=STRAIGHT); return clockwise_arc;}
    double GetSpan() const;
    double GetRadianE() const {_ASSERT(type!=STRAIGHT); return type==ARC ? e : s;}
    double GetRadianS() const {_ASSERT(type!=STRAIGHT); return s;}
    double GetRadianMidPoint() const;
    XY Pos2Point(double pos) const;
    void SetFullCircle() {_ASSERT(type!=STRAIGHT); type = FULL_CIRCLE; end=start; e=s; CalculateBoundingBox();}
    
    bool IsSane() const;
    bool IsSaneNoBoundingBox() const;
    //This should return the (directed) area between the y axis and the edge times 2, minus start.x*start.y, plus end.x*end.y
    double GetAreaAboveAdjusted() const {return type==STRAIGHT ? start.x*end.y-start.y*end.x : getAreaAboveAdjusted_curvy();}
    double GetLength() const {return type==STRAIGHT ? (end-start).length() : type == FULL_CIRCLE ? ell.FullCircumference() : ell.SectorCircumference(s,e);}

    bool Expand(double gap);

    void Shift(const XY &wh) {start+=wh; end+=wh; boundingBox.Shift(wh); if (type!=STRAIGHT) ell.Shift(wh);}
    void Rotate(double cos, double sin, double radian);
    void RotateAround(const XY&c, double cos, double sin, double radian);
    void SwapXY();

    bool operator ==(const Edge& p) const {return start==p.start && end==p.end && type==p.type && (type==STRAIGHT || equal_curvy(p));}
    bool operator < (const Edge& p) const {return start!=p.start ? start<p.start : end!=p.end ? end<p.end : type!=p.type ? type<p.type : type==STRAIGHT ? false : smaller_curvy(p);}

    void Invert() {std::swap(start, end); if (type!=STRAIGHT) {clockwise_arc = !clockwise_arc; std::swap(s, e);}}
    RayAngle Angle(bool incoming, const XY &p, double pos) const {return type==STRAIGHT ? RayAngle(incoming ? angle(end, XY(end.x+100, end.y), start) : angle(start, XY(start.x+100, start.y), end)) : angle_curvy(incoming, p, pos);}

    //Gives the intersecting points of me and another straight edge
    unsigned Crossing(const Edge &A, XY r[], double pos_my[], double pos_other[]) const;
    //Tells at what x pos this edge crosses the horizontal line at y, rets the number of crosses
    int CrossingVertical(double x, double y[], double pos[], bool forward[]) const;
    //calculates bb
    const Block& CalculateBoundingBox() {boundingBox.MakeInvalid(); boundingBox += start; boundingBox += end; if (type!=STRAIGHT) calcbb_curvy(); return boundingBox;}

    //assumes cairo position is at start (or at end if reverse is true)
    void   PathTo(cairo_t *cr) const {if (type==STRAIGHT) cairo_line_to(cr, end.x, end.y); else pathto_curvy(cr);}
    void   PathDashed(cairo_t *cr, const double pattern[], unsigned num, int &pos, double &offset, bool reverse=false) const;

    //helpers for offsetbelow
    double OffsetBelow(const Edge &M, double &touchpoint) const;

protected:
    bool equal_curvy(const Edge &p) const;
    bool smaller_curvy(const Edge &p) const;
    void pathto_curvy(cairo_t *cr) const;
    void calcbb_curvy();
    RayAngle angle_curvy(bool incoming, const XY &p, double pos) const;

    unsigned CrossingStraightStraight(const Edge &A, XY r[], double pos_my[], double pos_other[]) const;

    //Convert between pos (0..1) and coordinates
    double pos2radian_full_circle(double r) const {return fmod_negative_safe(clockwise_arc ? s+r*2*M_PI : s-r*2*M_PI, 2*M_PI);}
    double radian2pos_full_circle(double r) const {return fmod_negative_safe(clockwise_arc ? (r-s)/(2*M_PI) : (s-r)/(2*M_PI), 1.);}

    //Convert between pos (0..1) and coordinates
    double pos2radian(double r) const;
    double radian2pos(double r) const;
    //helper, checks if r is between s and e
    bool   radianbetween(double r) const;
    //remove the part of an edge before an after point p lying on the edge
    void   removebeforepoint_curvy(const XY &p);
    void   removeafterpoin_curvy(const XY &p);
    double getAreaAboveAdjusted_curvy() const;
    double offsetbelow_curvy_straight(const Edge &M_straight, bool straight_is_up, double &touchpoint) const;

    double FindRadianOfClosestPos(unsigned num, double pos[], double rad);
    bool UpdateClockWise(double new_s, double new_e);
    void SwapXYcurvy();
    int CrossingVerticalCurvy(double x, double y[], double pos[], bool forward[]) const;

    //returns a point on the line of a tangent at "pos", the point being towards the start of curve/edge.
    XY     PrevTangentPoint(double pos) const;
    //returns a point on the line of a tangent at "pos", the point being towards the end of curve/edge.
    XY     NextTangentPoint(double pos) const;

    //Removes the part of the edge before point p. Assumes p lies on us. Invalidates BoundingBox!!!
    Edge& SetStartStrict(const XY &p, double pos, bool keep_full_circle=false);
    Edge& SetStartLiberal(const XY &p, bool keep_full_circle=false);
    //Removes the part of the edge after point p. Assumes p lies on us. Invalidates BoundingBox!!!
    Edge& SetEndLiberal(const XY &p, bool keep_full_circle=false);
    //check if "next" is a direct continuation of "this". If yes combine "next" into "this" and return true
    bool   CheckAndCombine(const Edge &next);
    //Helpers for expand
    EExpandCPType FindExpandedEdgesCP(const Edge &M, XY &newcp) const;
    void SetStartEndForExpand(const XY &S, const XY &E);
    bool IsOpposite(const XY &S, const XY &E) const;
};

inline double Edge::GetSpan() const
{
    _ASSERT(type!=STRAIGHT);
    if (type==FULL_CIRCLE) return 2*M_PI;
    if (clockwise_arc) {
        if (s<e) return e-s;
        else return e-s+2*M_PI;
    } else {
        if (s>e) return s-e;
        else return s-e+2*M_PI;
    }
}

inline XY Edge::PrevTangentPoint(double pos) const
{
    if (type!=STRAIGHT)
        return ell.Tangent(pos2radian(pos), !clockwise_arc);
    if (pos<=0.5)
        return start*2-end;
    return start;
}

inline XY Edge::NextTangentPoint(double pos) const
{
    if (type!=STRAIGHT)
        return ell.Tangent(pos2radian(pos), clockwise_arc);
    if (pos>0.5)
        return end*2-start;
    return end;
}

//Removes the part of the edge or curve after point p. Assumes p lies on us.
inline Edge& Edge::SetEndLiberal(const XY &p, bool keep_full_circle)
{
    end = p;
    if (type==STRAIGHT) {
        CalculateBoundingBox();
        return *this;
    }
    const double r = ell.Point2Radian(p);
    if (type==FULL_CIRCLE && !(test_equal(s,r) && keep_full_circle))
        type = ARC;
    e = r;
    CalculateBoundingBox();
    return *this;
}

} //namespace

#endif //CONTOUR_H
