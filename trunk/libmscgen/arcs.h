#if !defined(ARCS_H)
#define ARCS_H

#include "color.h"
#include "style.h"
#include "entity.h"
#include "numbering.h"
#include "csh.h"
#include "contour_area.h"

using namespace contour;

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

    MSC_ARC_BIG,

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
    MSC_COMMAND_EMPTY
} MscArcType;

class EntityDistanceMap;

class ArcBase : public TrackableElement
{
protected:
    Msc   *chart;
    bool   valid;        /* If false, then construction failed, arc does not exist */
    bool   at_top_level; /* if at top level by PostParseProcess() */
    bool   compress;     /* if compress mechanism is on for this arc */
    bool   parallel;     /* if so, it will not set the area.mainline.till in DrawHeight */
    EIterator MinMaxByPos(EIterator i, EIterator value, bool min);
public:
    const MscArcType type;

    ArcBase(MscArcType t, Msc *msc);
    virtual ~ArcBase() {};
    void SetParallel() {parallel = true;}
    bool IsParallel() const {return parallel;}
    bool IsCompressed() const {return compress;}
    double GetPos() const {return yPos;}
    
    //Adding attributes and helpers for color syntax highlighting and hinting
    virtual ArcBase* AddAttributeList(AttributeList *);
    virtual bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    
    //Converting to text for debugging
    virtual string PrintType(void) const;
    virtual string Print(int ident = 0) const = 0;

    //These functions are called recursively for all arcs in this order
    /* This is called after parsing and adding attributes. Entity order is already known here */
    virtual void PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level) {}
    /* This fills in distances for hscale=auto mechanism */
    virtual void Width(EntityDistanceMap &distances) {}
    /* Calculates the height, and sets up the area at yPos==0, returns its cover to use at placement*/
    /* Cover or area does not include any spacing left around such as chart->emphVGapAbove*/
    virtual double Height(AreaList &cover) = 0;
    /* One can move the arc to its position with ShiftBy. This can be called multiple times. */
    virtual void ShiftBy(double y) {TrackableElement::ShiftBy(y);}
    /* This goes through the tree once more for drawing warnings that need height. */
    virtual void PostPosProcess(double autoMarker);
    /* This will actually draw the arc */
    virtual void Draw() = 0;
};

typedef PtrList<ArcBase> ArcList;

class ArcLabelled : public ArcBase
{
protected:
    string          label;
    Label           parsed_label;
    int             concrete_number; //if >=0 it shows what number the user wanted for this arc. if <0 automatic or no numerbing
    MscStyle        style; //numbering and compress fields of style are not used. The Arc member fields are used instead.
    NumberingStyle  numberingStyle; //This is not part of styles in general, but of contexts
public:
    ArcLabelled(MscArcType t, Msc *msc, const MscStyle &);
    ArcBase* AddAttributeList(AttributeList *);
    bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    string Print(int ident=0) const;
    virtual void PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level);
};

class ArcArrow : public ArcLabelled
{
public:
    ArcArrow(MscArcType t, Msc *msc, const MscStyle &s) : ArcLabelled(t, msc, s) {}
    virtual ArcArrow *AddSegment(const char *m, file_line_range ml, bool forward, file_line_range l) = 0;
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    bool isBidir(void) const {return type == MSC_ARC_SOLID_BIDIR || type == MSC_ARC_DOTTED_BIDIR ||
                                     type == MSC_ARC_DASHED_BIDIR || type == MSC_ARC_DOUBLE_BIDIR;}
};

class ArcSelfArrow : public ArcArrow
{
protected:
    EIterator src;
    double    YSize;
    
    mutable XY xy_s, xy_e, wh; //arrow parameters
    mutable double sx, dx;     //text parameters
public:
    ArcSelfArrow(MscArcType t, const char *s, file_line_range sl,
        Msc *msc, const MscStyle &, double ys);
    virtual ArcArrow *AddSegment(const char *m, file_line_range ml, bool forward, file_line_range l);
    string Print(int ident=0) const;
    virtual void PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level);
    virtual void Width(EntityDistanceMap &distances);
    virtual double Height(AreaList &cover);
    virtual void PostPosProcess(double autoMarker);
    virtual void Draw();
};


class ArcDirArrow : public ArcArrow
{
protected:
    EIterator              src, dst;
    file_line              linenum_src, linenum_dst;
    std::vector<EIterator> middle;
    std::vector<file_line> linenum_middle;
    bool                   modifyFirstLineSpacing;

