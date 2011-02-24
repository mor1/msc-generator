/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011 Zoltan Turanyi
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
#include <assert.h>
#include <iostream>
#include "csh.h"
#include "msc.h"

//////////////////////////////////////////////////////////////////////////////////

template<> const char EnumEncapsulator<MscIdentType>::names[][ENUM_STRING_LEN] =
    {"invalid", "left", "center", "right", ""};


void AddTristate(std::pair<bool, tristate> &a, const std::pair<bool, tristate> b)
{
    if (!b.first) return;
    if (!a.first || b.second != invert) {
        a = b;
        return;
    }
    switch (a.second) {
    case no: a.second = yes; return;
    case yes: a.second = no; return;
    case invert: a.first = false; return;
    }
}

//////////////////////////////////////////////////////////////////////////////////

void StringFormat::Empty()
{
    color.first = false;
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
}

bool StringFormat::IsComplete() const
{
    return 
        color.first &&
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
        smallFontSize.first;
}

void StringFormat::Default() 
{
    normalFontSize.first = true; normalFontSize.second = 16; 
    smallFontSize.first = true; smallFontSize.second = 10;
    textHGapPre.first = true; textHGapPre.second = 4; 
    textHGapPost.first = true; textHGapPost.second = 2;
    textVGapAbove.first = true; textVGapAbove.second = 2;
    textVGapBelow.first = true; textVGapBelow.second = 2;
    textVGapLineSpacing.first = true; textVGapLineSpacing.second = 0; 
    ident.first = true; ident.second = MSC_IDENT_CENTER;
    color.first = true; color.second =  MscColorType(0,0,0); 
    fontType.first = true; fontType.second = 0;
    spacingBelow.first = true; spacingBelow.second =  0; 
    face.first = true; face.second =  "Arial";
    bold.first = true; bold.second = no;
    italics.first = true; italics.second = no; 
    underline.first = true; underline.second =  no;
}

StringFormat &StringFormat::operator =(const StringFormat &f)
{
    color = f.color;
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

    return *this;
}

