#if !defined(STRINGPARSE_H)
#define STRINGPARSE_H
#include <set>
#include "mscdrawer.h"


class Label;

typedef enum
{
    MSC_IDENT_INVALID = 0,
    MSC_IDENT_LEFT,
    MSC_IDENT_CENTER,
    MSC_IDENT_RIGHT
}
MscIdentType;

enum tristate {no=0, yes, invert};

class ColorSet;
class StyleSet;

//This class stores string formatting (bold, color, fontsize, etc.)
//It can do operations on a fragment (a string containing no escape sequences)
// - It calculate height or width of a fragment using a drawing context
// - It draw a fragment using a drawing context
class StringFormat {
  protected:
    std::pair<bool, MscColorType> color;
    std::pair<bool, int>          fontType;   //0 large, 1 small, 2 superScript, 3 subscript
    std::pair<bool, double>       spacingBelow;
    std::pair<bool, tristate>     bold;
    std::pair<bool, tristate>     italics;
    std::pair<bool, tristate>     underline;
    std::pair<bool, string>       face;

    std::pair<bool, double>       textHGapPre, textHGapPost;
    std::pair<bool, double>       textVGapAbove, textVGapBelow;
    std::pair<bool, double>       textVGapLineSpacing;
    std::pair<bool, MscIdentType> ident;

    std::pair<bool, double>       normalFontSize;
    std::pair<bool, double>       smallFontSize;

    mutable cairo_font_extents_t smallFontExtents;
    mutable cairo_font_extents_t normalFontExtents;
    void ApplyFontToContext(MscDrawer *) const;
    double spaceWidth(const string &, MscDrawer *, bool front) const;

    friend class Label;

  public:
    // Generate the default formatting (all value set == all .second is true)
    StringFormat(void);
    // Generate a diff formatting from the escape (start with '\')
    // Returns the string to continue parsing on.
    explicit StringFormat(string&);
    explicit StringFormat(const char *s);

    void Empty();

    // Apply a formatting to us
    bool Apply(string &escape); //this one removes the escape char form input!
    bool Apply(const char *s) {string t(s); return Apply(t);}
    StringFormat &operator +=(const char*s) {Apply(s); return *this;};
    StringFormat &operator +=(const StringFormat& toadd);
    void SetColor(MscColorType c);
    bool AddAttribute(const Attribute &a, Msc *msc, StyleType t);

    MscIdentType GetIdent() const
        {return ident.first?ident.second:MSC_IDENT_CENTER;}
    string Print() const;
    //Return text geometry
    double getFragmentWidth(const string &, MscDrawer *) const;
    double getFragmentHeightAboveBaseLine(const string &, MscDrawer *) const;
    double getFragmentHeightBelowBaseLine(const string &, MscDrawer *) const;
    double getSpacingBelow(void) const
        {return spacingBelow.first?spacingBelow.second:0;}

    //Draw a fragment y specifies baseline (not in cairo sense)
    double drawFragment(const string &, MscDrawer *, XY, bool isRotated) const;

    //This adds CSH entries to msc.
    static void ExtractCSH(int startpos, const char *text, Msc &msc);
    //This adds a number at the beginning of the string
    //Taking all potential escape sequence at the beginning of the string
    //into account
    static void AddNumbering(string &label, int num);
    //This converts color names to RGBA numbers, returns the list of
    //unrecognized color names separated by commas, empty string if all is OK.
    //unrecognized color escapes are left intact
    static void ExpandColorAndStyle(string &escape, Msc *msc, file_line linenum,
									const StringFormat *basic, bool label);
};

//An object that stores a line (no '\n' inside)
class ParsedLine {
  protected:
    StringFormat startFormat;
    string     line;
    double     width;
    double     heightAboveBaseLine;
    double     heightBelowBaseLine;

    friend class Label;

  public:
    ParsedLine(const string&, MscDrawer *, StringFormat &sf);
    void Draw(XY xy, MscDrawer *, bool isRotated) const;
    XY getWidthHeight(void) const
        {return XY(width, heightAboveBaseLine+heightBelowBaseLine);}
};

//A class holding a list of parsed lines
class Label :  public std::vector<ParsedLine>
{
    using std::vector<ParsedLine>::size;
    using std::vector<ParsedLine>::at;
protected:
    MscDrawer *msc;
    unsigned AddText(const string &s, StringFormat);
public:
    Label(const string &s, MscDrawer *m , const StringFormat &f): msc(m)
        {AddText(s,f);}
    explicit Label(MscDrawer *m) : msc(m) {}
    void Set(const string &s, const StringFormat &f) {clear(); AddText(s,f);}
    void AddSpacing(unsigned line, double spacing);

    XY getTextWidthHeight(int line=-1) const;
    void DrawCovers(double sx, double dx, double y,
                    Geometry &cover, bool draw, bool isRotated=false);
};


#endif //STRINGPARSE_H
