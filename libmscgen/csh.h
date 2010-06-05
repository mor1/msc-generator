#if !defined(CSH_H)
#define CSH_H

#include<vector>
#include<string>

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

#endif
