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
#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <list>
#include "polygon.h"
#include "mscdrawer.h"

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
	cp_t(XY a, const cp_pointer_t &b, bool sw) : xy(a), other(b), switch_to_other(sw) {}
};

typedef std::multimap<double, cp_t> cp_list_t;

//an entire hidden class usable only by MscPolygon
//we store the points where the other polygon crosses an edge
//edges correspond to the elements in the vector and the crosspoints are sorted there
//according to their distance from the starting vertex of the edge
//we store a crosspoint even if it is equal to the starting vertex of an edge, but not if it is the end 
//(that will be stored for the next edge).
//at any given position on the edge there can be multiple crosspoints (hence a multimap)
class MscCrossPointStore : protected std::vector<cp_list_t> 
{
	friend class MscPolygon;
protected: 
	MscCrossPointStore(int n) : std::vector<cp_list_t>(n) {}

	cp_list_t::const_iterator Lookup(const cp_pointer_t &pCP) const {return at(pCP.vertex).find(pCP.pos);}
	cp_list_t::iterator       Lookup(const cp_pointer_t &pCP)       {return at(pCP.vertex).find(pCP.pos);}
	cp_list_t::iterator       Add(XY xy, const cp_pointer_t &us, const cp_pointer_t &other, bool sw);
	void                      Remove(cp_pointer_t const &pCP) {at(pCP.vertex).erase(Lookup(pCP));}

	cp_pointer_t First(void) const;  //returns vertex == -1 if we are empty
	cp_pointer_t NextCP(cp_pointer_t const &pCP) const;       //Gives the next crosspoint
	XY           GetXY (cp_pointer_t pCP, const MscPolygon &pl) const {return (pCP.pos == -1) ? pl[pCP.vertex] : Lookup(pCP)->second.xy;}

	static int  RateCrosspoint(int one_i, int two_j, XY p, bool do_union, MscPolygon::poly_result_t &contain,
		                       const MscPolygon &one, const MscPolygon &two,
							   cp_pointer_t &one_pCP, cp_pointer_t &two_pCP);
	static bool AddCrosspoint(int one_i, int two_j, XY p,bool do_union,
		                      const MscPolygon &one, const MscPolygon &two, 
		                      MscCrossPointStore &one_cp, MscCrossPointStore &two_cp);
	//find crosspoints between one and two, deposint them in *_cp. return true if any
	static bool FindCrosspoints(bool do_union, const MscPolygon &one, const MscPolygon &two, 
		                        MscCrossPointStore &one_cp, MscCrossPointStore &two_cp);
	
}; 

//////////////////Helper functions


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
		return 1-cos; //gives [0..2]
	else 
		return cos+3; //gives (2..4)
}

inline double angle_degrees(double angle) 
{
	if (angle>=2) 
		return 360 - acos(angle-3)*(180./M_PI);
	else
		return acos(1-angle)*(180./M_PI);
}

//slope of a line (assuming not vertical)
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
		if (A==B) {
			if (A==M) {
				r1 = M;
				return 1;
			}
			return 0; //both sections degenerate to a point, but do not overlap
		}
		double t;
		//select x or y depending on precision
		//M lies on AB, let us see if between A and B
		if (fabs(A.x-B.x) > fabs(A.y-B.y)) 
			t = (M.x-A.x)/(B.x-A.x);
		else 
			t = (M.y-A.y)/(B.y-A.y);
		if (t<0 || t>1) return 0;
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


//Assuming P is between A and B, return where (0 if on A, 1 if on B and values in between)
inline double relative_pos(XY A, XY B, XY P)
{
	//See which coordinate spans more space
	if (fabs(A.x - B.x) < fabs(A.y - B.y)) 
		return (P.y-A.y)/(B.y-A.y);
	else
		return (P.x-A.x)/(B.x-A.x);
}

/////////////////MscPolygonHelper implementation

inline cp_list_t::iterator MscCrossPointStore::Add(XY xy, const cp_pointer_t &us, const cp_pointer_t &other, bool sw) 
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

inline int NextVertex(const cp_pointer_t &pCP, const MscPolygon &pl) 
{
	return (pCP.vertex+1) % pl.size();
}

inline int PrevVertex(const cp_pointer_t &pCP, const MscPolygon &pl) 
{
	if (pCP.pos > 0)          
		return pCP.vertex;    //a cp at the middle of an edge: return preceeding vertex
	return (pCP.vertex-1+pl.size()) % pl.size();  //we are at a vertex: return previous vertex
}

//returns 0 if a crosspoint is to be neglected (walk should not start there and no switch to be done there)
//(if only two vertex touch, but no edges, also if edges touch but not right for union or intersection)
//returns 1 or two if the selected method (union or intersect) should continue at one or two, resp.
//one_i and two_j contains the number of the edges in the resp polygons, p is the coord of the actual CP
//contain is filled with the following values
//SAME: if the two outgoing edges leave the same direction (incoming edges ignored)
//COMPLEMENT: both incoming and outgoing edges lie in same direction, but opposite 
//A_CONTAINS_B: if the it is clear that polygon one is inside two, or they have a real intersect
//B_CONTAINS_A: vice versa. We will use "contain" only if we know that one and two do not haver real intersect,
//            so calling these "inside" is OK
//APART_OUTSIDE: if it is clear the two polynoms only touch each other from the outside
//APART_INSIDE: if it is clear the two polynoms only touch each other from the inside (if both are counterclockwise)

