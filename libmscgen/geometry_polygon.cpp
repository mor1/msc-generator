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
    double       pos;
    XY           xy;               //coordinates of the crosspoint
    cp_pointer_t other;            //the location of the crosspoint in the other polygon
    bool         switch_to_other;  //true if we need to swicth to the other polygon (set by AddCrosspoint)
    bool         stop_here;        //will be set true in the crosspoint we start walking
    bool         valid;            //after processing, this is no longer valid
    cp_t(double p, const XY &a, const cp_pointer_t &b, bool sw) :
        pos(p), xy(a), other(b), switch_to_other(sw), stop_here(false), valid(true) {}
        bool operator <(const cp_t &o) const {return pos<o.pos;}
};

inline bool operator <(const cp_t &cp, double p) {return cp.pos < p;}
inline bool operator <(double p, const cp_t &cp) {return p < cp.pos;}

struct cp_list_t : public std::vector<cp_t>
{
    const cp_t &find(double pos) const {const_iterator i = lower_bound(begin(), end(), pos); return *i;}
    cp_t &find(double pos)       {      iterator i = lower_bound(begin(), end(), pos); return *i;}
    double next_valid(double pos) const;
};


inline double cp_list_t::next_valid(double pos) const
{
    const_iterator i = upper_bound(begin(), end(), pos);
    while (i!=end() && !i->valid) i++;
    return i==end() ? -1 : i->pos;
}


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
    const Polygon *_polygon;
protected:
    const Polygon &GetPolygon() const {_ASSERT(_polygon); return *_polygon;}
    MscCrossPointStore() : _polygon(NULL) {}
    void Set (const Polygon &p);
    void Sort();
    void Finish();

    const cp_t& Lookup(const cp_pointer_t &pCP) const {return at(pCP.vertex).find(pCP.pos);}
    cp_t& Lookup(const cp_pointer_t &pCP)       {return at(pCP.vertex).find(pCP.pos);}
    void Add(const XY& xy, const cp_pointer_t &us, const cp_pointer_t &other, bool sw);
    void RemoveBothIfNeeded(cp_pointer_t const &pCP, MscCrossPointStore &other_cp);

    cp_pointer_t First(void) const;  //returns vertex == -1 if we are empty
    cp_pointer_t NextCP(cp_pointer_t const &pCP) const;       //Gives the next crosspoint
    Edge GetEdgeRemainder(cp_pointer_t pCP) const {
        if (pCP.pos == -1) return GetPolygon()[pCP.vertex];
        return Edge(GetPolygon()[pCP.vertex]).SetStart(Lookup(pCP).xy, pCP.pos);
    }

    typedef enum {
        //This type represents what type of crosspoint it is
        //o represents the cp, if two that is only ascii limitation
        //dots (.) represent areas polygon one
        //commas (,) represent areas polygon two
        //colons (:) represent areas of intersect
        //numbers indicate which polygon is which

        COMES_SAME_ONE_OUT, COMES_SAME_TWO_OUT, GOES_SAME_TWO_OUT, GOES_SAME_ONE_OUT, SAME,
        //  1^,,,,            2^,,,,            ,\2                .\1
        //   |,,,,             |,,,,            ,,\                ..\
        //===o--->2         ===o--->1           ,,,o===>           ...o===>           o===>
        //::::::::          ::::::::            ,,/:::::           ../:::::           /:::::
        //::::::::          ::::::::            ,/1:::::           ./2:::::          /21::::

        TOUCH_POINT_OUTSIDE, TOUCH_POINT_INSIDE, TOUCH_POINT_INSIDE_TWO_OUT, TOUCH_POINT_INSIDE_ONE_OUT,
        //..\1 2->,          1<-:::/2            ::\1,,/2                    ::\2,,/1
        //...\ /,,,             \:/              :::\,/                      :::\,/
        //....o,,,,              o               ::::o-                      ::::o-
        //.../ \,,,             /:\              :::/,,\                     :::/,,\
        //1<-   \2,           1/:::->2           1<-,,<-2                    2<-,,<-1

        OPPOSITE_ONE_GOES_AWAY, OPPOSITE_TWO_GOES_AWAY, OPPOSITE_ONE_GOES_IN, OPPOSITE_TWO_GOES_IN,
        //2\   ->1              1\   ->2                 ,,,|2                 ,,,|1
        //,,\ /..               ,,\ /..                  ,,,|                  ,,,|
        //,,oo...               ,,oo...                 1<--o+                2<--o+
        //,,||...               ,,||...                  :::||                 :::||
        //,,V|...               ,,V|...                  :::V|                 :::V|

        OPPOSITE_COMPLEMENT, SIMPLE_CROSS_TWO_OUT, SIMPLE_CROSS_ONE_OUT, WRONG_END} cp_category_t;
        //..^|2,,,           ::::|....             ::::|....             The crosspoint
        //..||,,,,           ::::|....             ::::|....             falls on the
        //..|o--->           <---o---1             <---o---2             endpoint of one
        //..o----1           ,,,,|                 ,,,,|                 of the edges.
        //........           ,,,,V2                ,,,,V1                (We ignore these.)

    static cp_category_t CategorizeCrosspoint(int one_i, int two_j, XY p, double one_pos, double two_pos,
                                              const Polygon &one, const Polygon &two,
                                              cp_pointer_t &one_pCP, cp_pointer_t &two_pCP);

    //find crosspoints between polygon of one and two, deposint them in *_cp. return true if any
    static bool AddCrosspoint(int one_i, int two_j, XY p,double one_pos, double two_pos,
                              bool do_union, MscCrossPointStore &one_cp, MscCrossPointStore &two_cp);
    static int  FindCrosspoints(bool do_union, MscCrossPointStore &one_cp, MscCrossPointStore &two_cp);
    //find crosspoints of the polygon with itself deposint them in this. return true if any
    bool AddCrosspointSelf(int one_i, int two_j, XY p,  double one_pos, double two_pos);
    int  FindCrosspointsSelf();
};

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

