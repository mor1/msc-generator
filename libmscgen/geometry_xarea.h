#if !defined(GEOMETRY_XAREA_H)
#define GEOMETRY_XAREA_H

#include <list>
#include "geometry_polygon.h"

namespace geometry {

class PolygonWithHoles;

//This can contain multiple positive polygons and holes
//holes are always within one of the polygons
class PolygonList : protected std::list<PolygonWithHoles>
{
	friend class PolygonWithHoles;
	friend class Polygon;
protected:
	Block boundingBox;
public:
	PolygonList();
	PolygonList(const PolygonWithHoles &p);
	PolygonList(PolygonList &&p) {swap(p);}

	void append(PolygonWithHoles &&p);
	void append(const PolygonWithHoles &p);
	void append(PolygonList &&p) {boundingBox += p.GetBoundingBox(); 
	    std::list<PolygonWithHoles>::splice(std::list<PolygonWithHoles>::end(), p, p.begin(), p.end());}
	void append(const PolygonList &p) {boundingBox += p.GetBoundingBox(); 
	    std::list<PolygonWithHoles>::insert(std::list<PolygonWithHoles>::end(), p.begin(), p.end());}
	void swap(PolygonList &a) {std::list<PolygonWithHoles>::swap(a); std::swap(boundingBox, a.boundingBox);}
	void clear() {std::list<PolygonWithHoles>::clear(); boundingBox.MakeInvalid();}
	bool operator <(const PolygonList &b) const;
	bool operator ==(const PolygonList &b) const;
	PolygonList &operator = (PolygonList &&a) {swap(a); return *this;}
    bool IsEmpty() const {return size()==0;}
	const Block &GetBoundingBox(void) const {return boundingBox;}
    is_within_t IsWithin(XY p) const;
	PolygonList &Shift(XY xy);

	PolygonList &operator += (const PolygonWithHoles &p);
	PolygonList &operator *= (const PolygonWithHoles &p);
	PolygonList &operator -= (const PolygonWithHoles &p);
	PolygonList &operator += (const PolygonList &a);
	PolygonList &operator *= (const PolygonList &a);
	PolygonList &operator -= (const PolygonList &a);

	void Expand(PolygonList &polygon, double gap) const;

	void Path(cairo_t *cr, bool hole=false) const;
	void Line(cairo_t *cr, bool hole=false) const;
	void Fill(cairo_t *cr) const;
};

//this contains a list of non-overlapping polygons as holes
class PolygonWithHoles : public Polygon
{
	friend class Polygon;
	friend class PolygonList;
protected:
	PolygonList holes;
	PolygonWithHoles() {};
public:
	PolygonWithHoles(const Polygon &p) : Polygon(p) {}
	PolygonWithHoles(PolygonWithHoles &&p) {swap(p);}
	void swap(PolygonWithHoles &b) {holes.swap(b.holes); Polygon::swap(b);}
	bool operator < (const PolygonWithHoles &p) const;
	bool operator ==(const PolygonWithHoles &p) const;
	PolygonWithHoles &operator = (PolygonWithHoles &&a) {swap(a); return *this;}
	const Block &GetBoundingBox(void) const {return Polygon::GetBoundingBox();}
	is_within_t IsWithin(XY p) const;
	void Shift(XY xy) {Polygon::Shift(xy); holes.Shift(xy);}
	Polygon::poly_result_t Add(const PolygonWithHoles &p, PolygonList &res) const;
	Polygon::poly_result_t Mul(const PolygonWithHoles &p, PolygonList &res) const;
	Polygon::poly_result_t Sub(const PolygonWithHoles &p, PolygonList &res) const;

	void Path(cairo_t *cr, bool hole=false) const {Polygon::Path(cr, hole); holes.Path(cr, !hole);}
	void Line(cairo_t *cr, bool hole=false) const;
};

class TrackableElement;

//plus it has additional stuff, such as arc, drawtype, findtype and mainline
class Area : protected PolygonList 
{
	friend void test_geo(cairo_t *cr, int x, int y, bool clicked); ///XXX
public:
    mutable TrackableElement *arc;
    mutable enum DrawType {NONE=0, FRAME=1, FULL=2} 
	                          draw;
    mutable bool              find;
            Range             mainline;

