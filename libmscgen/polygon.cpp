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
#include <list>
#include "polygon.h"
#include "mscdrawer.h"

//returns -1 if a==b
//returns -2 if a==c
//returns -3 if b==c
//returns -4 if a==b==c
//returns 0 if a, b and c are on the same line
//returns 1 if a->b->c is clockwise, 2 otherwise
typedef enum {ALL_EQUAL, A_EQUAL_B, A_EQUAL_C, B_EQUAL_C, IN_LINE, CLOCKWISE, COUNTERCLOCKWISE} triangle_dir_t;
triangle_dir_t triangle_dir(XY a, XY b, XY c) 
{
	if (a == b) return b==c ? ALL_EQUAL : A_EQUAL_B;
	if (a == c) return A_EQUAL_C;
	if (b == c) return B_EQUAL_C;
	//Decide if we divide by x or y coordinates
	if (fabs(a.x - b.x) < fabs(a.y - b.y)) {
		const double m = (a.x - b.x)/(a.y - b.y);
		double cx =  m*(c.y-a.y) + a.x; //(cx, c.y) is a point on the a-b line
		if (cx == c.x) return IN_LINE;
		return (c.x < cx ^ a.y < b.y) ? COUNTERCLOCKWISE : CLOCKWISE;
	} else {
		const double m = (a.y - b.y)/(a.x - b.x);
		double cy =  m*(c.x-a.x) + a.y; //(c.x, cy) is a point on the a-b line
		if (cy == c.y) return IN_LINE;
		return (c.y < cy ^ a.x < b.x) ? CLOCKWISE : COUNTERCLOCKWISE;
	}
}

//returns a value [0..4) that is representative of clockwise angle: bigger angle->bigger value, but not linear
//-1 is error
double angle(XY base, XY A, XY B) 
{
	bool clockwise;
	switch (triangle_dir(base, A, B)) {
	case IN_LINE:
	case CLOCKWISE:
		clockwise = true;
		break;
	case COUNTERCLOCKWISE:
		clockwise = false;
		break;
	case B_EQUAL_C:
		return 0;  //zero degrees
	default:
		return -1; //error one of them equals to another
	};
	double cos = (A-base).DotProduct(B-base) / (A-base).length() / (B-base).length();
	if (clockwise)
		return cos = 1-cos; //gives [0..2]
	else 
		return cos = cos+3; //gives (2..4)
}

inline bool section_overlaps(double ps, double pd, double qs, double qd, double &r1, double &r2)
{
	if (std::min(ps, pd)<std::max(qs,qd) && std::min(qs,qd)<std::max(ps,pd)) {
		r1 = std::max(std::min(ps,pd), std::min(qs,qd));
		r2 = std::min(std::max(ps,pd), std::max(qs,qd));
		return true;
	}
	return false;
}

inline double MM(XY A, XY B)
{
	return (A.y-B.y)/(A.x-B.x);
}

