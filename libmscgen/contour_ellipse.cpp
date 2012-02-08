/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011,2012 Zoltan Turanyi
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
#include "contour_ellipse.h"

namespace contour {

//////////////////Helper functions

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
            return -1;
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

    if ( fDiscr > 0.0f ) { // 1 real, 2 complex roots
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
    } else if ( fDiscr < 0.0f ) {  //3 real roots
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
    /*int iResolveCount = */ solve_degree3(kResolve, afResolveRoot);
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
                      afCoeff[3]*afCoeff[3]*afCoeff[3]) / (4.0f*fR);

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

//arc-length in the clockwise dir (msc space!) from s to e
inline double radianspan(double s, double e)
{
    s = fmod_negative_safe(s, 2*M_PI);
    e = fmod_negative_safe(e, 2*M_PI);
    if (s<=e) return e-s;
    return 2*M_PI - (s-e);
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
    quadratic_xy_t(const EllipseData &arc);
};

quadratic_xy_t::quadratic_xy_t(const EllipseData &arc)
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
//rets LINE_CROSSING_PARALLEL if parallel
//rets LINE_CROSSING_OUTSIDE if the crosspoint is outside M-N.
//rets LINE_CROSSING_INSIDE if the crosspoint is within (M-N). (We assume in calling function that it is inside A-B, too.)
ELineCrossingType crossing_line_line(const XY &A, const XY &B, const XY &M, const XY &N,  XY &r)
{
	const double perp = (B-A).PerpProduct(N-M);
    if (test_zero(perp)) return LINE_CROSSING_PARALLEL;
    //They are not parallel (and none of them are degenerate)
    const double t = (B-A).PerpProduct(A-M) / perp;
    r = M + (N-M)*t;
    return between01_approximate_inclusive(t) ? LINE_CROSSING_INSIDE : LINE_CROSSING_OUTSIDE;
}

//refines the location of a point using crosspoints of tangents
bool EllipseData::refine_point(const EllipseData &B, XY &p) const
{
    int max_itr = 32;
    XY p_orig = p;
    while (--max_itr>=0) {
        XY c1 =   conv_to_circle_space(p);
        XY c2 = B.conv_to_circle_space(p);
        const double closeness = fabs(c1.length()-1) + fabs(c2.length()-1);
        if (closeness<1e-30)
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
        if (crossing_line_line(A1, A2, B1, B2, p_new) == LINE_CROSSING_PARALLEL)
            return false; //no intersection. Lines are parallel two ellipses, we drop this
        if ((p-p_new).length_sqr()<1e-30)
            return true; //no improvement, exit
        p = p_new;
    }
    return true;
}

//refines the location of a point using crosspoints of tangents
bool EllipseData::refine_point(const XY &A, const XY &B, XY &p) const
{
    int max_itr = 32;
    while (--max_itr>=0) {
        XY c1 =   conv_to_circle_space(p);
        const double closeness = fabs(c1.length()-1);
        if (closeness<1e-30)
            return true;
        const double r1 = circle_space_point2radian_curvy(c1);
        const XY A1 =   conv_to_real_space(XY(cos(r1)        , sin(r1)        ));
        //A1 is a point on the ellipse closest to p
        const XY A2 =   conv_to_real_space(XY(cos(r1)+sin(r1), sin(r1)-cos(r1)));
        //Now A1-A2 is a tangent of the ellipse
        //We operate on the assumption that the intersection of two tangents is closer to the
        //intersection of the ellipses
        XY p_new;
        if (crossing_line_line(A1, A2, A, B, p_new) == LINE_CROSSING_PARALLEL)
            return true; //no intersection. We must be done...
        if ((p-p_new).length_sqr()<1e-30)
            return true; //no improvement, exit
        p = p_new;
    }
    return true;
}

//take the (at most)
int EllipseData::refine_crosspoints(int num_y, double y[], const EllipseData &B,
                                const quadratic_xy_t &one, const quadratic_xy_t &/*two*/, XY p[]) const
{
    // Adjustment for quadratics to allow for relative error testing.
    //const double fNorm_one = one.A*one.A + 2.0f*one.B*one.B + one.C*one.C;
    //const double fNorm_two = two.A*two.A + 2.0f*two.B*two.B + two.C*two.C;

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
            if (refine_point(B, points[num_tmp]))
                num_tmp++;
        }
    }
    if (num_tmp==0) return 0;
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

