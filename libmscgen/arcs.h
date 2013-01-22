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
#if !defined(ARCS_H)
#define ARCS_H

#include "color.h"
#include "style.h"
#include "entity.h"
#include "numbering.h"
#include "csh.h"
#include "area.h"
#include "progress.h"

typedef enum
{
    MSC_ARC_INVALID = 0,
    MSC_ARC_SOLID,
    MSC_ARC_SOLID_BIDIR,
    MSC_ARC_DOTTED,
    MSC_ARC_DOTTED_BIDIR,
    MSC_ARC_DASHED,
    MSC_ARC_DASHED_BIDIR,
    MSC_ARC_DOUBLE,
    MSC_ARC_DOUBLE_BIDIR,
    MSC_ARC_UNDETERMINED_SEGMENT,

    MSC_ARC_BIG,        /* For Block Arrows generated from a collapsed box */
    MSC_ARC_BIG_BIDIR,  /* For Block Arrows generated from a collapsed box */

    MSC_EMPH_SOLID,
    MSC_EMPH_DOTTED,
    MSC_EMPH_DASHED,
    MSC_EMPH_DOUBLE,
    MSC_EMPH_UNDETERMINED_FOLLOW,

    MSC_ARC_PARALLEL,   /* not visible, just container for multiple parallel flows */

    MSC_ARC_DISCO,      /* ... Discontinuity in time line */
    MSC_ARC_DIVIDER,    /* --- Divider */
    MSC_ARC_VSPACE,     /* no arc, just space (maybe with label) */

    MSC_COMMAND_HEADING,
    MSC_COMMAND_NUDGE,
    MSC_COMMAND_ENTITY,
    MSC_COMMAND_NEWPAGE,
    MSC_COMMAND_NEWBACKGROUND,
    MSC_COMMAND_NUMBERING,
    MSC_COMMAND_MARK,
    MSC_COMMAND_EMPTY,
    MSC_COMMAND_HSPACE,
    MSC_COMMAND_VSPACE,
    MSC_COMMAND_SYMBOL,
    MSC_COMMAND_NOTE,
    MSC_COMMAND_TITLE,
    MSC_COMMAND_SUBTITLE,

    MSC_ARC_ARCLIST,
    MSC_ARC_INDICATOR
} EArcType;

typedef enum {
    MSC_DIR_INDETERMINATE,
    MSC_DIR_RIGHT, /*forward*/
    MSC_DIR_LEFT,
    MSC_DIR_BIDIR
} EDirType;

class EntityDistanceMap;
struct ArcSignature;
//Box attributes we save to determine, if a box previously collapsed on the GUI
//is the same as a box after re-compilation (and potentially source text modification)
enum BoxCollapseType {BOX_COLLAPSE_INVALID=0, BOX_COLLAPSE_EXPAND, BOX_COLLAPSE_COLLAPSE, BOX_COLLAPSE_BLOCKARROW};
typedef std::map<ArcSignature, BoxCollapseType> ArcSignatureCatalog;
struct ArcSignature {
    FileLineColRange file_pos;
    bool operator == (const ArcSignature&o) const;
    bool operator < (const ArcSignature&o) const;
    ArcSignatureCatalog::iterator WhichIsSimilar(ArcSignatureCatalog &cat) const;
};

class ArcBase : public Element
{
protected: 
    friend int yyparse (Msc &, void *);
    void SetParallel() {parallel = true;}
private:
    bool had_add_attr_list;    //TODO: debug only, remove
protected:
    bool         valid;        /* If false, then construction failed, arc does not exist */
    bool         at_top_level; /* if at top level by PostParseProcess() */
    bool         compress;     /* if compress mechanism is on for this arc */
    bool         parallel;     /* if so, it will not set the area.mainline.till in DrawHeight */
    bool         keep_together; /* do not split this by automatic pagination.*/
    bool         keep_with_next; /* do not separate this from following element by automatic pagination */
    string         refname;    /* given by the "refname" attribute, to reference numbers & others*/
    mutable double height;     /* calculated by Layout()*/
public:
    const EArcType type;