int MscCrossPointStore::RateCrosspoint(int one_i, int two_j, XY p, bool do_union, MscPolygon::poly_result_t &contain,
		                               const MscPolygon &one, const MscPolygon &two, 
									   cp_pointer_t &one_pCP, cp_pointer_t &two_pCP)
{
	//if the crosspoint falls on any ending vertex, skip it, it will pop up for the next edge as the starting vertex
	//contain is not set, when we need that value in containment, we sould not return here
	one_pCP.pos = relative_pos(one[one_i], one[(one_i+1)%one.size()], p);
	if (one_pCP.pos == 1) return 0;
	two_pCP.pos = relative_pos(two[two_j], two[(two_j+1)%two.size()], p);
	if (two_pCP.pos == 1) return 0;
	one_pCP.vertex = one_i;
	two_pCP.vertex = two_j;

	//Determine the category of the crosspoint
	//Obtain coordinates of the previous and next points in both polygons
	XY prev_one_xy = one[PrevVertex(one_pCP, one)];
	XY next_one_xy = one[NextVertex(one_pCP, one)];
	XY prev_two_xy = two[PrevVertex(two_pCP, two)];
	XY next_two_xy = two[NextVertex(two_pCP, two)];

	//calculate angles (not real degrees) with the current point as the tip of
	//the angle and the two previous points as one line of it and the two
	//next points as the other line. Angles are clockwise and can be > 180 degrees
	double one_one_angle = 2.; //180 degrees: straight line 
	if (one_pCP.pos <= 0) //current point is a vertex on one polygon
		one_one_angle = angle(p, prev_one_xy, next_one_xy);
	double two_two_angle = 2.; //180 degrees: straight line 
	if (two_pCP.pos <= 0) //current point is a vertex on the two polygon
		two_two_angle = angle(p, prev_two_xy, next_two_xy);
	double two_one_angle = angle(p, prev_two_xy, next_one_xy);
	double one_two_angle = angle(p, prev_one_xy, next_two_xy);

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
		contain = MscPolygon::SAME;   //well they can *come* different, but we ignore that here 
		return 0; //skip this crosspoint
	}
	
	if (0 == one_two_angle) {  //case #3 for two
		if (0 == two_one_angle) { //case #3 for one, too                           //..^|2,,,   (o represents the cp, if two that is only ascii limitation)
			//both incoming and outgoing edges are the same here, but opposite dir //..||,,,,   (dots represent areas polygon one)
			//for unions we should not be here (middle of the union, not the edge) //..|o--->   (commas represent areas polygon one)
			//for intersects we can continue along any edge                        //..o----1   (colons represent areas of intersect)
			contain = MscPolygon::COMPLEMENT;                                    //........   (numbers indicate which polygon is which)
			return 0;
		}
		if (two_one_angle < two_two_angle) { //case #1 for one: it goes free
			contain = MscPolygon::APART_OUTSIDE;
			if (do_union)            // 2\   ->1
				return 1;            // ,,\ /..
			else                     // ,,oo...
				return 0;            // ,,||...
		}                            // ,,V|...   //for intersects this is not a relevant point
		contain = MscPolygon::A_INSIDE_B;
		if (do_union)            //one goes inside polygon two
			return 0;            // ,,,|2     //for unions this is not a relevant point
		else                     // ,,,|      
			return 1;            //1<--o+
			                     // :::||
			                     // :::V|
	}		
	if (0 == two_one_angle) {  //case #3 for one. From above we know case#3 does not hold for two
		if (one_two_angle < one_one_angle) { //case #1 for two: it goes free
			contain = MscPolygon::A_INSIDE_B;
			if (do_union)            // 1\   ->2
				return 2;            // ..\ /,,
			else                     // ..oo,,,
				return 0;            // ..||,,,
		}	                         // ..V|,,,   //for intersects this is not a relevant point
		contain = MscPolygon::B_INSIDE_A;
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
			contain =  MscPolygon::APART_OUTSIDE;  //...\ /,,,    //(from the outside)                   
		    return 0;                                //1<--o---2                         
		}                                            //                                   
		contain = MscPolygon::A_INSIDE_B;  //not entirealy fair as they can be true intersects
		                                       //but this value will be used only if the two polygons
		                                       //touch only at vertices or edges, in which case a is in b
		if (do_union)        //::::|....         //::\1,,/2         //  2^,,,,
		    return 2;        //::::|....    OR   //:::\,/      OR   //   |,,,,
		else                 //<---o---1         //1<--o-->2        //---o--->1
			return 1;        //,,,,|             //,,,,,,,,         //:::::::: 
	}                        //,,,,V2
	
	if (two_one_angle < two_two_angle) {  //same a above three cases but 1 and 2 reversed
		contain = MscPolygon::B_INSIDE_A;
		if (do_union)                     
			return 1;                     
		else                              
			return 2;                        // ::::|1   
	}                                        //2<---o---->1
	contain = MscPolygon::APART_INSIDE;      //    2|::::
	return 0;     
}

//returns if a crosspoint has actually been added or not
inline bool MscCrossPointStore::AddCrosspoint(int one_i, int two_j, XY p,  bool do_union,
		                              const MscPolygon &one, const MscPolygon &two, 
		                              MscCrossPointStore &one_cp, MscCrossPointStore &two_cp)
{
	cp_pointer_t one_pCP, two_pCP;
	MscPolygon::poly_result_t dummy;
	int rating = RateCrosspoint(one_i, two_j, p, do_union, dummy, one, two, one_pCP, two_pCP);
	if (rating == 0) return false;
	one_cp.Add(p, one_pCP, two_pCP, rating==2);
	two_cp.Add(p, two_pCP, one_pCP, rating==1);
	return true;
}


