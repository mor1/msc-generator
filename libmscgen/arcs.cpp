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
#include <math.h>
#include <cassert>
#include <iostream>
#include <algorithm>
#include "msc.h"

using namespace std;


void TrackableElement::SetLineEnd(file_line_range l, bool f)
{
    if (linenum_final) return;
    linenum_final = f;
    file_pos = l;
}

void TrackableElement::ShiftBy(double y) 
{
    if (y==0) return;
    area.Shift(XY(0, y)); 
    area_draw.Shift(XY(0, y)); 
    yPos+=y;
}


//template class PtrList<ArcBase>;

ArcBase::ArcBase(MscArcType t, Msc *msc) :
    type(t), chart(msc), valid(true), compress(false), parallel(false)
{
    if (msc) compress = msc->Contexts.back().compress;
}

//Helper function. If the pos of *value is smaller (or larger) than i
//if one of the elements is .end() always the other is returned, different
//from operator < above (where .end() is smaller)
EIterator ArcBase::MinMaxByPos(EIterator i, EIterator j, bool min)
{
    if (j==chart->NoEntity) return i;
    if (i==chart->NoEntity) return j;
    if (min ^ ((*i)->pos < (*j)->pos))
        return j;
    else
        return i;
};

//l can be an empty list
ArcBase* ArcBase::AddAttributeList(AttributeList *l)
{
    if (l==NULL) return this;
    for (AttributeList::iterator i=l->begin(); i!=l->end(); i++)
        AddAttribute(**i);
    delete l;
    return this;
}

bool ArcBase::AddAttribute(const Attribute &a)
{
    //In case of ArcLabelled this will not be called, for a compress attribute.
    //There the style.AddAtribute will process any compress attribute.
    //Then in ArcLabelled::PostParseProcess
    //we copy style.compress.second to the ArcBase::compress.
    if (a.Is("compress")) {
        if (!a.EnsureNotClear(chart->Error, STYLE_ARC)) return true;
        if (!a.CheckType(MSC_ATTR_BOOL, chart->Error)) return true;
        compress = a.yes;
        return true;
    }
    if (a.Is("parallel")) {
        if (!a.EnsureNotClear(chart->Error, STYLE_ARC)) return true;
        if (!a.CheckType(MSC_ATTR_BOOL, chart->Error)) return true;
        parallel = a.yes;
        return true;
    }
    return false;
}

void ArcBase::AttributeNames(Csh &csh)
{
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "compress", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "parallel", HINT_ATTR_NAME));
}

bool ArcBase::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr,"compress")||
        CaseInsensitiveEqual(attr,"parallel")) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "yes", HINT_ATTR_VALUE));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "no", HINT_ATTR_VALUE));
        return true;
    }
    return false;
}

string ArcBase::PrintType(void) const
{
    static const char arcnames[][25] = {
        "invalid_arc_type",
        "arc_solid", "arc_solid_bidir", "arc_dotted", "arc_dotted_bidir",
        "arc_dashed", "arc_dashed_bidir", "arc_double", "arc_double_bidir",
        "emph_solid", "emph_dotted", "emph_double", "arc_parallel",
        "arc_disco", "arc_divider", "arc_vspace",
        "command_heading", "command_nudge", "command_entity"};
    return arcnames[int(type)-1];
}

void ArcBase::PostPosProcess(double autoMarker)
{
    if (valid)
        chart->AllCovers += area;
    chart->AllArcs[file_pos] = this;
}


//////////////////////////////////////////////////////////////////////////////////////

//Take numbering style from the current context
ArcLabelled::ArcLabelled(MscArcType t, Msc *msc, const MscStyle &s) :
    ArcBase(t, msc), style(s), numberingStyle(msc->Contexts.back().numberingStyle),
	parsed_label(msc), concrete_number(-1)
{
    style.type = STYLE_ARC;
    //If style does not contain a numbering setting, apply the value of the
    //current chart option.
    if (!style.numbering.first) {
        style.numbering.first = true;
        style.numbering.second = msc->Contexts.back().numbering;
    }
    switch(type) {
    case MSC_ARC_SOLID:
    case MSC_ARC_SOLID_BIDIR:
		style += msc->Contexts.back().styles["->"]; break;
    case MSC_ARC_DOTTED:
    case MSC_ARC_DOTTED_BIDIR:
        style += msc->Contexts.back().styles[">"]; break;
    case MSC_ARC_DASHED:
    case MSC_ARC_DASHED_BIDIR:
        style += msc->Contexts.back().styles[">>"]; break;
    case MSC_ARC_DOUBLE:
    case MSC_ARC_DOUBLE_BIDIR:
        style += msc->Contexts.back().styles["=>"]; break;
    case MSC_EMPH_SOLID:
        style += msc->Contexts.back().styles["--"]; break;
    case MSC_EMPH_DASHED:
        style += msc->Contexts.back().styles["++"]; break;
    case MSC_EMPH_DOTTED:
        style += msc->Contexts.back().styles[".."]; break;
    case MSC_EMPH_DOUBLE:
        style += msc->Contexts.back().styles["=="]; break;
    case MSC_EMPH_UNDETERMINED_FOLLOW:
        break; /*do nothing*/
    case MSC_ARC_DIVIDER:
        style += msc->Contexts.back().styles["---"]; break;
    case MSC_ARC_DISCO:
        style += msc->Contexts.back().styles["..."]; break;
    };
}

ArcBase *ArcLabelled::AddAttributeList(AttributeList *l)
{
    if (l==NULL) return this;
    //Find label attribute
    file_line linenum_label;
    for (AttributeList::iterator i = l->begin(); i!=l->end(); i++)
        if ((*i)->Is("label")) linenum_label = (*i)->linenum_value.start;
    //Add attributest first
    ArcBase::AddAttributeList(l);
    //Then convert color and style names in labels
    if (label.length()>0)
        StringFormat::ExpandColorAndStyle(label, chart, linenum_label, &style.text,
                                          true, StringFormat::LABEL);
    return this;
}

bool ArcLabelled::AddAttribute(const Attribute &a)
{
    if (a.type == MSC_ATTR_STYLE) {
        style.AddAttribute(a, chart);
        return true;
    }
    string s;
    if (a.Is("color")) {
        // MSC_ATTR_CLEAR is handled by individual attributes below
        bool was = false;
        if (style.f_line) {
            style.line.AddAttribute(a, chart, style.type);
            was = true;
        }
        if (style.f_text) {
            style.text.AddAttribute(a, chart, style.type);
            was = true;
        }
        if (style.f_arrow) {
            style.arrow.AddAttribute(a, chart, style.type);
            was = true;
        }
        return was;
    }
    if (a.Is("label")) {
        if (!a.CheckType(MSC_ATTR_STRING, chart->Error)) return true;
        //MSC_ATTR_CLEAR is OK above with value = ""
        label = a.value;
        return true;
    }
    if (a.Is("number")) {
        if (a.type == MSC_ATTR_NUMBER) {
            if (a.number >= 0) {
                concrete_number = int(a.number);
                style.numbering.first = true;
                style.numbering.second = a.yes;
            } else
                chart->Error.Error(a, true, "Value for 'number' must not be negative. Ignoring attribute.");
            return true;
        }
        if (a.type == MSC_ATTR_BOOL) {
            style.numbering.first = true;
            style.numbering.second = a.yes;
            return true;
        }
        if (a.type == MSC_ATTR_CLEAR) { //turn off numbering
            style.numbering.first = true;
            style.numbering.second = false;
            return true;
        }
        //We have a string as number - it may be a roman number or abc
        int num;
        int off = chart->Contexts.back().numberingStyle.Last().Input(a.value, num);
        //off is how many characters we could not understand at the end of a.value
        if (off == a.value.length()) {
            //No characters understood
            chart->Error.Error(a, true, "Value for 'number' must be 'yes', 'no' or a number. Ignoring attribute.");
            return true;
        }
        if (off > 0) {
            file_line l(a.linenum_value.start);
            l.col += a.value.length() - off;
            chart->Error.Warning(l, "I could not understand number from here. Applying only '" +
                                 a.value.substr(0, a.value.length() - off) + "'.");
        }
        concrete_number = num;
        style.numbering.first = true;
        style.numbering.second = true;
        return true;
    }
    if (a.Is("id")) {
        chart->Error.Error(a, false, "Attribute '" + a.name
                           + "' is no longer supported. Ignoring it.",
                          "Try '\\^' inside a label for superscript.");
        return true;
    }
    if (style.AddAttribute(a, chart))
        return true;
    if (ArcBase::AddAttribute(a)) return true;
    a.InvalidAttrError(chart->Error);
    return false;
}

void ArcLabelled::AttributeNames(Csh &csh)
{
    ArcBase::AttributeNames(csh);
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "color", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "label", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "number", HINT_ATTR_NAME));
    csh.AddStylesToHints();
}

bool ArcLabelled::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr,"color")) {
        csh.AddColorValuesToHints();
        return true;
    }
    if (CaseInsensitiveEqual(attr,"label")) {
        return true;
    }
    if (CaseInsensitiveEqual(attr,"number")) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE) + "yes", HINT_ATTR_VALUE));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE) + "no", HINT_ATTR_VALUE));
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", HINT_ATTR_VALUE, false));
        return true;
    }
    if (ArcBase::AttributeValues(attr, csh)) return true;
    return false;
}

string ArcLabelled::Print(int ident) const
{
    string ss;
    ss << string(ident*2, ' ');
    ss << PrintType().append(":").append(label);
    return ss;
}

void ArcLabelled::PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level)
{
    at_top_level = top_level;
    string num;
    string pre_num_post;
    if (label.length()!=0) {
        if (style.numbering.second) {
            number.SetSize(numberingStyle.Size()); //append 1s if style has more levels
            if (concrete_number >= 0)
                number.Last() = concrete_number;
            num = numberingStyle.Print(number);
            pre_num_post = numberingStyle.pre + num + numberingStyle.post;
            ++number;
            //Recreate the text style at the point where the label will be inserted
            StringFormat basic = style.text;
            basic.Apply(label.c_str());
            //At this point the number text must be processed using StringFormat::ExpandColorAndStyle
            //to expand remaining empty \c(), \s(), etc escapes.
            //We use a dummy linenum, as we should not get ANY errors here...
            StringFormat::ExpandColorAndStyle(num, chart, file_line(), &basic,
                                              true, StringFormat::LABEL);
            StringFormat::ExpandColorAndStyle(pre_num_post, chart, file_line(), &basic,
                                              true, StringFormat::LABEL);
        }
        //We add empty num and pre_num_post if numberin is turned off, to remove \N escapes
        StringFormat::AddNumbering(label, num, pre_num_post);
    }
    parsed_label.Set(label, style.text);
    if (style.compress.first)
        compress = style.compress.second;
}

void ArcArrow::AttributeNames(Csh &csh)
{
    ArcLabelled::AttributeNames(csh);
    MscStyle style(STYLE_DEFAULT, ArrowHead::ARROW, true, true, false, false, false, false, true, true); //no fill, shadow, vline solid
    style.AttributeNames(csh);
}

bool ArcArrow::AttributeValues(const std::string attr, Csh &csh)
{
    MscStyle style(STYLE_DEFAULT, ArrowHead::ARROW, true, true, false, false, false, false, true, true); //no fill, shadow, vline solid
    if (style.AttributeValues(attr, csh)) return true;
    if (ArcLabelled::AttributeValues(attr, csh)) return true;
    return false;
}
//////////////////////////////////////////////////////////////////////////////////////

ArcSelfArrow::ArcSelfArrow(MscArcType t, const char *s, file_line_range sl,
                           Msc *msc, const MscStyle &st, double ys) :
    ArcArrow(t, msc, st), YSize(ys)
{
    src = chart->FindAllocEntity(s, sl, &valid);
}

ArcArrow * ArcSelfArrow::AddSegment(const char *m, file_line_range ml, bool forward, file_line_range l)
{
    if (!valid) return this; //display error only once
    chart->Error.Error(l.start, "Cannot add further segments to arrow pointing to the same entity. Ignoring arrow.");
    valid = false;
    return this;
}

string ArcSelfArrow::Print(int ident) const
{
    string ss;
    ss << string(ident*2, ' ');
    ss << (*src)->name << (isBidir()?"<->":"->") << (*src)->name;
    if (label.length()>0) ss << ": \"" << label << "\"";
    return ss;
};

void ArcSelfArrow::PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level)
{
    //Add numbering, if needed
    ArcLabelled::PostParseProcess(left, right, number, top_level);

    left = MinMaxByPos(left, src, true);    //true=Min
    right = MinMaxByPos(right, src, false); //false=Max
}

void ArcSelfArrow::Width(EntityDistanceMap &distances)
{
    distances.Insert((*src)->index, DISTANCE_RIGHT, chart->XCoord(0.375));
    distances.Insert((*src)->index, DISTANCE_LEFT, parsed_label.getTextWidthHeight().x);
}

