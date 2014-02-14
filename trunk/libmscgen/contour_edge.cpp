/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011,2012,2013 Zoltan Turanyi
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

/** @file contour_edge.cpp Defines Edge.
 * @ingroup contour_files
 */


#include <cassert>
#include <vector>
#include <algorithm>
#include "contour_edge.h"

namespace contour {

//////////////////HELPERS

    /** Solve quadratic equation.
*
* @param [in] m_afCoeff Parameters of the equation. `m_afCoeff[0]` is the constant, `m_afCoeff[2]` is the coeff of `x^2`.
* @param [out] afRoot Returns the root(s).
* @returns the number of roots [0..2].
*/
unsigned solve_degree2(double afCoeff[3], double afRoot[2])
{
    // compute real roots to c[2]x^2+c[1]*x+c[0] = 0
    if (afCoeff[2] == 0) {
        //linear
        if (test_zero(afCoeff[1]))
            return 0;
        afRoot[0] = -afCoeff[0]/afCoeff[1];
        return 1;
    }

    // make polynomial monic
    if (afCoeff[2] != 1.) {
        afCoeff[0] = afCoeff[0]/afCoeff[2];
        afCoeff[1] = afCoeff[1]/afCoeff[2];
    }

    double fDiscr = afCoeff[1]*afCoeff[1]-4.*afCoeff[0];
    if (test_zero(fDiscr)) {
        afRoot[0] = 0.5*(-afCoeff[1]);
        return 1;
    }

    if (fDiscr >= 0.) {
        fDiscr = sqrt(fDiscr);
        afRoot[0] = 0.5*(-afCoeff[1]-fDiscr);
        afRoot[1] = 0.5*(-afCoeff[1]+fDiscr);
        return 2;
    }
    return 0;
}

/** Solve qubic equation.
*
* @param [in] m_afCoeff Parameters of the equation. `m_afCoeff[0]` is the constant, `m_afCoeff[3]` is the coeff of `x^3`.
* @param [out] afRoot Returns the root(s).
* @returns the number of roots [1..3].
*/
unsigned solve_degree3(double afCoeff[4], double afRoot[3])
{
    //one can also consider this page
    //http://www.pouet.net/topic.php?which=9119&page=1

    // compute real roots to c[3]*x^3+c[2]*x^2+c[1]*x+c[0] = 0
    if (afCoeff[3] == 0)
        return solve_degree2(afCoeff, afRoot);

    // make polynomial monic
    if (afCoeff[3] != 1.) {
        afCoeff[0] = afCoeff[0]/afCoeff[3];
        afCoeff[1] = afCoeff[1]/afCoeff[3];
        afCoeff[2] = afCoeff[2]/afCoeff[3];
    }

    // convert to y^3+a*y+b = 0 by x = y-c[2]/3 and
    const double fA = (1./3.)*(3.*afCoeff[1]-afCoeff[2]*afCoeff[2]);
    const double fB = (1./27.)*(2.*afCoeff[2]*afCoeff[2]*afCoeff[2] -
        9.*afCoeff[1]*afCoeff[2]+27.*afCoeff[0]);
    const double fOffset = (1./3.)*afCoeff[2];

    double fDiscr = 0.25*fB*fB + (1./27.)*fA*fA*fA;
    const double fHalfB = 0.5*fB;
    if (fabs(fDiscr)<1e-10) {    //3 real roots, but at least two are equal
        double fTemp;
        if (fHalfB >= 0.)
            fTemp = -pow(fHalfB, double(1./3.));
        else
            fTemp = pow(-fHalfB, double(1./3.));
        if (test_zero(fTemp)) {
            afRoot[0] = -fOffset;
            return 1;
        }
        afRoot[0] = 2.*fTemp-fOffset;
        afRoot[1] = -fTemp-fOffset;
        return 2;
    }

    if (fDiscr > 0.) { // 1 real, 2 complex roots
        fDiscr = sqrt(fDiscr);
        double fTemp = -fHalfB + fDiscr;
        if (fTemp >= 0.)
            afRoot[0] = pow(fTemp, double(1./3.));
        else
            afRoot[0] = -pow(-fTemp, double(1./3.));
        fTemp = -fHalfB - fDiscr;
        if (fTemp >= 0.)
            afRoot[0] += pow(fTemp, double(1./3.));
        else
            afRoot[0] -= pow(-fTemp, double(1./3.));
        afRoot[0] -= fOffset;
        return 1;
    } else if (fDiscr < 0.) {  //3 real roots
        double fDist = sqrt(-1./3.*fA);
        double fAngle = (1./3.)*atan2(sqrt(-fDiscr), -fHalfB);
        double fCos = cos(fAngle);
        double fSin = sin(fAngle);
        static const double sqrt3 = sqrt(3.);
        afRoot[0] = 2.*fDist*fCos-fOffset;
        afRoot[1] = -fDist*(fCos+sqrt3*fSin)-fOffset;
        afRoot[2] = -fDist*(fCos-sqrt3*fSin)-fOffset;
        return 3;
    }

    return true;
}

/** Solve quadratic equation.
*
* Based on David Eberly's code at
* <http://svn.berlios.de/wsvn/lwpp/incubator/deeppurple/math/FreeMagic/Source/Core/MgcPolynomial.cpp>
* @param [in] m_afCoeff Parameters of the equation. `m_afCoeff[0]` is the constant, `m_afCoeff[4]` is the coeff of `x^4`.
* @param [out] afRoot Returns the root(s).
* @returns the number of roots [0..4]
*/
unsigned solve_degree4(double afCoeff[5], double afRoot[4])
{
    // compute real roots to c[4]*x^4+c[3]*x^3+c[2]*x^2+c[1]*x+c[0] = 0
    if (afCoeff[4] == 0)
        return solve_degree3(afCoeff, afRoot);

    // make polynomial monic
    if (afCoeff[4] != 1.) {
        afCoeff[0] = afCoeff[0]/afCoeff[4];
        afCoeff[1] = afCoeff[1]/afCoeff[4];
        afCoeff[2] = afCoeff[2]/afCoeff[4];
        afCoeff[3] = afCoeff[3]/afCoeff[4];
    }

    // reduction to resolvent cubic polynomial
    double kResolve[4];
    kResolve[3] = 1.;
    kResolve[2] = -afCoeff[2];
    kResolve[1] = afCoeff[3]*afCoeff[1]-4.*afCoeff[0];
    kResolve[0] = -afCoeff[3]*afCoeff[3]*afCoeff[0] +
        4.*afCoeff[2]*afCoeff[0]-afCoeff[1]*afCoeff[1];
    double afResolveRoot[3];
    /*int iResolveCount = */ solve_degree3(kResolve, afResolveRoot);
    double fY = afResolveRoot[0];

    unsigned num = 0;
    double fDiscr = 0.25*afCoeff[3]*afCoeff[3]-afCoeff[2]+fY;
    if (test_zero(fDiscr)) {
        double fT2 = fY*fY-4.*afCoeff[0];
        if (test_positive(fT2)) {
            if (fT2 < 0.) // round to zero
                fT2 = 0.;
            fT2 = 2.*sqrt(fT2);
            double fT1 = 0.75*afCoeff[3]*afCoeff[3]-2.*afCoeff[2];
            if (test_positive(fT1+fT2)) {
                double fD = sqrt(fT1+fT2);
                afRoot[0] = -0.25*afCoeff[3]+0.5*fD;
                afRoot[1] = -0.25*afCoeff[3]-0.5*fD;
                num = 2;
            }
            if (test_positive(fT1-fT2)) {
                double fE = sqrt(fT1-fT2);
                afRoot[num++] = -0.25*afCoeff[3]+0.5*fE;
                afRoot[num++] = -0.25*afCoeff[3]-0.5*fE;
            }
        }
        return num;
    }

    if (fDiscr > 0.) {
        double fR = sqrt(fDiscr);
        double fT1 = 0.75*afCoeff[3]*afCoeff[3]-fR*fR-2.*afCoeff[2];
        double fT2 = (4.*afCoeff[3]*afCoeff[2]-8.*afCoeff[1]-
            afCoeff[3]*afCoeff[3]*afCoeff[3]) / (4.*fR);

        double fTplus = fT1+fT2;
        if (test_zero(fTplus)) {
            afRoot[0] = -0.25*afCoeff[3]+0.5*fR;
            num = 1;
        } else if (fTplus >= 0.) {
            double fD = sqrt(fTplus);
            afRoot[0] = -0.25*afCoeff[3]+0.5*(fR+fD);
            afRoot[1] = -0.25*afCoeff[3]+0.5*(fR-fD);
            num = 2;
        }
        double fTminus = fT1-fT2;
        if (test_zero(fTminus))
            fTminus = 0.;
        if (fTminus >= 0.) {
            double fE = sqrt(fTminus);
            afRoot[num++] = -0.25*afCoeff[3]+0.5*(fE-fR);
            afRoot[num++] = -0.25*afCoeff[3]-0.5*(fE+fR);
        }
        return num;
    }

    //if ( fDiscr < 0. )
    return 0;
}

/////////////////Edge


/** Splits to two bezier curves 
 * The first will be start, ret, Mid(start, c1, t), r1
 * The second will be ret, end, r2, Mid(c2, end, t) */
XY Edge::Split(double t, XY &r1, XY &r2) const
{
    if (straight) {
        r1 = start;
        r2 = end;
    } else {
        const XY C = Mid(c1, c2, t);
        r1 = Mid(Mid(start, c1, t), C, t);
        r2 = Mid(C, Mid(c2, end, t), t);
    }
    return Mid(r1, r2, t);
}

/** Splits to two bezier curves at t=0.5
* The first will be start, ret, Mid(start, c1, t);, r1,
* The second will be ret, end, r2, Mid(c2, end, t); */
XY Edge::Split(XY &r1, XY &r2) const
{
    if (straight) {
        r1 = start;
        r2 = end;
    } else {
        const XY C = (c1+c2)/2;
        r1 = ((start+c1)/2 + C)/2;
        r2 = (C+(c2+end)/2)/2;
    }
    return (r1+r2)/2;
}


const double flatness_tolerance = 0.001;


void Edge::Split(Edge &r1, Edge &r2) const
{
    r1.start = start;
    r2.end = end;
    r2.start = r1.end = Split(r1.c2, r2.c1);
    r1.c1 = (start+c1)/2;
    r2.c2 = (c2+end)/2;
    r1.visible = r2.visible = visible;
    r1.straight = r1.start.test_equal(r1.end) || r1.Flatness()<flatness_tolerance*flatness_tolerance*16;
    r2.straight = r2.start.test_equal(r2.end) || r2.Flatness()<flatness_tolerance*flatness_tolerance*16;
}

void Edge::Split(double t, Edge &r1, Edge &r2) const
{
    r1.start = start;
    r2.end = end;
    r2.start = r1.end = Split(t, r1.c2, r2.c1);
    r1.c1 = Mid(start, c1, t);
    r2.c2 = Mid(c2, end, t);
    r1.visible = r2.visible = visible;
    r1.straight = r1.start.test_equal(r1.end) || r1.Flatness()<flatness_tolerance*flatness_tolerance*16;
    r2.straight = r2.start.test_equal(r2.end) || r2.Flatness()<flatness_tolerance*flatness_tolerance*16;
}

bool Edge::Chop(double t, double s)
{
    _ASSERT(t>=0 && s>=0 && t<=1 && s<=1);
    if (s<t) std::swap(s, t);
    XY r1, r2;
    if (straight) {
        r1 = (s<1) ? start+(end-start)*s : end;
        if (t) start = start + (end-start)*t;
        end = r1;
        return t==s;
    }
    if (s<1) {
        end = Split(s, r1, r2);
        c1 = Mid(start, c1, s);
        c2 = r1;
        t /= s;
    }
    if (t>0) {
        start = Split(t, r1, r2);
        c1 = r2;
        c2 = Mid(c2, end, t);
    }
    return t==s;
}

/** Check if two segments cross
 * returns 0 if none, 1 if yes, 2 if they are rectilinear and overlap.
 * If they cross t will be the position of the crosspoint on A1->A2, s will be on B1->B2.
 * If they are rectilinear, t and s will be the pos of B1 and B2 on A1->A2 */
int Cross(const XY &A1, const XY &A2, const XY &B1, const XY &B2, double &t, double &s)
{
    if (B1.test_equal(B2)) {
        if (A1.test_equal(A2)) {
            s = t = 0;
            return B1.test_equal(A1) ? 1 : 0;
        }
        s = t = fabs(A2.x-A1.x) > fabs(A2.y-A1.y) ? 
                    (B1.x-A1.x)/(A2.x-A1.x) : 
                    (B1.y-A1.y)/(A2.y-A1.y);
        return 0<=t && t<=1 ? 1 : 0;
    }
    if (A1.test_equal(A2)) {
        t = s = fabs(B2.x-B1.x) > fabs(B2.y-B1.y) ? 
                    (A1.x-B1.x)/(B2.x-B1.x) : 
                    (A1.y-B1.y)/(B2.y-B1.y);
        return 0<=s && s<=1 ? 1 : 0;
    }
    const double denom = ((B2.y-B1.y)*(A2.x-A1.x) - (B2.x-B1.x)*(A2.y-A1.y));
    if (test_zero(denom)) {
        //they are parallel t and s will be positions of B1 and B2 on A1->A2 segment
        t = fabs(A2.x-A1.x) > fabs(A2.y-A1.y) ?
                (B1.x-A1.x)/(A2.x-A1.x) :
                (B1.y-A1.y)/(A2.y-A1.y);
        s = fabs(A2.x-A1.x) > fabs(A2.y-A1.y) ?
                (B2.x-A1.x)/(A2.x-A1.x) :
                (B2.y-A1.y)/(A2.y-A1.y);
        return ((t<0 && s<0) || (t>1 && s>1)) ? 0 : 2;
    }
    s = ((B1.x-A1.x)*(A2.y-A1.y) - (B1.y-A1.y)*(A2.x-A1.x)) / denom;
    if (s<0 || s>1) return 0;
    t = fabs(A2.x-A1.x) > fabs(A2.y-A1.y) ? 
            (s*(B2.x-B1.x) + B1.x-A1.x)/(A2.x-A1.x) : 
            (s*(B2.y-B1.y) + B1.y-A1.y)/(A2.y-A1.y);
    return 0<=t && t<=1 ? 1 : 0;
}


/** Check if two segments cross
* returns 0 if none, 1 if yes, 2 if they are rectilinear and overlap.
* 0 - no crossing
* 1 - one crossing point (in r[0])
* 2 - the two sections intersects from r[0] to r[1]
* if any of the two sections are degenerate we return 1 only if it lies on the other section, else 0
* in pos_in_ab we return the relative pos of the crosspoint(s) in AB, in pos_in_mn for MN
* See http://softsurfer.com/Archive/algorithm_0104/algorithm_0104B.htm
* crossing points that result in a pos value close to 1 are ignored
* In case AB is the same (or lies on the same line) as MN, the two endpoint of the common
* sections are returned (if they overlap or touch)
*/
unsigned Edge::CrossingSegments(const Edge &o, XY r[], double pos_my[], double pos_other[]) const
{
    _ASSERT(straight && o.straight);
    if (!test_zero((end-start).PerpProduct(o.end-o.start))) {
        //They are not parallel (and none of them are degenerate, but that does not matter now)
        double t = (end-start).PerpProduct(start-o.start) / (end-start).PerpProduct(o.end-o.start);
        if (!between01_adjust(t)) return 0;
        double s = (o.end-o.start).PerpProduct(start-o.start) / (end-start).PerpProduct(o.end-o.start);
        if (!between01_adjust(s)) return 0; //the intersection of the line is outside AB
        r[0] = o.start + (o.end-o.start)*t;
        pos_other[0] = t;
        pos_my[0] = s;
        return 1;
    }
    //either they are parallel or one or both sections are degenerate (= a point)
    if (o.start==o.end) {
        if (!test_zero((o.start-start).PerpProduct(end-start)))
            return 0; //o.start is not on AB's line and AB is not degenerate
        if (start==end) {
            if (start==o.start) {
                r[0] = o.start;
                pos_other[0] = 0;
                pos_my[0] = 0;
                return 1;
            }
            return 0; //both sections degenerate to a point, but do not overlap
        }
        double s;
        //select x or y depending on precision
        //o.start lies on AB, let us see if between start and end
        if (fabs(start.x-end.x) > fabs(start.y-end.y))
            s = (o.start.x-start.x)/(end.x-start.x);
        else
            s = (o.start.y-start.y)/(end.y-start.y);
        if (!between01_adjust(s)) return 0;  //if outside [0..1] 
        r[0] = o.start; //o.start is on AB
        pos_other[0] = 0;
        pos_my[0] = s;
        return 1;
    }
    if (!test_zero((start-o.start).PerpProduct(o.end-o.start)))
        return 0; //start is not on MN's line
    //They are either parallel and on the same line or AB is degenerate and lies on MN's line
    double t[2];
    //select x or y depending on precision
    if (fabs(o.start.x-o.end.x) > fabs(o.start.y-o.end.y)) {
        t[0] = (start.x-o.start.x)/(o.end.x-o.start.x);
        t[1] = (end.x-o.start.x)/(o.end.x-o.start.x);
    } else {
        t[0] = (start.y-o.start.y)/(o.end.y-o.start.y);
        t[1] = (end.y-o.start.y)/(o.end.y-o.start.y);
    }
    if (t[0] > t[1]) std::swap(t[0], t[1]);
    if (!test_smaller(t[0], 1) || !test_smaller(1, t[1]))
        return 0; //AB lies outside MN
    if (!test_smaller(0, t[0])) t[0] = 0; //if close to 0 or even smaller snap it there
    if (!test_smaller(t[1], 1)) t[1] = 1;
    const bool dosecond = test_smaller(0, t[1]) && test_smaller(t[1], 1); //if t[1]!~=0 and t[1]!~=1
    unsigned num = 0;
    for (int i = 0; i<(dosecond ? 2 : 1); i++) {
        r[num] = o.start + (o.end-o.start)*t[i];
        pos_other[num] = t[i];
        if (fabs(start.x-end.x) > fabs(start.y-end.y))
            pos_my[num] = (r[num].x-start.x)/(end.x-start.x);
        else
            pos_my[num] = (r[num].y-start.y)/(end.y-start.y);
        if (between01_adjust(pos_my[num])) num++;
    }
    return num;
}


/** Check if two segments cross
 * returns 0 if none, 1 if yes, 2 if they are rectilinear and overlap.
 * 0 - no crossing
 * 1 - one crossing point (in r[0])
 * 2 - the two sections intersects from r[0] to r[1]
 * if any of the two sections are degenerate we return 1 only if it lies on the other section, else 0
 * in pos_in_ab we return the relative pos of the crosspoint(s) in AB, in pos_in_mn for MN
 * See http://softsurfer.com/Archive/algorithm_0104/algorithm_0104B.htm
 * crossing points that result in a pos value close to 1 are ignored
 * In case AB is the same (or lies on the same line) as MN, the two endpoint of the common
 * sections are returned (if they overlap or touch)
 */
unsigned Edge::CrossingSegments_NoSnap(const Edge &o, XY r[], double pos_my[], double pos_other[]) const
{
    _ASSERT(straight && o.straight);
    if (fabs((end-start).PerpProduct(o.end-o.start))>1e-10) {
        //They are not parallel (and none of them are degenerate, but that does not matter now)
        double t = (end-start).PerpProduct(start-o.start) / (end-start).PerpProduct(o.end-o.start);
        if (t<0 || t>1) return 0;
        double s = (o.end-o.start).PerpProduct(start-o.start) / (end-start).PerpProduct(o.end-o.start);
        if (s<0 || s>1) return 0; //the intersection of the line is outside AB
        r[0] = o.start + (o.end-o.start)*t;
        pos_other[0] = t;
        pos_my[0] = s;
        return 1;
    }
    //either they are parallel or one or both sections are degenerate (= a point)
    if (o.start==o.end) {
        if (!test_zero((o.start-start).PerpProduct(end-start)))
            return 0; //o.start is not on AB's line and AB is not degenerate
        if (start==end) {
            if (start==o.start) {
                r[0] = o.start;
                pos_other[0] = 0;
                pos_my[0] = 0;
                return 1;
            }
            return 0; //both sections degenerate to a point, but do not overlap
        }
        double s;
        //select x or y depending on precision
        //o.start lies on AB, let us see if between start and end
        if (fabs(start.x-end.x) > fabs(start.y-end.y))
            s = (o.start.x-start.x)/(end.x-start.x);
        else
            s = (o.start.y-start.y)/(end.y-start.y);
        if (s<0 || s>1) return 0;  //if outside [0..1] 
        r[0] = o.start; //o.start is on AB
        pos_other[0] = 0;
        pos_my[0] = s;
        return 1;
    }
    if (!test_zero((start-o.start).PerpProduct(o.end-o.start)))
        return 0; //start is not on MN's line
    //They are either parallel and on the same line or AB is degenerate and lies on MN's line
    double t[2];
    //select x or y depending on precision
    if (fabs(o.start.x-o.end.x) > fabs(o.start.y-o.end.y)) {
        t[0] = (start.x-o.start.x)/(o.end.x-o.start.x);
        t[1] = (end.x-o.start.x)/(o.end.x-o.start.x);
    } else {
        t[0] = (start.y-o.start.y)/(o.end.y-o.start.y);
        t[1] = (end.y-o.start.y)/(o.end.y-o.start.y);
    }
    if (t[0] > t[1]) std::swap(t[0], t[1]);
    if (t[0]>1 || t[1]<0)
        return 0; //AB lies outside MN
    unsigned num = 0;
    for (int i = 0; i<2; i++) {
        if (t[i]<0) t[i] = 0;
        if (t[i]>1) t[i] = 1;
        r[num] = o.start + (o.end-o.start)*t[i];
        pos_other[num] = t[i];
        if (fabs(start.x-end.x) > fabs(start.y-end.y))
            pos_my[num] = (r[num].x-start.x)/(end.x-start.x);
        else
            pos_my[num] = (r[num].y-start.y)/(end.y-start.y);
        _ASSERT(pos_my[num]>=0 && pos_my[num]<=1);
        if (pos_my[num]>=0 && pos_my[num]<=1) num++;
    }
    return num;
}

/** Returns the crossing of a bezier with an infinite line.
 * We do not filter the result by position values falling outside [0,1]*/
unsigned Edge::CrossingLine(const XY &A, const XY &B, double pos_my[], double pos_segment[]) const
{
    _ASSERT(!straight);
    _ASSERT(!B.test_equal(A));
    //rotate and shift the bezier so that the line we cross becomes the X axis
    const double l = A.Distance(B);
    const double sin_r = (B.y-A.y)/l;
    const double cos_r = (A.x-B.x)/l;
    const Edge e = CreateShifted(-B).Rotate(cos_r, sin_r);
    //Now find the parameter values of crossing x, (where y=0)
    //A==start, B==c1, C==c2, D==end
    //Curve is: (-A+3B-3C+D)t^3 + (3A-6B+3C)t^2 + (-3A+3B)t + A
    double coeff[4] = {e.start.y,
                       3*(e.c1.y-e.start.y),
                       3*(e.start.y-2*e.c1.y+e.c2.y),
                       -e.start.y+3*(e.c1.y-e.c2.y)+e.end.y};
    unsigned num = solve_degree3(coeff, pos_my);
    //x coordinates of the crosspoints divided by segment length serve as positions of the segment
    for (unsigned u = 0; u<num; u++) {
        between01_adjust(pos_my[u]);
        const XY xy = e.Split(pos_my[u]);
//        _ASSERT(test_zero(fabs(xy.y)));
        pos_segment[u] = xy.x/l;
    }
    return num;
}


bool Edge::HullOverlap(const Edge &o) const
{
    //Assume at least one is curvy
    _ASSERT(!straight || !o.straight);
    if (straight) return o.HullOverlap(*this);
    //Now we are curvy, o may or may not be.
    //First check if a bounding box signals no crossing
    if (o.straight) {
        if (!Range(std::min(std::min(start.x, end.x), std::min(c1.x, c2.x)), 
                   std::max(std::max(start.x, end.x), std::max(c1.x, c2.x))).Overlaps(
             Range(std::min(o.start.x, o.end.x), std::max(o.start.x, o.end.x))))
            return false;
        if (!Range(std::min(std::min(start.y, end.y), std::min(c1.y, c2.y)), 
                   std::max(std::max(start.y, end.y), std::max(c1.y, c2.y))).Overlaps(
             Range(std::min(o.start.y, o.end.y), std::max(o.start.y, o.end.y))))
            return false;
    } else {
        if (!Range(std::min(std::min(start.x, end.x), std::min(c1.x, c2.x)), 
                   std::max(std::max(start.x, end.x), std::max(c1.x, c2.x))).Overlaps(
             Range(std::min(std::min(o.start.x, o.end.x), std::min(o.c1.x, o.c2.x)), 
                   std::max(std::max(o.start.x, o.end.x), std::max(o.c1.x, o.c2.x)))))
            return false;
        if (!Range(std::min(std::min(start.y, end.y), std::min(c1.y, c2.y)), 
                   std::max(std::max(start.y, end.y), std::max(c1.y, c2.y))).Overlaps(
             Range(std::min(std::min(o.start.y, o.end.y), std::min(o.c1.y, o.c2.y)), 
                   std::max(std::max(o.start.y, o.end.y), std::max(o.c1.y, o.c2.y)))))
            return false;
    }
    //Two convex hulls intersect if any of A->B, A->C and A->D in one of them
    //crosses any of A->B, B->C, C->D D->A on the other. Now since we do not know
    //which order the four points give us a convex hull, we also check A->C and B->D.
    double t, s;
    if (Cross(o.start, o.end, start, end, t, s)) return true;
    if (Cross(o.start, o.end, start, c1, t, s)) return true;
    if (Cross(o.start, o.end, start, c2, t, s)) return true;
    if (Cross(o.start, o.end, end, c1, t, s)) return true;
    if (Cross(o.start, o.end, end, c2, t, s)) return true;
    if (Cross(o.start, o.end, c1, c2, t, s)) return true;
    if (o.straight) return false;
    if (Cross(o.start, o.c1, start, end, t, s)) return true;
    if (Cross(o.start, o.c1, start, c1, t, s)) return true;
    if (Cross(o.start, o.c1, start, c2, t, s)) return true;
    if (Cross(o.start, o.c1, end, c1, t, s)) return true;
    if (Cross(o.start, o.c1, end, c2, t, s)) return true;
    if (Cross(o.start, o.c1, c1, c2, t, s)) return true;
    if (Cross(o.start, o.c2, start, end, t, s)) return true;
    if (Cross(o.start, o.c2, start, c1, t, s)) return true;
    if (Cross(o.start, o.c2, start, c2, t, s)) return true;
    if (Cross(o.start, o.c2, end, c1, t, s)) return true;
    if (Cross(o.start, o.c2, end, c2, t, s)) return true;
    if (Cross(o.start, o.c2, c1, c2, t, s)) return true;
    return false;
}


/** Determines which side of a line four points are.
 * Returns +1 or -1 if all points are on one or the other side.
 * Returns 0 if some points are on one and others on the other side;
 * or if any point is exactly on the line.*/
int Edge::WhichSide(const XY &A, const XY &B) const
{
    //http://www.geometrictools.com/LibMathematics/Intersection/Intersection.html
    // Vertices are projected to the form A+t*B.  Return value is +1 if all
    // t > 0, -1 if all t < 0, 0 otherwise, in which case the line splits the
    // curve's hull or straight segment we are.

    const unsigned num = straight ? 2 : 4;
    const XY Perp = (A-B).Rotate90CW();
    unsigned positive = 0, negative = 0;
    for (unsigned i = 0; i < num; ++i) {
        const double t = Perp.DotProduct((&start)[i] - A);
        if (t > 0) ++positive;
        else if (t < 0) ++negative;
        if (positive > 0 && negative > 0) return 0;
    }
    return positive==num ? +1 : negative==num ? -1 : 0;
}

/**Determines if the convex hull represented by the four points overlap with us */
bool Edge::OverlapConvexHull(const XY&A, const XY&B, const XY&C, const XY&D) const
{
    int one, total;
    total = WhichSide(A, B);
    if (total==0) return true;
    total += one = WhichSide(B, C);
    if (one==0) return true;
    total += one = WhichSide(C, D);
    if (one==0) return true;
    total += one = WhichSide(D, A);
    if (one==0) return true;
    //it still may be that 'o' is completely iside 'ABCD'
    //XXX ToDo: Check for clockwiseness of ABCD
    if (total==-4) return true;
    return false;
}

/**Determines if the triangle represented by the three points overlap with us */
bool Edge::OverlapConvexHull(const XY&A, const XY&B, const XY&C) const
{
    int one, total;
    total = WhichSide(A, B);
    if (total==0) return true;
    total += one = WhichSide(B, C);
    if (one==0) return true;
    total += one = WhichSide(C, A);
    if (one==0) return true;
    //it still may be that 'o' is completely iside 'ABC'
    //XXX ToDo: Check for clockwiseness of ABC
    if (total==-3) return true;
    return false;
}



bool Edge::HullOverlap2(const Edge &o) const
{
    //Assume at least one is curvy
    _ASSERT(!straight || !o.straight);
    if (straight) return o.HullOverlap2(*this);
    //Now we are curvy, o may or may not be.
    //First check if a bounding box signals no crossing
    if (o.straight) {
        if (!Range(std::min(std::min(start.x, end.x), std::min(c1.x, c2.x)),
            std::max(std::max(start.x, end.x), std::max(c1.x, c2.x))).Overlaps(
            Range(std::min(o.start.x, o.end.x), std::max(o.start.x, o.end.x))))
            return false;
        if (!Range(std::min(std::min(start.y, end.y), std::min(c1.y, c2.y)),
            std::max(std::max(start.y, end.y), std::max(c1.y, c2.y))).Overlaps(
            Range(std::min(o.start.y, o.end.y), std::max(o.start.y, o.end.y))))
            return false;
    } else {
        if (!Range(std::min(std::min(start.x, end.x), std::min(c1.x, c2.x)),
            std::max(std::max(start.x, end.x), std::max(c1.x, c2.x))).Overlaps(
            Range(std::min(std::min(o.start.x, o.end.x), std::min(o.c1.x, o.c2.x)),
            std::max(std::max(o.start.x, o.end.x), std::max(o.c1.x, o.c2.x)))))
            return false;
        if (!Range(std::min(std::min(start.y, end.y), std::min(c1.y, c2.y)),
            std::max(std::max(start.y, end.y), std::max(c1.y, c2.y))).Overlaps(
            Range(std::min(std::min(o.start.y, o.end.y), std::min(o.c1.y, o.c2.y)),
            std::max(std::max(o.start.y, o.end.y), std::max(o.c1.y, o.c2.y)))))
            return false;
    }
    //Determine our convex hull by checking which diagonals cross
    double t, s;
    if (Cross(start, c2, end, c1, t, s))
        //diagonals are start->c2 and end->c1
        //hull is start->c1->c2->end
        return o.OverlapConvexHull(start, c1, c2, end);
    if (Cross(start, c1, end, c2, t, s))
        //diagonals are start->c1 and end->c2
        //hull is start->end->c1->c2
        return o.OverlapConvexHull(start, end, c1, c2);
    if (Cross(start, end, c1, c2, t, s))
        //diagonals are start->end and c1->c2
        //hull is start->c2->end->c1
        return o.OverlapConvexHull(start, c2, end, c1);
    //OK, the hull is a triangle, with one of the points inside it

    //Ok, now test how start->end crosses c1->c2
    const double denom = ((c2.y-c1.y)*(end.x-start.x) - (c2.x-c1.x)*(end.y-start.y));
    _ASSERT(!test_zero(denom)); //they should not be are parallel 
    s = ((c1.x-start.x)*(end.y-start.y) - (c1.y-start.y)*(end.x-start.x)) / denom;
    t = fabs(end.x-start.x) > fabs(end.y-start.y) ?
        (s*(c2.x-c1.x) + c1.x-start.x)/(end.x-start.x) :
        (s*(c2.y-c1.y) + c1.y-start.y)/(end.y-start.y);
    if (s>=0 && s<=1) {
        //OK, start->end crosses c1->c2 somewhere inside c1->c2
        _ASSERT(t<0 || t>1);
        if (t<0)
            //the point inside is "start"
            return o.OverlapConvexHull(end, c1, c2);
        else
            //the point inside is "end"
            return o.OverlapConvexHull(start, c1, c2);
    } else {
        //OK, start->end is crossed by c1->c2 somewhere inside start->end 
        //(or else they just cross outside both sections and one of the Cross() calls 
        //above should have fired)
        _ASSERT(s<0 || s>1);
        _ASSERT(t<=0 || t<=1);
        if (s<0)
            //the point inside is "c1"
            return o.OverlapConvexHull(end, start, c2);
        else
            //the point inside is "c2"
            return o.OverlapConvexHull(start, c1, end);
    }
}

bool Edge::HullOverlap3(const Edge &o) const
{
    //Assume at least one is curvy
    _ASSERT(!straight || !o.straight);
    if (straight) {
        if (!Range(std::min(std::min(o.start.y, o.end.y), std::min(o.c1.y, o.c2.y)),
            std::max(std::max(o.start.y, o.end.y), std::max(o.c1.y, o.c2.y))).Overlaps(
            Range(std::min(start.y, end.y), std::max(start.y, end.y))))
            return false;
        if (!Range(std::min(std::min(o.start.x, o.end.x), std::min(o.c1.x, o.c2.x)),
            std::max(std::max(o.start.x, o.end.x), std::max(o.c1.x, o.c2.x))).Overlaps(
            Range(std::min(start.x, end.x), std::max(start.x, end.x))))
            return false;
    } else if (o.straight) {
        if (!Range(std::min(std::min(start.y, end.y), std::min(c1.y, c2.y)),
            std::max(std::max(start.y, end.y), std::max(c1.y, c2.y))).Overlaps(
            Range(std::min(o.start.y, o.end.y), std::max(o.start.y, o.end.y))))
            return false;
        if (!Range(std::min(std::min(start.x, end.x), std::min(c1.x, c2.x)),
            std::max(std::max(start.x, end.x), std::max(c1.x, c2.x))).Overlaps(
            Range(std::min(o.start.x, o.end.x), std::max(o.start.x, o.end.x))))
            return false;
    } else {
        if (!Range(std::min(std::min(start.y, end.y), std::min(c1.y, c2.y)),
            std::max(std::max(start.y, end.y), std::max(c1.y, c2.y))).Overlaps(
            Range(std::min(std::min(o.start.y, o.end.y), std::min(o.c1.y, o.c2.y)),
            std::max(std::max(o.start.y, o.end.y), std::max(o.c1.y, o.c2.y)))))
            return false;
        if (!Range(std::min(std::min(start.x, end.x), std::min(c1.x, c2.x)),
            std::max(std::max(start.x, end.x), std::max(c1.x, c2.x))).Overlaps(
            Range(std::min(std::min(o.start.x, o.end.x), std::min(o.c1.x, o.c2.x)),
            std::max(std::max(o.start.x, o.end.x), std::max(o.c1.x, o.c2.x)))))
            return false;
    }
    return true;
}

/** Returns 16*flatness^2 */
double Edge::Flatness() const
{
    const double ux = 3.0*c1.x - 2.0*start.x - end.x; 
    const double uy = 3.0*c1.y - 2.0*start.y - end.y; 
    const double vx = 3.0*c2.x - 2.0*end.x - start.x; 
    const double vy = 3.0*c2.y - 2.0*end.y - start.y; 
    return std::max(ux*ux, vx*vx)+std::max(uy*uy, vy*vy);
}

/* Returns the number of crosspoints */
unsigned Edge::CrossingBezier(const Edge &A, XY r[], double pos_my[], double pos_other[],
                              double pos_my_mul, double pos_other_mul, 
                              double pos_my_offset, double pos_other_offset) const
{
    //If both straight, we calculate and leave
    if (straight) {
        if (!A.straight)
            return A.CrossingBezier(*this, r, pos_other, pos_my, pos_other_mul, pos_my_mul,
                                    pos_other_offset, pos_my_offset);
        switch (CrossingSegments_NoSnap(A, r, pos_my, pos_other)) {
        default:
            _ASSERT(0); //fallthrough
        case 0: 
            return 0;
        case 2:
            //parallel segments: take midpoint 
            //(this is clearly a parallel segment due to numeric precision issues)
            pos_my[0] = (pos_my[0]+pos_my[1])/2*pos_my_mul+pos_my_offset;
            pos_other[0] = (pos_other[0]+pos_other[1])/2*pos_other_mul+pos_other_offset;
            r[0] = (r[0]+r[1])/2;
            return 1;
        case 1:
            pos_my[0] = pos_my[0]*pos_my_mul+pos_my_offset;
            pos_other[0] = pos_other[0]*pos_other_mul+pos_other_offset;
            return 1;
        }
    }
    //const bool one = HullOverlap(A);
    //const bool  two = HullOverlap2(A);
   // _ASSERT(one==two || A.straight || straight);
    //if (!two) return 0;
    if (!HullOverlap3(A)) return 0;
    Edge M1, M2, A1, A2;
    Split(M1, M2);
    pos_my_mul /= 2;
    unsigned num;
    if (A.straight) {
        num = M1.CrossingBezier(A, r, pos_my, pos_other, pos_my_mul, pos_other_mul,
            pos_my_offset, pos_other_offset);
        num += M2.CrossingBezier(A, r+num, pos_my+num, pos_other+num, pos_my_mul, pos_other_mul,
            pos_my_offset+pos_my_mul, pos_other_offset);
        return num;
    }
    A.Split(A1, A2);
    pos_other_mul /= 2;
    num = M1.CrossingBezier(A1, r, pos_my, pos_other, pos_my_mul, pos_other_mul, 
        pos_my_offset, pos_other_offset);
    num += M2.CrossingBezier(A1, r+num, pos_my+num, pos_other+num, pos_my_mul, pos_other_mul, 
        pos_my_offset+pos_my_mul, pos_other_offset);
    num += M1.CrossingBezier(A2, r+num, pos_my+num, pos_other+num, pos_my_mul, pos_other_mul, 
        pos_my_offset, pos_other_offset+pos_other_mul);
    num += M2.CrossingBezier(A2, r+num, pos_my+num, pos_other+num, pos_my_mul, pos_other_mul, 
        pos_my_offset+pos_my_mul, pos_other_offset+pos_other_mul);
    return num;
}



/** Checks if this edge and `next` can be combined into a single edge.
* If so, also update "this" to be the combined edge & returns true.
* In pos returns the position of "next.start" in the new edge.*/
bool Edge::CheckAndCombine(const Edge &next, double *pos)
{
    if (straight!=next.straight) return false;
    if (straight) {
        const double a = angle(start, end, next.end);
        if (test_zero(a) || test_equal(a, 2)) {
            if (pos)
                *pos = (fabs(start.x-end.x)<fabs(start.y-end.y)) ? (next.start.y-start.y)/(next.end.y-start.y) : (next.start.x-start.x)/(next.end.x-start.x);
            //OK, next.end is on the start->end line. Test if next.start is also in-between start and end
            _ASSERT(start.test_equal(next.start) || test_equal(0, angle(start, end, next.start))
                || test_equal(2, angle(start, end, next.start)));
            _ASSERT(between01(fabs(start.x-end.x)<fabs(start.y-end.y) ? fabs((next.start.y-start.y)/(end.y-start.y)) : fabs((next.start.x-start.x)/(end.x-start.x))));
            end = next.end;
            return true;
        }
        return false;
    }
    if (end != next.start) return false;
    //XXX FIXME do this for bezier curver
    return false;
}


/** Calculates the crosspoints with another edge.
*
* This must ensure that we do not return pos values that are very close to 0 or 1
* such values are to be rounded to 0 (or 1) in that case exatly the start point
* of the arc shall be returned. 
* The arrays shall be at least Edge::MAX_CP big.
*
* @param [in] o The other edge.
* @param [out] r The crosspoins.
* @param [out] pos_my The pos values of the corresponding crosspoints on me.
* @param [out] pos_other The pos values of the corresponding crosspoints on the other edge.
* @returns The number of crosspoins found [0..4].
*/
unsigned Edge::Crossing(const Edge &A, XY r[Edge::MAX_CP], 
                        double pos_my[Edge::MAX_CP], double pos_other[Edge::MAX_CP]) const
{
    if (straight && A.straight)
        return CrossingSegments(A, r, pos_my, pos_other);
    //use int because we may substract one
    int num = CrossingBezier(A, r, pos_my, pos_other, 1, 1, 0, 0);
    _ASSERT(num<Edge::MAX_CP);
    //In case of multiple hits, we need to remove erroneously occuring ones
    //these are the ones, where both pos_my and pos_otehr are close
    //We do it pairwise
    for (int i = 0; i<num-1; i++)
        for (int j = i+1; j<num; j++)
            if (test_equal(pos_my[i], pos_my[j]) && test_equal(pos_other[i], pos_other[j])) {
                //erase index j
                num--;
                for (int k = j; k<num; k++) {
                    r[k] = r[k+1];
                    pos_my[k] = pos_my[k+1];
                    pos_other[k] = pos_other[k+1];
                }
            }
     //Snap the crosspoints to the start of the curves
     for (int i = 0; i<num; i++)
        if (test_zero(pos_my[i])) {
            pos_my[i] = 0;
            r[i] = start;
        } else if (test_zero(pos_other[i])) {
            pos_other[i] = 0;
            r[i] = A.start;
        } else if (test_equal(1, pos_my[i])) {
            pos_my[i] = 1;
            r[i] = end;
        } else if (test_equal(1, pos_other[i])) {
            pos_other[i] = 1;
            r[i] = A.end;
        }
        return num;
}

unsigned Edge::SelfCrossing(XY r[Edge::MAX_CP], double pos1[Edge::MAX_CP], double pos2[Edge::MAX_CP]) const
{
    if (straight) return 0;
    //Split the edge at local X extremes (where the tangent is vertical)
    //(we ignore if this happens at an endpoint
    //A==start, B==c1, C==c2, D==end
    //Curve is: (-A+3B-3C+D)t^3 + (3A-6B+3C)t^2 + (-3A+3B)t + A
    //dervivative is: (-3A+9B-9C+3D)t^2 + (6A-12B+6C)t + (-3A+3B)
    //substitutes are        [2]    t^2 +     [1]    t +    [0]
    double coeff[3] = {3*(c1.y-start.y),
                       6*(start.y-2*c1.y+c2.y),
                       3*(-start.y+3*(c1.y-c2.y)+end.y)};
    double roots[2];
    unsigned num = solve_degree2(coeff, roots);
    if (num==0) return 0; //if tangent is never vertical we cannot cross ourselves
    if (num==2 && !between01(roots[1]))
        num--;
    if (!between01(roots[0])) {
        num--;
        if (num==0) return 0;
        roots[0] = roots[1];
    }
    Edge A, B, C;
    if (num==1) {
        /* If we have one such point it can happen like this
         *            __    /
         *          --  -- /      The asterisk is the point where the
         *         /      X       tangent is vertical. X is the crosspoint
         *        *      / \
         *         \____/         
        */
        Split(roots[0], A, B);
        num = A.CrossingBezier(B, r, pos1, pos2, roots[0], 1-roots[0], 0, roots[0]);
    } else {
        /* Two such points may happen in two ways
        *                             \
        *                              |     In the first case we need to test only
        *          _-_                 *     the first and third segment.
        *         /   \                |     In the second case there is no CP,
        *        |     |              /      but testing the third and first segment
        *        *     *             /       will be quick (no overlap) and it is
        *        |     |            |        suitable to decide that we have the
        *         \   /             *        second case.
        *           X               |
        *          / \               \
        */
        _ASSERT(num==2);
        Split(roots[0], A, C); //C is a dummy
        Split(roots[1], C, B);
        num = A.CrossingBezier(B, r, pos1, pos2, roots[0], 1-roots[1], 0, roots[1]);
    }
    //remove degenerate values
    for (unsigned u = 0; u<num; u++)
        if (fabs(pos1[u]-pos2[u])<1e-10) {
            std::copy(pos1+u+1, pos1+num, pos1+u);
            std::copy(pos2+u+1, pos2+num, pos2+u);
            std::copy(r+u+1, r+num, r+u);
            num--;
        }
    //remove duplicates
    for (unsigned u = 0; u+1<num; u++) 
        for (unsigned uu = u+1; uu<num; uu++) 
            if (fabs(pos1[u]-pos1[uu])<1e-10 || fabs(pos2[u]-pos2[uu])<1e-10) {
                std::copy(pos1+uu+1, pos1+num, pos1+uu);
                std::copy(pos2+uu+1, pos2+num, pos2+uu);
                std::copy(r+uu+1, r+num, r+uu);
                num--;
            }
    if (num<=1) return num;
    //We can have a lot of crosspoints here, due to numerical problems, 
    //e.g., when a 
    //pick the widest -if possible
    unsigned min = std::min_element(pos1, pos1+num) - pos1;
    //_ASSERT(min == std::max_element(pos2, pos2+num) - pos2);
    if (min>0) {
        pos1[0] = pos1[min];
        pos2[0] = pos2[min];
        r[0] = r[min];
    }
    return 1;
}


/* Returns the number of crosspoints*/
unsigned Edge::CrossingVerticalBezier(double x, double y[], double pos[], bool forward[],
    double pos_mul, double pos_offset) const
{
    if (straight) {
        if ((start.x >= x && end.x < x) ||      //we cross leftward
            (start.x < x && end.x >= x)) {      //we cross rightward
            //we cross p's line y
            pos[0] = (x - start.x)/(end.x - start.x) * pos_mul + pos_offset;
            y[0] = (end.y - start.y)*pos[0]+ start.y;
            forward[0] = start.x < x; //we cross rightward
            return 1;
        }
        //We do not check for a vertical line here, just in CrossingVertical
        //If a split segment ends up being a vertical line, its neighbouring segments
        //will report it.
        //if (start.x == x && end.x == x) return -1; //vertical line
        return 0;
    }
    //if all points are left or right of the line, we certainly do not cross
    if (start.x<x && end.x<x && c1.x<x && c2.x<x) return 0;
    if (start.x>x && end.x>x && c1.x>x && c2.x>x) return 0;
    Edge M1, M2;
    Split(M1, M2);
    pos_mul /= 2;
    unsigned num;
    num = M1.CrossingVerticalBezier(x, y, pos, forward, pos_mul, pos_offset);
    num += M2.CrossingVerticalBezier(x, y+num, pos+num, forward+num, pos_mul, pos_offset+pos_mul);
    return num;
}




/** Calculates where an edge crosses a vertical line.
*
* This is done for the purposes of SimpleContour::IsWithin.
* Certain rules apply to the case when the vertical line crosses one of the ends of the edge.
* 1. a leftward edge includes its starting endpoint, and excludes its final endpoint;
* 2. a rightward edge excludes its starting endpoint, and includes its final endpoint;
* In short: we include the endpoint with the larger x coordinate only.
* There are some additional rules to the above. If a bezier curve touches the line at 'x'
* we return the touchpoint as a crosspoint (marking it with 0 in 'forward').
* Except if the touchpoint is at one end of the bezier - in this case we count it as
* a crossing point and return either +1 or -1 in 'forward'.
* @param [in] x The x coordinate of the vertical line.
* @param [out] y The y coordinates of the crosspoints.
* @param [out] pos The pos values for the respective crosspoints [0..1]
* @param [out] forward +1 for a crosspoint if the line at x is crossed from left to right
*              (so inside of contour is below y), -1 if the line crosses from right to left,
*              and 0 if the (bezier curve) touches the line.
* @returns The number of crosspoints, or -1 if edhe is a vertical line exacty on x.
*/
int Edge::CrossingVertical(double x, double y[3], double pos[3], int forward[3]) const
{
    if (straight) {
        if ((start.x >= x && end.x < x) ||      //we cross leftward
            (start.x < x && end.x >= x)) {      //we cross rightward
            //we cross p's line y
            pos[0] = (x - start.x)/(end.x - start.x);
            y[0] = (end.y - start.y)*pos[0]+ start.y;
            forward[0] = start.x < x ? +1 : -1; 
            return 1;
        }
        if (start.x == x && end.x == x)
            return -1; //vertical line
        return 0;
    }

    unsigned ret = atX(x, pos);
    if (ret==0)
        goto end;
    std::sort(pos, pos+ret);
    for (unsigned i = 0; i<ret; i++) {
        const double fw_x = NextTangentPoint(pos[i]).x;
        if (test_equal(x, fw_x))
            forward[i] = 0;
        else
            forward[i] = fsign(fw_x-x);
        y[i] = Split(pos[i]).y;
        _ASSERT(test_equal(Split(pos[i]).x, x));
    }

    //Below: Sanitize touching cps. 
    //This is needed, since we say that a cp is toucing on potentially
    //numerically unstable info
    //We ignore touching cps at the endpoints of the arc for now, those will
    //be converted to crossing cps later.

    switch (ret) {
    case 1:
        //1. A single touchpoint at the middle is valid only if start
        //and endpoints are on the same side of the vertical line
        if (forward[0]==0 && pos[0]!=1 && pos[0]!=0)
            if ((start.x < x) != (end.x < x))
                forward[0] = start.x < x ? +1 : -1;
        break;
    case 3:
        //2. A touchpoint (as middle cp) at the inside of the curve is valid only if 
        //and the halfway points till the prev cp and next cp are on the same side 
        //of the vertical line
        if (forward[1]==0)
            if ((Split((pos[0]+pos[1])/2).x < x) != (Split((pos[1]+pos[2])/2).x < x))
                forward[1] = Split((pos[0]+pos[1])/2).x < x ? +1 : -1;
        //fallthrough
    case 2:
        _ASSERT(forward[0] || forward[1]);
        //3. A first touchpoint at the inside of the curve is valid only if startpoint
        //and the halfway point till the next cp are on the same side of the vertical line
        if (forward[0]==0 && pos[0]!=0)
            if ((start.x < x) != (Split((pos[0]+pos[1])/2).x < x))
                forward[0] = start.x < x ? +1 : -1;
        //3. A last touchpoint at the inside of the curve is valid only if endpoint
        //and the halfway point till the prev cp are on the same side of the vertical line
        if (forward[ret-1]==0 && pos[ret-1]!=1)
            if ((end.x < x) != (Split((pos[ret-2]+pos[ret-1])/2).x < x))
                forward[ret-1] = end.x > x ? +1 : -1;
    }

    //Now convert touching cps if they occure at the endpoints
    //Either ignore them if they are at the wrong endpoint or convert them to crossing
    if (pos[ret-1]==1) 
        switch(forward[ret-1]) {
        case 0:  //If a touchpoint is at he end, we convert it to a crossing one
            //determine if we arrive to the endpoint left or right
            if (ret>=2)
                forward[ret-1] = -forward[ret-2];
            else
                forward[ret-1] = start.x < x ? +1 : -1;
            if (forward[ret-1]==1)
                break;
            //fallthrough if we set forward[ret-1] to -1
        case -1:
            //if the endpoint is right-to left, we ignore it
            if (--ret==0)
                goto end;
    }
    if (pos[0]==0)
        switch (forward[0]) {
        case 0: //If a touchpoint is at he start, we convert it to a crossing one
            //determine if we depart from the startpoint left or right
            if (ret>=2)
                forward[0] = -forward[1];
            else
                forward[0] = end.x>x ? +1 : -1;
            if (forward[0]==-1)
                break;
            //fallthrough for forward[0]==1
        case 1:
            //if the start point is left to right, we ignore it
            if (--ret==0)
                goto end;
            for (unsigned u = 0; u<ret; u++) {
                forward[u] = forward[u+1];
                y[u] = y[u+1];
                pos[u] = pos[u+1];
            }
    }

end:
    //the code below is just for error checking - remove when done.
    double loc_pos[3], loc_y[3];
    bool loc_fw[3];
    unsigned a = CrossingVerticalBezier(x, loc_y, loc_pos, loc_fw, 1, 0);
    std::sort(loc_pos, loc_pos+a);
    if (!test_equal(x, start.x) && !test_equal(x, end.x))
        _ASSERT(a==ret && ((a==0) || (a==1 && 0.01>fabs(loc_pos[0] - pos[0])) ||
            (a==2 && 0.01>fabs(loc_pos[0] - pos[0]) && 0.01>fabs(loc_pos[1] - pos[1])) ||
            (a==3 && 0.01>fabs(loc_pos[0] - pos[0]) && 0.01>fabs(loc_pos[1] - pos[1]) && 0.01>fabs(loc_pos[2] - pos[2]))));

    return ret;
}

/** Calculates how many times the edge crosses a vertical line right of a point.
* If we cross the vertical line clockwise (up-to-down, y grows downward) that counts
* as +1 crossing. If we cross it counterclockwise (down-to-up) that counts as -1.
* Crosses at or left of the point (lower or equal x values) are ignored.
* This is done for the purposes of ContoursHelper::CalcCoverageHelper().
*
* Certain rules apply to the case when the vertical line crosses one of the ends of the edge.
* 1. an upward edge includes its starting endpoint, and excludes its final endpoint;
* 2. a downward edge excludes its starting endpoint, and includes its final endpoint;
* In short: we include the endpoint with the larger y coordinate only (y grows downwards).
* Touchpoints (when the edge remains fully on one side of the line) are also ignored,
* except if it is exactly at one end of the bezier. In that case (if it is right of 
* 'x') we count it for the rules above.
* We also ignore horizontal lines exactly on xy.y. (Return 0 for these.)
* @param [in] xy The y coordinate specifies the vertical line. The x coordinate
*                specifies the point (right of which we are interested in cps).
* @param [in] self True, if we know that 'xy' lies on 'this'. In this case
*                  a crossing in the vicinity of xy is considered to go through 
*                  xy (and is therefore ignored).
* @returns The (signed) number of crosspoints.
*/
int Edge::CrossingHorizontalCPEvaluate(const XY &xy, bool self) const
{
    if (straight) {
        //If xy lies on us, we ignore the single crossing we may have.
        if (self) return 0; 
        //if the edge is fully above or below the line or left of the point, we return 0.
        //We also return 0, if the endpoint with the smaller y coordinate is on the line
        //(to implement rules #1 and #2 above).
        if (std::min(start.y, end.y) >= xy.y || std::max(start.y, end.y) < xy.y ||
            std::max(start.x, end.x) < xy.x)
            return 0;
        //return 0 for horizontal straight edges (now start.y==end.y must equal to xy.y)
        if (start.y == end.y) return 0;
        //test for start and endpoints
        if (start == xy) return start.y > end.y ? -1 : 0;
        if (end == xy) return start.y < end.y ? +1 : 0;
        //test if we cross right of xy.x
        if ((xy.y - start.y)/(end.y - start.y)*(end.x-start.x) + start.x <= xy.x)
            return 0;
        //Ok, we should not be ignored, return +1 or -1
            return start.y < xy.y ? +1 : -1;
    }
    //Here we handle beziers
    //Check if we are far off.
    const Block b = GetBezierHullBlock();
    if (b.y.from >= xy.y || b.y.till < xy.y || b.x.till < xy.x)
        return 0;

    double pos[3], x[3];
    unsigned num = atY(xy.y, pos);
    if (num==0)
        return 0;
    std::sort(pos, pos+num);
    unsigned close = 0;
    unsigned last = 0;
    const double threshold = 0.2;
    for (unsigned i = 0; i<num; i++) {
        x[i] = Split(pos[i]).x;
        //if the crosspoint is much to the left or right (by 'threshold' units) do not refine
        if (x[i]+threshold < xy.x) continue;
        if (x[i]-threshold > xy.x) continue;
        //else refine
        close++;
        last = i;
    }
    //now close holds the number of cps close to xy
    if (close>(self ? 1 : 0)) {
        //refine the relevant cps
        for (unsigned i = 0; i<num; i++) {
            if (x[i]+threshold < xy.x) continue;
            if (x[i]-threshold > xy.x) continue;
            if (fabs(Split(pos[i]).y - xy.y)<0.00001) continue;
            _ASSERT(0);
        }
    } else if (self && close==1) {
        //if only one cp and we know it is on us, use exact value.
        x[last] = xy.x;
    }
    int ret = 0;
    for (unsigned i = 0; i<num; i++)
        if (x[i]>xy.x) {
            int dir;
            const double fw_y = NextTangentPoint(pos[i]).y;
            if (test_equal(xy.y, fw_y)) {
                //a relevant crossing point, but the tangent is parallel to the horizontal line...
                //Get second derivative
                const double ss0 = 6 - 6 * pos[i];
                const double ss1 = -12 + 18 * pos[i];
                const double ss2 = 6 - 18 * pos[i];
                const double ss3 = 6 * pos[i];
                const double d2y = start.y * ss0 + c1.y * ss1 + c2.y * ss2 + end.y * ss3;
                if (test_zero(d2y)) {
                    //If the Y component of second derivative is zero (that is both 
                    //first and second derivatives point to exact same/opposite direction =>
                    //this is an inflection point)
                    //then let us see the third derivative.
                    const double d3y = start.y * -6 + c1.y * 18 + c2.y * -18 + end.y * 6;
                    _ASSERT(!test_zero(d3y));
                    //if the Y component of the 3rd derivative is positive, then Y will grow
                    //as we move with increasing position (we cross downwards = clockwise)
                    dir = fsign(d3y);
                    //We continue after the two elses below
                } else {
                    //no inflection point - then this is a touchpoint (tangent is horizontal)
                    //We can ignore if not at the endpoint
                    if (pos[i]!=0. && pos[i]!=1) continue;

                    //now if d2y is positive we curl upwards - this is the endpoint (start or end)
                    //with the smaller y coordinate - we ignore
                    if (d2y>0) continue;

                    //else if this is the startpoint we cross upwards (-1) else we cross downwards (+1)
                    if (pos[i]==0)
                        ret--;
                    else
                        ret++;
                    continue;
                }
            } else
                dir = fsign(fw_y-xy.y);
            //check correct behaviour for endings
            if (pos[i]==0 && dir==+1) continue;
            if (pos[i]==1 && dir==-1) continue;
            ret += dir;
        }

    return ret;
}


/** Calculates the angle of the edge at point `p`.
*
* @param [in] incoming If true the angle of the incoming segment is calculated (as if it were outgoing), if false the outgoing part.
* @param [in] pos The angle is calculated at this pos value. (indifferent for straight edges)
* @returns The angle of the edge
*/
RayAngle Edge::Angle(bool incoming,  double pos) const
{
    _ASSERT(!incoming || pos>0); //shoud not ask for an incoming angle at pos==0
    _ASSERT(incoming  || pos<1); //shoud not ask for an outgoing angle at pos==0
    if (straight)
        return RayAngle(incoming ? angle(end, XY(end.x+100, end.y), start) : 
                                   angle(start, XY(start.x+100, start.y), end));

    //bezier curve radius calc, see http://blog.avangardo.com/2010/10/c-implementation-of-bezier-curvature-calculation/

    //first get the derivative
    const double pos2 = pos * pos;
    const double s0 = -3 + 6 * pos - 3 * pos2;
    const double s1 = 3 - 12 * pos + 9 * pos2;
    const double s2 = 6 * pos - 9 * pos2;
    const double s3 = 3 * pos2;
    const double d1x = start.x * s0 + c1.x * s1 + c2.x * s2 + end.x * s3;
    const double d1y = start.y * s0 + c1.y * s1 + c2.y * s2 + end.y * s3;

    //then get second derivative
    const double ss0 = 6 - 6 * pos;
    const double ss1 = -12 + 18 * pos;
    const double ss2 = 6 - 18 * pos;
    const double ss3 = 6 * pos;
    const double d2x = start.x * ss0 + c1.x * ss1 + c2.x * ss2 + end.x * ss3;
    const double d2y = start.y * ss0 + c1.y * ss1 + c2.y * ss2 + end.y * ss3;

    //then the radius
    const double r1 = sqrt(pow(d1x * d1x + d1y * d1y, 3));
    const double r2 = d1x * d2y - d2x * d1y;
    const double invRadius = r2 / r1; //the inverse of the (signed) radius

    if (pos==0) //must be outgoing
        return RayAngle(angle(start, XY(start.x+100, start.y), c1), invRadius);
    if (pos==1) //must be incoming
        return RayAngle(angle(end, XY(end.x+100, end.y), c2), -invRadius);
    XY A, B;
    const XY C = Split(pos, A, B);
    return incoming ? RayAngle(angle(C, XY(C.x+100, C.y), A), -invRadius) :
                      RayAngle(angle(C, XY(C.x+100, C.y), B), invRadius);
}

inline double COORD(const XY &p, unsigned i) { return i ? p.y : p.x; }

Block Edge::CreateBoundingBox() const
{
    if (straight)
        return Block(start, end);
    //from: http://stackoverflow.com/questions/2587751/an-algorithm-to-find-bounding-box-of-closed-bezier-curves
    std::vector<XY> bounds; 
    bounds.reserve(6);
    bounds.push_back(start);
    bounds.push_back(end);

    for (unsigned i = 0; i < 2; ++i) {
        double b = 6 * COORD(start, i) - 12 * COORD(c1, i) + 6 * COORD(c2, i);
        double a = -3 * COORD(start, i) + 9 * COORD(c1, i) - 9 * COORD(c2, i) + 3 * COORD(end, i);
        double c = 3 * COORD(c1, i) - 3 * COORD(start, i);
        if (a == 0) {
            if (b == 0) {
                continue;
            }
            double t = -c / b;
            if (0 < t && t < 1) 
                bounds.push_back(Split(t));
            continue;
        }
        double b2ac = pow(b, 2) - 4 * c * a;
        if (b2ac < 0) {
            continue;
        }
        double  t1 = (-b + sqrt(b2ac))/(2 * a);
        if (0 < t1 && t1 < 1) 
            bounds.push_back(Split(t1));
        double  t2 = (-b - sqrt(b2ac))/(2 * a);
        if (0 < t2 && t2 < 1) 
            bounds.push_back(Split(t2));
    }
    const auto X = std::minmax_element(bounds.begin(), bounds.end(), 
                     [](const XY &a, const XY &b) {return a.x<b.x; });
    const auto Y = std::minmax_element(bounds.begin(), bounds.end(), 
                     [](const XY &a, const XY &b) {return a.y<b.y; });
    return Block(X.first->x, X.second->x, Y.first->y, Y.second->y);
}

/**Solves a cubic equation of x^3 + a*x^2 + b*x + c
 * Returns the number of roots.*/
int solveCubic(double a, double b, double c, float* r)
{
    double p = b - a*a / 3;
    double q = a * (2*a*a - 9*b) / 27 + c;
    double p3 = p*p*p;
    double d = q*q + 4*p3 / 27;
    double offset = -a / 3;
    if (d >= 0) { // Single solution
        double z = sqrtf(d);
        double u = (-q + z) / 2;
        double v = (-q - z) / 2;
        u = pow(u,1/3);
        v = pow(v, 1/3);
        r[0] = offset + u + v;
        return 1;
    }
    double u = sqrtf(-p / 3);
    double v = acos(-sqrtf(-27 / p3) * q / 2) / 3;
    double m = cos(v), n = sin(v)*1.732050808;
    r[0] = offset + u * (m + m);
    r[1] = offset - u * (n + m);
    r[2] = offset + u * (n - m);
    return 3;
}

double SectionPointDistance(const XY&A, const XY&B, const XY &M, XY &point, double &pos) 
{
    if (test_equal(A.x, B.x)) {
        point.x = A.x;
        if (M.y<std::min(A.y, B.y)) {
            point.y = std::min(A.y, B.y);
            pos = B.y<A.y ? 1 : 0;
        } else if (M.y>std::max(A.y, B.y)) {
            point.y = std::max(A.y, B.y);
            pos = B.y>A.y ? 1 : 0;
        } else {
            point.y = M.y;
            pos = (M.y-A.y)/(B.y-A.y);
            return fabs(A.x - M.x);
        }
        return point.Distance(M);
    }
    if (test_equal(A.y, B.y)) {
        point.y = A.y;
        if (M.x<std::min(A.x, B.x)) {
            point.x = std::min(A.x, B.x);
            pos = B.x<A.x ? 1 : 0;
        } else if (M.x>std::max(A.x, B.x)) {
            point.x = std::max(A.x, B.x);
            pos = B.x>A.x ? 1 : 0;
        } else {
            point.x = M.x;
            pos = (M.x-A.x)/(B.x-A.x);
            return fabs(A.y - M.y);
        }
        return point.Distance(M);
    }
    //http://mathworld.wolfram.com/Point-LineDistance2-Dimensional.html
    const double m = fabs((A.x-M.x)*(B.x-A.x)-(B.y-A.y)*(A.y-M.y))/A.DistanceSqr(B);
    if (m<0) { point = A; pos = 0; return A.Distance(M); }
    if (m>1) { point = B; pos = 1; return B.Distance(M); }
    point = A + (B-A)*m;
    pos = m;
    //return fabs((B.x-A.x)*(A.y-M.y)-(A.x-M.x)*(B.y-A.y))/l;
    return point.Distance(M);
}


double Edge::Distance(const XY &M, XY &point, double &pos) const //always nonnegative
{
    if (straight)
        return SectionPointDistance(start, end, M, point, pos);
   
    double result[5];
    const unsigned num = SolveForDistance(M, result);
    double d = std::min(start.DistanceSqr(M), end.DistanceSqr(M));
    if (start.DistanceSqr(M)<end.DistanceSqr(M)) {
        point = start;
        pos = 0;
    } else {
        point = end;
        pos = 1;
    }
    for (unsigned u = 0; u<num; u++) {
        const XY pu = Split(result[u]);
        const double du = pu.DistanceSqr(M);
        if (du<d) {
            point = pu;
            pos = result[u];
            d = du;
        }
    }
    return sqrt(d);
}

//This does not give the smallest distance. 
double SectionDistance(const XY &A, const XY &B, const XY &M, const XY &N)
{
    if (!test_zero((B-A).PerpProduct(N-M))) {
        //They are not parallel (and none of them are degenerate, but that does not matter now)
        const double t = (B-A).PerpProduct(A-M) / (B-A).PerpProduct(N-M);
        const double s = (N-M).PerpProduct(A-M) / (B-A).PerpProduct(N-M);
        if (between01_approximate_inclusive(t) && between01_approximate_inclusive(s)) 
            return 0;
        //here the two infinite lines cross, but the cp is outside at least one
        //of the two sections
    }
    //They are either parallel, degenerate or just far away
    //calculate the A distance only, B will come anyway
    XY point1;
    double dummy;
    const double d1 = SectionPointDistance(A, B, M, point1, dummy);
    const double d2 = SectionPointDistance(M, N, A, point1, dummy);
    const double d3 = SectionPointDistance(A, B, N, point1, dummy);
    const double d4 = SectionPointDistance(M, N, B, point1, dummy);
    return std::min(std::min(d1, d2), std::min(d3, d4));
}

/** Distance of the hull from a section */
double Edge::HullDistance(const XY &A, const XY &B) const
{
    double ret = SectionDistance(start, c1, A, B);
    if (ret==0) return 0;
    ret = std::min(ret, SectionDistance(c1, c2, A, B));
    if (ret==0) return 0;
    ret = std::min(ret, SectionDistance(c2, end, A, B));
    if (ret==0) return 0;
    ret = std::min(ret, SectionDistance(end, start, A, B));
    if (ret==0) return 0;
    ret = std::min(ret, SectionDistance(start, c2, A, B));
    if (ret==0) return 0;
    return std::min(ret, SectionDistance(c1, end, A, B));
}

/** Distance of the hull from another hull */
double Edge::HullDistance(const XY &A, const XY &B, const XY &C, const XY &D) const
{
    double ret = HullDistance(A, B);
    if (ret==0) return 0;
    ret = std::min(ret, HullDistance(B, C));
    if (ret==0) return 0;
    ret = std::min(ret, HullDistance(C, D));
    if (ret==0) return 0;
    ret = std::min(ret, HullDistance(D, A));
    if (ret==0) return 0;
    ret = std::min(ret, HullDistance(A, C));
    if (ret==0) return 0;
    return std::min(ret, HullDistance(B, D));
}

/** Assuming 'p' is on the bezier, return the corresponding parameter */
double Edge::FindBezierParam(const XY &p) const
{
    if (straight) 
        return point2pos_straight(start, end, p);
    double result[4];
    const unsigned num = SolveForDistance(p, result);
    _ASSERT(num);
    double ret = result[0];
    double d = (p-Split(result[0])).length_sqr();
    for (unsigned u = 1; u<num; u++)
    if (d >(p-Split(result[u])).length_sqr()) {
        d = (p-Split(result[u])).length_sqr();
        ret = result[u];
    }
    return ret;
}

void Edge::Distance(const Edge &o, DistanceType &ret) const    //always nonnegative
{
    if (ret.IsZero()) return;
    if (straight && o.straight) {
        //test if we cross
        if (!test_zero((end-start).PerpProduct(o.end-o.start))) {
            //They are not parallel (and none of them are degenerate, but that does not matter now)
            const double t = (end-start).PerpProduct(start-o.start) / (end-start).PerpProduct(o.end-o.start);
            const double s = (o.end-o.start).PerpProduct(start-o.start) / (end-start).PerpProduct(o.end-o.start);
            if (between01_approximate_inclusive(t) && between01_approximate_inclusive(s)) {
                ret.Merge(0, start + (end-start)*t, start + (end-start)*t);
                return;
            }
            //here the two infinite lines cross, but the cp is outside at least one
            //of the two sections
        }
        //They are either parallel, degenerate or just far away
        //calculate the start distance only, end will come anyway
        XY point1, point2;
        double dummy;
        const double d1 = Distance(o.start, point1, dummy);
        const double d2 = o.Distance(start, point2, dummy);
        if (d1<d2)
            ret.Merge(d1, point1, o.start);
        else
            ret.Merge(d2, start, point2);
        return ;
    }
    if (straight) {
        o.Distance(*this, ret.SwapPoints());
        ret.SwapPoints();
        return;
    }
    if (o.straight) {
        if (ret.Distance()<HullDistance(o.start, o.end))
            return ;
        Edge E1, E2;
        Split(E1, E2);
        E1.Distance(o, ret);
        E2.Distance(o, ret);
        return;
    }
    if (ret.Distance()<HullDistance(o.start, o.end, o.c1, o.c2))
        return;
    Edge E1, E2, F1, F2;
    Split(E1, E2);
    o.Split(F1, F2);
    E1.Distance(F1, ret);
    E2.Distance(F1, ret);
    E1.Distance(F2, ret);
    E2.Distance(F2, ret);
}

double Edge::GetAreaAboveAdjusted() const
{
    if (straight)
        return start.x*end.y-start.y*end.x;
    Edge E1, E2;
    Split(E1, E2);
    return E1.GetAreaAboveAdjusted()+E2.GetAreaAboveAdjusted();
}

double Edge::GetLength() const
{
    if (straight)
        return (start-end).length();
    Edge E1, E2;
    Split(E1, E2);
    return E1.GetLength()+E2.GetLength();
}

/** Return the centroid of the area below the edge multiplied by the area.
*
* We take the endpoints of the edge and draw a line from them to the x axis.
* The area bounded by these two lines, the edge itself and the x axis is
* the subject area of this function. If the `start.x < end.s` the return is
* negated. The return is also negated (perhaps 2nd time) if the edge is above
* the x axis (negative y values).
* The function returns the centroid of this area, scaled by the (signed)
* area of this area.
*
* In more visual terms, showing the x axis and a straight edge and the area.
* @verbatim
    ==========> =========>
    +++++        ------
    __+++        ------
    |\+++        \-----
      \++         \ ---
       \+          \---
        \           \--
        _\|
@endverbatim
* (Since in contour we treat y coordinates as growing downwards, we call these functions
* "xxAreaAbove", meaning the area between edge & X axis.)
* The dotted area is the area above an edge. It is counted as positive in the first example;
* and negative in the second. If we sum all such areas, we get the area of the contour
* (positive for clockwise and negative for counterclockwise), as it should be.
*
* Now, the formula for the above for an edge (X1,Y1)->(X2,Y2) is
* AreaAbove = (X1-X2)*(Y1+Y2)/2 = (X1*Y1 - X2*Y2)/2 + (X1*Y2 - X2*Y1)/2
* If we sum this for all edges of a connected contour the first term will
* cancel out. Thus in the edge::xxAreaAboveAdjusted functions we return
* the second term only (twice 2), thus we return
* AreaAbove*2 - (X1*Y1 - X2*Y2).
*/
XY Edge::GetCentroidAreaAboveUpscaled() const
{
    if (straight) {
        const XY &minY = start.y < end.y ? start : end;
        const XY &maxY = start.y < end.y ? end : start;
        //calculate the centroid of the rectangle and triangle below a (slanted) edge
        //multiplied by their area.
        //We do it in two steps: centroid by the Y coordinate of the area
        //then multiply by the (common) x coordinate of the area
        XY ret = XY(start.x+end.x, minY.y)*(minY.y/2) +                     //centroid of rectange times y coord of its area
            XY(maxY.x*2+minY.x, maxY.y+minY.y*2)*((maxY.y-minY.y)/6);  //centroid of triangle times y coord of its area
        return ret*(start.x - end.x); //this makes ret negative if edge is from right to left (as it should)
    }
    Edge E1, E2;
    Split(E1, E2);
    return E1.GetCentroidAreaAboveUpscaled()+E2.GetCentroidAreaAboveUpscaled();
}

namespace Edge_CreateExpand2D {
    inline int comp_int(const double &a, const double &b)
    {
        return a<b ? -1 : a==b ? 0 : 1;
    }
    inline double comp_dbl(const double &a, const double &b, double g)
    {
        return a<b ? -g : a==b ? 0 : g;
    }
}

/** Returns a list of edges resulting in expanding "this" in 2D.
 *
 * See overall documentation of what a 2D expansion is.
 * We return values describing the direction in which the edge
 * ends and starts as below.
 * @verbatim
   +4  _+3 _  +2
      |\ ^ /|
        \|/
   +1  <-0->  -1
        /|\
      |/ v \|
   -2   -3    -4
 * @endverbatim
 * The above picture describes, how the end is described,
 * the start is described in reverse, thus for lines
 * `stype==dtype`. For degenerate edges `type==0`.
 *
 * @param [in] gap The size of the expansion in the x and y directions.
 * @param [out] ret The edges resulting from the expansion (perhaps more than one).
 * @param [out] stype The type of the starting of the edge.
 * @param [out] etype The type of the ending of the edge.
 */
void Edge::CreateExpand2D(const XY &gap, std::vector<Edge> &ret, int &stype, int &etype) const
{
    if (straight) {
        ret.resize(ret.size()+1);
        Edge &e = ret.back();
        const XY off(Edge_CreateExpand2D::comp_dbl(end.y, start.y, gap.x),
            Edge_CreateExpand2D::comp_dbl(start.x, end.x, gap.y));
        e.start = start + off;
        e.end = end + off;
        //expand for horizontal and vertical edges
        if (start.x == end.x) {
            e.start.y += Edge_CreateExpand2D::comp_dbl(start.y, end.y, gap.y);
            e.end.y -= Edge_CreateExpand2D::comp_dbl(start.y, end.y, gap.y);
        }
        if (start.y == end.y) {
            e.start.x += Edge_CreateExpand2D::comp_dbl(start.x, end.x, gap.x);
            e.end.x -= Edge_CreateExpand2D::comp_dbl(start.x, end.x, gap.x);
        }
        etype = stype = Edge_CreateExpand2D::comp_int(start.x, end.x) +
            Edge_CreateExpand2D::comp_int(start.y, end.y)*3;
        return;
    }
    //see extreme points
    //from: http://stackoverflow.com/questions/2587751/an-algorithm-to-find-bounding-box-of-closed-bezier-curves
    std::vector<double> bounds;
    bounds.reserve(6);
    bounds.push_back(0);
    bounds.push_back(1);

    for (unsigned i = 0; i < 2; ++i) {
        double b = 6 * COORD(start, i) - 12 * COORD(c1, i) + 6 * COORD(c2, i);
        double a = -3 * COORD(start, i) + 9 * COORD(c1, i) - 9 * COORD(c2, i) + 3 * COORD(end, i);
        double c = 3 * COORD(c1, i) - 3 * COORD(start, i);
        if (a == 0) {
            if (b == 0) {
                continue;
            }
            double t = -c / b;
            if (0 < t && t < 1)
                bounds.push_back(t);
            continue;
        }
        double b2ac = pow(b, 2) - 4 * c * a;
        if (b2ac < 0) {
            continue;
        }
        double  t1 = (-b + sqrt(b2ac))/(2 * a);
        if (0 < t1 && t1 < 1)
            bounds.push_back(t1);
        double  t2 = (-b - sqrt(b2ac))/(2 * a);
        if (0 < t2 && t2 < 1)
            bounds.push_back(t2);
    }
    std::sort(bounds.begin(), bounds.end());
    for (unsigned u = 1; u<bounds.size(); u++)
        if (bounds[u-1]==bounds[u])
            bounds.erase(bounds.begin()+u);
    //now bounds contain a sorted list of extreme points
    //and all values are distinct


    const unsigned original_size = ret.size();
    //first we segment the edge at bounding points
    ret.reserve(original_size + bounds.size()*2);
    ret.push_back(*this);
    ret.back().Chop(bounds[0], bounds[1]);
    stype = Edge_CreateExpand2D::comp_int(ret.back().start.x, ret.back().end.x) +
            Edge_CreateExpand2D::comp_int(ret.back().start.y, ret.back().end.y)*3;
    for (unsigned bound_index = 1; bound_index<bounds.size()-1; bound_index++) {
        ret.emplace_back();
        ret.back().visible = visible;
        ret.push_back(*this);
        ret.back().Chop(bounds[bound_index], bounds[bound_index+1]);
    }
    //Shift the segments to place
    for (unsigned u = original_size; u<ret.size(); u += 2)
        ret[u].Shift(XY(Edge_CreateExpand2D::comp_dbl(ret[u].end.y, ret[u].start.y, gap.x),
            Edge_CreateExpand2D::comp_dbl(ret[u].start.x, ret[u].end.x, gap.y)));
    //if only one segment
    if (ret.size()==original_size+1) {
        etype = stype;
        return;
    }
    //connect the segments with lines
    for (unsigned u = original_size+1; u<ret.size(); u += 2) {
        ret[u].start = ret[u-1].end;
        ret[u].end = ret[u+1].start;
    }
    etype = Edge_CreateExpand2D::comp_int(ret.back().start.x, ret.back().end.x) +
            Edge_CreateExpand2D::comp_int(ret.back().start.y, ret.back().end.y)*3;
}

/** Put a dashed/dotted path into a cairo context. Needed for backends not supporting dashed lines.
*
* We also assume cairo dash is set to continuous: we fake dash here.
* @param [in] cr The cairo context.
* @param [in] pattern Contains lengths of alternating on/off segments.
* @param [in] num contains the number of elements in pattern.
* @param pos Shows which pattern element we shall start at (updated on return).
* @param offset Shows at which point we shall start at within the segment selected by `pos` (updated on return).
* @param reverse If true the path is drawn from `end` to `start`.
*/
void Edge::PathDashed(cairo_t *cr, const double pattern[], unsigned num, int &pos, double &offset, bool reverse) const
{
    _ASSERT(num);
    _ASSERT(offset<pattern[pos]);
    if (straight) {
        const XY & fr = reverse ? end : start;
        const XY & to = reverse ? start : end;
        const double len = sqrt((to.x-fr.x)*(to.x-fr.x) + (to.y-fr.y)*(to.y-fr.y));
        const double ddx = (to.x-fr.x)/len;
        const double ddy = (to.y-fr.y)/len;
        double processed = 0;
        double x = fr.x, y = fr.y;
        if (offset) {
            if (pattern[pos]-offset > len) { //remaining segment is shorter than the entire length
                if (pos%2==0) {//start with drawn
                    cairo_move_to(cr, fr.x, fr.y);
                    cairo_line_to(cr, to.x, to.y);
                }
                offset += len;
                return;
            }
            if (pos%2==0) cairo_move_to(cr, x, y);
            x += (pattern[pos]-offset)*ddx;
            y += (pattern[pos]-offset)*ddy;
            if (pos%2==0) cairo_line_to(cr, x, y);
            processed += pattern[pos]-offset;
            pos = (pos+1)%num;
        }

        while (processed+pattern[pos] <= len) {
            if (pos%2==0) cairo_move_to(cr, x, y);
            x += pattern[pos]*ddx;
            y += pattern[pos]*ddy;
            if (pos%2==0) cairo_line_to(cr, x, y);
            processed += pattern[pos];
            pos = (pos+1)%num;
        }

        offset = len - processed;
        if (pos%2==0 && fabs(offset)>1e-10) {
            cairo_move_to(cr, x, y);
            cairo_line_to(cr, to.x, to.y);
        }
        return;
    }
    //curvy
    double local_s = 0, local_e = 1;
    //TODO Do proper length calculations
    const double len = GetLength();
    const double inc = 1/len;
    double processed = 0;
    if (offset) {
        if (pattern[pos]-offset > len) { //remaining segment is longer than the entire length
            offset += len;
            if (pos%2==0) {//start with drawn
                cairo_new_sub_path(cr);
                cairo_move_to(cr, start.x, start.y);
                cairo_curve_to(cr, c1.x, c1.y, c2.x, c2.y, end.x, end.y);
            } 
            return;
        }
        const double new_s = local_s + inc*(pattern[pos]-offset);
        if (pos%2==0) {
            cairo_new_sub_path(cr);
            cairo_move_to(cr, start.x, start.y);
            Edge e(*this, local_s, new_s);
            cairo_curve_to(cr, e.c1.x, e.c1.y, e.c2.x, e.c2.y, e.end.x, e.end.y);
        }
        local_s = new_s;
        processed += pattern[pos]-offset;
        pos = (pos+1)%num;
    }

    while (processed+pattern[pos] <= len) {
        const double new_s = local_s + inc*pattern[pos];
        if (pos%2==0) {
            cairo_new_sub_path(cr);
            cairo_move_to(cr, start.x, start.y);
            Edge e(*this, local_s, new_s);
            cairo_curve_to(cr, e.c1.x, e.c1.y, e.c2.x, e.c2.y, e.end.x, e.end.y);
        }
        local_s = new_s;
        processed += pattern[pos];
        pos = (pos+1)%num;
    }

    offset = len - processed;
    if (pos%2==0 && !test_zero(offset)) {
        cairo_new_sub_path(cr);
        cairo_move_to(cr, start.x, start.y);
        Edge e(*this, local_s, local_e);
        cairo_curve_to(cr, e.c1.x, e.c1.y, e.c2.x, e.c2.y, e.end.x, e.end.y);
    }
}


//helpers for offsetbelow
double Edge::OffsetBelow(const Edge &o, double &touchpoint) const
{
    if (straight && o.straight) {
        //calc for two straight edges
        const double minAB = std::min(start.x, end.x);
        const double maxAB = std::max(start.x, end.x);
        const double minMN = std::min(o.GetStart().x, o.GetEnd().x);
        const double maxMN = std::max(o.GetStart().x, o.GetEnd().x);
        if (minAB > maxMN || minMN > maxAB) return CONTOUR_INFINITY;
        const double x1 = std::max(minAB, minMN);
        const double x2 = std::min(maxAB, maxMN);
        if (start.x == end.x) {
            touchpoint = std::max(start.y, end.y);
            if (o.GetStart().x == o.GetEnd().x)
                return std::min(o.GetStart().y, o.GetEnd().y) - std::max(start.y, end.y); //here all x coordinates must be the same
            return (o.GetEnd().y-o.GetStart().y)/(o.GetEnd().x-o.GetStart().x)*(start.x-o.GetStart().x) + o.GetStart().y -
                std::max(start.y, end.y);
        }
        if (o.GetStart().x == o.GetEnd().x) {
            touchpoint = (start.y-end.y)/(start.x-end.x)*(o.GetStart().x-start.x) + start.y;
            return std::min(o.GetStart().y, o.GetEnd().y) - touchpoint;
        }
        const double y1 = ((start.y-end.y)/(start.x-end.x)*(x1-start.x) + start.y);
        const double y2 = ((start.y-end.y)/(start.x-end.x)*(x2-start.x) + start.y);
        const double diff1 = (o.GetEnd().y-o.GetStart().y)/(o.GetEnd().x-o.GetStart().x)*(x1-o.GetStart().x) + o.GetStart().y - y1;
        const double diff2 = (o.GetEnd().y-o.GetStart().y)/(o.GetEnd().x-o.GetStart().x)*(x2-o.GetStart().x) + o.GetStart().y - y2;
        if (diff1<diff2) {
            touchpoint = y1;
            return diff1;
        }
        touchpoint = y2;
        return diff2;
    }
    if (straight) {
        if (!o.CreateBoundingBox().x.Overlaps(Range(std::min(start.x, end.x), std::max(start.x, end.x))))
            return CONTOUR_INFINITY;
        Edge E1, E2;
        o.Split(E1, E2);
        double t1, t2;
        const double o1 = OffsetBelow(E1, t1);
        const double o2 = OffsetBelow(E1, t2);
        touchpoint = o1<o2 ? t1 : t2;
        return std::min(o1, o2);
    }
    if (o.straight) {
        if (!CreateBoundingBox().x.Overlaps(Range(std::min(o.start.x, o.end.x), std::max(o.start.x, o.end.x))))
            return CONTOUR_INFINITY;
        Edge E1, E2;
        Split(E1, E2);
        double t1, t2;
        const double o1 = E1.OffsetBelow(o, t1);
        const double o2 = E2.OffsetBelow(o, t2);
        touchpoint = o1<o2 ? t1 : t2;
        return std::min(o1, o2);
    }
    if (!CreateBoundingBox().x.Overlaps(o.CreateBoundingBox().x))
        return CONTOUR_INFINITY;
    Edge E1, E2, F1, F2;
    Split(E1, E2);
    o.Split(F1, F2);
    double t[4], ob[4];
    ob[0] = E1.OffsetBelow(F1, t[0]);
    ob[1] = E2.OffsetBelow(F1, t[1]);
    ob[2] = E1.OffsetBelow(F2, t[2]);
    ob[3] = E2.OffsetBelow(F2, t[3]);
    const unsigned u = std::min_element(ob+0, ob+3) - ob;
    touchpoint = t[u];
    return ob[u];
}

/** Calculates the touchpoint of tangents drawn from a given point.
*
* For curvy edges:
*     Given the point `from` draw tangents to the edge (two can be drawn)
*     and calculate where these tangents touch the it.
*     In this context the *clockwise tangent* is the one which is traversed from
*     `from` towards the arc touches the ellipse in the clockwise direction.
* For straight edges:
*     We return the `start` in both clockwise and counterclockwise.
*     This is because we ignore the endpoint - after all it will be the startpoint
*     of the next edge and will be considered there.
* @param [in] from The point from which the tangents are drawn.
* @param [out] clockwise The point where the clockwise tangent touches the ellipse.
* @param [out] cclockwise The point where the counterclockwise tangent touches the ellipse.
*/
void Edge::TangentFrom(const XY &from, XY &clockwise, XY &cclockwise) const
{
    if (straight) 
        clockwise = cclockwise = start;
    else {
        Edge E1, E2;
        Split(E1, E2);
        E1.TangentFrom(from, clockwise, cclockwise);
        E2.TangentFrom(from, clockwise, cclockwise);
    }
}

/** Calculates the touchpoint of tangents drawn to touch two edges.
*
* Given the two edges, four such tangents can be drawn, here we focus on the two
* outer ones, the ones that touch either both edges from above or both from below,
* but not mixed.
* Note that we do not consider the endpoint - since this is a helper for closed
* contours - the start next edge will be the same as our endpoint.
* The clockwise cand cclockwise members shall be initialized to a point
* on the curves.
* @param [in] o The other edge.
* @param [out] clockwise The points where the clockwise tangent touches us
*                        (clockwise[0]) and `o` (clockwise[1]).
* @param [out] cclockwise The points where the counterclockwise tangent touches us
*                         (cclockwise[0]) and `o` (cclockwise[1]).
*/
void Edge::TangentFrom(const Edge &o, XY clockwise[2], XY cclockwise[2]) const
{
    XY c, cc;
    clockwise[1] = minmax_clockwise(clockwise[0], o.start, clockwise[1], true);
    cclockwise[1] = minmax_clockwise(cclockwise[0], o.start, cclockwise[1], false);

    clockwise[0] = minmax_clockwise(clockwise[1], start, clockwise[0], false);
    cclockwise[0] = minmax_clockwise(cclockwise[1], start, cclockwise[0], true);

    XY cw[2], ccw[2];
    if (straight) {
        if (!o.straight) {
            Edge E1, E2;
            o.Split(E1, E2);
            TangentFrom(E1, clockwise, cclockwise);
            TangentFrom(E2, clockwise, cclockwise);
        } 
    } else if (o.straight) {
        Edge E1, E2;
        Split(E1, E2);
        E1.TangentFrom(o, clockwise, cclockwise);
        E2.TangentFrom(o, clockwise, cclockwise);
    } else {
        Edge E1, E2, F1, F2;
        Split(E1, E2);
        o.Split(F1, F2);
        E1.TangentFrom(F1, clockwise, cclockwise);
        E2.TangentFrom(F1, clockwise, cclockwise);
        E1.TangentFrom(F2, clockwise, cclockwise);
        E2.TangentFrom(F2, clockwise, cclockwise);
    }
}


/** Finds a crosspoint of two infinite lines defined by `A`->`B` and `M`->`N`
*/
ELineCrossingType crossing_line_line(const XY &A, const XY &B, const XY &M, const XY &N, XY &r)
{
    const double perp = (B-A).PerpProduct(N-M);
    if (test_zero(perp)) return LINE_CROSSING_PARALLEL;
    //They are not parallel (and none of them are degenerate)
    const double t = (B-A).PerpProduct(A-M) / perp;
    r = M + (N-M)*t;
    const bool t_in = between01_approximate_inclusive(t);
    if (!t_in && t>0.5) return LINE_CROSSING_OUTSIDE_BK;
    const double s = (N-M).PerpProduct(M-A) / -perp;
    const bool s_in = between01_approximate_inclusive(s);
    if (s_in) return t_in ? LINE_CROSSING_INSIDE : LINE_CROSSING_OUTSIDE_FW;
    return s<0.5 ? LINE_CROSSING_OUTSIDE_BK : LINE_CROSSING_OUTSIDE_FW;
}



/** Find the new crosspoint of two originally neighbouring expanded edges.
*
* `this` and `M` are two expanded edges.
* The whole logic of this function assumes EXPAND_MITER, but this is called for all types of
* expansions to see how two edges relate.
* return value is
* DEGENERATE: One of the edges have start==end
*  (`newcp` is undefined on return)
* TRIVIAL: the two edges connect exactly at their end+startpoint
*  (`newcp` is this point on return)
* CP_REAL: The two edges actually cross (between "start" and "end")
*  (we return their crosspoint in `newcp`)
* CP_EXTENDED: The two edges do not meet but their extension does. Here extension means
* the continuation of the curve along the bezier's tangent for beziers.
*  (we return their crosspoint in `newcp`)
* NO_CP_PARALLEL: Either the two edges are parallel or their linear extension is such.
*   (`newcp` is a calculated convergence point: can be reached by adding linear segments)
* CP_INVERSE, if the extended cp is on the wrong side of one of the edges.
* (If one of the edges is a curvy one, we first try to find extended cps on the right
* side so we prefer to return CP_EXTENDED. Only if not possible, do we return this.
*
* @param [in] M The other expanded edge.
* @param [out] newcp Returns their new crosspoint.
* @param [out] my_pos In case of CP_REAL returns the position of the cp on 'this', if it is a bezier.
* @param [out[ M_pos In case of CP_REAL returns the position of the cp on M, if that is a bezier.
* @returns The relation of the two expanded edges.
*/
Edge::EExpandCPType Edge::FindExpandedEdgesCP(const Edge&M, XY &newcp, double &my_pos, double &M_pos) const
{
    if (start.test_equal(end) || M.start.test_equal(M.end)) return DEGENERATE;
    if (end.test_equal(M.start)) {
        newcp = end;
        return TRIVIAL;
    }
    const double parallel_join_multipiler = 5;
    if (M.straight) {
        if (straight) {
            switch (crossing_line_line(start, end, M.start, M.end, newcp)) {
            default: _ASSERT(0);
            case LINE_CROSSING_PARALLEL:
                if (end.test_equal(M.start)) {
                    newcp = end;
                    return CP_REAL;
                }
                return NO_CP_PARALLEL;
            case LINE_CROSSING_OUTSIDE_FW: return CP_EXTENDED;
            case LINE_CROSSING_OUTSIDE_BK: return CP_INVERSE;
            case LINE_CROSSING_INSIDE: return CP_REAL;
            }
        } else {
            //we are bezier, but M is straight
            XY r[8];
            double pos_us[8], pos_M[8];
            int num = Crossing(Edge(M.start, M.end), r, pos_us, pos_M);
            if (!num) {
                const XY tangent = NextTangentPoint(1.0);
                //if not parallel, we cross at an extension
                switch (crossing_line_line(end, tangent, M.start, M.end, newcp)) {
                case LINE_CROSSING_INSIDE:
                case LINE_CROSSING_OUTSIDE_FW:
                    return CP_EXTENDED;
                default:
                    _ASSERT(0); //fallthrough
                case LINE_CROSSING_OUTSIDE_BK:
                    return CP_INVERSE;
                case LINE_CROSSING_PARALLEL:
                    //if we are parallel, calculate a conv point
                    const double dist = end.Distance(M.start) * parallel_join_multipiler;
                    const double mlen = M.end.Distance(M.start);
                    newcp = M.start + (M.start-M.end)/mlen*dist;
                    return NO_CP_PARALLEL;
                }
            } else {
                //several crosspoints: use the one closest to the end of the bezier
                unsigned pos = 0;
                for (unsigned u = 1; u<num; u++)
                    if (pos_us[u]> pos_us[pos])
                        pos = u;
                newcp = r[pos];
                my_pos = pos_us[pos];
                return CP_REAL;
            }
        }
    } else {
        XY r[8];
        if (straight) {
            //we are a straight, but M is bezier
            double pos_us[8], pos_M[8];
            int num = M.Crossing(*this, r, pos_M, pos_us);
            if (!num) {
                const XY tangent = M.PrevTangentPoint(0.0);
                switch (crossing_line_line(start, end, tangent, M.start, newcp)) {
                case LINE_CROSSING_INSIDE:
                case LINE_CROSSING_OUTSIDE_FW:
                    return CP_EXTENDED;
                default:
                    _ASSERT(0); //fallthrough
                case LINE_CROSSING_OUTSIDE_BK:
                    return CP_INVERSE;
                case LINE_CROSSING_PARALLEL:
                    //if we are parallel, calculate a conv point
                    const double dist = end.Distance(M.start) * parallel_join_multipiler;
                    const double len = end.Distance(start);
                    newcp = end + (end-start)/len*dist;
                    return NO_CP_PARALLEL;
                }
            }
            //several crosspoints: use the one closest to the start of the bezier
            unsigned pos = 0;
            for (unsigned u = 1; u<num; u++)
            if (pos_M[u] < pos_M[pos])
                pos = u;
            newcp = r[pos];
            M_pos = pos_M[pos];
            return CP_REAL;
        } else {
            //both are beziers
            double pos_us[8], pos_M[8];
            int num = Crossing(M, r, pos_us, pos_M);
            if (!num) {
                const XY tangent1 = NextTangentPoint(1.0);
                const XY tangent2 = M.PrevTangentPoint(0.0);
                switch (crossing_line_line(end, tangent1, tangent2, M.start, newcp)) {
                case LINE_CROSSING_INSIDE:
                case LINE_CROSSING_OUTSIDE_FW:
                    return CP_EXTENDED;
                default:
                    _ASSERT(0); //fallthrough
                case LINE_CROSSING_OUTSIDE_BK:
                    return CP_INVERSE;
                case LINE_CROSSING_PARALLEL:
                    //if we are parallel, calculate a conv point
                    const double dist = end.Distance(M.start) * parallel_join_multipiler;
                    const double len = end.Distance(tangent1);
                    newcp = (end+M.start)/2 + (end-tangent1)/len*dist;
                    return NO_CP_PARALLEL;
                }
            }
            //several crosspoints: use the one closest to the end of us
            unsigned pos = 0;
            for (unsigned u = 1; u<num; u++)
            if (pos_us[u]> pos_us[pos])
                pos = u;
            newcp = r[pos];
            my_pos = pos_us[pos];
            M_pos = pos_M[pos];
            return CP_REAL;
        }
    }
}

/** Creates a circle, ellipse or a segment of it. */
void Edge::GenerateEllipse(std::vector<Edge> &append_to, const XY &c, double radius_x, double radius_y, 
                           double tilt_deg, double s_deg, double d_deg, bool clockwise)
{
    const size_t original_size = append_to.size();
    if (!clockwise) 
        std::swap(s_deg, d_deg);
    s_deg = fmod_negative_safe(s_deg, 360.)/90;
    d_deg = fmod_negative_safe(d_deg, 360.)/90;
    if (d_deg<=s_deg) d_deg += 4;
    if (radius_y==0) radius_y = radius_x;
    //first define the unit circle
    //http://hansmuller-flex.blogspot.hu/2011/04/approximating-circular-arc-with-cubic.html
    const double magic_number = 0.5522847498; //4/3 * (sqrt(2)-1)
    const XY Q[4][4] = {{XY(1, 0), XY(0, 1), XY(1, magic_number), XY(magic_number, 1)},
                        {XY(0, 1), XY(-1, 0), XY(-magic_number, 1), XY(-1, magic_number)},
                        {XY(-1, 0), XY(0, -1), XY(-1, -magic_number), XY(-magic_number, -1)},
                        {XY(0, -1), XY(1, 0), XY(magic_number, -1), XY(1, -magic_number)}};
    unsigned u = unsigned(s_deg);
    const unsigned u_till = unsigned(d_deg);
    append_to.emplace_back(Q[u][0], Q[u][1], Q[u][2], Q[u][3]);
    if (u==u_till)
        //single quadrant
        append_to.back().Chop(s_deg-floor(s_deg), d_deg-floor(d_deg));
    else {
        //multiple quadrants: first needs to be chopped only from its start
        append_to.back().Chop(s_deg-floor(s_deg), 1);
        for (u++; u!=u_till; u++)
            append_to.emplace_back(Q[u%4][0], Q[u%4][1], Q[u%4][2], Q[u%4][3]);
        if (d_deg!=floor(d_deg)) {
            //d_deg is not exactly at the start of a quadrant, we append a last fractional quadrant
            append_to.emplace_back(Q[u%4][0], Q[u%4][1], Q[u%4][2], Q[u%4][3]);
            append_to.back().Chop(0, d_deg-floor(d_deg));
        }
    }
    //Now we have (part of) a unit circle. 
    for (size_t i = original_size; i<append_to.size(); i++) {
        //Distort to make an ellipse
        append_to[i].Scale(XY(radius_x, radius_y));
        //next, tilt
        append_to[i].Rotate(cos(tilt_deg/180*M_PI), sin(tilt_deg/180*M_PI));
        //next move
        append_to[i].Shift(c);
    }
    //Done. Invert if counterclockwise is needed
    if (clockwise) return;
    for (size_t i = original_size; i<append_to.size(); i++)
        append_to[i].Invert();
    for (size_t i = 0; i<(append_to.size()-original_size)/2; i++)
        std::swap(append_to[original_size + i], append_to[append_to.size()-1-i]);
}



/*
Solving the Nearest Point-on-Curve Problem
and
A Bezier Curve-Based Root-Finder
by Philip J. Schneider
from "Graphics Gems", Academic Press, 1990
http://tog.acm.org/resources/GraphicsGems/category.html#Curves%20and%20Surfaces_link
http://tog.acm.org/resources/GraphicsGems/gems/NearestPoint.c
*/

const int MAXDEPTH = 64;	/*  Maximum depth for recursion */
const double EPSILON = (ldexp(1.0, -MAXDEPTH-1)); /*Flatness control value */



/*
* CrossingCount :
*	Count the number of times a Bezier control polygon
*	crosses the 0-axis. This number is >= the number of roots.
*
*/
unsigned CrossingCount(const XY V[6])
{
    unsigned n_crossings = 0;	/*  Number of zero-crossings	*/
    bool old_sign = V[0].y < 0;
    for (unsigned i = 1; i <= 5; i++) {
        const bool sign = V[i].y < 0;
        if (sign != old_sign) {
            n_crossings++;
            old_sign = sign;
        }
    }
    return n_crossings;
}

/*
*  ControlPolygonFlatEnough :
*	Check if the control polygon of a Bezier curve is flat enough
*	for recursive subdivision to bottom out.
*
*  Corrections by James Walker, jw@jwwalker.com, as follows:

There seem to be errors in the ControlPolygonFlatEnough function in the
Graphics Gems book and the repository (NearestPoint.c). This function
is briefly described on p. 413 of the text, and appears on pages 793-794.
I see two main problems with it.

The idea is to find an upper bound for the error of approximating the x
intercept of the Bezier curve by the x intercept of the line through the
first and last control points. It is claimed on p. 413 that this error is
bounded by half of the difference between the intercepts of the bounding
box. I don't see why that should be true. The line joining the first and
last control points can be on one side of the bounding box, and the actual
curve can be near the opposite side, so the bound should be the difference
of the bounding box intercepts, not half of it.

Second, we come to the implementation. The values distance[i] computed in
the first loop are not actual distances, but squares of distances. I
realize that minimizing or maximizing the squares is equivalent to
minimizing or maximizing the distances.  But when the code claims that
one of the sides of the bounding box has equation
a * x + b * y + c + max_distance_above, where max_distance_above is one of
those squared distances, that makes no sense to me.

I have appended my version of the function. If you apply my code to the
cubic Bezier curve used to test NearestPoint.c,

static XY bezCurve[4] = {    /  A cubic Bezier curve    /
{ 0.0, 0.0 },
{ 1.0, 2.0 },
{ 3.0, 3.0 },
{ 4.0, 2.0 },
};

my code computes left_intercept = -3.0 and right_intercept = 0.0, which you
can verify by sketching a graph. The original code computes
left_intercept = 0.0 and right_intercept = 0.9.

*/

bool ControlPolygonFlatEnough(const XY V[6]) 
{
    int     i;        /* Index variable        */
    double  value;
    double  max_distance_above;
    double  max_distance_below;
    double  error;        /* Precision of root        */
    double  intercept_1,
        intercept_2,
        left_intercept,
        right_intercept;
    double  det, dInv;
    double  a2, b2, c2;

    /* Coefficients of implicit    */
    /* eqn for line from V[0]-V[deg]*/
    /* Derive the implicit equation for line connecting first *'
    /*  and last control points */
    const double a = V[0].y - V[5].y;
    const double b = V[5].x - V[0].x;
    const double c = V[0].x * V[5].y - V[5].x * V[0].y;

    max_distance_above = max_distance_below = 0.0;

    for (i = 1; i < 5; i++) {
        value = a * V[i].x + b * V[i].y + c;

        if (value > max_distance_above) {
            max_distance_above = value;
        } else if (value < max_distance_below) {
            max_distance_below = value;
        }
    }

    /*  Implicit equation for zero line */
    const double a1 = 0.0;
    const double b1 = 1.0;
    const double c1 = 0.0;

    /*  Implicit equation for "above" line */
    a2 = a;
    b2 = b;
    c2 = c - max_distance_above;

    det = a1 * b2 - a2 * b1;
    dInv = 1.0/det;

    intercept_1 = (b1 * c2 - b2 * c1) * dInv;

    /*  Implicit equation for "below" line */
    a2 = a;
    b2 = b;
    c2 = c - max_distance_below;

    det = a1 * b2 - a2 * b1;
    dInv = 1.0/det;

    intercept_2 = (b1 * c2 - b2 * c1) * dInv;

    /* Compute intercepts of bounding box    */
    left_intercept = std::min(intercept_1, intercept_2);
    right_intercept = std::max(intercept_1, intercept_2);

    error = right_intercept - left_intercept;
    const double error_better = fabs((max_distance_below-max_distance_above)/a);

    return (error_better < EPSILON) ? 1 : 0;
}


bool ControlPolygonFlatEnough_Original(const XY V[6])
{
    int     i;        /* Index variable        */
    double  value;
    double  max_distance_above = 0;
    double  max_distance_below = 0;
    double  error;        /* Precision of root        */
    double  intercept_1,
        intercept_2,
        left_intercept,
        right_intercept;
    double  det, dInv;
    double  a2, b2, c2;

    /* Coefficients of implicit    */
    /* eqn for line from V[0]-V[deg]*/
    /* Derive the implicit equation for line connecting first *'
    /*  and last control points */
    const double a = V[0].y - V[5].y;
    const double b = V[5].x - V[0].x;
    const double c = V[0].x * V[5].y - V[5].x * V[0].y;


    const double abSquared = (a * a) + (b * b);

    for (i = 1; i < 5; i++) {
        /* Compute distance from each of the points to that line    */
        const double distance = a * V[i].x + b * V[i].y + c;
        if (distance > 0.0)
            max_distance_above = std::max((distance * distance) / abSquared, max_distance_above);
        else if (distance < 0.0)
            max_distance_below = std::min(-(distance * distance) / abSquared, max_distance_below);
    }
    /*  Implicit equation for zero line */
    const double a1 = 0.0;
    const double b1 = 1.0;
    const double c1 = 0.0;

    /*  Implicit equation for "above" line */
    a2 = a;
    b2 = b;
    c2 = c + max_distance_above;

    det = a1 * b2 - a2 * b1;
    dInv = 1.0/det;

    intercept_1 = (b1 * c2 - b2 * c1) * dInv;

    /*  Implicit equation for "below" line */
    a2 = a;
    b2 = b;
    c2 = c + max_distance_below;

    det = a1 * b2 - a2 * b1;
    dInv = 1.0/det;

    intercept_2 = (b1 * c2 - b2 * c1) * dInv;

    /* Compute intercepts of bounding box    */
    left_intercept = std::min(intercept_1, intercept_2);
    right_intercept = std::max(intercept_1, intercept_2);

    error = 0.5 * (right_intercept-left_intercept);
    const double error_better = fabs((max_distance_below-max_distance_above)/a)/2;

    return (error_better < EPSILON) ? 1 : 0;
}

/*
*  ComputeXIntercept :
*	Compute intersection of chord from first control point to last
*  	with 0-axis.
*
*/
/* NOTE: "T" and "Y" do not have to be computed, and there are many useless
* operations in the following (e.g. "0.0 - 0.0").
*/
double ComputeXIntercept(const XY V[6])
{
    //returns the x offset where the V[0]->V[5] line crosses the X axis
    const double XNM = V[5].x - V[0].x;
    const double YNM = V[5].y - V[0].y;
    const double XMK = V[0].x;
    const double YMK = V[0].y;

    return XMK - XNM*YMK / YNM;
}


/*
*  Bezier :
*	Evaluate a Bezier curve at a particular parameter value
*      Fill in control points for resulting sub-curves if "Left" and
*	"Right" are non-null.
*
*/
void Bezier(const XY V[6], double t, XY Left[6], XY Right[6])
{
    XY 	Vtemp[6][6];

    /* Copy control points	*/
    for (unsigned j = 0; j <= 5; j++) 
        Vtemp[0][j] = V[j];

    /* Triangle computation	*/
    for (unsigned i = 1; i <= 5; i++)
        for (unsigned j = 0; j <= 5 - i; j++) 
            Vtemp[i][j] = (1.0 - t) * Vtemp[i-1][j] + t * Vtemp[i-1][j+1];

    for (unsigned j = 0; j <= 5; j++)
        Left[j] = Vtemp[j][0];

    for (unsigned j = 0; j <= 5; j++)
        Right[j] = Vtemp[5-j][j];
}


/*
*  FindRoots :
*	Given a 5th-degree equation in Bernstein-Bezier form, find
*	all of the roots in the interval [0, 1].  Return the number
*	of roots found.
*/
unsigned FindRoots(XY w[6], double t[5], unsigned depth)
{
    int 	i;
    XY 	Left[6],	/* New left and right 		*/
        Right[6];	/* control polygons		*/

    switch (CrossingCount(w)) {
    case 0: 	/* No solutions here	*/
        return 0;
    case 1:
        /* Unique solution	*/
        /* Stop recursion when the tree is deep enough	*/
        /* if deep enough, return 1 solution at midpoint 	*/
        if (depth >= MAXDEPTH) {
            t[0] = (w[0].x + w[5].x) / 2.0;
            return 1;
        }
        if (ControlPolygonFlatEnough_Original(w)) {
            t[0] = ComputeXIntercept(w);
            return 1;
        }
    }

    /* Otherwise, solve recursively after	*/
    /* subdividing control polygon		*/
    Bezier(w, 0.5, Left, Right);
    const unsigned left_count = FindRoots(Left, t, depth+1);
    const unsigned right_count = FindRoots(Right, t+left_count, depth+1);

    /* Send back total number of solutions	*/
    return (left_count+right_count);
}


/** Return a series of parameter values potentially closest to 'p'*/
unsigned Edge::SolveForDistance1(const XY &p, double pos[5]) const
{
    /*  Convert problem to 5th-degree Bezier form	*/
    static const double z[3][4] = {	/* Precomputed "z" for cubics	*/
        {1.0, 0.6, 0.3, 0.1},
        {0.4, 0.6, 0.6, 0.4},
        {0.1, 0.3, 0.6, 1.0},
    };
    XY c[4], d[3]; 

    /*Determine the c's -- these are vectors created by subtracting*/
    /* point P from each of the control points				*/
    c[0] = start - p;
    c[1] = c1 - p;
    c[2] = c2 - p;
    c[3] = end - p;

    /* Determine the d's -- these are vectors created by subtracting*/
    /* each control point from the next					*/
    //for (i = 0; i <= DEGREE - 1; i++) {
    //    d[i] = V2ScaleII(V2Sub(&V[i+1], &V[i], &d[i]), 3.0);
    //}
    d[0] = 3*(c1-start);
    d[1] = 3*(c2-c1);
    d[2] = 3*(end-c2);

    // control-points for Bezier curve whose zeros represent candidates for closest point to the input parametric curve
    XY w[6] = {{0, 0}, {0.2, 0}, {0.4, 0}, {0.6, 0}, {0.8, 0}, {1, 0}}; 
    for (int k = 0; k <= 5; k++) {
        const unsigned lb = std::max(0, k - 2);
        const unsigned ub = std::min(k, 3);
        for (unsigned i = lb; i <= ub; i++) {
            const int j = k - i;
            w[k].y += d[j].DotProduct(c[i]) * z[j][i];
        }
    }

    /* Find all possible roots of 5th-degree equation */
    return FindRoots(w, pos, 0);
}

/** Return a series of parameter values potentially closest to 'p'*/
unsigned Edge::SolveForDistance2(const XY &p, double pos[5]) const
{
    //A==start, B==c1, C==c2, D==end
    //Curve is: (-A+3B-3C+D)t^3 + (3A-6B+3C)t^2 + (-3A+3B)t + A
    //M-to-curve vector is: curve-M
    //substitutes:     P * t^3  +      Q *  t^2 +     R * t + S
    //dervivative is: (-3A+9B-9C+3D)t^2 + (6A-12B+6C)t + (-3A+3B)
    //substitutes            3*P  * t^2 +    2*Q   * t +    R
    // curve-M is orthogonal to curve derivative where their dot product is zero
    //Dot product in general is: W.Z = Wx*Zx + Wy*Zy
    //(curve-M).(derivative) is: 
    // (P.3P)t^5 + (P.2Q + Q.3P)t^4 + (P.R + Q.2Q + R.3P)t^3 + 
    // (Q.R + R.2Q + S.3P)t^2 + (R.R + S.2Q)t + S.R
    //so we need to search for roots

    const XY P = -start+3*(c1-c2)+end;
    const XY Q = (3*(start+c2)-6*c1);
    const XY R = 3*(c1-start);
    const XY S = start-p;

    double coeff[6] = {
        R.DotProduct(R) + 2*S.DotProduct(Q),
        6*(Q.DotProduct(R) + S.DotProduct(P)),
        6*(2*P.DotProduct(R) + Q.DotProduct(Q)),
        20*P.DotProduct(Q),
        15*P.DotProduct(P)
    };

    return  solve_degree4(coeff, pos);
}

/**
* Refine a point projection's [t] value.
*/
double Edge::refineProjection(const XY &p, double t, double distancesqr, double precision) const
{
    if (precision < 0.0001) return t;
    // refinement
    const double prev_t = t-precision;
    const double next_t = t+precision;
    const XY prev = Split(prev_t);
    const XY next = Split(next_t);
    const double prev_distance = p.DistanceSqr(prev);
    const double next_distance = p.DistanceSqr(next);
    // smaller distances?
    if (prev_t >= 0 && prev_distance < distancesqr) { return refineProjection(p, prev_t, prev_distance, precision); }
    if (next_t <= 1 && next_distance < distancesqr) { return refineProjection(p, next_t, next_distance, precision); }
    // larger distances
    return refineProjection(p, t, distancesqr, precision/2.0);
}


unsigned Edge::SolveForDistance(const XY &p, double ret[5]) const
{
    Edge a(XY(0, 0), XY(1, 2), XY(3, 3), XY(4, 2));
    XY pp(3.5, 2.0);
    double pos[5];
    unsigned u = a.SolveForDistance1(XY(0,0), pos);

    return SolveForDistance1(p, ret);
}

/** Return a series of parameter values where the x coordinate of the curve is 'x'*/
unsigned Edge::atX(double x, double roots[3]) const
{
    _ASSERT(!straight);
    //A==start, B==c1, C==c2, D==end
    //Curve is: (-A+3B-3C+D)t^3 + (3A-6B+3C)t^2 + (-3A+3B)t + A

    double coeff[4] = {start.x-x,
        3*(c1.x-start.x),
        3*(start.x+c2.x)-6*c1.x,
        -start.x+3*(c1.x-c2.x)+end.x};
    unsigned ret = solve_degree3(coeff, roots);
    for (unsigned i = 0; i<ret; /*nope*/)
        if (!between01_adjust(roots[i])) {
            for (unsigned k = i+1; k<ret; k++)
                roots[k-1] = roots[k];
            ret--;
        } else
            i++;
        return ret;
}

/** Return a series of parameter values where the x coordinate of the curve is 'x'*/
unsigned Edge::atY(double y, double roots[3]) const
{
    _ASSERT(!straight);
    //A==start, B==c1, C==c2, D==end
    //Curve is: (-A+3B-3C+D)t^3 + (3A-6B+3C)t^2 + (-3A+3B)t + A

    double coeff[4] = {start.y-y,
        3*(c1.y-start.y),
        3*(start.y+c2.y)-6*c1.y,
        -start.y+3*(c1.y-c2.y)+end.y};
    unsigned ret = solve_degree3(coeff, roots);
    for (unsigned i = 0; i<ret; /*nope*/)
        if (!between01_adjust(roots[i])) {
            for (unsigned k = i+1; k<ret; k++)
                roots[k-1] = roots[k];
            ret--;
        } else
            i++;
        return ret;
}

/** Expands the edge.
*
* This takes the direction of the edge to determine the 'outside' side of the
* edge to expand towards (or away from in case of a negative `gap`).
* Destroys bounding box!
* @param [in] gap The amount to expand (or shrink if <0)
* @param [out] expanded Append the expanded edges to this. The elements of this container
*                       must have a constructor (XY,XY,XY,XY,bool) for beziers and one
*                       (XY,XY,bool) for straight lines.
* @param [out] original Append the original edge to this. If the expansion
*                       requires to split the edge to several pieces append
*                       the split chunks of the original here.
*/
bool Edge::CreateExpand(double gap, std::vector<Edge> &expanded, std::vector<Edge> *original) const
{
    if (straight) {
        const double length = start.Distance(end);
        const XY wh = (end-start).Rotate90CCW()/length*gap;
        expanded.emplace_back(start+wh, end+wh, !!visible); //Visual Studio complains for bitfields in such templates
        if (original)
            original->push_back(*this);
        return true;
    }
    //calculate X and Y extremes and inflection points
    //we can have at most 2 of each. Plus 2 for the two endpoints
    std::vector<double> t(8);
    //Start with extremes. X first
    //A==start, B==c1, C==c2, D==end
    //Curve is: (-A+3B-3C+D)t^3 + (3A-6B+3C)t^2 + (-3A+3B)t + A
    //dervivative is: (-3A+9B-9C+3D)t^2 + (6A-12B+6C)t + (-3A+3B)
    //substitutes are        [2]    t^2 +     [1]    t +    [0]
    double Ycoeff[3] = {3*(c1.y-start.y),
        6*(start.y-2*c1.y+c2.y),
        3*(-start.y+3*(c1.y-c2.y)+end.y)};
    unsigned num = solve_degree2(Ycoeff, &t[1]);
    double Xcoeff[3] = {3*(c1.x-start.x),
        6*(start.x-2*c1.x+c2.x),
        3*(-start.x+3*(c1.x-c2.x)+end.x)};
    num += solve_degree2(Xcoeff, &t[num+1]);
    //The inflection ponts are
    const XY a = c1-start, b = c2-c1-a, c = end-c2-a-2*b;
    double Icoeff[3] = {a.x*b.y - a.y*b.x,
        a.x*c.y - a.y*c.x,
        b.x*c.y - b.y*c.x};
    num += solve_degree2(Icoeff, &t[num+1]);
    t.resize(num+1);
    //Second derivative is: (-6A+18B-18C+6D)t + (6A-12B+6C)
    double Y2coeff[2] = {6*(start.y-2*c1.y+c2.y),
        6*(-start.y+3*(c1.y-c2.y)+end.y)};
    if (!test_zero(Y2coeff[1])) {
        double s = -Y2coeff[0]/Y2coeff[1];
        if (between01_adjust(s))
            t.push_back(s);
    }
    double X2coeff[2] = {6*(start.x-2*c1.x+c2.x),
        6*(-start.x+3*(c1.x-c2.x)+end.x)};
    if (!test_zero(X2coeff[1])) {
        double s = -X2coeff[0]/X2coeff[1];
        if (between01_adjust(s))
            t.push_back(s);
    }
    //prune roots outside [0,1] range or close to 0 or one
    for (unsigned k = num; k>0; k--)
        if (t[k] < SMALL_NUM || t[k]>1-SMALL_NUM)
            t.erase(t.begin()+k);
    if (t.size()>1) {
        std::sort(++t.begin(), t.end());
        //remove duplicates (like a cusp at an X extreme)
        for (unsigned k = t.size()-2; k>0; k--)
            if (test_equal(t[k], t[k+1]))
                t.erase(t.begin()+k);
    }
    const unsigned size_before = expanded.size();
    const int orig_offset = original ? int(original->size())-int(size_before) : 0;
    if (t.size()==1)
        CreateExpandOneSegment(gap, expanded, original);
    else {
        t[0] = 0;
        t.push_back(1);
        for (unsigned k = 0; k<t.size()-1; k++) {
            Edge e(*this, t[k], t[k+1]);
            e.CreateExpandOneSegment(gap, expanded, original);
        }
    }
    //Remove loops in the created set - beware some of them degenerated to a segment
    XY r[9];
    double pos_one[9], pos_two[9];
    //Start by removing self-intersections
    for (unsigned u = size_before; u<expanded.size(); u++) 
        if (!expanded[u].IsStraight()) {
            unsigned num = expanded[u].SelfCrossing(r, pos_one, pos_two);
            if (num==0) continue;
            if (num==2) {
                if (pos_one[0]<pos_one[1] && pos_two[0]>pos_two[1])
                    num--;
                else if (pos_one[0]>pos_one[1] && pos_two[0]<pos_two[1]) {
                    num--;
                    pos_one[0] = pos_one[1];
                    pos_two[0] = pos_two[1];
                } else
                    //a strange crossing system - give up
                    continue;
            }
            const double p1 = std::min(pos_one[0], pos_two[0]);
            const double p2 = std::max(pos_one[0], pos_two[0]);
            //Shop to remove the part between the two positions
            expanded.emplace(expanded.begin()+u+1, expanded[u], p2, 1);
            expanded[u].Chop(0,p1);
            if (original) {
                original->emplace(original->begin()+u+orig_offset+1, (*original)[u+orig_offset], p2, 1);
                (*original)[u+orig_offset].Chop(0, p1);
            }
        }
    //Now find the two outermost crosspoints.
    //If crosspoints do not happen like that, we will be unsuccessful here.
    //That case will be handled during the untangle operation that completes 
    //SimpleContour::Expand().
    for (unsigned u = size_before; u+1<expanded.size(); u++)
        for (unsigned v = expanded.size()-1; u<v; v--) {
            unsigned num = expanded[u].Crossing(expanded[v], r, pos_one, pos_two);
            if (!num) continue;
            //Check if in case of several crosspoints the one with smallest pos in "pos_one"
            //corresponds to the one with largest pos in "pos_two"
            unsigned small = std::min_element(pos_one, pos_one+num) - pos_one;
            unsigned large = std::max_element(pos_two, pos_two+num) - pos_two;
            //If they do not - we could not easily remove the loop - we give up
            if (small!=large) 
                return size_before != expanded.size();
            //Loop found. Chop these edges and remove all edges in-between
            //XXX ToDo: This is not perfect. We may have a bad loop still or even several!!!
            expanded[u].Chop(0, pos_one[small]);
            expanded[v].Chop(pos_two[large], 1);
            expanded.erase(expanded.begin()+u+1, expanded.begin()+v);
            if (original) {
                u += orig_offset;
                v += orig_offset;
                (*original)[u].Chop(0, pos_one[small]);
                (*original)[v].Chop(pos_two[large], 1);
                original->erase(original->begin()+u+1, original->begin()+v);
            }
            goto sanitize;
        }
sanitize:
    //Make sure connecting edges really connect
    for (unsigned u = size_before; u+1<expanded.size(); u++) 
        expanded[u].end = expanded[u+1].start = (expanded[u].end + expanded[u+1].start)/2;
    return size_before != expanded.size();
}

bool Edge::CreateExpandOneSegment(double gap, std::vector<Edge> &expanded, std::vector<Edge> *original) const
{
    if (straight) {
        const double length = start.Distance(end);
        const XY wh = (end-start).Rotate90CCW()/length*gap;
        expanded.emplace_back(start+wh, end+wh, !!visible); //Visual Studio complains for bitfields in such templates
        if (original)
            original->push_back(*this);
        return true;
    }
    //test assumptions
    XY dummy;
    //_ASSERT(LINE_CROSSING_INSIDE == crossing_line_line(start, c2, end, c1, dummy));
    //Tiller-Hansson: expand the start,c1,c2,end polygon.
    const double l0 = start.Distance(c1);
    const double l12 = c1.Distance(c2);
    const double l3 = c2.Distance(end);
    const XY wh0 = (c1-start).Rotate90CCW()/l0*gap;
    const XY wh12 = (c2-c1).Rotate90CCW()/l12*gap;
    const XY wh3 = (end-c2).Rotate90CCW()/l3*gap;
    const XY new_start = start+wh0;
    const XY new_end = end+wh3;
    XY new_c1, new_c2;
    //If we can compute new c1 and c2 from the intersection of the offset hull lines, do so.
    if (LINE_CROSSING_PARALLEL == crossing_line_line(new_start, c1+wh0, c1+wh12, c2+wh12, new_c1) ||
        LINE_CROSSING_PARALLEL == crossing_line_line(c1+wh12, c2+wh12, c2+wh3, new_end, new_c2)) {
        //if not, project the control points towards the original curve
        XY dummy, C1, C2;
        double dumm;
        //get the normal for all 4 control points
        const double l1 = Distance(c1, C1, dumm);
        const double l2 = Distance(c2, C2, dumm);
        const bool left_1 = (start-C1).Rotate90CCW().DotProduct(c1-C1) < 0;
        const bool left_2 = (start-C1).Rotate90CCW().DotProduct(c1-C1) < 0;
        const XY wh1 = (c1-C1)/l1*(left_1 ? gap : -gap);
        const XY wh2 = (c2-C2)/l2*(left_2 ? gap : -gap);
        new_c1 = c1+wh1;
        new_c2 = c2+wh2;
    }
    expanded.emplace_back(new_start, new_end, new_c1, new_c2, !!visible);

    //Test error of the resulting offset curve
    const double EPSILON = 0.1;
    //offset the middle point of the result along the normal of the result back onto the original
    const XY Mid = expanded.back().Split();
    const XY Normal = (expanded.back().NextTangentPoint(0.5)-Mid).Rotate90CW()+Mid;
    XY r[9];
    double pos_me[9], pos_seg[9];
    unsigned num = CrossingLine(Mid, Normal, pos_me, pos_seg);
    _ASSERT(num);
    //find crossing closest to Mid (pos_seg==0)
    double minpos = MaxVal(minpos);
    int p = -1;
    for (unsigned u = 0; u<num; u++)
        if (between01(pos_me[u]) && minpos>fabs(pos_seg[u])) {
            p = u;
            minpos = fabs(pos_seg[u]);
        }
    //if the normal does not eben project back to the original 
    //or if the projected point is too far away, we subdivide
    if (p==-1 || fabs(fabs(gap)-Split(pos_me[p]).Distance(Mid)) > EPSILON) {
        expanded.pop_back();
        Edge E1, E2;
        Split(E1, E2);
        E1.CreateExpandOneSegment(gap, expanded, original);
        E2.CreateExpandOneSegment(gap, expanded, original);
    } else 
        //else we are OK with the emplaced offset curve
        if (original)
            original->push_back(*this);

    return true;
}




} //namespace contour 
