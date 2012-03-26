/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011,2012 Zoltan Turanyi
    Distributed under GNU Affero General Public License.

    Msc-generator is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Msc-generator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with Msc-generator.  If not, see <http://www.gnu.org/licenses/>.
*/
#if !defined(COMMANDS_H)
#define COMMANDS_H

#include "arcs.h"

class ArcCommand : public ArcBase
{
public:
    ArcCommand(MscArcType t, Msc *msc) : ArcBase(t, msc) {};
    bool AddAttribute(const Attribute &) {return false;}
    string Print(int ident=0) const;
    virtual void Draw(MscCanvas &/*canvas*/, DrawPassType /*pass*/) {}
};

class CommandEntity : public ArcCommand
{
protected:
    EntityDefList entities;
    bool full_heading;
    mutable AreaList cover_at_0;
public:
    CommandEntity(EntityDefList *e, Msc *msc);
    virtual bool CanBeNoted() const {return true;}
    virtual TrackableElement* AttachNote(CommandNote *);
	bool IsFullHeading() {return full_heading;}
	void MoveMyEntityDefsAfter(EntityDefList *e) {if (e) e->splice(e->end(), entities);} //effectively empty 'entities'
    string Print(int ident=0) const;
    void AppendToEntities(const EntityDefList &e);
    void Combine(CommandEntity *ce);
    bool AddAttribute(const Attribute &);
    CommandEntity *ApplyPrefix(const char *prefix);
    void ApplyShowToChildren(const string &name, bool show);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    EntityDef* FindAddEntityDefForEntity(const string &entity, const file_line_range &l);
    EEntityStatus GetCombinedStatus(const std::set<string>& children) const;
    virtual ArcBase* PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right, Numbering &number, bool top_level);
    virtual void MoveNotesToChart();
    virtual void Width(MscCanvas &canvas, EntityDistanceMap &distances);
    virtual double Height(MscCanvas &canvas, AreaList &cover, bool reflow);

    virtual void ShiftBy(double y);
    virtual void PostPosProcess(MscCanvas &cover, double autoMarker);
    virtual void Draw(MscCanvas &canvas, DrawPassType pass);
};

class CommandNewpage : public ArcCommand
{
public:
    CommandNewpage(Msc *msc)
        : ArcCommand(MSC_COMMAND_NEWPAGE, msc) {compress=false;}
    bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    virtual double Height(MscCanvas &canvas, AreaList &cover, bool reflow);

    virtual void PostPosProcess(MscCanvas &cover, double autoMarker);
};

class CommandNewBackground : public ArcCommand
{
public:
    MscFillAttr fill;

    CommandNewBackground(Msc *msc, MscFillAttr f)
        : ArcCommand(MSC_COMMAND_NEWBACKGROUND, msc), fill(f) {}
    virtual double Height(MscCanvas &canvas, AreaList &cover, bool reflow);

    virtual void PostPosProcess(MscCanvas &cover, double autoMarker);
};

class CommandNumbering : public ArcCommand
{
public:
    typedef enum {INCREMENT=1, DECREMENT=2, SIZE=4} EAction;
    EAction action;
    size_t  length;

    CommandNumbering(Msc *msc, EAction a, size_t l=0)
        : ArcCommand(MSC_COMMAND_NUMBERING, msc), action(a), length(l) {if (l) action = EAction(action | SIZE); AddAttributeList(NULL);}
    virtual ArcBase* PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right, Numbering &number, bool top_level);
};

class CommandMark : public ArcCommand
{
public:
    string name;
    double offset;
    CommandMark(const char *m, file_line_range ml, Msc *msc);
    bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    virtual double Height(MscCanvas &canvas, AreaList &cover, bool reflow);

    virtual void ShiftBy(double y);
};

class CommandEmpty : public ArcCommand
{
    Label parsed_label;
public:
    CommandEmpty(Msc *msc);
    virtual void Width(MscCanvas &canvas, EntityDistanceMap &distances);
    virtual double Height(MscCanvas &canvas, AreaList &cover, bool reflow);

    virtual void Draw(MscCanvas &canvas, DrawPassType pass);
};

class NamePair
{
public:
    string src, dst;
    file_line_range sline, dline;
    NamePair(const char *s, const file_line_range &sl,
                   const char *d, const file_line_range &dl) :
        src(s ? s : ""), dst(d ? d : ""), sline(sl), dline(dl) {}
};