	explicit Area(TrackableElement *a=NULL) : arc(a), draw(FULL), find(true) {mainline.MakeInvalid();}
	Area(const Polygon &p, TrackableElement *a=NULL, DrawType d=FULL, bool f=true) : 
	    PolygonList(p), arc(a), draw(d), find(f)  {mainline.MakeInvalid();}

	void clear() {PolygonList::clear(); mainline.MakeInvalid();}
	void swap(Area &a);
	bool operator <(const Area &b) const; 
	bool operator ==(const Area &b) const; 
	Area &operator += (const Area &b);
	Area &operator *= (const Area &b);
	Area &operator -= (const Area &b);
	Area &operator += (const Polygon &b) {PolygonList::operator+=(PolygonWithHoles(b)); return *this;}
	Area &operator *= (const Polygon &b) {PolygonList::operator*=(PolygonWithHoles(b)); return *this;}
	Area &operator -= (const Polygon &b) {PolygonList::operator-=(PolygonWithHoles(b)); return *this;}
	
	Area & Shift(XY xy) {PolygonList::Shift(xy); mainline.Shift(xy.y); return *this;}
	bool IsEmpty() const {return size()==0;}
	void Path(cairo_t *cr) const {PolygonList::Path(cr);}
	void Line(cairo_t *cr) const {PolygonList::Line(cr);}
	void Fill(cairo_t *cr) const {PolygonList::Fill(cr);}
};


inline PolygonList::PolygonList() {boundingBox.MakeInvalid();}
inline PolygonList::PolygonList(const PolygonWithHoles &p) : 
	  std::list<PolygonWithHoles>(1, p), boundingBox(p.GetBoundingBox()) {}


inline void PolygonList::append(PolygonWithHoles &&p) 
{
	boundingBox += p.GetBoundingBox(); 
	push_back(p);
}

inline void PolygonList::append(const PolygonWithHoles &p) 
{
	boundingBox += p.GetBoundingBox(); 
	push_back(p);
}


inline bool PolygonList::operator <(const PolygonList &b) const 
{
	if (boundingBox != b.boundingBox) return boundingBox<b.boundingBox;
	if (size() != b.size()) return size()<b.size();
	for (auto i = begin(), j=b.begin(); i!=end(); i++, j++)
		if (!(*i==*j)) return *i<*j;
	return false;
}

inline bool PolygonList::operator ==(const PolygonList &b) const 
{
	if (boundingBox != b.boundingBox || size() != b.size()) return false;
	for (auto i = begin(), j=b.begin(); i!=end(); i++, j++)
		if (!(*i==*j)) return false;
	return true;
}

inline is_within_t PolygonList::IsWithin(XY p) const 
{
	is_within_t ret = boundingBox.IsWithin(p);
	if (ret==WI_OUTSIDE) return WI_OUTSIDE;
	for (auto i = begin(); i!=end(); i++)
		if (WI_OUTSIDE != (ret=i->IsWithin(p))) return ret;
	return WI_OUTSIDE;
}

inline PolygonList &PolygonList::Shift(XY xy) 
{
	for (auto i = begin(); i!=end(); i++)
		i->Shift(xy);
	boundingBox.Shift(xy);
	return *this;
}


inline bool PolygonWithHoles::operator <(const PolygonWithHoles &p) const 
{
	if (!(Polygon::operator==(p))) return Polygon::operator<(p);
	if (holes.size() != p.holes.size()) return size()<p.holes.size();
	for (auto i = holes.begin(), j=p.holes.begin(); i!=holes.end(); i++, j++)
		if (!(*i==*j)) return *i<*j;
	return false;
}

inline bool PolygonWithHoles::operator ==(const PolygonWithHoles &p) const 
{
	if (holes.size() != p.holes.size() || !(Polygon::operator==(p))) return false;
	for (auto i = holes.begin(), j=p.holes.begin(); i!=holes.end(); i++, j++)
		if (!(*i==*j)) return false;
	return true;
}

inline is_within_t PolygonWithHoles::IsWithin(XY p) const 
{
	is_within_t ret = Polygon::IsWithin(p);
	if (ret!=WI_INSIDE) return ret;
	for (auto i = holes.begin(); i!=holes.end(); i++) {
		ret = i->IsWithin(p); 
		if (ret!=WI_OUTSIDE) 
			return ret == WI_INSIDE ? WI_OUTSIDE : ret;
	}
	return WI_INSIDE;
}

inline bool Area::operator <(const Area &b) const
{
	if (mainline!=b.mainline) return mainline<b.mainline;
	return PolygonList::operator<(b);
}

inline bool Area::operator ==(const Area &b) const
{
	if (mainline!=b.mainline) return false;
	return PolygonList::operator==(b);
}

inline PolygonList operator + (const PolygonList &p1, const PolygonWithHoles &p2) {return std::move(PolygonList(p1)+=p2);}
inline PolygonList operator * (const PolygonList &p1, const PolygonWithHoles &p2) {return std::move(PolygonList(p1)*=p2);}
inline PolygonList operator - (const PolygonList &p1, const PolygonWithHoles &p2) {return std::move(PolygonList(p1)-=p2);}
inline PolygonList operator + (PolygonList &&p1, const PolygonWithHoles &p2) {return std::move(p1+=p2);}
inline PolygonList operator * (PolygonList &&p1, const PolygonWithHoles &p2) {return std::move(p1*=p2);}
inline PolygonList operator - (PolygonList &&p1, const PolygonWithHoles &p2) {return std::move(p1-=p2);}

inline PolygonList operator + (const PolygonList &p1, const PolygonList &p2) {return std::move(PolygonList(p1)+=p2);}
inline PolygonList operator * (const PolygonList &p1, const PolygonList &p2) {return std::move(PolygonList(p1)*=p2);}
inline PolygonList operator - (const PolygonList &p1, const PolygonList &p2) {return std::move(PolygonList(p1)-=p2);}
inline PolygonList operator + (PolygonList &&p1, const PolygonList &p2) {return std::move(p1+=p2);}
inline PolygonList operator * (PolygonList &&p1, const PolygonList &p2) {return std::move(p1*=p2);}
inline PolygonList operator - (PolygonList &&p1, const PolygonList &p2) {return std::move(p1-=p2);}
inline PolygonList operator + (const PolygonList &p1, PolygonList &&p2) {return std::move(p2+=p1);}
inline PolygonList operator * (const PolygonList &p1, PolygonList &&p2) {return std::move(p2*=p1);}
inline PolygonList operator + (PolygonList &&p1, PolygonList &&p2) {return std::move(p2+=p1);}
inline PolygonList operator * (PolygonList &&p1, PolygonList &&p2) {return std::move(p2*=p1);}

inline PolygonList operator + (const Polygon &p1, Polygon &p2) {return std::move(PolygonList(p1)+=p2);}
inline PolygonList operator * (const Polygon &p1, Polygon &p2) {return std::move(PolygonList(p1)*=p2);}
inline PolygonList operator - (const Polygon &p1, Polygon &p2) {return std::move(PolygonList(p1)-=p2);}

inline Area operator + (const Area &a, const Polygon &p)  {return std::move(Area(a)+=p);}
inline Area operator * (const Area &a, const Polygon &p)  {return std::move(Area(a)*=p);}
inline Area operator - (const Area &a, const Polygon &p)  {return std::move(Area(a)-=p);}
inline Area operator + (Area &&a, const Polygon &p)  {return std::move(a+=p);}
inline Area operator * (Area &&a, const Polygon &p)  {return std::move(a*=p);}
inline Area operator - (Area &&a, const Polygon &p)  {return std::move(a-=p);}

inline Area operator + (const Area &a1, const Area &a2)  {return std::move(Area(a1)+=a2);}
inline Area operator * (const Area &a1, const Area &a2)  {return std::move(Area(a1)*=a2);}
inline Area operator - (const Area &a1, const Area &a2)  {return std::move(Area(a1)-=a2);}
inline Area operator + (Area &&a1, const Area &a2)  {return std::move(a1+=a2);}
inline Area operator * (Area &&a1, const Area &a2)  {return std::move(a1*=a2);}
inline Area operator - (Area &&a1, const Area &a2)  {return std::move(a1-=a2);}
inline Area operator + (const Area &a1, Area &&a2)  {return std::move(a2+=a1);}
inline Area operator * (const Area &a1, Area &&a2)  {return std::move(a2*=a1);}
inline Area operator + (Area &&a1, Area &&a2)  {return std::move(a2+=a1);}
inline Area operator * (Area &&a1, Area &&a2)  {return std::move(a2*=a1);}

}; //namespace

#endif //GEOMETRY_XAREA_H
