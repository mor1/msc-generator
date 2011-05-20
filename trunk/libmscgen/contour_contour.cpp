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

class CPsByContour;
struct CPOnEdge;

//A "ray" is an edge segment incoming or outgoing from a crosspoint
//This struct identifies a ray
//by giving the edge and the pos on the edge and wether it is incoming or outgoing
struct Ray
{
    CPsByContour &bycont;
    int vertex;     //the number of the vertex staring the edge the crosspoint is on
    double pos;     //-1 if it is a vertex and not a crosspoint, [0..1) the pos of the crosspoint on the edge
    bool incoming;  //true if it is incoming
    double fangle;  //the "fake angle" value compared to a ray pointing up (assuming y grows down)
    double cangle;  //0 if a straight edge, -1 or +1 if a curve with this tangent but curving in pos or neg dir.
    //values used during any walk (both untangle and combine)
    mutable bool valid;
    //playground below for combine
    mutable int seq_num; //same seq num as where we started a walk indicates a stopping criteria
    mutable std::multiset<Ray>::iterator switch_to;
    mutable enum switch_action_t {IGNORE, DROP, ERROR, SWITCH} switch_action;
    mutable bool can_start_here;
    //CPXY() {}
    Ray(const XY &point, CPsByContour &st, int v, double p, bool i);
    bool operator <(const Ray &o) const {return test_equal(fangle, o.fangle) ? test_smaller(cangle, o.cangle) : test_smaller(fangle, o.fangle);}
    bool operator==(const Ray &o) const {return test_equal(fangle, o.fangle) && test_equal(cangle, o.cangle);}
};

class CPRays : public std::multiset<Ray>
{
public:
    bool Add(const XY &point, CPsByContour &st, int v, double p);
    void inc(iterator &i) const {if (++i == end()) i = begin();}
    void dec(iterator &i) const {if (i == begin()) i = end(); --i;}
    iterator get_prev(iterator i) const {if (i == begin()) i = end(); return --i;}

    iterator next_diff(iterator i, int &counted, int &coverage) const;
    bool     search_min_coverage(iterator  &from, iterator  &to, int &counted, int &coverage, const int min_coverage, bool goes_below) const;
    bool     search_coverage_change(iterator  &from, iterator  &to, int &counted, int &coverage) const;
    iterator search_lowest_coverage(iterator loc, int &coverage) const;

    //Process a crosspoint for various operations
    void Process4Combine(int &seq_num, int coverage, int min_coverage) const;
};

struct CPXY
{
    XY xy;
    mutable int coverage_at_0_angle;
    CPXY(const XY&p) : xy(p), coverage_at_0_angle(0) {}
    bool operator < (const CPXY &o) const {return xy.test_smaller(o.xy);}
};

//This class contains crosspoints sorted by their coordinates
//First we will find all crosspoints between the contours and bycont them here
//Then we use Sort() to sort their rays based on angle
class CPSet : public std::map<CPXY, CPRays>
{
protected:
    int num_of_polygons;
    CPsByContour *byconts;
    void Add(const XY &xy, CPsByContour &bycont1, int v1, double p1,
                           CPsByContour &bycont2, int v2, double p2);
public:
    CPSet() : num_of_polygons(0), byconts(NULL) {}
    unsigned FindCrosspoints(CPsByContour bycont[], unsigned num);
    void Process4Combine(bool doUnion, std::vector<CPOnEdge> &startpoints) const;
    void Process4Untangle() const;
    void clear() {std::map<CPXY, CPRays>::clear(); byconts=NULL; num_of_polygons=0;}
};


//This points to the incoming ray associated with the edge this cp is on
struct CPOnEdge
{
    CPSet::const_iterator  iRays;
    CPRays::const_iterator iRay;
    CPOnEdge(CPSet::const_iterator i1, CPRays::const_iterator i2) : iRays(i1), iRay(i2) {};
    Edge GetOutGoingEdgeRemainder() const;
};

struct CPPos
{
    int vertex;
    double pos;
    CPPos(int v, double p) : vertex(v), pos(p) {}
    bool operator<(const CPPos &o) const {return vertex==o.vertex ? pos<o.pos : vertex<o.vertex;}
};

//For each contour we store crosspoint pointers to outgoing rays
class CPsByContour : public std::map<CPPos, CPOnEdge>
{
public:
    const Contour *contour;

    CPsByContour() : contour(NULL) {}
    void Set(const Contour *p) {clear(); contour=p;}
    void Finish() {clear(); contour=NULL;}
    void Add(const CPOnEdge &cp) {_ASSERT(contour); _ASSERT(&cp.iRay->bycont == this); insert(std::pair<const CPPos, CPOnEdge>(CPPos(cp.iRay->vertex, cp.iRay->pos), cp));}
};


/////////////////CPSet implementation

Ray::Ray(const XY &point, CPsByContour &st, int v, double p, bool i) :
    bycont(st), vertex(v), pos(p), incoming(i), valid(true), switch_action(ERROR), can_start_here(false)
{
    XY xy = incoming ? bycont.contour->PrevTangentPoint(vertex, pos) : bycont.contour->NextTangentPoint(vertex, pos);
    fangle = angle(point, XY(point.x, -100), xy);
    if (test_equal(fangle, 4)) fangle = 0;
    const Edge &e = (incoming && pos == 0) ? bycont.contour->at_prev(vertex) : bycont.contour->at(vertex);
    if (e.IsStraight())
        cangle = 0;
    else switch (triangle_dir(point, xy, e.GetEllipseData().GetCenter())) {
        case CLOCKWISE:
            cangle = +1; break;   //TODO do it right: smaller circles should get larger value!!!
        case COUNTERCLOCKWISE:
            cangle = -1; break;
        default:
            _ASSERT(0);
    }
}

//returns if this contour covers a hypothetical ray with angle 0
//it happens if the incoming angle is smaller than the outgoing
bool CPRays::Add(const XY &point, CPsByContour &st, int v, double p)
{
    iterator i = insert(Ray(point, st, v, p, true));
    iterator o = insert(Ray(point, st, v, p, false));
    return *i < *o;
}


void CPSet::Add(const XY &xy, CPsByContour &bycont1, int v1, double p1,
                                         CPsByContour &bycont2, int v2, double p2)
{
    //We add 4 eays for each crosspoint, unless these rays are already added
    CPXY cp_ray(xy);
    auto i = find(cp_ray);
    if (i==end()) {
        CPRays a;
        if (a.Add(xy, bycont1, v1, p1)) cp_ray.coverage_at_0_angle++;
        if (a.Add(xy, bycont2, v2, p2)) cp_ray.coverage_at_0_angle++;
        operator[](cp_ray) = std::move(a);
    } else {
        //insert the two rays for the edge only if that edge has not yet been inserted
        bool one = false, two = false;
        for (auto j = i->second.begin(); j != i->second.end(); j++)
            if (j->vertex == v1 && j->pos == p1 && &j->bycont == &bycont1) one = true;
            else if (j->vertex == v2 && j->pos == p2 && &j->bycont == &bycont2) two = true;
        if (!one)
            if (i->second.Add(xy, bycont1, v1, p1))
                i->first.coverage_at_0_angle++;
        if (!two)
            if (i->second.Add(xy, bycont2, v2, p2))
                i->first.coverage_at_0_angle++;
    }
}

