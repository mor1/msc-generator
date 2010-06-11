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

//template class PtrList<ArcBase>;

ArcBase::ArcBase(MscArcType t, Msc *msc) :
    type(t), chart(msc), valid(true), compress(false), parallel(false), yPos(0)
{
    if (msc)
        compress = msc->StyleSets.top().compress;
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

void ArcBase::PostHeightProcess(void)
{
    if (valid)
        chart->AllCovers.insert(chart->AllCovers.end(), geometry.GetCover().begin(), geometry.GetCover().end());
    chart->AllArcs[file_pos] = this;
}


//////////////////////////////////////////////////////////////////////////////////////

ArcLabelled::ArcLabelled(MscArcType t, Msc *msc, const MscStyle &s) :
    ArcBase(t, msc), style(s), parsed_label(msc)
{
    numbering = msc->StyleSets.top().numbering?-999:-1000;
    style.type = STYLE_ARC;
    switch(type) {
    case MSC_ARC_SOLID:
    case MSC_ARC_SOLID_BIDIR:
        style += msc->StyleSets.top()["->"]; break;
    case MSC_ARC_DOTTED:
    case MSC_ARC_DOTTED_BIDIR:
        style += msc->StyleSets.top()[">"]; break;
    case MSC_ARC_DASHED:
    case MSC_ARC_DASHED_BIDIR:
        style += msc->StyleSets.top()[">>"]; break;
    case MSC_ARC_DOUBLE:
    case MSC_ARC_DOUBLE_BIDIR:
        style += msc->StyleSets.top()["=>"]; break;
    case MSC_EMPH_SOLID:
        style += msc->StyleSets.top()["--"]; break;
    case MSC_EMPH_DASHED:
        style += msc->StyleSets.top()["++"]; break;
    case MSC_EMPH_DOTTED:
        style += msc->StyleSets.top()[".."]; break;
    case MSC_EMPH_DOUBLE:
        style += msc->StyleSets.top()["=="]; break;
    case MSC_EMPH_UNDETERMINED_FOLLOW:
        break; /*do nothing*/
    case MSC_ARC_DIVIDER:
        style += msc->StyleSets.top()["---"]; break;
    case MSC_ARC_DISCO:
        style += msc->StyleSets.top()["..."]; break;
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
        StringFormat::ExpandColorAndStyle(label, chart, linenum_label, &style.text, true);
    return this;
}

bool ArcLabelled::AddAttribute(const Attribute &a)
{
    if (a.type == MSC_ATTR_STYLE) {
		style.AddAttribute(a, chart);
        if (style.numbering.first && numbering < 0)
            numbering = style.numbering.second?-999:-1000;
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
        //MSC_ATTR_CLEAR is well handled by faulting both tests below
        if (a.type == MSC_ATTR_NUMBER) {
            numbering = int(a.number);
            return true;
        }
        if (a.type == MSC_ATTR_BOOL) {
            numbering = a.yes?-999:-1000;
            return true;
        }
        chart->Error.Error(a, true, "Value for 'number' must be 'yes', 'no' or a number. Ignoring attribute.");
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
};

string ArcLabelled::Print(int ident) const
{
    string ss;
    ss << string(ident*2, ' ');
    ss << PrintType().append(":").append(label);
    return ss;
};

void ArcLabelled::PostParseProcess(EIterator &left, EIterator &right, int &number, bool top_level)
{
    at_top_level = top_level;
    if (label.length()!=0 && numbering != -1000) {
        if (numbering == -999)
            numbering = number;
        else
            number = numbering;
        style.text.AddNumbering(label, numbering);
        number++;
    }
    parsed_label.Set(label, style.text);
    if (style.compress.first)
        compress = style.compress.second;
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

void ArcSelfArrow::PostParseProcess(EIterator &left, EIterator &right, int &number, bool top_level)
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

double ArcSelfArrow::DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker)
{
    if (!valid) return 0;
    if (final) yPos = y;
    if (draw) y = yPos;
    if (!draw) geometry.Clear();

    double dx = chart->XCoord((*src)->pos);
    double sx = 0;

    const XY xy_e = style.arrow.getWidthHeight(isBidir(), MSC_ARROW_END, chart);
    const XY xy_s = style.arrow.getWidthHeight(isBidir(), MSC_ARROW_START, chart);

    y  += chart->arcVGapAbove;

    //inserts elements into cover only if not drawing
    parsed_label.DrawCovers(sx, dx, y, geometry, draw);
    XY wh(chart->XCoord(0.375), 2*YSize);

    if (!draw) {
        Block b;
        b.y.from = y;
        b.y.till = y + xy_s.y + wh.y + xy_e.y;
        b.x.from = dx;
        b.x.till = dx + wh.x;
        geometry += b;
        b.y.from = y - chart->nudgeSize/2;
        b.y.till = y + wh.y + chart->nudgeSize/2;
        geometry.mainline.Extend(b.y);
        geometry.SetArc(this);
        g += geometry;
    }

    y += xy_s.y;

    if (draw) {
        //If both params are 100 we have traditional style
        //else rounded squares should look symmetric
        if (style.line.radius.second < 0) {
            wh.x *=2;
            chart->_new_path();
            chart->arc(XY(dx, y+YSize), wh, 270, 90, style.line);
        } else {
            chart->ClipRectangle(XY(dx,0), XY(chart->totalWidth, chart->totalHeight));
            chart->rectangle(XY(0, y), XY(dx,y)+wh, style.line);
            chart->UnClip();
        }

        style.arrow.Draw(XY(dx, y+2*YSize), false, isBidir(), MSC_ARROW_END, chart);
        style.arrow.Draw(XY(dx, y        ), true,  isBidir(), MSC_ARROW_START, chart);
    }
    return max(xy_s.x + wh.y + xy_e.x, parsed_label.getTextWidthHeight().y) +
        chart->arcVGapAbove + chart->arcVGapBelow;
}

void ArcSelfArrow::PostHeightProcess(void)
{
    ArcBase::PostHeightProcess();
    if (!valid) return;

    //Check if the entity involved is actually turned on.
    if (!(*src)->status.GetHideStatus(yPos) || 
		!(*src)->status.GetStatus(yPos)) {
        string sss;
        sss << "Entity '" << (*src)->name << "' is";
        sss << " turned off, but referenced here.";
        chart->Error.Warning(file_pos.start, sss, "It will look strange.");
    }
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

void ArcDirArrow::PostParseProcess(EIterator &left, EIterator &right, int &number, bool top_level)
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
    if (!valid) return;
    distances.Insert((*src)->index, (*dst)->index,
                     style.arrow.getWidthHeight(isBidir(), MSC_ARROW_END, chart).x +
                     style.arrow.getWidthHeight(isBidir(), MSC_ARROW_START, chart).x +
                     parsed_label.getTextWidthHeight().x);
}

double ArcDirArrow::DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker)
{
    if (!valid) return 0;
    if (final) yPos = y;
    if (draw) y = yPos;
    if (!draw) geometry.Clear();

    double sx = chart->XCoord((*src)->pos);
    double dx = chart->XCoord((*dst)->pos);

    const XY xy_e = style.arrow.getWidthHeight(isBidir(), MSC_ARROW_END, chart);
    const XY xy_s = style.arrow.getWidthHeight(isBidir(), MSC_ARROW_START, chart);

    y  += chart->arcVGapAbove;

    /* A regular horizontal arrow */
    double aH  = style.arrow.getWidthHeight(isBidir(), MSC_ARROW_MIDDLE, chart).y;

    //If there are middle arrows, make aH be the highest of endType/startType
    //and midType arrows.
    //If not use endType/startType only
    if (middle.size()>0)
        aH = max(aH, max(xy_e.y, xy_s.y));
    else
        aH = max(xy_e.y, xy_s.y);

    double height;
    if (parsed_label.getTextWidthHeight().y) {
        double firstLineHeight = parsed_label.getTextWidthHeight(0).y;
        double furtherLinesHeight = parsed_label.getTextWidthHeight().y - firstLineHeight;

        height = max(firstLineHeight, aH);
        /* Part below the arrow */
        height += max(furtherLinesHeight, aH);
        height += chart->arcVGapAbove + chart->arcVGapBelow;

        //Determine coverage for the text (or draw it if we already draw)
        if (sx<dx)
            parsed_label.DrawCovers(sx + xy_s.x, dx - xy_e.x, y, geometry, draw);
        else
            parsed_label.DrawCovers(dx + xy_e.x, sx - xy_s.x, y, geometry, draw);
        //Exclude the areas covered by the text from further drawing
        //geometry.cover will be empty if we draw but then it is !final anyway
        if (final) chart->HideEntityLines(geometry);
        //determine position of arrow midline
        y += max(aH, firstLineHeight+ARROW_TEXT_VSPACE_ABOVE+style.line.LineWidth()/2);
    } else {
        /* no text */
        height = 2*aH + chart->arcVGapAbove + chart->arcVGapBelow;
        y+=aH;
    }
    if (!draw) {
        // Now y is midline of arrow. Calculate mainline and add cover block for arrow
        Block b(min(sx,dx)+1, max(sx,dx)-1, y-aH, y+aH);
        geometry += b;
        b.y.from = y - chart->nudgeSize/2;
        b.y.till = y + chart->nudgeSize/2;
        geometry.mainline.Extend(b.y);
        geometry.SetArc(this);
        g += geometry;
    }

    /* Draw the line */
    if (draw) {
        //all the entities this (potentially multi-segment arrow visits)
        vector<EIterator> e(1);
        e[0] = src;
        e.insert(e.end(), middle.begin(), middle.end());
        e.push_back(dst);
        //The x-size of the arrow at each stop
        vector<pair<double, double> > asize(1);
        int dir = (sx<dx) ? 1 : -1;
        asize[0] = style.arrow.getWidthForLine(isBidir(), MSC_ARROW_START, chart);
        asize.insert(asize.end(), middle.size(), style.arrow.getWidthForLine(isBidir(), MSC_ARROW_MIDDLE, chart));
        asize.push_back(style.arrow.getWidthForLine(isBidir(), MSC_ARROW_END, chart));

        const double fy = floor(y);
        for (unsigned i=0; i<e.size()-1; i++) {
            const double tsx = chart->XCoord((*e[i]  )->pos);
            const double tdx = chart->XCoord((*e[i+1])->pos);
            chart->line(XY(tsx+dir*asize[i].second, fy), XY(tdx-dir*asize[i+1].first, fy), style.line);
        }
        /* Now the arrow heads */
        style.arrow.Draw(XY(dx, fy), sx<dx, isBidir(), MSC_ARROW_END, chart);
        style.arrow.Draw(XY(sx, fy), sx<dx, isBidir(), MSC_ARROW_START, chart);
    }

    //record where do we have to skip the entityline
    if (final) {
        Range hideEntityLine;
        hideEntityLine = style.arrow.EntityLineCover(XY(dx, y), sx<dx, isBidir(), MSC_ARROW_END, chart);
        (*dst)->status.HideRange(hideEntityLine);
        hideEntityLine = style.arrow.EntityLineCover(XY(sx, y), sx<dx, isBidir(), MSC_ARROW_START, chart);
        (*src)->status.HideRange(hideEntityLine);
    }

    // Do the above two things for segmented arrows
    for (unsigned i=0; i<middle.size(); i++) {
        const double mx = chart->XCoord((*middle[i])->pos);
        if (draw)
            style.arrow.Draw(XY(mx, floor(y)), sx<dx, isBidir(), MSC_ARROW_MIDDLE, chart);
        if (final) {
            Range hideEntityLine = style.arrow.EntityLineCover(XY(mx, y), sx<dx, isBidir(), MSC_ARROW_MIDDLE, chart);
            (*middle[i])->status.HideRange(hideEntityLine);
        }
    }
    return height;
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
	    if ((!(*temp[i])->status.GetHideStatus(y) || 
			 !(*temp[i])->status.GetStatus(y)) &&
            (*temp[i])->name != LSIDE_ENT_STR && (*temp[i])->name != RSIDE_ENT_STR) {
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

//////////////////////////////////////////////////////////////////////////////////////

ArcBigArrow::ArcBigArrow(const ArcDirArrow &dirarrow, const MscStyle &s) :
    ArcDirArrow(dirarrow), content(NULL), height(0)
{
    style = s;
    switch(type) {
    case MSC_ARC_SOLID:
    case MSC_ARC_SOLID_BIDIR:
        style += chart->StyleSets.top()["block->"]; break;
    case MSC_ARC_DOTTED:
    case MSC_ARC_DOTTED_BIDIR:
        style += chart->StyleSets.top()["block>"]; break;
    case MSC_ARC_DASHED:
    case MSC_ARC_DASHED_BIDIR:
        style += chart->StyleSets.top()["block>>"]; break;
    case MSC_ARC_DOUBLE:
    case MSC_ARC_DOUBLE_BIDIR:
        style += chart->StyleSets.top()["block=>"]; break;
    }
    modifyFirstLineSpacing = false;
}

string ArcBigArrow::Print(int ident) const
{
    return ArcDirArrow::Print(ident);
}

void ArcBigArrow::PostParseProcess(EIterator &left, EIterator &right, int &number, bool top_level)
{
    //Determine src and dst entity, check validity of multi-segment ones, add numbering, etc
    ArcDirArrow::PostParseProcess(left, right, number, top_level);
    //Check if we still play
    if (!valid) return;

    if (content)
        chart->PostParseProcessArcList(*content, false, left, right, number, top_level);

    //Finally copy the line attribute to the arrow, as well
    style.arrow.line = style.line;
}

void ArcBigArrow::Width(EntityDistanceMap &distances)
{
    if (!valid) return;
    //Add distances for the content
    if (content) {
        EntityDistanceMap d;
        chart->WidthArcList(*content, d);
        //Now d contains distance requirements within this big arrow
        d.CombineLeftRightToPair_Max(chart->hscaleAutoXGap);
        d.CombineLeftRightToPair_Single(chart->hscaleAutoXGap);
        distances += d;
    }

    const double sext = style.arrow.getBigEndWidthExt(isBidir(), MSC_ARROW_START, chart);
    const double dext = style.arrow.getBigEndWidthExt(isBidir(), MSC_ARROW_END, chart);

    const double tw = parsed_label.getTextWidthHeight().x;

    //Add external distances
    if ((*src)->index  <  (*dst)->index) {
        distances.Insert((*src)->index, DISTANCE_LEFT,  sext);
        distances.Insert((*dst)->index, DISTANCE_RIGHT, dext);
    } else {
        distances.Insert((*src)->index, DISTANCE_RIGHT, sext);
        distances.Insert((*dst)->index, DISTANCE_LEFT,  dext);
    }

    //Collect indeses and distances into arrays
    std::vector<double> margins;
    std::vector<EIterator> iterators;
    iterators.push_back(src);
    margins.push_back(style.arrow.getBigEndWidthMargin(isBidir(), MSC_ARROW_START, chart));
    for (int i=0; i<middle.size(); i++) {
        iterators.push_back(middle[i]);
        margins.push_back(style.arrow.getBigEndWidthMargin(isBidir(), MSC_ARROW_MIDDLE, chart));
    }
    iterators.push_back(dst);
    margins.push_back(style.arrow.getBigEndWidthMargin(isBidir(), MSC_ARROW_END, chart));
    //Sort to make them increasing - direction does not count
    if ((*src)->index > (*dst)->index) {
        std::reverse(iterators.begin(), iterators.end());
        std::swap(margins[0], margins[margins.size()-1]);
    }
    for (int i=0; i<iterators.size()-1; i++) {
        //if neighbours
        if ((*iterators[i])->index + 1 == (*iterators[i+1])->index) {
            distances.Insert((*iterators[i])->index, (*iterators[i+1])->index,
                             margins[i]+margins[i+1] + 3);
        } else {
            distances.Insert((*iterators[i  ])->index,   (*iterators[i  ])->index+1,
                             margins[i] + 3);
            distances.Insert((*iterators[i+1])->index-1, (*iterators[i+1])->index  ,
                             margins[i+1] + 3);
        }
    }

    //Determine if there are multiple segments, if so add text into the appropriate one
    const double lw = style.line.LineWidth();
    if (middle.size()>0)
        switch (style.arrow.GetType(isBidir(), MSC_ARROW_MIDDLE)) {
        case MSC_ARROW_SOLID: /* Filled */
        case MSC_ARROW_EMPTY: /* Non-Filled */
        case MSC_ARROW_LINE: /* Two lines */
        case MSC_ARROW_HALF: /* Unfilled half */
            unsigned index;
            switch (style.text.GetIdent()) {
            case MSC_IDENT_LEFT:   index = 0; break;
            case MSC_IDENT_CENTER: index = iterators.size()/2-1; break;
            case MSC_IDENT_RIGHT:  index = iterators.size()-2; break;
            }
            stext = iterators[index];
            dtext = iterators[index+1];
            sm = margins[index];
            dm = margins[index+1];
            /*Correct for the fact that non-bidir arrows do not have
             **bidir arrowheads in the middle.
             ** TODO: We should do this in the whole Width() by using 2
             ** margins[], but maybe later.. */
            if (!isBidir()) {
                if ((*src)->index < (*dst)->index)
                    sm = 0;
                else
                    dm = 0;
            }
            goto final;
        }
    //if no segments, then span the whole arrow.
    stext = iterators[0];
    dtext = iterators[iterators.size()-1];
    sm = margins[0];
    dm = margins[margins.size()-1];

    final:
    sm =  (sm == 0) ? lw : sm  - lw - 3;
    dm =  (dm == 0) ? lw : dm  - lw - 3;
    distances.Insert((*stext)->index, (*dtext)->index, sm + tw + dm);
}

double ArcBigArrow::DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker)
{
    if (!valid) return 0;
    if (final) yPos = y;
    if (draw) y = yPos;
    if (!draw) geometry.Clear();

    const double ah =style.arrow.Big_yExtent(isBidir(), middle.size()>0, chart);
    const XY twh = parsed_label.getTextWidthHeight();
    const double lw = style.line.LineWidth();

    const double orig_y = y;
    y += ah;

    XY s, d;
    s.x = chart->XCoord((*src)->pos);
    s.y = y;
    d.x = chart->XCoord((*dst)->pos);
    //height has uninitialized value when called first time
    //but we do not care, d is used only when actually drawing
    d.y = y + height - 2*ah;

    //Prepare an array with the visual x positions of segments
    vector<double> xPos;
    xPos.push_back(s.x);
    for (int i=0; i<middle.size(); i++)
        xPos.push_back(chart->XCoord((*middle[i])->pos));
    xPos.push_back(d.x);

    if (draw) {
        //Draw background
        style.arrow.FillBig(xPos, s.y, d.y, isBidir(), chart, style.fill);
        //Draw outline
        style.arrow.DrawBig(xPos, s.y, d.y, isBidir(), chart);
        //draw content if any
        if (content) {
            if (s.x < d.x)
                chart->DrawEntityLines(s.y, d.y-s.y, src, ++EIterator(dst));
            else
                chart->DrawEntityLines(s.y, d.y-s.y, dst, ++EIterator(src));
        }
    }

    Geometry geom;

    //Ok, now do the content
    y += lw; //+chart->emphVGapInside;

    //Add text cover. stext, dtext, sm, dm values calculated in Width()
    parsed_label.DrawCovers(chart->XCoord((*stext)->pos) + sm,
                            chart->XCoord((*dtext)->pos) - dm,
                            y, geom, draw);

    //If final position, cover the entity lines where text goes
    if (final) chart->HideEntityLines(geom);
    //store the uppermost level where content can come
    double low_y = y;
    //Advance label height
    if (twh.y == 0)
        y += Label("M", chart, style.text).getTextWidthHeight().y;
    else
        y += twh.y;
    //Draw content if any
    if (content)
        y += chart->PlaceDrawListUnder(content->begin(), content->end(),
                                       low_y, y, geom, geom, draw, final);

    //Final advance of linewidth, gap and arrow tips
    y += lw; //+ chart->emphVGapInside;
    y += ah;

    height = y - orig_y;

    if (!draw) {
        //Generate one block for cover
        Block box(s.x, d.x, orig_y+ah, y-ah);
        geometry += box;
        style.arrow.CoverBig(xPos, orig_y+ah, y-ah, isBidir(), chart, geometry);
        geometry.SetArc(this);
        g += geometry;
    }

    return height;
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
    ArcArrow(t, msc, msc->StyleSets.top()["vertical"]), pos(*p),
    ypos(2)
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
    aMarker = -1;
    //overwrite the sty;e set by ArcArrow
    style = msc->StyleSets.top()["vertical"];
    switch(type) {
    case MSC_ARC_SOLID:
    case MSC_ARC_SOLID_BIDIR:
        style += chart->StyleSets.top()["vertical->"]; break;
    case MSC_ARC_DOTTED:
    case MSC_ARC_DOTTED_BIDIR:
        style += chart->StyleSets.top()["vertical>"]; break;
    case MSC_ARC_DASHED:
    case MSC_ARC_DASHED_BIDIR:
        style += chart->StyleSets.top()["vertical>>"]; break;
    case MSC_ARC_DOUBLE:
    case MSC_ARC_DOUBLE_BIDIR:
        style += chart->StyleSets.top()["vertical=>"]; break;
    case MSC_EMPH_SOLID:
        style += chart->StyleSets.top()["vertical--"]; break;
    case MSC_EMPH_DASHED:
        style += chart->StyleSets.top()["vertical++"]; break;
    case MSC_EMPH_DOTTED:
        style += chart->StyleSets.top()["vertical.."]; break;
    case MSC_EMPH_DOUBLE:
        style += chart->StyleSets.top()["vertical=="]; break;
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

void ArcVerticalArrow::PostParseProcess(EIterator &left, EIterator &right,
                                        int &number, bool top_level)
{
    if (!valid) return;
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
	if (!valid) return;
    //No extra space requirement
    if (!makeroom) return;
    //The offset is ignored during the process of setting space requirements
    const XY twh = parsed_label.getTextWidthHeight();
    const double lw = style.line.LineWidth();
    double width = twh.y;
    if (width==0)
        width = Label("M", chart, style.text).getTextWidthHeight().y;
    width += 2*lw + 2*chart->hscaleAutoXGap;

    const double aw =style.arrow.Big_yExtent(isBidir(), false, chart);
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

void ArcVerticalArrow::PostHeightProcess(void)
{
    ArcBase::PostHeightProcess();
	//geometry is empty here, so we will have to add our stuff to chart->AllCovers later in this function
    if (!valid) return;

    //Here we are sure markers are OK
    if (src == MARKER_HERE_STR)
        ypos[0] = yPos;
    else if (src != MARKER_PREV_PARALLEL_STR)
        ypos[0] = chart->Markers.find(src)->second.second;
    else if (aMarker>=0)
        ypos[0] = aMarker;
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
    else if (aMarker>=0)
        ypos[1] = aMarker;
    else {
        chart->Error.Error(file_pos.start, "Vertical with no markers cannot take its size from the preceeding blocks."
                            " Ignoring vertical arrow.",
                            "Try putting it into a later block.");
        valid = false;
        return;
    }

    if (!readfromleft)
        swap(ypos[0], ypos[1]);

    const XY twh = parsed_label.getTextWidthHeight();
    double arrow_x_size;
    arrow_x_size =  style.arrow.getBigEndWidthExt(isBidir(), MSC_ARROW_START, chart);
    arrow_x_size += style.arrow.getBigEndWidthExt(isBidir(), MSC_ARROW_END, chart);
    arrow_x_size += style.arrow.getBigEndWidthMargin(isBidir(), MSC_ARROW_START, chart);
    arrow_x_size += style.arrow.getBigEndWidthMargin(isBidir(), MSC_ARROW_END, chart);
    if (arrow_x_size + twh.x > fabs(ypos[0]-ypos[1])) {
        if (arrow_x_size>0)
            chart->Error.Warning(file_pos.start, "Size of vertical element is smaller than needed for text and arrow.",
                                 "May look strange.");
        else
            chart->Error.Warning(file_pos.start, "Size of vertical element is smaller than needed for text.",
                                 "May look strange.");
    }
	double x, width;
	CalculateXandWidth(x, width);

	//Generate one block for cover
    Geometry geom;
    Block box(min(ypos[0], ypos[1]), max(ypos[0], ypos[1]), x-width/2, x+width/2);
    geom += box;
    style.arrow.CoverBig(ypos, x-width/2, x+width/2, isBidir(), chart, geom);
    //Ok, now rotate it by 90 and copy to geometry
    for(std::set<Block>::const_iterator i=geom.GetCover().begin(); i!=geom.GetCover().end(); i++) {
        Block b;
        b.x = i->y;
        b.y = i->x;
        b.arc = this;
        geometry += b;
    }
    chart->AllCovers.insert(chart->AllCovers.end(), geometry.GetCover().begin(), geometry.GetCover().end());
}

//Can be called after postparseprocess
void ArcVerticalArrow::CalculateXandWidth(double &x, double &width) const
{
    const XY twh = parsed_label.getTextWidthHeight();
    const double lw = style.line.LineWidth();
    width = twh.y;
    if (width==0)
        width = Label("M", chart, style.text).getTextWidthHeight().y;
    width += 2*lw;

    const double aw =style.arrow.Big_yExtent(isBidir(), false, chart)/2;
    x = chart->XCoord((*pos.entity1)->pos);
    const double gap = chart->hscaleAutoXGap;
    switch (pos.pos) {
    default:
    case VertXPos::POS_AT: break;
    case VertXPos::POS_CENTER: x = (x + chart->XCoord((*pos.entity2)->pos))/2; break;
    case VertXPos::POS_LEFT_BY:     x -= width/2 + aw + gap; break;
    case VertXPos::POS_RIGHT_BY:    x += width/2 + aw + gap; break;

    case VertXPos::POS_LEFT_SIDE:   x -= width/2 + gap; break;
    case VertXPos::POS_RIGHT_SIDE:  x += width/2 + gap; break;
    };
    x += offset;
};


//We do not store anything in here
double ArcVerticalArrow::DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker)
{
    if (!valid) return 0;
    if (final) {
        yPos = y;
        aMarker = autoMarker;
    }
    if (!draw) return 0; //all relevant calculation happens in postheightprocess

	double x, width;
	CalculateXandWidth(x, width);

    if (readfromleft)
        chart->Rotate90(x-width/2, x+width/2, false);
    else
        chart->Rotate90(ypos[0], ypos[1], true);
    //Draw background
    style.arrow.FillBig(ypos, x-width/2, x+width/2, isBidir(), chart, style.fill);
    //Draw outline
    style.arrow.DrawBig(ypos, x-width/2, x+width/2, isBidir(), chart);
    Geometry dummy;
    style.arrow.ClipBig(ypos, x-width/2, x+width/2, isBidir(), chart);
    parsed_label.DrawCovers(min(ypos[0], ypos[1]), max(ypos[0], ypos[1]),
                            x-width/2+style.line.LineWidth(), dummy, draw, true);
    chart->UnClip();
    if (readfromleft)
        chart->Rotate90(x-width/2, x+width/2, true);
    else
        chart->Rotate90(ypos[0], ypos[1], false);

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////

ArcEmphasis::ArcEmphasis(MscArcType t, const char *s, file_line_range sl,
                         const char *d, file_line_range dl, Msc *msc) :
    ArcLabelled(t, msc, msc->StyleSets.top()["emptybox"]),
    emphasis(NULL), follow(true), first(NULL), height(0), total_height(0),
    pipe(false), pipe_connect_left(false), pipe_connect_right(false)
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
    style = chart->StyleSets.top()["pipe"];
    switch (type) {
    case MSC_EMPH_SOLID:
        style += chart->StyleSets.top()["pipe--"]; break;
    case MSC_EMPH_DASHED:
        style += chart->StyleSets.top()["pipe++"]; break;
    case MSC_EMPH_DOTTED:
        style += chart->StyleSets.top()["pipe.."]; break;
    case MSC_EMPH_DOUBLE:
        style += chart->StyleSets.top()["pipe=="]; break;
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
        style += chart->StyleSets.top()["box"];
    return this;
}


bool ArcEmphasis::AddAttribute(const Attribute &a)
{
    if (a.type == MSC_ATTR_STYLE) return ArcLabelled::AddAttribute(a);
    if (a.Is("color")) {
        return style.fill.AddAttribute(a, chart, style.type);
    }
    return ArcLabelled::AddAttribute(a);
};

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
                                   int &number, bool top_level)
{
    //Add ourselves as the first element in follow.
    follow.push_front(this);

    EIterator e1=src;      //src.pos < dst.pos guaranteed in constructor (if none is NULL)
    EIterator e2=dst;
    //OK, now expand further if there are following boxes
    for (PtrList<ArcEmphasis>::iterator i = follow.begin(); i!=follow.end(); i++) {
        //Add numbering, if needed
        (*i)->ArcLabelled::PostParseProcess(left, right, number, top_level);
        if ((*i)->emphasis)
            chart->PostParseProcessArcList(*((*i)->emphasis), false, e1, e2, number, top_level);
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
    if (!valid) return;
    if (pipe) {
        WidthPipe(distances);
        return;
    }
    EntityDistanceMap d;
    double width = 0;
    for (PtrList<ArcEmphasis>::iterator i = follow.begin(); i!=follow.end(); i++) {
        if ((*i)->emphasis)
            chart->WidthArcList(*((*i)->emphasis), d);
        width = max(width, (*i)->parsed_label.getTextWidthHeight().x);
    }

    //Now d contains distance requirements within this emph box series
    //And width contains the widest label

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

void ArcEmphasis::WidthPipe(EntityDistanceMap &distances)
{
    if (!valid) return;
    EntityDistanceMap d;
    double width = 0;
    if (emphasis)
        chart->WidthArcList(*emphasis, d);

    for (PtrList<ArcEmphasis>::iterator i = follow.begin(); i!=follow.end(); i++) {
        width = (*i)->parsed_label.getTextWidthHeight().x;
        (*i)->left_space = d.Query((*(*i)->src)->index, DISTANCE_LEFT) + 3;
        (*i)->right_space = d.Query((*(*i)->dst)->index, DISTANCE_RIGHT) + 3;

        if ((*i)->src==(*i)->dst) {
            //This can happen only if the pipe contains one segment only i == follow.begin
            //so no need to check pipe_connect flags
            (*i)->left_space  = max(width/2, (*i)->left_space)  + (*i)->style.line.LineWidth() +
                (*i)->style.line.radius.second;
            (*i)->right_space = max(width/2, (*i)->right_space) + (*i)->style.line.LineWidth() +
                (*i)->style.line.radius.second + (*i)->style.shadow.offset.second;
        } else {
            (*i)->left_space  += (*i)->style.line.LineWidth() + (*i)->style.line.radius.second;
            (*i)->right_space += (*i)->style.line.LineWidth() + (*i)->style.line.radius.second +
                (*i)->style.shadow.offset.second;
            //Check if we are connecting to a neighbour pipe segment
            if ((*i)->pipe_connect_left) (*i)->left_space = 0;
            if ((*i)->pipe_connect_right) (*i)->right_space = 0;
            distances.Insert((*(*i)->src)->index, (*(*i)->dst)->index,
                             width + (*i)->left_space + (*i)->right_space);
        }
    }
    distances.Insert((*src)->index, DISTANCE_LEFT, left_space);
    distances.Insert((*dst)->index, DISTANCE_RIGHT, right_space);

    //Finally add the requirements of the content
    d.CombineLeftRightToPair_Max(chart->hscaleAutoXGap);
    d.CombineLeftRightToPair_Single(chart->hscaleAutoXGap);
    distances += d;
}

//Draw a pipe, this is called for each segment
//topside is the bigger part of the pipe
//backside is the small oval visible form the back of the pipe
//this->yPos is the outer edge of the top line
//this->height includes the upper linewidth, emphvgapinside, content, lower emphvgapinside, and for pipes the lower linewidth, too
//this->left_space and right_space includes linewidth
void ArcEmphasis::DrawPipe(bool topSideFill, bool topSideLine, bool backSide, bool text)
{
    if (!valid || !pipe) return;
    //ls and ld will point to the middle of the line
    const double lw = style.line.LineWidth();
    XY ls, ld;
    ls.y = ceil(yPos + lw/2);
    ld.y = floor(yPos + height - lw/2);
    ls.x = chart->XCoord((*src)->pos) - left_space;
    ld.x = chart->XCoord((*dst)->pos) + right_space;

    XY cs(ls.x, (ls.y+ld.y)/2);
    XY cd(ld.x, (ls.y+ld.y)/2);
    XY wh(style.line.radius.second*2, ld.y-ls.y);
    XY shift(style.line.radius.second+lw, lw/2);

    if (backSide) {
        chart->shadow(ls, ld+XY(style.line.radius.second/2, lw/2), style.shadow, 0, false);

		//The back of the main pipe
		chart->_arc_path(cs, wh, 270, 90, 0, LINE_SOLID, true);
        chart->_arc_path(cd, wh, 90, 270, 0, LINE_SOLID);
        chart->Clip();
        chart->filledRectangle(ls-shift, ld+shift, style.fill, 0);
        chart->UnClip();

		//the back of the small ellipsis visible from the side
		MscFillAttr fill = style.fill;
        if (fill.gradient.second == GRADIENT_UP)
            fill.gradient.second = GRADIENT_DOWN;
        else if (fill.gradient.second == GRADIENT_DOWN)
            fill.gradient.second = GRADIENT_UP;
        chart->_arc_path(cd, wh, 0, 360, 0, LINE_SOLID);
        chart->Clip();
        chart->filledRectangle(ls-shift, ld+shift, fill, 0);
        chart->UnClip();

        //Draw the backside line
        chart->SetLineAttr(style.line);
        chart->_arc_path(cd, wh, 270, 90, style.line.width.second, style.line.type.second); //and not line.LineWidth()!
        cairo_stroke(chart->GetContext());
        chart->_arc_path(cs, wh, 270, 90, style.line.width.second, style.line.type.second); //and not line.LineWidth()!
        cairo_stroke(chart->GetContext());
    }
    if (topSideFill) {
		MscFillAttr fill = style.fill;
        fill.color.second.a = unsigned(style.solid.second) * unsigned(fill.color.second.a) / 255;
        chart->_arc_path(cs, wh, 270, 90, 0, LINE_SOLID, true);
        chart->_arc_path(cd, wh, 90, 270, 0, LINE_SOLID);
        chart->Clip();
        chart->filledRectangle(ls-shift, ld+shift, fill, 0);
        chart->UnClip();
    }
    if (topSideLine) {
        chart->_arc_path(cs, wh, 90, 270, style.line.width.second, style.line.type.second);
        cairo_new_sub_path(chart->GetContext());
        chart->_arc_path(cd, wh, 90, 270, style.line.width.second, style.line.type.second);
        chart->SetLineAttr(style.line);
        cairo_stroke(chart->GetContext());
        chart->line(ls, XY(ld.x, ls.y), style.line);
        chart->line(XY(ls.x, ld.y), ld, style.line);
    }
    if (text) {
        Geometry dummy;
        double curve_gap = 0;
        if (style.text.GetIdent() == MSC_IDENT_RIGHT)
            curve_gap = style.line.radius.second;
        parsed_label.DrawCovers(ls.x, ld.x-curve_gap,
                                yPos + style.line.LineWidth() + chart->emphVGapInside,
                                dummy, true);
    }
}

//Will only be called for the first box of a multi-segment box series
//geometry conatins cover for label (if any), all of the labels in the first box's geometry (rest empty)
//geomery_all is set only the first box, and is a full cover for all, as well
double ArcEmphasis::DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker)
{
    if (!valid) return 0;
    y += chart->emphVGapOutside;
    if (final) yPos = y;
    if (draw) y = yPos;

    //A few explanations of the variables exact meaning
    //total_orig_y points to the upper edge of the upper line
    //total_height do not contain the emphvgapoutside, but includes linewidth
    //left_space and right_space includes linewidth
    //height includes the upper linewidth, emphvgapinside, content, lower emphvgapinside, and for pipes the lower linewidth, too
    //yPos is pointing to the upper edge of the upper line
    const double total_orig_y = y;

    if (pipe) {
        //If we are drawing, first iterate through the segments and draw their backside
        if (draw)
            for (PtrList<ArcEmphasis>::iterator i = follow.begin(); i!=follow.end(); i++)
                //Draw the topside fill only if the pipe is at least a bit transparent. Else it will be later covered anyway.
                //Draw the topside line only if pipe is fully transparent. Else we may cover the line.
                //Draw the backside in any case.
                //Do not draw text
                (*i)->DrawPipe((*i)->style.solid.second < 255, (*i)->style.solid.second == 0, true, false);
        //if we just calculate collect the cover for those labels that are not on a fully opaque segment
        //Then we draw the content of the first segment's emphasis
        Geometry geom_labels;
        double max_lineWidth = 0;
        double lowest_label_bottom = total_orig_y;
        double lowest_line_bottom;
        if (!draw) {
            //Determine thickest line for precise pipe alignment
            for (PtrList<ArcEmphasis>::iterator i = follow.begin(); i!=follow.end(); i++)
                if (max_lineWidth < (*i)->style.line.LineWidth()) max_lineWidth = (*i)->style.line.LineWidth();
            lowest_line_bottom = total_orig_y + max_lineWidth + chart->emphVGapInside;
            //Collect cover information from labels and linewidth, so compression of content arrows can be done
            for (PtrList<ArcEmphasis>::iterator i = follow.begin(); i!=follow.end(); i++) {
                //Set this variables in each segment
                (*i)->yPos = total_orig_y;
                y = total_orig_y + max_lineWidth/2 + (*i)->style.line.LineWidth()/2 + chart->emphVGapInside;
                (*i)->geometry.Clear();
                const double lsx = chart->XCoord((*(*i)->src)->pos) - (*i)->left_space;
                const double ldx = chart->XCoord((*(*i)->dst)->pos) + (*i)->right_space;
                //do not draw here, only add text cover
                // ... but even omit text cover for pipes if the pipe is fully opaque,
                //     in that case content can be drawn at same position as label - opaque pipe will cover anyway
                if (style.solid.second < 255 ) {
                    Geometry geom_this_label;
                    double curve_gap = 0;
                    if ((*i)->style.text.GetIdent() == MSC_IDENT_RIGHT)
                        curve_gap = (*i)->style.line.radius.second;
                    (*i)->parsed_label.DrawCovers(lsx, ldx-curve_gap, y, geom_this_label, draw);
                    y += (*i)->parsed_label.getTextWidthHeight().y;
                    if (lowest_label_bottom < y) lowest_label_bottom = y;

                    if (final) {
                        //Add cover block only to this->geometry not to g
                        //We collect geometries of all follow's the geometry of the first box
                        geom_this_label.SetArc(*i);
                        geom_this_label.SetFindType(Block::FIND_NONE);
                        (*i)->geometry += geom_this_label;
                    }
                    geom_labels += geom_this_label;
                }
            }
            //check if thick lined segments having no label have lower still
            if (lowest_label_bottom < lowest_line_bottom)
                lowest_label_bottom = lowest_line_bottom;
            y = lowest_label_bottom;
        } /* if (!draw) */

        //Draw content arrows if any. If not yet drawing, then just calculate their pos
        if (emphasis)
            y += chart->PlaceDrawListUnder(emphasis->begin(), emphasis->end(),
                                           lowest_line_bottom, lowest_label_bottom,
                                           geom_labels, g, draw, final);
        //now y contains the bottom of the content arrows
        //draw the top side of the pipe and the label
        for (PtrList<ArcEmphasis>::iterator i = follow.begin(); i!=follow.end(); i++) {
            if (draw)
                //Draw the topside fill only if the pipe is not fully transparent.
                //Draw the topside line in any case
                //Do not draw the backside (that may content arrow lines already drawn)
                //Draw the text
                (*i)->DrawPipe((*i)->style.solid.second > 0, true, false, true);
            else if ((*i)->style.solid.second == 255) {
                //if not yet drawing but we are fully opaque adjust y to be at least as large as the sum of the texts
                //(content height may be smaller than labels)
                const double covered_text_y = total_orig_y + (*i)->style.line.LineWidth() +
                    chart->emphVGapInside + (*i)->parsed_label.getTextWidthHeight().y;
                if (y < covered_text_y)
                    y = covered_text_y;
            }
        }
        y += chart->emphVGapInside + max_lineWidth;
        if (!draw) {
            const double bottom = y;
            //Now bottom contains the bottom of the pipes (inner edge of line)
            //y will be updated to be the bottom of the pipes (linewidth and shadow-wise)
            for (PtrList<ArcEmphasis>::iterator i = follow.begin(); i!=follow.end(); i++) {
                const double mybottom = bottom + (*i)->style.shadow.offset.second;
                if (y<mybottom) y = mybottom;
                //Set variables in each segment if this is the final position
                //Height does not include the lower linewidth or shadow
                (*i)->height = bottom - total_orig_y;
                //Add a frame/box for tracking for the entire segment
                const double lsx = chart->XCoord((*(*i)->src)->pos) - (*i)->left_space;
                const double ldx = chart->XCoord((*(*i)->dst)->pos) + (*i)->right_space;
                Block box(lsx, ldx, total_orig_y, bottom, *i);
                //If we have content and not fully opaque, draw only the frame of the box for tracking
                if (final) {
                    if (emphasis && (*i)->style.solid.second < 255)
                        box.drawType = Block::DRAW_FRAME;
                    (*i)->geometry += box;
                }
                //Ok, now add a box to g to be returned as cover. Here we also include shadows
                box.x.till += (*i)->style.shadow.offset.second;
                box.y.till += (*i)->style.shadow.offset.second;
                g += box;
            }
            //y now contains the full bottom, including shadows and linewidths
            g.mainline.Extend(Range(total_orig_y, y));
        } /* if (!draw) */
    } else { /* if (pipe) */
        //For boxes we cycle through the segments, drawing/calculating labels and content alike
        //First set a few constants
        //s and d are the inner edges of the lines of the whole box
        const XY lw(style.line.LineWidth(), style.line.LineWidth());
        const XY lw2(lw.x/2, lw.y/2);
        XY s, d;
        s.x = chart->XCoord((*src)->pos) - left_space + lw.x;
        s.y = y + lw.y;
        d.x = chart->XCoord((*dst)->pos) + right_space - lw.x - style.shadow.offset.second;
        //height has uninitialized value when this function is called the first time
        //but we do not care, d is used only when actually drawing
        //height includes the upper linewidth, emphvgapinside, content, lower emphvgapinside,
        //but for boxes not lower linewidth (that is just for pipes)
        d.y = y + total_height - lw.y - style.shadow.offset.second;

        //First draw the background and lines
        if (draw) {
            //for boxes draw background for each segment, then separator lines, then bounding rectangle lines
            //First draw the shadow. If we have non-opaque fill colors, draw also
            //"behind" the box
            bool clipShadow = true;
            for (PtrList<ArcEmphasis>::const_iterator i = follow.begin(); i!=follow.end(); i++)
                if ((*i)->style.fill.color.second.a<255) {
                    clipShadow = false;
                    break;
                }
            chart->shadow(s-lw2, d+lw2, style.shadow, style.line.radius.second, clipShadow);
            //First do a clip region for the overall box (for round corners)
            chart->ClipRectangle(s, d, style.line.radius.second);
            //ls and ld are the inner edges of the lines of the local box (that of (*i))
            XY ls = s, ld = d;
            for (PtrList<ArcEmphasis>::const_iterator i = follow.begin(); i!=follow.end(); i++) {
                ls.y = (*i)->yPos + (*i)->style.line.LineWidth();
                ld.y = (*i)->yPos + (*i)->height;
                //Increase the fill area downward by half of the linewidth below us
                PtrList<ArcEmphasis>::const_iterator next = i;
                next++;
                if (next==follow.end())
                    ld.y += style.line.LineWidth()/2;
                else
                    ld.y += (*next)->style.line.LineWidth()/2;
                //Draw square-corenered rectangles, radius = 0
                chart->filledRectangle(ls, ld, (*i)->style.fill, 0);
                //if there are contained entities, draw entity lines
                if ((*i)->emphasis)
                    chart->DrawEntityLines(ls.y, ld.y-ls.y, src, ++EIterator(dst));
            }
            //Draw box lines - Cycle only for subsequent boxes
            for (PtrList<ArcEmphasis>::const_iterator i = ++(follow.begin()); i!=follow.end(); i++) {
                ls.y = ld.y = ceil((*i)->yPos + (*i)->style.line.LineWidth()/2);
                chart->line(ls, ld, (*i)->style.line);
            }
            chart->UnClip();
            //Finally draw the overall line around the box using original s and d
            chart->rectangle(s-lw2, d+lw2, style.line);
        } /* if (draw) */
        for (PtrList<ArcEmphasis>::iterator i = follow.begin(); i!=follow.end(); i++) {
            // y now points to the *top* of the line of the top edge of this box
            // if we are pipe, we draw the segment side-by side, so we reset y here
            if (draw) y = (*i)->yPos;
            else (*i)->geometry.Clear();
            y = ceil(y);
            const double orig_y = y;
            Geometry geom_label;

            //Advance upper line and spacing
            y += (*i)->style.line.LineWidth() + chart->emphVGapInside;
            //Add text cover & draw if necessary
            (*i)->parsed_label.DrawCovers(s.x, d.x, y, geom_label, draw);
            if (final) {
                //Add label cover block only to this->geometry not to g
                //Only if box has content. Else we just add a rectangle covering all of it later
                if ((*i)->emphasis) {
                    geom_label.SetArc(*i);
                    geom_label.SetFindType(Block::FIND_NONE);
                    (*i)->geometry += geom_label;
                }
                //If final position, cover the entity lines where text goes
                chart->HideEntityLines(geom_label);
            }
            //Advance label height
            double th = (*i)->parsed_label.getTextWidthHeight().y;
            y += th;
            //Draw arrows if any under the label (place cover in geom_label, which we will discard)
            if ((*i)->emphasis)
                y += chart->PlaceDrawListUnder((*i)->emphasis->begin(), (*i)->emphasis->end(),
                                               y - th, y, geom_label, geom_label, draw, final);

            y += chart->emphVGapInside;

            //Set variables to store for later to draw the box lines and bkg above
            if (final) {
                (*i)->yPos = orig_y;
                (*i)->height = y - orig_y;
                //Add a frame/box for tracking for the entire segment
                Block box(s.x-lw.x+style.shadow.offset.second, d.x+lw.x, orig_y, y, *i);
                //If we are the last in the follow series extend to cover bottom shadow
                if (++PtrList<ArcEmphasis>::iterator(i) == follow.end())
                    box.y.till += lw.y + style.shadow.offset.second;
                //If we have content, draw only the frame of the box for tracking
                if ((*i)->emphasis)
                    box.drawType = Block::DRAW_FRAME;
                (*i)->geometry += box;
            }
        } /* for cycle through segments */
        //Final advance of linewidth
        y = ceil(y + lw.y + style.shadow.offset.second);

        //Generate a single block covering the entire box series to be returned for compression
        if (!draw) {
            Block box(s.x-lw.x, d.x+lw.x+style.shadow.offset.second, total_orig_y, y);
            g += box;
            g.mainline.Extend(box.y);
        }
    } /* else if pipe */

    if (!draw)
        total_height = y - total_orig_y;

    return total_height + 2*chart->emphVGapOutside;
}

//Will only be called for the first box of a multi-segment box series
void ArcEmphasis::PostHeightProcess(void)
{
    if (pipe) {
        //For pipes we first add those covers to chart->AllCovers that are at least a bit transparent,
        //then the content (only in the first segment)
        //then those segments, which are fully opaque
        //(this is because search is backwards and this arrangement fits the visual best
        for (PtrList<ArcEmphasis>::iterator i = follow.begin(); i!=follow.end(); i++) {
            if (!(*i)->valid || (*i)->style.solid.second == 255) continue;
            chart->AllCovers.insert(chart->AllCovers.end(), (*i)->geometry.GetCover().begin(), (*i)->geometry.GetCover().end());
            chart->AllArcs[(*i)->file_pos] = *i;
        }
        if (emphasis)
            for (ArcList::iterator j = emphasis->begin(); j!=emphasis->end(); j++)
                (*j)->PostHeightProcess();
        for (PtrList<ArcEmphasis>::iterator i = follow.begin(); i!=follow.end(); i++) {
            if (!(*i)->valid || (*i)->style.solid.second < 255) continue;
            chart->AllCovers.insert(chart->AllCovers.end(), (*i)->geometry.GetCover().begin(), (*i)->geometry.GetCover().end());
            chart->AllArcs[(*i)->file_pos] = *i;
        }
    } else
        //For boxes we always add the background first then the content
        //And we do this for each segment sequentially
        for (PtrList<ArcEmphasis>::iterator i = follow.begin(); i!=follow.end(); i++)
            if ((*i)->valid) {
                chart->AllCovers.insert(chart->AllCovers.end(), (*i)->geometry.GetCover().begin(), (*i)->geometry.GetCover().end());
                chart->AllArcs[(*i)->file_pos] = *i;
                if ((*i)->emphasis)
                    for (ArcList::iterator j = (*i)->emphasis->begin(); j!=(*i)->emphasis->end(); j++)
                        (*j)->PostHeightProcess();
            }
}

//////////////////////////////////////////////////////////////////////////////////////

ArcDivider::ArcDivider(MscArcType t, Msc *msc) :
    ArcLabelled(t, msc, msc->StyleSets.top()["divider"]),
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

void ArcDivider::PostParseProcess(EIterator &left, EIterator &right, int &number, bool top_level)
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


double ArcDivider::DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker)
{
    if (!valid) return 0;
    if (final) yPos = y;
    if (draw) y = yPos;
    if (!draw) geometry.Clear();

    if (nudge) {
        if (!draw) {
            Block b(0, chart->totalWidth, y, y + chart->nudgeSize);
            geometry += b;
            geometry.mainline.Extend(b.y);
            geometry.SetArc(this);
            g += geometry;
        }
        return  chart->nudgeSize;
    }

    double charheight = Label("M", chart, style.text).getTextWidthHeight().y;
    if (!wide) y += chart->arcVGapAbove;

    XY wh = parsed_label.getTextWidthHeight();
    if (!wh.y) wh.y = charheight;
    const double lineYPos = y+wh.y/2;

    const double text_margin = wide ? 0 : chart->XCoord(MARGIN*1.3);
    const double line_margin = chart->XCoord(MARGIN);
    Geometry text_cover;
    //Never draw here, just obtain the cover
    //(DrawCovers does not return a cover if drawing)
    parsed_label.DrawCovers(text_margin, chart->totalWidth-text_margin, y,
                            text_cover, false);
    if (draw) {
        parsed_label.DrawCovers(text_margin, chart->totalWidth-text_margin, y,
                                text_cover, true);
        //determine widest extent for coverage at the lineYpos+- style.line.LineWidth()/2;
        Range yRange(lineYPos - ceil(style.line.LineWidth()/2.), lineYPos + ceil(style.line.LineWidth()/2.));
        Range xRange(chart->totalWidth-line_margin, line_margin);
        //geometry so far contains the cover of the text
        for (std::set<Block>::const_iterator i = text_cover.GetCover().begin(); i!=text_cover.GetCover().end(); i++)
            if (yRange.Overlaps(i->y, chart->compressYGap))
                xRange.Extend(i->x);
        chart->line(XY(line_margin, lineYPos),
                    XY(xRange.from-chart->compressXGap/2., lineYPos), style.line);
        if (xRange.from < xRange.till) // Text covers the line - two part drawing
            chart->line(XY(xRange.till+chart->compressXGap, lineYPos),
                        XY(chart->totalWidth-line_margin, lineYPos), style.line);
    }

    //Hide entity lines where text shows
    if (final)
        chart->HideEntityLines(text_cover);

    if (!draw) {
        geometry += text_cover;
        //Add a cover block for the line, if one exists
        if (style.line.type.second != LINE_NONE && style.line.color.second.valid && style.line.color.second.a>0)
            geometry += Block(line_margin, chart->totalWidth-line_margin,
                              lineYPos - style.line.LineWidth()*2, lineYPos + style.line.LineWidth()*2);
        Range r(lineYPos-charheight/2, lineYPos+charheight/2);
        geometry.mainline.Extend(r);
        geometry.SetArc(this);
        g+=geometry;
    }

    double height = wh.y;
    if (!wide)
        height += chart->arcVGapAbove + chart->arcVGapBelow;
    if (!final) return height;
	//If there is a vline in the current style, add that to entitylines
	if (style.vline.width.first || style.vline.type.first || style.vline.color.first) {
		MscStyle toadd;
		toadd.vline = style.vline;
        for(EIterator i = chart->Entities.begin(); i!=chart->Entities.end(); i++) 
			(*i)->status.ApplyStyleRange(Range(yPos, yPos+height), toadd);
	}
    return height;
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

void ArcParallel::PostParseProcess(EIterator &left, EIterator &right, int &number, bool top_level)
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

double ArcParallel::DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker)
{
    if (!valid) return 0;
    if (final) yPos = y;
    if (draw) y = yPos;
    //we keep this->geometry empty, as it is merely the sum of its content

    double height = 0;
    if (draw) { //quicker without the geometry manipulations
        for (PtrList<ArcList>::iterator i=blocks.begin(); i != blocks.end(); i++) {
            double h = chart->DrawHeightArcList((*i)->begin(), (*i)->end(),
                                                y, g, true, final, height?y+height:autoMarker);
            if (height < h) height = h;
        }
    } else {
        Block limiter(0, chart->totalWidth, y, y);
        g += limiter; // prevent any compression for now in DrawHeightArcList
        Geometry g_original = g;
        for (PtrList<ArcList>::iterator i=blocks.begin(); i != blocks.end(); i++) {
            //Each parallel block is compressed without regard to the others
            Geometry geom = g_original;
            double h = chart->DrawHeightArcList((*i)->begin(), (*i)->end(),
                                                y, geom, false, final, height?y+height:autoMarker);
            if (height < h) height = h;
            g += geom;
        };
        g -= limiter; //remove blockage
    }
    return height;
}

void ArcParallel::PostHeightProcess(void)
{
    ArcBase::PostHeightProcess();
    if (!valid) return;
    for (PtrList<ArcList>::iterator i=blocks.begin(); i!=blocks.end(); i++)
        for (ArcList::iterator j = (*i)->begin(); j != (*i)->end(); j++)
            (*j)->PostHeightProcess();
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

string CommandEntity::Print(int ident) const
{
    string ss;
    ss << string(ident*2, ' ');
    ss << "Entity Command";
    if (full_heading) ss<<"(full_heading)";
    if (entities)
        for (EntityDefList::iterator i = entities->begin();i != entities->end(); i++)
            ss << "\n" << (*i)->Print(ident+1);
    return ss;
}

void CommandEntity::AppendToEntities(const EntityDefList &e)
{
    if (!entities) entities = new EntityDefList;
    entities->insert(entities->end(),e.begin(),e.end());
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
    if (ce->entities) {
        AppendToEntities(*(ce->entities));
        ce->entities->clear();
    }
}


void CommandEntity::PostParseProcess(EIterator &left, EIterator &right, int &number,
                                     bool top_level)
{
    at_top_level = top_level;
    if (full_heading && !top_level)
        chart->Error.Warning(file_pos.start, "The command 'heading' is specified "
                             "inside a parallel block. May display incorrectly.");
    if (!entities)
        return; //a pure "heading" command does not change left or right
    for (EntityDefList::iterator i=entities->begin(); i!=entities->end(); i++) {
        EIterator j = chart->Entities.Find_by_Name((*i)->name);
        if (left != chart->NoEntity)
            if ((*j)->pos < (*left)->pos)
                left = j;
        if (right != chart->NoEntity)
            if ((*j)->pos > (*right)->pos)
                right = j;
		//Make the style of the entitydef fully specified using the accumulated style info in Entity
		(*j)->running_style += (*i)->style;
		(*i)->style = (*j)->running_style;
		double w = (*j)->Width((*j)->running_style);
		if ((*j)->maxwidth < w) (*j)->maxwidth = w;
    }
}


void CommandEntity::Width(EntityDistanceMap &distances)
{
    if (!valid) return;
    //Add distances for entity heading
    EntityDistanceMap d;
    if (entities)   //An entity command
        for (EntityDefList::iterator i = entities->begin(); i!=entities->end(); i++) {
            EIterator j = chart->Entities.Find_by_Name((*i)->name);
            //Take entity height into account or draw it if show=on was added
            if ((*i)->show.second && (*i)->show.first) {
				const double halfsize = (*j)->maxwidth/2;
                const unsigned index = (*j)->index;
                d.Insert(index, DISTANCE_LEFT, halfsize);
                d.Insert(index, DISTANCE_RIGHT, halfsize);
            }
        }
    if (full_heading)  //A "heading" command, all entities that are on
        for (EIterator i = chart->Entities.begin(); i!=chart->Entities.end(); i++) {
            if ((*i)->name == NONE_ENT_STR) continue;
            if ((*i)->name == LSIDE_ENT_STR) continue;
            if ((*i)->name == RSIDE_ENT_STR) continue;
            //Here we do not know if all entities will be on at the same time.
            //So we assume worst case (yes) and add distances for all
            const double halfsize = (*i)->maxwidth/2;
            const unsigned index = (*i)->index;
            d.Insert(index, DISTANCE_LEFT, halfsize);
            d.Insert(index, DISTANCE_RIGHT, halfsize);

        }
    //convert left and right requirements (added above) to pairwise ones.
    d.CombineLeftRightToPair_Sum(chart->hscaleAutoXGap);
    distances += d;
}

double CommandEntity::DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker)
{
    if (!valid) return 0;
    if (final) yPos = y;
    if (draw)  y = yPos;
    if (!draw) geometry.Clear();

    double height = 0;
    //Those entities explicitly listed, put their cover in their geometry
    //only the ones added by a heading command to this commandentity will
    //put anything into this->geometry
    list<EIterator> explicitly_listed_entities;
    if (entities) {  //An entity command
        for (EntityDefList::iterator i = entities->begin(); i!=entities->end(); i++) {
            //Clear geometry that may have been accumulated by !final rounds
            if (!draw)
                (*i)->geometry.Clear();
            EIterator j = chart->Entities.Find_by_Name((*i)->name);
            assert(j != chart->NoEntity);

			//Take entity height into account if it gets drawn
            //It can get drawn because we 1) said show=yes, or
            //2) because it is on, we mention it (without show=yes) and it is
            //a full heading.
            if (((*i)->show.second && (*i)->show.first) ||
				(full_heading && (*j)->status.GetStatus(y))) {
                const double h = (*j)->DrawHeight(y, (*i)->geometry, draw, final);
                //If we are not drawing, we add the cover to the entitydef's geometry
                if (!draw) {
                    (*i)->geometry.SetArc(*i);
                    g += (*i)->geometry; //we add it to the one returned
                }
                //We add this entity to the list of entities already done
                explicitly_listed_entities.push_back(j);
                if (height <h) height = h;
            }
        }
    }

    if (!full_heading) return height;
    //A "heading" command, draw all entities that are on
    for (EntityList::const_iterator i = chart->Entities.begin(); i!=chart->Entities.end(); i++) {
		if (!(*i)->status.GetHideStatus(y) || !(*i)->status.GetStatus(y)) continue;
        bool was = false;
        for (list<EIterator>::const_iterator k = explicitly_listed_entities.begin(); k!=explicitly_listed_entities.end(); k++) {
            if (*k != i) continue;
            was = true;
            break;
        }
        if (was) continue;
        const double h = (*i)->DrawHeight(y, geometry, draw, final);
        if (height <h) height = h;
    }
    if (!draw) {
        geometry.SetArc(this);
        g += geometry;
    }
    return height;
}

void CommandEntity::PostHeightProcess(void)
{
	//Add us to the big file positions list
	if (entities)
        for (EntityDefList::const_iterator i = entities->begin(); i!=entities->end(); i++)
            chart->AllArcs[(*i)->file_pos] = *i;
    chart->AllArcs[file_pos] = this;
    if (!valid) return;
	//Add our covers to the big cover list
    chart->AllCovers.insert(chart->AllCovers.end(), geometry.GetCover().begin(), geometry.GetCover().end());
    if (entities)   //An entity command
        for (EntityDefList::const_iterator i = entities->begin(); i!=entities->end(); i++)
            chart->AllCovers.insert(chart->AllCovers.end(), (*i)->geometry.GetCover().begin(), (*i)->geometry.GetCover().end());

	//Record style changes
    if (!entities) return;
    for (EntityDefList::iterator i = entities->begin(); i!=entities->end(); i++) {
        EIterator j = chart->Entities.Find_by_Name((*i)->name);
        assert(j != chart->NoEntity);
		//Apply changes in style
		(*j)->status.ApplyStyle(yPos, (*i)->style);
		//Apply changes in show status
        if ((*i)->show.first)
			(*j)->status.SetStatus(yPos, (*i)->show.second);
    }
}


//////////////////////////////////////////////////////////////////////////////////////

double CommandNewpage::DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker)
{
    if (!final || !valid) return 0;
    yPos = y;
    chart->yPageStart.push_back(y);
    Block b(0, chart->totalWidth, y, y, this);
    b.findType = Block::FIND_NONE;
    geometry += b;
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////

double CommandNewBackground::DrawHeight(double y, Geometry &g,
                                        bool draw, bool final, double autoMarker)
{
    if (!final || !valid) return 0;
    yPos = y;
    chart->Background[y] = fill;
    Block b(0, chart->totalWidth, y, y, this);
    b.findType = Block::FIND_NONE;
    geometry += b;
    return 0;
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

double CommandMark::DrawHeight(double y, Geometry &g,
                               bool draw, bool final, double autoMarker)
{
    if (draw || !valid) return 0;
    chart->Markers[name].second = y+offset;
    if (!final) return 0;
    Block b(0, chart->totalWidth, y+offset, y+offset, this);
    b.findType = Block::FIND_NONE;
    geometry += b;
    return 0;
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

double CommandEmpty::DrawHeight(double y, Geometry &g, bool draw, bool final, double autoMarker)
{
    if (!valid) return 0;
    const double width  = parsed_label.getTextWidthHeight().x;
    const double height = parsed_label.getTextWidthHeight().y;
    if (draw) {
        MscLineAttr line;
        line.width.second = 3;
        line.radius.second = 10;

        MscFillAttr fill;
        fill.color.second = MscColorType(0,0,128);
        fill.gradient.second = GRADIENT_BUTTON;

        MscShadowAttr shadow;
        shadow.offset.second = 5;
        shadow.blur.second = 5;

        chart->shadow(XY((chart->totalWidth-width)/2 , y+EMPTY_MARGIN_Y),
                      XY((chart->totalWidth+width)/2 , y+EMPTY_MARGIN_Y+height),
                      shadow, line.radius.second);
        chart->filledRectangle(XY((chart->totalWidth-width)/2 , y+EMPTY_MARGIN_Y),
                               XY((chart->totalWidth+width)/2 , y+EMPTY_MARGIN_Y+height),
                               fill, line.radius.second);
        chart->rectangle(XY((chart->totalWidth-width)/2 , y+EMPTY_MARGIN_Y),
                         XY((chart->totalWidth+width)/2 , y+EMPTY_MARGIN_Y+height), line);
    }
    parsed_label.DrawCovers((chart->totalWidth-width)/2,
                            (chart->totalWidth+width)/2,
                            y+EMPTY_MARGIN_Y, g, draw);
    return height + EMPTY_MARGIN_Y*2;
}
