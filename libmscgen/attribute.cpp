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
/** @file attribute.cpp The definition of basic classes for attributes and basic styles.
 * @ingroup libmscgen_files */
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
    //Here the end of `base` equals to `a`.
    //If i2<0 then `a` must equal to `base`, which is a true return.
    //Else, we return true only if `a` comes in `base` strictly after a dot.
    return i2<0 || base[i2]=='.';
}

/** Converts the attribute to a string
 * @param [in] ident Tells how much space to prepend each line.
 * @returns The string */
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

/** True if the (one or more) last (dot-separated) segments of the attribute name equal `a` (case insensitive)*/
bool Attribute::EndsWith(const char *a) const
{
    return CaseInsensitiveEndsWith(name.c_str(), a);
}

/** True if the (one or more) first (dot-separated) segments of the attribute name equal `a` (case insensitive)*/
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

/** Ensure that the attribute is of a certain type.
 *
 * If the attribute is not of type `t` we generate an error into `error` and
 * mark the attribute as one in error (which will result in ignoring it). 
 * @param [in] t The type the attribute shall be.
 * @param error The object we shall add our error message to.
 * @returns True if the types matched.*/
bool Attribute::CheckType(EAttrType t, MscError &error) const
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

/** Ensure that the attribute value specifies a color.
 *
 * If the attribute value is not a string that conforms to color specifications
 * (that is, not a name of a color and not an rgb value 
 * triplet, etc., see ColorSet::GetColor()) we generate an error into `error` and
 * mark the attribute as one in error (which will result in ignoring it). 
 * @param [in] colors A set of color names, we search the color name in.
 * @param error The object we shall add our error message to.
 * @returns True if the value specifies a color.*/
bool Attribute::CheckColor(const ColorSet &colors, MscError &error) const
{
    if (type==MSC_ATTR_STRING) {
        ColorType c = colors.GetColor(value);
        if (c.type!=ColorType::INVALID) return true;
    }
    string ss;
    ss << "Unrecognized color name or definition: '" << value << "'. Ignoring attribute.";
    error.Error(*this, true, ss);
    return false;
}

/** Marks the attribute as one having an invalid value and generates an appropriate error message.
 *
 * @param [in] candidates The slash ("/") separated list of value candidates that could be used instead of the faulty value.
 * @param error The object we shall add the error message to.*/
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

/** Marks the attribute as not applicable here and generates an appropriate error message.
 *
 * @param error The object we shall add the error message to.*/
void Attribute::InvalidAttrError(MscError &error) const
{
    string s;
    s << "Attribute  '" << name << "' cannot be applied here. Ignoring it.";
    error.Error(*this, false, s);
}

/** Marks the attribute an unknown style and generates an appropriate error message.
 *
 * We assume the attribute specifies a style (is of MSC_ATTR_STYLE type).
 * @param error The object we shall add the error message to.*/
void Attribute::InvalidStyleError(MscError &error) const
{
    _ASSERT(type==MSC_ATTR_STYLE);
    string s;
    s << "Undefined style: '" << name << "'. Ignoring it.";
    error.Error(*this, false, s);
}

/** Ensures the attribute has a value after the "=" sign.
 *
 * We generate an error message if not so.
 * Some attributes cannot be unset (or cleared), we check this here.
 * @param error The object we shall add the error message to.
 * @param [in] t The type of style the attribute is part of. */
