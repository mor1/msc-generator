#if !defined(GEOMETRY_EDGE_H)
#define GEOMETRY_EDGE_H

#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "cairo.h" 
#include "geometry_basics.h"

//An edge of a polygon. (edges are directed) 
//We distinguish three types: straight or curvy and tilted curvy.
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

namespace geometry {

class PolyEdge
{
protected:
	friend class MscPolygon;
	friend void test_geo(cairo_t *cr, int x, int y, bool clicked); ///XXX
	//these must be called only if we are sure we are of the right type
	//for tilted ellipses translate from msc space to a space, where
	//  the ellipse is a non-tilted unit circle
	XY         conv_to_circle_space(const XY &p) const;
	XY         conv_to_real_space(const XY &p) const;
	//Convert between pos (0..1) and coordinates
	XY         pos2point_straight(double r, const XY &B) const;
	double     pos2radian(double r) const;
	XY         pos2point_curvy(double r) const;
	double     radian2pos(double r) const;
	double     point2pos_curvy(XY p, is_within_t *inout=NULL) const;
	//helper for curvy or tilted
	bool       radianbetween(double r) const;
	//crossing_X_Y: says if two edges have common points or not
	//return the number of common points, coordinates in "r"
	//also the relative position inside the edge in "pos_X" between [0..1]
	static int crossing_straight_straight(const XY A, const XY B, const XY M, const XY N, 
		                                  XY *r, double *pos_in_ab, double *pos_in_mn);
	       int crossing_curvy_straight(const XY &A, const XY &B, 
			                           XY *r, double *pos_us, double *pos_b) const;
	       int crossing_curvy_curvy(const PolyEdge &B, 
		                            XY *r, double *pos_us, double *pos_b) const;
	//calculate one point on the tangent at pos (between 0..1) either in the
	//  forward direction (next==true) or in the backward direction
	XY         prevnexttangent_curvy(double pos, bool next) const;
	//remove the part of an edge before an after point p lying on the edge
	void removebeforepoint_curvy(const XY &p);
	void removeafterpoint_curvy(const XY &p);

	void transpose_curvy_non_tilted();

public:
	typedef enum {STRAIGHT, CURVY_NON_TILTED, CURVY_TILTED} edge_t;
	edge_t type;
	XY     start;
	XY     center;
	double s, e;   //supposedly between [0..2pi]
	bool   clockwise_arc;  //this is clocwise if y is downwards
	double radius1, radius2;
	double tilt;   //supposedly between [0..pi/2)
	double sintilt, costilt; //pre-computed values
	Block  boundingBox;      //used only for curvy edge

	PolyEdge() : type(STRAIGHT) {}
	explicit PolyEdge(const XY &xy) : start(xy), type(STRAIGHT) {}
	PolyEdge(const XY &c, double radius_x, double radius_y=0, double tilt_degree=0);

	PolyEdge &   operator +=(XY wh) {start+=wh; if (type!=STRAIGHT) center+=wh; return *this;}
	PolyEdge &   operator -=(XY wh) {start-=wh; if (type!=STRAIGHT) center-=wh; return *this;}
	bool   operator ==(const PolyEdge& p) const; 
	bool   operator < (const PolyEdge& p) const; 

