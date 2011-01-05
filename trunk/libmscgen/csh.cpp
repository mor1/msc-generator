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
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
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
    for (int scheme=0; scheme<CSH_SCHEME_MAX; scheme++)
        for (int i=0; i<COLOR_MAX; i++)
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
    l[3][COLOR_MARKERNAME].        SetColor(  0,255,  0); l[2][COLOR_MARKERNAME].effects = COLOR_FLAG_BOLD;
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
    l[3][COLOR_ATTRVALUE].         SetColor(  0,  0,  0); l[3][COLOR_ATTRVALUE].effects = 0;
    l[3][COLOR_COLORDEF].          SetColor(  0,  0,  0); l[3][COLOR_COLORDEF].effects = 0;
    l[3][COLOR_LABEL_TEXT].        SetColor(  0,  0,  0); l[3][COLOR_LABEL_TEXT].effects = 0;
    l[3][COLOR_LABEL_ESCAPE].      SetColor(  0,  0,  0); l[3][COLOR_LABEL_ESCAPE].effects = COLOR_FLAG_BOLD;
    l[3][COLOR_COMMENT].           SetColor(100,100,100); l[3][COLOR_COMMENT].effects = COLOR_FLAG_ITALICS;
    l[3][COLOR_ERROR].      SetColor(255,  0,  0);        l[3][COLOR_ERROR].effects = COLOR_FLAG_UNDERLINE;
}

struct CurrentState {
    int effects;
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
    if (color.valid) ret += fakeDash?"\377c":"\\c" + color.Print();
    return ret;
}

//Takes a chart description and makes it into a color syntax highlighted label
string Cshize(const char *input, unsigned len, const CshListType &cshList, int cshStyle,
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
        offset += format.length();
        lastState = i->second;
    }
    //replace "\n" to "\\n"
    int pos = ret.find("\n");
    while (pos != string::npos) {
        ret.replace(pos, 1, "\377n");
        pos = ret.find("\n", pos+2);
    }
    //add escape for {[;#" if they are not yet escaped
    pos = ret.find_first_of("{[;#\\\"");
    while (pos != string::npos) {
        int pos2 = pos-1;
        while (pos2>=0 && ret[pos2]=='\\') pos2--;
        //if odd number of \s
        if ((pos-pos2)%2)
            ret.insert(pos, "\\");
        pos = ret.find_first_of("{[;#\\\"", pos+2);
    }
    //replace octal 277 back to '\'
    for (pos = ret.length()-1; pos>=0; pos--)
        if (ret[pos]=='\377') ret[pos] = '\\';

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
        StringFormat::ExtractCSH(pos.first_pos, value, *this);
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
    pos.first_pos++;
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
                comment.first_pos = pos.first_pos + (p - copy)-1;
                while (*p!=0 && *p!=0x0d && *p!=0x0a) *(p++) = ' ';
                comment.last_pos = pos.first_pos + (p - copy);
                AddCSH(comment, COLOR_COMMENT);
            } else
                p++; //step over the escaped #

        }
    }
    AddCSH_AttrValue(pos, copy+1, NULL);
    free(copy);
}

const char *const keyword_names[] = {"heading", "newpage", "nudge", "parallel",
"block", "pipe", "defdesign", "defcolor", "defstyle",
"vertical", "mark", ""};

const char *const opt_names[] = {"msc", "hscale", "compress", "numbering",
"numbering.pre", "numbering.post", "numbering.append", "numbering.format",
"pedantic", "background.color", "background.color2", "background.gradient", ""};

const char *const attr_names[] = {"compress", "color", "label", "number", "id",
"pos", "relative", "show", "makeroom", "readfrom", "offset",
"text.color", "text.ident", "ident", "text.format",
"arrow", "arrowsize", "arrow.size", "arrow.type", "arrow.starttype", "arrow.midtype",
"arrow.endtype", "arrow.color",
"line.color", "line.type", "line.width", "line.radius",
"vline.color", "vline.type", "vline.width",
"fill.color", "fill.color2", "fill.gradient", "shadow.color", "shadow.offset", "shadow.blur", ""};

