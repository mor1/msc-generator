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

/** @file csh.h The declaration of classes for Color Syntax Highlighting.
 * @ingroup libmscgen_files */

#if !defined(CSH_H)
#define CSH_H

#include <cassert>
#include<vector>
#include<list>
#include<string>
#include<set>
#include<map>
#include<stack>
#include "color.h"

/** A range inside an input file. */
struct CshPos
{
  int first_pos; ///<The index of the first character of the range.
  int last_pos;  ///<The index of the last character of the range.
  CshPos() : first_pos(0), last_pos(0) {}
  bool IsWithin(int p) const {return first_pos<=p && last_pos>=p;} ///<Returns true if positin p is inside the range.
  bool IsWithin(const CshPos &p) const {return IsWithin(p.first_pos) && IsWithin(p.last_pos);} ///<Returns true if another range is completely within the range.
};

/** Describes types of language elements to color differently.*/
typedef enum
{
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
    COLOR_COMMENT,         ///<A comment
    COLOR_MAX              ///<No specific element, simply the maximum value
} MscColorSyntaxType;

/** Describes a colored range in the input file.*/
struct CshEntry
{
    int first_pos;            ///<The first char in the range
    int last_pos;             ///<The last char in the range
    MscColorSyntaxType color; ///<The type of the language element in the range
};

/** An ordered collection of colored ranges.*/
class CshListType : public std::vector<CshEntry>
{
public:
    void AddToFront(const CshEntry &e) {insert(begin(), e);} ///<Add to the front of the collection
    void AddToBack(const CshEntry &e) {push_back(e);}        ///<Add to the end of the collection
};

/** An error detected during csh parse */
struct CshError : public CshEntry
{
    std::string text; ///<The text of the error message.
};

/** A list of errors detected during csh parse */
class CshErrorList : public std::vector<CshError> 
{
public:
    void Add(CshPos &pos, const char *t); ///<Add an error to the collection
};

/** Flags to describe appearance of colored text*/
typedef enum
{
    COLOR_FLAG_BOLD = 1,
    COLOR_FLAG_ITALICS = 2,
    COLOR_FLAG_UNDERLINE = 4,
    COLOR_FLAG_COLOR = 8
} MscColorSyntaxFlag;

/** Describes the appearance of colored text*/
struct MscColorSyntaxAppearance {
    unsigned effects;    ///<Value of flags 
    unsigned mask;       ///<Mask for the flags. Needed to be able to combine.
    unsigned char r;     ///<Red value
    unsigned char g;     ///<Green value
    unsigned char b;     ///<Blue value
    void SetColor(unsigned char rr, unsigned char gg, unsigned char bb)
    {r=rr; g=gg; b=bb;} ///<Set the color
};

/** How many coloring schemes shall we have.*/
#define CSH_SCHEME_MAX 4

/** Definition of the appearance of language elements for each coloring scheme.*/
extern MscColorSyntaxAppearance MscCshAppearanceList[CSH_SCHEME_MAX][COLOR_MAX];
/** Initializes the coloring schemes*/
void MscInitializeCshAppearanceList(void);

std::string Cshize(const char *input, unsigned len, const CshListType &cshList, unsigned cshStyle,
                   const char *textformat=NULL);


class MscCanvas;
class StringFormat;
/** The X size of small symbols in the hint popup.*/
#define HINT_GRAPHIC_SIZE_X 25
/** The Y size of small symbols in the hint popup.*/
#define HINT_GRAPHIC_SIZE_Y 18
typedef unsigned CshHintGraphicParam;
/** A callback to draw the small symbols in hint popups.*/
typedef bool (*CshHintGraphicCallback)(MscCanvas*, CshHintGraphicParam);

/** The current context during a csh parse.*/
class CshContext
{
public:
    bool                  full;        ///<True if the context is fully specified.
    ColorSet              Colors;      ///<The colors defined so far
    std::set<std::string> StyleNames;  ///<The names of styles defined so far
    CshContext() : full(false) {}
    /** Set the context to plain design (default styles and colors)*/
    void SetPlain();                   
    /** Combine two contexts */
    CshContext &operator+=(const CshContext &o) {
        Colors.insert(o.Colors.begin(), o.Colors.end());
        StyleNames.insert(o.StyleNames.begin(), o.StyleNames.end());
        full |= o.full;
        return *this;
    }
};

