/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011,2012,2013,2014 Zoltan Turanyi
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
/** Name of the builtin chart.top marker */
#define MARKER_BUILTIN_CHART_TOP_STR "chart.top"
/** Name of the builtin chart.bottom marker */
#define MARKER_BUILTIN_CHART_BOTTOM_STR "chart.bottom"

/** The margin on left and right side, between LNote-LSide and RSide-RNote. 
 * In pos space, will be fed into XCoord().*/
#define MARGIN 0.1
/** The default size of the comment area in `pos` space if 
 * - we have comments
 * - we have no 'hspace comment' commands and
 * - all comments are wrapped */
#define DEFAULT_COMMENT_SIZE 1


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
    /** Entity pairs and the distance needed between them requested via the hspace command.
     * They are not necessarily neighbouring entities. */
    std::map<IPair, double, IPairComp> hspace_pairs;
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
    bool had_l_comment; ///<We had comments on the left side actually showing
    bool had_r_comment; ///<We had comments on the right side actually showing

    EntityDistanceMap() : had_l_comment(false), had_r_comment(false) {}

    /** Return the set of pairwise distances. Ordered by entity pair distance.*/
    const std::map<IPair, double, IPairComp> &GetPairs() const {return pairs;}
    /** Return the set of pairwise hspace distances. Ordered by entity pair distance.*/
    const std::map<IPair, double, IPairComp> &GetHSpacePairs() const {return hspace_pairs;}

    /** Insert a new distance requirement either between two entities or on one side of an entity.
     * @param [in] e1 Entity index.
     * @param [in] e2 Entity index or DISTANCE_LEFT or DISTANCE_RIGHT to represent one side of e1.
     * @param [in] d The distance to insert (pixel space).
     * @param [in] hspace True, if the distance was requested by a hspace command.
     * If such a distance requirement was inserted, the max of the old and new values are stored.*/
    void Insert(unsigned e1, int e2, double d, bool hspace=false);
    /** Get the distance stored between two entities or on the side of one.
     * Set the `hspace` param to true if you are interested in the distance requirement
     * specified exclusively by hspace commands. If set to false, all type of requirements
     * are included. */
    double Query(unsigned e1, int e2, bool hspace=false) const;
    /** Increase an existing distance requirement if already nonzero.
     * Returns true, if an increase has been made. */
    bool IncreaseIfNonZero(unsigned e1, int e2, double d, bool hspace=false);
    /** Insert a box side distance pair 
     * A box side distance is taken into account, when a box shall be drawn around it,
     * but not to space entities (since the box will add that requirement).
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

/** A collection storing side distance requirements after one marker.
 * We use this as part of class DistanceMapVertical.*/
class DistanceMapVerticalElement
{
    friend class DistanceMapVertical;
    const std::string          marker;  ///<The name of the marker after which we store. Empty for top of chart. If equal to MARKER_HERE_STR, 'element' and 'top' specifies the position.
    const Element * const      element; ///<If we are not aligned to a marker but to an element, this is the one.
    const bool                 top; 
    std::map<unsigned, double> left;    ///<Space requirements on the left side of entities
    std::map<unsigned, double> right;   ///<Space requirements on the left side of entities
    unsigned                   left_entity;  ///<The index of the leftmost entity touched
    unsigned                   right_entity; ///<The index of the rightmost entity touched
public:
    enum {NO_LEFT_ENTITY=10000, NO_RIGHT_ENTITY=0};
    explicit DistanceMapVerticalElement(const std::string &m = "") : marker(m), element(NULL), top(false), left_entity(NO_LEFT_ENTITY), right_entity(NO_RIGHT_ENTITY) {}
    explicit DistanceMapVerticalElement(const Element *e, bool t) : marker(MARKER_HERE_STR), element(e), top(t), left_entity(NO_LEFT_ENTITY), right_entity(NO_RIGHT_ENTITY) {}
    void Insert(unsigned e1, int e2, double d);  ///<Insert a distance requirement 
    double Query(unsigned e1, int e2); ///<Return a distance req
    void InsertEntity(unsigned e) { left_entity = std::min(left_entity, e); right_entity = std::max(right_entity, e); }
    unsigned QueryLeftEntity() const { return left_entity; }
    unsigned QueryRightEntity() const { return right_entity; }
    DistanceMapVerticalElement &operator +=(const DistanceMapVerticalElement &d);
};

/** A collection storing side distance requirements between markers 
 * Used to store distance requirements left and right of the entities
 * (in addition to storing them in an EntityDistanceMap), in order
 * to best place verticals. Note that we store the max requested 
 * distance on the side of each entity between markers.*/
