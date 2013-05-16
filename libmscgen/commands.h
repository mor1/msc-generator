/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011,2012,2013 Zoltan Turanyi
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
    ArcCommand(EArcType t, MscProgress::ECategory c, Msc *msc) : ArcBase(t, c, msc) {};
    bool AddAttribute(const Attribute &) {return false;}
    string Print(int ident=0) const;
    virtual void Layout(Canvas &canvas, AreaList *cover);
    virtual void Draw(Canvas &/*canvas*/, EDrawPassType /*pass*/) {}
};

class CommandEntity : public ArcCommand
{
protected:
    EntityDefList entities;
    bool full_heading;
    PtrList<CommandNote>   tmp_stored_notes;
    std::list<std::string> tmp_stored_note_targets;
    std::string            target_entity; //subsequent note shall target this
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
    EntityDef* FindAddEntityDefForEntity(const string &entity, const FileLineColRange &l);
    EEntityStatus GetCombinedStatus(const std::set<string>& children) const;
    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, Element **note_target);
    virtual void Width(Canvas &canvas, EntityDistanceMap &distances);
    virtual void LayoutCommentsHelper(Canvas &canvas, AreaList *cover, double &l, double &r);
    virtual void Layout(Canvas &canvas, AreaList *cover);

    virtual void ShiftBy(double y);
    virtual double SplitByPageBreak(Canvas &/*canvas*/, double /*netPrevPageSize*/,
                                    double /*pageBreak*/, bool &/*addCommandNewpage*/, 
                                    bool /*addHeading*/, ArcList &/*res*/);
    virtual void PostPosProcess(Canvas &cover);
    virtual void Draw(Canvas &canvas, EDrawPassType pass);
};

class CommandNewpage : public ArcCommand
{
    bool auto_heading_attr;
    CommandEntity *autoHeading; ///<automatically inserted heading that follows
public:
    const bool manual; ///<True if this was inserted by the user manually
    CommandNewpage(Msc *msc, bool m, CommandEntity *ah);
    ~CommandNewpage() {if (autoHeading) delete autoHeading;}
	virtual MscProgress::ECategory GetProgressCategory() const {return MscProgress::NEWPAGE;}
    bool AddAttribute(const Attribute &a);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);

    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, Element **note_target);
    virtual void FinalizeLabels(Canvas &canvas);
    virtual void Width(Canvas &canvas, EntityDistanceMap &distances);
    virtual void Layout(Canvas &canvas, AreaList *cover);
    virtual void ShiftBy(double y);
    virtual void CollectPageBreak(double hSize);
    virtual void PlaceWithMarkers(Canvas &/*cover*/, double /*autoMarker*/);
    virtual void PostPosProcess(Canvas &cover);
    virtual void Draw(Canvas &/*canvas*/, EDrawPassType /*pass*/);
};

class CommandNewBackground : public ArcCommand
{
public:
    FillAttr fill;

    CommandNewBackground(Msc *msc, FillAttr f) : 
        ArcCommand(MSC_COMMAND_NEWBACKGROUND, MscProgress::TINY_EFFORT, msc), 
        fill(f) {}
    virtual void PostPosProcess(Canvas &cover);
};

class CommandNumbering : public ArcCommand
{
public:
    enum EAction {
        INCREMENT=1, 
        DECREMENT=2, 
        SIZE=4
    };
    EAction action;
    size_t  length;

    CommandNumbering(Msc *msc, EAction a, size_t l=0) :
        ArcCommand(MSC_COMMAND_NUMBERING, MscProgress::TINY_EFFORT, msc), 
        action(a), length(l) 
        {if (l) action = EAction(action | SIZE); AddAttributeList(NULL);}
    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, Element **note_target);
};

class CommandMark : public ArcCommand
{
public:
    string name;
    double offset;
    CommandMark(const char *m, FileLineColRange ml, Msc *msc);
    bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);

    virtual void ShiftBy(double y);
};

