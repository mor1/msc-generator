#if !defined(CONTOUR_CONTOURS_H)
#define CONTOUR_CONTOURS_H

#include <list>
#include "contour_simple.h"

class ContourWithHoles;

class ContourList : protected std::list<ContourWithHoles>
{
    friend class ContourWithHoles;
    friend class Contour;
    friend class ContoursHelper;
    friend class MscCanvas;
    Block boundingBox;

    ContourList() {boundingBox.MakeInvalid();}

    bool operator < (const ContourList &p) const;
    bool operator ==(const ContourList &p) const;

    void append(const ContourWithHoles &p);
    void append(ContourWithHoles &&p);
    void append(const ContourList &p) {boundingBox += p.GetBoundingBox(); insert(end(), p.begin(), p.end());}
    void append(ContourList &&p) {boundingBox += p.GetBoundingBox(); splice(end(), p);}
    void swap(ContourList &a) {std::list<ContourWithHoles>::swap(a); std::swap(boundingBox, a.boundingBox);}
    void clear() {std::list<ContourWithHoles>::clear(); boundingBox.MakeInvalid();}

    bool IsEmpty() const {return size()==0;}
    const Block &GetBoundingBox(void) const {return boundingBox;}
    void Invert();

    is_within_t IsWithin(XY p) const;
    void Shift(const XY &xy);
    void SwapXY();
    void Rotate(double cos, double sin, double radian);
    void RotateAround(const XY&c, double cos, double sin, double radian);
    
    void DoVerticalCrossSection(double x, DoubleMap<bool> &section) const;
    double OffsetBelow(const SimpleContour &below, double &touchpoint, double offset) const;
    double OffsetBelow(const ContourList &below, double &touchpoint, double offset) const;

    void Path(cairo_t *cr) const;
    void Path(cairo_t *cr, bool clockwiseonly) const;
};


//this contains a list of non-overlapping contours as holes
class ContourWithHoles : public SimpleContour
{
    friend class SimpleContour;
    friend class ContourList;
    friend class Contour;
    friend class ContoursHelper;
protected:
    ContourList holes;
    ContourWithHoles() {};
    ContourWithHoles(const SimpleContour &p) : SimpleContour(p) {}
public:
    ContourWithHoles(ContourWithHoles &&p) : SimpleContour(std::move(p)), holes(std::move(p.holes)) {}  //for list
protected: 
    friend class MscCanvas;
    ContourWithHoles(double sx, double dx, double sy, double dy) : SimpleContour(sx, dx, sy, dy) {}
    ContourWithHoles(const Block &b) : SimpleContour(b) {}
    ContourWithHoles(XY a, XY b, XY c) : SimpleContour(a,b,c) {}
    ContourWithHoles(double ax, double ay, double bx, double by, double cx, double cy) : SimpleContour(ax, ay, bx, by, cx, cy) {}
    ContourWithHoles(const XY &c, double radius_x, double radius_y=0, double tilt_deg=0, double s_deg=0, double d_deg=360) : 
        SimpleContour (c, radius_x, radius_y, tilt_deg, s_deg, d_deg) {}

    void assign(const SimpleContour &a) {SimpleContour::operator=(a); holes.clear();}
    void assign(SimpleContour &&a) {SimpleContour::swap(a); holes.clear(); }
    void assign(const ContourWithHoles &a) {operator=(a);}
    void assign(ContourWithHoles &&a) {if (this!=&a) swap(a);}
    ContourWithHoles &operator = (const SimpleContour &a) {SimpleContour::operator=(a); holes.clear(); return *this;}
    ContourWithHoles &operator = (SimpleContour &&a) {SimpleContour::swap(a); holes.clear(); return *this;}
    ContourWithHoles &operator = (ContourWithHoles &&a) {if (this!=&a) swap(a); return *this;}

    void swap(ContourWithHoles &b) {holes.swap(b.holes); SimpleContour::swap(b);}
    bool operator < (const ContourWithHoles &p) const {return SimpleContour::operator==(p) ? holes < p.holes : SimpleContour::operator<(p);}
    bool operator ==(const ContourWithHoles &p) const {return SimpleContour::operator==(p) && holes == p.holes;}