//Delete crosspoint (also from other store), if both swicth_to_other flags are false
void MscCrossPointStore::RemoveBothIfNeeded(cp_pointer_t const &pCP, MscCrossPointStore &other_cp)
{
    cp_t &mine = at(pCP.vertex).find(pCP.pos);
    cp_t &yours= other_cp.at(mine.other.vertex).find(mine.other.pos);
    if (!mine.switch_to_other && !yours.switch_to_other) {
        mine.valid=false;
        yours.valid=false;
    }
}


inline void MscCrossPointStore::Add(const XY &xy, const cp_pointer_t &us, const cp_pointer_t &other, bool sw)
{
    _ASSERT(size() >= _polygon->size());
	for (int i=0; i<at(us.vertex).size(); i++)
		if (test_equal(at(us.vertex)[i].pos, us.pos))
			_ASSERT(0);
    at(us.vertex).push_back(cp_t(us.pos, xy, other, sw));
}

cp_pointer_t MscCrossPointStore::First(void) const
{
    for (int i=0; i<_polygon->size(); i++)
        for (int j=0; j<at(i).size(); j++)
            if (at(i)[j].valid)
                return cp_pointer_t(i, at(i)[j].pos);
    return cp_pointer_t(-1, -1);
}

cp_pointer_t MscCrossPointStore::NextCP(const cp_pointer_t &pCP) const
{
    double pos = at(pCP.vertex).next_valid(pCP.pos);
    if (pos == -1)
        return cp_pointer_t((pCP.vertex+1) % _polygon->size(), -1);  //no next crosspoint
    else
        return cp_pointer_t(pCP.vertex, pos); //there is a next crosspoint
}


