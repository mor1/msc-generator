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
/** @file style.cpp The implementation of styles (MscStyle) and contexts (Context).
 * @ingroup libmscgen_files */


#include "msc.h"

/** Returns true if value `v` is valid for side type `t` */
bool IsValidSideValue(ESideType t, ESide v)
{
    return ((v==ESide::LEFT || v==ESide::RIGHT) && (t==ESideType::ANY || t==ESideType::LEFT_RIGHT)) ||
                                                     (v==ESide::END && t==ESideType::ANY);
}


/** Create an empty style that contains all possible attributes.*/
MscStyle::MscStyle(EStyleType tt, EColorMeaning cm) : type(tt), color_meaning(cm)
{
    f_line=f_vline=f_fill=f_vfill=f_shadow=f_text=true;
    f_solid=f_numbering=f_vspacing=f_indicator=true;
    f_makeroom=f_note=f_lost=f_lsym=true;
    f_arrow = ArrowHead::ANY;
    f_side = ESideType::ANY;
    f_shape = true;
    f_tag = true;
    Empty();
}

/** Create an empty style that contains only some of the attributes.
 * @param [in] tt The type of style instance.
 * @param [in] cm How an unqualified "color" attribute shall be interpreted.
 * @param [in] a What type of arrows we accept.
 * @param [in] t True if the style shall contain text attributes.
 * @param [in] l True if the style shall contain line attributes.
 * @param [in] f True if the style shall contain fill attributes.
 * @param [in] s True if the style shall contain shadow attributes.
 * @param [in] vl True if the style shall contain vline attributes.
 * @param [in] so True if the style shall contain the `solid` attribute.
 * @param [in] nu True if the style shall contain the `number` attribute.
 * @param [in] co True if the style shall contain the `compress`/`vspacing` attribute.
 * @param [in] si True if the style shall contain the `side` attribute.
 * @param [in] i True if the style shall contain the `indicator` attribute.
 * @param [in] vf True if the style shall contain vfill attributes.
 * @param [in] mr True if the style shall contain the `makeroom` attribute.
 * @param [in] n True if the style shall contain note attributes.
 * @param [in] lo True if the style shall contain lost.* attributes.
 * @param [in] lsym True if the style shall contain loss symbol attributes (x.*)
 * @param [in] shp True if the style shall contain entity related attributes (shape, shape.size)
 */
MscStyle::MscStyle(EStyleType tt, EColorMeaning cm, ArrowHead::EArcArrowType a, 
                   bool t, bool l, bool f, bool s, bool vl,
                   bool so, bool nu, bool co, ESideType si, bool i, bool vf, bool mr, bool n, 
                   bool lo, bool lsym, bool shp, bool tag) :
    arrow(a), lost_arrow(ArrowHead::ARROW), type(tt), color_meaning(cm),
    f_line(l), f_vline(vl), f_fill(f), f_vfill(vf), f_shadow(s),
    f_text(t), f_solid(so), f_numbering(nu), f_vspacing(co), f_side(si),
    f_indicator(i), f_makeroom(mr), f_note(n), f_arrow(a), f_lost(lo), f_lsym(lsym),
    f_shape(shp), f_tag(tag)
{
    Empty();
}

/** Make a style complete by setting the default values - but leave text & lost attributes empty.
 * Default attributes are half solid, right side, no numbering, no compress, indicator yes,
 * makeroom yes and the default of other attribute classes (specified there).
 * We skip text styles, since we have a global text attribute per chart and 
 * we set the default there. We also skip loss attributes as they do not have to be
 * fully specified.
 */
void MscStyle::MakeCompleteButText() 
{
    if (f_line) line.MakeComplete();
    else line.Empty();
    if (f_vline) vline.MakeComplete();
    else vline.Empty();
    if (f_fill) fill.MakeComplete();
    else fill.Empty();
    if (f_vfill) vfill.MakeComplete();
    else vfill.Empty();
    if (f_shadow) shadow.MakeComplete();
    else shadow.Empty();
    if (f_arrow!=ArrowHead::NONE) arrow.MakeComplete();
    else arrow.Empty();
    //text untouched
    solid.first=f_solid;
    solid.second = 128;
    side.first = f_side != ESideType::NO;  
    side.second = ESide::RIGHT;
    numbering.first = f_numbering;
    numbering.second = false;
    vspacing.first = f_vspacing;
    vspacing.second = 0;
    indicator.first = f_indicator;
    indicator.second = true;
    makeroom.first = f_makeroom;
    makeroom.second = true;
    if (f_note) note.MakeComplete();
    else note.Empty();
    //no change to lost fields - those may remain incomplete, 
    lsym_line.MakeComplete(); 
    lsym_line.color.second = ColorType(255, 0, 0);
    lsym_line.width.second = 2;
    lsym_size.first = true;
    lsym_size.second = MSC_ARROW_NORMAL;
    shape.first = true;
    shape.second = -1; //no shape
    shape_size.first = true;
    shape_size.second = MSC_ARROW_SMALL;
    tag_line.MakeComplete();
    tag_fill.MakeComplete();
    //tag_text unchanged
}

/** Make the style empty be unsetting all attributes it contains.*/
void MscStyle::Empty()
{
    line.Empty();
    vline.Empty();
    fill.Empty();
    vfill.Empty();
    text.Empty();
    arrow.Empty();
    shadow.Empty();
    solid.first = false;
    side.first = false;
    vspacing.first = false;
    numbering.first = false;
    indicator.first = false;
    makeroom.first = false;
    note.Empty();
    lost_line.Empty();
    lost_arrow.Empty();
    lost_text.Empty();
    lsym_line.Empty();
    lsym_size.first = false;
    shape.first = false;
    shape_size.first = false;
    tag_line.Empty();
    tag_fill.Empty();
    tag_text.Empty();
}

