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
#include <algorithm>
#include <map>
#include <set>
#include <stack>
#include "contour.h"


///////////////////////////// SimpleContour

//Do not create degenerate triangles.
//Always create clockwise.
SimpleContour::SimpleContour(XY a, XY b, XY c)
{
    switch (triangle_dir(a,b,c)) {
    default: //create empty if degenerate triangle
        return;
    case CLOCKWISE:
        push_back(Edge(a, b));
        push_back(Edge(b, c));
        push_back(Edge(c, a));
        break;
    case COUNTERCLOCKWISE:
        push_back(Edge(a, c));
        push_back(Edge(c, b));
        push_back(Edge(b, a));
        break;
    }
    boundingBox.MakeInvalid();
    for (unsigned i=0; i<size(); i++)
        boundingBox += at(i).GetBoundingBox();
    clockwise = true;
}

SimpleContour::SimpleContour(const XY &c, double radius_x, double radius_y, double tilt_deg, double s_deg, double d_deg)
{
    clockwise = true;
    if (radius_y==0) radius_y = radius_x;
    Edge edge(c, radius_x, radius_y, tilt_deg, s_deg, d_deg);
    boundingBox = edge.GetBoundingBox();
    push_back(edge);
    if (edge.GetType()==EDGE_FULL_CIRCLE) return; //full circle
    push_back(Edge(edge.GetEnd(), edge.GetStart()));
}

SimpleContour &SimpleContour::operator =(const Block &b)
{
    clear();
    boundingBox = b;
    if (b.IsInvalid()) return *this;
    push_back(Edge(b.UpperLeft(), XY(b.x.till, b.y.from)));
    push_back(Edge(XY(b.x.till, b.y.from), b.LowerRight()));
    push_back(Edge(b.LowerRight(), XY(b.x.from, b.y.till)));
    push_back(Edge(XY(b.x.from, b.y.till), b.UpperLeft()));
    clockwise = true;
    return *this;
}

void SimpleContour::Invert() 
{
    for (unsigned i=0; i<size(); i++)
        at(i).Invert();
    for (unsigned i=0; i<size()/2; i++)
        std::swap(at(i), at(size()-1-i));
    clockwise = !clockwise;
}

