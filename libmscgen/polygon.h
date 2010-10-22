#if !defined(POLYGON_H)
#define POLYGON_H

#include <set>
#include <climits>
#include "error.h" //for file_line
#include "trackable.h"
#include "cairo.h"

//An edge of a polygon: either straight or curvy
//for a straight edge we only hold the startpoint, endpoint comes from next edge
//for a straight edge, radius1==0, start contains the startpoint, other fields undefined
//for a curvy edge it is an ellipsis having "center" as centerpoint,
//radius1 and radius2 as x and y direction radiuses, from radian s to radian e
//rotated by tilt around center
class PolyEdge
{
protected:
	friend class MscPolygon;
	//these must be called only if we are sure we are curvy
	static int straight_crossing(const XY A, const XY B, const XY M, const XY N, XY&r1, XY &r2);
	static int curvy_crossing(const PolyEdge &A, const PolyEdge &B, XY *r1);
	       int curvy_segment_crossing(const XY &A, const XY &B, XY &r1, XY &r2) const;
	XY         curvy_prevnexttangentpoint(double pos, bool next) const;
	XY         conv_to_circle_space(const XY &p) const;
	XY         conv_to_real_space(const XY &p) const;
	XY         RadianToPoint(double r) const;
	double     PointToRadian(XY p, is_within_t *inout=NULL) const;
	static double RadianSpan(double s, double e);
	bool       RadianBetween(double r) const {return RadianSpan(s, r) <= RadianSpan(s, e);}

public:
	XY     start;
	XY     center;
	double tilt;   //supposedly between [0..pi)
	double s, e;   //supposedly between [0..2pi]
	bool   clockwise_arc;  //this is clocwise if y is downwards
	double radius1, radius2;

	PolyEdge() : radius1(0) {}
	PolyEdge(const XY &xy) : start(xy), radius1(0) {}
	PolyEdge(const XY &c, double radius_x, double radius_y=0, double tilt=0);
	bool     IsStraight() const {return radius1==0;}
	double   StartPos() const {return IsStraight() ? 0 : s;}
	double   EndPos() const {return IsStraight() ? 1 : e;}

	PolyEdge &   operator +=(XY wh)             {start+=wh; center+=wh; return *this;}
	PolyEdge &   operator -=(XY wh)             {start-=wh; center-=wh; return *this;}
	bool   operator ==(const PolyEdge& p) const; 
	bool   operator < (const PolyEdge& p) const; 

	//Gives the intersecting points of AB and MN
	static int Crossing(const PolyEdge &A, const XY &B, const PolyEdge &M, const XY &N, XY *r);
	//Tells where P is on the this->end section or curve, for straight it is [0..1] for curves it is [0..2pi)
	double     RelativePos(const XY &P, const XY &end) const;
	//tells where does this->B cross the horizontal line at y
	int        CrossingHorizontal(double y, const XY &B, double &r1, double &r2) const;
	//Removes the part of the edge or curve before point p. Assumes p lies on us.
	PolyEdge&  RemoveBeforePoint(const XY &p) {start = p; if (!IsStraight()) {is_within_t inout; s = PointToRadian(p, &inout);
		assert(inout==WI_ON_VERTEX || inout==WI_ON_EDGE);} return *this;}
	//Removes the part of the edge or curve after point p. Assumes p lies on us.
	PolyEdge&  RemoveAfterPoint(const XY &p) {if (!IsStraight()) {is_within_t inout; e = PointToRadian(p, &inout);
		assert(inout==WI_ON_VERTEX || inout==WI_ON_EDGE);}; return *this;}
	//Adds this point (for straight) or full curve to a bounding box
	Block&     AddToBoundingBox(Block &b) const {return IsStraight() ? b+=start : b+=Block(-radius1,radius1, -radius2,radius2).Shift(center);} 
	//returns a point on the line of a tangent at "pos", the point being towards the start of curve/edge.
	XY         PrevTangentPoint(double pos, const PolyEdge &prev_vertex) const;
	//returns a point on the line of a tangent at "pos", the point being towards the end of curve/edge.
	XY         NextTangentPoint(double pos, const PolyEdge &next_vertex) const;
	//check if next->after is a direct continuation of this->next and (if a curve) combine it into this, if yes
	bool       CheckAndCombine(const PolyEdge &next, const XY &after);
};