//We assume one_cp and two_cp are dimensioned to be of equal size with one and two
//Here we find all points where the two polygons touch in a way relevant for the op.
//Eventually we shall be able to develop something more efficient than the O(N^2) we have here
//like Bentley-Ottoman
bool MscCrossPointStore::FindCrosspoints(bool do_union, const MscPolygon &one, const MscPolygon &two, 
		                                 MscCrossPointStore &one_cp, MscCrossPointStore &two_cp)
{
	XY r1, r2;
	bool was = false;
	for (int i = 0; i<one.size(); i++)
		for (int j = 0; j<two.size(); j++) 
			switch (are_crossing(one[i], one[(i+1)%one.size()], two[j], two[(j+1)%two.size()], r1, r2)) {
			case 2: was |= AddCrosspoint(i, j, r2, do_union, one, two, one_cp, two_cp); //fallthrough!!
			case 1: was |= AddCrosspoint(i, j, r1, do_union, one, two, one_cp, two_cp);
			}
	return was;
}



///////////////////////////// MscPolygon

//Do not create degenerate triangles.
//Always create clockwise.
PosMscPolygon::PosMscPolygon(XY a, XY b, XY c) 
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
	boundingBox.x.from = std::min(std::min(a.x, b.x), c.x);
	boundingBox.x.till = std::max(std::max(a.x, b.x), c.x);
	boundingBox.y.from = std::min(std::min(a.y, b.y), c.y);
	boundingBox.y.till = std::max(std::max(a.y, b.y), c.y);
	clockwise = true;
}


PosMscPolygon &PosMscPolygon::operator =(const Block &b) 
{
	clear();
	boundingBox = b;
	clockwise = true;
	if (b.IsInvalid()) return *this;
	push_back(b.UpperLeft());
	push_back(XY(b.x.till, b.y.from));
	push_back(b.LowerRight());
	push_back(XY(b.x.from, b.y.till));
	return *this;
}

//honors the direction of the polygon (clockwise or counterclockwise)
//in p* return the number of vertex or edge we have fallen on if result is such
is_within_t MscPolygon::IsWithin(XY p, int*pEdge) const
{
	if (size()<3) return WI_OUTSIDE;
	if (boundingBox.IsWithin(p)==WI_OUTSIDE) 
		return clockwise ? WI_OUTSIDE : WI_INSIDE; //for clockwise, if a point is outside the BB it is not within

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
		if ((at(i).y >= p.y && at(ipp).y < p.y) ||      //we cross upward 
			(at(i).y < p.y && at(ipp).y >= p.y)) {      //we cross downward
			//we cross p's line x
			double x = (at(i).x - at(ipp).x)/(at(i).y - at(ipp).y)*(p.y - at(ipp).y) + at(ipp).x;
			if (x == p.x) {  //on an edge
				if (pEdge) *pEdge = i;
				if (at(i).y == p.y) return WI_ON_VERTEX; //on vertex
				else if (at(ipp).y == p.y) {
					if (pEdge) ++*pEdge;
					return WI_ON_VERTEX;
				} else return WI_ON_EDGE; // on an edge, but not vertex
			}
			if (x > p.x) count ++;
		} else if (at(i).y == p.y && at(ipp).y == p.y) { //horizontal edge level with p
			if (pEdge) *pEdge = i;
			if (at(ipp).x == p.x || at(i).x == p.x) return WI_ON_VERTEX; //on vertex
			if ((at(i).x > p.x && at(ipp).x < p.x) ||
				(at(i).x < p.x && at(ipp).x > p.x)) return WI_ON_EDGE; //goes through p
		}
	}
	if (clockwise) 
		return count&1 ? WI_INSIDE : WI_OUTSIDE; //even is out 
	else
		return count&1 ? WI_OUTSIDE : WI_INSIDE; //even is out, but counterclockwise means an inside-out
}

//Can result SAME, COMPLEMENT, A_INSIDE_B or B_INSIDE_A
MscPolygon::poly_result_t MscPolygon::CheckContainmentHelper(const MscPolygon &b) const 
{
	int edge = 0;
	poly_result_t retval;
	for (int i=0; i<size(); i++) 
		switch (b.IsWithin(at(i), &edge)) {      //iswithin also honors clockwiseness
		case WI_INSIDE:  return A_INSIDE_B;
		case WI_OUTSIDE: return B_INSIDE_A;
		case WI_ON_EDGE: 
		case WI_ON_VERTEX: 
			cp_pointer_t dummy1, dummy2;
			MscCrossPointStore::RateCrosspoint(i, edge, at(i), true, retval, *this, b, dummy1, dummy2);
			switch (retval) {
			case A_INSIDE_B:
			case B_INSIDE_A: return retval;
			case APART_INSIDE:
			case APART_OUTSIDE: break;  //next vertext will not be the same, just wait
			case SAME:
			case COMPLEMENT: break; //if all vertices are like this, we are same or complement
			}	
	}
	//we got only SAME or COMPLEMENT results for all our vertices (BTW should get either and not both)
	return retval;
}