bool Attribute::EnsureNotClear(MscError &error, EStyleType t) const
{
    if (type != MSC_ATTR_CLEAR) return true;
    string s;
    switch (t) {
    case STYLE_STYLE: 
    case STYLE_DEF_ADD:
        return true;
    case STYLE_ARC:
        s << "Can not unset element attribute '" << name << "'.";
        break;
    case STYLE_DEFAULT:
        //text and loss attributes are delta, so they may be unset
        if (StartsWith("text") || StartsWith("lost")) return true;
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

/** Possible values for a line type*/
template<> const char EnumEncapsulator<ELineType>::names[][ENUM_STRING_LEN] =
    {"invalid", "none", "solid", "dotted", "dashed", "long_dashed", "dash_dotted", 
     "double", "triple", "triple_thick", ""};

/** Possible values for a rectangle corner type*/
template<> const char EnumEncapsulator<ECornerType>::names[][ENUM_STRING_LEN] =
    {"invalid", "none", "round", "bevel", "note", ""};

/**Create a fully specified default line style. 
 * Solid line, black color, width of 1, zero radius and no corner. 
 * All attributes (`first` members) are set.*/
LineAttr::LineAttr() :
    type(true, LINE_SOLID), color(true, ColorType(0,0,0)), width(true, 1.),
    radius(true, 0), corner(true, CORNER_NONE)
{
}

/**Make the style fully specified using default line style values.
 * If the `first` member of any attribute is false, we set it true and
 * use the default value (Solid line, black color, width of 1, zero radius and no corner.)
 * All attributes (`first` members) are eventually set.
 * No change made to an already fully specified style.*/
void LineAttr::MakeComplete()
{
    if (!type.first) {type.first = true; type.second = LINE_SOLID;}
    if (!color.first) { color.first = true; color.second.r = color.second.g = color.second.b = 0; color.second.a = 255; color.second.type = ColorType::COMPLETE; }
    if (!width.first) {width.first = true; width.second = 1;}
    if (!corner.first) {corner.first = true; corner.second = CORNER_NONE;}
    if (!radius.first) {radius.first = true; radius.second = 0;}
}


const double * LineAttr::DashPattern(unsigned &num) const
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

LineAttr &LineAttr::operator +=(const LineAttr&a)
{
    if (a.type.first) type = a.type;
    if (a.color.first) {
        if (color.first)
            color.second += a.color.second;
        else
            color = a.color;
    }
    if (a.width.first) width = a.width;
    if (a.corner.first) corner = a.corner;
    if (a.radius.first) radius = a.radius;
    return *this;
};

bool LineAttr::operator == (const LineAttr &a)
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

/** Take an attribute and apply it to us.
 *
 * We consider attributes ending with 'color', 'width', 'type', 'corner' and 'radius'; 
 * or any style at the current context in `msc`. We also accept the clearing of
 * an attribute if `t` is STYLE_STYLE, that is for style definitions only.
 * At a problem, we generate an error into msc->Error.
 * @param [in] a The attribute to apply.
 * @param msc The chart we build.
 * @param [in] t The situation we set the attribute.
 * @returns True, if the attribute was recognized as ours (may have been a bad value though).*/
bool LineAttr::AddAttribute(const Attribute &a, Msc *msc, EStyleType t)
{
    if (a.type == MSC_ATTR_STYLE) {
        if (msc->Contexts.back().styles.find(a.name) == msc->Contexts.back().styles.end()) {
            a.InvalidStyleError(msc->Error);
            return true;
        }
        const MscStyle &style = msc->Contexts.back().styles[a.name].read();
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
        if (color.first)
            color.second += msc->Contexts.back().colors.GetColor(a.value);
        else {
            color.first = true;
            color.second = msc->Contexts.back().colors.GetColor(a.value);
        }
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

/** Add the attribute names we take to `csh`.*/
void LineAttr::AttributeNames(Csh &csh, const string &prefix)
{
    static const char names[][ENUM_STRING_LEN] =
    {"", "color", "type", "width", "radius", "corner", ""};
    csh.AddToHints(names, csh.HintPrefix(COLOR_ATTRNAME)+prefix, HINT_ATTR_NAME);
}

/** Callback for drawing a symbol before line type names in the hints popup list box.
 * @ingroup libmscgen_hintpopup_callbacks*/
bool CshHintGraphicCallbackForLineType(Canvas *canvas, CshHintGraphicParam p, Csh &)
{
    if (!canvas) return false;
    LineAttr line(ELineType(int(p)), ColorType(0,0,0), 1, CORNER_NONE, 0);
    canvas->Line(XY(HINT_GRAPHIC_SIZE_X*0.2, HINT_GRAPHIC_SIZE_Y*0.2), 
              XY(HINT_GRAPHIC_SIZE_X*0.8, HINT_GRAPHIC_SIZE_Y*0.8), line);
    return true;
}

/** Callback for drawing a symbol before corner type names in the hints popup list box.
 * @ingroup libmscgen_hintpopup_callbacks*/
bool CshHintGraphicCallbackForCornerType(Canvas *canvas, CshHintGraphicParam p, Csh &)
{
    if (!canvas) return false;
    canvas->Clip(XY(HINT_GRAPHIC_SIZE_X*0.2, HINT_GRAPHIC_SIZE_Y*0.2), 
              XY(HINT_GRAPHIC_SIZE_X*0.8, HINT_GRAPHIC_SIZE_Y*0.8));
    LineAttr line(LINE_SOLID, ColorType(0,0,0), 2, ECornerType(int(p)), ceil(HINT_GRAPHIC_SIZE_Y*0.3));
    canvas->Line(Block(-HINT_GRAPHIC_SIZE_X, HINT_GRAPHIC_SIZE_X*0.7, 
              HINT_GRAPHIC_SIZE_Y*0.3, HINT_GRAPHIC_SIZE_Y*2), line);
    canvas->UnClip();
    return true;
}

/** Add a list of possible attribute value names to `csh` for attribute `attr`.*/
bool LineAttr::AttributeValues(const std::string &attr, Csh &csh)
{
    if (CaseInsensitiveEndsWith(attr, "color")) {
        csh.AddColorValuesToHints();
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "type")) {
        csh.AddToHints(EnumEncapsulator<ELineType>::names, csh.HintPrefix(COLOR_ATTRVALUE), 
                       HINT_ATTR_VALUE, CshHintGraphicCallbackForLineType);
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "width")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable()+"<number in pixels>", HINT_ATTR_VALUE, false));
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "corner")) {
        csh.AddToHints(EnumEncapsulator<ECornerType>::names, csh.HintPrefix(COLOR_ATTRVALUE), 
                       HINT_ATTR_VALUE, CshHintGraphicCallbackForCornerType);
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "radius")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable()+"<number in pixels>", HINT_ATTR_VALUE, false));
        return true;
    }
    return false;
}

