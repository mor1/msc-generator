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

/** @file arcs.h The declaration of classes for arcs, boxes215, verticals.
 * @ingroup libmscgen_files */


#if !defined(ARCS_H)
#define ARCS_H

#include "color.h"
#include "style.h"
#include "entity.h"
#include "numbering.h"
#include "csh.h"
#include "area.h"
#include "progress.h"

/** Defines type information for arc related classes.
 *
 * Every arc needs a type from this class.
 * This is a bit of a historic artefact and is of mixed use.
 * On one hand it denotes the type of the arc (arrow, box, command, etc.)
 * but it is not so much used in this capacity.
 * On the other hand, for arrows, boxes and dividers it also denotes the
 * symbol used for the definition (->, => or <->). */
enum EArcType
{
    MSC_ARC_INVALID = 0,         ///<Invalid value
    MSC_ARC_SOLID,               ///<A solid, unidir arrow: -> or <- (ArcDirArrow, ArcSelfArrow, ArcBigArrow, ArcVertical)
    MSC_ARC_SOLID_BIDIR,         ///<A solid, bidirectional arrow: <-> (ArcDirArrow, ArcSelfArrow, ArcBigArrow, ArcVertical)
    MSC_ARC_DOTTED,              ///<A dotted unidir arrow: > or < (ArcDirArrow, ArcSelfArrow, ArcBigArrow, ArcVertical)
    MSC_ARC_DOTTED_BIDIR,        ///<A dotted bidir arrow: <> (ArcDirArrow, ArcSelfArrow, ArcBigArrow, ArcVertical)
    MSC_ARC_DASHED,              ///<A dashed unidir arrow: >> or << (ArcDirArrow, ArcSelfArrow, ArcBigArrow, ArcVertical)
    MSC_ARC_DASHED_BIDIR,        ///<A dashed bidir arrow: <<>> (ArcDirArrow, ArcSelfArrow, ArcBigArrow, ArcVertical)
    MSC_ARC_DOUBLE,              ///<A double lined unidir arrow: => or <= (ArcDirArrow, ArcSelfArrow, ArcBigArrow, ArcVertical)
    MSC_ARC_DOUBLE_BIDIR,        ///<A double lined bidirectional arrow: <=> (ArcDirArrow, ArcSelfArrow, ArcBigArrow, ArcVertical)
    MSC_ARC_UNDETERMINED_SEGMENT,///<An arrow segment of undefined type: - (ArcDirArrow, ArcBigArrow)

    MSC_ARC_BIG,                 ///<A unidir block arrow generated from a collapsed box (ArcBigArrow)
    MSC_ARC_BIG_BIDIR,           ///<A bidir block arrow generated from a collapsed box (ArcBigArrow)

    MSC_BOX_SOLID,               ///<A solid box: -- (ArcBox, ArcPipe, ArcVertical)
    MSC_BOX_DOTTED,              ///<A dotted box: .. (ArcBox, ArcPipe, ArcVertical)
    MSC_BOX_DASHED,              ///<A dashed box: ++ (ArcBox, ArcPipe, ArcVertical)
    MSC_BOX_DOUBLE,              ///<A double box: == (ArcBox, ArcPipe, ArcVertical)
    MSC_BOX_UNDETERMINED_FOLLOW, ///<A (subsequent) box in a box segment, without a type specifier (ArcBox)

    MSC_ARC_PARALLEL,            ///<An ArcParallel

    MSC_ARC_DISCO,               ///<A discontinuity in time line: ... (ArcDivider)
    MSC_ARC_DIVIDER,             ///<A divider: --- (ArcDivider)
    MSC_ARC_VSPACE,              ///<No arc, just space (maybe with label) (ArcDivider)

    MSC_COMMAND_HEADING,         ///<A heading command (CommandEntity)
    MSC_COMMAND_NUDGE,           ///<A nudge command (ArcDivider)
    MSC_COMMAND_ENTITY,          ///<Some entities (CommandEntity)
    MSC_COMMAND_NEWPAGE,         ///<The newpage command (CommandNewPage)
    MSC_COMMAND_NEWBACKGROUND,   ///<A background option (CommandBackground)
    MSC_COMMAND_NUMBERING,       ///<A number fromat option (CommandNumbering)
    MSC_COMMAND_MARK,            ///<A marker definition (CommandMark)
    MSC_COMMAND_EMPTY,           ///<The default fallback for an empty chart (CommandEmpty)
    MSC_COMMAND_HSPACE,          ///<The hspace command (CommandHSpace)
    MSC_COMMAND_VSPACE,          ///<The vspace command (CommandVSpace)
    MSC_COMMAND_SYMBOL,          ///<The symbol command (CommandSymbol)
    MSC_COMMAND_NOTE,            ///<A note or comment command (CommandNote)
    MSC_COMMAND_TITLE,           ///<A title command (ArcDivider)
    MSC_COMMAND_SUBTITLE,        ///<A subtitle command (ArcDivider)

    MSC_ARC_ARCLIST,             ///<A list of arcs for internal use (CommandArcList)
    MSC_ARC_EN_SEPARATOR,        ///<A list of arcs for internal use (CommandArcList)
    MSC_ARC_INDICATOR            ///<An indicator element inserted to replace hidden elements (ArcIndicator)
};

