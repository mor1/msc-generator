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

/** Enumerates the type of arrow, box and divider symbols (->, => or <->, etc). */
enum class EArcSymbol
{
    INVALID = 0,             ///<Invalid value
    ARC_SOLID,               ///<A solid, unidir arrow: -> or <- (ArcDirArrow, ArcSelfArrow, ArcBigArrow, ArcVertical)
    ARC_SOLID_BIDIR,         ///<A solid, bidirectional arrow: <-> (ArcDirArrow, ArcSelfArrow, ArcBigArrow, ArcVertical)
    ARC_DOTTED,              ///<A dotted unidir arrow: > or < (ArcDirArrow, ArcSelfArrow, ArcBigArrow, ArcVertical)
    ARC_DOTTED_BIDIR,        ///<A dotted bidir arrow: <> (ArcDirArrow, ArcSelfArrow, ArcBigArrow, ArcVertical)
    ARC_DASHED,              ///<A dashed unidir arrow: >> or << (ArcDirArrow, ArcSelfArrow, ArcBigArrow, ArcVertical)
    ARC_DASHED_BIDIR,        ///<A dashed bidir arrow: <<>> (ArcDirArrow, ArcSelfArrow, ArcBigArrow, ArcVertical)
    ARC_DOUBLE,              ///<A double lined unidir arrow: => or <= (ArcDirArrow, ArcSelfArrow, ArcBigArrow, ArcVertical)
    ARC_DOUBLE_BIDIR,        ///<A double lined bidirectional arrow: <=> (ArcDirArrow, ArcSelfArrow, ArcBigArrow, ArcVertical)
    ARC_DBLDBL,              ///<An mscgen double lined double unidir arrow: =>> or <<= (ArcDirArrow, ArcSelfArrow, ArcBigArrow, ArcVertical)
    ARC_DBLDBL_BIDIR,        ///<An mscgen double lined double bidirectional arrow: <<=>> (ArcDirArrow, ArcSelfArrow, ArcBigArrow, ArcVertical)
    ARC_COLON,               ///<An mscgen unidir arrow with colon: :> or <: (ArcDirArrow, ArcSelfArrow, ArcBigArrow, ArcVertical)
    ARC_COLON_BIDIR,         ///<An mscgen bidirectional arrow with colon: <:> (ArcDirArrow, ArcSelfArrow, ArcBigArrow, ArcVertical)
    ARC_UNDETERMINED_SEGMENT,///<An arrow segment of undefined type: - (ArcDirArrow, ArcBigArrow)
    BOX_SOLID,               ///<A solid box: -- (ArcBox, ArcPipe, ArcVertical)
    BOX_DOTTED,              ///<A dotted box: .. (ArcBox, ArcPipe, ArcVertical)
    BOX_DASHED,              ///<A dashed box: ++ (ArcBox, ArcPipe, ArcVertical)
    BOX_DOUBLE,              ///<A double box: == (ArcBox, ArcPipe, ArcVertical)
    BOX_UNDETERMINED_FOLLOW, ///<A (subsequent) box in a box segment, without a type specifier (ArcBox)
    DIV_DISCO,               ///<A discontinuity in time line: ... (ArcDivider)
    DIV_DIVIDER,             ///<A divider: --- (ArcDivider)
    DIV_VSPACE,              ///<No arc, just space (maybe with label) (ArcDivider)
    DIV_NUDGE,               ///<The nudge command
    DIV_TITLE,               ///<The title command
    DIV_SUBTITLE,            ///<The subtitle command
};
inline bool IsArcSymbolArrow(EArcSymbol t) { return t<=EArcSymbol::ARC_UNDETERMINED_SEGMENT && t>EArcSymbol::INVALID; }
inline bool IsArcSymbolBidir(EArcSymbol t) { _ASSERT(IsArcSymbolArrow(t));  return (unsigned(t)&1) == 0 && t<EArcSymbol::ARC_UNDETERMINED_SEGMENT && t>EArcSymbol::INVALID; }
inline bool IsArcSymbolBox(EArcSymbol t) { return t<=EArcSymbol::BOX_UNDETERMINED_FOLLOW && t>=EArcSymbol::BOX_SOLID; }
inline bool IsArcSymbolDivider(EArcSymbol t) { return t>=EArcSymbol::DIV_DISCO; }
inline EArcSymbol ConvertBoxSymbol2ArrowSymbol(EArcSymbol t, bool bidir) { _ASSERT(IsArcSymbolBox(t));  return EArcSymbol(((unsigned)t-9)*2 + unsigned(bidir)); }

/** The direction of an arrow in the text file */
enum class EDirType {
    INDETERMINATE, ///<An arrow segment of unspecified direction
    RIGHT,         ///<An arrow pointing left-to-right. This is the forward direction.
    LEFT,          ///<An arrow pointing right-to-left. This is the reverse direction.
    BIDIR          ///<A bidirectional arrow.
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
    EArcSymbol type;              ///<The type of the arrow such as ARC_SOLID or BOX_DASHED
    EArrowLost lost;              ///<Whether there was a loss indication (*) or not.
    PODFileLineColRange lost_pos; ///<The location of the asterisk if any.
};

/** A structure used to hold an arrow type & loss and a direction. Used with ArcVerticalArrow. */
struct ArcTypePlusDir
{
    ArrowSegmentData arc;
    EDirType dir;
};