int find_opt_attr_name(const char *name, const char * const array[])
{
    for (int i=0; array[i][0]; i++)
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
    int match_result = find_opt_attr_name(name, keyword_names);
    int match_result_options = find_opt_attr_name(name, opt_names);
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
    char const *const *array;
    if (color == COLOR_OPTIONNAME) array = opt_names;
    if (color == COLOR_ATTRNAME) array = attr_names;
    int match_result = find_opt_attr_name(name, array);
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
    int match_result = find_opt_attr_name(name, attr_names);
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

void Csh::ParseText(const char *input, unsigned len, int cursor_p, int scheme)
{
    //initialize data struct
    cursor_pos = cursor_p;
    cshScheme = scheme;
    CshList.clear();
    EntityNames.clear();
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
    if (hintStatus!=HINT_READY || Hints.size()==0)
        return;
    //Take one from first, since RichEditCtrel works that way
    --hintedStringPos.first_pos;
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
    StyleNames.insert("weak");
    StyleNames.insert("strong");
}

Csh::Csh() : hintStatus(HINT_NONE), cursor_pos(-1), was_partial(false)
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
        }
    } else switch (CursorIn(one.last_pos+1, lookahead.first_pos-1)) {
        case CURSOR_AFTER:
        case CURSOR_BEFORE:
        case CURSOR_AT_END:
            return false;
        }
    hintStatus = HINT_LOCATED;
    hintsForcedOnly = false;
    hintType = ht;
    hintAttrName = a_name?a_name:"";
    return true;
}

//Check specifically for entity hints and if true, add entities to hints & set to HINT_READY
bool Csh::CheckEntityHintAtAndBefore(const CshPos &one, const CshPos &two)
{
    if (!CheckHintAtAndBefore(one, two, HINT_ENTITY)) return false;
    AddEntitiesToHints();
    hintStatus = HINT_READY;
    return true;
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
    hintAttrName = a_name?a_name:"";
    CshCursorRelPosType in_two = CursorIn(two);
    if (in_two==CURSOR_AT_END || in_two==CURSOR_BEFORE)
        hintsForcedOnly = true;
    if (in_two>=CURSOR_AT_BEGINNING)
        hintedStringPos = two;
    return true;
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
    hintAttrName = a_name?a_name:"";
    hintedStringPos = one;
    return true;
}

//If the hint had been located and equals to this, we set it ready
bool Csh::CheckHintLocated(CshHintType ht)
{
    if (hintStatus!=HINT_LOCATED || hintType!=ht)
        return false;
    hintStatus = HINT_READY;
    return true;
}

std::string Csh::HintPrefix(MscColorSyntaxType t) const
{
    CurrentState state;
    state.Apply(MscCshAppearanceList[cshScheme][t]);
    return state.Print(false);
}


bool CshHintGraphicCallbackForAttributeNames(MscDrawer *msc, CshHintGraphicParam /*p*/)
{
    if (!msc) return false;
    const double w = 0.4*HINT_GRAPHIC_SIZE_X;
    const double h = 0.08*HINT_GRAPHIC_SIZE_Y;
    const double off = 0.35*HINT_GRAPHIC_SIZE_Y;
    MscColorType color(0, 0, 0);
    MscLineAttr line;
    line.radius.second = 3;
    MscFillAttr fill(color, GRADIENT_NONE);
    msc->Fill(XY((HINT_GRAPHIC_SIZE_X-w)/2, off), XY((HINT_GRAPHIC_SIZE_X+w)/2, off+h), line, fill);
    msc->Fill(XY((HINT_GRAPHIC_SIZE_X-w)/2, HINT_GRAPHIC_SIZE_Y-off-h), XY((HINT_GRAPHIC_SIZE_X+w)/2, HINT_GRAPHIC_SIZE_Y-off), line, fill);
    return true;
}

