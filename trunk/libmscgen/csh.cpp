/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011,2012 Zoltan Turanyi
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

#define _USE_MATH_DEFINES
#include <math.h>
#include <map>
#include <cstring>   //strdup, free
#include "csh.h"
#include "color.h"
#include "stringparse.h" //for extracting csh out of strings
#include "attribute.h"  //for CaseInsensitive compares
#include "style.h"  //for Design::Reset() to obtain forbidden style names
#include "msc.h"

using namespace std;

void CshErrorList::Add(CshPos &pos, const char *t)
{
    resize(size()+1);
    CshError &e = at(size()-1);
    e.first_pos=pos.first_pos;
    e.last_pos=pos.last_pos;
    e.color = COLOR_ERROR;
    if (t) e.text=t;
}
    
MscColorSyntaxAppearance MscCshAppearanceList[CSH_SCHEME_MAX][COLOR_MAX];
void MscInitializeCshAppearanceList(void)
{
    //shorter alias to make things easier to see below
    MscColorSyntaxAppearance (&l)[CSH_SCHEME_MAX][COLOR_MAX] = MscCshAppearanceList;

    //Set the mask to default: we set all parameters
    for (unsigned scheme=0; scheme<CSH_SCHEME_MAX; scheme++)
        for (unsigned i=0; i<COLOR_MAX; i++)
            l[scheme][i].mask = COLOR_FLAG_BOLD | COLOR_FLAG_ITALICS | COLOR_FLAG_UNDERLINE | COLOR_FLAG_COLOR;

    //CSH_SCHEME ==0 is the Minimal one
    l[0][COLOR_KEYWORD].           SetColor(  0,  0,  0); l[0][COLOR_KEYWORD].effects = COLOR_FLAG_BOLD;
    l[0][COLOR_KEYWORD_PARTIAL].   SetColor(  0,  0,  0); l[0][COLOR_KEYWORD_PARTIAL].effects = 0;
    l[0][COLOR_ATTRNAME].          SetColor(  0,  0,  0); l[0][COLOR_ATTRNAME].effects = COLOR_FLAG_BOLD;
    l[0][COLOR_ATTRNAME_PARTIAL].  SetColor(  0,  0,  0); l[0][COLOR_ATTRNAME_PARTIAL].effects = 0;
    l[0][COLOR_OPTIONNAME].        SetColor(  0,  0,  0); l[0][COLOR_OPTIONNAME].effects = COLOR_FLAG_BOLD;
    l[0][COLOR_OPTIONNAME_PARTIAL].SetColor(  0,  0,  0); l[0][COLOR_OPTIONNAME_PARTIAL].effects = 0;
    l[0][COLOR_EQUAL].             SetColor(  0,  0,  0); l[0][COLOR_EQUAL].effects = 0;
    l[0][COLOR_SEMICOLON].         SetColor(  0,  0,  0); l[0][COLOR_SEMICOLON].effects = 0;
    l[0][COLOR_COLON].             SetColor(  0,  0,  0); l[0][COLOR_COLON].effects = 0;
    l[0][COLOR_BRACE].             SetColor(  0,  0,  0); l[0][COLOR_BRACE].effects = 0;
    l[0][COLOR_BRACKET].           SetColor(  0,  0,  0); l[0][COLOR_BRACKET].effects = 0;
    l[0][COLOR_SYMBOL].            SetColor( 20, 20,  0); l[0][COLOR_SYMBOL].effects = COLOR_FLAG_BOLD;
    l[0][COLOR_DESIGNNAME].        SetColor( 50,  0,  0); l[0][COLOR_DESIGNNAME].effects = 0;
    l[0][COLOR_STYLENAME].         SetColor( 50,  0,  0); l[0][COLOR_STYLENAME].effects = 0;
    l[0][COLOR_COLORNAME].         SetColor( 50,  0,  0); l[0][COLOR_COLORNAME].effects = 0;
    l[0][COLOR_ENTITYNAME].        SetColor(  0, 50,  0); l[0][COLOR_ENTITYNAME].effects = COLOR_FLAG_BOLD;
    l[0][COLOR_ENTITYNAME_FIRST].  SetColor(  0,  0,  0); l[0][COLOR_ENTITYNAME_FIRST].effects = COLOR_FLAG_BOLD;
    l[0][COLOR_MARKERNAME].        SetColor(  0,  0, 50); l[0][COLOR_MARKERNAME].effects = COLOR_FLAG_BOLD;
    l[0][COLOR_MARKERNAME_PARTIAL].SetColor( 25, 25, 50); l[0][COLOR_MARKERNAME_PARTIAL].effects = COLOR_FLAG_BOLD;
    l[0][COLOR_ATTRVALUE].         SetColor(  0,  0,  0); l[0][COLOR_ATTRVALUE].effects = 0;
    l[0][COLOR_COLORDEF].          SetColor(  0,  0,  0); l[0][COLOR_COLORDEF].effects = 0;
    l[0][COLOR_LABEL_TEXT].        SetColor(  0,  0,  0); l[0][COLOR_LABEL_TEXT].effects = 0;
    l[0][COLOR_LABEL_ESCAPE].      SetColor(  0,  0,  0); l[0][COLOR_LABEL_ESCAPE].effects = COLOR_FLAG_BOLD;
    l[0][COLOR_COMMENT].           SetColor(100,100,100); l[0][COLOR_COMMENT].effects = COLOR_FLAG_ITALICS;
    //For errors we keep bold, italics and color settings, just underline
    l[0][COLOR_ERROR].mask = COLOR_FLAG_UNDERLINE;        l[0][COLOR_ERROR].effects = COLOR_FLAG_UNDERLINE;

    //CSH_SCHEME ==1 is the Standard one
    l[1][COLOR_KEYWORD].           SetColor(128,128,  0); l[1][COLOR_KEYWORD].effects = COLOR_FLAG_BOLD;
    l[1][COLOR_KEYWORD_PARTIAL].   SetColor(128,128,  0); l[1][COLOR_KEYWORD_PARTIAL].effects = 0;
    l[1][COLOR_ATTRNAME].          SetColor(128,128,  0); l[1][COLOR_ATTRNAME].effects = COLOR_FLAG_BOLD;
    l[1][COLOR_ATTRNAME_PARTIAL].  SetColor(128,128,  0); l[1][COLOR_ATTRNAME_PARTIAL].effects = 0;
    l[1][COLOR_OPTIONNAME].        SetColor(128,128,  0); l[1][COLOR_OPTIONNAME].effects = COLOR_FLAG_BOLD;
    l[1][COLOR_OPTIONNAME_PARTIAL].SetColor(128,128,  0); l[1][COLOR_OPTIONNAME_PARTIAL].effects = 0;
    l[1][COLOR_EQUAL].             SetColor(  0,  0,  0); l[1][COLOR_EQUAL].effects = 0;
    l[1][COLOR_SEMICOLON].         SetColor(  0,  0,  0); l[1][COLOR_SEMICOLON].effects = 0;
    l[1][COLOR_COLON].             SetColor(  0,  0,  0); l[1][COLOR_COLON].effects = 0;
    l[1][COLOR_BRACE].             SetColor(  0,  0,  0); l[1][COLOR_BRACE].effects = 0;
    l[1][COLOR_BRACKET].           SetColor(  0,  0,  0); l[1][COLOR_BRACKET].effects = 0;
    l[1][COLOR_SYMBOL].            SetColor(255,  0,  0); l[1][COLOR_SYMBOL].effects = COLOR_FLAG_BOLD;
    l[1][COLOR_DESIGNNAME].        SetColor(  0,  0,  0); l[1][COLOR_DESIGNNAME].effects = 0;
    l[1][COLOR_STYLENAME].         SetColor(  0,  0,  0); l[1][COLOR_STYLENAME].effects = 0;
    l[1][COLOR_COLORNAME].         SetColor(  0,  0,  0); l[1][COLOR_COLORNAME].effects = 0;
    l[1][COLOR_ENTITYNAME].        SetColor(200,  0,  0); l[1][COLOR_ENTITYNAME].effects = COLOR_FLAG_BOLD;
    l[1][COLOR_ENTITYNAME_FIRST].  SetColor(200,  0,  0); l[1][COLOR_ENTITYNAME_FIRST].effects = COLOR_FLAG_BOLD|COLOR_FLAG_UNDERLINE;
    l[1][COLOR_MARKERNAME].        SetColor(  0,200,  0); l[1][COLOR_MARKERNAME].effects = COLOR_FLAG_BOLD;
    l[1][COLOR_MARKERNAME_PARTIAL].SetColor( 50,200, 50); l[1][COLOR_MARKERNAME_PARTIAL].effects = COLOR_FLAG_BOLD;
    l[1][COLOR_ATTRVALUE].         SetColor(  0,  0,200); l[1][COLOR_ATTRVALUE].effects = 0;
    l[1][COLOR_COLORDEF].          SetColor(  0,  0,200); l[1][COLOR_COLORDEF].effects = 0;
    l[1][COLOR_LABEL_TEXT].        SetColor(  0,  0,  0); l[1][COLOR_LABEL_TEXT].effects = 0;
    l[1][COLOR_LABEL_ESCAPE].      SetColor(  0,150,  0); l[1][COLOR_LABEL_ESCAPE].effects = COLOR_FLAG_BOLD;
    l[1][COLOR_COMMENT].           SetColor(100,100,100); l[1][COLOR_COMMENT].effects = COLOR_FLAG_ITALICS;
    //For errors we keep bold, italics and color settings, just underline
    l[1][COLOR_ERROR].mask = COLOR_FLAG_UNDERLINE;        l[1][COLOR_ERROR].effects = COLOR_FLAG_UNDERLINE;

    //CSH_SCHEME ==2 is the Colorful one
    l[2][COLOR_KEYWORD].           SetColor(128,128,  0); l[2][COLOR_KEYWORD].effects = COLOR_FLAG_BOLD;
    l[2][COLOR_KEYWORD_PARTIAL].   SetColor(128,128,  0); l[2][COLOR_KEYWORD_PARTIAL].effects = 0;
    l[2][COLOR_ATTRNAME].          SetColor(128,128,  0); l[2][COLOR_ATTRNAME].effects = COLOR_FLAG_BOLD;
    l[2][COLOR_ATTRNAME_PARTIAL].  SetColor(128,128,  0); l[2][COLOR_ATTRNAME_PARTIAL].effects = 0;
    l[2][COLOR_OPTIONNAME].        SetColor(128,128,  0); l[2][COLOR_OPTIONNAME].effects = COLOR_FLAG_BOLD;
    l[2][COLOR_OPTIONNAME_PARTIAL].SetColor(128,128,  0); l[2][COLOR_OPTIONNAME_PARTIAL].effects = 0;
    l[2][COLOR_EQUAL].             SetColor(  0,  0,  0); l[2][COLOR_EQUAL].effects = 0;
    l[2][COLOR_SEMICOLON].         SetColor(  0,  0,  0); l[2][COLOR_SEMICOLON].effects = 0;
    l[2][COLOR_COLON].             SetColor(  0,  0,  0); l[2][COLOR_COLON].effects = 0;
    l[2][COLOR_BRACE].             SetColor(  0,  0,  0); l[2][COLOR_BRACE].effects = 0;
    l[2][COLOR_BRACKET].           SetColor(  0,  0,  0); l[2][COLOR_BRACKET].effects = 0;
    l[2][COLOR_SYMBOL].            SetColor(  0,128,128); l[2][COLOR_SYMBOL].effects = COLOR_FLAG_BOLD;
    l[2][COLOR_DESIGNNAME].        SetColor(128,  0,  0); l[2][COLOR_DESIGNNAME].effects = 0;
    l[2][COLOR_STYLENAME].         SetColor(128,  0,  0); l[2][COLOR_STYLENAME].effects = 0;
    l[2][COLOR_COLORNAME].         SetColor(128,  0,  0); l[2][COLOR_COLORNAME].effects = 0;
    l[2][COLOR_ENTITYNAME].        SetColor(200,  0,  0); l[2][COLOR_ENTITYNAME].effects = COLOR_FLAG_BOLD;
    l[2][COLOR_ENTITYNAME_FIRST].  SetColor(200,  0,  0); l[2][COLOR_ENTITYNAME_FIRST].effects = COLOR_FLAG_BOLD|COLOR_FLAG_UNDERLINE;
    l[2][COLOR_MARKERNAME].        SetColor(  0,255,  0); l[2][COLOR_MARKERNAME].effects = COLOR_FLAG_BOLD;
    l[2][COLOR_MARKERNAME_PARTIAL].SetColor( 50,255, 50); l[2][COLOR_MARKERNAME_PARTIAL].effects = COLOR_FLAG_BOLD;
    l[2][COLOR_ATTRVALUE].         SetColor(  0,  0,255); l[2][COLOR_ATTRVALUE].effects = 0;
    l[2][COLOR_COLORDEF].          SetColor(  0,  0,255); l[2][COLOR_COLORDEF].effects = 0;
    l[2][COLOR_LABEL_TEXT].        SetColor(  0,200,  0); l[2][COLOR_LABEL_TEXT].effects = 0;
    l[2][COLOR_LABEL_ESCAPE].      SetColor(255,  0,  0); l[2][COLOR_LABEL_ESCAPE].effects = COLOR_FLAG_BOLD;
    l[2][COLOR_COMMENT].           SetColor(100,100,100); l[2][COLOR_COMMENT].effects = COLOR_FLAG_ITALICS;
    //For errors we keep bold, italics and color settings, just underline
    l[2][COLOR_ERROR].mask = COLOR_FLAG_UNDERLINE;        l[2][COLOR_ERROR].effects = COLOR_FLAG_UNDERLINE;

    //CSH_SCHEME ==3 is the Error oriented one
    l[3][COLOR_KEYWORD].           SetColor(  0,  0,  0); l[3][COLOR_KEYWORD].effects = COLOR_FLAG_BOLD;
    l[3][COLOR_KEYWORD_PARTIAL].   SetColor(  0,  0,  0); l[1][COLOR_KEYWORD_PARTIAL].effects = 0;
    l[3][COLOR_ATTRNAME].          SetColor(  0,  0,  0); l[3][COLOR_ATTRNAME].effects = COLOR_FLAG_BOLD;
    l[3][COLOR_ATTRNAME_PARTIAL].  SetColor(  0,  0,  0); l[3][COLOR_ATTRNAME_PARTIAL].effects = 0;
    l[3][COLOR_OPTIONNAME].        SetColor(  0,  0,  0); l[3][COLOR_OPTIONNAME].effects = COLOR_FLAG_BOLD;
    l[3][COLOR_OPTIONNAME_PARTIAL].SetColor(  0,  0,  0); l[2][COLOR_OPTIONNAME_PARTIAL].effects = 0;
    l[3][COLOR_EQUAL].             SetColor(  0,  0,  0); l[3][COLOR_EQUAL].effects = 0;
    l[3][COLOR_SEMICOLON].         SetColor(  0,  0,  0); l[3][COLOR_SEMICOLON].effects = 0;
    l[3][COLOR_COLON].             SetColor(  0,  0,  0); l[3][COLOR_COLON].effects = 0;
    l[3][COLOR_BRACE].             SetColor(  0,  0,  0); l[3][COLOR_BRACE].effects = 0;
    l[3][COLOR_BRACKET].           SetColor(  0,  0,  0); l[3][COLOR_BRACKET].effects = 0;
    l[3][COLOR_SYMBOL].            SetColor( 20, 20,  0); l[3][COLOR_SYMBOL].effects = COLOR_FLAG_BOLD;
    l[3][COLOR_DESIGNNAME].        SetColor( 50,  0,  0); l[3][COLOR_DESIGNNAME].effects = 0;
    l[3][COLOR_STYLENAME].         SetColor( 50,  0,  0); l[3][COLOR_STYLENAME].effects = 0;
    l[3][COLOR_COLORNAME].         SetColor( 50,  0,  0); l[3][COLOR_COLORNAME].effects = 0;
    l[3][COLOR_ENTITYNAME].        SetColor(  0, 50,  0); l[3][COLOR_ENTITYNAME].effects = COLOR_FLAG_BOLD;
    l[3][COLOR_ENTITYNAME_FIRST].  SetColor(  0,  0,  0); l[3][COLOR_ENTITYNAME_FIRST].effects = COLOR_FLAG_BOLD|COLOR_FLAG_UNDERLINE;
    l[3][COLOR_MARKERNAME].        SetColor(  0,  0, 50); l[3][COLOR_MARKERNAME].effects = COLOR_FLAG_BOLD;
    l[3][COLOR_MARKERNAME_PARTIAL].SetColor( 25, 25, 50); l[3][COLOR_MARKERNAME_PARTIAL].effects = COLOR_FLAG_BOLD;
    l[3][COLOR_ATTRVALUE].         SetColor(  0,  0,  0); l[3][COLOR_ATTRVALUE].effects = 0;
    l[3][COLOR_COLORDEF].          SetColor(  0,  0,  0); l[3][COLOR_COLORDEF].effects = 0;
    l[3][COLOR_LABEL_TEXT].        SetColor(  0,  0,  0); l[3][COLOR_LABEL_TEXT].effects = 0;
    l[3][COLOR_LABEL_ESCAPE].      SetColor(  0,  0,  0); l[3][COLOR_LABEL_ESCAPE].effects = COLOR_FLAG_BOLD;
    l[3][COLOR_COMMENT].           SetColor(100,100,100); l[3][COLOR_COMMENT].effects = COLOR_FLAG_ITALICS;
    l[3][COLOR_ERROR].      SetColor(255,  0,  0);        l[3][COLOR_ERROR].effects = COLOR_FLAG_UNDERLINE;
}