class EntityDistanceMap;
class DistanceMapVertical;
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
    double vspacing;       ///<Contains the spacing *above* this arc. DBL_MIN if compress mechanism is on for this arc (compress attribute set)
    bool   parallel;       ///<If true, subsequent arcs can be drawn beside this.
    bool   overlap;        ///<If true, subsequent arcs can be drawn overlaid on this.
    bool   keep_together;  ///<If true, do not split this by automatic pagination.
    bool   keep_with_next; ///<If true, do not separate this from following element by automatic pagination 
    string refname;        ///<Value of the "refname" attribute, to reference numbers & others. Empty if none.
    mutable double  height;///<Pixel height of the arc, calculated by Layout()
    static void AttributeNamesSet1(Csh &csh);
    static bool AttributeValuesSet1(const std::string attr, Csh &csh);
public:
    const MscProgress::ECategory myProgressCategory; ///<The category of the arc for calculating progress.

    /** Basic constructor, taking const members and the reference to Msc we are member of.*/
    ArcBase(MscProgress::ECategory c, Msc *msc);
    /** A virtual destructor */
	~ArcBase() override;
    /** Ture if object is valid */
    bool IsValid() const {return valid;}
    /** True if subsequent arcs can be drawn beside this.*/
    bool IsParallel() const {return parallel;}
    /** Set the parallel member to true. Used only during parsing.*/
    void SetParallel() {parallel = true;}
    /** True if subsequent arcs can be drawn overlaid on top of this.*/
    bool IsOverlap() const {return overlap;}
    /** Set the overlay member to true. Used only during parsing.*/
    void SetOverlap() {overlap = true;}
    /** True, if compress mechanism is on foe this arc and can be shifted upwards.*/
    bool IsCompressed() const { return vspacing==DBL_MIN; }
    /** The value of extra space to be added above this arc. Zero if compress is used.*/
    double GetVSpacing() const { return vspacing==DBL_MIN ? 0 : vspacing; }
    /** True if this arc shall be placed at the centerline of a previous one.*/
    bool IsKeepTogether() const {return keep_together;}
    /** If true, do not separate this from following element by automatic pagination */
    bool IsKeepWithNext() const {return keep_with_next;}
    /** True if a note can be attached to this arc. */
    virtual bool CanBeNoted() const {return false;}
    /** True if the entity is likely to produce height and can be the target of a vertical*/
    virtual bool CanBeAlignedTo() const { return false; }
    /** True if implicitly generated entities (Msc::AutoGenEntities) shall be placed *after* this element (like titles, etc) */
    virtual bool BeforeAutoGenEntities() const { return false; }
    /** Get the Y coordinate of the top of the arc. 
     * The actual visual part may not start here, 
     * if we have gap proscribed above in 'chart'
     * However, VSpacing is above this position.*/
    double GetFormalPos() const {return yPos;}
    /** Return the height of the element with its side comments - 
     * the this may be more than the visual height as the arc may have
     * gap above and/or below proscribed by the 'chart'.*/
    double GetFormalHeight() const {return std::max(height, comment_height);}
    /** Get the Y coordinate range occupied by the arc.
     * This may be wider than the actual visual effect and is used for
     * layout and to gauge associated verticals.*/
    virtual Range GetFormalYExtent() const {return Range(yPos, yPos+GetFormalHeight());}
    /** Get the Y coordinate range actually occupied by visual elements of the arc. 
     * This is used to do automatic pagination. */
    virtual Range GetVisualYExtent(bool include_comments) const;
    /** Get an (ordered) list of entities that this arrow or box touches.
      * @return the direction of the arrows inside (left is all arrows are left; bidir if mixed.*/
    virtual EDirType GetToucedEntities(EntityList &) const {return EDirType::INDETERMINATE;}
    /** Expands our contour calculated as `a` to a cover used for compress bump checking*/
    Area GetCover4Compress(const Area &a) const;
    /** Allocates and fills in a signature object identifying the arc. NULL if no signature available.*/
    virtual const ArcSignature* GetSignature() const {return NULL;}

    /** Applies a set of attributes to us.*/
    virtual void AddAttributeList(AttributeList *);
    /** Applies one attribute to us */
    bool AddAttribute(const Attribute &) override;
    /** Add attribute names and helpers to csh for hinting */
    static void AttributeNames(Csh &csh);
    /** Add attribute values and helpers to csh for hinting */
    static bool AttributeValues(const std::string attr, Csh &csh);

    /** Add the current activation status to the last element in 'vdist'. Used by Width().*/
    void AddEntityLineWidths(DistanceMapVertical &vdist);

    /** Lay out our comments & return their combined coverage in `cover`*/
    /** @name Recursive processors
     * These functions are called recursively for all arcs in this order*/
    /** @{ */
    /** Calculate things  after parsing.
     * Called after parsing and adding attributes. 
     * Entity order and collapse/expand is already known here 
     * See documentation for libmscgen for more info.*/
    virtual ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                      Numbering &number, Element **note_target, ArcBase *vertical_target);
    /** Substitute name references in labels & process all escapes 
     * See documentation for libmscgen for more info.*/
    virtual void FinalizeLabels(Canvas &canvas);
    /** Collects vertical distances for hscale=auto mechanism.
     * See documentation for libmscgen for more info.*/
    virtual void Width(Canvas &/*canvas*/, EntityDistanceMap &/*distances*/, DistanceMapVertical &/*vdist*/) {}
    /** Calculate entity layout at y=0 vertical position. 
     * Calculates the height, and sets up the area at yPos==0, 
     * add its cover to use at placement later to `cover`.
     * Cover or area does not include any spacing left around such as chart->emphVGapAbove.
     * See documentation for libmscgen for more info.*/
    virtual void Layout(Canvas &canvas, AreaList *cover);
    /** Move the arc up or down. This can be called multiple times. */
    void ShiftBy(double y) override {if (valid) {Element::ShiftBy(y);}}
    /** Collect the y position of page breaks into Msc::pageBreakData. 
     * See documentation for libmscgen for more info.*/
    virtual void CollectPageBreak() {}
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
    virtual void PlaceWithMarkers(Canvas &/*canvas*/) {}
    /** Emit warnings that need final layout. 
     * See documentation for libmscgen for more info.*/
    void PostPosProcess(Canvas &canvas) override;
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
    bool CanBeAlignedTo() const override { return true; }
    /** True if `src` and `dst` are properly set.*/
    bool IsComplete() const;
    void SetEntities(EIterator s, EIterator d) {src=s; dst=d;}
    /** Expand the `file_pos` member with that of another interator if at same location. 
     * @returns false, if `src` or `dst` differ (and then no change to `file_pos`.*/
    bool Combine(const ArcIndicator *o);
    /** Returns the x coordinate in chart space of the middle of the indicator */
    double GetXCoord() const;
    EDirType GetToucedEntities(class EntityList &el) const override;
    void Width(Canvas &canvas, EntityDistanceMap &distances, DistanceMapVertical &vdist) override;
    void Layout(Canvas &canvas, AreaList *cover) override;
    void Draw(Canvas &canvas, EDrawPassType pass) override;
};

