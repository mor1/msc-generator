/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011,2012,2013 Zoltan Turanyi
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
    static bool Parse(Msc *, FileLineCol linenum, const char *text, std::vector<NumberingStyleFragment> &result);
    std::string Print(int n) const;
    unsigned Input(const std::string &number, int &value);
    static bool FindReplaceNumberFormatToken(std::string &text, FileLineCol l, std::string::size_type pos=0);
};

/** Describes the current (multi-level) label number - essentially a list of numbers.
 * This class is used to store the current label number without the formatting.
 * Thus for "A.1" we store <1, 1>, for "B.2.1" we store <2, 2, 1>, etc.
 * When we increment, we increment the last one.
 */
class Numbering {
    friend class NumberingStyle;
protected:
    std::vector<int> values; ///<The actual numbers. Index zero is the highest level one.
public:
    /** True if we shall decrement the last number when adding a level.
     * This flag is set after numbering a box label if the box has content.
     * As a result, if we add a level for sub-numbers for labels inside the box, 
     * we decrement the counter before adding the levels,
     * so if the box title got number 7, its first content will get 7.1 and not 8.1.*/
    bool decrementOnAddingLevels;
    Numbering() : decrementOnAddingLevels(false) {values.push_back(1);} ///<A single number of '1'
    int &Last(void) {return values[values.size()-1];} ///<Returns a reference to the last number.
    const int &Last(void) const {return values[values.size()-1];} ///<Returns the value of the last number.
    void SetSize(unsigned n);
    /** Increment the last number. */
    Numbering& operator ++() {++Last(); decrementOnAddingLevels=false; return *this;}
    /** Decrement the last number. */
    Numbering& operator --() {--Last(); return *this;}
};

/** Describes (multi-level) label numbering style. 
 * Also allows to specify only the latter parts of the style, that is,
 * we may skip the style specification for the first few levels.
 * When the first (few) levels are not specified, we omit them when printing the number.
 * That is, if the style is <not spec, letter, arabic>, we print the number <1, 2, 3> as
 * "B.3".*/
class NumberingStyle {
protected:
    std::pair<bool, std::vector<NumberingStyleFragment>> elements; ///<The style for each individual level. Not set if `first` is false.
    unsigned                            startAt; ///<Specifies the number of levels we do not specify at the beginning.
public:
    std::pair<bool, std::string> pre;  ///<Text to prepend to the whole (multi-level) number. Not specified if `first` is false.
    std::pair<bool, std::string> post; ///<Text to append to the whole (multi-level) number. Not specified if `first` is false.

    /** A fully specified style, with a single arabic numbering level and nothing to append or prepend.*/
    NumberingStyle(): startAt(0) {elements.first = pre.first = post.first = true; elements.second.push_back(NumberingStyleFragment());}
    /** A reference to the style of the lowest level.*/
    NumberingStyleFragment &Last(void) {_ASSERT(IsComplete()); return elements.second[elements.second.size()-1];}
    /** A reference to the style of the lowest level.*/
    const NumberingStyleFragment &Last(void) const {_ASSERT(IsComplete()); return elements.second[elements.second.size()-1];}
    /** True if a fully specified numberin style.*/
    bool IsComplete() const {return elements.first && pre.first && post.first;}
    /** Merges another numbering style to us. Any attribute specified in `o` is copied to us, others remain unchanged.*/
    NumberingStyle &operator +=(const NumberingStyle&o); 
    /** Resets the content to the fully specified style of a single arabic numbering level and nothing to append or prepend.*/
    void Reset() {elements.first = pre.first = post.first = true; pre.second.clear(); post.second.clear(); startAt=0; elements.second.clear(); elements.second.push_back(NumberingStyleFragment());}
    /** Clears all content, making none of the attributes specified.*/
    void Empty() {elements.first = pre.first = post.first = false;} 
    /** Returns, the level of the last element */
    size_t Size(void) const {_ASSERT(IsComplete()); return startAt + elements.second.size();}
    int Apply(const std::vector<NumberingStyleFragment> &nsfs);
    /** Adds a level.*/
    void Push(const NumberingStyleFragment &nf) {_ASSERT(IsComplete()); elements.second.push_back(nf);}
    /** Adds a number of levels.*/
    void Push(const std::vector<NumberingStyleFragment> &nsfs) {elements.first = true; elements.second.insert(elements.second.end(), nsfs.begin(), nsfs.end());}
    /** Deletes the last level. */
    void Pop() {_ASSERT(IsComplete()); if (elements.second.size()>1) elements.second.pop_back();}
    /** Creates a number with our formatting.*/
    std::string Print(const Numbering &n) const;
};

#endif //NUMBERING_H
