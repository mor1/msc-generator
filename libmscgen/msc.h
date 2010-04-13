#if !defined(MSC_H)
#define MSC_H

#include <cassert>
#include <climits>
#include <string>
#include <cstring>
#include <sstream>
#include <list>
#include <map>
#include <set>
#include <vector>
#include <stack>
#include "error.h"
#include "color.h"
#include "attribute.h"
#include "arrowhead.h"
#include "stringparse.h"

using std::string;

#define NONE_ENT_STR  "()"
#define LSIDE_ENT_STR "(leftside)"
#define RSIDE_ENT_STR "(rightside)"

#define MARKER_HERE_STR "\""
#define MARKER_PREV_PARALLEL_STR "\"\""

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

    MSC_ARC_PARALLEL,   /* not visible, just container for multiple parallel flows */

    MSC_ARC_DISCO,      /* ... Discontinuity in time line */
    MSC_ARC_DIVIDER,    /* --- Divider */
    MSC_ARC_VSPACE,     /* no arc, just space (maybe with label) */

    MSC_COMMAND_HEADING,
    MSC_COMMAND_NUDGE,
    MSC_COMMAND_ENTITY,
    MSC_COMMAND_NEWPAGE,
    MSC_COMMAND_NEWBACKGROUND,
    MSC_COMMAND_MARK
}
MscArcType;

//General gap between boxes, etc. In pos space, will be fed into XCoord().
#define GAP 0.05
//The margin on left and right side. In pos space, will be fed into XCoord().
#define MARGIN 0.25
#define MARGIN_HSCALE_AUTO 0.1

/////////////////////////////////////////////////////////////

//Structs for compress mechanism
/* Compress mechanism changes Height behaviour of Arcs.
 * yPos input parameter remains unchanged meaning: the total bottom of the item
 * above. However, the object can be placed higher if
 *  -its upper mainline is still below the lower mainline of the object above
 *  -it does not overlap in any way with the object above.
 * The object is never placed lower than yPos input parameter.
 * (In case of overlap a warning may be thrown.)
 * Return value will not be full object height, but rather the difference between
 * the placed objects bottomline and the original yPos. So next yPos can be
 * still calculated by adding the return value to a running y counter.
 *
 * As side effect Height() for ArcBase descendants
 *  - adds its covering blocks to Plain_Msc::cover
 *  - advances bottom_mainline to its own mainline.
 */


struct Geometry
{
    std::set<Block> cover;
    Range      mainline;
    Geometry() {mainline.from=INT_MAX; mainline.till=0;}
    Geometry &operator+=(const Geometry &g) {
        cover.insert(g.cover.begin(), g.cover.end());
        mainline.Extend(g.mainline);
        return *this;
    }
    bool Overlaps(const Geometry &g, double gap=0) const;
    void Draw(MscDrawer *chart, MscLineAttr line=MscLineAttr(MscColorType(255,0,0))) const {
        for(std::set<Block>::const_iterator i=cover.begin(); i!=cover.end(); i++)
            chart->rectangle(i->UpperLeft(), i->LowerRight(), line);
        //chart->line((XY){0, mainline.from}, (XY){chart->totalWidth, mainline.from},MscColorType(255,0,0),LINE_SOLID,1);
        //chart->line((XY){0, mainline.till}, (XY){chart->totalWidth, mainline.till},MscColorType(255,0,0),LINE_DASHED,1);
    }
    string Print() const {
        string s;
        for(std::set<Block>::const_iterator i=cover.begin(); i!=cover.end(); i++)
            s << "(" << i->x.from << "-" <<i->x.till
                << "," << i->y.from << "-" <<i->y.till << ");  ";
        return s;
    }
};

//Types for hscale=auto mechanism
class IPair  {
public:
    //If both first>0 and second>0 then the ipair represents a pair of entities (via indexes)
	//first can be< in that case -1:to the left, -2:to the right of the entity represented by second
	//In the latter case Diff() is erroneous.
    unsigned first, second;
    IPair(unsigned e1, unsigned e2) : first(std::min(e1,e2)), second(std::max(e1,e2)) {}
	unsigned Diff() const {return second-first;}
};

class IPairComp {
public:
    bool operator()(const IPair ei1, const IPair ei2) const{
        if (ei1.Diff()==ei2.Diff()) {
            if (ei1.first == ei2.first)
                return ei1.second < ei2.second;
            else return ei1.first < ei2.first;
        }
        return ei1.Diff()<ei2.Diff();
    }
};

