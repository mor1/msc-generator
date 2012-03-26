#if !defined(CONTOUR_CONTOURS_H)
#define CONTOUR_CONTOURS_H

#include <list>
#include "contour_simple.h"

namespace contour {

class ContourWithHoles;

//should only contain contours with the same clockwiseness
class ContourList : protected std::list<ContourWithHoles>
{
    friend class ContourWithHoles;
    friend class Contour;
    friend class ContoursHelper;
    Block boundingBox;

    ContourList() {boundingBox.MakeInvalid();}

    void append(const ContourWithHoles &p);
    void append(ContourWithHoles &&p);
    void append(const ContourList &p) {_ASSERT(p.size()==0 || size()==0 || GetClockWise() == p.GetClockWise()); boundingBox += p.GetBoundingBox(); insert(end(), p.begin(), p.end());}
    void append(ContourList &&p) {_ASSERT(p.size()==0 || size()==0 || GetClockWise() == p.GetClockWise()); boundingBox += p.GetBoundingBox(); splice(end(), p);}

    void Invert();

    void Shift(const XY &xy);
    void Scale(double sc);
    void SwapXY();
    void Rotate(double cos, double sin, double radian);
    void RotateAround(const XY&c, double cos, double sin, double radian);

    SimpleContour::result_t RelationTo(const ContourWithHoles &c, bool ignore_holes) const;
    SimpleContour::result_t RelationTo(const ContourList &c, bool ignore_holes) const;
    void Distance(const ContourWithHoles &c, DistanceType &dist_so_far) const;
    void Distance(const ContourList &cl, DistanceType &dist_so_far) const;
public:
    const ContourWithHoles & operator[](size_type i) const;
    void swap(ContourList &a) {std::list<ContourWithHoles>::swap(a); std::swap(boundingBox, a.boundingBox);}
    void clear() {std::list<ContourWithHoles>::clear(); boundingBox.MakeInvalid();}
    size_type size() const {return std::list<ContourWithHoles>::size();}
    bool operator < (const ContourList &p) const;
    bool operator ==(const ContourList &p) const;
    bool IsEmpty() const {return size()==0;}
    const Block &GetBoundingBox(void) const {return boundingBox;}
    bool GetClockWise() const;
    double GetArea(bool consider_holes=true) const;
    double GetCircumference(bool consider_holes=true, bool include_hidden=false) const;
    XY CentroidUpscaled() const;

    is_within_t IsWithin(XY p) const;
    void VerticalCrossSection(double x, DoubleMap<bool> &section) const;
    double OffsetBelow(const SimpleContour &below, double &touchpoint, double offset=CONTOUR_INFINITY) const;
    double OffsetBelow(const Contour &below, double &touchpoint, double offset=CONTOUR_INFINITY) const;
    double OffsetBelow(const ContourList &below, double &touchpoint, double offset=CONTOUR_INFINITY) const;

    void Path(cairo_t *cr, bool show_hidden) const;
    void Path(cairo_t *cr, bool show_hidden, bool clockwiseonly) const;
    void PathDashed(cairo_t *cr, const double pattern[], unsigned num, bool show_hidden) const;
    void PathDashed(cairo_t *cr, const double pattern[], unsigned num, bool show_hidden, bool clockwiseonly) const;

    Range Cut(const XY &A, const XY &B) const;
    void Cut(const XY &A, const XY &B, DoubleMap<bool> &map) const;
    bool TangentFrom(const XY &from, XY &clockwise, XY &cclockwise) const;
};

//this contains a list of non-overlapping contours as holes
class ContourWithHoles
{
    friend class SimpleContour;
    friend class ContourList;
    friend class Contour;
    friend class ContoursHelper;
protected:
    SimpleContour outline;
    ContourList   holes;

    ContourWithHoles() {};
    ContourWithHoles(double sx, double dx, double sy, double dy) : outline(sx, dx, sy, dy) {}
    ContourWithHoles(const Block &b) : outline(b) {}
    ContourWithHoles(XY a, XY b, XY c) : outline(a,b,c) {}
    ContourWithHoles(double ax, double ay, double bx, double by, double cx, double cy) : outline(ax, ay, bx, by, cx, cy) {}
    ContourWithHoles(const XY &c, double radius_x, double radius_y=0, double tilt_deg=0, double s_deg=0, double d_deg=360) :
        outline(c, radius_x, radius_y, tilt_deg, s_deg, d_deg) {}