//We assume all elements of bycont are dimensioned equal to its contour
//Here we find all points where the two or more contours touch
//Eventually we shall be able to develop something more efficient than the O(N^2) we have here
//like Bentley-Ottoman
//if num == 1, we search for the crosspoints of the contour which are not vertices
//returns the number of crosspoints found
unsigned CPSet::FindCrosspoints(CPsByContour bycont[], unsigned num)
{
    clear();
    if (num<1) return num_of_polygons = 0;
    num_of_polygons = num;
    byconts = bycont;
    XY r[4];
    double one_pos[4], two_pos[4];
    unsigned ret=0;
    if (num==1) {
        const Contour &poly = *bycont[0].contour;
        for (unsigned i = 1; i<bycont[0].size(); i++)
            for (unsigned j = 0; j<i; j++) {
                unsigned n=Edge::Crossing(poly[i], poly.at_next(i).GetStart(),
                                     poly[j], poly.at_next(j).GetStart(), r, one_pos, two_pos);
                for (unsigned k=0; k<n;k++)
                    Add(r[k], bycont[0], i, one_pos[k], bycont[0], j, two_pos[k]);
                ret += n;
            }
    } else {
        for (unsigned g = 0; g<num-1; g++)
            for (unsigned h = g+1; h<num; h++)
                for (unsigned i = 0; i<bycont[g].contour->size(); i++)
                    for (unsigned j = 0; j<bycont[h].contour->size(); j++) {
                        unsigned n=Edge::Crossing(bycont[g].contour->at(i), bycont[g].contour->at_next(i).GetStart(),
                                             bycont[h].contour->at(j), bycont[h].contour->at_next(j).GetStart(), r, one_pos, two_pos);
                        for (unsigned k=0; k<n;k++)
                            Add(r[k], bycont[g], i, one_pos[k], bycont[h], j, two_pos[k]);
                        ret += n;
                    }
    }
    return ret;
}


//Returns the iterator of the next ray which has a substantially different angle
//assumes coverage is set to the value just before i when called
//and sets "coverage" to the coverage just before the returned index when returns
//if counted reaches size() we exit
CPRays::iterator CPRays::next_diff(iterator i, int &counted, int &coverage) const
{
    iterator j = i;
    if (counted>=int(size())) return end();
    do {
        if (i->incoming) --coverage;
        else ++coverage;
        counted++;
        inc(i);
    } while (counted<int(size()) && *i == *j);
    return counted<=int(size()) ? i : end();
}

//searches for a set of rays where angles are virtually equal and their collective actions
//push the coverage above or below min_coverage
//starts the search at ray "from" assuming the coverage just before "from" is "coverage"
//the set of rays is returned in [from,to), "coverage" is set to coverage just before "to"
//from->to means in the circular sense: from may actually be > than to
//if from==to returned than at "from" coverage is already below/above min_coverage
//if false is returned we did not find any place
bool CPRays::search_min_coverage(iterator &from, iterator &to, int &counted, int &coverage, const int min_coverage, bool goes_below) const
{
    to = from;
    while (goes_below == (coverage>=min_coverage)) {
        from = to;
        to = next_diff(from, counted, coverage);
        if (to==end()) return  false;
    }
    return true;
}

//searches for a set of rays where angles are virtually equal and their collective actions
//change coverage. So if we find a set of rays in the same dir, but the number of incoming
//rays == number of outgoing we go on.
bool CPRays::search_coverage_change(iterator &from, iterator &to, int &counted, int &coverage) const
{
    const int intial_coverage = coverage;
    to = from;
    do {
        from = to;
        to = next_diff(from, counted, coverage);
        if (to==end()) return false;
    } while (intial_coverage == coverage);
    return true;
}

//searches for a set of rays where angles are virtually equal and their collective actions
//change coverage to the lowest among after these groups
//returns the iterator just before which coverage is changed to the lowest
CPRays::iterator CPRays::search_lowest_coverage(iterator loc, int &coverage) const
{
    int counted = 0;
    iterator min_loc = begin();
    int min_coverage = coverage;
    do {
        loc = next_diff(loc, counted, coverage);
        if (loc==end()) break;
        if (min_coverage>coverage) {
            min_loc = loc;
            min_coverage = coverage;
        }
    } while (1);
    coverage = min_coverage;
    return min_loc;
}