/* The status of collecting hints*/
typedef enum {
    HINT_NONE,     ///<No hint identified yet
    HINT_LOCATED,  ///<We have located the type (e.g., attribute name or keyword) and location, but hints need filling
    HINT_FILLING,  ///<We have located the type and location and are in the process of adding hints
    HINT_READY     ///<We have a complete set of hints ready (may be empty)
} CshHintStatus;

/** The type of language element under the cursor that we shall provide alternatives (hints) for.*/
typedef enum {
    HINT_ENTITY,     ///<Entities
    HINT_KEYWORD,    ///<Commands and other keywords
    HINT_ATTR_NAME,  ///<Attribute names (which of these shall be determined)
    HINT_ATTR_VALUE, ///<Attribute values
    HINT_MARKER,     ///<Marker names
    HINT_LINE_START  ///<Anything that can be at the beginning of a line (keyword, option, entity)
} CshHintType;

/** The relative position of the cursor to a range in the input file.
 *
 * Note that the cursor is always between two character positions.*/
typedef enum {
    CURSOR_BEFORE=-2,      ///<The cursor is well before the range
    CURSOR_AFTER=-1,       ///<The cursor is well after the range
    CURSOR_AT_BEGINNING=1, ///<The cursor is immediately before the beginning of the range
    CURSOR_IN=2,           ///<The cursor is inside the range
    CURSOR_AT_END=3        ///<The cursor is right after the last character of the range
} CshCursorRelPosType;

/** Describes the selection status of a hint in the popup window.*/
typedef enum {
    HINT_ITEM_NOT_SELECTED,  ///<Not selected
    HINT_ITEM_SELECTED_HALF, ///<Selected, but the hint is not one that can be inserted into the text
    HINT_ITEM_SELECTED       ///<Selected and can be inserted into the chart text
} CshHintItemSelectionState;

/** One possibility for autocompletion valid at the cursor */
struct CshHint {
    std::string  decorated;         ///<Full text of the hint with formatting escapes
    CshHintType  type;              ///<The type of the hint.
    bool selectable;                ///<True if this hint can be inserted to the chart text, false if it is just an explanation
    CshHintGraphicCallback callback;///<A procedure that draws the small symbol before the hint text
    CshHintGraphicParam    param;   ///<A parameter to pass to the callback
    /** @name Derived values filled in by Csh::ProcessHints
     * @{  */
    mutable std::string plain;  ///<Plain text of the hint for sorting and insertion into the chart text
    mutable bool keep;          ///<wether after insertion we shall keep the hint box up
    mutable int x_size;         ///<calculated size of hint text in the hint listbox
    mutable int y_size;         ///<calculated size of hint text in the hint listbox
    mutable int ul_x;           ///<Size of the rectange shown in list box
    mutable int ul_y;           ///<Size of the rectange shown in list box
    mutable int br_x;           ///<Size of the rectange shown in list box
    mutable int br_y;           ///<Size of the rectange shown in list box
    mutable CshHintItemSelectionState state; ///<Will show if this hint is selected or not
    /** @} */
    CshHint(const std::string &d, CshHintType t, bool s = true, CshHintGraphicCallback c=NULL, CshHintGraphicParam p=0)  : 
        decorated(d), type(t), selectable(s), callback(c), param(p), keep(false) {}
    void swap(CshHint &o);
    bool operator < (const CshHint &o) const {if (type==o.type) return decorated<o.decorated; return type<o.type;}
    bool operator ==(const CshHint &o) const {return type == o.type && decorated == o.decorated;}
};

/** The max length of the keywords, attribute and option names.*/
#define ENUM_STRING_LEN 30

/** A class collecting syntax coloring infor and hints during a csh parse.
 *
 * A few words on how hints are collected.
 * Often we realize what is the type of hint in a low-level yacc rule, but know no specifics.
 * For example, we have yacc rues describing how an attribute looks like, that is "name = value". 
 * In this rule we can realize that the cursor is inside value (or we match to a rule saying
 * "name =" and realize that the cursor is just after). Here we can conclude that the hint
 * will be a sort of attribute value. (So we pick CshHintType of HINT_ATTRVALUE.) But we do not know
 * what values the attribute can take, because that depens on what arc we are parsing. So after we macth
 * the low level rule, we set the hint type only and when processing the higher level rule for the
 * full arc, do we fill in the actual hints. In the meantime we set the hintStatus member to
 * HINT_LOCATED. Some cases we cannot fill in all the hints in one go and must go to an even higher
 * rule, then we use HINT_FILLING. Specifically for attribute values we also store the name
 * of the attribute in the low level rule into hintAttrName because it is needed in the high
 * level rule to select which attribute we are hinting the values of.*/
