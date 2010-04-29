#if !defined(CSH_H)
#define CSH_H

#include<vector>

struct CshPos
{
  int first_pos;
  int last_pos;
  CshPos() : first_pos(0), last_pos(0) {}
};

typedef enum
{
    COLOR_NORMAL = 0,
    COLOR_KEYWORD,
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

#endif
