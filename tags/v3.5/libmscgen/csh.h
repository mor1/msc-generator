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

struct CshPos
{
  int first_pos;
  int last_pos;
  CshPos() : first_pos(0), last_pos(0) {}
  bool IsWithin(int p) const {return first_pos<=p && last_pos>=p;}
  bool IsWithin(const CshPos &p) const {return IsWithin(p.first_pos) && IsWithin(p.last_pos);}
};

typedef enum
{
    COLOR_NORMAL = 0,
    COLOR_KEYWORD,
    COLOR_KEYWORD_PARTIAL,
    COLOR_EQUAL,
    COLOR_SEMICOLON,
    COLOR_COLON,
    COLOR_COMMA,
    COLOR_SYMBOL,
    COLOR_BRACE,
    COLOR_BRACKET,
    COLOR_DESIGNNAME,
    COLOR_STYLENAME,
    COLOR_ENTITYNAME,
    COLOR_ENTITYNAME_FIRST,
    COLOR_COLORNAME,
    COLOR_COLORDEF,
    COLOR_MARKERNAME,
    COLOR_MARKERNAME_PARTIAL,
    COLOR_OPTIONNAME,
    COLOR_OPTIONNAME_PARTIAL,
    COLOR_ATTRNAME,
    COLOR_ATTRNAME_PARTIAL,
    COLOR_ATTRVALUE,
    COLOR_LABEL_TEXT,
    COLOR_LABEL_ESCAPE,
    COLOR_ERROR,
    COLOR_COMMENT,
    COLOR_MAX
} MscColorSyntaxType;

struct CshEntry
{
    int first_pos;
    int last_pos;
    MscColorSyntaxType color;
};

class CshListType : public std::vector<CshEntry>
{
public:
    void AddToFront(const CshEntry &e) {insert(begin(), e);}
    void AddToBack(const CshEntry &e) {push_back(e);}
};

struct CshError : public CshEntry
{
    std::string text;
};

class CshErrorList : public std::vector<CshError> 
{
public:
    void Add(CshPos &pos, const char *t);
};


typedef enum
{
    COLOR_FLAG_BOLD = 1,
    COLOR_FLAG_ITALICS = 2,
    COLOR_FLAG_UNDERLINE = 4,
    COLOR_FLAG_COLOR = 8
} MscColorSyntaxFlag;

struct MscColorSyntaxAppearance {
    unsigned effects;
    unsigned mask;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    void SetColor(unsigned char rr, unsigned char gg, unsigned char bb)
    {r=rr; g=gg; b=bb;}
};

#define CSH_SCHEME_MAX 4

extern MscColorSyntaxAppearance MscCshAppearanceList[CSH_SCHEME_MAX][COLOR_MAX];
void MscInitializeCshAppearanceList(void);

std::string Cshize(const char *input, unsigned len, const CshListType &cshList, unsigned cshStyle,
                   const char *command=NULL);


class MscCanvas;
class StringFormat;
#define HINT_GRAPHIC_SIZE_X 25
#define HINT_GRAPHIC_SIZE_Y 18
typedef unsigned CshHintGraphicParam;
typedef bool (*CshHintGraphicCallback)(MscCanvas*, CshHintGraphicParam);

//Object to collect color syntax highlighting data
class CshContext
{
public:
    bool                  full;
    ColorSet              Colors;
    std::set<std::string> StyleNames;
    CshContext() : full(false) {}
    void SetPlain();
    CshContext &operator+=(const CshContext &o) {
        Colors.insert(o.Colors.begin(), o.Colors.end());
        StyleNames.insert(o.StyleNames.begin(), o.StyleNames.end());
        full |= o.full;
        return *this;
    }
};

typedef enum {
    HINT_NONE,     //No hint identified yet
    HINT_LOCATED,  //We have located the type and location, but hints need filling
    HINT_FILLING,  //We have located the type and location and are in the process of adding hints
    HINT_READY     //We have a complete set of hints ready (may be empty)
} CshHintStatus;

typedef enum {
    HINT_ATTR_NAME,
    HINT_ATTR_VALUE,
    HINT_ENTITY,
    HINT_MARKER,
    HINT_KEYWORD,
    HINT_LINE_START
} CshHintType;

typedef enum {
    CURSOR_BEFORE=-2,
    CURSOR_AFTER=-1,
    CURSOR_AT_BEGINNING=1,
    CURSOR_IN=2,
    CURSOR_AT_END=3
} CshCursorRelPosType;

typedef enum {
    HINT_ITEM_NOT_SELECTED,
    HINT_ITEM_SELECTED_HALF,
    HINT_ITEM_SELECTED
} CshHintItemSelectionState;

struct CshHint {
    std::string  decorated;     //full text with formatting escapes
    CshHintType  type;
    bool selectable;    //wether user can select this hint for insertion 
    CshHintGraphicCallback callback;
    CshHintGraphicParam    param;
    //Derived values filled in by Csh::ProcessHints
    mutable std::string plain;  //plain text for sorting and substitution
    mutable bool keep;          //wether after substitution we shall keep the hint box up
    mutable int x_size;         //calculated size of hint text in the hint listbox
    mutable int y_size;
    mutable int ul_x, ul_y, br_x, br_y;     //Size of the rectange shown in list box
    mutable CshHintItemSelectionState state; //will show if this hint is selected or not
    CshHint(const std::string &d, CshHintType t, bool s = true, CshHintGraphicCallback c=NULL, CshHintGraphicParam p=0)  : 
        decorated(d), type(t), selectable(s), callback(c), param(p), keep(false) {}
    void swap(CshHint &o);
    bool operator < (const CshHint &o) const {if (type==o.type) return decorated<o.decorated; return type<o.type;}
    bool operator ==(const CshHint &o) const {return type == o.type && decorated == o.decorated;}
};