//Recognize and potentially process one escape sequence in the beginning of input
//Return what is at that location:
//- a valid format-changing escape sequence (FORMATTING_OK)
//- an escape resulting in no formatting, but also no characters (FORMATTING_OK)
//- an invalid escape sequence (INVALID_ESCAPE)
//- a non-format-changing escape sequence (e.g., \\) (NON_FORMATTING)
//- just regular characters (NON_ESCAPE)
//- a \n escape (LINE_BREAK)
//- a \NN escape (NUMBERING)
//- a \N{1aAiI} escape (NUMBERING_FORMAT)
//- a lone '\' at the end of the string (SOLO_ESCAPE)
//In length return the length of the escape found (or for NON_ESCAPE the offset to the next '\').
//for FORMATTING_OK in replaceto return what to replace the escape to:
//  - color names are resolved to actual numbers
//  - style names are resolved to actual formatting instructions
//  - empty parenthesis \c(), \s(), \f(), \mX() are filled in with the value in "basic" or if NULL, then ignored
//for NON_FORMATTING in replaceto return the character represented by the escape (can be zero length for "\|")
//for NON_ESCAPE return the text verbatim
//for INVALID_ESCAPE return empty string
//for NUMBERING and NUMBERING_FORMAT return the actual escape
//What do we do:
//- if apply==true: Apply the string formatting escape to "this". (or do nothing for invalid escapes)
//- if resolve==true we resolve color/style references (stored in msc->Contexts). 
//  (If msc==NULL, or we do not find the stye/color we return INVALID_ESCAPE)
//  if apply==true, resolve parameter is ignored, we assume resolve==true.
//  resolve has no impact on other than \s \c and has impact only if msc!=NULL && apply==false
//- if linenum!=NULL: any problem generates an Error/Warning. if sayIgnore the we say we ignore the error,
//                    otherwise we say we keep the problematic escape as verbatim text.
//In general linenum carries the location of the first char of input. When we return, it contains the position
//of the character in input+length. (In the original input file this can e.g., be in a different line.)
//"length" returns the length of the escape (or non-escape string) we have processed
//"basic" carries the formatting we use to resolve \s() \c() \f() and \mX(). If NULL, we apply nothing even if "apply" is
//true and we keep the escape in replaceto unchanged. Basic must be a fully specified StringFormat (all .first == true)
#define PER_S_DEPRECATED_MSG "The use of '\\s' control escape to indicate small text is deprecated. Use '\\-' instead."
#define MAX_ESCAPE_M_VALUE 500
#define TOO_LARGE_M_VALUE_MSG  "Use an integer between [0..500]."
StringFormat::EEscapeType StringFormat::ProcessEscape(
	const char * const input, unsigned &length,
	bool resolve, bool apply, string *replaceto, const StringFormat *basic,
	Msc *msc, file_line *linenum, bool sayIgnore)
{
    /*
     ** Recognize the following escape sequences
     ** "\-" - switch to small font (also "\s")
     ** "\+" - switch to normal font
     ** "\^" - switch to superscript
     ** "\_" - swucth to subscript
     ** "\mX(num) - set text margings (X=_u_p, _d_own, _l_eft, _r_ight)
     **             and font size (X=_s_mall, _n_ormal)
     **             E.g. \mu(7) sets upper margin to 7.
     ** "\c(color) - set color, E.g., \c(0,0,0) is black
     ** "\pX"- set paragraph ident to _c_enter, _l_eft, _r_ight
     ** "\0".."\9" - keep this much of line space after the line
     ** "\\" - an escaped "\"
     ** \#[]{}"; - an escaped chars
     ** "\|" - a zero length non-formatting escape. Can be used to separate number from initial escapes.
     ** \n - a line break
     ** \N - insert line number here - should not appear in a numbering format
     ** \0x21 \0x2a \0x2A \0x2i \0x2I - used to define numbering formats, should not appear in a label
     ** \0x1(file,line,col) - notes the position of the next char in the original file
     */

    MscColorType c;
    string parameter;
    string maybe_s_msg;
    StyleSet::const_iterator i;
    const string errorAction = sayIgnore ? " Ignoring it." : " Keeping it as verbatim text.";
    const char *end;
    int was_m;

    //If this is not an escape, search for the next escape
    if (input[0]!='\\') {
        length = 0;
        while (input[length] && input[length]!='\\') length++;
        if (replaceto) replaceto->assign(input, length);
        if (linenum) linenum->col += length;
        return NON_ESCAPE;
    }

    const unsigned remaining = strlen(input);

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
            fontType.second = string("+-^_").find(input[1]);
        }
        goto ok2; //valid formatting character of length 2

    case 'b':    // bold font
    case 'B':
        if (apply)
            AddTristate(bold, std::pair<bool,tristate>(true, input[1]=='B'?yes:invert));
        goto ok2; //valid formatting character of length 2

    case 'i':    // bold font
    case 'I':
        if (apply)
            AddTristate(italics, std::pair<bool,tristate>(true, input[1]=='I'?yes:invert));
        goto ok2; //valid formatting character of length 2

    case 'u':    // bold font
    case 'U':
        if (apply)
            AddTristate(underline, std::pair<bool,tristate>(true, input[1]=='U'?yes:invert));
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
        MscIdentType id;
        switch (input[2]) {
        case 'c': id = MSC_IDENT_CENTER; break;
        case 'l': id = MSC_IDENT_LEFT; break;
        case 'r': id = MSC_IDENT_RIGHT; break;
        default: id = MSC_IDENT_INVALID; break;
        }
        if (id == MSC_IDENT_INVALID) {
            if (msc && linenum) {
                file_line l = *linenum;
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
            if (msc) msc->Error.Error(*linenum, "Internal error: bad number format escape.");
            return FORMATTING_OK;
        }
        if (replaceto) replaceto->assign(input, length);
        //No change to linenum, these should always be followed by position escapes
        return NUMBERING_FORMAT;

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

    case '|':          // zero-length non-formatting escape
        length = 2;
        if (replaceto) replaceto->clear();
        if (linenum) linenum->col += length;
        return NON_FORMATTING;
    }

    if (!strchr(ESCAPE_STRING_LOCATION "csfm", input[1])) {
        //Unrecognized escape comes here
        length = 2;
        if (msc && linenum)
            msc->Error.Error(*linenum, "Unrecognized escape: '" + string(input, 2) + "'." + errorAction);
        goto nok;
    }

    //All the following escapes take a parameter in parenthesis
    was_m = (input[1] == 'm');

    if (input[was_m+2]!='(') {
        length = 2 + was_m;
        if (input[1] == 's') {
            maybe_s_msg = "Missing style name after \\s control escape. Assuming small text switch.";
            goto maybe_s;
        }
        //skip silently if a location escape: we have inserted a bad one???
        if (input[1] == ESCAPE_CHAR_LOCATION) {
            if (replaceto) replaceto->clear();
            if (msc) msc->Error.Error(*linenum, "Internal error: no '(' after position escape.");
            return FORMATTING_OK;
        }

        if (msc && linenum) {
            file_line l = *linenum;
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
            if (msc) msc->Error.Error(*linenum, "Internal error: no matching ')' for position escape.");
            return FORMATTING_OK;
        }
        if (msc && linenum)
            msc->Error.Error(*linenum, "Missing closing parenthesis after " + string(input, length) +
                             " control escape." + errorAction);
        length = 3 + was_m;
        goto nok;
    }
    length = end-input+1; //since we have both ( and ) found, we have at least () after the escape
    parameter.assign(input+was_m+3, length-was_m-4); //stuff inside parenthesis

    //start with escapes taking a string value as parameter
    switch (input[1]) {
    case ESCAPE_CHAR_LOCATION:
        if (replaceto) replaceto->clear();
        if (linenum) {
            file_line l = *linenum;
            if (3!=sscanf(parameter.c_str(), "%d,%d,%d", &l.file, &l.line, &l.col)) {
                if (msc) msc->Error.Error(*linenum, "Internal error: could not parse position escape.");
                return FORMATTING_OK;
            }
            *linenum = l;
        }
        return FORMATTING_OK;
    case 'c':
        if (length==4) { // this is a "\c()"
            if (basic == NULL) {
                if (replaceto) *replaceto="\\c()";
                if (linenum) linenum->col += length;
                return FORMATTING_OK;
            }
            //substitute parameter to the value from basic
            parameter = basic->color.second.Print().substr(1);
            parameter.substr(0, parameter.length()-1);
        }
        if (msc)
            c = msc->Contexts.back().colors.GetColor(parameter); //consider color names and defs
        else if (apply || resolve) //try to resolve this if we are applying
            c = MscColorType(parameter);  //just consider defs
        else	   //if we are just parsing (probably for csh) keep as is.
            goto ok;
        if (c.valid) {
            if (apply) {
                color.first = true;
                color.second = c;
            }
            if (replaceto) replaceto->assign("\\c" + c.Print());
            if (linenum) linenum->col += length;
            return FORMATTING_OK;
        }
        if (msc && linenum) {
            file_line l = *linenum;
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
            if (replaceto) replaceto->assign(basic->Print());
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
            if (i->second.f_text) {
                if (apply)
                    *this += i->second.text;
                if (replaceto) replaceto->assign(i->second.text.Print());
            } else
                if (replaceto)
                    replaceto->clear();
            if (linenum) linenum->col += length;
            return FORMATTING_OK;
        }

    case 'f':
        if (length==4) { // this is a "\f()"
            if (basic == NULL) {
                if (replaceto) *replaceto="\\f()";
                if (linenum) linenum->col += length;
                return FORMATTING_OK;
            }
            //substitute parameter to the value from basic
            parameter = basic->face.second;
        }
        if (apply) {
            face.first = true;
            face.second = parameter;
        }
        if (replaceto) replaceto->assign("\\f(" + parameter + ")");
        if (linenum) linenum->col += length;
        return FORMATTING_OK;

    case 'm':
        std::pair<bool, double> *p;
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
            if (msc && linenum) {
                file_line l = *linenum;
                l.col += 2;
                msc->Error.Warning(l, "Escape '\\m' shall be followed by one of 'udlrins'." + errorAction);
            }
            length = 2;
            goto nok;
        }
        if (parameter.length()==0) { //this is \mX()
            if (basic) {
                const std::pair<bool, double> *p2 = reinterpret_cast<const std::pair<bool, double> *>((const char*)p - (const char*)this + (const char*)&basic);
                if (p2->first) {
                    if (apply)
                        *p = *p2;
                    string num;
                    num << p2->second;
                    if (replaceto) replaceto->assign(num);
                    if (linenum) linenum->col += length;
                    return FORMATTING_OK;
                }
	            if (replaceto) replaceto->clear();
            } else {
                if (replaceto) *replaceto="\\mX()";
                (*replaceto)[2] = input[2];
            }
            if (linenum) linenum->col += length;
            return FORMATTING_OK;
        }
        //OK, now we know we have a valid escape with a non-empty parameter, digest number
        int local_pos = 0;
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
                if (msc && linenum) {
                    file_line l = *linenum;
                    l.col += 4;
                    msc->Error.Error(l, msg, TOO_LARGE_M_VALUE_MSG);
                }
                goto nok;
            }
            msg.append(" Using value '").append(parameter.substr(0, local_pos));
            msg.append("' instead.");
            if (msc && linenum) {
                file_line l = *linenum;
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
            if (msc && linenum) {
                file_line l = *linenum;
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
            fontType.second = 1;
        }
        if (msc && linenum)
            msc->Error.Warning(*linenum, maybe_s_msg, PER_S_DEPRECATED_MSG);
        if (linenum) linenum->col += length;
        return FORMATTING_OK;
}