/** Print the line style to a string.*/
string LineAttr::Print(int) const
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
Contour LineAttr::CreateRectangle_Midline(double x1, double x2, double y1, double y2, double r) const
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
            std::vector<Edge> edges;
            edges.reserve(8);

            edges.emplace_back(XY(x1+r, y1), XY(x2-r, y1));
            Edge::GenerateEllipse(edges, XY(x2-r, y1+r), r, r, 0, 270, 360);
            edges.emplace_back(XY(x2, y1+r), XY(x2, y2-r));
            Edge::GenerateEllipse(edges, XY(x2-r, y2-r), r, r, 0, 0, 90);
            edges.emplace_back(XY(x2-r, y2), XY(x1+r, y2));
            Edge::GenerateEllipse(edges, XY(x1+r, y2-r), r, r, 0, 90, 180);
            edges.emplace_back(XY(x1, y2-r), XY(x1, y1+r));
            Edge::GenerateEllipse(edges, XY(x1+r, y1+r), r, r, 0, 180, 270);
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

/** Returns the fill area of a note-styled rectangle for double or triple line styles.
 *
 * The function assumes double or triple line style and CORNER_NOTE corner style. It 
 * considers line style, see an example below for
 * single, double and triple line styles.
 * @verbatim

   +------\       +----------\        +--------------\
   |       \      | +----+ |\ \       | +-----------\ \
   |       |      | |    | +-\ \      | | +-------+ |\ \ 
   +-------+      | |    +----+ \     | | |       | +-\ \
                  | |         | |     | | |       +--+ \ \
                  | +---------+ |     | | |          | | |
                  +-------------+     | | +----------+ | |
                                      | +--------------+ |
                                      +------------------+
   @endverbatim
 * It returns two SimpleContours, one L-shaped rectangle and the midline 
 * of the inner line of the double line; plus a small triangle, also at the middle of its line.
 * ret[0] will be the body and ret[1] will be the triangle.
 * At call time `this->radius` corresponds to the radius at the middle of the (double or triple) line.
 * `x1, x2, y1, y2` corresponds to the midline of the rectangle.*/
Contour LineAttr::CreateRectangle_ForFill_Note(double x1, double x2, double y1, double y2) const
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

/** Returns the inner area of a note-styled rectangle.
 *
 * The function assumes CORNER_NOTE corner style. It 
 * considers line style, see an example below for
 * single, double and triple line styles.
 * @verbatim

   +------\       +----------\        +--------------\
   |       \      | +----+ |\ \       | +-----------\ \
   |       |      | |    | +-\ \      | | +-------+ |\ \ 
   +-------+      | |    +----+ \     | | |       | +-\ \
                  | |         | |     | | |       +--+ \ \
                  | +---------+ |     | | |          | | |
                  +-------------+     | | +----------+ | |
                                      | +--------------+ |
                                      +------------------+
   @endverbatim
 * It returns one SimpleContour, the copped one for single lines and one L-shaped rectangle 
 * for double and triple line styles. The returned shape represents the inner edge of the line
 * surrounding it, thus inside it, nothing will be painted when we draw the line of the rectangle.
 * At call time `this->radius` corresponds to the radius at the middle of the (double or triple) line.
 * `x1, x2, y1, y2` corresponds to the midline of the rectangle.*/
