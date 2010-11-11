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
#include "geometry_xarea.h"

namespace geometry {

/////////////////MscCrossPointStore implementation

//cp means crosspoint from here
//a struct that identifies a crosspoint at a polygon by giving the edge and the pos on the edge
struct cp_pointer_t 
{
	int vertex;   //the number of the vertex staring the edge the crosspoint is on
	double pos;   //-1 if it is a vertex and not a crosspoint, [0..1) the pos of the crosspoint on the edge
	cp_pointer_t() {}
	cp_pointer_t(int a, double b) : vertex(a), pos(b) {}
	bool operator !=(const cp_pointer_t &o) {return vertex!=o.vertex || pos!=o.pos;}
};

//we store structs like this for each crosspoint
struct cp_t 
{
	XY           xy;               //coordinates of the crosspoint
	cp_pointer_t other;            //the location of the crosspoint in the other polygon
	bool         switch_to_other;  //true if we need to swicth to the other polygon (set by AddCrosspoint)
	bool         stop_here;        //will be set true in the crosspoint we start walking
	cp_t(const XY &a, const cp_pointer_t &b, bool sw) : 
		xy(a), other(b), switch_to_other(sw), stop_here(false) {}
};

typedef std::multimap<double, cp_t> cp_list_t;

//an entire hidden class usable only by Polygon
//we store the points where the other polygon crosses an edge, plus remaining part of curving edges
//edges correspond to the elements in the vector and the crosspoints are sorted there
//according to their distance from the starting vertex of the edge
//we store a crosspoint even if it is equal to the starting vertex of an edge, but not if it is the end 
//(that will be stored for the next edge).
//at any given position on the edge there can be multiple crosspoints (hence a multimap)
class MscCrossPointStore : protected std::vector<cp_list_t> 
{
	friend class Polygon;
protected: 
	const Polygon &polygon; 
	MscCrossPointStore(const Polygon &p) : std::vector<cp_list_t>(p.size()), polygon(p) {}

	cp_list_t::const_iterator Lookup(const cp_pointer_t &pCP) const {return at(pCP.vertex).find(pCP.pos);}
	cp_list_t::iterator       Lookup(const cp_pointer_t &pCP)       {return at(pCP.vertex).find(pCP.pos);}
	cp_list_t::iterator       Add(const XY& xy, const cp_pointer_t &us, const cp_pointer_t &other, bool sw);
	void                      Remove(cp_pointer_t const &pCP) {at(pCP.vertex).erase(Lookup(pCP));}