void Csh::AddToHints(CshHint &&h) 
{
    if (h.callback==NULL && h.type == HINT_ATTR_NAME) {
        h.callback = CshHintGraphicCallbackForAttributeNames;
        h.param = NULL; 
    }
    Hints.insert(std::move(h));
}

void Csh::AddToHints(const char names[][ENUM_STRING_LEN], const string &prefix, CshHintType t, 
                     CshHintGraphicCallback c)
{
    //index==0 is usually "invalid"
    for (int i=1; names[i][0]; i++)
        AddToHints(CshHint(prefix+names[i], t, true, c, CshHintGraphicParam(i)));
}

void Csh::AddToHints(const char names[][ENUM_STRING_LEN], const string &prefix, CshHintType t, 
                     CshHintGraphicCallback c, CshHintGraphicParam p)
{
    //index==0 is usually "invalid"
    for (int i=1; names[i][0]; i++)
        AddToHints(CshHint(prefix+names[i], t, true, c, p));
}

bool CshHintGraphicCallbackForColors(MscDrawer *msc, CshHintGraphicParam p)
{
    if (!msc) return false;
    const int size = HINT_GRAPHIC_SIZE_Y-3;
    const int off_x = (HINT_GRAPHIC_SIZE_X - size)/2;
    const int off_y = 1;
    MscColorType color(p);
    Block b(XY(off_x, off_y), XY(off_x+size, off_y+size));
    b.Round();
    msc->Fill(b, MscFillAttr(color, GRADIENT_NONE));
    b.Expand(0.5);
    msc->Line(b, MscLineAttr(LINE_SOLID, MscColorType(0,0,0), 1, 0));
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
        hint.param = i->second;
        AddToHints(hint);
    }
}

bool CshHintGraphicCallbackForDesigns(MscDrawer *msc, CshHintGraphicParam /*p*/)
{
    if (!msc) return false;
    const XY ul(0.2*HINT_GRAPHIC_SIZE_X, 0.2*HINT_GRAPHIC_SIZE_Y);
    const XY br(0.8*HINT_GRAPHIC_SIZE_X, 0.8*HINT_GRAPHIC_SIZE_Y);
    MscColorType color(0, 0, 0);
    MscLineAttr line;
    line.radius.second = 2;
    msc->Clip(ul, br, line);
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
    cairo_set_source(msc->GetContext(), pattern);
    cairo_rectangle(msc->GetContext(), ul.x, ul.y, br.x, br.y);
    cairo_fill(msc->GetContext());
    //Over a white rectange to lower saturation
    cairo_set_source_rgba(msc->GetContext(), 1, 1, 1, 0.5);
    cairo_rectangle(msc->GetContext(), ul.x, ul.y, br.x, br.y);
    cairo_fill(msc->GetContext());
    msc->UnClip();
    msc->Line(ul, br, line);
    cairo_pattern_destroy(pattern);
    return true;
}

void Csh::AddDesignsToHints()
{
    for (auto i= Designs.begin(); i!=Designs.end(); i++)
        Hints.insert(CshHint(HintPrefix(COLOR_ATTRVALUE) + i->first, HINT_ATTR_VALUE, true, CshHintGraphicCallbackForDesigns));
}

