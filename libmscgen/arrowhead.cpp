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
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include "msc.h"

template<> const char EnumEncapsulator<MscArrowType>::names[][ENUM_STRING_LEN] =
    {"invalid", "none", "solid", "diamond", "dot", "empty", "line", "half", 
     "empty_diamond", "empty_dot", ""};
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

void ArrowHead::Empty() {
    line.Empty();
    size.first = false;
    endType.first = false;
    midType.first = false;
    startType.first = false;
}

ArrowHead & ArrowHead::operator += (const ArrowHead &toadd)
{
    const ArrowHead &a = static_cast<const ArrowHead&>(toadd);
    line += a.line;
    if (a.size.first) size = a.size;
    if (a.endType.first) endType = a.endType;
    if (a.midType.first) midType = a.midType;
    if (a.startType.first) startType = a.startType;
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
            if (type == ARROW && MSC_ARROW_OK_FOR_ARROWS(at)) {
                msc->Error.Error(a, true, "Value '"+a.value+"' is applicable only to block arrows.");
                return true;
            }
            if (type == BIGARROW && MSC_ARROW_OK_FOR_BIG_ARROWS(at)) {
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
    if (a.EndsWith("color") || a.EndsWith("line.width")) {
        return line.AddAttribute(a, msc, t);
    }
    return false;
}

void ArrowHead::AttributeNames(Csh &csh)
{
    static const char names[][ENUM_STRING_LEN] =
    {"invalid", "arrow.type", "arrow.size", "arrow.color", "arrow.starttype", "arrow.midtype",
     "arrow.endtype", "line.width", ""};
    csh.AddToHints(names, csh.HintPrefix(COLOR_ATTRNAME), HINT_ATTR_NAME);
}

bool CshHintGraphicCallbackForBigArrows(MscDrawer *msc, CshHintGraphicParam p)
{
    if (!msc) return false;
    const double xx = 0.7;
    std::vector<double> xPos(2); 
    xPos[0] = 0;
    xPos[1] = HINT_GRAPHIC_SIZE_X*0.7;
    MscLineAttr eLine(LINE_SOLID, MscColorType(0,0,0), 1, CORNER_NONE, 0);
    msc->Clip(XY(HINT_GRAPHIC_SIZE_X*0.1,1), XY(HINT_GRAPHIC_SIZE_X-1, HINT_GRAPHIC_SIZE_Y-1));
    msc->Line(XY(xPos[1], 1), XY(xPos[1], HINT_GRAPHIC_SIZE_Y-1), eLine);
    ArrowHead ah(ArrowHead::BIGARROW);
    ah.line += MscColorType(0,192,32); //green-blue
    ah.endType.second = (MscArrowType)(int)p;
    ah.size.second = MSC_ARROWS_INVALID;
    MscFillAttr fill(ah.line.color.second.Lighter(0.7), GRADIENT_UP);
    ah.BigDraw(xPos, HINT_GRAPHIC_SIZE_Y*0.3, HINT_GRAPHIC_SIZE_Y*0.7, false, fill, msc);
    msc->UnClip();
    return true;
}

bool CshHintGraphicCallbackForArrows(MscDrawer *msc, MscArrowType type, MscArrowSize size, bool left)
{
    if (!msc) return false;
    const double xx = left ? 0.9 : 0.7;
    XY xy(HINT_GRAPHIC_SIZE_X*xx, HINT_GRAPHIC_SIZE_Y/2);
    MscLineAttr eLine(LINE_SOLID, MscColorType(0,0,0), 1, CORNER_NONE, 0);
    ArrowHead ah;
    ah.line += MscColorType(0,192,32); //green-blue
    ah.endType.second = type;
    ah.size.second = size;
    Range cover = ah.EntityLineCover(xy, true, false, MSC_ARROW_END);
    msc->Clip(XY(1,1), XY(HINT_GRAPHIC_SIZE_X-1, HINT_GRAPHIC_SIZE_Y-1));
    if (cover.from>1)
        msc->Line(XY(xy.x, 1), XY(xy.x, cover.from), eLine);
    if (cover.till<HINT_GRAPHIC_SIZE_Y-1)
        msc->Line(XY(xy.x, cover.till), XY(xy.x, HINT_GRAPHIC_SIZE_Y-1), eLine);
    double stop = ah.getWidths(true, false, MSC_ARROW_END, true, eLine).first;
    if (stop<HINT_GRAPHIC_SIZE_X*(xx-0.1))
        msc->Line(XY(HINT_GRAPHIC_SIZE_X*0.1, xy.y), XY(xy.x-stop, xy.y), ah.line);
    ah.Draw(xy, true, false, MSC_ARROW_END, msc);
    msc->UnClip();
    return true;
}

bool CshHintGraphicCallbackForArrowTypes(MscDrawer *msc, CshHintGraphicParam p)
{
    return CshHintGraphicCallbackForArrows(msc, (MscArrowType)(int)p, MSC_ARROW_SMALL, false);
}

bool CshHintGraphicCallbackForArrowSizes(MscDrawer *msc, CshHintGraphicParam p)
{
    return CshHintGraphicCallbackForArrows(msc, MSC_ARROW_SOLID, (MscArrowSize)(int)p, true);
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

    case MSC_ARROW_DIAMOND:
    case MSC_ARROW_DIAMOND_EMPTY:
        xy.x = xy.y = baseDiamondSize*sizePercentage/100;
        break;

    case MSC_ARROW_DOT:
    case MSC_ARROW_DOT_EMPTY:
        xy.x = xy.y = baseDotSize*sizePercentage/100;
        break;
    }
    return xy;
}

