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

//helper class for ellipsis intersection calculation
struct quadratic_xy_t;

//Class implementing tilted ellipses, intersection rotation, etc.
//It always considers the full ellipse, not just a section of it
class Ellipse
{
	friend struct quadratic_xy_t;
protected:
	XY     center;
	double radius1, radius2;
	bool tilted;
	double tilt;             //supposedly between [0..pi/2)
	double sintilt, costilt; //pre-computed values

	//for tilted ellipses translate from msc space to a space, where
	//  the ellipse is a non-tilted unit circle
	XY         conv_to_circle_space(const XY &p) const;
	XY         conv_to_real_space(const XY &p) const;
	int refine_crosspoints(int num_y, double y[], const Ellipse &b, 
		                   const quadratic_xy_t &one, const quadratic_xy_t &two, XY p[]) const;
	int refine_crosspoints2(int num_y, double y[], const Ellipse &b, 
		                    const quadratic_xy_t &one, const quadratic_xy_t &two, XY p[]) const;
	bool refine_point1(const Ellipse &B, XY &p) const;

	void transpose_curvy_non_tilted();

public:
	Ellipse() {};
	Ellipse(const XY &c, double radius1, double radius2, double tilt_degree);
	bool operator ==(const Ellipse& p) const;
	bool operator <(const Ellipse& p) const;
	void Shift(XY xy) {center += xy;}
	const XY & GetCenter() const {return center;}
	double GetRadius1() const {return radius1;}
	double GetRadius2() const {return radius2;}
	bool IsTilted() const {return tilted;}

	//calculate one point on the tangent at pos (between 0..1) either in the
	//  forward direction (next==true) or in the backward direction
	XY Tangent(double radian, bool next) const;
	double Point2Radian(const XY &p) const;
	XY Radian2Point(double r) const {return conv_to_real_space(XY(cos(r), sin(r)));}

	//return the number of common points, coordinates in "r"
	//also the radian or relative position inside the straight edge 
	int CrossingEllipse(const Ellipse &B, 
		                XY r[], double radian_us[], double radian_b[]) const;
	int CrossingStraight(const XY &A, const XY &B, 
			             XY r[], double radian_us[], double pos_b[]) const;
	int CrossingHorizontal(double y, double x[], double radian[]) const;

	void TransformForDrawing(cairo_t *cr) const;
};

//A straight edge, or a section of an ellipse
//For straight edges, we do not store the endpoint, just the start
//(the endpoint is given by the next edge in the polygon)
class Edge
{
	friend class Polygon;
	friend void test_geo(cairo_t *cr, int x, int y, bool clicked); ///XXX
protected:
	bool    straight;
	XY      start;
	Ellipse ell;
	double  s, e;   //supposedly between [0..2pi]
	bool    clockwise_arc;  //this is clocwise if y is downwards
	Block   boundingBox;    //only for curvy edge

	//Convert between pos (0..1) and coordinates
	double     pos2radian(double r) const;
	double     radian2pos(double r) const;
	//helper, checks if r is between s and e
	bool       radianbetween(double r) const;
	//remove the part of an edge before an after point p lying on the edge
	void removebeforepoint_curvy(const XY &p);
	void removeafterpoin_curvy(const XY &p);

	//functions for straight edges
	static int crossing_straight_straight(const XY A, const XY B, const XY M, const XY N, 
		                                  XY *r, double *pos_in_ab, double *pos_in_mn);

public:
	Edge() : straight(true) {}
	explicit Edge(const XY &xy) : start(xy), straight(true) {}
	Edge(const XY &c, double radius_x, double radius_y=0, double tilt_degree=0);

	Edge &   Shift(XY wh) {start+=wh; if (!straight) ell.Shift(wh); return *this;}
	bool   operator ==(const Edge& p) const; 
	bool   operator < (const Edge& p) const; 

	bool IsStraight() const {return straight;}
	const XY & GetStart() const {return start;}
	const XY & GetCenter() const {_ASSERT(!straight); return ell.GetCenter();}
	void CopyInverseToMe(const Edge &B, const XY &next);
	void SetEllipseToFull() {_ASSERT(!straight); s=0; e=2*M_PI; start=ell.Radian2Point(0);}

