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
#include "contour.h"

//////ContourList/////////////////

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
    for (auto i = begin(); i!=end(); i++) 
        i->Invert();
}

void ContourList::SwapXY()
{
    for (auto i = begin(); i!=end(); i++) 
        i->SwapXY();
}

double ContourList::OffsetBelow(const SimpleContour &below, double &touchpoint, double offset) const
{
    if (offset < below.GetBoundingBox().y.from - GetBoundingBox().y.till) return offset;
    if (!GetBoundingBox().x.Overlaps(below.GetBoundingBox().x)) return offset;
    for (auto i = begin(); i!=end(); i++) {
        double off, tp;
        off = i->OffsetBelow(below, tp, offset);
        if (off < offset) {
            offset = off;
            touchpoint = tp;
        }
    }
    return offset;
}

double ContourList::OffsetBelow(const ContourList &below, double &touchpoint, double offset) const
{
    if (offset < below.GetBoundingBox().y.from - GetBoundingBox().y.till) return offset;
    if (!GetBoundingBox().x.Overlaps(below.GetBoundingBox().x)) return offset;
    for (auto i = begin(); i!=end(); i++)
        for (auto j = below.begin(); j!=below.end(); j++) {
            double off, tp;
            off = i->OffsetBelow(*j, tp, offset);
            if (off < offset) {
                offset = off;
                touchpoint = tp;
            }
        }
    return offset;
}

void ContourList::Expand(EExpandType et, double gap, Contour &result) const
{
    for (auto i=begin(); i!=end(); i++) {
        Contour res;
        i->Expand(et, gap, res);
        result += res;
    }
}

void ContourList::Path(cairo_t *cr) const
{
    for (auto i=begin(); i!=end(); i++) 
        i->Path(cr);
}

void ContourList::Path(cairo_t *cr, bool clockwiseonly) const
{
    for (auto i=begin(); i!=end(); i++) 
        i->Path(cr, clockwiseonly);
}



///ContoursHelper/////////////////////////////////////////////////////////


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
    const SimpleContour *contour;   //the contour the vertex belongs to
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
    mutable int switch_to;      //index of another ray for this cp, -1 if error
    Ray(const XY &point, const SimpleContour *c, unsigned v, double p, bool i, const RayAngle &a) :
        contour(c), vertex(v), pos(p), incoming(i), angle(a), xy(point),
        valid(true), switch_to(-1) {_ASSERT(c);}
    void Reset() const {valid = true; switch_to = -1;}
};

typedef std::vector<Ray> RayCollection;
struct RayPointer {
    bool at_vertex;  //true if we are at a vertex
    unsigned index;  //index of a ray. If we are at a vertex this points to a nearby ray on this contour
    unsigned vertex; //the #of vertex if we are at a vertex. Else ignore
    explicit RayPointer(unsigned i) : at_vertex(false), index(i) {}
};

//for post-processing
struct node
{
    SimpleContour contour;
    std::list<node> children;
    node(SimpleContour &&c) : contour(std::move(c)) {}
    node(const SimpleContour &c) : contour(c) {}
};


