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

/** @file csh.h The declaration of classes for Color Syntax Highlighting.
 * @ingroup libmscgen_files */

#if !defined(CSH_H)
#define CSH_H

#include <cassert>
#include <vector>
#include <list>
#include <string>
#include <set>
#include <map>
#include <stack>
#include <algorithm>
#include "color.h"

/** This message is given when a user specifies multiple asterisks in an arrow.*/
#define MULTIPLE_ASTERISK_ERROR_MSG "One arrow may be lost only once. Use a single asterisk ('*')."

/** A range inside an input file. */
struct CshPos 
{
    int first_pos; ///<The index of the first character of the range.
    int last_pos;  ///<The index of the last character of the range.
    CshPos() = default;
    CshPos(int a, int b) : first_pos(a), last_pos(b) {}
    CshPos operator + (const CshPos &a) const { return CshPos(std::min(first_pos, a.first_pos), std::max(last_pos, a.last_pos)); }
    CshPos &operator += (const CshPos &a) { first_pos = std::min(first_pos, a.first_pos); last_pos = std::max(last_pos, a.last_pos); return *this; }
    bool IsWithin(int p) const { return first_pos<=p && last_pos>=p; } ///<Returns true if positin p is inside the range.
    bool IsWithin(const CshPos &p) const {return IsWithin(p.first_pos) && IsWithin(p.last_pos);} ///<Returns true if another range is completely within the range.
    template<class PosList> static void AdjustList(PosList &list, int start, int offset);
};

/** Describes types of language elements to color differently.*/
enum EColorSyntaxType {
    COLOR_NORMAL = 0,      ///<Regular text
    COLOR_KEYWORD,         ///<A keyword, like commands or 'parallel'
    COLOR_KEYWORD_PARTIAL, ///<A partially typed keyword
    COLOR_EQUAL,           ///<The equal sign
    COLOR_SEMICOLON,       ///<The semicolon sign
    COLOR_COLON,           ///<The colon character
    COLOR_COMMA,           ///<The comma character
    COLOR_SYMBOL,          ///<A symbol, e.g., arrows, box symbols, etc.
    COLOR_BRACE,           ///<An opening or closing brace {}
    COLOR_BRACKET,         ///<An opening or closing bracket []
    COLOR_DESIGNNAME,      ///<The name of a design
    COLOR_STYLENAME,       ///<The name of a style
    COLOR_ENTITYNAME,      ///<The name of an entity
    COLOR_ENTITYNAME_FIRST,///<The name of an entity when first occures in the file
    COLOR_COLORNAME,       ///<The name of a color
    COLOR_COLORDEF,        ///<The description of a color other than a name
    COLOR_MARKERNAME,      ///<The name of a marker
    COLOR_MARKERNAME_PARTIAL,///<The name of a marker partially written
    COLOR_OPTIONNAME,      ///<The name of a chart option
    COLOR_OPTIONNAME_PARTIAL,///<The name of a chart option partially written
    COLOR_ATTRNAME,        ///<The name of an attribute
    COLOR_ATTRNAME_PARTIAL,///<The name of a attribute partially written
    COLOR_ATTRVALUE,       ///<The value of an attribute
    COLOR_LABEL_TEXT,      ///<The text of a label, either after a colon or between quotation marks
    COLOR_LABEL_ESCAPE,    ///<The text of a lanel escape, such as '\n'
    COLOR_ERROR,           ///<A place where an error was detected.
    COLOR_NO_ERROR,        ///<The inverse of error formatting
    COLOR_COMMENT,         ///<A comment
    COLOR_MAX              ///<No specific element, simply the maximum value
};

/** Describes a colored range in the input file.*/
struct CshEntry : public CshPos
{
    EColorSyntaxType color; ///<The type of the language element in the range
    CshEntry() = default;
    CshEntry(const CshEntry&) = default;
    CshEntry(const CshPos &a, EColorSyntaxType c) : CshPos(a), color(c) {}
    CshEntry(int f, int l, EColorSyntaxType c) : CshPos(f, l), color(c) {}
    bool operator==(const CshEntry&o) const { return color == o.color && first_pos==o.first_pos && last_pos ==o.last_pos; }
};


/** An ordered collection of position ranges.*/
typedef std::vector<CshPos> CshPosList;

