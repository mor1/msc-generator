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
/** @file msc.h The declaration of the msc class describing a chart.
 * @ingroup libmscgen_files */
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

/** Name of the virtual entity representing a nonexistent entity and the left of the chart (of left comments), but notes may extend leftward. */
#define NONE_ENT_STR  "()"
/** Name of the virtual entity representing the line separating the left side comments from the chart body. */
#define LNOTE_ENT_STR "(leftside_note)"
/** Name of the virtual entity representing the left side of the chart body (some margin away from LNOTE_ENT_STR). */
#define LSIDE_ENT_STR "(leftside)"
/** Name of the virtual entity representing the right side of the chart body (some margin away from RNOTE_ENT_STR). */
#define RSIDE_ENT_STR "(rightside)"
/** Name of the virtual entity representing the line separating the right side comments from the chart body. */
#define RNOTE_ENT_STR "(rightside_note)"
/** Name of the virtual entity representing the ultimate right side of the chart (of left comments), but notes may extend rightward*/
#define END_ENT_STR "(end)"

/** Name of a virtual marker representing the current vertical location. Used in verticals when one end is not specified.*/
#define MARKER_HERE_STR "\""
/** Name of a virtual marker representing end of the last parallel block. Used in verticals when both ends are not specified. -- This feature is currently removed.*/
#define MARKER_PREV_PARALLEL_STR "\"\""

/** General gap between boxes, etc. In 'pos' space, will be fed into Msc::XCoord(). */
#define GAP 0.05
/** The margin on left and right side, if no autoscale. In pos space, will be fed into XCoord().*/
#define MARGIN 0.25
/** The margin on left and right side, if hscale is set to auto. In pos space, will be fed into XCoord().*/
#define MARGIN_HSCALE_AUTO 0.1

/////////////////////////////////////////////////////////////


/** A virtual entity index, used when we represent a distance required at the left side of an entity. */
#define DISTANCE_LEFT -1
/** A virtual entity index, used when we represent a distance required at the right side of an entity. */
#define DISTANCE_RIGHT -2

/** A pair of entity indexes for the purpose of later association with a required distance between them.*/
class IPair {
public:
    unsigned first; ///<Index of the left entity. 
    unsigned second;///<Index of the right entity.
    IPair(unsigned e1, unsigned e2) : first(std::min(e1,e2)), second(std::max(e1,e2)) {}
    /** Calculates the difference between the two index values*/
    unsigned Diff() const {return second-first;}
};

/** An ordering comparison object for IPair object.
 * IPairs holding two closer entities are deemed smaller. 
 * Of IPairs with entities of same distance, the pair leftmost is smaller.*/
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

/** Stores all type of horizontal distance requirements between entities for hscale = auto. 
 * One such object holds the collected horizontal distance requirements from a part of the
 * chart, such as the inside of a box or the whole chart. The collected requirements
 * in a sub-part (in a box) are processed (aggregated), used to compute box parameters
 * (in Width() of ArcBoxSeries, for example) and then copied from to the EntityDistanceMap 
 * for the enclosing part (another box for nested boxes or that of the whole chart).
 *
 * Entities are identified here by their index (an enumeration from left to right, with 
 * zero being the NoEntity (invalid) and 1 being the virtual entity representing the 
 * line between the leftside comments and the chart body. The largest index shall be
 * of the virtual entity representing the line between the right side comments and the 
 * chart body.
 *
 * We store several type of distance requirements
 * - Between pairs of entities (e.g., a label on an arrow between them)
 * - On the left or right side of entities 
 *   (e.g., entitiy headings, box/pipe sides, arrowheads expanding beyond the entity line)
 * - BoxSide distances are pairs of distances between two neighbouring entities.
 *   One part of the pair is a distance on the left side of an entity (say "e"); the other
 *   is a distance on the right side of the other entity (e+1). If nothing special happens,
 *   these two will be added and converted to a distance between e and e+1.
 *   However, if this distance requirement comes from inside a box and the box ends between
 *   e and e+1, we will have to space the side of the box appropriately and also make one 
 *   of the pair (*) wider by the thickness of the box line width and gaps (* which one will 
 *   depend on whether the right of left side of the box falls between e and e+1).
 *   This is useful for arc elements (especially entity commands) that cover multiple disjoint
 *   areas (e.g., the shown entity headings) some of which can fall into a box around them, some of them
 *   can fall outside.
 * - Entity activation size (how wide the activation is for each entity)
 */