/** Root class for all arcs having a label.
 * Contains style, label and numbering fields.*/
class ArcLabelled : public ArcBase
{
protected:
    string          label;           ///<The label as the user specified it. Empty if none.
    FileLineCol     linenum_label;   ///<The position of the label in the input file.
    Label           parsed_label;    ///<The label processed and parsed into lines. Set in PostParseProcess()
    int             concrete_number; ///<Negative, if the user specified no specific value for the number (no numbering or automatic). Else the number specified.
    string          url;             ///<The value of the 'url' attribute, empty if none.
    FileLineCol     linenum_url_attr;///<The location of the 'url' attribute, if any.
    string          id;              ///<The value of the 'id' attribute;
    string          idurl;           ///<The value of the 'idurl' attribute;
    FileLineCol     linenum_idurl_attr;///<The location of the 'idurl' attribute, if any.
    StyleCoW        style;           ///<The style of the arc. `numbering` and `compress` fields of style are not used. The ArcBase member fields are used instead.
    NumberingStyle  numberingStyle;  ///<The numbering style to use. (This is not part of Styles in general, but is a property of contexts. This is a snapshot at the location of the arc.
    string          number_text;     ///<The formatted number assigned to this arc (used by references and notes/comments). Set in PostParseProcess()
    Range           entityLineRange; ///<The y range to apply any vFill or vLine attributes
public:
    ArcLabelled(MscProgress::ECategory c, Msc *msc);
    ArcLabelled(MscProgress::ECategory c, const ArcLabelled &al);
    const StyleCoW &GetStyle() const {return style;}
    bool CanBeNoted() const override { return true; }
    bool CanBeAlignedTo() const override { return true; }
    /** Set style to this name, but combine it with default text style */
    void SetStyleWithText(const char *style_name, const StyleCoW *refinement);
    /** Set style to this name, but combine it with default text style. Use existing style if NULL.*/
    void SetStyleWithText(const StyleCoW *style_to_use, const StyleCoW *refinement);
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
    virtual void SetStyleBeforeAttributes() = 0;
    FileLineCol AddAttributeListStep1(AttributeList *);
    void AddAttributeListStep2(const FileLineCol &label_pos);
    void AddAttributeList(AttributeList *l) override {SetStyleBeforeAttributes(); AddAttributeListStep2(AddAttributeListStep1(l)); }
    bool AddAttribute(const Attribute &) override;
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                              Numbering &number, Element **note_target, ArcBase *vertical_target) override;
    /** Return the formatted number of the arc (empty string if none).*/
    const string &GetNumberText() const {return number_text;}
    /** Return the numbering style of the arc (even if numbering is turned off).*/
    const NumberingStyle &GetNumberingStyle() const {return numberingStyle;}
    void FinalizeLabels(Canvas &canvas) override;
    void PostPosProcess(Canvas &canvas) override;
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
struct VertXPos
{
    /** AT is at the entity
    ** CENTER is between the two entities
    ** BY is immediately at the line, but not overlapping it
    ** SIDE is immediately at the line, arrow tips overlap the line */
    /** Options for horizontal position in relation to one or two entities */
    enum EPosType
    {
        POS_INVALID = 0,///<Invalid value
        POS_AT,         ///<At the entity
        POS_CENTER,     ///<Halfway between two entities
        POS_THIRD_LEFT, ///<left third between two entities
        POS_THIRD_RIGHT,///<left third between two entities
        POS_LEFT_BY,    ///<Immediately left by the entity line, but not overlapping it.
        POS_LEFT_SIDE,  ///<immediately left by the line, arrow tips overlap the line.
        POS_RIGHT_BY,   ///<Immediately right by the entity line, but not overlapping it.
        POS_RIGHT_SIDE  ///<immediately right by the line, arrow tips overlap the line.
    };

