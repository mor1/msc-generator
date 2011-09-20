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
    ah.BigDraw(xPos, active, HINT_GRAPHIC_SIZE_Y*0.3, HINT_GRAPHIC_SIZE_Y*0.7, false, shadow, fill, NULL, canvas);
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
XY ArrowHead::getBigWidthHeight(bool bidir, MscArrowEnd which) const
{
    static const double sizes[] = {3, 5, 10, 15, 25, 35};
    XY ret(0,0);
    switch(GetType(bidir, which)) {
    case MSC_ARROW_NONE:
    default:
        break;
    case MSC_ARROW_SOLID: /* Normal triangle */
    case MSC_ARROW_LINE: /* Normal Triangle */
        ret = XY(2*sizes[size.second], sizes[size.second]);
        break;
    case MSC_ARROW_SHARP: /* Sharp triangle */
        ret = XY(SHARP_MUL_1*2*sizes[size.second], sizes[size.second]);
        break;
    case MSC_ARROW_SHARP_EMPTY: /* Small triangle, no "serifs" */
    case MSC_ARROW_EMPTY: /* Small triangle, no "serifs" */
    case MSC_ARROW_EMPTY_INV: /* Inverse small triangle */
    case MSC_ARROW_HALF: /* Half triangle */
        ret.x = 2*sizes[size.second];
        break;
    case MSC_ARROW_DIAMOND_EMPTY:
    case MSC_ARROW_DOT_EMPTY:
        ret.x = 2*sizes[size.second];
        break;
    case MSC_ARROW_DOT:
    case MSC_ARROW_DIAMOND:
        ret = XY(2*sizes[size.second], sizes[size.second]);
        break;
    case MSC_ARROW_STRIPES:
        ret = XY(sizes[size.second]*2, 0);
        break;
    case MSC_ARROW_TRIANGLE_STRIPES:
        ret = XY(sizes[size.second]*2*1.2, 0);
        break;
    }
    if (xmul.first) ret.x *= xmul.second;
    if (ymul.first) ret.y *= ymul.second;
    return ret;
}

/* return value:
 * - first is the extent on the left side of the entity line
 * - second is the extent on the right side of the entity line
 * (extent means, how long the body of the arrow shall be drawn)
 * formard is true if the start entiry of the arrow is left from its destination (like -> and not like <-)
 * if which is start, we swap the two values (as we should, to get the above)
 * body_height contains the height of the text within
 */
DoublePair ArrowHead::getBigWidthsForBody(bool forward, bool bidir, MscArrowEnd which, double body_height) const
{
    XY wh = getBigWidthHeight(bidir, which);
    const double lw2 = line.LineWidth()/2;
    DoublePair ret(0,0);
    const MscArrowType t = GetType(bidir, which);
    switch(t) {
    default:
    case MSC_ARROW_NONE:
    case MSC_ARROW_DIAMOND:
    case MSC_ARROW_DOT:
    case MSC_ARROW_DIAMOND_EMPTY:
    case MSC_ARROW_DOT_EMPTY:
        break;
    case MSC_ARROW_SOLID: 
    case MSC_ARROW_EMPTY: 
    case MSC_ARROW_SHARP_EMPTY: /* Small triangle, no "serifs" */
    case MSC_ARROW_LINE: 
    case MSC_ARROW_HALF: 
    case MSC_ARROW_EMPTY_INV: /* Inverse small triangle */
    case MSC_ARROW_TRIANGLE_STRIPES:
        //wh.x -= lw2;
    case MSC_ARROW_STRIPES:
        ret.first += wh.x;
        if ((bidir || t== MSC_ARROW_EMPTY_INV) && which == MSC_ARROW_MIDDLE)
            ret.second = ret.first;
        break;
    case MSC_ARROW_SHARP: /* Sharp triangle */
        //wh.x -= lw2;
        ret.first = wh.x - wh.x*(wh.y/(wh.y+body_height/2));
        if (bidir && which == MSC_ARROW_MIDDLE)
            ret.second=ret.first;
        break;
    }
    if (!forward ^ (which==MSC_ARROW_START)) //if reverse or start, but not if both
        ret.swap();
    return ret;
}