double ArcSelfArrow::Height(AreaList &cover) 
{
    yPos = 0;
    xy_s = style.arrow.getWidthHeight(isBidir(), MSC_ARROW_START);
    xy_e = style.arrow.getWidthHeight(isBidir(), MSC_ARROW_END);
    xy_s.y = ceil(xy_s.y);
    xy_e.y = ceil(xy_e.y);
    XY wh(ceil(chart->XCoord(0.375)), ceil(2*YSize));
    dx = chart->XCoord((*src)->pos);
    sx = 0;

    double y = chart->arcVGapAbove;
    area = parsed_label.Cover(sx, dx, y);
    area += Block(dx, ceil(dx+wh.x), y, ceil(y+xy_s.y+wh.y+xy_e.y));
    area.mainline = Range(y - chart->nudgeSize/2, y + wh.y + chart->nudgeSize/2);

    cover = area;
    return area.GetBoundingBox().y.till + chart->arcVGapBelow;
}

void ArcSelfArrow::PostPosProcess(double autoMarker)
{
    ArcBase::PostPosProcess(autoMarker);

    //Check if the entity involved is actually turned on.
    if (!(*src)->status.GetStatus(yPos)) {
        string sss;
        sss << "Entity '" << (*src)->name << "' is";
        sss << " turned off, but referenced here.";
        chart->Error.Warning(file_pos.start, sss, "It will look strange.");
    }
}

void ArcSelfArrow::Draw()
{
    double y = yPos + chart->arcVGapAbove;

    parsed_label.Draw(sx, dx, y);
    y += xy_s.y;

    if (style.line.radius.second < 0) {
        //draw an arc
        wh.x *=2;
        chart->Line(Edge(XY(dx, y+YSize), wh.x, wh.y, 270, 90), style.line);
    } else {
        //draw (part of) a rounded rectangle
        chart->Clip(dx, chart->total.x, 0, chart->total.y);
        chart->Line(Block(XY(0, y), XY(dx,y)+wh), style.line);
        chart->UnClip();
    }
    //draw arrowheads
    style.arrow.Draw(XY(dx, y+2*YSize), false, isBidir(), MSC_ARROW_END, chart);
    style.arrow.Draw(XY(dx, y        ), true,  isBidir(), MSC_ARROW_START, chart);
}

//////////////////////////////////////////////////////////////////////////////////////

ArcDirArrow::ArcDirArrow(MscArcType t, const char *s, file_line_range sl,
                         const char *d, file_line_range dl, Msc *msc, const MscStyle &st) :
    ArcArrow(t, msc, st), linenum_src(sl.start), linenum_dst(dl.start)
{
    src = chart->FindAllocEntity(s, sl, &valid);
    dst = chart->FindAllocEntity(d, dl, &valid);
    modifyFirstLineSpacing = true;
};

ArcArrow * ArcDirArrow::AddSegment(const char *m, file_line_range ml, bool forward, file_line_range l)
{
    EIterator mid;
    if (m==NULL) {
        if (((*src)->pos < (*dst)->pos) ^ forward)
            // b->a-> and b<-a<-
            mid = chart->FindAllocEntity(LSIDE_ENT_STR, ml);
        else
            // a->b-> and a<-b<-
            mid = chart->FindAllocEntity(RSIDE_ENT_STR, ml);
    } else
        mid = chart->FindAllocEntity(m, ml);
    assert(mid != chart->NoEntity);
    if (forward) {
        //check for this situation: ->b->a (where a is left of b)
        if (middle.size()==0 && (*src)->name ==LSIDE_ENT_STR &&
            (*dst)->pos > (*mid)->pos && m!=NULL)
            src = chart->FindAllocEntity(RSIDE_ENT_STR, ml);
        middle.push_back(dst);
        linenum_middle.push_back(linenum_dst);
        dst = mid;
        linenum_dst = ml.start;
    } else {
        //check for this situation: <-b<-a (where a is left of b)
        if (middle.size()==0 && (*dst)->name ==LSIDE_ENT_STR &&
            (*src)->pos > (*mid)->pos && m!=NULL)
            dst = chart->FindAllocEntity(RSIDE_ENT_STR, ml);
        middle.insert(middle.begin(), src);
        linenum_middle.insert(linenum_middle.begin(), linenum_src);
        src = mid;
        linenum_src = ml.start;
    };
    return this;
}

string ArcDirArrow::Print(int ident) const
{
    string ss;
    ss << string(ident*2, ' ');
    const char *arrow_string = isBidir()?"<->":"->";
    ss << (*src)->name << arrow_string;
    for (unsigned i=0; i<middle.size(); i++)
        ss << (*(middle[i]))->name << arrow_string;
    ss << (*dst)->name;
    if (label.length()>0) ss << ": \"" << label << "\"";
    return ss;
};

#define ARROW_TEXT_VSPACE_ABOVE 1
#define ARROW_TEXT_VSPACE_BELOW 1

void ArcDirArrow::PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level)
{
    //Add numbering, if needed
    ArcLabelled::PostParseProcess(left, right, number, top_level);

    const EIterator _left = ++chart->Entities.begin(); // leftmost entity;
    const EIterator _right = --chart->Entities.end();  // rightmost entiry
    const EIterator our_left =  MinMaxByPos(src, dst, true);  //true=Min
    const EIterator our_right = MinMaxByPos(src, dst, false); //false=Max

    //Change left and right only if they actually point to a "real entity"
    //and not (left) or (right). If they do, consider our other "end"
    if (our_left != _left)
        left = MinMaxByPos(left, our_left, true);
    else
        left = MinMaxByPos(left, our_right, true);

    if (our_right != _right)
        right = MinMaxByPos(right, our_right, false);
    else
        right = MinMaxByPos(right, our_left, false);

    //Insert a small extra spacing for the arrow line
    if (parsed_label.getTextWidthHeight().y && modifyFirstLineSpacing)
        parsed_label.AddSpacing(0, style.line.LineWidth()+
                                ARROW_TEXT_VSPACE_ABOVE+ARROW_TEXT_VSPACE_BELOW);

    //OK, now we check if arrow will display correctly
    if (middle.size()==0) return; //nothing to check

    const bool dir = (*src)->pos < (*dst)->pos; //true for ->, false for <-
    if (dir != ((*src)->pos < ((*middle[0])->pos))) goto problem;
    for (unsigned i=0; i<middle.size()-1; i++)
        if (dir != ((*middle[i])->pos < (*middle[i+1])->pos)) goto problem;
    if (dir != (((*middle[middle.size()-1])->pos < (*dst)->pos))) goto problem;
    return; // no problem

    problem:
        const char *arrow_string = isBidir()?"<->":dir?"->":"<-";
        string ss;
        EntityList e(false);
        ss << "Multi-segment arrow specified as ";
        e.push_back(*src);
        for (int f = 0; f<middle.size(); f++)
             e.push_back(*(middle[f]));
        e.push_back(*dst);
        if (!dir) e.reverse();
        EntityList::iterator ii = e.begin();
        ss << (*ii++)->name;
        while (ii!=e.end())
            ss << arrow_string << (*ii++)->name;
        ss << ", but entity order is ";
        e.SortByPos();
        ii = e.begin();
        ss << (*ii++)->name;
        while (ii!=e.end())
            ss << arrow_string << (*ii++)->name;
        ss << ". Ignoring arc.";
        valid = false;
		chart->Error.Error(file_pos.start, ss);
}

void ArcDirArrow::Width(EntityDistanceMap &distances)
{
    //we lie about us being forward (we do not check), so we know which of first/second to use
    DoublePair end = style.arrow.getWidths(true, isBidir(), MSC_ARROW_END, false, style.line.LineWidth());
    DoublePair start = style.arrow.getWidths(true, isBidir(), MSC_ARROW_START, false, style.line.LineWidth());
    distances.Insert((*src)->index, (*dst)->index,
                     end.first + start.second + parsed_label.getTextWidthHeight().x);
    //Add distances for arrowheads
    const bool fw = (*src)->index  <  (*dst)->index;
    distances.Insert((*src)->index, fw ? DISTANCE_LEFT : DISTANCE_RIGHT, start.first);
    distances.Insert((*dst)->index, fw ? DISTANCE_RIGHT : DISTANCE_LEFT, end.second);

    if (middle.size()==0) return;
    EntityDistanceMap d;
    for (int i=0; i<middle.size(); i++) {
        DoublePair mid = style.arrow.getWidths(true, isBidir(), MSC_ARROW_MIDDLE, false, style.line.LineWidth());
        distances.Insert((*middle[i])->index, fw ? DISTANCE_LEFT : DISTANCE_RIGHT, start.first);
        distances.Insert((*middle[i])->index, fw ? DISTANCE_RIGHT : DISTANCE_LEFT, end.second);
    }
    d.CombineLeftRightToPair_Sum(chart->hscaleAutoXGap);
    distances += d;
}

MscArrowEnd ArcDirArrow::WhichArrow(int i)
{
    if (i>0 && i<xPos.size()-1) return MSC_ARROW_MIDDLE;
    if ((i==0) == (sx<dx)) return MSC_ARROW_START;
    return MSC_ARROW_END;
}

double ArcDirArrow::Height(AreaList &cover)
{
    yPos = 0;
    area.clear();
    sx = chart->XCoord((*src)->pos);
    dx = chart->XCoord((*dst)->pos);

    const double lw = style.line.LineWidth();
    const XY xy_e = style.arrow.getWidthHeight(isBidir(), MSC_ARROW_END);
    const XY xy_s = style.arrow.getWidthHeight(isBidir(), MSC_ARROW_START);
    //If there are middle arrows, make aH be the highest of endType/startType
    //and midType arrows.
    //If not use endType/startType only
    double aH = max(xy_e.y, xy_s.y);
    if (middle.size()>0)
        aH = max(aH, style.arrow.getWidthHeight(isBidir(), MSC_ARROW_MIDDLE).y);

    double y = chart->arcVGapAbove;
    const XY text_wh = parsed_label.getTextWidthHeight();
    if (text_wh.y) {
        double firstLineHeight = parsed_label.getTextWidthHeight(0).y;
        double furtherLinesHeight = parsed_label.getTextWidthHeight().y - firstLineHeight;

        //Determine coverage for the text (or draw it if we already draw)
        if (sx<dx) {
            sx_text = sx + xy_s.x;
            dx_text = dx - xy_e.x;
        } else {
            sx_text = dx + xy_s.x;
            dx_text = sx - xy_e.x;
        }
        area += text_cover = parsed_label.Cover(sx_text, dx_text, y);
        //determine top edge position of arrow midline
        y += std::max(aH, firstLineHeight+ARROW_TEXT_VSPACE_ABOVE);
    } else {
        /* no text */
        y += aH;
    }
    centerline = y = ceil(y) + lw/2; 
    //prepare xPos and margins
    xPos.clear(); xPos.reserve(2+middle.size());
    margins.clear(); margins.reserve(2+middle.size());
    xPos.push_back(sx);
    margins.push_back(style.arrow.getWidths(sx<dx, isBidir(), MSC_ARROW_START, true, lw));
    for (int i=0; i<middle.size(); i++) {
        xPos.push_back(chart->XCoord((*middle[i])->pos));
        margins.push_back(style.arrow.getWidths(sx<dx, isBidir(), MSC_ARROW_MIDDLE, true, lw));
    }
    xPos.push_back(dx);
    margins.push_back(style.arrow.getWidths(sx<dx, isBidir(), MSC_ARROW_END, true, lw));
    if (sx>=dx) {
        std::reverse(xPos.begin(), xPos.end());
        std::reverse(margins.begin(), margins.end());
    }

    //Add arrowheads and line segments to cover
    for (int i=0; i<xPos.size(); i++) 
        area += style.arrow.Cover(XY(xPos[i], y), sx<dx, isBidir(), WhichArrow(i), lw);
    for (int i=0; i<xPos.size()-1; i++) 
        //x coordinates below are not integer- but this will be merged with other contours - so they disappear
        area += Block(xPos[i]+margins[i].second, xPos[i+1]-margins[i+1].first, y-ceil(lw/2), y+ceil(lw/2)); 
    //set mainline - not much dependent on main line with
    area.mainline = Range(y - (chart->nudgeSize+1)/2, y + (chart->nudgeSize+1)/2);
    cover = area;
    return std::max(y+max(aH, lw/2), chart->arcVGapAbove + text_wh.y) + chart->arcVGapBelow;
}

void ArcDirArrow::ShiftBy(double y)
{
    if (y) text_cover.Shift(XY(0, y));
    ArcArrow::ShiftBy(y);
}

