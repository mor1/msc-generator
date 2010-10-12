#if !defined(POLYGON_H)
#define POLYGON_H

#include <set>
#include <climits>
#include "error.h" //for file_line
#include "trackable.h"
#include "cairo.h"

//inside of polygon is to the right as we go through: counterclockwise pointlists contain the "outside"
//never degenerate - always has actual area
//never flopped - edges never cross each-other
//always contagious - one single surface
class MscPointList : protected std::vector<XY>
{
protected:
	MscPointList() {};
	void Path(cairo_t *cr) const;
	friend class MscPolygon;
public:
    MscPointList(double sx, double dx, double sy, double dy) {operator = (Block(sx,dx,sy,dy));}
	MscPointList(const Block &b) {operator =(b);}
	MscPointList(XY a, XY b, XY c);
	MscPointList &operator =(const Block &b);
	bool GetBoundingBox(Block &b) const;
	void Expand(MscPointList &pointlist, double gap) const;
    bool Overlaps(const MscPointList &b, double gap=0) const;
    bool IsWithin(XY p) const;
    bool Union(const MscPointList &b, MscPointList &result) const; //true if OK, false if not touching
	void Rotate90Clockwise(XY);
	void Rotate90CounterClockwise(XY);
};


class MscPolygon {
protected:
	std::list<MscPointList> pl;  
	mutable Block	boundingBox;
	mutable bool	boundingBoxCurrent;
public:
	mutable TrackableElement *arc;
    mutable enum DrawType {DRAW_NONE, DRAW_NORMAL, DRAW_FRAME} drawType;
    mutable enum FindType {FIND_NONE, FIND_NORMAL} findType;

    explicit MscPolygon(TrackableElement *a=NULL) : arc(a), drawType(DRAW_NORMAL), findType(FIND_NORMAL) 
		{clear();}
	MscPolygon(const MscPointList &pll, TrackableElement *a=NULL) : arc(a), drawType(DRAW_NORMAL), findType(FIND_NORMAL),
		boundingBoxCurrent(false), pl(1, pll) {}
    MscPolygon(double sx, double dx, double sy, double dy, TrackableElement *a=NULL) : arc(a),
		drawType(DRAW_NORMAL), findType(FIND_NORMAL), boundingBoxCurrent(false), pl(1, MscPointList(sx,dx,sy,dy)) {}
    MscPolygon(const Block &b, TrackableElement *a=NULL) : arc(a), drawType(DRAW_NORMAL), findType(FIND_NORMAL),
		boundingBoxCurrent(false), pl(1, MscPointList(b)) {}
	MscPolygon(XY a, XY b, XY c, TrackableElement *te=NULL) : arc(te), drawType(DRAW_NORMAL), findType(FIND_NORMAL),
		boundingBoxCurrent(false), pl(1, MscPointList(a,b,c)) {}

	MscPolygon &operator =(const MscPointList &pll) {pl.clear(); pl.push_back(pll); boundingBoxCurrent=false;}

	void clear() {pl.clear(); pl.push_back(MscPointList()); boundingBoxCurrent=true; boundingBox.MakeInvalid();}
	void swap(MscPolygon &a);

    bool IsEmpty() const {return pl.begin()->size()<3;}
	const Block &GetBoundingBox(void) const;
	void Expand(MscPolygon &polygon, double gap) const;
    bool Overlaps(const MscPolygon &b, double gap=0) const;
    bool IsWithin(XY p) const;
    bool Union(const MscPolygon &b, MscPolygon &result) const; //true if OK, false if not touching
	void Rotate90Clockwise(XY);
	void Rotate90CounterClockwise(XY);

	void Line(cairo_t *cr) const;
	void Fill(cairo_t *cr) const;
};
//
//template <typename PolygonContainer>
//bool BoundingBlock(const PolygonContainer &container, Block &ret, bool reuse=false)
//{
//    if (container.size()==0) return false;
//	if (!reuse) ret.MakeInvalid();
//	Block b;
//    for(typename PolygonContainer::const_iterator i=container.begin(); i!=container.end(); i++) 
//		if (i->GetBoundingBox(b)) ret |= b;
//    return true;
//};
//
//template <typename PolygonContainer>
//const Block *InWhich(const PolygonContainer &container, XY p) //search backwards - as drawing z order decreases
//{
//    for (typename PolygonContainer::const_reverse_iterator i=container.rbegin(); i!=container.rend(); i++)
//        if (i->findType==Block::FIND_NORMAL && i->IsWithin(p)) return &(*i);
//    return NULL;
//};
//
class Geometry2
{
	typedef std::list<MscPolygon> cover_t;
    cover_t       cover;
	mutable Block boundingBox;
	mutable bool  boundingBoxCurrent;
public:
    Range           mainline;

    Geometry2() : boundingBoxCurrent(false) {mainline.MakeInvalid();}
    const cover_t &GetCover() const {return cover;}
    void clear() {mainline.MakeInvalid(); cover.clear(); boundingBoxCurrent=false;}
	void swap(Geometry2 &a);
    void SetArc(TrackableElement *a) const {
        for(cover_t::const_iterator i=cover.begin(); i!=cover.end(); i++)
            i->arc = a;
    }
    void SetDrawType(MscPolygon::DrawType t) const {
        for(cover_t::const_iterator i=cover.begin(); i!=cover.end(); i++)
            i->drawType = t;
    }
    void SetFindType(MscPolygon::FindType t) const {
        for(cover_t::const_iterator i=cover.begin(); i!=cover.end(); i++)
            i->findType = t;
    }
    Geometry2 &operator+=(const Geometry2 &g);
	Geometry2 &operator+=(const MscPolygon &g);
    bool IsEmpty() const {return cover.size()==0;}
    bool Overlaps(const Geometry2 &g, double gap=0) const {
        for(cover_t::const_iterator i=cover.begin(); i!=cover.end(); i++)
            for(cover_t::const_iterator j=g.cover.begin(); j!=g.cover.end(); j++)
                if (j->Overlaps(*i, gap))
                    return true;
        return false;
    }
	const Block &GetBoundingBox(void) const;
    bool IsWithin(XY p) const;
	void Line(cairo_t *cr) const;
	void Fill(cairo_t *cr) const;
};

#endif //POLYGON_H