/** Merge another style to us by copying the value of those attributes which are contained by both and are set in `toadd`.*/
MscStyle & MscStyle::operator +=(const MscStyle &toadd)
{
    if (toadd.f_line && f_line) line += toadd.line;
    if (toadd.f_vline && f_vline) vline += toadd.vline;
    if (toadd.f_fill && f_fill) fill += toadd.fill;
    if (toadd.f_vfill && f_vfill) vfill += toadd.vfill;
    if (toadd.f_shadow &&f_shadow) shadow += toadd.shadow;
    if (toadd.f_text && f_text) text += toadd.text;
    if (toadd.f_arrow!=ArrowHead::NONE && f_arrow!=ArrowHead::NONE) arrow += toadd.arrow;
    if (toadd.f_solid && f_solid && toadd.solid.first) solid = toadd.solid;
    if (toadd.f_side != ESideType::NO && toadd.side.first && IsValidSideValue(f_side, toadd.side.second)) side = toadd.side;
    if (toadd.f_vspacing && f_vspacing && toadd.vspacing.first) vspacing = toadd.vspacing;
    if (toadd.f_numbering && f_numbering && toadd.numbering.first) numbering = toadd.numbering;
    if (toadd.f_indicator && f_indicator && toadd.indicator.first) indicator = toadd.indicator;
    if (toadd.f_makeroom && f_makeroom && toadd.makeroom.first) makeroom = toadd.makeroom;
    if (toadd.f_note && f_note) note += toadd.note;
    if (toadd.f_lost && f_lost) {
        lost_line += toadd.lost_line;
        lost_arrow += toadd.lost_arrow;
        lost_text += toadd.lost_text;
    }
    if (toadd.f_lsym && f_lsym) {
        lsym_line += toadd.lsym_line;
        if (toadd.lsym_size.first) lsym_size = toadd.lsym_size;
    }
    if (toadd.f_shape && f_shape) {
        if (toadd.shape.first) shape = toadd.shape;
        if (toadd.shape_size.first) shape_size = toadd.shape_size;
    }
    if (toadd.f_tag && f_tag) {
        tag_line += toadd.tag_line;
        tag_fill += toadd.tag_fill;
        tag_text += toadd.tag_text;
    }
    return *this;
}

/** Possible values for the 'side' attribute.*/
template<> const char EnumEncapsulator<ESide>::names[][ENUM_STRING_LEN] =
    {"invalid", "left", "right", "end", ""};


/** Apply an attribute to us.
 * Generate an error if the we recognize the attribute, but bad value.
 * Do not recognize attribute names that correspond to attributes we do not contain.*/
