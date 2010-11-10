#if !defined(GEOMETRY_XAREA_H)
#define GEOMETRY_XAREA_H

#include <list>
#include "geometry_polygon.h"

namespace geometry {

class GeoArea;

//this contains a list of non-overlapping polygons
template <class polygon_type>
class PolygonList : protected std::list<polygon_type>
{
protected:
	friend class Polygon;
	friend class GeoArea;
	Block         boundingBox;
	PolygonList() {boundingBox.MakeInvalid();}

	void swap(PolygonList &b) {std::list<polygon_type>::swap(b); std::swap(boundingBox, b.boundingBox);}
	PolygonList &append(const polygon_type &p) {push_back(p); boundingBox += p.GetBoundingBox(); return *this;}
	PolygonList &append(const PolygonList &pl) {for (auto i=pl.begin(); i!=pl.end(); i++) append(*i); return *this;}
	void splice_with_bb(iterator i, PolygonList &b, iterator from, iterator to)
		{for (auto j=from; j!=to; j++) boundingBox += j->GetBoundingBox(); splice(i, b, from, to);}
	void splice_with_bb(iterator i, PolygonList &pl) {splice_with_bb(i, pl, pl.begin(), pl.end());}
	void splice_with_bb(PolygonList &pl) {splice_with_bb(end(), pl);}
	const Block &GetBoundingBox(void) const {return boundingBox;}
	void Path(cairo_t *cr) const {for (auto i=begin(); i!=end(); i++) i->Path(cr);}
	void Shift(XY xy) {for (auto i = begin(); i!=end(); i++) i->Shift(xy); boundingBox.Shift(xy);}
	void Rotate90(XY xy) {for (auto i = begin(); i!=end(); i++) i->Rotate90(xy); boundingBox.Transpose();}
	void Expand(double gap);

public:
	bool operator < (const PolygonList &pl) const;
	bool operator ==(const PolygonList &pl) const;

};

template <class polygon_type>
inline bool PolygonList<polygon_type>::operator <(const PolygonList &pl) const 
{
	if (boundingBox != pl.boundingBox) return boundingBox < pl.boundingBox;
	if (size() != pl.size()) return size()<pl.size();
	for (auto i = begin(), j=pl.begin(); i!=end(); i++, j++)
		if (!(*i==*j)) return *i<*j;
	return false;
}

template <class polygon_type>
inline bool PolygonList<polygon_type>::operator ==(const PolygonList &pl) const 
{
	if (boundingBox == pl.boundingBox || size() != pl.size()) return false;
	for (auto i = begin(), j=pl.begin(); i!=end(); i++, j++)
		if (!(*i==*j)) return false;
	return true;
}



//clockwise 
class PosPolygonList : public PolygonList<PosPolygon> 
{
	friend class Polygon;
	friend class InvPolygonList;
	friend class GeoArea;
protected:
	PosPolygonList() {boundingBox.MakeInvalid();}
	PosPolygonList(const PosPolygon &p) {boundingBox.MakeInvalid(); if (p.IsClockWise()) append(p);}
	void NormalizeWith(InvPolygonList &holes);
	void Union(const PosPolygon &b, InvPolygonList &holes);
	void Intersect(const PosPolygon &b);
	void Intersect(const InvPolygon &b, InvPolygonList &holes);
	void Intersect(const PosPolygonList &b);
	void Substract(const PosPolygon &b, InvPolygonList &holes) {InvPolygon neg_b(b); Intersect(neg_b, holes);}
	void Substract(const InvPolygon &b) {PosPolygon neg_b(b); Intersect(neg_b);}
};

//counterclockwise
class InvPolygonList : public PolygonList<InvPolygon>  
{
	friend class Polygon;
	friend class PosPolygonList;
	friend class GeoArea;
protected:
	InvPolygonList() {boundingBox.MakeInvalid();}
	bool Union(const PosPolygon &b);
	void Union(const InvPolygonList &b);   //pairwise intersection of holes
	void Intersect(const InvPolygon &b, PosPolygonList &surfaces);
};

//This can contain multiple positive polygons and holes
//holes are always within one of the polygons
//plus it has additional stuff, such as arc, drawtype, findtype and mainline
class GeoArea {
	friend class GeoRectangle;
	friend class GeoTriangle;
	friend class GeoCircle;
	friend class GeoEllipse;
	friend void test_geo(cairo_t *cr, int x, int y, bool clicked); ///XXX

protected:
	PosPolygonList surfaces;
	InvPolygonList holes;
	