/** The direction of an arrow in the text file */
enum EDirType {
    MSC_DIR_INDETERMINATE, ///<An arrow segment of unspecified direction
    MSC_DIR_RIGHT,         ///<An arrow pointing left-to-right. This is the forward direction.
    MSC_DIR_LEFT,          ///<An arrow pointing right-to-left. This is the reverse direction.
    MSC_DIR_BIDIR          ///<A bidirectional arrow.
};

/** Indicates if the message was lost and where. */
enum class EArrowLost {
    NOT,     ///<The arrow is not lost
    AT_SRC,  ///<The arrow is lost close to the src entity
    AT_DST   ///<The arrow is lost close to the dst entity
};

/** Collects data about an arrow segments (dashed/solid/dotted/double and 
 * whether it indicates a lost message using an asterisk */
struct ArrowSegmentData {
    EArcType   type;
    EArrowLost lost;
    PODFileLineColRange lost_pos;
};

class EntityDistanceMap;
struct ArcSignature;

/** The collapse/expand status of a box */
enum EBoxCollapseType {
    BOX_COLLAPSE_INVALID=0, ///<Invalid value
    BOX_COLLAPSE_EXPAND,    ///<The box is expanded.
    BOX_COLLAPSE_COLLAPSE,  ///<The box is collapsed to a box with no content
    BOX_COLLAPSE_BLOCKARROW ///<The box is collapsed to a block arrow
};

/**A catalog of box collapse status */
typedef std::map<ArcSignature, EBoxCollapseType> ArcSignatureCatalog;
/** Box attributes defining a box.
 * We save these to determine, if a box previously collapsed on the GUI
 * is the same as a box after re-compilation (and potentially source text modification)*/
struct ArcSignature {
    FileLineColRange file_pos;
    bool operator == (const ArcSignature&o) const;
    bool operator < (const ArcSignature&o) const;
    ArcSignatureCatalog::iterator WhichIsSimilar(ArcSignatureCatalog &cat) const;
};


/** The base class for all arcs and commands.
 * One object of this type roughly correspond to one semicolon (;) delimited
 * line of input text. */
class ArcBase : public Element
{
private:
    bool had_add_attr_list;    //TODO: debug only, remove
protected:
    bool   valid;          ///<If false, then construction failed, arc does not exist 
    bool   compress;       ///<True if compress mechanism is on for this arc (compress attribute set)
    bool   parallel;       ///<If true, subsequent arcs can be drawn beside this.
    bool   keep_together;  ///<If true, do not split this by automatic pagination.
    bool   keep_with_next; ///<If true, do not separate this from following element by automatic pagination 
    string refname;        ///<Value of the "refname" attribute, to reference numbers & others. Empty if none.
    mutable double height; ///<Pixel height of the arc, calculated by Layout()
public:
    const EArcType type;   ///<Type of the arc
    const MscProgress::ECategory myProgressCategory; ///<The category of the arc for calculating progress.

    /** Basic constructor, taking const members and the reference to Msc we are member of.*/
    ArcBase(EArcType t, MscProgress::ECategory c, Msc *msc);
    /** A virtual destructor */
	virtual ~ArcBase();
    /** Ture if object is valid */
    bool IsValid() const {return valid;}
    /** True if subsequent arcs can be drawn beside this.*/
    bool IsParallel() const {return parallel;}
    /** Set the parallel member to true. Used only during parsing.*/
    void SetParallel() {parallel = true;}
    /** True, if compress mechanism is on foe this arc and can be shifted upwards.*/
    bool IsCompressed() const {return compress;}
    /** True if this arc shall be placed at the centerline of a previous one.*/
    bool IsKeepTogether() const {return keep_together;}
    /** If true, do not separate this from following element by automatic pagination */
    bool IsKeepWithNext() const {return keep_with_next;}
    /** True if a note can be attached to this arc. */
    virtual bool CanBeNoted() const {return false;}
    /** Get the Y coordinate of the top of the arc. */
    double GetPos() const {return yPos;}
    /** Return the height of the element with its side comments */
    double GetHeight() const {return std::max(height, comment_height);}
    /** Get the Y coordinate range occupied by the arc.*/
    virtual Range GetYExtent() const {return Range(yPos, yPos+GetHeight());}
    /** Get an (ordered) list of entities that this arrow or box touches.
      * @return the direction of the arrows inside (left is all arrows are left; bidir if mixed.*/
    virtual EDirType GetToucedEntities(EntityList &) const {return MSC_DIR_INDETERMINATE;}
    /** Expands our contour calculated as `a` to a cover used for compress bump checking*/
    Area GetCover4Compress(const Area &a) const;
    /** Allocates and fills in a signature object identifying the arc. NULL if no signature available.*/
    virtual const ArcSignature* GetSignature() const {return NULL;}

    /** Applies a set of attributes to us.*/
    virtual void AddAttributeList(AttributeList *);
    /** Applies one attribute to us */
    virtual bool AddAttribute(const Attribute &);
    /** Add attribute names and helpers to csh for hinting */
    static void AttributeNames(Csh &csh);
    /** Add attribute values and helpers to csh for hinting */
    static bool AttributeValues(const std::string attr, Csh &csh);

    /** Converting type to text for debugging*/
    virtual string PrintType(void) const;
    /** Converting to text for debugging, idented at specified level*/
    virtual string Print(int ident = 0) const = 0;

