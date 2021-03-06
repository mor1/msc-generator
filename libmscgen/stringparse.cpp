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
/** @file stringparse.cpp Implementation of StringFormat and Label for text functions.
 * @ingroup libmscgen_files */
#include <assert.h>
#include <iostream>
#include "csh.h"
#include "msc.h"

//////////////////////////////////////////////////////////////////////////////////

/** Values for text.ident attribute*/
template<> const char EnumEncapsulator<EIdentType>::names[][ENUM_STRING_LEN] =
    {"invalid", "left", "center", "right", ""};

/** Values for text.type attribute*/
template<> const char EnumEncapsulator<EFontType>::names[][ENUM_STRING_LEN] =
    {"invalid", "normal", "small", "superscript", "subscript", ""};


/** Merge two ETriState values.*/
void AddTristate(std::pair<bool, ETriState> &a, const std::pair<bool, ETriState> b)
{
    if (!b.first) return;
    if (!a.first || b.second != invert) {
        a = b;
        return;
    }
    //here b.second is invert and a.first is true
    switch (a.second) {
    case no: a.second = yes; return;
    case yes: a.second = no; return;
    case invert: 
        a.first = false; 
        return;
    }
}

//////////////////////////////////////////////////////////////////////////////////

void StringFormat::Empty()
{
    color.first = false;
    bgcolor.first = false;
    fontType.first = false;
    spacingBelow.first = false;
    bold.first = false;
    italics.first = false;
    underline.first = false;
    face.first = false;
    textHGapPre.first = false;
    textHGapPost.first = false;
    textVGapAbove.first = false;
    textVGapBelow.first = false;
    textVGapLineSpacing.first = false;
    ident.first = false;
    normalFontSize.first = false;
    smallFontSize.first = false;
    word_wrap.first = false;
    link_format.first = false;
}

bool StringFormat::IsComplete() const
{
    return 
        color.first &&
        bgcolor.first &&
        fontType.first &&
        spacingBelow.first &&
        bold.first && bold.second!=invert &&
        italics.first && italics.second!=invert &&
        underline.first && underline.second!=invert &&
        face.first &&
        textHGapPre.first &&
        textHGapPost.first &&
        textVGapAbove.first &&
        textVGapBelow.first &&
        textVGapLineSpacing.first &&
        ident.first &&
        normalFontSize.first &&
        smallFontSize.first &&
        word_wrap.first &&
        link_format.first;
}

/** Sets the fully specified default font.
 * Font sizes are 16 and 10 point, all margins at 2,
 * no line spacing, centered, black, not bold, not italic,
 * not underlined, face name is empty. */
void StringFormat::Default() 
{
    normalFontSize.first = true; normalFontSize.second = 16; 
    smallFontSize.first = true; smallFontSize.second = 10;
    textHGapPre.first = true; textHGapPre.second = 2; 
    textHGapPost.first = true; textHGapPost.second = 2;
    textVGapAbove.first = true; textVGapAbove.second = 2;
    textVGapBelow.first = true; textVGapBelow.second = 2;
    textVGapLineSpacing.first = true; textVGapLineSpacing.second = 0; 
    ident.first = true; ident.second = MSC_IDENT_CENTER;
    color.first = true; color.second =  ColorType(0,0,0); 
    bgcolor.first = true; bgcolor.second = ColorType(0,0,0,0); //fully transparent
    fontType.first = true; fontType.second = MSC_FONT_NORMAL;
    spacingBelow.first = true; spacingBelow.second =  0; 
    face.first = true; face.second.clear();
    bold.first = true; bold.second = no;
    italics.first = true; italics.second = no; 
    underline.first = true; underline.second =  no;
    word_wrap.first = true; word_wrap.second = false;
    link_format.first = true; link_format.second = "\\c(0,0,1)\\U";
}

StringFormat &StringFormat::operator =(const StringFormat &f)
{
    color = f.color;
    bgcolor = f.bgcolor;
    fontType = f.fontType;
    spacingBelow = f.spacingBelow;
    bold = f.bold;
    italics = f.italics;
    underline = f.underline;
    face.first = f.face.first;
    face.second = f.face.second;

    textHGapPre = f.textHGapPre;
    textHGapPost = f.textHGapPost;
    textVGapAbove = f.textVGapAbove;
    textVGapBelow = f.textVGapBelow;
    textVGapLineSpacing = f.textVGapLineSpacing;
    ident = f.ident;

    normalFontSize = f.normalFontSize;
    smallFontSize = f.smallFontSize;

    smallFontExtents = f.smallFontExtents;
    normalFontExtents = f.normalFontExtents;

    word_wrap = f.word_wrap;

    link_format = f.link_format;

    return *this;
}

/** Recognize and process one escape sequence.
 * The escape is sought for at at the beginning of `input`. The string iteself is not
 * changed, but a suggested replacement/resolution is returned in `replaceto`.
 * References to styles and colors are resolved from the top of `msc->Contexts`,
 * references to the default style, color, etc. (via "\s()" or "\c()") are taken
 * from `basic` if supplied.
 * In general, `linenum` carries the location of the first char of input. 
 * `When` we return, it contains the position
 * of the character in `input`+`length`. (In the original input file this can e.g., 
 * be in a different line.)
 * If `linenum`!=NULL then any problem generates an Error/Warning. 
 * If `sayIgnore` is true the we say we ignore the error,
 * otherwise we say we keep the problematic escape verbatim.
 *
 * This function recognize the following escape sequences:
 * - "\-" - switch to small font (also "\s")
 * - "\+" - switch to normal font
 * - "\^" - switch to superscript
 * - "\_" - swucth to subscript
 * - "\b\B\u\U\i\I" - to set bold, underline, italics
 * - "\mX(num)" - set text margings (X=_u_p, _d_own, _l_eft, _r_ight)
 * -             and font size (X=_s_mall, _n_ormal)
 * -             E.g. "\mu(7)" sets upper margin to 7.
 * - "\c(color)" - set color, E.g., "\c(0,0,0)" is black
 * - "\C(color)" - set bgcolor, E.g., "\C(0,0,0,0)" is transparent
 * - "\pX"- set paragraph ident to _c_enter, _l_eft, _r_ight
 * - "\0".."\9" - keep this much of line space after the line
 * - "\\" - an escaped "\"
 * - \#[]{}"; - an escaped chars
 * - "\|" - a zero length non-formatting escape. Can be used to separate number from initial escapes.
 * - "\n" - a line break
 * - "\0x3" - a soft line break, which is replaced to a space when doing word wrapping
 * - "\ N" - insert line number here - should not appear in a numbering format
 * - "\ r(ref)" - insert the number line of another entity here - "\ r()" equals "\ N"
 * - "\0x21" "\0x2a" "\0x2A" "\0x2i" "\0x2I" - used to define numbering formats, should not appear in a label
 * - "\0x1(file,line,col)" - notes the position of the next char in the original file
 *
 * @param [in] input The string to recognize.
 * @param [out] length Returns the length of the escape found 
 *                     (or for NON_ESCAPE the offset to the next '\').
 * @param [in] resolve If true we resolve color/style references (stored in Msc::Contexts). 
 *   (If msc==NULL, or we do not find the stye/color we return INVALID_ESCAPE)
 *   If apply==true, resolve parameter is ignored, we assume resolve==true.
 *   This parameter has impact only on "\s" and "\c" escapes and only if msc!=NULL.
 * @param [in] apply If true then besides parsing, we apply the string formatting escape 
 *                   to "this". (Do nothing for invalid escapes.)
 * @param [out] replaceto If not NULL, return a suggested resolution for the escape.
 * - for FORMATTING_OK we return what to replace the escape to:
 *    + color names are resolved to actual RGBA numbers in a syntax suitable for MscColor::MscColor();
 *    + style names are resolved to actual formatting instructions (a lot of "\b\i" and similar escapes);
 *    + empty parenthesis escapes "\c()", "\s()", "\f()", "\mX()" are replaced with the respective 
 *      value in `basic` or if that is NULL, then are left in place.
 * - for LINK_ESCAPE, we return the parameter, the URL itself
 * - for NON_FORMATTING we return the character represented by the escape (can be zero length for "\|")
 * - for NON_ESCAPE we return the text verbatim
 * - for LINE_BREAK and INVALID_ESCAPE we return empty string
 * - for REFERENCE we attempt to resolve it to an element number if `references` is 
 *   true. If so and we do not find this reference, we generate an error. If 
 *   `references` is false, we return the escape verbatim.
 * - for NUMBERING and NUMBERING_FORMAT we return the actual escape verbatim
 * - for SOLO_ESCAPE we return a single backslash
 * @param [in] basic This is a string format that is used for resolving empty "\c()", "\s()", "\f()", "\mX()"
 *                   escapes. If NULL than those are returned verbatim in `replaceto`.
 * @param msc This object is used as a source of context for style, color names and references.
 *            We also use it to send errors to. If NULL, we do not generate errors nor do we
 *            resolve style, color names and references.
 * @param [in] references If true, indicates that msc->References is complete and therefore we attempt 
 *                        to replace "\r" escapes with actual numbers. (Empty "\r()" is always treated
 *                        as equal to "\N" and we return NUMBERING.)
 *                        If true, we emit error/warning messages only in relation to "\r" escapes,
 *                        others are silently ignored. This allows calling this function twice, once
 *                        with references=false and once with references=true.
 * @param linenum When called, this contains the position of the first character of `input`.
 *                At return it is updated (if not NULL) to point to the first character after
 *                the escape.
 * @param [in] sayIgnore If true we use language in error messages that say we ignore
 *                       a bad escape. If false, we say that we keep it verbatim.
 * @returns The code for what is found at the beginning of `input`:
 * - a valid format-changing escape sequence (FORMATTING_OK)
 * - an URL escape (URL_ESCAPE) - here replaceto contains the parameter (the URL itself)
 * - an escape resulting in no formatting, but also no characters (FORMATTING_OK)
 * - an invalid escape sequence (INVALID_ESCAPE)
 * - a non-format-changing escape sequence (e.g., \\) (NON_FORMATTING)
 * - just literal text up until the next escape (NON_ESCAPE)
 * - a "\n" escape (LINE_BREAK)
 * - a soft line break (SOFT_LINE_BREAK)
 * - a "\r(xxx)" escape (REFERENCE) - no lookups or replacement is offered if `references` is false
 * - a "\N" or "\r()" escape (NUMBERING) - the location of the number in this label
 * - a "\0x2{1aAiI}" escape (NUMBERING_FORMAT)
 * - a lone '\' at the end of the string (SOLO_ESCAPE)
 */