void ArcDirArrow::CheckSegmentOrder(double y)
{
    if (!valid) return;

    //Check if all entities involved are actually turned on.
    //we can only do this check here, as entity status is filled
    //During the Height() process, and can be considered complete only here
    vector<EIterator> temp = middle;
    vector<file_line> linenum_temp = linenum_middle;
    temp.insert(temp.begin(), src);
    linenum_temp.insert(linenum_temp.begin(), linenum_src);
    if (src!=dst) {
        temp.push_back(dst);
        linenum_temp.push_back(linenum_dst);
    }
    std::vector<string> ss;
    int earliest = -1;
    for (int i = 0; i<temp.size(); i++)
        if (!(*temp[i])->status.GetStatus(y) &&
            (*temp[i])->name != LSIDE_ENT_STR &&
            (*temp[i])->name != RSIDE_ENT_STR) {
            ss.push_back("'" + (*temp[i])->name + "'");
            if (earliest == -1 || linenum_temp[i] < linenum_temp[earliest]) earliest = i;
        }
    if (ss.size()>0) {
        string sss;
        if (ss.size() == 1)  //One missing entity
            sss << "Entity" << ss[0] << " is";
        else {
            sss << "Entities ";
            for(int i=0; i<ss.size()-2; i++)
                sss << ss[i] << ", ";
            sss << ss[ss.size()-2] << " and " << ss[ss.size()-1] << " are";
        }
        sss << " turned off, but referenced here.";
        chart->Error.Warning(linenum_temp[earliest], sss, "It will look strange.");
    }
}


void ArcDirArrow::PostPosProcess(double autoMarker) 
{
    ArcArrow::PostPosProcess(autoMarker); 
    CheckSegmentOrder(yPos+centerline);
    //Exclude the areas covered by the text from entity lines
    chart->HideEntityLines(text_cover);
    //Exclude the areas covered by the arrow heads from entity lines
    Range hideEntityLine;
    hideEntityLine = style.arrow.EntityLineCover(XY(dx, yPos+centerline), sx<dx, isBidir(), MSC_ARROW_END);
    (*dst)->status.HideRange(hideEntityLine);
    hideEntityLine = style.arrow.EntityLineCover(XY(sx, yPos+centerline), sx<dx, isBidir(), MSC_ARROW_START);
    (*src)->status.HideRange(hideEntityLine);
    //for multi-segment arrows
    for (unsigned i=0; i<middle.size(); i++) {
        const double mx = chart->XCoord((*middle[i])->pos);
        Range hideEntityLine = style.arrow.EntityLineCover(XY(mx, yPos+centerline), sx<dx, isBidir(), MSC_ARROW_MIDDLE);
        (*middle[i])->status.HideRange(hideEntityLine);
    }
}

void ArcDirArrow::Draw()
{
    if (parsed_label.getTextWidthHeight().y) 
        parsed_label.Draw(sx_text, dx_text, yPos + chart->arcVGapAbove);
    /* Draw the line */
    //all the entities this (potentially multi-segment arrow visits)
    const double y = yPos+centerline;  //should be integer
    for (unsigned i=0; i<xPos.size()-1; i++) 
        chart->Line(XY(xPos[i]+margins[i].second, y), XY(xPos[i+1]+margins[i+1].first, y), style.line);
    /* Now the arrow heads */
    for (unsigned i=0; i<xPos.size(); i++) 
        style.arrow.Draw(XY(xPos[i], y), sx<dx, isBidir(), WhichArrow(i), chart);
}

//////////////////////////////////////////////////////////////////////////////////////

ArcBigArrow::ArcBigArrow(const ArcDirArrow &dirarrow, const MscStyle &s) : ArcDirArrow(dirarrow)
{
    style = s;
    switch(type) {
    case MSC_ARC_SOLID:
    case MSC_ARC_SOLID_BIDIR:
        style += chart->Contexts.back().styles["block->"]; break;
    case MSC_ARC_DOTTED:
    case MSC_ARC_DOTTED_BIDIR:
        style += chart->Contexts.back().styles["block>"]; break;
    case MSC_ARC_DASHED:
    case MSC_ARC_DASHED_BIDIR:
        style += chart->Contexts.back().styles["block>>"]; break;
    case MSC_ARC_DOUBLE:
    case MSC_ARC_DOUBLE_BIDIR:
        style += chart->Contexts.back().styles["block=>"]; break;
    }
    modifyFirstLineSpacing = false;
}

void ArcBigArrow::AttributeNames(Csh &csh)
{
    ArcLabelled::AttributeNames(csh);
    MscStyle style(STYLE_DEFAULT, ArrowHead::BIGARROW, true, true, true, false, false, false, true, true);  //no shadow, vline solid
    style.AttributeNames(csh);
}

bool ArcBigArrow::AttributeValues(const std::string attr, Csh &csh)
{
    MscStyle style(STYLE_DEFAULT, ArrowHead::BIGARROW, true, true, true, false, false, false, true, true);  //no shadow, vline solid
    if (style.AttributeValues(attr, csh)) return true;
    if (ArcLabelled::AttributeValues(attr, csh)) return true;
    return false;
}

string ArcBigArrow::Print(int ident) const
{
    return ArcDirArrow::Print(ident);
}

void ArcBigArrow::PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level)
{
    //Determine src and dst entity, check validity of multi-segment ones, add numbering, etc
    ArcDirArrow::PostParseProcess(left, right, number, top_level);
    //Finally copy the line attribute to the arrow, as well
    style.arrow.line = style.line;
}

void ArcBigArrow::Width(EntityDistanceMap &distances)
{
    //Set sy and dy
    //sy and dy are at the midline of the line around the body.
    //We ensure that the outer edge of the body falls on an integer value
    const double lw = style.line.LineWidth();
    const double aH = ceil(style.arrow.bigYExtent(isBidir(), middle.size()>0));
    XY twh = parsed_label.getTextWidthHeight();
    if (twh.y == 0)
        dy += Label("M", chart, style.text).getTextWidthHeight().y;
    sy = chart->arcVGapAbove + aH + lw/2;
    dy = ceil(sy + lw/2 + twh.y + chart->emphVGapInside*2 + lw) - lw/2;

    Area tcov = parsed_label.Cover(0, twh.x, lw/2);  
    const bool fw = (*src)->index < (*dst)->index;
    const DoublePair start = style.arrow.getBigWidths(fw, isBidir(), MSC_ARROW_START, dy-sy);
    const DoublePair end   = style.arrow.getBigWidths(fw, isBidir(), MSC_ARROW_END, dy-sy);

    distances.Insert((fw ? *src : *dst)->index, DISTANCE_LEFT, start.first);
    distances.Insert((fw ? *dst : *src)->index, DISTANCE_RIGHT, end.second);

    //Collect iterators and distances into arrays
    std::vector<EIterator> iterators;
    iterators.reserve(2+middle.size());
    margins.reserve(2+middle.size()); margins.clear();
    iterators.push_back(src);
    margins.push_back(start);
    for (int i=0; i<middle.size(); i++) {
        iterators.push_back(middle[i]);
        margins.push_back(style.arrow.getBigWidths(fw, isBidir(), MSC_ARROW_MIDDLE, dy-sy));
    }
    iterators.push_back(dst);
    margins.push_back(end);
    //Sort to make them increasing - direction does not count
    if ((*src)->index > (*dst)->index) {
        std::reverse(iterators.begin(), iterators.end());
        std::reverse(margins.begin(), margins.end());
    }
    for (int i=0; i<iterators.size()-1; i++) {
        //if neighbours
        if ((*iterators[i])->index + 1 == (*iterators[i+1])->index) {
            distances.Insert((*iterators[i])->index, (*iterators[i+1])->index,
                             margins[i].second+margins[i+1].first + 3);
        } else {
            distances.Insert((*iterators[i  ])->index,   (*iterators[i  ])->index+1,
                             margins[i].second + 3);
            distances.Insert((*iterators[i+1])->index-1, (*iterators[i+1])->index  ,
                             margins[i+1].first + 3);
        }
    }

    //Determine if there are multiple segments, if so add text into the appropriate one
    if (middle.size()>0 && style.arrow.bigDoesSegment(isBidir(), MSC_ARROW_MIDDLE)) {
        unsigned index;
        switch (style.text.GetIdent()) {
		default:
        case MSC_IDENT_LEFT:   index = 0; break;
        case MSC_IDENT_CENTER: index = iterators.size()/2-1; break;
        case MSC_IDENT_RIGHT:  index = iterators.size()-2; break;
        }
        stext = index;
        dtext = index+1;
    } else {
        //if no segments, then span the whole arrow.
        stext = 0;
        dtext = iterators.size()-1;
    }
    sm = style.arrow.getBigMargin(tcov, 0, dy-sy, true, fw, isBidir(), WhichArrow(stext)) + lw;
    dm = style.arrow.getBigMargin(tcov, 0, dy-sy, false, fw, isBidir(), WhichArrow(dtext)) + lw;
    distances.Insert((*iterators[stext])->index, (*iterators[dtext])->index, sm + twh.x + dm);
}


double ArcBigArrow::Height(AreaList &cover)
{
    yPos = 0;

    //Reuse sy and dy set in Width()
    centerline = (sy+dy)/2;

    //set sx and dx
    sx = chart->XCoord((*src)->pos);
    dx = chart->XCoord((*dst)->pos);
    //prepare xPos (margins were already done in Width)
    xPos.clear(); xPos.reserve(2+middle.size());
    xPos.push_back(sx);
    for (int i=0; i<middle.size(); i++) 
        xPos.push_back(chart->XCoord((*middle[i])->pos));
    xPos.push_back(dx);
    if (sx>=dx) 
        std::reverse(xPos.begin(), xPos.end());

    sx_text = xPos[stext] + sm;
    dx_text = xPos[dtext] + dm;

    area = style.arrow.BigCover(xPos, sy, dy, isBidir());
    //set mainline - not much dependent on main line with
    area.mainline = Range(centerline - chart->nudgeSize/2, centerline + chart->nudgeSize/2);
    cover = area;
    return centerline*2 - chart->arcVGapAbove + chart->arcVGapBelow;
}

void ArcBigArrow::ShiftBy(double y)
{
    sy += y;
    dy += y;
    ArcArrow::ShiftBy(y); //Skip ArcDirArrow
}

void ArcBigArrow::PostPosProcess(double autoMarker) 
{
    ArcArrow::PostPosProcess(autoMarker); //Skip ArcDirArrow
    CheckSegmentOrder(yPos + centerline);
}

void ArcBigArrow::Draw()
{
    style.arrow.BigDraw(xPos, sy, dy, isBidir(), style.fill, chart);
    parsed_label.Draw(sx_text, dx_text, sy+style.line.LineWidth()/2 + chart->emphVGapInside);
}

//////////////////////////////////////////////////////////////////////////////////////

VertXPos::VertXPos(Msc&m, const char *e1, file_line_range e1l,
                   const char *e2, file_line_range e2l, postype p)
{
    valid = true;
    pos = p;
    entity1 = m.FindAllocEntity(e1, e1l, &valid);
    if (pos == POS_CENTER) entity2 = m.FindAllocEntity(e2, e2l, &valid);
    else entity2 = m.NoEntity;
}

VertXPos::VertXPos(Msc&m, const char *e1, file_line_range e1l, postype p)
{
    valid = true;
    pos = p;
    entity1 = m.FindAllocEntity(e1, e1l, &valid);
    entity2 = m.NoEntity;
}

VertXPos::VertXPos(Msc&m, postype p)
{
    valid = true;
    entity1 = m.NoEntity;
    entity2 = m.NoEntity;
}

ArcVerticalArrow::ArcVerticalArrow(MscArcType t, const char *s, const char *d,
                                   VertXPos *p, Msc *msc) :
    ArcArrow(t, msc, msc->Contexts.back().styles["vertical"]), pos(*p), ypos(2)
{
    if (!p || !p->valid) {
        valid = false;
        return;
    }
    if (s) src = s;
    if (d) dst = d;
    switch (pos.pos) {
    case VertXPos::POS_LEFT_BY:
    case VertXPos::POS_LEFT_SIDE:
        readfromleft=false; break;
    default:
        readfromleft=true; break;
    }
    makeroom=false;
    offset = 0;
    //overwrite the sty;e set by ArcArrow
    style = msc->Contexts.back().styles["vertical"];
    switch(type) {
    case MSC_ARC_SOLID:
    case MSC_ARC_SOLID_BIDIR:
        style += chart->Contexts.back().styles["vertical->"]; break;
    case MSC_ARC_DOTTED:
    case MSC_ARC_DOTTED_BIDIR:
        style += chart->Contexts.back().styles["vertical>"]; break;
    case MSC_ARC_DASHED:
    case MSC_ARC_DASHED_BIDIR:
        style += chart->Contexts.back().styles["vertical>>"]; break;
    case MSC_ARC_DOUBLE:
    case MSC_ARC_DOUBLE_BIDIR:
        style += chart->Contexts.back().styles["vertical=>"]; break;
    case MSC_EMPH_SOLID:
        style += chart->Contexts.back().styles["vertical--"]; break;
    case MSC_EMPH_DASHED:
        style += chart->Contexts.back().styles["vertical++"]; break;
    case MSC_EMPH_DOTTED:
        style += chart->Contexts.back().styles["vertical.."]; break;
    case MSC_EMPH_DOUBLE:
        style += chart->Contexts.back().styles["vertical=="]; break;
    }
}

