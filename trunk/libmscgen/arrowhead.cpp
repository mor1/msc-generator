/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011,2012,2013,2014 Zoltan Turanyi
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
/** @file arrowhead.cpp The implementation of arrowhead styles and drawing including block arrows.
 * @ingroup libmscgen_files */

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include "msc.h"

template<> const char EnumEncapsulator<EArrowType>::names[][ENUM_STRING_LEN] =
    {"invalid", "none", "solid", 
     "empty", 
     "line", 
     "half", 
     "diamond", 
     "empty_diamond", 
     "dot", 
     "empty_dot", 
     "sharp", "empty_sharp",
     "double", "double_empty", "double_line", "double_half",
     "triple", "triple_empty", "triple_line", "triple_half", "jumpover",
     "empty_inv", "stripes", "triangle_stripes", ""};
template<> const char * const EnumEncapsulator<EArrowType>::descriptions[] =
    {NULL, "No arrowhead, just a line (or rectangle for block arrows).", "Arrows: a filled equilateral triangle.\nBlock arrows: a triangle wider than the body.", 
     "Arrows: A line-only equilateral triangle.\nBlock arrows: A triangle of same with as the body.", 
     "Arrows: Two-lines.\nBlock arrows: a triangle wider than the body.", 
     "Arrows: Just one line.\nBlock arrows: a lop-sided triangle.", 
     "Arrows: A filled diamond shape.\nBlock arrows:A diamond shape wider than the body.", 
     "Arrows: A line-only diamond.\nBlock arrows:A triangle of same with as the body, extending beyond the entity line.", 
     "Arrows: A filled circle.\nBlock arrows:A circle wider than the body.", 
     "Arrows: A line-only circle.\nBlock arrows:A round ending.", 
     "Arrows: A filled acute triangle.\nBlock arrows:An acute triangle.", 
     "Arrows: A line-only acute triangle.\nBlock arrows: A triangle of same with as the body.",
     "Two filled triangles.", "Two line-only triangles.", "Two times two lines.", "Two times just one line.",
     "Three filled triangles.", "Three line-only triangles.", "Three times two lines.", "Three times just one line.",
     "A little half circle, jumping over the entity line indicating that we skip this entity. Best used for arrow.skipType.",
     "An inverse (cut-out) equilateral triangle.", "Two straight stripes.", "Two bent stripes.", ""};

template<> const char EnumEncapsulator<EArrowSize>::names[][ENUM_STRING_LEN] =
    {"invalid", "tiny", "small", "normal", "big", "huge", ""};
template<> const char * const EnumEncapsulator<EArrowSize>::descriptions[] = {
    NULL, "40% of normal.", "66% of normal.", "What feels just right.", "133% of normal.", "173% of normal.", ""};

const double ArrowHead::arrowSizePercentage[6] = {
    20,  /* INVALID, value is set for big arrow hint pictograms */
    30,  /* MSC_ARROW_TINY */
    50,  /* MSC_ARROW_SMALL */
    75,  /* MSC_ARROW_NORMAL */
    100,  /* MSC_ARROW_BIG */
    130   /* MSC_ARROW_HUGE */
};

const double ArrowHead::baseArrowWidth = 17;    /* Arrow size for BIG */
const double ArrowHead::baseArrowHeight = 10;    /* Half Arrow size for BIG */
const double ArrowHead::baseDiamondSize = 10;    /* Half Diamond size for BIG */
const double ArrowHead::baseDotSize = 9;    /* Dot radius for BIG */

const double SHARP_MUL_1 = 1.2;  /* How much wider is a sharp arrow than a normal one */
const double SHARP_MUL_2 = 0.7;  /* What percentage of the width of a sharp arrow is at the line */

void ArrowHead::Empty() {
    line.Empty();
    size.first = false;
    endType.first = false;
    midType.first = false;
    skipType.first = false;
    startType.first = false;
    xmul.first = false;
    ymul.first = false;
}


void ArrowHead::MakeComplete() 
{
    if (!size.first) {size.first = true; size.second= MSC_ARROW_SMALL;}
    if (!xmul.first) {xmul.first = true; xmul.second= 1;}
    if (!ymul.first) {ymul.first = true; ymul.second= 1;}
    if (!endType.first) {endType.first = true; endType.second= MSC_ARROW_SOLID;}
    if (!midType.first) { midType.first = true; midType.second = MSC_ARROW_SOLID; }
    if (!skipType.first) { skipType.first = true; skipType.second = MSC_ARROW_NONE; }
    if (!startType.first) { startType.first = true; startType.second = MSC_ARROW_NONE; }
    line.MakeComplete();
}

ArrowHead & ArrowHead::operator += (const ArrowHead &toadd)
{
    //in all ernesty we strive to keep skipType NONE for bigarrows.
    const ArrowHead &a = static_cast<const ArrowHead&>(toadd);
    line += a.line;
    if (a.size.first) size = a.size;
    if (a.endType.first) endType = a.endType;
    if (a.midType.first) midType = a.midType;
    if (a.skipType.first && type != BIGARROW) skipType = a.skipType;
    if (a.startType.first) startType = a.startType;
    if (a.xmul.first) xmul = a.xmul;
    if (a.ymul.first) ymul = a.ymul;
    return *this;
}

/** Take an Attribute object and add its value to us.
 * We recognize 'arrow', '*.type', '*.endtyoe', 
 * '*.midtype', '*.starttype', '*.size', 'arrowsize',
 * '*.xmul', '*.ymul', '*.color' and '*.line.width'.
 */
bool ArrowHead::AddAttribute(const Attribute &a, Msc *msc, EStyleType t)
{
    if (a.type == MSC_ATTR_STYLE) {
        if (msc->Contexts.back().styles.find(a.name) == msc->Contexts.back().styles.end()) {
            a.InvalidStyleError(msc->Error);
            return true;
        }
        const MscStyle &style = msc->Contexts.back().styles[a.name].read();
        if (style.f_arrow) operator +=(style.arrow);
        return true;
    }
    //do this first since ".type" ending is checked for below.
    if (a.EndsWith("color") || a.EndsWith("line.width") ||
        a.EndsWith("line.type")) {
        return line.AddAttribute(a, msc, t);
    }
    if (type == BIGARROW && a.EndsWith("skiptype")) {
        msc->Error.Error(a, false, "Attribute 'skipType' is not applicable to block arrows. Ignoring it.");
        return true;
    }
    std::pair<bool, EArrowType> *pType = NULL, *pType2 = NULL;
    if (a.Is("arrow") || a.EndsWith("type") || a.EndsWith("endtype")) {pType = &endType; pType2=&midType;}
    else if (a.EndsWith("starttype")) pType = &startType;
    else if (a.EndsWith("midtype")) pType = &midType; 
    else if (a.EndsWith("skiptype")) pType = &skipType;
    if (pType) {
        if (a.Is("arrow"))
            msc->Error.Warning(a, false, "Attribute 'arrow' is deprecated, but understood.",
                                "Use 'arrow.type'.");
        if (pType != &endType && type == NOTE) {
            msc->Error.Error(a, false, "Only the end type of a note poiner can be set. Ignoring '"+a.name+"' attribute.");
            return true;
        }
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, t)) {
                pType->first = false;
                if (pType2) pType2->first = false;
            }
            return true;
        }
        EArrowType at;
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
            msc->Error.Warning(a, false, "Attribute 'arrowsize' is deprecated, but understood.",
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
    return false;
}