Contour LineAttr::CreateRectangle_InnerEdge_Note(double x1, double x2, double y1, double y2) const
{
    _ASSERT(IsComplete());
    _ASSERT(corner.second == CORNER_NOTE);
    Block bb(x1, x2, y1, y2);
    LineAttr line2(*this);
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


/** Calculates how wide a rectangle should be when drawing one around a text label.
 *
 * See the situation below. @verbatim
      /---------------\
     /AAAAAAAAAAAAAAAAA\
    /                   \
   /                     \
   |                     |
   @endverbatim
 * Here we see the top of a bevelled rectangle with a label 'AAAAAAAAA' in it.
 * We can see that the vertical sides of the rectangle are spaced further apart than
 * the length of the label - this is because of the bevelled corners.
 * 
 * In this function the label is already laid out on the surface - its extent is captured in `textCover`.
 * We want the very top of the rectangle (the outer edge of its upper horizontal line) 
 * to be at the y coordinate `rect_top`. The rectangle is supposed to have corner style and radius
 * as specified in `this` along with the line width. (Radius corresponds to the midline.)
 * And checks how much margin the text needs form the _outer_edge_ of the rectangle
 * This is at least lineWidth() (if radius==0)
 * return first contains the left margin and second the right one
 * This one assumes that the 
 * @param [in] textCover The extents of the label
 * @param [in] rect_top The y coordinate of the outer edge of the upper horizontal line of the rectangle.
 * @returns The left and right margin to be used from the leftmost and rightmost point of `textCover`, resp 
 *          till the *outer* edge of the vertical sides of the rectangle.
 *          This margin is at least as much as the line width then. We return (0,0) if `textCover` is empty.
 */
DoublePair LineAttr::CalculateTextMargin(Contour textCover, double rect_top) const
{
    _ASSERT(IsComplete());
    DoublePair ret(0,0);
    if (textCover.IsEmpty()) return ret;
    const double lw = LineWidth();
    if (radius.second <= LineWidth() || corner.second==CORNER_NONE) return DoublePair(lw, lw);
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






/**Create a fully specified default fill style. 
 * White color, no gradients and color2 is not set.
 * (Fill is considered complete even if color2 is not set, because then
 * we derive it from the primary color.*/
FillAttr::FillAttr() :
    color(true, ColorType(255,255,255)), color2(false, ColorType(255,255,255)), 
    gradient(true, GRADIENT_NONE)
{
}

/**Make the style fully specified using default fill style values.
 * If the `first` member of any attribute is false, we set it true and
 * use the default value (White color, no gradients.) We leave `color2` as is.
 * (Fill is considered complete even if `color2` is not set, because then
 * we derive it from the primary color.)
 * All attributes (`first` members) are eventually set, except perhaps `color2`.
 * No change made to an already fully specified style.*/
void FillAttr::MakeComplete()
{
    if (!color.first) {color.first = true; color.second.r = color.second.g = color.second.b = color.second.a = 255;}
    //color2 is not needed for completeness
    if (!gradient.first) {gradient.first = true; gradient.second = GRADIENT_NONE;}
}


FillAttr &FillAttr::operator +=(const FillAttr&a)
{
    if (a.color.first) {
        if (color.first)
            color.second += a.color.second;
        else
            color= a.color;
    }
    if (a.color2.first) {
        if (color2.first)
            color2.second += a.color2.second;
        else
            color2.second = a.color2.second;
    }
    if (a.gradient.first) gradient = a.gradient;
    return *this;
};

bool FillAttr::operator == (const FillAttr &a) const
{
    if (a.color.first != color.first) return false;
    if (color.first && !(a.color.second == color.second)) return false;
    if (a.color2.first != color2.first) return false;
    if (color2.first && !(a.color2.second == color2.second)) return false;
    if (a.gradient.first != gradient.first) return false;
    if (gradient.first && !(a.gradient.second == gradient.second)) return false;
    return true;
}

/** Possible values for fill gradients.*/
template<> const char EnumEncapsulator<EGradientType>::names[][ENUM_STRING_LEN] =
    {"invalid", "none", "out", "in", "down", "up", "left", "right", "button", ""};

/** Take an attribute and apply it to us.
 *
 * We consider attributes ending with 'color', 'color2' and 'gradient'; 
 * or any style at the current context in `msc`. We also accept the clearing of
 * an attribute if `t` is STYLE_STYLE, that is for style definitions only.
 * At a problem, we generate an error into msc->Error.
 * @param [in] a The attribute to apply.
 * @param msc The chart we build.
 * @param [in] t The situation we set the attribute.
 * @returns True, if the attribute was recognized as ours (may have been a bad value though).*/
bool FillAttr::AddAttribute(const Attribute &a, Msc *msc, EStyleType t)
{
    if (a.type == MSC_ATTR_STYLE) {
        if (msc->Contexts.back().styles.find(a.name) == msc->Contexts.back().styles.end()) {
            a.InvalidStyleError(msc->Error);
            return true;
        }
        const MscStyle &style = msc->Contexts.back().styles[a.name].read();
        if (style.f_fill) operator +=(style.fill);
        return true;
    }
    if (a.EndsWith("color") || a.EndsWith("color2")) {
        std::pair<bool, ColorType> &c = (a.name[a.name.length()-1]=='2') ? color2 : color;
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t))
                c.first = false;
            return true;
        }
        if (!a.CheckColor(msc->Contexts.back().colors, msc->Error)) return true;
        if (c.first)
            c.second = msc->Contexts.back().colors.GetColor(a.value);
        else {
            c.first = true;
            c.second = msc->Contexts.back().colors.GetColor(a.value);
        }
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

/** Callback for drawing a symbol before gradient names in the hints popup list box.
 * @ingroup libmscgen_hintpopup_callbacks*/
bool CshHintGraphicCallbackForGradient(Canvas *canvas, CshHintGraphicParam p, Csh &)
{
    if (!canvas) return false;
    const int size = HINT_GRAPHIC_SIZE_Y-2;
    const int off_x = (HINT_GRAPHIC_SIZE_X - size)/2;
    const int off_y = 1;
    ColorType black(0,0,0);
    FillAttr fill(black, ColorType(255,255,255), EGradientType(int(p)));
    LineAttr line(LINE_SOLID, black, 1, CORNER_NONE, 0);
    Block rect(XY(off_x, off_y), XY(off_x+size, off_y+size));
    rect.Round().Shift(XY(.5,.5));
    canvas->Fill(rect, fill);
    canvas->Line(rect, line);
    return true;
}


/** Add the attribute names we take to `csh`.*/
void FillAttr::AttributeNames(Csh &csh, const string &prefix)
{
    static const char names[][ENUM_STRING_LEN] =
    {"", "color", "color2", "gradient", ""};
    csh.AddToHints(names, csh.HintPrefix(COLOR_ATTRNAME)+prefix, HINT_ATTR_NAME);
}

/** Add a list of possible attribute value names to `csh` for attribute `attr`.*/
bool FillAttr::AttributeValues(const std::string &attr, Csh &csh)
{
    if (CaseInsensitiveEndsWith(attr, "color") || CaseInsensitiveEndsWith(attr, "color2")) {
        csh.AddColorValuesToHints();
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "gradient")) {
        csh.AddToHints(EnumEncapsulator<EGradientType>::names, csh.HintPrefix(COLOR_ATTRVALUE), 
                       HINT_ATTR_VALUE, CshHintGraphicCallbackForGradient);
        return true;
    }
    return false;
}

