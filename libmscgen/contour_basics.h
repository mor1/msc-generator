#if !defined(CONTOUR_BASICS_H)
#define CONTOUR_BASICS_H

#include <cfloat>
#include <utility>
#include <cassert>
#include <cstddef>
#include <limits>

using namespace std::rel_ops;  //so that we have != and <= and >= etc from only == and <

#ifndef _ASSERT
#define  _ASSERT(A)
#endif



namespace contour {

#define MaxVal(real) (std::numeric_limits<decltype(real)>::max())
#define CONTOUR_INFINITY (std::numeric_limits<double>::max())

//other helpers
static const double SMALL_NUM = 1e-10; //avoid division overflow
inline bool test_zero(double n) {return fabs(n)<SMALL_NUM;}
inline bool test_equal(double n, double m) {return test_zero(n-m);}
inline bool test_smaller(double n, double m) {return n<m-SMALL_NUM;} //true if *really* smaller, not just by epsilon
inline bool test_positive(double n) {return n >= SMALL_NUM;}

template <typename real> real minabs(real a, real b) {return fabs(a)<fabs(b) ? a : b;}
template <typename real> int fsign(real a) {return a>0 ? +1 : a<0 ? -1 : 0;}

class XY {
public:
    double x;
    double y;
    XY() {}
    XY(double a, double b) : x(a), y(b) {}
    XY &   operator +=(const XY &wh)      {x+=wh.x; y+=wh.y; return *this;}
    XY &   operator -=(const XY &wh)      {x-=wh.x; y-=wh.y; return *this;}
    XY     operator +(const XY &wh) const {return XY(x+wh.x, y+wh.y);}
    XY     operator -(const XY &wh) const {return XY(x-wh.x, y-wh.y);}
    double DotProduct(const XY &B) const  {return x*B.x+y*B.y;}
    XY     operator *(double scale) const {return XY(x*scale, y*scale);}
    XY     operator *=(double scale)      {x*=scale; y*=scale; return *this;}
    XY     operator /(double scale) const {return XY(x/scale, y/scale);}
    XY     operator /=(double scale)      {x/=scale; y/=scale; return *this;}
    double PerpProduct(const XY &B) const {return x*B.y - y*B.x;}        //This is this(T) * B
    double length(void) const             {return sqrt(x*x+y*y);}
    double length_sqr(void) const         {return x*x+y*y;}
    bool   operator ==(const XY& p) const {return x==p.x && y==p.y;}
    XY	   operator -() const             {return XY(-x, -y);}
    bool   operator <(const XY& p) const  {return x!=p.x ? x<p.x : y<p.y;}
    bool   test_equal(const XY& p) const  {return contour::test_equal(x, p.x) && contour::test_equal(y, p.y);}
    bool   test_smaller(const XY& p) const{return contour::test_equal(x, p.x) ? contour::test_smaller(y, p.y) : contour::test_smaller(x, p.x);}
    XY     Rotate90CW() const             {return XY(-y, x);}
    XY     Rotate90CCW() const            {return XY(y, -x);}
	void   Rotate(double cos, double sin) {double X=x; x=X*cos-y*sin; y=X*sin+y*cos;}
	void   RotateAround(const XY&c, double cos, double sin)
	    {double X=x-c.x, Y=y-c.y; x=X*cos-Y*sin+c.x; y=X*sin+Y*cos+c.y;}
    XY &   SwapXY()                       {std::swap(x,y); return *this;}
    XY &   Round()                        {x = floor(x+.5); y=floor(y+.5); return *this;}
    XY &   RoundUp()                      {x = ceil(x); y=ceil(y); return *this;}
    XY &   RoundDown()                    {x = floor(x); y=floor(y); return *this;}
    XY &   Scale(const XY &sc)            {x*=sc.x; y*=sc.y; return *this;}
    XY &   Normalize()                    {const double l = length(); if (l) {x/=l; y/=l;} return *this;}
    double Distance(const XY &p) const    {return sqrt((x-p.x)*(x-p.x)+(y-p.y)*(y-p.y));}
    XY     ProjectOntoLine(const XY&A, const XY&B) const {const XY d = A-B; return B + d*d.DotProduct(*this-B)/d.length_sqr();}
};

inline XY operator*(double a, const XY &xy) {return xy*a;}

typedef enum {WI_OUTSIDE=0, WI_INSIDE, WI_ON_EDGE, WI_ON_VERTEX, WI_IN_HOLE} is_within_t;
inline bool inside(is_within_t t) {return t!=WI_OUTSIDE && t!=WI_IN_HOLE;}

//Structs for compress mechanism
struct Range {
    double from;
    double till;
    Range() {}
    Range(double s, double d) : from(s), till(d) {}
    void MakeInvalid() {from = MaxVal(from); till = -MaxVal(till);}
    bool IsInvalid() const {return from == CONTOUR_INFINITY && till == -CONTOUR_INFINITY;}
    bool Overlaps(const struct Range &r, double gap=0) const   //true if they at least touch
        {return from<=r.till+gap && r.from <= till+gap;}
    Range &operator+=(double a)
        {if (from>a) from=a; if (till<a) till=a; return *this;}
    Range &operator+=(const Range &a)
        {if (from>a.from) from=a.from; if (till<a.till) till=a.till; return *this;}
    Range &operator-=(const Range &a) {
        if (a.IsWithin(from)) from=a.till;
        if (a.IsWithin(till)) till=a.from;
        return *this;}
    Range &operator*=(const Range &a)
        {if (from<a.from) from=a.from; if (till>a.till) till=a.till; return *this;}
    is_within_t IsWithin(double p) const {
		if (p==from || p == till) return WI_ON_VERTEX;
                return from<p && p<till ? WI_INSIDE : WI_OUTSIDE;
    }
    Range &Shift(double a) {from+=a; till+=a; return *this;}
    Range &Scale(double a) {from*=a; till*=a; if (a<0) std::swap(from, till); return *this;}
    Range &Expand(double a) {from-=a; till+=a; return *this;}
    bool HasValidFrom() const {return from != CONTOUR_INFINITY;}
    bool HasValidTill() const {return till != -CONTOUR_INFINITY;}
    double Spans() const {return till-from;}
    double MidPoint() const {return (from+till)/2;}
    bool operator <(const Range &r) const {
        if (till==r.till) return from<r.from;
        return till<r.till;
    }
    bool operator ==(const Range &r) const {return from==r.from && till==r.till;}
    Range & Round()       {from = floor(from+.5); till=floor(till+.5); return *this;}
    Range & RoundUp()     {from = ceil(from); till=ceil(till); return *this;}
    Range & RoundDown()   {from = floor(from); till=floor(till); return *this;}
    Range & RoundWider()  {from = floor(from); till=ceil(till); return *this;}
    Range & RoundCloser() {from = ceil(from); till=floor(till); return *this;}