    bool HasHoles() const {return !holes.IsEmpty();}
    const Block &GetBoundingBox(void) const {return SimpleContour::GetBoundingBox();}
    void Invert() {SimpleContour::Invert(); if (holes.size()) holes.Invert();}
    bool IsSane(bool shouldbehole=false) const;

    is_within_t IsWithin(const XY &p) const;
    void Shift(const XY &xy) {SimpleContour::Shift(xy); if (holes.size()) holes.Shift(xy);}
    void SwapXY() {SimpleContour::SwapXY(); if (holes.size()) holes.SwapXY();}
    void Rotate(double cos, double sin, double radian)
        {SimpleContour::Rotate(cos, sin, radian); if (holes.size()) holes.Rotate(cos, sin, radian);}
    void RotateAround(const XY&c, double cos, double sin, double radian)
        {SimpleContour::RotateAround(c, cos, sin, radian); if (holes.size()) holes.RotateAround(c, cos, sin, radian);}

    //Call these for both us and the holes (except for OffsetBelow)
    void DoVerticalCrossSection(double x, DoubleMap<bool> &section) const {SimpleContour::DoVerticalCrossSection(x, section); if (holes.size()) holes.DoVerticalCrossSection(x, section);}
    double OffsetBelow(const SimpleContour &below, double &touchpoint, double offset) const {return SimpleContour::OffsetBelow(below, touchpoint, offset);}
    void Expand(EExpandType type, double gap, Contour &res) const;
    void Path(cairo_t *cr) const {SimpleContour::Path(cr); if (holes.size()) holes.Path(cr);}
    void Path(cairo_t *cr, bool clockwiseonly) const {SimpleContour::Path(cr, clockwiseonly); if (holes.size()) holes.Path(cr, clockwiseonly);}
};

//This can contain multiple positive contours and holes
//holes are always within one of the contours
class Contour : public ContourWithHoles
{
    friend class SimpleContour;
    friend class ContourWithHoles;
    friend class ContoursHelper;
    typedef enum {POSITIVE_UNION=0, POSITIVE_INTERSECT, POSITIVE_XOR, 
                  WINDING_RULE_NONZERO, WINDING_RULE_EVENODD, 
                  EXPAND_POSITIVE,
                  NEGATIVE_UNION, NEGATIVE_INTERSECT, NEGATIVE_XOR,
                  EXPAND_NEGATIVE,} operation_t;
    static bool is_positive(operation_t t) {return t<=EXPAND_POSITIVE;}
    ContourList further;
    Block boundingBox;
protected:
    friend class MscCanvas;
    Contour(const ContourWithHoles &p) : ContourWithHoles(p) {boundingBox = ContourWithHoles::GetBoundingBox();}
    Contour(const SimpleContour &p) : ContourWithHoles(p) {boundingBox = ContourWithHoles::GetBoundingBox();}
    Contour(ContourWithHoles &&p) : ContourWithHoles(std::move(p)) {boundingBox = ContourWithHoles::GetBoundingBox();}
    Contour(SimpleContour &&p) : ContourWithHoles(std::move(p)) {boundingBox = ContourWithHoles::GetBoundingBox();}

    Contour &operator = (const SimpleContour &a) {ContourWithHoles::operator=(a); further.clear(); boundingBox = ContourWithHoles::GetBoundingBox(); return *this;}
    Contour &operator = (SimpleContour &&a) {ContourWithHoles::operator=(std::move(a)); boundingBox = ContourWithHoles::GetBoundingBox(); return *this;}
    Contour &operator = (const ContourWithHoles &a) {ContourWithHoles::operator=(a); further.clear(); boundingBox = ContourWithHoles::GetBoundingBox(); return *this;}
    Contour &operator = (ContourWithHoles &&a) {ContourWithHoles::operator=(std::move(a)); further.clear(); boundingBox = ContourWithHoles::GetBoundingBox(); return *this;}

