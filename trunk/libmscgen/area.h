#if !defined(CONTOUR_XAREA_H)
#define CONTOUR_XAREA_H

#include <list>
#include "contour.h"

using contour::XY;
using contour::Range;
using contour::Block;
using contour::Edge;
using contour::Contour;

using contour::DoubleMap;
using contour::fmod_negative_safe;

using contour::EXPAND_MITER;
using contour::EXPAND_BEVEL;
using contour::EXPAND_ROUND;

class TrackableElement;
//plus it has additional stuff, such as arc, drawtype, findtype and mainline
class Area : public Contour
{
    friend void test_geo(cairo_t *cr, int x, int y, bool clicked); ///XXX
public:
    mutable TrackableElement *arc;
    Range                     mainline;

    explicit Area(TrackableElement *a=NULL) : arc(a) {mainline.MakeInvalid();}
    Area(const Contour &cl, TrackableElement *a=NULL) : Contour(cl), arc(a) {mainline.MakeInvalid();}
    Area(Contour &&cl, TrackableElement *a=NULL) : Contour(std::move(cl)), arc(a) {mainline.MakeInvalid();}
    Area(const Block &b, TrackableElement *a=NULL) : Contour(b), arc(a) {mainline.MakeInvalid();}

    void clear() {Contour::clear(); mainline.MakeInvalid();}
    void swap(Area &a);
    void assign(const std::vector<XY> &v, bool winding=true)  {Contour::assign(v); mainline.MakeInvalid();}
    void assign(const XY v[], unsigned size, bool winding=true)  {Contour::assign(v, size); mainline.MakeInvalid();}
    template<unsigned size> void assign(const XY v[size], bool winding=true) {assign (v, size, winding);}
    void assign(const std::vector<Edge> &v, bool winding=true)  {Contour::assign(v); mainline.MakeInvalid();}
    void assign(const Edge v[], unsigned size, bool winding=true)  {Contour::assign(v, size); mainline.MakeInvalid();}
    bool operator <(const Area &b) const;
    bool operator ==(const Area &b) const;
    Area &operator =(const Block &a) {Contour::operator=(a); mainline.MakeInvalid(); return *this;}
    Area &operator =(const Contour &a) {Contour::operator=(a); mainline.MakeInvalid(); return *this;}
    Area &operator =(Contour &&a) {Contour::operator=(std::move(a)); mainline.MakeInvalid(); return *this;}
    
    Area &operator += (const Area &b) {Contour::operator+=(b); mainline+=b.mainline; if (arc==NULL) arc = b.arc; return *this;}
    Area &operator *= (const Area &b) {Contour::operator*=(b); mainline*=b.mainline; if (arc==NULL) arc = b.arc; return *this;}
    Area &operator -= (const Area &b) {Contour::operator+=(b); mainline-=b.mainline; if (arc==NULL) arc = b.arc; return *this;}
    Area &operator ^= (const Area &b) {Contour::operator+=(b);                       if (arc==NULL) arc = b.arc; return *this;}
    Area &operator += (Area &&b) {Contour::operator+=(std::move(b)); mainline+=b.mainline; if (arc==NULL) arc = b.arc; return *this;}
    Area &operator *= (Area &&b) {Contour::operator*=(std::move(b)); mainline*=b.mainline; if (arc==NULL) arc = b.arc; return *this;}
    Area &operator -= (Area &&b) {Contour::operator+=(std::move(b)); mainline-=b.mainline; if (arc==NULL) arc = b.arc; return *this;}
    Area &operator ^= (Area &&b) {Contour::operator+=(std::move(b));                       if (arc==NULL) arc = b.arc; return *this;}

    Area operator + (const Area &p) const {return Area(*this)+=p;}
    Area operator * (const Area &p) const {return Area(*this)*=p;}
    Area operator - (const Area &p) const {return Area(*this)-=p;} 
    Area operator ^ (const Area &p) const {return Area(*this)^=p;}
    Area operator + (Area &&p) const {return Area(*this)+=std::move(p);}
    Area operator * (Area &&p) const {return Area(*this)*=std::move(p);}
    Area operator - (Area &&p) const {return Area(*this)-=std::move(p);}
    Area operator ^ (Area &&p) const {return Area(*this)^=std::move(p);}

    void Shift(XY xy) {Contour::Shift(xy); mainline.Shift(xy.y);}
    Area CreateShifted(const XY & xy) const {Area a(*this); a.Shift(xy); return std::move(a);}
    void Rotate(double degrees) {Contour::Rotate(degrees);}
    void RotateAround(const XY&c, double degrees) {Contour::RotateAround(c, degrees);}
    void SwapXY() {Contour::SwapXY(); mainline.MakeInvalid();}

    Area CreateExpand(double gap, contour::EExpandType et=EXPAND_MITER) const;
	void ClearHoles() {Contour::ClearHoles();}

    double OffsetBelow(const Area &below, double &touchpoint, double offset=CONTOUR_INFINITY, bool bMainline = true) const;
};