ArcArrow *ArcVerticalArrow::AddSegment(const char *m, file_line_range ml,
                                        bool forward, file_line_range l)
{
    if (!valid) return this; //display error only once
    chart->Error.Error(l.start, "Cannot add further segments to vertical arrow. Ignoring it.");
    valid = false;
    return this;
}

bool ArcVerticalArrow::AddAttribute(const Attribute &a)
{
    if (a.Is("pos")) {
        if (!a.EnsureNotClear(chart->Error, STYLE_ARC)) return true;
        if (!a.CheckType(MSC_ATTR_NUMBER, chart->Error)) return true;
        offset = a.number;
        return true;
    }
    if (a.Is("makeroom")) {
        if (!a.EnsureNotClear(chart->Error, STYLE_ARC)) return true;
        if (!a.CheckType(MSC_ATTR_BOOL, chart->Error)) return true;
        makeroom = a.yes;
        return true;
    }
    if (a.Is("readfrom")) {
        if (!a.EnsureNotClear(chart->Error, STYLE_ARC)) return true;
        if (!a.CheckType(MSC_ATTR_STRING, chart->Error)) return true;
        if (CaseInsensitiveEqual(a.value, "left")) readfromleft = true;
        else if (CaseInsensitiveEqual(a.value, "right")) readfromleft = false;
        else a.InvalidValueError("left/right", chart->Error);
        return true;
    }
    return ArcArrow::AddAttribute(a);
}

void ArcVerticalArrow::AttributeNames(Csh &csh)
{
    ArcLabelled::AttributeNames(csh);
    MscStyle style(STYLE_DEFAULT, ArrowHead::BIGARROW, true, true, true, false, false, false, true, true);  //no shadow, vline solid
    style.AttributeNames(csh);
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"pos", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"makeroom", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"readfrom", HINT_ATTR_NAME));
}

bool ArcVerticalArrow::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr,"pos")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable()+"<number>", HINT_ATTR_VALUE, false));
        return true;
    }
    if (CaseInsensitiveEqual(attr,"number")) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+"yes", HINT_ATTR_VALUE));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+"no", HINT_ATTR_VALUE));
        return true;
    }
    if (CaseInsensitiveEqual(attr,"number")) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+"left", HINT_ATTR_VALUE));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+"right", HINT_ATTR_VALUE));
        return true;
    }
    MscStyle style(STYLE_DEFAULT, ArrowHead::BIGARROW, true, true, true, false, false, false, true, true);  //no shadow, vline solid
    if (style.AttributeValues(attr, csh)) return true;
    if (ArcLabelled::AttributeValues(attr, csh)) return true;
    return false;
}


void ArcVerticalArrow::PostParseProcess(EIterator &left, EIterator &right,
                                        Numbering &number, bool top_level)
{
    if (src == MARKER_HERE_STR || src == MARKER_PREV_PARALLEL_STR)
        if (dst == MARKER_HERE_STR || dst == MARKER_PREV_PARALLEL_STR)
            if (top_level) {
                chart->Error.Error(file_pos.start, "Need at least one marker specified."
                                   " Ignoring vertical arrow.",
                                   "Only verticals inside a parallel block can omit both markers.");
                valid = false;
                return;
            }

    if (src != MARKER_HERE_STR && src != MARKER_PREV_PARALLEL_STR) {
        std::map<string, Msc::MarkerType>::const_iterator i = chart->Markers.find(src);
        if (i == chart->Markers.end()) {
            chart->Error.Error(file_pos.start, "Cannot find marker '" + src + "'."
                               " Ignoring vertical arrow.");
            valid=false;
            return;
        }
    }

    if (dst != MARKER_HERE_STR && dst != MARKER_PREV_PARALLEL_STR) {
        std::map<string, Msc::MarkerType>::const_iterator i = chart->Markers.find(dst);
        if (i == chart->Markers.end()) {
            chart->Error.Error(file_pos.start, "Cannot find marker '" + dst + "'."
                               " Ignoring vertical arrow.");
            valid=false;
            return;
        }
    }
    //Add numbering, if needed
    ArcLabelled::PostParseProcess(left, right, number, top_level);

    left = MinMaxByPos(left, pos.entity1, true);    //true=Min
    right = MinMaxByPos(right, pos.entity1, false); //false=Max
    left = MinMaxByPos(left, pos.entity2, true);    //true=Min
    right = MinMaxByPos(right, pos.entity2, false); //false=Max

    //Finally copy the line attribute to the arrow, as well
    style.arrow.line = style.line;

    static const MscGradientType readfrom_left_gardient[] = {
        GRADIENT_INVALID, GRADIENT_NONE, GRADIENT_OUT, GRADIENT_IN,
        GRADIENT_RIGHT, GRADIENT_LEFT, GRADIENT_DOWN, GRADIENT_UP,
        GRADIENT_LEFT};
    static const MscGradientType readfrom_right_gardient[] = {
        GRADIENT_INVALID, GRADIENT_NONE, GRADIENT_OUT, GRADIENT_IN,
        GRADIENT_LEFT, GRADIENT_RIGHT, GRADIENT_UP, GRADIENT_DOWN,
        GRADIENT_RIGHT};

    if (style.fill.gradient.first) {
        if (readfromleft)
            style.fill.gradient.second = readfrom_left_gardient[style.fill.gradient.second];
        else
            style.fill.gradient.second = readfrom_right_gardient[style.fill.gradient.second];
    }
}

void ArcVerticalArrow::Width(EntityDistanceMap &distances)
{
    //No extra space requirement
    if (!makeroom) return;
    //The offset is ignored during the process of setting space requirements
    const XY twh = parsed_label.getTextWidthHeight();
    const double lw = style.line.LineWidth();
    double width = twh.y;
    if (width==0)
        width = Label("M", chart, style.text).getTextWidthHeight().y;
    width += 2*lw + 2*chart->hscaleAutoXGap;

    const double aw =style.arrow.bigYExtent(isBidir(), false);
    const unsigned index = (*pos.entity1)->index;
    switch (pos.pos) {
    default:
    case VertXPos::POS_AT:
        distances.Insert(index, DISTANCE_LEFT, (width+aw)/2);
        distances.Insert(index, DISTANCE_RIGHT, (width+aw)/2);
        break;
    case VertXPos::POS_CENTER:
        distances.Insert(index, (*pos.entity2)->index, width+aw);
        break;
    case VertXPos::POS_LEFT_BY:
        distances.Insert(index, (*--EIterator(pos.entity1))->index, width + aw);
        break;
    case VertXPos::POS_RIGHT_BY:
        distances.Insert(index, (*++EIterator(pos.entity1))->index, width + aw);
        break;

    case VertXPos::POS_LEFT_SIDE:
        distances.Insert(index, (*--EIterator(pos.entity1))->index, width);
        break;
    case VertXPos::POS_RIGHT_SIDE:
        distances.Insert(index, (*++EIterator(pos.entity1))->index, width);
        break;
    };
}

//Height and parameters of this can only be calculated in PostPosProcess, when all other edges are set
//So here we do nothing. yPos is not used for this 
double ArcVerticalArrow::Height(AreaList &cover) 
{
    return 0;
}

void ArcVerticalArrow::ShiftBy(double y)
{
}

void ArcVerticalArrow::PostPosProcess(double autoMarker)
{
    ArcArrow::PostPosProcess(autoMarker);
	//area is empty here, so we will have to add our stuff to chart->AllCovers later in this function

    //Here we are sure markers are OK
    //all below are integers. yPos is such, in general. "Markers" are yPos of the markers
    if (src == MARKER_HERE_STR)
        ypos[0] = yPos;
    else if (src != MARKER_PREV_PARALLEL_STR)
        ypos[0] = chart->Markers.find(src)->second.second;
    else if (autoMarker>=0)
        ypos[0] = floor(autoMarker+0.5);
    else {
        chart->Error.Error(file_pos.start, "Vertical with no markers cannot take its size from the preceeding blocks."
                            " Ignoring vertical arrow.",
                            "Try putting it into a later block.");
        valid = false;
        return;
    }

    if (dst == MARKER_HERE_STR)
        ypos[1] = yPos;
    else if (dst != MARKER_PREV_PARALLEL_STR)
        ypos[1] = chart->Markers.find(dst)->second.second;
    else if (autoMarker>=0)
        ypos[1] = floor(autoMarker+0.5);
    else {
        chart->Error.Error(file_pos.start, "Vertical with no markers cannot take its size from the preceeding blocks."
                            " Ignoring vertical arrow.",
                            "Try putting it into a later block.");
        valid = false;
        return;
    }

    if (!readfromleft)
        swap(ypos[0], ypos[1]);

    const double lw = style.line.LineWidth();
    const XY twh = parsed_label.getTextWidthHeight();
    Area text_cover = parsed_label.Cover(0, twh.x, lw/2);

    const double sm = style.arrow.getBigMargin(text_cover, lw/2, twh.y+lw, true, readfromleft, isBidir(), MSC_ARROW_START);
    const double dm = style.arrow.getBigMargin(text_cover, lw/2, twh.y+lw, false, readfromleft, isBidir(), MSC_ARROW_END);

    if (sm + twh.x + dm  > fabs(ypos[0]-ypos[1])) 
        chart->Error.Warning(file_pos.start, "Size of vertical element is smaller than needed for text.",
                                 "May look strange.");

    //calculate xpos and width
    width = twh.y;
    if (width==0)
        width = Label("M", chart, style.text).getTextWidthHeight().y;
    width = ceil(width + 2*lw + 2*chart->emphVGapInside);
    width += fmod(width, 2); //width is even integer now: the distance from outer edge to outer edge

    const double aw = style.arrow.bigYExtent(isBidir(), false)/2;
    xpos = chart->XCoord((*pos.entity1)->pos);
    const double gap = chart->hscaleAutoXGap;
    switch (pos.pos) {
    default:
    case VertXPos::POS_AT: break;
    case VertXPos::POS_CENTER:      xpos = (xpos + chart->XCoord((*pos.entity2)->pos))/2; break;
    case VertXPos::POS_LEFT_BY:     xpos -= width/2 + aw + gap; break;
    case VertXPos::POS_RIGHT_BY:    xpos += width/2 + aw + gap; break;

    case VertXPos::POS_LEFT_SIDE:   xpos -= width/2 + gap; break;
    case VertXPos::POS_RIGHT_SIDE:  xpos += width/2 + gap; break;
    };
    xpos = floor(xpos + offset + 0.5); //xpos is integer now: the centerline of arrow
    width -= lw; //not necessarily integer, the distance from midline to midline

    if (readfromleft) {
        sy_text = std::min(ypos[0], ypos[1])+sm;
        dy_text = std::max(ypos[0], ypos[1])-dm;
    } else {
        sy_text = std::max(ypos[0], ypos[1])-sm;
        dy_text = std::min(ypos[0], ypos[1])+dm;
    }

	//Generate area
    area = style.arrow.BigCover(ypos, xpos-width/2, xpos+width/2, isBidir());
    area.SwapXY();
    //Add us to chart's all covers list
    chart->AllCovers += area;
    yPos = std::min(ypos[0], ypos[1]);  //not used for much...
}


void ArcVerticalArrow::Draw()
{
    if (readfromleft)
        chart->Transform_Rotate90(xpos-width/2, xpos+width/2, false);
    else
        chart->Transform_Rotate90(ypos[0], ypos[1], true);
    //Draw background
    style.arrow.BigDraw(ypos, xpos-width/2, xpos+width/2, isBidir(), style.fill, chart);
    parsed_label.Draw(min(ypos[0], ypos[1]), max(ypos[0], ypos[1]),
                      xpos-width/2+style.line.LineWidth()/2+chart->emphVGapInside, true);
    chart->UnTransform();
}

//////////////////////////////////////////////////////////////////////////////////////

ArcEmphasis::ArcEmphasis(MscArcType t, const char *s, file_line_range sl,
                         const char *d, file_line_range dl, Msc *msc) :
    ArcLabelled(t, msc, msc->Contexts.back().styles["emptybox"]),
    emphasis(NULL), follow(true), first(NULL), pipe(false), fromright(true), drawEntityLines(true)
{
    src = chart->FindAllocEntity(s, sl, &valid);
    dst = chart->FindAllocEntity(d, dl, &valid);

    //If both src and dst specified, order them
    if (src!=chart->NoEntity && dst!=chart->NoEntity)
        if ((*src)->pos > (*dst)->pos) {
            EIterator e = dst;
            dst = src;
            src = e;
        }
};