MscCrossPointStore::cp_category_t
MscCrossPointStore::CategorizeCrosspoint(int one_i, int two_j, XY p, double one_pos, double two_pos,
                                         const Polygon &one, const Polygon &two,
                                         cp_pointer_t &one_pCP, cp_pointer_t &two_pCP)
{
    //if the crosspoint falls on any ending vertex, skip it, it will pop up for the next edge as the starting vertex
    //contain is not set, when we need that value in containment, we sould not return here
    if (test_equal(one_pos, 1) || test_equal(two_pos, 1)) return WRONG_END;
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

    //check for case #2
    if (test_equal(one_two_angle, one_one_angle)) { //two outgoing edges equal
		if (test_smaller(one_two_angle, two_two_angle))
			return GOES_SAME_ONE_OUT;
		if (test_smaller(two_one_angle, one_one_angle))
			return GOES_SAME_TWO_OUT;
        return SAME;
	}
    if (test_zero(one_two_angle)) {  //incoming of one == outgoing of two
        if (test_smaller(4-two_two_angle, one_one_angle))  
            return OPPOSITE_ONE_GOES_AWAY;
        if (test_smaller(one_one_angle, 4-two_two_angle))  
            return OPPOSITE_ONE_GOES_IN;
		return OPPOSITE_COMPLEMENT;

    }
    if (test_zero(two_one_angle)) {  //incoming of two == outgoing of one
        if (test_smaller(4-one_one_angle, two_two_angle))  
            return OPPOSITE_TWO_GOES_AWAY;
		//OPPOSITE_COMPLEMENT if below assert not true and we should have caught it above
		_ASSERT(test_smaller(two_two_angle, 4-one_one_angle));
		return OPPOSITE_TWO_GOES_IN;//two goes inside polygon one
    }

    //here we know none of the incoming edges are the same ad the outgoing of the other
    //(well, we did not check for one_one_angle or two_two_angle for zero
    //whch would mean a degenerate polynom, since we assume we get good polynoms)
    if (test_smaller(one_two_angle, one_one_angle)) {  //case #1 for two: two goes outside polygon one
        if (test_smaller(two_one_angle, two_two_angle))
			return TOUCH_POINT_OUTSIDE;

        if (test_smaller(one_two_angle, two_two_angle)) //the two polygons simply cross
			return SIMPLE_CROSS_TWO_OUT;
        if (test_smaller(two_two_angle, one_two_angle)) //inside touch
            return TOUCH_POINT_INSIDE_TWO_OUT;
        //if (one_two_angle == two_two_angle) //the two incoming edges are the same
        return COMES_SAME_TWO_OUT;
    }

    if (test_smaller(two_one_angle, two_two_angle)) {  //same a above three cases but 1 and 2 reversed
        if (test_smaller(two_one_angle, one_one_angle)) //the two polygons simply cross
			return SIMPLE_CROSS_ONE_OUT;
        if (test_smaller(one_one_angle, two_one_angle)) //inside touch
            return TOUCH_POINT_INSIDE_ONE_OUT;
        //if (two_one_angle == one_one_angle) //the two incoming edges are the same
        return COMES_SAME_ONE_OUT;
    }
    return TOUCH_POINT_INSIDE;
}