bool CshHintGraphicCallbackForStyles(MscDrawer *msc, CshHintGraphicParam p)
{
    if (!msc) return false;
    MscLineAttr line(LINE_SOLID, MscColorType(0,0,0), 1, 1);
    MscFillAttr fill(MscColorType(0,255,0), GRADIENT_UP);
    MscShadowAttr shadow(MscColorType(0,0,0));
    shadow.offset.first=true;
    shadow.offset.second=2;
    shadow.blur.first=true;
    shadow.blur.second=0;

    Block b(HINT_GRAPHIC_SIZE_X*0.1, HINT_GRAPHIC_SIZE_X*0.5, HINT_GRAPHIC_SIZE_Y*0.1, HINT_GRAPHIC_SIZE_Y*0.5);
    msc->Fill(b, line, fill);
    msc->Line(b, line);

    b.Shift(XY(HINT_GRAPHIC_SIZE_X*0.15, HINT_GRAPHIC_SIZE_X*0.15));
    fill.color.second = MscColorType(255,0,0);
    msc->Fill(b, fill);
    msc->Line(b, line);

    b.Shift(XY(HINT_GRAPHIC_SIZE_X*0.15, HINT_GRAPHIC_SIZE_X*0.15));
    fill.color.second = MscColorType(0,0,255);
    msc->Shadow(b, shadow, false);
    msc->Fill(b, fill);
    msc->Line(b, line);
    return true;
}

bool CshHintGraphicCallbackForStyles2(MscDrawer *msc, CshHintGraphicParam p)
{
    if (!msc) return false;
    const double xx = 0.7;
    std::vector<double> xPos(2); 
    xPos[0] = HINT_GRAPHIC_SIZE_X*0.2;
    xPos[1] = HINT_GRAPHIC_SIZE_X*0.8;
    msc->Clip(XY(1,1), XY(HINT_GRAPHIC_SIZE_X-1, HINT_GRAPHIC_SIZE_Y-1));
    ArrowHead ah(ArrowHead::BIGARROW);
    ah.line += MscColorType(0,0,0); //black
    ah.endType.second = MSC_ARROW_SOLID;
    ah.size.second = MSC_ARROWS_INVALID;
    MscFillAttr fill(MscColorType(0,255,0), GRADIENT_UP);
    ah.BigDraw(xPos, HINT_GRAPHIC_SIZE_Y*0.3, HINT_GRAPHIC_SIZE_Y*0.7, false, fill, msc);
    msc->UnClip();
    return true;
}


void Csh::AddStylesToHints()
{
    for (auto i=Contexts.back().StyleNames.begin(); i!=Contexts.back().StyleNames.end(); i++)
        AddToHints(CshHint(HintPrefix(COLOR_STYLENAME) + *i, HINT_ATTR_VALUE, true, CshHintGraphicCallbackForStyles));
}

void Csh::AddOptionsToHints()
{
    Msc::AttributeNames(*this);
}

bool CshHintGraphicCallbackForKeywords(MscDrawer *msc, CshHintGraphicParam p)
{
    if (!msc) return false;
    const int size = HINT_GRAPHIC_SIZE_Y-2;
    const int off_x = (HINT_GRAPHIC_SIZE_X - size)/2;
    const int off_y = 1;
    MscColorType color(128, 64, 64);
    msc->Clip(contour::Ellipse(XY(HINT_GRAPHIC_SIZE_X/2, HINT_GRAPHIC_SIZE_Y/2), HINT_GRAPHIC_SIZE_Y*0.6));
    msc->Fill(XY(0,0), XY(HINT_GRAPHIC_SIZE_X, HINT_GRAPHIC_SIZE_Y), MscFillAttr(color, GRADIENT_DOWN));
    msc->UnClip();
    return true;
}

void Csh::AddKeywordsToHints()
{
    static const char names[][ENUM_STRING_LEN] =
    {"parallel", "block", "pipe", "nudge", "heading", "newpage", "defstyle",
    "defcolor", "defdesign", "vertical", "mark", "parallel", ""};
    AddToHints(names, HintPrefix(COLOR_KEYWORD), HINT_ATTR_VALUE, CshHintGraphicCallbackForKeywords);
}

