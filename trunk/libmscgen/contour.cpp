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
/** @file commands.cpp Implementation of composite contours.
 * @ingroup contour_files
 */
#include <cassert>
#include <cstdlib> //for abs
#include <limits>
#include "contour.h"
#include "contour_test.h"

//////ContourList/////////////////

namespace contour {


bool ContourList::operator <(const ContourList &b) const
{
    if (boundingBox != b.boundingBox) return boundingBox<b.boundingBox;
    if (size() != b.size()) return size()<b.size();
    for (auto i = begin(), j=b.begin(); i!=end(); i++, j++)
        if (!(*i==*j)) return *i<*j;
    return false;
}

bool ContourList::operator ==(const ContourList &b) const
{
    if (boundingBox != b.boundingBox || size() != b.size()) return false;
    for (auto i = begin(), j=b.begin(); i!=end(); i++, j++)
        if (!(*i==*j)) return false;
    return true;
}

void ContourList::Invert()
{
    for (auto &c : *this)
        c.Invert();
}

void ContourList::SwapXY()
{
    for (auto &c : *this)
        c.SwapXY();
    boundingBox.SwapXY();
}

EContourRelationType ContourList::RelationTo(const ContourWithHoles &c, bool ignore_holes) const
{
    if (IsEmpty())
        return c.IsEmpty() ? REL_BOTH_EMPTY : REL_A_IS_EMPTY;
    else if (c.IsEmpty())
        return REL_B_IS_EMPTY;

    if (!boundingBox.Overlaps(c.GetBoundingBox())) return REL_APART;

    EContourRelationType res = begin()->RelationTo(c, ignore_holes);
    switch (res) {
    default:
    case REL_A_IS_EMPTY:
    case REL_B_IS_EMPTY:
    case REL_BOTH_EMPTY:
    case REL_IN_HOLE_APART:
        _ASSERT(0);
    case REL_OVERLAP:
        return res;
    case REL_B_INSIDE_A:
    case REL_A_INSIDE_B:
    case REL_SAME:
    case REL_APART:
    case REL_B_IN_HOLE_OF_A:
    case REL_A_IN_HOLE_OF_B:
        break;
    }

    auto i = ++begin();
    while (i!=end()) {
        const EContourRelationType res2 = i->RelationTo(c, ignore_holes);
        if (res2 == res) continue;
        switch (res2) {
        case REL_A_IS_EMPTY:
        case REL_B_IS_EMPTY:
        case REL_BOTH_EMPTY:
        case REL_IN_HOLE_APART:
            _ASSERT(0);
        case REL_OVERLAP:
            return res;
        case REL_APART:
        case REL_B_IN_HOLE_OF_A:
        case REL_A_IN_HOLE_OF_B:
            if (result_overlap(res)) return REL_OVERLAP;
            else res = REL_IN_HOLE_APART;
            break;
        case REL_B_INSIDE_A:
        case REL_A_INSIDE_B:
        case REL_SAME:
            if (res == REL_SAME) res = res2;
            else return REL_OVERLAP;
            break;
        }
        ++i;
    }
    return res;
}

EContourRelationType ContourList::RelationTo(const ContourList &c, bool ignore_holes) const
{
    if (IsEmpty())
        return c.IsEmpty() ? REL_BOTH_EMPTY : REL_A_IS_EMPTY;
    else if (c.IsEmpty())
        return REL_B_IS_EMPTY;

    if (!boundingBox.Overlaps(c.GetBoundingBox())) return REL_APART;

    EContourRelationType res = c.RelationTo(*begin(), ignore_holes);
    switch (res) {
    default:
    case REL_A_IS_EMPTY:
    case REL_B_IS_EMPTY:
    case REL_BOTH_EMPTY:
        _ASSERT(0);
    case REL_OVERLAP:
        return res;
    case REL_B_INSIDE_A:
    case REL_A_INSIDE_B:
    case REL_SAME:
    case REL_APART:
    case REL_B_IN_HOLE_OF_A:
    case REL_A_IN_HOLE_OF_B:
    case REL_IN_HOLE_APART:
        break;
    }

    auto i = ++begin();
    while (i!=end()) {
        const EContourRelationType res2 = c.RelationTo(*i, ignore_holes);
        if (res2 == res) continue;
        switch (res2) {
        case REL_A_IS_EMPTY:
        case REL_B_IS_EMPTY:
        case REL_BOTH_EMPTY:
            _ASSERT(0);
        case REL_OVERLAP:
            return res;
        case REL_APART:
        case REL_B_IN_HOLE_OF_A:
        case REL_A_IN_HOLE_OF_B:
        case REL_IN_HOLE_APART:
            if (result_overlap(res)) return REL_OVERLAP;
            else res = REL_IN_HOLE_APART;
            break;
        case REL_B_INSIDE_A:
        case REL_A_INSIDE_B:
        case REL_SAME:
            if (res == REL_SAME) res = res2;
            else return REL_OVERLAP;
            break;
        }
        ++i;
    }
    return res;
}


/** Determine the relative vertical distance between our shapes and another one.
 *
 * For detailed explanation and parameters, see 
 * Contour::OffsetBelow(const Contour &below, double &touchpoint, double offset).
 */
double ContourList::OffsetBelow(const SimpleContour &below, double &touchpoint, double offset) const
{
    if (offset < below.GetBoundingBox().y.from - GetBoundingBox().y.till) return offset;
    if (!GetBoundingBox().x.Overlaps(below.GetBoundingBox().x)) return offset;
    for (auto &c : *this)
        offset = c.OffsetBelow(below, touchpoint, offset);
    return offset;
}

/** Determine the relative vertical distance between two list of shapes.
 *
 * For detailed explanation and parameters, see 
 * Contour::OffsetBelow(const Contour &below, double &touchpoint, double offset).
 */
double ContourList::OffsetBelow(const ContourList &below, double &touchpoint, double offset) const
{
    if (offset < below.GetBoundingBox().y.from - GetBoundingBox().y.till) return offset;
    if (!GetBoundingBox().x.Overlaps(below.GetBoundingBox().x)) return offset;
    for (auto &c : *this)
        for (auto &b : below)
            offset = c.OffsetBelow(b.outline, touchpoint, offset);
    return offset;
}

void ContourList::CairoPath(cairo_t *cr, bool show_hidden) const
{
    for (auto &c : *this)
        c.CairoPath(cr, show_hidden);
}

void ContourList::CairoPath(cairo_t *cr, bool show_hidden, bool clockwiseonly) const
{
    for (auto &c : *this)
        c.CairoPath(cr, show_hidden, clockwiseonly);
}

void ContourList::CairoPathDashed(cairo_t *cr, const double pattern[], unsigned num, bool show_hidden) const
{
    for (auto &c : *this)
        c.CairoPathDashed(cr, pattern, num, show_hidden);
}

void ContourList::CairoPathDashed(cairo_t *cr, const double pattern[], unsigned num, bool show_hidden, bool clockwiseonly) const
{
    for (auto &c : *this)
        c.CairoPathDashed(cr, pattern, num, show_hidden, clockwiseonly);
}

void ContourList::Distance(const ContourWithHoles &ch, DistanceType &dist_so_far) const
{
    if (IsEmpty() || ch.IsEmpty()) return;
    if (dist_so_far.IsZero()) return;
    const double bbdist = GetBoundingBox().Distance(ch.GetBoundingBox());
    if (!dist_so_far.ConsiderBB(bbdist)) {
        dist_so_far.MergeInOut(bbdist);
        return;
    }
    for (auto &c : *this) {
        const double bbdist2 = ch.GetBoundingBox().Distance(c.GetBoundingBox());
        if (dist_so_far.ConsiderBB(bbdist2))
            c.Distance(ch, dist_so_far);
        else
            dist_so_far.MergeInOut(bbdist2);
        if (dist_so_far.IsZero())
            break;
    }
}

void ContourList::Distance(const ContourList &cl, DistanceType &dist_so_far) const
{
    if (IsEmpty() || cl.IsEmpty()) return;
    if (dist_so_far.IsZero()) return;
    const double bbdist = GetBoundingBox().Distance(cl.GetBoundingBox());
    if (!dist_so_far.ConsiderBB(bbdist)) {
        dist_so_far.MergeInOut(bbdist);
        return;
    }
    dist_so_far.SwapPoints();
    for (auto &c : *this) {
        const double bbdist2 = c.GetBoundingBox().Distance(cl.GetBoundingBox());
        if (dist_so_far.ConsiderBB(bbdist2))
            cl.Distance(c, dist_so_far);
        else
            dist_so_far.MergeInOut(bbdist2);
        if (dist_so_far.IsZero())
            break;
    }
    dist_so_far.SwapPoints();
}



///ContoursHelper/////////////////////////////////////////////////////////


/** Implements the index of a doubly linked list */
struct link_info
{
    size_t next;
    size_t prev;
    static const size_t no_link;
};

const size_t link_info::no_link = std::numeric_limits<size_t>::max();

/** Stores data for a ray to aid walking.
 * @nosubgrouping
 * A ray is a half line going to or coming from a crosspoint.
 * For curvy edges, a ray is also curvy. If two edges cross, we have 4 resulting ray.
 * This structure contains a set of memebers identifying the ray, another set to 
 * link it to other rays and a third set to store the evaluation and walk status.*/
struct Ray
{
    /** @name Members for Identification
     * @{ */
    bool                 main_clockwise;///<True if the original Contour this edge is part of is clockwise. Unspecified for untangle operations (where the original Contour has no clockwiseness).
    const SimpleContour *contour;       ///<The SimpleContour the edge of the ray belongs to.
    size_t               vertex;        ///<The number of the vertex staring the edge the crosspoint is on
    double               pos;           ///<The `pos` of the crosspoint on the edge between [0..1). (-1 if it is a vertex and not a crosspoint.)
    bool                 incoming;      ///<True if the ray is incoming to the cp.
    RayAngle             angle;         ///<The angle of the ray.
    XY                   xy;            ///<The coordinates of the crosspoint of the ray.
    /** @} */
    /** @name Members for the Linked lists
     * @{ */
    enum list_type {
        LIST_CONTOUR, ///<The loose list linking the heads of the per-contour lists
        LIST_CP,      ///<The loose list linking the heads of the per-crosspoint lists
        IN_CONTOUR,   ///<The strict list linking all rays along a contour
        IN_CP         ///<The strict list linking all rays along a crosspoint in clockwise order
    }; ///<Enumeration describing linked lists for Rays.
    link_info link_contours;   ///<The loose list linking the heads of the per-contour lists
    link_info link_cps;        ///<The loose list linking the heads of the per-crosspoint lists
    link_info link_in_contour; ///<The strict list linking all rays along a contour
    link_info link_in_cp;      ///<The strict list linking all rays along a crosspoint in clockwise order
    /** Get the list identified by `type`. */
    link_info &get_index(list_type type) {return type==LIST_CONTOUR?link_contours:type==LIST_CP?link_cps:type==IN_CONTOUR?link_in_contour:link_in_cp;}
    /** @} */
    /** @name Members for Elvaluation and Status
     * Values used during any walk (both untangle and combine)
     * They are mutable so that updating status can be made on const Ray sturctures.
     * @{ */
    mutable bool   valid;     ///<False if this ray have already been included in a contour resulting from a walk.
    mutable size_t seq_num;   ///<Stop the walk if arriving to a ray with the same seq num as where we started the walk 
    mutable size_t switch_to; ///<Index of another ray for this cp, where the walk shoud be continued if arriving on this ray. `no_link` on error.
    /** @} */
    Ray(const XY &point, bool m_c, const SimpleContour *c, size_t v, double p, bool i, const RayAngle &a) :
        main_clockwise(m_c), contour(c), vertex(v), pos(p), incoming(i), angle(a),
        xy(point), valid(true), switch_to(link_info::no_link) {_ASSERT(c);}
    void Reset() const {valid = true; switch_to = link_info::no_link;}
};

/** Structure holding our position during walk.
 * 
 * We may be either at a vertex or a crosspoint
 */
struct RayPointer {
    bool   at_vertex;  ///<True if we are at a vertex, false if at a cp.
    size_t index;      ///Index of a ray. If we are at a vertex this points to a nearby ray on this contour
    size_t vertex;     ///the number of the vertex if we are at a vertex. Else ignore.
    explicit RayPointer(size_t i) : at_vertex(false), index(i) {}
};

/** To store info for walk backtracing.
 *
 * If we are unable to continue walk (cps are calculated badly) 
 * we need to backtrack. We store info for that here.
 */
struct walk_data {
    size_t rays_size;       ///<What was the size of the rays array before chosing
    size_t result_size;     ///<What was the size of the resulting contour before choosing
    size_t chosen_outgoing; ///<Which ray did we choose last time
    double terminating_pos; ///<Position-to-apply to the end of the previous edge
    walk_data(size_t s, size_t s2, size_t c, double p) :
        rays_size(s), result_size(s2), chosen_outgoing(c), terminating_pos(p) {}
};

/** Node of a tree holding SimpleContours.
 * 
 * Used for post-processing after walks to determine which SimpleContour is inside which others.
 */
struct node
{
    SimpleContour contour;    ///<This is part of the result.
    std::list<node> children; ///<These SimpleContours are inside the above member.
    node(SimpleContour &&c) : contour(std::move(c)) {}  ///<Move a SimpleContour
    node(const SimpleContour &c) : contour(c) {}        ///<Copy a Simplecontour
};


/** A helper class to implement contour operations and walking.
 * @ingroup contour_internal
 *
 * # Overview of the process
 *
 * This class is instantiated with one or two contours.
 * Note that they can be either const versions or move versions. We store
 * pointers to both along with flags if they were const or move versions.
 * The difference is important when parts of a contour (specifically a SimpleContour) 
 * has no cp ('cp' means crosspoint from now on) with any other. 
 * In that case it may (e.g. for unions) need to be copied to 
 * to the result. If its origin contour was a move version we do not copy, but move.
 *
 * ### Finding crosspoints
 *
 * After instantiation, we search for crosspoints between the edges comprising the two
 * contours. (Or between the edges of a single contour if we were instantiated with one.)
 * Currently this is done with pairwise checking (O(n^2)). For straight edges we could use
 * Bently-Ottoman this is a TODO. See FindCrosspoints().
 *
 * ### Evaluating crosspoints
 *
 * After we have identified all the crosspoints we evaluate them in the light of the
 * operation (see details at EvaluateCrosspoints()). Evaluation tells us for each
 * ray of a cp, which other ray shall we continue walking, if arrived on the first ray.
 * Evaluation also tells us which ray of which crosspoints can we start a walk from. 
 * See the example below.
 * @verbatim
 +-->----+
 |   A   |
 |     +-o-->-+
 |     | |    |
 +--<--x-+    |
       |   B  |
       +----<-+
 @endverbatim
 * Here we have two Contours A and B. Signs (`>` and `<`) show they are clockwise.
 * Netither have holes and both consist of only a single
 * SimpleContour. They have two cps (marked with `o` and `x`). Let us assume we take the union
 * of the two contours. In this case the `o` cp
 * is evaluated as follows. From the north ray, we have to switch to the east ray and vice versa.
 * Also, the east is eligible to start the walk with. (Starting at the north would yield in a 
 * counterclockwise result and here we are after a clockwise union. This is because we always
 * start *outwards* from a cp, irrespective of edge direction. Sometimes we have to walk *against*
 * edge direction, e.g., for XOR operations, thus we mostly ignore edge direction on walking.)
 * The other two rays should never be arrived on (they will not be part of the union, since they are
 * inside it), thus these are marked as *error*. Similar for `x` if we arrive on the west ray we shall 
 * continue on the south one and vice versa (and we can start with the west one); while the other two
 * represent errors.
 * For the case of the intersection operation, the east and south ray of `o` would be valid ones
 * where for `x` the north and east ones. See EvaluateCrosspoints().
 *
 * ### Walking
 *
 * After evaluation we start walking from one of the eligible rays. (See Walk().) We follow the edges
 * of the SimpleContour we are on until we hit a crosspoint. There we follow the result of the evaluation
 * for the ray we arrived to the crosspoint. Eventually we arrive back to where we started from.
 * (In fact we may terminate the walk after arriving back to another ray of the same cp. To this end,
 * rays in the cp are assigned a *sequence number*. If we arrive back to a ray with the same
 * sequence number we stop the walk. Note that at one crosspoint there may be rays with different 
 * sequence numbers. Sometimes we have to continue the walk even if we arrived back to the cp until
 * we get back on the right ray, see the example below at Untangling.)
 * Note that during walk we ignore the original direction of edges. Sometimes we have to walk them
 * in the opposite direction (for XOR, or see the example below for Untangling.)
 *
 * In the meantime we mark all rays we crossed, so that we do not start a walk resulting in this very
 * SimpleContour again. If there are eligible starting rays, we do another walk, as long as we have ones left -
 * each walk generating a separate, non-flipped, disjoint SimpleContour, which however, may be inside one another.
 * See for example the two shapes ('A' forming an upside-down U-shape and 'B' forming a rectangle) below 
 * and assume we do an intersection operation.
 * @verbatim
    +-------->-------------+
    |   +-----<-------+  A |
    |   |      H      |    |
  +-1---2------>------3----4---+
  | | M |             | N  |   |
  | +---+      B      +----+   |
  +-------------<--------------+
 @endverbatim
 * Four cps can be find (each having one eligible ray to start the walk, specifically
 * the east one for '1' and '3'; and the south one for '2' and '4'). 
 * The Contour resulting from the intersection will have two parts, marked with 'M' and 'N'.
 * Let's assume we started from the leftmost crosspoint (marked with `1`). 
 * During the walk we cross `2` and get back to '1' having
 * walked the SimpleContour 'M'. In order to obtain 'N' we need to start walking from one of the 
 * two remaining eligible rays. Starting from any of the two that walk will give us 'N'.
 *
 * ### Post-processing
 *
 * After completing all the walks we have a set of SimpleContours, whose edges do not cross. Note that 
 * even if we hunt for a clockwise union or intersection, some of them can be counterclockwise.
 * Take the above example and assume a union operation. In this case the result will have a hole
 * marked with 'H'. The eligible rays for the same 4 cps are north for '1' and '3'; east for '2' 
 * and '4'. Walking from '2' or '3' will walk around 'H' - but in a counterclockwise direction.
 * This is well and shows us 'H' will be a hole.
 *
 * The final step is to determine which of the resulting SimpleContours are contained in which other.
 * We also have to consider those parts of ther original contours that had no cps and therefore
 * were not walked. They are of course disjoint from the result of the walks and may either be added 
 * as is, or omitted completely. We determine the containment relationship (which contains whitch other)
 * and build a tree of `node` structure in Do() and lastly convert the tree to a Contour in ConvertNode().
 *
 * ## Untangling
 *
 * A few words about untangling. In this case we have only a single shape - which is not a 
 * valid contour, but we know that its neighbouring edges connect. There are two rules on which part the 
 * result shall contain.
 *     - *Winding* rule: If the original shape crosses the ray from left-to-right, counts +1. 
 *       If the path crosses the ray from right to left, counts -1. (Left and right are determined from the 
 *       perspective of looking along the ray from the starting point.) 
 *       If the total count is non-zero, the point will be included in the untangled shape.
 *     - *Even-odd* rule: Counts the total number of intersections, without regard to the orientation of the contour. 
 *       If the total number of intersections is odd, the point will be included in the untangled shape.
 *
 * See the below example.
 * @verbatim
   +--->---------------+  +--->---------------+   +--->---------------+  
   |                   |  |...................|   |...................|
   |      +-->---+     |  |...................|   |......+--<---+.....|
   |      |  B   |     |  |...................|   |......|      |.....|
   ^      +--<---o--<--+  ^.............+--<--+   ^..... +-->---+--<--+
   |  A          |        |...winding...|         |..even-odd...|
   +-------<-----+        +-------<-----+         +-------<-----+
 @endverbatim
 * In case the winding rule, the final shape would contain 'B', where in case of the evenodd rule
 * it would not, see the two shapes on the right, where dots indicate the areas covered. 
 * (The counter would be +2 for points inside 'B'.)
 *
 * Note that for the evenodd rule the direction of the edges of 'B' had been reversed. Also note that the crosspoint
 * is walked across two times: once when coming from the east and continuing north and the second
 * time when coming from the west and continuing south. This is one of the occasions when just by getting
 * back to the same crosspoint we shall not stop the walk. Assuming we start in the north direction from
 * this crosspoint (the only one eligible), we shall not stop the walk when we get back from the east.
 * Only when we get back from the west. This is achieved by assigning different sequence numbers
 * to the north/west rays and the east/south rays.
 * Finally note that we could stop the walk every time we get back to the same cp. In the above
 * example for evenodd that would result in a SimpleContour identical to the middle example and
 * a hole in it - that would also be a completely legal representation according to contour library principles.
 * We just do it the former way.
 *
 * Basically the process is very similar to the above one. We take crosspoints, evaluate and walk.
 * However, the crosspoints are taken between the edges of the single shape.
 * 
 * # Data Structures
 *
 * The data structure is a bit exotic here.
 * We have Ray. They represent half lines going outwards from or inwards to crosspoints.
 * In fact they can be curvy, for curvy edges.
 * When two edges cross (and form a cp), there are 4 rays: two incoming and two outgoing
 * (as edges are directed). Rays have an *angle* which is the clockwise angle from the positive x axis
 * and the outgoing tangent of the ray. The angle of the ray does not consider whether the ray is incoming
 * or outgoing. Thus for the examples above east rays (both incoming and outgoing) have angle of zero,
 * south rays the angle of 90 degrees, west rays 180 degrees and north rays 270 degrees. (Note that we
 * store the 'false angle' not the precise one as we just use it for soriting.) This angle is
 * used to sorth the rays around a crosspoint in clockwise order. To be able to sort curvy edges with the 
 * same tangent, we also store the curvyness of the ange, which is the inverse of the curve radius at the
 * crosspoint (thus zero for straight lines). See RayAngle.
 *
 * Rays are stored in a big vector (ContoursHelper::Rays) and are arranged in 4 doubly linked lists.
 * For each SimpleContour that is part of the operation we have a circular list that indexes rays according
 * to their order along the contour, by increasing <edge_no, pos_on_the_edge, incoming>.
 * (incoming rays are earlier in the list than their outgoing pair).
 * There is a separate such circular list for each SimpleContour, the head of such lists are the rays with
 * the smallest triplet for each contour.
 * In addition, the heads of the circular lists are themselves organized into another list (circular),
 * which links only the heads. This second list contains as many elements as there are SimpleContour with crosspoints.
 * I call the first list "strict" since it contains all rays (in one of the circular lists), and the
 * latter loose, as it only contains head rays.
 * 
 * Furthermore, there is a third list which lists rays associated with a crosspoint in clockwise order.
 * In fact they are ordered by <false angle, curve angle> and the head is the ray closest to <0, -inf>.
 * E.g., for a simple corsspoint of two edges crossing, we will have 4 rays in the circular list, two
 * for each edge. This is also a strict list, since all rays belong to one such list.
 * Finally, a fourth list links the heads of the per-crosspoint lists. This is a loose list. 
 * For loose lists: only heads of the strict lists are doubly linked, 
 * any other ray has its `next` member set to themselves and `prev` member set to the respective strict list head.
 * For strict lists: a separate circular list is maintained for each contour and cp. 
 * If `next==prev`, we are alone in our circular list.
 *
 * Each Ray structure contains four link_info members with `prev` and `next` members for the four lists, resp.
 * Rays that are not part of a loose list have these members set to "no_link" for that loose list.
 * This enables us to move both along the endges of a SimpleContour during walking and also to rotate around crosspoints easily.
 *
 * ### Ray groups
 * 
 * Rays are ordered clockwise around a crosspoint in their list. In this ordering we ignore
 * whether the ray is incoming or outgoing, we take its angle as if it were outgoing - 
 * horizontal right is angle 0 and angle grows clockwise (in a space where the y coordinate grows
 * downward). We use fake angles here with curvature values, see RayAngle.
 *
 * A ray group is a set of rays around a cp with exactly the same angle (within numeric precision)
 * and curvature angle.
 * This means that they lie exactly on each other. See the three examples below.
 * @verbatim
   +-->-----o=====o----+    +-->--o=====o->---+        +--->--+
   |        |  B  |    |    |  A  |     |     |        |  A   |
   |    A   +--<--+    |    +--<--x-----+     |     +--o======o--+
   +-----<-------------+          |        B  |     |      B     |
                                  +------<----+     +-----<------+
   @endverbatim
 * Here the two shapes have some parts of their edges common. In the first two case the common 
 * segments share the same direction, but in the third they are opposite. In all cases the 
 * crosspoints marked by `o` have three ray groups. Two of the ray groups have a single ray,
 * whereas a third one has two.
 *
 * What follows from the above definition is that if we have to continue 
 * the walk along one of the rays of a ray group, we can (at least in theory) pick any of them. 
 * The crosspoint we hit next will have to be on all such rays. This is unfortunately not always 
 * true, due to numeric imprecision. 
 * For this we use ClosestNextCP() to see which of the edges in a ray group has its next crosspoint
 * closest to the cp of the ray group and use sthat ray. Also, we have the idea of *backtracking*.
 *
 * ### Backtracking
 * 
 * During walk, we record a stack of decisions we made, that is selection between ray group members.
 * If later in the walk we end up in a ray with error, we backtrack and try other members of the ray group.
 * See Walk() and sturct walk_data for details.
 */
class ContoursHelper {
protected:
    const Contour * const C1, * const C2;     ///<The contours we process.
    const bool            const_C1, const_C2; ///<True if the contours cannot be touched. False, if we shall move out SimpleContours from these.
    const bool            clockwise_C1, clockwise_C2; ///<Store the clockwiseness status of C1 and C2 (save, as C1 and C2 may be destroyed)
    std::vector<Ray>      Rays;               ///<All the rays of an operation for all cps
    size_t                link_contours_head; ///<head of the contour loose list
    size_t                link_cps_head;      ///<head of the cp loose list
    mutable std::vector<size_t> StartRays;    ///<Indexes of rays where walks can start

private:
    /** Compare two rays based on their contour. 
     * Returns 0 if on same contour, +1/-1 if not. 
     Ordering is deterministic, but not defined. */
    int RayCompareBy_Contour(size_t r1, size_t r2) const
        {return Rays[r1].contour == Rays[r2].contour ? 0 : Rays[r1].contour < Rays[r2].contour ? -1 : +1; }
    /** Compare two rays based on (vertex, pos, incoming) triplets. 
     * If vertex & pos is equal, we put incoming as smaller than outgoing. 
     * Returns true if r1<r2. */
    bool RayCompareBy_VertexPosIncoming (size_t r1, size_t r2) const
        {return Rays[r1].vertex == Rays[r2].vertex ? test_equal(Rays[r1].pos, Rays[r2].pos) ? Rays[r1].incoming == Rays[r2].incoming ? false : Rays[r1].incoming : Rays[r1].pos<Rays[r2].pos : Rays[r1].vertex<Rays[r2].vertex;}
    /** Compare two rays based on their crosspoint. 
     * Returns 0 if on same CP, +1/-1 if not. 
     Ordering is deterministic, but not defined. */
    int RayCompareBy_CP(size_t r1, size_t r2) const
        {return Rays[r1].xy.test_equal(Rays[r2].xy) ? 0 : Rays[r1].xy < Rays[r2].xy ? -1 : +1; }
    /** Compare two rays based on (angle, contour, vertex, pos, incoming) sextets. 
     * Returns true if r1<r2. */
    bool RayCompareBy_Angle(size_t r1, size_t r2) const
        {return Rays[r1].angle.IsSimilar(Rays[r2].angle) ? Rays[r1].contour == Rays[r2].contour ? RayCompareBy_VertexPosIncoming(r1, r2) : Rays[r1].contour < Rays[r2].contour : Rays[r1].angle.Smaller(Rays[r2].angle);}

protected:
    /**Compares two rays for the purpose of inserting them into a loose list.*/
    int RayCompareLoose(size_t r1, size_t r2, Ray::list_type type) const {return type==Ray::LIST_CONTOUR ? RayCompareBy_Contour(r1,r2) : RayCompareBy_CP(r1,r2);}
    /**Compares two rays for the purpose of inserting them into a strict list.*/
    bool RayCompareStrict(size_t r1, size_t r2, Ray::list_type type) const {return type==Ray::IN_CONTOUR ? RayCompareBy_VertexPosIncoming(r1,r2) : RayCompareBy_Angle(r1,r2);}
    /**Links a ray `index` to the loose list of type `type. Returns the head of the strict list we need to use.*/
    size_t InsertToLooseList(Ray::list_type type, size_t index);
    /**Links a ray `index` to the loose list of type `type' with head of `head`.*/
    bool InsertToStrictList(Ray::list_type type, size_t index, size_t head);
    /** Finds the cp_head of this ray */
    size_t FindCPHead(size_t r) const;
    /**Returns true if `c` has any crosspoints (any ray having it as contour)*/
    bool IsContourInRays(const SimpleContour *c) const;
    bool AddCrosspointHelper(const XY &point, bool m_c, const SimpleContour *c, size_t v, double p, bool i, const RayAngle &a);
    void AddCrosspoint(const XY &xy, bool m_c1, const SimpleContour *c1, size_t v1, double p1, bool m_c2, const SimpleContour *c2, size_t v2, double p2);
    /**Find the crosspoints between the edges of `i`, adds them to 'Rays' and returns the number of crosspoints found.*/
    unsigned FindCrosspointsHelper(const SimpleContour *i);
    /**Find the crosspoints between the edges of `i`, adds them to 'Rays' and returns the number of crosspoints found.*/
    unsigned FindCrosspointsHelper(const ContourWithHoles *i);
    /**Find the crosspoints between the edges of shapes in `c`, adds them to 'Rays' and returns the number of crosspoints found.
     * If "cwh_included" is false we do not test individual ContourWithHoles in the list for self-crossing, 
     * just pairwise touch between the list elements */
    unsigned FindCrosspointsHelper(const ContourList *c, bool cwh_included);
    /**Find the crosspoints between the edges of `c`, adds them to 'Rays' and returns the number of crosspoints found.
     * If "cwh_included" is false we do not test individual ContourWithHoles for self-crossing, 
     * just pairwise touch between the ContourWithHoles this Contour consists of.*/
    unsigned FindCrosspointsHelper(const Contour *c, bool cwh_included);
    /**Finds the crosspoints between `c1` and `c2`, adds them to 'Rays' and returns the number of crosspoints.
     * `m_c1` and `m_c2` are true if the main Contour of them is clockwise, respectively. */
    unsigned FindCrosspointsHelper(bool m_c1, const SimpleContour *c1, bool m_c2, const SimpleContour *c2);
    /**Finds the crosspoints between `c1` and `c2`, adds them to 'Rays' and returns the number of crosspoints.
     * `m_c1` and `m_c2` are true if the main Contour of them is clockwise, respectively. */
    unsigned FindCrosspointsHelper(bool m_c1, const ContourWithHoles *c1, bool m_c2, const ContourWithHoles *c2);
    /**Finds the crosspoints between `cwh` and `cs`, adds them to 'Rays' and returns the number of crosspoints.
     * `m_c1` and `m_c2` are true if the main Contour of them is clockwise, respectively. */
    unsigned FindCrosspointsHelper(bool m_c1, const ContourWithHoles *cwh, bool m_c2, const ContourList *cs);
    /**Finds the crosspoints between `c1` and `c2`, adds them to 'Rays' and returns the number of crosspoints.
     * `m_c1` and `m_c2` are true if the main Contour of them is clockwise, respectively. */
    unsigned FindCrosspointsHelper(bool m_c1, const ContourList *c1, bool m_c2, const ContourList *c2);
    /**Finds the crosspoints between C1 and C2 or just in C1. Adds them to 'Rays' & return their number.*/
    unsigned FindCrosspoints();  
    bool OperationWithAnEmpty(Contour::EOperationType type, bool clockwise) const;
    bool IsCoverageToInclude(int cov, Contour::EOperationType type) const;
    size_t FindRayGroupEnd(size_t from, int &coverage, size_t abort_at1, size_t abort_at2) const;
    bool GoToCoverage(size_t &from, size_t &to, int &cov_now, Contour::EOperationType type, bool start, size_t abort_at) const;
    int  CalcCoverageHelper(const XY &xy, size_t cp_head, const ContourWithHoles *cwh) const;
    int  CalcCoverageHelper2(const XY &xy, size_t cp_head, const ContourWithHoles *cwh) const;
    int  CalcCoverageHelper(size_t cp_head) const;
    int  CalcCoverageHelper(const SimpleContour *sc) const;
    size_t ClosestNextCP(size_t from, size_t to) const;
    void ResetCrosspoints() const;    //make them valid and set switch_action to ERROR
    void EvaluateCrosspoints(Contour::EOperationType type) const; //Fills in switch actions and StartRays
    //helpers for walking
    void Advance(RayPointer &p, bool forward) const;
    void MarkAllInRayGroupOf(size_t ray, bool valid) const;
    void RevalidateAllAfter(std::vector<size_t> &ray_array, size_t from) const;
    SimpleContour Walk(RayPointer start) const;