    /** @name Recursive processors
     * These functions are called recursively for all arcs in this order*/
    /** @{ */
    /** Calculate things  after parsing.
     * Called after parsing and adding attributes. 
     * Entity order and collapse/expand is already known here 
     * See documentation for libmscgen for more info.*/
    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, Element **note_target);
    /** Substitute name references in labels & process all escapes 
     * See documentation for libmscgen for more info.*/
    virtual void FinalizeLabels(Canvas &canvas);
    /** Collects vertical distances for hscale=auto mechanism.
     * See documentation for libmscgen for more info.*/
    virtual void Width(Canvas &/*canvas*/, EntityDistanceMap &/*distances*/) {}
    /** Calculate entity layout at y=0 vertical position. 
     * Calculates the height, and sets up the area at yPos==0, 
     * add its cover to use at placement later to `cover`.
     * Cover or area does not include any spacing left around such as chart->emphVGapAbove.
     * See documentation for libmscgen for more info.*/
    virtual void Layout(Canvas &canvas, AreaList *cover);
    /** Move the arc up or down. This can be called multiple times. */
    virtual void ShiftBy(double y) {if (valid) {Element::ShiftBy(y);}}
    /** Collect the y position of page breaks into Msc::pageBreakData. 
     * See documentation for libmscgen for more info.*/
    virtual void CollectPageBreak(double /*hSize*/) {}
    /** Split the element into two by a page break. 
     * Maintain running_state of entities. 
     * @returns The amount the element has grown. -1 if we cannot rearrange, -2 if we are to ignore.
     * See documentation for libmscgen for more info.*/
    virtual double SplitByPageBreak(Canvas &/*canvas*/, double /*netPrevPageSize*/,
                                    double /*pageBreak*/, bool &/*addCommandNewpage*/, 
                                    bool /*addHeading*/, ArcList &/*res*/) {return -1;}
    /** Place verticals. 
     * All height & pos info final by now, except on verticals & notes 
     * See documentation for libmscgen for more info.*/
    virtual void PlaceWithMarkers(Canvas &/*cover*/, double /*autoMarker*/) {}
    /** Emit warnings that need final layout. 
     * See documentation for libmscgen for more info.*/
    virtual void PostPosProcess(Canvas &cover);
    /** Draw the arc.
     * See documentation for libmscgen for more info.*/
    virtual void Draw(Canvas &canvas, EDrawPassType pass) = 0;
    /** @} */
};

/** A list of ArcBase pointers*/
typedef PtrList<ArcBase> ArcList;

/** An arc representing a small indicator symbol.
 * This arc cannot be generated by the user..
 * it is generated by libmscgen to replace arcs that get hidden due to collapsing 
 * a group entity.
 * An indicator is either on a specific entity or in a box, which may have two entities
 * setting its location. `src` and `dst` members capture the horizontal location.*/
class ArcIndicator : public ArcBase
{
    const StyleCoW style; ///<Our visual style
    EIterator src;        ///<Our leftmost entity. Shall always point to ActiveEntities
    EIterator dst;        ///<Our rightmost entity. Shall always point to ActiveEntities
public:
    /** Creates an indicator object setting its location (`src` and `dst`) to NoEntity.*/
    ArcIndicator(Msc *chart, const StyleCoW &st, const FileLineColRange &l);
    /** Creates an indicator object at Entity `s`*/
    ArcIndicator(Msc *chart, EIterator s, const StyleCoW &st, const FileLineColRange &l);
	virtual MscProgress::ECategory GetProgressCategory() const {return MscProgress::INDICATOR;}
    /** True if `src` and `dst` are properly set.*/
    bool IsComplete() const;
    void SetEntities(EIterator s, EIterator d) {src=s; dst=d;}
    /** Expand the `file_pos` member with that of another interator if at same location. 
     * @returns false, if `src` or `dst` differ (and then no change to `file_pos`.*/
    bool Combine(const ArcIndicator *o);
    /** Returns the x coordinate in chart space of the middle of the indicator */
    double GetXCoord() const;
    virtual EDirType GetToucedEntities(class EntityList &el) const;
    virtual string Print(int ident = 0) const {return string(ident*2, ' ')+"Indicator";}
    virtual void Width(Canvas &canvas, EntityDistanceMap &distances);
    virtual void Layout(Canvas &canvas, AreaList *cover);
    virtual void Draw(Canvas &canvas, EDrawPassType pass);
};

/** Root class for all arcs having a label.
 * Contains style, label and numbering fields.*/
