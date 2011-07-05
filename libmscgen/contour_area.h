#if !defined(CONTOUR_XAREA_H)
#define CONTOUR_XAREA_H

#include <list>
#include "contour_contour.h"

class TrackableElement;
class ContourWithHoles;

//This can contain multiple positive contours and holes
//holes are always within one of the contours
class ContourList : protected std::list<ContourWithHoles>
{
    friend class ContourWithHoles;
    friend class Contour;
    friend class ContourHelper;
protected:
    Block boundingBox;
    void DoVerticalCrossSection(double x, DoubleMap<bool> &section, bool add) const;
    iterator begin() {return std::list<ContourWithHoles>::begin();}
    iterator end() {return std::list<ContourWithHoles>::end();}
    friend class untangle_node_list;
public:
    ContourList();
    explicit ContourList(const ContourWithHoles &p);
    explicit ContourList(const Contour &p);
    explicit ContourList(ContourWithHoles &&p);
    explicit ContourList(Contour &&p);
    explicit ContourList(const std::vector<XY> &v) {assign(v);}
    ContourList(ContourList &&p) {swap(p);}

    const_iterator begin() const {return std::list<ContourWithHoles>::begin();}
    const_iterator end() const {return std::list<ContourWithHoles>::end();}
    void append(ContourWithHoles &&p);
    void append(const ContourWithHoles &p);
    void append(ContourList &&p) {boundingBox += p.GetBoundingBox(); splice(end(), p);}
    void append(const ContourList &p) {boundingBox += p.GetBoundingBox(); insert(end(), p.begin(), p.end());}
    void swap(ContourList &a) {std::list<ContourWithHoles>::swap(a); std::swap(boundingBox, a.boundingBox);}
    void clear() {std::list<ContourWithHoles>::clear(); boundingBox.MakeInvalid();}
    void assign(const std::vector<XY> &v, bool winding=true);
    bool operator <(const ContourList &b) const;
    bool operator ==(const ContourList &b) const;
    ContourList &operator =(const std::vector<XY> &v) {assign(v); return *this;}

    ContourList &operator = (ContourList &&a) {swap(a); return *this;}
    bool IsEmpty() const {return size()==0;}
    const Block &GetBoundingBox(void) const {return boundingBox;}
    const ContourWithHoles GetFirst() const;
    is_within_t IsWithin(XY p) const;
    void Shift(XY xy);
    ContourList CreateShifted(const XY & xy) const {ContourList a(*this); a.Shift(xy); return std::move(a);}
    void Rotate(double cos, double sin, double radian);
    void RotateAround(const XY&c, double cos, double sin, double radian);
	void Rotate(double degrees) {double r=deg2rad(degrees); Rotate(cos(r), sin(r), r);}
    void RotateAround(const XY&c, double degrees) {double r=deg2rad(degrees); RotateAround(c, cos(r), sin(r), r);}
    void SwapXY();

    ContourList &operator += (const ContourWithHoles &p);
    ContourList &operator *= (const ContourWithHoles &p);
    ContourList &operator -= (const ContourWithHoles &p);
    ContourList &operator ^= (const ContourWithHoles &p);
    ContourList &operator += (const Contour &p);
    ContourList &operator *= (const Contour &p);
    ContourList &operator -= (const Contour &p);
    ContourList &operator ^= (const Contour &p);
    ContourList &operator += (const ContourList &a);
    ContourList &operator *= (const ContourList &a);
    ContourList &operator -= (const ContourList &a);
    ContourList &operator ^= (const ContourList &a);

    void Expand(EExpandType et, double gap, ContourList &result) const;
	void ClearHoles();

    void Path(cairo_t *cr, bool inv) const;
    void Path(cairo_t *cr, bool inv, bool do_this) const;
};

