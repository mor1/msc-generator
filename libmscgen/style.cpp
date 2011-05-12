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

#include "msc.h"

MscStyle::MscStyle(StyleType tt) : type(tt)
{
    f_line=f_vline=f_fill=f_vfill=f_shadow=f_text=f_solid=f_numbering=f_compress=f_side=f_indicator=true;
    f_arrow=ArrowHead::ANY;
    Empty();
}

MscStyle::MscStyle(StyleType tt, ArrowHead::ArcType a, bool t, bool l, bool f, bool s, bool vl, 
                   bool so, bool nu, bool co, bool si, bool i, bool vf) :
    type(tt), f_arrow(a), f_text(t), f_line(l), f_vline(vl), f_fill(f), f_vfill(vf),
    f_shadow(s), f_solid(so), f_numbering(nu), f_compress(co), f_side(si),
    f_indicator(i), arrow(a)
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
    if ((a.StartsWith("text") || a.Is("ident")) && f_text)
        return text.AddAttribute(a, msc, type);
    if (a.StartsWith("line") && f_line)
        return line.AddAttribute(a, msc, type);
    if (a.StartsWith("vline") && f_vline)
        return vline.AddAttribute(a, msc, type);
    if (a.StartsWith("fill") && f_fill)
        return fill.AddAttribute(a, msc, type);
    if (a.StartsWith("vfill") && f_vfill)
        return vfill.AddAttribute(a, msc, type);
    if (a.StartsWith("shadow") && f_shadow)
        return shadow.AddAttribute(a, msc, type);
    if ((a.StartsWith("arrow") || a.Is("arrowsize")) && f_arrow!=ArrowHead::NONE)
        return arrow.AddAttribute(a, msc, type);
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
    return false;
}

void MscStyle::AttributeNames(Csh &csh) const
{
    static const char names[][ENUM_STRING_LEN] =
    {"vline.color", "vline.type", "vline.width", "vline.radius", ""};
    static const char names2[][ENUM_STRING_LEN] =
    {"vfill.color", "vfill.color2", "vfill.gradient", ""};

    if (f_line) MscLineAttr::AttributeNames(csh);
    if (f_fill) MscFillAttr::AttributeNames(csh);
    if (f_arrow!=ArrowHead::NONE) ArrowHead::AttributeNames(csh);
    if (f_shadow) MscShadowAttr::AttributeNames(csh);
    if (f_text) StringFormat::AttributeNames(csh);
    if (f_solid) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"solid", HINT_ATTR_NAME));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"side", HINT_ATTR_NAME));
    }
    if (f_numbering) csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"number", HINT_ATTR_NAME));
    if (f_indicator) csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"indicator", HINT_ATTR_NAME));
    if (f_compress) csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"compress", HINT_ATTR_NAME));
    if (f_vline) csh.AddToHints(names, csh.HintPrefix(COLOR_ATTRNAME), HINT_ATTR_NAME);
    if (f_vfill) csh.AddToHints(names2, csh.HintPrefix(COLOR_ATTRNAME), HINT_ATTR_NAME);
    csh.AddStylesToHints();
}

bool CshHintGraphicCallbackForSide(MscCanvas *canvas, CshHintGraphicParam p)
{
    if (!canvas) return false;
    const MscSideType t = (MscSideType)(int)p;
    const double xx = 0.7;
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
    ah.BigDraw(xPos, active, HINT_GRAPHIC_SIZE_Y*0.3, HINT_GRAPHIC_SIZE_Y*0.7, false, shadow, fill, NULL, canvas);
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
        (CaseInsensitiveEqual(attr, "indicator") && f_indicator)) {
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
    return false;

}

string MscStyle::Print(int ident) const
{
    string s = "style:(";
    if (f_line) s.append(line.Print());
    if (f_vline) s.append(vline.Print());
    if (f_fill) s.append(fill.Print());
    if (f_vfill) s.append(vfill.Print());
    if (f_shadow) s.append(shadow.Print());
    if (f_solid) s.append("solid:").append(solid.second?"yes":"no").append("\n");
    if (f_solid) s.append("fromright:").append(side.second==SIDE_LEFT?"left":"right").append("\n");
//    if (f_arrow!=ArrowHead::NONE) s.append(arrow.Print());
//    if (f_text) s.append(text.Print());
    s.append(")");
    return s;
}

const MscStyle &StyleSet::GetStyle(const string &s) const
{
    const_iterator i = find(s);
    if (i==end()) return defaultStyle;
    else return i->second;
};

void Design::Reset()
{
    numbering=false;
    compress=false;
    numberingStyle.Reset();
    numberingStyle.post = ": ";
    hscale = 1.0;

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
    MscStyle style(STYLE_DEFAULT, ArrowHead::ARROW, true, true, false, false, false, false, true, true, false, true, false); //no fill, shadow, vline solid side vfill
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

    style= MscStyle(STYLE_DEFAULT, ArrowHead::BIGARROW, true, true, true, true, false, false, true, true, false, false, false);  //no vline solid side indicator vfill
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

    style= MscStyle(STYLE_DEFAULT, ArrowHead::BIGARROW, true, true, true, true, false, false, true, true, true, false, false);  //no vline solid indicator vfill
    style.compress.first = false;
    style.numbering.first = false;
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

    style = MscStyle(STYLE_DEFAULT, ArrowHead::NONE, true, true, false, false, true, false, true, true, false, false, false); //no arrow, fill, shadow solid side indicator vfill
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

    style = MscStyle(STYLE_DEFAULT, ArrowHead::NONE, true, true, true, true, false, false, true, true, false, true, false); //no arrow, vline solid side vfill
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

    style = MscStyle(STYLE_DEFAULT, ArrowHead::NONE, true, true, true, true, false, true, true, true, true, false, false); //no arrow, vline indicator vfill
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


    style = MscStyle(STYLE_DEFAULT, ArrowHead::NONE, true, true, true, true, true, false, false, false, false, true, true); //no arrow, solid numbering compress side
    styles["entity"] = style;
    styles["entitygroup_collapsed"] = style;
    style.line.type.second=LINE_DASHED;
    styles["entitygroup"] = style;

    style = MscStyle(STYLE_DEFAULT, ArrowHead::NONE, false, true, true, true, false, false, false, false, false, false, false); //fill line shadow only 
    style.line.width.second = 2;
    styles["indicator"] = style;

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

