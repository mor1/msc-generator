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
#include <cassert>
#include <cstring>
#include "msc.h"


unsigned CaseInsensitiveCommonPrefixLen(const char *a, const char *b)
{
    if (a==NULL || b==NULL) return 0;
    unsigned i=0;
    while(a[i] && b[i]) {
        if (tolower(a[i]) != tolower(b[i]))
            return i;
        i++;
    }
    return i;
}

//0 if a does not begin with b
//1 if a begins with b
//2 if a == b
//Rule 1: "" begins with a NULL, but is not equal to it
//Rule 2: every string begins with a "" or a NULL
int CaseInsensitiveBeginsWith(const char *a, const char *b)
{
    if (b==NULL) return a==NULL?2:1;
    if (a==NULL) return 0;
    unsigned i=CaseInsensitiveCommonPrefixLen(a, b);
    if (b[i]==0) return a[i]==0?2:1;
    return 0;
}

bool CaseInsensitiveEndsWith(const char *base, const char *a)
{
    //strlen is size_t which may be bigger than unsigned - but I expect the value to fit an int
    int i1 = (int)(unsigned)strlen(a)-1;
    int i2 = (int)(unsigned)strlen(base)-1;
    if (i1>i2 || i1<0 || i2<0) return false;
    while(i1>=0 && i2>=0) {
        if (toupper(a[i1]) != toupper(base[i2]))
            return false;
        i1--; i2--;
    }
    return i2<0 || base[i2]=='.';
}

string Attribute::Print(int ident) const
{
    string s(ident*2,' ');
    s << name ;
    switch (type) {
    case MSC_ATTR_STYLE:
        s << " (style)";
        break;
    case MSC_ATTR_CLEAR:
        s << "=";
        break;
    case MSC_ATTR_STRING:
        s << "=" << value;
        break;
    case MSC_ATTR_NUMBER:
        s << "=" << number;
        break;
    case MSC_ATTR_BOOL:
        s << "=" << (yes?"yes":"no");
        break;
    };
    return s;
};