	//Gives the intersecting points of AB and MN
	static int Crossing(const Edge &A, const XY &B, const Edge &M, const XY &N, 
		                XY r[], double pos_ab[], double pos_mn[]);
	//Tells at what x pos this->B crosses the horizontal line at y, rets the number of crosses
	int    CrossingHorizontal(double y, const XY &B, double r[]) const;
	//Removes the part of the edge or curve before point p. Assumes p lies on us. pos must match p
	Edge&  SetStart(const XY &p, double pos);
	//Removes the part of the edge or curve after point p. Assumes p lies on us.
	Edge&  SetEnd(const XY &p);
	//Adds this point (for straight) or full curve to a bounding box
	Block& CalculateBoundingBox();
	//returns a point on the line of a tangent at "pos", the point being towards the start of curve/edge.
	XY     PrevTangentPoint(double pos, const Edge &prev_vertex) const;
	//returns a point on the line of a tangent at "pos", the point being towards the end of curve/edge.
	XY     NextTangentPoint(double pos, const Edge &next_vertex) const;
	//check if next->after is a direct continuation of this->next and (if a curve) combine it into this, if yes
	bool   CheckAndCombine(const Edge &next, const XY &after);
	//assumes cairo position is at start
	void   Path(cairo_t *cr, const XY &next, bool reverse=false) const;
};




inline bool Ellipse::operator ==(const Ellipse& p) const 
{
	if (center!=p.center || tilted!=p.tilted ||
		radius1!=p.radius1 || radius2!=p.radius2 ) return false;
	if (tilted) return tilt==p.tilt;
	return true;
}

inline bool Ellipse::operator < (const Ellipse& p) const  
{
	if (center!=p.center) return center<p.center;
	if (tilted != p.tilted) return tilted;
	if (radius1!=p.radius1) return radius1 < p.radius1;
	if (radius2!=p.radius2) return radius2 < p.radius2;
	if (tilted) return tilt < p.tilt;
	return false; //they are equal
}

inline bool Edge::operator ==(const Edge& p) const 
{
	if (start!=p.start || straight!=p.straight) return false;
	if (straight) return true;
	if (s!=p.s || e!=p.e || clockwise_arc!=p.clockwise_arc) return false;
	return ell==p.ell;
}

inline bool Edge::operator < (const Edge& p) const  
{
	if (start!=p.start) return start<p.start;
	if (straight!= p.straight) return straight;
	if (straight) return false; //they are equal
	if (s!=p.s) return s < p.s;
	if (e!=p.e) return e < p.e;
	if (clockwise_arc!=p.clockwise_arc) return clockwise_arc;
	return ell < p.ell;
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


inline double circle_space_point2radian_curvy(XY p) 
{
	double r = asin(p.y/p.length());     //calc angle normalized to 1
	if (p.x<0) r = M_PI - r;    //between 90 and 270 degrees
	if (r<0) r += 2*M_PI;       //if between -90 and 0 degrees
	return r;
}



//Removes the part of the edge or curve before point p. Assumes p lies on us.
inline Edge& Edge::SetStart(const XY &p, double pos)
{
	start = p;
	if (straight) return *this;
	const double r = pos2radian(pos);
	_ASSERT(radianbetween(r));
	s = r;
	return *this;
}
//Removes the part of the edge or curve after point p. Assumes p lies on us.
inline Edge& Edge::SetEnd(const XY &p)
{
	if (straight) return *this;
	e = ell.Point2Radian(p);
	return *this;
}

inline void Ellipse::TransformForDrawing(cairo_t *cr) const
{
	cairo_translate(cr, center.x, center.y);
	if (tilted)
		cairo_rotate(cr, tilt);
	cairo_scale(cr, radius1, radius2);
}

typedef enum {ALL_EQUAL, A_EQUAL_B, A_EQUAL_C, B_EQUAL_C, IN_LINE, CLOCKWISE, COUNTERCLOCKWISE} triangle_dir_t;
triangle_dir_t triangle_dir(XY a, XY b, XY c);
double angle(XY base, XY A, XY B);
inline double angle_degrees(double angle) {
	return (angle>=2) ? 360 - acos(angle-3)*(180./M_PI) : acos(1-angle)*(180./M_PI);
}

}; //namespace

#endif //GEOMETRY_EDGE_H