//Used by the hscale=auto mechanism
#define DISTANCE_LEFT -1
#define DISTANCE_RIGHT -2

class EntityDistanceMap
{
public:
    std::map<IPair, double, IPairComp> pairs;
    std::map<unsigned, double> left;
    std::map<unsigned, double> right;
    void Insert(unsigned, int, double);
    double Query(unsigned, int);
    void CombineLeftRightToPair_Sum(double gap);
    void CombineLeftRightToPair_Max(double gap);
    void CombineLeftRightToPair_Single(double gap);
    EntityDistanceMap &operator +=(const EntityDistanceMap &d);
    string Print();
};

/////////////////////////////////////////////////////////////

struct MscStyle
{
    MscLineAttr line, vline;
    MscFillAttr fill;
    MscShadowAttr shadow;
    ArrowHead arrow;
    StringFormat text;
    std::pair<bool, unsigned char> solid;
    std::pair<bool, bool> numbering;
    std::pair<bool, bool> compress;

    StyleType type;

    bool f_line, f_vline, f_fill, f_shadow, f_text, f_arrow, f_solid, f_numbering, f_compress;

    MscStyle(StyleType tt=STYLE_STYLE); //Has all the components, but is empty
    MscStyle(StyleType tt, bool a, bool t, bool l, bool f, bool s, bool vl, bool so, bool nu, bool co);
    void Empty();
    MscStyle &operator +=(const MscStyle &toadd);
    bool AddAttribute(const Attribute &a, Msc *m);
    std::string Print(int ident = 0) const;
};

class StyleSet : public std::map<std::string, MscStyle>
{
public:
    bool numbering;
    bool compress;

    MscStyle defaultStyle;
    StyleSet() : numbering(false), compress(false) {} //def style is empty
    StyleSet(const MscStyle &a, bool num, bool comp) :
        defaultStyle(a), numbering(num), compress(comp) {};
    const MscStyle &GetStyle(const string&) const;
};

class Design
{
public:
    StyleSet styles;
    ColorSet colors;
    double hscale;
    Design() {Reset();}
    void Reset();
};

/////////////////////////////////////////////////////////////

class EntityStatus
{
public:
    MscLineAttr  line;
    bool         status;
    EntityStatus() {}
    EntityStatus(const MscLineAttr &l, bool s) : status(s) {line=l;}
    bool operator == (const EntityStatus &a)
        {return line==a.line && status==a.status;}
};

class EntityStatusMap
{
    protected:
        std::map<double, EntityStatus> storage;
        EntityStatus defaultStatus;
    public:
        void SetDefaultStatus(EntityStatus def)
            {defaultStatus=def;}
        void Set(double pos, EntityStatus status)
            {storage[pos] = status;}
        void SetRange(Range pos, EntityStatus status);
        void HideRange(Range pos)
            {SetRange(pos, EntityStatus(MscLineAttr(LINE_NONE, MscColorType(), 0, 0), true));}
        EntityStatus Get(double pos) const;
        double Till(double pos) const;
        string Print(int ident = 0);

};

class Msc;

class Entity
{
protected:
    Msc          *chart;
public:
    const string  name;        // Name of Entity as referenced in src file
    const string  label;       // Label of the entity (==name if none)
    double        pos;         // 0 for the 1st, 1 for 2nd, etc. 1.5 for one in-between
    unsigned      index;       // counts entities left to right
    MscStyle      style;
    EntityStatusMap  status;   // contains vertical line status & type & color

    Entity(const string &n, const string &l, double p, Msc *msc);
    virtual ~Entity() {};
    string Print(int ident = 0) const;
    double DrawHeight(double yPos, Geometry &g, bool draw, bool final);
    double Width();
};

class EntityList : public PtrList<Entity>
{
    public:
        explicit EntityList(bool r=true) :PtrList<Entity>(r) {}
        //Returns ::begin() if not found, assuming NoEntity is there
        PtrList<Entity>::iterator Find_by_Name(const string &name)
            {PtrList<Entity>::iterator i = PtrList<Entity>::begin();
             while (i!=PtrList<Entity>::end()) {
                if ((*i)->name == name) return i;
                i++;}
             return PtrList<Entity>::begin();}
        void SortByPos(void);
};
typedef EntityList::iterator EIterator;


