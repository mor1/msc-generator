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
#include "contour_area.h"

//"cp" means crosspoint from here

/* The data structure is a bit exotic here.
** We have "Ray"s that are half lines going outwards from or inwards to crosspoints.
** In fact they can be curvy, for curvy edges. 
** When two edges cross (and form a cp), there are 4 rays: two incoming and two outgoing 
** (as edges are directed).
**
** Rays are stored in a big vector and are arranged in 4 doubly linked lists.
** For each contour part of the operation we have a circular list that indexes rays according 
** to their order along the contour, by increasing <edge_no, pos_on_the_edge, incoming>. 
** (incoming rays are earlier than their outgoing pair).
** There is a separate such circular list for each contour, the head of such lists are the rays with 
** the smallest triplet for each contour.
** In addition the heads of the circular lists are themselves organized into another list (circular), 
** which links only the heads. This second list contains as many elements as there are contours.
** I call the first list "strict" since it contains all rays (in one of the circular lists), and the 
** latter loose, as it only contains head rays.
**
** In addition, there is a third list which lists rays associated with a crosspoint in clockwise order.
** In fact they are ordered by <false angle, curve angle> and the head is the ray closest to <0, -inf>.
** E.g., for a simple corsspoint of two edges crossing, we will have 4 rays in the circular list, two
** for each edge. This is also a strict list, since all rays belong to one such list.
** Finally, a fourth list links the heads of the per crosspoint lists. This is a loose list.
**
** Each "Ray" structure contains "prev" and "next" members for the four lists.
** Rays that are not part of a loose list have these members set to "no_link" for that loose list.
** This enables us to move both along the endges of a contour and also rotate around a crosspoint easily.
*/

struct link_info
{
    unsigned next;
    unsigned prev;
    enum {no_link=3200000};
};

struct Ray
{
    unsigned contour;   //number of the contour the vertex belongs to
    unsigned vertex;    //the number of the vertex staring the edge the crosspoint is on
    double   pos;       //-1 if it is a vertex and not a crosspoint, [0..1) the pos of the crosspoint on the edge
    bool     incoming;  //true if it is incoming
    RayAngle angle;     //the angle of the 
    XY       xy;        //the position of the crosspoint of the ray
    //linked list indices
    enum list_type {LIST_CONTOUR, LIST_CP, IN_CONTOUR, IN_CP};
    //loose lists: only heads are doubly linked, additional rays on the same contour have their next to themselves and prev to the head
    link_info link_contours;
    link_info link_cps;
    //strict lists: a separate circular list for each contour and cp. If next==prev, we are alone in the circular list
    link_info link_in_contour;
    link_info link_in_cp;
    link_info &get_index(list_type type) {return type==LIST_CONTOUR?link_contours:type==LIST_CP?link_cps:type==IN_CONTOUR?link_in_contour:link_in_cp;}
    //values used during any walk (both untangle and combine)
    mutable bool valid;         //wether this ray have already been included in a contour resulting from a walk
    mutable unsigned seq_num;   //same seq num as where we started a walk indicates a stopping criteria
    mutable unsigned switch_to; //index of another ray for this cp
    mutable enum switch_action_t {IGNORE, ERROR, SWITCH} switch_action;
    Ray(const XY &point, unsigned c, unsigned v, double p, bool i, const RayAngle &a) :
        contour(c), vertex(v), pos(p), incoming(i), angle(a), xy(point),
        valid(true), switch_action(ERROR) {}
    void Reset() {valid = true; switch_action = ERROR;}
};

typedef std::vector<Ray> RayCollection;
struct RayPointer {
    bool at_vertex;  //true if we are at a vertex
    unsigned index;  //index of a ray. If we are at a vertex this points to a nearby ray on this contour
    unsigned vertex; //the #of vertex if we are at a vertex. Else ignore
    explicit RayPointer(unsigned i) : at_vertex(false), index(i) {}
};

