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
#include "element.h"
#include "style.h"
#include "entity.h"
#include "mscdrawer.h"
#include "commands.h" //includes also arcs.h

using std::string;

//Name and index of the virtual entities
#define NONE_ENT_STR  "()"
#define LNOTE_ENT_STR "(leftside_note)"
#define LSIDE_ENT_STR "(leftside)"
#define RSIDE_ENT_STR "(rightside)"
#define RNOTE_ENT_STR "(rightside_note)"

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
class IPair {
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
    //This contains entity pairs and the distance needed between them
    //They are not necessarily neighbouring entities
    std::map<IPair, double, IPairComp> pairs;
    //These two contain space requirements on the left and right side of an
    //entity. These are folded into "pairs" using the CombineLeftRightToPair_*
    //functions.
    std::map<unsigned, double> left;
    std::map<unsigned, double> right;
    //This contain a list of distance pairs on the right side of an entity.
    //The "first" of the pair contains a distance on the right side of the entity,
    //while the 'second" of the pair contains a distance on the left side of the next
    //entity. They are used when a box has to be drawn between these two entities.
    //The CopyBoxSizeToPair() fn adds the "first" and "second" of each list element,
    //takes the maximum and inserts this value as a pair between the two neighbouring
    //entity.
    //QueryBoxSide finds the list element for an entity where the left or the right
    //(second or first, resp) is the biggest.
    std::map<unsigned, std::list<std::pair<double, double>>> box_side;
    //contains a set of entities that were active at any time while this map is used
    std::set<unsigned> was_activated;

    //Use DISTANCE_LEFT or DISTANCE_RIGHT as second param to insert into 'left' or 'right'
    //Use two nonnegative values (entity indexes) if you want to insert into 'pairs'
    void Insert(unsigned, int, double);
    double Query(unsigned, int) const;
    void InsertBoxSide(unsigned, double, double);
    std::pair<double, double> QueryBoxSide(unsigned, bool left) const;
    void CopyBoxSideToPair(double gap);
    void ClearBoxSize() {box_side.clear();}
    void CombineLeftRightToPair_Sum(double gap);
    void CombineLeftRightToPair_Max(double gap, double act_size);
    void CombineLeftRightToPair_Single(double gap);
    EntityDistanceMap &operator +=(const EntityDistanceMap &d);
    string Print();
};

/////////////////////////////////////////////////////////////////////

struct PageBreakData {
    double y;
    bool manual;
    CommandEntity *autoHeading;
    double autoHeadingSize;
    PageBreakData(double _y, bool m, CommandEntity *ce=NULL, double h=0) :
        y(_y), manual(m), autoHeading(ce), autoHeadingSize(h ? h : ce ? ce->GetHeight() : 0) {}
};


class Msc {
public:
    typedef std::pair<FileLineCol, double> MarkerType;
    struct RefType {
        string    number_text; // the number of the arc
        FileLineCol linenum;     //position of the value of the "refname" attr
        ArcBase  *arc;         // the arc or NULL if not shown (set in FinalizeLabels())
        RefType() : arc(NULL) {}
    };
    typedef std::map<FileLineColRange, Element*, file_line_range_length_compare>
            LineToArcMapType;
    struct ContourAttr {
        Contour     area;
        LineAttr line;
        FillAttr fill;
        ContourAttr() : fill(ColorType(0,0,0,0)) {} //transparent
        ContourAttr(const Contour &c, const LineAttr &l=LineAttr(), const FillAttr &f=FillAttr(ColorType(0,0,0,0))) : area(c), line(l), fill(f) {}
        ContourAttr(const Contour &c, const FillAttr &f) : area(c), line(LINE_NONE), fill(f) {}
    };

    MscProgress  Progress;
    MscError     Error;
    unsigned     current_file;  /* The number of the file under parsing, plus the error location */

    EntityList                    AllEntities;
    EntityList                    ActiveEntities;
    Entity                       *NoEntity, *LNote, *LSide, *RSide, *RNote;
    EntityAppList                 AutoGenEntities;
    ArcList                       Arcs;
    std::list<Context>            Contexts;
    std::map<string, Context>     Designs;
    std::map<string, MarkerType>  Markers;
    std::map<string, RefType>     ReferenceNames;
    std::map<double, FillAttr>    Background;
    std::string                   copyrightText;
    LineToArcMapType              AllArcs;
    AreaList                      AllCovers;
    Contour                       HideELinesHere;
    PBDataVector                  pageBreakData; /** The starting ypos of each page, one for each page. pageBreakData[0] is always 0. */

    CommandNoteList               Notes;            /** all floating notes after PostParseProcess */
    PtrList<const Element>        NoteBlockers;   /** Ptr to all elements that may block a floating note*/
    
