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

StringFormat::StringFormat(string &escape)
{
    Empty();
    Apply(escape);
}

StringFormat::StringFormat(const char *s)
{
    Empty();
    string escape(s);
    Apply(escape);
}

void StringFormat::ExtractCSH(int startpos, const char *text, Msc &msc)
{
    if (text==NULL) return;
    int pos=0;
    do {
        while(text[pos] && text[pos]!='\\') pos++;
        if (text[pos] == 0) return;
        int len = 0;
        MscColorSyntaxType color = COLOR_LABEL_ESCAPE;
		if (strchr("-+^_biuBIU0123456789n\\#", text[pos+1])) {
            len = 2;
        } else switch (text[pos+1]) {
        case 'p':
            if (strchr("lrc", text[pos+2]))
                len = 3;
            break;
        case 'm':
            if (strchr("udlrins", text[pos+2]))
                len = 1; //indicate next block an extra char
            //fallthrough
        case 'f':
        case 'c':
        case 's':
            if (text[pos+len+2]!='(') {
                len = 0; //'m' may have set this to 1.
                break;
            }
            int l=pos+3+len;
            while(text[l] && text[l]!=')') l++;
            if (!text[l]) break;
            len = l-pos+1;
            break;
        }
        if (len) {
            CshPos loc;
            loc.first_pos = startpos+pos;
            loc.last_pos = loc.first_pos+len-1;
            msc.AddCSH(loc, color);
            pos+=len;
        } else
            pos++;
    } while (1);
}

