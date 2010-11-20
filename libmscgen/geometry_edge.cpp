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
#include <vector>
#include <algorithm>
#include "geometry_edge.h"


//////////////////Helper functions

namespace geometry {

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
}

void Edge::CopyInverseToMe(const Edge &B, const XY &next)
{
    if (!B.straight) {
        ell = B.ell;
        s = B.e;
        e = B.s;
        clockwise_arc = !B.clockwise_arc;
    }
    start = next;
    straight = B.straight;
	boundingBox = B.boundingBox;
}

//This must ensure that we do not return pos values that are very close to 0 or 1
//such values are to be rounded to 0 or 1 and in that case exatly the endpoint of the arc
//shall be returned
int Edge::Crossing(const Edge &A, const XY &B, const Edge &M, const XY &N,
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
    if (!A.straight && !M.straight) 
        num = A.ell.CrossingEllipse(M.ell, loc_r, loc_ab, loc_mn);
    else if (A.straight) 
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
int Edge::CrossingHorizontal(double y, const XY &B, double x[], double pos[]) const
{
    if (straight) {
        if ((start.y >= y && B.y < y) ||      //we cross upward
            (start.y < y && B.y >= y)) {      //we cross downward
            //we cross p's line x
			pos[0] = (y - start.y)/(B.y - start.y);
            x[0] = (B.x - start.x)*pos[0]+ start.x;
            return 1;
        }
        if (start.y == y && B.y == y) return -1; //horizontal line
        return 0;
    }
	if (test_smaller(y, boundingBox.y.from) || test_smaller(boundingBox.y.till, y))
		return 0;
    double radian[2], loc_x[2];
    int loc_num = ell.CrossingHorizontal(y, loc_x, radian);
    //if the crosspoints are at the end of the segments, we check for up or downward crossing.
    if (loc_num==1) { //just touch
		//we return a cp only if the cp is one of the endpoints, in which case the rules below apply
        //if the centerpoint is above y and a1==s it is the beginning of an upward edge  => include
        //if the centerpoint is above y and a1==e it is the end of a downward edge       => include
        //if the centerpoint is below y and a1==s it is the beginning of a downward edge => exclude
        //if the centerpoint is below y and a1==e it is the end of an upward edge        => exclude
		if (ell.GetCenter().y >= y)
			return 0;
        if (test_arc_end(start,y,loc_x[0])) {
			x[0] = start.x;
			pos[0] = 0;
			return 1;
		}
		if (test_arc_end(B,y,loc_x[0])) {
			x[0] = B.x;
			pos[0] = 1;
			return 1;
		}
		return 0;
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
			x[num] = start.x;
			pos[num++] = 0;
			continue;
        } else if (test_equal(radian[i], e)) {                //OK we treat this as a crosspoint
            if (test_arc_end(B,y,loc_x[i]) &&                 //but if *exactly* on the endpoint...
                ell.Tangent(radian[i], !clockwise_arc).y > y)//...and bw tangent below: exclude
                continue;
			x[num] = B.x;
			pos[num++] = 1;
			continue;
        } else if (!radianbetween(radian[i]))  //exclude also, if not close to end and not between s&e
            continue;
        x[num] = loc_x[i]; //include
		pos[num++] = radian2pos(radian[i]);
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

//return false if curvy edge disappears
//Destroys bounding box!!!
bool Edge::ExpandEdge(double gap, const XY&next, Edge &r1, XY &r2) const
{
	r1.straight=straight;
	if (straight) {
		const double length = (next-start).length();
		const XY wh = (next-start).Rotate90CCW()/length*gap;
		r1.start = start+wh; 
		r2 = next+wh;
	} else {
		r1.ell = ell;
		if (!r1.ell.Exppand(gap))
			return false;
		r1.s = s;
		r1.e = e;
		r1.clockwise_arc = clockwise_arc;
		r1.start = r1.ell.Radian2Point(s);
		r2 = r1.ell.Radian2Point(e);
	}
	return true;
}

int find_closest(int num, const double r[], double p) 
{
	int pos = -1;
	double diff=200;
	for (int i=0; i<num; i++)
		if (fabs(r[i]-p) < diff) {
			diff = fabs(r[i]-p);
			pos = i;
		}
	return pos;
}

//this->B and M->N are two expanded edges
//Calculate the crosspoint and place it into res.
//Any possible second point is placed adfter, if needed.
//can also return 0 if this->B is parallel with M->N
//cal also return -1 if this or M is an ellipse and they do not touch the other (which can be line, too)
int Edge::CombineExpandedEdges(const XY&B, const Edge&M, const XY&N, std::vector<Edge> &res)  
{
	res.push_back(M);
	if (M.straight) {
		if (straight) {
			if (1==crossing_line_line(start, B, M.start, N, res.rbegin()->start))
				return 1;
			res.pop_back();
			return 0;
		}
		XY r[8];
		double radian_us[8], pos_M[8];
		int num = ell.CrossingStraight(M.start, N, r, radian_us, pos_M);
		int pos = find_closest(num, radian_us, e);
		if (pos = -1) {
			res.pop_back();
			return -1;
		}
		res.rbegin()->start = r[pos];
		return 1;
	} else {
		XY r[8];
		double radian_us[8], radian_M[8];
		int num;
		if (straight) 
			num = M.ell.CrossingStraight(start, B, r, radian_M, radian_us);
		else
			num = ell.CrossingEllipse(M.ell, r, radian_us, radian_M);
		int pos = find_closest(num, radian_M, M.s);
		if (pos = -1) {
			res.pop_back();
			return -1;
		}
		res.rbegin()->start = r[pos];
		res.rbegin()->s = radian_M[pos];
		if (!straight)
			e = radian_us[pos];
		return 1;
	}
}


}; //namespace
