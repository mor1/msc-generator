/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010 Zoltan Turanyi
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
#include <stack>
#include "geometry_xarea.h"

namespace geometry {

/////////////////MscCrossPointStore implementation

//cp means crosspoint from here
//a struct that identifies a crosspoint at a polygon by giving the edge and the pos on the edge
struct cp_id_t
{
	MscCrossPointStore *store;
    int vertex;   //the number of the vertex staring the edge the crosspoint is on
    double pos;   //-1 if it is a vertex and not a crosspoint, [0..1) the pos of the crosspoint on the edge
    cp_id_t() {}
    cp_id_t(MscCrossPointStore *s, int a, double b) : store(s), vertex(a), pos(b) {}

	XY PrevTangentPoint() const;
    XY NextTangentPoint() const;
};

//we store structs like this for each crosspoint for walking
struct cp_t
{
    double       pos;
    XY           xy;               //coordinates of the crosspoint
    cp_id_t      switch_to;        //the location of the crosspoint in the other polygon
    bool         can_start_here;   //true if this is a cp we can start a walk at
	int          seq_num;          //used to identify equivalent cps on different edges (for stopping condition of walking)
	mutable bool valid;            //after processing, this is no longer valid

    bool operator <(const cp_t &o) const {return pos<o.pos;}
};

inline bool operator <(const cp_t &cp, double p) {return cp.pos < p;}
inline bool operator <(double p, const cp_t &cp) {return p < cp.pos;}

//for each edge we will have this list: list of crosspoints (eventually) ordered by increasing pos
struct cp_list_t : public std::vector<cp_t>
{
	const cp_t *find_at(double pos) const;
	const cp_t *find_after(double pos, int sn) const;
};

inline const cp_t *cp_list_t::find_at(double pos) const
{
	const_iterator i = lower_bound(begin(), end(), pos); 
	return i==end() ? NULL : &*i;
}

inline const cp_t *cp_list_t::find_after(double pos, int sn) const
{
	for (auto i = upper_bound(begin(), end(), pos); i!=end(); i++)
		if (i->seq_num == sn)
			return &*i;
		else if (i->valid && i->switch_to.vertex != -2) 
			return &*i;
	return NULL;
}

//A "pointer" to a crosspoint. We also store which store it is at and which vertes, s owe can
//easily calculate the next pone
struct p_cp_t 
{
	MscCrossPointStore *store;
	int vertex;
	const cp_t *cp;
	p_cp_t(MscCrossPointStore *s, int v, const cp_t *c) : store(s), vertex(v), cp(c) {}
	Edge GetOutGoingEdgeRemainder() const;  //formulate an edge starting in pCP (and open-ended)         
};

//we store the points where the other polygon crosses an edge in a fashion that we need to switch to that edge
//edges correspond to the elements in the vector and the crosspoints are sorted there
//according to their distance from the starting vertex of the edge
//we store a crosspoint even if it is equal to the starting vertex of an edge, if we need to switch there to another polygon
//but not if it is the end => then it is stored at the beginning of the next edge
//at any given position on the edge there can be only ine crosspoint here. This list where we have to switch to.
class MscCrossPointStore : protected std::vector<cp_list_t>
{
protected:
    const Polygon *_polygon;
public:
    MscCrossPointStore() : _polygon(NULL) {}
	const Polygon &GetPolygon() const {_ASSERT(_polygon); return *_polygon;}
	void Set(const Polygon &p);
    void Sort();
    void Finish();