StringFormat::EEscapeType StringFormat::ProcessEscape(
	const char * const input, size_t &length,
	bool resolve, bool apply, string *replaceto, const StringFormat *basic,
	Msc *msc, bool references, FileLineCol *linenum, bool sayIgnore)
{
    /** The message saying that the use of "\s" escape for small text is deprecated.*/
    static const char PER_S_DEPRECATED_MSG[] = 
        "The use of '\\s' control escape to indicate small text is deprecated. Use '\\-' instead.";
    /** Maximum margin for text margins */
    static const double MAX_ESCAPE_M_VALUE = 500;
    /** The explanatory message for out-of bound margins */
    static const char TOO_LARGE_M_VALUE_MSG[] = "Use an integer between [0..500].";

    ColorType c;
    string parameter;
    string maybe_s_msg;
    StyleSet::const_iterator i;
    const string errorAction = sayIgnore ? " Ignoring it." : " Keeping it as verbatim text.";
    const char *end;
    int was_m;

    //If this is not an escape, search for the next escape
    if (input[0]!='\\') {
        length = 0;
        while (input[length] && input[length]!='\\') {
            //If there is an unescaped ']' or '}' in the verbatim string give a warning
            if (linenum && msc && !references)
                if (input[length] == '}' || input[length] == ']') 
                    msc->Error.Warning(FileLineCol(linenum->file, linenum->line, linenum->col+length),
                    string("'") + input[length] + "' character found in label without escape. Is this what you want?",
                    "Insert a '\\' in front of it to remove this warning.");
            length++;
        }
        if (replaceto) replaceto->assign(input, length);
        if (linenum) linenum->col += length;
        return NON_ESCAPE;
    }

    //First check for two-or three character escapes not taking an argument
    switch (input[1]) {
	case 0:      //End of string (string ends with single '\'), replace to quoted version
        length = 1;
        if (replaceto) *replaceto = "\\";
        if (linenum) linenum->col += length;
        return SOLO_ESCAPE;

    case '-':    // small font
    case '+':    // normal font
    case '^':    // superscript
    case '_':    // subscript
        if (apply) {
            fontType.first = true;
            fontType.second = (EFontType)string("+-^_").find(input[1]);
        }
        goto ok2; //valid formatting character of length 2

    case 'b':    // bold font
    case 'B':
        if (apply)
            AddTristate(bold, std::pair<bool,ETriState>(true, input[1]=='B'?yes:invert));
        goto ok2; //valid formatting character of length 2

    case 'i':    // bold font
    case 'I':
        if (apply)
            AddTristate(italics, std::pair<bool,ETriState>(true, input[1]=='I'?yes:invert));
        goto ok2; //valid formatting character of length 2

    case 'u':    // bold font
    case 'U':
        if (apply)
            AddTristate(underline, std::pair<bool,ETriState>(true, input[1]=='U'?yes:invert));
        goto ok2; //valid formatting character of length 2

    case '0': //Line spacing mark
    case '1': //Line spacing mark
    case '2': //Line spacing mark
    case '3': //Line spacing mark
    case '4': //Line spacing mark
    case '5': //Line spacing mark
    case '6': //Line spacing mark
    case '7': //Line spacing mark
    case '8': //Line spacing mark
    case '9': //Line spacing mark
        if (apply) {
            spacingBelow.first = true;
            spacingBelow.second = input[1] - '0';
        }
        goto ok2; //valid formatting character of length 2

    case 'p':
        EIdentType id;
        switch (input[2]) {
        case 'c': id = MSC_IDENT_CENTER; break;
        case 'l': id = MSC_IDENT_LEFT; break;
        case 'r': id = MSC_IDENT_RIGHT; break;
        default: id = MSC_IDENT_INVALID; break;
        }
        if (id == MSC_IDENT_INVALID) {
            if (msc && linenum && !references) {
                FileLineCol l = *linenum;
                l.col += 2;
                msc->Error.Warning(l, "Escape '\\p' shall be followed by one of 'lrc'." + errorAction);
            }
            length = 2;
            goto nok;
        }
        if (apply) {
            ident.first = true;
            ident.second = id;
        }
        length = 3;
        goto ok;

    case ESCAPE_CHAR_SOFT_NEWLINE: //soft line breaks: should disappear after splitting a parsedline into fragments
        length = 2;
        if (replaceto) replaceto->clear();
        if (linenum) linenum->col += length;
        return SOFT_LINE_BREAK;

    case 'n':           // enter: should disappear after splitting a parsedline into fragments
        length = 2;
        if (replaceto) replaceto->clear();
        if (linenum) linenum->col += length;
        return LINE_BREAK;

    case 'N': // location of label numbering
        length = 2;
        if (replaceto)
            *replaceto = "\\N";
        if (linenum) linenum->col += length;
        return NUMBERING;

    case ESCAPE_CHAR_NUMBERFORMAT:
        length = 3;
        if (!strchr("1aAiI", input[2])) { //not one of 1, a, A, i, I
            if (replaceto) replaceto->clear();
            if (msc && !references) msc->Error.Error(*linenum, "Internal error: bad number format escape.");
            return FORMATTING_OK;
        }
        if (replaceto) replaceto->assign(input, length);
        //No change to linenum, these should always be followed by position escapes
        return NUMBERING_FORMAT;

    case ESCAPE_CHAR_WORD_WRAP:
        length = 3;
        _ASSERT(input[2] == '+' || input[2] == '-');
        if (apply) {
            word_wrap.first = true;
            word_wrap.second = (input[2] == '+');
        }
        goto ok;

    case '\\':          // escaped "\"
    case '#':           // escaped "#"
    case '{':           // escaped "{"
    case '}':           // escaped "}"
    case '[':           // escaped "["
    case ']':           // escaped "]"
    case ';':           // escaped ";"
    case '\"':          // escaped quotation mark
        length = 2;
        if (replaceto) replaceto->assign(input+1, 1);
        if (linenum) linenum->col += length;
        return NON_FORMATTING;

    case ESCAPE_CHAR_SPACE: //escaped space
        length = 2;
        if (replaceto) replaceto->assign(" ");
        if (linenum) linenum->col += length;
        return NON_FORMATTING;

    case '|':          // zero-length non-formatting escape
        length = 2;
        if (replaceto) replaceto->clear();
        if (linenum) linenum->col += length;
        return NON_FORMATTING;
    }

    if (!strchr(ESCAPE_STRING_LOCATION "cCsfmrL" ESCAPE_STRING_NON_FORMATTING_LINK, input[1])) {
        //Unrecognized escape comes here
        length = 2;
        if (msc && linenum && !references)
            msc->Error.Error(*linenum, "Unrecognized escape: '" + string(input, 2) + "'." + errorAction);
        goto nok;
    }

    //All the following escapes take a parameter in parenthesis
    was_m = (input[1] == 'm');

    if (input[was_m+2]!='(') {
        //report length of 3 if we have a \m escape followed by one of 'udlrins' (valid)
        length = 2 + (was_m && strchr("udlrins", input[2]));
        if (input[1] == 's') {
            maybe_s_msg = "Missing style name after \\s control escape. Assuming small text switch.";
            goto maybe_s;
        }
        //skip silently if a location escape: we have inserted a bad one???
        if (input[1] == ESCAPE_CHAR_LOCATION) {
            if (replaceto) replaceto->clear();
            if (msc && !references) msc->Error.Error(*linenum, "Internal error: no '(' after position escape.");
            return FORMATTING_OK;
        }

        if (msc && linenum && !references) {
            FileLineCol l = *linenum;
            l.col += 2+was_m;
            msc->Error.Error(l, "Missing parameter after " + string(input, length) +
                             " control escape." + errorAction,
                             "This escape requires a parameter in parenthesis right after it.");
        }
        goto nok;
    }
    end = strchr(input+was_m, ')');
    if (!end) {
        if (input[1] == 's') {
            maybe_s_msg = "Missing closing parenthesis after \\s control escape. Assuming small text switch.";
            goto maybe_s;
        }
        //skip silently if a location escape: we have inserted a bad one???
        if (input[1] == ESCAPE_CHAR_LOCATION) {
            if (replaceto) replaceto->clear();
            if (msc && !references) msc->Error.Error(*linenum, "Internal error: no matching ')' for position escape.");
            return FORMATTING_OK;
        }
        if (msc && linenum && !references)
            msc->Error.Error(*linenum, "Missing closing parenthesis after " + string(input, length) +
                             " control escape." + errorAction);
        length = strlen(input);
        goto nok;
    }
    length = end-input+1; //since we have both ( and ) found, we have at least () after the escape
    parameter.assign(input+was_m+3, length-was_m-4); //stuff inside parenthesis

    //start with escapes taking a string value as parameter
    switch (input[1]) {
    case ESCAPE_CHAR_LOCATION:
        if (replaceto) replaceto->clear();
        if (linenum) {
            FileLineCol l = *linenum;
            if (3!=sscanf(parameter.c_str(), "%d,%u,%u", &l.file, &l.line, &l.col)) {
                if (msc && !references) msc->Error.Error(*linenum, "Internal error: could not parse position escape.");
                return FORMATTING_OK;
            }
            *linenum = l;
        }
        return FORMATTING_OK;
    case 'L':
        if (replaceto) replaceto->assign(parameter);
        if (linenum) linenum->col += length;
        return LINK_ESCAPE;
    case ESCAPE_CHAR_NON_FORMATTING_LINK:
        if (replaceto) replaceto->assign(parameter);
        if (linenum) linenum->col += length;
        return LINK2_ESCAPE;
    case 'c':
    case 'C':
        if (length==4) { // this is a "\c()" or a \C()
            if (basic == NULL) {
                if (replaceto) {
                    if (input[1]=='c')
                        *replaceto = "\\c()";
                    else 
                        *replaceto = "\\C()";
                }
                if (linenum) linenum->col += length;
                return FORMATTING_OK;
            }
            //substitute parameter to the value from basic
            parameter = (input[1]=='c' ? basic->color : basic->bgcolor).second.Print().substr(1);
            parameter.substr(0, parameter.length()-1);
        }
        if (msc)
            c = msc->Contexts.back().colors.GetColor(parameter); //consider color names and defs
        else if (apply || resolve) //try to resolve this if we are applying
            c = ColorType(parameter);  //just consider defs
        else	   //if we are just parsing (probably for csh) keep as is.
            goto ok;
        if (c.type!=ColorType::INVALID) {
            if (apply) {
                if (input[1]=='c') {
                    color.first = true;
                    color.second = c;
                } else {
                    bgcolor.first = true;
                    bgcolor.second = c;
                }
            }
            if (replaceto) replaceto->assign("\\c" + c.Print());
            if (linenum) linenum->col += length;
            return FORMATTING_OK;
        }
        if (msc && linenum && !references) {
            FileLineCol l = *linenum;
            l.col += 3;
            msc->Error.Error(l, "Unrecognized color name or definition: '" + parameter + "'." + errorAction);
        }
        goto nok;

    case 's':
        if (length==4) { // this is a "\s()"
            if (basic == NULL) {
                if (replaceto) *replaceto="\\s()";
                if (linenum) linenum->col += length;
                return FORMATTING_OK;
            }
            if (replaceto) replaceto->assign((*this-*basic).Print());
            if (linenum) linenum->col += length;
            if (apply)
                *this += *basic;
            return FORMATTING_OK;
        }
        if (!msc) {
            if (apply || resolve) //drop silently if there is a style in a place where it should not be
                goto nok;
            else	   //if we are just parsing (probably for csh) keep as is.
                goto ok;
        }
        i = msc->Contexts.back().styles.find(parameter);
        if (i==msc->Contexts.back().styles.end()) {
            maybe_s_msg="Unrecognized style '" + parameter +
                "'. Treating style name as small text in parenthesis.";
            goto maybe_s;
        } else {
            if (i->second.read().f_text) {
                if (apply)
                    *this += i->second.read().text;
                if (replaceto) replaceto->assign((*this-i->second.read().text).Print());
            } else
                if (replaceto)
                    replaceto->clear();
            if (linenum) linenum->col += length;
            return FORMATTING_OK;
        }

    case 'r':
        if (length==4) { // this is a "\r()"
            if (replaceto)
                *replaceto = "\\N";
            if (linenum) linenum->col += length;
            return NUMBERING;
        }
        //suggest the number
        if (replaceto && msc && references) {
            auto itr = msc->ReferenceNames.find(parameter);
            if (itr!=msc->ReferenceNames.end()) {
                *replaceto = itr->second.number_text;
            } else if (linenum) { 
                //here we did not find the reference
                msc->Error.Error(*linenum, "Unrecognized reference '" + parameter +
                                        "'. Ignoring it.", "References are case-sensitive.");
            }
        } else if (replaceto) { 
            //If we could not replace the reference, we add a linenum here from ArcLabelled::AddAttributeList()
            //(during the first call of this function), so that when we get back here later in 
            //from ArcLabelled::FinalizeLabels() we can have a valid line number, so any error above will be OK.
            if (linenum) {
                *replaceto = linenum->Print();
                replaceto->append(input, length);
            } else {
                replaceto->assign(input, length);
            }
        }
        if (linenum) linenum->col += length;
        return REFERENCE;
    case 'f':
        if (length==4) { // this is a "\f()"
            if (basic == NULL) {
                if (replaceto) *replaceto="\\f()";
                if (linenum) linenum->col += length;
                return FORMATTING_OK;
            }
            //substitute parameter to the value from basic
            parameter = basic->face.second;
        } else if (msc && linenum && !references) {
            FileLineCol l = *linenum;
            l.col += 3;
            string font_name = parameter;
            if (!Canvas::HasFontFace(font_name)) {
                if (font_name.length()) {
                    msc->Error.Warning(l, "Font '" + parameter + "' not found. "
                        "Using '"+font_name+"' instead.");
                    parameter = font_name;
                } else {
                    msc->Error.Error(l, "Font '" + parameter + "' not found "
                        "and no substitute available. Ignoring option.");
                    goto nok;
                }
            }
        }
        if (apply) {
            string font_name = parameter;
            Canvas::HasFontFace(font_name);
            if (font_name.length()) {
                face.first = true;
                face.second = font_name;
            }
        }
        if (replaceto) replaceto->assign("\\f(" + parameter + ")");
        if (linenum) linenum->col += length;
        return FORMATTING_OK;

    case 'm':
        typedef std::pair<bool, double> BoolDouble;
        BoolDouble *p;
        int modifer = 0;
        switch (input[2]) {
        case 'u': p = &textVGapAbove; modifer = +2; break;
        case 'd': p = &textVGapBelow; modifer = +2; break;
        case 'l': p = &textHGapPre;   modifer = +4; break;
        case 'r': p = &textHGapPost;  modifer = +2; break;
        case 'i': p = &textVGapLineSpacing; break;
        case 'n': p = &normalFontSize; break;
        case 's': p = &smallFontSize; break;
        default: p = NULL; break;
        }
        if (!p) {
            if (msc && linenum && !references) {
                FileLineCol l = *linenum;
                l.col += 2;
                msc->Error.Warning(l, "Escape '\\m' shall be followed by one of 'udlrins'." + errorAction);
            }
            length = 2;
            goto nok;
        }
        if (parameter.length()==0) { //this is \mX()
            if (basic) {
                const size_t offset = (const char*)p - (const char*)this;
                const BoolDouble *p2 = reinterpret_cast<const BoolDouble *>((const char*)basic + offset);
                if (p2->first) {
                    if (apply)
                        *p = *p2;
                    if (replaceto) {
                        replaceto->assign("\\m");
                        (*replaceto) << input[2] << "(" << p2->second << ")";
                    }
                    if (linenum) linenum->col += length;
                    return FORMATTING_OK;
                }
	            if (replaceto) replaceto->clear();
            } else {
                if (replaceto) {
                    *replaceto = "\\mX()";
                    (*replaceto)[2] = input[2];
                }
            }
            if (linenum) linenum->col += length;
            return FORMATTING_OK;
        }
        //OK, now we know we have a valid escape with a non-empty parameter, digest number
        size_t local_pos = 0;
        double val = 0;
        while (parameter.length()>local_pos && parameter[local_pos]>='0' && parameter[local_pos]<='9') {
            val = val*10 + parameter[local_pos]-'0';
            local_pos++;
        }
        if (parameter.length()>local_pos) {
            string msg = "Invalid value to the '\\m";
            msg += input[2];
            msg.append("' control escape: '").append(parameter).append("'.");
            if (val>MAX_ESCAPE_M_VALUE) {
                msg.append(" I could deduct '").append(parameter.substr(0, local_pos));
                msg.append("', but that seems too large.");
                if (sayIgnore)
                    msg.append(" Ignoring control escape.");
                else
                    msg.append(" Keeping escape as verbatim text.");
                if (msc && linenum && !references) {
                    FileLineCol l = *linenum;
                    l.col += 4;
                    msc->Error.Error(l, msg, TOO_LARGE_M_VALUE_MSG);
                }
                goto nok;
            }
            msg.append(" Using value '").append(parameter.substr(0, local_pos));
            msg.append("' instead.");
            if (msc && linenum && !references) {
                FileLineCol l = *linenum;
                l.col += 4;
                msc->Error.Error(l, msg, TOO_LARGE_M_VALUE_MSG);
            }
        } else if (val>MAX_ESCAPE_M_VALUE) { //Ok here we successfully parsed the number
            string msg = "Too large value after the '\\m";
            msg += input[2];
            msg.append("' control escape: '").append(parameter).append("'.");
            if (sayIgnore)
                msg.append(" Ignoring control escape.");
            else
                msg.append(" Keeping escape as verbatim text.");
            if (msc && linenum && !references) {
                FileLineCol l = *linenum;
                l.col += 4;
                msc->Error.Error(*linenum, msg, TOO_LARGE_M_VALUE_MSG);
            }
            goto nok;
        }
        //OK, here good value we have
        if (apply) {
            p->first = true;
            p->second = val + modifer;
        }
        //It is this complicated because it may be that we use just (the valid) part of parameter
        if (replaceto) replaceto->assign(string(input, 3) + "(" + parameter.substr(0, local_pos) + ")");
        if (linenum) linenum->col += length;
        return FORMATTING_OK;
    } /* big switch */

    //fallthrough, but we should not be here
    nok:
    if (replaceto) replaceto->clear();
    if (linenum) linenum->col += length;
    return INVALID_ESCAPE;


    ok2:
        length = 2;
        //fallthrough
    ok:
        if (replaceto) replaceto->assign(input, length);
        if (linenum) linenum->col += length;
        return FORMATTING_OK;

    maybe_s: //we get here when it is \s, but not a valid style or ()s after
        if (replaceto) replaceto->assign("\\-");
        length = 2;
        if (apply) {
            fontType.first = true;
            fontType.second = MSC_FONT_SMALL;
        }
        if (msc && linenum && !references)
            msc->Error.Warning(*linenum, maybe_s_msg, PER_S_DEPRECATED_MSG);
        if (linenum) linenum->col += length;
        return FORMATTING_OK;
}