    //helper for post-processing
    int CoverageInNodeList(const std::list<node> &list, const SimpleContour &c) const;
    node *InsertContour(std::list<node> *list, node &&n) const;
    void InsertIfNotInRays(std::list<node> *list, const ContourWithHoles *c, bool const_c,
                           const Contour *C_other, Contour::EOperationType type) const;
    void ConvertNode(Contour::EOperationType type, std::list<node> &&list, Contour &result, bool positive) const;
    void ConvertNode(Contour::EOperationType type, std::list<node> &&list, ContourList &result, bool positive) const;

public:
    //external interface
    ContoursHelper(const Contour &c) : C1(&c), C2(NULL), const_C1(true), const_C2(true), clockwise_C1(c.GetClockWise()), clockwise_C2(false) {FindCrosspoints();}
    ContoursHelper(const Contour &c1, const Contour &c2) : C1(&c1), C2(&c2), const_C1(true), const_C2(true), clockwise_C1(c1.GetClockWise()), clockwise_C2(c2.GetClockWise()) {FindCrosspoints();}
    ContoursHelper(const Contour &c1, Contour &&c2) : C1(&c1), C2(&c2), const_C1(true), const_C2(false), clockwise_C1(c1.GetClockWise()), clockwise_C2(c2.GetClockWise()) {FindCrosspoints();}
    ContoursHelper(Contour &&c1, const Contour &c2) : C1(&c1), C2(&c2), const_C1(false), const_C2(true), clockwise_C1(c1.GetClockWise()), clockwise_C2(c2.GetClockWise()) {FindCrosspoints();}
    ContoursHelper(Contour &&c1, Contour &&c2) : C1(&c1), C2(&c2), const_C1(false), const_C2(false), clockwise_C1(c1.GetClockWise()), clockwise_C2(c2.GetClockWise()) {FindCrosspoints();}
    void Do(Contour::EOperationType type, Contour &result) const;
};

//returns the head of the strict list
size_t ContoursHelper::InsertToLooseList(Ray::list_type type, size_t index)
{
    _ASSERT(type==Ray::LIST_CONTOUR || type==Ray::LIST_CP); //assume a loose list
    if (index==0) {//we are first to be inserted
        Rays[0].get_index(type).next = 0;
        Rays[0].get_index(type).prev = 0;
        return 0;
    }
    size_t &list_head = type==Ray::LIST_CONTOUR ? link_contours_head : link_cps_head;
    size_t now = list_head;
    //search until now is smaller and we have a valid "next"
    while (RayCompareLoose(now, index, type) < 0 &&  Rays[now].get_index(type).next != list_head)
        now = Rays[now].get_index(type).next;
    switch (RayCompareLoose(now, index, type)) {
    case -1: //now is still smaller than index, we stopped because we are at the end of the loose list
        Rays[now].get_index(type).next = index;
        Rays[list_head].get_index(type).prev = index;
        Rays[index].get_index(type).prev = now;
        Rays[index].get_index(type).next = list_head;
        return index;
    case 0: //we found a head, set next to head and prev to ourself -> we are not part of the loose list
        Rays[index].get_index(type).prev = link_info::no_link;
        Rays[index].get_index(type).next = link_info::no_link;
        return now;  //the element we have found - the head of the strict list
    case +1: //no such entry yet, link it in before "now"
        if (Rays[now].get_index(type).next == now) { //single element so far
            Rays[now].get_index(type).next = index;
            Rays[now].get_index(type).prev = index;
            Rays[index].get_index(type).prev = now;
            Rays[index].get_index(type).next = now;
        } else {
            size_t prev = Rays[now].get_index(type).prev;
            Rays[now].get_index(type).prev = index;
            Rays[prev].get_index(type).next = index;
            Rays[index].get_index(type).prev = prev;
            Rays[index].get_index(type).next = now;
        }
        //Check if we inserted to the beginning of the list. If so, update head of the loose list
        if (list_head == now)
            list_head = index;
        return index; //we became head of the
    default:
        _ASSERT(0);
        return false;
    }
}

//return true if we inserted a new element
bool ContoursHelper::InsertToStrictList(Ray::list_type type, size_t index, size_t head)
{
    _ASSERT(type==Ray::IN_CONTOUR || type==Ray::IN_CP); //assume a strict list
    if (head==index) {
        //initialze new strict list
        Rays[index].get_index(type).prev = index;
        Rays[index].get_index(type).next = index;
        return true;
    }
    size_t now=head;
    //search until now is smaller and we have a valid "next"
    while (RayCompareStrict(now, index, type) && Rays[now].get_index(type).next != head)
        now = Rays[now].get_index(type).next;
    if (RayCompareStrict(now, index, type)) {
        //now < index: insert after (at the end, but a circular buffer, so does not matter)
        Rays[now].get_index(type).next = index;
        Rays[head].get_index(type).prev = index;
        Rays[index].get_index(type).prev = now;
        Rays[index].get_index(type).next = head; //this was the original "next" of "now"
        return true;
    }
    //Now we know that now>=index
    //If now>index, insert before, else they are == and we shall skip
    if (RayCompareStrict(index, now, type)) {
        //ok index<now, instert before "now"
        size_t prev = Rays[now].get_index(type).prev;
        Rays[now].get_index(type).prev = index;
        Rays[prev].get_index(type).next = index;
        Rays[index].get_index(type).prev = prev;
        Rays[index].get_index(type).next = now;
        //check if we insert to the beginning of the strict list
        if (head == now) {
            //if so, move list head to "index"
            size_t &loose_list_head = type==Ray::IN_CONTOUR ? link_contours_head : link_cps_head;
            const Ray::list_type type_loose = type==Ray::IN_CONTOUR ? Ray::LIST_CONTOUR : Ray::LIST_CP;
            const size_t loose_prev = Rays[head].get_index(type_loose).prev;
            const size_t loose_next = Rays[head].get_index(type_loose).next;
            if (loose_prev == head) {
                //A loose list of a single element
                _ASSERT(head==loose_list_head);
                //set the links of the new head
                Rays[index].get_index(type_loose).next = index;
                Rays[index].get_index(type_loose).prev = index;
            } else {
                //set the links of the prev and next elements in the loose list
                Rays[loose_prev].get_index(type_loose).next = index;
                Rays[loose_next].get_index(type_loose).prev = index;
                //set the links of the new head
                Rays[index].get_index(type_loose).next = loose_next;
                Rays[index].get_index(type_loose).prev = loose_prev;
            }
            //set the links of the old head -> no longer part of the loose list
            Rays[head].get_index(type_loose).next = link_info::no_link;
            Rays[head].get_index(type_loose).prev = link_info::no_link;
            //Check if the head of the loose list has changed
            if (head==loose_list_head)
                loose_list_head = index;
        }
        return true;
    }
    return false;
}

size_t ContoursHelper::FindCPHead(size_t r) const
{
    if (r == link_info::no_link) {
        _ASSERT(0);
        return r;
    }
    while (Rays[r].link_cps.next != link_info::no_link)
        r = Rays[r].link_cps.next;
    return r;
}

//Finds the head of the strict list for a contour
bool ContoursHelper::IsContourInRays(const SimpleContour *c) const
{
    size_t u = link_contours_head;
    do {
        if (Rays[u].contour == c) return true;
        u = Rays[u].link_contours.next;
    } while (u!=link_contours_head);
    return Rays[u].contour == c;
}

/** Adds and links a ray. 
 *
 * This is a helper, it shall be called four times for each crosspoint.
 * We check for already inserted rays and do not insert duplicate ones.
 * @param [in] point The coordinate of the crosspoint of the ray.
 * @param [in] m_c True if the main Contour this ray belongs to was clockwise.
 * @param [in] c The SimpleContour the ray belongs to.
 * @param [in] v The index of the edge the ray is at.
 * @param [in] p The pos of the crosspoint on the edge.
 * @param [in] i True if the ray is incoming into the cp.
 * @param [in] a The angle of the ray (as if it were outgoing).
 * @returns true if a ray already exists with the same <contour, vertex, pos, incoming>.*/
bool ContoursHelper::AddCrosspointHelper(const XY &point, bool m_c, const SimpleContour *c, size_t v, double p, bool i, const RayAngle &a)
{
    //_ASSERT(c->at(v).Pos2Point(p).test_equal(point));
    Rays.push_back(Ray(point, m_c, c, v, p, i, a));
    //link in to the loose lists
    const size_t index = Rays.size()-1;
    const size_t contour_head = InsertToLooseList(Ray::LIST_CONTOUR, index);
    //Insert to strict list of the contours. Stop if already there
    if (!InsertToStrictList(Ray::IN_CONTOUR, index, contour_head)) {
        //OK, this <contour, vertex, pos, incoming> already exists
        //in this case loose lists were not modified, we can safely delete the back
        Rays.pop_back();
        return false;
    }
    const size_t cp_head      = InsertToLooseList(Ray::LIST_CP     , index);
    InsertToStrictList(Ray::IN_CP, index, cp_head);
    return true;
}

/** Adds and links all for rays for a crosspoint.
 *
 * This shall be called for all cps, when found.
 * In this function we avoid incoming rays with pos==0, we use the previous edge and pos==1.
 * We expect the position to never equal to 1 (those should be reported as position 0 at
 * the next edge). We check for doubly inserted cps and ignore them.
 * @param [in] xy The coordinate of the crosspoint.
 * @param [in] m_c1 True if the main Contour of c1 is clockwise.
 * @param [in] c1 One of the SimpleContour the cp belongs to.
 * @param [in] v1 The index of the edge of c1 the cp is on.
 * @param [in] p1 The pos of the crosspoint on edge v1.
 * @param [in] m_c2 True if the main Contour of c2 is clockwise.
 * @param [in] c2 The other of the SimpleContour the cp belongs to (can be the same as c1 at untangle).
 * @param [in] v2 The index of the edge of c2 the cp is on.
 * @param [in] p2 The pos of the crosspoint on edge v2.*/
void ContoursHelper::AddCrosspoint(const XY &xy, bool m_c1, const SimpleContour *c1, size_t v1, double p1,
                                                 bool m_c2, const SimpleContour *c2, size_t v2, double p2)
{
    _ASSERT(p1<1 && p2<1 && p1>=0 && p2>=0);
    //In allrays even indexed positions are incoming edges, odd positions are outgoing ones
    if (p1==0)  //avoid pos==0 and incoming
        AddCrosspointHelper(xy, m_c1, c1, v1, p1, true,  c1->at_prev(v1).Angle(true, 1));
    else
        AddCrosspointHelper(xy, m_c1, c1, v1, p1, true,  c1->at(v1).Angle(true, p1));
    //if (p1==1) //avoid pos==1 and outgoing
    //    AddCrosspointHelper(xy, m_c1, c1, v1, p1, false, c1->at_next(v1).Angle(false, 0));
    //else
    AddCrosspointHelper(xy, m_c1, c1, v1, p1, false, c1->at(v1).Angle(false, p1));
    if (c2==NULL) return;
    if (p2==0)  //avoid pos==0 and incoming
        AddCrosspointHelper(xy, m_c2, c2, v2, p2, true,  c2->at_prev(v2).Angle(true, 1));
    else
        AddCrosspointHelper(xy, m_c2, c2, v2, p2, true,  c2->at(v2).Angle(true, p2));
    //if (p2==1) //avoid pos==1 and outgoing
    //    AddCrosspointHelper(xy, m_c2, c2, v2, p2, false, c2->at_next(v2).Angle(false, 0));
    //else
    AddCrosspointHelper(xy, m_c2, c2, v2, p2, false, c2->at(v2).Angle(false, p2));
}


//finds all crosspoints between all edges of this SimpleContour (except vertices)
unsigned ContoursHelper::FindCrosspointsHelper(const SimpleContour *i)
{
    unsigned ret=0;
    XY r[Edge::MAX_CP];
    double one_pos[Edge::MAX_CP], two_pos[Edge::MAX_CP];
    //We need to check edges subsequent to each other, since crazy circles may cross at places
    //more than their endpoint. Assumedly Crossing() will not return pos==1 crosspoints
    //so we will not get normal vertices back
    for (size_t u1 = 1; u1<i->size(); u1++)
        for (size_t u2 = 0; u2<u1; u2++) {
            const unsigned n = i->at(u2).Crossing(i->at(u1), i->next(u2)==u1, r, two_pos, one_pos);
            for (unsigned k=0; k<n;k++) {
                //main_clockwise values are dummy
                //_ASSERT(i->at(u1).Pos2Point(one_pos[k]).test_equal(r[k]));
                //_ASSERT(i->at(u2).Pos2Point(two_pos[k]).test_equal(r[k]));
                _ASSERT(i->at(u1).Pos2Point(one_pos[k]).DistanceSqr(r[k])<0.1);
                _ASSERT(i->at(u2).Pos2Point(two_pos[k]).DistanceSqr(r[k])<0.1);
                //skip crosspoints that are vertices: one edge joins the next
                if (one_pos[k]==1 && two_pos[k]==0 && i->next(u1)==u2) continue;
                if (one_pos[k]==0 && two_pos[k]==1 && i->next(u2)==u1) continue;
                //change vertex crosspoints which are detected at the _end_
                //of an edge to be expressed as being at the beginning of the
                //following edge.
                //This is to avoid a vertex cp to be added twice (in case it is detected
                //again for the following edge).
                AddCrosspoint(r[k], bool(), &*i, one_pos[k]==1 ? i->next(u1) : u1,
                                                 one_pos[k]==1 ? 0 : one_pos[k], 
                                    bool(), &*i, two_pos[k]==1 ? i->next(u2) : u2,
                                                 two_pos[k]==1 ? 0 : two_pos[k]);
            }
            ret += n;
        }
    for (size_t u1 = 1; u1<i->size(); u1++) {
        const unsigned n = i->at(u1).SelfCrossing(r, one_pos, two_pos);
        _ASSERT(n<=1);
        if (n==1 && !(one_pos))
            AddCrosspoint(r[0], bool(), &*i, one_pos[0]==1 ? i->next(u1) : u1,
                                             one_pos[0]==1 ? 0 : one_pos[0], 
                                bool(), &*i, two_pos[0]==1 ? i->next(u1) : u1,
                                             two_pos[0]==1 ? 0 : two_pos[0]);
    }
    return ret;
}

//finds all crosspoints between all simplecontours found in this tree.
unsigned ContoursHelper::FindCrosspointsHelper(const ContourWithHoles *i)
{
    //Find intersections of the main contour with itself
    unsigned ret = FindCrosspointsHelper(&i->outline);
    //Now see where the main contour meets its holes
    for (const auto &h : i->holes)
        //main_clockwise values are dummy
        ret += FindCrosspointsHelper(bool(), &i->outline, bool(), &h.outline);
    return ret + FindCrosspointsHelper(&i->holes, true);
}

//Finds all crosspoints between any two contours and among the edges of each
//If "cwh_included" is false we do not test individual contours in the list,
//just pairwise touch between the list elements
unsigned ContoursHelper::FindCrosspointsHelper(const ContourList *c, bool cwh_included)
{
    unsigned ret=0;
    //We ignore the holes, just calculate
    for (auto i = c->begin(); i!=c->end(); i++) {
        //see where i crosses itself, including its holes
        if (cwh_included)
            ret += FindCrosspointsHelper(&*i);
        auto j = i;
        //now see where i crosses others (including holes)
        for (++j; j!=c->end(); j++) {
            if (i->GetBoundingBox().Overlaps(j->GetBoundingBox()))
                //main_clockwise values are dummy if we do a single contour operation
                //but if we do multi-contour, we shall use the clockwiseness of these
                ret += FindCrosspointsHelper(i->GetClockWise(), &*i, j->GetClockWise(), &*j);
        }
    }
    return ret;
}

//Finds all crosspoints between any two contours and among the edges of each
//If "cwh_included" is false we do not test individual contours in the list,
//just pairwise touch between the list elements
inline unsigned ContoursHelper::FindCrosspointsHelper(const Contour *c, bool cwh_included)
{
    unsigned ret = 0;
    if (cwh_included)
        ret = FindCrosspointsHelper(&c->first);
    if (c->further.size()) {
        ret += FindCrosspointsHelper(c->GetClockWise(), &c->first, c->GetClockWise(), &c->further);
        ret += FindCrosspointsHelper(&c->further, cwh_included);
    }
    return ret;
}


//finds all crosspoints between the two arguments, considering holes, too
unsigned ContoursHelper::FindCrosspointsHelper(bool m_c1, const SimpleContour *i1,
                                               bool m_c2, const SimpleContour *i2)
{
    unsigned ret=0;
    XY r[Edge::MAX_CP];
    double one_pos[Edge::MAX_CP], two_pos[Edge::MAX_CP];
    for (size_t u1 = 0; u1<i1->size(); u1++)
        for (size_t u2 = 0; u2<i2->size(); u2++) {
            const unsigned n = i1->at(u1).Crossing(i2->at(u2), false, r, one_pos, two_pos);
            for (unsigned k = 0; k<n; k++) 
                //change vertex crosspoints which are detected at the _end_
                //of an edge to be expressed as being at the beginning of the
                //following edge.
                //This is to avoid a vertex cp to be added twice (in case it is detected
                //again for the following edge).
                AddCrosspoint(r[k], m_c1, i1, one_pos[k]==1 ? i1->next(u1) : u1, 
                                              one_pos[k]==1 ? 0 : one_pos[k], 
                                    m_c2, i2, two_pos[k]==1 ? i2->next(u2) : u2,
                                              two_pos[k]==1 ? 0 : two_pos[k]);
            ret += n;
        }
    return ret;
}

//finds all crosspoints between the two arguments, considering holes, too
unsigned ContoursHelper::FindCrosspointsHelper(bool m_c1, const ContourWithHoles *i1,
                                               bool m_c2, const ContourWithHoles *i2)
{
    //see where the two main covers meet
    unsigned ret = FindCrosspointsHelper(m_c1, &i1->outline,
                                         m_c2, &i2->outline);
    //now see how the holes cross the other's main cover
    for (auto i = i1->holes.begin(); i!=i1->holes.end(); i++)
        if (i->GetBoundingBox().Overlaps(i2->GetBoundingBox()))
            ret += FindCrosspointsHelper(m_c1, &*i, m_c2, i2);
    for (auto i = i2->holes.begin(); i!=i2->holes.end(); i++)
        if (i->GetBoundingBox().Overlaps(i1->GetBoundingBox()))
            ret += FindCrosspointsHelper(m_c1, i1, m_c2, &*i);
    //And now see how the holes meet each other
    if (i1->holes.GetBoundingBox().Overlaps(i2->holes.GetBoundingBox()))
        ret += FindCrosspointsHelper(m_c1, &i1->holes, m_c2, &i2->holes);
    return ret;
}

//finds all crosspoints between the two arguments, considering holes, too
unsigned ContoursHelper::FindCrosspointsHelper(bool m_c1, const ContourWithHoles *cwh,
                                               bool m_c2, const ContourList *cs)
{
    unsigned ret=0;
    for (auto i = cs->begin(); i!=cs->end(); i++)
        ret += FindCrosspointsHelper(m_c1, cwh, m_c2, &*i);
    return ret;
}


unsigned ContoursHelper::FindCrosspointsHelper(bool m_c1, const ContourList *c1,
                                               bool m_c2, const ContourList *c2)
{
    unsigned ret=0;
    for (auto i1 = c1->begin(); i1 != c1->end(); i1++)
        for (auto i2 = c2->begin(); i2 != c2->end(); i2++)
            //Search for cps only if the two main contour overlaps
            if (i1->GetBoundingBox().Overlaps(i2->GetBoundingBox()))
                ret += FindCrosspointsHelper(m_c1, &*i1, m_c2, &*i2);
    return ret;
}

unsigned ContoursHelper::FindCrosspoints()
{
    //Reset stuff
    Rays.clear();
    link_cps_head = link_contours_head = 0;

    unsigned ret = 0;
    Rays.reserve(200);
    if (C2==NULL) {
        ret = FindCrosspointsHelper(C1, true);
    } else if (C1->GetBoundingBox().Overlaps(C2->GetBoundingBox())) {
        ret = FindCrosspointsHelper(C1->GetClockWise(), &C1->first,
                                    C2->GetClockWise(), &C2->first);
        if (C2->further.GetBoundingBox().Overlaps(C1->GetBoundingBox()))
            ret += FindCrosspointsHelper(C1->GetClockWise(), &C1->first,
                                         C2->GetClockWise(), &C2->further);
        if (C1->further.GetBoundingBox().Overlaps(C2->GetBoundingBox()))
            ret += FindCrosspointsHelper(C2->GetClockWise(), &C2->first,
                                         C1->GetClockWise(), &C1->further);
        if (C2->further.GetBoundingBox().Overlaps(C1->further.GetBoundingBox()))
            ret += FindCrosspointsHelper(C1->GetClockWise(), &C1->further,
                                         C2->GetClockWise(), &C2->further);
        //Here we also need to test if there are ContourWithHoles:s inside C1 and C2 that touch
        //ret += FindCrosspointsHelper(C1, false);
        //ret += FindCrosspointsHelper(C2, false);
    }
    return ret;
}

/**After this you can call again EvaluateCrosspoints with a different operation*/
void ContoursHelper::ResetCrosspoints() const
{
    StartRays.clear();
    for (auto i = Rays.begin(); i!=Rays.end(); i++)
        i->Reset();
}

/** Returns wether to include a part of a shape.
 * 
 * Specifically : 
 * - If union, pick the angle ranges covered by at least one contour
 * - If intersect, pick angle ranges covered by all of the contours
 * - If xor, we pick ranges of odd coverage
 * - for untangle we formulate winding rules as they say
 *
 * Note that a clockwise surface has coverage 1 inside and zero outside
 * a counterclockwise surface has coverage -1 inside and zero outside
 * thus we have the following non-intuitive results (`+` is union, `*`
 * is intersection, 'hole' is counterclockwise, 'surface' is clockwise')
 * - hole1 + hole2 = inv(inv(hole1) + inv(hole2))
 * - hole1 * hole  = inv(inv(hole1) * inv(hole2))
 * - hole * surface = 0
 * - surface - surface = surface1 + inv(surface2) and not surface1 * inv(surface2) 
 * @param [in] cov The total cumulative coverage at the area. E.g., if two positive (clockwise)
 *                 surface cover an area, coverage there is +2.
 * @param [in] type The type of operation we perform.
 * @returns True if the area shall be included in the result of the operation.*/
inline bool ContoursHelper::IsCoverageToInclude(int cov, Contour::EOperationType type) const
{
    switch (type) {
    default: _ASSERT(0); /*fallthrough*/
    case Contour::POSITIVE_INTERSECT:   return cov>=2;
    case Contour::NEGATIVE_INTERSECT:   return cov<=-2;
    case Contour::POSITIVE_UNION:       return cov>=1;
    case Contour::NEGATIVE_UNION:       return cov<=-1;
    case Contour::POSITIVE_XOR:         return cov==1;
    case Contour::NEGATIVE_XOR:         return cov==-1;
    case Contour::WINDING_RULE_NONZERO: return cov!=0;
    case Contour::WINDING_RULE_EVENODD: return abs(cov)%2==1; //here we can have cov<0 & we may keep it
    case Contour::EXPAND_POSITIVE:      return cov>0;
    case Contour::EXPAND_NEGATIVE:      return cov<0;
    }
}

/** Returns the ray after the current ray-group.
 * 
 * A ray group is a set of rays of the same cp where RayAngle is the same.
 * This functions steps ahead in the per cp list until it hits a ray with a different RayAngle.
 * @param [in] from The index of the ray we start the search from.
 * @param coverage When calling it shall be the coverage of the area just before from. 
 *                 On return it will contain the coverage just before the returned ray.
 * @param [in] abort_at1 Stop searching if hitting this ray. Return this as result (with coverage set accordingly)
 * @param [in] abort_at2 Stop searching if hitting this ray. Return this as result (with coverage set accordingly)
 * @returns The index of the ray following the ray group of from.*/
inline size_t ContoursHelper::FindRayGroupEnd(size_t from, int &coverage, size_t abort_at1, size_t abort_at2) const
{
    size_t to = from;
    do {
        if (Rays[to].incoming) coverage--;
        else coverage++;
        to = Rays[to].link_in_cp.next;
    } while (to != from && to!=abort_at1 && to!=abort_at2 && Rays[from].angle.IsSimilar(Rays[to].angle));
    return to;
}

/** Walk around a cp to a ray group where inclusion becomes true or false.
 *
 * If `start` is true, go to the ray group *after* which coverage becomes eligible for inclusion
 *   (that is, for intersects >=no of contours, for union >=1 and for xor odd; see IsCoverageToInclude()).
 * Else go to the ray group *after* which coverage becomes ineligible.
 * @param from At calling this is the beginning of the search. At return this is the first element of the
 *             ray group returned.
 * @param [out] to This is the first element after the ray group returned.
 * @param cov_now The coverage just before ray "from" at calling. The coverage just before "to" is returned.
 * @param [in] type The type of operation.
 * @param [in] start If true we search for a ray group after which coverage becomes eligible, else ineligible.
 * @param [in] abort_at Stop search here. 'to' will be set to this (and 'cov_now' is set appropriately).
 * @return True if coverage is eligible/ineligible just before to as requested by 'start'. */
bool ContoursHelper::GoToCoverage(size_t &from, size_t &to, int &cov_now, Contour::EOperationType type, bool start, size_t abort_at) const
{
    const size_t started = to = from;
    do {
        from = to;
        to = FindRayGroupEnd(from, cov_now, started, abort_at);
    } while (to!=started && to!=abort_at && start != IsCoverageToInclude(cov_now, type));
    return start == IsCoverageToInclude(cov_now, type);
}

/** Calculates the coverage of a ContourWithHoles close to a point in the <0, -inf> direction assuming an untangle operation.
 *
 * Holes are considered just like the outline.
 * @param [in] xy The coordinates of the point
 * @param [in] cp_head If the point is a cp in 'Rays' the head ray for the crosspoint. 
 *                     Else link_info::no_link.
 * @param [in] cwh The shape for which coverage is calculated.
 * @returns The coverage just a bit right-up from the cp, just before a hypothetical ray with RayAngle of <0,-inf>.*/
int ContoursHelper::CalcCoverageHelper(const XY &xy, size_t cp_head, const ContourWithHoles *cwh) const
{
    _ASSERT(cp_head == link_info::no_link || Rays[cp_head].xy == xy);
    //For each edge, we see if it crosses the (xy.x, xy.y)->(inf, xy.y) line
    //if it crosses clockwise: +1 cov
    //if it crosses cclockwise: -1 cov
    //if it is an ellipse that crosses twice and not in xy:
    //    change cov according to cps that ar strictly left of xy
    //if it is an ellipse that touches the line: no change in cov
    //horizontal lines through x are ignored: we seek coverage just before (0;-inf)

    //For beziers, we check if this particular edge is part of the crosspoint. 
    //If so, we ignore it outright to save processing (and avoid problems from numerical 
    //imprecision)
    int ret = 0;
    for (size_t e=0; e<cwh->outline.size();e++) {
        if (cp_head != link_info::no_link && !cwh->outline[e].IsStraight()) {
            //for beziers search the crosspoint if we are part of this cp
            size_t u = cp_head; 
            //XXX ToDo: We cannot FULLY ignore these edges, as they may be curvy 
            //and cross they line more than once....
            do {
                if (Rays[u].contour == &cwh->outline) {
                    //We ignore an edge if the cp is on it...
                    //...or the cp is a vertex and the endpoint of the edge is it.
                    if ((Rays[u].vertex == e) ||
                        (Rays[u].vertex == cwh->outline.next(e) && Rays[u].pos==0)) {
                        u = link_info::no_link;
                        break;
                    }
                }
                u = Rays[u].link_in_cp.next;
            } while (u!=cp_head);
            //if the cp is on 'e' we do nothing
            if (u!=cp_head) 
                continue;
        }
        double x[3], pos[3];
        int fw[3];
        Edge tmp(cwh->outline[e]);
        tmp.SwapXY();
        const int num = tmp.CrossingVertical(xy.y, x, pos, fw);
        //do nothing for -1 or 0 returns
        for (int f=0; f<num; f++)
            if (test_smaller(xy.x, x[f])) {
                if (fw[f]==+1) ret++;    //fw is inverse due to Edge::SwapXY above
                else if (fw[f]==-1) ret--;
                //do nothing for touchpoints
            }
    }
    //now consider the holes
    for (auto i = cwh->holes.begin(); i!=cwh->holes.end(); i++)
        ret += CalcCoverageHelper(xy, cp_head, &*i);
    return ret;
}

/** Calculates the coverage of a ContourWithHoles close to a point in the <0, -inf> direction assuming an untangle operation.
*
* Holes are considered just like the outline.
* @param [in] xy The coordinates of the point
* @param [in] cp_head If the point is a cp in 'Rays' the head ray for the crosspoint.
*                     Else link_info::no_link.
* @param [in] cwh The shape for which coverage is calculated.
* @returns The coverage just a bit right-up from the cp, just before a hypothetical ray with RayAngle of <0,-inf>.*/
int ContoursHelper::CalcCoverageHelper2(const XY &xy, size_t cp_head, const ContourWithHoles *cwh) const
{
    _ASSERT(cp_head == link_info::no_link || Rays[cp_head].xy == xy);
    //For each edge, we see if it crosses the (xy.x, xy.y)->(inf, xy.y) line
    //if it crosses clockwise: +1 cov
    //if it crosses cclockwise: -1 cov
    //if it is an ellipse that crosses twice and not in xy:
    //    change cov according to cps that ar strictly left of xy
    //if it is an ellipse that touches the line: no change in cov
    //horizontal lines through x are ignored: we seek coverage just before (0;-inf)

    //For beziers, we check if this particular edge is part of the crosspoint. 
    //If so, we ignore it outright to save processing (and avoid problems from numerical 
    //imprecision)
    int ret = 0;
    for (size_t e = 0; e<cwh->outline.size(); e++) {
        size_t u = cp_head;
        if (cp_head != link_info::no_link) {
            //for beziers search the crosspoint if we are part of this cp
            do {
                if (Rays[u].contour == &cwh->outline) {
                    //We ignore an edge if the cp is on it...
                    //...or the cp is a vertex and the endpoint of the edge is it.
                    if ((Rays[u].vertex == e) ||
                        (Rays[u].vertex == cwh->outline.next(e) && Rays[u].pos==0)) {
                        u = link_info::no_link;
                        break;
                    }
                }
                u = Rays[u].link_in_cp.next;
            } while (u!=cp_head);
        }
        ret += cwh->outline[e].CrossingHorizontalCPEvaluate(xy, u!=cp_head);
    }
    //now consider the holes
    for (auto i = cwh->holes.begin(); i!=cwh->holes.end(); i++)
        ret += CalcCoverageHelper2(xy, cp_head, &*i);
    return ret;
}

/** Calculates the coverage close to a cp in the <0, -inf> direction.
 *
 * Holes are considered just like the outline. We may have binary operations, not just untangle.
 * @param [in] cp_head The index of a ray at the head of the per-cp strict list.
 * @returns The coverage just a bit right-up from the cp, just before a hypothetical ray with RayAngle of <0,-inf>.*/
int ContoursHelper::CalcCoverageHelper(size_t cp_head) const
{
    int ret = CalcCoverageHelper2(Rays[cp_head].xy, cp_head, &C1->first);
    for (auto i = C1->further.begin(); i!=C1->further.end(); i++)
        ret += CalcCoverageHelper2(Rays[cp_head].xy, cp_head, &*i);
    if (C2!=NULL) {
        ret += CalcCoverageHelper2(Rays[cp_head].xy, cp_head, &C2->first);
        for (auto i = C2->further.begin(); i!=C2->further.end(); i++)
            ret += CalcCoverageHelper2(Rays[cp_head].xy, cp_head, &*i);
    }
    //And now add coverage for curves with tangent of zero
    size_t ray_no = cp_head;
    while (Rays[ray_no].angle.angle==0 && Rays[ray_no].angle.curve<0) {
        if (Rays[ray_no].incoming) ret++;
        else ret--;
        ray_no = Rays[ray_no].link_in_cp.next;
        if (ray_no == cp_head) break;
    }
    return ret;
}

/** Calculates the coverage inside a SimpleContour assuming it has no crosspoints.
 *
 * We assume C2==NULL, an untangle operation.
 * This is called at the end of an untangle, when 'sc' has no crosspoints. In this case 
 * 1. sc itself is untangled, thus there is a valid area it encloses.
 * 2. it touches nothing, thus the coverage in it is meaningful to talk aboiut.
 * 3. We can calculate its clockwisedness.
 *
 * @param [in] sc The shape for which coverage is calculated.
 * @returns The coverage inside sc considering C1 and sc itself.*/
int ContoursHelper::CalcCoverageHelper(const SimpleContour *sc) const
{
    const XY &xy = sc->at(0).GetStart();
    int ret =  CalcCoverageHelper2(xy, link_info::no_link, &C1->first);
    const XY next = sc->NextTangentPoint(0, 0);
    const XY prev = sc->PrevTangentPoint(0, 0);
    const double a_next = angle(xy, XY(xy.x+100, xy.y), next);
    const double a_prev = angle(xy, XY(xy.x+100, xy.y), prev);
    //if the clockwise order is next->0->prev, then the horizontal line is inside
    //if sc is clockwise, then we are OK. Also vice versa.
    //Otherwise we need to add sc's coverage
    if ((a_next>a_prev) != sc->GetClockWise()) {
        if (sc->GetClockWise()) ret ++;
        else ret--;
    }
    return ret;
}


/** Determine which ray of a ray group has its following crosspoint closest to the cp of the ray group.
 *
 * Used to select between elements of a ray group during walking.
 * We assume rays in [from, to) actually form a ray group.
 * For performance reasons the caller should check that ray group has more than one member.
 * (Otherwise there is no ambiguity on which ray to pick.)
 * @param [in] from The first element of the ray group.
 * @param [in] to The element following the last element of the ray group.
 * @returns The index of the ray inside the ray group on which the following cp is closest to the cp of the ray group.*/
size_t ContoursHelper::ClosestNextCP(size_t from, size_t to) const
{
    double dist = MaxVal(dist);
    size_t closest=from;
    for (size_t i = from; i!=to; i = Rays[i].link_in_cp.next) {
        RayPointer p(i);
        Advance (p, !Rays[i].incoming);
        const XY &xy = p.at_vertex ? Rays[i].contour->at(p.vertex).GetStart() : Rays[p.index].xy;
        const double d = Rays[i].xy.Distance(xy);
        if (d < dist) {
            dist = d;
            closest = i;
        }
    }
    return closest;
}

/** Go through all the rays and for each one determine which other ray to switch to.
 *
 * During walking in order to see which ray to switch to at a crosspoint, we use the concept of
 * *coverage*. A clockwise surface has coverage 1 inside and zero outside.
 * A counterclockwise surface has coverage -1 inside and zero outside.
 * Coverage values add up, thus if a point is covered by two clockwise surfaces,
 * we say that the coverage at that point is +2.
 *
 * See the examples below. We show two edges for each example, with two rays per edge. 
 * @verbatim
       (A1)
        |                       0
        ^  +1        (X1)--->-----o===>===(X2) & (Y2)
     0  |                         |
(B1)----o---->--(B2)      +1      ^  +2
    +1  |  +2                     |
       (A2)                      (Y1)
   @endverbatim
 * We show the coverage at the respected areas. During crosspoint evaluation, as we go through the rays
 * we always maintain the coverage *just before* a given ray (meaning a bit counterclockwise from it).
 * Thus, for example the coverage for ray Y1 is +2 and for B2 it is +1. (Incoming/outgoing property is
 * ignored when talking about the angle of a ray.)
 * We can thus say that as we process rays in clockwise order outgoing rays *increase* the coverage,
 * whereas incoming edges *decrease*. So if we know the coverage just before any ray it is easy to
 * calculate it for any place around a cp.
 *
 * Thus the evaluation starts by determining the coverage just before the smallest possible RayAngle value,
 * which is angle zero (tangent is horizontal to the right) and curvature -inf, which is a counterclockwise
 * curve with infinitely small radius. Depending on the operation this can be calculated as below.
 *
 * - If we make operations on sane and untangled shapes 
 *   we can be sure that if we walk along an edge, to our left is the outside of the 
 *   surface and to our right is the inside of the surface. 
 *   Thus (knowing if the shape is clockwise or counterclockwise) we can detemine the coverage outside
 *   (always zero) and inside (+1 or -1). So we merely check each edge 
 *   that is part of this cp and determine if the point in question is to its left or right, giving us
 *   the coverage from the contour of that shape. We sum up and are done. This is done inside
 *   EvaluateCrosspoints().
 *
 * - For untangle operations we are in a more difficult situation, since we have no well behaving contours
 *   and it is impossible to locally tell the coverage just by looking at the vicinity of a cp. Thus
 *   here we have to make a vertical cut of the (untangled) shape and count how many edges cross the 
 *   vertical line drawn at the cp above the cp (with smaller y coordinate) and count how many left-to-right
 *   and right-to-left crossings we have and thereby determining the coverage. This is more expensive.
 *   This is done in CalcCoverageHelper().
 *
 * After we have determined the coverage at RayAngle (0, -inf), we use IsCoverageToInclude() to can see 
 * if this area shall be included in the result or not. We then walk arount the cp in clockwise fashion
 * updating the coverage at each edge, until we see a change in the inclusion. This then enables us to pair rays along the contour of the
 * result shape. This will be stored in the 'switch_to' member of the Ray. 
 *
 * We also set the 'seq_num' member of the ray to set which rays are equivalent from a stopping perspective.
 * 
 * This function also fills in StartRays with incoming rays that are at the contour of the result.
 */