class ArcLabelled : public ArcBase
{
protected:
    string          label;           ///<The label as the user specified it. Empty if none.
    Label           parsed_label;    ///<The label processed and parsed into lines. Set in PostParseProcess()
    int             concrete_number; ///<Negative, if the user specified no specific value for the number (no numbering or automatic). Else the number specified.
    StyleCoW        style;           ///<The style of the arc. `numbering` and `compress` fields of style are not used. The ArcBase member fields are used instead.
    NumberingStyle  numberingStyle;  ///<The numbering style to use. (This is not part of Styles in general, but is a property of contexts. This is a snapshot at the location of the arc.
    string          number_text;     ///<The formatted number assigned to this arc (used by references and notes/comments). Set in PostParseProcess()
    Range           entityLineRange; ///<The y range to apply any vFill or vLine attributes
public:
    ArcLabelled(EArcType t, MscProgress::ECategory c, Msc *msc, const StyleCoW &);
    ArcLabelled(EArcType t, MscProgress::ECategory c, const ArcLabelled &al);
    const StyleCoW &GetStyle() const {return style;}
    virtual bool CanBeNoted() const {return true;}
    /** Set style to this name, but combine it with default text style */
    void SetStyleWithText(const char *style_name); 
    /** Set style to this name, but combine it with default text style. Use existing style if NULL.*/
    void SetStyleWithText(const StyleCoW *style_to_use=NULL); 
    /** Generate a warning on overflown labels.
     * Shall be called from Layout().
     * @param [in] overflow The amount of overflow.
     * @param [in] msg The message to display. NULL if the space depends on entities and a
     *                 corresponding message shall be displayed.
     * @param [in] e1 The left entity governing the space available for the label.
     * @param [in] e2 The right entity governing the space available for the label. */
    void OverflowWarning(double overflow, const string &msg, 
                         EIterator e1=EIterator(),  EIterator e2=EIterator());
    /** Counts non-word-wrapped labels in the chart and also how many has been overflown.
     * It is used to display a warning at the end if may labels were 
     * overflown. Shall be called from Layout();
     * @param [in] space Says how much vertial space the label has.
     *                   If the label is wider, we mark it as overflown,
     *                   but only if no word_wrapping is used for the labels. */
    void CountOverflow(double space);
    /** Depending on the arc type return the refinement style.
     * An example of a refinement style is the style named '->', which (by default)
     * sets the line type to solid and is applied after the 'arrow' style, to reflect
     * the difference between a '->' arrow and a '=>' arrow, for example.
     * This works for boxes, block arrows, etc.*/
    virtual const StyleCoW *GetRefinementStyle(EArcType t) const;
    virtual void AddAttributeList(AttributeList *);
    bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    string Print(int ident=0) const;
    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, Element **note_target);
    /** Return the formatted number of the arc (empty string if none).*/
    const string &GetNumberText() const {return number_text;}
    /** Return the numbering style of the arc (even if numbering is turned off).*/
    const NumberingStyle &GetNumberingStyle() const {return numberingStyle;}
    virtual void FinalizeLabels(Canvas &canvas);
    virtual void PostPosProcess(Canvas &cover);
};

/** The base class for all arrows: Self, Dir, Block and Vertical*/
class ArcArrow : public ArcLabelled
{
public:
    /** This is the typical constructor */
    ArcArrow(EArcType t, MscProgress::ECategory c, Msc *msc, const StyleCoW &s) : ArcLabelled(t, c, msc, s) {}
    /** Constructor used only when converting an ArcBox to and ArcBigArrow*/
    ArcArrow(EArcType t, MscProgress::ECategory c, const ArcLabelled &al) : ArcLabelled(t, c, al) {}
    /** Add a new arrow segment. Called during processing.
     * @param [in] data The type of the new arrow segment, indication of amy 
     *                  potential loss indication and its location.
     * @param [in] m The name of the entity (can be NULL when omitted by user)
     * @param [in] ml The location of the name in the input file.
     * @param [in] l The location of the whole added segment (for error messages)*/
    virtual ArcArrow *AddSegment(ArrowSegmentData data, const char *m, const FileLineColRange &ml, 
                                 const FileLineColRange &l) = 0;
    bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    /** True if the arrow is bidirectional */
    bool isBidir(void) const {return type == MSC_ARC_SOLID_BIDIR || type == MSC_ARC_DOTTED_BIDIR ||
                                     type == MSC_ARC_DASHED_BIDIR || type == MSC_ARC_DOUBLE_BIDIR ||
                                     type == MSC_ARC_BIG_BIDIR;}
    virtual void PostPosProcess(Canvas &cover);
};

/** An arrow with the same entity as start and end (like "a->a")*/
class ArcSelfArrow : public ArcArrow
{
protected:
    EIterator src;          ///<The entity of the arrow
    double    YSize;        ///<The vertical difference between the start and end of the arrow 
    const double XSizeUnit; ///<This is the width of the arrow arc in entity units (=130 pixel).

    mutable XY xy_s;       ///<Calculated startpoint of the arrow
    mutable XY xy_e;       ///<Calculated endpoint of the arrow
    mutable XY wh;         ///<Calculated width and height of the arrow arc
    mutable double sx;     ///<Calculated left side of label
    mutable double dx;     ///<Calculated right side of label
    mutable double src_act;///<Activation offset of entity 'src' at the arrow. 0 if not active.
public:
    ArcSelfArrow(EArcType t, const char *s, const FileLineColRange &sl,
        Msc *msc, const StyleCoW &, double ys);
    virtual ArcArrow *AddSegment(ArrowSegmentData data, const char *m, const FileLineColRange &ml,
                                 const FileLineColRange &l);
    virtual EDirType GetToucedEntities(EntityList &el) const;
    string Print(int ident=0) const;
    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, Element **note_target);
    virtual void Width(Canvas &canvas, EntityDistanceMap &distances);
    virtual void Layout(Canvas &canvas, AreaList *cover);

    virtual void PostPosProcess(Canvas &cover);
    virtual void Draw(Canvas &canvas, EDrawPassType pass);
};

/** A (potentially multi-segmented) arrow.
 * This is the most common chart element.*/