//in p* return the number of vertex or edge we have fallen on if result is such
//if strict is false, any point that is even _close_ to an edge will snap to the edge
//for counterclockwise contours, 'inside' is mean to be the limited space enclosed by
//the contour
is_within_t SimpleContour::IsWithin(XY p, int *edge, double *pos, bool strict) const
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
    unsigned count = 0;
    unsigned e;
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
    int edge;
    for (unsigned i=0; i<size(); i++) {
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
            if (really_between04_warp(one_prev, two_next, two_prev) &&
                really_between04_warp(one_next, two_next, two_prev)) return A_INSIDE_B;
            if (really_between04_warp(two_prev, one_next, one_prev) &&
                really_between04_warp(two_next, one_next, one_prev)) return B_INSIDE_A;
            if (test_equal(one_prev, two_prev) && test_equal(one_next, two_next)) break; //SAME - do another edge
            if (test_equal(one_next, two_prev) && test_equal(one_prev, two_next)) break; //SAME opposite dir - do another edge
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
    //special case of two full ellipses beging the same - not caught otherwise
    if (size()==1 && other.size()==1 && at(0) == other.at(0))
        return SAME;
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

void SimpleContour::CalculateClockwise() 
{
    //determine if this is clockwise.
    if (size()>2) {
        double angles = 0;
        XY prev = at(0).PrevTangentPoint(0, at_prev(0));
        for (unsigned i=0; i<size(); i++)
            if (at(i).GetType() == EDGE_STRAIGHT) {
				_ASSERT(!at(i).GetStart().test_equal(at_next(i).GetStart()));
                angles += angle_degrees(angle(at(i).GetStart(), at_next(i).GetStart(), prev));
                prev = at(i).GetStart();
            } else {
                angles += angle_degrees(angle(at(i).GetStart(), at(i).NextTangentPoint(0, at_next(i)), prev));
                prev = at(i).PrevTangentPoint(1, at_prev(i));
                if (at(i).GetClockWise()) {
                    if (at(i).GetRadianS()<at(i).GetRadianE()) angles -=       (at(i).GetRadianE()-at(i).GetRadianS())*(180./M_PI);
                    else                                       angles -= 360 - (at(i).GetRadianS()-at(i).GetRadianE())*(180./M_PI);
                } else {
                    if (at(i).GetRadianE()<at(i).GetRadianS()) angles +=       (at(i).GetRadianS()-at(i).GetRadianE())*(180./M_PI);
                    else                                       angles += 360 - (at(i).GetRadianE()-at(i).GetRadianS())*(180./M_PI);
                }
            }
            //angle is (n-2)*180 for clockwise, (n+2)*180 for counterclockwise, we draw the line at n*180
            if (angles/180. - floor(angles/180.)*180. >= 1)
                angles = angles;
        clockwise = bool(angles < size()*180);
        return;
    }
    if (size()==2) {
        //if a contour is two edges, it should not be two straigth edges
        if (at(0).GetType() == EDGE_STRAIGHT && at(1).GetType() == EDGE_STRAIGHT) {
            _ASSERT(0);
        }
        if (at(0).GetType() != EDGE_STRAIGHT && at(1).GetType() != EDGE_STRAIGHT) {
            //two curves
            //if they are of same direction we get it
            if (at(0).GetClockWise() == at(1).GetClockWise()) {
                clockwise = at(0).GetClockWise();
                return;
            }
            //two curves with opposite dir, they do not touch only at the two ends
            //the one contains the other decides
            const XY center_line = (at(0).GetStart()+at(1).GetStart())/2;
            const XY center0 = at(0).GetEllipseData().Radian2Point(at(0).GetRadianMidPoint());
            const XY center1 = at(1).GetEllipseData().Radian2Point(at(1).GetRadianMidPoint());
            const double dist0 = (center0 - center_line).length();
            const double dist1 = (center1 - center_line).length();
            clockwise = at(0).GetClockWise() == (dist0 > dist1);
            return;
        }
        //one curve, one straight: dir is decided by curve
        if (at(0).GetType() == EDGE_STRAIGHT)
            clockwise = at(1).GetClockWise();
        else
            clockwise = at(0).GetClockWise();
        return;
    }
    //if (size()==1), a full ellipsis
    clockwise = at(0).GetClockWise();
    return;
}

//Appends an edge.
//Checks that we do not append a zero long edge
//Checks if the edge to append is a direct continuation of the last edge
void SimpleContour::AppendDuringWalk(const Edge &edge)
{
    //if the last point equals to the startpoint of edge, skip the last edge added
    //except if the last edge is a full circle
    if (size()>0 && edge.GetStart().test_equal(rbegin()->GetStart()))
        if (rbegin()->GetType() != EDGE_FULL_CIRCLE)
            pop_back();
    //check if current_xy falls on the same line as the previous two
    if (size()>=2 && at(size()-2).CheckAndCombine(at(size()-1)))
        at(size()-1) = edge; //if so overwrite last
    else
        push_back(edge);
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
    //if the crosspoint we started at was also a vertex, it may be that it is repeated at the end
    if (size()>1 && at(0).GetStart().test_equal(at(size()-1).GetStart()))
        pop_back();
    //Also, the beginning point (at(0]) can fall on an edge
    //if both the last and the first edge are straight and are in-line
    //such as when two side-by-side rectangles are merged
    if (size()>2) {
        if (at(size()-2).CheckAndCombine(at(size()-1)))
            pop_back();
        if (at(size()-1).CheckAndCombine(at(0)))
            erase(begin());
    }
    //also two semi-circles combined should give a single edge (a circle)
    if (size()==2 && at(0).GetType()!=EDGE_STRAIGHT && at(1).GetType()!=EDGE_STRAIGHT)
        if (at(0).CheckAndCombine(at(1)))
            pop_back();

    //Sanity checks
    if (size()>0 && at(0).GetType()==EDGE_STRAIGHT)
        if (size()==1 || (size()==2 && at(1).GetType()==EDGE_STRAIGHT))
            clear();
    if (size()==0) return false;

    //we checked above it is curvy. We assert that this is a full circle
    if (size()==1)
        at(0).SetFullCircle();
    else
        //go around and set "end" value. This was not known previously
        for (unsigned i=0; i<size(); i++)
            at(i).SetEndLiberal(at_next(i).GetStart(), true);
    //compute the bounding box
    for (unsigned i=0; i<size(); i++)
        boundingBox += at(i).GetBoundingBox();
    //compute clockwise
    CalculateClockwise();
    return true;
}

bool SimpleContour::IsSane() const
{
    if (size()==0) return true;
    if (size()==1)
        return at(0).GetType() == EDGE_FULL_CIRCLE;
    for (unsigned u=0; u<size(); u++)
        if (at(u).GetStart().test_equal(at(u).GetEnd()))  //we do not tolerate full circles either
            return false;
    if (size()==2 && at(0).GetType()==EDGE_STRAIGHT && at(1).GetType()==EDGE_STRAIGHT)
        return false;
    return true;
}

//returns true if we were sane.
//returns false if changes were needed
bool SimpleContour::Sanitize()
{
    bool ret = true;
    if (size()==0) return true;
    if (size()==1) {
        if (at(0).GetType() == EDGE_FULL_CIRCLE) return true;
        goto clear;
    }
    for (unsigned u=0; u<size(); /*nope*/)
        if (at(u).GetType()!=EDGE_FULL_CIRCLE && at(u).GetStart().test_equal(at(u).GetEnd())) {
            erase(begin()+u);
            ret = false;
        } else
            u++;
    switch (size()) {
    case 0: return false;
    case 1:
        if (at(0).GetType() == EDGE_FULL_CIRCLE) return true;
        goto clear;
    case 2:
        if (at(0).GetType()==EDGE_STRAIGHT && at(1).GetType()==EDGE_STRAIGHT)
            goto clear;
        /*fallthrough*/
    default:
        //ensure we have no FULL_CIRCLE and edges connect
        for (unsigned u=0; u<size(); u++)
            if (at(u).GetType()==EDGE_FULL_CIRCLE)
                goto clear;
            else if (!at(u).GetEnd().test_equal(at_next(u).GetStart()))
                goto clear;
    }
    CalculateBoundingBox();
    return ret;
clear:
    clear();
    return false;
}


const Block &SimpleContour::CalculateBoundingBox()
{
    boundingBox.MakeInvalid();
    for(unsigned i=0; i<size(); i++)
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
            ret.pop_back();
        else         //set the last edge to straight
            ret[size()-1] = Edge(ret[size()-1].GetStart(), edge.GetStart());
    }
    //insert edge
    ret.push_back(edge);
    int num_to_check = 1;
    //if edge is curvy and does not end in at(0).start, insert a straight edge afterwards
    if (edge.GetType()!=EDGE_STRAIGHT) {
        if (!edge.GetEnd().test_equal(ret[0].GetStart())) {
            ret.push_back(Edge(edge.GetEnd(), ret[0].GetStart()));
            //check if this edge to insert do not cross the previously inserted edge
            if (ret[ret.size()-2].Crossing(ret[ret.size()-1], dum1, dum2, dum2))
                return false;
            num_to_check = 2;
        }
    }
    //now check if inserted edges cross any of the ones before
    //check if edge->end is crossing any existing edges
    ret.CalculateBoundingBox(); //needed by crossing
    for (unsigned i = 0; i<ret.size()-num_to_check-1; i++)
        for (unsigned j = ret.size()-num_to_check-1; j<ret.size(); j++)
            if (ret[i].Crossing(ret[j], dum1, dum2, dum2))
                return false;
    //OK, we can have these edges inserted
    swap(ret);
    return true;
}