inline void EllipseData::transpose_curvy_non_tilted()
{
    _ASSERT(!tilted);
    std::swap(center.x, center.y);
    std::swap(radius1, radius2);
}

void EllipseData::calculate_extremes()
{
    if (tilted) {
        //this is the radian at which the x coordinate is
        const double ry = atan( (radius2/radius1)*(costilt/sintilt));
        const double rx = atan( (radius2/radius1)/(costilt/sintilt));
        extreme[0] = Radian2Point(extreme_radian[0] = 2.0*M_PI-rx);  //leftmost
        extreme[1] = Radian2Point(extreme_radian[1] =     M_PI-rx);  //rightmost
        extreme[2] = Radian2Point(extreme_radian[2] =     M_PI+ry);  //topmost
        extreme[3] = Radian2Point(extreme_radian[3] =          ry);  //bottommost
    } else {
        extreme[0].x = center.x - radius1;
        extreme[0].y = center.y;
        extreme_radian[0] = M_PI;
        extreme[1].x = center.x + radius1;
        extreme[1].y = center.y;
        extreme_radian[1] = 0;
        extreme[2].y = center.y - radius2;
        extreme[2].x = center.x;
        extreme_radian[2] = M_PI*1.5;
        extreme[3].y = center.y + radius2;
        extreme[3].x = center.x;
        extreme_radian[3] = M_PI*0.5;
    }
}

double EllipseData::add_to_tilt(double cos, double sin, double radian)
{
    _ASSERT(radian>=0 && radian<=2*M_PI);
	if (radius1==radius2) return radian;
    const double old_tilt = tilted ? tilt : 0;
	if (tilted)
		tilt += radian;
	else
		tilt = radian;
	if (tilt>=2*M_PI) tilt-=2*M_PI;
    if (test_equal(fmod_negative_safe(tilt, M_PI) ,0)) {
		tilted = false;
	} else
        if (test_equal(fmod_negative_safe(tilt, M_PI), M_PI/2)) {
        tilted = false;
        std::swap(radius1, radius2);
    } else if (tilted) { //already tilted and remains so
        double c = costilt;
        costilt = c*cos + sintilt*sin;
        sintilt = -c*sin + sintilt*cos;
    } else {
        tilted=true;
        costilt = cos;
        sintilt = sin;
    }
    const double new_tilt = tilted ? tilt : 0;
    return fmod_negative_safe(radian-(new_tilt-old_tilt), 2*M_PI);
	_ASSERT((tilt>=0 && tilt<2*M_PI) || !tilted);
}

EllipseData::EllipseData(const XY &c, double radius_x, double radius_y, double tilt_degree) :
    center(c), radius1(fabs(radius_x)), radius2(fabs(radius_y)), tilted(false), perimiter_cache(-1)
{
    if (radius2 == 0) radius2 = radius1; //circle
    if (radius1 != radius2 && tilt_degree!=0) {
        tilt_degree -= floor(tilt_degree/180)*180;
        if (tilt_degree>=90) {
            tilt_degree -= 90;
            std::swap(radius1, radius2);
        }
        if (tilt_degree) {
            tilt = deg2rad(tilt_degree);
            sintilt = sin(tilt);
            costilt = cos(tilt);
            tilted = true;
        }
    }
	calculate_extremes();
}

void EllipseData::Shift(const XY &xy)
{
    center += xy;
    for (int i=0; i<4; i++)
        extreme[i] += xy;
}

//returns how much radians modify
double EllipseData::Rotate(double cos, double sin, double radian)
{
    center.Rotate(cos, sin);
    double ret = add_to_tilt(cos, sin, radian);
    calculate_extremes();
    return ret;
}

//returns how much radians modify
double EllipseData::RotateAround(const XY&c, double cos, double sin, double radian)
{
    center.RotateAround(c, cos, sin);
    double ret = add_to_tilt(cos, sin, radian);
    calculate_extremes();
    return ret;
}