ArcEmphasis* ArcEmphasis::SetPipe()
{
    pipe = true;
    drawEntityLines = false;
    style = chart->Contexts.back().styles["pipe"];
    switch (type) {
    case MSC_EMPH_SOLID:
        style += chart->Contexts.back().styles["pipe--"]; break;
    case MSC_EMPH_DASHED:
        style += chart->Contexts.back().styles["pipe++"]; break;
    case MSC_EMPH_DOTTED:
        style += chart->Contexts.back().styles["pipe.."]; break;
    case MSC_EMPH_DOUBLE:
        style += chart->Contexts.back().styles["pipe=="]; break;
    }
    return this;
}

ArcEmphasis* ArcEmphasis::AddArcList(ArcList*l)
{
    if (l!=NULL && l->size()>0) {
        if (emphasis) {
            emphasis->insert(emphasis->end(), l->begin(), l->end());
            l->clear(); //so that l's constructor does not delete Arcs in arclist
            delete l;
        } else {
            emphasis=l;
        }
    }
    if (!pipe)
        style += chart->Contexts.back().styles["box"];
    return this;
}


bool ArcEmphasis::AddAttribute(const Attribute &a)
{
    if (a.type == MSC_ATTR_STYLE) return ArcLabelled::AddAttribute(a);
    if (a.Is("color")) {
        return style.fill.AddAttribute(a, chart, style.type);
    }
    return ArcLabelled::AddAttribute(a);
}

void ArcEmphasis::AttributeNames(Csh &csh, bool pipe)
{
    ArcLabelled::AttributeNames(csh);
    MscStyle style(STYLE_DEFAULT, ArrowHead::NONE, true, true, true, true, false, pipe, true, true); //no arrow, vline solid
    style.AttributeNames(csh);
}

bool ArcEmphasis::AttributeValues(const std::string attr, Csh &csh, bool pipe)
{
    if (CaseInsensitiveEqual(attr,"color")) {
        csh.AddColorValuesToHints();
        return true;
    }
    MscStyle style(STYLE_DEFAULT, ArrowHead::NONE, true, true, true, true, false, pipe, true, true); //no arrow, vline solid
    if (style.AttributeValues(attr, csh)) return true;
    if (ArcLabelled::AttributeValues(attr, csh)) return true;
    return false;
}

ArcEmphasis* ArcEmphasis::ChangeStyleForFollow(ArcEmphasis* other)
{
    style.Empty();
    if (other)
        style.line = other->style.line;
    else
        style.line.type.first = true;
    return this;
}

ArcEmphasis* ArcEmphasis::AddFollow(ArcEmphasis*f)
{
    f->first = this;
    if (!pipe) { //Inherit styles only for boxes, not pipes
        MscStyle s = style;
        s += f->style;
        f->style = s;
    }
    follow.Append(f);
    return this;
}

string ArcEmphasis::Print(int ident) const
{
    string ss;
    ss << string(ident*2, ' ');
    ss << (*src)->name << "--" << (*dst)->name;
    if (label.length()>0) ss << ": \"" << label << "\"";
    if (emphasis)
        for (ArcList::iterator i = emphasis->begin(); i != emphasis->end(); i++)
            ss << "\n" << (*i)->Print(ident+1);
    return ss;
}

//will only be called for the first box of a multi-segment box series
void ArcEmphasis::PostParseProcess(EIterator &left, EIterator &right,
                                   Numbering &number, bool top_level)
{
    //Add ourselves as the first element in follow.
    follow.push_front(this);

    EIterator e1=src;      //src.pos < dst.pos guaranteed in constructor (if none is NULL)
    EIterator e2=dst;
    //OK, now expand further if there are following boxes
    for (PtrList<ArcEmphasis>::iterator i = follow.begin(); i!=follow.end(); i++) {
        //Add numbering, if needed
        (*i)->ArcLabelled::PostParseProcess(left, right, number, top_level);
        if ((*i)->emphasis) {
            //If we have attached a number to the label of a box, set flag on number,
            //so that if we add levels inside the content of the box (before displaying any number)
            //we continue from present value
            if ((*i)->style.numbering.second && (*i)->label.length()!=0)
                number.decrementOnAddingLevels = true;
            chart->PostParseProcessArcList(*((*i)->emphasis), false, e1, e2, number, top_level);
            number.decrementOnAddingLevels = false;
        }
    }

    if (src==chart->NoEntity) src = e1;
    if (dst==chart->NoEntity) dst = e2;

    //Src and dst can still be == NoEntity, if no arcs specified
    //inside the emphasis and no enity specified at emph declaration.
    //In this case emph box spans to leftmost and rightmost entity in chart.
    //At PostParse "Entities" is already sorted by pos values
    //we only do this step if we are the first in an Emphasis box series.
    if (src==chart->NoEntity) src = ++ ++chart->Entities.begin();     //leftmost entity after Noentity and (left)
    if (dst==chart->NoEntity) dst = -- --chart->Entities.end();    //rightmost entity (before (right)

    left = MinMaxByPos(MinMaxByPos(left, src, true), dst, true);    //true=Min
    right = MinMaxByPos(MinMaxByPos(right, src, false), dst, false); //false=Max

    if (!pipe) return;

    //Check that all pipe segments are fully specified, non-overlapping
    //Also set pipe_connect_left/right flags
    EIterator last = dst;
    for (PtrList<ArcEmphasis>::iterator i = ++follow.begin(); i!=follow.end(); i++) {
        if ((*i)->src==chart->NoEntity && (*i)->dst==chart->NoEntity) {
            chart->Error.Error((*i)->file_pos.start, "Pipes in a pipe series must be given at least an end entity."
                            " Ignoring pipe segment.");
            (*i)->valid = false;
            continue;
        }
        //Make both src and dst specified and ordered
        if ((*i)->src == chart->NoEntity) (*i)->src = last;
        else if ((*i)->dst == chart->NoEntity) (*i)->dst = last;
        if ((*i)->src != MinMaxByPos((*i)->src, (*i)->dst, true)) //true=Min
            swap((*i)->src, (*i)->dst);

        //We can be here if follow.size()>1, so we are definitely talking about segments, not full pipes
        if ((*i)->src == (*i)->dst) {
            chart->Error.Error((*i)->file_pos.start, "This pipe segment seems to start and end at the same entity."
                               " Ignoring pipe segment.");
            (*i)->valid = false;
            continue;
        }
        (*i)->pipe_connect_left = (*i)->pipe_connect_right = false;
        //Set flags if we are adjacent to previous one
        if (last == (*i)->src) {
            (*i)->pipe_connect_left = true;
            i--;
            (*i)->pipe_connect_right = true;
            i++;
        } else if (last == MinMaxByPos((*i)->src, last, false)) {
            chart->Error.Warning((*i)->file_pos.start, "This pipe segment overlaps the previous."
                                 " It may not look so good.",
                                 "Encapsulate one in the other if you want that effect.");
        }
        last = MinMaxByPos((*i)->dst, last, false); //false = Max
    }

    //if there is no content, no need to draw a transparent cover
    //save drawing cost (and potential fallback img)
    //only first pipe can have content (which becomes the content of all pipe)
    if (!emphasis || emphasis->size() == 0)
        for (PtrList<ArcEmphasis>::iterator i = follow.begin(); i!=follow.end(); i++)
            (*i)->style.solid.second = 255;
}

//will only be called for the first box of a multi-segment box series
void ArcEmphasis::Width(EntityDistanceMap &distances)
{
    if (pipe) {
        EntityDistanceMap d;
        if (emphasis)
            chart->WidthArcList(*emphasis, d);

        for (auto i = follow.begin(); i!=follow.end(); i++) {
            const double ilw = (*i)->style.line.LineWidth();
            const double width = (*i)->parsed_label.getTextWidthHeight().x + 2*chart->emphVGapInside;
            (*i)->left_space = d.Query((*(*i)->src)->index, DISTANCE_LEFT) + chart->emphVGapInside;   
            (*i)->right_space = d.Query((*(*i)->dst)->index, DISTANCE_RIGHT) + chart->emphVGapInside;

            if ((*i)->src==(*i)->dst) {
                //This can happen only if the pipe contains one segment only i == follow.begin
                //so no need to check pipe_connect flags
                (*i)->left_space  = ceil(max(width/2, (*i)->left_space) + ilw/2 +
                                         style.line.radius.second);  //first pipe's radius
                (*i)->right_space = ceil(max(width/2, (*i)->right_space) + ilw/2 +
                                         style.line.radius.second);
            } else {
                //keep a big enough space between src and dst for the text + curvature of pipe
                //the text can go out of the entity lines, all the way to the left-space
                distances.Insert((*(*i)->src)->index, (*(*i)->dst)->index,
                                 width - (*i)->left_space - (*i)->right_space +
                                 2*chart->emphVGapInside);
                (*i)->left_space  = ceil((*i)->left_space  + ilw/2 + style.line.radius.second);
                (*i)->right_space = ceil((*i)->right_space + ilw/2 + style.line.radius.second);
                //Check if we are connecting to a neighbour pipe segment
                if ((*i)->pipe_connect_left) (*i)->left_space = 0;
                if ((*i)->pipe_connect_right) (*i)->right_space = 0;
            }
            if (!(*i)->pipe_connect_left) 
                distances.Insert((*(*i)->src)->index, DISTANCE_LEFT, left_space + ilw/2);
            if ((*i)->pipe_connect_right) 
                distances.Insert((*(*i)->dst)->index, DISTANCE_RIGHT, right_space + ilw/2 + 
                                                                  (*i)->style.shadow.offset.second);
        }

        //Finally add the requirements of the content
        d.CombineLeftRightToPair_Max(chart->hscaleAutoXGap);
        d.CombineLeftRightToPair_Single(chart->hscaleAutoXGap);
        distances += d;
    } else {
        EntityDistanceMap d;
        double width = 0; //the widest label
        for (PtrList<ArcEmphasis>::iterator i = follow.begin(); i!=follow.end(); i++) {
            if ((*i)->emphasis)
                chart->WidthArcList(*((*i)->emphasis), d);
            width = max(width, (*i)->parsed_label.getTextWidthHeight().x);
        }

        //Now d contains distance requirements within this emph box series
        //And width contains the widest 
        left_space = d.Query((*src)->index, DISTANCE_LEFT) + 3;
        right_space = d.Query((*dst)->index, DISTANCE_RIGHT) + 3;

        d.CombineLeftRightToPair_Max(chart->hscaleAutoXGap);
        d.CombineLeftRightToPair_Single(chart->hscaleAutoXGap);

        if (src==dst) {
            left_space = max(width/2, left_space) + style.line.LineWidth();
            right_space = max(width/2, right_space) + style.line.LineWidth() +
                style.shadow.offset.second;
        } else {
            const double def_margin = chart->XCoord(0.25);
            left_space = max(def_margin, left_space) + style.line.LineWidth();
            right_space = max(def_margin, right_space) + style.line.LineWidth() +
                style.shadow.offset.second;
            if (width > left_space + right_space)
                distances.Insert((*src)->index, (*dst)->index,
                                 width - left_space - right_space);
        }
        distances.Insert((*src)->index, DISTANCE_LEFT, left_space);
        distances.Insert((*dst)->index, DISTANCE_RIGHT, right_space);
        distances += d;
    }
}

