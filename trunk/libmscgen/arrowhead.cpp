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
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include "msc.h"

template<> const char EnumEncapsulator<MscArrowType>::names[][ENUM_STRING_LEN] =
    {"invalid", "none", "solid", "empty", "line", "half", "diamond", "empty_diamond", 
     "dot", "empty_dot", "sharp", "empty_sharp",
     "double", "double_empty", "double_line", "double_half",
     "triple", "triple_empty", "triple_line", "triple_half",
     "empty_inv", "stripes", "triangle_stripes", ""};
template<> const char EnumEncapsulator<MscArrowSize>::names[][ENUM_STRING_LEN] =
    {"invalid", "tiny", "small", "normal", "big", "huge", ""};

double ArrowHead::arrowSizePercentage[6] = {
    20,  /* INVALID, value is set for big arrow hint pictograms */
    30,  /* MSC_ARROW_TINY */
    50,  /* MSC_ARROW_SMALL */
    75,  /* MSC_ARROW_NORMAL */
    100,  /* MSC_ARROW_BIG */
    130   /* MSC_ARROW_HUGE */
};

double ArrowHead::baseArrowWidth = 17;    /* Arrow size for BIG */
double ArrowHead::baseArrowHeight = 10;    /* Half Arrow size for BIG */
double ArrowHead::baseDiamondSize = 10;    /* Half Diamond size for BIG */
double ArrowHead::baseDotSize = 9;    /* Dot radius for BIG */

const double SHARP_MUL_1 = 1.2;  /* How much wider is a sharp arrow than a normal one */
const double SHARP_MUL_2 = 0.7;  /* What percentage of the width of a sharp arrow is at the line */

void ArrowHead::Empty() {
    line.Empty();
    size.first = false;
    endType.first = false;
    midType.first = false;
    startType.first = false;
    xmul.first = false;
    ymul.first = false;
}

ArrowHead & ArrowHead::operator += (const ArrowHead &toadd)
{
    const ArrowHead &a = static_cast<const ArrowHead&>(toadd);
    line += a.line;
    if (a.size.first) size = a.size;
    if (a.endType.first) endType = a.endType;
    if (a.midType.first) midType = a.midType;
    if (a.startType.first) startType = a.startType;
    if (a.xmul.first) xmul = a.xmul;
    if (a.ymul.first) ymul = a.ymul;
    return *this;
}


bool ArrowHead::AddAttribute(const Attribute &a, Msc *msc, StyleType t)
{
    if (a.type == MSC_ATTR_STYLE) {
        if (msc->Contexts.back().styles.find(a.name) == msc->Contexts.back().styles.end()) {
            a.InvalidStyleError(msc->Error);
            return true;
        }
        const MscStyle &style = msc->Contexts.back().styles[a.name];
        if (style.f_arrow) operator +=(style.arrow);
        return true;
    }
    std::pair<bool, MscArrowType> *pType = NULL, *pType2 = NULL;
    if (a.Is("arrow") || a.EndsWith("type") || a.EndsWith("endtype")) {pType = &endType; pType2=&midType;}
    else if (a.EndsWith("starttype")) pType = &startType;
    else if (a.EndsWith("midtype")) pType = &midType;
    if (pType) {
        if (a.Is("arrow"))
            msc->Error.Warning(a, "Option/Attribute 'arrow' is deprecated, but understood.",
                                "Use 'arrow.type'.");
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t)) {
                pType->first = false;
                if (pType2) pType2->first = false;
            }
            return true;
        }
        MscArrowType at;
        if (a.type == MSC_ATTR_STRING &&
            Convert(a.value, at)) {
            if (type == ARROW && !MSC_ARROW_OK_FOR_ARROWS(at)) {
                msc->Error.Error(a, true, "Value '"+a.value+"' is applicable only to block arrows.");
                return true;
            }
            if (type == BIGARROW && !MSC_ARROW_OK_FOR_BIG_ARROWS(at)) {
                msc->Error.Error(a, true, "Value '"+a.value+"' is not applicable to block arrows.");
                return true;
            }
            pType->first = true;
            pType->second = at;
            if (pType2) *pType2 = *pType;
            return true;
        }
        a.InvalidValueError(CandidatesFor(pType->second), msc->Error);
        return true;
    }
    if (a.Is("arrowsize") || a.EndsWith("size")) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t))
                size.first = false;
            return true;
        }
        if (a.type == MSC_ATTR_STRING &&
            Convert(a.value, size.second)) {
            size.first = true;
        if (a.Is("arrowsize"))
            msc->Error.Warning(a, "Option/Attribute 'arrowsize' is deprecated, but understood.",
                               "Use 'arrow.size'.");
            return true;
        }
        a.InvalidValueError(CandidatesFor(size.second), msc->Error);
        return true;
    }
    if (a.EndsWith("xmul") || a.EndsWith("ymul")) {
        std::pair<bool, double> &mul = a.EndsWith("xmul") ? xmul : ymul;
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t))
                mul.first = false;
            return true;
        }
        if (a.type == MSC_ATTR_NUMBER && a.number>=0.1 && a.number<=10) {
            mul.second = a.number;
            mul.first = true;
            return true;
        }
        a.InvalidValueError("0.1 .. 10", msc->Error);
        return true;
    }
    if (a.EndsWith("color") || a.EndsWith("line.width")) {
        return line.AddAttribute(a, msc, t);
    }
    return false;
}

void ArrowHead::AttributeNames(Csh &csh)
{
    static const char names[][ENUM_STRING_LEN] =
    {"invalid", "arrow.type", "arrow.size", "arrow.color", "arrow.starttype", "arrow.midtype",
     "arrow.endtype", "line.width", "arrow.xmul", "arrow.ymul", ""};
    csh.AddToHints(names, csh.HintPrefix(COLOR_ATTRNAME), HINT_ATTR_NAME);
}

bool CshHintGraphicCallbackForArrows(MscCanvas *canvas, MscArrowType type, MscArrowSize size, bool left);