    void assign(const SimpleContour &a) {outline = a; holes.clear();}
    void assign(SimpleContour &&a) {outline.swap(a); holes.clear(); }
    void assign(const ContourWithHoles &a) {operator=(a);}
    void assign(ContourWithHoles &&a) {if (this!=&a) swap(a);}

    void clear() {outline.clear(); holes.clear();}

    void Invert() {outline.Invert(); if (holes.size()) holes.Invert();}
    void Shift(const XY &xy) {outline.Shift(xy); if (holes.size()) holes.Shift(xy);}
    void Scale(double sc)  {outline.Scale(sc); if (holes.size()) holes.Scale(sc);}
    void SwapXY() {outline.SwapXY(); if (holes.size()) holes.SwapXY();}
    void Rotate(double cos, double sin, double radian)
        {outline.Rotate(cos, sin, radian); if (holes.size()) holes.Rotate(cos, sin, radian);}
    void RotateAround(const XY&c, double cos, double sin, double radian)
        {outline.RotateAround(c, cos, sin, radian); if (holes.size()) holes.RotateAround(c, cos, sin, radian);}
    void Expand(EExpandType type4positive, EExpandType type4negative, double gap, Contour &res,
                double miter_limit_positive, double miter_limit_negative) const;
    void Expand2D(const XY &gap, Contour &res) const;
    SimpleContour::result_t RelationTo(const ContourWithHoles &c, bool ignore_holes) const;
    SimpleContour::result_t RelationTo(const ContourList &c, bool ignore_holes) const {return SimpleContour::switch_side(c.RelationTo(*this, ignore_holes));}

    void Distance(const ContourWithHoles &c, DistanceType &ret) const;

public:
    ContourWithHoles(const SimpleContour &p) : outline(p) {}
    ContourWithHoles(const ContourWithHoles &p) : outline(p.outline), holes(p.holes) {}  //this must be added by hand for g++
    ContourWithHoles(ContourWithHoles &&p) : outline(std::move(p.outline)), holes(std::move(p.holes)) {}  //this must be public for list<ContourWithHoles>
    void swap(ContourWithHoles &b) {holes.swap(b.holes); outline.swap(b.outline);}
    bool operator < (const ContourWithHoles &p) const {return outline==p.outline ? holes < p.holes : outline<p.outline;}
    bool operator ==(const ContourWithHoles &p) const {return outline==p.outline && holes == p.holes;}
    SimpleContour::size_type size() const {return outline.size();}
    const Edge &operator[](SimpleContour::size_type edge) const {return outline[edge];}
    ContourWithHoles &operator = (const Block &a) {outline = a; holes.clear(); return *this;}
    ContourWithHoles &operator = (const SimpleContour &a) {outline=a; holes.clear(); return *this;}
    ContourWithHoles &operator = (SimpleContour &&a) {outline.swap(a); holes.clear(); return *this;}
    ContourWithHoles &operator = (const ContourWithHoles &a) {if (this!=&a) outline = a.outline; holes=a.holes; return *this;}
    ContourWithHoles &operator = (ContourWithHoles &&a) {if (this!=&a) swap(a); return *this;}

    bool IsSane(bool shouldbehole=false) const;
    bool IsEmpty() const {return outline.IsEmpty();}

    is_within_t IsWithin(const XY &p) const;
    bool HasHoles() const {return !holes.IsEmpty();}
    const ContourList &Holes() const {return holes;}
    const Block &GetBoundingBox(void) const {return outline.GetBoundingBox();}
    bool GetClockWise() const {return outline.GetClockWise();}
    double GetArea(bool consider_holes=true) const {return consider_holes && !holes.IsEmpty() ? outline.GetArea() + holes.GetArea(true) : outline.GetArea();}
    double GetCircumference(bool consider_holes=true, bool include_hidden=false) const {return consider_holes ? outline.GetCircumference(include_hidden) + holes.GetCircumference(true, include_hidden) : outline.GetCircumference(include_hidden);}
    XY CentroidUpscaled() const {return holes.IsEmpty() ? outline.CentroidUpscaled() : outline.CentroidUpscaled() + holes.CentroidUpscaled();}

    //Call these for both us and the holes (except for OffsetBelow)
    void VerticalCrossSection(double x, DoubleMap<bool> &section) const {outline.VerticalCrossSection(x, section); if (holes.size()) holes.VerticalCrossSection(x, section);}
    double OffsetBelow(const SimpleContour &below, double &touchpoint, double offset=CONTOUR_INFINITY) const {return outline.OffsetBelow(below, touchpoint, offset);}