bool MscStyle::AddAttribute(const Attribute &a, Msc *msc)
{
    if (a.type == MSC_ATTR_STYLE) {
        const char *newname = a.name == "emphasis"?"box":"emptybox";
        if (a.name == "emphasis" || a.name == "emptyemphasis") {
            msc->Error.Warning(a.linenum_attr.start, "Stylename '" + a.name + "' is deprecated, using " + newname + " instead.");
            operator +=(msc->Contexts.back().styles[newname].read());
            return true;
        }
        if (msc->Contexts.back().styles.find(a.name) == msc->Contexts.back().styles.end()) {
            a.InvalidStyleError(msc->Error);
            return true;
        }
        operator +=(msc->Contexts.back().styles[a.name].read());
        return true;
    }
    if (a.Is("color")) {
        unsigned num = 0;
        switch (color_meaning) {
        case EColorMeaning::LINE_ARROW_TEXT:
            if (f_line)
                num += line.AddAttribute(a, msc, type);
            if (f_text)
                num += text.AddAttribute(a, msc, type);
            if (f_arrow!=ArrowHead::NONE)
                num += arrow.AddAttribute(a, msc, type);
            return num;
        case EColorMeaning::LINE_VLINE_TEXT:
            if (f_line)
                num += line.AddAttribute(a, msc, type);
            if (f_text)
                num += text.AddAttribute(a, msc, type);
            if (f_vline)
                num += vline.AddAttribute(a, msc, type);
            return num;
        case EColorMeaning::TEXT:
            if (f_text)
                return text.AddAttribute(a, msc, type);
            return false;
        case EColorMeaning::FILL:
            if (f_fill)
                return fill.AddAttribute(a, msc, type);
            return false;
        default:
            _ASSERT(0); //fallback
        case EColorMeaning::NOHOW:
            return false;
        }
    }
    if (a.Is("line.width")) {
        if (f_arrow!=ArrowHead::NONE) arrow.AddAttribute(a, msc, type);
        if (f_line) line.AddAttribute(a, msc, type);
        return f_arrow!=ArrowHead::NONE || f_line;
    }
    if (f_text && (a.StartsWith("text") || a.Is("ident")))
        return text.AddAttribute(a, msc, type);
    if (f_line && a.StartsWith("line"))
        return line.AddAttribute(a, msc, type);
    if (f_vline && a.StartsWith("vline"))
        return vline.AddAttribute(a, msc, type);
    if (a.StartsWith("fill") && f_fill)
        return fill.AddAttribute(a, msc, type);
    if (f_vfill && a.StartsWith("vfill"))
        return vfill.AddAttribute(a, msc, type);
    if (f_shadow && a.StartsWith("shadow"))
        return shadow.AddAttribute(a, msc, type);
    if (f_arrow!=ArrowHead::NONE && (a.StartsWith("arrow") || a.Is("arrowsize")))
        return arrow.AddAttribute(a, msc, type);
    if (f_note && a.StartsWith("note")) 
        return note.AddAttribute(a, msc, type);
    if (a.Is("solid") && f_solid) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, type))
                solid.first = false;
            return true;
        }
        if (a.type == MSC_ATTR_NUMBER && a.number>=0 && a.number <= 255) {
            solid.first = true;
            solid.second = a.number<=1 ? (unsigned char)(a.number*255) : (unsigned char)a.number;
            return true;
        }
        a.InvalidValueError("0..1' or '0..255", msc->Error);
    }
    if (a.Is("side") && f_side != ESideType::NO) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, type))
                side.first = false;
            return true;
        }
        ESide tmp_side;
        if (a.type == MSC_ATTR_STRING && Convert(a.value, tmp_side)) 
            if (IsValidSideValue(f_side, tmp_side)) {
                side.first = true;
                side.second = tmp_side;
                return true;
            }
        a.InvalidValueError(f_side == ESideType::LEFT_RIGHT ? "left/right" : "left/right/end", msc->Error);
        return true;
    }
    if (a.Is("compress") && f_vspacing) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, type))
                vspacing.first = false;
            return true;
        }
        if (!a.CheckType(MSC_ATTR_BOOL, msc->Error)) return true;
        vspacing.first = true;
        vspacing.second = a.yes ? DBL_MIN : 0;
        return true;
    }
    if (a.Is("vspacing") && f_vspacing) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, type))
                vspacing.first = false;
        } else if (a.type == MSC_ATTR_STRING && a.value == "compress") {
            vspacing.first = true;
            vspacing.second = DBL_MIN;
        } else if (a.CheckType(MSC_ATTR_NUMBER, msc->Error)) {
            vspacing.first = true;
            vspacing.second = a.number;
        }
        return true;
    }
    if (a.Is("number") && f_numbering) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, type))
                numbering.first = false;
            return true;
        }
        if (a.type != MSC_ATTR_BOOL) {
            msc->Error.Error(a, true, "The 'number' attribute must be 'yes' or 'no' for styles. Ignoring it.");
            return true;
        }
        numbering.first = true;
        numbering.second = a.yes;
        return true;
    }
    if (a.Is("indicator") && f_indicator) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, type))
                indicator.first = false;
            return true;
        }
        if (!a.CheckType(MSC_ATTR_BOOL, msc->Error)) return true;
        indicator.first = true;
        indicator.second = a.yes;
        return true;
    }
    if (a.Is("makeroom") && f_makeroom) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, type))
                makeroom.first = false;
            return true;
        }
        if (!a.CheckType(MSC_ATTR_BOOL, msc->Error)) return true;
        makeroom.first = true;
        makeroom.second = a.yes;
        return true;
    }
    if (f_lost) {
        if (a.StartsWith("lost.line"))
            return lost_line.AddAttribute(a, msc, type);
        if (a.StartsWith("lost.arrow"))
            return lost_arrow.AddAttribute(a, msc, type);
        if (a.StartsWith("lost.text"))
            return lost_text.AddAttribute(a, msc, type);
    }
    if (f_lsym) {
        if (a.StartsWith("x.line"))
            return lsym_line.AddAttribute(a, msc, type);
        if (a.Is("x.size")) {
            if (a.type == MSC_ATTR_STRING &&
                Convert(a.value, lsym_size.second)) {
                lsym_size.first = true;
                return true;
            }
            a.InvalidValueError(CandidatesFor(lsym_size.second), msc->Error);
            return true;
        }
    }
    if (f_shape) {
        if (a.Is("shape")) {
            if (!msc->Shapes) {
                msc->Error.Error(a, false, "No shapes have been defined. Ignoring attribute.", 
                    "Use the 'defshape' command to define shapes.");
                return true;
            }
            if (a.type == MSC_ATTR_CLEAR) {
                shape.first = false;
                return true;
            }
            if (!a.CheckType(MSC_ATTR_STRING, msc->Error))
                return true;
            int sh = msc->Shapes.GetShapeNo(a.value);
            if (sh==-1) {
                string msg("Use one of '");
                const auto v = msc->Shapes.ShapeNames(a.value);
                for (size_t u = 0; u<v.size()-1; u++)
                    if (u+2==v.size())
                        msg.append(v[u]).append("' or '");
                    else
                        msg.append(v[u]).append("', '");
                msg.append(v.back());
                msc->Error.Error(a, true, "Unrecognized Shape. Ignoring attribute.", msg + "'.");
            } else {
                shape.first = true;
                shape.second = sh;
            }
            return true;
        }
        if (a.Is("shape.size")) {
            if (!msc->Shapes) {
                msc->Error.Error(a, false, "No shapes have been defined. Ignoring attribute.", 
                    "Use the 'defshape' command to define shapes.");
                return true;
            }
            if (a.type == MSC_ATTR_CLEAR) {
                shape_size.first = false; 
                return true;
            }
            if (a.type == MSC_ATTR_STRING &&
                Convert(a.value, shape_size.second)) {
                shape_size.first = true;
                return true;
            }
            a.InvalidValueError(CandidatesFor(shape_size.second), msc->Error);
            return true;
        }
    }
    if (f_tag && a.StartsWith("tag.line"))
        return tag_line.AddAttribute(a, msc, type);
    if (f_tag && a.StartsWith("tag.fill"))
        return tag_fill.AddAttribute(a, msc, type);
    if (f_tag && a.StartsWith("tag.text"))
        return tag_text.AddAttribute(a, msc, type);
    return false;
}

bool MscStyle::DoIAcceptUnqualifiedColorAttr() const
{
    switch (color_meaning) {
    case EColorMeaning::FILL: return f_fill;
    case EColorMeaning::TEXT: return f_text;
    case EColorMeaning::LINE_ARROW_TEXT:
        return f_text || f_line || f_arrow!=ArrowHead::NONE;
    case EColorMeaning::LINE_VLINE_TEXT:
        return f_text || f_line || f_vline;
    default:
        _ASSERT(0);
    case EColorMeaning::NOHOW:
        return false;
    }
}


/** Add the attribute names we take to `csh`.*/
void MscStyle::AttributeNames(Csh &csh) const
{
    if (DoIAcceptUnqualifiedColorAttr())
          csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"color", HINT_ATTR_NAME));
    if (f_line) LineAttr::AttributeNames(csh, "line.");
    if (f_fill) FillAttr::AttributeNames(csh, "fill.");
    if (f_arrow!=ArrowHead::NONE) ArrowHead::AttributeNames(csh, "arrow.");
    if (f_shadow) ShadowAttr::AttributeNames(csh);
    if (f_text) StringFormat::AttributeNames(csh, "text.");
    if (f_solid) 
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"solid", HINT_ATTR_NAME));
    if (f_side != ESideType::NO)
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"side", HINT_ATTR_NAME));
    if (f_numbering) csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"number", HINT_ATTR_NAME));
    if (f_indicator) csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"indicator", HINT_ATTR_NAME));
    if (f_makeroom) csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"makeroom", HINT_ATTR_NAME));
    if (f_vspacing) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"compress", HINT_ATTR_NAME));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"vspacing", HINT_ATTR_NAME));
    }
    if (f_line) LineAttr::AttributeNames(csh, "vline.");
    if (f_fill) FillAttr::AttributeNames(csh, "vfill.");
    if (f_note) NoteAttr::AttributeNames(csh);
    if (f_lost) {
        LineAttr::AttributeNames(csh, "lost.line.");
        ArrowHead::AttributeNames(csh, "lost.arrow.");
        StringFormat::AttributeNames(csh, "lost.text.");
    }
    if (f_lsym) {
        LineAttr::AttributeNames(csh, "x.line.");
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"x.size", HINT_ATTR_NAME));
    }
    if (f_shape && csh.pShapes && *csh.pShapes) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "shape", HINT_ATTR_NAME));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "shape.size", HINT_ATTR_NAME));
    }
    if (f_tag) {
        LineAttr::AttributeNames(csh, "tag.line.");
        FillAttr::AttributeNames(csh, "tag.fill.");
        StringFormat::AttributeNames(csh, "tag.text.");
    }
    csh.AddStylesToHints();
}

