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
#include "trackable.h"
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

class Msc {
public:
    typedef std::pair<file_line, double> MarkerType;
    struct RefType {
        string    number_text; // the number of the arc
        file_line linenum;     //position of the value of the "refname" attr
        ArcBase  *arc;         // the arc or NULL if not shown (set in FinalizeLabels())
        RefType() : arc(NULL) {}
    };
    typedef std::map<file_line_range, TrackableElement*, file_line_range_length_compare>
            LineToArcMapType;

    MscError     Error;
    unsigned     current_file;  /* The number of the file under parsing, plus the error location */

    EntityList                    AllEntities;
    EntityList                    ActiveEntities;
    Entity                       *NoEntity, *LNote, *LSide, *RSide, *RNote;
    EntityDefList                 AutoGenEntities;
    ArcList                       Arcs;
    std::list<Context>            Contexts;
    std::map<string, Design>      Designs;
    std::map<string, MarkerType>  Markers;
    std::map<string, RefType>     ReferenceNames;
    std::map<double, MscFillAttr> Background;
    std::string                   copyrightText;
    LineToArcMapType              AllArcs;
    AreaList                      AllCovers;
    Contour                       HideELinesHere;
    std::vector<double>           yPageStart; /** The starting ypos of each page, one for each page. yPageStart[0] is always 0. */

    CommandNoteList               Notes;      /** all notes after PostParseProcess */
    PtrList<const Contour>        NoteMapImp; /** Ptr to the note_map of all elements */
    PtrList<const Contour>        NoteMapAll; /** Ptr to the draw_area of all elements */
    
    ArcBase                      *last_notable_arc;     //during parse: last arc inserted (the one notes attach to) or NULL if none
    bool                          last_note_is_on_left; //during post-parse: was th last non-float note on the left side

    XY     total;                //Total size of the chart (minus copyright)
    double copyrightTextHeight;  //Y size of the copyright text calculated
    double headingSize;          //Y size of first heading row collected during PostPosProcess(?)

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
    /** Nudge size */
    double nudgeSize;
    /** The width of entity activation bars **/
    double activeEntitySize;
    /** Size of gap at compress. We expand by half of it */
    double compressGap;
    /** Size of gap at hscale=auto */
    double hscaleAutoXGap;
    /* Width of the frames used for tracking boxes on screen */
    double trackFrameWidth;
    /* How much do we expand tracking covers */
    double trackExpandBy;

    /* Parse Options */
    double       hscale;     /** Relative xsize, -1 is auto **/
    double       saved_hscale; /** save hscale during design definition */
    bool         pedantic;   /* if we require pre-defined entities. */
    bool         ignore_designs; /* ignore design changes */

    //Collapse/Expand instructions from and feedback to the GUI
    EntityCollapseCatalog force_entity_collapse; //these entities must be collapsed/expanded
    ArcSignatureCatalog   force_box_collapse;    //These boxes must be collapsed/expanded
    ArcSignatureCatalog   force_box_collapse_instead; //These should be kept from force_box_collapse
    
    Msc();

    void AddStandardDesigns(void);
    bool SetDesign(const string &design, bool force);
    string GetDesigns() const;

    bool AddAttribute(const Attribute&);
    bool AddDesignAttribute(const Attribute&);
    static void AttributeNames(Csh &csh, bool designOnly);
    static bool AttributeValues(const std::string attr, Csh &csh);