struct CurrentState {
    unsigned effects;
    MscColorType color;
    CurrentState() : effects(0) {}
    void Apply(const MscColorSyntaxAppearance &appearance);
    string Print(bool fakeDash=true) const;
    bool operator == (const CurrentState &other) const
    {return effects == other.effects && color == other.color;}
};

void CurrentState::Apply(const MscColorSyntaxAppearance &appearance)
{
    effects &= !appearance.mask;
    effects |= appearance.effects & appearance.mask;
    if (appearance.mask & COLOR_FLAG_COLOR) {
        color.r = appearance.r;
        color.g = appearance.g;
        color.b = appearance.b;
        color.a = 255;
        color.valid = true;
    }
}

string CurrentState::Print(bool fakeDash) const
{
    //Insert \377 (octal 255) instead of a \ to distinguish between \s already there
    string ret = fakeDash?"\377B\377I\377U":"\\B\\I\\U";
    if (!(effects & COLOR_FLAG_BOLD)) ret += fakeDash?"\377b":"\\b";
    if (!(effects & COLOR_FLAG_ITALICS)) ret += fakeDash?"\377i":"\\i";
    if (!(effects & COLOR_FLAG_UNDERLINE)) ret += fakeDash?"\377u":"\\u";
    if (color.valid) ret += (fakeDash?"\377c":"\\c") + color.Print();
    return ret;
}