    bool valid;               ///<True if the position is valid
    EPosType pos;             ///<The type of the position
    EIterator entity1;        ///<The first (or only one) entity
    EIterator entity2;        ///<The second entity, used only for POS_CENTER
    FileLineColRange e1line;  ///<Location of the mention of the first entity in the input line (inside this vertpos fragment)
    FileLineColRange e2line;  ///<Location of the mention of the second entity in the input line (inside this vertpos fragment)
    double offset;            ///<An arbitrary user specified horizontal offset on top of the poition defined by `pos`

    /** Create a position with 2 entities (always POS_CENTER) */
    VertXPos(Msc&m, const char *e1, const FileLineColRange &e1l, const char *e2, const FileLineColRange &e2l, double off = 0, EPosType p = POS_CENTER);
    /** Create a position with one entity (must not be POS_CENTER) */
    VertXPos(Msc&m, const char *e1, const FileLineColRange &e1l, EPosType p = POS_AT, double off = 0);
    /** Create (yet) invalid object */
    explicit VertXPos(Msc&m);
    /** Calculate the x coordinate of the middle of the element we specify the location for (only after entities are placed)
    * @param m The chart
    * @param width The width of the object
    * @param gap1 Specifies how much gap between the entity and object for POS_XXX_SIDE. Defaults to `m.hscaleAutoXGap`.
    * @param gap2 Specifies how much extra gap between the entity and the object for POS_XXX_BY compared to POS_XXX_SIDE. Defaults to `m.hscaleAutoXGap`*/
    double CalculatePos(Msc &m, double width = 0, double gap1 = -1, double gap2 = -1) const;
};

/** The base class for all arrows: Self, Dir, Block and Vertical*/
class ArcArrow : public ArcLabelled
{
protected: 
    bool headless_mscgen_arrow; ///<True if this is an mscgen syntax arc defined without arrowhead via one of (--, ==, .., ::)
public:
    const bool bidir;
    /** This is the typical constructor */
    ArcArrow(bool b, MscProgress::ECategory c, Msc *msc) : ArcLabelled(c, msc), headless_mscgen_arrow(false), bidir(b){}
    /** Constructor used only when converting an ArcBox to and ArcBigArrow*/
    ArcArrow(bool b, MscProgress::ECategory c, const ArcLabelled &al) : ArcLabelled(c, al), headless_mscgen_arrow(false), bidir(b) {}
    /** Get the refinement style for a given arrow symbol 
     * The ArcArrow:: version returns the style for self and dir arrows, ArcBigArrow overrides. */
    virtual const StyleCoW *GetRefinementStyle4ArrowSymbol(EArcSymbol t) const;
    /** Add a new arrow segment. Called during processing.
     * @param [in] data The type of the new arrow segment, indication of amy 
     *                  potential loss indication and its location.
     * @param [in] m The name of the entity (can be NULL when omitted by user)
     * @param [in] ml The location of the name in the input file.
     * @param [in] l The location of the whole added segment (for error messages)*/
    virtual ArcArrow *AddSegment(ArrowSegmentData data, const char *m, const FileLineColRange &ml, 
                                 const FileLineColRange &l) = 0;
    virtual ArcArrow *AddLostPos(VertXPos *pos, const FileLineColRange &l) = 0;
    /** Indicate that this arrow was defined via an mscgen headless arrow symbol (--, ==, ., ::)*/
    void Indicate_Mscgen_Headless() { headless_mscgen_arrow = true; }
    bool AddAttribute(const Attribute &) override;
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    void PostPosProcess(Canvas &canvas) override;
    void DrawLSym(Canvas &canvas, const XY &C, XY size);
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
    const EArcSymbol type;
    ArcSelfArrow(EArcSymbol t, const char *s, const FileLineColRange &sl,
                 Msc *msc, double ys);
    ArcArrow *AddSegment(ArrowSegmentData data, const char *m, const FileLineColRange &ml,
                         const FileLineColRange &l) override;
    ArcArrow *AddLostPos(VertXPos *pos, const FileLineColRange &l) override;
    EDirType GetToucedEntities(EntityList &el) const override;
    void SetStyleBeforeAttributes() override;
    ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                              Numbering &number, Element **note_target, ArcBase *vertical_target) override;
    void Width(Canvas &canvas, EntityDistanceMap &distances, DistanceMapVertical &vdist) override;
    void Layout(Canvas &canvas, AreaList *cover) override;
    void PostPosProcess(Canvas &canvas) override;
    void RegisterLabels() override;
    void CollectIsMapElements(Canvas &canvas) override;
    void Draw(Canvas &canvas, EDrawPassType pass) override;
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
    std::vector<EArcSymbol>  segment_types;        ///<Types of segments: one for each segment ([0] is the one from src). Set during AddSegment() calls.
    std::vector<LineAttr>    segment_lines;        ///<Line types of segments. Set during AddAttributeList() from `segment_types`.
    const bool               specified_as_forward; ///<True if user specified "a->b", false if "b<-a"
    double                   slant_angle;          ///<The angle of the arrow. Taken from the context, may be modified by style and/or attribute.
    double                   slant_depth;          ///<The value of the slant depth, an alternative to the slant arrow. Used only if the user specified the "slant_depth" attr.
    int                      lost_at;              ///<Index of where the message was lost: -2=none, -1=src, 0..n = at a midpoint, n=dst; where n is the size of 'middle'.
    bool                     lost_is_forward;      ///<True if the loss was at the fw side of 'lost_at', false if at the backwards size
    FileLineCol              linenum_asterisk;     ///<The line and column number of the asterisk marking the loss. Invalid if none.
    VertXPos                 lost_pos;             ///<The position of the loss symbol
    FileLineCol              linenum_lost_at;      ///<The line and column number of the 'lost at' clause marking the loss. Invalid if none.
    static const double      lsym_side_by_offset;  ///<if 'lost at a++' or 'lost at a--' is specified, we add the size of the loss symbol times this much extra offset compared to 'lost at a+/-'

    mutable double sin_slant; ///<Pre-calculated value of `slant_angle`
    mutable double cos_slant; ///<Pre-calculated value of `slant_angle`
    mutable double sx;        ///<Calculated value of the start arrowhead (sx can be > dx)
    mutable double dx;        ///<Calculated value of the end arrowhead (sx can be > dx)
    mutable double sx_text;   ///<Calculated value of the left of the label (usually sx adjusted by arrowhead)
    mutable double dx_text;   ///<Calculated value of the right of the label (usually dx adjusted by arrowhead)
    mutable double cx_text;   ///<Calculated value of the middle of the label (usually middle of sx and dx)
    mutable double cx_lsym;   ///<Calculated value of the middle of the loss symbol (if any)
    mutable XY lsym_size;     ///<Calculates size of the lost symbol (if any loss)
    mutable std::vector<double> xPos;        ///<X coordinates of arrowheads (sorted to increase, [0] is for left end of the arrow). Always the middle of the entity line.
    mutable std::vector<double> act_size;    ///<Activation size of the entities (sorted from left to right). ==0 if not active
    mutable std::vector<DoublePair> margins; ///<How much of the arrow line is covered by the arrowhead (sorted left to right), see ArrowHead::getWidths()
    mutable Contour text_cover; ///<The cover of the label, so that we can hide entity lines in PostPosProcess() - not used by ArcBigArrow
    mutable double centerline;  ///<y offset of line compared to yPos
    mutable Contour clip_area;  ///<Used to mask out the line at arrowheads - not used by ArcBigArrow.