//Replaces style and color references to actual definitions found in msc->StyleSets.top()
//and ColorSets.top(). Also performs syntax error checking and generates errors/warnings
//escape contais the string to parse (and change)
//if label is true we are expanding a label, else a text.format attribute
//basic contains the baseline style and color 
//(the one to return to at \s() and \c() and \f(), mX())
#define PER_S_DEPRECATED_MSG "The use of '\\s' control escape to indicate small text is deprecated. Use '\\-' instead."
#define MAX_ESCAPE_M_VALUE 500
#define TOO_LARGE_M_VALUE_MSG  "Use an integer between [0..500]."
void StringFormat::ExpandColorAndStyle(string &escape, Msc *msc, file_line linenum,
									   const StringFormat *basic, bool label)
{
	int pos=0;

    while(escape.length()>=pos+2) {
        if (escape[pos] != '\\') {
            pos++;
            continue;
        }
        char e = escape[pos+1];
		if (string("-+^_biuBIU0123456789n\\#").find(e) != string::npos) {
            pos+=2;
            continue;
        }
		bool was_m = false;
        switch (e) {
		case 'p': 
            if (string("lrc").find(escape[pos+2]) == string::npos) {
				msc->Error.Warning(linenum, string("Escape '\\p' shall be continued with one of 'lrc'. ") +(label?"Keeping it as verbatim text.":"Ignoring it."));
				if (label) pos += 2;
				else escape.erase(pos, 2);
            } else
				pos += 3;
			continue;
        case 'm':
            if (string("udlrins").find(escape[pos+2]) == string::npos) {
				msc->Error.Warning(linenum, string("Escape '\\m' shall be continued with one of 'udlrins'. ") +(label?"Keeping it as verbatim text.":"Ignoring it."));
				if (label) pos += 2;
				else escape.erase(pos, 2);
                continue;
            }
			was_m = true;
			pos++; //m escapes are 2 char long, others below just one
			e = escape[pos+1];
			//Fallthrough
		case 'f':
		case 'c':
		case 's':
            break;
		default:
			if (label) {
				string ee;
				ee.push_back(e);
				msc->Error.Warning(linenum, string("Unrecognized control escape '\\") + ee + (label?"'. Keeping it as verbatim text.":"'. Ignoring it."));
	            pos += 2;
			} else
				escape.erase(pos, 2);
            continue;
        }
        //continue parsing after the "\c", \s, \f, \mX
        pos += 2;
		string esc = "\\";
		if (was_m) esc.append("m");
		esc += e;
        //leave it as is, if no opening parenthesis
		if (escape.length()<pos+1 || escape[pos]!='(') {
			if (!was_m && e=='s') {
				escape[pos-1] = '-';
				msc->Error.Warning(linenum, "Missing style name after \\s control escape. Assuming small text switch.",
				                   PER_S_DEPRECATED_MSG);
			} else {
				msc->Error.Warning(linenum, "Escape '" +esc+ "' requires a value in parenthesis. " +(label?"Keeping it as verbatim text.":"Ignoring it."));
				if (!label) {
					pos -= 2 + was_m;
					escape.erase(pos, 2+was_m);
				}
			}
			continue;
		}
        int pos2 = escape.find(")", pos);
        //stop if no closing parenthesis
		if (pos2==string::npos) {
			string msg = "Missing ')' after '" + esc;
			if (label) msg += "(' in label";
			else msg += " in attribute 'format'";
			msg += ". Ignoring after '" + esc + "('.";
			msc->Error.Error(linenum, msg);
			escape.erase(pos-2-was_m);
			return;
		}
        string subst = escape.substr(pos+1,pos2-pos-1);
		bool replace = false;
        //subst now contains the text inside the parenthesis
		if (was_m) {
			if (subst.length()==0) {
				const std::pair<bool, double> *p;
				int modifer = 0;
				switch (e) {
					case 'u': p = &basic->textVGapAbove; modifer = +2; break;
					case 'd': p = &basic->textVGapBelow; modifer = +2; break;
					case 'l': p = &basic->textHGapPre;   modifer = +4; break;
					case 'r': p = &basic->textHGapPost;  modifer = +2; break;
					case 'i': p = &basic->textVGapLineSpacing; break;
					case 'n': p = &basic->normalFontSize; break;
					case 's': p = &basic->smallFontSize; break;
				}
				if (p && p->first) {
					modifer = p->second - modifer;
					if (modifer<0) modifer = 0; 
					subst << "\\m" << e << "(" << modifer << ")";
				} else 
					subst.erase();
				replace = true;
			}
			else {
				int local_pos = 0;
				double value = 0;               
				while (subst.length()>local_pos && subst[local_pos]>='0' && subst[local_pos]<='9') {
					value = value*10 + subst[local_pos]-'0';
	                local_pos++;
				}
				if (subst.length()>local_pos) {
					string msg = "Invalid value to the '\\m";
					msg += e;
					msg.append("' control escape: '").append(subst).append("'.");
					if (value>MAX_ESCAPE_M_VALUE) {
						msg.append(" I could deduct '").append(subst.substr(0, local_pos));
						msg.append("', but that seems too large.");
						msg.append(" Ignoring control escape.");
						subst.erase();
						replace = true;
					} else {
						msg.append(" Using value '").append(subst.substr(0, local_pos));
						msg.append("' instead.");
						subst = string("\\m")+ e + "(" + subst.substr(0, local_pos) + ")";
						replace = true;
					}
					msc->Error.Error(linenum, msg, TOO_LARGE_M_VALUE_MSG);
				} else if (value>MAX_ESCAPE_M_VALUE) {
					string msg = "Too large value after the '\\m";
					msg += e;
					msg.append("' control escape: '").append(subst).append("'.");
					msg.append(" Ignoring control escape.");
					subst.erase();
					replace = true;
					msc->Error.Error(linenum, msg, TOO_LARGE_M_VALUE_MSG);
				}
					//else: if value is a good number, keep replace = false and keep the number
            }
		} else if (e=='f') {
            if (subst.length()==0) {
				if (basic && basic->face.first) 
					subst = "\\f(" + basic->face.second + ")";
				replace = true; //if basic does not exist we remove this switch
            } //else if there is a non-empty font name here, we keep it
		} else if (e=='c') {
            if (subst.length()==0) {
				if (basic && basic->color.first) 
                    subst = "\\c" + basic->color.second.Print();
				replace = true; //if basic does not exist we remove this switch
            } else {
				MscColorType color = msc->ColorSets.top().GetColor(subst);
                if (color.valid)
                    subst = "\\c" + color.Print();
                else {
                    //report error 
					string msg="Unrecognized color name/definition: '";
					msg.append(subst).append("' in ");
					if (label) msg.append("label. Treating color definition as verbatim text.");
					else msg.append("attribute 'format'. Ignoring color change.");
					msc->Error.Error(linenum, msg, "");
					if (!label) subst.erase();
                }
				replace = true;
            }
        } else if (e=='s') {
            if (subst.length()==0) {
                if (basic)
                    subst = basic->Print();
				replace = true;
            } else {
				StyleSet::const_iterator i = msc->StyleSets.top().find(subst);
                if (i==msc->StyleSets.top().end()) {
                    //record erroneous style name
					string msg="Unrecognized style '";
					msg.append(subst).append("' in ");
					if (label) msg.append("label. Treating style name as small text in parenthesis.");
					else msg.append("attribute 'format'. Ignoring style.");
					msc->Error.Error(linenum, msg, label?PER_S_DEPRECATED_MSG:"");
                    if (label) subst = "\\-(" + subst + ")";
                    else subst.erase();
					replace = true;
                } else {
                    if (i->second.f_text)
                        subst = i->second.text.Print();
                    else
                        subst.erase();
					replace = true;
                }
            }
        }
        //subst now contains a full escape sequence
		if (replace) {
	        pos -= 2;
			if (was_m) pos --;
	        escape.erase(pos,pos2-pos+1);
		    escape.insert(pos, subst);
		} else 
			pos +=2; //we are not replacing, skip over the two parenthesis
		pos += subst.length();
    }
}