class ContourHelper {
protected:
    const bool with_self;                        //true if we search for crosspoint within contours, too
    std::vector<const Contour*> ContourPointers; //The contours we process
    RayCollection               Rays;            //All the rays of an operation for all cps
    unsigned                    link_contours_head;  //head of the contour loose list
    unsigned                    link_cps_head;       //head of the cp loose list
    std::vector<unsigned>       StartRays;       //Rays where walks can start

private:
    int RayCompareBy_Contour(unsigned r1, unsigned r2) const 
        {return Rays[r1].contour == Rays[r2].contour ? 0 : Rays[r1].contour < Rays[r2].contour ? -1 : +1; }
    //If pos is equal, we put incoming as smaller than outgoing
    bool RayCompareBy_VertexPosIncoming (unsigned r1, unsigned r2) const 
        {return Rays[r1].vertex == Rays[r2].vertex ? Rays[r1].pos == Rays[r2].pos ? Rays[r1].incoming == Rays[r2].incoming ? false : Rays[r1].incoming : Rays[r1].pos<Rays[r2].pos : Rays[r1].vertex<Rays[r2].vertex;}
    int RayCompareBy_CP(unsigned r1, unsigned r2) const 
        {return Rays[r1].xy.test_equal(Rays[r2].xy) ? 0 : Rays[r1].xy < Rays[r2].xy ? -1 : +1; }
    //if angle is equal, we use contour, edge, pos and incoming as tie breaker
    bool RayCompareBy_Angle(unsigned r1, unsigned r2) const 
        {return Rays[r1].angle.IsSimilar(Rays[r2].angle) ? Rays[r1].contour == Rays[r2].contour ? RayCompareBy_VertexPosIncoming(r1, r2) : Rays[r1].contour < Rays[r2].contour : Rays[r1].angle.Smaller(Rays[r2].angle);}

protected:
    int RayCompareLoose(unsigned r1, unsigned r2, Ray::list_type type) const {return type==Ray::LIST_CONTOUR ? RayCompareBy_Contour(r1,r2) : RayCompareBy_CP(r1,r2);}
    bool RayCompareStrict(unsigned r1, unsigned r2, Ray::list_type type) const {return type==Ray::IN_CONTOUR ? RayCompareBy_VertexPosIncoming(r1,r2) : RayCompareBy_Angle(r1,r2);}
    //helpers for adding crosspoints and linking them
    unsigned InsertToLooseList(Ray::list_type type, unsigned index);
    bool InsertToStrictList(Ray::list_type type, unsigned index, unsigned head);
    bool AddCrosspointHelper(const XY &point, unsigned c, unsigned v, double p, bool i, const RayAngle &a);
    void AddCrosspoint(const XY &xy, unsigned c1, unsigned v1, double p1, unsigned c2, unsigned v2, double p2);
    //helpers for rating crosspoints
    bool IsCoverageToInclude(int cov, Contour::operation_t type) const;
    unsigned FindRayGroupEnd(unsigned from, int &coverage, unsigned abort_at1, unsigned abort_at2) const;
    bool GoToCoverage(unsigned &from, unsigned &to, int &cov_now, Contour::operation_t type, bool start, unsigned abort_at) const;
    //helpers for walking
    void Advance(RayPointer &p, bool forward) const;
    void InvalidateAllInRayGroupOf(unsigned ray);

    //main functions
    unsigned FindCrosspoints(); //fills in AllRays, RaysByCP and RaysByContour
    void ResetCrosspoints();    //make them valid and set switch_action to ERROR
    void EvaluateCrosspoints(Contour::operation_t type); //Fills in switch actions and StartRays
    void Walk(RayPointer start, Contour &result);

public:
    //external interface
    ContourHelper(const Contour &c) : ContourPointers(1, &c), with_self(true) {}
    ContourHelper(const Contour &c1, const Contour &c2, bool s=false) : ContourPointers(2), with_self(s) {ContourPointers[0]= &c1, ContourPointers[1] = &c2;}
    ContourHelper(const std::vector<Contour> &c, bool s=false) : ContourPointers(c.size()), with_self(c.size()<=1 || s) {for (unsigned u=0;u<c.size();u++) ContourPointers[u]=&c[u];}
    ContourHelper(const std::vector<const Contour*> &c, bool s=false) : ContourPointers(c.size()), with_self(c.size()<=1 || s) {for (unsigned u=0;u<c.size();u++) ContourPointers[u]=c[u];}
    ContourHelper(unsigned size, const Contour c[], bool s=false) : ContourPointers(size), with_self(size<=1 || s) {for (unsigned u=0;u<size;u++) ContourPointers[u]=c+u;}
    ContourHelper(unsigned size, const Contour *const c[], bool s=false) : ContourPointers(size), with_self(size<=1 || s) {for (unsigned u=0;u<size;u++) ContourPointers[u]=c[u];}
    bool DoIt(Contour::operation_t type, ContourList &result);
};