public:
    /** Regular constructor, does pretty much nothing. We need to add segments.
     * @param data The type of the arrow and the position of any loss indication (*).
     * @param s The name of the source entity.
     * @param sl The location of the name of the source entity in the input file
     * @param d The name of the destination entity
     * @param dl The location of the name of the destination entity in the input file
     * @param msc The char we add the arrow to
     * @param fw True if declared as a->b and false if b<-a*/
    ArcDirArrow(ArrowSegmentData data, 
               const char *s, const FileLineColRange &sl,
               const char *d, const FileLineColRange &dl, 
               Msc *msc, bool fw);
    /** Constructor used while converting a collapsed ArcBox to an ArcBigArrow*/
    ArcDirArrow(const EntityList &el, bool bidir, EArcSymbol boxsymbol, const ArcLabelled &al);
    ArcArrow *AddSegment(ArrowSegmentData data, const char *m, const FileLineColRange &ml,
                         const FileLineColRange &l) override;
    ArcArrow *AddLostPos(VertXPos *pos, const FileLineColRange &l) override;
    void SetStyleBeforeAttributes() override;
    void AddAttributeList(AttributeList *l) override;
    bool AddAttribute(const Attribute &) override;
    virtual EDirType GetToucedEntities(EntityList &el) const;
    ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                              Numbering &number, Element **note_target, ArcBase *vertical_target) override;
    /** Update the stored activation status of entities.
     * Called if a centierlined entity command after us has changed active entities.*/
    virtual void UpdateActiveSizes(); 
    void FinalizeLabels(Canvas &canvas) override;
    void Width(Canvas &canvas, EntityDistanceMap &distances, DistanceMapVertical &vdist) override;
    void Layout(Canvas &canvas, AreaList *cover) override;
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
    void ShiftBy(double y) override;
    /** Check if the entities in `src` - `middle[]` - `dst` are all left-to-right or right-to-left*/
    void CheckSegmentOrder(double y);
    void PostPosProcess(Canvas &canvas) override;
    void RegisterLabels() override;
    void CollectIsMapElements(Canvas &canvas) override;
    void DrawArrow(Canvas &canvas, const Label &loc_parsed_label,
                   const std::vector<LineAttr>& loc_segment_lines,
                   const ArrowHead &loc_arrow);
    void Draw(Canvas &canvas, EDrawPassType pass) override;
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
    ArcBigArrow(const ArcDirArrow &);
    /** Used when converting a collapsed ArcBox to an ArcBigArrow */
    ArcBigArrow(const EntityList &, bool bidir, EArcSymbol boxsymbol, const ArcLabelled &al, const ArcSignature *s);
    ~ArcBigArrow() override { if (sig) delete sig; }
    void SetStyleBeforeAttributes() override;
    const StyleCoW *GetRefinementStyle4ArrowSymbol(EArcSymbol t) const override;
    const ArcSignature* GetSignature() const override { return sig; }
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                              Numbering &number, Element **note_target, ArcBase *vertical_target) override;
    void FinalizeLabels(Canvas &canvas) override;
    void Width(Canvas &canvas, EntityDistanceMap &distances, DistanceMapVertical &vdist) override;
    void Layout(Canvas &canvas, AreaList *cover) override;
    Range GetVisualYExtent(bool include_comments) const override;
    void ShiftBy(double y) override;
    void PostPosProcess(Canvas &canvas) override;
    void RegisterLabels() override;
    void CollectIsMapElements(Canvas &canvas) override;
    void Draw(Canvas &canvas, EDrawPassType pass) override;
};

