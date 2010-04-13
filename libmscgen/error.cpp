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
    along with Msc-generator.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "error.h"
#include "msc.h"

MscError::MscError() {
    strict = false;
}

unsigned MscError::AddFile(const string &filename)
{
    for (int i=0; i<Files.size(); i++)
        if (Files[i] == filename) return i;
    Files.push_back(filename);
    return Files.size()-1;
}

void MscError::Add(const Attribute &a, const std::string &s, const std::string &once,
                   std::multimap<file_line, std::string>& store)
{
    if (a.error) return;
    a.error = true;
    Add(a.linenum, s, once, store);
}

void MscError::Add(file_line linenum, const std::string &s, const std::string &once,
                   std::multimap<file_line, std::string>& store)
{
    string msg;
    if (Files[linenum.file].length()>0) msg = Files[linenum.file];
    if (linenum.line) {
        if (msg.length()>0) msg.append(":");
        msg << linenum.line;
        if (linenum.col)
            msg << ":" << linenum.col;
    }
    if (msg.length()>0) msg.append(": ");
    string type;
    if (&store == &Warnings)
        type = "warning: ";
    else
        type = "error: ";
    store.insert(std::pair<file_line, string>(linenum, msg+type+s));
    if (once.length()>0)
        store.insert(std::pair<file_line, string>(linenum, msg+"_o_n_c_e: ("+once+")"));

};

string MscError::Print(bool oWarnings) const
{
    string a;
    std::multimap<file_line, string> ErrorsWarnings(Errors);
    if (oWarnings)
        ErrorsWarnings.insert(Warnings.begin(), Warnings.end());

    std::set<string> once_printed;
    std::multimap<file_line, string>::const_iterator i;
    for (i = ErrorsWarnings.begin(); i!=ErrorsWarnings.end(); i++) {
        string str = i->second;
        size_t pos = str.find("_o_n_c_e: ");
        if (pos != std::string::npos) {
            str.erase(pos, 10);
            if (once_printed.find(str.substr(pos)) != once_printed.end())
                continue;
            once_printed.insert(str.substr(pos));
        }
        a.append(str).append("\n");
    }
    return a;
}

file_line MscError::GetFileLineOfErrorNo(unsigned num) const
{
	if (Errors.size() < num) return file_line(0,0,0);
    std::multimap<file_line, string>::const_iterator i=Errors.begin();
	unsigned u=0;
	while (i!=Errors.end()&&u<num) {
		i++;
		u++;
	}
	if (i!=Errors.end() && num==u)
		return i->first;
	return file_line(0,0,0);
}
 