/*
    This file is part of Msc-generator.
	Copyright 2008,2009,2010 Zoltan Turanyi
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

template<> const char EnumEncapsulator<MscIdentType>::names[][15] =
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

StringFormat::StringFormat(void) :
    normalFontSize(true,16), smallFontSize(true, 10),
    textHGapPre(true, 4), textHGapPost(true, 2),
    textVGapAbove(true, 2), textVGapBelow(true,2),
    textVGapLineSpacing(true, 0), ident(true, MSC_IDENT_CENTER),
    color(true, MscColorType(0,0,0)), fontType(true, 0),
    spacingBelow(true, 0), face(true, "Arial"),
    bold(true,no), italics(true,no), underline(true, no)
{
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

//Recognize and potentially process one escape sequence in the input at position pos
//Return what is at that location:
//- a valid format-changing escape sequence (FORMATTING_OK)
//- an invalid format-changing escape sequence (FORMATTING_NOK)
//- a non-format-changing escape sequence (e.g., \\) (NON_FORMATTING)
//- just regular characters (NON_ESCAPE)
//- a \n escape (LINE_BREAK)
//In length return the length of the escape found (or for NON_ESCAPE the offset to the next '\').
//for FORMATTING_OK in replaceto return what to replace the escape to:
//  - color names are resolved to actual numbers
//  - style names are resolved to actual formatting instructions
//  - empty parenthesis \c(), \s(), \f(), \mX() are filled in with the value in "basic" or if NULL, then ignored
//for NON_FORMATTING in replaceto return the character represented by the escape
//for NON_ESCAPE return the text verbatim
//for FORMATTING_NOK return empty string
//- if apply==true: Apply the string formatting escape to "this". (or do nothing for not valid formating escapes)
//- if resolve==true we resolve strings in msc. If msc==NULL, or we do not find the stye/color we return NOK
//  if apply==true, resolve parameter is ignored, we assume resolve==true.
//  resolve has no impact on other than \s \c and only if msc==NULL, apply==false
//- if reportError==true: any problem is generated an Error. if sayIgnore the we say we ignore the error,
//                        otherwise we say we keep it as verbatim text.
#define PER_S_DEPRECATED_MSG "The use of '\\s' control escape to indicate small text is deprecated. Use '\\-' instead."
#define MAX_ESCAPE_M_VALUE 500
#define TOO_LARGE_M_VALUE_MSG  "Use an integer between [0..500]."
StringFormat::EEscapeType StringFormat::ProcessEscape(
	const char * const input, const int pos, unsigned &length,
	bool resolve, bool apply, string *replaceto, const StringFormat *basic,
	Msc *msc, file_line linenum, bool reportError, bool sayIgnore)
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
     ** "\\" - an escaped "\"
     ** \#[]{}"; - an escaped chars
     ** "\0".."\9" - keep this much of line space after the line
     */

    unsigned value;
    MscColorType c;
    string parameter;
    string maybe_s_msg;
    StyleSet::const_iterator i;
    const string errorAction = sayIgnore ? " Ignoring it." : " Keeping it as verbatim text.";
    const char *end;
    bool was_m;

    //If this is not an escape, search for the next escape
    if (input[pos]!='\\') {
        length = 0;
        while (input[pos+length] && input[pos+length]!='\\') length++;
        if (replaceto) replaceto->assign(input+pos, length);
        return NON_ESCAPE;
    }

    const unsigned remaining = strlen(input+pos);

    //First check for two-or three character escapes not taking an argument
    switch (input[pos+1]) {
    case '-':    // small font
    case '+':    // normal font
    case '^':    // superscript
    case '_':    // subscript
        if (apply) {
            fontType.first = true;
            fontType.second = string("+-^_").find(input[pos+1]);
        }
        goto ok2; //valid formatting character of length 2

    case 'b':    // bold font
    case 'B':
        if (apply)
            AddTristate(bold, std::pair<bool,tristate>(true, input[pos+1]=='B'?yes:invert));
        goto ok2; //valid formatting character of length 2

    case 'i':    // bold font
    case 'I':
        if (apply)
            AddTristate(italics, std::pair<bool,tristate>(true, input[pos+1]=='I'?yes:invert));
        goto ok2; //valid formatting character of length 2

    case 'u':    // bold font
    case 'U':
        if (apply)
            AddTristate(underline, std::pair<bool,tristate>(true, input[pos+1]=='U'?yes:invert));
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
            spacingBelow.second = input[pos+1] - '0';
        }
        goto ok2; //valid formatting character of length 2

    case 'p':
        MscIdentType id;
        switch (input[pos+2]) {
        case 'c': id = MSC_IDENT_CENTER; break;
        case 'l': id = MSC_IDENT_LEFT; break;
        case 'r': id = MSC_IDENT_RIGHT; break;
        default: id = MSC_IDENT_INVALID; break;
        }
        if (id == MSC_IDENT_INVALID) {
            if (msc && reportError)
                msc->Error.Warning(linenum, "Escape '\\p' shall be followed by one of 'lrc'." + errorAction);
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
        if (replaceto) replaceto->erase();
        return LINE_BREAK;

    case '\\':          // escaped "\"
    case '#':           // escaped "#"
    case '{':           // escaped "{"
    case '}':           // escaped "}"
    case '[':           // escaped "["
    case ']':           // escaped "]"
    case ';':           // escaped ";"
    case '\"':          // escaped quotation mark
        length = 2;
        if (replaceto) replaceto->assign(input+pos+1, 1);
        return NON_FORMATTING;
    }

    if (!strchr("csfm", input[pos+1])) {
        //Unrecognized escape comes here
        length = 2;
        if (msc && reportError)
            msc->Error.Error(linenum, "Unrecognized escape: '" + string(input+pos, 2) + "'." + errorAction);
        goto nok;
    }

    //All the following escapes take a parameter in parenthesis
    was_m = (input[pos+1] == 'm');

    if (input[pos+was_m+2]!='(') {
        length = 2 + was_m;
        if (input[pos+1] == 's') {
            maybe_s_msg = "Missing style name after \\s control escape. Assuming small text switch.";
            goto maybe_s;
        }
        if (msc && reportError)
            msc->Error.Error(linenum, "Missing parameter after " + string(input+pos, length) +
                             " control escape." + errorAction);
        goto nok;
    }
    end = strchr(input+pos+was_m, ')');
    if (!end) {
        if (input[pos+1] == 's') {
            maybe_s_msg = "Missing closing parenthesis after \\s control escape. Assuming small text switch.";
            goto maybe_s;
        }
        if (msc && reportError)
            msc->Error.Error(linenum, "Missing closing parenthesis after " + string(input+pos, length) +
                             " control escape." + errorAction);
        length = 3 + was_m;
        goto nok;
    }
    length = end-input-pos+1; //since we have both ( and ) found, we have at least () after the escape
    parameter.assign(input+pos+was_m+3, length-was_m-4); //stuff inside parenthesis

    //start with escapes taking a string value as parameter
    switch (input[pos+1]) {
    case 'c':
        if (length==4) { // this is a "\c()"
            if (basic == NULL) {
                if (replaceto) replaceto->erase();
                return FORMATTING_OK;
            }
            //substitute parameter to the value from basic
            parameter = basic->color.second.Print().substr(1);
            parameter.substr(0, parameter.length()-1);
        }
        if (msc)
            c = msc->ColorSets.top().GetColor(parameter); //consider color names and defs
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
            return FORMATTING_OK;
        }
        if (msc && reportError)
            msc->Error.Error(linenum, "Unrecognized color name or definition: '" + parameter + "'." + errorAction);
        goto nok;

    case 's':
        if (length==4) { // this is a "\s()"
            if (basic == NULL) {
                if (replaceto) replaceto->erase();
                return FORMATTING_OK;
            }
            if (replaceto) replaceto->assign(basic->Print());
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
        i = msc->StyleSets.top().find(parameter);
        if (i==msc->StyleSets.top().end()) {
            maybe_s_msg="Unrecognized style '" + parameter +
                "'. Treating style name as small text in parenthesis.";
            goto maybe_s;
        } else {
            if (i->second.f_text) {
                if (apply)
                    *this += i->second.text;
                if (replaceto) replaceto->assign(i->second.text.Print());
            } else
                if (replaceto) replaceto->erase();
            return FORMATTING_OK;
        }

    case 'f':
        if (length==4) { // this is a "\f()"
            if (basic == NULL) {
                if (replaceto) replaceto->erase();
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
        return FORMATTING_OK;

    case 'm':
        std::pair<bool, double> *p;
        int modifer = 0;
        switch (input[pos+2]) {
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
            if (msc && reportError)
                msc->Error.Warning(linenum, "Escape '\\m' shall be followed by one of 'udlrins'." + errorAction);
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
                    return FORMATTING_OK;
                }
            }
            if (replaceto) replaceto->erase();
            return FORMATTING_OK;
        }
        //OK, now we know we have a valid escape with a non-empty parameter, digest number
        int local_pos = 0;
        double value = 0;
        while (parameter.length()>local_pos && parameter[local_pos]>='0' && parameter[local_pos]<='9') {
            value = value*10 + parameter[local_pos]-'0';
            local_pos++;
        }
        if (parameter.length()>local_pos) {
            string msg = "Invalid value to the '\\m";
            msg += input[pos+2];
            msg.append("' control escape: '").append(parameter).append("'.");
            if (value>MAX_ESCAPE_M_VALUE) {
                msg.append(" I could deduct '").append(parameter.substr(0, local_pos));
                msg.append("', but that seems too large.");
                if (sayIgnore)
                    msg.append(" Ignoring control escape.");
                else
                    msg.append(" Keeping escape as verbatim text.");
                if (msc && reportError)
                    msc->Error.Error(linenum, msg, TOO_LARGE_M_VALUE_MSG);
                goto nok;
            }
            msg.append(" Using value '").append(parameter.substr(0, local_pos));
            msg.append("' instead.");
            if (msc && reportError)
                msc->Error.Error(linenum, msg, TOO_LARGE_M_VALUE_MSG);
        } else if (value>MAX_ESCAPE_M_VALUE) { //Ok here we successfully parsed the number
            string msg = "Too large value after the '\\m";
            msg += input[pos+2];
            msg.append("' control escape: '").append(parameter).append("'.");
            if (sayIgnore)
                msg.append(" Ignoring control escape.");
            else
                msg.append(" Keeping escape as verbatim text.");
            if (msc && reportError)
                msc->Error.Error(linenum, msg, TOO_LARGE_M_VALUE_MSG);
            goto nok;
        }
        //OK, here good value we have
        if (apply) {
            p->first = true;
            p->second = value + modifer;
        }
        //It is this complicated because it may be that we use just (the valid) part of parameter
        if (replaceto) replaceto->assign(string(input+pos,3) + "(" + parameter.substr(0, local_pos) + ")");
        return FORMATTING_OK;
    } /* big switch */

    //fallthrough, but we should not be here
    nok:
    if (replaceto) replaceto->erase();
    return FORMATTING_NOK;


    ok2:
        length = 2;
        //fallthrough
    ok:
        if (replaceto) replaceto->assign(input+pos, length);
        return FORMATTING_OK;

    maybe_s: //we get here when it is \s, but not a valid style or ()s after
        if (replaceto) replaceto->assign("\\-");
        length = 2;
        if (apply) {
            fontType.first = true;
            fontType.second = 1;
        }
        if (msc && reportError)
            msc->Error.Warning(linenum, maybe_s_msg, PER_S_DEPRECATED_MSG);
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
        switch (sf.ProcessEscape(text, pos, length)) {
        case LINE_BREAK:
        case FORMATTING_OK:
        case NON_FORMATTING:
            color = COLOR_LABEL_ESCAPE; break;
        case FORMATTING_NOK:
            color = COLOR_ERROR; break;
        case NON_ESCAPE:
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

//Replaces style and color references to actual definitions found in msc->StyleSets.top()
//and ColorSets.top(). Also performs syntax error checking and generates errors/warnings
//escape contais the string to parse (and change)
//if ignore then in errors we say we ignore the erroneous escape and also remove it from the str
//if not then we say we keep verbatim and we do so
//basic contains the baseline style and color
//(the one to return to at \s() and \c() and \f(), mX())
void StringFormat::ExpandColorAndStyle(string &text, Msc *msc, file_line linenum,
									   const StringFormat *basic, bool ignore)
{
    int pos=0;
    string replaceto;
    StringFormat sf;
    while(text.length()>pos) {
        unsigned length;
        switch (sf.ProcessEscape(text.c_str(), pos, length, true, false, &replaceto, basic, msc, linenum, true, ignore)) {
        case FORMATTING_NOK:
            if (ignore) break;  //replaceto is empty here, we will remove the bad esc.
            //fallthrough
        case LINE_BREAK:        //keep \n as is
        case NON_FORMATTING:
            replaceto.assign(text.c_str()+pos, length); break; //do not (yet) resolve: keep what is in input
        }
        text.replace(pos, length, replaceto);
        pos += replaceto.length();
    }
}

//Parse the string as long as valid escape sequences come and apply their meaning
//If you hit something not a valid format changing escape char, stop & return
//return the #of characters processed
unsigned StringFormat::Apply(const char *text)
{
    unsigned pos = 0;
    unsigned length;
    while (pos < strlen(text)) {
        if (FORMATTING_OK != ProcessEscape(text, pos, length, true, true)) break;
        pos += length;
    }
    return pos;
}

unsigned StringFormat::Apply(string &text)
{
    unsigned pos = Apply(text.c_str());
    text.erase(0, pos);
    return pos;
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
        StyleSet::const_iterator i = msc->StyleSets.top().find(a.name);
        if (i == msc->StyleSets.top().end()) {
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
        if (!a.CheckColor(msc->ColorSets.top(), msc->Error)) return true;
        color.first = true;
        color.second = msc->ColorSets.top().GetColor(a.value);
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

		StringFormat::ExpandColorAndStyle(tmp, msc, a.linenum_value.start, NULL, this);

        StringFormat sf(tmp);
        if (tmp.length()) {
            string s("Could not recognize these:'");
            s.append(tmp).append("' in attribute '").append(a.name).append("'.");
            s.append(" Applying only what I understood.");
            msc->Error.Warning(a, true, s);
            return true;
        }
        operator += (sf);
        return true;
    }
    return false;

}
//This shall be called only after StringFormat::ExpandColorAndStyle on a label
void StringFormat::AddNumbering(string &label, int num)
{
    if (label.length()==0 || num == -1000) return;
	StringFormat sf;
	unsigned pos = 0;
	unsigned length;
	while (pos < label.length()) {
		if (FORMATTING_OK != sf.ProcessEscape(label.c_str(), pos, length)) break;
		pos += length;
	}
    string ss;
    ss << num << ": ";
    label.insert(pos, ss);
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
	mscd->text(xy, s, isRotated);

    double advance = spaceWidth(s, mscd, true) + te.x_advance + spaceWidth(s, mscd, false);

    if (underline.first && underline.second) {
        xy.y++;
        XY xy2(xy.x+advance, xy.y);
        mscd->line(xy, xy2, MscLineAttr(LINE_SOLID, to_use.color.second, 1, 0));
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
        fragment.erase();
        //collect characters up until we hit a vaild formatting escape (or string end)
        while (line.length()>pos) {
            //we avoid changing format!
            if (StringFormat::FORMATTING_OK == format.ProcessEscape(line.c_str(), pos, length, true, false, &replaceto, &startFormat)) break;
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

void ParsedLine::Draw(XY xy, MscDrawer *mscd, bool isRotated) const
{
    StringFormat format(startFormat);
    size_t pos = 0;
    unsigned length;
    string replaceto;
    string fragment;

    xy.y += heightAboveBaseLine;

    while (line.length()>pos) {
        fragment.erase();
        //collect characters up until we hit a vaild formatting escape (or string end)
        while (line.length()>pos) {
            if (StringFormat::FORMATTING_OK == format.ProcessEscape(line.c_str(), pos, length, true, false, &replaceto, &startFormat)) break;
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
            if (StringFormat::LINE_BREAK == format.ProcessEscape(input.c_str(), pos, length)) break; //format not changed here!!!
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

void Label::DrawCovers(double sx, double dx, double y,
                       Geometry &cover, bool draw, bool isRotated) const
{
    if (size()==0) return;
    XY xy;
    xy.y = y + at(0).startFormat.textVGapAbove.second;
    for (unsigned i = 0; i<size(); i++) {
        const XY wh = at(i).getWidthHeight();
        switch (at(i).startFormat.ident.second) {
        case MSC_IDENT_LEFT:
        default:
            xy.x = sx+at(i).startFormat.textHGapPre.second; break;
        case MSC_IDENT_CENTER:
            xy.x = (sx + dx - wh.x + at(i).startFormat.textHGapPre.second -
                    at(i).startFormat.textHGapPost.second) / 2; break;
        case MSC_IDENT_RIGHT:
            xy.x = dx - wh.x - at(i).startFormat.textHGapPost.second; break;
        }
        //Draw line of text
        if (draw)
            at(i).Draw(xy, msc, isRotated);
        else
            cover += Block(xy, xy+wh);
        xy.y += wh.y + at(i).startFormat.spacingBelow.second +
            at(i).startFormat.textVGapLineSpacing.second;
    }
}



//////////////

//void StringFormat::ExtractCSH(int startpos, const char *text, Msc &msc)
//{
//    if (text==NULL) return;
//    int pos=0;
//    do {
//        while(text[pos] && text[pos]!='\\') pos++;
//        if (text[pos] == 0) return;
//        int len = 0;
//        MscColorSyntaxType color = COLOR_LABEL_ESCAPE;
//		if (strchr("-+^_biuBIU0123456789n\\#{}[];\"", text[pos+1])) {
//            len = 2;
//        } else switch (text[pos+1]) {
//        case 'p':
//            if (strchr("lrc", text[pos+2]))
//                len = 3;
//            break;
//        case 'm':
//            if (strchr("udlrins", text[pos+2]))
//                len = 1; //indicate next block an extra char
//            //fallthrough
//        case 'f':
//        case 'c':
//        case 's':
//            if (text[pos+len+2]!='(') {
//                len = 0; //'m' may have set this to 1.
//                break;
//            }
//            int l=pos+3+len;
//            while(text[l] && text[l]!=')') l++;
//            if (!text[l]) break;
//            len = l-pos+1;
//            break;
//        }
//        if (len) {
//            CshPos loc;
//            loc.first_pos = startpos+pos;
//            loc.last_pos = loc.first_pos+len-1;
//            msc.AddCSH(loc, color);
//            pos+=len;
//        } else
//            pos++;
//    } while (1);
//}
//
////Replaces style and color references to actual definitions found in msc->StyleSets.top()
////and ColorSets.top(). Also performs syntax error checking and generates errors/warnings
////escape contais the string to parse (and change)
////if label is true we are expanding a label, else a text.format attribute
////basic contains the baseline style and color
////(the one to return to at \s() and \c() and \f(), mX())
//#define PER_S_DEPRECATED_MSG "The use of '\\s' control escape to indicate small text is deprecated. Use '\\-' instead."
//#define MAX_ESCAPE_M_VALUE 500
//#define TOO_LARGE_M_VALUE_MSG  "Use an integer between [0..500]."
//void StringFormat::ExpandColorAndStyle(string &escape, Msc *msc, file_line linenum,
//									   const StringFormat *basic, bool label)
//{
//    int pos=0;
//
//    while(escape.length()>=pos+2) {
//        if (escape[pos] != '\\') {
//            pos++;
//            continue;
//        }
//        char e = escape[pos+1];
//        if (string("-+^_biuBIU0123456789n\\#{}[];\"").find(e) != string::npos) {
//            pos+=2;
//            continue;
//        }
//        bool was_m = false;
//        switch (e) {
//        case 'p':
//            if (string("lrc").find(escape[pos+2]) == string::npos) {
//                msc->Error.Warning(linenum, string("Escape '\\p' shall be continued with one of 'lrc'. ") +(label?"Keeping it as verbatim text.":"Ignoring it."));
//                if (label) pos += 2;
//                else escape.erase(pos, 2);
//            } else
//                pos += 3;
//            continue;
//        case 'm':
//            if (string("udlrins").find(escape[pos+2]) == string::npos) {
//                msc->Error.Warning(linenum, string("Escape '\\m' shall be continued with one of 'udlrins'. ") +(label?"Keeping it as verbatim text.":"Ignoring it."));
//                if (label) pos += 2;
//                else escape.erase(pos, 2);
//                continue;
//            }
//            was_m = true;
//            pos++; //m escapes are 2 char long, others below just one
//            e = escape[pos+1];
//            //Fallthrough
//        case 'f':
//        case 'c':
//        case 's':
//            break;
//        default:
//            if (label) {
//                string ee;
//                ee.push_back(e);
//                msc->Error.Warning(linenum, string("Unrecognized control escape '\\") + ee + (label?"'. Keeping it as verbatim text.":"'. Ignoring it."));
//                pos += 2;
//            } else
//                escape.erase(pos, 2);
//            continue;
//        }
//        //continue parsing after the "\c", \s, \f, \mX
//        pos += 2;
//        string esc = "\\";
//        if (was_m) esc.append("m");
//        esc += e;
//        //leave it as is, if no opening parenthesis
//        if (escape.length()<pos+1 || escape[pos]!='(') {
//            if (!was_m && e=='s') {
//                escape[pos-1] = '-';
//                msc->Error.Warning(linenum, "Missing style name after \\s control escape. Assuming small text switch.",
//                                   PER_S_DEPRECATED_MSG);
//            } else {
//                msc->Error.Warning(linenum, "Escape '" +esc+ "' requires a value in parenthesis. " +(label?"Keeping it as verbatim text.":"Ignoring it."));
//                if (!label) {
//                    pos -= 2 + was_m;
//                    escape.erase(pos, 2+was_m);
//                }
//            }
//            continue;
//        }
//        int pos2 = escape.find(")", pos);
//        //stop if no closing parenthesis
//        if (pos2==string::npos) {
//            string msg = "Missing ')' after '" + esc;
//            if (label) msg += "(' in label";
//            else msg += " in attribute 'format'";
//            msg += ". Ignoring after '" + esc + "('.";
//            msc->Error.Error(linenum, msg);
//            escape.erase(pos-2-was_m);
//            return;
//        }
//        string subst = escape.substr(pos+1,pos2-pos-1);
//        bool replace = false;
//        //subst now contains the text inside the parenthesis
//        if (was_m) {
//            if (subst.length()==0) {
//                const std::pair<bool, double> *p;
//                int modifer = 0;
//                switch (e) {
//                case 'u': p = &basic->textVGapAbove; modifer = +2; break;
//                case 'd': p = &basic->textVGapBelow; modifer = +2; break;
//                case 'l': p = &basic->textHGapPre;   modifer = +4; break;
//                case 'r': p = &basic->textHGapPost;  modifer = +2; break;
//                case 'i': p = &basic->textVGapLineSpacing; break;
//                case 'n': p = &basic->normalFontSize; break;
//                case 's': p = &basic->smallFontSize; break;
//                }
//                if (p && p->first) {
//                    modifer = p->second - modifer;
//                    if (modifer<0) modifer = 0;
//                    subst << "\\m" << e << "(" << modifer << ")";
//                } else
//                    subst.erase();
//                replace = true;
//            }
//            else {
//                int local_pos = 0;
//                double value = 0;
//                while (subst.length()>local_pos && subst[local_pos]>='0' && subst[local_pos]<='9') {
//                    value = value*10 + subst[local_pos]-'0';
//                    local_pos++;
//                }
//                if (subst.length()>local_pos) {
//                    string msg = "Invalid value to the '\\m";
//                    msg += e;
//                    msg.append("' control escape: '").append(subst).append("'.");
//                    if (value>MAX_ESCAPE_M_VALUE) {
//                        msg.append(" I could deduct '").append(subst.substr(0, local_pos));
//                        msg.append("', but that seems too large.");
//                        msg.append(" Ignoring control escape.");
//                        subst.erase();
//                        replace = true;
//                    } else {
//                        msg.append(" Using value '").append(subst.substr(0, local_pos));
//                        msg.append("' instead.");
//                        subst = string("\\m")+ e + "(" + subst.substr(0, local_pos) + ")";
//                        replace = true;
//                    }
//                    msc->Error.Error(linenum, msg, TOO_LARGE_M_VALUE_MSG);
//                } else if (value>MAX_ESCAPE_M_VALUE) {
//                    string msg = "Too large value after the '\\m";
//                    msg += e;
//                    msg.append("' control escape: '").append(subst).append("'.");
//                    msg.append(" Ignoring control escape.");
//                    subst.erase();
//                    replace = true;
//                    msc->Error.Error(linenum, msg, TOO_LARGE_M_VALUE_MSG);
//                }
//                //else: if value is a good number, keep replace = false and keep the number
//            }
//        } else if (e=='f') {
//            if (subst.length()==0) {
//                if (basic && basic->face.first)
//                    subst = "\\f(" + basic->face.second + ")";
//                replace = true; //if basic does not exist we remove this switch
//            } //else if there is a non-empty font name here, we keep it
//        } else if (e=='c') {
//            if (subst.length()==0) {
//                if (basic && basic->color.first)
//                    subst = "\\c" + basic->color.second.Print();
//                replace = true; //if basic does not exist we remove this switch
//            } else {
//                MscColorType color = msc->ColorSets.top().GetColor(subst);
//                if (color.valid)
//                    subst = "\\c" + color.Print();
//                else {
//                    //report error
//                    string msg="Unrecognized color name/definition: '";
//                    msg.append(subst).append("' in ");
//                    if (label) msg.append("label. Treating color definition as verbatim text.");
//                    else msg.append("attribute 'format'. Ignoring color change.");
//                    msc->Error.Error(linenum, msg, "");
//                    if (!label) subst.erase();
//                }
//                replace = true;
//            }
//        } else if (e=='s') {
//            if (subst.length()==0) {
//                if (basic)
//                    subst = basic->Print();
//                replace = true;
//            } else {
//                StyleSet::const_iterator i = msc->StyleSets.top().find(subst);
//                if (i==msc->StyleSets.top().end()) {
//                    //record erroneous style name
//                    string msg="Unrecognized style '";
//                    msg.append(subst).append("' in ");
//                    if (label) msg.append("label. Treating style name as small text in parenthesis.");
//                    else msg.append("attribute 'format'. Ignoring style.");
//                    msc->Error.Error(linenum, msg, label?PER_S_DEPRECATED_MSG:"");
//                    if (label) subst = "\\-(" + subst + ")";
//                    else subst.erase();
//                    replace = true;
//                } else {
//                    if (i->second.f_text)
//                        subst = i->second.text.Print();
//                    else
//                        subst.erase();
//                    replace = true;
//                }
//            }
//        }
//        //subst now contains a full escape sequence
//        if (replace) {
//            pos -= 2;
//            if (was_m) pos --;
//            escape.erase(pos,pos2-pos+1);
//            escape.insert(pos, subst);
//        } else
//            pos +=2; //we are not replacing, skip over the two parenthesis
//        pos += subst.length();
//    }
//}
//
////Parse the string as long as valid escape sequences come
//// -- apply their meaning
//// -- remove them from the string
////If you hit something not an escape char, stop
////if you hit a non-formatting escape, convert it to a real character and stop
////if you hit a \n, do nothing, leave it - it is not supposed to be here
////return true if there were format-changig escapes
//bool StringFormat::Apply(string &escape)
//{
//    /*
//     ** Recognize the following escape sequences
//     ** "\-" - switch to small font (also "\s")
//     ** "\+" - switch to normal font
//     ** "\^" - switch to superscript
//     ** "\_" - swucth to subscript
//     ** "\mX(num) - set text margings (X=_u_p, _d_own, _l_eft, _r_ight)
//     **             and font size (X=_s_mall, _n_ormal)
//     **             E.g. \mu7 sets upper margin to 7.
//     ** "\c(color) - set color, E.g., \c(0,0,0) is black
//     ** "\pX"- set paragraph ident to _c_enter, _l_eft, _r_ight
//     ** "\\" - an escaped "\"
//	 ** "\#| - an escaped "#"
//     ** "\0".."\9" - keep this much of line space after the line
//     */
//
//    bool ret = false;
//    int pos;
//    unsigned value;
//    MscColorType c;
//
//    while(escape.length()>=2 && escape[0] == '\\') {
//        switch (escape[1]) {
//        case 's':    // small font
//        case '-':    // small font
//        case '+':    // normal font
//        case '^':    // superscript
//        case '_':    // subscript
//            fontType.first = true;
//            fontType.second = string("+-^_s").find(escape[1]);
//            if (fontType.second == 4) fontType.second = 1;
//            escape.erase(0,2);
//            ret = true;
//            break;
//
//        case 'b':    // bold font
//        case 'B':
//            AddTristate(bold, std::pair<bool,tristate>(true, escape[1]=='B'?yes:invert));
//            escape.erase(0,2);
//            ret = true;
//            break;
//
//        case 'i':    // bold font
//        case 'I':
//            AddTristate(italics, std::pair<bool,tristate>(true, escape[1]=='I'?yes:invert));
//            escape.erase(0,2);
//            ret = true;
//            break;
//
//        case 'u':    // bold font
//        case 'U':
//            AddTristate(underline, std::pair<bool,tristate>(true, escape[1]=='U'?yes:invert));
//            escape.erase(0,2);
//            ret = true;
//            break;
//
//        case '0': //Line spacing mark
//        case '1': //Line spacing mark
//        case '2': //Line spacing mark
//        case '3': //Line spacing mark
//        case '4': //Line spacing mark
//        case '5': //Line spacing mark
//        case '6': //Line spacing mark
//        case '7': //Line spacing mark
//        case '8': //Line spacing mark
//        case '9': //Line spacing mark
//            spacingBelow.first = true;
//            spacingBelow.second = escape[1] - '0';
//            escape.erase(0,2);
//            ret = true;
//            break;
//
//        case 'c':
//            if (escape.length()<4) return ret;
//            if (escape[2]!='(') return ret;
//            pos = escape.find(")");
//            if (pos==string::npos) return ret;
//            c = MscColorType(escape.substr(3,pos-3));
//            if (c.valid) {
//                escape.erase(0,pos+1);
//                color.first = true;
//                color.second = c;
//                ret = true;
//                break;
//            }
//            return ret;
//
//        case 'f':
//            if (escape.length()<4) return ret;
//            if (escape[2]!='(') return ret;
//            pos = escape.find(")");
//            if (pos==string::npos) return ret;
//            face.first = true;
//            face.second = escape.substr(3,pos-3);
//            escape.erase(0,pos+1);
//            ret = true;
//            break;
//
//        case 'm':
//            if (escape.length()<5) return ret;
//            if (escape[3]!='(') return ret;
//            value = 0;
//            pos = 4;
//            while (escape.length()>pos && escape[pos]>='0' && escape[pos]<='9') {
//                value = value*10 + escape[pos]-'0';
//                pos++;
//            }
//            if (escape.length()==pos || escape[pos] != ')') return ret;
//            if (value<=MAX_ESCAPE_M_VALUE)
//                switch (escape[2]) {
//                case 'u': textVGapAbove.first = true; textVGapAbove.second = value+2; break;
//                case 'd': textVGapBelow.first = true; textVGapBelow.second = value+2; break;
//                case 'l': textHGapPre.first   = true; textHGapPre.second   = value+4; break;
//                case 'r': textHGapPost.first  = true; textHGapPost.second  = value+2; break;
//                case 'i': textVGapLineSpacing.first  = true; textVGapLineSpacing.second  = value; break;
//                case 'n': normalFontSize.first= true; normalFontSize.second= value; break;
//                case 's': smallFontSize.first = true; smallFontSize.second = value; break;
//                default: return ret;
//                }
//                escape.erase(0,pos+1);
//            ret = true;
//            break;
//
//        case 'p':
//            if (escape.length()<3) return ret;
//            switch (escape[2]) {
//            case 'c': ident.first = true; ident.second = MSC_IDENT_CENTER; break;
//            case 'l': ident.first = true; ident.second = MSC_IDENT_LEFT; break;
//            case 'r': ident.first = true; ident.second = MSC_IDENT_RIGHT; break;
//            default: return ret;
//            }
//            escape.erase(0,3);
//            ret = true;
//            break;
//
//        case 'n':           // enter: remove completely, should not appear anyway when splitting a parsedline into fragments
//            escape.erase(0,2);
//            ret = true;
//            break;
//
//        case '\\':          // escaped "\"
//        case '#':           // escaped "#"
//        case '{':           // escaped "{"
//        case '}':           // escaped "}"
//        case '[':           // escaped "["
//        case ']':           // escaped "]"
//        case ';':           // escaped ";"
//        case '\"':          // escaped quotation mark
//            escape.erase(0,1);  //Remove just the preceeding "\"
//        default:            // not an escaped char, leave it
//            return ret;    //return, there may have been real escapes before these
//        }
//    }
//    return ret;
//}
//
