//Case insensitive comparison with name
bool Attribute::EndsWith(const char *a) const
{
    return CaseInsensitiveEndsWith(name.c_str(), a);
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
    case MSC_ATTR_STYLE:
        error.Error(*this, true, "Expecting a style here instead of an attribute.");
        break;
    case MSC_ATTR_CLEAR:
        error.Error(*this, true, "I expect no value for attribute '" + name + "'.");
        break;
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
    default:
        _ASSERT(0);
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

template<> const char EnumEncapsulator<MscLineType>::names[][ENUM_STRING_LEN] =
    {"invalid", "none", "solid", "dotted", "dashed", "long_dashed", "dash_dotted", 
     "double", "triple", "triple_thick", ""};

template<> const char EnumEncapsulator<MscCornerType>::names[][ENUM_STRING_LEN] =
    {"invalid", "none", "round", "bevel", "note", ""};

MscLineAttr::MscLineAttr() :
    type(true, LINE_SOLID), color(true, MscColorType(0,0,0)), width(true, 1.),
    radius(true, 0), corner(true, CORNER_NONE)
{
}

void MscLineAttr::MakeComplete()
{
    if (!type.first) {type.first = true; type.second = LINE_SOLID;}
    if (!color.first) {color.first = true; color.second.r = color.second.g = color.second.b = 0; color.second.a = 255;}
    if (!width.first) {width.first = true; width.second = 1;}
    if (!corner.first) {corner.first = true; corner.second = CORNER_NONE;}
    if (!radius.first) {radius.first = true; radius.second = 0;}
}


const double * MscLineAttr::DashPattern(unsigned &num) const
{
    //last number is sum of the ones before
    static const double dash_dotted[]={2, 2, 4};
    static const double dash_dashed[]={6, 6, 12};
    static const double dash_long_dashed[]={12, 12, 24};
    static const double dash_dash_dot[]={6, 4, 2, 4, 16};
    static const double dash_solid[] ={0};
    _ASSERT(type.first && type.second!=LINE_INVALID);
    switch (type.second) {
    case LINE_DOTTED: num = 2; return dash_dotted;
    case LINE_DASHED: num = 2; return dash_dashed;
    case LINE_LONG_DASHED: num = 2; return dash_long_dashed;
    case LINE_DASH_DOT: num = 4; return dash_dash_dot;
    default: num = 0; return dash_solid;
    }
}

MscLineAttr &MscLineAttr::operator +=(const MscLineAttr&a)
{
    if (a.type.first) type = a.type;
    if (a.color.first) color = a.color;
    if (a.width.first) width = a.width;
    if (a.corner.first) corner = a.corner;
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
    if (a.corner.first != corner.first) return false;
    if (corner.first && a.corner.second != corner.second) return false;
    return true;
}

bool MscLineAttr::AddAttribute(const Attribute &a, Msc *msc, StyleType t)
{
    if (a.type == MSC_ATTR_STYLE) {
        if (msc->Contexts.back().styles.find(a.name) == msc->Contexts.back().styles.end()) {
            a.InvalidStyleError(msc->Error);
            return true;
        }
        const MscStyle &style = msc->Contexts.back().styles[a.name];
        if (style.f_line) operator +=(style.line);
        return true;
    }
    if (a.EndsWith("color")) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t))
                color.first = false;
            return true;
        }
        if (!a.CheckColor(msc->Contexts.back().colors, msc->Error)) return true;
        color.second = msc->Contexts.back().colors.GetColor(a.value);
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
    if (a.EndsWith("corner")) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t))
                corner.first = false;
            return true;
        }
        if (a.type == MSC_ATTR_STRING && Convert(a.value, corner.second)) {
            corner.first = true;
            if (!radius.first || radius.second<=0) {
                radius.first = true;
                radius.second = 10;
            }
            return true;
        }
        a.InvalidValueError(CandidatesFor(corner.second), msc->Error);
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
            if (a.number<0) {
                corner.second = CORNER_NONE;
            } else if (!corner.first || corner.second == CORNER_NONE) {
                corner.first = true;
                corner.second = CORNER_ROUND;
            }
            return true;
        }
        return true;
    }
    return false;
}

void MscLineAttr::AttributeNames(Csh &csh)
{
    static const char names[][ENUM_STRING_LEN] =
    {"", "line.color", "line.type", "line.width", "line.radius", "line.corner", ""};
    csh.AddToHints(names, csh.HintPrefix(COLOR_ATTRNAME), HINT_ATTR_NAME);
}

bool CshHintGraphicCallbackForLineType(MscCanvas *canvas, CshHintGraphicParam p)
{
    if (!canvas) return false;
    MscLineAttr line(MscLineType(int(p)), MscColorType(0,0,0), 1, CORNER_NONE, 0);
    canvas->Line(XY(HINT_GRAPHIC_SIZE_X*0.2, HINT_GRAPHIC_SIZE_Y*0.2), 
              XY(HINT_GRAPHIC_SIZE_X*0.8, HINT_GRAPHIC_SIZE_Y*0.8), line);
    return true;
}

bool CshHintGraphicCallbackForCornerType(MscCanvas *canvas, CshHintGraphicParam p)
{
    if (!canvas) return false;
    canvas->Clip(XY(HINT_GRAPHIC_SIZE_X*0.2, HINT_GRAPHIC_SIZE_Y*0.2), 
              XY(HINT_GRAPHIC_SIZE_X*0.8, HINT_GRAPHIC_SIZE_Y*0.8));
    MscLineAttr line(LINE_SOLID, MscColorType(0,0,0), 2, MscCornerType(int(p)), ceil(HINT_GRAPHIC_SIZE_Y*0.3));
    canvas->Line(Block(-HINT_GRAPHIC_SIZE_X, HINT_GRAPHIC_SIZE_X*0.7, 
              HINT_GRAPHIC_SIZE_Y*0.3, HINT_GRAPHIC_SIZE_Y*2), line);
    canvas->UnClip();
    return true;
}