//The values returned here are used to determine spacing between entities
DoublePair ArrowHead::getBigWidthsForSpace(bool forward, bool bidir, MscArrowEnd which, double body_height) const
{
    const MscArrowType t = GetType(bidir, which);
    DoublePair ret(getBigWidthHeight(bidir, which).x,0);
    switch (t) {
    default:
        return getBigWidthsForBody(forward, bidir, which, body_height);
    case MSC_ARROW_SHARP:
        if (bidir && which == MSC_ARROW_MIDDLE)
            ret.second=ret.first;
        break;
    case MSC_ARROW_DOT_EMPTY:
    case MSC_ARROW_DIAMOND_EMPTY:
        ret.swap();
        break;
    }
    return ret;
}

bool ArrowHead::bigDoesSegment(bool bidir, MscArrowEnd which) const
{
    switch(GetType(bidir, which)) {
    default:
        case MSC_ARROW_SOLID: 
        case MSC_ARROW_EMPTY: 
        case MSC_ARROW_EMPTY_INV: 
        case MSC_ARROW_LINE: 
        case MSC_ARROW_HALF: 
        case MSC_ARROW_STRIPES:
        case MSC_ARROW_TRIANGLE_STRIPES:
            return true;
        case MSC_ARROW_NONE:
        case MSC_ARROW_DIAMOND:
        case MSC_ARROW_DIAMOND_EMPTY:
        case MSC_ARROW_DOT:
        case MSC_ARROW_DOT_EMPTY:
            return false;
    }
}

//Determines the margin of the text (from the entity line). Can be negative
//left tells us if we need the left or right margin for the text
//forward tells us if the arrow is like -> or like <-
//bidir and which further specifies which arrowhead to use
//sy and dy tells us where is the text drawn (should be equal to the y aspect of the bounding box of text_cover)
double ArrowHead::getBigMargin(Contour text_cover, double sy, double dy, bool left, bool forward, bool bidir, MscArrowEnd which) const
{
    DoublePair tmp = getBigWidthsForBody(forward, bidir, which, dy-sy);
    const double asize = left ? tmp.second : -tmp.first;
    if (asize == 0) return 0; //no margin for no arrowhead
    Contour arrow_head = Contour(asize, left ? -MSC_BIG_COORD : MSC_BIG_COORD, -MSC_BIG_COORD, +MSC_BIG_COORD) -
        BigCoverOne(0, 0, sy, dy, forward, bidir, which);
    const Range left_right = text_cover.GetBoundingBox().x;
    text_cover.Rotate(90);
    arrow_head.Rotate(90);
    double off, tp;
    if (!left) {
        off = text_cover.OffsetBelow(arrow_head, tp, CONTOUR_INFINITY);
        //now off is negative and smaller than tp, which is probably negative, too
        return -left_right.till-off;
    } else {
        off = arrow_head.OffsetBelow(text_cover, tp, CONTOUR_INFINITY);
        //now tp is negative and shows the margin
        return left_right.from - off;
    }
}

double ArrowHead::bigYExtent(bool bidir, bool multisegment) const
{
    double ret = getBigWidthHeight(bidir, MSC_ARROW_START).y;
    ret = std::max(ret, getBigWidthHeight(bidir, MSC_ARROW_END).y);
    if (multisegment)
        ret = std::max(ret, getBigWidthHeight(bidir, MSC_ARROW_MIDDLE).y);
    return ret;
}