class ArcDirArrow : public ArcArrow
{
protected:
    EIterator                src;                  ///<Starting entity of arrow.
    EIterator                dst;                  ///<Ending entity of arrow.
    FileLineCol              linenum_src;          ///<Location of `src` in the input file (within the arrow definition)
    FileLineCol              linenum_dst;          ///<Location of `dst` in the input file (within the arrow definition)
    std::vector<EIterator>   middle;               ///<An array of mid-stops of the arrow in the order as they appear in the input file
    std::vector<FileLineCol> linenum_middle;       ///<Location of mid-stop entity names in the input file (within this arrow definition)
    std::vector<EArcType>    segment_types;        ///<Types of segments: one for each segment ([0] is the one from src). Set during AddSegment() calls.
    std::vector<LineAttr>    segment_lines;        ///<Line types of segments. Set during AddAttributeList() from `segment_types`.
    const bool               specified_as_forward; ///<True if user specified "a->b", false if "b<-a"
    double                   slant_angle;          ///<The angle of the arrow. Taken from the context, may be modified by style and/or attribute.
    int                      lost_at;              ///<Index of where the message was lost: -2=none, -1=src, 0..n = at a midpoint, n=dst; where n is the size of 'middle'.
    bool                     lost_is_forward;      ///<True if the loss was at the fw side of 'lost_at', false if at the backwards size

    mutable double sin_slant; ///<Pre-calculated value of `slant_angle`
    mutable double cos_slant; ///<Pre-calculated value of `slant_angle`
    mutable double sx;        ///<Calculated value of the start arrowhead (sx can be > dx)
    mutable double dx;        ///<Calculated value of the end arrowhead (sx can be > dx)
    mutable double sx_text;   ///<Calculated value of the left of the label (usually sx adjusted by arrowhead)
    mutable double dx_text;   ///<Calculated value of the right of the label (usually dx adjusted by arrowhead)
    mutable double cx_text;   ///<Calculated value of the middle of the label (usually middle of sx and dx)
    mutable std::vector<double> xPos;        ///<X coordinates of arrowheads (sorted to increase, [0] is for left end of the arrow). Always the middle of the entity line.
    mutable std::vector<double> act_size;    ///<Activation size of the entities (sorted from left to right). ==0 if not active
    mutable std::vector<DoublePair> margins; ///<How much of the arrow line is covered by the arrowhead (sorted left to right), see ArrowHead::getWidths()
    mutable Contour text_cover; ///<The cover of the label, so that we can hide entity lines in PostPosProcess() - not used by ArcBigArrow
    mutable double centerline;  ///<y offset of line compared to yPos
    mutable Contour clip_area;  ///<Used to mask out the line at arrowheads - not used by ArcBigArrow.
public:
    /** Regular constructor. 
     * @param t The type of the arrow, like MSC_ARROW_SOLID.
     * @param s The name of the source entity.
     * @param sl The location of the name of the source entity in the input file
     * @param d The name of the destination entity
     * @param dl The location of the name of the destination entity in the input file
     * @param msc The char we add the arrow to
     * @param fw True if declared as a->b and false if b<-a
     * @param st The style to apply*/
    ArcDirArrow(ArrowSegmentData data, const char *s, const FileLineColRange &sl,
                const char *d, const FileLineColRange &dl, Msc *msc, bool fw, 
                const StyleCoW &st);
    /** Constructor used while converting a collapsed ArcBox to an ArcBigArrow*/
    ArcDirArrow(const EntityList &el, bool bidir, const ArcLabelled &al);
    virtual ArcArrow *AddSegment(ArrowSegmentData data, const char *m, const FileLineColRange &ml,
                                 const FileLineColRange &l);
    virtual void AddAttributeList(AttributeList *l);
    bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    virtual EDirType GetToucedEntities(EntityList &el) const;
    string Print(int ident=0) const;
    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, Element **note_target);
    /** Update the stored activation status of entities.
     * Called if a centierlined entity command after us has changed active entities.*/
    virtual void UpdateActiveSizes(); 
    virtual void FinalizeLabels(Canvas &canvas);
    virtual void Width(Canvas &canvas, EntityDistanceMap &distances);
    virtual void Layout(Canvas &canvas, AreaList *cover);
    /** Get the Centerline Delta from the top of the arc at horizontal position `x`.
     * A negative return value indicates the object has no centerline. 
     * Shall be called only after Layout()*/
    virtual double GetCenterline(double x) const;
    /** Calculates `area.mainline`. Tricky for slanted arrows.
     * @param [in] thickness The y thinckess of the mainline*/
    void CalculateMainline(double thickness);
    /** Is this the start or end of the arrow?
     * From the index of `xPos` and `specified_as_forward` give MSC_ARROW_{START,MIDDLE,END}*/
    EArrowEnd WhichArrow(unsigned i); 
    virtual void ShiftBy(double y);
    /** Check if the entities in `src` - `middle[]` - `dst` are all left-to-right or right-to-left*/
    void CheckSegmentOrder(double y);
    virtual void PostPosProcess(Canvas &cover);
    virtual void Draw(Canvas &canvas, EDrawPassType pass);
};

/** A block arrow */
class ArcBigArrow : public ArcDirArrow
{
protected:
    /** Signature of the ArcBox, if we got created as a collapsed ArcBox. NULL if not*/
    const ArcSignature * const sig;   