    void Path(cairo_t *cr, bool show_hidden) const {outline.Path(cr, show_hidden); if (holes.size()) holes.Path(cr, show_hidden);}
    void Path(cairo_t *cr, bool show_hidden, bool clockwiseonly) const {outline.Path(cr, show_hidden, clockwiseonly); if (holes.size()) holes.Path(cr, show_hidden, clockwiseonly);}
    void PathDashed(cairo_t *cr, const double pattern[], unsigned num, bool show_hidden) const {outline.PathDashed(cr, pattern, num, show_hidden); if (holes.size()) holes.PathDashed(cr, pattern, num, show_hidden);}
    void PathDashed(cairo_t *cr, const double pattern[], unsigned num, bool show_hidden, bool clockwiseonly) const {outline.PathDashed(cr, pattern, num, show_hidden, clockwiseonly); if (holes.size()) holes.PathDashed(cr, pattern, num, show_hidden, clockwiseonly);}

    Range Cut(const XY &A, const XY &B) const {return outline.Cut(A, B);}
    void Cut(const XY &A, const XY &B, DoubleMap<bool> &map) const {outline.Cut(A, B, map); holes.Cut(A, B, map);}
};

//This can contain multiple positive contours and holes
//holes are always within one of the contours
class Contour
{
    friend void contour_test(void);
    friend class SimpleContour;
    friend class ContourWithHoles;
    friend class ContourList;
    friend class ContoursHelper;
    template <typename LT> friend void Distance(const LT &list, const Contour &c, DistanceType &dist_so_far);
    typedef enum {POSITIVE_UNION=0, POSITIVE_INTERSECT, POSITIVE_XOR,
                  WINDING_RULE_NONZERO, WINDING_RULE_EVENODD,
                  EXPAND_POSITIVE,
                  NEGATIVE_UNION, NEGATIVE_INTERSECT, NEGATIVE_XOR,
                  EXPAND_NEGATIVE} operation_t;
    typedef SimpleContour::size_type size_type;
    static bool is_positive(operation_t t) {return t<=EXPAND_POSITIVE;}
    ContourWithHoles first;
    ContourList further;
    Block boundingBox;
protected:
    void append(const ContourWithHoles &p) {if (p.IsEmpty()) return; if (IsEmpty()) {boundingBox = p.GetBoundingBox(); first.assign(p);} else {boundingBox+=p.GetBoundingBox(); further.append(p);}}
    void append(ContourWithHoles &&p)  {if (p.IsEmpty()) return; if (IsEmpty()) {boundingBox = p.GetBoundingBox(); first.assign(std::move(p));} else {boundingBox+=p.GetBoundingBox(); further.append(std::move(p));}}

    void Invert();
    Contour CreateInverse() const {Contour tmp(*this); tmp.Invert(); return tmp;}
    void Rotate(double cos, double sin, double radian) {first.Rotate(cos, sin, radian); boundingBox = first.GetBoundingBox(); if (further.size()) {further.Rotate(cos, sin, radian); boundingBox += further.GetBoundingBox();}}
    void RotateAround(const XY&c, double cos, double sin, double radian) {first.RotateAround(c, cos, sin, radian); boundingBox = first.GetBoundingBox(); if (further.size()) {further.RotateAround(c, cos, sin, radian); boundingBox += further.GetBoundingBox();}}

    void Operation(operation_t type, const Contour &c1);
    void Operation(operation_t type, Contour &&c1);
    void Operation(operation_t type, const Contour &c1, const Contour &c2);
    void Operation(operation_t type, const Contour &c1, Contour &&c2);
    void Operation(operation_t type, Contour &&c1, Contour &&c2);