//checks if two sections are crossing or not
//return the number of points:
//0 - no crossing
//1 - one crossing point (in r1)
//2 - the two sections intersects from r1 to r2
//if any of the two sections are degenerate we return 1 only if it lies on the other section, else 0
//See http://softsurfer.com/Archive/algorithm_0104/algorithm_0104B.htm
#define SMALL_NUM 0.00000001 //avoid division overflow
int are_crossing(const XY A, const XY B, const XY M, const XY N, XY&r1, XY &r2) 
{
	if (fabs((B-A).PerpProduct(N-M)) >= SMALL_NUM) {
		//They are not parallel (and none of them are degenerate, but that does not matter now)
		const double t = (B-A).PerpProduct(A-M) / (B-A).PerpProduct(N-M);
		if (t < 0 || t > 1) 
			return 0; //the intersection of the line is outside MN
		const double s = (N-M).PerpProduct(A-M) / (B-A).PerpProduct(N-M);
		if (s < 0 || s > 1) 
			return 0; //the intersection of the line is outside AB
		r1 = M + (N-M)*t;
		return 1;
	}
	//either they are parallel or one or both sections are degenerate (= a point)
	if (M==N) {
		if (fabs((M-A).PerpProduct(B-A)) >= SMALL_NUM) 
			return 0; //M is not on AB's line and AB is not degenerate
		if (A==B && !(A==M)) 
			return 0; //both sections degenerate to a point, but do not overlap
		r1 = M; //M is on AB 
		return 1;
	}
	if (fabs((A-M).PerpProduct(N-M)) >= SMALL_NUM) 
		return 0; //A is not on MN's line
	//They are either parallel and on the same line or AB is degenerate and lies on MN's line
	double t0, t1;
	//select x or y depending on precision
	if (fabs(M.x-N.x) > fabs(M.y-N.y)) {
		t0 = (A.x-M.x)/(N.x-M.x);
		t1 = (B.x-M.x)/(N.x-M.x);
	} else {
		t0 = (A.y-M.y)/(N.y-M.y);
		t1 = (B.y-M.y)/(N.y-M.y);
	}
	if (t0 > t1) std::swap(t0, t1);
	if (t0 > 1 || t1 < 0) 
		return 0; //AB lies outside MN
	if (t0 < 0) t0 = 0;
	if (t1 > 1) t1 = 1;
	r1 = M + (N-M)*t0;
	if (t0 == t1) 
		return 1; //AB is degenerate
	r2 = M + (N-M)*t1;
	return 2;
}

//Do not create degenerate triangles.
//Always create clockwise.
MscPointList::MscPointList(XY a, XY b, XY c)
{
	switch (triangle_dir(a,b,c)) {
	default: //create empty if degenerate triangle
		return;
	case CLOCKWISE:
		push_back(a);
		push_back(b);
		push_back(c);
		break;
	case COUNTERCLOCKWISE:
		push_back(a);
		push_back(c);
		push_back(b);
		break;
	}
}


MscPointList &MscPointList::operator =(const Block &b) {
	clear();
	if (b.IsInvalid()) return *this;
	push_back(b.UpperLeft());
	push_back(XY(b.x.till, b.y.from));
	push_back(b.LowerRight());
	push_back(XY(b.x.from, b.y.till));
	return *this;
}

bool MscPointList::GetBoundingBox(Block &b) const 
{
	b.MakeInvalid();
	if (size()<3) return false;
	for (int i = 0; i<size(); i++) 
		b += at(i);
	return true;
}

void MscPointList::Expand(MscPointList &pointlist, double gap) const
{
	gap = 0; //XXX
	if (gap <= 0) {
		pointlist = *this;
		return;
	}
	pointlist.clear();
	if (size()<3) return;
	for (int i=0; i<size(); i++) {
		const int ipp = (i+1)%size();
		const int imm = (i-1+size())%size();
	//XXX

	}
}

bool MscPointList::Overlaps(const MscPointList &b, double gap) const 
{
	MscPointList pointlist;
	const MscPointList *pPointlist = this;
	if (gap) {
		Expand(pointlist, gap);
		pPointlist = &pointlist;
	}
	XY r1, r2;
	//if any two edges cross, we overlap
	for (int i = 0; i<pPointlist->size(); i++)
		for (int j = 0; j<b.size(); j++)
			if (are_crossing(pPointlist->at(i), pPointlist->at((i+1)%pPointlist->size()),
				             b[j], b[(j+1)%b.size()], r1, r2) > 0)
				return true;
	return false;
}

bool MscPointList::IsWithin(XY p) const
{
	if (size()<3) return false;
	//Follow the polygon and see how much it crosses the horizontal line going through p
	//count the crossings to our right
	//http://softsurfer.com/Archive/algorithm_0103/algorithm_0103.htm
	//1. an upward edge includes its starting endpoint, and excludes its final endpoint;
	//2. a downward edge excludes its starting endpoint, and includes its final endpoint;
	//3. horizontal edges are excluded; and
	//4. the edge-ray intersection point must be strictly right of the point P.
	//4b: since we say containment also for edge points, if the egde goes through p, we stop
	int count = 0;
	for (int i = 0; i<size(); i++) {
		const int ipp = (i+1)%size();  //wrap back at the end
		if ((at(i).y >= p.y && at(ipp).y < p.y) ||
			(at(i).y < p.y && at(ipp).y >= p.y)) {
			//we cross p's line x
			double x = (at(i).x - at(ipp).x)/(at(i).y - at(ipp).y)*(p.y - at(ipp).y) + at(ipp).x;
			if (x == p.x) return true; //on edge
			if (x > p.x) count ++;
		} else if (at(i).y == p.y && at(ipp).y == p.y) //horizontal edge level with p
			if ((at(i).x >= p.x && at(ipp).x <= p.x) ||
				(at(i).x <= p.x && at(ipp).x >= p.x))
				return true; //goes through p
	}
	return count&1; //even is out (false)
}

