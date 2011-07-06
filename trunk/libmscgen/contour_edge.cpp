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

///////////////////////////////////////////////////////////////////////////////////

void EdgeStraight::Rotate(double cos, double sin, double /*radian*/)
{
    start.Rotate(cos, sin);
    end.Rotate(cos, sin);
    CalculateBoundingBox();
}

void EdgeStraight::RotateAround(const XY&c, double cos, double sin, double /*radian*/)
{
    start.RotateAround(c, cos, sin);
    end.RotateAround(c, cos, sin);
    CalculateBoundingBox();
}

void EdgeStraight::SwapXY()
{
    start.SwapXY();
    end.SwapXY();
    boundingBox.SwapXY();
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
unsigned EdgeStraight::Crossing(const EdgeStraight &o, XY r[],
                                double pos_my[], double pos_other[]) const
{
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
        if (!between01_adjust(s)) return 0;  //if outside [0..1] or too close to 1
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
	if (!test_smaller(t[0], 1) || test_smaller(t[1], 0))
        return 0; //AB lies outside MN
	if (!test_smaller(0,t[0])) t[0] = 0; //if close to 0 or even smaller snap it there
	const bool dosecond = test_smaller(0,t[1]) && test_smaller(t[1],1); //if t[1]!~=0 and t[1]!~=1
	unsigned num = 0;
	for (int i = 0; i<(dosecond?2:1); i++) {
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

//returns -1 if the edge is a straight line exactly on x
int EdgeStraight::CrossingVertical(double x, double y[],
                                        double pos[], bool forward[]) const
{
    if ((start.x >= x && end.x < x) ||      //we cross leftward
        (start.x < x && end.x >= x)) {      //we cross rightward
        //we cross p's line y
		pos[0] = (x - start.x)/(end.x - start.x);
        y[0] = (end.y - start.y)*pos[0]+ start.y;
        forward[0] = start.x < x; //we cross rightward
        return 1;
    }
    if (start.x == x && end.x == x) return -1; //vertical line
    return 0;

}

bool EdgeStraight::CheckAndCombine(const EdgeStraight &next)
{
    const double a = angle(start, end, next.end);
    if (test_zero(a) || test_equal(a,2)) {
        end = next.end;
        return true;
    }
    return false;
}

//the cairo cursor is assumed to be at the "start" (or "end" if "reverse");
void EdgeStraight::Path(cairo_t *cr, bool reverse) const
{
    if (reverse)
        cairo_line_to(cr, start.x, start.y);
    else
        cairo_line_to(cr, end.x, end.y);
}



//Returns how much MN can be shifted up to bump into AB
double EdgeStraight::OffsetBelow(const EdgeStraight &M, double &touchpoint) const
{
    const double minAB = std::min(start.x, end.x);
    const double maxAB = std::max(start.x, end.x);
    const double minMN = std::min(M.GetStart().x, M.GetEnd().x);
    const double maxMN = std::max(M.GetStart().x, M.GetEnd().x);
    if (minAB > maxMN || minMN > maxAB) return CONTOUR_INFINITY;
    const double x1 = std::max(minAB, minMN);
    const double x2 = std::min(maxAB, maxMN);
    if (start.x == end.x) {
        touchpoint = std::max(start.y, end.y);
        if (M.GetStart().x == M.GetEnd().x)
            return std::min(M.GetStart().y, M.GetEnd().y) - std::max(start.y, end.y); //here all x coordinates must be the same
        return (M.GetEnd().y-M.GetStart().y)/(M.GetEnd().x-M.GetStart().x)*(start.x-M.GetStart().x) + M.GetStart().y -
               std::max(start.y, end.y);
    }
    if (M.GetStart().x == M.GetEnd().x) {
        touchpoint = std::max(M.GetStart().y, M.GetEnd().y);
        return std::min(M.GetStart().y,M.GetEnd().y) - ((start.y-end.y)/(start.x-end.x)*(M.GetStart().x-start.x) + start.y);
    }
    const double y1 = ((start.y-end.y)/(start.x-end.x)*(x1-start.x) + start.y);
    const double y2 = ((start.y-end.y)/(start.x-end.x)*(x2-start.x) + start.y);
    const double diff1 = (M.GetEnd().y-M.GetStart().y)/(M.GetEnd().x-M.GetStart().x)*(x1-M.GetStart().x) + M.GetStart().y - y1;
    const double diff2 = (M.GetEnd().y-M.GetStart().y)/(M.GetEnd().x-M.GetStart().x)*(x2-M.GetStart().x) + M.GetStart().y - y2;
    if (diff1<diff2) {
        touchpoint = y1;
        return diff1;
    }
    touchpoint = y2;
    return diff2;
}

///////////////////////////////////////////////////////////////////////////////////

EdgeFullCircle::EdgeFullCircle(const XY &c, double radius_x,
                               double radius_y, double tilt_deg, double s_deg) :
    ell(c, radius_x, radius_y, tilt_deg), s(deg2rad(s_deg)), clockwise_arc(true)
{
    type = EDGE_FULL_CIRCLE;
    end = start = ell.Radian2Point(s);
    CalculateBoundingBox();
}

void EdgeFullCircle::Rotate(double cos, double sin, double radian)
{
    EdgeStraight::Rotate(cos, sin, radian);
    if (type!=EDGE_STRAIGHT) {
        const double turn = ell.Rotate(cos, sin, radian);
        if (turn) {s += turn; if (s>2*M_PI) s-= 2*M_PI;}
    }
}

void EdgeFullCircle::RotateAround(const XY&c, double cos, double sin, double radian)
{
    EdgeStraight::RotateAround(c, cos, sin, radian);
    if (type!=EDGE_STRAIGHT) {
        const double turn = ell.RotateAround(c, cos, sin, radian);
        if (turn) {s += turn; if (s>2*M_PI) s-= 2*M_PI;}
    }
}

bool EdgeFullCircle::operator ==(const EdgeFullCircle& o) const
{
    if (!EdgeStraight::operator==(o)) return false;
    if (type != EDGE_STRAIGHT) {
        if (s!=o.s) return false;
        if (ell!=o.ell) return false;
        if (clockwise_arc != o.clockwise_arc) return false;
    }
    return true;
}

bool EdgeFullCircle::operator < (const EdgeFullCircle& o) const
{
    if (start!=o.start) return start<o.start;
    if (end!=o.end) return end<o.end;
    if (type!=o.type) return type==EDGE_STRAIGHT;
    if (type==EDGE_STRAIGHT) return false; //they are equal
    if (s!=o.s) return s<o.s;
    if (clockwise_arc!=o.clockwise_arc) return clockwise_arc;
    return ell < o.ell;
}

//Gives the intersecting points of me and another full circle or straight edge
//returns the number of crosspoints, or -1 if two identical circle
unsigned EdgeFullCircle::Crossing(const EdgeFullCircle &o, XY r[], double pos_my[], double pos_other[]) const
{
    //First check bounding boxes
    if (!boundingBox.Overlaps(o.boundingBox, SMALL_NUM))
        return 0;
    if (type==EDGE_STRAIGHT) {
        if (o.type==EDGE_STRAIGHT) return EdgeStraight::Crossing(o, r, pos_my, pos_other);
        return o.Crossing(*this, r, pos_other, pos_my);
    }
    int num;
    XY loc_r[8];
    double loc_my[8], loc_other[8];
    if (o.type==EDGE_STRAIGHT) {
        num = ell.CrossingStraight(o.start, o.end, loc_r, loc_my, loc_other);
        int ret = 0;
        for (num--; num>=0; num--) {
		    //for curves, convert to position from radian
            loc_my[num] = radian2pos(loc_my[num]);
		    //now loc_XY is a position [0..1] indicate on edge
		    //test if loc_ab is proper (snap to zero if close, drop if much below zero or close or above 1
		    if (test_smaller(loc_my[num], 0)) //much smaller than 0
			    continue;
		    if (!test_smaller(0, loc_my[num])) { //if close to 0 snap it there
			    loc_my[num] = 0;
			    loc_r[num] = start;
		    } else if (!test_smaller(loc_my[num], 1))  //if close or above to 1 skip
			    continue;
		    //test if loc_other
		    if (test_smaller(loc_other[num], 0)) //much smaller than 0
			    continue;
		    if (!test_smaller(0, loc_other[num])) { //if close to 0 snap it there
			    loc_other[num] = 0;
			    loc_r[num] = o.start;
		    } else if (!test_smaller(loc_other[num], 1))  //if close or above to 1 skip
			    continue;
		    //Both OK: add to points we return
            r[ret] = loc_r[num];
            pos_my[ret] = loc_my[num];
            pos_other[ret] = loc_other[num];
            ret++;
        }
        return ret;
    }
    //Two full cicrles
    num = ell.CrossingEllipse(o.ell, loc_r, loc_my, loc_other);
    if (num == -1) { //two equal ellipses, s may be different
        r[0] = start;
        pos_my[0] = 0;
        pos_other[0] = o.radian2pos(s); //az en kezdopontom az o rendszereben
        if (s==o.s) return 1;
        r[1] = o.start;
        pos_my[1] = radian2pos(o.s);
        pos_other[1] = 0;
        return 2;
    }
    //Now we have the radian(s) in loc_my and loc_other. Convert to xy and pos & return
    for (int i=0; i<num; i++) {
        r[i] = ell.Radian2Point(loc_my[i]);
        pos_my[i] = radian2pos(loc_my[i]);
        pos_other[i] = radian2pos(loc_other[i]);
        if (!test_smaller(pos_my[i],1) || !test_smaller(pos_other[i], 1)) {
            //this crosspoint is of pos==1, we skip it
            if (i==num-1) return num-1;
            //if we skip, we copy next crosspoint to index of 0
            loc_my[0] = loc_my[1]; //can only be if num==2, i==1
            loc_other[0] = loc_other[1]; //can only be if num==2, i==1
            i=-1; num=1;
        }
    }
    return num;
}

//Tells at what x pos this edge crosses the horizontal line at y, rets the number of crosses
int EdgeFullCircle::CrossingVertical(double x, double y[], double pos[], bool forward[]) const
{
	if (test_smaller(x, boundingBox.x.from) || test_smaller(boundingBox.x.till, x))
		return 0;
    int num = ell.CrossingVertical(x, y, pos);
    for (int i=0; i<num; i++) {
		pos[i] = radian2pos(pos[i]);
        forward[i] = y[i] == std::min(y[0], y[1]);
    }
    return num;
}

//calculates bb
const Block& EdgeFullCircle::CalculateBoundingBox()
{
    //For full circles we add all extremes
    if (type==EDGE_FULL_CIRCLE) {
        boundingBox.MakeInvalid();
        //include the extreme points
        for (int i=0; i<4; i++) {
            XY xy;
            ell.GetExtreme(i, xy);
            boundingBox += xy;
        }
        return boundingBox;
    }
    //for straight edges (or arcs) we just add start & stop
    return EdgeStraight::CalculateBoundingBox();
}

//assumes cairo position is at start (or at end if reverse is true)
void EdgeFullCircle::Path(cairo_t *cr, bool reverse) const
{
    cairo_save(cr);
    ell.TransformForDrawing(cr);
    if (reverse) {
        if (clockwise_arc)
            cairo_arc_negative(cr, 0, 0, 1, s+2*M_PI, s);
        else
            cairo_arc(cr, 0, 0, 1, s+2*M_PI, s);
    } else {
        if (clockwise_arc)
            cairo_arc(cr, 0, 0, 1, s, s+2*M_PI);
        else
            cairo_arc_negative(cr, 0, 0, 1, s, s+2*M_PI);
    }
    cairo_restore(cr);
}


///////////////////////////////////////////////////////////////////////////////////


//helper for curvy or tilted
bool EdgeArc::radianbetween(double r) const
{
    _ASSERT(type!=EDGE_STRAIGHT);
    if (type==EDGE_FULL_CIRCLE) return true;
    r = fmod_negative_safe(r, 2*M_PI);
    if (clockwise_arc) {
        if (s<e) return s<=r && r<=e;
        else return r>=s || r<=e;
    } else {
        if (s<e) return s>=r || r>=e;
        else return s>=r && r>=e;
    }
}

//convert from pos [0..1] to actual radian. Guaranteed between [0..2pi)
double EdgeArc::pos2radian(double r) const
{
    _ASSERT(type!=EDGE_STRAIGHT);
    _ASSERT(0<=r && r<=1);
    if (type==EDGE_FULL_CIRCLE)
        return EdgeFullCircle::pos2radian(r);
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
double EdgeArc::radian2pos(double r) const
{
    _ASSERT(type!=EDGE_STRAIGHT);
    r = fmod_negative_safe(r, 2*M_PI);
    if (type==EDGE_FULL_CIRCLE)
        return EdgeFullCircle::radian2pos(r);
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

EdgeArc::EdgeArc(const XY &c, double radius_x, double radius_y, double tilt_deg,double s_deg, double d_deg) :
    EdgeFullCircle(c, radius_x, radius_y, tilt_deg, s_deg), e(deg2rad(d_deg))
{
    if (d_deg==360) {
        e = 0;
        type = (s==0) ? EDGE_FULL_CIRCLE : EDGE_ARC;
    } else
        type = EDGE_ARC;
    start = ell.Radian2Point(e);
    CalculateBoundingBox();
}

void EdgeArc::Rotate(double cos, double sin, double radian)
{
    EdgeStraight::Rotate(cos, sin, radian);
    if (type!=EDGE_STRAIGHT) {
        const double turn = ell.Rotate(cos, sin, radian);
        if (turn) {
            s += turn; if (s>2*M_PI) s-= 2*M_PI;
            e += turn; if (e>2*M_PI) e-= 2*M_PI;
        }
    }
}

void EdgeArc::RotateAround(const XY&c, double cos, double sin, double radian)
{
    EdgeStraight::RotateAround(c, cos, sin, radian);
    if (type!=EDGE_STRAIGHT) {
        const double turn = ell.RotateAround(c, cos, sin, radian);
        if (turn) {
            s += turn; if (s>2*M_PI) s-= 2*M_PI;
            e += turn; if (e>2*M_PI) e-= 2*M_PI;
        }
    }
}

//In angle1 return the angle (in false angles in [0..4]) of a tangent to this edge at pos
//depending on incoming or outgoing.
//For curvy edges angle2 will be set according to which direction the curve goes
//and how wide the arc is.
//These two values will be used to order rays in a crosspoint
//p and pos must agree
RayAngle EdgeArc::Angle(bool incoming, const XY &p, double pos) const
{
    _ASSERT(!incoming || (pos!=0));
    _ASSERT( incoming || (pos!=1));
    if (type==EDGE_STRAIGHT) return EdgeStraight::Angle(incoming);
    const XY xy = ell.Tangent(pos2radian(pos), incoming ^ clockwise_arc);
    RayAngle ret;
    ret.angle = angle(p, XY(p.x+100, p.y), xy);
    if (test_equal(ret.angle, 4)) ret.angle = 0;
    switch (triangle_dir(p, xy, ell.GetCenter())) {
        case CLOCKWISE:
            ret.curve = +1; break;
        case COUNTERCLOCKWISE:
            ret.curve = -1; break;
        default:
            _ASSERT(0);
    }
    ret.curve /= (ell.GetRadius1()+ell.GetRadius2())/2; //TODO it right: use actual radius for ellipses
    return ret;
};

//the radian difference between two radians
inline double radian_diff_abs(double a, double b) 
{
    _ASSERT(0<=a && a<2*M_PI);
    _ASSERT(0<=b && b<2*M_PI);
    return std::min(fabs(a-b), a>b ? 2*M_PI-(a-b) : 2*M_PI-(b-a));
}


//pos lists a series of pos values. We find the one that is closest to
//"rad", which is in radians. We return the radian.
double EdgeArc::FindRadianOfClosestPos(unsigned num, double pos[], double rad)
{
    _ASSERT(type!=EDGE_STRAIGHT);
    double ret=-1, diff=10;
    for (unsigned i=0; i<num; i++) {
        const double radian = pos2radian(pos[i]);
        const double d = radian_diff_abs(rad, radian);
        if (d < diff) {
            diff = d;
            ret = radian;
        }
    }
    return ret;
}

//flip clockwise if new_s and new_e would be an arc of opposite dir
//ret true if we did so
bool EdgeArc::UpdateClockWise(double new_s, double new_e)
{
    //check which is the smallest diff: s<->new_s plus e<->new_e or the other
    if (radian_diff_abs(s, new_s) + radian_diff_abs(e, new_e) <=
        radian_diff_abs(e, new_s) + radian_diff_abs(s, new_e))
        return false;
    clockwise_arc = !clockwise_arc;
    return true;
}




bool EdgeArc::operator ==(const EdgeArc& o) const
{
    if (!EdgeFullCircle::operator==(o)) return false;
    if (type != EDGE_ARC) return true;
    return e==o.e;
}

bool EdgeArc::operator < (const EdgeArc& o) const
{
    if (start!=o.start) return start<o.start;
    if (end!=o.end) return end<o.end;
    if (type!=o.type) return type==EDGE_STRAIGHT;
    if (type==EDGE_STRAIGHT) return false; //they are equal
    if (s!=o.s) return s<o.s;
    if (type==EDGE_ARC) {
        if (e!=o.s) return e<o.s;
    }
    if (clockwise_arc!=o.clockwise_arc) return clockwise_arc;
    return ell < o.ell;
}

double EdgeArc::GetRadianMidPoint() const
{
    _ASSERT(type!=EDGE_STRAIGHT);
    if (type==EDGE_FULL_CIRCLE) return M_PI;
    if (e==s) return s;
    if (clockwise_arc == (s<e)) return (s+e)/2;
    return fmod_negative_safe((s+e)/2+M_PI, 2*M_PI);
}

//This must ensure that we do not return pos values that are very close to 0 or 1
//such values are to be rounded to 0 or 1 and in that case exatly the endpoint of the arc
//shall be returned
unsigned EdgeArc::Crossing(const EdgeArc &o, XY r[], double pos_my[], double pos_other[]) const
{
    //First check bounding boxes
    if (!boundingBox.Overlaps(o.boundingBox, SMALL_NUM))
        return 0;
    if (type!=EDGE_ARC) {
        if (o.type!=EDGE_ARC)
            return EdgeFullCircle::Crossing(o, r, pos_my, pos_other);
        return o.Crossing(*this, r, pos_other, pos_my);
    }
    if (o.type==EDGE_STRAIGHT) {
        XY loc_r[8];
        double loc_my[8], loc_other[8];
        int num = ell.CrossingStraight(o.start, o.end, loc_r, loc_my, loc_other);
        int ret = 0;
        for (num--; num>=0; num--) {
            if (!radianbetween(loc_my[num])) continue;
		    //for curves, convert to position from radian
            loc_my[num] = radian2pos(loc_my[num]);
		    //now loc_XY is a position [0..1] indicate on edge
		    //test if loc_ab is proper (snap to zero if close, drop if much below zero or close or above 1
		    if (test_smaller(loc_my[num], 0)) //much smaller than 0
			    continue;
		    if (!test_smaller(0, loc_my[num])) { //if close to 0 snap it there
			    loc_my[num] = 0;
			    loc_r[num] = start;
		    } else if (!test_smaller(loc_my[num], 1))  //if close or above to 1 skip
			    continue;
		    //test if loc_other
		    if (test_smaller(loc_other[num], 0)) //much smaller than 0
			    continue;
		    if (!test_smaller(0, loc_other[num])) { //if close to 0 snap it there
			    loc_other[num] = 0;
			    loc_r[num] = o.start;
		    } else if (!test_smaller(loc_other[num], 1))  //if close or above to 1 skip
			    continue;
            //OK add as a crosspoint
            r[ret] = loc_r[num];
            pos_my[ret] = loc_my[num];
            pos_other[ret] = loc_other[num];
            ret ++;
        }
        return ret;
    }
    //here "this" is an arc and "o" is either an arc or a full circle
    XY loc_r[8];
    double loc_my[8], loc_other[8];
    int num = ell.CrossingEllipse(o.ell, loc_r, loc_my, loc_other);
    if (num == -1) { //two equal ellipses, determine joint section (if any)
        if (o.type!=EDGE_ARC) { //if other is full circle, the two poins are "s" and "e"
            r[0] = start;
            pos_my[0] = 0;
            pos_other[0] = o.radian2pos(s);
            //we do not report "e", as its pos==1
            return 1;
        }
        //here two arcs. Find intersection of them
        if (radianbetween(o.s)) {
            loc_my[0] = o.s;
            if (radianbetween(o.e))
                loc_my[1] = o.e;
            else
                if (o.radianbetween(s))
                    loc_my[1] = s;
                else
                    if (o.radianbetween(e))
                        loc_my[1] = e;
                    else
                        num = 1;
        } else if (radianbetween(o.e)) {
            loc_my[0] = o.e;
            if (o.radianbetween(s))
                loc_my[1] = s;
            else
                if (o.radianbetween(e))
                    loc_my[1] = e;
                else
                    num = 1;
        } else if (o.radianbetween(s)) {
            loc_my[0] = s;
            if (o.radianbetween(e))
                loc_my[1] = e;
            else
                num = 1;
        } else if (o.radianbetween(e)) {
            loc_my[0] = e;
            num = 1;
        } else
            return 0;
        if (num == -1) //we have not changed num above
            num = (loc_my[0] == loc_my[1]) ? 1 : 2;
    }
    //Now we have the radian(s) in loc_my (but nothing in loc_other).
    //Convert to xy and pos (for both arcs) & return
    for (int i=0; i<num; i++) {
        r[i] = ell.Radian2Point(loc_my[i]);
        pos_my[i] = radian2pos(loc_my[i]);
        pos_other[i] = o.radian2pos(loc_my[i]);
        if (!test_smaller(pos_my[i],1) || !test_smaller(pos_other[i], 1)) {
            if (i==num-1) return num-1;
            loc_my[0] = loc_my[1]; //can only be if num==2, i==1
            i=-1; num=1;
        }
    }
    return num;
}

//test if the end of an arc equals to a point or not
//x coordinate must match exactly, x can be approximate
inline bool test_arc_end(const XY &a, double x, double y)
{
    return a.x==x && test_equal(a.y, y);
}


//Where does an edge or arc corss a vertical line? (for the purposes of SimpleContour::IsWithin)
//1. a leftward edge includes its starting endpoint, and excludes its final endpoint;
//2. a rightward edge excludes its starting endpoint, and includes its final endpoint;
//in short: we include the endpoint with the smaller x coordinate
//returns 0 if no crossing, 1/2 if there are crosspoints and -1 if it is a vertical line.
//in forward we return true if the line at x is crossed from left to right (so inside of contour
//is below y).
int EdgeArc::CrossingVertical(double x, double y[], double pos[], bool forward[]) const
{
    if (type == EDGE_STRAIGHT) return EdgeStraight::CrossingVertical(x, y, pos, forward);
    int num = EdgeFullCircle::CrossingVertical(x, y, pos, forward);
    if (num<=0 || type!=EDGE_ARC) return num;
    //if the crosspoints are at the end of the segments, we check for left and rightward crossing.
    if (num==1) { //just touch
		//we return a cp only if the cp is one of the endpoints, in which case the rules below apply
        //if the centerpoint is left of x and a1==s it is the beginning of a leftward edge  => include
        //if the centerpoint is left of x and a1==e it is the end of a rightward edge       => include
        //if the centerpoint is left of x and a1==s it is the beginning of a rightward edge => exclude
        //if the centerpoint is left of x and a1==e it is the end of a leftward edge        => exclude
		if (ell.GetCenter().x >= x)
			return 0;
        if (test_arc_end(start, x, y[0])) {
			y[0] = start.y;
			pos[0] = 0;
            forward[0] = false;
			return 1;
		}
		if (test_arc_end(end, x, y[0])) {
			y[0] = end.y;
			pos[0] = 1;
            forward[0] = true;
			return 1;
		}
        //not close to any endpoint: touching is not valid then
		return 0;
	}
    num = 0;
    for (int i=0; i<2; i++) {
        //recalculate radians. "pos" now contains a position assuming a full circle
        pos[i] = EdgeFullCircle::pos2radian(pos[i]);
        //if r == s (start) and forward tangent is left of x, it is the beginning of a leftward edge => include
        //if r == e (B)     and backwardtangent is left of x, it is the end of a rightward edge      => include
        //else exclude
        //This is a numerically sensitive place especially close to the endpoints.
        //With straight lines we use simple < > == operators to see if the endpoint is on x or not
        //Here we have numerical imprecision.
        //So first we see if we are close to the endpoints in an approximate manner
        //and then if yes, then we further refine the decision on endpoint using > < operators
        //and then we see if this is a leftward or rightward edge
        if (test_equal(pos[i], s)) {                      //OK we treat this as a crosspoint
            if (test_arc_end(start, x, y[i]) &&           //but if *exactly* on the startpoint...
                ell.Tangent(s, clockwise_arc).x > x)      //...and fw tangent to the left: exclude
                continue;
			y[num] = start.y;
			pos[num] = 0;
            forward[num++] = false;
			continue;
        } else if (test_equal(pos[i], e)) {               //OK we treat this as a crosspoint
            if (test_arc_end(end, x, y[i]) &&             //but if *exactly* on the endpoint...
                ell.Tangent(e, !clockwise_arc).x > x)     //...and bw tangent to the left: exclude
                continue;
			y[num] = end.y;
			pos[num] = 1;
            forward[num++] = true;
			continue;
        } else if (!radianbetween(pos[i]))  //exclude also, if not close to end and not between s&e
            continue;
        y[num] = y[i]; //include
		pos[num] = radian2pos(pos[i]);
        forward[num++] = y[i] == std::min(y[0], y[1]);
    }
    return num;
}

//Removes the part of the edge or curve before point p. Assumes p lies on us.
EdgeArc& EdgeArc::SetStartStrict(const XY &p, double pos, bool keep_full_circle)
{
    start = p;
    if (type==EDGE_STRAIGHT) {
        EdgeStraight::CalculateBoundingBox();
        return *this;
    }
    const double r = pos2radian(pos);
    _ASSERT(radianbetween(r));
    if (type==EDGE_FULL_CIRCLE && !(test_equal(s,r) && keep_full_circle)) {
        e = s;
        type = EDGE_ARC;
    }
    s = r;
    CalculateBoundingBox();
    return *this;
}

//Removes the part of the edge or curve before point p. Does not assumes p lies on us,
//this can be used to extend an edge
//We do not check if an arc wraps or not!!!
EdgeArc& EdgeArc::SetStartLiberal(const XY &p, bool keep_full_circle)
{
    start = p;
    if (type==EDGE_STRAIGHT) {
        EdgeStraight::CalculateBoundingBox();
        return *this;
    }
    const double r = ell.Point2Radian(p);
    if (type==EDGE_FULL_CIRCLE && !(test_equal(s,r) && keep_full_circle)) {
        e = s;
        type = EDGE_ARC;
    }
    s = r;
    CalculateBoundingBox();
    return *this;
}



//Calculates the bounding box _without adding the endpoint_, !!!
const Block& EdgeArc::CalculateBoundingBox()
{
    EdgeStraight::CalculateBoundingBox();
    if (type==EDGE_STRAIGHT) return boundingBox;
    XY xy;
    //if any extreme point is included in the arc add them
    for (int i=0; i<4; i++)
        if (radianbetween(ell.GetExtreme(i, xy))) boundingBox += xy;
    return boundingBox;
}

//Checks if this edge and next can be combined into a single edge
//if so, also update this to be the combined edge
bool EdgeArc::CheckAndCombine(const EdgeArc &next)
{
    //if next is a degenerate (zero length) arc, we just skip that
    if (next.type != EDGE_FULL_CIRCLE && next.start.test_equal(next.end))
        return true;
    if ((type==EDGE_STRAIGHT) != (next.type==EDGE_STRAIGHT)) return false;
    if (type==EDGE_STRAIGHT)
        return EdgeStraight::CheckAndCombine(next);
    //OK, now we are both curvy edges
    if (ell != next.ell) return false;
    //OK, we are the same ellipse
    //if any of us is full circle, keep only the other one
    //if both of us is a full circle, we must equal (since next.start==end), keep any
    if (next.type==EDGE_FULL_CIRCLE)
        return true; //we keep what we are
    if (type==EDGE_FULL_CIRCLE) {
        //start and s remain (must be equal to next.start)
        _ASSERT(test_equal(s,next.s) && start.test_equal(next.start));
        clockwise_arc = next.clockwise_arc;
        e = next.e;
        end = next.end;
        type = next.type; //next cannot be full circle
        boundingBox = next.boundingBox;
        return true;
    }
    //Now none of us is a full circle
    if (clockwise_arc == next.clockwise_arc) {
        //full circle, if next.e == s and we cover the whole circumference
        if (test_equal(next.e,s)) type = EDGE_FULL_CIRCLE;
    } else {
        //opposite dir. never full circle
        //direction changes if new endpoint outside our old span
        if (!radianbetween(next.e)) clockwise_arc = !clockwise_arc;
    }
    e = next.e; //we may end up as degenerate (s==e) but that is life
    end = next.end;
    return true;
}

void EdgeArc::Path(cairo_t *cr, bool reverse) const
{
    switch (type) {
    case EDGE_STRAIGHT:
        EdgeStraight::Path(cr, reverse);
        return;
    case EDGE_FULL_CIRCLE:
        EdgeFullCircle::Path(cr, reverse);
        return;
    case EDGE_ARC:
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
}

//return false if curvy edge disappears
//Destroys bounding box!!!
bool EdgeArc::Expand(double gap)
{
	if (type!=EDGE_STRAIGHT) {
        const EllipseData old_ell(ell);
		switch (ell.Expand(clockwise_arc ? gap : -gap)) {
        case 1: //ell remained an ellipse
            start = ell.Radian2Point(s);
            if (type==EDGE_ARC) end = ell.Radian2Point(e);
            return true;
        case -1: //completely disappeared
            return false;
        case 0:  //degenerates to line
            if (type==EDGE_FULL_CIRCLE)
                return false; //start==end here, we do not return such a straight edge
            //if ellipse becomes too small, fall through to below and replace it with a straight line
            //Now decrease just up to the point od getting degenerate
            const double rad = std::min(old_ell.GetRadius1(), old_ell.GetRadius2());
            _ASSERT(rad<fabs(gap));
            ell = old_ell;
            ell.Expand(gap>0 ? rad : -rad);
            start = ell.Radian2Point(s);
            end = ell.Radian2Point(e);
            //and then move remaining
            gap = (gap>0) ? gap-rad : gap+rad;
            type = EDGE_STRAIGHT;
        }
	}
    const double length = (end-start).length();
    const XY wh = (end-start).Rotate90CCW()/length*gap;
    start += wh;
    end += wh;
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

//"this" and "M" are two expanded edges
//"oldcp" is their old intersection point, before expansion
//in "newcp" we return their new crosspoint
//The whole logic of this function assumes EXPAND_MITER, but this is called for all types of
//expansions to see how two edges relate.
//return value is
//SAME_ELLIPSIS: two arc of the same ellipses (no cp returned, radians not checked)
//PARALLEL_LINES: two parallel lines (can happen only if an edge was removed from between, no cp returned)
//CP_REAL: The two edges actually cross (between "start" and "end")
//CP_EXTENDED: The two edges do not meet but their extension does
//In the below cases neither the two edges nor their expanded version meet. We will have to add
//line segments to fix this. There are 4 situations how this can happen.
//Three variants refer to spiky vertices, like the one below
//..|           The dots mean the inside of the contour. "o" marks the vertex.
//...\          Here we got an arc in "this" (counterclockwise)
//....-         And an edge in "M" (from right to left)
//<----o        This is a CP_ADD_LINE_ME situation.
//CP_ADD_LINE_OTHER: "this" is straight and should be extended to "newxp", while "M" is an arc
//     and a straight line should be added from "newcp" to M.start.
//CP_ADD_LINE_ME: "M" is a straight and should be extended to "newxp", while "this" is an arc
//     and a straight line should be added from "this"->end to "newcp" 
//CP_ADD_LINE_BOTH: both "M" and "this" are arcs and two straight lines shall be added (as above)
//If the vertex is spiky to the inside, like the below:
//    ^...           Here we have a straight "this" from left to right.
//    |....          And a clockwise arc in "M".
//     \...          In this case a straight line shall be added from 
//      -...         "this"->end to "M".start.
//--->---o...
//............
//NO_CP_ADD_LINE: represents the situ above. No cp is returned in CP.
EdgeArc::EExpandCPType EdgeArc::FindExpandedEdgesCP(const EdgeArc&M, const XY &oldcp, XY &newcp) const
{
    const double parallel_join_multipiler = 5;
    if (M.type == EDGE_STRAIGHT) {
        if (type == EDGE_STRAIGHT) {
            switch (crossing_line_line(start, end, M.start, M.end, newcp)) {
            default: _ASSERT(0);
            case LINE_CROSSING_PARALLEL: return PARALLEL_LINES;
            case LINE_CROSSING_OUTSIDE: return CP_EXTENDED;
            case LINE_CROSSING_INSIDE: return CP_REAL;
            }
        } else {
            XY r[8];
            double radian_us[8], pos_M[8];
            int num = ell.CrossingStraight(M.start, M.end, r, radian_us, pos_M);
            if (!num) {
                const XY tangent = PrevTangentPoint(1, M);  //We use prev-tangent so as not to take the next edge, second param is dummy
                if (crossing_line_line(tangent, end, M.start, M.end, newcp) == LINE_CROSSING_PARALLEL) {
                    //OK we are parallel
                    const double dist = (end-M.start).length() * parallel_join_multipiler;
                    const double mlen = (M.end-M.start).length();
                    newcp = M.start + (M.start-M.end)/mlen*dist;
                }
                return CP_ADD_LINE_ME;
            } else {
                int pos = find_closest(num, radian_us, e, !clockwise_arc);
                newcp = r[pos];
                _ASSERT(newcp.x>-1000000 && newcp.x<10000000);
                if (radianbetween(radian_us[pos]) && between01_approximate_inclusive(pos_M[pos]))
                    return CP_REAL;
                return CP_EXTENDED;
            }
        }
    } else {
        XY r[8];
        if (type == EDGE_STRAIGHT) {
            double pos_us[8], radian_M[8];
            int num = M.ell.CrossingStraight(start, end, r, radian_M, pos_us);
            if (!num) {
                const XY tangent = M.NextTangentPoint(0, M);  //We use next-tangent so as not to take the next edge, second param is dummy
                if (crossing_line_line(tangent, M.start, start, end, newcp) == LINE_CROSSING_PARALLEL) {
                    //OK we are parallel
                    const double dist = (end-M.start).length() * parallel_join_multipiler;
                    const double len = (end-start).length();
                    newcp = start + (start-end)/len*dist;
                }
                return CP_ADD_LINE_OTHER;
            }
            int pos = find_closest(num, radian_M, M.s, M.clockwise_arc);
            newcp = r[pos];
            _ASSERT(newcp.x>-1000000 && newcp.x<10000000);
            if (M.radianbetween(radian_M[pos]) && between01_approximate_inclusive(pos_us[pos]))
                return CP_REAL;
            return CP_EXTENDED;
        } else {
            if (ell == M.ell) return SAME_ELLIPSIS; //we combine two segments of the same ellipse - all must be OK
            double radian_us[8], radian_M[8];
            int num = ell.CrossingEllipse(M.ell, r, radian_us, radian_M);
            if (!num) {
                const XY tangent1 = PrevTangentPoint(1, M);  
                const XY tangent2 = M.NextTangentPoint(0, M);
                if (crossing_line_line(tangent1, end, tangent2, M.start, newcp) == LINE_CROSSING_PARALLEL) {
                    //OK we are parallel
                    const double dist = (end-M.start).length() * parallel_join_multipiler;
                    const double len = (end-tangent1).length();
                    newcp = (end+M.start)/2 + (end-tangent1)/len*dist;
                }
                return CP_ADD_LINE_BOTH;
            }
            int pos = find_closest(num, radian_M, M.s, M.clockwise_arc);
            newcp = r[pos];
            _ASSERT(newcp.x>-1000000 && newcp.x<10000000);
            if (M.radianbetween(radian_M[pos]) && radianbetween(radian_us[pos]))
                return CP_REAL;
            return CP_EXTENDED;
        }
    }
}


void EdgeArc::SetStartEndForExpand(const XY &S, const XY &E) 
{
    double new_s, new_e;
    if (type!=EDGE_STRAIGHT) {
        new_s = ell.Point2Radian(S);
        if (S.test_equal(E)) 
            new_e = new_s;
        else 
            new_e = ell.Point2Radian(E);
    }
    switch (type) {
    case EDGE_ARC:
        //check if direction has changed
        //start with an exception: if any is exactly 180 degrees, we assume same direction
        //(thus we test the rest only if none is exactly 180 degrees)
        if (fabs(s-e) != M_PI && fabs(new_s-new_e) != M_PI) {
            const bool dir_us = (e>s && (e-s)<M_PI) || (s>e && (s-e)>=M_PI);
            const bool dir_M = (new_e>new_s && (new_e-new_s)<M_PI) || (new_s>new_e && (new_s-new_e)>=M_PI);
            if (dir_us != dir_M) 
                clockwise_arc = !clockwise_arc;
        }
        /* Falltherough */
    case EDGE_FULL_CIRCLE:
        //for full circle we assume dir has not changed
        s = new_s;
        e = new_e;
        type = S.test_equal(E) ? EDGE_FULL_CIRCLE : EDGE_ARC;
    case EDGE_STRAIGHT:
        break;
    }
    start = S;
    end = E;
}

//int EdgeArc::CombineExpandedEdges(EdgeArc&M, EExpandType et, const XY &old, EdgeArc &res)
//{
//    XY cp;
//    if (M.type == EDGE_STRAIGHT) {
//        if (type == EDGE_STRAIGHT) {
//            if (!crossing_line_line(start, end, M.start, M.end, cp))
//                return -1;
//        } else {
//            XY r[8];
//            double radian_us[8], pos_M[8];
//            int num = ell.CrossingStraight(M.start, M.end, r, radian_us, pos_M);
//            int pos = find_closest(num, radian_us, e, !clockwise_arc);
//            if (pos==-1) return -1;
//            cp = r[pos];
//        }
//    } else {
//        XY r[8];
//        double radian_us[8], radian_M[8];
//        int num;
//        if (type == EDGE_STRAIGHT)
//            num = M.ell.CrossingStraight(start, end, r, radian_M, radian_us);
//        else {
//            if (ell == M.ell) return false; //we combine two segments of the same ellipse - all must be OK
//            num = ell.CrossingEllipse(M.ell, r, radian_us, radian_M);
//        }
//        int pos = find_closest(num, radian_M, M.s, M.clockwise_arc);
//        //arc does not intersect with a straight edge: add line
//        if (pos == -1) return -1;
//        cp = r[pos];
//        _ASSERT(cp.x>-1000000 && cp.x<10000000);
//    }
//
//    //We get here if the two edges join "normally"
//    //"cp" contains their crosspoint
//    switch (et) {
//    default:
//        _ASSERT(0);
//    case EXPAND_MITER:
//        SetEndLiberal(cp);
//        M.SetStartLiberal(cp);
//        return false;
//    case EXPAND_BEVEL:
//        res.type = EDGE_STRAIGHT;
//        res.start = end;
//        res.end = M.start;
//        return true;
//    case EXPAND_ROUND:
//        _ASSERT(test_equal((old-end).length(), (old-M.start).length()));
//        res = Edge(old, (old-end).length()); //full circle
//        res.SetStartLiberal(end);
//        res.SetEndLiberal(M.start);
//        return true;
//    }
//}


inline bool radbw(double r, double s, double e)
{
	if (s<e) return s<=r && r<=e;
    else return r>=s || r<=e;
}

//M is the same edge as *this (straight & ell) with different start and endpoints
//(so if *this is straight then "this" is parallel to "M")
//return 0 if start->end lies in the same direction on as M.start->M.end
//return 1, if they lie in the opposite dir or if start~~end
//return 2, if *this is a circle, the direction has changed,
//   but the M curve span more than 180 degrees
//int Edge::IsOppositeDir(const EdgeArc &M) const
//{
//	if (start.test_equal(end)) return 1;
//	if (type==EDGE_STRAIGHT) {
//		if (fabs(start.x-end.x) > fabs(start.y-end.y))
//			return (start.x<end.x) != (M.start.x<M.end.x);
//		else
//			return (start.y<end.y) != (M.start.y<M.end.y);
//	}
//    //exception: if any is exactly 180 degrees, we return same direction
//    if (fabs(s-e) == M_PI || fabs(M.s-M.e) == M_PI) return 0;
//	const bool dir_us = (e>s && (e-s)<M_PI) || (s>e && (s-e)>=M_PI);
//	const bool dir_M = (M.e>M.s && (M.e-M.s)<M_PI) || (M.s>M.e && (M.s-M.e)>=M_PI);
//    if (dir_us == dir_M) return 0;
//	//OK, dir has changed on a curyv edge
//	//Now see, if the original s->e was more than 180 degrees
//	return M.GetSpan()>M_PI ? 2 : 1;
//}
//
#define CURVY_OFFSET_BELOW_GRANULARIRY 5

double EdgeArc::offsetbelow_curvy_straight(const EdgeStraight &M, bool straight_is_up, double &touchpoint) const
{
    _ASSERT(type!=EDGE_STRAIGHT);
    _ASSERT(M.GetType()==EDGE_STRAIGHT);
    double rad = CURVY_OFFSET_BELOW_GRANULARIRY*2/(ell.GetRadius1() + ell.GetRadius2());
    double end, beg;
    if (type==EDGE_FULL_CIRCLE) {
        beg = 0;
        end = 2*M_PI;
    } else if (clockwise_arc) {
        beg = s;
        end = s<e ? e : e+2*M_PI;
    } else {
        beg = e;
        end = s>e ? s : s+2*M_PI;
    }
    EdgeStraight e(ell.Radian2Point(beg), XY(0,0));
    double ret = CONTOUR_INFINITY;
    for (double r = beg+rad; r<end; r+=rad) {
        e.end = ell.Radian2Point(r);
        double tp, off;
        if (straight_is_up)
            off = M.OffsetBelow(e, tp);
        else
            off = e.OffsetBelow(M, tp);
        if (off < ret) {
            ret = off;
            touchpoint = tp;
        }
        e.start = e.end;
    }
    return ret;
}

//both must be curvy edges
//"this" is higher than o
double Edge::OffsetBelow(const EdgeArc &o, double &touchpoint) const
{
    if (o.GetType() == EDGE_STRAIGHT) {
        if (type == EDGE_STRAIGHT)
            return EdgeStraight::OffsetBelow(o, touchpoint);
        else
            return offsetbelow_curvy_straight(o, false, touchpoint);
    }
    if (type == EDGE_STRAIGHT)
        return o.offsetbelow_curvy_straight(*this, true, touchpoint);
    //now both of us are curves
    const double rad1 = CURVY_OFFSET_BELOW_GRANULARIRY*2/(ell.GetRadius1() + ell.GetRadius2());
    const double rad2 = CURVY_OFFSET_BELOW_GRANULARIRY*2/(o.ell.GetRadius1() + o.ell.GetRadius2());
    //TODO: Fix this for full_circle==true, too!!
    const double end1 = s<e ? e : e+2*M_PI;
    const double end2 = o.s<o.e ? o.e : o.e+2*M_PI;
    double ret = CONTOUR_INFINITY;
    EdgeStraight e1(start, XY(0,0));
    for (double r1 = s+rad1; r1<end1; r1+=rad1) {
        e1.end = ell.Radian2Point(r1);
        EdgeStraight e2(o.start, XY(0,0));
        for (double r2 = o.s+rad2; r2<end2; r2+=rad2) {
            e2.end = o.ell.Radian2Point(r2);
            double tp, off;
            off = e1.OffsetBelow(e2, tp);
            if (off < ret) {
                ret = off;
                touchpoint = tp;
            }
            e2.start = e2.end;
        }
        e1.start = e1.end;
    }
    return ret;
}