class CommandEmpty : public ArcCommand
{
    Label parsed_label;
public:
    CommandEmpty(Msc *msc) : 
        ArcCommand(MSC_COMMAND_EMPTY, MscProgress::EMPTY, msc) {}
    virtual void Width(Canvas &canvas, EntityDistanceMap &distances);
    virtual void Layout(Canvas &canvas, AreaList *cover);

    virtual void Draw(Canvas &canvas, EDrawPassType pass);
};

class NamePair
{
public:
    string src, dst;
    FileLineColRange sline, dline;
    NamePair(const char *s, const FileLineColRange &sl,
                   const char *d, const FileLineColRange &dl) :
        src(s ? s : ""), dst(d ? d : ""), sline(sl), dline(dl) {}
};

class CommandHSpace : public ArcCommand
{
protected:
    EIterator src, dst;
    FileLineColRange sline, dline;
    StringFormat format;
    std::pair<bool, string> label;
    std::pair<bool, double> space;
public:
    CommandHSpace(Msc*, const NamePair*);
    virtual bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, Element **note_target);
    virtual void Width(Canvas &canvas, EntityDistanceMap &distances);
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
    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, Element **note_target);
    virtual void Layout(Canvas &canvas, AreaList *cover);

};

class ExtVertXPos : public VertXPos
{
public:
    enum {LEFT=0, CENTER=1, RIGHT=2, NONE=3, BAD_SIDE} side;
    FileLineColRange side_line;
    explicit ExtVertXPos(Msc&m) : VertXPos(m), side(NONE) {}
    ExtVertXPos(const char *s, const FileLineColRange &sl, const VertXPos *p);
    ExtVertXPos(const VertXPos *p);
};

class CommandSymbol : public ArcCommand
{
protected:
    enum ESymbolType {
        ARC, 
        RECTANGLE, 
        ELLIPSIS
    };
    static const double ellipsis_space_ratio;
    ESymbolType             symbol_type;
    StyleCoW                style;
    ExtVertXPos             hpos1, hpos2;
    NamePair                vpos;
    std::pair<bool, double> xsize, ysize;  //used for 'arc' and 'rectangle'
    EArrowSize              size;          //used for '...'
    mutable Block outer_edge;
public:
    CommandSymbol(Msc*, const char *symbol, const NamePair *enp,
                  const ExtVertXPos *vxpos1, const ExtVertXPos *vxpos2);
    virtual bool CanBeNoted() const {return true;}
    virtual bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, Element **note_target);
    virtual void Width(Canvas &canvas, EntityDistanceMap &distances);
    virtual void Layout(Canvas &canvas, AreaList *cover);

    virtual void ShiftBy(double y);
    /** We are to be ignored if we are off-line (outer_edge is valid) or we cannot rearrange if in-line */
    virtual double SplitByPageBreak(Canvas &/*canvas*/, double /*netPrevPageSize*/,
                                    double /*pageBreak*/, bool &/*addCommandNewpage*/, 
                                    bool /*addHeading*/, ArcList &/*res*/)
                                       {return outer_edge.y.IsInvalid() ? -2 : -1;}
    virtual void PlaceWithMarkers(Canvas &cover, double autoMarker);
    void CalculateAreaFromOuterEdge();
    virtual void Draw(Canvas &canvas, EDrawPassType pass);
};

struct score_t;

class CommandNote : public ArcLabelled
{
public:
    const bool            is_float;
protected:
    Element *    target;
    string                point_toward; //an entity or NoEntity for center of target
    FileLineColRange       point_toward_pos;
    std::pair<bool, int>  float_dist; //user preferences of floating note placement
    int                   float_dir_x;
    int                   float_dir_y;