class ContoursHelper {
protected:
    const Contour        *C1, *C2;           //The contours we process
    RayCollection         Rays;               //All the rays of an operation for all cps
    unsigned              link_contours_head; //head of the contour loose list
    unsigned              link_cps_head;      //head of the cp loose list
    mutable std::vector<unsigned> StartRays;  //Rays where walks can start

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
    bool AddCrosspointHelper(const XY &point, const SimpleContour *c, unsigned v, double p, bool i, const RayAngle &a);
    void AddCrosspoint(const XY &xy, const SimpleContour *c1, unsigned v1, double p1, const SimpleContour *c2, unsigned v2, double p2);
    unsigned FindCrosspointsHelper(const SimpleContour *i);
    unsigned FindCrosspointsHelper(const ContourWithHoles *i);
    unsigned FindCrosspointsHelper(const ContourList *c); 
    unsigned FindCrosspointsHelper(const SimpleContour *c1, const SimpleContour *c2); 
    unsigned FindCrosspointsHelper(const ContourWithHoles *c1, const ContourWithHoles *c2); 
    unsigned FindCrosspointsHelper(const ContourWithHoles *cwh, const ContourList *cs);
    unsigned FindCrosspointsHelper(const ContourList *c1, const ContourList *c2); 
    unsigned FindCrosspoints();  //fills in Rays and the linked lists
    //helpers for rating crosspoints
    bool IsCoverageToInclude(int cov, Contour::operation_t type) const;
    unsigned FindRayGroupEnd(unsigned from, int &coverage, unsigned abort_at1, unsigned abort_at2) const;
    bool GoToCoverage(unsigned &from, unsigned &to, int &cov_now, Contour::operation_t type, bool start, unsigned abort_at) const;
    int  CalcCoverageHelper(unsigned cp_head, const ContourWithHoles *cwh) const;
    void ResetCrosspoints() const;    //make them valid and set switch_action to ERROR
    void EvaluateCrosspoints(Contour::operation_t type) const; //Fills in switch actions and StartRays
    //helpers for walking
    void Advance(RayPointer &p, bool forward) const;
    void InvalidateAllInRayGroupOf(unsigned ray) const;
    void Walk(RayPointer start, SimpleContour &result) const;
    //helper for post-processing
    node *ContoursHelper::InsertContour(std::list<node> *list, node &&n) const; 
    void InsertIfNotInRays(std::list<node> *list, const ContourWithHoles *c) const;
    void ConvertNode(std::list<node> &&list, ContourList &result) const;

public:
    //external interface
    ContoursHelper(const Contour &c) : C1(&c), C2(NULL) {FindCrosspoints();}
    ContoursHelper(const Contour &c1, const Contour &c2) : C1(&c1), C2(&c2) {FindCrosspoints();}
    void Do(Contour::operation_t type, Contour &result) const;
};

//returns the head of the strict list 
unsigned ContoursHelper::InsertToLooseList(Ray::list_type type, unsigned index) 
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
bool ContoursHelper::InsertToStrictList(Ray::list_type type, unsigned index, unsigned head)
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
bool ContoursHelper::AddCrosspointHelper(const XY &point, const SimpleContour *c, unsigned v, double p, bool i, const RayAngle &a)
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

void ContoursHelper::AddCrosspoint(const XY &xy, const SimpleContour *c1, unsigned v1, double p1, const SimpleContour *c2, unsigned v2, double p2)
{
    //In allrays even indexed positions are incoming edges, odd positions are outgoing ones
    if (p1==0)  //avoid pos==0 and incoming
        AddCrosspointHelper(xy, c1, v1, p1, true,  c1->at_prev(v1).Angle(true,  xy, 1));
    else
        AddCrosspointHelper(xy, c1, v1, p1, true,  c1->at(v1).Angle(true,  xy, p1));
    if (p1==1) //avoid pos==1 and outgoing
        AddCrosspointHelper(xy, c1, v1, p1, false, c1->at_next(v1).Angle(false, xy, 0));
    else
        AddCrosspointHelper(xy, c1, v1, p1, false, c1->at(v1).Angle(false, xy, p1));
    if (p2==0)  //avoid pos==0 and incoming
        AddCrosspointHelper(xy, c2, v2, p2, true,  c2->at_prev(v2).Angle(true,  xy, 1));
    else
        AddCrosspointHelper(xy, c2, v2, p2, true,  c2->at(v2).Angle(true,  xy, p2));
    if (p2==2) //avoid pos==1 and outgoing
        AddCrosspointHelper(xy, c2, v2, p2, false, c2->at_next(v2).Angle(false, xy, 0));
    else
        AddCrosspointHelper(xy, c2, v2, p2, false, c2->at(v2).Angle(false, xy, p2));
}


