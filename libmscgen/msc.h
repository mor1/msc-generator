#if !defined(MSC_H)
#define MSC_H

#include <cassert>
#include <string>
#include <cstring>
#include <sstream>
#include <list>
#include <map>
#include <set>
#include <vector>
#include <stack>
#include "csh.h"
#include "trackable.h"
#include "style.h"
#include "entity.h"
#include "arcs.h"

using std::string;

#define NONE_ENT_STR  "()"
#define LSIDE_ENT_STR "(leftside)"
#define RSIDE_ENT_STR "(rightside)"

#define MARKER_HERE_STR "\""
#define MARKER_PREV_PARALLEL_STR "\"\""

//General gap between boxes, etc. In pos space, will be fed into XCoord().
#define GAP 0.05
//The margin on left and right side. In pos space, will be fed into XCoord().
#define MARGIN 0.25
#define MARGIN_HSCALE_AUTO 0.1

/////////////////////////////////////////////////////////////

/* Compress mechanism changes Height behaviour of Arcs.
 * y input parameter to DrawHeight remains unchanged meaning: the total bottom of the item
 * above. However, the object can be placed higher if
 *  -its upper mainline is still below the lower mainline of the object above
 *  -it does not overlap in any way with the object above.
 * The object is never placed lower than y input parameter.
 * (In case of overlap a warning may be thrown.)
 * Return value will not be full object height, but rather the difference between
 * the placed objects bottomline and the original y. So next y can be
 * still calculated by adding the return value to a running y counter.
 *
 * As side effect DrawHeight(draw==false) for ArcBase descendants
 *  - adds its covering blocks to input parameter Geometry
 *  - advances Geometry.mainline to its own mainline (if parallel flag is not set)
 */

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

/////////////////////////////////////////////////////////////////////

struct file_line_range_length_compare
{
    bool operator() (const file_line_range &a, const file_line_range &b) const {
        if (b.end.file - b.start.file == a.end.file - a.start.file) {
            if (b.end.line - b.start.line == a.end.line - a.start.line) {
                if (b.end.col - b.start.col == a.end.col - a.start.col)
                    return a.start<b.start;
                else return b.end.col - b.start.col > a.end.col - a.start.col;
            } else return b.end.line - b.start.line > a.end.line - a.start.line;
        } else return b.end.file - b.start.file > a.end.file - a.start.file;
    }
};

class Msc : public MscDrawer {
public:
    typedef std::pair<file_line, double> MarkerType;
	typedef std::map<file_line_range, TrackableElement*, file_line_range_length_compare> LineToArcMapType;
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
    LineToArcMapType              AllArcs;
    std::list<Block>              AllCovers;

    MscCshListType                CshList;
    std::set<string>              CshEntityNames;

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

    void AddCSH(CshPos&, MscColorSyntaxType);
    void AddCSH_AttrValue(CshPos&, const char *value, const char *name);
    void AddCSH_ColonString(CshPos& pos, const char *value, bool processComments);
    void AddCSH_AttrName(CshPos&, const char *name, MscColorSyntaxType);
    void AddCSH_EntityName(CshPos&pos, const char *name);
    void ParseForCSH(const char *input, unsigned len);

    void AddStandardDesigns(void);
    bool SetDesign(const string &design, bool force);
    string GetDesigns() const;

    bool AddAttribute(const Attribute&);
    bool AddDesignAttribute(const Attribute&);

    EIterator FindAllocEntity(const char *, file_line_range, bool*validptr=NULL);
    void AddArcs(ArcList *a);
    ArcArrow *CreateArcArrow(MscArcType t, const char*s, file_line_range sl,
                             const char*d, file_line_range dl);
    ArcBigArrow *CreateArcBigArrow(const ArcBase *);
    void PushContext(bool empty=false);
    void PopContext();

    void PostParseProcessArcList(ArcList &arcs, bool resetiterators,
                                         EIterator &left, EIterator &right,
                                         int &number, bool top_level);
    void PostParseProcess(void);
    virtual string Print(int ident=0) const;

    double XCoord(double pos) const
        {return pos*130*(hscale>0?hscale:1);}
    double FindCollision(const Geometry &a, const Geometry &b, double &CollisionYPos) const;
    double FindCollision(const Geometry &a, const Geometry &b) const
	    {double dummy; return FindCollision(a,b,dummy);}

    void HideEntityLines(const Geometry &geom);

    void DrawEntityLines(double y, double height, EIterator from, EIterator to);
    void DrawEntityLines(double y, double height)
         {DrawEntityLines(y, height, Entities.begin(), Entities.end());}

    void WidthArcList(ArcList &arcs, EntityDistanceMap &distances);
    double PlaceDrawListUnder(ArcList::iterator from, ArcList::iterator to,
                              double top_y, double low_y,
                              Geometry &g, Geometry &g_result,
                              bool draw, bool final, bool forceCompress=false);
    double DrawHeightArcList(ArcList::iterator from, ArcList::iterator to,
                             double y, Geometry &g,
                             bool draw, bool final, double autoMarker=-1);

    void CalculateWidthHeight(void);
    void PostHeightProcess(void);
    void Draw(bool pageBreaks);
    void DrawCopyrightText(int page=-1);
    void DrawPageBreaks();

    void ParseText(const char *input, const char *filename);
    void CompleteParse(OutputType, bool avoidEmpty);
    void DrawToOutput(OutputType, const string &);
};


//Parser and lexer related stuff
#ifndef HAVE_UNISTD_H
#define YY_NO_UNISTD_H
extern int isatty (int );
#endif

typedef struct
{
    void            *yyscanner;
    char            *buf;
    int             pos;
    int             length;
    Msc             *msc;
} parse_parm;

#define YY_EXTRA_TYPE   parse_parm *

//If we scan for color syntax highlight use this location
//yyerror is defined by bison, the other is defined for flex
#ifdef C_S_H_IS_COMPILED
#define YYLTYPE_IS_DECLARED
#define YYLTYPE CshPos
#endif

#include "language.h"

int     yylex(YYSTYPE *, YYLTYPE *, void *);
int     yylex_init(void **);
int     yylex_destroy(void *);
void    yyset_extra(YY_EXTRA_TYPE, void *);
int     yyparse(Msc&, void *);
int     yyget_lineno(void* scanner);

void MscParse(Msc &msc, const char *buff, unsigned len);
void CshParse(Msc &msc, const char *buff, unsigned len);

#endif

/* END OF FILE */
