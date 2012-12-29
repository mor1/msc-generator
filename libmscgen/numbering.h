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

/** @file numbering.h The declaration of basic classes for numbering styles.
 * @ingroup libmscgen_files */

#if !defined(NUMBERING_H)
#define NUMBERING_H
#include <string>
#include <sstream>
#include <vector>
#include "error.h"

class Msc;

/**Specifies the style of a segment in a multi-level number, such as "A.1"*/
class NumberingStyleFragment {
public:
    /**Describes the type of number to use.*/
    typedef enum {
        ARABIC,      ///<Arabic numbers, such as 1, 8 or 14.
        ROMAN_UPPER, ///<Uppercase roman numbers, such as I, VIII or XIV.
        ROMAN_LOWER, ///<Uppercase roman numbers, such as i, viii or xiv. 
        ABC_UPPER,   ///<Uppercase letters, such as A, H or N.
        ABC_LOWER    ///<Uppercase letters, such as a, h or n.
    } ENumberingStyle;
    ENumberingStyle type; ///<The type of number to use in this segment
    std::string     pre;  ///<Text to prepend to this segment, like '.' to achieve "A.1" for example
    std::string     post; ///<Text to append to this segment
    /** Create a style fragment with the given or default attributes*/
    explicit NumberingStyleFragment(ENumberingStyle t=ARABIC, std::string pr="", std::string po=""): type(t), pre(pr), post(po) {}
    static bool Parse(Msc *, file_line linenum, const char *text, std::vector<NumberingStyleFragment> &result);
    std::string Print(int n) const;
    unsigned Input(const std::string &number, int &value);
    static bool FindReplaceNumberFormatToken(std::string &text, file_line l, std::string::size_type pos=0);
};

class Numbering {
    friend class NumberingStyle;
protected:
    std::vector<int> values;
public:
    //This flag is set after adding a number to box or bigarrow labels if they have content.
    //If we add levels with this flag set, we decrement the counter before adding the levels
    //so if the box title got number 7, its first content will get 7.1 and not 8.1
    bool decrementOnAddingLevels;
    Numbering() : decrementOnAddingLevels(false) {values.push_back(1);}
    int &Last(void) {return values[values.size()-1];}
    const int &Last(void) const {return values[values.size()-1];}
    void SetSize(unsigned n);
    Numbering& operator ++() {++Last(); decrementOnAddingLevels=false; return *this;}
    Numbering& operator --() {--Last(); return *this;}
};

class NumberingStyle {
protected:
    std::pair<bool, std::vector<NumberingStyleFragment>> elements;
    unsigned                            startAt;
public:
    std::pair<bool, std::string> pre;
    std::pair<bool, std::string> post;

    NumberingStyle(): startAt(0) {elements.first = pre.first = post.first = true; elements.second.push_back(NumberingStyleFragment());}
    NumberingStyleFragment &Last(void) {_ASSERT(IsComplete()); return elements.second[elements.second.size()-1];}
    const NumberingStyleFragment &Last(void) const {_ASSERT(IsComplete()); return elements.second[elements.second.size()-1];}
    bool IsComplete() const {return elements.first && pre.first && post.first;}
    NumberingStyle &operator +=(const NumberingStyle&o); 
    void Reset() {elements.first = pre.first = post.first = true; pre.second.clear(); post.second.clear(); startAt=0; elements.second.clear(); elements.second.push_back(NumberingStyleFragment());}
    void Empty() {elements.first = pre.first = post.first = false;} 
    size_t Size(void) const {_ASSERT(IsComplete()); return startAt + elements.second.size();}
    void CopyShifted(const NumberingStyle &ns, unsigned start=0);
    int Apply(const std::vector<NumberingStyleFragment> &nsfs);
    void Push(const NumberingStyleFragment &nf) {_ASSERT(IsComplete()); elements.second.push_back(nf);}
    void Push(const std::vector<NumberingStyleFragment> &nsfs) {elements.first = true; elements.second.insert(elements.second.end(), nsfs.begin(), nsfs.end());}
    void Pop() {_ASSERT(IsComplete()); if (elements.second.size()>1) elements.second.pop_back();}
    std::string Print(const Numbering &n) const;
};

#endif //NUMBERING_H
