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

/** @file stringparse.h The declaration of arrowhead styles.
 * @ingroup libmscgen_files */

#if !defined(STRINGPARSE_H)
#define STRINGPARSE_H
#include <set>
#include "cairo.h"
#include "numbering.h"
#include "attribute.h"
#include "csh.h"

class Label;
class Canvas;

using std::string;

/** Describes the values of the 'side' attribute*/
enum class ESide
{
    INVALID = 0,///<The invalid value.
    LEFT,       ///<The left side
    RIGHT,      ///<The right side
    END         ///<At the bottom of the chart (used for comments only)
};

/** Describes text identation */
enum EIdentType {
    MSC_IDENT_INVALID = 0, ///<The invalid value
    MSC_IDENT_LEFT,        ///<Left-aligned
    MSC_IDENT_CENTER,      ///<Centered
    MSC_IDENT_RIGHT        ///<Right-aligned
};

/**  Describes text characteristics */
enum EFontType {
    MSC_FONT_NORMAL=0,      ///<Regular text
    MSC_FONT_SMALL=1,       ///<Small font
    MSC_FONT_SUPERSCRIPT=2, ///<Small font superscript
    MSC_FONT_SUBSCRIPT=3    ///<Small font subscript
};

/** Describes, set, unset and invert for text attributes*/
enum ETriState {no=0, yes, invert};

/** Escape character showing the location in the input file.
 * Preceeded by backslash and followed by a FileLineCol in parenthesis.*/
#define ESCAPE_CHAR_LOCATION ((char)1)
/** Same as ESCAPE_CHAR_LOCATION, but in string*/
#define ESCAPE_STRING_LOCATION "\x01"
/** Escape character representing a numbering format token (abc, 123, etc.).
 * Preceeded by backslash and followed by a code showing the format.*/
#define ESCAPE_CHAR_NUMBERFORMAT ((char)2)
/** Same as ESCAPE_CHAR_NUMBERFORMAT, but in string*/
#define ESCAPE_STRING_NUMBERFORMAT "\x02"
/** Escape character representing a soft new line (a line break in the input file).
 * Preceeded by backslash.*/
#define ESCAPE_CHAR_SOFT_NEWLINE ((char)3)
/** Same as ESCAPE_CHAR_SOFT_NEWLINE, but in string*/
#define ESCAPE_STRING_SOFT_NEWLINE "\x03"
/** Escape character representing a word_wrap status
 * Preceeded by backslash, followed by a + or a - character.*/
#define ESCAPE_CHAR_WORD_WRAP ((char)4)
/** Same as ESCAPE_CHAR_WORD_WRAP, but in string*/
#define ESCAPE_STRING_WORD_WRAP "\x04"
/** Escape character representing a space
* Preceeded by backslash */
#define ESCAPE_CHAR_SPACE ((char)5)
/** Same as ESCAPE_CHAR_WORD_WRAP, but in string*/
#define ESCAPE_STRING_SPACE "\x05"

bool CshHintGraphicCallbackForTextIdent(Canvas *canvas, CshHintGraphicParam p, Csh &);

/**This class stores string formatting (bold, color, fontsize, etc.)
 * Contrary to other attribute types (LineAttr, FillAttr, ShadowAttr,
 * ArrowHead and NoteAttr) its default constructor creates an empty set.
 * The Msc::defaultStringFormat value is used for elements not set, 
 * whenever applied to a context.
 * This class contains the functions to parse labels and resolve
 * escape sequences.
 * This object can do operations on a text fragment. A *fragment* is a piece
 * of a label containing no line breaks and having the same formatting,
 * thus a fragment contains no escape sequences.
 *  - It calculate height or width of a fragment using a drawing context.
 *  - It draw a fragment using a drawing context. */