class EntityDistanceMap
{
protected:
    /** Entity pairs and the distance needed between them.
     * They are not necessarily neighbouring entities. */
    std::map<IPair, double, IPairComp> pairs;
    //These two contain space requirements on the left and right side of an
    //entity. These are folded into "pairs" using the CombineLeftRightToPair_*
    //functions.
    std::map<unsigned, double> left; ///<Space requirements on the left side of entities
    std::map<unsigned, double> right;///<Space requirements on the left side of entities
    //This contain a list of distance pairs on the right side of an entity.
    //The "first" of the pair contains a distance on the right side of the entity,
    //while the 'second" of the pair contains a distance on the left side of the next
    //entity. They are used when a box has to be drawn between these two entities.
    //The CopyBoxSizeToPair() fn adds the "first" and "second" of each list element,
    //takes the maximum and inserts this value as a pair between the two neighbouring
    //entity.
    //QueryBoxSide finds the list element for an entity where the left or the right
    //(second or first, resp) is the biggest.
    /** Contains a list of side requirement pairs on the left side of an entity `e` 
     * and onthe right side of the next entity `e+1` */
    std::map<unsigned, std::list<std::pair<double, double>>> box_side;
public:
    /** Contains a set of entities that were active at any time while this map is used */
    std::set<unsigned> was_activated;
    double comment_l; ///<Maximum explicit user-requested left comment size
    double comment_r; ///<Maximum explicit user-requested right comment size
    bool had_l_comment; ///<We had comments on the left side actually showing
    bool had_r_comment; ///<We had comments on the right side actually showing

    EntityDistanceMap() : comment_l(0), comment_r(0), had_l_comment(false), had_r_comment(false) {}

    /** Return the set of pairwise distances. Ordered by entity pair distance.*/
    const std::map<IPair, double, IPairComp> &GetPairs() const {return pairs;}

    /** Insert a new distance requirement either between two entities or on one side of an entity.
     * @param [in] e1 Entity index.
     * @param [in] e2 Entity index or DISTANCE_LEFT or DISTANCE_RIGHT to represent one side of e1.
     * @param [in] d The distance to insert (pixel space).
     * If such a distance requirement was inserted, the max of the old and new values are stored.*/
    void Insert(unsigned e1, int e2, double d);
    /** Get the distance stored between two entities or on the side of one.*/
    double Query(unsigned e1, int e2) const;
    /** Insert a box side distance pair 
     * @param [in] e The entity we refer to.
     * @param [in] l The distance req on the left side of `e`
     * @param [in] r The distance req on the right side of `e+1`*/
    void InsertBoxSide(unsigned e, double l , double r);
    /** Returns the pair of distances for an entity where the left or the right value is greatest*/
    std::pair<double, double> QueryBoxSide(unsigned, bool left) const;
    /** Aggregate and move the values in box_side to pairs. Leaves box_side empty.
     * @param [in] gap Adds this much space to the sum of left and right distances.*/
    void CombineBoxSideToPair(double gap);
    /** Aggregates and moves "paired" side distances to `pair`. 
     * If for an entity we have registered distance requirements both right of and left of 
     * the next entity, we add these, add `gap`, insert a pairwise distance requirmenet between
     * the two neighbouring entity and remove the two side distance requirements. All other
     * ("unpaired") side distance requirements remain.*/
    void CombinePairedLeftRightToPair_Sum(double gap);
    /** Moves "unpaired" side distances to `pair`. 
     * If for an entity we have registered distance requirements only either right of it or left of 
     * the next entity, we add `gap` to this side distance, insert a pairwise distance requirmenet between
     * the two neighbouring entity and remove the the side distance requirement. All other
     * ("paired") side distance requirements remain.*/
    void CombineUnPairedLeftRightToPair(double gap);
    /** Aggregates and moves side distances to `pair`. Aggregation is done by taking the larger of
     * the distance left of an entity with the distance right of the next entity, plus `gap`.
     * In addition if the entity of the larger distance was active, we also add `act_size`.
     * At the end no side distances will remain.*/
    void CombineLeftRightToPair_Max(double gap, double act_size);
    EntityDistanceMap &operator +=(const EntityDistanceMap &d);
    string Print();
};