//Gives valid result only if the two polygons have no crosspoints
//clockwiseness fully honored
MscPolygon::poly_result_t MscPolygon::CheckContainment(const MscPolygon &other) const 
{
	poly_result_t this_in_other = CheckContainmentHelper(other);
	if (this_in_other == SAME || this_in_other == COMPLEMENT) return this_in_other;
	poly_result_t other_in_this = other.CheckContainmentHelper(*this);
	if (other_in_this == SAME || other_in_this == COMPLEMENT) assert(0);
	//values can only be A_INSIDE_B or B_INSIDE_A by now
	if (this_in_other == A_INSIDE_B) {
		if (other_in_this == A_INSIDE_B) 
			return APART_INSIDE;              //both inside the other
		else    
			return A_INSIDE_B;                //both checks says this
	} else {
		if (other_in_this == A_INSIDE_B) 
			return B_INSIDE_A;                //both inside the other
		else    
			return APART_OUTSIDE;             //both outside the other
	}
}

inline void MscPolygon::append_to_surfaces_or_holes(PosMscPolygonList &surfaces, InvMscPolygonList &holes) const
{
	if (clockwise) surfaces.append(*static_cast<const PosMscPolygon*>(this));
	else holes.append(*static_cast<const InvMscPolygon*>(this));
}

//Does a union or intersection of two poligons, stores the result in "surfaces" and "holes"
//- For union "surfaces" will be one bounding polygin, the "holes" may have zero or more elements
//- For intersection "surfaces" may have more elements, "holes" is guaranteed to be empty
//"surfaces"  and "holes" will contain counterclockwise output only if you call union with counterclockwise input
//Return values
//OK: the two polygons actually intersect (or at least touch for union) if b is counterclockwise it is honored (used for substracts)
//A_IS_EMPTY, B_IS_EMPTY, BOTH_EMPTY: they mean just that. "surfaces" or "holes" contains one or the other or none of b & this
//A_INSIDE_B: *this is fully inside b. *this is placed for unions, b for intersects (into surfaces/holes depending on if clockwise)
//B_INSIDE_A: b is fully inside *this. *this is placed for intersects, b for unions (into surfaces/holes depending on if clockwise)
//A_SAME_B: The two polygons are actually the same (including clockwiseness). *this is put into "surfaces"/"holes"
//A_COMPLEMENT_B: The two polygons have the same outline, but differ in direction. nothing is put into  "surfaces"/"holes"
//A_APART_B: The two polygons do not actually touch (for intersections this includes common edges, for union common vertices)
//       both are returen for unions (in holes and/or surfaces) and nothing for intersects
MscPolygon::poly_result_t MscPolygon::PolyProcess(const MscPolygon &b, PosMscPolygonList &surfaces, 
													  InvMscPolygonList &holes, bool do_union) const
{
	surfaces.clear();
	holes.clear();
	if (size()<3 && b.size()<3) return BOTH_EMPTY;
	if (size()<3) {
		if (do_union) b.append_to_surfaces_or_holes(surfaces,holes);
		return A_IS_EMPTY;
	}
	if (b.size()<3) {
		if (do_union) append_to_surfaces_or_holes(surfaces,holes);
		return B_IS_EMPTY;
	}

	if (!boundingBox.Overlaps(b.boundingBox)) {
		if (clockwise && b.clockwise) {
			if (do_union) 
				surfaces.append(*static_cast<const PosMscPolygon*>(this)).append(*static_cast<const PosMscPolygon*>(&b));
			return APART_OUTSIDE;
		}
		if (!clockwise && !b.clockwise) {
			if (!do_union) 
				holes.append(*static_cast<const InvMscPolygon*>(this)).append(*static_cast<const InvMscPolygon*>(&b));
			return APART_INSIDE;
		}
		//now clockwise and !clockwise are different
		if (clockwise && !b.clockwise) {
			(do_union ? b : *this).append_to_surfaces_or_holes(surfaces, holes);
			return A_INSIDE_B;
		} else {  //if (!clockwise && b.clockwise) 
			(do_union ? *this : b).append_to_surfaces_or_holes(surfaces, holes);
			return B_INSIDE_A;
		}
	}

	//Set up some data structures
	MscCrossPointStore cp_one(size()), cp_two(b.size());
	struct poly_t {
		const MscPolygon *pl;
		MscCrossPointStore *cp;
	} 
	pl_cp_one = {this, &cp_one},   &poly = pl_cp_one,    //poly will point to the current polygon we are at
	pl_cp_two = {&b, &cp_two},     &poly_o = pl_cp_two;  //this will hold data for the other, we will swap these

	//We will first find all the crossing points between the two polygons
	if (!MscCrossPointStore::FindCrosspoints(do_union, *this, b, cp_one, cp_two)) 
		//if no crosspoints fill surfaces and holes with what is needed and return correct value
		switch (CheckContainment(b)) {
		case A_INSIDE_B:
			(do_union ? b : *this).append_to_surfaces_or_holes(surfaces, holes); return A_INSIDE_B;
		case B_INSIDE_A:
			(do_union ? *this : b).append_to_surfaces_or_holes(surfaces, holes); return B_INSIDE_A;
		case SAME:
			append_to_surfaces_or_holes(surfaces, holes); return SAME;
		case COMPLEMENT:
			return COMPLEMENT;
		case APART_OUTSIDE:
			if (do_union) {
				append_to_surfaces_or_holes(surfaces, holes); 
				b.append_to_surfaces_or_holes(surfaces, holes);
			}
			return APART_OUTSIDE;
		case APART_INSIDE:             //two inside-out polygons not touching
			if (!do_union) {
				append_to_surfaces_or_holes(surfaces, holes); 
				b.append_to_surfaces_or_holes(surfaces, holes);
			}
			return APART_INSIDE;
		}
	
	//OK, we have crosspoints relevant for the operation (union, intersection) we do
	//Start the walks around the polygon, as long as there are crosspoints left
	//The poly.cp->First on the next iteration will give a -1 vertex back if there is no more CPs
	for (cp_pointer_t current = poly.cp->First(); current.vertex>=0; current = poly.cp->First()) {
		XY do_not_insert(-1,-1); //to avoid inserting the same point multiple times
		cp_pointer_t halt = current; //if we get back here this polygon is complete
		MscPolygon result;
		
		//do a walk from the current
		do {
			//Add current point to results 
			XY current_xy = poly.cp->GetXY(current, *poly.pl); 
			if (current_xy != do_not_insert) {
				//check if current_xy falls on the same line as the previous two
				if (result.size()>=2 &&
					0 == angle(current_xy, result[result.size()-2], result[result.size()-1]))
					result[result.size()-1] = current_xy; //if so overwrite last
				else 
					result.push_back(current_xy);
				result.boundingBox += current_xy;
				do_not_insert = current_xy;
			}

			//this is a vertex not crossed by the other polygon, we keep walking this one
			if (current.pos < 0) {
				current = poly.cp->NextCP(current);
				continue;
			}

			cp_pointer_t other = poly.cp->Lookup(current)->second.other;
			if (poly.cp->Lookup(current)->second.switch_to_other) {
				std::swap(current, other);
				std::swap(poly, poly_o);
			}

			//remove crosspoints and advance to next point
			cp_pointer_t next = poly.cp->NextCP(current);
			if (result.size() == 1) { //mark exit condition
				poly.cp->Lookup(current)->second.xy.x = -1;
				poly_o.cp->Lookup(other)->second.xy.x = -1;
			} else {
				poly.cp->Remove(current);
				poly_o.cp->Remove(other);
			}
			current = next;
		} while (current.pos ==-1 || poly.cp->Lookup(current)->second.xy.x != -1); 
		
		//remove the crosspoint we started and halted at
		poly_o.cp->Remove(poly.cp->Lookup(current)->second.other);
		poly.cp->Remove(current);

		//if the crosspoint we started at was also a vertex, it may be that it is repeated at the end
		if (result[0] == result[result.size()-1])
			result.pop_back();
		//Also, the beginning point can fall on an edge such as when two side-by-side rectangles are merged
		if (angle(result[0], result[1], result[result.size()-1]) == 2) 
			result.erase(result.begin());
		//complete direction counting, adding angle for last inserted vertex and the first one
		double angles = 0;
		for (int i=1; i<result.size()-1; i++)
			angles += angle_degrees(angle(result[i], result[i+1], result[i-1]));
		angles += angle_degrees(angle(result[result.size()-1], result[0], result[result.size()-2]));
		angles += angle_degrees(angle(result[0], result[1], result[result.size()-1]));
		//angle is (n-2)*180 for clockwise, (n+2)*180 for counterclockwise, we draw the line at n*180
		result.clockwise = (angles < result.size()*180); 

		result.append_to_surfaces_or_holes(surfaces, holes);
	}
	return OK;
}