/** Callback for drawing a symbol before side values in the hints popup list box.
 * @ingroup libmscgen_hintpopup_callbacks*/
bool CshHintGraphicCallbackForSide(Canvas *canvas, CshHintGraphicParam p, Csh &)
{
    if (!canvas) return false;
    const ESide t = (ESide)(int)p;
    XY bounds(HINT_GRAPHIC_SIZE_X, HINT_GRAPHIC_SIZE_Y);
    if (t==ESide::END) bounds.SwapXY();
    std::vector<double> xPos(2); 
    xPos[0] = 0;
    xPos[1] = bounds.x*0.7;
    LineAttr eLine(LINE_SOLID, ColorType(0,0,0), 1, CORNER_NONE, 0);
    ArrowHead ah(ArrowHead::BIGARROW);
    ah.line += ColorType(0,32,192); //blue-green
    ah.endType.second = MSC_ARROW_SOLID;
    ah.startType.second = MSC_ARROW_NONE;
    ah.size.second = MSC_ARROWS_INVALID;
    ShadowAttr shadow;
    FillAttr fill(ah.line.color.second.Lighter(0.7), t==ESide::RIGHT ? GRADIENT_DOWN : GRADIENT_UP);
    std::vector<double> active(2,0.);
    canvas->Transform_Rotate(bounds/2, t==ESide::LEFT ? 0 : t==ESide::RIGHT ? M_PI : M_PI/2);
    canvas->Clip(XY(bounds.x*0.1,1), XY(bounds.x-1, bounds.y-1));
    ah.BigCalculateAndDraw(xPos, active, bounds.y*0.3, bounds.y*0.7, 
                           true, false, fill, shadow, *canvas);
    canvas->UnClip();
    canvas->UnTransform();
    return true;
}

/** Add a list of possible attribute value names to `csh` for attribute `attr`.*/
bool MscStyle::AttributeValues(const std::string &attr, Csh &csh) const
{
    if (CaseInsensitiveEqual(attr, "color")) {
        if (!DoIAcceptUnqualifiedColorAttr()) return false;
        csh.AddColorValuesToHints(false);
        return true;
    }
    if (CaseInsensitiveEqual(attr, "line.width")) {
        if (f_arrow!=ArrowHead::NONE) return arrow.AttributeValues(attr, csh, f_arrow);
        if (f_line) return line.AttributeValues(attr, csh);
        return false;
    }
    if ((CaseInsensitiveBeginsWith(attr, "text") || CaseInsensitiveEqual(attr, "ident")) && f_text)
        return text.AttributeValues(attr, csh);
    if (CaseInsensitiveBeginsWith(attr, "line") && f_line)
        return line.AttributeValues(attr, csh);
    if (CaseInsensitiveBeginsWith(attr, "vline") && f_vline)
        return vline.AttributeValues(attr, csh);
    if (CaseInsensitiveBeginsWith(attr, "fill") && f_fill)
        return fill.AttributeValues(attr, csh);
    if (CaseInsensitiveBeginsWith(attr, "vfill") && f_vfill)
        return vfill.AttributeValues(attr, csh);
    if (CaseInsensitiveBeginsWith(attr, "shadow") && f_shadow)
        return shadow.AttributeValues(attr, csh);
    if ((CaseInsensitiveBeginsWith(attr, "arrow") || CaseInsensitiveEqual(attr, "arrowsize")) && f_arrow!=ArrowHead::NONE)
        return arrow.AttributeValues(attr, csh, f_arrow);
    if (CaseInsensitiveEqual(attr, "solid") && f_solid) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number: \b0.0..1.0\b>", HINT_ATTR_VALUE, false));
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number: \b0..255\b>", HINT_ATTR_VALUE, false));
        return true;
    }
    if ((CaseInsensitiveEqual(attr, "compress") && f_vspacing) ||
        (CaseInsensitiveEqual(attr, "indicator") && f_indicator) ||
        (CaseInsensitiveEqual(attr, "makeroom") && f_makeroom)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+"yes", HINT_ATTR_VALUE, true, CshHintGraphicCallbackForYesNo, CshHintGraphicParam(1)));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+"no", HINT_ATTR_VALUE, true, CshHintGraphicCallbackForYesNo, CshHintGraphicParam(0)));
        return true;
    }
    if (CaseInsensitiveEqual(attr, "vspacing") && f_vspacing) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", HINT_ATTR_VALUE, false));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+"compress", HINT_ATTR_VALUE, true));
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "side")) {
        for (auto s = ESide::LEFT; s<=ESide::END; s = ESide(int(s)+1))
            if (IsValidSideValue(f_side, s))
                csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+EnumEncapsulator<ESide>::names[unsigned(s)], 
                                       HINT_ATTR_VALUE, true, CshHintGraphicCallbackForSide, 
                                       CshHintGraphicParam(s)));
        return true;
    }
    if (CaseInsensitiveEqual(attr, "number") && f_numbering) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", HINT_ATTR_VALUE, false));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+"yes", HINT_ATTR_VALUE, true, CshHintGraphicCallbackForYesNo, CshHintGraphicParam(1)));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+"no", HINT_ATTR_VALUE, true, CshHintGraphicCallbackForYesNo, CshHintGraphicParam(0)));
        return true;
    }
    if (f_note && CaseInsensitiveBeginsWith(attr, "note"))
        return note.AttributeValues(attr, csh);
    if (f_lost) {
        if (CaseInsensitiveEqual(attr, "lost.line"))
            return line.AttributeValues(attr, csh);
        if (CaseInsensitiveEqual(attr, "lost.arrow"))
            return arrow.AttributeValues(attr, csh, ArrowHead::ARROW);
        if (CaseInsensitiveEqual(attr, "lost.text"))
            StringFormat::AttributeValues(attr, csh);
    }
    if (f_lsym) {
        if (CaseInsensitiveEqual(attr, "x.line"))
            return line.AttributeValues(attr, csh);
        if (CaseInsensitiveEqual(attr, "x.size")) {
            csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"x.size", HINT_ATTR_NAME));
            return true;
        }
    }
    if (f_shape) {
        if (csh.pShapes && *csh.pShapes && CaseInsensitiveEqual(attr, "shape")) {
            csh.pShapes->AttributeValues(csh);
            for (const auto &s : csh.shape_names)
                csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+s,
                HINT_ATTR_VALUE, true));
            return true;
        }
        if (CaseInsensitiveEqual(attr, "shape.size")) {
            ArrowHead::AttributeValues(attr, csh, ArrowHead::ANY);
            return true;
        }
    }
    if (f_tag) {
        if (CaseInsensitiveBeginsWith(attr, "tag_text"))
            return tag_text.AttributeValues(attr, csh);
        if (CaseInsensitiveBeginsWith(attr, "tag.line"))
            return tag_line.AttributeValues(attr, csh);
        if (CaseInsensitiveBeginsWith(attr, "tag.fill"))
            return tag_fill.AttributeValues(attr, csh);
    }
    return false;
}