/** An ordered collection of colored ranges.*/
class CshListType : public std::vector<CshEntry>
{
    bool CheckIfOverlap(const CshEntry &e) const;
public:
    void AddToBack(const CshEntry &e) { _ASSERT(!CheckIfOverlap(e));  push_back(e); }        ///<Add to the end of the collection
    void DiffInto(const CshListType &old_list, const CshListType &new_list,
                  EColorSyntaxType neutral);
    bool CheckOrderedAndNonOverlapping() const;
    bool CheckIfErrorOrMaxColorsOnly() const;
};

/** An error detected during csh parse */
struct CshError 
{
    int first_pos;    ///<The beginning of the error
    std::string text; ///<The text of the error message.
};


/** A list of errors detected during csh parse */
class CshErrorList
{
    bool AddHelper(CshPos pos, const char *t);
public:
    std::vector<CshError> error_texts;  ///<The error messages. Not necessarily sorted.
    CshListType           error_ranges; ///<The character ranges to underline. Always sorted, contains only COLOR_ERROR or COLOR_MAX types
    void Add(const CshPos &pos, const char *t) { if (AddHelper(pos, t) && t) error_texts.back().text = t; } ///<Add an error to the collection
    void Add(const CshPos &pos, const std::string &t) { if (AddHelper(pos, t.c_str())) error_texts.back().text = t; } ///<Add an error to the collection
    void Add(const CshPos &pos, std::string &&t) { if (AddHelper(pos, t.c_str())) error_texts.back().text = std::move(t); } ///<Add an error to the collection
    void clear() {error_texts.clear(); error_ranges.clear(); }
    void swap(CshErrorList &a) { error_texts.swap(a.error_texts); error_ranges.swap(a.error_ranges); }
};

/** Flags to describe appearance of colored text*/
enum EColorSyntaxFlag {
    COLOR_FLAG_BOLD = 1,                 ///<Text shall be bold
    COLOR_FLAG_ITALICS = 2,              ///<Text shall be italics
    COLOR_FLAG_UNDERLINE = 4,            ///<Text shall be underlined
    COLOR_FLAG_COLOR = 8,                ///<Text shall be of specific color (actual value in separate members)
    COLOR_FLAG_DIFFERENT_NO_DRAW = 16,   ///<This Csh entry shall be recorded, even if text appears exactly as COLOR_NORMAL, but no need to draw (used by smart ident)
    COLOR_FLAG_DIFFERENT_DRAW = 32       ///<This Csh entry shall be both recorded and drawn even if appears exactly as COLOR_NORMAL (e.g., comments inside a label)
};

/** Describes the appearance of colored text*/
struct ColorSyntaxAppearance {
    unsigned effects;    ///<Value of flags 
    unsigned mask;       ///<Mask for the flags. Needed to be able to combine.
    unsigned char r;     ///<Red value
    unsigned char g;     ///<Green value
    unsigned char b;     ///<Blue value
    void SetColor(unsigned char rr, unsigned char gg, unsigned char bb)
    {r=rr; g=gg; b=bb;} ///<Set the color
    bool operator==(const struct ColorSyntaxAppearance &p) const;
};

/** How many coloring schemes shall we have.*/
#define CSH_SCHEME_MAX 4

/** Definition of the appearance of language elements for each coloring scheme.*/
extern ColorSyntaxAppearance MscCshAppearanceList[CSH_SCHEME_MAX][COLOR_MAX];
/** Initializes the coloring schemes*/
void MscInitializeCshAppearanceList(void);

std::string Cshize(const char *input, unsigned len, const CshListType &cshList, unsigned cshStyle,
                   const char *textformat=NULL);


class Canvas;
class StringFormat;
/** The X size of small symbols in the hint popup.*/
#define HINT_GRAPHIC_SIZE_X 25
/** The Y size of small symbols in the hint popup.*/
#define HINT_GRAPHIC_SIZE_Y 18
typedef unsigned CshHintGraphicParam;
class Csh;
/** A callback to draw the small symbols in hint popups.*/
typedef bool (*CshHintGraphicCallback)(Canvas*, CshHintGraphicParam, Csh &);

class Context;

/** The current context during a csh parse.*/
class CshContext
{
public:
    bool                  full;        ///<True if the context is fully specified.
    ColorSet              Colors;      ///<The colors defined so far
    std::set<std::string> StyleNames;  ///<The names of styles defined so far
    CshContext() : full(false) {}
    /** Set the context to a given design (styles and colors)*/
    void SetToDesign(const Context &);                   
    /** Combine two contexts */
    CshContext &operator+=(const CshContext &o) {
        Colors.insert(o.Colors.begin(), o.Colors.end());
        StyleNames.insert(o.StyleNames.begin(), o.StyleNames.end());
        full |= o.full;
        return *this;
    }
};