//returns the head of the strict list 
unsigned ContourHelper::InsertToLooseList(Ray::list_type type, unsigned index) 
{
    _ASSERT(type==Ray::LIST_CONTOUR || type==Ray::LIST_CP); //assume a loose list
    if (index==0) {//we are first to be inserted
        Rays[0].get_index(type).next = 0;
        Rays[0].get_index(type).prev = 0;
        return 0;
    }
    unsigned &list_head = type==Ray::LIST_CONTOUR ? link_contours_head : link_cps_head;
    unsigned now = list_head;
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
            unsigned prev = Rays[now].get_index(type).prev;
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
bool ContourHelper::InsertToStrictList(Ray::list_type type, unsigned index, unsigned head)
{
    _ASSERT(type==Ray::IN_CONTOUR || type==Ray::IN_CP); //assume a strict list
    if (head==index) {
        //initialze new strict list
        Rays[index].get_index(type).prev = index;
        Rays[index].get_index(type).next = index;
        return true;
    }
    unsigned now=head;
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
        unsigned prev = Rays[now].get_index(type).prev;
        Rays[now].get_index(type).prev = index;
        Rays[prev].get_index(type).next = index;
        Rays[index].get_index(type).prev = prev;
        Rays[index].get_index(type).next = now;
        //check if we insert to the beginning of the strict list
        if (head == now) {
            //if so, move list head to "index"
            unsigned &loose_list_head = type==Ray::IN_CONTOUR ? link_contours_head : link_cps_head;
            const Ray::list_type type_loose = type==Ray::IN_CONTOUR ? Ray::LIST_CONTOUR : Ray::LIST_CP;
            const unsigned loose_prev = Rays[head].get_index(type_loose).prev;
            const unsigned loose_next = Rays[head].get_index(type_loose).next;
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

//return true if we could add (no duplication)
bool ContourHelper::AddCrosspointHelper(const XY &point, unsigned c, unsigned v, double p, bool i, const RayAngle &a)
{
    Rays.push_back(Ray(point, c, v, p, i, a));
    //link in to the loose lists
    const unsigned index = Rays.size()-1;
    const unsigned contour_head = InsertToLooseList(Ray::LIST_CONTOUR, index);
    const unsigned cp_head      = InsertToLooseList(Ray::LIST_CP     , index);
    //Insert to strict list of the contours. Stop if already there
    if (!InsertToStrictList(Ray::IN_CONTOUR, index, contour_head)) {
        //OK, this <contour, vertex, pos, incoming> already exists
        //in this case loose lists were not modified, we can safely delete the back
        Rays.pop_back();
        return false;
    }
    InsertToStrictList(Ray::IN_CP, index, cp_head);
    return true;
}

void ContourHelper::AddCrosspoint(const XY &xy, unsigned c1, unsigned v1, double p1, unsigned c2, unsigned v2, double p2)
{
    //In allrays even indexed positions are incoming edges, odd positions are outgoing ones
    if (p1==0)  //avoid pos==0 and incoming
        AddCrosspointHelper(xy, c1, v1, p1, true,  ContourPointers[c1]->at_prev(v1).Angle(true,  xy, 1));
    else
        AddCrosspointHelper(xy, c1, v1, p1, true,  ContourPointers[c1]->at(v1).Angle(true,  xy, p1));
    if (p1==1) //avoid pos==1 and outgoing
        AddCrosspointHelper(xy, c1, v1, p1, false, ContourPointers[c1]->at_next(v1).Angle(false, xy, 0));
    else
        AddCrosspointHelper(xy, c1, v1, p1, false, ContourPointers[c1]->at(v1).Angle(false, xy, p1));
    if (p2==0)  //avoid pos==0 and incoming
        AddCrosspointHelper(xy, c2, v2, p2, true,  ContourPointers[c2]->at_prev(v2).Angle(true,  xy, 1));
    else
        AddCrosspointHelper(xy, c2, v2, p2, true,  ContourPointers[c2]->at(v2).Angle(true,  xy, p2));
    if (p2==2) //avoid pos==1 and outgoing
        AddCrosspointHelper(xy, c2, v2, p2, false, ContourPointers[c2]->at_next(v2).Angle(false, xy, 0));
    else
        AddCrosspointHelper(xy, c2, v2, p2, false, ContourPointers[c2]->at(v2).Angle(false, xy, p2));
}

unsigned ContourHelper::FindCrosspoints()
{
    //Reset stuff
    Rays.clear();
    Rays.reserve(200);
    link_cps_head = link_contours_head = 0;

    XY r[4];
    double one_pos[4], two_pos[4];
    unsigned ret=0;
    for (unsigned c_out=0; c_out<ContourPointers.size(); c_out++) {
        const Contour &c0 = *ContourPointers[c_out];
        if (with_self) 
            for (unsigned i = 1; i<c0.size(); i++)
                for (unsigned j = 0; j<i; j++) {
                    const unsigned n = c0[i].Crossing(c0[j], r, one_pos, two_pos);
                    for (unsigned k=0; k<n;k++)
                        AddCrosspoint(r[k], 0, i, one_pos[k], 0, j, two_pos[k]);
                    ret += n;
                }
        for (unsigned c_in=c_out+1; c_in<ContourPointers.size(); c_in++) {
            const Contour &c1 = *ContourPointers[c_in];
            //here "c1" is guatanteed to be different from "c0"
            //Search for cps only if the two overlaps
            if (c0.GetBoundingBox().Overlaps(c1.GetBoundingBox())) 
                for (unsigned i = 0; i<c0.size(); i++)
                    for (unsigned j = 0; j<c1.size(); j++) {
                        const unsigned n = c0[i].Crossing(c1[j], r, one_pos, two_pos);
                        for (unsigned k=0; k<n;k++)
                            AddCrosspoint(r[k], 0, i, one_pos[k], 1, j, two_pos[k]);
                        ret += n;
                    }
        }
    }
    return ret;
}

//After this you can call again EvaluateCrosspoints with a different operation
void ContourHelper::ResetCrosspoints() 
{
    StartRays.clear();
    for (auto i = Rays.begin(); i!=Rays.end(); i++) 
        i->Reset();
}

inline bool ContourHelper::IsCoverageToInclude(int cov, Contour::operation_t type) const
{
    //If union, pick the angle ranges covered by at least one contour
    //If intersect, pick angle ranges covered by all of the contours
    //If xor, we pick ranges of odd coverage
    //for untangle we formulate winding rules as they say
    switch (type) {
    default: _ASSERT(0); /*fallthrough*/
    case Contour::COMBINE_INTERSECT:   return cov>=ContourPointers.size();
    case Contour::COMBINE_UNION:       return cov>=1;
    case Contour::COMBINE_XOR:         return cov%2==1; //assume cov>=0; unspecified for cov<0
    case Contour::WINDING_NONZERO:     return cov!=0;
    case Contour::WINDING_EVENODD:     return cov%2==1; //here we can gave cov<0
    case Contour::WINDING_NONNEGATIVE: return cov>=0;
    }
}

//return the ray after the current ray-group
//"coverage" starts as coverage just before "from" and ends as coverage just before returned ray
inline unsigned ContourHelper::FindRayGroupEnd(unsigned from, int &coverage, unsigned abort_at1, unsigned abort_at2) const
{
    unsigned to = from;
    do {
        if (Rays[to].incoming) coverage--;
        else coverage++;
        to = Rays[to].link_in_cp.next;
    } while (to != from && to!=abort_at1 && to!=abort_at2 && Rays[from].angle.IsSimilar(Rays[to].angle));
    return to;
}

//If "start" is true, go to the ray group *after* which coverage becomes eligible for inclusion
//  (that is, for intersects >=no of contours, for union >=1 and for xor odd; see above in IsCoverageToInclude),
//else go to the ray group *after* which coverage becomes ineligible
//coverage just before ray "from" is "cov_now" at the time of call
//returns the first ray of the ray group in "from", the ray after the ray group in "to" and the coverage just before "to" in "cov_now"
//return false if we wrap (or hit abort_at). Coverage may become eligible or ineligible at the startpoint (as required by start)
bool ContourHelper::GoToCoverage(unsigned &from, unsigned &to, int &cov_now, Contour::operation_t type, bool start, unsigned abort_at) const
{
    const unsigned started = to = from;
    do {
        from = to;
        to = FindRayGroupEnd(from, cov_now, started, abort_at);
    } while (to!=started && to!=abort_at && start != IsCoverageToInclude(cov_now, type));
    return start == IsCoverageToInclude(cov_now, type);
}

//Copy the processed crosspoints from the set to the bycont, where they are sorted by <contour,edge,pos>.
//(each contour has its own bycont).
//Fill in startrays with incoming rays marked as can_start_here
void ContourHelper::EvaluateCrosspoints(Contour::operation_t type)
{
    if (Rays.size()==0) return;
    StartRays.clear();
    StartRays.reserve(Rays.size()/4);
    unsigned seq_num = 0;

    //Cylce through the crosspoints
    unsigned cp_head = link_cps_head;
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

        int coverage_before_r = 0;
        if (with_self) {
            //For each edge, we see if it crosses the (xy.x, xy.y)->(inf, xy.y) line
            //if it crosses clockwise: +1 cov
            //if it crosses cclockwise: -1 cov
            //if it is an ellipse that crosses twice and not in xy: 
            //    change cov according to cps that ar strictly left of xy
            //if it is an ellipse that touches the line: no change in cov
            //horizontal lines through x are ignored: we seek coverage just before (0;-inf)
            const XY xy = Rays[cp_head].xy;
            for (unsigned c=0;c<ContourPointers.size();c++) {
                const Contour &contour = *ContourPointers[c];
                for (unsigned e=0;e<contour.size();e++) {
                    double x[2], pos[2];
                    bool fw[2];
                    Edge tmp(contour[e]);
                    tmp.SwapXY();
                    const int num = tmp.CrossingVertical(xy.y, x, pos, fw);
                    //do nothing for -1 or 0 returns
                    for (int f=0; f<num; f++)
                        if (test_smaller(xy.x, x[f])) {
                            if (fw[f]) coverage_before_r++;
                            else coverage_before_r--;
                        }
                }
            }
            //And now add coverage for curves with tangent of zero
            unsigned ray_no = cp_head; 
            do {
                if (Rays[ray_no].incoming) coverage_before_r++;
                else coverage_before_r--;
                ray_no = Rays[ray_no].link_in_cp.next;
            } while (ray_no != cp_head && Rays[ray_no].angle.angle==0 && Rays[ray_no].angle.curve<0);
        } else {
            //here we have well-formed contours and all cps lie at the edge (outside have coverage 0)
            //We calcualte how many contours include the (0,inf) angle inside them.
            unsigned ray_no = cp_head; 
            do {
                if (Rays[ray_no].incoming) {//for incoming edges the previous in link_cont
                    unsigned outgoing = Rays[ray_no].link_in_contour.next;  //the outgoing pair of the incoming link
                    if (Rays[ray_no].angle.Smaller(Rays[outgoing].angle))
                        coverage_before_r++;
                }
                ray_no = Rays[ray_no].link_in_cp.next;
            } while (ray_no != cp_head);
        }
        
        unsigned r = cp_head;
        unsigned tmp = r;
        //coverage_before_r actually shows coverage before the first ray group
        //find first ray group after we shall not include. (coverage is not according to the requirement)
        if (!GoToCoverage(tmp, r, coverage_before_r, type, false, tmp))
            goto next_cp; //never happens -> this is a crosspoint not needed, all switch_action will remain ERROR
        const unsigned original_started_at_ray = r;
        while(1) {
            //find first ray group (between "start_from" and "start_to" after which coverage is above reauirement
            unsigned start_from = r, start_to;
            if (!GoToCoverage(start_from, start_to, coverage_before_r, type, true, original_started_at_ray))
                break; //OK, we are done - either no coverage fulfills the req, or we processed all rays (hitting "started_at_ray")
            unsigned end_from = start_to, end_to;
            if (!GoToCoverage(end_from, end_to, coverage_before_r, type, false, original_started_at_ray)) {
                _ASSERT(0); //we must find one
            }
            r = end_to; //next cycle will use this

            //Now we know that coverage is like required between ray groups <start_from, start_to> and <end_from, end_to>
            //Now we set the "switch_to" members of both ray groups to the "from" of the other (could be any)
            //and leave the "switch" to of ray groups in between on "error"
            //(An optimization can be that if a ray in <start_from, start_to> continues as one ray in <end_from, end_to>
            // then we just set that ray to IGNORE. Now such situations will result in an added vertex, later removed by
            // EdgeArc::CheckAndCombine().)
            for (unsigned i = start_from; i!=start_to; i = Rays[i].link_in_cp.next) {
                Rays[i].switch_action = Ray::SWITCH;
                Rays[i].switch_to = end_from;
            }
            for (unsigned i = end_from; i!=end_to; i = Rays[i].link_in_cp.next) {
                Rays[i].switch_action = Ray::SWITCH;
                Rays[i].switch_to = start_from;
            }
            //Now set the seq_num for all rays in the covered range
            //start_from may equal end_to. We destroy "start_from" here
            do {
                Rays[start_from].seq_num = seq_num;
                start_from = Rays[start_from].link_in_cp.next;
            } while (start_from!=end_to);
            seq_num++;

            //Finally add a ray to start_rays
            StartRays.push_back(end_from);

            //if we made a full scan stop now
            if (original_started_at_ray == r)
                break;
        } //while through regions of sufficient coverage
next_cp:
        cp_head = Rays[cp_head].link_cps.next;
    } while (cp_head != link_cps_head); /* while cycle through the crosspoints */
}


void ContourHelper::Advance(RayPointer &p, bool forward) const
{
    if (p.at_vertex) {  //we are at a vertex
        if (forward) {
            if (Rays[p.index].vertex == p.vertex) {  //Ok, upcoming edge has a crosspoint
                _ASSERT(Rays[p.index].incoming);
                p.at_vertex = false;
                //p.index remains, p.vertex is ignored
            } else { //no cp on upcoming edge, go to next vertex
                p.vertex = ContourPointers[Rays[p.index].contour]->next(p.vertex);  //we are at the contour pointed by the ray in p
            }
        } else {
            //move to previous vertex
            p.vertex = ContourPointers[Rays[p.index].contour]->prev(p.vertex);
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
            p.at_vertex = true;
            if (forward) 
                p.vertex = ContourPointers[Rays[p.index].contour]->next(p.vertex);
            // else use updated p.vertex
        }
        //else we remained on the same edge, use updated p.index
    }
}

//Marks all rays in the ray group of "ray" as invalid
void ContourHelper::InvalidateAllInRayGroupOf(unsigned ray)
{
    //walk forward 
    unsigned u = ray;
    do {
        Rays[u].valid = false;
        u = Rays[u].link_in_cp.next;
    } while (Rays[ray].angle.IsSimilar(Rays[u].angle));
    //walk backward
    u = ray;
    do {
        Rays[u].valid = false;
        u = Rays[u].link_in_cp.prev;
    } while (Rays[ray].angle.IsSimilar(Rays[u].angle));
}

//Walk around the contours starting from startpoint and follow the
//switch_action bycontd for each incoming ray. This is used for union and intersect (and substract)
void ContourHelper::Walk(RayPointer start, Contour &result)
{
    result.clear();
    //current will point to an incoming ray at a crosspoint
    _ASSERT(start.index<Rays.size() && !start.at_vertex && Rays[start.index].valid);
    RayPointer current = start;

    //do a walk from the current crosspoint, until we get back here
    const unsigned sn_finish = Rays[current.index].seq_num;
    bool forward = Rays[current.index].incoming;
    do {
        //here "current" points to an incoming ray
        if (current.at_vertex) { //we are at a vertex
            if (forward)
                result.AppendDuringWalk(ContourPointers[Rays[current.index].contour]->at(current.vertex));
            else {
                Edge edge;
                edge.CopyInverseToMe(ContourPointers[Rays[current.index].contour]->at_prev(current.vertex));
                result.AppendDuringWalk(edge);
            }
            //no need to modify "current" here. A vertex is treated as both an incoming and outgoing ray
        } else { //we are at a crosspoint, not a vertex
            const Ray &ray = Rays[current.index];  //ray on which we arrive
            _ASSERT(forward == ray.incoming);
            //Mark the incoming ray (and all incoming rays its entire ray group) as DONE
            InvalidateAllInRayGroupOf(current.index);
            switch (ray.switch_action) {
            default:
                _ASSERT(0);
            case Ray::ERROR:
                _ASSERT(0);
                result.clear();
                return;
            case Ray::IGNORE:
                //for ignore outgoing ray is really a continuation of incoming ray on the same edge
                if (forward)
                    current.index = Rays[current.index].link_in_contour.next;
                else
                    current.index = Rays[current.index].link_in_contour.prev;
                break;
            case Ray::SWITCH:
                //Now switch to outgoing ray
                current.index = Rays[current.index].switch_to;
                const Ray &next_ray = Rays[current.index];
                forward = !next_ray.incoming;  //fw may change if we need to walk on an incoming ray
                //Append a point
                if (forward) {
                    Edge edge(ContourPointers[next_ray.contour]->at(next_ray.vertex));
                    edge.SetStartStrict(next_ray.xy, next_ray.pos, true);
                    result.AppendDuringWalk(edge);
                } else {
                    Edge edge;
                    edge.CopyInverseToMe(ContourPointers[next_ray.contour]->at(next_ray.vertex));
                    edge.SetStartStrict(next_ray.xy, 1-next_ray.pos, true);
                    result.AppendDuringWalk(edge);
                }
                break;
            }
        }
        //Now find the next cp and corresponding incoming ray
        Advance(current, forward);
    } while (current.at_vertex || Rays[current.index].seq_num != sn_finish);
}

bool ContourHelper::DoIt(Contour::operation_t type, ContourList &result)
{
    //Find and evaluate crosspoints
    if (FindCrosspoints()) 
        EvaluateCrosspoints(type); // Process each cp and determine if it is relevant to us or not
    if (StartRays.size()==0)
        return false; //two contours do not touch, determine their relative position

    //Walk while we have eligible starting points
    ContourList holes;
    while (StartRays.size()) {
        ContourWithHoles walk; //static: we keep allocated memory between calls for performance
        Walk(RayPointer(*StartRays.rbegin()), walk);
        //clear invalid startrays from end
        while (StartRays.size() && !Rays[*StartRays.rbegin()].valid)
            StartRays.pop_back();
		//If we have abandoned this polyline, do not perform the processing below: start with another polyline
        if (!walk.PostWalk()) continue;
        _ASSERT(walk.IsSane());
        if (walk.CalculateClockwise())
            result.append(walk);
        else
            holes.append(walk.CreateInverse());
    }
    
    //Post-process holes   
    switch (type) {
    case Contour::COMBINE_UNION:    
        //With union we may have holes, but only one surface
        _ASSERT(result.size()<=1); 
        if (holes.size() && result.size())  //if we got only holes & no surfaces, we return empty "result"
            result.begin()->holes.swap(holes);
        break;
    case Contour::COMBINE_INTERSECT:
        //With intersect (and substract), we can have multiple surfaces, but no holes
        //...or also, we may have just holes (e.g., substracting a larger area from a smaller one) ??? We assume this does not happen
        _ASSERT(holes.size()==0 /* || results.size()==0 */); 
        break;
    case Contour::COMBINE_XOR: 
        //With XOR, we shall see
        break;
    case Contour::WINDING_NONZERO:     
    case Contour::WINDING_EVENODD:     
    case Contour::WINDING_NONNEGATIVE: 
        break;
    default:
        _ASSERT(0);
        break;
    }
    return true;
}

///////////////////////////// Contour

//Do not create degenerate triangles.
//Always create clockwise.
Contour::Contour(XY a, XY b, XY c)
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
}

Contour::Contour(double ax, double ay, double bx, double by, double cx, double cy)
{
    *this = Contour(XY(ax,ay), XY(bx,by), XY(cx,cy));
}

Contour::Contour(const XY &c, double radius_x, double radius_y, double tilt_deg, double s_deg, double d_deg)
{
    if (radius_y==0) radius_y = radius_x;
    Edge edge(c, radius_x, radius_y, tilt_deg, s_deg, d_deg);
    boundingBox = edge.GetBoundingBox();
    push_back(edge);
    if (edge.GetType()==EDGE_FULL_CIRCLE) return; //full circle
    push_back(Edge(edge.GetEnd(), edge.GetStart()));
}

Contour &Contour::operator =(const Block &b)
{
    clear();
    boundingBox = b;
    if (b.IsInvalid()) return *this;
    push_back(Edge(b.UpperLeft(), XY(b.x.till, b.y.from)));
    push_back(Edge(XY(b.x.till, b.y.from), b.LowerRight()));
    push_back(Edge(b.LowerRight(), XY(b.x.from, b.y.till)));
    push_back(Edge(XY(b.x.from, b.y.till), b.UpperLeft()));
    return *this;
}

Contour Contour::CreateInverse() const
{
    Contour result;
    result.resize(size());
    for (unsigned i=0; i<size(); i++)
        result[size()-1-i].CopyInverseToMe(at(i));
    result.boundingBox = boundingBox;
    return result;
}

//in p* return the number of vertex or edge we have fallen on if result is such
//if strict is false, any point that is even _close_ to an edge will snap to the edge
is_within_t Contour::IsWithin(XY p, int *edge, double *pos, bool strict) const
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


//////////////////////////////////Contour::Combine (Union, intersection, subsrtaction) implementation


//true if q is between (a and b) modulo 4
//if q is almost equal to a or b, we return false;
inline bool really_between04_warp (double q, double a, double b)
{
    if (a<b) return test_smaller(q,b) && !test_smaller(q,a);
    return test_smaller(a,q) || test_smaller(q,b);
}

//Can result SAME, APRT, A_INSIDE_B or B_INSIDE_A
//It can also return OVERLAP, which means one of our point is outside b, can either be APART or b may be in us
Contour::result_t Contour::CheckContainmentHelper(const Contour &b) const
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
            const double one_prev = angle(p, XY(p.x, -100),   PrevTangentPoint(i, 0));
            const double one_next = angle(p, XY(p.x, -100),   NextTangentPoint(i, 0));
            const double two_prev = angle(p, XY(p.x, -100), b.PrevTangentPoint(edge, pos));
            const double two_next = angle(p, XY(p.x, -100), b.NextTangentPoint(edge, pos));
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
Contour::result_t Contour::CheckContainment(const Contour &other) const
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

bool Contour::CalculateClockwise() const
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
        return angles < size()*180;
    }
    if (size()==2) {
        //if a contour is two edges, it should not be two straigth edges
        if (at(0).GetType() == EDGE_STRAIGHT && at(1).GetType() == EDGE_STRAIGHT) {
            _ASSERT(0);
        }
        if (at(0).GetType() != EDGE_STRAIGHT && at(1).GetType() != EDGE_STRAIGHT) {
            //two curves
            //if they are of same direction we get it
            if (at(0).GetClockWise() == at(1).GetClockWise())
                return at(0).GetClockWise();
            //two curves with opposite dir, they do not touch only at the two ends
            //the one contains the other decides
            const XY center_line = (at(0).GetStart()+at(1).GetStart())/2;
            const XY center0 = at(0).GetEllipseData().Radian2Point(at(0).GetRadianMidPoint());
            const XY center1 = at(1).GetEllipseData().Radian2Point(at(1).GetRadianMidPoint());
            const double dist0 = (center0 - center_line).length();
            const double dist1 = (center1 - center_line).length();
            return at(0).GetClockWise() == (dist0 > dist1);
        }
        //one curve, one straight: dir is decided by curve
        if (at(0).GetType() == EDGE_STRAIGHT)
            return at(1).GetClockWise();
        else
            return at(0).GetClockWise();
    }
    //if (size()==1), a full ellipsis
    return at(0).GetClockWise();
}