//Draw one arrowhead. If bidir and solid, we draw both
Contour ArrowHead::BigCoverOne(double x, double act_size, double sy, double dy, bool forward, bool bidir, MscArrowEnd which) const
{
    if (which == MSC_ARROW_START) forward = !forward;
    const double lw2 = line.LineWidth()/2;
    const XY wh = getBigWidthHeight(bidir, which);
          double x_off = forward ? - wh.x : wh.x;
    const double x_act = forward ? -act_size : act_size;
    const double mid_y = (sy+dy)/2;
    Contour area;
    switch(GetType(bidir, which)) {
    case MSC_ARROW_NONE: 
        if (which == MSC_ARROW_MIDDLE) 
            area = Block(x-act_size, x+act_size, sy, dy);
        else
            area = Block(x, x+x_act, sy, dy);
    default:
        break;
    
        //all these below draw a triangle
    case MSC_ARROW_SOLID: 
    case MSC_ARROW_LINE: 
    case MSC_ARROW_EMPTY: /* wh.y is zero here */
    case MSC_ARROW_SHARP_EMPTY: /* wh.y is zero here */
        sy -= lw2; dy += lw2; x_off += forward ? -lw2 : lw2;
        area += Contour(x+x_act, mid_y, x+x_act+x_off, sy-wh.y, x+x_act+x_off, dy+wh.y);
        if (bidir && which == MSC_ARROW_MIDDLE)
            area += Contour(x-x_act, mid_y, x-x_act-x_off, sy-wh.y, x-x_act-x_off, dy+wh.y);
        area.Expand(-lw2);
        break;

    case MSC_ARROW_SHARP:
        sy -= lw2; dy += lw2; x_off += forward ? -lw2 : lw2;
        area += Contour(x+x_act                  , mid_y, x+x_act+x_off, sy-wh.y, x+x_act+x_off, dy+wh.y);
        area -= Contour(x+x_act+x_off*SHARP_MUL_2, mid_y, x+x_act+x_off, sy-wh.y, x+x_act+x_off, dy+wh.y);
        if (bidir && which == MSC_ARROW_MIDDLE) {
            area += Contour(x-x_act                  , mid_y, x-x_act-x_off, sy-wh.y, x-x_act-x_off, dy+wh.y);
            area -= Contour(x-x_act-x_off*SHARP_MUL_2, mid_y, x-x_act-x_off, sy-wh.y, x-x_act-x_off, dy+wh.y);
        }
        area.Expand(-lw2);
        break;

    case MSC_ARROW_HALF: 
        sy -= lw2; dy += lw2; x_off += forward ? -lw2 : lw2;
        area += Contour(x+x_act, sy, x+x_act+x_off, dy, x+x_act+x_off, sy);
        if (bidir && which == MSC_ARROW_MIDDLE)
            area += Contour(x-x_act, sy, x-x_act-x_off, sy, x-x_act-x_off, dy);
        area.Expand(-lw2);
        break;

    case MSC_ARROW_EMPTY_INV:
        sy += lw2; dy -= lw2; x_off += forward ? -lw2 : lw2;
        area += Contour(x+x_act+x_off, mid_y, x+x_act+x_off, sy, x+x_act, sy);
        area += Contour(x+x_act+x_off, mid_y, x+x_act+x_off, dy, x+x_act, dy);
        if (which == MSC_ARROW_MIDDLE) {
            if (bidir) {
                area += Contour(x-x_act-x_off, mid_y, x-x_act-x_off, sy, x-x_act, sy);
                area += Contour(x-x_act-x_off, mid_y, x-x_act-x_off, dy, x-x_act, dy);
            } else {
                area += Contour(x-x_act, mid_y, x-x_act-x_off, sy, x-x_act-x_off, dy);
            }
        }
        area.Expand(+lw2);
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
        area += Contour(x+x_act+x_off/5.*0, x+x_act+x_off/5.*1, sy, dy);
        area += Contour(x+x_act+x_off/5.*2, x+x_act+x_off/5.*4, sy, dy);
        if (bidir && which == MSC_ARROW_MIDDLE) {
            area += Contour(x-x_act-x_off/5.*0, x-x_act-x_off/5.*1, sy, dy);
            area += Contour(x-x_act-x_off/5.*2, x-x_act-x_off/5.*4, sy, dy);
        }
        break;
    case MSC_ARROW_TRIANGLE_STRIPES:
        sy -= lw2; dy += lw2; x_off += forward ? -lw2 : lw2;
        Area a1;
        a1 += Contour(x+x_off/6.*0, x+x_off/6.*1, sy, dy);
        a1 += Contour(x+x_off/6.*2, x+x_off/6.*4, sy, dy);
        a1 += Contour(x+x_off/6.*5, x+x_off/6.*6, sy, dy);
        a1 -= Contour(XY(x+x_off/6.*1, mid_y), XY(x+x_off/6.*0, sy), XY(x+x_off/6.*0, dy));
        a1 += Contour(XY(x+x_off/6.*2, mid_y), XY(x+x_off/6.*1, sy), XY(x+x_off/6.*1, dy));
        a1 -= Contour(XY(x+x_off/6.*3, mid_y), XY(x+x_off/6.*2, sy), XY(x+x_off/6.*2, dy));
        a1 += Contour(XY(x+x_off/6.*5, mid_y), XY(x+x_off/6.*4, sy), XY(x+x_off/6.*4, dy));
        a1 -= Contour(XY(x+x_off/6.*6, mid_y), XY(x+x_off/6.*5, sy), XY(x+x_off/6.*5, dy));
        a1.Shift(XY(x_act, 0));
        area += a1;
        if (bidir && which == MSC_ARROW_MIDDLE) {
            Area a2;
            a2 += Contour(x-x_off/6.*0, x-x_off/6.*1, sy, dy);
            a2 += Contour(x-x_off/6.*2, x-x_off/6.*4, sy, dy);
            a2 += Contour(x-x_off/6.*5, x-x_off/6.*6, sy, dy);
            a2 -= Contour(XY(x-x_off/6.*1, mid_y), XY(x-x_off/6.*0, sy), XY(x-x_off/6.*0, dy));
            a2 += Contour(XY(x-x_off/6.*2, mid_y), XY(x-x_off/6.*1, sy), XY(x-x_off/6.*1, dy));
            a2 -= Contour(XY(x-x_off/6.*3, mid_y), XY(x-x_off/6.*2, sy), XY(x-x_off/6.*2, dy));
            a2 += Contour(XY(x-x_off/6.*5, mid_y), XY(x-x_off/6.*4, sy), XY(x-x_off/6.*4, dy));
            a2 -= Contour(XY(x-x_off/6.*6, mid_y), XY(x-x_off/6.*5, sy), XY(x-x_off/6.*5, dy));
            a2.Shift(XY(-x_act,0));
            area += a2;
        }
        area.Expand(-lw2);
    }
    return area;
}