//true if the intersection is not empty (quicker than the intersection)
bool MscPolygon::Overlaps(const MscPolygon &b) const 
{
	if (size()<3 || b.size()<3) return false;
	if (!boundingBox.Overlaps(b.boundingBox)) return false;
	XY r1, r2;
	poly_result_t dummy0;
	cp_pointer_t dummy1, dummy2;
	for (int i = 0; i<size(); i++)
		for (int j = 0; j<b.size(); j++) 
			switch (are_crossing(at(i), at((i+1)%size()), b[j], b[(j+1)%b.size()], r1, r2)) {
			case 2: if (MscCrossPointStore::RateCrosspoint(i, j, r2, false, dummy0, *this, b, dummy1, dummy2)) return true; //fallthrough!!
			case 1: if (MscCrossPointStore::RateCrosspoint(i, j, r1, false, dummy0, *this, b, dummy1, dummy2)) return true;
			}
	return false;
}

void MscPolygon::Path(cairo_t *cr) const
{
	if (size()<3 || cr==NULL) return;
	cairo_move_to(cr, at(0).x, at(0).y);
	for (int i = 1; i<size(); i++) 
		cairo_line_to(cr, at(i).x, at(i).y);
	cairo_close_path(cr);
}

/////////////////////////////////////////  MscPolygonList implementation

//Substract all holes from surfaces (we are the surfaces)
//drop holes that were not fully inside at least one surface
//we assume we hold al positive polygons, holes hold all inside-out ones
//polygons can fully contain other polygons, but not touch or have real intersects
//same for holes
void PosMscPolygonList::NormalizeWith(InvMscPolygonList &holes)
{
	if (!boundingBox.Overlaps(holes.boundingBox)) {
		holes.clear();
		return;
	}
	PosMscPolygonList s;
	InvMscPolygonList h;
	for (auto hole_i=holes.begin(); hole_i!=holes.end(); /*none*/) {
		bool keep = false;
		for (auto surf_i=begin(); surf_i!=end(); /*none*/) 
			switch (surf_i->PolyProcess(*hole_i, s, h, false)) {
			case MscPolygon::OK:
				//a hole intersects with a polygon, result is the decreased size polygon
				//replace the current polygon with the results
				erase(surf_i++);
				splice_with_bb(surf_i, s);
				break;
			case MscPolygon::A_IS_EMPTY:
				erase(surf_i++);
				break;
			case MscPolygon::B_IS_EMPTY:
				holes.erase(hole_i++);
				continue;
			case MscPolygon::BOTH_EMPTY:
			case MscPolygon::COMPLEMENT:   //polygon plugs hole: remove both
				erase(surf_i);
				holes.erase(hole_i++);
				continue;
			case MscPolygon::APART_INSIDE: //non-inversed hole is inside the polygon
				keep = true;                 //this hole is to keep
				//fallthrough
			case MscPolygon::A_INSIDE_B:
				//a polygon is fully inside an (inverse) hole. 
				//which means in normal terms the hole is not in the polygon
			case MscPolygon::APART_OUTSIDE:
				//can only be if the positive surface is inside the inverted hole
				//do nothing
				surf_i++;
				break;
			case MscPolygon::SAME:         //An inverted hole cannot be equal to a non-inverted polygon
			case MscPolygon::B_INSIDE_A:   //An inverted hole cannot be inside a non-inverted polygon
				assert(0);
		}
		if (!keep) {
			holes.erase(hole_i++);
			continue;
		}
		hole_i++;
	}
}