class AreaList
{
    std::list<Area> cover;
    Block           boundingBox;
public:
    Range           mainline;
    AreaList() {boundingBox.MakeInvalid(); mainline.MakeInvalid();}
    AreaList(const Area &area) {boundingBox.MakeInvalid(); mainline.MakeInvalid(); operator+=(area);}
    AreaList(Area &&area) {boundingBox.MakeInvalid(); mainline.MakeInvalid(); operator+=(std::move(area));}
    AreaList(AreaList &&al) : cover(std::move(al.cover)), boundingBox(al.boundingBox), mainline(al.mainline) {}
    const std::list<Area> &GetCover() const {return cover;}
    void clear() {boundingBox.MakeInvalid(); mainline.MakeInvalid(); cover.clear();}
    void swap(AreaList &o) {cover.swap(o.cover); std::swap(boundingBox, o.boundingBox); std::swap(mainline, o.mainline);}
    void SetArc(TrackableElement *a) const {for(auto i=cover.begin(); i!=cover.end(); i++) i->arc = a;}
    AreaList &operator+=(const Area &b) {cover.push_back(b); boundingBox+=b.GetBoundingBox(); mainline+=b.mainline; return *this;}
    AreaList &operator+=(Area &&b) {cover.push_back(std::move(b)); boundingBox+=b.GetBoundingBox(); mainline+=b.mainline; return *this;}
    AreaList &operator+=(const AreaList &g) {cover.insert(cover.end(), g.cover.begin(), g.cover.end()); boundingBox+=g.boundingBox; mainline+=g.mainline; return *this;}
    AreaList &operator+=(AreaList &&g) {cover.splice(cover.end(), g.cover); boundingBox+=g.boundingBox; mainline+=g.mainline; return *this;}
    bool IsEmpty() const {return cover.size()==0;}
    AreaList &Shift(XY xy) {for (auto i=cover.begin(); i!=cover.end(); i++) i->Shift(xy); mainline.Shift(xy.y); boundingBox.Shift(xy); return *this;}
    const Block& GetBoundingBox() const {return boundingBox;}

    AreaList CreateExpand(double gap, contour::EExpandType et=EXPAND_MITER) const;

    double OffsetBelow(const Area &below, double &touchpoint, double offset=CONTOUR_INFINITY, bool bMainline=true) const;
    double OffsetBelow(const AreaList &below, double &touchpoint, double offset=CONTOUR_INFINITY, bool bMainline=true) const;

    const Area *InWhich(const XY &p) const {for (auto i=cover.begin(); i!=cover.end(); i++) if (inside(i->IsWithin(p))) return &*i; return NULL;}
    const Area *InWhichFromBack(const XY &p) const {for (auto i=cover.rbegin(); !(i==cover.rend()); i++) if (inside(i->IsWithin(p))) return &*i; return NULL;}
    void InvalidateMainLine() {mainline.MakeInvalid(); for (auto i=cover.begin(); i!=cover.end(); i++) i->mainline.MakeInvalid();}
};



inline bool Area::operator <(const Area &b) const
{
    if (mainline!=b.mainline) return mainline<b.mainline;
    return Contour::operator<(b);
}

inline bool Area::operator ==(const Area &b) const
{
    if (mainline!=b.mainline) return false;
    return Contour::operator==(b);
}

inline double Area::OffsetBelow(const Area &below, double &touchpoint, double offset, bool bMainline) const
{
    if (offset < below.GetBoundingBox().y.from - GetBoundingBox().y.till) return offset;
    if (bMainline && mainline.HasValidTill() && below.mainline.HasValidFrom())
        if (below.mainline.from - mainline.till < offset) {
            offset = below.mainline.from - mainline.till;
            touchpoint = mainline.till;
        }
    if (!GetBoundingBox().x.Overlaps(below.GetBoundingBox().x)) return offset;
    return Contour::OffsetBelow(below, touchpoint, offset);
}

inline double AreaList::OffsetBelow(const Area &below, double &touchpoint, double offset, bool bMainline) const
{
    if (offset < below.GetBoundingBox().y.from - boundingBox.y.till) return offset;
    if (!boundingBox.x.Overlaps(below.GetBoundingBox().x)) return offset;
    if (bMainline && mainline.HasValidTill() && below.mainline.HasValidFrom())
        if (below.mainline.from - mainline.till < offset) {
            offset = below.mainline.from - mainline.till;
            touchpoint = mainline.till;
        }
    for (auto i = cover.begin(); i!=cover.end(); i++)
        offset = std::min(offset, i->OffsetBelow(below, touchpoint, offset, bMainline));
    return offset;
}

inline double AreaList::OffsetBelow(const AreaList &below, double &touchpoint, double offset, bool bMainline) const
{
    if (offset < below.boundingBox.y.from - boundingBox.y.till) return offset;
    if (bMainline && mainline.HasValidTill() && below.mainline.HasValidFrom())
        if (below.mainline.from - mainline.till < offset) {
            offset = below.mainline.from - mainline.till;
            touchpoint = mainline.till;
        }
    if (!boundingBox.x.Overlaps(below.boundingBox.x)) return offset;
    for (auto i = cover.begin(); i!=cover.end(); i++)
        for (auto j = below.cover.begin(); j!=below.cover.end(); j++)
            offset = std::min(offset, i->OffsetBelow(*j, touchpoint, offset, bMainline));
    return offset;
}

#endif //CONTOUR_XAREA_H
