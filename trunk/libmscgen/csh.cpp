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

#include <map>
#include <cstring>   //strdup, free
#include "csh.h"
#include "color.h"
#include "stringparse.h" //for extracting csh out of strings
#include "attribute.h"  //for CaseInsensitive compares
#include "style.h"  //for Design::Reset() to obtain forbidden style names
#include "msc.h"

using namespace std;


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
    l[0][COLOR_ERROR].             SetColor( 50, 50, 50); l[0][COLOR_ERROR].effects = 0;
    l[0][COLOR_COMMENT].           SetColor(100,100,100); l[0][COLOR_COMMENT].effects = COLOR_FLAG_ITALICS;
    //For errors we keep bold, italics and color settings, just underline
    l[0][COLOR_ACTIVE_ERROR].mask = COLOR_FLAG_UNDERLINE; l[0][COLOR_ACTIVE_ERROR].effects = COLOR_FLAG_UNDERLINE;

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
    l[1][COLOR_ERROR].             SetColor( 50, 50, 50); l[1][COLOR_ERROR].effects = 0;
    l[1][COLOR_COMMENT].           SetColor(100,100,100); l[1][COLOR_COMMENT].effects = COLOR_FLAG_ITALICS;
    //For errors we keep bold, italics and color settings, just underline
    l[1][COLOR_ACTIVE_ERROR].mask = COLOR_FLAG_UNDERLINE; l[1][COLOR_ACTIVE_ERROR].effects = COLOR_FLAG_UNDERLINE;

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
    l[2][COLOR_ERROR].             SetColor( 50, 50, 50); l[2][COLOR_ERROR].effects = 0;
    l[2][COLOR_COMMENT].           SetColor(100,100,100); l[2][COLOR_COMMENT].effects = COLOR_FLAG_ITALICS;
    //For errors we keep bold, italics and color settings, just underline
    l[2][COLOR_ACTIVE_ERROR].mask = COLOR_FLAG_UNDERLINE; l[2][COLOR_ACTIVE_ERROR].effects = COLOR_FLAG_UNDERLINE;

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
    l[3][COLOR_ERROR].             SetColor(255,  0,  0); l[3][COLOR_ERROR].effects = COLOR_FLAG_ITALICS;
    l[3][COLOR_COMMENT].           SetColor(100,100,100); l[3][COLOR_COMMENT].effects = COLOR_FLAG_ITALICS;
    l[3][COLOR_ACTIVE_ERROR].      SetColor(255,  0,  0); l[3][COLOR_ACTIVE_ERROR].effects = COLOR_FLAG_UNDERLINE;
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
string Cshize(const char *input, unsigned len, const MscCshListType &cshList, int cshStyle,
              const char *textformat)
{
    map<int, CurrentState> textState;

    //Set default to COLOR_NORMAL (we know this is fully specified, mask contails all COLOR_FLAG_*s)
    textState[-1].Apply(MscCshAppearanceList[cshStyle][COLOR_NORMAL]);

    for (MscCshListType::const_iterator i = cshList.begin(); i!=cshList.end(); i++) {
        map<int, CurrentState>::iterator j = textState.upper_bound(i->last_pos); //j points strictly *after* last_pos
        j--; //now j is either before or at last_pos
        CurrentState state2revert2 = j->second;
        // if there are state changes between (first_pos and j], apply appearance to them
        if (i->first_pos-1 < j->first) {
            j++;
            for (map<int, CurrentState>::iterator jj = textState.upper_bound(i->first_pos-1); jj!=j; jj++)
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
    for (map<int, CurrentState>::const_iterator i = textState.begin(); i!=textState.end(); i++) {
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


////////////////////////////////////////////////////////////////////
void Csh::AddCSH(CshPos&pos, MscColorSyntaxType i)
{
    CshEntry e;
    e.first_pos = pos.first_pos;
    e.last_pos = pos.last_pos;
    e.color = i;
    CshList.push_back(e);
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
"pedantic", "background.color", "background.gradient", ""};

const char *const attr_names[] = {"compress", "color", "label", "number", "id",
"pos", "relative", "show", "makeroom", "readfrom", "offset",
"text.color", "text.ident", "ident", "text.format",
"arrow", "arrowsize", "arrow.size", "arrow.type", "arrow.starttype", "arrow.midtype",
"arrow.endtype", "arrow.color",
"line.color", "line.type", "line.width", "line.radius",
"vline.color", "vline.type", "vline.width",
"fill.color", "fill.gradient", "shadow.color", "shadow.offset", "shadow.blur", ""};

int find_opt_attr_name(const char *name, const char * const array[])
{
    for (int i=0; array[i][0]; i++)
        switch (CaseInsensitiveBeginsWidth(array[i], name)) {
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
}

MscColorSyntaxType Csh::GetCshAt(int pos)
{
    //Search labels backwards
    for (MscCshListType::const_reverse_iterator i = CshList.rbegin(); i!=CshList.rend(); i++)
        if (i->first_pos<=pos && i->last_pos>=pos) return i->color;
    return COLOR_NORMAL;
}

void CshContext::SetPlain()
{
    Design plain;
    plain.Reset();
    for (auto i=plain.colors.begin(); i!=plain.colors.end(); i++)
        ColorNames.insert(i->first);
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
    if (cursor_pos+1<a) return CURSOR_AT_BEGINNING;
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

//Checks if the cursor is between the two ranges and the first range
//indicated hint_can_come. If so, it copies the type and other data
//It sets hintsForcedOnly to false
//If the cursor is immediately at the beginning of the second range we do nothing
bool Csh::CheckHintCanComeBetween(const CshPos &one, const CshPos &two)
{
    if (!one.hint_can_come) return false;
    switch (CursorIn(one.last_pos+1, two.first_pos-1)) {
    case CURSOR_AT_BEGINNING:
    case CURSOR_IN:
        hintStatus = HINT_LOCATED;
        hintsForcedOnly = false;
        hintType = one.hint_type;
        hintAttrName = one.hint_attr_name;
        return true;
    }
    return false;
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

std::string Csh::HintPrefix(MscColorSyntaxType t) const
{
    CurrentState state;
    state.Apply(MscCshAppearanceList[cshScheme][t]);
    return state.Print(false);
}

void Csh::AddToHints(const char names[][ENUM_STRING_LEN], const string &prefix)
{
    //index==0 is usually "invalid"
    for (int i=1; names[i][0]; i++)
        AddToHints(prefix+names[i]);
}

void Csh::AddColorValuesToHints()
{
    AddToHints(HintPrefixNonSelectable()+"<\"red,green,blue\">");
    AddToHints(HintPrefixNonSelectable()+"<\"red,green,blue,opacity\">");
    AddToHints(HintPrefixNonSelectable()+"<\"color name,opacity\">");
    AddToHints(HintPrefixNonSelectable()+"<\"color name+-brightness%\">");
    for (auto i=Contexts.back().ColorNames.begin(); i!=Contexts.back().ColorNames.end(); i++)
        AddToHints(HintPrefix(COLOR_ATTRVALUE) + *i);
}

void Csh::AddDesignsToHints()
{
    for (auto i= Designs.begin(); i!=Designs.end(); i++)
        Hints.insert(HintPrefix(COLOR_ATTRVALUE) + i->first);
}

void Csh::AddStylesToHints()
{
    for (auto i=Contexts.back().StyleNames.begin(); i!=Contexts.back().StyleNames.end(); i++)
        AddToHints(HintPrefix(COLOR_STYLENAME) + *i);
}

void Csh::AddOptionsToHints()
{
    Msc::AttributeNames(*this);
}

void Csh::AddKeywordsToHints()
{
    static const char names[][ENUM_STRING_LEN] =
    {"parallel", "block", "pipe", "nudge", "heading", "newpage", "defstyle",
    "defcolor", "defdesign", "vertical", "mark", ""};
    AddToHints(names, HintPrefix(COLOR_KEYWORD));
}

void Csh::AddEntitiesToHints()
{
    for (auto i=EntityNames.begin(); i!=EntityNames.end(); i++)
        AddToHints(HintPrefix(COLOR_ENTITYNAME) + *i);
}