void PosMscPolygonList::Union(const PosMscPolygon &b, InvMscPolygonList &holes)
{
	if (b.size()<3) return;
	if (!boundingBox.Overlaps(b.boundingBox)) {
		append(b);
		return;
	}
	PosMscPolygonList s;
	InvMscPolygonList h;
	PosMscPolygon current_blob(b);
	for (auto i=begin(); i!=end(); /*none*/)
		switch (i->PolyProcess(current_blob, s, h, true)) {
		case MscPolygon::OK:             //real union with an existing surface 
			assert(s.size()>0);
			current_blob.swap(*s.begin());   //update blob with a positive union 
			//add holes. Holes formed this way cannot overlap with existing ones
			//since they are fully outside of the original surfaces
			holes.splice_with_bb(h);
			//fallthrough: now delete i
		case MscPolygon::A_IS_EMPTY:
		case MscPolygon::A_INSIDE_B:     // i is fully covered by b, delete it
			erase(i++);
			break;
		case MscPolygon::APART_OUTSIDE:  //go to next in queue, no holes to add
			i++;
			break;
		case MscPolygon::SAME:           //no change
			return;
		case MscPolygon::B_INSIDE_A:     //b (and the blob) is fully contained in existing surface
			return;                      //here we need to check for nested polygons XXX
		case MscPolygon::B_IS_EMPTY:     //should have been caught above
		case MscPolygon::BOTH_EMPTY:     //oh my
		case MscPolygon::APART_INSIDE:   //this can only be us being inside-out (= a hole)
		case MscPolygon::COMPLEMENT:     //well, b and us both are clockwise->this cannot be
		default:
			assert(0);
		}
	append(current_blob);
}

void PosMscPolygonList::Intersect(const PosMscPolygon &b)
{
	if (b.size()<3) return;
	if (!boundingBox.Overlaps(b.boundingBox)) {
		clear();
		return;
	}
	PosMscPolygonList s;
	InvMscPolygonList h;
	for (auto i=begin(); i!=end(); /*none*/)
		switch (i->PolyProcess(b, s, h, false)) {
		case MscPolygon::B_INSIDE_A:     //b is fully in one of our members
		case MscPolygon::SAME:     
			swap(s);                     //keep the one (in s), erase all else
			return;                      //we will hit no other element
		case MscPolygon::A_IS_EMPTY:
		case MscPolygon::APART_OUTSIDE:  //go to next in queue, remove current one
			erase(i++);			
			break;
		case MscPolygon::A_INSIDE_B:     //this one is fully inside b=>keep it
			i++;
			break;                       
		case MscPolygon::OK:             //real intersect with our polygon
			erase(i++);  
			//splice resuling fragments before current position -> not 
			//to be processed again (works with both cw or countercw b)
			splice_with_bb(i, s);
			break;
		case MscPolygon::B_IS_EMPTY:   //should have been caught above
		case MscPolygon::BOTH_EMPTY:   //oh my
		case MscPolygon::COMPLEMENT:   //cannot be for two positive surfaces
		case MscPolygon::APART_INSIDE: //cannot be for two positive surfaces
			assert(0);
		}
}

//returns true of b as a hole is fully inside one polygon
//and thus shall be added to holes
void PosMscPolygonList::Intersect(const InvMscPolygon &b, InvMscPolygonList &holes)
{
	if (b.size()<3) return;
	if (!boundingBox.Overlaps(b.boundingBox)) 
		return;
	PosMscPolygonList s;
	InvMscPolygonList h;
	for (auto i=begin(); i!=end(); /*none*/)
		switch (i->PolyProcess(b, s, h, false)) {
		case MscPolygon::COMPLEMENT:     //b is exactly our inverse
			erase(i);                    //kill this, but keep all other polygons
			return;
		case MscPolygon::APART_OUTSIDE:  //i is inside the hole -> delete it
		case MscPolygon::A_IS_EMPTY:     //go to next in queue, remove current one
			erase(i++);			
			break;
		case MscPolygon::A_INSIDE_B:     //this one is fully inside b=>keep it
			i++;
			break;                       
		case MscPolygon::APART_INSIDE:   //b (hole) is inside us
			holes.append(b);
			return;         
		case MscPolygon::OK:             //real intersect with our polygon
			erase(i++);  
			//splice resuling fragments before current position -> not 
			//to be processed again 
			splice_with_bb(i, s);
			break;
		case MscPolygon::B_IS_EMPTY:   //should have been caught above
		case MscPolygon::BOTH_EMPTY:   //oh my
		case MscPolygon::SAME:         //cannot be for a pos and an inv polygon
		case MscPolygon::B_INSIDE_A:   //can only happen if b is clockwise, as i is so
			assert(0);
		}
}

