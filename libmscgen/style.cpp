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
/** @file style.cpp The implementation of styles (MscStyle) and contexts (Context).
 * @ingroup libmscgen_files */


#include "msc.h"

/** Create an empty style that contains all possible attributes.*/
MscStyle::MscStyle(EStyleType tt) : type(tt)
{
    f_line=f_vline=f_fill=f_vfill=f_shadow=f_text=true;
    f_solid=f_numbering=f_compress=f_side=f_indicator=true;
    f_makeroom=f_note=true;
    f_arrow=ArrowHead::ANY;
    Empty();
}

/** Create an empty style that contains only some of the attributes.
 * @param [in] tt The type of style instance.
 * @param [in] a What type of arrows we accept.
 * @param [in] t True if the style shall contain text attributes.
 * @param [in] l True if the style shall contain line attributes.
 * @param [in] f True if the style shall contain fill attributes.
 * @param [in] s True if the style shall contain shadow attributes.
 * @param [in] vl True if the style shall contain vline attributes.
 * @param [in] so True if the style shall contain the 'solid' attribute.
 * @param [in] nu True if the style shall contain the 'number' attribute.
 * @param [in] co True if the style shall contain the `compress` attribute.
 * @param [in] si True if the style shall contain the `side` attribute.
 * @param [in] i True if the style shall contain the `indicator` attribute.
 * @param [in] vf True if the style shall contain vfill attributes.
 * @param [in] mr True if the style shall contain the `makeroom` attribute.
 * @param [in] n True if the style shall contain note attributes.
 */
MscStyle::MscStyle(EStyleType tt, ArrowHead::EArcArrowType a, bool t, bool l, bool f, bool s, bool vl, 
                   bool so, bool nu, bool co, bool si, bool i, bool vf, bool mr, bool n) :
    arrow(a), type(tt), f_line(l), f_vline(vl), f_fill(f), f_vfill(vf), f_shadow(s),
    f_text(t), f_solid(so), f_numbering(nu), f_compress(co), f_side(si),
    f_indicator(i), f_makeroom(mr), f_note(n), f_arrow(a)
{
    Empty();
}

/** Make a style complete by setting the default values - but leave text attributes empty.
 * Default attributes are half solid, right side, no numbering, no compress, indicator yes,
 * makeroom yes and the default of other attribute classes (specified there).
 * We skip text styles, since we have a global text attribute per chart and 
 * we set the default there.
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
    side.first = f_side;  
    side.second = SIDE_RIGHT;
    numbering.first = f_numbering;
    numbering.second = false;
    compress.first = f_compress;
    compress.second = false;
    indicator.first = f_indicator;
    indicator.second = true;
    makeroom.first = f_makeroom;
    makeroom.second = true;
    if (f_note) note.MakeComplete();
    else note.Empty();
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
    compress.first = false;
    numbering.first = false;
    indicator.first = false;
    makeroom.first = false;
    note.Empty();
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
    if (toadd.f_side && f_side && toadd.side.first) side = toadd.side;
    if (toadd.f_compress && f_compress && toadd.compress.first) compress = toadd.compress;
    if (toadd.f_numbering && f_numbering && toadd.numbering.first) numbering = toadd.numbering;
    if (toadd.f_indicator && f_indicator && toadd.indicator.first) indicator = toadd.indicator;
    if (toadd.f_makeroom && f_makeroom && toadd.makeroom.first) makeroom = toadd.makeroom;
    if (toadd.f_note && f_note) note += toadd.note;
    return *this;
}

/** Possible values for the 'side' attribute.*/
template<> const char EnumEncapsulator<ESideType>::names[][ENUM_STRING_LEN] =
    {"invalid", "left", "right", ""};


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
    if (a.Is("side") && f_side) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, type))
                side.first = false;
            return true;
        }
        if (a.type == MSC_ATTR_STRING && Convert(a.value, side.second)) {
            side.first = true;
            return true;
        }
        a.InvalidValueError(CandidatesFor(side.second), msc->Error);
        return true;
    }
    if (a.Is("compress") && f_compress) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, type))
                compress.first = false;
            return true;
        }
        if (!a.CheckType(MSC_ATTR_BOOL, msc->Error)) return true;
        compress.first = true;
        compress.second = a.yes;
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
                compress.first = false;
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
    return false;
}

