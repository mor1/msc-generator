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

/** Calculates the difference between two CshEntries list and stores into 'this'.
*  That is what is new in new_list compared to old_list.
*  Entries that have disappeared will be retained using 'neutral' color.
* Assumes that both input lists are ordered and non-overlapping and this is,
* how the resulting list will be.*/
void CshListType::DiffInto(const CshListType &old_list, const CshListType &new_list,
                            EColorSyntaxType neutral)
{
    //A reminder about positions: first_pos==last_pos indicates a single char range.
    //We assume here that neither old_list nor new_list have overlapping entries
    //and we will produce a delta which also has none.
    auto o = old_list.begin();
    auto n = new_list.begin();
    int done_till = 0; //indicates the index beyond the last position we have handled in 'delta'
    while (o!=old_list.end() && n!= new_list.end()) {
        //ignore empty ranges in the old list (which can happen due to delete)
        //also ignore if the complete old entry is before we have completely OK
        if (o->first_pos > o->last_pos || o->last_pos < done_till) {
            o++;
            continue;
        }
        const int o_first = std::max(done_till, o->first_pos);
        if (o_first == n->first_pos) {
            if (o->color != n->color) //begins at same position, but different color: keep new version
                AddToBack(*n);
            else if (o->last_pos==n->last_pos)
                o++; //equal entries - sort of fast path
            else if (o->last_pos<n->last_pos)
                //old entry is shorter, add that part of 'n' that
                //is beyond of 'o' 
                AddToBack(CshEntry(o->last_pos+1, n->last_pos, n->color));
            done_till = n->last_pos+1;
            n++;
        } else if (o_first < n->first_pos) {
            //a (perhaps partially) removed item: paint with normal color
            //up to the beginning of 'n'
            done_till = std::min(o->last_pos+1, n->first_pos);
            AddToBack(CshEntry(o_first, done_till-1, neutral));
            if (done_till > o->last_pos)
                o++;
        } else {//n->first_pos < o->first_pos
            AddToBack(*n);
            done_till = n->last_pos+1;
            n++;
        }
    }
    while (o!=old_list.end()) {
        if (o->last_pos >= done_till)
            AddToBack(CshEntry(std::max(done_till, o->first_pos), o->last_pos, neutral));
        o++;
    }
    while (n!=new_list.end()) {
        AddToBack(*n);
        n++;
    }
}

/** Checks if all elements are sorted and non-overlapping. 
 *  Used in debug mode only.*/
bool CshListType::CheckOrderedAndNonOverlapping() const
{
    for (unsigned u = 1; u<size(); u++)
        if (at(u-1).last_pos >= at(u).first_pos)
            return false;
    return true;
}

/** Checks if all elements are either COLOR_ERROR or COLOR_MAX
*  Used in debug mode only.*/
bool CshListType::CheckIfErrorOrMaxColorsOnly() const
{
    for (auto &e : *this)
        if (e.color!=COLOR_ERROR && e.color!=COLOR_MAX)
            return false;
    return true;
}


/** Helper to add 
 * - checks if we have alread added the error or not
 * - appends a new entry to error_texts with correct pos, but not text.
 * - Adds 'pos' to error_ranges.
 * Returns true if we have have created a new text entry. */