//returns if a crosspoint has actually been added or not
inline bool MscCrossPointStore::AddCrosspoint(int one_i, int two_j, XY p,  double one_pos, double two_pos,
                                              bool do_union, MscCrossPointStore &one_cp, MscCrossPointStore &two_cp)
{
    cp_pointer_t one_pCP, two_pCP;
    cp_category_t cat = CategorizeCrosspoint(one_i, two_j, p, one_pos, two_pos,
                                             one_cp.GetPolygon(), two_cp.GetPolygon(),
											 one_pCP, two_pCP);
    int which; //set this to 1 or 2 depending on which edge we shall continue from this cp
    if (do_union)
        switch (cat) {
        case WRONG_END:
        case OPPOSITE_COMPLEMENT:
        case OPPOSITE_ONE_GOES_IN:
        case OPPOSITE_TWO_GOES_IN:
        case TOUCH_POINT_OUTSIDE:
        case TOUCH_POINT_INSIDE:
        case TOUCH_POINT_INSIDE_ONE_OUT:
        case TOUCH_POINT_INSIDE_TWO_OUT:
		case GOES_SAME_ONE_OUT:
		case GOES_SAME_TWO_OUT:
		case SAME:
            return false;
        case OPPOSITE_ONE_GOES_AWAY:
        case SIMPLE_CROSS_ONE_OUT:
        case COMES_SAME_ONE_OUT:
            which = 1;
            break;
        case OPPOSITE_TWO_GOES_AWAY:
        case SIMPLE_CROSS_TWO_OUT:
        case COMES_SAME_TWO_OUT:
            which = 2;
            break;
        default:
            _ASSERT(0);
        }
    else switch (cat) {
		case WRONG_END:
		case OPPOSITE_COMPLEMENT:
		case OPPOSITE_ONE_GOES_AWAY:
		case OPPOSITE_TWO_GOES_AWAY:
		case TOUCH_POINT_OUTSIDE:
		case TOUCH_POINT_INSIDE:
		case GOES_SAME_ONE_OUT:
		case GOES_SAME_TWO_OUT:
		case SAME:
			return false;
		case OPPOSITE_ONE_GOES_IN:
		case TOUCH_POINT_INSIDE_TWO_OUT:
		case SIMPLE_CROSS_TWO_OUT:
		case COMES_SAME_TWO_OUT:
			which = 1;
			break;
		case OPPOSITE_TWO_GOES_IN:
		case TOUCH_POINT_INSIDE_ONE_OUT:
		case SIMPLE_CROSS_ONE_OUT:
		case COMES_SAME_ONE_OUT:
			which = 2;
			break;
		default:
			_ASSERT(0);
		}
    one_cp.Add(p, one_pCP, two_pCP, which==2);
    two_cp.Add(p, two_pCP, one_pCP, which==1);
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
    for (int i = 0; i<one_cp.GetPolygon().size(); i++)
        for (int j = 0; j<two_cp.GetPolygon().size(); j++) {
            int n=Edge::Crossing(one_cp.GetPolygon()[i], one_cp.GetPolygon().at_next(i).GetStart(),
                                 two_cp.GetPolygon()[j], two_cp.GetPolygon().at_next(j).GetStart(), r, one_pos, two_pos);
            for (int k=0; k<n;k++)
                num += AddCrosspoint(i, j, r[k], one_pos[k], two_pos[k], do_union, one_cp, two_cp);
        }
	if (num>1) {
		one_cp.Sort();
		two_cp.Sort();
	}
    return num;
}

//returns if a crosspoint has actually been added or not
inline bool MscCrossPointStore::AddCrosspointSelf(int one_i, int two_j, XY p, double one_pos, double two_pos)
{
    cp_pointer_t one_pCP, two_pCP;
    int which; //set it to 1 or 2 depending on which edge to go out. Set it to 0 if both.
    switch (CategorizeCrosspoint(one_i, two_j, p, one_pos, two_pos, *_polygon, *_polygon, one_pCP, two_pCP)) {
    case WRONG_END:
	case GOES_SAME_ONE_OUT:
	case GOES_SAME_TWO_OUT:
	case SAME:
    case COMES_SAME_ONE_OUT:
    case COMES_SAME_TWO_OUT:
    case OPPOSITE_COMPLEMENT:
    case OPPOSITE_ONE_GOES_IN:
    case OPPOSITE_TWO_GOES_IN:
    case TOUCH_POINT_INSIDE_ONE_OUT:
    case TOUCH_POINT_INSIDE_TWO_OUT:
        return false;
    case TOUCH_POINT_OUTSIDE:
    case TOUCH_POINT_INSIDE:
    case SIMPLE_CROSS_ONE_OUT:
    case SIMPLE_CROSS_TWO_OUT:
        which = 0;
        break;
    case OPPOSITE_ONE_GOES_AWAY:
        which = 1;
        break;
    case OPPOSITE_TWO_GOES_AWAY:
        which = 2;
        break;
    default:
        _ASSERT(0);
    }
    Add(p, one_pCP, two_pCP, which!=1);
    Add(p, two_pCP, one_pCP, which!=2);
    return true;
}