inline bool PolyEdge::operator ==(const PolyEdge& p) const 
{
	if (start!=p.start || radius1!=p.radius1) return false;
	if (IsStraight() || p.IsStraight()) return true;
	return s==p.s && e==p.e && tilt==p.tilt && radius2==p.radius2 && clockwise_arc==p.clockwise_arc;
}

inline bool PolyEdge::operator < (const PolyEdge& p) const  
{
	if (start!=p.start) return start<p.start;
	if (radius1!=p.radius1) return radius1 < p.radius1;
	if (radius1==0) return false; //both are zero, both are straight, they are equal
	if (radius2!=p.radius2) return radius2 < p.radius2;
	if (s!=p.s) return s < p.s;
	if (e!=p.e) return e < p.e;
	if (clockwise_arc!=p.clockwise_arc) return clockwise_arc;
	return tilt < p.tilt;
}

class PosMscPolygonList;
class InvMscPolygonList;

//inside of polygon is to the right as we go through: counterclockwise pointlists contain the "outside"
//never degenerate - always has actual area
//never flopped - edges never cross each-other
//always contagious - one single surface
//if empty and clockwise is false it means the whole universe
class MscPolygon : protected std::vector<PolyEdge>
{
protected:
	friend class MscCrossPointStore;
	friend class PosMscPolygonList;
	friend class InvMscPolygonList;

	Block  boundingBox;
	bool   clockwise;

	MscPolygon() {boundingBox.MakeInvalid(); clockwise=true;}
	void Inverse() {for (int i=0; i<size()/2; i++) std::swap(at(i), at(size()-1-i)); clockwise ^= true;}
public:
	typedef enum {OK, A_IS_EMPTY, B_IS_EMPTY, BOTH_EMPTY, A_INSIDE_B, B_INSIDE_A, SAME, COMPLEMENT, APART_OUTSIDE, APART_INSIDE} poly_result_t;
	void append_to_surfaces_or_holes(PosMscPolygonList &surfaces, InvMscPolygonList &holes) const;
	is_within_t   IsWithin(XY p, int &edge) const;
	poly_result_t CheckContainmentHelper(const MscPolygon &b) const;
	poly_result_t CheckContainment(const MscPolygon &b) const;
	poly_result_t PolyProcess(const MscPolygon &b, PosMscPolygonList &surfaces, InvMscPolygonList &holes, bool do_union) const;
	bool CalculateClockwise() const;
	
	bool IsClockWise() const {return clockwise;}
	const Block &GetBoundingBox() const {return boundingBox;}
	void swap(MscPolygon &b) {std::vector<PolyEdge>::swap(b); std::swap(boundingBox, b.boundingBox);}
	int next(int vertex) const {return (vertex+1)%size();}
	int prev(int vertex) const {return (vertex-1+size())%size();}
	PolyEdge       &at_next(int i)       {return at(next(i));}
	const PolyEdge &at_next(int i) const {return at(next(i));}
	PolyEdge       &at_prev(int i)       {return at(prev(i));}
	const PolyEdge &at_prev(int i) const {return at(prev(i));}
	bool operator < (const MscPolygon &b) const;
	bool operator ==(const MscPolygon &b) const;
    bool Overlaps(const MscPolygon &b) const;
	void Expand(double gap);
	void Rotate90(XY) {}
	void Shift(XY xy) {for (int i=0; i<size(); i++) at(i) += xy; boundingBox.Shift(xy);}
	void Path(cairo_t *cr) const;

	size_type size() const {return std::vector<PolyEdge>::size();}
};

inline bool MscPolygon::operator <(const MscPolygon &b) const 
{
	if (boundingBox != b.boundingBox) return boundingBox < b.boundingBox;
	if (clockwise != b.clockwise) return clockwise; //clockwise is smaller than not
	if (size() != b.size()) return size() < b.size();
	for (int i=0; i<size(); i++) if (at(i) != b[i]) return at(i)<b[i];
	return false; //equal
}

inline bool MscPolygon::operator ==(const MscPolygon &b) const 
{
	if (boundingBox != b.boundingBox || clockwise != b.clockwise
		|| size() != b.size()) return false;
	for (int i=0; i<size(); i++) if (at(i) != b[i]) false;
	return true; //equal
}


