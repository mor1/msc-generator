/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011,2012,2013,2014 Zoltan Turanyi
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
#include <list>
#include <algorithm>
#include "contour_edge.h"

namespace contour {

//////////////////HELPERS

    /** Solve quadratic equation.
*
* @param [in] afCoeff Parameters of the equation. `m_afCoeff[0]` is the constant, `m_afCoeff[2]` is the coeff of `x^2`.
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
* @param [in] afCoeff Parameters of the equation. `m_afCoeff[0]` is the constant, `m_afCoeff[3]` is the coeff of `x^3`.
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
* @param [in] afCoeff Parameters of the equation. `m_afCoeff[0]` is the constant, `m_afCoeff[4]` is the coeff of `x^4`.
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

/** Split the bezier curve to two halves.*/
void Edge::Split(Edge &r1, Edge &r2) const
{
    r1.start = start;
    r2.end = end;
    r2.start = r1.end = Split(r1.c2, r2.c1);
    r1.c1 = (start+c1)/2;
    r2.c2 = (c2+end)/2;
    r1.visible = r2.visible = visible;
    r1.straight = r1.start.test_equal(r1.end);
    r1.MakeStraightIfNeeded(flatness_tolerance);
    r2.straight = r2.start.test_equal(r2.end);
    r2.MakeStraightIfNeeded(flatness_tolerance);
}

/** Split the bezier curve at position t. t must be in (0..1).*/
void Edge::Split(double t, Edge &r1, Edge &r2) const
{
    r1.start = start;
    r2.end = end;
    r2.start = r1.end = Split(t, r1.c2, r2.c1);
    r1.c1 = Mid(start, c1, t);
    r2.c2 = Mid(c2, end, t);
    r1.visible = r2.visible = visible;
    r1.straight = r1.start.test_equal(r1.end);
    r1.MakeStraightIfNeeded(flatness_tolerance);
    r2.straight = r2.start.test_equal(r2.end);
    r2.MakeStraightIfNeeded(flatness_tolerance);
}

/** Cuts away the beginning and the end of the edge.
 * Keep only the parts between 't' and 's'.
 * Returns true if the result is degenerate to a single point.
 * 's' and 't' must be between [0..1].*/
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
 * sections are returned (if they overlap or touch).
 * If they cross close to the end of one of the section we snap the positions 
 * to exact 0 or 1.
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
 * If they cross close to the end of one of the section we DO NOT snap the positions
 * to exact 0 or 1 (as opposed to CrossingSegments()).
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
        if (start==end)
            pos_my[num] = 0;
        else if (fabs(start.x-end.x) > fabs(start.y-end.y))
            pos_my[num] = (r[num].x-start.x)/(end.x-start.x);
        else
            pos_my[num] = (r[num].y-start.y)/(end.y-start.y);
        _ASSERT(pos_my[num]>=0 && pos_my[num]<=1);
        if (pos_my[num]>=0 && pos_my[num]<=1) num++;
    }
    return num;
}

const double CP_PRECISION = 1e-8;

/** Returns the crossing of a bezier with an infinite line.
 * We do not filter the result by position values falling outside [0,1]
 * Those that fall out in pos_my contains invalid values for pos_segment - should be
 * completely ignored by the caller.*/