	cp_pointer_t First(void) const;  //returns vertex == -1 if we are empty
	cp_pointer_t NextCP(cp_pointer_t const &pCP) const;       //Gives the next crosspoint
	Edge GetEdgeRemainder(cp_pointer_t pCP) const {
		if (pCP.pos == -1) return polygon[pCP.vertex];
	    return Edge(polygon[pCP.vertex]).SetStart(Lookup(pCP)->second.xy, pCP.pos);
	}
	static int  RateCrosspoint(int one_i, int two_j, XY p, double one_pos, double two_pos,
		                       bool do_union, Polygon::poly_result_t &contain,
		                       const Polygon &one, const Polygon &two,
							   cp_pointer_t &one_pCP, cp_pointer_t &two_pCP);
	static bool AddCrosspoint(int one_i, int two_j, XY p,double one_pos, double two_pos, 
		                      bool do_union, MscCrossPointStore &one_cp, MscCrossPointStore &two_cp);
	//find crosspoints between polygon of one and two, deposint them in *_cp. return true if any
	static int FindCrosspoints(bool do_union, MscCrossPointStore &one_cp, MscCrossPointStore &two_cp);
	
}; 

inline cp_list_t::iterator MscCrossPointStore::Add(const XY &xy, const cp_pointer_t &us, const cp_pointer_t &other, bool sw) 
{
	return at(us.vertex).insert(cp_list_t::value_type(us.pos, cp_t(xy, other, sw)));
}

cp_pointer_t MscCrossPointStore::First(void) const
{
	for (int i=0; i<size(); i++)
		if (at(i).size()>0) return cp_pointer_t(i, at(i).begin()->first);
	return cp_pointer_t(-1, -1);
}

cp_pointer_t MscCrossPointStore::NextCP(const cp_pointer_t &pCP) const 
{
	if (pCP.pos < 0) {                                                      //in a vertex    
		if (at(pCP.vertex).size() == 0)                                
			return cp_pointer_t((pCP.vertex+1) % size(), -1);               //no CPs: next vertex
		else 
			return cp_pointer_t(pCP.vertex, at(pCP.vertex).begin()->first); //first CP on this edge
	}
	cp_list_t::const_iterator i = ++Lookup(pCP);
	if (i == at(pCP.vertex).end()) 
		return cp_pointer_t((pCP.vertex+1) % size(), -1);  //no next crosspoint
	else
		return cp_pointer_t(pCP.vertex, i->first); //there is a next crosspoint
}

//returns 0 if a crosspoint is to be neglected (walk should not start there and no switch to be done there)
//(if only two vertex touch, but no edges, also if edges touch but not right for union or intersection)
//returns 1 or two if the selected method (union or intersect) should continue at one or two, resp.
//one_i and two_j contains the number of the edges in the resp polygons, p is the coord of the actual CP
//contain is filled with the following values
//SAME: if the two outgoing edges leave the same direction (incoming edges ignored)
//A_CONTAINS_B: if the it is clear that polygon one is inside two, or they have a real intersect
//B_CONTAINS_A: vice versa. We will use "contain" only if we know that one and two do not haver real intersect,
//            so calling these "inside" is OK
//APART: if it is clear the two polynoms only touch each other from the outside
//    or if it is clear the two polynoms only touch each other from the inside (if both are counterclockwise)
//    or both incoming and outgoing edges lie in same direction, but opposite 

int MscCrossPointStore::RateCrosspoint(int one_i, int two_j, XY p, double one_pos, double two_pos,
	                                   bool do_union, Polygon::poly_result_t &contain,
									   const Polygon &one, const Polygon &two,
									   cp_pointer_t &one_pCP, cp_pointer_t &two_pCP)
{
	//if the crosspoint falls on any ending vertex, skip it, it will pop up for the next edge as the starting vertex
	//contain is not set, when we need that value in containment, we sould not return here
	if (one_pos == 1 || two_pos == 1) return 0;
	one_pCP.pos = one_pos;   one_pCP.vertex = one_i;
	two_pCP.pos = two_pos;   two_pCP.vertex = two_j;

	//Determine the category of the crosspoint
	//Obtain coordinates of the previous and next points in both polygons
	const XY prev_one_xy = one[one_pCP.vertex].PrevTangentPoint(one_pCP.pos, one.at_prev(one_pCP.vertex));
	const XY next_one_xy = one[one_pCP.vertex].NextTangentPoint(one_pCP.pos, one.at_next(one_pCP.vertex));
	const XY prev_two_xy = two[two_pCP.vertex].PrevTangentPoint(two_pCP.pos, two.at_prev(two_pCP.vertex));
	const XY next_two_xy = two[two_pCP.vertex].NextTangentPoint(two_pCP.pos, two.at_next(two_pCP.vertex));

	//calculate angles (not real degrees) with the current point as the tip of
	//the angle and the two previous points as one line of it and the two
	//next points as the other line. Angles are clockwise and can be > 180 degrees
	const double one_one_angle = angle(p, prev_one_xy, next_one_xy);
	const double two_two_angle = angle(p, prev_two_xy, next_two_xy);
	const double two_one_angle = angle(p, prev_two_xy, next_one_xy);
	const double one_two_angle = angle(p, prev_one_xy, next_two_xy);

	//What do the angles tell us?
	//1. if 0 < X_Y_angle < X_X_angle then the outgoing edge of polygon Y
	//   goes fully outside the area of polygon X 
	//2. if X_Y_angle == X_X_angle, the outgoing edges of the two polygons lie on 
	//   the same line, meaning they occupy the same area (will be true for both X and Y)
	//3. if 0 == X_Y_angle the incoming edge of X lies in the same area as the outgoing
	//   edge of Y meaning the two polygons touch by an edge 

	//case #2: we should not switch either with unions nor intersects
	//This cannot be the only cp, so letting it go does not harm either
	//(if all cps are like this and we let all go: fine, that means the 
	//two polygons are the same)
	if (one_two_angle == one_one_angle) {
		contain = Polygon::SAME;   //well they can *come* different, but we ignore that here 
		return 0; //skip this crosspoint
	}
	
	if (0 == one_two_angle) {  //case #3 for two
		if (0 == two_one_angle) { //case #3 for one, too                           //..^|2,,,   (o represents the cp, if two that is only ascii limitation)
			//both incoming and outgoing edges are the same here, but opposite dir //..||,,,,   (dots represent areas polygon one)
			//for unions we should not be here (middle of the union, not the edge) //..|o--->   (commas represent areas polygon one)
			//for intersects we can continue along any edge                        //..o----1   (colons represent areas of intersect)
			contain = Polygon::APART;                                              //........   (numbers indicate which polygon is which)
			return 0;
		}
		if (two_one_angle < two_two_angle) { //case #1 for one: it goes free
			contain = Polygon::APART;
			if (do_union)            // 2\   ->1
				return 1;            // ,,\ /..
			else                     // ,,oo...
				return 0;            // ,,||...
		}                            // ,,V|...   //for intersects this is not a relevant point
		contain = Polygon::A_INSIDE_B;
		if (do_union)            //one goes inside polygon two
			return 0;            // ,,,|2     //for unions this is not a relevant point
		else                     // ,,,|      
			return 1;            //1<--o+
			                     // :::||
			                     // :::V|
	}		
	if (0 == two_one_angle) {  //case #3 for one. From above we know case#3 does not hold for two
		if (one_two_angle < one_one_angle) { //case #1 for two: it goes free
			contain = Polygon::A_INSIDE_B;
			if (do_union)            // 1\   ->2
				return 2;            // ..\ /,,
			else                     // ..oo,,,
				return 0;            // ..||,,,
		}	                         // ..V|,,,   //for intersects this is not a relevant point
		contain = Polygon::B_INSIDE_A;
		if (do_union)            //two goes inside polygon one
			return 0;            // ...|1     //for unions this is not a relevant point
		else                     // ...|      
			return 2;            //2<--o+
			                     // :::||
	}                            // :::V|
	
	//here we know no incoming or outgoing edges are the same
	//(well, we did not check for one_one_angle or two_two_angle for zero
	//whch would mean a degenerate polynom, since we assume we get good polynoms)
	if (one_two_angle < one_one_angle) {  //case #1 for two: two goes outside polygon one
		if (two_one_angle < two_two_angle){          //..\1 2->     //the two polygons merely touch at a vertex
			contain =  Polygon::APART;               //...\ /,,,    //(from the outside)                   
		    return 0;                                //1<--o---2                         
		}                                            //                                   
		contain = Polygon::A_INSIDE_B;  //not entirealy fair as they can be true intersects
		                                   //but this value will be used only if the two polygons
		                                   //touch only at vertices or edges, in which case a is in b
		if (do_union)        //::::|....         //::\1,,/2         //  2^,,,,
		    return 2;        //::::|....    OR   //:::\,/      OR   //   |,,,,
		else                 //<---o---1         //1<--o-->2        //---o--->1
			return 1;        //,,,,|             //,,,,,,,,         //:::::::: 
	}                        //,,,,V2
	
	if (two_one_angle < two_two_angle) {  //same a above three cases but 1 and 2 reversed
		contain = Polygon::B_INSIDE_A;
		if (do_union)                     
			return 1;                     
		else                              
			return 2;              
	}                              // ::::|1   
	contain = Polygon::APART;      //2<---o---->1
	return 0;                      //    2|:::: 
}

//returns if a crosspoint has actually been added or not
inline bool MscCrossPointStore::AddCrosspoint(int one_i, int two_j, XY p,  double one_pos, double two_pos,
	                                          bool do_union, MscCrossPointStore &one_cp, MscCrossPointStore &two_cp)
{
	cp_pointer_t one_pCP, two_pCP;
	Polygon::poly_result_t dummy;
	int rating = RateCrosspoint(one_i, two_j, p, one_pos, two_pos, do_union, dummy, 
		                        one_cp.polygon, two_cp.polygon, one_pCP, two_pCP);
	if (rating == 0) return false;
	//cut the part of the edge before the crosspoint, so that it starts at p
	one_cp.Add(p, one_pCP, two_pCP, rating==2);
	two_cp.Add(p, two_pCP, one_pCP, rating==1);
	return true;
}


//We assume one_cp and two_cp are dimensioned to be of equal size with one and two
//Here we find all points where the two polygons touch in a way relevant for the op.
//Eventually we shall be able to develop something more efficient than the O(N^2) we have here
//like Bentley-Ottoman
int MscCrossPointStore::FindCrosspoints(bool do_union, MscCrossPointStore &one_cp, MscCrossPointStore &two_cp)
{
	XY r[4];
	double one_pos[4], two_pos[4];
	bool was = false;
	int num=0;
	for (int i = 0; i<one_cp.polygon.size(); i++)
		for (int j = 0; j<two_cp.polygon.size(); j++) {
			int n=Edge::Crossing(one_cp.polygon[i], one_cp.polygon.at_next(i).GetStart(), 
			                     two_cp.polygon[j], two_cp.polygon.at_next(j).GetStart(), r, one_pos, two_pos);
			for (int k=0; k<n;k++)
				num += AddCrosspoint(i, j, r[k], one_pos[k], two_pos[k], do_union, one_cp, two_cp); 
		}
	return num;
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
	boundingBox.x.from = std::min(std::min(a.x, b.x), c.x);
	boundingBox.x.till = std::max(std::max(a.x, b.x), c.x);
	boundingBox.y.from = std::min(std::min(a.y, b.y), c.y);
	boundingBox.y.till = std::max(std::max(a.y, b.y), c.y);
}

Polygon::Polygon(const XY &c, double radius_x, double radius_y, double tilt_degree) 
{
	if (radius_y==0) radius_y = radius_x;
	Edge edge(c, radius_x, radius_y, tilt_degree);
	boundingBox = edge.CalculateBoundingBox();
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
is_within_t Polygon::IsWithin(XY p, int *edge) const
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
		double x[2];
		switch (at(e).CrossingHorizontal(p.y, at(epp).GetStart(), x)) {
		case 2:
			if (x[1] == p.x) {  //on an edge
				if (at(e).GetStart().y == p.y) return WI_ON_VERTEX; //on vertex
				else if (at(epp).GetStart().y == p.y) {
					if (edge) *edge = epp;
					return WI_ON_VERTEX;
				} else return WI_ON_EDGE; // on an edge, but not vertex
			}
			if (x[1] > p.x) count ++;
			//fallthrough
		case 1:
			if (x[0] == p.x) {  //on an e
				if (at(e).GetStart().y == p.y) return WI_ON_VERTEX; //on vertex
				else if (at(epp).GetStart().y == p.y) {
					if (edge) *edge = epp;
					return WI_ON_VERTEX;
				} else return WI_ON_EDGE; // on an edge, but not vertex
			}
			if (x[0] > p.x) count ++;
			break;
		case -1:
			if ((at(e).GetStart().x > p.x && at(epp).GetStart().x < p.x) ||
				(at(e).GetStart().x < p.x && at(epp).GetStart().x > p.x)) 
					return WI_ON_EDGE; //goes through p
			if (at(e).GetStart().x == p.x) return WI_ON_VERTEX; //on vertex
			if (at(epp).GetStart().x == p.x) {
				if (edge) *edge = epp;
				return WI_ON_VERTEX; //on vertex
			}
			break;
		}
	}
	return count&1 ? WI_INSIDE : WI_OUTSIDE; //even is out 
}