    ArcBase(EArcType t, Msc *msc);
	virtual ~ArcBase();
    virtual MscProgress::ECategory GetProgressCategory() const {_ASSERT(0); return MscProgress::NO_CATEGORY;}
    bool IsValid() const {return valid;}
    virtual const ArcSignature* GetSignature() const {return NULL;}
    bool IsParallel() const {return parallel;}
    bool IsCompressed() const {return compress;}
    bool IsKeepTogether() const {return keep_together;}
    bool IsKeepWithNext() const {return keep_with_next;}
    virtual bool CanBeNoted() const {return false;}
    double GetPos() const {return yPos;}
    /* Return the height of the element with its side comments */
    double GetHeight() const {return std::max(height, comment_height);}
    /** Tells if page break crosses us and how much we need to be shifted down. 0 if not crossed. */
    virtual Range GetYExtent() const {return Range(yPos, yPos+GetHeight());}
    //Get an (ordered) list of entities that this arrow/box touches
    virtual EDirType GetToucedEntities(EntityList &) const {return MSC_DIR_INDETERMINATE;}
    //expands the contour to a cover used for compress
    Area GetCover4Compress(const Area &a) const;

    //Adding attributes and helpers for color syntax highlighting and hinting
    virtual ArcBase* AddAttributeList(AttributeList *);
    virtual bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);

    //Converting to text for debugging
    virtual string PrintType(void) const;
    virtual string Print(int ident = 0) const = 0;

    //These functions are called recursively for all arcs in this order
    /* This is called after parsing and adding attributes. Entity order and collapse/expand is already known here */
    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, bool top_level, Element **note_target);
    /* This is called to substitute name references in labels & process all escapes */
    virtual void FinalizeLabels(Canvas &canvas);
    /* This fills in distances for hscale=auto mechanism */
    virtual void Width(Canvas &/*canvas*/, EntityDistanceMap &/*distances*/) {}
    /* Calculates the height, and sets up the area at yPos==0, returns its cover to use at placement*/
    /* Cover or area does not include any spacing left around such as chart->emphVGapAbove*/
    virtual void Layout(Canvas &canvas, AreaList &cover);
    /* One can move the arc to its position with ShiftBy. This can be called multiple times. */
    virtual void ShiftBy(double y) {if (valid) {Element::ShiftBy(y);}}
    /* Collect the y position of page breaks into Msc::pageBreakData. next is the Arc immediately after us.*/
    virtual void CollectPageBreak(double /*hSize*/) {}
    /** Split the element into two by a page break. Maintain running_state of entities. Return the amount the element has grown. -1 if we cannot rearrange, -2 if we are to ignore.*/
    virtual double SplitByPageBreak(Canvas &/*canvas*/, double /*netPrevPageSize*/,
                                    double /*pageBreak*/, bool &/*addCommandNewpage*/, 
                                    bool /*addHeading*/, ArcList &/*res*/) {return -1;}
    /* Goes through the tree to place verticals. All height & pos info final by now, except on verticals & notes */
    virtual void PlaceWithMarkers(Canvas &/*cover*/, double /*autoMarker*/) {}
    /* This goes through the tree once more for drawing warnings that need height. */
    virtual void PostPosProcess(Canvas &cover);
    /* This will actually draw the arc */
    virtual void Draw(Canvas &canvas, EDrawPassType pass) = 0;
};

typedef PtrList<ArcBase> ArcList;

class ArcIndicator : public ArcBase
{
    const MscStyle style;
    EIterator src;  //Shall always point to ActiveEntities
    EIterator dst;  //Shall always point to ActiveEntities
public:
    ArcIndicator(Msc *chart, const MscStyle &st, const FileLineColRange &l);
    ArcIndicator(Msc *chart, EIterator s, const MscStyle &st, const FileLineColRange &l);
	virtual MscProgress::ECategory GetProgressCategory() const {return MscProgress::INDICATOR;}
    bool IsComplete() const;
    void SetEntities(EIterator s, EIterator d) {src=s; dst=d;}
    bool Combine(const ArcIndicator *o);
    double GetXCoord() const;
    virtual EDirType GetToucedEntities(class EntityList &el) const;
    virtual string Print(int ident = 0) const {return string(ident*2, ' ')+"Indicator";}
    virtual void Width(Canvas &canvas, EntityDistanceMap &distances);
    virtual void Layout(Canvas &canvas, AreaList &cover);

    virtual void Draw(Canvas &canvas, EDrawPassType pass);
};