/** Tells if the string has any escape character or not*/
bool StringFormat::HasEscapes(const char *text)
{
    if (text==NULL || text[0]==0) return false;
    StringFormat sf;
    const size_t len = strlen(text);
    size_t pos=0;
    while (pos<len) {
        size_t length;
        const EEscapeType t = sf.ProcessEscape(text+pos, length);
        if (t!=NON_ESCAPE && t!=SOLO_ESCAPE) return true;
        pos+=length;
    }
    return false;
}

/** Tells if the string has link escape character ('\\L') or not*/
bool StringFormat::HasLinkEscapes(const char *text)
{
    if (text==NULL || text[0]==0) return false;
    StringFormat sf;
    const size_t len = strlen(text);
    size_t pos = 0;
    while (pos<len) {
        size_t length;
        const EEscapeType t = sf.ProcessEscape(text+pos, length);
        if (t==LINK_ESCAPE || t==LINK2_ESCAPE) return true;
        pos += length;
    }
    return false;
}


/** Adds CSH entries to csh for each formatting escape and the verbatim text.
 * Malformed "\c" and "\s" arguments are assumed OK
 * @param [in] startpos The location of the first byte of `text` in the input file.
 * @param [in] text The text to process.
 * @param [in] len The length of the text to process. If text has a zero
 *                 before len, we terminate processing there.
 * @param csh The object collecting the entries.
 * @returns what kind of hint shall we provide based on the cursor position in 'csh'.
 *               If we shall provide escape hints also set hintedStringPos to the escape (all of it),
 *               if we shall provide parameter hint, we set hintedStringPos to the parameter itself.*/
EEscapeHintType StringFormat::ExtractCSH(int startpos, const char *text, const size_t len, Csh &csh)
{
    _ASSERT(len<32000000U); //safety against negative numbers
    _ASSERT(len<=strlen(text)); //safety against idiotic callers
    EEscapeHintType ret = HINTE_NONE;
    if (text==NULL) return ret;
    size_t pos = 0;
    StringFormat sf;
    while (pos<len) {
        size_t length;
        const EEscapeType escape = sf.ProcessEscape(text+pos, length);
        CshPos loc;
        loc.first_pos = startpos + pos;
        loc.last_pos =  startpos + std::min(pos+length, len)-1;
        switch (escape) {
        case SOLO_ESCAPE:
            if (csh.cursor_pos==loc.last_pos) {
                ret = HINTE_ESCAPE;
                csh.hintedStringPos = loc;
            }
            //fallthrough
        case NON_ESCAPE:
            if (length==0)
                return ret; //len>actual string length - we are done
            //fallthrough
        default:
            csh.AddCSH(loc, COLOR_LABEL_TEXT);
            break;
        case NUMBERING_FORMAT:
        case NUMBERING:
        case LINE_BREAK:
        case SOFT_LINE_BREAK:
        case FORMATTING_OK:
        case LINK_ESCAPE:
        case LINK2_ESCAPE:
        case NON_FORMATTING:
        case REFERENCE:
        case INVALID_ESCAPE:
            if (escape == INVALID_ESCAPE)
                csh.AddCSH_Error(loc, "Invalid escape sequence.");
            else {
                //check if we have a parameter
                int p = loc.first_pos;
                while (p<=loc.last_pos) 
                    if (text[p-startpos]=='(') break;
                    else p++;
                const bool ends_in_par = text[loc.last_pos-startpos]==')';
                const int end = loc.last_pos - ends_in_par;
                if (p<=loc.last_pos && end>p) {
                    //Label has a param
                    csh.AddCSH(CshPos(loc.first_pos, p), COLOR_LABEL_ESCAPE); 
                    csh.AddCSH(CshPos(p+1, end), COLOR_ATTRVALUE);
                    if (ends_in_par)
                        csh.AddCSH(CshPos(end+1, end+1), COLOR_LABEL_ESCAPE);
                } else
                    csh.AddCSH(loc, COLOR_LABEL_ESCAPE); //add label color for all escape
            }
            if (csh.CursorIn(loc)==CURSOR_IN) {
                //Assume cursor does not stand in a parameter
                //->we hint the whole escepa
                ret = HINTE_ESCAPE;
                csh.hintedStringPos = loc;
                //check if there is a parenthesis before the cursor
                for (unsigned u = csh.cursor_pos+1 - loc.first_pos; u>0; u--)
                    if (text[pos+u-1] == ')') {
                        //after the closing parenthesis do not hint
                        //(but otherwise hint at the end of the escape)
                        ret = HINTE_NONE;
                        break; 
                    } else if (text[pos+u-1] == '(') {
                        //Yes, now the cursor stands in a parameter.
                        //Update ret and hintedstringpos
                        switch (text[pos+1]) {
                        case 'c': ret = HINTE_PARAM_COLOR; break;
                        case 's': ret = HINTE_PARAM_STYLE; break;
                        case 'f': ret = HINTE_PARAM_FONT; break;
                        case 'r': ret = HINTE_PARAM_REF; break;
                        case ESCAPE_CHAR_NON_FORMATTING_LINK: //fallthrough
                        case 'L': ret = HINTE_PARAM_LINK; break;
                        default: _ASSERT(0); //fallthrough
                        case 'm': ret = HINTE_PARAM_NUMBER; break;
                        }
                        csh.hintedStringPos.first_pos += u;
                        if (text[pos+length-1] == ')')
                            csh.hintedStringPos.last_pos--;
                        break;
                    }
            }
        }
        pos+=length;
    }
    return ret;
}

/** Replaces style, color names and URL marks to actual definitions.
 * Definitions are pulled from in msc->Contexts.back()
 * and in msc->ReferenceNames.
 * Also performs syntax error checking and generates errors/warnings into msc->Error.
 * @param text The text to process.
 * @param msc The chart to add errors to and a source for style, color and reference names.
 * @param [in] linenum The location of the first byte of `text` in the input file.
 * @param [in] basic The formatting to use for empty "\s()" "\c()" "\mX()" and "\f()" escapes.
 *                   Can be NULL if not available, in this case the above escapes are left as is.
 *                   If not null, it must point to a fully specified StringFormat object.
 * @param [in] references True if the msc->References are complete. If false, "\r" escapes left intact.
 *                        If true, we emit error/warning messages only for "\r" substitution.
 * @param [in] ignore If true then in error messages we say we ignore the erroneous escape and also 
 *                    remove it from the string, if not then we say we keep verbatim and we do so.
 * @param [in] textType The type of text we process. This is to generate the right errors. 
 *                      NUMBER_FORMAT cannot contain "\N", whereas LABEL and TEXT_FORMAT cannot contain 
 *                      numbering format token escapes ("\0x2{1aAiI}").*/