class StringFormat {
  protected:
    std::pair<bool, ColorType>   color;             ///<The color of the font. Not set if `first` is false.
    std::pair<bool, EFontType>   fontType;          ///<The size/index of the font. Not set if `first` is false.
    std::pair<bool, double>      spacingBelow;      ///<The extra spacing to add below this line. Not set if `first` is false.
    std::pair<bool, ETriState>   bold;              ///<Indicates if the font is thick. Not set if `first` is false.
    std::pair<bool, ETriState>   italics;           ///<Indicates if the font is italic. Not set if `first` is false.
    std::pair<bool, ETriState>   underline;         ///<Indicates if the font is underlined. Not set if `first` is false.
    std::pair<bool, std::string> face;              ///<The face name of the font. Not set if `first` is false.

    std::pair<bool, double>     textHGapPre;        ///<The margin left of the text. Not set if `first` is false.
    std::pair<bool, double>     textHGapPost;       ///<The margin right of the text. Not set if `first` is false.
    std::pair<bool, double>     textVGapAbove;      ///<The margin above the text. Not set if `first` is false.
    std::pair<bool, double>     textVGapBelow;      ///<The margin below the text. Not set if `first` is false.
    std::pair<bool, double>     textVGapLineSpacing;///<The spacing between the lines of the text. Not set if `first` is false.
    std::pair<bool, EIdentType> ident;              ///<The identation/alignment of the text. Not set if `first` is false.

    std::pair<bool, double>      normalFontSize;    ///<The height of normal-sized font. Not set if `first` is false.
    std::pair<bool, double>      smallFontSize;     ///<The height of small, superscript and subscript font. Not set if `first` is false.

    std::pair<bool, bool>        word_wrap;         ///<If true, this label shall be word wrapped

    mutable cairo_font_extents_t smallFontExtents;  ///<Cached extent of small fonts.
    mutable cairo_font_extents_t normalFontExtents; ///<Cached extent of normal-sized fonts.
    void ApplyFontTo(Canvas &) const;
    double spaceWidth(const string &, Canvas &, bool front) const;

    /** Describes the type of an escape sequence, see StringFormat::ProcessEscape(). */
    enum EEscapeType {
        FORMATTING_OK, ///<A syntactically correct formatting escape
        INVALID_ESCAPE,///<A non-recognized escape 
        NON_FORMATTING,///<A non-formatting escape, such as "\{" 
        REFERENCE,     ///<A reference to another element "\r(xxx)"
        NON_ESCAPE,    ///<Literal text, not an escape
        LINE_BREAK,    ///<A line break "\n"
        SOFT_LINE_BREAK,///<A soft line break (a newline in input file of a colon label)
        NUMBERING,     ///<A reference to the number of this label "\N"
        NUMBERING_FORMAT, ///<A replacement for a numbering format token (such as "abc" or "roman")
        SOLO_ESCAPE    ///<A single backslash "\"
    };
    EEscapeType ProcessEscape(const char *input, unsigned &length,
                              bool resolve=false, bool apply=false, string *replaceto=NULL, 
                              const StringFormat *basic=NULL,
                              Msc *msc=NULL, bool references=false, 
                              FileLineCol *linenum=NULL, bool sayIgnore=true);
    friend class Label;
    friend class ParsedLine;

  public:
      /** Describes what kind of text we process*/
      enum ETextType {
          LABEL,         ///<The text is an element (entity, arc, box, etc) label. Cannot contain numbering format token escapes ("\0x2{1aAiI}").
          TEXT_FORMAT,   ///<The text is a value assigned to "text.format" attribute. Cannot contain numbering format token escapes ("\0x2{1aAiI}").
          NUMBER_FORMAT  ///<The text is a value assigned to "numbering.*" attribute. Cannot contain numbering escapes ("\N").
      };
    /** Generate an empty formatting, no `first` is set to true.
     * This is contrary to all other attributes, for which the default
     * constructor generates a fully specified object with the default values.*/
    StringFormat(void) {Empty();}
    StringFormat &operator =(const StringFormat &f);
    /** Parse `text` up until the first non-escape or non-formatting escape and assignes values.*/
    explicit StringFormat(string&text) {Empty(); Apply(text);}
    /** Parse `s` up until the first non-escape or non-formatting escape and assignes values.*/
    explicit StringFormat(const char *s) {Empty(); Apply(s);}