    void append(const ContourWithHoles &p) {if (p.IsEmpty()) return; if (IsEmpty()) {boundingBox = p.GetBoundingBox(); ContourWithHoles::assign(p);} else {boundingBox+=p.GetBoundingBox(); further.append(p);}}
    void append(ContourWithHoles &&p)  {if (p.IsEmpty()) return; if (IsEmpty()) {boundingBox = p.GetBoundingBox(); ContourWithHoles::assign(std::move(p));} else {boundingBox+=p.GetBoundingBox(); further.append(std::move(p));}}

    void Invert() {ContourWithHoles::Invert(); if (further.size()) further.Invert();}
    Contour CreateInverse() const {Contour tmp(*this); tmp.Invert(); return tmp;}
    void Rotate(double cos, double sin, double radian) {ContourWithHoles::Rotate(cos, sin, radian); boundingBox = ContourWithHoles::GetBoundingBox(); if (further.size()) {further.Rotate(cos, sin, radian); boundingBox += further.GetBoundingBox();}}
    void RotateAround(const XY&c, double cos, double sin, double radian) {ContourWithHoles::RotateAround(c, cos, sin, radian); boundingBox = ContourWithHoles::GetBoundingBox(); if (further.size()) {further.RotateAround(c, cos, sin, radian); boundingBox += further.GetBoundingBox();}}

    void Operation(operation_t type, const Contour &c1);
    void Operation(operation_t type, Contour &&c1);
    void Operation(operation_t type, const Contour &c1, const Contour &c2);
    void Operation(operation_t type, const Contour &c1, Contour &&c2);
    void Operation(operation_t type, Contour &&c1, Contour &&c2);

    void Expand(EExpandType type, double gap, Contour &res) const;
public:
    Contour() {boundingBox.MakeInvalid();}
    Contour(double sx, double dx, double sy, double dy) : ContourWithHoles(sx, dx, sy, dy) {boundingBox = ContourWithHoles::GetBoundingBox();}
    Contour(const Block &b) : ContourWithHoles(b), boundingBox(b) {}
    Contour(XY a, XY b, XY c) : ContourWithHoles(a,b,c) {boundingBox = ContourWithHoles::GetBoundingBox();}
    Contour(double ax, double ay, double bx, double by, double cx, double cy) : ContourWithHoles(ax, ay, bx, by, cx, cy) {boundingBox = ContourWithHoles::GetBoundingBox();}
    Contour(const XY &c, double radius_x, double radius_y=0, double tilt_deg=0, double s_deg=0, double d_deg=360) : 
        ContourWithHoles (c, radius_x, radius_y, tilt_deg, s_deg, d_deg) {boundingBox = ContourWithHoles::GetBoundingBox();}
    explicit Contour(const std::vector<XY> &v) {assign(v);}
    explicit Contour(const XY v[], unsigned size) {assign(v, size);}
    template<unsigned size> explicit Contour(const XY (&v)[size]) {assign (v, size);}
    explicit Contour(const std::vector<Edge> &v) {assign(v);}
    explicit Contour(const Edge v[], unsigned size) {assign(v, size);}
    template<unsigned size> explicit Contour(const Edge (&v)[size]) {assign (v, size);}
    Contour(Contour &&p) {swap(p);}

    Contour &operator = (const Block &a) {SimpleContour::operator=(a); further.clear(); boundingBox = a; return *this;}
    Contour &operator = (const std::vector<XY> &v) {assign(v); return *this;}
    Contour &operator = (const std::vector<Edge> &v) {assign(v); return *this;}
    template<unsigned size> Contour &operator = (const XY (&v)[size]) {assign (v, size); return *this;}
    template<unsigned size> Contour &operator = (const Edge (&v)[size]) {assign (v, size);  return *this;}

    void swap(Contour &a) {ContourWithHoles::swap(a); further.swap(a.further); std::swap(boundingBox, a.boundingBox);}
    void clear() {ContourWithHoles::clear(); further.clear(); boundingBox.MakeInvalid();}
    void assign(const std::vector<XY> &v, bool winding=true);
    void assign(const XY v[], unsigned size, bool winding=true);
    template<unsigned size> void assign(const XY (&v)[size], bool winding=true) {assign (v, size, winding);}
    void assign(const std::vector<Edge> &v, bool winding=true);
    void assign(const Edge v[], unsigned size, bool winding=true);
    template<unsigned size> void assign(const Edge (&v)[size], bool winding=true) {assign (v, size, winding);}
    
