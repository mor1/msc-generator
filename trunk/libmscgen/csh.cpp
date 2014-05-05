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
/** @file csh.cpp The definition of classes for Color Syntax Highlighting.
 * @ingroup libmscgen_files */
/** @defgroup libmscgen_hintpopup_callbacks Callback functions for hint popup listbox symbols
 * @ingroup libmscgen*/

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

const ShapeCollection *Csh::def_shapes = new ShapeCollection;

/** Finds a text in a collection - or its prefix if no exact match.
 @returns 0 if no match, 1 if text is a prefix of one of the entries,
 2 if text is one of the entries.*/
unsigned FindPrefix(const std::set<std::string> &set, const string &text) 
{
    auto i = set.lower_bound(text);
    if (i==set.end()) return 0;
    if (*i == text) return 2;
    //no exact match, see if text is a prefix of the next one
    i++;
    if (i==set.end()) return 0;
    return CaseInsensitiveBeginsWith(i->c_str(), text.c_str());
}
                                                               
bool ColorSyntaxAppearance::operator==(const struct ColorSyntaxAppearance &p) const
{
    if ((effects & mask) != (p.effects & p.mask)) return false;
    if (mask != p.mask) return false;
    return r==p.r && g==p.g && b==p.b;
}


/**Checks if 'e' overlaps an entry already in the list */
bool CshListType::CheckIfOverlap(const CshEntry &e) const
{

    for (const auto &p :*this)
        if (p.first_pos<=e.last_pos && e.first_pos<=p.last_pos)
            return true;
    return false;
}


void CshErrorList::Add(const CshPos &pos, const char *t)
{
    //check that we do not add the same error twice
    for (const auto &e : *this)
        if (e.first_pos == pos.first_pos && e.text==t)
            return;
    resize(size()+1);
    CshError &e = at(size()-1);
    e.first_pos=pos.first_pos;
    e.last_pos=pos.last_pos;
    e.color = COLOR_ERROR;
    if (t) e.text=t;
}

void CshErrorList::Add(const CshPos &pos, const std::string &t)
{
    //check that we do not add the same error twice
    for (const auto &e : *this)
        if (e.first_pos == pos.first_pos && e.text==t)
            return;
    resize(size()+1);
    CshError &e = at(size()-1);
    e.first_pos = pos.first_pos;
    e.last_pos = pos.last_pos;
    e.color = COLOR_ERROR;
    e.text = t;
}

void CshErrorList::Add(const CshPos &pos, std::string &&t)
{
    //check that we do not add the same error twice
    for (const auto &e : *this)
        if (e.first_pos == pos.first_pos && e.text==t)
            return;
    resize(size()+1);
    CshError &e = at(size()-1);
    e.first_pos = pos.first_pos;
    e.last_pos = pos.last_pos;
    e.color = COLOR_ERROR;
    e.text = std::move(t);
}

ColorSyntaxAppearance MscCshAppearanceList[CSH_SCHEME_MAX][COLOR_MAX];
void MscInitializeCshAppearanceList(void)
{
    //shorter alias to make things easier to see below
    ColorSyntaxAppearance (&l)[CSH_SCHEME_MAX][COLOR_MAX] = MscCshAppearanceList;

    //case should be taken. Any value that equals to COLOR_NORMAL may not get a CSH entry
    //(to optimize away CSH entries of no effect).
    //However, some of the CSH entries are not only applied to plain text, but also 
    //on top of other CSH entries. E.g., text escapes on top of text, and often comments on
    //top of text, perhaps also error.
    //Some others are needed for smart ident (LABEL_TEXT, COLON)
    //For these we widen the mask with COLOR_FLAG_DIFFERENT, so they become different from 
    //COLOR_NORMAL

    //Set the mask to default: we set all parameters
    for (unsigned scheme=0; scheme<CSH_SCHEME_MAX; scheme++) {
        for (unsigned i=0; i<COLOR_MAX; i++) 
            l[scheme][i].mask = COLOR_FLAG_BOLD | COLOR_FLAG_ITALICS | COLOR_FLAG_UNDERLINE | COLOR_FLAG_COLOR;
        l[scheme][COLOR_LABEL_ESCAPE].mask |= COLOR_FLAG_DIFFERENT_DRAW;
        l[scheme][COLOR_COMMENT].mask |= COLOR_FLAG_DIFFERENT_DRAW;
        l[scheme][COLOR_LABEL_TEXT].mask |= COLOR_FLAG_DIFFERENT_NO_DRAW;
        l[scheme][COLOR_COLON].mask |= COLOR_FLAG_DIFFERENT_NO_DRAW;
    }


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
    l[0][COLOR_NO_ERROR].mask = COLOR_FLAG_UNDERLINE;     l[0][COLOR_NO_ERROR].effects = 0;

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
    l[1][COLOR_NO_ERROR].mask = COLOR_FLAG_UNDERLINE;     l[1][COLOR_NO_ERROR].effects = 0;

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
    l[2][COLOR_NO_ERROR].mask = COLOR_FLAG_UNDERLINE;     l[2][COLOR_NO_ERROR].effects = 0;

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
    l[3][COLOR_ERROR].             SetColor(255,  0,  0); l[3][COLOR_ERROR].effects = COLOR_FLAG_UNDERLINE;
    l[3][COLOR_NO_ERROR].          SetColor(  0,  0,  0); l[3][COLOR_NO_ERROR].effects = 0;
    l[3][COLOR_ERROR].mask = COLOR_FLAG_UNDERLINE | COLOR_FLAG_COLOR;
    l[3][COLOR_NO_ERROR].mask = COLOR_FLAG_UNDERLINE | COLOR_FLAG_COLOR;
}

/** State of coloring */
struct CurrentState {
    unsigned effects;    ///<What effects are in effect
    ColorType color;  ///<What is the color
    CurrentState() : effects(0) {}
    void Apply(const ColorSyntaxAppearance &appearance);  ///<Apply coloring for a language element type to us.
    string Print(bool fakeDash=true) const;                  ///<Print a set of string formatting escapes to represent the status. Do \\377 (octal 255) instead of slashes if fakeDash is true.
    bool operator == (const CurrentState &other) const
    {return effects == other.effects && color == other.color;}
};

void CurrentState::Apply(const ColorSyntaxAppearance &appearance)
{
    effects &= !appearance.mask;
    effects |= appearance.effects & appearance.mask;
    if (appearance.mask & COLOR_FLAG_COLOR) {
        color.r = appearance.r;
        color.g = appearance.g;
        color.b = appearance.b;
        color.a = 255;
        color.type = ColorType::COMPLETE;
    }
}