//Takes a chart description and makes it into a color syntax highlighted label
string Cshize(const char *input, unsigned len, const CshListType &cshList, unsigned cshStyle,
              const char *textformat)
{
    map<int, CurrentState> textState;

    //Set default to COLOR_NORMAL (we know this is fully specified, mask contails all COLOR_FLAG_*s)
    textState[-1].Apply(MscCshAppearanceList[cshStyle][COLOR_NORMAL]);

    for (auto i = cshList.begin(); i!=cshList.end(); i++) {
        auto j = textState.upper_bound(i->last_pos); //j points strictly *after* last_pos
        j--; //now j is either before or at last_pos
        CurrentState state2revert2 = j->second;
        // if there are state changes between (first_pos and j], apply appearance to them
        if (i->first_pos-1 < j->first) {
            j++;
            for (auto jj = textState.upper_bound(i->first_pos-1); jj!=j; jj++)
                jj->second.Apply(MscCshAppearanceList[cshStyle][i->color]);
        }
        //Apply the new appearance from first pos. First copy any previous state to a new state at first pos...
        j = textState.upper_bound(i->first_pos-1);
        j--;
        textState[i->first_pos-1] = j->second;
        //...then modify that with the appearance
        textState[i->first_pos-1].Apply(MscCshAppearanceList[cshStyle][i->color]);
        //Finally revert at the end to the original state
        textState[i->last_pos] = state2revert2;
    }
    //Remove initial state at -1. Re-insert it if there is no state at 0.
    if (textState.find(0) == textState.end())
        textState[0] = textState[-1];
    textState.erase(-1);

    unsigned offset = 0;
    string ret;
    ret.append(input, len);
    CurrentState lastState;
    string format;
    for (auto i = textState.begin(); i!=textState.end(); i++) {
        if (lastState == i->second) continue;
        if (i->first + offset > ret.length()) break; // color codes past the end of the string
        format = i->second.Print();
        ret.insert(i->first + offset, format);
        offset += (unsigned)format.length();
        lastState = i->second;
    }
    //replace "\n" to "\\n"
    string::size_type pos = ret.find("\n");
    while (pos != string::npos) {
        ret.replace(pos, 1, "\377n");
        pos = ret.find("\n", pos+2);
    }
    //add escape for {}[];#" if they are not yet escaped
    pos = ret.find_first_of("{}[];#\\\"");
    while (pos != string::npos) {
        string::size_type pos2 = pos;
        while (pos2>0 && ret[pos2-1]=='\\') pos2--;
        //if odd number of \s
        if ((pos-pos2+1)%2)
            ret.insert(pos, "\\");
        pos = ret.find_first_of("{}[];#\\\"", pos+2);
    }
    //replace octal 277 back to '\'
    for (int ipos = int(ret.length())-1; ipos>=0; ipos--)
        if (ret[ipos]=='\377') ret[ipos] = '\\';

    //make it a label msc-generator can draw
    string preamble = "hscale=auto;[_wide=yes]:\\pl";
    if (textformat) preamble += textformat;
    ret.insert(0, preamble);
    ret.append(";");
    return ret;
}

void CshHint::swap(CshHint &o) 
{
    decorated.swap(o.decorated); 
    std::swap(type, o.type);
    std::swap(selectable, o.selectable);
    std::swap(param, o.param);
    std::swap(callback, o.callback);
}


////////////////////////////////////////////////////////////////////
void Csh::AddCSH(CshPos&pos, MscColorSyntaxType i)
{
    if (pos.first_pos>pos.last_pos) return;
    CshEntry e;
    e.first_pos = pos.first_pos;
    e.last_pos = pos.last_pos;
    e.color = i;
    CshList.AddToBack(e);
}

void Csh::AddCSH_ErrorAfter(CshPos&pos, const char *text)
{
    CshPos pos2;
    pos2.first_pos = pos.last_pos;
    pos2.last_pos = pos.last_pos+1;
    CshErrors.Add(pos2, text);
}

void Csh::AddCSH_AttrValue(CshPos& pos, const char *value, const char *name)
{
    if (!name || CaseInsensitiveEqual(name, "label") ||
        CaseInsensitiveEqual(name, "text.format") ||
        CaseInsensitiveEqual(name, "numbering.format") ||
        CaseInsensitiveEqual(name, "numbering.append") ||
        CaseInsensitiveEqual(name, "numbering.pre") ||
        CaseInsensitiveEqual(name, "numbering.post")) {
        //This is a label or text.format
        AddCSH(pos, COLOR_LABEL_TEXT);
        //Add escape symbols
        //Note the trick here: we add +1 to the pos.
        //If the string has escapes, it must have been specified via 
        //a quoted string, but in this case "pos" points to the beginning
        //quotation mark, so we add one.
        //If there are no escapes ExtractCSH() does nothing, so the passed 
        //pos will not matter anyway.
        //For exactly this reason if "AddCSH_AttrValue" is called from
        //"AddCSH_ColonString" below, it is called with the colon position.
        StringFormat::ExtractCSH(pos.first_pos+1, value, *this);
    } else {
        // No match - regular attribute value
        AddCSH(pos, COLOR_ATTRVALUE);
    }
}