void EllipseData::SwapXY()
{
    center.SwapXY();
    if (!IsTilted())
        std::swap(radius1, radius2);
    else {
        tilt = M_PI/2 - tilt; //mirror on 45 degrees
        std::swap(sintilt, costilt);
    }
}

//Return -1 if the two ellipses are equal
int EllipseData::CrossingEllipse(const EllipseData &B, XY r[], double radian_us[], double radian_b[]) const
{
    //Now this is scary shit. Above there is a suite to solve 4th degree equations, we use those
    //we are interested only in real solutions
    //for ellipses see http://www.geometrictools.com/Documentation/IntersectionOfEllipses.pdf

    //First check if the two ellipses are the same or fully contain one another
    if (center == B.center) { //same center
        if (std::min(radius1, radius2) > std::max(B.radius1, B.radius2) ||
            std::max(radius1, radius2) < std::min(B.radius1, B.radius2))
            return 0; //one is fully in the other (both radiuses smaller than any of the other)
        if (tilted == B.tilted && (!tilted || tilt == B.tilt)) //same tilt
            if ((radius1 == B.radius1 && radius2 == B.radius2) ||
                (radius1 == B.radius2 && radius2 == B.radius1))
                return -1; //equal radiuses - the two ellipses are the same
            if ((radius1<B.radius1 && radius2<B.radius2) ||
                (radius1>B.radius1 && radius2>B.radius2))
                return 0; //one fully in the other
    }

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
            EllipseData p1(*this), p2(B);
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
    if (M.test_equal(N)) {
        _ASSERT(M.test_equal(p));
        return 0;
    }
    if (fabs(M.x-N.x) > fabs(M.y-N.y)) {
        double t = (p.x-M.x)/(N.x-M.x);
        if (test_equal(p.y, M.y + (N.y-M.y)*t)) return t;
    } else {
        double t = (p.y-M.y)/(N.y-M.y);
        if (test_equal(p.x, M.x + (N.x-M.x)*t)) return t;
    }
    _ASSERT(0);
    return -1;
}

int EllipseData::CrossingStraight(const XY &A, const XY &B,
  	                          XY *r, double *radian_us, double *pos_b) const
{
    const XY M = conv_to_circle_space(A);
    const XY N = conv_to_circle_space(B);

    //See circle intersection with a line specificed by two points
    //http://mathworld.wolfram.com/Circle-LineIntersection.html
    double D = M.PerpProduct(N);
    const XY d = N-M;
    double disc = d.length()*d.length() - D*D;
    if (disc<0) {
        //no intersection, check if it almost touches
        const double dist = fabs((N.x-M.x)*(M.y-0) - (M.x-0)*(N.y-M.y))/(N-M).length();
        if (test_equal(dist,1)) disc=0;
        else return 0;
    }

    const XY v((d.y<0 ? -d.x : d.x)*sqrt(disc), fabs(d.y)*sqrt(disc));
    const XY f(D*d.y, -D*d.x);
    int num;
    if (test_zero(disc)) { //only touch
        r[0] = f/d.length()/d.length();
        num = 1;
    } else {
        //the intersect coordinates in unit circle space
        r[0] = (f+v)/d.length()/d.length();
        r[1] = (f-v)/d.length()/d.length();
        num = 2;
    }


    for (int i=0; i<num; i++) {
        r[i] = conv_to_real_space(r[i]);
        refine_point(A, B, r[i]);
        radian_us[i] = Point2Radian(r[i]);
        //special case: horizontal line
        if (A.y==B.y) r[i].y = A.y;
        if (A.x==B.x) r[i].x = A.x;
        pos_b[i]  = point2pos_straight(A, B, r[i]);
    }
    return num;
}

