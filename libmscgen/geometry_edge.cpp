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

/*Parts of this file is based on WildMagic 5.4, which uses the Boost licence,
  which requires me to repeat it here. See http://www.geometrictools.com

	Boost Software License - Version 1.0 - August 17th, 2003

	Permission is hereby granted, free of charge, to any person or organization
	obtaining a copy of the software and accompanying documentation covered by
	this license (the "Software") to use, reproduce, display, distribute,
	execute, and transmit the Software, and to prepare derivative works of the
	Software, and to permit third-parties to whom the Software is furnished to
	do so, all subject to the following:

	The copyright notices in the Software and this entire statement, including
	the above license grant, this restriction and the following disclaimer,
	must be included in all copies of the Software, in whole or in part, and
	all derivative works of the Software, unless such copies or derivative
	works are solely in the form of machine-executable object code generated by
	a source language processor.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
	SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
	FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
	ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
	DEALINGS IN THE SOFTWARE.
*/

#include <cassert>
#include <vector>
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

//true if *really* smaller, not just by epsilon
inline bool test_smaller(double n, double m)
{
	return n<m-SMALL_NUM;
}

//true if *really* greater, not just by epsilon
inline bool test_greater(double n, double m)
{
	return n-SMALL_NUM>m;
}

inline bool test_positive(double n)
{
	return n >= SMALL_NUM;
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

//Solve quadratic equation m_afCoeff[0] is the constant, m_afCoeff[2] is the coeff of x^2 
unsigned solve_degree2 (const double m_afCoeff[3], double afRoot[2]) 
{
    // compute real roots to c[2]x^2+c[1]*x+c[0] = 0
	if (m_afCoeff[2] == 0) {
		//linear
		if (m_afCoeff[1] == 0) 
			return 0;
		afRoot[0] = -m_afCoeff[0]/m_afCoeff[1];
		return 1;
	}
	
	// make polynomial monic
    double afCoeff[2] = { m_afCoeff[0], m_afCoeff[1] };
    if ( m_afCoeff[2] != 1.0f ) {
        double fInv = 1.0f/m_afCoeff[2];
        afCoeff[0] *= fInv;
        afCoeff[1] *= fInv;
    }

    double fDiscr = afCoeff[1]*afCoeff[1]-4.0f*afCoeff[0];
    if ( test_zero(fDiscr) ) {
        afRoot[0] = 0.5f*(-afCoeff[1]);
		return 1;
	}

    if ( fDiscr >= 0.0f ) {
        fDiscr = sqrt(fDiscr);
        afRoot[0] = 0.5f*(-afCoeff[1]-fDiscr);
        afRoot[1] = 0.5f*(-afCoeff[1]+fDiscr);
        return 2;
    }
    return 0;
}

//Solve cubic equation m_afCoeff[0] is the constant, m_afCoeff[3] is the coeff of x^3 
unsigned solve_degree3 (const double m_afCoeff[4], double afRoot[3])
{
    // compute real roots to c[3]*x^3+c[2]*x^2+c[1]*x+c[0] = 0
	if (m_afCoeff[3] == 0)
		return solve_degree2(m_afCoeff, afRoot);

    // make polynomial monic
    double afCoeff[3] = { m_afCoeff[0], m_afCoeff[1], m_afCoeff[2] };
    if ( m_afCoeff[3] != 1.0f ) {
        double fInv = 1.0f/m_afCoeff[3];
        afCoeff[0] *= fInv;
        afCoeff[1] *= fInv;
        afCoeff[2] *= fInv;
    }

    // convert to y^3+a*y+b = 0 by x = y-c[2]/3 and
    double fA = (1.0f/3.0f)*(3.0f*afCoeff[1]-afCoeff[2]*afCoeff[2]);
    double fB = (1.0f/27.0f)*(2.0f*afCoeff[2]*afCoeff[2]*afCoeff[2] -
        9.0f*afCoeff[1]*afCoeff[2]+27.0f*afCoeff[0]);
    double fOffset = (1.0f/3.0f)*afCoeff[2];

    double fDiscr = 0.25f*fB*fB + (1.0f/27.0f)*fA*fA*fA;
    double fHalfB = 0.5f*fB;
    if ( test_zero(fDiscr) ) {    //3 real roots, but at least two are equal
        double fTemp;
        if ( fHalfB >= 0.0f )
            fTemp = -pow(fHalfB, double(1.0/3.0));
		else
            fTemp = pow(-fHalfB, double(1.0/3.0));
		if (test_zero(fTemp)) {
			afRoot[0] = -fOffset;
			return 1;
		}
        afRoot[0] = 2.0f*fTemp-fOffset;
        afRoot[1] = -fTemp-fOffset;
        return 2;
    }

    if ( fDiscr > 0.0f )  // 1 real, 2 complex roots
    {
        fDiscr = sqrt(fDiscr);
        double fTemp = -fHalfB + fDiscr;
        if ( fTemp >= 0.0f )
            afRoot[0] = pow(fTemp, double(1.0/3.0));
        else
            afRoot[0] = -pow(-fTemp, double(1.0/3.0));
        fTemp = -fHalfB - fDiscr;
        if ( fTemp >= 0.0f )
            afRoot[0] += pow(fTemp, double(1.0/3.0));
        else
            afRoot[0] -= pow(-fTemp, double(1.0/3.0));
        afRoot[0] -= fOffset;
        return 1;
    }
    else if ( fDiscr < 0.0f ) 
    {
        double fDist = sqrt(-1.0/3.0*fA);
        double fAngle = (1.0/3.0)*atan2(sqrt(-fDiscr),-fHalfB);
        double fCos = cos(fAngle);
        double fSin = sin(fAngle);
		static const double sqrt3 = sqrt(3.0);
        afRoot[0] = 2.0f*fDist*fCos-fOffset;
        afRoot[1] = -fDist*(fCos+sqrt3*fSin)-fOffset;
        afRoot[2] = -fDist*(fCos-sqrt3*fSin)-fOffset;
        return 3;
    }

    return true;
}

//Solve 4th degree equation m_afCoeff[0] is the constant, m_afCoeff[4] is the coeff of x^4 
//Based on David Eberly's code at
//http://svn.berlios.de/wsvn/lwpp/incubator/deeppurple/math/FreeMagic/Source/Core/MgcPolynomial.cpp
unsigned solve_degree4(const double m_afCoeff[5], double afRoot[4]) 
{
    // compute real roots to c[4]*x^4+c[3]*x^3+c[2]*x^2+c[1]*x+c[0] = 0
	if (m_afCoeff[4] == 0)
		return solve_degree3(m_afCoeff, afRoot);

	// make polynomial monic
    double afCoeff[4] = { m_afCoeff[0], m_afCoeff[1], m_afCoeff[2], m_afCoeff[3] };
    if ( m_afCoeff[4] != 1.0f ) {
        double fInv = 1.0f/m_afCoeff[4];
        afCoeff[0] *= fInv;
        afCoeff[1] *= fInv;
        afCoeff[2] *= fInv;
        afCoeff[3] *= fInv;
    }

    // reduction to resolvent cubic polynomial
    double kResolve[4];
    kResolve[3] = 1.0f;
    kResolve[2] = -afCoeff[2];
    kResolve[1] = afCoeff[3]*afCoeff[1]-4.0f*afCoeff[0];
    kResolve[0] = -afCoeff[3]*afCoeff[3]*afCoeff[0] +
        4.0f*afCoeff[2]*afCoeff[0]-afCoeff[1]*afCoeff[1];
    double afResolveRoot[3];
	int iResolveCount = solve_degree3(kResolve, afResolveRoot);
    double fY = afResolveRoot[0];

	unsigned num = 0;
    double fDiscr = 0.25f*afCoeff[3]*afCoeff[3]-afCoeff[2]+fY;
    if ( test_zero(fDiscr) ) {
        double fT2 = fY*fY-4.0f*afCoeff[0];
        if ( test_positive(fT2) ) {
            if ( fT2 < 0.0f ) // round to zero
                fT2 = 0.0f;
            fT2 = 2.0f*sqrt(fT2);
            double fT1 = 0.75f*afCoeff[3]*afCoeff[3]-2.0f*afCoeff[2];
            if ( test_positive(fT1+fT2) ) {
                double fD = sqrt(fT1+fT2);
                afRoot[0] = -0.25f*afCoeff[3]+0.5f*fD;
                afRoot[1] = -0.25f*afCoeff[3]-0.5f*fD;
                num = 2;
            }
            if ( test_positive(fT1-fT2) ) {
                double fE = sqrt(fT1-fT2);
                afRoot[num++] = -0.25f*afCoeff[3]+0.5f*fE;
                afRoot[num++] = -0.25f*afCoeff[3]-0.5f*fE;
            }
        }
		return num;
    }

    if ( fDiscr > 0.0f ) {
        double fR = sqrt(fDiscr);
        double fT1 = 0.75f*afCoeff[3]*afCoeff[3]-fR*fR-2.0f*afCoeff[2];
        double fT2 = (4.0f*afCoeff[3]*afCoeff[2]-8.0f*afCoeff[1]-
            afCoeff[3]*afCoeff[3]*afCoeff[3])/(4.0f*fR);

        double fTplus = fT1+fT2;
        if ( test_zero(fTplus) ) {
			afRoot[0] = -0.25f*afCoeff[3]+0.5f*fR;
			num = 1;
		} else if ( fTplus >= 0.0f ) {
            double fD = sqrt(fTplus);
            afRoot[0] = -0.25f*afCoeff[3]+0.5f*(fR+fD);
            afRoot[1] = -0.25f*afCoeff[3]+0.5f*(fR-fD);
            num = 2;
        }
        double fTminus = fT1-fT2;
        if ( test_zero(fTminus) ) 
            fTminus = 0.0f;
        if ( fTminus >= 0.0f ) {
            double fE = sqrt(fTminus);
            afRoot[num++] = -0.25f*afCoeff[3]+0.5f*(fE-fR);
            afRoot[num++] = -0.25f*afCoeff[3]-0.5f*(fE+fR);
        }
		return num;
    }

    //if ( fDiscr < 0.0f )
    return 0;
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

inline XY Ellipse::conv_to_real_space(const XY &p) const
{
	if (!tilted) 
		return center + XY(radius1 * p.x, radius2 * p.y);
	return center + 
		XY(radius1 * p.x * costilt - radius2 * p.y * sintilt,
			radius1 * p.x * sintilt + radius2 * p.y * costilt);
}

inline XY Ellipse::conv_to_circle_space(const XY &p) const
{
	if (!tilted) 
		return XY((p.x-center.x)/radius1, (p.y-center.y)/radius2);
	return XY(( (p.x-center.x)*costilt + (p.y-center.y)*sintilt)/radius1, 
	          (-(p.x-center.x)*sintilt + (p.y-center.y)*costilt)/radius2);
}

//helper class for ellipsis intersection calculation
// The quadratic equation representing the ellipse is
//   Q(x,y) = a*x^2 + b*x*y + c*y^2 + d*x + e*y + f = 0
// where b*b < 4*a*c is required for this to represent an ellipse.
struct quadratic_xy_t 
{
	double A;
	double B;
	double C;
	double D;
	double E;
	double F;
	quadratic_xy_t() {}
	quadratic_xy_t(const Ellipse &arc);
};

quadratic_xy_t::quadratic_xy_t(const Ellipse &arc)
{
	if (!arc.tilted) {
		//equation for non-tilt ellypses is (x-Cx)^2/radius1^2 + (y-Cy)^2/radius2^2 = 1;
		A = 1/sqr(arc.radius1);                                 //*x^2
		B = 0;                                                  //*xy
		C = 1/sqr(arc.radius2);                                 //*y^2
		D = -2*arc.center.x/sqr(arc.radius1);                   //*x
		E = -2*arc.center.y/sqr(arc.radius2);                   //*y
		F = sqr(arc.center.x/arc.radius1) + sqr(arc.center.y/arc.radius2) - 1;
	} else {
		A = sqr(arc.costilt/arc.radius1) + sqr(arc.sintilt/arc.radius2);
		B = 2*arc.costilt*arc.sintilt*(1/sqr(arc.radius1) -  1/sqr(arc.radius2));
		C = sqr(arc.sintilt/arc.radius1) + sqr(arc.costilt/arc.radius2);
		D = -2*arc.costilt/sqr(arc.radius1)*(arc.sintilt*arc.center.y + arc.costilt*arc.center.x) +
			 2*arc.sintilt/sqr(arc.radius2)*(arc.costilt*arc.center.y - arc.sintilt*arc.center.x);
		E = -2*arc.sintilt/sqr(arc.radius1)*(arc.sintilt*arc.center.y + arc.costilt*arc.center.x) -
			 2*arc.costilt/sqr(arc.radius2)*(arc.costilt*arc.center.y - arc.sintilt*arc.center.x);
		F = (sqr(arc.costilt*arc.center.x) + 2*arc.costilt*arc.sintilt*arc.center.x*arc.center.y + sqr(arc.sintilt*arc.center.y)) / sqr(arc.radius1) +
			(sqr(arc.sintilt*arc.center.x) - 2*arc.costilt*arc.sintilt*arc.center.x*arc.center.y + sqr(arc.costilt*arc.center.y)) / sqr(arc.radius2) -
			1;
	}
}

void get_bezout_determinant (const quadratic_xy_t &one, const quadratic_xy_t & two, double res[5])
{
    // polynomial is constructed as a Bezout determinant
    double fAB = one.A*two.B-two.A*one.B;
    double fAC = one.A*two.C-two.A*one.C;
    double fAD = one.A*two.D-two.A*one.D;
    double fAE = one.A*two.E-two.A*one.E;
    double fAF = one.A*two.F-two.A*one.F;
    double fBC = one.B*two.C-two.B*one.C;
    double fBE = one.B*two.E-two.B*one.E;
    double fBF = one.B*two.F-two.B*one.F;
    double fCD = one.C*two.D-two.C*one.D;
    double fDE = one.D*two.E-two.D*one.E;
    double fDF = one.D*two.F-two.D*one.F;
    double fBFpDE = fBF+fDE;
    double fBEmCD = fBE-fCD;

    res[0] = fAD*fDF-fAF*fAF;
    res[1] = fAB*fDF+fAD*fBFpDE-2.0f*fAE*fAF;
    res[2] = fAB*fBFpDE+fAD*fBEmCD-fAE*fAE-2.0f*fAC*fAF;
    res[3] = fAB*fBEmCD+fAD*fBC-2.0f*fAC*fAE;
    res[4] = fAB*fBC-fAC*fAC;
}

//finds a crosspoint of two infinite lines defined by AB and MN
bool crossing_line_line(const XY &A, const XY &B, const XY &M, const XY &N,  XY &r)
{
	if (test_zero((B-A).PerpProduct(N-M))) return false;
	//They are not parallel (and none of them are degenerate)
	const double t = (B-A).PerpProduct(A-M) / (B-A).PerpProduct(N-M);
	r = M + (N-M)*t;
	return true;
}

//refines the location of a point using crosspoints of tangents
bool Ellipse::refine_point1(const Ellipse &B, XY &p) const
{
	int max_itr = 32;
	XY p_orig = p;
	while (--max_itr>=0) {
		XY c1 =   conv_to_circle_space(p);
		XY c2 = B.conv_to_circle_space(p);
		const double closeness = sqr(c1.length()-1) + sqr(c2.length()-1);
		if (test_zero(closeness)) 
			return true;
		const double r1 = circle_space_point2radian_curvy(c1);
		const double r2 = circle_space_point2radian_curvy(c2);
		const XY A1 =   conv_to_real_space(XY(cos(r1)        , sin(r1)        ));
		const XY B1 = B.conv_to_real_space(XY(cos(r2)        , sin(r2)        ));
		//A1 and B1 are points on the ellipses closest to p
		//get rid of p, if it is very far from one of the ellipses
		if ((A1-p_orig).length() > 4 || (B1-p_orig).length() > 4) 
			return false;
		const XY A2 =   conv_to_real_space(XY(cos(r1)+sin(r1), sin(r1)-cos(r1)));
		const XY B2 = B.conv_to_real_space(XY(cos(r2)+sin(r2), sin(r2)-cos(r2)));
		//Now A1-A2 and B1-B2 are tangents of the two ellipses
		//We operate on the assumption that the intersection of two tangents is closer to the 
		//intersection of the ellipses
		XY p_new; 
		if (crossing_line_line(A1, A2, B1, B2, p_new) == 0) 
			return false; //no intersection. Lines are parallel two ellipses, we drop this
		if (test_zero((p-p_new).length())) 
			return true; //no improvement, exit
		p = p_new;
	}
	return true;
}

//take the (at most)
int Ellipse::refine_crosspoints(int num_y, double y[], const Ellipse &B, 
	                         const quadratic_xy_t &one, const quadratic_xy_t &two, XY p[]) const
{
    // Adjustment for quadratics to allow for relative error testing.
    //const double fNorm_one = one.A*one.A + 2.0f*one.B*one.B + one.C*one.C;
    const double fNorm_two = two.A*two.A + 2.0f*two.B*two.B + two.C*two.C;

	//go through all solutions of y, find corresponding x 
	XY points[8];
	int num_tmp=0;
    for (int i = 0; i < num_y; i++) {
        double ellipse[3], x[2];
        ellipse[0] = one.F+y[i]*(one.E+y[i]*one.C);
        ellipse[1] = one.D+y[i]*one.B;
        ellipse[2] = one.A;
		const int num_x = solve_degree2(ellipse, x);
		//find the two closest point pairs between x_one and x_two 
        for (int j = 0; j < num_x; j++) {
			points[num_tmp].x = x[j];
			points[num_tmp].y = y[i];
			if (refine_point1(B, points[num_tmp]))
				num_tmp++;
		}
    }
	// now we have max 8 points, where at most 4 are distinct
	int num = 1;
	p[0] = points[0];
	for (int i=1; i<num_tmp; i++) {
		int j;
		for (j=0; j<num; j++) 
			if ((p[j]-points[i]).length()<0.1) 
				break;  
		if (j==num) 
			p[num++] = points[i];
	}
	return num;
}

void Ellipse::transpose_curvy_non_tilted() 
{
	_ASSERT(!tilted);
	std::swap(center.x, center.y);
	std::swap(radius1, radius2);
}


Ellipse::Ellipse(const XY &c, double radius_x, double radius_y, double tilt_degree) :
	center(c), radius1(radius_x), radius2(radius_y), tilted(false)
{
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
			tilted = true;
		}
	}
}