void Csh::AddCSH_ColonString(CshPos& pos, const char *value, bool processComments)
{
    CshPos colon = pos;
    colon.last_pos = colon.first_pos;
    AddCSH(colon, COLOR_COLON);
    //pos.first_pos++;
    char *copy = strdup(value);
    if (processComments) {
        char *p = copy;
        while (*p!=0) {
            //search for #
            while (*p!=0 && *p!='#') p++;
            if (!*p) break;
            //if we hit a # count the \s before
            unsigned count = 0;
            //string starts with colon, so we are limited by that
            while (*(p-1-count) == '\\') count++;
            //if even number then replace comment with spaces till end of line
            if (count%2 == 0) {
                CshPos comment;
                comment.first_pos = pos.first_pos + unsigned(p - copy)-1;
                while (*p!=0 && *p!=0x0d && *p!=0x0a) *(p++) = ' ';
                comment.last_pos = pos.first_pos + unsigned(p - copy);
                AddCSH(comment, COLOR_COMMENT);
            } else
                p++; //step over the escaped #

        }
    }
    AddCSH_AttrValue(pos, copy+1, NULL);
    free(copy);
}


static const char keyword_names[][ENUM_STRING_LEN] =
{"", "parallel", "block", "pipe", "nudge", "heading", "newpage", "defstyle",
"defcolor", "defdesign", "vertical", "mark", "show", "hide", "activate", "deactivate",
"bye", "hspace", "vspace", "symbol", "note", "comment", ""};

static const char opt_names[][ENUM_STRING_LEN] =
{"msc", "hscale", "compress", "numbering", "indicator", 
"numbering.pre", "numbering.post", "numbering.append", "numbering.format",
"pedantic", "background.color", "background.color2", "background.gradient", 
"text.color", "text.format", "text.ident", 
"text.font.face", "text.font.type", 
"text.bold", "text.italic", "text.underline", 
"text.gap.up", "text.gap.down", "text.gap.left", "text.gap.right",
"text.gap.spacing", "text.size.normal", "text.size.small", 
"lcomment.line.color", "lcomment.line.type", "lcomment.line.width", 
"rcomment.line.color", "rcomment.line.type", "rcomment.line.width",
"lcomment.fill.color", "lcomment.fill.color2", "lcomment.fill.gradient",
"rcomment.fill.color", "rcomment.fill.color2", "rcomment.fill.gradient",
"angle", ""};

static const char attr_names[][ENUM_STRING_LEN] =
{"draw_time", "compress", "color", "label", "refname", "number", "indicator", "collapsed", 
"pos", "relative", "show", "active", "makeroom", "side", "offset", "solid",
"text.color", "text.ident", "ident", "text.format",
"text.font.face", "text.font.type", 
"text.bold", "text.italic", "text.underline", 
"text.gap.up", "text.gap.down", "text.gap.left", "text.gap.right",
"text.gap.spacing", "text.size.normal", "text.size.small",
"arrow", "arrowsize", "arrow.size", "arrow.type", "arrow.starttype", "arrow.midtype",
"arrow.endtype", "arrow.color", "arrow.xmul", "arrow.ymul",
"line.color", "line.type", "line.width", "line.corner", "line.radius", 
"vline.color", "vline.type", "vline.width", "vline.radius", "vline.corner",
"fill.color", "fill.color2", "fill.gradient", 
"vfill.color", "vfill.color2", "vfill.gradient",
"shadow.color", "shadow.offset", "shadow.blur", 
"compressable", "xsize", "ysize", "size", "space", "angle",
"note.pointer", "note.pos", ""};

static const char symbol_names[][ENUM_STRING_LEN] =
{"arc", "rectangle", "...", ""};

static const char extvxpos_designator_names[][ENUM_STRING_LEN] =
{"left", "right", "center", ""};


unsigned find_opt_attr_name(const char *name, const char array[][ENUM_STRING_LEN])
{
    for (unsigned i=0; array[i][0]; i++)
        switch (CaseInsensitiveBeginsWith(array[i], name)) {
        case 1: return 1;
        case 2: return 2;
        }
    return 0;
}

//This is called when a string is at the beginning of the line and is not part
//of a valid option: it can either be a command or an entity definition
// we give KEYWORD or KEYWORD_PARTIAL for full or partial keyword matches
// and ENTITYNAME or ENTITYNAME_FIRST for no matches
// optionnames are not searched
//All-in-all partial matches are only given if the cursor is just after the
//string in question. In this case we also store the partial match in
// Csh::partial_at_cursor_pos
void Csh::AddCSH_KeywordOrEntity(CshPos&pos, const char *name)
{
    MscColorSyntaxType type = COLOR_KEYWORD;
    unsigned match_result = find_opt_attr_name(name, keyword_names);
    unsigned match_result_options = find_opt_attr_name(name, opt_names);
    //If options fit better, we switch to them
    if (match_result_options > match_result) {
        type = COLOR_OPTIONNAME;
        match_result = match_result_options;
    }
    //Full match
    if (match_result == 2) {
        AddCSH(pos, type);
        return;
    }
    //Partial match but currently typing...
    if (pos.last_pos == cursor_pos && match_result == 1) {
        AddCSH(pos, MscColorSyntaxType(type+1));
        partial_at_cursor_pos.first_pos = pos.first_pos;
        partial_at_cursor_pos.last_pos = pos.last_pos;
        if (EntityNames.find(string(name)) == EntityNames.end())
            partial_at_cursor_pos.color = COLOR_ENTITYNAME_FIRST;
        else
            partial_at_cursor_pos.color = COLOR_ENTITYNAME;
        was_partial = true;
        return;
    }
    //if no keyword or option match, we assume an entityname
    AddCSH_EntityName(pos, name);
    return;
}

void Csh::AddCSH_AttrName(CshPos&pos, const char *name, MscColorSyntaxType color)
{
    const char (*array)[ENUM_STRING_LEN];
    if (color == COLOR_OPTIONNAME) array = opt_names;
    if (color == COLOR_ATTRNAME) array = attr_names;
    unsigned match_result = find_opt_attr_name(name, array);
    //Honor partial matches only if cursor is right after
    if (pos.last_pos != cursor_pos && match_result == 1)
        match_result = 0;
    switch (match_result) {
    case 2: AddCSH(pos, color); return;
    case 0: AddCSH(pos, COLOR_ERROR); return;
    case 1:
        AddCSH(pos, MscColorSyntaxType(color+1));
        partial_at_cursor_pos.first_pos = pos.first_pos;
        partial_at_cursor_pos.last_pos = pos.last_pos;
        partial_at_cursor_pos.color = COLOR_NORMAL;
        was_partial = true;
    }
}

//This is called when a string is at the beginning of where an attribute
//is expected and there is no '=' following.
//It can either be a to-be typed attribute or a style name
// we give ATTRNAME or ATTRNAME_PARTIAL for full or partial attr name matches
// and STYLE for no matched
//All-in-all partial matches are only given if the cursor is just after the
//string in question. In this case we also store the partial match in
// Csh::partial_at_cursor_pos
void Csh::AddCSH_StyleOrAttrName(CshPos&pos, const char *name)
{
    unsigned match_result = find_opt_attr_name(name, attr_names);
    if (pos.last_pos == cursor_pos && match_result == 1) {
        AddCSH(pos, COLOR_ATTRNAME_PARTIAL);
        partial_at_cursor_pos.first_pos = pos.first_pos;
        partial_at_cursor_pos.last_pos = pos.last_pos;
        partial_at_cursor_pos.color = COLOR_STYLENAME;
        was_partial = true;
        return;
    }
    if (match_result == 2) {
        AddCSH(pos, COLOR_ATTRNAME);
        return;
    }
    //if no keyword match, we assume an entityname
    AddCSH(pos, COLOR_STYLENAME);
    return;
}

