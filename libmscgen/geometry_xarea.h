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
    friend void test_geo(cairo_t *cr, int x, int y, bool clicked); //XXX
protected:
    Block boundingBox;
public:
    PolygonList();
    explicit PolygonList(const PolygonWithHoles &p);
    explicit PolygonList(const Polygon &p);
    explicit PolygonList(PolygonWithHoles &&p);
    explicit PolygonList(Polygon &&p);
    explicit PolygonList(std::vector<Edge> &&v) {assign(std::move(v));}
    explicit PolygonList(const std::vector<Edge> &v) {assign(v);}
    PolygonList(PolygonList &&p) {swap(p);}

    void append(PolygonWithHoles &&p);
    void append(const PolygonWithHoles &p);
    void append(PolygonList &&p) {boundingBox += p.GetBoundingBox(); splice(end(), p);}
    void append(const PolygonList &p) {boundingBox += p.GetBoundingBox(); insert(end(), p.begin(), p.end());}
    void swap(PolygonList &a) {std::list<PolygonWithHoles>::swap(a); std::swap(boundingBox, a.boundingBox);}
    void clear() {std::list<PolygonWithHoles>::clear(); boundingBox.MakeInvalid();}
    void assign(std::vector<Edge> &&v, bool winding=true);
    void assign(const std::vector<Edge> &v, bool winding=true);
    bool operator <(const PolygonList &b) const;
    bool operator ==(const PolygonList &b) const;
    PolygonList &operator =(std::vector<Edge> &&v) {assign(std::move(v)); return *this;}
    PolygonList &operator =(const std::vector<Edge> &v) {assign(v); return *this;}

    PolygonList &operator = (PolygonList &&a) {swap(a); return *this;}
    bool IsEmpty() const {return size()==0;}
    const Block &GetBoundingBox(void) const {return boundingBox;}
    is_within_t IsWithin(XY p) const;
    PolygonList &Shift(XY xy);
    PolygonList &Rotate(double cos, double sin, double radian);
    PolygonList &RotateAround(const XY&c, double cos, double sin, double radian);
	PolygonList &Rotate(double degrees) {double r=deg2rad(degrees); Rotate(cos(r), sin(r), r); return *this;}
    PolygonList &RotateAround(const XY&c, double degrees) {double r=deg2rad(degrees); RotateAround(c, cos(r), sin(r), r); return *this;}

    PolygonList &operator += (const PolygonWithHoles &p);
    PolygonList &operator *= (const PolygonWithHoles &p);
    PolygonList &operator -= (const PolygonWithHoles &p);
    PolygonList &operator += (const PolygonList &a);
    PolygonList &operator *= (const PolygonList &a);
    PolygonList &operator -= (const PolygonList &a);

    void Expand(double gap, PolygonList &result) const;
	void ClearHoles();

    void Path(cairo_t *cr, bool inv) const;
    void Path(cairo_t *cr, bool inv, bool do_this) const;
};

//this contains a list of non-overlapping polygons as holes
class PolygonWithHoles : public Polygon
{
    friend class Polygon;
    friend class PolygonList;
	friend class node_list;   //to access holes
	friend void test_geo(cairo_t *cr, int x, int y, bool clicked); //XXX

	void Rotate(double radian) {} //hide these - do not
	void RotateAround(const XY&c, double radian) {} //hide these - do not
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
    void Rotate(double cos, double sin, double radian)
        {Polygon::Rotate(cos, sin, radian); if (holes.size()) holes.Rotate(cos, sin, radian);}
    void RotateAround(const XY&c, double cos, double sin, double radian)
        {Polygon::RotateAround(c, cos, sin, radian); if (holes.size()) holes.RotateAround(c, cos, sin, radian);}

    Polygon::poly_result_t Add(const PolygonWithHoles &p, PolygonList &res) const;
    Polygon::poly_result_t Mul(const PolygonWithHoles &p, PolygonList &res) const;
    Polygon::poly_result_t Sub(const PolygonWithHoles &p, PolygonList &res) const;

	void Expand(double gap, PolygonList &res) const;
	void ClearHoles() {holes.clear();}

    void Path(cairo_t *cr, bool inv) const
        {Polygon::Path(cr, inv); if (holes.size()) holes.Path(cr, !inv);}
    void Path(cairo_t *cr, bool inv, bool do_this) const
        {if (do_this) Polygon::Path(cr, inv); if (holes.size()) holes.Path(cr, !inv, !do_this);}
};

class TrackableElement;

//plus it has additional stuff, such as arc, drawtype, findtype and mainline
class Area : protected PolygonList
{
    friend void test_geo(cairo_t *cr, int x, int y, bool clicked); ///XXX
public:
    typedef enum {NONE=0, FRAME=1, FULL=2} DrawType;
    mutable TrackableElement *arc;
    mutable DrawType          draw;
    mutable bool              find;
    Range                     mainline;

    explicit Area(TrackableElement *a=NULL, DrawType d=FULL, bool f=true) :
	   arc(a), draw(d), find(f) {mainline.MakeInvalid();}
    Area(const Polygon &p, TrackableElement *a=NULL, DrawType d=FULL, bool f=true) :
       PolygonList(p), arc(a), draw(d), find(f)  {mainline.MakeInvalid();}