//Assuming P is between A and B, return where (0 if on A, 1 if on B and values in between)
inline double relative_pos(XY A, XY B, XY P)
{
	//See which coordinate spans more space
	if (fabs(A.x - B.x) < fabs(A.y - B.y)) 
		return (P.y-A.y)/(B.y-A.y);
	else
		return (P.x-A.x)/(B.x-A.x);
}

//Union of two simple polygons
//if the two do not overlap, returns false
bool MscPointList::Union(const MscPointList &b, MscPointList &result) const
{
	if (b.size()<3) {
		result = *this;
		return true;
	}
	if (size()<3) {
		result = b;
		return true;
	}
	//We will first find all the crossing points between the two polygons
	//These will be inserted into me and you below.
	//me[x] will contain the crossing points on the edge between vertex x and x+1 ordered from verted x towards x+1
	//we store a crosspoint even if it is equal to the starting vertex of an edge, but not if it is the end 
	//(that will be stored for the next edge).
	struct crosspoint{
		XY xy;
		int other_vertex;
		double other_pos;
	};
	std::vector<std::map<double, crosspoint>> me(size()), you(b.size());
	XY r1, r2;
	double pos_me, pos_you;
	for (int i = 0; i<size(); i++)
		for (int j = 0; j<b.size(); j++) 
			switch (are_crossing(at(i), at((i+1)%size()), b[j], b[(j+1)%b.size()], r1, r2)) {
			case 0: continue;
			case 2: 
				pos_me  = relative_pos(at(i), at((i+1)%size()), r2);
				pos_you = relative_pos(b[j], b[(j+1)%b.size()], r2);
				//if the crosspoint falls on any ending vertex, skip it, it will pop up for the next edge as the starting vertex
				if (pos_me < 1 && pos_you < 1) {
					me [i][pos_me] .xy = r2;  me [i][pos_me]. other_vertex = j;   me [i][pos_me]. other_pos = pos_you;
					you[j][pos_you].xy = r2;  you[j][pos_you].other_vertex = i;   you[j][pos_you].other_pos = pos_me;
				}
				//fallthrough
			case 1:
				pos_me  = relative_pos(at(i), at((i+1)%size()), r1);
				pos_you = relative_pos(b[j], b[(j+1)%b.size()], r1);
				//if the crosspoint falls on any ending vertex, skip it, it will pop up for the next edge as the starting vertex
				if (pos_me < 1 && pos_you < 1) {
					me [i][pos_me] .xy = r1;  me [i][pos_me]. other_vertex = j;   me [i][pos_me]. other_pos = pos_you;
					you[j][pos_you].xy = r1;  you[j][pos_you].other_vertex = i;   you[j][pos_you].other_pos = pos_me;
				}
			}

	//Start walking around the polygon
	//First select a point in this that is outside the other
	int index = 0;
	while (index < b.size() && IsWithin(b[index]))
		index++;
	//If other is completely within "this", we simply return: the union is "this"
	if (index==b.size()) {
		result = *this;
		return true;
	}
	//Else, we found a point outside of "this", start at that
	struct poly {
		const MscPointList *pl;
		std::vector<std::map<double, crosspoint>> *crosspoints;
	} p[2] = {{this, &me}, {&b, &you}};
	result.clear();

	//find the previous point
	XY previous;
	int prev_index = (index-1+b.size()) % b.size();
	if (you[prev_index].begin() == you[prev_index].end())
		previous = b[prev_index];  //no crosspoints, we pick the previous vertex
	else
		previous = (--you[prev_index].end())->second.xy;  //the last crosspoint on the previous edge

	int walk = 1; //we currently walk "b" and not "this"
	int halt = index; //we started from this index, if we get back here, we are OK
	double pos = -1; //we are at the vertex of index
	bool was_walk_0 = false; //see if we ever walk on "this" (walk==0)
	XY do_not_insert(-1,-1);
	do {
		if (walk == 0) was_walk_0 = true;

		//Add current point to results and calculate next crosspoint (if any)
		std::map<double, crosspoint>::const_iterator cross_i;
		XY inserted; 
		if (pos == -1) {
			//we stand at a vertex
			inserted = p[walk].pl->at(index);
			cross_i = p[walk].crosspoints->at(index).begin();
		} else {
			//we stand at a crosspoint
			cross_i = p[walk].crosspoints->at(index).find(pos);
			assert(cross_i != p[walk].crosspoints->at(index).end());
			inserted = cross_i->second.xy;
			cross_i++;
		}
		if (inserted != do_not_insert) {
			result.push_back(inserted);
			do_not_insert = inserted;
		}

		//no more crosspoins on this edge, move to the next vertex
		if (cross_i == p[walk].crosspoints->at(index).end()) {
			index = (index + 1) % p[walk].pl->size(); //increment and wrap around
			pos = -1; //take the vertex at index
			previous = inserted;
			continue;
		}

		//crosspoint: see which polygon continues on the outside. To do this determine next points in both
		XY next_me, next_you;  //next_me is walk, you is 1-walk
		auto other_next_cross_i = ++p[1-walk].crosspoints->at(cross_i->second.other_vertex).find(cross_i->second.other_pos);
		auto next_cross_i = cross_i; next_cross_i++;

		if (other_next_cross_i == p[1-walk].crosspoints->at(cross_i->second.other_vertex).end())
			next_you = p[1-walk].pl->at((cross_i->second.other_vertex + 1) % p[1-walk].pl->size());
		else 
			next_you = other_next_cross_i->second.xy;
		if (next_cross_i == p[walk].crosspoints->at(index).end())
			next_me = p[walk].pl->at((index + 1) % p[walk].pl->size());
		else 
			next_me = next_cross_i->second.xy;

		//now we have a walked edge: previous->inserted which branches off to two directions:
		//inserted->next_me and inserted->next_you. Let us pick the one where the angle is less.
		double angle_me  = angle(inserted, previous, next_me);  assert(angle_me>=0);
		double angle_you = angle(inserted, previous, next_you); assert(angle_you>=0);
		
		if (angle_me < angle_you || angle_you == 0) { //remain on this polygon
			pos = cross_i->first;
			if (angle_me == 2) //we continue in a straight line
				result.pop_back();  //do_not_insert holds the popped value: will not get re-inserted in the next round
		} else { //swicth to the other polygon
			index = cross_i->second.other_vertex;
			pos = cross_i->second.other_pos;
			walk = 1-walk;
			if (angle_you == 2) //we continue in a straight line
				result.pop_back();  //do_not_insert holds the popped value: will not get re-inserted in the next round 
		}
		previous = inserted; //Ok to use this value even if we popped inserted just above only its direction counts
	} while (walk != 1 || index != halt || pos >= 0); //do until we get be to the vertex we started at

	//if we never touched walk==0 (the "this" polygon) the two polygons do not touch, return empty
	if (!was_walk_0) {
		result.clear();
		return false;
	}

	//Now supposedly result contains the result
	return true;
}