bool CshHintGraphicCallbackForBigArrows(MscCanvas *canvas, CshHintGraphicParam p)
{
    if (!canvas) return false;
    if (!MSC_ARROW_OK_FOR_BIG_ARROWS((MscArrowType)(int)p)) {
        if (MSC_ARROW_OK_FOR_ARROWS((MscArrowType)(int)p))
            return CshHintGraphicCallbackForArrows(canvas, (MscArrowType)(int)p, MSC_ARROW_SMALL, false);
        else return false;
    }
    std::vector<double> xPos(2); 
    xPos[0] = 0;
    xPos[1] = HINT_GRAPHIC_SIZE_X*0.7;
    MscLineAttr eLine(LINE_SOLID, MscColorType(0,0,0), 1, CORNER_NONE, 0);
    canvas->Clip(XY(HINT_GRAPHIC_SIZE_X*0.1,1), XY(HINT_GRAPHIC_SIZE_X-1, HINT_GRAPHIC_SIZE_Y-1));
    canvas->Line(XY(xPos[1], 1), XY(xPos[1], HINT_GRAPHIC_SIZE_Y-1), eLine);
    ArrowHead ah(ArrowHead::BIGARROW);
    ah.line += MscColorType(0,192,32); //green-blue
    ah.endType.second = (MscArrowType)(int)p;
    ah.size.second = MSC_ARROWS_INVALID;
    MscShadowAttr shadow;
    MscFillAttr fill(ah.line.color.second.Lighter(0.7), GRADIENT_UP);
    std::vector<double> active(2,0.);
    ah.BigCalculateAndDraw(xPos, active, HINT_GRAPHIC_SIZE_Y*0.3, HINT_GRAPHIC_SIZE_Y*0.7, 
                           true, false, fill, shadow, *canvas);
    canvas->UnClip();
    return true;
}

bool CshHintGraphicCallbackForArrows(MscCanvas *canvas, MscArrowType type, MscArrowSize size, bool left)
{
    if (!canvas) return false;
    const double xx = left ? 0.9 : 0.7;
    XY xy(HINT_GRAPHIC_SIZE_X*xx, HINT_GRAPHIC_SIZE_Y/2);
    MscLineAttr eLine(LINE_SOLID, MscColorType(0,0,0), 1, CORNER_NONE, 0);
    ArrowHead ah;
    ah.line += MscColorType(0,192,32); //green-blue
    ah.endType.second = type;
    ah.size.second = size;
    Range cover = ah.EntityLineCover(xy, true, false, MSC_ARROW_END).GetBoundingBox().y;
    canvas->Clip(XY(1,1), XY(HINT_GRAPHIC_SIZE_X-1, HINT_GRAPHIC_SIZE_Y-1));
    if (cover.from>1)
        canvas->Line(XY(xy.x, 1), XY(xy.x, cover.from), eLine);
    if (cover.till<HINT_GRAPHIC_SIZE_Y-1)
        canvas->Line(XY(xy.x, cover.till), XY(xy.x, HINT_GRAPHIC_SIZE_Y-1), eLine);
    Area clip = ah.ClipForLine(xy, 0, true, false, MSC_ARROW_END, Block(XY(0,0), canvas->GetSize()), eLine, eLine);
    canvas->Clip(clip);
    canvas->Line(XY(HINT_GRAPHIC_SIZE_X*0.1, xy.y), xy, ah.line);
    canvas->UnClip();
    ah.Draw(xy, 0, true, false, MSC_ARROW_END, ah.line, ah.line, canvas);
    canvas->UnClip();
    return true;
}

bool CshHintGraphicCallbackForArrowTypes(MscCanvas *canvas, CshHintGraphicParam p)
{
    if (!MSC_ARROW_OK_FOR_ARROWS((MscArrowType)(int)p)) {
        if (MSC_ARROW_OK_FOR_BIG_ARROWS((MscArrowType)(int)p))
            return CshHintGraphicCallbackForBigArrows(canvas, p);
        else return false;
    }
    return CshHintGraphicCallbackForArrows(canvas, (MscArrowType)(int)p, MSC_ARROW_SMALL, false);
}

bool CshHintGraphicCallbackForArrowSizes(MscCanvas *canvas, CshHintGraphicParam p)
{
    return CshHintGraphicCallbackForArrows(canvas, MSC_ARROW_SOLID, (MscArrowSize)(int)p, true);
}

bool ArrowHead::AttributeValues(const std::string &attr, Csh &csh, ArcType t)
{
    if (CaseInsensitiveEqual(attr, "arrow") ||
        CaseInsensitiveEndsWith(attr, "type") ||
        CaseInsensitiveEndsWith(attr, "starttype") ||
        CaseInsensitiveEndsWith(attr, "midtype") ||
        CaseInsensitiveEndsWith(attr, "endtype")) {
        for (int i=1; EnumEncapsulator<MscArrowType>::names[i][0]; i++)
            if (t==ANY || (t==BIGARROW && MSC_ARROW_OK_FOR_BIG_ARROWS(MscArrowType(i))) || 
                          (t==ARROW && MSC_ARROW_OK_FOR_ARROWS(MscArrowType(i))))
                csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE) + EnumEncapsulator<MscArrowType>::names[i],
                               HINT_ATTR_VALUE, true, 
                               t==BIGARROW ? CshHintGraphicCallbackForBigArrows : CshHintGraphicCallbackForArrowTypes,
                               CshHintGraphicParam(i)));
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "xmul") || CaseInsensitiveEndsWith(attr, "ymul")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable()+"<multiplier between 0.1 and 10>", HINT_ATTR_VALUE, false));
        return true;
    }
    if (CaseInsensitiveEqual(attr, "arrowsize") ||
        CaseInsensitiveEndsWith(attr, "size")) {
        csh.AddToHints(EnumEncapsulator<MscArrowSize>::names, csh.HintPrefix(COLOR_ATTRVALUE), 
                       HINT_ATTR_VALUE, CshHintGraphicCallbackForArrowSizes);
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "color") ||
        CaseInsensitiveEndsWith(attr, "line.width")) {
        return MscLineAttr::AttributeValues(attr, csh);
    }
    return false;

}

MscArrowType ArrowHead::GetType(bool bidir, MscArrowEnd which) const
{
    if (bidir && (which == MSC_ARROW_START))
        which=MSC_ARROW_END;
    switch(which) {
    case MSC_ARROW_END:
        if (endType.first) return endType.second;
        else return MSC_ARROW_SOLID;
    case MSC_ARROW_MIDDLE:
        if (midType.first) return midType.second;
        else return MSC_ARROW_SOLID;
    case MSC_ARROW_START:
        if (startType.first) return startType.second;
        else return MSC_ARROW_NONE;
    }
    return MSC_ARROW_NONE;
}

//"forward" is true if arrow is from left to right
//"left" is true if we are interested in the arrow to draw to the left side of the entity line
//for symmetric types (dot) this gives the same irrespective of "left"
//this can return MSC_ARROW_INVALID for start and end arrows (outside part)
MscArrowType ArrowHead::GetType(bool forward, bool bidir, MscArrowEnd which, bool left) const
{
    MscArrowType ret = GetType(bidir, which);
    if (MSC_ARROW_IS_SYMMETRIC(ret)) return ret;
    switch (which) {
    case MSC_ARROW_START: 
        return forward == left ? MSC_ARROW_INVALID : ret;
    case MSC_ARROW_MIDDLE:
        if (bidir) return ret;
        return forward == left ? ret : MSC_ARROW_NONE;
    case MSC_ARROW_END:
        return forward == left ? ret : MSC_ARROW_INVALID;
    }
    _ASSERT(0);
    return MSC_ARROW_NONE;
}