/////////////////////////////////////////////////////////////////////

/** Holds information about one page break */
struct PageBreakData {
    double y;                    ///<The vertical position in chart space
    bool manual;                 ///<True if insierted by a newpage command, false if inserted via autopagination
    CommandEntity *autoHeading;  ///<A pointer to the automatically inserted heading command. NULL if none.
    double autoHeadingSize;      ///<The height of the automaically inserted heading. 0 if none.
    PageBreakData(double _y, bool m, CommandEntity *ce=NULL, double h=0) :
        y(_y), manual(m), autoHeading(ce), autoHeadingSize(h ? h : ce ? ce->GetHeight() : 0) {}
};

/** The main class holding a chart 
  (This is a non movable, non copyable object, once it has arcs in it, due to the 
  cross-references between them.)

  # Coordinate Spaces

  - The pixel coordinate space (1 pixel = 1/72 inch for vector output) is the most natural. 
    Font heights are represented in pixels. This is also called "chart coordinate space" 
    when drawing, see the comment for Canvas.
  - The 'pos' coordinate space, which is only used on the horizontal axis to place entities.
    The 'pos' attribute of entities uses this. For historical purposes 1 unit of the 'pos' 
    coordinate space is 130 pixels. The conversion is done by Msc::XCoord().

  # Compress mechanism

    The compress mechanism (invoked by setting compress attribute to yes)
    changes the layout of Arcs.
    As normally they are placed in LayoutArcList() below the arc above.
    However, they can be placed higher if
    - its upper mainline is still below the lower mainline of the object above
    - it does not overlap in any way with the object(s) above.

    The object is never placed lower than the bottom of the arc above.
*/
class Msc {
public:
    /** Data we store about a marker */
    struct MarkerData {
        FileLineCol line; ///<Marker's location in the input file 
        double      y;    ///<Marker's vertical position in chart space
    };
    /** Data we store about a named arc, that can be referenced via @\r(<name>) */
    struct RefNameData {
        string      number_text; ///<The number of the arc in text format
        FileLineCol linenum;     ///<Position of the value of the "refname" attribute used to name the arc
        ArcBase    *arc;         ///<The arc or NULL if not shown (set in FinalizeLabels())
        RefNameData() : arc(NULL) {}
    };
    /** Maps ranges of the input file to trackable Elements*/
    typedef std::map<FileLineColRange, Element*, file_line_range_length_compare>
            LineToArcMapType;

    /** @name Members holding chart content (entities, arcs, definitions, etc.)
     * @{ */
    MscProgress                   Progress;        ///<Tracks the progress of parsing, compilation and drawing
    MscError                      Error;           ///<Collects error and warning messages
    unsigned                      current_file;    ///<The number of the file under parsing. Used when generating errors.*/