/** Print the line style to a string.*/
string FillAttr::Print(int) const
{
    string ss = "fill(";
    if (color.first) ss << " color:" << color.second.Print();
    if (color2.first) ss << " color2:" << color2.second.Print();
    if (gradient.first) ss << " gradient:" << PrintEnum(gradient.second);
    return ss + ")";
}


/**Create a fully specified default shadow style. 
 * Black color, no offset (effectively no shadow) and no blur.*/
ShadowAttr::ShadowAttr() :
    color(true, ColorType(0,0,0)), offset(true, 0), blur(true, 0)
{}

/**Make the style fully specified using default shadow style values.
 * If the `first` member of any attribute is false, we set it true and
 * use the default value (Black color, no offset, no blur.) 
 * All attributes (`first` members) are eventually set.
 * No change made to an already fully specified style.*/
void ShadowAttr::MakeComplete()
{
    if (!color.first) {color.first = true; color.second.r = color.second.g = color.second.b = 0; color.second.a = 255;}
    if (!offset.first) {offset.first = true; offset.second = 0;}
    if (!blur.first) {blur.first = true; blur.second = 0;}
}

ShadowAttr &ShadowAttr::operator +=(const ShadowAttr&a)
{
    if (a.color.first) {
        if (color.first)
            color.second += a.color.second;
        else
            color = a.color;
    }
    if (a.offset.first) offset = a.offset;
	if (a.blur.first) blur = a.blur;
    return *this;
};

bool ShadowAttr::operator == (const ShadowAttr &a)
{
    if (a.color.first != color.first) return false;
    if (color.first && !(a.color.second == color.second)) return false;
    if (a.offset.first != offset.first) return false;
    if (offset.first && !(a.offset.second == offset.second)) return false;
    if (a.blur.first != blur.first) return false;
    if (blur.first && !(a.blur.second == blur.second)) return false;
    return true;
}

/** Take an attribute and apply it to us.
 *
 * We consider attributes ending with 'color', 'offset' and 'blur'; 
 * or any style at the current context in `msc`. We also accept the clearing of
 * an attribute if `t` is STYLE_STYLE, that is for style definitions only.
 * At a problem, we generate an error into msc->Error.
 * @param [in] a The attribute to apply.
 * @param msc The chart we build.
 * @param [in] t The situation we set the attribute.
 * @returns True, if the attribute was recognized as ours (may have been a bad value though).*/
