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

#include <cassert>
#include <vector>
#include <algorithm>
#include "contour_edge.h"

namespace contour {

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
    double cos = (A-base).DotProduct(B-base) / base.Distance(A) / base.Distance(B);
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

//to generate a full circle instead of an empty one, specify d_deg as s_deg+360
Edge::Edge(const XY &c, double radius_x, double radius_y, double tilt_deg, double s_deg, double d_deg)
    : type(FULL_CIRCLE), s(deg2rad(s_deg)), e(deg2rad(d_deg)), clockwise_arc(true),
    ell(c, radius_x, radius_y, tilt_deg), visible(true)
{
    start = ell.Radian2Point(s);
    if (test_equal(s_deg, d_deg) || !test_equal(s,e)) {
        type = ARC;
        end = ell.Radian2Point(e);
    } else {
        e = s;
        end = start;
    }
    CalculateBoundingBox();
}


double Edge::Distance(const XY &p, XY &point, double &pos) const
{
    if (type==STRAIGHT) {
        if (test_equal(start.x, end.x)) {
            point.x = start.x;
            if (p.y<std::min(start.y, end.y)) {
                point.y = std::min(start.y, end.y);
                pos = end.y<start.y ? 1 : 0;
            } else if (p.y>std::max(start.y, end.y)) {
                point.y = std::max(start.y, end.y);
                pos = end.y>start.y ? 1 : 0;
            } else {
                point.y = p.y;
                pos = (p.y-start.y)/(end.y-start.y);
                return fabs(start.x - p.x);
            }
            return point.Distance(p);
        }
        if (test_equal(start.y, end.y)) {
            point.y = start.y;
            if (p.x<std::min(start.x, end.x)) {
                point.x = std::min(start.x, end.x);
                pos = end.x<start.x ? 1 : 0;
            } else if (p.x>std::max(start.x, end.x)) {
                point.x = std::max(start.x, end.x);
                pos = end.x>start.x ? 1 : 0;
            } else {
                point.x = p.x;
                pos = (p.x-start.x)/(end.x-start.x);
                return fabs(start.y - p.y);
            }
            return point.Distance(p);
        }
        //http://mathworld.wolfram.com/Point-LineDistance2-Dimensional.html
        const double m = fabs((start.x-p.x)*(end.x-start.x)-(end.y-start.y)*(start.y-p.y))/start.DistanceSqr(end);
        if (m<0) {point = start; pos = 0; return start.Distance(p);}
        if (m>1) {point = end; pos = 1; return end.Distance(p);}
        point = start + (end-start)*m;
        pos = m;
        //return fabs((end.x-start.x)*(start.y-p.y)-(start.x-p.x)*(end.y-start.y))/l;
        return point.Distance(p);
    }
    if (type==FULL_CIRCLE) {
        const double d = ell.Distance(p, point, pos);
        pos = radian2pos(pos);
        return d;
    }
    double radian;
    const double dist = ell.Distance(p, point, radian);
    if (radianbetween(radian)) { //on arc
        pos = radian2pos(radian);
        return dist;  
    }
    //outside: find the closest endpoint
    if (std::min(fmod_negative_safe(radian-s, 2*M_PI), fmod_negative_safe(radian-s, 2*M_PI)) <
        std::min(fmod_negative_safe(radian-e, 2*M_PI), fmod_negative_safe(radian-e, 2*M_PI)))
        {point = start; pos = 0; return start.Distance(p);}
    else
        {point = end; pos = 1; return end.Distance(p);}
}

//This one assumes an empty "ret", which gets filled in, with a nonnegative value
DistanceType Edge::Distance(const Edge &o) const
{
    DistanceType ret;
    if (type==STRAIGHT) {
        if (o.type == STRAIGHT) {
            //test if we cross
            if (!test_zero((end-start).PerpProduct(o.end-o.start))) {
                //They are not parallel (and none of them are degenerate, but that does not matter now)
                const double t = (end-start).PerpProduct(start-o.start) / (end-start).PerpProduct(o.end-o.start);
                const double s = (o.end-o.start).PerpProduct(start-o.start) / (end-start).PerpProduct(o.end-o.start);
                if (between01_approximate_inclusive(t) && between01_approximate_inclusive(s)) {
                    ret.Merge(0, start + (end-start)*t, start + (end-start)*t);
                    return ret;
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
            return ret;
        }
        if (o.type == FULL_CIRCLE) {
            XY p[2];
            const double d = o.GetEllipseData().Distance(start, end, p);
            //now p[0] is a point on the ellipse, p[1] is a point on the line "this"
            const double line_pos = fabs(start.x-end.x) < fabs(start.y - end.y) ?
                                    (p[1].y-start.y)/(end.y-start.y) : (p[1].x-start.x)/(end.x-start.x);
            if (d<0) { //we cross
                //the two points are the crosspoints
                //check if the first in inside the section
                if (between01_approximate_inclusive(line_pos)) {
                    ret.Merge(0, p[1], p[1]);
                    return ret;
                }
                //now check if the other is
                const double line_pos2  = fabs(start.x-end.x) < fabs(start.y - end.y) ?
                                           (p[0].y-start.y)/(end.y-start.y) : (p[0].x-start.x)/(end.x-start.x);
                if (between01_approximate_inclusive(line_pos2)) {
                    ret.Merge(0, p[0], p[0]);
                    return ret;
                }
                //none of the crosspoints are between start and end of the section
                //if both are at the same side, return closer one
                if ((line_pos<0) == (line_pos2<0)) {
                    if (line_pos < 0) goto start_is_closer;
                    else goto end_is_closer;
                } else { //we are inside the "o"
                    if (line_pos<0) { //"point_on_line" is closer to "start" than to "end"
                        if (-line_pos < line_pos2-1)  goto start_is_closer;
                        else goto end_is_closer;
                    } else {//"point_on_line" is closer to "end" than to "start"
                        if (line_pos-1 < -line_pos2) goto end_is_closer;
                        else goto start_is_closer;
                    }
                }
            }
            //if we touch & touchpoint between start and end
            if (d==0 && between01_approximate_inclusive(line_pos)) {
                ret.Merge(0, p[0], p[0]);
                return ret;
            }
            //d>0: //line avoids ellipse
            if (!test_smaller(0,line_pos)) {//"start" is closer
start_is_closer:
                XY other;
                double dummy;
                const double d = o.ell.Distance(start, other, dummy);
                ret.Merge(d, start, other);
                return ret;
            }
            if (!test_smaller(line_pos,1)) { //"end" is closer
end_is_closer:
                XY other;
                double dummy;
                const double d = o.ell.Distance(end, other, dummy);
                ret.Merge(d, end, other);
                return ret;
            }
            //"p[1]" contains a point of the line "this" which is closest to the ellipse "o"
            XY other;
            double dummy;
            const double di = o.ell.Distance(p[1], other, dummy);
            ret.Merge(di, p[1], other);
            return ret;
        }
        if (o.type == ARC) {
            //The following points can be the shortest distance
            //the infinite line and the full circle
            //the start and endpoints of either arc to the other
            //(from the latter we skip endpoints as the next arc will do it)
            XY p[2];
            double d = o.GetEllipseData().Distance(start, end, p);
            //now p[0] is a point on the ellipse, p[1] is a point on the line "this"
            //now validate that this is a real distance
            const double line_pos  = fabs(start.x-end.x) < fabs(start.y - end.y) ?
                                           (p[1].y-start.y)/(end.y-start.y) : (p[1].x-start.x)/(end.x-start.x);
            const double rad = o.GetEllipseData().Point2Radian(p[0]);
            if (d>=0) {//touch or apart
                if (!between01_approximate_inclusive(line_pos) || !o.radianbetween(rad))
                    d = MaxVal(d); //other options will be closer
            } else { //cross
                //if any crosspoint is valid we are 0 distance
                const double line_pos2 = fabs(start.x-end.x) < fabs(start.y - end.y) ?
                                            (p[0].y-start.y)/(end.y-start.y) : (p[0].x-start.x)/(end.x-start.x);
                //check line_pos and rad generated from point on ell (="p[0]")
                if (between01_approximate_inclusive(line_pos2) && o.radianbetween(rad)) {
                ret.Merge(0, p[0], p[0]);
                    return ret;
                }
                //check line_pos and rad generated from point on line (="p[1]")
                const double rad2 = o.GetEllipseData().Point2Radian(p[1]);
                if (between01_approximate_inclusive(line_pos) && o.radianbetween(rad2)) {
                ret.Merge(0, p[1], p[1]);
                    return ret;
                }
                d = MaxVal(d); //other options will be closer
            }
            //Here d is nonnegative (can be maxval)
            //Ok, now check if the startpoint of the arc is closer to the line
            XY point1, point2;
            double dummy;
            const double d1 = Distance(o.start, point1, dummy);
            const double d2 = o.Distance(start, point2, dummy);
            if (d < d1 && d < d2) {
                ret.Merge(d, p[1], p[0]);
                return ret;
            }
            if (d1<d2)
                ret.Merge(d1, point1, o.start);
            else
                ret.Merge(d2, start, point2);
            return ret;
        }
        _ASSERT(0); //not arc neither full_circle nor straight
    }
    if (o.type==STRAIGHT) {
        ret = o.Distance(*this); //straight-curvy combinations
        ret.SwapPoints();
        return ret;
    }
    //Now we have two curvy edges
    //We select the one with shorter arc for interpolation
    //TODO: replace to binary search
    const double l1 = (ell.GetRadius1()+ell.GetRadius1())*GetSpan();
    const double l2 = (o.ell.GetRadius1()+o.ell.GetRadius1())*o.GetSpan();
    const Edge &a = l1<l2 ? *this : o;
    const Edge &b = l1<l2 ? o : *this;
    const double step_length = 5;
    const double step_arc = step_length/((a.ell.GetRadius1()+a.ell.GetRadius1()));
    double r1 = a.GetRadianS(), r2 = a.GetRadianE();
    if (a.GetClockWise()) std::swap(r1, r2);
    if (r2<=r1) r2 += 2*M_PI;
    const XY me = a.GetEllipseData().Radian2Point(r2);
    XY other;
    double dummy;
    const double d = b.Distance(me, other, dummy);
    ret.Merge(d, me, other);
    for (; r1<r2; r1+=step_arc) {
        const XY me = a.GetEllipseData().Radian2Point(r1);
        const double d = b.Distance(me, other, dummy);
        ret.Merge(d, me, other);
        if (ret.IsZero()) return ret;
    }
    return ret;
}



void Edge::Rotate(double cos, double sin, double radian)
{
    start.Rotate(cos, sin);
    end.Rotate(cos, sin);
    if (type!=STRAIGHT) {
        const double turn = ell.Rotate(cos, sin, radian);
        if (turn) {
            s += turn; if (s>=2*M_PI) s-= 2*M_PI;
            e += turn; if (e>=2*M_PI) e-= 2*M_PI;
        }
    }
    CalculateBoundingBox();
    _ASSERT(IsSane());
}

void Edge::RotateAround(const XY&c, double cos, double sin, double radian)
{
    start.RotateAround(c, cos, sin);
    end.RotateAround(c, cos, sin);
    if (type!=STRAIGHT) {
        const double turn = ell.RotateAround(c, cos, sin, radian);
        if (turn) {
            s += turn; if (s>2*M_PI) s-= 2*M_PI;
            e += turn; if (e>2*M_PI) e-= 2*M_PI;
        }
    }
    CalculateBoundingBox();
    _ASSERT(IsSane());
}

void Edge::SwapXY()
{
    start.SwapXY();
    end.SwapXY();
    boundingBox.SwapXY();
    if (type==STRAIGHT) return;
    ell.SwapXY();
    clockwise_arc=!clockwise_arc;
    if (ell.IsTilted()) {
        s = s==0 ? 0 : 2*M_PI-s;
        e = e==0 ? 0 : 2*M_PI-e;
    } else {
        s = s<M_PI/2 ? M_PI/2-s : 2.5*M_PI - s;
        e = e<M_PI/2 ? M_PI/2-e : 2.5*M_PI - e;
    }
}


//This must ensure that we do not return pos values that are very close to 0 or 1
//such values are to be rounded to 0 or 1 and in that case exatly the endpoint of the arc
//shall be returned
unsigned Edge::Crossing(const Edge &o, XY r[], double pos_my[], double pos_other[]) const
{
    //First check bounding boxes
    if (!boundingBox.Overlaps(o.boundingBox, SMALL_NUM))
        return 0;
    //call ourselves with swapped "this" and "o" so that "this" is later in this list:
    //straight, full_circle, arc
    if (type<o.type) return o.Crossing(*this, r, pos_other, pos_my);
    if (type==STRAIGHT) {
        //two straight lines
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
    //"this is either a full circle or an arc
    int num;
    XY loc_r[8];
    double loc_my[8], loc_other[8];
    if (type==FULL_CIRCLE) {
        if (o.type==ARC)
            return o.Crossing(*this, r, pos_other, pos_my);
        if (o.type==STRAIGHT) {
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
        unsigned numnum = 0;
        for (int i=0; i<num; i++) {
            r[numnum] = loc_r[i];
            pos_my[numnum] = radian2pos(loc_my[i]);
            pos_other[numnum] = o.radian2pos(loc_other[i]);
            if (!test_smaller(pos_my[i],1) || !test_smaller(pos_other[i], 1))
                continue;
            numnum++;
        }
        return num;
    }
    //"this" is an ARC
    if (o.type==STRAIGHT) {
        num = ell.CrossingStraight(o.start, o.end, loc_r, loc_my, loc_other);
        int ret = 0;
        for (num--; num>=0; num--) {
            //if (!radianbetween(loc_my[num])) continue;
		    //for curves, convert to position from radian
            loc_my[num] = radian2pos(loc_my[num]);
		    //now loc_XY is a position [0..1] indicate on edge
		    //test if loc_ab is proper (snap to zero if close, drop if much below zero or close or above 1
		    if (test_smaller(loc_my[num], 0)) //much smaller than 0
			    continue;
		    if (!test_smaller(0, loc_my[num])) { //if close to 0 snap it there
			    loc_my[num] = 0;
			    loc_r[num] = start;
                loc_other[num] = point2pos_straight(o.start, o.end, start);
		    } else if (!test_smaller(loc_my[num], 1))  //if close or above to 1 skip
			    continue;
		    //test if loc_other
		    if (test_smaller(loc_other[num], 0)) //much smaller than 0
			    continue;
		    if (!test_smaller(0, loc_other[num])) { //if close to 0 snap it there
			    loc_other[num] = 0;
			    loc_r[num] = o.start;
                loc_my[num] = radian2pos(ell.Point2Radian(o.start));
                if (test_zero(loc_my[num])) loc_my[num] = 0;
		    } else if (!test_smaller(loc_other[num], 1))  //if close or above to 1 skip
			    continue;
            //OK add as a crosspoint
            r[ret] = loc_r[num];
            pos_my[ret] = loc_my[num];
            pos_other[ret] = loc_other[num];
            _ASSERT(Pos2Point(pos_my[ret]).test_equal(r[ret]));
            _ASSERT(o.Pos2Point(pos_other[ret]).test_equal(r[ret]));
            ret ++;
        }
        return ret;
    }
    //here "this" is an arc and "o" is either an arc or a full circle
    num = ell.CrossingEllipse(o.ell, loc_r, loc_my, loc_other);
    if (num==0) return 0;
    if (num>0) {
        //Now we have the radian(s) in loc_my & loc_other.
        //Convert to xy and pos (for both arcs) & return
        for (int i=0; i<num; i++) {
            r[i] = loc_r[i];
            pos_my[i] = radian2pos(loc_my[i]);
            pos_other[i] = o.radian2pos(loc_other[i]);
            if (!test_smaller(pos_my[i], 1) || !test_smaller(pos_other[i], 1) ||
                 test_smaller(pos_my[i], 0) ||  test_smaller(pos_other[i], 0)) {
                if (i==num-1) {
                    num--;
                    break;
                }
                loc_my[0] = loc_my[1]; //can only be if num==2, i==1
                loc_other[0] = loc_other[1];
                loc_r[0] = loc_r[1];
                i=-1; num=1;
            } else {
                if (!test_smaller(0, pos_my[i])) pos_my[i] = 0;
                if (!test_smaller(0, pos_other[i])) pos_other[i] = 0; //snap to zero
            }
        }
        for (unsigned u=0; u<unsigned(num); u++) {
            _ASSERT(Pos2Point(pos_my[u]).test_equal(r[u]));
            _ASSERT(o.Pos2Point(pos_other[u]).test_equal(r[u]));
        }
        return num;
    }

    //num==-1 => two equal ellipses, determine joint section (if any)
    if (o.type==FULL_CIRCLE) { //if other is full circle, the two poins are "s" and "e"
        r[0] = start;
        pos_my[0] = 0;
        pos_other[0] = o.radian2pos(s);
        //we do not report "e", as its pos==1
        return 1;
    }
    //here two arcs. Find intersection of them
    if (radianbetween(o.s)) {
        loc_my[0] = o.s;
        loc_r[0] = o.start;
        if (radianbetween(o.e)) {
            loc_my[1] = o.e;
            loc_r[1] = o.end;
        } else
            if (o.radianbetween(s)) {
                loc_my[1] = s;
                loc_r[1] = start;
            } else
                if (o.radianbetween(e)) {
                    loc_my[1] = e;
                    loc_r[1] = end;
                } else
                    num = 1;
    } else if (radianbetween(o.e)) {
        loc_my[0] = o.e;
        loc_r[0] = o.end;
        if (o.radianbetween(s)) {
            loc_my[1] = s;
            loc_r[1] = start;
        } else
            if (o.radianbetween(e)) {
                loc_my[1] = e;
                loc_r[1] = end;
            } else
                num = 1;
    } else if (o.radianbetween(s)) {
        loc_my[0] = s;
        loc_r[0] = start;
        if (o.radianbetween(e)) {
            loc_my[1] = e;
            loc_r[1] = end;
        } else
            num = 1;
    } else if (o.radianbetween(e)) {
        loc_my[0] = e;
        loc_r[0] = end;
        num = 1;
    } else
        return 0;
    if (num == -1) //we have not changed num above
        num = (loc_my[0] == loc_my[1]) ? 1 : 2;
    unsigned res_num = 0;
    for (unsigned i=0; i<unsigned(num); i++) {
        if (loc_my[i]==e || loc_my[i]==o.e) continue; //do not return pos==1 cps
        r[res_num] = loc_r[i];
        pos_my[res_num] = radian2pos(loc_my[i]);
        pos_other[res_num] = o.radian2pos(loc_my[i]); //loc_my contains good radians for "o", too
        _ASSERT(Pos2Point(pos_my[res_num]).test_equal(r[res_num]));
        _ASSERT(o.Pos2Point(pos_other[res_num]).test_equal(r[res_num]));
        res_num++;
    }
    return res_num;
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
//in short: we include the endpoint with the larger x coordinate
//returns 0 if no crossing, 1/2 if there are crosspoints and -1 if it is a vertical line.
//in forward we return true if the line at x is crossed from left to right (so inside of contour
//is below y).
int Edge::CrossingVertical(double x, double y[], double pos[], bool forward[]) const
{
    if (type==STRAIGHT) {
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
    int num = ell.CrossingVertical(x, y, pos);  //pos here is in radian
    if (num<=0) return 0;
    //if the crosspoints are at the end of the segments, we check for left and rightward crossing.
    if (num==1) { //just touch
        if (type==FULL_CIRCLE) return 0;
		//we return a cp only if the cp is one of the endpoints, in which case the rules below apply
        //now, we touch a vertical line with either start or end of an arc
        //if center is left of x, we touch the larger end (both clockwise and cclockwise)
		if (ell.GetCenter().x >= x)
			return 0;
        if (test_arc_end(start, x, y[0])) {
			y[0] = start.y;
			pos[0] = 0;
            forward[0] = true;  //always from left to right with endpouints
			return 1;
		}
		if (test_arc_end(end, x, y[0])) {
			y[0] = end.y;
			pos[0] = 1;
            forward[0] = true;  //always from left to right with endpouints
			return 1;
		}
        //not close to any endpoint: touching is not valid then
		return 0;
	}
    if (type==FULL_CIRCLE) {
        for (int i=0; i<2; i++) {
            pos[i] = radian2pos(pos[i]);
            forward[i] = (y[i] == std::min(y[0], y[1])) == clockwise_arc;
        }
        return num;
    }
    //now we are an arc
    num = 0;
    for (int i=0; i<2; i++) {
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
                ell.Tangent(s, clockwise_arc).x > x)      //...and fw tangent to the right: exclude
                continue;
			y[num] = start.y;
			pos[num] = 0;
        } else if (test_equal(pos[i], e)) {               //OK we treat this as a crosspoint
            if (test_arc_end(end, x, y[i]) &&             //but if *exactly* on the endpoint...
                ell.Tangent(e, !clockwise_arc).x > x)     //...and bw tangent to the right: exclude
                continue;
			y[num] = end.y;
			pos[num] = 1;
        } else if (radianbetween(pos[i])) { //include, if not close to end but between s&e
            y[num] = y[i];
            pos[num] = radian2pos(pos[i]);
        } else
            continue; //exclude
        forward[num] = (y[i] == std::min(y[0], y[1])) == clockwise_arc;
        num++;
    }
    return num;
}



//Path a dashed/dotted line
//We also assume cairo dash is set to continuous: we fake dash here
//The pattern contains lengths of alternating on/off segments, num contains the number of elements in pattern
//pos shows which pattern element we shall start at, offset shows at which point. We return updated versions of these
void Edge::PathDashed(cairo_t *cr, const double pattern[], unsigned num, int &pos, double &offset, bool reverse) const
{
    _ASSERT(num);
    _ASSERT(offset<pattern[pos]);
    if (type == STRAIGHT) {
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

        while(processed+pattern[pos] <= len) {
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
    cairo_save(cr);
    ell.TransformForDrawing(cr);
    double local_s = s, local_e = e;
    bool cl = clockwise_arc;
    if (reverse) {
        std::swap(local_s, local_e);
        cl = !cl;
    }
    if (cl) {
        if (local_e<local_s) local_e += 2*M_PI; //ensure local_e is larger than local_s
    } else {
        if (local_s<local_e) local_s += 2*M_PI; //ensure local_s is larger than local_e (assume local_e and local_s between [0, 2PI])
    }
    //TODO Do proper ellipse arc length calculations
    const double avg_r = (ell.GetRadius1()+ell.GetRadius2())/2;
    const double len = fabs(local_s-local_e)*avg_r; 
    const double inc_s = local_s<local_e ? 1/avg_r : -1/avg_r;
    double processed = 0;
    if (offset) {
        if (pattern[pos]-offset > len) { //remaining segment is shorter than the entire length
            offset += len;
            if (pos%2==0) {//start with drawn
                cairo_new_sub_path(cr);
                if (cl) cairo_arc(cr, 0, 0, 1., local_s, local_e);
                else cairo_arc_negative(cr, 0, 0, 1., local_s, local_e);
                cairo_restore(cr);
            } else
                cairo_restore(cr);
            return;
        }
        const double new_s = local_s + inc_s*(pattern[pos]-offset);
        if (pos%2==0) {
            cairo_new_sub_path(cr);
            if (cl) cairo_arc(cr, 0, 0, 1., local_s, new_s);
            else cairo_arc_negative(cr, 0, 0, 1., local_s, new_s);
        }
        local_s = new_s;
        processed += pattern[pos]-offset;
        pos = (pos+1)%num;
    }

    while (processed+pattern[pos] <= len) {
        const double new_s = local_s + inc_s*pattern[pos];
        if (pos%2==0) {
            cairo_new_sub_path(cr);
            if (cl) cairo_arc(cr, 0, 0, 1., local_s, new_s);
            else cairo_arc_negative(cr, 0, 0, 1., local_s, new_s);
        }
        local_s = new_s;
        processed += pattern[pos];
        pos = (pos+1)%num;
    }

    offset = len - processed;
    if (pos%2==0 && !test_zero(offset)) {
        cairo_new_sub_path(cr);
        if (cl) cairo_arc(cr, 0, 0, 1., local_s, local_e);
        else cairo_arc_negative(cr, 0, 0, 1., local_s, local_e);
    }
    cairo_restore(cr);
}



///////////////////////////////////////////////////////////////////////////////////

bool Edge::equal_curvy(const Edge& o) const
{
    _ASSERT(type != STRAIGHT);
    if (s!=o.s) return false;
    if (ell!=o.ell) return false;
    if (clockwise_arc != o.clockwise_arc) return false;
    if (type==ARC && e!=o.e) return false;
    return true;
}

bool Edge::smaller_curvy(const Edge& o) const
{
    if (s!=o.s) return s<o.s;
    if (type==ARC) {
        if (e!=o.s) return e<o.s;
    }
    if (clockwise_arc!=o.clockwise_arc) return clockwise_arc;
    return ell < o.ell;
}


//calculates bb
void Edge::calcbb_curvy()
{
    //boundingBox.MakeInvalid(); already invalidated in CalculateBoundingBox()
    //For full circles we add all extremes
    XY xy;
    if (type==FULL_CIRCLE) {
        //include the extreme points
        for (int i=0; i<4; i++) {
            ell.GetExtreme(i, xy);
            boundingBox += xy;
        }
    } else {
        //if any extreme point is included in the arc add them
        for (int i=0; i<4; i++)
            if (radianbetween(ell.GetExtreme(i, xy)))
                boundingBox += xy;
    }
}

//assumes cairo position is at start
void Edge::pathto_curvy(cairo_t *cr) const
{
    cairo_save(cr);
    ell.TransformForDrawing(cr);
    if (clockwise_arc)
        cairo_arc(cr, 0, 0, 1, s, type==ARC ? e : s+2*M_PI);
    else
        cairo_arc_negative(cr, 0, 0, 1, s, type==ARC ? e : s-2*M_PI);
    cairo_restore(cr);
}


///////////////////////////////////////////////////////////////////////////////////


//helper for curvy or tilted
bool Edge::radianbetween(double r) const
{
    _ASSERT(type!=STRAIGHT);
    if (type==FULL_CIRCLE) return true;
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
double Edge::pos2radian(double r) const
{
    if (test_zero(r)) return s;
    if (test_equal(r,1)) return e;
    _ASSERT(type!=STRAIGHT);
    _ASSERT(0<=r && r<=1);
    if (type==FULL_CIRCLE) {
        double a = clockwise_arc ? s+r*2*M_PI : s-r*2*M_PI ;
        if (a>=2*M_PI) a -= 2*M_PI;
        if (a<0) a += 2*M_PI;
        _ASSERT(0<=a && a<2*M_PI);
        return a;
    }
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
    _ASSERT(type!=STRAIGHT);
    r = fmod_negative_safe(r, 2*M_PI);
    if (type==FULL_CIRCLE) {
        if (clockwise_arc)
            return s<=r ? (r-s)/(2*M_PI) : (r-s)/(2*M_PI)+1;
        else
            return s>=r ? (s-r)/(2*M_PI) : (s-r)/(2*M_PI)+1;
    }
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

//Here is how area calculation works
// for straight edges
// ==========> =========>    (Since in contour we treat y coordinates as
//  +++++        ------      growing downwards, we call these functions
//  __+++        ------      "xxAreaAbove", meaning the area between edge & X axis.)
//  |\+++        \-----      The dotted area is the area above an edge.
//    \++         \ ---      It is counted as positive in the first example;
//     \+          \---      and negative in the second. If we sum all such
//      \           \--      areas, we get the area of the contour (positive for
//                  _\|      clockwise and negative for counterclockwise, as it
//                           should be.
// Now, the formula for the above for an edge (X1,Y1)->(X2,Y2) is
// AreaAbove = (X1-X2)*(Y1+Y2)/2 = (X1*Y1 - X2*Y2)/2 + (X1*Y2 - X2*Y1)/2
// If we sum this for all edges of a connected contour the first term will
// cancel out. Thus in the edge::xxAreaAboveAdjusted functions we return
// the second term only (twice 2), thus we return
// AreaAbove*2 - (X1*Y1 - X2*Y2).
// In the below we return it for curvy edges.

double Edge::getAreaAboveAdjusted_curvy() const
{
    _ASSERT(type!=STRAIGHT);
    //For a full circle we return the (signed) area enclosed by the circle times 2.
    if (type==FULL_CIRCLE) return ell.FullArea()*(clockwise_arc ? 2 : -2);
    //
    // =======>   ======>   ===========>   The first figure shows a clockwise arc (all
    //      ++       --                    positive areas). The second shows the same,
    //    =>++     ===>      ^=======o     but since we start from a smaller X and end on
    //   /++++    /++oo       \::::://     a larger, the difference compared to a straight
    //   \+++/    \++o         \::://      edge is is cancelled out (indcated by "o" zero)
    //    ===      --           ===        and the area below is negative.
    //                                     The third figure shows the center of the circle.
    // Denote "S" and "D" the start and endpoint of the arc, resp; while "O" the center.
    // To calculate this, we have three areas: the sector area of the arc, the area
    // above the D->O and the O->S straight lines. If we treat all of these signed
    // (if the sector is clockwise have it positive, and vice versa), we get the
    // (signed) area eaxctly above the arc. Thus
    // AreaAboveArc = SignedSectorArea + AreaAbove(D->O) + AreaAbove(O->S) =
    //   SignedSectorArea + (DX-OX)*(DY+OY)/2 + (OX-SX)*(OY+SY)/2 =
    //   SSA + (DX*DY - OX*OY)/2 + (DX*OY - OX*DY)/2 + (OX*OY - SX*SY)/2 + (OX*SY - SX*OY)/2 =
    //   SSA + (DX*DY - SX*SY)/2 + (DX-SX)*OY/2 - (DY-SY)*OX/2.
    // Since we need ret = AreaAboveArc*2 - (DX*DY - SX*SY)/2 =
    //   SSA*2 + (DX-SX)*OY - (DY-SY)*OX.
    //Note that SectorArea() always assume clockwise and returns a positive value.
    return 2*(clockwise_arc ? ell.SectorArea(s, e) : -ell.SectorArea(e, s)) +
           - (end.x - start.x)*ell.GetCenter().y + (end.y-start.y)*ell.GetCenter().x;
}

bool Edge::IsSane() const
{
    if (!IsSaneNoBoundingBox()) return false;
    if (boundingBox.IsWithin(start) == WI_OUTSIDE ||
        boundingBox.IsWithin(end) == WI_OUTSIDE) return false;
    return true;
}

bool Edge::IsSaneNoBoundingBox() const
{
    if (type == ARC)
        if (!ell.Radian2Point(e).test_equal(end)) return false;
    if (type == FULL_CIRCLE)
        if (!end.test_equal(start)) return false;
    if (type != STRAIGHT)
        if (!ell.Radian2Point(s).test_equal(start)) return false;
    return true;
}



//In angle1 return the angle (in false angles in [0..4]) of a tangent to this edge at pos
//depending on incoming or outgoing.
//For curvy edges angle2 will be set according to which direction the curve goes
//and how wide the arc is.
//These two values will be used to order rays in a crosspoint
//p and pos must agree
RayAngle Edge::angle_curvy(bool incoming, const XY &p, double pos) const
{
    _ASSERT(!incoming || (pos!=0));
    _ASSERT( incoming || (pos!=1));
    _ASSERT(type != STRAIGHT);
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
    _ASSERT(std::min(fabs(a-b), fabs(a>b ? 2*M_PI-(a-b) : 2*M_PI-(b-a)))<2*M_PI);
    return std::min(fabs(a-b), fabs(a>b ? 2*M_PI-(a-b) : 2*M_PI-(b-a)));
}


//pos lists a series of pos values. We find the one that is closest to
//"rad", which is in radians. We return the radian.
double Edge::FindRadianOfClosestPos(unsigned num, double pos[], double rad)
{
    _ASSERT(type!=STRAIGHT);
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
bool Edge::UpdateClockWise(double new_s, double new_e)
{
    //check which is the smallest diff: s<->new_s plus e<->new_e or the other
    if (radian_diff_abs(s, new_s) + radian_diff_abs(e, new_e) <=
        radian_diff_abs(e, new_s) + radian_diff_abs(s, new_e))
        return false;
    clockwise_arc = !clockwise_arc;
    return true;
}


void Edge::SwapXYcurvy()
{
    _ASSERT(type!=STRAIGHT);
    ell.SwapXY();
    clockwise_arc=!clockwise_arc;
    if (ell.IsTilted()) {
        s = s==0 ? 0 : 2*M_PI-s;
        e = e==0 ? 0 : 2*M_PI-e;
    } else {
        s = s<M_PI/2 ? M_PI/2-s : 2.5*M_PI - s;
        e = e<M_PI/2 ? M_PI/2-e : 2.5*M_PI - e;
    }
};



double Edge::GetRadianMidPoint() const
{
    _ASSERT(type!=STRAIGHT);
    if (type==FULL_CIRCLE) return M_PI;
    if (e==s) return s;
    if (clockwise_arc == (s<e)) return (s+e)/2;
    return fmod_negative_safe((s+e)/2+M_PI, 2*M_PI);
}

XY Edge::Pos2Point(double pos) const
{
    _ASSERT(pos>=-0 && pos<=1);
    switch (type) {
    case STRAIGHT:
        return start + (end-start)*pos;
    case FULL_CIRCLE:
        return ell.Radian2Point(s + pos*(clockwise_arc ? 2*M_PI : -2*M_PI));
    case ARC:
        return ell.Radian2Point(pos2radian(pos));
    }
    _ASSERT(0);
    return XY();
}



//Removes the part of the edge or curve before point p. Assumes p lies on us.
Edge& Edge::SetStartStrict(const XY &p, double pos, bool keep_full_circle)
{
    start = p;
    if (type==STRAIGHT) {
        Edge::CalculateBoundingBox();
        return *this;
    }
    const double r = pos2radian(pos);
    _ASSERT(radianbetween(r));
    _ASSERT(ell.Radian2Point(r).test_equal(p));
    if (type==FULL_CIRCLE && !(test_equal(s,r) && keep_full_circle)) {
        e = s;
        type = ARC;
    }
    s = r;
    CalculateBoundingBox();
    return *this;
}

//Removes the part of the edge or curve before point p. Does not assumes p lies on us,
//this can be used to extend an edge
//We do not check if an arc wraps or not!!!
Edge& Edge::SetStartLiberal(const XY &p, bool keep_full_circle)
{
    start = p;
    if (type==STRAIGHT) {
        Edge::CalculateBoundingBox();
        return *this;
    }
    const double r = ell.Point2Radian(p);
    if (type==FULL_CIRCLE && !(test_equal(s,r) && keep_full_circle)) {
        e = s;
        type = ARC;
    }
    s = r;
    CalculateBoundingBox();
    return *this;
}


//Checks if this edge and "next" can be combined into a single edge
//if so, also update "this" to be the combined edge & return true
bool Edge::CheckAndCombine(const Edge &next)
{
    //if next is a degenerate (zero length) arc, we just skip that
    if (next.type != FULL_CIRCLE && next.start.test_equal(next.end))
        return true;
    if ((type==STRAIGHT) != (next.type==STRAIGHT)) return false;
    if (type==STRAIGHT) {
        const double a = angle(start, end, next.end);
        if (test_zero(a) || test_equal(a,2)) {
            end = next.end;
            return true;
        }
        return false;
    }
    //OK, now we are both curvy edges
    if (ell != next.ell) return false;
    //OK, we are the same ellipse
    //if any of us is full circle, keep only the other one
    //if both of us is a full circle, we must equal (since next.start==end), keep any
    if (next.type==FULL_CIRCLE)
        return true; //we keep what we are
    if (type==FULL_CIRCLE) {
        //start and s remain (must be equal to next.start)
        if (!test_equal(s,next.s) || !start.test_equal(next.start))
            return false;
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
        if (test_equal(next.e,s)) type = FULL_CIRCLE;
    } else {
        //opposite dir. never full circle
        //direction changes if new endpoint outside our old span
        if (!radianbetween(next.e)) clockwise_arc = !clockwise_arc;
    }
    e = next.e; //we may end up as degenerate (s==e) but that is life
    end = next.end;
    return true;
}

XY Edge::getcentroidareaaboveupscaled_straight(const XY&start, const XY&end)
{
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

XY Edge::getcentroidareaaboveupscaled_curvy() const
{
    XY ret = clockwise_arc ? ell.SectorCentroidTimesArea(s, e) : -ell.SectorCentroidTimesArea(e, s);
    ret += getcentroidareaaboveupscaled_straight(start, ell.GetCenter());
    ret += getcentroidareaaboveupscaled_straight(ell.GetCenter(), end);
    return ret;
}

//return false if curvy edge disappears
//Destroys bounding box!!!
bool Edge::Expand(double gap)
{
	if (type!=STRAIGHT) {
        const EllipseData old_ell(ell);
		switch (ell.Expand(clockwise_arc ? gap : -gap)) {
        case 1: //ell remained an ellipse
            start = ell.Radian2Point(s);
            if (type==ARC) end = ell.Radian2Point(e);
            else end = start; //full circle
            return true;
        case 0:
        case -1: //completely disappeared
            return false;
        //case 0:  //degenerates to line
        //    if (type==FULL_CIRCLE)
        //        return false; //start==end here, we do not return such a straight edge
        //    //if ellipse becomes too small, fall through to below and replace it with a straight line
        //    //Now decrease just up to the point od getting degenerate
        //    const double rad = std::min(old_ell.GetRadius1(), old_ell.GetRadius2());
        //    _ASSERT(rad<fabs(gap));
        //    ell = old_ell;
        //    ell.Expand(gap>0 ? rad : -rad);
        //    start = ell.Radian2Point(s);
        //    end = ell.Radian2Point(e);
        //    //and then move remaining
        //    gap = (gap>0) ? gap-rad : gap+rad;
        //    type = STRAIGHT;
        }
	}
    const double length = start.Distance(end);
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
//DEGENERATE: One of the edges have start==end
//  "newcp" is undefined on return
//SAME_ELLIPSIS: two arc of the same ellipses (no cp returned, radians not checked)
//  "newcp" is undefined on return
//CP_REAL: The two edges actually cross (between "start" and "end")
//CP_EXTENDED: The two edges do not meet but their extension does
//NO_CP_CONVERGE: Not even the extension of the two edges meet, but a linear extrapolation does
//  "newcp" is the convergence point: can be reached by adding linear segments
//NO_CP_PARALLEL: Either the two edges are parallel or their linear extrapolation
//  "newcp" is a calculated convergence point: can be reached by adding linear segments

Edge::EExpandCPType Edge::FindExpandedEdgesCP(const Edge&M, XY &newcp) const
{
    if (start.test_equal(end) || M.start.test_equal(M.end)) return DEGENERATE;
    const double parallel_join_multipiler = 5;
    if (M.type == STRAIGHT) {
        if (type == STRAIGHT) {
            switch (crossing_line_line(start, end, M.start, M.end, newcp)) {
            default: _ASSERT(0);
            case LINE_CROSSING_PARALLEL:
                if (end.test_equal(M.start)) {
                    newcp = end;
                    return CP_REAL;
                }
                return NO_CP_PARALLEL;
            case LINE_CROSSING_OUTSIDE: return CP_EXTENDED;
            case LINE_CROSSING_INSIDE: return CP_REAL;
            }
        } else {
            XY r[8];
            double radian_us[8], pos_M[8];
            int num = ell.CrossingStraight(M.start, M.end, r, radian_us, pos_M);
            if (!num) {
                const XY tangent = NextTangentPoint(1.0);
                if (crossing_line_line(end, tangent, M.start, M.end, newcp) != LINE_CROSSING_PARALLEL)
                    return NO_CP_CONVERGE;
                //if we are parallel, calculate a conv point
                const double dist = end.Distance(M.start) * parallel_join_multipiler;
                const double mlen = M.end.Distance(M.start);
                newcp = M.start + (M.start-M.end)/mlen*dist;
                return NO_CP_PARALLEL;
            } else {
                int pos = find_closest(num, radian_us, e, !clockwise_arc);
                newcp = r[pos];
                _ASSERT(newcp.x>-1000000 && newcp.x<10000000);
                if (radianbetween(radian_us[pos]) || between01_approximate_inclusive(pos_M[pos]))
                    return CP_REAL;
                return CP_EXTENDED;
            }
        }
    } else {
        XY r[8];
        if (type == STRAIGHT) {
            double pos_us[8], radian_M[8];
            int num = M.ell.CrossingStraight(start, end, r, radian_M, pos_us);
            if (!num) {
                const XY tangent = M.PrevTangentPoint(0.0);
                if (crossing_line_line(tangent, M.start, start, end, newcp) != LINE_CROSSING_PARALLEL)
                    return NO_CP_CONVERGE;
                //if we are parallel, calculate a conv point
                const double dist = end.Distance(M.start) * parallel_join_multipiler;
                const double len = end.Distance(start);
                newcp = end + (end-start)/len*dist;
                return NO_CP_PARALLEL;
            }
            int pos = find_closest(num, radian_M, M.s, M.clockwise_arc);
            newcp = r[pos];
            _ASSERT(newcp.x>-1000000 && newcp.x<10000000);
            if (M.radianbetween(radian_M[pos]) || between01_approximate_inclusive(pos_us[pos]))
                return CP_REAL;
            return CP_EXTENDED;
        } else {
            if (ell == M.ell) return SAME_ELLIPSIS; //we combine two segments of the same ellipse - all must be OK
            double radian_us[8], radian_M[8];
            int num = ell.CrossingEllipse(M.ell, r, radian_us, radian_M);
            if (!num) {
                const XY tangent1 = PrevTangentPoint(1.0);
                const XY tangent2 = M.NextTangentPoint(0.0);
                if (crossing_line_line(tangent1, end, tangent2, M.start, newcp) != LINE_CROSSING_PARALLEL)
                    return NO_CP_CONVERGE;
                //if we are parallel, calculate a conv point
                const double dist = end.Distance(M.start) * parallel_join_multipiler;
                const double len = end.Distance(tangent1);
                newcp = (end+M.start)/2 + (end-tangent1)/len*dist;
                return NO_CP_PARALLEL;
            }
            int pos = find_closest(num, radian_M, M.s, M.clockwise_arc);
            newcp = r[pos];
            _ASSERT(newcp.x>-1000000 && newcp.x<10000000);
            if (M.radianbetween(radian_M[pos]) || radianbetween(radian_us[pos]))
                return CP_REAL;
            return CP_EXTENDED;
        }
    }
}


void Edge::SetStartEndForExpand(const XY &S, const XY &E)
{
    double new_s, new_e;
    if (type!=STRAIGHT) {
        new_s = ell.Point2Radian(S);
        if (S.test_equal(E))
            new_e = new_s;
        else
            new_e = ell.Point2Radian(E);
    }
    switch (type) {
    case ARC:
        //check if direction has changed
        //start with an exception: if any is exactly 180 degrees, we assume same direction
        //(thus we test the rest only if none is exactly 180 degrees)
        //if (fabs(s-e) != M_PI && fabs(new_s-new_e) != M_PI) {
        //    const bool dir_us = (e>s && (e-s)<M_PI) || (s>e && (s-e)>=M_PI);
        //    const bool dir_M = (new_e>new_s && (new_e-new_s)<M_PI) || (new_s>new_e && (new_s-new_e)>=M_PI);
        //    if (dir_us != dir_M)
        //        clockwise_arc = !clockwise_arc;
        //}
        if (radian_diff_abs(e, new_e) + radian_diff_abs(s, new_s) >
            radian_diff_abs(e, new_s) + radian_diff_abs(s, new_e))
            clockwise_arc = !clockwise_arc;
        /* Falltherough */
    case FULL_CIRCLE:
        //for full circle we assume dir has not changed
        s = new_s;
        e = new_e;
        type = S.test_equal(E) ? FULL_CIRCLE : ARC;
    case STRAIGHT:
        break;
    }
    start = S;
    end = E;
}

bool Edge::IsOpposite(const XY &S, const XY &E) const
{
	if (start.test_equal(end)) return false;
	if (S.test_equal(E)) return false;
	if (type==STRAIGHT) {
		if (fabs(start.x-end.x) > fabs(start.y-end.y))
			return (start.x<end.x) != (S.x<E.x);
		else
			return (start.y<end.y) != (S.y<E.y);
	}
    const double new_s = ell.Point2Radian(S);
    const double new_e = ell.Point2Radian(E);
    return radian_diff_abs(e, new_e) + radian_diff_abs(s, new_s) >
           radian_diff_abs(e, new_s) + radian_diff_abs(s, new_e);
}

void Edge::CreateExpand2DCurvy(const XY &gap, std::vector<Edge> &ret, int &stype, int &etype) const
{
    _ASSERT(type!=STRAIGHT);
    unsigned index = ret.size();
    const unsigned original_index = index;
    const double ee = GetRadianE();
    ret.push_back(Edge());
    ret[index].type = ARC;
    ret[index].clockwise_arc = clockwise_arc;
    ret[index].start = start;
    ret[index].ell = ell;
    ret[index].s = s;
    XY extreme;
    double current = s;
    while (1) {
        const double next = ell.FindExtreme(current, clockwise_arc, extreme);
        //if we have moved over "ee" stop.
        if (!(type==FULL_CIRCLE && current==s) &&  //exclude the first segment of full circles
            fmod_negative_safe(clockwise_arc ? next-current : current-next, 2*M_PI) >=
            fmod_negative_safe(clockwise_arc ? ee-current : current-ee, 2*M_PI)) break;
        current = next;
        ret.resize(index+3);
        index+=2;
        ret[index].type = ARC;
        ret[index].clockwise_arc = clockwise_arc;
        ret[index].start = ret[index-2].end = extreme;
        ret[index].s     = ret[index-2].e   = fmod(current, 2*M_PI);
        ret[index].ell = ell;
    }
    ret[index].e = ee;
    ret[index].end = end;
    stype = Edge_CreateExpand2D::comp_int(ret[original_index].start.x, ret[original_index].end.x) +
            Edge_CreateExpand2D::comp_int(ret[original_index].start.y, ret[original_index].end.y)*3;
    for (unsigned u=original_index; u<=index; u+=2) {
        ret[u].CalculateBoundingBox();
        ret[u].Shift(XY(Edge_CreateExpand2D::comp_dbl(ret[u].end.y, ret[u].start.y, gap.x),
                        Edge_CreateExpand2D::comp_dbl(ret[u].start.x, ret[u].end.x, gap.y)));
    }
    if (index==original_index) {
        etype=stype;
        return;
    }
    for (unsigned u=original_index+1; u<index; u+=2) {
        ret[u].type = STRAIGHT;
        ret[u].start = ret[u-1].end;
        ret[u].end = ret[u+1].start;
        ret[u].CalculateBoundingBox();
    }
    etype = Edge_CreateExpand2D::comp_int(ret[index].start.x, ret[index].end.x) +
            Edge_CreateExpand2D::comp_int(ret[index].start.y, ret[index].end.y)*3;
}


//int Edge::CombineExpandedEdges(Edge&M, EExpandType et, const XY &old, Edge &res)
//{
//    XY cp;
//    if (M.type == STRAIGHT) {
//        if (type == STRAIGHT) {
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
//        if (type == STRAIGHT)
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
//        res.type = STRAIGHT;
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
//int Edge::IsOppositeDir(const Edge &M) const
//{
//	if (start.test_equal(end)) return 1;
//	if (type==STRAIGHT) {
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

double Edge::offsetbelow_curvy_straight(const Edge &M, bool straight_is_up, double &touchpoint) const
{
    _ASSERT(type!=STRAIGHT);
    _ASSERT(M.GetType()==STRAIGHT);
    double rad = CURVY_OFFSET_BELOW_GRANULARIRY*2/(ell.GetRadius1() + ell.GetRadius2());
    double end, beg;
    if (type==FULL_CIRCLE) {
        beg = 0;
        end = 2*M_PI;
    } else if (clockwise_arc) {
        beg = s;
        end = s<e ? e : e+2*M_PI;
    } else {
        beg = e;
        end = s>e ? s : s+2*M_PI;
    }
    Edge e(ell.Radian2Point(beg), XY(0,0));
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
double Edge::OffsetBelow(const Edge &o, double &touchpoint) const
{
    if (o.GetType() == STRAIGHT) {
        if (type != STRAIGHT)
            return offsetbelow_curvy_straight(o, false, touchpoint);
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
            return std::min(o.GetStart().y,o.GetEnd().y) - touchpoint;
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
    if (type == STRAIGHT)
        return o.offsetbelow_curvy_straight(*this, true, touchpoint);
    //now both of us are curves
    const double rad1 = CURVY_OFFSET_BELOW_GRANULARIRY*2/(ell.GetRadius1() + ell.GetRadius2());
    const double rad2 = CURVY_OFFSET_BELOW_GRANULARIRY*2/(o.ell.GetRadius1() + o.ell.GetRadius2());
    //TODO: Fix this for full_circle==true, too!!
    const double end1 = s<e ? e : e+2*M_PI;
    const double end2 = o.s<o.e ? o.e : o.e+2*M_PI;
    double ret = CONTOUR_INFINITY;
    Edge e1(start, XY(0,0));
    for (double r1 = s+rad1; r1<end1; r1+=rad1) {
        e1.end = ell.Radian2Point(r1);
        Edge e2(o.start, XY(0,0));
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


//here we merge to clockwise and counterclockwise
//directions are viewed from "this" toward "o"
bool Edge::TangentFrom(const Edge &o, XY clockwise[2], XY cclockwise[2]) const
{
    XY c, cc;
    clockwise[1]  = minmax_clockwise(clockwise[0], o.start, clockwise[1], true);
    cclockwise[1] = minmax_clockwise(cclockwise[0], o.start, cclockwise[1], false);

    clockwise[0]  = minmax_clockwise(clockwise[1], start, clockwise[0], false);
    cclockwise[0] = minmax_clockwise(cclockwise[1], start, cclockwise[0], true);

    //clockwise[1]  = minmax_clockwise(clockwise[0], o.start, clockwise[1], true);
    //cclockwise[1] = minmax_clockwise(cclockwise[0], o.start, cclockwise[1], false);

    if (type==STRAIGHT) {
        if (o.type!=STRAIGHT) {
            if (o.TangentFrom(start, c, cc)) {
                if (o.radianbetween(o.ell.Point2Radian(c)))
                    clockwise[1]  = minmax_clockwise(start, c, clockwise[1], true);
                if (o.radianbetween(o.ell.Point2Radian(cc)))
                    cclockwise[1] = minmax_clockwise(start, cc, cclockwise[1], false);
            }
        }
    } else if (o.type == STRAIGHT) {
        if (TangentFrom(o.start, c, cc)) {
            if (radianbetween(ell.Point2Radian(c)))
                clockwise[0]  = minmax_clockwise(o.start, cc, clockwise[0], false);
            if (radianbetween(ell.Point2Radian(cc)))
                cclockwise[0] = minmax_clockwise(o.start, c, cclockwise[0], true);
        }
    } else {
        XY c[2], cc[2];
        if (ell.TangentFrom(o.ell, c, cc)) {
            if (radianbetween(ell.Point2Radian(c[0])) && o.radianbetween(o.ell.Point2Radian(c[1]))) {
                clockwise[1]  = minmax_clockwise(clockwise[0], c[1], clockwise[1], true);
                clockwise[0]  = minmax_clockwise(clockwise[1], c[0], clockwise[0], false);
            }
            if (radianbetween(ell.Point2Radian(cc[0])) && o.radianbetween(o.ell.Point2Radian(cc[1]))) {
                cclockwise[0] = minmax_clockwise(cclockwise[1], cc[0], cclockwise[0], true);
                cclockwise[1] = minmax_clockwise(cclockwise[0], cc[1], cclockwise[1], false);
            }
        }
        if (o.ell.TangentFrom(ell, c, cc)) {
            if (radianbetween(ell.Point2Radian(c[1])) && o.radianbetween(o.ell.Point2Radian(c[0]))) {
                clockwise[1]  = minmax_clockwise(clockwise[0], cc[0], clockwise[1], false);
                clockwise[0]  = minmax_clockwise(clockwise[1], cc[1], clockwise[0], true);
            }
            if (radianbetween(ell.Point2Radian(cc[1])) && o.radianbetween(o.ell.Point2Radian(cc[0]))) {
                cclockwise[0] = minmax_clockwise(cclockwise[1], c[1], cclockwise[0], false);
                cclockwise[1] = minmax_clockwise(cclockwise[0], c[0], cclockwise[1], true);
            }
        }
    }
    return true;
}

} //namespace