#if !defined(GEOMETRY_XAREA_H)
#define GEOMETRY_XAREA_H

#include <list>
#include "geometry_polygon.h"

namespace geometry {

class ContourWithHoles;

//This can contain multiple positive contours and holes
//holes are always within one of the contours
class ContourList : protected std::list<ContourWithHoles>
{
    friend class ContourWithHoles;
    friend class Contour;
    friend void test_geo(cairo_t *cr, int x, int y, bool clicked); //XXX
protected:
    Block boundingBox;
public:
    ContourList();
    explicit ContourList(const ContourWithHoles &p);
    explicit ContourList(const Contour &p);
    explicit ContourList(ContourWithHoles &&p);
    explicit ContourList(Contour &&p);
    explicit ContourList(std::vector<Edge> &&v) {assign(std::move(v));}
    explicit ContourList(const std::vector<Edge> &v) {assign(v);}
    ContourList(ContourList &&p) {swap(p);}

    void append(ContourWithHoles &&p);
    void append(const ContourWithHoles &p);
    void append(ContourList &&p) {boundingBox += p.GetBoundingBox(); splice(end(), p);}
    void append(const ContourList &p) {boundingBox += p.GetBoundingBox(); insert(end(), p.begin(), p.end());}
    void swap(ContourList &a) {std::list<ContourWithHoles>::swap(a); std::swap(boundingBox, a.boundingBox);}
    void clear() {std::list<ContourWithHoles>::clear(); boundingBox.MakeInvalid();}
    void assign(std::vector<Edge> &&v, bool winding=true);
    void assign(const std::vector<Edge> &v, bool winding=true);
    bool operator <(const ContourList &b) const;
    bool operator ==(const ContourList &b) const;
    ContourList &operator =(std::vector<Edge> &&v) {assign(std::move(v)); return *this;}
    ContourList &operator =(const std::vector<Edge> &v) {assign(v); return *this;}

    ContourList &operator = (ContourList &&a) {swap(a); return *this;}
    bool IsEmpty() const {return size()==0;}
    const Block &GetBoundingBox(void) const {return boundingBox;}
    is_within_t IsWithin(XY p) const;
    ContourList &Shift(XY xy);
    ContourList &Rotate(double cos, double sin, double radian);
    ContourList &RotateAround(const XY&c, double cos, double sin, double radian);
	ContourList &Rotate(double degrees) {double r=deg2rad(degrees); Rotate(cos(r), sin(r), r); return *this;}
    ContourList &RotateAround(const XY&c, double degrees) {double r=deg2rad(degrees); RotateAround(c, cos(r), sin(r), r); return *this;}

    ContourList &operator += (const ContourWithHoles &p);
    ContourList &operator *= (const ContourWithHoles &p);
    ContourList &operator -= (const ContourWithHoles &p);
    ContourList &operator ^= (const ContourWithHoles &p);
    ContourList &operator += (const ContourList &a);
    ContourList &operator *= (const ContourList &a);
    ContourList &operator -= (const ContourList &a);
    ContourList &operator ^= (const ContourList &a);

    void Expand(double gap, ContourList &result) const;
	void ClearHoles();

    double OffsetBelow(const Contour &below) const;
    double OffsetBelow(const ContourList &below) const;

    void Path(cairo_t *cr, bool inv) const;
    void Path(cairo_t *cr, bool inv, bool do_this) const;
};

//this contains a list of non-overlapping contours as holes
class ContourWithHoles : public Contour
{
    friend class Contour;
    friend class ContourList;
	friend class node_list;   //to access holes
	friend void test_geo(cairo_t *cr, int x, int y, bool clicked); //XXX