//returns the radian
inline double Ellipse::Point2Radian(const XY &p) const
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



int Ellipse::CrossingEllipse(const Ellipse &B, XY r[], double radian_us[], double radian_b[]) const
{
	//Now this is scary shit. Above there is a suite to solve 4th degree equations, we use those
	//we are interested only in real solutions
	//for ellipses see http://www.geometrictools.com/Documentation/IntersectionOfEllipses.pdf

	double y[4];
	int num_y;
	const quadratic_xy_t one(*this), two(B);
	const double TRSHOLD = 1;
	//if two non-tilted ellipses are aliged by an axis bezout determinant becomes
	//numerically instable and gives no roots -> we handle this case separately
	if (!tilted && !B.tilted &&
		(fabs(center.x - B.center.x) < TRSHOLD || fabs(center.y - B.center.y) < TRSHOLD)) {
		if (fabs(center.y - B.center.y) < TRSHOLD && fabs(center.x - B.center.x) >= TRSHOLD ) {
			//In this case we transpose the ellipses call ourself and re-transpose the results
			Ellipse p1(*this), p2(B);
			p1.transpose_curvy_non_tilted();
			p2.transpose_curvy_non_tilted();
			int num = p1.CrossingEllipse(p2, r, radian_us, radian_b);
			for (int i=0; i<num; i++) {
				std::swap(r[i].x, r[i].y);
				radian_us[i] = radian_us[i]<0.5*M_PI ? 0.5*M_PI-radian_us[i] : 2.5*M_PI-radian_us[i];
				radian_b [i] = radian_b [i]<0.5*M_PI ? 0.5*M_PI-radian_b [i] : 2.5*M_PI-radian_b [i];
			}
			return num;
		}
		//in this case multiplying the two ellipse equations by radian1^2 and substracting one from the other
		//we get a simple quadratic equation in y
		double quadratic[3];
		quadratic[2] =                 sqr(radius1/radius2) -              sqr(B.radius1/B.radius2);
		quadratic[1] =     -2*center.y*sqr(radius1/radius2) + 2*B.center.y*sqr(B.radius1/B.radius2);
		quadratic[0] = sqr(  center.y)*sqr(  radius1/  radius2) - sqr(  radius1) +
					  -sqr(B.center.y)*sqr(B.radius1/B.radius2) + sqr(B.radius1);
		num_y = solve_degree2(quadratic, y);
		//there are sure to have 2 x values for each y (situation is symmetric to y axis of ellipses)
	} else {
		//This code is based on MgcIntr2DElpElp.cpp written by David Eberly.  
		//http://svn.berlios.de/wsvn/lwpp/incubator/deeppurple/math/FreeMagic/Source/Intersection2D/MgcIntr2DElpElp.cpp
		//or http://www.geometrictools.com/LibMathematics/Intersection/Wm5IntrEllipse2Ellipse2.cpp
		//we determine the bezout determinant, which is zero only at y coordinate of intersection points
		double bezout[5];
		get_bezout_determinant(one, two, bezout);
		//Solve the Bezout determinant 
		num_y = solve_degree4(bezout, y);
	}

	int num = refine_crosspoints(num_y, y, B, one, two, r);

	_ASSERT(num<=4);
	//determine radians
	for (int i=0; i<num; i++) {
		radian_us[i] =   Point2Radian(r[i]);
		radian_b [i] = B.Point2Radian(r[i]);
	}
	return num;
}