    mutable double sy;      ///<The y coordinate of the top of the arrow body (not including arrowheads). Set in Width()
    mutable double dy;      ///<The y coordinate of the bottom of the arrow body (not including arrowheads). Set in Width()
    mutable double ind_off; ///<If we draw an indicator inside, this is its y offset from top of text
    mutable int stext;      ///<Index of the left entity of the segment containing the text: xPos[stext]. Set in Width()
    mutable int dtext;      ///<Index of the right entity of the segment containing the text: xPos[stext]. Set in Width()
    mutable double sm;      ///<Left marging for text. Set in Width()
    mutable double dm;      ///<Right marging for text. Set in Width()
    mutable std::vector<Contour> outer_contours; ///<The outline for each segment
public:
    /** Upgrade an ArcDirArrow to a block arrow: the regular way of creating one. */
    ArcBigArrow(const ArcDirArrow &, const StyleCoW &);
    /** Used when converting a collapsed ArcBox to an ArcBigArrow */
    ArcBigArrow(const EntityList &, bool bidir, const ArcLabelled &al, const ArcSignature *s);
    ~ArcBigArrow() {if (sig) delete sig;}
    virtual const StyleCoW *GetRefinementStyle(EArcType t) const;
    virtual const ArcSignature* GetSignature() const {return sig;}
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    string Print(int ident=0) const;
    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, Element **note_target);
    virtual void FinalizeLabels(Canvas &canvas);
    virtual void Width(Canvas &canvas, EntityDistanceMap &distances);
    virtual void Layout(Canvas &canvas, AreaList *cover);

    virtual void ShiftBy(double y);
    virtual void PostPosProcess(Canvas &cover);
    virtual void Draw(Canvas &canvas, EDrawPassType pass);
};

/** Holds a horizontal position, as specified by the user.
 * (Apologies for the name.)
 * Can encode the following language fragments:
 * 1. AT @<entity> [@<offset>]
 * 2. AT @<entity> - @<entity> [@<offset>]
 * 3. AT @<entity> -|--|+|++ [@<offset>]
 *
 * The first will result in type POS_AT, the second in POS_CENTER, while the third in
 * POS_LEFT_SIDE, POS_LEFT_BY, POS_RIGHT_SIDE or POS_RIGHT_BY, resp. */
struct VertXPos {
    /** AT is at the entity
     ** CENTER is between the two entities
     ** BY is immediately at the line, but not overlapping it
     ** SIDE is immediately at the line, arrow tips overlap the line */
    /** Options for horizontal position in relation to one or two entities */
    enum EPosType {
        POS_INVALID=0, ///<Invalid value
        POS_AT,        ///<At the entity
        POS_CENTER,    ///<Halfway between two entities
        POS_LEFT_BY,   ///<Immediately left by the entity line, but not overlapping it.
        POS_LEFT_SIDE, ///<immediately left by the line, arrow tips overlap the line.
        POS_RIGHT_BY,  ///<Immediately right by the entity line, but not overlapping it.
        POS_RIGHT_SIDE ///<immediately right by the line, arrow tips overlap the line.
    }; 

    bool valid;               ///<True if the position is valid
    EPosType pos;             ///<The type of the position
    EIterator entity1;        ///<The first (or only one) entity
    EIterator entity2;        ///<The second entity, used only for POS_CENTER
    FileLineColRange e1line;  ///<Location of the mention of the first entity in the input line (inside this vertpos fragment)
    FileLineColRange e2line;  ///<Location of the mention of the second entity in the input line (inside this vertpos fragment)
    double offset;            ///<An arbitrary user specified horizontal offset on top of the poition defined by `pos`

    /** Create a position with 2 entities (always POS_CENTER) */
    VertXPos(Msc&m, const char *e1, const FileLineColRange &e1l, const char *e2, const FileLineColRange &e2l, double off=0);
    /** Create a position with one entity (must not be POS_CENTER) */
    VertXPos(Msc&m, const char *e1, const FileLineColRange &e1l, EPosType p=POS_AT, double off=0);
    /** Create (yet) invalid object */
    explicit VertXPos(Msc&m);
    /** Calculate the x coordinate of the middle of the element we specify the location for (only after entities are placed) 
     * @param m The chart
     * @param width The width of the object
     * @param gap Specifies how much gap between the entity and the object for POS_XXX_BY. Defaults to `m.hscaleAutoXGap`*/
    double CalculatePos(Msc &m, double width=0, double gap=-1) const;
};

