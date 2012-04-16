#if !defined(CONTOUR_XAREA_H)
#define CONTOUR_XAREA_H

#include <list>
#include "contour.h"

class TrackableElement;
//plus it has additional stuff, such as arc and mainline

class Area : public contour::Contour
{
public:
    mutable TrackableElement *arc;
    contour::Contour          mainline;

    explicit Area(TrackableElement *a=NULL) : arc(a) {}
    Area(const Contour &cl, TrackableElement *a=NULL) : Contour(cl), arc(a) {}
    Area(Contour &&cl, TrackableElement *a=NULL) : Contour(std::move(cl)), arc(a) {}
    Area(const contour::Block &b, TrackableElement *a=NULL) : Contour(b), arc(a) {}

    void clear() {Contour::clear(); mainline.clear();}
    void swap(Area &a);
    void assign(const std::vector<contour::XY> &v, bool winding=true)  {Contour::assign(v, winding); mainline.clear();}
    void assign(const contour::XY v[], unsigned size, bool winding=true)  {Contour::assign(v, size, winding); mainline.clear();}
    template<unsigned SIZE> void assign(const contour::XY v[SIZE], bool winding=true) {assign (v, SIZE, winding);}
    void assign(const std::vector<contour::Edge> &v, bool winding=true)  {Contour::assign(v, winding); mainline.clear();}
    void assign(const contour::Edge v[], unsigned size, bool winding=true)  {Contour::assign(v, size, winding); mainline.clear();}
    template<unsigned SIZE> void assign(const contour::Edge v[SIZE], bool winding=true) {assign (v, SIZE, winding);}

    bool operator <(const Area &b) const;
    bool operator ==(const Area &b) const;
    Area &operator =(const contour::Block &a) {Contour::operator=(a); mainline.clear(); return *this;}
    Area &operator =(const Contour &a) {Contour::operator=(a); mainline.clear(); return *this;}
    Area &operator =(Contour &&a) {Contour::operator=(std::move(a)); mainline.clear(); return *this;}

    Area &operator += (const Area &b) {Contour::operator+=(b); mainline+=b.mainline; if (arc==NULL) arc = b.arc; return *this;}
    Area &operator *= (const Area &b) {Contour::operator*=(b); mainline*=b.mainline; if (arc==NULL) arc = b.arc; return *this;}
    Area &operator -= (const Area &b) {Contour::operator-=(b); mainline-=b.mainline; if (arc==NULL) arc = b.arc; return *this;}
    Area &operator ^= (const Area &b) {Contour::operator^=(b);                       if (arc==NULL) arc = b.arc; return *this;}
    Area &operator += (Area &&b) {Contour::operator+=(std::move(b)); mainline+=std::move(b.mainline); if (arc==NULL) arc = b.arc; return *this;}
    Area &operator *= (Area &&b) {Contour::operator*=(std::move(b)); mainline*=std::move(b.mainline); if (arc==NULL) arc = b.arc; return *this;}
    Area &operator -= (Area &&b) {Contour::operator-=(std::move(b)); mainline-=std::move(b.mainline); if (arc==NULL) arc = b.arc; return *this;}
    Area &operator ^= (Area &&b) {Contour::operator^=(std::move(b));                                  if (arc==NULL) arc = b.arc; return *this;}

    Area operator + (const Area &p) const {return Area(*this)+=p;}
    Area operator * (const Area &p) const {return Area(*this)*=p;}
    Area operator - (const Area &p) const {return Area(*this)-=p;}
    Area operator ^ (const Area &p) const {return Area(*this)^=p;}
    Area operator + (Area &&p) const {return Area(*this)+=std::move(p);}
    Area operator * (Area &&p) const {return Area(*this)*=std::move(p);}
    Area operator - (Area &&p) const {return Area(*this)-=std::move(p);}
    Area operator ^ (Area &&p) const {return Area(*this)^=std::move(p);}

    Area& Shift(contour::XY xy) {Contour::Shift(xy); mainline.Shift(xy); return *this;}
    Area CreateShifted(const contour::XY & xy) const {Area a(*this); a.Shift(xy); return std::move(a);}
    Area& Rotate(double degrees) {Contour::Rotate(degrees); return *this;}
    Area& RotateAround(const contour::XY&c, double degrees) {Contour::RotateAround(c, degrees); return *this;}
    Area& SwapXY() {Contour::SwapXY(); mainline.clear(); return *this;}