/* The status of collecting hints*/
enum EHintStatus {
    HINT_NONE,     ///<No hint identified yet
    HINT_LOCATED,  ///<We have located the type (e.g., attribute name or keyword) and location, but hints need filling
    HINT_FILLING,  ///<We have located the type and location and are in the process of adding hints
    HINT_READY     ///<We have a complete set of hints ready (may be empty)
};

/** The type of language element under the cursor that we shall provide alternatives (hints) for.*/
enum EHintType {
    HINT_ENTITY,     ///<Entities
    HINT_KEYWORD,    ///<Commands and other keywords
    HINT_ATTR_NAME,  ///<Attribute names (which of these shall be determined)
    HINT_ATTR_VALUE, ///<Attribute values
    HINT_MARKER,     ///<Marker names
    HINT_LINE_START, ///<Anything that can be at the beginning of a line (keyword, option, entity)
    HINT_ESCAPE      ///<A string escape sequence
};

/** The relative position of the cursor to a range in the input file.
 *
 * Note that the cursor is always between two character positions.*/
enum ECursorRelPosType {
    CURSOR_BEFORE=-2,      ///<The cursor is well before the range
    CURSOR_AFTER=-1,       ///<The cursor is well after the range
    CURSOR_AT_BEGINNING=1, ///<The cursor is immediately before the beginning of the range
    CURSOR_IN=2,           ///<The cursor is inside the range
    CURSOR_AT_END=3        ///<The cursor is right after the last character of the range
};

/** Describes the selection status of a hint in the popup window.*/
enum EHintItemSelectionState {
    HINT_ITEM_NOT_SELECTED,  ///<Not selected
    HINT_ITEM_SELECTED_HALF, ///<Selected, but the hint is not one that can be inserted into the text
    HINT_ITEM_SELECTED       ///<Selected and can be inserted into the chart text
};

/** One possibility for autocompletion valid at the cursor */
struct CshHint {
    unsigned sort;                  ///<Governs ordering of entries. Used for HINT_ESCAPEs, which do not sort alphabetically
    std::string decorated;          ///<Full text of the hint with formatting escapes
    EHintType  type;                ///<The type of the hint.
    bool selectable;                ///<True if this hint can be inserted to the chart text, false if it is just an explanation
    mutable const char *description;///<Descriptive text of the hint
    CshHintGraphicCallback callback;///<A procedure that draws the small symbol before the hint text
    CshHintGraphicParam    param;   ///<A parameter to pass to the callback
    /** @name Derived values filled in by Csh::ProcessHints
     * @{  */
    mutable std::string plain;  ///<Plain text of the hint for sorting
    mutable std::string replaceto;///<When selected, insert this to the chart text. When empty, use 'plain'.
    mutable bool keep;          ///<wether after insertion we shall keep the hint box up
    mutable int x_size;         ///<calculated size of hint text in the hint listbox
    mutable int y_size;         ///<calculated size of hint text in the hint listbox
    mutable int ul_x;           ///<Size of the rectange shown in list box
    mutable int ul_y;           ///<Size of the rectange shown in list box
    mutable int br_x;           ///<Size of the rectange shown in list box
    mutable int br_y;           ///<Size of the rectange shown in list box
    mutable EHintItemSelectionState state; ///<Will show if this hint is selected or not
    /** @} */
    CshHint(const std::string &d, const char *desc, EHintType t, bool s = true, CshHintGraphicCallback c = NULL, CshHintGraphicParam p = 0, unsigned so = 0) :
        sort(so), decorated(d), type(t), selectable(s), description(desc), callback(c), param(p), keep(false) {}
    void swap(CshHint &o);
    bool operator < (const CshHint &o) const { if (type==o.type) { if (sort==o.sort) return decorated<o.decorated; return sort<o.sort; } return type<o.type; }
    bool operator ==(const CshHint &o) const {return type == o.type && sort == o.sort && decorated == o.decorated;}
};

/** The max length of the keywords, attribute and option names.*/
#define ENUM_STRING_LEN 30

class ShapeCollection;