template <class T1, class T2, class T3> struct triplet
{
  typedef T1 first_type;
  typedef T2 second_type;
  typedef T3 third_type;

  T1 first;
  T2 second;
  T3 third;
//  triplet() : first(T1()), second(T2(), third(T3()) {}
//  triplet(const T1& x, const T2& y, const T3& z) : first(x), second(y), third(z) {}
//  template <class U, class V, class W>
//    triplet (const triplet<U,V,W> &p) : first(p.first), second(p.second), third(p.third) { }
};

/* Class allocated during parse */
class EntityDef
{
    public:
        //Store attribute values added in an entity command subsequent to declaration
        const string                   name;
        triplet<bool,string,file_line> label;
        triplet<bool,double,file_line> pos;
        triplet<bool,string,file_line> rel;
        std::pair<bool,bool>           show;
        MscStyle                       style;

        explicit EntityDef(const char *s, Msc* chart);

        virtual EntityDef* AddAttributeList(AttributeList *, Msc *msc);
        virtual bool AddAttribute(const Attribute&, Msc *msc);
        virtual string Print(int ident=0) const;
};

typedef PtrList<EntityDef> EntityDefList;

/////////////////////////////////////////////////////////////////////
class ArcBase
{
    protected:
        Msc *chart;
        double yPos;
        EIterator MinMaxByPos(EIterator i, EIterator value, bool min);

    public:
              bool       valid;  /* If false, then construction failed, arc does not exist */
        const MscArcType type;
        const file_line  linenum;
              bool       at_top_level; /* if at top level by PostParseProcess() */
              bool       compress;     /* if compress mechanism is on for this arc */

        ArcBase(MscArcType t, long unsigned l, Msc *msc);
        virtual ~ArcBase() {};

        virtual ArcBase* AddAttributeList(AttributeList *);
        virtual bool AddAttribute(const Attribute &);
        virtual string PrintType(void) const;
        virtual string Print(int ident = 0) const = 0;
        virtual void PostParseProcess(EIterator &left, EIterator &right, int &number, bool top_level) {}

        /* width fills in distances for hscale=auto mechanism */
        virtual void Width(EntityDistanceMap &distances) {}
        /* Height should be possible to call any number of times with final=false,
         * but only once with final=true
         * Then call again with draw=true*/
        virtual double DrawHeight(double yPos, Geometry &g, bool draw, bool final, double autoMarker) =0;
        /* Then PostHeightProcess goes through the tree once more for
         * drawing warnings that need height. */
        virtual void PostHeightProcess(void) {};
};

typedef PtrList<ArcBase> ArcList;

class ArcLabelled : public ArcBase
{
    protected:
        string          label;
        Label           parsed_label;
        int             numbering; //-999:Yes, -1000: no, >=0 concrete num.
        MscStyle        style; //numbering and compress fields of style are not used. The Arc member fields are used instead.
    public:
        ArcLabelled(MscArcType t, long unsigned l, Msc *msc, const MscStyle &);
        ArcBase* AddAttributeList(AttributeList *);
        bool AddAttribute(const Attribute &);
        string Print(int ident=0) const;
        virtual void PostParseProcess(EIterator &left, EIterator &right, int &number, bool top_level);
};

class ArcArrow : public ArcLabelled
{
    public:
        ArcArrow(MscArcType t, long unsigned l, Msc *msc, const MscStyle &s) :
            ArcLabelled(t, l, msc, s) {}
        virtual ArcArrow *AddSegment(const char *m, bool forward, long unsigned l) = 0;
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
        ArcSelfArrow(MscArcType t, const char *s, long unsigned l,
                     Msc *msc, const MscStyle &, double ys);
        virtual ArcArrow *AddSegment(const char *m, bool forward, long unsigned l);
        string Print(int ident=0) const;
        virtual void PostParseProcess(EIterator &left, EIterator &right, int &number, bool top_level);
        virtual void Width(EntityDistanceMap &distances);
        virtual double DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker);
        virtual void PostHeightProcess(void);
};