bool CshHintGraphicCallbackForEntities(MscDrawer *msc, CshHintGraphicParam /*p*/)
{
    if (!msc) return false;
    MscLineAttr line(LINE_SOLID, MscColorType(0,0,0), 1, 0);
    MscLineAttr vline(LINE_SOLID, MscColorType(0,0,0), 2, 0);
    MscFillAttr fill(MscColorType(192,192,192), GRADIENT_UP);
    MscShadowAttr shadow(MscColorType(0,0,0));
    shadow.offset.first=true;
    shadow.offset.second=2;
    shadow.blur.first=true;
    shadow.blur.second=0;

    Block b(HINT_GRAPHIC_SIZE_X*0.25, HINT_GRAPHIC_SIZE_X*0.75, HINT_GRAPHIC_SIZE_Y*0.1, HINT_GRAPHIC_SIZE_Y*0.5);
    msc->Line(XY(HINT_GRAPHIC_SIZE_X/2, HINT_GRAPHIC_SIZE_Y*0.5), XY(HINT_GRAPHIC_SIZE_X/2, HINT_GRAPHIC_SIZE_Y*0.9), vline);
    msc->Shadow(b, line, shadow, false);
    msc->Fill(b, line, fill);
    msc->Line(b, line);
    return true;
}

void Csh::AddEntitiesToHints()
{
    for (auto i=EntityNames.begin(); i!=EntityNames.end(); i++)
        AddToHints(CshHint(HintPrefix(COLOR_ENTITYNAME) + *i, HINT_ATTR_VALUE, true, CshHintGraphicCallbackForEntities));
}

void Csh::ProcessHints(MscDrawer *msc, StringFormat *format, const std::string &uc, bool filter_by_uc, bool compact_same)
{
    if (!msc) return;
    StringFormat f;
    if (format==NULL) format = &f;
    Label label(msc);
    CshHint start("", HINT_ENTITY); //empty start
    int start_len=0;
    int start_counter;
    for (auto i=Hints.begin(); i!=Hints.end(); /*none*/) {
        label.Set(i->decorated, *format);
        i->plain = label;
        //if we filter and label does not begin with uc, we drop it
        if (filter_by_uc && uc.length() && !CaseInsensitiveBeginsWith(i->plain, uc.c_str())) {
            Hints.erase(i++);
            continue;
        }
        int dot_pos;
        if (compact_same) {
            int len = CaseInsensitiveCommonPrefixLen(i->plain.c_str(), uc.c_str());
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
                    int pos_in_dec = start.decorated.find(start.plain.substr(start_len));
                    start.decorated.replace(pos_in_dec, start.plain.length()-start_len, ".*");
                    start.plain.erase(start_len+1);
                    start.keep = true;
                }
                Label label2(start.decorated, msc, *format);
                XY xy = label2.getTextWidthHeight();
                start.x_size = xy.x;
                start.y_size = xy.y;
                start.state = HINT_ITEM_NOT_SELECTED;
                Hints.insert(start);
                start_len = 0;
            }
            //OK, now start is empty, see if i can start a new compacting run
            //it must have a dot and we shall be able to tweak the decorated string, too
            if (dot_pos != string::npos && i->decorated.find(i->plain.substr(dot_pos)) != string::npos) {
                start = *i;
                start_len = dot_pos;
                start_counter = 0;
                Hints.erase(i++);
                continue;
            }
        }
        //OK, either we do no compacting or this one is not even a candidate for compacting
        XY xy = label.getTextWidthHeight();
        i->x_size = xy.x;
        i->y_size = xy.y;
        i->state = HINT_ITEM_NOT_SELECTED;
        i++;
    }
    //If we have unfinished compact, flush it
    if (start_len) {
        if (start_counter) {
            int pos_in_dec = start.decorated.find(start.plain.substr(start_len));
            start.decorated.replace(pos_in_dec, start.plain.length()-start_len, ".*");
            start.plain.erase(start_len+1);
            start.keep = true;
        }
        Label label2(start.decorated, msc, *format);
        XY xy = label2.getTextWidthHeight();
        start.x_size = xy.x;
        start.y_size = xy.y;
        start.state = HINT_ITEM_NOT_SELECTED;
        Hints.insert(start);
    }
}
