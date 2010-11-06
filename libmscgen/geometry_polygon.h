#if !defined(GEOMETRY_POLYGON_H)
#define GEOMETRY_POLYGON_H

#include <vector>
#include "geometry_edge.h"

namespace geometry {

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
	friend void test_geo(cairo_t *cr, int x, int y, bool clicked); ///XXX

	Block  boundingBox;
	bool   clockwise;

	MscPolygon() {boundingBox.MakeInvalid(); clockwise=true;}
	void CopyInverseToMe(const MscPolygon &b);
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
	explicit PosMscPolygon(const MscPolygon &b) {if (b.IsClockWise()) *(MscPolygon*)this = b; else CopyInverseToMe(b);}
    PosMscPolygon(double sx, double dx, double sy, double dy) {operator = (Block(sx,dx,sy,dy));}
	PosMscPolygon(const Block &b) {operator =(b);}
	PosMscPolygon(XY a, XY b, XY c);
	PosMscPolygon(const XY &c, double radius_x, double radius_y=0, double tilt_degree=0);

	PosMscPolygon &operator =(const Block &b);
};

class InvMscPolygon : public MscPolygon
{
public:
	explicit InvMscPolygon(const MscPolygon &b) : MscPolygon(b) {if (!b.IsClockWise()) *(MscPolygon*)this = b; else CopyInverseToMe(b);}
};





}; //namespace
#endif //GEOMETRY_POLYGON_H