    EntityList                    AllEntities;     ///<A list of all entities defined (in no particular order)
    EntityList                    ActiveEntities;  ///<A list of the entities that are not hidden as part of a collapsed group entity
    Entity                       *NoEntity;        ///<A virtual entity representing no entity. (Used as a kind of NULL pointer or end() iterator.)
    Entity                       *LNote;           ///<A virtual entity representing the line separating the left side comments from the chart body.
    Entity                       *LSide;           ///<A virtual entity representing the left side of the chart body (some margin away from LNote)
    Entity                       *RSide;           ///<A virtual entity representing the right side of the chart body (some margin away from RNote)
    Entity                       *RNote;           ///<A virtual entity representing the line separating the right side comments from the chart body. 
    Entity                       *EndEntity;       ///<A virtual entity representing the very right side of the chart. Equals to total.x.till.
    EntityAppList                 AutoGenEntities; ///<A list of entity appearance objects, one for each implicitly defined entity. After parsing, they are appended to the first EntityCommand.
    ArcList                       Arcs;            ///<The list of all arcs in the chart (in order of definition). Notes are moved away from here in PostParseProcess().
    std::list<Context>            Contexts;        ///<A stack of Context objects used during parsing
    std::map<string, Context>     Designs;         ///<A set of named designs. Each one was created via a `defdesign` command, except `plain`.
    std::map<string, MarkerData>  Markers;         ///<A set of named markers. Each one was defined via a 'mark' command.
    std::map<string, RefNameData> ReferenceNames;  ///<A set of named arcs. Each one was named via the 'refname' attribute.
    std::map<double, FillAttr>    Background;      ///<A map of background fill indexed by the position downward of which this fill is applicable. Each entry was created by a background.* chart option.
    std::string                   copyrightText;   ///<The copyright text we display at the bottom of each page.
    LineToArcMapType              AllArcs;         ///<A map of input file ranges to trackable Element objects (essentially arcs and entity headings)
    AreaList                      AllCovers;       ///<A set of arc contours with a pointer to the arcs. Used to identify the arc covering an XY coordinate.
    Contour                       HideELinesHere;  ///<A complex contour used as a mask when drawing entity lines.
    PBDataVector                  pageBreakData;   ///<Starting y pos and auto-heading info for each page break. pageBreakData[0].y is always 0. 
    ArcList                       EndNotes;        ///<We move all endnotes here during PostParseProcessArcList(). We reappend them in PostParseProcess().
    CommandNoteList               Notes;           ///<All notes are moved here after PostParseProcess 
    PtrList<const Element>        NoteBlockers;    ///<Ptr to all elements that may block a floating note and which therefore should not overlap with them
    /** @} */
    
protected:
    /** @name Members holding calculated information on chart geometry
     * @{ */
    friend class CommandEntity;
    Block  total;                ///<Total bounding box of the chart (minus copyright), in chart space, not considering pagination.
    Block  drawing;              ///<The area where chart elements can be (total minus the comment lanes at the side)
    double copyrightTextHeight;  ///<The y size of the copyright text (calculated in CalculateWidthHeight())
    double headingSize;          ///<Y size of first heading row (collected during PostPosProcess())
    /** @} */

public:
    /** Returns the total bounding box of the chart (minus copyright), in chart space, not considering pagination.*/
    const Block &GetTotal() const {return total;}
    /** Returns the area where chart elements can be (total minus the comment lanes at the side)*/
    const Block &GetDrawing() const {return drawing;}
    /** Returns the y size of the copyright text */
    double GetCopyrightTextHeight() const {return copyrightTextHeight;}
    /** Returns the Y coordinate of the bottom of the top entity headings (for AutoSplit)*/
    double GetHeadingSize() {return headingSize;}

    /** @name Members defining various spacing values.
     * These should have an integer value for nice drawing on bitmaps.
     * @{ */    
    
    double chartTailGap;     ///<Gap at the bottom of the page for lengthening entity lines 
    double selfArrowYSize;   ///<Self Pointing arc Y size. 
    double headingVGapAbove; ///<Vertical gap above headings 
    double headingVGapBelow; ///<Vertical gap below headings 
    double boxVGapOutside;   ///<Vertical gap above and below boxes 
    double boxVGapInside;    ///<Vertical gap inside boxes 
    double arcVGapAbove;     ///<Vertical gap above and below arcs 
    double arcVGapBelow;     ///<Vertical gap above and below arcs 
    double discoVgap;        ///<How much extra space above and below a discontinuity line (...) 
    double titleVgap;        ///<How much extra space above and below a title 
    double subtitleVgap;     ///<How much extra space above and below a subtitle     
    double nudgeSize;        ///<Size of the `nudge` command
    double activeEntitySize; ///<The width of entity activation bars 
    double compressGap;      ///<Size of gap we keep between elements at compress. We expand by half of it 
    double hscaleAutoXGap;   ///<Size of horizontal gap between elements at hscale=auto     
    double sideNoteGap;      ///<Gap between the side comment line and the comments     
    double defWNoteWidth;    ///<The default width for word wrapping notes in pos "space"
    double trackFrameWidth;  ///<Width of the frames used for tracking boxes and pipes on screen 
    double trackExpandBy;    ///<How much do we expand tracking covers 
    /** @} */

