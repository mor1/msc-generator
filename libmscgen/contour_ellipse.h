#if !defined(CONTOUR_ELLIPSE_H)
#define CONTOUR_ELLIPSE_H

#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "cairo.h"
#include "contour_basics.h"


//helper class for ellipsis intersection calculation
struct quadratic_xy_t;

//A helper class implementing an ellipse, its crosspoints with ellipses and lines
//it always represents a full ellipse, not just a section of it
class EllipseData
{
    friend struct quadratic_xy_t;
    friend class EdgeFullCircle;
protected:
    XY     center;
    double radius1, radius2;
    bool tilted;
    double tilt;                //supposedly between [0..pi/2)
    double sintilt, costilt;    //pre-computed values
	XY     extreme[4];          //pre-computed left, rightmost, topmost, bottommost point, resp
	double extreme_radian[4]; 

    //for tilted ellipses translate from msc space to a space, where
    //  the ellipse is a non-tilted unit circle
    XY   conv_to_circle_space(const XY &p) const;
    XY   conv_to_real_space(const XY &p) const;
    int  refine_crosspoints(int num_y, double y[], const EllipseData &b,
                            const quadratic_xy_t &one, const quadratic_xy_t &two, XY p[]) const;
    bool refine_point(const EllipseData &B, XY &p) const;
    void transpose_curvy_non_tilted();
	void calculate_extremes();
	double add_to_tilt(double cos, double sin, double radian);

public:
    EllipseData() {};
    EllipseData(const XY &c, double radius1, double radius2=0, double tilt_degree=0);
    bool operator ==(const EllipseData& p) const;
    bool operator <(const EllipseData& p) const;
    bool IsTilted() const {return tilted;}
    const XY & GetCenter() const {return center;}
    double GetRadius1() const {return radius1;}
    double GetRadius2() const {return radius2;}
    double GetTilt() const {return tilted ? tilt : 0;}
	double GetExtreme(int n, XY &xy) const {xy = extreme[n]; return extreme_radian[n];}
    void Shift(const XY &xy);
	double Rotate(double cos, double sin, double radian);
	double RotateAround(const XY&c, double cos, double sin, double radian);
    void SwapXY();

    //calculate one point on the tangent at pos (between 0..1) either in the
    //  forward direction (next==true) or in the backward direction
    XY Tangent(double radian, bool next) const;
    double Point2Radian(const XY &p) const;
    XY Radian2Point(double r) const {return conv_to_real_space(XY(cos(r), sin(r)));}

    //return the number of common points, coordinates in "r"
    //also the radian or relative position inside the straight edge
    int CrossingEllipse(const EllipseData &B,
                        XY r[], double radian_us[], double radian_b[]) const;
    int CrossingStraight(const XY &A, const XY &B,
                         XY r[], double radian_us[], double pos_b[]) const;
    int CrossingVertical(double x, double y[], double radian[]) const;

    int Expand(double gap); //returns -1 if completely disappear, 0 if degrades to line, 1 if OK
	void TransformForDrawing(cairo_t *cr) const;

	//return a positive number if the two can be moved closer together
	//return Infinity() if the two do not overlap in x coordinates
	double OffsetBelow(const EllipseData&) const;
	double OffsetBelow(const XY&A, const XY&B) const;
	double OffsetAbove(const XY&A, const XY&B) const;
};


inline bool EllipseData::operator ==(const EllipseData& p) const
{
    if (center!=p.center || tilted!=p.tilted ||
        radius1!=p.radius1 || radius2!=p.radius2 ) return false;
    if (tilted) return tilt==p.tilt;
    return true;
}

inline bool EllipseData::operator < (const EllipseData& p) const
{
    if (center!=p.center) return center<p.center;
    if (tilted != p.tilted) return tilted;
    if (radius1!=p.radius1) return radius1 < p.radius1;
    if (radius2!=p.radius2) return radius2 < p.radius2;
    if (tilted) return tilt < p.tilt;
    return false; //they are equal
}

inline XY EllipseData::conv_to_real_space(const XY &p) const
{
    if (!tilted)
        return center + XY(radius1 * p.x, radius2 * p.y);
    return center +
        XY(radius1 * p.x * costilt - radius2 * p.y * sintilt,
           radius1 * p.x * sintilt + radius2 * p.y * costilt);
}

inline XY EllipseData::conv_to_circle_space(const XY &p) const
{
    if (!tilted)
        return XY((p.x-center.x)/radius1, (p.y-center.y)/radius2);
    return XY(( (p.x-center.x)*costilt + (p.y-center.y)*sintilt)/radius1,
              (-(p.x-center.x)*sintilt + (p.y-center.y)*costilt)/radius2);
}

inline double circle_space_point2radian_curvy(XY p)
{
    double r = asin(p.y/p.length()); //calc angle normalized to 1
    if (p.x<0) r = M_PI - r;         //between 90 and 270 degrees
    if (r<0) r += 2*M_PI;            //if between -90 and 0 degrees
    return r;
}

//returns the radian
inline double EllipseData::Point2Radian(const XY &p) const
{
    XY q = conv_to_circle_space(p);
    const double d = q.length();
    const double r = circle_space_point2radian_curvy(q/d);
    return r;
    //if (inout) {
    //	if (test_smaller(d,1)) *inout = WI_INSIDE;
    //	else if (test_smaller(1,d)) *inout = WI_OUTSIDE;
    //	else if (test_equal(r,s) || test_equal(r,e)) *inout = WI_ON_VERTEX;
    //	else *inout = WI_ON_EDGE;
    //}
}

inline void EllipseData::TransformForDrawing(cairo_t *cr) const
{
    cairo_translate(cr, center.x, center.y);
    if (tilted)
        cairo_rotate(cr, tilt);
    cairo_scale(cr, radius1, radius2);
}

inline bool between01(double r)
{
    return r>=0 && r<=1;
}

inline bool between01_adjust(double &n) 
{
	if (!test_smaller(n,1) || test_smaller(n,0)) //if n>1 or n~=1 or n<<0
		return 0;
	if (!test_smaller(0,n)) n=0; //if n~=0;
	return true;
}

inline double deg2rad(double degree) 
{
	return degree==360 ? M_PI*2 : fmod(degree, 360)*M_PI/180;
}

bool crossing_line_line(const XY &A, const XY &B, const XY &M, const XY &N,  XY &r);
double point2pos_straight(const XY &M, const XY&N, const XY &p);


#endif //CONTOUR_ELLIPSE_H
