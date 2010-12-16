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
#include <cassert>
#include <cstring>
#include "msc.h"

//0 if a does not begin with b
//1 if a begins with b
//2 if a == b
//Rule 1: "" begins with a NULL, but is not equal to it
//Rule 2: every string begins with a "" or a NULL
int CaseInsensitiveBeginsWidth(const char *a, const char *b)
{
    if (b==NULL) return a==NULL?2:1;
    if (a==NULL) return 0;
    unsigned i=0;
    while(a[i] && b[i]) {
        if (tolower(a[i]) != tolower(b[i]))
            return 0;
        i++;
    }
    if (b[i]==0) return a[i]==0?2:1;
    return 0;
}


string Attribute::Print(int ident) const
{
    string s(ident*2,' ');
    s << name << "=";
    switch (type) {
        case MSC_ATTR_STRING:
            s << value;
            break;

        case MSC_ATTR_NUMBER:
            s << number;
            break;

        case MSC_ATTR_BOOL:
            s << (yes?"yes":"no");
            break;
    };
    return s;
};

//Case insensitive comparison with name
bool Attribute::EndsWith(const char *a) const
{
    int i1 = strlen(a)-1;
    int i2 = name.length()-1;
    if (i1>i2 || i1<0 || i2<0) return false;
    while(i1>=0 && i2>=0) {
        if (toupper(a[i1]) != toupper(name[i2]))
            return false;
        i1--; i2--;
    }
    return i2<0 || name[i2]=='.';
}

bool Attribute::StartsWith(const char *a) const
{
    unsigned i=0;
    while(a[i] && i<name.length()) {
        if (toupper(a[i]) != toupper(name[i]))
            return false;
        i++;
    }
    return a[i]==0 && (i==name.length() || name[i]=='.');
}

bool Attribute::CheckType(MscAttrType t, MscError &error) const
{
    if (type == t) return true;
    string ss;
    ss << "Value for '" << name << "' ";
    switch (t) {
    case MSC_ATTR_STRING:
        if (type != MSC_ATTR_STYLE)
            return true;
        ss << "Attribute '" << name << "' seems to be a style. Ignoring attribute.";
        error.Error(*this, true, ss, "Use quotation marks to force this value.");
        break;

    case MSC_ATTR_BOOL:
        error.Error(*this, true, ss << "must be 'yes' or 'no'. Ignoring attribute.");
        break;

    case MSC_ATTR_NUMBER:
        error.Error(*this, true, ss << "must be a number. Ignoring attribute.");
        break;
    }
    return false;
}

bool Attribute::CheckColor(const ColorSet &colors, MscError &error) const
{
    if (type==MSC_ATTR_STRING) {
        MscColorType c = colors.GetColor(value);
        if (c.valid) return true;
    }
    string ss;
    ss << "Unrecognized color name or definition: '" << value << "'. Ignoring attribute.";
    error.Error(*this, true, ss);
    return false;
}

void Attribute::InvalidValueError(const string &candidates, MscError &error) const
{
    string s = string("Invalid value '");
    switch (type) {
    case MSC_ATTR_STRING: s << value; break;
    case MSC_ATTR_BOOL:   s << (yes?"yes":"no"); break;
    case MSC_ATTR_NUMBER: s << number; break;
    }
    s.append("' for attribute '").append(name);
    s.append("'. Use '").append(candidates).append("'. Ignoring attribute.");
    error.Error(*this, true, s);
}

void Attribute::InvalidAttrError(MscError &error) const
{
    string s;
    s << "Attribute  '" << name << "' cannot be applied here. Ignoring it.";
    error.Error(*this, false, s);
}

void Attribute::InvalidStyleError(MscError &error) const
{
    string s;
    s << "Undefined style: '" << name << "'. Ignoring it.";
    error.Error(*this, false, s);
}

