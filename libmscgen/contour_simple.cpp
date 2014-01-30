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

int expand_debug=false;

///////////////////////////// SimpleContour

//Do not create degenerate triangles.
//Always create clockwise.
SimpleContour::SimpleContour(XY a, XY b, XY c) :
    clockwise_fresh(true), area_fresh(false), boundingBox_fresh(true),
    clockwise(true), boundingBox(a, b)
{
    switch (triangle_dir(a,b,c)) {
    default: //create empty if degenerate triangle
        boundingBox.MakeInvalid();
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
    boundingBox += c;
}

/** Create an ellipse (or ellipse slice) shape.
 * If not a full ellipse/circle a line segment is added as _the last edge_ to close it.
 *
 * @param [in] c Centerpoint
 * @param [in] radius_x Radius in the x direction. Its absolute value is used.
 * @param [in] radius_y Radius in the y direction (same as `radius_x` if omitted = circle) Its absolute value is used.
 * @param [in] tilt_deg The tilt of the ellipse in degrees. 0 if omitted.
 * @param [in] s_deg The startpoint of the arc.
 * @param [in] d_deg The endpoint of the arc. If equal to `s_deg` a full ellipse results, else a straight line is added to close the arc.
 * If `radius_x` is zero, we return an empty shape. If `radius_y` is zero, we assume it to be the same as `radius_x` (circle).
*/
SimpleContour::SimpleContour(const XY &c, double radius_x, double radius_y, double tilt_deg, double s_deg, double d_deg) :
   clockwise_fresh(true), area_fresh(false), boundingBox_fresh(false),
   clockwise(true)
{
    if (radius_x==0) {
        area_fresh = true;
        boundingBox_fresh = true;
        area = 0;
        boundingBox.MakeInvalid();
        return;
    }
    Edge::GenerateEllipse(edges, c, radius_x, radius_y, tilt_deg, s_deg, d_deg);
    //Add a segment if not a full ellipse
    const XY start = edges.front().start;
    const XY end = edges.back().end;
    if (!start.test_equal(end))
        edges.emplace_back(end, start);
}

/** Clear the shape and assign a rectangle shape. */
SimpleContour &SimpleContour::operator =(const Block &b)
{
    clear(); //clockwise is set to fresh and true here, bounding box & area is set to fresh
    if (b.IsInvalid() || !test_smaller(b.x.from, b.x.till) || !test_smaller(b.y.from, b.y.till)) return *this;
    edges.push_back(Edge(b.UpperLeft(), XY(b.x.till, b.y.from)));
    edges.push_back(Edge(XY(b.x.till, b.y.from), b.LowerRight()));
    edges.push_back(Edge(b.LowerRight(), XY(b.x.from, b.y.till)));
    edges.push_back(Edge(XY(b.x.from, b.y.till), b.UpperLeft()));
    area = b.x.Spans()*b.y.Spans();
    boundingBox = b;
    return *this;
}

/** Revert the clockwisedness of the shape by reverting, all edges and their order. */
void SimpleContour::Invert()
{
    for (size_t i=0; i<size(); i++)
        at(i).Invert();
    for (size_t i=0; i<size()/2; i++)
        std::swap(at(i), at(size()-1-i));
    area *= -1;
    clockwise = !clockwise;
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
    if (size()==0 || GetBoundingBox().IsWithin(p)==WI_OUTSIDE) return WI_OUTSIDE;

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
        if (at(e).GetStart().test_equal(p)) 
            return WI_ON_VERTEX;
        double y[3], po[3];
        int forward[3];
        int num = at(e).CrossingVertical(p.x, y, po, forward);

        if (num==-1) {
            if (test_equal(at(e).GetEnd().y, p.y)) {
                if (edge) *edge = next(e);
                return WI_ON_VERTEX; //on vertex
            }
            //Equality to at(e).GetStart is already tested above
            //now test if we are in-between 
            if ((p.y < at(e).GetStart().y && at(e).GetEnd().y < p.y) ||
                (at(e).GetStart().y < p.y && p.y < at(e).GetEnd().y)) {
                if (pos) *pos = (p.y-at(e).GetStart().y)/(at(e).GetEnd().y-at(e).GetStart().y);
                return WI_ON_EDGE; //goes through p
            }
            //if we are not between we just continue
            continue;
        }

        for (unsigned u = 0; u<num; u++) {
            if ((strict && y[u] == p.y) ||  //on an edge, we are _not_ approximate here
                (!strict&& test_equal(y[u], p.y))) {  //on an edge, we are approximate here
                //we have tested that at(e).start is not equal to p, so no need to test for that here
                if (test_equal(at(e).GetEnd().x, p.x)) {
                    if (edge) *edge = next(e);
                    return WI_ON_VERTEX;
                } else {
                    if (pos) *pos = po[1];
                    return WI_ON_EDGE; // on an edge, but not vertex
                }
            }
            if (forward[u]!=0 && y[u] > p.y) 
                count++;
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
            if (!  GetClockWise()) std::swap(one_prev, one_next); //make angles as if clockwise
            if (!b.GetClockWise()) std::swap(two_prev, two_next); //make angles as if clockwise

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



/** Swap data with `b`. */
void SimpleContour::swap(SimpleContour &b) 
{
    edges.swap(b.edges); 
    std::swap(boundingBox, b.boundingBox); 
    std::swap(area, b.area); 
    //std::swap() does not work on bitfields, so we do it as below.
    bool B;
    B = clockwise; clockwise = b.clockwise; b.clockwise = B;
    B = clockwise_fresh; clockwise_fresh = b.clockwise_fresh; b.clockwise_fresh = B;
    B = area_fresh; area_fresh = b.area_fresh; b.area_fresh = B;
    B = boundingBox_fresh; boundingBox_fresh = b.boundingBox_fresh; b.boundingBox_fresh = B;
} 


void SimpleContour::assign_dont_check(const std::vector<XY> &v)
{
    clear();
    if (v.size()<2) return;
    for (size_t i = 0; i<v.size(); i++)
        edges.push_back(Edge(v[i], v[(i+1)%v.size()]));
    clockwise_fresh = boundingBox_fresh = area_fresh = false;
    Sanitize();  //includes CalculateBoundingBox() and CalculateClockwise()
}

void SimpleContour::assign_dont_check(const XY v[], size_t size)
{
    clear();
    if (size < 2) return;
    for (size_t i=0; i<size; i++)
        edges.push_back(Edge(v[i], v[(i+1)%size]));
    clockwise_fresh = boundingBox_fresh = area_fresh = false;
    Sanitize();  //includes CalculateBoundingBox() and CalculateClockwise()
}

void SimpleContour::assign_dont_check(const std::vector<Edge> &v)
{
    clear();
    if (v.size()<2) return;
    edges = v;
    clockwise_fresh = boundingBox_fresh = area_fresh = false;
    Sanitize();  //includes includes CalculateBoundingBox() and CalculateClockwise()
}

void SimpleContour::assign_dont_check(std::vector<Edge> &&v)
{
    clear();
    if (v.size()<2) return;
    edges.swap(v);
    clockwise_fresh = boundingBox_fresh = area_fresh = false;
    Sanitize();  //includes includes CalculateBoundingBox() and CalculateClockwise()
}

void SimpleContour::assign_dont_check(const Edge v[], size_t size)
{
    clear();
    if (size < 2) return;
    for (size_t i=0; i<size; i++)
        edges.push_back(v[i]);
    clockwise_fresh = boundingBox_fresh = area_fresh = false;
    Sanitize();  //includes includes CalculateBoundingBox() and CalculateClockwise()
}

bool SimpleContour::IsSane() const
{
    if (size()==0) return true;
    for (size_t u = 0; u<size(); u++)
        if (at(u).GetStart().test_equal(at(u).GetEnd()))
            return false;
    if (size()==1 && at(0).straight)
        return false;
    if (size()==2 && at(0).straight && at(1).straight)
        return false;
    return true;
}

/** Fix problems in a shape (edges not joint, etc).
 *
 * @returns true if we were sane, false if changes were needed.
 */
bool SimpleContour::Sanitize()
{
    bool ret = true;
    if (size()==0) return true;
    if (size()==1) goto clear;
    for (size_t u=0; u<size(); /*nope*/)
        if (at(u).GetStart().test_equal(at(u).GetEnd())) {
            edges.erase(edges.begin()+u);
            ret = false;
        } else
            u++;
    switch (size()) {
    case 0: return false;
    case 1: goto clear;
    case 2:
        if (at(0).straight && at(1).straight)
            goto clear;
        /*fallthrough*/
    default:
        //ensure that edges connect
        for (size_t u=0; u<size(); u++)
            if (!at(u).GetEnd().test_equal(at_next(u).GetStart()))
                goto clear;
    }
    if (!ret) area_fresh = boundingBox_fresh = false; //we think clockwiseness remains just by deleting a few edges.
    return ret;
clear:
    clear();
    return false;
}

/** Calculate the boundingBox of the whole shape. 
 *  Assumes the bounding box of curved edges is already calculated.*/
const Block &SimpleContour::CalculateBoundingBox() const
{
    boundingBox.MakeInvalid();
    for(const auto &e : edges) 
        if (e.straight)
            boundingBox += e.GetStart();
        else
            boundingBox += e.CreateBoundingBox();
    boundingBox_fresh = true;
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
    if (!edge.straight) {
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
    for (size_t i = 0; i<ret.size()-num_to_check-1; i++)
        for (size_t j = ret.size()-num_to_check-1; j<ret.size(); j++)
            if (ret[i].Crossing(ret[j], dum1, dum2, dum2))
                return false;
    //OK, we can have these edges inserted
    swap(ret);
    area_fresh = boundingBox_fresh = clockwise_fresh = false; //clockwiseness could change by adding a curvy edge
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
double SimpleContour::CalculateArea() const
{
    register double ret = 0;
    for (const auto &e: edges)
        ret += e.GetAreaAboveAdjusted();
    area_fresh= true;
    return area = ret/2;
}

/** Calculate the circumference length of the shape.
 *
 * @param [in] include_hidden If false, we leave out edges marked as non-visible.
 * @returns Length of the circumference.
 */
double SimpleContour::GetCircumference(bool include_hidden) const
{
    register double ret = 0;
    for (const auto &e : edges)
        if (include_hidden || e.visible)
            ret += e.GetLength();
    return ret;
}

/** Returns the coordinates of the centroid of the shape multiplied by its (signed) area. */
XY SimpleContour::CentroidUpscaled() const
{
    XY ret (0,0);
    for (const auto &e : edges)
        ret += e.GetCentroidAreaAboveUpscaled();
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
    at(0).TangentFrom(from, clockwise, cclockwise);
    XY c, cc;
    for (unsigned u = 1; u<size(); u++) {
        at(u).TangentFrom(from, c, cc);
        clockwise = minmax_clockwise(from, clockwise, c, true);
        cclockwise = minmax_clockwise(from, cclockwise, cc, false);
    }
    return true;
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
    for (const auto &e : edges)
        for (const auto &f : edges)
            e.TangentFrom(f, clockwise, cclockwise);
    return true;
}


//////////////////////////////////SimpleContour::Expand implementation


/** Helper that creates a half circle for EXPAND_ROUND or EXPAND_MITER_ROUND type expansion,
 * where expanded edges are parallel.
 *
 * @param [in] start The start of the half circle (end of the previous expanded edge)
 * @param [in] end The end of the half circle (the start of the next previous edge)
 * @param [in] old The position of the vertex before expanding the edges.
 * @param [in] clockwise The clockwisedness of the original shape.
 * @param [out] append_to Append the resulting (series of) Edge(s) to this array.
 */
void SimpleContour::CreateRoundForExpand(const XY &center, const XY &start, const XY &end, bool clockwise,
                          std::list<ExpandMetaData> &insert_to, std::list<ExpandMetaData>::iterator here,
                          bool visible)
{
    _ASSERT(test_equal(center.DistanceSqr(start), center.DistanceSqr(end)));
    double radius = center.Distance(start);
    double s_deg = acos(std::max(-1., std::min(1., (start.x-center.x)/radius)))*180/M_PI;
    if (start.y<center.y) s_deg = 360-s_deg;
    double d_deg = acos(std::max(-1., std::min(1., (end.x-center.x)/radius)))*180/M_PI;
    if (end.y<center.y) d_deg = 360-d_deg;

    //We do not know if the resulting contour will be clockwise or not
    //But, if we use the clockwiseness of the original contour, we are safe
    //since for positive originals we will keep only positive results,
    //so if we miss and we will be part of a negative contour, that
    //will get dropped anyway. (Likewise for negative orgiginals.)
    SimpleContour round(center, radius, radius, 0, clockwise ? s_deg : d_deg, clockwise ? d_deg : s_deg);
    //delete the straight edge
    if (round.edges.back().straight)
        round.edges.pop_back();
    if (!clockwise) 
        round.Invert();
    //Make sure the construct starts *exactly* at 'start' and 'end'
    //The result is not precise, we simply set the star/endpoint (keeping control points)
    _ASSERT(round.edges.front().start.DistanceSqr(start)<1);
    _ASSERT(round.edges.back().end.DistanceSqr(end)<1);
    round.edges.front().start = start;
    round.edges.back().end = end;
    if (!visible)
        for (auto &e: round.edges)
            e.visible = false;
    insert_to.insert(here, round.edges.begin(), round.edges.end()); //all inserted edges marked as trivially connecting
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
    const double shrink_by = GetClockWise() ? -gap : gap;
    if (size()<=1 || GetBoundingBox().x.Spans() < 2*shrink_by || GetBoundingBox().y.Spans() < 2*shrink_by)
        return; //empty "res" assumed

    class MetaDataList : public std::list<ExpandMetaData>
    {
    public:
        iterator next(iterator i) { if (++i == end()) return begin(); return i; }
        iterator prev(iterator i) { if (i == begin()) return --end(); return --i; }
        const_iterator next(const_iterator i) const { if (++i == end()) return begin(); return i; }
        const_iterator prev(const_iterator i) const { if (i == begin()) return --end(); return --i; }
    } r;


    //Expand all the edges
    for (size_t u = 0; u<size(); u++) {
        at(u).CreateExpand(gap, r); 
        //all edges are marked as trivially connecting
        //the relation of the last of the generated edges with the next: 
        //mark this as a relation needed to be computed
        r.back().cross_type = Edge::CP_INVERSE;
        r.back().original_point = at(u).GetEnd(); 
    }

    //Calculate actual max miter length.
    const double gap_limit = fabs(miter_limit) < MaxVal(miter_limit) ? gap*miter_limit : MaxVal(gap_limit);
    const double gap_limit_sqr = fabs(miter_limit) < MaxVal(miter_limit) ? gap*gap*miter_limit*miter_limit : MaxVal(gap_limit_sqr);

    bool changed_from_cp_inverse;
    
    if (expand_debug==2) goto end;

    do {
        changed_from_cp_inverse = false;
        //Find how and where expanded edges meet
        //do not do this for edge segments generated by expansion above or for cps where 
        //we have already sorted things in a previous run of this do-while cycle
        for (auto i = r.begin(); i!=r.end(); i++) 
            if (i->cross_type==Edge::CP_INVERSE) {
                i->cross_type = i->edge.FindExpandedEdgesCP(r.next(i)->edge, i->cross_point, 
                                                            i->us_end_pos, i->next_start_pos);
                //note if we have changed the status of at least one such CP
                //this means we may need to do another pass
                if (i->cross_type!=Edge::CP_INVERSE)
                    changed_from_cp_inverse = true;
            }

        //Adjust the start and end of the edges to the crosspoints
        //and insert additional edges, if needed
        //Ignore&keep CP_INVERSE edge joins for now

        //Calculate if we start with a miter limited in length
        bool need_miter_limit_bevel = IsMiter(type) &&
            r.back().cross_type == Edge::CP_EXTENDED &&
            r.back().cross_point.DistanceSqr(r.back().edge.GetEnd()) > gap_limit_sqr;

        //cycle through the edges
        for (auto i = r.begin(); i!=r.end(); /*nope*/) {
            const auto prev_i = r.prev(i);
            //fast path: both ends trivial
            if (prev_i->cross_type == Edge::TRIVIAL && i->cross_type==Edge::TRIVIAL) {
                i++;
                continue;
            }
            //fast path2: if both ends are inverse, we do nothing
            if (prev_i->cross_type == Edge::CP_INVERSE && i->cross_type==Edge::CP_INVERSE) {
                i++;
                continue;
            }

            //The new start and endpoint for us (edge #i)
            XY new_start, new_end;
            if (prev_i->cross_type==Edge::CP_REAL)
                new_start = prev_i->cross_point;
            else if (prev_i->cross_type==Edge::CP_EXTENDED && IsMiter(type))
                new_start = prev_i->cross_point;
            else
                new_start = i->edge.GetStart();

            if (i->cross_type==Edge::CP_REAL)
                new_end = i->cross_point;
            else if (i->cross_type==Edge::CP_EXTENDED && IsMiter(type))
                new_end = i->cross_point;
            else
                new_end = i->edge.GetEnd();

            if (IsMiter(type) && (need_miter_limit_bevel || i->cross_type == Edge::CP_EXTENDED)) {
                //If we connected to previous edge via a too long miter, we limit its length
                //The bevel needed in this case was added when we processed the previous edge.
                if (need_miter_limit_bevel)
                    new_start = i->edge.GetStart() + (new_start-i->edge.GetStart()).Normalize()*gap_limit;
                //Check if we connect to the next edge via a too long miter.
                need_miter_limit_bevel = i->cross_type == Edge::CP_EXTENDED &&
                    new_end.DistanceSqr(i->edge.GetEnd()) > gap_limit_sqr;
                //Adjust the endpoint of us to where the miter shall end.
                if (need_miter_limit_bevel)
                    new_end = i->edge.GetEnd() + (new_end-i->edge.GetEnd()).Normalize()*gap_limit;
                //The value of 'need_miter_limit_bevel' will also be re-used next
                _ASSERT(fabs(new_start.length()) < 100000);
                _ASSERT(fabs(new_end.length()) < 100000);
            }

            //if (new_start.test_equal(new_end)) {
            //    //We got degenerate, remove
            //    prev_i->cross_point = new_end = new_start; //Use this from now on.
            //    r.erase(i++);
            //    continue;
            //}

            const auto next_i = r.next(i);
            const bool done = next_i == r.begin();

            //If we did not degenerate to a point, adjust start/end points 
            //for beziers we may have to add straight lines as continuation.
            if (i->edge.straight) {
                i->edge.start = new_start;
                i->edge.end = new_end;
            } else {
                //add straight edge before the inserted bezier for miters
                if (IsMiter(type) && prev_i->cross_type==Edge::CP_EXTENDED)
                    r.emplace(i, new_start, i->edge.GetStart(), !!i->edge.visible);
                if (prev_i->cross_type == Edge::CP_REAL && i->cross_type == Edge::CP_REAL) 
                    i->edge.SetStartEndIgn(new_start, new_end, prev_i->next_start_pos, i->us_end_pos);
                else if (prev_i->cross_type == Edge::CP_REAL)
                    i->edge.SetStartIgn(new_start, prev_i->next_start_pos);
                else if (i->cross_type == Edge::CP_REAL)
                    i->edge.SetEndIgn(new_end, i->us_end_pos);

                //add straight edge after the bezier for miters
                //insert _after_ i (before next_i)
                if (IsMiter(type) && i->cross_type ==Edge::CP_EXTENDED)
                    r.emplace(next_i, i->edge.GetEnd(), new_end, !!i->edge.visible);
            }

            //For miters Add bevel if the miter would be too long. (can be true only for miters)
            if (need_miter_limit_bevel) {
                _ASSERT(IsMiter(type));
                const XY bevel_end = next_i->edge.GetStart() +
                    (i->cross_point-next_i->edge.GetStart()).Normalize()*gap_limit;
                _ASSERT(bevel_end.length() < 100000);
                if (!bevel_end.test_equal(new_end))
                    r.emplace(next_i, new_end, bevel_end, i->edge.visible && next_i->edge.visible);
            }

            //Ok here the current status is
            //for miters we have added the edge (for all) and the straight line of CP_EXTENDED, but
            //nothing for NO_CP_PARALLEL. For round/bevel expand types we just added the edge.
            //Below we add whatever line join is needed.
            //Note that we do nothing for CP_REAL, TRIVIAL or CP_INVERSE. 
            //now even if we have inserted 'i' still points to the original edge

            if (i->cross_type == Edge::NO_CP_PARALLEL && type == EXPAND_MITER) {
                //We add two lines (two miters)
                //Remember, i->cross_point contains the point in between the two lines.
                r.emplace(next_i, new_end, i->cross_point, !!i->edge.visible);
                r.emplace(next_i, i->cross_point, next_i->edge.GetStart(), !!next_i->edge.visible);
            } else if ((i->cross_type == Edge::NO_CP_PARALLEL && IsRound(type)) ||
                (i->cross_type == Edge::CP_EXTENDED && type == EXPAND_ROUND)) {
                //Add a circle, from new_end and the next start
                CreateRoundForExpand(i->original_point, new_end, next_i->edge.GetStart(), gap>0, 
                                     r, next_i, i->edge.visible && next_i->edge.visible);
            } else if ((i->cross_type == Edge::NO_CP_PARALLEL && IsBevel(type)) ||
                (i->cross_type == Edge::CP_EXTENDED && type == EXPAND_BEVEL)) {
                //Add a bevel from new_end and the next start
                r.emplace(next_i, new_end, next_i->edge.GetStart(), i->edge.visible && next_i->edge.visible);
            } else if (i->cross_type == Edge::NO_CP_PARALLEL && type == EXPAND_MITER_SQUARE) {
                //Here we add 3 edges of a half-square
                const XY &next_start = next_i->edge.GetStart();
                const double dist = new_end.Distance(next_start)/2;
                const XY first_tangent = i->edge.NextTangentPoint(1.0);
                const XY first = new_end + (first_tangent - new_end).Normalize()*dist;
                const XY second_tangent = next_i->edge.PrevTangentPoint(0.0);
                const XY second = next_start + (second_tangent - next_start).Normalize()*dist;
                r.emplace(next_i, new_end, first, i->edge.visible && next_i->edge.visible);
                r.emplace(next_i, first, second, i->edge.visible && next_i->edge.visible);
                r.emplace(next_i, second, next_start, i->edge.visible && next_i->edge.visible);
            }
            
            //we may have inserted edges, which will all have TRIVIAL cp connections
            //towards the edge _after_ them. This is erroneour for the last such edge
            //so we copy the cp type of the original edge to the last inserted one.
            //(If we did not insert edges, this is a NOP)
            if (r.prev(next_i) != i) {
                const auto j = r.prev(next_i);
                j->cross_point = i->cross_point;
                j->cross_type = i->cross_type;
                j->original_point = i->original_point;
                j->next_start_pos = i->next_start_pos;
            }

            i = next_i; //step the cycle
            if (done)
                break;
        } //for cycle through edges

        //mark all non-inverse edges as trivial
        for (auto &e : r)
            if (e.cross_type != Edge::CP_INVERSE)
                e.cross_type = Edge::TRIVIAL;

        //avoid round in subsequent rounds, as 'original_point' values are no longer valid
        //and cannot be used during CreateRoundForExpand(). If this does not work,
        //the we will need to compute a suitable center for such rounded edges by re-Expanding
        //the edges in question and finding their crosspoints (if any), but that is probably expensive.
        if (type == EExpandType::EXPAND_MITER_ROUND) type = EExpandType::EXPAND_MITER_BEVEL;
        else if (type == EExpandType::EXPAND_ROUND) type = EExpandType::EXPAND_BEVEL;
    } while (changed_from_cp_inverse && r.size());
end:
    //Insert straight segments for CP_INVERSE joins (not visible)
    for (auto i = r.begin(); i!=r.end(); i++)
        if (i->cross_type == Edge::CP_INVERSE) {
            const auto next_i = r.next(i);
            r.emplace(next_i, Edge(i->edge.GetEnd(), next_i->edge.GetStart(), false));
        } 

    //Ok, now we have the expanded contour in 'r', hopefully all its edges connected to the
    //next one. 
    if (r.size()==0) return; //empty shape
    SimpleContour sp_r;
    sp_r.edges.reserve(r.size());
    for (auto &e : r)
        sp_r.edges.push_back(e.edge);
    sp_r.clockwise_fresh = sp_r.area_fresh = sp_r.boundingBox_fresh = false;
    sp_r.Sanitize();

    //OK, now untangle 
    switch (expand_debug) {
    case 2:
    case 1:
        res = std::move(sp_r);
        break;
    case 0:
        res.Operation(GetClockWise() ? Contour::EXPAND_POSITIVE : Contour::EXPAND_NEGATIVE, Contour(std::move(sp_r)));
        break;
    }
}


//my index is the index of the edge (of "this") just added
void SimpleContour::Expand2DHelper(const XY &gap, std::vector<Edge> &a,
                                   unsigned original_last, unsigned next, unsigned my_index,
                                   int last_type, int stype) const
{
    XY cp;
    double pos_a, pos_b;
    switch (a[original_last].FindExpandedEdgesCP(a[next], cp, pos_a, pos_b)) {
    case Edge::CP_REAL:
        a[original_last].SetEndIgn(cp, pos_a); 
        a[next].SetStartIgn(cp, pos_b);
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
    if (!GetBoundingBox().Overlaps(c.GetBoundingBox())) return REL_APART;
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
    for (const auto &e : edges)
        if (show_hidden || e.visible)
            e.PathTo(cr);
        else {
            cairo_move_to(cr, e.GetEnd().x, e.GetEnd().y);
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
            if (at(i).CreateBoundingBox().x.Overlaps(below.at(j).CreateBoundingBox().x)) {
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
    double y[3], pos[3];
    int forward[3];
    for (size_t i=0; i<size(); i++) {
        const int num = at(i).CrossingVertical(x, y, pos, forward);
        for (int j = 0; j<num; j++)
            if (forward[j])
                section.Set(y[j], forward[j] == (GetClockWise()?1:-1));
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
    for (const auto &e : edges)
        for (const auto &f : edges) {
            if (e.straight && f.straight)
                tmp = e.Distance(f);
            else if (running.ConsiderBB(fabs(e.CreateBoundingBox().Distance(f.CreateBoundingBox()))))
                //if not both are straight try to avoid calculation if bbs are far enough
                tmp = e.Distance(f);
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
    for (const auto &e: edges) {
        XY tmp;
        double dummy;
        double d = e.Distance(o, tmp, dummy);
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
    if (!GetBoundingBox().Overlaps(e.CreateBoundingBox())) return ret;
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
    const Range ret = GetBoundingBox().Cut(A, B); //also tests for A==B or invalid bb, which catches empty "this"
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

} //namespace contour bezier