    bool operator < (const Contour &p) const {return ContourWithHoles::operator==(p) ? further < p.further: SimpleContour::operator<(p);}
    bool operator ==(const Contour &p) const {return ContourWithHoles::operator==(p) && further == p.further;}

    bool IsEmpty() const {return ContourWithHoles::IsEmpty();}
    const Block &GetBoundingBox(void) const {return boundingBox;}
    //bool AddAnEdge(const Edge &edge) {SimpleContour::AddAnEdge(edge);}
    //bool AddPoint(const XY &xy) {return AddAnEdge(Edge(xy, at(0).GetStart()));}
    Contour GetNth(unsigned n);
    void ClearHoles() {holes.clear(); for (auto i = further.begin(); i!=further.end(); i++) i->holes.clear();}
    bool IsSane() const;

    is_within_t IsWithin(const XY &p) const {is_within_t ret = ContourWithHoles::IsWithin(p); if (ret==WI_OUTSIDE) ret = further.IsWithin(p); return ret;}
    void Shift(const XY &xy) {ContourWithHoles::Shift(xy); if (further.size()) further.Shift(xy); boundingBox.Shift(xy);}
    void SwapXY() {ContourWithHoles::SwapXY(); if (further.size()) further.SwapXY(); boundingBox.SwapXY();}
	void Rotate(double degrees) {double r=deg2rad(degrees); Rotate(cos(r), sin(r), r);}
    void RotateAround(const XY&c, double degrees) {double r=deg2rad(degrees); RotateAround(c, cos(r), sin(r), r);}

    Contour CreateShifted(const XY & xy) const {Contour a(*this); a.Shift(xy); return a;}
    Contour CreateSwapXYd() {Contour a(*this); a.SwapXY(); return a;}
	Contour CreateRotated(double degrees) {Contour a(*this); a.Rotate(degrees); return a;}
    Contour CreateRotatedAround(const XY&c, double degrees) {Contour a(*this); a.RotateAround(c, degrees); return a;}

    Contour &operator += (const Contour &a) {Operation(clockwise || a.clockwise ? POSITIVE_UNION : NEGATIVE_UNION, *this, a); return *this;}
    Contour &operator += (Contour &&a)      {Operation(clockwise || a.clockwise ? POSITIVE_UNION : NEGATIVE_UNION, *this, std::move(a)); return *this;}  
    Contour &operator *= (const Contour &a) {Operation(clockwise || a.clockwise ? POSITIVE_INTERSECT : NEGATIVE_INTERSECT, *this, a); return *this;}
    Contour &operator *= (Contour &&a)      {Operation(clockwise || a.clockwise ? POSITIVE_INTERSECT : NEGATIVE_INTERSECT, *this, std::move(a)); return *this;}  
    Contour &operator -= (const Contour &a) {Operation(clockwise || a.clockwise ? POSITIVE_UNION : NEGATIVE_UNION, *this, a.CreateInverse()); return *this;}
    Contour &operator -= (Contour &&a)      {a.Invert(); Operation(clockwise || a.clockwise ? POSITIVE_UNION : NEGATIVE_UNION, *this, std::move(a)); return *this;}  
    Contour &operator ^= (const Contour &a) {Operation(clockwise || a.clockwise ? POSITIVE_XOR : NEGATIVE_XOR, *this, a); return *this;}
    Contour &operator ^= (Contour &&a)      {Operation(clockwise || a.clockwise ? POSITIVE_XOR : NEGATIVE_XOR, *this, std::move(a)); return *this;}

    Contour operator + (const Contour &p) const {return Contour(*this)+=p;}
    Contour operator * (const Contour &p) const {return Contour(*this)*=p;}
    Contour operator - (const Contour &p) const {return Contour(*this)-=p;} 
    Contour operator ^ (const Contour &p) const {return Contour(*this)^=p;}
    Contour operator + (Contour &&p) const {return Contour(*this)+=std::move(p);}
    Contour operator * (Contour &&p) const {return Contour(*this)*=std::move(p);}
    Contour operator - (Contour &&p) const {return Contour(*this)-=std::move(p);}
    Contour operator ^ (Contour &&p) const {return Contour(*this)^=std::move(p);}