unsigned Edge::CrossingLine(const XY &A, const XY &B, double pos_my[], double pos_segment[]) const
{
    _ASSERT(!straight);
    _ASSERT(!B.test_equal(A));
    //rotate and shift the bezier so that the line we cross becomes the X axis
    const double l = A.Distance(B);
    const double sin_r = (A.y-B.y)/l;
    const double cos_r = (B.x-A.x)/l;
    const Edge e = CreateShifted(-A).Rotate(cos_r, sin_r);
    //Now find the parameter values of crossing x, (where y=0)
    //A==start, B==c1, C==c2, D==end
    //Curve is: (-A+3B-3C+D)t^3 + (3A-6B+3C)t^2 + (-3A+3B)t + A
    const double coeff[4] = {e.start.y,
                             3*(e.c1.y-e.start.y),
                             3*(e.start.y-2*e.c1.y+e.c2.y),
                             -e.start.y+3*(e.c1.y-e.c2.y)+e.end.y};
    double cc[4] = {coeff[0], coeff[1], coeff[2], coeff[3]};
    if (fabs(cc[3])<1e-5)
        cc[3] = 0;
    unsigned num = solve_degree3(cc, pos_my);
    //x coordinates of the crosspoints divided by segment length serve as positions of the segment
    for (unsigned u = 0; u<num; u++) 
        if (between01_adjust(pos_my[u])) {
            //refine position found
            while(1) {
                double y = ((coeff[3]*pos_my[u] + coeff[2])*pos_my[u] + coeff[1])*pos_my[u] + coeff[0];
                _ASSERT(!isnan(y));
                if (fabs(y)<CP_PRECISION) break; //done. y is in pixel space (just rotated) so this means a precision of CP_PRECISION pixels
                //approximate with the derivative
                //Derivatie is: 3*(-A+3B-3C+D)t^2 + (6A-12B+6C)t + (-3A+3B)
                const double derive[3] = {coeff[1], 2*coeff[2], 3*coeff[3]};
                const double dy_per_dpos = (derive[2]*pos_my[u] +derive[1])*pos_my[u] + derive[0];
                //protect against horizontal tangent
                if (fabs(dy_per_dpos)<1e-10)
                    break;
                pos_my[u] -= y/dy_per_dpos;
            }
            const XY xy = e.Split(pos_my[u]);
            _ASSERT(test_zero(fabs(xy.y)));
            pos_segment[u] = xy.x/l;
        }
    return num;
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


/** Checks if the bounding box of the hull of two beziers overlap.
 * @param [in] o The other edge
 * @param [in] is_next If true 'o' is an edge following 'this' and we ignore if they 
 *             connect via this->end == o.start.
 * @returns True if there is overlap */
bool Edge::HullOverlap(const Edge &o, bool is_next) const
{
    const Block O = o.straight ? Block(o.start, o.end) : o.GetBezierHullBlock();
    const Block T =   straight ? Block(  start,   end) :   GetBezierHullBlock();
    if (!O.Overlaps(T)) return false;
    //test if 'o' and 'this' only overlaps at their start/endpoint
    if (!is_next || end!=o.start) return true;
    if (O.y.from != T.y.till && O.y.till != T.y.from)
        return true;
    if (O.x.from != T.x.till && O.x.till != T.x.from)
        return true;
    //OK, now we know that the two BBs touch only at a single vertex
    //since end==o.start, this can only be that point
    return false;
}

/** A helper to find edge crosspoints, to be called recursively.
 * We subdivide each edge and try to find crosspoints that way.
 * So 'this' and 'A' may be only part of the original edges we search
 * crosspoints of.
 * @param [in] A (part of) the other edge.
 * @param [out] r The resulting crosspoints.
 * @param [out] pos_my The positions of the resulting crosspoints on the
 *                     original edge of 'this'.
 * @param [out] pos_other The positions of the resulting crosspoints on the
 *                     original edge of 'A'.
 * @param [in] pos_my_mul The multiplier to apply to a found position on
 *             'this' to convert it to a position on the original of 'this'.
 * @param [in] pos_other_mul The multiplier to apply to a found position on
 *             'A' to convert it to a position on the original of 'A'.
 * @param [in] pos_my_offset The offset to add to a found (and multiplied)
 *             position on 'this' to convert it to a position on the original of 'this'.
 * @param [in] pos_other_offset The offset to add to a found (and multiplied)
 *             position on 'A' to convert it to a position on the original of 'A'.
 * @param [in] alloc_size The number of crosspoints for which 'r', 'pos_my' and
 *             'pos_other' has space for.
 * @returns the number of crosspoints found.*/
unsigned Edge::CrossingBezier(const Edge &A, XY r[], double pos_my[], double pos_other[],
                              double pos_my_mul, double pos_other_mul,
                              double pos_my_offset, double pos_other_offset, unsigned alloc_size) const
{
    _ASSERT(alloc_size);
    //If both straight, we calculate and leave
    if (straight) {
        if (!A.straight)
            return A.CrossingBezier(*this, r, pos_other, pos_my, pos_other_mul, pos_my_mul,
                                    pos_other_offset, pos_my_offset, alloc_size);
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
    if (!HullOverlap(A, false)) return 0;
    Edge M1, M2, A1, A2;
    Split(M1, M2);
    pos_my_mul /= 2;
    unsigned num;
    if (A.straight) {
        num = M1.CrossingBezier(A, r, pos_my, pos_other, pos_my_mul, pos_other_mul,
            pos_my_offset, pos_other_offset, alloc_size);
        if (num >= alloc_size) return num;
        num += M2.CrossingBezier(A, r+num, pos_my+num, pos_other+num, pos_my_mul, pos_other_mul,
            pos_my_offset+pos_my_mul, pos_other_offset, alloc_size-num);
        return num;
    }
    A.Split(A1, A2);
    pos_other_mul /= 2;
    num = M1.CrossingBezier(A1, r, pos_my, pos_other, pos_my_mul, pos_other_mul,
        pos_my_offset, pos_other_offset, alloc_size);
    if (num >= alloc_size) return num;
    num += M2.CrossingBezier(A1, r+num, pos_my+num, pos_other+num, pos_my_mul, pos_other_mul,
        pos_my_offset+pos_my_mul, pos_other_offset, alloc_size-num);
    if (num >= alloc_size) return num;
    num += M1.CrossingBezier(A2, r+num, pos_my+num, pos_other+num, pos_my_mul, pos_other_mul,
        pos_my_offset, pos_other_offset+pos_other_mul, alloc_size-num);
    if (num >= alloc_size) return num;
    num += M2.CrossingBezier(A2, r+num, pos_my+num, pos_other+num, pos_my_mul, pos_other_mul,
        pos_my_offset+pos_my_mul, pos_other_offset+pos_other_mul, alloc_size-num);
    return num;
}



/** Checks if this edge and `next` can be combined into a single edge.
* If so, also update "this" to be the combined edge & returns true.
* In pos returns the position of "next.start" in the new edge.*/
bool Edge::CheckAndCombine(const Edge &next, double *pos)
{
    if (straight!=next.straight) return false;
    if (straight) {
        //http://mathworld.wolfram.com/Point-LineDistance2-Dimensional.html
        const double m = fabs((start.x-next.end.x)*(end.x-start.x)-(end.y-start.y)*(start.y-next.end.y))/start.DistanceSqr(end);
        const double dist_sqr = next.end.Distance(start + (end-start)*m);
        if (dist_sqr<0.0001) {
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
    //XXX ToDo: do this for bezier curves
    return false;
}


/** Calculates the crosspoints with another edge.
*
* This must ensure that we do not return pos values that are very close to 0 or 1
* such values are to be rounded to 0 (or 1) in that case exatly the start point
* of the arc shall be returned. 
* In case the two edges overlap and have common sections, we return 2 and the two
* endpoint of the common section.
* The arrays shall be at least Edge::MAX_CP big.
*
* @param [in] A The other edge.
* @param [in] is_next If true we assume A follows *this (and thus do not report
*             if their start and endpoints (respectively) meet.
* @param [out] r The crosspoins.
* @param [out] pos_my The pos values of the corresponding crosspoints on me.
* @param [out] pos_other The pos values of the corresponding crosspoints on the other edge.
* @returns The number of crosspoins found [0..MAX_CP]
*/
unsigned Edge::Crossing(const Edge &A, bool is_next, XY r[Edge::MAX_CP],
    double pos_my[Edge::MAX_CP], double pos_other[Edge::MAX_CP]) const
{
    //A quick test: if their bounding box only meets at the joint
    //start/endpoint, we return here
    if (is_next && !HullOverlap(A, is_next)) return 0;
    const unsigned LOC_MAX_CP = MAX_CP*2;
    XY loc_r[LOC_MAX_CP+1];
    double loc_pos_my[LOC_MAX_CP+1], loc_pos_other[LOC_MAX_CP+1];
    unsigned ret = 0;
    unsigned num;
    if (straight) {
        if (A.straight) {
            ret = CrossingSegments(A, r, pos_my, pos_other);
            if (ret==1 && is_next && pos_my[0]==1 && pos_other[0]==1)
                return 0;
            goto snap_ends;
        } else { //A is curvy
            num = A.CrossingLine(start, end, loc_pos_other, loc_pos_my);
            for (unsigned u = 0; u<num; u++)
                if (between01_adjust(loc_pos_other[u]) && between01_adjust(loc_pos_my[u])) //test pos_other first, since if that is outside [0..1] pos_my is uninitialized
                    if (!is_next || loc_pos_my[u]!=1 || loc_pos_other[u]!=1) {
                        pos_my[ret] = loc_pos_my[u];
                        pos_other[ret] = loc_pos_other[u];
                        r[ret++] = A.Split(loc_pos_other[u]);
                    }
            if (ret==0) return 0;
            goto snap_ends;
        }
    }
    if (A.straight) {
        //'this' is curvy
        num = CrossingLine(A.start, A.end, loc_pos_my, loc_pos_other);
        for (unsigned u = 0; u<num; u++)
            if (between01_adjust(loc_pos_my[u]) && between01_adjust(loc_pos_other[u])) //test pos_my first, since if that is outside [0..1] pos_other is uninitialized
                if (!is_next || loc_pos_my[u]!=1 || loc_pos_other[u]!=1) {
                    pos_my[ret] = loc_pos_my[u];
                    pos_other[ret] = loc_pos_other[u];
                    r[ret++] = Split(loc_pos_my[u]);
                }
        if (ret==0) return 0;
        goto snap_ends;
    }
    //Now both are curvy
    num = CrossingBezier(A, loc_r, loc_pos_my, loc_pos_other, 1, 1, 0, 0, LOC_MAX_CP);
    //Snap the crosspounsigneds to the start of the curves
    for (unsigned i = 0; i<num; i++) {
        if (test_zero(loc_pos_my[i])) {
            loc_pos_my[i] = 0;
            loc_r[i] = start;
        } else if (test_equal(1, loc_pos_my[i])) {
            loc_pos_my[i] = 1;
            loc_r[i] = end;
        }
        if (test_equal(1, loc_pos_other[i])) {
            loc_pos_other[i] = 1;
            loc_r[i] = A.end;
        } else if (test_zero(loc_pos_other[i])) {
            loc_pos_other[i] = 0;
            loc_r[i] = A.start;
        }
    }
    //In case of multiple hits, we need to remove erroneously occuring ones
    //these are the ones, where both pos_my and pos_otehr are close
    //We do it pairwise
    for (unsigned i = 0; i<num; i++) {
        //We remove crosspounsigneds where our end meets A's start (if is_next is true)
        if (is_next && loc_pos_my[i]==1 && loc_pos_other[i]==0)
            continue;
        //if i==num-1 the below coop will not roll
        unsigned j = i+1;
        for (; j<num; j++)
            if ((test_equal(loc_pos_my[i], loc_pos_my[j]) && test_equal(loc_pos_other[i], loc_pos_other[j])))
                break;
        if (j==num) {
            //index 'i' did not match any later element - we keep it
            if (ret >= MAX_CP) {
                //we have too many crosspoints - maybe the two beziers run parallel at a segment?
                //Which of the endpoints are inside the other segment?
                struct data {
                    double d;
                    XY r;
                    double pos_my;
                    double pos_other;
                    bool operator <(const struct data &o) const { return d<o.d; }
                } d[4];
                d[0].d =   Distance(A.start, d[0].r, d[0].pos_my);     d[0].pos_other = 0; d[0].r = A.start;
                d[1].d =   Distance(A.end,   d[1].r, d[1].pos_my);     d[1].pos_other = 1; d[1].r = A.end;
                d[2].d = A.Distance(  start, d[2].r, d[2].pos_other);  d[2].pos_my = 0;    d[2].r = start;
                d[3].d = A.Distance(  end  , d[3].r, d[3].pos_other);  d[3].pos_my = 1;    d[3].r = end;
                std::sort(d, d+4);
                num = std::unique(d, d+4,   [&](const data &a, const data &b) {return test_equal(a.pos_my, b.pos_my); }) - d;
                num = std::unique(d, d+num, [&](const data &a, const data &b) {return test_equal(a.pos_other, b.pos_other); }) - d;
                _ASSERT(num>=2);
                _ASSERT(d[1].d<0.1);
                for (unsigned uu = 0; uu<2; uu++) {
                    r[uu] = d[uu].r;
                    pos_my[uu] = d[uu].pos_my;
                    pos_other[uu] = d[uu].pos_other;
                }
                return 2;
            }
            r[ret] = loc_r[i];
            pos_my[ret] = loc_pos_my[i];
            pos_other[ret] = loc_pos_other[i];
            ++ret;
        }
    }
snap_ends:
    //if one is a horizontal/vertical straight line, we need to snap the cp to that line
    if (straight) {
        if (start.x==end.x)
            std::for_each(r, r+ret, [&](XY &a) {a.x = start.x; });
        else if (start.y==end.y)
            std::for_each(r, r+ret, [&](XY &a) {a.y = start.y; });
    } else if (A.straight) {
        if (A.start.x==A.end.x)
            std::for_each(r, r+ret, [&](XY &a) {a.x = A.start.x; });
        else if (A.start.y==A.end.y)
            std::for_each(r, r+ret, [&](XY &a) {a.y = A.start.y; });
    }
    return ret;
}

/** Finds where an edge crosses itself.
 * @param [out] r The crosspoints
 * @param [out] pos1 The positions associated with the crosspoints.
 * @param [out] pos2 The other positions associated with the crosspoints.
 * @returns the number fof crosspoints found.*/
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
    if (roots[0]==0 || roots[0]==1) {
        if (num==1) return 0;
        if (roots[1]==0 || roots[1]==1) return 0;
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
        num = A.CrossingBezier(B, r, pos1, pos2, roots[0], 1-roots[0], 0, roots[0], MAX_CP);
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
        num = A.CrossingBezier(B, r, pos1, pos2, roots[0], 1-roots[1], 0, roots[1], MAX_CP);
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
        return 0;
    std::sort(pos, pos+ret);
    for (unsigned i = 0; i<ret; i++) {
        //refine cp
        const double coeff[4] = {start.x,
                                 3*(c1.x-start.x),
                                 3*(start.x-2*c1.x+c2.x),
                                 -start.x+3*(c1.x-c2.x)+end.x};
        while (1) {
            const double xx = ((coeff[3]*pos[i] + coeff[2])*pos[i] + coeff[1])*pos[i] + coeff[0];
            if (fabs(xx-x)<CP_PRECISION) break; //done. This means a precision of CP_PRECISION pixels
            //approximate with the derivative
            //Derivatie is: 3*(-A+3B-3C+D)t^2 + (6A-12B+6C)t + (-3A+3B)
            const double derive[3] = {coeff[1], 2*coeff[2], 3*coeff[3]};
            const double dx_per_dpos = (derive[2]*pos[i] +derive[1])*pos[i] + derive[0];
            //protect against vertical tangent
            if (fabs(dx_per_dpos)<1e-10)
                break;
            pos[i] += (x-xx)/dx_per_dpos;
        }
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
                return 0;
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
                return 0;
            for (unsigned u = 0; u<ret; u++) {
                forward[u] = forward[u+1];
                y[u] = y[u+1];
                pos[u] = pos[u+1];
            }
    }
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
        //test for start and endpoints - anything that goes through xy is ignored
        if (start == xy) return 0; // start.y > end.y ? -1 : 0;
        if (end == xy) return 0; // start.y < end.y ? +1 : 0;
        //test if we cross right of xy.x
        if ((xy.y - start.y)/(end.y - start.y)*(end.x-start.x) + start.x <= xy.x)
            return 0;
        //Ok, we should not be ignored, return +1 or -1
            return start.y < xy.y ? +1 : -1;
    }
    //Here we handle beziers
    //Check if we are far off.
    const Block b = GetBezierHullBlock();
    if (!test_smaller(b.y.from, xy.y) || test_smaller(b.y.till, xy.y) || test_smaller(b.x.till, xy.x))
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
    if (close>(self ? 1U : 0U)) {
        //refine the relevant cps
        for (unsigned i = 0; i<num; i++) {
            if (x[i]+threshold < xy.x) continue;
            if (x[i]-threshold > xy.x) continue;
            //refine this cp
            //A==start, B==c1, C==c2, D==end
            //Curve is: (-A+3B-3C+D)t^3 + (3A-6B+3C)t^2 + (-3A+3B)t + A
            const double coeff[4] = {start.y,
                                     3*(c1.y-start.y),
                                     3*(start.y-2*c1.y+c2.y),
                                     -start.y+3*(c1.y-c2.y)+end.y};
            while (1) {
                double yy = ((coeff[3]*pos[i] + coeff[2])*pos[i] + coeff[1])*pos[i] + coeff[0];
                if (fabs(xy.y-yy)<CP_PRECISION) break; //done. y is in pixel space (just rotated) so this means a precision of CP_PRECISION pixels
                //approximate with the derivative
                //Derivatie is: 3*(-A+3B-3C+D)t^2 + (6A-12B+6C)t + (-3A+3B)
                const double derive[3] = {coeff[1], 2*coeff[2], 3*coeff[3]};
                const double dy_per_dpos = (derive[2]*pos[i] +derive[1])*pos[i] + derive[0];
                //protect against horizontal tangent
                if (fabs(dy_per_dpos)<1e-10)
                    break;
                pos[i] += (xy.y-yy)/dy_per_dpos;
            }
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
        return RayAngle(incoming ? angle_to_horizontal(end, start) :
                                   angle_to_horizontal(start, end));

    //bezier curve radius calc, see http://blog.avangardo.com/2010/10/c-implementation-of-bezier-curvature-calculation/

    typedef long double real;

    //first get the derivative
    const real pos2 = pos * pos;
    const real s0 = -3 + 6 * pos - 3 * pos2;
    const real s1 = 3 - 12 * pos + 9 * pos2;
    const real s2 = 6 * pos - 9 * pos2;
    const real s3 = 3 * pos2;
    const real d1x = start.x * s0 + c1.x * s1 + c2.x * s2 + end.x * s3;
    const real d1y = start.y * s0 + c1.y * s1 + c2.y * s2 + end.y * s3;

    //then get second derivative
    const real ss0 = 6 - 6 * pos;
    const real ss1 = -12 + 18 * pos;
    const real ss2 = 6 - 18 * pos;
    const real ss3 = 6 * pos;
    const real d2x = start.x * ss0 + c1.x * ss1 + c2.x * ss2 + end.x * ss3;
    const real d2y = start.y * ss0 + c1.y * ss1 + c2.y * ss2 + end.y * ss3;

    //then the radius
    const real r1 = sqrt(pow(d1x * d1x + d1y * d1y, 3));
    const real r2 = d1x * d2y - d2x * d1y;
    const real invRadius = r2 / r1; //the inverse of the (signed) radius

    if (pos==0) //must be outgoing
        return RayAngle(angle_to_horizontal(start, c1), invRadius);
    if (pos==1) //must be incoming
        return RayAngle(angle_to_horizontal(end, c2), -invRadius);
    XY A, B;
    const XY C = Split(pos, A, B);
    return incoming ? RayAngle(angle_to_horizontal(C, A), -invRadius) :
                      RayAngle(angle_to_horizontal(C, B), invRadius);
}

/** Returns either the x or the y coordinate of 'p' depending on the value of 'i'*/
inline double COORD(const XY &p, unsigned i) { return i ? p.y : p.x; }

/** Calculates the tight bounding box of 'this'.
 * Moderately expensive.*/
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

/** Returns the position of (one of) the rightmost point of the edge */
XY Edge::XMaxExtreme(double &p) const
{
    if (straight) {
        p = start.x>end.x ? 0. : 1.;
        return start.x>end.x ? start : end;
    }

    //from: http://stackoverflow.com/questions/2587751/an-algorithm-to-find-bounding-box-of-closed-bezier-curves
    std::vector<double> pos;
    std::vector<XY> xy;
    pos.reserve(6);   xy.reserve(6);
    pos.push_back(0); xy.push_back(start);
    pos.push_back(1); xy.push_back(end);

    double b = 6 * start.x - 12 * c1.x + 6 * c2.x;
    double a = -3 * start.x + 9 * c1.x - 9 * c2.x + 3 * end.x;
    double c = 3 * c1.x - 3 * start.x;
    if (a == 0) {
        if (b != 0) {
            double t = -c / b;
            if (0 < t && t < 1) {
                pos.push_back(t);
                xy.push_back(Split(t));
            }
        }
    } else {
        double b2ac = pow(b, 2) - 4 * c * a;
        if (b2ac >= 0) {
            double  t1 = (-b + sqrt(b2ac))/(2 * a);
            if (0 < t1 && t1 < 1) {
                pos.push_back(t1);
                xy.push_back(Split(t1));
            }
            double  t2 = (-b - sqrt(b2ac))/(2 * a);
            if (0 < t2 && t2 < 1) {
                pos.push_back(t2);
                xy.push_back(Split(t2));
            }
        }
    }
    const auto X = std::max_element(xy.begin(), xy.end(),
                   [](const XY &a, const XY &b) {return a.x<b.x; });
    p = pos[X - xy.begin()];
    return *X;
}



/**Solves a cubic equation of x^3 + a*x^2 + b*x + c
 * Returns the number of roots.*/
int solveCubic(double a, double b, double c, double* r)
{
    double p = b - a*a / 3;
    double q = a * (2*a*a - 9*b) / 27 + c;
    double p3 = p*p*p;
    double d = q*q + 4*p3 / 27;
    double offset = -a / 3;
    if (d >= 0) { // Single solution
        double z = sqrt(d);
        double u = (-q + z) / 2;
        double v = (-q - z) / 2;
        u = pow(u,1/3);
        v = pow(v, 1/3);
        r[0] = offset + u + v;
        return 1;
    }
    double u = sqrt(-p / 3);
    double v = acos(-sqrt(-27 / p3) * q / 2) / 3;
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


/** Calculates the distance of a point from the edge.
 * @param [in] M The point to calculate the distance of.
 * @param [out] point The point of 'this' closest to 'M'.
 * @param [out] pos The position corresponding to 'point'.
 * @returns the distance, always nonnegative.*/
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

/** Merges the distance of 'o' from 'this' to 'ret'.
 * That is, if 'o' is closer to 'this' than the distance stored in 'ret',
 * we update 'ret' with this new distance.*/
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
        const XY off(Edge_CreateExpand2D::comp_dbl(end.y, start.y, gap.x),
                     Edge_CreateExpand2D::comp_dbl(start.x, end.x, gap.y));
        ret.emplace_back(start+off, end+off, bool(!!visible));
        Edge &e = ret.back();
        //expand for horizontal and vertical edges
        if (start.x == end.x) {
            e.start.y += Edge_CreateExpand2D::comp_dbl(start.y, end.y, fabs(gap.y));
            e.end.y -= Edge_CreateExpand2D::comp_dbl(start.y, end.y, fabs(gap.y));
        }
        if (start.y == end.y) {
            e.start.x += Edge_CreateExpand2D::comp_dbl(start.x, end.x, fabs(gap.x));
            e.end.x -= Edge_CreateExpand2D::comp_dbl(start.x, end.x, fabs(gap.x));
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
    for (double &v : bounds)
        if (test_zero(v))
            v = 0;
        else if (test_equal(1, v))
            v = 1;
    std::sort(bounds.begin(), bounds.end());
    bounds.erase(std::unique(bounds.begin(), bounds.end(), [](double a, double b) {return test_equal(a, b); }), bounds.end());
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


/** Finds a crosspoint of two infinite lines defined by `A`->`B` and `M`->`N`
*/
ELineCrossingType crossing_line_line(const XY &A, const XY &B, const XY &M, const XY &N, XY &r)
{
    const double perp = (B-A).PerpProduct(N-M);
    if (test_zero(perp)) return LINE_CROSSING_PARALLEL;
    //They are not parallel (and none of them are degenerate)
    const double t = (B-A).PerpProduct(A-M) / perp;
    r = M + (N-M)*t;
    //in case of horizontal or vertical lines, ensure that cp lies *exactly* on them
    if (A.x==B.x) r.x = A.x;
    else if (M.x==N.x) r.x = M.x;
    if (A.y==B.y) r.y = A.y;
    else if (M.y==N.y) r.y = M.y;
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
* @param [in] my_next_tangent The ideal next tangent of *this. 
*                             Used to detect parallel joins and for calculating CP_EXTENDED crosspoints.
* @param [in] Ms_prev_tangent The ideal previous tangent of M.
*                             Used to detect parallel joins and for calculating CP_EXTENDED crosspoints.
* @param [in] is_my_origin_bezier True, if the original edge of *this was a bezier. Used when calculating
*                                 'newcp' for parallel joins.
* @param [in] is_Ms_origin_bezier True, if the original edge of M was a bezier. Used when calculating
*                                 'newcp' for parallel joins.
* @param [out] my_pos In case of CP_REAL returns the position of the cp on 'this', if it is a bezier.
* @param [out] M_pos In case of CP_REAL returns the position of the cp on M, if that is a bezier.
* @returns The relation of the two expanded edges.
*/
Edge::EExpandCPType Edge::FindExpandedEdgesCP(const Edge&M, XY &newcp, 
                                              const XY &my_next_tangent, const XY &Ms_prev_tangent,
                                              bool is_my_origin_bezier, bool is_Ms_origin_bezier,
                                              double &my_pos, double &M_pos) const
{
    if (start.test_equal(end) || M.start.test_equal(M.end)) return CP_DEGENERATE;
    if (end.test_equal(M.start)) {
        newcp = end;
        return CP_TRIVIAL;
    }
    const double parallel_join_multipiler = 5;
    if (M.straight) {
        if (straight) {
            if (LINE_CROSSING_INSIDE == crossing_line_line(start, end, M.start, M.end, newcp)) 
                return CP_REAL;
            //fallthrough to very end if no real crosspoints
        } else {
            //we are bezier, but M is straight
            XY r[8];
            double pos_us[8], pos_M[8];
            int num = Crossing(Edge(M.start, M.end), false, r, pos_us, pos_M);
            if (num) {
                //if several crosspoints: use the one closest to the end of the bezier
                unsigned pos = 0;
                for (int u = 1; u<num; u++)
                    if (pos_us[u]> pos_us[pos])
                        pos = u;
                newcp = r[pos];
                my_pos = pos_us[pos];
                return CP_REAL;
            }
            //fallthrough to very end if no real crosspoints
        }
    } else {
        XY r[8];
        if (straight) {
            //we are a straight, but M is bezier
            double pos_us[8], pos_M[8];
            int num = M.Crossing(*this, false, r, pos_M, pos_us);
            if (num) {
                //if several crosspoints: use the one closest to the start of the bezier
                unsigned pos = 0;
                for (int u = 1; u<num; u++)
                    if (pos_M[u] < pos_M[pos])
                        pos = u;
                newcp = r[pos];
                M_pos = pos_M[pos];
                return CP_REAL;
            }
            //fallthrough to very end if no real crosspoints
        } else {
            //both are beziers
            double pos_us[8], pos_M[8];
            int num = Crossing(M, false, r, pos_us, pos_M);
            if (num) {
                //several crosspoints: use the one closest to the end of us
                unsigned pos = 0;
                for (int u = 1; u<num; u++)
                    if (pos_us[u]> pos_us[pos])
                        pos = u;
                newcp = r[pos];
                my_pos = pos_us[pos];
                M_pos = pos_M[pos];
                return CP_REAL;
            }
            //fallthrough to very end if no real crosspoints
        }
    }

    //We fall through here from all branches if the two edges do not actually cross themselves
    //We test if their linear extension meet
    switch (crossing_line_line(end, my_next_tangent, Ms_prev_tangent, M.start, newcp)) {
    case LINE_CROSSING_INSIDE:
    case LINE_CROSSING_OUTSIDE_FW:
        //They meet, no problem, we have an extended CP
        return CP_EXTENDED;
    default:
        _ASSERT(0); //fallthrough
    case LINE_CROSSING_OUTSIDE_BK:
        //They meet but on the wrong side: an inverse join situ
        return CP_INVERSE;
    case LINE_CROSSING_PARALLEL:
        //if we are parallel, calculate a convergence point
        const double dist = end.Distance(M.start) * parallel_join_multipiler;
        if (is_Ms_origin_bezier) {
            const double len = end.Distance(my_next_tangent);
            if (is_my_origin_bezier)
                newcp = (end+M.start)/2 - (end-my_next_tangent)/len*dist;
            else 
                newcp = end + (my_next_tangent-end)/len*dist;
        } else {
            _ASSERT(is_my_origin_bezier); //two originally straight edges should not join parallel
            const double mlen = M.start.Distance(Ms_prev_tangent);
            newcp = M.start + (Ms_prev_tangent-M.start)/mlen*dist;
        }
        return NO_CP_PARALLEL;
    }
}

/** Creates a circle, ellipse or a segment of it. 
 * @param [out] append_to append the resulting edge(s) to this collection.
 * @param [in] c The center of the circle/ellipse.
 * @param [in] radius_x The radius along the x axis. Must be positive.
 * @param [in] radius_y The radius along the x axis. Must be nonnegative. 
 *                      If zero, we use radius_x instead. 
 * @param [in] tilt_deg Degrees by which we rotate the ellipse clockwise 
 *                      (ignored if radius_x==radius_y)
 * @param [in] s_deg Degrees where to start the arc. 
 * @param [in] d_deg Degrees where to end the arc. Both d_deg and s_deg are 
 *                   counted from the "right" direction clockwise.
 * @param [in] clockwise If true the arc goes clockwise from s_deg to d_deg.*/
void Edge::GenerateEllipse(std::vector<Edge> &append_to, const XY &c, double radius_x, double radius_y, 
                           double tilt_deg, double s_deg, double d_deg, bool clockwise)
{
    const size_t original_size = append_to.size();
    if (!clockwise) 
        std::swap(s_deg, d_deg);
    s_deg = fmod_negative_safe(s_deg, 360.)/90;
    d_deg = fmod_negative_safe(d_deg, 360.)/90;
    if (d_deg<=s_deg) d_deg += 4;
    _ASSERT(radius_x>0 && radius_y>=0);
    if (radius_x<=0)
        return;
    if (radius_y==0)
        radius_y = radius_x;
    else if (radius_y<0)
        return;
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
    /* Coefficients of implicit
     * eqn for line from V[0]-V[deg]
     * Derive the implicit equation for line connecting first
     *  and last control points */
    const double a = V[0].y - V[5].y;
    const double b = V[5].x - V[0].x;
    const double c = V[0].x * V[5].y - V[5].x * V[0].y;

    double  max_distance_above = 0;
    double  max_distance_below = 0;

    for (unsigned i = 1; i < 5; i++) {
        const double value = a * V[i].x + b * V[i].y + c;
        if (value > max_distance_above) {
            max_distance_above = value;
        } else if (value < max_distance_below) {
            max_distance_below = value;
        }
    }
    const double error_better = fabs((max_distance_below-max_distance_above)/a);
    return error_better < EPSILON;
}


//bool ControlPolygonFlatEnough_Original(const XY V[6])
//{
//    int     i;        /* Index variable        */
//    double  max_distance_above = 0;
//    double  max_distance_below = 0;
//    double  error;        /* Precision of root        */
//    double  intercept_1,
//            intercept_2,
//            left_intercept,
//            right_intercept;
//    double  det, dInv;
//    double  a2, b2, c2;
//
//    /* Coefficients of implicit
//     * eqn for line from V[0]-V[deg]
//     * Derive the implicit equation for line connecting first
//     *  and last control points */
//    const double a = V[0].y - V[5].y;
//    const double b = V[5].x - V[0].x;
//    const double c = V[0].x * V[5].y - V[5].x * V[0].y;
//
//
//    const double abSquared = (a * a) + (b * b);
//
//    for (i = 1; i < 5; i++) {
//        /* Compute distance from each of the points to that line    */
//        const double distance = a * V[i].x + b * V[i].y + c;
//        if (distance > 0.0)
//            max_distance_above = std::max((distance * distance) / abSquared, max_distance_above);
//        else if (distance < 0.0)
//            max_distance_below = std::min(-(distance * distance) / abSquared, max_distance_below);
//    }
//    /*  Implicit equation for zero line */
//    const double a1 = 0.0;
//    const double b1 = 1.0;
//    const double c1 = 0.0;
//
//    /*  Implicit equation for "above" line */
//    a2 = a;
//    b2 = b;
//    c2 = c + max_distance_above;
//
//    det = a1 * b2 - a2 * b1;
//    dInv = 1.0/det;
//
//    intercept_1 = (b1 * c2 - b2 * c1) * dInv;
//
//    /*  Implicit equation for "below" line */
//    a2 = a;
//    b2 = b;
//    c2 = c + max_distance_below;
//
//    det = a1 * b2 - a2 * b1;
//    dInv = 1.0/det;
//
//    intercept_2 = (b1 * c2 - b2 * c1) * dInv;
//
//    /* Compute intercepts of bounding box    */
//    left_intercept = std::min(intercept_1, intercept_2);
//    right_intercept = std::max(intercept_1, intercept_2);
//
//    error = 0.5 * (right_intercept-left_intercept);
//    const double error_better = fabs((max_distance_below-max_distance_above)/a)/2;
//
//    return (error_better < EPSILON) ? 1 : 0;
//}

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
        if (ControlPolygonFlatEnough(w)) {
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
unsigned Edge::SolveForDistance(const XY &p, double pos[5]) const
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
    return std::unique(roots, roots+ret) - roots;  //remove duplicate solutions
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
    return std::unique(roots, roots+ret) - roots;  //remove duplicate solutions
}

/** Expands the edge.
*
* This takes the direction of the edge to determine the 'outside' side of the
* edge to expand towards (or away from in case of a negative `gap`).
* We also return a previous and next tangent points of the expanded curve.
* This is because a bezier might have been converted to a servies of sub-curves at
* expansion and the last/first one of them may have degenerated to a line.
* In this case some of the Expand() algorithms fail.
* The points returned represent the 'ideal' tangents as if the lines did not
* degenerate. We return them even if nothing degenerated even for straight edges.
* @param [in] gap The amount to expand (or shrink if <0)
* @param [out] expanded Append the expanded edges to this. The elements of this container
*                       must have a constructor (XY,XY,XY,XY,bool) for beziers and one
*                       (XY,XY,bool) for straight lines.
* @param [out] prev_tangent A point that is a previous tangent for the expanded edge.
* @param [out] next_tangent A point that is a next tangent for the expanded edge.
* @param [out] original Append the original edge to this. If the expansion
*                       requires to split the edge to several pieces append
*                       the split chunks of the original here.
*/
bool Edge::CreateExpand(double gap, std::list<Edge> &expanded, XY &prev_tangent, XY &next_tangent,
                        std::vector<Edge> *original) const
{
    _ASSERT(!IsDot());
    if (IsDot()) return false;
    if (straight) {
        const double length = start.Distance(end);
        const XY wh = (end-start).Rotate90CCW()/length*gap;
        expanded.emplace_back(start+wh, end+wh, bool(!!visible)); //Visual Studio complains for bitfields in such templates
        if (original)
            original->push_back(*this);
        prev_tangent = 2*start-end + wh;
        next_tangent = 2*end-start + wh;
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
    for (unsigned k = t.size()-1; k>0; k--)
        if (t[k] < SMALL_NUM || t[k]>1-SMALL_NUM)
            t.erase(t.begin()+k);
    std::sort(++t.begin(), t.end());
    t[0] = 0;
    t.push_back(1);
    //remove duplicates (like a cusp at an X extreme)
    for (int k = t.size()-2; k>=0; k--)
        if (test_equal(t[k], t[k+1]))
            t.erase(t.begin()+k+1);

    std::list<Edge> tmp;
    const size_t orig_offset = original ? original->size() : 0;
    _ASSERT(t.size()>=2);
    if (t.size()==2) {
        _ASSERT(t[0]==0 && t[1]==1);
        CreateExpandOneSegment(gap, tmp, original);
    } else {
        for (unsigned k = 0; k<t.size()-1; k++) {
            Edge e(*this, t[k], t[k+1]);
            e.CreateExpandOneSegment(gap, tmp, original);
            //If e degenerated to a line, we need to adjust the endpoints to
            //expand along the original tangents.
            if (e.IsStraight() && k==0)
                tmp.front().start = start + (c1-start).Rotate90CCW().Normalize()*gap;
            if (e.IsStraight() && k==t.size()-1)
                tmp.back().end = end + (end-c2).Rotate90CCW().Normalize()*gap;
        }
    }

    //Remove loops in the created set - beware some of them degenerated to a segment
    RemoveLoop(tmp, tmp.begin(), tmp.end(), true, original, orig_offset);
    if (tmp.size()==0) return false;

    //Make sure connecting edges really connect
    for (auto i = tmp.begin(), j = ++tmp.begin(); j!=tmp.end(); i++, j++)
        i->end = j->start = (i->end + j->start)/2;
    //Generate tangent points. These point towards the original direction of the edge ends
    //and ignore if the ends changed direction.
    prev_tangent = tmp.front().GetStart() + (PrevTangentPoint(0)-start);
    next_tangent = tmp.back().GetEnd() + (NextTangentPoint(1)-end);
    expanded.splice(expanded.end(), tmp);
    return true;
}

/** Helper for CreateExpand(). */
bool Edge::CreateExpandOneSegment(double gap, std::list<Edge> &expanded, std::vector<Edge> *original) const
{
    if (straight) {
        const double length = start.Distance(end);
        const XY wh = (end-start).Rotate90CCW()/length*gap;
        expanded.emplace_back(start+wh, end+wh, bool(!!visible)); //Visual Studio complains for bitfields in such templates
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
    expanded.emplace_back(new_start, new_end, new_c1, new_c2, bool(!!visible));

    //Test error of the resulting offset curve
    const double EPSILON = 0.1;
    //offset the middle point of the result along the normal of the result back onto the original
    const XY Mid = expanded.back().Split();
    const XY Normal = (expanded.back().NextTangentPoint(0.5)-Mid).Rotate90CW()+Mid;
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
        std::list<Edge> tmp;
        E1.CreateExpandOneSegment(gap, tmp, original);
        E2.CreateExpandOneSegment(gap, tmp, original);
        //Set the endpoints as we have calculated here. These are to prevent
        //mis-aligned expansion if E1 or E2 became straight by the split above
        tmp.front().start = new_start;
        tmp.back().end = new_end;
        expanded.splice(expanded.end(), tmp);
    } else 
        //else we are OK with the emplaced offset curve
        if (original)
            original->push_back(*this);

    return true;
}

/** Determines how much one edge is below us.
 * @param [in] o The other edge to consider.
 * @param [out] touchpoint The y coordinate where 'o' would touch 
 *                         us if shifted upwards by what we return.
 * @param [in] min_so_far If o is further down below us by more than this value,
 *                        return this value. This also allows optimization for
 *                        bezier curves: if the hull or bounding box of 'o' is 
 *                        below our hull or bounding box more than 'min_so_far'
 *                        we do not need to calculate the exact offset between
 *                        us and 'o', just return min_so_far.
 * @returns how much 'o' should be shifted upwards to touch us. If o is above us
 *          or overlaps us, we return a negative value (since o need shifting
 *          down). If o is besides us, we consider the offset CONTOUR_INFINITE
 *          and return min_so_far.*/
double Edge::OffsetBelow(const Edge &o, double &touchpoint, double min_so_far) const
{
    const Range AB = GetHullXRange();
    const Range MN = o.GetHullXRange();
    if (!AB.Overlaps(MN)) return min_so_far;
    if (GetHullYRange().till+min_so_far <= o.GetHullYRange().from)
        return min_so_far;

    if (straight && o.straight) {
        //calc for two straight edges
        const double x1 = std::max(AB.from, MN.from);
        const double x2 = std::min(AB.till, MN.till);
        if (start.x == end.x) {
            double off;
            if (o.GetStart().x == o.GetEnd().x)
                off = std::min(o.GetStart().y, o.GetEnd().y) - std::max(start.y, end.y); //here all x coordinates must be the same
            else 
                off = (o.GetEnd().y-o.GetStart().y)/(o.GetEnd().x-o.GetStart().x)*(start.x-o.GetStart().x) + o.GetStart().y -
                      std::max(start.y, end.y);
            if (off<min_so_far) {
                touchpoint = std::max(start.y, end.y);
                return off;
            }
            return min_so_far;
        }
        if (o.GetStart().x == o.GetEnd().x) {
            double tp = (start.y-end.y)/(start.x-end.x)*(o.GetStart().x-start.x) + start.y;
            double off = std::min(o.GetStart().y, o.GetEnd().y) - tp;
            if (off<min_so_far) {
                touchpoint = tp;
                return off;
            }
            return min_so_far;
        }
        const double y1 = ((start.y-end.y)/(start.x-end.x)*(x1-start.x) + start.y);
        const double y2 = ((start.y-end.y)/(start.x-end.x)*(x2-start.x) + start.y);
        const double diff1 = (o.GetEnd().y-o.GetStart().y)/(o.GetEnd().x-o.GetStart().x)*(x1-o.GetStart().x) + o.GetStart().y - y1;
        const double diff2 = (o.GetEnd().y-o.GetStart().y)/(o.GetEnd().x-o.GetStart().x)*(x2-o.GetStart().x) + o.GetStart().y - y2;
        if (std::min(diff1, diff2)<min_so_far) {
            touchpoint = diff1<diff2 ? y1 : y2;
            return std::min(diff1, diff2);
        }
        return min_so_far;
    }
    if (straight) {
        if (!o.CreateBoundingBox().x.Overlaps(Range(std::min(start.x, end.x), std::max(start.x, end.x))))
            return min_so_far;
        Edge E1, E2;
        o.Split(E1, E2);
        min_so_far = OffsetBelow(E1, touchpoint, min_so_far);
        min_so_far = OffsetBelow(E1, touchpoint, min_so_far);
        return min_so_far;
    }
    if (o.straight) {
        if (!CreateBoundingBox().x.Overlaps(Range(std::min(o.start.x, o.end.x), std::max(o.start.x, o.end.x))))
            return min_so_far;
        Edge E1, E2;
        Split(E1, E2);
        min_so_far = E1.OffsetBelow(o, touchpoint, min_so_far);
        min_so_far = E2.OffsetBelow(o, touchpoint, min_so_far);
        return min_so_far;
    }
    if (!CreateBoundingBox().x.Overlaps(o.CreateBoundingBox().x))
        return min_so_far;
    Edge E1, E2, F1, F2;
    Split(E1, E2);
    o.Split(F1, F2);
    min_so_far = E1.OffsetBelow(F1, touchpoint, min_so_far);
    min_so_far = E2.OffsetBelow(F1, touchpoint, min_so_far);
    min_so_far = E1.OffsetBelow(F2, touchpoint, min_so_far);
    min_so_far = E2.OffsetBelow(F2, touchpoint, min_so_far);
    return min_so_far;
}

/** Helper to TangentFrom */
void Edge::TangentFromRecursive(const XY &from, XY &clockwise, XY &cclockwise) const
{
    if (straight) {
        if (CLOCKWISE == triangle_dir(from, clockwise, start))
            clockwise = start;
        if (COUNTERCLOCKWISE == triangle_dir(from, cclockwise, start))
            clockwise = start;
    } else {
        Edge E1, E2;
        Split(E1, E2);
        E1.TangentFromRecursive(from, clockwise, cclockwise);
        E2.TangentFromRecursive(from, clockwise, cclockwise);
    }
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
    clockwise = cclockwise = start;
    if (straight)
        return;
    Edge E1, E2;
    Split(E1, E2);
    E1.TangentFromRecursive(from, clockwise, cclockwise);
    E2.TangentFromRecursive(from, clockwise, cclockwise);
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





} //namespace contour 