//Can result SAME, COMPLEMENT, A_INSIDE_B or B_INSIDE_A
Polygon::poly_result_t Polygon::CheckContainmentHelper(const Polygon &b) const 
{
	int edge;
	poly_result_t retval;
	for (int i=0; i<size(); i++)
		//if we are a single ellipsis, use our center point, else use a vertex
		switch (b.IsWithin(at(i).GetStart(), &edge)) {      
		case WI_INSIDE:  return A_INSIDE_B;
		case WI_OUTSIDE: return B_INSIDE_A;
		case WI_ON_EDGE: 
		case WI_ON_VERTEX: 
			cp_pointer_t dummy1, dummy2;
			MscCrossPointStore::RateCrosspoint(i, edge, at(i).GetStart(), 0, 0,         // position on both edge is at the start 
											   true, retval, *this, b, dummy1, dummy2); //(IsWithin returns such an edge)
			switch (retval) {
			case A_INSIDE_B:
			case B_INSIDE_A: return retval;
			case APART:         //next vertext will not be the same, just wait
			case SAME:          //if all vertices are like this, we are same or complement
				break;
			default:
				_ASSERT(0);
			}	
	}
	//we got only SAME or COMPLEMENT results for all our vertices (BTW should get either and not both)
	return retval;
}


//Gives valid result only if the two polygons have no crosspoints
//clockwiseness fully honored
Polygon::poly_result_t Polygon::CheckContainment(const Polygon &other) const 
{
	//special case of two full ellipses beging the same - not caught otherwise
	if (size()==1 && other.size()==1 && at(0).ell == other.at(0).ell)
		return SAME;
	poly_result_t this_in_other = CheckContainmentHelper(other);
	if (this_in_other == SAME) return this_in_other;
	poly_result_t other_in_this = other.CheckContainmentHelper(*this);
	if (other_in_this == SAME) _ASSERT(0);
	//values can only be A_INSIDE_B or B_INSIDE_A by now
	if (this_in_other == A_INSIDE_B) {
		if (other_in_this == A_INSIDE_B) 
			return APART;                     //both inside the other, can only be for two inverse
		else    
			return A_INSIDE_B;                //both checks says this
	} else {
		if (other_in_this == A_INSIDE_B) 
			return B_INSIDE_A;                //both inside the other
		else    
			return APART;                     //both outside the other
	}
}

