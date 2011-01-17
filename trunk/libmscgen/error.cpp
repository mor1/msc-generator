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

#include <algorithm>
#include "msc.h"

std::string file_line::Print()
{
    std::string ret("\\" ESCAPE_STRING_LOCATION "(");
    ret << file << "," << line << "," << col << ")";
    return ret;
}

unsigned MscError::AddFile(const string &filename)
{
    for (int i=0; i<Files.size(); i++)
        if (Files[i] == filename) return i;
    Files.push_back(filename);
    return Files.size()-1;
}

ErrorElement MscError::FormulateElement(file_line linenum, bool is_err, bool is_once, const std::string &msg) const
{
    ErrorElement e;
    e.message = msg;
    if (Files[linenum.file].length()>0) e.text = Files[linenum.file];
    if (linenum.line) {
        if (e.text.length()>0) e.text.append(":");
        e.text << linenum.line;
        if (linenum.col)
            e.text << ":" << linenum.col;
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
    e.ordering_line = linenum;
    e.isError = is_err;
    e.isOnlyOnce = is_once;
    return e;
}

void MscError::Add(const Attribute &a, bool atValue, const std::string &s, const std::string &once, bool is_err)
{
    if (a.error) return;
    a.error = true;
    if (atValue)
        Add(a.linenum_value.start, s, once, is_err);
    else
        Add(a.linenum_attr.start, s, once, is_err);
}

void MscError::Add(file_line linenum, const std::string &s, const std::string &once, bool is_err)
{
    ErrorElement e1 = FormulateElement(linenum, is_err, false, s);
    if (is_err)
        Errors.push_back(e1);
    ErrorsAndWarnings.push_back(e1);

    if (once.length()>0) {
        ErrorElement e2 = FormulateElement(linenum, is_err, true, once);
        if (is_err)
            Errors.push_back(e2);
        ErrorsAndWarnings.push_back(e2);
    }
};

void MscError::_sort(std::vector<ErrorElement> &store)
{
    if (store.size()<2) return;
    sort(store.begin(), store.end());
    for (int i=store.size()-1; i>0; i--)
        if (store[i].isOnlyOnce)
            for (int j=i-1; j>=0; j--)
                if (store[j].isOnlyOnce && store[i].message==store[j].message) {
                    store.erase(store.begin()+i);
                    break;
                }
}

string MscError::Print(bool oWarnings) const
{
    string a;
	const std::vector<ErrorElement> &store = get_store(oWarnings);
    for (int i = 0; i<store.size(); i++)
        a.append(store[i].text).append("\n");
    return a;
}