bool MscLineAttr::AttributeValues(const std::string &attr, Csh &csh)
{
    if (CaseInsensitiveEndsWith(attr, "color")) {
        csh.AddColorValuesToHints();
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "type")) {
        csh.AddToHints(EnumEncapsulator<MscLineType>::names, csh.HintPrefix(COLOR_ATTRVALUE), 
                       HINT_ATTR_VALUE, CshHintGraphicCallbackForLineType);
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "width")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable()+"<number in pixels>", HINT_ATTR_VALUE, false));
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "corner")) {
        csh.AddToHints(EnumEncapsulator<MscCornerType>::names, csh.HintPrefix(COLOR_ATTRVALUE), 
                       HINT_ATTR_VALUE, CshHintGraphicCallbackForCornerType);
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "radius")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable()+"<number in pixels>", HINT_ATTR_VALUE, false));
        return true;
    }
    return false;
}

string MscLineAttr::Print(int) const
{
    string ss = "line(";
    if (type.first) ss << " type:" << PrintEnum(type.second);
    if (width.first) ss << " width:" << width.second;
    if (color.first) ss << " color:" << color.second.Print();
    return ss + ")";
}

//This one does not assume anything about wether the resulting rectange should be the
//outer edge or inner edge of the line - just uses the radius value and coordinates
//as they are.
//"this->radius" corresponds to the radius at the middle of the line
//"x1, x2, y1, y2" corresponds to the midline -> this is what is returned
//For CORNER_NOTE it creates the outer line only 
Contour MscLineAttr::CreateRectangle_Midline(double x1, double x2, double y1, double y2, double r) const
{
    if (r==-1 && radius.first) 
        r = radius.second;
    if (r<=0 || !corner.first) 
        return Contour(x1, x2, y1, y2);
    Contour ret;
    r = std::min(r, MaxRadius(x1, x2, y1, y2));
    switch (corner.second) {
    default: 
        return Contour(x1, x2, y1, y2);
    case CORNER_ROUND:
        {
        const Edge edges[] = {
            Edge(XY(x1+r, y1), XY(x2-r, y1)),
            Edge(XY(x2-r, y1+r), r, r, 0, 270, 360),
            Edge(XY(x2, y1+r), XY(x2, y2-r)),
            Edge(XY(x2-r, y2-r), r, r, 0,   0,  90),
            Edge(XY(x2-r, y2), XY(x1+r, y2)),
            Edge(XY(x1+r, y2-r), r, r, 0,  90, 180),
            Edge(XY(x1, y2-r), XY(x1, y1+r)),
            Edge(XY(x1+r, y1+r), r, r, 0, 180, 270)};
        ret.assign_dont_check(edges);
        break;
        }
    case CORNER_BEVEL:
        {
        const XY points[] = {
            XY(x1+r, y1), 
            XY(x2-r, y1),
            XY(x2, y1+r),
            XY(x2, y2-r),
            XY(x2-r, y2),
            XY(x1+r, y2),
            XY(x1, y2-r),
            XY(x1, y1+r)};
        ret.assign_dont_check(points);
        break;
        }
    case CORNER_NOTE:
        {
        const XY points[] = {
            XY(x1, y1), 
            XY(x2-r, y1),
            XY(x2, y1+r),
            XY(x2, y2),
            XY(x1, y2)};
        ret.assign_dont_check(points);
        break;
        }
    }
    return ret;
}