XY ArrowHead::getWidthHeight(bool bidir, MscArrowEnd which) const
{
    XY xy;
    double sizePercentage;
    if (size.first) sizePercentage = arrowSizePercentage[size.second];
    else sizePercentage = arrowSizePercentage[MSC_ARROW_SMALL];
    switch(GetType(bidir, which))
    {
    case MSC_ARROW_NONE:
        xy.x = 0;
        xy.y = 0;
        break;

    case MSC_ARROW_SOLID: /* Filled */
    case MSC_ARROW_EMPTY: /* Non-Filled */
    case MSC_ARROW_LINE: /* Two lines */
    case MSC_ARROW_HALF: /* Unfilled half */
        xy.x = baseArrowWidth*sizePercentage/100;
        xy.y = baseArrowHeight*sizePercentage/100;
        break;

    case MSC_ARROW_SHARP:
    case MSC_ARROW_SHARP_EMPTY:
        xy.x = baseArrowWidth*SHARP_MUL_1*sizePercentage/100;
        xy.y = baseArrowHeight*sizePercentage/100;
        break;

    case MSC_ARROW_DOUBLE:
    case MSC_ARROW_DOUBLE_EMPTY:
        xy.x = 2 * baseArrowWidth*sizePercentage/100;
        xy.y = baseArrowHeight*sizePercentage/100;
        break;

    case MSC_ARROW_DOUBLE_LINE:
    case MSC_ARROW_DOUBLE_HALF:
        xy.x = 1.5 * baseArrowWidth*sizePercentage/100;
        xy.y = baseArrowHeight*sizePercentage/100;
        break;

    case MSC_ARROW_TRIPLE:
    case MSC_ARROW_TRIPLE_EMPTY:
        xy.x = 3 * baseArrowWidth*sizePercentage/100;
        xy.y = baseArrowHeight*sizePercentage/100;
        break;
    case MSC_ARROW_TRIPLE_LINE:
    case MSC_ARROW_TRIPLE_HALF:
        xy.x = 2 * baseArrowWidth*sizePercentage/100;
        xy.y = baseArrowHeight*sizePercentage/100;
        break;

    case MSC_ARROW_DIAMOND:
    case MSC_ARROW_DIAMOND_EMPTY:
        xy.x = xy.y = baseDiamondSize*sizePercentage/100;
        break;

    case MSC_ARROW_DOT:
    case MSC_ARROW_DOT_EMPTY:
        xy.x = xy.y = baseDotSize*sizePercentage/100;
        break;
    default:
        _ASSERT(0);
    }
    if (xmul.first) xy.x *= xmul.second;
    if (ymul.first) xy.y *= ymul.second;
    return xy;
}

double ArrowHead::getTriWidth(bool bidir, MscArrowEnd which) const
{
    const double w = getWidthHeight(bidir, which).x;
    switch(GetType(bidir, which)) {
    case MSC_ARROW_TRIPLE_LINE:
    case MSC_ARROW_TRIPLE_HALF:
    case MSC_ARROW_DOUBLE:
    case MSC_ARROW_DOUBLE_EMPTY:
        return w/2;

    case MSC_ARROW_DOUBLE_LINE:
    case MSC_ARROW_DOUBLE_HALF:
        return w/1.5;

    case MSC_ARROW_TRIPLE:
    case MSC_ARROW_TRIPLE_EMPTY:
        return w/3;
    default:
        return w;
    }
}


/* return value:
 * - first is the extent on the left side of the entity line
 * - second is the extent on the right side of the entity line
 * formard is true if the start entiry of the arrow is left from its destination (like -> and not like <-)
 * if which is start, we swap the two values (as we should, to get the above)
 * If forLine is true, we return how much of the arrow line the arrowhead covers
 * If false, we return how much the text margin should be from the entity line
 */
DoublePair ArrowHead::getWidths(bool forward, bool bidir, MscArrowEnd which, const MscLineAttr &) const
{
    DoublePair ret(0,0);
    const MscArrowType t = GetType(bidir, which);
    switch (t) {
    case MSC_ARROW_NONE: /* No end */
        break;
    case MSC_ARROW_SOLID: /* Filled */
    case MSC_ARROW_EMPTY: /* Non-Filled */
    case MSC_ARROW_HALF: /* Unfilled half */
    case MSC_ARROW_LINE: /* Two lines */
    case MSC_ARROW_SHARP:
    case MSC_ARROW_SHARP_EMPTY:
    case MSC_ARROW_DOUBLE:
    case MSC_ARROW_DOUBLE_EMPTY:
    case MSC_ARROW_DOUBLE_LINE:
    case MSC_ARROW_DOUBLE_HALF:
    case MSC_ARROW_TRIPLE:
    case MSC_ARROW_TRIPLE_EMPTY:
    case MSC_ARROW_TRIPLE_LINE:
    case MSC_ARROW_TRIPLE_HALF:
        ret.first = getWidthHeight(bidir, which).x;
        //Now see if we put the value to the right of first/second
        if (bidir && (which==MSC_ARROW_MIDDLE))
            ret.second = ret.first;
        else if ( (!forward) ^ (which==MSC_ARROW_START)) //if reverse or start, but not if both
            ret.swap();
        break;

    case MSC_ARROW_DIAMOND:
    case MSC_ARROW_DIAMOND_EMPTY:
    case MSC_ARROW_DOT:
    case MSC_ARROW_DOT_EMPTY:
    default:
        ret.second = ret.first = getWidthHeight(bidir, which).x;
    }
    return ret;
}

Contour diamond(XY xy, XY wh)
{
    wh.x = fabs(wh.x);
    wh.y = fabs(wh.y);
    XY points[] = {XY(xy.x,      xy.y-wh.y), 
                   XY(xy.x+wh.x, xy.y     ), 
                   XY(xy.x,      xy.y+wh.y), 
                   XY(xy.x-wh.x, xy.y     )};
    return Contour(points, 4);
}

/** Return the Y range for which the entity line shall not be drawn
 */
Contour ArrowHead::EntityLineCover(XY xy, bool /*forward*/, bool bidir, MscArrowEnd which) const
{
    XY wh = getWidthHeight(bidir, which);
    Contour ret;
    switch(GetType(bidir, which)) {
    case MSC_ARROW_DIAMOND_EMPTY:
    case MSC_ARROW_DIAMOND:
        ret = diamond(xy, wh);
        break;

    case MSC_ARROW_DOT_EMPTY:
    case MSC_ARROW_DOT:
        ret = Contour(xy, wh.x, wh.y);
        break;
    default:
        break; //No other arrow type covers the entity line
    }
    return ret;
}