//returns [0..1] if p is on MN, other value if not
double point2pos_straight(const XY &M, const XY&N, const XY &p)
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

int Ellipse::CrossingStraight(const XY &A, const XY &B, 
  	                          XY *r, double *radian_us, double *pos_b) const
{
	XY M = conv_to_circle_space(A);
	XY N = conv_to_circle_space(B);

	//See circle intersection with a line specificed by two points
	//http://mathworld.wolfram.com/Circle-LineIntersection.html
	double D = M.PerpProduct(N);
	XY d = N-M;
	double disc = d.length()*d.length() - D*D;
	if (disc<0) return 0; //no intersection

	XY v(d.x*sqrt(disc), fabs(d.y)*sqrt(disc));
	if (test_zero(fabs(v.x)+fabs(v.y))) return 0; //only touch
	if (d.y<0) v.x = -v.x;
	XY f(D*d.y, -D*d.x);

	//the intersect coordinates in unit circle space
	r[0] = (f+v)/d.length()/d.length();
	r[1] = (f-v)/d.length()/d.length();

	for (int i=0; i<2; i++) {
		radian_us[i] = circle_space_point2radian_curvy(r[i]);
		r[i] = conv_to_real_space(r[i]);
		//special case: horizontal line
		if (A.y==B.y) r[i].y = A.y;
		if (A.x==B.x) r[i].x = A.x;
		pos_b[i]  = point2pos_straight(A, B, r[i]);
	}
	return 2;
}