//This one considers linewidth and returns the middle of the inner line (for double or triple)
//It assumes double or triple lines.
//"this->radius" corresponds to the radius at the middle of the line
//"x1, x2, y1, y2" corresponds to the midline 
//For CORNER_NOTE it creates the inner line and the triangle
//(//ret[0] will be the body and ret[1] will be the triangle)
Contour MscLineAttr::CreateRectangle_ForFill_Note(double x1, double x2, double y1, double y2) const
{
    _ASSERT(IsComplete());
    _ASSERT(corner.second == CORNER_NOTE);
    _ASSERT(IsDoubleOrTriple());
    Block bb(x1, x2, y1, y2);
    const double s = Spacing();
    const double r1 = radius.second;
    const double r2 = std::max(0., r1 - s*::RadiusIncMultiplier(corner.second));
    const double r3 = r2 + 2*width.second;
    Contour ret;
    if (IsDouble()) {
        bb.Expand(-s);
        const XY c2[] = {XY(bb.x.till - r2, bb.y.from + r2),
                         XY(bb.x.till - r2, bb.y.from),
                         XY(bb.x.till     , bb.y.from + r2)};
        const XY c1[] = {bb.UpperLeft(), 
                         XY(bb.x.till - r3, bb.y.from), 
                         XY(bb.x.till - r3, bb.y.from + r3), 
                         XY(bb.x.till     , bb.y.from + r3), 
                         bb.LowerRight(),
                         bb.LowerLeft()};
        ret.assign_dont_check(c1);
        ret.append_dont_check(c2);
    } else {
        //IsTriple()
        const XY c2[] = {XY(bb.x.till - r1, bb.y.from + r1),
                         XY(bb.x.till - r1, bb.y.from),
                         XY(bb.x.till     , bb.y.from + r1)};
        bb.Expand(-s);
        const XY c1[] = {bb.UpperLeft(), 
                         XY(bb.x.till - r1, bb.y.from), 
                         XY(bb.x.till - r1, bb.y.from + r1), 
                         XY(bb.x.till     , bb.y.from + r1), 
                         bb.LowerRight(),
                         bb.LowerLeft()};
        ret.assign_dont_check(c1);
        ret.append_dont_check(c2);
    }
    return ret;
}

//This one considers linewidth and returns the inner edge of the inner line 
//"this->radius" corresponds to the radius at the middle of the line
//"x1, x2, y1, y2" corresponds to the midline 
//For CORNER_NOTE it creates the inner line only
Contour MscLineAttr::CreateRectangle_InnerEdge_Note(double x1, double x2, double y1, double y2) const
{
    _ASSERT(IsComplete());
    _ASSERT(corner.second == CORNER_NOTE);
    Block bb(x1, x2, y1, y2);
    MscLineAttr line2(*this);
    const double lw2 = line2.LineWidth()/2;
    bb.Expand(-lw2);
    line2.Expand(-lw2);
    const double r = line2.radius.second + 2*line2.width.second;
    const XY c1[] = {bb.UpperLeft(), 
                     XY(bb.x.till - r, bb.y.from), 
                     XY(bb.x.till - r, bb.y.from + r), 
                     XY(bb.x.till    , bb.y.from + r), 
                     bb.LowerRight(),
                     bb.LowerLeft()};
    Contour ret;
    ret.assign_dont_check(c1);
    return ret;
}



//This assumes that we draw a rectangle at outer edge y position rect_top 
//with the corner specified in "this".
//And checks how much margin the text needs form the _outer_edge_ of the rectangle
//This is at least lineWidth() (if radius==0)
//return first contains the left margin and second the right one
//This one assumes that the radius corresponds to the midline
DoublePair MscLineAttr::CalculateTextMargin(Contour textCover, double rect_top) const
{
    DoublePair ret(0,0);
    if (textCover.IsEmpty()) return ret;
    const double lw = LineWidth();
    if (radius.second <= LineWidth()) return DoublePair(lw, lw);
    //create a path at the inner edge of the rectangle 
    const XY lr = textCover.GetBoundingBox().LowerRight();
    const Block inner(lw/2, radius.second*3, rect_top+lw/2, rect_top+lw/2 +lr.y+radius.second*2);
    const Contour inner_area = CreateRectangle_InnerEdge(inner); //the radius we have in the style luckily corresponds to the line midpoint
    const Range left_right = textCover.GetBoundingBox().x;
    textCover.Rotate(90);

    double off, tp;
    //left margin
    Contour a = Contour(0, inner.x.MidPoint(), inner.y.from-1, inner.y.till+1) - inner_area;
    a.Rotate(90);
    off = a.OffsetBelow(textCover, tp, CONTOUR_INFINITY);
    ret.first = left_right.from - off;
    //right margin
    a = Contour(inner.x.MidPoint(), inner.x.till+lw, inner.y.from-1, inner.y.till+1) - inner_area;
    a.Rotate(90);
    off = textCover.OffsetBelow(a, tp, CONTOUR_INFINITY);
    ret.second = inner.x.till-off-left_right.till + lw;
    return ret;
}







