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
    MscLineAttr eLine(LINE_SOLID, MscColorType(0,0,0), 1, 0);
    msc->ClipRectangle(XY(HINT_GRAPHIC_SIZE_X*0.1,1), XY(HINT_GRAPHIC_SIZE_X-1, HINT_GRAPHIC_SIZE_Y-1), 0);
    msc->line(XY(xPos[1], 1), XY(xPos[1], HINT_GRAPHIC_SIZE_Y-1), eLine);
    ArrowHead ah(ArrowHead::BIGARROW);
    ah.line += MscColorType(128,128,0); //brown
    ah.endType.second = (MscArrowType)(int)p;
    ah.size.second = MSC_ARROWS_INVALID;
    MscFillAttr fill(ah.line.color.second.Lighter(0.7), GRADIENT_UP);
    ah.FillBig(xPos, HINT_GRAPHIC_SIZE_Y*0.3, HINT_GRAPHIC_SIZE_Y*0.7, false, msc, fill);
    ah.DrawBig(xPos, HINT_GRAPHIC_SIZE_Y*0.3, HINT_GRAPHIC_SIZE_Y*0.7, false, msc);
    msc->UnClip();
    return true;
}

bool CshHintGraphicCallbackForArrows(MscDrawer *msc, MscArrowType type, MscArrowSize size, bool left)
{
    if (!msc) return false;
    const double xx = left ? 0.9 : 0.7;
    XY xy(HINT_GRAPHIC_SIZE_X*xx, HINT_GRAPHIC_SIZE_Y/2);
    MscLineAttr eLine(LINE_SOLID, MscColorType(0,0,0), 1, 0);
    ArrowHead ah;
    ah.line += MscColorType(128,128,0); //brown
    ah.endType.second = type;
    ah.size.second = size;
    Range cover = ah.EntityLineCover(xy, true, false, MSC_ARROW_END, msc);
    msc->ClipRectangle(XY(1,1), XY(HINT_GRAPHIC_SIZE_X-1, HINT_GRAPHIC_SIZE_Y-1), 0);
    if (cover.from>1)
        msc->line(XY(xy.x, 1), XY(xy.x, cover.from), eLine);
    if (cover.till<HINT_GRAPHIC_SIZE_Y-1)
        msc->line(XY(xy.x, cover.till), XY(xy.x, HINT_GRAPHIC_SIZE_Y-1), eLine);
    double stop = ah.getWidthForLine(false, MSC_ARROW_END, msc).first;
    if (stop<HINT_GRAPHIC_SIZE_X*(xx-0.1))
        msc->line(XY(HINT_GRAPHIC_SIZE_X*0.1, xy.y), XY(xy.x-stop, xy.y), ah.line);
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

XY ArrowHead::getWidthHeight(bool bidir, MscArrowEnd which, MscDrawer *) const
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
 * - first is the forward direction of the arrowhead
 * - second is the reverse
 * E.g., the ->- arrow has >0 forward direction, but 0 at the reverse
 * E.g., the -><- arrow or the -O- symbol has both >0
 */
std::pair<double, double> ArrowHead::getWidthForLine(bool bidir, MscArrowEnd which, MscDrawer *_dummy) const
{
    std::pair<double, double> ret(0,0);
    switch (GetType(bidir, which))
    {
    case MSC_ARROW_NONE: /* No end */
    case MSC_ARROW_LINE: /* Two lines */
    case MSC_ARROW_HALF: /* Unfilled half */
        return ret;

    case MSC_ARROW_SOLID: /* Filled */
    case MSC_ARROW_EMPTY: /* Non-Filled */
        ret.first = getWidthHeight(bidir, which, _dummy).x;
        if (bidir)
            ret.second = ret.first;
        return ret;

    case MSC_ARROW_DIAMOND:
    case MSC_ARROW_DIAMOND_EMPTY:
    case MSC_ARROW_DOT:
    case MSC_ARROW_DOT_EMPTY:
    default:
        ret.first = getWidthHeight(bidir, which, _dummy).x;
        ret.second = ret.first;
        return ret;
    }
}


void triangle(cairo_t *cr,
                         double x1, double y1,
                         double x2, double y2,
                         double x3, double y3)
{
    cairo_move_to(cr, x1+CAIRO_OFF, y1+CAIRO_OFF);
    cairo_line_to(cr, x2+CAIRO_OFF, y2+CAIRO_OFF);
    cairo_line_to(cr, x3+CAIRO_OFF, y3+CAIRO_OFF);
    cairo_close_path(cr);
}

void diamond(cairo_t *cr, XY xy, XY wh)
{
    cairo_move_to(cr, xy.x-wh.x+CAIRO_OFF, xy.y+CAIRO_OFF);
    cairo_line_to(cr, xy.x+CAIRO_OFF, xy.y+wh.y+CAIRO_OFF);
    cairo_line_to(cr, xy.x+wh.x+CAIRO_OFF, xy.y+CAIRO_OFF);
    cairo_line_to(cr, xy.x+CAIRO_OFF, xy.y-wh.y+CAIRO_OFF);
    cairo_close_path(cr);
}

/** Return the Y range for which the entity line shall not be drawn
 */
Range  ArrowHead::EntityLineCover(XY xy, bool forward, bool bidir, MscArrowEnd which,
                                  MscDrawer *msc) const
{
    XY wh = getWidthHeight(bidir, which, msc);
    if (bidir && which == MSC_ARROW_START)
        forward = !forward;

    Range ret(0,0);

    switch(GetType(bidir, which))
    {
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


/** Draw an arrowhead pointing to the right or to the left.
 * x,y is the tip of the arrow
 * which is 0 for destination, 1 for middle, 2 for source end of arrow
 */
void ArrowHead::Draw(XY xy, bool forward, bool bidir, MscArrowEnd which,
                     MscDrawer *msc) const
{
    XY wh = getWidthHeight(bidir, which, msc);
    if (bidir && which == MSC_ARROW_START)
        forward = !forward;
    const double xOffset = forward ? -wh.x : wh.x;

    switch(GetType(bidir, which))
    {
    case MSC_ARROW_NONE:
        break;
    case MSC_ARROW_SOLID: /* Filled */
        triangle(msc->GetContext(), xy.x, xy.y,
                 xy.x + xOffset, xy.y + wh.y,
                 xy.x + xOffset, xy.y - wh.y);
        if (bidir && (which==MSC_ARROW_MIDDLE))
            triangle(msc->GetContext(), xy.x, xy.y,
                     xy.x - xOffset, xy.y + wh.y,
                     xy.x - xOffset, xy.y - wh.y);
        msc->SetColor(line.color.second);
        cairo_fill(msc->GetContext());
        break;

    case MSC_ARROW_EMPTY: /* Non-Filled */
        cairo_move_to(msc->GetContext(), xy.x + xOffset+CAIRO_OFF, xy.y + wh.y+CAIRO_OFF);
        cairo_line_to(msc->GetContext(), xy.x + xOffset+CAIRO_OFF, xy.y - wh.y+CAIRO_OFF);
        if (bidir && (which==MSC_ARROW_MIDDLE)) {
            cairo_move_to(msc->GetContext(), xy.x - xOffset+CAIRO_OFF, xy.y + wh.y+CAIRO_OFF);
            cairo_line_to(msc->GetContext(), xy.x - xOffset+CAIRO_OFF, xy.y - wh.y+CAIRO_OFF);
        }
        //fallthrough
    case MSC_ARROW_LINE: /* Two lines */
        cairo_move_to(msc->GetContext(), xy.x + xOffset+CAIRO_OFF, xy.y - wh.y+CAIRO_OFF);
        cairo_line_to(msc->GetContext(), xy.x+CAIRO_OFF, xy.y+CAIRO_OFF);
        if (bidir && (which==MSC_ARROW_MIDDLE))
            cairo_line_to(msc->GetContext(), xy.x - xOffset+CAIRO_OFF, xy.y - wh.y+CAIRO_OFF);
        //fallthrough
    case MSC_ARROW_HALF: /* Unfilled half */
        cairo_move_to(msc->GetContext(), xy.x + xOffset+CAIRO_OFF, xy.y + wh.y+CAIRO_OFF);
        cairo_line_to(msc->GetContext(), xy.x+CAIRO_OFF, xy.y+CAIRO_OFF);
        if (bidir && (which==MSC_ARROW_MIDDLE))
            cairo_line_to(msc->GetContext(), xy.x - xOffset+CAIRO_OFF, xy.y + wh.y+CAIRO_OFF);
        msc->SetLineAttr(line);
        cairo_stroke(msc->GetContext());
        break;

    case MSC_ARROW_DIAMOND:
        diamond(msc->GetContext(), xy, wh);
        msc->SetColor(line.color.second);
        cairo_fill(msc->GetContext());
        break;

    case MSC_ARROW_DIAMOND_EMPTY:
        msc->SetLineAttr(line);
        diamond(msc->GetContext(), xy, wh);
        cairo_stroke(msc->GetContext());
        break;

    case MSC_ARROW_DOT:
        cairo_new_sub_path(msc->GetContext());
        cairo_arc (msc->GetContext(), xy.x, xy.y, wh.x, 0, 2*M_PI);
        msc->SetColor(line.color.second);
        cairo_fill(msc->GetContext());
        break;

    case MSC_ARROW_DOT_EMPTY:
        msc->SetLineAttr(line);
        cairo_new_sub_path(msc->GetContext());
        cairo_arc (msc->GetContext(), xy.x, xy.y, wh.x, 0, 2*M_PI);
        cairo_stroke(msc->GetContext());
        break;
    }
}

//For largely internal use - characteristic size of the big arrowheads
XY ArrowHead::getBigWidthHeight(MscArrowType type, MscDrawer *) const
{
    static double sizes[] = {3, 5, 10, 15, 25, 35};
    switch(type)
    {
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

//How much the arrowhead extends outwards from the entity line
double ArrowHead::getBigEndWidthExt(bool bidir, MscArrowEnd which, MscDrawer *msc) const
{
    switch(GetType(bidir, which))
    {
    default:
    case MSC_ARROW_NONE:
    case MSC_ARROW_SOLID: /* Filled */
    case MSC_ARROW_EMPTY: /* Non-Filled */
    case MSC_ARROW_LINE: /* Two lines */
    case MSC_ARROW_HALF: /* Unfilled half */
        return 0;
    case MSC_ARROW_DIAMOND:
    case MSC_ARROW_DIAMOND_EMPTY:
    case MSC_ARROW_DOT:
    case MSC_ARROW_DOT_EMPTY:
        return getBigWidthHeight(GetType(bidir, which), msc).x;
    }
}

Block ArrowHead::big_cover_block(double x, double sy, double dy,
                                 bool bidir, MscArrowEnd which, bool forward, MscDrawer *msc) const
{
    double y2 = getBigWidthHeight(GetType(bidir, which), msc).y;
    double x2, x3;
    switch(GetType(bidir, which))
    {
    default:
    case MSC_ARROW_NONE:
        x2 = x3 = 0;
        break;
    case MSC_ARROW_SOLID: /* Filled */
    case MSC_ARROW_EMPTY: /* Non-Filled */
    case MSC_ARROW_LINE: /* Two lines */
    case MSC_ARROW_HALF: /* Unfilled half */
        x2 = getBigWidthHeight(bidir, which, msc).x; //inner
        x3 = 0; //outer
        break;
    case MSC_ARROW_DIAMOND:
    case MSC_ARROW_DIAMOND_EMPTY:
    case MSC_ARROW_DOT:
    case MSC_ARROW_DOT_EMPTY:
        x2 = getBigWidthHeight(GetType(bidir, which), msc).x/2; //inner
        x3 = x2 *2; //outer
        break;
    }
    if (which == MSC_ARROW_MIDDLE) //x3 not used - symmetric
        return Block(x-x2, x+x2, sy-y2, dy+y2);
    //END or START
    if (forward ^ (which==MSC_ARROW_START))
        return Block(x-x2, x+x3, sy-y2, dy+y2);
    else
        return Block(x-x3, x+x2, sy-y2, dy+y2);
}

//How much margin we need for this arrowhead
double ArrowHead::getBigEndWidthMargin(bool bidir, MscArrowEnd which, MscDrawer *msc) const
{
    switch(GetType(bidir, which))
    {
    default:
        case MSC_ARROW_NONE:
        case MSC_ARROW_SOLID: /* Filled */
        case MSC_ARROW_EMPTY: /* Non-Filled */
        case MSC_ARROW_LINE: /* Two lines */
        case MSC_ARROW_HALF: /* Unfilled half */
            return getBigWidthHeight(GetType(bidir, which), msc).x;
        case MSC_ARROW_DIAMOND:
        case MSC_ARROW_DIAMOND_EMPTY:
        case MSC_ARROW_DOT:
        case MSC_ARROW_DOT_EMPTY:
            return -getBigWidthHeight(GetType(bidir, which), msc).x/2;
    }
}

double ArrowHead::Big_yExtent(bool bidir, bool segmented, MscDrawer *msc) const
{
    double ret = getBigWidthHeight(bidir, MSC_ARROW_START, msc).x;
    ret = std::max(ret, getBigWidthHeight(bidir, MSC_ARROW_END, msc).x);
    if (segmented)
        ret = std::max(ret, getBigWidthHeight(bidir, MSC_ARROW_MIDDLE, msc).x);
    return ret;
}

//type cannot be DOUBLE
//dir is true if we draw to the right side of x
//up is true if we need to draw the upper part
//We start from visual left to right on the top side, then from right to left on
//the bottom side, so if dir and up is true we start the path and pos is unset
//X: pos shows the x pos we stopped drawing the last segment
//Y: if up is true current ypos is sy-lw, if false, it is dy+lw
//x is the xpos of the entity line
//sy dy are the top and bottom of the arrow
//lw is the "wider" value for double lines
void ArrowHead::big_end_path(double &xpos, double x, double sy, double dy, bool dir,
                             bool up, MscDrawer *msc, MscArrowType atype,
                             MscLineType type, double lw) const
{
    cairo_t *cr = msc->GetContext();
    //helper values
    const double ypos = up ? sy+lw : dy-lw; //just a helper
    const XY a = getBigWidthHeight(atype, msc);
    switch (atype) {
    case MSC_ARROW_NONE:
        if (up) {
            if (dir) {
                xpos = x+lw;
            } else {
                msc->_line_path(xpos, ypos, x-lw, ypos, 0, type);
                xpos = x-lw;
            }
        } else {
            if (dir) {
                msc->_line_path(xpos, ypos, x+lw, ypos, 0, type);
                msc->_line_path(x+lw, ypos, x+lw, sy+lw, 0, type);
                //last piece, no need for xpos any more
            } else {
                msc->_line_path(xpos, sy+lw, xpos, ypos, 0, type);
                //no change to xpos
            }
        }
        break;
    case MSC_ARROW_SOLID: /* Filled */
    case MSC_ARROW_EMPTY: /* Non-Filled */
    case MSC_ARROW_LINE: /* Two lines */
    case MSC_ARROW_HALF: /* Unfilled half */
	{
            //lwx and lwy are x and y projections of a slanted lw distance
            const double Y = (dy-sy)/2 + a.y;
            const double A = sqrt(a.x*a.x + Y*Y);
            const double lwx = A/Y*lw;
            const double lwy = (A+Y)/a.x*lw;
            if (up) {
                if (dir) {
                    xpos = x+a.x-lw;
                    msc->_line_path(x+lwx, (sy+dy)/2, xpos, sy-a.y+lwy, 0, type);
                    msc->_line_path(xpos, sy-a.y+lwy, xpos, ypos, 0, type);
                } else {
                    msc->_line_path(xpos, ypos, x-a.x+lw, ypos, 0, type);
                    xpos = x-a.x+lw;
                    msc->_line_path(xpos, ypos, xpos, sy-a.y+lwy, 0, type);
                    msc->_line_path(xpos, sy-a.y+lwy, x-lwx, (sy+dy)/2, 0, type);
                    msc->_line_path(x-lwx, (sy+dy)/2, xpos, dy+a.y-lwy, 0, type);
                    msc->_line_path(xpos, dy+a.y-lwy, xpos, dy-lw, 0, type);
                }
            } else {
                if (dir) {
                    msc->_line_path(xpos, ypos, x+a.x-lw, ypos, 0, type);
                    xpos = x+a.x-lw;
                    msc->_line_path(xpos, ypos, xpos, dy+a.y-lwy, 0, type);
                    msc->_line_path(xpos, dy+a.y-lwy, x+lwx, (sy+dy)/2, 0, type);
                    //last piece, no need for xpos any more
                } else {
                    //We have drawn all the arrow in the up phase
                    //no change to xpos
                }
            }
            break;
        }
    case MSC_ARROW_DOT:
    case MSC_ARROW_DOT_EMPTY:
        {
            const XY c(x, (sy+dy)/2);
            const XY wh(a.x-lw, (dy-sy)/2 + a.y-lw); //radius
            const double angle = asin(1-a.y/wh.y)*180./M_PI;
            const double dx = wh.x*cos(angle*M_PI/180.);
            if (up) {
                if (dir) {
                    msc->_arc_path(c, wh+wh, angle, 360-angle, lw, type, false);
                    xpos = x + dx;
                    if (type == LINE_SOLID || !msc->fake_dash) msc->_line_path_to(xpos, ypos, 0, type);
                } else {
                    msc->_line_path(xpos, ypos, x-dx, ypos, 0, type);
                    msc->_arc_path(c, wh+wh, 180+angle, 180-angle, lw, type, false);
                    xpos = x - dx;
                    if (type == LINE_SOLID || !msc->fake_dash)msc->_line_path_to(xpos, dy-lw, 0, type);
                }
            } else {
                if (dir) {
                    msc->_line_path(xpos, ypos, x+dx, ypos, 0, type);
                } else {
                    //nothing to do
                }
            }
            break;
        }
    case MSC_ARROW_DIAMOND:
    case MSC_ARROW_DIAMOND_EMPTY:
        {
            //lwx and lwy are x and y projections of a slanted lw distance
            const double Y = (dy-sy)/2 + a.y;
            const double A = sqrt(double(a.x*a.x + Y*Y));
            const double lwx = A/Y*lw;
            const double lwy = A/a.x*lw;
            const double lwx2 = a.x/a.y*lw - lwx;
            if (up) {
                if (dir) {
                    xpos = x+a.x/2-lwx2;
                    msc->_line_path(x-a.x+lwx, (sy+dy)/2, x, sy-a.y+lwy, 0, type);
                    msc->_line_path(x, sy-a.y+lwy, xpos, ypos, 0, type);
                } else {
                    msc->_line_path(xpos, ypos, x-a.x/2+lwx2, ypos, 0, type);
                    xpos = x-a.x/2+lwx2;
                    msc->_line_path(xpos, ypos, x, sy-a.y+lwy, 0, type);
                    msc->_line_path(x, sy-a.y+lwy, x+a.x-lwx, (sy+dy)/2, 0, type);
                    msc->_line_path(x+a.x-lwx, (sy+dy)/2, x, dy+a.y-lwy, 0, type);
                    msc->_line_path(x, dy+a.y-lwy, xpos, dy-lw, 0, type);
                }
            } else {
                if (dir) {
                    msc->_line_path(xpos, ypos, x+a.x/2-lwx2, ypos, 0, type);
                    msc->_line_path(x+a.x/2-lwx2, ypos, x, dy+a.y-lwy, 0, type);
                    msc->_line_path(x, dy+a.y-lwy, x-a.x+lwx, (sy+dy)/2, 0, type);
                    //last piece - xpos is not needed
                } else {
                    //We have drawn all the arrow in the up phase
                    //no change to xpos
                }
            }
            break;
        }
    }
}

void ArrowHead::big_mid_path(double &xpos, double x, double sy, double dy,
                             bool up, MscDrawer *msc, MscArrowType atype, MscLineType type, double lw) const
{
    cairo_t *cr = msc->GetContext();
    const double ypos = up ? sy+lw : dy-lw; //just a helper
    const XY a = getBigWidthHeight(atype, msc);
    switch (atype) {
    case MSC_ARROW_NONE:
        //Nothing to do
        break;
    case MSC_ARROW_SOLID: /* Filled */
    case MSC_ARROW_EMPTY: /* Non-Filled */
    case MSC_ARROW_LINE: /* Two lines */
    case MSC_ARROW_HALF: /* Unfilled half */
        assert(0); //should not happen
        break;
    case MSC_ARROW_DOT:
    case MSC_ARROW_DOT_EMPTY:
        {
            const XY c(x, (sy+dy)/2);
            const XY wh(a.x-lw, (dy-sy)/2 + a.y-lw); //radius
            const double angle = asin(1-a.y/wh.y)*180./M_PI;
            const double dx = wh.x*cos(angle*M_PI/180.);
            if (up) {
                msc->_line_path(xpos, ypos, x-dx, ypos, 0, type);
                msc->_arc_path(c, wh+wh, 180+angle, 360-angle, lw, type, false);
                xpos = x + dx;
				if (type == LINE_SOLID || !msc->fake_dash) msc->_line_path_to(xpos, ypos, 0, type);
            } else {
                msc->_line_path(xpos, ypos, x+dx, ypos, 0, type);
                msc->_arc_path(c, wh+wh, angle, 180-angle, lw, type, false);
                xpos = x - dx;
                if (type == LINE_SOLID || !msc->fake_dash)msc->_line_path_to(xpos, ypos, 0, type);
            }
            break;
        }
    case MSC_ARROW_DIAMOND:
    case MSC_ARROW_DIAMOND_EMPTY:
        //lwx and lwy are x and y projections of a slanted lw distance
        const double Y = (dy-sy)/2 + a.y;
        const double A = sqrt(double(a.x*a.x + Y*Y));
        const double lwx = A/Y*lw;
        const double lwy = A/a.x*lw;
        const double lwx2 = a.x/a.y*lw - lwx;
        if (up) {
            msc->_line_path(xpos, ypos, x-a.x/2+lwx2, ypos, 0, type);
            msc->_line_path(x-a.x/2+lwx2, ypos, x, sy-a.y+lwy, 0, type);
            xpos = x+a.x/2-lwx2;
            msc->_line_path(x, sy-a.y+lwy, xpos, ypos, 0, type);
        } else {
            msc->_line_path(xpos, ypos, x+a.x/2-lwx2, ypos, 0, type);
            msc->_line_path(x+a.x/2-lwx2, ypos, x, dy+a.y-lwy, 0, type);
            xpos = x-a.x/2+lwx2;
            msc->_line_path(x, dy+a.y-lwy, xpos, ypos, 0, type);
        }
        break;
    }
}

//type cannot be DOUBLE
void ArrowHead::big_path(std::vector<double> xPos, double sy, double dy,
                        MscDrawer * msc, MscLineType type, bool bidir, double lw) const
{
    unsigned drawn_till = 0;
    double xpos;
    const MscArrowEnd i_begin = xPos[0] < xPos[1] ? MSC_ARROW_START : MSC_ARROW_END;
    const MscArrowEnd i_end   = xPos[0] < xPos[1] ? MSC_ARROW_END : MSC_ARROW_START;

    std::sort(xPos.begin(), xPos.end()); //from left to right

    if (type == LINE_DOUBLE) type = LINE_SOLID;

    //Draw the block arrow.
    //Stop at each SOLID arrowhead, we do a separate sub_path for those
    while (drawn_till < xPos.size()-1) {
        //Initially assume we draw all the way to the end
        unsigned draw_upto = xPos.size()-1;
        msc->_new_path();
        //Draw upper part of the leftmost arrowhead
        big_end_path(xpos, xPos[drawn_till], sy, dy, true, true, msc, GetType(bidir, i_begin), type, lw);
        //Draw the top parts of mid-arrows
        for (int i=drawn_till+1; i<xPos.size()-1; i++) {
            //If in the middle there is a SOLID arrowhead, stop and turn back
            const MscArrowType atype = GetType(bidir, MSC_ARROW_MIDDLE);
            if (atype == MSC_ARROW_SOLID || atype == MSC_ARROW_EMPTY ||
                atype == MSC_ARROW_LINE || atype == MSC_ARROW_HALF) {
                draw_upto = i;
                break;
            }
            big_mid_path(xpos, xPos[i], sy, dy, true, msc, GetType(bidir, MSC_ARROW_MIDDLE), type, lw);
        }
        //Draw top and bottom part of the right arrowhead
        big_end_path(xpos, xPos[draw_upto], sy, dy, false,  true, msc, GetType(bidir, i_end), type, lw);
        big_end_path(xpos, xPos[draw_upto], sy, dy, false, false, msc, GetType(bidir, i_end), type, lw);
        //Drow bottom part of mid-arrows
        for (int i=draw_upto-1; i>drawn_till; i--)
            big_mid_path(xpos, xPos[i], sy, dy, false, msc, GetType(bidir, MSC_ARROW_MIDDLE), type, lw);
        //Finally the bottom part of the leftmost arrowhead
        big_end_path(xpos, xPos[drawn_till], sy, dy, true, false, msc, GetType(bidir, i_begin), type, lw);

        //If there are more segments to go open a new sub-path
        if (draw_upto<xPos.size()-1)
            msc->_new_path();
        cairo_close_path(msc->GetContext());
        drawn_till = draw_upto;
    }
}

void ArrowHead::DrawBig(const std::vector<double> &xPos, double sy, double dy,
                        bool bidir, MscDrawer * msc)
{
    if (line.type.second == LINE_NONE || !line.color.second.valid) return;
    big_path(xPos, sy, dy, msc, line.type.second, bidir, 0);
    if (line.type.second == LINE_DOUBLE)
        big_path(xPos, sy, dy, msc, LINE_SOLID, bidir, 2*line.width.second);
    msc->SetLineAttr(line);
    cairo_stroke(msc->GetContext());
}

void ArrowHead::FillBig(const std::vector<double> &xPos, double sy, double dy,
                        bool bidir, MscDrawer *msc, MscFillAttr fill)
{
    ClipBig(xPos, sy, dy, bidir, msc);
    const double extent = Big_yExtent(bidir, xPos.size()>2, msc);
    const double sext = getBigEndWidthExt(bidir, MSC_ARROW_START, msc);
    const double dext = getBigEndWidthExt(bidir, MSC_ARROW_END, msc);
    double sx = std::min(xPos[0], xPos[xPos.size()-1]);
    double dx = std::max(xPos[0], xPos[xPos.size()-1]);
    if (xPos[0] < xPos[1]) {
        sx = xPos[0] - sext;
        dx = xPos[xPos.size()-1] + dext;
    } else {
        sx = xPos[xPos.size()-1] - dext;
        dx = xPos[0] + sext;
    }
    msc->filledRectangle(XY(sx, sy-extent), XY(dx,dy+extent), fill);
    msc->UnClip();
}

//Also extends mainline
void ArrowHead::CoverBig(const std::vector<double> &xPos, double sy, double dy,
                         bool bidir, MscDrawer * msc, Geometry &cover) const
{
    const bool forward = xPos[0] < xPos[1];
    cover += big_cover_block(xPos[0],             sy, dy, bidir, MSC_ARROW_START, forward, msc);
    cover += big_cover_block(xPos[xPos.size()-1], sy, dy, bidir, MSC_ARROW_END,   forward, msc);
    for (int i=1; i<xPos.size()-1; i++)
        cover += big_cover_block(xPos[i], sy, dy, bidir, MSC_ARROW_MIDDLE, forward, msc);
	cover.mainline += Range(sy, dy);
}

void ArrowHead::ClipBig(std::vector<double> xPos, double sy, double dy,
                 bool bidir, MscDrawer *m) const
{
	big_path(xPos, sy, dy, m, LINE_SOLID, bidir, 0);
	m->Clip();
}