//this contains a list of non-overlapping contours as holes
class ContourWithHoles : public Contour
{
    friend class Contour;
    friend class ContourList;
    friend class Area;
    friend class node_list;
	//hide these - do not implement the one with degrees
    void Rotate(double /*degrees*/) {}
    void RotateAround(const XY& /*c*/, double /*degrees*/) {}
protected:
    friend class ContourHelper;
    ContourList holes;
    ContourWithHoles() {};
    void DoVerticalCrossSection(double x, DoubleMap<bool> &section, bool add) const {Contour::DoVerticalCrossSection(x, section, add); holes.DoVerticalCrossSection(x, section, !add);}
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
    ContourWithHoles CreateShifted(const XY & xy) const {ContourWithHoles a(*this); a.Shift(xy); return std::move(a);}
    void Rotate(double cos, double sin, double radian)
        {Contour::Rotate(cos, sin, radian); if (holes.size()) holes.Rotate(cos, sin, radian);}
    void RotateAround(const XY&c, double cos, double sin, double radian)
        {Contour::RotateAround(c, cos, sin, radian); if (holes.size()) holes.RotateAround(c, cos, sin, radian);}
    void SwapXY() {Contour::SwapXY(); holes.SwapXY();}

    Contour::result_t Add(const ContourWithHoles &p, ContourList &res) const;
    Contour::result_t Mul(const ContourWithHoles &p, ContourList &res) const;
    Contour::result_t Sub(const ContourWithHoles &p, ContourList &res) const;
    Contour::result_t Xor(const ContourWithHoles &p, ContourList &res) const;

	void Expand(EExpandType et, double gap, ContourList &res) const;
	void ClearHoles() {holes.clear();}
    bool HasHoles() const {return holes.size()>0;}
    const ContourList &GetHoles() const {return holes;}

    void Path(cairo_t *cr, bool inv) const
        {Contour::Path(cr, inv); if (holes.size()) holes.Path(cr, !inv);}
    void Path(cairo_t *cr, bool inv, bool do_this) const
        {if (do_this) Contour::Path(cr, inv); if (holes.size()) holes.Path(cr, !inv, !do_this);}
};

//plus it has additional stuff, such as arc, drawtype, findtype and mainline
class Area : public ContourList
{
    friend void test_geo(cairo_t *cr, int x, int y, bool clicked); ///XXX
public:
    mutable TrackableElement *arc;
    Range                     mainline;

    explicit Area(TrackableElement *a=NULL) : arc(a) {mainline.MakeInvalid();}
    Area(const Contour &p, TrackableElement *a=NULL) : ContourList(std::move(p)), arc(a) {mainline.MakeInvalid();}
    Area(Contour &&p, TrackableElement *a=NULL) : ContourList(p), arc(a) {mainline.MakeInvalid();}
    Area(const ContourList &cl, TrackableElement *a=NULL) : ContourList(cl), arc(a) {mainline.MakeInvalid();}
    Area(ContourList &&cl, TrackableElement *a=NULL) : ContourList(std::move(cl)), arc(a) {mainline.MakeInvalid();}

    const_iterator begin() const {return std::list<ContourWithHoles>::begin();}
    const_iterator end() const {return std::list<ContourWithHoles>::end();}
    void clear() {ContourList::clear(); mainline.MakeInvalid();}
    void swap(Area &a);
    void assign(const std::vector<XY> &v) {ContourList::assign(v);}
    bool operator <(const Area &b) const;
    bool operator ==(const Area &b) const;
    Area &operator =(const std::vector<XY> &v) {assign(v); return *this;}
    Area &operator =(Contour &&v) {clear(); append(std::move(v)); return *this;}
    Area &operator =(const Contour &v) {clear(); append(v); return *this;}
    Area &operator += (const Area &b);
    Area &operator *= (const Area &b);
    Area &operator -= (const Area &b);
    Area &operator ^= (const Area &b);
    Area &operator += (const Contour &b) {ContourList::operator+=(b); return *this;}
    Area &operator *= (const Contour &b) {ContourList::operator*=(b); return *this;}
    Area &operator -= (const Contour &b) {ContourList::operator-=(b); return *this;}
    Area &operator ^= (const Contour &b) {ContourList::operator^=(b); return *this;}

