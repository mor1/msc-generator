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

#include "msc.h"

MscStyle::MscStyle(StyleType tt) : type(tt)
{
    f_line=f_vline=f_fill=f_vfill=f_shadow=f_text=true;
    f_solid=f_numbering=f_compress=f_side=f_indicator=true;
    f_makeroom=f_note=true;
    f_arrow=ArrowHead::ANY;
    Empty();
}

MscStyle::MscStyle(StyleType tt, ArrowHead::ArcType a, bool t, bool l, bool f, bool s, bool vl, 
                   bool so, bool nu, bool co, bool si, bool i, bool vf, bool mr, bool n) :
    arrow(a), type(tt), f_line(l), f_vline(vl), f_fill(f), f_vfill(vf), f_shadow(s),
    f_text(t), f_solid(so), f_numbering(nu), f_compress(co), f_side(si),
    f_indicator(i), f_makeroom(mr), f_note(n), f_arrow(a)
{
    solid.first=so;
    solid.second = 128;
    side.first = si;  
    side.second = SIDE_RIGHT;
    compress.first = co;
    compress.second = false;
    numbering.first = nu;
    numbering.second = false;
    indicator.first = i;
    indicator.second = true;
    makeroom.first = mr;
    makeroom.second = true;
}

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

template<> const char EnumEncapsulator<MscSideType>::names[][ENUM_STRING_LEN] =
    {"invalid", "left", "right", ""};


