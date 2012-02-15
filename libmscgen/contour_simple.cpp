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
    for (size_type i=0; i<size(); i++)
        boundingBox += at(i).GetBoundingBox();
    clockwise = true;
    area_cache.first=false;
}

SimpleContour::SimpleContour(const XY &c, double radius_x, double radius_y, double tilt_deg, double s_deg, double d_deg)
{
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

void SimpleContour::Invert()
{
    for (size_type i=0; i<size(); i++)
        at(i).Invert();
    for (size_type i=0; i<size()/2; i++)
        std::swap(at(i), at(size()-1-i));
    clockwise = !clockwise;
    area_cache.second = -area_cache.second;
}

//in p* return the number of vertex or edge we have fallen on if result is such
//if strict is false, any point that is even _close_ to an edge will snap to the edge
//for counterclockwise contours, 'inside' is mean to be the limited space enclosed by
//the contour
is_within_t SimpleContour::IsWithin(XY p, size_type *edge, double *pos, bool strict) const
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
    size_type count = 0;
    size_type e;
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

//Can result SAME, APRT, A_INSIDE_B or B_INSIDE_A
//It can also return OVERLAP, which means one of our point is outside b, can either be APART or b may be in us
SimpleContour::result_t SimpleContour::CheckContainmentHelper(const SimpleContour &b) const
{
    size_type edge;
    for (size_type i=0; i<size(); i++) {
        double pos;
        const XY p = at(i).GetStart();
        //if we are a single ellipsis, use our center point, else use a vertex
        switch (b.IsWithin(p, &edge, &pos, /*strict=*/false)) {
        default:
            _ASSERT(0);
        case WI_INSIDE:  return A_INSIDE_B;
        case WI_OUTSIDE: return OVERLAP;
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
                really_between04_warp(one_next, two_next, two_prev)) return A_INSIDE_B;
            if (really_between04_warp(two_prev, one_next, one_prev) &&
                really_between04_warp(two_next, one_next, one_prev)) return B_INSIDE_A;
            return APART;
        }
    }
    //All points were on a vertex equal in one of the directions
    return SAME;
}


//Gives valid result only if the two contours have no crosspoints
//clockwiseness fully honored
SimpleContour::result_t SimpleContour::CheckContainment(const SimpleContour &other) const
{
    //special case of two full ellipses touching - not caught otherwise
    if (size()==1 && other.size()==1 && at(0).GetStart() == other.at(0).GetStart()) {
        if (at(0).ell==other[0].ell) return SAME;
        //if one of the centers is in the other ellipses then x_INSIDE_y
        if (inside(IsWithin(other[0].ell.GetCenter())) ||
            inside(other.IsWithin(at(0).ell.GetCenter()))) {
                //the center closer to the touchpoint will be inside
                if ((at(0).ell.GetCenter()-at(0).GetStart()).length() <
                    (other[0].ell.GetCenter()-at(0).GetStart()).length())
                    return A_INSIDE_B;
                else
                    return B_INSIDE_A;
        }
        return APART;
    }
    result_t this_in_other = CheckContainmentHelper(other);
    if (this_in_other != OVERLAP) return this_in_other;
    switch (other.CheckContainmentHelper(*this)) {
    default:
    case SAME: _ASSERT(0); return SAME;
    case OVERLAP:
    case APART:       return APART;
    case A_INSIDE_B:  return B_INSIDE_A; //other is in us
    case B_INSIDE_A:  return A_INSIDE_B; //we are in other
    }
}

//void SimpleContour::CalculateClockwise()
//{
//    //determine if this is clockwise.
//    if (size()>2) {
//        double angles = 0;
//        XY prev = PrevTangentPoint(0, 0.0);
//        for (size_type i=0; i<size(); i++)
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

//Appends an edge.
//Checks that we do not append a zero long edge
//Checks if the edge to append is a direct continuation of the last edge
//WE DO NOT UPDATE area_cache, clockwise, bounding box
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


//Clean up the contour after a walk
//-Check repeated points at the end
//-Set "e" values for curvy edges
//-Calculate edge and contour bounding boxes
//Also, do a lot of sanity checks:
//-a contour with one edge must be a full ellipse. If it is straight, we clear()
//-a contour with two edges must have at least one curvy edge, if not, we clear()
//returns true if contour is OK
bool SimpleContour::PostWalk()
{
    boundingBox.MakeInvalid();
    area_cache.first = false;

    //if the crosspoint we started at was also a vertex, it may be that it is repeated at the end
    if (size()>1 && at(0).GetStart().test_equal(at(size()-1).GetStart()))
        edges.pop_back();

    //set "end" values. Not known previously
    for (size_type i=0; i<size(); i++)
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
    for (size_type i=0; i<v.size(); i++)
        edges.push_back(Edge(v[i], v[(i+1)%v.size()]));
    Sanitize();  //includes CalculateBoundingBox() and CalculateClockwise()
}