string CurrentState::Print(bool fakeDash) const
{
    //Insert \377 (octal 255) instead of a \ to distinguish between \s already there
    string ret = fakeDash?"\377B\377I\377U":"\\B\\I\\U";
    if (!(effects & COLOR_FLAG_BOLD)) ret += fakeDash?"\377b":"\\b";
    if (!(effects & COLOR_FLAG_ITALICS)) ret += fakeDash?"\377i":"\\i";
    if (!(effects & COLOR_FLAG_UNDERLINE)) ret += fakeDash?"\377u":"\\u";
    if (color.type!=ColorType::INVALID) ret += (fakeDash?"\377c":"\\c") + color.Print();
    return ret;
}

/** Turn a chart text into another chart text, which when drawn produces the colored version of the original chart text.
 *
 * This is done by creating no entities, but a series of labels each representing a 
 * line in the original chart.
 * @param [in] input The original chart text.
 * @param [in] len The length of the chart text.
 * @param [in] cshList The identified colored language elements.
 * @param [in] cshStyle The number of the coloring scheme to use.
 * @param [in] textformat Some formatting escapes defining the default font to use,
 */
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
    //replace spaces with escaped ESCAPE_CHAR_SPACE
    for (unsigned i = 0; i<ret.length(); i++)
        if (ret[i] ==' ')
            ret.replace(i, 1, "\\" ESCAPE_STRING_SPACE);

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
void Csh::AddCSH(const CshPos&pos, EColorSyntaxType i)
{
    if (use_scheme && 
          MscCshAppearanceList[*use_scheme][i] == 
          MscCshAppearanceList[*use_scheme][COLOR_NORMAL])
        return;
    if (pos.first_pos>pos.last_pos) return;
    CshEntry e;
    e.first_pos = pos.first_pos;
    e.last_pos = pos.last_pos;
    e.color = i;
    CshList.AddToBack(e);
}

void Csh::AddCSH_ErrorAfter(const CshPos&pos, const char *text)
{
    CshPos pos2;
    pos2.first_pos = pos.last_pos;
    pos2.last_pos = pos.last_pos+1;
    CshErrors.Add(pos2, text);
}

void Csh::AddCSH_ErrorAfter(const CshPos&pos, std::string &&text)
{
    CshPos pos2;
    pos2.first_pos = pos.last_pos;
    pos2.last_pos = pos.last_pos+1;
    CshErrors.Add(pos2, std::move(text));
}

void Csh::AddCSH_AttrValue(const CshPos& pos, const char *value, const char *name)
{
    if (!name || CaseInsensitiveEqual(name, "label") ||
        CaseInsensitiveEqual(name, "text.format") ||
        CaseInsensitiveEqual(name, "numbering.format") ||
        CaseInsensitiveEqual(name, "numbering.append") ||
        CaseInsensitiveEqual(name, "numbering.pre") ||
        CaseInsensitiveEqual(name, "numbering.post")) {
        //This is a label or text.format chart option
        //Add escape symbols
        //Note the trick here: we add +1 to the pos.
        //If the string has escapes, it must have been specified via 
        //a quoted string, but in this case "pos" points to the beginning
        //quotation mark, so we add one.
        //If there are no escapes ExtractCSH() does nothing, so the passed 
        //pos will not matter anyway.
        StringFormat::ExtractCSH(pos.first_pos+1, value, strlen(value), *this);
    } else {
        // No match - regular attribute value
        AddCSH(pos, COLOR_ATTRVALUE);
    }
}

void Csh::AddCSH_AttrColorValue(const CshPos& pos)
{
    AddCSH(pos, COLOR_ATTRVALUE);
}

void Csh::AddCSH_ColonString(const CshPos& pos, const char *value, bool processComments)
{
    CshPos colon = pos;
    colon.last_pos = colon.first_pos;
    AddCSH(colon, COLOR_COLON);
    if (processComments) {
        const char *beginning = value+1;
        if (*beginning) {
            const char *p = beginning;
            while (*p!=0) {
                //search for #
                while (*p!=0 && *p!='#') p++;
                if (!*p) break;
                //if we hit a # count the \s before
                unsigned count = 0;
                //string starts with colon, so we are limited by that
                while (*(p-1-count) == '\\') count++;
                //if even number then mark as comment till end of line
                if (count%2 == 0) {
                    //'p' points to the # symbol starting the comment
                    CshPos txt;
                    txt.first_pos = pos.first_pos + int(beginning - value);
                    txt.last_pos = pos.first_pos + int(p - value) - 1;
                    if (txt.last_pos>=txt.first_pos) 
                        StringFormat::ExtractCSH(txt.first_pos, beginning, p-beginning, *this);
                    CshPos comment;
                    comment.first_pos = pos.first_pos + int(p - value);
                    //step to end of comment
                    while (*p!=0 && *p!=0x0d && *p!=0x0a) p++;
                    comment.last_pos = pos.first_pos + int(p - value);
                    AddCSH(comment, COLOR_COMMENT);
                    //step over (potential) newlines
                    while (*p==0x0d || *p==0x0a) p++;
                    beginning = p;
                } else
                    p++; //odd number: an escaped # symbol: step over the escaped #
            }
            //'p' points to the char after the last one (terminating zero)
            CshPos txt;
            txt.first_pos = pos.first_pos + int(beginning - value);
            txt.last_pos = pos.first_pos + int(p - value) - 1;
            StringFormat::ExtractCSH(txt.first_pos, beginning, p-beginning, *this); //omit the colon
        }
    } else {
        CshPos p;
        p.first_pos = pos.first_pos+1;
        p.last_pos = pos.last_pos;
        StringFormat::ExtractCSH(p.first_pos, value+1, strlen(value), *this); //omit the colon
    }
}

void Csh::AddCSH_LabelEscape(const CshPos& pos)
{
    //We go back and find if there is any existing CSH record for us
    for (auto i = --CshList.end(); i!=CshList.end() && i->last_pos>pos.first_pos; i--)
        if (i->color == COLOR_LABEL_TEXT && i->first_pos < pos.last_pos) {
            const int orig_end = i->last_pos;
            CshEntry esc;
            esc.color = COLOR_LABEL_ESCAPE;
            esc.first_pos = pos.first_pos;
            esc.last_pos = pos.last_pos;
            if (i->first_pos<pos.first_pos) {
                //LABEL_TEXT extends before pos.
                //truncate it
                i->last_pos = pos.first_pos-1;
                //insert a LABEL_ESCAPE
                i++;
                i = CshList.insert(i, esc);
            } else
                //just change it to LABEL_ESCAPE
                *i = esc;
            //i now points to the LABEL_ESCAPE
            if (orig_end > pos.last_pos) {
                //LABEL_TEXT extended after pos: insert remainder
                i++;
                esc.color = COLOR_LABEL_TEXT;
                esc.first_pos = pos.last_pos + 1;
                esc.last_pos = orig_end;
                CshList.insert(i, esc);
            }
            break; //we are done
        }
}

/** Names of keywords for coloring.
 *
 * All keywords shall be repeated here, not only known by the parser.
 * We color only these where keywords should come.*/
