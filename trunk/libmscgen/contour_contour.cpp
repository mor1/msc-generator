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

struct Ray
{
    unsigned contour;   //number of the contour the vertex belongs to
    unsigned vertex;    //the number of the vertex staring the edge the crosspoint is on
    double   pos;       //-1 if it is a vertex and not a crosspoint, [0..1) the pos of the crosspoint on the edge
    bool     incoming;  //true if it is incoming
    RayAngle angle;
    XY       xy;
    //values used during any walk (both untangle and combine)
    mutable bool valid;
    //playground below for combine
    mutable unsigned seq_num; //same seq num as where we started a walk indicates a stopping criteria
    mutable std::pair<unsigned, unsigned> switch_to; //indices of another ray for this cp
    mutable enum switch_action_t {IGNORE, ERROR, SWITCH} switch_action;
    Ray(const XY &point, unsigned c, unsigned v, double p, bool i, const RayAngle &a) :
        contour(c), vertex(v), pos(p), incoming(i), angle(a), xy(point),
        valid(true), switch_action(ERROR) {}
};

struct RayPointer {
    unsigned contour;
    unsigned vertex;
    int cp_index; //if <0, we are not at a crosspoint, but at a vertex
    RayPointer() {}
    RayPointer(unsigned c, unsigned v, int p=-1) : contour(c), vertex(v), cp_index(p) {}
};

//index type for crosspoints
struct XY2
{
    XY xy;
    mutable unsigned coverage_before_0_minusinf;
    XY2(const XY &p) : xy(p), coverage_before_0_minusinf(0) {}
    bool operator < (const XY2&o) const {return xy.test_smaller(o.xy);}
};

typedef std::vector<Ray> RayCollection;
typedef std::vector<std::vector<unsigned>> RayArray;

//If pos is equal, we put incoming as smaller than outgoing
struct SmallerRayByPos {
    const RayCollection & rc;
    SmallerRayByPos(const RayCollection & _rc) : rc(_rc) {};
    bool operator()(unsigned r1, unsigned r2) const 
        {return rc[r1].pos == rc[r2].pos ? rc[r1].incoming == rc[r2].incoming ? false : rc[r1].incoming : rc[r1].pos<rc[r2].pos;}
};

struct SmallerRayArrayByAngle {
    const RayCollection & rc;
    SmallerRayArrayByAngle(const RayCollection & _rc) : rc(_rc) {};
    bool operator()(const std::vector<unsigned> &ra1, const std::vector<unsigned> &ra2) const 
        {return rc[ra1[0]].angle.Smaller(rc[ra2[0]].angle);}
};


class ContourHelper {
protected:
    std::vector<const Contour*> Contours;      //The contours we process
    RayCollection               AllRays;       //All the rays of an operation for all cps
    std::map<XY2, RayArray>     RaysByCP;      //Rays ordered by crosspoints
    std::vector<RayArray>       RaysByContour; //Rays ordered by contours and vertices and pos
    std::vector<RayPointer>     StartRays;

    //helpers
    void AddCrosspointHelper(const Ray &ray);
    void AddCrosspoint(const XY &xy, unsigned c1, unsigned v1, double p1, unsigned c2, unsigned v2, double p2);
    bool IsCoverageToInclude(unsigned cov, Contour::combine_t type) const;
    bool GoToCoverage(const RayArray &ra, unsigned &cov_now, Contour::combine_t type, bool start, unsigned &i) const;
    unsigned FindRayOnEdge(const Ray &ray) const;
    const Ray* StepOutgoing2Incoming(RayPointer &p) const;
    RayPointer RayToPointer(const Ray &outgoing) const;
    std::pair<unsigned, unsigned> FindRayInCP(const RayArray &CP, const Ray*ray) const;
    bool FindSuitableOutgoingRayInCP(const RayArray &CP, std::pair<unsigned, unsigned> &rai) const;
    unsigned GetStartRayNo() const {return StartRays.size();}

    //main functions
    unsigned FindCrosspoints(); //fills in AllRays, RaysByCP and RaysByContour
    void Process4Combine(Contour::combine_t type); //Fills in switch actions and StartRays, too
    void Walk4Combine(Contour &result); //walks two polygons based on Rays
    void Walk4Untangle(RayPointer start, ContourList &surfaces, ContourList &holes);

public:
    //external interface
    ContourHelper(const Contour &c) : Contours(1, &c) {};
    ContourHelper(const Contour &c1, const Contour &c2) : Contours(2, &c1) {Contours[1] = &c2;}
    bool Combine(Contour::combine_t type, ContourList &surfaces, ContourList &holes);
    bool Untangle(ContourList &surfaces, ContourList &holes);
};