class DistanceMapVertical
{
protected:
    std::list<DistanceMapVerticalElement> elements;
public:
    typedef std::list<DistanceMapVerticalElement>::iterator iterator;
    typedef std::list<DistanceMapVerticalElement>::const_iterator const_iterator;
    DistanceMapVertical() { elements.emplace_back(""); }
    /** Insert a side distance, which happened after the last marker*/
    void Insert(unsigned e1, int e2, double d) { elements.back().Insert(e1, e2, d); }
    void InsertEntity(unsigned e) { elements.back().InsertEntity(e); }
    void InsertEntity(EIterator e) { elements.back().InsertEntity((*e)->index); }
    void Insert(unsigned e1, int e2, double d, iterator from) { Insert(e1, e2, d, from, elements.end()); }
    void Insert(unsigned e1, int e2, double d, iterator from, iterator to);
    void InsertMarker(const std::string &m) { elements.emplace_back(m); }
    void InsertElementTop(const Element *e) { elements.emplace_back(e, true); }
    void InsertElementBottom(const Element *e) { elements.emplace_back(e, false); }
    iterator GetIterator(const string &m) { return std::find_if(elements.begin(), elements.end(), [&](const DistanceMapVerticalElement&e) {return e.marker==m; }); }
    const_iterator GetIterator(const string &m) const { return std::find_if(elements.begin(), elements.end(), [&](const DistanceMapVerticalElement&e) {return e.marker==m; }); };
    iterator GetIterator(const Element *elm, bool t) { return std::find_if(elements.begin(), elements.end(), [&](const DistanceMapVerticalElement&e) {return e.marker==MARKER_HERE_STR && e.element==elm && e.top==t; }); }
    const_iterator GetIterator(const Element *elm, bool t) const { return std::find_if(elements.begin(), elements.end(), [&](const DistanceMapVerticalElement&e) {return e.marker==MARKER_HERE_STR && e.element == elm && e.top==t; }); }
    iterator GetIteratorEnd() { return elements.end(); }
    const_iterator GetIteratorEnd() const { return elements.end(); }
    iterator GetIteratorLast() { return --elements.end(); }
    const_iterator GetIteratorLast() const { return --elements.end(); }
    DistanceMapVerticalElement Get(const_iterator m1, const_iterator m2) const;
};
/////////////////////////////////////////////////////////////////////

/** Holds information about one page break */
struct PageBreakData {
    double y;                    ///<The vertical position in chart space
    bool manual;                 ///<True if insierted by a newpage command, false if inserted via autopagination
    CommandEntity *autoHeading;  ///<A pointer to the automatically inserted heading command. NULL if none.
    double autoHeadingSize;      ///<The height of the automaically inserted heading. 0 if none.
    PageBreakData(double _y, bool m, CommandEntity *ce=NULL, double h=0) :
        y(_y), manual(m), autoHeading(ce), autoHeadingSize(h ? h : ce ? ce->GetFormalHeight() : 0) {}
};

/** Holds coordinate info about one arc or entity label. 
 * Used to generate 'lmap' output.*/
struct LabelInfo {
    /** Describes what type of labels we can have */
    enum LabelType {
        INVALID=0, ///<Not valid
        ENTITY,    ///<The label belongs to an entity header
        ARROW,     ///<The label belongs to an arrow 
        DIVIDER,   ///<The label belongs to a divider (title, etc.)
        BOX,       ///<The label belongs to a box with content
        EMPTYBOX,  ///<The label belongs to an empty box
        PIPE,      ///<The label belongs to a pipe (segment)
        VERTICAL,  ///<The label belongs to a vertical (brace, etc.)
        COMMENT,   ///<The label belongs to a comment (side or end)
        SYMBOL,    ///<The label of symbol commands
        NOTE,      ///<The label belongs to a floating note
    };
    static const char labelTypeChar[NOTE+2];
    LabelType type; ///<The type of the element of the label
    string    text; ///<The text of the label
    Block     coord;///<The bounding box of the label (all lines), in chart coordinates, ignoring pagination
    LabelInfo(LabelType t, const string &s, const Block &b) : 
        type(t), text(s), coord(b) {}
};

/**Helper struct to manage multiple list of arcs, sorted by 
 * a criteria so that always the smallest shall be taken an element from.*/