//Will only be called for the first box of a multi-segment box/pipe series
double ArcEmphasis::Height(AreaList &cover)
{
    if (pipe) {
        cover.clear();
        //collect the cover for those labels that are not on a fully opaque segment
        AreaList labels;
        double max_lineWidth = 0;

        //Determine thickest line for precise pipe alignment
        for (auto i = follow.begin(); i!=follow.end(); i++)
            max_lineWidth = std::max(max_lineWidth, (*i)->style.line.LineWidth());
        double lowest_line_bottom =  max_lineWidth + chart->emphVGapInside;
        double lowest_label_bottom = 0;
        double lowest_opaque_label_bottom = 0;
        //Collect cover information from labels and linewidth, so compression of content arrows can be done
        for (auto i = follow.begin(); i!=follow.end(); i++) {
            //Variables already set (all of them rounded):
            //pipe_connect true if a segment connects to us directly
            //left_space, right_space contains how much our content expands beyond the entity line, 
            //  plus the curvature of the pipe and a linewidth/2. This is the pipe's rectangle's extent.
            (*i)->yPos = 0; 
            (*i)->area.clear();
            (*i)->area_draw.clear();
            (*i)->draw_is_different = false;

            //Set pipe_block.x, sx_text, dx_text in each segment, in the meantime
            (*i)->y_text = ceil(chart->emphVGapOutside + (*i)->style.line.LineWidth() + 
                           chart->emphVGapInside);
            (*i)->area.clear();
            (*i)->pipe_block.x.from = chart->XCoord((*(*i)->src)->pos) - (*i)->left_space; //rounded
            (*i)->pipe_block.x.till = chart->XCoord((*(*i)->dst)->pos) + (*i)->right_space;
            (*i)->sx_text = pipe_block.x.from + (*i)->style.line.LineWidth()/2 + chart->emphVGapInside; //not rounded
            (*i)->dx_text = pipe_block.x.till - (*i)->style.line.LineWidth()/2 - chart->emphVGapInside;
            if (style.text.GetIdent() == MSC_IDENT_RIGHT && fromright)
                (*i)->sx_text += style.line.radius.second;
            if (style.text.GetIdent() == MSC_IDENT_LEFT && !fromright)
                (*i)->dx_text -= style.line.radius.second;
            (*i)->text_cover = (*i)->parsed_label.Cover(sx_text, dx_text, (*i)->y_text);
            //if there is content and not opaque, make the drawn and found area different
            if (emphasis && (*i)->style.solid.second < 255) {
                (*i)->area_draw = (*i)->text_cover;
                (*i)->draw_is_different = true;
            }
            // omit text cover for pipes if the pipe is fully opaque,
            // in that case content can be drawn at same position as label - opaque pipe will cover anyway
            double y = (*i)->y_text + (*i)->parsed_label.getTextWidthHeight().y;
            if (style.solid.second < 255) {
                labels += (*i)->text_cover;
                lowest_label_bottom = std::max(lowest_label_bottom, y);
            } else {
                //collect the highest label of opaque segments for later use
                lowest_opaque_label_bottom = std::max(lowest_opaque_label_bottom, y); 
            }
        }
        //check if thick lined segments having no label have lower still
        if (lowest_label_bottom < lowest_line_bottom)
            lowest_label_bottom = lowest_line_bottom;
        double y = lowest_label_bottom; 

        //Calculate the Height of the content
        if (emphasis) 
            y = ceil(chart->PlaceListUnder(emphasis->begin(), emphasis->end(), ceil(y),
                                      lowest_line_bottom, labels));
        //now y contains the bottom of the content arrows (if any), 
        //adjust if an opaque pipe's label was not yet considered in y
        y = std::max(y, lowest_opaque_label_bottom);
        y += chart->emphVGapInside + max_lineWidth;
        //now y contains the bottommost pixel of the pipe itself
        total_height = y = ceil(y);
        //Now bottom contains the bottom of the pipes (outer edge of line)
        for (auto i = follow.begin(); i!=follow.end(); i++) {
            total_height = std::max(total_height, y + (*i)->style.shadow.offset.second);

            //fill in pipe_block.y (both are integer if lw is even and .5 if lw is odd: OK)
            (*i)->pipe_block.y.from = chart->emphVGapOutside + (*i)->style.line.LineWidth()/2;
            (*i)->pipe_block.y.till = y - (*i)->style.line.LineWidth()/2;

            const XY cs((*i)->pipe_block.x.from, (*i)->pipe_block.y.MidPoint());
            const XY cd((*i)->pipe_block.x.till, (*i)->pipe_block.y.MidPoint());
            const XY rad(style.line.radius.second, pipe_block.y.Spans()/2); //we use the first pipe's line.radius
            //                                                    ____              __
            //pipe body is the skewed rectangle with curvy edges (___( , hole is the _() at the end
            (*i)->pipe_body.clear();
            (*i)->pipe_hole.clear();
            Contour es(cs, rad.x, rad.y);
            Contour ed(cd, rad.x, rad.y);

            if (fromright) {    //use the first pipe's fromright, not (*i)->fromright
                (*i)->pipe_body = Contour(*(pipe_block + es - ed).begin());
                (*i)->pipe_hole = ed;
            } else {
                (*i)->pipe_body = Contour(*(pipe_block + ed - es).begin());
                (*i)->pipe_hole = es;
            }
            //Add covers
            const bool use_body_only = ((*i)->pipe_connect_left && !fromright) || ((*i)->pipe_connect_right && fromright);
            Area pipe_area = use_body_only ? (*i)->pipe_body : (*i)->pipe_body + (*i)->pipe_hole; 
            (*i)->area = pipe_area;
            if (emphasis && (*i)->style.solid.second < 255) {
                //Make a frame, add it to the already added label
                (*i)->area_draw += pipe_area.CreateExpand(4) - pipe_area;
            }
            //now determine the cover to be used for placement
            if (style.shadow.offset.second) 
                cover += pipe_area + Area(pipe_area).Shift(XY(style.shadow.offset.second, style.shadow.offset.second));
            else
                cover += pipe_area;
        }
        cover.mainline = Range(chart->emphVGapOutside, total_height);
    } else {
        //We are a box here
        //A few explanations of the variables exact meaning
        //the upper edge of the upper line of each segment is at yPos
        //total_height includes linewidths and shadow, but not emphVGapOutside
        //left_space and right_space includes linewidth and shadow
        //height includes the upper linewidth, emphvgapinside, content, lower emphvgapinside, but not lower lw
        //sx and dx are the inner edges of the lines of the whole box
        const double lw = style.line.LineWidth();
        const double sx = chart->XCoord((*src)->pos) - left_space + lw;
        const double dx = chart->XCoord((*dst)->pos) + right_space - lw - style.shadow.offset.second;

        double y = chart->emphVGapOutside;
        for (auto i = follow.begin(); i!=follow.end(); i++) {
            (*i)->yPos = y;
            // y now points to the *top* of the line of the top edge of this box
            // if we are pipe, we draw the segment side-by side, so we reset y here

            //Advance upper line and spacing
            y += (*i)->style.line.LineWidth() + chart->emphVGapInside;
            (*i)->y_text = y;
            (*i)->sx_text = sx;
            (*i)->dx_text = dx;
            //Add text cover & draw if necessary
            (*i)->text_cover = (*i)->parsed_label.Cover(sx, dx, y);
            //Advance label height
            double th = (*i)->parsed_label.getTextWidthHeight().y;
            y += th;
            //Draw arrows if any under the label 
            if ((*i)->emphasis)
                y = chart->PlaceListUnder((*i)->emphasis->begin(), (*i)->emphasis->end(),
                                           y, y-th, (*i)->text_cover, compress);

            y += chart->emphVGapInside;
            (*i)->height = y - (*i)->yPos;

            double bottom = y; 
            //Add the linewidth of the next box or the final one
            if (i==--follow.end()) 
                bottom += lw; 
            else
                bottom += (*++PtrList<ArcEmphasis>::iterator(i))->style.line.LineWidth();

            (*i)->area = Block(sx-lw, dx+lw, (*i)->yPos, bottom);
            if ((*i)->emphasis) {
                //Make a frame, add it to the already added label
                (*i)->area_draw = (*i)->area.CreateExpand(4) - (*i)->area;
                (*i)->area_draw += (*i)->text_cover;
                (*i)->draw_is_different = true;
            } else {
                (*i)->area_draw.clear();
                (*i)->draw_is_different = false;
            }
        } /* for cycle through segments */
        //Final advance of linewidth
        total_height = y + lw + style.shadow.offset.second - yPos;

        Block b(sx-lw, dx+lw+style.shadow.offset.second, yPos, yPos+total_height);
        Area whole(b);
        whole.mainline = b.y;
        cover += whole;
    } 
    return total_height + chart->emphVGapOutside;
}

void ArcEmphasis::ShiftBy(double y)
{
    if (y==0) return;
    for (auto i=follow.begin(); i!=follow.end(); i++) {
        if (pipe) {
            (*i)->pipe_block.Shift(XY(0,y));
            (*i)->pipe_body.Shift(XY(0,y));
            (*i)->pipe_hole.Shift(XY(0,y));
        } 
        (*i)->y_text += y;
        (*i)->text_cover.Shift(XY(0,y));
        (*i)->ArcLabelled::ShiftBy(y);
        if ((*i)->emphasis)
            chart->ShiftByArcList((*i)->emphasis->begin(), (*i)->emphasis->end(), y);
    }
}

//Will only be called for the first box of a multi-segment box series
void ArcEmphasis::PostPosProcess(double autoMarker)
{
    if (pipe) {
        //For pipes we first add those covers to chart->AllCovers that are not fully opaque,
        //then the content (only in the first segment)
        //then those segments, which are fully opaque
        //(this is because search is backwards and this arrangement fits the visual best
        for (auto i = follow.begin(); i!=follow.end(); i++) 
            if ((*i)->valid && (*i)->style.solid.second < 255) 
                (*i)->ArcLabelled::PostPosProcess(autoMarker);
        if (emphasis)
            chart->PostPosProcessArcList(*emphasis, autoMarker);
        for (auto i = follow.begin(); i!=follow.end(); i++) 
            if ((*i)->valid && (*i)->style.solid.second == 255) 
                (*i)->ArcLabelled::PostPosProcess(autoMarker);
    } else
        //For boxes we always add the background first then the content
        //And we do this for each segment sequentially
        for (auto i = follow.begin(); i!=follow.end(); i++)
            if ((*i)->valid) {
                (*i)->ArcLabelled::PostPosProcess(autoMarker);
                if ((*i)->emphasis)
                    chart->PostPosProcessArcList(*emphasis, autoMarker);
            }
    for (auto i = follow.begin(); i!=follow.end(); i++)
        chart->HideEntityLines((*i)->text_cover);
}



//Draw a pipe, this is called for each segment
//topside is the bigger part of the pipe
//backside is the small oval visible form the back of the pipe
//this->yPos is the outer edge of the top line
//this->left_space and right_space includes linewidth
void ArcEmphasis::DrawPipe(bool topSideFill, bool topSideLine, bool backSide, bool text)
{
    if (backSide) {
        //Shadow under the whole pipe
        chart->Shadow(pipe_body+pipe_hole, style.shadow, true);
        //The back of the main pipe
        chart->Fill(pipe_body, style.fill);
        //the back of the small ellipsis visible from the side
        MscFillAttr fill = style.fill;
        if (fill.gradient.second == GRADIENT_UP)
            fill.gradient.second = GRADIENT_DOWN;
        else if (fill.gradient.second == GRADIENT_DOWN)
            fill.gradient.second = GRADIENT_UP;
    //    chart->Fill(pipe_hole, fill);
        //Draw the backside line
    //    chart->Line(pipe_hole, style.line);
    }
    if (topSideFill) {
        //apply the transparency of the solid attribute
        MscFillAttr fill = style.fill;
        fill.color.second.a = unsigned(style.solid.second) * unsigned(fill.color.second.a) / 255;
        chart->Fill(pipe_body, fill);
    }
    if (topSideLine) {
        chart->Line(pipe_body, style.line);
        //XXX double lines: fix it
        //XXX: We draw a pipe arc twice, check if OK
    }
    if (text) {
        double curve_gap_left = 0, curve_gap_right = 0;
        if (style.text.GetIdent() == MSC_IDENT_RIGHT && fromright)
            curve_gap_right = style.line.radius.second;
        if (style.text.GetIdent() == MSC_IDENT_LEFT && !fromright)
            curve_gap_left = style.line.radius.second;
        parsed_label.Draw(pipe_block.x.from+curve_gap_left, pipe_block.x.till-curve_gap_right,
                          yPos + style.line.LineWidth() + chart->emphVGapInside);
    }
}