//finds all crosspoints between all simplecontours found in this tree.
unsigned ContoursHelper::FindCrosspointsHelper(const SimpleContour *i)
{
    unsigned ret=0;
    XY r[4];
    double one_pos[4], two_pos[4];
    //We ignore the holes for now, just calculate where i crosses itself
    for (unsigned u1 = 1; u1<i->size(); u1++)
        for (unsigned u2 = 0; u2<u1; u2++) {
            const unsigned n = i->at(u1).Crossing(i->at(u2), r, one_pos, two_pos);
            for (unsigned k=0; k<n;k++)
                AddCrosspoint(r[k], &*i, u1, one_pos[k], &*i, u2, two_pos[k]);
            ret += n;
        }
    return ret;
}

//finds all crosspoints between all simplecontours found in this tree.
unsigned ContoursHelper::FindCrosspointsHelper(const ContourWithHoles *i)
{
    //Find intersections of the main contour with itself
    unsigned ret = FindCrosspointsHelper(static_cast<const SimpleContour*>(i));
    //Now see where the main contour meets its holes
    for (auto j=i->holes.begin(); j!=i->holes.end(); j++)
        ret += FindCrosspointsHelper(static_cast<const SimpleContour*>(i), static_cast<const SimpleContour*>(&*j));
    return ret + FindCrosspointsHelper(&i->holes);
}

//finds all crosspoints between any two contours (no holes), and among the edges of each
unsigned ContoursHelper::FindCrosspointsHelper(const ContourList *c) 
{
    unsigned ret=0;
    //We ignore the holes, just calculate 
    for (auto i = c->begin(); i!=c->end(); i++) {
        //see where i crosses itself, including its holes
        ret += FindCrosspointsHelper(&*i);
        auto j = i;
        //now see where i crosses others (including holes)
        for (++j; j!=c->end(); j++) {
            if (i->GetBoundingBox().Overlaps(j->GetBoundingBox()))
                ret += FindCrosspointsHelper(&*i, &*j);
        }

    }
    return ret;
}


//finds all crosspoints between the two arguments, considering holes, too
unsigned ContoursHelper::FindCrosspointsHelper(const SimpleContour *i1, const SimpleContour *i2)
{
    unsigned ret=0;
    XY r[4];
    double one_pos[4], two_pos[4];
    for (unsigned u1 = 0; u1<i1->size(); u1++)
        for (unsigned u2 = 0; u2<i2->size(); u2++) {
            const unsigned n = i1->at(u1).Crossing(i2->at(u2), r, one_pos, two_pos);
            for (unsigned k=0; k<n;k++)
                AddCrosspoint(r[k], i1, u1, one_pos[k], i2, u2, two_pos[k]);
            ret += n;
        }
    return ret;
}

//finds all crosspoints between the two arguments, considering holes, too
unsigned ContoursHelper::FindCrosspointsHelper(const ContourWithHoles *i1, const ContourWithHoles *i2)
{
    //see where the two main covers meet
    unsigned ret = FindCrosspointsHelper(static_cast<const SimpleContour*>(i1), static_cast<const SimpleContour*>(i2));
    //now see how the holes cross the other's main cover
    for (auto i = i1->holes.begin(); i!=i1->holes.end(); i++)
        if (i->GetBoundingBox().Overlaps(i2->GetBoundingBox()))
            ret += FindCrosspointsHelper(&*i, i2);
    for (auto i = i2->holes.begin(); i!=i2->holes.end(); i++)
        if (i->GetBoundingBox().Overlaps(i1->GetBoundingBox()))
            ret += FindCrosspointsHelper(i1, &*i);
    //And now see how the holes meet each other
    if (i1->holes.GetBoundingBox().Overlaps(i2->holes.GetBoundingBox()))
        ret += FindCrosspointsHelper(&i1->holes, &i2->holes);
    return ret;
}
    
//finds all crosspoints between the two arguments, considering holes, too
unsigned ContoursHelper::FindCrosspointsHelper(const ContourWithHoles *cwh, const ContourList *cs)
{
    unsigned ret=0;
    for (auto i = cs->begin(); i!=cs->end(); i++)
        ret += FindCrosspointsHelper(cwh, &*i);
    return ret;
}
    