/**Print the content of the style*/
string MscStyle::Print(int) const
{
    string s = "style:(";
    if (f_line) s.append(line.Print());
    if (f_vline) s.append(vline.Print());
    if (f_fill) s.append(fill.Print());
    if (f_vfill) s.append(vfill.Print());
    if (f_shadow) s.append(shadow.Print());
    if (f_solid) s.append("solid:").append(solid.second?"yes":"no").append("\n");
    if (f_indicator) s.append("indicator:").append(solid.second?"yes":"no").append("\n");
    if (f_makeroom) s.append("makeroom:").append(solid.second?"yes":"no").append("\n");
    if (f_side != ESideType::NO) s.append("side:").append(side.second==ESide::LEFT?"left":side.second==ESide::RIGHT?"right":"end").append("\n");
//    if (f_arrow!=ArrowHead::NONE) s.append(arrow.Print());
//    if (f_text) s.append(text.Print());
    if (f_note) s.append(note.Print());
    //MISSING lost & shape & tag ones
    s.append(")");
    return s;
}


MscStyle &StyleCoW::Copy()
{
    _ASSERT(p->ref_count>1);
    p->ref_count--;
    StyleCopy *p_new = new StyleCopy(p->style);
    p = p_new;
    return p->style;
}


/** Look up a style by name. Return the default style if not found.*/
const StyleCoW &StyleSet::GetStyle(const string &s) const
{
    const_iterator i = find(s);
    if (i==end()) return defaultStyle;
    else return i->second;
};

Context &Context::operator +=(const Context &o)
{
    is_full |= o.is_full;
    if (o.hscale.first) hscale = o.hscale;
    if (o.numbering.first) numbering = o.numbering;
    if (o.vspacing.first) vspacing = o.vspacing;
    if (o.indicator.first) indicator = o.indicator;
    if (o.slant_angle.first) slant_angle = o.slant_angle;
    if (o.auto_heading.first) auto_heading = o.auto_heading;
    defCommentLine += o.defCommentLine;
    defCommentFill += o.defCommentFill;
    defBackground += o.defBackground;
    text += o.text;
    for (auto i = o.colors.begin(); i!=o.colors.end(); i++) 
        colors[i->first] = i->second;
    for (auto i = o.styles.begin(); i!=o.styles.end(); i++) {
        auto j = styles.find(i->first);
        if (j==styles.end()) 
            styles[i->first] = i->second;
        else
            j->second += i->second;
    }
    numberingStyle += o.numberingStyle;
    return *this;
}

