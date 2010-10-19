#if !defined(POLYGON_H)
#define POLYGON_H

#include <set>
#include <climits>
#include "error.h" //for file_line
#include "trackable.h"
#include "cairo.h"

class PosMscPolygonList;
class InvMscPolygonList;

//inside of polygon is to the right as we go through: counterclockwise pointlists contain the "outside"
//never degenerate - always has actual area
//never flopped - edges never cross each-other
//always contagious - one single surface
//if empty and clockwise is false it means the whole universe
class MscPolygon : protected std::vector<XY>
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
	is_within_t   IsWithin(XY p, int*pEdge=NULL) const;
	poly_result_t CheckContainmentHelper(const MscPolygon &b) const;
	poly_result_t CheckContainment(const MscPolygon &b) const;
	poly_result_t PolyProcess(const MscPolygon &b, PosMscPolygonList &surfaces, InvMscPolygonList &holes, bool do_union) const;
	
	bool IsClockWise() const {return clockwise;}
	const Block &GetBoundingBox() const {return boundingBox;}
	void swap(MscPolygon &b) {std::vector<XY>::swap(b); std::swap(boundingBox, b.boundingBox);}
    bool Overlaps(const MscPolygon &b) const;
	void Expand(MscPolygon &pointlist, double gap) const;
	void Rotate90(XY) {}
	void Shift(XY xy) {for (int i=0; i<size(); i++) at(i) += xy; boundingBox.Shift(xy);}
	void Path(cairo_t *cr) const;

	size_type size() const {return std::vector<XY>::size();}
};

class PosMscPolygon : public MscPolygon
{
public:
	explicit PosMscPolygon(const MscPolygon &b) : MscPolygon(b) {if (!clockwise) Inverse();}
    PosMscPolygon(double sx, double dx, double sy, double dy) {operator = (Block(sx,dx,sy,dy));}
	PosMscPolygon(const Block &b) {operator =(b);}
	PosMscPolygon(XY a, XY b, XY c);

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
};

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
	
	void clear() {holes.clear(); surfaces.clear(); mainline.MakeInvalid();}
	void swap(MscArea &a);
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
