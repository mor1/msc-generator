#if !defined(CONTOUR_CONTOURS_H)
#define CONTOUR_CONTOURS_H

#include <list>
#include "contour_contour.h"

class ContourWithHoles;

//This can contain multiple positive contours and holes
//holes are always within one of the contours
class Contours 
{
    friend class ContourWithHoles;
    friend class Contour;
    friend class ContourHelper;
    std::list<ContourWithHoles> content;
    Block boundingBox;

protected:
    void append(ContourWithHoles &&p);
    void append(const ContourWithHoles &p);

    void Expand(EExpandType et, double gap, Contours &result) const;
    void DoVerticalCrossSection(double x, DoubleMap<bool> &section, bool add) const;
public:
    Contours();
    Contours(const ContourWithHoles &p);
    Contours(const Contour &p);
    Contours(ContourWithHoles &&p);
    Contours(Contour &&p);
    explicit Contours(const std::vector<XY> &v) {assign(v);}
    Contours(Contours &&p) {swap(p);}

    void append(Contours &&p) {boundingBox += p.GetBoundingBox(); content.splice(content.end(), p.content);}
    void append(const Contours &p) {boundingBox += p.GetBoundingBox(); content.insert(content.end(), p.content.begin(), p.content.end());}
    void swap(Contours &a) {content.swap(a.content); std::swap(boundingBox, a.boundingBox);}
    void clear() {content.clear(); boundingBox.MakeInvalid();}
    void assign(const std::vector<XY> &v, bool winding=true);
    bool operator <(const Contours &b) const;
    bool operator ==(const Contours &b) const;
    Contours &operator =(const std::vector<XY> &v) {assign(v); return *this;}
    Contours &operator =(Contours &&a) {swap(a); return *this;}
    Contours &operator =(const Contour &a); 
    Contours &operator =(Contour &&a);

    bool IsEmpty() const {return content.size()==0;}
    const Block &GetBoundingBox(void) const {return boundingBox;}
    Contour GetFirst() const;
    const std::list<ContourWithHoles> &GetContent() const {return content;}
    is_within_t IsWithin(XY p) const;
    void Shift(XY xy);
    Contours CreateShifted(const XY & xy) const {Contours a(*this); a.Shift(xy); return std::move(a);}
    void Rotate(double cos, double sin, double radian);
    void RotateAround(const XY&c, double cos, double sin, double radian);
	void Rotate(double degrees) {double r=deg2rad(degrees); Rotate(cos(r), sin(r), r);}
    void RotateAround(const XY&c, double degrees) {double r=deg2rad(degrees); RotateAround(c, cos(r), sin(r), r);}
    void SwapXY();

    Contours &operator += (const Contour &p);
    Contours &operator *= (const Contour &p);
    Contours &operator -= (const Contour &p);
    Contours &operator ^= (const Contour &p);
    Contours &operator += (const ContourWithHoles &p);
    Contours &operator *= (const ContourWithHoles &p);
    Contours &operator -= (const ContourWithHoles &p);
    Contours &operator ^= (const ContourWithHoles &p);
    Contours &operator += (const Contours &a);
    Contours &operator *= (const Contours &a);
    Contours &operator -= (const Contours &a);
    Contours &operator ^= (const Contours &a);

    double OffsetBelow(const Contours &below, double &touchpoint, double offset=CONTOUR_INFINITY) const;
	void ClearHoles();
    Contours CreateExpand(double gap, EExpandType et=EXPAND_MITER) const;

    void Path(cairo_t *cr, bool inv=false) const;
    void Path(cairo_t *cr, bool inv, bool do_this) const;
};

//this contains a list of non-overlapping contours as holes
class ContourWithHoles : public Contour
{
    friend class Contour;
    friend class Contours;
	//hide these - do not implement the one with degrees
    void Rotate(double /*degrees*/) {}
    void RotateAround(const XY& /*c*/, double /*degrees*/) {}
protected:
    friend class ContourHelper;
    Contours holes;
    ContourWithHoles() {};
public:
    ContourWithHoles(const Contour &p) : Contour(p) {}
    ContourWithHoles(ContourWithHoles &&p) {swap(p);}
	void ClearHoles() {holes.clear();}
    bool HasHoles() const {return !holes.IsEmpty();}
    const Contours &GetHoles() const {return holes;}
protected:
    void swap(ContourWithHoles &b) {holes.swap(b.holes); Contour::swap(b);}
    bool operator < (const ContourWithHoles &p) const;
    bool operator ==(const ContourWithHoles &p) const;
    ContourWithHoles &operator = (ContourWithHoles &&a) {swap(a); return *this;}
    const Block &GetBoundingBox(void) const {return Contour::GetBoundingBox();}
    is_within_t IsWithin(XY p) const;
    void Shift(XY xy) {Contour::Shift(xy); holes.Shift(xy);}
    ContourWithHoles CreateShifted(const XY & xy) const {ContourWithHoles a(*this); a.Shift(xy); return std::move(a);}
    void Rotate(double cos, double sin, double radian)
        {Contour::Rotate(cos, sin, radian); if (!holes.IsEmpty()) holes.Rotate(cos, sin, radian);}
    void RotateAround(const XY&c, double cos, double sin, double radian)
        {Contour::RotateAround(c, cos, sin, radian); if (!holes.IsEmpty()) holes.RotateAround(c, cos, sin, radian);}
    void SwapXY() {Contour::SwapXY(); holes.SwapXY();}