	void Rotate(double radian) {} //hide these - do not
	void RotateAround(const XY&c, double radian) {} //hide these - do not
protected:
    ContourList holes;
    ContourWithHoles() {};
public:
    ContourWithHoles(const Contour &p) : Contour(p) {}
    ContourWithHoles(ContourWithHoles &&p) {swap(p);}
    void swap(ContourWithHoles &b) {holes.swap(b.holes); Contour::swap(b);}
    bool operator < (const ContourWithHoles &p) const;
    bool operator ==(const ContourWithHoles &p) const;
    ContourWithHoles &operator = (ContourWithHoles &&a) {swap(a); return *this;}
    const Block &GetBoundingBox(void) const {return Contour::GetBoundingBox();}
    is_within_t IsWithin(XY p) const;
    void Shift(XY xy) {Contour::Shift(xy); holes.Shift(xy);}
    void Rotate(double cos, double sin, double radian)
        {Contour::Rotate(cos, sin, radian); if (holes.size()) holes.Rotate(cos, sin, radian);}
    void RotateAround(const XY&c, double cos, double sin, double radian)
        {Contour::RotateAround(c, cos, sin, radian); if (holes.size()) holes.RotateAround(c, cos, sin, radian);}

    Contour::result_t Add(const ContourWithHoles &p, ContourList &res) const;
    Contour::result_t Mul(const ContourWithHoles &p, ContourList &res) const;
    Contour::result_t Sub(const ContourWithHoles &p, ContourList &res) const;
    Contour::result_t Xor(const ContourWithHoles &p, ContourList &res) const;

	void Expand(double gap, ContourList &res) const;
	void ClearHoles() {holes.clear();}

    void Path(cairo_t *cr, bool inv) const
        {Contour::Path(cr, inv); if (holes.size()) holes.Path(cr, !inv);}
    void Path(cairo_t *cr, bool inv, bool do_this) const
        {if (do_this) Contour::Path(cr, inv); if (holes.size()) holes.Path(cr, !inv, !do_this);}
};

class TrackableElement;

//plus it has additional stuff, such as arc, drawtype, findtype and mainline
class Area : protected ContourList
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
    Area(const Contour &p, TrackableElement *a=NULL, DrawType d=FULL, bool f=true) :
       ContourList(p), arc(a), draw(d), find(f)  {mainline.MakeInvalid();}

    void clear() {ContourList::clear(); mainline.MakeInvalid();}
    void swap(Area &a);
    void assign(std::vector<Edge> &&v) {ContourList::assign(std::move(v));}
    void assign(const std::vector<Edge> &v) {ContourList::assign(v);}
    bool operator <(const Area &b) const;
    bool operator ==(const Area &b) const;
    Area &operator =(std::vector<Edge> &&v) {assign(std::move(v)); return *this;}
    Area &operator =(const std::vector<Edge> &v) {assign(v); return *this;}
    Area &operator =(Contour &&v) {clear(); append(std::move(v)); return *this;}
    Area &operator =(const Contour &v) {clear(); append(v); return *this;}
    Area &operator += (const Area &b);
    Area &operator *= (const Area &b);
    Area &operator -= (const Area &b);
    Area &operator += (const Contour &b) {ContourList::operator+=(ContourWithHoles(b)); return *this;}
    Area &operator *= (const Contour &b) {ContourList::operator*=(ContourWithHoles(b)); return *this;}
    Area &operator -= (const Contour &b) {ContourList::operator-=(ContourWithHoles(b)); return *this;}

    Area & Shift(XY xy) {ContourList::Shift(xy); mainline.Shift(xy.y); return *this;}
    Area & Rotate(double degrees) {ContourList::Rotate(degrees); return *this;}
    Area & RotateAround(const XY&c, double degrees) {ContourList::RotateAround(c, degrees); return *this;}

	Area CreateExpand(double gap) const;
	void ClearHoles() {ContourList::ClearHoles();}

    bool IsEmpty() const {return size()==0;}
    void Path(cairo_t *cr) const {ContourList::Path(cr, true);}
    void Line(cairo_t *cr) const {ContourList::Path(cr, true); cairo_stroke(cr);}
    void Line2(cairo_t *cr) const;
    void Fill(cairo_t *cr) const {ContourList::Path(cr, true); cairo_fill(cr);}
    void Fill2(cairo_t *cr, int r=255, int g=255, int b=255) const;
};

//these must be here due to ContourList forward referencing ContourWithHoles
inline ContourList::ContourList() {boundingBox.MakeInvalid();}
inline ContourList::ContourList(const ContourWithHoles &p) :
        std::list<ContourWithHoles>(1, p), boundingBox(p.GetBoundingBox()) {}
