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
#include <sstream>
#include "msc.h"

using namespace std;

//string may or may not have enclosing parenthesis
MscColorType::MscColorType(const string&text)
{
    valid=false;
    size_t pos = 0;
    pos = text.find_first_not_of(" \t", pos);
    if (text[pos]=='(') pos++;
    double fr, fg, fb, fa=0;
    int db = sscanf(text.c_str()+pos,"%lf,%lf,%lf,%lf", &fr, &fg, &fb, &fa);
    if (db<3) return;
    if (fr<0 || fr>255 || fg<0 || fg>255 || fb<0 || fb>255) return;
    if (db == 4 && (fa<0 || fa>255)) return;
    if (fr<=1.0 && fg<=1.0 && fb<=1.0 && fa<=1.0) {
        fr *= 255;
        fg *= 255;
        fb *= 255;
        fa *= 255;
    }
    r = fr;
    g = fg;
    b = fb;
    if (db==4) a = fa;
    else a = 255;
    valid = true;
}

string MscColorType::Print(void) const
{
    if (!valid) return "-invalid-";
    std::ostringstream ss;
    ss<<"("<<(int)r<<","<<(int)g<<","<<(int)b<<","<<int(a)<<")";
    return ss.str();
}

bool ColorSet::AddColor(const std::string alias, const std::string colordef,
                        MscError &error, file_line_range linenum)
{
    MscColorType c = GetColor(colordef);
    if (c.valid) {
        this->operator[](alias) = c;
        return true;
    }
    string ss = "Invalid color definition for ";
    ss << alias << ": '" << colordef << "'. Ignoring color alias definition.";
    error.Error(linenum.start, ss);
    return false;
}

MscColorType ColorSet::GetColor(const std::string &s) const
{
    /* s can be one of four things
     * 1. a color name
     * 2. a color name, followidered by a comma and an alpha value
     * 3. three int value separated by commas (rgb)
     * 4. four int values separated by commas (rgba)
     */
    const_iterator i = find(s);
    //if #1, return the value for the color name
    if (this->end()!=i) return i->second;
    size_t pos = s.find(",");
    //if no comma and not #1, return invalid color
    if (pos == string::npos) return MscColorType();
    i = find(s.substr(0, pos));
    //if first element in collection, see if followidered by an alpha value
    if (this->end()!=i) {
        double alpha;
        if (sscanf(s.c_str()+pos+1,"%lf",&alpha)!=1) return MscColorType();
        if (alpha<0 || alpha>255) return MscColorType();
        MscColorType color = i->second;
        if (alpha <= 1.0) color.a = (unsigned char)(alpha*255);
        else color.a = (unsigned char)(alpha);
        return color;
    }
    //if first element not in collection, it is either #3 or #4,
    //MscColorType constructor can handle these
    return  MscColorType(s);
}

/*MscColorType &MscColorType::operator +=(const MscColorType &c)
{
    a = 255;
    r = (unsigned(255-c.a)*r + unsigned(c.a)*c.r)/255;
    g = (unsigned(255-c.a)*g + unsigned(c.a)*c.g)/255;
    b = (unsigned(255-c.a)*b + unsigned(c.a)*c.b)/255;
    return *this;
    } */

MscColorType MscColorType::operator +(const MscColorType &c) const
{
    return MscColorType((unsigned(255-c.a)*r + unsigned(c.a)*c.r)/255,
                        (unsigned(255-c.a)*g + unsigned(c.a)*c.g)/255,
                        (unsigned(255-c.a)*b + unsigned(c.a)*c.b)/255,
                        255);
}