//Appends an edge.
//Checks that we do not append a zero long edge
//Checks if the edge to append is a direct continuation of the last edge
void Contour::AppendDuringWalk(const Edge &edge)
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
bool Contour::PostWalk()
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
    return true;
}

bool Contour::IsSane() const
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
bool Contour::Sanitize()
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



//Does a union or intersection of two poligons, byconts the result in "result"
//- For union "result" will be one bounding contour, its "holes" may have zero or more elements
//- For intersection "result" may have more elements, their "holes" are guaranteed to be empty
//Return values
//OVERLAP: the two contours actually intersect (or at least touch for union)
//surfaces contains return values only if the result is OVERLAP.
//A_IS_EMPTY, B_IS_EMPTY, BOTH_EMPTY: they mean just that.
//A_INSIDE_B: *this is fully inside b. (ignoring clockwiseness).
//B_INSIDE_A: b is fully inside *this. (ignoring clockwiseness).
//SAME: The two contours are actually the same (ignoring clockwiseness).
//APART: The two contours are apart. (ignoring clockwiseness).
Contour::result_t Contour::UnionIntersectXor(const Contour &b, ContourList &result, operation_t type) const
{
    if (size()==0) return b.size() ? A_IS_EMPTY : BOTH_EMPTY;
    if (b.size()==0) return B_IS_EMPTY;
    if (!boundingBox.Overlaps(b.boundingBox)) return APART;

    ContourHelper helper(*this, b);

    if (helper.DoIt(type, result)) return OVERLAP;
    
    //no crosspoints: result is empty
    return CheckContainment(b); //two contours do not touch, determine their relative position
}