void MscPointList::Rotate90Clockwise(XY p) 
{
	//XXX
}

void MscPointList::Rotate90CounterClockwise(XY p) 
{
	//XXX
}

void MscPointList::Path(cairo_t *cr) const
{
	if (size()<3 || cr==NULL) return;
	cairo_move_to(cr, at(0).x, at(0).y);
	for (int i = 1; i<size(); i++) 
		cairo_line_to(cr, at(i).x, at(i).y);
	cairo_close_path(cr);
}

void MscPolygon::swap(MscPolygon &a) 
{
	pl.swap(a.pl); 
	std::swap(boundingBox, a.boundingBox);
	std::swap(boundingBoxCurrent, a.boundingBoxCurrent);
	std::swap(arc, a.arc);
    std::swap(drawType, a.drawType);
    std::swap(findType, a.findType);
}

const Block &MscPolygon::GetBoundingBox(void) const
{
	if (!boundingBoxCurrent) {
		pl.begin()->GetBoundingBox(boundingBox);
		boundingBoxCurrent = true;
	}
	return boundingBox;
}

void MscPolygon::Expand(MscPolygon &polygon, double gap) const
{
	//XXX
}

bool MscPolygon::Overlaps(const MscPolygon &b, double gap) const 
{
	return false; //XXX
}