    void clear() {PolygonList::clear(); mainline.MakeInvalid();}
    void swap(Area &a);
    void assign(std::vector<Edge> &&v) {PolygonList::assign(std::move(v));}
    void assign(const std::vector<Edge> &v) {PolygonList::assign(v);}
    bool operator <(const Area &b) const;
    bool operator ==(const Area &b) const;
    Area &operator =(std::vector<Edge> &&v) {assign(std::move(v)); return *this;}
    Area &operator =(const std::vector<Edge> &v) {assign(v); return *this;}
    Area &operator =(Polygon &&v) {clear(); append(std::move(v)); return *this;}
    Area &operator =(const Polygon &v) {clear(); append(v); return *this;}
    Area &operator += (const Area &b);
    Area &operator *= (const Area &b);
    Area &operator -= (const Area &b);
    Area &operator += (const Polygon &b) {PolygonList::operator+=(PolygonWithHoles(b)); return *this;}
    Area &operator *= (const Polygon &b) {PolygonList::operator*=(PolygonWithHoles(b)); return *this;}
    Area &operator -= (const Polygon &b) {PolygonList::operator-=(PolygonWithHoles(b)); return *this;}

    Area & Shift(XY xy) {PolygonList::Shift(xy); mainline.Shift(xy.y); return *this;}
    Area & Rotate(double degrees) {PolygonList::Rotate(degrees); return *this;}
    Area & RotateAround(const XY&c, double degrees) {PolygonList::RotateAround(c, degrees); return *this;}

	Area Expand(double gap) const;
	void ClearHoles() {PolygonList::ClearHoles();}

    bool IsEmpty() const {return size()==0;}
    void Path(cairo_t *cr) const {PolygonList::Path(cr, true);}
    void Line(cairo_t *cr) const {PolygonList::Path(cr, true); cairo_stroke(cr);}
    void Line2(cairo_t *cr) const;
    void Fill(cairo_t *cr) const {PolygonList::Path(cr, true); cairo_fill(cr);}
};

//these must be here due to PolygonList forward referencing PolygonWithHoles
inline PolygonList::PolygonList() {boundingBox.MakeInvalid();}
inline PolygonList::PolygonList(const PolygonWithHoles &p) :
        std::list<PolygonWithHoles>(1, p), boundingBox(p.GetBoundingBox()) {}
inline PolygonList::PolygonList(const Polygon &p) :
        std::list<PolygonWithHoles>(1, p), boundingBox(p.GetBoundingBox()) {}
inline PolygonList::PolygonList(PolygonWithHoles &&p) :
	std::list<PolygonWithHoles>(1, p), boundingBox(p.GetBoundingBox()) {}
inline PolygonList::PolygonList(Polygon &&p) :
	std::list<PolygonWithHoles>(1, p), boundingBox(p.GetBoundingBox()) {}


inline void PolygonList::append(PolygonWithHoles &&p)
        {boundingBox += p.GetBoundingBox(); push_back(p);}
inline void PolygonList::append(const PolygonWithHoles &p)
        {boundingBox += p.GetBoundingBox(); push_back(p);}

inline PolygonList &PolygonList::operator += (const PolygonList &a)
{
    if (boundingBox.Overlaps(a.GetBoundingBox()))
        for (auto i = a.begin(); i!=a.end(); i++)
            operator += (*i);
    else
        append(a);
    return *this;
}

inline PolygonList &PolygonList::operator *= (const PolygonList &a)
{
    if (boundingBox.Overlaps(a.boundingBox))
        for (auto i = a.begin(); i!=a.end(); i++)
            operator *= (*i);
    else
        clear();
    return *this;
}

inline PolygonList &PolygonList::operator -= (const PolygonList &a)
{
    if (boundingBox.Overlaps(a.boundingBox))
        for (auto i = a.begin(); i!=a.end(); i++)
            operator -= (*i);
    return *this;
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


inline PolygonList &PolygonList::Rotate(double cos, double sin, double radian)
{
    boundingBox.MakeInvalid();
    for (auto i = begin(); i!=end(); i++) {
        i->Rotate(cos, sin, radian);
        boundingBox += i->GetBoundingBox();
    }
    return *this;
}

inline PolygonList &PolygonList::RotateAround(const XY&c, double cos, double sin, double radian)
{
    boundingBox.MakeInvalid();
    for (auto i = begin(); i!=end(); i++) {
        i->RotateAround(c, cos, sin, radian);
        boundingBox += i->GetBoundingBox();
    }
    return *this;
}

inline void PolygonList::ClearHoles()
{
	for (auto i=begin(); i!=end(); i++)
		i->ClearHoles();
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

inline void PolygonList::Path(cairo_t *cr, bool inv, bool do_this) const
{
    for(auto i = begin(); i!=end(); i++)
        i->Path(cr, inv, do_this);
}

inline void PolygonList::Path(cairo_t *cr, bool inv) const
{
    for(auto i = begin(); i!=end(); i++)
        i->Path(cr, inv);
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