void ContourHelper::AddCrosspointHelper(const Ray &ray)
{
    //See if we already have this crosspoint
    std::vector<unsigned> &v_vertex = RaysByContour[ray.contour][ray.vertex];
    for (unsigned u = 0; u<v_vertex.size(); u++)
        if (test_equal(AllRays[v_vertex[u]].pos, ray.pos) && 
            AllRays[v_vertex[u]].incoming == ray.incoming) 
                return;
    //no crosspoint have been registered for this <contour, vertex, pos, incoming> triplet
    AllRays.push_back(ray);
    v_vertex.push_back(AllRays.size()-1);
}

void ContourHelper::AddCrosspoint(const XY &xy, unsigned c1, unsigned v1, double p1, unsigned c2, unsigned v2, double p2)
{
    //In allrays even indexed positions are incoming edges, odd positions are outgoing ones
    if (p1==0)  //avoid pos==0 and incoming
        AddCrosspointHelper(Ray(xy, c1, v1, p1, true,  Contours[c1]->at_prev(v1).Angle(true,  xy, 1)));
    else
        AddCrosspointHelper(Ray(xy, c1, v1, p1, true,  Contours[c1]->at(v1).Angle(true,  xy, p1)));
    if (p1==1) //avoid pos==1 and outgoing
        AddCrosspointHelper(Ray(xy, c1, v1, p1, false, Contours[c1]->at_next(v1).Angle(false, xy, 0)));
    else
        AddCrosspointHelper(Ray(xy, c1, v1, p1, false, Contours[c1]->at(v1).Angle(false, xy, p1)));
    if (p2==0)  //avoid pos==0 and incoming
        AddCrosspointHelper(Ray(xy, c2, v2, p2, true,  Contours[c2]->at_prev(v2).Angle(true,  xy, 1)));
    else
        AddCrosspointHelper(Ray(xy, c2, v2, p2, true,  Contours[c2]->at(v2).Angle(true,  xy, p2)));
    if (p2==2) //avoid pos==1 and outgoing
        AddCrosspointHelper(Ray(xy, c2, v2, p2, false, Contours[c2]->at_next(v2).Angle(false, xy, 0)));
    else
        AddCrosspointHelper(Ray(xy, c2, v2, p2, false, Contours[c2]->at(v2).Angle(false, xy, p2)));
}

unsigned ContourHelper::FindCrosspoints()
{
    //Allocate Raysbycontour
    RaysByContour.resize(Contours.size());
    for (unsigned i = 0; i<RaysByContour.size(); i++)
        RaysByContour[i].resize(Contours[i]->size());

    XY r[4];
    double one_pos[4], two_pos[4];
    unsigned ret=0;
    if (Contours.size()==1) {
        const Contour &contour = *Contours[0];
        for (unsigned i = 1; i<contour.size(); i++)
            for (unsigned j = 0; j<i; j++) {
                const unsigned n = contour[i].Crossing(contour[j], r, one_pos, two_pos);
                for (unsigned k=0; k<n;k++)
                    AddCrosspoint(r[k], 0, i, one_pos[k], 0, j, two_pos[k]);
                ret += n;
            }
    } else if (Contours.size()==2) {
        const Contour &c0 = *Contours[0];
        const Contour &c1 = *Contours[1];
        for (unsigned i = 0; i<c0.size(); i++)
            for (unsigned j = 0; j<c1.size(); j++) {
                const unsigned n = c0[i].Crossing(c1[j], r, one_pos, two_pos);
                for (unsigned k=0; k<n;k++)
                    AddCrosspoint(r[k], 0, i, one_pos[k], 1, j, two_pos[k]);
                ret += n;
            }
    } else {
        _ASSERT(0); //not yet prepared to do more than two polygons
    }

    //Add rays to both indexes
    for(auto i = AllRays.begin(); i!=AllRays.end(); i++) {
        //Add this ray to the CP index. (Bycontour was done by AddCrosspoint)
        //see if i->xy is already a cp, if not, insert an empty array for it
        auto itr = RaysByCP.insert(std::pair<XY2, RayArray>(i->xy, RayArray())).first;
        //find if there is a ray already that is similar to "i"
        auto j = itr->second.begin();
        for (/*none*/; j!=itr->second.end(); j++)
            if (AllRays[j->at(0)].angle.IsSimilar(i->angle)) break;
        if (j == itr->second.end()) {
            itr->second.resize(itr->second.size()+1);
            j = --itr->second.end();
        }
        j->push_back(i-AllRays.begin());

        //if we are processing an outgoing edge, see if the incoming->outgoing pair
        //covers angle(0,-inf), in which case we need to increment coverage_at_0 for this cp
        //(In allrays incoming edges come first then their outgoing peer)
        if (!i->incoming)
            if ((--RayCollection::iterator(i))->angle.Smaller(i->angle))
                itr->first.coverage_before_0_minusinf++;
    }

    //Now sort the indexes
    //By contour shall be sorted by "pos" values per vertex and contour
    SmallerRayByPos smaller_ray_by_pos(AllRays);
    SmallerRayArrayByAngle smaller_ray_array_by_angle(AllRays);
    for (unsigned i = 0; i<RaysByContour.size(); i++)
        for (unsigned j = 0; j<RaysByContour[i].size(); j++)
            std::sort(RaysByContour[i][j].begin(), RaysByContour[i][j].end(), smaller_ray_by_pos);
    //CP ray blocks shall be sorted by their angle (same for all, use first in array)
    for (auto i = RaysByCP.begin(); i!=RaysByCP.end(); i++)
        std::sort(i->second.begin(), i->second.end(), smaller_ray_array_by_angle);
    return ret;
}