//Here we completely mark all rays: what to do during walk.
//During walk we do not make any further decisions, we merely follow
//instructions here
//if switch_action is IGNORE we compare seq_num to see if we need to stop
//if switch_action is ERROR we drop the collected path and start another one
//if swicth_action is SWITCH we change to the outgoing ray pointed by switch_to
//In addition we set can_start_here, which will be ignored during walk, but such rays will
//be collected by CorssPointStore::Process4Combine as startpoints for walks
void CPRays::Process4Combine(int &seq_num, int coverage, int min_coverage) const
{
    //For regular unions or intersects we split the radian to ranges of coverage
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
    //                                 back here again on 2(in)). Note that (incoming(3) will have switch_to==-1
    //                                indicating that if we come this way, we shall drop points we collected so far.
    //                              For intersects only 3(out)->3(in) range will be had.
    // An x(out)->y(in) range selected will result in y(in) be designated a cp switching to edge x.

    iterator start_from = begin();
    iterator A_from;
    int counted = 0;
    //Walk to first ray after the one establishing below minimum coverage after angle 0
    if (!search_min_coverage(start_from, A_from, counted, coverage, min_coverage, true))
        return; //never happens -> this is a crosspoint not needed, all switch_action will remain ERROR

    //Cylce through sections of good coverage
    counted = 0;
    do {
        //First find, the beginning of the range with sufficient coverage
        iterator A_to;
        if (!search_min_coverage(A_from, A_to, counted, coverage, min_coverage, false))
            break; //no more ranges above minimum coverage
        iterator B_from = A_to;
        iterator B_to;
        //Now find the end of the range
        if (!search_min_coverage(B_from, B_to, counted, coverage, min_coverage, true)) {
            _ASSERT(0); //we must find one
        }

        //Now we have a set of rays (A) [A_from, A_to) pointing to the same direction collectively making coverage
        //to go from below the minimum up to at least the minimum.
        //Another set of rays (B) [B_from, B_to) coming after (A) will take the coverage below minimum again.
        //Basically we can say the result contour will have a vertex here with (B) as incoming edge and (A) as outgoing.

        //First We set all incoming cp's sequence number to the same seq number. So that if the Walk started at this cp
        //if we hit this cp again, we could stop the walk, based on equal sequence numbers
        //we need to do it as a do-while since A_from can be == B_to, in which case we need to mark all rays
        //also, they may wrap, so incrementing shall be modulo rays.size()
        iterator ii=A_from;
        do {
            ii->seq_num = seq_num;
            inc(ii);
        } while (ii!=B_to);
        ++seq_num;

        //Now set the "switch_to" in all the incoming edges in (B) to one of the outgoing edges in (A)
        //If the same edge can be found among the outgoing ones, then mark switch_action with IGNORE
        //IGNORE will result in an invalid cp, meaning it will not be added to any polyline (but will be checked for stop condition)
        //We mark only one of the incoming edges as a crosspoint we can start at
        //Find an outgoing
        iterator an_outgoing;
        for (an_outgoing = A_from; an_outgoing!=A_to; inc(an_outgoing))
            if (!an_outgoing->incoming) break;
        _ASSERT(an_outgoing!=A_to);

        bool did_start = false;
        for (iterator i=B_from; i!=B_to; inc(i))
            if (i->incoming) {
                //search for this incoming edge among the outgoing ones
                iterator j=A_from;
                for (j=A_from; j!=A_to; inc(j))
                    if (&j->bycont==&i->bycont && j->vertex==i->vertex && !j->incoming) break;
                if (j!=A_to)         //the outgoing ray corresponding to this incoming ray is in (A)
                    i->switch_action = Ray::IGNORE;
                else {               //not found, just pick an arbitrary outgoing edge
                    i->switch_to = an_outgoing;
                    i->switch_action = Ray::SWITCH;
                    if (!did_start) {
                        i->can_start_here = true;
                        did_start = true;
                    }
                }
            }
            //if we did not insert a cp that we can start at, change all IGNORE values to DROP,
            //so they get dropped entirely and not even inserted into CrosspointStore
            if (!did_start)
                for (iterator i=B_from; i!=B_to; inc(i))
                    if (i->incoming && i->switch_action == Ray::IGNORE)
                        i->switch_action = Ray::DROP;

        //start a new cycle after (B)
        A_from = B_to;
    } while(1);
}

//Copy the processed crosspoints from the set to the bycont, where they are sorted by <contour,edge,pos>.
//(each contour has its own bycont).
//Fill in startpoints with incoming rays marked as can_start_here
void CPSet::Process4Combine(bool doUnion, std::vector<CPOnEdge> &startpoints) const
{
    _ASSERT(byconts);
    startpoints.clear();
    int seq_num = 0;

    //Cylce through the crosspoints
    for (auto iCP = begin(); iCP!=end(); iCP++) {
        //If union, pick the angle ranges covered by at least one contour
        //If intersect, pick angle ranges covered by all of the contours
        iCP->second.Process4Combine(seq_num, iCP->first.coverage_at_0_angle,
                                    doUnion ? 1 : num_of_polygons);

        //Now we have all the rays in the current cp marked as to which one to switch to
        //Add elements to crosspointbyconts and to the startpoints array
        for (auto i=iCP->second.begin(); i!=iCP->second.end(); i++)
            if (i->incoming) {
                switch (i->switch_action) {
                case Ray::SWITCH: //add cp and fill in the edge&pos to switch to
                    if (i->can_start_here)
                        startpoints.push_back(CPOnEdge(iCP, i));
                    /*fallthrough*/
                case Ray::IGNORE: //ignore this cp on a walk: we continue on the edge we came. But check if walk has to stop here
                case Ray::ERROR: //Indicate that this edge was a dead end
                    i->bycont.Add(CPOnEdge(iCP, i));
                case Ray::DROP:
                    break;
                }
            }
    }
}


//Copy the processed crosspoints from the set to the bycont, where they are sorted by <contour,edge,pos>.
//(each contour has its own bycont).
//Fill in startpoints with incoming rays marked as can_start_here
void CPSet::Process4Untangle() const
{
    _ASSERT(byconts);
    //Cylce through the crosspoints
    for (auto iCP = begin(); iCP!=end(); iCP++) {
        //Mark each ray in the cp with the same seq_num
        for (auto i = iCP->second.begin(); i!=iCP->second.end(); i++)  {
            if (i->incoming)
                i->bycont.Add(CPOnEdge(iCP, i));
        }
    }
}



/////////////////CPPointer implementation

//This is a pointer type, pointing to a crosspoint. It has all the attributes to look up
//a next entry on the same contour or to look up the cp in a CPSet for picking
//another outgong ray (to switch to another contour)
//if iCP == end(), it really means that we are at the startpoint of an edge (a vertex) which is
//really before begin(), but this is the only way to express it
struct CPPointer
{
    CPsByContour                *bycont;
    int                          vertex;
    CPsByContour::const_iterator iCP;
    CPPointer(const CPOnEdge &cp);   //construct from an incoming edge
    CPPointer(const Ray &ray); //construct from an outgoing ray
    bool IsAtVertex() const {return iCP==bycont->end();}
    void StepToNext();
    void SwitchTo(CPRays::iterator i);
    Ray::switch_action_t DoSwitchAction();
    Edge GetOutGoingEdgeRemainder() const;
};

CPPointer::CPPointer(const CPOnEdge &cp) : bycont(&cp.iRay->bycont), vertex(cp.iRay->vertex)
{
    iCP = bycont->find(CPPos(cp.iRay->vertex, cp.iRay->pos));
    //if a cp at pos is not found, jump to next vertex
    if (iCP == bycont->end()) {
        _ASSERT(0); // remove this assert
        vertex = (vertex+1)%bycont->contour->size();
        iCP = bycont->end();
    }
}

CPPointer::CPPointer(const Ray &ray) : bycont(&ray.bycont), vertex(ray.vertex)
{
    iCP = bycont->find(CPPos(ray.vertex, ray.pos));
    //if a cp at pos is not found, jump to next vertex
    if (iCP == bycont->end()) {
        _ASSERT(0); //XXX remove this assert
        vertex = (vertex+1)%bycont->contour->size();
        iCP = bycont->end();
    }
}

void CPPointer::StepToNext()
{
    if (IsAtVertex())
        iCP = bycont->lower_bound(CPPos(vertex, 0));
    else
        ++iCP;
    if (iCP==bycont->end() || iCP->second.iRay->vertex != vertex) {
        vertex = (vertex+1)%bycont->contour->size();
        iCP = bycont->end();
    }
}

//Switch to a ray
void CPPointer::SwitchTo(CPRays::iterator i)
{
    bycont = &i->bycont;
    vertex = i->vertex;
    iCP = bycont->lower_bound(CPPos(i->vertex, i->pos));
    //if a cp at pos is not found, jump to next vertex
    if (IsAtVertex()) {
        _ASSERT(0); //XXX remove this assert
        vertex = (vertex+1)%bycont->contour->size();
        iCP = bycont->end();
    }
}