     //Copy the processed crosspoints from the set to the bycont, where they are sorted by <contour,edge,pos>.
    //(each contour has its own bycont).
    //Fill in startrays with incoming rays marked as can_start_here
void ContoursHelper::EvaluateCrosspoints(Contour::EOperationType type) const
{
    if (Rays.size()==0) return;
    StartRays.clear();
    StartRays.reserve(Rays.size()/4);
    size_t seq_num = 0;

    //Cylce through the crosspoints
    size_t cp_head = link_cps_head;
    do {
        //for union coverage is sufficient if any one contour covers an area.
        //for intersects all of them have to cover it.
        //for xor, an odd number needs to cover it
        //
        //Example:      1\    ->2,,     .=area covered by contour 1
        //              ..\  /,,,,,     ,=area covered by contour 2
        //              ...\|,,,,,,     :=area covered by contour 3 and 2
        //              ....o----->3
        //              .../|\:::::     For unions one (clockwise) range will be 2(outgoing)->2(incoming)
        //              ../ |,\::::       and another is from 1(out)->1(in). The two ranges get different seq_nums.
        //             1<- 2|,,\3::       (So if we started on 2(in) (immeditately switching to 2(out)), we will not
        //                                 stop when coming through 1(in), but will continue on 1(out) until we get
        //                                 back here again on 2(in)). Note that (incoming(3) will have switch_action==ERROR
        //                                indicating that we should never come this way.
        //                              For intersects only 3(out)->3(in) range will be had.
        // An x(out)->y(in) range selected will result in y(in) be designated a cp switching to edge x.
        // Finally, some terminology. A <ray group> is a set of rays (incoming/outgoing mixed) with the same angle.
        // (On the example above each ray is a separate ray group of its own.)
        // Supposedly these rays have another common crosspoint besides the current one (since they lie on one another).


        //For well formed contours any intersect is at the edges of a contour,
        //thus the sections which are covered by no contour at this crosspoint
        //(like 1(in)->2(out) below) have total coverage of zero.
        //For untangles, we can be in the interior of a contour, thus sections
        //not covered by any contour in this cp may have nonzero coverage.
        //Thus we need to calculate what is the coverage around us.

        int coverage_before_r;
        if (C2==NULL)  { //we do untangle
            //For untangle we go through all the edges and see, how many times are this
            //particular cp being circled around to find what is the coverage here
            //If we could calculate the coverage for the crosspoints, use that
            coverage_before_r = CalcCoverageHelper(cp_head);
        } else {
            //here we have
            //1. well-formed contours
            //2. CPs that are between two edges of _different_ contours
            //thus all cps lie at the edge (outside have coverage 0)
            //We calcualte how many contours include the (0,inf) angle inside them.
            coverage_before_r = 0;
            size_t ray_no = cp_head;
            do {
                if (Rays[ray_no].incoming) {//for incoming edges the previous in link_cont
                    size_t outgoing = Rays[ray_no].link_in_contour.next;  //the outgoing pair of the incoming link
                    //if incoming ray is smaller angle than outgoing, then the
                    //dir before <0, -inf> is included in a clockwise surface
                    //For overall clockwise Contours in this case coverage is +1
                    //For overall ccl Contours coverage is -1 exactly in the other case
                    if (Rays[ray_no].angle.Smaller(Rays[outgoing].angle) ==
                        Rays[ray_no].contour->GetClockWise())
                        coverage_before_r += Rays[ray_no].contour->GetClockWise() ? +1 : -1;
                }
                ray_no = Rays[ray_no].link_in_cp.next;
            } while (ray_no != cp_head);
        }


        size_t r = cp_head;
        size_t tmp = r;
        //coverage_before_r actually shows coverage before the first ray group
        //find first ray group after we shall not include. (coverage is not according to the requirement)
        if (!GoToCoverage(tmp, r, coverage_before_r, type, false, tmp))
            goto next_cp; //never happens -> this is a crosspoint not needed, all switch_action will remain ERROR
        { //open a scope to allow constructions to be jumped over by the goto above
        const size_t original_started_at_ray = r;
        const unsigned orig_start_size = StartRays.size();
        const unsigned orig_seq_num = seq_num;
        unsigned marked_rays = 0;

        while(1) {
            //find first ray group (between "start_from" and "start_to" after which coverage is above reauirement
            size_t start_from = r, start_to;
            if (!GoToCoverage(start_from, start_to, coverage_before_r, type, true, original_started_at_ray))
                break; //OK, we are done - either no coverage fulfills the req, or we processed all rays (hitting "started_at_ray")
            size_t end_from = start_to, end_to;
            if (!GoToCoverage(end_from, end_to, coverage_before_r, type, false, original_started_at_ray)) {
                _ASSERT(0); //we must find one
            }
            r = end_to; //next cycle will use this

            //Now we know that coverage is like required between ray groups <start_from, start_to> and <end_from, end_to>
            //Now we set the "switch_to" members of both ray groups to the "from" of the other (could be any)
            //and leave the "switch" to of ray groups in between on "error"
            //(An optimization can be that if a ray in <start_from, start_to> continues as one ray in <end_from, end_to>
            // then we just set that ray to IGNORE. Now such situations will result in an added vertex, later removed by
            // Edge::CheckAndCombine().)
            for (size_t i = start_from; i!=start_to; i = Rays[i].link_in_cp.next)
                Rays[i].switch_to = end_from;
            for (size_t i = end_from; i!=end_to; i = Rays[i].link_in_cp.next)
                Rays[i].switch_to = start_from;
            //Now set the seq_num for all rays in the covered range
            //start_from may equal end_to. We destroy "start_from" here
            do {
                Rays[start_from].seq_num = seq_num;
                marked_rays++;
                start_from = Rays[start_from].link_in_cp.next;
            } while (start_from!=end_to);
            seq_num++;

            //Finally add a ray to start_rays
            StartRays.push_back(end_from);

            //if we made a full scan stop now
            if (original_started_at_ray == r)
                break;
        } //while through regions of sufficient coverage

        //Test for a special case:
        //A touching crosspoint, where a bezier touches a line or two beziers touch, without
        //actually crossing each other. In this case we strive to keep the contour together
        //in order to avoid spiky vertices.
        //This is important only if these edges are all at the border of included and not
        //included coverage (so that they are included in the walk). If this happens inside
        //a contour and would not be part of the final result we do not care.
        //We start by testing a few prerequisites. If we have created two sequence nums and
        //marked four edges, only then do we start investigating.
        if (marked_rays==4 && seq_num == orig_seq_num+2) {
            //OK, we may have a thing here. See if 
            //a) we have 4 rays in the cp
            //b) all are along the same line (same angle +-2)
            unsigned no_rays = 1;
            size_t rays[4] = {cp_head};
            const double angle = Rays[cp_head].angle.angle;
            const double angle2 = angle<2 ? angle+2 : angle-2;
            for (size_t x = Rays[cp_head].link_in_cp.next; x!=cp_head; x = Rays[x].link_in_cp.next)
                if (!test_equal(Rays[x].angle.angle, angle) && !test_equal(Rays[x].angle.angle, angle2))
                    goto next_cp;  //angles do not align: this is not the special case
                else if (no_rays==4)
                    goto next_cp; //too many rays: this is not the special case
                else
                    rays[no_rays++] = x;

            //OK, now we shall see if two of the rays go in one and two of the rays go in the other dir
            const size_t same = test_equal(Rays[rays[0]].angle.angle, Rays[rays[1]].angle.angle) ? 0 : 3;
            if (!test_equal(Rays[rays[same]].angle.angle, Rays[rays[(same+1)%4]].angle.angle))
                goto next_cp;
            if (!test_equal(Rays[rays[(same+2)%4]].angle.angle, Rays[rays[(same+3)%4]].angle.angle))
                goto next_cp;
            if (!test_equal(angle2, Rays[rays[(same+2)%4]].angle.angle))
                goto next_cp;
            //Now we know this is the special case. 
            //Index 'same' and 'same+1' (modulo 4) of 'rays' point to the same direction and 
            //'same+2' and 'same+3' also point to the same dir and these two dirs are opposite
            //if the two rays going the same dir are not switching to each other, the existing
            //arrangement will keep the contours together and we need not do anything.
            if (Rays[rays[same]].seq_num != Rays[rays[(same+1)%4]].seq_num)
                goto next_cp;
            _ASSERT(Rays[rays[same]].switch_to == rays[(same+1)%4]);
            _ASSERT(Rays[rays[(same+1)%4]].switch_to == rays[same]);
            _ASSERT(Rays[rays[(same+2)%4]].switch_to == rays[(same+3)%4]);
            _ASSERT(Rays[rays[(same+3)%4]].switch_to == rays[(same+2)%4]);
            _ASSERT(Rays[rays[same]].seq_num == Rays[rays[(same+1)%4]].seq_num);
            _ASSERT(Rays[rays[(same+2)%4]].seq_num == Rays[rays[(same+3)%4]].seq_num);
            //Ok, we need to rearrange
            //instead of switching from same<->same+1 and same+2<->same+3, we shall 
            //switch from same<->same+3 and same+1<->same+2.
            Rays[rays[same]].switch_to = rays[(same+3)%4];
            Rays[rays[(same+3)%4]].switch_to = rays[same];
            Rays[rays[(same+1)%4]].switch_to = rays[(same+2)%4];
            Rays[rays[(same+2)%4]].switch_to = rays[(same+1)%4];
            std::swap(Rays[rays[same]].seq_num, Rays[rays[(same+2)%4]].seq_num);
            StartRays.resize(orig_start_size);
            StartRays.push_back(rays[same]);
            StartRays.push_back(rays[(same+1)%4]);
        }

        }
next_cp:
        cp_head = Rays[cp_head].link_cps.next;
    } while (cp_head != link_cps_head); /* while cycle through the crosspoints */
}

/** Do one step of the walking.
 *
 * This function does not actually switch from one contour to another.
 * For vertices it simply goes to along the next edge up to the first
 * cp or the ending vertex. For the first cp, it stops at the incoming
 * ray of that cp. 
 * For cps it leaves the cp along the ray pointed by `p` and finds the next
 * cp on that edge (or the ending vertex of the edge if no cp).
 * Actual switching is done in Walk() from which this is called.
 *
 * @param p The current position of the walk. Can be a vertex or a crosspoint.
 * @param [in] forward True if we walk the same direction as edges go or opposite. 
*/
void ContoursHelper::Advance(RayPointer &p, bool forward) const
{
    if (p.at_vertex) {  //we are at a vertex
        if (forward) {
            //Check if upcoming edge has a crosspoint
            //Note: either we have a cp mid-edge (pos!=0) or at the startpoint of the edge.
            //In the latter case the cp is recorded with pos==0 for the next edge
            if ((Rays[p.index].vertex == p.vertex) ||
                (Rays[p.index].pos == 0.0 && Rays[p.index].vertex == Rays[p.index].contour->next(p.vertex))) {
                _ASSERT(Rays[p.index].incoming);
                p.at_vertex = false;
                //p.index remains, p.vertex is ignored
            } else { //no cp on upcoming edge, go to next vertex
                p.vertex = Rays[p.index].contour->next(p.vertex);  //we are at the contour pointed by the ray in p
            }
        } else {
            //move to previous vertex
            p.vertex = Rays[p.index].contour->prev(p.vertex);
            if (Rays[p.index].vertex == p.vertex) { //Ok, upcoming (previous) vertex has a cp
                _ASSERT(!Rays[p.index].incoming);
                p.at_vertex = false;
            }
            //else we use the updated p.vertex
        }
    } else { //we are at a crosspoint
        _ASSERT(Rays[p.index].incoming != forward); //when forward we must be at an outgoing (and vice versa)
        p.vertex = Rays[p.index].vertex;
        double pos = Rays[p.index].pos;
        if (forward)
            p.index = Rays[p.index].link_in_contour.next;
        else
            p.index = Rays[p.index].link_in_contour.prev;
        if (p.vertex != Rays[p.index].vertex || (pos < Rays[p.index].pos)!=forward) {
            //If next cp is on a different edge or on the same edge but wrong direction
            //we switch to vertex (In the latter case next cp will happen once we have
            //cricled the whole contour and got back to this edge)
            if (forward) {
                p.vertex = Rays[p.index].contour->next(p.vertex);
                //However, if the next cp is exactlty on the start of the next edge
                //(p.vertex(the next edge) == Rays[p.index].vertex (the edge of the next cp)
                // AND (Rays[p.index].pos==0 (the next cp is at the start of the edge)
                // then we remain in cp mode
                p.at_vertex = p.vertex != Rays[p.index].vertex || Rays[p.index].pos>0;
            } else 
                p.at_vertex = true;
        }
        //else we remained on the same edge, use updated p.index
    }
}

///<Marks all rays in the ray group of "ray" as in "valid"
void ContoursHelper::MarkAllInRayGroupOf(size_t ray, bool valid) const
{
    //walk forward
    size_t u = ray;
    do {
        Rays[u].valid = valid;
        u = Rays[u].link_in_cp.next;
    } while (Rays[ray].angle.IsSimilar(Rays[u].angle));
    //walk backward
    u = ray;
    do {
        Rays[u].valid = valid;
        u = Rays[u].link_in_cp.prev;
    } while (Rays[ray].angle.IsSimilar(Rays[u].angle));
}

/** Revoke invalidation from a certain points in the walk stack at a backtracking operation*/
void ContoursHelper::RevalidateAllAfter(std::vector<size_t> &ray_array, size_t from) const
{
    if (from >= ray_array.size()) return;
    for (size_t u = from; u<ray_array.size(); u++)
        MarkAllInRayGroupOf(ray_array[u], true);
    ray_array.resize(from);
}

/** Appends an edge and makes checks. Used during walking.
*
* Appends an edge to the (yet incomplete, not closed) shape.
* Checks that we do not append a degenerate (start==end) edge.
* Checks if the edge to append is a direct continuation of the last edge, in which case
* we merge the two.
* 
* endpoints[i] contain the positions of the crosspoint represented by the startpoint of edge [i].
* first is the position of the endpoint of edge i-1, second is the position of startpoint of edge i on 
* the original edge of them.
*/
void AppendDuringWalk(std::vector<Edge> &edges, std::vector<std::pair<double, double>>&endpoints, 
                      const Edge &edge, double endpos, double startpos)
{
    //XXX FIXME looping edges.
    _ASSERT(endpoints.size() == edges.size());
    //if the endpos == 0 this means the crosspoint is at a vertex
    //use 1 instead
    if (endpos==0) endpos = 1;

    //if the last point equals to the startpoint of edge, skip the last edge added
    if (edges.size()>0 && edge.GetStart().test_equal(edges.back().GetStart())) {
        endpos = endpoints.back().first; //use the endpoint calculated for that edge
        edges.pop_back();
        endpoints.pop_back();
    }

    edges.push_back(edge);
    endpoints.emplace_back(endpos, startpos);
};




/** Perform a walk operation on evaluated crosspoints.
 *
 * Walk around the contours starting from startpoint and follow the
 * switch_action for each ray. We invalidate rays visited.
 * @param [in] start The (incoming) ray to start from.
 * @returns The resulting SimpleContour.
 */
SimpleContour ContoursHelper::Walk(RayPointer start) const
{
    //Some info on the 'endpoints' local vairable.
    //In endpoints[i].first we store the end position of the (i-1)th edge.
    //In endpoints[i].second, we store the start position of the ith edge.
    //(In short: endpoints[i] contains values for the ith vertex.)
    //Each position is respective to the original egde. So for the bottom view
    //we have two (clockwise) rectangles. Assuming we do a union and focus on 
    //           +-------+    the edge marked with (1) and (2) and their crosspoint.
    //           |       |    This crosspoint lies on position 0.75 on edge #1 and
    //          (2)      |    at position 0.25 on edge #2 (approximately using 
    //           |       |    ascii art). If we start the walk from the asterisk
    //  *-(1)----o---+   |    (a vertex) endpoints[0].first will be 1 (complete
    //  |        |   |   |    end of edge marked by (3)) and endpoints[0].second
    // (3)       +---o---+    will be 0 (complete start of the edge maked by (1)).
    //  |            |        Then endpoints[1].first will be 0.75 (we do not include
    //  +------------+        the entire length of edge (1), but only up to position 
    //                        0.75. endpoints[1].second will be 0.25, since edge (2)
    // will be included not from its start, but only from the crosspoint (0.25).
    // Then endpoints[2].first will be 1 to indicate that edge #2 is included to its
    // original endpoint.
    // Note that during walk, we collect the edges of the new contour in "edges"
    // and set their starting point to the desired output (thus edge #2 is inserted
    // with its startpoint set to the crosspoint at position 0.25 of the _original_
    // edge #2. The endpoints of the edges are not adjusted in the walking step,
    // so edge #1 will be inserted into "edge" in its original form.
    //
    // Then, after the walk, we go through the collected edges and adjust their endpoints.
    // Note that by this step the positions we noted in "endpoints" will not be accurate
    // for the edges in "edges" since we have already adjusted the startpoint and these
    // position values were computed for the _original_ edges. So we need to adjust the
    // ending position value to cater for the (potential) shortening of the edge when we 
    // have set its starting point. Consider the below situation when we do an intersection.
    //         +---+     In this case the edge (1) has two crosspoints (at positions 0.5
    //         |   |     and 0.75 roughly) and we only indlude the part between the two 
    // +-(1)---*---o--+  cps. This means that if this is the asterisk endpoints[0].second
    // |       +---+  |  will be 0.5 and endpoints[1].first will be 0.75.
    // +--------------+  When we append the above mentioned segment of edge #1 to the 
    // "edges" vector, we already set its starting point to the asterisk, so it is (roughly)
    // half as long as the original edge #1. Now if we were to set its endpoint blindly to
    // position 0.75, we would create an edge longer than the desired fragment, which is 
    // now at the half-point of edges[0]. Thus the end positions need to be adjusted
    // by the start positions.
    // That is, every endpoints[i].first (the desired endpoint of edges[i-1], corresponding
    // in value to the original of edges[i]) has to be adjusted by endpoints[i-1].second
    // (the desired (and already set) starting position of edges[i-1] understood as a 
    // position on the original of edges[i-1].
    //
    // You could ask why do we bother with these position values? We already know the coordinates
    // of the crosspoints, by dont we just set the start/end of the edges to those. The answer is
    // that for bezier curves it is an expensive operation to find the parameter value (position)
    // of a point (given by XY coordinates) on the curve and chop the rest away. In contrast 
    // if you know the parameter of the point you want to chop away it is fast. Since we have
    // already got all position values for all crosspoints anyway in FindCrosspoints(), we might
    // as well use them to speed up.

    //XXX FIXME looping edges.
    std::vector<size_t> ray_array; //should allocate these as thread local
    std::vector<walk_data> wdata;  //should allocate these as thread local
    std::vector<std::pair<double, double>> endpoints;
    std::vector<Edge> edges;
    ray_array.reserve(200);
    wdata.reserve(200);
    endpoints.reserve(200);
    edges.reserve(200);

    //"current" will point to an incoming ray at a crosspoint
    _ASSERT(start.index<Rays.size() && !start.at_vertex && Rays[start.index].valid);
    RayPointer current = start;

    //do a walk from the current crosspoint, until we get back here
    const size_t sn_finish = Rays[current.index].seq_num; //we shall finish at this sequence number.
    bool forward = Rays[current.index].incoming;
    do {
        //here "current" points to an incoming ray
        if (current.at_vertex) { //we are at a vertex
            if (forward) 
                AppendDuringWalk(edges, endpoints, Rays[current.index].contour->at(current.vertex), 1, 0); //just append the whole edge
            else {
                //Append the inverse of the edge
                Edge edge(Rays[current.index].contour->at_prev(current.vertex));
                edge.Invert();
                AppendDuringWalk(edges, endpoints, edge, 1, 0);
            }
            //no need to modify "current" here. A vertex is treated as both an incoming and outgoing ray
        } else { //we are at a crosspoint, not a vertex
            const Ray &ray = Rays[current.index];  //ray on which we arrive
            _ASSERT(forward == ray.incoming);
            //The position to use as ending position for the previous ray
            double terminating_pos = ray.incoming ? ray.pos : 1-ray.pos;
            //Mark the incoming ray (and all incoming rays its entire ray group) as invalid (=done).
            MarkAllInRayGroupOf(current.index, false);
            ray_array.push_back(current.index); //store our decision to the backtracking stack.
            size_t switch_to = ray.switch_to;
            if (switch_to == link_info::no_link || !Rays[switch_to].valid) { //hm nowhere to swicth or already been there
                //backtrack to last position
                size_t last_chosen;
                do {
                    if (wdata.size()==0) {
                        //cannot backtrace: give up
                        //if (edges.size()==0)
                            return SimpleContour();
                        edges.emplace_back(ray.xy, edges[0].GetStart());
                        endpoints.emplace_back(terminating_pos, 0);
                        for (size_t i = 0; i<edges.size()-1; /*nope*/) {
                            const size_t next = i+1;
                            if (edges[i].GetStart() == edges[next].GetStart() && edges[i].IsStraight()) {
                                //the next edge starts at the same location as the current one 
                                //and the current edge is straight (no chance of a looping edge)
                                //we shall delete the current edge.
                                endpoints[next].first = endpoints[i].first; //save the end position of the previous edge
                                edges.erase(edges.begin()+i);
                                endpoints.erase(endpoints.begin()+i);
                                continue;
                            }
                            //Set edge endpoint. The position is updated as described at the beginning of the function
                            edges[i].SetEnd(edges[next].GetStart(), 1-(1-endpoints[next].first)/(1-endpoints[i].second));
                            i++;
                        }
                        goto endend;
                    }
                    _ASSERT(ray_array.size()>=wdata.back().rays_size);
                    _ASSERT(edges.size()>=wdata.back().result_size);
                    RevalidateAllAfter(ray_array, wdata.back().rays_size); //Make rays on discarded part valid again
                    edges.resize(wdata.back().result_size);  //Discard parts.
                    endpoints.resize(wdata.back().result_size);
                    last_chosen = wdata.back().chosen_outgoing;
                    terminating_pos = wdata.back().terminating_pos;
                    wdata.pop_back(); //pop the backtracking stack
                    switch_to = Rays[last_chosen].link_in_cp.next; //next to pick
                    //if next to pick is another ray group, we need to backtrace one more
                } while (!Rays[switch_to].angle.IsSimilar(Rays[last_chosen].angle));
                //OK here, wdata, ray_array are restored to correct size
                //switch_to shows next alternative to try
                //we even removed this backtrace from wdata, if there are more
                //rays to choose from, we will re-add them below
            }
            //'ray' cannot be used from here, as we may have switched to a different previous ray above

            //Now switch to outgoing ray
            current.index = switch_to;
            MarkAllInRayGroupOf(current.index, false);
            const Ray &next_ray = Rays[current.index];
            //check if this was the only choice (exclude case when we did a backtrace)
            if (next_ray.angle.IsSimilar(Rays[next_ray.link_in_cp.next].angle))
                wdata.emplace_back(ray_array.size(), edges.size(), switch_to, terminating_pos);
            forward = !next_ray.incoming;  //fw may change if we need to walk on an incoming ray
            //Append a point
            if (forward) {
                Edge edge(next_ray.contour->at(next_ray.vertex));
                edge.SetStart(next_ray.xy, next_ray.pos);
                AppendDuringWalk(edges, endpoints, edge, terminating_pos, next_ray.pos);
            } else {
                Edge edge(next_ray.contour->at(next_ray.vertex));
                edge.Invert();
                edge.SetStart(next_ray.xy, 1-next_ray.pos);
                AppendDuringWalk(edges, endpoints, edge, terminating_pos, 1-next_ray.pos);
            }
        }
        //Now find the next cp and corresponding incoming ray
        Advance(current, forward);
    } while (current.at_vertex || Rays[current.index].seq_num != sn_finish);

    //Now set end and startpoint of edges and cleanup
    //if the crosspoint we started at was also a vertex, it may be that it is repeated at the end
    if (edges.size()>1 && edges[0].GetStart().test_equal(edges.back().GetStart())) {
        endpoints[0].first = endpoints.back().first;
        edges.pop_back();
        endpoints.pop_back();
    }

    if (edges.size()==0) return SimpleContour();
    if (edges.size()==1 && edges.front().IsStraight()) return SimpleContour();
    if (edges.size()==2 && edges.front().IsStraight() && edges.back().IsStraight()) return SimpleContour();
    //set "end" values. Not known previously
    for (size_t i = 0; i<edges.size(); i++) {
        const size_t next = (i+1)%edges.size();
        //Set edge endpoint. The position is updated as described at the beginning of the function
        edges[i].SetEnd(edges[next].GetStart(), 1-(1-endpoints[next].first)/(1-endpoints[i].second));
    }
endend:
    //Remove empty edges - which we get if a crosspoint falls exactly on pos==0 of an edge
    //and we do not use the edge.
    //We do not maintain endpoints any more
    //Also try to combine edges to save on them.
    //Since the last edge was deleted we try to combine with our previous one
    for (size_t i = 0; i<edges.size(); /*nope*/) {
        const size_t prev = (i+edges.size()-1)%edges.size();
        if (edges[i].IsDot() || edges[prev].CheckAndCombine(edges[i]))
            edges.erase(edges.begin()+i);
        else
            i++;
    }

    //Sanity checks
    if (edges.size()==0)
        return SimpleContour();
    if (edges.size()==2 && edges[0].straight && edges[1].straight)
        return SimpleContour();

    SimpleContour ret;
    ret.edges.swap(edges);
    ret.clockwise_fresh = ret.area_fresh = ret.boundingBox_fresh = false;
    _ASSERT(ret.IsSane());
    return ret;
}

/** Finds the combined coverage of elements in "list" at the area covered by "c" 
 * We assume c does not overlap with any element. */
int ContoursHelper::CoverageInNodeList(const std::list<node> &list, const SimpleContour &c) const
{
    for (auto i=list.begin(); i!=list.end(); i++) 
        switch(i->contour.CheckContainment(c)) {
        case REL_OVERLAP: 
        case REL_A_IS_EMPTY: 
        case REL_B_IS_EMPTY: 
        case REL_BOTH_EMPTY: 
        case REL_SAME: 
        case REL_A_IN_HOLE_OF_B: 
        case REL_B_IN_HOLE_OF_A: 
        case REL_IN_HOLE_APART:
            _ASSERT(0); //should not happen
            //fallthrough
        case REL_A_INSIDE_B: 
        case REL_APART:     //c is outside "i->contour" check further elements in list
            continue;
        case REL_B_INSIDE_A: //"c" is inside "i->contour", dwelve deeper
            return CoverageInNodeList(i->children, c) + (i->contour.GetClockWise() ? +1 : -1);
        }
    return 0;
}


/** Inserts a node into the containment tree.
 *
 * We first find where to insert, then we insert.
 * @param list The tree to insert to.
 * @param n The node to insert.
 * @returns the resulting inserted node.*/
node * ContoursHelper::InsertContour(std::list<node> *list, node &&n) const
{
    if (n.contour.size()==0) return 0;
    for (auto i = list->begin(); i!=list->end(); /*nope*/) {
        const EContourRelationType res = i->contour.CheckContainment(n.contour);
        switch (res) {
        default:
        case REL_A_IS_EMPTY:
        case REL_BOTH_EMPTY:  //nodes already inserted cannot be empty
        case REL_OVERLAP:     //at this point we cannot have overlap, that was eliminated by walking or we did not find crosspoints
            _ASSERT(0);
        case REL_A_INSIDE_B:
            //move "i" from the "list" to "n.children"
            n.children.splice(n.children.end(), *list, i++);
            //continue checking
            break;
        case REL_B_IS_EMPTY:
        case REL_SAME:
            return &*i; //easy, do nothig
        case REL_B_INSIDE_A:
            //insert into the children of "i" instead;
            return InsertContour(&i->children, std::move(n));
        case REL_APART:
            i++;
            break; //continue checking the remainder of the list
        }
    }
    //at this point we are APART with all elements in "list"
    list->push_back(std::move(n));
    return &list->back();
}


/** Insert a shape that had no crosspoints into the post-processing tree.
 *
 * If the SimpleContour in "c->outline" has no crosspoints and 
 * it should be included due to the operation, this is called and we insert it into the tree.
 * This is done by finding where to insert (which shape already in the contour is it inside).
 * Plus we go on and process its holes, too 
 * @param list The tree to insert to
 * @param c The shape to insert 
 * @param [in] const_c True if `c` should be copied, false if we can move. 
 * @param [in] C_other One of C1 or C2 - the other than the one `c` is from.
 * @param [in] type The operation we perform.
 */
void ContoursHelper::InsertIfNotInRays(std::list<node> *list, const ContourWithHoles *c, bool const_c,
                                       const Contour *C_other, Contour::EOperationType type) const
{
    if (!Rays.size() || !IsContourInRays(&c->outline)) {
        int coverage_in_c;
        if (C2==NULL)
            coverage_in_c = CalcCoverageHelper(&c->outline);
        else {
            //In order to avoid checking ray crossing for all contours, we calculate
            //coverage within "c" using the assumptions on C1 and C2 being nice.
            //Now, it may be that C_other is already empty, if we have already moved all its
            //content to "list". In that case we have to traverse the "list"
            const bool clockwise_us = C1==C_other ? clockwise_C2 : clockwise_C1;
            const bool clockwise_other = C1==C_other ? clockwise_C1 : clockwise_C2;
            if (C_other->IsEmpty()) {
                const int cov_from_other = CoverageInNodeList(*list, c->outline);
                //if our whole surface is clockwise, then coverage inside "c" us can be +1 or 0
                //if our whole surface is ccl then 0 or -1
                const int cov_from_us = clockwise_us ? (c->GetClockWise() ? 1 : 0) : (c->GetClockWise() ? 0 : -1);
                coverage_in_c = cov_from_us + cov_from_other;
            } else {
                const EPointRelationType is_within_other = C_other->IsWithin(c->outline[0].GetStart());
                _ASSERT(is_within_other != WI_ON_EDGE && is_within_other != WI_ON_VERTEX);
                //if we are outside the other, coverage from that is zero
                //if we are inside, then coverage is either +1 or -1 depending on dir
                const int cov_from_other = inside(is_within_other) ? (clockwise_other ? 1 : -1) : 0;
                //if our whole surface is clockwise, then coverage inside "c" us can be +1 or 0
                //if our whole surface is ccl then 0 or -1
                const int cov_from_us = clockwise_us ? (c->GetClockWise() ? 1 : 0) : (c->GetClockWise() ? 0 : -1);
                coverage_in_c = cov_from_us + cov_from_other;
            }
        }
        const int coverage_outside_c = coverage_in_c + (c->GetClockWise() ? -1 : +1);
        if (IsCoverageToInclude(coverage_in_c, type) !=
            IsCoverageToInclude(coverage_outside_c, type)) {
            //place into the tree
            //If the contour is not const, we can destroy it.
            //The below move does only destroy the outline of c, not the holes
            node *n = InsertContour(list, const_c ? node(c->outline) : node(std::move(const_cast<SimpleContour&&>(c->outline))));
            _ASSERT(n);
            //use its children list for any holes it may have
            list = &n->children;
        }
    } //else we do not insert the contour "c" and use the original list for holes
    for (const auto &h : c->holes)
        InsertIfNotInRays(list, &h, const_c, C_other, type);
}

/** Converts a post-processing tree to a class Contour.
 *
 * We simply walk the top level entries of the tree (a forest really) and convert them to 
 * ContourWithHoles objects and we add them to 'result'
 * @param [in] type The type of operation we do.
 * @param list The tree we process. This will be emptied.
 * @param [out] result The result.
 * @param [in] positive True if we want to have a clockwise Contour at the end.
 */
void ContoursHelper::ConvertNode(Contour::EOperationType type, std::list<node> &&list, Contour &result, bool positive) const
{
    for (auto &n : list) {
        if (n.contour.GetClockWise() != positive) 
            n.contour.Invert();
        ContourWithHoles c(std::move(n.contour));
        ConvertNode(type, std::move(n.children), c.holes, !positive);
        result.append(std::move(c));
    }
}

/** Converts a post-processing tree to a class ContourList.
 *
 * We simply walk the top level entries of the tree (a forest really) and convert them to 
 * ContourWithHoles objects and we add them to 'result'
 * @param [in] type The type of operation we do.
 * @param list The tree we process. This will be emptied.
 * @param [out] result The result.
 * @param [in] positive True if we want to have a clockwise Contour at the end.
 */
void ContoursHelper::ConvertNode(Contour::EOperationType type, std::list<node> &&list, ContourList &result, bool positive) const
{
    for (auto &n : list) {
        if (n.contour.GetClockWise() != positive) 
            n.contour.Invert();
        ContourWithHoles c(std::move(n.contour));
        ConvertNode(type, std::move(n.children), c.holes, !positive);
        result.append(std::move(c));
    }
}

/** Returns what to do if performing an operation with an empty contour.
 *
 * E.g., if A+B is done and B is empty. In this case either we have to return 
 * A or an empty contour. (E.g., for unions we need to return A, for intersections
 * we need to return an empty contour.)
 * @param type The type of operation we do.
 * @param clockwise The clockwiseness of the non-empty shape.
 * @returns True if the non-empty contour shall be returned, false if an empty one. */
inline bool ContoursHelper::OperationWithAnEmpty(Contour::EOperationType type, bool clockwise) const
{
    //if the sole non-empty contoutr is to be included, we return this
    return IsCoverageToInclude(clockwise ? 1 : 0, type);
}


/** The main function for carrying out the operation.
 *
 * @param [in] type The type of operation we do.
 * @param [out] result The resulting Contour.
 */
void ContoursHelper::Do(Contour::EOperationType type, Contour &result) const
{
    //"result" may be the identical to "C1" or "C2"
    if (C2==NULL) {
        //We do some form of untangle
        _ASSERT(type != Contour::POSITIVE_INTERSECT);
        _ASSERT(type != Contour::POSITIVE_UNION);
        _ASSERT(type != Contour::POSITIVE_XOR);
        _ASSERT(type != Contour::NEGATIVE_INTERSECT);
        _ASSERT(type != Contour::NEGATIVE_UNION);
        _ASSERT(type != Contour::NEGATIVE_XOR);
        if (C1->IsEmpty()) {
            result.clear();
            return;
        }
        //fast path: if single surface, no holes and no crosspoints
        if (C1->further.size()==0 && C1->first.holes.size()==0 && Rays.size()==0) {
            if (const_C1) result = *C1;
            else result = std::move(*C1);
            result.first.outline.CalculateClockwise();
            //If the result is not to be included in this operation, delete
            if (!IsCoverageToInclude(result.GetClockWise() ? 1 : -1, type))
                result.clear();
            return;
        }
    } else {
        //We do a binary operation, do trivial cases
        if (C1->IsEmpty()) {
            if (C2->IsEmpty() || !OperationWithAnEmpty(type, C2->GetClockWise())) {
                result.clear();
                return;
            }
            if (const_C2) result = *C2;
            else result = std::move(*C2);
            return;
        }
        if (C2->IsEmpty()) {
            if (OperationWithAnEmpty(type, C1->GetClockWise())) {
                if (const_C1) result = *C1;
                else result = std::move(*C1);
            } else
                result.clear();
            return;
        }
        _ASSERT(C1->IsSane() && C2->IsSane());
    }
    std::list<node> list; //this will be the root(s) of the post-processing tree (forest).
    if (Rays.size()) {
#ifdef _DEBUG
        if (1) {
            expand_debug_cps.clear();
            size_t u = link_cps_head;
            do {
                expand_debug_cps[u] = Rays[u].xy;
                u = Rays[u].link_cps.next;
            } while (u!=link_cps_head);
        }
#endif
        //evaluate crosspoints
        EvaluateCrosspoints(type); // Process each cp and determine if it is relevant to us or not
        //Walk while we have eligible starting points
        while (StartRays.size()) {
            InsertContour(&list, node(Walk(RayPointer(StartRays.back()))));
            //clear invalid startrays from end
            while (StartRays.size() && !Rays[StartRays.back()].valid)
                StartRays.pop_back();
        }
        ResetCrosspoints(); //A subsequent call to "Do" can work OK, again
        //result of the walk(s) are in "list"
    }
    //Now we need to consider those contours from the original set
    //for which we found no crosspoints (may be all of them, none of them or in between)
    //We shall always demand a positive surface, except if both input are hole
    InsertIfNotInRays(&list, &C1->first, const_C1, C2, type);
    for (auto i = C1->further.begin(); i!=C1->further.end(); i++)
        InsertIfNotInRays(&list, &*i, const_C1, C2, type);
    if (C2) {
        InsertIfNotInRays(&list, &C2->first, const_C2, C1, type);
        for (auto i = C2->further.begin(); i!=C2->further.end(); i++)
            InsertIfNotInRays(&list, &*i, const_C2, C1, type);

    }
    result.clear(); //Remove anything the caller left in "result"
    //Convert the tree of nodes to a Contour object. Make it positive unless
    //we do a binary operation on holes
    ConvertNode(type, std::move(list), result, Contour::is_positive(type));
    _ASSERT(result.IsSane());
}

double ContourList::Distance(const XY &o, XY &ret) const
{
    double d = CONTOUR_INFINITY;
    for (auto i = begin(); i!=end(); i++) {
        XY tmp;
        double dd = i->Distance(o, tmp);
        if (fabs(dd)<fabs(d)) {
            d = -dd;
            ret = tmp;
            if (d==0) return 0;
        }
    }
    return d;
}

double ContourList::DistanceWithTangents(const XY &o, XY &ret, XY &t1, XY &t2) const
{
    double d = CONTOUR_INFINITY;
    for (auto i = begin(); i!=end(); i++) {
        XY tmp, _1, _2;
        double dd = i->DistanceWithTangents(o, tmp, _1, _2);
        if (fabs(dd)<fabs(d)) {
            d = -dd;
            ret = tmp;
            t1 = _1;
            t2 = _2;
            if (d==0) return 0;
        }
    }
    return d;
}


/////////////////////ContourWithHoles

bool ContourWithHoles::IsSane(bool shouldbehole) const
{
    if (!outline.IsSane()) return false;
    if (outline.GetClockWise()==shouldbehole) return false;
    for (auto i=holes.begin(); i!=holes.end(); i++)
        if (!i->IsSane(!shouldbehole))
            return false;
    return true;
}

void ContourWithHoles::Expand(EExpandType type4positive, EExpandType type4negative, double gap, Contour &res,
                              double miter_limit_positive, double miter_limit_negative) const
{
    if (outline.size()==0) return;
    if (gap==0) {res = *this; return;}
    outline.Expand(outline.GetClockWise() && gap>0 ? type4positive : type4negative, gap, res,
                   outline.GetClockWise() && gap>0 ? miter_limit_positive : miter_limit_negative);
    if (holes.size()==0 || res.IsEmpty()) return;
    for (const auto &h : holes) {
        Contour tmp;
        h.Expand(type4positive, type4negative, gap, tmp, miter_limit_positive, miter_limit_negative);
        //in case "i" is an actual holes, it is are already inversed, adding is the right op
        res.Operation(GetClockWise() ? Contour::POSITIVE_UNION : Contour::NEGATIVE_UNION, res, std::move(tmp));
    }
}

void ContourWithHoles::Expand2D(const XY &gap, Contour &res) const
{
    if (outline.size()==0) return;
    if (test_zero(gap.x) && test_zero(gap.y)) {res = *this; return;}
    outline.Expand2D(gap, res);
    if (holes.size()==0 || res.IsEmpty()) return;
    for (const auto & h: holes) {
        Contour tmp;
        h.Expand2D(gap, tmp);
        //in case "i" is an actual holes, it is are already inversed, adding is the right op
        res.Operation(GetClockWise() ? Contour::POSITIVE_UNION : Contour::NEGATIVE_UNION, res, std::move(tmp));
    }
}

EContourRelationType ContourWithHoles::RelationTo(const ContourWithHoles &c, bool ignore_holes) const
{
    const EContourRelationType res = outline.RelationTo(c.outline);
    if (ignore_holes) return res;
    switch (res) {
    default:
    case REL_A_IN_HOLE_OF_B:
    case REL_B_IN_HOLE_OF_A:
    case REL_IN_HOLE_APART:
        _ASSERT(0);
    case REL_OVERLAP:
    case REL_A_IS_EMPTY:
    case REL_B_IS_EMPTY:
    case REL_BOTH_EMPTY:
    case REL_APART:
        return res;
    case REL_SAME:
        if (holes.RelationTo(c.holes, ignore_holes) == REL_SAME) return REL_SAME;
        return REL_OVERLAP;
    case REL_A_INSIDE_B:
        switch (c.holes.RelationTo(outline, ignore_holes)) {
        case REL_A_IS_EMPTY:
            return REL_A_INSIDE_B;
        case REL_B_INSIDE_A:
        case REL_SAME:
            return REL_A_IN_HOLE_OF_B;
        case REL_A_INSIDE_B:
        case REL_OVERLAP:
            return REL_OVERLAP;
        case REL_APART:
        case REL_B_IN_HOLE_OF_A:
            return REL_A_INSIDE_B;
        case REL_A_IN_HOLE_OF_B:
        case REL_B_IS_EMPTY:
        case REL_BOTH_EMPTY:
        case REL_IN_HOLE_APART:
        default:
            _ASSERT(0);
            return REL_OVERLAP;
        }
    case REL_B_INSIDE_A:
        switch (holes.RelationTo(c.outline, ignore_holes)) {
        case REL_A_IS_EMPTY:
            return REL_B_INSIDE_A;
        case REL_B_INSIDE_A:
        case REL_SAME:
            return REL_B_IN_HOLE_OF_A;
        case REL_A_INSIDE_B:
        case REL_OVERLAP:
            return REL_OVERLAP;
        case REL_APART:
        case REL_B_IN_HOLE_OF_A:
            return REL_B_INSIDE_A;
        case REL_A_IN_HOLE_OF_B:
        case REL_B_IS_EMPTY:
        case REL_BOTH_EMPTY:
        case REL_IN_HOLE_APART:
        default:
            _ASSERT(0);
            return REL_OVERLAP;
        }
    }
}

void ContourWithHoles::Distance(const ContourWithHoles &c, DistanceType &dist_so_far) const
{
    if (dist_so_far.IsZero()) return;
    DistanceType d = dist_so_far;
    d.ClearInOut();
    outline.Distance(c.outline, d);
    if (d.WasIn()) { //one outline is inside another one, consider holes
        //see which one is in the other
        DistanceType temp = d;
        temp.ClearInOut();
        if (GetBoundingBox().GetArea() < c.GetBoundingBox().GetArea()) {
            //we are inside, see if we are in the holes of 'c'
            if (!c.holes.IsEmpty()) {
                temp.SwapPoints();
                c.holes.Distance(*this, temp);
                temp.SwapPoints();
            }
        } else {
            if (!holes.IsEmpty())
                holes.Distance(c, temp);
        }
        temp.SwapInOut();
        d.Merge(temp);
    }
    dist_so_far.Merge(d);
}

/////////////////////////////////////////  Contour implementation

void Contour::assign(const std::vector<XY> &v, bool winding)
{
    clear();
    if (v.size()<2) return;
    Contour tmp;
    tmp.assign_dont_check(v);
    Operation(winding ? Contour::WINDING_RULE_NONZERO : Contour::WINDING_RULE_EVENODD, std::move(tmp));
}

void Contour::assign(const XY v[], size_t size, bool winding)
{
    clear();
    if (size < 2) return;
    Contour tmp;
    tmp.assign_dont_check(v, size);
    Operation(winding ? Contour::WINDING_RULE_NONZERO : Contour::WINDING_RULE_EVENODD, std::move(tmp));
}

void Contour::assign(const std::vector<Edge> &v, bool winding)
{
    clear();
    Contour tmp;
    tmp.assign_dont_check(v);
    Operation(winding ? Contour::WINDING_RULE_NONZERO : Contour::WINDING_RULE_EVENODD, std::move(tmp));
}

void Contour::assign(std::vector<Edge> &&v, bool winding)
{
    clear();
    Contour tmp;
    tmp.assign_dont_check(std::move(v));
    Operation(winding ? Contour::WINDING_RULE_NONZERO : Contour::WINDING_RULE_EVENODD, std::move(tmp));
}

void Contour::assign(const Edge v[], size_t size, bool winding)
{
    clear();
    Contour tmp;
    tmp.assign_dont_check(v, size);
    Operation(winding ? Contour::WINDING_RULE_NONZERO : Contour::WINDING_RULE_EVENODD, std::move(tmp));
}

void Contour::assign(const Path &p, bool close_everything, bool force_clockwise, bool winding)
{
    clear();
    Contour tmp;
    for (auto &e: p.ConvertToClosed(close_everything))
        tmp.append_dont_check(e);
    Operation(winding ? Contour::WINDING_RULE_NONZERO : Contour::WINDING_RULE_EVENODD, std::move(tmp));
    if (force_clockwise && !GetClockWise())
        invert_dont_check();
}

void Contour::assign(Path &&p, bool close_everything, bool force_clockwise, bool winding)
{
    clear();
    Contour tmp;
    //First see if p is a single closed shape
    for (size_t u = 0; u<p.edges.size(); u++)
        if (p.at(u).GetEnd() != p.at((u+1)%p.edges.size()).GetStart()) {
            //no, we need to work this through
            for (auto &e: p.ConvertToClosed(close_everything))
                tmp.append_dont_check(e);
            goto untangle;
        }
    //OK, a single round thingy
    tmp.first.outline.assign_dont_check(std::move(p.edges));
untangle:
    Operation(winding ? Contour::WINDING_RULE_NONZERO : Contour::WINDING_RULE_EVENODD, std::move(tmp));
    if (force_clockwise && !GetClockWise())
        invert_dont_check();
}

void Contour::assign_dont_check(Path &&p, bool close_everything)
{
    //First see if p is a single closed shape
    for (size_t u = 0; u<p.edges.size(); u++)
        if (p.at(u).GetEnd() != p.at((u+1)%p.edges.size()).GetStart())
            return assign_dont_check(p, close_everything); //the non-move version
    //OK, fully closed - we can assign the edges directly
    assign_dont_check(std::move(p.edges));
}

bool Contour::IsSane() const
{
    if (!first.IsSane(!first.GetClockWise())) return false;
    for (auto i = further.begin(); i!=further.end(); i++)
        if (!i->IsSane(!i->GetClockWise())) return false;
    return true;
}

void Contour::Invert()
{
    Contour tmp(*this);
    tmp.first.Invert();
    if (tmp.further.size())
        tmp.further.Invert();
    Operation(tmp.GetClockWise() ? EXPAND_POSITIVE : EXPAND_NEGATIVE, std::move(tmp));
}

void Contour::Operation(EOperationType type, const Contour &c1)
{
    ContoursHelper h(c1);
    h.Do(type, *this);

}

void Contour::Operation(EOperationType type, Contour &&c1)
{
    ContoursHelper h(std::move(c1));
    h.Do(type, *this);
}

void Contour::Operation(EOperationType type, const Contour &c1, const Contour &c2)
{
    ContoursHelper h(c1, c2);
    h.Do(type, *this);
}


void Contour::Operation(EOperationType type, const Contour &c1, Contour &&c2)
{
    ContoursHelper h(c1, std::move(c2));
    h.Do(type, *this);
}

void Contour::Operation(EOperationType type, Contour &&c1, Contour &&c2)
{
    ContoursHelper h(std::move(c1), std::move(c2));
    h.Do(type, *this);
}

void Contour::Expand(EExpandType type4positive, EExpandType type4negative, double gap, Contour &res,
                     double miter_limit_positive, double miter_limit_negative) const
{
    first.Expand(type4positive, type4negative, gap, res, miter_limit_positive, miter_limit_negative);
    if (further.size()==0) return;
    Contour tmp;
    for (auto i = further.begin(); i!=further.end(); i++) {
        i->Expand(type4positive, type4negative, gap, tmp, miter_limit_positive, miter_limit_negative);
        res += std::move(tmp);
        tmp.clear();
    }
    _ASSERT(IsSane());
}

void Contour::Expand2D(const XY &gap, Contour &res) const
{
    first.Expand2D(gap, res);
    if (further.size()==0) return;
    Contour tmp;
    for (auto i = further.begin(); i!=further.end(); i++) {
        i->Expand2D(gap, tmp);
        res += std::move(tmp);
        tmp.clear();
    }
    _ASSERT(IsSane());
}


//if "ignore holes" is true, it can only return
//A_IS_EMPTY, B_IS_EMPTY, BOTH_EMPTY, A_INSIDE_B, B_INSIDE_A, SAME, APART, OVERLAP
//if false, these additional values may come:
//A_IN_HOLE_OF_B, B_IN_HOLE_OF_A, IN_HOLE_APART
//(latter meaning no overlap, but some parts of A is in holes and outside of B)
EContourRelationType Contour::RelationTo(const Contour &c, bool ignore_holes) const
{
    if (IsEmpty())
        return c.IsEmpty() ? REL_BOTH_EMPTY : REL_A_IS_EMPTY;
    else if (c.IsEmpty())
        return REL_B_IS_EMPTY;
    if (!boundingBox.Overlaps(c.GetBoundingBox())) return REL_APART;
    if (further.IsEmpty() && c.further.IsEmpty()) return first.RelationTo(c.first, ignore_holes);
    //Here we admittedly hack.
    //To avoid writing code again, we append the "first" to "further" and compare
    //two ContourLists re-using the code there.
    //Then we restore "further" by deleting the appended cwh and restoring the boundingbox
    Contour &c1 = const_cast<Contour&>(*this);
    Contour &c2 = const_cast<Contour&>(c);
    EContourRelationType res;
    if (further.IsEmpty()) {
        const Block bb(c2.further.boundingBox);
        c2.further.append(c2.first);
        res = first.RelationTo(c2.further, ignore_holes);
        c2.further.pop_back();
        c2.further.boundingBox = bb;
    } else if (c2.further.IsEmpty()) {
        const Block bb(c1.further.boundingBox);
        c1.further.append(c1.first);
        res = c1.further.RelationTo(c.first, ignore_holes);
        c1.further.pop_back();
        c1.further.boundingBox = bb;
    } else {
        const Block bb(c1.further.boundingBox);
        const Block b2(c2.further.boundingBox);
        c1.further.push_back(c1.first);
        c2.further.push_back(c2.first);
        res = c1.further.RelationTo(c2.further, ignore_holes);
        c1.further.pop_back();
        c2.further.pop_back();
        c1.further.boundingBox = bb;
        c2.further.boundingBox = b2;
    }
    return res;
}

inline bool MergeTangentFroms(bool was, XY clockwise[2], XY cclockwise[2], bool was2, const XY c[2], const XY cc[2])
{
    if (was && was2) {
        clockwise[1]  = minmax_clockwise(clockwise[0], c[1], clockwise[1], true);
        clockwise[0]  = minmax_clockwise(clockwise[1], c[0], clockwise[0], false);
        cclockwise[0] = minmax_clockwise(cclockwise[1], cc[0], cclockwise[0], true);
        cclockwise[1] = minmax_clockwise(cclockwise[0], cc[1], cclockwise[1], false);
        //clockwise[1]  = minmax_clockwise(clockwise[0], c[1], clockwise[1], true);
        //clockwise[0]  = minmax_clockwise(clockwise[1], c[0], clockwise[0], false);
    } else if (was2) {
        clockwise[1]  = c[1];
        clockwise[0]  = c[0];
        cclockwise[0] = cc[0];
        cclockwise[1] = cc[1];
    }
    return was || was2;
}


bool Contour::TangentFrom(const Contour &from, XY clockwise[2], XY cclockwise[2]) const
{
    if (IsEmpty() || from.IsEmpty()) return false;
    bool was = first.outline.TangentFrom(from.first.outline, clockwise, cclockwise);
    XY c[2], cc[2];
    for (auto i = from.further.begin(); i!=from.further.end(); i++) {
        const bool was2 = first.outline.TangentFrom(i->outline, c, cc);
        was = MergeTangentFroms(was, clockwise, cclockwise, was2, c, cc);
    }
    for (auto j = further.begin(); j!=further.end(); j++) {
        const bool was2 = j->outline.TangentFrom(from.first.outline, c, cc);
        was = MergeTangentFroms(was, clockwise, cclockwise, was2, c, cc);
        for (auto i = from.further.begin(); i!=from.further.end(); i++) {
            const bool was3 = j->outline.TangentFrom(i->outline, c, cc);
            was = MergeTangentFroms(was, clockwise, cclockwise, was3, c, cc);
        }
    }
    return was;
}

Range Contour::Cut(const XY &A, const XY &B) const 
{
    Range ret = boundingBox.Cut(A, B); //also tests for A==B or invalid bb, which catches empty "this"
    if (ret.IsInvalid()) return ret;
    const Edge s(A+(B-A)*ret.from, A+(B-A)*(ret.till+0.1));
    ret = Cut(s);
    if (ret.IsInvalid()) return ret;
    Range ret2;
    if (fabs(A.x-B.x) > fabs(A.y-B.y)) {
        const double p1 = s.GetStart().x + (s.GetEnd().x-s.GetStart().x)*ret.from;
        const double p2 = s.GetStart().x + (s.GetEnd().x-s.GetStart().x)*ret.till;
        //p1 and p2 are now the x coordinate of the two CP, convert to "pos" on A-B
        ret2.from = (p1-A.x)/(B.x-A.x);
        ret2.till = (p2-A.x)/(B.x-A.x);
    } else { //do it in y coordinates
        const double p1 = s.GetStart().y + (s.GetEnd().y-s.GetStart().y)*ret.from;
        const double p2 = s.GetStart().y + (s.GetEnd().y-s.GetStart().y)*ret.till;
        //p1 and p2 are now the y coordinate of the two CP, convert to "pos" on A-B
        ret2.from = (p1-A.y)/(B.y-A.y);
        ret2.till = (p2-A.y)/(B.y-A.y);
    }
    return ret2;
}


Range Contour::CutWithTangent(const XY &A, const XY &B, std::pair<XY, XY> &from, std::pair<XY, XY> &till) const
{
    Range ret = boundingBox.Cut(A, B); //also tests for A==B or invalid bb, which catches empty "this"
    if (ret.IsInvalid()) return ret;
    const Edge s(A+(B-A)*ret.from, A+(B-A)*(ret.till+0.1));
    ret = CutWithTangent(s, from, till);
    if (ret.IsInvalid()) return ret;
    Range ret2;
    if (fabs(A.x-B.x) > fabs(A.y-B.y)) {
        const double p1 = s.GetStart().x + (s.GetEnd().x-s.GetStart().x)*ret.from;
        const double p2 = s.GetStart().x + (s.GetEnd().x-s.GetStart().x)*ret.till;
        //p1 and p2 are now the x coordinate of the two CP, convert to "pos" on A-B
        ret2.from = (p1-A.x)/(B.x-A.x);
        ret2.till = (p2-A.x)/(B.x-A.x);
    } else { //do it in y coordinates
        const double p1 = s.GetStart().y + (s.GetEnd().y-s.GetStart().y)*ret.from;
        const double p2 = s.GetStart().y + (s.GetEnd().y-s.GetStart().y)*ret.till;
        //p1 and p2 are now the y coordinate of the two CP, convert to "pos" on A-B
        ret2.from = (p1-A.y)/(B.y-A.y);
        ret2.till = (p2-A.y)/(B.y-A.y);
    }
    return ret2;
}

#ifdef _DEBUG
int expand_debug = false;
std::map<size_t, XY> expand_debug_cps;
std::list<std::vector<Edge>> expand_debug_contour;
#endif


} //namespace