    void Expand(EExpandType type4positive, EExpandType type4negative, double gap, Contour &res,
                double miter_limit_positive, double miter_limit_negative) const;
    void Expand2D(const XY &gap, Contour &res) const;
    void Distance(const Contour &c, DistanceType &dist_so_far) const;
public:
    typedef SimpleContour::result_t relation_t;
    Contour() {boundingBox.MakeInvalid();}
    Contour(double sx, double dx, double sy, double dy) : first(sx, dx, sy, dy) {boundingBox = first.GetBoundingBox();}
    Contour(const Block &b) : first(b), boundingBox(b) {}
    Contour(XY a, XY b, XY c) : first(a,b,c) {boundingBox = first.GetBoundingBox();}
    Contour(double ax, double ay, double bx, double by, double cx, double cy) : first(ax, ay, bx, by, cx, cy) {boundingBox = first.GetBoundingBox();}
    Contour(const XY &c, double radius_x, double radius_y=0, double tilt_deg=0, double s_deg=0, double d_deg=360) :
        first (c, radius_x, radius_y, tilt_deg, s_deg, d_deg) {boundingBox = first.GetBoundingBox();}
    Contour(const std::vector<XY> &v, bool winding=true) {assign(v, winding);}
    Contour(const XY v[], size_type size, bool winding=true) {assign (v, size, winding);}
    template<size_type SIZE> Contour(const XY (&v)[SIZE], bool winding=true) {assign (v, SIZE, winding);}
    Contour(const std::vector<Edge> &v, bool winding=true) {assign (v, winding);}
    Contour(const Edge v[], size_type size, bool winding=true) {assign (v, size, winding);}
    template<size_type SIZE> Contour(const Edge (&v)[SIZE], bool winding=true) {assign (v, SIZE, winding);}

    Contour(const ContourWithHoles &p) : first(p) {boundingBox = first.GetBoundingBox();}
    Contour(const SimpleContour &p) : first(p) {boundingBox = first.GetBoundingBox();}
    Contour(ContourWithHoles &&p) : first(std::move(p)) {boundingBox = first.GetBoundingBox();}
    Contour(SimpleContour &&p) : first(std::move(p)) {boundingBox = first.GetBoundingBox();}
    Contour(Contour &&a) : first(std::move(a.first)), further(std::move(a.further)), boundingBox(a.boundingBox) {}
    Contour(const Contour &a) : first(a.first), further(a.further), boundingBox(a.boundingBox) {}

    Contour &operator = (const Block &a) {first = a; further.clear(); boundingBox = a; return *this;}
    Contour &operator = (const SimpleContour &a) {first.operator=(a); further.clear(); boundingBox = first.GetBoundingBox(); return *this;}
    Contour &operator = (SimpleContour &&a) {first.operator=(std::move(a)); boundingBox = first.GetBoundingBox(); return *this;}
    Contour &operator = (const ContourWithHoles &a) {first.operator=(a); further.clear(); boundingBox = first.GetBoundingBox(); return *this;}
    Contour &operator = (ContourWithHoles &&a) {first.operator=(std::move(a)); further.clear(); boundingBox = first.GetBoundingBox(); return *this;}
    Contour &operator = (Contour &&a) {first.swap(a.first); further.swap(a.further); boundingBox = a.boundingBox; return *this;}
    Contour &operator = (const Contour &a) {first = a.first; further = a.further; boundingBox = a.boundingBox; return *this;}

    bool operator < (const Contour &p) const {return first==p.first ? further < p.further: first < p.first;}
    bool operator ==(const Contour &p) const {return first==p.first && further == p.further;}

    void swap(Contour &a) {first.swap(a.first); further.swap(a.further); std::swap(boundingBox, a.boundingBox);}
    size_type size() const {if (first.outline.size()) return further.size()+1; return 0;}
    void clear() {first.clear(); further.clear(); boundingBox.MakeInvalid();}
    void assign(const std::vector<XY> &v, bool winding=true);
    void assign(const XY v[], size_type size, bool winding=true);
    template<size_type SIZE> void assign(const XY (&v)[SIZE], bool winding=true) {assign (v, SIZE, winding);}
    void assign(const std::vector<Edge> &v, bool winding=true);
    void assign(const Edge v[], size_type size, bool winding=true);
    template<size_type SIZE> void assign(const Edge (&v)[SIZE], bool winding=true) {assign (v, SIZE, winding);}

    void assign_dont_check(const std::vector<XY> &v) {clear(); first.outline.assign_dont_check(v); boundingBox = first.GetBoundingBox();}
    void assign_dont_check(const XY v[], size_type size)  {first.outline.assign_dont_check(v, size); boundingBox = first.GetBoundingBox();}
    template<size_type SIZE> void assign_dont_check(const XY (&v)[SIZE]) {assign_dont_check (v, SIZE);}
    void assign_dont_check(const std::vector<Edge> &v)  {first.outline.assign_dont_check(v); boundingBox = first.GetBoundingBox();}
    void assign_dont_check(const Edge v[], size_type size)  {first.outline.assign_dont_check(v, size); boundingBox = first.GetBoundingBox();}
    template<size_type SIZE> void assign_dont_check(const Edge (&v)[SIZE]) {assign_dont_check (v, SIZE);}