inline ContourList::ContourList(const Contour &p) :
        std::list<ContourWithHoles>(1, p), boundingBox(p.GetBoundingBox()) {}
inline ContourList::ContourList(ContourWithHoles &&p) :
	std::list<ContourWithHoles>(1, p), boundingBox(p.GetBoundingBox()) {}
inline ContourList::ContourList(Contour &&p) :
	std::list<ContourWithHoles>(1, p), boundingBox(p.GetBoundingBox()) {}


inline void ContourList::append(ContourWithHoles &&p)
        {boundingBox += p.GetBoundingBox(); push_back(p);}
inline void ContourList::append(const ContourWithHoles &p)
        {boundingBox += p.GetBoundingBox(); push_back(p);}

inline ContourList &ContourList::operator += (const ContourList &a)
{
    if (boundingBox.Overlaps(a.GetBoundingBox()))
        for (auto i = a.begin(); i!=a.end(); i++)
            operator += (*i);
    else
        append(a);
    return *this;
}

inline ContourList &ContourList::operator *= (const ContourList &a)
{
    if (boundingBox.Overlaps(a.boundingBox))
        for (auto i = a.begin(); i!=a.end(); i++)
            operator *= (*i);
    else
        clear();
    return *this;
}

inline ContourList &ContourList::operator -= (const ContourList &a)
{
    if (boundingBox.Overlaps(a.boundingBox))
        for (auto i = a.begin(); i!=a.end(); i++)
            operator -= (*i);
    return *this;
}

inline ContourList &ContourList::operator ^= (const ContourList &a)
{
    if (boundingBox.Overlaps(a.boundingBox))
        for (auto i = a.begin(); i!=a.end(); i++)
            operator ^= (*i);
    return *this;
}

inline bool ContourList::operator <(const ContourList &b) const
{
    if (boundingBox != b.boundingBox) return boundingBox<b.boundingBox;
    if (size() != b.size()) return size()<b.size();
    for (auto i = begin(), j=b.begin(); i!=end(); i++, j++)
        if (!(*i==*j)) return *i<*j;
    return false;
}

inline bool ContourList::operator ==(const ContourList &b) const
{
    if (boundingBox != b.boundingBox || size() != b.size()) return false;
    for (auto i = begin(), j=b.begin(); i!=end(); i++, j++)
        if (!(*i==*j)) return false;
    return true;
}

inline is_within_t ContourList::IsWithin(XY p) const
{
    is_within_t ret = boundingBox.IsWithin(p);
    if (ret==WI_OUTSIDE) return WI_OUTSIDE;
    for (auto i = begin(); i!=end(); i++)
        if (WI_OUTSIDE != (ret=i->IsWithin(p))) return ret;
    return WI_OUTSIDE;
}

inline ContourList &ContourList::Shift(XY xy)
{
    for (auto i = begin(); i!=end(); i++)
        i->Shift(xy);
    boundingBox.Shift(xy);
    return *this;
}


inline ContourList &ContourList::Rotate(double cos, double sin, double radian)
{
    boundingBox.MakeInvalid();
    for (auto i = begin(); i!=end(); i++) {
        i->Rotate(cos, sin, radian);
        boundingBox += i->GetBoundingBox();
    }
    return *this;
}

inline ContourList &ContourList::RotateAround(const XY&c, double cos, double sin, double radian)
{
    boundingBox.MakeInvalid();
    for (auto i = begin(); i!=end(); i++) {
        i->RotateAround(c, cos, sin, radian);
        boundingBox += i->GetBoundingBox();
    }
    return *this;
}

inline void ContourList::ClearHoles()
{
	for (auto i=begin(); i!=end(); i++)
		i->ClearHoles();
}


inline bool ContourWithHoles::operator <(const ContourWithHoles &p) const
{
    if (!(Contour::operator==(p))) return Contour::operator<(p);
    if (holes.size() != p.holes.size()) return size()<p.holes.size();
    for (auto i = holes.begin(), j=p.holes.begin(); i!=holes.end(); i++, j++)
        if (!(*i==*j)) return *i<*j;
    return false;
}