void ArcEmphasis::Draw()
{
    if (pipe) {
        for (auto i = follow.begin(); i!=follow.end(); i++) {
            //Dont draw the topside fill
            //Draw the topside line only if pipe is fully transparent. Else we may cover the line.
            //Draw the backside in any case.
            //Do not draw text
            (*i)->DrawPipe(false, (*i)->style.solid.second == 0, true, false);
            if (emphasis && drawEntityLines)
                chart->DrawEntityLines(yPos, height, (*i)->src, ++EIterator((*i)->dst));
        }
        if (emphasis)
            chart->DrawArcList(*emphasis);
        for (auto i = follow.begin(); i!=follow.end(); i++) {
            //Draw the topside fill only if the pipe is not fully transparent.
            //Draw the topside line in any case
            //Do not draw the backside (that may content arrow lines already drawn)
            //Draw the text
            (*i)->DrawPipe((*i)->style.solid.second > 0, true, false, true);
        }
    } else {
        //For boxes draw background for each segment, then separator lines, then bounding rectangle lines, then content
        const double lw = style.line.LineWidth();
        MscLineAttr line = style.line;  //We will vary the radius, so we need a copy
        //The radius specified in style.line will be that of the actual line (midpoint of)
        //First draw the shadow. If we have only opaque fill colors, clip for performance
        bool clipShadow = true;
        for (auto i = follow.begin(); i!=follow.end(); i++)
            if ((*i)->style.fill.color.second.a<255) {
                clipShadow = false;
                break;
            }
        Block r(sx_text-lw, dx_text+lw, yPos, yPos+total_height-style.shadow.offset.second); //The outer edge of the lines
        //The shadow will have to be lw/2 outer than the original line midpoint
        if (line.radius.second > 0)
            line.radius.second += lw/2;
        chart->Shadow(r, style.shadow, clipShadow); 
        //First do a clip region for the overall box (for round corners) at the midpoint of the line
        line.radius.second -= lw/2;
        r.Expand(-lw/2);
        chart->Clip(r, line);
        for (auto i = follow.begin(); i!=follow.end(); i++) {
            //Overall rule for background fill:
            //for single line borders we fill up to the middle of the border
            //for double line borders we fill up to the middle of the inner line of the border
            //style.line.LineWidth() gives the full width of the (double) line, width.second is just one line of it
            //for single lines style.line.LineWidth()==style.line.width.second
            double sy = (*i)->yPos + (*i)->style.line.LineWidth() - (*i)->style.line.width.second/2;
            double dy = (*i)->yPos + (*i)->height + (*i)->style.line.width.second/2;
            //Increase the fill area downward by half of the linewidth below us
            PtrList<ArcEmphasis>::const_iterator next = i;
            next++;
            if (next==follow.end())
                dy += style.line.width.second/2;
            else
                dy += (*next)->style.line.width.second/2;
            chart->Fill(Block(sx_text-style.line.width.second/2, dx_text+style.line.width.second/2, sy, dy), (*i)->style.fill);
            //if there are contained entities, draw entity lines, strictly from inside of line
            if ((*i)->emphasis)
                chart->DrawEntityLines((*i)->yPos + (*i)->style.line.LineWidth(), (*i)->height, src, ++EIterator(dst));
        }
        chart->UnClip();
        //Draw box lines - Cycle only for subsequent boxes
        for (auto i = ++(follow.begin()); i!=follow.end(); i++) {
            const double y = (*i)->yPos + (*i)->style.line.LineWidth()/2;
            chart->Line(XY(sx_text, y), XY(dx_text, y), (*i)->style.line);
        }
        //Finally draw the overall line around the box 
        chart->Line(r, line);
        //XXX double line joints: fix it
        for (auto i = follow.begin(); i!=follow.end(); i++) {
            parsed_label.Draw((*i)->sx_text, (*i)->dx_text, (*i)->y_text);
            if (emphasis)
                chart->DrawArcList(*emphasis);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////

ArcDivider::ArcDivider(MscArcType t, Msc *msc) :
    ArcLabelled(t, msc, msc->Contexts.back().styles["divider"]),
    nudge(t==MSC_COMMAND_NUDGE)
{
}

bool ArcDivider::AddAttribute(const Attribute &a)
{
    if (a.Is("_wide")) {
        if (a.CheckType(MSC_ATTR_BOOL, chart->Error))
            wide = a.yes;
        return true;
    }
    return ArcLabelled::AddAttribute(a);
};

void ArcDivider::AttributeNames(Csh &csh)
{
    ArcLabelled::AttributeNames(csh);
    MscStyle style(STYLE_DEFAULT, ArrowHead::NONE, true, true, false, false, true, false, true, true); //no arrow, fill, shadow solid
    style.AttributeNames(csh);
}

bool ArcDivider::AttributeValues(const std::string attr, Csh &csh)
{
    MscStyle style(STYLE_DEFAULT, ArrowHead::NONE, true, true, false, false, true, false, true, true); //no arrow, fill, shadow solid
    if (style.AttributeValues(attr, csh)) return true;
    if (ArcLabelled::AttributeValues(attr, csh)) return true;
    return false;
}

void ArcDivider::PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level)
{
    //Add numbering, if needed
    ArcLabelled::PostParseProcess(left, right, number, top_level);

    if (!top_level && (type==MSC_ARC_DISCO || type==MSC_ARC_DIVIDER)) {
        string ss;
        ss << (type==MSC_ARC_DISCO ? "'...'" : "'---'") << " is specified inside a parallel block.";
        chart->Error.Warning(file_pos.start, ss, "May display incorrectly.");
    }
}

void ArcDivider::Width(EntityDistanceMap &distances)
{
    if (!valid) return;
    if (nudge || !valid || parsed_label.getTextWidthHeight().y==0)
        return;
    const unsigned lside_index = (*chart->Entities.Find_by_Name(LSIDE_ENT_STR))->index;
    const unsigned rside_index = (*chart->Entities.Find_by_Name(RSIDE_ENT_STR))->index;
    //Get marging from chart edge
    double margin = wide ? 0 : chart->XCoord(MARGIN*1.3);
    //convert it to a margin from lside and rside
    if (chart->hscale>0)
        margin -= chart->XCoord(MARGIN);
    else
        margin -= chart->XCoord(MARGIN_HSCALE_AUTO);
    //calculate space requirement between lside and rside
    const double width = 2*margin + parsed_label.getTextWidthHeight().x;
    if (width>0)
        distances.Insert(lside_index, rside_index, width);
}

double ArcDivider::Height(AreaList &cover)
{
    yPos = 0;
    if (nudge) {
        Block b(0, chart->total.x, 0, chart->nudgeSize);
        area = b;
        area.mainline=b.y;
        cover+=area;
        return chart->nudgeSize;
    }
    double y = wide ? 0 : chart->arcVGapAbove;
    const double charheight = Label("M", chart, style.text).getTextWidthHeight().y;

    XY wh = parsed_label.getTextWidthHeight();
    if (!wh.y) wh.y = charheight;
    centerline = y+wh.y/2;
    text_margin = wide ? 0 : chart->XCoord(MARGIN*1.3);
    line_margin = chart->XCoord(MARGIN);
    text_cover = parsed_label.Cover(text_margin, chart->total.x-text_margin, y);
    area = text_cover;

    //Add a cover block for the line, if one exists
    if (style.line.type.second != LINE_NONE && style.line.color.second.valid && style.line.color.second.a>0)
        area += Block(line_margin, chart->total.x-line_margin,
                      centerline - style.line.LineWidth()*2, centerline + style.line.LineWidth()*2);
    area.mainline += Range(centerline-charheight/2, centerline+charheight/2);
    cover += area;
    if (!wide)
        wh.y += chart->arcVGapAbove + chart->arcVGapBelow;
    return height = wh.y;
}

void ArcDivider::ShiftBy(double y)
{
    if (!nudge)
        text_cover.Shift(XY(0,y));
    ArcLabelled::ShiftBy(y);
}

void ArcDivider::PostPosProcess(double autoMarker)
{
	//If there is a vline in the current style, add that to entitylines
	if (style.vline.width.first || style.vline.type.first || style.vline.color.first) {
		MscStyle toadd;
		toadd.vline = style.vline;
        for(EIterator i = chart->Entities.begin(); i!=chart->Entities.end(); i++)
            (*i)->status.ApplyStyle(Range(yPos, yPos+height), toadd);
	}

    if (!nudge)
        chart->HideEntityLines(text_cover);
    ArcLabelled::PostPosProcess(autoMarker);
}

void ArcDivider::Draw()
{
    if (nudge) return;
    parsed_label.Draw(text_margin, chart->total.x-text_margin, wide ? 0 : chart->arcVGapAbove);
    //determine widest extent for coverage at the centerline+-style.line.LineWidth()/2;
    const double lw2 = ceil(style.line.LineWidth()/2.);
    Block b(line_margin, chart->total.x-line_margin, centerline - lw2, centerline + lw2);
    Range r = (text_cover * b).GetBoundingBox().x;
    if (r.IsInvalid())  
        chart->Line(XY(line_margin, centerline), XY(chart->total.x-line_margin, centerline), style.line);
    else {
        chart->Line(XY(line_margin, centerline), XY(r.from-chart->compressXGap/2., centerline), style.line);
        chart->Line(XY(r.till+chart->compressXGap/2., centerline), XY(chart->total.x-line_margin, centerline), style.line);
    }
}

//////////////////////////////////////////////////////////////////////////////////////

string ArcParallel::Print(int ident) const
{
    string ss;
    ss << string(ident*2, ' ');
    ss << PrintType() << "\n";
    for (PtrList<ArcList>::const_iterator i = blocks.begin(); i!=blocks.end(); i++) {
        if (i!=blocks.begin())
            ss << string(ident*2+2, ' ') << "---\n";
        ss << (*i)->Print(ident+2);
        if (i!=blocks.end())
            ss << "\n";
    }
    return ss;
};

void ArcParallel::PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level)
{
    at_top_level = top_level;
    for (PtrList<ArcList>::iterator i=blocks.begin(); i != blocks.end(); i++)
        chart->PostParseProcessArcList(**i, false, left, right, number, false);
}

void ArcParallel::Width(EntityDistanceMap &distances)
{
    if (!valid) return;
    EntityDistanceMap d;
    for (PtrList<ArcList>::iterator i=blocks.begin(); i != blocks.end(); i++)
        chart->WidthArcList(**i, d);
    d.CombineLeftRightToPair_Sum(chart->hscaleAutoXGap);
    d.CombineLeftRightToPair_Single(chart->hscaleAutoXGap);
    distances += d;
}

double ArcParallel::Height(AreaList &cover)
{
    heights.clear();
    heights.reserve(blocks.size());
    double height = 0;
    int n = 0;
    for (auto i=blocks.begin(); i != blocks.end(); i++, n++) {
        AreaList cover_block;
        //Each parallel block is compressed without regard to the others
        double h = chart->HeightArcList((*i)->begin(), (*i)->end(), cover_block);
        heights[n] = height = std::max(height, h);
        cover += cover_block;
    }
    return height;
}

void ArcParallel::ShiftBy(double y)
{
    for (auto i=blocks.begin(); i!=blocks.end(); i++)
        chart->ShiftByArcList((*i)->begin(), (*i)->end(), y);
    ArcBase::ShiftBy(y);
}

void ArcParallel::PostPosProcess(double autoMarker)
{
    ArcBase::PostPosProcess(autoMarker);
    int n=0;
    //For automarker, give the bottom of the largest of previous blocks
    for (auto i=blocks.begin(); i!=blocks.end(); i++, n++)
        chart->PostPosProcessArcList(*(*i), 
            n>0 && heights[n-1]>0 ? yPos + heights[n-1] : autoMarker);
}

void ArcParallel::Draw()
{
    for (auto i=blocks.begin(); i != blocks.end(); i++) 
        chart->DrawArcList(*(*i));
}

//////////////////////////////////////////////////////////////////////////////////////

string ArcCommand::Print(int ident) const
{
    string ss;
    ss << string(ident*2, ' ');
    ss << PrintType();
    return ss;
}

//////////////////////////////////////////////////////////////////////////////////////

CommandEntity::CommandEntity(EntityDefList *e, Msc *msc)
    : ArcCommand(MSC_COMMAND_ENTITY, msc) 
{
    full_heading = (e==NULL);
    if (e) {
        entities.splice(entities.end(), *e);
        delete e;
    }
}


string CommandEntity::Print(int ident) const
{
    string ss;
    ss << string(ident*2, ' ');
    ss << "Entity Command";
    if (full_heading) ss<<"(full_heading)";
    for (auto i = entities.begin();i != entities.end(); i++)
        ss << "\n" << (*i)->Print(ident+1);
    return ss;
}

void CommandEntity::AppendToEntities(const EntityDefList &e)
{
    entities.insert(entities.end(), e.begin(), e.end());
}


void CommandEntity::Combine(CommandEntity *ce)
{
    if (!ce) return;
    if (!ce->valid) return;
    //Always keep the line_pos of the heading command
    //If we are already one, keep ours
    if (!full_heading && ce->full_heading)
        file_pos = ce->file_pos;
    if (ce->full_heading) full_heading = true;
    AppendToEntities(ce->entities);
    ce->entities.clear();
}


void CommandEntity::PostParseProcess(EIterator &left, EIterator &right, Numbering &number,
                                     bool top_level)
{
    at_top_level = top_level;
    if (full_heading && !top_level)
        chart->Error.Warning(file_pos.start, "The command 'heading' is specified "
                             "inside a parallel block. May display incorrectly.");
    set<Entity*> explicitly_listed;
    for (auto i_def = entities.begin(); i_def != entities.end(); i_def++) {
        EIterator j_ent = chart->Entities.Find_by_Name((*i_def)->name);
        (*i_def)->itr = j_ent;
        //Make the style of the entitydef fully specified using the accumulated style info in Entity
        (*j_ent)->running_style += (*i_def)->style;  //(*i)->style is a partial style here specified by the user
        (*i_def)->style = (*j_ent)->running_style;	 //(*i)->style now become the full style to use from this point
        (*i_def)->parsed_label.Set((*j_ent)->label, (*i_def)->style.text);

        //Decide, if this entitydef will show an entity or not
        //It can get drawn because we 1) said show=yes, or
        //2) because it is on, we mention it (without show=yes) and it is
        //a full heading.
        (*i_def)->shown = ((*i_def)->show.second && (*i_def)->show.first) || (full_heading && (*j_ent)->shown);
        explicitly_listed.insert(*j_ent);
        //Adjust the running status of the entity. This is just for the Height process
        if ((*i_def)->show.first) 
            (*j_ent)->shown = (*i_def)->show.second;
    }
    //A "heading" command, we have to draw all entities that are on
    //for these we create additional EntityDefs and append them to entities
    if (full_heading) 
        for (auto i = chart->Entities.begin(); i!=chart->Entities.end(); i++) {
            if (!(*i)->shown) continue;
            if (explicitly_listed.find(*i) != explicitly_listed.end()) continue;
            EntityDef *e = new EntityDef((*i)->name.c_str(), chart);
            //fill in all values necessary
            e->implicit = true;
            e->itr = i;
            e->style = (*i)->running_style;	 
            e->parsed_label.Set((*i)->label, e->style.text);
            e->shown = true;
            entities.Append(e);
        }

    //Now we have all entities among "entities" that will show here
    //Go through them and update left, right and the entities' maxwidth
    for (auto i_def = entities.begin(); i_def != entities.end(); i_def++) {
        if (!(*i_def)->shown) continue;
        left = MinMaxByPos(left, (*i_def)->itr, true);
        right = MinMaxByPos(right, (*i_def)->itr, false);
        double w = (*i_def)->Width();
        if ((*(*i_def)->itr)->maxwidth < w) (*(*i_def)->itr)->maxwidth = w;
    }
}


void CommandEntity::Width(EntityDistanceMap &distances)
{
    if (!valid) return;
    //Add distances for entity heading
    EntityDistanceMap d;
    //in PostParseProcess we created an entitydef for all entities shown here. "full_heading" not even checked here
    for (auto i = entities.begin(); i!=entities.end(); i++) {
        //Take entity height into account or draw it if show=on was added
        if ((*i)->shown) {
			const double halfsize = (*(*i)->itr)->maxwidth/2;
            const unsigned index = (*(*i)->itr)->index;
            d.Insert(index, DISTANCE_LEFT, halfsize);
            d.Insert(index, DISTANCE_RIGHT, halfsize);
        }
    }
    //convert left and right requirements (added above) to pairwise ones.
    d.CombineLeftRightToPair_Sum(chart->hscaleAutoXGap);
    distances += d;
}

double CommandEntity::Height(AreaList &cover)
{
    double height = 0;
    //Those entities explicitly listed will have their own EntityDef for this line.
    //Thus their area will be stored there and not in CommandEntity->area
    //But, still put those into "cover" so they can be considered for placement
    //There are other entities shown here, those triggered by a heading command.
    //They have "implicit" set to true. They have no line info and they do not add 
    //their "area" to the allcovers of the chart in EntityDef::PostPosProcess. 
    //Instead we add their area to this->area now
    set<Entity*> explicitly_listed_entities;
    for (auto i = entities.begin(); i!=entities.end(); i++) {
        if ((*i)->shown) {
            const double h = (*i)->Height(cover); //this also adds the cover to the entitydef's area
            if (height <h) height = h;
            if ((*i)->implicit)
                area += (*i)->GetAreaToSearch();
        }
    }
    return height;
}

void CommandEntity::ShiftBy(double y)
{
    for (auto i = entities.begin(); i!=entities.end(); i++) 
        (*i)->ShiftBy(y);
    ArcCommand::ShiftBy(y);
}

void CommandEntity::PostPosProcess(double autoMarker)
{
    for (auto i = entities.begin(); i!=entities.end(); i++) 
        (*i)->PostPosProcess();
    ArcCommand::PostPosProcess(autoMarker);
}

void CommandEntity::Draw()
{
    for (auto i = entities.begin(); i!=entities.end(); i++) 
        (*i)->Draw();
}



//double CommandEntity::DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker)
//{
//    if (!valid) return 0;
//    if (final) yPos = y;
//    if (draw)  y = yPos;
//    if (!draw) geometry.Clear();
//
//    double height = 0;
//    //Those entities explicitly listed, put their cover in their geometry
//    //only the ones added by a heading command to this commandentity will
//    //put anything into this->geometry
//    set<Entity*> explicitly_listed_entities;
//    if (entities) {  //An entity command
//        for (EntityDefList::iterator i = entities->begin(); i!=entities->end(); i++) {
//            //Clear geometry that may have been accumulated by !final rounds
//            if (!draw)
//                (*i)->geometry.Clear();
//            EIterator j = chart->Entities.Find_by_Name((*i)->name);
//            assert(j != chart->NoEntity);
//
//            //Record status and style changes
//            if (final) {
//                EIterator k = chart->Entities.Find_by_Name((*i)->name);
//                assert(k != chart->NoEntity);
//                //Apply changes in style
//                (*k)->status.ApplyStyle(yPos, (*i)->style);
//                //Apply changes in show status
//                if ((*i)->show.first)
//					(*k)->status.SetStatus(yPos, (*i)->show.second ? EntityStatusMap::SHOW_ON : EntityStatusMap::SHOW_OFF);
//            }
//            //Take entity height into account if it gets drawn
//            //It can get drawn because we 1) said show=yes, or
//            //2) because it is on, we mention it (without show=yes) and it is
//            //a full heading.
//            if (((*i)->show.second && (*i)->show.first) ||
//                (full_heading && (*j)->status.GetStatus(y))) {
//                const double h = (*j)->DrawHeight(y, (*i)->geometry, draw, final);
//                //If we are not drawing, we add the cover to the entitydef's geometry
//                if (!draw) {
//                    (*i)->geometry.SetArc(*i);
//                    g += (*i)->geometry; //we add it to the one returned
//                }
//                //We add this entity to the list of entities already done
//                explicitly_listed_entities.insert(*j);
//                if (height <h) height = h;
//            }
//        }
//    }
//
//    if (!full_heading) return height;
//    //A "heading" command, draw all entities that are on
//    for (EntityList::const_iterator i = chart->Entities.begin(); i!=chart->Entities.end(); i++) {
//        if (!(*i)->status.GetStatus(y)) continue;
//        if (explicitly_listed_entities.find(*i) != explicitly_listed_entities.end()) continue;
//        const double h = (*i)->DrawHeight(y, geometry, draw, final);
//        if (height <h) height = h;
//    }
//    if (!draw) {
//        geometry.SetArc(this);
//        g += geometry;
//    }
//    return height;
//}

//void CommandEntity::PostHeightProcess(void)
//{
//    //Add us to the big file positions list
//    if (entities)
//        for (EntityDefList::const_iterator i = entities->begin(); i!=entities->end(); i++)
//            chart->AllArcs[(*i)->file_pos] = *i;
//    chart->AllArcs[file_pos] = this;
//    if (!valid) return;
//	//Add our covers to the big cover list
//    chart->AllCovers.insert(chart->AllCovers.end(), geometry.GetCover().begin(), geometry.GetCover().end());
//    if (entities)   //An entity command
//        for (EntityDefList::const_iterator i = entities->begin(); i!=entities->end(); i++)
//            chart->AllCovers.insert(chart->AllCovers.end(), (*i)->geometry.GetCover().begin(), (*i)->geometry.GetCover().end());
//}
//

//////////////////////////////////////////////////////////////////////////////////////

double CommandNewpage::Height(AreaList &cover) 
{
    Block b(0, chart->total.x, -chart->nudgeSize/2, chart->nudgeSize/2);
    area_draw = b;
    area_draw.mainline = b.y;
    draw_is_different = true; //area is empty - never find this
    return 0;
}

void CommandNewpage::PostPosProcess(double autoMarker)
{
    chart->yPageStart.push_back(yPos);
}

//////////////////////////////////////////////////////////////////////////////////////

double CommandNewBackground::Height(AreaList &cover) 
{
    Block b(0, chart->total.x, -chart->nudgeSize/2, chart->nudgeSize/2);
    area_draw = b;
    area_draw.mainline = b.y;
    draw_is_different = true; //area is empty - never find this
    return 0;
}

void CommandNewBackground::PostPosProcess(double autoMarker)
{
    chart->Background[yPos] = fill;
}

//////////////////////////////////////////////////////////////////////////////////////
void CommandNumbering::PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level)
{
    if ((action & SIZE) && length)
        number.SetSize(length);
    if (action & INCREMENT)
        ++number;
    if (action & DECREMENT)
        --number;
}