//switches to an outgoing edge, if an action is proscribed in pointed (incoming) ray
//if at vertex, pretend OK, do nothing
//return SWITCH if OK, else the switch action of the pointed ray
Ray::switch_action_t CPPointer::DoSwitchAction()
{
    if (IsAtVertex()) return Ray::SWITCH;
    const Ray &ray = *iCP->second.iRay;
    if (ray.switch_action != Ray::SWITCH) return ray.switch_action;
    SwitchTo(ray.switch_to);
    return Ray::SWITCH;
}


inline Edge CPPointer::GetOutGoingEdgeRemainder() const
{
    //if we are pointing at a vertex, use the corresponding edge
    if (IsAtVertex()) return bycont->contour->GetEdge(vertex);
    //if at a real crosspoint, we return the edge
    const Ray &ray = *iCP->second.iRay;
    if (ray.pos==0) return ray.bycont.contour->GetEdge(ray.vertex);
    return Edge(ray.bycont.contour->GetEdge(ray.vertex)).SetStart(iCP->second.iRays->first.xy, ray.pos);
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
        push_back(Edge(a));
        push_back(Edge(b));
        push_back(Edge(c));
        break;
    case COUNTERCLOCKWISE:
        push_back(Edge(a));
        push_back(Edge(c));
        push_back(Edge(b));
        break;
    }
    boundingBox.MakeInvalid();
    for (unsigned i=0; i<size(); i++)
        boundingBox += at(i).CalculateBoundingBox(at_next(i).start);
}

Contour::Contour(double ax, double ay, double bx, double by, double cx, double cy)
{
    *this = Contour(XY(ax,ay), XY(bx,by), XY(cx,cy));
}

Contour::Contour(const XY &c, double radius_x, double radius_y, double tilt_deg, double s_deg, double d_deg)
{
    if (radius_y==0) radius_y = radius_x;
    Edge edge(c, radius_x, radius_y, tilt_deg, s_deg, d_deg);
    XY end = edge.GetEllipseData().Radian2Point(deg2rad(d_deg));
    boundingBox = edge.CalculateBoundingBox(end);
    push_back(edge);
    if (edge.GetStart().test_equal(end)) return; //full circle
    Edge edge2(end);
    edge2.CalculateBoundingBox(edge.GetStart());
    push_back(edge2);
}

Contour &Contour::operator =(const Block &b)
{
    clear();
    boundingBox = b;
    if (b.IsInvalid()) return *this;
    push_back(Edge((b.UpperLeft())));
    push_back(Edge(XY(b.x.till, b.y.from)));
    push_back(Edge(b.LowerRight()));
    push_back(Edge(XY(b.x.from, b.y.till)));
    for (unsigned i=0; i<size(); i++)
        at(i).CalculateBoundingBox(at_next(i).start);
    return *this;
}

Contour Contour::CreateInverse() const
{
    Contour result;
    result.resize(size());
    for (unsigned i=0; i<size(); i++)
        result[size()-1-i].CopyInverseToMe(at(i), at_next(i).GetStart());
    result.boundingBox = boundingBox;
    return result;
}