//////////////////////////////////SimpleContour::Expand implementation


void SimpleContour::Expand(EExpandType type, double gap, Contour &res) const
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
        _ASSERT(at(0).GetType()==EDGE_FULL_CIRCLE);
        if (!r[0].Expand(gap)) return; //full circle disappeared, we return empty "res"
        res = std::move(r);
        return;
    }

    //Expand all the edges
    for (unsigned i = 0; i<r.size(); /*nope*/) 
        if (!r[i].Expand(gap)) {  //circles that disappear are erased
            r.erase(r.begin()+i);
            o.erase(o.begin()+i);
        } else
            i++;

    //Now find how and where expanded edges meet
    std::vector<EdgeArc::EExpandCPType> cross_type (r.size());
    std::vector<XY>                     cross_point(r.size());
    for (unsigned i = 0; i<r.size(); i++)
        cross_type[i] = r[i].FindExpandedEdgesCP(r.at_next(i), o[i].GetStart(), cross_point[i]);

    //OK, now adjust the edges and/or insert additional ones
    Contour r2;
    r2.reserve(size()*3);
    switch (type) {
    default:
        _ASSERT(0);
    case EXPAND_MITER:
        for (unsigned i = 0; i<r.size(); i++) {
            XY new_start = r[i].GetStart();
            XY new_end =   r[i].GetEnd();
            switch (cross_type[r.prev(i)]) {
            case EdgeArc::CP_REAL:           
            case EdgeArc::CP_EXTENDED:       
            case EdgeArc::CP_ADD_LINE_ME:    new_start = cross_point[r.prev(i)]; break;
            case EdgeArc::SAME_ELLIPSIS:     _ASSERT(0); /*fallthrough*/
            case EdgeArc::PARALLEL_LINES:    
            case EdgeArc::CP_ADD_LINE_OTHER:
            case EdgeArc::CP_ADD_LINE_BOTH:
            case EdgeArc::NO_CP_ADD_LINE:    break; /*list all values to avoid warnings*/
            }
            switch (cross_type[i]) {
            case EdgeArc::CP_REAL:           
            case EdgeArc::CP_EXTENDED:       
            case EdgeArc::CP_ADD_LINE_OTHER: new_end = cross_point[i]; break;
            case EdgeArc::SAME_ELLIPSIS: 
            case EdgeArc::PARALLEL_LINES:    _ASSERT(0); /*fallthrough*/
            case EdgeArc::CP_ADD_LINE_ME:
            case EdgeArc::CP_ADD_LINE_BOTH:
            case EdgeArc::NO_CP_ADD_LINE:    break; /*list all values to avoid warnings*/
            }
            r[i].SetStartEndForExpand(new_start, new_end);
            r2.push_back(r[i]);
            //Now see if we need to add a line
            switch (cross_type[i]) {
            case EdgeArc::NO_CP_ADD_LINE:    
            case EdgeArc::PARALLEL_LINES:    
                r2.push_back(EdgeArc(new_end, r.at_next(i).GetStart())); 
                break;
            case EdgeArc::CP_ADD_LINE_ME:    
                r2.push_back(EdgeArc(new_end, cross_point[i])); 
                break;
            case EdgeArc::CP_ADD_LINE_BOTH:  
                r2.push_back(EdgeArc(new_end, cross_point[i]));
                r2.push_back(EdgeArc(cross_point[i], r.at_next(i).GetStart())); 
                break;
            case EdgeArc::SAME_ELLIPSIS: 
                _ASSERT(0); /*fallthrough*/
            case EdgeArc::CP_REAL:           
            case EdgeArc::CP_EXTENDED:       
            case EdgeArc::CP_ADD_LINE_OTHER: 
                break; /*list all values to avoid warnings*/
            }
        }
        break;
    case EXPAND_BEVEL:
    case EXPAND_ROUND:
        for (unsigned i = 0; i<r.size(); i++) {
            XY new_start = r[i].GetStart();
            XY new_end =   r[i].GetEnd();
            if (cross_type[r.prev(i)] == EdgeArc::CP_REAL)
                new_start = cross_point[r.prev(i)]; 
            if (cross_type[i] == EdgeArc::CP_REAL)
                new_end = cross_point[i]; 
            r[i].SetStartEndForExpand(new_start, new_end);
            r2.push_back(r[i]);
            if (cross_type[i] == EdgeArc::CP_REAL) continue; //no need to insert
            if (type==EXPAND_BEVEL) 
                r2.push_back(Edge(new_end, r.at_next(i).GetStart()));  //insert line
            else {
                r2.push_back(Edge(o[i].GetEnd(), (new_end-o[i].GetEnd()).length()));  //insert circle
                r2.rbegin()->SetStartLiberal(new_end);  //these keep clockwise
                r2.rbegin()->SetEndLiberal(r.at_next(i).GetStart());
            }
        }
        break;
    }