void Csh::AddCSH_EntityName(CshPos&pos, const char *name)
{
    if (EntityNames.find(string(name)) != EntityNames.end()) {
        AddCSH(pos, COLOR_ENTITYNAME);
        return;
    }
    //We know that there has been no such entity yet
    //If we are currently typing it, use normal color, else
    //the one designated for first use of entities
    //In both cases insert to entity name database
    EntityNames.insert(string(name));
    if (pos.last_pos != cursor_pos) {
        AddCSH(pos, COLOR_ENTITYNAME_FIRST);
        return;
    }
    partial_at_cursor_pos.first_pos = pos.first_pos;
    partial_at_cursor_pos.last_pos = pos.last_pos;
    partial_at_cursor_pos.color = COLOR_ENTITYNAME_FIRST;
    was_partial = true;
}

void Csh::AddCSH_EntityOrMarkerName(CshPos&pos, const char *name)
{
    if (EntityNames.find(string(name)) != EntityNames.end()) 
        AddCSH(pos, COLOR_ENTITYNAME);
    else
        AddCSH(pos, COLOR_MARKERNAME);
}


//This is called when a string is after the keyword "symbol"
// we give KEYWORD or KEYWORD_PARTIAL for full or partial matches
// and STYLE for no matched
//All-in-all partial matches are only given if the cursor is just after the
//string in question. In this case we also store the partial match in
// Csh::partial_at_cursor_pos
void Csh::AddCSH_SymbolName(CshPos&pos, const char *name)
{
    unsigned match_result = find_opt_attr_name(name, symbol_names);
    if (pos.last_pos == cursor_pos && match_result == 1) {
        AddCSH(pos, COLOR_KEYWORD_PARTIAL);
        partial_at_cursor_pos.first_pos = pos.first_pos;
        partial_at_cursor_pos.last_pos = pos.last_pos;
        partial_at_cursor_pos.color = COLOR_KEYWORD;
        was_partial = true;
        return;
    }
    if (match_result == 2) {
        AddCSH(pos, COLOR_KEYWORD);
        return;
    }
    //if no keyword match, we assume an entityname
}

void Csh::AddCSH_ExtvxposDesignatorName(CshPos&pos, const char *name)
{
    unsigned match_result = find_opt_attr_name(name, extvxpos_designator_names);
    if (pos.last_pos == cursor_pos && match_result == 1) {
        AddCSH(pos, COLOR_KEYWORD_PARTIAL);
        partial_at_cursor_pos.first_pos = pos.first_pos;
        partial_at_cursor_pos.last_pos = pos.last_pos;
        partial_at_cursor_pos.color = COLOR_KEYWORD;
        was_partial = true;
        return;
    }
    if (match_result == 2) {
        AddCSH(pos, COLOR_KEYWORD);
        return;
    }
    //if no keyword match, we assume an entityname
}


bool CshHintGraphicCallbackForMarkers(MscCanvas *canvas, CshHintGraphicParam /*p*/)
{
    if (!canvas) return false;
    MscLineAttr line(LINE_SOLID, MscColorType(64,0,255), 1, CORNER_NONE, 0);
    MscFillAttr fill(MscColorType(64,0,255).Lighter(0.2), GRADIENT_UP);
    MscShadowAttr shadow(MscColorType(0,0,0));
    shadow.offset.first=true;
    shadow.offset.second=3;
    shadow.blur.first=true;
    shadow.blur.second=3;

    Contour c(XY(int(HINT_GRAPHIC_SIZE_X*0.3), int(HINT_GRAPHIC_SIZE_Y*0.2)), 
              XY(int(HINT_GRAPHIC_SIZE_X*0.3), int(HINT_GRAPHIC_SIZE_Y*0.8)),
              XY(int(HINT_GRAPHIC_SIZE_X*0.8), int(HINT_GRAPHIC_SIZE_Y*0.5)));
    canvas->Shadow(c, shadow);
    canvas->Fill(c, fill);
    canvas->Line(c, line);
    return true;
}

void Csh::ParseText(const char *input, unsigned len, int cursor_p, unsigned scheme)
{
    //initialize data struct
    cursor_pos = cursor_p;
    cshScheme = scheme;
    CshList.clear();
    EntityNames.clear();
    MarkerNames.clear();
    Contexts.clear();
    if (!ForcedDesign.empty() && Designs.find(ForcedDesign) != Designs.end())
        Contexts.push_back(Designs[ForcedDesign]);
    else
        PushContext(true);
    hintStatus = HINT_NONE;
    //Positions returned by yacc contain the first and last char of range
    //so for a 1 char long selection first_pos=last_pos
    //Also for yacc the first character has index of 1.
    //To express a 0 long selection we set last = first -1
    hintedStringPos.first_pos = cursor_p+1;
    hintedStringPos.last_pos  = cursor_p;
    hintAttrName.clear();
    Hints.clear();

    CshParse(*this, input, len);
    if (addMarkersAtEnd) {
        hintStatus = HINT_FILLING;
        for (auto i=MarkerNames.begin(); i!=MarkerNames.end(); i++)
            AddToHints(CshHint(HintPrefix(COLOR_ENTITYNAME) + *i, HINT_ATTR_VALUE, true, CshHintGraphicCallbackForMarkers));
        hintStatus = HINT_READY;
    }
    if (hintStatus == HINT_FILLING) hintStatus = HINT_READY;
    //Take one from first, since RichEditCtrel works that way
    --hintedStringPos.first_pos;
    _ASSERT(hintStatus==HINT_READY || Hints.size()==0);
    if (hintStatus!=HINT_READY || Hints.size()==0)
        return;
    //Find the plain text for all hints <-- What was this??
}

MscColorSyntaxType Csh::GetCshAt(int pos)
{
    //Search labels backwards
    for (auto i = CshList.rbegin(); !(i==CshList.rend()); i++)
        if (i->first_pos<=pos && i->last_pos>=pos) return i->color;
    return COLOR_NORMAL;
}

void CshContext::SetPlain()
{
    Design plain;
    plain.Reset();
    for (auto i=plain.colors.begin(); i!=plain.colors.end(); i++)
        Colors[i->first] = i->second;
    for (auto i=plain.styles.begin(); i!=plain.styles.end(); i++)
        StyleNames.insert(i->first);
}

Csh::Csh() : was_partial(false), hintStatus(HINT_NONE), addMarkersAtEnd(false), cursor_pos(-1)
{
    Design plain;
    plain.Reset();
    for (auto i=plain.styles.begin(); i!=plain.styles.end(); i++)
        ForbiddenStyles.insert(i->first);
    ForbiddenStyles.erase("weak");
    ForbiddenStyles.erase("strong");
    PushContext(true);
}

void Csh::PushContext(bool empty)
{
    if (empty){
        Contexts.push_back(CshContext());
        Contexts.back().SetPlain();
    } else {
        Contexts.push_back(Contexts.back());
    }
}

bool Csh::SetDesignTo(const std::string&design)
{
    auto i = Designs.find(design);
    if (i==Designs.end()) return false;
    Contexts.back() += i->second;
    return true;
}

CshCursorRelPosType Csh::CursorIn(int a, int b) const
{
    //cursor_pos is in CRichEdit context and is zero based
    //a and b are yacc contex based, are one-based
    //and a==b means a one long selection (whereas the cursor is zero length)
    if (cursor_pos+1<a) return CURSOR_BEFORE;
    if (cursor_pos+1==a) return CURSOR_AT_BEGINNING;
    if (cursor_pos<b) return CURSOR_IN;
    if (cursor_pos==b) return CURSOR_AT_END;
    return CURSOR_AFTER;
}

//Checks if the cursor is between the two ranges and if so, it applies
//the hinttype with LOCATED. It sets hintsForcedOnly to false
//If the cursor is immediately at the beginning of the second range we do nothing
bool Csh::CheckHintBetween(const CshPos &one, const CshPos &two, CshHintType ht, const char *a_name)
{
    switch (CursorIn(one.last_pos+1, two.first_pos-1)) {
    case CURSOR_AT_BEGINNING:
    case CURSOR_IN:
        hintStatus = HINT_LOCATED;
        hintsForcedOnly = false;
        hintType = ht;
        if (ht==HINT_MARKER)
            addMarkersAtEnd = true;
        hintAttrName = a_name?a_name:"";
        return true;
    default:
        return false;
    }
}