void SimpleContour::assign_dont_check(const XY v[], size_type size)
{
    clear();
    if (size < 2) return;
    for (size_type i=0; i<size; i++)
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

void SimpleContour::assign_dont_check(const Edge v[], size_type size)
{
    clear();
    if (size < 2) return;
    for (size_type i=0; i<size; i++)
        edges.push_back(v[i]);
    Sanitize();  //includes includes CalculateBoundingBox() and CalculateClockwise()
}

bool SimpleContour::IsSane() const
{
    if (size()==0) return true;
    if (size()==1)
        return at(0).GetType() == Edge::FULL_CIRCLE;
    for (size_type u=0; u<size(); u++) {
        if (at(u).GetStart().test_equal(at(u).GetEnd()) &&
            at(u).GetType() != Edge::FULL_CIRCLE)
            return false;
        if (!at(u).IsSane()) return false;
    }
    if (size()==2 && at(0).GetType()==Edge::STRAIGHT && at(1).GetType()==Edge::STRAIGHT)
        return false;
    return true;
}

//returns true if we were sane.
//returns false if changes were needed
bool SimpleContour::Sanitize()
{
    area_cache.first=false;
    bool ret = true;
    if (size()==0) return true;
    if (size()==1) {
        if (at(0).GetType() == Edge::FULL_CIRCLE) return true;
        goto clear;
    }
    for (size_type u=0; u<size(); /*nope*/)
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
        for (size_type u=0; u<size(); u++)
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


const Block &SimpleContour::CalculateBoundingBox()
{
    boundingBox.MakeInvalid();
    for(size_type i=0; i<size(); i++)
        boundingBox += at(i).CalculateBoundingBox();
    return boundingBox;
}

//If the edge to insert starts at the last point, we replace the last edge to it.
//(makes sense if we replace a straight edge to a curve or vice versa)
//If the last edge is a curve, we anyway replace the edge to be inserted.
//If we insert a curve, which does not
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
    size_type num_to_check = 1;
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
    for (size_type i = 0; i<ret.size()-num_to_check-1; i++)
        for (size_type j = ret.size()-num_to_check-1; j<ret.size(); j++)
            if (ret[i].Crossing(ret[j], dum1, dum2, dum2))
                return false;
    //OK, we can have these edges inserted
    swap(ret);
    area_cache.first=false;
    return true;
}

//This is based on http://www.mathopenref.com/coordpolygonarea.html
//For each edge, we calculate the area above it (the area between the edge and Y axis,
//but since y grows downward, the area is "above" the edge).
//The value is signed. If the edge is from right to left it is positive, else negative.
//Since (for straight edges) the area would contain "start.x*start.y-end.x*end.y",
//which will cancel out. Plus it is easier to return twice the area.
//Thus we request edge to return
//"2*(area_above_edge - start.x*start.y + end.x*end.y)", this is what 
//Edge::GetAreaAboveAdjusted() returns;
double SimpleContour::CalcualteArea() const
{
    register double ret = 0;
    for (size_type i=0; i<size(); i++)
        ret += at(i).GetAreaAboveAdjusted();
    area_cache.first = true; 
    return area_cache.second = ret /2;
}

double SimpleContour::GetCircumference(bool include_hidden) const
{
    register double ret = 0;
    for (size_type i=0; i<size(); i++)
        if (include_hidden || at(i).visible)
            ret += at(i).GetLength();
    return ret;
}

XY SimpleContour::CentroidUpscaled() const
{
    XY ret (0,0);
    for (size_type i=0; i<size(); i++)
            ret += at(i).GetCentroidAreaAboveUpscaled();
    return ret;
}



//////////////////////////////////SimpleContour::Expand implementation

Edge SimpleContour::CreateRoundForExpand(const XY &start, const XY &end, const XY& old, bool clockwise)
{
    XY center = old;
    double radius = (start-old).length();
    if (!test_equal(radius, (end-old).length())) {
        center = (start+end)/2;
        radius = (start-center).length();
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

//miter_limit tells us that with EXPAND_MITER_* how long the miter
//edges can be in terms of the "gap"
//E.g., miter_limit=2 says they can be as long as 2*gap
//the rest is cut away with a bevel-like edge
void SimpleContour::Expand(EExpandType type, double gap, Contour &res, double miter_limit) const
{
    if (gap==0) {
        res = *this;
        return;
    }
    const double shrink_by = clockwise ? -gap : gap;
    if (size()==0 || boundingBox.x.Spans() < 2*shrink_by || boundingBox.y.Spans() < 2*shrink_by)
        return;

    SimpleContour r(*this);
    SimpleContour o(*this);
    if (size()==1) {
        _ASSERT(at(0).GetType()==Edge::FULL_CIRCLE);
        if (!r[0].Expand(gap)) return; //full circle disappeared, we return empty "res"
        r.CalculateBoundingBox();
        r.area_cache.first = false;
        res = std::move(r);
        return;
    }

    //Expand all the edges
    for (size_type i = 0; i<r.size(); i++)
        if (!r[i].Expand(gap)) {  //circles that disappear are replaced with straight lines
            r[i] = o[i];
            r[i].type = Edge::STRAIGHT;
            r[i].Expand(gap);
        }

    if (r.size()==0) return;
    if (r.size()==1 && r[0].GetType()==Edge::STRAIGHT)
        return;
    if (r.size()==2 && r[0].GetType()==Edge::STRAIGHT && r[1].GetType()==Edge::STRAIGHT)
        return;


    //Now find how and where expanded edges meet and how
    std::vector<Edge::EExpandCPType> cross_type (r.size());
    std::vector<XY>                     cross_point(r.size());
    for (size_type i = 0; i<r.size(); i++) {
        cross_type[i] = r[i].FindExpandedEdgesCP(r.at_next(i), cross_point[i]);
        _ASSERT(cross_type[i] != Edge::DEGENERATE);
    }

    //now kill the ones that changed direction
    if (type == EXPAND_MITER ||
        type == EXPAND_MITER_ROUND ||
        type == EXPAND_MITER_BEVEL ||
        type == EXPAND_MITER_SQUARE) {   //for bevel and round we check this after inserting bevel & round
        size_type prev_size;
        do {
            prev_size = r.size();
            XY tmp_point;
            Edge::EExpandCPType tmp_type;
            //Kill edges if they have good crosspoints with both prev and next
            //.. and if the new endpoins that we will use below are in reverse order
            //     than the originals
            //.. and the prev has a good cp with next
            for (size_type i = 0; i<r.size(); /*nope*/) {
                if (Edge::HasCP(cross_type[r.prev(i)]) && Edge::HasCP(cross_type[i]) &&
                    r[i].IsOpposite(cross_point[r.prev(i)], cross_point[i])) {
                    tmp_type = r.at_prev(i).FindExpandedEdgesCP(r.at_next(i), tmp_point);
                    if (Edge::HasCP(tmp_type)) {
                        cross_type[r.prev(i)] = tmp_type;
                        cross_point[r.prev(i)]= tmp_point;
                        r.edges.erase(r.edges.begin()+i);
                        o.edges.erase(o.edges.begin()+i);
                        cross_type.erase(cross_type.begin()+i);
                        cross_point.erase(cross_point.begin()+i);
                        continue;
                    }
                }
                i++;
            }
        } while (prev_size != r.size());
    }

    //OK, now adjust the edges and/or insert additional ones
    Contour res_before_untangle;
    SimpleContour &r2 = res_before_untangle.first.outline;
    r2.edges.reserve(size()*3);
    const double gap_limit = fabs(miter_limit) < DBL_MAX ? fabs(gap)*fabs(miter_limit) : DBL_MAX;
    switch (type) {
    default:
        _ASSERT(0);
    case EXPAND_MITER:
    case EXPAND_MITER_ROUND:
    case EXPAND_MITER_BEVEL:
    case EXPAND_MITER_SQUARE:
        for (size_type i = 0; i<r.size(); i++) {
            XY new_start = Edge::HasCP(cross_type[r.prev(i)]) ? cross_point[r.prev(i)] : r[i].GetStart();
            XY new_end   = Edge::HasCP(cross_type[i])         ? cross_point[i]         : r[i].GetEnd();

            //if we are a too sharp miter, we limit its length
            if (cross_type[r.prev(i)] == Edge::CP_EXTENDED &&
                   (new_start-r[i].GetStart()).length() > gap_limit)
                new_start = r[i].GetStart() + (new_start-r[i].GetStart()).Normalize()*gap_limit;
            bool need_miter_limit_bevel = false;
            if (cross_type[i] == Edge::CP_EXTENDED &&
                   (new_end-r[i].GetEnd()).length() > gap_limit) {
                new_end = r[i].GetEnd() + (new_end-r[i].GetEnd()).Normalize()*gap_limit;
                need_miter_limit_bevel = true;
            }
            _ASSERT(new_start.length() < 10000);
            _ASSERT(new_end.length() < 10000);

            if (!new_start.test_equal(new_end)) {
                //insert existin edge (updated)
                Edge tmp = r[i];
                tmp.SetStartEndForExpand(new_start, new_end);
                r2.edges.push_back(tmp);
            } else {
                //existing edge got degenerate, do not insert
                cross_point[i] = new_end = new_start; //use this from now on
            }
            if (Edge::HasCP(cross_type[i])) {
                if (need_miter_limit_bevel) {
                    const XY bevel_end = r.at_next(i).GetStart() +
                        (cross_point[i]-r.at_next(i).GetStart()).Normalize()*gap_limit;
                    _ASSERT(bevel_end.length() < 10000);
                    if (!bevel_end.test_equal(new_end))
                        r2.edges.push_back(Edge(new_end, bevel_end));
                }
            } else
                //no natural crosspoint: see if we need to add a line, circle or square
                switch (type) {
                default:
                    _ASSERT(0);
                case EXPAND_MITER:
                    r2.edges.push_back(Edge(new_end, cross_point[i]));
                    r2.edges.push_back(Edge(cross_point[i], r.at_next(i).GetStart()));
                    break;
                case EXPAND_MITER_ROUND:
                    r2.edges.push_back(CreateRoundForExpand(new_end, r.at_next(i).GetStart(),
                        o[i].GetEnd(), clockwise ^ (gap<0)));
                    break;
                case EXPAND_MITER_BEVEL:
                    r2.edges.push_back(Edge(new_end, r.at_next(i).GetStart()));
                    break;
                case EXPAND_MITER_SQUARE:
                    const XY &next_start = r.at_next(i).GetStart();
                    const double dist = (new_end - next_start).length()/2;
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
        break; //EXPAND_MITER_*
    case EXPAND_BEVEL:
    case EXPAND_ROUND:
        enum EEdgeType {EXISTING, EXISTING_CHANGED_DIR, INSERTED};
        std::vector<EEdgeType> edge_type;  edge_type.reserve(200);
        std::vector<size_type> edge_orig;        edge_orig.reserve(200);
        for (size_type i = 0; i<r.size(); i++) {
            XY new_start = cross_type[r.prev(i)] == Edge::CP_REAL ? cross_point[r.prev(i)] : r[i].GetStart();
            XY new_end =   cross_type[i] == Edge::CP_REAL         ? cross_point[i]         : r[i].GetEnd();

            if (!new_start.test_equal(new_end)) {
                const bool changed_dir = r[i].IsOpposite(new_start, new_end);
                r[i].SetStartEndForExpand(new_start, new_end);
                r2.edges.push_back(r[i]);
                edge_type.push_back(changed_dir ? EXISTING_CHANGED_DIR : EXISTING);
                edge_orig.push_back(i);
            } else {
                //existing edge got degenerate
                cross_point[i] = new_end = new_start; //use this from now on
            }
            if (cross_type[i] == Edge::CP_REAL) continue; //no need to insert
            edge_type.push_back(INSERTED);
            edge_orig.push_back(size_type(-1));
            if (type==EXPAND_BEVEL ||
                o[i].GetEnd() != o.at_next(i).GetStart()) //a missing edge
                r2.edges.push_back(Edge(new_end, r.at_next(i).GetStart()));  //insert line
            else
                r2.edges.push_back(CreateRoundForExpand(new_end, r.at_next(i).GetStart(),
                                                  o[i].GetEnd(), gap>0));  //circle
        }
        //Now check all inserted edge.
        //If any neighbouring original edge has changed direction and the
        //inserted edge has a good cp with the edge beyond the neighbour that
        //changed dir, skip the neighbour that changed dir.
        bool all_orig_changed_dir = true;
        for (size_type u = 0; u<r2.size(); u++) {
            if (edge_type[u]!=INSERTED) {
                if (edge_type[u]!=EXISTING_CHANGED_DIR)
                    all_orig_changed_dir = false;
                continue;
            }
            XY tmp_point;
            Edge::EExpandCPType tmp_type;
            //first check out the original edge before the inserted one
            const size_type prev = r2.prev(u);
            if (edge_type[prev] == EXISTING_CHANGED_DIR) {
                const size_type prevprev = r2.prev(prev);
                tmp_type = r2[prevprev].FindExpandedEdgesCP(r2[u], tmp_point);
                if (tmp_type == Edge::CP_REAL || tmp_type==Edge::CP_EXTENDED) {
                    //set the inserted bevel/round to end in cp
                    r2[u].SetStartLiberal(tmp_point);
                    r2[prevprev].SetEndLiberal(tmp_point);
                    //erase "prev"
                    r2.edges.erase(r2.edges.begin()+prev);
                    edge_type.erase(edge_type.begin()+prev);
                    edge_orig.erase(edge_orig.begin()+prev);
                    //update "u"
                    if (prev<u) u--; //this way u was >=1, can decrement
                    //else u==0 && prev==size()-1 => "u" can remain
                }
            }
            //now do the same for the original edge *after* the inserted one
            const size_type next = r2.next(u);
            if (edge_type[next] == EXISTING_CHANGED_DIR) {
                const size_type nextnext = r2.next(next);
                tmp_type = r2[u].FindExpandedEdgesCP(r2[nextnext], tmp_point);
                if (tmp_type == Edge::CP_REAL || tmp_type==Edge::CP_EXTENDED) {
                    //set the inserted bevel/round to end in cp
                    r2[u].SetEndLiberal(tmp_point);
                    r2[nextnext].SetStartLiberal(tmp_point);
                    //erase "next"
                    r2.edges.erase(r2.edges.begin()+next);
                    edge_type.erase(edge_type.begin()+next);
                    edge_orig.erase(edge_orig.begin()+next);
                    //update "u"
                    if (next<u) u--; //this way u was >=1, can decrement
                }
            }
        }
        if (all_orig_changed_dir) return;
    }
    r2.Sanitize(); //calculates clockwise and boundingbox, as well
    if (r2.size()==0) return;
    if (r2.size()==1) {
        r2[0].SetFullCircle();
        res_before_untangle.boundingBox = r2.CalculateBoundingBox();  //also calculates bounding boxes of edges
        res = std::move(res_before_untangle);
        return;
    }
    //OK, now untangle - use 'res_before_untangle' instead of r2    
    res_before_untangle.boundingBox = r2.boundingBox; //copy bb to outer
    res.Operation(clockwise ? Contour::EXPAND_POSITIVE : Contour::EXPAND_NEGATIVE, std::move(res_before_untangle));
}

void SimpleContour::Path(cairo_t *cr, bool show_hidden) const
{
    if (size()==0 || cr==NULL) return;
    bool closed = true;
    cairo_move_to(cr, at(0).GetStart().x, at(0).GetStart().y);
    for (size_type i = 0; i<size(); i++)
        if (show_hidden || at(i).visible)
            at(i).PathTo(cr);
        else {
            cairo_move_to(cr, at(i).GetEnd().x, at(i).GetEnd().y);
            closed = false;
        }
    if (closed)
        cairo_close_path(cr);
}

void SimpleContour::PathDashed(cairo_t *cr, const double pattern[], unsigned num, bool show_hidden) const
{
    if (size()==0 || cr==NULL) return;
    if (num<2) {
        Path(cr, show_hidden);
        return;
    }
    double offset = 0;
    int pos = 0;
    for (size_type i = 0; i<size(); i++)
        if (at(i).visible)
            at(i).PathDashed(cr, pattern, num, pos, offset);
}

//True for all of the OffsetBelow() functions (except Edge:: ones):
//if the "offset" received is not changed (did not find a smaller one), "touchpoint" is also
//left unchanged
double SimpleContour::do_offsetbelow(const SimpleContour &below, double &touchpoint, double offset) const
{
    double tp = 0;
    for (size_type i = 0; i<size(); i++)
        for (size_type j = 0; j<below.size(); j++)
            if (at(i).boundingBox.x.Overlaps(below.at(j).boundingBox.x)) {
                double off = at(i).OffsetBelow(below.at(j), tp);
                if (off < offset) {
                    offset = off;
                    touchpoint = tp;
                }
            }
    return offset;
}

void SimpleContour::VerticalCrossSection(double x, DoubleMap<bool> &section) const
{
    double y[2], pos[2];
    bool forward[2];
    for (size_type i=0; i<size(); i++) {
        const int num = at(i).CrossingVertical(x, y, pos, forward);
        for (int j = 0; j<num; j++)
            section.Set(y[j], forward[j] == clockwise);
    }
}

} //namespace