	void add_to_surfaces(const geometry::PosPolygon &p) {surfaces.append(p);}
public:
    Range           mainline;

	GeoArea() {mainline.MakeInvalid();}

	void clear() {holes.clear(); surfaces.clear(); mainline.MakeInvalid();}
	void swap(GeoArea &a);
	bool operator <(const GeoArea &b) const {
		if (surfaces != b.surfaces) return surfaces<b.surfaces;
		if (holes != b.holes) return holes<b.holes;
		if (mainline!=b.mainline) return mainline<b.mainline; 
		return false;
	}
    bool IsEmpty() const {return surfaces.size()==0;}
	const Block &GetBoundingBox(void) const {return surfaces.GetBoundingBox();}

	void Expand(GeoArea &polygon, double gap) const;
    bool Overlaps(const GeoArea &b, double gap=0) const;
    bool IsWithin(XY p) const;
	GeoArea &operator += (const GeoArea &b);
	GeoArea &operator *= (const GeoArea &b);
	GeoArea &operator -= (const GeoArea &b);
	GeoArea operator + (const GeoArea &b) const {GeoArea tmp(*this); tmp+=b; return tmp;}
	GeoArea operator * (const GeoArea &b) const {GeoArea tmp(*this); tmp*=b; return tmp;}
	GeoArea operator - (const GeoArea &b) const {GeoArea tmp(*this); tmp-=b; return tmp;}
	//clockwise: give the lower left corner of the encompassing box
	//counterclockwise: give the upper right of the *oroginal, non rotated* block (we assume y increases downward, as on screen)
	void Rotate90Clockwise(const Block &b) {surfaces.Rotate90(b.LowerRight()); holes.Rotate90(b.LowerRight()); mainline.MakeInvalid();}
	void Rotate90Back(const Block &b) {surfaces.Rotate90(b.UpperRight()); holes.Rotate90(b.UpperRight()); mainline.MakeInvalid();}
	GeoArea & Shift(XY xy) {surfaces.Shift(xy); holes.Shift(xy); mainline.Shift(xy.y); return *this;}

	void Path(cairo_t *cr) const;
	void Line(cairo_t *cr) const;
	void Fill(cairo_t *cr) const;
};


class GeoRectangle : public GeoArea  
{
public:
    GeoRectangle(double sx, double dx, double sy, double dy) 
		{add_to_surfaces(geometry::PosPolygon(Block(sx, dx, sy, dy)));}
	GeoRectangle(const XY &ul, const XY &dr)
		{add_to_surfaces(geometry::PosPolygon(Block(ul, dr)));}
	GeoRectangle(const Block &b)
		{add_to_surfaces(geometry::PosPolygon(b));}
};

class GeoTriangle : public GeoArea  
{
public:
    GeoTriangle(XY a, XY b, XY c)
		{add_to_surfaces(geometry::PosPolygon(a,b,c));}
};

class GeoCircle : public GeoArea  
{
public:
	GeoCircle(const XY &c, double radius_x)
		{add_to_surfaces(geometry::PosPolygon(c, radius_x));}
};

class GeoEllipse : public GeoArea  
{
public:
	GeoEllipse(const XY &c, double radius_x, double radius_y, double tilt_degree=0)
		{add_to_surfaces(geometry::PosPolygon(c, radius_x, radius_y, tilt_degree));}
};

}; //namespace

#endif //GEOMETRY_XAREA_H