//Find crosspoints of edges of a single polygon
int MscCrossPointStore::FindCrosspointsSelf()
{
    XY r[4];
    double one_pos[4], two_pos[4];
    bool was = false;
    int num=0;
    for (int i = 1; i<GetPolygon().size(); i++)
        for (int j = 0; j<i; j++) {
            int n=Edge::Crossing(GetPolygon()[i], GetPolygon().at_next(i).GetStart(),
                                 GetPolygon()[j], GetPolygon().at_next(j).GetStart(), r, one_pos, two_pos);
            for (int k=0; k<n;k++)
                num += AddCrosspointSelf(i, j, r[k], one_pos[k], two_pos[k]);
        }
	if (num>1)
		Sort();
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

//Can result SAME, APRT, A_INSIDE_B or B_INSIDE_A
//It can also return OK, which means one of our point is outside b, can either be APART or b may be in us
Polygon::poly_result_t Polygon::CheckContainmentHelper(const Polygon &b) const
{
    int edge;
    poly_result_t retval = A_INSIDE_B;
    for (int i=0; i<size(); i++) {
		double pos;
        //if we are a single ellipsis, use our center point, else use a vertex
        switch (b.IsWithin(at(i).GetStart(), &edge, &pos)) {
        default:
            _ASSERT(0);
        case WI_INSIDE:  return A_INSIDE_B;
        case WI_OUTSIDE: return OK;
        case WI_ON_VERTEX:
			pos = 0; //if we are on b's vertex, this is the right pos for that edge
        case WI_ON_EDGE:
            cp_pointer_t dummy1, dummy2;
            switch (MscCrossPointStore::CategorizeCrosspoint(i, edge, at(i).GetStart(), 0, pos, 
                                                             *this, b, dummy1, dummy2)) {       
            case MscCrossPointStore::COMES_SAME_ONE_OUT:
            case MscCrossPointStore::OPPOSITE_TWO_GOES_IN:
            case MscCrossPointStore::TOUCH_POINT_INSIDE_ONE_OUT:
			case MscCrossPointStore::GOES_SAME_ONE_OUT: 
                return B_INSIDE_A;
            case MscCrossPointStore::COMES_SAME_TWO_OUT:
            case MscCrossPointStore::OPPOSITE_ONE_GOES_IN:
            case MscCrossPointStore::TOUCH_POINT_INSIDE_TWO_OUT:
			case MscCrossPointStore::GOES_SAME_TWO_OUT:
                return A_INSIDE_B;
            case MscCrossPointStore::OPPOSITE_COMPLEMENT:
			case MscCrossPointStore::TOUCH_POINT_OUTSIDE:
			case MscCrossPointStore::OPPOSITE_ONE_GOES_AWAY:
			case MscCrossPointStore::OPPOSITE_TWO_GOES_AWAY:
                return APART;
			case MscCrossPointStore::SAME:
			case MscCrossPointStore::TOUCH_POINT_INSIDE:
                break; //inconclusive
			default:
			case MscCrossPointStore::SIMPLE_CROSS_ONE_OUT:
			case MscCrossPointStore::SIMPLE_CROSS_TWO_OUT:
			case MscCrossPointStore::WRONG_END:
                _ASSERT(0); //should not happen
            }
        }
	}
    //All points were on a vertex and reported either GOES_SAME or TOUCH_POINT_INSIDE
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
    if (this_in_other != OK) return this_in_other;
    switch (other.CheckContainmentHelper(*this)) {
    default:
    case SAME: _ASSERT(0); return SAME;
    case OK:
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
void Polygon::PolyProcessWalk(MscCrossPointStore *poly, MscCrossPointStore *poly_o, PolygonList &surfaces, PolygonList &holes)
{
    surfaces.clear();
    holes.clear();
    //OK, we have crosspoints relevant for the operation (union, intersection) we do
    //Start the walks around the polygon, as long as there are crosspoints left
    //The poly.cp->First on the next iteration will give a -1 vertex back if there is no more CPs
    for (cp_pointer_t current = poly->First(); current.vertex>=0; current = poly->First()) {
        static PolygonWithHoles result;
        result.clear();
        result.reserve(2*(poly->GetPolygon().size()+poly_o->GetPolygon().size()));

        //do a walk from the current
        do {
            Edge current_xy_edge;

            if (current.pos < 0) {
                //this is a vertex not crossed by the other polygon, we keep walking this one
                current_xy_edge = poly->GetEdgeRemainder(current);
                current = poly->NextCP(current);
            } else {
                //this is a crosspoint let's see if we switch to the other polygon
                cp_pointer_t other = poly->Lookup(current).other;
                if (poly->Lookup(current).switch_to_other) {
                    std::swap(current, other);
                    std::swap(poly, poly_o);
                }
                //Ok, we will insert this edge
                current_xy_edge = poly->GetEdgeRemainder(current);

                //for PolyProcess: 1) only one of the switch_to_others is true, 2) we need to delete crosspoint
                //for PolyProcessSelf: 1) initially both switch_to_others are true, 2) we shall visit this cp twice,
                //   but leave it on different edges each time (and delete after second try)
                //So we since we continue on poly: we set poly_o->switch_to_other to false
                //   and delete the cp if both are false
                poly_o->Lookup(other).switch_to_other = false;

                //remove crosspoints if needed and advance to next point
                cp_pointer_t next = poly->NextCP(current);
                if (result.size() == 0) { //mark exit condition
                    poly->Lookup(current).stop_here = true;
                    poly_o->Lookup(other).stop_here = true;
                } else {
                    poly->RemoveBothIfNeeded(current, *poly_o);
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
        } while (current.pos ==-1 || !poly->Lookup(current).stop_here);

        //remove the crosspoint we started and halted at
        poly->RemoveBothIfNeeded(current, *poly_o);

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
    }
};

//Does a union or intersection of two poligons, stores the result in "surfaces"
//- For union "surfaces" will be one bounding polygon, its "holes" may have zero or more elements
//- For intersection "surfaces" may have more elements, their "holes" are guaranteed to be empty
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
    if (size()==0 && b.size()==0) return BOTH_EMPTY;
    if (size()==0) return A_IS_EMPTY;
    if (b.size()==0) return B_IS_EMPTY;

    if (!boundingBox.Overlaps(b.boundingBox))
        return APART;
    //Set up some data structures
    static Polygon b_inv;
    /*static*/ MscCrossPointStore cp_one, cp_two;
    cp_one.Set(*this);
    cp_two.Set(action==SUBSTRACT ? b_inv = b.GetInverse() : b); //invert b if action is SUBSTRACT

    //We will first find all the crossing points between the two polygons
    Polygon::poly_result_t ret = OK;
    if (MscCrossPointStore::FindCrosspoints(action==UNION, cp_one, cp_two)) {
        static PolygonList holes;
        PolyProcessWalk(&cp_one, &cp_two, surfaces, holes); //they touch, walk the pieces
        //With union we may have holes, but only one surface
        //With intersect (and substract), we can have multiple surfaces, but no holes
        _ASSERT(surfaces.size()==1 || holes.size()==0);
        if (holes.size())
            surfaces.begin()->holes.swap(holes);
    } else
        ret = CheckContainment(b); //two polygons do not touch, determine their relative position
    cp_one.Finish();
    cp_two.Finish(); //keep memory allocated, but erase pointers to input polygons
    return ret;
}

//Checks if the polygon touches itself and splits it into multiple pieces and holes
//Can return
//- A_IS_EMPTY, if we are empty
//- SAME, if there is nothing to untangle (nothing is in surfaces)
//- OK if there were intersections (or is counterclockwise) and they are placed in surfaces
Polygon::poly_result_t Polygon::PolyProcessSelf(PolygonList &surfaces, bool add_separate_holes, bool check_clockwise) const
{
    surfaces.clear();
    if (size()==0) return A_IS_EMPTY;

    //Set up some data structures,
    static Polygon inv_poly;
    static MscCrossPointStore cp;
    bool c = !check_clockwise || CalculateClockwise();   //assume clockwise if check==false
    cp.Set(!c ? inv_poly = GetInverse() : *this); //invert us not clockwise

    //We will first find all the crossing points between our edges
    //neighbouring edges will not be stored since one of them will have pos==1
    Polygon::poly_result_t ret = OK;
    if (cp.FindCrosspointsSelf()) {
        static PolygonList s, h;
        PolyProcessWalk(&cp, &cp, s, h); //they touch, walk the pieces
        //combine the surfaces
        for (auto i=s.begin(); i!=s.end();i++)
            surfaces += *i;
        //substract the holes. If any hole is fully apart from the surfaces, add it as a positive surface
		//optimize for the case when surfaces is empty
		if (surfaces.size()==0) {
			if (add_separate_holes) surfaces.swap(h);
		} else {
			for (auto i=h.begin(); i!=h.end();i++) {
				PolygonList new_surfaces;
				bool all_apart=add_separate_holes;
				for (auto j=surfaces.begin(); j!=surfaces.end(); j++) {
					PolygonList res;
					if (APART != j->Sub(*i, res))
						all_apart=false;
					new_surfaces.append(std::move(res));
				}
				if (all_apart) new_surfaces.append(*i);
				surfaces.swap(new_surfaces);
			}
        }
    } else if (c) ret = SAME; //dont touch and clockwise
    else {                    //dont touch and clounterclockwise
        ret = OK;
        surfaces.append(PolygonWithHoles(inv_poly));
    }
    cp.Finish(); //keep memory allocated, but erase pointers to input polygons
    return ret;
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
        for (int j = 0; j<b.size(); j++) {
            int num = Edge::Crossing(at(i), at_next(i).GetStart(),b[j], b.at_next(j).GetStart(),
                                     r, one_pos, two_pos);
            for (int k=0; k<num; k++)
                switch (MscCrossPointStore::CategorizeCrosspoint(i, j, r[k], one_pos[k], two_pos[k],
                                                                 *this, b, dummy1, dummy2)) {
                case MscCrossPointStore::OPPOSITE_ONE_GOES_IN:
                case MscCrossPointStore::TOUCH_POINT_INSIDE_TWO_OUT:
                case MscCrossPointStore::SIMPLE_CROSS_TWO_OUT:
                case MscCrossPointStore::COMES_SAME_TWO_OUT:
                case MscCrossPointStore::OPPOSITE_TWO_GOES_IN:
                case MscCrossPointStore::TOUCH_POINT_INSIDE_ONE_OUT:
                case MscCrossPointStore::SIMPLE_CROSS_ONE_OUT:
                case MscCrossPointStore::COMES_SAME_ONE_OUT:
                    return true; //all cases when an intersection gives a crosspoint
                }
        }
    return false;
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
		Polygon result;
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
			if (SAME==result.PolyProcessSelf(res, false, false)) 
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


//void Polygon::Expand(double gap, PolygonList &res) const
//{
//    if (gap==0 || size()==0) return;
//	if (boundingBox.x.Spans() < -2*gap || boundingBox.y.Spans() < -2*gap) return;
//
//	typedef struct {
//		Edge s;
//		XY   e;
//		bool substitue_exists;
//		Edge subst_s;
//		XY   subst_e;
//	} EdgeXY;
//
//	static std::vector<EdgeXY> edges1, edges2;
//	static Polygon result;  
//	static std::vector<Ellipse> extra_ellipses;
//	edges1.resize(size());
//	edges2.reserve(size());
//	result.reserve(size());
//	//calculate 2 points for each angle (expand the vertices)
//	int orig_size = 0;
//	for (int i=0; i<size(); i++) 
//		if (at(i).ExpandEdge(gap, at_next(i).start, edges1[orig_size].s, edges1[orig_size].e))
//			orig_size++;
//	extra_ellipses.clear();
//	//Now remove edges that got reversed as long as there are none such
//	do {
//		orig_size = edges1.size();
//		edges2.clear();
//		result.clear();
//		//calculate the new vertices
//		for (int i=0; i<edges1.size(); i++) {
//			std::vector<EdgeXY>::reverse_iterator iPrev = edges2.size() ? edges2.rbegin() : edges1.rbegin();
//			//the following call modifies *iPrev if it is a curve ("e" the ending radian.
//			//it is Ok to modify it even if it belongs to edges1
//			switch (iPrev->s.CombineExpandedEdges(iPrev->e, edges1[i].s, edges1[i].e, result)) {
//			case 2:
//				//we add two edges: the one added by the two points and edges[i]
//				edges2.resize(edges2.size()+1);
//				edges2.rbegin()->s = result[result.size()-2];      
//				edges2.rbegin()->e = result[result.size()-1].start;
//				edges2.rbegin()->substitue_exists = false;
//				//fallthrough
//			case 1:
//				edges2.push_back(edges1[i]);
//				edges2.rbegin()->substitue_exists = false;
//				break;
//			case 0:
//				//the last added edge and this one is parallel. We need to add only one of them.
//				//we do not know which. So we add both (one as a substitute), if the direction of points
//				//(calculated below) is wrong for one, we use the other. 
//				//if the previous was not already added, we add it now (and remove duplicates at the end)
//				if (edges2.size()==0) 
//					edges2.push_back(*iPrev); 
//				edges2.rbegin()->substitue_exists = true;
//				edges2.rbegin()->subst_s = edges1[i].s;
//				edges2.rbegin()->subst_e = edges1[i].e;
//				break;
//			case -1: //A circle not matching
//				//add it to extras otherwise drop it
//				if (!edges1[i].s.IsStraight())
//					extra_ellipses.push_back(edges1[i].s.GetEllipse());
//				else {
//					//we are line: remove previous ellipse
//					extra_ellipses.push_back(iPrev->s.GetEllipse());
//					if (edges2.size()) 
//						edges2.pop_back();
//					//add current edge (line);
//					edges2.push_back(edges1[i]);
//					edges2.rbegin()->substitue_exists = false;
//				}
//			}
//		}
//		//Check if last one equals to first one and remove
//		if (edges2.rbegin()->s == edges2.begin()->s && edges2.rbegin()->e == edges2.begin()->e)
//			edges2.pop_back();
//
//		//remove edges that changed direction
//		edges1.clear();
//		for (int i=0; i<edges2.size(); i++) 
//			if (same_dir(result[i].start, result.at_next(i).start, edges2[i].s.start, edges2[i].e)) 
//				edges1.push_back(edges2[i]);
//			else if (edges2[i].substitue_exists) { //use substitute if exists
//				edges2[i].s = edges2[i].subst_s;
//				edges2[i].e = edges2[i].subst_e;
//				//force one more cycle immediately
//				edges2.swap(edges1);
//				orig_size++;
//				break;
//			}
//		//if no edges have been removed during this cycle then the polygon in result is OK
//	} while(edges1.size() < orig_size && 0 < edges1.size());
//	//result1 contains the final polygon
//	if (edges1.size()==0) {
//		res.clear();
//		return;
//	}
//	result.CalculateBoundingBox();  //also calculates bounding boxes of edges
//	if (SAME==result.PolyProcessSelf(res, false, false)) 
//		res.append(result);        //expanded polygon is not tangled, just add it
//}