/* return value:
 * - first is the extent on the left side of the entity line
 * - second is the extent on the right side of the entity line
 * formard is true if the start entiry of the arrow is left from its destination (like -> and not like <-)
 * if which is start, we swap the two values (as we should, to get the above)
 * If forLine is true, we return how much of the arrow line the arrowhead covers
 * If false, we return how much the text margin should be from the entity line
 */
DoublePair ArrowHead::getWidths(bool forward, bool bidir, MscArrowEnd which, bool forLine, const MscLineAttr &mainline) const
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
        if (forLine && (t == MSC_ARROW_HALF || t == MSC_ARROW_LINE)) {
            if (mainline.IsDouble())
                ret.first = mainline.DoubleSpacing(); //these do not cover the line that much
            else if (mainline.IsTriple())
                ret.first = mainline.TripleSpacing();
            else
                ret.first = 0; 
        } else
            ret.first = getWidthHeight(bidir, which).x;

        //Now see if we put the value to the right of first/second
        if (bidir && (which==MSC_ARROW_MIDDLE))
            ret.second = ret.first;
        else if (!forward ^ which==MSC_ARROW_START) //if reverse or start, but not if both
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
    Contour poly(xy.x, xy.y-wh.x, xy.x, xy.y+wh.y, xy.x+wh.x, xy.y);
    if (!poly.AddAnEdge(Edge(XY(xy.x-wh.x, xy.y))))
        _ASSERT(0);
    return poly;
}

/** Return the Y range for which the entity line shall not be drawn
 */
Range ArrowHead::EntityLineCover(XY xy, bool forward, bool bidir, MscArrowEnd which) const
{
    XY wh = getWidthHeight(bidir, which);
    if (bidir && which == MSC_ARROW_START)
        forward = !forward;

    Range ret(0,0);

    switch(GetType(bidir, which)) {
    case MSC_ARROW_DIAMOND_EMPTY:
        ret.from = xy.y-wh.y;
        ret.till = xy.y+wh.y;
        break;

    case MSC_ARROW_DOT_EMPTY:
        ret.from = xy.y-wh.x;
        ret.till = xy.y+wh.x;
        break;
    }
    return ret;
}