void StringFormat::ExtractCSH(int startpos, const char *text, Csh &csh)
{
    if (text==NULL) return;
    int pos=0;
    StringFormat sf;
    while (pos<strlen(text)) {
        MscColorSyntaxType color = COLOR_NORMAL;
        unsigned length;
        switch (sf.ProcessEscape(text+pos, length)) {
        case NUMBERING_FORMAT:
        case NUMBERING:
        case LINE_BREAK:
        case FORMATTING_OK:
        case NON_FORMATTING:
            color = COLOR_LABEL_ESCAPE; break;
        case INVALID_ESCAPE:
            color = COLOR_ERROR; break;
        case NON_ESCAPE:
		case SOLO_ESCAPE:
            color = COLOR_NORMAL; break;
        }
        if (color != COLOR_NORMAL) {
            CshPos loc;
            loc.first_pos = startpos+pos;
            loc.last_pos = loc.first_pos+length-1;
            csh.AddCSH(loc, color);
        }
        pos+=length;
    }
}

//Replaces style and color references to actual definitions found in msc->Contexts.back()
//Also performs syntax error checking and generates errors/warnings
//escape contais the string to parse (and change)
//if ignore then in errors we say we ignore the erroneous escape and also remove it from the str
//if not then we say we keep verbatim and we do so
//basic contains the baseline style and color, must be a fully specified StringFormat
//(the one to return to at \s() and \c() and \f(), mX())
//if NULL, those escapes remain in unchanged
//textType is NUMBER_FORMAT if this string is a number format (cannot contain \N)
//or it is LABEL or a TEXT_FORMAT (cannot contain \0x2{1aAiI}
void StringFormat::ExpandColorAndStyle(string &text, Msc *msc, file_line linenum,
                                       const StringFormat *basic, bool ignore, ETextType textType)
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
    int pos=0;
    string replaceto;
    StringFormat sf;
    string ignoreText = ignore?" Ignoring it.":"";
    while(text.length()>pos) {
        unsigned length;
        file_line beginning_of_escape = linenum;
        switch (sf.ProcessEscape(text.c_str()+pos, length, true, false, &replaceto, basic, msc, &linenum, ignore)) {
        case INVALID_ESCAPE:
            if (ignore) break;  //replaceto is empty here, we will remove the bad escape
            //fallthrough, if we do not ignore: this will set replaceto to the bad escape
        case LINE_BREAK:        //keep \n as is
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
        case NUMBERING:         //keep \NN as is
            if (textType == NUMBER_FORMAT) {
                msc->Error.Error(beginning_of_escape, "The '\\N' escape can not be used for numbering formatting options. Ignoring it.");
                replaceto.clear();
            }
            break;
        case NON_ESCAPE:       //verbatim text. If numberformat search it
            if (textType != NUMBER_FORMAT) break;
            if (NumberingStyleFragment::FindReplaceNumberFormatToken(text, beginning_of_escape, pos))
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

int StringFormat::FindNumberingFormatEscape(const char *text)
{
    StringFormat sf;
    unsigned pos = 0;
	unsigned text_length = strlen(text);
    while (pos < text_length) {
        unsigned length;
		if (NUMBERING_FORMAT == sf.ProcessEscape(text+pos, length))
            return pos;
        pos += length;
    }
	return -1;
}

void StringFormat::RemovePosEscapes(string &text)
{
    StringFormat sf;
    unsigned pos = 0;
    while (pos < text.length()) {
        unsigned length;
        if (FORMATTING_OK == sf.ProcessEscape(text.c_str()+pos, length))
            if (text[pos]=='\\' && text[pos+1]=='\x01') {
                text.erase(pos, length);
                continue;
            }
        pos += length;
    }
}

void StringFormat::ConvertToPlainText(string &text)
{
    StringFormat sf;
    unsigned pos = 0;
    while (pos < text.length()) {
        unsigned length;
        switch (sf.ProcessEscape(text.c_str()+pos, length)) {
        case NON_FORMATTING:
        case NON_ESCAPE:
            pos += length;
            break;
        case LINE_BREAK:
            text.replace(pos, length, "\n");
            pos++;
            break;
        default:
            text.erase(pos, length);  //all other escapes
            break;
        }
    }
}


//Parse the string as long as valid escape sequences come and apply their meaning
//If you hit something not a valid format changing escape char, stop & return
//return the #of characters processed
unsigned StringFormat::Apply(const char *text)
{
    unsigned pos = 0;
    unsigned length;
	StringFormat basic(*this);
    while (pos < strlen(text)) {
        if (FORMATTING_OK != ProcessEscape(text+pos, length, true, true, NULL, &basic)) break;
        pos += length;
    }
    return pos;
}

unsigned StringFormat::Apply(string &text)
{
    unsigned len = Apply(text.c_str());
    text.erase(0, len);
    return len;
}


void StringFormat::SetColor(MscColorType c)
{
    color.first = true;
    color.second = c;
}

StringFormat &StringFormat::operator +=(const StringFormat& toadd)
{
    if (toadd.fontType.first)
        fontType = toadd.fontType;

    if (toadd.color.first)
        color = toadd.color;

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

    return *this;
}

string StringFormat::Print() const
{
    string ret;
    char fonttypes[] = "+-^_";
    if (fontType.first)
        ret << string("\\") + fonttypes[fontType.second];;

    if (color.first)
        ret << "\\c" + color.second.Print();

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

    return ret;
}

bool StringFormat::AddAttribute(const Attribute &a, Msc *msc, StyleType t)
{
    if (a.type == MSC_ATTR_STYLE) {
        StyleSet::const_iterator i = msc->Contexts.back().styles.find(a.name);
        if (i == msc->Contexts.back().styles.end()) {
            a.InvalidStyleError(msc->Error);
            return true;
        }
        if (i->second.f_text) operator +=(i->second.text);
        return true;
    }
    if (a.EndsWith("color")) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t))
                color.first = false;
            return true;
        }
        if (!a.CheckColor(msc->Contexts.back().colors, msc->Error)) return true;
        color.first = true;
        color.second = msc->Contexts.back().colors.GetColor(a.value);
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

        StringFormat::ExpandColorAndStyle(tmp, msc, a.linenum_value.start, NULL, this, TEXT_FORMAT);

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
    return false;

}

