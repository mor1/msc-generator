/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011,2012,2013,2014 Zoltan Turanyi
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
#include <cmath>
#include "contour.h"



namespace contour {



/** Calculate the boundingBox of the whole shape.
*  Assumes the bounding box of curved edges is already calculated.*/
Block Path::CalculateBoundingBox() const
{
    Block ret(true);
    for (const auto &e : edges)
        if (e.IsStraight())
            ret += e.GetStart();
        else
            ret += e.CreateBoundingBox();
    return ret;
}


/** Create an ellipse (or ellipse slice) shape.
 * If not a full ellipse/circle a line segment is added as _the last edge_ to close it.
 *
 * @param [in] c Centerpoint
 * @param [in] radius_x Radius in the y direction. Its absolute value is used.
 * @param [in] radius_y Radius in the y direction (same as `radius_x` if omitted = circle) Its absolute value is used.
 * @param [in] tilt_deg The tilt of the ellipse in degrees. 0 if omitted.
 * @param [in] s_deg The startpoint of the arc.
 * @param [in] d_deg The endpoint of the arc. If equal to `s_deg` a full ellipse results, else a straight line is added to close the arc.
 * @param [in] add_closing_line If true the function adds a line to not entirely closed arcs. If false, such arcs
 *             remain open. No impact on full circles/ellipses.
 * If `radius_x` is zero, we return an empty shape. If `radius_y` is zero, we assume it to be the same as `radius_x` (circle).
*/
Path::Path(const XY &c, double radius_x, double radius_y, double tilt_deg, double s_deg, double d_deg, bool add_closing_line)
{
    if (radius_x==0) return;
    Edge::GenerateEllipse(edges, c, radius_x, radius_y, tilt_deg, s_deg, d_deg);
    if (add_closing_line) {
        //Add a segment if not a full ellipse
        const XY start = edges.front().start;
        const XY end = edges.back().end;
        edges.emplace_back(end, start);
    }
}

void Path::AddAnEdge(const Edge &edge, bool ensure_connect)
{
    if (ensure_connect && edges.size() && edge.GetStart()!=edges.back().GetEnd()) {
        const XY tmp = edges.back().GetEnd();
        edges.emplace_back(tmp, edge.GetStart(), bool(!!edge.visible));
    }
    edges.push_back(edge);
}


/** Revert the clockwisedness of the shape by reverting, all edges and their order. */
Path &Path::Invert()
{
    for (size_t i = 0; i<size(); i++)
        at(i).Invert();
    for (size_t i = 0; i<size()/2; i++)
        std::swap(at(i), at(size()-1-i));
    return *this;
}


Path &Path::append(const std::vector<XY> &v, bool ensure_connect)
{
    if (ensure_connect && edges.size() && v.size() && v.front()!=edges.back().GetEnd()) {
        edges.reserve(edges.size()+v.size()+1);
        const XY tmp = edges.back().GetEnd();
        edges.emplace_back(tmp, v.front());
    } else
        edges.reserve(edges.size()+v.size());
    for (size_t i = 0; i+1<v.size(); i++)
        if (v[i] != v[i+1])
            edges.emplace_back(v[i], v[i+1]);
    return *this;
}

Path &Path::append(const XY v[], size_t size, bool ensure_connect)
{
    if (ensure_connect && edges.size() && size && v[0]!=edges.back().GetEnd()) {
        edges.reserve(edges.size()+size+1);
        const XY tmp = edges.back().GetEnd();
        edges.emplace_back(tmp, v[0]);
    } else
        edges.reserve(edges.size()+size);
    for (size_t i = 0; i+1<size; i++)
        if (v[i] != v[i+1])
            edges.emplace_back(v[i], v[i+1]);
    return *this;
}

Path &Path::append(const std::vector<Edge> &v, bool ensure_connect)
{
    if (ensure_connect && edges.size() && v.size() && v.front().GetStart()!=edges.back().GetEnd()) {
        edges.reserve(edges.size()+v.size()+1);
        const XY tmp = edges.back().GetEnd();
        edges.emplace_back(tmp, v.front().GetStart(), edges.back().visible && v.front().visible);
    } else
        edges.reserve(edges.size()+v.size());
    edges.insert(edges.end(), v.begin(), v.end());
    return *this;
}

Path &Path::append(const Edge v[], size_t size, bool ensure_connect)
{
    if (ensure_connect && edges.size() && size && v[0].GetStart()!=edges.back().GetEnd()) {
        edges.reserve(edges.size()+size+1);
        const XY tmp = edges.back().GetEnd();
        edges.emplace_back(tmp, v[0].GetStart(), edges.back().visible && v[0].visible);
    } else
        edges.reserve(edges.size()+size);
    for (size_t i = 0; i<size; i++)
        edges.push_back(v[i]);
    return *this;
}

Path &Path::append(const Path &p, bool ensure_connect)
{
    if (ensure_connect && edges.size() && p.size() && p[0].GetStart()!=edges.back().GetEnd()) {
        edges.reserve(edges.size()+p.size()+1);
        const XY tmp = edges.back().GetEnd();
        edges.emplace_back(tmp, p[0].GetStart(), edges.back().visible && p[0].visible);
    } else
        edges.reserve(edges.size()+p.size());
    edges.insert(edges.end(), p.edges.begin(), p.edges.end());
    return *this;
}


/** Draw the path as a path to a cairo context.
*
* @param [in] cr The cairo context.
* @param [in] show_hidden If false, we skip edges marked not visible.
*/
void Path::CairoPath(cairo_t *cr, bool show_hidden) const
{
    if (size()==0 || cr==NULL) return;
    XY started_at = at(0).GetStart();
    cairo_move_to(cr, at(0).GetStart().x, at(0).GetStart().y);
    for (size_t u = 0; u<edges.size(); u++) {
        const Edge & prev = edges[(u+edges.size()-1)%edges.size()];
        if (show_hidden || edges[u].visible) {
            if (prev.GetEnd()!=at(u).GetStart()) {
                cairo_new_sub_path(cr);
                started_at = at(u).GetStart();
                cairo_move_to(cr, started_at.x, started_at.y);
            }
            edges[u].PathTo(cr);
        } else {
            if (prev.GetEnd()==started_at)
                cairo_close_path(cr);
            cairo_new_sub_path(cr);
            cairo_move_to(cr, edges[u].GetEnd().x, edges[u].GetEnd().y);
            started_at = edges[u].GetEnd();
        }
    }
    if (edges.back().GetEnd()==started_at)
        cairo_close_path(cr);
}

/** Draw the path in dashed lines to the path of a cairo context. Needed for backends not supporting dashed lines.
*
* We also assume cairo dash is set to continuous: we fake dash here.
* @param [in] cr The cairo context.
* @param [in] pattern Contains lengths of alternating on/off segments.
* @param [in] num contains the number of elements in pattern.
* @param [in] show_hidden If false, we skip edges marked not visible.
*/

void Path::CairoPathDashed(cairo_t *cr, const double pattern[], unsigned num, bool show_hidden) const
{
    if (size()==0 || cr==NULL) return;
    if (num<2) {
        CairoPath(cr, show_hidden);
        return;
    }
    double offset = 0;
    int pos = 0;
    for (size_t i = 0; i<size(); i++)
        if (at(i).visible)
            at(i).PathDashed(cr, pattern, num, pos, offset);
}

/**Take those consecuitve segments that are closed and put them to Edge vectors.
 * No untangle is done here.
 * @param [in] close_everything If true then open segments are also closed.*/
std::list<std::vector<Edge>> Path::ConvertToClosed(bool close_everything) const
{
    std::list<std::vector<Edge>> ret;
    if (size()==0) return ret;
    ret.emplace_back();
    for (auto &e: edges) {
        if (ret.back().size() && ret.back().back().GetEnd()!=e.GetStart()) {
            //Close last run. First see if it ended up closed
            if (close_everything && ret.back().front().GetStart()!=ret.back().back().GetEnd()) {
                const XY st = ret.back().back().GetEnd();
                const XY en = ret.back().front().GetStart();
                ret.back().emplace_back(st, en);
            }
            ret.emplace_back();
        }
        ret.back().push_back(e);
    }
    //check if last segment needs to be closed
    if (close_everything && ret.back().front().GetStart()!=ret.back().back().GetEnd()) {
        const XY st = ret.back().back().GetEnd();
        const XY en = ret.back().front().GetStart();
        ret.back().emplace_back(st, en);
    }
    return ret;
}



///////////////////////////// SimpleContour

//Do not create degenerate triangles.
//Always create clockwise.
SimpleContour::SimpleContour(XY a, XY b, XY c) :
    clockwise_fresh(true), boundingBox_fresh(true), area_fresh(false),
    clockwise(true), boundingBox(a, b)
{
    switch (triangle_dir(a,b,c)) {
    default: //create empty if degenerate triangle
        boundingBox.MakeInvalid();
        return;
    case CLOCKWISE:
        edges.emplace_back(a, b);
        edges.emplace_back(b, c);
        edges.emplace_back(c, a);
        break;
    case COUNTERCLOCKWISE:
        edges.emplace_back(a, c);
        edges.emplace_back(c, b);
        edges.emplace_back(b, a);
        break;
    }
    boundingBox += c;
}

/** Create an ellipse (or ellipse slice) shape.
 * If not a full ellipse/circle a line segment is added as _the last edge_ to close it.
 *
 * @param [in] c Centerpoint
 * @param [in] radius_x Radius in the y direction. Its absolute value is used.
 * @param [in] radius_y Radius in the y direction (same as `radius_x` if omitted = circle) Its absolute value is used.
 * @param [in] tilt_deg The tilt of the ellipse in degrees. 0 if omitted.
 * @param [in] s_deg The startpoint of the arc.
 * @param [in] d_deg The endpoint of the arc. If equal to `s_deg` a full ellipse results, else a straight line is added to close the arc.
 * If `radius_x` is zero, we return an empty shape. If `radius_y` is zero, we assume it to be the same as `radius_x` (circle).
*/
SimpleContour::SimpleContour(const XY &c, double radius_x, double radius_y, double tilt_deg, double s_deg, double d_deg) :
   clockwise_fresh(true), boundingBox_fresh(false), area_fresh(false),
   clockwise(true)
{
    if (radius_x<=0 || radius_y<0) {
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
SimpleContour &SimpleContour::Invert()
{
    for (size_t i=0; i<size(); i++)
        at(i).Invert();
    for (size_t i=0; i<size()/2; i++)
        std::swap(at(i), at(size()-1-i));
    area *= -1;
    clockwise = !clockwise;
    return *this;
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
    if (size()==0 || !GetBoundingBox().IsWithinBool(p)) return WI_OUTSIDE;

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

        for (int u = 0; u<num; u++) {
            if ((strict && y[u] == p.y) ||  //on an edge, we are _not_ approximate here
                (!strict&& test_equal(y[u], p.y))) {  //on an edge, we are approximate here
                //we have tested that at(e).start is not equal to p, so no need to test for that here
                if (test_equal(at(e).GetEnd().x, p.x)) {
                    if (edge) *edge = next(e);
                    return WI_ON_VERTEX;
                } else {
                    if (pos) *pos = po[u];
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

//true if q is between (a and b) modulo 4
//if q is almost equal to a or b, we return false;
inline bool really_between04_warp(const RayAngle &q, const RayAngle &a, const RayAngle &b)
{
    if (a.Smaller(b)) return q.Smaller(b) && !q.Smaller(a);
    return a.Smaller(q) || q.Smaller(b);
}

/** A helper for CheckContainment().
 * Can result in SAME, APRT, A_INSIDE_B and these results represent the actual
 * relation between 'this' and 'b'.
 * It can also return OVERLAP, which only means one of our point is outside 'b', 
 * either because we are APART or because B_INSIDE_A.
 * clockwiseness is ignored: the inside of a counterclockwise contour is the limited space
 * it encircles.*/
EContourRelationType SimpleContour::CheckContainmentHelper(const SimpleContour &b) const
{
    size_t edge;
    _ASSERT(!b.IsEmpty() && !IsEmpty());
    const size_t max = size();
    for (size_t i=0; i<max; i++) {
        double pos;
        const XY p = at(i).GetStart();
        //Optimize: check BB here and not in SimpleContour::IsWithin below
        if (!b.GetBoundingBox().IsWithinBool(p)) return REL_OVERLAP;
        //check bb here
        switch (b.IsWithin(p, &edge, &pos, /*strict=*/false)) {
        default:
            _ASSERT(0);
        case WI_INSIDE:  return REL_A_INSIDE_B;
        case WI_OUTSIDE: return REL_OVERLAP;
        case WI_ON_VERTEX:
            pos = 0; //if we are on b's vertex, this is the right pos for that edge
        case WI_ON_EDGE:
            RayAngle prev1 = at_prev(i).Angle(true, 1);
            RayAngle next1 = at(i).Angle(false, 0);
            RayAngle prev2 = pos == 0 ? b.at_prev(edge).Angle(true, 1) : b.at(edge).Angle(true, pos);
            RayAngle next2 = pos == 1 ? b.at_next(edge).Angle(false, 0) : b.at(edge).Angle(false, pos);
            if (!GetClockWise()) std::swap(prev1, next1); //make angles as if clockwise
            if (!b.GetClockWise()) std::swap(prev2, next2); //make angles as if clockwise

            //if both the same, we continue: this vertex is non-decisive
            if (prev1.IsSimilar(prev2) && next1.IsSimilar(next2)) break; //SAME - do another edge

            //if values are too close it is dangerous, we look for other vertices
            if (prev1.IsSimilar(next1)) break;
            if (prev2.IsSimilar(next2)) break;

            if (really_between04_warp(prev1, next2, prev2) &&
                really_between04_warp(next1, next2, prev2)) return REL_A_INSIDE_B;
            if (really_between04_warp(prev2, next1, prev1) &&
                really_between04_warp(next2, next1, prev1)) return REL_B_INSIDE_A;
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
    //fast path
    if (!GetBoundingBox().Overlaps(other.GetBoundingBox()))
        return REL_APART;
    if (other.IsEmpty()) 
        return IsEmpty() ? REL_BOTH_EMPTY : REL_B_IS_EMPTY;
    if (IsEmpty())
        return REL_A_IS_EMPTY;
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
    edges = v;
    clockwise_fresh = boundingBox_fresh = area_fresh = false;
    Sanitize();  //includes includes CalculateBoundingBox() and CalculateClockwise()
}

void SimpleContour::assign_dont_check(std::vector<Edge> &&v)
{
    clear();
    edges.swap(v);
    clockwise_fresh = boundingBox_fresh = area_fresh = false;
    Sanitize();  //includes includes CalculateBoundingBox() and CalculateClockwise()
}

void SimpleContour::assign_dont_check(const Edge v[], size_t size)
{
    clear();
    for (size_t i=0; i<size; i++)
        edges.push_back(v[i]);
    clockwise_fresh = boundingBox_fresh = area_fresh = false;
    Sanitize();  //includes includes CalculateBoundingBox() and CalculateClockwise()
}

/** Checks if all edges connect, the shape is closed, no edges 
 * are degenerated to a dot, the shape has an actual area, etc.*/
bool SimpleContour::IsSane() const
{
    if (size()==0) return true;
    if (size()==1) {
        if (at(0).IsStraight()) return false;
        if (at(0).IsDot()) return false;
        if (at(0).GetStart().test_equal(at(0).GetEnd())) return true;
        return false;
    }
    if (size()==2 && at(0).straight && at(1).straight)
        return false;
    for (size_t u = 0; u<size(); u++)
        if (at(u).IsDot())
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
        if (at(u).IsDot()) {
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
            if (ret[ret.size()-2].Crossing(ret[ret.size()-1], true, dum1, dum2, dum2))
                return false;
            num_to_check = 2;
        }
    }
    //now check if inserted edges cross any of the ones before
    //check if edge->end is crossing any existing edges
    for (size_t i = 0; i<ret.size()-num_to_check-1; i++)
        for (size_t j = ret.size()-num_to_check-1; j<ret.size(); j++)
            if (ret[i].Crossing(ret[j], next(i)==j, dum1, dum2, dum2))
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
        for (const auto &f : from.edges)
            e.TangentFrom(f, clockwise, cclockwise);
    return true;
}


//////////////////////////////////SimpleContour::Expand implementation


/** Helper that creates a half circle for EXPAND_ROUND or EXPAND_MITER_ROUND type expansion,
 * where expanded edges are parallel.
 *
 * @param [in] center The center of the circle of the arc (likely the original vertex)
 * @param [in] start The start of the arc (end of the previous expanded edge)
 * @param [in] end The end of the arc (the start of the next previous edge)
 * @param [in] clockwise The clockwisedness of the original shape.
 * @param [out] insert_to Insert the resulting (series of) Edge(s) to this list...
 * @param [in] here ...before this location.
 * @param [in] visible Set the visibility of the newly inserted edges to this.
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

    bool replace_cp_inverse_to_extended = false;

    class MetaDataList : public std::list<ExpandMetaData>
    {
    public:
        iterator next(iterator i) { if (++i == end()) return begin(); return i; }
        iterator prev(iterator i) { if (i == begin()) return --end(); return --i; }
        const_iterator next(const_iterator i) const { if (++i == end()) return begin(); return i; }
        const_iterator prev(const_iterator i) const { if (i == begin()) return --end(); return --i; }
    } r;

    //Expand all the edges
    std::list<Edge> tmp;
    for (size_t u = 0; u<size(); u++) {
        tmp.clear();
        XY prev_tangent, next_tangent;
        if (!at(u).CreateExpand(gap, tmp, prev_tangent, next_tangent))
            //false is returned if a bezier degenerates - replace with a line
            Edge(at(u).GetStart(), at(u).GetEnd(), at(u).visible).CreateExpand(gap, tmp, prev_tangent, next_tangent);
        _ASSERT(tmp.size());
        bool first = true;
        for (auto t: tmp) {
            r.emplace_back(t, u);
            if (first) {
                r.back().prev_tangent = prev_tangent;
                first = false;
            }
        }
        r.back().next_tangent = next_tangent;
        //all edges are marked as trivially connecting
        //the relation of the last of the generated edges with the next:
        //mark this as a relation needed to be computed
        r.back().cross_type = Edge::CP_INVERSE;
    }

    //Calculate actual max miter length.
    const double gap_limit_to_use = fabs(miter_limit) < MaxVal(miter_limit) ? gap*miter_limit : MaxVal(gap_limit_to_use);
    //Calculate the square of it to test on a DistanceSqr() - to avoid sqrt() calls.
    //Make it a bit bigger (2 pixels) - apply miter only if miter would be somewhat larger than
    //limit. This is to avoid very small chopped miters.
    const double gap_limit_sqr_to_test = fabs(miter_limit) < MaxVal(miter_limit) ? gap*gap*miter_limit*miter_limit + 2: MaxVal(gap_limit_sqr_to_test);

    bool need_to_process, has_remaining_cp_inverse;

#ifdef _DEBUG
    if (expand_debug==2) goto end;
#endif

    do {
        need_to_process = false;
        has_remaining_cp_inverse = false;
        //Find how and where expanded edges meet
        //do not do this for edge segments generated by expansion above or for cps where
        //we have already sorted things in a previous run of this do-while cycle
        for (auto i = r.begin(); i!=r.end(); i++)
            if (i->cross_type==Edge::CP_TRIVIAL)
                continue;
            else if (i->cross_type==Edge::CP_INVERSE) {
                i->cross_type = i->FindExpandedEdgesCP(*r.next(i), i->cross_point,
                    i->next_tangent, r.next(i)->prev_tangent,
                    !at(i->original_edge).IsStraight(), !at(r.next(i)->original_edge).IsStraight(),
                    i->us_end_pos, i->next_start_pos);
                _ASSERT(!isnan(i->cross_point.x) && !isnan(i->cross_point.y));
                //note if we have changed the status of at least one such CP
                //this means we may need to do another pass
                if (i->cross_type!=Edge::CP_INVERSE)
                    need_to_process = true;
                else
                    has_remaining_cp_inverse = true;
            } else
                need_to_process = true; //any join that is not CP_TRIVIAL or CP_INVERSE

        //Adjust the start and end of the edges to the crosspoints
        //and insert additional edges, if needed
        //Ignore&keep CP_INVERSE edge joins for now

        //Calculate if we start with a miter limited in length
        bool need_miter_limit_bevel = IsMiter(type) &&
            r.back().cross_type == Edge::CP_EXTENDED &&
            r.back().cross_point.DistanceSqr(r.back().GetEnd()) > gap_limit_sqr_to_test;
        XY prev_miter_limit_bevel_point;
        if (need_miter_limit_bevel)
            prev_miter_limit_bevel_point = r.front().GetStart() +
                    (r.back().cross_point-r.front().GetStart()).Normalize()*gap_limit_to_use;

        //cycle through the edges if we have any connections that is not CP_TRIVIAL or CP_INVERSE
        if (need_to_process) {
            for (auto i = r.begin(); i!=r.end(); /*nope*/) {
                const auto prev_i = r.prev(i);
                //fast path: both ends either trivial or inverse
                if ((prev_i->cross_type == Edge::CP_TRIVIAL || prev_i->cross_type == Edge::CP_INVERSE) &&
                    (i->cross_type==Edge::CP_TRIVIAL || i->cross_type==Edge::CP_INVERSE)) {
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
                    new_start = i->GetStart();

                if (i->cross_type==Edge::CP_REAL)
                    new_end = i->cross_point;
                else if (i->cross_type==Edge::CP_EXTENDED && IsMiter(type))
                    new_end = i->cross_point;
                else
                    new_end = i->GetEnd();

                if (IsMiter(type) && (need_miter_limit_bevel || i->cross_type == Edge::CP_EXTENDED)) {
                    //If we connected to previous edge via a too long miter, we limit its length
                    //The bevel needed in this case was added when we processed the previous
                    if (need_miter_limit_bevel)
                        new_start = prev_miter_limit_bevel_point;
                    //Check if we connect to the next edge via a too long miter.
                    need_miter_limit_bevel = i->cross_type == Edge::CP_EXTENDED &&
                        new_end.DistanceSqr(i->GetEnd()) > gap_limit_sqr_to_test;
                    //Adjust the endpoint of us to where the miter shall end.
                    if (need_miter_limit_bevel)
                        new_end = i->GetEnd() + (new_end-i->GetEnd()).Normalize()*gap_limit_to_use;
                    //The value of 'need_miter_limit_bevel' will also be re-used next
                    _ASSERT(fabs(new_start.length()) < 100000);
                    _ASSERT(!isnan(new_end.x) && !isnan(new_end.y));
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
                if (i->straight) {
                    i->start = new_start;
                    i->end = new_end;
                } else {
                    //add straight edge before the inserted bezier for miters
                    if (IsMiter(type) && prev_i->cross_type==Edge::CP_EXTENDED)
                        r.emplace(i, new_start, i->GetStart(), bool(!!i->visible));
                    if (prev_i->cross_type == Edge::CP_REAL && i->cross_type == Edge::CP_REAL)
                        i->SetStartEndIgn(new_start, new_end, prev_i->next_start_pos, i->us_end_pos);
                    else if (prev_i->cross_type == Edge::CP_REAL)
                        i->SetStartIgn(new_start, prev_i->next_start_pos);
                    else if (i->cross_type == Edge::CP_REAL)
                        i->SetEndIgn(new_end, i->us_end_pos);

                    //add straight edge after the bezier for miters
                    //insert _after_ i (before next_i)
                    if (IsMiter(type) && i->cross_type ==Edge::CP_EXTENDED)
                        //make sure we copy the metadata, as well ('cross_type' and afterwards)
                        //beacuse the next iteration will rely on these, when adding
                        r.emplace(next_i, i->GetEnd(), new_end, bool(!!i->visible), i->cross_type, i->cross_point, double(), i->next_start_pos);
                }

                //For miters Add bevel if the miter would be too long. (can be true only for miters)
                if (need_miter_limit_bevel) {
                    _ASSERT(IsMiter(type));
                    //If we are last, we cannot use next_i->GetStart in claculating the miter
                    //end, since that has been already modified. In fact it is already the
                    //right miter end, so we can just re-use it.
                    XY bevel_end = next_i->GetStart();
                    if (i != --r.end())
                        bevel_end += (i->cross_point-next_i->GetStart()).Normalize()*gap_limit_to_use;
                    _ASSERT(bevel_end.length() < 100000);
                    if (bevel_end != new_end)
                        //make sure we copy the metadata, as well ('cross_type' and afterwards)
                        //beacuse the next iteration will rely on these, when adding
                        r.emplace(next_i, new_end, bevel_end, i->visible && next_i->visible, i->cross_type, i->cross_point, double(), i->next_start_pos);
                    prev_miter_limit_bevel_point = bevel_end;
                }

                //What we did so far:
                //- We have adjusted the start/endpoint of the edge and handled everything around
                //  the start of the edge.
                //- for miters we have 1) added the straight line needed for beziers and CP_EXTENDED;
                //  2) added the bevel if the miter would have been too long; basically we handled
                //  everything, except the parallel case
                //- for round/bevel expand types we just added the edge, no join specifics
                //Below we add whatever line join is needed.
                //Note that we do nothing for CP_REAL, TRIVIAL or CP_INVERSE, as for the first two
                //nothing needs to be done, for the latter we do not know what to do.
                //Now even if we have inserted 'i' still points to the original edge.

                if (i->cross_type == Edge::CP_EXTENDED) {
                    if (type == EXPAND_BEVEL) {
                        //Add a bevel from new_end and the next start
                        r.emplace(next_i, new_end, next_i->GetStart(), i->visible && next_i->visible);
                    } else if (type == EXPAND_ROUND) {
                        //Add a circle, from new_end and the next start
                        CreateRoundForExpand(at(i->original_edge).GetEnd(), new_end, next_i->GetStart(), gap>0,
                            r, next_i, i->visible && next_i->visible);
                    }
                }
                //We have done everything for non-parallel joins
                if (i->cross_type != Edge::NO_CP_PARALLEL) {
                    i = next_i;
                    if (done) break;
                    else continue;
                }

                /* Here we handle CP_PARALLEL. */
                //Find where the expanded version of this edge begins and where does the next one end.
                //This is so that we can remove loops
                auto start_orig_us = i;
                auto end_orig_next = next_i;
                while (start_orig_us!=r.begin() && start_orig_us->original_edge==i->original_edge)
                    start_orig_us--;
                //step back to the first element
                if (start_orig_us->original_edge!=i->original_edge)
                    start_orig_us++;
                while (end_orig_next!=r.end() && end_orig_next->original_edge==next_i->original_edge)
                    end_orig_next++;
                //leave to point beyond the last element

                //Now determine which direction the round/bevel/miter needs to be added.
                //This is done by updating i->cross_point.
                //Note that FindExpandedEdgesCP() calculates a CP as below
                //- if one is straight, the cp is on the straight line. This is the right
                //  (original) direction as ends of straight edges do not change direction when expanded.
                //- if both are bezier, the calculated cp is calcualted based on the direction
                //  of the first edge (in "i")
                //- if both of the edges are straight, the cp is as for the two bezier cases.
                //  This can happen if the last segment of a bezier has turned to a line during expansion.
                //NOTE that FindExpandedEdgesCP() uses the tangens of the *original* edge, thus the cp
                //is in the direction of the origial edge ends even if a bezier edge changed direction
                //during expansion.


                ////Thus, we need to alter the cp, if 1) neither of them is straight and 2) the edge "i"
                ////changed direction.
                //if (!i->IsStraight() && !next_i->IsStraight() &&
                //    !Edge::IsSameDir(at(i->original_edge).GetEnd(), at(i->original_edge).NextTangentPoint(1.0),
                //    i->GetEnd(), i->next_tangent))
                //    //in this case flip cp to the original vertex
                //    i->cross_point = 2*at(i->original_edge).GetEnd() - i->cross_point;

                //If we are at the end of the list (and next_i & end_orig_next
                //are at the beginning, we copy them to the end, so RemoveLoop
                //can operate on a contigous series of edges.
                //(RemoveLoop does not do circular iterator increments/decrements)
                if (done) {
                    r.splice(r.end(), r, next_i, end_orig_next);
                    end_orig_next = r.end();
                }

                //Below we add the line joins.
                //We do not copy metadata, all new edges will have CP_TRIVIAL joins to their
                //following edge. This is just as well, since we add edges such that the last
                //edge ends exactly in next_i->GetStart() - so we form a trivial join.
                if (type == EXPAND_MITER) {
                    //We add two lines (two miters)
                    //Remember, i->cross_point contains the point in between the two lines.
                    //We do not limit these miters, even if miter_limit is not infinite.
                    r.emplace(next_i, new_end, i->cross_point, bool(!!i->visible));
                    auto ii = r.emplace(next_i, i->cross_point, next_i->GetStart(), bool(!!next_i->visible));
                    Edge::RemoveLoop(r, start_orig_us, ii);
                    Edge::RemoveLoop(r, ii, end_orig_next);
                } else if (IsRound(type)) {
                    //Add a circle, from new_end and the next start
                    CreateRoundForExpand(at(i->original_edge).GetEnd(), new_end, next_i->GetStart(), gap>0,
                        r, next_i, i->visible && next_i->visible);
                    Edge::RemoveLoop(r, start_orig_us, next_i);
                    //i and next_i may be destroyed here. Search loops from start_orig_us again
                    Edge::RemoveLoop(r, start_orig_us, end_orig_next);
                } else if (IsBevel(type)) {
                    //Add a bevel from new_end and the next start
                    //r.emplace(next_i, new_end, next_i->GetStart(), i->visible && next_i->visible);
                    const XY tangent = (at(i->original_edge).NextTangentPoint(1.0) -
                                        at(i->original_edge).GetEnd()).Normalize();
                    const XY cp = (new_end + next_i->GetStart())/2 +
                                    tangent * (new_end - next_i->GetStart()).length()/2;
                    r.emplace(next_i, new_end, cp, i->visible && next_i->visible);
                    auto ii = r.emplace(next_i, cp, next_i->GetStart(), i->visible && next_i->visible);
                    Edge::RemoveLoop(r, start_orig_us, ii);
                    Edge::RemoveLoop(r, ii, end_orig_next);
                } else if (type == EXPAND_MITER_SQUARE) {
                    //Here we add 3 edges of a half-square
                    const XY &next_start = next_i->GetStart();
                    const double dist = new_end.Distance(next_start)/2;
                    const XY tangent = (at(i->original_edge).NextTangentPoint(1.0) -
                                        at(i->original_edge).GetEnd()).Normalize()*dist;
                    r.emplace(next_i, new_end, new_end+tangent, i->visible && next_i->visible);
                    r.emplace(next_i, new_end+tangent, next_start+tangent, i->visible && next_i->visible);
                    r.emplace(next_i, next_start+tangent, next_start, i->visible && next_i->visible);
                    Edge::RemoveLoop(r, start_orig_us, next_i);
                    //i and next_i may be destroyed here. Search loops from start_orig_us again
                    Edge::RemoveLoop(r, start_orig_us, end_orig_next);
                }

                i = --end_orig_next; //step the cycle
                if (done)
                    break;
            } //for cycle through edges

            //mark all non-inverse edges as trivial
            for (auto &e : r)
                if (e.cross_type != Edge::CP_INVERSE)
                    e.cross_type = Edge::CP_TRIVIAL;
        } else if (replace_cp_inverse_to_extended && has_remaining_cp_inverse) {
            //this branch is called if need_to_process is false (so we did not
            //resolve any CP_INVERSE since the last run) but we do have CP_INVERSE joins left
            //This means we cannot resolve these, so we try doing them as if they were CP_EXPAND
            for (auto &e : r)
                if (e.cross_type == Edge::CP_INVERSE)
                    e.cross_type = Edge::CP_EXTENDED;
            need_to_process = true;
        }

        ////avoid round in subsequent rounds, as metadata values are no longer valid
        ////and cannot be used during CreateRoundForExpand(). If this does not work,
        ////the we will need to compute a suitable center for such rounded edges by re-Expanding
        ////the edges in question and finding their crosspoints (if any), but that is probably expensive.
        //if (type == EExpandType::EXPAND_MITER_ROUND) type = EExpandType::EXPAND_MITER_BEVEL;
        //else if (type == EExpandType::EXPAND_ROUND) type = EExpandType::EXPAND_BEVEL;
    } while (need_to_process && r.size());

    //Insert straight segments for CP_INVERSE joins (not visible) & try to combine edges
    for (auto i = r.begin(); i!=r.end(); i++) {
        const auto next_i = r.next(i);
        if (i->cross_type == Edge::CP_INVERSE)
            r.emplace(next_i, i->GetEnd(), next_i->GetStart(), false);
        else if (i->CheckAndCombine(*next_i))
            r.erase(next_i);
    }
    //Ok, now we have the expanded contour in 'r', hopefully all its edges connected to the
    //next one.
#ifdef _DEBUG
end:
#endif

    if (r.size()==0) return; //empty shape
    SimpleContour sp_r;
    sp_r.edges.reserve(r.size());
    for (auto &e : r)
        sp_r.edges.push_back(e);
    sp_r.clockwise_fresh = sp_r.area_fresh = sp_r.boundingBox_fresh = false;
    sp_r.Sanitize(); //remove degenerate edges

#ifdef _DEBUG
    //OK, now untangle
    if (expand_debug)
        expand_debug_contour.append(sp_r);
#endif
    res.Operation(GetClockWise() ? Contour::EXPAND_POSITIVE : Contour::EXPAND_NEGATIVE, Contour(std::move(sp_r)));
}


/** A helper to Expand2D.
 * It connects two edges, which ended up disconnected after being Expand2D'd.
 * @param [in] gap Specifies how much to expand.
 * @param a The series of edges on which to work.
 * @param [in] original_last The index of the first edge in 'a' to connect.
 * @param [in] next The index of the second edge in 'a' to connect. Edges used to connect 
 *                  will be inserted before this edge.
 * @param [in] my_index The index of the edge in 'this' corresponding to 'a[next]'.
 * @param [in] last_type The ending direction of 'a[original_last]'
 * @param [in] stype The starting direction of 'a[next]'
 */
void SimpleContour::Expand2DHelper(const XY &gap, std::vector<Edge> &a,
                                   unsigned original_last, unsigned next, unsigned my_index,
                                   int last_type, int stype) const
{
    XY cp;
    double pos_a, pos_b;
    if (Edge::CP_REAL == a[original_last].FindExpandedEdgesCP(a[next], cp,
                         a[original_last].NextTangentPoint(1), 
                         a[next].PrevTangentPoint(0), true, true, pos_a, pos_b)) {
        a[original_last].SetEndIgn(cp, pos_a);
        a[next].SetStartIgn(cp, pos_b);
        return;
    }
    const bool turn_clwise = CLOCKWISE == triangle_dir(PrevTangentPoint(my_index, 0),
                                                       at(my_index).GetStart(),
                                                       NextTangentPoint(my_index, 0));
    if (last_type && !(last_type & 1) && stype == -last_type && turn_clwise) {
        cp = a[original_last].GetEnd();
        //insert an extra point only if we have a sharp convex vertex
        switch (last_type) {
        case +2: cp.x += 2*gap.x; break;
        case -2: cp.x -= 2*gap.x; break;
        case +4: cp.y -= 2*gap.y; break;
        case -4: cp.y += 2*gap.y; break;
        default: _ASSERT(0); break;
        }
        const Edge A(a[original_last].GetEnd(), cp, !!a[original_last].visible);
        const Edge B(cp, a[next].GetStart(), !!a[next].visible);
        a.insert(a.begin()+original_last+1, A);
        a.insert(a.begin()+original_last+2, B);
        return;
    }
    const Edge C(a[original_last].GetEnd(), a[next].GetStart(), a[original_last].visible && a[next].visible);
    a.insert(a.begin()+original_last+1, C);
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
    //if we do nothing, we return us
    if (gap.x==0 && gap.y==0) {
        res = *this;
        return;
    }
    //if we srink too much we return empty
    if ((GetBoundingBox().x.Spans()<-2*gap.x && GetClockWise()) ||
        (GetBoundingBox().y.Spans()<-2*gap.y && GetClockWise()) ||
        (GetBoundingBox().x.Spans()<2*gap.x && !GetClockWise()) ||
        (GetBoundingBox().y.Spans()<2*gap.y && !GetClockWise()) ||
        size()==0) {
        res.clear();
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

    _ASSERT(r2.first.outline.edges.size()==0 || r2.first.outline.edges.front().GetStart()==r2.first.outline.edges.back().GetEnd());

    r2.first.outline.clockwise_fresh = r2.first.outline.area_fresh = r2.first.outline.boundingBox_fresh = false;
    r2.first.outline.Sanitize(); //calculates clockwise and boundingbox, as well
    if (r2.first.outline.size()==0) return;
    r2.boundingBox = r2.first.outline.GetBoundingBox(); //copy bb to outer
#ifdef _DEBUG
    //OK, now untangle
    if (expand_debug)
        expand_debug_contour.append(r2.first.outline);
#endif
    res.Operation(GetClockWise() ? Contour::EXPAND_POSITIVE : Contour::EXPAND_NEGATIVE, std::move(r2));
    //XXX Fix this better:
    if ((gap.x<0) == GetClockWise()) {
        //Kill those cwh's which are not entirely inside the original
        Contour tmp;
        for (unsigned u = 0; u<res.size(); u++)
            if (RelationTo(res[u].outline) == REL_B_INSIDE_A )
                tmp.append(std::move(res[u]));
        res.swap(tmp);
    }
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
            if (at(u1).Crossing(c.at(u2), false, r, one_pos, two_pos))
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
void SimpleContour::CairoPath(cairo_t *cr, bool show_hidden) const
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

void SimpleContour::CairoPathDashed(cairo_t *cr, const double pattern[], unsigned num, bool show_hidden) const
{
    if (size()==0 || cr==NULL) return;
    if (num<2) {
        CairoPath(cr, show_hidden);
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
    if (GetBoundingBox().x.Overlaps(below.GetBoundingBox().x) &&
              GetBoundingBox().y.till+offset > below.GetBoundingBox().y.from)
        for (size_t i = 0; i<size(); i++)
            for (size_t j = 0; j<below.size(); j++)
                offset = at(i).OffsetBelow(below.at(j), touchpoint, offset);
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
        for (const auto &f : o.edges) {
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
 * @param [in] o The point to take the distance from.
 * @param [out] ret We return the point on our contour closes to `o`.
 * @return The distance, negative if `o` is inside us. `CONTOUR_INFINITY` if we are empty. */
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
 * Same as Distance(const XY &o, XY &ret), but in addition we return two tangent points
 * from the tangent of the shape at `ret`. See @ref contour for a description of tangents.
 *
 * @param [in] o The point to take the distance from.
 * @param [out] ret We return the point on our contour closes to `o`.
 * @param [out] t1 The forward tangent point.
 * @param [out] t2 The backward tangent point.
 * @return The distance, negative if `o` is inside us. `CONTOUR_INFINITY` if we are empty.*/
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
 * range is returned.*/
Range SimpleContour::Cut(const Edge &s) const
{
    Range ret;
    ret.MakeInvalid();
    XY dummy1[2];
    double pos[2], dummy2[2];
    for (unsigned u = 0; u<size(); u++)
        for (int i = at(u).Crossing(s, false, dummy1, dummy2, pos)-1; i>=0; i--)
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
        for (int i = at(u).Crossing(e, false, point, scpos, pos)-1; i>=0; i--) {
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
        for (int i = at(u).Crossing(s, false, dummy1, dummy2, pos)-1; i>=0; i--)
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