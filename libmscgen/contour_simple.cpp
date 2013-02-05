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

/** @file contour_simple.cpp Defines SimpleContour.
 * @ingroup contour_files
 */

#include <cassert>
#include <algorithm>
#include <map>
#include <set>
#include <stack>
#include "contour.h"

namespace contour {

///////////////////////////// SimpleContour

//Do not create degenerate triangles.
//Always create clockwise.
SimpleContour::SimpleContour(XY a, XY b, XY c)
{
    switch (triangle_dir(a,b,c)) {
    default: //create empty if degenerate triangle
        return;
    case CLOCKWISE:
        edges.push_back(Edge(a, b));
        edges.push_back(Edge(b, c));
        edges.push_back(Edge(c, a));
        break;
    case COUNTERCLOCKWISE:
        edges.push_back(Edge(a, c));
        edges.push_back(Edge(c, b));
        edges.push_back(Edge(b, a));
        break;
    }
    boundingBox.MakeInvalid();
    for (size_t i=0; i<size(); i++)
        boundingBox += at(i).GetBoundingBox();
    clockwise = true;
    area_cache.first=false;
}

/** Create an ellipse (or ellipse slice) shape.
 *
 * @param [in] c Centerpoint
 * @param [in] radius_x Radius in the x direction. Its absolute value is used.
 * @param [in] radius_y Radius in the y direction (same as `radius_x` if omitted = circle) Its absolute value is used.
 * @param [in] tilt_deg The tilt of the ellipse in degrees. 0 if omitted.
 * @param [in] s_deg The startpoint of the arc.
 * @param [in] d_deg The endpoint of the arc. If equal to `s_deg` a full ellipse results, else a straight line is added to close the arc.
 * If `radius_x` is zero, we return an empty shape. If `radius_y` is zero, we assume it to be the same as `radius_x` (circle).
*/
SimpleContour::SimpleContour(const XY &c, double radius_x, double radius_y, double tilt_deg, double s_deg, double d_deg)
{
    if (radius_x==0) return;
    clockwise = true;
    if (radius_y==0) radius_y = radius_x;
    Edge edge(c, radius_x, radius_y, tilt_deg, s_deg, d_deg);
    boundingBox = edge.GetBoundingBox();
    edges.push_back(edge);
    area_cache.first=false;
    if (edge.GetType()==Edge::FULL_CIRCLE) return; //full circle
    edges.push_back(Edge(edge.GetEnd(), edge.GetStart()));
    edges.rbegin()->visible = false;
}

/** Clear the shape and assign a rectangle shape. */
SimpleContour &SimpleContour::operator =(const Block &b)
{
    clear();
    if (b.IsInvalid() || !test_smaller(b.x.from, b.x.till) || !test_smaller(b.y.from, b.y.till)) return *this;
    boundingBox = b;
    edges.push_back(Edge(b.UpperLeft(), XY(b.x.till, b.y.from)));
    edges.push_back(Edge(XY(b.x.till, b.y.from), b.LowerRight()));
    edges.push_back(Edge(b.LowerRight(), XY(b.x.from, b.y.till)));
    edges.push_back(Edge(XY(b.x.from, b.y.till), b.UpperLeft()));
    clockwise = true;
    area_cache.first = true;
    area_cache.second = b.x.Spans()*b.y.Spans();
    return *this;
}

/** Revert the clockwisedness of the shape by reverting, all edges and their order. */
void SimpleContour::Invert()
{
    for (size_t i=0; i<size(); i++)
        at(i).Invert();
    for (size_t i=0; i<size()/2; i++)
        std::swap(at(i), at(size()-1-i));
    clockwise = !clockwise;
    area_cache.second = -area_cache.second;
}

/** Calculate the relation of a point and the shape.
 *
 * @param [in] p The point in question.
 * @param [out] edge If not NULL it can receive the number of the edge if `p` lies on an edge/vertex.
 * @param [out] pos If not NULL it can receive the position of `p` on the edge if `p` lies on one.
 * @param [in] strict If false, points close to edges will be reported as on the edge.
 * @returns The relation. Note that for counerclockwise 'inside' is the same as if the
            contour were clockwise, that is in the limited space enclosed.
 */
EPointRelationType SimpleContour::IsWithin(XY p, size_t *edge, double *pos, bool strict) const
{
    if (size()==0 || boundingBox.IsWithin(p)==WI_OUTSIDE) return WI_OUTSIDE;

    //Follow the contour and see how much it crosses the vertical line going through p
    //count the crossings below us (with larger y)
    //http://softsurfer.com/Archive/algorithm_0103/algorithm_0103.htm
    //1. a leftward edge includes its starting endpoint, and excludes its final endpoint;
    //2. a rightward edge excludes its starting endpoint, and includes its final endpoint;
    //3. vertical edges are excluded; and
    //4. the edge-ray intersection point must be strictly below of the point P.
    //4b: since we say containment also for edge points, if the egde goes through p, we stop
    size_t count = 0;
    size_t e;
    for (e = 0; e<size(); e++) {
        if (edge) *edge = e;      //return value
        if (at(e).GetStart().test_equal(p)) return WI_ON_VERTEX;
        double y[2], po[2];
        bool forward[2];
        switch (at(e).CrossingVertical(p.x, y, po, forward)) {
        case 2:
            if ((strict && y[1] == p.y) ||  //on an edge, we are _not_ approximate here
                (!strict&& test_equal(y[1], p.y))) {  //on an edge, we are approximate here
                //we have tested that at(e) is not equal to p, so no need to test for that here
                if (test_equal(at(e).GetEnd().x, p.x)) {
                    if (edge) *edge = next(e);
                    return WI_ON_VERTEX;
                } else {
                    if (pos) *pos = po[1];
                    return WI_ON_EDGE; // on an edge, but not vertex
                }
            }
            if (y[1] > p.y) count ++;
            //fallthrough
        case 1:
            if ((strict && y[0] == p.y) ||  //on an edge, we are _not_ approximate here
                (!strict&& test_equal(y[0], p.y))) {  //on an edge, we are approximate here
                //we have tested that at(e) is not equal to p, so no need to test for that here
                if (test_equal(at(e).GetEnd().x, p.x)) {
                    if (edge) *edge = next(e);
                    return WI_ON_VERTEX;
                } else {
                    if (pos) *pos = po[0];
                    return WI_ON_EDGE; // on an edge, but not vertex
                }
            }
            if (y[0] > p.y) count ++;
            break;
        case -1:
            if ((test_smaller(p.y, at(e).GetStart().y) && test_smaller(at(e).GetEnd().y, p.y)) ||
                (test_smaller(at(e).GetStart().y, p.y) && test_smaller(p.y, at(e).GetEnd().y))) {
                if (pos) *pos = (p.y-at(e).GetStart().y)/(at(e).GetEnd().y-at(e).GetStart().y);
                return WI_ON_EDGE; //goes through p
            }
            //we have tested that at(e) is not equal to p, so no need to test for that here
            if (test_equal(at(e).GetEnd().y, p.y)) {
                if (edge) *edge = next(e);
                return WI_ON_VERTEX; //on vertex
            }
        case 0:
            break;
        default:
            _ASSERT(0);
        }
    }
    return count&1 ? WI_INSIDE : WI_OUTSIDE; //even is out
}


//////////////////////////////////SimpleContour::Combine (Union, intersection, subsrtaction) implementation


//true if q is between (a and b) modulo 4
//if q is almost equal to a or b, we return false;
inline bool really_between04_warp (double q, double a, double b)
{
    if (a<b) return test_smaller(q,b) && !test_smaller(q,a);
    return test_smaller(a,q) || test_smaller(q,b);
}

//Can result SAME, APRT, A_INSIDE_B or B_INSIDE_A.
//It can also return OVERLAP, which means one of our point is outside b, can either be APART or b may be in us
//clockwiseness is ignored: the inside of a counterclockwise contour is the limited space
//it encircles.
EContourRelationType SimpleContour::CheckContainmentHelper(const SimpleContour &b) const
{
    size_t edge;
    for (size_t i=0; i<size(); i++) {
        double pos;
        const XY p = at(i).GetStart();
        //if we are a single ellipsis, use our center point, else use a vertex
        switch (b.IsWithin(p, &edge, &pos, /*strict=*/false)) {
        default:
            _ASSERT(0);
        case WI_INSIDE:  return REL_A_INSIDE_B;
        case WI_OUTSIDE: return REL_OVERLAP;
        case WI_ON_VERTEX:
            pos = 0; //if we are on b's vertex, this is the right pos for that edge
        case WI_ON_EDGE:
            double one_prev = angle(p, XY(p.x, -100),   PrevTangentPoint(i, 0));
            double one_next = angle(p, XY(p.x, -100),   NextTangentPoint(i, 0));
            double two_prev = angle(p, XY(p.x, -100), b.PrevTangentPoint(edge, pos));
            double two_next = angle(p, XY(p.x, -100), b.NextTangentPoint(edge, pos));
            if (!  clockwise) std::swap(one_prev, one_next); //make angles as if clockwise
            if (!b.clockwise) std::swap(two_prev, two_next); //make angles as if clockwise

            //if both the same, we continue: this vertex is non-decisive
            if (test_equal(one_prev, two_prev) && test_equal(one_next, two_next)) break; //SAME - do another edge

            //if values are too close it is dangerous, we look for other vertices
            if (test_equal(one_prev, one_next)) break;
            if (test_equal(two_prev, two_next)) break;

            if (really_between04_warp(one_prev, two_next, two_prev) &&
                really_between04_warp(one_next, two_next, two_prev)) return REL_A_INSIDE_B;
            if (really_between04_warp(two_prev, one_next, one_prev) &&
                really_between04_warp(two_next, one_next, one_prev)) return REL_B_INSIDE_A;
            return REL_APART;
        }
    }
    //All points were on a vertex equal in one of the directions
    return REL_SAME;
}


/** Determines the relation of two simplecontours assuming none of their edges cross.
 *
 * Gives valid result only if the two contours have no crosspoints.
 * Clockwiseness fully ignored: the inside of counterclockwise contours is the
 * limited space they encloses.
 */
EContourRelationType SimpleContour::CheckContainment(const SimpleContour &other) const
{
    //special case of two full ellipses touching - not caught otherwise
    if (size()==1 && other.size()==1 && at(0).GetStart() == other.at(0).GetStart()) {
        if (at(0).ell==other[0].ell) return REL_SAME;
        //if one of the centers is in the other ellipses then x_INSIDE_y
        if (inside(IsWithin(other[0].ell.GetCenter())) ||
            inside(other.IsWithin(at(0).ell.GetCenter()))) {
                //the center closer to the touchpoint will be inside
                if (at(0).GetStart().Distance(at(0).ell.GetCenter()) <
                    at(0).GetStart().Distance(other[0].ell.GetCenter()))
                    return REL_A_INSIDE_B;
                else
                    return REL_B_INSIDE_A;
        }
        return REL_APART;
    }
    EContourRelationType this_in_other = CheckContainmentHelper(other);
    if (this_in_other != REL_OVERLAP) return this_in_other;
    switch (other.CheckContainmentHelper(*this)) {
    default:
    case REL_A_IN_HOLE_OF_B:
    case REL_B_IN_HOLE_OF_A:
    case REL_IN_HOLE_APART:
    case REL_SAME: _ASSERT(0); return REL_SAME;
    case REL_OVERLAP:
    case REL_APART:       return REL_APART;
    case REL_A_INSIDE_B:  return REL_B_INSIDE_A; //other is in us
    case REL_B_INSIDE_A:  return REL_A_INSIDE_B; //we are in other
    }
}

//void SimpleContour::CalculateClockwise()
//{
//    //determine if this is clockwise.
//    if (size()>2) {
//        double angles = 0;
//        XY prev = PrevTangentPoint(0, 0.0);
//        for (size_t i=0; i<size(); i++)
//            switch (at(i).GetType()) {
//            case Edge::STRAIGHT:
//				_ASSERT(!at(i).GetStart().test_equal(at_next(i).GetStart()));
//                angles += angle_degrees(angle(at(i).GetStart(), at_next(i).GetStart(), prev));
//                prev = at(i).GetStart();
//                break;
//            case Edge::FULL_CIRCLE:
//                //continue; //do nothing
//            case Edge::ARC:
//                angles += angle_degrees(angle(at(i).GetStart(), NextTangentPoint(i, 0.0), prev));
//                prev = PrevTangentPoint(i, 1.0);
//                if (at(i).GetClockWise()) {
//                    if (at(i).GetRadianS()<at(i).GetRadianE()) angles -=       (at(i).GetRadianE()-at(i).GetRadianS())*(180./M_PI);
//                    else                                       angles -= 360 - (at(i).GetRadianS()-at(i).GetRadianE())*(180./M_PI);
//                } else {
//                    if (at(i).GetRadianE()<at(i).GetRadianS()) angles +=       (at(i).GetRadianS()-at(i).GetRadianE())*(180./M_PI);
//                    else                                       angles += 360 - (at(i).GetRadianE()-at(i).GetRadianS())*(180./M_PI);
//                }
//            }
//            //angle is (n-2)*180 for clockwise, (n+2)*180 for counterclockwise, we draw the line at n*180
//            if (angles/180. - floor(angles/180.)*180. >= 1)
//                angles = angles;
//        clockwise = bool(angles < size()*180);
//        return;
//    }
//    if (size()==2) {
//        //if a contour is two edges, it should not be two straigth edges
//        if (at(0).GetType() == Edge::STRAIGHT && at(1).GetType() == Edge::STRAIGHT) {
//            _ASSERT(0);
//        }
//        if (at(0).GetType() != Edge::STRAIGHT && at(1).GetType() != Edge::STRAIGHT) {
//            //two curves
//            //if they are of same direction we get it
//            if (at(0).GetClockWise() == at(1).GetClockWise()) {
//                clockwise = at(0).GetClockWise();
//                return;
//            }
//            //two curves with opposite dir, they do not touch only at the two ends
//            //the one contains the other decides
//            const XY center_line = (at(0).GetStart()+at(1).GetStart())/2;
//            const XY center0 = at(0).GetEllipseData().Radian2Point(at(0).GetRadianMidPoint());
//            const XY center1 = at(1).GetEllipseData().Radian2Point(at(1).GetRadianMidPoint());
//            const double dist0 = (center0 - center_line).length();
//            const double dist1 = (center1 - center_line).length();
//            clockwise = at(0).GetClockWise() == (dist0 > dist1);
//            return;
//        }
//        //one curve, one straight: dir is decided by curve
//        if (at(0).GetType() == Edge::STRAIGHT)
//            clockwise = at(1).GetClockWise();
//        else
//            clockwise = at(0).GetClockWise();
//        return;
//    }
//    //if (size()==1), a full ellipsis
//    clockwise = at(0).GetClockWise();
//    return;
//}

/** Set the `clockwise` member according to how edges go. */
void SimpleContour::CalculateClockwise()
{
    //determine if this is clockwise.
    if (size()==1)
        clockwise = at(0).GetClockWise();
    else if (size()==2 && at(0).GetType() != Edge::STRAIGHT && at(1).GetType() != Edge::STRAIGHT && at(0).GetClockWise() == at(1).GetClockWise())
        clockwise = at(0).GetClockWise();
    else
        clockwise = GetArea()>0;
}

/** Appends an edge and makes checks. Used during walking.
 *
 * Appends an edge to the (yet incomplete, not closed) shape.
 * Checks that we do not append a degenerate (start==end) edge.
 * Checks if the edge to append is a direct continuation of the last edge, in which case
 * we merge the two.
 * WE DO NOT UPDATE `area_cache`, `clockwise`, `boundingBox` and do not ensure
 * we leave the shape closed.
 */
void SimpleContour::AppendDuringWalk(const Edge &edge)
{
    _ASSERT(edge.IsSaneNoBoundingBox());
    //if the last point equals to the startpoint of edge, skip the last edge added
    //except if the last edge is a full circle
    if (size()>0 && edge.GetStart().test_equal(edges.rbegin()->GetStart()))
        if (edges.rbegin()->GetType() != Edge::FULL_CIRCLE)
            edges.pop_back();
    //check if "edge" is a direct continuation of the last edge and can be combined
    //if not, append "edge"
    if (size()==0 || !edges.rbegin()->CheckAndCombine(edge))
        edges.push_back(edge);
    _ASSERT(at(size()-1).IsSaneNoBoundingBox());
};


/** Clean up and sanitize the contour after a walk.
 *
 * Cleanup consists of
 * - Check repeated points at the end
 * - Set `e` values for curvy edges
 * - Calculate edge and contour bounding boxes
 *
 * Also, do a lot of sanity checks:
 * - a contour with one edge must be a full ellipse. If it is straight, we clear()
 * - a contour with two edges must have at least one curvy edge, if not, we clear()
 * returns true if contour is OK.
 */
bool SimpleContour::PostWalk()
{
    boundingBox.MakeInvalid();
    area_cache.first = false;

    //if the crosspoint we started at was also a vertex, it may be that it is repeated at the end
    if (size()>1 && at(0).GetStart().test_equal(at(size()-1).GetStart()))
        edges.pop_back();

    //set "end" values. Not known previously
    for (size_t i=0; i<size(); i++)
        at(i).SetEndLiberal(at_next(i).GetStart(), true);

    //Also, the beginning point (at(0]) can fall on an edge
    //if both the last and the first edge are straight and are in-line
    //such as when two side-by-side rectangles are merged
    if (size()>2) {
        if (at(size()-2).CheckAndCombine(at(size()-1)))
            edges.pop_back();
        if (at(size()-1).CheckAndCombine(at(0)))
            edges.erase(edges.begin());
    }

    //also two semi-circles combined should give a single edge (a circle)
    if (size()==2 && at(0).GetType()!=Edge::STRAIGHT && at(1).GetType()!=Edge::STRAIGHT)
        if (at(0).CheckAndCombine(at(1)))
            edges.pop_back();

    //Sanity checks
    if (size()>0 && at(0).GetType()==Edge::STRAIGHT)
        if (size()==1 || (size()==2 && at(1).GetType()==Edge::STRAIGHT))
            clear();
    if (size()==0) return false;

    //we checked above it is curvy. We assert that this is a full circle
    if (size()==1)
        at(0).SetFullCircle();

    //compute the bounding box & clockwise
    CalculateBoundingBox();
    CalculateClockwise();
    _ASSERT(IsSane());
    return true;
}

void SimpleContour::assign_dont_check(const std::vector<XY> &v)
{
    clear();
    if (v.size()<2) return;
    for (size_t i=0; i<v.size(); i++)
        edges.push_back(Edge(v[i], v[(i+1)%v.size()]));
    Sanitize();  //includes CalculateBoundingBox() and CalculateClockwise()
}

void SimpleContour::assign_dont_check(const XY v[], size_t size)
{
    clear();
    if (size < 2) return;
    for (size_t i=0; i<size; i++)
        edges.push_back(Edge(v[i], v[(i+1)%size]));
    Sanitize();  //includes CalculateBoundingBox() and CalculateClockwise()
}

void SimpleContour::assign_dont_check(const std::vector<Edge> &v)
{
    clear();
    if (v.size()<2) return;
    edges = v;
    Sanitize();  //includes includes CalculateBoundingBox() and CalculateClockwise()
}

void SimpleContour::assign_dont_check(const Edge v[], size_t size)
{
    clear();
    if (size < 2) return;
    for (size_t i=0; i<size; i++)
        edges.push_back(v[i]);
    Sanitize();  //includes includes CalculateBoundingBox() and CalculateClockwise()
}

bool SimpleContour::IsSane() const
{
    if (size()==0) return true;
    if (size()==1)
        return at(0).GetType() == Edge::FULL_CIRCLE;
    for (size_t u=0; u<size(); u++) {
        if (at(u).GetStart().test_equal(at(u).GetEnd()) &&
            at(u).GetType() != Edge::FULL_CIRCLE)
            return false;
        if (!at(u).IsSane()) return false;
    }
    if (size()==2 && at(0).GetType()==Edge::STRAIGHT && at(1).GetType()==Edge::STRAIGHT)
        return false;
    return true;
}

/** Fix problems in a shape (edges not joint, etc).
 *
 * @returns true if we were sane, false if changes were needed.
 */
bool SimpleContour::Sanitize()
{
    area_cache.first=false;
    bool ret = true;
    if (size()==0) return true;
    if (size()==1) {
        if (at(0).GetType() == Edge::FULL_CIRCLE) return true;
        goto clear;
    }
    for (size_t u=0; u<size(); /*nope*/)
        if (at(u).GetType()!=Edge::FULL_CIRCLE && at(u).GetStart().test_equal(at(u).GetEnd())) {
            edges.erase(edges.begin()+u);
            ret = false;
        } else
            u++;
    switch (size()) {
    case 0: return false;
    case 1:
        if (at(0).GetType() == Edge::FULL_CIRCLE) return true;
        goto clear;
    case 2:
        if (at(0).GetType()==Edge::STRAIGHT && at(1).GetType()==Edge::STRAIGHT)
            goto clear;
        /*fallthrough*/
    default:
        //ensure that edges connect
        for (size_t u=0; u<size(); u++)
            if (!at(u).GetEnd().test_equal(at_next(u).GetStart()))
                goto clear;
    }
    CalculateBoundingBox();
    CalculateClockwise();
    return ret;
clear:
    clear();
    return false;
}

/** Calculate the boundingBox of edges and the whole shape. */
const Block &SimpleContour::CalculateBoundingBox()
{
    boundingBox.MakeInvalid();
    for(size_t i=0; i<size(); i++)
        boundingBox += at(i).CalculateBoundingBox();
    return boundingBox;
}

/** This takes a sane shape and adds an edge to the end. Leaves a sane shape.
 *
 * If the edge to insert starts at the last point, we replace the last edge to it.
 * (makes sense if we replace a straight edge to a curve or vice versa)
 * If the last edge is a curve, we anyway replace the edge to be inserted.
 * If we insert a curve, which does not.
 * Finally, if the edge does not end at the first vertex, we add a line to close the shape.
 * @param [in] edge The edge to insert.
 * @returns true if edge inserted successfully. False if it would have leaved the shape flopped.
 */
bool SimpleContour::AddAnEdge(const Edge &edge)
{
    XY dum1[8];
    double dum2[8];
    SimpleContour ret(*this);
    if (size()) {
        //see if we need to replace the last edge, drop it
        if (edge.GetStart().test_equal(ret[size()-1].GetStart()))
            ret.edges.pop_back();
        else         //set the last edge to straight
            ret[size()-1] = Edge(ret[size()-1].GetStart(), edge.GetStart());
    }
    //insert edge
    ret.edges.push_back(edge);
    size_t num_to_check = 1;
    //if edge is curvy and does not end in at(0).start, insert a straight edge afterwards
    if (edge.GetType()!=Edge::STRAIGHT) {
        if (!edge.GetEnd().test_equal(ret[0].GetStart())) {
            ret.edges.push_back(Edge(edge.GetEnd(), ret[0].GetStart()));
            //check if this edge to insert do not cross the previously inserted edge
            if (ret[ret.size()-2].Crossing(ret[ret.size()-1], dum1, dum2, dum2))
                return false;
            num_to_check = 2;
        }
    }
    //now check if inserted edges cross any of the ones before
    //check if edge->end is crossing any existing edges
    ret.CalculateBoundingBox(); //needed by crossing
    for (size_t i = 0; i<ret.size()-num_to_check-1; i++)
        for (size_t j = ret.size()-num_to_check-1; j<ret.size(); j++)
            if (ret[i].Crossing(ret[j], dum1, dum2, dum2))
                return false;
    //OK, we can have these edges inserted
    swap(ret);
    area_cache.first=false;
    return true;
}

/** Calculate & cache the surface area of the shape.
 *
 * This is based on <http://www.mathopenref.com/coordpolygonarea.html>
 * For each edge, we calculate the area above it (the area between the edge and Y axis,
 * but since y grows downward, the area is "above" the edge).
 * The value is signed. If the edge is from right to left it is positive, else negative.
 * Since (for straight edges) the area would contain "start.x*start.y-end.x*end.y",
 * which will cancel out. Plus it is easier to return twice the area.
 * Thus we request edge to return
 * "2*(area_above_edge - start.x*start.y + end.x*end.y)", this is what
 * Edge::GetAreaAboveAdjusted() returns.
 *
 * After this function the area cache will hold this value for instant access.
 *
 * @returns the calculated area. Negative if shape is counterclockwise.
 */
double SimpleContour::CalcualteArea() const
{
    register double ret = 0;
    for (size_t i=0; i<size(); i++)
        ret += at(i).GetAreaAboveAdjusted();
    area_cache.first = true;
    return area_cache.second = ret /2;
}

/** Calculate the circumference length of the shape.
 *
 * @param [in] include_hidden If false, we leave out edges marked as non-visible.
 * @returns Length of the circumference.
 */
double SimpleContour::GetCircumference(bool include_hidden) const
{
    register double ret = 0;
    for (size_t i=0; i<size(); i++)
        if (include_hidden || at(i).visible)
            ret += at(i).GetLength();
    return ret;
}

/** Returns the coordinates of the centroid of the shape multiplied by its (signed) area. */
XY SimpleContour::CentroidUpscaled() const
{
    XY ret (0,0);
    for (size_t i=0; i<size(); i++)
        ret += at(i).GetCentroidAreaAboveUpscaled();
    return ret;
}

/** Calculates the touchpoint of tangents drawn from a given point.
 *
 * Given the point `from` draw tangents to the shape (two can be drawn)
 * and calculate where these tangents touch the shape.
 * Tangent is anything touching the shape (at a vertex, at the middle of a curvy edge
 * or going along, in parallel with a full straight edge).
 * In this context the *clockwise tangent* is the one which is traversed from
 * `from` towards the shape touches the shape in the clockwise direction.
 * @param [in] from The point from which the tangents are drawn.
 * @param [out] clockwise The point where the clockwise tangent touches the shape.
 * @param [out] cclockwise The point where the counterclockwise tangent touches the shape.
 * @returns True if success, false if `from` is inside or on the shape.
 */
bool SimpleContour::TangentFrom(const XY &from, XY &clockwise, XY &cclockwise) const
{
    if (size()==0 || IsWithin(from)!=WI_OUTSIDE) return false;
    bool was=false;
    XY c, cc;
    for (unsigned u=0; u<size(); u++)
        if (!was)
            was = at(u).TangentFrom(from, clockwise, cclockwise);
        else if (at(u).TangentFrom(from, c, cc)) {
            clockwise =  minmax_clockwise(from, clockwise,  c,  true);
            cclockwise = minmax_clockwise(from, cclockwise, cc, false);
        }
    return was;
}

/** Calculates the touchpoint of tangents drawn to touch two shapes.
 *
 * Given the two shapes, four such tangents can be drawn, here we focus on the two
 * outer ones, the ones that touch either both shapes clockwise or both of them
 * counterclockwise, but not mixed.
 * Tangent is anything touching the shape (at a vertex, at the middle of a curvy edge
 * or going along, in parallel with a full straight edge).
 * @param [in] from The other shape to the tangents are drawn.
 * @param [out] clockwise The points where the clockwise tangent touches our shape
 *                        (clockwise[0]) and `from` (clockwise[1]).
 * @param [out] cclockwise The points where the counterclockwise tangent touches our shape
 *                         (cclockwise[0]) and `from` (cclockwise[1]).
 * @returns True if success, false if `from` is inside us.
 */
bool SimpleContour::TangentFrom(const SimpleContour &from, XY clockwise[2], XY cclockwise[2]) const
{
    if (size()==0 || from.size()==0) return false;
    clockwise[0] = cclockwise[0] = at(0).GetStart();
    clockwise[1] = cclockwise[1] = from.at(0).GetStart();
    bool was = false;
    for (unsigned u=0; u<size(); u++)
        for(unsigned v=0; v<from.size(); v++)
            was |= at(u).TangentFrom(from[v], clockwise, cclockwise);
    return was;
}


//////////////////////////////////SimpleContour::Expand implementation

/** Helper that creates a half circle for EXPAND_ROUND or EXPAND_MITER_ROUND type expansion,
 * where expanded edges are parallel.
 *
 * @param [in] start The start of the half circle (end of the previous expanded edge)
 * @param [in] end The end of the half circle (the start of the next previous edge)
 * @param [in] old The position of the vertex before expanding the edges.
 * @param [in] clockwise The clockwisedness of the original shape.
 * @returns A half circle connecting to previous and following expanded edges.
 */
Edge SimpleContour::CreateRoundForExpand(const XY &start, const XY &end, const XY& old, bool clockwise)
{
    XY center = old;
    double radius = old.Distance(start);
    if (!test_equal(radius, old.Distance(end))) {
        center = (start+end)/2;
        radius = start.Distance(center);
    }
    Edge edge(center, radius);
    //We do not know if the resulting contour will be clockwise or not
    //But, if we use the clockwiseness of the original contour, we are safe
    //since for positive originals we will keep only positive results,
    //so if we miss and we will be part of a negative contour, that
    //will get dropped anyway. (Likewise for negative orgiginals.)
    if (!clockwise) edge.Invert();
    edge.SetStartLiberal(start);  //these keep clockwise
    edge.SetEndLiberal(end);
    return edge;
}

/** Expands a shape.
 *
 * Expansion aims to create a shape whose edges run parallel along the edges of the
 * original, but at `gap` pixels away. Positive `gap` values result in actual expansion
 * negative values in shrinkage (the new shape is inside the original).
 * The function works for counterclockwise shapes, the meaning of the sign of `gap` is
 * reversed.
 *
 * After the expansion, we check if there are edges crossing each other and
 * remove them by walking. In this space we also may end up in multiple separate
 * SimpleContours. (E.g., shrinking a dog-bone shape may lead us to two circles,
 * like `O=O` becomes `o o`.) This is why the result is stored in a full Contour
 * which may hold multiple SimpleContours.
 *
 * @param [in] type There are multiple typical ways to handle edge joins, see EExpandType.
 * @param [in] gap The amount of expansion/shrinkage. No-op if zero.
 * @param [out] res The expanded shape.
 * @param miter_limit Tells us that with EXPAND_MITER_* how long the miter edges can be
 *                    in terms of the `gap`.
 *                    E.g., miter_limit=2 says they can be as long as `2*gap`.
 *                    The rest is cut away with a bevel-like edge.
 */
void SimpleContour::Expand(EExpandType type, double gap, Contour &res, double miter_limit) const
{
    //Return us if nothing to do.
    if (gap==0) {
        res = *this;
        return;
    }
    //if we shrink beyond our bounding box, we become empty
    const double shrink_by = clockwise ? -gap : gap;
    if (size()==0 || boundingBox.x.Spans() < 2*shrink_by || boundingBox.y.Spans() < 2*shrink_by)
        return; //empty "res" assumed

    SimpleContour r(*this);  //In this we perform the expansion
    SimpleContour o(*this);  //In this we keep track of the original edges
    //If a full circle, do it quick.
    if (size()==1) {
        _ASSERT(at(0).GetType()==Edge::FULL_CIRCLE);
        if (!r[0].Expand(gap)) return; //full circle disappeared, we return empty "res"
        r.CalculateBoundingBox();
        r.area_cache.first = false;
        res = std::move(r);
        return;
    }

    //Expand all the edges
    for (size_t i = 0; i<r.size(); i++)
        if (!r[i].Expand(gap)) {
            //Expand() returns false if a circle has shrunken to degenerate into a line or point.
            //If so, we replace them with straight lines
            r[i] = o[i];
            r[i].type = Edge::STRAIGHT;
            r[i].Expand(gap);
        }

    //We had two arcs, both became straight lines, we became empty.
    if (r.size()==2 && r[0].GetType()==Edge::STRAIGHT && r[1].GetType()==Edge::STRAIGHT)
        return;


    //Find how and where expanded edges meet
    std::vector<Edge::EExpandCPType> cross_type (r.size()); //Stores the type of crossing between edge #i and #i+1 (wrapped around)
    std::vector<XY>                  cross_point(r.size()); //Stores the cp between edge #i and #i+1 (wrapped around)
    for (size_t i = 0; i<r.size(); i++) {
        cross_type[i] = r[i].FindExpandedEdgesCP(r.at_next(i), cross_point[i]);
        _ASSERT(cross_type[i] != Edge::DEGENERATE);
    }

    //Remove those edges which changed direction
    // This may happen especially at shrinkage, see the example below.
    //     +->-+
    //    /     \      after shrinkage    +---+
    //   /       \     this becomes ->     \ /
    //  /         \                         o
    // /           \                       / \    .
    //
    // Here the top horizontal edge is walked in the opposite direction (from right to left)
    // as before the expansion (left to right). Such edges (and the small triangle associated
    // shall be removed from the result). This is what we do below.
    if (type == EXPAND_MITER ||
        type == EXPAND_MITER_ROUND ||
        type == EXPAND_MITER_BEVEL ||
        type == EXPAND_MITER_SQUARE) {   //for bevel and round we check this after inserting bevel & round
        size_t prev_size;
        do {
            prev_size = r.size();
            XY tmp_point;
            Edge::EExpandCPType tmp_type;
            //Kill edges if
            //1. they have good crosspoints with both previus and next edge;
            //2. if those good crosspoints are in opposite order than the originals; and
            //3. if the previous and next edges has a good cp with each other.
            //   (So if we skip the edge in between we get a meaningful result.)
            for (size_t i = 0; i<r.size(); /*nope*/) {
                if (Edge::HasCP(cross_type[r.prev(i)]) && Edge::HasCP(cross_type[i]) &&
                    r[i].IsOpposite(cross_point[r.prev(i)], cross_point[i])) {
                    tmp_type = r.at_prev(i).FindExpandedEdgesCP(r.at_next(i), tmp_point);
                    if (Edge::HasCP(tmp_type)) {
                        cross_type[r.prev(i)] = tmp_type;
                        cross_point[r.prev(i)]= tmp_point;
                        r.edges.erase(r.edges.begin()+i);   //TODO recalc crosspoint!!
                        o.edges.erase(o.edges.begin()+i);
                        cross_type.erase(cross_type.begin()+i);
                        cross_point.erase(cross_point.begin()+i);
                        //Here we removed also from "o" to maintain indices
                        continue;
                    }
                }
                i++;
            }
        } while (prev_size != r.size());
    }

    //Adjust the start and end of the edges to the crosspoints
    //and insert additional edges, if needed
    Contour res_before_untangle;                           //This is where we collect the result...
    SimpleContour &r2 = res_before_untangle.first.outline; //... well actually into the "first.outline" of it
    r2.edges.reserve(size()*3);                            //We can have at most this many edges in result.
    //Calculate actual max miter length.
    const double gap_limit = fabs(miter_limit) < MaxVal(miter_limit) ? fabs(gap)*fabs(miter_limit) : MaxVal(gap_limit);
    switch (type) {
    default:
        _ASSERT(0);
    case EXPAND_MITER:
    case EXPAND_MITER_ROUND:
    case EXPAND_MITER_BEVEL:
    case EXPAND_MITER_SQUARE:
        {
        bool need_miter_limit_bevel = cross_type[r.size()-1] == Edge::CP_EXTENDED &&
                   cross_point[r.size()-1].Distance(r[r.size()-1].GetEnd()) > gap_limit;
        //In these cases we have already removed edges that changed direction.
        for (size_t i = 0; i<r.size(); i++) {
            //The new start and endpoint for us (edge #i)
            XY new_start = Edge::HasCP(cross_type[r.prev(i)]) ? cross_point[r.prev(i)] : r[i].GetStart();
            XY new_end   = Edge::HasCP(cross_type[i])         ? cross_point[i]         : r[i].GetEnd();

            //If we connected to previous edge via a too long miter, we limit its length
            //The bevel needed in this case was added when we processed the previous edge.
            if (need_miter_limit_bevel)
                new_start = r[i].GetStart() + (new_start-r[i].GetStart()).Normalize()*gap_limit;
            //Check if we connect to the next edge via a too long miter.
            need_miter_limit_bevel = cross_type[i] == Edge::CP_EXTENDED &&
                   new_end.Distance(r[i].GetEnd()) > gap_limit;
            if (need_miter_limit_bevel) {
                //Adjust the endpoint of us to where the miter shall end.
                new_end = r[i].GetEnd() + (new_end-r[i].GetEnd()).Normalize()*gap_limit;
                need_miter_limit_bevel = true;
            }
            _ASSERT(fabs(new_start.length()) < 100000);
            _ASSERT(fabs(new_end.length()) < 100000);

            //If we did not degenerate to a point, insert us to the end result.
            if (!new_start.test_equal(new_end)) {
                //insert existin edge (updated)
                Edge tmp = r[i];
                tmp.SetStartEndForExpand(new_start, new_end);
                r2.edges.push_back(tmp);
            } else {
                //We got degenerate, do not insert
                cross_point[i] = new_end = new_start; //Use this from now on.
            }
            //See below if we need to insert additional edges
            if (Edge::HasCP(cross_type[i])) {
                //If we have a normal crosspoint, the only reason to add an edge if
                //the miter would be too long. In this case we need to add a straight
                //edge that "cuts" the miter.
                if (need_miter_limit_bevel) {
                    const XY bevel_end = r.at_next(i).GetStart() +
                        (cross_point[i]-r.at_next(i).GetStart()).Normalize()*gap_limit;
                    _ASSERT(bevel_end.length() < 10000);
                    if (!bevel_end.test_equal(new_end))
                        r2.edges.push_back(Edge(new_end, bevel_end));
                }
            } else
                //No natural crosspoint: see if we need to add a line, circle or square
                switch (type) {
                default:
                    _ASSERT(0);
                case EXPAND_MITER:
                    //We add two lines (two miters)
                    //Remember, crosspoint[i] contains a point in between the two lines.
                    r2.edges.push_back(Edge(new_end, cross_point[i]));
                    r2.edges.push_back(Edge(cross_point[i], r.at_next(i).GetStart()));
                    break;
                case EXPAND_MITER_ROUND:
                    //We add a half circe
                    r2.edges.push_back(CreateRoundForExpand(new_end, r.at_next(i).GetStart(),
                        o[i].GetEnd(), clockwise ^ (gap<0)));
                    break;
                case EXPAND_MITER_BEVEL:
                    //We simply add a line between our end and the start of the next edge
                    r2.edges.push_back(Edge(new_end, r.at_next(i).GetStart()));
                    break;
                case EXPAND_MITER_SQUARE:
                    //Here we add 3 edges of a half-square
                    const XY &next_start = r.at_next(i).GetStart();
                    const double dist = new_end.Distance(next_start)/2;
                    const XY first_tangent = r[i].NextTangentPoint(1.0);
                    const XY first = new_end + (first_tangent - new_end).Normalize()*dist;
                    const XY second_tangent = r.at_next(i).PrevTangentPoint(0.0);
                    const XY second = next_start + (second_tangent - next_start).Normalize()*dist;
                    r2.edges.push_back(Edge(new_end, first));
                    r2.edges.push_back(Edge(first, second));
                    r2.edges.push_back(Edge(second, next_start));
                    break;
                }
        }
        }
        break; //EXPAND_MITER_*
    case EXPAND_BEVEL:
    case EXPAND_ROUND:
        //Here we have not yet removed edges that changed dir, so we rate them here.
        //We actually allow edges that change dir to persist and will remove them during
        //an untangle at the end. We do this because with round or bevel modes, the inserted
        //join element (a round edge or bevel) may replace such an edge that changed direction
        //resulting in better outcome.
        enum EEdgeType {
            EXISTING,             //An edge that comes from the original shape
            EXISTING_CHANGED_DIR, //An edge that comes from the original shape, but changed direction
            INSERTED              //An edge we have inserted
        };
        std::vector<EEdgeType> edge_type;  edge_type.reserve(200);  //the type of edge in "r2"
        std::vector<size_t> edge_orig;     edge_orig.reserve(200);  //its place in "o" if not INSERTED
        for (size_t i = 0; i<r.size(); i++) {
            XY new_start = cross_type[r.prev(i)] == Edge::CP_REAL ? cross_point[r.prev(i)] : r[i].GetStart();
            XY new_end =   cross_type[i] == Edge::CP_REAL         ? cross_point[i]         : r[i].GetEnd();

            if (!new_start.test_equal(new_end)) {
                //If edge is not degenerate after expansion...
                const bool changed_dir = r[i].IsOpposite(new_start, new_end);
                //...we insert with new start and end (the crosspoints if any)
                r[i].SetStartEndForExpand(new_start, new_end);
                r2.edges.push_back(r[i]);
                edge_type.push_back(changed_dir ? EXISTING_CHANGED_DIR : EXISTING);
                edge_orig.push_back(i);
            } else {
                //existing edge got degenerate
                cross_point[i] = new_end = new_start; //use this from now on
            }
            if (cross_type[i] == Edge::CP_REAL) continue; //no need to insert bevel or round
            //We insert a bevel or round
            edge_type.push_back(INSERTED);
            edge_orig.push_back(size_t(-1));
            if (type==EXPAND_BEVEL ||
                o[i].GetEnd() != o.at_next(i).GetStart()) //a missing edge
                r2.edges.push_back(Edge(new_end, r.at_next(i).GetStart()));  //insert line
            else
                r2.edges.push_back(CreateRoundForExpand(new_end, r.at_next(i).GetStart(),
                                                  o[i].GetEnd(), gap>0));  //circle
        }
        //Now check all inserted edges.
        //If any edge in "r2" befor or after an inserted edge
        //1. is an original edge;
        //2. has changed direction; and he
        //3. the inserted edge has a good cp with the edge beyond the neighbour
        //then skip the neighbour that changed dir.
        bool all_orig_changed_dir = true;
        for (size_t u = 0; u<r2.size(); u++) {
            if (edge_type[u]!=INSERTED) {
                //We do nothing for original edges, merely note if all of them changed dir or not
                if (edge_type[u]!=EXISTING_CHANGED_DIR)
                    all_orig_changed_dir = false;
                continue;
            }
            XY tmp_point;
            //First check out the original edge before the inserted one
            const size_t prev = r2.prev(u);
            if (edge_type[prev] == EXISTING_CHANGED_DIR) { //1. an original edge and 2. changed dir...
                const size_t prevprev = r2.prev(prev);
                const Edge::EExpandCPType tmp_type = r2[prevprev].FindExpandedEdgesCP(r2[u], tmp_point);
                if (Edge::HasCP(tmp_type)) {   //... and 3. has a good cp with the one beyond.
                    //Set the inserted bevel/round to end in cp
                    r2[u].SetStartLiberal(tmp_point);
                    r2[prevprev].SetEndLiberal(tmp_point);
                    //Erase "prev"
                    r2.edges.erase(r2.edges.begin()+prev);
                    edge_type.erase(edge_type.begin()+prev);
                    edge_orig.erase(edge_orig.begin()+prev);
                    //decrement "u", so that we can u++ in the "for" statement
                    if (prev<u) u--; //this way u was >=1, can decrement
                    //else u==0 and prev==size()-1 => "u" can be incremented in "for"
                }
            }
            //Now do the same for the original edge *after* the inserted one
            const size_t next = r2.next(u);
            if (edge_type[next] == EXISTING_CHANGED_DIR) {  //1. an original edge and 2. changed dir...
                const size_t nextnext = r2.next(next);
                const Edge::EExpandCPType tmp_type = r2[u].FindExpandedEdgesCP(r2[nextnext], tmp_point);
                if (Edge::HasCP(tmp_type)) {  //... and 3. has a good cp with the one beyond.
                    //Set the inserted bevel/round to end in cp
                    r2[u].SetEndLiberal(tmp_point);
                    r2[nextnext].SetStartLiberal(tmp_point);
                    //Erase "next"
                    r2.edges.erase(r2.edges.begin()+next);
                    edge_type.erase(edge_type.begin()+next);
                    edge_orig.erase(edge_orig.begin()+next);
                    //decrement "u", so that we can u++ in the "for" statement
                    if (prev<u) u--; //this way u was >=1, can decrement
                    //else u==0 and prev==size()-1 => "u" can be incremented in "for"
                }
            }
        }
        //If all edges changed dir, we became an empty shape
        if (all_orig_changed_dir) return;
    }
    r2.Sanitize(); //calculates clockwise and boundingbox, as well. Now r2 is sane, but not res_before_untangle
    if (r2.size()==0) return; //An empty shape
    if (r2.size()==1) {
        r2[0].SetFullCircle();
        res_before_untangle.boundingBox = r2.CalculateBoundingBox();  //also calculates bounding boxes of edges
        res = std::move(res_before_untangle);
        return;
    }
    //OK, now untangle - use 'res_before_untangle' instead of r2
    res_before_untangle.boundingBox = r2.boundingBox; //copy bb to outer, now res_before_untangle is sane
    res.Operation(clockwise ? Contour::EXPAND_POSITIVE : Contour::EXPAND_NEGATIVE, std::move(res_before_untangle));
}


//my index is the index of the edge (of "this") just added
void SimpleContour::Expand2DHelper(const XY &gap, std::vector<Edge> &a,
                                   unsigned original_last, unsigned next, unsigned my_index,
                                   int last_type, int stype) const
{
    XY cp;
    switch (a[original_last].FindExpandedEdgesCP(a[next], cp)) {
    case Edge::CP_REAL:
        a[original_last].SetEndLiberal(cp);
        a[next].SetStartLiberal(cp);
        break;
    default:
        if (last_type && !(last_type & 1) && stype == -last_type &&
            //insert an extra point only if we have a sharp convex vertex
            CLOCKWISE == triangle_dir(PrevTangentPoint(my_index, 0), at(my_index).GetStart(),
                                      NextTangentPoint(my_index, 0))) {
            cp = a[original_last].GetEnd();
            switch (last_type) {
            case +2: cp.x += 2*gap.x; break;
            case -2: cp.x -= 2*gap.x; break;
            case +4: cp.y -= 2*gap.y; break;
            case -4: cp.y += 2*gap.y; break;
            default: _ASSERT(0); break;
            }
            XY next_start = a[next].GetStart();
            a.insert(a.begin()+original_last+1, Edge(a[original_last].GetEnd(), cp));
            a.insert(a.begin()+original_last+2, Edge(cp, next_start));
        } else {
            a.insert(a.begin()+original_last+1,
                     Edge(a[original_last].GetEnd(), a[next].GetStart()));
        }
    }
}

/** Expand2D the shape.
 *
 * See the contour library definition on what Expand2D means.
 *
 * @param [in] gap The rectangle used for the expansion.
 * @param [out] res The result.
 */
void SimpleContour::Expand2D(const XY &gap, Contour &res) const
{
    if (gap.x==0 && gap.y==0) {
        res = *this;
        return;
    }
    Contour r2;
    if (IsEmpty()) return;
    int first_type, last_type;
    at(0).CreateExpand2D(gap, r2.first.outline.edges, first_type, last_type);
    for (unsigned u=1; u<size(); u++) {
        int stype, etype;
        const unsigned original_last = r2.first.outline.edges.size()-1;
        at(u).CreateExpand2D(gap, r2.first.outline.edges, stype, etype);
        Expand2DHelper(gap, r2.first.outline.edges, original_last, original_last+1, u, last_type, stype);
        last_type = etype;
    }
    //Now meld last and first
    Expand2DHelper(gap, r2.first.outline.edges, r2.first.outline.edges.size()-1, 0, 0, last_type, first_type);

    r2.first.outline.Sanitize(); //calculates clockwise and boundingbox, as well
    if (r2.first.outline.size()==0) return;
    r2.boundingBox = r2.first.outline.boundingBox; //copy bb to outer
    res.Operation(clockwise ? Contour::EXPAND_POSITIVE : Contour::EXPAND_NEGATIVE, std::move(r2));
}


/** Determines the relation of two shapes with no prior assumptions. */
EContourRelationType SimpleContour::RelationTo(const SimpleContour &c) const
{
    if (!boundingBox.Overlaps(c.boundingBox)) return REL_APART;
    //Look for crosspoints
    XY r[4];
    double one_pos[4], two_pos[4];
    for (size_t u1 = 0; u1<size(); u1++)
        for (size_t u2 = 0; u2<c.size(); u2++)
            if (at(u1).Crossing(c.at(u2), r, one_pos, two_pos))
                if (one_pos[0]>0 || two_pos[0]>0) //Crosspoint found, we overlap
                    return REL_OVERLAP;
    //No real crosspoint (only perhaps vertices are equal)
    return CheckContainment(c);
}

/** Draw the shape as a path to a cairo context.
 *
 * @param [in] cr The cairo context.
 * @param [in] show_hidden If false, we skip edges marked not visible.
 */
void SimpleContour::Path(cairo_t *cr, bool show_hidden) const
{
    if (size()==0 || cr==NULL) return;
    bool closed = true;
    cairo_move_to(cr, at(0).GetStart().x, at(0).GetStart().y);
    for (size_t i = 0; i<size(); i++)
        if (show_hidden || at(i).visible)
            at(i).PathTo(cr);
        else {
            cairo_move_to(cr, at(i).GetEnd().x, at(i).GetEnd().y);
            closed = false;
        }
    if (closed)
        cairo_close_path(cr);
}

/** Draw the shape in dashed lines to the path of a cairo context. Needed for backends not supporting dashed lines.
 *
 * We also assume cairo dash is set to continuous: we fake dash here.
 * @param [in] cr The cairo context.
 * @param [in] pattern Contains lengths of alternating on/off segments.
 * @param [in] num contains the number of elements in pattern.
 * @param [in] show_hidden If false, we skip edges marked not visible.
 */

void SimpleContour::PathDashed(cairo_t *cr, const double pattern[], unsigned num, bool show_hidden) const
{
    if (size()==0 || cr==NULL) return;
    if (num<2) {
        Path(cr, show_hidden);
        return;
    }
    double offset = 0;
    int pos = 0;
    for (size_t i = 0; i<size(); i++)
        if (at(i).visible)
            at(i).PathDashed(cr, pattern, num, pos, offset);
}

/** Determine the relative vertical distance between two shapes.
 *
 * This is a helper for OffsetBelow, that is called if the x range of bounding boxes
 * actually overlap and there is a chance of resuling in an offset less than infinity.
 */
double SimpleContour::do_offsetbelow(const SimpleContour &below, double &touchpoint, double offset) const
{
    double tp = 0;
    for (size_t i = 0; i<size(); i++)
        for (size_t j = 0; j<below.size(); j++)
            if (at(i).boundingBox.x.Overlaps(below.at(j).boundingBox.x)) {
                double off = at(i).OffsetBelow(below.at(j), tp);
                if (off < offset) {
                    offset = off;
                    touchpoint = tp;
                }
            }
    return offset;
}

/** Creates a cross-section of the SimpleContour along a vertical line.
 *
 * @param [in] x The x coordinate of the vertical line
 * @param section A DoubleMap containing true for points inside the shape, false for outside. May contain entries already.
 */
void SimpleContour::VerticalCrossSection(double x, DoubleMap<bool> &section) const
{
    double y[2], pos[2];
    bool forward[2];
    for (size_t i=0; i<size(); i++) {
        const int num = at(i).CrossingVertical(x, y, pos, forward);
        for (int j = 0; j<num; j++)
            section.Set(y[j], forward[j] == clockwise);
    }
}

/** Calculates the distance between two shapes by finding their two closest points.
 *
 * @param [in] o The other shape to take the distance from.
 * @param ret We return the distance of the two closest points and the two points themselves.
 *            Distance is negative one is inside the other, zero if partial overlap only (two points equal)
 *            'Inside' here ignores clockwiseness. Distance is `CONTOUR_INFINITY` if one of the shapes is empty.
 *            Note that `ret` can contain the result of previous searches, we update it if we find two points
 *            with a smaller distance (in absolute value).
 *
 * Note that we do not check if the bounding box of the two shapes are further apart than the distance received in
 * `ret`. We assume caller does not call us in that case, but only if needed.
 */
void SimpleContour::Distance(const SimpleContour &o, DistanceType &ret) const
{
    if (IsEmpty() || o.IsEmpty()) return;
    DistanceType running = ret, tmp;
    running.MakeAllOutside();
    //both running and tmp are positive throught this call, except at the very end
    for (unsigned u = 0; u<size(); u++)
        for (unsigned v=0; v<o.size(); v++) {
            if (at(u).GetType() == Edge::STRAIGHT && o[v].GetType() == Edge::STRAIGHT)
                tmp = at(u).Distance(o[v]);
            else if (running.ConsiderBB(fabs(at(u).GetBoundingBox().Distance(o[v].GetBoundingBox()))))
                //if not both are straight try to avoid calculation if bbs are far enough
                tmp = at(u).Distance(o[v]);
            else continue;
            running.Merge(tmp);
            if (running.IsZero()) goto final_merge;
        }
    //now check if one is in the other - they cannot cross each other or else d would be 0
    _ASSERT(RelationTo(o)!=REL_OVERLAP);
    if (IsWithin(o[0].GetStart()) == WI_INSIDE || o.IsWithin(at(0).GetStart()) == WI_INSIDE)
        running.MakeAllInside();
final_merge:
    ret.Merge(running);
}

/** Calculates the distance between a point and us by finding our closest point.
 *
 * @param [in] o The point to take the distance from.
 * @param [out] ret We return the point on our contour closes to `o`.
 * @return The distance, negative if `o` is inside us. `CONTOUR_INFINITY` if we are empty.
 */
double SimpleContour::Distance(const XY &o, XY &ret) const
{
    if (IsEmpty()) return CONTOUR_INFINITY;
    double r = CONTOUR_INFINITY;
    for (unsigned u = 0; u<size(); u++) {
        XY tmp;
        double dummy;
        double d = at(u).Distance(o, tmp, dummy);
        if (d<r) {
            r = d;
            ret = tmp;
            if (d==0) return 0;
        }
    }
    if (IsWithin(o) == WI_INSIDE)
        r = -r;
    return r;
}

/** Calculates the distance between a point and us by finding our closest point and returns two tangent points.
 *
 * Same as Distance(const XY &o, XY &ret), but in addition we return two tangent points
 * from the tangent of the shape at `ret`. See @ref contour for a description of tangents.
 *
 * @param [in] o The point to take the distance from.
 * @param [out] ret We return the point on our contour closes to `o`.
 * @param [out] t1 The forward tangent point.
 * @param [out] t2 The backward tangent point.
 * @return The distance, negative if `o` is inside us. `CONTOUR_INFINITY` if we are empty.
 */
double SimpleContour::DistanceWithTangents(const XY &o, XY &ret, XY &t1, XY &t2) const
{
    if (IsEmpty()) return CONTOUR_INFINITY;
    double r = CONTOUR_INFINITY;
    for (unsigned u = 0; u<size(); u++) {
        XY tmp;
        double _pos;
        double d = at(u).Distance(o, tmp, _pos);
        if (d<r) {
            r = d;
            ret = tmp;
            t1 = PrevTangentPoint(u, _pos);
            t2 = NextTangentPoint(u, _pos);
            if (d==0) return 0;
        }
    }
    if (IsWithin(o) == WI_INSIDE)
        r = -r;
    return r;
}


/** Returns a cut of the shape along an edge.
 *
 * Takes all crosspoints of a (finite length, possibly curvy) edge and the shape
 * and returns the two outermost. We return `pos` values corresponding to the edge,
 * thus both values are between 0 and 1. It is possible that a single value is
 * returned if the edge touches the shape or if its start is inside the shape, but
 * its end is outside. If the edge does not cross or touch the shape, an invalid
 * range is returned.
 */
Range SimpleContour::Cut(const Edge &s) const
{
    Range ret;
    ret.MakeInvalid();
    XY dummy1[2];
    double pos[2], dummy2[2];
    for (unsigned u = 0; u<size(); u++)
        for (int i = at(u).Crossing(s, dummy1, dummy2, pos)-1; i>=0; i--)
            ret += pos[i];
    return ret;
}

/** Returns a cut of the shape along an edge.
 *
 * Same as Cut(const Edge &s), but also returns the coordinates of the positions
 * returned and a forward tangent point for them. In case the edge cuts the shape
 * at a non-smooth vertex, then we take the tangent line of both edges of the vertex
 * average them and return a point from this middle line in the forward direction.
 *
 * @param [in] e The edge to cut with.
 * @param [out] from Values correspond to the position returned in `from`. `first` is the coordinate of the point, `second` is the forward tangent point.
 * @param [out] till Values correspond to the position returned in `till`. `first` is the coordinate of the point, `second` is the forward tangent point.
 * @return The `pos` value corresponding to the two outmost crosspoints.
 */
Range SimpleContour::CutWithTangent(const Edge &e, std::pair<XY, XY> &from, std::pair<XY, XY> &till) const
{
    Range ret;
    ret.MakeInvalid();
    if (!boundingBox.Overlaps(e.GetBoundingBox())) return ret;
    XY point[2];
    double pos[2], scpos[2];
    for (unsigned u = 0; u<size(); u++)
        for (int i = at(u).Crossing(e, point, scpos, pos)-1; i>=0; i--) {
            if (ret.IsWithin(pos[i])) continue;
            XY tangent = at(u).NextTangentPoint(scpos[i]);
            if (scpos[i] == 0) {
                const XY prev_t_norm = (at_prev(u).NextTangentPoint(1.0)- at(u).GetStart()).Normalize();
                const XY tangen_norm = (tangent - at(u).GetStart()).Normalize();
                const XY delta = prev_t_norm+tangen_norm;
                if (delta.length_sqr()<1e-8)
                    tangent.Rotate90CW();
                else
                    tangent = at(u).GetStart() + delta/2;
            }
            if (ret.from > pos[i]) {
                ret.from = pos[i];
                from.first = point[i];
                from.second = tangent;
            }
            if (ret.till < pos[i]) {
                ret.till = pos[i];
                till.first = point[i];
                till.second = tangent;
            }
        }
    return ret;
}


/** Creates a cross-section (or cut) of the SimpleContour along a straight line.
 *
 * We return all crosspoints of an infinite line specified by `A` and `B`.
 * We return them in `pos` terms, that is, at `A` we return 0, at `B` we return 1 and
 * linearly intrapolate and extrapolate between them and outside, respectively.
 *
 * @param [in] A One point of the infinite line.
 * @param [in] B Another point of the infinite line.
 * @param [out] map A DoubleMap containing true for points inside the shape, false for outside.
 */
void SimpleContour::Cut(const XY &A, const XY &B, DoubleMap<bool> &map) const
{
    const Range ret = boundingBox.Cut(A, B); //also tests for A==B or invalid bb, which catches empty "this"
    if (ret.IsInvalid()) return;
    const Edge s(A+(B-A)*ret.from, A+(B-A)*(ret.till+0.1));
    DoubleMap<bool> local;
    XY dummy1[2];
    double pos[2], dummy2[2];
    for (unsigned u = 0; u<size(); u++)
        for (int i = at(u).Crossing(s, dummy1, dummy2, pos)-1; i>=0; i--)
            if (fabs(A.x-B.x) > fabs(A.y-B.y)) {
                const double p = s.GetStart().x + (s.GetEnd().x-s.GetStart().x)*pos[i];
                local.Set((p-A.x)/(B.x-A.x), true);
            } else { //do it in y coordinates
                const double p = s.GetStart().y + (s.GetEnd().y-s.GetStart().y)*pos[i];
                //p is now the y coordinate of the two CP, convert to "pos" on A-B
                local.Set((p-A.y)/(B.y-A.y), true);
            }
    if (local.size()<=1) return;
    for (DoubleMap<bool>::const_iterator i=++local.begin(), j=local.begin(); i!=local.end(); j=i,i++)
        if (inside(IsWithin(A+(B-A)*(i->first+j->first)/2)))
            map.Set(Range(j->first, i->first), clockwise);
}

} //namespace