bool CshErrorList::AddHelper(CshPos pos, const char *t)
{
    //check that we do not add the same error twice
    for (const auto &e : error_texts)
        if (e.first_pos == pos.first_pos && e.text==t)
            return false;
    error_texts.resize(error_texts.size()+1);
    error_texts.back().first_pos = pos.first_pos;

    //if (error_ranges.size()==0) {
    //    error_ranges.emplace_back(pos, COLOR_ERROR);
    //    return true;
    //}
    //Now insert 'pos'
    int i = error_ranges.size()-1;
    while (i>=0 && error_ranges[i].first_pos>pos.last_pos)
        i--;
    int j = i; 
    while (j>=0 && error_ranges[j].last_pos>=pos.first_pos)
        pos += error_ranges[j--];
    //now j is *before* the range of elements
    //we need to replace to pos, whereas i is the last element of that range. 
    //If i==j, pos does not overlap with any entries and needs to be 
    //inserted *after* j.
    //Note: both 'i' and 'j' can be -1.
    if (i==j) {
        error_ranges.emplace(error_ranges.begin()+(j+1), pos, COLOR_ERROR);
    } else {
        error_ranges[i].first_pos = pos.first_pos;
        error_ranges[i].last_pos = pos.last_pos;
        error_ranges[i].color = COLOR_ERROR;
        if (i-j > 1) 
            error_ranges.erase(error_ranges.begin()+(j+1), error_ranges.begin()+i); //deletes parts before i
    }
    _ASSERT(error_ranges.CheckOrderedAndNonOverlapping());
    return true;
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
    //Some others are needed for smart indent (LABEL_TEXT, COLON)
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
    l[0][COLOR_KEYWORD_MSCGEN].SetColor(50, 50, 50); l[0][COLOR_KEYWORD_MSCGEN].effects = COLOR_FLAG_BOLD | COLOR_FLAG_ITALICS;
    l[0][COLOR_ATTRNAME].          SetColor(  0,  0,  0); l[0][COLOR_ATTRNAME].effects = COLOR_FLAG_BOLD;
    l[0][COLOR_ATTRNAME_PARTIAL].  SetColor(  0,  0,  0); l[0][COLOR_ATTRNAME_PARTIAL].effects = 0;
    l[0][COLOR_ATTRNAME_MSCGEN].   SetColor( 50, 50, 50); l[0][COLOR_ATTRNAME_MSCGEN].effects = COLOR_FLAG_BOLD | COLOR_FLAG_ITALICS;
    l[0][COLOR_OPTIONNAME].        SetColor(  0,  0,  0); l[0][COLOR_OPTIONNAME].effects = COLOR_FLAG_BOLD;
    l[0][COLOR_OPTIONNAME_PARTIAL].SetColor(  0,  0,  0); l[0][COLOR_OPTIONNAME_PARTIAL].effects = 0;
    l[0][COLOR_OPTIONNAME_MSCGEN]. SetColor( 50, 50, 50); l[0][COLOR_OPTIONNAME_MSCGEN].effects = COLOR_FLAG_BOLD | COLOR_FLAG_ITALICS;
    l[0][COLOR_EQUAL].             SetColor(  0,  0,  0); l[0][COLOR_EQUAL].effects = 0;
    l[0][COLOR_SEMICOLON].         SetColor(  0,  0,  0); l[0][COLOR_SEMICOLON].effects = 0;
    l[0][COLOR_COLON].             SetColor(  0,  0,  0); l[0][COLOR_COLON].effects = 0;
    l[0][COLOR_BRACE].             SetColor(  0,  0,  0); l[0][COLOR_BRACE].effects = 0;
    l[0][COLOR_BRACKET].           SetColor(  0,  0,  0); l[0][COLOR_BRACKET].effects = 0;
    l[0][COLOR_SYMBOL].            SetColor( 20, 20,  0); l[0][COLOR_SYMBOL].effects = COLOR_FLAG_BOLD;
    l[0][COLOR_SYMBOL_MSCGEN].     SetColor( 80, 80,  0); l[0][COLOR_SYMBOL_MSCGEN].effects = COLOR_FLAG_BOLD;
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
    l[1][COLOR_KEYWORD_MSCGEN].    SetColor(178,178,  0); l[1][COLOR_KEYWORD_MSCGEN].effects = COLOR_FLAG_BOLD | COLOR_FLAG_ITALICS;
    l[1][COLOR_ATTRNAME].          SetColor(128,128,  0); l[1][COLOR_ATTRNAME].effects = COLOR_FLAG_BOLD;
    l[1][COLOR_ATTRNAME_PARTIAL].  SetColor(128,128,  0); l[1][COLOR_ATTRNAME_PARTIAL].effects = 0;
    l[1][COLOR_ATTRNAME_MSCGEN].   SetColor(178,178,  0); l[1][COLOR_ATTRNAME_MSCGEN].effects = COLOR_FLAG_BOLD | COLOR_FLAG_ITALICS;
    l[1][COLOR_OPTIONNAME].        SetColor(128,128,  0); l[1][COLOR_OPTIONNAME].effects = COLOR_FLAG_BOLD;
    l[1][COLOR_OPTIONNAME_PARTIAL].SetColor(128,128,  0); l[1][COLOR_OPTIONNAME_PARTIAL].effects = 0;
    l[1][COLOR_OPTIONNAME_MSCGEN]. SetColor(178,178,  0); l[1][COLOR_OPTIONNAME_MSCGEN].effects = COLOR_FLAG_BOLD | COLOR_FLAG_ITALICS;
    l[1][COLOR_EQUAL].             SetColor(  0,  0,  0); l[1][COLOR_EQUAL].effects = 0;
    l[1][COLOR_SEMICOLON].         SetColor(  0,  0,  0); l[1][COLOR_SEMICOLON].effects = 0;
    l[1][COLOR_COLON].             SetColor(  0,  0,  0); l[1][COLOR_COLON].effects = 0;
    l[1][COLOR_BRACE].             SetColor(  0,  0,  0); l[1][COLOR_BRACE].effects = 0;
    l[1][COLOR_BRACKET].           SetColor(  0,  0,  0); l[1][COLOR_BRACKET].effects = 0;
    l[1][COLOR_SYMBOL].            SetColor(255,  0,  0); l[1][COLOR_SYMBOL].effects = COLOR_FLAG_BOLD;
    l[1][COLOR_SYMBOL_MSCGEN].     SetColor(255, 50, 50); l[1][COLOR_SYMBOL_MSCGEN].effects = COLOR_FLAG_BOLD;
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
    l[1][COLOR_KEYWORD_MSCGEN].    SetColor(178,178,  0); l[1][COLOR_KEYWORD_MSCGEN].effects = COLOR_FLAG_BOLD | COLOR_FLAG_ITALICS;
    l[2][COLOR_ATTRNAME].          SetColor(128,128,  0); l[2][COLOR_ATTRNAME].effects = COLOR_FLAG_BOLD;
    l[2][COLOR_ATTRNAME_PARTIAL].  SetColor(128,128,  0); l[2][COLOR_ATTRNAME_PARTIAL].effects = 0;
    l[1][COLOR_ATTRNAME_MSCGEN].   SetColor(178,178,  0); l[1][COLOR_ATTRNAME_MSCGEN].effects = COLOR_FLAG_BOLD | COLOR_FLAG_ITALICS;
    l[2][COLOR_OPTIONNAME].        SetColor(128,128,  0); l[2][COLOR_OPTIONNAME].effects = COLOR_FLAG_BOLD;
    l[2][COLOR_OPTIONNAME_PARTIAL].SetColor(128,128,  0); l[2][COLOR_OPTIONNAME_PARTIAL].effects = 0;
    l[1][COLOR_OPTIONNAME_MSCGEN]. SetColor(178,178,  0); l[1][COLOR_OPTIONNAME_MSCGEN].effects = COLOR_FLAG_BOLD | COLOR_FLAG_ITALICS;
    l[2][COLOR_EQUAL].             SetColor(  0,  0,  0); l[2][COLOR_EQUAL].effects = 0;
    l[2][COLOR_SEMICOLON].         SetColor(  0,  0,  0); l[2][COLOR_SEMICOLON].effects = 0;
    l[2][COLOR_COLON].             SetColor(  0,  0,  0); l[2][COLOR_COLON].effects = 0;
    l[2][COLOR_BRACE].             SetColor(  0,  0,  0); l[2][COLOR_BRACE].effects = 0;
    l[2][COLOR_BRACKET].           SetColor(  0,  0,  0); l[2][COLOR_BRACKET].effects = 0;
    l[2][COLOR_SYMBOL].            SetColor(  0,128,128); l[2][COLOR_SYMBOL].effects = COLOR_FLAG_BOLD;
    l[2][COLOR_SYMBOL_MSCGEN].     SetColor(  0,178,178); l[2][COLOR_SYMBOL_MSCGEN].effects = COLOR_FLAG_BOLD;
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
    l[0][COLOR_KEYWORD].           SetColor(  0,  0,  0); l[0][COLOR_KEYWORD].effects = COLOR_FLAG_BOLD;
    l[0][COLOR_KEYWORD_PARTIAL].   SetColor(  0,  0,  0); l[0][COLOR_KEYWORD_PARTIAL].effects = 0;
    l[0][COLOR_KEYWORD_MSCGEN].    SetColor( 50, 50, 50); l[0][COLOR_KEYWORD_MSCGEN].effects = COLOR_FLAG_BOLD | COLOR_FLAG_ITALICS;
    l[0][COLOR_ATTRNAME].          SetColor(  0,  0,  0); l[0][COLOR_ATTRNAME].effects = COLOR_FLAG_BOLD;
    l[0][COLOR_ATTRNAME_PARTIAL].  SetColor(  0,  0,  0); l[0][COLOR_ATTRNAME_PARTIAL].effects = 0;
    l[0][COLOR_ATTRNAME_MSCGEN].   SetColor( 50, 50, 50); l[0][COLOR_ATTRNAME_MSCGEN].effects = COLOR_FLAG_BOLD | COLOR_FLAG_ITALICS;
    l[0][COLOR_OPTIONNAME].        SetColor(  0,  0,  0); l[0][COLOR_OPTIONNAME].effects = COLOR_FLAG_BOLD;
    l[0][COLOR_OPTIONNAME_PARTIAL].SetColor(  0,  0,  0); l[0][COLOR_OPTIONNAME_PARTIAL].effects = 0;
    l[0][COLOR_OPTIONNAME_MSCGEN]. SetColor( 50, 50, 50); l[0][COLOR_OPTIONNAME_MSCGEN].effects = COLOR_FLAG_BOLD | COLOR_FLAG_ITALICS;
    l[3][COLOR_EQUAL].             SetColor(  0,  0,  0); l[3][COLOR_EQUAL].effects = 0;
    l[3][COLOR_SEMICOLON].         SetColor(  0,  0,  0); l[3][COLOR_SEMICOLON].effects = 0;
    l[3][COLOR_COLON].             SetColor(  0,  0,  0); l[3][COLOR_COLON].effects = 0;
    l[3][COLOR_BRACE].             SetColor(  0,  0,  0); l[3][COLOR_BRACE].effects = 0;
    l[3][COLOR_BRACKET].           SetColor(  0,  0,  0); l[3][COLOR_BRACKET].effects = 0;
    l[3][COLOR_SYMBOL].            SetColor( 20, 20,  0); l[3][COLOR_SYMBOL].effects = COLOR_FLAG_BOLD;
    l[3][COLOR_SYMBOL].            SetColor( 80, 80,  0); l[3][COLOR_SYMBOL_MSCGEN].effects = COLOR_FLAG_BOLD;
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
    string ret(input, len);
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
    std::swap(description, o.description);
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

/** Extract CSH entries for attribute values.
 * At pos there is an attribute value. 
 * If the attribute name indicates a label, color the escapes, too. 
 * Not used for colon labels.
 * returns true, if the cursor is at a place to add escape hints.*/
void Csh::AddCSH_AttrValue_CheckAndAddEscapeHint(const CshPos& pos, const char *value, const char *name)
{
    if (!name || CaseInsensitiveEqual(name, "label") ||
        CaseInsensitiveEqual(name, "tag") ||
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
        const EEscapeHintType hint = 
            StringFormat::ExtractCSH(pos.first_pos+1, value, strlen(value), *this);
        if (hint != HINTE_NONE) {
            AddEscapesToHints(hint);
            hintSource = EHintSourceType::ESCAPE;
            hintStatus = HINT_READY;
            hintsForcedOnly = false;
        }
    } else {
        // No match - regular attribute value
        AddCSH(pos, COLOR_ATTRVALUE);
        //Register reference names
        if (CaseInsensitiveEqual(name, "refname"))
            RefNames.insert(value);
    }
}

void Csh::AddCSH_AttrColorValue(const CshPos& pos)
{
    AddCSH(pos, COLOR_ATTRVALUE);
}

/** Parse a colon-label for CSH entries and checks if we shall provide escape hints.
 * This is called for a colon followed by a (quoted or unquoted) label. 
 * if unquoted is true, search for @# comments and color them so. 
 * (False for quoted colon strings.)
 * returns true, if the cursor is at a position to provide escape hints.*/
void Csh::AddCSH_ColonString_CheckAndAddEscapeHint(const CshPos& pos, const char *value, bool unquoted)
{
    EEscapeHintType ret = HINTE_NONE;
    CshPos colon = pos;
    colon.last_pos = colon.first_pos;
    AddCSH(colon, COLOR_COLON);
    if (unquoted) {
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
                        ret |= StringFormat::ExtractCSH(txt.first_pos, beginning, p-beginning, *this);
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
            ret |= StringFormat::ExtractCSH(txt.first_pos, beginning, p-beginning, *this); 
        }
    } else {
        //This is a quoted string
        CshPos p(pos);
        //search for heading quotation mark
        while (*value && *value!='"') {
            value++;
            p.first_pos++;
        }
        _ASSERT(*value); //we should not reach end - this function must be called like this only for qouted strings
        //check trailing quotation mark (may be missing)
        size_t len = strlen(value);
        if (len && value[len-1]=='"')
            len--;
        ret = StringFormat::ExtractCSH(p.first_pos, value, len, *this); //omit the colon and quotation marks
    }
    if (ret != HINTE_NONE) {
        AddEscapesToHints(ret);
        hintSource = EHintSourceType::ESCAPE;
        hintStatus = HINT_READY;
        hintsForcedOnly = false;
    }
}