    std::list<ContourAttr>        DebugContours;

protected:
    Block  total;                //Total size of the chart (minus copyright)
    Block  drawing;              //The area where chart elements can be (total minus the side note lanes)
public:
    const Block &GetTotal() const {return total;}
    const Block &GetDrawing() const {return drawing;}
    double comments_right_side;  //the right side of comments (or the drawing area if no rcomments)
    double copyrightTextHeight;  //Y size of the copyright text calculated
    double headingSize;          //Y size of first heading row collected during PostPosProcess(?)
    Range yDrawing;              //Restrict drawing to this range

    //These below should have an integer value for nice drawing
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
    /* How much extra space above and below a discontinuity line (...) */
    double discoVgap;
    /* How much extra space above and below a title */
    double titleVgap, subtitleVgap;
    /** Nudge size */
    double nudgeSize;
    /** The width of entity activation bars **/
    double activeEntitySize;
    /** Size of gap at compress. We expand by half of it */
    double compressGap;
    /** Size of gap at hscale=auto */
    double hscaleAutoXGap;
    /* Gap between the side note line and the comments */
    double sideNoteGap;
    /* Width of the frames used for tracking boxes on screen */
    double trackFrameWidth;
    /* How much do we expand tracking covers */
    double trackExpandBy;

    /* Parse Options */
    bool pedantic;   /* if we require pre-defined entities. */
    bool ignore_designs; /* ignore design changes */
    bool simple_arc_parallel_layout; /* if false, we use Msc::HeightArcLists() to lay out ArcParallel*/
    bool prepare_for_tracking; /* If true, all elements shall compute an 'area' member, we fill 'AllCovers' and 'AllArcs'.*/

    //Collapse/Expand instructions from and feedback to the GUI
    EntityCollapseCatalog force_entity_collapse; //these entities must be collapsed/expanded
    ArcSignatureCatalog   force_box_collapse;    //These boxes must be collapsed/expanded
    ArcSignatureCatalog   force_box_collapse_instead; //These should be kept from force_box_collapse

    Msc();
    ~Msc();

    void AddStandardDesigns(void);
    int SetDesign(bool full, const string &design, bool force, ArcBase **ret, const FileLineColRange &l = FileLineColRange(FileLineCol(0,0,0), FileLineCol(0,0,0)));
    string GetDesignNames(bool full) const;

    CommandEntity *CEForComments(const MscStyle &s, const FileLineColRange &l);
    ArcBase *AddAttribute(const Attribute&);
    bool AddDesignAttribute(const Attribute&);
    static void AttributeNames(Csh &csh, bool designOnly);
    static bool AttributeValues(const std::string attr, Csh &csh);

    EIterator EntityMinMaxByPos(EIterator i, EIterator j, bool min) const;
    EIterator EntityMinByPos(EIterator i, EIterator j) const {return EntityMinMaxByPos(i, j, true);}
    EIterator EntityMaxByPos(EIterator i, EIterator j) const {return EntityMinMaxByPos(i, j, false);}
    EIterator FindAllocEntity(const char *, FileLineColRange);
    EIterator FindLeftRightDescendant(EIterator, bool left, bool stop_at_collapsed);
    EIterator FindActiveParentEntity(EIterator);
    EIterator FindWhoIsShowingInsteadOf(EIterator, bool left);
    string ListGroupedEntityChildren(EIterator ei);
    bool ErrorIfEntityGrouped(EIterator, FileLineCol l);
    bool IsMyParentEntity(const string &children, const string &parent);
    double GetEntityMaxPos() const;
    double GetEntityMaxPosExp() const;
    bool IsVirtualEntity(const Entity*e) const {return e==NoEntity || e==LNote || e==LSide || e==RSide || e==RNote;}
    void AddArcs(ArcList *a);
    ArcArrow *CreateArcArrow(EArcType t, const char*s, FileLineColRange sl,
                             const char*d, bool fw, FileLineColRange dl);
    ArcBigArrow *CreateArcBigArrow(const ArcBase *);
    void PushContext(bool empty=false);
    ArcBase *PopContext();

    void ParseText(const char *input, const char *filename);

    void PostParseProcessArcList(Canvas &canvas, bool hide, ArcList &arcs, bool resetiterators, EIterator &left,
                                 EIterator &right, Numbering &number, Element **note_target);
    void PostParseProcess(Canvas &canvas);
    template <typename list> void FinalizeLabelsArcList(list &arcs, Canvas &canvas);

    EDirType GetTouchedEntitiesArcList(const ArcList &, EntityList &el, EDirType dir=MSC_DIR_INDETERMINATE) const;