    mutable double sx, dx;     //xpos of two final arrowheads (sx can be > dx)
    mutable double sx_text, dx_text; //xpos for text display (sorted)
    mutable std::vector<double> xPos; //positions sorted
    mutable std::vector<DoublePair> margins; //margins sorted
    mutable Area text_cover;   //so that we can hide entity lines in PostPosProcess - not used by ArcBigArrow
    mutable double centerline; //y offset of line compared to yPos
public:
    ArcDirArrow(MscArcType t, const char *s, file_line_range sl,
        const char *d, file_line_range dl, Msc *msc, const MscStyle &);
    virtual ArcArrow *AddSegment(const char *m, file_line_range ml, bool forward, file_line_range l);
    string Print(int ident=0) const;
    virtual void PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level);
    virtual void Width(EntityDistanceMap &distances);
    virtual double Height(AreaList &cover);
    MscArrowEnd WhichArrow(int i); //from the index of xPos or marging give MSC_ARROW_{START,MIDDLE,END}
    virtual void ShiftBy(double y);
    void CheckSegmentOrder(double y);
    virtual void PostPosProcess(double autoMarker);
    virtual void Draw();
};

class ArcBigArrow : public ArcDirArrow
{
protected:
    XY getArrowWidthHeight(MscArrowSize) const;

    mutable double sy, dy;    //The middle of the contour of the body: set in Width already
    mutable int stext, dtext; //filled by Width: index the two entity in xPos between which the text spans (sorted)
    mutable double sm, dm;    //filled by Width: margin (left and right) for text
public:
    ArcBigArrow(const ArcDirArrow &, const MscStyle &);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    string Print(int ident=0) const;
    virtual void PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level);
    virtual void Width(EntityDistanceMap &distances);
    virtual double Height(AreaList &cover);
    virtual void ShiftBy(double y);
    virtual void PostPosProcess(double autoMarker);
    virtual void Draw();
};

struct VertXPos {
    bool valid;
    EIterator entity1, entity2;
    /** AT is at the entity
     ** CENTER is between the two entities
     ** BY is immediately at the line, but not overlapping it
     ** SIDE is immediately at the line, arrow tips overlap the line */
    enum postype {POS_INVALID=0, POS_AT, POS_CENTER,
        POS_LEFT_BY, POS_LEFT_SIDE,
        POS_RIGHT_BY, POS_RIGHT_SIDE} pos;
    VertXPos(Msc&m, const char *e1, file_line_range e1l, const char *e2, file_line_range e2l, postype p=POS_AT);
    VertXPos(Msc&m, const char *e1, file_line_range e1l, postype p=POS_AT);
    explicit VertXPos(Msc&m, postype p=POS_AT);
};

class ArcVerticalArrow : public ArcArrow
{
protected:
    string src, dst;   //vertical position
    VertXPos pos;
    double offset; //horizontal position base offset
    bool readfromleft;
    bool makeroom;
    mutable std::vector<double> ypos; //calculate them in PostPosProcess
    mutable double sy_text, dy_text;
    mutable double xpos, width;
public:
    ArcVerticalArrow(MscArcType t, const char *s, const char *d,
        VertXPos *p, Msc *msc);
    ArcArrow *AddSegment(const char *m, file_line_range ml, bool forward, file_line_range l);
    bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    void PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level);
    void Width(EntityDistanceMap &distances);
    virtual double Height(AreaList &cover);
    virtual void ShiftBy(double y);
    virtual void PostPosProcess(double autoMarker);
    virtual void Draw();
};

class ArcEmphasis : public ArcLabelled
{
protected:
    EIterator       src, dst;
    bool            pipe;       //True if we display as a pipe
    bool            fromright;  //true if we can see into the pipe's hole from right
    bool            drawEntityLines; //true if we draw the entity lines (only if there is content)
    ArcList        *emphasis;
    ArcEmphasis    *first;      //If null, we are the first
    PtrList<ArcEmphasis> follow;