void StringFormat::AttributeNames(Csh &csh)
{
    static const char names[][ENUM_STRING_LEN] =
    {"", "text.color", "text.ident", "text.format", ""};
    csh.AddToHints(names, csh.HintPrefix(COLOR_ATTRNAME), HINT_ATTR_NAME);
}

bool CshHintGraphicCallbackForTextIdent(MscDrawer *msc, CshHintGraphicParam p)
{
    if (!msc) return false;
    const MscIdentType t = (MscIdentType)(int)p;
    const static double sizePercentage[] = {50, 30, 60};
    const MscLineAttr line(LINE_SOLID, MscColorType(0,0,0), 1, CORNER_NONE, 0);
    double y = floor(HINT_GRAPHIC_SIZE_Y*0.2)+0.5;
    double y_inc = ceil(HINT_GRAPHIC_SIZE_Y*0.3/(sizeof(sizePercentage)/sizeof(double)-1));
    for (int i=0; i<sizeof(sizePercentage)/sizeof(double); i++) {
        double x1 = floor(HINT_GRAPHIC_SIZE_X*sizePercentage[i]/100.+0.5);
        double x2 = floor(HINT_GRAPHIC_SIZE_X*0.2);
        switch (t) {
        case MSC_IDENT_LEFT: x1 = x2+x1; break;
        case MSC_IDENT_CENTER: x2 = floor(HINT_GRAPHIC_SIZE_X*0.5)-x1/2; x1 = x1+x2; break;
        case MSC_IDENT_RIGHT: x2 = floor(HINT_GRAPHIC_SIZE_X*0.8); x1 = x2-x1; break;
        }
        msc->Line(XY(x1, y+y_inc), XY(x2, y+y_inc), line);
        y+=y_inc;
    }
    return true;
}