class ArcLabelled : public ArcBase
{
protected:
    friend class CommandNote;
    string          label;
    Label           parsed_label;
    int             concrete_number; //if >=0 it shows what number the user wanted for this arc. if <0 automatic or no numerbing
    MscStyle        style;           //numbering and compress fields of style are not used. The Arc member fields are used instead.
    NumberingStyle  numberingStyle;  //This is not part of styles in general, but of contexts
    string          number_text;     //the formatted number (for references, e.g., notes)
public:
    ArcLabelled(EArcType t, Msc *msc, const MscStyle &);
    ArcLabelled(EArcType t, const ArcLabelled &al);
    const MscStyle &GetStyle() const {return style;}
    virtual bool CanBeNoted() const {return true;}
    void SetStyleWithText(const char *style_name); //set style to this name, but combine it with default text style
    void SetStyleWithText(const MscStyle *style_to_use=NULL); //set style to this name, but combine it with default text style
    virtual const MscStyle *GetRefinementStyle(EArcType t) const;
    ArcBase* AddAttributeList(AttributeList *);
    bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    string Print(int ident=0) const;
    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, bool top_level, Element **note_target);
    const string &GetNumberText() const {return number_text;}
    virtual void FinalizeLabels(Canvas &canvas);
    virtual void PostPosProcess(Canvas &cover);
};

class ArcArrow : public ArcLabelled
{
public:
    ArcArrow(EArcType t, Msc *msc, const MscStyle &s) : ArcLabelled(t, msc, s) {}
    ArcArrow(EArcType t, const ArcLabelled &al) : ArcLabelled(t, al) {}
    virtual ArcArrow *AddSegment(EArcType t, const char *m, FileLineColRange ml, FileLineColRange l) = 0;
    bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    bool isBidir(void) const {return type == MSC_ARC_SOLID_BIDIR || type == MSC_ARC_DOTTED_BIDIR ||
                                     type == MSC_ARC_DASHED_BIDIR || type == MSC_ARC_DOUBLE_BIDIR ||
                                     type == MSC_ARC_BIG_BIDIR;}
};

class ArcSelfArrow : public ArcArrow
{
protected:
    EIterator src;
    double    YSize;
    const double XSizeUnit;

    mutable XY xy_s, xy_e, wh; //arrow parameters
    mutable double sx, dx;     //text parameters
    mutable double src_act;
public:
    ArcSelfArrow(EArcType t, const char *s, FileLineColRange sl,
        Msc *msc, const MscStyle &, double ys);
	virtual MscProgress::ECategory GetProgressCategory() const {return MscProgress::SELF_ARROW;}
    virtual ArcArrow *AddSegment(EArcType t, const char *m, FileLineColRange ml, FileLineColRange l);
    virtual EDirType GetToucedEntities(EntityList &el) const;
    string Print(int ident=0) const;
    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, bool top_level, Element **note_target);
    virtual void Width(Canvas &canvas, EntityDistanceMap &distances);
    virtual void Layout(Canvas &canvas, AreaList &cover);

    virtual void PostPosProcess(Canvas &cover);
    virtual void Draw(Canvas &canvas, EDrawPassType pass);
};


class ArcDirArrow : public ArcArrow
{
protected:
    EIterator                src, dst;
    FileLineCol                linenum_src, linenum_dst;
    std::vector<EIterator>   middle;
    std::vector<FileLineCol>   linenum_middle;
    std::vector<EArcType>  segment_types; //one for each segment ([0] is the one from src), set during AddSegment()s
    std::vector<LineAttr> segment_lines; //one for each segment ([0] is the one from src), set during AddAttributeList()
    const bool               specified_as_forward; //true if user specified "a->b", false if "b<-a"
    double                   slant_angle;

    mutable double sin_slant, cos_slant;
    mutable double sx, dx;     //xpos of two final arrowheads (sx can be > dx)
    mutable double sx_text, dx_text, cx_text; //xpos for text display (sorted)
    mutable std::vector<double> xPos; //positions sorted
    mutable std::vector<double> act_size;  //activation size sorted
    mutable std::vector<DoublePair> margins; //margins sorted
    mutable Contour text_cover;   //so that we can hide entity lines in PostPosProcess - not used by ArcBigArrow
    mutable double centerline; //y offset of line compared to yPos
    mutable Contour clip_area; //Used only by DirArrow to mask out the line at arrowheads
public:
    ArcDirArrow(EArcType t, const char *s, FileLineColRange sl,
        const char *d, FileLineColRange dl, Msc *msc, bool fw, const MscStyle &);
    ArcDirArrow(const EntityList &el, bool bidir, const ArcLabelled &al);
	virtual MscProgress::ECategory GetProgressCategory() const {return MscProgress::DIR_ARROW;}
    virtual ArcArrow *AddSegment(EArcType t, const char *m, FileLineColRange ml, FileLineColRange l);
    ArcBase *AddAttributeList(AttributeList *l);
    bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    virtual EDirType GetToucedEntities(EntityList &el) const;
    string Print(int ident=0) const;
    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, bool top_level, Element **note_target);
    virtual void FinalizeLabels(Canvas &canvas);
    virtual void Width(Canvas &canvas, EntityDistanceMap &distances);
    virtual void Layout(Canvas &canvas, AreaList &cover);

    void CalculateMainline(double thickness);
    EArrowEnd WhichArrow(unsigned i); //from the index of xPos or marging give MSC_ARROW_{START,MIDDLE,END}
    virtual void ShiftBy(double y);
    void CheckSegmentOrder(double y);
    virtual void PostPosProcess(Canvas &cover);
    virtual void Draw(Canvas &canvas, EDrawPassType pass);
};