/** A class collecting syntax coloring infor and hints during a csh parse.
 *
 * A few words on how hints are collected.
 * Often we realize what is the type of hint in a low-level yacc rule, but know no specifics.
 * For example, we have yacc rues describing how an attribute looks like, that is "name = value". 
 * In this rule we can realize that the cursor is inside value (or we match to a rule saying
 * "name =" and realize that the cursor is just after). Here we can conclude that the hint
 * will be a sort of attribute value. (So we pick EHintType of HINT_ATTRVALUE.) But we do not know
 * what values the attribute can take, because that depens on what arc we are parsing. So after we macth
 * the low level rule, we set the hint type only and when processing the higher level rule for the
 * full arc, do we fill in the actual hints. In the meantime we set the hintStatus member to
 * HINT_LOCATED. Some cases we cannot fill in all the hints in one go and must go to an even higher
 * rule, then we use HINT_FILLING. Specifically for attribute values we also store the name
 * of the attribute in the low level rule into hintAttrName because it is needed in the high
 * level rule to select which attribute we are hinting the values of.*/
class Csh
{
    static const ShapeCollection *def_shapes;   ///<An empty shape collection - if there is no other one.
public:
    /** @name The collected CSH info and hints 
     * @{ */
    CshListType  CshList;                ///<The collected color syntax info
    CshPosList   ColonLabels;            ///<The position of colon labels 
    CshPosList   QuotedStrings;          ///<The position of quoted strings 
    CshErrorList CshErrors;              ///<The collected errors
    bool         was_partial;            ///<Indicates if the cursor is at the end of a partial keyword. On Sel Change we need to re-csh
    std::set<CshHint> Hints;             ///<The collected hints
    bool         hintsForcedOnly;        ///<Set to true if hint is located such that it should be displayed only if user forces that by Ctrl+Space
    unsigned     input_text_length;      ///<Length of the text we parse
    /** @} */
    /** @name Running variables during csh parsing
     * @{  */
    std::set<std::string>             EntityNames;    ///<The names of entities defined so far
    std::set<std::string>             MarkerNames;    ///<The names of markers defined so far
    std::map<std::string, CshContext> FullDesigns;    ///<The names and content of full designs defined so far
    std::map<std::string, CshContext> PartialDesigns; ///<The names and content of partial designs defined so far
    std::list<CshContext>             Contexts;       ///<The context stack
    std::list<std::string>            shape_names;    ///<list of shape names we have parsed definition for, but did not store
    EHintType                         hintType;       ///<The type of hint we found the cursor is at
    EHintStatus                       hintStatus;     ///<Shows if we have located the hint type and if we have filled in the hints or not
    CshPos                            hintedStringPos;///<The actual text location the hints refer to (can be the cursor only, which is a zero length range)
    std::string                       hintAttrName;   ///<In case of an ATTR_VALUE contains the name of the attribute
    bool                              addMarkersAtEnd;///<Set to true if at the end of the csh parsing we shall add the name of the markers to the hint list.
    unsigned                          asteriskNo;     ///<Number of asterisks inside an arrow spec so far.
    /** @}*/
    /** @name Input parameters to the hint lookup process
     * @{  */
    std::set<std::string> ForbiddenStyles; ///<Styles we never offer as hints (e.g., ->)
    const ShapeCollection*pShapes;         ///<What shapes do we have available prior csh parsing (pre-defined shapes).
    unsigned              cshScheme;       ///<What color shceme is used by the app now (to format hints)
    std::string           ForcedDesign;    ///<What design is forced on us (so its colors and styles can be offered)
    int                   cursor_pos;      ///<The location of the cursor now (used to identify partial keyword names & hint list)
    unsigned             *use_scheme;      ///<A pointer to the color scheme to use or NULL if unknown. Used to ignore CSH entries same as COLOR_NORMAL.                   
    /** @} */

    /** Initializes the Csh Object.
     * @param [in] defaultDesign Specifies a Context to collect 
     *   forbidden and default style names, color names and definitons to learn 
     *   from. If not in the initialization of a global variable, best to use 
     *   ArcBase::defaultDesign.
     * @param [in] shapes Specifies a shape collection to learn available shape 
     *   names from. May be NULL, if no shapes.*/
    Csh(const Context &defaultDesign, const ShapeCollection *shapes);

    /**Add a name of a shape (whithout the actual shape)*/
    void AddShapeName(const char *n) { shape_names.emplace_back(n); }