//if no_segment==-1 return the cover for all segments, if not, return only one segment (if we have multiple)
Contour ArrowHead::BigCover(std::vector<double> xPos, std::vector<double> act_size, double sy, double dy, bool bidir, int /*no_segment*/) const
{
    Contour area;
    const bool forward = xPos[0] < xPos[1];
    const MscArrowEnd i_begin = forward ? MSC_ARROW_START : MSC_ARROW_END;
    const MscArrowEnd i_end   = forward ? MSC_ARROW_END : MSC_ARROW_START;
    std::sort(xPos.begin(), xPos.end()); //from left to right

    const bool segment = bigDoesSegment(bidir, MSC_ARROW_MIDDLE);
    //draw leftmost arrowhead
    area += BigCoverOne(xPos[0], act_size[0], sy, dy, forward, bidir, i_begin);
    double from_x = xPos[0] + getBigWidthsForBody(forward, bidir, i_begin, dy-sy).second + act_size[0];

    //set up variables for mid-points
    DoublePair mid_xx = getBigWidthsForBody(forward, bidir, MSC_ARROW_MIDDLE, dy-sy);
    for (unsigned i=1; i<xPos.size()-1; i++) {
        area += BigCoverOne(xPos[i], act_size[i], sy, dy, forward, bidir, MSC_ARROW_MIDDLE);
        if (segment) {
            double to_x = xPos[i] - mid_xx.first - act_size[i];
            area += Block(from_x, to_x, sy, dy);
            from_x = xPos[i] + mid_xx.second + act_size[i];
        }
    }
    //draw rightmost arrowhead
    area += BigCoverOne(xPos[xPos.size()-1], act_size[act_size.size()-1], sy, dy, forward, bidir, i_end);
    double to_x = xPos[xPos.size()-1] - getBigWidthsForBody(forward, bidir, i_end, dy-sy).first - act_size[act_size.size()-1];
    area += Block(from_x, to_x, sy, dy);

    return area;
}