/** A vertical */
class ArcVerticalArrow : public ArcArrow
{
protected:
    string src;   ///<The top marker (if any)
    string dst;   ///<The bottom marker (if any)
    VertXPos pos; ///<The horizontal position, specified by the user
    mutable std::vector<double> ypos; ///<A two element array with the y pos of the start and end. Set in PlaceWithMarkers()
    mutable double sy_text;  ///<Top/Bottom y edge of text (depending on side)
    mutable double dy_text;  ///<Top/Bottom y edge of text (depending on side) 
    mutable double xpos;     ///<x coordinate of middle    
    mutable double width;    ///<width of us
    mutable std::vector<Contour> outer_contours; ///<Calculated contour (only one element)
public:
    /** Regular constructor with two marker names (one can be NULL)*/
    ArcVerticalArrow(EArcType t, const char *s, const char *d, Msc *msc);
    virtual ArcArrow *AddSegment(ArrowSegmentData data, const char *m, const FileLineColRange &ml,
                                 const FileLineColRange &l);
    /** Add the parsed horizontal position (starting with the AT keyword) */
    ArcVerticalArrow* AddXpos(VertXPos *p);
    virtual const StyleCoW *GetRefinementStyle(EArcType t) const;
    bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    virtual Element* AttachNote(CommandNote *);
    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, Element **note_target);
    virtual void Width(Canvas &canvas, EntityDistanceMap &distances);
    virtual void Layout(Canvas &canvas, AreaList *cover);

    virtual void ShiftBy(double y);
    virtual Range YExtent() {return Range(false);}
    virtual double SplitByPageBreak(Canvas &/*canvas*/, double /*netPrevPageSize*/,
                                    double /*pageBreak*/, bool &/*addCommandNewpage*/, 
                                    bool /*addHeading*/, ArcList &/*res*/) {return -2;}
    virtual void PlaceWithMarkers(Canvas &cover, double autoMarker);
    virtual void PostPosProcess(Canvas &cover);
    virtual void Draw(Canvas &canvas, EDrawPassType pass);
};

/** One box (potentially in a box series) with or without elements in it */
class ArcBox : public ArcLabelled
{
    friend struct pipe_compare;
    friend class ArcBoxSeries;
    friend class ArcPipe;
    friend struct ArcSignature;
protected:
    EIterator        src;             ///<The left entity specified by the user (or calculated from content)
    EIterator        dst;             ///<The right entity specified by the user (or calculated from content)
    EBoxCollapseType collapsed;       ///<The collapseStatus of the box.
    bool             drawEntityLines; ///<True if we draw the entity lines. Only for boxes with content: empty boxes never draw lines
    ArcList          content;         ///<The arcs contained within the box.

    mutable double height_w_lower_line; ///<Height of this box including its lower line
    mutable double sx_text;             ///<Left boundary of label
    mutable double dx_text;             ///<Right boundary of label
    mutable double y_text;              ///<Top boundary of label
    mutable Contour text_cover;         ///<Area covered by the label
public:
    ArcBox(EArcType t, const char *s, const FileLineColRange &sl,
        const char *d, const FileLineColRange &dl, Msc *msc);
    virtual bool CanBeNoted() const {return true;}
    virtual const ArcSignature* GetSignature() const;
    ArcBox* AddArcList(ArcList*l);
    virtual void AddAttributeList(AttributeList *);
    bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    string Print(int ident=0) const;
    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, Element **note_target);
    virtual void FinalizeLabels(Canvas &canvas);
    virtual void Layout(Canvas &/*canvas*/, AreaList &/*cover*/) {_ASSERT(0);}
    virtual void ShiftBy(double y);
    virtual void Draw(Canvas &/*canvas*/, EDrawPassType /*pass*/) {} //will never be called
};

/** A box series - used even for a single box.*/
class ArcBoxSeries : public ArcBase
{
protected:
    PtrList<ArcBox> series;          ///<The boxes in the series - may contain only a single one
    int             drawing_variant; ///<One of 3 variants on how to draw double or triple lines

    mutable double total_height;     ///<Total height of the series
    mutable double left_space;       ///<How much do we expand left of the entity line of src. Includes linewidth.
    mutable double right_space;      ///<How much do we expand right of the entity line of dst. Includes linewidth and shadow.
public:
    /** Create a series using a box as a first element */
    ArcBoxSeries(ArcBox *first);
    /** Append subsequent boxes to the series */
    ArcBoxSeries* AddFollow(ArcBox *f);
    virtual EDirType GetToucedEntities(EntityList &el) const;
    string Print(int ident=0) const;
    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, Element **note_target);
    virtual void FinalizeLabels(Canvas &canvas);
    virtual void Width(Canvas &canvas, EntityDistanceMap &distances);
    virtual void Layout(Canvas &canvas, AreaList *cover);

    virtual void ShiftBy(double y);
    virtual void CollectPageBreak(double hSize);
    virtual double SplitByPageBreak(Canvas &canvas, double netPrevPageSize,
                                    double pageBreak, bool &addCommandNewpage, 
                                    bool addHeading, ArcList &res);
    virtual void PlaceWithMarkers(Canvas &cover, double autoMarker);
    virtual void PostPosProcess(Canvas &cover);
    virtual void Draw(Canvas &canvas, EDrawPassType pass);
};

/** One pipe segment */
class ArcPipe : public ArcLabelled
{
    friend struct pipe_compare;
    friend class ArcPipeSeries;
protected:
    EIterator src;             ///<The left entity specified by the user (or calculated)
    EIterator dst;             ///<The right entity specified by the user (or calculated)
    bool      drawEntityLines; ///<True if we draw the entity lines. Only for pipes with content: empty boxes never draw lines

