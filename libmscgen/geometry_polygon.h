#if !defined(GEOMETRY_POLYGON_H)
#define GEOMETRY_POLYGON_H

#include <vector>
#include "geometry_edge.h"

namespace geometry {

class PosPolygonList;
class InvPolygonList;

//inside of polygon is to the right as we go through: counterclockwise pointlists contain the "outside"
//never degenerate - always has actual area
//never flopped - edges never cross each-other
//always contagious - one single surface
//if empty and clockwise is false it means the whole universe
class Polygon : protected std::vector<Edge>
{
protected:
	friend class MscCrossPointStore;
	friend class PosPolygonList;
	friend class InvPolygonList;
	friend void test_geo(cairo_t *cr, int x, int y, bool clicked); ///XXX

	Block  boundingBox;
	bool   clockwise;

	Polygon() {boundingBox.MakeInvalid(); clockwise=true;}
	void CopyInverseToMe(const Polygon &b);
public:
	typedef enum {OK, A_IS_EMPTY, B_IS_EMPTY, BOTH_EMPTY, A_INSIDE_B, B_INSIDE_A, SAME, COMPLEMENT, APART_OUTSIDE, APART_INSIDE} poly_result_t;
	void append_to_surfaces_or_holes(PosPolygonList &surfaces, InvPolygonList &holes) const;
	is_within_t   IsWithin(XY p, int &edge) const;
	poly_result_t CheckContainmentHelper(const Polygon &b) const;
	poly_result_t CheckContainment(const Polygon &b) const;
	poly_result_t PolyProcess(const Polygon &b, PosPolygonList &surfaces, InvPolygonList &holes, bool do_union) const;
	bool CalculateClockwise() const;
	
	bool IsClockWise() const {return clockwise;}
	const Block &GetBoundingBox() const {return boundingBox;}
	void swap(Polygon &b) {std::vector<Edge>::swap(b); std::swap(boundingBox, b.boundingBox);}
	int next(int vertex) const {return (vertex+1)%size();}
	int prev(int vertex) const {return (vertex-1+size())%size();}
	Edge       &at_next(int i)       {return at(next(i));}
	const Edge &at_next(int i) const {return at(next(i));}
	Edge       &at_prev(int i)       {return at(prev(i));}
	const Edge &at_prev(int i) const {return at(prev(i));}
	bool operator < (const Polygon &b) const;
	bool operator ==(const Polygon &b) const;
    bool Overlaps(const Polygon &b) const;
	void Expand(double gap);
	void Rotate90(XY) {}
	void Shift(XY xy) {for (int i=0; i<size(); i++) at(i).Shift(xy); boundingBox.Shift(xy);}
	void Path(cairo_t *cr) const;

	size_type size() const {return std::vector<Edge>::size();}
};

inline bool Polygon::operator <(const Polygon &b) const 
{
	if (boundingBox != b.boundingBox) return boundingBox < b.boundingBox;
	if (clockwise != b.clockwise) return clockwise; //clockwise is smaller than not
	if (size() != b.size()) return size() < b.size();
	for (int i=0; i<size(); i++) if (at(i) != b[i]) return at(i)<b[i];
	return false; //equal
}

inline bool Polygon::operator ==(const Polygon &b) const 
{
	if (boundingBox != b.boundingBox || clockwise != b.clockwise
		|| size() != b.size()) return false;
	for (int i=0; i<size(); i++) if (at(i) != b[i]) false;
	return true; //equal
}


class PosPolygon : public Polygon
{
public:
	explicit PosPolygon(const Polygon &b) {if (b.IsClockWise()) *(Polygon*)this = b; else CopyInverseToMe(b);}
    PosPolygon(double sx, double dx, double sy, double dy) {operator = (Block(sx,dx,sy,dy));}
	PosPolygon(const Block &b) {operator =(b);}
	PosPolygon(XY a, XY b, XY c);
	PosPolygon(const XY &c, double radius_x, double radius_y=0, double tilt_degree=0);

	PosPolygon &operator =(const Block &b);
};

class InvPolygon : public Polygon
{
public:
	explicit InvPolygon(const Polygon &b) : Polygon(b) {if (!b.IsClockWise()) *(Polygon*)this = b; else CopyInverseToMe(b);}
};





}; //namespace
#endif //GEOMETRY_POLYGON_H