bool Attribute::EnsureNotClear(MscError &error, StyleType t) const
{
    if (type != MSC_ATTR_CLEAR) return true;
    string s;
    switch (t) {
    case STYLE_STYLE: return true;
    case STYLE_ARC:
        s << "Can not unset element attribute '" << name << "'.";
        break;
    case STYLE_DEFAULT:
        s << "Can not unset attribute '" << name << "' of a default style.";
        break;
    case STYLE_OPTION:
        s << "Can not unset chart option '" << name << "'.";
        break;
    }
    s.append(" Ignoring attempt.");
    error.Error(*this, false, s, "Can only unset attributes of non-default styles.");
    return false;
}


/////////////////////////////////////////////////////////////////////////////

template<> const char EnumEncapsulator<MscLineType>::names[][15] =
    {"invalid", "none", "solid", "dotted", "dashed", "double", ""};

MscLineAttr::MscLineAttr() :
    type(true, LINE_SOLID), color(true, MscColorType(0,0,0)), width(true, 1.),
    radius(true, 0)
{}

MscLineAttr &MscLineAttr::operator +=(const MscLineAttr&a)
{
    if (a.type.first) type = a.type;
    if (a.color.first) color = a.color;
    if (a.width.first) width = a.width;
    if (a.radius.first) radius = a.radius;
    return *this;
};

bool MscLineAttr::operator == (const MscLineAttr &a)
{
    if (a.type.first != type.first) return false;
    if (type.first && a.type.second != type.second) return false;
    if (a.color.first != color.first) return false;
    if (color.first && !(a.color.second == color.second)) return false;
    if (a.width.first != width.first) return false;
    if (width.first && a.width.second != width.second) return false;
    if (a.radius.first != radius.first) return false;
    if (radius.first && a.radius.second != radius.second) return false;
    return true;
}

bool MscLineAttr::AddAttribute(const Attribute &a, Msc *msc, StyleType t)
{
    if (a.type == MSC_ATTR_STYLE) {
        if (msc->Contexts.top().styles.find(a.name) == msc->Contexts.top().styles.end()) {
            a.InvalidStyleError(msc->Error);
            return true;
        }
        const MscStyle &style = msc->Contexts.top().styles[a.name];
        if (style.f_line) operator +=(style.line);
        return true;
    }
    if (a.EndsWith("color")) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t))
                color.first = false;
            return true;
        }
        if (!a.CheckColor(msc->Contexts.top().colors, msc->Error)) return true;
        color.second = msc->Contexts.top().colors.GetColor(a.value);
        color.first = true;
        return true;
    }
    if (a.EndsWith("type")) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t))
                type.first = false;
            return true;
        }
        if (a.type == MSC_ATTR_STRING && Convert(a.value, type.second)) {
            type.first = true;
            return true;
        }
        a.InvalidValueError(CandidatesFor(type.second), msc->Error);
        return true;
    }
    if (a.EndsWith("width")) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t))
                width.first = false;
            return true;
        }
        if (a.type == MSC_ATTR_NUMBER && a.number>=0 && a.number<=10) {
            width.second = a.number;
            width.first = true;
            return true;
        }
        a.InvalidValueError("0..10", msc->Error);
        return true;
    }
    if (a.EndsWith("radius")) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t))
                radius.first = false;
            return true;
        }
        if (a.CheckType(MSC_ATTR_NUMBER, msc->Error)) {
            radius.second = a.number;
            radius.first = true;
            return true;
        }
        return true;
    }
    return false;
}

string MscLineAttr::Print(int ident) const
{
    string ss = "line(";
    if (type.first) ss << " type:" << PrintEnum(type.second);
    if (width.first) ss << " width:" << width.second;
    if (color.first) ss << " color:" << color.second.Print();
    return ss + ")";
}

MscFillAttr::MscFillAttr() :
    color(true, MscColorType(255,255,255)), gradient(true, GRADIENT_NONE)
{}

MscFillAttr &MscFillAttr::operator +=(const MscFillAttr&a)
{
    if (a.color.first) color = a.color;
    if (a.gradient.first) gradient = a.gradient;
    return *this;
};

bool MscFillAttr::operator == (const MscFillAttr &a)
{
    if (a.color.first != color.first) return false;
    if (color.first && !(a.color.second == color.second)) return false;
    if (a.gradient.first != gradient.first) return false;
    if (gradient.first && !(a.gradient.second == gradient.second)) return false;
    return true;
}

