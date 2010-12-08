#if !defined(GEOMETRY_BASICS_H)
#define GEOMETRY_BASICS_H

#ifndef _ASSERT
#define  _ASSERT(A)
#endif

#include <cfloat>
#include <utility>

using namespace std::rel_ops;  //so that we have != and <= and >= etc from only == and <

namespace geometry {

inline double Infinity() {return DBL_MAX;}

class XY {
public:
    double x;
    double y;
    XY() {}
    XY(double a, double b) : x(a), y(b) {}
    XY &   operator +=(XY wh)             {x+=wh.x; y+=wh.y; return *this;}
    XY &   operator -=(XY wh)             {x-=wh.x; y-=wh.y; return *this;}
    XY     operator +(XY wh) const        {return XY(x+wh.x, y+wh.y);}
    XY     operator -(XY wh) const        {return XY(x-wh.x, y-wh.y);}
    double DotProduct(XY B) const         {return x*B.x+y*B.y;}
    XY     operator *(double scale) const {return XY(x*scale, y*scale);}
    XY     operator *=(double scale)      {x*=scale; y*=scale; return *this;}
    XY     operator /(double scale) const {return XY(x/scale, y/scale);}
    XY     operator /=(double scale)      {x/=scale; y/=scale; return *this;}
    double PerpProduct(XY B) const        {return x*B.y - y*B.x;}        //This is this(T) * B
    double length(void) const             {return sqrt(x*x+y*y);}
    bool   operator ==(const XY& p) const {return x==p.x && y==p.y;}
    XY	   operator -() const             {return XY(-x, -y);}
    bool   operator <(const XY& p) const  {return x!=p.x ? x<p.x : y<p.y;}
    XY     Rotate90CW() const             {return XY(-y, x);}
    XY     Rotate90CCW() const            {return XY(y, -x);}
	void   Rotate(double cos, double sin) {double X=x; x=X*cos-y*sin; y=X*sin+y*cos;}
	void   RotateAround(const XY&c, double cos, double sin) 
	    {double X=x-c.x, Y=y-c.y; x=X*cos-Y*sin+c.x; y=X*sin+Y*cos+c.y;}
};

typedef enum {WI_OUTSIDE=0, WI_INSIDE, WI_ON_EDGE, WI_ON_VERTEX} is_within_t;

//Structs for compress mechanism
struct Range {
    double from;
    double till;
    Range() {}
    Range(double s, double d) : from(s), till(d) {}
    void MakeInvalid() {from = DBL_MAX; till = -DBL_MAX;}
    bool IsInvalid() const {return from == Infinity() && till == -Infinity();}
    bool Overlaps(const struct Range &r, double gap=0) const   //true if they at least touch
        {return from<=r.till+gap && r.from <= till+gap;}
    Range &operator+=(double a)
        {if (from>a) from=a; if (till<a) till=a; return *this;}
    Range &operator+=(const Range &a)
        {if (from>a.from) from=a.from; if (till<a.till) till=a.till; return *this;}
    Range &operator*=(const Range &a)
        {if (from<a.from) from=a.from; if (till>a.till) till=a.till; return *this;}
    is_within_t IsWithin(double p) const {
		if (p==from || p == till) return WI_ON_VERTEX;
                return from<p && p<till ? WI_INSIDE : WI_OUTSIDE;
    }
    Range &Shift(double a) {from +=a; till+=a; return *this;}
    bool HasValidFrom() const {return from != Infinity();}
    bool HasValidTill() const {return till != -Infinity();}
    double Spans() const
        {return till-from;}
    bool operator <(const Range &r) const {
        if (till==r.till) return from<r.from;
        return till<r.till;
    }
    bool operator ==(const Range &r) const {return from==r.from && till==r.till;}
};

struct Block {
    struct Range x;
    struct Range y;
    Block() {}
    Block(double sx, double dx, double sy, double dy) :
        x(std::min(sx, dx),std::max(sx,dx)), y(std::min(sy, dy),std::max(sy,dy)) {}
    Block(const XY &ul, const XY &dr) :
        x(std::min(ul.x, dr.x),std::max(ul.x, dr.x)), y(std::min(ul.y, dr.y),std::max(ul.y, dr.y)) {}
    void MakeInvalid() {x.MakeInvalid(); y.MakeInvalid();}
    bool IsInvalid() const {return x.IsInvalid() && y.IsInvalid();}
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
        {return XY(x.from, y.till);}
    XY LowerLeft(void) const
        {return XY(x.from, y.till);}
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
    void Transpose() {std::swap(x,y);}
};

}; //namespace

#endif //GEOMETRY_BASICS_H