class PosMscPolygon : public MscPolygon
{
public:
	explicit PosMscPolygon(const MscPolygon &b) : MscPolygon(b) {if (!clockwise) Inverse();}
    PosMscPolygon(double sx, double dx, double sy, double dy) {operator = (Block(sx,dx,sy,dy));}
	PosMscPolygon(const Block &b) {operator =(b);}
	PosMscPolygon(XY a, XY b, XY c);
	PosMscPolygon(const XY &c, double radius_x, double radius_y=0, double tilt=0);

	PosMscPolygon &operator =(const Block &b);
};

class InvMscPolygon : public MscPolygon
{
public:
	explicit InvMscPolygon(const MscPolygon &b) : MscPolygon(b) {if (clockwise) Inverse();}
};

//this contains a list of non-overlapping polygons
template <class polygon_type>
class MscPolygonList : protected std::list<polygon_type>
{
protected:
	friend class MscPolygon;
	friend class MscArea;
	Block         boundingBox;
	MscPolygonList() {boundingBox.MakeInvalid();}

	void swap(MscPolygonList &b) {std::list<polygon_type>::swap(b); std::swap(boundingBox, b.boundingBox);}
	MscPolygonList &append(const polygon_type &p) {push_back(p); boundingBox += p.GetBoundingBox(); return *this;}
	MscPolygonList &append(const MscPolygonList &pl) {for (auto i=pl.begin(); i!=pl.end(); i++) append(*i); return *this;}
	void splice_with_bb(iterator i, MscPolygonList &b, iterator from, iterator to)
		{for (auto j=from; j!=to; j++) boundingBox += j->GetBoundingBox(); splice(i, b, from, to);}
	void splice_with_bb(iterator i, MscPolygonList &pl) {splice_with_bb(i, pl, pl.begin(), pl.end());}
	void splice_with_bb(MscPolygonList &pl) {splice_with_bb(end(), pl);}
	const Block &GetBoundingBox(void) const {return boundingBox;}
	void Path(cairo_t *cr) const {for (auto i=begin(); i!=end(); i++) i->Path(cr);}
	void Shift(XY xy) {for (auto i = begin(); i!=end(); i++) i->Shift(xy); boundingBox.Shift(xy);}
	void Rotate90(XY xy) {for (auto i = begin(); i!=end(); i++) i->Rotate90(xy); boundingBox.Transpose();}
	void Expand(double gap);

public:
	bool operator < (const MscPolygonList &pl) const;
	bool operator ==(const MscPolygonList &pl) const;

};

template <class polygon_type>
inline bool MscPolygonList<polygon_type>::operator <(const MscPolygonList &pl) const 
{
	if (boundingBox != pl.boundingBox) return boundingBox < pl.boundingBox;
	if (size() != pl.size()) return size()<pl.size();
	for (auto i = begin(), j=pl.begin(); i!=end(); i++, j++)
		if (!(*i==*j)) return *i<*j;
	return false;
}

template <class polygon_type>
inline bool MscPolygonList<polygon_type>::operator ==(const MscPolygonList &pl) const 
{
	if (boundingBox == pl.boundingBox || size() != pl.size()) return false;
	for (auto i = begin(), j=pl.begin(); i!=end(); i++, j++)
		if (!(*i==*j)) return false;
	return true;
}



//clockwise 
class PosMscPolygonList : public MscPolygonList<PosMscPolygon> 
{
	friend class MscPolygon;
	friend class InvMscPolygonList;
	friend class MscArea;
protected:
	PosMscPolygonList() {boundingBox.MakeInvalid();}
	PosMscPolygonList(const PosMscPolygon &p) {boundingBox.MakeInvalid(); if (p.IsClockWise()) append(p);}
	void NormalizeWith(InvMscPolygonList &holes);
	void Union(const PosMscPolygon &b, InvMscPolygonList &holes);
	void Intersect(const PosMscPolygon &b);
	void Intersect(const InvMscPolygon &b, InvMscPolygonList &holes);
	void Intersect(const PosMscPolygonList &b);
	void Substract(const PosMscPolygon &b, InvMscPolygonList &holes) {InvMscPolygon neg_b(b); Intersect(neg_b, holes);}
	void Substract(const InvMscPolygon &b) {PosMscPolygon neg_b(b); Intersect(neg_b);}
};