void StringFormat::ExpandReferences(string &text, Msc *msc, FileLineCol linenum,
                                    const StringFormat *basic, bool references,
                                    bool ignore,  ETextType textType)
{
    //We have three cases regarding linenum
    //1. it is a colon-label, in which case msc_process_colon_string() inserted
    //   a position escape to the beginning of the string, so the linenum we got
    //   here will be overridden
    //2. it is an unquoted string, which cannot contain escapes, hence neither
    //   errors, so the linenum we got as parameter here will not be used
    //3. it is a quoted string. In this case we do not start by a pos escape
    //   and we may need the linenum parameter to report an error. In this case
    //   we know that the linenum parameter points to the opening quotation mark
    //   so we increment it at the beginning - for cases #1 and #2 it does not matter
    linenum.col++;
    string::size_type pos=0;
    string replaceto;
    StringFormat sf, sf_with_links;
    if (basic)
        sf = *basic;
    FileLineCol beginning_of_URL;
    beginning_of_URL.MakeInvalid();
    string ignoreText = ignore?" Ignoring it.":"";
    while(text.length()>pos && text[pos]) {
        size_t length;
        const FileLineCol beginning_of_escape = linenum;
        switch (sf.ProcessEscape(text.c_str()+pos, length, true, true, &replaceto, 
                                 basic, msc, references, &linenum, ignore)) {
        case FORMATTING_OK: //do nothing, just reflect 
        case REFERENCE:     //a valid \r(xxx) escape. If we "references" is true we have the number
            break;          //if not, we keep the escape as is.
        case LINK_ESCAPE:
            //resolve formattin
            if (beginning_of_URL.IsInvalid()) {
                if (replaceto.length()) {
                    //replace \\L to ESCAPE_STRING_NON_FORMATTING_LINK
                    beginning_of_URL = beginning_of_escape;
                    replaceto = "\\" ESCAPE_STRING_NON_FORMATTING_LINK "(" + replaceto + ")";
                    //insert the formatting needed for a link, plus a location escape
                    if (basic && basic->link_format.first) {
                        sf_with_links = sf;
                        sf_with_links.Apply(basic->link_format.second.c_str());
                        replaceto.append(basic->link_format.second+linenum.Print());
                    }
                    //sf will not contain the format added by link_format here, so
                    //we can use sf to restore formatting to at the end of the link.
                    //but sf_with_links will, so we can take their difference
                } else {
                    //problem: an empty first \\L
                    replaceto.clear();
                    msc->Error.Error(beginning_of_escape, "Empty link ('\\L') escape - you need to specify a link. Ignoring escape.",
                        "You can specify a link using an \\L() escape with the link target, followed by the link text, terminated by an empty '\\L()' escape.");
                }
            } else {
                if (replaceto.length()) {
                    //Error - two non-empty \\L escapes
                    msc->Error.Error(beginning_of_escape, "Non-empty link ('\\L') escape, use an empty one to terminate the link text. Assuming empty one.",
                        "You can specify a link using an \\L() escape with the link target, followed by the link text, terminated by an empty '\\L()' escape.");
                    msc->Error.Error(beginning_of_escape, beginning_of_URL, "Here is the first non-empty '\\L()' escape.");
                    //fallthrough
                } 
                //We are terminating an URL
                //restore formatting, if we have changed it prior
                if (basic && basic->link_format.first)
                    replaceto = "\\" ESCAPE_STRING_NON_FORMATTING_LINK "()" + 
                        (sf-sf_with_links).Print() + linenum.Print();
                else 
                    replaceto = "\\" ESCAPE_STRING_NON_FORMATTING_LINK "()" + linenum.Print();
                beginning_of_URL.MakeInvalid();
            }
            break;
        case LINK2_ESCAPE:
            //here replaceto contains just the parameter
            //Add the escape back, but otherwise keep the escape
            replaceto = "\\" ESCAPE_STRING_NON_FORMATTING_LINK "("+replaceto+")";
            break;
        case INVALID_ESCAPE:
            if (ignore) break;  //replaceto is empty here, we will remove the bad escape
            //fallthrough, if we do not ignore: this will set replaceto to the bad escape
        case LINE_BREAK:        //keep \n as is
        case SOFT_LINE_BREAK:
        case NON_FORMATTING:
            replaceto.assign(text.c_str()+pos, length);
            break; //do not (yet) resolve: keep what is in input
        case SOLO_ESCAPE:
            replaceto = "\\\\"; //replace to an escaped version: allows trouble-free string concatenation
            break;
        case NUMBERING_FORMAT:  //keep \x02{1aAiI} as is, if
            if (textType != NUMBER_FORMAT) {
                msc->Error.Error(beginning_of_escape, "Internal error: Number format escape in a label or text format.");
                replaceto.clear();
            }
            break;
        case NUMBERING:         //keep \N as is
            if (textType == NUMBER_FORMAT) {
                msc->Error.Error(beginning_of_escape, "The '\\N' escape can not be used for numbering formatting options. Ignoring it.");
                replaceto.clear();
            }
            break;
        case NON_ESCAPE:       //verbatim text. If numberformat search it
            if (textType != NUMBER_FORMAT) break;
            if (NumberingStyleFragment::FindReplaceNumberFormatToken(text, beginning_of_escape, pos, pos+length))
                //OK, a number descriptor found and has been replaced to
                //a numberformat escape. Re-parse verbatim text again
                continue;
        }
        text.replace(pos, length, replaceto);
        pos += replaceto.length();
        //if we inserted or removed characters, we need to insert another position escape
        //if (replaceto.length() != length) {
        //    const string pos_escape = linenum.Print();
        //    text.insert(pos, pos_escape);
        //    pos += pos_escape.length();
        //}
    }
}

/** Returns the offset of first numbering format escape, -1 if none*/
int StringFormat::FindNumberingFormatEscape(const char *text)
{
    StringFormat sf;
    string::size_type pos = 0;
	const string::size_type text_length = strlen(text);
    while (pos < text_length) {
        size_t length;
		if (NUMBERING_FORMAT == sf.ProcessEscape(text+pos, length))
            return (int)pos;
        pos += length;
    }
	return -1;
}

/** Removes any potential leftover position escape ("\0x1(xxx)")*/
void StringFormat::RemovePosEscapes(string &text)
{
    StringFormat sf;
    size_t pos = 0;
    while (pos < text.length() && text[pos]) {
        size_t length;
        if (FORMATTING_OK == sf.ProcessEscape(text.c_str()+pos, length))
            if (text[pos]=='\\' && text[pos+1]=='\x01') {
                text.erase(pos, length);
                continue;
            }
        pos += length;
    }
}

/** Removes all escapes, except line breaks, which are replaced with "\n"*/
void StringFormat::ConvertToPlainText(string &text)
{
    StringFormat sf;
    size_t pos = 0;
    while (pos < text.length() && text[pos]) {
        size_t length;
        switch (sf.ProcessEscape(text.c_str()+pos, length)) {
        case NON_FORMATTING:
        case NON_ESCAPE:
            pos += length;
            break;
        case LINE_BREAK:
        case SOFT_LINE_BREAK:
            text.replace(pos, length, "\n");
            pos++;
            break;
        default:
            text.erase(pos, length);  //all other escapes
            break;
        }
    }
}


size_t StringFormat::Apply(const char *text)
{
    size_t pos = 0;
    size_t length;
    const size_t len = strlen(text);
	StringFormat basic(*this);
    while (pos < len) {
        const EEscapeType t =
            ProcessEscape(text+pos, length, true, true, NULL, &basic);
        if (t == FORMATTING_OK || t == LINK_ESCAPE || t== LINK2_ESCAPE)
            pos += length;
        else
            break;
    }
    return pos;
}

void StringFormat::Apply(string &text)
{
    size_t start = 0;
    StringFormat basic(*this);
    while (start < text.length()) {
        EEscapeType t = NON_FORMATTING;
        size_t pos = 0;
        size_t length;
        while (start+pos<text.length()) {
            t = ProcessEscape(text.c_str()+start+pos, length, true, true, NULL, &basic);
            if (t!=FORMATTING_OK) break;
            pos += length;
        }
        text.erase(start, pos);
        if (t != LINK_ESCAPE && t != LINK2_ESCAPE)
            break;
        else
            start += length;
    }
}


void StringFormat::SetColor(ColorType c)
{
    color.first = true;
    color.second = c;
}

StringFormat &StringFormat::operator +=(const StringFormat& toadd)
{
    if (toadd.fontType.first)
        fontType = toadd.fontType;

    if (toadd.color.first) {
        if (color.first)
            color.second += toadd.color.second;
        else
            color = toadd.color;
    }

    if (toadd.bgcolor.first) {
        if (bgcolor.first)
            bgcolor.second += toadd.bgcolor.second;
        else
            bgcolor = toadd.bgcolor;
    }

    if (toadd.face.first)
        face = toadd.face;

    if (toadd.spacingBelow.first) {
        if (spacingBelow.first)
            spacingBelow.second =
                std::max(toadd.spacingBelow.second, spacingBelow.second);
        else
            spacingBelow = toadd.spacingBelow;
    }
    AddTristate(bold, toadd.bold);
    AddTristate(italics, toadd.italics);
    AddTristate(underline, toadd.underline);

    if (toadd.textHGapPre.first)
        textHGapPre = toadd.textHGapPre;

    if (toadd.textHGapPost.first)
        textHGapPost = toadd.textHGapPost;

    if (toadd.textVGapAbove.first)
        textVGapAbove = toadd.textVGapAbove;

    if (toadd.textVGapBelow.first)
        textVGapBelow = toadd.textVGapBelow;

    if (toadd.textVGapLineSpacing.first)
        textVGapLineSpacing = toadd.textVGapLineSpacing;

    if (toadd.ident.first)
        ident = toadd.ident;

    if (toadd.normalFontSize.first)
        normalFontSize = toadd.normalFontSize;

    if (toadd.smallFontSize.first)
        smallFontSize = toadd.smallFontSize;

    if (toadd.word_wrap.first)
        word_wrap = toadd.word_wrap;

    if (toadd.link_format.first)
        link_format = toadd.link_format;

    return *this;
}

StringFormat &StringFormat::operator -=(const StringFormat& base)
{
    _ASSERT(IsComplete());
    if (base.fontType == fontType) fontType.first = false;
    if (base.color == color) color.first = false;
    if (base.bgcolor == bgcolor) bgcolor.first = false;
    if (base.face == face) face.first = false;
    if (base.spacingBelow == spacingBelow) spacingBelow.first = false;
    if (base.bold == bold) bold.first = false;
    if (base.italics == italics) italics.first = false;
    if (base.underline == underline) underline.first = false;
    if (base.textHGapPre == textHGapPre) textHGapPre.first = false;
    if (base.textHGapPost == textHGapPost) textHGapPost.first = false;
    if (base.textVGapAbove == textVGapAbove) textVGapAbove.first = false;
    if (base.textVGapBelow == textVGapBelow) textVGapBelow.first = false;
    if (base.textVGapLineSpacing == textVGapLineSpacing) textVGapLineSpacing.first = false;
    if (base.ident == ident) ident.first = false;
    if (base.normalFontSize == normalFontSize) normalFontSize.first = false;
    if (base.smallFontSize == smallFontSize) smallFontSize.first = false;
    if (base.word_wrap == word_wrap) word_wrap.first = false;
    if (base.link_format == link_format) link_format.first = false;
    return *this;
}

string StringFormat::Print() const
{
    string ret;
    char fonttypes[] = "+-^_";
    if (fontType.first)
        ret << string("\\") + fonttypes[(unsigned)fontType.second];;

    if (color.first)
        ret << "\\c" + color.second.Print();
    if (bgcolor.first)
        ret << "\\C" + bgcolor.second.Print();

    if (face.first)
        ret << "\\f(" + face.second + ")";

    if (spacingBelow.first)
        ret << string("\\") << spacingBelow.second;

    if (bold.first) switch (bold.second) {
    case no:     ret << "\\B\\b"; break;
    case yes:    ret << "\\B"; break;
    case invert: ret << "\\b"; break;
    }
    if (italics.first) switch  (italics.second) {
    case no:     ret << "\\I\\i"; break;
    case yes:    ret << "\\I"; break;
    case invert: ret << "\\i"; break;
    }
    if (underline.first) switch (underline.second) {
    case no:     ret << "\\U\\u"; break;
    case yes:    ret << "\\U"; break;
    case invert: ret << "\\u"; break;
    }

    if (ident.first)
        switch (ident.second) {
        default:
        case MSC_IDENT_LEFT:   ret << "\\pl"; break;
        case MSC_IDENT_CENTER: ret << "\\pc"; break;
        case MSC_IDENT_RIGHT:  ret << "\\pr"; break;
        }

    if (textHGapPre.first)
        ret << "\\ml(" << textHGapPre.second << ")";

    if (textHGapPost.first)
        ret << "\\mr(" << textHGapPost.second << ")";

    if (textVGapAbove.first)
        ret << "\\mu(" << textVGapAbove.second << ")";

    if (textVGapBelow.first)
        ret << "\\md(" << textVGapBelow.second << ")";

    if (textVGapLineSpacing.first)
        ret << "\\mi(" << textVGapLineSpacing.second << ")";

    if (normalFontSize.first)
        ret << "\\mn(" << normalFontSize.second << ")";

    if (smallFontSize.first)
        ret << "\\ms(" << smallFontSize.second << ")";

    if (word_wrap.first)
        ret << "\\" ESCAPE_STRING_WORD_WRAP << (word_wrap.second ? "+" : "-");

    //link format is not written out

    return ret;
}

/** Take an attribute and apply it to us.
 *
 * We consider attributes ending with 'color', 'ident', 'format', 'font.face',
 * 'font.type', 'bold', 'italics', 'underline', 'gap.*', 'size.normal' and
 * 'size.small' or any style at the current context in `msc`. 
 * We also accept the clearing of
 * an attribute if `t` is STYLE_STYLE, that is for style definitions only.
 * At a problem, we generate an error into msc->Error.
 * @param [in] a The attribute to apply.
 * @param msc The chart we build.
 * @param [in] t The situation we set the attribute.
 * @returns True, if the attribute was recognized as ours (may have been a bad value though).*/