//return the number of crosspoints. 1 means a touch
int EllipseData::CrossingVertical(double x, double y[], double radian[]) const
{
    if (tilted) {
        XY xy[2];
        double dummy[2];
        int num = CrossingStraight(XY(x,0), XY(x, 100), xy, radian, dummy);
        for (int i = 0; i<num; i++)
            y[i] = xy[i].y;
        return num;
    }
    if (x < center.x-radius1 || x > center.x+radius1) return 0; //outside
    y[0] = center.y + radius2*sqrt(1 - sqr((x-center.x)/radius1)); //y coord of one cp
    radian[0] = circle_space_point2radian_curvy(conv_to_circle_space(XY(x,y[0])));
    if (test_equal(y[0], center.y))   //just touch
        return 1;
    y[1] = 2*center.y - y[0];  //the other cp is mirrored (we are !tilted)
    radian[1] = 2*M_PI - radian[0];
    return 2;
}


XY EllipseData::Tangent(double radian, bool next) const
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

inline double gk(double h) 
{
    double z = 0, x = 1;
    unsigned n = 0;
    while (!test_zero(x)) {
        n++;
        x = h * x * ((n-1.5)/n)*((n-1.5)/n);
        z += x;
    }
    return z+1;
}

inline double cayley(double x) 
{
    double y = log(16/x) - 1;
    double t = x / 4;
    unsigned n = 1;
    double z = 0;
    double u = t * y;
    double v = (n - .5) / n;
    double w = .5 / ((n - .5) * n);
    while (!test_zero(u)) {
        z += u;
        n++;
        t *= x*v;
        v = (n - .5) / n;
        t *= v;
        y -= w;
        w = .5 / ((n - .5) * n);
        y -= w;
        u = t * y;
    }
    return 1 + z;
}


void EllipseData::CalcPerimiterEllipse() const
{
    //This is copied from http://www.numericana.com/answer/ellipse.htm#high
    const double a = std::max(radius1, radius2);
    const double b = std::min(radius1, radius2);
    if (b >= 0.28*a) {
        const double h = ((a-b)/(a+b))*((a-b)/(a+b));
        perimiter_cache = M_PI*(a+b)*gk(h);
    } else 
        perimiter_cache = 4*a*cayley((b/a)*(b/a));
}



//This returns the length of the 0->to arc (always positive)
//Here a computation of incomplete elliptic integral of the second kind is needed
//See http://www.numericana.com/answer/geometry.htm#ellipticarc
double EllipseData::PerimiterHelper(double to) const
{
    unsigned num_of_quarters;
    double a, b;
    if (radius1 < radius2) {
        a = radius2;
        b = radius1;
        if (to <= 0.5*M_PI) num_of_quarters = 0;
        else if (to <= 1.5*M_PI) num_of_quarters = 2;
        else num_of_quarters = 4;
    } else {
        a = radius1;
        b = radius2;
        if (to <= M_PI) num_of_quarters = 1;
        else num_of_quarters = 3;
    }
    to -= num_of_quarters*M_PI/2;
    //Now num_of_quarters contains how many quarters of the ellipse we have from 0 to that 
    //end of the shorter axis, which is closer to the original "to".
    //Now "to" contains a (-pi/2..+pi/2) range showing a diff from the end of the short axis
    //we need to calculate the length or arc from the end of short axis till "fabs(to)" and add 
    //to the quarters if "to" is pos, and substract it if "to" is negative
    //The below formula is by David W. Cantrell from 2002 see in the Internet Archive
    //http://web.archive.org/web/20030225001402/http://mathforum.org/discuss/sci.math/t/469668
    const double L = a * (sin(fabs(to)) + (fabs(to) - sin(fabs(to)))*pow(b/a, 2 - 0.216*fabs(to)*fabs(to)));
    return num_of_quarters*Perimiter()/4 + (to<0 ? -L : L);
}

int EllipseData::Expand(double gap)
{
    perimiter_cache = -1; //invalidate perimiter cache
    int ret = 1;
    radius1+=gap;
    if (!test_smaller(0,radius1)) ret--;
    radius2+=gap;
    if (!test_smaller(0,radius2)) ret--;
    if (ret>=0) calculate_extremes();
    return ret;
}

double EllipseData::OffsetBelow(const EllipseData&) const
{
    return 0;
}

double EllipseData::OffsetBelow(const XY&, const XY&) const
{

    return 0;
}

double EllipseData::OffsetAbove(const XY&, const XY&) const
{
    return 0;
}

} //namespace