Area ArrowHead::Cover(XY xy, bool forward, bool bidir, MscArrowEnd which, const MscLineAttr &mainline) const
{
    XY wh = getWidthHeight(bidir, which);
    if (bidir && which == MSC_ARROW_START)
        forward = !forward;
    if (forward) wh.x *= -1;

    MscFillAttr fill(line.color.second);
    Contour tri1(xy+wh, xy, xy + wh - XY(0, wh.y) - XY(0, wh.y));
    Contour tri2(xy-wh, xy, xy - wh + XY(0, wh.y) + XY(0, wh.y));

    Area area;

    switch(GetType(bidir, which)) {
    case MSC_ARROW_NONE:
        break;
    case MSC_ARROW_SOLID: /* Filled */
    case MSC_ARROW_EMPTY: /* Non-Filled */
    case MSC_ARROW_LINE: /* Two lines */
        area = tri1;
        if (bidir && (which==MSC_ARROW_MIDDLE)) area += tri2;
        break;
    case MSC_ARROW_HALF: /* Unfilled half */
        area = Contour(xy+wh, xy, xy + XY(wh.x, 0));
        if (bidir && (which==MSC_ARROW_MIDDLE))
            area += Contour(xy + XY(-wh.x, wh.y), xy, xy - XY(wh.x, 0));
        break;

    case MSC_ARROW_DIAMOND:
    case MSC_ARROW_DIAMOND_EMPTY:
        area = diamond(xy, wh);
        break;

    case MSC_ARROW_DOT:
    case MSC_ARROW_DOT_EMPTY:
        area = Contour(xy, wh.x, wh.y);
        break;
    }
    if (line.LineWidth()>1)
        area = area.CreateExpand(line.LineWidth()/2);
    if (GetType(bidir, which)==MSC_ARROW_HALF) {
        if (line.LineWidth() > mainline.LineWidth())
            area -= Block(-MSC_BIG_COORD, +MSC_BIG_COORD, xy.y+mainline.LineWidth(), MSC_BIG_COORD);
        else
            area += Block(xy.x+wh.x, xy.x, xy.y, xy.y+mainline.LineWidth());
    }
    return area;
}


/** Draw an arrowhead pointing to the right or to the left.
 * x,y is the tip of the arrow
 * which is 0 for destination, 1 for middle, 2 for source end of arrow
 */
void ArrowHead::Draw(XY xy, bool forward, bool bidir, MscArrowEnd which, MscDrawer *msc) const
{
    XY wh = getWidthHeight(bidir, which);
    if (bidir && which == MSC_ARROW_START)
        forward = !forward;
    if (forward) wh.x *= -1;

    MscFillAttr fill(line.color.second, GRADIENT_NONE);
    Contour tri1(xy + wh - XY(0, wh.y) - XY(0, wh.y), xy, xy+wh);
    Contour tri2(xy-wh, xy, xy - wh + XY(0, wh.y) + XY(0, wh.y));

    switch(GetType(bidir, which))
    {
    case MSC_ARROW_NONE:
        break;
    case MSC_ARROW_SOLID: /* Filled */
        msc->Fill(tri1, fill);
        if (bidir && (which==MSC_ARROW_MIDDLE)) msc->Fill(tri2, fill);
        break;
    case MSC_ARROW_EMPTY: /* Non-Filled */
        msc->Line(tri1, line);
        if (bidir && (which==MSC_ARROW_MIDDLE)) msc->Line(tri2, line);
        break;
    case MSC_ARROW_LINE: /* Two lines */
        msc->LineOpen(tri1, line);
        if (bidir && (which==MSC_ARROW_MIDDLE)) msc->LineOpen(tri2, line);
        break;
    case MSC_ARROW_HALF: /* Unfilled half */
        msc->Line(tri1.GetEdge(2).GetStart(), tri1.GetEdge(1).GetStart(), line);
        if (bidir && (which==MSC_ARROW_MIDDLE))
            msc->Line(tri2.GetEdge(0).GetStart(), tri2.GetEdge(1).GetStart(), line);
        break;

    case MSC_ARROW_DIAMOND:
        msc->Fill(diamond(xy, wh), fill);
        break;
    case MSC_ARROW_DIAMOND_EMPTY:
        msc->Line(diamond(xy, wh), line);
        break;

    case MSC_ARROW_DOT:
        msc->Fill(Contour(xy, wh.x, wh.y), fill);
        break;
    case MSC_ARROW_DOT_EMPTY:
        msc->Line(Contour(xy, wh.x, wh.y), line);
        break;
    }
}