Contour ArrowHead::ClipForLine(XY xy, double act_size, bool forward, bool bidir, MscArrowEnd which, const Block &total,
    const MscLineAttr &mainline_left, const MscLineAttr &mainline_right) const
{
    XY act(act_size, 0);
    XY wh = getWidthHeight(bidir, which);
    double w = getTriWidth(bidir, which);
    if (bidir && which == MSC_ARROW_START)
        forward = !forward;
    if (forward) {
        wh.x *= -1;
        w *= -1;
        act.x *= -1;
    }
    std::swap(wh.x, w);
    //wh now contains the triangle size, w the size of the entire stuff (same for all,but DOUBLE & TRIPLE)

    MscFillAttr fill(line.color.second);
    Contour tri1(xy+act+wh, xy+act, xy+act + wh - XY(0, wh.y) - XY(0, wh.y));
    Contour tri2(xy-act-wh, xy-act, xy-act - wh + XY(0, wh.y) + XY(0, wh.y));
    const Contour tri_sharp1(xy+act+wh, xy+act+XY(wh.x*SHARP_MUL_2, 0), xy+act + wh - XY(0, wh.y) - XY(0, wh.y));
    const Contour tri_sharp2(xy-act-wh, xy-act-XY(wh.x*SHARP_MUL_2, 0), xy-act - wh + XY(0, wh.y) + XY(0, wh.y));
    Range r;  //skip this x range (area can contain parts though)
    Contour area;
    XY half_offset(0,0);

    switch(GetType(bidir, which)) {
    case MSC_ARROW_NONE:
        r.from = xy.x+act_size;
        r.till = xy.x-act_size;
        break;

    case MSC_ARROW_SHARP:
    case MSC_ARROW_SHARP_EMPTY:
        area = tri_sharp1;
        if (bidir && (which==MSC_ARROW_MIDDLE)) area += tri_sharp2;
        /* Fallthrough for calculating r */
    case MSC_ARROW_SOLID: /* Filled */
    case MSC_ARROW_EMPTY: /* Non-Filled */
    case MSC_ARROW_DOUBLE:
    case MSC_ARROW_DOUBLE_EMPTY:
    case MSC_ARROW_TRIPLE:
    case MSC_ARROW_TRIPLE_EMPTY:
        r.from = xy.x + act.x + w ;
        r.till = (bidir && (which==MSC_ARROW_MIDDLE)) ? xy.x - act.x - w : xy.x - act.x;
        if (r.from>r.till) std::swap(r.from, r.till);
        break;

    case MSC_ARROW_TRIPLE_LINE:
        tri1.Shift(XY( wh.x/2,0));
        tri2.Shift(XY(-wh.x/2,0));
        //Fallthrough
    case MSC_ARROW_DOUBLE_LINE:
        tri1.Shift(XY( wh.x/2,0));
        tri2.Shift(XY(-wh.x/2,0));
        //Fallthrough
    case MSC_ARROW_LINE: /* Two lines */
        area = tri1;
        if (bidir && (which==MSC_ARROW_MIDDLE)) area += tri2;
        r = area.GetBoundingBox().x;
        r += xy.x; //expand range so that it includes the vertical line
        break;

    case MSC_ARROW_TRIPLE_HALF:
        //half_offset.x += wh.x/2;
        //Fallthrough
    case MSC_ARROW_DOUBLE_HALF:
        //half_offset.x += wh.x/2;
        //Fallthrough
    case MSC_ARROW_HALF: /* Unfilled half */
        area = Contour(xy+act+wh+half_offset, xy+act+half_offset, xy+act + XY(wh.x, 0)+half_offset);
        if (bidir && (which==MSC_ARROW_MIDDLE))
            area += Contour(xy-act + XY(-wh.x, wh.y)-half_offset, xy-act-half_offset, xy-act - XY(wh.x, 0)-half_offset);
        r = area.GetBoundingBox().x;
        r += xy.x; //expand so that it includes the vertical line
        //now allow the top half of the line to be visible
        if (r.from < xy.x) {
            if (mainline_left.IsDoubleOrTriple()) 
                area += Block(r.from, xy.x, total.y.from, xy.y-(mainline_left.LineWidth()/2-mainline_left.width.second));
            area += Block(r.from, r.from+fabs(wh.x), total.y.from, xy.y);
        }
        if (r.till > xy.x) {
            if (mainline_right.IsDoubleOrTriple()) 
                area += Block(xy.x, r.till, total.y.from, xy.y-(mainline_right.LineWidth()/2-mainline_right.width.second));
            area += Block(r.till-fabs(wh.x), r.till, total.y.from, xy.y);
        }
        break;

    case MSC_ARROW_DIAMOND:
    case MSC_ARROW_DIAMOND_EMPTY:
        area = diamond(xy, wh);
        r = area.GetBoundingBox().x;
        area = Area(Block(r, total.y)) - area;
        break;

    case MSC_ARROW_DOT:
    case MSC_ARROW_DOT_EMPTY:
        area = Contour(xy, wh.x, wh.y);
        r = area.GetBoundingBox().x;
        area = Area(Block(r, total.y)) - area;
        break;
    default:
        _ASSERT(0);
    }
    if (r.from==r.till) 
        area = total;
    else {
        if (r.from>r.till) std::swap(r.from, r.till);
        if (total.x.from < r.from)
            area += Block(total.x.from, r.from, total.y.from, total.y.till);
        if (total.x.till > r.till)
            area += Block(r.till, total.x.till, total.y.from, total.y.till);
    }
    return area;
}