unsigned ContoursHelper::FindCrosspointsHelper(const ContourList *c1, const ContourList *c2)
{
    unsigned ret=0;
    for (auto i1 = c1->begin(); i1 != c1->end(); i1++) 
        for (auto i2 = c2->begin(); i2 != c2->end(); i2++) 
            //Search for cps only if the two main contour overlaps
            if (i1->GetBoundingBox().Overlaps(i2->GetBoundingBox()))
                ret += FindCrosspointsHelper(&*i1, &*i2);
    return ret;
}

unsigned ContoursHelper::FindCrosspoints()
{
    //Reset stuff
    Rays.clear();
    Rays.reserve(200);
    link_cps_head = link_contours_head = 0;

    unsigned ret = 0;
    if (C2==NULL) {
        ret = FindCrosspointsHelper(static_cast<const ContourWithHoles*>(C1));
        if (C1->further.size())
            ret += FindCrosspointsHelper(static_cast<const ContourWithHoles*>(C1), &C1->holes);
    } else if (C1->GetBoundingBox().Overlaps(C2->GetBoundingBox())) {
        ret = FindCrosspointsHelper(static_cast<const ContourWithHoles*>(C1), static_cast<const ContourWithHoles*>(C2));
        if (C2->further.GetBoundingBox().Overlaps(C1->GetBoundingBox()))
            ret += FindCrosspointsHelper(static_cast<const ContourWithHoles*>(C1), &C2->further);
        if (C1->further.GetBoundingBox().Overlaps(C2->GetBoundingBox()))
            ret += FindCrosspointsHelper(static_cast<const ContourWithHoles*>(C2), &C1->further);
        if (C2->further.GetBoundingBox().Overlaps(C1->further.GetBoundingBox()))
            ret += FindCrosspointsHelper(&C1->further, &C2->further);
    }
    return 0;
}

//After this you can call again EvaluateCrosspoints with a different operation
void ContoursHelper::ResetCrosspoints() const
{
    StartRays.clear();
    for (auto i = Rays.begin(); i!=Rays.end(); i++) 
        i->Reset();
}

inline bool ContoursHelper::IsCoverageToInclude(int cov, Contour::operation_t type) const
{
    //If union, pick the angle ranges covered by at least one contour
    //If intersect, pick angle ranges covered by all of the contours
    //If xor, we pick ranges of odd coverage
    //for untangle we formulate winding rules as they say
    switch (type) {
    default: _ASSERT(0); /*fallthrough*/
    case Contour::COMBINE_INTERSECT:   _ASSERT(C2); return cov>=2;
    case Contour::COMBINE_UNION:       _ASSERT(C2); return cov>=1;
    case Contour::COMBINE_XOR:         _ASSERT(C2); return cov==1; 
    case Contour::WINDING_NONZERO:     return cov!=0;
    case Contour::WINDING_EVENODD:     return cov%2==1; //here we can gave cov<0
    case Contour::WINDING_NONNEGATIVE: return cov>0;
    }
}

//return the ray after the current ray-group
//"coverage" starts as coverage just before "from" and ends as coverage just before returned ray
inline unsigned ContoursHelper::FindRayGroupEnd(unsigned from, int &coverage, unsigned abort_at1, unsigned abort_at2) const
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
bool ContoursHelper::GoToCoverage(unsigned &from, unsigned &to, int &cov_now, Contour::operation_t type, bool start, unsigned abort_at) const
{
    const unsigned started = to = from;
    do {
        from = to;
        to = FindRayGroupEnd(from, cov_now, started, abort_at);
    } while (to!=started && to!=abort_at && start != IsCoverageToInclude(cov_now, type));
    return start == IsCoverageToInclude(cov_now, type);
}