bool StringFormat::AddAttribute(const Attribute &a, Msc *msc, EStyleType t)
{
    if (a.type == MSC_ATTR_STYLE) {
        StyleSet::const_iterator i = msc->Contexts.back().styles.find(a.name);
        if (i == msc->Contexts.back().styles.end()) {
            a.InvalidStyleError(msc->Error);
            return true;
        }
        if (i->second.read().f_text) operator +=(i->second.read().text);
        return true;
    }
    if (a.EndsWith("color") || a.EndsWith("bgcolor")) {
        std::pair<bool, ColorType> &ref = a.EndsWith("bgcolor") ? bgcolor : color;
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t))
                ref.first = false;
            return true;
        }
        if (!a.CheckColor(msc->Contexts.back().colors, msc->Error)) return true;
        if (ref.first)
            ref.second += msc->Contexts.back().colors.GetColor(a.value);
        else {
            ref.first = true;
            ref.second = msc->Contexts.back().colors.GetColor(a.value);
        }
        return true;
    }
    if (a.EndsWith("ident")) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t))
                ident.first = false;
            return true;
        }
        if (a.type == MSC_ATTR_STRING && Convert(a.value, ident.second)) {
            ident.first = true;
            return true;
        }
        a.InvalidValueError(CandidatesFor(ident.second), msc->Error);
        return true;
    }
    if (a.EndsWith("format")) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t))
                Empty(); //remove all formatting
            return true;
        }
        string tmp = a.value;
        if (tmp.length()==0) return true;

        StringFormat::ExpandReferences(tmp, msc, a.linenum_value.start, this, false, true, TEXT_FORMAT);

        StringFormat sf(tmp);
        RemovePosEscapes(tmp);
        if (tmp.length()) {
            string s("Found some literal text: '");
            s.append(tmp).append("' in attribute '").append(a.name).append("'.");
            s.append(" Ignoring them.");
            msc->Error.Warning(a, true, s, "Attriute 'text.format' can only contain formatting escapes.");
            return true;
        }
        operator += (sf);
        return true;
    }
    if (a.EndsWith("font.face")) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t))
                face.first = false;
            return true;
        }
        if (a.CheckType(MSC_ATTR_STRING, msc->Error)) {
            string font_name = a.value;
            if (Canvas::HasFontFace(font_name)) {
                face.first = true;
                face.second = a.value;
            } else if (font_name.length()) {
                face.first = true;
                face.second = font_name;
                msc->Error.Warning(a, true, "Font '" + a.value + "' not found. "
                    "Using '"+font_name+"' instead.");
            } else {
                msc->Error.Error(a, true, "Font '" + a.value + "' not found "
                    "and no substitute available. Ignoring option.");
            }
        }
        return true;
    }
    if (a.EndsWith("font.type")) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t))
                fontType.first = false;
            return true;
        }
        if (a.type == MSC_ATTR_STRING && Convert(a.value, fontType.second)) {
            fontType.first = true;
            return true;
        }
        a.InvalidValueError(CandidatesFor(fontType.second), msc->Error);
        return true;
    }

    if (a.EndsWith("wrap")) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t))
                word_wrap.first = false;
            return true;
        }
        if (a.CheckType(MSC_ATTR_BOOL, msc->Error)) {
            word_wrap.first = true;
            word_wrap.second = a.yes;
            return true;
        }
        return true;
    }

    if (a.EndsWith("link_format")) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t))
                link_format.first = false;
            return true;
        }
        string tmp = a.value;
        if (tmp.length()==0) return true;

        //completely remove all ambigous escapes, such as empty '\\c()' - use the 
        //chart default as a fallback.
        StringFormat::ExpandReferences(tmp, msc, a.linenum_value.start, 
            &msc->Contexts.back().text, false, true, TEXT_FORMAT);
        string tmp_2(tmp);

        StringFormat sf(tmp);
        RemovePosEscapes(tmp);
        if (tmp.length()) {
            string s("Found some literal text: '");
            s.append(tmp).append("' in attribute '").append(a.name).append("'.");
            s.append(" Ignoring them.");
            msc->Error.Warning(a, true, s, "Attriute 'text.link_format' can only contain formatting escapes.");
            return true;
        }
        link_format.first = true;
        link_format.second = tmp_2;
        return true;
    }

    std::pair<bool, ETriState> *tri = NULL;
    if (a.EndsWith("bold")) tri = &bold;
    else if (a.EndsWith("italic")) tri = &italics;
    else if (a.EndsWith("underline")) tri = &underline;
    //handle bold, italics and underline
    if (tri) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t))
                tri->first = false;
            return true;
        }
        if (a.CheckType(MSC_ATTR_BOOL, msc->Error)) {
            tri->first = true;
            tri->second = a.yes ? yes : no;
        }
        return true;
    }

    std::pair<bool, double> *dou = NULL;
    if (a.EndsWith("gap.up")) dou = &textVGapAbove;
    else if (a.EndsWith("gap.down")) dou = &textVGapBelow;
    else if (a.EndsWith("gap.left")) dou = &textHGapPre;
    else if (a.EndsWith("gap.right")) dou = &textHGapPost;
    else if (a.EndsWith("gap.spacing")) dou = &textVGapLineSpacing;
    else if (a.EndsWith("size.normal")) dou = &normalFontSize;
    else if (a.EndsWith("size.small")) dou = &smallFontSize;

    if (dou) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t))
                dou->first = false;
            return true;
        }
        if (a.CheckType(MSC_ATTR_NUMBER, msc->Error)) {
            dou->first = true;
            dou->second = a.number;
        }
        return true;
    }
    return false;

}

/** Add the attribute names we take to `csh`.*/
void StringFormat::AttributeNames(Csh &csh, const string &prefix)
{
    static const char * const names_descriptions[] =
    {"", NULL,
    "color", "Set the color of the font",
    "bgcolor", "Set the color behind the text.",
    "ident", "Select left, right idented or centered text.",
    "format", "Use this attribute to set text format via formatting escapes, like '\\b'.",
    "font.face", "Select font face, such as 'Arial'.",
    "font.type", "Select between normal or small text, subscript or superscript.",
    "font.*", "Select font face and type.",
    "bold", "Select bold face.",
    "italic", "Select italics.",
    "underline", "Make the font underlined.",
    "gap.up", "Set the top margin of the label.",
    "gap.down", "Set the bottom margin of the label.",
    "gap.left", "Set the left margin of the label.",
    "gap.right", "Set the right margin of the label.",
    "gap.spacing", "Set the extra space between the lines of a multiline label.",
    "gap.*", "Set margins and spacing.",
    "size.normal", "Set the height of the normal text (i.e., not superscript or small font) in pixels.",
    "size.small", "Set the height of small text (including superscript and subscript) in pixels.",
    "wrap", "Turning this on will make the text word wrapped. This also means that horizontal auto-scaling is off, so caution with 'hscape=auto;'",
    "link_format", "Specify here what formatting shall be applied to a link (\\L escape).",
    ""};
    csh.AddToHints(names_descriptions, csh.HintPrefix(COLOR_ATTRNAME)+prefix, EHintType::ATTR_NAME);
}

/** Callback for drawing a symbol before text ident types in the hints popup list box.
* @ingroup libmscgen_hintpopup_callbacks*/
bool CshHintGraphicCallbackForTextIdent(Canvas *canvas, CshHintGraphicParam p, Csh&)
{
    if (!canvas) return false;
    const EIdentType t = (EIdentType)(int)p;
    const static double sizePercentage[] = {50, 30, 60};
    const LineAttr line(LINE_SOLID, ColorType(0, 0, 0), 1, CORNER_NONE, 0);
    double y = floor(HINT_GRAPHIC_SIZE_Y*0.2)+0.5;
    double y_inc = ceil(HINT_GRAPHIC_SIZE_Y*0.3/(sizeof(sizePercentage)/sizeof(double)-1));
    for (size_t i = 0; i<sizeof(sizePercentage)/sizeof(double); i++) {
        double x1 = floor(HINT_GRAPHIC_SIZE_X*sizePercentage[i]/100.+0.5);
        double x2 = floor(HINT_GRAPHIC_SIZE_X*0.2);
        switch (t) {
        default:
        case MSC_IDENT_LEFT: x1 = x2+x1; break;
        case MSC_IDENT_CENTER: x2 = floor(HINT_GRAPHIC_SIZE_X*0.5)-x1/2; x1 = x1+x2; break;
        case MSC_IDENT_RIGHT: x2 = floor(HINT_GRAPHIC_SIZE_X*0.8); x1 = x2-x1; break;
        }
        canvas->Line(XY(x1, y+y_inc), XY(x2, y+y_inc), line);
        y += y_inc;
    }
    return true;
}


/** Draws a symbol for text formatting escapes in the hints popup list box.
* Helper to CshHintGraphicCallbackForEscapes() .*/
bool DrawTextForHint(Canvas *canvas, const std::string &text1, const std::string &text2)
{
    StringFormat sf;
    sf.Default();
    string ss("\\pl\\mn(");
    ss << (int)(HINT_GRAPHIC_SIZE_Y*0.8) << ")";
    sf.Apply(ss.c_str());
    canvas->Clip(0, HINT_GRAPHIC_SIZE_X, 0, HINT_GRAPHIC_SIZE_Y);
    Label label1(text1, *canvas, sf);
    Label label2(text1+text2, *canvas, sf);
    const double w1 = label1.getTextWidthHeight().x;
    const double h2 = label2.getTextWidthHeight().y;
    label2.Draw(*canvas, HINT_GRAPHIC_SIZE_X*0.4-w1, HINT_GRAPHIC_SIZE_X, 
        HINT_GRAPHIC_SIZE_Y/2-h2/2); //center vertically. Start text2 at 40% of X range
    canvas->UnClip();
    return true;
}

/** Draws a symbol before some of the text formatting escapes in the hints popup list box.
* Helper to CshHintGraphicCallbackForEscapes() .*/
bool DrawFormattingEscape(Canvas *canvas, const char *format)
{
    return DrawTextForHint(canvas, "ABD", string(format)+"FGH");
}

/** Draws a symbol before literal escapes in the hints popup list box.
* Helper to CshHintGraphicCallbackForEscapes() .*/
bool DrawLiteral(Canvas *canvas, const char *literal)
{
    return DrawTextForHint(canvas, "", literal);
}