void ArrowHead::AttributeNames(Csh &csh, const string &prefix)
{
    static const char * const names_descriptions[] ={
        "invalid", NULL,
        "type", "Select the type of the arrowhead, such as sharp, solid, double, etc. A shorthand for setting both 'endtype' and 'midtype'.",
        "size", "The size of the arrowhead.",
        "color", "The color of the arrowheads",
        "starttype", "The arrowhead type used where the arrow starts.",
        "midtype", "The arrowhead type to use for multi-segment arrows, where the arrow stops an intermediate entity.",
        "skiptype", "The arrowhead type to use where the arrow goes over, but does not touch an entity.",
        "endtype", "The arrowhead type to use at the destination of the arrow.",
        "line.width", "The line with of line-like arrowheads.",
        "line.color", "The color of the line of the arrwhead.",
        "line.type", "The line style of the line-like arrowheads, like dotted, dashed, etc.",
        "xmul", "A size multiplier applied to the width arrowheads. To increase width, for example, specify a number greater than 1.",
        "ymul", "A size multiplier applied to the height arrowheads. To decrease height, for example, specify a number smaller than 1.",
        ""};
    csh.AddToHints(names_descriptions, csh.HintPrefix(COLOR_ATTRNAME)+prefix, EHintType::ATTR_NAME);
}

/** Callback for drawing a symbol before arrowhead type names for line arrows in the hints popup list box.
 * @ingroup libmscgen_hintpopup_callbacks*/
bool CshHintGraphicCallbackForArrows(Canvas *canvas, EArrowType type, EArrowSize size, bool left);

/** Callback for drawing a symbol before arrowhead type names for block arrows in the hints popup list box.
 * @ingroup libmscgen_hintpopup_callbacks*/
bool CshHintGraphicCallbackForBigArrows(Canvas *canvas, CshHintGraphicParam p, Csh&)
{
    if (!canvas) return false;
    if (!MSC_ARROW_OK_FOR_BIG_ARROWS((EArrowType)(int)p)) {
        if (MSC_ARROW_OK_FOR_ARROWS((EArrowType)(int)p))
            return CshHintGraphicCallbackForArrows(canvas, (EArrowType)(int)p, MSC_ARROW_SMALL, false);
        else return false;
    }
    std::vector<double> xPos(2); 
    xPos[0] = 0;
    xPos[1] = HINT_GRAPHIC_SIZE_X*0.7;
    LineAttr eLine(LINE_SOLID, ColorType(0,0,0), 1, CORNER_NONE, 0);
    canvas->Clip(XY(HINT_GRAPHIC_SIZE_X*0.1,1), XY(HINT_GRAPHIC_SIZE_X-1, HINT_GRAPHIC_SIZE_Y-1));
    canvas->Line(XY(xPos[1], 1), XY(xPos[1], HINT_GRAPHIC_SIZE_Y-1), eLine);
    ArrowHead ah(ArrowHead::BIGARROW);
    ah.line += ColorType(0,192,32); //green-blue
    ah.endType.second = (EArrowType)(int)p;
    ah.size.second = MSC_ARROWS_INVALID;
    ShadowAttr shadow;
    FillAttr fill(ah.line.color.second.Lighter(0.7), GRADIENT_UP);
    std::vector<double> active(2,0.);
    ah.BigCalculateAndDraw(xPos, active, HINT_GRAPHIC_SIZE_Y*0.3, HINT_GRAPHIC_SIZE_Y*0.7, 
                           true, false, fill, shadow, *canvas);
    canvas->UnClip();
    return true;
}

bool CshHintGraphicCallbackForArrows(Canvas *canvas, EArrowType type, EArrowSize size, bool left)
{
    if (!canvas) return false;
    const double xx = left ? 0.3 : 0.7;
    XY xy(HINT_GRAPHIC_SIZE_X*xx, HINT_GRAPHIC_SIZE_Y/2);
    LineAttr eLine(LINE_SOLID, ColorType(0,0,0), 1, CORNER_NONE, 0);
    ArrowHead ah;
    ah.line += ColorType(0,192,32); //green-blue
    ah.endType.second = type;
    ah.size.second = size;
    Range cover = ah.EntityLineCover(xy, left, false, MSC_ARROW_END, eLine, eLine).GetBoundingBox().y;
    canvas->Clip(XY(1,1), XY(HINT_GRAPHIC_SIZE_X-1, HINT_GRAPHIC_SIZE_Y-1));
    if (cover.from>1)
        canvas->Line(XY(xy.x, 1), XY(xy.x, cover.from), eLine);
    if (cover.till<HINT_GRAPHIC_SIZE_Y-1)
        canvas->Line(XY(xy.x, cover.till), XY(xy.x, HINT_GRAPHIC_SIZE_Y-1), eLine);
    Contour clip = ah.ClipForLine(xy, 0, !left, false, MSC_ARROW_END, Block(0, HINT_GRAPHIC_SIZE_X, 0, HINT_GRAPHIC_SIZE_Y), eLine, eLine);
    canvas->Clip(clip);
    canvas->Line(XY(HINT_GRAPHIC_SIZE_X*0.1, xy.y), xy, ah.line);
    canvas->UnClip();
    ah.Draw(xy, 0, true, false, MSC_ARROW_END, ah.line, ah.line, canvas);
    canvas->UnClip();
    return true;
}

/** Callback for drawing a symbol before arrowhead type names (in general) in the hints popup list box.
 * @ingroup libmscgen_hintpopup_callbacks*/
bool CshHintGraphicCallbackForArrowTypes(Canvas *canvas, CshHintGraphicParam p, Csh&csh)
{
    if (!MSC_ARROW_OK_FOR_ARROWS((EArrowType)(int)p)) {
        if (MSC_ARROW_OK_FOR_BIG_ARROWS((EArrowType)(int)p))
            return CshHintGraphicCallbackForBigArrows(canvas, p, csh);
        else return false;
    }
    return CshHintGraphicCallbackForArrows(canvas, (EArrowType)(int)p, MSC_ARROW_SMALL, false);
}