void PosMscPolygonList::Intersect(const PosMscPolygonList &b) 
{
	if (!boundingBox.Overlaps(b.boundingBox)) {
		clear();
		return;
	}
	PosMscPolygonList result;
	PosMscPolygonList s;
	InvMscPolygonList h;
	for (auto i=begin(); i!=end(); i++)
		for (auto j=b.begin(); j!=b.end(); j++)
			switch (i->PolyProcess(*j, s, h, false)) {
			case MscPolygon::A_INSIDE_B:     //this one is fully inside b=>keep it
			case MscPolygon::SAME:     
				j = b.end();                 //this i is handled permanently
			case MscPolygon::B_INSIDE_A:     //b is fully in one of our members
			case MscPolygon::OK:             //real intersect with our polygon
				result.append(s);
				break;                       //we will hit no other element
			case MscPolygon::A_IS_EMPTY:
			case MscPolygon::BOTH_EMPTY:   
				j = b.end();
			case MscPolygon::APART_OUTSIDE:  //go to next in queue, remove current one
			case MscPolygon::B_IS_EMPTY:     //no action 
				break;                       
			case MscPolygon::COMPLEMENT:   //cannot be for two positive surfaces
			case MscPolygon::APART_INSIDE: //cannot be for two positive surfaces
			default:
				assert(0);
			}
	swap(result);
}


//this is very much like a union of holes
void InvMscPolygonList::Intersect(const InvMscPolygon &b, PosMscPolygonList &surfaces)
{
	if (b.size()<3) return;
	if (!boundingBox.Overlaps(b.boundingBox)) {
		append(b);
		return;
	}
	PosMscPolygonList s;
	InvMscPolygonList h;
	InvMscPolygon current_blob(b);
	for (auto i=begin(); i!=end();  /*none*/)
		switch (i->PolyProcess(current_blob, s, h, false)) {
		case MscPolygon::OK:             //real union with an existing surface 
			assert(h.size()>0);
			current_blob.swap(*h.begin());   //update blob with a bigger hole 
			//add positive fragments. surfaces formed this way cannot overlap with existing ones
			//since they are fully outside of the original holes
			surfaces.splice_with_bb(s);
			//fallthrough: now delete i
		case MscPolygon::A_IS_EMPTY:
		case MscPolygon::B_INSIDE_A:     // the blob is the bigger hole, delete the smaller one
			erase(i++);
			break;
		case MscPolygon::APART_INSIDE:   //the two holes do not meet: keep them
			i++;
			break;
		case MscPolygon::SAME:           //no change to us
			return;
		case MscPolygon::A_INSIDE_B:     //the blob hole is inside one of our holes
			return;                      //here we need to check for nested holes XXX
		case MscPolygon::B_IS_EMPTY:     //should have been caught above
		case MscPolygon::BOTH_EMPTY:     //oh my
		case MscPolygon::APART_OUTSIDE:  //this can only be for two holes
		case MscPolygon::COMPLEMENT:     //well, b and us both are counterclockwise->this cannot be
		default:
			assert(0);
		}
	append(current_blob);
	return;
}

//return true if b is not touching any hole edges and is inside one fully
//this means b is fully inside one of the holes and will need to be added as an extra positive surface
bool InvMscPolygonList::Union(const PosMscPolygon &b)
{
	if (b.size()<3) return false;
	if (!boundingBox.Overlaps(b.boundingBox)) {
		return false;
	}
	PosMscPolygonList s;
	InvMscPolygonList h;
	bool inside = false;
	bool touched = false;
	for (auto i=begin(); i!=end(); /*none*/)
		switch (i->PolyProcess(b, s, h, true)) {
		case MscPolygon::OK:             //real union with an existing surface 
			assert (s.size()==0);        //union should really be the universe minus some holes in h
			splice_with_bb(i, h);        //insert them before i
			touched = true;
			//fallthrough: now delete i
		case MscPolygon::A_IS_EMPTY:
		case MscPolygon::APART_INSIDE:   //b fully cover the hole->remove it
			erase(i++);
			break;
		case MscPolygon::APART_OUTSIDE:  //b fully inside the hole->flag for return
			inside = true;
		case MscPolygon::B_INSIDE_A:     //b is outside the hole, just move on
			i++;
			break;
		case MscPolygon::B_IS_EMPTY:     //shpould have been caught above
		case MscPolygon::BOTH_EMPTY:     //oh my
		case MscPolygon::SAME:           //well, b is clockwise, we are not->this cannot be
		case MscPolygon::A_INSIDE_B:     //A is inside out, b is not, this should not be
		default:
			assert(0);
		}
	return inside && !touched;
}

void InvMscPolygonList::Union(const InvMscPolygonList &b) 
{
	if (!boundingBox.Overlaps(b.boundingBox)) {
		clear();
		return;
	}
	//this is like intersect of the holes, i will use that wording in comments
	InvMscPolygonList result;
	PosMscPolygonList s;
	InvMscPolygonList h;
	for (auto i=begin(); i!=end(); i++)
		for (auto j=b.begin(); j!=b.end(); j++)
			switch (i->PolyProcess(*j, s, h, true)) {
			case MscPolygon::SAME:           //well, b is clockwise, we are not->this cannot be
			case MscPolygon::B_INSIDE_A:     //hole in i is fully inside hole in j
				j = b.end();
			case MscPolygon::A_INSIDE_B:     //hole in j is fully inside hole in i
			case MscPolygon::OK:             //real union with an existing surface 
				assert (s.size()==0);        //union should really be the universe minus some holes in h
				result.append(h);            //insert them before i
				break;
			case MscPolygon::A_IS_EMPTY:
			case MscPolygon::BOTH_EMPTY:     //skip this hole in a
				j = b.end();
				break;
			case MscPolygon::APART_INSIDE:   //holes do not touch: no intersect
			case MscPolygon::B_IS_EMPTY:     //no action
				break;
			case MscPolygon::APART_OUTSIDE:  //b fully inside the hole->flag for return
			case MscPolygon::COMPLEMENT:     //cannot be for two negative surfaces
			default:
				assert(0);
			}
	swap(result);
}