inline bool ContourHelper::IsCoverageToInclude(unsigned cov, Contour::combine_t type) const
{
    switch (type) {
    default: _ASSERT(0); /*fallthrough*/
    case Contour::COMBINE_INTERSECT: return cov>=Contours.size();
    case Contour::COMBINE_UNION:     return cov>=1;
    case Contour::COMBINE_XOR:       return cov%2==1;
    }
}

//If "start" is true, go to the ray group *after* which coverage becomes eligible for inclusion
//  (that is, for intersects >=no of contours, for union >=1 and for xor odd; see above in IsCoverageToInclude),
//else go to the ray group *after* which coverage becomes ineligible
//coverage after ray group "i" is "cov_now"
//return false if coverage never becomes eligible or ineligible (as required by start)
//if coverage is appropriate after current group, remain as is
bool ContourHelper::GoToCoverage(const RayArray &ra, unsigned &cov_now, Contour::combine_t type, bool start, unsigned &i) const
{
    const unsigned started = i;
    while (start != IsCoverageToInclude(cov_now, type)) {
        i = (i+1)%ra.size();
        if (i == started) return false;
        for (unsigned u = 0; u<ra[i].size(); u++)
            if (AllRays[ra[i][u]].incoming) cov_now--;
            else cov_now++;
    };
    return true;
}

unsigned ContourHelper::FindRayOnEdge(const Ray &ray) const
{
    int p;
    for (p=RaysByContour[ray.contour][ray.vertex].size()-1; p>=0; p--)
        if (&AllRays[RaysByContour[ray.contour][ray.vertex][p]] == &ray) break;
    if (p>=0) return p;
    _ASSERT(0);
    return 0;
}