class ArcBigArrow : public ArcDirArrow
{
protected:
    const ArcSignature * const sig;   //non-null if created from a collapsed box with content
    XY getArrowWidthHeight(EArrowSize) const;

    mutable double sy, dy;    //The middle of the contour of the body: set in Width already
    mutable double ind_off;   //if we draw an indicator inside, this is its y offset from top of text
    mutable int stext, dtext; //filled by Width: index the two entity in xPos between which the text spans (sorted)
    mutable double sm, dm;    //filled by Width: margin (left and right) for text
    mutable std::vector<Contour> outer_contours; //the outer line of all segments
public:
    ArcBigArrow(const ArcDirArrow &, const MscStyle &);
    ArcBigArrow(const EntityList &, bool bidir, const ArcLabelled &al, const ArcSignature *s);
    ~ArcBigArrow() {if (sig) delete sig;}
	virtual MscProgress::ECategory GetProgressCategory() const {return MscProgress::BLOCK_ARROW;}
    virtual const MscStyle *GetRefinementStyle(EArcType t) const;
    virtual const ArcSignature* GetSignature() const {return sig;}
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    string Print(int ident=0) const;
    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, bool top_level, Element **note_target);
    virtual void FinalizeLabels(Canvas &canvas);
    virtual void Width(Canvas &canvas, EntityDistanceMap &distances);
    virtual void Layout(Canvas &canvas, AreaList &cover);

    virtual void ShiftBy(double y);
    virtual void PostPosProcess(Canvas &cover);
    virtual void Draw(Canvas &canvas, EDrawPassType pass);
};

struct VertXPos {
    bool valid;
    EIterator entity1, entity2;
    FileLineColRange e1line, e2line;
    /** AT is at the entity
     ** CENTER is between the two entities
     ** BY is immediately at the line, but not overlapping it
     ** SIDE is immediately at the line, arrow tips overlap the line */
    enum postype {POS_INVALID=0, POS_AT, POS_CENTER,
        POS_LEFT_BY, POS_LEFT_SIDE,
        POS_RIGHT_BY, POS_RIGHT_SIDE} pos;
    double offset;
    VertXPos(Msc&m, const char *e1, FileLineColRange e1l, const char *e2, FileLineColRange e2l, postype p=POS_AT, double off=0);
    VertXPos(Msc&m, const char *e1, FileLineColRange e1l, postype p=POS_AT, double off=0);
    explicit VertXPos(Msc&m);
    double CalculatePos(Msc &m, double width=0, double gap=-1) const;
};

class ArcVerticalArrow : public ArcArrow
{
protected:
    string src, dst;   //vertical position
    VertXPos pos;
    double offset; //horizontal position base offset
    mutable std::vector<double> ypos; //calculate them in PlaceWithMarkers
    mutable double sy_text, dy_text;
    mutable double xpos, width;
    mutable std::vector<Contour> outer_contours;
public:
    ArcVerticalArrow(EArcType t, const char *s, const char *d, Msc *msc);
	virtual MscProgress::ECategory GetProgressCategory() const {return MscProgress::VERTICAL;}
    ArcArrow *AddSegment(EArcType t, const char *m, FileLineColRange ml, FileLineColRange l);
    ArcVerticalArrow* AddXpos(VertXPos *p);
    virtual const MscStyle *GetRefinementStyle(EArcType t) const;
    bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    virtual Element* AttachNote(CommandNote *);
    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, bool top_level, Element **note_target);
    virtual void Width(Canvas &canvas, EntityDistanceMap &distances);
    virtual void Layout(Canvas &canvas, AreaList &cover);

    virtual void ShiftBy(double y);
    virtual Range YExtent() {return Range(false);}
    virtual double SplitByPageBreak(Canvas &/*canvas*/, double /*netPrevPageSize*/,
                                    double /*pageBreak*/, bool &/*addCommandNewpage*/, 
                                    bool /*addHeading*/, ArcList &/*res*/) {return -2;}
    virtual void PlaceWithMarkers(Canvas &cover, double autoMarker);
    virtual void PostPosProcess(Canvas &cover);
    virtual void Draw(Canvas &canvas, EDrawPassType pass);
};