/** Callback for drawing a symbol before text formatting escapes in the hints popup list box.
* @ingroup libmscgen_hintpopup_callbacks*/
bool CshHintGraphicCallbackForEscapes(Canvas *canvas, CshHintGraphicParam p, Csh&csh)
{
    if (!canvas) return false;

    LineAttr line; //black, single line
    FillAttr fill(ColorType(0,0,0), GRADIENT_NONE); //black

    const double X = HINT_GRAPHIC_SIZE_X/10.;
    const double Y = HINT_GRAPHIC_SIZE_Y/10.;

    switch (unsigned(p)/2) {
    case 01: // \n
        canvas->Line(XY(9*X,   0), XY(9*X, 7*Y), line);
        canvas->Line(XY(9*X, 7*Y), XY(4*X, 7*Y), line);
        canvas->Line(XY(2*X, 7*Y), XY(6*X, 4*Y), line);
        canvas->Line(XY(2*X, 7*Y), XY(6*X, 10*Y), line);
        return true;
    case 02: // \b
    case 34: // \B
        return DrawFormattingEscape(canvas, "\\b");
    case 03: // \i
    case 35: // \I
        return DrawFormattingEscape(canvas, "\\i");
    case 04: // \u
    case 36: // \U
        return DrawFormattingEscape(canvas, "\\u");
    case 05: // \+
        return DrawFormattingEscape(canvas, "\\+");
    case 06: // \-
        return DrawFormattingEscape(canvas, "\\-");
    case 07: // \_
        return DrawFormattingEscape(canvas, "\\_");
    case  8: // \^
        return DrawFormattingEscape(canvas, "\\^");
    case 12: // \p*
    case  9: // \pc
        return CshHintGraphicCallbackForTextIdent(canvas, (CshHintGraphicParam)(int)MSC_IDENT_CENTER, csh);
    case 10: // \pl
        return CshHintGraphicCallbackForTextIdent(canvas, (CshHintGraphicParam)(int)MSC_IDENT_LEFT, csh);
    case 11: // \pr
        return CshHintGraphicCallbackForTextIdent(canvas, (CshHintGraphicParam)(int)MSC_IDENT_RIGHT, csh);
    case 13: // \c()  
        return DrawFormattingEscape(canvas, "\\c(1,0,0)");
    case 14: // \C()  
        return DrawFormattingEscape(canvas, "\\c(1,0,0)");
    case 15: // \s()
        return CshHintGraphicCallbackForStyles(canvas, 0, csh);
    case 16: // '\\'
        return DrawLiteral(canvas, "\\");
    case 17: // \#
        return DrawLiteral(canvas, "#");
    case 18: // \{
        return DrawLiteral(canvas, "{");
    case 19: // \}
        return DrawLiteral(canvas, "}");
    case 20: // \[
        return DrawLiteral(canvas, "[");
    case 21: // \]
        return DrawLiteral(canvas, "]");
    case 22: // \;
        return DrawLiteral(canvas, ";");
    case 23: // \\"
        return DrawLiteral(canvas, "\"");
    case 48: // \L()
    case 24: // \r()
        return DrawLiteral(canvas, "Ref");
    case 25: // \f()
        return DrawFormattingEscape(canvas, "\\f(Courier New)");
    case 32: // \m*()
    case 26: // \mu()
    case 27: // \md()
    case 28: // \ml()
    case 29: // \mr()
    case 30: // \mn()
    case 31: // \ms()
        return true;
    case 33: // \N
        return DrawTextForHint(canvas, "(", "1)");
    case 37: // \0
    case 38: // \1
    case 39: // \2
    case 40: // \3
    case 41: // \4
    case 42: // \5
    case 43: // \6
    case 44: // \7
    case 45: // \8
    case 46: // \9     
        canvas->Line(XY(5*X, 3*Y), XY(5*X, 7*Y), line);
        canvas->Line(XY(5*X, 3*Y), XY(4*X, 4*Y), line);
        canvas->Line(XY(5*X, 3*Y), XY(6*X, 4*Y), line);
        canvas->Line(XY(5*X, 7*Y), XY(4*X, 6*Y), line);
        canvas->Line(XY(5*X, 7*Y), XY(6*X, 6*Y), line);
        //line.width.second = 1.5;
        canvas->Line(XY(2*X, 3*Y), XY(8*X, 3*Y), line);
        canvas->Line(XY(2*X, 7*Y), XY(8*X, 7*Y), line);
        return true;
    case 47: // \|
        return DrawTextForHint(canvas, "a", "|b");
    };
    return false;
}

void StringFormat::EscapeHints(Csh &csh, const string &prefix)
{
    //We escape the initial escape so that a decorated->plain conversion keeps a literal '\' at the font
    static const char * const names_descriptions[] ={ "", NULL,
/*01*/  "\\\\n", "Insert a manual line break. This line break will be honoured even with word wrapping turned on.",
/*02*/  "\\\\b", "Toggle bold font.",
/*03*/  "\\\\i", "Toggle italics font.",
/*04*/  "\\\\u", "Toggle font underline.",
/*05*/  "\\\\+", "Change to normal font (as opposed to superscript, subscript or small font.",
/*06*/  "\\\\-", "Change to small font. (Revert to normal font via '\\+'.)",
/*07*/  "\\\\_", "Change to subscript. (Revert to normal font via '\\+'.)",
/*08*/  "\\\\^", "Change to superscript. (Revert to normal font via '\\+'.)",
/*09*/  "\\\\pc", "Center the text.",
/*10*/  "\\\\pl", "Ident the text left.",
/*11*/  "\\\\pr", "Ident the text right.",
/*12*/  "\\\\p*", "Set paragraph ident.",
/*13*/  "\\\\c()", "Set font color. Omitting the color name will restore the default color of the label (in effect at its beginning).",
/*14*/  "\\\\c()", "Set font color. Omitting the color name will restore the default color of the label (in effect at its beginning).",
/*15*/  "\\\\s()", "Apply a style. Omitting the style name will restore the default formatting of the label (in effect at its beginning).",
/*16*/  "\\\\\\", "Insert a literal '\\'.",
/*17*/  "\\\\#", "Insert a literal '#'.",
/*18*/  "\\\\{", "Insert a literal '{'.",
/*19*/  "\\\\}", "Insert a literal '}'.",
/*20*/  "\\\\[", "Insert a literal '['.",
/*21*/  "\\\\]", "Insert a literal ']'.",
/*22*/  "\\\\;", "Insert a literal semicolon (';').",
/*23*/  "\\\\\"", "Insert a literal quotation mark ('\"').",
/*24*/  "\\\\r()", "Paste the number of another chart element name by its 'refname' attribute.",
/*25*/  "\\\\f()", "Set the font family of the text. Omitting the font name will restore the default font of the label (in effect at its beginning).",
/*26*/  "\\\\mu()", "Set the top margin of the label in pixels.",
/*27*/  "\\\\md()", "Set the bottom margin of the label in pixels.",
/*28*/  "\\\\ml()", "Set the left margin of the label in pixels.",
/*29*/  "\\\\mr()", "Set the right margin of the label in pixels.",
/*30*/  "\\\\mn()", "Set the font height of normal font in pixels.",
/*31*/  "\\\\ms()", "Set the font height of small font, superscript and subscript in pixels.",
/*32*/  "\\\\m*()", "Set margins and font height.",
/*33*/  "\\\\N", "Use this escape to specify the location of the automatic numbering within a label. Useful if you want it somewhere else than the front of the label.",
/*34*/  "\\\\B", "Make the font bold (no change if already bold).",
/*35*/  "\\\\I", "Make the font italic (no change if already italic).",
/*36*/  "\\\\U", "Make the font underlined (no change if already so).",
/*37*/  "\\\\0", "Remove line spacing from below this line.",
/*38*/  "\\\\1", "Create one pixel of line spacing below this line.",
/*39*/  "\\\\2", "Create two pixels of line spacing below this line.",
/*40*/  "\\\\3", "Create three pixels of line spacing below this line.",
/*41*/  "\\\\4", "Create four pixels of line spacing below this line.",
/*42*/  "\\\\5", "Create five pixels of line spacing below this line.",
/*43*/  "\\\\6", "Create six pixels of line spacing below this line.",
/*44*/  "\\\\7", "Create seven pixels of line spacing below this line.",
/*45*/  "\\\\8", "Create eight pixels of line spacing below this line.",
/*46*/  "\\\\9", "Create nine pixels of line spacing below this line.",
/*47*/  "\\\\|", "Use this around the beginning of a label to separate initial formatting escapes into two groups. "
               "The escapes before this one will determine the default format of the label, used by empty '\\s()', '\\c()', etc. escapes to restore default style, color and so on.",
/*48*/  "\\\\L()", "Use this escape to mark the beginning and end of a link. At the beginning, specify the link target (e.g., URL) in between the parentheses.",
    ""};
    csh.AddToHints(names_descriptions, csh.HintPrefix(COLOR_LABEL_ESCAPE)+prefix, EHintType::ESCAPE,
        CshHintGraphicCallbackForEscapes, true);
}

/** Add a list of possible attribute value names to `csh` for attribute `attr`.*/
bool StringFormat::AttributeValues(const std::string &attr, Csh &csh)
{
    if (CaseInsensitiveEndsWith(attr, "color") ||
        CaseInsensitiveEndsWith(attr, "bgcolor")) {
        csh.AddColorValuesToHints(false);
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "ident")) {
        csh.AddToHints(EnumEncapsulator<EIdentType>::names, NULL, csh.HintPrefix(COLOR_ATTRVALUE), EHintType::ATTR_VALUE,
            CshHintGraphicCallbackForTextIdent);
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "format")||
        CaseInsensitiveEndsWith(attr, "link_format")) {
        if (!csh.hadEscapeHint)
            csh.AddToHints(CshHint(csh.HintPrefixNonSelectable()+"\"format string\"",
                "Specify a format string using text escapes, like '\\b'. Do not use plain text (non-formatting characters or escapes).",
                EHintType::ATTR_VALUE, false));
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "font.type")) {
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "font.face")) {
        if (csh.fontnames) {
            for (const auto &str : *csh.fontnames)
                if (str.length() && str[0]!='@') {
                    //Add font names containing space using quotation marks
                    if (str.find_first_of(' ')==string::npos)
                        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+str, NULL,
                            EHintType::ATTR_VALUE));
                    else
                        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+"\""+str+"\"", NULL,
                        EHintType::ATTR_VALUE));
                }
        }
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "bold") || 
        CaseInsensitiveEndsWith(attr, "italic") ||
        CaseInsensitiveEndsWith(attr, "wrap") ||
        CaseInsensitiveEndsWith(attr, "underline")) {
            csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE) + "yes", NULL, EHintType::ATTR_VALUE, true, CshHintGraphicCallbackForYesNo, CshHintGraphicParam(1)));
            csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE) + "no", NULL, EHintType::ATTR_VALUE, true, CshHintGraphicCallbackForYesNo, CshHintGraphicParam(0)));
            return true;
    }
    if (CaseInsensitiveEndsWith(attr, "gap.up")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable()+"<number in pixels>",
            "Set the top margin of the label in pixels.",
            EHintType::ATTR_VALUE, false));
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "gap.down")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable()+"<number in pixels>",
            "Set the bottom margin of the label in pixels.",
            EHintType::ATTR_VALUE, false));
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "gap.left")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable()+"<number in pixels>",
            "Set the left margin of the label in pixels.",
            EHintType::ATTR_VALUE, false));
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "gap.right")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable()+"<number in pixels>",
            "Set the right margin of the label in pixels.",
            EHintType::ATTR_VALUE, false));
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "gap.spacing")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable()+"<number in pixels>",
            "Set the line spacing (the gap between lines) of the label in pixels.",
            EHintType::ATTR_VALUE, false));
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "size.normal")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable()+"<number in pixels>",
            "Set the height of the normal text in pixels.",
            EHintType::ATTR_VALUE, false));
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "size.small")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable()+"<number in pixels>",
            "Set the height of small text (inclusing subscript and superscript) in pixels.",
            EHintType::ATTR_VALUE, false));
        return true;
    }
    return false;
}


/** Adds numbering to a label.
 * By default to the beginning of it, unless "\N" escape directs it elsewhere.
 * If we find a "\N" escape in the label we replace that to `num` (for multiple "\N"s, if needed).
 * If we find no such escape, we add `pre_num_post` to the beginning, 
 * but after the initial formatting strings.
 * @param label The label to add numbers to.
 * @param [in] num The number to replace "\N" with.
 * @param [in] pre_num_post The number to add to the beginning.*/
//This shall be called only after StringFormat::ExpandReferences on a label and both num strings
void StringFormat::AddNumbering(string &label, const string &num, const string &pre_num_post)
{
    if (label.length()==0) return;
	StringFormat sf;
	size_t pos= 0;
	size_t length;
    //skip over initial formatting & links
	while (pos < label.length() && label[pos]) {
        const EEscapeType t = sf.ProcessEscape(label.c_str()+pos, length);
        if (FORMATTING_OK != t && LINK2_ESCAPE != t && LINK_ESCAPE != t) break;
        pos += length;
    }
    bool number_added = false;
    size_t beginning_pos = pos;
    while (pos < label.length() && label[pos]) {
        if (NUMBERING == sf.ProcessEscape(label.c_str()+pos, length)) {
            label.replace(pos, length, num);
            length = num.length();
            number_added = true;
        }
        pos += length;
    }
    //Add number at the beginning, if not yet added somewhere
    if (!number_added)
        label.insert(beginning_pos, pre_num_post);
}

/** Apply the formatting in us to `canvas`.
 * After this putting text to the canvas will use 
 * our formatting*/
void StringFormat::ApplyFontTo(Canvas &canvas) const
{
    _ASSERT(IsComplete()); 
    canvas.SetFontFace(face.second.c_str(), italics.first && italics.second,
                        bold.first && bold.second);

    canvas.SetFontSize(smallFontSize.second);
    cairo_font_extents(canvas.GetContext(), &smallFontExtents);
    canvas.SetFontSize(normalFontSize.second);
    cairo_font_extents(canvas.GetContext(), &normalFontExtents);

    if (fontType.second == MSC_FONT_NORMAL) /*Normal font*/
        canvas.SetFontSize(normalFontSize.second) ;
    else /* Small, subscript, superscript */
        canvas.SetFontSize(smallFontSize.second);
    canvas.SetColor(color.second);
}

/** Returns the width of spaces at the beginning or at the end of a string.
 * This is because cairo removes these spaces and sometimes we need
 * them.
 * @param [in] text The text in which we look for heading/trailing spaces.
 * @param canvas A canvas used to query font sizes
 * @param [in] front If true we return the width of heading spaces, else trailing.
 * @returns The width of the spaces in pixels. */