//Copy the processed crosspoints from the set to the bycont, where they are sorted by <contour,edge,pos>.
//(each contour has its own bycont).
//Fill in startrays with incoming rays marked as can_start_here
void ContourHelper::Process4Combine(Contour::combine_t type)
{
    unsigned seq_num = 0;
    //If union, pick the angle ranges covered by at least one contour
    //If intersect, pick angle ranges covered by all of the contours
    //If xor, we pick ranges of odd coverage
    //const unsigned coverage_req = doUnion ? 1 : Contours.size();

    //Cylce through the crosspoints
    for (auto iCP = RaysByCP.begin(); iCP!=RaysByCP.end(); iCP++) {
        //for union coverage is sufficient if any one contour covers an area.
        //for intersects all of them have to cover it.
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

        const RayArray &CP = iCP->second;

        unsigned ray_group = CP.size()-1;
        unsigned coverage_after_rg = iCP->first.coverage_before_0_minusinf;
        //coverage_before_0_minusinf actually shows coverage after the last ray group
        //(which is of number CP.size()-1)
        //find first ray group after we shall not include. (coverage is below requirement or is even for xor)
        if (!GoToCoverage(CP, coverage_after_rg, type, false, ray_group))
            continue; //never happens -> this is a crosspoint not needed, all switch_action will remain ERROR
        const unsigned original_started_at_ray_group = ray_group;
        while(1) {
            const unsigned started_at_ray_group = ray_group; //==the previous "ray_group_end" if not the first pass of while-cycle
            //find first ray group after which coverage is above reauirement
            if (!GoToCoverage(CP, coverage_after_rg, type, true, ray_group))
                break; //OK, we are done
            //find the group after which takes the coverage below reauirement
            const unsigned ray_group_begin = ray_group;
            //if the new ray_group_begin moved over the original_started_at_ray_group, we are done
            if ((CP.size()-original_started_at_ray_group+ray_group_begin)%CP.size() < (CP.size()-original_started_at_ray_group+started_at_ray_group)%CP.size())
                break;
            if (!GoToCoverage(CP, coverage_after_rg, type, false, ray_group)) {
                _ASSERT(0); //we must find one
            }
            const unsigned ray_group_end = ray_group;
            //Now we know that coverage is above the required between ray_group_begin and ray_group.
            //First we set all incoming ray's sequence number in the whole range to the same seq number.
            //This way if the a Walk starts at this cp, if we hit this cp again, we can stop the walk,
            //based on equal sequence numbers.
            //RA indexes may wrap, so incrementing shall be modulo

            unsigned i = ray_group_begin;
            while(1) {
                for (unsigned u=0; u<CP[i].size(); u++)
                    if (AllRays[CP[i][u]].incoming)
                        AllRays[CP[i][u]].seq_num = seq_num;
                if (i==ray_group_end) break;
                i = (i+1)%CP.size();
            };

            //Now set the "switch_to" in all the incoming edges in (ray_group) to one of the outgoing edges in (ray_group_begin)
            //If the same edge of the same contour and vertex can be found among the rays of ray_group and ray_group_begin,
            //then instead of the above, just mark switch_action with IGNORE: we will remain on the same contour
            //IGNORE will result in an invalid cp, meaning it will not be added to any polyline (but will be checked for stop condition)
            //We mark only one of the incoming edges as a crosspoint we can start at
            //Find an outgoing
            unsigned an_outgoing;  //index of an outgoing edge in ray_group_begin
            for (an_outgoing = 0; an_outgoing < CP[ray_group_begin].size(); an_outgoing++)
                if (!AllRays[CP[ray_group_begin][an_outgoing]].incoming) break;
            _ASSERT(an_outgoing < CP[ray_group_begin].size());

            bool did_start = false;
            for (unsigned i=0; i<CP[ray_group_end].size(); i++) {
                const Ray &ray = AllRays[CP[ray_group_end][i]];
                if (ray.incoming) {
                    //search for this incoming edge among the outgoing rays in ray_group_begin
                    unsigned the_outgoing;
                    for (the_outgoing=0; the_outgoing<CP[ray_group_begin].size(); the_outgoing++) {
                        const Ray &ray2 = AllRays[CP[ray_group_begin][the_outgoing]];
                        if (ray.contour==ray2.contour && ray.vertex==ray2.vertex && !ray2.incoming) break;
                    }
                    if (the_outgoing<CP[ray_group_begin].size()) {//the outgoing ray corresponding to this incoming ray is in (A)
                        ray.switch_action = Ray::IGNORE;
                        ray.switch_to.first = ray_group_begin;
                        ray.switch_to.second = the_outgoing;
                    } else {               //not found, just pick an arbitrary outgoing edge
                        ray.switch_action = Ray::SWITCH;
                        ray.switch_to.first = ray_group_begin;
                        ray.switch_to.second = an_outgoing;
                        if (!did_start) {
                            did_start = true;
                            //Add this ray to startpoints
                            StartRays.push_back(RayPointer(ray.contour, ray.vertex, FindRayOnEdge(ray)));
                        }
                    }
                } /* if incoming */
            } /* for through the ray group's incoming rays*/
            seq_num++;
            if (original_started_at_ray_group == ray_group_end)
                break; //we have just completed a full scan
        } //while through regions of sufficient coverage
    } /* while cycle through the crosspoints */
}

const Ray *ContourHelper::StepOutgoing2Incoming(RayPointer &p) const 
{
    p.cp_index++;
    _ASSERT(p.cp_index>=0);
    if (RaysByContour[p.contour][p.vertex].size()<=unsigned(p.cp_index)) {
        //we are last cp on an edge, next point is the vertex
        p.vertex = (p.vertex+1)%Contours[p.contour]->size();
        p.cp_index = -1;
        return NULL;
    }
    return &AllRays[RaysByContour[p.contour][p.vertex][p.cp_index]];
}

inline RayPointer ContourHelper::RayToPointer(const Ray &outgoing) const
{
    return RayPointer(outgoing.contour, outgoing.vertex, FindRayOnEdge(outgoing));
}

