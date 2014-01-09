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

/** @file contour_bezier.cpp Defines Bezier calculation code
* @ingroup contour_files
*/

#include "contour_bezier.h"
#include <vector>

namespace contour_bezier_edge {

    using contour::angle;
    using contour::between01_approximate_inclusive;

XY Edge::Split(double t) const
{
    return pow(1-t, 3) * start
            + 3 * pow(1-t, 2) * t * c1
            + 3 * (1-t) * pow(t, 2) * c2
            + pow(t, 3) * end;
    //(-A+3B-3C+D)t^3 + (3A-6B+3C)t^2 + (-3A+3B)t + A
}


/** Splits to two bezier curves 
 * The first will be start, ret, c1, r1,
 * The second will be ret, end, r2, c2 */
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

void Edge::Chop(double t, double s)
{
    _ASSERT(t>=0 && s>=0 && t<=1 && s<=1 && s!=t);
    if (s<t) std::swap(s, t);
    XY r1, r2;
    if (s<1) {
        end = Split(s, r1, r2);
        c1 = Mid(start, c1, s);
        c2 = r2;
        t /= s;
    }
    if (t>0) {
        start = Split(t, r1, r2);
        c1 = r1;
        c2 = Mid(c2, end, t);
    }
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
    if (s<0 || s>1) return false;
    t = fabs(A2.x-A1.x) > fabs(A2.y-A1.y) ? 
            (s*(B2.x-B1.x) + B1.x-A1.x)/(A2.x-A1.x) : 
            (s*(B2.y-B1.y) + B1.y-A1.y)/(A2.y-A1.y);
    return 0<=t && t<=1 ? 1 : 0;
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

/** Returns 16*flatness^2 */
double Edge::Flatness() const
{
    const double ux = 3.0*c1.x - 2.0*start.x - end.x; 
    const double uy = 3.0*c1.y - 2.0*start.y - end.y; 
    const double vx = 3.0*c2.x - 2.0*end.x - start.x; 
    const double vy = 3.0*c2.y - 2.0*end.y - start.y; 
    return std::max(ux*ux, vx*vx)+std::max(uy*uy, vy*vy);
}

/* Returns the number of crosspoints*/
unsigned Edge::CrossingBezier(const Edge &A, XY r[], double pos_my[], double pos_other[],
                              double pos_my_mul, double pos_other_mul, 
                              double pos_my_offset, double pos_other_offset) const
{
    //If both straight, we calculate and leave
    if (straight) {
        if (!A.straight)
            return A.CrossingBezier(*this, r, pos_other, pos_my, pos_other_mul, pos_my_mul,
                                    pos_other_offset, pos_my_offset);
        double t, s;
        switch (Cross(start, end, A.start, A.end, t, s)) {
        default:
            _ASSERT(0); //fallthrough
        case 0: 
            return 0;
        case 1: 
            pos_my[0] = t*pos_my_mul+pos_my_offset;
            pos_other[0] = s*pos_other_mul+pos_other_offset;
            r[0] = start + (end-start)*t;
            return 1;
        case 2:
            if (s<t) std::swap(t, s);
            if (t<0) {
                r[0] = start;
                pos_my[0] = 0*pos_my_mul + pos_my_offset;
                pos_other[0] = (fabs(A.end.x-A.start.x) > fabs(A.end.y-A.start.y) ?
                                    (start.x-A.start.x)/(A.end.x-A.start.x) :
                                    (start.y-A.start.y)/(A.end.y-A.start.y)) * pos_other_mul +
                               pos_other_offset;
            } else {
                r[0] = start + (end-start)*t;
                pos_my[0] = t*pos_my_mul + pos_my_offset;
                pos_other[0] = 0*pos_other_mul+pos_other_offset;
            }
            if (s>1) {
                r[1] = end;
                pos_my[1] = 1*pos_my_mul + pos_my_offset;
                pos_other[1] = (fabs(A.end.x-A.start.x) > fabs(A.end.y-A.start.y) ?
                    (end.x-A.start.x)/(A.end.x-A.start.x) :
                    (end.y-A.start.y)/(A.end.y-A.start.y)) * pos_other_mul +
                    pos_other_offset;
            } else {
                r[1] = start + (end-start)*s;
                pos_my[1] = s*pos_my_mul + pos_my_offset;
                pos_other[1] = 1*pos_other_mul+pos_other_offset;
            }
            return 2;
        }
    }
    if (!HullOverlap(A)) return 0;
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

/** Calculates the crosspoints with another edge.
*
* This must ensure that we do not return pos values that are very close to 0 or 1
* such values are to be rounded to 0 or 1 and in that case exatly the endpoint 
* of the arc shall be returned. (FIX This)
* The arrays shall be greater than 4, at least twice.
*
* @param [in] o The other edge.
* @param [out] r The crosspoins.
* @param [out] pos_my The pos values of the corresponding crosspoints on me.
* @param [out] pos_other The pos values of the corresponding crosspoints on the other edge.
* @returns The number of crosspoins found [0..4].
*/
unsigned Edge::Crossing(const Edge &A, XY r[], double pos_my[], double pos_other[]) const
{
    int num = CrossingBezier(A, r, pos_my, pos_other, 1, 1, 0, 0);
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
    return num;
}


/* Returns the number of crosspoints*/
unsigned Edge::CrossingVerticalBezier(double x, double y[], double pos[], bool forward[],
    double pos_mul, double pos_offset) const
{
    if (straight) {
        if ((start.x >= x && end.x < x) ||      //we cross leftward
            (start.x < x && end.x >= x)) {      //we cross rightward
            //we cross p's line y
            pos[0] = (x - start.x)/(end.x - start.x);
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
* @param [in] x The x coordinate of the vertical line.
* @param [out] y The y coordinates of the crosspoints.
* @param [out] pos The pos values for the respective crosspoints [0..1]
* @param [out] forward True for a crosspoint if the line at x is crossed from left to right
*              (so inside of contour is below y).
* @returns The number of crosspoints, or -1 if edhe is a vertical line exacty on x.
*/
int Edge::CrossingVertical(double x, double y[], double pos[], bool forward[]) const
{
    if (straight && start.x == x && end.x == x) return -1; //vertical line
    return CrossingVerticalBezier(x, y, pos, forward, 1, 0);
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
        return RayAngle(angle(end, XY(end.x+100, start.y), c2), -invRadius);
    XY A, B;
    const XY C = Split(pos, A, B);
    return incoming ? RayAngle(angle(C, XY(C.x+100, C.y), A), -invRadius) :
                      RayAngle(angle(C, XY(C.x+100, C.y), B), invRadius);
}

inline double COORD(const XY &p, unsigned i) { return i ? p.y : p.x; }

Block Edge::CreateBoundingBox() const
{
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
    if (test_zero(fDiscr)) {    //3 real roots, but at least two are equal
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
   
    //A==start, B==c1, C==c2, D==end
    //Curve is: (-A+3B-3C+D)t^3 + (3A-6B+3C)t^2 + (-3A+3B)t + A
    //M-to-curve vector is: curve-M
    //substitutes:     P * t^3  +      Q *  t^2 +     R * t + S
    //dervivative is: (-3A+9B-9C+3D)t^2 + (6A-12B+6C)t + (-3A+3B)
    //substitutes            3*P  * t^2 +    2*Q   * t +    R
    //(curve-M).derivative is: x*x + y*y
    // (P.3P)t^5 + (P.2Q + Q.3P)t^4 + (P.R + Q.2Q + R.3P)t^3 + 
    // (Q.R + R.2Q + S.3P)t^2 + (R.R + S.2Q)t + S.R
    //so we need to derive it and search for roots
    //its derivative is:
    // 15(P.P)t^4 + 20(P.Q)t^3 + 6(2P.R + Q.Q)t^2 + 
    // 6(Q.R + S.P)t^2 + (R.R + S.2Q)

    const XY P = -start+3*(c1-c2)+end;
    const XY Q = (3*(start+c2)-6*c1);
    const XY R = 3*(c1-start);
    const XY S = start-M;

    double coeff[5] = {
        R.DotProduct(R) + 2*S.DotProduct(Q),
        6*(Q.DotProduct(R) + S.DotProduct(P)),
        6*(2*P.DotProduct(R) + Q.DotProduct(Q)),
        20*P.DotProduct(Q),
        15*P.DotProduct(P)
    };

    double result[4];
    const unsigned num = solve_degree4(coeff, result+2);
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

//returns the centroid of the area above multipled by the (signed) area above
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

std::vector<Edge> Edge::CreateExpand(double gap) const
{
    //XXX FIXME
    Edge E;
    if (straight) {
        const double length = start.Distance(end);
        const XY wh = (end-start).Rotate90CCW()/length*gap;
        return std::vector<Edge>({Edge(start+wh, end+wh)});
    }
    Edge E1, E2;
    Split(E1, E2);
    std::vector<Edge> ret = E1.CreateExpand(gap);
    std::vector<Edge> r = E1.CreateExpand(gap);
    ret.insert(ret.end(), r.begin(), r.end());
    return ret;
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
    for (unsigned bound_index = 1; bound_index<bounds.size(); bound_index++) {
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


}; //namespace