/** Add the attribute names we take to `csh`.*/
void MscStyle::AttributeNames(Csh &csh) const
{
    static const char names[][ENUM_STRING_LEN] =
    {"", "vline.color", "vline.type", "vline.width", "vline.radius", "vline.corner", ""};
    static const char names2[][ENUM_STRING_LEN] =
    {"", "vfill.color", "vfill.color2", "vfill.gradient", ""};

    if (f_line) LineAttr::AttributeNames(csh);
    if (f_fill) FillAttr::AttributeNames(csh);
    if (f_arrow!=ArrowHead::NONE) ArrowHead::AttributeNames(csh);
    if (f_shadow) ShadowAttr::AttributeNames(csh);
    if (f_text) StringFormat::AttributeNames(csh);
    if (f_solid) 
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"solid", HINT_ATTR_NAME));
    if (f_side)
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"side", HINT_ATTR_NAME));
    if (f_numbering) csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"number", HINT_ATTR_NAME));
    if (f_indicator) csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"indicator", HINT_ATTR_NAME));
    if (f_makeroom) csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"makeroom", HINT_ATTR_NAME));
    if (f_compress) csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"compress", HINT_ATTR_NAME));
    if (f_vline) csh.AddToHints(names, csh.HintPrefix(COLOR_ATTRNAME), HINT_ATTR_NAME);
    if (f_vfill) csh.AddToHints(names2, csh.HintPrefix(COLOR_ATTRNAME), HINT_ATTR_NAME);
    if (f_note) NoteAttr::AttributeNames(csh);
    csh.AddStylesToHints();
}

/** Callback for drawing a symbol before side values in the hints popup list box.
 * @ingroup libmscgen_hintpopup_callbacks*/
bool CshHintGraphicCallbackForSide(Canvas *canvas, CshHintGraphicParam p)
{
    if (!canvas) return false;
    const ESideType t = (ESideType)(int)p;
    std::vector<double> xPos(2); 
    xPos[0] = t==SIDE_LEFT ? 0 : HINT_GRAPHIC_SIZE_X*0.3;
    xPos[1] = t==SIDE_LEFT ? HINT_GRAPHIC_SIZE_X*0.7 : HINT_GRAPHIC_SIZE_X;
    LineAttr eLine(LINE_SOLID, ColorType(0,0,0), 1, CORNER_NONE, 0);
    canvas->Clip(XY(HINT_GRAPHIC_SIZE_X*0.1,1), XY(HINT_GRAPHIC_SIZE_X-1, HINT_GRAPHIC_SIZE_Y-1));
    ArrowHead ah(ArrowHead::BIGARROW);
    ah.line += ColorType(0,32,192); //blue-green
    ah.endType.second =   t==SIDE_LEFT ? MSC_ARROW_SOLID : MSC_ARROW_NONE;
    ah.startType.second = t!=SIDE_LEFT ? MSC_ARROW_SOLID : MSC_ARROW_NONE;
    ah.size.second = MSC_ARROWS_INVALID;
    ShadowAttr shadow;
    FillAttr fill(ah.line.color.second.Lighter(0.7), GRADIENT_UP);
    std::vector<double> active(2,0.);
    ah.BigCalculateAndDraw(xPos, active, HINT_GRAPHIC_SIZE_Y*0.3, HINT_GRAPHIC_SIZE_Y*0.7, 
                           true, false, fill, shadow, *canvas);
    canvas->UnClip();
    return true;
}