    Area CreateExpand(double gap, contour::EExpandType et4pos=contour::EXPAND_MITER_ROUND, contour::EExpandType et4neg=contour::EXPAND_MITER_ROUND,
                      double miter_limit_positive=CONTOUR_INFINITY, double miter_limit_negative=CONTOUR_INFINITY) const;
    Area &Expand(double gap, contour::EExpandType et4pos=contour::EXPAND_MITER_ROUND, contour::EExpandType et4neg=contour::EXPAND_MITER_ROUND,
                 double miter_limit_positive=CONTOUR_INFINITY, double miter_limit_negative=CONTOUR_INFINITY)
         {Contour::Expand(gap, et4pos, et4neg, miter_limit_positive, miter_limit_negative); mainline.Expand(gap); return *this;}
	void ClearHoles() {Contour::ClearHoles();}

    double OffsetBelow(const Area &below, double &touchpoint, double offset=CONTOUR_INFINITY, bool bMainline = true) const;
};

class AreaList
{
    std::list<Area>  cover;
    contour::Block   boundingBox;
public:
    contour::Contour mainline;
    AreaList() {boundingBox.MakeInvalid();}
    AreaList(const Area &area) {boundingBox.MakeInvalid(); operator+=(area);}
    AreaList(Area &&area) {boundingBox.MakeInvalid(); operator+=(std::move(area));}
    AreaList(const AreaList &al) : cover(al.cover), boundingBox(al.boundingBox), mainline(al.mainline) {}
    AreaList(AreaList &&al) : cover(std::move(al.cover)), boundingBox(al.boundingBox), mainline(std::move(al.mainline)) {}
    const std::list<Area> &GetCover() const {return cover;}
    void clear() {boundingBox.MakeInvalid(); mainline.clear(); cover.clear();}
    size_t size() const {return cover.size();}
    std::list<Area>::const_iterator begin() const {return cover.begin();}
    std::list<Area>::const_iterator end() const {return cover.end();}
    void swap(AreaList &o) {cover.swap(o.cover); std::swap(boundingBox, o.boundingBox); std::swap(mainline, o.mainline);}
    void SetArc(TrackableElement *a) const {for(auto i=cover.begin(); i!=cover.end(); i++) i->arc = a;}
    AreaList &operator+=(const Area &b) {cover.push_back(b); boundingBox+=b.GetBoundingBox(); mainline+=b.mainline; return *this;}
    AreaList &operator+=(Area &&b) {cover.push_back(std::move(b)); boundingBox+=b.GetBoundingBox(); mainline+=std::move(b.mainline); return *this;}
    AreaList &operator+=(const AreaList &g) {cover.insert(cover.end(), g.cover.begin(), g.cover.end()); boundingBox+=g.boundingBox; mainline+=g.mainline; return *this;}
    AreaList &operator+=(AreaList &&g) {cover.splice(cover.end(), g.cover); boundingBox+=g.boundingBox; mainline+=std::move(g.mainline); return *this;}
    bool IsEmpty() const {return cover.size()==0;}
    AreaList &Shift(contour::XY xy) {for (auto i=cover.begin(); i!=cover.end(); i++) i->Shift(xy); mainline.Shift(xy); boundingBox.Shift(xy); return *this;}
    const contour::Block& GetBoundingBox() const {return boundingBox;}

    AreaList CreateExpand(double gap, contour::EExpandType et4pos=contour::EXPAND_MITER_ROUND, contour::EExpandType et4neg=contour::EXPAND_MITER_ROUND,
                          double miter_limit_positive=CONTOUR_INFINITY, double miter_limit_negative=CONTOUR_INFINITY) const;

    double OffsetBelow(const Area &below, double &touchpoint, double offset=CONTOUR_INFINITY, bool bMainline=true) const;
    double OffsetBelow(const AreaList &below, double &touchpoint, double offset=CONTOUR_INFINITY, bool bMainline=true) const;

    template <typename T> void sort(T t) {cover.sort(t);}

    const Area *InWhich(const contour::XY &p) const {for (auto i=cover.begin(); i!=cover.end(); i++) if (inside(i->IsWithin(p))) return &*i; return NULL;}
    const Area *InWhichFromBack(const contour::XY &p) const {for (auto i=cover.rbegin(); !(i==cover.rend()); i++) if (inside(i->IsWithin(p))) return &*i; return NULL;}
    void InvalidateMainLine() {mainline.clear(); for (auto i=cover.begin(); i!=cover.end(); i++) i->mainline.clear();}
};

inline bool Area::operator <(const Area &b) const
{
    if (arc!=b.arc) return arc<b.arc;
    if (mainline!=b.mainline) return mainline<b.mainline;
    return Contour::operator<(b);
}

inline bool Area::operator ==(const Area &b) const
{
    if (arc!=arc) return false;
    if (mainline!=b.mainline) return false;
    return Contour::operator==(b);
}


#endif //CONTOUR_XAREA_H
