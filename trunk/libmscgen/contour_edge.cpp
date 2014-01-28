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

//namespace contour_standard_edge {
//
////////////////////Helper functions
//
////slope of a line (assuming not vertical)
//inline double MM(XY A, XY B)
//{
//    return (A.y-B.y)/(A.x-B.x);
//}
//
//EdgeArc::EdgeArc(const XY &c, double radius_x, double radius_y, double tilt_deg, double s_deg, double d_deg) :
//    type(FULL_CIRCLE), s(deg2rad(s_deg)), e(deg2rad(d_deg)), clockwise_arc(true),
//    ell(c, radius_x, radius_y, tilt_deg)
//{
//    //BoundingBox will be calculated in Edge::Edge
//}
//
/////////////////////////////////////////////////////////////////////////////////////
//
////to generate a full circle instead of an empty one, specify d_deg as s_deg+360
//Edge::Edge(const XY &c, double radius_x, double radius_y, double tilt_deg, double s_deg, double d_deg)
//    : arc(new EdgeArc(c, radius_x, radius_y, tilt_deg, s_deg, d_deg)), visible(true)
//{
//    start = arc->ell.Radian2Point(arc->s);
//    if (test_equal(s_deg, d_deg) || !test_equal(arc->s, arc->e)) {
//        arc->type = EdgeArc::ARC;
//        end = arc->ell.Radian2Point(arc->e);
//    } else {
//        arc->e = arc->s;
//        end = start;
//    }
//    CalculateBoundingBoxCurvy();
//}
//
///** Return the distance of a point from the edge.
// *
// * @param [in] p The distance is from this point.
// * @param [out] point A point on the edge closes to `p`.
// * @param [out] pos The pos value [0..1] corresponding to `point`.
// * @returns The distance.
// */
//double Edge::Distance(const XY &p, XY &point, double &pos) const
//{
//    if (!arc) {
//        if (test_equal(start.x, end.x)) {
//            point.x = start.x;
//            if (p.y<std::min(start.y, end.y)) {
//                point.y = std::min(start.y, end.y);
//                pos = end.y<start.y ? 1 : 0;
//            } else if (p.y>std::max(start.y, end.y)) {
//                point.y = std::max(start.y, end.y);
//                pos = end.y>start.y ? 1 : 0;
//            } else {
//                point.y = p.y;
//                pos = (p.y-start.y)/(end.y-start.y);
//                return fabs(start.x - p.x);
//            }
//            return point.Distance(p);
//        }
//        if (test_equal(start.y, end.y)) {
//            point.y = start.y;
//            if (p.x<std::min(start.x, end.x)) {
//                point.x = std::min(start.x, end.x);
//                pos = end.x<start.x ? 1 : 0;
//            } else if (p.x>std::max(start.x, end.x)) {
//                point.x = std::max(start.x, end.x);
//                pos = end.x>start.x ? 1 : 0;
//            } else {
//                point.x = p.x;
//                pos = (p.x-start.x)/(end.x-start.x);
//                return fabs(start.y - p.y);
//            }
//            return point.Distance(p);
//        }
//        //http://mathworld.wolfram.com/Point-LineDistance2-Dimensional.html
//        const double m = fabs((start.x-p.x)*(end.x-start.x)-(end.y-start.y)*(start.y-p.y))/start.DistanceSqr(end);
//        if (m<0) {point = start; pos = 0; return start.Distance(p);}
//        if (m>1) {point = end; pos = 1; return end.Distance(p);}
//        point = start + (end-start)*m;
//        pos = m;
//        //return fabs((end.x-start.x)*(start.y-p.y)-(start.x-p.x)*(end.y-start.y))/l;
//        return point.Distance(p);
//    }
//    if (arc->type==EdgeArc::FULL_CIRCLE) {
//        const double d = arc->ell.Distance(p, point, pos);
//        pos = radian2pos(pos);
//        return d;
//    }
//    double radian;
//    const double dist = arc->ell.Distance(p, point, radian);
//    if (radianbetween(radian)) { //on arc
//        pos = radian2pos(radian);
//        return dist;  
//    }
//    //outside: find the closest endpoint
//    if (std::min(fmod_negative_safe(radian - arc->s, 2*M_PI), fmod_negative_safe(radian - arc->s, 2*M_PI)) <
//        std::min(fmod_negative_safe(radian - arc->e, 2*M_PI), fmod_negative_safe(radian - arc->e, 2*M_PI)))
//        {point = start; pos = 0; return start.Distance(p);}
//    else
//        {point = end; pos = 1; return end.Distance(p);}
//}
//
///** Calculates the distance to another edge
// */
//DistanceType Edge::Distance(const Edge &o) const
//{
//    DistanceType ret;
//    if (!arc) {
//        if (!o.arc) {
//            //test if we cross
//            if (!test_zero((end-start).PerpProduct(o.end-o.start))) {
//                //They are not parallel (and none of them are degenerate, but that does not matter now)
//                const double t = (end-start).PerpProduct(start-o.start) / (end-start).PerpProduct(o.end-o.start);
//                const double s = (o.end-o.start).PerpProduct(start-o.start) / (end-start).PerpProduct(o.end-o.start);
//                if (between01_approximate_inclusive(t) && between01_approximate_inclusive(s)) {
//                    ret.Merge(0, start + (end-start)*t, start + (end-start)*t);
//                    return ret;
//                }
//                //here the two infinite lines cross, but the cp is outside at least one
//                //of the two sections
//            }
//            //They are either parallel, degenerate or just far away
//            //calculate the start distance only, end will come anyway
//            XY point1, point2;
//            double dummy;
//            const double d1 = Distance(o.start, point1, dummy);
//            const double d2 = o.Distance(start, point2, dummy);
//            if (d1<d2)
//                ret.Merge(d1, point1, o.start);
//            else
//                ret.Merge(d2, start, point2);
//            return ret;
//        }
//        if (o.arc->type == EdgeArc::FULL_CIRCLE) {
//            XY p[2];
//            const double d = o.GetEllipseData().Distance(start, end, p);
//            //now p[0] is a point on the ellipse, p[1] is a point on the line "this"
//            const double line_pos = fabs(start.x-end.x) < fabs(start.y - end.y) ?
//                                    (p[1].y-start.y)/(end.y-start.y) : (p[1].x-start.x)/(end.x-start.x);
//            if (d<0) { //we cross
//                //the two points are the crosspoints
//                //check if the first in inside the section
//                if (between01_approximate_inclusive(line_pos)) {
//                    ret.Merge(0, p[1], p[1]);
//                    return ret;
//                }
//                //now check if the other is
//                const double line_pos2  = fabs(start.x-end.x) < fabs(start.y - end.y) ?
//                                           (p[0].y-start.y)/(end.y-start.y) : (p[0].x-start.x)/(end.x-start.x);
//                if (between01_approximate_inclusive(line_pos2)) {
//                    ret.Merge(0, p[0], p[0]);
//                    return ret;
//                }
//                //none of the crosspoints are between start and end of the section
//                //if both are at the same side, return closer one
//                if ((line_pos<0) == (line_pos2<0)) {
//                    if (line_pos < 0) goto start_is_closer;
//                    else goto end_is_closer;
//                } else { //we are inside the "o"
//                    if (line_pos<0) { //"point_on_line" is closer to "start" than to "end"
//                        if (-line_pos < line_pos2-1)  goto start_is_closer;
//                        else goto end_is_closer;
//                    } else {//"point_on_line" is closer to "end" than to "start"
//                        if (line_pos-1 < -line_pos2) goto end_is_closer;
//                        else goto start_is_closer;
//                    }
//                }
//            }
//            //if we touch & touchpoint between start and end
//            if (d==0 && between01_approximate_inclusive(line_pos)) {
//                ret.Merge(0, p[0], p[0]);
//                return ret;
//            }
//            //d>0: //line avoids ellipse
//            if (!test_smaller(0,line_pos)) {//"start" is closer
//start_is_closer:
//                XY other;
//                double dummy;
//                const double d2 = o.arc->ell.Distance(start, other, dummy);
//                ret.Merge(d2, start, other);
//                return ret;
//            }
//            if (!test_smaller(line_pos,1)) { //"end" is closer
//end_is_closer:
//                XY other;
//                double dummy;
//                const double d2 = o.arc->ell.Distance(end, other, dummy);
//                ret.Merge(d2, end, other);
//                return ret;
//            }
//            //"p[1]" contains a point of the line "this" which is closest to the ellipse "o"
//            XY other;
//            double dummy;
//            const double di = o.arc->ell.Distance(p[1], other, dummy);
//            ret.Merge(di, p[1], other);
//            return ret;
//        }
//        if (o.arc->type == EdgeArc::ARC) {
//            //The following points can be the shortest distance
//            //the infinite line and the full circle
//            //the start and endpoints of either arc to the other
//            //(from the latter we skip endpoints as the next arc will do it)
//            XY p[2];
//            double d = o.GetEllipseData().Distance(start, end, p);
//            //now p[0] is a point on the ellipse, p[1] is a point on the line "this"
//            //now validate that this is a real distance
//            const double line_pos  = fabs(start.x-end.x) < fabs(start.y - end.y) ?
//                                           (p[1].y-start.y)/(end.y-start.y) : (p[1].x-start.x)/(end.x-start.x);
//            const double rad = o.GetEllipseData().Point2Radian(p[0]);
//            if (d>=0) {//touch or apart
//                if (!between01_approximate_inclusive(line_pos) || !o.radianbetween(rad))
//                    d = MaxVal(d); //other options will be closer
//            } else { //cross
//                //if any crosspoint is valid we are 0 distance
//                const double line_pos2 = fabs(start.x-end.x) < fabs(start.y - end.y) ?
//                                            (p[0].y-start.y)/(end.y-start.y) : (p[0].x-start.x)/(end.x-start.x);
//                //check line_pos and rad generated from point on ell (="p[0]")
//                if (between01_approximate_inclusive(line_pos2) && o.radianbetween(rad)) {
//                ret.Merge(0, p[0], p[0]);
//                    return ret;
//                }
//                //check line_pos and rad generated from point on line (="p[1]")
//                const double rad2 = o.GetEllipseData().Point2Radian(p[1]);
//                if (between01_approximate_inclusive(line_pos) && o.radianbetween(rad2)) {
//                ret.Merge(0, p[1], p[1]);
//                    return ret;
//                }
//                d = MaxVal(d); //other options will be closer
//            }
//            //Here d is nonnegative (can be maxval)
//            //Ok, now check if the startpoint of the arc is closer to the line
//            XY point1, point2;
//            double dummy;
//            const double d1 = Distance(o.start, point1, dummy);
//            const double d2 = o.Distance(start, point2, dummy);
//            if (d < d1 && d < d2) {
//                ret.Merge(d, p[1], p[0]);
//                return ret;
//            }
//            if (d1<d2)
//                ret.Merge(d1, point1, o.start);
//            else
//                ret.Merge(d2, start, point2);
//            return ret;
//        }
//        _ASSERT(0); //not arc neither full_circle nor straight
//    }
//    if (!o.arc) {
//        ret = o.Distance(*this); //straight-curvy combinations
//        ret.SwapPoints();
//        return ret;
//    }
//    //Now we have two curvy edges
//    //We select the one with shorter arc for interpolation
//    //TODO: replace to binary search
//    const double l1 = (arc->ell.GetRadius1() + arc->ell.GetRadius1())*GetSpan();
//    const double l2 = (o.arc->ell.GetRadius1() + o.arc->ell.GetRadius1())*o.GetSpan();
//    const Edge &a = l1<l2 ? *this : o;
//    const Edge &b = l1<l2 ? o : *this;
//    const double step_length = 5;
//    const double step_arc = step_length/((a.arc->ell.GetRadius1() + a.arc->ell.GetRadius1()));
//    double r1 = a.GetRadianS(), r2 = a.GetRadianE();
//    if (a.GetClockWise()) std::swap(r1, r2);
//    if (r2<=r1) r2 += 2*M_PI;
//    const XY me = a.GetEllipseData().Radian2Point(r2);
//    XY other;
//    double dummy;
//    const double d = b.Distance(me, other, dummy);
//    ret.Merge(d, me, other);
//    for (; r1<r2; r1+=step_arc) {
//        const XY me2 = a.GetEllipseData().Radian2Point(r1);
//        const double d2 = b.Distance(me2, other, dummy);
//        ret.Merge(d2, me2, other);
//        if (ret.IsZero()) return ret;
//    }
//    return ret;
//}
//
//
///** Roatates edge around the origin. 
// *
// * @param [in] cos The (pre-computed) cosine of `radian`.
// * @param [in] sin The (pre-computed) sine of `radian`.
// * @param [in] radian The amount to rotate (positive if clockwise).
// */
//void Edge::Rotate(double cos, double sin, double radian)
//{
//    start.Rotate(cos, sin);
//    end.Rotate(cos, sin);
//    if (arc) {
//        const double turn = arc->ell.Rotate(cos, sin, radian);
//        if (turn) {
//            arc->s += turn; if (arc->s>=2*M_PI) arc->s-= 2*M_PI;
//            arc->e += turn; if (arc->e>=2*M_PI) arc->e-= 2*M_PI;
//        }
//        CalculateBoundingBoxCurvy();
//    }
//    _ASSERT(IsSane());
//}
//
///** Roatates ellipse around the `c`.
// *
// * @param [in] c The point to rotate around.
// * @param [in] cos The (pre-computed) cosine of `radian`.
// * @param [in] sin The (pre-computed) sine of `radian`.
// * @param [in] radian The amount to rotate (positive if clockwise).
// */
//void Edge::RotateAround(const XY&c, double cos, double sin, double radian)
//{
//    start.RotateAround(c, cos, sin);
//    end.RotateAround(c, cos, sin);
//    if (arc) {
//        const double turn = arc->ell.RotateAround(c, cos, sin, radian);
//        if (turn) {
//            arc->s += turn; if (arc->s>2*M_PI) arc->s-= 2*M_PI;
//            arc->e += turn; if (arc->e>2*M_PI) arc->e-= 2*M_PI;
//        }
//        CalculateBoundingBoxCurvy();
//    }
//    _ASSERT(IsSane());
//}
//
///** Transposes the edge: swaps x & y coordinates of all its points.
// */
//void Edge::SwapXY()
//{
//    start.SwapXY();
//    end.SwapXY();
//    if (arc) {
//        arc->boundingBox.SwapXY();
//        arc->ell.SwapXY();
//        arc->clockwise_arc = !arc->clockwise_arc;
//        if (arc->ell.IsTilted()) {
//            arc->s = arc->s==0 ? 0 : 2*M_PI - arc->s;
//            arc->e = arc->e==0 ? 0 : 2*M_PI - arc->e;
//        } else {
//            arc->s = arc->s < M_PI/2 ? M_PI/2 - arc->s : 2.5*M_PI - arc->s;
//            arc->e = arc->e < M_PI/2 ? M_PI/2 - arc->e : 2.5*M_PI - arc->e;
//        }
//    }
//}
//
///** Calculates the crosspoints with another edge.
// * 
// * This must ensure that we do not return pos values that are very close to 0 or 1
// * such values are to be rounded to 0 or 1 and in that case exatly the endpoint of the arc
// * shall be returned.
// *
// * @param [in] o The other edge.
// * @param [out] r The crosspoins.
// * @param [out] pos_my The pos values of the corresponding crosspoints on me.
// * @param [out] pos_other The pos values of the corresponding crosspoints on the other edge.
// * @returns The number of crosspoins found [0..4].
// */
//unsigned Edge::Crossing(const Edge &o, XY r[], double pos_my[], double pos_other[]) const
//{
//    //First check bounding boxes
//    if (!CreateBoundingBox().Overlaps(o.CreateBoundingBox(), SMALL_NUM))
//        return 0;
//    //call ourselves with swapped "this" and "o" so that "this" is later in this list:
//    //straight, full_circle, arc
//    if ((arc==NULL && o.arc!=NULL) ||
//        (arc!=NULL && o.arc!=NULL && arc->type<o.arc->type))
//        return o.Crossing(*this, r, pos_other, pos_my);
//    if (!arc) {
//        //two straight lines
//        //return the number of points:
//        //0 - no crossing
//        //1 - one crossing point (in r[0])
//        //2 - the two sections intersects from r[0] to r[1]
//        //if any of the two sections are degenerate we return 1 only if it lies on the other section, else 0
//        //in pos_in_ab we return the relative pos of the crosspoint(s) in AB, in pos_in_mn for MN
//        //See http://softsurfer.com/Archive/algorithm_0104/algorithm_0104B.htm
//        //crossing points that result in a pos value close to 1 are ignored
//        //In case AB is the same (or lies on the same line) as MN, the two endpoint of the common
//        //sections are returned (if they overlap or touch)
//        if (!test_zero((end-start).PerpProduct(o.end-o.start))) {
//            //They are not parallel (and none of them are degenerate, but that does not matter now)
//            double t = (end-start).PerpProduct(start-o.start) / (end-start).PerpProduct(o.end-o.start);
//            if (!between01_adjust(t)) return 0;
//            double s = (o.end-o.start).PerpProduct(start-o.start) / (end-start).PerpProduct(o.end-o.start);
//            if (!between01_adjust(s)) return 0; //the intersection of the line is outside AB
//            r[0] = o.start + (o.end-o.start)*t;
//            pos_other[0] = t;
//            pos_my[0] = s;
//            return 1;
//        }
//        //either they are parallel or one or both sections are degenerate (= a point)
//        if (o.start==o.end) {
//            if (!test_zero((o.start-start).PerpProduct(end-start)))
//                return 0; //o.start is not on AB's line and AB is not degenerate
//            if (start==end) {
//                if (start==o.start) {
//                    r[0] = o.start;
//                    pos_other[0] = 0;
//                    pos_my[0] = 0;
//                    return 1;
//                }
//                return 0; //both sections degenerate to a point, but do not overlap
//            }
//            double s;
//            //select x or y depending on precision
//            //o.start lies on AB, let us see if between start and end
//            if (fabs(start.x-end.x) > fabs(start.y-end.y))
//                s = (o.start.x-start.x)/(end.x-start.x);
//            else
//                s = (o.start.y-start.y)/(end.y-start.y);
//            if (!between01_adjust(s)) return 0;  //if outside [0..1] or too close to 1
//            r[0] = o.start; //o.start is on AB
//            pos_other[0] = 0;
//            pos_my[0] = s;
//            return 1;
//        }
//        if (!test_zero((start-o.start).PerpProduct(o.end-o.start)))
//            return 0; //start is not on MN's line
//        //They are either parallel and on the same line or AB is degenerate and lies on MN's line
//        double t[2];
//        //select x or y depending on precision
//        if (fabs(o.start.x-o.end.x) > fabs(o.start.y-o.end.y)) {
//            t[0] = (start.x-o.start.x)/(o.end.x-o.start.x);
//            t[1] = (end.x-o.start.x)/(o.end.x-o.start.x);
//        } else {
//            t[0] = (start.y-o.start.y)/(o.end.y-o.start.y);
//            t[1] = (end.y-o.start.y)/(o.end.y-o.start.y);
//        }
//        if (t[0] > t[1]) std::swap(t[0], t[1]);
//	    if (!test_smaller(t[0], 1) || test_smaller(t[1], 0))
//            return 0; //AB lies outside MN
//	    if (!test_smaller(0,t[0])) t[0] = 0; //if close to 0 or even smaller snap it there
//	    const bool dosecond = test_smaller(0,t[1]) && test_smaller(t[1],1); //if t[1]!~=0 and t[1]!~=1
//	    unsigned num = 0;
//	    for (int i = 0; i<(dosecond?2:1); i++) {
//		    r[num] = o.start + (o.end-o.start)*t[i];
//		    pos_other[num] = t[i];
//		    if (fabs(start.x-end.x) > fabs(start.y-end.y))
//			    pos_my[num] = (r[num].x-start.x)/(end.x-start.x);
//		    else
//			    pos_my[num] = (r[num].y-start.y)/(end.y-start.y);
//		    if (between01_adjust(pos_my[num])) num++;
//	    }
//	    return num;
//
//    }
//    //"this" is either a full circle or an arc
//    int num;
//    XY loc_r[8];
//    double loc_my[8], loc_other[8];
//    if (arc->type==EdgeArc::FULL_CIRCLE) {
//        //"o" can be either straight or full circle here
//        _ASSERT(!arc || arc->type == EdgeArc::FULL_CIRCLE);
//        if (!o.arc) {
//            num = arc->ell.CrossingStraight(o.start, o.end, loc_r, loc_my, loc_other);
//            int ret = 0;
//            for (num--; num>=0; num--) {
//		        //for curves, convert to position from radian
//                loc_my[num] = radian2pos(loc_my[num]);
//		        //now loc_XY is a position [0..1] indicate on edge
//		        //test if loc_ab is proper (snap to zero if close, drop if much below zero or close or above 1
//		        if (test_smaller(loc_my[num], 0)) //much smaller than 0
//			        continue;
//		        if (!test_smaller(0, loc_my[num])) { //if close to 0 snap it there
//			        loc_my[num] = 0;
//			        loc_r[num] = start;
//		        } else if (!test_smaller(loc_my[num], 1))  //if close or above to 1 skip
//			        continue;
//		        //test if loc_other
//		        if (test_smaller(loc_other[num], 0)) //much smaller than 0
//			        continue;
//		        if (!test_smaller(0, loc_other[num])) { //if close to 0 snap it there
//			        loc_other[num] = 0;
//			        loc_r[num] = o.start;
//		        } else if (!test_smaller(loc_other[num], 1))  //if close or above to 1 skip
//			        continue;
//		        //Both OK: add to points we return
//                r[ret] = loc_r[num];
//                pos_my[ret] = loc_my[num];
//                pos_other[ret] = loc_other[num];
//                ret++;
//            }
//            return ret;
//        }
//        //Two full cicrles
//        num = arc->ell.CrossingEllipse(o.arc->ell, loc_r, loc_my, loc_other);
//        if (num == -1) { //two equal ellipses, s may be different
//            r[0] = start;
//            pos_my[0] = 0;
//            pos_other[0] = o.radian2pos(arc->s); //az en kezdopontom az o rendszereben
//            if (arc->s==o.arc->s) return 1;
//            r[1] = o.start;
//            pos_my[1] = radian2pos(o.arc->s);
//            pos_other[1] = 0;
//            return 2;
//        }
//        //Now we have the radian(s) in loc_my and loc_other. Convert to xy and pos & return
//        unsigned numnum = 0;
//        for (int i=0; i<num; i++) {
//            r[numnum] = loc_r[i];
//            pos_my[numnum] = radian2pos(loc_my[i]);
//            pos_other[numnum] = o.radian2pos(loc_other[i]);
//            if (!test_smaller(pos_my[i],1) || !test_smaller(pos_other[i], 1))
//                continue;
//            numnum++;
//        }
//        return num;
//    }
//    //"this" is an ARC
//    if (!o.arc) {
//        num = arc->ell.CrossingStraight(o.start, o.end, loc_r, loc_my, loc_other);
//        int ret = 0;
//        for (num--; num>=0; num--) {
//            //if (!radianbetween(loc_my[num])) continue;
//		    //for curves, convert to position from radian
//            loc_my[num] = radian2pos(loc_my[num]);
//		    //now loc_XY is a position [0..1] indicate on edge
//		    //test if loc_ab is proper (snap to zero if close, drop if much below zero or close or above 1
//		    if (test_smaller(loc_my[num], 0)) //much smaller than 0
//			    continue;
//		    if (!test_smaller(0, loc_my[num])) { //if close to 0 snap it there
//			    loc_my[num] = 0;
//			    loc_r[num] = start;
//                loc_other[num] = point2pos_straight(o.start, o.end, start);
//		    } else if (!test_smaller(loc_my[num], 1))  //if close or above to 1 skip
//			    continue;
//		    //test if loc_other
//		    if (test_smaller(loc_other[num], 0)) //much smaller than 0
//			    continue;
//		    if (!test_smaller(0, loc_other[num])) { //if close to 0 snap it there
//			    loc_other[num] = 0;
//			    loc_r[num] = o.start;
//                loc_my[num] = radian2pos(arc->ell.Point2Radian(o.start));
//                if (test_zero(loc_my[num])) loc_my[num] = 0;
//		    } else if (!test_smaller(loc_other[num], 1))  //if close or above to 1 skip
//			    continue;
//            //OK add as a crosspoint
//            r[ret] = loc_r[num];
//            pos_my[ret] = loc_my[num];
//            pos_other[ret] = loc_other[num];
//            _ASSERT(Pos2Point(pos_my[ret]).test_equal(r[ret]));
//            _ASSERT(o.Pos2Point(pos_other[ret]).test_equal(r[ret]));
//            ret ++;
//        }
//        return ret;
//    }
//    //here "this" is an arc and "o" is either an arc or a full circle
//    num = arc->ell.CrossingEllipse(o.arc->ell, loc_r, loc_my, loc_other);
//    if (num==0) return 0;
//    if (num>0) {
//        //Now we have the radian(s) in loc_my & loc_other.
//        //Convert to xy and pos (for both arcs) & return
//        for (int i=0; i<num; i++) {
//            r[i] = loc_r[i];
//            pos_my[i] = radian2pos(loc_my[i]);
//            pos_other[i] = o.radian2pos(loc_other[i]);
//            if (!test_smaller(pos_my[i], 1) || !test_smaller(pos_other[i], 1) ||
//                 test_smaller(pos_my[i], 0) ||  test_smaller(pos_other[i], 0)) {
//                if (i==num-1) {
//                    num--;
//                    break;
//                }
//                loc_my[0] = loc_my[1]; //can only be if num==2, i==1
//                loc_other[0] = loc_other[1];
//                loc_r[0] = loc_r[1];
//                i=-1; num=1;
//            } else {
//                if (!test_smaller(0, pos_my[i])) pos_my[i] = 0;
//                if (!test_smaller(0, pos_other[i])) pos_other[i] = 0; //snap to zero
//            }
//        }
//        for (unsigned u=0; u<unsigned(num); u++) {
//            _ASSERT(Pos2Point(pos_my[u]).test_equal(r[u]));
//            _ASSERT(o.Pos2Point(pos_other[u]).test_equal(r[u]));
//        }
//        return num;
//    }
//
//    //num==-1 => two equal ellipses, determine joint section (if any)
//    if (o.arc->type==EdgeArc::FULL_CIRCLE) { //if other is full circle, the two poins are "s" and "e"
//        r[0] = start;
//        pos_my[0] = 0;
//        pos_other[0] = o.radian2pos(arc->s);
//        //we do not report "e", as its pos==1
//        return 1;
//    }
//    //here two arcs. Find intersection of them
//    if (radianbetween(o.arc->s)) {
//        loc_my[0] = o.arc->s;
//        loc_r[0] = o.start;
//        if (radianbetween(o.arc->e)) {
//            loc_my[1] = o.arc->e;
//            loc_r[1] = o.end;
//        } else
//            if (o.radianbetween(arc->s)) {
//                loc_my[1] = arc->s;
//                loc_r[1] = start;
//            } else
//                if (o.radianbetween(arc->e)) {
//                    loc_my[1] = arc->e;
//                    loc_r[1] = end;
//                } else
//                    num = 1;
//    } else if (radianbetween(o.arc->e)) {
//        loc_my[0] = o.arc->e;
//        loc_r[0] = o.end;
//        if (o.radianbetween(arc->s)) {
//            loc_my[1] = arc->s;
//            loc_r[1] = start;
//        } else
//            if (o.radianbetween(arc->e)) {
//                loc_my[1] = arc->e;
//                loc_r[1] = end;
//            } else
//                num = 1;
//    } else if (o.radianbetween(arc->s)) {
//        loc_my[0] = arc->s;
//        loc_r[0] = start;
//        if (o.radianbetween(arc->e)) {
//            loc_my[1] = arc->e;
//            loc_r[1] = end;
//        } else
//            num = 1;
//    } else if (o.radianbetween(arc->e)) {
//        loc_my[0] = arc->e;
//        loc_r[0] = end;
//        num = 1;
//    } else
//        return 0;
//    if (num == -1) //we have not changed num above
//        num = (loc_my[0] == loc_my[1]) ? 1 : 2;
//    unsigned res_num = 0;
//    for (unsigned i=0; i<unsigned(num); i++) {
//        if (loc_my[i]==arc->e || loc_my[i]==o.arc->e) continue; //do not return pos==1 cps
//        r[res_num] = loc_r[i];
//        pos_my[res_num] = radian2pos(loc_my[i]);
//        pos_other[res_num] = o.radian2pos(loc_my[i]); //loc_my contains good radians for "o", too
//        _ASSERT(Pos2Point(pos_my[res_num]).test_equal(r[res_num]));
//        _ASSERT(o.Pos2Point(pos_other[res_num]).test_equal(r[res_num]));
//        res_num++;
//    }
//    return res_num;
//}
//
////test if the end of an arc equals to a point or not
////x coordinate must match exactly, x can be approximate
//inline bool test_arc_end(const XY &a, double x, double y)
//{
//    return a.x==x && test_equal(a.y, y);
//}
//
///** Calculates where an edge crosses a vertical line.
// *
// * This is done for the purposes of SimpleContour::IsWithin.
// * Certain rules apply to the case when the vertical line crosses one of the ends of the edge.
// * 1. a leftward edge includes its starting endpoint, and excludes its final endpoint;
// * 2. a rightward edge excludes its starting endpoint, and includes its final endpoint;
// * In short: we include the endpoint with the larger x coordinate only.
// * @param [in] x The x coordinate of the vertical line.
// * @param [out] y The y coordinates of the crosspoints.
// * @param [out] pos The pos values for the respective crosspoints [0..1]
// * @param [out] forward True for a crosspoint if the line at x is crossed from left to right 
// *              (so inside of contour is below y).
// * @returns The number of crosspoints, or -1 if edhe is a vertical line exacty on x.
// */
//int Edge::CrossingVertical(double x, double y[], double pos[], bool forward[]) const
//{
//    if (!arc) {
//        if ((start.x >= x && end.x < x) ||      //we cross leftward
//            (start.x < x && end.x >= x)) {      //we cross rightward
//            //we cross p's line y
//		    pos[0] = (x - start.x)/(end.x - start.x);
//            y[0] = (end.y - start.y)*pos[0]+ start.y;
//            forward[0] = start.x < x; //we cross rightward
//            return 1;
//        }
//        if (start.x == x && end.x == x) return -1; //vertical line
//        return 0;
//    }
//    int num = arc->ell.CrossingVertical(x, y, pos);  //pos here is in radian
//    if (num<=0) return 0;
//    //if the crosspoints are at the end of the segments, we check for left and rightward crossing.
//    if (num==1) { //just touch
//        if (arc->type==EdgeArc::FULL_CIRCLE) return 0;
//		//we return a cp only if the cp is one of the endpoints, in which case the rules below apply
//        //now, we touch a vertical line with either start or end of an arc
//        //if center is left of x, we touch the larger end (both clockwise and cclockwise)
//		if (arc->ell.GetCenter().x >= x)
//			return 0;
//        if (test_arc_end(start, x, y[0])) {
//			y[0] = start.y;
//			pos[0] = 0;
//            forward[0] = false;  //always from left to right with startpoints
//			return 1;
//		}
//		if (test_arc_end(end, x, y[0])) {
//			y[0] = end.y;
//			pos[0] = 1;
//            forward[0] = true;  //always from left to right with endpoints
//			return 1;
//		}
//        //not close to any endpoint: touching is not valid then
//		return 0;
//	}
//    if (arc->type==EdgeArc::FULL_CIRCLE) {
//        for (int i=0; i<2; i++) {
//            pos[i] = radian2pos(pos[i]);
//            forward[i] = (y[i] == std::min(y[0], y[1])) == arc->clockwise_arc;
//        }
//        return num;
//    }
//    //now we are an arc
//    num = 0;
//    for (int i=0; i<2; i++) {
//        //if r == s (start) and forward tangent is left of x, it is the beginning of a leftward edge => include
//        //if r == e (B)     and backwardtangent is left of x, it is the end of a rightward edge      => include
//        //else exclude
//        //This is a numerically sensitive place especially close to the endpoints.
//        //With straight lines we use simple < > == operators to see if the endpoint is on x or not
//        //Here we have numerical imprecision.
//        //So first we see if we are close to the endpoints in an approximate manner
//        //and then if yes, then we further refine the decision on endpoint using > < operators
//        //and then we see if this is a leftward or rightward edge
//        if (test_equal(pos[i], arc->s)) {                           //OK we treat this as a crosspoint
//            if (test_arc_end(start, x, y[i]) &&                     //but if *exactly* on the startpoint...
//                arc->ell.Tangent(arc->s, arc->clockwise_arc).x > x) //...and fw tangent to the right: exclude
//                continue;
//			y[num] = start.y;
//			pos[num] = 0;
//        } else if (test_equal(pos[i], arc->e)) {                     //OK we treat this as a crosspoint
//            if (test_arc_end(end, x, y[i]) &&                        //but if *exactly* on the endpoint...
//                arc->ell.Tangent(arc->e, !arc->clockwise_arc).x > x) //...and bw tangent to the right: exclude
//                continue;
//			y[num] = end.y;
//			pos[num] = 1;
//        } else if (radianbetween(pos[i])) { //include, if not close to end but between s&e
//            y[num] = y[i];
//            pos[num] = radian2pos(pos[i]);
//        } else
//            continue; //exclude
//        forward[num] = (y[i] == std::min(y[0], y[1])) == arc->clockwise_arc;
//        num++;
//    }
//    return num;
//}
//
//
//
///** Put a dashed/dotted path into a cairo context. Needed for backends not supporting dashed lines.
// *
// * We also assume cairo dash is set to continuous: we fake dash here.
// * @param [in] cr The cairo context.
// * @param [in] pattern Contains lengths of alternating on/off segments.
// * @param [in] num contains the number of elements in pattern.
// * @param pos Shows which pattern element we shall start at (updated on return).
// * @param offset Shows at which point we shall start at within the segment selected by `pos` (updated on return).
// * @param reverse If true the path is drawn from `end` to `start`.
// */
//void Edge::PathDashed(cairo_t *cr, const double pattern[], unsigned num, int &pos, double &offset, bool reverse) const
//{
//    _ASSERT(num);
//    _ASSERT(offset<pattern[pos]);
//    if (!arc) {
//        const XY & fr = reverse ? end : start;
//        const XY & to = reverse ? start : end;
//        const double len = sqrt((to.x-fr.x)*(to.x-fr.x) + (to.y-fr.y)*(to.y-fr.y));
//        const double ddx = (to.x-fr.x)/len;
//        const double ddy = (to.y-fr.y)/len;
//        double processed = 0;
//        double x = fr.x, y = fr.y;  
//        if (offset) {
//            if (pattern[pos]-offset > len) { //remaining segment is shorter than the entire length
//                if (pos%2==0) {//start with drawn
//                    cairo_move_to(cr, fr.x, fr.y);
//                    cairo_line_to(cr, to.x, to.y);
//                }
//                offset += len;
//                return;
//            }
//            if (pos%2==0) cairo_move_to(cr, x, y);
//            x += (pattern[pos]-offset)*ddx;
//            y += (pattern[pos]-offset)*ddy;
//            if (pos%2==0) cairo_line_to(cr, x, y);
//            processed += pattern[pos]-offset;
//            pos = (pos+1)%num;
//        }
//
//        while(processed+pattern[pos] <= len) {
//            if (pos%2==0) cairo_move_to(cr, x, y);
//            x += pattern[pos]*ddx;
//            y += pattern[pos]*ddy;
//            if (pos%2==0) cairo_line_to(cr, x, y);
//            processed += pattern[pos];
//            pos = (pos+1)%num;
//        }
//
//        offset = len - processed;
//        if (pos%2==0 && fabs(offset)>1e-10) {
//            cairo_move_to(cr, x, y);
//            cairo_line_to(cr, to.x, to.y); 
//        }
//        return;
//    }
//    //curvy
//    cairo_save(cr);
//    arc->ell.TransformForDrawing(cr);
//    double local_s = arc->s, local_e = arc->e;
//    bool cl = arc->clockwise_arc;
//    if (reverse) {
//        std::swap(local_s, local_e);
//        cl = !cl;
//    }
//    if (cl) {
//        if (local_e<local_s) local_e += 2*M_PI; //ensure local_e is larger than local_s
//    } else {
//        if (local_s<local_e) local_s += 2*M_PI; //ensure local_s is larger than local_e (assume local_e and local_s between [0, 2PI])
//    }
//    //TODO Do proper ellipse arc length calculations
//    const double avg_r = (arc->ell.GetRadius1() + arc->ell.GetRadius2())/2;
//    const double len = fabs(local_s-local_e)*avg_r; 
//    const double inc_s = local_s<local_e ? 1/avg_r : -1/avg_r;
//    double processed = 0;
//    if (offset) {
//        if (pattern[pos]-offset > len) { //remaining segment is shorter than the entire length
//            offset += len;
//            if (pos%2==0) {//start with drawn
//                cairo_new_sub_path(cr);
//                if (cl) cairo_arc(cr, 0, 0, 1., local_s, local_e);
//                else cairo_arc_negative(cr, 0, 0, 1., local_s, local_e);
//                cairo_restore(cr);
//            } else
//                cairo_restore(cr);
//            return;
//        }
//        const double new_s = local_s + inc_s*(pattern[pos]-offset);
//        if (pos%2==0) {
//            cairo_new_sub_path(cr);
//            if (cl) cairo_arc(cr, 0, 0, 1., local_s, new_s);
//            else cairo_arc_negative(cr, 0, 0, 1., local_s, new_s);
//        }
//        local_s = new_s;
//        processed += pattern[pos]-offset;
//        pos = (pos+1)%num;
//    }
//
//    while (processed+pattern[pos] <= len) {
//        const double new_s = local_s + inc_s*pattern[pos];
//        if (pos%2==0) {
//            cairo_new_sub_path(cr);
//            if (cl) cairo_arc(cr, 0, 0, 1., local_s, new_s);
//            else cairo_arc_negative(cr, 0, 0, 1., local_s, new_s);
//        }
//        local_s = new_s;
//        processed += pattern[pos];
//        pos = (pos+1)%num;
//    }
//
//    offset = len - processed;
//    if (pos%2==0 && !test_zero(offset)) {
//        cairo_new_sub_path(cr);
//        if (cl) cairo_arc(cr, 0, 0, 1., local_s, local_e);
//        else cairo_arc_negative(cr, 0, 0, 1., local_s, local_e);
//    }
//    cairo_restore(cr);
//}
//
//
//
/////////////////////////////////////////////////////////////////////////////////////
//
///** Equality test for curvy edges */
//bool Edge::equal_curvy(const Edge& o) const
//{
//    _ASSERT(arc);
//    if (arc->s != o.arc->s) return false;
//    if (arc->ell != o.arc->ell) return false;
//    if (arc->clockwise_arc != o.arc->clockwise_arc) return false;
//    if (arc->type==EdgeArc::ARC && arc->e!=o.arc->e) return false;
//    return true;
//}
//
///** Comparison for curvy edges */
//bool Edge::smaller_curvy(const Edge& o) const
//{
//    if (arc->s != o.arc->s) return arc->s < o.arc->s;
//    if (arc->type == EdgeArc::ARC) {
//        if (arc->e!=o.arc->s) return arc->e < o.arc->s;
//    }
//    if (arc->clockwise_arc != o.arc->clockwise_arc) return arc->clockwise_arc;
//    return arc->ell < o.arc->ell;
//}
//
//
///** Calculates bounding box for curvy edges - a*/
//void Edge::CalculateBoundingBoxCurvy()
//{
//    arc->boundingBox.MakeInvalid(); 
//    arc->boundingBox += start; 
//    arc->boundingBox += end; 
//    XY xy;
//    if (arc->type==EdgeArc::FULL_CIRCLE) {
//        //include the extreme points
//        for (int i=0; i<4; i++) {
//            arc->ell.GetExtreme(i, xy);
//            arc->boundingBox += xy;
//        }
//    } else {
//        //if any extreme point is included in the arc add them
//        for (int i=0; i<4; i++)
//            if (radianbetween(arc->ell.GetExtreme(i, xy)))
//                arc->boundingBox += xy;
//    }
//}
//
///** Draws cairo arc for curvy edges */
//void Edge::pathto_curvy(cairo_t *cr) const
//{
//    cairo_save(cr);
//    arc->ell.TransformForDrawing(cr);
//    if (arc->clockwise_arc)
//        cairo_arc(cr, 0, 0, 1, arc->s, arc->type==EdgeArc::ARC ? arc->e : arc->s + 2*M_PI);
//    else
//        cairo_arc_negative(cr, 0, 0, 1, arc->s, arc->type==EdgeArc::ARC ? arc->e : arc->s - 2*M_PI);
//    cairo_restore(cr);
//}
//
//
/////////////////////////////////////////////////////////////////////////////////////
//
//
///** Return true if the point represented by radian `r` is on the arc (between `s` and `e`). */
//bool Edge::radianbetween(double r) const
//{
//    _ASSERT(arc);
//    if (arc->type==EdgeArc::FULL_CIRCLE) return true;
//    r = fmod_negative_safe(r, 2*M_PI);
//    if (arc->clockwise_arc) {
//        if (arc->s < arc->e) return arc->s <= r && r <= arc->e;
//        else return r >= arc->s || r <= arc->e;
//    } else {
//        if (arc->s < arc->e) return arc->s >= r || r >= arc->e;
//        else return arc->s >= r && r >= arc->e;
//    }
//}
//
///** Convert from pos [0..1] to actual radian. Guaranteed between [0..2pi) */
//double Edge::pos2radian(double r) const
//{
//    if (test_zero(r)) return arc->s;
//    if (test_equal(r,1)) return arc->e;
//    _ASSERT(arc);
//    _ASSERT(0<=r && r<=1);
//    if (arc->type==EdgeArc::FULL_CIRCLE) {
//        double a = arc->clockwise_arc ? arc->s + r*2*M_PI : arc->s - r*2*M_PI ;
//        if (a>=2*M_PI) a -= 2*M_PI;
//        if (a<0) a += 2*M_PI;
//        _ASSERT(0<=a && a<2*M_PI);
//        return a;
//    }
//    double ret;
//    if (arc->clockwise_arc) {
//        if (arc->s < arc->e) ret = arc->s + (arc->e - arc->s)*r;
//        else                 ret = arc->s + (arc->e - arc->s + 2*M_PI)*r;
//    } else {
//        if (arc->s < arc->e) ret = arc->e + (arc->s - arc->e + 2*M_PI)*(1-r);
//        else                 ret = arc->e + (arc->s - arc->e)*(1-r);
//    }
//    if (ret-2*M_PI>0) ret = ret-2*M_PI;
//    _ASSERT(0<=ret && ret<=2*M_PI);
//    return ret;
//}
//
///** Convert from radian to the pos value. Can return outside [0..1] if point is not between `s` and `e`. */
//double Edge::radian2pos(double r) const
//{
//    _ASSERT(arc);
//    r = fmod_negative_safe(r, 2*M_PI);
//    if (arc->type==EdgeArc::FULL_CIRCLE) {
//        if (arc->clockwise_arc)
//            return arc->s <= r ? (r - arc->s)/(2*M_PI) : (r - arc->s)/(2*M_PI)+1;
//        else
//            return arc->s >= r ? (arc->s - r)/(2*M_PI) : (arc->s - r)/(2*M_PI)+1;
//    }
//    if (test_equal(arc->s, arc->e)) return 0;
//    if (arc->clockwise_arc) {
//        //here r-s can be <0 or bigger than span,
//        //so returned pos can be outside [0..1]
//        if (arc->s < arc->e) return (r - arc->s)/(arc->e - arc->s);
//        //ok, e<s
//        if (r >= arc->s) return (r - arc->s)/(arc->e - arc->s+2*M_PI);
//        return (r - arc->s + 2*M_PI)/(arc->e - arc->s + 2*M_PI);
//    }
//    if (arc->s > arc->e) return (r - arc->s)/(arc->e - arc->s);
//    if (r >= arc->e) return (arc->s - r + 2*M_PI)/(arc->s - arc->e + 2*M_PI);
//    return (arc->s - r)/(arc->s - arc->e + 2*M_PI);
//}
//
//
//double Edge::getAreaAboveAdjusted_curvy() const
//{
//    _ASSERT(arc);
//    //For a full circle we return the (signed) area enclosed by the circle times 2.
//    if (arc->type==EdgeArc::FULL_CIRCLE) return arc->ell.FullArea()*(arc->clockwise_arc ? 2 : -2);
//    //
//    // =======>   ======>   ===========>   The first figure shows a clockwise arc (all
//    //      ++       --                    positive areas). The second shows the same,
//    //    =>++     ===>      ^=======o     but since we start from a smaller X and end on
//    //   /++++    /++oo       \::::://     a larger, the difference compared to a straight
//    //   \+++/    \++o         \::://      edge is is cancelled out (indcated by "o" zero)
//    //    ===      --           ===        and the area below is negative.
//    //                                     The third figure shows the center of the circle.
//    // Denote "S" and "D" the start and endpoint of the arc, resp; while "O" the center.
//    // To calculate this, we have three areas: the sector area of the arc, the area
//    // above the D->O and the O->S straight lines. If we treat all of these signed
//    // (if the sector is clockwise have it positive, and vice versa), we get the
//    // (signed) area eaxctly above the arc. Thus
//    // AreaAboveArc = SignedSectorArea + AreaAbove(D->O) + AreaAbove(O->S) =
//    //   SignedSectorArea + (DX-OX)*(DY+OY)/2 + (OX-SX)*(OY+SY)/2 =
//    //   SSA + (DX*DY - OX*OY)/2 + (DX*OY - OX*DY)/2 + (OX*OY - SX*SY)/2 + (OX*SY - SX*OY)/2 =
//    //   SSA + (DX*DY - SX*SY)/2 + (DX-SX)*OY/2 - (DY-SY)*OX/2.
//    // Since we need ret = AreaAboveArc*2 - (DX*DY - SX*SY)/2 =
//    //   SSA*2 + (DX-SX)*OY - (DY-SY)*OX.
//    //Note that SectorArea() always assume clockwise and returns a positive value.
//    return 2*(arc->clockwise_arc ? arc->ell.SectorArea(arc->s, arc->e) : -arc->ell.SectorArea(arc->e, arc->s)) +
//           - (end.x - start.x)*arc->ell.GetCenter().y + (end.y-start.y)*arc->ell.GetCenter().x;
//}
//
///** Returns true if the edge is OK */
//bool Edge::IsSane() const
//{
//    if (!IsSaneNoBoundingBox()) return false;
//    if (arc && 
//        (arc->boundingBox.IsWithin(start) == WI_OUTSIDE ||
//         arc->boundingBox.IsWithin(end) == WI_OUTSIDE)) return false;
//    return true;
//}
//
///** Returns if the edge is OK, not considering the value in `boundinBox`. */
//bool Edge::IsSaneNoBoundingBox() const
//{
//    if (arc) {
//        if (arc->type == EdgeArc::ARC)
//            if (!arc->ell.Radian2Point(arc->e).test_equal(end)) return false;
//        if (arc->type == EdgeArc::FULL_CIRCLE)
//            if (!end.test_equal(start)) return false;
//        if (!arc->ell.Radian2Point(arc->s).test_equal(start)) return false;
//    }
//    return true;
//}
//
//
///** Helper for Angle() for curvy edges */
//RayAngle Edge::angle_curvy(bool incoming, const XY &p, double pos) const
//{
//    _ASSERT(!incoming || (pos!=0));
//    _ASSERT( incoming || (pos!=1));
//    _ASSERT(arc);
//    const XY xy = arc->ell.Tangent(pos2radian(pos), incoming ^ arc->clockwise_arc);
//    RayAngle ret;
//    ret.angle = angle(p, XY(p.x+100, p.y), xy);
//    if (test_equal(ret.angle, 4)) ret.angle = 0;
//    switch (triangle_dir(p, xy, arc->ell.GetCenter())) {
//        case CLOCKWISE:
//            ret.curve = +1; break;
//        case COUNTERCLOCKWISE:
//            ret.curve = -1; break;
//        default:
//            _ASSERT(0);
//    }
//    ret.curve /= (arc->ell.GetRadius1() + arc->ell.GetRadius2())/2; //TODO it right: use actual radius for ellipses
//    return ret;
//};
//
///** Helper: The radian difference between two radians */
//inline double radian_diff_abs(double a, double b)
//{
//    _ASSERT(std::min(fabs(a-b), fabs(a>b ? 2*M_PI-(a-b) : 2*M_PI-(b-a)))<2*M_PI);
//    return std::min(fabs(a-b), fabs(a>b ? 2*M_PI-(a-b) : 2*M_PI-(b-a)));
//}
//
//
///** Helper: `pos` lists a series of pos values. We find the one that is closest to
// * `rad`, which is in radians. We return the radian. */
//double Edge::FindRadianOfClosestPos(unsigned num, double pos[], double rad)
//{
//    _ASSERT(arc);
//    double ret=-1, diff=10;
//    for (unsigned i=0; i<num; i++) {
//        const double radian = pos2radian(pos[i]);
//        const double d = radian_diff_abs(rad, radian);
//        if (d < diff) {
//            diff = d;
//            ret = radian;
//        }
//    }
//    return ret;
//}
//
///** Helper: Flip `clockwise` if `new_s` and `new_e` would be an arc of opposite 
// * clockwisedness. Return true if we did so. */
//bool Edge::UpdateClockWise(double new_s, double new_e)
//{
//    //check which is the smallest diff: s<->new_s plus e<->new_e or the other
//    if (radian_diff_abs(arc->s, new_s) + radian_diff_abs(arc->e, new_e) <=
//        radian_diff_abs(arc->e, new_s) + radian_diff_abs(arc->s, new_e))
//        return false;
//    arc->clockwise_arc = !arc->clockwise_arc;
//    return true;
//}
//
///** Helper for SwapXY */
//void Edge::SwapXYcurvy()
//{
//    _ASSERT(arc);
//    arc->ell.SwapXY();
//    arc->clockwise_arc = !arc->clockwise_arc;
//    if (arc->ell.IsTilted()) {
//        arc->s = arc->s == 0 ? 0 : 2*M_PI - arc->s;
//        arc->e = arc->e == 0 ? 0 : 2*M_PI - arc->e;
//    } else {
//        arc->s = arc->s < M_PI/2 ? M_PI/2 - arc->s : 2.5*M_PI - arc->s;
//        arc->e = arc->e < M_PI/2 ? M_PI/2 - arc->e : 2.5*M_PI - arc->e;
//    }
//};
//
//
///** Return the radian at the middle of the arc */
//double Edge::GetRadianMidPoint() const
//{
//    _ASSERT(arc);
//    if (arc->type==EdgeArc::FULL_CIRCLE) return M_PI;
//    if (arc->e == arc->s) return arc->s;
//    if (arc->clockwise_arc == (arc->s < arc->e)) return (arc->s + arc->e)/2;
//    return fmod_negative_safe((arc->s + arc->e)/2+M_PI, 2*M_PI);
//}
//
///** Return the point on the edge corresponding to `pos`. */
//XY Edge::Pos2Point(double pos) const
//{
//    _ASSERT(pos>=-0 && pos<=1);
//    if (!arc) return start + (end-start)*pos;
//    switch (arc->type) {
//    case EdgeArc::FULL_CIRCLE:
//        return arc->ell.Radian2Point(arc->s + pos*(arc->clockwise_arc ? 2*M_PI : -2*M_PI));
//    case EdgeArc::ARC:
//        return arc->ell.Radian2Point(pos2radian(pos));
//    case EdgeArc::STRAIGHT: break; //should not happen
//    }
//    _ASSERT(0);
//    return XY();
//}
//
//
//
///** Removes the part of the edge before point `p`. Assumes p lies on us. */
//Edge& Edge::SetStartStrict(const XY &p, double pos, bool keep_full_circle)
//{
//    start = p;
//    if (!arc) return *this;
//    const double r = pos2radian(pos);
//    _ASSERT(radianbetween(r));
//    _ASSERT(arc->ell.Radian2Point(r).test_equal(p));
//    if (arc->type==EdgeArc::FULL_CIRCLE && !(test_equal(arc->s, r) && keep_full_circle)) {
//        arc->e = arc->s;
//        arc->type = EdgeArc::ARC;
//    }
//    arc->s = r;
//    CalculateBoundingBoxCurvy();
//    return *this;
//}
//
///** Removes the part of the edge before point p. Does not assumes p lies on us.
// *
// * This can be used to extend an edge.
// * We do not check if an arc wraps or not!!!
// */
//Edge& Edge::SetStartLiberal(const XY &p, bool keep_full_circle)
//{
//    start = p;
//    if (!arc) return *this;
//    const double r = arc->ell.Point2Radian(p);
//    if (arc->type==EdgeArc::FULL_CIRCLE && !(test_equal(arc->s, r) && keep_full_circle)) {
//        arc->e = arc->s;
//        arc->type = EdgeArc::ARC;
//    }
//    arc->s = r;
//    CalculateBoundingBoxCurvy();
//    return *this;
//}
//
//
///** Checks if this edge and `next` can be combined into a single edge.
// * If so, also update "this" to be the combined edge & return true. */
//bool Edge::CheckAndCombine(const Edge &next)
//{
//    //if next is a degenerate (zero length) arc, we just skip that
//    if (next.start.test_equal(next.end) && (!next.arc || next.arc->type != EdgeArc::FULL_CIRCLE))
//        return true;
//    if ((arc==NULL) != (next.arc==NULL)) return false;
//    if (!arc) {
//        const double a = angle(start, end, next.end);
//        if (test_zero(a) || test_equal(a,2)) {
//            end = next.end;
//            return true;
//        }
//        return false;
//    }
//    //OK, now we are both curvy edges
//    if (arc->ell != next.arc->ell) return false;
//    //OK, we are the same ellipse
//    //if any of us is full circle, keep only the other one
//    //if both of us is a full circle, we must equal (since next.start==end), keep any
//    if (next.arc->type==EdgeArc::FULL_CIRCLE)
//        return true; //we keep what we are
//    if (arc->type==EdgeArc::FULL_CIRCLE) {
//        //start and s remain (must be equal to next.start)
//        if (!test_equal(arc->s, next.arc->s) || !start.test_equal(next.start))
//            return false;
//        arc->clockwise_arc = next.arc->clockwise_arc;
//        arc->e = next.arc->e;
//        end = next.end;
//        arc->type = next.arc->type; //next cannot be full circle
//        arc->boundingBox = next.arc->boundingBox;
//        return true;
//    }
//    //Now none of us is a full circle
//    if (arc->clockwise_arc == next.arc->clockwise_arc) {
//        //full circle, if next.e == s and we cover the whole circumference
//        if (test_equal(next.arc->e, arc->s)) arc->type = EdgeArc::FULL_CIRCLE;
//    } else {
//        //opposite dir. never full circle
//        //direction changes if new endpoint outside our old span
//        if (!radianbetween(next.arc->e)) 
//            arc->clockwise_arc = !arc->clockwise_arc;
//    }
//    arc->e = next.arc->e; //we may end up as degenerate (s==e) but that is life
//    end = next.end;
//    return true;
//}
//
//XY Edge::getcentroidareaaboveupscaled_straight(const XY&start, const XY&end)
//{
//    const XY &minY = start.y < end.y ? start : end;
//    const XY &maxY = start.y < end.y ? end : start;
//    //calculate the centroid of the rectangle and triangle below a (slanted) edge
//    //multiplied by their area.
//    //We do it in two steps: centroid by the Y coordinate of the area
//    //then multiply by the (common) x coordinate of the area
//    XY ret = XY(start.x+end.x, minY.y)*(minY.y/2) +                     //centroid of rectange times y coord of its area
//             XY(maxY.x*2+minY.x, maxY.y+minY.y*2)*((maxY.y-minY.y)/6);  //centroid of triangle times y coord of its area
//    return ret*(start.x - end.x); //this makes ret negative if edge is from right to left (as it should)
//}
//
//XY Edge::getcentroidareaaboveupscaled_curvy() const
//{
//    XY ret = arc->clockwise_arc ? arc->ell.SectorCentroidTimesArea(arc->s, arc->e) : -arc->ell.SectorCentroidTimesArea(arc->e, arc->s);
//    ret += getcentroidareaaboveupscaled_straight(start, arc->ell.GetCenter());
//    ret += getcentroidareaaboveupscaled_straight(arc->ell.GetCenter(), end);
//    return ret;
//}
//
///** Expands the edge.
// * 
// * This takes the direction of the edge to determine the 'outside' side of the 
// * edge to expand towards (or away from in case of a negative `gap`).
// * Destroys bounding box!
// * @param [in] gap The amount to expand (or shrink if <0)
// * @returns False if a curvy edge degenerates to a line or disappears due to shrinkage.
// */
//bool Edge::Expand(double gap)
//{
//	if (arc) {
//        const EllipseData old_ell(arc->ell);
//		switch (arc->ell.Expand(arc->clockwise_arc ? gap : -gap)) {
//        case 1: //ell remained an ellipse
//            start = arc->ell.Radian2Point(arc->s);
//            if (arc->type==EdgeArc::ARC) end = arc->ell.Radian2Point(arc->e);
//            else end = start; //full circle
//            return true;
//        case 0:
//        case -1: //completely disappeared
//            return false;
//        //case 0:  //degenerates to line
//        //    if (type==FULL_CIRCLE)
//        //        return false; //start==end here, we do not return such a straight edge
//        //    //if ellipse becomes too small, fall through to below and replace it with a straight line
//        //    //Now decrease just up to the point od getting degenerate
//        //    const double rad = std::min(old_ell.GetRadius1(), old_ell.GetRadius2());
//        //    _ASSERT(rad<fabs(gap));
//        //    ell = old_ell;
//        //    ell.Expand(gap>0 ? rad : -rad);
//        //    start = ell.Radian2Point(s);
//        //    end = ell.Radian2Point(e);
//        //    //and then move remaining
//        //    gap = (gap>0) ? gap-rad : gap+rad;
//        //    type = STRAIGHT;
//        }
//	}
//    const double length = start.Distance(end);
//    const XY wh = (end-start).Rotate90CCW()/length*gap;
//    start += wh;
//    end += wh;
//    return true;
//}
//
///** Helper: Fistance of two radians between -pi and +pi.
// * true that diff+b == a(+-2pi) */
//double rad_dist(double a, double b)
//{
//    _ASSERT(a>=0 && a<=2*M_PI && b>=0 && b<=2*M_PI);
//    if (a-b <= -M_PI) return a-b + 2*M_PI;
//    if (a-b >   M_PI) return a-b - 2*M_PI;
//    return a-b;
//}
//
///** Helper: Find closest point to a radian after expansion.
// *
// * We find among the 'r' values the one that is closest (in circle distance) to `p`
// * 
// * @param [in] r The radians of the crosspointss of the expanded edges.
// * @param [in] num The number of crosspoints in `r`.
// * @param [in] p The original radian of the crosspoint.
// * @param [in] larger If true then in case of a tie, we pick the larger radian value.
// * @return The index of the crosspoint picked.
// */
//int find_closest(int num, const double r[], double p, bool larger)
//{
//    if (num==0) return -1;
//	int pos = 0;
//	double diff= fabs(rad_dist(r[0], p));
//	for (int i=1; i<num; i++) {
//        const double dist = rad_dist(r[i], p);
//        if (test_smaller(diff, fabs(dist))) continue;
//        if (!test_smaller(fabs(dist), diff))
//            if (larger == (rad_dist(r[pos], p) >= diff)) continue;
//        diff = fabs(dist);
//        pos = i;
//	}
//	return pos;
//}
//
///** Find the new crosspoint of two originally neighbouring expanded edges.
// *
// * `this` and `M` are two expanded edges.
// * The whole logic of this function assumes EXPAND_MITER, but this is called for all types of
// * expansions to see how two edges relate.
// * return value is
// * DEGENERATE: One of the edges have start==end
// *  (`newcp` is undefined on return)
// * SAME_ELLIPSIS: the two edges are two arc of the same ellipses 
// *  (`newcp` is undefined on return)
// * CP_REAL: The two edges actually cross (between "start" and "end")
// *  (we return their crosspoint in `newcp`)
// * CP_EXTENDED: The two edges do not meet but their extension does. Here extension means
// * the continuation of the curve for curvy edges.
// *  (we return their crosspoint in `newcp`)
// * NO_CP_CONVERGE: Not even the extension of the two edges meet, but a linear extension does.
// * This can happen if one (or both) is curvy and appending a tangent to the edge do cross the other
// * edge, but the continuation of the curve does not.
// *   (`newcp` is the convergence point: can be reached by adding linear segments)
// * NO_CP_PARALLEL: Either the two edges are parallel or their linear extension is such.
// *   (`newcp` is a calculated convergence point: can be reached by adding linear segments)
// *
// * @param [in] M The other expanded edge.
// * @param [out] newcp Returns their new crosspoint.
// * @returns The relation of the two expanded edges.
// */
//Edge::EExpandCPType Edge::FindExpandedEdgesCP(const Edge&M, XY &newcp) const
//{
//    if (start.test_equal(end) || M.start.test_equal(M.end)) return DEGENERATE;
//    const double parallel_join_multipiler = 5;
//    if (!M.arc) {
//        if (!arc) {
//            switch (crossing_line_line(start, end, M.start, M.end, newcp)) {
//            default: _ASSERT(0);
//            case LINE_CROSSING_PARALLEL:
//                if (end.test_equal(M.start)) {
//                    newcp = end;
//                    return CP_REAL;
//                }
//                return NO_CP_PARALLEL;
//            case LINE_CROSSING_OUTSIDE: return CP_EXTENDED;
//            case LINE_CROSSING_INSIDE: return CP_REAL;
//            }
//        } else {
//            XY r[8];
//            double radian_us[8], pos_M[8];
//            int num = arc->ell.CrossingStraight(M.start, M.end, r, radian_us, pos_M);
//            if (!num) {
//                const XY tangent = NextTangentPoint(1.0);
//                if (crossing_line_line(end, tangent, M.start, M.end, newcp) != LINE_CROSSING_PARALLEL)
//                    return NO_CP_CONVERGE;
//                //if we are parallel, calculate a conv point
//                const double dist = end.Distance(M.start) * parallel_join_multipiler;
//                const double mlen = M.end.Distance(M.start);
//                newcp = M.start + (M.start-M.end)/mlen*dist;
//                return NO_CP_PARALLEL;
//            } else {
//                int pos = find_closest(num, radian_us, arc->e, !arc->clockwise_arc);
//                newcp = r[pos];
//                _ASSERT(newcp.x>-1000000 && newcp.x<10000000);
//                if (radianbetween(radian_us[pos]) || between01_approximate_inclusive(pos_M[pos]))
//                    return CP_REAL;
//                return CP_EXTENDED;
//            }
//        }
//    } else {
//        XY r[8];
//        if (!arc) {
//            double pos_us[8], radian_M[8];
//            int num = M.arc->ell.CrossingStraight(start, end, r, radian_M, pos_us);
//            if (!num) {
//                const XY tangent = M.PrevTangentPoint(0.0);
//                if (crossing_line_line(tangent, M.start, start, end, newcp) != LINE_CROSSING_PARALLEL)
//                    return NO_CP_CONVERGE;
//                //if we are parallel, calculate a conv point
//                const double dist = end.Distance(M.start) * parallel_join_multipiler;
//                const double len = end.Distance(start);
//                newcp = end + (end-start)/len*dist;
//                return NO_CP_PARALLEL;
//            }
//            int pos = find_closest(num, radian_M, M.arc->s, M.arc->clockwise_arc);
//            newcp = r[pos];
//            _ASSERT(newcp.x>-1000000 && newcp.x<10000000);
//            if (M.radianbetween(radian_M[pos]) || between01_approximate_inclusive(pos_us[pos]))
//                return CP_REAL;
//            return CP_EXTENDED;
//        } else {
//            if (arc->ell == M.arc->ell) return SAME_ELLIPSIS; //we combine two segments of the same ellipse - all must be OK
//            double radian_us[8], radian_M[8];
//            int num = arc->ell.CrossingEllipse(M.arc->ell, r, radian_us, radian_M);
//            if (!num) {
//                const XY tangent1 = PrevTangentPoint(1.0);
//                const XY tangent2 = M.NextTangentPoint(0.0);
//                if (crossing_line_line(tangent1, end, tangent2, M.start, newcp) != LINE_CROSSING_PARALLEL)
//                    return NO_CP_CONVERGE;
//                //if we are parallel, calculate a conv point
//                const double dist = end.Distance(M.start) * parallel_join_multipiler;
//                const double len = end.Distance(tangent1);
//                newcp = (end+M.start)/2 + (end-tangent1)/len*dist;
//                return NO_CP_PARALLEL;
//            }
//            int pos = find_closest(num, radian_M, M.arc->s, M.arc->clockwise_arc);
//            newcp = r[pos];
//            _ASSERT(newcp.x>-1000000 && newcp.x<10000000);
//            if (M.radianbetween(radian_M[pos]) || radianbetween(radian_us[pos]))
//                return CP_REAL;
//            return CP_EXTENDED;
//        }
//    }
//}
//
///** Expand helper: Set the startpoint and endpoint to 'S' and 'E' 
// *
// * We assume S and E are on the edge (or an extension)
// */
//void Edge::SetStartEndForExpand(const XY &S, const XY &E)
//{
//    double new_s, new_e;
//    if (arc) {
//        new_s = arc->ell.Point2Radian(S);
//        if (S.test_equal(E))
//            new_e = new_s;
//        else
//            new_e = arc->ell.Point2Radian(E);
//        switch (arc->type) {
//        case EdgeArc::STRAIGHT: _ASSERT(0); break; //should not happen if 'arc' is not NULL
//        case EdgeArc::ARC:
//            //check if direction has changed
//            //start with an exception: if any is exactly 180 degrees, we assume same direction
//            //(thus we test the rest only if none is exactly 180 degrees)
//            //if (fabs(s-e) != M_PI && fabs(new_s-new_e) != M_PI) {
//            //    const bool dir_us = (e>s && (e-s)<M_PI) || (s>e && (s-e)>=M_PI);
//            //    const bool dir_M = (new_e>new_s && (new_e-new_s)<M_PI) || (new_s>new_e && (new_s-new_e)>=M_PI);
//            //    if (dir_us != dir_M)
//            //        clockwise_arc = !clockwise_arc;
//            //}
//            if (radian_diff_abs(arc->e, new_e) + radian_diff_abs(arc->s, new_s) >
//                radian_diff_abs(arc->e, new_s) + radian_diff_abs(arc->s, new_e))
//                arc->clockwise_arc = !arc->clockwise_arc;
//            /* Falltherough */
//        case EdgeArc::FULL_CIRCLE:
//            //for full circle we assume dir has not changed
//            arc->s = new_s;
//            arc->e = new_e;
//            arc->type = S.test_equal(E) ? EdgeArc::FULL_CIRCLE : EdgeArc::ARC;
//        }
//    }
//    start = S;
//    end = E;
//}
//
///** Expand helper: Check if `S`->`E` is an opposite direction arc than us now. */
//bool Edge::IsOpposite(const XY &S, const XY &E) const
//{
//	if (start.test_equal(end)) return false;
//	if (S.test_equal(E)) return false;
//	if (!arc) {
//		if (fabs(start.x-end.x) > fabs(start.y-end.y))
//			return (start.x<end.x) != (S.x<E.x);
//		else
//			return (start.y<end.y) != (S.y<E.y);
//	}
//    const double new_s = arc->ell.Point2Radian(S);
//    const double new_e = arc->ell.Point2Radian(E);
//    return radian_diff_abs(arc->e, new_e) + radian_diff_abs(arc->s, new_s) >
//           radian_diff_abs(arc->e, new_s) + radian_diff_abs(arc->s, new_e);
//}
//
///** Helper for CreateExpand2D for curvy edges */
//void Edge::CreateExpand2DCurvy(const XY &gap, std::vector<Edge> &ret, int &stype, int &etype) const
//{
//    _ASSERT(arc);
//    unsigned index = ret.size();
//    const unsigned original_index = index;
//    const double ee = GetRadianE();
//    ret.push_back(Edge());
//    ret[index].arc = new EdgeArc;
//    ret[index].arc->type = EdgeArc::ARC;
//    ret[index].arc->clockwise_arc = arc->clockwise_arc;
//    ret[index].start = start;
//    ret[index].arc->ell = arc->ell;
//    ret[index].arc->s = arc->s;
//    XY extreme;
//    double current = arc->s;
//    while (1) {
//        const double next = arc->ell.FindExtreme(current, arc->clockwise_arc, extreme);
//        //if we have moved over "ee" stop.
//        if (!(arc->type==EdgeArc::FULL_CIRCLE && current==arc->s) &&  //exclude the first segment of full circles
//            fmod_negative_safe(arc->clockwise_arc ? next-current : current-next, 2*M_PI) >=
//            fmod_negative_safe(arc->clockwise_arc ? ee-current : current-ee, 2*M_PI)) break;
//        current = next;
//        ret.resize(index+3);
//        index+=2;
//        ret[index].arc = new EdgeArc;
//        ret[index].arc->type = EdgeArc::ARC;
//        ret[index].arc->clockwise_arc = arc->clockwise_arc;
//        ret[index].start = ret[index-2].end = extreme;
//        ret[index].arc->s     = ret[index-2].arc->e   = fmod(current, 2*M_PI);
//        ret[index].arc->ell = arc->ell;
//    }
//    ret[index].arc->e = ee;
//    ret[index].end = end;
//    stype = Edge_CreateExpand2D::comp_int(ret[original_index].start.x, ret[original_index].end.x) +
//            Edge_CreateExpand2D::comp_int(ret[original_index].start.y, ret[original_index].end.y)*3;
//    for (unsigned u=original_index; u<=index; u+=2) {
//        if (ret[u].arc) ret[u].CalculateBoundingBoxCurvy();
//        ret[u].Shift(XY(Edge_CreateExpand2D::comp_dbl(ret[u].end.y, ret[u].start.y, gap.x),
//                        Edge_CreateExpand2D::comp_dbl(ret[u].start.x, ret[u].end.x, gap.y)));
//    }
//    if (index==original_index) {
//        etype=stype;
//        return;
//    }
//    for (unsigned u=original_index+1; u<index; u+=2) {
//        ret[u].start = ret[u-1].end;
//        ret[u].end = ret[u+1].start;
//    }
//    etype = Edge_CreateExpand2D::comp_int(ret[index].start.x, ret[index].end.x) +
//            Edge_CreateExpand2D::comp_int(ret[index].start.y, ret[index].end.y)*3;
//}
//
//
///** Helper: Radian `r` is between `s` and `e`? */
//inline bool radbw(double r, double s, double e)
//{
//	if (s<e) return s<=r && r<=e;
//    else return r>=s || r<=e;
//}
//
///** Defines how long segments we break an arc to when testing OffsetBelow */
//#define CURVY_OFFSET_BELOW_GRANULARIRY 5
//
///** Helper to OffsetBelow for curvy edges */
//double Edge::offsetbelow_curvy_straight(const Edge &M, bool straight_is_up, double &touchpoint) const
//{
//    _ASSERT(arc);
//    _ASSERT(!M.arc);
//    double rad = CURVY_OFFSET_BELOW_GRANULARIRY*2/(arc->ell.GetRadius1() + arc->ell.GetRadius2());
//    double end, beg;
//    if (arc->type==EdgeArc::FULL_CIRCLE) {
//        beg = 0;
//        end = 2*M_PI;
//    } else if (arc->clockwise_arc) {
//        beg = arc->s;
//        end = arc->s < arc->e ? arc->e : arc->e + 2*M_PI;
//    } else {
//        beg = arc->e;
//        end = arc->s > arc->e ? arc->s : arc->s + 2*M_PI;
//    }
//    Edge e(arc->ell.Radian2Point(beg), XY(0,0));
//    double ret = CONTOUR_INFINITY;
//    for (double r = beg+rad; r<end; r+=rad) {
//        e.end = arc->ell.Radian2Point(r);
//        double tp, off;
//        if (straight_is_up)
//            off = M.OffsetBelow(e, tp);
//        else
//            off = e.OffsetBelow(M, tp);
//        if (off < ret) {
//            ret = off;
//            touchpoint = tp;
//        }
//        e.start = e.end;
//    }
//    return ret;
//}
//
///** Determine the relative vertical position of two edges. 
// *
// * Assuming we are higher than `o` (lower y coordinates), how much can `o`
// * be shifted upwards before it collides with us.
// * If `o` and us do not share any poins with the same x coordinate, CONTOUR_INFINITY is returned
// * (`o` can pass infinitely up besides us without collision).
// * If `o` and us share such points, but `o` is above us, we return a negative value, since
// * `o` needs to be pulled down to be below us. This is normal operation.
// * @param [in] o The other edge below us.
// * @param [out] touchpoint After moving `o` up, we return the point on us where we touched.
// * @return How much `o` was moved up before it hit us. Negative if `o` must be moved down to be below us.
// */
//double Edge::OffsetBelow(const Edge &o, double &touchpoint) const
//{
//    if (!o.arc) {
//        if (arc)
//            return offsetbelow_curvy_straight(o, false, touchpoint);
//        //calc for two straight edges
//        const double minAB = std::min(start.x, end.x);
//        const double maxAB = std::max(start.x, end.x);
//        const double minMN = std::min(o.GetStart().x, o.GetEnd().x);
//        const double maxMN = std::max(o.GetStart().x, o.GetEnd().x);
//        if (minAB > maxMN || minMN > maxAB) return CONTOUR_INFINITY;
//        const double x1 = std::max(minAB, minMN);
//        const double x2 = std::min(maxAB, maxMN);
//        if (start.x == end.x) {
//            touchpoint = std::max(start.y, end.y);
//            if (o.GetStart().x == o.GetEnd().x)
//                return std::min(o.GetStart().y, o.GetEnd().y) - std::max(start.y, end.y); //here all x coordinates must be the same
//            return (o.GetEnd().y-o.GetStart().y)/(o.GetEnd().x-o.GetStart().x)*(start.x-o.GetStart().x) + o.GetStart().y -
//                   std::max(start.y, end.y);
//        }
//        if (o.GetStart().x == o.GetEnd().x) {
//            touchpoint = (start.y-end.y)/(start.x-end.x)*(o.GetStart().x-start.x) + start.y;
//            return std::min(o.GetStart().y,o.GetEnd().y) - touchpoint;
//        }
//        const double y1 = ((start.y-end.y)/(start.x-end.x)*(x1-start.x) + start.y);
//        const double y2 = ((start.y-end.y)/(start.x-end.x)*(x2-start.x) + start.y);
//        const double diff1 = (o.GetEnd().y-o.GetStart().y)/(o.GetEnd().x-o.GetStart().x)*(x1-o.GetStart().x) + o.GetStart().y - y1;
//        const double diff2 = (o.GetEnd().y-o.GetStart().y)/(o.GetEnd().x-o.GetStart().x)*(x2-o.GetStart().x) + o.GetStart().y - y2;
//        if (diff1<diff2) {
//            touchpoint = y1;
//            return diff1;
//        }
//        touchpoint = y2;
//        return diff2;
//    }
//    if (!arc)
//        return o.offsetbelow_curvy_straight(*this, true, touchpoint);
//    //now both of us are curves
//    const double rad1 = CURVY_OFFSET_BELOW_GRANULARIRY*2/(arc->ell.GetRadius1() + arc->ell.GetRadius2());
//    const double rad2 = CURVY_OFFSET_BELOW_GRANULARIRY*2/(o.arc->ell.GetRadius1() + o.arc->ell.GetRadius2());
//    //TODO: Fix this for full_circle==true, too!!
//    const double end1 = arc->s < arc->e ? arc->e : arc->e + 2*M_PI;
//    const double end2 = o.arc->s < o.arc->e ? o.arc->e : o.arc->e + 2*M_PI;
//    double ret = CONTOUR_INFINITY;
//    Edge e1(start, XY(0,0));
//    for (double r1 = arc->s + rad1; r1<end1; r1+=rad1) {
//        e1.end = arc->ell.Radian2Point(r1);
//        Edge e2(o.start, XY(0,0));
//        for (double r2 = o.arc->s + rad2; r2<end2; r2+=rad2) {
//            e2.end = o.arc->ell.Radian2Point(r2);
//            double tp, off;
//            off = e1.OffsetBelow(e2, tp);
//            if (off < ret) {
//                ret = off;
//                touchpoint = tp;
//            }
//            e2.start = e2.end;
//        }
//        e1.start = e1.end;
//    }
//    return ret;
//}
//
//
///** Calculates the touchpoint of tangents drawn to touch two edges.
// * 
// * Given the two edges, four such tangents can be drawn, here we focus on the two 
// * outer ones, the ones that touch either both edges from above or both from below,
// * but not mixed.
// * @param [in] o The other edge.
// * @param [out] clockwise The points where the clockwise tangent touches us
// *                        (clockwise[0]) and `from` (clockwise[1]).
// * @param [out] cclockwise The points where the counterclockwise tangent touches us
// *                         (cclockwise[0]) and `from` (cclockwise[1]).
// * @returns True if success, false if `o` is inside us.
// */
//bool Edge::TangentFrom(const Edge &o, XY clockwise[2], XY cclockwise[2]) const
//{
//    XY c, cc;
//    clockwise[1]  = minmax_clockwise(clockwise[0], o.start, clockwise[1], true);
//    cclockwise[1] = minmax_clockwise(cclockwise[0], o.start, cclockwise[1], false);
//
//    clockwise[0]  = minmax_clockwise(clockwise[1], start, clockwise[0], false);
//    cclockwise[0] = minmax_clockwise(cclockwise[1], start, cclockwise[0], true);
//
//    //clockwise[1]  = minmax_clockwise(clockwise[0], o.start, clockwise[1], true);
//    //cclockwise[1] = minmax_clockwise(cclockwise[0], o.start, cclockwise[1], false);
//
//    if (!arc) {
//        if (o.arc) {
//            if (o.TangentFrom(start, c, cc)) {
//                if (o.radianbetween(o.arc->ell.Point2Radian(c)))
//                    clockwise[1]  = minmax_clockwise(start, c, clockwise[1], true);
//                if (o.radianbetween(o.arc->ell.Point2Radian(cc)))
//                    cclockwise[1] = minmax_clockwise(start, cc, cclockwise[1], false);
//            }
//        }
//    } else if (!o.arc) {
//        if (TangentFrom(o.start, c, cc)) {
//            if (radianbetween(arc->ell.Point2Radian(c)))
//                clockwise[0]  = minmax_clockwise(o.start, cc, clockwise[0], false);
//            if (radianbetween(arc->ell.Point2Radian(cc)))
//                cclockwise[0] = minmax_clockwise(o.start, c, cclockwise[0], true);
//        }
//    } else {
//        XY c2[2], cc2[2];
//        if (arc->ell.TangentFrom(o.arc->ell, c2, cc2)) {
//            if (radianbetween(arc->ell.Point2Radian(c2[0])) && o.radianbetween(o.arc->ell.Point2Radian(c2[1]))) {
//                clockwise[1]  = minmax_clockwise(clockwise[0], c2[1], clockwise[1], true);
//                clockwise[0]  = minmax_clockwise(clockwise[1], c2[0], clockwise[0], false);
//            }
//            if (radianbetween(arc->ell.Point2Radian(cc2[0])) && o.radianbetween(o.arc->ell.Point2Radian(cc2[1]))) {
//                cclockwise[0] = minmax_clockwise(cclockwise[1], cc2[0], cclockwise[0], true);
//                cclockwise[1] = minmax_clockwise(cclockwise[0], cc2[1], cclockwise[1], false);
//            }
//        }
//        if (o.arc->ell.TangentFrom(arc->ell, c2, cc2)) {
//            if (radianbetween(arc->ell.Point2Radian(c2[1])) && o.radianbetween(o.arc->ell.Point2Radian(c2[0]))) {
//                clockwise[1]  = minmax_clockwise(clockwise[0], cc2[0], clockwise[1], false);
//                clockwise[0]  = minmax_clockwise(clockwise[1], cc2[1], clockwise[0], true);
//            }
//            if (radianbetween(arc->ell.Point2Radian(cc2[1])) && o.radianbetween(o.arc->ell.Point2Radian(cc2[0]))) {
//                cclockwise[0] = minmax_clockwise(cclockwise[1], c2[1], cclockwise[0], false);
//                cclockwise[1] = minmax_clockwise(cclockwise[0], c2[0], cclockwise[1], true);
//            }
//        }
//    }
//    return true;
//}
//
//} //namespace contour standard
//

