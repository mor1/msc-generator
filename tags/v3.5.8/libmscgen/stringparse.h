#if !defined(STRINGPARSE_H)
#define STRINGPARSE_H
#include <set>
#include "cairo.h"
#include "numbering.h"
#include "attribute.h"
#include "csh.h"

class Label;
class MscCanvas;

using std::string;

typedef enum
{
    MSC_IDENT_INVALID = 0,
    MSC_IDENT_LEFT,
    MSC_IDENT_CENTER,
    MSC_IDENT_RIGHT
}
MscIdentType;

typedef enum 
{
    MSC_FONT_NORMAL=0, 
    MSC_FONT_SMALL=1, 
    MSC_FONT_SUPERSCRIPT=2, 
    MSC_FONT_SUBSCRIPT=3
}
MscFontType;

enum tristate {no=0, yes, invert};

#define ESCAPE_CHAR_LOCATION ((char)1)
#define ESCAPE_STRING_LOCATION "\x01"
#define ESCAPE_CHAR_NUMBERFORMAT ((char)2)
#define ESCAPE_STRING_NUMBERFORMAT "\x02"

bool CshHintGraphicCallbackForTextIdent(MscCanvas *canvas, CshHintGraphicParam p);

//This class stores string formatting (bold, color, fontsize, etc.)
//Contrary to other attribute types its default constructor contains an empty set
//A chart->defaultStringFormat value is used whenever applied to a context
//It can do operations on a fragment (a string containing no escape sequences)
// - It calculate height or width of a fragment using a drawing context
// - It draw a fragment using a drawing context
class StringFormat {
  protected:
    std::pair<bool, MscColorType> color;
    std::pair<bool, MscFontType>  fontType;   
    std::pair<bool, double>       spacingBelow;
    std::pair<bool, tristate>     bold;
    std::pair<bool, tristate>     italics;
    std::pair<bool, tristate>     underline;
    std::pair<bool, std::string>  face;

    std::pair<bool, double>       textHGapPre, textHGapPost;
    std::pair<bool, double>       textVGapAbove, textVGapBelow;
    std::pair<bool, double>       textVGapLineSpacing;
    std::pair<bool, MscIdentType> ident;

    std::pair<bool, double>       normalFontSize;
    std::pair<bool, double>       smallFontSize;

    mutable cairo_font_extents_t smallFontExtents;
    mutable cairo_font_extents_t normalFontExtents;
    void ApplyFontTo(MscCanvas &) const;
    double spaceWidth(const string &, MscCanvas &, bool front) const;

    typedef enum {FORMATTING_OK, INVALID_ESCAPE, NON_FORMATTING, REFERENCE, NON_ESCAPE, LINE_BREAK, NUMBERING, NUMBERING_FORMAT, SOLO_ESCAPE} EEscapeType;
    EEscapeType ProcessEscape(const char *input, unsigned &length,
                              bool resolve=false, bool apply=false, string *replaceto=NULL, const StringFormat *basic=NULL,
                              Msc *msc=NULL, file_line *linenum=NULL, bool sayIgnore=true);
    friend class Label;
    friend class ParsedLine;

  public:
      typedef enum {LABEL, TEXT_FORMAT, NUMBER_FORMAT} ETextType;
    // Generate the default formatting (all value set == all .second is true)
    StringFormat(void) {Empty();}
    StringFormat &operator =(const StringFormat &f);
    explicit StringFormat(string&text) {Empty(); Apply(text);}
    explicit StringFormat(const char *s) {Empty(); Apply(s);}

    void Empty();
    bool IsComplete() const;
    void Default();

    // Apply a formatting to us, stop at non-formatting escape or a bad formatting one or at one including style/color name
    unsigned Apply(string &escape); //this one removes the escape chars form beginning of input!
    unsigned Apply(const char *s);
    StringFormat &operator +=(const char*s) {Apply(s); return *this;};
    StringFormat &operator +=(const StringFormat& toadd);
    void SetColor(MscColorType c);
    bool AddAttribute(const Attribute &a, Msc *msc, StyleType t);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string &attr, Csh &csh);

    MscIdentType GetIdent() const
        {return ident.first?ident.second:MSC_IDENT_CENTER;}
    string Print() const;
    //Return text geometry
    double getCharHeight(MscCanvas &) const;
    double getFragmentWidth(const string &, MscCanvas &) const;
    double getFragmentHeightAboveBaseLine(const string &, MscCanvas &) const;
    double getFragmentHeightBelowBaseLine(const string &, MscCanvas &) const;
    double getSpacingBelow(void) const
        {return spacingBelow.first?spacingBelow.second:0;}

    //Draw a fragment y specifies baseline (not in cairo sense)
    double drawFragment(const string &, MscCanvas &, XY, bool isRotated) const;

    //This one tells if the string has any escape character or not
    static bool HasEscapes(const char *text);
    //This adds CSH entries to csh. Malformed \c and \s arguments are assumed OK
    static void ExtractCSH(int startpos, const char *text, Csh &csh);
    //This adds a number at the beginning of the string
    //Taking all potential escape sequence at the beginning of the string
    //into account
    static void AddNumbering(string &label, const string &num, const string &pre_num_post);
    //This converts color, style references to primitives
    //unrecognized escapes are left intact
    static void ExpandReferences(string &text, Msc *msc, file_line linenum,
                                 const StringFormat *basic, bool ignore,
                                 ETextType textType);
    //Expands \r() escapes
    static void ExpandElementReferences(string &text, Msc *msc, file_line linenum);
    static int FindNumberingFormatEscape(const char *text);
    static void RemovePosEscapes(string &text);
    static void ConvertToPlainText(string &text);
};

//An object that stores a line (no '\n' inside)
class ParsedLine {
    friend class Label;
protected:
    StringFormat startFormat;
    string     line;
    double     width;
    double     heightAboveBaseLine;
    double     heightBelowBaseLine;
public:
    ParsedLine(const string&, MscCanvas &, StringFormat &sf);
    operator std::string() const;
    void Draw(XY xy, MscCanvas &, bool isRotated) const;
    XY getWidthHeight(void) const
        {return XY(width, heightAboveBaseLine+heightBelowBaseLine);}
};

//A class holding a list of parsed lines
class Label : public std::vector<ParsedLine>
{
    using std::vector<ParsedLine>::size;
    using std::vector<ParsedLine>::at;
protected:
    unsigned AddText(const string &s, MscCanvas &canvas, StringFormat);
    void CoverOrDraw(MscCanvas *canvas, double sx, double dx, double y, double cx, bool isRotated, Contour *area) const;
public:
    Label(const string &s, MscCanvas &c , const StringFormat &f)
        {AddText(s,c,f);}
    Label() {}
    void Set(const string &s, MscCanvas &c, const StringFormat &f) {clear(); AddText(s,c,f);}
    void AddSpacing(unsigned line, double spacing);
    operator std::string() const;

    XY getTextWidthHeight(int line=-1) const;
    Contour Cover(double sx, double dx, double y, double cx=-CONTOUR_INFINITY) const {Contour a; CoverOrDraw(NULL, sx, dx, y, cx, false, &a); return a;}
    void Draw(MscCanvas &canvas, double sx, double dx, double y, double cx=-CONTOUR_INFINITY, bool isRotated=false) const {CoverOrDraw(&canvas, sx, dx, y, cx, isRotated, NULL);}
};


#endif //STRINGPARSE_H
