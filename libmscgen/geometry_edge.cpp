/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010 Zoltan Turanyi
    Distributed under GNU Affero General Public License.

    Msc-generator is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Msc-generator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with Msc-generator.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <cassert>
#include <algorithm>
#include "geometry_edge.h"


//////////////////Helper functions

namespace geometry {

static const double SMALL_NUM = 1e-10; //avoid division overflow
inline bool test_zero(double n) 
{
	return n<SMALL_NUM && n>-SMALL_NUM;
}

inline bool test_equal(double n, double m) 
{
	return test_zero(n-m);
}

inline bool test_smaller(double n, double m)
{
	return n<m-SMALL_NUM;
}

//safe cubic root
inline double curt(double n)
{
	if (n==0) return 0;
	if (n<0) return -pow(-n, 1./3);
	else return pow(n, 1./3);
}

inline double sqr(double n)
{
	return n*n;
}

//solve Ax^2 + Bx + C = 0 for real roots. 
//Place them in r[0] and r[1] and return the number of real roots
int quadratic_solve(double A, double B, double C, double *R) 
{
	if (A==0) {
		//linear case
		if (B==0) return 0;
		R[0] = -C/B;
		return 1;
	}
	double D = B*B - 4*A*C;
	if (D<0) return 0;
	R[0] = -B/(2*A);
	if (D==0) return 1;
	D = sqrt(D)/(2*A);
	R[1] = R[0] + D;
	R[0] -= D;
	return 2;
}

//Solve C1x^3 + C2x^2 + C3x + C4 = 0 
int cubic_solve(double C1, double C2, double C3, double C4, double *R) 
{
	if (C1==0) 
		return quadratic_solve(C2, C3, C4, R);
	
	//using x = y-p/3 we get y^3 +Ay +B 
	//see http://www.me.gatech.edu/energy/andy_phd/appA.htm
	const double p = C2/C1, q = C3/C1, r = C4/C1;
	const double A = (3*q - p*p)/3;
	const double B = (2*p*p*p - 9*p*q + 27*r)/27;
	const double D = A*A*A/27 + B*B/4;

	if (D<0) {
		const double cosphi = sqrt((B*B/4) / (-A*A*A/27));
		const double phi = acos(B>0 ? -cosphi : cosphi);
		const double t = 2*sqrt(-A/3);
		R[0] = t*cos(phi) - p/3;
		R[1] = t*cos(phi + 120./180.*M_PI) - p/3;
		R[2] = t*cos(phi + 240./180.*M_PI) - p/3;
		return 3;
	}
	if (D>0) {
		const double M = curt(-B/2 + sqrt(D));
		const double N = curt(-B/2 - sqrt(D));
		R[0] =  M + N - p/3;
		if (A==0) return 1; //all three roots are the same
		R[1] = -(M+N)/2 - p/3;  
		return 2;
	}
	R[0] = 2*curt(-B/2) - p/3;
	return 1;
}

//Solve C1x^3 + C2x^2 + C3x + C4 = 0 
double cubic_substitute(double C1, double C2, double C3, double C4, double R)
{
	return C1*R*R*R + C2*R*R + C3*R + C4;
}


//Solves Ax^4 + Bx^3 + Cx^2 + Dx + E = 0
int quartic_solve(double A, double B, double C, double D, double E, double *r)
{
	if (A==0) 
		return cubic_solve(B, C, D, E, r);

	//rearrange for a depressed solution
	//x = u - B/(4*A)
	//u^4 + alpha*u^2 + beta*u + gamma, where beta!=0 && gamma!=0
	//see http://en.wikipedia.org/wiki/Quartic_equation#The_general_case.2C_along_Ferrari.27s_lines

	const double alpha = -3*B*B/(8*A*A) +C/A;
	const double beta  = B*B*B/(8*A*A*A) -B*C/(2*A*A) +D/A;
	const double gamma = -3*B*B*B*B/(256*A*A*A*A) + C*B*B/(16*A*A*A) - B*D/(4*A*A) +E/A;
	const double B4A = B/(4*A);
	
	if (beta==0) {
		//biquadratic case u^4 +alpha*u^2 + gamma
		int num = quadratic_solve(1, alpha, gamma, r);
		if (num==0) return 0;
		if (num == 2) {
			if (r[1]<0) {
				num=1;
				r[0] = r[1];
			} else {
				r[2] = sqrt(r[1]);
				r[3] = -r[1];
			}
		}
		if (r[0]<0) {
			if (num==1) return 0;
			r[0] = r[2] - B4A;
			r[1] = r[3] - B4A;
			return 2;
		}
		r[0] = sqrt(r[0]);
		r[1] = -r[0];

		r[0] -= B4A;
		r[1] -= B4A;
		r[2] -= B4A;
		r[3] -= B4A;
		return 4;
	}

	if (gamma==0) {
		r[0] = -B4A; //zero is solution for u
		int num = cubic_solve(1, 0, alpha, beta, r+1);
		r[1] -= B4A;
		r[2] -= B4A;
		r[3] -= B4A;
		return num+1;
	}

	//now solve the depressed using Ferrari's method

	double ys[3];
	//use eqn #4 from the wiki page
	//y^3 + (5*alpha/2)y^2 + (2alpha^2-gamma)y +(alpha^3/2 - alpha*gamma/2 - beta^2/2) = 0
	int nn = cubic_solve(1, 5*alpha/2, 2*sqr(alpha)-gamma, sqr(alpha)*alpha/2 - alpha*gamma/2 - sqr(beta)/2, ys);
	//we need only one root.

	const double P = -alpha*alpha/12 - gamma;
	const double Q = -alpha*alpha*alpha/108 + alpha*gamma/3 - beta*beta/8;
	const double R = -Q/2 + sqrt(Q*Q/4 +P*P*P/27);
	const double U = curt(R);
	const double y = -5*alpha/6 + (U==0 ? curt(Q) : U - P/(3*U));
	const double W = sqrt(alpha +2*y);

	int num=0;

	const double dist1 = -(3*alpha + 2*y - 2*beta/W);
	if (dist1 >= 0) {
		r[2] = -B4A + (-W + sqrt(dist1))/2;
		if (dist1==0) 
			num = 1;
		else {
			num = 2;
			r[3] = -B4A + (-W - sqrt(dist1))/2;
		}
	}
	const double dist2 = -(3*alpha + 2*y + 2*beta/W);
	if (dist2 >= 0) {
		r[0] = -B4A + (W + sqrt(dist2))/2;
		if (dist2==0) {
			if (num >= 1) r[1] = r[2];
			if (num == 2) r[2] = r[3];
			return num + 1;
		} else {
			r[1] = -B4A + (W - sqrt(dist1))/2;
			return num + 2;
		}
	}
	if (num >= 1) r[0] = r[2];
	if (num == 2) r[1] = r[3];
	return num;
}

//returns -1 if a==b
//returns -2 if a==c
//returns -3 if b==c
//returns -4 if a==b==c
//returns 0 if a, b and c are on the same line
//returns 1 if a->b->c is clockwise, 2 otherwise
triangle_dir_t triangle_dir(XY a, XY b, XY c) 
{
	if (a == b) return b==c ? ALL_EQUAL : A_EQUAL_B;
	if (a == c) return A_EQUAL_C;
	if (b == c) return B_EQUAL_C;
	//Decide if we divide by x or y coordinates
	if (fabs(a.x - b.x) < fabs(a.y - b.y)) {
		const double m = (a.x - b.x)/(a.y - b.y);
		double cx =  m*(c.y-a.y) + a.x; //(cx, c.y) is a point on the a-b line
		if (cx == c.x) return IN_LINE;
		return (c.x < cx ^ a.y < b.y) ? COUNTERCLOCKWISE : CLOCKWISE;
	} else {
		const double m = (a.y - b.y)/(a.x - b.x);
		double cy =  m*(c.x-a.x) + a.y; //(c.x, cy) is a point on the a-b line
		if (cy == c.y) return IN_LINE;
		return (c.y < cy ^ a.x < b.x) ? CLOCKWISE : COUNTERCLOCKWISE;
	}
}

//returns a value [0..4) that is representative of clockwise angle: bigger angle->bigger value, but not linear
//-1 is error
double angle(XY base, XY A, XY B) 
{
	bool clockwise;
	switch (triangle_dir(base, A, B)) {
	case IN_LINE:
	case CLOCKWISE:
		clockwise = true;
		break;
	case COUNTERCLOCKWISE:
		clockwise = false;
		break;
	case B_EQUAL_C:
		return 0;  //zero degrees
	default:
		return -1; //error one of them equals to another
	};
	double cos = (A-base).DotProduct(B-base) / (A-base).length() / (B-base).length();
	if (clockwise)
		return 1-cos; //gives [0..2]
	else 
		return cos+3; //gives (2..4)
}

//slope of a line (assuming not vertical)
inline double MM(XY A, XY B)
{
	return (A.y-B.y)/(A.x-B.x);
}

//ensures r is between [0..2pi]
inline double radiannormalize(double r)
{
	if (r>=0 && r<2*M_PI) return r;
	return r - floor(r/(2*M_PI))*(2*M_PI);
}

//arc-length in the clockwise dir (msc space!) from s to e
inline double radianspan(double s, double e)
{
	s = radiannormalize(s);
	e = radiannormalize(e);
	if (s<=e) return e-s;
	return 2*M_PI - (s-e);
}

inline bool between01(double r)
{
	return r>=0 && r<=1;
}

/////////////////PolyEdge implementation

inline XY PolyEdge::conv_to_real_space(const XY &p) const
{
	_ASSERT(type!=STRAIGHT);
	if (type==CURVY_NON_TILTED) 
		return center + XY(radius1 * p.x, radius2 * p.y);
	return center + 
		XY(radius1 * p.x * costilt - radius2 * p.y * sintilt,
			radius1 * p.x * sintilt + radius2 * p.y * costilt);
}

inline XY PolyEdge::conv_to_circle_space(const XY &p) const
{
	_ASSERT(type!=STRAIGHT);
	if (type==CURVY_NON_TILTED) 
		return XY((p.x-center.x)/radius1, (p.y-center.y)/radius2);
	return XY(( (p.x-center.x)*costilt + (p.y-center.y)*sintilt)/radius1, 
	          (-(p.x-center.x)*sintilt + (p.y-center.y)*costilt)/radius2);
}


//Convert between pos (0..1) and coordinates
inline XY PolyEdge::pos2point_straight(double r, const XY &B) const
{
	_ASSERT(type==STRAIGHT);
	return XY(start.x+(B.x-start.x)*r, start.y+(B.y-start.y)*r);
}

//convert from pos [0..1] to actual radian. Guaranteed between [0..2pi)
inline double PolyEdge::pos2radian(double r) const
{
	_ASSERT(type!=STRAIGHT);
	if (clockwise_arc) {
		if (s<e) return s + (e-s)*r;
		else     return s + (e-s+2*M_PI)*r;
	} else {
		if (s<e) return e + (s-e+2*M_PI)*(1-r);
		else     return e + (s-e)*(1-r);
	} 
}

inline XY PolyEdge::pos2point_curvy(double r) const
{
	r=pos2radian(r);
	return conv_to_real_space(XY(cos(r), sin(r)));
}


//returns [0..1] if p is on MN, other value if not
inline double point2pos_straight(const XY &M, const XY&N, const XY &p)
{
	if (M==N) return p==M ? 0 : -1;
	if (fabs(M.x-N.x) > fabs(M.y-N.y)) {
		double t = (p.x-M.x)/(N.x-M.x);
		if (test_equal(p.y, M.y + (N.y-M.y)*t)) return t;
	} else {
		double t = (p.y-M.y)/(N.y-M.y);
		if (test_equal(p.x, M.x + (N.x-M.x)*t)) return t;
	}
	return -1; 
}

//can return outside [0..1] if point is not between s and e
inline double PolyEdge::radian2pos(double r) const
{
	r = radiannormalize(r);
	_ASSERT(type!=STRAIGHT);
	if (test_equal(s,e)) return 0;
	if (clockwise_arc) {
		//here r-s can be <0 or bigger than span, 
		//so returned pos can be outside [0..1]
		if (s<e) return (r-s)/(e-s);
		//ok, e<s
		if (r>=s) return (r-s)/(e-s+2*M_PI);
		return (r-s+2*M_PI)/(e-s+2*M_PI);
	}
	if (s>e) return (r-s)/(e-s);
	if (r>=e) return (s-r+2*M_PI)/(s-e+2*M_PI);
	return (s-r)/(s-e+2*M_PI);
}

inline double circle_space_point2radian_curvy(XY p) 
{
	double r = asin(p.y/p.length());     //calc angle normalized to 1
	if (p.x<0) r = M_PI - r;    //between 90 and 270 degrees
	if (r<0) r += 2*M_PI;       //if between -90 and 0 degrees
	return r;
}

//returns
inline double PolyEdge::point2pos_curvy(XY p, is_within_t *inout) const
{
	_ASSERT(type!=STRAIGHT);
	XY q = conv_to_circle_space(p);
	const double d = q.length();
	const double r = circle_space_point2radian_curvy(q/d);
	if (inout) {
		if (test_smaller(d,1)) *inout = WI_INSIDE;
		else if (test_smaller(1,d)) *inout = WI_OUTSIDE;
		else if (test_equal(r,s) || test_equal(r,e)) *inout = WI_ON_VERTEX;
		else *inout = WI_ON_EDGE;
	}
	return radian2pos(r);
}

//helper for curvy or tilted
inline bool PolyEdge::radianbetween(double r) const
{
	_ASSERT(type!=STRAIGHT);
	r = radiannormalize(r);
	if (clockwise_arc) {
		if (s<e) return s<=r && r<=e;
		else return r>=s || r<=e;
	} else {
		if (s<e) return s>=r || r<=e;
		else return s>=r && r<=e;
	}
}

//checks if two straight sections are crossing or not
//return the number of points:
//0 - no crossing
//1 - one crossing point (in r[0])
//2 - the two sections intersects from r[0] to r[1]
//if any of the two sections are degenerate we return 1 only if it lies on the other section, else 0
//in pos_in_ab we return the relative pos of the crosspoint(s) in AB, in pos_in_mn for MN
//See http://softsurfer.com/Archive/algorithm_0104/algorithm_0104B.htm
int PolyEdge::crossing_straight_straight(const XY A, const XY B, const XY M, const XY N, 
		                                 XY *r, double *pos_in_ab, double *pos_in_mn)
{
	if (!test_zero((B-A).PerpProduct(N-M))) {
		//They are not parallel (and none of them are degenerate, but that does not matter now)
		const double t = (B-A).PerpProduct(A-M) / (B-A).PerpProduct(N-M);
		if (!between01(t)) 
			return 0; //the intersection of the line is outside MN
		const double s = (N-M).PerpProduct(A-M) / (B-A).PerpProduct(N-M);
		if (!between01(s)) 
			return 0; //the intersection of the line is outside AB
		r[0] = M + (N-M)*t;
		pos_in_mn[0] = t;
		pos_in_ab[0] = s;
		return 1;
	}
	//either they are parallel or one or both sections are degenerate (= a point)
	if (M==N) {
		if (!test_zero((M-A).PerpProduct(B-A))) 
			return 0; //M is not on AB's line and AB is not degenerate
		if (A==B) {
			if (A==M) {
				r[0] = M;
				pos_in_mn[0] = 0;
				pos_in_ab[0] = 0;
				return 1;
			}
			return 0; //both sections degenerate to a point, but do not overlap
		}
		double s;
		//select x or y depending on precision
		//M lies on AB, let us see if between A and B
		if (fabs(A.x-B.x) > fabs(A.y-B.y)) 
			s = (M.x-A.x)/(B.x-A.x);
		else 
			s = (M.y-A.y)/(B.y-A.y);
		if (!between01(s)) return 0;
		r[0] = M; //M is on AB 
		pos_in_mn[0] = 0;
		pos_in_ab[0] = s;
		return 1;
	}
	if (!test_zero((A-M).PerpProduct(N-M))) 
		return 0; //A is not on MN's line
	//They are either parallel and on the same line or AB is degenerate and lies on MN's line
	double t0, t1;
	//select x or y depending on precision
	if (fabs(M.x-N.x) > fabs(M.y-N.y)) {
		t0 = (A.x-M.x)/(N.x-M.x);
		t1 = (B.x-M.x)/(N.x-M.x);
	} else {
		t0 = (A.y-M.y)/(N.y-M.y);
		t1 = (B.y-M.y)/(N.y-M.y);
	}
	if (t0 > t1) std::swap(t0, t1);
	if (t0 > 1 || t1 < 0) 
		return 0; //AB lies outside MN
	if (t0 < 0) t0 = 0;
	if (t1 > 1) t1 = 1;
	r[0] = M + (N-M)*t0;
	pos_in_mn[0] = t0;
	if (fabs(A.x-B.x) > fabs(A.y-B.y)) 
		pos_in_ab[0] = (r[0].x-A.x)/(B.x-A.x);
	else 
		pos_in_ab[0] = (r[0].y-A.y)/(B.y-A.y);
	if (t0 == t1) 
		return 1; //AB is degenerate
	r[1] = M + (N-M)*t1;
	pos_in_mn[1] = t1;
	if (fabs(A.x-B.x) > fabs(A.y-B.y)) 
		pos_in_ab[1] = (r[1].x-A.x)/(B.x-A.x);
	else 
		pos_in_ab[1] = (r[1].y-A.y)/(B.y-A.y);
	return 2;
}

int PolyEdge::crossing_curvy_curvy(const PolyEdge &B, XY *r, double *pos_us, double *pos_b) const
{
	//Now this is scary shit. Above there is a suite to solve 4th degree equations, we use those
	//we are interested only in real solutions
	//for ellipses see http://www.geometrictools.com/Documentation/IntersectionOfEllipses.pdf

	//XXX ignore tilt for just now
	//then an equation for our ellypses is (x-Cx)^2/radius1^2 + (y-Cy)^2/radius2^2 = 1;
	_ASSERT(type == CURVY_NON_TILTED && B.type == CURVY_NON_TILTED);

	const double Aa00 = 1/sqr(radius1);
	const double Aa01 = 0;
	const double Aa11 = 1/sqr(radius2);
	const double Ab0  = -2*center.x/sqr(radius1);
	const double Ab1  = -2*center.y/sqr(radius2);
	const double Ac   = sqr(center.x/radius1) + 
		                sqr(center.y/radius2) - 1;

	const double Ba00 = 1/sqr(B.radius1);                                 //*x^2
	const double Ba01 = 0;                                                //*xy
	const double Ba11 = 1/sqr(B.radius2);                                 //*y^2
	const double Bb0  = -2*B.center.x/sqr(B.radius1);                     //*x
	const double Bb1  = -2*B.center.y/sqr(B.radius2);                     //*y
	const double Bc   = sqr(B.center.x/B.radius1) +                       //constant
		                sqr(B.center.y/B.radius2) - 1;

	const double v0 = 2*(Aa00*Ba01 - Ba00*Aa01);
	const double v1 = Aa00*Ba11 - Ba00*Aa11;
	const double v2 = Aa11*Bb0  - Ba00*Ab0;
	const double v3 = Aa00*Bb1  - Ba00*Ab1;
	const double v4 = Aa00*Bc   - Ba00*Ac;
	const double v5 = 2*(Aa01*Ba11 - Ba01*Aa11);
	const double v6 = 2*(Aa01*Bb1  - Ba01*Ab1);
	const double v7 = 2*(Aa01*Bc   - Ba01*Ac);
	const double v8 = Aa11*Bb0  - Ba11*Ab0;
	const double v9 = Ab0*Bb1 - Bb0*Ab1;
	const double v10= Ab0*Bc - Bb0*Ac;

	const double u0 = v1*v10-v4*v4;
	const double u1 = v0*v10 + v2*(v7+v9) - 2*v3*v4;
	const double u2 = v0*(v7+v9) + v2*(v6-v8) - v3*v3 - 2*v1*v4;
	const double u3 = v0*(v6-v8) + v2*v5 - 2*v1*v3;
	const double u4 = v0*v5 - v1*v1;

	double y[4];
	//The Bezout determinant is now a quartic polynomialwith u:s as coefficienst
	int num = quartic_solve(u4, u3, u2, u1, u0, y);
	int num_found = 0;

	for (int i=0; i<num; i++) {
		double x[2];
		//solve for x from curve A
		int num2 = quadratic_solve(Aa00, Aa01*y[i] + Ab0, 
			                       Aa11*y[i]*y[i] + Ab1*y[i] + Ac, 
								   x);
		//eliminate the wrong x from curve B
		for (int j = 0; j<2; j++)
			if (test_smaller(Ba00*x[j]*x[j] + Ba01*y[i]*x[j] + Bb0*x[j] +
					         Ba11*y[i]*y[i] + Bb1*y[i] + Bc, 0)) {
				r[num_found].x = x[i];
				r[num_found].y = y[j];
				break;
			}
	}
	//Now we need to check if all points fall between s and e
	for (int i=0; i<num_found; i++) {
		pos_us[i] =   point2pos_curvy(r[i]);
		pos_b[i]  = B.point2pos_curvy(r[i]);
		if (pos_us[i]<0 || pos_us[i]>1 || pos_b[i]<0 || pos_b[i]>1) {
			for (int j=1; j<num_found-i; j++) {
				r[i+j-1]      = r[i+j];
				pos_us[i+j-1] = pos_us[i+j];
				pos_b[i+j-1]  = pos_b[i+j];
			}
			num_found--;
		}
	}
	return num_found;
}

int PolyEdge::crossing_curvy_straight(const XY &A, const XY &B, 
			                         XY *r, double *pos_us, double *pos_b) const
{
	_ASSERT(type != STRAIGHT);
	XY M = conv_to_circle_space(A);
	XY N = conv_to_circle_space(B);

	//See circle intersection with a line specificed by two points
	//http://mathworld.wolfram.com/Circle-LineIntersection.html
	double D = M.PerpProduct(N);
	XY d = N-M;
	double disc = d.length()*d.length() - D*D;
	if (disc<0) return 0; //no intersection

	XY v(d.x*sqrt(disc), fabs(d.y)*sqrt(disc));
	if (d.y<0) v.x = -v.x;
	XY f(D*d.y, -D*d.x);

	//the intersect coordinates in unit circle space
	r[0] = (f+v)/d.length()/d.length();
	r[1] = (f-v)/d.length()/d.length();

	pos_us[1] = radian2pos(circle_space_point2radian_curvy(r[1]));
	r[1] = conv_to_real_space(r[1]);
	pos_b[1]  = point2pos_straight(A, B, r[1]);
	bool both = between01(pos_us[1]) && between01(pos_b[1]);
	if (test_smaller(disc, 0)) { //just touch, r[0] == r[2]
		only_second_is_good:
		if (!both) return 0;   
		r[0]      = r[1];
		pos_us[0] = pos_us[1];
		pos_b[0]  = pos_b[1];
		return 1;
	}

	//now check r1
	pos_us[0] = radian2pos(circle_space_point2radian_curvy(r[0]));
	r[0] = conv_to_real_space(r[0]);
	pos_b[0]  = point2pos_straight(A, B, r[0]);
	if (!between01(pos_us[0]) || !between01(pos_b[0])) //we are out
		goto only_second_is_good;
	return both ? 2 : 1;
}

inline XY PolyEdge::prevnexttangent_curvy(double pos, bool next) const
{
	const double r = pos2radian(pos);
	const double x = cos(r);
	const double y = sin(r);
	//for a point xy on the unit circle to have a point on the
	//forward tangent is: x+y, y-x
	//backward tangent is: x-y, y+x
	if (next ^ clockwise_arc) 
		return conv_to_real_space(XY(x+y, y-x));
	else
		return conv_to_real_space(XY(x-y, y+x));
}

void PolyEdge::removebeforepoint_curvy(const XY &p) 
{
	const double r = circle_space_point2radian_curvy(conv_to_circle_space(p));
	_ASSERT(radianbetween(r));
	s = r;
}

void PolyEdge::removeafterpoint_curvy(const XY &p) 
{
	e = circle_space_point2radian_curvy(conv_to_circle_space(p));
}


PolyEdge::PolyEdge(const XY &c, double radius_x, double radius_y, double tilt_degree) :
	center(c), radius1(radius_x), radius2(radius_y),
	s(0), e(2*M_PI), clockwise_arc(true)
{
	type = CURVY_NON_TILTED;
	if (radius2 == 0) radius2 = radius1; //circle
	if (radius1 != radius2 && tilt_degree!=0) {
		tilt_degree -= floor(tilt_degree/180)*180;
		if (tilt_degree>=90) {
			tilt_degree -= 90;
			std::swap(radius1, radius2);
		}
		if (tilt_degree) {
			tilt = tilt_degree * M_PI/180.;
			sintilt = sin(tilt);
			costilt = cos(tilt);
			type = CURVY_TILTED;
			start = pos2point_curvy(0);
			return;
		}
	}
	//type is curvy here
	start = pos2point_curvy(0);
}


//Where does an edge or arc corss a horizontal line? (for the purposes of Polygon::IsWithin)
//1. an upward edge includes its starting endpoint, and excludes its final endpoint;
//2. a downward edge excludes its starting endpoint, and includes its final endpoint;
//returns 0 if no crossing, 1/2 if there are crosspoints and -1 if it is a horizontal line
//internally "downward" is understood in a coordinate system where y grows downwards
int PolyEdge::CrossingHorizontal(double y, const XY &B, double *x) const
{
	double a1, a2;
	int num;
	switch (type) {
	case STRAIGHT:
		if ((start.y >= y && B.y < y) ||      //we cross upward 
			(start.y < y && B.y >= y)) {      //we cross downward
			//we cross p's line x
			x[0] = (start.x - B.x)/(start.y - B.y)*(y - B.y) + B.x;
			return 1;
		} 
		if (start.y == y && B.y == y) return -1; //horizontal line
		return 0;
	case CURVY_NON_TILTED:
		if (y < center.y-radius2 || y > center.y+radius2) return 0;
		double xx;
		xx = center.x + radius1*sqrt(1 - sqr((y-center.y)/radius2));
		a1 = circle_space_point2radian_curvy(conv_to_circle_space(XY(xx,y)));
		if (test_equal(xx, center.x)) {  //just touch
			if (!test_equal(a1, s) && !test_equal(a1, e)) 
				return 0; //touch is ignored except in endpoints
			x[0] = xx;
			num = -1;
			break;
		}
		a2 = (a1<=M_PI) ? M_PI - a1 : 3*M_PI - a1;
		if (radianbetween(a1)) {
			x[0] = xx;
			if (radianbetween(a2)) {
				x[1] = 2*center.x - xx;
				num = 2;
			} else
				num = 1;
		} else if (radianbetween(a2)) {
			x[0] = 2*center.x - xx;
			a1 = a2;
			num = 1;
		} else
			return 0;
		break;
	case CURVY_TILTED:
		//transform the y line into the system of 
		//we start with two points 0,y and y,y
		XY M = conv_to_circle_space(XY(0, y));
		XY N = conv_to_circle_space(XY(y, y));
		//See circle intersection with a line specificed by two points
		//http://mathworld.wolfram.com/Circle-LineIntersection.html
		double D = M.PerpProduct(N);
		XY d = N-M;
		double disc = d.length()*d.length() - D*D;
		if (disc<0) return 0; //no intersection

		XY v(d.x*sqrt(disc), fabs(d.y)*sqrt(disc));
		if (d.y<0) v.x = -v.x;
		XY f(D*d.y, -D*d.x);

		//M, N: the intersect coordinates in unit circle space
		M = (f+v)/sqr(d.length());
		a1 = circle_space_point2radian_curvy(M);

		if (test_zero(disc)) { //just touch
			if (!test_equal(a1, s) && !test_equal(a1, e)) 
				return 0; //touch is ignored except in endpoints
			x[0] = conv_to_real_space(M).x;
			num = -1;
			break;
		}
		N = (f-v)/sqr(d.length());
		a2 = circle_space_point2radian_curvy(N);
		if (radianbetween(a1)) {
			x[0] = conv_to_real_space(M).x;
			if (radianbetween(a2)) {
				x[1] = conv_to_real_space(N).x;
				num = 2;
			} else 
				num = 1;
		} else if (radianbetween(a2)) {
			x[0] = conv_to_real_space(N).x;
			num = 1;
		} else 
			return 0;
		break;
	}
	//now num contains how many crossings we have (-1 if it is a touch in an endpoint)
	//x[0], x[1] contains the points, a1 and a2 contains the resp radians
	//if the crosspoints are at the end of the segments, we check for up or downward crossing.

	if (num==-1) 
		//here we know either a1==s or ==e and we touch the horizontal line at y
		//if the centerpoint is above y and a1==s it is the beginning of an upward edge  => include
		//if the centerpoint is above y and a1==e it is the end of a downward edge       => include
		//if the centerpoint is below y and a1==s it is the beginning of a downward edge => exclude
		//if the centerpoint is below y and a1==e it is the end of an upward edge        => exclude
		return center.y < y ? 1 : 0;

	if (a2==s || a2==e)  //if a2 is on one endpoint 
		//if a2 == s and forward tangent is above y, it is the beginning of an upward edge => include
		//if a2 == d and backwardtangent is above y, it is the end of an downward edge     => include
		//else exclude
		if (prevnexttangent_curvy(a2, a2==s).y > y)  //exclude
			num = 1;
	if (a1==s || a1==e)  //if a1 is on one endpoint 
		if (prevnexttangent_curvy(a1, a1==s).y > y)  {//exclude
			if (num == 1) return 0;
			x[0] = x[1];
			return 1;
		}
	return num; 
}


//Adds this point (for straight) or full curve to a bounding box
Block& PolyEdge::AddToBoundingBox(Block &b) const
{
	switch (type) {
	default:
		_ASSERT(0);
	case CURVY_NON_TILTED:
		//if any extreme point is included in the arc add them
		if (radianbetween(       0)) b += (center+XY(radius1,       0));
		if (radianbetween(0.5*M_PI)) b += (center+XY(      0, radius2));
		if (radianbetween(    M_PI)) b += (center-XY(radius1,       0));
		if (radianbetween(1.5*M_PI)) b += (center-XY(      0, radius2));
		//add endpoint
		b += conv_to_real_space(XY(cos(e), sin(e)));
		//fallthrough
	case STRAIGHT: 
		//add startpoint
		return b += start;
	case CURVY_TILTED:
		//Add the biggest rectangle a tilted ellipse can fit
		const double r = std::max(radius1, radius2);
		b += (center-XY(r,r));
		b += (center+XY(r,r));
		return b;
	}
}

//Checks if this->next edge and next->after can be combined into a single edge
//if so, also update this such that this->after will be the combined edge
bool PolyEdge::CheckAndCombine(const PolyEdge &next, const XY &after)
{
	if (type != next.type) return false;
	if (type==STRAIGHT && next.type==STRAIGHT) {
		const double a = angle(start, next.start, after);
		return a==0 || a==2;
	}
	//calc tangent for curvy edges
	if (center != next.center || radius1 != next.radius1 ||
		radius2 != next.radius2 || 
		clockwise_arc != next.clockwise_arc) return false;
	if (type == CURVY_TILTED && tilt != next.tilt) 
		return false;
	//same center, same radiuses, same tilt, same dir
	//keep our s and pick the bigger of the two e:s
	//but not accidentally pick an e smaller than s
	if (s<e) {
		if (s<next.e) e = std::max(e, next.e);
		else e = next.e;
	} else 
		if (s>=next.e) e = std::max(e, next.e);
		else e = s; //round over, we crop to full circle
	return true;
}

}; //namespace