    const cp_t* Add(const XY& xy, int vertex, double pos, const cp_id_t &switch_to, int sn);
	const cp_t* Find(int vertex, double pos) const {return at(vertex).find_at(pos);}
	p_cp_t  After(int vertex, double pos, const int sn); //next crosspoint strictly after the pos designated here
};

inline XY cp_id_t::PrevTangentPoint() const 
{
	return store->GetPolygon().PrevTangentPoint(vertex, pos);
}

inline XY cp_id_t::NextTangentPoint() const 
{
	return store->GetPolygon().NextTangentPoint(vertex, pos);
}

inline Edge p_cp_t::GetOutGoingEdgeRemainder() const 
{
	//if we are pointing at a vertex, use the corresponding edge
	if (cp==NULL) return store->GetPolygon().GetEdge(vertex);
	//if cp!=NULL, we return the edge from the other polygon
	_ASSERT(cp->switch_to.vertex!=-2);
	//if we will stop anyway, just return a dummy value 
	if (cp->switch_to.vertex<0) return Edge();
	//if the other edge's pos is zero, we return all of the edge
	if (cp->switch_to.pos==0) return cp->switch_to.store->GetPolygon().GetEdge(cp->switch_to.vertex);
	return Edge(cp->switch_to.store->GetPolygon().GetEdge(cp->switch_to.vertex)).SetStart(cp->xy, cp->switch_to.pos);
}

void MscCrossPointStore::Set(const Polygon &p)
{
    _ASSERT(_polygon==NULL);
    _polygon = &p;
    if (size() < p.size()) resize(p.size());
    for(int i=0; i<_polygon->size(); i++)
        at(i).clear();
}

void MscCrossPointStore::Sort()
{
    if (_polygon == NULL) return;
    for(int i=0; i<_polygon->size(); i++)
        if (at(i).size())
            std::sort(at(i).begin(), at(i).end());
}

inline void MscCrossPointStore::Finish()
{
    _polygon = NULL;
}

//if switch_to.vertex == -2 a not valid cp should be added, just for being able to terminate based on sequence number
//if switch_to.vertex == -1 it means we should not arrive on this vertex to this CP: Walk() will drop all points gathered so far
//other values indicate which other polygon+edge to switch to
const cp_t* MscCrossPointStore::Add(const XY& xy, int vertex, double pos, const cp_id_t &switch_to, int sn)
{
    _ASSERT(size() >= _polygon->size());
    at(vertex).resize(at(vertex).size()+1); 
	cp_t *cp = &*at(vertex).rbegin();
	cp->pos = pos;
	cp->xy = xy;
	cp->switch_to = switch_to;
	cp->seq_num = sn;
	cp->valid = switch_to.vertex != -2;
	return cp;
}

//Gets the next relevant point (cp or vertex) strictly after <vertex,pos>
//In the meanwhile it checks if any cp between (including) <vertex,pos> and the returned value has sn as seq_num
//if so, it returns a -1 vertex and NULL cp_t
p_cp_t MscCrossPointStore::After(int vertex, double pos, const int sn) 
{
	const cp_t *cp = at(vertex).find_after(pos, sn);
	if (cp) {
		if (cp->seq_num == sn) return p_cp_t(this, -1, NULL);
		return p_cp_t(this, vertex, cp);
	}
	vertex = GetPolygon().next(vertex);
	if (at(vertex).size() && at(vertex).begin()->pos == 0) {
		//the vertex is a crosspoint: return the crosspoint
		if (at(vertex).begin()->seq_num == sn) return p_cp_t(this, -1, NULL);
		if (at(vertex).begin()->switch_to.vertex==-2) return p_cp_t(this, vertex, NULL);
		return p_cp_t(this, vertex, &*at(vertex).begin());
	}
	//a vertex, which is not a crosspoint
	return p_cp_t(this, vertex, NULL);
}

/////////////////MscCrossPointSet implementation

//This class contains crosspoints, too, as the first step in processing.
//After collecting all crosspoints here, we can use Process() to go through them
//and decide when to switch between which edges. Then we copy them to the respective MscCrossPointStore objects
class MscCrossPointSet : protected std::map<XY, std::vector<cp_id_t>> 
{
protected:
	int num_of_polygons;
	MscCrossPointStore *stores;
	void Add(const XY &xy, MscCrossPointStore *p1, int a1, double b1, MscCrossPointStore *p2, int a2, double b2);
public:
	MscCrossPointSet() : stores(NULL) {}
    int FindCrosspoints(MscCrossPointStore store[], int num);
	void Process(Polygon::poly_action_t action, std::vector<cp_id_t> &startpoints) const;
};

void MscCrossPointSet::Add(const XY &xy, MscCrossPointStore *p1, int a1, double b1, MscCrossPointStore *p2, int a2, double b2)
{
	auto i = find(xy);
	if (i==end()) {
		mapped_type a;
		a.push_back(cp_id_t(p1, a1, b1));
		a.push_back(cp_id_t(p2, a2, b2));
		operator[](xy) = std::move(a);
	} else {
		i->second.push_back(cp_id_t(p1, a1, b1));
		i->second.push_back(cp_id_t(p2, a2, b2));
	}
}

//We assume all elements of store are dimensioned equal to its polygon
//Here we find all points where the two or more polygons touch 
//Eventually we shall be able to develop something more efficient than the O(N^2) we have here
//like Bentley-Ottoman
//if num == 1, we search for the crosspoints of the polygon which are not vertices
//returns the number of crosspoints found
int MscCrossPointSet::FindCrosspoints(MscCrossPointStore store[], int num) 
{
	clear();
	if (num<1) return num_of_polygons = 0;
	num_of_polygons = num;
	stores = store;
    XY r[4];
    double one_pos[4], two_pos[4];
    int ret=0;
	if (num==1) {
		const Polygon &poly = store[0].GetPolygon();
		for (int i = 1; i<poly.size(); i++)
			for (int j = 0; j<i; j++) {
				int n=Edge::Crossing(poly[i], poly.at_next(i).GetStart(),
									 poly[j], poly.at_next(j).GetStart(), r, one_pos, two_pos);
				for (int k=0; k<n;k++)
					Add(r[k], store, i, one_pos[k], store, j, two_pos[k]);
				ret += n;
			}
	} else {
		for (int g = 0; g<num-1; g++)
			for (int h = g+1; h<num; h++) 
				for (int i = 0; i<store[g].GetPolygon().size(); i++)
					for (int j = 0; j<store[h].GetPolygon().size(); j++) {
						int n=Edge::Crossing(store[g].GetPolygon()[i], store[g].GetPolygon().at_next(i).GetStart(),
											 store[h].GetPolygon()[j], store[h].GetPolygon().at_next(j).GetStart(), r, one_pos, two_pos);
						for (int k=0; k<n;k++)
							Add(r[k], store+g, i, one_pos[k], store+h, j, two_pos[k]);
						ret += n;
					}
	}
    return ret;
}

struct ray_t {
	int index;
	bool incoming;
	double angle;
	int switch_to;
	int can_start_here;
	int seq_num;
	ray_t(int i, bool in, double a) : index(i), incoming(in), angle(a), switch_to(-1), can_start_here(0) {}
	bool operator < (const ray_t &o) const {return angle<o.angle;}
};
inline bool ray_less_by_index(const ray_t &a, const ray_t &o) {return a.index<o.index;}

class rays_t : public std::vector<ray_t>
{
public:
    int inc(int &cp) const {return cp = (cp+1)%size();}

	int next_diff(int i, int &counted, int &coverage) const;
	bool search_min_coverage(int &from, int &to, int &counted, int &coverage, const int min_coverage, bool goes_below) const;
	bool search_coverage_change(int &from, int &to, int &counted, int &coverage) const;
	int search_lowest_coverage(int loc, int &coverage) const;