Contour ArrowHead::Cover(XY xy, double act_size, bool forward, bool bidir, MscArrowEnd which,
                         const MscLineAttr &mainline_left, const MscLineAttr &mainline_right) const
{
    XY act(act_size, 0);
    XY wh = getWidthHeight(bidir, which);
    double w = getTriWidth(bidir, which);
    if (bidir && which == MSC_ARROW_START)
        forward = !forward;
    if (forward) {
        wh.x *= -1;
        w *= -1;
        act.x *= -1;
    }
    std::swap(wh.x, w);
    //wh now contains the triangle size, w the size of the entire stuff (same for all,but DOUBLE & TRIPLE)

    MscFillAttr fill(line.color.second);
    const Contour tri1(xy+act+wh, xy+act, xy+act + wh - XY(0, wh.y) - XY(0, wh.y));
    const Contour tri2(xy-act-wh, xy-act, xy-act - wh + XY(0, wh.y) + XY(0, wh.y));
    const Contour tri_sharp1(xy+act+wh, xy+act+XY(wh.x*SHARP_MUL_2, 0), xy+act + wh - XY(0, wh.y) - XY(0, wh.y));
    const Contour tri_sharp2(xy-act-wh, xy-act-XY(wh.x*SHARP_MUL_2, 0), xy-act - wh + XY(0, wh.y) + XY(0, wh.y));

    Contour area;

    switch(GetType(bidir, which)) {
    case MSC_ARROW_NONE:
        break;
    case MSC_ARROW_TRIPLE:
    case MSC_ARROW_TRIPLE_EMPTY:
        area += tri1.CreateShifted(XY(wh.x*2, 0));
        if (bidir && (which==MSC_ARROW_MIDDLE)) area += tri2.CreateShifted(XY(-wh.x*2, 0));
        /*Fallthrough*/
    case MSC_ARROW_DOUBLE:
    case MSC_ARROW_DOUBLE_EMPTY:
        area += tri1.CreateShifted(XY(wh.x, 0));
        if (bidir && (which==MSC_ARROW_MIDDLE)) area += tri2.CreateShifted(XY(-wh.x, 0));
        /*Fallthrough*/
    case MSC_ARROW_SOLID: /* Filled */
    case MSC_ARROW_EMPTY: /* Non-Filled */
    case MSC_ARROW_LINE: /* Two lines */
    case MSC_ARROW_HALF: /* Unfilled half */
        area += tri1;
        if (bidir && (which==MSC_ARROW_MIDDLE)) area += tri2;
        break;

    case MSC_ARROW_TRIPLE_LINE:
    case MSC_ARROW_DOUBLE_LINE:
    case MSC_ARROW_DOUBLE_HALF:
    case MSC_ARROW_TRIPLE_HALF:
        area += Contour(xy.x+act.x+w, xy.x+act.x+wh.x, xy.y-wh.y, xy.y+wh.y) + tri1;
        if (bidir && (which==MSC_ARROW_MIDDLE)) 
            area += Contour(xy.x-act.x-wh.x, xy.x-act.x-w, xy.y-wh.y, xy.y+wh.y) + tri2;
        break;

    case MSC_ARROW_SHARP:
    case MSC_ARROW_SHARP_EMPTY:
        area = tri1 - tri_sharp1;
        if (bidir && (which==MSC_ARROW_MIDDLE)) area += tri2 - tri_sharp2;
        break;

    case MSC_ARROW_DIAMOND:
    case MSC_ARROW_DIAMOND_EMPTY:
        area = diamond(xy, wh);
        break;

    case MSC_ARROW_DOT:
    case MSC_ARROW_DOT_EMPTY:
        area = Contour(xy, wh.x, wh.y);
        break;
    default:
        _ASSERT(0);
    }
    if (line.LineWidth()>1)
        area = area.CreateExpand(line.LineWidth()/2);
    if (MSC_ARROW_IS_HALF(GetType(bidir, which))) 
        area -= Block(-MSC_BIG_COORD, +MSC_BIG_COORD, -MSC_BIG_COORD, xy.y);
    return area;
}


/** Draw an arrowhead pointing to the right or to the left.
 * x,y is the tip of the arrow
 * which is 0 for destination, 1 for middle, 2 for source end of arrow
 */
void ArrowHead::Draw(XY xy, double act_size, bool forward, bool bidir, MscArrowEnd which, 
                     const MscLineAttr &mainline_left, const MscLineAttr &mainline_right, 
                     MscCanvas *canvas) const
{
    const MscArrowType arrow_type = GetType(bidir, which);
    if (arrow_type == MSC_ARROW_NONE) return;

    XY act(act_size, 0);
    XY wh = getWidthHeight(bidir, which);
    double w = getTriWidth(bidir, which);
    const bool forward_orig = forward;
    if (bidir && which == MSC_ARROW_START)
        forward = !forward;
    if (forward) {
        wh.x *= -1;
        w *= -1;
        act.x *= -1;
    }
    std::swap(wh.x, w);
    //wh now contains the triangle size, w the size of the entire stuff (same for all,but DOUBLE & TRIPLE)

    //do filled ones
    if (arrow_type == MSC_ARROW_TRIPLE || arrow_type == MSC_ARROW_DOUBLE ||
        arrow_type == MSC_ARROW_SOLID  || arrow_type == MSC_ARROW_SHARP ||
        arrow_type == MSC_ARROW_DIAMOND|| arrow_type == MSC_ARROW_DOT) {
        const MscFillAttr fill(line.color.second, GRADIENT_NONE);
        canvas->Fill(Cover(xy, act_size, forward_orig, bidir, which, mainline_left, mainline_right), fill);
        return;
    }

    const double lw2 = line.LineWidth()/2;
    //Do closed line ones
    if (arrow_type == MSC_ARROW_TRIPLE_EMPTY || arrow_type == MSC_ARROW_DOUBLE_EMPTY ||
        arrow_type == MSC_ARROW_EMPTY        || arrow_type == MSC_ARROW_SHARP_EMPTY ||
        arrow_type == MSC_ARROW_DIAMOND_EMPTY|| arrow_type == MSC_ARROW_DOT_EMPTY) {
        Contour cover = Cover(xy, act_size, forward_orig, bidir, which, mainline_left, mainline_right);
        if (!canvas->HasImprecisePositioning())
            cover.Expand(-lw2);
        canvas->Line(cover, line);
        return;
    }

    Contour tri1, tri2, tri_sharp1, tri_sharp2;
    if (forward) {
        tri1 = Contour(xy+act + wh - XY(0, wh.y) - XY(0, wh.y), xy+act, xy+act+wh);
        tri2 = Contour(xy-act - wh + XY(0, wh.y) + XY(0, wh.y), xy-act, xy-act-wh);
        tri_sharp1 = Contour(xy+act + wh - XY(0, wh.y) - XY(0, wh.y), xy+act+XY(wh.x*SHARP_MUL_2, 0), xy+act+wh);
        tri_sharp2 = Contour(xy-act - wh + XY(0, wh.y) + XY(0, wh.y), xy-act-XY(wh.x*SHARP_MUL_2, 0), xy-act-wh);
    } else {
        tri1 = Contour(xy+act+wh, xy+act, xy+act + wh - XY(0, wh.y) - XY(0, wh.y));
        tri2 = Contour(xy-act-wh, xy-act, xy-act - wh + XY(0, wh.y) + XY(0, wh.y));
        tri_sharp1 = Contour(xy+act+wh, xy+act+XY(wh.x*SHARP_MUL_2, 0), xy+act + wh - XY(0, wh.y) - XY(0, wh.y));
        tri_sharp2 = Contour(xy-act-wh, xy-act-XY(wh.x*SHARP_MUL_2, 0), xy-act - wh + XY(0, wh.y) + XY(0, wh.y));
    }
    //Shrink by linewidth so that the above contours are outer edge
    if (!canvas->HasImprecisePositioning()) {
        tri1.Expand(-lw2);
        tri2.Expand(-lw2);
        tri_sharp1.Expand(-lw2);
        tri_sharp2.Expand(-lw2);
    }

    if (MSC_ARROW_IS_LINE(arrow_type) || MSC_ARROW_IS_HALF(arrow_type)) {
        tri1[0][2].visible = false;
        tri2[0][2].visible = false;
    }

    //for double or triple arrows, second and third line should be shorter, so that 
    //they do not extend to the middle of a double line
    if (arrow_type == MSC_ARROW_DOUBLE_HALF || arrow_type == MSC_ARROW_TRIPLE_HALF) {
        Block ext1(XY(0,0), canvas->GetSize());
        Block ext2 = ext1;
        ext1.y.from = xy.y + mainline_left.Spacing();
        ext2.y.from = xy.y + mainline_right.Spacing();
        ext1.x.till = ext2.x.from = xy.x;
        canvas->Clip(Contour(ext1)+Contour(ext2));
    }
    switch(arrow_type)
    {
    case MSC_ARROW_TRIPLE_LINE:
    case MSC_ARROW_TRIPLE_HALF:
        canvas->Line(tri1.CreateShifted(XY(wh.x, 0)), line);
        if (bidir && (which==MSC_ARROW_MIDDLE)) canvas->Line(tri2.CreateShifted(XY(-wh.x, 0)), line);
        /*Fallthrough*/
    case MSC_ARROW_DOUBLE_LINE:
    case MSC_ARROW_DOUBLE_HALF:
        canvas->Line(tri1.CreateShifted(XY(wh.x/2, 0)), line);
        if (bidir && (which==MSC_ARROW_MIDDLE)) canvas->Line(tri2.CreateShifted(XY(-wh.x/2, 0)), line);
        /*Fallthrough*/
        if (arrow_type == MSC_ARROW_DOUBLE_HALF || arrow_type == MSC_ARROW_TRIPLE_HALF)
            canvas->UnClip();
    case MSC_ARROW_LINE: /* Two lines */
    case MSC_ARROW_HALF: /* Unfilled half */
        if (MSC_ARROW_IS_HALF(arrow_type)) {
            Block ext(XY(0,0), canvas->GetSize());
            ext.y.from = xy.y;
            canvas->Clip(ext);
        }
        canvas->Line(tri1, line);
        if (bidir && (which==MSC_ARROW_MIDDLE)) canvas->Line(tri2, line);
        break;
    default:
        _ASSERT(0);
    }
    if (MSC_ARROW_IS_HALF(arrow_type)) 
        canvas->UnClip();
}