static const char keyword_names[][ENUM_STRING_LEN] =
{"", "parallel", "overlap", "block", "pipe", "nudge", "heading", "newpage", "defstyle",
"defcolor", "defdesign", "vertical", "mark", "show", "hide", "activate", "deactivate",
"bye", "hspace", "vspace", "symbol", "note", "comment", "endnote", /*"footnote",*/
"title", "subtitle", "defshape", ""};

/** Names of chart options for coloring.
 *
 * All options shall be repeated here, not only known by class Msc.
 * We color only these where options should come.*/
static const char opt_names[][ENUM_STRING_LEN] =
{"msc", "hscale", "compress", "numbering", "indicator", "auto_heading",
"numbering.pre", "numbering.post", "numbering.append", "numbering.format",
"pedantic", "background.color", "background.color2", "background.gradient", 
"text.color", "text.format", "text.ident", 
"text.font.face", "text.font.type", 
"text.bold", "text.italic", "text.underline", 
"text.gap.up", "text.gap.down", "text.gap.left", "text.gap.right",
"text.gap.spacing", "text.size.normal", "text.size.small", "text.wrap",
"comment.line.color", "comment.line.type", "comment.line.width", 
"comment.fill.color", "comment.fill.color2", "comment.fill.gradient",
"comment.side", 
"comment.text.color", "comment.text.format", "comment.text.ident", 
"comment.text.font.face", "comment.text.font.type", 
"comment.text.bold", "comment.text.italic", "comment.text.underline", 
"comment.text.gap.up", "comment.text.gap.down", "comment.text.gap.left", "comment.text.gap.right",
"comment.text.gap.spacing", "comment.text.size.normal", "comment.text.size.small", "comment.text.wrap",
"angle", "classic_parallel_layout", "file.url", "file.info", ""};

/** Names of attributes for coloring.
 *
 * All attribute shall be repeated here, not only known by the parser.
 * We color only these where attributes should come. 
 * We do not distinguish which arc the attribute belongs to.*/
static const char attr_names[][ENUM_STRING_LEN] =
{"draw_time", "compress", "color", "label", "refname", "number", "indicator", "collapsed",
"parallel", "overlap", "keep_with_next", "keep_together", "auto_heading",
"pos", "relative", "show", "active", "makeroom", "side", "offset", "solid",
"text.color", "text.ident", "ident", "text.format",
"text.font.face", "text.font.type", 
"text.bold", "text.italic", "text.underline", 
"text.gap.up", "text.gap.down", "text.gap.left", "text.gap.right",
"text.gap.spacing", "text.size.normal", "text.size.small", "text.wrap", "text.width",
"arrow", "arrowsize", "arrow.size", "arrow.type", "arrow.starttype", "arrow.midtype",
"arrow.endtype", "arrow.color", "arrow.xmul", "arrow.ymul", 
"arrow.line.color", "arrow.line.type", "arrow.line.width",
"line.color", "line.type", "line.width", "line.corner", "line.radius", 
"vline.color", "vline.type", "vline.width", "vline.radius", "vline.corner",
"fill.color", "fill.color2", "fill.gradient", 
"vfill.color", "vfill.color2", "vfill.gradient",
"shadow.color", "shadow.offset", "shadow.blur", 
"compressable", "xsize", "ysize", "size", "space", "angle", "slant_depth",
"note.pointer", "note.pos", "note.width", 
"lost.line.color", "lost.line.type", "lost.line.width", "lost.line.corner", "lost.line.radius", 
"x.line.color", "x.line.type", "x.line.width", "x.line.corner", "x.line.radius", "x.size",
"lost.arrow.size", "lost.arrow.type", "lost.arrow.starttype", "lost.arrow.midtype",
"lost.arrow.endtype", "lost.arrow.color", "lost.arrow.xmul", "lost.arrow.ymul",
"lost.text.color", "lost.text.ident", "ident", "lost.text.format",
"lost.text.font.face", "lost.text.font.type",
"lost.text.bold", "lost.text.italic", "lost.text.underline",
"lost.text.gap.up", "lost.text.gap.down", "lost.text.gap.left", "lost.text.gap.right",
"lost.text.gap.spacing", "lost.text.size.normal", "lost.text.size.small", "lost.text.wrap",
"shape", "shape.size", ""}; //we add shape even if it may not be available

/** Names of symbols for coloring
 *
 * All keywords shall be repeated here, not only known by the parser.
 * We only color these as keywords where a symbol name is expected (after 'symbol' keyword).*/
static const char symbol_names[][ENUM_STRING_LEN] =
{"arc", "rectangle", "...", ""};

/** Names of extended vertical position designators for coloring
 *
 * All keywords shall be repeated here, not only known by the parser.
 * We only color these as keywords where a extended vertical position designator is expected (e.g., after 'vertical' keyword).*/
static const char extvxpos_designator_names[][ENUM_STRING_LEN] =
{"left", "right", "center", ""};

/** Helper to check if 'name' us in the 'array' of string constants.*/
unsigned find_opt_attr_name(const char *name, const char array[][ENUM_STRING_LEN])
{
    for (unsigned i=0; array[i][0]; i++)
        switch (CaseInsensitiveBeginsWith(array[i], name)) {
        case 1: return 1;
        case 2: return 2;
        }
    return 0;
}

/** At this point it is either a keyword or an entity - decide and add appropriate coloring.
 *
 * This is called when a string is at the beginning of the line and is not part
 * of a valid option (e.g., has no '=' after it): it can either be a command or an entity definition
 * we give KEYWORD or KEYWORD_PARTIAL coloring for full or partial keyword matches;
 * ENTITYNAME if no keyword match, but an entity does and ENTITYNAME_FIRST otherwise.
 * Optionnames are not searched.
 * All-in-all partial matches are only given if the cursor is just after the
 * string in question. In this case we also store the partial match in
 * Csh::partial_at_cursor_pos
 * @param [in] pos The range to color 
 * @param [in] name The content of the range: the supposed keyword or entity.*/
void Csh::AddCSH_KeywordOrEntity(const CshPos&pos, const char *name)
{
    EColorSyntaxType type = COLOR_KEYWORD;
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
        AddCSH(pos, EColorSyntaxType(type+1));
        was_partial = true;
        return;
    }
    //if no keyword or option match, we assume an entityname
    AddCSH_EntityName(pos, name);
    return;
}