/** A vertical */
class ArcVerticalArrow : public ArcArrow
{
public:
    /** Lists Vertical shapes */
    enum EVerticalShape {
        ARROW_OR_BOX, ///<A block arrow or a box
        BRACE,        ///<A curly brace
        BRACKET,      ///<A square bracket (potentially with custom corners)
        RANGE,        ///<A range with potential arrow
        POINTER,      ///<An arrow that looks like a pointer to self
    };
protected:
    const EArcSymbol type;   ///<Tells us what kind of symbol was used to define us (-> or -- etc)
    string src;              ///<The top marker (if any)
    string dst;              ///<The bottom marker (if any)
    const bool lost;         ///<Whether we had a loss symbol or not
    const FileLineCol lost_pos; ///<The position of the loss symbol (if any)
    EVerticalShape shape;    ///<The shape of the vertical
    VertXPos pos;            ///<The horizontal position, specified by the user
    WidthAttr width_attr;    ///<The value of the width attribute
    const ArcBase *prev_arc; ///<The arc before us. Used only if no markers specified.
    mutable bool forward;    ///<if the arrow is up to down.
    mutable bool left;       ///<True if we are at the left side of the entity (or we draw such)
    mutable double width;    ///<width of us (just the body for arrows)
    mutable double ext_width;///<Extra width of the arrowhead
    mutable double radius;   ///<Calculated radius (important for brace, where height may limit)
    mutable XY lsym_size;     ///<Calculates size of the lost symbol (if any loss)
    mutable std::vector<double> ypos; ///<A two element array with the y pos of the start and end. Set in PlaceWithMarkers()
    mutable double s_text;   ///<Left/Top/Bottom edge of text for side==END/LEFT/RIGHT. An y coordinate for LEFT/RIGHT, an x for END
    mutable double d_text;   ///<Right/Bottom/Top edge of text for side==END/LEFT/RIGHT. An y coordinate for LEFT/RIGHT, an x for END
    mutable double t_text;   ///<Top/Right/Left edge of text for side==END/LEFT/RIGHT. An x coordinate for LEFT/RIGHT, an y for END
    mutable double xpos;     ///<x coordinate of middle (ARROW_OR_BOX) or the key line (BRACE, BRACKET, RANGE)
    mutable double vline_off;///<The distance of the edge of the construct facing the entity line from the main vertical line (signed value), only used for brace/pointer/range/bracket
    mutable Contour clip_line;  ///<The clip of the line for ranges, brackets and pointers
    mutable std::vector<Contour> outer_contours; ///<Calculated contour (only one element)
public:
    /** Constructor for arrow or box symbols with two marker names (one can be NULL)*/
    ArcVerticalArrow(ArcTypePlusDir t, const char *s, const char *d, Msc *msc);
    ArcArrow *AddSegment(ArrowSegmentData data, const char *m, const FileLineColRange &ml,
                         const FileLineColRange &l) override;
    ArcArrow *AddLostPos(VertXPos *pos, const FileLineColRange &l) override;
    /** Add the parsed horizontal position (starting with the AT keyword) */
    ArcVerticalArrow* AddXpos(VertXPos *p);
    /** Sets the shape of the vertical. Used only in parsing.*/
    void SetVerticalShape(EVerticalShape sh);
    bool CanBeNoted() const override { return false; }
    bool CanBeAlignedTo() const override { return false; }
    void SetStyleBeforeAttributes() override;
    const StyleCoW *GetMyRefinementStyle() const;
    bool AddAttribute(const Attribute &) override;
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                              Numbering &number, Element **note_target, ArcBase *vertical_target) override;
    void Width(Canvas &canvas, EntityDistanceMap &distances, DistanceMapVertical &vdist) override;
    void Layout(Canvas &canvas, AreaList *cover) override;
    Range GetVisualYExtent(bool include_comments) const override;
    void ShiftBy(double y) override;
    Range GetFormalYExtent() const override { return Range(false); }
    double SplitByPageBreak(Canvas &/*canvas*/, double /*netPrevPageSize*/,
                            double /*pageBreak*/, bool &/*addCommandNewpage*/, 
                            bool /*addHeading*/, ArcList &/*res*/) override {return -2;}
    void PlaceWithMarkers(Canvas &canvas) override;
    void PostPosProcess(Canvas &canvas) override;
    void RegisterLabels() override;
    void CollectIsMapElements(Canvas &canvas) override;
    void DrawBraceLostPointer(Canvas &canvas, const LineAttr &line, const ArrowHead &arrow);
    void Draw(Canvas &canvas, EDrawPassType pass) override;
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
    string           tag_label;       ///<The tag label as the user specified it. Empty if none.
    Label            parsed_tag_label;///<The tag label processed and parsed into lines. Set in PostParseProcess()
    EBoxCollapseType collapsed;       ///<The collapseStatus of the box.
    bool             drawEntityLines; ///<True if we draw the entity lines. Only for boxes with content: empty boxes never draw lines
    ArcList          content;         ///<The arcs contained within the box.

    mutable double height_w_lower_line; ///<Height of this box including its lower line
    mutable double sx_text;             ///<Left boundary of label
    mutable double dx_text;             ///<Right boundary of label
    mutable double y_text;              ///<Top boundary of label
    mutable Contour text_cover;         ///<Area covered by the label
    mutable double sx_tag;              ///<Left boundary of tag label (if any)
    mutable double dx_tag;              ///<Right boundary of tag label (if any)
    mutable double y_tag;               ///<Top boundary of tag label (if any) 
    mutable Block tag_outer_edge;       ///<The outer edge of the tag rectange (must be clipped by the inner edge of the box)
    mutable Contour tag_cover;          ///<Area covered by the tag (not just the label of it) - you can use it to clip