    /** @name Parse Options 
     * @{ */
    bool pedantic;                   ///<If we require pre-defined entities. 
    bool ignore_designs;             ///<Ignore design changes
    bool simple_arc_parallel_layout; ///<If false, we use Msc::HeightArcLists() to lay out ArcParallel*/
    bool prepare_for_tracking;       ///<If true, all elements shall compute an 'area' member, we fill 'AllCovers' and 'AllArcs'. 
       
    EntityCollapseCatalog force_entity_collapse;      ///<These entities must be collapsed/expanded
    ArcSignatureCatalog   force_box_collapse;         ///<These boxes must be collapsed/expanded
    ArcSignatureCatalog   force_box_collapse_instead; ///<After parsing, this holds an updated version of the box collapse/expanded instructions (removing nonexistent boxes, etc)
    /** @} */

    Msc();
    virtual ~Msc();

    int SetDesign(bool full, const string &design, bool force, ArcBase **ret, const FileLineColRange &l = FileLineColRange(FileLineCol(0,0,0), FileLineCol(0,0,0)));
    string GetDesignNames(bool full) const;

    CommandEntity *CEForComments(const MscStyle &s, const FileLineColRange &l);
    ArcBase *AddAttribute(const Attribute&);
    bool AddDesignAttribute(const Attribute&);
    static void AttributeNames(Csh &csh, bool designOnly);
    static bool AttributeValues(const std::string attr, Csh &csh);

    /** Return the entity with the smaller (min=true) or larger (min=false) `pos` value*/
    EIterator EntityMinMaxByPos(EIterator i, EIterator j, bool min) const;
    /** Return the entity with the smaller `pos` value*/
    EIterator EntityMinByPos(EIterator i, EIterator j) const {return EntityMinMaxByPos(i, j, true);}
    /** Return the entity with the larger `pos` value*/
    EIterator EntityMaxByPos(EIterator i, EIterator j) const {return EntityMinMaxByPos(i, j, false);}
    /** Finds entity named `e` (mentioned at `l` inthe source file in AllEntities. 
     * If not found, it creates one. */
    EIterator FindAllocEntity(const char *e, const FileLineColRange &l);
    EIterator FindLeftRightDescendant(EIterator, bool left, bool stop_at_collapsed);
    EIterator FindActiveParentEntity(EIterator);
    EIterator FindWhoIsShowingInsteadOf(EIterator, bool left);
    string ListGroupedEntityChildren(EIterator ei);
    bool ErrorIfEntityGrouped(EIterator, FileLineCol l);
    bool IsMyParentEntity(const string &child, const string &parent);
    double GetEntityMaxPos() const;
    double GetEntityMaxPosExp() const;
    /** True if `e` is one of NoEntity, LSide, LNote, RSide, RNote or EndEntity.*/
    bool IsVirtualEntity(const Entity*e) const {return e==NoEntity || e==LNote || e==LSide || e==RSide || e==RNote || e==EndEntity;}
    /** Moves the contents of `a` to the end of the arc list of the chart*/
    void AddArcs(ArcList *a) {if (!a) return; Arcs.splice(Arcs.end(), *a); delete a;}
    ArcArrow *CreateArcArrow(EArcType t, const char*s, const FileLineColRange &sl,
                             const char*d, bool fw, const FileLineColRange &dl);
    /** Create a block arrow from an arrow. Returns null, emits an error if not possible*/
    ArcBigArrow *CreateArcBigArrow(const ArcBase *);
    /** Push a new context to the context stack. 
        If `empty` is true, an empty context is pushed (no styles, colors, etc.), if false, the top context is duplicated*/
    void PushContext(bool empty=false);
    /** Pop the top context. May return arcs to be appended as a result (CommandNumbering)*/
    ArcBase *PopContext();