inline std::pair<unsigned, unsigned> ContourHelper::FindRayInCP(const RayArray &CP, const Ray*ray) const
{
    std::pair<unsigned, unsigned> rai; //as _R_ay_A_rray _i_ndex
    for (rai.first=0; rai.first<CP.size(); rai.first++)
        for (rai.second=0; rai.second<CP[rai.first].size(); rai.second++)
            if (&AllRays[CP[rai.first][rai.second]] == ray) return rai;
    _ASSERT(0); //should have found it
    return rai; //supress warnings
}

//finds a valid outgoing ray in this crosspoint counterclockwise, if any.
//Search rules are:
//If there is a valid outgoing ray in the current ray group, we return that (and true).
//Else we go counterclockwise through the ray groups and 
//- For each ray group we calculate the difference of the number of valid incoming and outgoing rays
//   and manage a runnig sum.
//- If after the calculations and summation at a ray group we got a negative value (more outgoing)
//  we return any outgoing ray from that ray group (and true).
//- If we do not find any valid rays, we return false and do not change "rai"
//We assume that at any given time during an untangle walk, in any crosspoint the number of 
//valid incoming and outgoing rays are the same.
inline bool ContourHelper::FindSuitableOutgoingRayInCP(const RayArray &CP, std::pair<unsigned, unsigned> &rai) const
{
    const std::pair<unsigned, unsigned> rai_started = rai; //as _R_ay_A_rray _i_ndex
    //we always try to find a valid ray in the current ray group first
    for (rai.second=0; rai.second<CP[rai.first].size(); rai.second++)
        if (AllRays[CP[rai.first][rai.second]].valid && 
            !AllRays[CP[rai.first][rai.second]].incoming) 
            return true;
    int counter = 0;
    while(1) {
        //decrement ray group
        if (rai.first>0)
            --rai.first;
        else
            rai.first = CP.size()-1;
        //if we got back to the starting one, we fail
        if (rai.first == rai_started.first) {
            rai = rai_started;
            return false;
        }
        //now search new ray group
        int an_outgoing = -1;
        for (rai.second=0; rai.second<CP[rai.first].size(); rai.second++)
            if (AllRays[CP[rai.first][rai.second]].valid) {
                if (AllRays[CP[rai.first][rai.second]].incoming) 
                    counter++;
                else {
                    counter--;
                    an_outgoing = (int)rai.second;
                }
            }
        if (counter<0) {//we found outgoing rays, return one
            _ASSERT(an_outgoing>=0);
            rai.second = (unsigned)an_outgoing;
            return true;
        }
        //We need to continue
    };
}


//Walk around the contours starting from startpoint and follow the
//switch_action bycontd for each incoming ray. This is used for union and intersect (and substract)
void ContourHelper::Walk4Combine(Contour &result)
{
    result.clear();
    //current will point to an incoming ray at a crosspoint
    RayPointer current;
    //find a crosspoint ray that is still valid
    do {
        if (StartRays.size()==0) return; //no more crosspoints
        current = *StartRays.rbegin();
        StartRays.pop_back();
    } while (!AllRays[RaysByContour[current.contour][current.vertex][current.cp_index]].valid);

    //do a walk from the current crosspoint, until we get back here
    const unsigned sn_finish = AllRays[RaysByContour[current.contour][current.vertex][current.cp_index]].seq_num;
    do {
        //here "current" points to an incoming ray
        if (current.cp_index>=0) { //we are at a crosspoint, not a vertex
            const Ray &incoming = AllRays[RaysByContour[current.contour][current.vertex][current.cp_index]];
            //Mark the incoming ray (and all start rays its entire ray group) as DONE
            incoming.valid = false;
            for (auto i = StartRays.begin(); i!=StartRays.end(); i++) {
                const Ray &r = AllRays[RaysByContour[i->contour][i->vertex][i->cp_index]];
                if (r.valid && r.xy.test_equal(incoming.xy) && r.angle.IsSimilar(incoming.angle))
                    r.valid = false;
            }
            switch (incoming.switch_action) {
            case Ray::ERROR:
                _ASSERT(0);
                result.clear();
                return;
            case Ray::IGNORE:
                //for ignore outgoing ray is really a continuation of incoming ray on the same edge
                //ensure this and also that next element is outgoing
                _ASSERT(AllRays[RaysByContour[current.contour][current.vertex][current.cp_index+1]].pos == incoming.pos);
                _ASSERT(!AllRays[RaysByContour[current.contour][current.vertex][current.cp_index+1]].incoming);
                current.cp_index++;
                break;
            case Ray::SWITCH:
                //Now switch to outgoing ray
                const Ray &outgoing = AllRays[RaysByCP[incoming.xy][incoming.switch_to.first][incoming.switch_to.second]];
                current = RayToPointer(outgoing);
                //Append a point
                Edge edge(Contours[outgoing.contour]->at(outgoing.vertex));
                edge.SetStartStrict(outgoing.xy, outgoing.pos, true);
                result.AppendDuringWalk(edge);
                break;
            }
        } else { //we are at a vertex
            result.AppendDuringWalk(Contours[current.contour]->at(current.vertex));
            //Here the edge we indicate is an "outgoing" ray,
            //in the sense that we are at pos==0 at the beginning of the edge
            //so we do not modify current here
        }
        //Now find the next cp and corresponding incoming ray
        StepOutgoing2Incoming(current);
    } while (current.cp_index<0 ||
             AllRays[RaysByContour[current.contour][current.vertex][current.cp_index]].seq_num != sn_finish);
}