//This assumes there is only one Contour and with_self is true
//if finds what is the coverage near the cp of the cp_head, just before the direction of <0,-inf>
int ContoursHelper::CalcCoverageHelper(unsigned cp_head, const ContourWithHoles *cwh) const
{
            //For each edge, we see if it crosses the (xy.x, xy.y)->(inf, xy.y) line
            //if it crosses clockwise: +1 cov
            //if it crosses cclockwise: -1 cov
            //if it is an ellipse that crosses twice and not in xy: 
            //    change cov according to cps that ar strictly left of xy
            //if it is an ellipse that touches the line: no change in cov
            //horizontal lines through x are ignored: we seek coverage just before (0;-inf)
    int ret = 0;
    const XY xy = Rays[cp_head].xy;
    for (unsigned e=0;e<cwh->size();e++) {
        double x[2], pos[2];
        bool fw[2];
        Edge tmp(cwh->at(e));
        tmp.SwapXY();
        const int num = tmp.CrossingVertical(xy.y, x, pos, fw);
        //do nothing for -1 or 0 returns
        for (int f=0; f<num; f++)
            if (test_smaller(xy.x, x[f])) {
                if (fw[f]) ret++;
                else ret--;
            }
    }
    //And now add coverage for curves with tangent of zero
    unsigned ray_no = cp_head; 
    while (Rays[ray_no].angle.angle==0 && Rays[ray_no].angle.curve<0) {
        if (Rays[ray_no].incoming) ret++;
        else ret--;
        ray_no = Rays[ray_no].link_in_cp.next;
        if (ray_no == cp_head) break;
    };

    //now consider the holes
    for (auto i = cwh->holes.begin(); i!=cwh->holes.end(); i++)
        ret += CalcCoverageHelper(cp_head, &*i);
    return ret;
}


//Copy the processed crosspoints from the set to the bycont, where they are sorted by <contour,edge,pos>.
//(each contour has its own bycont).
//Fill in startrays with incoming rays marked as can_start_here
void ContoursHelper::EvaluateCrosspoints(Contour::operation_t type) const
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
        if (C2==NULL)  { //we do untangle
            coverage_before_r = CalcCoverageHelper(cp_head, C1);
            for (auto i = C1->further.begin(); i!=C1->further.end(); i++)
                coverage_before_r += CalcCoverageHelper(cp_head, &*i);
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
            for (unsigned i = start_from; i!=start_to; i = Rays[i].link_in_cp.next) 
                Rays[i].switch_to = end_from;
            for (unsigned i = end_from; i!=end_to; i = Rays[i].link_in_cp.next) 
                Rays[i].switch_to = start_from;
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


void ContoursHelper::Advance(RayPointer &p, bool forward) const
{
    if (p.at_vertex) {  //we are at a vertex
        if (forward) {
            if (Rays[p.index].vertex == p.vertex) {  //Ok, upcoming edge has a crosspoint
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
            p.at_vertex = true;
            if (forward) 
                p.vertex = Rays[p.index].contour->next(p.vertex);
            // else use updated p.vertex
        }
        //else we remained on the same edge, use updated p.index
    }
}

//Marks all rays in the ray group of "ray" as invalid
void ContoursHelper::InvalidateAllInRayGroupOf(unsigned ray) const
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
void ContoursHelper::Walk(RayPointer start, SimpleContour &result) const
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
                result.AppendDuringWalk(Rays[current.index].contour->at(current.vertex));
            else {
                Edge edge(Rays[current.index].contour->at_prev(current.vertex));
                edge.Invert();
                result.AppendDuringWalk(edge);
            }
            //no need to modify "current" here. A vertex is treated as both an incoming and outgoing ray
        } else { //we are at a crosspoint, not a vertex
            const Ray &ray = Rays[current.index];  //ray on which we arrive
            _ASSERT(forward == ray.incoming);
            //Mark the incoming ray (and all incoming rays its entire ray group) as DONE
            InvalidateAllInRayGroupOf(current.index);
            if (ray.switch_to < 0) {
                _ASSERT(0);
                result.clear();
                return;
            }
            //Now switch to outgoing ray
            current.index = Rays[current.index].switch_to;
            const Ray &next_ray = Rays[current.index];
            forward = !next_ray.incoming;  //fw may change if we need to walk on an incoming ray
            //Append a point
            if (forward) {
                Edge edge(next_ray.contour->at(next_ray.vertex));
                edge.SetStartStrict(next_ray.xy, next_ray.pos, true);
                result.AppendDuringWalk(edge);
            } else {
                Edge edge(next_ray.contour->at(next_ray.vertex));
                edge.Invert();
                edge.SetStartStrict(next_ray.xy, 1-next_ray.pos, true);
                result.AppendDuringWalk(edge);
            }
        }
        //Now find the next cp and corresponding incoming ray
        Advance(current, forward);
    } while (current.at_vertex || Rays[current.index].seq_num != sn_finish);
}