    void append_dont_check(const std::vector<XY> &v) {ContourWithHoles tmp; tmp.outline.assign_dont_check(v); if (!tmp.IsEmpty()) append(std::move(tmp));}
    void append_dont_check(const XY v[], size_type size) {ContourWithHoles tmp; tmp.outline.assign_dont_check(v, size); if (!tmp.IsEmpty()) append(std::move(tmp));}
    template<size_type SIZE> void append_dont_check(const XY (&v)[SIZE]) {append_dont_check (v, SIZE);}
    void append_dont_check(const std::vector<Edge> &v) {ContourWithHoles tmp; tmp.outline.assign_dont_check(v); if (!tmp.IsEmpty()) append(std::move(tmp));}
    void append_dont_check(const Edge v[], size_type size) {ContourWithHoles tmp; tmp.outline.assign_dont_check(v, size); if (!tmp.IsEmpty()) append(std::move(tmp));}
    template<size_type SIZE> void append_dont_check(const Edge (&v)[SIZE]) {append_dont_check (v, SIZE);}

    bool IsEmpty() const {return first.IsEmpty();}
    const Block &GetBoundingBox(void) const {return boundingBox;}
    bool GetClockWise() const {return first.GetClockWise();}
    double GetArea(bool consider_holes=true) const {return further.size() ? further.GetArea(consider_holes) + first.GetArea(consider_holes) : first.GetArea(consider_holes);}
    double GetCircumference(bool consider_holes=true, bool include_hidden=false) const {return further.size() ? further.GetCircumference(consider_holes, include_hidden) + first.GetCircumference(consider_holes, include_hidden) : first.GetCircumference(consider_holes, include_hidden);}
    XY Centroid() const {return (further.IsEmpty() ? first.CentroidUpscaled() : first.CentroidUpscaled()+further.CentroidUpscaled())/GetArea();}
    //bool AddAnEdge(const Edge &edge) {SimpleContour::AddAnEdge(edge);}
    //bool AddPoint(const XY &xy) {return AddAnEdge(Edge(xy, at(0).GetStart()));}
    const ContourWithHoles &operator[](size_type n) const {return n==0 ? first : further[n-1];}
    void ClearHoles() {first.holes.clear(); for (auto i = further.begin(); i!=further.end(); i++) i->holes.clear();}
    bool IsSane() const;

    is_within_t IsWithin(const XY &p) const {is_within_t ret = first.IsWithin(p); if (ret==WI_OUTSIDE) ret = further.IsWithin(p); return ret;}
    Contour &Shift(const XY &xy) {first.Shift(xy); if (further.size()) further.Shift(xy); boundingBox.Shift(xy); return *this;}
    Contour &Scale(double sc) {first.Scale(sc); if (further.size()) further.Scale(sc); boundingBox.Scale(sc); return *this;}
    Contour &SwapXY() {first.SwapXY(); if (further.size()) further.SwapXY(); boundingBox.SwapXY(); return *this;}
    Contour &Rotate(double degrees) {if (degrees) {double r=deg2rad(degrees); Rotate(cos(r), sin(r), r);} return *this;}
    Contour &RotateAround(const XY&c, double degrees) {if (degrees) {double r=deg2rad(degrees); RotateAround(c, cos(r), sin(r), r);} return *this;}

    Contour CreateShifted(const XY & xy) const {Contour a(*this); a.Shift(xy); return a;}
    Contour CreateScaled(double sc) const {Contour a(*this); a.Scale(sc); return a;}
    Contour CreateSwapXYd() const {Contour a(*this); a.SwapXY(); return a;}
    Contour CreateRotated(double degrees) const {Contour a(*this); a.Rotate(degrees); return a;}
    Contour CreateRotatedAround(const XY&c, double degrees) const {Contour a(*this); a.RotateAround(c, degrees); return a;}