void Context::Empty()
{
    is_full = false;
    hscale.first = false;
    numbering.first = false;
    vspacing.first = false;
    indicator.first = false;
    slant_angle.first = false;
    auto_heading.first = false;
    defCommentLine.Empty();
    defCommentFill.Empty();
    defBackground.Empty();
    text.Empty();
    styles.clear();
    colors.clear();
    numberingStyle.Empty();  

    //Now add default styles, but all empty
    
    styles["arrow"] = 
        MscStyle(STYLE_DEFAULT, EColorMeaning::LINE_ARROW_TEXT, ArrowHead::ARROW,
                 true, true, false, false, false, 
                 false, true, true, ESideType::NO, true, false, false, false, 
                 true, true, false, false); 
                 //no fill, shadow, vline solid side vfill, makeroom, note, shape, tag
    styles["->"] = styles["arrow"]; 
    styles["->"].write().type = STYLE_DEF_ADD;
    styles[">"]  = styles["->"];
    styles[">>"] = styles["->"];
    styles["=>"] = styles["->"];

    styles["blockarrow"] =
        MscStyle(STYLE_DEFAULT, EColorMeaning::FILL, ArrowHead::BIGARROW,
                 true, true, true, true, false,
                 false, true, true, ESideType::NO, false, false, false, false, 
                 false, false, false, false);
                 //no vline solid side indicator vfill makeroom note loss lsym shape, tag
    styles["box_collapsed_arrow"] = styles["blockarrow"];
    styles["block->"] = styles["blockarrow"];
    styles["block->"].write().type = STYLE_DEF_ADD;
    styles["block>"] = styles["block->"];
    styles["block>>"] = styles["block->"];
    styles["block=>"] = styles["block->"];

    styles["vertical"] = 
        MscStyle(STYLE_DEFAULT, EColorMeaning::FILL, ArrowHead::BIGARROW, 
                 true, true, true, true, false,
                 false, true, true, ESideType::ANY, false, false, true, false, 
                 false, false, false, false);
                 //no vline solid indicator vfill note loss lsym shape tag
    styles["vertical_range"] = 
        MscStyle(STYLE_DEFAULT, EColorMeaning::LINE_ARROW_TEXT, ArrowHead::ARROW, 
                 true, true, true, true, false,
                 false, true, true, ESideType::ANY, false, false, true, false, 
                 false, false, false, false);
                 //no vline solid indicator vfill note loss lsym shape tag
    styles["vertical_pointer"] =
        MscStyle(STYLE_DEFAULT, EColorMeaning::LINE_ARROW_TEXT, ArrowHead::ARROW, 
                 true, true, true, true, false,
                 false, true, true, ESideType::ANY, false, false, true, false, 
                 true, true, false, false);
                 //no vline solid indicator vfill note shape tag
    styles["vertical_bracket"] = 
        MscStyle(STYLE_DEFAULT, EColorMeaning::LINE_ARROW_TEXT, ArrowHead::NONE, 
                 true, true, true, true, false,
                 false, true, true, ESideType::ANY, false, false, true, false, 
                 false, false, false, false);
                 //no arrow vline solid indicator vfill note loss lsym shape tag
    styles["vertical_brace"] = styles["vertical_bracket"];

    styles["vertical->"] =
        MscStyle(STYLE_DEF_ADD, EColorMeaning::NOHOW, ArrowHead::ANY,
                true, true, true, true, false,
                false, true, true, ESideType::ANY, false, false, true, false,
                true, true, false, false);
                //no vline solid indicator vfill note  shape tag
    styles["vertical>"] = styles["vertical->"];
    styles["vertical>>"] = styles["vertical->"];
    styles["vertical=>"] = styles["vertical->"];
    styles["vertical--"] = styles["vertical->"];
    styles["vertical++"] = styles["vertical->"];
    styles["vertical.."] = styles["vertical->"];
    styles["vertical=="] = styles["vertical->"];

    styles["divider"] = 
        MscStyle(STYLE_DEFAULT, EColorMeaning::LINE_ARROW_TEXT, ArrowHead::NONE, 
                 true, true, false, false, true,
                 false, true, true, ESideType::NO, false, false, false, false, 
                 false, false, false, false);
                 //no arrow, fill, shadow solid side indicator vfill makeroom note loss lsym shape tag
    styles["---"] = styles["divider"];
    styles["---"].write().type = STYLE_DEF_ADD;
    styles["..."] = styles["---"];

    styles["box"] = 
        MscStyle(STYLE_DEFAULT, EColorMeaning::FILL, ArrowHead::NONE, 
                 true, true, true, true, false,
                 false, true, true, ESideType::NO, true, false, false, false, 
                 false, false, false, true);
                 //no arrow, vline solid side vfill makeroom note loss lsym shape
    styles["box_collapsed"] = styles["box"];
    styles["emptybox"] = styles["box"];
    styles["--"] = styles["box"];
    styles["--"].write().type = STYLE_DEF_ADD;
    styles["++"] = styles["--"];
    styles[".."] = styles["--"];
    styles["=="] = styles["--"];

    styles["pipe"] = 
        MscStyle(STYLE_DEFAULT, EColorMeaning::FILL, ArrowHead::NONE, 
                 true, true, true, true, false,
                 true, true, true, ESideType::LEFT_RIGHT, false, false, false, false, 
                 false, false, false, false);
                 //no arrow, vline indicator vfill makeroom note loss lsym shape tag
    styles["pipe--"] = styles["pipe"];
    styles["pipe--"].write().type = STYLE_DEF_ADD;
    styles["pipe++"] = styles["pipe--"];
    styles["pipe.."] = styles["pipe--"];
    styles["pipe=="] = styles["pipe--"];

    styles["entity"] = 
        MscStyle(STYLE_DEFAULT, EColorMeaning::LINE_VLINE_TEXT, ArrowHead::NONE, 
                 true, true, true, true, true,
                 false, false, false, ESideType::NO, true, true, false, false, 
                 false, false, true, false);
                 //no arrow, solid numbering compress side makeroom note loss lsym tag
    styles["entitygroup_collapsed"] = styles["entity"];
    styles["entitygroup"] = 
        MscStyle(STYLE_DEFAULT, EColorMeaning::LINE_VLINE_TEXT, ArrowHead::NONE, 
                 true, true, true, true, true,
                 false, false, false, ESideType::NO, true, true, false, false, 
                 false, false, false, false);
                 //no arrow, solid numbering compress side makeroom note loss lsym shape tag

    styles["indicator"] = 
        MscStyle(STYLE_DEFAULT, EColorMeaning::LINE_ARROW_TEXT, ArrowHead::NONE, 
                 false, true, true, true, false,
                 false, false, false, ESideType::NO, false, false, false, false, 
                 false, false, false, false);
                 //fill line shadow only
    styles["symbol"] =
        MscStyle(STYLE_DEFAULT, EColorMeaning::NOHOW, ArrowHead::NONE, 
                 false, true, true, true, false,
                 false, false, false, ESideType::NO, false, false, false, false, 
                 false, false, false, false);
                 //only line fill and shadow

    styles["note"] = 
        MscStyle(STYLE_DEFAULT, EColorMeaning::NOHOW, ArrowHead::NOTE,
                 true, true, true, true, false, 
                 false, true, false, ESideType::NO, false, false, false, true, 
                 false, false, false, false);
                 //no vline side solid indicator compress vfill makreoom loss lsym shape tag
    
    styles["comment"] = 
        MscStyle(STYLE_DEFAULT, EColorMeaning::LINE_ARROW_TEXT, ArrowHead::NONE, 
                 true, false, false, false, false,
                 false, true, false, ESideType::ANY, false, false, false, false, 
                 false, false, false, false);
                 //only text numbering and side
    styles["endnote"] = styles["comment"];
    //styles["footnote"] = styles["comment"];

    styles["title"] = 
        MscStyle(STYLE_DEFAULT, EColorMeaning::TEXT, ArrowHead::NONE, 
                 true, true, true, true, true,
                 false, false, false, ESideType::NO, false, false, false, false, 
                 false, false, false, false);
                 //line, fill, shadow, vline text  
    styles["subtitle"] = styles["title"];
}