//inserts a node into the tree, returns the resulting inserted node
node * ContoursHelper::InsertContour(std::list<node> *list, node &&n) const
{
    for (auto i = list->begin(); i!=list->end(); /*nope*/) {
        const SimpleContour::result_t res = i->contour.CheckContainment(n.contour);
        switch (res) {
        default:
        case SimpleContour::A_IS_EMPTY:
        case SimpleContour::BOTH_EMPTY:  //nodes already inserted cannot be empty
        case SimpleContour::OVERLAP:     //at this point we cannot have overlap
            _ASSERT(0);
        case SimpleContour::A_INSIDE_B:  
            //move "i" from the "list" to "n.children"
            n.children.splice(n.children.end(), *list, i++);
            //continue checking
            break; 
        case SimpleContour::B_IS_EMPTY:
        case SimpleContour::SAME:
            return &*i; //easy, do nothig
        case SimpleContour::B_INSIDE_A:
            //insert into the children of "i" instead;
            return InsertContour(&i->children, std::move(n));
        case SimpleContour::APART:
            i++;
            break; //continue checking the remainder of the list
        }
    }
    //at this point we are APART with all elements in "list"
    list->push_back(std::move(n));
    return &*list->rbegin();
}

//inserts a node into the tree, returns the resulting inserted node
void ContoursHelper::InsertIfNotInRays(std::list<node> *list, const ContourWithHoles *c) const
{
    //Check if "i" has crosspoints, if so, ignore
    bool had_cp = false;
    if (Rays.size()) {
        unsigned u = link_contours_head; 
        do {
            if (Rays[u].contour == c) break;
            u = Rays[u].link_contours.next;
        } while (u!=link_contours_head);
        if (Rays[u].contour == c) had_cp = true;
    }
    if (!had_cp) {
        //this contour had no crosspoints -> place into the tree
        //use its children list for any holes it may have
        node *n = InsertContour(list, node(*c));
        _ASSERT(n);
        list = &n->children;
    } //else we skip the contour and use the original list for holes
    for (auto i = c->holes.begin(); i!=c->holes.end(); i++)
        InsertIfNotInRays(list, &*i);
}

void ContoursHelper::ConvertNode(std::list<node> &&list, ContourList &result) const 
{
    for (auto i = list.begin(); i!=list.end(); i++) {
        ContourWithHoles c(std::move(i->contour));
        ConvertNode(std::move(i->children), c.holes);
        result.append(std::move(c));
    }
}

void ContoursHelper::Do(Contour::operation_t type, Contour &result) const
{
    if (C2==NULL) {
        _ASSERT(type != Contour::COMBINE_UNION);
        _ASSERT(type != Contour::COMBINE_INTERSECT);
        _ASSERT(type != Contour::COMBINE_XOR);
    }
    std::list<node> list;
    if (Rays.size()) {
        //evaluate crosspoints
        EvaluateCrosspoints(type); // Process each cp and determine if it is relevant to us or not
        //Walk while we have eligible starting points
        SimpleContour walk; //static: we keep allocated memory between calls for performance
        walk.reserve(200);
        while (StartRays.size()) {
            Walk(RayPointer(*StartRays.rbegin()), walk);
            //clear invalid startrays from end
            while (StartRays.size() && !Rays[*StartRays.rbegin()].valid)
                StartRays.pop_back();
		    //If we have abandoned this polyline, do not perform the processing below: start with another polyline
            if (!walk.PostWalk()) continue;
            _ASSERT(walk.IsSane());
            InsertContour(&list, node(std::move(walk)));  //"walk" is destroyed
        }
        ResetCrosspoints(); //A subsequent call to "Do" can work OK, again
    }
    //Now we need to consider those contours from the original set
    //for which we found no crosspoints (may be all of them, none of them or in bw)
    InsertIfNotInRays(&list, C1);
    for (auto i = C1->further.begin(); i!=C1->further.end(); i++)
        InsertIfNotInRays(&list, &*i);
    if (C2) {
        InsertIfNotInRays(&list, C2);
        for (auto i = C1->further.begin(); i!=C1->further.end(); i++)
            InsertIfNotInRays(&list, &*i);
    }
    //Convert the tree of nodes to a Contour object
    result.clear();
    for (auto i = list.begin(); i!=list.end(); i++) {
        ContourWithHoles c(std::move(i->contour));
        ConvertNode(std::move(i->children), c.holes);
        result.append(std::move(c));
    }
}

