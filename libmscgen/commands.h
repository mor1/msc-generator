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
    virtual void Layout(MscCanvas &canvas, AreaList &cover);
    virtual void Draw(MscCanvas &/*canvas*/, DrawPassType /*pass*/) {}
};

class CommandEntity : public ArcCommand
{
protected:
    EntityDefList entities;
    bool full_heading;
    PtrList<CommandNote>   tmp_stored_notes;
    std::list<std::string> tmp_stored_note_targets;
    std::string            target_entity; //subsequent note shall target this
    mutable AreaList cover_at_0;
public:
    const bool internally_defined; //true if generated by an lcomment.* command: will not merge with other CommandEntities
    CommandEntity(EntityDefHelper *e, Msc *msc, bool in);
    virtual bool CanBeNoted() const {return !internally_defined;}
    virtual void AttachComment(CommandNote *);
	bool IsFullHeading() {return full_heading;}
    void TmpStoreNote(CommandNote *);
    void ReinsertTmpStoredNotes(ArcList &list, ArcList::iterator after);
	void MoveMyContentAfter(EntityDefHelper &e); //effectively empty 'entities'
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
    virtual ArcBase* PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, bool top_level, TrackableElement **note_target);
    virtual void Width(MscCanvas &canvas, EntityDistanceMap &distances);
    virtual void CommentHeightHelper(MscCanvas &canvas, AreaList &cover, double &l, double &r);
    virtual void Layout(MscCanvas &canvas, AreaList &cover);

    virtual void ShiftBy(double y);
    virtual double SplitByPageBreak(MscCanvas &/*canvas*/, double /*netPrevPageSize*/,
                                    double /*pageBreak*/, bool &/*addCommandNewpage*/, 
                                    bool /*addHeading*/, ArcList &/*res*/);
    virtual void PostPosProcess(MscCanvas &cover);
    virtual void Draw(MscCanvas &canvas, DrawPassType pass);
};

class CommandNewpage : public ArcCommand
{
public:
    CommandEntity *const autoHeading; ///<For automatically inserted PBs, this can be a heading that follows
    const bool manual; ///<True if this was inserted by the user manually
    CommandNewpage(Msc *msc, bool m, CommandEntity *ah)
        : ArcCommand(MSC_COMMAND_NEWPAGE, msc), autoHeading(ah), 
        manual(m) {compress=false;}
    ~CommandNewpage() {if (autoHeading) delete autoHeading;}
    bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);

    virtual void ShiftBy(double y);
    virtual void CollectPageBreak(double hSize);
};

class CommandNewBackground : public ArcCommand
{
public:
    MscFillAttr fill;

    CommandNewBackground(Msc *msc, MscFillAttr f)
        : ArcCommand(MSC_COMMAND_NEWBACKGROUND, msc), fill(f) {}

    virtual void PostPosProcess(MscCanvas &cover);
};

class CommandNumbering : public ArcCommand
{
public:
    typedef enum {INCREMENT=1, DECREMENT=2, SIZE=4} EAction;
    EAction action;
    size_t  length;

    CommandNumbering(Msc *msc, EAction a, size_t l=0)
        : ArcCommand(MSC_COMMAND_NUMBERING, msc), action(a), length(l) {if (l) action = EAction(action | SIZE); AddAttributeList(NULL);}
    virtual ArcBase* PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, bool top_level, TrackableElement **note_target);
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

    virtual void ShiftBy(double y);
};

class CommandEmpty : public ArcCommand
{
    Label parsed_label;
public:
    CommandEmpty(Msc *msc);
    virtual void Width(MscCanvas &canvas, EntityDistanceMap &distances);
    virtual void Layout(MscCanvas &canvas, AreaList &cover);

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
    virtual ArcBase* PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, bool top_level, TrackableElement **note_target);
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
    virtual ArcBase* PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, bool top_level, TrackableElement **note_target);
    virtual void Layout(MscCanvas &canvas, AreaList &cover);

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
    virtual ArcBase* PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, bool top_level, TrackableElement **note_target);
    virtual void Width(MscCanvas &canvas, EntityDistanceMap &distances);
    virtual void Layout(MscCanvas &canvas, AreaList &cover);

    virtual void ShiftBy(double y);
    /** We are to be ignored if we are off-line (outer_edge is valid) or we cannot rearrange if in-line */
    virtual double SplitByPageBreak(MscCanvas &/*canvas*/, double /*netPrevPageSize*/,
                                    double /*pageBreak*/, bool &/*addCommandNewpage*/, 
                                    bool /*addHeading*/, ArcList &/*res*/)
                                       {return outer_edge.y.IsInvalid() ? -2 : -1;}
    virtual void PlaceWithMarkers(MscCanvas &cover, double autoMarker);
    void CalculateAreaFromOuterEdge();
    virtual void Draw(MscCanvas &canvas, DrawPassType pass);
};