    //Returns negative if inside
    double Distance(double a) const {_ASSERT(!IsInvalid()); return minabs(from-a,a-till);}
};

struct Block {
    struct Range x;
    struct Range y;
    Block() {}
    Block(double sx, double dx, double sy, double dy) :
        x(std::min(sx, dx),std::max(sx,dx)), y(std::min(sy, dy),std::max(sy,dy)) {}
    Block(const XY &ul, const XY &dr) :
        x(std::min(ul.x, dr.x),std::max(ul.x, dr.x)), y(std::min(ul.y, dr.y),std::max(ul.y, dr.y)) {}
    Block(const Range &X, const Range &Y) : x(X), y(Y) {}
    void MakeInvalid() {x.MakeInvalid(); y.MakeInvalid();}
    bool IsInvalid() const {return x.IsInvalid() || y.IsInvalid();}
    //operator required for set ordering
    //this improves performance when checking overlaps (lower blocks later)
    bool operator <(const struct Block &b) const {
        if (y.till != b.y.till) return y.till < b.y.till;
        if (x.till != b.x.till) return x.till < b.x.till;
        if (x.from != b.x.from) return x.from < b.x.from;
        return y.from < b.y.from;
    }
    bool operator == (const struct Block &b) const {return x==b.x && y==b.y;}
    bool Overlaps(const struct Block &b, double gap=0) const
        {return x.Overlaps(b.x, gap) && y.Overlaps(b.y, gap);}
    XY UpperLeft(void) const
        {return XY(x.from, y.from);}
    XY LowerRight(void) const
        {return XY(x.till, y.till);}
    XY UpperRight(void) const
        {return XY(x.till, y.from);}
    XY LowerLeft(void) const
        {return XY(x.from, y.till);}
    XY Centroid(void) const
        {return XY(x.MidPoint(), y.MidPoint());}
    XY Spans(void) const
        {return XY(x.Spans(), y.Spans());}
    double GetArea() const {return IsInvalid() ? 0 : x.Spans()*y.Spans();}
    double GetCircumference() const {return IsInvalid() ? 0 : 2*(x.Spans()+y.Spans());}
    is_within_t IsWithin(const XY &p) const {
        if (x.IsWithin(p.x) == WI_OUTSIDE   || y.IsWithin(p.y) == WI_OUTSIDE)   return WI_OUTSIDE;
        if (x.IsWithin(p.x) == WI_INSIDE    && y.IsWithin(p.y) == WI_INSIDE)    return WI_INSIDE;
        if (x.IsWithin(p.x) == WI_ON_VERTEX && y.IsWithin(p.y) == WI_ON_VERTEX) return WI_ON_VERTEX;
        return WI_ON_EDGE;
    }
    Block & operator +=(const XY &p)
        {x += p.x; y += p.y; return *this;}
    Block & operator +=(const Block &b)
        {x += b.x; y += b.y; return *this;}
    Block &Shift(const XY &a)
        {x.Shift(a.x); y.Shift(a.y); return *this;}
    Block &Scale(const XY &sc) {x.Scale(sc.x); y.Scale(sc.y); return *this;}
    Block &Scale(double sc) {x.Scale(sc); y.Scale(sc); return *this;}
    Block &Expand(double a) {x.Expand(a); y.Expand(a); return *this;}
    Block CreateExpand(double a) const {Block b(*this); b.Expand(a); return b;}
    Block &SwapXY() {std::swap(x,y); return *this;}
    Block & Round()       {x.Round(); y.Round(); return *this;}
    Block & RoundUp()     {x.RoundUp(); y.RoundUp(); return *this;}
    Block & RoundDown()   {x.RoundDown(); y.RoundDown(); return *this;}
    Block & RoundWider()  {x.RoundWider(); y.RoundWider(); return *this;}
    Block & RoundCloser() {x.RoundCloser(); y.RoundCloser(); return *this;}