//counterclockwise
class InvMscPolygonList : public MscPolygonList<InvMscPolygon>  
{
	friend class MscPolygon;
	friend class PosMscPolygonList;
	friend class MscArea;
protected:
	InvMscPolygonList() {boundingBox.MakeInvalid();}
	bool Union(const PosMscPolygon &b);
	void Union(const InvMscPolygonList &b);   //pairwise intersection of holes
	void Intersect(const InvMscPolygon &b, PosMscPolygonList &surfaces);
};



//This can contain multiple positive polygons and holes
//holes are always within one of the polygons
//plus it has additional stuff, such as arc, drawtype, findtype and mainline
class MscArea {
protected:
	PosMscPolygonList surfaces;
	InvMscPolygonList holes;

	//MscArea(const PosMscPolygon &p) : surfaces(p) {}
public:
    Range           mainline;
	mutable TrackableElement *arc;
    mutable enum DrawType {DRAW_NONE, DRAW_NORMAL, DRAW_FRAME} drawType;
    mutable enum FindType {FIND_NONE, FIND_NORMAL} findType;

	explicit MscArea(TrackableElement *a=NULL) : arc(a), 
		drawType(DRAW_NORMAL), findType(FIND_NORMAL) {}
    MscArea(double sx, double dx, double sy, double dy, TrackableElement *a=NULL) : arc(a), 
		drawType(DRAW_NORMAL), findType(FIND_NORMAL), surfaces(PosMscPolygon(sx,dx,sy,dy)) {mainline.MakeInvalid();}
    MscArea(const Block &b, TrackableElement *a=NULL) : arc(a), 
		drawType(DRAW_NORMAL), findType(FIND_NORMAL), surfaces(PosMscPolygon(b)) {mainline.MakeInvalid();}
	MscArea(XY a, XY b, XY c, TrackableElement *aa=NULL) : arc(aa), 
		drawType(DRAW_NORMAL), findType(FIND_NORMAL), surfaces(PosMscPolygon(a,b,c)) {mainline.MakeInvalid();}
	MscArea(const XY &c, double radius_x, double radius_y=0, double tilt=0, TrackableElement *aa=NULL) : arc(aa), 
		drawType(DRAW_NORMAL), findType(FIND_NORMAL), surfaces(PosMscPolygon(c, radius_x, radius_y, tilt)) {mainline.MakeInvalid();}
	
	void clear() {holes.clear(); surfaces.clear(); mainline.MakeInvalid();}
	void swap(MscArea &a);
	bool operator <(const MscArea &b) const {
		if (surfaces != b.surfaces) return surfaces<b.surfaces;
		if (holes != b.holes) return holes<b.holes;
		if (mainline!=b.mainline) return mainline<b.mainline; 
		return false;
	}
    bool IsEmpty() const {return surfaces.size()==0;}
	const Block &GetBoundingBox(void) const {return surfaces.GetBoundingBox();}

	void Expand(MscArea &polygon, double gap) const;
    bool Overlaps(const MscArea &b, double gap=0) const;
    bool IsWithin(XY p) const;
	MscArea &operator += (const MscArea &b);
	MscArea &operator *= (const MscArea &b);
	MscArea &operator -= (const MscArea &b);
	MscArea operator + (const MscArea &b) const {MscArea tmp(*this); tmp+=b; return tmp;}
	MscArea operator * (const MscArea &b) const {MscArea tmp(*this); tmp*=b; return tmp;}
	MscArea operator - (const MscArea &b) const {MscArea tmp(*this); tmp-=b; return tmp;}
	//clockwise: give the lower left corner of the encompassing box
	//counterclockwise: give the upper right of the *oroginal, non rotated* block (we assume y increases downward, as on screen)
	void Rotate90Clockwise(const Block &b) {surfaces.Rotate90(b.LowerRight()); holes.Rotate90(b.LowerRight()); mainline.MakeInvalid();}
	void Rotate90Back(const Block &b) {surfaces.Rotate90(b.UpperRight()); holes.Rotate90(b.UpperRight()); mainline.MakeInvalid();}
	MscArea & Shift(XY xy) {surfaces.Shift(xy); holes.Shift(xy); mainline.Shift(xy.y); return *this;}

	void Path(cairo_t *cr) const;
	void Line(cairo_t *cr) const;
	void Fill(cairo_t *cr) const;
};


#endif //POLYGON_H