void Context::Plain()
{
    Empty();
    is_full = true;
    hscale.first = true;
    hscale.second = 1.0;
    numbering.first = true;
    numbering.second=false;
    vspacing.first = true;
    vspacing.second = 0;
    indicator.first = true;
    indicator.second = true;
    slant_angle.first = true;
    slant_angle.second = 0;
    auto_heading.first = true;
    auto_heading.second = false;
    defCommentLine.MakeComplete();
    defCommentFill.MakeComplete();
    defBackground.MakeComplete();
    defCommentLine.width.second = 3;
    defCommentFill.color.second.a = 0; //fully transparent
    defBackground.color.second.a = 0; //fully transparent
    numberingStyle.Reset();
    numberingStyle.pre.first = true;
    numberingStyle.pre.second.clear();
    numberingStyle.post.first = true;
    numberingStyle.post.second = ": ";
    text.Default();

    colors.clear();
    colors["none"]  = ColorType(  0,   0,   0, 0);
    colors["black"] = ColorType(  0,   0,   0);
    colors["white"] = ColorType(255, 255, 255);
    colors["red"]   = ColorType(255,   0,   0);
    colors["green"] = ColorType(  0, 255,   0);
    colors["blue"]  = ColorType(  0,   0, 255);
    colors["yellow"]= ColorType(255, 255,   0);
    colors["gray"]  = ColorType(150, 150, 150);
    colors["lgray"] = ColorType(200, 200, 200);

    const std::pair<bool, ColorType> faint(true, ColorType(255, 255, 255, 224, ColorType::OVERLAY));

    styles["arrow"].write().MakeCompleteButText();
    styles["arrow"].write().vspacing.first = false;
    styles["arrow"].write().numbering.first = false;
    styles["arrow"].write().line.radius.second = -1;
    styles["arrow"].write().lost_line.color = faint;
    styles["arrow"].write().lost_arrow.line.color = faint;
    //styles["arrow"].write().lost_text.SetColor(faint);  do not change label in the lost part

    styles["->"].write().line.type.first = true;
    styles["->"].write().line.type.second = LINE_SOLID;
    styles[">"].write().line.type.first = true;
    styles[">"].write().line.type.second = LINE_DOTTED;
    styles[">>"].write().line.type.first = true;
    styles[">>"].write().line.type.second = LINE_DASHED;
    styles["=>"].write().line.type.first = true;
    styles["=>"].write().line.type.second = LINE_DOUBLE;

    styles["blockarrow"].write().MakeCompleteButText();
    styles["blockarrow"].write().vspacing.first = false;
    styles["blockarrow"].write().numbering.first = false;
    styles["blockarrow"].write().line.radius.second = -1;
    styles["box_collapsed_arrow"] = styles["blockarrow"];
    styles["box_collapsed_arrow"].write().arrow.midType.second = MSC_ARROW_DOT;

    styles["block->"].write().line.type.first = true;
    styles["block->"].write().line.type.second = LINE_SOLID;
    styles["block>"].write().line.type.first = true;
    styles["block>"].write().line.type.second = LINE_DOTTED;
    styles["block>>"].write().line.type.first = true;
    styles["block>>"].write().line.type.second = LINE_DASHED;
    styles["block=>"].write().line.type.first = true;
    styles["block=>"].write().line.type.second = LINE_DOUBLE;

    styles["vertical"].write().MakeCompleteButText();
    styles["vertical"].write().vspacing.first = false;
    styles["vertical"].write().numbering.first = false;
    styles["vertical"].write().makeroom.second = true;
    styles["vertical"].write().side.first = false;
    styles["vertical"].write().line.radius.second = -1;
    styles["vertical"].write().text.UnsetWordWrap();
    //We need this otherwise setting the global text style to wrap
    //would cause warnings for all horizontally set verticals which
    //have no text.width attribute. Better if the user sets this explicitly.

    styles["vertical_range"].write().MakeCompleteButText();
    styles["vertical_range"].write().vspacing.first = false;
    styles["vertical_range"].write().numbering.first = false;
    styles["vertical_range"].write().makeroom.second = true;
    styles["vertical_range"].write().side.first = false;
    styles["vertical_range"].write().line.radius.second = 8;
    styles["vertical_range"].write().text.UnsetWordWrap();

    styles["vertical_bracket"].write().MakeCompleteButText();
    styles["vertical_bracket"].write().vspacing.first = false;
    styles["vertical_bracket"].write().numbering.first = false;
    styles["vertical_bracket"].write().makeroom.second = true;
    styles["vertical_bracket"].write().side.first = false;
    styles["vertical_bracket"].write().line.radius.second = 8;
    styles["vertical_bracket"].write().line.corner.second = CORNER_NONE;
    styles["vertical_bracket"].write().text.UnsetWordWrap();

    styles["vertical_pointer"].write().MakeCompleteButText();
    styles["vertical_pointer"].write().vspacing.first = false;
    styles["vertical_pointer"].write().numbering.first = false;
    styles["vertical_pointer"].write().makeroom.second = true;
    styles["vertical_pointer"].write().side.first = false;
    styles["vertical_pointer"].write().line.radius.second = 8;
    styles["vertical_pointer"].write().line.corner.second = CORNER_NONE;
    styles["vertical_pointer"].write().text.UnsetWordWrap();
    styles["vertical_pointer"].write().lost_line.color = faint;
    styles["vertical_pointer"].write().lost_arrow.line.color = faint;
    //styles["vertical_pointer"].write().lost_text.SetColor(faint.second); do not change label in lost part

    styles["vertical_brace"].write().MakeCompleteButText();
    styles["vertical_brace"].write().vspacing.first = false;
    styles["vertical_brace"].write().numbering.first = false;
    styles["vertical_brace"].write().makeroom.second = true;
    styles["vertical_brace"].write().side.first = false;
    styles["vertical_brace"].write().line.radius.second = 8;
    styles["vertical_brace"].write().line.corner.second = CORNER_ROUND;
    styles["vertical_brace"].write().text.UnsetWordWrap();

    styles["vertical->"].write().line.type.first = true;
    styles["vertical->"].write().line.type.second = LINE_SOLID;
    styles["vertical>"].write().line.type.first = true; 
    styles["vertical>"].write().line.type.second = LINE_DOTTED;
    styles["vertical>>"].write().line.type.first = true;
    styles["vertical>>"].write().line.type.second = LINE_DASHED;
    styles["vertical=>"].write().line.type.first = true;
    styles["vertical=>"].write().line.type.second = LINE_DOUBLE;

    styles["vertical--"].write().arrow.startType.first = true;
    styles["vertical--"].write().arrow.startType.second = MSC_ARROW_NONE;
    styles["vertical--"].write().arrow.midType.first = true;
    styles["vertical--"].write().arrow.midType.second = MSC_ARROW_NONE;
    styles["vertical--"].write().arrow.endType.first = true;
    styles["vertical--"].write().arrow.endType.second = MSC_ARROW_NONE;
    styles["vertical--"].write().line.type.first = true;
    styles["vertical--"].write().line.type.second = LINE_SOLID;
    styles["vertical++"] = styles["vertical--"];
    styles["vertical++"].write().line.type.second = LINE_DASHED;
    styles["vertical.."] = styles["vertical--"];
    styles["vertical.."].write().line.type.second = LINE_DOTTED;
    styles["vertical=="] = styles["vertical--"];
    styles["vertical=="].write().line.type.second = LINE_DOUBLE;

    styles["divider"].write().MakeCompleteButText();
    styles["divider"].write().vspacing.first = false;
    styles["divider"].write().numbering.first = false;
    styles["divider"].write().vline.Empty();
    styles["divider"].write().line.type.second = LINE_NONE;

    styles["---"].write().line.type.first = true;
    styles["---"].write().line.type.second = LINE_DOTTED;
    styles["..."].write().vline.type.first = true;
    styles["..."].write().vline.type.second = LINE_DOTTED;
    styles["..."].write().text.Apply("\\mu(10)\\md(10)");

    styles["emptybox"].write().MakeCompleteButText();
    styles["emptybox"].write().vspacing.first = false;
    styles["emptybox"].write().numbering.first = false;
    styles["emptybox"].write().tag_line.corner.second = CORNER_BEVEL;
    styles["emptybox"].write().tag_line.radius.second = 10;
    styles["emptybox"].write().tag_text.Apply("\\B\\-");
    styles["box_collapsed"] = styles["emptybox"];
    styles["box"] = styles["emptybox"];
    styles["box"].write().text.Apply("\\pl");
    styles["box"].write().line.type.first = false;

    styles["--"].write().line.type.first = true;
    styles["--"].write().line.type.second = LINE_SOLID;
    styles["++"].write().line.type.first = true;
    styles["++"].write().line.type.second = LINE_DASHED;
    styles[".."].write().line.type.first = true;
    styles[".."].write().line.type.second = LINE_DOTTED;
    styles["=="].write().line.type.first = true;
    styles["=="].write().line.type.second = LINE_DOUBLE;

    styles["pipe"].write().MakeCompleteButText();
    styles["pipe"].write().vspacing.first = false;
    styles["pipe"].write().numbering.first = false;
    styles["pipe"].write().line.radius.second = 5;

    styles["pipe--"].write().line.type.first = true;
    styles["pipe--"].write().line.type.second = LINE_SOLID;
    styles["pipe++"].write().line.type.first = true;
    styles["pipe++"].write().line.type.second = LINE_DASHED;
    styles["pipe.."].write().line.type.first = true;
    styles["pipe.."].write().line.type.second = LINE_DOTTED;
    styles["pipe=="].write().line.type.first = true;
    styles["pipe=="].write().line.type.second = LINE_DOUBLE;

    styles["entity"].write().MakeCompleteButText();
    styles["entitygroup_collapsed"].write().MakeCompleteButText();
    styles["entitygroup"].write().MakeCompleteButText();
    styles["entitygroup"].write().line.type.second = LINE_DASHED;

    styles["indicator"].write().MakeCompleteButText();
    styles["indicator"].write().line.width.second = 2;

    styles["symbol"].write().MakeCompleteButText();

    styles["note"].write().MakeCompleteButText();
    styles["note"].write().numbering.first = false;
    styles["note"].write().text += "\\mn(10)\\ms(6)"; //small font to 6, normal to 10

    styles["comment"].write().MakeCompleteButText();
    styles["comment"].write().numbering.first = false;
    styles["comment"].write().text += "\\mn(10)\\ms(6)\\pl"; //small font to 6, normal to 10, left para

    styles["endnote"].write().MakeCompleteButText();
    styles["endnote"].write().numbering.first = false;
    styles["endnote"].write().text += "\\mn(10)\\ms(6)\\pl"; //small font to 6, normal to 10, left para
    styles["endnote"].write().side.second = ESide::END;

    //styles["footnote"].write().MakeCompleteButText();
    //styles["footnote"].write().numbering.first = false;
    //styles["footnote"].write().text += "\\mn(10)\\ms(6)\\pl"; //small font to 6, normal to 10, left para
    //styles["footnote"].write().side.second = ESide::END;

    styles["title"].write().MakeCompleteButText();
    styles["title"].write().vline.type.second = LINE_NONE;
    styles["title"].write().line.type.second = LINE_NONE;
    styles["title"].write().fill.color.second = ColorType(0,0,0,0); //no fill
    styles["title"].write().text += "\\mn(28)\\ms(18)\\B";
    styles["subtitle"] = styles["title"];
    styles["subtitle"].write().text += "\\mn(22)\\ms(14)\\B";
    
    //Ok, now "weak" and "strong"
    const std::pair<bool, ColorType> weak(true, ColorType(255, 255, 255, 128, ColorType::OVERLAY));
    MscStyle style = MscStyle(STYLE_STYLE); //has everything, but is empty
    style.line.color = weak;
    style.vline.color = weak;
    style.arrow.line.color = weak;
    style.text.SetColor(weak.second);
	style.text.Apply("\\i");
    styles["weak"] = style;
    
    style.Empty();
    std::pair<bool, double> w2(true, 2.0);
    style.line.width = w2;
    style.vline.width = w2;
    style.arrow.line.width = w2;
    style.text.Apply("\\b");
    styles["strong"] = style;
}