bool ContourHelper::Combine(Contour::combine_t type, ContourList &surfaces, ContourList &holes)
{
    if (FindCrosspoints())
        Process4Combine(type); // Process each cp and determine if it is relevant to us or not
    if (GetStartRayNo()==0)
        return false; //two contours do not touch, determine their relative position

    while (GetStartRayNo()) {
        ContourWithHoles walk; //static: we keep allocated memory between calls for performance
        Walk4Combine(walk);
		//If we have abandoned this polyline, do not perform the processing below: start with another polyline
        if (!walk.PostWalk()) continue;
        _ASSERT(walk.IsSane());
        if (walk.CalculateClockwise())
            surfaces.append(walk);
        else
            holes.append(walk.CreateInverse());
    }
    return true;
}


//Walk around the contours starting from the last startray, which is an incoming ray
//At each incoming ray we see if the (clockwise) previous ray (which is not yet done) is outgoing.
//if so go there, if not, start a walk there recursively. Collect the results in surfaces and holes.
//In this walk, we ignore the switch_action and seq_num stored in rays (no such are actually set, when this is called
//Used for XOR and Untangle
void ContourHelper::Walk4Untangle(RayPointer current, ContourList &surfaces, ContourList &holes)
{
    _ASSERT(AllRays[RaysByContour[current.contour][current.vertex][current.cp_index]].valid);

    //do a walk from the current crosspoint, until we get back here
    const XY finish = AllRays[RaysByContour[current.contour][current.vertex][current.cp_index]].xy;
    ContourWithHoles walk;
    const Ray *incoming = &AllRays[RaysByContour[current.contour][current.vertex][current.cp_index]];
    _ASSERT(incoming->valid);
    //"incoming" points to the same ray as "current". If "current" points to a vertex 
    //"incoming" is NULL.
    do {
        if (current.cp_index<0) { //we are at a vertex
            walk.AppendDuringWalk(Contours[current.contour]->at(current.vertex));
            incoming = StepOutgoing2Incoming(current);
            if (incoming) {
                _ASSERT(incoming->valid);
                incoming->valid = false;
            }
            continue;
        }
        const RayArray &CP = RaysByCP[incoming->xy];
        //search previous ray around the crosspoint, which is still valid
        std::pair<unsigned, unsigned> rai = FindRayInCP(CP, incoming);
        if (!FindSuitableOutgoingRayInCP(CP, rai)) {
            //not found a ray different from the one we arrived on -- should not happen
            _ASSERT(0);
            return;
        }
        const Ray *outgoing = &AllRays[CP[rai.first][rai.second]];
        _ASSERT(outgoing->valid);
        outgoing->valid = false;
        //Switch to outgoing ray;
        current = RayToPointer(*outgoing);
        //Append point
        Edge edge(Contours[outgoing->contour]->at(outgoing->vertex));
        edge.SetStartStrict(outgoing->xy, outgoing->pos, true);
        walk.AppendDuringWalk(edge);
        //Go to next cp or vertex on the edge
        incoming = StepOutgoing2Incoming(current); //now current points either to a vertex or to an incoming ray again
        if (incoming) {
            _ASSERT(incoming->valid);
            incoming->valid = false;
        }
        //We continue if in a vertex or if we have not reached back to the starting crosspoint
    } while (current.cp_index<0 || !incoming->xy.test_equal(finish));
    if (!walk.PostWalk()) return;
    _ASSERT(walk.IsSane());
    if (walk.CalculateClockwise())
        surfaces.append(std::move(walk));
    else
        holes.append(walk.CreateInverse());
}