double StringFormat::spaceWidth(const string &text, Canvas &canvas, bool front) const
{
    if (text.length()==0 || !canvas.fake_spaces) return 0;
    size_t s;
    if (front) {
        s = text.find_first_not_of(' ');
        if (s==text.npos) s=text.length();
    } else {
        s = text.find_last_not_of(' ');
        if (s==text.npos) return 0; //if it is spaces only, we only report front
        s = text.length() - s - 1;
    }
    if (s==0) return 0;
    ApplyFontTo(canvas);
    cairo_text_extents_t te;
	cairo_text_extents (canvas.GetContext(), "M N", &te); //this counts one more space then needed
    double  w = te.x_advance;
    cairo_text_extents (canvas.GetContext(), "MN", &te);
    w -= te.x_advance;
	return w*s;
}

double StringFormat::getCharHeight(Canvas &canvas) const
{
    ApplyFontTo(canvas);
    return normalFontExtents.ascent + normalFontExtents.descent;
}

double StringFormat::getFragmentWidth(const string &s, Canvas &canvas) const
{
    if (s.length()==0) return 0;
    ApplyFontTo(canvas);
    cairo_text_extents_t te;
    if (canvas.individual_chars) {
        double advance = 0;
        char tmp_stirng[2] = "a";
        for (size_t i = 0; i<s.length(); i++) {
            tmp_stirng[0] = s[i];
            cairo_text_extents(canvas.GetContext(), tmp_stirng, &te);
            advance += te.x_advance;
        }
        te.x_advance = advance;
    } else {
        cairo_text_extents (canvas.GetContext(), s.c_str(), &te);
    }
    return spaceWidth(s, canvas, true) + te.x_advance + spaceWidth(s, canvas, false);
}

double StringFormat::getFragmentHeightAboveBaseLine(const string &s,
                                                      Canvas &canvas) const
{
    if (s.length()==0) return 0;
    ApplyFontTo(canvas);
    switch(fontType.second) {
    case MSC_FONT_NORMAL:  //normal font
        return normalFontExtents.ascent;
    case MSC_FONT_SMALL:  //Small font
        return smallFontExtents.ascent;
    case MSC_FONT_SUPERSCRIPT: //superscript - typeset 20% higher than normal font
        return normalFontExtents.ascent*1.2;
    case MSC_FONT_SUBSCRIPT: //subscript - typeset 20% lower than normal font
        return std::max(0., smallFontExtents.ascent-normalFontExtents.ascent*0.2);
    }
    return 0;
}

double StringFormat::getFragmentHeightBelowBaseLine(const string &s,
                                                      Canvas &canvas) const
{
    if (s.length()==0) return 0;
    ApplyFontTo(canvas);
    switch(fontType.second) {
    case MSC_FONT_NORMAL:  //normal font
        return normalFontExtents.descent;
    case MSC_FONT_SMALL:  //Small font
        return smallFontExtents.descent;
    case MSC_FONT_SUPERSCRIPT: //superscript - typeset 20% higher than normal font
        return std::max(0., smallFontExtents.descent - normalFontExtents.ascent*0.2);
    case MSC_FONT_SUBSCRIPT: //subscript - typeset 20% lower than normal font
        return smallFontExtents.descent + normalFontExtents.ascent*0.2;
    }
    return 0;
}

/** Draw a piece of text with our (uniform) formatting. 
 * All drawing is strictly in english. Left to right and ASCII only.
 * @param [in] s The text to output.
 * @param canvas The canvas to draw to. 
 * @param [in] xy The starting point. `y` specifies the baseline of the text, not the
 *                upper left corner. (Baseline is not meant in cairo sense.)
 * @param [in] Y The y extent to draw for background.
 * @param [in] isRotated If true, and the canvas cannot display rotated text,
 *                       the canvas will use cairo_text_path() as fallback.
 * @returns The y advancement.*/
double StringFormat::drawFragment(const string &s, Canvas &canvas, 
                                  XY xy, Range Y, bool isRotated) const
{
    if (s.length()==0) return 0;
    ApplyFontTo(canvas);
    cairo_text_extents_t te;
    cairo_text_extents(canvas.GetContext(), s.c_str(), &te);
    const double advance = spaceWidth(s, canvas, true) + te.x_advance + spaceWidth(s, canvas, false);

    xy.x += spaceWidth(s, canvas, true);

    if (bgcolor.first && !bgcolor.second.IsFullyTransparent()) {
        canvas.Fill(Block(Range(xy.x, xy.x+advance), Y),
            FillAttr(bgcolor.second, GRADIENT_NONE));
        //reset color to that of the font
        canvas.SetColor(color.second);
    }

    switch (fontType.second) {
    case MSC_FONT_NORMAL:  //normal font
    case MSC_FONT_SMALL:  //Small font
        break;
    case MSC_FONT_SUPERSCRIPT: //superscript - typeset 20% higher than normal font
        xy.y -= normalFontExtents.ascent*0.2;
        break;
    case MSC_FONT_SUBSCRIPT: //subscript - typeset 20% lower than normal font
        xy.y += normalFontExtents.ascent*0.2;
        break;
    }
    
	canvas.Text(xy, s, isRotated);

    if (underline.first && underline.second) {
        xy.y++;
        XY xy2(xy.x+advance, xy.y);
        canvas.Line(xy, xy2, LineAttr(LINE_SOLID, color.second, 1, CORNER_NONE, 0));
    }
    return advance;
}

//////////////////////////////////////////////////////////////

/** Creates one element of an NCSA formatted ISMAP file */
string ISMapElement::Print() const
{
    string ret("rect ");
    ret << target << " ";
    ret << round(rect.x.from) << "," << round(rect.y.from) << " ";
    ret << round(rect.x.till) << "," << round(rect.y.till);
    return ret;
}


ParsedLine::ParsedLine(const string &in, Canvas &canvas, StringFormat &format, bool h) :
    line(in), width(0), heightAboveBaseLine(0), heightBelowBaseLine(0), hard_new_line(h)
{
    //Remove heading and trailing whitespace.
    while (line.size() && line.front() == ' ') line.erase(0,1);
    while (line.size() && line.back() == ' ') line.erase(line.size()-1,1);
    format.Apply(line); //eats away initial formatting, so that we do not process them every time
    //but we keep link escapes among the initial formatting escapes.
    //'line' may start with escapes, like a link escape or non-formatting ones.
    startFormat = format;
    size_t pos = 0;
    size_t length;
    string replaceto;
    string fragment;
    while (line.length()>pos) {
        //collect characters up until we hit a vaild formatting escape (or string end)
        while (line.length()>pos && line[pos]) {
            const StringFormat::EEscapeType res = 
                format.ProcessEscape(line.c_str()+pos, length, true, false, &replaceto, &startFormat);
            if (res==StringFormat::FORMATTING_OK) break;
            pos += length;
            _ASSERT(StringFormat::LINK_ESCAPE != res);
            if (StringFormat::LINK_ESCAPE == res || StringFormat::LINK2_ESCAPE == res) continue;
            fragment.append(replaceto);
        }
        //fragment can be empty due to labels beginning with link escapes & repeated formatting escapes
        if (fragment.length()) {
            //store fragment data
            width += format.getFragmentWidth(fragment, canvas);
            heightAboveBaseLine =
                std::max(heightAboveBaseLine, format.getFragmentHeightAboveBaseLine(fragment, canvas));
            heightBelowBaseLine =
                std::max(heightBelowBaseLine, format.getFragmentHeightBelowBaseLine(fragment, canvas));
            fragment.clear();
        }
        if (line.length()>pos && line[pos]) {
            format.ProcessEscape(line.c_str()+pos, length, true, true, NULL, &startFormat);
            pos += length;
        }
    }
    //If an empty line, add standard height
    if (line.length()==0) {
        fragment = "M"; //the functions below ignore the text, just return pre-stored values for non-empty stings
        heightAboveBaseLine =
            std::max(heightAboveBaseLine, format.getFragmentHeightAboveBaseLine(fragment, canvas));
        heightBelowBaseLine =
            std::max(heightBelowBaseLine, format.getFragmentHeightBelowBaseLine(fragment, canvas));
    }
    //Add spacing below. If there is spacingbelow, span a full height line
    if (heightAboveBaseLine == 0 && format.getSpacingBelow())
        heightAboveBaseLine = format.getFragmentHeightAboveBaseLine("M", canvas);
};

ParsedLine::operator std::string() const
{
    StringFormat format(startFormat);
    size_t pos = 0;
    size_t length;
    string replaceto;
    string ret;

    while (line.length()>pos && line[pos]) {
        const StringFormat::EEscapeType res =
            format.ProcessEscape(line.c_str()+pos, length, true, false, &replaceto, &startFormat);
        //move pos to after the piece analyzed
        pos += length;
        _ASSERT(StringFormat::LINK_ESCAPE != res);
        if (StringFormat::FORMATTING_OK == res ||
            StringFormat::LINK_ESCAPE == res || StringFormat::LINK2_ESCAPE == res) continue;
            //do not copy link escapes - but all else
        ret.append(replaceto);
    }
    return ret;
}

void ParsedLine::CollectIsMapElements(XY xy, Canvas &canvas, ISMap &ismap,
                                      string &target_at_front) const
{
    StringFormat format(startFormat);
    size_t pos = 0;
    size_t length;
    string replaceto;
    string fragment;

    ISMapElement elem;
    if (target_at_front.length()) {
        elem.target = target_at_front;
        elem.rect.x.from = xy.x;
        elem.rect.y.from = xy.y;
        target_at_front.clear();
    }

    xy.y += heightAboveBaseLine;

    while (line.length()>pos && line[pos]) {
        StringFormat::EEscapeType res = StringFormat::NON_ESCAPE;
        //collect characters up until we hit a vaild formatting escape (or string end)
        while (line.length()>pos) {
            res = format.ProcessEscape(line.c_str()+pos, length, true, true, &replaceto, &startFormat);
            //move pos to after the piece analyzed
            pos += length;
            _ASSERT(StringFormat::LINK_ESCAPE != res);
            if (StringFormat::FORMATTING_OK == res ||
                StringFormat::LINK_ESCAPE == res || StringFormat::LINK2_ESCAPE == res)
                break; // in this case 'format' already contains the effect of this escape
            //just append whatever we got - verbatim text 
            fragment.append(replaceto);
        }
        if (fragment.length()) {
            xy.x += format.getFragmentWidth(fragment, canvas);
            fragment.clear();
        }
        if (StringFormat::LINK_ESCAPE == res || StringFormat::LINK2_ESCAPE == res) {
            if (elem.target.length()) {
                //If a second \L is non-empty -> we ignore parameter (so here we ignore 'replaceto')
                elem.rect.x.till = xy.x;
                elem.rect.y.till = xy.y + heightBelowBaseLine;
                ismap.push_back(std::move(elem));
                elem.target.clear();
            } else {
                if (replaceto.length()) {
                    elem.target = replaceto;
                    elem.rect.x.from = xy.x;
                    elem.rect.y.from = xy.y - heightAboveBaseLine;
                }
                //just do nothing if we encounter a first empty '\L'. Such escape is to ignore.
            }
        }
    }
    //flush any remaining opened links.
    if (elem.target.length()) {
        target_at_front = elem.target;
        elem.rect.x.till = xy.x;
        elem.rect.y.till = xy.y + heightBelowBaseLine;
        ismap.push_back(std::move(elem));
    }
}

void ParsedLine::Draw(XY xy, Canvas &canvas, bool isRotated) const
{
    StringFormat format(startFormat);
    size_t pos = 0;
    size_t length;
    string replaceto;
    string fragment;
    const Range Y(xy.y, xy.y+heightAboveBaseLine+heightBelowBaseLine);

    xy.y += heightAboveBaseLine;

    while (line.length()>pos && line[pos]) {
        //collect characters up until we hit a vaild formatting escape (or string end)
        while (line.length()>pos) {
            const StringFormat::EEscapeType res = 
                format.ProcessEscape(line.c_str()+pos, length, true, false, &replaceto, &startFormat);
            if (StringFormat::FORMATTING_OK == res) break;
            pos += length;
            _ASSERT(StringFormat::LINK_ESCAPE != res);
            if (StringFormat::LINK_ESCAPE == res || StringFormat::LINK2_ESCAPE == res) continue;
            //do not draw link escapes
            fragment.append(replaceto);
        }
        if (fragment.length()) {
            //draw Fragment
            xy.x += format.drawFragment(fragment, canvas, xy, Y, isRotated);
            fragment.clear();
        }
        //apply the formatting
        if (line.length()>pos) {
            format.ProcessEscape(line.c_str()+pos, length, true, true, NULL, &startFormat);
            pos += length;
        }
    }
}