public:
    const EArcSymbol type;
    const enum Emscgen_compat {
        MSCGEN_COMPAT_NONE, MSCGEN_COMPAT_BOX, MSCGEN_COMPAT_ABOX, MSCGEN_COMPAT_RBOX, MSCGEN_COMPAT_NOTE
    } mscgen_compat;
    ArcBox(EArcSymbol t, const char *s, const FileLineColRange &sl,
           const char *d, const FileLineColRange &dl, Msc *msc);
    ArcBox(Emscgen_compat c, const char *s, const FileLineColRange &sl,
           const char *d, const FileLineColRange &dl, Msc *msc);
    bool CanBeNoted() const override { return true; }
    const ArcSignature* GetSignature() const override;
    const StyleCoW *GetRefinementStyle4BoxSymbol(EArcSymbol t) const;
    virtual void SetStyleBeforeAttributes() {} //we do it in AddAttributeList() below
    ArcBox* AddArcList(ArcList*l);
    void AddAttributeList(AttributeList *) override;
    bool AddAttribute(const Attribute &) override;
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                              Numbering &number, Element **note_target, ArcBase *vertical_target) override;
    void FinalizeLabels(Canvas &canvas) override;
    void Layout(Canvas &/*canvas*/, AreaList * /*cover*/) override { _ASSERT(0); }
    void ShiftBy(double y) override;
    void RegisterCover(EDrawPassType /*pass*/) override { _ASSERT(0); } //will never be called
    void Draw(Canvas &/*canvas*/, EDrawPassType /*pass*/) override { _ASSERT(0); } //will never be called
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
    bool CanBeAlignedTo() const override { return true; }
    /** Add the first and append subsequent boxes to the series */
    ArcBoxSeries* AddBox(ArcBox *f);
    EDirType GetToucedEntities(EntityList &el) const override;
    ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                              Numbering &number, Element **note_target, ArcBase *vertical_target) override;
    void FinalizeLabels(Canvas &canvas) override;
    void Width(Canvas &canvas, EntityDistanceMap &distances, DistanceMapVertical &vdist) override;
    void Layout(Canvas &canvas, AreaList *cover) override;

    Range GetVisualYExtent(bool include_comments) const override;

    void ShiftBy(double y) override;
    void CollectPageBreak() override;
    double SplitByPageBreak(Canvas &canvas, double netPrevPageSize,
                            double pageBreak, bool &addCommandNewpage, 
                            bool addHeading, ArcList &res) override;
    void PlaceWithMarkers(Canvas &canvas) override;
    void PostPosProcess(Canvas &canvas) override;
    void RegisterLabels() override;
    void CollectIsMapElements(Canvas &canvas) override;
    void RegisterCover(EDrawPassType pass) override;
    void Draw(Canvas &canvas, EDrawPassType pass) override;
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
    const EArcSymbol type;
    /** Upgrade a box to a pipe: this is how we create pipes normally.
     * Deletes 'box'*/
    ArcPipe(ArcBox *box);
    bool CanBeNoted() const override { return true; }
    const StyleCoW *GetRefinementStyle4PipeSymbol(EArcSymbol t) const;
    void SetStyleBeforeAttributes() override;
    bool AddAttribute(const Attribute &) override;
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    void Layout(Canvas &/*canvas*/, AreaList * /*cover*/) override { _ASSERT(0); }
    void ShiftBy(double y) override;
    void RegisterCover(EDrawPassType /*pass*/) override {_ASSERT(0); } //will never be called
    /** Helper to draw various parts of the pipe */
    void DrawPipe(Canvas &canvas, EDrawPassType pass, bool topSideFill, bool topSideLine,  
                  bool backSide, bool shadow, bool text, double next_lw, int drawing_variant);
    void Draw(Canvas &/*canvas*/, EDrawPassType /*pass*/) override { _ASSERT(0); } //will never be called
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
    bool CanBeAlignedTo() const override { return true; }
    EDirType GetToucedEntities(EntityList &el) const override;
    ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                              Numbering &number, Element **note_target, ArcBase *vertical_target) override;
    void FinalizeLabels(Canvas &canvas) override;
    void Width(Canvas &canvas, EntityDistanceMap &distances, DistanceMapVertical &vdist) override;
    void CalculateContours(Area *pipe_body_cover = NULL);
    void Layout(Canvas &canvas, AreaList *cover) override;
    Range GetVisualYExtent(bool include_comments) const override;
    void ShiftBy(double y) override;
    void CollectPageBreak() override;
    double SplitByPageBreak(Canvas &canvas, double netPrevPageSize,
                            double pageBreak, bool &addCommandNewpage, 
                            bool addHeading, ArcList &res) override;
    void PlaceWithMarkers(Canvas &canvas) override;
    void PostPosProcess(Canvas &canvas) override;
    void RegisterLabels() override;
    void CollectIsMapElements(Canvas &canvas) override;
    void RegisterCover(EDrawPassType pass) override;
    void Draw(Canvas &canvas, EDrawPassType pass) override;
};


