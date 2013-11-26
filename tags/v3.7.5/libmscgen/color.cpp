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
/** @file color.cpp The definition of classes for colors.
 * @ingroup libmscgen_files */

#include <sstream>
#include "msc.h"

using namespace std;

//string may or may not have enclosing parenthesis
ColorType::ColorType(const string&text)
{
    type = INVALID;
    size_t pos = 0;
    pos = text.find_first_not_of(" \t", pos);
    if (text[pos]=='(') pos++;
    bool should_be_overlay = false;
    if (text.length() > pos+1 && text[pos] == '+' && text[pos] == '+') {
        pos += 2;
        should_be_overlay = true;
    }

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
    r = (unsigned char)fr;
    g = (unsigned char)fg;
    b = (unsigned char)fb;
    if (db==4) a = (unsigned char)fa;
    else a = 255;
    type = should_be_overlay ? OVERLAY : COMPLETE;
}

ColorType::ColorType(unsigned p)
{
    r = (((unsigned)(p))>>24)&255;
    g = (((unsigned)(p))>>16)&255;
    b = (((unsigned)(p))>>8)&255;
    a = ((unsigned)(p))&255;
    type = COMPLETE;
}

string ColorType::Print(void) const
{
    if (type==INVALID) return "(0,0,0,0)";
    std::ostringstream ss;
    ss<<"(";
    if (type==OVERLAY) ss<<">>";
    ss<<(int)r<<","<<(int)g<<","<<(int)b;
	if (a!=255) ss<<","<<int(a);
	ss<<")";
    return ss.str();
}

ColorType ColorType::operator +(const ColorType &o) const
{
    if (o.type==INVALID) return *this;
    if (o.type==COMPLETE) return o;
    if (type!=COMPLETE) return o; //if o is overlay we replace what we have
    //now we are COMPLETE, o is OVERLAY, but all can be transparent
    //http://en.wikipedia.org/wiki/Alpha_compositing#Alpha_blending
    //this is DST, o is SRC
    double ret_a = double(o.a) + double(a)*(255-o.a)/255.;
    ColorType ret;
    ret.type = (o.type==COMPLETE || type==COMPLETE) ? COMPLETE : OVERLAY;
    if (ret_a < 1e-3)
        return ColorType(0, 0, 0, 0, ret.type); //fully transparent
    ret.a = (unsigned char)ret_a;
    ret.r = (unsigned char)((o.r*o.a + r*a*(255-o.a)/255.)/ret_a);
    ret.g = (unsigned char)((o.g*o.a + g*a*(255-o.a)/255.)/ret_a);
    ret.b = (unsigned char)((o.b*o.a + b*a*(255-o.a)/255.)/ret_a);
    return ret;
}


/** Add an element to the collection.
 *
 * If the color definition is not valid, no color is added to the collection,
 * but an error is generated.
 * @param [in] alias The name of the color to add.
 * @param [in] colordef The textual definition of the color.
 * @param error This is where we add errors if the color description is not valid.
 * @param [in] linenum The location of `colordef` in the input file.
 */
bool ColorSet::AddColor(const std::string &alias, const std::string &colordef,
                        MscError &error, FileLineColRange linenum)
{
    ColorType c = GetColor(colordef);
    if (c.type!=ColorType::INVALID) {
        this->operator[](alias) = c;
        return true;
    }
    string ss = "Invalid color definition for ";
    ss << alias << ": '" << colordef << "'. Ignoring color alias definition.";
    error.Error(linenum.start, ss);
    return false;
}

inline string remove_spaces(const string &s) 
{
    string::size_type a = s.find_first_not_of(" \t\n");
    if (a == string::npos) 
        return string();    
    string::size_type b = s.find_last_not_of(" \t\n");
    return s.substr(a, b-a+1);
}

/** Return a color from the collection by name.
 *
 * The name can also be an rgb color definition or a variation of 
 * an existing color, one of the following.
 * 1. a color name
 * 2. a color name, followed by a comma and an alpha value
 * 3. a color name followed by {+-} and a percentage of lightness
 * 4. a color name, followed by a comma and an alpha value,  followed by {+-} and a percentage of lightness
 * 5. three int value separated by commas (rgb)
 * 6. four int values separated by commas (rgba)
 * Either can be preceeded by '++' to indicate a color to overlay.
 *
 * In the first 4 case we consult the collection in the last case we return
 * a value irrespective of the collection content.
 * @param [in] s_original The description of the color.
 * @returns A color, which may be invalid if the description is invalid or not in the collection.
 */
ColorType ColorSet::GetColor(const std::string &s_original) const
{
    string s = remove_spaces(s_original);
    ColorType::EColorType type = ColorType::COMPLETE;
    if (s.length()>2 && s[0]=='+' && s[1]=='+') {
        type = ColorType::OVERLAY;
        s.erase(0, 2);
    }
    const_iterator i = find(s);
    //if #1, return the value for the color name
    if (this->end()!=i) return ColorType(i->second, type);
	string::size_type pos = s.find_first_of(",+-");
    //if no comma, + or - and not #1, return invalid color
    if (pos == string::npos) return ColorType();
    string name = remove_spaces(s.substr(0, pos));
    i = find(name);
    if (this->end()==i) {
        //if first element not in collection, it is either #5 or #6 (or completely invalid)
        //ColorType constructor can handle these, but we add OVERLAY type if we have 
        //seen ++
        ColorType ret(s);
        if (ret.type!=ColorType::INVALID)
            ret.type = type;
        return ret;
    }
    //if first element in collection, see if followidered by an alpha value and/or lightness
    ColorType color = i->second;
    color.type = type;
    double num;
    bool had_alpha = false;
    bool had_light = false;
    while (pos<s.length()) {
        switch (s[pos]) {
        case ',': //alpha value
            if (had_alpha) return ColorType();
            if (sscanf(s.c_str()+pos+1, "%lf", &num)!=1) return ColorType();
            if (num<0 || num>255) return ColorType();
            if (num <= 1.0) color.a = (unsigned char)(num*255);
            else color.a = (unsigned char)(num);
            had_alpha = true;
            break;
        case '+':
        case '-':
            if (had_light) return ColorType();
            //include sign, too
            if (sscanf(s.c_str()+pos, "%lf", &num)!=1) return ColorType();
            if (num<-200 || num >200) return ColorType();
            if (num<-1.0 || num>1.0) num /= 100;
            if (num>=0) color = color.Lighter(num);
            else color = color.Darker(-num);
            had_light = true;
            break;
        default:
            _ASSERT(0);
        }
        //now search for + or -
        pos = s.find_first_of(",+-", pos+1);
    }
    return color;
}