//in p* return the number of vertex or edge we have fallen on if result is such
is_within_t Contour::IsWithin(XY p, int *edge, double *pos) const
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
        const int epp = next(e);  //wrap back at the end
        double y[2], po[2];
        bool forward[2];
        switch (at(e).CrossingVertical(p.x, at(epp).GetStart(), y, po, forward)) {
        case 2:
            if (y[1] == p.y) {  //on an edge, we are _not_ approximate here
                //we have tested that at(e) is not equal to p, so no need to test for that here
                if (test_equal(at(epp).GetStart().x, p.x)) {
                    if (edge) *edge = epp;
                    return WI_ON_VERTEX;
                } else {
                    if (pos) *pos = po[1];
                    return WI_ON_EDGE; // on an edge, but not vertex
                }
            }
            if (y[1] > p.y) count ++;
            //fallthrough
        case 1:
            if (y[0] == p.y) {  //on an e
                //we have tested that at(e) is not equal to p, so no need to test for that here
                if (test_equal(at(epp).GetStart().x, p.x)) {
                    if (edge) *edge = epp;
                    return WI_ON_VERTEX;
                } else {
                    if (pos) *pos = po[0];
                    return WI_ON_EDGE; // on an edge, but not vertex
                }
            }
            if (y[0] > p.y) count ++;
            break;
        case -1:
            if ((test_smaller(p.y, at(e).GetStart().y) && test_smaller(at(epp).GetStart().y, p.y)) ||
                (test_smaller(at(e).GetStart().y, p.y) && test_smaller(p.y, at(epp).GetStart().y))) {
                if (pos) *pos = (p.y-at(e).GetStart().y)/(at(epp).GetStart().y-at(e).GetStart().y);
                return WI_ON_EDGE; //goes through p
            }
            //we have tested that at(e) is not equal to p, so no need to test for that here
            if (test_equal(at(epp).GetStart().y, p.y)) {
                if (edge) *edge = epp;
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
        switch (b.IsWithin(p, &edge, &pos)) {
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
            if (really_between04_warp(one_prev, two_next, two_prev) ||
                really_between04_warp(one_next, two_next, two_prev)) return A_INSIDE_B;
            if (really_between04_warp(two_prev, one_next, one_prev) ||
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
    if (size()==1 && other.size()==1 && at(0).ell == other.at(0).ell)
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
            if (at(i).IsStraight()) {
				_ASSERT(!at(i).GetStart().test_equal(at_next(i).GetStart()));
                angles += angle_degrees(angle(at(i).GetStart(), at_next(i).GetStart(), prev));
                prev = at(i).GetStart();
            } else {
                angles += angle_degrees(angle(at(i).GetStart(), at(i).NextTangentPoint(0, at_next(i)), prev));
                prev = at(i).PrevTangentPoint(1, at_prev(i));
                if (at(i).clockwise_arc) {
                    if (at(i).s<at(i).e) angles -= (at(i).e-at(i).s)*(180./M_PI);
                    else                 angles -= 360 - (at(i).s-at(i).e)*(180./M_PI);
                } else {
                    if (at(i).e<at(i).s) angles += (at(i).s-at(i).e)*(180./M_PI);
                    else                 angles += 360 - (at(i).e-at(i).s)*(180./M_PI);
                }
            }
            //angle is (n-2)*180 for clockwise, (n+2)*180 for counterclockwise, we draw the line at n*180
            if (angles/180. - floor(angles/180.)*180. >= 1)
                angles = angles;
        return angles < size()*180;
    }
    if (size()==2) {
        //if a contour is two edges, it should not be two straigth edges
        if (at(0).IsStraight() && at(1).IsStraight()) {
            _ASSERT(0);
        }
        if (!at(0).IsStraight() && !at(1).IsStraight()) {
            //two curves
            //if they are of same direction we get it
            if (at(0).clockwise_arc == at(1).clockwise_arc)
                return at(0).clockwise_arc;
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
        if (at(0).IsStraight())
            return at(1).clockwise_arc;
        else
            return at(0).clockwise_arc;
    }
    //if (size()==1), a full ellipsis
    return at(0).clockwise_arc;
}

//Appends an edge.
//Checks that we do not append a zero long edge
//Checks if the edge to append is a direct continuation of the last edge
void Contour::AppendDuringWalk(const Edge &edge)
{
    //if the last point equals to the startpoint of edge, skip the last edge added
    //except if the last edge is a full circle
    if (size()>0 && edge.GetStart().test_equal(rbegin()->GetStart()))
        if (rbegin()->IsStraight() || !rbegin()->IsFullCircle())
            pop_back();
    //check if current_xy falls on the same line as the previous two
    if (size()>=2 &&
        at(size()-2).CheckAndCombine(at(size()-1), edge.GetStart()))
        at(size()-1) = edge; //if so overwrite last
    else
        push_back(edge);
};


//Walk around the contours starting from startpoint and follow the
//switch_action bycontd for each incoming ray. This is used for union and intersect (and substract)
void Contour::Walk4Combine(const CPOnEdge &startpoint)
{
    clear();
    //current will point to an incoming ray at a crosspoint
    CPPointer current(startpoint);
    if (!current.iCP->second.iRay->valid) //we have already passed over this startpoint
        return;
    //do a walk from the current crosspoint, until we get back here
    const int sn_finish = current.iCP->second.iRay->seq_num;
    do {
        //Mark the incoming ray as DONE
        if (!current.IsAtVertex())
            current.iCP->second.iRay->valid = false;
        //Switch to the other contour, to get an outgoing ray
        Ray::switch_action_t a = current.DoSwitchAction();
        if (a==Ray::SWITCH)
            AppendDuringWalk(current.GetOutGoingEdgeRemainder()); //Add current point (plus edge out)
        else if (a==Ray::ERROR) {
            //We have working from an invalid crosspoint (e.g., in the interior of a union)
            clear();
            return;
        }
        //a can be IGNORE, then we do nothing
        current.StepToNext(); //now current points either to a vertex or to an incoming ray again
    } while (current.IsAtVertex() || current.iCP->second.iRay->seq_num != sn_finish);
}

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
    //if the crosspoint we started at was also a vertex, it may be that it is repeated at the end
    if (size()>1 && at(0).GetStart().test_equal(at(size()-1).GetStart()))
        pop_back();
    //Also, the beginning point (at(0]) can fall on an edge
    //if both the last and the first edge are straight and are in-line
    //such as when two side-by-side rectangles are merged
    if (size()>2) {
        if (at(size()-2).CheckAndCombine(at(size()-1), at(0).GetStart()))
            pop_back();
        if (at(size()-1).CheckAndCombine(at(0), at(1).GetStart()))
            erase(begin());
    }
    //also two semi-circles combined should give a single edge (a circle)
    if (size()==2 && !at(0).IsStraight() && !at(1).IsStraight() &&
        at(0).CheckAndCombine(at(1), at(1).GetStart()))  //second param is a dummy
        pop_back();

    //Sanity checks
    if (size()>0 && at(0).IsStraight())
        if (size()==1 || (size()==2 && at(1).IsStraight()))
            clear();
    if (size()==0) return false;

    //go around and set "e" value for curvy edges. This was not known previously
    //after that compute the bounding box
    if (size()==1) {
        at(0).SetEllipseToFull();  //we checked above it is curvy. We assert that this is a full circle
		boundingBox = at(0).CalculateBoundingBox(at(0).GetStart());
    } else {
        for (unsigned i=0; i<size(); i++) {
            if (at(i).IsStraight()) {
                at(i).CalculateBoundingBox(at_next(i).GetStart());
                boundingBox += at(i).start;
            } else {
                at(i).SetEnd(at_next(i).GetStart(), true);
                at(i).CalculateBoundingBox(at_next(i).GetStart());
                boundingBox += at(i).GetBoundingBox();
            }
        }
    }
    return true;
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
Contour::result_t Contour::UnionIntersect(const Contour &b, ContourList &result, bool doUnion) const
{
    if (size()==0) return b.size() ? A_IS_EMPTY : BOTH_EMPTY;
    if (b.size()==0) return B_IS_EMPTY;
    if (!boundingBox.Overlaps(b.boundingBox)) return APART;

    //Set up some data structures
    CPsByContour cp[2];
    CPSet cp_set;
    std::vector<CPOnEdge> startpoints;
    cp[0].Set(this);
    cp[1].Set(&b);

    //We will first find all the crossing points between the two contours
    cp_set.FindCrosspoints(cp, 2);
    if (cp_set.size())
        cp_set.Process4Combine(doUnion, startpoints); // Process each cp and determine if it is relevant to us or not
    if (startpoints.size()==0) {
        cp[0].Finish();
        cp[1].Finish(); //keep memory allocated, but erase pointers to input contours
        return CheckContainment(b); //two contours do not touch, determine their relative position
    }
    ContourList holes;
    for (auto iStart = startpoints.begin(); iStart != startpoints.end(); iStart++) {
        if (!iStart->iRay->valid) continue;
        static ContourWithHoles walk; //static: we keep allocated memory between calls for performance
        walk.Walk4Combine(*iStart);
		//If we have abandoned this polyline, do not perform the processing below: start with another polyline
        if (!walk.PostWalk()) continue;
        if (walk.CalculateClockwise())
            result.append(walk);
        else
            holes.append(walk.CreateInverse());
    }
    //With union we may have holes, but only one surface
    //With intersect (and substract), we can have multiple surfaces, but no holes
    //...or also, we may have just holes (e.g., substracting a larger area from a smaller one)
    if (doUnion && result.size()>1)
        result.erase(++result.begin(), result.end()); //TODO: THis is a bad fix!!
    if (doUnion) {
        _ASSERT(result.size()<=1);
    } else {
        _ASSERT(holes.size()==0);
    }
    if (holes.size() && result.size())  //if we got only holes, we return empty
        result.begin()->holes.swap(holes);
    cp[0].Finish();
    cp[1].Finish();
    return OVERLAP;
}


//////////////////////////////////Contour::Untangle (and Xor) implementation

//Walk around the contours starting from startpoint, which is an incoming ray
//At each incoming ray we see if the (clockwise) previous ray (which is not yet done) is outgoing.
//if so go there, if not, start a walk there recursively. Collect the results in surfaces and holes.
//In this walk, we ignore the switch_action and seq_num stored in rays (no such are actually set, when this is called
//Used for XOR and Untangle
void Contour::Walk4Untangle(const CPOnEdge &startpoint, ContourList &surfaces, ContourList &holes)
{
	//current will point to an incoming ray at a crosspoint
	CPPointer current(startpoint);
    if (!current.iCP->second.iRay->valid) //we have already passed over this startpoint
		return;
    //do a walk from the current crosspoint, until we get back here
    const CPSet::const_iterator finish = current.iCP->second.iRays;
    ContourWithHoles walk;
    do {
        if (current.IsAtVertex()) {
            walk.AppendDuringWalk(current.GetOutGoingEdgeRemainder()); //Add current point (plus edge out)
            current.StepToNext();
            continue;
        }
        CPRays::iterator i = current.iCP->second.iRay;
        _ASSERT(i->valid);
        do {
            //search previous ray, which is still valid
            do {
                current.iCP->second.iRays->second.dec(i);
            } while (!i->valid);
            _ASSERT(i!=current.iCP->second.iRay); //found a ray different from the one we arrived on.
            if (!i->incoming) break;
            //incoming ray: start a new walk recursively
            Walk4Untangle(CPOnEdge(current.iCP->second.iRays, i), surfaces, holes);
            //if our initial incoming ray is still valid, try it again
            if (current.iCP->second.iRay->valid)
                continue;
            //If the walk above also touched the current incoming ray,
            //we give up and generate no contour from this walk (apart from the
            //ones geenrated by the walk above). THis can only happen when
            //we have not yet collected any vertices and walk.size()==0.
            //(Since any outgoing ray leading to our incoming one in "current"
            //would be invalidated if we had colleced any vertices and no other
            //walk could then reach "current" to invalidate it)
            _ASSERT(walk.size()==0);
            return;
        } while (1);
        //Mark the incoming and outgoing ray as DONE
        current.iCP->second.iRay->valid = false;
        i->valid = false;
        //Switch to outgoing ray;
        current.SwitchTo(i);
        //Append point
        walk.AppendDuringWalk(current.GetOutGoingEdgeRemainder()); //Add current point (plus edge out)
        //Go to next cp or vertex on the edge
        current.StepToNext(); //now current points either to a vertex or to an incoming ray again
    } while (current.IsAtVertex() || current.iCP->second.iRays != finish);
    if (!walk.PostWalk()) return;
    if (walk.CalculateClockwise())
        surfaces.append(std::move(walk));
    else
        holes.append(walk.CreateInverse());
}

//This is a tree of contours (holes or srufaces) that do not intersect, but may contain each other
struct node;
class node_list : public std::list<node>
{
public:
    void insert_tree(Contour &&p, bool hole);
    void Convert(bool hole, int counter, Contour::untangle_t rule, ContourList &result);
};

struct node {
    bool ishole;
    Contour contour;
    node_list inside_me;
    node(Contour &&p, bool hole) : ishole(hole), contour(p)  {}
    node(node &&n) : ishole(n.ishole), contour(std::move(n.contour)), inside_me(std::move(n.inside_me)) {}
};

void node_list::insert_tree(Contour &&p, bool hole)
{
    node n(std::move(p), hole);
    iterator in_what;
    bool was = false;
    for (auto i = begin(); i!=end(); /*none*/)
        switch (n.contour.CheckContainment(i->contour)) {
        default:
        case Contour::SAME: _ASSERT(0);
        case Contour::A_INSIDE_B:
            _ASSERT(!was);
            in_what = i;
            was = true;
            i=end(); //skip the rest: nothing can be in us, if we are in someone
            break;
        case Contour::APART:
            i++;
            break;
        case Contour::B_INSIDE_A:
            if (i==begin()) {
                n.inside_me.splice(n.inside_me.end(), *this, i);
                i = begin();
            } else {
                n.inside_me.splice(n.inside_me.end(), *this, i--);
                i++;
            }
        }
    if (was) in_what->inside_me.insert_tree(std::move(n.contour), hole);
    else push_back(std::move(n));
}


//This is a set of surfaces with surfaces contaied within
//counter counts the paths around us.
//for winding rules, positive surfaces increment it, negative surfaces decrement it, if nonzero, we include
//for evenodd rules, any path increments it, if odd, we include
//for expand, positive surfaces increment it, negative surfaces decrement it, if one, we include
//We also do a technical trick. Hole contains if we are inside a surface and we shall list holes.
//if so, we simply surfaces we need to include based on the criterion above (but process their children).
//And vice versa for holes=false
void node_list::Convert(bool hole, int counter, Contour::untangle_t rule, ContourList &result)
{
    for (auto i=begin(); i!=end(); i++) {
        int new_counter;
        bool include;
        //for winding and expand we decrease if it is a hole
        if (rule != Contour::EVENODD_RULE && i->ishole) new_counter = counter-1;
        else new_counter = counter+1;
        switch (rule) {
        case Contour::WINDING_RULE: include = new_counter!=0; break;
        case Contour::XOR_RULE:
        case Contour::EVENODD_RULE: include = new_counter%2==1; break;
        case Contour::EXPAND_RULE:  include = new_counter>=1; break;
        }
        if (hole != include) {
            //Either we collect holes and shall not include this part->we shall include a hole
            //or we collect surfaces and we shall include this part->we shall include a surface
            ContourWithHoles p(std::move(i->contour));
            i->inside_me.Convert(!hole, new_counter, rule, p.holes);
            result.append(std::move(p));
        } else {
            //Either we collect holes and shall include this part->we shall not include this, maybe its children
            //or we collect surfaces and we shall not include this part->we shall not include this, maybe its children
            i->inside_me.Convert(hole, new_counter, rule, result);
        }
    }
}



//Return values
//OVERLAP: the two contours actually intersect or contain one another
//surfaces contains return values only if the result is OVERLAP.
//A_IS_EMPTY, B_IS_EMPTY, BOTH_EMPTY: they mean just that.
//A_INSIDE_B: *this is fully inside b. (ignoring clockwiseness).
//B_INSIDE_A: b is fully inside *this. (ignoring clockwiseness).
//SAME: The two contours are actually the same (ignoring clockwiseness).
//APART: The two contours are apart. (ignoring clockwiseness).
Contour::result_t Contour::DoXor(const Contour &b, ContourList &result) const
{
    if (size()==0) return b.size() ? A_IS_EMPTY : BOTH_EMPTY;
    if (b.size()==0) return B_IS_EMPTY;
    if (!boundingBox.Overlaps(b.boundingBox)) return APART;


    //Set up some data structures
    /*static*/ CPsByContour cp[2];
    /*static*/ CPSet cp_set;
    /*static*/ std::vector<CPOnEdge> startpoints;
    cp[0].Set(this);
    cp[1].Set(&b);

    //We will first find all the crossing points between the two contours
    cp_set.FindCrosspoints(cp, 2);
    if (!cp_set.size()) {
        cp[0].Finish();
        cp[1].Finish();
        return CheckContainment(b);
    }
    cp_set.Process4Untangle(); // copy cps to edges in bycont
    /*static*/ ContourList holes;
    //Cycle through all crosspoints and start a walk at each ray not yet touched
    for (auto ii = cp_set.begin(); ii!=cp_set.end(); ii++)
        for (auto i = ii->second.begin(); i!=ii->second.end(); i++)
            if (i->incoming && i->valid)
                Walk4Untangle(CPOnEdge(ii,i), result, holes);

    node_list nl;
	for (auto i = result.begin(); i!=result.end(); i++)
        nl.insert_tree(std::move(*i), false);
	for (auto i = holes.begin(); i!=holes.end(); i++)
        nl.insert_tree(std::move(*i), true);
    result.clear();
	nl.Convert(false, 0, XOR_RULE, result);
    return OVERLAP;
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
Contour::result_t Contour::Untangle(ContourList &result, untangle_t rule) const
{
    if (size()==0) return A_IS_EMPTY;

    //Set up some data structures,
    static CPsByContour cp;
	CPSet cp_set;
    std::vector<CPOnEdge> startpoints;

    cp.Set(this);

    //We will first find all the crossing points between our edges
    //neighbouring edges will not be bycontd since one of them will have pos==1
    cp_set.FindCrosspoints(&cp, 1);
    if (!cp_set.size()) {
        cp.Finish();
        if (CalculateClockwise())
            return SAME;
        //no crosspoints, but counterclockwise
        if (rule != EXPAND_RULE)
            result.append(CreateInverse());
		return OVERLAP;
    }
    cp_set.Process4Untangle();  //action==UNION will be ignored
    /*static*/ ContourList holes;
    //Cycle through all crosspoints and start a walk at each ray not yet touched
    for (auto ii = cp_set.begin(); ii!=cp_set.end(); ii++)
        for (auto i = ii->second.begin(); i!=ii->second.end(); i++)
            if (i->incoming && i->valid)
                Walk4Untangle(CPOnEdge(ii,i), result, holes);
    cp.Finish(); //keep memory allocated, but erase pointers to input contours

    node_list nl;
	for (auto i = result.begin(); i!=result.end(); i++)
        nl.insert_tree(std::move(*i), false);
	for (auto i = holes.begin(); i!=holes.end(); i++)
        nl.insert_tree(std::move(*i), true);
    result.clear();
	//Now nl_ok contains a lot of untangled surfaces and holes neatly arranged
	nl.Convert(false, 0, rule, result);
    return OVERLAP;
}


//////////////////////////////////Contour::Rotate implementation


const Block &Contour::CalculateBoundingBox()
{
    boundingBox.MakeInvalid();
    for(unsigned i=0; i<size(); i++)
        boundingBox += at(i).CalculateBoundingBox(at_next(i).start);
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
        //set the last edge to straight
        ret[size()-1] = Edge(ret[size()-1].GetStart());
        //see if we need to replace the last edge, drop it
        if (edge.GetStart().test_equal(ret[size()-1].GetStart()))
            ret.pop_back();
    }
    //insert edge
    ret.push_back(edge);
    int num_to_check = 1;
    //if edge is curvy and does not end in at(0).start, insert a straight edge afterwards
    if (!edge.IsStraight()) {
        XY end = edge.GetEllipseData().Radian2Point(edge.e);
        if (!end.test_equal(ret[0].GetStart())) {
            ret.push_back(Edge(end));
            //check if this edge to insert do not cross the previously inserted edge
            if (Edge::Crossing(ret[ret.size()-2], ret[ret.size()-1].GetStart(), ret[ret.size()-1], ret[0].GetStart(), dum1, dum2, dum2))
                return false;
            num_to_check = 2;
        }
    }
    //now check if inserted edges cross any of the ones before
    //check if edge->end is crossing any existing edges
    ret.CalculateBoundingBox(); //needed by crossing
    for (unsigned i = 0; i<ret.size()-num_to_check-1; i++)
        for (unsigned j = ret.size()-num_to_check-1; j<ret.size(); j++)
            if (Edge::Crossing(ret[i], ret.at_next(i).GetStart(), ret[j], ret.at_next(j).GetStart(), dum1, dum2, dum2))
                return false;
    //OK, we can have these edges inserted
    swap(ret);
    return true;
}

bool OpenHere(const XY &)
{
    _ASSERT(0); //Add Openhere to Contour
    return false;
}


void Contour::Rotate(double cos, double sin, double radian)
{
    boundingBox.MakeInvalid();
    XY next = at(0).start;
    next.Rotate(cos, sin);  //keep the rotated version of next vertex for arc bounding box calculation
    for (int i=size()-1; i>=0; i--) {
        at(i).Rotate(cos, sin, radian, next);
        if (at(i).IsStraight())
            boundingBox += at(i).start;
        else
            boundingBox += at(i).GetBoundingBox();
        next = at(i).start;
    }
}

void Contour::RotateAround(const XY&c, double cos, double sin, double radian)
{
    boundingBox.MakeInvalid();
    XY next = at(0).start;
    next.RotateAround(c, cos, sin);
    for (int i=size()-1; i>=0; i--) {
        at(i).RotateAround(c, cos, sin, radian, next);
        if (at(i).IsStraight())
            boundingBox += at(i).start;
        else
            boundingBox += at(i).GetBoundingBox();
        next = at(i).start;
    }
}

//////////////////////////////////Contour::Expand implementation

typedef struct {
    bool valid;
    Edge s;         //two endpoint of original edge expanded by gap
    XY   e;
    int substitue;  //if orig_s->e needs to be removed, try this; -1 if none
    bool separate;  //true if it is a circle that became detached (used only if valid==false)
    Edge res;
    unsigned next;  //holds the index of edge with which res is a crosspoint (for optimization)
} EdgeXY;

int next_edgexy(const std::vector<EdgeXY> &edges, int i)
{
    int j = i;
    do {
        j = (j+1)%edges.size();
    } while (!edges[j].valid && j!=i);
    return edges[j].valid ? j : -1;
}

int prev_edgexy(const std::vector<EdgeXY> &edges, int i)
{
    int j = i;
    do {
        j = (j-1+edges.size())%edges.size();
    } while (!edges[j].valid && j!=i);
    return edges[j].valid ? j : -1;
}

//TODO Fix Expand to deal with expanding counterclockwise curvy edges
void Contour::Expand(double gap, ContourList &res) const
{
    if (gap==0) {
        res.append(*this);
        return;
    }
    if (size()==0 || boundingBox.x.Spans() < -2*gap || boundingBox.y.Spans() < -2*gap)
        return;

    if (size()==1) {
        _ASSERT(!at(0).IsStraight());
        Contour r = *this;
        if (r[0].ell.Expand(gap))
            res.append(std::move(r));
        return;
    }

    static std::vector<EdgeXY> edges;
    edges.resize(size());

    //calculate 2 points for each angle (expand the vertices)
    int current_size = 0, num_separate = 0;
    for (unsigned i=0; i<size(); i++) {
        edges[i].valid = at(i).ExpandEdge(gap, at_next(i).start, edges[i].s, edges[i].e);
        edges[i].substitue = -1;
        edges[i].next = -1;
        if (edges[i].valid) current_size++;
    }
    if (current_size==0)
        return; //empty - all edges were removed by expand (e.g., circles with too small radius and gap<0)

    bool once_more;
    //Now remove edges that got reversed as long as there are none such
    //From now on edges[].s and edges[].e will not change. The new crosspoints will be in edges[].res
    do {
        once_more = false;
        //calculate the new vertices
        int prev = prev_edgexy(edges, 0);
        for (unsigned i=0; i<size(); i++) {
            if (!edges[i].valid) continue;
            if (edges[prev].next == i) {
                prev = i;
                continue; //no need to re-calculate again
            }
            switch (edges[prev].s.CombineExpandedEdges(edges[prev].e, edges[i].s, edges[i].e, edges[i].res, edges[prev].res)) {
            case 1:  //Expanded edges cross at a single point: the normal case
                edges[prev].next = i;
                edges[i].substitue = -1;  //no substitute, that is only if edges[prev] and edges[i] are parallel
                prev = i;
                continue;
            case -2: //We combine two segments of the same ellipse
                //Drop the second and modify the endpoint of the first
                edges[i].valid = false;
                edges[i].separate = false;
                edges[prev].substitue = -1;
                edges[prev].s.e = edges[i].s.e;
                //keep prev as is
                current_size--;
                once_more = true;
                continue;
            case -1: //We combine two parallel edges.
                //This can be if we removed an edge from between them.
                //We need to keep only one of them, but yet we do not know which.
                //Since edges[prev] is already there, we skip adding edges[i], but
                //mark edges[prev] with i as a potential substitute.
                edges[prev].substitue = i;
                edges[i].valid = false;
                edges[i].separate = false;
                //keep prev as is
                current_size--;
                once_more = true;
                continue;
            case 0: //A circle not matching: either edges[prev] and/or edges[i] are curvy and they do not cross
                //if edges[i] is curvy, mark it, keep prev unchanged and continue
                int n;
                if (!edges[i].s.IsStraight())
                    n = i;
                else { //edges[i] is line: remove edges[prev] and re-do it from there
                    n = prev;
                    prev = prev_edgexy(edges, prev); //what was the last valid node before that
                }
                edges[n].valid = false;
                edges[n].separate = edges[n].s.GetSpan()>M_PI;
                if (edges[n].separate)
                    num_separate++;
                current_size--;
                once_more = true;
                continue;
            }
            _ASSERT(0); //should not flow here
        }
        if (current_size<=1) break; //all edges gone (but there may be separate)
        //remove edges that changed direction compared to original
        int i = next_edgexy(edges,size()-1);
        _ASSERT(i>=0);
        while(1) {
            int after = next_edgexy(edges, i);
            //detect if there have been any changes here
            const int situ = edges[i].res.IsOppositeDir(edges[after].res.start, edges[i].s, edges[i].e);
            if (situ>0) {
                //edge changed direction: we shall skip it
                if (edges[i].substitue == -1) { //no substitute exists: remove this edge
                    edges[i].valid = false;
                    edges[i].separate = situ==2;  //original edge spanned > 180, keep it as separate
                    current_size--;
                } else {
                    //there is a substitute, leave these
                    edges[edges[i].substitue].valid = true;
                    edges[i].valid = false;
                    edges[i].separate = situ==2;  //original edge spanned > 180, keep it as separate
                    //There were no valid edges between i and edges[i].substitute
                    //the problem now is that the next valid edge after i (hence after substitute)
                    //has its .res set to the crosspoint with i and not with substitute, so we need to
                    //skip that edge for now
                    if (after <= i) break;  //we are done
                    after = next_edgexy(edges, after);
                    //Even so the crosspoint with the edge previous to i is still not OK, so we need
                    //to do another pass
                }
                once_more = true;
            }
            if (after <= i) break;
            i = after;
        }
    } while (once_more && current_size>1);

    //copy resulting contour to res
    if (current_size) {
        ContourWithHoles result;
        int i = next_edgexy(edges,size()-1);
        while(1) {
            result.push_back(edges[i].res);
            int next = next_edgexy(edges, i);
            if (next <= i) break;
            i = next;
        }
        if (result.size()>1 || !result[0].IsStraight()) {
            if (result.size()==1)
                result[0].SetEllipseToFull();
            result.CalculateBoundingBox();  //also calculates bounding boxes of edges
            if (SAME==result.Untangle(res, EXPAND_RULE))
                res.append(result);        //expanded contour is not tangled, just add it
        }
    }
    if (num_separate)
        for (unsigned i=0; i<size(); i++) {
            if (edges[i].valid || !edges[i].separate) continue;
            Contour b;
            b.push_back(edges[i].s);
            b.begin()->SetEllipseToFull();
            res += b;
        }
}

ContourList Contour::CreateExpand(double gap) const
{
    ContourList cl;
    Expand(gap, cl);
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
            at(i).Path(cr, at_prev(i).GetStart(), true);
    else
        for (unsigned i = 0; i<size(); i++)
            at(i).Path(cr, at_next(i).GetStart()); //TODO: for circles???
    cairo_close_path(cr);
}

//Do not close the path or include the last edge (even if curvy)
void Contour::PathOpen(cairo_t *cr) const
{
    if (size()==0 || cr==NULL) return;
    cairo_move_to(cr, at(0).GetStart().x, at(0).GetStart().y);
    for (unsigned i = 0; i<size()-1; i++)
        at(i).Path(cr, at_next(i).GetStart());
}

double Contour::do_offsetbelow(const Contour &below, double &touchpoint) const
{
    double offset = CONTOUR_INFINITY;
    for (unsigned i = 0; i<size(); i++)
        for (unsigned j = 0; j<below.size(); j++)
            if (at(i).boundingBox.x.Overlaps(below.at(j).boundingBox.x)) {
                double off, tp;
                if (at(i).IsStraight()) {
                    if (below.at(j).IsStraight())
                        off = Edge::offsetbelow_straight_straight(at(i).start, at_next(i).start,
                                                below.at(j).start, below.at_next(j).start, tp);
                    else
                        off = below.at(j).offsetbelow_curvy_straight(at(i).start,at_next(i).start, true, tp);
                } else {
                    if (below.at(j).IsStraight())
                        off = at(i).offsetbelow_curvy_straight(below.at(j).start, below.at_next(j).start, false, tp);
                    else
                        off = at(i).offsetbelow_curvy_curvy(below.at(j), tp);
                }
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
        const int num = at(i).CrossingVertical(x, at_next(i).GetStart(), y, pos, forward);
        for (int j = 0; j<num; j++)
            section.Set(y[j], forward[j] == add);
    }
}