//For largely internal use - characteristic size of the big arrowheads
XY ArrowHead::getBigWidthHeight(MscArrowType type, const MscLineAttr &ltype) const
{
    static const double sizes[] = {3, 5, 10, 15, 25, 35};
    const XY lw(ltype.LineWidth(), ltype.LineWidth());
    XY ret(0,0);
    switch(type) {
    case MSC_ARROW_NONE:
    default:
        break;
    case MSC_ARROW_SOLID: /* Normal triangle */
    case MSC_ARROW_LINE: /* Normal Triangle */
        ret = XY(2*sizes[size.second], sizes[size.second])+lw*2;
        break;
    case MSC_ARROW_SHARP: /* Sharp triangle */
        ret = XY(SHARP_MUL_1*2*sizes[size.second], sizes[size.second]) + lw*2;
        break;
    case MSC_ARROW_SHARP_EMPTY: /* Small triangle, no "serifs" */
    case MSC_ARROW_EMPTY: /* Small triangle, no "serifs" */
    case MSC_ARROW_EMPTY_INV: /* Inverse small triangle */
    case MSC_ARROW_HALF: /* Half triangle */
        ret.x = 2*sizes[size.second];
        break;
    case MSC_ARROW_DIAMOND_EMPTY:
    case MSC_ARROW_DOT_EMPTY:
        ret.x = 2*sizes[size.second]+lw.x*2;
        break;
    case MSC_ARROW_DOT:
    case MSC_ARROW_DIAMOND:
        ret = XY(2*sizes[size.second], sizes[size.second])+lw*2;
        break;
    case MSC_ARROW_STRIPES:
        ret = XY(4*sizes[size.second]/6+5*lw.x+5*ltype.width.second+2, 0);
        break;
    case MSC_ARROW_TRIANGLE_STRIPES:
        ret = XY(6*sizes[size.second]/6+5*lw.x+5*ltype.width.second+2, 0);
        break;
    }
    if (xmul.first) ret.x *= xmul.second;
    if (ymul.first) ret.y *= ymul.second;
    return ret;
}

//The values returned here are used to determine spacing between entities
//the value returned counts from the midline of the entity line
double ArrowHead::getBigWidthsForSpace(bool bidir, MscArrowType type, MscArrowEnd which, 
                                       double body_height, double act_size, const MscLineAttr &ltype) const
{
    switch(type) {
    case MSC_ARROW_STRIPES:
    case MSC_ARROW_TRIANGLE_STRIPES:
        return act_size+getBigWidthHeight(type, ltype).x + ltype.LineWidth();
    default:
        return act_size+getBigWidthHeight(type, ltype).x;
    }
}

//Determines the margin of the text (from the entity line). Can be negative
//"margin_side_is_left" tells us if we need the left or right margin for the text
//"type" tells us what type of arrowhead do we have on the side of the text pointed by "left"
//   (this type will be either a segmenting (non-symmetric) arrowhead or the start/end head)
//sy and dy tells us where is the outer line of the body
//(should be equal to the y aspect of the bounding box of text_cover + plus two linewidths
double ArrowHead::getBigMargin(Contour text_cover, double sy, double dy, bool margin_side_is_left, 
                               bool bidir, MscArrowType type, const MscLineAttr &ltype) const
{
    const double lw = ltype.LineWidth();
    if (type == MSC_ARROW_NONE) return lw;
    double asize;
    Contour arrow_head = BigContourOneEntity(0, 0, sy, dy, bidir, type, MSC_ARROW_END, 
                                             ltype, !margin_side_is_left, &asize);
    Block b = arrow_head.GetBoundingBox().CreateExpand(1);
    const double max = std::max(asize, std::max(fabs(b.x.from), fabs(b.x.till)))+1+100*lw;
    if (margin_side_is_left) {
        arrow_head += Block(asize, max, sy, dy); //add arrow block
        b.x.till = max-lw-1;
    } else {
        arrow_head += Block(-max, -asize, sy, dy);
        b.x.from = -max+lw+1;
    }
    arrow_head.Expand(-lw);
    arrow_head = Contour(b) - arrow_head;
    const Range left_right = text_cover.GetBoundingBox().x;
    text_cover.Rotate(90);
    arrow_head.Rotate(90);
    double off, tp;
    if (!margin_side_is_left) {
        off = text_cover.OffsetBelow(arrow_head, tp, CONTOUR_INFINITY);
        //now off is negative and smaller than tp, which is probably negative, too
        return -left_right.till-off;
    } else {
        off = arrow_head.OffsetBelow(text_cover, tp, CONTOUR_INFINITY);
        //now tp is negative and shows the margin
        return left_right.from - off;
    }
}