//////////////////////////////////////////////////////////////////////////////////////

CommandMark::CommandMark(const char *m, file_line_range ml, Msc *msc) :
    ArcCommand(MSC_COMMAND_MARK, msc), name(m)
{
    map<string, Msc::MarkerType>::iterator i = chart->Markers.find(name);
    if (i != chart->Markers.end()) {
        string msg = "Marker '"+name+"' has already been defined at line ";
        msg << i->second.first.line;
        if (i->second.first.file != chart->current_file)
            msg << " (in input '" + chart->Error.Files[i->second.first.file] + "')";
        msg.append(". Keeping old definition.");
		chart->Error.Error(file_pos.start, msg);
        valid = false;
        return;
    }
    chart->Markers[name].first = ml.start;
    chart->Markers[name].second = -1001;
    offset = 0;
}

bool CommandMark::AddAttribute(const Attribute &a)
{
    if (a.Is("offset")) {
        if (!a.EnsureNotClear(chart->Error, STYLE_ARC)) return true;
        if (!a.CheckType(MSC_ATTR_NUMBER, chart->Error)) return true;
        offset = a.number;
        return true;
    }
    return ArcBase::AddAttribute(a);
}

void CommandMark::AttributeNames(Csh &csh)
{
    ArcBase::AttributeNames(csh);
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"offset", HINT_ATTR_NAME));
}

bool CommandMark::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr,"offset")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable()+"<number>", HINT_ATTR_VALUE, false));
        return true;
    }
    if (ArcBase::AttributeValues(attr, csh)) return true;
    return false;
}

double CommandMark::Height(AreaList &cover) 
{
    Block b(0, chart->total.x, offset-chart->nudgeSize/2, offset+chart->nudgeSize/2);
    area_draw = b;
    area_draw.mainline = b.y;
    draw_is_different = true; //area is empty - never find this
    return 0;
}

void CommandMark::PostPosProcess(double autoMarker)
{
    chart->Markers[name].second = yPos+offset;
}

//////////////////////////////////////////////////////////////////////////////////////

#define EMPTY_MARGIN_X 50
#define EMPTY_MARGIN_Y 5

CommandEmpty::CommandEmpty(Msc *msc) :
    ArcCommand(MSC_COMMAND_EMPTY, msc), parsed_label(msc)
{
    StringFormat format;
    format.Apply("\\pc\\mu(10)\\md(10)\\ml(10)\\mr(10)\\c(255,255,255)\\b\\i");
    parsed_label.Set(string("\\i\\bEmpty chart"), format);
}

void CommandEmpty::Width(EntityDistanceMap &distances)
{
    if (!valid) return;
    const unsigned lside_index = (*chart->Entities.Find_by_Name(LSIDE_ENT_STR))->index;
    const unsigned rside_index = (*chart->Entities.Find_by_Name(RSIDE_ENT_STR))->index;
    const double width = parsed_label.getTextWidthHeight().x + 2*EMPTY_MARGIN_X;
    distances.Insert(lside_index, rside_index, width);
}

double CommandEmpty::Height(AreaList &cover) 
{
    yPos = 0;
    const XY wh = parsed_label.getTextWidthHeight();
    Area a = Block((chart->total.x-wh.x)/2, (chart->total.x+wh.x)/2, EMPTY_MARGIN_Y, EMPTY_MARGIN_Y+wh.y);
    cover += a;
    return wh.y + EMPTY_MARGIN_Y*2;
}

void CommandEmpty::Draw()
{
    const double width  = parsed_label.getTextWidthHeight().x;
    const double height = parsed_label.getTextWidthHeight().y;
    MscLineAttr line;
    line.width.second = 3;
    line.radius.second = 10;

    MscFillAttr fill;
    fill.color.second = MscColorType(0,0,128);
    fill.gradient.second = GRADIENT_BUTTON;

    MscShadowAttr shadow;
    shadow.offset.second = 5;
    shadow.blur.second = 5;

    Block b(XY((chart->total.x-width)/2 , yPos+EMPTY_MARGIN_Y),
            XY((chart->total.x+width)/2 , yPos+EMPTY_MARGIN_Y+height));

    chart->Shadow(b, line, shadow);
    chart->Fill(b, line, fill);
    chart->Line(b, line);
}
