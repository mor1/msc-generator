#if !defined(GEOMETRY_POLYGON_H)
#define GEOMETRY_POLYGON_H

#include <vector>
#include <list>
#include "geometry_edge.h"

namespace geometry {

class PolygonList;

//inside of polygon is to the right as we go through: counterclockwise pointlists contain the "outside"
//never degenerate - always has actual area
//never flopped - edges never cross each-other
//always contagious - one single surface
//always goes in clockwise direction - except internally
class Polygon : protected std::vector<Edge>
{
protected:
	friend class MscCrossPointStore;
	friend void test_geo(cairo_t *cr, int x, int y, bool clicked); ///XXX

	Block  boundingBox;

	Polygon() {boundingBox.MakeInvalid();}
	Polygon Polygon::GetInverse() const;

public:
	typedef enum {OK, A_IS_EMPTY, B_IS_EMPTY, BOTH_EMPTY, A_INSIDE_B, B_INSIDE_A, SAME, APART} poly_result_t;
	typedef enum {UNION, INTERSECT, SUBSTRACT} poly_action_t;
protected:
	is_within_t   IsWithin(XY p, int *edge=NULL) const;
	poly_result_t CheckContainmentHelper(const Polygon &b) const;
	poly_result_t CheckContainment(const Polygon &b) const;
	poly_result_t PolyProcess(const Polygon &b, PolygonList &result, poly_action_t action) const;
	bool CalculateClockwise() const;
	
	int next(int vertex) const {return (vertex+1)%size();}
	int prev(int vertex) const {return (vertex-1+size())%size();}
	Edge       &at_next(int i)       {return at(next(i));}
	const Edge &at_next(int i) const {return at(next(i));}
	Edge       &at_prev(int i)       {return at(prev(i));}
	const Edge &at_prev(int i) const {return at(prev(i));}
public:
	Polygon(Polygon &&p) {swap(p);}
	Polygon(double sx, double dx, double sy, double dy) {operator = (Block(sx,dx,sy,dy));}
	Polygon(const Block &b) {operator =(b);}
	Polygon(XY a, XY b, XY c);
	Polygon(const XY &c, double radius_x, double radius_y=0, double tilt_degree=0);
	Polygon &operator =(const Block &b);

	bool operator < (const Polygon &b) const;
	bool operator ==(const Polygon &b) const;
	Polygon &operator=(Polygon &&p) {swap(p);  return *this;}

	const Block &GetBoundingBox() const {return boundingBox;}
	void swap(Polygon &b) {std::vector<Edge>::swap(b); std::swap(boundingBox, b.boundingBox);}
	void clear() {std::vector<Edge>::clear(); boundingBox.MakeInvalid();}

	bool Overlaps(const Polygon &b) const;
	void Expand(double gap);
	void Rotate90(XY) {}
	void Shift(XY xy) {for (int i=0; i<size(); i++) at(i).Shift(xy); boundingBox.Shift(xy);}
	void Path(cairo_t *cr, bool inverse=false) const;
};

inline bool Polygon::operator <(const Polygon &b) const 
{
	if (boundingBox != b.boundingBox) return boundingBox < b.boundingBox;
	if (size() != b.size()) return size() < b.size();
	for (int i=0; i<size(); i++) if (at(i) != b[i]) return at(i)<b[i];
	return false; //equal
}

inline bool Polygon::operator ==(const Polygon &b) const 
{
	if (boundingBox != b.boundingBox || size() != b.size()) return false;
	for (int i=0; i<size(); i++) if (at(i) != b[i]) false;
	return true; //equal
}

}; //namespace
#endif //GEOMETRY_POLYGON_H