bool ShadowAttr::AddAttribute(const Attribute &a, Msc *msc, EStyleType t)
{
    if (a.type == MSC_ATTR_STYLE) {
        if (msc->Contexts.back().styles.find(a.name) == msc->Contexts.back().styles.end()) {
            a.InvalidStyleError(msc->Error);
            return true;
        }
        const MscStyle &style = msc->Contexts.back().styles[a.name].read();
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
        if (color.first)
            color.second += msc->Contexts.back().colors.GetColor(a.value);
        else {
            color.first = true;
            color.second = msc->Contexts.back().colors.GetColor(a.value);
        }
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

/** Add the attribute names we take to `csh`.*/
void ShadowAttr::AttributeNames(Csh &csh)
{
    static const char names[][ENUM_STRING_LEN] =
    {"", "shadow.color", "shadow.offset", "shadow.blur", ""};
    csh.AddToHints(names, csh.HintPrefix(COLOR_ATTRNAME), HINT_ATTR_NAME);
}

/** Add a list of possible attribute value names to `csh` for attribute `attr`.*/
bool ShadowAttr::AttributeValues(const std::string &attr, Csh &csh)
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


/** Print the line style to a string.*/
string ShadowAttr::Print(int) const
{
    string ss = "shadow(";
    if (color.first) ss << " color:" << color.second.Print();
    if (offset.first) ss << " offset:" << offset.second;
    if (blur.first) ss << " blur:" << blur.second;
    return ss + ")";
}


/** Callback for drawing a symbol before 'yes' or 'no' in the hints popup list box.
 * @ingroup libmscgen_hintpopup_callbacks*/
bool CshHintGraphicCallbackForYesNo(Canvas *canvas, CshHintGraphicParam p, Csh &)
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

/////////////////////////////////////////////////////////////////////////////

bool WidthAttr::operator == (const WidthAttr &a) const
{
    if (a.first != first) return false;
    if (a.first && (a.second != second || (second<0 && a.str != str))) return false;
    return true;
}

/** Take an attribute and apply it to us.
*
* We consider attributes ending with 'width', 'pointer' and 'pos';
* or any style at the current context in `msc`. We also accept the clearing of
* an attribute if `t` is STYLE_STYLE, that is for style definitions only.
* At a problem, we generate an error into msc->Error.
* @param [in] a The attribute to apply.
* @param msc The chart we build.
* @param [in] t The situation we set the attribute.
* @returns True, if the attribute was recognized as ours (may have been a bad value though).*/
bool WidthAttr::AddAttribute(const Attribute &a, Msc *msc, EStyleType t)
{
    if (a.EndsWith("width")) {
        switch (a.type) {
        default:
        case MSC_ATTR_STYLE:
            _ASSERT(0);
        case MSC_ATTR_CLEAR:
            if (a.EnsureNotClear(msc->Error, t))
                first = false;
            break;
        case MSC_ATTR_NUMBER:
            first = true;
            second = a.number;
            str.clear();
            break;
        case MSC_ATTR_BOOL:
        case MSC_ATTR_STRING:
            first = true;
            second = -1;
            str = a.value;
            break;
        }
    }
    return false;
}

/** Add a list of possible attribute value names to `csh` for attribute `attr`.*/
bool WidthAttr::AttributeValues(const std::string &attr, Csh &csh)
{
    if (CaseInsensitiveEndsWith(attr, "width")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", HINT_ATTR_VALUE, false));
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<string>", HINT_ATTR_VALUE, false));
        return true;
    }
    return false;
}

/** Get the width in pixels for the value of the width attribute. 
 * Returns 0 if not set.*/
double WidthAttr::GetWidth(Canvas &canvas, const StringFormat &format) const
{
    if (!first) return 0;
    if (second>=0) return second;
    if (str.length()==0) return 0;
    return second = Label(str, canvas, format).getTextWidthHeight().x;
}


/**Make the style fully specified using default note style values.
 * If the `first` member of any attribute is false, we set it true and
 * use the default value (Callout type, no specific position preference.) 
 * All attributes (`first` members) are eventually set.
 * No change made to an already fully specified style.*/
void NoteAttr::MakeComplete()
{
    if (!pointer.first) {pointer.first = true; pointer.second = CALLOUT;}
    if (!def_float_dist.first) {def_float_dist.first = true; def_float_dist.second = 0;}
    if (!def_float_x.first) {def_float_x.first = true; def_float_x.second = 0;}
    if (!def_float_y.first) {def_float_y.first = true; def_float_y.second = 0;}
    width.MakeComplete();
}

NoteAttr &NoteAttr::operator +=(const NoteAttr&a)
{
    if (a.pointer.first) pointer = a.pointer;
    if (a.def_float_dist.first) def_float_dist = a.def_float_dist;
    if (a.def_float_x.first) def_float_x = a.def_float_x;
    if (a.def_float_y.first) def_float_y = a.def_float_y;
    width = a.width;
    return *this;
};

bool NoteAttr::operator == (const NoteAttr &a) const
{
    if (a.pointer.first != pointer.first) return false;
    if (pointer.first && !(a.pointer.second == pointer.second)) return false;
    if (a.def_float_dist.first != def_float_dist.first) return false;
    if (def_float_dist.first && !(a.def_float_dist.second == def_float_dist.second)) return false;
    if (a.def_float_x.first != def_float_x.first) return false;
    if (def_float_x.first && !(a.def_float_x.second == def_float_x.second)) return false;
    if (a.def_float_y.first != def_float_y.first) return false;
    if (def_float_y.first && !(a.def_float_y.second == def_float_y.second)) return false;
    if (a.width != width) return false;
    return true;
}

/** Take an attribute and apply it to us.
 *
 * We consider attributes ending with 'width', 'pointer' and 'pos'; 
 * or any style at the current context in `msc`. We also accept the clearing of
 * an attribute if `t` is STYLE_STYLE, that is for style definitions only.
 * At a problem, we generate an error into msc->Error.
 * @param [in] a The attribute to apply.
 * @param msc The chart we build.
 * @param [in] t The situation we set the attribute.
 * @returns True, if the attribute was recognized as ours (may have been a bad value though).*/
bool NoteAttr::AddAttribute(const Attribute &a, Msc *msc, EStyleType t)
{
    if (a.type == MSC_ATTR_STYLE) {
        if (msc->Contexts.back().styles.find(a.name) == msc->Contexts.back().styles.end()) {
            a.InvalidStyleError(msc->Error);
            return true;
        }
        const MscStyle &style = msc->Contexts.back().styles[a.name].read();
        if (style.f_note) operator +=(style.note);
        return true;
    }
    if (a.EndsWith("pointer")) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t))
                pointer.first = false;
            return true;
        }
        if (a.type == MSC_ATTR_STRING && Convert(a.value, pointer.second)) {
            pointer.first = true;
            return true;
        }
        a.InvalidValueError(CandidatesFor(pointer.second), msc->Error);
        return true;
    }
    if (a.EndsWith("pos")) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t))
                def_float_dist.first = def_float_x.first = def_float_y.first= false;
            return true;
        }
        EPosType tmp;
        if (a.type == MSC_ATTR_STRING && Convert(a.value, tmp)) {
            switch(tmp) {
            default:
            case POS_INVALID: _ASSERT(0); break;
            case POS_NEAR:   def_float_dist.first=true; def_float_dist.second=-1; break;
            case POS_FAR:    def_float_dist.first=true; def_float_dist.second=+1; break;
            case LEFT:       def_float_x.second=-1; def_float_y.second= 0; def_float_x.first=def_float_y.first=true; break;
            case RIGHT:      def_float_x.second=+1; def_float_y.second= 0; def_float_x.first=def_float_y.first=true; break;
            case UP:         def_float_x.second= 0; def_float_y.second=-1; def_float_x.first=def_float_y.first=true; break;
            case DOWN:       def_float_x.second= 0; def_float_y.second=+1; def_float_x.first=def_float_y.first=true; break;
            case LEFT_UP:    def_float_x.second=-1; def_float_y.second=-1; def_float_x.first=def_float_y.first=true; break;
            case LEFT_DOWN:  def_float_x.second=-1; def_float_y.second=+1; def_float_x.first=def_float_y.first=true; break;
            case RIGHT_UP:   def_float_x.second=+1; def_float_y.second=-1; def_float_x.first=def_float_y.first=true; break;
            case RIGHT_DOWN: def_float_x.second=+1; def_float_y.second=+1; def_float_x.first=def_float_y.first=true; break;
            }
            return true;
        }
        a.InvalidValueError(CandidatesFor(tmp), msc->Error);
        return true;
    }
    if (width.AddAttribute(a, msc, t))
        return true;
    return false;
}