    bool IsEmpty() const {return size()==0;}
    const Block &GetBoundingBox(void) const {return boundingBox;}
    is_within_t IsWithin(const XY &p) const;
    void Shift(XY xy) {ContourList::Shift(xy); mainline.Shift(xy.y);}
    Area CreateShifted(const XY & xy) const {Area a(*this); a.Shift(xy); return std::move(a);}
    void Rotate(double degrees) {ContourList::Rotate(degrees);}
    void RotateAround(const XY&c, double degrees) {ContourList::RotateAround(c, degrees);}
    void SwapXY() {ContourList::SwapXY(); mainline.MakeInvalid();}
    void VerticalCrossSection(double x, DoubleMap<bool> &section) const {DoVerticalCrossSection(x, section, true);}

    Area CreateExpand(double gap, EExpandType et=EXPAND_MITER) const;
	void ClearHoles() {ContourList::ClearHoles();}

    double OffsetBelow(const Contour &below, double &touchpoint, double offset=CONTOUR_INFINITY) const;
    double OffsetBelow(const Area &below, double &touchpoint, double offset=CONTOUR_INFINITY, bool bMainline = true) const;

    void Path(cairo_t *cr) const {ContourList::Path(cr, true);}
    void Line(cairo_t *cr) const {ContourList::Path(cr, true); cairo_stroke(cr);}
    void Line2(cairo_t *cr) const;
    void Fill(cairo_t *cr) const {ContourList::Path(cr, true); cairo_fill(cr);}
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
inline const ContourWithHoles ContourList::GetFirst() const {return size()?*begin():ContourWithHoles();}

inline ContourList &ContourList::operator += (const Contour &p) {return *this+=ContourWithHoles(p);}
inline ContourList &ContourList::operator *= (const Contour &p) {return *this*=ContourWithHoles(p);}
inline ContourList &ContourList::operator -= (const Contour &p) {return *this-=ContourWithHoles(p);}
inline ContourList &ContourList::operator ^= (const Contour &p) {return *this^=ContourWithHoles(p);}

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
    if (boundingBox.Overlaps(a.boundingBox)) {
        ContourList result;
        for (auto i = a.begin(); i!=a.end(); i++) {
            ContourList tmp(*this);
            tmp *= *i;
            result.append(tmp);
        }
        swap(result);
    } else
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

inline void ContourList::Shift(XY xy)
{
    for (auto i = begin(); i!=end(); i++)
        i->Shift(xy);
    boundingBox.Shift(xy);
}


inline void ContourList::Rotate(double cos, double sin, double radian)
{
    boundingBox.MakeInvalid();
    for (auto i = begin(); i!=end(); i++) {
        i->Rotate(cos, sin, radian);
        boundingBox += i->GetBoundingBox();
    }
}

inline void ContourList::RotateAround(const XY&c, double cos, double sin, double radian)
{
    boundingBox.MakeInvalid();
    for (auto i = begin(); i!=end(); i++) {
        i->RotateAround(c, cos, sin, radian);
        boundingBox += i->GetBoundingBox();
    }
}

inline void ContourList::ClearHoles()
{
	for (auto i=begin(); i!=end(); i++)
		i->ClearHoles();
}

inline void ContourList::DoVerticalCrossSection(double x, DoubleMap<bool> &section, bool add) const
{
    for (auto i=begin(); i!=end(); i++)
        i->DoVerticalCrossSection(x, section, add);
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

//these are inline to quickly
inline double Area::OffsetBelow(const Contour &below, double &touchpoint, double offset) const
{
    if (offset < below.GetBoundingBox().y.from - boundingBox.y.till) return offset;
    if (!boundingBox.x.Overlaps(below.GetBoundingBox().x)) return offset;
    for (auto i = begin(); i!=end(); i++) {
        double off, tp;
        off = i->OffsetBelow(below, tp, offset);
        if (off < offset) {
            offset = off;
            touchpoint = tp;
        }
    }
    return offset;
}

inline double Area::OffsetBelow(const Area &below, double &touchpoint, double offset, bool bMainline) const
{
    if (offset < below.boundingBox.y.from - boundingBox.y.till) return offset;
    if (bMainline && mainline.HasValidTill() && below.mainline.HasValidFrom())
        if (below.mainline.from - mainline.till < offset) {
            offset = below.mainline.from - mainline.till;
            touchpoint = mainline.till;
        }
    if (!boundingBox.x.Overlaps(below.boundingBox.x)) return offset;
    for (auto i = begin(); i!=end(); i++)
        for (auto j = below.begin(); j!=below.end(); j++) {
            double off, tp;
            off = i->OffsetBelow(*j, tp, offset);
            if (off < offset) {
                offset = off;
                touchpoint = tp;
            }
        }
    return offset;
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

inline ContourList operator + (const ContourList &p1, const Contour &p2) {return ContourList(p1)+=p2;}
inline ContourList operator * (const ContourList &p1, const Contour &p2) {return ContourList(p1)*=p2;}
inline ContourList operator - (const ContourList &p1, const Contour &p2) {return ContourList(p1)-=p2;}
inline ContourList operator ^ (const ContourList &p1, const Contour &p2) {return ContourList(p1)^=p2;}
inline ContourList operator + (const ContourList &p1, const ContourWithHoles &p2) {return ContourList(p1)+=p2;}
inline ContourList operator * (const ContourList &p1, const ContourWithHoles &p2) {return ContourList(p1)*=p2;}
inline ContourList operator - (const ContourList &p1, const ContourWithHoles &p2) {return ContourList(p1)-=p2;}
inline ContourList operator ^ (const ContourList &p1, const ContourWithHoles &p2) {return ContourList(p1)^=p2;}
inline ContourList operator + (ContourList &&p1, const ContourWithHoles &p2) {return p1+=p2;}
inline ContourList operator * (ContourList &&p1, const ContourWithHoles &p2) {return p1*=p2;}
inline ContourList operator - (ContourList &&p1, const ContourWithHoles &p2) {return p1-=p2;}
inline ContourList operator ^ (ContourList &&p1, const ContourWithHoles &p2) {return p1^=p2;}

inline ContourList operator + (const ContourList &p1, const ContourList &p2) {return ContourList(p1)+=p2;}
inline ContourList operator * (const ContourList &p1, const ContourList &p2) {return ContourList(p1)*=p2;}
inline ContourList operator - (const ContourList &p1, const ContourList &p2) {return ContourList(p1)-=p2;}
inline ContourList operator ^ (const ContourList &p1, const ContourList &p2) {return ContourList(p1)^=p2;}
inline ContourList operator + (ContourList &&p1, const ContourList &p2) {return p1+=p2;}
inline ContourList operator * (ContourList &&p1, const ContourList &p2) {return p1*=p2;}
inline ContourList operator - (ContourList &&p1, const ContourList &p2) {return p1-=p2;}
inline ContourList operator ^ (ContourList &&p1, const ContourList &p2) {return p1^=p2;}
inline ContourList operator + (const ContourList &p1, ContourList &&p2) {return p2+=p1;}
inline ContourList operator * (const ContourList &p1, ContourList &&p2) {return p2*=p1;}
//inline ContourList operator - (const ContourList &p1, ContourList &&p2) {return p2-=p1;} NONE!
inline ContourList operator ^ (const ContourList &p1, ContourList &&p2) {return p2^=p1;}

inline Area operator + (const Contour &p1, Contour &p2) {return Area(p1)+=p2;}
inline Area operator * (const Contour &p1, Contour &p2) {return Area(p1)*=p2;}
inline Area operator - (const Contour &p1, Contour &p2) {return Area(p1)-=p2;}
inline Area operator ^ (const Contour &p1, Contour &p2) {return Area(p1)^=p2;}

inline Area operator + (const Area &a, const Contour &p)  {return Area(a)+=p;}
inline Area operator * (const Area &a, const Contour &p)  {return Area(a)*=p;}
inline Area operator - (const Area &a, const Contour &p)  {return Area(a)-=p;}
inline Area operator ^ (const Area &a, const Contour &p)  {return Area(a)^=p;}
inline Area operator + (Area &&a, const Contour &p)  {return a+=p;}
inline Area operator * (Area &&a, const Contour &p)  {return a*=p;}
inline Area operator - (Area &&a, const Contour &p)  {return a-=p;}
inline Area operator ^ (Area &&a, const Contour &p)  {return a^=p;}

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
