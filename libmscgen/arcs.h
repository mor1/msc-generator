#if !defined(ARCS_H)
#define ARCS_H

#include "trackable.h"
#include "color.h"
#include "style.h"
#include "entity.h"
#include "numbering.h"

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
        Msc       *chart;
        double     yPos;
        bool       valid;        /* If false, then construction failed, arc does not exist */
        bool       at_top_level; /* if at top level by PostParseProcess() */
        bool       compress;     /* if compress mechanism is on for this arc */
        bool       parallel;     /* if so, it will not set the mainline.till in DrawHeight */
        EIterator MinMaxByPos(EIterator i, EIterator value, bool min);
    public:
        const MscArcType type;

        ArcBase(MscArcType t, Msc *msc);
        virtual ~ArcBase() {};
                void SetParallel() {parallel = true;}
                bool IsParallel() const {return parallel;}
                bool IsCompressed() const {return compress;}
                const Geometry &GetGeometry() const {return geometry;};
        virtual ArcBase* AddAttributeList(AttributeList *);
        virtual bool AddAttribute(const Attribute &);
        virtual string PrintType(void) const;
        virtual string Print(int ident = 0) const = 0;
        virtual void PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level) {}

        /* width fills in distances for hscale=auto mechanism */
        virtual void Width(EntityDistanceMap &distances) {}
        /* Height should be possible to call any number of times with final=false,
         * but only once with final=true
         * Then call again with draw=true*/
        virtual double DrawHeight(double yPos, Geometry &g, bool draw, bool final, double autoMarker) =0;
        /* Then PostHeightProcess goes through the tree once more for
         * drawing warnings that need height. */
        virtual void PostHeightProcess(void);
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
        string Print(int ident=0) const;
        virtual void PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level);
};

class ArcArrow : public ArcLabelled
{
    public:
        ArcArrow(MscArcType t, Msc *msc, const MscStyle &s) :
            ArcLabelled(t, msc, s) {}
        virtual ArcArrow *AddSegment(const char *m, file_line_range ml, bool forward, file_line_range l) = 0;
        bool isBidir(void) const
            {return type == MSC_ARC_SOLID_BIDIR || type == MSC_ARC_DOTTED_BIDIR ||
                    type == MSC_ARC_DASHED_BIDIR || type == MSC_ARC_DOUBLE_BIDIR;}
};

class ArcSelfArrow : public ArcArrow
{
    protected:
        EIterator       src;
        double YSize;
    public:
        ArcSelfArrow(MscArcType t, const char *s, file_line_range sl,
                     Msc *msc, const MscStyle &, double ys);
        virtual ArcArrow *AddSegment(const char *m, file_line_range ml, bool forward, file_line_range l);
        string Print(int ident=0) const;
        virtual void PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level);
        virtual void Width(EntityDistanceMap &distances);
        virtual double DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker);
        virtual void PostHeightProcess(void);
};


class ArcDirArrow : public ArcArrow
{
    protected:
        EIterator              src, dst;
        file_line              linenum_src, linenum_dst;
        std::vector<EIterator> middle;
        std::vector<file_line> linenum_middle;
        bool modifyFirstLineSpacing;
    public:
        ArcDirArrow(MscArcType t, const char *s, file_line_range sl,
                    const char *d, file_line_range dl, Msc *msc, const MscStyle &);
        virtual ArcArrow *AddSegment(const char *m, file_line_range ml, bool forward, file_line_range l);
        string Print(int ident=0) const;
        virtual void PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level);
        virtual void Width(EntityDistanceMap &distances);
        virtual double DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker);
        void CheckSegmentOrder(double y);
        virtual void PostHeightProcess(void) {ArcBase::PostHeightProcess(); (yPos);}
};

class ArcBigArrow : public ArcDirArrow
{
    protected:
        ArcList *content;
        double height;
        XY getArrowWidthHeight(MscArrowSize) const;
        mutable EIterator stext, dtext; //filled by Width
        mutable double sm, dm;
    public:
        ArcBigArrow(const ArcDirArrow &, const MscStyle &);
        string Print(int ident=0) const;
        virtual void PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level);
        virtual void Width(EntityDistanceMap &distances);
        virtual double DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker);
        virtual void PostHeightProcess(void) {ArcBase::PostHeightProcess(); CheckSegmentOrder(yPos + height/2);}
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
        double aMarker; //stored autoMarker
        bool readfromleft;
        bool makeroom;
        std::vector<double> ypos; //calculate them in DrawHeight(final)
    public:
        ArcVerticalArrow(MscArcType t, const char *s, const char *d,
                         VertXPos *p, Msc *msc);
        ArcArrow *AddSegment(const char *m, file_line_range ml, bool forward, file_line_range l);
        bool AddAttribute(const Attribute &);
        void PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level);
        void Width(EntityDistanceMap &distances);
        void CalculateXandWidth(double &x, double &width) const;
        double DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker);
        void PostHeightProcess(void);
};