class ArcDirArrow : public ArcArrow
{
    protected:
        EIterator       src, dst;
        std::vector<EIterator> middle;
		bool modifyFirstLineSpacing;
    public:
        ArcDirArrow(MscArcType t, const char *s, const char *d, long unsigned l,
                 Msc *msc, const MscStyle &);
        virtual ArcArrow *AddSegment(const char *m, bool forward, long unsigned l);
        string Print(int ident=0) const;
        virtual void PostParseProcess(EIterator &left, EIterator &right, int &number, bool top_level);
        virtual void Width(EntityDistanceMap &distances);
        virtual double DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker);
        virtual void PostHeightProcess(void);
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
        virtual void PostParseProcess(EIterator &left, EIterator &right, int &number, bool top_level);
        virtual void Width(EntityDistanceMap &distances);
        virtual double DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker);
        virtual void PostHeightProcess(void);
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
    VertXPos(long unsigned l, Msc&m, const char *e1, postype p=POS_AT, const char *e2=NULL);
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
                         VertXPos *p, long unsigned l, Msc *msc);
        ArcArrow *AddSegment(const char *m, bool forward, long unsigned l);
        bool AddAttribute(const Attribute &);
        void PostParseProcess(EIterator &left, EIterator &right, int &number, bool top_level);
        virtual void Width(EntityDistanceMap &distances);
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
    public:
        //Constructor to construct the first emphasis in a series
        ArcEmphasis(MscArcType t, const char *s, const char *d, long unsigned l, Msc *msc);
        ~ArcEmphasis()
            {if (*follow.begin()==this) follow.pop_front(); delete emphasis;}
        ArcEmphasis* SetPipe();
        ArcEmphasis* AddArcList(ArcList*l);
        bool AddAttribute(const Attribute &);
        ArcEmphasis* EmptyStyle() {style.Empty(); style.line.type.first = true; return this;}
        ArcEmphasis* AddFollow(ArcEmphasis*f);
        string Print(int ident=0) const;
        virtual void PostParseProcess(EIterator &left, EIterator &right, int &number, bool top_level);
        virtual void Width(EntityDistanceMap &distances);
        virtual double DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker);
        virtual void PostHeightProcess(void);
};

class ArcParallel : public ArcBase
{
    protected:
        PtrList<ArcList> parallel;
    public:
        ArcParallel(long unsigned l, Msc *msc) :
            ArcBase(MSC_ARC_PARALLEL, l, msc) {}
        ArcParallel* AddArcList(ArcList*l)
            {parallel.push_back(l); return this;}
        bool AddAttribute(const Attribute &) {return false;}
        string Print(int ident=0) const;
        virtual void PostParseProcess(EIterator &left, EIterator &right, int &number, bool top_level);
        virtual void Width(EntityDistanceMap &distances);
        virtual double DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker);
        virtual void PostHeightProcess(void);
};


class ArcDivider : public ArcLabelled
{
    protected:
        const bool  nudge;
		bool wide;
    public:
        ArcDivider(MscArcType t, long unsigned l, Msc *msc);
        bool AddAttribute(const Attribute &);
        virtual void Width(EntityDistanceMap &distances);
        virtual void PostParseProcess(EIterator &left, EIterator &right, int &number, bool top_level);
        virtual double DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker);
};


class ArcCommand : public ArcBase
{
    public:
        ArcCommand(MscArcType t, long unsigned l, Msc *msc) :
            ArcBase(t, l, msc) {};
        bool AddAttribute(const Attribute &) {return false;}
        string Print(int ident=0) const;
};

class CommandEntity : public ArcCommand
{
    protected:
        EntityDefList *entities;
        bool full_heading;
    public:
        CommandEntity(EntityDefList *e, long unsigned l, Msc *msc)
            : ArcCommand(MSC_COMMAND_ENTITY, l, msc) {entities=e; full_heading = e==NULL;}
        ~CommandEntity()
            {delete entities;}
        string Print(int ident=0) const;
        void AppendToEntities(const EntityDefList &e);
        void Combine(CommandEntity *ce);

        virtual void PostParseProcess(EIterator &left, EIterator &right, int &number, bool top_level);
        virtual void Width(EntityDistanceMap &distances);
        virtual double DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker);
};

class CommandNewpage : public ArcCommand
{
    public:
        CommandNewpage(long unsigned l, Msc *msc)
            : ArcCommand(MSC_COMMAND_NEWPAGE, l, msc) {compress=false;}
        virtual double DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker);
};

class CommandNewBackground : public ArcCommand
{
    public:
        MscFillAttr fill;

        CommandNewBackground(long unsigned l, Msc *msc, MscFillAttr f)
            : ArcCommand(MSC_COMMAND_NEWBACKGROUND, l, msc), fill(f) {}
        virtual double DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker);
};