//For largely internal use - characteristic size of the big arrowheads
XY ArrowHead::getBigWidthHeight(bool bidir, MscArrowEnd which) const
{
    static double sizes[] = {3, 5, 10, 15, 25, 35};
    switch(GetType(bidir, which)) {
    case MSC_ARROW_NONE:
    default:
        return XY(0,0);
    case MSC_ARROW_SOLID: /* Filled */
    case MSC_ARROW_EMPTY: /* Non-Filled */
    case MSC_ARROW_LINE: /* Two lines */
    case MSC_ARROW_HALF: /* Unfilled half */
        return XY(2*sizes[size.second], sizes[size.second]);
    case MSC_ARROW_DIAMOND:
    case MSC_ARROW_DIAMOND_EMPTY:
        return XY(2*sizes[size.second], sizes[size.second]);
    case MSC_ARROW_DOT:
    case MSC_ARROW_DOT_EMPTY:
        return XY(2*sizes[size.second], sizes[size.second]);
    }
}

/* return value:
 * - first is the extent on the left side of the entity line
 * - second is the extent on the right side of the entity line
 * formard is true if the start entiry of the arrow is left from its destination (like -> and not like <-)
 * if which is start, we swap the two values (as we should, to get the above)
 * body_height contains the height of the text within
 */
DoublePair ArrowHead::getBigWidths(bool forward, bool bidir, MscArrowEnd which, double body_height) const
{
    XY wh = getBigWidthHeight(bidir, which);
    DoublePair ret(0,0);
    switch(GetType(bidir, which)) {
    default:
    case MSC_ARROW_NONE:
        break;
    case MSC_ARROW_SOLID: /* Filled */
    case MSC_ARROW_EMPTY: /* Non-Filled */
    case MSC_ARROW_LINE: /* Two lines */
    case MSC_ARROW_HALF: /* Unfilled half */
        ret.first = wh.x;
        if (bidir && which == MSC_ARROW_MIDDLE)
            ret.second = ret.first;
        break;
    case MSC_ARROW_DIAMOND:
    case MSC_ARROW_DIAMOND_EMPTY:
        ret.first = wh.x*wh.y/(body_height/2+wh.y); //visible part towards the body
        ret.second = which==MSC_ARROW_MIDDLE ? ret.first : wh.x;
        break;
    case MSC_ARROW_DOT:
    case MSC_ARROW_DOT_EMPTY:
        //The same ellipse as will be drawn, but 90 degree rotated, so we can use CrossingVertical
        const EllipseData e(XY(0, 0), body_height/2 + wh.y, wh.x, 0);    
        double x[2], r[2];
        e.CrossingVertical(body_height/2, x, r);
        ret.first = fabs(x[0]); //visible part towards the body, any cp would do x[0]==-x[1]
        ret.second = which==MSC_ARROW_MIDDLE ? ret.first : wh.x;
        break;
    }
    if (!forward ^ (which==MSC_ARROW_START)) //if reverse or start, but not if both
        ret.swap();
    return ret;
}