//Does a union or intersection of two poligons, stores the result in "surfaces" and "holes"
//- For union "surfaces" will be one bounding polygin, the "holes" may have zero or more elements
//- For intersection "surfaces" may have more elements, "holes" is guaranteed to be empty
//"surfaces"  and "holes" will contain counterclockwise output only if you call union with counterclockwise input
//Return values
//OK: the two polygons actually intersect (or at least touch for union) 
//surfaces contains return values only if the result is OK. 
//A_IS_EMPTY, B_IS_EMPTY, BOTH_EMPTY: they mean just that. 
//A_INSIDE_B: *this is fully inside b. (ignoring clockwiseness). 
//B_INSIDE_A: b is fully inside *this. (ignoring clockwiseness). 
//SAME: The two polygons are actually the same (ignoring clockwiseness). 
//APART: The two polygons are apart. (ignoring clockwiseness). 
Polygon::poly_result_t Polygon::PolyProcess(const Polygon &b, PolygonList &surfaces, poly_action_t action) const
{
	PolygonList holes;
	if (size()==0 && b.size()==0) return BOTH_EMPTY;
	if (size()==0) return A_IS_EMPTY;
	if (b.size()==0) return B_IS_EMPTY;

	if (!boundingBox.Overlaps(b.boundingBox)) 
		return APART;
	Polygon b_inv;
	//Set up some data structures, invert b if action is SUBSTRACT
	MscCrossPointStore cp_one(*this), cp_two(action==SUBSTRACT ? b_inv = b.GetInverse() : b);
	MscCrossPointStore *poly   = &cp_one;  //poly will point to the current polygon we are at
	MscCrossPointStore *poly_o = &cp_two; //this will hold data for the other, we will swap these

	//We will first find all the crossing points between the two polygons
	const int crosspoint_num = MscCrossPointStore::FindCrosspoints(action==UNION, cp_one, cp_two);
	if (crosspoint_num==0) 
		return CheckContainment(b);
	
	//OK, we have crosspoints relevant for the operation (union, intersection) we do
	//Start the walks around the polygon, as long as there are crosspoints left
	//The poly.cp->First on the next iteration will give a -1 vertex back if there is no more CPs
	for (cp_pointer_t current = poly->First(); current.vertex>=0; current = poly->First()) {
		static PolygonWithHoles result;
		result.clear();
		result.reserve(size()+b.size()+crosspoint_num);
		
		//do a walk from the current
		do {
			Edge current_xy_edge;  

			if (current.pos < 0) {
				//this is a vertex not crossed by the other polygon, we keep walking this one
				current_xy_edge = poly->GetEdgeRemainder(current); 
				current = poly->NextCP(current);
			} else {
				//this is a crosspoint let's see if we switch to the other polygon
				cp_pointer_t other = poly->Lookup(current)->second.other;
				if (poly->Lookup(current)->second.switch_to_other) {
					std::swap(current, other);
					std::swap(poly, poly_o);
				}
				//Ok, we will insert this edge
				current_xy_edge = poly->GetEdgeRemainder(current); 

				//remove crosspoints and advance to next point
				cp_pointer_t next = poly->NextCP(current);
				if (result.size() == 0) { //mark exit condition
					poly->Lookup(current)->second.stop_here = true;
					poly_o->Lookup(other)->second.stop_here = true;
				} else {
					poly->Remove(current);
					poly_o->Remove(other);
				}
				current = next;
			}
			//Add current point (plus edge out) to results, but not if we added that last
			if (result.size()>0 && current_xy_edge.GetStart() == result.rbegin()->GetStart()) 
				result.pop_back();
			//check if current_xy falls on the same line as the previous two
			if (result.size()>=2 && 
				result[result.size()-2].CheckAndCombine(result[result.size()-1], current_xy_edge.GetStart()))
				result[result.size()-1] = current_xy_edge; //if so overwrite last
			else 
				result.push_back(current_xy_edge);
		} while (current.pos ==-1 || !poly->Lookup(current)->second.stop_here); 
		
		//remove the crosspoint we started and halted at
		poly_o->Remove(poly->Lookup(current)->second.other);
		poly->Remove(current);

		//if the crosspoint we started at was also a vertex, it may be that it is repeated at the end
		if (result[0].GetStart() == result[result.size()-1].GetStart())
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

		//go around and set "s" value for curvy edges. This was not known previously
		//after that compute the bounding box
		for (int i=0; i<result.size(); i++) {
			if (!result[i].IsStraight())
				result[i].SetEnd(result.at_next(i).GetStart());
			result.boundingBox += result[i].CalculateBoundingBox();
		}
		if (result.CalculateClockwise()) 
			surfaces.append(result);
		else 
			holes.append(result.GetInverse());
	}
	//With union we may have holes, but only one surface
	//With intersect (and substract), we can have multiple surfaces, but no holes
	_ASSERT(surfaces.size()==1 || holes.size()==0);
	if (holes.size())
		surfaces.begin()->holes.swap(holes);
	return OK;
}

