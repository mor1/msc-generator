#if !defined(CONTOUR_XAREA_H)
#define CONTOUR_XAREA_H

#include <list>
#include "contour_contours.h"

class TrackableElement;
//plus it has additional stuff, such as arc, drawtype, findtype and mainline
class Area : public Contours
{
    friend void test_geo(cairo_t *cr, int x, int y, bool clicked); ///XXX
public:
    mutable TrackableElement *arc;
    Range                     mainline;

    explicit Area(TrackableElement *a=NULL) : arc(a) {mainline.MakeInvalid();}
    Area(const Contour &p, TrackableElement *a=NULL) : Contours(std::move(p)), arc(a) {mainline.MakeInvalid();}
    Area(Contour &&p, TrackableElement *a=NULL) : Contours(p), arc(a) {mainline.MakeInvalid();}
    Area(const Contours &cl, TrackableElement *a=NULL) : Contours(cl), arc(a) {mainline.MakeInvalid();}
    Area(Contours &&cl, TrackableElement *a=NULL) : Contours(std::move(cl)), arc(a) {mainline.MakeInvalid();}

    void clear() {Contours::clear(); mainline.MakeInvalid();}
    void swap(Area &a);
    void assign(const std::vector<XY> &v) {Contours::assign(v);}
    bool operator <(const Area &b) const;
    bool operator ==(const Area &b) const;
    Area &operator =(const std::vector<XY> &v) {assign(v); return *this;}
    Area &operator =(const Contour &a) {Contours::operator=(a); mainline.MakeInvalid(); return *this;}
    Area &operator =(Contour &&a) {Contours::operator=(std::move(a)); mainline.MakeInvalid(); return *this;}
    Area &operator =(const Contours &a) {Contours::operator=(a); mainline.MakeInvalid(); return *this;}
    Area &operator =(Contours &&a) {Contours::operator=(std::move(a)); mainline.MakeInvalid(); return *this;}
    
    Area &operator += (const Area &b);
    Area &operator *= (const Area &b);
    Area &operator -= (const Area &b);
    Area &operator ^= (const Area &b);
    Area &operator += (const Contours &b) {Contours::operator+=(b); return *this;}
    Area &operator *= (const Contours &b) {Contours::operator*=(b); return *this;}
    Area &operator -= (const Contours &b) {Contours::operator-=(b); return *this;}
    Area &operator ^= (const Contours &b) {Contours::operator^=(b); return *this;}
    Area &operator += (const Contour &b) {Contours::operator+=(b); return *this;}
    Area &operator *= (const Contour &b) {Contours::operator*=(b); return *this;}
    Area &operator -= (const Contour &b) {Contours::operator-=(b); return *this;}
    Area &operator ^= (const Contour &b) {Contours::operator^=(b); return *this;}

    void Shift(XY xy) {Contours::Shift(xy); mainline.Shift(xy.y);}
    Area CreateShifted(const XY & xy) const {Area a(*this); a.Shift(xy); return std::move(a);}
    void Rotate(double degrees) {Contours::Rotate(degrees);}
    void RotateAround(const XY&c, double degrees) {Contours::RotateAround(c, degrees);}
    void SwapXY() {Contours::SwapXY(); mainline.MakeInvalid();}
    void VerticalCrossSection(double x, DoubleMap<bool> &section) const {DoVerticalCrossSection(x, section, true);}

    Area CreateExpand(double gap, EExpandType et=EXPAND_MITER) const;
	void ClearHoles() {Contours::ClearHoles();}

    //double OffsetBelow(const Contour &below, double &touchpoint, double offset=CONTOUR_INFINITY) const;
    double OffsetBelow(const Area &below, double &touchpoint, double offset=CONTOUR_INFINITY, bool bMainline = true) const;

    void Path(cairo_t *cr) const {Contours::Path(cr, true);}
    void Line(cairo_t *cr) const {Contours::Path(cr, true); cairo_stroke(cr);}
    void Line2(cairo_t *cr) const;
    void Fill(cairo_t *cr) const {Contours::Path(cr, true); cairo_fill(cr);}
    void Fill2(cairo_t *cr, int r=255, int g=255, int b=255) const;

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