class ArcBox : public ArcLabelled
{
    friend struct pipe_compare;
    friend class ArcBoxSeries;
    friend class ArcPipe;
    friend struct ArcSignature;
protected:
    EIterator       src, dst;
    BoxCollapseType collapsed;
    bool            drawEntityLines; //true if we draw the entity lines (only if there is content)
    ArcList         content;

    mutable double height_w_lower_line;
    mutable double sx_text, dx_text, y_text;  //label placement
    mutable Contour text_cover;
public:
    //Constructor to construct the first box/pipe in a series
    ArcBox(EArcType t, const char *s, FileLineColRange sl,
        const char *d, FileLineColRange dl, Msc *msc);
	virtual MscProgress::ECategory GetProgressCategory() const {return MscProgress::BOX;}
    virtual bool CanBeNoted() const {return true;}
    virtual const ArcSignature* GetSignature() const;
    ArcBox* AddArcList(ArcList*l);
    ArcBase* AddAttributeList(AttributeList *);
    bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    string Print(int ident=0) const;
    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, bool top_level, Element **note_target);
    virtual void FinalizeLabels(Canvas &canvas);
    virtual void Layout(Canvas &/*canvas*/, AreaList &/*cover*/) {_ASSERT(0);}
    virtual void ShiftBy(double y);
    virtual void Draw(Canvas &/*canvas*/, EDrawPassType /*pass*/) {} //will never be called
};

class ArcBoxSeries : public ArcBase
{
protected:
    PtrList<ArcBox> series;
    int             drawing_variant; //how to draw double or triple lines

    mutable double total_height;
    mutable double left_space, right_space;  //how much do we expand beyond src/dst. Include lw and shadow
public:
    //Constructor to construct the first box/pipe in a series
    ArcBoxSeries(ArcBox *first);
	virtual MscProgress::ECategory GetProgressCategory() const {return MscProgress::BOX_SERIES;}
    ArcBoxSeries* AddFollow(ArcBox *f);
    virtual EDirType GetToucedEntities(EntityList &el) const;
    string Print(int ident=0) const;
    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, bool top_level, Element **note_target);
    virtual void FinalizeLabels(Canvas &canvas);
    virtual void Width(Canvas &canvas, EntityDistanceMap &distances);
    virtual void Layout(Canvas &canvas, AreaList &cover);

    virtual void ShiftBy(double y);
    virtual void CollectPageBreak(double hSize);
    virtual double SplitByPageBreak(Canvas &canvas, double netPrevPageSize,
                                    double pageBreak, bool &addCommandNewpage, 
                                    bool addHeading, ArcList &res);
    virtual void PlaceWithMarkers(Canvas &cover, double autoMarker);
    virtual void PostPosProcess(Canvas &cover);
    virtual void Draw(Canvas &canvas, EDrawPassType pass);
};

class ArcPipe : public ArcLabelled
{
    friend struct pipe_compare;
    friend class ArcPipeSeries;
protected:
    EIterator       src, dst;
    bool            drawEntityLines; //true if we draw the entity lines (only if there is content)

    mutable bool pipe_connect_back, pipe_connect_forw; //true if connects to neighbour pipe in pipe series
    mutable double left_space, right_space;  //how much do we expand beyond src/dst. Include lw and shadow
    mutable double sx_text, dx_text, y_text;  //label placement
    mutable Contour text_cover;
    mutable Block pipe_block;   //representative rectangle of pipe                    ____
    //These below correspond to the body of the pipe (skewed rectangle with curves): (____(
    mutable Contour pipe_shadow, pipe_body_line, pipe_whole_line, pipe_body_fill;
    mutable Contour pipe_hole_fill, pipe_hole_line;    //These refer to the hole __
    mutable Contour pipe_hole_curve;                   //                        _()
public:
    //Constructor to construct the first box/pipe in a series
    ArcPipe(ArcBox *box);
	virtual MscProgress::ECategory GetProgressCategory() const {return MscProgress::PIPE;}
    virtual bool CanBeNoted() const {return true;}
    bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    string Print(int ident=0) const;
    virtual void Layout(Canvas &/*canvas*/, AreaList &/*cover*/) {_ASSERT(0);}
    virtual void ShiftBy(double y);
    void DrawPipe(Canvas &canvas, bool topSideFill, bool topSideLine, bool backSide, bool shadow,
                  bool text, double next_lw, int drawing_variant);
    virtual void Draw(Canvas &/*canvas*/, EDrawPassType /*pass*/) {} //will never be called
};