//Checks if the cursor is between the two ranges (but one char after the firts) and if so, it applies
//the hinttype with LOCATED. It sets hintsForcedOnly to false
//If the cursor is immediately at the beginning of the second range we do nothing
bool Csh::CheckHintBetweenPlusOne(const CshPos &one, const CshPos &two, CshHintType ht, const char *a_name)
{
    if  (CursorIn(one.last_pos+1, two.first_pos-1) == CURSOR_IN) {
        hintStatus = HINT_LOCATED;
        hintsForcedOnly = false;
        hintType = ht;
        if (ht==HINT_MARKER)
            addMarkersAtEnd = true;
        hintAttrName = a_name?a_name:"";
        return true;
    }
    return false;
}
//Checks if the cursor is between one and lookahead or if atEnd==true then right after one
//if so then set the hinttype with LOCATED. It sets hintsForcedOnly to false
//If the cursor is immediately at the beginning of the second range we do nothing
bool Csh::CheckHintAfter(const CshPos &one, const CshPos &lookahead, bool atEnd, CshHintType ht, const char *a_name)
{
    if (atEnd) {
        switch (CursorIn(one)) {
        case CURSOR_BEFORE:
        case CURSOR_AT_BEGINNING:
        case CURSOR_IN:
            return false;
        default:
            break;
        }
    } else switch (CursorIn(one.last_pos+1, lookahead.first_pos-1)) {
        case CURSOR_AFTER:
        case CURSOR_BEFORE:
        case CURSOR_AT_END:
            return false;
        default:
            break;
        }
    hintStatus = HINT_LOCATED;
    hintsForcedOnly = false;
    hintType = ht;
    if (ht==HINT_MARKER)
        addMarkersAtEnd = true;
    hintAttrName = a_name?a_name:"";
    return true;
}

//Checks if the cursor is between one char beyond "one" and "lookahead" or 
//if atEnd==true then right after one char beyond "one"
//if so then set the hinttype with LOCATED. It sets hintsForcedOnly to false
//If the cursor is immediately at the beginning of the second range we do nothing
bool Csh::CheckHintAfterPlusOne(const CshPos &one, const CshPos &lookahead, bool atEnd, CshHintType ht, const char *a_name)
{
    if (one.last_pos >= lookahead.first_pos) return false;
    CshPos one_oneAfter = one;
    one_oneAfter.last_pos++;
    return CheckHintAfter(one_oneAfter, lookahead, atEnd, ht, a_name);
}

//Check specifically for entity hints and if true, add entities to hints & set to HINT_READY
bool Csh::CheckEntityHintAtAndBefore(const CshPos &one, const CshPos &two)
{
    if (!CheckHintAtAndBefore(one, two, HINT_ENTITY)) return false;
    AddEntitiesToHints();
    hintStatus = HINT_READY;
    return true;
}

//Check specifically for entity hints and if true, add entities to hints & set to HINT_READY
//Does not result in hints if cursor is exactly after "one"
bool Csh::CheckEntityHintAtAndBeforePlusOne(const CshPos &one, const CshPos &two)
{
    if (one.last_pos >= two.first_pos) return false;
    CshPos one_oneAfter = one;
    one_oneAfter.last_pos++;
    return CheckEntityHintAtAndBefore(one_oneAfter, two);
}

bool Csh::CheckEntityHintAt(const CshPos &one)
{
    if (!CheckHintAt(one, HINT_ENTITY)) return false;
    AddEntitiesToHints();
    hintStatus = HINT_READY;
    return true;
}

bool Csh::CheckEntityHintAfter(const CshPos &one, const CshPos &lookahead, bool atEnd)
{
    if (!CheckHintAfter(one, lookahead, atEnd, HINT_ENTITY)) return false;
    AddEntitiesToHints();
    hintStatus = HINT_READY;
    return true;
}


bool Csh::CheckEntityHintAfterPlusOne(const CshPos &one, const CshPos &lookahead, bool atEnd)
{
    if (!CheckHintAfterPlusOne(one, lookahead, atEnd, HINT_ENTITY)) return false;
    AddEntitiesToHints();
    hintStatus = HINT_READY;
    return true;
}

//Checks if the cursor is between one and two or inside two
//If so, it sets status to HINT_LOCATED. If cursor is inside two, hintedStringPos
//is set to two. hintsForcedOnly is set to true if the cursor is truely before two
//or is at the end of two
bool Csh::CheckHintAtAndBefore(const CshPos &one, const CshPos &two, CshHintType ht, const char *a_name)
{
    if (CursorIn(one.last_pos+1, two.last_pos)<=CURSOR_AFTER) return false;
    hintStatus = HINT_LOCATED;
    hintsForcedOnly = false;
    hintType = ht;
    if (ht==HINT_MARKER)
        addMarkersAtEnd = true;
    hintAttrName = a_name?a_name:"";
    CshCursorRelPosType in_two = CursorIn(two);
    if (in_two==CURSOR_AT_END || in_two==CURSOR_BEFORE)
        hintsForcedOnly = true;
    if (in_two>=CURSOR_AT_BEGINNING)
        hintedStringPos = two;
    return true;
}

bool Csh::CheckHintAtAndBeforePlusOne(const CshPos &one, const CshPos &two, CshHintType ht, const char *a_name)
{
    CshPos one_oneAfter = one;
    one_oneAfter.last_pos++;
    return CheckHintAtAndBefore(one_oneAfter, two, ht, a_name);
}


//Checks if the cursor is between one and two or inside two
//If so, it sets status to HINT_LOCATED. If cursor is inside two, hintedStringPos
//is set to two. hintsForcedOnly is set to true if the cursor is truely before two
//or is at the end of two
bool Csh::CheckHintAt(const CshPos &one, CshHintType ht, const char *a_name)
{
    if (CursorIn(one)<=CURSOR_AFTER) return false;
    hintStatus = HINT_LOCATED;
    hintsForcedOnly = false;
    hintType = ht;
    if (ht==HINT_MARKER)
        addMarkersAtEnd = true;
    hintAttrName = a_name?a_name:"";
    hintedStringPos = one;
    return true;
}

//If the hint had been located, is in the "location_to_check" and equals to "ht", 
//we set it ready and return true
bool Csh::CheckHintLocated(CshHintType ht, const CshPos &location_to_check)
{
    if (hintStatus!=HINT_LOCATED || hintType!=ht)
        return false;
    //If hintedString is fully inside the location_to_check only then do we signal a located hint.
    //If the hinted string is empty then hintedStringPos.first equals hintedStringPos.last+1.
    //In this case if the last is within (perhaps at the end) of "location_to_check", we are
    //still OK and shall give hints.
    if (!location_to_check.IsWithin(hintedStringPos) && 
        (hintedStringPos.first_pos<=hintedStringPos.last_pos || !location_to_check.IsWithin(hintedStringPos.last_pos)))
        return false;
    hintStatus = HINT_FILLING;
    return true;
}

std::string Csh::HintPrefix(MscColorSyntaxType t) const
{
    CurrentState state;
    state.Apply(MscCshAppearanceList[cshScheme][t]);
    return state.Print(false);
}


bool CshHintGraphicCallbackForAttributeNames(MscCanvas *canvas, CshHintGraphicParam /*p*/)
{
    if (!canvas) return false;
    const double w = 0.4*HINT_GRAPHIC_SIZE_X;
    const double h = 0.08*HINT_GRAPHIC_SIZE_Y;
    const double off = 0.35*HINT_GRAPHIC_SIZE_Y;
    MscColorType color(0, 0, 0);
    MscLineAttr line;
    line.radius.second = 3;
    MscFillAttr fill(color, GRADIENT_NONE);
    canvas->Fill(XY((HINT_GRAPHIC_SIZE_X-w)/2, off), XY((HINT_GRAPHIC_SIZE_X+w)/2, off+h), line, fill);
    canvas->Fill(XY((HINT_GRAPHIC_SIZE_X-w)/2, HINT_GRAPHIC_SIZE_Y-off-h), XY((HINT_GRAPHIC_SIZE_X+w)/2, HINT_GRAPHIC_SIZE_Y-off), line, fill);
    return true;
}