class CommandMark : public ArcCommand
{
    public:
        string name;
        double offset;
        CommandMark(const char *m, long unsigned l, Msc *msc);
        bool AddAttribute(const Attribute &);
        double DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker);
};

/////////////////////////////////////////////////////////////////////

class Msc : public MscDrawer {
public:
    typedef std::pair<file_line, double> MarkerType;
    EntityList                    Entities;
    EIterator                     NoEntity;
    EntityDefList                 AutoGenEntities;
    double                        Entity_max_pos;
    ArcList                       Arcs;
    std::stack<StyleSet>          StyleSets;
    std::stack<ColorSet>          ColorSets;
    std::map<string, Design>      Designs;
    std::map<string, MarkerType>  Markers;
    std::map<double, MscFillAttr> Background;
	std::string                   copyrightText;

    /** Gap at the bottom of the page for lengthening entity lines */
    double chartTailGap;
    /** Self Pointing arc Y size. */
    double selfArrowYSize;
    /** Vertical gap above and below headings */
    double headingVGapAbove, headingVGapBelow;
    /** Vertical gap above and below emph boxes */
    double emphVGapOutside, emphVGapInside;
    /** Vertical gap above and below arcs */
    double arcVGapAbove, arcVGapBelow;
    /** Nudge size */
    double nudgeSize;
    /** Size of gap at compress */
    double compressXGap, compressYGap;
    /** Size of gap at hscale=auto */
    double hscaleAutoXGap;

    /* Parse Options */
    double       hscale;     /** Relative xsize, -1 is auto **/
    double       saved_hscale; /** save hscale during design definition */
    bool         pedantic;   /* if we require pre-defined entities. */
    bool         ignore_designs; /* ignore design changes */

    Msc();

    void AddStandardDesigns(void);
    bool SetDesign(const string &design, bool force);
    string GetDesigns() const;

    bool AddAttribute(const Attribute&);
    bool AddDesignAttribute(const Attribute&);

    EIterator FindAllocEntity(const char *, unsigned long, bool*validptr=NULL);
    void AddArcs(ArcList *a);
    ArcArrow *CreateArcArrow(MscArcType t, const char*s, const char*d, long unsigned l);
    ArcBigArrow *CreateArcBigArrow(const ArcBase *);
    void PushContext();
    void PopContext();

    void PostParseProcessArcList(ArcList &arcs, bool resetiterators,
                                         EIterator &left, EIterator &right,
                                         int &number, bool top_level);
    void PostParseProcess(void);
    virtual string Print(int ident=0) const;

    double XCoord(double pos) const
        {return pos*130*(hscale>0?hscale:1);}
    double FindCollision(const std::set<Block> &a, const  std::set<Block> &b,
                         double &CollisionYPos) const;

    void HideEntityLines(const std::set<Block> &blocks);

    void DrawEntityLines(double y, double height, EIterator from, EIterator to);
    void DrawEntityLines(double y, double height)
         {DrawEntityLines(y, height, Entities.begin(), Entities.end());}

    void WidthArcList(ArcList &arcs, EntityDistanceMap &distances);
    double DrawHeightArcList(ArcList &arcs, double y, Geometry &g,
                             bool draw, bool final, bool &prevCompress,
                             double autoMarker);

    void CalculateWidthHeight(void);
    void PostHeightProcess(void);
    void Draw(void);
	void DrawCopyrightText(int page=-1);

	void ParseText(const char *input, unsigned length, const char *filename);
    void CompleteParse(OutputType);
    void DrawToOutput(OutputType, const string &);
};

//Parser and lexer related stuff

#include "language.h"

typedef struct
{
    void            *yyscanner;
    char            *buf;
    int             pos;
    int             length;
} parse_parm;

#define YY_EXTRA_TYPE   parse_parm *

#ifndef HAVE_UNISTD_H
#define YY_NO_UNISTD_H
extern int isatty (int );
#endif


int     yylex(YYSTYPE *, void *);
int     yylex_init(void **);
int     yylex_destroy(void *);
void    yyset_extra(YY_EXTRA_TYPE, void *);
int     yyparse(Msc&, void *);
int     yyget_lineno(void* scanner);


void MscParse(Msc &msc, const char *buff, unsigned len);

#endif

/* END OF FILE */