    void VerticalCrossSection(double x, DoubleMap<bool> &section) const {ContourWithHoles::DoVerticalCrossSection(x, section); if (further.size()) further.DoVerticalCrossSection(x, section);}
    double OffsetBelow(const SimpleContour &below, double &touchpoint, double offset) const {return SimpleContour::OffsetBelow(below, touchpoint, offset);}
    void Expand(double gap, EExpandType et=EXPAND_MITER);
    Contour CreateExpand(double gap, EExpandType et=EXPAND_MITER) const;

    void Path(cairo_t *cr) const {ContourWithHoles::Path(cr); if (further.size()) further.Path(cr);}
    void Path(cairo_t *cr, bool clockwiseonly) const {ContourWithHoles::Path(cr, clockwiseonly); if (further.size()) further.Path(cr,clockwiseonly);}
    void Line(cairo_t *cr) const {Contour::Path(cr); cairo_stroke(cr);}
    void Line2(cairo_t *cr) const {cairo_save(cr); double dash[]={2,2}; cairo_set_dash(cr, dash, 2, 0); Path(cr, false); cairo_stroke(cr); cairo_set_dash(cr, NULL, 0, 0); Path(cr, true); cairo_stroke(cr); cairo_restore(cr);}
    void Fill(cairo_t *cr) const {Contour::Path(cr); cairo_fill(cr);}
};


inline void ContourList::append(const ContourWithHoles &p) 
{
    if (p.IsEmpty()) return;
    boundingBox += p.GetBoundingBox(); 
    std::list<ContourWithHoles>::push_back(p);
}

inline void ContourList::append(ContourWithHoles &&p) 
{
    if (p.IsEmpty()) return;
    boundingBox += p.GetBoundingBox(); 
    std::list<ContourWithHoles>::push_back(std::move(p));
}


inline is_within_t ContourList::IsWithin(XY p) const
{
    is_within_t ret = boundingBox.IsWithin(p);
    if (ret==WI_OUTSIDE) return WI_OUTSIDE;
    for (auto i = begin(); i!=end(); i++) 
        if (WI_OUTSIDE != (ret=i->IsWithin(p))) return ret; //we also return IN_HOLE
    return WI_OUTSIDE;
}

inline void ContourList::Shift(const XY &xy)
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

///////ContourWithHoles///////////////////////////////////////

inline void ContourWithHoles::Expand(EExpandType type, double gap, Contour &res) const 
{
    if (size()==0) return;
    if (gap==0) {res.clear(); static_cast<ContourWithHoles&>(res) = *this; return;}
    SimpleContour::Expand(type, gap, res); 
    if (holes.size()==0 || res.IsEmpty()) return;
    Contour tmp; 
    for (auto i=holes.begin(); i!=holes.end(); i++) {
        i->Expand(type, gap, tmp);
        //in case "i" is an actual holes, it is are already inversed, adding is the right op
        res.Operation(GetClockWise() ? Contour::POSITIVE_UNION : Contour::NEGATIVE_UNION, res, std::move(tmp));
        res += std::move(tmp); 
        tmp.clear();
    }
}

///////Contour///////////////////////////////////////

inline is_within_t ContourWithHoles::IsWithin(const XY &p) const
{
    is_within_t ret = SimpleContour::IsWithin(p);
    if (ret!=WI_INSIDE || holes.size()==0) return ret;
    ret = holes.IsWithin(p);
    switch (ret) {
    case WI_INSIDE: return WI_IN_HOLE;
    case WI_IN_HOLE:
    case WI_OUTSIDE: return WI_INSIDE;
    default: return ret;
    }
}

inline void Contour::Expand(double gap, EExpandType et) 
{
    if (gap) {
        Contour res; 
        Expand(et, gap, res); 
        swap(res);
    }
}

inline Contour Contour::CreateExpand(double gap, EExpandType et) const 
{
    if (!gap) return *this; 
    Contour res; 
    Expand(et, gap, res); 
    return res;
}


#endif //CONTOUR_CONTOURS_H
