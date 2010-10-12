#if !defined(TRACKABLE_H)
#define TRACKABLE_H

#include <set>
#include <climits>
#include <cmath>
#include "error.h" //for file_line

class XY {
public:
    double x;
    double y;
    XY() {}
    XY(double a, double b) : x(a), y(b) {}
    XY     operator +(XY wh) const        {return XY(x+wh.x, y+wh.y);}
    XY     operator -(XY wh) const        {return XY(x-wh.x, y-wh.y);} 
    double DotProduct(XY B) const         {return x*B.x+y*B.y;} 
    XY     operator *(double scale) const {return XY(x*scale, y*scale);}
	double PerpProduct(XY B) const        {return x*B.y - y*B.x;}        //This is this(T) * B
	double length(void) const             {return sqrt(x*x+y*y);}
	bool operator ==(const XY& p) const {return x==p.x && y==p.y;}
	bool operator !=(const XY& p) const {return x!=p.x || y!=p.y;}
};

//Structs for compress mechanism
#define MAINLINE_INF INT_MAX
struct Range {
    double from;
    double till;
    Range() {}
    Range(double s, double d) : from(s), till(d) {}
    void MakeInvalid() {from = MAINLINE_INF; till = -MAINLINE_INF;}
    bool IsInvalid() const {return from == MAINLINE_INF && till == -MAINLINE_INF;}
    bool Overlaps(const struct Range &r, double gap=0) const
        {return from<r.till+gap && r.from < till+gap;}
    void Extend(double a)
        {if (from>a) from=a; if (till<a) till=a;}
    void Extend(Range a)
        {if (from>a.from) from=a.from; if (till<a.till) till=a.till;}
    bool IsWithin(double p) const
        {return from<=p && p<=till;}
    bool HasValidFrom() const {return from != MAINLINE_INF;}
    bool HasValidTill() const {return till != -MAINLINE_INF;}
    double Spans() const
        {return till-from;}
	bool operator <(const Range &r) const {
		if (till==r.till) return from<r.from;
		return till<r.till;
	}
	bool operator ==(const Range &r) const {return from==r.from && till==r.till;}
};

class TrackableElement;

struct Block {
    struct Range x;
    struct Range y;
    mutable TrackableElement *arc;
    mutable enum DrawType {DRAW_NONE, DRAW_NORMAL, DRAW_FRAME} drawType;
    mutable enum FindType {FIND_NONE, FIND_NORMAL} findType;
    explicit Block(TrackableElement *a=NULL) : arc(a), drawType(DRAW_NORMAL), findType(FIND_NORMAL) {}
    Block(double sx, double dx, double sy, double dy, TrackableElement *a=NULL) : arc(a),
        drawType(DRAW_NORMAL), findType(FIND_NORMAL),
        x(std::min(sx, dx),std::max(sx,dx)), y(std::min(sy, dy),std::max(sy,dy)) {}
    Block(XY ul, XY dr, TrackableElement *a=NULL) : arc(a), drawType(DRAW_NORMAL), findType(FIND_NORMAL) {
        x.from = ul.x<dr.x?ul.x:dr.x; x.till = ul.x>dr.x?ul.x:dr.x;
        y.from = ul.y<dr.y?ul.y:dr.y; y.till = ul.y>dr.y?ul.y:dr.y;
    }
    void MakeInvalid() {x.MakeInvalid(); y.MakeInvalid();}
    bool IsInvalid() const {return x.IsInvalid() && y.IsInvalid();}
    //operator required for set ordering
    //this improves performance when checking overlaps (lower blocks later)
    bool operator <(const struct Block &b) const {
        if (y.till != b.y.till) return y.till < b.y.till;
        if (x.till != b.x.till) return x.till < b.x.till;
        if (x.from != b.x.from) return x.from < b.x.from;
        return y.from < b.y.from;}
	bool operator == (const struct Block &b) const {return x==b.x && y==b.y;}
    bool Overlaps(const struct Block &b, double gap=0) const
        {return x.Overlaps(b.x, gap) && y.Overlaps(b.y, gap);}
    XY UpperLeft(void) const
        {return XY(x.from, y.from);}
    XY LowerRight(void) const
        {return XY(x.till, y.till);}
    bool IsWithin(XY p) const
        {return x.IsWithin(p.x) && y.IsWithin(p.y);}
    Block & operator +=(const XY &p)
        {x.Extend(p.x); y.Extend(p.y); return *this;}
    Block & operator +=(const Block &b)
        {x.Extend(b.x); y.Extend(b.y); return *this;}
};