MscFillAttr::MscFillAttr() :
    color(true, MscColorType(255,255,255)), color2(false, MscColorType(255,255,255)), 
    gradient(true, GRADIENT_NONE)
{
}

void MscFillAttr::MakeComplete()
{
    if (!color.first) {color.first = true; color.second.r = color.second.g = color.second.b = color.second.a = 255;}
    //color2 is not needed for completeness
    if (!gradient.first) {gradient.first = true; gradient.second = GRADIENT_NONE;}
}


MscFillAttr &MscFillAttr::operator +=(const MscFillAttr&a)
{
    if (a.color.first) color = a.color;
    if (a.color2.first) color2 = a.color2;
    if (a.gradient.first) gradient = a.gradient;
    return *this;
};

bool MscFillAttr::operator == (const MscFillAttr &a)
{
    if (a.color.first != color.first) return false;
    if (color.first && !(a.color.second == color.second)) return false;
    if (a.color2.first != color2.first) return false;
    if (color2.first && !(a.color2.second == color2.second)) return false;
    if (a.gradient.first != gradient.first) return false;
    if (gradient.first && !(a.gradient.second == gradient.second)) return false;
    return true;
}

template<> const char EnumEncapsulator<MscGradientType>::names[][ENUM_STRING_LEN] =
    {"invalid", "none", "out", "in", "down", "up", "left", "right", "button", ""};

bool MscFillAttr::AddAttribute(const Attribute &a, Msc *msc, StyleType t)
{
    if (a.type == MSC_ATTR_STYLE) {
        if (msc->Contexts.back().styles.find(a.name) == msc->Contexts.back().styles.end()) {
            a.InvalidStyleError(msc->Error);
            return true;
        }
        const MscStyle &style = msc->Contexts.back().styles[a.name];
        if (style.f_fill) operator +=(style.fill);
        return true;
    }
    if (a.EndsWith("color") || a.EndsWith("color2")) {
        std::pair<bool, MscColorType> &c = (a.name[a.name.length()-1]=='2') ? color2 : color;
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t))
                c.first = false;
            return true;
        }
        if (!a.CheckColor(msc->Contexts.back().colors, msc->Error)) return true;
        c.second = msc->Contexts.back().colors.GetColor(a.value);
        c.first = true;
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

bool CshHintGraphicCallbackForGradient(MscCanvas *canvas, CshHintGraphicParam p)
{
    if (!canvas) return false;
    const int size = HINT_GRAPHIC_SIZE_Y-2;
    const int off_x = (HINT_GRAPHIC_SIZE_X - size)/2;
    const int off_y = 1;
    MscColorType black(0,0,0);
    MscFillAttr fill(black, MscColorType(255,255,255), MscGradientType(int(p)));
    MscLineAttr line(LINE_SOLID, black, 1, CORNER_NONE, 0);
    Block rect(XY(off_x, off_y), XY(off_x+size, off_y+size));
    rect.Round().Shift(XY(.5,.5));
    canvas->Fill(rect, fill);
    canvas->Line(rect, line);
    return true;
}


void MscFillAttr::AttributeNames(Csh &csh)
{
    static const char names[][ENUM_STRING_LEN] =
    {"", "fill.color", "fill.color2", "fill.gradient", ""};
    csh.AddToHints(names, csh.HintPrefix(COLOR_ATTRNAME), HINT_ATTR_NAME);
}

bool MscFillAttr::AttributeValues(const std::string &attr, Csh &csh)
{
    if (CaseInsensitiveEndsWith(attr, "color") || CaseInsensitiveEndsWith(attr, "color2")) {
        csh.AddColorValuesToHints();
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "gradient")) {
        csh.AddToHints(EnumEncapsulator<MscGradientType>::names, csh.HintPrefix(COLOR_ATTRVALUE), 
                       HINT_ATTR_VALUE, CshHintGraphicCallbackForGradient);
        return true;
    }
    return false;
}