void Csh::AddToHints(CshHint &&h) 
{
    if (hintStatus == HINT_READY) return; //we add no more
    if (h.callback==NULL && h.type == HINT_ATTR_NAME) {
        h.callback = CshHintGraphicCallbackForAttributeNames;
        h.param = 0; 
    }
    Hints.insert(std::move(h));
}

void Csh::AddToHints(const char names[][ENUM_STRING_LEN], const string &prefix, CshHintType t, 
                     CshHintGraphicCallback c)
{
    //index==0 is usually "invalid"
    for (unsigned i=1; names[i][0]; i++)
        AddToHints(CshHint(prefix+names[i], t, true, c, CshHintGraphicParam(i)));
}

void Csh::AddToHints(const char names[][ENUM_STRING_LEN], const string &prefix, CshHintType t, 
                     CshHintGraphicCallback c, CshHintGraphicParam p)
{
    //index==0 is usually "invalid"
    for (unsigned i=1; names[i][0]; i++)
        AddToHints(CshHint(prefix+names[i], t, true, c, p));
}

bool CshHintGraphicCallbackForColors(MscCanvas *canvas, CshHintGraphicParam p)
{
    if (!canvas) return false;
    const int size = HINT_GRAPHIC_SIZE_Y-3;
    const int off_x = (HINT_GRAPHIC_SIZE_X - size)/2;
    const int off_y = 1;
    MscColorType color(p);
    Block b(XY(off_x, off_y), XY(off_x+size, off_y+size));
    b.Round();
    if (color.a<255) {
        MscFillAttr fill(MscColorType(255,255,255), GRADIENT_NONE);
        canvas->Fill(b.Centroid(), b.UpperLeft(), fill);
        canvas->Fill(b.Centroid(), b.LowerRight(), fill);
        fill.color.second = MscColorType(196,196,196);
        canvas->Fill(b.Centroid(), b.UpperRight(), fill);
        canvas->Fill(b.Centroid(), b.LowerLeft(), fill);
    }
    canvas->Fill(b, MscFillAttr(color, GRADIENT_NONE));
    b.Expand(0.5);
    canvas->Line(b, MscLineAttr(LINE_SOLID, MscColorType(0,0,0), 1, CORNER_NONE, 0));
    return true;
}

void Csh::AddColorValuesToHints()
{
    AddToHints(CshHint(HintPrefixNonSelectable()+"<\"red,green,blue\">", HINT_ATTR_VALUE, false));
    AddToHints(CshHint(HintPrefixNonSelectable()+"<\"red,green,blue,opacity\">", HINT_ATTR_VALUE, false));
    AddToHints(CshHint(HintPrefixNonSelectable()+"<\"color name,opacity\">", HINT_ATTR_VALUE, false));
    AddToHints(CshHint(HintPrefixNonSelectable()+"<\"color name+-brightness%\">", HINT_ATTR_VALUE, false));
    CshHint hint("", HINT_ATTR_VALUE, true, CshHintGraphicCallbackForColors, 0);
    for (auto i=Contexts.back().Colors.begin(); i!=Contexts.back().Colors.end(); i++) {
        hint.decorated = HintPrefix(COLOR_ATTRVALUE) + i->first;
        hint.param = i->second.ConvertToUnsigned();
        AddToHints(hint);
    }
}

bool CshHintGraphicCallbackForDesigns(MscCanvas *canvas, CshHintGraphicParam /*p*/)
{
    if (!canvas) return false;
    const XY ul(0.2*HINT_GRAPHIC_SIZE_X, 0.2*HINT_GRAPHIC_SIZE_Y);
    const XY br(0.8*HINT_GRAPHIC_SIZE_X, 0.8*HINT_GRAPHIC_SIZE_Y);
    MscColorType color(0, 0, 0);
    MscLineAttr line;
    line.radius.second = 2;
    canvas->Clip(ul, br, line);
    cairo_pattern_t *pattern = cairo_pattern_create_linear(ul.x, ul.y, br.x, br.y);
    cairo_pattern_add_color_stop_rgb(pattern, 0.0, 255/255., 255/255., 255/255.);  //white
    cairo_pattern_add_color_stop_rgb(pattern, 0.1, 128/255., 128/255.,   0/255.);  //brown
    cairo_pattern_add_color_stop_rgb(pattern, 0.2, 255/255.,   0/255.,   0/255.);  //red
    cairo_pattern_add_color_stop_rgb(pattern, 0.3, 255/255., 255/255.,   0/255.);  //yellow
    cairo_pattern_add_color_stop_rgb(pattern, 0.4,   0/255., 255/255.,   0/255.);  //green
    cairo_pattern_add_color_stop_rgb(pattern, 0.5,   0/255., 255/255., 255/255.);  //cyan
    cairo_pattern_add_color_stop_rgb(pattern, 0.6,   0/255.,   0/255., 255/255.);  //blue
    cairo_pattern_add_color_stop_rgb(pattern, 0.7, 255/255.,   0/255., 255/255.);  //magenta
    cairo_pattern_add_color_stop_rgb(pattern, 0.8, 255/255.,   0/255.,   0/255.);  //red
    cairo_pattern_add_color_stop_rgb(pattern, 0.9, 255/255., 255/255.,   0/255.);  //yellow
    cairo_pattern_add_color_stop_rgb(pattern, 1.0, 255/255., 255/255., 255/255.);  //white
    cairo_set_source(canvas->GetContext(), pattern);
    cairo_rectangle(canvas->GetContext(), ul.x, ul.y, br.x, br.y);
    cairo_fill(canvas->GetContext());
    //Over a white rectange to lower saturation
    cairo_set_source_rgba(canvas->GetContext(), 1, 1, 1, 0.5);
    cairo_rectangle(canvas->GetContext(), ul.x, ul.y, br.x, br.y);
    cairo_fill(canvas->GetContext());
    canvas->UnClip();
    canvas->Line(ul, br, line);
    cairo_pattern_destroy(pattern);
    return true;
}

void Csh::AddDesignsToHints()
{
    for (auto i= Designs.begin(); i!=Designs.end(); i++)
        Hints.insert(CshHint(HintPrefix(COLOR_ATTRVALUE) + i->first, HINT_ATTR_VALUE, true, CshHintGraphicCallbackForDesigns));
}

bool CshHintGraphicCallbackForStyles(MscCanvas *canvas, CshHintGraphicParam)
{
    if (!canvas) return false;
    MscLineAttr line(LINE_SOLID, MscColorType(0,0,0), 1, CORNER_ROUND, 1);
    MscFillAttr fill(MscColorType(0,255,0), GRADIENT_UP);
    MscShadowAttr shadow(MscColorType(0,0,0));
    shadow.offset.first=true;
    shadow.offset.second=2;
    shadow.blur.first=true;
    shadow.blur.second=0;

    Block b(HINT_GRAPHIC_SIZE_X*0.1, HINT_GRAPHIC_SIZE_X*0.5, HINT_GRAPHIC_SIZE_Y*0.1, HINT_GRAPHIC_SIZE_Y*0.5);
    canvas->Fill(b, line, fill);
    canvas->Line(b, line);

    b.Shift(XY(HINT_GRAPHIC_SIZE_X*0.15, HINT_GRAPHIC_SIZE_X*0.15));
    fill.color.second = MscColorType(255,0,0);
    canvas->Fill(b, fill);
    canvas->Line(b, line);

    b.Shift(XY(HINT_GRAPHIC_SIZE_X*0.15, HINT_GRAPHIC_SIZE_X*0.15));
    fill.color.second = MscColorType(0,0,255);
    canvas->Shadow(b, shadow);
    canvas->Fill(b, fill);
    canvas->Line(b, line);
    return true;
}