struct LayoutColumn
{
    /**If this list is put in as a nested parallel block, 
     * this is the parent list*/
    LayoutColumn * const parent;
    /**If we have nested parallel blocks under processing 
     * this is how many are not yet done.*/
    unsigned number_of_children;
    /** This is the actual list of arcs we contain.*/
    ArcList * const list;
    /** This points to the next arc to process in 'list'.
     * Equals to list->end() if we are done.*/
    ArcList::iterator arc;
    /** Tie-breaker: in case of everything equal, use the column number
     * This tells which column we were in our parallel block series.*/
    const unsigned column;
    /** This is where the next arc should be placed.*/
    double y;
    /**we will never shift elements higher than this runnning value
     * (due to compress). 
     * (any element marked with "parallel" will set this to its top)*/
    double y_upper_limit;
    /**true if the previous element was marked with parallel */
    bool previous_was_parallel;
    /**the lowest element's bottom (largest num value) we have seen in 'list'
     * so far. The lowest of these will be returned from LayoutParallelArcLists().
     * This is not always that of the last element so far, if the last 
     * element has been shifted up due to compress and its bottom is no longer
     * the lowest.*/
    double y_bottom_all;
    /**The bottom of the lowest element except elements marked as
    //"overlap". We use this when laying out the next element.*/
    double y_bottom;
    /**These contain all arc_covers (from all lists), without the mainlines, 
    //plus the mainlines of the arcs in their own column.*/
    AreaList covers;
    unsigned last_action;
    LayoutColumn(ArcList *a, unsigned c, double Y = 0, LayoutColumn *p = NULL, unsigned action=0, const AreaList *initial_cover=NULL) :
        parent(p), number_of_children(0), list(a), arc(list->begin()), column(c), y(Y),
        y_upper_limit(Y), previous_was_parallel(false), y_bottom_all(Y), y_bottom(Y),
        last_action(action) {if (initial_cover) covers = *initial_cover;}
    ///lists not done sorted earliest,
    // then the ones with no children are sorted earlier, 
    //then with lower y, 
    //then 'last_action' (the one we took an element longest time ago is smaller)
    //finally we tie break on the pointer value of 'list'
    bool operator <(const LayoutColumn &o) const {
        return list->end()!=arc && o.list->end()==o.arc ? true : list->end()==arc && o.list->end()!=o.arc ? false :
            number_of_children < o.number_of_children ? true : number_of_children > o.number_of_children ? false :
            y < o.y ? true : y > o.y ? false :
            last_action < o.last_action ? true : last_action > o.last_action ? false :
            column < o.column;
    }
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
        FileLineCol line; ///<Marker's location in the input file, invalid if a built-in marker 
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
    std::string                   file_url;        ///<Set by the file.url chart option
    std::string                   file_info;        ///<Set by the file.info chart option
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
    ShapeCollection               Shapes;          ///<A set of named shapes. Each one was created via 'defshape' command.
    std::map<string, Context>     Designs;         ///<A set of named designs. Each one was created via a `defdesign` command, except `plain`.
    std::map<string, MarkerData>  Markers;         ///<A set of named markers. Each one was defined via a 'mark' command.
    std::map<string, RefNameData> ReferenceNames;  ///<A set of named arcs. Each one was named via the 'refname' attribute.
    std::map<double, FillAttr>    Background;      ///<A map of background fill indexed by the position downward of which this fill is applicable. Each entry was created by a background.* chart option.
    std::string                   copyrightText;   ///<The copyright text we display at the bottom of each page.
    LineToArcMapType              AllArcs;         ///<A map of input file ranges to trackable Element objects (essentially arcs and entity headings)
    AreaList                      AllCovers;       ///<A set of arc contours with a pointer to the arcs. Used to identify the arc covering an XY coordinate.
    Contour                       HideELinesHere;  ///<A complex contour used as a mask when drawing entity lines.
    PBDataVector                  pageBreakData;   ///<Starting y pos and auto-heading info for each page break. pageBreakData[0].y is always 0. 
    std::list<LabelInfo>          labelData;       ///<Holds a catalogue of all labels with their coordinates. Filled by RegisterLabels().
    ISMap                         ismapData;       ///<Holds a list of link information from \\L escapes. Filled by CollectIsMapElements().
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
    unsigned noLabels;           ///<The number of labels (except entities)
    unsigned noOverflownLabels;  ///<The number of labels overflown
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
    double entityShapeHeight;///<The default height of an entity shape with size=normal.
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
    std::set<unsigned>    used_shapes;                ///<The id number of the shapes we used (for saving them in OLE objects)
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
    ArcArrow *CreateArcArrow(ArrowSegmentData data, const char*s, const FileLineColRange &sl,
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

    virtual string Print(int indent=0) const;
    /** Returns the current value of the hscale chart option*/
    double GetHScale() const {_ASSERT(Contexts.size() && Contexts.back().hscale.first); return Contexts.back().hscale.second;}
    /** Converts an x coordinate from pos coordinat space to pixel space. Always returns an integer */
    double XCoord(double pos) const {return floor(pos*130*(GetHScale()>0?GetHScale():1)+0.5);} //rounded
    /** Returns the x coordinate of the middle of an entity in pixel space, rounded*/
    double XCoord(EIterator i) const {return XCoord((*i)->pos);} //rounded

    void WidthArcList(Canvas &canvas, ArcList &arcs, EntityDistanceMap &distances, DistanceMapVertical &vdist);
    double LayoutArcList(Canvas &canvas, ArcList &arcs, AreaList *cover);
    double LayoutParallelArcLists(Canvas &canvas, std::list<LayoutColumn> &y, AreaList *cover);
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
    void PlaceWithMarkersArcList(Canvas &canvas, ArcList &arcs);
    void PlaceFloatingNotes(Canvas &canvas);

    void InvalidateNotesToThisTarget(const Element *target);
    void RemoveFromNotes(const CommandNote *note);

    /** Hides the entity lines in `area` */
    void HideEntityLines(const Contour &area) {HideELinesHere += area;}
    /** Hides the entity lines in `area` */
    void HideEntityLines(const Block &area) {HideELinesHere += Contour(area);}
    void CountLabel(bool overflown) {noLabels++; if (overflown) noOverflownLabels++;}
    void RegisterLabel(const Label &l, LabelInfo::LabelType type, const Block &b);
    /** Register a horizontally drawn label (with slant perhaps).
    * This is used to generate lmaps
    * if angle is nonzero, sx, dx, cx and y are interpreted in the rotated space.
    * @param [in] l The label to register.
    * @param [in] type The type of element the label belongs to.
    * @param [in] sx The left margin.
    * @param [in] dx The right margin.
    * @param [in] y The top of the label.
    * @param [in] cx If also specified, we center around it for centered lines,
    *                but taking care not to go ouside the margings.
    *                If the line is wider than `dx-sx` we will go outside
    *                as little as possible (thus we center around `(sx+dx)/2`.
    * @param [in] c The center of rotation in case of a slanted arrow label.
    * @param [in] angle The degrees of rotation in case of a slanted arrow label.*/
    void RegisterLabel(const Label &l, LabelInfo::LabelType type,
                       double sx, double dx, double y, double cx = -CONTOUR_INFINITY, const XY &c=XY(), double angle=0)
    { if (l.size()) labelData.emplace_back(type, l, l.Cover(sx, dx, y, cx).RotateAround(c, angle).GetBoundingBox());}
    /** Register a vertically drawn label.
    * This is used to generate lmaps
    * @param [in] l The label to register.
    * @param [in] type The type of element the label belongs to.
    * @param [in] s The left edge of the text (as the text reads)
    *               In the result this is the Left/Top/Bottom edge of text for side==END/LEFT/RIGHT.
    * @param [in] d The right edge of the text (as the text reads)
    *               In the result this is the Right/Bottom/Top edge of text for side==END/LEFT/RIGHT
    * @param [in] t The top edge of the text (as the text reads)
    *               In the result this is the Top/Right/Left edge of text for side==END/LEFT/RIGHT.
    * @param [in] side from which direction is the text read. For END it will be laid out horizontally.
    * @param [in] c If also specified, we center around it for centered lines,
    *                but taking care not to go ouside the margings.
    *                If the line is wider than `d-s` we will go outside
    *                as little as possible (thus we center around `(s+d)/2`.*/
    void RegisterLabel(const Label &l, LabelInfo::LabelType type,
                       double s, double d, double t, ESide side, double c = -CONTOUR_INFINITY)
    { if (l.size()) labelData.emplace_back(type, l, l.Cover(s, d, t, side, c).GetBoundingBox());}
    void PostPosProcessArcList(Canvas &canvas, ArcList &arcs);
    void RegisterCoverArcList(ArcList &arcs, EDrawPassType pass);

    void RegisterLabelArcList(ArcList &arcs) { for (auto pArc : arcs) pArc->RegisterLabels(); }
    void CollectIsMapElementsArcList(ArcList &arcs, Canvas &canvas) { for (auto pArc : arcs) pArc->CollectIsMapElements(canvas); }

    void CompleteParse(Canvas::EOutputType, bool avoidEmpty,
                       bool autoPaginate=false, bool addHeading=true, 
                       XY pageSize=XY(0,0), bool fitWidth=true, 
                       bool collectLinkInfo=false);

    void DrawEntityLines(Canvas &canvas, double y, double height, EIterator from, EIterator to);
    /** Draw all entity lines between vertical positions `y` and `y+height`*/
    void DrawEntityLines(Canvas &canvas, double y, double height)
         {DrawEntityLines(canvas, y, height, ActiveEntities.begin(), ActiveEntities.end());}

    void DrawArcList(Canvas &canvas, ArcList &arcs, Range yDrawing, EDrawPassType pass);
    void DrawChart(Canvas &canvas, Range yDrawing, bool pageBreaks);

    void DrawPageBreaks(Canvas &canvas);
    void DrawHeaderFooter(Canvas &canvas, unsigned page);
    void DrawComplete(Canvas &canvas, bool pageBreaks, unsigned page);

    bool DrawToFile(Canvas::EOutputType, const XY &scale, 
                    const string &fn, bool bPageBreak, bool ignore_pagebreaks, 
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