    AreaList CreateExpand(double gap, EExpandType et=EXPAND_MITER) const;

    double OffsetBelow(const Area &below, double &touchpoint, double offset=CONTOUR_INFINITY, bool bMainline=true) const;
    double OffsetBelow(const AreaList &below, double &touchpoint, double offset=CONTOUR_INFINITY, bool bMainline=true) const;

    const Area *InWhich(const XY &p) const {for (auto i=cover.begin(); i!=cover.end(); i++) if (i->IsWithin(p)!=WI_OUTSIDE) return &*i; return NULL;}
    const Area *InWhichFromBack(const XY &p) const {for (auto i=cover.rbegin(); !(i==cover.rend()); i++) if (i->IsWithin(p)!=WI_OUTSIDE) return &*i; return NULL;}
    void InvalidateMainLine() {mainline.MakeInvalid(); for (auto i=cover.begin(); i!=cover.end(); i++) i->mainline.MakeInvalid();}
};



inline bool Area::operator <(const Area &b) const
{
    if (mainline!=b.mainline) return mainline<b.mainline;
    return Contours::operator<(b);
}

inline bool Area::operator ==(const Area &b) const
{
    if (mainline!=b.mainline) return false;
    return Contours::operator==(b);
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
    return Contours::OffsetBelow(below, touchpoint, offset);
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


inline Area operator + (const Area &a, const Contour &p)  {return Area(a)+=p;}
inline Area operator * (const Area &a, const Contour &p)  {return Area(a)*=p;}
inline Area operator - (const Area &a, const Contour &p)  {return Area(a)-=p;}
inline Area operator ^ (const Area &a, const Contour &p)  {return Area(a)^=p;}
inline Area operator + (Area &&a, const Contour &p)  {return a+=p;}
inline Area operator * (Area &&a, const Contour &p)  {return a*=p;}
inline Area operator - (Area &&a, const Contour &p)  {return a-=p;}
inline Area operator ^ (Area &&a, const Contour &p)  {return a^=p;}

inline Area operator + (const Contour &p, const Area &a)  {return Area(p, a.arc)+=a;}
inline Area operator * (const Contour &p, const Area &a)  {return Area(p, a.arc)*=a;}
inline Area operator - (const Contour &p, const Area &a)  {return Area(p, a.arc)-=a;}
inline Area operator ^ (const Contour &p, const Area &a)  {return Area(p, a.arc)^=a;}
inline Area operator + (Contour &&p, const Area &a)  {return Area(std::move(p), a.arc)+=a;}
inline Area operator * (Contour &&p, const Area &a)  {return Area(std::move(p), a.arc)*=a;}
inline Area operator - (Contour &&p, const Area &a)  {return Area(std::move(p), a.arc)-=a;}
inline Area operator ^ (Contour &&p, const Area &a)  {return Area(std::move(p), a.arc)^=a;}

inline Area operator + (const Area &a1, const Area &a2)  {return Area(a1)+=a2;}
inline Area operator * (const Area &a1, const Area &a2)  {return Area(a1)*=a2;}
inline Area operator - (const Area &a1, const Area &a2)  {return Area(a1)-=a2;}
inline Area operator ^ (const Area &a1, const Area &a2)  {return Area(a1)^=a2;}
inline Area operator + (Area &&a1, const Area &a2)  {return a1+=a2;}
inline Area operator * (Area &&a1, const Area &a2)  {return a1*=a2;}
inline Area operator - (Area &&a1, const Area &a2)  {return a1-=a2;}
inline Area operator ^ (Area &&a1, const Area &a2)  {return a1^=a2;}
inline Area operator + (const Area &a1, Area &&a2)  {return a2+=a1;}
inline Area operator * (const Area &a1, Area &&a2)  {return a2*=a1;}
//inline Area operator - (const Area &a1, Area &&a2)  {return std::move(a2-=a1);} NONE!
inline Area operator ^ (const Area &a1, Area &&a2)  {return a2^=a1;}

#endif //CONTOUR_XAREA_H