string MscFillAttr::Print(int) const
{
    string ss = "fill(";
    if (color.first) ss << " color:" << color.second.Print();
    if (color2.first) ss << " color2:" << color2.second.Print();
    if (gradient.first) ss << " gradient:" << PrintEnum(gradient.second);
    return ss + ")";
}


MscShadowAttr::MscShadowAttr() :
    color(true, MscColorType(0,0,0)), offset(true, 0), blur(true, 0)
{}

void MscShadowAttr::MakeComplete()
{
    if (!color.first) {color.first = true; color.second.r = color.second.g = color.second.b = color.second.a = 255;}
    if (!offset.first) {offset.first = true; offset.second = 0;}
    if (!blur.first) {blur.first = true; blur.second = 0;}
}

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
        if (msc->Contexts.back().styles.find(a.name) == msc->Contexts.back().styles.end()) {
            a.InvalidStyleError(msc->Error);
            return true;
        }
        const MscStyle &style = msc->Contexts.back().styles[a.name];
        if (style.f_shadow) operator +=(style.shadow);
        return true;
    }
    if (a.EndsWith("color")) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t))
                color.first = false;
            return true;
        }
        if (!a.CheckColor(msc->Contexts.back().colors, msc->Error)) return true;
        color.second = msc->Contexts.back().colors.GetColor(a.value);
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

void MscShadowAttr::AttributeNames(Csh &csh)
{
    static const char names[][ENUM_STRING_LEN] =
    {"", "shadow.color", "shadow.offset", "shadow.blur", ""};
    csh.AddToHints(names, csh.HintPrefix(COLOR_ATTRNAME), HINT_ATTR_NAME);
}

bool MscShadowAttr::AttributeValues(const std::string &attr, Csh &csh)
{
    if (CaseInsensitiveEndsWith(attr, "color")) {
        csh.AddColorValuesToHints();
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "offset") ||
        CaseInsensitiveEndsWith(attr, "blur")) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+"<number in pixels>", HINT_ATTR_VALUE));
        return true;
    }
    return false;
}


string MscShadowAttr::Print(int) const
{
    string ss = "shadow(";
    if (color.first) ss << " color:" << color.second.Print();
    if (offset.first) ss << " offset:" << offset.second;
    if (blur.first) ss << " blur:" << blur.second;
    return ss + ")";
}


bool CshHintGraphicCallbackForYesNo(MscCanvas *canvas, CshHintGraphicParam p)
{
    if (!canvas) return false;
    canvas->Clip(XY(1,1), XY(HINT_GRAPHIC_SIZE_X-1, HINT_GRAPHIC_SIZE_Y-1));
    cairo_t *cr = canvas->GetContext();
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
    cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);  //UnClip will remove these
    cairo_set_line_width(cr, 3);
    if (int(p)) {
        cairo_set_source_rgb(cr, 0, 0.8, 0); //green
        cairo_move_to(cr, HINT_GRAPHIC_SIZE_X*0.3, HINT_GRAPHIC_SIZE_Y*0.6);
        cairo_line_to(cr, HINT_GRAPHIC_SIZE_X*0.4, HINT_GRAPHIC_SIZE_Y*0.9);
        cairo_line_to(cr, HINT_GRAPHIC_SIZE_X*0.7, HINT_GRAPHIC_SIZE_Y*0.2);
    } else {
        cairo_set_source_rgb(cr, 1, 0, 0); //red
        const XY xy(HINT_GRAPHIC_SIZE_X/2, HINT_GRAPHIC_SIZE_Y/2);
        const double off = HINT_GRAPHIC_SIZE_Y*0.3;
        cairo_move_to(cr, xy.x-off, xy.y-off);
        cairo_line_to(cr, xy.x+off, xy.y+off);
        cairo_move_to(cr, xy.x-off, xy.y+off);
        cairo_line_to(cr, xy.x+off, xy.y-off);
    }
    cairo_stroke(cr);
    canvas->UnClip();
    return true;
}