/** Add a list of possible attribute value names to `csh` for attribute `attr`.*/
bool MscStyle::AttributeValues(const std::string &attr, Csh &csh) const
{
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
    if ((CaseInsensitiveEqual(attr, "compress") && f_compress) ||
        (CaseInsensitiveEqual(attr, "indicator") && f_indicator) ||
        (CaseInsensitiveEqual(attr, "makeroom") && f_makeroom)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+"yes", HINT_ATTR_VALUE, true, CshHintGraphicCallbackForYesNo, CshHintGraphicParam(1)));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+"no", HINT_ATTR_VALUE, true, CshHintGraphicCallbackForYesNo, CshHintGraphicParam(0)));
        return true;
    }
    if (CaseInsensitiveEndsWith(attr, "side")) {
        csh.AddToHints(EnumEncapsulator<ESideType>::names, csh.HintPrefix(COLOR_ATTRVALUE), 
                       HINT_ATTR_VALUE, CshHintGraphicCallbackForSide); 
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
    if (f_side) s.append("side:").append(side.second==SIDE_LEFT?"left":"right").append("\n");
//    if (f_arrow!=ArrowHead::NONE) s.append(arrow.Print());
//    if (f_text) s.append(text.Print());
    if (f_note) s.append(note.Print());
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
    if (o.compress.first) compress = o.compress;
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
    compress.first = false;
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
        MscStyle(STYLE_DEFAULT, ArrowHead::ARROW, true, true, false, false, false, 
                 false, true, true, false, true, false, false, false); 
                 //no fill, shadow, vline solid side vfill, makeroom, note;
    styles["->"] = styles["arrow"];
    styles[">"]  = styles["arrow"];
    styles[">>"] = styles["arrow"];
    styles["=>"] = styles["arrow"];

    styles["blockarrow"] = 
        MscStyle(STYLE_DEFAULT, ArrowHead::BIGARROW, true, true, true, true, false, 
                 false, true, true, false, false, false, false, false);  
                 //no vline solid side indicator vfill makeroom note;
    styles["box_collapsed_arrow"] = styles["blockarrow"];
    styles["block->"] = styles["blockarrow"];
    styles["block>"]  = styles["blockarrow"];
    styles["block>>"] = styles["blockarrow"];
    styles["block=>"] = styles["blockarrow"];

    styles["vertical"] = 
        MscStyle(STYLE_DEFAULT, ArrowHead::BIGARROW, true, true, true, true, false, 
                 false, true, true, true, false, false, true, false);  
                 //no vline solid indicator vfill note
    styles["vertical->"] = styles["vertical"];
    styles["vertical>"]  = styles["vertical"];
    styles["vertical>>"] = styles["vertical"];
    styles["vertical=>"] = styles["vertical"];
    styles["vertical--"] = styles["vertical"];
    styles["vertical++"] = styles["vertical"];
    styles["vertical.."] = styles["vertical"];
    styles["vertical=="] = styles["vertical"];

    styles["divider"] = 
        MscStyle(STYLE_DEFAULT, ArrowHead::NONE, true, true, false, false, true, 
                 false, true, true, false, false, false, false, false); 
                 //no arrow, fill, shadow solid side indicator vfill makeroom note
    styles["---"] = styles["divider"];
    styles["..."] = styles["divider"];

    styles["box"] = 
        MscStyle(STYLE_DEFAULT, ArrowHead::NONE, true, true, true, true, false, 
                 false, true, true, false, true, false, false, false);
                 //no arrow, vline solid side vfill makeroom note;
    styles["box_collapsed"] = styles["box"];
    styles["emptybox"] = styles["box"];
    styles["--"] = styles["box"];
    styles["++"] = styles["box"];
    styles[".."] = styles["box"];
    styles["=="] = styles["box"];

    styles["pipe"] = 
        MscStyle(STYLE_DEFAULT, ArrowHead::NONE, true, true, true, true, false,
                 true, true, true, true, false, false, false, false); 
                 //no arrow, vline indicator vfill makeroom note;
    styles["pipe--"] = styles["pipe"];
    styles["pipe++"] = styles["pipe"];
    styles["pipe.."] = styles["pipe"];
    styles["pipe=="] = styles["pipe"];

    styles["entity"] = 
        MscStyle(STYLE_DEFAULT, ArrowHead::NONE, true, true, true, true, true, 
                 false, false, false, false, true, true, false, false); 
                 //no arrow, solid numbering compress side makeroom note
    styles["entitygroup_collapsed"] = styles["entity"];
    styles["entitygroup"] = styles["entity"];

    styles["indicator"] = 
        MscStyle(STYLE_DEFAULT, ArrowHead::NONE, false, true, true, true, false, 
                 false, false, false, false, false, false, false, false); 
                 //fill line shadow only 
    styles["symbol"] =
        MscStyle(STYLE_DEFAULT, ArrowHead::NONE, false, true, true, true, false, 
                 false, false, false, false, false, false, false, false); 
                 //only line fill and shadow

    styles["note"] = 
        MscStyle(STYLE_DEFAULT, ArrowHead::NOTE, true, true, true, true, false, 
                 false, true, false, false, false, false, false, true);  
                 //no vline side solid indicator compress vfill makreoom
    
    styles["comment"] = 
        MscStyle(STYLE_DEFAULT, ArrowHead::NONE, true, false, false, false, false, 
                 false, true, false, true, false, false, false, false);  
                 //only text numbering and side

    styles["title"] = 
        MscStyle(STYLE_DEFAULT, ArrowHead::NONE, true, true, true, true, true, 
                 false, false, false, false, false, false, false, false);  
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
    compress.first = true;
    compress.second=false;
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

    styles["arrow"].write().MakeCompleteButText();
    styles["arrow"].write().compress.first = false;
    styles["arrow"].write().numbering.first = false;
    styles["arrow"].write().line.radius.second = -1;

    styles["->"].write().line.type.first = true;
    styles["->"].write().line.type.second = LINE_SOLID;
    styles[">"].write().line.type.first = true;
    styles[">"].write().line.type.second = LINE_DOTTED;
    styles[">>"].write().line.type.first = true;
    styles[">>"].write().line.type.second = LINE_DASHED;
    styles["=>"].write().line.type.first = true;
    styles["=>"].write().line.type.second = LINE_DOUBLE;

    styles["blockarrow"].write().MakeCompleteButText();
    styles["blockarrow"].write().compress.first = false;
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
    styles["vertical"].write().compress.first = false;
    styles["vertical"].write().numbering.first = false;
    styles["vertical"].write().makeroom.second = false;
    styles["vertical"].write().line.radius.second = -1;

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
    styles["divider"].write().compress.first = false;
    styles["divider"].write().numbering.first = false;
    styles["divider"].write().vline.Empty();
    styles["divider"].write().line.type.second = LINE_NONE;

    styles["---"].write().line.type.first = true;
    styles["---"].write().line.type.second = LINE_DOTTED;
    styles["..."].write().vline.type.first = true;
    styles["..."].write().vline.type.second = LINE_DOTTED;
    styles["..."].write().text.Apply("\\mu(10)\\md(10)");

    styles["emptybox"].write().MakeCompleteButText();
    styles["emptybox"].write().compress.first = false;
    styles["emptybox"].write().numbering.first = false;
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
    styles["pipe"].write().compress.first = false;
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

    styles["title"].write().MakeCompleteButText();
    styles["title"].write().vline.type.second = LINE_NONE;
    styles["title"].write().line.type.second = LINE_NONE;
    styles["title"].write().fill.color.second = ColorType(0,0,0,0); //no fill
    styles["title"].write().text += "\\mn(28)\\ms(18)\\B";
    styles["subtitle"] = styles["title"];
    styles["subtitle"].write().text += "\\mn(22)\\ms(14)\\B";
    
    //Ok, now "weak" and "strong"
    MscStyle style = MscStyle(STYLE_STYLE); //has everything, but is empty
    LineAttr line(ColorType(150,150,150));
    style.line += line;;
    style.vline += line;
    style.arrow.line += line;
    style.text.SetColor(line.color.second);
	style.text.Apply("\\i");
    styles["weak"] = style;
    
    style.Empty();
    line.Empty();
    line.width.first = true;
    line.width.second = 2.0;
    style.line += line;
    style.vline += line;
    style.arrow.line += line;
    style.text.Apply("\\b");
    styles["strong"] = style;
}