    //for boxes
    mutable double height;
    //for pipes
    mutable bool pipe_connect_left, pipe_connect_right; //true if connects to neighbour pipe in pipe series
    mutable Block pipe_block;   //The body of the pipe
    mutable Contour pipe_body, pipe_hole; 
    mutable Contour pipe_whole_shadow, pipe_body_fill, pipe_hole_fill;
    mutable Edge pipe_hole_curve;
    //for both
    mutable double total_height;
    mutable double left_space, right_space;  //how much do we expand beyond src/dst. Include lw and shadow
    mutable double sx_text, dx_text, y_text;  //label placement
    mutable Area text_cover;
public:
    //Constructor to construct the first emphasis in a series
    ArcEmphasis(MscArcType t, const char *s, file_line_range sl,
        const char *d, file_line_range dl, Msc *msc);
    ~ArcEmphasis()
    {if (follow.size()>0 && *follow.begin()==this) follow.pop_front(); delete emphasis;}
    ArcEmphasis* SetPipe();
    ArcEmphasis* AddArcList(ArcList*l);
    bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh, bool pipe);
    static bool AttributeValues(const std::string attr, Csh &csh, bool pipe);
    ArcEmphasis* ChangeStyleForFollow(ArcEmphasis* =NULL);
    ArcEmphasis* AddFollow(ArcEmphasis*f);
    string Print(int ident=0) const;
    virtual void PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level);
    virtual void Width(EntityDistanceMap &distances);
    virtual double Height(AreaList &cover);
    virtual void ShiftBy(double y);
    virtual void PostPosProcess(double autoMarker);
    void DrawPipe(bool topSideFill, bool topSideLine, bool backSide, bool text);
    virtual void Draw();
};

class ArcDivider : public ArcLabelled
{
protected:
    const bool nudge;
    bool wide;

    mutable double centerline, height;
    mutable double text_margin, line_margin;
    mutable Area text_cover;
public:
    ArcDivider(MscArcType t, Msc *msc);
    bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    virtual void PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level);
    virtual void Width(EntityDistanceMap &distances);
    virtual double Height(AreaList &cover);
    virtual void ShiftBy(double y);
    virtual void PostPosProcess(double autoMarker);
    virtual void Draw();
};

class ArcParallel : public ArcBase
{
protected:
    PtrList<ArcList> blocks;
    std::vector<double> heights;  //max height of previous blocks
public:
    ArcParallel(Msc *msc) : ArcBase(MSC_ARC_PARALLEL, msc) {}
    ArcParallel* AddArcList(ArcList*l) {blocks.push_back(l); return this;}
    string Print(int ident=0) const;
    virtual void PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level);
    virtual void Width(EntityDistanceMap &distances);
    virtual double Height(AreaList &cover);
    virtual void ShiftBy(double y);
    virtual void PostPosProcess(double autoMarker);
    virtual void Draw();
};

class ArcCommand : public ArcBase
{
public:
    ArcCommand(MscArcType t, Msc *msc) : ArcBase(t, msc) {};
    bool AddAttribute(const Attribute &) {return false;}
    string Print(int ident=0) const;
    virtual double Height(AreaList &cover) {return 0;}
    virtual void Draw() {}
};

class CommandEntity : public ArcCommand
{
protected:
    EntityDefList entities;
    bool full_heading;
public:
    CommandEntity(EntityDefList *e, Msc *msc);
    string Print(int ident=0) const;
    void AppendToEntities(const EntityDefList &e);
    void Combine(CommandEntity *ce);

    virtual void PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level);
    virtual void Width(EntityDistanceMap &distances);
    virtual double Height(AreaList &cover);
    virtual void ShiftBy(double y);
    virtual void PostPosProcess(double autoMarker);
    virtual void Draw();
};

class CommandNewpage : public ArcCommand
{
public:
    CommandNewpage(Msc *msc)
        : ArcCommand(MSC_COMMAND_NEWPAGE, msc) {compress=false;}
    virtual double Height(AreaList &cover);
    virtual void PostPosProcess(double autoMarker);
};

class CommandNewBackground : public ArcCommand
{
public:
    MscFillAttr fill;

    CommandNewBackground(Msc *msc, MscFillAttr f)
        : ArcCommand(MSC_COMMAND_NEWBACKGROUND, msc), fill(f) {}
    virtual double Height(AreaList &cover);
    virtual void PostPosProcess(double autoMarker);
};

class CommandNumbering : public ArcCommand
{
public:
    typedef enum {INCREMENT=1, DECREMENT=2, SIZE=4} EAction;
    EAction action;
    int     length;

    CommandNumbering(Msc *msc, EAction a, int l=0)
        : ArcCommand(MSC_COMMAND_NUMBERING, msc), action(a), length(l) {if (l) action = EAction(action | SIZE);}
    virtual void PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level);
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
    virtual double Height(AreaList &cover);
    virtual void PostPosProcess(double autoMarker);
};

class CommandEmpty : public ArcCommand
{
    Label parsed_label;
public:
    CommandEmpty(Msc *msc);
    virtual void Width(EntityDistanceMap &distances);
    virtual double Height(AreaList &cover);
    virtual void Draw();
};

#endif //ARCS_H