    Contour &operator += (const Contour &a) {Operation(GetClockWise() || a.GetClockWise() ? POSITIVE_UNION : NEGATIVE_UNION, *this, a); return *this;}
    Contour &operator += (Contour &&a)      {Operation(GetClockWise() || a.GetClockWise() ? POSITIVE_UNION : NEGATIVE_UNION, *this, std::move(a)); return *this;}
    Contour &operator *= (const Contour &a) {Operation(GetClockWise() || a.GetClockWise() ? POSITIVE_INTERSECT : NEGATIVE_INTERSECT, *this, a); return *this;}
    Contour &operator *= (Contour &&a)      {Operation(GetClockWise() || a.GetClockWise() ? POSITIVE_INTERSECT : NEGATIVE_INTERSECT, *this, std::move(a)); return *this;}
    Contour &operator -= (const Contour &a) {Operation(GetClockWise() || a.GetClockWise() ? POSITIVE_UNION : NEGATIVE_UNION, *this, a.CreateInverse()); return *this;}
    Contour &operator -= (Contour &&a)      {a.Invert(); Operation(GetClockWise() || a.GetClockWise() ? POSITIVE_UNION : NEGATIVE_UNION, *this, std::move(a)); return *this;}
    Contour &operator ^= (const Contour &a) {Operation(GetClockWise() || a.GetClockWise() ? POSITIVE_XOR : NEGATIVE_XOR, *this, a); return *this;}
    Contour &operator ^= (Contour &&a)      {Operation(GetClockWise() || a.GetClockWise() ? POSITIVE_XOR : NEGATIVE_XOR, *this, std::move(a)); return *this;}

    Contour operator + (const Contour &p) const {return Contour(*this)+=p;}
    Contour operator * (const Contour &p) const {return Contour(*this)*=p;}
    Contour operator - (const Contour &p) const {return Contour(*this)-=p;}
    Contour operator ^ (const Contour &p) const {return Contour(*this)^=p;}
    Contour operator + (Contour &&p) const {return Contour(*this)+=std::move(p);}
    Contour operator * (Contour &&p) const {return Contour(*this)*=std::move(p);}
    Contour operator - (Contour &&p) const {return Contour(*this)-=std::move(p);}
    Contour operator ^ (Contour &&p) const {return Contour(*this)^=std::move(p);}

    void VerticalCrossSection(double x, DoubleMap<bool> &section) const {first.VerticalCrossSection(x, section); if (further.size()) further.VerticalCrossSection(x, section);}
    double OffsetBelow(const SimpleContour &below, double &touchpoint, double offset=CONTOUR_INFINITY) const;
    double OffsetBelow(const Contour &below, double &touchpoint, double offset=CONTOUR_INFINITY) const;
    Contour& Expand(double gap, EExpandType et4pos=EXPAND_MITER_ROUND, EExpandType et4neg=EXPAND_MITER_ROUND,
                    double miter_limit_positive=CONTOUR_INFINITY, double miter_limit_negative=CONTOUR_INFINITY);
    Contour CreateExpand(double gap, EExpandType et4pos=EXPAND_MITER_ROUND, EExpandType et4neg=EXPAND_MITER_ROUND,
                         double miter_limit_positive=CONTOUR_INFINITY, double miter_limit_negative=CONTOUR_INFINITY) const;
    Contour& Expand2D(const XY &gap);
    Contour CreateExpand2D(const XY &gap) const;

    relation_t RelationTo(const Contour &c, bool ignore_holes) const;
    static bool Overlaps(relation_t t) {return SimpleContour::result_overlap(t);}
    bool Overlaps(const Contour &c, bool ignore_holes) const {return Overlaps(RelationTo(c, ignore_holes));}
    bool Overlaps(const Contour &c, double gap) const; //not implemented on purpose. Declaration here to prevent unintended use

    void Path(cairo_t *cr, bool show_hidden) const {first.Path(cr, show_hidden); if (further.size()) further.Path(cr, show_hidden);}
    void Path(cairo_t *cr, bool show_hidden, bool clockwiseonly) const {first.Path(cr, show_hidden, clockwiseonly); if (further.size()) further.Path(cr, show_hidden, clockwiseonly);}
    void PathDashed(cairo_t *cr, const double pattern[], unsigned num, bool show_hidden) const {first.PathDashed(cr, pattern, num, show_hidden); if (further.size()) further.PathDashed(cr, pattern, num, show_hidden);}
    void PathDashed(cairo_t *cr, const double pattern[], unsigned num, bool show_hidden, bool clockwiseonly) const {first.PathDashed(cr, pattern, num, show_hidden, clockwiseonly); if (further.size()) further.PathDashed(cr, pattern, num, show_hidden, clockwiseonly);}
    void Line(cairo_t *cr) const {Contour::Path(cr, false); cairo_stroke(cr);}
    void Line2(cairo_t *cr) const {cairo_save(cr); double dash[]={2,2}; cairo_set_dash(cr, dash, 2, 0); Path(cr, false); cairo_stroke(cr); cairo_set_dash(cr, NULL, 0, 0); Path(cr, true); cairo_stroke(cr); cairo_restore(cr);}
    void Fill(cairo_t *cr) const {Contour::Path(cr, true); cairo_fill(cr);}