void Csh::AddCSH_AttrName(const CshPos&pos, const char *name, EColorSyntaxType color)
{
    static const char empty_names[][ENUM_STRING_LEN] = {""};
    const char (*array)[ENUM_STRING_LEN];
    if (color == COLOR_OPTIONNAME) array = opt_names;
    else if (color == COLOR_ATTRNAME) array = attr_names;
    else array = empty_names;
    unsigned match_result = find_opt_attr_name(name, array);
    //Honor partial matches only if cursor is right after
    if (pos.last_pos != cursor_pos && match_result == 1)
        match_result = 0;
    switch (match_result) {
    case 2: AddCSH(pos, color); return;
    case 0: AddCSH_Error(pos, array == opt_names ? "Unkown chart option." : "Unknown attribute."); return;
    case 1:
        AddCSH(pos, EColorSyntaxType(color+1));
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
void Csh::AddCSH_StyleOrAttrName(const CshPos&pos, const char *name)
{
    unsigned match_result = find_opt_attr_name(name, attr_names);
    if (pos.last_pos == cursor_pos && match_result == 1) {
        AddCSH(pos, COLOR_ATTRNAME_PARTIAL);
        was_partial = true;
        return;
    }
    if (match_result == 2) {
        AddCSH(pos, COLOR_ATTRNAME);
        return;
    }
    //if no keyword match, we assume a style name
    const unsigned u = FindPrefix(Contexts.back().StyleNames, name);
    if (u==2)
        AddCSH(pos, COLOR_ATTRVALUE);
    else if (u==1 && pos.last_pos==cursor_pos) {
        AddCSH(pos, COLOR_ATTRVALUE);
        was_partial = true;
    } else 
        //If we do not type or is it a prefix it add error
        AddCSH_Error(pos, "Unknown attribute or style name.");
}

void Csh::AddCSH_EntityName(const CshPos&pos, const char *name)
{
    const unsigned u = FindPrefix(EntityNames, name);
    if (u==2) {
        AddCSH(pos, COLOR_ENTITYNAME);
        return;
    }
    //We know that there has been no such entity yet
    //If we are currently typing it and it is a prefix 
    //of an entity, use entitycolor, else
    //the one designated for first use of entities
    //In the latter case insert to entity name database
    if (u==1 && pos.last_pos==cursor_pos) {
        AddCSH(pos, COLOR_ENTITYNAME);
        was_partial = true;
        return;
    }
    EntityNames.insert(string(name));
    AddCSH(pos, COLOR_ENTITYNAME_FIRST);
}

void Csh::AddCSH_EntityOrMarkerName(const CshPos&pos, const char *name)
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
void Csh::AddCSH_SymbolName(const CshPos&pos, const char *name)
{
    unsigned match_result = find_opt_attr_name(name, symbol_names);
    if (pos.last_pos == cursor_pos && match_result == 1) {
        AddCSH(pos, COLOR_KEYWORD_PARTIAL);
        was_partial = true;
        return;
    }
    if (match_result == 2) {
        AddCSH(pos, COLOR_KEYWORD);
        return;
    }
    //if no keyword match, not even partial, we emit an error
    string s = "Unknown symbol name. Use one of '";
    for (auto t: symbol_names)
        s.append(t).append("', '");
    s.erase(s.length()-3);
    s.append(".");
    AddCSH_Error(pos, s);
}

//Mark anything beyond the end of 'pos' as comment
void Csh::AddCSH_AllCommentBeyond(const CshPos&pos)
{
    AddCSH(CshPos(pos.last_pos+1, input_text_length), COLOR_COMMENT);
}

void Csh::AddCSH_ExtvxposDesignatorName(const CshPos&pos, const char *name)
{
    unsigned match_result = find_opt_attr_name(name, extvxpos_designator_names);
    if (pos.last_pos == cursor_pos && match_result == 1) {
        AddCSH(pos, COLOR_KEYWORD_PARTIAL);
        was_partial = true;
        return;
    }
    if (match_result == 2) {
        AddCSH(pos, COLOR_KEYWORD);
        return;
    }
    //if no keyword match, not even partial, we emit an error
    string s = "Invalid keyword here. Use one of '";
    for (auto t: extvxpos_designator_names)
        s.append(t).append("', '");
    s.erase(s.length()-3);
    s.append(".");
    AddCSH_Error(pos, s);
}

/** Callback for drawing a symbol before marker names in the hints popup list box.
 * @ingroup libmscgen_hintpopup_callbacks*/
bool CshHintGraphicCallbackForMarkers(Canvas *canvas, CshHintGraphicParam /*p*/, Csh &)
{
    if (!canvas) return false;
    LineAttr line(LINE_SOLID, ColorType(64,0,255), 1, CORNER_NONE, 0);
    FillAttr fill(ColorType(64,0,255).Lighter(0.2), GRADIENT_UP);
    ShadowAttr shadow(ColorType(0,0,0));
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

/** Parse chart text for color syntax and hint info
 *
 * @param [in] input The chart text
 * @param [in] len The length of the chart text
 * @param [in] cursor_p The current position of the cursor.
 * @param [in] scheme The number of the color syntax scheme to use.*/
void Csh::ParseText(const char *input, unsigned len, int cursor_p, unsigned scheme)
{
    //initialize data struct
    input_text_length = len;
    cursor_pos = cursor_p;
    cshScheme = scheme;
    CshList.clear();
    ColonLabels.clear();
    QuotedStrings.clear();
    CshErrors.clear();
    EntityNames.clear();
    MarkerNames.clear();
    if (!ForcedDesign.empty() && FullDesigns.find(ForcedDesign) != FullDesigns.end())
        Contexts.back() = FullDesigns[ForcedDesign];
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


const CshPos *Csh::IsInColonLabel(int pos) const
{
    //'pos' is in RichEdit units: index zero is before the first char.
    //the positions in 'Labels' is in csh units: the first char is
    //indexed 1, and for single character ranges first_pos==last_pos
    for (auto &p : ColonLabels)
        if (p.first_pos<=pos && p.last_pos>pos)
            return &p;
    return NULL;
}

const CshPos *Csh::IsInQuotedString(int pos) const
{
    //'pos' is in RichEdit units: index zero is before the first char.
    //the positions in 'Labels' is in csh units: the first char is
    //indexed 1, and for single character ranges first_pos==last_pos
    for (auto &p : QuotedStrings)
        if (p.first_pos<=pos && p.last_pos>pos)
            return &p;
    return NULL;
}

void CshContext::SetToDesign(const Context &design)
{
    full = true;
    for (auto i=design.colors.begin(); i!=design.colors.end(); i++)
        Colors[i->first] = i->second;
    for (auto i=design.styles.begin(); i!=design.styles.end(); i++)
        StyleNames.insert(i->first);
}

Csh::Csh(const Context &defaultDesign, const ShapeCollection *shapes) : 
    was_partial(false), 
    input_text_length(0),
    hintStatus(HINT_NONE), 
    addMarkersAtEnd(false), 
    pShapes(shapes),
    cursor_pos(-1),
    use_scheme(NULL)
{
    for (auto i=defaultDesign.styles.begin(); i!=defaultDesign.styles.end(); i++)
        ForbiddenStyles.insert(i->first);
    ForbiddenStyles.erase("weak");
    ForbiddenStyles.erase("strong");
    PushContext(true);
    Contexts.back().SetToDesign(defaultDesign);
    FullDesigns["plain"] = Contexts.back();
}

void Csh::PushContext(bool empty)
{
    if (empty)
        Contexts.push_back(CshContext());
    else 
        Contexts.push_back(Contexts.back());
}

/** Set the chart design. 
 *
 * Used when an msc= or msc+= option is found during csh parse 
 * @param [in] design The name of the design. 
 * @param [in] full True if the option was msc=, false if it was msc+=
 * @returns An error message if the design is not found or not of appropriate type. Empty if OK. */
std::string Csh::SetDesignTo(const std::string&design, bool full)
{
    bool found_full = true;
    auto i = FullDesigns.find(design);
    if (i==FullDesigns.end()) {
        i = PartialDesigns.find(design);
        if (i==PartialDesigns.end()) {
            return "Design '" + design + "' not defined earlier.";
        }
        found_full = false;
    }
    Contexts.back() += i->second;
    if (found_full == full) return "";
    if (found_full) return "Design '" + design + "' is a full design. Use 'msc = ' instead of 'msc += '.";
    return "Design '" + design + "' is a partial design. Use 'msc += ' instead of 'msc = '.";
}

ECursorRelPosType Csh::CursorIn(int a, int b) const
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

/** Mark hint status to HINT_LOCATED if cursor is between two ranges
 *
 * Checks if the cursor is between the two ranges and if so, it applies
 * the hinttype with status HINT_LOCATED. It sets hintsForcedOnly to false.
 * If the cursor is immediately at the beginning of the second range we do nothing.
 * @param [in] one The first range
 * @param [in] two The second range
 * @param [in] ht The type of hint appropriate to the position in the file.
 * @param [in] a_name The name of the attribute if the hint type is HINT_ATTRVALUE
 * @returns True if the cursor is in this hintable place.*/
bool Csh::CheckHintBetween(const CshPos &one, const CshPos &two, EHintType ht, const char *a_name)
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

/** Mark hint status to HINT_LOCATED if cursor is between two ranges, not touching the first.
 *
 * Checks if the cursor is between the two ranges, but not immediately after the first.
 * If so, it applies
 * the hinttype with status HINT_LOCATED. It sets hintsForcedOnly to false.
 * If the cursor is immediately at the beginning of the second range we do nothing.
 * This function is used if the hint we want to provide cannot be written immediately after
 * the preceeding element. (E.g., the preceeding element ends in a letter and our hint starts
 * with a letter. In this case concatenating the two without a space would yield a bad result.)
 * @param [in] one The first range
 * @param [in] two The second range
 * @param [in] ht The type of hint appropriate to the position in the file.
 * @param [in] a_name The name of the attribute if the hint type is HINT_ATTRVALUE
 * @returns True if the cursor is in this hintable place.*/
bool Csh::CheckHintBetweenPlusOne(const CshPos &one, const CshPos &two, EHintType ht, const char *a_name)
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

/** Mark hint status to HINT_LOCATED if cursor is after a range.
 *
 * Checks if the cursor is between 'one' and 'lookahead'.
 * If 'atEnd' is true we trigger only if the cursor is immediately after
 *'one'. If so, it applies
 * the hinttype with status HINT_LOCATED. It sets hintsForcedOnly to false.
 * This function is used when we want to hint something at the end of a yacc rule.
 * In this case lookahead contains the location of the next token. 
 * If the cursor is immediately at the beginning of the lookahead symbol we do nothing
 * @param [in] one The first range
 * @param [in] lookahead The range occupied by the lookahead symbol of yacc.
 * @param [in] atEnd If true, a hint is produced if the cursor is immediately after 'one',
 * @param [in] ht The type of hint appropriate to the position in the file.
 * @param [in] a_name The name of the attribute if the hint type is HINT_ATTRVALUE
 * @returns True if the cursor is in this hintable place.*/
bool Csh::CheckHintAfter(const CshPos &one, const CshPos &lookahead, bool atEnd, EHintType ht, const char *a_name)
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

/** Mark hint status to HINT_LOCATED if cursor is after a range, but not immediately after.
 *
 * Checks if the cursor is between 'one' and 'lookahead', but not immediately after 'one'.
 * If 'atEnd' is true we trigger only if the cursor is exactly one character after
 *'one'. If so, it applies
 * the hinttype with status HINT_LOCATED. It sets hintsForcedOnly to false.
 * This function is used when we want to hint something at the end of a yacc rule.
 * In this case lookahead contains the location of the next token. 
 * If the cursor is immediately at the beginning of the lookahead symbol we do nothing
 * @param [in] one The first range
 * @param [in] lookahead The range occupied by the lookahead symbol of yacc.
 * @param [in] atEnd If true, a hint is produced if the cursor is immediately after 'one',
 * @param [in] ht The type of hint appropriate to the position in the file.
 * @param [in] a_name The name of the attribute if the hint type is HINT_ATTRVALUE
 * @returns True if the cursor is in this hintable place.*/
bool Csh::CheckHintAfterPlusOne(const CshPos &one, const CshPos &lookahead, bool atEnd, EHintType ht, const char *a_name)
{
    if (one.last_pos >= lookahead.first_pos) return false;
    CshPos one_oneAfter = one;
    one_oneAfter.last_pos++;
    return CheckHintAfter(one_oneAfter, lookahead, atEnd, ht, a_name);
}

/** Mark hint status to HINT_LOCATED if cursor is between two ranges or inside the second one.
 *
 * Checks if the cursor is between the two ranges or inside the second. If so, it applies
 * the hinttype with status HINT_LOCATED. If cursor is inside two, hintedStringPos
 * is set to two. hintsForcedOnly is set to true iff the cursor is truely before two
 * or is at the end of two.
 * @param [in] one The first range
 * @param [in] two The second range
 * @param [in] ht The type of hint appropriate to the position in the file.
 * @param [in] a_name The name of the attribute if the hint type is HINT_ATTRVALUE
 * @returns True if the cursor is in this hintable place.*/
bool Csh::CheckHintAtAndBefore(const CshPos &one, const CshPos &two, EHintType ht, const char *a_name)
{
    if (CursorIn(one.last_pos+1, two.last_pos)<=CURSOR_AFTER) return false;
    hintStatus = HINT_LOCATED;
    hintsForcedOnly = false;
    hintType = ht;
    if (ht==HINT_MARKER)
        addMarkersAtEnd = true;
    hintAttrName = a_name?a_name:"";
    ECursorRelPosType in_two = CursorIn(two);
    if (in_two==CURSOR_AT_END || in_two==CURSOR_BEFORE)
        hintsForcedOnly = true;
    if (in_two>=CURSOR_AT_BEGINNING)
        hintedStringPos = two;
    return true;
}

/** Mark hint status to HINT_LOCATED if cursor is between two ranges or inside the second one, but not immediately after the first one.
 *
 * Checks if the cursor is between the two ranges or inside the second, but not 
 * immediately after the first. If so, it applies
 * the hinttype with status HINT_LOCATED. If cursor is inside two, hintedStringPos
 * is set to two. hintsForcedOnly is set to true iff the cursor is truely before two
 * or is at the end of two.
 * @param [in] one The first range
 * @param [in] two The second range
 * @param [in] ht The type of hint appropriate to the position in the file.
 * @param [in] a_name The name of the attribute if the hint type is HINT_ATTRVALUE
 * @returns True if the cursor is in this hintable place.*/
bool Csh::CheckHintAtAndBeforePlusOne(const CshPos &one, const CshPos &two, EHintType ht, const char *a_name)
{
    CshPos one_oneAfter = one;
    one_oneAfter.last_pos++;
    return CheckHintAtAndBefore(one_oneAfter, two, ht, a_name);
}


/** Mark hint status to HINT_LOCATED if cursor is inside, just before or just after a range.
 *
 * Checks if the cursor is inside, just before or just after a range.
 * If so, it applies the hinttype with status HINT_LOCATED. hintedStringPos
 * is set to 'one'. hintsForcedOnly is set to true if the cursor is just before one
 * or is at the end of one
 * @param [in] one The range
 * @param [in] ht The type of hint appropriate to the position in the file.
 * @param [in] a_name The name of the attribute if the hint type is HINT_ATTRVALUE
 * @returns True if the cursor is in this hintable place.*/
bool Csh::CheckHintAt(const CshPos &one, EHintType ht, const char *a_name)
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

/** Mark hint type to HINT_ENTITY and status to HINT_READY if cursor is between two ranges or inside the second one.
 *
 * Checks if the cursor is between the two ranges or inside the second. If so, it sets hint type to 
 * HINT_ENTITY and status to HINT_READY and adds the entities collected so far to the hints.
 * If cursor is inside two, hintedStringPos
 * is set to two. hintsForcedOnly is set to true iff the cursor is truely before two
 * or is at the end of two.
 * @param [in] one The first range
 * @param [in] two The second range
 * @returns True if the cursor is in this hintable place.*/
bool Csh::CheckEntityHintAtAndBefore(const CshPos &one, const CshPos &two)
{
    if (!CheckHintAtAndBefore(one, two, HINT_ENTITY)) return false;
    AddEntitiesToHints();
    hintStatus = HINT_READY;
    return true;
}

/** Mark hint type to HINT_ENTITY and status to HINT_READY if cursor is between two range or inside the second one, but not immediately after the first one.
 *
 * Checks if the cursor is between the two ranges or inside the second, but not 
 * immediately after the first. If so, sets hint type to 
 * HINT_ENTITY and status to HINT_READY and adds the entities collected so far to the hints.
 * If cursor is inside two, hintedStringPos
 * is set to two. hintsForcedOnly is set to true iff the cursor is truely before two
 * or is at the end of two. * @param [in] one The first range
 * @param [in] two The second range
 * @returns True if the cursor is in this hintable place.*/
bool Csh::CheckEntityHintAtAndBeforePlusOne(const CshPos &one, const CshPos &two)
{
    if (one.last_pos >= two.first_pos) return false;
    CshPos one_oneAfter = one;
    one_oneAfter.last_pos++;
    return CheckEntityHintAtAndBefore(one_oneAfter, two);
}

/** Mark hint type to HINT_ENTITY and status to HINT_READY if cursor is inside, just before or just after a range.
 *
 * Checks if the cursor is inside, just before or just after a range.
 * If so, sets hint type to 
 * HINT_ENTITY and status to HINT_READY and adds the entities collected so far to the hints.
 * hintedStringPos
 * is set to 'one'. hintsForcedOnly is set to true if the cursor is just before one
 * or is at the end of one
 * @param [in] one The range
 * @returns True if the cursor is in this hintable place.*/
bool Csh::CheckEntityHintAt(const CshPos &one)
{
    if (!CheckHintAt(one, HINT_ENTITY)) return false;
    AddEntitiesToHints();
    hintStatus = HINT_READY;
    return true;
}

/** Mark hint type to HINT_ENTITY and status to HINT_READY if cursor is after a range.
 *
 * Checks if the cursor is between 'one' and 'lookahead'.
 * If 'atEnd' is true we trigger only if the cursor is immediately after
 *'one'.  If so, sets hint type to 
 * HINT_ENTITY and status to HINT_READY and adds the entities collected so far to the hints.
 * It sets hintsForcedOnly to false.
 * This function is used when we want to hint an entity at the end of a yacc rule.
 * In this case lookahead contains the location of the next token. 
 * If the cursor is immediately at the beginning of the lookahead symbol we do nothing
 * @param [in] one The first range
 * @param [in] lookahead The range occupied by the lookahead symbol of yacc.
 * @param [in] atEnd If true, a hint is produced if the cursor is immediately after 'one',
 * @returns True if the cursor is in this hintable place.*/
bool Csh::CheckEntityHintAfter(const CshPos &one, const CshPos &lookahead, bool atEnd)
{
    if (!CheckHintAfter(one, lookahead, atEnd, HINT_ENTITY)) return false;
    AddEntitiesToHints();
    hintStatus = HINT_READY;
    return true;
}


/** Mark hint type to HINT_ENTITY and status to HINT_READY if cursor is after a range, but not immediately after.
 *
 * Checks if the cursor is between 'one' and 'lookahead', but not immediately after 'one'.
 * If 'atEnd' is true we trigger only if the cursor is exactly one character after
 *'one'. If so, sets hint type to 
 * HINT_ENTITY and status to HINT_READY and adds the entities collected so far to the hints.
 * It sets hintsForcedOnly to false.
 * This function is used when we want to hint something at the end of a yacc rule.
 * In this case lookahead contains the location of the next token. 
 * If the cursor is immediately at the beginning of the lookahead symbol we do nothing
 * @param [in] one The first range
 * @param [in] lookahead The range occupied by the lookahead symbol of yacc.
 * @param [in] atEnd If true, a hint is produced if the cursor is immediately after 'one',
 * @returns True if the cursor is in this hintable place.*/
bool Csh::CheckEntityHintAfterPlusOne(const CshPos &one, const CshPos &lookahead, bool atEnd)
{
    if (!CheckHintAfterPlusOne(one, lookahead, atEnd, HINT_ENTITY)) return false;
    AddEntitiesToHints();
    hintStatus = HINT_READY;
    return true;
}



/** Check if a hint has been previously located with specific properties.
 *
 * If the hint had been located and its location is fully inside the "location_to_check" 
 * and its type equals to "ht" we set its status to HINT_READY and return true.*/
bool Csh::CheckHintLocated(EHintType ht, const CshPos &location_to_check)
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

std::string Csh::HintPrefix(EColorSyntaxType t) const
{
    CurrentState state;
    state.Apply(MscCshAppearanceList[cshScheme][t]);
    return state.Print(false);
}

/** Callback for drawing a symbol before attribute names in the hints popup list box.
 * @ingroup libmscgen_hintpopup_callbacks*/
bool CshHintGraphicCallbackForAttributeNames(Canvas *canvas, CshHintGraphicParam /*p*/, Csh &)
{
    if (!canvas) return false;
    const double w = 0.4*HINT_GRAPHIC_SIZE_X;
    const double h = 0.08*HINT_GRAPHIC_SIZE_Y;
    const double off = 0.35*HINT_GRAPHIC_SIZE_Y;
    ColorType color(0, 0, 0);
    LineAttr line;
    line.radius.second = 3;
    FillAttr fill(color, GRADIENT_NONE);
    canvas->Fill(XY((HINT_GRAPHIC_SIZE_X-w)/2, off), XY((HINT_GRAPHIC_SIZE_X+w)/2, off+h), line, fill);
    canvas->Fill(XY((HINT_GRAPHIC_SIZE_X-w)/2, HINT_GRAPHIC_SIZE_Y-off-h), XY((HINT_GRAPHIC_SIZE_X+w)/2, HINT_GRAPHIC_SIZE_Y-off), line, fill);
    return true;
}

/** Insert a hint to the list of hints.*/
void Csh::AddToHints(CshHint &&h) 
{
    if (hintStatus == HINT_READY) return; //we add no more
    if (h.callback==NULL && h.type == HINT_ATTR_NAME) {
        h.callback = CshHintGraphicCallbackForAttributeNames;
        h.param = 0; 
    }
    Hints.insert(std::move(h));
}

/** Append a bunch of hints to the hint list.
 * 
 * @param [in] names The text of the hints in a 2D char array. The last one shall be "".
 * @param [in] prefix A string to prepend to each hint.
 * @param [in] t The type of the hints.
 * @param [in] c The callback function to use. The index of the hints in 'names' will be passed as parameter to the callback. */
void Csh::AddToHints(const char names[][ENUM_STRING_LEN], const string &prefix, EHintType t, 
                     CshHintGraphicCallback c)
{
    //index==0 is usually "invalid"
    for (unsigned i=1; names[i][0]; i++)
        AddToHints(CshHint(prefix+names[i], t, true, c, CshHintGraphicParam(i)));
}

/** Append a bunch of hints to the hint list.
 * 
 * @param [in] names The text of the hints in a 2D char array. The last one shall be "".
 * @param [in] prefix A string to prepend to each hint.
 * @param [in] t The type of the hints.
 * @param [in] c The callback function to use. 
 * @param [in] p The parameter to pass to the callback.*/
void Csh::AddToHints(const char names[][ENUM_STRING_LEN], const string &prefix, EHintType t, 
                     CshHintGraphicCallback c, CshHintGraphicParam p)
{
    //index==0 is usually "invalid"
    for (unsigned i=1; names[i][0]; i++)
        AddToHints(CshHint(prefix+names[i], t, true, c, p));
}

/** Callback for drawing a symbol before color names in the hints popup list box.
 * @ingroup libmscgen_hintpopup_callbacks*/
bool CshHintGraphicCallbackForColors(Canvas *canvas, CshHintGraphicParam p, Csh &)
{
    if (!canvas) return false;
    const int size = HINT_GRAPHIC_SIZE_Y-3;
    const int off_x = (HINT_GRAPHIC_SIZE_X - size)/2;
    const int off_y = 1;
    ColorType color(p);
    Block b(XY(off_x, off_y), XY(off_x+size, off_y+size));
    b.Round();
    if (color.a<255) {
        FillAttr fill(ColorType(255,255,255), GRADIENT_NONE);
        canvas->Fill(b.Centroid(), b.UpperLeft(), fill);
        canvas->Fill(b.Centroid(), b.LowerRight(), fill);
        fill.color.second = ColorType(196,196,196);
        canvas->Fill(b.Centroid(), b.UpperRight(), fill);
        canvas->Fill(b.Centroid(), b.LowerLeft(), fill);
    }
    canvas->Fill(b, FillAttr(color, GRADIENT_NONE));
    b.Expand(0.5);
    canvas->Line(b, LineAttr(LINE_SOLID, ColorType(0,0,0), 1, CORNER_NONE, 0));
    return true;
}

/** Add colors available at the cursor to the list of hints.
 * @param [in] define If true we hint after a 'defcolor' command. 
 *                    Else we hint at a value of a 'color' attr.
 *                    In the second case we add explanatory hints
 *                    on color sytnax definition. In the first
 *                    case we add an explanatory hint to say any new
 *                    color name can be used. */
void Csh::AddColorValuesToHints(bool define)
{
     if (define) {
         AddToHints(CshHint(HintPrefixNonSelectable()+"new color name to define", HINT_ATTR_VALUE, false));
     } else {
         AddToHints(CshHint(HintPrefixNonSelectable()+"<\"red,green,blue\">", HINT_ATTR_VALUE, false));
         AddToHints(CshHint(HintPrefixNonSelectable()+"<\"red,green,blue,opacity\">", HINT_ATTR_VALUE, false));
         AddToHints(CshHint(HintPrefixNonSelectable()+"<\"++red,green,blue,opacity\">", HINT_ATTR_VALUE, false));
         AddToHints(CshHint(HintPrefixNonSelectable()+"<\"color name,opacity\">", HINT_ATTR_VALUE, false));
         AddToHints(CshHint(HintPrefixNonSelectable()+"<\"++color name,opacity\">", HINT_ATTR_VALUE, false));
         AddToHints(CshHint(HintPrefixNonSelectable()+"<\"color name+-brightness%\">", HINT_ATTR_VALUE, false));
     }
    CshHint hint("", HINT_ATTR_VALUE, true, CshHintGraphicCallbackForColors, 0);
    for (auto i=Contexts.back().Colors.begin(); i!=Contexts.back().Colors.end(); i++) {
        hint.decorated = HintPrefix(COLOR_ATTRVALUE) + i->first;
        hint.param = i->second.ConvertToUnsigned();
        AddToHints(hint);
    }
}

/** Callback for drawing a symbol before design names in the hints popup list box.
 * @ingroup libmscgen_hintpopup_callbacks*/
bool CshHintGraphicCallbackForDesigns(Canvas *canvas, CshHintGraphicParam /*p*/, Csh &)
{
    if (!canvas) return false;
    const XY ul(0.2*HINT_GRAPHIC_SIZE_X, 0.2*HINT_GRAPHIC_SIZE_Y);
    const XY br(0.8*HINT_GRAPHIC_SIZE_X, 0.8*HINT_GRAPHIC_SIZE_Y);
    ColorType color(0, 0, 0);
    LineAttr line;
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

/** Add design available at the cursor to the list of hints.
 * @param [in] full Decides if full or partial design names shall be added.*/
void Csh::AddDesignsToHints(bool full)
{
    for (auto i= (full ? FullDesigns : PartialDesigns).begin(); i!=(full ? FullDesigns : PartialDesigns).end(); i++)
        Hints.insert(CshHint(HintPrefix(COLOR_ATTRVALUE) + i->first, HINT_ATTR_VALUE, true, CshHintGraphicCallbackForDesigns));
}

/** Callback for drawing a symbol before style names in the hints popup list box.
 * @ingroup libmscgen_hintpopup_callbacks*/
bool CshHintGraphicCallbackForStyles(Canvas *canvas, CshHintGraphicParam, Csh &)
{
    if (!canvas) return false;
    LineAttr line(LINE_SOLID, ColorType(0,0,0), 1, CORNER_ROUND, 1);
    FillAttr fill(ColorType(0,255,0), GRADIENT_UP);
    ShadowAttr shadow(ColorType(0,0,0));
    shadow.offset.first=true;
    shadow.offset.second=2;
    shadow.blur.first=true;
    shadow.blur.second=0;

    Block b(HINT_GRAPHIC_SIZE_X*0.1, HINT_GRAPHIC_SIZE_X*0.5, HINT_GRAPHIC_SIZE_Y*0.1, HINT_GRAPHIC_SIZE_Y*0.5);
    canvas->Fill(b, line, fill);
    canvas->Line(b, line);

    b.Shift(XY(HINT_GRAPHIC_SIZE_X*0.15, HINT_GRAPHIC_SIZE_X*0.15));
    fill.color.second = ColorType(255,0,0);
    canvas->Fill(b, fill);
    canvas->Line(b, line);

    b.Shift(XY(HINT_GRAPHIC_SIZE_X*0.15, HINT_GRAPHIC_SIZE_X*0.15));
    fill.color.second = ColorType(0,0,255);
    canvas->Shadow(b, shadow);
    canvas->Fill(b, fill);
    canvas->Line(b, line);
    return true;
}

/** Callback for drawing a symbol before style names in the hints popup list box.
 * @ingroup libmscgen_hintpopup_callbacks*/
bool CshHintGraphicCallbackForStyles2(Canvas *canvas, CshHintGraphicParam, Csh &)
{
    if (!canvas) return false;
    std::vector<double> xPos(2); 
    xPos[0] = HINT_GRAPHIC_SIZE_X*0.2;
    xPos[1] = HINT_GRAPHIC_SIZE_X*0.8;
    canvas->Clip(XY(1,1), XY(HINT_GRAPHIC_SIZE_X-1, HINT_GRAPHIC_SIZE_Y-1));
    ArrowHead ah(ArrowHead::BIGARROW);
    ah.line += ColorType(0,0,0); //black
    ah.endType.second = MSC_ARROW_SOLID;
    ah.size.second = MSC_ARROWS_INVALID;
    ShadowAttr shadow;
    FillAttr fill(ColorType(0,255,0), GRADIENT_UP);
    std::vector<double> active(2,0.);
    ah.BigCalculateAndDraw(xPos, active, HINT_GRAPHIC_SIZE_Y*0.3, HINT_GRAPHIC_SIZE_Y*0.7, 
                           true, false, fill, shadow, *canvas);
    canvas->UnClip();
    return true;
}

/** Add styles available at the cursor to the list of hints. */
void Csh::AddStylesToHints(bool include_forbidden)
{
    for (auto i=Contexts.back().StyleNames.begin(); i!=Contexts.back().StyleNames.end(); i++)
        if (include_forbidden || ForbiddenStyles.find(*i) == ForbiddenStyles.end())
            AddToHints(CshHint(HintPrefix(COLOR_STYLENAME) + *i, HINT_ATTR_VALUE, true, CshHintGraphicCallbackForStyles));
}

/** Add chart option names to the list of hints. */
void Csh::AddOptionsToHints() 
{
    Msc::AttributeNames(*this, false);
}

/** Add names of chart options that are valid inside a design definition to the list of hints. */
void Csh::AddDesignOptionsToHints() 
{
    Msc::AttributeNames(*this, true);
}

/** Callback for drawing a symbol before keywords in the hints popup list box.
 * @ingroup libmscgen_hintpopup_callbacks*/
bool CshHintGraphicCallbackForKeywords(Canvas *canvas, CshHintGraphicParam, Csh &)
{
    if (!canvas) return false;
    ColorType color(128, 64, 64);
    canvas->Clip(Contour(XY(HINT_GRAPHIC_SIZE_X/2, HINT_GRAPHIC_SIZE_Y/2), HINT_GRAPHIC_SIZE_Y*0.4));
    canvas->Fill(XY(0,0), XY(HINT_GRAPHIC_SIZE_X, HINT_GRAPHIC_SIZE_Y), FillAttr(color, GRADIENT_DOWN));
    canvas->UnClip();
    return true;
}

/** Add keywords to the list of hints. */
void Csh::AddKeywordsToHints(bool includeParallel)
{
    AddToHints(keyword_names+1-(includeParallel?1:0), HintPrefix(COLOR_KEYWORD), HINT_ATTR_VALUE, 
               CshHintGraphicCallbackForKeywords);
}

/** Callback for drawing a symbol before entities in the hints popup list box.
 * @ingroup libmscgen_hintpopup_callbacks*/
bool CshHintGraphicCallbackForEntities(Canvas *canvas, CshHintGraphicParam /*p*/, Csh &)
{
    if (!canvas) return false;
    LineAttr line(LINE_SOLID, ColorType(0,0,0), 1, CORNER_NONE, 0);
    LineAttr vline(LINE_SOLID, ColorType(0,0,0), 2, CORNER_NONE, 0);
    FillAttr fill(ColorType(192,192,192), GRADIENT_UP);
    ShadowAttr shadow(ColorType(0,0,0));
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

/** Add entities defined up to now to the list of hints. */
void Csh::AddEntitiesToHints()
{
    for (auto i=EntityNames.begin(); i!=EntityNames.end(); i++)
        AddToHints(CshHint(HintPrefix(COLOR_ENTITYNAME) + *i, HINT_ENTITY, true, CshHintGraphicCallbackForEntities));
}

/** Post-process the list of hints.
 * 
 * Fill in the 'size' and 'plain' members.
 * Fill and compact the list. Compact means here that entries having the same beginning
 * up to a dot will be combined to a single hint.
 * For example, if we have two hints "line.width" and "line.color" and the user has so
 * far typed "lin" we compact the two into "line.*". If, however, the user has already
 * typed "line.w" we do not compact (but perhaps filter line.color away if filterin is on).
 * Hints are also sorted.
 * @param [in] canvas A canvas to use at size calculation.
 * @param [in] format The text format to use at size calculation.
 * @param [in] uc The string under the cursor. Used as prefix for filtering and grouping.
 * @param [in] filter_by_uc If true we remove those hints that do not start by 'uc'.
 * @param [in] compact_same We do compaction as described above if true.
 */
void Csh::ProcessHints(Canvas &canvas, StringFormat *format, const std::string &uc, 
                       bool filter_by_uc, bool compact_same)
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
        if (compact_same && (i->type == HINT_ATTR_NAME || i->type == HINT_ATTR_VALUE)) {
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

/** Helper to find a prefix in a set of strings.
 * returns -1 if txt is ""
 * returns 0 if txt is not in coll
 * returns 1 if txt is a prefix of something in coll, but not equals anything
 * returns 2 if txt equals to something in coll*/
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