bool ArrowHead::bigDoesSegment(bool bidir, MscArrowEnd which) const
{
    switch(GetType(bidir, which)) {
    default:
        case MSC_ARROW_SOLID: /* Filled */
        case MSC_ARROW_EMPTY: /* Non-Filled */
        case MSC_ARROW_LINE: /* Two lines */
        case MSC_ARROW_HALF: /* Unfilled half */
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
double ArrowHead::getBigMargin(Area text_cover, double sy, double dy, bool left, bool forward, bool bidir, MscArrowEnd which) const
{
    DoublePair tmp = getBigWidths(forward, bidir, which, dy-sy);
    const double asize = left ? tmp.second : -tmp.first;
    if (asize == 0) return 0; //no margin for no arrowhead
    Area arrow_head = Contour(asize, left ? -MSC_BIG_COORD : MSC_BIG_COORD, -MSC_BIG_COORD, +MSC_BIG_COORD) -
        BigCoverOne(0, sy, dy, forward, bidir, which);
    const Range left_right = text_cover.GetBoundingBox().x;
    text_cover.Rotate(90);
    arrow_head.Rotate(90);
    double off, tp;
    if (!left) {
        off = text_cover.OffsetBelow(arrow_head, tp, CONTOUR_INFINITY, false);
        //now off is negative and smaller than tp, which is probably negative, too
        return -left_right.till-off;
    } else {
        off = arrow_head.OffsetBelow(text_cover, tp, CONTOUR_INFINITY, false);
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
Area ArrowHead::BigCoverOne(double x, double sy, double dy, bool forward, bool bidir, MscArrowEnd which) const
{
    if (which == MSC_ARROW_START) forward = !forward;
    const XY wh = getBigWidthHeight(bidir, which);
    const double x_off = forward ? - wh.x : wh.x;
    const double mid_y = (sy+dy)/2;
    Area area;
    switch(GetType(bidir, which)) {
    default:
    case MSC_ARROW_NONE: break;
        //all these below draw a triangle
    case MSC_ARROW_SOLID: /* Filled */
    case MSC_ARROW_EMPTY: /* Non-Filled */
    case MSC_ARROW_LINE: /* Two lines */
    case MSC_ARROW_HALF: /* Unfilled half */
        area += Contour(x, mid_y, x+x_off, sy-wh.y, x+x_off, dy+wh.y);
        if (bidir && which == MSC_ARROW_MIDDLE)
            area += Contour(x, mid_y, x-x_off, sy-wh.y, x-x_off, dy+wh.y);
        break;
    case MSC_ARROW_DIAMOND:
    case MSC_ARROW_DIAMOND_EMPTY:
        area += Contour(x-wh.x, mid_y, x, sy-wh.y, x, dy+wh.y) + Contour(x+wh.x, mid_y, x, sy-wh.y, x, dy+wh.y);
        break;
    case MSC_ARROW_DOT:
    case MSC_ARROW_DOT_EMPTY:
        area += Contour(XY(x, mid_y), wh.x, (dy-sy)/2 + wh.y);    
        break;
    }
    return area;
}

Area ArrowHead::BigCover(std::vector<double> xPos, double sy, double dy, bool bidir) const
{
    Area area;
    const bool forward = xPos[0] < xPos[1];
    const MscArrowEnd i_begin = forward ? MSC_ARROW_START : MSC_ARROW_END;
    const MscArrowEnd i_end   = forward ? MSC_ARROW_END : MSC_ARROW_START;
    std::sort(xPos.begin(), xPos.end()); //from left to right

    //draw leftmost arrowhead
    area += BigCoverOne(xPos[0], sy, dy, forward, bidir, i_begin);
    double from_x = xPos[0] + getBigWidths(forward, bidir, i_begin, dy-sy).second;

    //set up variables for mid-points
    DoublePair mid_xx = getBigWidths(forward, bidir, MSC_ARROW_MIDDLE, dy-sy);
    const bool segment = bigDoesSegment(bidir, MSC_ARROW_MIDDLE);
    for (int i=1; i<xPos.size()-1; i++) {
        area += BigCoverOne(xPos[i], sy, dy, forward, bidir, MSC_ARROW_MIDDLE);
        if (segment) {
            double to_x = xPos[i] - mid_xx.first;
            area += Block(from_x, to_x, sy, dy);
            from_x = xPos[i] + mid_xx.second;
        }
    }
    //draw rightmost arrowhead
    area += BigCoverOne(xPos[xPos.size()-1], sy, dy, forward, bidir, i_end);
    double to_x = xPos[xPos.size()-1] - getBigWidths(forward, bidir, i_end, dy-sy).first;
    area += Block(from_x, to_x, sy, dy);

    return area;
}

void ArrowHead::BigDraw(const std::vector<double> &xPos, double sy, double dy, bool bidir, const MscFillAttr &fill, MscDrawer *msc) const
{
    Area area = BigCover(xPos, sy, dy, bidir);
    msc->Fill(area, fill);
    msc->Line(area, line);
}