bool StringFormat::Apply(string &escape)
{
    /*
     ** Recognize the following escape sequences
     ** "\-" - switch to small font (also "\s")
     ** "\+" - switch to normal font
     ** "\^" - switch to superscript
     ** "\_" - swucth to subscript
     ** "\mX(num) - set text margings (X=_u_p, _d_own, _l_eft, _r_ight)
     **             and font size (X=_s_mall, _n_ormal)
     **             E.g. \mu7 sets upper margin to 7.
     ** "\c(color) - set color, E.g., \c(0,0,0) is black
     ** "\pX"- set paragraph ident to _c_enter, _l_eft, _r_ight
     ** "\\" - an escaped "\"
	 ** "\#| - an escaped "#"
     ** "\0".."\9" - keep this much of line space after the line
     */

    bool ret = false;
    int pos;
    unsigned value;
    MscColorType c;

    while(escape.length()>=2 && escape[0] == '\\') {
        switch (escape[1]) {
        case 's':    // small font
        case '-':    // small font
        case '+':    // normal font
        case '^':    // superscript
        case '_':    // subscript
            fontType.first = true;
            fontType.second = string("+-^_s").find(escape[1]);
            if (fontType.second == 4) fontType.second = 1;
            escape.erase(0,2);
            ret = true;
            break;

        case 'b':    // bold font
        case 'B':
            AddTristate(bold, std::pair<bool,tristate>(true, escape[1]=='B'?yes:invert));
            escape.erase(0,2);
            ret = true;
            break;

        case 'i':    // bold font
        case 'I':
            AddTristate(italics, std::pair<bool,tristate>(true, escape[1]=='I'?yes:invert));
            escape.erase(0,2);
            ret = true;
            break;

        case 'u':    // bold font
        case 'U':
            AddTristate(underline, std::pair<bool,tristate>(true, escape[1]=='U'?yes:invert));
            escape.erase(0,2);
            ret = true;
            break;

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
            spacingBelow.first = true;
            spacingBelow.second = escape[1] - '0';
            escape.erase(0,2);
            ret = true;
            break;

        case 'c':
            if (escape.length()<4) return ret;
            if (escape[2]!='(') return ret;
            pos = escape.find(")");
            if (pos==string::npos) return ret;
            c = MscColorType(escape.substr(3,pos-3));
            if (c.valid) {
                escape.erase(0,pos+1);
                color.first = true;
                color.second = c;
                ret = true;
            }
            return ret;

        case 'f':
            if (escape.length()<4) return ret;
            if (escape[2]!='(') return ret;
            pos = escape.find(")");
            if (pos==string::npos) return ret;
			face.first = true;
			face.second = escape.substr(3,pos-3);
            escape.erase(0,pos+1);
            ret = true;
            break;

        case 'm':
            if (escape.length()<5) return ret;
            if (escape[3]!='(') return ret;
            value = 0;
            pos = 4;
            while (escape.length()>pos && escape[pos]>='0' && escape[pos]<='9') {
                value = value*10 + escape[pos]-'0';
                pos++;
            }
            if (escape.length()==pos || escape[pos] != ')') return ret;
			if (value<=MAX_ESCAPE_M_VALUE)
				switch (escape[2]) {
				case 'u': textVGapAbove.first = true; textVGapAbove.second = value+2; break;
				case 'd': textVGapBelow.first = true; textVGapBelow.second = value+2; break;
				case 'l': textHGapPre.first   = true; textHGapPre.second   = value+4; break;
				case 'r': textHGapPost.first  = true; textHGapPost.second  = value+2; break;
				case 'i': textVGapLineSpacing.first  = true; textVGapLineSpacing.second  = value; break;
				case 'n': normalFontSize.first= true; normalFontSize.second= value; break;
				case 's': smallFontSize.first = true; smallFontSize.second = value; break;
				default: return ret;
				}
            escape.erase(0,pos+1);
            ret = true;
            break;

        case 'p':
            if (escape.length()<3) return ret;
            switch (escape[2]) {
            case 'c': ident.first = true; ident.second = MSC_IDENT_CENTER; break;
            case 'l': ident.first = true; ident.second = MSC_IDENT_LEFT; break;
            case 'r': ident.first = true; ident.second = MSC_IDENT_RIGHT; break;
            default: return ret;
            }
            escape.erase(0,3);
            ret = true;
            break;

        case 'n':
        case '\\':          // escaped "\"
		case '#':           // escaped "#"
        default:            // not an escaped char
            return ret;
        }
    }
    return ret;
}