//if lines == NULL, we assume there are no middle arrowheads (single segment) and "this->line" is the
//line type
//else we use the linewidths in "lines" to estimate top height, except if mid arrowhead is not segmenting
//we assume "lines" is ordered in increasing x coordinate. "forward" tells us if the arrow is -> or <-
double ArrowHead::bigYExtent(bool bidir, bool forward, const std::vector<MscLineAttr> *lines) const
{
    if (lines==NULL || lines->size()==0 || MSC_ARROW_IS_SYMMETRIC(midType.second)) {
        const double h = std::max(getBigWidthHeight(startType.second, line).y, getBigWidthHeight(endType.second, line).y);
        if (lines==NULL || lines->size()==0) return h;
        return std::max(h, getBigWidthHeight(midType.second, line).y);
    }
    double ret = 0;
    for (unsigned i=0; i<lines->size(); i++) {
        MscArrowEnd t_left = MSC_ARROW_MIDDLE, t_right = MSC_ARROW_MIDDLE;
        if (i==0) 
            t_left = forward ? MSC_ARROW_START : MSC_ARROW_END;
        if (i==lines->size()) 
            t_right = forward ? MSC_ARROW_END : MSC_ARROW_START;
        
        const MscArrowType tt_left = GetType(forward, bidir, t_left, false);
        const MscArrowType tt_right = GetType(forward, bidir, t_right, true);

        const double h_left = getBigWidthHeight(tt_left, lines->at(i)).y;
        const double h_right = getBigWidthHeight(tt_right, lines->at(i)).y;

        ret = std::max(ret, std::max(h_left, h_right));
    }
    return ret;
}


//Draw one arrowhead. either on the left or the right side of the entity line
Contour ArrowHead::BigContourOneEntity(double x, double act_size, double sy, double dy, bool bidir,
                                       MscArrowType type, MscArrowEnd which, const MscLineAttr &ltype, 
                                       bool left, double *body_margin) const
{
    const double lw2 = ltype.LineWidth()/2;
    const XY wh = getBigWidthHeight(type, ltype);
          double x_off = left ? - wh.x : wh.x;
    const double x_act = left ? -act_size : act_size;
    const double mid_y = (sy+dy)/2;
    Contour area;
    double ret_body_margin = 0;
    switch(type) {
    default:
    case MSC_ARROW_NONE: 
        ret_body_margin = act_size;
        break;
    
        //all these below draw a triangle
    case MSC_ARROW_SOLID: 
    case MSC_ARROW_LINE: 
    case MSC_ARROW_EMPTY: /* wh.y is zero here */
    case MSC_ARROW_SHARP_EMPTY: /* wh.y is zero here */
        area = Contour(x+x_act, mid_y, x+x_act+x_off, sy-wh.y, x+x_act+x_off, dy+wh.y);
        ret_body_margin = act_size + wh.x;
        break;

    case MSC_ARROW_SHARP:
        area = Contour(x+x_act                   , mid_y, x+x_act+x_off, sy-wh.y, x+x_act+x_off, dy+wh.y);
        area -= Contour(x+x_act+x_off*SHARP_MUL_2, mid_y, x+x_act+x_off, sy-wh.y, x+x_act+x_off, dy+wh.y);
        ret_body_margin = act_size + wh.x*SHARP_MUL_2;
        break;

    case MSC_ARROW_HALF: 
        area = Contour(x+x_act, sy, x+x_act+x_off, dy, x+x_act+x_off, sy);
        ret_body_margin = act_size + wh.x;
        break;

    case MSC_ARROW_EMPTY_INV:
        area  = Contour(x+x_act+x_off, mid_y, x+x_act+x_off, sy, x+x_act, sy);
        area += Contour(x+x_act+x_off, mid_y, x+x_act+x_off, dy, x+x_act, dy);
        if (which == MSC_ARROW_MIDDLE && !bidir) 
            area += Contour(x-x_act, mid_y, x-x_act-x_off, sy, x-x_act-x_off, dy);
        ret_body_margin = act_size + wh.x;
        break;

    case MSC_ARROW_DIAMOND_EMPTY: /* wh.y is zero here */
        if (which==MSC_ARROW_MIDDLE) break;
        area = Block(x, x+x_act, sy, dy);
        /* Fallthrough */
    case MSC_ARROW_DIAMOND:
        area += Contour(x-wh.x, mid_y, x, sy-wh.y, x, dy+wh.y) + Contour(x+wh.x, mid_y, x, sy-wh.y, x, dy+wh.y);
        break;

    case MSC_ARROW_DOT_EMPTY: /* wh.y is zero here */
        if (which==MSC_ARROW_MIDDLE) break;
        area = Block(x, x+x_act, sy, dy);
        /* Fallthrough */
    case MSC_ARROW_DOT:
        area += Contour(XY(x, mid_y), wh.x, (dy-sy)/2 + wh.y);    
        break;

    case MSC_ARROW_STRIPES:
        {
            //first box is LW+(a+line.width)+LW big
            //second box is LW+(3*a+2*line.width)+LW big
            //with a line.width+1 space in between and after the second box
            //wh.x equals to the sum of the above, which is 4*a+4*LW+5*l.width.
            //"a" depends on arrowhead size
            const double l = left ? -ltype.width.second : ltype.width.second;
            const double sp = left ? l-1 : l+1;
            const double lw = left ? -lw2*2 : lw2*2;
            const double a = (x_off - 4*lw - 3*l - 2*sp)/4;
            area = Contour(x+x_act, x+x_act+2*lw+a+l, sy, dy);
            area += Contour(x+x_act+2*lw+a+l+sp, x+x_act+4*lw+4*a+3*l+sp, sy, dy);
            ret_body_margin = act_size + wh.x;
        }
        break;
    case MSC_ARROW_TRIANGLE_STRIPES:
        {
            //same spacing as above, but there is an indentation of size "a"
            //also total width is "2*a" wider
            const double l = left ? -ltype.width.second : ltype.width.second;
            const double sp = left ? l-1 : l+1;
            const double lw = left ? -lw2*2 : lw2*2;
            const double a = (x_off - 4*lw - 3*l - 2*sp)/6;
            const double sh = 2*a;
            double xx = x+x_act;
            Contour h1 = Contour(xx, xx+2*lw+a+l, sy, dy);
            h1 += Contour(XY(xx+2*lw+a+l,sy), XY(xx+2*lw+a+l,dy), XY(xx+2*lw+a+l+sh, mid_y));
            h1 -= Contour(XY(xx,sy), XY(xx,dy), XY(xx+sh, mid_y));
            xx += 2*lw+a+l+sp;
            Contour h2 = Contour(xx, xx+2*lw+3*a+2*l, sy, dy);
            h2 += Contour(XY(xx+2*lw+3*a+2*l,sy), XY(xx+2*lw+3*a+2*l,dy), XY(xx+2*lw+3*a+2*l+sh, mid_y));
            h2 -= Contour(XY(xx,sy), XY(xx,dy), XY(xx+sh, mid_y));
            area = h1;
            area += h2;
            xx += 2*lw+3*a+2*l+sp;
            //x+x_act+x_off is supposedly == xx+sh
            area += Contour(XY(xx, sy), XY(x+x_act+x_off, sy), XY(x+x_act+x_off, mid_y));
            area += Contour(XY(xx, dy), XY(x+x_act+x_off, dy), XY(x+x_act+x_off, mid_y));
            ret_body_margin = act_size + wh.x;
        }
    }
    if (body_margin) *body_margin = ret_body_margin;
    return area;
}