    /** @name Functions to add a CSH entry 
     * @{  */
    void AddCSH(const CshPos&, EColorSyntaxType); ///<The basic variant: color a range to this language element type
    void AddCSH_Error(const CshPos&pos, const char *text) { CshErrors.Add(pos, text); } ///<The basic variant for errors: add error "text" at this location
    void AddCSH_Error(const CshPos&pos, const std::string &text) { CshErrors.Add(pos, text); } ///<The basic variant for errors: add error "text" at this location
    void AddCSH_Error(const CshPos&pos, std::string &&text) { CshErrors.Add(pos, std::move(text)); } ///<The basic variant for errors: add error "text" at this location
    void AddCSH_ErrorAfter(const CshPos&pos, const char *text); ///<Add an error just after this range
    void AddCSH_ErrorAfter(const CshPos&pos, const std::string &text) { AddCSH_ErrorAfter(pos, text.c_str()); } ///<Add an error just after this range
    void AddCSH_ErrorAfter(const CshPos&pos, std::string &&text); ///<Add an error just after this range
    void AddCSH_KeywordOrEntity(const CshPos&pos, const char *name);
    bool AddCSH_ColonString_CheckEscapeHint(const CshPos& pos, const char *value, bool processComments);
    void AddCSH_AttrName(const CshPos&, const char *name, EColorSyntaxType); ///<At pos there is either an option or attribute name (specified by the type). Search and color.
    bool AddCSH_AttrValue_CheckEscapeHint(const CshPos& pos, const char *value, const char *name); 
    void AddCSH_AttrColorValue(const CshPos& pos); ///<At pos there is an attribute value that looks like a color definition (with commas and all). 
    void AddCSH_StyleOrAttrName(const CshPos&pos, const char *name); ///<At pos there is either an attribute name or a style. Decide and color.
    void AddCSH_EntityName(const CshPos&pos, const char *name); ///<At pos there is an entity name. Search and color.
    void AddCSH_EntityOrMarkerName(const CshPos&pos, const char *name); ///<At pos there is an entity or marker name. Search and color.
    void AddCSH_ExtvxposDesignatorName(const CshPos&pos, const char *name); ///<At pos there is an ext pos designator name. Search and color.
    void AddCSH_SymbolName(const CshPos&pos, const char *name); ///<At pos there is a symbol. Color it.
    void AddCSH_AllCommentBeyond(const CshPos&pos); ///<Mark anything beyond the end of 'pos' as comment
    /** @}*/
    void ParseText(const char *input, unsigned len, int cursor_p, unsigned scheme); 
    void AddColonLabel(const CshPos&pos) { ColonLabels.push_back(pos); } ///<Marks a range as a colon label
    void AddQuotedString(const CshPos&pos) { QuotedStrings.push_back(pos); } ///<Marks a range as a quoted string
    const CshPos *IsInColonLabel(int pos) const;        ///<After parsing return true if character 'pos' is in a colon label. (Used for smart ident.)
    const CshPos *IsInQuotedString(int pos) const;        ///<After parsing return true if character 'pos' is in a quoted string. (Used for smart ident.)

    void PushContext(bool empty=false);       ///<Push the context stack. If empty is false copy what was on top.
    void PopContext() {Contexts.pop_back();}  ///<Pop the context stack.
    std::string SetDesignTo(const std::string&design, bool full); 
    /** Returns the relation of the cursor pos to a range in the input file.*/
    ECursorRelPosType CursorIn(const CshPos &p) const {return CursorIn(p.first_pos, p.last_pos);}
    /** Returns the relation of the cursor pos to a range in the input file.*/
    ECursorRelPosType CursorIn(int a, int b) const;
    /** @name Set hint status if cursor position is at a hintable place.
     * @{ */
    bool CheckHintBetween(const CshPos &one, const CshPos &two, EHintType ht, const char *a_name=NULL);
    bool CheckHintBetweenPlusOne(const CshPos &one, const CshPos &two, EHintType ht, const char *a_name=NULL);
    bool CheckHintAtAndBefore(const CshPos &one, const CshPos &two, EHintType ht, const char *a_name=NULL);
    bool CheckHintAtAndBeforePlusOne(const CshPos &one, const CshPos &two, EHintType ht, const char *a_name=NULL);
    bool CheckHintAt(const CshPos &one, EHintType ht, const char *a_name=NULL);
    bool CheckHintAfter(const CshPos &one, const CshPos &lookahead, bool atEnd, EHintType ht, const char *a_name=NULL);
    bool CheckHintAfterPlusOne(const CshPos &one, const CshPos &lookahead, bool atEnd, EHintType ht, const char *a_name=NULL);
    bool CheckLineStartHintBefore(const CshPos &pos);
    bool CheckEntityHintAtAndBefore(const CshPos &one, const CshPos &two);
    bool CheckEntityHintAtAndBeforePlusOne(const CshPos &one, const CshPos &two);
    bool CheckEntityHintAt(const CshPos &one);
    bool CheckEntityHintAfter(const CshPos &one, const CshPos &lookahead, bool atEnd);
    bool CheckEntityHintAfterPlusOne(const CshPos &one, const CshPos &lookahead, bool atEnd);
    bool CheckHintLocated(EHintType ht, const CshPos  &location_to_check);
    /** @}*/
    