    /** Deletes all values, setting `first` values to false.*/
    void Empty();
    /** True if all attributes of text formatting is specified.*/
    bool IsComplete() const;
    void Default();
    void UnsetWordWrap() {word_wrap.first = true; word_wrap.second = false;}
    bool IsWordWrap() const {_ASSERT(word_wrap.first); return word_wrap.second;}

    /** Parse a sequence of escapes and apply the formatting to us. 
     * If you hit something like a non-formatting escapea or a bad 
     * formatting one or one that includes style/color name, stop, 
     * _remove the processed escapes_ and return.
     * @param [in] text The text to parse.
     * @return The number of characters processed.*/
    unsigned Apply(string &text); 
    /** Parse a sequence of escapes and apply the formatting to us. 
     * If you hit something like a non-formatting escapea or a bad formatting one or 
     * one that includes style/color name, stop and return.
     * @param [in] s The text to parse.
     * @return The number of characters processed.*/
    unsigned Apply(const char *s);
    /** Parse a sequence of escapes and apply the formatting to us. 
     * If you hit something like a non-formatting escapea or a bad formatting one or 
     * one that includes style/color name, stop and return.*/
    StringFormat &operator +=(const char*s) {Apply(s); return *this;}
    /** Merge another StringFormat to us by copying all attributes where `first` is set there.*/
    StringFormat &operator +=(const StringFormat& toadd);
    /** Set the color of the format */
    void SetColor(ColorType c);
    bool AddAttribute(const Attribute &a, Msc *msc, EStyleType t);
    static void AttributeNames(Csh &csh, const string &prefix);
    static bool AttributeValues(const std::string &attr, Csh &csh);

    /** Returns the ident value, a MSC_IDENT_CENTER if not set.*/
    EIdentType GetIdent() const
        {return ident.first?ident.second:MSC_IDENT_CENTER;}
    /** Returns the extra spacing below this line, 0 if not set.*/
    double getSpacingBelow(void) const
        {return spacingBelow.first?spacingBelow.second:0;}
    /** Prints all attributes set as escape sequences */
    string Print() const;

    /** @name Static text manipulation functions
     * @{ */
    static bool HasEscapes(const char *text);
    static void ExtractCSH(int startpos, const char *text, const size_t len, Csh &csh);
    static void AddNumbering(string &label, const string &num, const string &pre_num_post);
    static void ExpandReferences(string &text, Msc *msc, FileLineCol linenum,
                                 const StringFormat *basic, bool references, 
                                 bool ignore, ETextType textType);
    static int FindNumberingFormatEscape(const char *text);
    static void RemovePosEscapes(string &text);
    static void ConvertToPlainText(string &text);
    /** @}*/

    /** @name Graphics: text geometry and drawing.
     * @{ */
    /** Return the total character height of normal-sized text. */
    double getCharHeight(Canvas &) const;
    /** Return the width of a piece of text with our formatting */
    double getFragmentWidth(const string &, Canvas &) const;
    /** Return the height of a piece of text above the base line with our formatting */
    double getFragmentHeightAboveBaseLine(const string &, Canvas &) const;
    /** Return the height of a piece of text below the base line with our formatting */
    double getFragmentHeightBelowBaseLine(const string &, Canvas &) const;
    double drawFragment(const string &, Canvas &, XY, bool isRotated) const;
    /** @}*/
};