/** Add the attribute names we take to `csh`.*/
void NoteAttr::AttributeNames(Csh &csh)
{
    static const char names[][ENUM_STRING_LEN] =
    {"", "note.pointer", "note.pos", "note.width", ""};
    csh.AddToHints(names, csh.HintPrefix(COLOR_ATTRNAME), HINT_ATTR_NAME);
}

/** Possible values for a note pointer type*/
template<> const char EnumEncapsulator<NoteAttr::EPointerType>::names[][ENUM_STRING_LEN] =
    {"invalid", "none", "callout", "arrow", "blockarrow", ""};

/** Possible values for a note pos attribute*/
template<> const char EnumEncapsulator<NoteAttr::EPosType>::names[][ENUM_STRING_LEN] =
    {"invalid", "near", "far", "left", "right", "up", "down", "left_up", "left_down", "right_up", "right_down", ""};

/** Add a list of possible attribute value names to `csh` for attribute `attr`.*/
bool NoteAttr::AttributeValues(const std::string &attr, Csh &csh)
{
    if (CaseInsensitiveEndsWith(attr, "pointer")) {
        csh.AddToHints(EnumEncapsulator<NoteAttr::EPointerType>::names, csh.HintPrefix(COLOR_ATTRVALUE), 
                       HINT_ATTR_VALUE, CshHintGraphicCallbackForPointer);
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "pos")) {
        csh.AddToHints(EnumEncapsulator<NoteAttr::EPosType>::names, csh.HintPrefix(COLOR_ATTRVALUE), 
                       HINT_ATTR_VALUE, CshHintGraphicCallbackForPos);
        return true;
    }
    if (WidthAttr::AttributeValues(attr, csh)) return true;
    return false;
}

/** Callback for drawing a symbol before note pointer type names in the hints popup list box.
 * @ingroup libmscgen_hintpopup_callbacks*/
bool NoteAttr::CshHintGraphicCallbackForPointer(Canvas *canvas, CshHintGraphicParam p, Csh &csh)
{
    if (!canvas) return false;
    const NoteAttr::EPointerType v = NoteAttr::EPointerType(p);
    if (v!=NoteAttr::ARROW && v!=NoteAttr::BLOCKARROW && 
        v!=NoteAttr::NONE && v!=NoteAttr::CALLOUT) 
        return false;
    const Block object(HINT_GRAPHIC_SIZE_X*0.7, HINT_GRAPHIC_SIZE_X, 0, HINT_GRAPHIC_SIZE_Y);
    const FillAttr object_fill(ColorType(128,128,128), GRADIENT_LEFT);
    const LineAttr object_line;
    Contour note;
    if (v==NoteAttr::CALLOUT) {
        const XY points[] ={XY(HINT_GRAPHIC_SIZE_X*0.7, HINT_GRAPHIC_SIZE_Y*0.6),
                            XY(HINT_GRAPHIC_SIZE_X*0.2, HINT_GRAPHIC_SIZE_Y*0.3),
                            XY(HINT_GRAPHIC_SIZE_X*0.2, HINT_GRAPHIC_SIZE_Y*0.0),
                            XY(HINT_GRAPHIC_SIZE_X*0.0, HINT_GRAPHIC_SIZE_Y*0.0),
                            XY(HINT_GRAPHIC_SIZE_X*0.0, HINT_GRAPHIC_SIZE_Y*1.0),
                            XY(HINT_GRAPHIC_SIZE_X*0.2, HINT_GRAPHIC_SIZE_Y*1.0),
                            XY(HINT_GRAPHIC_SIZE_X*0.2, HINT_GRAPHIC_SIZE_Y*0.5),
                            XY(HINT_GRAPHIC_SIZE_X*0.7, HINT_GRAPHIC_SIZE_Y*0.6)};
        note = Contour(points);
    } else {
        note = Block(0, HINT_GRAPHIC_SIZE_X*0.2, 0, HINT_GRAPHIC_SIZE_Y);
    }
    canvas->Clip(XY(1,1), XY(HINT_GRAPHIC_SIZE_X-1, HINT_GRAPHIC_SIZE_Y-1));
    //draw object we comment
    canvas->Fill(object, object_fill);
    canvas->Line(object.UpperLeft(), object.LowerLeft(), object_line);
    //draw note
    const LineAttr line(LINE_SOLID, ColorType(0,192,32), 1, CORNER_NONE, 0); //green-blue
    const FillAttr fill(line.color.second.Lighter(0.7), GRADIENT_NONE);
    const ShadowAttr shadow;
    canvas->Shadow(note, shadow);
    canvas->Fill(note, fill);
    canvas->Line(note, line);
    //draw arrow
    switch (v) {
    case NoteAttr::ARROW: 
        canvas->Clip(Block(HINT_GRAPHIC_SIZE_X*0.2-1, HINT_GRAPHIC_SIZE_X, 0, HINT_GRAPHIC_SIZE_Y));
        CshHintGraphicCallbackForArrows(canvas, MSC_ARROW_SOLID, MSC_ARROW_SMALL, false);
        canvas->UnClip();
        break;
    case NoteAttr::BLOCKARROW: 
        canvas->Clip(Block(HINT_GRAPHIC_SIZE_X*0.2-1, HINT_GRAPHIC_SIZE_X, 0, HINT_GRAPHIC_SIZE_Y));
        CshHintGraphicCallbackForBigArrows(canvas, (int)MSC_ARROW_SOLID, csh);
        canvas->UnClip();
    default:
        break;
    }
    canvas->UnClip();
    return true;
}