    /** Tells what formatting prefix to append to an information-only hint that should not be inserted to chart text.*/
    std::string HintPrefixNonSelectable() const {return "\\i";}
    /** Tells what formatting prefix to append to a hint of a given type.*/
    std::string HintPrefix(EColorSyntaxType) const;

    /** @name Add specific items to the list of hints.
     * @{ */
    void AddToHints(CshHint &&h);
    void AddToHints(const CshHint &h) {AddToHints(CshHint(h));} ///<Insert a hint to the list of hints.
    void AddToHints(const char * const * names_descriptions, 
                    const std::string &prefix, EHintType t, 
                    CshHintGraphicCallback c = NULL, bool in_order=false);
    void AddToHints(const char * const * names_descriptions,
                    const std::string &prefix, EHintType t,
                    CshHintGraphicCallback c, CshHintGraphicParam);
    void AddToHints(const char names[][ENUM_STRING_LEN], const char * const descriptions[],
                    const std::string &prefix, EHintType t,
                    CshHintGraphicCallback c = NULL);
    void AddToHints(const char names[][ENUM_STRING_LEN], const char * const descriptions[],
                    const std::string &prefix, EHintType t,
                    CshHintGraphicCallback c, CshHintGraphicParam);
    void AddColorValuesToHints(bool define);
    void AddDesignsToHints(bool full);
    void AddStylesToHints(bool include_forbidden, bool define);
    void AddOptionsToHints();
    void AddDesignOptionsToHints();
    void AddKeywordsToHints(bool includeParallel=true);
    void AddEntitiesToHints();
    void AddSymbolTypesToHints();
    void AddLeftRightCenterToHints();
    /** Add entities, keywords and option names to hint list.*/
    void AddLineBeginToHints(bool includeParallel=true) {AddEntitiesToHints(); AddKeywordsToHints(includeParallel); AddOptionsToHints();}
    void AddEscapesToHints();
    /** @}*/
    //fill in size, plain and filter/compact if needed
    void ProcessHints(Canvas &canvas, StringFormat *format, const std::string &uc, bool filter_by_uc, bool compact_same);
};

void CshParse(Csh &csh, const char *buff, unsigned len);

//returns -1 if txt is ""
//returns 0 if txt is not in coll
//returns 1 if txt is a prefix of something in coll, but not equals anything
//returns 2 if txt equals to something in coll
int FindPrefix(const std::set<std::string> &coll, const char *txt);

/** Emulate inserting or removing offset chars after start & update CSH entries
* We cancel entries that are entirely in the deleted region (if offset < 0) by setting their
* last position to a value smaller than their first pos.
* @param list The list to adjust. Any container, shall contain CshPos (or derivative) elements.
*             As for normal csh entires, the first character of the file is 1.
*             Also, if the first and last position of a csh entry equals, that means
*             that the csh entry corresponds to a single character of the file.
* @param [in] start The first character of the change. Again assuming the first character of
*                   the file is named 1. When inserting we insert *before* this character;
*                   when deleting, this is the first character we delete.
* @param [in] offset The number of bytes inserted (for positive value) or removed (for
*                    negative value). Value zero requires nothing to do.*/
template<class PosList>
void CshPos::AdjustList(PosList &list, int start, int offset)
{
    if (offset==0) return;
    const int upper = offset < 0 ? start - offset : start;
    for (auto &csh : list) {
        //if the entry is completely before "start" we do nothing
        if (csh.last_pos < start) continue;
        //if the entry is entirely after the affected region
        //we just shift it.
        if (csh.first_pos >= upper) {
            csh.first_pos += offset;
            csh.last_pos += offset;
            continue;
        }
        if (offset>0) {
            //here 'upper==start' thus it must be that 
            // 'first pos < start' and 'last pos >= start'
            csh.last_pos += offset;
            continue;
        }
        //here we delete between start and upper (but not upper itself)
        if (csh.first_pos >= start)
            csh.first_pos = start; //first pos is in the region deleted
        if (csh.last_pos < upper)
            csh.last_pos = start-1;  //last pos is in the region deleted
        else
            csh.last_pos += offset; //last pos is beyond the region deleted
    }
}

#endif