	//Process a crosspoint for various operations
	void ProcessForUntangle(int &seq_num, int coverage);
	void ProcessForCombine(int &seq_num, int coverage, int min_coverage);
};

//Returns the index of the next ray which has a substantially different angle
//assumes coverage is set to the value just before i when called
//and sets "coverage" to the coverage just before the returned index when returns
int rays_t::next_diff(int i, int &counted, int &coverage) const
{
	int j = i;
	if (counted>=size()) return -1;
	do {
		if (at(i).incoming) --coverage;
		else ++coverage;
		counted++;
		inc(i);
	} while (counted<size() && test_equal(at(j).angle, at(i).angle));
	return counted<=size() ? i : -1;
}

//searches for a set of rays where angles are virtually equal and their collective actions
//push the coverage above or below min_coverage
//starts the search at ray "from" assuming the coverage just before "from" is "coverage"
//the set of rays is returned in [from,to), "coverage" is set to coverage just before "to"
//from->to means in the circular sense: from may actually be > than to
//if from==to returned than at "from" coverage is already below/above min_coverage
//if false is returned we did not find any place 
bool rays_t::search_min_coverage(int &from, int &to, int &counted, int &coverage, const int min_coverage, bool goes_below) const
{
	to = from;
	while (goes_below == (coverage>=min_coverage)) {
		from = to;
		to = next_diff(from, counted, coverage);
		if (to==-1) return  false;
	}
	return true;
}

//searches for a set of rays where angles are virtually equal and their collective actions
//change coverage. So if we find a set of rays in the same dir, but the number of incoming
//rays == number of outgoing we go on.
bool rays_t::search_coverage_change(int &from, int &to, int &counted, int &coverage) const
{
	const int intial_coverage = coverage;
	to = from;
	do {
		from = to;
		to = next_diff(from, counted, coverage);
		if (to==-1) return false;
	} while (intial_coverage == coverage);
	return true;
}

//searches for a set of rays where angles are virtually equal and their collective actions
//change coverage to the lowest among after these groups
//returns the index just before which coverage is changed to the lowest
int rays_t::search_lowest_coverage(int loc, int &coverage) const 
{
	int counted = 0;
	int min_loc = 0;
	int min_coverage = coverage;
	do {
		loc = next_diff(loc, counted, coverage);
		if (loc==-1) break;
		if (min_coverage>coverage) {
			min_loc = loc;
			min_coverage = coverage;
		}
	} while (1);
	coverage = min_coverage;
	return min_loc;
}

void rays_t::ProcessForUntangle(int &seq_num, int coverage)
{
	//Here the rule is that we always pair an incoming edge to the outgoing right after it
	//Example: 1\  |2                
	//           \ |              Here incoming(2) will be switched to outgoing(3)
	//            \|                   incoming(1) will be switched to outgoing(1)
	//         3---o---->3             incoming(3) will be switched to outgoing(2) 
	//             |\
	//             | \            The problem is if multiple edges come in/out overlapped.
	//            2V  -->1        Then we force them to be startpoints processed first: this will
	//                            chop off some of the tangled polygon (and we will call untangle)
	//                            until all parts come off.
	// All cps will get the same sequence number, so if we start here, we will stop here no matter
	// what edge do we come in.

	//Walk to the first ray group after the lowest coverage
	int from = search_lowest_coverage(0, coverage);
	std::stack<int> stack;

	//Now cycle around 
	//Every time we find a ray group that increases coverage, we push some outgoing rays to the stack
	//(as many as we have increased coverage, arbitrarily selected)
	//Every time we find a ray group that decreases coverage, we pick elements from the stack
	int counted = 0;
	do {
		int orig_coverage = coverage;
		const int to = next_diff(from, counted, coverage);
		if (to==-1) break;
		if (orig_coverage == coverage) {
			from = to;
			continue;
		}
		if (coverage>orig_coverage) {
			for (int i = from, c = orig_coverage; c<coverage; inc(i))
				if (!at(i).incoming) {
					c++;
					stack.push(i);
				}
			from = to;
			continue;
		}
		//from->to now decreases coverage-> has more incoming rays than outgoing
		//Associate incoming rays with the ones on with the top of the stack
		//and add them to the beginning of the startpoint list. This guarantees 
		//that at least some of the polygons get apart in one UntangleOnePolygon phase.
		//We only process (orig_coverage-coverage) incoming edge in the batch,
		//the rest have matching outgoing edges in this batch.
		for (; from!=to; inc(from))
			if (at(from).incoming) {
				at(from).switch_to = stack.top();
				at(from).can_start_here = -1;
				stack.pop();
				if (++coverage == orig_coverage)
					break;
			}
		from = to;
	} while(1);
	//Now mark all cps here with the same seq num. If for any reason we come back here, we stop.
	for (auto i = begin(); i!=end(); i++)
		i->seq_num = seq_num;
	++seq_num;
}

void rays_t::ProcessForCombine(int &seq_num, int coverage, int min_coverage)
{
	//For regular unions or intersects we split the radian to ranges of coverage
	//for union coverage is sufficient if any one polygon covers an area.
	//for intersects all of them have to cover it.
	//
	//Example:      1\    ->2,,     .=area covered by polygon 1
	//              ..\  /,,,,,     ,=area covered by polygon 2
	//              ...\|,,,,,,     :=area covered by polygon 3 and 2
	//              ....o----->3
	//              .../|\:::::     For unions one (clockwise) range will be 2(outgoing)->2(incoming)
	//              ../ |,\::::       and another is from 1(out)->1(in). The two ranges get different seq_nums.
	//             1<- 2|,,\3::       (So if we started on 2(in) (immeditately switching to 2(out)), we will not
	//                                 stop when coming through 1(in), but will continue on 1(out) until we get
	//                                 back here again on 2(in)). Note that (incoming(3) will have switch_to==-1 
	//                                indicating that if we come this way, we shall drop points we collected so far.
	//                              For intersects only 3(out)->3(in) range will be had.
	// An x(out)->y(in) range selected will result in y(in) be designated a cp switching to edge x.

	int start_from = 0;
	int A_from;
	int counted = 0;
	//Walk to first ray establishing below minimum coverage after angle 0
	if (!search_min_coverage(start_from, A_from, counted, coverage, min_coverage, true))
		return; //never happens -> this is a crosspoint not needed, all switch_to will remain -1

	//Cylce through sections of good coverage
	counted = 0;
	do {
		//First find, the beginning of the range with sufficient coverage
		int A_to;
		if (!search_min_coverage(A_from, A_to, counted, coverage, min_coverage, false))
			break; //no more ranges above minimum coverage
		int B_from = A_to;
		int B_to;
		//Now find the end of the range
		if (!search_min_coverage(B_from, B_to, counted, coverage, min_coverage, true))
			_ASSERT(0); //we must find one

		//Now we have a set of rays (A) [A_from, A_to) pointing to the same direction collectively making coverage
		//to go from below the minimum up to at least the minimum.
		//Another set of rays (B) [B_from, B_to) coming after (A) will take the coverage below minimum again.
		//Basically we can say the result polygon will have a vertex here with (B) as incoming edge and (A) as outgoing.
				
		//First We set all incoming cp's sequence number to the same seq number. So that if the Walk started at this cp
		//if we hit this cp again, we could stop the walk, based on equal sequence numbers
		//we need to do it as a do-while since A_from can be == B_to, in which case we need to mark all rays
		//also, they may wrap, so incrementing shall be modulo rays.size()
		int ii=A_from;
		do {
			at(ii).seq_num = seq_num;
			inc(ii);
		} while (ii!=B_to);
		++seq_num;

		//Now set the "switch_to" in all the incoming edges in (B) to one of the outgoing edges in (A)
		//If the same edge can be found among the outgoing ones, then mark switch_to with -2
		//-2 will result in an invalid cp, meaning it will not be added to any polyline (but will be checked for stop condition)
		//We mark only one of the incoming edges as a crosspoint we can start at
		//Find an outgoing 
		int an_outgoing;
		for (an_outgoing = A_from; an_outgoing!=A_to; inc(an_outgoing))
			if (!at(an_outgoing).incoming) break;
		_ASSERT(an_outgoing!=A_to);

		bool did_start = false;
		for (int i=B_from; i!=B_to; inc(i))
			if (at(i).incoming) {
				//search for this incoming edge among the outgoing ones
				int j=A_from;
				for (j=A_from; j!=A_to; inc(j))
					if (at(j).index==at(i).index && !at(j).incoming) break;
				if (j!=A_to)         //the outgoing ray corresponding to this incoming ray is in (A)
					at(i).switch_to = -2;
				else {               //not found, just pick an arbitrary outgoing edge
					at(i).switch_to = an_outgoing;
					if (!did_start) {
						at(i).can_start_here = 1;
						did_start = true;
					}
				}
			}
		//if we did not insert a cp that we can start at, change all -2 values to -3, so they get dropped entirely
		if (!did_start)
			for (int i=B_from; i!=B_to; inc(i))
				if (at(i).incoming && at(i).switch_to == -2) 
					at(i).switch_to = -3;
				
		//start a new cycle after (B)
		A_from = B_to;
	} while(1);
}


void MscCrossPointSet::Process(Polygon::poly_action_t action, std::vector<cp_id_t> &startpoints) const
{
	//Action is ignored if we have only one polygon
	//Either we do intersect (but then on multiple polygions) or union
	//Union can be done on a single or multiple ones. ona single polygon it equals a winding rule untangle
	_ASSERT((num_of_polygons==1 || action == Polygon::UNION || action == Polygon::INTERSECT));
	_ASSERT(stores);
	startpoints.clear();
	int seq_num = 0;

	for (auto iCP = begin(); iCP!=end(); iCP++) {
		static rays_t rays;
		rays.clear();

		//Sort the incoming and outgoing edges (rays) in clockwise order
		int coverage_at_0 = 0;  //how many polygons cover the ray at angle==0
		for (int i = 0; i<iCP->second.size(); i++) {
			//these angles are not real radians, but they can be sorted properly
			double angle_prev = angle(iCP->first, XY(iCP->first.x, -100), iCP->second[i].PrevTangentPoint());
			double angle_next = angle(iCP->first, XY(iCP->first.x, -100), iCP->second[i].NextTangentPoint());
			if (test_equal(angle_prev, 4)) angle_prev = 0;
			if (test_equal(angle_next, 4)) angle_next = 0;
			if (test_smaller(angle_prev, angle_next))
				coverage_at_0++;
			rays.push_back(ray_t(i, true,  angle_prev));
			rays.push_back(ray_t(i, false, angle_next));
		}
		std::sort(rays.begin(), rays.end());


		//Now fill in switch_to, can_start_here and seq_num in elements of rays
		//We do it differently for untangle and for union/intersect
		if (num_of_polygons == 1) 
			rays.ProcessForUntangle(seq_num, coverage_at_0);
		else 
			//If union, pick the angle ranges covered by at least one polygon
			//If intersect, pick angle ranges covered by all of the polygons
			rays.ProcessForCombine(seq_num, coverage_at_0, action==Polygon::UNION ? 1 : num_of_polygons);

		//Now we have all the rays in the current cp marked as to which one to switch to
		//Add elements to crosspointstores and to the startpoints array
		for (int i=0; i<rays.size(); i++)
			if (rays[i].incoming && rays[i].switch_to!=-3) {
				const cp_id_t &cp = iCP->second[rays[i].index];
				switch (rays[i].switch_to) {
				case -2: //ignore this cp on a walk: we continue on the edge we came. But check if walk has to stop here
					cp.store->Add(iCP->first, cp.vertex, cp.pos, cp_id_t(cp.store, -2, -2), rays[i].seq_num);
					break;
				case -1: //Indicate that this edge was a dead end
					cp.store->Add(iCP->first, cp.vertex, cp.pos, cp_id_t(cp.store, -1, -1), rays[i].seq_num);
					break;
				default: //add cp and fill in the edge&pos to switch to
					const cp_t *p = cp.store->Add(iCP->first, cp.vertex, cp.pos, iCP->second[rays[rays[i].switch_to].index], rays[i].seq_num);
					if (rays[i].can_start_here==1) 
						startpoints.push_back(cp_id_t(cp.store, cp.vertex, cp.pos));
					if (rays[i].can_start_here==-1) 
						startpoints.insert(startpoints.begin(), cp_id_t(cp.store, cp.vertex, cp.pos));
					break;
				}
			}
	}
	//Sort the crosspoints on edges according to their pos in all of the stores
	for (int i=0; i<num_of_polygons; i++) 
		stores[i].Sort();
}


///////////////////////////// Polygon

//Do not create degenerate triangles.
//Always create clockwise.
Polygon::Polygon(XY a, XY b, XY c)
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
	for (int i=0; i<size(); i++)
		boundingBox += at(i).CalculateBoundingBox(at_next(i).start);
}