/** Names and descriptions of keywords for coloring.
 *
 * All keywords shall be repeated here, not only known by the parser.
 * We color only these where keywords should come. We also give hints based on these.*/
static const char * const keyword_names[] =
{"", NULL,
"parallel", "If you prepend 'parallel' in front of an element, subseqent elements will be laid besides it (and not strictly below), if possible without overlap.",
"overlap", "If you prepend 'overlap' in front of an element, subseqent elements will be laid besides it even if overlap occurs. Useful for slanted arrows crossing each other.",
"block", "Use before an an arrow definition to create a block arrow, like 'block a->b;'.",
"pipe", "Create a pipe between two entitie.",
"nudge", "Add a bit of a vertical space to pust subsequent elements lower.",
"heading", "Force the display of an entity heading for each currently showing entity.",
"newpage", "Insert a page break here.",
"defstyle", "Define a new style or change existing styles.",
"defcolor", "Define or redefine a color.",
"defdesign", "Define a full or partial chart design.",
"vertical", "Add a vertical chart element, such as a brace or vertical arrow.",
"mark", "Note this vertical position and name it so that you can refer to it later in a vertical element definition.",
"show", "Turn on entities, so that they become visible.",
"hide", "Turn off entities, so that they do not show from now on.",
"activate", "Activate entities, so that their entity line becomes wider.",
"deactivate", "Deactivate entities, swicthing back to a single entity line.",
"bye", "Anything specified after this command is treated as comment and is ignored.",
"hspace", "Add (extra) horizontal spacing between two entities.",
"vspace", "Add vertical space pushing subsequent elements lower.",
"symbol", "Add a drawn element (arc, rectange, etc.) to the chart.",
"note", "Add a note (floating in a callout) to the previous chart element.",
"comment", "Add a comment (visible on the side) to the previous chart element.",
"endnote", "Add an endnote (visible at the bottom of the chart) referring to the previous chart element.",
/*"footnote", "Add footnote (visible at the bottom of the page) referring to the previous chart element.", */
"title", "Add a title to the chart.",
"subtitle", "Add a subtitle to a section of the chart.",
"defshape", "Define a new shape.",
"text at", "Draws free text.",
""};

/** Names and descriptions of keywords valid inside a design definition for coloring/hints.
*
* All keywords shall be repeated here, not only known by the parser.
* We color only these where keywords should come. We also give hints based on these.*/
static const char * const design_keyword_names[] =
{"", NULL,
"defstyle", "Define a new style or change existing styles (applicable only in this design).",
"defcolor", "Define or redefine a color. (applicable only in this design)",
""
};


/** Names of chart options for coloring.
 *
 * All options shall be repeated here, not only known by class Msc.
 * We color only these where options should come. We also give hints based on these.*/
static const char opt_names[][ENUM_STRING_LEN] =
{"msc", "hscale", "vspacing", "compress", "numbering", "indicator", "auto_heading",
"numbering.pre", "numbering.post", "numbering.append", "numbering.format",
"pedantic", "background.color", "background.color2", "background.gradient", 
"text.color", "text.bgcolor", "text.format", "text.ident", 
"text.font.face", "text.font.type", 
"text.bold", "text.italic", "text.underline", 
"text.gap.up", "text.gap.down", "text.gap.left", "text.gap.right",
"text.gap.spacing", "text.size.normal", "text.size.small", "text.wrap",
"comment.line.color", "comment.line.type", "comment.line.width", 
"comment.fill.color", "comment.fill.color2", "comment.fill.gradient",
"comment.side", 
"comment.text.color", "comment.text.bgcolor", "comment.text.format", "comment.text.ident", 
"comment.text.font.face", "comment.text.font.type", 
"comment.text.bold", "comment.text.italic", "comment.text.underline", 
"comment.text.gap.up", "comment.text.gap.down", "comment.text.gap.left", "comment.text.gap.right",
"comment.text.gap.spacing", "comment.text.size.normal", "comment.text.size.small", "comment.text.wrap",
"angle", /*"classic_parallel_layout", DEPRECATED*/ "file.url", "file.info", "width", ""};

/** Additional option names in mscgen compatibility mode*/
static const char opt_names_mscgen[][ENUM_STRING_LEN] =
{"arcgradient", "wordwraparcs", ""};


/** Names of attributes for coloring.
 *
 * All attribute shall be repeated here, not only known by the parser.
 * We color only these where attributes should come. 
 * We do not distinguish which arc the attribute belongs to.*/
static const char attr_names[][ENUM_STRING_LEN] =
{"draw_time", "vspacing", "compress", "color", "label", "refname", "number", "indicator", "collapsed",
"parallel", "overlap", "keep_with_next", "keep_together", "auto_heading",
"vertical_ident", "layout",
"pos", "relative", "show", "active", "makeroom", "side", "offset", "solid",
"text.color", "text.bgcolor", "text.ident", "ident", "text.format",
"text.font.face", "text.font.type", "text.link_format",
"text.bold", "text.italic", "text.underline",
"text.gap.up", "text.gap.down", "text.gap.left", "text.gap.right",
"text.gap.spacing", "text.size.normal", "text.size.small", "text.wrap", "text.width",
"arrow", "arrowsize", "arrow.size", "arrow.type", "arrow.starttype", "arrow.midtype",
"arrow.skiptype", "arrow.endtype", "arrow.color", "arrow.xmul", "arrow.ymul",
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
"lost.text.color", "lost.text.bgcolor", "lost.text.ident", "ident", "lost.text.format",
"lost.text.font.face", "lost.text.font.type", "lost.text.link_format",
"lost.text.bold", "lost.text.italic", "lost.text.underline",
"lost.text.gap.up", "lost.text.gap.down", "lost.text.gap.left", "lost.text.gap.right",
"lost.text.gap.spacing", "lost.text.size.normal", "lost.text.size.small", "lost.text.wrap",
"shape", "shape.size",  //we add shape even if it may not be available
"tag", "tag.line.color", "tag.line.type", "tag.line.width", "tag.line.corner", "tag.line.radius",
"tag.fill.color", "tag.fill.color2", "tag.fill.gradient",
"tag.text.color", "tag.text.bgcolor", "tag.text.ident", "ident", "tag.text.format",
"tag.text.font.face", "tag.text.font.type", "tag.text.link_format",
"tag.text.bold", "tag.text.italic", "tag.text.underline",
"tag.text.gap.up", "tag.text.gap.down", "tag.text.gap.left", "tag.text.gap.right",
"tag.text.gap.spacing", "tag.text.size.normal", "tag.text.size.small", "tag.text.wrap", 
"tag.text.width", "url", ""};