    Contour::result_t Add(const ContourWithHoles &p, Contours &res) const;
    Contour::result_t Mul(const ContourWithHoles &p, Contours &res) const;
    Contour::result_t Sub(const ContourWithHoles &p, Contours &res) const;
    Contour::result_t Xor(const ContourWithHoles &p, Contours &res) const;

	void Expand(EExpandType et, double gap, Contours &res) const;

    void DoVerticalCrossSection(double x, DoubleMap<bool> &section, bool add) const {Contour::DoVerticalCrossSection(x, section, add); holes.DoVerticalCrossSection(x, section, !add);}
    void Path(cairo_t *cr, bool inv) const
        {Contour::Path(cr, inv); if (!holes.IsEmpty()) holes.Path(cr, !inv);}
    void Path(cairo_t *cr, bool inv, bool do_this) const
        {if (do_this) Contour::Path(cr, inv); if (!holes.IsEmpty()) holes.Path(cr, !inv, !do_this);}
};


//these must be here due to Contours forward referencing ContourWithHoles
inline Contours::Contours() {boundingBox.MakeInvalid();}
inline Contours::Contours(const ContourWithHoles &p) : content(1, p), boundingBox(p.GetBoundingBox()) {}
inline Contours::Contours(const Contour &p) : content(1, p), boundingBox(p.GetBoundingBox()) {}
inline Contours::Contours(ContourWithHoles &&p) : content(1, p), boundingBox(p.GetBoundingBox()) {}
inline Contours::Contours(Contour &&p) : content(1, p), boundingBox(p.GetBoundingBox()) {}
inline void Contours::append(ContourWithHoles &&p) {boundingBox += p.GetBoundingBox(); content.push_back(p);}
inline void Contours::append(const ContourWithHoles &p) {boundingBox += p.GetBoundingBox(); content.push_back(p);}
inline Contour Contours::GetFirst() const {return IsEmpty()?Contour():*content.begin();}
inline Contours &Contours::operator =(const Contour &a) {clear(); content.push_back(ContourWithHoles(a)); return *this;}
inline Contours &Contours::operator =(Contour &&a) {clear(); content.push_back(ContourWithHoles(std::move(a))); return *this;}

inline Contours &Contours::operator += (const Contour &p) {return *this+=ContourWithHoles(p);}
inline Contours &Contours::operator *= (const Contour &p) {return *this*=ContourWithHoles(p);}
inline Contours &Contours::operator -= (const Contour &p) {return *this-=ContourWithHoles(p);}
inline Contours &Contours::operator ^= (const Contour &p) {return *this^=ContourWithHoles(p);}

inline Contours &Contours::operator += (const Contours &a)
{
    if (boundingBox.Overlaps(a.GetBoundingBox()))
        for (auto i = a.content.begin(); i!=a.content.end(); i++)
            operator += (*i);
    else
        append(a);
    return *this;
}

inline Contours &Contours::operator *= (const Contours &a)
{
    if (boundingBox.Overlaps(a.boundingBox)) {
        Contours result;
        for (auto i = a.content.begin(); i!=a.content.end(); i++) {
            Contours tmp(*this);
            tmp *= *i;
            result.append(tmp);
        }
        swap(result);
    } else
        clear();
    return *this;
}

inline Contours &Contours::operator -= (const Contours &a)
{
    if (boundingBox.Overlaps(a.boundingBox))
        for (auto i = a.content.begin(); i!=a.content.end(); i++)
            operator -= (*i);
    return *this;
}

inline Contours &Contours::operator ^= (const Contours &a)
{
    if (boundingBox.Overlaps(a.boundingBox))
        for (auto i = a.content.begin(); i!=a.content.end(); i++)
            operator ^= (*i);
    return *this;
}