struct score_t;

class CommandNote : public ArcLabelled
{
public:
    const bool            is_float;
protected:
    TrackableElement *    target;
    string                point_toward; //an entity or NoEntity for center of target
    file_line_range       point_toward_pos;
    std::pair<bool, int>  float_dist; //user preferences of floating note placement
    int                   float_dir_x;
    int                   float_dir_y;

    mutable XY            halfsize, pos_center, point_to;
    mutable EIterator     point_toward_iterator;
    double pointer_width(double distance) const;
    Contour cover_pointer(MscCanvas &canvas, const XY &point_to, const XY &center) const; //places upper left corner of the body to 0,0
public:
    CommandNote(Msc*, bool is_note, const char *pt=NULL, const file_line_range &ptm=file_line_range());
    ~CommandNote();
    virtual bool CanBeNoted() const {return false;}
    TrackableElement *GetTarget() const {return target;}
    void SetTarget(EntityDef *e) {target = e;}
    void Invalidate() {valid = false;} //called by Msc::InvalidateNotesToThisTarget if target becomes hidden
    virtual bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh, bool is_float);
    static bool AttributeValues(const std::string attr, Csh &csh, bool is_float);
    virtual ArcBase* PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, bool top_level, TrackableElement **note_target);
    virtual void FinalizeLabels(MscCanvas &canvas);
    virtual void Width(MscCanvas &canvas, EntityDistanceMap &distances);
    virtual void Layout(MscCanvas &canvas, AreaList &cover);

    Contour CoverBody(MscCanvas &canvas, const XY &center) const; //places upper left corner to 0,0
    Contour CoverPointer(MscCanvas &canvas, const XY &pointto, const XY &center) const //places upper left corner of the body to 0,0
        {return cover_pointer(canvas, pointto, center) - CoverBody(canvas, center);}
    Contour CoverAll(MscCanvas &canvas, const XY &pointto, const XY &center) const //places upper left corner of the body to 0,0
        {return cover_pointer(canvas, pointto, center) + CoverBody(canvas, center);}
    static Contour GetRegionMask(const Block &outer, const XY &center, int dir_x, int dir_y);
    std::vector<std::pair<XY, XY>> GetPointerTarget() const;
    void CoverPenalty(const XY &pointto, const XY &center, MscCanvas &canvas,
                      const Contour &block_all, const Contour &block_imp,
                      score_t &cover_penalty) const;
    void SlantPenalty(const XY &pointto, const XY &center, const XY &tangent, 
                      score_t &slant_penalty) const;
    static bool GetAPointInside(const Contour &c, const XY &p1, const XY &p2, XY&ret);
    static bool GetAPointInside(const DoubleMap<bool> &map, double &ret);
    void PlaceFloating(MscCanvas &canvas);
    void PlaceSideTo(MscCanvas &canvas, AreaList &cover, double &y);

    virtual void ShiftCommentBy(double y);
    //virtual void PostPosProcess(MscCanvas &cover, double autoMarker);
    virtual void Draw(MscCanvas &canvas, DrawPassType pass);
};

class CommandArcList : public ArcCommand
{
protected:
    ArcList content;
public:
    explicit CommandArcList(Msc *m) : ArcCommand(MSC_ARC_ARCLIST, m), content(true) {}
    CommandArcList(Msc *m, ArcBase *p) :  ArcCommand(MSC_ARC_ARCLIST, m), content(true) {Append(p);}
    CommandArcList(Msc *m, ArcList *l) :  ArcCommand(MSC_ARC_ARCLIST, m), content(true) {Append(l);}

    void Append(ArcBase *p) {content.Append(p);}
    void Append(ArcList *l) {content.splice(content.end(), *l);}
    void MoveContent(ArcList &list, ArcList::iterator after) {list.splice(++after, content);}
    virtual double SplitByPageBreak(MscCanvas &/*canvas*/, double /*netPrevPageSize*/,
                                    double /*pageBreak*/, bool &/*addCommandNewpage*/, 
                                    bool /*addHeading*/, ArcList &/*res*/)
                                             {_ASSERT(0); return -1;}

};


#endif //COMMANDS_H