template<> const char EnumEncapsulator<MscGradientType>::names[][15] =
    {"invalid", "none", "out", "in", "down", "up", "left", "right", "button", ""};

bool MscFillAttr::AddAttribute(const Attribute &a, Msc *msc, StyleType t)
{
    if (a.type == MSC_ATTR_STYLE) {
        if (msc->Contexts.top().styles.find(a.name) == msc->Contexts.top().styles.end()) {
            a.InvalidStyleError(msc->Error);
            return true;
        }
        const MscStyle &style = msc->Contexts.top().styles[a.name];
        if (style.f_fill) operator +=(style.fill);
        return true;
    }
    if (a.EndsWith("color")) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t))
                color.first = false;
            return true;
        }
        if (!a.CheckColor(msc->Contexts.top().colors, msc->Error)) return true;
        color.second = msc->Contexts.top().colors.GetColor(a.value);
        color.first = true;
        return true;
    }
    if (a.EndsWith("gradient")) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t))
                gradient.first = false;
            return true;
        }
        if (a.type == MSC_ATTR_STRING && Convert(a.value, gradient.second)) {
            gradient.first = true;
            return true;
        }
        a.InvalidValueError(CandidatesFor(gradient.second), msc->Error);
        return true;
    }
    return false;
}

string MscFillAttr::Print(int ident) const
{
    string ss = "fill(";
    if (color.first) ss << " color:" << color.second.Print();
    if (gradient.first) ss << " gradient:" << PrintEnum(gradient.second);
    return ss + ")";
}


MscShadowAttr::MscShadowAttr() :
    color(true, MscColorType(0,0,0)), offset(true, 0), blur(true, 0)
{}

MscShadowAttr &MscShadowAttr::operator +=(const MscShadowAttr&a)
{
    if (a.color.first) color = a.color;
	if (a.offset.first) offset = a.offset;
	if (a.blur.first) blur = a.blur;
    return *this;
};

bool MscShadowAttr::operator == (const MscShadowAttr &a)
{
    if (a.color.first != color.first) return false;
    if (color.first && !(a.color.second == color.second)) return false;
    if (a.offset.first != offset.first) return false;
    if (offset.first && !(a.offset.second == offset.second)) return false;
    if (a.blur.first != blur.first) return false;
    if (blur.first && !(a.blur.second == blur.second)) return false;
    return true;
}

bool MscShadowAttr::AddAttribute(const Attribute &a, Msc *msc, StyleType t)
{
    if (a.type == MSC_ATTR_STYLE) {
        if (msc->Contexts.top().styles.find(a.name) == msc->Contexts.top().styles.end()) {
            a.InvalidStyleError(msc->Error);
            return true;
        }
        const MscStyle &style = msc->Contexts.top().styles[a.name];
        if (style.f_shadow) operator +=(style.shadow);
        return true;
    }
    if (a.EndsWith("color")) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t))
                color.first = false;
            return true;
        }
        if (!a.CheckColor(msc->Contexts.top().colors, msc->Error)) return true;
        color.second = msc->Contexts.top().colors.GetColor(a.value);
        color.first = true;
        return true;
    }
    if (a.EndsWith("offset")) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t))
                offset.first = false;
            return true;
        }
        if (a.type == MSC_ATTR_NUMBER && a.number>=0 && a.number<=100) {
            offset.first = true;
            offset.second = a.number;
            return true;
        }
        a.InvalidValueError("0..100", msc->Error);
        return true;
    }
    if (a.EndsWith("blur")) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t))
                blur.first = false;
            return true;
        }
        if (a.type == MSC_ATTR_NUMBER && a.number>=0 && a.number<=100) {
            blur.first = true;
            blur.second = a.number;
            return true;
        }
        a.InvalidValueError("0..100", msc->Error);
        return true;
    }
    return false;
}

string MscShadowAttr::Print(int ident) const
{
    string ss = "shadow(";
    if (color.first) ss << " color:" << color.second.Print();
    if (offset.first) ss << " offset:" << offset.second;
    if (blur.first) ss << " blur:" << blur.second;
    return ss + ")";
}