bool CshHintGraphicCallbackForStyles2(MscCanvas *canvas, CshHintGraphicParam)
{
    if (!canvas) return false;
    std::vector<double> xPos(2); 
    xPos[0] = HINT_GRAPHIC_SIZE_X*0.2;
    xPos[1] = HINT_GRAPHIC_SIZE_X*0.8;
    canvas->Clip(XY(1,1), XY(HINT_GRAPHIC_SIZE_X-1, HINT_GRAPHIC_SIZE_Y-1));
    ArrowHead ah(ArrowHead::BIGARROW);
    ah.line += MscColorType(0,0,0); //black
    ah.endType.second = MSC_ARROW_SOLID;
    ah.size.second = MSC_ARROWS_INVALID;
    MscShadowAttr shadow;
    MscFillAttr fill(MscColorType(0,255,0), GRADIENT_UP);
    std::vector<double> active(2,0.);
    ah.BigCalculateAndDraw(xPos, active, HINT_GRAPHIC_SIZE_Y*0.3, HINT_GRAPHIC_SIZE_Y*0.7, 
                           true, false, fill, shadow, *canvas);
    canvas->UnClip();
    return true;
}


void Csh::AddStylesToHints()
{
    for (auto i=Contexts.back().StyleNames.begin(); i!=Contexts.back().StyleNames.end(); i++)
        if (ForbiddenStyles.find(*i) == ForbiddenStyles.end())
            AddToHints(CshHint(HintPrefix(COLOR_STYLENAME) + *i, HINT_ATTR_VALUE, true, CshHintGraphicCallbackForStyles));
}

void Csh::AddOptionsToHints() 
{
    Msc::AttributeNames(*this, false);
}

void Csh::AddDesignOptionsToHints() 
{
    Msc::AttributeNames(*this, true);
}

bool CshHintGraphicCallbackForKeywords(MscCanvas *canvas, CshHintGraphicParam)
{
    if (!canvas) return false;
    MscColorType color(128, 64, 64);
    canvas->Clip(Contour(XY(HINT_GRAPHIC_SIZE_X/2, HINT_GRAPHIC_SIZE_Y/2), HINT_GRAPHIC_SIZE_Y*0.6));
    canvas->Fill(XY(0,0), XY(HINT_GRAPHIC_SIZE_X, HINT_GRAPHIC_SIZE_Y), MscFillAttr(color, GRADIENT_DOWN));
    canvas->UnClip();
    return true;
}

void Csh::AddKeywordsToHints(bool includeParallel)
{
    AddToHints(keyword_names+1-includeParallel, HintPrefix(COLOR_KEYWORD), HINT_ATTR_VALUE, 
               CshHintGraphicCallbackForKeywords);
}

bool CshHintGraphicCallbackForEntities(MscCanvas *canvas, CshHintGraphicParam /*p*/)
{
    if (!canvas) return false;
    MscLineAttr line(LINE_SOLID, MscColorType(0,0,0), 1, CORNER_NONE, 0);
    MscLineAttr vline(LINE_SOLID, MscColorType(0,0,0), 2, CORNER_NONE, 0);
    MscFillAttr fill(MscColorType(192,192,192), GRADIENT_UP);
    MscShadowAttr shadow(MscColorType(0,0,0));
    shadow.offset.first=true;
    shadow.offset.second=2;
    shadow.blur.first=true;
    shadow.blur.second=0;

    Block b(HINT_GRAPHIC_SIZE_X*0.25, HINT_GRAPHIC_SIZE_X*0.75, HINT_GRAPHIC_SIZE_Y*0.1, HINT_GRAPHIC_SIZE_Y*0.5);
    canvas->Line(XY(HINT_GRAPHIC_SIZE_X/2, HINT_GRAPHIC_SIZE_Y*0.5), XY(HINT_GRAPHIC_SIZE_X/2, HINT_GRAPHIC_SIZE_Y*0.9), vline);
    canvas->Shadow(b, line, shadow);
    canvas->Fill(b, line, fill);
    canvas->Line(b, line);
    return true;
}

void Csh::AddEntitiesToHints()
{
    for (auto i=EntityNames.begin(); i!=EntityNames.end(); i++)
        AddToHints(CshHint(HintPrefix(COLOR_ENTITYNAME) + *i, HINT_ATTR_VALUE, true, CshHintGraphicCallbackForEntities));
}

void Csh::ProcessHints(MscCanvas &canvas, StringFormat *format, const std::string &uc, bool filter_by_uc, bool compact_same)
{
    StringFormat f;
    f.Default();
    if (format==NULL) format = &f;
    Label label;
    CshHint start("", HINT_ENTITY); //empty start
    unsigned start_len=0;
    unsigned start_counter=0;
    for (auto i=Hints.begin(); i!=Hints.end(); /*none*/) {
        label.Set(i->decorated, canvas, *format);
        i->plain = label;
        //if we filter and label does not begin with uc, we drop it
        if (filter_by_uc && uc.length() && !CaseInsensitiveBeginsWith(i->plain, uc.c_str())) {
            Hints.erase(i++);
            continue;
        }
        string::size_type dot_pos;
        //if compacting is on we combine all hints with the same prefix into a xxx.*-like hint
        //but only if  its type is ATTR_NAME
        if (compact_same && i->type == HINT_ATTR_NAME) {
            unsigned len = CaseInsensitiveCommonPrefixLen(i->plain.c_str(), uc.c_str());
            dot_pos = i->plain.find('.', len);
            if (start_len) {
                if (dot_pos != string::npos) {
                    len = CaseInsensitiveCommonPrefixLen(i->plain.c_str(), start.plain.c_str());
                    if (len>=dot_pos && i->callback==start.callback && 
                        i->param == start.param && i->selectable == start.selectable) {
                        //OK, we need to be merged with start
                        //just erase us
                        start_counter++;
                        Hints.erase(i++);
                        continue;
                    }
                }
                //Here we need to close combining, put start back to Hints
                if (start_counter) {
                    string::size_type pos_in_dec = start.decorated.find(start.plain.substr(start_len));
                    start.decorated.replace(pos_in_dec, start.plain.length()-start_len, ".*");
                    start.plain.erase(start_len+1);
                    start.keep = true;
                }
                Label label2(start.decorated, canvas, *format);
                XY xy = label2.getTextWidthHeight();
                start.x_size = int(xy.x);
                start.y_size = int(xy.y);
                start.state = HINT_ITEM_NOT_SELECTED;
                Hints.insert(start);
                start_len = 0;
            }
            //OK, now start is empty, see if i can start a new compacting run
            //it must have a dot and we shall be able to tweak the decorated string, too
            if (dot_pos != string::npos && i->decorated.find(i->plain.substr(dot_pos)) != string::npos) {
                start = *i;
                start_len = unsigned(dot_pos);
                start_counter = 0;
                Hints.erase(i++);
                continue;
            }
        }
        //OK, either we do no compacting or this one is not even a candidate for compacting
        XY xy = label.getTextWidthHeight();
        i->x_size = int(xy.x);
        i->y_size = int(xy.y);
        i->state = HINT_ITEM_NOT_SELECTED;
        i++;
    }
    //If we have unfinished compact, flush it
    if (start_len) {
        if (start_counter) {
            string::size_type pos_in_dec = start.decorated.find(start.plain.substr(start_len));
            start.decorated.replace(pos_in_dec, start.plain.length()-start_len, ".*");
            start.plain.erase(start_len+1);
            start.keep = true;
        }
        Label label2(start.decorated, canvas, *format);
        XY xy = label2.getTextWidthHeight();
        start.x_size = int(xy.x);
        start.y_size = int(xy.y);
        start.state = HINT_ITEM_NOT_SELECTED;
        Hints.insert(start);
    }
}

//returns -1 if txt is ""
//returns 0 if txt is not in coll
//returns 1 if txt is a prefix of something in coll, but not equals anything
//returns 2 if txt equals to something in coll
int FindPrefix(const std::set<std::string> &coll, const char *txt)
{
    if (txt == NULL || txt[0]==0) return -1;
    unsigned ret = 0;
    _ASSERT(strlen(txt)<UINT_MAX);
    const unsigned len = (unsigned)strlen(txt);
    for (auto i = coll.begin(); i!=coll.end(); i++) {
        if (len > i->length()) continue;
        if (strncmp(i->c_str(), txt, len)) continue;
        if (len == i->length()) return 2;
        ret = 1;
    }
    return ret;
}