    //Returns distance and two points on the two contours.
    //may return invalid distance if one or the other contour is empty
    //negative distance is returned if one is inside another (but positive if in a hole)
    //zero returned if they cross. If the two points are equal that is a crosspoint, if not they are invalid
    //(Latter may happen if one of the contours have two pieces and one is inside and one is outside the other
    DistanceType Distance(const Contour &c) const {DistanceType r; Distance(c, r); return r;}
    Range Cut(const XY &A, const XY &B) const {Range ret = first.Cut(A, B); if (!further.IsEmpty()) ret += further.Cut(A, B); return ret;}
    void Cut(const XY &A, const XY &B, DoubleMap<bool> &map) const {first.Cut(A, B, map); if (further.IsEmpty()) further.Cut(A, B, map); map.Prune();}
    bool TangentFrom(const XY &from, XY &clockwise, XY &cclockwise) const;
};

template <typename LT> void Distance(const LT &list, const Contour &c, DistanceType &dist_so_far)
{
    if (list.size()==0 || c.IsEmpty()) return;
    for (auto i = list.begin(); i!=list.end(); i++) {
        const double bbdist = (*i)->GetBoundingBox().Distance(c.GetBoundingBox());
        if (dist_so_far.ConsiderBB(bbdist))
            (*i)->Distance(c, dist_so_far);
        else
            dist_so_far.MergeInOut(bbdist);
        if (dist_so_far.IsZero())
            break;
    }
}



inline bool ContourList::GetClockWise() const
{
    return size() ? begin()->GetClockWise() : true;
}

inline double ContourList::GetArea(bool consider_holes) const
{
    register double ret = 0;
    for (auto i = begin(); i!=end(); i++)
        ret += i->GetArea(consider_holes);
    return ret;
}

inline double ContourList::GetCircumference(bool consider_holes, bool include_hidden) const
{
    register double ret = 0;
    for (auto i = begin(); i!=end(); i++)
        ret += i->GetCircumference(consider_holes, include_hidden);
    return ret;
}

inline XY ContourList::CentroidUpscaled() const
{
    XY ret(0,0);
    for (auto i = begin(); i!=end(); i++)
        ret += i->CentroidUpscaled();
    return ret;
}


inline void ContourList::append(const ContourWithHoles &p)
{
    if (p.IsEmpty()) return;
    _ASSERT(size()==0 || GetClockWise() == p.GetClockWise());
    boundingBox += p.GetBoundingBox();
    std::list<ContourWithHoles>::push_back(p);
}

inline void ContourList::append(ContourWithHoles &&p)
{
    if (p.IsEmpty()) return;
    _ASSERT(size()==0 || GetClockWise() == p.GetClockWise());
    boundingBox += p.GetBoundingBox();
    std::list<ContourWithHoles>::push_back(std::move(p));
}


inline const ContourWithHoles & ContourList::operator[](size_type i) const
{
    const_iterator itr = begin();
    while (i>0) i--, itr++;
    return *itr;
}


inline is_within_t ContourList::IsWithin(XY p) const
{
    is_within_t ret = boundingBox.IsWithin(p);
    if (ret==WI_OUTSIDE) return WI_OUTSIDE;
    for (auto i = begin(); i!=end(); i++)
        if (WI_OUTSIDE != (ret=i->IsWithin(p))) return ret; //we also return IN_HOLE
    return WI_OUTSIDE;
}

inline void ContourList::VerticalCrossSection(double x, DoubleMap<bool> &section) const
{
    for (auto i=begin(); i!=end(); i++)
        i->VerticalCrossSection(x, section);
}


inline void ContourList::Shift(const XY &xy)
{
    for (auto i = begin(); i!=end(); i++)
        i->Shift(xy);
    boundingBox.Shift(xy);
}

inline void ContourList::Scale(double sc)
{
    for (auto i = begin(); i!=end(); i++)
        i->Scale(sc);
    boundingBox.Scale(sc);
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

inline double ContourList::OffsetBelow(const Contour &below, double &touchpoint, double offset) const
{
    if (below.further.size())
        offset = OffsetBelow(below.further, touchpoint, offset);
    return OffsetBelow(below.first.outline, touchpoint, offset);
}

inline Range ContourList::Cut(const XY &A, const XY &B) const
{
    Range ret;
    ret.MakeInvalid();
    for (auto i = begin(); i!=end(); i++)
        ret += i->Cut(A, B);
    return ret;
}

inline void ContourList::Cut(const XY &A, const XY &B, DoubleMap<bool> &map) const
{
    for (auto i = begin(); i!=end(); i++)
        i->Cut(A, B, map);
}


//returns true only if all is true
inline bool ContourList::TangentFrom(const XY &from, XY &clockwise, XY &cclockwise) const
{
    if (size()==0) return false;
    if (!begin()->outline.TangentFrom(from, clockwise, cclockwise))
        return false;
    XY c, cc;
    for (auto i=++begin(); i!=end(); i++) {
        if (!i->outline.TangentFrom(from, c, cc)) return false;
        clockwise =  minmax_clockwise(from, clockwise,  c,  true);
        cclockwise = minmax_clockwise(from, cclockwise, cc, false);
    }
    return true;
}
///////Contour///////////////////////////////////////

inline is_within_t ContourWithHoles::IsWithin(const XY &p) const
{
    is_within_t ret = outline.IsWithin(p);
    if (ret!=WI_INSIDE || holes.size()==0) return ret;
    ret = holes.IsWithin(p);
    switch (ret) {
    case WI_INSIDE: return WI_IN_HOLE;
    case WI_IN_HOLE:
    case WI_OUTSIDE: return WI_INSIDE;
    default: return ret;
    }
}

inline double Contour::OffsetBelow(const SimpleContour &below, double &touchpoint, double offset) const
{
    if (further.size())
        offset = further.OffsetBelow(below, touchpoint, offset);
    return first.outline.OffsetBelow(below, touchpoint, offset);
}

inline double Contour::OffsetBelow(const Contour &below, double &touchpoint, double offset) const
{
    if (further.size())
        offset = further.OffsetBelow(below, touchpoint, offset);
    if (below.further.size())
        for (auto i = below.further.begin(); i!=below.further.end(); i++)
            offset = OffsetBelow(*i, touchpoint, offset);
    return first.outline.OffsetBelow(below.first.outline, touchpoint, offset);
}

inline Contour &Contour::Expand(double gap, EExpandType et4pos, EExpandType et4neg,
                                double miter_limit_positive, double miter_limit_negative)
{
    if (!gap) return *this;
    Contour res;
    Expand(et4pos, et4neg, gap, res, miter_limit_positive, miter_limit_negative);
    swap(res);
    return *this;
}

inline Contour Contour::CreateExpand(double gap, EExpandType et4pos, EExpandType et4neg,
                                     double miter_limit_positive, double miter_limit_negative) const
{
    if (!gap) return *this;
    Contour res;
    Expand(et4pos, et4neg, gap, res, miter_limit_positive, miter_limit_negative);
    return res;
}

inline Contour& Contour::Expand2D(const XY &gap)
{
    if (test_zero(gap.x) && test_zero(gap.y)) return *this;
    Contour res;
    Expand2D(gap, res);
    swap(res);
}

inline Contour Contour::CreateExpand2D(const XY &gap) const
{
    if (test_zero(gap.x) && test_zero(gap.y)) return *this;
    Contour res;
    Expand2D(gap, res);
    return res;
}


inline void Contour::Distance(const Contour &c, DistanceType &dist_so_far) const
{
    if (IsEmpty() || c.IsEmpty() || dist_so_far.IsZero()) return;
    first.Distance(c.first, dist_so_far);
    if (dist_so_far.IsZero()) return;
    if (!c.further.IsEmpty()) {
        dist_so_far.SwapPoints();
        c.further.Distance(first, dist_so_far);
        if (dist_so_far.IsZero()) return;
        dist_so_far.SwapPoints();
    }
    if (!further.IsEmpty()) {
        further.Distance(c.first, dist_so_far);
        if (dist_so_far.IsZero()) return;
    }
    if (!c.further.IsEmpty() && !further.IsEmpty())
        further.Distance(c.further, dist_so_far);
}

inline bool Contour::TangentFrom(const XY &from, XY &clockwise, XY &cclockwise) const
{
    if (first.outline.TangentFrom(from, clockwise, cclockwise)) {
        XY c, cc;
        if (further.TangentFrom(from, c, cc)) {
            clockwise =  minmax_clockwise(from, clockwise,  c,  true);
            cclockwise = minmax_clockwise(from, cclockwise, cc, false);
        }
        return true;
    }
    return false;
}


} //namespace


#endif //CONTOUR_CONTOURS_H
