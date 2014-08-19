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

/** @file error.cpp The definition of classes for error registration.
 * @ingroup libmscgen_files */

#include <algorithm>
#include "msc.h"

std::string FileLineCol::Print()
{
    std::string ret("\\" ESCAPE_STRING_LOCATION "(");
    ret << file << "," << line << "," << col << ")";
    return ret;
}

unsigned MscError::AddFile(const string &filename)
{
    for (unsigned i=0; i<Files.size(); i++)
        if (Files[i] == filename) return i;
    Files.push_back(filename);
    return (unsigned)Files.size()-1;
}

/** Formulate an error element - used internally. 
 * @param [in] linenum The line number to show.
 * @param [in] linenum_ord The line number to use at ordering.
 * @param [in] is_err True if this is an error, false if a Warning. 
 * @param [in] is_once True if the message is to be displayed only once.
 * @param [in] msg The message to display
 * @returns The error element. */
ErrorElement MscError::FormulateElement(FileLineCol linenum, FileLineCol linenum_ord, bool is_err, bool is_once, const std::string &msg) const
{
    ErrorElement e;
    e.message = msg;
    if (linenum.file>=0) { //only do this for valid linenums
        if (linenum.file<int(Files.size()) && Files[linenum.file].length()>0)
            e.text = Files[linenum.file];
        if (linenum.line) {
            if (e.text.length()>0) e.text.append(":");
            e.text << linenum.line;
            if (linenum.col)
                e.text << ":" << linenum.col;
        }
    }
    if (e.text.length()>0) e.text.append(": ");
    if (is_once)
        e.message = "("+e.message+")";
    else if (is_err)
        e.text.append("error: ");
    else
        e.text.append("warning: ");
    e.text.append(e.message);
    e.relevant_line = linenum;
    e.ordering_line = linenum_ord;
    e.isError = is_err;
    e.isOnlyOnce = is_once;
    return e;
}

/**Adds an error/warning related to an attribute.
 *
 * Line numbers are deducted from the Attribute.
 * @param [in] a The faulty attribute.
 * @param [in] atValue True if the problem is with the value of the attr, false if with its name.
 * @param [in] s The message to display
 * @param [in] once Auxiliary info, a type of hint on what to do. Added as a separate element, but only once.
 * @param [in] is_err True if this is an error, false if a Warning. */
void MscError::Add(const Attribute &a, bool atValue, const std::string &s, const std::string &once, bool is_err)
{
    if (a.error) return;
    a.error = true;
    if (atValue)
        Add(a.linenum_value.start, a.linenum_value.start, s, once, is_err);
    else
        Add(a.linenum_attr.start, a.linenum_attr.start, s, once, is_err);
}

/**Adds an error/warning.
 *
 * @param [in] linenum The line number to show.
 * @param [in] linenum_ord The line number to use at ordering.
 * @param [in] s The message to display
 * @param [in] once Auxiliary info, a type of hint on what to do. Added as a separate element, but only once.
 * @param [in] is_err True if this is an error, false if a Warning. */
void MscError::Add(FileLineCol linenum, FileLineCol linenum_ord, const std::string &s, const std::string &once, bool is_err)
{
    ErrorElement e1 = FormulateElement(linenum, linenum_ord, is_err, false, s);
    if (is_err)
        Errors.push_back(e1);
    ErrorsAndWarnings.push_back(e1);

    if (once.length()>0) {
        ErrorElement e2 = FormulateElement(linenum, linenum_ord, is_err, true, once);
        if (is_err)
            Errors.push_back(e2);
        ErrorsAndWarnings.push_back(e2);
    }
};

void MscError::_sort(std::vector<ErrorElement> &store)
{
    if (store.size()<2) return;
    sort(store.begin(), store.end());
    for (std::vector<ErrorElement>::size_type i=store.size()-1; i>0; i--)
        if (store[i].isOnlyOnce)
            for (std::vector<ErrorElement>::size_type j=i; j>0; j--)
                if (store[j-1].isOnlyOnce && store[i].message==store[j-1].message) {
                    store.erase(store.begin()+i);
                    break;
                }
}

string MscError::Print(bool oWarnings) const
{
    string a;
    const std::vector<ErrorElement> &store = get_store(oWarnings);
    for (unsigned i = 0; i<store.size(); i++)
        a.append(store[i].text).append("\n");
    return a;
}


MscError &MscError::operator += (const MscError &o)
{
    hadFatal |= o.hadFatal;
    const unsigned old_e = Errors.size();
    const unsigned old_w = ErrorsAndWarnings.size();
    Errors.insert(Errors.end(), o.Errors.begin(), o.Errors.end());
    ErrorsAndWarnings.insert(ErrorsAndWarnings.end(), o.ErrorsAndWarnings.begin(), o.ErrorsAndWarnings.end());
    for (unsigned u = old_e; u<Errors.size(); u++) {
        Errors[u].relevant_line.file += Files.size();
        Errors[u].ordering_line.file += Files.size();
    }
    for (unsigned u = old_w; u<ErrorsAndWarnings.size(); u++) {
        ErrorsAndWarnings[u].relevant_line.file += Files.size();
        ErrorsAndWarnings[u].ordering_line.file += Files.size();
    }
    Files.insert(Files.end(), o.Files.begin(), o.Files.end());
    return *this;
}