    mutable XY            halfsize, pos_center, point_to;
    mutable EIterator     point_toward_iterator;
    double pointer_width(double distance) const;
    Contour cover_pointer(Canvas &canvas, const XY &point_to, const XY &center) const; //places upper left corner of the body to 0,0
public:
    CommandNote(Msc*, bool is_note, const char *pt=NULL, const FileLineColRange &ptm=FileLineColRange());
    ~CommandNote();
    virtual bool CanBeNoted() const {return false;}
    Element *GetTarget() const {return target;}
    void SetTarget(EntityDef *e) {target = e;}
    void Invalidate() {valid = false;} //called by Msc::InvalidateNotesToThisTarget if target becomes hidden
    virtual bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh, bool is_float);
    static bool AttributeValues(const std::string attr, Csh &csh, bool is_float);
    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, Element **note_target);
    virtual void FinalizeLabels(Canvas &canvas);
    virtual void Width(Canvas &canvas, EntityDistanceMap &distances);
    virtual void Layout(Canvas &canvas, AreaList *cover);
    virtual void ShiftBy(double /*y*/) {} //Comments are shifted by their owner, notes are laid out last and shall not be shifted anyway
    virtual double SplitByPageBreak(Canvas &/*canvas*/, double /*netPrevPageSize*/,
                                    double /*pageBreak*/, bool &/*addCommandNewpage*/, 
                                    bool /*addHeading*/, ArcList &/*res*/) {return -2;} //Ignore comments for pagination (moved with their owner)

    Contour CoverBody(Canvas &canvas, const XY &center) const; //places upper left corner to 0,0
    Contour CoverPointer(Canvas &canvas, const XY &pointto, const XY &center) const //places upper left corner of the body to 0,0
        {return cover_pointer(canvas, pointto, center) - CoverBody(canvas, center);}
    Contour CoverAll(Canvas &canvas, const XY &pointto, const XY &center) const //places upper left corner of the body to 0,0
        {return cover_pointer(canvas, pointto, center) + CoverBody(canvas, center);}
    static Contour GetRegionMask(const Block &outer, const XY &center, int dir_x, int dir_y);
    std::vector<std::pair<XY, XY>> GetPointerTarget() const;
    void CoverPenalty(const XY &pointto, const XY &center, Canvas &canvas,
                      const Contour &block_all, const Contour &block_imp,
                      score_t &cover_penalty) const;
    void SlantPenalty(const XY &pointto, const XY &center, const XY &tangent, 
                      score_t &slant_penalty) const;
    static bool GetAPointInside(const Contour &c, const XY &p1, const XY &p2, XY&ret);
    static bool GetAPointInside(const DoubleMap<bool> &map, double &ret);
    void PlaceFloating(Canvas &canvas);
    void PlaceSideTo(Canvas &canvas, AreaList *cover, double &y);

    virtual void ShiftCommentBy(double y);
    //virtual void PostPosProcess(Canvas &cover, double autoMarker);
    virtual void Draw(Canvas &canvas, EDrawPassType pass);
};

class CommandArcList : public ArcCommand
{
protected:
    ArcList content;
public:
    explicit CommandArcList(Msc *m) : 
        ArcCommand(MSC_ARC_ARCLIST, MscProgress::TINY_EFFORT, m), content(true) {}
    CommandArcList(Msc *m, ArcBase *p) :  
        ArcCommand(MSC_ARC_ARCLIST, MscProgress::TINY_EFFORT, m), content(true) {Append(p);}
    CommandArcList(Msc *m, ArcList *l) :  
        ArcCommand(MSC_ARC_ARCLIST, MscProgress::TINY_EFFORT, m), content(true) {Append(l);}

    void Append(ArcBase *p) {content.Append(p);}
    void Append(ArcList *l) {content.splice(content.end(), *l);}
    void MoveContent(ArcList &list, ArcList::iterator after) {list.splice(++after, content);}
    virtual double SplitByPageBreak(Canvas &/*canvas*/, double /*netPrevPageSize*/,
                                    double /*pageBreak*/, bool &/*addCommandNewpage*/, 
                                    bool /*addHeading*/, ArcList &/*res*/)
                                             {_ASSERT(0); return -1;}
};

#endif //COMMANDS_H