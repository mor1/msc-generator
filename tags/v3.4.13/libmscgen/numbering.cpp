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

#include <cstdlib>
#include "numbering.h"
#include "msc.h"

int const   romandata_value[] =   {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1, 0};
char const *romandata_numeral[] = {"M", "CM","D", "CD", "C", "XC", "L","XL", "X", "IX", "V", "IV", "I", NULL};
int const   romandata_times[] =   {10, 1, 1, 1, 3, 1, 1, 1, 3, 1, 1, 1, 3, 0};

std::string NumberingStyleFragment::Print(int n) const
{
    if (n==0)
        return "0";
    if (type==ARABIC) {
        char buff[200];
        sprintf(buff, "%d", n);
        return buff;
    }
    std::string ret;
    int num = abs(n);
    if (type == ABC_LOWER || type == ABC_UPPER) {
        do {
            num--;
            ret.insert(ret.begin(), char(num%26 + ((type==ABC_UPPER) ? 'A' : 'a')));
            num /= 26;
        } while (num>0);
    } else {
        //Roman letters here
        for (int current=0; romandata_value[current]>0; current++) {
            while (num >= romandata_value[current]) {
                ret += romandata_numeral[current];
                num -= romandata_value[current];
            }
        }
        if (type == ROMAN_LOWER)
            for (unsigned i = 0; i<ret.length(); i++)
                ret[i] += 'a' - 'A';
    }
    if (n<0) return '-'+ret;
    else return ret;
};

bool NumberingStyleFragment::Parse(Msc *msc, file_line linenum, const char *text, std::vector<NumberingStyleFragment> &result)
{
    string str(text);
    //We keep \s() \c() etc escapes, but remove the erroneous ones.
    StringFormat::ExpandColorAndStyle(str, msc, linenum, NULL, true, StringFormat::NUMBER_FORMAT);
    //Now find a numbering format escape and separate pre and post
    int pos_of_number = StringFormat::FindNumberingFormatEscape(str.c_str());
    if (pos_of_number == -1) {
        msc->Error.Error(linenum, "You must include at least one of the number format specifications in a numbering format. Ignoring option.",
                         "Try one of '123', 'iii', 'III', 'abc' or 'ABC'. You can also prepend and append additional text.");
        return false;
    }
    unsigned pos = 0;
    do {
        NumberingStyleFragment nsf;
        switch (str[pos + pos_of_number + 2]) {
        default:
        case '1': nsf.type = ARABIC; break;
        case 'a': nsf.type = ABC_LOWER; break;
        case 'A': nsf.type = ABC_UPPER; break;
        case 'i': nsf.type = ROMAN_LOWER; break;
        case 'I': nsf.type = ROMAN_UPPER; break;
        }
        nsf.pre  = str.substr(pos, pos_of_number);
        result.push_back(nsf);
        pos += pos_of_number+3;
        pos_of_number = StringFormat::FindNumberingFormatEscape(str.c_str()+pos);
    } while (pos_of_number != -1);
    result[result.size()-1].post = str.substr(pos);
    return true;
}

//Reads in a string value and converts it to int
//Returns how many characters it could not understand at the end (0 = fully OK)
//At a complete error, it leaves value unchanged
unsigned NumberingStyleFragment::Input(const std::string &number, int &value)
{
    _ASSERT(number.length()<std::numeric_limits<unsigned>::max());
    if (number.length()==0) return 0; //OK, but do not set the value
    unsigned num=0;
    unsigned pos=0;
    if (type==ARABIC) {
        goto out;
    } else if (type == ABC_LOWER || type == ABC_UPPER) {
        for(; number.length()>pos; pos++) {
            if (number[pos]<='Z' && number[pos]>='A') num = num*26 + number[pos] - 'A';
            else if (number[pos]<='z' && number[pos]>='a') num = num*26 + number[pos] - 'a';
            else {
                num++;
                goto out;
            }
        }
        num++;
    } else { //roman letters here
        for (int current = 0; romandata_value[current] > 0; ++current) {
            int repeat = 0;
            while (romandata_numeral[current][0] == toupper(number[pos]) &&
                   (romandata_numeral[current][1] == 0 || (number.length() > pos+1 && romandata_numeral[current][1] == toupper(number[pos+1])))) {
                if (++repeat > romandata_times[current]) goto out;
                pos += romandata_numeral[current][1] ? 2 : 1;
                num += romandata_value[current];
            }
        }
        if (pos!=number.length()) goto out;
    }
    value = num;
    return 0; //OK

out:
    //If we are arabic or completely failed to parse the roman or letter,
    //we try as an arabic number
    if (pos==0) {
        num = 0;
        for(; number.length()>pos; pos++)
            if (number[pos]<'0' || number[pos]>'9') goto out;
            else num = num*10 + number[pos] - '0';
    }
    if (pos>0) value = num;
    return (unsigned)number.length()-pos;
}

bool NumberingStyleFragment::FindReplaceNumberFormatToken(string &text, file_line l,
                                                          string::size_type pos)
{
    char const *formats[] = {"123", "arabic","ARABIC",
                             "iii", "xxx", "roman", "III", "XXX", "ROMAN",
                             "abc", "letters", "letter", "ABC", "LETTERS", "LETTER",
                             NULL};
    char const *codes[] = {"1", "1", "1",
                           "i", "i", "i", "I", "I", "I",
                           "a", "a", "a", "A", "A", "A"};

    for (unsigned i=0; formats[i]!=NULL; i++) {
        string::size_type pos2 = text.find(formats[i], pos);
        if (pos2 == string::npos) continue;
        l.col += (unsigned)strlen(formats[i]);
        string esc("\\" ESCAPE_STRING_NUMBERFORMAT);
        esc += codes[i];
        text.replace(pos2, strlen(formats[i]), esc + l.Print());
        return true;
    }
    return false;
}

void Numbering::SetSize(unsigned n)
{
    if (n==0) return;
    if (n > values.size()) {
        if (decrementOnAddingLevels)
            --Last();
        values.insert(values.end(), n-values.size(), 1);
    } else if (n < values.size())
        values.erase(values.begin() + n, values.end());
    decrementOnAddingLevels = false;
}

void NumberingStyle::CopyShifted(const NumberingStyle &ns, unsigned start)
{
    elements.clear();
    for (unsigned i = start; i<ns.elements.size(); i++)
        elements.push_back(ns.elements[i]);
    if (elements.size() == 0) //too much shift
        elements.push_back(NumberingStyleFragment());
    startAt = start + ns.startAt;
}

int NumberingStyle::Apply(const std::vector<NumberingStyleFragment> &nsfs)
{
    int off = startAt + (int)elements.size() - (int)nsfs.size();
    if (off < 0) return startAt + (int)elements.size();
    startAt = off;
    elements = nsfs;
    return 0;
}

std::string NumberingStyle::Print(const Numbering &n) const
{
    std::string ret;
    for (unsigned i = 0; i<elements.size(); i++) {
        int num=1;
        if (n.values.size() > startAt + i)
            num = n.values[startAt+i];
        ret += elements[i].pre + elements[i].Print(num) + elements[i].post;
    }
    return ret;
}