//returns the outer line of the block arrow, each segment in a separate
//Contour. If mid-arrow does not segment only one Contour is returned
Contour ArrowHead::BigContour(const std::vector<double> &xPos, const std::vector<double> &act_size, 
                              double sy, double dy, bool forward, bool bidir, 
                              const std::vector<MscLineAttr> *lines,
                              std::vector<Contour> &result) const
{
    Contour area_overall, area_segment;
    const bool segment = !MSC_ARROW_IS_SYMMETRIC(GetType(bidir,MSC_ARROW_MIDDLE)) && xPos.size()>2;
    const MscArrowEnd e_left  = forward ? MSC_ARROW_START : MSC_ARROW_END;
    const MscArrowType t_left_end  = GetType(bidir, e_left);
    const MscLineAttr *ltype_current = (segment && lines) ? &*lines->begin() : &line;

    //draw leftmost arrowhead
    double from_x;
    area_segment = BigContourOneEntity(xPos[0], act_size[0], sy, dy, bidir, 
                                       t_left_end, e_left, *ltype_current, false, &from_x);
    from_x = xPos[0] + from_x;

    //set up variables for mid-points
    if (midType.second != MSC_ARROW_NONE)
        for (unsigned i=1; i<xPos.size()-1; i++) {
            MscArrowType type = GetType(forward, bidir, MSC_ARROW_MIDDLE, true);
            //draw left side of the entity line
            double right_margin; 
            area_segment += BigContourOneEntity(xPos[i], act_size[i], sy, dy, bidir, 
                                                type, MSC_ARROW_MIDDLE, *ltype_current, true, &right_margin);
            //draw body that connects them
            area_segment += Block(from_x, xPos[i] - right_margin, sy, dy);

            //calculate next linewidth
            if (segment && lines && lines->size()>i)
                ltype_current  = &lines->at(i);

            area_overall += area_segment;
            //if we are segmented, add us to result
            if (segment) 
                result.push_back(std::move(area_segment));
            area_segment.clear();

            //if segmented, draw arrowhead at right side of entity line
            if (segment) {
                type = GetType(forward, bidir, MSC_ARROW_MIDDLE, false); //overwrite
                area_segment = BigContourOneEntity(xPos[i], act_size[i], sy, dy, bidir, 
                                                   type, MSC_ARROW_MIDDLE, *ltype_current, false, &from_x);
                //calculate from_x
                from_x = xPos[i] + from_x;
            }
    }
    //draw rightmost arrowhead
    const MscArrowEnd e_right = forward ? MSC_ARROW_END : MSC_ARROW_START;
    const MscArrowType t_right_end = GetType(bidir, e_right);
    double right_margin;
    area_segment += BigContourOneEntity(*xPos.rbegin(), *act_size.rbegin(), sy, dy, bidir, 
                                        t_right_end, e_right, *ltype_current, true, &right_margin);
    area_segment += Block(from_x, *xPos.rbegin() - right_margin, sy, dy);
    area_overall += area_segment;
    if (segment) 
        result.push_back(std::move(area_segment));
    else
        result.push_back(area_overall);
    return area_overall;
}

void ArrowHead::BigDrawFromContour(std::vector<Contour> &result, const std::vector<MscLineAttr> *lines,
                 const MscFillAttr &fill, const MscShadowAttr &shadow, MscCanvas &canvas,
                 bool shadow_x_neg, bool shadow_y_neg) const
{
    
    for (unsigned i=0; i<result.size(); i++) {
        const MscLineAttr &l = lines ? lines->at(i) : line;
        const double gap = l.LineWidth()/2-lines->at(i).width.second/2;
        const Contour midline = result[i].CreateExpand(-l.LineWidth()/2);
        canvas.Shadow(midline, shadow, shadow_x_neg, shadow_y_neg);
        canvas.Fill  (midline.CreateExpand(-l.Spacing()), fill); 
        canvas.Line  (midline, l);
    }
}
void ArrowHead::BigDrawEmptyMid(const std::vector<double> &xPos, double sy, double dy, 
                                MscCanvas &canvas, const Contour *clip) const
{
    //if we have empty_dot or empty_diamond middle arrow types, draw them in addition
    //..but clip the drawing area, so that text is not disturbed
    if (xPos.size()<=2 || (midType.second!=MSC_ARROW_DOT_EMPTY && midType.second==MSC_ARROW_DIAMOND_EMPTY)) 
        return;
    if (clip) canvas.ClipInverse(*clip);
    const double x_off = getBigWidthHeight(midType.second, line).x;
    const XY center(0, (dy+sy)/2);
    const double lw2 = line.LineWidth()/2;
    const XY wh(x_off - lw2, (dy-sy)/2 - lw2);
    const Contour contour = midType.second==MSC_ARROW_DOT_EMPTY ? Contour(center, wh.x, wh.y) : diamond(center, wh);
    for (unsigned i=1; i<xPos.size()-1; i++)
        canvas.Line(contour.CreateShifted(XY(xPos[i], 0)), line);
    if (clip) canvas.UnClip();
}

void ArrowHead::BigCalculateAndDraw(const std::vector<double> &xPos, const std::vector<double> &act_size, 
                                    double sy, double dy, bool forward, bool bidir,
                                    const MscFillAttr &fill, const MscShadowAttr &shadow, MscCanvas &canvas,
                                    const Contour *clip, bool shadow_x_neg, bool shadow_y_neg) const
{
    std::vector<Contour> cont;
    BigContour(xPos, act_size, sy, dy, forward, bidir, NULL, cont);
    BigDrawFromContour(cont, NULL, fill, shadow, canvas, shadow_x_neg, shadow_y_neg);
    BigDrawEmptyMid(xPos, sy, dy, canvas, clip);
}