bool MscStyle::AddAttribute(const Attribute &a, Msc *msc)
{
    if (a.type == MSC_ATTR_STYLE) {
        const char *newname = a.name == "emphasis"?"box":"emptybox";
        if (a.name == "emphasis" || a.name == "emptyemphasis") {
            msc->Error.Warning(a.linenum_attr.start, "Stylename '" + a.name + "' is deprecated, using " + newname + " instead.");
            operator +=(msc->Contexts.back().styles[newname]);
            return true;
        }
        if (msc->Contexts.back().styles.find(a.name) == msc->Contexts.back().styles.end()) {
            a.InvalidStyleError(msc->Error);
            return true;
        }
        operator +=(msc->Contexts.back().styles[a.name]);
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

void MscStyle::AttributeNames(Csh &csh) const
{
    static const char names[][ENUM_STRING_LEN] =
    {"", "vline.color", "vline.type", "vline.width", "vline.radius", "vline.corner", ""};
    static const char names2[][ENUM_STRING_LEN] =
    {"", "vfill.color", "vfill.color2", "vfill.gradient", ""};

    if (f_line) MscLineAttr::AttributeNames(csh);
    if (f_fill) MscFillAttr::AttributeNames(csh);
    if (f_arrow!=ArrowHead::NONE) ArrowHead::AttributeNames(csh);
    if (f_shadow) MscShadowAttr::AttributeNames(csh);
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
    if (f_note) MscNoteAttr::AttributeNames(csh);
    csh.AddStylesToHints();
}

bool CshHintGraphicCallbackForSide(MscCanvas *canvas, CshHintGraphicParam p)
{
    if (!canvas) return false;
    const MscSideType t = (MscSideType)(int)p;
    std::vector<double> xPos(2); 
    xPos[0] = t==SIDE_LEFT ? 0 : HINT_GRAPHIC_SIZE_X*0.3;
    xPos[1] = t==SIDE_LEFT ? HINT_GRAPHIC_SIZE_X*0.7 : HINT_GRAPHIC_SIZE_X;
    MscLineAttr eLine(LINE_SOLID, MscColorType(0,0,0), 1, CORNER_NONE, 0);
    canvas->Clip(XY(HINT_GRAPHIC_SIZE_X*0.1,1), XY(HINT_GRAPHIC_SIZE_X-1, HINT_GRAPHIC_SIZE_Y-1));
    ArrowHead ah(ArrowHead::BIGARROW);
    ah.line += MscColorType(0,32,192); //blue-green
    ah.endType.second =   t==SIDE_LEFT ? MSC_ARROW_SOLID : MSC_ARROW_NONE;
    ah.startType.second = t!=SIDE_LEFT ? MSC_ARROW_SOLID : MSC_ARROW_NONE;
    ah.size.second = MSC_ARROWS_INVALID;
    MscShadowAttr shadow;
    MscFillAttr fill(ah.line.color.second.Lighter(0.7), GRADIENT_UP);
    std::vector<double> active(2,0.);
    ah.BigCalculateAndDraw(xPos, active, HINT_GRAPHIC_SIZE_Y*0.3, HINT_GRAPHIC_SIZE_Y*0.7, 
                           true, false, fill, shadow, *canvas);
    canvas->UnClip();
    return true;
}

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
        csh.AddToHints(EnumEncapsulator<MscSideType>::names, csh.HintPrefix(COLOR_ATTRVALUE), 
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

const MscStyle &StyleSet::GetStyle(const string &s) const
{
    const_iterator i = find(s);
    if (i==end()) return defaultStyle;
    else return i->second;
};

void Context::Empty()
{
    is_full = false;
    hscale.first = false;
    numbering.first = false;
    compress.first = false;
    indicator.first = false;
    slant_angle.first = false;
    defLCommentLine.Empty();
    defRCommentLine.Empty();
    defLCommentFill.Empty();
    defRCommentFill.Empty();
    defBackground.Empty();
    text.Empty();
    styles.clear();
    colors.clear();
    numberingStyle.Empty();  
}

Context &Context::operator +=(const Context &o)
{
    is_full |= o.is_full;
    if (o.hscale.first) hscale = o.hscale;
    if (o.numbering.first) numbering = o.numbering;
    if (o.compress.first) compress = o.compress;
    if (o.indicator.first) indicator = o.indicator;
    if (o.slant_angle.first) slant_angle = o.slant_angle;
    defLCommentLine += o.defLCommentLine;
    defRCommentLine += o.defRCommentLine;
    defLCommentFill += o.defLCommentFill;
    defRCommentFill += o.defRCommentFill;
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

void Context::Plain()
{
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
    defLCommentLine.MakeComplete();
    defRCommentLine.MakeComplete();
    defLCommentFill.MakeComplete();
    defRCommentFill.MakeComplete();
    defBackground.MakeComplete();
    defLCommentLine.width.second = 3;
    defRCommentLine.width.second = 3;
    defLCommentFill.color.second.a = 0; //fully transparent
    defRCommentFill.color.second.a = 0; //fully transparent
    defBackground.color.second.a = 0; //fully transparent
    numberingStyle.Reset();
    numberingStyle.pre.first = true;
    numberingStyle.pre.second.clear();
    numberingStyle.post.first = true;
    numberingStyle.post.second = ": ";
    text.Default();

    colors.clear();
    colors["none"]  = MscColorType(  0,   0,   0, 0);
    colors["black"] = MscColorType(  0,   0,   0);
    colors["white"] = MscColorType(255, 255, 255);
    colors["red"]   = MscColorType(255,   0,   0);
    colors["green"] = MscColorType(  0, 255,   0);
    colors["blue"]  = MscColorType(  0,   0, 255);
    colors["yellow"]= MscColorType(255, 255,   0);
    colors["gray"]  = MscColorType(150, 150, 150);
    colors["lgray"] = MscColorType(200, 200, 200);

    styles.clear();
    MscStyle style(STYLE_DEFAULT, ArrowHead::ARROW, true, true, false, false, false, false, true, true, false, true, false, false, false); //no fill, shadow, vline solid side vfill, makeroom, note
    style.compress.first = false;
    style.numbering.first = false;
    style.line.radius.second = -1;
    styles["arrow"] = style;

    style.Empty();
    style.type = STYLE_STYLE;
    style.line.type.first = true;
    style.line.type.second = LINE_SOLID;
    styles["->"] = style;
    style.line.type.second = LINE_DOTTED;
    styles[">"] = style;
    style.line.type.second = LINE_DASHED;
    styles[">>"] = style;
    style.line.type.second = LINE_DOUBLE;
    styles["=>"] = style;

    style= MscStyle(STYLE_DEFAULT, ArrowHead::BIGARROW, true, true, true, true, false, false, true, true, false, false, false, false, false);  //no vline solid side indicator vfill makeroom note
    style.compress.first = false;
    style.numbering.first = false;
    style.line.radius.second = -1;
    styles["blockarrow"] = style;
    style.arrow.midType.second = MSC_ARROW_DOT;
    styles["box_collapsed_arrow"] = style;

    style.Empty();
    style.type = STYLE_STYLE;
    style.line.type.first = true;
    style.line.type.second = LINE_SOLID;
    styles["block->"] = style;
    style.line.type.second = LINE_DOTTED;
    styles["block>"] = style;
    style.line.type.second = LINE_DASHED;
    styles["block>>"] = style;
    style.line.type.second = LINE_DOUBLE;
    styles["block=>"] = style;

    style= MscStyle(STYLE_DEFAULT, ArrowHead::BIGARROW, true, true, true, true, false, false, true, true, true, false, false, true, false);  //no vline solid indicator vfill note
    style.compress.first = false;
    style.numbering.first = false;
    style.makeroom.second = false;
    style.line.radius.second = -1;
    styles["vertical"] = style;

    style.Empty();
    style.type = STYLE_STYLE;
    style.line.type.first = true;
    style.line.type.second = LINE_SOLID;
    styles["vertical->"] = style;
    style.line.type.second = LINE_DOTTED;
    styles["vertical>"] = style;
    style.line.type.second = LINE_DASHED;
    styles["vertical>>"] = style;
    style.line.type.second = LINE_DOUBLE;
    styles["vertical=>"] = style;

    style.arrow.startType.first = true;
    style.arrow.startType.second = MSC_ARROW_NONE;
    style.arrow.midType.first = true;
    style.arrow.midType.second = MSC_ARROW_NONE;
    style.arrow.endType.first = true;
    style.arrow.endType.second = MSC_ARROW_NONE;
    style.line.type.second = LINE_SOLID;
    styles["vertical--"] = style;
    style.line.type.second = LINE_DASHED;
    styles["vertical++"] = style;
    style.line.type.second = LINE_DOTTED;
    styles["vertical.."] = style;
    style.line.type.second = LINE_DOUBLE;
    styles["vertical=="] = style;

    style = MscStyle(STYLE_DEFAULT, ArrowHead::NONE, true, true, false, false, true, false, true, true, false, false, false, false, false); //no arrow, fill, shadow solid side indicator vfill makeroom note
    style.compress.first = false;
    style.numbering.first = false;
    style.vline.Empty();
    style.line.type.second = LINE_NONE;
    styles["divider"] = style;

    style.Empty();
    style.type = STYLE_STYLE;
    style.line.type.first = true;
    style.line.type.second = LINE_DOTTED;
    styles["---"] = style;
    style.Empty();
    style.vline.type.first = true;
    style.vline.type.second = LINE_DOTTED;
    style.text.Apply("\\mu(10)\\md(10)");
    styles["..."] = style;

    style = MscStyle(STYLE_DEFAULT, ArrowHead::NONE, true, true, true, true, false, false, true, true, false, true, false, false, false); //no arrow, vline solid side vfill makeroom note
    style.compress.first = false;
    style.numbering.first = false;
    styles["emptybox"] = style;
    styles["box_collapsed"] = style;
    style.text.Apply("\\pl");
    style.line.type.first = false;
    styles["box"] = style;

    style.Empty();
    style.type = STYLE_STYLE;
    style.line.type.first = true;
    style.line.type.second = LINE_SOLID;
    styles["--"] = style;
    style.line.type.second = LINE_DASHED;
    styles["++"] = style;
    style.line.type.second = LINE_DOTTED;
    styles[".."] = style;
    style.line.type.second = LINE_DASHED;
    style.line.type.second = LINE_DOUBLE;
    styles["=="] = style;

    style = MscStyle(STYLE_DEFAULT, ArrowHead::NONE, true, true, true, true, false, true, true, true, true, false, false, false, false); //no arrow, vline indicator vfill makeroom note
    style.compress.first = false;
    style.numbering.first = false;
    style.line.radius.second = 5;
    styles["pipe"] = style;

    style.Empty();
    style.type = STYLE_STYLE;
    style.line.type.first = true;
    style.line.type.second = LINE_SOLID;
    styles["pipe--"] = style;
    style.line.type.second = LINE_DASHED;
    styles["pipe++"] = style;
    style.line.type.second = LINE_DOTTED;
    styles["pipe.."] = style;
    style.line.type.second = LINE_DASHED;
    style.line.type.second = LINE_DOUBLE;
    styles["pipe=="] = style;


    style = MscStyle(STYLE_DEFAULT, ArrowHead::NONE, true, true, true, true, true, false, false, false, false, true, true, false, false); //no arrow, solid numbering compress side makeroom note
    styles["entity"] = style;
    styles["entitygroup_collapsed"] = style;
    style.line.type.second=LINE_DASHED;
    styles["entitygroup"] = style;

    style = MscStyle(STYLE_DEFAULT, ArrowHead::NONE, false, true, true, true, false, false, false, false, false, false, false, false, false); //fill line shadow only 
    style.line.width.second = 2;
    styles["indicator"] = style;

    style = MscStyle(STYLE_DEFAULT, ArrowHead::NONE, false, true, true, true, false, false, false, false, false, false, false, false, false); //only line fill and shadow
    styles["symbol"] = style;

    style= MscStyle(STYLE_DEFAULT, ArrowHead::NOTE, true, true, true, true, false, false, true, false, false, false, false, false, true);  //no vline side solid indicator compress vfill makreoom
    style.numbering.first = false;
    style.text += "\\mn(10)\\ms(6)"; //small font to 6, normal to 10
    styles["note"] = style;

    style= MscStyle(STYLE_DEFAULT, ArrowHead::NONE, true, false, false, false, false, false, true, false, true, false, false, false, false);  //only text numbering and side
    style.numbering.first = false;
    style.text += "\\mn(10)\\ms(6)\\pl"; //small font to 6, normal to 10, left para
    styles["comment"] = style;


    //Ok, now "weak" and "strong"
    style = MscStyle(STYLE_STYLE); //has everything, but is empty
    MscLineAttr line(MscColorType(150,150,150));
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