#define ENUM_STRING_LEN 30

class Csh
{
public:
    //The collected color syntax info
    CshListType  CshList;
    CshErrorList CshErrors;              
    bool         was_partial;            //indicates if the cursor is at the end of a partial keyword
    CshEntry     partial_at_cursor_pos;  //if the cursor is at a partially matching keyword
                                           //this contains that keyword (color==what we have to revert to
                                           //if cursor moves away)
    //The collected hints
    std::set<CshHint> Hints;
    bool              hintsForcedOnly; //if true hints should be displayed only if user forces that
    //Running variables during csh parsing
    std::set<std::string>             EntityNames;
    std::set<std::string>             MarkerNames;
    std::map<std::string, CshContext> FullDesigns, PartialDesigns;
    std::list<CshContext>             Contexts;
    CshHintType                       hintType;        //low level rules use to indicate type to higher rules
    CshHintStatus                     hintStatus;      //indicates if a lower level rule has anything to indicate
    CshPos                            hintedStringPos; //contains the actual text the hints refer to (can be the cursor only)
    std::string                       hintAttrName;    //in case of an ATTR_VALUE show the name of the attribute
    bool                              addMarkersAtEnd;
    //Input parameters to the hint lookup process
    std::set<std::string> ForbiddenStyles; //Styles we never offer as hints (e.g., ->)
    unsigned              cshScheme;       //What color shceme is used by the app now (to format hints)
    std::string           ForcedDesign;    //What design is forced on us (so its colors and styles can be offered)
    int                   cursor_pos;      //to identify partial keyword names & hint list

    Csh();
    void AddCSH(CshPos&, MscColorSyntaxType);
    void AddCSH_Error(CshPos&pos, const char *text) {CshErrors.Add(pos, text);}
    void AddCSH_ErrorAfter(CshPos&pos, const char *text); //Add an error just after this range
    void AddCSH_KeywordOrEntity(CshPos&pos, const char *name);
    void AddCSH_ColonString(CshPos& pos, const char *value, bool processComments);
    void AddCSH_AttrName(CshPos&, const char *name, MscColorSyntaxType);
    void AddCSH_AttrValue(CshPos& pos, const char *value, const char *name);
    void AddCSH_StyleOrAttrName(CshPos&pos, const char *name);
    void AddCSH_EntityName(CshPos&pos, const char *name);
    void AddCSH_EntityOrMarkerName(CshPos&pos, const char *name);
    void AddCSH_ExtvxposDesignatorName(CshPos&pos, const char *name);
    void AddCSH_SymbolName(CshPos&pos, const char *name);
    void ParseText(const char *input, unsigned len, int cursor_p, unsigned scheme);
    void AddErrorsToCsh() {for (unsigned i=0; i<CshErrors.size(); i++) CshList.AddToFront(CshErrors[i]);}
    MscColorSyntaxType GetCshAt(int pos);

    void PushContext(bool empty=false);
    void PopContext() {Contexts.pop_back();}
    std::string SetDesignTo(const std::string&design, bool full);
    CshCursorRelPosType CursorIn(const CshPos &p) const {return CursorIn(p.first_pos, p.last_pos);}
    CshCursorRelPosType CursorIn(int a, int b) const;
    bool CheckHintBetween(const CshPos &one, const CshPos &two, CshHintType ht, const char *a_name=NULL);
    bool CheckHintBetweenPlusOne(const CshPos &one, const CshPos &two, CshHintType ht, const char *a_name=NULL);
    bool CheckHintAtAndBefore(const CshPos &one, const CshPos &two, CshHintType ht, const char *a_name=NULL);
    bool CheckHintAtAndBeforePlusOne(const CshPos &one, const CshPos &two, CshHintType ht, const char *a_name=NULL);
    bool CheckHintAt(const CshPos &one, CshHintType ht, const char *a_name=NULL);
    bool CheckHintAtPlusOne(const CshPos &one, CshHintType ht, const char *a_name=NULL);
    bool CheckHintAfter(const CshPos &one, const CshPos &lookahead, bool atEnd, CshHintType ht, const char *a_name=NULL);
    bool CheckHintAfterPlusOne(const CshPos &one, const CshPos &lookahead, bool atEnd, CshHintType ht, const char *a_name=NULL);
    bool CheckEntityHintAtAndBefore(const CshPos &one, const CshPos &two);
    bool CheckEntityHintAtAndBeforePlusOne(const CshPos &one, const CshPos &two);
    bool CheckEntityHintAt(const CshPos &one);
    bool CheckEntityHintAfter(const CshPos &one, const CshPos &lookahead, bool atEnd);
    bool CheckEntityHintAfterPlusOne(const CshPos &one, const CshPos &lookahead, bool atEnd);
    bool CheckHintLocated(CshHintType ht, const CshPos  &location_to_check);
     
    std::string HintPrefixNonSelectable() const {return "\\i";}
    std::string HintPrefix(MscColorSyntaxType) const;
    void AddToHints(CshHint &&h);
    void AddToHints(const CshHint &h) {AddToHints(CshHint(h));}
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
    void AddLineBeginToHints(bool includeParallel=true) {AddOptionsToHints(); AddKeywordsToHints(includeParallel); AddEntitiesToHints();}
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