/////////////////////////////////////////  MscArea implementation


bool MscArea::IsWithin(XY p) const 
{
	if (surfaces.boundingBox.IsWithin(p) == WI_OUTSIDE) return WI_OUTSIDE;
	is_within_t ret;
	for (auto i = holes.begin(); i!= holes.end(); i++) 
		if (WI_INSIDE == (ret = i->IsWithin(p))) continue; //holes are outside-in, this means outside the hole
		else return ret; //on edge, vertex or in a hole (marked by WI_OUTSIDE)
	for (auto i = surfaces.begin(); i!= surfaces.end(); i++) 
		if (WI_OUTSIDE == (ret = i->IsWithin(p))) continue; 
		else return ret; //on edge, vertex or in a surface 
	return WI_OUTSIDE;
}


MscArea &MscArea::operator += (const MscArea &b) 
{
	if (!surfaces.boundingBox.Overlaps(b.surfaces.boundingBox)) {  // do not touch
		surfaces.append(b.surfaces);
		holes.append(b.holes);
		return *this;
	}
	//create a copy of b's holes and substract our surface from them
	InvMscPolygonList new_b_holes = b.holes;
	for (auto i=surfaces.begin(); i!=surfaces.end(); i++)
		new_b_holes.Union(*i);
	//and vice versa
	InvMscPolygonList new_a_holes = holes;
	for (auto i=b.surfaces.begin(); i!=b.surfaces.end(); i++) 
		new_a_holes.Union(*i);
	//create an intersection of holes
	holes.Union(b.holes);
	//splice all holes (they cannot intersect...)
	holes.splice_with_bb(new_a_holes);
	holes.splice_with_bb(new_b_holes);
	//Add all of b's surfaces to us 
	for (auto i=b.surfaces.begin(); i!=b.surfaces.end(); i++) 
		surfaces.Union(*i, holes);  //extra holes appended to "holes" by Union

	mainline += b.mainline;
	return *this;
}

MscArea &MscArea::operator *= (const MscArea &b) 
{
	if (!surfaces.boundingBox.Overlaps(b.surfaces.boundingBox)) {  // do not touch
		surfaces.clear();
		holes.clear();
		return *this;
	}
	PosMscPolygonList collect, tmp;
	//create an intersect of the positive surfaces
	surfaces.Intersect(b.surfaces);

	//substract both holesets (dropping unnecessary holes in the process)
	InvMscPolygonList new_b_holes = b.holes;
	surfaces.NormalizeWith(holes);
	surfaces.NormalizeWith(new_b_holes);
	//Now combine the two holesets
	PosMscPolygonList dummy;
	for (auto i=new_b_holes.begin(); i!=new_b_holes.end(); i++)
		holes.Intersect(*i, dummy);  
	surfaces.NormalizeWith(holes);

	mainline *= b.mainline;
	return *this;
}


MscArea &MscArea::operator -= (const MscArea &b) 
{
	if (!surfaces.boundingBox.Overlaps(b.surfaces.boundingBox)) {  // do not touch
		return *this;
	}

	//first substract the surfaces of b from us
	for (auto i=b.surfaces.begin(); i!=b.surfaces.end(); i++)
		surfaces.Substract(*i, holes);  
	//calculate pieces of our surface that fell into holes of b
	PosMscPolygonList surf_fragments(surfaces);
	for (auto i=b.holes.begin(); i!=b.holes.end(); i++)
		surf_fragments.Substract(*i);    //intersection of holes and surfaces
	//combine the two surface sets
	for (auto i=surf_fragments.begin(); i!=surf_fragments.end(); i++) 
		surfaces.Union(*i, holes);  //extra holes appended to "holes" by Union
	//now limit the holes to the new surface
	surfaces.NormalizeWith(holes);

	//substract mainlines, too
	//if any of our from or till falls into g's mainline, act.
	if (b.mainline.IsWithin(mainline.from)) mainline.from=b.mainline.till; 
	if (b.mainline.IsWithin(mainline.till)) mainline.till=b.mainline.from;
	//we may end up being invalid (=empty) (=till<from);
	return *this;
}
void MscArea::Line(cairo_t *cr) const
{
	double dash[] = {2, 2};
	holes.Path(cr);
	cairo_set_dash(cr, dash, 2, 0);
	cairo_stroke(cr);
	surfaces.Path(cr);
	cairo_set_dash(cr, dash, 0, 0);
	cairo_stroke(cr);
}

void MscArea::Fill(cairo_t *cr) const
{
	surfaces.Path(cr);
	holes.Path(cr);
	cairo_fill(cr);
}

void MscArea::swap(MscArea &a) 
{
	surfaces.swap(a.surfaces); 
	holes.swap(a.holes); 
	std::swap(mainline, a.mainline);
	std::swap(arc, a.arc);
    std::swap(drawType, a.drawType);
    std::swap(findType, a.findType);
}