    mutable bool pipe_connect_back; ///<True if this segment connects to a previous segment on its left side
    mutable bool pipe_connect_forw; ///<True if this segment connects to a subsequent segment on its right side
    mutable double left_space;      ///<How much we expand beyond src. Includes linewidth.
    mutable double right_space;     ///<How much we expand beyond dst. Includes linewidth and shadow.
    mutable double sx_text;         ///<Left boundary of label
    mutable double dx_text;         ///<Right boundary of label
    mutable double y_text;          ///<Top boundary of label
    mutable Contour text_cover;     ///<Area covered by the label
    mutable Block pipe_block;       ///<A representative rectangle of the pipe                    
    mutable Contour pipe_shadow;    ///<The outer contour of the pipe used to draw the shadow
    mutable Contour pipe_body_line; ///<The line around the body of the pipe (excluding the hole at the end)
    mutable Contour pipe_whole_line;///<The line around the whole of the pipe (including the hole at the end)
    mutable Contour pipe_body_fill; ///<The fill area of the body of the pipe (excluding the hole at the end)
    mutable Contour pipe_hole_fill; ///<The fill area of the hole at the end
    mutable Contour pipe_hole_line; ///<The line around the hole at the ehnd   
    mutable Contour pipe_hole_curve;///<The curve at the hole end
public:
    /** Upgrade a box to a pipe: this is how we create pipes normally */
    ArcPipe(ArcBox *box);
    virtual bool CanBeNoted() const {return true;}
    bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    string Print(int ident=0) const;
    virtual void Layout(Canvas &/*canvas*/, AreaList &/*cover*/) {_ASSERT(0);}
    virtual void ShiftBy(double y);
    /** Helper to draw various parts of the pipe */
    void DrawPipe(Canvas &canvas, bool topSideFill, bool topSideLine, bool backSide, bool shadow,
                  bool text, double next_lw, int drawing_variant);
    virtual void Draw(Canvas &/*canvas*/, EDrawPassType /*pass*/) {} //will never be called
};

/** A pipe series - used even for a pipe with one segment */
class ArcPipeSeries : public ArcBase
{
protected:
    PtrList<ArcPipe> series;          ///<The pipes in the series
    ArcList          content;         ///<Any arcs contained in the pipe series
    int              drawing_variant; ///<One of 3 variants on how to draw double or triple lines

    mutable double total_height;      ///<Total height of the pipe series
public:
    /** Create a series using a pipe as a first element */
    ArcPipeSeries(ArcPipe *first);
    /** Add another pipe segment (can be left or right from existing ones) */
    ArcPipeSeries* AddFollowWithAttributes(ArcPipe*f, AttributeList *l);
    /** Add actual content */
    ArcPipeSeries* AddArcList(ArcList*l);
    virtual EDirType GetToucedEntities(EntityList &el) const;
    string Print(int ident=0) const;
    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, Element **note_target);
    virtual void FinalizeLabels(Canvas &canvas);
    virtual void Width(Canvas &canvas, EntityDistanceMap &distances);
    void CalculateContours(Area *pipe_body_cover=NULL);
    virtual void Layout(Canvas &canvas, AreaList *cover);

    virtual void ShiftBy(double y);
    virtual void CollectPageBreak(double hSize);
    virtual double SplitByPageBreak(Canvas &canvas, double netPrevPageSize,
                                    double pageBreak, bool &addCommandNewpage, 
                                    bool addHeading, ArcList &res);
    virtual void PlaceWithMarkers(Canvas &cover, double autoMarker);
    virtual void PostPosProcess(Canvas &cover);
    virtual void Draw(Canvas &canvas, EDrawPassType pass);
};


/** A divider, gap, title, subtitle or nudge command */
class ArcDivider : public ArcLabelled
{
protected:
    const bool nudge;          ///<True if created from a `nudge` command
    const bool title;          ///<True if created from a `title` or `subtitle` command
    bool wide;                 ///<If true, we keep no margin and add no gap above & below (for copyright text)
    const double extra_space;  ///<Extra vertical space above and below: depends on how we created this object

    mutable double centerline; ///<Y coordinate of the centerline
    mutable double text_margin;///<left and right text margin from chart edge
    mutable double line_margin;///<left and right line margin from chart edge
    mutable Contour text_cover;///<The cover of the label
public:
    ArcDivider(EArcType t, Msc *msc);
    /** Return default style name for this arc type */
    static const char *MyStyleName(EArcType t);
    bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh, bool nudge, bool title);
    static bool AttributeValues(const std::string attr, Csh &csh, bool nudge, bool title);
    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, Element **note_target);
    virtual void Width(Canvas &canvas, EntityDistanceMap &distances);
    virtual void Layout(Canvas &canvas, AreaList *cover);

    virtual void ShiftBy(double y);
    virtual void PostPosProcess(Canvas &cover);
    virtual void Draw(Canvas &canvas, EDrawPassType pass);
};

/** Multiple parallel blocks */
class ArcParallel : public ArcBase
{
protected:
    std::vector<ArcList> blocks; ///<Arc lists, one for each bloxk
public:
    /** Create a parallel construct from its first block */
    ArcParallel(Msc *msc, ArcList*l) : ArcBase(MSC_ARC_PARALLEL, MscProgress::PARALLEL, msc) {AddArcList(l);}
    /** Add one more parallel block */
    ArcParallel* AddArcList(ArcList*l);
    virtual EDirType GetToucedEntities(EntityList &el) const;
    string Print(int ident=0) const;
    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, Element **note_target);
    virtual void FinalizeLabels(Canvas &canvas);
    virtual void Width(Canvas &canvas, EntityDistanceMap &distances);
    virtual void Layout(Canvas &canvas, AreaList *cover);

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