class Csh
{
public:
    /** @name The collected CSH info and hints 
     * @{ */
    CshListType  CshList;                ///<The collected color syntax info
    CshErrorList CshErrors;              ///<The collected errors
    bool         was_partial;            ///<Indicates if the cursor is at the end of a partial keyword
    CshEntry     partial_at_cursor_pos;  ///<If the cursor is at a partially matching keyword this contains that keyword (color==what we have to revert to if cursor moves away)
    std::set<CshHint> Hints;             ///<The collected hints
    bool         hintsForcedOnly;        ///<Set to true if hint is located such that it should be displayed only if user forces that by Ctrl+Space
    /** @} */
    /** @name Running variables during csh parsing
     * @{  */
    std::set<std::string>             EntityNames;    ///<The names of entities defined so far
    std::set<std::string>             MarkerNames;    ///<The names of markers defined so far
    std::map<std::string, CshContext> FullDesigns;    ///<The names and content of full designs defined so far
    std::map<std::string, CshContext> PartialDesigns; ///<The names and content of partial designs defined so far
    std::list<CshContext>             Contexts;       ///<The context stack
    CshHintType                       hintType;       ///<The type of hint we found the cursor is at
    CshHintStatus                     hintStatus;     ///<Shows if we have located the hint type and if we have filled in the hints or not
    CshPos                            hintedStringPos;///<The actual text location the hints refer to (can be the cursor only, which is a zero length range)
    std::string                       hintAttrName;   ///<In case of an ATTR_VALUE contains the name of the attribute
    bool                              addMarkersAtEnd;///<Set to true if at the end of the csh parsing we shall add the name of the markers to the hint list.
    /** @}*/
    /** @name Input parameters to the hint lookup process
     * @{  */
    std::set<std::string> ForbiddenStyles; ///<Styles we never offer as hints (e.g., ->)
    unsigned              cshScheme;       ///<What color shceme is used by the app now (to format hints)
    std::string           ForcedDesign;    ///<What design is forced on us (so its colors and styles can be offered)
    int                   cursor_pos;      ///<The location of the cursor now (used to identify partial keyword names & hint list)
    /** @} */

    Csh();

    /** @name Functions to add a CSH entry 
     * @{  */
    void AddCSH(CshPos&, MscColorSyntaxType); ///<The basic variant: color a range to this language element type
    void AddCSH_Error(CshPos&pos, const char *text) {CshErrors.Add(pos, text);} ///<The basic variant for errors: add error "text" at this location
    void AddCSH_ErrorAfter(CshPos&pos, const char *text); ///<Add an error just after this range
    void AddCSH_KeywordOrEntity(CshPos&pos, const char *name); 
    void AddCSH_ColonString(CshPos& pos, const char *value, bool processComments); ///<This is a colon followed by a label. if processComments is true, search for @# comments and color them so. (False for quoted colon strings.)
    void AddCSH_AttrName(CshPos&, const char *name, MscColorSyntaxType); ///<At pos there is either an option or attribute name (specified by the type). Search and color.
    void AddCSH_AttrValue(CshPos& pos, const char *value, const char *name); ///<At pos there is an attribute value. If the attribute name indicates a label, color the escapes, too.
    void AddCSH_StyleOrAttrName(CshPos&pos, const char *name); ///<At pos there is either an attribute name or a style. Decide and color.
    void AddCSH_EntityName(CshPos&pos, const char *name); ///<At pos there is an entity name. Search and color.
    void AddCSH_EntityOrMarkerName(CshPos&pos, const char *name); ///<At pos there is an entity or marker name. Search and color.
    void AddCSH_ExtvxposDesignatorName(CshPos&pos, const char *name); ///<At pos there is an ext pos designator name. Search and color.
    void AddCSH_SymbolName(CshPos&pos, const char *name); ///<At pos there is a symbol. Color it.
    void AddErrorsToCsh() {for (unsigned i=0; i<CshErrors.size(); i++) CshList.AddToFront(CshErrors[i]);} ///<Add error coloring for each error collected.
    /** @}*/
    void ParseText(const char *input, unsigned len, int cursor_p, unsigned scheme); 
    MscColorSyntaxType GetCshAt(int pos); ///<After parsing return what is the language element at character 'pos'.