inline bool ContourWithHoles::operator ==(const ContourWithHoles &p) const
{
    if (holes.size() != p.holes.size() || !(Contour::operator==(p))) return false;
    for (auto i = holes.begin(), j=p.holes.begin(); i!=holes.end(); i++, j++)
        if (!(*i==*j)) return false;
    return true;
}

inline is_within_t ContourWithHoles::IsWithin(XY p) const
{
    is_within_t ret = Contour::IsWithin(p);
    if (ret!=WI_INSIDE) return ret;
    for (auto i = holes.begin(); i!=holes.end(); i++) {
        ret = i->IsWithin(p);
        if (ret!=WI_OUTSIDE)
            return ret == WI_INSIDE ? WI_OUTSIDE : ret;
    }
    return WI_INSIDE;
}

inline void ContourList::Path(cairo_t *cr, bool inv, bool do_this) const
{
    for(auto i = begin(); i!=end(); i++)
        i->Path(cr, inv, do_this);
}

inline void ContourList::Path(cairo_t *cr, bool inv) const
{
    for(auto i = begin(); i!=end(); i++)
        i->Path(cr, inv);
}

inline bool Area::operator <(const Area &b) const
{
    if (mainline!=b.mainline) return mainline<b.mainline;
    return ContourList::operator<(b);
}

inline bool Area::operator ==(const Area &b) const
{
    if (mainline!=b.mainline) return false;
    return ContourList::operator==(b);
}

inline ContourList operator + (const ContourList &p1, const ContourWithHoles &p2) {return std::move(ContourList(p1)+=p2);}
inline ContourList operator * (const ContourList &p1, const ContourWithHoles &p2) {return std::move(ContourList(p1)*=p2);}
inline ContourList operator - (const ContourList &p1, const ContourWithHoles &p2) {return std::move(ContourList(p1)-=p2);}
inline ContourList operator + (ContourList &&p1, const ContourWithHoles &p2) {return std::move(p1+=p2);}
inline ContourList operator * (ContourList &&p1, const ContourWithHoles &p2) {return std::move(p1*=p2);}
inline ContourList operator - (ContourList &&p1, const ContourWithHoles &p2) {return std::move(p1-=p2);}

inline ContourList operator + (const ContourList &p1, const ContourList &p2) {return std::move(ContourList(p1)+=p2);}
inline ContourList operator * (const ContourList &p1, const ContourList &p2) {return std::move(ContourList(p1)*=p2);}
inline ContourList operator - (const ContourList &p1, const ContourList &p2) {return std::move(ContourList(p1)-=p2);}
inline ContourList operator + (ContourList &&p1, const ContourList &p2) {return std::move(p1+=p2);}
inline ContourList operator * (ContourList &&p1, const ContourList &p2) {return std::move(p1*=p2);}
inline ContourList operator - (ContourList &&p1, const ContourList &p2) {return std::move(p1-=p2);}
inline ContourList operator + (const ContourList &p1, ContourList &&p2) {return std::move(p2+=p1);}
inline ContourList operator * (const ContourList &p1, ContourList &&p2) {return std::move(p2*=p1);}
inline ContourList operator + (ContourList &&p1, ContourList &&p2) {return std::move(p2+=p1);}
inline ContourList operator * (ContourList &&p1, ContourList &&p2) {return std::move(p2*=p1);}

inline ContourList operator + (const Contour &p1, Contour &p2) {return std::move(ContourList(p1)+=p2);}
inline ContourList operator * (const Contour &p1, Contour &p2) {return std::move(ContourList(p1)*=p2);}
inline ContourList operator - (const Contour &p1, Contour &p2) {return std::move(ContourList(p1)-=p2);}

inline Area operator + (const Area &a, const Contour &p)  {return std::move(Area(a)+=p);}
inline Area operator * (const Area &a, const Contour &p)  {return std::move(Area(a)*=p);}
inline Area operator - (const Area &a, const Contour &p)  {return std::move(Area(a)-=p);}
inline Area operator + (Area &&a, const Contour &p)  {return std::move(a+=p);}
inline Area operator * (Area &&a, const Contour &p)  {return std::move(a*=p);}
inline Area operator - (Area &&a, const Contour &p)  {return std::move(a-=p);}

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