//Checks if the contour touches itself and splits it into multiple pieces, using the winding rule
//Any point on the plane is part of the result, if a ray from it to infinity statisfies this rule:
//"If the contour crosses the ray from left-to-right, counts +1; if from right to left, counts -1.
//(Left and right are determined from the perspective of looking along the ray from the starting point.)
//If the total count is non-zero, the point will be inside the result."
//Can return
//- A_IS_EMPTY, if we are empty  (nothing is added to surfaces or holes)
//- SAME, if we are already untangled (nothing is added to surfaces or holes)
//- OVERLAP if there were intersections (or is counterclockwise) and they are placed in result
Contour::result_t Contour::Untangle(ContourList &result, operation_t rule) const
{
    Contour tmp(*this);
    const bool original = tmp.Sanitize();
    if (tmp.size()==0)
        return OVERLAP; //an empty "result" is the untangled version
    ContourHelper helper(tmp);
    if (helper.DoIt(rule, result)) return OVERLAP;

    if (original) {
        if (CalculateClockwise()) return SAME;
        //no crosspoints, but counterclockwise
        //in case of WINDING_NONNEGATIVE, we ignore holes: we return an empty result.
        if (rule != WINDING_NONNEGATIVE)
            result.append(CreateInverse());
        return OVERLAP;
    } else {
        //Here we did make changes in Sanitize() above, so we return OVERLAP in any case
        if (tmp.CalculateClockwise())
            result.append(std::move(tmp));
        else if (rule != WINDING_NONNEGATIVE)
            result.append(tmp.CreateInverse());
        /*Note in case you add code here: tmp was destroyed by "move" above!*/
        return OVERLAP;
    }
}