/** Callback for drawing a symbol before arrowhead size names in the hints popup list box.
 * @ingroup libmscgen_hintpopup_callbacks*/
bool CshHintGraphicCallbackForArrowSizes(Canvas *canvas, CshHintGraphicParam p, Csh&)
{
    return CshHintGraphicCallbackForArrows(canvas, MSC_ARROW_SOLID, (EArrowSize)(int)p, true);
}

bool ArrowHead::AttributeValues(const std::string &attr, Csh &csh, EArcArrowType t)
{
    if (CaseInsensitiveEqual(attr, "arrow") ||
        CaseInsensitiveEndsWith(attr, "type") ||
        CaseInsensitiveEndsWith(attr, "starttype") ||
        CaseInsensitiveEndsWith(attr, "midtype") ||
        CaseInsensitiveEndsWith(attr, "skiptype") ||
        CaseInsensitiveEndsWith(attr, "endtype")) {
        for (int i=1; EnumEncapsulator<EArrowType>::names[i][0]; i++)
            if (t==ANY || (t==BIGARROW && MSC_ARROW_OK_FOR_BIG_ARROWS(EArrowType(i))) || 
                          (t==ARROW && MSC_ARROW_OK_FOR_ARROWS(EArrowType(i))))
                csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE) + EnumEncapsulator<EArrowType>::names[i],
                               EnumEncapsulator<EArrowType>::descriptions[i],
                               EHintType::ATTR_VALUE, true, 
                               t==BIGARROW ? CshHintGraphicCallbackForBigArrows : CshHintGraphicCallbackForArrowTypes,
                               CshHintGraphicParam(i)));
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "xmul")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable()+"<multiplier between [0.1 and 10]>",
            "Specify a number how much wider you want the arrowhead.",
            EHintType::ATTR_VALUE, false));
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "ymul")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable()+"<multiplier between [0.1 and 10]>",
            "Specify a number how much higher you want the arrowhead.",
            EHintType::ATTR_VALUE, false));
        return true;
    }
    if (CaseInsensitiveEqual(attr, "arrowsize") ||
        CaseInsensitiveEndsWith(attr, "size")) {
        csh.AddToHints(EnumEncapsulator<EArrowSize>::names, EnumEncapsulator<EArrowSize>::descriptions,
                       csh.HintPrefix(COLOR_ATTRVALUE), 
                       EHintType::ATTR_VALUE, CshHintGraphicCallbackForArrowSizes);
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "color") ||
        CaseInsensitiveEndsWith(attr, "line.type") ||
        CaseInsensitiveEndsWith(attr, "line.width")) {
                return LineAttr::AttributeValues(attr, csh);
    }
    return false;

}

EArrowType ArrowHead::GetType(bool bidir, EArrowEnd which) const
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
    case MSC_ARROW_SKIP:
        if (midType.first) return skipType.second;
        else return MSC_ARROW_NONE;
    case MSC_ARROW_START:
        if (startType.first) return startType.second;
        else return MSC_ARROW_NONE;
    }
    return MSC_ARROW_NONE;
}

/** Return the type of arrowhead at the end specified by `which`.
 * This considers a whole arrow, including the two sides of middle arrowheads.
 * Say for 'a<->b<->c' we actually draw two triangles around 'b'.
 * @param [in] forward True if arrow points from left to right, that is a->b and not a<-b.
 * @param [in] left True if we are interested in the arrow to draw to the left side of the entity line.
 *                  For symmetric types (dot) this gives the same irrespective of `left`.
 * @param [in] bidir True if the arrow is bi-directional, that is like a<->b and not a->b.
 * @param [in] which Tells us which end are we interested in (or middle).
 * @return The arrowhead type to use. Can return MSC_ARROW_INVALID for start and end arrows (outside part).*/
EArrowType ArrowHead::GetType(bool forward, bool bidir, EArrowEnd which, bool left) const
{
    EArrowType ret = GetType(bidir, which);
    if (MSC_ARROW_IS_SYMMETRIC(ret)) return ret;
    switch (which) {
    case MSC_ARROW_START: 
        return forward == left ? MSC_ARROW_INVALID : ret;
    case MSC_ARROW_MIDDLE:
        if (bidir) return ret;
        return forward == left ? ret : MSC_ARROW_NONE;
    case MSC_ARROW_SKIP:
        if (bidir) return ret;
        return forward == left ? ret : MSC_ARROW_NONE;
    case MSC_ARROW_END:
        return forward == left ? ret : MSC_ARROW_INVALID;
    }
    _ASSERT(0);
    return MSC_ARROW_NONE;
}


//Transformation functions for angled arrows
//In the transformed space (sx,y) will be the same, the space will be rotated by "radian"
//and the arrow will be vertical

//Transformation functions for angled arrows
//"radian" is assumed to be between -45..+45 degrees (but in radian)

/** Transform the space for angled arrows. 
 * Useful, so that we can draw angled arrows as if they were horizontal.
 * Transform space such that if we draw a horizontal arrow, it will be of 
 * angle="radian" on the canvas. 
 * In the transformed space (sx,y) will be the same, the space will be rotated by "radian".
 * @param [in] degree The angle to rotate clockwise (with y growing downwards. 
 *                    Assumed to be between -45..+45 degrees.
 * @param [in] sx The x coordinate of the center of the rotation.
 * @param [in] y The y coordinate of the center of the rotation.
 * @param canvas The canvas to rotate.
 *
 * Saves cairo context, always call ArrowHead::UntransformCanvas() to kill it, 
 * when done.*/
void ArrowHead::TransformCanvasForAngle(double degree, Canvas &canvas, double sx, double y) const
{
    cairo_t *cr = canvas.GetContext();
    cairo_save(cr);
    cairo_translate(cr, sx, y);
    cairo_rotate(cr, degree*M_PI/180.);
    cairo_translate(cr, -sx, -y);
}

/** Rotates back the canvas rotated by ArrowHead::TransformCanvasForAngle()*/
void ArrowHead::UnTransformCanvas(Canvas &canvas) const 
{
    canvas.UnClip();
}

XY ArrowHead::getWidthHeight(bool bidir, EArrowEnd which, const LineAttr &mainline_left, const LineAttr &mainline_right) const
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

    case MSC_ARROW_JUMPOVER:
        xy.x = xy.y = baseDotSize*0.66*sizePercentage/100 + 
            std::max(mainline_left.LineWidth(), mainline_right.LineWidth());
        break;
    default:
        _ASSERT(0);
    }
    if (xmul.first) xy.x *= xmul.second;
    if (ymul.first) xy.y *= ymul.second;
    return xy;
}