class ArcPipeSeries : public ArcBase
{
protected:
    PtrList<ArcPipe> series;
    ArcList          content;
    int              drawing_variant; //how to draw double or triple lines

    mutable double total_height;
public:
    //Constructor to construct the first box/pipe in a series
    ArcPipeSeries(ArcPipe *first);
	virtual MscProgress::ECategory GetProgressCategory() const {return MscProgress::PIPE_SERIES;}
    ArcPipeSeries* AddFollowWithAttributes(ArcPipe*f, AttributeList *l);
    ArcPipeSeries* AddArcList(ArcList*l);
    virtual EDirType GetToucedEntities(EntityList &el) const;
    string Print(int ident=0) const;
    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, bool top_level, Element **note_target);
    virtual void FinalizeLabels(Canvas &canvas);
    virtual void Width(Canvas &canvas, EntityDistanceMap &distances);
    void CalculateContours(Area *pipe_body_cover=NULL);
    virtual void Layout(Canvas &canvas, AreaList &cover);

    virtual void ShiftBy(double y);
    virtual void CollectPageBreak(double hSize);
    virtual double SplitByPageBreak(Canvas &canvas, double netPrevPageSize,
                                    double pageBreak, bool &addCommandNewpage, 
                                    bool addHeading, ArcList &res);
    virtual void PlaceWithMarkers(Canvas &cover, double autoMarker);
    virtual void PostPosProcess(Canvas &cover);
    virtual void Draw(Canvas &canvas, EDrawPassType pass);
};



class ArcDivider : public ArcLabelled
{
protected:
    const bool nudge;
    const bool title;
    bool wide;  //if true, we keep no margin and add no arcvgapabove & below (for copyright text)
    const double extra_space;

    mutable double centerline;
    mutable double text_margin, line_margin;
    mutable Contour text_cover;
public:
    ArcDivider(EArcType t, Msc *msc);
	virtual MscProgress::ECategory GetProgressCategory() const {return MscProgress::DIVIDER;}
    static const char *MyStyleName(EArcType t);
    bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh, bool nudge, bool title);
    static bool AttributeValues(const std::string attr, Csh &csh, bool nudge, bool title);
    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, bool top_level, Element **note_target);
    virtual void Width(Canvas &canvas, EntityDistanceMap &distances);
    virtual void Layout(Canvas &canvas, AreaList &cover);

    virtual void ShiftBy(double y);
    virtual void PostPosProcess(Canvas &cover);
    virtual void Draw(Canvas &canvas, EDrawPassType pass);
};

class ArcParallel : public ArcBase
{
protected:
    std::vector<ArcList> blocks;
public:
    ArcParallel(Msc *msc) : ArcBase(MSC_ARC_PARALLEL, msc) {}
	virtual MscProgress::ECategory GetProgressCategory() const {return MscProgress::PARALLEL;}
    ArcParallel* AddArcList(ArcList*l) {if (l) {blocks.push_back(std::move(*l)); l->clear(); delete l; keep_together = false;} return this;}
    virtual EDirType GetToucedEntities(EntityList &el) const;
    string Print(int ident=0) const;
    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, bool top_level, Element **note_target);
    virtual void FinalizeLabels(Canvas &canvas);
    virtual void Width(Canvas &canvas, EntityDistanceMap &distances);
    virtual void Layout(Canvas &canvas, AreaList &cover);

    virtual void ShiftBy(double y);
    virtual void CollectPageBreak(double hSize);
    virtual double SplitByPageBreak(Canvas &canvas, double netPrevPageSize,
                                    double pageBreak, bool &addCommandNewpage, 
                                    bool addHeading, ArcList &res);
    virtual void PlaceWithMarkers(Canvas &cover, double autoMarker);
    virtual void PostPosProcess(Canvas &cover);
    virtual void Draw(Canvas &canvas, EDrawPassType pass);
};


#endif //ARCS_H