void StringFormat::SetColor(MscColorType c)
{
    color.first = true;
    color.second = c;
}

void StringFormat::Apply(const StringFormat *a)
{
    const StringFormat *toadd = dynamic_cast<const StringFormat*>(a);
    if (toadd)
        operator += (*toadd);
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

		StringFormat::ExpandColorAndStyle(tmp, msc, a.linenum_value.start, NULL, false);

        StringFormat sf(tmp);
        if (tmp.length()) {
            string s("Could not recognize these:'");
            s.append(tmp).append("' in attribute '").append(a.name).append("'.");
            s.append(" Applying only what I understood.");
            msc->Error.Warning(a, true, s);
            if (!msc->Error.strict) Apply(&sf);
            return true;
        }
        Apply(&sf);
        return true;
    }
    return false;

}

void StringFormat::AddNumbering(string &label, int num)
{
    if (label.length()==0 || num == -1000) return;
    string s = label;
    int len = s.length();
    while (len>=2) {
        int len2=len;
        //Consume escape char
        StringFormat dummy(s);
        len = s.length();
        if (len == len2) break;
    }
    string ss;
    ss << num << ": ";
    label.insert(label.length()-len, ss);
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
    if (text.length()==0) return 0;
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
	cairo_text_extents (mscd->GetContext(), "M N", &te);
    double  w = te.x_advance;
    cairo_text_extents (mscd->GetContext(), "MN", &te);
    return w - te.x_advance;
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

ParsedLine::ParsedLine(string s, MscDrawer *mscd, StringFormat &format)
{
    width = 0;
    heightAboveBaseLine = 0;
    heightBelowBaseLine = 0;
    format.Apply(s); //Apply initial formatting
    startFormat = format;
    line = s;
    size_t pos = 0;
    while (true) {
        pos = s.find("\\", pos);

        if (pos==string::npos || pos == s.length()-1) {
            // if no escape remained: no ''\\' or "\\" is last char
            width += format.getFragmentWidth(s, mscd);
            heightAboveBaseLine =
                std::max(heightAboveBaseLine, format.getFragmentHeightAboveBaseLine(s, mscd));
            heightBelowBaseLine =
                std::max(heightBelowBaseLine, format.getFragmentHeightBelowBaseLine(s, mscd));
            //Add spacing below. If there is spacingbelow, span a full height line
            if (heightAboveBaseLine == 0 && format.getSpacingBelow())
                heightAboveBaseLine = format.getFragmentHeightAboveBaseLine("M", mscd);
            return;
        }
        if (s[pos+1] == '\\') {       //escaped "\\"
            pos+=1;
            s.erase(pos,1);
            continue;
        }
        // Potential Escape found
        string t = s.substr(pos);
        StringFormat newFormat;
        newFormat.Empty();
        //Check if these were real escape characters
        if (!newFormat.Apply(t)) {
            pos+=2;
            continue;
        }
        //Update geometry using existing format
        s.erase(pos); //s now contains the fragment up to the new formatting point
        width += format.getFragmentWidth(s, mscd);
        heightAboveBaseLine =
            std::max(heightAboveBaseLine, format.getFragmentHeightAboveBaseLine(s, mscd));
        heightBelowBaseLine =
            std::max(heightBelowBaseLine, format.getFragmentHeightBelowBaseLine(s, mscd));

        format += newFormat;
        s = t;
        pos = 0;
    };
};

void ParsedLine::Draw(XY xy, MscDrawer *mscd, bool isRotated) const
{
    StringFormat format = startFormat;
    string s = line;
    size_t pos = 0;
    //y will be the base line
    xy.y += heightAboveBaseLine;

    while (true) {
        pos = s.find("\\", pos);

        if (pos==string::npos || pos == s.length()-1) {
            // if no escape remained: no ''\\' or "\\" is last char
            format.drawFragment(s, mscd, xy, isRotated);
            return;
        }
        if (s[pos+1] == '\\') {       //escaped "\\"
            pos+=1;
            s.erase(pos,1);
            continue;
        }
        // Potential Escape found
        string t = s.substr(pos);
        StringFormat newFormat;
        newFormat.Empty();
        //Check if these were real escape characters
        if (!newFormat.Apply(t)) {
            pos+=2;
            continue;
        }
        s.erase(pos); //s now contains the text up to the formating point
        xy.x += format.drawFragment(s, mscd, xy, isRotated);
        format += newFormat;
        s = t;
        pos = 0;
    };
}

//////////////////////////////////////////////////


/** Split a string to substrings based on '\n' delimiters.
 * Then parse the strings for escape control characters
 */
unsigned Label::AddText(const string &input, StringFormat format)
{
    size_t pos = 0, line_start = 0;
    while(true)
    {
        size_t pos1 = input.find("\\n", pos);
        size_t pos2 = input.find("\\\\", pos);
        //Avoid escaped '\\'
        if (pos2 < pos1) {
            pos = pos2+2;
            continue;
        }
        if (pos1==string::npos) {
            const ParsedLine line(input.substr(line_start), msc, format);
            if (line.getWidthHeight().y>0)
                push_back(line);
            return this->size();
        }
        const ParsedLine line(input.substr(line_start, pos1-line_start), msc, format);
        push_back(line);
        line_start = pos = pos1+2;
    }
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
                       Geometry &cover, bool draw, bool isRotated)
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








