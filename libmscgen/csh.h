#if !defined(CSH_H)
#define CSH_H

#ifndef _ASSERT
#define  _ASSERT(A)
#endif

#include<vector>
#include<list>
#include<string>
#include<set>
#include<map>
#include<stack>

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
    COLOR_OPTIONNAME,
    COLOR_OPTIONNAME_PARTIAL,
    COLOR_ATTRNAME,
    COLOR_ATTRNAME_PARTIAL,
    COLOR_ATTRVALUE,
    COLOR_LABEL_TEXT,
    COLOR_LABEL_ESCAPE,
    COLOR_ERROR,
    COLOR_ACTIVE_ERROR,
    COLOR_COMMENT,
    COLOR_MAX
} MscColorSyntaxType;

struct CshEntry
{
    int first_pos;
    int last_pos;
    MscColorSyntaxType color;
};

typedef std::vector<CshEntry> MscCshListType;

typedef enum
{
    COLOR_FLAG_BOLD = 1,
    COLOR_FLAG_ITALICS = 2,
    COLOR_FLAG_UNDERLINE = 4,
    COLOR_FLAG_COLOR = 8
} MscColorSyntaxFlag;

struct MscColorSyntaxAppearance {
    int effects;
    int mask;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    void SetColor(unsigned char rr, unsigned char gg, unsigned char bb)
    {r=rr; g=gg; b=bb;}
};

#define CSH_SCHEME_MAX 4

extern MscColorSyntaxAppearance MscCshAppearanceList[CSH_SCHEME_MAX][COLOR_MAX];
void MscInitializeCshAppearanceList(void);

std::string Cshize(const char *input, unsigned len, const MscCshListType &cshList, int cshStyle,
                   const char *command=NULL);


//Object to collect color syntax highlighting data
class CshContext
{
public:
    std::set<std::string> ColorNames;
    std::set<std::string> StyleNames;
    void SetPlain();
    CshContext &operator+=(const CshContext &o) {
        ColorNames.insert(o.ColorNames.begin(), o.ColorNames.end());
        StyleNames.insert(o.StyleNames.begin(), o.StyleNames.end());
        return *this;
    }
};

typedef enum {
    HINT_NONE,     //No hint identified yet
    HINT_LOCATED,  //We have located the type and location, but hints need filling
    HINT_READY     //We have a complete set of hints ready (may be empty)
} CshHintStatus;

typedef enum {
    HINT_ATTR_NAME,
    HINT_ATTR_VALUE,
    HINT_ENTITY,
    HINT_LINE_START,
    HINT_DESIGNS
} CshHintType;

struct CshPos
{
  int first_pos;
  int last_pos;
  bool hint_can_come;
  CshHintType hint_type;
  std::string hint_attr_name;
  CshPos() : first_pos(0), last_pos(0), hint_can_come(false) {}
  bool IsWithin(int p) {return first_pos<=p && last_pos>=p;}
};

typedef enum {
    CURSOR_BEFORE=-2,
    CURSOR_AFTER=-1,
    CURSOR_AT_BEGINNING=1,
    CURSOR_IN=2,
    CURSOR_AT_END=3
} CshCursorRelPosType;

//These shall be one char long - they flag the hints (first char of a hint can be this)
//marks this prefix shall not be selected (and shall be in italics)
#define CSH_HINT_NON_SELECTABLE_PREFIX "\x03"
#define ENUM_STRING_LEN 20

class Csh
{
public:
    //The collected color syntax info
    MscCshListType CshList;
    bool           was_partial;            //indicates if the cursor is at the end of a partial keyword
    CshEntry       partial_at_cursor_pos;  //if the cursor is at a partially matching keyword
                                           //this contains that keyword (color==what we have to revert to
                                           //if cursor moves away)
    //The collected hints
    std::set<std::string> Hints;
    bool                  hintsForcedOnly; //if true hints should be displayed only if user forces that
    //Running variables during csh parsing
    std::set<std::string>             EntityNames;
    std::map<std::string, CshContext> Designs;
    std::list<CshContext>             Contexts;
    CshHintType                       hintType;        //low level rules use to indicate type to higher rules
    CshHintStatus                     hintStatus;      //indicates if a lower level rule has anything to indicate
    CshPos                            hintedStringPos; //contains the actual text the hints refer to (can be the cursor only)
    std::string                       hintAttrName;    //in case of an ATTR_VALUE show the name of the attribute
    //Input parameters to the hint lookup process
    std::set<std::string> ForbiddenStyles; //Styles we never offer as hints (e.g., ->)
    int                   cshScheme;       //What color shceme is used by the app now (to format hints)
    std::string           ForcedDesign;    //What design is forced on us (so its colors and styles can be offered)
    int                   cursor_pos;      //to identify partial keyword names & hint list

    Csh();
    void AddCSH(CshPos&, MscColorSyntaxType);
    void AddCSH_AttrValue(CshPos&, const char *value, const char *name);
    void AddCSH_KeywordOrEntity(CshPos&pos, const char *name);
    void AddCSH_ColonString(CshPos& pos, const char *value, bool processComments);
    void AddCSH_AttrName(CshPos&, const char *name, MscColorSyntaxType);
    void AddCSH_StyleOrAttrName(CshPos&pos, const char *name);
    void AddCSH_EntityName(CshPos&pos, const char *name);
    void ParseText(const char *input, unsigned len, int cursor_p, int scheme);
    MscColorSyntaxType GetCshAt(int pos);

    void PushContext(bool empty=false);
    void PopContext() {Contexts.pop_back();}
    bool SetDesignTo(const std::string&design);
    CshCursorRelPosType CursorIn(const CshPos &p) const {return CursorIn(p.first_pos, p.last_pos);}
    CshCursorRelPosType CursorIn(int a, int b) const;
    bool CheckHintBetween(const CshPos &one, const CshPos &two, CshHintType ht, const char *a_name=NULL);
    bool CheckHintCanComeBetween(const CshPos &one, const CshPos &two);
    bool CheckHintAtAndBefore(const CshPos &one, const CshPos &two, CshHintType ht, const char *a_name=NULL);
    bool CheckHintAt(const CshPos &one, CshHintType ht, const char *a_name=NULL);

    std::string HintPrefixNonSelectable() const {return CSH_HINT_NON_SELECTABLE_PREFIX "\\i";}
    std::string HintPrefix(MscColorSyntaxType) const;
    void AddToHints(const std::string &a) {Hints.insert(a);}
    void AddToHints(const char names[][ENUM_STRING_LEN], const std::string &prefix);
    void AddColorValuesToHints();
    void AddDesignsToHints();
    void AddStylesToHints();
    void AddOptionsToHints();
    void AddKeywordsToHints();
    void AddEntitiesToHints();
    void AddLineBeginToHnts() {AddOptionsToHints(); AddKeywordsToHints(); AddEntitiesToHints();}
};

void CshParse(Csh &csh, const char *buff, unsigned len);


#endif
