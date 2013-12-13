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

namespace contour2 {

XY Edge::Split(double t) const
{
    return pow(1-t, 3) * start
            + 3 * pow(1-t, 2) * t * c1
            + 3 * (1-t) * pow(t, 2) * c2
            + pow(t, 3) * end;
    //(-A+3B-3C+D)t^3 + (3A-6B+3C)t^2 + (-3A+3B)t + A
}


inline XY Mid(const XY &A, const XY &B, double t)
{
    return A+t*(B-A);
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
* The first will be start, ret, c1, r1,
* The second will be ret, end, r2, c2 */
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

/** Describes how three points can relate to each other.
*/
enum ETriangleDirType
{
    ALL_EQUAL,       ///<All three are identical.
    A_EQUAL_B,       ///<Two of them are identical.
    A_EQUAL_C,       ///<Two of them are identical.
    B_EQUAL_C,       ///<Two of them are identical.
    IN_LINE,         ///<Three separate points on a line.
    CLOCKWISE,       ///<`A`->`B`->`C` define a non-degenerate clockwise triangle.
    COUNTERCLOCKWISE ///<`A`->`B`->`C` define a non-degenerate counterclockwise triangle.
};

/** Returns the relation of the three points `a`, `b`, `c`.
* @ingroup contour_internal
*/
ETriangleDirType triangle_dir(XY a, XY b, XY c)
{
    if (a == b) return b==c ? ALL_EQUAL : A_EQUAL_B;
    if (a == c) return A_EQUAL_C;
    if (b == c) return B_EQUAL_C;
    //Decide if we divide by x or y coordinates
    if (fabs(a.x - b.x) < fabs(a.y - b.y)) {
        const double m = (a.x - b.x)/(a.y - b.y);
        double cx = m*(c.y-a.y) + a.x; //(cx, c.y) is a point on the a-b line
        if (test_equal(cx, c.x)) return IN_LINE;
        return ((c.x < cx) ^ (a.y < b.y)) ? COUNTERCLOCKWISE : CLOCKWISE;
    } else {
        const double m = (a.y - b.y)/(a.x - b.x);
        double cy = m*(c.x-a.x) + a.y; //(c.x, cy) is a point on the a-b line
        if (test_equal(cy, c.y)) return IN_LINE;
        return ((c.y < cy) ^ (a.x < b.x)) ? CLOCKWISE : COUNTERCLOCKWISE;
    }
}

/** Returns the *fake angle* of the `base`->`A` and the `base`->`B` segments.
* @ingroup contour_internal
*
* In order to save computation we do not use the angles in radian
* merely its sine, since we only do comparison with these values, no summation
* or other arithmetics. We call this *fake angle*.
* Values between [0..1] thus correspond to radians [0..PI/2], values
* between [1..2] to radians [PI/2..PI], etc.
*
* We return an angle that follows clockwise and can be larger than 180 degrees.
* So if `B` is just a bit cunterclockwise from `A`, we get a value close to 360
* degree (that is the fake value 4).
* @param base The base of the angle.
* @param A one end
* @param B the other end.
* @returns The fake angle from 'A' to 'B'. -1 on error (degenerate cases)
*/
double compute_angle(XY base, XY A, XY B)
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
    double cos = (A-base).DotProduct(B-base) / base.Distance(A) / base.Distance(B);
    cos = std::min(cos, 1.);
    cos = std::max(cos, -1.);
    if (clockwise)
        return 1-cos; //gives [0..2]
    else
        return cos+3; //gives (2..4)
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
        return RayAngle(incoming ? compute_angle(end, XY(end.x+100, end.y), start) : 
                                   compute_angle(start, XY(start.x+100, start.y), end));

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
        return RayAngle(compute_angle(start, XY(start.x+100, start.y), c1), invRadius);
    if (pos==1) //must be incoming
        return RayAngle(compute_angle(end, XY(end.x+100, start.y), c2), -invRadius);
    XY A, B;
    const XY C = Split(pos, A, B);
    return incoming ? RayAngle(compute_angle(C, XY(C.x+100, C.y), A), -invRadius) :
                      RayAngle(compute_angle(C, XY(C.x+100, C.y), B), invRadius);
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

double Edge::Distance(const XY &M, XY &point, double &pos) const //always nonnegative
{
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


DistanceType Edge::Distance(const Edge &O) const    //always nonnegative
{
    return DistanceType();
}


}; //namespace