//return the number of crosspoints. 1 means a touch
int Ellipse::CrossingHorizontal(double y, double x[], double radian[]) const
{
	if (tilted) {
		XY xy[2];
		double dummy[2];
		int num = CrossingStraight(XY(0,y), XY(100,y), xy, radian, dummy);
		for (int i = 0; i<num; i++)
			x[i] = xy[i].x;
		return num;
	}
	if (y < center.y-radius2 || y > center.y+radius2) return 0;
	x[0] = center.x + radius1*sqrt(1 - sqr((y-center.y)/radius2));
	radian[0] = circle_space_point2radian_curvy(conv_to_circle_space(XY(x[0],y)));
	if (test_equal(x[0], center.x))   //just touch
		return 1;
	x[1] = 2*center.x - x[0];
	radian[1] = (radian[0]<=M_PI) ? M_PI - radian[0] : 3*M_PI - radian[0];
	return 2;
}


XY Ellipse::Tangent(double radian, bool next) const
{
	const double x = cos(radian);
	const double y = sin(radian);
	//for a point xy on the unit circle to have a point on the
	//forward tangent is: x+y, y-x
	//backward tangent is: x-y, y+x
	if (next) 
		return conv_to_real_space(XY(x-y, y+x));
	else
		return conv_to_real_space(XY(x+y, y-x));
}