template <typename BlockContainer>
bool BoundingBlock(const BlockContainer &container, Block &ret, bool reuse=false)
{
    if (container.size()==0) return false;
	if (!reuse) ret.MakeInvalid();
    for(typename BlockContainer::const_iterator i=container.begin(); i!=container.end(); i++) {
        if (ret.x.from > i->x.from) ret.x.from = i->x.from;
        if (ret.x.till < i->x.till) ret.x.till = i->x.till;
        if (ret.y.from > i->y.from) ret.y.from = i->y.from;
        if (ret.y.till < i->y.till) ret.y.till = i->y.till;
    }
    return true;
};

template <typename BlockContainer>
const Block *InWhich(const BlockContainer &container, XY p) //search backwards - as drawing z order decreases
{
    for (typename BlockContainer::const_reverse_iterator i=container.rbegin(); i!=container.rend(); i++)
        if (i->findType==Block::FIND_NORMAL && i->IsWithin(p)) return &(*i);
    return NULL;
};

class Geometry
{
    std::set<Block> cover;
	mutable Block boundingBox;
	mutable bool  boundingBoxCurrent, boundingBoxEmpty;
public:
    Range           mainline;

    Geometry() : boundingBoxCurrent(false) {mainline.MakeInvalid();}
    const std::set<Block> &GetCover() const {return cover;}
    void Clear() {mainline.MakeInvalid(); cover.clear(); boundingBoxCurrent=false;}
    void SetArc(TrackableElement *a) const {
        for(std::set<Block>::const_iterator i=cover.begin(); i!=cover.end(); i++)
            i->arc = a;
    }
    void SetDrawType(Block::DrawType t) const {
        for(std::set<Block>::const_iterator i=cover.begin(); i!=cover.end(); i++)
            i->drawType = t;
    }
    void SetFindType(Block::FindType t) const {
        for(std::set<Block>::const_iterator i=cover.begin(); i!=cover.end(); i++)
            i->findType = t;
    }
    Geometry &operator+=(const Geometry &g) {
        cover.insert(g.cover.begin(), g.cover.end());
        mainline.Extend(g.mainline);
		boundingBoxCurrent=false;
        return *this;
    }
    Geometry &operator-=(const Geometry &g) {
        for(std::set<Block>::const_iterator i=g.cover.begin(); i!=g.cover.end(); i++)
            cover.erase(*i);
		boundingBoxCurrent=false;
        return *this;
    }
    Geometry &operator+=(const Block &b) {
        cover.insert(b);
		boundingBoxCurrent=false;
        return *this;
    }
    Geometry &operator-=(const Block &b) {
        cover.erase(b);
		boundingBoxCurrent=false;
        return *this;
    }
    bool IsEmpty() const {return cover.size()==0;}
    bool Overlaps(const Geometry &g, double gap=0) const {
        for(std::set<Block>::const_iterator i=cover.begin(); i!=cover.end(); i++)
            for(std::set<Block>::const_iterator j=g.cover.begin(); j!=g.cover.end(); j++)
                if (j->Overlaps(*i, gap))
                    return true;
        return false;
    }
    bool GetBoundingBox(Block &b) const {
        if (!boundingBoxCurrent) {
            boundingBoxEmpty = !BoundingBlock(cover, boundingBox);
            boundingBoxCurrent = true;
        }
        b = boundingBox;
        return !boundingBoxEmpty;
    }
};

class TrackableElement {
protected:
    bool linenum_final;
public:
    Geometry  geometry;
    file_line_range file_pos;
    TrackableElement() : linenum_final(false) {}
    void SetLineEnd(file_line_range l, bool f=true);
};

#endif //TRACKABLE_H