namespace contour {

XY Edge::Split(double t) const
{
    const XY P = pow(1-t, 3) * start
        + 3 * pow(1-t, 2) * t * c1
        + 3 * (1-t) * pow(t, 2) * c2
        + pow(t, 3) * end;
    double t2; 
    XY TP;
    double d = Distance(P, TP, t2);
    _ASSERT(fabs(d)<0.001);

    return pow(1-t, 3) * start
            + 3 * pow(1-t, 2) * t * c1
            + 3 * (1-t) * pow(t, 2) * c2
            + pow(t, 3) * end;
    //(-A+3B-3C+D)t^3 + (3A-6B+3C)t^2 + (-3A+3B)t + A
}


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
    _ASSERT(!straight);
    if (s<t) std::swap(s, t);
    XY r1, r2;
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
    if (s<0 || s>1) return false;
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
        bool second_exists = false;
        switch (CrossingSegments(A, r, pos_my, pos_other)) {
        default:
            _ASSERT(0); //fallthrough
        case 0: 
            return 0;
        case 2:
            pos_my[1] = pos_my[1]*pos_my_mul+pos_my_offset;
            pos_other[1] = pos_other[1]*pos_other_mul+pos_other_offset;
            second_exists = true;
            //fallthrough
        case 1:
            pos_my[0] = pos_my[0]*pos_my_mul+pos_my_offset;
            pos_other[0] = pos_other[0]*pos_other_mul+pos_other_offset;
            return second_exists ? 2 : 1;
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
            _ASSERT(start.test_equal(next.start) || test_equal(a, angle(start, end, next.start)));
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
unsigned Edge::Crossing(const Edge &A, XY r[], double pos_my[], double pos_other[]) const
{
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
   
    double result[4];
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
        const XY pu = Split2(result[u]);
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

////These are expensive for curves
//Edge& Edge::SetStart(const XY &p)
//{
//    if (!straight) {
//        XY dummy;
//        double t;
//        const double d = Distance(p, dummy, t);
//        _ASSERT(fabs(d)<0.01);
//        Chop(t, 1);
//    }
//    start = p;
//    return *this;
//}
//
//Edge& Edge::SetEnd(const XY &p)
//{
//    if (!straight) {
//        XY dummy;
//        double t;
//        const double d = Distance(p, dummy, t);
//        _ASSERT(fabs(d)<0.01);
//        Chop(0, t);
//    }
//    end = p;
//    return *this;
//}
//

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

///** Expand helper: Check if `S`->`E` is an opposite direction curve than us now. 
// * This assumes both points are on us. */
//bool Edge::IsOpposite(const XY &S, const XY &E) const
//{
//    if (start.test_equal(end)) return false;
//    if (S.test_equal(E)) return false;
//    if (straight) {
//        if (fabs(start.x-end.x) > fabs(start.y-end.y))
//            return (start.x<end.x) != (S.x<E.x);
//        else
//            return (start.y<end.y) != (S.y<E.y);
//    }
//    return FindBezierParam(S)>FindBezierParam(E);
//}

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

unsigned Edge::SolveForDistance(const XY &p, double ret[5]) const
{
    Edge a(XY(0, 0), XY(1, 2), XY(3, 3), XY(4, 2));
    XY pp(3.5, 2.0);
    double pos[5];
    unsigned u = a.SolveForDistance1(XY(0,0), pos);

    return SolveForDistance1(p, ret);
}

} //namespace contour 