bool Polygon::CalculateClockwise() const
{
	//determine if this is clockwise. 
	if (size()>2) {
		double angles = 0;
		XY prev = at(0).PrevTangentPoint(0, at_prev(0));
		for (int i=0; i<size(); i++) 
			if (at(i).IsStraight()) {
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


//true if the intersection is not empty (quicker than the intersection)
bool Polygon::Overlaps(const Polygon &b) const 
{
	if (size()==0 || b.size()==0) return false;
	if (!boundingBox.Overlaps(b.boundingBox)) return false;
	XY r[4];
	double one_pos[4], two_pos[4];
	poly_result_t dummy0;
	cp_pointer_t dummy1, dummy2;
	for (int i = 0; i<size(); i++)
		for (int j = 0; j<b.size(); j++) 
			switch (Edge::Crossing(at(i), at_next(i).GetStart(), 
			                           b[j], b.at_next(j).GetStart(),
									   r, one_pos, two_pos)) {
			case 4: if (MscCrossPointStore::RateCrosspoint(i, j, r[3], one_pos[3], two_pos[3], false, dummy0, *this, b, dummy1, dummy2)) return true; //fallthrough!!
			case 3: if (MscCrossPointStore::RateCrosspoint(i, j, r[2], one_pos[2], two_pos[2], false, dummy0, *this, b, dummy1, dummy2)) return true; //fallthrough!!
			case 2: if (MscCrossPointStore::RateCrosspoint(i, j, r[1], one_pos[1], two_pos[1], false, dummy0, *this, b, dummy1, dummy2)) return true; //fallthrough!!
			case 1: if (MscCrossPointStore::RateCrosspoint(i, j, r[0], one_pos[0], two_pos[0], false, dummy0, *this, b, dummy1, dummy2)) return true;
			}
	return false;
}


//current vertex is v in a->v->b order clockwise
//expands v outwards by gap (or inwards if negative)
//returns two points if angle of v is too shap so that one point would be too far out
int expand_helper(double gap, const XY &v, const XY &a, const XY &b, XY& r1, XY &r2)
{
	XY D = (b-v).Rotate90CCW()*(gap/(b-v).length());
	///... yet to come
	return 0;
}


void Polygon::Expand(double gap) 
{
	if (gap==0) return;

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
