/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011 Zoltan Turanyi
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
#include <vector>
#include <algorithm>
#include "contour_edge.h"


//////////////////Helper functions

//an fmod that always return value strictly in [0..mod)
inline double pos_fmod(double orig, double mod)
{
    _ASSERT(mod>0);
    double ret = orig>=0 ? fmod(orig, mod) : mod - fmod(-orig, mod);
    if (ret == mod) return 0;
    _ASSERT(ret>=0 && ret<mod);
    return ret;
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
        if (test_equal(cx, c.x)) return IN_LINE;
        return ((c.x < cx) ^ (a.y < b.y)) ? COUNTERCLOCKWISE : CLOCKWISE;
    } else {
        const double m = (a.y - b.y)/(a.x - b.x);
        double cy =  m*(c.x-a.x) + a.y; //(c.x, cy) is a point on the a-b line
        if (test_equal(cy, c.y)) return IN_LINE;
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
    cos = std::min(cos, 1.);
    cos = std::max(cos, -1.);
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
//crossing points that result in a pos value close to 1 are ignored
//In case AB is the same (or lies on the same line) as MN, the two endpoint of the common
//sections are returned (if they overlap or touch)
int Edge::crossing_straight_straight(const XY A, const XY B, const XY M, const XY N,
		                                 XY *r, double *pos_in_ab, double *pos_in_mn)
{
    if (!test_zero((B-A).PerpProduct(N-M))) {
        //They are not parallel (and none of them are degenerate, but that does not matter now)
        double t = (B-A).PerpProduct(A-M) / (B-A).PerpProduct(N-M);
        if (!between01_adjust(t)) return 0;
        double s = (N-M).PerpProduct(A-M) / (B-A).PerpProduct(N-M);
        if (!between01_adjust(s)) return 0; //the intersection of the line is outside AB
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
        if (!between01_adjust(s)) return 0;  //if outside [0..1] or too close to 1
        r[0] = M; //M is on AB
        pos_in_mn[0] = 0;
        pos_in_ab[0] = s;
        return 1;
    }
    if (!test_zero((A-M).PerpProduct(N-M)))
        return 0; //A is not on MN's line
    //They are either parallel and on the same line or AB is degenerate and lies on MN's line
    double t[2];
    //select x or y depending on precision
    if (fabs(M.x-N.x) > fabs(M.y-N.y)) {
        t[0] = (A.x-M.x)/(N.x-M.x);
        t[1] = (B.x-M.x)/(N.x-M.x);
    } else {
        t[0] = (A.y-M.y)/(N.y-M.y);
        t[1] = (B.y-M.y)/(N.y-M.y);
    }
    if (t[0] > t[1]) std::swap(t[0], t[1]);
	if (!test_smaller(t[0], 1) || test_smaller(t[1], 0))
        return 0; //AB lies outside MN
	if (!test_smaller(0,t[0])) t[0] = 0; //if close to 0 or even smaller snap it there
	const bool dosecond = test_smaller(0,t[1]) && test_smaller(t[1],1); //if t[1]!~=0 and t[1]!~=1
	int num = 0;
	for (int i = 0; i<(dosecond?2:1); i++) {
		r[num] = M + (N-M)*t[i];
		pos_in_mn[num] = t[i];
		if (fabs(A.x-B.x) > fabs(A.y-B.y))
			pos_in_ab[num] = (r[num].x-A.x)/(B.x-A.x);
		else
			pos_in_ab[num] = (r[num].y-A.y)/(B.y-A.y);
		if (between01_adjust(pos_in_ab[num])) num++;
	}
	return num;
}


//helper for curvy or tilted
bool Edge::radianbetween(double r) const
{
    _ASSERT(!straight);
    if (full_circle) return true;
    r = pos_fmod(r, 2*M_PI);
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
    if (full_circle)
        return pos_fmod(clockwise_arc ? s+r*2*M_PI : s-(r*2*M_PI), 2*M_PI);
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
    r = pos_fmod(r, 2*M_PI);
    if (full_circle)
        return pos_fmod(clockwise_arc ? (r-s)/(2*M_PI) : (s-r)/(2*M_PI), 1);
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

Edge::Edge(const XY &c, double radius_x, double radius_y, double tilt_deg,double s_deg, double d_deg) :
    ell(c, radius_x, radius_y, tilt_deg), s(deg2rad(s_deg)), e(deg2rad(d_deg)), clockwise_arc(true)
{
    straight = false;
    start = ell.Radian2Point(s);
    if (d_deg==360) {
        e = 0;
        full_circle = (s==0);
    } else
        full_circle = false;
}


void Edge::Rotate(double cos, double sin, double radian, const XY&B)
{
    start.Rotate(cos, sin);
    if (!straight) {
        const double turn = ell.Rotate(cos, sin, radian);
        if (turn) {
            s += turn; if (s>2*M_PI) s-= 2*M_PI;
            e += turn; if (e>2*M_PI) e-= 2*M_PI;
        }
    }
    CalculateBoundingBox(B);
}

void Edge::RotateAround(const XY&c, double cos, double sin, double radian, const XY&B)
{
    start.RotateAround(c, cos, sin);
    if (!straight) {
        const double turn = ell.RotateAround(c, cos, sin, radian);
        if (turn) {
            s += turn; if (s>2*M_PI) s-= 2*M_PI;
            e += turn; if (e>2*M_PI) e-= 2*M_PI;
        }
    }
    CalculateBoundingBox(B);
}


void Edge::SwapXY()
{
    start.SwapXY();
    if (IsStraight()) return;
    ell.SwapXY();
    s = s==0 ? s : 2*M_PI - s;
    e = e==0 ? e : 2*M_PI - e;
    clockwise_arc = !clockwise_arc;
    boundingBox.SwapXY();
}

void Edge::CopyInverseToMe(const Edge &B, const XY &next)
{
    if (!B.straight) {
        ell = B.ell;
        s = B.e;
        e = B.s;
        full_circle = B.full_circle;
        clockwise_arc = !B.clockwise_arc;
    }
    start = next;
    straight = B.straight;
	boundingBox = B.boundingBox;
}

double Edge::GetRadianMidPoint() const
{
    _ASSERT(!straight);
    if (full_circle) return fmod(s+M_PI, 2*M_PI);
    if (e==s) return s;
    if (clockwise_arc == (s<e)) return (s+e)/2;
    return fmod((s+e)/2+M_PI, 2*M_PI);
}

//This must ensure that we do not return pos values that are very close to 0 or 1
//such values are to be rounded to 0 or 1 and in that case exatly the endpoint of the arc
//shall be returned
unsigned Edge::Crossing(const Edge &A, const XY &B, const Edge &M, const XY &N,
	                          XY *r, double *pos_ab, double *pos_mn)
{
    //First check bounding boxes
    if (!A.boundingBox.Overlaps(M.boundingBox, SMALL_NUM))
        return 0;
    if (A.straight && M.straight)
		//never returns pos only slightly off from 0 (rounded to 0) and never returns pos close to 1 (skipped)
        return crossing_straight_straight(A.start, B, M.start, N, r, pos_ab, pos_mn);
    int num;
    XY loc_r[8];
    double loc_ab[8], loc_mn[8];
    if (!A.straight && !M.straight) {
        num = A.ell.CrossingEllipse(M.ell, loc_r, loc_ab, loc_mn);
        if (num == -1) { //two equal ellipses, determine joint section (if any)
            if (A.radianbetween(M.s)) {
                loc_ab[0] = M.s;
                if (A.radianbetween(M.e))
                    loc_ab[1] = M.e;
                else
                    if (M.radianbetween(A.s))
                        loc_ab[1] = A.s;
                    else
                        if (M.radianbetween(A.e))
                            loc_ab[1] = A.e;
                        else
                            num = 1;
            } else if (A.radianbetween(M.e)) {
                loc_ab[0] = M.e;
                if (M.radianbetween(A.s))
                    loc_ab[1] = A.s;
                else
                    if (M.radianbetween(A.e))
                        loc_ab[1] = A.e;
                    else
                        num = 1;
            } else if (M.radianbetween(A.s)) {
                loc_ab[0] = A.s;
                if (M.radianbetween(A.e))
                    loc_ab[1] = A.e;
                else
                        num = 1;
            } else if (M.radianbetween(A.e)) {
                loc_ab[0] = A.e;
                num = 1;
            } else
                return 0;
            if (num == -1)
                num = (loc_ab[0] == loc_ab[1]) ? 1 : 2;
            //Now we have the radian(s) in loc_ab. Convert to xy and pos & return
            for (int i=0; i<num; i++) {
                r[i] = A.ell.Radian2Point(loc_ab[i]);
                pos_ab[i] = A.radian2pos(loc_ab[i]);
                pos_mn[i] = M.radian2pos(loc_ab[i]);
                if (!test_smaller(pos_ab[i],1) || !test_smaller(pos_mn[i], 1)) {
                    if (i==num-1) return num-1;
                    loc_ab[0] = loc_ab[1]; //can only be if num==2, i==1
                    i=-1; num=1;
                }
            }
            return num;
        }
    } else if (A.straight)
        num = M.ell.CrossingStraight(A.start, B, loc_r, loc_mn, loc_ab);
    else /* (M.straight)*/
        num = A.ell.CrossingStraight(M.start, N, loc_r, loc_ab, loc_mn);
    int ret = 0;
    for (num--; num>=0; num--) {
		//for curves, convert to position from radian
        if (!A.straight) loc_ab[num] = A.radian2pos(loc_ab[num]);
        if (!M.straight) loc_mn[num] = M.radian2pos(loc_mn[num]);
		//now loc_XY is a position [0..1] indicate on edge
		//test if loc_ab is proper (snap to zero if close, drop if much below zero or close or above 1
		if (test_smaller(loc_ab[num], 0)) //much smaller than 0
			continue;
		if (!test_smaller(0, loc_ab[num])) { //if close to 0 snap it there
			loc_ab[num] = 0;
			loc_r[num] = A.start;
		} else if (!test_smaller(loc_ab[num], 1))  //if close or above to 1 skip
			continue;
		//test if loc_mn
		if (test_smaller(loc_mn[num], 0)) //much smaller than 0
			continue;
		if (!test_smaller(0, loc_mn[num])) { //if close to 0 snap it there
			loc_mn[num] = 0;
			loc_r[num] = M.start;
		} else if (!test_smaller(loc_mn[num], 1))  //if close or above to 1 skip
			continue;
		//Both OK: add to points we return
        r[ret] = loc_r[num];
        pos_ab[ret] = loc_ab[num];
        pos_mn[ret] = loc_mn[num];
        ret++;
    }
    return ret;
}

//test if the end of an arc equals to a point or not
//x coordinate must match exactly, x can be approximate
inline bool test_arc_end(const XY &a, double x, double y)
{
    return a.x==x && test_equal(a.y, y);
}


//Where does an edge or arc corss a vertical line? (for the purposes of Contour::IsWithin)
//1. a leftward edge includes its starting endpoint, and excludes its final endpoint;
//2. a rightward edge excludes its starting endpoint, and includes its final endpoint;
//in short: we include the endpoint with the smaller x coordinate
//returns 0 if no crossing, 1/2 if there are crosspoints and -1 if it is a vertical line.
//in forward we return true if the line at x is crossed from left to right (so inside of contour
//is below y).
unsigned Edge::CrossingVertical(double x, const XY &B, double y[], double pos[], bool forward[]) const
{
    if (straight) {
        if ((start.x >= x && B.x < x) ||      //we cross leftward
            (start.x < x && B.x >= x)) {      //we cross rightward
            //we cross p's line y
			pos[0] = (x - start.x)/(B.x - start.x);
            y[0] = (B.y - start.y)*pos[0]+ start.y;
            forward[0] = start.x < x; //we cross rightward
            return 1;
        }
        if (start.x == x && B.x == x) return -1; //vertical line
        return 0;
    }
	if (test_smaller(x, boundingBox.x.from) || test_smaller(boundingBox.x.till, x))
		return 0;
    double radian[2], loc_y[2];
    int loc_num = ell.CrossingVertical(x, loc_y, radian);
    //if the crosspoints are at the end of the segments, we check for left and rightward crossing.
    if (loc_num==1) { //just touch
		//we return a cp only if the cp is one of the endpoints, in which case the rules below apply
        //if the centerpoint is left of x and a1==s it is the beginning of a leftward edge  => include
        //if the centerpoint is left of x and a1==e it is the end of a rightward edge       => include
        //if the centerpoint is left of x and a1==s it is the beginning of a rightward edge => exclude
        //if the centerpoint is left of x and a1==e it is the end of a leftward edge        => exclude
		if (ell.GetCenter().x >= x)
			return 0;
        if (full_circle)
            return 0;
        if (test_arc_end(start, x, loc_y[0])) {
			y[0] = start.y;
			pos[0] = 0;
            forward[0] = false;
			return 1;
		}
		if (test_arc_end(B, x, loc_y[0])) {
			y[0] = B.y;
			pos[0] = 1;
            forward[0] = true;
			return 1;
		}
		return 0;
	}
    int num = 0;
    for (int i=0; i<loc_num; i++) {
        //if r == s (start) and forward tangent is left of x, it is the beginning of a leftward edge => include
        //if r == e (B)     and backwardtangent is left of x, it is the end of a rightward edge      => include
        //else exclude
        //This is a numerically sensitive place especially close to the endpoints.
        //With straight lines we use simple < > == operators to see if the endpoint is on x or not
        //Here we have numerical imprecision.
        //So first we see if we are close to the endpoints in an approximate manner
        //and then if yes, then we further refine the decision on endpoint using > < operators
        //and then we see if this is a leftward or rightward edge
        if (full_circle)
            ; //If full circle, any crosspoint is valid
        else if (test_equal(radian[i], s)) {                       //OK we treat this as a crosspoint
            if (test_arc_end(start, x, loc_y[i]) &&           //but if *exactly* on the startpoint...
                ell.Tangent(radian[i], clockwise_arc).x > x)  //...and fw tangent to the left: exclude
                continue;
			y[num] = start.y;
			pos[num] = 0;
            forward[num++] = false;
			continue;
        } else if (test_equal(radian[i], e)) {                //OK we treat this as a crosspoint
            if (test_arc_end(B, x, loc_y[i]) &&               //but if *exactly* on the endpoint...
                ell.Tangent(radian[i], !clockwise_arc).x > x)//...and bw tangent to the left: exclude
                continue;
			y[num] = B.y;
			pos[num] = 1;
            forward[num++] = true;
			continue;
        } else if (!radianbetween(radian[i]))  //exclude also, if not close to end and not between s&e
            continue;
        y[num] = loc_y[i]; //include
		pos[num] = radian2pos(radian[i]);
        forward[num++] = loc_y[i] == std::min(loc_y[0], loc_y[1]);
    }
    return num;
}

//Calculates the bounding box _without adding the endpoint_, !!!
const Block& Edge::CalculateBoundingBox(const XY &next)
{
    boundingBox.x.from = boundingBox.x.till = start.x;
    boundingBox.y.from = boundingBox.y.till = start.y;
    boundingBox += next;
    if (straight)
        return boundingBox;
    XY xy;
    //if any extreme point is included in the arc add them
    for (int i=0; i<4; i++)
        if (radianbetween(ell.GetExtreme(i, xy))) boundingBox += xy;
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
    if (ell != next.ell) return false;
    //same center, same radiuses, same tilt
    //if same dir, take the "e" of next
    if (clockwise_arc == next.clockwise_arc) {
        //full circle, if next.e == s and we cover the whole circumference
        full_circle = next.e==s && next.radianbetween(e);
        e = next.e;
    } else {
        //opposite dir. Full circle only if either of us is already full & we remain so
        full_circle = (full_circle || next.full_circle) && s==next.e;
        //direction changes if new endpoint outside our old span
        if (!radianbetween(next.e))
            clockwise_arc = !clockwise_arc;
        e = next.e;
    }
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
    double new_e = !full_circle ? e : clockwise_arc ? e+2*M_PI : e-2*M_PI;
    if (reverse) {
        if (clockwise_arc)
            cairo_arc_negative(cr, 0, 0, 1, new_e, s);
        else
            cairo_arc(cr, 0, 0, 1, new_e, s);
    } else {
        if (clockwise_arc)
            cairo_arc(cr, 0, 0, 1, s, new_e);
        else
            cairo_arc_negative(cr, 0, 0, 1, s, new_e);
    }
    cairo_restore(cr);
}

//return false if curvy edge disappears
//Destroys bounding box!!!
bool Edge::ExpandEdge(double gap, const XY&next, Edge &r1, XY &r2) const
{
	if (!straight) {
		r1.ell = ell;
		if (r1.ell.Expand(clockwise_arc ? gap : -gap)) {
            r1.straight = false;
            r1.s = s;
            r1.e = e;
            r1.full_circle = full_circle;
            r1.clockwise_arc = clockwise_arc;
            r1.start = r1.ell.Radian2Point(s);
            r2 = r1.ell.Radian2Point(e);
            return true;
        }
        //if ellipse becomes too small, fall through to below and replace it with a straight line
	}
    const double length = (next-start).length();
    const XY wh = (next-start).Rotate90CCW()/length*gap;
    r1.start = start+wh;
    r2 = next+wh;
    r1.straight = true;
    return true;
}

//distance of two radians between -pi and +pi
//true that diff+b == a(+-2pi)
double rad_dist(double a, double b)
{
    _ASSERT(a>=0 && a<=2*M_PI && b>=0 && b<=2*M_PI);
    if (a-b <= -M_PI) return a-b + 2*M_PI;
    if (a-b >   M_PI) return a-b - 2*M_PI;
    return a-b;
}

//p is the original radian of the crosspoint, r are the radians of the cps of the expanded edges
//We find among the r values the one that is closes (in circle distance) to p
//In case of a tie, we pick the larger "larger" is ture
int find_closest(int num, const double r[], double p, bool larger)
{
    if (num==0) return -1;
	int pos = 0;
	double diff= fabs(rad_dist(r[0], p));
	for (int i=1; i<num; i++) {
        const double dist = rad_dist(r[i], p);
        if (test_smaller(diff, fabs(dist))) continue;
        if (!test_smaller(fabs(dist), diff))
            if (larger == (rad_dist(r[pos], p) >= diff)) continue;
        diff = fabs(dist);
        pos = i;
	}
	return pos;
}

//this->B and M->N are two expanded edges
//Calculate the crosspoint and place it into res.
//Any possible second point is placed adfter, if needed.
//cal also return 0 if this or M is an ellipse and they do not touch the other (which can be line, too)
//can also return -1 if this->B is parallel with M->N
//can also return -2 if this is the same curve as M
//Returned edge has no valid boundingBox
//if *this is curvy, the crosspoints radian for *this is stored in res_prev.e
int Edge::CombineExpandedEdges(const XY&B, const Edge&M, const XY&N, Edge &res, Edge &res_prev) const
{
    if (M.straight) {
        if (straight) {
            if (0==crossing_line_line(start, B, M.start, N, res.start))
                return -1;
            res.straight = true;
            return 1;
        }
        XY r[8];
        double radian_us[8], pos_M[8];
        int num = ell.CrossingStraight(M.start, N, r, radian_us, pos_M);
        int pos = find_closest(num, radian_us, e, !clockwise_arc);
        if (pos == -1) return 0;
        res.start = r[pos];
        res.straight = true;
        res_prev.e = radian_us[pos];
        return 1;
    } else {
        XY r[8];
        double radian_us[8], radian_M[8];
        int num;
        if (straight)
            num = M.ell.CrossingStraight(start, B, r, radian_M, radian_us);
        else {
            if (ell == M.ell) return -2; //we combine two segments of the same ellipse
            num = ell.CrossingEllipse(M.ell, r, radian_us, radian_M);
        }
        int pos = find_closest(num, radian_M, M.s, M.clockwise_arc);
        if (pos == -1) return 0;
        res.start = r[pos];
        res.straight = false;
        res.ell = M.ell;
        res.s = radian_M[pos];
        //do not modify res.e, it may have been set
        //we are full circle if res.e == res.s and the original edge (M) is more than half
        res.full_circle = test_equal(res.s, res.e) && M.GetSpan()>M_PI;
        res.clockwise_arc = M.clockwise_arc;
        if (!straight)
            res_prev.e = radian_us[pos];
        return 1;
    }
}


inline bool radbw(double r, double s, double e)
{
	if (s<e) return s<=r && r<=e;
    else return r>=s || r<=e;
}

//M is the same edge as *this (straight & ell) with different start and endpoints
//(B is the endpoint for *this, N is for M)
//(so if *this is straight this->B is parallel to M->N)
//return 0 if start->B lies in the same direction on as M.start->N
//return 1, if they lie in the opposite dir or if start~~B
//return 2, if *this is a circle, the direction has changed,
//   but the M curve span more than 180 degrees
int Edge::IsOppositeDir(const XY &B, const Edge &M, const XY &N) const
{
	if (test_equal(start.x, B.x) && test_equal(start.y, B.y))
		return 1;
	if (straight) {
		if (fabs(start.x-B.x) > fabs(start.y-B.y))
			return (start.x<B.x) != (M.start.x<N.x);
		else
			return (start.y<B.y) != (M.start.y<N.y);
	}
    //exception: if any is exactly 180 degrees, we return same direction
    if (fabs(s-e) == M_PI || fabs(M.s-M.e) == M_PI) return 0;
	const bool dir_us = (e>s && (e-s)<M_PI) || (s>e && (s-e)>=M_PI);
	const bool dir_M = (M.e>M.s && (M.e-M.s)<M_PI) || (M.s>M.e && (M.s-M.e)>=M_PI);
    if (dir_us == dir_M) return 0;
	//OK, dir has changed on a curyv edge
	//Now see, if the original s->e was more than 180 degrees
	return M.GetSpan()>M_PI ? 2 : 1;
}


//Returns how much MN can be shifted up to bump into AB
double Edge::offsetbelow_straight_straight(const XY &A, const XY &B, const XY &M, const XY &N, double &touchpoint)
{
    const double minAB = std::min(A.x, B.x);
    const double maxAB = std::max(A.x, B.x);
    const double minMN = std::min(M.x, N.x);
    const double maxMN = std::max(M.x, N.x);
    if (minAB > maxMN || minMN > maxAB) return CONTOUR_INFINITY;
    const double x1 = std::max(minAB, minMN);
    const double x2 = std::min(maxAB, maxMN);
    if (A.x == B.x) {
        touchpoint = std::max(A.y, B.y);
        if (M.x == N.x) return std::min(M.y, N.y) - std::max(A.y, B.y); //here all x coordinates must be the same
        return (N.y-M.y)/(N.x-M.x)*(A.x-M.x) + M.y - std::max(A.y, B.y);
    }
    if (M.x == N.x) {
        touchpoint = std::max(M.y, N.y);
        return std::min(M.y,N.y) - ((A.y-B.y)/(A.x-B.x)*(M.x-A.x) + A.y);
    }
    const double y1 = ((A.y-B.y)/(A.x-B.x)*(x1-A.x) + A.y);
    const double y2 = ((A.y-B.y)/(A.x-B.x)*(x2-A.x) + A.y);
    const double diff1 = (N.y-M.y)/(N.x-M.x)*(x1-M.x) + M.y - y1;
    const double diff2 = (N.y-M.y)/(N.x-M.x)*(x2-M.x) + M.y - y2;
    if (diff1<diff2) {
        touchpoint = y1;
        return diff1;
    }
    touchpoint = y2;
    return diff2;
}

#define CURVY_OFFSET_BELOW_GRANULARIRY 5

//"this" must be a curvy edge
double Edge::offsetbelow_curvy_straight(const XY &A, const XY &B, bool straight_is_up, double &touchpoint) const
{
    _ASSERT(!IsStraight());
    double rad = CURVY_OFFSET_BELOW_GRANULARIRY*2/(ell.radius1 + ell.radius2);
    double end, beg;
    if (full_circle) {
        beg = 0;
        end = 2*M_PI;
    } else if (clockwise_arc) {
        beg = s;
        end = s<e ? e : e+2*M_PI;
    } else {
        beg = e;
        end = s>e ? s : s+2*M_PI;
    }
    XY prev = ell.Radian2Point(beg);
    double ret = CONTOUR_INFINITY;
    for (double r = beg+rad; r<end; r+=rad) {
        const XY xy = ell.Radian2Point(r);
        double tp, off;
        if (straight_is_up)
            off = offsetbelow_straight_straight(A, B, prev, xy, tp);
        else
            off = offsetbelow_straight_straight(prev, xy, A, B, tp);
        if (off < ret) {
            ret = off;
            touchpoint = tp;
        }
        prev = xy;
    }
    return ret;
}

//both must be curvy edges
//"this" is higher than o
double Edge::offsetbelow_curvy_curvy(const Edge &o, double &touchpoint) const
{
    _ASSERT(!IsStraight() && !o.IsStraight());
    const double rad1 = CURVY_OFFSET_BELOW_GRANULARIRY*2/(ell.radius1 + ell.radius2);
    const double rad2 = CURVY_OFFSET_BELOW_GRANULARIRY*2/(o.ell.radius1 + o.ell.radius2);
    //TODO: Fix this for full_circle==true, too!!
    const double end1 = s<e ? e : e+2*M_PI;
    const double end2 = o.s<o.e ? o.e : o.e+2*M_PI;
    double ret = CONTOUR_INFINITY;
    XY prev1 = start;
    for (double r1 = s+rad1; r1<end1; r1+=rad1) {
        const XY xy1 = ell.Radian2Point(r1);
        XY prev2 = o.start;
        for (double r2 = o.s+rad2; r2<end2; r2+=rad2) {
            const XY xy2 = o.ell.Radian2Point(r2);
            double tp, off;
            off = offsetbelow_straight_straight(prev1, xy1, prev2, xy2, tp);
            if (off < ret) {
                ret = off;
                touchpoint = tp;
            }
            prev2 = xy2;
        }
        prev1 = xy1;
    }
    return ret;
}
