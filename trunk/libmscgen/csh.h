#if !defined(CSH_H)
#define CSH_H

#include<vector>
#include<list>
#include<string>
#include<set>
#include<map>
#include<stack>

struct CshPos
{
  int first_pos;
  int last_pos;
  CshPos() : first_pos(0), last_pos(0) {}
  bool IsWithin(int p) {return first_pos<=p && last_pos>=p;}
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
    HINT_NONE,
    HINT_ATTR_NAME,
    HINT_ATTR_NAME_CAN_COME,
    HINT_ATTR_VALUE,
    HINT_ATTR_VALUE_CAN_COME,
    HINT_ENTITY,
    HINT_LINE_START,
    HINT_READY
} CshHintStatus;

//These shall be one char long - they flag the hints (first char of a hint can be this)
//marks this prefix shall not be selected (and shall be in italics)
#define CSH_HINT_NON_SELECTABLE_PREFIX "\x03"

class Csh
{
public:
    MscCshListType                    CshList;
    std::set<std::string>             EntityNames;
    std::map<std::string, CshContext> Designs;
    std::list<CshContext>             Contexts;

    CshHintStatus                     hintStatus;
    CshPos                            hintedStringPos;
    std::string                       hintAttrName;  //if status is ATTR_VALUE
    std::set<std::string>             Hints;
    std::set<std::string>             ForbiddenStyles;

    int                   cshScheme;
    std::string           ForcedDesign;
    int                   cursor_pos;             //for partial keyword names & hint list
    bool                  was_partial;
    CshEntry              partial_at_cursor_pos;  //if the cursor is at a partially matching keyword
                                                  //this contains that keyword (color==what we have to revert to
                                                  //if cursor moves away)

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
    bool CursorIn(CshPos &p) const {return p.first_pos<=cursor_pos && cursor_pos<=p.last_pos;}
    bool CursorIn(int a, int b) const {return a<=cursor_pos && cursor_pos<=b;}
    void SetHintsReady(std::set<std::string> &&v);
    std::string HintPrefixNonSelectable() const {return CSH_HINT_NON_SELECTABLE_PREFIX "\\i";}
    std::string HintPrefix(MscColorSyntaxType) const;
    void AddColorValues(std::set<std::string> &v) const;
};

void CshParse(Csh &csh, const char *buff, unsigned len);


#endif