bool StringFormat::AttributeValues(const std::string &attr, Csh &csh)
{
    if (CaseInsensitiveEndsWith(attr, "color")) {
        csh.AddColorValuesToHints();
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "ident")) {
        csh.AddToHints(EnumEncapsulator<MscIdentType>::names, csh.HintPrefix(COLOR_ATTRVALUE), HINT_ATTR_VALUE,
            CshHintGraphicCallbackForTextIdent);
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "format")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable()+"<\format string\">", HINT_ATTR_VALUE, false));
        return true;
    }
    return false;
}


//This shall be called only after StringFormat::ExpandColorAndStyle on a label and both num strings
//If we find a \N escape in the label we replace that to num (for multiple \Ns, if needed)
//If we find no such escape, we add it to the beginning, but after the initial formatting strings
void StringFormat::AddNumbering(string &label, const string &num, const string &pre_num_post)
{
    if (label.length()==0) return;
	StringFormat sf;
	unsigned pos= 0;
	unsigned length;
	while (pos < label.length()) {
        if (FORMATTING_OK != sf.ProcessEscape(label.c_str()+pos, length)) break;
        pos += length;
    }
    bool number_added = false;
    unsigned beginning_pos = pos;
    while (pos < label.length()) {
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

void StringFormat::ApplyFontToContext(MscDrawer *mscd) const
{
    StringFormat to_use; //default
    to_use += *this; //apply our settings;
    mscd->SetFontFace(to_use.face.second.c_str(), to_use.italics.first&&to_use.italics.second,
                      to_use.bold.first&&to_use.bold.second);

    mscd->SetFontSize(to_use.smallFontSize.second);
    cairo_font_extents(mscd->GetContext(), &smallFontExtents);
    mscd->SetFontSize(to_use.normalFontSize.second);
    cairo_font_extents(mscd->GetContext(), &normalFontExtents);

    if (to_use.fontType.second == 0) /*Normal font*/
        mscd->SetFontSize(to_use.normalFontSize.second) ;
    else /* Small, subscript, superscript */
        mscd->SetFontSize(to_use.smallFontSize.second);
    mscd->SetColor(to_use.color.second);
}

//front is yes if we want heading space width and false if trailing
double StringFormat::spaceWidth(const string &text, MscDrawer *mscd, bool front) const
{
    if (text.length()==0 || !mscd->fake_spaces) return 0;
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
    ApplyFontToContext(mscd);
    cairo_text_extents_t te;
	cairo_text_extents (mscd->GetContext(), "M N", &te); //this counts one more space then needed
    double  w = te.x_advance;
    cairo_text_extents (mscd->GetContext(), "MN", &te);
    w -= te.x_advance;
	return w*s;
}

double StringFormat::getFragmentWidth(const string &s, MscDrawer *mscd) const
{
    if (s.length()==0 || mscd==NULL) return 0;
    _ASSERT(IsComplete()); //XXX If no failt, remove to_use bullshit
    StringFormat to_use; //default
    to_use += *this; //apply our settings;
    to_use.ApplyFontToContext(mscd);
    cairo_text_extents_t te;
    if (mscd->individual_chars) {
        double advance = 0;
        char tmp_stirng[2] = "a";
        for (int i=0; i<s.length(); i++) {
            tmp_stirng[0] = s[i];
            cairo_text_extents(mscd->GetContext(), tmp_stirng, &te);
            advance += te.x_advance;
        }
        te.x_advance = advance;
    } else {
        cairo_text_extents (mscd->GetContext(), s.c_str(), &te);
    }
    return spaceWidth(s, mscd, true) + te.x_advance + spaceWidth(s, mscd, false);
}

double StringFormat::getFragmentHeightAboveBaseLine(const string &s,
                                                      MscDrawer *mscd) const
{
    if (s.length()==0 || mscd==NULL) return 0;
    _ASSERT(IsComplete()); //XXX If no failt, remove to_use bullshit
    StringFormat to_use; //default
    to_use += *this; //apply our settings;
    to_use.ApplyFontToContext(mscd);
    switch(to_use.fontType.second) {
    case 0:  //normal font
        return to_use.normalFontExtents.ascent;
    case 1:  //Small font
        return to_use.smallFontExtents.ascent;
    case 2: //superscript - typeset 20% higher than normal font
        return to_use.normalFontExtents.ascent*1.2;
    case 3: //subscript - typeset 20% lower than normal font
        return std::max(0., to_use.smallFontExtents.ascent-to_use.normalFontExtents.ascent*0.2);
    }
    return 0;
}

double StringFormat::getFragmentHeightBelowBaseLine(const string &s,
                                                      MscDrawer *mscd) const
{
    if (s.length()==0 || mscd==NULL) return 0;
    _ASSERT(IsComplete()); //XXX If no failt, remove to_use bullshit
    StringFormat to_use; //default
    to_use += *this; //apply our settings;
    to_use.ApplyFontToContext(mscd);
    switch(to_use.fontType.second) {
    case 0:  //normal font
        return to_use.normalFontExtents.descent;
    case 1:  //Small font
        return to_use.smallFontExtents.descent;
    case 2: //superscript - typeset 20% higher than normal font
        return std::max(0., to_use.smallFontExtents.descent - to_use.normalFontExtents.ascent*0.2);
    case 3: //subscript - typeset 20% lower than normal font
        return to_use.smallFontExtents.descent + to_use.normalFontExtents.ascent*0.2;
    }
    return 0;
}

double StringFormat::drawFragment(const string &s, MscDrawer *mscd, XY xy, bool isRotated) const
{
    if (s.length()==0 || mscd==NULL) return 0;
    //Mybe we have not all fields set
    _ASSERT(IsComplete()); //XXX If no failt, remove to_use bullshit
    StringFormat to_use; //default
    to_use += *this; //apply our settings;
    to_use.ApplyFontToContext(mscd);
    switch(to_use.fontType.second) {
    case 0:  //normal font
    case 1:  //Small font
        break;
    case 2: //superscript - typeset 20% higher than normal font
        xy.y -= to_use.normalFontExtents.ascent*0.2;
        break;
    case 3: //subscript - typeset 20% lower than normal font
        xy.y += to_use.normalFontExtents.ascent*0.2;
        break;
    }

    cairo_text_extents_t te;
    cairo_text_extents (mscd->GetContext(), s.c_str(), &te);
	xy.x += spaceWidth(s, mscd, true);
	mscd->Text(xy, s, isRotated);

    double advance = spaceWidth(s, mscd, true) + te.x_advance + spaceWidth(s, mscd, false);

    if (underline.first && underline.second) {
        xy.y++;
        XY xy2(xy.x+advance, xy.y);
        mscd->Line(xy, xy2, MscLineAttr(LINE_SOLID, to_use.color.second, 1, CORNER_NONE, 0));
    }
    return advance;
}

//////////////////////////////////////////////////////////////

ParsedLine::ParsedLine(const string &in, MscDrawer *mscd, StringFormat &format) :
    line(in), width(0), heightAboveBaseLine(0), heightBelowBaseLine(0)
{
    format.Apply(line); //eats away initial formatting, ensures we do not start with escape
    startFormat = format;
    size_t pos = 0;
    unsigned length;
    string replaceto;
    string fragment;
    while (line.length()>pos) {
        fragment.clear();
        //collect characters up until we hit a vaild formatting escape (or string end)
        while (line.length()>pos) {
            //we avoid changing format!
            if (StringFormat::FORMATTING_OK == format.ProcessEscape(line.c_str()+pos, length, true, false, &replaceto, &startFormat)) break;
            fragment.append(replaceto);
            pos += length;
        }
        //store fragment data
        width += format.getFragmentWidth(fragment, mscd);
        heightAboveBaseLine =
            std::max(heightAboveBaseLine, format.getFragmentHeightAboveBaseLine(fragment, mscd));
        heightBelowBaseLine =
            std::max(heightBelowBaseLine, format.getFragmentHeightBelowBaseLine(fragment, mscd));

        //Apply the formatting escapes as they come
        if (line.length()>pos)
            pos += format.Apply(line.c_str()+pos);
    }
    //Add spacing below. If there is spacingbelow, span a full height line
    if (heightAboveBaseLine == 0 && format.getSpacingBelow())
        heightAboveBaseLine = format.getFragmentHeightAboveBaseLine("M", mscd);
};

ParsedLine::operator std::string() const
{
    StringFormat format(startFormat);
    size_t pos = 0;
    unsigned length;
    string replaceto;
    string ret;

    while (line.length()>pos) {
        //collect characters up until we hit a vaild formatting escape (or string end)
        while (line.length()>pos) {
            if (StringFormat::FORMATTING_OK == format.ProcessEscape(line.c_str()+pos, length, true, false, &replaceto, &startFormat)) break;
            ret.append(replaceto);
            pos += length;
        }
        //Apply the formatting escapes as they come
        if (line.length()>pos)
            pos += format.Apply(line.c_str()+pos);
    }
    return ret;
}

void ParsedLine::Draw(XY xy, MscDrawer *mscd, bool isRotated) const
{
    StringFormat format(startFormat);
    size_t pos = 0;
    unsigned length;
    string replaceto;
    string fragment;

    xy.y += heightAboveBaseLine;

    while (line.length()>pos) {
        fragment.clear();
        //collect characters up until we hit a vaild formatting escape (or string end)
        while (line.length()>pos) {
            if (StringFormat::FORMATTING_OK == format.ProcessEscape(line.c_str()+pos, length, true, false, &replaceto, &startFormat)) break;
            fragment.append(replaceto);
            pos += length;
        }
        //draw Fragment
        xy.x += format.drawFragment(fragment, mscd, xy, isRotated);

        //Apply the formatting escapes as they come
        if (line.length()>pos)
            pos += format.Apply(line.c_str()+pos);
    }
}

//////////////////////////////////////////////////


/** Split a string to substrings based on '\n' delimiters.
 * Then parse the strings for escape control characters
 */
unsigned Label::AddText(const string &input, StringFormat format)
{
    size_t pos = 0, line_start = 0;
    unsigned length;
    while (pos < input.length()) {
        //find next new line
        while (pos < input.length()) {
            if (StringFormat::LINE_BREAK == format.ProcessEscape(input.c_str()+pos, length)) break; //format not changed here!!!
            pos += length;
        }
        //Add a new line
        push_back(ParsedLine(input.substr(line_start, pos-line_start), msc, format)); //format changed here
        pos += length; //add the length of the line break itself
        line_start = pos;
    }
    return this->size();
}

void Label::AddSpacing(unsigned line, double spacing)
{
    if (this->size()<line) return;
    if (at(line).startFormat.spacingBelow.first)
        at(line).startFormat.spacingBelow.second += spacing;
    else {
        at(line).startFormat.spacingBelow.first = true;
        at(line).startFormat.spacingBelow.second = spacing;
    }
}

Label::operator std::string() const
{
    string ret;
    if (size()>0)
        for (unsigned i = 0; i<size(); i++)
            ret += at(i);
    return ret;
}


/* Get the width of a (potentially multi-line) text or one of its lines
 *  If text is empty return 0
 * we add spacings around,
 * for an individual line, we add the spacings above, but not the ones below,
 */
XY Label::getTextWidthHeight(int line) const
{
    XY xy(0,0);
    if (size()==0) return xy;
    if (line!=-1) {
        if (size()<=line) return xy;
        xy = at(line).getWidthHeight();
        if (line == 0)  //First line
            xy.y += at(0).startFormat.textVGapAbove.second;
        else
            xy.y += at(line-1).startFormat.textVGapLineSpacing.second +
                at(line-1).startFormat.spacingBelow.second;
    } else {
        for (unsigned i = 0; i<size(); i++) {
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
        xy.x=xy.y=0;
    return xy;
};

void Label::CoverOrDraw(double sx, double dx, double y, double cx, bool isRotated, Area *area) const
{
    if (size()==0) return;
    XY xy;
    xy.y = y + at(0).startFormat.textVGapAbove.second;
    for (unsigned i = 0; i<size(); i++) {
        const XY wh = at(i).getWidthHeight();
        switch (at(i).startFormat.ident.second) {
        case MSC_IDENT_LEFT:
        default:
            xy.x = sx+at(i).startFormat.textHGapPre.second; 
            break;
        case MSC_IDENT_CENTER: {
            double w = wh.x + at(i).startFormat.textHGapPre.second + at(i).startFormat.textHGapPost.second;
            //if center, attempt to center around cx, but minimize extension beyond sx and dx
            if (w >= dx-sx || cx<sx || cx>dx) xy.x = (sx + dx - w) / 2; 
            else {
                xy.x = std::max(cx - w/2, sx);
                xy.x = std::min(xy.x, dx - w);
            }   
            break;
            }
        case MSC_IDENT_RIGHT:
            xy.x = dx - wh.x - at(i).startFormat.textHGapPost.second; break;
        }
        //Draw line of text
        if (area)
            *area += Block(xy, xy+wh); //TODO: Make this finer if there are smaller text or italics...
        else
            at(i).Draw(xy, msc, isRotated);
        xy.y += wh.y + at(i).startFormat.spacingBelow.second +
            at(i).startFormat.textVGapLineSpacing.second;
    }
}