class CommandHSpace : public ArcCommand
{
protected:
    EIterator src, dst;
    file_line_range sline, dline;
    StringFormat format;
    std::pair<bool, string> label;
    std::pair<bool, double> space;
public:
    CommandHSpace(Msc*, const NamePair*);
    virtual bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    virtual ArcBase* PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right, Numbering &number, bool top_level);
    virtual void Width(MscCanvas &canvas, EntityDistanceMap &distances);
};

class CommandVSpace : public ArcCommand
{
protected:
    StringFormat format;
    std::pair<bool, string> label;
    std::pair<bool, double> space;
    bool compressable;
public:
    CommandVSpace(Msc*);
    virtual bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    virtual ArcBase* PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right, Numbering &number, bool top_level);
    virtual double Height(MscCanvas &canvas, AreaList &cover, bool reflow);

};

class ExtVertXPos : public VertXPos
{
public:
    enum {LEFT=0, CENTER=1, RIGHT=2, NONE=3, BAD_SIDE} side;
    file_line_range side_line;
    explicit ExtVertXPos(Msc&m) : VertXPos(m), side(NONE) {}
    ExtVertXPos(const char *s, const file_line_range &sl, const VertXPos *p);
    ExtVertXPos(const VertXPos *p);
};

class CommandSymbol : public ArcCommand
{
protected:
    typedef enum {ARC, RECTANGLE, ELLIPSIS} SymbolType;
    static const double ellipsis_space_ratio;
    SymbolType              symbol_type;
    MscStyle                style;
    ExtVertXPos             hpos1, hpos2;
    NamePair                vpos;
    std::pair<bool, double> xsize, ysize;  //used for 'arc' and 'rectangle'
    MscArrowSize            size;          //used for '...'
    mutable Block outer_edge;
public:
    CommandSymbol(Msc*, const char *symbol, const NamePair *enp,
                  const ExtVertXPos *vxpos1, const ExtVertXPos *vxpos2);
    virtual bool CanBeNoted() const {return true;}
    virtual bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    virtual ArcBase* PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right, Numbering &number, bool top_level);
    virtual void Width(MscCanvas &canvas, EntityDistanceMap &distances);
    virtual double Height(MscCanvas &canvas, AreaList &cover, bool reflow);

    virtual void ShiftBy(double y);
    virtual void PostPosProcess(MscCanvas &cover, double autoMarker);
    void CalculateAreaFromOuterEdge();
    virtual void Draw(MscCanvas &canvas, DrawPassType pass);
};

class CommandNote : public ArcLabelled
{
protected:
    friend class NotePlacement;
    TrackableElement *    target;
    const ExtVertXPos     extvertxpos;
    string                point_toward; //an entity or NoEntity for center of target
    string                ypos_marker;  //a markername or empty for automatic
    file_line             ypos_marker_linenum;
    int                   float_dir_x, float_dir_y, float_dist; //user preferences of floating note placement

    mutable XY            halfsize, pos_center, point_to;
    mutable EIterator     point_toward_iterator;
    double pointer_width(double distance) const;
    Contour cover_pointer(MscCanvas &canvas, const XY &point_to, const XY &center) const; //places upper left corner of the body to 0,0
public:
    CommandNote(Msc*, const ExtVertXPos *vxpos, AttributeList *al);
    TrackableElement *GetTarget() const {return target;}
    virtual bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    virtual ArcBase* PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right, Numbering &number, bool top_level);
    virtual void FinalizeLabels(MscCanvas &canvas);
    virtual void Width(MscCanvas &canvas, EntityDistanceMap &distances);

    Contour CoverBody(MscCanvas &canvas, const XY &center) const; //places upper left corner to 0,0
    Contour CoverPointer(MscCanvas &canvas, const XY &pointto, const XY &center) const //places upper left corner of the body to 0,0
        {return cover_pointer(canvas, pointto, center) - CoverBody(canvas, center);} 
    Contour CoverAll(MscCanvas &canvas, const XY &pointto, const XY &center) const //places upper left corner of the body to 0,0
        {return cover_pointer(canvas, pointto, center) + CoverBody(canvas, center);} 
    Contour GetRegion(const Contour &region_belt, int dir_x, int dir_y, XY &start_xy) const;
    void PlaceTo(MscCanvas &canvas, const XY &pointto, const XY &center);
    void PlaceFloating(MscCanvas &canvas);

    virtual void ShiftBy(double y);
    //virtual void PostPosProcess(MscCanvas &cover, double autoMarker);
    virtual void Draw(MscCanvas &canvas, DrawPassType pass);
};


#endif //COMMANDS_H