Contour ArrowHead::BigEntityLineCover(const std::vector<double> &xPos, std::vector<double> act_size, 
                                      double sy, double dy, bool bidir,
                                      const std::vector<MscLineAttr> *lines, const Block &total) const
{
    const Contour area = BigCover(xPos, act_size, sy, dy, bidir);
    Contour ret;
    if (xPos.size()>2 && bigDoesSegment(bidir, MSC_ARROW_MIDDLE) && lines) {
        for (unsigned i=0; i<xPos.size()-1; i++) {
            if (!lines->at(i).IsDoubleOrTriple()) continue;
            Block this_segment(Range(xPos[i], xPos[i+1]), total.y);
            if (i==0) {
                if (xPos[0]<xPos[1]) this_segment.x.from = total.x.from;
                else this_segment.x.till = total.x.till;
            } else if (i==xPos.size()-1) {
                if (xPos[0]<xPos[1]) this_segment.x.till = total.x.till;
                else this_segment.x.from = total.x.from;
            }
            const Area local_area = area * this_segment;
            const double gap = lines->at(i).Spacing();
            ret += local_area.CreateExpand(gap);
        }
    } else if (!line.IsDoubleOrTriple()) {
        const double gap = line.Spacing();
        ret = area.CreateExpand(gap);
    }
    return ret;
}

void ArrowHead::BigDraw(const std::vector<double> &xPos, std::vector<double> act_size, 
                        double sy, double dy, bool bidir,
                        const MscShadowAttr &shadow, const MscFillAttr &fill,
                        const std::vector<MscLineAttr> *lines, MscCanvas *canvas,
                        const Contour * /*clip*/, bool shadow_x_neg, bool shadow_y_neg) const
{
    const Contour area = BigCover(xPos, act_size, sy, dy, bidir);
    if (xPos.size()>2 && bigDoesSegment(bidir, MSC_ARROW_MIDDLE) && lines) {
        for (unsigned i=0; i<xPos.size()-1; i++) {
            Block this_segment(xPos[i], xPos[i+1], 0, canvas->GetSize().y);
            if (i==0) {
                if (xPos[0]<xPos[1]) this_segment.x.from = 0;
                else this_segment.x.till = canvas->GetSize().x;
            } else if (i==xPos.size()-1) {
                if (xPos[0]<xPos[1]) this_segment.x.till = canvas->GetSize().x;
                else this_segment.x.from = 0;
            }
            const Area local_area = area * this_segment;
            const double gap = lines->at(i).LineWidth()/2-lines->at(i).width.second/2;
            canvas->Shadow(local_area.CreateExpand( gap), shadow, shadow_x_neg, shadow_y_neg);
            canvas->Fill  (local_area.CreateExpand(-gap), fill);
            canvas->Line  (local_area, lines->at(i));
        }
    } else {
        const double gap = line.LineWidth()/2-line.width.second/2;
        canvas->Shadow(area.CreateExpand( gap), shadow, shadow_x_neg, shadow_y_neg);
        canvas->Fill  (area.CreateExpand(-gap), fill);
        canvas->Line  (area, line);
        const MscArrowType t = GetType(bidir, MSC_ARROW_MIDDLE);
        //if we have empty_dot or empty_diamond middle arrow types, draw them in addition
        //..but clip the drawing area, so that text is not disturbed
        //if (clip) msc->ClipInverse(*clip);
        if (xPos.size()>2 && (t==MSC_ARROW_DOT_EMPTY ||t==MSC_ARROW_DIAMOND_EMPTY)) {
            const double x_off = getBigWidthHeight(bidir, MSC_ARROW_MIDDLE).x;
            const XY center(0, (dy+sy)/2);
            const XY wh(x_off, (dy-sy)/2);
            const Contour contour = t==MSC_ARROW_DOT_EMPTY ? Contour(center, wh.x, wh.y) : diamond(center, wh);
            for (unsigned i=1; i<xPos.size()-1; i++)
                canvas->Line(contour.CreateShifted(XY(xPos[i], 0)), line);
        }
        //if (clip) msc->UnClip();
    }
}