/** Additional attribute names in mscgen compatibility mode*/
static const char attr_names_mscgen[][ENUM_STRING_LEN] =
{"id", "idurl", "arcskip",
"linecolor", "linecolour", "textcolor", "textcolour", "textbgcolor", "textbgcolour",
"arclinecolor", "arclinecolour", "arctextcolor", "arctextcolour", "arctextbgcolor", "arctextbgcolour",
""};


/** Names of symbols for coloring
 *
 * All keywords shall be repeated here, not only known by the parser.
 * We only color these as keywords where a symbol name is expected (after 'symbol' keyword).*/
static const char symbol_names[][ENUM_STRING_LEN] =
{"arc", "rectangle", "...", "text", ""};

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

/** Helper to check if 'name' us in the 'array' of string constants considering only every second element.*/
unsigned find_opt_attr_name_with_description(const char *name, const char * const array[])
{
    for (unsigned i = 0; array[i][0]; i += 2)
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
    unsigned match_result = find_opt_attr_name_with_description(name, keyword_names);
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
    const char(*array)[ENUM_STRING_LEN];
    if (color == COLOR_OPTIONNAME) array = opt_names;
    else if (color == COLOR_ATTRNAME) array = attr_names;
    else array = empty_names;
    unsigned match_result = find_opt_attr_name(name, array);
    bool mscgen_compat_match = false;

    //check mscgen-specific attributes/options
    if (color == COLOR_OPTIONNAME) array = opt_names_mscgen;
    else if (color == COLOR_ATTRNAME) array = attr_names_mscgen;
    else array = empty_names;
    unsigned match_result2 = find_opt_attr_name(name, array);
    //take the better of the two matches
    if (match_result2 > match_result) {
        match_result = match_result2;
        mscgen_compat_match = true;
    }
    //Honor partial matches only if cursor is right after
    if (pos.last_pos != cursor_pos && match_result == 1)
        match_result = 0;
    switch (match_result) {
    case 2: AddCSH(pos, mscgen_compat_match ? EColorSyntaxType(color+2) : color); return;
    case 0: AddCSH_Error(pos, color == COLOR_OPTIONNAME ? "Unkown chart option." : "Unknown attribute."); return;
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
    bool mscgen_compat_match = false;

    //check mscgen-specific attributes
    unsigned match_result2 = find_opt_attr_name(name, attr_names_mscgen);
    //take the better of the two matches
    if (match_result2 > match_result) {
        match_result = match_result2;
        mscgen_compat_match = true;
    }

    if (pos.last_pos == cursor_pos && match_result == 1) {
        AddCSH(pos, COLOR_ATTRNAME_PARTIAL);
        was_partial = true;
        return;
    }
    if (match_result == 2) {
        AddCSH(pos, mscgen_compat_match ? COLOR_ATTRNAME_MSCGEN : COLOR_ATTRNAME);
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

void Csh::AddCSH_LeftRightCenterMarker(const CshPos&pos, const char *name)
{
    switch (find_opt_attr_name(name, extvxpos_designator_names)) {
    case 2: //Full match
        AddCSH(pos, COLOR_KEYWORD); break;
    case 1: //partial match
        AddCSH(pos, COLOR_KEYWORD_PARTIAL); break;
    default:
        _ASSERT(0); //fallthrough
    case 0:
        AddCSH(pos, COLOR_MARKERNAME);
    }
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
    ResetMarkers();
    RefNames.clear();
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
            AddToHints(CshHint(HintPrefix(COLOR_MARKERNAME) + *i, 
            "Markers defined via the 'mark' command.", EHintType::MARKER,
            true, CshHintGraphicCallbackForMarkers));
        hintStatus = HINT_READY;
    }
    if (addRefNamesAtEnd) {
        hintStatus = HINT_FILLING;
        for (auto i = RefNames.begin(); i!=RefNames.end(); i++)
            AddToHints(CshHint(HintPrefix(COLOR_ATTRVALUE) + *i,
            "Reference names defined via the 'refname' attributes.", EHintType::ATTR_VALUE,
            true));
        hintStatus = HINT_READY;
    }
    if (hintStatus == HINT_FILLING) hintStatus = HINT_READY;
    //Take one from first, since RichEditCtrel works that way
    --hintedStringPos.first_pos;
    _ASSERT(hintStatus==HINT_READY || Hints.size()==0);
    if (hintStatus!=HINT_READY || Hints.size()==0)
        return;
}


const CshPos *Csh::IsInColonLabel(int pos) const
{
    //'pos' is in RichEdit units: index zero is before the first char.
    //the positions in 'Labels' is in csh units: the first char is
    //indexed 1, and for single character ranges first_pos==last_pos
    for (auto &p : ColonLabels)
        if (p.first_pos<=pos && p.last_pos>=pos)
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

Csh::Csh(const Context &defaultDesign, const ShapeCollection *shapes, 
         const std::set<string> *fn) :
    was_partial(false), 
    input_text_length(0),
    hadEscapeHint(false),
    hintStatus(HINT_NONE),
    addMarkersAtEnd(false), 
    addRefNamesAtEnd(false),
    pShapes(shapes),
    cursor_pos(-1),
    use_scheme(NULL), 
    fontnames(fn),
    mscgen_compat(EMscgenCompat::AUTODETECT)
{
    for (auto i=defaultDesign.styles.begin(); i!=defaultDesign.styles.end(); i++)
        ForbiddenStyles.insert(i->first);
    ForbiddenStyles.erase("weak");
    ForbiddenStyles.erase("strong");
    PushContext(true);
    Contexts.back().SetToDesign(defaultDesign);
    FullDesigns["plain"] = Contexts.back();
    ResetMarkers();
}

void Csh::ResetMarkers()
{
    MarkerNames.clear();
    MarkerNames.insert(MARKER_BUILTIN_CHART_TOP_STR);
    MarkerNames.insert(MARKER_BUILTIN_CHART_BOTTOM_STR);
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

/** If our compatibility mode is AUTODETECT, we switch to MSCGEN compatibility mode*/
void Csh::SwitchToMscgenCompatMode()
{
    if (mscgen_compat != EMscgenCompat::AUTODETECT) return;
    mscgen_compat = EMscgenCompat::FORCE_MSCGEN;
    SetDesignTo("mscgen", false);
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
 * @param [in] hsource The source type at the position in the file.
 * @param [in] a_name The name of the attribute if the hint type is HINT_ATTRVALUE
 * @returns True if the cursor is in this hintable place.*/
bool Csh::CheckHintBetween(const CshPos &one, const CshPos &two, 
                           EHintSourceType hsource, const char *a_name)
{
    switch (CursorIn(one.last_pos+1, two.first_pos-1)) {
    case CURSOR_AT_BEGINNING:
    case CURSOR_IN:
        hintStatus = HINT_LOCATED;
        hintsForcedOnly = false;
        hintSource = hsource;
        if (hsource == EHintSourceType::MARKER)
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
 * @param [in] hsource The source type at the position in the file.
 * @param [in] a_name The name of the attribute if the hint type is HINT_ATTRVALUE
 * @returns True if the cursor is in this hintable place.*/
bool Csh::CheckHintBetweenPlusOne(const CshPos &one, const CshPos &two,
                                  EHintSourceType hsource, const char *a_name)
{
    if  (CursorIn(one.last_pos+1, two.first_pos-1) == CURSOR_IN) {
        hintStatus = HINT_LOCATED;
        hintsForcedOnly = false;
        hintSource = hsource;
        if (hsource == EHintSourceType::MARKER)
            addMarkersAtEnd = true;
        hintAttrName = a_name ? a_name : "";
        return true;
    }
    return false;
}

/** Mark hint status to HINT_LOCATED if cursor is after a range.
 *
 * Checks if the cursor is between 'one' and 'lookahead'. If so, it applies
 * the hinttype with status HINT_LOCATED. It sets hintsForcedOnly to false.
 * If 'atEnd' is true we are at the very end of the file, 'one' is the last symbol and
 * 'lookahead' is ignored.
 * This function is used when we want to hint something at the end of a yacc rule.
 * In this case lookahead contains the location of the next token. 
 * If the cursor is immediately at the beginning of the lookahead symbol we do nothing.
 * @param [in] one The first range
 * @param [in] lookahead The range occupied by the lookahead symbol of yacc.
 * @param [in] atEnd If true, we are at the end of the file and no lookahead symbol is 
 *                   detected (parameter 'lookahead' is ignored). We just test if the cursor
 *                   is just after or well after 'one'. If false, we have a lookahead symbol
 *                   and we test if the cursor is just after 'one' or well after it, but 
 *                   strictly before or just before 'lookahead'.
 * @param [in] hsource The source type at the position in the file.
 * @param [in] a_name The name of the attribute if the hint type is HINT_ATTRVALUE
 * @returns True if the cursor is in this hintable place.*/
bool Csh::CheckHintAfter(const CshPos &one, const CshPos &lookahead, bool atEnd, 
                         EHintSourceType hsource, const char *a_name)
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
    hintSource = hsource;
    if (hsource == EHintSourceType::MARKER)
        addMarkersAtEnd = true;
    hintAttrName = a_name ? a_name : "";
    return true;
}

/** Mark hint status to HINT_LOCATED if cursor is after a range, but not immediately after.
 *
 * Checks if the cursor is between 'one' and 'lookahead', but not immediately after 'one'.
 * If so, it applies the hinttype with status HINT_LOCATED. It sets hintsForcedOnly to false.
 * If 'atEnd' is true we are at the very end of the file, 'one' is the last symbol and
 * 'lookahead' is ignored.
 * This function is used when we want to hint something at the end of a yacc rule.
 * In this case lookahead contains the location of the next token. 
 * If the cursor is immediately at the beginning of the lookahead symbol we do nothing.
 * @param [in] one The first range
 * @param [in] lookahead The range occupied by the lookahead symbol of yacc.
 * @param [in] atEnd If true, we are at the end of the file and no lookahead symbol is
 *                   detected (parameter 'lookahead' is ignored). We just test if the cursor
 *                   is just after or well after 'one'. If false, we have a lookahead symbol
 *                   and we test if the cursor is just after 'one' or well after it, but
 *                   strictly before or just before 'lookahead'.
 * @param [in] hsource The source type at the position in the file.
 * @param [in] a_name The name of the attribute if the hint type is HINT_ATTRVALUE
 * @returns True if the cursor is in this hintable place.*/
bool Csh::CheckHintAfterPlusOne(const CshPos &one, const CshPos &lookahead, bool atEnd, 
                                EHintSourceType hsource, const char *a_name)
{
    if (one.last_pos >= lookahead.first_pos) return false;
    CshPos one_oneAfter = one;
    one_oneAfter.last_pos++;
    return CheckHintAfter(one_oneAfter, lookahead, atEnd, hsource, a_name);
}

/** Mark hint status to HINT_LOCATED if cursor is between two ranges or inside the second one.
 *
 * Checks if the cursor is between the two ranges or inside the second. If so, it applies
 * the hinttype with status HINT_LOCATED. If cursor is inside two, hintedStringPos
 * is set to two. hintsForcedOnly is set to true iff the cursor is truely before two
 * or is at the end of two.
 * @param [in] one The first range
 * @param [in] two The second range
 * @param [in] hsource The source type at the position in the file.
 * @param [in] a_name The name of the attribute if the hint type is HINT_ATTRVALUE
 * @returns True if the cursor is in this hintable place.*/
bool Csh::CheckHintAtAndBefore(const CshPos &one, const CshPos &two, 
                               EHintSourceType hsource, const char *a_name)
{
    if (CursorIn(one.last_pos+1, two.last_pos)<=CURSOR_AFTER) return false;
    hintStatus = HINT_LOCATED;
    hintsForcedOnly = false;
    hintSource = hsource;
    if (hsource == EHintSourceType::MARKER)
        addMarkersAtEnd = true;
    hintAttrName = a_name ? a_name : "";
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
 * @param [in] hsource The source type at the position in the file.
 * @param [in] a_name The name of the attribute if the hint type is HINT_ATTRVALUE
 * @returns True if the cursor is in this hintable place.*/
bool Csh::CheckHintAtAndBeforePlusOne(const CshPos &one, const CshPos &two, 
                                      EHintSourceType hsource, const char *a_name)
{
    CshPos one_oneAfter = one;
    one_oneAfter.last_pos++;
    return CheckHintAtAndBefore(one_oneAfter, two, hsource, a_name);
}


/** Mark hint status to HINT_LOCATED if cursor is inside, just before or just after a range.
 *
 * Checks if the cursor is inside, just before or just after a range.
 * If so, it applies the hinttype with status HINT_LOCATED. hintedStringPos
 * is set to 'one'. hintsForcedOnly is set to true if the cursor is just before one
 * or is at the end of one
 * @param [in] one The range
 * @param [in] hsource The source type at the position in the file.
 * @param [in] a_name The name of the attribute if the hint type is HINT_ATTRVALUE
 * @returns True if the cursor is in this hintable place.*/
bool Csh::CheckHintAt(const CshPos &one, 
                      EHintSourceType hsource, const char *a_name)
{
    if (CursorIn(one)<=CURSOR_AFTER) return false;
    hintStatus = HINT_LOCATED;
    hintsForcedOnly = false;
    hintSource = hsource;
    if (hsource == EHintSourceType::MARKER)
        addMarkersAtEnd = true;
    hintAttrName = a_name ? a_name : "";
    hintedStringPos = one;
    return true;
}


/** Check if the cursor is anywhere before (or at the beginning of) 'pos'.
* If so, mark hint status as HINT_FILLING and hint type as HINT_LINE_START
*
* Checks if the cursor is well before or just before a range.
* If so, it applies the hinttype with status HINT_FILLING. hintedStringPos
* is set to 'pos' only if the cursor is at its beginning. 
* We set hintsForcedOnly to true, as such hints need Ctrl+Space.
* This function is used to check if the cursor is before any characters in the file.
* @param [in] pos The range to check 
* @returns True if the cursor is in this hintable place.*/
bool Csh::CheckLineStartHintBefore(const CshPos &pos)
{
    const ECursorRelPosType p = CursorIn(pos);
    if (p!=CURSOR_BEFORE && p!=CURSOR_AT_BEGINNING) return false;
    hintStatus = HINT_FILLING;
    hintsForcedOnly = true;
    hintSource = EHintSourceType::LINE_START;
    if (p==CURSOR_AT_BEGINNING)
        hintedStringPos = pos;
    return true;
}

/** Check if the cursor is anywhere in 'pos'.
* If so, mark hint status as HINT_FILLING and hint type as HINT_LINE_START
*
* Checks if the cursor just before, in or just after a range.
* If so, it applies the hinttype with status HINT_FILLING. hintedStringPos
* is set to 'pos'.
* @param [in] pos The range to check
* @returns True if the cursor is in this hintable place.*/
bool Csh::CheckLineStartHintAt(const CshPos &pos)
{
    const ECursorRelPosType p = CursorIn(pos);
    if (p<CURSOR_AT_BEGINNING) return false;
    hintStatus = HINT_FILLING;
    hintsForcedOnly = true;
    hintSource = EHintSourceType::LINE_START;
    hintedStringPos = pos;
    return true;
}

/** Mark hint type to HINT_ENTITY and status to HINT_FILLING if cursor is between two ranges.
*
* Checks if the cursor is between the two ranges. If so, it sets hint type to
* HINT_ENTITY and status to HINT_READY and adds the entities collected so far to the hints.
* If cursor is inside two, hintedStringPos
* is set to two. hintsForcedOnly is set to true iff the cursor is truely before two
* or is at the end of two.
* @param [in] one The first range
* @param [in] two The second range
* @returns True if the cursor is in this hintable place.*/
bool Csh::CheckEntityHintBetween(const CshPos &one, const CshPos &two)
{
    if (!CheckHintBetween(one, two, EHintSourceType::ENTITY))
        return false;
    AddEntitiesToHints();
    hintStatus = HINT_FILLING;
    return true;
}

/** Mark hint type to HINT_ENTITY and status to HINT_FILLING if cursor is between two ranges or inside the second one.
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
    if (!CheckHintAtAndBefore(one, two, EHintSourceType::ENTITY)) 
        return false;
    AddEntitiesToHints();
    hintStatus = HINT_FILLING;
    return true;
}

/** Mark hint type to HINT_ENTITY and status to HINT_FILLING if cursor is between two range or inside the second one, but not immediately after the first one.
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

/** Mark hint type to HINT_FILLING and status to HINT_FILLING if cursor is inside, just before or just after a range.
 *
 * Checks if the cursor is inside, just before or just after a range.
 * If so, sets hint type to 
 * HINT_ENTITY and status to HINT_FILLING and adds the entities collected so far to the hints.
 * hintedStringPos
 * is set to 'one'. hintsForcedOnly is set to true if the cursor is just before one
 * or is at the end of one
 * @param [in] one The range
 * @returns True if the cursor is in this hintable place.*/
bool Csh::CheckEntityHintAt(const CshPos &one)
{
    if (!CheckHintAt(one, EHintSourceType::ENTITY))
        return false;
    AddEntitiesToHints();
    hintStatus = HINT_FILLING;
    return true;
}

/** Mark hint type to HINT_ENTITY and status to HINT_FILLING if cursor is after a range.
 *
 * Checks if the cursor is between 'one' and 'lookahead'.
 * If 'atEnd' is true we trigger only if the cursor is immediately after
 *'one'.  If so, sets hint type to 
 * HINT_ENTITY and status to HINT_FILLING and adds the entities collected so far to the hints.
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
    if (!CheckHintAfter(one, lookahead, atEnd, EHintSourceType::ENTITY))
        return false;
    AddEntitiesToHints();
    hintStatus = HINT_READY;
    return true;
}


/** Mark hint type to HINT_ENTITY and status to HINT_FILLING if cursor is after a range, but not immediately after.
 *
 * Checks if the cursor is between 'one' and 'lookahead', but not immediately after 'one'.
 * If 'atEnd' is true we trigger only if the cursor is exactly one character after
 *'one'. If so, sets hint type to 
 * HINT_ENTITY and status to HINT_FILLING and adds the entities collected so far to the hints.
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
    if (!CheckHintAfterPlusOne(one, lookahead, atEnd, EHintSourceType::ENTITY))
        return false;
    AddEntitiesToHints();
    hintStatus = HINT_FILLING;
    return true;
}



/** Check if a hint has been previously located with specific properties.
 *
 * If the hint had been located and its location is fully inside the "location_to_check" 
 * and its type equals to "ht" we set its status to HINT_FILLING and return true.
 * After this one can add the hints.*/
bool Csh::CheckHintLocated(EHintSourceType hsource, const CshPos &location_to_check)
{
    if (hintStatus!=HINT_LOCATED || hintSource!=hsource)
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
    _ASSERT(hintStatus!=HINT_READY);
    if (hintStatus == HINT_READY) return; //we add no more
    if (h.callback==NULL && h.type == EHintType::ATTR_NAME) {
        h.callback = CshHintGraphicCallbackForAttributeNames;
        h.param = 0;
    }
    Hints.insert(std::move(h));
    if (h.type == EHintType::ESCAPE)
        hadEscapeHint = true;
    else {
        //when we add an escape hint, we should have only hints of escape type
        _ASSERT(!hadEscapeHint); 
    }
}

/** Append a bunch of hints to the hint list.
 * 
 * @param [in] names_descriptions The text of the hints and descriptions (alternating) in a char 
 *                                pointer array. The last hint shall be "".
 * @param [in] prefix A string to prepend to each hint.
 * @param [in] t The type of the hints.
 * @param [in] c The callback function to use. The index of the hints in 'names' will be passed as parameter to the callback. 
 * @param [in] in_order If true, elements will be attached a 'sort' key in the order they are added. (for HINT_ESCAPEs which do not sort alphabetically.)*/
void Csh::AddToHints(const char * const * names_descriptions,
                     const string &prefix, EHintType t,
                     CshHintGraphicCallback c, bool in_order)
{
    //index==0 is usually "invalid"
    for (unsigned i = 2; names_descriptions[i][0]; i += 2)
        AddToHints(CshHint(prefix+names_descriptions[i], names_descriptions[i+1],
                           t, true, c, CshHintGraphicParam(i), in_order ? i/2 : 0));
}

/** Append a bunch of hints to the hint list.
 * 
 * @param [in] names_descriptions The text of the hints and descriptions (alternating) in a char 
 *                                pointer array. The last hint shall be "".
 * @param [in] prefix A string to prepend to each hint.
 * @param [in] t The type of the hints.
 * @param [in] c The callback function to use. 
 * @param [in] p The parameter to pass to the callback.*/
void Csh::AddToHints(const char * const * names_descriptions,
                     const string &prefix, EHintType t,
                     CshHintGraphicCallback c, CshHintGraphicParam p)
{
    //index==0 is usually "invalid"
    for (unsigned i = 2; names_descriptions[i][0]; i += 2)
        AddToHints(CshHint(prefix+names_descriptions[i], names_descriptions[i+1],
                           t, true, c, p));
}

/** Append a bunch of hints to the hint list.
 * 
 * @param [in] names The text of the hints in a 2D char 
 *                   array. The last hint shall be "".
 * @param [in] descriptions The descriptions of the hints and in a char 
 *                          pointer array. At least as many as in 'names'.
 * @param [in] prefix A string to prepend to each hint.
 * @param [in] t The type of the hints.
 * @param [in] c The callback function to use. The index of the hints in 'names' will be passed as parameter to the callback. */
void Csh::AddToHints(const char names[][ENUM_STRING_LEN], const char * const descriptions[],
                     const string &prefix, EHintType t,
                     CshHintGraphicCallback c)
{
    //index==0 is usually "invalid"
    for (unsigned i = 1; names[i][0]; i++)
        AddToHints(CshHint(prefix+names[i], descriptions ? descriptions[i] : NULL,
                           t, true, c, CshHintGraphicParam(i)));
}

/** Append a bunch of hints to the hint list.
 * 
 * @param [in] names The text of the hints in a 2D char 
 *                   array. The last hint shall be "".
 * @param [in] descriptions The descriptions of the hints and in a char 
 *                          pointer array. At least as many as in 'names'.
 * @param [in] prefix A string to prepend to each hint.
 * @param [in] t The type of the hints.
 * @param [in] c The callback function to use. 
 * @param [in] p The parameter to pass to the callback.*/
void Csh::AddToHints(const char names[][ENUM_STRING_LEN], const char * const descriptions[],
                     const string &prefix, EHintType t,
                     CshHintGraphicCallback c, CshHintGraphicParam p)
{
    //index==0 is usually "invalid"
    for (unsigned i = 1; names[i][0]; i++)
        AddToHints(CshHint(prefix+names[i], descriptions ? descriptions[i] : NULL,
                           t, true, c, p));
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
         AddToHints(CshHint(HintPrefixNonSelectable()+"new color name to define", 
             "You can specify a new color name here if you want to define a new color.",
             EHintType::ATTR_VALUE, false));
     } else {
         AddToHints(CshHint(HintPrefixNonSelectable()+"<\"red,green,blue\">", 
             "You can specify the three components of an RGB color. They can be either integers between "
             "0..255 or floating point numbers between [0..1].",
             EHintType::ATTR_VALUE, false));
         AddToHints(CshHint(HintPrefixNonSelectable()+"<\"red,green,blue,opacity\">", 
             "You can specify the three components of an RGB color, plus an opacity value. They can be either integers between "
             "0..255 or floating point numbers between [0..1]. Opacity of zero means full transparency - nothing visible.", 
             EHintType::ATTR_VALUE, false));
         AddToHints(CshHint(HintPrefixNonSelectable()+"<\"++red,green,blue,opacity\">", 
             "Using the '++' prefix you can specify a transculent ovelay color, which will be overlaid on top of "
             "an existing color.",
             EHintType::ATTR_VALUE, false));
         AddToHints(CshHint(HintPrefixNonSelectable()+"<\"color name,opacity\">", 
             "You can make an existing color transparent by specifying an opacity value separated by a comma. "
             "It can be either an integer between [0..255] or a floating point number between [0..1]. "
             "Opacity of zero means full transparency - nothing visible.",
             EHintType::ATTR_VALUE, false));
         AddToHints(CshHint(HintPrefixNonSelectable()+"<\"++color name,opacity\">", 
             "Using the '++' prefix you can specify a transculent ovelay color, which will be overlaid on top of "
             "an existing color.",
             EHintType::ATTR_VALUE, false));
         AddToHints(CshHint(HintPrefixNonSelectable()+"<\"color name+-brightness%\">", 
             "You can take an existing color and make it lighter/darker like this.",
             EHintType::ATTR_VALUE, false));
     }
    CshHint hint("", "Apply this color.", EHintType::ATTR_VALUE, true, CshHintGraphicCallbackForColors, 0);
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
        Hints.insert(CshHint(HintPrefix(COLOR_ATTRVALUE) + i->first, 
                             full ? "Apply this full design to the chart." : "Apply this partial design to the chart.",
                             EHintType::ATTR_VALUE, true, CshHintGraphicCallbackForDesigns));
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

/** Add styles available at the cursor to the list of hints. 
 * @param [in] include_forbidden If true, we include the forbidden styles, too.
 *                               These include all default and refinement styles (like 'arrow' and '->'),
 *                               but not user-defined styles or 'strong' and 'weak'.
 * @param [in] define If true we formulate description appropriate for defstyle commands, else
 *                    as for when applying a style.*/
void Csh::AddStylesToHints(bool include_forbidden, bool define)
{
    for (auto i=Contexts.back().StyleNames.begin(); i!=Contexts.back().StyleNames.end(); i++)
        if (include_forbidden || ForbiddenStyles.find(*i) == ForbiddenStyles.end())
            AddToHints(CshHint(HintPrefix(COLOR_STYLENAME) + *i, 
                               define ? "Change this style." : "Apply this style.",
                               EHintType::ATTR_VALUE, true, CshHintGraphicCallbackForStyles));
}


/** Add the symbol types to the hints.*/
void Csh::AddSymbolTypesToHints()
{
    AddToHints(CshHint(HintPrefix(COLOR_KEYWORD) + "arc",
        "This draws a circle or ellipse.",
        EHintType::KEYWORD, true));
    AddToHints(CshHint(HintPrefix(COLOR_KEYWORD) + "rectangle",
        "This draws a rectangle.",
        EHintType::KEYWORD, true));
    AddToHints(CshHint(HintPrefix(COLOR_KEYWORD) + "...",
        "This draws three small circles one below another, a kind of vertical ellipsys.",
        EHintType::KEYWORD, true));
    AddToHints(CshHint(HintPrefix(COLOR_KEYWORD) + "text",
        "This draws just text.",
        EHintType::KEYWORD, true));
}

/** Add the left/right/center keyword (for symbol positioning) to the hints.*/
void Csh::AddLeftRightCenterToHints()
{
    AddToHints(CshHint(HintPrefix(COLOR_KEYWORD) + "left", 
        "Use this if you want to specify where the left edge shall be positioned.",
        EHintType::KEYWORD, true, CshHintGraphicCallbackForTextIdent,
        CshHintGraphicParam(MSC_IDENT_LEFT)));
    AddToHints(CshHint(HintPrefix(COLOR_KEYWORD) + "right", 
        "Use this if you want to specify where the right edge shall be positioned.",
        EHintType::KEYWORD, true, CshHintGraphicCallbackForTextIdent,
        CshHintGraphicParam(MSC_IDENT_RIGHT)));
    AddToHints(CshHint(HintPrefix(COLOR_KEYWORD) + "center",
        "Use this if you want to specify where the center shall be positioned.",
        EHintType::KEYWORD, true, CshHintGraphicCallbackForTextIdent,
        CshHintGraphicParam(MSC_IDENT_CENTER)));
}

/** Add the left/right keyword (for hspace specification) to the hints.*/
void Csh::AddLeftRightHSpaceToHints()
{
    AddToHints(CshHint(HintPrefix(COLOR_KEYWORD) + "left comment",
        "Use this to size the left comment area.",
        EHintType::KEYWORD));
    AddToHints(CshHint(HintPrefix(COLOR_KEYWORD) + "right comment",
        "Use this to size the right comment area.",
        EHintType::KEYWORD));
}


/** Add the vertical types to the hints.*/
void Csh::AddVerticalTypesToHints()
{
    AddToHints(CshHint(HintPrefix(COLOR_KEYWORD) + "brace",
        "Use this to add a large vertical curly brace, like '}'.",
        EHintType::KEYWORD));
    AddToHints(CshHint(HintPrefix(COLOR_KEYWORD) + "bracket",
        "Use this to add a large square baracket, like ']'.",
        EHintType::KEYWORD));
    AddToHints(CshHint(HintPrefix(COLOR_KEYWORD) + "range",
        "Use this to mark a vertical range, like this 'I'.",
        EHintType::KEYWORD));
    AddToHints(CshHint(HintPrefix(COLOR_KEYWORD) + "box",
        "Use this to add a box with vertically typeset text.",
        EHintType::KEYWORD));
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
    canvas->Fill(XY(0, 0), XY(HINT_GRAPHIC_SIZE_X, HINT_GRAPHIC_SIZE_Y), FillAttr(color, GRADIENT_DOWN));
    canvas->UnClip();
    return true;
}

/** Add names of chart options and commands that are valid inside a design definition to the list of hints. */
void Csh::AddDesignLineBeginToHints()
{
    AddDesignOptionsToHints();
    AddToHints(design_keyword_names, HintPrefix(COLOR_KEYWORD), EHintType::KEYWORD,
        CshHintGraphicCallbackForKeywords);
}

/** Add keywords to the list of hints. */
void Csh::AddKeywordsToHints(bool includeParallel)
{
    AddToHints(keyword_names+(includeParallel?0:4), HintPrefix(COLOR_KEYWORD), EHintType::KEYWORD, 
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
        AddToHints(CshHint(HintPrefix(COLOR_ENTITYNAME) + *i, NULL, EHintType::ENTITY, true, CshHintGraphicCallbackForEntities));
}


void Csh::AddVertXPosSyntaxNonSelectableToHints(bool include_at)
{
    const string prefix = include_at ? 
                           "\\|"+HintPrefix(COLOR_KEYWORD)+"at\\s()"+HintPrefixNonSelectable() :
                           HintPrefixNonSelectable();
    AddToHints(CshHint(prefix + "<entity> [offset]",
        "Position exactly on the entity line (optionally offset by the specified number of pixels).",
        EHintType::KEYWORD));
    AddToHints(CshHint(prefix + "<entity>+ [offset]",
        "Position a bit right of the entity line (optionally offset by the specified number of pixels).",
        EHintType::KEYWORD));
    AddToHints(CshHint(prefix + "<entity>- [offset]",
        "Position a bit left of the entity line (optionally offset by the specified number of pixels).",
        EHintType::KEYWORD));
    AddToHints(CshHint(prefix + "<entity>++ [offset]",
        "Position a somewhat right of the entity line (optionally offset by the specified number of pixels).",
        EHintType::KEYWORD));
    AddToHints(CshHint(prefix + "<entity>-- [offset]",
        "Position a somewhat left of the entity line (optionally offset by the specified number of pixels).",
        EHintType::KEYWORD));
    AddToHints(CshHint(prefix + "<entity>-<entity> [offset]",
        "Position to the midpoint between the two entities (optionally offset by the specified number of pixels).",
        EHintType::KEYWORD));
    if (include_at)
        AddToHints(CshHint(HintPrefix(COLOR_KEYWORD) + "at", NULL, EHintType::KEYWORD, true));
}

/** Add text escape sequences to hints.*/
void Csh::AddEscapesToHints(EEscapeHintType hint)
{
    switch (hint) {
    default: _ASSERT(0); //fallthrough
    case HINTE_NONE: 
        return;
    case HINTE_ESCAPE:
        StringFormat::EscapeHints(*this, string());
        break;
    case HINTE_PARAM_COLOR:
        AddColorValuesToHints(false);
        break;
    case HINTE_PARAM_STYLE:
        AddStylesToHints(false, false);
        break;
    case HINTE_PARAM_FONT:
        if (fontnames) {
            for (const auto &str : *fontnames)
                if (str.length() && str[0]!='@')
                    AddToHints(CshHint(HintPrefix(COLOR_ATTRVALUE)+str, NULL,
                                       EHintType::ATTR_VALUE));
        }
        break;
    case HINTE_PARAM_REF:
        addRefNamesAtEnd = true;
        break;
    case HINTE_PARAM_NUMBER:
        AddToHints(CshHint(HintPrefixNonSelectable()+"<number in pixels>", NULL,
                           EHintType::ATTR_VALUE, false));
        break;
    case HINTE_PARAM_LINK:
        const bool empty = hintedStringPos.first_pos>hintedStringPos.last_pos;
        const string prefix = empty ? 
            HintPrefix(COLOR_LABEL_ESCAPE) : HintPrefixNonSelectable();
        AddToHints(CshHint(prefix+"http://", 
            "You can enter a valid URL here.",
            EHintType::ATTR_VALUE, empty));
        AddToHints(CshHint(prefix+"\ref ",
            "Use this to refer to documented items, when the chart is a part of Doxygen documentation.",
            EHintType::ATTR_VALUE, empty));
        break;
    }
}

/** Post-process the list of hints.
 * 
 * Fill in the 'size' and 'plain' members.
 * Remove hints ending in an asterisk (like 'line.*') - these are just included to 
 * attach a description to a compacted group of hints. Compact means here that 
 * entries having the same beginning up to a dot will be combined to a single hint.
 * Then fill and compact the list (attach descriptions to compacted items). 
 * For example, if we have two hints "line.width" and "line.color" and the user has so
 * far typed "lin" we compact the two into "line.*". If, however, the user has already
 * typed "line.w" we do not compact (but perhaps filter line.color away if filterin is on).
 * Hints are also sorted.
 * @param [in] canvas A canvas to use at size calculation.
 * @param [in] format The text format to use at size calculation.
 * @param [in] orig_uc The string under the cursor. Used as prefix for filtering and grouping.
 * @param [in] filter_by_uc If true we remove those hints that do not start by 'uc'.
 * @param [in] compact_same We do compaction as described above if true.
 */
void Csh::ProcessHints(Canvas &canvas, StringFormat *format, const std::string &orig_uc, 
                       bool filter_by_uc, bool compact_same)
{
    //First separate hints ending in an asterisk (except for ESCAPE hints)
    std::list<CshHint> group_hints;
    //In case of dot compression, copy the elements ending in * to a separate list
    for (auto i = Hints.begin(); i!=Hints.end(); /*none*/) 
        if (i->decorated.size() && WantsDotCompress(i->type) &&
            i->decorated[i->decorated.size()-1]=='*') {
            group_hints.push_back(*i);
            Hints.erase(i++);
        } else
            i++;
    StringFormat f;
    f.Default();
    if (format==NULL) format = &f;
    Label label;
    CshHint start("", NULL, EHintType::ENTITY); //empty start
    unsigned start_len = 0;
    unsigned start_counter = 0;
    string uc;
    //remove the () part from the string under cursor.
    if (hadEscapeHint)
        uc = orig_uc.substr(0, orig_uc.find('('));
    else
        uc = orig_uc;

    //For escape hints we may only have groups for '\p' and '\m'. We shall display
    //all hints for any other escape char.
    if (hadEscapeHint && (uc.length()<2 || (uc[1]!='p' && uc[1]!='m')))
        filter_by_uc = false;

    for (auto i = Hints.begin(); i!=Hints.end(); /*none*/) {
        label.Set(i->decorated, canvas, *format);
        i->plain = label;
        //if we filter and label does not begin with uc, we drop it
        if (filter_by_uc && uc.length() && !CaseInsensitiveBeginsWith(i->plain, uc.c_str())) {
            Hints.erase(i++);
            continue;
        }
        string::size_type dot_pos;
        /* if compacting is on we combine all hints with the same prefix into a xxx.*-like hint
          There are two kinds of compression, dot_compression and escape compression
          
          With dot compression we may have multiple levels of compression, e.g.,
          tag.line.width can be compressed into either tag.* or tag.line.* depending
          on what is the string under the cursor, that is, if it begins with 
          'tag.' we have to use the latter, else the former.
          With dot compression we add hints for both of these cases (with individual
          descriptions), but remove them to 'group_hints' above. So when we create
          the the grouping, we actually create the group hint (only the right one, 
          so only one of tag.* or tag.line.*) and add it to Hints in this for cycle. 
          Then later, at the end of this function we match 'group_hints' with 
          'Hints' and if we find a match, we copy the description to the entry in
          'Hints'.

          With escape hints, the situation is different. Only one level of
          compress may happen and not along dots. Specialty is that only \\p and \\m
          escapes can be grouped. In this case we simply drop the non-grouped
          hint from Hints if we need the grouped one and vice versa.
         */
        if (compact_same && WantsDotCompress(i->type)) {
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
        //if compacting is on and we process escape hints, we combine all hints with 
        //the same prefix into a '\p*'-like hint
        if (i->type == EHintType::ESCAPE) {
            if (compact_same) {
                //We only compress \p* and \m*
                if ((i->plain[1]=='p' || i->plain[1]=='m')) {
                    //if uc holds the same character as this entry, we delete the group version, else 
                    //individual versions
                    if (uc[0] && (uc[1]==i->plain[1]) == (i->plain[2]=='*')) {
                        Hints.erase(i++);
                        continue;
                    }
                    if (i->plain[2]=='*') {
                        i->keep = true;
                        i->replaceto = i->plain.substr(0, 2);
                    }
                }
            } else if (i->plain[2]=='*') {
                //if we are not compressing, remove compressed hints
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
    //Now check if any one of the resulting hints ended up being the same as one of the group 
    //hints separaeted at the beginning and if so, copy the description.
    for (auto &hint : Hints) 
        if (hint.decorated.size() && 
            hint.decorated[hint.decorated.size()-1] == '*')
            for (const auto &ghint : group_hints)
                if (hint.decorated == ghint.decorated) {
                    hint.description = ghint.description;
                    break;
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