//    _ASSERT(r2.IsSane());
    r2.Sanitize();
    if (r2.size()==0) return;
    if (r2.size()==1) 
        r2[0].SetFullCircle();
    //OK, now untangle
    r2.CalculateBoundingBox();  //also calculates bounding boxes of edges
    if (r2.size()==1) {
        res = std::move(r2);
        return;
    }
    res.Operation(clockwise ? Contour::EXPAND_POSITIVE : Contour::EXPAND_NEGATIVE, std::move(r2));
}

SimpleContour SimpleContour::CreateWithLastEdge(unsigned i) const
{
    //if i is degenerate or already the last edge, return a copy of us
    if (size()==0 || i>=size()-1) return *this;
    SimpleContour ret;
    ret.reserve(size());
    for (unsigned j = 0; j<size(); j++)
        ret.push_back(at((i+1+j)%size()));
    ret.boundingBox = boundingBox;
    return ret;
}


void SimpleContour::Path(cairo_t *cr) const
{
    if (size()==0 || cr==NULL) return;
    cairo_move_to(cr, at(0).GetStart().x, at(0).GetStart().y);
    for (unsigned i = 0; i<size(); i++)
        at(i).Path(cr);
    cairo_close_path(cr);
}

//Do not close the path or include the last edge (even if curvy)
void SimpleContour::PathOpen(cairo_t *cr) const
{
    if (size()==0 || cr==NULL) return;
    cairo_move_to(cr, at(0).GetStart().x, at(0).GetStart().y);
    for (unsigned i = 0; i<size()-1; i++)
        at(i).Path(cr);
}

double SimpleContour::do_offsetbelow(const SimpleContour &below, double &touchpoint) const
{
    double offset = CONTOUR_INFINITY;
    for (unsigned i = 0; i<size(); i++)
        for (unsigned j = 0; j<below.size(); j++)
            if (at(i).boundingBox.x.Overlaps(below.at(j).boundingBox.x)) {
                double tp, off = at(i).OffsetBelow(below.at(j), tp);
                if (off < offset) {
                    touchpoint = tp;
                    offset = off;
                }
            }
    return offset;
}

void SimpleContour::DoVerticalCrossSection(double x, DoubleMap<bool> &section) const
{
    double y[2], pos[2];
    bool forward[2];
    for (unsigned i=0; i<size(); i++) {
        const int num = at(i).CrossingVertical(x, y, pos, forward);
        for (int j = 0; j<num; j++)
            section.Set(y[j], forward[j] == clockwise);
    }
}