//////////////////////////////////Contour::Rotate implementation


const Block &Contour::CalculateBoundingBox()
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
bool Contour::AddAnEdge(const Edge &edge)
{
    XY dum1[8];
    double dum2[8];
    Contour ret(*this);
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

//////////////////////////////////Contour::Expand implementation


void Contour::Expand(EExpandType type, double gap, ContourList &res) const
{
    if (gap==0) {
        res.append(*this);
        return;
    }
    if (size()==0 || boundingBox.x.Spans() < -2*gap || boundingBox.y.Spans() < -2*gap)
        return;

    Contour r(*this);
    Contour o(*this);
    if (size()==1) {
        _ASSERT(at(0).GetType()==EDGE_FULL_CIRCLE);
        if (!r[0].Expand(gap)) return; //full circle disappeared, we return empty "res"
        res.append(std::move(r));
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
    if (r2.size()==1 || SAME==r2.Untangle(res, WINDING_NONNEGATIVE))
        res.append(r2);        //expanded contour is not tangled, just add it
}

ContourList Contour::CreateExpand(double gap, EExpandType et) const
{
    ContourList cl;
    Expand(et, gap, cl);
    return cl;
}

Contour Contour::CreateWithLastEdge(unsigned i) const
{
    //if i is degenerate or already the last edge, return a copy of us
    if (size()==0 || i>=size()-1) return *this;
    Contour ret;
    ret.reserve(size());
    for (unsigned j = 0; j<size(); j++)
        ret.push_back(at((i+1+j)%size()));
    ret.boundingBox = boundingBox;
    return ret;
}


void Contour::Path(cairo_t *cr, bool inverse) const
{
    if (size()==0 || cr==NULL) return;
    cairo_move_to(cr, at(0).GetStart().x, at(0).GetStart().y);
    if (inverse)
        for (int i = size()-1; i>=0; i--)
            at(i).Path(cr, true);
    else
        for (unsigned i = 0; i<size(); i++)
            at(i).Path(cr);
    cairo_close_path(cr);
}

//Do not close the path or include the last edge (even if curvy)
void Contour::PathOpen(cairo_t *cr) const
{
    if (size()==0 || cr==NULL) return;
    cairo_move_to(cr, at(0).GetStart().x, at(0).GetStart().y);
    for (unsigned i = 0; i<size()-1; i++)
        at(i).Path(cr);
}

double Contour::do_offsetbelow(const Contour &below, double &touchpoint) const
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

void Contour::DoVerticalCrossSection(double x, DoubleMap<bool> &section, bool add) const
{
    double y[2], pos[2];
    bool forward[2];
    for (unsigned i=0; i<size(); i++) {
        const int num = at(i).CrossingVertical(x, y, pos, forward);
        for (int j = 0; j<num; j++)
            section.Set(y[j], forward[j] == add);
    }
}