inline bool Contours::operator <(const Contours &b) const
{
    if (boundingBox != b.boundingBox) return boundingBox<b.boundingBox;
    if (content.size() != b.content.size()) return content.size()<b.content.size();
    for (auto i = content.begin(), j=b.content.begin(); i!=content.end(); i++, j++)
        if (!(*i==*j)) return *i<*j;
    return false;
}

inline bool Contours::operator ==(const Contours &b) const
{
    if (boundingBox != b.boundingBox || content.size() != b.content.size()) return false;
    for (auto i = content.begin(), j=b.content.begin(); i!=content.end(); i++, j++)
        if (!(*i==*j)) return false;
    return true;
}

inline is_within_t Contours::IsWithin(XY p) const
{
    is_within_t ret = boundingBox.IsWithin(p);
    if (ret==WI_OUTSIDE) return WI_OUTSIDE;
    for (auto i = content.begin(); i!=content.end(); i++)
        if (WI_OUTSIDE != (ret=i->IsWithin(p))) return ret;
    return WI_OUTSIDE;
}

inline void Contours::Shift(XY xy)
{
    for (auto i = content.begin(); i!=content.end(); i++)
        i->Shift(xy);
    boundingBox.Shift(xy);
}


inline void Contours::Rotate(double cos, double sin, double radian)
{
    boundingBox.MakeInvalid();
    for (auto i = content.begin(); i!=content.end(); i++) {
        i->Rotate(cos, sin, radian);
        boundingBox += i->GetBoundingBox();
    }
}

inline void Contours::RotateAround(const XY&c, double cos, double sin, double radian)
{
    boundingBox.MakeInvalid();
    for (auto i = content.begin(); i!=content.end(); i++) {
        i->RotateAround(c, cos, sin, radian);
        boundingBox += i->GetBoundingBox();
    }
}

inline double Contours::OffsetBelow(const Contours &below, double &touchpoint, double offset) const
{
    if (offset < below.GetBoundingBox().y.from - GetBoundingBox().y.till) return offset;
    if (!GetBoundingBox().x.Overlaps(below.GetBoundingBox().x)) return offset;
    for (auto i = content.begin(); i!=content.end(); i++)
        for (auto j = below.content.begin(); j!=below.content.end(); j++) {
            double off, tp;
            off = i->OffsetBelow(*j, tp, offset);
            if (off < offset) {
                offset = off;
                touchpoint = tp;
            }
        }
    return offset;
}



inline void Contours::ClearHoles()
{
	for (auto i=content.begin(); i!=content.end(); i++)
		i->ClearHoles();
}

inline Contours Contours::CreateExpand(double gap, EExpandType et) const
{
    Contours result;
    if (gap == 0) return (result = *this);  //always return result->compiler optimizes
    Contours::Expand(et, gap, result);
    return result;
}



inline void Contours::DoVerticalCrossSection(double x, DoubleMap<bool> &section, bool add) const
{
    for (auto i=content.begin(); i!=content.end(); i++)
        i->DoVerticalCrossSection(x, section, add);
}


inline bool ContourWithHoles::operator <(const ContourWithHoles &p) const
{
    if (!(Contour::operator==(p))) return Contour::operator<(p);
    if (holes.content.size() != p.holes.content.size()) return holes.content.size()<p.holes.content.size();
    for (auto i = holes.content.begin(), j=p.holes.content.begin(); i!=holes.content.end(); i++, j++)
        if (!(*i==*j)) return *i<*j;
    return false;
}

inline bool ContourWithHoles::operator ==(const ContourWithHoles &p) const
{
    if (holes.content.size() != p.holes.content.size() || !(Contour::operator==(p))) return false;
    for (auto i = holes.content.begin(), j=p.holes.content.begin(); i!=holes.content.end(); i++, j++)
        if (!(*i==*j)) return false;
    return true;
}

inline is_within_t ContourWithHoles::IsWithin(XY p) const
{
    is_within_t ret = Contour::IsWithin(p);
    if (ret!=WI_INSIDE) return ret;
    for (auto i = holes.content.begin(); i!=holes.content.end(); i++) {
        ret = i->IsWithin(p);
        if (ret!=WI_OUTSIDE)
            return ret == WI_INSIDE ? WI_OUTSIDE : ret;
    }
    return WI_INSIDE;
}

inline void Contours::Path(cairo_t *cr, bool inv, bool do_this) const
{
    for(auto i = content.begin(); i!=content.end(); i++)
        i->Path(cr, inv, do_this);
}