double ArrowHead::getTriWidth(bool bidir, EArrowEnd which, const LineAttr &mainline_left, const LineAttr &mainline_right) const
{
    const double w = getWidthHeight(bidir, which, mainline_left, mainline_right).x;
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

/** Tells how much of the arrow line is covered by the arrowhead (on both sides of the entity line). 
 * Used for text margin calculation. 
 * @param [in] forward True if arrow points from left to right, that is a->b and not a<-b.
 * @param [in] bidir True if the arrow is bi-directional, that is like a<->b and not a->b.
 * @param [in] which Tells us which end are we interested in (or middle).
 * @param [in] mainline_left The line style of the arrow line on the left side.
 * @param [in] mainline_right The line style of the arrow line on the right side.
 * @return `first` is the extent on the left side of the entity line, `second` is on the right side. */
DoublePair ArrowHead::getWidths(bool forward, bool bidir, EArrowEnd which,
                                const LineAttr &mainline_left, const LineAttr &mainline_right) const
{
    DoublePair ret(0,0);
    const EArrowType t = GetType(bidir, which);
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
        ret.first = getWidthHeight(bidir, which, mainline_left, mainline_right).x;
        //Now see if we put the value to the right of first/second
        if (bidir && (which==MSC_ARROW_MIDDLE || which==MSC_ARROW_SKIP))
            ret.second = ret.first;
        else if ( (!forward) ^ (which==MSC_ARROW_START)) //if reverse or start, but not if both
            ret.swap();
        break;

    case MSC_ARROW_DIAMOND:
    case MSC_ARROW_DIAMOND_EMPTY:
    case MSC_ARROW_DOT:
    case MSC_ARROW_DOT_EMPTY:
    default:
        ret.second = ret.first = getWidthHeight(bidir, which, mainline_left, mainline_right).x;
        break;
    case MSC_ARROW_JUMPOVER:
        ret.second = ret.first = getWidthHeight(bidir, which, mainline_left, mainline_right).x;
        ret.first += mainline_left.LineWidth();
        ret.second += mainline_right.LineWidth();
        break;
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

/** Return the area for which the entity line shall not be drawn.
 * Only dots and diamonds cover it, for others an empty Contour is returned.
 * @param [in] xy The tip of the arrowhead.
 * @param [in] bidir If the arrowhead is bidirectional
 * @param [in] which Which end of the arrow (or middle).
 * @param [in] mainline_left The line style of the arrow line on the left side.
 * @param [in] mainline_right The line style of the arrow line on the right side.
 * @returns The contour of the dot or diamond.*/
Contour ArrowHead::EntityLineCover(XY xy, bool /*forward*/, bool bidir, EArrowEnd which,
                                   const LineAttr &mainline_left, const LineAttr &mainline_right) const
{
    XY wh = getWidthHeight(bidir, which, mainline_left, mainline_right);
    Contour ret;
    switch(GetType(bidir, which)) {
    case MSC_ARROW_DIAMOND_EMPTY:
    //case MSC_ARROW_DIAMOND:
        ret = diamond(xy, wh);
        break;

    case MSC_ARROW_DOT_EMPTY:
    //case MSC_ARROW_DOT:
        ret = Contour(xy, wh.x, wh.y);
        break;
    default:
        break; //No other arrow type covers the entity line
    }
    return ret;
}

/** Returns an area which will be used to clip the arrow line.
 * E.g., for empty dots this is a big block, with a hole in it 
 * => line will be not drawn in the hole.
 * @param [in] xy The tip of the arrowhead.
 * @param [in] bidir If the arrowhead is bidirectional
 * @param [in] which Which end of the arrow (or middle).
 * @param [in] forward True if arrow points from left to right, that is a->b and not a<-b.
 * @param [in] act_size Zero if the entity is not active at this point. Else it contains the 
 *                      half-width of the activated entity line.
 * @param [in] total A block used as the basis, it is guaranteed that the whole line will fit 
                     into this.
 * @param [in] mainline_left The line style of the arrow line on the left side.
 * @param [in] mainline_right The line style of the arrow line on the right side.
 * @returns An area covering all of `total` except where the arrowhead is drawn.
 *          If no clipping is needed (all of the line can be shown at this arrowhead, 
 *          we return `total`.*/
Contour ArrowHead::ClipForLine(XY xy, double act_size, bool forward, bool bidir, EArrowEnd which, const Block &total,
    const LineAttr &mainline_left, const LineAttr &mainline_right) const
{
    XY act(act_size, 0);
    XY wh = getWidthHeight(bidir, which, mainline_left, mainline_right);
    double w = getTriWidth(bidir, which, mainline_left, mainline_right);
    if (bidir && which == MSC_ARROW_START)
        forward = !forward;
    if (forward) {
        wh.x *= -1;
        w *= -1;
        act.x *= -1;
    }
    std::swap(wh.x, w);
    //wh now contains the triangle size, w the size of the entire stuff (same for all,but DOUBLE & TRIPLE)

    FillAttr fill(line.color.second);
    Contour tri1(xy+act+wh, xy+act, xy+act + wh - XY(0, wh.y) - XY(0, wh.y));
    Contour tri2(xy-act-wh, xy-act, xy-act - wh + XY(0, wh.y) + XY(0, wh.y));
    const Contour tri_sharp1(xy+act+wh, xy+act+XY(wh.x*SHARP_MUL_2, 0), xy+act + wh - XY(0, wh.y) - XY(0, wh.y));
    const Contour tri_sharp2(xy-act-wh, xy-act-XY(wh.x*SHARP_MUL_2, 0), xy-act - wh + XY(0, wh.y) + XY(0, wh.y));
    Range r;  //skip this x range (area can contain parts though)
    Contour area;
    XY half_offset(0,0);

    switch(GetType(bidir, which)) {
    default:
        _ASSERT(0);
        //Fallthrough to kill "uninitialized r" warnings
    case MSC_ARROW_NONE:
        r.from = xy.x+act_size;
        r.till = xy.x-act_size;
        break;

    case MSC_ARROW_SHARP:
    case MSC_ARROW_SHARP_EMPTY:
        area = tri_sharp1;
        if (bidir && (which==MSC_ARROW_MIDDLE || which==MSC_ARROW_SKIP)) 
            area += tri_sharp2;
        /* Fallthrough for calculating r */
    case MSC_ARROW_SOLID: /* Filled */
    case MSC_ARROW_EMPTY: /* Non-Filled */
    case MSC_ARROW_DOUBLE:
    case MSC_ARROW_DOUBLE_EMPTY:
    case MSC_ARROW_TRIPLE:
    case MSC_ARROW_TRIPLE_EMPTY:
        r.from = xy.x + act.x + w ;
        r.till = (bidir && (which==MSC_ARROW_MIDDLE || which==MSC_ARROW_SKIP)) ? 
            xy.x - act.x - w : xy.x - act.x;
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
        if (bidir && (which==MSC_ARROW_MIDDLE || which==MSC_ARROW_SKIP)) area += tri2;
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
        if (bidir && (which==MSC_ARROW_MIDDLE || which==MSC_ARROW_SKIP))
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
        area = Contour(total) - diamond(xy, wh);
        r = total.x;
        break;

    case MSC_ARROW_DOT:
    case MSC_ARROW_DOT_EMPTY:
        area = Contour(total) - Contour(xy, fabs(wh.x), fabs(wh.y));
        r = total.x;
        break;

    case MSC_ARROW_JUMPOVER:
        r.from = xy.x-fabs(wh.x)-mainline_left.LineWidth()/2;
        r.till = xy.x+fabs(wh.x)+mainline_right.LineWidth()/2;
        break;
    }
    //now expand returned area to cover from total.x.from to total.x.till,
    //assuming "area" now covers from r.from till r.till.
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

/** Returns a contour covering the arrowhead.
 * @param [in] xy The tip of the arrowhead.
 * @param [in] bidir If the arrowhead is bidirectional
 * @param [in] which Which end of the arrow (or middle).
 * @param [in] forward True if arrow points from left to right, that is a->b and not a<-b.
 * @param [in] act_size Zero if the entity is not active at this point. Else it contains the 
 *                      half-width of the activated entity line.
 * @returns the contour of the arrowhead.*/
Contour ArrowHead::Cover(XY xy, double act_size, bool forward, bool bidir, EArrowEnd which,
                         const LineAttr &mainline_left, const LineAttr &mainline_right) const
{
    XY act(act_size, 0);
    XY wh = getWidthHeight(bidir, which, mainline_left, mainline_right);
    double w = getTriWidth(bidir, which, mainline_left, mainline_right);
    if (bidir && which == MSC_ARROW_START)
        forward = !forward;
    if (forward) {
        wh.x *= -1;
        w *= -1;
        act.x *= -1;
    }
    std::swap(wh.x, w);
    //wh now contains the triangle size, w the size of the entire stuff (same for all,but DOUBLE & TRIPLE)

    FillAttr fill(line.color.second);
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
        if (bidir && (which==MSC_ARROW_MIDDLE || which==MSC_ARROW_SKIP)) 
            area += tri2.CreateShifted(XY(-wh.x*2, 0));
        /*Fallthrough*/
    case MSC_ARROW_DOUBLE:
    case MSC_ARROW_DOUBLE_EMPTY:
        area += tri1.CreateShifted(XY(wh.x, 0));
        if (bidir && (which==MSC_ARROW_MIDDLE || which==MSC_ARROW_SKIP)) 
            area += tri2.CreateShifted(XY(-wh.x, 0));
        /*Fallthrough*/
    case MSC_ARROW_SOLID: /* Filled */
    case MSC_ARROW_EMPTY: /* Non-Filled */
    case MSC_ARROW_LINE: /* Two lines */
    case MSC_ARROW_HALF: /* Unfilled half */
        area += tri1;
        if (bidir && (which==MSC_ARROW_MIDDLE || which==MSC_ARROW_SKIP))
            area += tri2;
        break;

    case MSC_ARROW_TRIPLE_LINE:
    case MSC_ARROW_DOUBLE_LINE:
    case MSC_ARROW_DOUBLE_HALF:
    case MSC_ARROW_TRIPLE_HALF:
        area += Contour(xy.x+act.x+w, xy.x+act.x+wh.x, xy.y-wh.y, xy.y+wh.y) + tri1;
        if (bidir && (which==MSC_ARROW_MIDDLE || which==MSC_ARROW_SKIP))
            area += Contour(xy.x-act.x-wh.x, xy.x-act.x-w, xy.y-wh.y, xy.y+wh.y) + tri2;
        break;

    case MSC_ARROW_SHARP:
    case MSC_ARROW_SHARP_EMPTY:
        area = tri1 - tri_sharp1;
        if (bidir && (which==MSC_ARROW_MIDDLE || which==MSC_ARROW_SKIP)) 
            area += tri2 - tri_sharp2;
        break;

    case MSC_ARROW_DIAMOND:
    case MSC_ARROW_DIAMOND_EMPTY:
        area = diamond(xy, wh);
        break;

    case MSC_ARROW_DOT:
    case MSC_ARROW_DOT_EMPTY:
        area = Contour(xy, fabs(wh.x), fabs(wh.y));
        break;
    
    case MSC_ARROW_JUMPOVER:
        area = Contour(xy, fabs(wh.x), fabs(wh.y)) * Block(xy.x-wh.x*2, xy.x+wh.x*2, xy.y-wh.y*2, xy.y);
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
 * @param [in] xy The tip of the arrowhead.
 * @param [in] bidir If the arrowhead is bidirectional
 * @param [in] which Which end of the arrow (or middle).
 * @param [in] forward True if arrow points from left to right, that is a->b and not a<-b.
 * @param [in] act_size Zero if the entity is not active at this point. Else it contains the 
 *                      half-width of the activated entity line.
 * @param [in] mainline_left The line style of the arrow line on the left side.
 * @param [in] mainline_right The line style of the arrow line on the right side.
 * @param canvas The canvas onto which we draw.
*/
void ArrowHead::Draw(XY xy, double act_size, bool forward, bool bidir, EArrowEnd which, 
                     const LineAttr &mainline_left, const LineAttr &mainline_right, 
                     Canvas *canvas) const
{
    const EArrowType arrow_type = GetType(bidir, which);
    if (arrow_type == MSC_ARROW_NONE) return;

    XY act(act_size, 0);
    XY wh = getWidthHeight(bidir, which, mainline_left, mainline_right);
    double w = getTriWidth(bidir, which, mainline_left, mainline_right);
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
        const FillAttr fill(line.color.second, GRADIENT_NONE);
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
    //Do jumpover
    if (arrow_type == MSC_ARROW_JUMPOVER) {
        const Contour cover = Contour(xy, fabs(wh.x), fabs(wh.y)) + Block(xy.x-wh.x*2-lw2*2, xy.x+wh.x*2+lw2*2, xy.y, xy.y+wh.y*2+lw2*10);
        if (mainline_left == mainline_right) {
            const double w = fabs(wh.x) + mainline_left.LineWidth()/2;
            canvas->Clip(Block(xy.x-w, xy.x+w, 0, xy.y+mainline_left.LineWidth()/2));
            canvas->Line(cover, mainline_left);
            canvas->UnClip();
        } else {
            double w = fabs(wh.x) + mainline_left.LineWidth()/2;
            canvas->Clip(Block(xy.x-w, xy.x, 0, xy.y+mainline_left.LineWidth()/2));
            canvas->Line(cover, mainline_left);
            canvas->UnClip();
            w = fabs(wh.x) + mainline_right.LineWidth()/2;
            canvas->Clip(Block(xy.x, xy.x+w, 0, xy.y+mainline_right.LineWidth()/2));
            canvas->Line(cover, mainline_right);
            canvas->UnClip();
        }
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
        Block ext1(canvas->GetSize());
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
        if (bidir && (which==MSC_ARROW_MIDDLE || which==MSC_ARROW_SKIP)) 
            canvas->Line(tri2.CreateShifted(XY(-wh.x, 0)), line);
        /*Fallthrough*/
    case MSC_ARROW_DOUBLE_LINE:
    case MSC_ARROW_DOUBLE_HALF:
        canvas->Line(tri1.CreateShifted(XY(wh.x/2, 0)), line);
        if (bidir && (which==MSC_ARROW_MIDDLE || which==MSC_ARROW_SKIP)) 
            canvas->Line(tri2.CreateShifted(XY(-wh.x/2, 0)), line);
        /*Fallthrough*/
        if (arrow_type == MSC_ARROW_DOUBLE_HALF || arrow_type == MSC_ARROW_TRIPLE_HALF)
            canvas->UnClip();
    case MSC_ARROW_LINE: /* Two lines */
    case MSC_ARROW_HALF: /* Unfilled half */
        if (MSC_ARROW_IS_HALF(arrow_type)) {
            Block ext(canvas->GetSize());
            ext.y.from = xy.y;
            canvas->Clip(ext);
        }
        canvas->Line(tri1, line);
        if (bidir && (which==MSC_ARROW_MIDDLE || which==MSC_ARROW_SKIP)) 
            canvas->Line(tri2, line);
        break;
    default:
        _ASSERT(0);
    }
    if (MSC_ARROW_IS_HALF(arrow_type)) 
        canvas->UnClip();
}

//For largely internal use - characteristic size of the big arrowheads
XY ArrowHead::getBigWidthHeight(EArrowType type, const LineAttr &ltype) const
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

/** The full width of the arrowhead (on both sides of the entity line). 
 * The values returned here are used to determine spacing between entities
 * in for ArcBigArrow::Width().
 * @param [in] forward True if arrow points from left to right, that is a->b and not a<-b.
 * @param [in] bidir True if the arrow is bi-directional, that is like a<->b and not a->b.
 * @param [in] which Tells us which end are we interested in (or middle).
 * @param [in] act_size The (half) width of the entity line due to entity activation.
 * @param [in] ltype The line style that will be used to draw the block arrow.
 * @returns The size of the arrowhead from the middle of the entity line towards the 
 *          middle on the arrow. 'first' returns the size on the left side, 
 *          'second' returns the space on the right side.*/
DoublePair ArrowHead::getBigWidthsForSpace(bool forward, bool bidir, EArrowEnd which,
                                           double /*body_height*/, double act_size, const LineAttr &ltype) const
{
    DoublePair ret(0, 0);
    EArrowType type = GetType(bidir, which);
    switch(type) {
    case MSC_ARROW_DIAMOND_EMPTY: /* Half diamond at ends nothing in the middle.*/
    case MSC_ARROW_DOT_EMPTY:     /* Half circle at ends nothing in the middle.*/
        if (which==MSC_ARROW_START || which==MSC_ARROW_END) {
            ret.first = act_size+getBigWidthHeight(type, ltype).x;
            if ((which==MSC_ARROW_END) == forward) //only on the right side if (-> and END) or (<- and START)
                ret.swap();
        }
        //fallthrough
    case MSC_ARROW_NONE:
    default:
        break;

    case MSC_ARROW_DOT:          /* Full diamond at ends and at middle. Independent of act_size*/
    case MSC_ARROW_DIAMOND:
        ret.first = ret.second = getBigWidthHeight(type, ltype).x;
        break;

    case MSC_ARROW_STRIPES:
    case MSC_ARROW_TRIANGLE_STRIPES:
        ret.first = ltype.LineWidth();
        //fallthrough
    case MSC_ARROW_SOLID: /* Normal triangle */
    case MSC_ARROW_LINE: /* Normal Triangle */
    case MSC_ARROW_SHARP: /* Sharp triangle */
    case MSC_ARROW_SHARP_EMPTY: /* Small triangle, no "serifs" */
    case MSC_ARROW_EMPTY: /* Small triangle, no "serifs" */
    case MSC_ARROW_EMPTY_INV: /* Inverse small triangle */
    case MSC_ARROW_HALF: /* Half triangle */
        ret.first += act_size+getBigWidthHeight(type, ltype).x;
        if (bidir && (which==MSC_ARROW_MIDDLE || which==MSC_ARROW_SKIP))
            ret.second = ret.first;
        else if ((which==MSC_ARROW_START && forward) || //only on the left side if (-> and END) or (<- and START)
                 (which==MSC_ARROW_END && !forward))
            ret.swap();
    }
    return ret;
}

/** Determines how much margin is needed for a text with a given cover.
 * Determines the margin of the text (from the entity line). Can be negative, 
 * if the block arrowhead spans over the entity line (e.g., dot does) and
 * hence the text can be wider than the space between the entity lines.
 * (sx, sy) should be equal to the y aspect of the bounding box of text_cover
 * plus two linewidths.
 *
 * @param [in] text_cover The cover of the label.
 * @param [in] sy The top of the arrow body (outer edge of the line).
 * @param [in] dy The bottom of the arrow body (outer edge of the line).
 * @param [in] margin_side_is_left If we need the left or right margin for the text.
 * @param [in] type The type of the arrowhead we have on the side of the text pointed by 
 *                  `margin_side_is_left`. This type will be either a segmenting 
 *                  (non-symmetric) arrowhead or the start/end head.
 * @param [in] ltype The line style that will be used to draw the block arrow.
 * @param [in] bidir True if bi-directional arrow.
 */
double ArrowHead::getBigMargin(Contour text_cover, double sy, double dy, bool margin_side_is_left, 
                               bool bidir, EArrowType type, const LineAttr &ltype) const
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

/** Tells how much the arrow (overall) extends above or below the body.
 * @param [in] forward True if arrow points from left to right, that is a->b and not a<-b.
 * @param [in] bidir True if the arrow is bi-directional, that is like a<->b and not a->b.
 * @param [in] lines Specifies the line style for each segment.
 *                   If NULL, we assume there are no middle arrowheads (arrow is of a 
 *                   single segment) and `this->line` is the line type.
 *                   Else we use the linewidths in 'lines' to estimate top height, 
 *                   except if mid arrowhead is not segmenting.
 *                   We assume `lines` is ordered in increasing x coordinate. */
double ArrowHead::bigYExtent(bool bidir, bool forward, const std::vector<LineAttr> *lines) const
{
    if (lines==NULL || lines->size()==0 || MSC_ARROW_IS_SYMMETRIC(midType.second)) {
        const double h = std::max(getBigWidthHeight(startType.second, line).y, getBigWidthHeight(endType.second, line).y);
        if (lines==NULL || lines->size()==0) return h;
        return std::max(h, getBigWidthHeight(midType.second, line).y);
    }
    double ret = 0;
    for (unsigned i=0; i<lines->size(); i++) {
        EArrowEnd t_left = MSC_ARROW_MIDDLE, t_right = MSC_ARROW_MIDDLE;
        if (i==0) 
            t_left = forward ? MSC_ARROW_START : MSC_ARROW_END;
        if (i==lines->size()) 
            t_right = forward ? MSC_ARROW_END : MSC_ARROW_START;
        
        const EArrowType tt_left = GetType(forward, bidir, t_left, false);
        const EArrowType tt_right = GetType(forward, bidir, t_right, true);

        const double h_left = getBigWidthHeight(tt_left, lines->at(i)).y;
        const double h_right = getBigWidthHeight(tt_right, lines->at(i)).y;

        ret = std::max(ret, std::max(h_left, h_right));
    }
    return ret;
}


/** Returns the contours of one arrowhead. 
 * Either on the left or the right side of the entity line.
 * @param [in] x The x coordinate of the tip of the arrowhead.
 * @param [in] act_size Zero if the entity is not active at this point. Else it contains the 
 *                      half-width of the activated entity line.
 * @param [in] sy The top of the arrow body (outer edge of the line).
 * @param [in] dy The bottom of the arrow body (outer edge of the line).
 * @param [in] bidir If the arrowhead is bidirectional
 * @param [in] type The type of arrowhead to draw.
 * @param [in] which Which end of the arrow (or middle).
 * @param [in] left True if we are interested in the arrow to draw to the left side of the entity line.
 *                  For symmetric types (dot) this gives the same irrespective of `left`.
 * @param [in] ltype The line style that will be used to draw the block arrow.
 * @param [out] body_margin The space from the entity line the body shoud end at.
 *                          Essentially the width of the arrowhead.
 * @returns The contour of the arrowhead.
 */
Contour ArrowHead::BigContourOneEntity(double x, double act_size, double sy, double dy, bool /*bidir*/,
                                       EArrowType type, EArrowEnd which, const LineAttr &ltype, 
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
        //if (which == MSC_ARROW_MIDDLE && !bidir) 
        //    area += Contour(x-x_act, mid_y, x-x_act-x_off, sy, x-x_act-x_off, dy);
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

/** Returns the outer line of the block arrow, each segment in a separate Contour.
 * If mid-arrow does not segment only one Contour is returned
 * @param [in] xPos The x coordinate of entities the arrow visits. If the arrow is
 *                  slanted, these shall be spaced somewhat wider apart than in the
 *                  horizontal direction. This must be done by the caller.
 * @param [in] act_size The activation status for each visited entity. 
 *                      Zero if the entity is not active at this point. Else it contains the 
 *                      half-width of the activated entity line.
 * @param [in] sy The top of the arrow body (outer edge of the line).
 * @param [in] dy The bottom of the arrow body (outer edge of the line).
 * @param [in] forward True if arrow points from left to right, that is a->b and not a<-b.
 * @param [in] bidir If the arrowhead is bidirectional
 * @param [in] lines Specifies the line style for each segment.
 *                   If NULL, we assume there are no middle arrowheads (arrow is of a 
 *                   single segment) and `this->line` is the line type.
 *                   We assume `lines` is ordered in increasing x coordinate. 
 * @param [out] result The contours for individual segments. Only one if there is
 *                     only one segment.
 * @returns The union of segment contours.*/ 
Contour ArrowHead::BigContour(const std::vector<double> &xPos, const std::vector<double> &act_size, 
                              double sy, double dy, bool forward, bool bidir, 
                              const std::vector<LineAttr> *lines,
                              std::vector<Contour> &result) const
{
    Contour area_overall, area_segment;
    const bool segment = !MSC_ARROW_IS_SYMMETRIC(GetType(bidir,MSC_ARROW_MIDDLE)) && xPos.size()>2;
    const EArrowEnd e_left  = forward ? MSC_ARROW_START : MSC_ARROW_END;
    const EArrowType t_left_end  = GetType(bidir, e_left);
    const LineAttr *ltype_current = (segment && lines) ? &*lines->begin() : &line;

    //draw leftmost arrowhead
    double from_x;
    area_segment = BigContourOneEntity(xPos.front(), act_size.front(), sy, dy, bidir, 
                                       t_left_end, e_left, *ltype_current, false, &from_x);
    from_x = xPos[0] + from_x;

    //set up variables for mid-points
    if (midType.second != MSC_ARROW_NONE)
        for (unsigned i=1; i<xPos.size()-1; i++) {
            EArrowType type = GetType(forward, bidir, MSC_ARROW_MIDDLE, true);
            //draw left side of the entity line
            double margin; 
            area_segment += BigContourOneEntity(xPos[i], act_size[i], sy, dy, bidir, 
                                                type, MSC_ARROW_MIDDLE, *ltype_current, true, &margin);
            //draw body that connects them
            area_segment += Block(from_x, xPos[i] - margin, sy, dy);

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
                                                   type, MSC_ARROW_MIDDLE, *ltype_current, false, &margin);
            }
            from_x = xPos[i] + margin;
    }
    //draw rightmost arrowhead
    const EArrowEnd e_right = forward ? MSC_ARROW_END : MSC_ARROW_START;
    const EArrowType t_right_end = GetType(bidir, e_right);
    double right_margin;
    area_segment += BigContourOneEntity(xPos.back(), act_size.back(), sy, dy, bidir, 
                                        t_right_end, e_right, *ltype_current, true, &right_margin);
    area_segment += Block(from_x, xPos.back() - right_margin, sy, dy);
    area_overall += area_segment;
    if (segment) 
        result.push_back(std::move(area_segment));
    else
        result.push_back(area_overall);
    return area_overall;
}

/** Returns the outer line of the heads of the block arrow, but not of the body.
 * If the two ends are ARROW_NONE, a small block is added.
 * This is used to place float notes.
 * @param [in] xPos The x coordinate of entities the arrow visits.
 * @param [in] act_size The activation status for each visited entity. 
 *                      Zero if the entity is not active at this point. Else it contains the 
 *                      half-width of the activated entity line.
 * @param [in] sy The top of the arrow body (outer edge of the line).
 * @param [in] dy The bottom of the arrow body (outer edge of the line).
 * @param [in] forward True if arrow points from left to right, that is a->b and not a<-b.
 * @param [in] bidir If the arrowhead is bidirectional
 * @param [in] lines Specifies the line style for each segment.
 *                   If NULL, we assume there are no middle arrowheads (arrow is of a 
 *                   single segment) and `this->line` is the line type.
 *                   We assume `lines` is ordered in increasing x coordinate. 
 * @param [in] compressGap How far we shall be.
 * @returns The contours of the heads, but no body.
 */
Contour ArrowHead::BigHeadContour(const std::vector<double> &xPos, const std::vector<double> &act_size, 
                                  double sy, double dy, bool forward, bool bidir, 
                                  const std::vector<LineAttr> *lines, double compressGap) const
{
    Contour ret;
    const bool segment = !MSC_ARROW_IS_SYMMETRIC(GetType(bidir,MSC_ARROW_MIDDLE)) && xPos.size()>2;
    const EArrowEnd e_left  = forward ? MSC_ARROW_START : MSC_ARROW_END;
    const EArrowType t_left_end  = GetType(bidir, e_left);
    const LineAttr *ltype_current = (segment && lines) ? &*lines->begin() : &line;

    //add leftmost arrowhead
    double dummy_margin;
    if (t_left_end == MSC_ARROW_NONE)
        ret = Block(xPos[0]+act_size[0]-compressGap/2, xPos[0]+act_size[0]+compressGap/2, sy, dy);
    else 
        ret = BigContourOneEntity(xPos[0], act_size[0], sy, dy, bidir, 
                                  t_left_end, e_left, *ltype_current, false, &dummy_margin);

    //add mid-heads
    if (midType.second != MSC_ARROW_NONE)
        for (unsigned i=1; i<xPos.size()-1; i++) {
            //draw left side of the entity line
            ret += BigContourOneEntity(xPos[i], act_size[i], sy, dy, bidir, 
                                       GetType(forward, bidir, MSC_ARROW_MIDDLE, true),
                                       MSC_ARROW_MIDDLE, *ltype_current, true, &dummy_margin);
            //calculate next linewidth
            if (segment && lines && lines->size()>i)
                ltype_current  = &lines->at(i);

            //if segmented, draw arrowhead at right side of entity line
            if (segment) 
                ret += BigContourOneEntity(xPos[i], act_size[i], sy, dy, bidir, 
                                           GetType(forward, bidir, MSC_ARROW_MIDDLE, false),
                                           MSC_ARROW_MIDDLE, *ltype_current, false, &dummy_margin);
    }
    //draw rightmost arrowhead
    const EArrowEnd e_right = forward ? MSC_ARROW_END : MSC_ARROW_START;
    const EArrowType t_right_end = GetType(bidir, e_right);
    if (t_right_end == MSC_ARROW_NONE)
        ret += Block(*xPos.rbegin()+ *act_size.rbegin()-compressGap/2, *xPos.rbegin()+*act_size.rbegin()+compressGap/2, sy, dy);
    else 
        ret += BigContourOneEntity(*xPos.rbegin(), *act_size.rbegin(), sy, dy, bidir, 
                                   t_right_end, e_right, *ltype_current, true, &dummy_margin);
    return ret;
}

/** Draw the block arrow, assuming we already calculated its contour.
 * 
 * @param [out] result The contours for individual segments. Only one if there is
 *                     only one segment.
 * @param [in] lines Specifies the line style for each segment.
 *                   If NULL, we assume there are no middle arrowheads (arrow is of a 
 *                   single segment) and `this->line` is the line type.
 *                   We assume `lines` is ordered in increasing x coordinate. 
 * @param [in] fill The fill we shall use inside the block arrow.
 * @param [in] shadow The shadow we shall use below the block arrow.
 * @param canvas The canvas to draw on.
 * @param [in] angle_radian Our slant. We assume the space around us is rotated 
                            this much when we are called. We also assume 
                            the contours are enlengthtened already. This value
                            is used to adjust the shadow to be in the right dir. */
void ArrowHead::BigDrawFromContour(std::vector<Contour> &result, const std::vector<LineAttr> *lines,
                 const FillAttr &fill, const ShadowAttr &shadow, Canvas &canvas,
                 double angle_radian) const
{
    
    for (unsigned i=0; i<result.size(); i++) {
        const LineAttr &l = lines ? lines->at(i) : line;
        const Contour midline = result[i].CreateExpand(-l.LineWidth()/2);
        canvas.Shadow(midline, shadow, angle_radian);
        canvas.Fill  (midline.CreateExpand(-l.Spacing()), fill); 
        canvas.Line  (midline, l);
    }
}

/** I have no clue right now, why this exists.
 * Here are my notes: 
 * If we have empty_dot or empty_diamond middle arrow types, draw them in addition
 * ..but clip the drawing area, so that text is not disturbed*/
void ArrowHead::BigDrawEmptyMid(const std::vector<double> &xPos, double sy, double dy, 
                                Canvas &canvas, const Contour *clip) const
{
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

/** Calculate the geometry of an arrow and draw it on a canvas.
 * @param [in] xPos The x coordinate of entities the arrow visits. If the arrow is
 *                  slanted, these shall be spaced somewhat wider apart than in the
 *                  horizontal direction. This must be done by the caller.
 * @param [in] act_size The activation status for each visited entity. 
 *                      Zero if the entity is not active at this point. Else it contains the 
 *                      half-width of the activated entity line.
 * @param [in] sy The top of the arrow body (outer edge of the line).
 * @param [in] dy The bottom of the arrow body (outer edge of the line).
 * @param [in] forward True if arrow points from left to right, that is a->b and not a<-b.
 * @param [in] bidir If the arrowhead is bidirectional
 * @param [in] fill The fill we shall use inside the block arrow.
 * @param [in] shadow The shadow we shall use below the block arrow.
 * @param canvas The canvas to draw on.
 * @param [in] clip Some clip value, not sure what.
 * @param [in] angle_radian Our slant. We assume the space around us is rotated 
                            this much when we are called. We also assume 
                            the contours are enlengthtened already. This value
                            is used to adjust the shadow to be in the right dir. */
void ArrowHead::BigCalculateAndDraw(const std::vector<double> &xPos, const std::vector<double> &act_size, 
                                    double sy, double dy, bool forward, bool bidir,
                                    const FillAttr &fill, const ShadowAttr &shadow, Canvas &canvas,
                                    const Contour *clip, double angle_radian) const
{
    std::vector<Contour> cont;
    BigContour(xPos, act_size, sy, dy, forward, bidir, NULL, cont);
    BigDrawFromContour(cont, NULL, fill, shadow, canvas, angle_radian);
    BigDrawEmptyMid(xPos, sy, dy, canvas, clip);
}