class ArcEmphasis : public ArcLabelled
{
    protected:
        EIterator       src, dst;
        bool            pipe;       //True if we display as a pipe
        ArcList        *emphasis;
        ArcEmphasis    *first;      //If null, we are the first
        PtrList<ArcEmphasis> follow;
        double height, total_height;
        double left_space, right_space;
        bool pipe_connect_left, pipe_connect_right; //true if connects to neighbour pipe in pipe series
    public:
        //Constructor to construct the first emphasis in a series
        ArcEmphasis(MscArcType t, const char *s, file_line_range sl,
                    const char *d, file_line_range dl, Msc *msc);
        ~ArcEmphasis()
            {if (follow.size()>0 && *follow.begin()==this) follow.pop_front(); delete emphasis;}
        ArcEmphasis* SetPipe();
        ArcEmphasis* AddArcList(ArcList*l);
        bool AddAttribute(const Attribute &);
        ArcEmphasis* ChangeStyleForFollow(ArcEmphasis* =NULL);
        ArcEmphasis* AddFollow(ArcEmphasis*f);
        string Print(int ident=0) const;
        virtual void PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level);
        virtual void Width(EntityDistanceMap &distances);
                void WidthPipe(EntityDistanceMap &distances);
        virtual double DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker);
                void DrawPipe(bool topSideFill, bool topSideLine, bool backSide, bool text);
        virtual void PostHeightProcess(void);
};

class ArcParallel : public ArcBase
{
    protected:
        PtrList<ArcList> blocks;
    public:
        ArcParallel(Msc *msc) :
            ArcBase(MSC_ARC_PARALLEL, msc) {}
        ArcParallel* AddArcList(ArcList*l)
            {blocks.push_back(l); return this;}
        bool AddAttribute(const Attribute &) {return false;}
        string Print(int ident=0) const;
        virtual void PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level);
        virtual void Width(EntityDistanceMap &distances);
        virtual double DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker);
        virtual void PostHeightProcess(void);
};


class ArcDivider : public ArcLabelled
{
    protected:
        const bool nudge;
	      bool wide;
    public:
        ArcDivider(MscArcType t, Msc *msc);
        bool AddAttribute(const Attribute &);
        virtual void Width(EntityDistanceMap &distances);
        virtual void PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level);
        virtual double DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker);
};


class ArcCommand : public ArcBase
{
    public:
        ArcCommand(MscArcType t, Msc *msc) :
            ArcBase(t, msc) {};
        bool AddAttribute(const Attribute &) {return false;}
        string Print(int ident=0) const;
};

class CommandEntity : public ArcCommand
{
    protected:
        EntityDefList *entities;
        bool full_heading;
    public:
        CommandEntity(EntityDefList *e, Msc *msc)
            : ArcCommand(MSC_COMMAND_ENTITY, msc) {entities=e; full_heading = (e==NULL);}
        ~CommandEntity()
            {delete entities;}
        string Print(int ident=0) const;
        void AppendToEntities(const EntityDefList &e);
        void Combine(CommandEntity *ce);

        virtual void PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level);
        virtual void Width(EntityDistanceMap &distances);
        virtual double DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker);
        virtual void PostHeightProcess(void);
};

class CommandNewpage : public ArcCommand
{
    public:
        CommandNewpage(Msc *msc)
            : ArcCommand(MSC_COMMAND_NEWPAGE, msc) {compress=false;}
        virtual double DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker);
};

class CommandNewBackground : public ArcCommand
{
    public:
        MscFillAttr fill;

        CommandNewBackground(Msc *msc, MscFillAttr f)
            : ArcCommand(MSC_COMMAND_NEWBACKGROUND, msc), fill(f) {}
        virtual double DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker);
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
        virtual double DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker);
};

class CommandMark : public ArcCommand
{
    public:
        string name;
        double offset;
        CommandMark(const char *m, file_line_range ml, Msc *msc);
        bool AddAttribute(const Attribute &);
        double DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker);
};

class CommandEmpty : public ArcCommand
{
        Label parsed_label;
    public:
        CommandEmpty(Msc *msc);
        virtual void Width(EntityDistanceMap &distances);
        double DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker);
};


#endif //ARCS_H