/** An object that stores a line of text (no '\n' inside) */
class ParsedLine {
    friend class Label;
protected:
    StringFormat startFormat;        ///<The starting format valid at the beginning of the line
    string       line;               ///<The text of the line (may contain escape sequences, but not \n nor ESCAPE_CHAR_SOFT_NEWLINE)
    double       width;              ///<The pre-computed width of the line in pixels
    double       heightAboveBaseLine;///<The pre-computed height of the line in pixels above the baseline
    double       heightBelowBaseLine;///<The pre-computed height of the line in pixels below the baseline
    bool         hard_new_line;      ///<True if this line is terminated by a hard new line ("\n" escape)
public:
    /** Creates a parsed line from a string.
     * We also specify a canvas to be used at calculating the geometry
     * and a starting format. The latter contains the formatting at the
     * end of the line. We pre-parse the text and determine geometry.*/
    ParsedLine(const string&, Canvas &, StringFormat &sf, bool h);
    /** Converts the line to an escape-free string*/
    operator std::string() const;
    /** Draws the line to a canvas.
     * @param [in] xy Where the text shall be placed, y is the height of the baseline.
     * @param canvas The canvas to draw on.
     * @param [in] isRotated If true, the canvas will use text paths as fallback for surfaces not supporting rotated text (WMF).*/
    void Draw(XY xy, Canvas &canvas, bool isRotated) const;
    /** Returns the width and height of the line.*/
    XY getWidthHeight(void) const
        {return XY(width, heightAboveBaseLine+heightBelowBaseLine);}
};

/** A class holding a list of parsed lines*/
class Label : public std::vector<ParsedLine>
{
    using std::vector<ParsedLine>::at;
    using std::vector<ParsedLine>::clear;
    using std::vector<ParsedLine>::swap;
protected:
    double first_line_extra_spacing; ///<Extra spacing to add after first line (for arrow width). Must survive a reflow.
    /** Helper to determine cover & to draw*/
    void CoverOrDraw(Canvas *canvas, double sx, double dx, double y, double cx, bool isRotated, Contour *area) const;
public:
    using std::vector<ParsedLine>::size;
    /** Creates a Label from a string.
     * We also specify a canvas to be used at calculating the geometry
     * and a starting format. We pre-parse the text and determine geometry.*/
    Label(const string &s, Canvas &c , const StringFormat &f) 
        : first_line_extra_spacing(0) {Set(s,c,f);}
    /** Creates an empty label*/
    Label() : first_line_extra_spacing(0) {}

    /** Set the content.
     * Split a string to substrings based on '\n' delimiters. 
     * Then parse the strings for escape control characters and
     * determine geometry (width and height of each line and in total).
     * @param [in] s The (potentially multi-line) text (potentially with escapes) to set to.
     * @param c The canvas to use to determine geometry.
     * @param [in] f The starting text format.
     * @returns the number of lines in the label.*/
    unsigned Set(const string &s, Canvas &c, StringFormat f);
    /** Add extra spacing below the first line 
     * This space remains even after a reflow. */
    void AddSpacingAfterFirstLine(double spacing) {first_line_extra_spacing += spacing;}
    /** Converts the line to an escape-free string*/
    operator std::string() const;
    /** Applies a text style. Ignores line spacing parts of the style. */
    void ApplyStyle(const StringFormat &sf);
    /** Reflows the text top fit a box of x width.
     * Honours hard line breaks, but not soft ones.
     * @param c The canvas to calculate on.
     * @param x The width to reflow in.
     * @returns nonzero, if there was overflow and the size of overflow. */
    double Reflow(Canvas &c, double x);
    /** Return the width requirement of the label.
     * If word_wrap is off, this is the width of the label.
     * If it is on, it is the first parameter */
    double getSpaceRequired(double def = 0, int line = -1) const;
    /** Returns true if word wrapping is enabled for this label */
    bool IsWordWrap() const {if (size()==0) return false; return at(0).startFormat.word_wrap.first && at(0).startFormat.word_wrap.second;} 