/** A divider, gap, title, subtitle or nudge command */
class ArcDivider : public ArcLabelled
{
protected:
    const EArcSymbol type;     ///<What kind of divider symbol was used to define us
    const bool nudge;          ///<True if created from a `nudge` command
    const bool title;          ///<True if created from a `title` or `subtitle` command
    bool wide;                 ///<If true, we keep no margin and add no gap above & below (for copyright text)
    const double extra_space;  ///<Extra vertical space above and below: depends on how we created this object

    mutable double centerline; ///<Y coordinate of the centerline
    mutable double text_margin;///<left and right text margin from chart edge
    mutable double line_margin;///<left and right line margin from chart edge
    mutable Contour text_cover;///<The cover of the label
public:
    ArcDivider(EArcSymbol t, Msc *msc);
    bool CanBeAlignedTo() const override { return !nudge; } //nudges cannot be targets of a vertical
    bool BeforeAutoGenEntities() const override { return title; }
    void SetStyleBeforeAttributes() override;
    bool AddAttribute(const Attribute &) override;
    static void AttributeNames(Csh &csh, bool nudge, bool title);
    static bool AttributeValues(const std::string attr, Csh &csh, bool nudge, bool title);
    ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                              Numbering &number, Element **note_target, ArcBase *vertical_target) override;
    void Width(Canvas &canvas, EntityDistanceMap &distances, DistanceMapVertical &vdist) override;
    void Layout(Canvas &canvas, AreaList *cover) override;

    void ShiftBy(double y) override;
    void PostPosProcess(Canvas &canvas) override;
    void RegisterLabels() override;
    void CollectIsMapElements(Canvas &canvas) override;
    void Draw(Canvas &canvas, EDrawPassType pass) override;
};

/** Multiple parallel blocks */
class ArcParallel : public ArcBase
{
public:
    /** Describes the three layout algoritm for parallel blocks.*/
    enum EParallelLayoutType
    {
        INVALID_LAYOUT = 0,
        OVERLAP,          ///<Lay out each block independently and allow overlaps
        ONE_BY_ONE,       ///<Lay out elements from each block one by one and avoid overlap
        ONE_BY_ONE_MERGE, ///<Lay out elements from each block one by one and avoid overlap, merge with parallel block around us
    };
    /** Specify vertical alignmen. */
    enum EVerticalIdent
    {
        NOT_SET = 0,
        TOP,    ///<Align on top
        MIDDLE, ///<Align by middle
        BOTTOM, ///<Align at bottom
    };
protected:
    /** Represents one parallel block */
    struct Column {
        ArcList arcs;          ///<The list of arcs in this column
        EVerticalIdent ident;  ///<For OVERLAP layout, how to ident this block.
        FileLineCol ident_pos; ///<Location of the vertical_ident attribute
        double height;         ///column height
        Column(ArcList &&a, EVerticalIdent i) : arcs(std::move(a)), ident(i), height(0) { ident_pos.MakeInvalid(); }
        bool IsEmpty() const { return arcs.size()==0; }
    };
    friend class Msc; //to manage our layout in LayoutParallelArcLists()
    void SetYPos(double y) { yPos = y; }
    void SetBottom(double y) { height = y-yPos; }
    std::list<Column> blocks;       ///<Arc lists, one for each block
public:
    const bool before_autogen_entities;///<True if this is a single-block series defined e.g., to return chart options. Auto headings jump over these.
    const bool unroll;                 ///<True if this is a single-block series defined internally and PostParseProcessArcList() shall unroll these.
    EParallelLayoutType layout;        ///<The layout method
    /** Create a parallel construct from its first block */
    ArcParallel(Msc *msc, ArcList*l, AttributeList *al, bool before, bool u);
    bool CanBeAlignedTo() const override { return true; }
    bool BeforeAutoGenEntities() const override { return before_autogen_entities; }
    /** Add one more parallel block */
    ArcParallel* AddArcList(ArcList*l, AttributeList *al);
    bool AddAttribute(const Attribute &) override;
    static void AttributeNames(Csh &csh, bool first);
    static bool AttributeValues(const std::string attr, Csh &csh, bool first);
    EDirType GetToucedEntities(EntityList &el) const override;
    ArcBase* PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                              Numbering &number, Element **note_target, ArcBase *vertical_target) override;
    void FinalizeLabels(Canvas &canvas) override;
    void Width(Canvas &canvas, EntityDistanceMap &distances, DistanceMapVertical &vdist) override;
    void Layout(Canvas &canvas, AreaList *cover) override;
    Range GetVisualYExtent(bool include_comments) const override;

    void ShiftBy(double y) override;
    void CollectPageBreak() override;
    double SplitByPageBreak(Canvas &canvas, double netPrevPageSize,
                            double pageBreak, bool &addCommandNewpage, 
                            bool addHeading, ArcList &res) override;
    void PlaceWithMarkers(Canvas &canvas) override;
    void PostPosProcess(Canvas &canvas) override;
    void RegisterLabels() override;
    void CollectIsMapElements(Canvas &canvas) override;
    void RegisterCover(EDrawPassType pass) override;
    void Draw(Canvas &canvas, EDrawPassType pass) override;
};


#endif //ARCS_H