    double Distance(const XY &xy) const;
    double Distance(const Block &b) const;
};

inline double Block::Distance(const XY &xy) const 
{
    const double xd = x.Distance(xy.x);
    const double yd = y.Distance(xy.y);
    //if both outside we return the distance
    if (fsign(xd)>0 && fsign(yd)>0) return sqrt(xd*xd + yd*yd);
    //if both inside, we are fully inside the box, return the smaller abs value
    if (fsign(xd)<0 && fsign(yd)<0) return std::max(xd, yd); 
    //else we return the one with bigger sign (positive or zero)
    return fsign(xd) > fsign(yd) ? xd : yd;
}

inline double Block::Distance(const Block &b) const 
{
    const double uld = Distance(b.UpperLeft());
    const double urd = Distance(b.UpperRight());
    const double lld = Distance(b.LowerLeft());
    const double lrd = Distance(b.LowerRight());
    //if some squares of b are inside and some are outside (or there is a touch), return 0
    if (fsign(uld) != fsign(lrd) || fsign(uld) != fsign(urd) || fsign(uld) != fsign(lld))
        return 0;
    //if all are either inside or outside, return the one with smallest abs value
    return minabs(minabs(uld, urd), minabs(lld, lrd));
}


} //namespace

#endif //CONTOUR_BASICS_H