//////////////////////////////////////////Edge and straight helpers


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
		return ((c.x < cx) ^ (a.y < b.y)) ? COUNTERCLOCKWISE : CLOCKWISE;
	} else {
		const double m = (a.y - b.y)/(a.x - b.x);
		double cy =  m*(c.x-a.x) + a.y; //(c.x, cy) is a point on the a-b line
		if (cy == c.y) return IN_LINE;
		return ((c.y < cy) ^ (a.x < b.x)) ? CLOCKWISE : COUNTERCLOCKWISE;
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

//checks if two straight sections are crossing or not
//return the number of points:
//0 - no crossing
//1 - one crossing point (in r[0])
//2 - the two sections intersects from r[0] to r[1]
//if any of the two sections are degenerate we return 1 only if it lies on the other section, else 0
//in pos_in_ab we return the relative pos of the crosspoint(s) in AB, in pos_in_mn for MN
//See http://softsurfer.com/Archive/algorithm_0104/algorithm_0104B.htm
int Edge::crossing_straight_straight(const XY A, const XY B, const XY M, const XY N, 
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


//helper for curvy or tilted
bool Edge::radianbetween(double r) const
{
	_ASSERT(!straight);
	r = radiannormalize(r);
	if (clockwise_arc) {
		if (s<e) return s<=r && r<=e;
		else return r>=s || r<=e;
	} else {
		if (s<e) return s>=r || r>=e;
		else return s>=r && r>=e;
	}
}

//convert from pos [0..1] to actual radian. Guaranteed between [0..2pi)
double Edge::pos2radian(double r) const
{
	_ASSERT(!straight);
	_ASSERT(0<=r && r<=1);
	double ret;
	if (clockwise_arc) {
		if (s<e) ret = s + (e-s)*r;
		else     ret = s + (e-s+2*M_PI)*r;
	} else {
		if (s<e) ret = e + (s-e+2*M_PI)*(1-r);
		else     ret = e + (s-e)*(1-r);
	} 
	if (ret-2*M_PI>0) ret = ret-2*M_PI;
	_ASSERT(0<=ret && ret<=2*M_PI);
	return ret;
}

//can return outside [0..1] if point is not between s and e
double Edge::radian2pos(double r) const
{
	_ASSERT(!straight);
	r = radiannormalize(r);
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

Edge::Edge(const XY &c, double radius_x, double radius_y, double tilt_degree) :
	ell(c, radius_x, radius_y, tilt_degree),
	s(0), e(2*M_PI), clockwise_arc(true)
{
	straight = false;
	start = ell.Radian2Point(0);
	CalculateBoundingBox();
}

void Edge::CopyInverseToMe(const Edge &B, const XY &next) 
{
	if (!B.straight) {
		ell = B.ell;
		s = B.e;
		e = B.s;
		clockwise_arc = !B.clockwise_arc;
		boundingBox = B.boundingBox;
	}
	start = next;
	straight = B.straight;
}

int Edge::Crossing(const Edge &A, const XY &B, const Edge &M, const XY &N, 
	                          XY *r, double *pos_ab, double *pos_mn) 
{
	if (A.straight && M.straight) 
		return crossing_straight_straight(A.start, B, M.start, N, r, pos_ab, pos_mn);
	int num;
	XY loc_r[8];
	double loc_ab[8], loc_mn[8];
	if (!A.straight && !M.straight) {
		//First check bounding boxes
		if (!A.boundingBox.Overlaps(M.boundingBox)) 
			return 0;
		num = A.ell.CrossingEllipse(M.ell, loc_r, loc_ab, loc_mn);
	} else if (A.straight) {
		if (!Block(A.start, B).Overlaps(M.boundingBox))
			return 0;
		num = M.ell.CrossingStraight(A.start, B, loc_r, loc_mn, loc_ab);
	/* (M.type == STRAIGHT*/ 
	} else {
		if (!Block(M.start, N).Overlaps(A.boundingBox))
			return 0;
		num = A.ell.CrossingStraight(M.start, N, loc_r, loc_ab, loc_mn);
	}
	int ret = 0;
	for (num--; num>=0; num--) {
		if (!A.straight) loc_ab[num] = A.radian2pos(loc_ab[num]);
		if (!M.straight) loc_mn[num] = M.radian2pos(loc_mn[num]);
		if (loc_ab[num]>=0 && loc_ab[num]<=1 && loc_mn[num]>=0 && loc_mn[num]<=1) {
			r[ret] = loc_r[num];
			pos_ab[ret] = loc_ab[num];
			pos_mn[ret] = loc_mn[num];
			ret++;
		}
	}
	return ret;
}

//test if the end of an arc equals to a point or not
//y coordinate must match exactly, x can be approximate
inline bool test_arc_end(const XY &a, double y, double x)
{
	return a.y==y && test_equal(a.x, x);
}

inline bool test_equal(const XY &a, const XY &b)
{
	return test_equal(a.y, b.y) && test_equal(a.x, b.x);
}

//Where does an edge or arc corss a horizontal line? (for the purposes of Polygon::IsWithin)
//1. an upward edge includes its starting endpoint, and excludes its final endpoint;
//2. a downward edge excludes its starting endpoint, and includes its final endpoint;
//SUM: we include the endpoint with the smaller y coordinate
//returns 0 if no crossing, 1/2 if there are crosspoints and -1 if it is a horizontal line.
//Internally "downward" is understood in a coordinate system where y grows downwards
int Edge::CrossingHorizontal(double y, const XY &B, double *x) const
{
	if (straight) {
		if ((start.y >= y && B.y < y) ||      //we cross upward 
			(start.y < y && B.y >= y)) {      //we cross downward
			//we cross p's line x
			x[0] = (start.x - B.x)/(start.y - B.y)*(y - B.y) + B.x;
			return 1;
		} 
		if (start.y == y && B.y == y) return -1; //horizontal line
		return 0;
	}
	double radian[2], loc_x[2];
	int loc_num = ell.CrossingHorizontal(y, loc_x, radian);
	//if the crosspoints are at the end of the segments, we check for up or downward crossing.
	if (loc_num==1) { //just touch
		if (test_arc_end(start,y,loc_x[0]) || test_arc_end(B,y,loc_x[0])) {
			//here we know either a1==s or ==e and we touch the horizontal line at y
			//if the centerpoint is above y and a1==s it is the beginning of an upward edge  => include
			//if the centerpoint is above y and a1==e it is the end of a downward edge       => include
			//if the centerpoint is below y and a1==s it is the beginning of a downward edge => exclude
			//if the centerpoint is below y and a1==e it is the end of an upward edge        => exclude
			return ell.GetCenter().y < y ? 1 : 0;
		}
		return 0; //we tocuh, but not at an endpoint: we ignore these (even if between s and e)
	}
	int num = 0;
	for (int i=0; i<loc_num; i++) {
		//if r == s (start) and forward tangent is above y, it is the beginning of an upward edge => include
		//if r == e (B)     and backwardtangent is above y, it is the end of an downward edge     => include
		//else exclude
		//This is a numerically sensitive place especially close to the endpoints.
		//With straight lines we use simple < > == operators to see if the endpoint is on y or not
		//Here we have numerical imprecision.
		//So first we see if we are close to the endpoints in an approximate manner
		//and then if yes, then we further refine the decision on endpoint using > < operators
		//and then we see if this is an upward or downward edge
		if (test_equal(radian[i], s)) {                       //OK we treat this as a crosspoint
			if (test_arc_end(start,y,loc_x[i]) &&             //but if *exactly* on the startpoint...
				ell.Tangent(radian[i], clockwise_arc).y > y)  //...and fw tangent below: exclude
				continue;
		} else if (test_equal(radian[i], e)) {                //OK we treat this as a crosspoint
			if (test_arc_end(B,y,loc_x[i]) &&                 //but if *exactly* on the endpoint...
				ell.Tangent(radian[i], !clockwise_arc).y > y)//...and bw tangent below: exclude
				continue;
		} else if (!radianbetween(radian[i]))  //exclude also, if not close to end and not between s&e
			continue;
		x[num++] = loc_x[i]; //include 
	}
	return num; 
}
//Adds this point (for straight) or full curve to a bounding box
Block& Edge::CalculateBoundingBox() 
{
	boundingBox.MakeInvalid();
	if (straight)
		//add startpoint
		return boundingBox += start;
	if (ell.IsTilted()) {
		//Add the biggest rectangle a tilted ellipse can fit
		const double r = std::max(ell.GetRadius1(), ell.GetRadius2());
		boundingBox += (ell.GetCenter()-XY(r,r));
		boundingBox += (ell.GetCenter()+XY(r,r));
	} else {
		//if any extreme point is included in the arc add them
		if (radianbetween(       0)) boundingBox += (ell.GetCenter()+XY(ell.GetRadius1(),0));
		if (radianbetween(0.5*M_PI)) boundingBox += (ell.GetCenter()+XY(0, ell.GetRadius2()));
		if (radianbetween(    M_PI)) boundingBox += (ell.GetCenter()-XY(ell.GetRadius1(),0));
		if (radianbetween(1.5*M_PI)) boundingBox += (ell.GetCenter()-XY(0, ell.GetRadius2()));
		//add endpoint
		boundingBox += ell.Radian2Point(e);
	}
	return boundingBox;
}

//Checks if this->next edge and next->after can be combined into a single edge
//if so, also update this such that this->after will be the combined edge
bool Edge::CheckAndCombine(const Edge &next, const XY &after)
{
	if (straight != next.straight) return false;
	if (straight) {
		const double a = angle(start, next.start, after);
		return test_zero(a) || test_equal(a,2);
	}
	//calc tangent for curvy edges
	if (ell != next.ell || clockwise_arc != next.clockwise_arc) return false;
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

void Edge::Path(cairo_t *cr, const XY &next, bool reverse) const
{
	if (straight) {
		if (reverse)
			cairo_line_to(cr, start.x, start.y);
		else
			cairo_line_to(cr, next.x, next.y);
		return;
	}
	cairo_save(cr);
	ell.TransformForDrawing(cr);
	if (reverse) {
		if (clockwise_arc)
			cairo_arc_negative(cr, 0, 0, 1, e, s);
		else
			cairo_arc(cr, 0, 0, 1, e, s);
	} else {
		if (clockwise_arc)
			cairo_arc(cr, 0, 0, 1, s, e);
		else
			cairo_arc_negative(cr, 0, 0, 1, s, e);
	}
	cairo_restore(cr);
}

}; //namespace


//int Edge::CrossingHorizontal2(double y, const XY &B, double *x) const
//{
//	if (straight) {
//		if (test_smaller(start.y, B.y)) {      //downward edge: include startpoint
//			if (test_smaller(y, start.y) || !test_smaller(B.y, y)) 
//				//y<start.y-epsilon || B.y-epsilon <= y
//				//either not enough outside on start side or too outside on B side
//				return 0; 
//			if (test_equal(start.y, y)) {  //preserve exact x coordinate
//				x[0] = start.x;
//				return 1;
//			}
//		} else if (test_smaller(B.y, start.y)) {      //upward edge
//			if (test_smaller(y, B.y) || !test_smaller(start.y, y)) 
//				//y<B.y-epsilon || start.y-epsilon <= y
//				//either not enough outside on start side or too outside on B side
//				return 0; 
//			if (test_equal(B.y, y)) {  //preserve exact x coordinate
//				x[0] = B.x;
//				return 1;
//			}
//		} else //start and endpoint are almost the same
//			return test_equal(start.y, y) ? -1 : 0;
//		//we have a point inside the section
//		x[0] = (start.x - B.x)/(start.y - B.y)*(y - B.y) + B.x;
//		return 1;
//	}
//	double radian[2], loc_x[2];
//	int loc_num = ell.CrossingHorizontal(y, loc_x, radian);
//	//if the crosspoints are at the end of the segments, we check for up or downward crossing.
//	if (loc_num==1) { //just touch
//		if (test_equal(start,XY(y,loc_x[0])) || test_equal(B,XY(y,loc_x[0]))) {
//			//here we know either a1==s or ==e and we touch the horizontal line at y
//			//if the centerpoint is above y and a1==s it is the beginning of an upward edge  => include
//			//if the centerpoint is above y and a1==e it is the end of a downward edge       => include
//			//if the centerpoint is below y and a1==s it is the beginning of a downward edge => exclude
//			//if the centerpoint is below y and a1==e it is the end of an upward edge        => exclude
//			return ell.GetCenter().y < y ? 1 : 0;
//		}
//		return 0; //we tocuh, but not at an endpoint: we ignore these (even if between s and e)
//	}
//	int num = 0;
//	for (int i=0; i<loc_num; i++) {
//		//if r == s (start) and forward tangent is above y, it is the beginning of an upward edge => include
//		//if r == e (B)     and backwardtangent is above y, it is the end of an downward edge     => include
//		//else exclude
//		if (test_equal(start, XY(loc_x[i], y))) {              //if on the startpoint...
//			if (ell.Tangent(radian[i], clockwise_arc).y <= y)   //...and fw tangent above: include
//				x[num++] = start.x;                            //preserve crosspoint coordinates
//		} else if (test_equal(B, XY(loc_x[i], y))) {           //if on the startpoint...
//			if (ell.Tangent(radian[i], !clockwise_arc).y <= y)  //...and bw tangent below: include
//				x[num++] = B.x;                                //preserve crosspoint coordinates
//		} else if (radianbetween(radian[i]))                   //inlcude also, if not close to ends but between s&e
//			x[num++] = loc_x[i]; 
//	}
//	return num; 
//}
//