    /** Return the size of a line of text. 
     * @param [in] line The number of the line starting from 0. If -1 we return the total size of all lines.*/
    XY getTextWidthHeight(int line=-1) const;
    /** Return the cover for the label.
     * We lay out the label between `sx` and `dx` according to the ident of each line.
     * Each fragment is modelled as a rectangle.
     * @param [in] sx The left margin.
     * @param [in] dx The right margin.
     * @param [in] y The top of the label.
     * @param [in] cx If also specified, we center around it for centered lines, 
     *                but taking care not to go ouside the margings. 
     *                If the line is wider than `dx-sx` we will go outside
     *                as little as possible (thus we center around `(sx+dx)/2`.
     * @returns The cover of the label.*/
    Contour Cover(double sx, double dx, double y, double cx=-CONTOUR_INFINITY) const {Contour a; CoverOrDraw(NULL, sx, dx, y, cx, false, &a); return a;}
    /** Draw the label onto a canvas 
     * We lay out the label between `sx` and `dx` according to the ident of each line.
     * Each fragment is modelled as a rectangle.
     * @param canvas The canvas to draw onto.
     * @param [in] sx The left margin.
     * @param [in] dx The right margin.
     * @param [in] y The top of the label.
     * @param [in] cx If also specified, we center around it for centered lines, 
     *                but taking care not to go ouside the margings. 
     *                If the line is wider than `dx-sx` we will go outside
     *                as little as possible (thus we center around `(sx+dx)/2`.
     * @param [in] isRotated If true then the canvas will fall back to text_path
     *                       for surfaces that do not support rotated text (WMF)*/
    void Draw(Canvas &canvas, double sx, double dx, double y, double cx=-CONTOUR_INFINITY, bool isRotated=false) const {CoverOrDraw(&canvas, sx, dx, y, cx, isRotated, NULL);}
    /** Return the cover for the a label that can be 90 degree rotated or not.
    * We lay out the label between `s` and `d` according to the ident of each line.
    * Each fragment is modelled as a rectangle.
    * If the label is vertical, s, d and c are Y coordinates and t is X coordinate.
    * If the label is horizontal, vice versa.
    * @param [in] s The left edge of the text (as the text reads)
    *               In the result this is the Left/Top/Bottom edge of text for side==END/LEFT/RIGHT. 
    * @param [in] d The right edge of the text (as the text reads)
    *               In the result this is the Right/Bottom/Top edge of text for side==END/LEFT/RIGHT
    * @param [in] t The top edge of the text (as the text reads)
    *               In the result this is the Top/Right/Left edge of text for side==END/LEFT/RIGHT.  
    * @param [in] side from which direction is the text read. For END it will be laid out horizontally.
    * @param [in] c If also specified, we center around it for centered lines,
    *                but taking care not to go ouside the margings.
    *                If the line is wider than `d-s` we will go outside
    *                as little as possible (thus we center around `(s+d)/2`.
    * @returns The cover of the label.*/
    Contour Cover(double s, double d, double t, ESide side, double c = -CONTOUR_INFINITY) const;
    /** Draw the label rotated 90 degrees onto a canvas
    * We lay out the label between `s` and `d` according to the ident of each line.
    * Each fragment is modelled as a rectangle.
    * If the label is vertical, s, d and c are Y coordinates and t is X coordinate.
    * If the label is horizontal, vice versa.
    * @param canvas The canvas to draw onto.
    * @param [in] s The left edge of the text (as the text reads)
    *               In the result this is the Left/Top/Bottom edge of text for side==END/LEFT/RIGHT.
    * @param [in] d The right edge of the text (as the text reads)
    *               In the result this is the Right/Bottom/Top edge of text for side==END/LEFT/RIGHT
    * @param [in] t The top edge of the text (as the text reads)
    *               In the result this is the Top/Right/Left edge of text for side==END/LEFT/RIGHT.
    * @param [in] side from which direction is the text read. For END it will be laid out horizontally.
    * @param [in] c If also specified, we center around it for centered lines,
    *                but taking care not to go ouside the margings.
    *                If the line is wider than `d-s` we will go outside
    *                as little as possible (thus we center around `(s+d)/2`.*/
    void Draw(Canvas &canvas, double s, double d, double t, ESide side, double c = -CONTOUR_INFINITY) const;
};

#endif //STRINGPARSE_H