/////////////////////ContourWithHoles
    
bool ContourWithHoles::IsSane(bool shouldbehole) const
{
    if (!SimpleContour::IsSane()) return false;
    if (GetClockWise()==shouldbehole) return false; 
    for (auto i=holes.begin(); i!=holes.end(); i++) 
        if (!i->IsSane(!shouldbehole))
            return false;
    return true;
}


/////////////////////////////////////////  Contour implementation

void Contour::assign(const std::vector<XY> &v, bool winding)
{
    clear();
    if (v.size()<2) return;
    SimpleContour tmp;
    for (unsigned i=0; i<v.size(); i++)
        tmp.push_back(Edge(v[i], v[(i+1)%v.size()]));
    tmp.CalculateBoundingBox();
    Contour tmp2(std::move(tmp));
    Operation(winding ? Contour::WINDING_NONZERO : Contour::WINDING_EVENODD, tmp2);
}

void Contour::assign(const XY v[], unsigned size, bool winding)
{
    clear();
    if (size < 2) return;
    SimpleContour tmp;
    for (unsigned i=0; i<size; i++)
        tmp.push_back(Edge(v[i], v[(i+1)%size]));
    tmp.CalculateBoundingBox();
    Contour tmp2(std::move(tmp));
    Operation(winding ? Contour::WINDING_NONZERO : Contour::WINDING_EVENODD, tmp2);
}

void Contour::assign(const std::vector<Edge> &v, bool winding)
{
    clear();
    if (v.size()<2) return;
    SimpleContour tmp;
    static_cast<std::vector<Edge>*>(&tmp)->operator=(v);
    if (tmp.Sanitize()) {
        tmp.CalculateBoundingBox();
        Contour tmp2(std::move(tmp));
        Operation(winding ? Contour::WINDING_NONZERO : Contour::WINDING_EVENODD, tmp2);
    }
}

void Contour::assign(const Edge v[], unsigned size, bool winding)
{
    clear();
    if (size < 2) return;
    SimpleContour tmp;
    for (unsigned i=0; i<size; i++)
        tmp.push_back(v[i]);
    if (tmp.Sanitize()) {
        tmp.CalculateBoundingBox();
        Contour tmp2(std::move(tmp));
        Operation(winding ? Contour::WINDING_NONZERO : Contour::WINDING_EVENODD, tmp2);
    }
}

bool Contour::IsSane() const
{
    if (!ContourWithHoles::IsSane()) return false;
    for (auto i = further.begin(); i!=further.end(); i++)
        if (!i->IsSane()) return false;
    return true;
}

void Contour::Operation(operation_t type, const Contour &c1) 
{
    ContoursHelper h(c1); 
    h.Do(type, *this);
//    _ASSERT(IsSane());
}

void Contour::Operation(operation_t type, const Contour &c1, const Contour &c2)  
{
    ContoursHelper h(c1, c2); 
    h.Do(type, *this);
    _ASSERT(IsSane());
}

Contour Contour::CreateExpand(double gap, EExpandType et) const
{
    Contour result;
    if (gap == 0) return (result = *this);  //always return result->compiler optimizes
    Contour::Expand(et, gap, result);
    return result;
}