	//Gives the intersecting points of AB and MN
	static int Crossing(const PolyEdge &A, const XY &B, const PolyEdge &M, const XY &N, 
		                XY *r, double *pos_ab, double *pos_mn);
	//Tells at what x pos this->B crosses the horizontal line at y, rets the number of crosses
	int        CrossingHorizontal(double y, const XY &B, double *r) const;
	//Removes the part of the edge or curve before point p. Assumes p lies on us.
	PolyEdge&  RemoveBeforePoint(const XY &p);
	//Removes the part of the edge or curve after point p. Assumes p lies on us.
	PolyEdge&  RemoveAfterPoint(const XY &p);
	//Adds this point (for straight) or full curve to a bounding box
	Block&     CalculateBoundingBox();
	//returns a point on the line of a tangent at "pos", the point being towards the start of curve/edge.
	XY         PrevTangentPoint(double pos, const PolyEdge &prev_vertex) const;
	//returns a point on the line of a tangent at "pos", the point being towards the end of curve/edge.
	XY         NextTangentPoint(double pos, const PolyEdge &next_vertex) const;
	//check if next->after is a direct continuation of this->next and (if a curve) combine it into this, if yes
	bool       CheckAndCombine(const PolyEdge &next, const XY &after);
	//assumes cairo position is at start
	void       Path(cairo_t *cr, const XY &next) const;
};

inline bool PolyEdge::operator ==(const PolyEdge& p) const 
{
	if (start!=p.start || type!=p.type) return false;
	if (type==STRAIGHT || p.type==STRAIGHT) return true;
	if (s!=p.s && e!=p.e && radius2!=p.radius2 && clockwise_arc!=p.clockwise_arc) return false;
	if (type==CURVY_NON_TILTED || p.type==CURVY_NON_TILTED) return true;
	return tilt==p.tilt;
}

inline bool PolyEdge::operator < (const PolyEdge& p) const  
{
	if (start!=p.start) return start<p.start;
	if (type != p.type) return (int)type < (int)p.type;
	if (type==STRAIGHT) return false; //both are straight, they are equal
	if (radius1!=p.radius1) return radius1 < p.radius1;
	if (radius2!=p.radius2) return radius2 < p.radius2;
	if (s!=p.s) return s < p.s;
	if (e!=p.e) return e < p.e;
	if (clockwise_arc!=p.clockwise_arc) return clockwise_arc;
	if (type==CURVY_NON_TILTED) return false; //both are curvy, they are equal
	return tilt < p.tilt;
}



inline int PolyEdge::Crossing(const PolyEdge &A, const XY &B, const PolyEdge &M, const XY &N, 
	                          XY *r, double *pos_ab, double *pos_mn) 
{
	if (A.type == STRAIGHT && M.type == STRAIGHT) 
		return crossing_straight_straight(A.start, B, M.start, N, r, pos_ab, pos_mn);
	if (A.type != STRAIGHT && M.type != STRAIGHT) 
		return A.crossing_curvy_curvy(M, r, pos_ab, pos_mn);
	if (A.type == STRAIGHT) 
		return M.crossing_curvy_straight(A.start, B, r, pos_mn, pos_ab);
	/* (M.type == STRAIGHT*/ 
	    return A.crossing_curvy_straight(M.start, N, r, pos_ab, pos_mn);
}

inline XY PolyEdge::PrevTangentPoint(double pos, const PolyEdge &prev_vertex) const
{
	//we are at a vertex, or at a crosspoint at a vertex
	if (pos <= 0) 
		return prev_vertex.PrevTangentPoint(1, prev_vertex); //second parameter will be ignored
	if (type == STRAIGHT) 
		return start;
	return prevnexttangent_curvy(pos, false);
}

inline XY PolyEdge::NextTangentPoint(double pos, const PolyEdge &next_vertex) const
{
	if (type == STRAIGHT) 
		return next_vertex.start;
	//calc tangent for curvy edges
	return prevnexttangent_curvy(pos, true);
}

//Removes the part of the edge or curve before point p. Assumes p lies on us.
inline PolyEdge& PolyEdge::RemoveBeforePoint(const XY &p)
{
	start = p;
	if (type!=STRAIGHT) 
		removebeforepoint_curvy(p);
	return *this;
}
//Removes the part of the edge or curve after point p. Assumes p lies on us.
inline PolyEdge& PolyEdge::RemoveAfterPoint(const XY &p)
{
	if (type!=STRAIGHT) 
		removeafterpoint_curvy(p);
	return *this;
}

inline void PolyEdge::Path(cairo_t *cr, const XY &next) const
{
	if (type==STRAIGHT) {
		cairo_line_to(cr, next.x, next.y);
		return;
	}
	cairo_save(cr);
	cairo_translate(cr, center.x, center.y);
	if (type==CURVY_TILTED)
		cairo_rotate(cr, tilt);
	cairo_scale(cr, radius1, radius2);
	if (clockwise_arc)
		cairo_arc(cr, 0, 0, 1, s, e);
	else
		cairo_arc_negative(cr, 0, 0, 1, s, e);
	cairo_restore(cr);
}



typedef enum {ALL_EQUAL, A_EQUAL_B, A_EQUAL_C, B_EQUAL_C, IN_LINE, CLOCKWISE, COUNTERCLOCKWISE} triangle_dir_t;
triangle_dir_t triangle_dir(XY a, XY b, XY c);
double angle(XY base, XY A, XY B);
inline double angle_degrees(double angle) {
	return (angle>=2) ? 360 - acos(angle-3)*(180./M_PI) : acos(1-angle)*(180./M_PI);
}

int quartic_solve(double A, double B, double C, double D, double E, double *r);
int cubic_solve(double C1, double C2, double C3, double C4, double *R);
double cubic_substitute(double C1, double C2, double C3, double C4, double R);


}; //namespace

#endif //GEOMETRY_EDGE_H