bool MscPolygon::IsWithin(XY p) const 
{
	if (!pl.begin()->IsWithin(p)) return false;
	for (auto i = ++pl.begin(); i!= pl.end(); i++) 
		if (i->IsWithin(p)) return false;
	return true;
}

bool MscPolygon::Union(const MscPolygon &b, MscPolygon &result) const //true if OK, false if not touching
{
	result.clear();
	return pl.begin()->Union(*b.pl.begin(), *result.pl.begin()); //XXX
}

void MscPolygon::Rotate90Clockwise(XY p) 
{
	for (auto i = pl.begin()++; i!= pl.end(); i++) 
		i->Rotate90Clockwise(p);
}

void MscPolygon::Rotate90CounterClockwise(XY p) 
{
	for (auto i = pl.begin()++; i!= pl.end(); i++) 
		i->Rotate90CounterClockwise(p);
}


void MscPolygon::Line(cairo_t *cr) const
{
	for (auto i = pl.begin(); i!= pl.end(); i++) {
		i->Path(cr);
		cairo_stroke(cr);
	}
}

void MscPolygon::Fill(cairo_t *cr) const
{
	for (auto i = pl.begin(); i!= pl.end(); i++) 
		i->Path(cr);
	cairo_fill(cr);
}

void Geometry2::swap(Geometry2 &a) 
{
	cover.swap(a.cover); 
	std::swap(boundingBox, a.boundingBox);
	std::swap(boundingBoxCurrent, a.boundingBoxCurrent);
	std::swap(mainline, a.mainline);
}

Geometry2 &Geometry2::operator+=(const MscPolygon &polygon)
{
	boundingBoxCurrent=false;
	for (cover_t::iterator j = cover.begin(); j!=cover.end(); j++) {
		MscPolygon result;
		if (j->Union(polygon, result)) {
			//i overlaps with one of our polygons: go out and check if there are more
			cover.erase(j);
			for (cover_t::iterator k = cover.begin(); k!=cover.end(); /*none*/) {
				MscPolygon result2;
				if (k->Union(result, result2)) {
					cover.erase(k++);
					result.swap(result2);
				} else k++;
			}
			cover.push_back(result);
			return *this;
		}
	}
	//polygon overlaps with none of our polygons, just add it.
	cover.push_back(polygon);
    return *this;
}

Geometry2 &Geometry2::operator+=(const Geometry2 &g)
{
	for(cover_t::const_iterator i=g.cover.begin(); i!=g.cover.end(); i++) 
		operator += (*i);
    mainline.Extend(g.mainline);
	boundingBoxCurrent=false;
    return *this;
}

const Block &Geometry2::GetBoundingBox(void) const
{
	if (!boundingBoxCurrent) {
		boundingBox.MakeInvalid();
		for (cover_t::const_iterator j = cover.begin(); j!=cover.end(); j++)
			boundingBox += j->GetBoundingBox();
		boundingBoxCurrent = true;
	}
	return boundingBox;
}


bool Geometry2::IsWithin(XY p) const
{
	for(cover_t::const_iterator i=cover.begin(); i!=cover.end(); i++) 
		if (i->IsWithin(p)) return true;
	return false;
}

void Geometry2::Line(cairo_t *cr) const
{
	for(cover_t::const_iterator i=cover.begin(); i!=cover.end(); i++) 
		i->Line(cr);
}

void Geometry2::Fill(cairo_t *cr) const 
{
	for(cover_t::const_iterator i=cover.begin(); i!=cover.end(); i++) 
		i->Fill(cr);
}