inline void Contours::Path(cairo_t *cr, bool inv) const
{
    for(auto i = content.begin(); i!=content.end(); i++)
        i->Path(cr, inv);
}

inline Contours operator + (const Contour &p1, const Contour &p2) {return Contours(p1)+=p2;}
inline Contours operator * (const Contour &p1, const Contour &p2) {return Contours(p1)*=p2;}
inline Contours operator - (const Contour &p1, const Contour &p2) {return Contours(p1)-=p2;}
inline Contours operator ^ (const Contour &p1, const Contour &p2) {return Contours(p1)^=p2;}
inline Contours operator + (Contour &&p1, const Contour &p2) {return Contours(std::move(p1))+=p2;}
inline Contours operator * (Contour &&p1, const Contour &p2) {return Contours(std::move(p1))*=p2;}
inline Contours operator - (Contour &&p1, const Contour &p2) {return Contours(std::move(p1))-=p2;}
inline Contours operator ^ (Contour &&p1, const Contour &p2) {return Contours(std::move(p1))^=p2;}
inline Contours operator + (const Contour &p1, Contour &&p2) {return Contours(p1)+=std::move(p2);}
inline Contours operator * (const Contour &p1, Contour &&p2) {return Contours(p1)*=std::move(p2);}
inline Contours operator - (const Contour &p1, Contour &&p2) {return Contours(p1)-=std::move(p2);}
inline Contours operator ^ (const Contour &p1, Contour &&p2) {return Contours(p1)^=std::move(p2);}
inline Contours operator + (Contour &&p1, Contour &&p2) {return Contours(std::move(p1))+=std::move(p2);}
inline Contours operator * (Contour &&p1, Contour &&p2) {return Contours(std::move(p1))*=std::move(p2);}
inline Contours operator - (Contour &&p1, Contour &&p2) {return Contours(std::move(p1))-=std::move(p2);}
inline Contours operator ^ (Contour &&p1, Contour &&p2) {return Contours(std::move(p1))^=std::move(p2);}


inline Contours operator + (const Contours &p1, const Contour &p2) {return Contours(p1)+=p2;}
inline Contours operator * (const Contours &p1, const Contour &p2) {return Contours(p1)*=p2;}
inline Contours operator - (const Contours &p1, const Contour &p2) {return Contours(p1)-=p2;}
inline Contours operator ^ (const Contours &p1, const Contour &p2) {return Contours(p1)^=p2;}
inline Contours operator + (const Contours &p1, const ContourWithHoles &p2) {return Contours(p1)+=p2;}
inline Contours operator * (const Contours &p1, const ContourWithHoles &p2) {return Contours(p1)*=p2;}
inline Contours operator - (const Contours &p1, const ContourWithHoles &p2) {return Contours(p1)-=p2;}
inline Contours operator ^ (const Contours &p1, const ContourWithHoles &p2) {return Contours(p1)^=p2;}
inline Contours operator + (Contours &&p1, const ContourWithHoles &p2) {return p1+=p2;}
inline Contours operator * (Contours &&p1, const ContourWithHoles &p2) {return p1*=p2;}
inline Contours operator - (Contours &&p1, const ContourWithHoles &p2) {return p1-=p2;}
inline Contours operator ^ (Contours &&p1, const ContourWithHoles &p2) {return p1^=p2;}

inline Contours operator + (const Contours &p1, const Contours &p2) {return Contours(p1)+=p2;}
inline Contours operator * (const Contours &p1, const Contours &p2) {return Contours(p1)*=p2;}
inline Contours operator - (const Contours &p1, const Contours &p2) {return Contours(p1)-=p2;}
inline Contours operator ^ (const Contours &p1, const Contours &p2) {return Contours(p1)^=p2;}
inline Contours operator + (Contours &&p1, const Contours &p2) {return p1+=p2;}
inline Contours operator * (Contours &&p1, const Contours &p2) {return p1*=p2;}
inline Contours operator - (Contours &&p1, const Contours &p2) {return p1-=p2;}
inline Contours operator ^ (Contours &&p1, const Contours &p2) {return p1^=p2;}
inline Contours operator + (const Contours &p1, Contours &&p2) {return p2+=p1;}
inline Contours operator * (const Contours &p1, Contours &&p2) {return p2*=p1;}
//inline Contours operator - (const Contours &p1, Contours &&p2) {return p2-=p1;} NONE!
inline Contours operator ^ (const Contours &p1, Contours &&p2) {return p2^=p1;}

#endif //CONTOUR_CONTOURS_H