//returns true if there is something to untangle and results are placed in "surfaces" an "holes"
bool ContourHelper::Untangle(ContourList &surfaces, ContourList &holes)
{
    //We will first find all the crossing points between the two contours
    if (!FindCrosspoints())
        return false;
    //Cycle through all crosspoints and start a walk at each ray not yet touched
    //Cycle through all crosspoints and start a walk at each ray not yet touched
    for (auto i = AllRays.begin(); i!=AllRays.end(); i++)
        if (i->incoming && i->valid)
            Walk4Untangle(RayToPointer(*i), surfaces, holes);
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
Contour::result_t Contour::UnionIntersectXor(const Contour &b, ContourList &result, combine_t type) const
{
    if (size()==0) return b.size() ? A_IS_EMPTY : BOTH_EMPTY;
    if (b.size()==0) return B_IS_EMPTY;
    if (!boundingBox.Overlaps(b.boundingBox)) return APART;

    ContourHelper helper(*this, b);
    ContourList holes;

    if (!helper.Combine(type, result, holes))
        return CheckContainment(b); //two contours do not touch, determine their relative position

    //With union we may have holes, but only one surface
    //With intersect (and substract), we can have multiple surfaces, but no holes
    //...or also, we may have just holes (e.g., substracting a larger area from a smaller one)
            //if (type==COMBINE_UNION && result.size()>1)
            //    result.erase(++result.begin(), result.end()); //TODO: THis is a bad fix!!
    switch (type) {
    case COMBINE_UNION:    _ASSERT(result.size()<=1); break;
    case COMBINE_INTERSECT:_ASSERT(holes.size()==0); break;
    case COMBINE_XOR: break;
    }
    if (holes.size() && result.size())  //if we got only holes, we return empty
        result.begin()->holes.swap(holes);
    return OVERLAP;
}

//Splits the two contours into three parts: an intersect, a "this" minus "b" and a "b" minus "this"
Contour::result_t Contour::Split(const Contour &b, ContourList &intersect, ContourList &left_of_me, ContourList &left_of_b) const
{
    if (size()==0) return b.size() ? A_IS_EMPTY : BOTH_EMPTY;
    if (b.size()==0) return B_IS_EMPTY;
    if (!boundingBox.Overlaps(b.boundingBox)) return APART;

    result_t ret = Intersect(b, intersect);
    if (ret!=OVERLAP) return ret;
    Substract(b, left_of_me);
    b.Substract(*this, left_of_b);
    return OVERLAP;
}



//This is a DAG of contours (holes or srufaces) 
//An edge between A->B means that A fully contains B

struct untangle_node 
{
    ContourWithHoles contour;
    std::list<std::list<untangle_node>::iterator> children;
    std::list<untangle_node>::iterator            parent;
    int coverage;  //shows how many arounds we have (in addition to "parent")
    untangle_node(ContourWithHoles &&p, int c) : contour(std::move(p)), coverage(c) {}
};

class untangle_node_list : public std::list<untangle_node>
{
public:
    void insert_node(ContourWithHoles &&p, int c) {push_back(untangle_node(std::move(p), c));}
    void Convert(Contour::winding_rule_t rule, ContourList &result);
};

void untangle_node_list::Convert(Contour::winding_rule_t rule, ContourList &result) 
{
    //Go through the list pairwise and
    //if <earlier> is fully in <later> move <later> just before <earlier> & continue
    //if <earlier> overlaps <later>, split them into parts and move all parts to the end
    bool changed;
    do {
        changed = false;
        for (auto i1 = begin(); i1!=end(); /*nope*/) {
            for (auto i2 = ++iterator(i1); i2!=end(); /*nope*/) {
                ContourList intersect, left_of_i1, left_of_i2;
                switch (i1->contour.Split(i2->contour, intersect, left_of_i1, left_of_i2)) {
                case Contour::OVERLAP:
                    for (ContourList::iterator j = intersect.begin(); j!=intersect.end(); j++)
                        insert_node(std::move(*j), i1->coverage + i2->coverage);
                    for (ContourList::iterator j = left_of_i1.begin(); j!=left_of_i1.end(); j++)
                        insert_node(std::move(*j), i1->coverage);
                    for (ContourList::iterator j = left_of_i2.begin(); j!=left_of_i2.end(); j++)
                        insert_node(std::move(*j), i2->coverage);
                    erase(i2);
                    erase(i1++);
                    changed=true;
                    goto next;
                case Contour::B_INSIDE_A:
                    splice(++iterator(i1), *this, i2++);
                    changed=true;
                    continue;
                default:
                    i2++;
                }
            }
            i1++;
            next:   ;
        }
    } while(changed);

    //Now we have partial ordering all contained nodes are later than their parents
    //and there are no overlapping ones (except for full containment)
    result.clear();
    ContourList Dummy;
    for (auto i1=begin(); i1!=end(); i1++) {
        //search for a parent
        auto i2 = i1;
        while (i2 != begin()) {
            i2--;
            if (i1->contour.Intersect(i2->contour, Dummy) == Contour::B_INSIDE_A) {
                i1->coverage += i2->coverage;
                break;
            }
        }
        bool include;
        switch (rule) {
        case Contour::WINDING_NONZERO:     include = i1->coverage!=0; break;
        case Contour::WINDING_EVENODD:     include = i1->coverage%2==1; break;
        case Contour::WINDING_NONNEGATIVE: include = i1->coverage>=0; break;
        }
        if (include)
            result += i1->contour;
        else 
            result -= i1->contour;
    }
}

//
//struct node {
//    bool ishole;
//    Contour contour;
//    node_list inside_me;
//    node(Contour &&p, bool hole) : ishole(hole), contour(p)  {}
//    node(node &&n) : ishole(n.ishole), contour(std::move(n.contour)), inside_me(std::move(n.inside_me)) {}
//};
//
//void node_list::insert_tree(Contour &&p, bool hole)
//{
//    node n(std::move(p), hole);
//    iterator in_what;
//    bool was = false;
//    for (auto i = begin(); i!=end(); /*none*/) {
//        ContourList res;
//        switch (n.contour.Intersect(i->contour, res)) {
//        default:
//        case Contour::SAME: _ASSERT(0);
//        case Contour::A_INSIDE_B:
//            _ASSERT(!was);
//            in_what = i;
//            was = true;
//            i=end(); //skip the rest: nothing can be in us, if we are in someone
//            break;
//        case Contour::APART:
//            i++;
//            break;
//        case Contour::B_INSIDE_A:
//            if (i==begin()) {
//                n.inside_me.splice(n.inside_me.end(), *this, i);
//                i = begin();
//            } else {
//                n.inside_me.splice(n.inside_me.end(), *this, i--);
//                i++;
//            }
//        }
//    if (was) in_what->inside_me.insert_tree(std::move(n.contour), hole);
//    else push_back(std::move(n));
//}



//Checks if the contour touches itself and splits it into multiple pieces, using the winding rule
//Any point on the plane is part of the result, if a ray from it to infinity statisfies this rule:
//"If the contour crosses the ray from left-to-right, counts +1; if from right to left, counts -1.
//(Left and right are determined from the perspective of looking along the ray from the starting point.)
//If the total count is non-zero, the point will be inside the result."
//Can return
//- A_IS_EMPTY, if we are empty  (nothing is added to surfaces or holes)
//- SAME, if we are already untangled (nothing is added to surfaces or holes)
//- OVERLAP if there were intersections (or is counterclockwise) and they are placed in result
Contour::result_t Contour::Untangle(ContourList &result, winding_rule_t rule) const
{
    Contour tmp(*this);
    const bool original = tmp.Sanitize();
    if (tmp.size()==0)
        return OVERLAP; //an empty "result" is the untangled version
    ContourHelper helper(tmp);
    ContourList holes;

    if (!helper.Untangle(result, holes)) {
        if (original) {
            if (CalculateClockwise()) return SAME;
            //no crosspoints, but counterclockwise
            //in case of WINDING_POSITIVE, we ignore holes: we return an empty result.
            if (rule != WINDING_NONNEGATIVE)
                result.append(CreateInverse());
            return OVERLAP;
        } else {
            if (tmp.CalculateClockwise())
                result.append(std::move(tmp));
            else if (rule != WINDING_NONNEGATIVE)
                result.append(tmp.CreateInverse());
            /*Note in case you add code here: tmp was destroyed by "move" above!*/
            return OVERLAP;
        }
    }

    //Now place resulting contours into each other properly
    untangle_node_list nl;
	for (auto i = result.begin(); i!=result.end(); i++)
        nl.insert_node(std::move(*i), +1);
	for (auto i = holes.begin(); i!=holes.end(); i++)
        nl.insert_node(std::move(*i), -1);
    result.clear();
	//Now nl_ok contains a lot of untangled surfaces and holes neatly arranged
	nl.Convert(rule, result);
    return OVERLAP;
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