    /** Parse a piece of input text as a new input file.*/
    void ParseText(const char *input, const char *filename);
    void PostParseProcessArcList(Canvas &canvas, bool hide, ArcList &arcs, bool resetiterators, EIterator &left,
                                 EIterator &right, Numbering &number, Element **note_target);
    void PostParseProcess(Canvas &canvas);
    /** Call FinalizeLabels() for each element in `arcs*/
    template <typename list> void FinalizeLabelsArcList(list &arcs, Canvas &canvas);

    EDirType GetTouchedEntitiesArcList(const ArcList &, EntityList &el, EDirType dir=MSC_DIR_INDETERMINATE) const;

    virtual string Print(int ident=0) const;
    /** Returns the current value of the hscale chart option*/
    double GetHScale() const {_ASSERT(Contexts.size() && Contexts.back().hscale.first); return Contexts.back().hscale.second;}
    /** Converts an x coordinate from pos coordinat space to pixel space. Always returns an integer */
    double XCoord(double pos) const {return floor(pos*130*(GetHScale()>0?GetHScale():1)+0.5);} //rounded
    /** Returns the x coordinate of the middle of an entity in pixel space, rounded*/
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

    /** Hides the entity lines in `area` */
    void HideEntityLines(const Contour &area) {HideELinesHere += area;}
    /** Hides the entity lines in `area` */
    void HideEntityLines(const Block &area) {HideELinesHere += Contour(area);}
    void PostPosProcessArcList(Canvas &canvas, ArcList &arcs);

    void CompleteParse(Canvas::EOutputType, bool avoidEmpty,
                       bool autoPaginate=false, bool addHeading=true, XY pageSize=XY(0,0), bool fitWidth=true);

    void DrawEntityLines(Canvas &canvas, double y, double height, EIterator from, EIterator to);
    /** Draw all entity lines between vertical positions `y` and `y+height`*/
    void DrawEntityLines(Canvas &canvas, double y, double height)
         {DrawEntityLines(canvas, y, height, ActiveEntities.begin(), ActiveEntities.end());}

    void DrawArcList(Canvas &canvas, ArcList &arcs, Range yDrawing, EDrawPassType pass);
    void DrawChart(Canvas &canvas, Range yDrawing, bool pageBreaks);

    void DrawPageBreaks(Canvas &canvas);
    void DrawHeaderFooter(Canvas &canvas, unsigned page);
    void DrawComplete(Canvas &canvas, bool pageBreaks, unsigned page);

    bool DrawToFile(Canvas::EOutputType, const std::vector<XY> &scale, 
                    const string &fn, bool bPageBreak,
                    const XY &pageSize=XY(0,0), const double margins[4]=NULL,
                    int ha=-1, int va=-1, bool generateErrors=false);
#ifdef CAIRO_HAS_WIN32_SURFACE
    HENHMETAFILE DrawToMetaFile(Canvas::EOutputType, unsigned page, bool bPageBreaks,
                                double fallback_image_resolution=-1, size_t *metafile_size=NULL,
                                Contour *fallback_images=NULL, bool generateErrors=false);
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

/** Global function used to parse a chart. Body defined in language.cc.*/
void MscParse(Msc &msc, const char *buff, unsigned len);

template <typename list>
void Msc::FinalizeLabelsArcList(list &arcs, Canvas &canvas)
{
	for (auto i : arcs) {
		i->FinalizeLabels(canvas);
		Progress.DoneItem(MscProgress::FINALIZE_LABELS, i->myProgressCategory);
	}
}

#endif

/* END OF FILE */