Polygon::Polygon(double ax, double ay, double bx, double by, double cx, double cy)
{
    *this = Polygon(XY(ax,ay), XY(bx,by), XY(cx,cy));
}

Polygon::Polygon(const XY &c, double radius_x, double radius_y, double tilt_degree)
{
    if (radius_y==0) radius_y = radius_x;
    Edge edge(c, radius_x, radius_y, tilt_degree);
	boundingBox = edge.CalculateBoundingBox(edge.GetStart());
    push_back(edge);
}

Polygon &Polygon::operator =(const Block &b)
{
    clear();
    boundingBox = b;
    if (b.IsInvalid()) return *this;
    push_back(Edge((b.UpperLeft())));
    push_back(Edge(XY(b.x.till, b.y.from)));
    push_back(Edge(b.LowerRight()));
    push_back(Edge(XY(b.x.from, b.y.till)));
	for (int i=0; i<size(); i++)
		at(i).CalculateBoundingBox(at_next(i).start);
    return *this;
}

Polygon Polygon::GetInverse() const
{
    Polygon result;
    result.resize(size());
    for (int i=0; i<size(); i++)
        result[size()-1-i].CopyInverseToMe(at(i), at_next(i).GetStart());
    result.boundingBox = boundingBox;
    return result;
}

//in p* return the number of vertex or edge we have fallen on if result is such
is_within_t Polygon::IsWithin(XY p, int *edge, double *pos) const
{
    if (size()==0 || boundingBox.IsWithin(p)==WI_OUTSIDE) return WI_OUTSIDE;

    //Follow the polygon and see how much it crosses the horizontal line going through p
    //count the crossings to our right
    //http://softsurfer.com/Archive/algorithm_0103/algorithm_0103.htm
    //1. an upward edge includes its starting endpoint, and excludes its final endpoint;
    //2. a downward edge excludes its starting endpoint, and includes its final endpoint;
    //3. horizontal edges are excluded; and
    //4. the edge-ray intersection point must be strictly right of the point P.
    //4b: since we say containment also for edge points, if the egde goes through p, we stop
    int count = 0;
    int e;
    for (e = 0; e<size(); e++) {
        if (edge) *edge = e;      //return value
        const int epp = next(e);  //wrap back at the end
        double x[2], po[2];
        switch (at(e).CrossingHorizontal(p.y, at(epp).GetStart(), x, po)) {
        case 2:
            if (x[1] == p.x) {  //on an edge, we are _not_ approximate here
                if (test_equal(at(e).GetStart().y, p.y)) return WI_ON_VERTEX; //on vertex
                else if (test_equal(at(epp).GetStart().y, p.y)) {
                    if (edge) *edge = epp;
                    return WI_ON_VERTEX;
                } else {
					if (pos) *pos = po[1];
					return WI_ON_EDGE; // on an edge, but not vertex
				}
            }
            if (x[1] > p.x) count ++;
            //fallthrough
        case 1:
            if (x[0] == p.x) {  //on an e
                if (test_equal(at(e).GetStart().y, p.y)) return WI_ON_VERTEX; //on vertex
                else if (test_equal(at(epp).GetStart().y, p.y)) {
                    if (edge) *edge = epp;
                    return WI_ON_VERTEX;
                } else {
					if (pos) *pos = po[0];
					return WI_ON_EDGE; // on an edge, but not vertex
				}
            }
            if (x[0] > p.x) count ++;
            break;
        case -1:
            if ((test_smaller(p.x, at(e).GetStart().x) && test_smaller(at(epp).GetStart().x, p.x)) ||
                (test_smaller(at(e).GetStart().x, p.x) && test_smaller(p.x, at(epp).GetStart().x))) {
				if (pos) *pos = (p.x-at(e).GetStart().x)/(at(epp).GetStart().x-at(e).GetStart().x);
				return WI_ON_EDGE; //goes through p
			}
            if (test_equal(at(e).GetStart().x, p.x)) return WI_ON_VERTEX; //on vertex
            if (test_equal(at(epp).GetStart().x, p.x)) {
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

//true if q is between (a and b) modulo 4
//if q is almost equal to a or b, we return false;
inline bool really_between04_warp (double q, double a, double b)
{
	if (a<b) return test_smaller(q,b) && !test_smaller(q,a);
	return test_smaller(a,q) || test_smaller(q,b);
}

//Can result SAME, APRT, A_INSIDE_B or B_INSIDE_A
//It can also return OVERLAP, which means one of our point is outside b, can either be APART or b may be in us
Polygon::poly_result_t Polygon::CheckContainmentHelper(const Polygon &b) const
{
    int edge;
    poly_result_t retval = A_INSIDE_B;
    for (int i=0; i<size(); i++) {
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


//Gives valid result only if the two polygons have no crosspoints
//clockwiseness fully honored
Polygon::poly_result_t Polygon::CheckContainment(const Polygon &other) const
{
    //special case of two full ellipses beging the same - not caught otherwise
    if (size()==1 && other.size()==1 && at(0).ell == other.at(0).ell)
        return SAME;
    poly_result_t this_in_other = CheckContainmentHelper(other);
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

bool Polygon::CalculateClockwise() const
{
    //determine if this is clockwise.
    if (size()>2) {
        double angles = 0;
        XY prev = at(0).PrevTangentPoint(0, at_prev(0));
        for (int i=0; i<size(); i++)
            if (at(i).IsStraight()) {
				_ASSERT(!(test_equal(at(i).GetStart().x, at_next(i).GetStart().x) && test_equal(at(i).GetStart().y, at_next(i).GetStart().y)));
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
        //if a polygon is two edges, it should not be two straigth edges
        if (at(0).IsStraight() && at(1).IsStraight())
            _ASSERT(0);
        if (!at(0).IsStraight() && !at(1).IsStraight()) {
            //two curves
            //if they are of same direction we get it
            if (at(0).clockwise_arc == at(1).clockwise_arc)
                return at(0).clockwise_arc;
            //two curves with opposite dir, they do not touch only at the two ends
            //the one contains the other decides
            _ASSERT(0); //figure this out;
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

//the two crosspointstores shall be ordered here. They can be equal if we process a single polygon.
void Polygon::Walk(const std::vector<cp_id_t> &startpoints, PolygonList &surfaces, PolygonList &holes)
{
	surfaces.clear();
	holes.clear();
    //Start the walks around the polygon, as long as there are crosspoints left marked with "can_start_here"
	for (auto iStart = startpoints.begin(); iStart != startpoints.end(); iStart++) {
		//find the next still valid crosspoint
		p_cp_t current(iStart->store, iStart->vertex, iStart->store->Find(iStart->vertex, iStart->pos));
		if (!current.cp->valid) //we have already passed over this startpoint
			continue;
        static PolygonWithHoles result; //static: we keep allocated memory between calls for performance
        result.clear();

        //do a walk from the current crosspoint, until we get back here
		const int sn_finish = current.cp->seq_num;
        do {
            //Add current point (plus edge out) to results, but not if we added that last
			Edge out_edge = current.GetOutGoingEdgeRemainder();
			if (result.size()>0 && out_edge.GetStart() == result.rbegin()->GetStart())
                result.pop_back();
            //check if current_xy falls on the same line as the previous two
            if (result.size()>=2 &&
                result[result.size()-2].CheckAndCombine(result[result.size()-1], out_edge.GetStart()))
                result[result.size()-1] = out_edge; //if so overwrite last
            else
                result.push_back(out_edge);

			//Now go to next point
            if (current.cp == NULL) { //this is a vertex not crossed by the other polygon, we keep walking on next edge
				current = current.store->After(current.vertex, 0, sn_finish);
            } else { //this is a crosspoint let's see if we switch to the other polygon
				//invalidate cp 
				current.cp->valid = false;
				if (current.cp->switch_to.vertex == -1) {
					//We have working from an invalid crosspoint (e.g., in the interior of a union)
					result.clear();
					break;
				}
				current = current.cp->switch_to.store->After(current.cp->switch_to.vertex, current.cp->switch_to.pos, sn_finish);
			}
        } while (current.vertex>=0);  //After() returns vertex==-1 if we pass over a cp with seq num == "finish_sn"

		//If we have abandoned this polyline, do not perform the processing below: start with another polyline
		if (result.size()==0) continue;

        //if the crosspoint we started at was also a vertex, it may be that it is repeated at the end
        if (result.size()>1 && result[0].GetStart() == result[result.size()-1].GetStart())
            result.pop_back();
        //Also, the beginning point (result[0]) can fall on an edge
        //if both the last and the first edge are straight and are in-line
        //such as when two side-by-side rectangles are merged
        if (result.size()>2) {
            if (result[result.size()-2].CheckAndCombine(result[result.size()-1], result[0].GetStart()))
                result.pop_back();
            if (result[result.size()-1].CheckAndCombine(result[0], result[1].GetStart()))
                result.erase(result.begin());
        }
        //also two semi-circles combined should give a single edge (a circle)
        if (result.size()==2 && !result[0].IsStraight() && !result[1].IsStraight() &&
            result[0].CheckAndCombine(result[1], result[1].GetStart()))  //second param is a dummy
            result.pop_back();

        //go around and set "e" value for curvy edges. This was not known previously
        //after that compute the bounding box
        if (result.size()==1) {
            result[0].SetEllipseToFull();  //asserts that this is a full circle
			result.boundingBox = result[0].CalculateBoundingBox(result[0].GetStart());
        } else {
            for (int i=0; i<result.size(); i++) {
                if (result[i].IsStraight()) {
                    result[i].CalculateBoundingBox(result.at_next(i).GetStart());
                    result.boundingBox += result[i].start;
                } else {
                    result[i].SetEnd(result.at_next(i).GetStart());
                    result[i].CalculateBoundingBox(result.at_next(i).GetStart());
                    result.boundingBox += result[i].GetBoundingBox();
                }
            }
        }
        if (result.CalculateClockwise())
            surfaces.append(result);
        else
            holes.append(result.GetInverse());
    }//for thru startpoints
}

//Does a union or intersection of two poligons, stores the result in "surfaces"
//- For union "surfaces" will be one bounding polygon, its "holes" may have zero or more elements
//- For intersection "surfaces" may have more elements, their "holes" are guaranteed to be empty
//Return values
//OVERLAP: the two polygons actually intersect (or at least touch for union)
//surfaces contains return values only if the result is OVERLAP.
//A_IS_EMPTY, B_IS_EMPTY, BOTH_EMPTY: they mean just that.
//A_INSIDE_B: *this is fully inside b. (ignoring clockwiseness).
//B_INSIDE_A: b is fully inside *this. (ignoring clockwiseness).
//SAME: The two polygons are actually the same (ignoring clockwiseness).
//APART: The two polygons are apart. (ignoring clockwiseness).
Polygon::poly_result_t Polygon::Combine(const Polygon &b, PolygonList &result, poly_action_t action) const
{
    if (size()==0) return b.size() ? A_IS_EMPTY : BOTH_EMPTY;
    if (b.size()==0) return B_IS_EMPTY;
    if (!boundingBox.Overlaps(b.boundingBox)) return APART;

	if (action==XOR) {
		poly_result_t ret = Combine(b, result, UNION);
		if (ret!=OVERLAP) return ret;
		PolygonList holes;
		if (OVERLAP == Combine(b, holes, INTERSECT))
			result -= holes;
		return OVERLAP;
	}

	//Now action can only be UNION, INTERSECT or SUBSTRACT and none of the polygons is empty
	//Set up some data structures
    static Polygon b_inv;
    /*static*/ MscCrossPointStore cp[2];
    cp[0].Set(*this);
    cp[1].Set(action==SUBSTRACT ? b_inv = b.GetInverse() : b); //invert b if action is SUBSTRACT
	/*static*/ MscCrossPointSet cp_set;

    //We will first find all the crossing points between the two polygons
    Polygon::poly_result_t ret = BOTH_EMPTY;
    if (cp_set.FindCrosspoints(cp, 2)) {
		/*static*/ std::vector<cp_id_t> startpoints;
		// Process each cp and determine if it is relevant to us or not
		cp_set.Process(action==SUBSTRACT ? INTERSECT : action, startpoints);
		if (startpoints.size()) {
			//relevant cps found
			static PolygonList holes;
			Walk(startpoints, result, holes); //they touch, walk the pieces
			//With union we may have holes, but only one surface
			//With intersect (and substract), we can have multiple surfaces, but no holes
			_ASSERT(result.size()==1 || holes.size()==0);
			if (holes.size())
				result.begin()->holes.swap(holes);
			ret = OVERLAP;
		}
    } 
	if (ret != OVERLAP)
        ret = CheckContainment(b); //two polygons do not touch, determine their relative position
    cp[0].Finish();
    cp[1].Finish(); //keep memory allocated, but erase pointers to input polygons
    return ret;
}

//Checks if the polygon touches itself and splits it into multiple pieces, using the winding rule
//Any point on the plane is part of the result, if a ray from it to infinity statisfies this rule:
//"If the polygon crosses the ray from left-to-right, counts +1; if from right to left, counts -1. 
//(Left and right are determined from the perspective of looking along the ray from the starting point.) 
//If the total count is non-zero, the point will be inside the result."
//Can return
//- A_IS_EMPTY, if we are empty
//- SAME, if we are already untangled (nothing is in result)
//- OVERLAP if there were intersections (or is counterclockwise) and they are placed in result
Polygon::poly_result_t Polygon::UntangleOnePolygon(PolygonList &surfaces, PolygonList &holes) const
{
    if (size()==0) return A_IS_EMPTY;

    //Set up some data structures,
    static MscCrossPointStore cp;
    cp.Set(*this); 
	MscCrossPointSet cp_set;

    //We will first find all the crossing points between our edges
    //neighbouring edges will not be stored since one of them will have pos==1
    Polygon::poly_result_t ret = SAME;
    if (cp_set.FindCrosspoints(&cp, 1)) {
		static std::vector<cp_id_t> startpoints;
		cp_set.Process(UNION, startpoints);  //action==UNION will be ignored
		if (startpoints.size()) {
			Walk(startpoints, surfaces, holes); //they touch, walk the pieces
			ret = OVERLAP;
		}
    } 
	if (ret == SAME && !CalculateClockwise()) {//no crosspoints, but counterclockwise
		holes.append(GetInverse());  
		ret = OVERLAP;
    }
    cp.Finish(); //keep memory allocated, but erase pointers to input polygons
    return ret;
}

//This is a tree of polygons (holes or srufaces) that do not intersect, but may contain each other
struct node;
class node_list : public std::list<node>
{
public:
	void insert_tree(Polygon &&p, bool hole);
	void Convert(bool hole, int counter, Polygon::untangle_t rule, PolygonList &result);
};

struct node {
	bool ishole;
	Polygon polygon;
	node_list inside_me;
	node(Polygon &&p, bool hole) : polygon(p), ishole(hole) {}
	node(node &&n) : polygon(std::move(n.polygon)), ishole(n.ishole), inside_me(std::move(n.inside_me)) {}
};

void node_list::insert_tree(Polygon &&p, bool hole)
{
	node n(std::move(p), hole);
	iterator in_what;
	bool was = false;
	for (auto i = begin(); i!=end(); /*none*/)
		switch (n.polygon.CheckContainment(i->polygon)) {
		case Polygon::SAME: _ASSERT(0);
		case Polygon::A_INSIDE_B: 
			_ASSERT(!was);
			in_what = i;
			//i=end(); //skip the rest: nothing can be in us, if we are in someone
			break;
		case Polygon::APART:
			i++;
			break;
		case Polygon::B_INSIDE_A:
			if (i==begin()) {
				n.inside_me.splice(n.inside_me.end(), *this, i);
				i = begin();
			} else {
				n.inside_me.splice(n.inside_me.end(), *this, i--);
				i++;
			}
	}
	node_list *to_be_added_to;
	if (was) in_what->inside_me.insert_tree(std::move(n.polygon), hole);
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
void node_list::Convert(bool hole, int counter, Polygon::untangle_t rule, PolygonList &result)
{
	for (auto i=begin(); i!=end(); i++) {
		int new_counter;
		bool include;
		//for winding and expand we decrease if it is a hole
		if (rule != Polygon::EVENODD_RULE && i->ishole) new_counter = counter-1;
		else new_counter = counter+1;
		switch (rule) {
		case Polygon::WINDING_RULE: include = new_counter!=0; break;
		case Polygon::EVENODD_RULE: include = new_counter%2==1; break;
		case Polygon::EXPAND_RULE:  include = new_counter>=1; break;
		}
		if (hole != include) {
			//Either we collect holes and shall not include this part->we shall include a hole
			//or we collect surfaces and we shall include this part->we shall include a surface
			PolygonWithHoles p(std::move(i->polygon));
			i->inside_me.Convert(!hole, new_counter, rule, p.holes);
			result.append(std::move(p));
		} else {
			//Either we collect holes and shall include this part->we shall not include this, maybe its children
			//or we collect surfaces and we shall not include this part->we shall not include this, maybe its children
			i->inside_me.Convert(hole, new_counter, rule, result);
		}
	}
}

Polygon::poly_result_t Polygon::Untangle(PolygonList &result, untangle_t rule) const
{
	PolygonList s, h;
	//Untangle polygon
	switch (Polygon::UntangleOnePolygon(s, h)) {
	case A_IS_EMPTY:
		return A_IS_EMPTY;
	case SAME:
		return SAME;
	}
	//Call untangle as long as we have fragments not untangled
	node_list nl, nl2, nl_ok;
	for (auto i = s.begin(); i!=s.end(); i++) 
		nl.push_back(node(std::move(*i), false));
	for (auto i = h.begin(); i!=h.end(); i++) 
		nl.push_back(node(std::move(*i), true));
	while (nl.size()) {
		for (auto i = nl.begin(); i!=nl.end(); /*none*/) 
			switch(i->polygon.UntangleOnePolygon(s, h)) {
			case OVERLAP:
				for (auto j = s.begin(); j!=s.end(); j++) 
					nl2.push_back(node(std::move(*j), i->ishole));
				for (auto j = h.begin(); j!=h.end(); j++) 
					nl2.push_back(node(std::move(*j), !i->ishole));
			case A_IS_EMPTY:
				i++;
				break;
			case SAME:
				if (i==nl.begin()) {
					nl_ok.insert_tree(std::move(i->polygon), i->ishole);
					nl.pop_front();
					i = nl.begin();
				} else {
					nl_ok.insert_tree(std::move(i->polygon), i->ishole);
					nl.erase(i--);
					i++;
				}
			}
		nl.swap(nl2);
	}
	//Now nl_ok contains a lot of untangled surfaces and holes neatly arranged
	nl_ok.Convert(false, 0, rule, result);
	return OVERLAP;
}


const Block &Polygon::CalculateBoundingBox()
{
    boundingBox.MakeInvalid();
    for(int i=0; i<size(); i++)
        boundingBox += at(i).CalculateBoundingBox(at_next(i).start);
    return boundingBox;
}


void Polygon::Rotate(double cos, double sin, double radian)
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

void Polygon::RotateAround(const XY&c, double cos, double sin, double radian)
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

typedef struct {
	bool valid;
	Edge s;        //two endpoint of original edge expanded by gap
	XY   e;
	int  substitue;//if orig_s->e needs to be removed, try this
	bool separate; //true if it is a circle that became detached (used only if valid==false)
	Edge res;    
	int  next;     //holds the index of edge with which res is a crosspoint (for optimization)
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


void Polygon::Expand(double gap, PolygonList &res) const
{
    if (gap==0 || size()==0) return;
	if (boundingBox.x.Spans() < -2*gap || boundingBox.y.Spans() < -2*gap) return;

	static std::vector<EdgeXY> edges;
	edges.resize(size());

	//calculate 2 points for each angle (expand the vertices)
	int current_size = 0, num_separate = 0;
	for (int i=0; i<size(); i++) {
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
		for (int i=0; i<size(); i++) {
			if (!edges[i].valid) continue;
			if (edges[prev].next == i) {
				prev = i;
				continue; //no need to re-calculate again
			}
			switch (edges[prev].s.CombineExpandedEdges(edges[prev].e, edges[i].s, edges[i].e, edges[i].res, edges[prev].res)) {
			case 1:
				edges[prev].next = i;
				edges[i].substitue = -1;  //no substitute, that is only if edges[prev] and edges[i] are parallel
				prev = i;
				continue;
			case -2:
				//We combine two segments of the same ellipse
				//Drop the second and modify the endpoint of the first
				edges[i].valid = false;
				edges[i].separate = false;
				edges[prev].substitue = -1;  
				edges[prev].s.e = edges[i].s.e;  
				//keep prev as is 
				current_size--;
				once_more = true;
				continue;
			case -1:
				//the last added edge and this one is parallel. We need to add only one of them, but
				//we do not know which. Since edges[prev] is already there, we skip adding edges[i], but
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
				if (edges[n].separate = edges[n].s.GetSpan()>M_PI)
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

	//copy resulting polygon to res
	if (current_size) {
		PolygonWithHoles result;
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
			//if (SAME==result.Untangle(res, EXPAND_RULE)) 
				res.append(result);        //expanded polygon is not tangled, just add it
		}
	}
	if (num_separate) 
		for (int i=0; i<size(); i++) {
			if (edges[i].valid || !edges[i].separate) continue;
			Polygon b;
			b.push_back(edges[i].s);
			b.begin()->SetEllipseToFull();
			res += b;
		}
}

void Polygon::Path(cairo_t *cr, bool inverse) const
{
    if (size()==0 || cr==NULL) return;
    cairo_move_to(cr, at(0).GetStart().x, at(0).GetStart().y);
    if (inverse)
        for (int i = size()-1; i>=0; i--)
            at(i).Path(cr, at_prev(i).GetStart(), true);
    else
        for (int i = 0; i<size(); i++)
            at(i).Path(cr, at_next(i).GetStart());
    cairo_close_path(cr);
}


}; //namespace