    virtual string Print(int ident=0) const;
    double GetHScale() const {_ASSERT(Contexts.size() && Contexts.back().hscale.first); return Contexts.back().hscale.second;}
    double XCoord(double pos) const {return floor(pos*130*(GetHScale()>0?GetHScale():1)+0.5);} //rounded
    double XCoord(EIterator i) const {return XCoord((*i)->pos);} //rounded

    void WidthArcList(Canvas &canvas, ArcList &arcs, EntityDistanceMap &distances);
    double LayoutArcList(Canvas &canvas, ArcList &arcs, AreaList *cover);
    std::vector<double> LayoutArcLists(Canvas &canvas, std::vector<ArcList> &arcs, AreaList *cover);
    double PlaceListUnder(Canvas &canvas, ArcList &arcs, double start_y,
                          double top_y, const AreaList &area_top,
                          bool forceCompress=false, AreaList *ret_cover=NULL);
    void ShiftByArcList(ArcList &arcs, double y);
    void InsertAutoPageBreak(Canvas &canvas, ArcList &arcs, ArcList::iterator i,
                             double pageBreak, bool addHeading);
    double PageBreakArcList(Canvas &canvas, ArcList &arcs, double netPrevPageSize,
                            double pageBreak, bool &addCommandNewpage, bool addHeading,
                            bool canChangePBPos, bool dontshiftall);
    void CollectPageBreakArcList(ArcList &arcs);
    void AutoPaginate(Canvas &canvas, double pageSize, bool addHeading);
    void CalculateWidthHeight(Canvas &canvas,
                              bool autoPaginate, bool addHeading, XY pageSize, bool fitWidth);
    void PlaceWithMarkersArcList(Canvas &canvas, ArcList &arcs, double autoMarker);
    void PlaceFloatingNotes(Canvas &canvas);

    void InvalidateNotesToThisTarget(const Element *target);
    void RemoveFromNotes(const CommandNote *note);

    void HideEntityLines(const Contour &area) {HideELinesHere += area;}
    void HideEntityLines(const Block &area) {HideELinesHere += Contour(area);}
    void PostPosProcessArcList(Canvas &canvas, ArcList &arcs);

    void CompleteParse(Canvas::EOutputType, bool avoidEmpty,
                       bool autoPaginate=false, bool addHeading=true, XY pageSize=XY(0,0), bool fitWidth=true);

    void DrawEntityLines(Canvas &canvas, double y, double height, EIterator from, EIterator to);
    void DrawEntityLines(Canvas &canvas, double y, double height)
         {DrawEntityLines(canvas, y, height, ActiveEntities.begin(), ActiveEntities.end());}

    void DrawArcList(Canvas &canvas, ArcList &arcs, EDrawPassType pass);
    void DrawChart(Canvas &canvas, bool pageBreaks);

    void DrawCopyrightTextAndAutoHeading(Canvas &canvas, unsigned page=0);
    void DrawPageBreaks(Canvas &canvas);
    void DrawHeaderFooter(Canvas &canvas, unsigned page);
    void DrawComplete(Canvas &canvas, bool pageBreaks, unsigned page);

    bool DrawToFile(Canvas::EOutputType, const std::vector<XY> &scale, const string &fn, bool bPageBreaks,
                    const XY &pageSize=XY(0,0), const double margins[4]=NULL,
                    int ha=-1, int va=-1, bool generateErrors=false);
#ifdef CAIRO_HAS_WIN32_SURFACE
    HENHMETAFILE DrawToMetaFile(Canvas::EOutputType, unsigned page, bool bPageBreaks,
                                double fallback_image_resolution=-1, size_t *metafile_size=NULL,
                                Contour *fallback_images=NULL, bool generateErrors=false);
    /** Returns the size of the metafile or zero at error.*/
    size_t DrawToDC(Canvas::EOutputType ot, HDC hdc, const XY &scale,
                  unsigned page, bool bPageBreaks,
                  double fallback_image_resolution=-1,
                  bool generateErrors=false);
#endif
    cairo_surface_t *DrawToRecordingSurface(Canvas::EOutputType, bool bPageBreaks,
                                            bool generateErrors=false);
    cairo_surface_t *ReDrawOnePage(cairo_surface_t *full, unsigned page,
                                   bool generateErrors=false);
};

void MscParse(Msc &msc, const char *buff, unsigned len);

template <typename list>
void Msc::FinalizeLabelsArcList(list &arcs, Canvas &canvas)
{
	for (auto i=arcs.begin(); i!=arcs.end(); i++) {
		(*i)->FinalizeLabels(canvas);
		Progress.DoneItem(MscProgress::FINALIZE_LABELS, (*i)->myProgressCategory);
	}
}

#endif

/* END OF FILE */