    EIterator EntityMinMaxByPos(EIterator i, EIterator j, bool min) const;
    EIterator EntityMinByPos(EIterator i, EIterator j) const {return EntityMinMaxByPos(i, j, true);}
    EIterator EntityMaxByPos(EIterator i, EIterator j) const {return EntityMinMaxByPos(i, j, false);}
    EIterator FindAllocEntity(const char *, file_line_range);
    EIterator FindLeftRightDescendant(EIterator, bool left, bool stop_at_collapsed);
    EIterator FindActiveParentEntity(EIterator);
    EIterator FindWhoIsShowingInsteadOf(EIterator, bool left);
    string ListGroupedEntityChildren(EIterator ei);
    bool ErrorIfEntityGrouped(EIterator, file_line l);
    bool IsMyParentEntity(const string &children, const string &parent);
    double GetEntityMaxPos() const;
    double GetEntityMaxPosExp() const;
    bool IsVirtualEntity(const Entity*e) const {return e==NoEntity || e==LNote || e==LSide || e==RSide || e==RNote;}
    void AddArcs(ArcList *a);
    ArcArrow *CreateArcArrow(MscArcType t, const char*s, file_line_range sl,
                             const char*d, bool fw, file_line_range dl);
    ArcBigArrow *CreateArcBigArrow(const ArcBase *);
    void PushContext(bool empty=false);
    ArcBase *PopContext();

    void ParseText(const char *input, const char *filename);

    void PostParseProcessArcList(MscCanvas &canvas, bool hide, ArcList &arcs, bool resetiterators, EIterator &left,
                                 EIterator &right, Numbering &number, bool top_level);
    void PostParseProcess(MscCanvas &canvas);
    template <typename list> void FinalizeLabelsArcList(list &arcs, MscCanvas &canvas) {for (auto i=arcs.begin(); i!=arcs.end(); i++) (*i)->FinalizeLabels(canvas);}
    void FinalizeLabels(MscCanvas &canvas) {FinalizeLabelsArcList(Arcs, canvas); FinalizeLabelsArcList(Notes, canvas);}

    MscDirType GetTouchedEntitiesArcList(const ArcList &, EntityList &el, MscDirType dir=MSC_DIR_INDETERMINATE) const;

    virtual string Print(int ident=0) const;
    double XCoord(double pos) const {return floor(pos*130*(hscale>0?hscale:1)+0.5);} //rounded
    double XCoord(EIterator i) const {return XCoord((*i)->pos);} //rounded

    void WidthArcList(MscCanvas &canvas, ArcList &arcs, EntityDistanceMap &distances);
    double HeightArcList(MscCanvas &canvas, ArcList::iterator from, ArcList::iterator to, AreaList &cover, bool reflow);
    double PlaceListUnder(MscCanvas &canvas, ArcList::iterator from, ArcList::iterator to, double start_y,
                          double top_y, const AreaList &area_top, bool reflow,
                          bool forceCompress=false, AreaList *ret_cover=NULL);
    void ShiftByArcList(ArcList::iterator from, ArcList::iterator to, double y);
    void CalculateWidthHeight(MscCanvas &canvas);
    void PlaceNotes(MscCanvas &canvas);

    void HideEntityLines(const Contour &area) {HideELinesHere += area;}
    void HideEntityLines(const Block &area) {HideELinesHere += Contour(area);}
    void PostPosProcessArcList(MscCanvas &canvas, ArcList &arcs, double autoMarker);

    void CompleteParse(MscCanvas::OutputType, bool avoidEmpty);

    void DrawEntityLines(MscCanvas &canvas, double y, double height, EIterator from, EIterator to);
    void DrawEntityLines(MscCanvas &canvas, double y, double height)
         {DrawEntityLines(canvas, y, height, ActiveEntities.begin(), ActiveEntities.end());}

    template<typename list> void DrawArcList(MscCanvas &canvas, list &arcs, ArcBase::DrawPassType pass) {for (auto i = arcs.begin();i!=arcs.end(); i++) (*i)->Draw(canvas, pass);}
    void DrawArcs(MscCanvas &canvas, ArcBase::DrawPassType pass);
    void Draw(MscCanvas &canvas, bool pageBreaks);
    void DrawCopyrightText(MscCanvas &canvas, unsigned page=0);
    void DrawPageBreaks(MscCanvas &canvas);
    void DrawToOutput(MscCanvas::OutputType, const XY &scale, const string &fn, bool bPageBreaks);
};


void MscParse(Msc &msc, const char *buff, unsigned len);

#endif

/* END OF FILE */