    void PushContext(bool empty=false);       ///<Push the context stack. If empty is false copy what was on top.
    void PopContext() {Contexts.pop_back();}  ///<Pop the context stack.
    std::string SetDesignTo(const std::string&design, bool full); 
    /** Returns the relation of the cursor pos to a range in the input file.*/
    CshCursorRelPosType CursorIn(const CshPos &p) const {return CursorIn(p.first_pos, p.last_pos);}
    /** Returns the relation of the cursor pos to a range in the input file.*/
    CshCursorRelPosType CursorIn(int a, int b) const;
    /** @name Set hint status if cursor position is at a hintable place.
     * @{ */
    bool CheckHintBetween(const CshPos &one, const CshPos &two, CshHintType ht, const char *a_name=NULL);
    bool CheckHintBetweenPlusOne(const CshPos &one, const CshPos &two, CshHintType ht, const char *a_name=NULL);
    bool CheckHintAtAndBefore(const CshPos &one, const CshPos &two, CshHintType ht, const char *a_name=NULL);
    bool CheckHintAtAndBeforePlusOne(const CshPos &one, const CshPos &two, CshHintType ht, const char *a_name=NULL);
    bool CheckHintAt(const CshPos &one, CshHintType ht, const char *a_name=NULL);
    bool CheckHintAfter(const CshPos &one, const CshPos &lookahead, bool atEnd, CshHintType ht, const char *a_name=NULL);
    bool CheckHintAfterPlusOne(const CshPos &one, const CshPos &lookahead, bool atEnd, CshHintType ht, const char *a_name=NULL);
    bool CheckEntityHintAtAndBefore(const CshPos &one, const CshPos &two);
    bool CheckEntityHintAtAndBeforePlusOne(const CshPos &one, const CshPos &two);
    bool CheckEntityHintAt(const CshPos &one);
    bool CheckEntityHintAfter(const CshPos &one, const CshPos &lookahead, bool atEnd);
    bool CheckEntityHintAfterPlusOne(const CshPos &one, const CshPos &lookahead, bool atEnd);
    bool CheckHintLocated(CshHintType ht, const CshPos  &location_to_check);
    /** @}*/
    
    /** Tells what formatting prefix to append to an information-only hint that should not be inserted to chart text.*/
    std::string HintPrefixNonSelectable() const {return "\\i";}
    /** Tells what formatting prefix to append to a hint of a given type.*/
    std::string HintPrefix(MscColorSyntaxType) const;

    /** @name Add specific items to the list of hints.
     * @{ */
    void AddToHints(CshHint &&h);
    void AddToHints(const CshHint &h) {AddToHints(CshHint(h));} ///<Insert a hint to the list of hints.
    void AddToHints(const char names[][ENUM_STRING_LEN], const std::string &prefix, CshHintType t, 
                    CshHintGraphicCallback c=NULL);
    void AddToHints(const char names[][ENUM_STRING_LEN], const std::string &prefix, CshHintType t, 
                    CshHintGraphicCallback c, CshHintGraphicParam);
    void AddColorValuesToHints();
    void AddDesignsToHints(bool full);
    void AddStylesToHints();
    void AddOptionsToHints();
    void AddDesignOptionsToHints();
    void AddKeywordsToHints(bool includeParallel=true);
    void AddEntitiesToHints();
    /** Add entities, keywords and option names to hint list.*/
    void AddLineBeginToHints(bool includeParallel=true) {AddEntitiesToHints(); AddKeywordsToHints(includeParallel); AddOptionsToHints();}
    /** @}*/
    //fill in size, plain and filter/compact if needed
    void ProcessHints(MscCanvas &canvas, StringFormat *format, const std::string &uc, bool filter_by_uc, bool compact_same);
};

void CshParse(Csh &csh, const char *buff, unsigned len);

//returns -1 if txt is ""
//returns 0 if txt is not in coll
//returns 1 if txt is a prefix of something in coll, but not equals anything
//returns 2 if txt equals to something in coll
int FindPrefix(const std::set<std::string> &coll, const char *txt);

#endif