//////////////////////////////////////////////////


size_t Label::Set(const string &input, Canvas &canvas, StringFormat format)
{
    clear();
    size_t pos = 0, line_start = 0;
    size_t length = 0;
    while (pos < input.length() && input[pos]) {
        bool hard_line_break = false; //=false is just there to supress a compilation warning
        //find next new line
        while (pos < input.length() && input[pos]) {
            const auto ret = format.ProcessEscape(input.c_str()+pos, length);
            hard_line_break = ret == StringFormat::LINE_BREAK;
            if (ret == StringFormat::LINE_BREAK ||
                ret == StringFormat::SOFT_LINE_BREAK) 
                break; //format not changed here!!!
            pos += length;
        }
        //Add a new line
        push_back(ParsedLine(input.substr(line_start, pos-line_start), canvas, format, hard_line_break)); //format changed here
        pos += length; //add the length of the line break itself
        line_start = pos;
    }
    return this->size();
}

Label::operator std::string() const
{
    string ret;
    if (size()>0)
        for (size_t i = 0; i<size(); i++)
            ret += at(i);
    return ret;
}

void Label::ApplyStyle(const StringFormat &sf)
{
    for (auto &line : *this)
        line.startFormat += sf;
}

void Label::EnsureMargins(double left, double right)
{
    if (size()==0) return;
    double lmin = DBL_MAX, rmin = DBL_MAX;
    for (const auto &l : *this) {
        if (lmin > l.startFormat.textHGapPre.second)
            lmin = l.startFormat.textHGapPre.second;
        if (rmin > l.startFormat.textHGapPost.second)
            rmin = l.startFormat.textHGapPost.second;
    }
    lmin = std::max(0., left-lmin);  //we need to add this much so that all margins are >= 'left'
    rmin = std::max(0., right-lmin); //we need to add this much so that all margins are >= 'right'
    if (lmin || rmin)
        for (auto &l : *this) {
            l.startFormat.textHGapPre.second += lmin;
            l.startFormat.textHGapPost.second += rmin;
        }
}


/* Get the width of a (potentially multi-line) text or one of its lines
*  If text is empty return 0
* we add spacings around,
* for an individual line, we add the spacings above, but not the ones below,
*/
XY Label::getTextWidthHeight(int line) const
{
    XY xy(0, 0);
    if (size()==0) return xy;
    if (line>=0) {
        if (size()<=unsigned(line)) return xy;
        xy = at(line).getWidthHeight();
        if (line == 0)  //First line
            xy.y += at(0).startFormat.textVGapAbove.second;
        else
            xy.y += at(line-1).startFormat.textVGapLineSpacing.second +
            at(line-1).startFormat.spacingBelow.second;
    } else {
        for (size_t i = 0; i<size(); i++) {
            double x = at(i).getWidthHeight().x +
                at(i).startFormat.textHGapPre.second +
                at(i).startFormat.textHGapPost.second;
            if (xy.x < x)
                xy.x = x;
            xy.y += at(i).getWidthHeight().y;
            if (i!=size()-1)
                xy.y += at(i).startFormat.textVGapLineSpacing.second +
                at(i).startFormat.spacingBelow.second;
        }
        xy.y += at(0).startFormat.textVGapAbove.second +
            at(size()-1).startFormat.textVGapBelow.second;
    }
    if (xy.x == 0 || xy.y == 0)
        xy.x = xy.y = 0;
    return xy;
};

#define LETTERS_NUMBERS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_"

double Label::Reflow(Canvas &c, double x)
{
    if (size()==0) return false;
    double overflow = 0;
    std::vector<ParsedLine> reflown; //the resulted set of lines
    size_t lnum = 0; //the number of the line we process
    size_t pos = 0;  //the position within at(lnum)
    string line;     //the line we gather in the new flow
    string keep_with; //elements that shall stick to the next line just before a break
    double width = 0;//width of the text in "line"
    StringFormat start_format = at(0).startFormat;  //format at the beginning of "line"
    StringFormat running_format = start_format;
    double available = x - start_format.textHGapPre.second 
                         - start_format.textHGapPost.second;
    do {
        while (pos < at(lnum).line.length() && at(lnum).line[pos]) {
            string replaceto;
            size_t length;
            switch (running_format.ProcessEscape(at(lnum).line.c_str()+pos, length, 
                                                 true, true, &replaceto, 
                                                 &at(0).startFormat)) {
            case StringFormat::FORMATTING_OK: 
                //running format has already applied this formatting 
                line.append(at(lnum).line.c_str()+pos, length); //now we copy it to the output.
                pos += length;
                break;
            case StringFormat::LINK2_ESCAPE:
                //keep it, do not change 'width' - it does not impact reflow
                //escapes with target stick to the word after them, empty ones to the word before
                (replaceto.length() ? keep_with : line).
                    append("\\" ESCAPE_STRING_NON_FORMATTING_LINK "(").append(replaceto).append(")");
                pos += length;
                break;
            case StringFormat::INVALID_ESCAPE:
            case StringFormat::REFERENCE:
            case StringFormat::LINK_ESCAPE:
            case StringFormat::LINE_BREAK:
            case StringFormat::SOFT_LINE_BREAK: 
            case StringFormat::NUMBERING: 
            case StringFormat::NUMBERING_FORMAT: 
                _ASSERT(0);  //These should not come here
                pos+=length; //If they come we just ignore them
                break;
            case StringFormat::NON_FORMATTING: 
            case StringFormat::NON_ESCAPE: 
            case StringFormat::SOLO_ESCAPE: 
                //Literal text. Calculate width using current format. Go word-by-word.
                size_t replaceto_pos = 0; //position within `replace_to`
                while (replaceto_pos < replaceto.length()) {
                    size_t replaceto_end = replaceto.find_first_not_of(LETTERS_NUMBERS, replaceto_pos);
                    if (replaceto_end == string::npos) //a single word remaining
                        replaceto_end = replaceto.length();
                    else if (replaceto_end == replaceto_pos) //non-letter first char: process a single char
                        replaceto_end++; 
                    const string word = replaceto.substr(replaceto_pos, replaceto_end-replaceto_pos);
                    const double word_width = running_format.getFragmentWidth(word, c);
                    if (word_width + width <= available || line.length() == 0) {
                        //either the word fits or we are at the beginning of the 
                        //line - add this word to the current line anyway
                        if (replaceto[replaceto_pos] != ' ' || line.length() != 0) {
                            //..but do not insert a space to the beginning of the line
                            if (keep_with.length()) {
                                line.append(keep_with);
                                keep_with.clear();
                            }
                            line.append(word);
                            width += word_width;
                            overflow = std::max(overflow, width - available);
                        }
                        replaceto_pos = replaceto_end;
                    } else {
                        // word has to move to a new line
                        reflown.push_back(ParsedLine(line, c, start_format, false));
                        start_format = running_format;
                        line.clear();
                        width = 0;
                        available = x - start_format.textHGapPre.second 
                                      - start_format.textHGapPost.second;
                    } 
                } //while (processing `replaceto`)
                pos += length;
            } //switch
        } // while (processing at(lnum)
        //if line ended in hard line break, we break line here
        if (at(lnum).hard_new_line) {
            if (line.length() == 0) 
                reflown.rbegin()->hard_new_line = true;
            else {
                reflown.push_back(ParsedLine(line, c, start_format, false));
                start_format = running_format;
                line.clear();
                width = 0;
                available = x - start_format.textHGapPre.second 
                              - start_format.textHGapPost.second;
            }
        }
        //if not at the beginning of a new line, add a space
        if (line.length() != 0) {
            line.append(" ");
            width += running_format.getFragmentWidth(" ", c);
        }
        lnum++;
        pos = 0;
    } while (lnum < size());
    //Flush any remaining text to a last line
    if (line.length())
        reflown.push_back(ParsedLine(line, c, start_format, false));
    //swap reflown in
    std::vector<ParsedLine>::swap(reflown);    
    return overflow;
}

double Label::getSpaceRequired(double def, int line) const
{
    if (size()==0) return 0;
    if (at(0).startFormat.word_wrap.first && at(0).startFormat.word_wrap.second)
        return def;
    return getTextWidthHeight(line).x;
}



void Label::CoverOrDrawOrISMap(Canvas *canvas, double sx, double dx, double y, double cx, bool isRotated, Contour *area, ISMap *ismap) const
{
    if (size()==0) return;
    XY xy;
    string target_at_front;
    xy.y = y + at(0).startFormat.textVGapAbove.second;
    for (size_t i = 0; i<size(); i++) {
        const XY wh = at(i).getWidthHeight();
        switch (at(i).startFormat.ident.second) {
        case MSC_IDENT_LEFT:
        default:
            xy.x = sx+at(i).startFormat.textHGapPre.second; 
            break;
        case MSC_IDENT_CENTER: {
            double w = wh.x + at(i).startFormat.textHGapPre.second + at(i).startFormat.textHGapPost.second;
            //if center, attempt to center around cx, but minimize extension beyond sx and dx
            if (cx<sx || cx>dx)  {                //if bad params, ignore cx
                xy.x = (sx + dx - w) / 2; 
            } else if (w <= dx-sx) {              //if text fits between, try around cx, but do not let outside sx or dx
                xy.x = std::max(cx - w/2, sx);
                xy.x = std::min(xy.x, dx - w);
            } else {                              //if text does not fit anyway, do it around cx
                xy.x = cx - w/2;
            }
            xy.x += at(i).startFormat.textHGapPre.second;
            break;
            }
        case MSC_IDENT_RIGHT:
            xy.x = dx - wh.x - at(i).startFormat.textHGapPost.second; break;
        }
        //Draw line of text
        if (area)
            *area += Block(xy, xy+wh); //TODO: Make this finer if there are smaller text or italics...
        else if (ismap && canvas)
            at(i).CollectIsMapElements(xy, *canvas, *ismap, target_at_front);
        else if (canvas)
            at(i).Draw(xy, *canvas, isRotated);
        xy.y += wh.y + at(i).startFormat.spacingBelow.second +
            at(i).startFormat.textVGapLineSpacing.second;
        if (i==0) 
            xy.y += first_line_extra_spacing;
    }
}

void Label::CollectIsMapElements(ISMap &ismap, Canvas &canvas, double sx, double dx, 
                                 double y, double cx, const XY &c, double angle) const
{ 
    ISMap tmp_ismap;
    CollectIsMapElements(tmp_ismap, canvas, sx, dx, y, cx);
    for (auto &e: tmp_ismap) {
        e.rect = Contour(e.rect).RotateAround(c, angle).GetBoundingBox();
        ismap.push_back(std::move(e));
    }
}


void Label::CollectIsMapElements(ISMap &ismap, Canvas &canvas, 
                                 double s, double d, double t, ESide side, double c) const
{ 
    if (side==ESide::LEFT)
        t -= getTextWidthHeight().y;
    ISMap tmp_ismap;
    CollectIsMapElements(tmp_ismap, canvas, s, d, t, c);
    for (auto &e: tmp_ismap) {
        switch (side) {
        case ESide::LEFT:
            e.rect.Rotate90CW();
            e.rect.Shift(XY(2*t+getTextWidthHeight().y, 0));
            break;
        case ESide::RIGHT:
            e.rect.Rotate90CCW();
            e.rect.Shift(XY(0, s+d));
        break;
        default:
            break;
        }
        ismap.push_back(std::move(e));
    }
}
                         
Contour Label::Cover(double s, double d, double t, ESide side, double c) const
{
    if (side==ESide::LEFT)
        t -= getTextWidthHeight().y;
    Contour ret = Cover(s, d, t, c);
    switch (side) {
    case ESide::LEFT:
        ret.Rotate(90);
        ret.Shift(XY(2*t+getTextWidthHeight().y, 0));
        break;
    case ESide::RIGHT:
        ret.Rotate(-90);
        ret.Shift(XY(0, s+d));
        break;
    default:
        break;
    }
    return ret;
}

void Label::Draw(Canvas &canvas, double s, double d, double t, ESide side, double c) const
{
    switch (side) {
    case ESide::LEFT:
        t -= getTextWidthHeight().y;
        canvas.Transform_Rotate90(2*t, getTextWidthHeight().y, false);
        break;
    case ESide::RIGHT:
        canvas.Transform_Rotate90(s, d, true);
        break;
    default:
        break;
    }
    Draw(canvas, std::min(s, d), std::max(s, d), t, c, side != ESide::END);
    if (side != ESide::END)
        canvas.UnTransform();
}