/** Callback for drawing a symbol before names of values for the note positions in the hints popup list box.
 * @ingroup libmscgen_hintpopup_callbacks*/
bool NoteAttr::CshHintGraphicCallbackForPos(Canvas *canvas, CshHintGraphicParam p, Csh &)
{
    if (!canvas) return false;
    double dist = 1;
    XY pos(+1, +1);
    switch(NoteAttr::EPosType(int(p))) {
    default:
    case NoteAttr::POS_INVALID: return false;
    case NoteAttr::POS_NEAR:   dist=0.8; break;
    case NoteAttr::POS_FAR:    dist=1.2; break;
    case NoteAttr::LEFT:       pos.x=-1; pos.y= 0; break;
    case NoteAttr::RIGHT:      pos.x=+1; pos.y= 0; break;
    case NoteAttr::UP:         pos.x= 0; pos.y=-1; break;
    case NoteAttr::DOWN:       pos.x= 0; pos.y=+1; break;
    case NoteAttr::LEFT_UP:    pos.x=-1; pos.y=-1; break;
    case NoteAttr::LEFT_DOWN:  pos.x=-1; pos.y=+1; break;
    case NoteAttr::RIGHT_UP:   pos.x=+1; pos.y=-1; break;
    case NoteAttr::RIGHT_DOWN: pos.x=+1; pos.y=+1; break;
    }

    const double r0 = 0.4; //object rectangle offset
    const double r1 = 0.3; //object rectangle halfsize
    const double r2 = 0.15; //note rectangle halfsize
    const double r3 = 0.65; //note rectangle midpoint offset

    canvas->Clip(XY(1,1), XY(HINT_GRAPHIC_SIZE_X-1, HINT_GRAPHIC_SIZE_Y-1));
    //the center of the object
    const XY center(HINT_GRAPHIC_SIZE_X*(0.5-pos.x*r0), HINT_GRAPHIC_SIZE_Y*(0.5-pos.y*r0));
    //the tip of the note (on the contour of the object)
    const XY tip = XY(pos.x*HINT_GRAPHIC_SIZE_X*r1, pos.y*HINT_GRAPHIC_SIZE_Y*r1) + center;
    //the center of the note box
    const XY ori = XY(pos.x*HINT_GRAPHIC_SIZE_X*r3, pos.y*HINT_GRAPHIC_SIZE_Y*r3)*dist + center;
    //the halfsize of the note box
    const XY wh = XY(HINT_GRAPHIC_SIZE_X*r2, HINT_GRAPHIC_SIZE_Y*r2);
    //the offset of the pointer's base from "ori" (1.5 pixels)
    const XY para = (ori-center).Rotate90CW().Normalize()*1.5;

    //Draw object
    const Contour object = Contour(-HINT_GRAPHIC_SIZE_X*r1, HINT_GRAPHIC_SIZE_X*r1,
                         -HINT_GRAPHIC_SIZE_Y*r1, HINT_GRAPHIC_SIZE_Y*r1).Shift(center);
    canvas->Fill(object, FillAttr(ColorType(128,128,128), GRADIENT_NONE));
    canvas->Line(object, LineAttr());
    //Draw note
    const Contour c = Contour(Block(ori-wh, ori+wh)) + Contour(tip, ori+para, ori-para);
    const LineAttr line(LINE_SOLID, ColorType(0,192,32), 1, CORNER_NONE, 0); //green-blue
    const FillAttr fill(line.color.second.Lighter(0.7), GRADIENT_NONE);
    const ShadowAttr shadow;
    canvas->Shadow(c, shadow);
    canvas->Fill(c, fill);
    canvas->Line(c, line);
    canvas->UnClip();
    return true;
}


/** Print the line style to a string.*/
string NoteAttr::Print(int) const
{
    string ss = "note(";
    if (pointer.first) ss << " pointer:" << EnumEncapsulator<NoteAttr::EPointerType>::names[pointer.second];
    if (def_float_dist.first) ss << " def_float_dist:" << def_float_dist.second;
    if (def_float_x.first) ss << " def_float_x:" << def_float_x.second;
    if (def_float_y.first) ss << " def_float_y:" << def_float_y.second;
    return ss + ")";
}
