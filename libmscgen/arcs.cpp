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

/*
    Here is how the lifecycle of an Arc goes.
    1. Construction: Only basic initialization is done here. For arcs with style (descendants of ArcLabelled)
       we fetch the appropriate style from Msc::Contexts.back()
    2. AddAttributeList: This is called to add the attributes set by the user. It is called even if
       there are no attributes (with NULL), which allows to do processing after the addition of attributes.
       This is the last place where Msc::Contexts.back() holds the parsing context (with current 
       style, color, numbering, compress settings).
    3. Additional small functions (SetLineEnd, SetPipe (before AddAttributeList), etc.)
    4. PostParseProcess: This is called recursive to determine the non-specified entities for boxes, to
       add numering to labels, combine CommandEntities one after the other, set up extra variables and
       to print error messages. This function can only be called once, as it changes arcs (you do not want to
       add numbering twice).
    5. Width: This is also called recursively. Here each arc can place the distance requirements between
       entities. Using the data Msc::CalculateWithAndHeight() can place entities dynamically if hscale==auto.
       If hcale!=auto, entities have fixed positions, but this function is still called (so as it can be used
       to calculate cached values).
    6. Height: This is a key function, returning the vertical space an element(/arc) occupies. It also places
       the contour of the element in its "cover" parameter. The former (height) is used when compress is off and 
       the latter (contour) if compress is on. In the latter case the entity will be placed just below entities
       abover such that their contours just touch but do not overlap. Height also fills in 
       TrackableElement::area and area_draw with contours that will be used to detect if a mouse pointer is 
       inside the arc or not and to draw a tracking contour over the arc. Observe
       - contour returned in cover is used for placement and should contain shadows
       - area is used to detect if the mouse is within, should not contain shadows
       - area_draw is used to draw, it should be a frame for boxes and pipes with content, not the contour of the box.
       Height can also store some pre-computed values and contours to make drawing faster.
       Height always places the element at the vertical position=0. Any contour should assume that.
    7. ShiftBy: During the placement process this is called to shift an entity in the vertical direction 
       (usually downwards). This should update area, area_draw and any other cached variable.
       ArcBase::yPos contains the sum of these shifts. This function can be called multiple times.
    8. PostPosProcess: Called after the last call to ShiftBy. Here entity lines are hidden behind text and
       warnings/errors are generated which require vertical position to decide. No error messages can be printed
       after this function.
    9. Draw: This function actually draws the chart to the Msc pointed by its "chart" member (initialized in the
       constructor). This function can rely cached values in the elements. It can be called several times and
       should not change state of the element including the cached values.
   10. Destructor.

       All the above functions are called from the Msc object. #1-#3 are called from Msc::ParseText, whereas
       the remainder from the Msc:: memeber functions of similar names, with the exception of ShiftBy, which is
       called from Msc::Height and Msc::PlaceListUnder.

    Color Syntax Highlighting support also has functions in Arcs. 
    1. AttributeNames: A static function that inserts the attributes valid for this type of arc into a Csh object.
    2. AttributeValues: A static function that inserts the possible values for a given attribute into a Csh object.
    Both of the above calls include a callback function to draw the small pictograms for hints. These functions
    are in fact only used for the hinting process and not for actual syntax highlighting (which requires no
    support from Arc objects, since those are not created during a Csh parse.
*/
#include <math.h>
#include <cassert>
#include <iostream>
#include <algorithm>
#include "msc.h"

using namespace std;

TrackableElement::TrackableElement(const TrackableElement&o) :
    linenum_final(o.linenum_final), area(o.area), yPos(o.yPos),
    area_draw(o.area_draw), draw_is_different(o.draw_is_different)
{
    area.arc = this;
    area_draw.arc = this;
}

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
    type(t), chart(msc), valid(true), compress(false), parallel(false), area_draw_is_frame(false)
{
    if (msc) compress = msc->Contexts.back().compress;
    had_add_attr_list = false;
}

//l can be an empty list
ArcBase* ArcBase::AddAttributeList(AttributeList *l)
{
    had_add_attr_list = true;
    if (l==NULL || !valid) return this;
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
    _ASSERT(had_add_attr_list);
    if (valid) {
        if (!area.IsEmpty()) {
            //TODO: Pipe segments suck here, so if expand cannot do it,
            //we still keep the original stuff.
            Area expanded_area = area.CreateExpand(chart->trackExpandBy);
            if (!expanded_area.IsEmpty())
                area = expanded_area;
            area.arc = this;
            chart->AllCovers += area;
        }
        if (draw_is_different && !area_draw.IsEmpty() && !area_draw_is_frame)
            area_draw = area_draw.CreateExpand(chart->trackExpandBy);
    }
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
    if (!valid) return this;
    //Find label attribute
    file_line linenum_label;
    if (l)
        for (AttributeList::iterator i = l->begin(); i!=l->end(); i++)
            if ((*i)->Is("label")) linenum_label = (*i)->linenum_value.start;
    //Add attributest first
    ArcBase::AddAttributeList(l);
    //Make style.text complete using the default text formatting in the context as default
    StringFormat to_use(chart->Contexts.back().text);
    to_use += style.text;
    style.text = to_use;
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
                style.numbering.second = true;
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
    if (!valid) return;
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
    MscStyle style(STYLE_DEFAULT, ArrowHead::ARROW, true, true, false, false, false, false, true, true, false); //no fill, shadow, vline solid side
    style.AttributeNames(csh);
}

bool ArcArrow::AttributeValues(const std::string attr, Csh &csh)
{
    MscStyle style(STYLE_DEFAULT, ArrowHead::ARROW, true, true, false, false, false, false, true, true, false); //no fill, shadow, vline solid side
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
    if (!valid) return;
    //Add numbering, if needed
    ArcLabelled::PostParseProcess(left, right, number, top_level);

    left = chart->EntityMinByPos(left, src);
    right = chart->EntityMaxByPos(right, src);
}

void ArcSelfArrow::Width(EntityDistanceMap &distances)
{
    if (!valid) return;
    distances.Insert((*src)->index, DISTANCE_RIGHT, chart->XCoord(0.375));
    distances.Insert((*src)->index, DISTANCE_LEFT, parsed_label.getTextWidthHeight().x);
}

double ArcSelfArrow::Height(AreaList &cover)
{
    if (!valid) return 0;
    yPos = 0;
    xy_s = style.arrow.getWidthHeight(isBidir(), MSC_ARROW_START);
    xy_e = style.arrow.getWidthHeight(isBidir(), MSC_ARROW_END);
    xy_s.y = ceil(xy_s.y);
    xy_e.y = ceil(xy_e.y);
    wh.x = ceil(chart->XCoord(0.375));
    wh.y = ceil(2*YSize);
    dx = chart->XCoord(src);
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
    if (!valid) return;
    ArcArrow::PostPosProcess(autoMarker);

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
    if (!valid) return;
    double y = yPos + chart->arcVGapAbove;

    parsed_label.Draw(sx, dx, y);
    y += xy_s.y;

    if (style.line.radius.second < 0) {
        //draw an arc
        chart->Line(Edge(XY(dx, y+YSize), wh.x, wh.y/2, 0, 270, 90), style.line);
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
    if (!valid) return this;
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
    if (!valid) return;
    //Add numbering, if needed
    ArcLabelled::PostParseProcess(left, right, number, top_level);

    const EIterator _left = ++chart->Entities.begin(); // leftmost entity;
    const EIterator _right = --chart->Entities.end();  // rightmost entiry
    const EIterator our_left =  chart->EntityMinByPos(src, dst);
    const EIterator our_right = chart->EntityMaxByPos(src, dst);

    //Change left and right only if they actually point to a "real entity"
    //and not (left) or (right). If they do, consider our other "end"
    if (our_left != _left)
        left = chart->EntityMinByPos(left, our_left);
    else
        left = chart->EntityMinByPos(left, our_right);

    if (our_right != _right)
        right = chart->EntityMaxByPos(right, our_right);
    else
        right = chart->EntityMaxByPos(right, our_left);

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
    //we lie about us being forward (we do not check), so we know which of first/second to use
    DoublePair end = style.arrow.getWidths(true, isBidir(), MSC_ARROW_END, false, style.line);
    DoublePair start = style.arrow.getWidths(true, isBidir(), MSC_ARROW_START, false, style.line);
    distances.Insert((*src)->index, (*dst)->index,
                     end.first + start.second + parsed_label.getTextWidthHeight().x);
    //Add distances for arrowheads
    const bool fw = (*src)->index  <  (*dst)->index;
    distances.Insert((*src)->index, fw ? DISTANCE_RIGHT : DISTANCE_LEFT, start.second);
    distances.Insert((*dst)->index, fw ? DISTANCE_LEFT : DISTANCE_RIGHT, end.first);

    if (middle.size()==0) return;
    EntityDistanceMap d;
    for (int i=0; i<middle.size(); i++) {
        DoublePair mid = style.arrow.getWidths(fw, isBidir(), MSC_ARROW_MIDDLE, false, style.line);
        distances.Insert((*middle[i])->index, DISTANCE_LEFT, mid.first);
        distances.Insert((*middle[i])->index, DISTANCE_RIGHT, mid.second);
    }
    d.CombineLeftRightToPair_Sum(chart->hscaleAutoXGap);
    distances += d;
}

MscArrowEnd ArcDirArrow::WhichArrow(int i)
{
    //in xPos (may not be filled up yet, index 0 will be the src and indes xpos.size()-1 will be dest
    //in between there will be middle.size() number of middle arrows.
    //"i" here is an index that will be used for xPos.
    if (i>0 && i<middle.size()+1) return MSC_ARROW_MIDDLE;
    if ((i==0) == (*src)->index  <  (*dst)->index) return MSC_ARROW_START;
    return MSC_ARROW_END;
}

double ArcDirArrow::Height(AreaList &cover)
{
    if (!valid) return 0;
    yPos = 0;
    area.clear();
    sx = chart->XCoord(src);
    dx = chart->XCoord(dst);

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
            sx_text = dx + xy_e.x;
            dx_text = sx - xy_s.x;
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
    margins.push_back(style.arrow.getWidths(sx<dx, isBidir(), MSC_ARROW_START, true, style.line));
    for (int i=0; i<middle.size(); i++) {
        xPos.push_back(chart->XCoord(middle[i]));
        margins.push_back(style.arrow.getWidths(sx<dx, isBidir(), MSC_ARROW_MIDDLE, true, style.line));
    }
    xPos.push_back(dx);
    margins.push_back(style.arrow.getWidths(sx<dx, isBidir(), MSC_ARROW_END, true, style.line));
    if (sx>=dx) {
        std::reverse(xPos.begin(), xPos.end());
        std::reverse(margins.begin(), margins.end());
    }

    //Add arrowheads and line segments to cover
    for (int i=0; i<xPos.size(); i++)
        area += style.arrow.Cover(XY(xPos[i], y), sx<dx, isBidir(), WhichArrow(i), style.line);
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
    if (!valid) return;
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
    if (!valid) return;
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
    //calculate clip 
    const double y = yPos+centerline;  //should be integer
    clip_area  = style.arrow.ClipForLine(XY(sx, y), sx<dx, isBidir(), MSC_ARROW_START, chart);
    clip_area *= style.arrow.ClipForLine(XY(dx, y), sx<dx, isBidir(), MSC_ARROW_END, chart);
    //for multi-segment arrows
    for (unsigned i=0; i<middle.size(); i++) {
        const double mx = chart->XCoord(middle[i]);
        Range hideEntityLine = style.arrow.EntityLineCover(XY(mx, yPos+centerline), sx<dx, isBidir(), MSC_ARROW_MIDDLE);
        (*middle[i])->status.HideRange(hideEntityLine);
        clip_area *= style.arrow.ClipForLine(XY(mx, y), sx<dx, isBidir(), MSC_ARROW_MIDDLE, chart);
    }
}

void ArcDirArrow::Draw()
{
    if (!valid) return;
    if (parsed_label.getTextWidthHeight().y)
        parsed_label.Draw(sx_text, dx_text, yPos + chart->arcVGapAbove);
    /* Draw the line */
    //all the entities this (potentially multi-segment arrow visits)
    const double y = yPos+centerline;  //should be integer
    chart->Clip(clip_area);
    for (unsigned i=0; i<xPos.size()-1; i++)
        chart->Line(XY(xPos[i], y), XY(xPos[i+1], y), style.line);
        //chart->Line(XY(xPos[i]+margins[i].second, y), XY(xPos[i+1]-margins[i+1].first, y), style.line);
    chart->UnClip();
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
    MscStyle style(STYLE_DEFAULT, ArrowHead::BIGARROW, true, true, true, true, false, false, true, true, false);  //no vline solid side
    style.AttributeNames(csh);
}

bool ArcBigArrow::AttributeValues(const std::string attr, Csh &csh)
{
    MscStyle style(STYLE_DEFAULT, ArrowHead::BIGARROW, true, true, true, false, false, false, true, true, false);  //no shadow, vline solid side
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
    if (!valid) return;
    //Determine src and dst entity, check validity of multi-segment ones, add numbering, etc
    ArcDirArrow::PostParseProcess(left, right, number, top_level);
    //Finally copy the line attribute to the arrow, as well
    style.arrow.line = style.line;
}

void ArcBigArrow::Width(EntityDistanceMap &distances)
{
    if (!valid) return;
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
    if (!fw) {
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
    if (!valid) return 0;
    yPos = 0;

    //Reuse sy and dy set in Width()
    centerline = (sy+dy)/2;

    //set sx and dx
    sx = chart->XCoord(src);
    dx = chart->XCoord(dst);
    //prepare xPos (margins were already done in Width)
    xPos.clear(); xPos.reserve(2+middle.size());
    xPos.push_back(sx);
    for (int i=0; i<middle.size(); i++)
        xPos.push_back(chart->XCoord(middle[i]));
    xPos.push_back(dx);
    //calculate text margings
    //note that stext and dtext note indexes with indexes ordered from 
    //left to right, not in the original order of them, so we adjust here
    if (sx < dx) {
        sx_text = xPos[stext] + sm;
        dx_text = xPos[dtext] - dm;
    } else {
        sx_text = xPos[xPos.size()-1-stext] + sm;
        dx_text = xPos[xPos.size()-1-dtext] - dm;
    }

    //use += to keep arc and other params of area
    area = style.arrow.BigCover(xPos, sy, dy, isBidir());
    area.arc = this;
    //set mainline - not much dependent on main line with
    area.mainline = Range(centerline - chart->nudgeSize/2, centerline + chart->nudgeSize/2);
    cover = area;
    return centerline*2 - chart->arcVGapAbove + chart->arcVGapBelow;
}

void ArcBigArrow::ShiftBy(double y)
{
    if (!valid) return;
    sy += y;
    dy += y;
    ArcArrow::ShiftBy(y); //Skip ArcDirArrow
}

void ArcBigArrow::PostPosProcess(double autoMarker)
{
    if (!valid) return;
    ArcArrow::PostPosProcess(autoMarker); //Skip ArcDirArrow
    CheckSegmentOrder(yPos + centerline);
}

void ArcBigArrow::Draw()
{
    if (!valid) return;
    chart->Shadow(style.arrow.BigCover(xPos, sy, dy, isBidir()), style.shadow);
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

ArcVerticalArrow::ArcVerticalArrow(MscArcType t, const char *s, const char *d, Msc *msc) :
    ArcArrow(t, msc, msc->Contexts.back().styles["vertical"]), ypos(2), pos(*msc)
{
    if (s) src = s;
    if (d) dst = d;
    makeroom=false;
    offset = 0;
    valid = false; //without an x pos we are invalid
}

ArcVerticalArrow*  ArcVerticalArrow::AddXpos(VertXPos *p)
{
    if (!p || !p->valid) return this;
    valid = true;
    pos = *p;
    switch (pos.pos) {
    case VertXPos::POS_LEFT_BY:
    case VertXPos::POS_LEFT_SIDE:
        style.side.second = SIDE_RIGHT; break;
    default:
        style.side.second = SIDE_LEFT; break;
    }
    //overwrite the style set by ArcArrow::ArcArrow
    style = chart->Contexts.back().styles["vertical"];
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
    return this;
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
    return ArcArrow::AddAttribute(a);
}

void ArcVerticalArrow::AttributeNames(Csh &csh)
{
    ArcLabelled::AttributeNames(csh);
    MscStyle style(STYLE_DEFAULT, ArrowHead::BIGARROW, true, true, true, false, false, false, true, true, true);  //no shadow, vline solid
    style.AttributeNames(csh);
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"pos", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"makeroom", HINT_ATTR_NAME));
}

bool ArcVerticalArrow::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr,"pos")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable()+"<number>", HINT_ATTR_VALUE, false));
        return true;
    }
    if (CaseInsensitiveEqual(attr,"makeroom")) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+"yes", HINT_ATTR_VALUE, true, CshHintGraphicCallbackForYesNo, CshHintGraphicParam(1)));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+"no", HINT_ATTR_VALUE, true, CshHintGraphicCallbackForYesNo, CshHintGraphicParam(0)));
        return true;
    }
    MscStyle style(STYLE_DEFAULT, ArrowHead::BIGARROW, true, true, true, false, false, false, true, true, true);  //no shadow, vline solid
    if (style.AttributeValues(attr, csh)) return true;
    if (ArcLabelled::AttributeValues(attr, csh)) return true;
    return false;
}


void ArcVerticalArrow::PostParseProcess(EIterator &left, EIterator &right,
                                        Numbering &number, bool top_level)
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

    left = chart->EntityMinByPos(left, pos.entity1);
    right = chart->EntityMaxByPos(right, pos.entity1);
    left = chart->EntityMinByPos(left, pos.entity2);
    right = chart->EntityMaxByPos(right, pos.entity2);

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
        if (style.side.second == SIDE_LEFT)
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
    yPos += y;
}

void ArcVerticalArrow::PostPosProcess(double autoMarker)
{
    if (!valid) return;
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

    if (ypos[0] == ypos[1]) {
        chart->Error.Error(file_pos.start, "This vertical is of zero size. Ignoring it.");
        valid = false;
        return;
    }

    if (style.side.second == SIDE_RIGHT)
        swap(ypos[0], ypos[1]);

    const double lw = style.line.LineWidth();
    const XY twh = parsed_label.getTextWidthHeight();
    Area text_cover = parsed_label.Cover(0, twh.x, lw/2);

    const double sm = style.arrow.getBigMargin(text_cover, lw/2, twh.y+lw, true, style.side.second == SIDE_LEFT, isBidir(), MSC_ARROW_START);
    const double dm = style.arrow.getBigMargin(text_cover, lw/2, twh.y+lw, false, style.side.second == SIDE_LEFT, isBidir(), MSC_ARROW_END);
    const double ss = style.arrow.getBigWidthHeight(isBidir(), MSC_ARROW_START).x;
    const double ds = style.arrow.getBigWidthHeight(isBidir(), MSC_ARROW_END).x;

    if (sm + twh.x + dm  > fabs(ypos[0]-ypos[1]))
        chart->Error.Warning(file_pos.start, "Size of vertical element is smaller than needed for text.",
                                 "May look strange.");
    if (ss+ds > fabs(ypos[0]-ypos[1])) {
        chart->Error.Warning(file_pos.start, "Size of vertical element is too small to draw arrowheads. Ignoring it.");
        valid = false;
    }

    //calculate xpos and width
    width = twh.y;
    if (width==0)
        width = Label("M", chart, style.text).getTextWidthHeight().y;
    width = ceil(width + 2*lw + 2*chart->emphVGapInside);
    width += fmod(width, 2); //width is even integer now: the distance from outer edge to outer edge

    const double aw = style.arrow.bigYExtent(isBidir(), false)/2;
    xpos = chart->XCoord(pos.entity1);
    const double gap = chart->hscaleAutoXGap;
    switch (pos.pos) {
    default:
    case VertXPos::POS_AT: break;
    case VertXPos::POS_CENTER:      xpos = (xpos + chart->XCoord(pos.entity2))/2; break;
    case VertXPos::POS_LEFT_BY:     xpos -= width/2 + aw + gap; break;
    case VertXPos::POS_RIGHT_BY:    xpos += width/2 + aw + gap; break;

    case VertXPos::POS_LEFT_SIDE:   xpos -= width/2 + gap; break;
    case VertXPos::POS_RIGHT_SIDE:  xpos += width/2 + gap; break;
    };
    xpos = floor(xpos + offset + 0.5); //xpos is integer now: the centerline of arrow
    width -= lw; //not necessarily integer, the distance from midline to midline

    if (style.side.second == SIDE_LEFT) {
        sy_text = std::min(ypos[0], ypos[1])+sm;
        dy_text = std::max(ypos[0], ypos[1])-dm;
    } else {
        sy_text = std::max(ypos[0], ypos[1])-sm;
        dy_text = std::min(ypos[0], ypos[1])+dm;
    }

	//Generate area
    auto ypos_tmp = ypos;
    std::swap(ypos_tmp[0], ypos_tmp[1]);
    area = style.arrow.BigCover(ypos_tmp, xpos-width/2, xpos+width/2, isBidir());
    area.SwapXY();
    //Expand area and add us to chart's all covers list
    ArcArrow::PostPosProcess(autoMarker);
}


void ArcVerticalArrow::Draw()
{
    if (!valid) return;
    if (style.side.second == SIDE_LEFT)
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
    content(NULL), follow(true), first(NULL), pipe(false),
    drawEntityLines(true), drawing_variant(1)
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
    if (!valid) return this;
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

ArcEmphasis::~ArcEmphasis() 
{
    //"this" is also inserted into follow
    //pipes reorder follow so it can be any element
    for (auto i=follow.begin(); i!=follow.end(); i++)
        if (*i == this) {
            follow.erase(i);
            break;
        }
    delete content;
}


ArcEmphasis* ArcEmphasis::AddArcList(ArcList*l)
{
    if (!valid) return this;
    if (l!=NULL && l->size()>0) {
        if (content) {
            content->insert(content->end(), l->begin(), l->end());
            l->clear(); //so that l's constructor does not delete Arcs in arclist
            delete l;
        } else {
            content=l;
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
    MscStyle style(STYLE_DEFAULT, ArrowHead::NONE, true, true, true, true, false, pipe, true, true, false); //no arrow, vline solid side
    style.AttributeNames(csh);
}

bool ArcEmphasis::AttributeValues(const std::string attr, Csh &csh, bool pipe)
{
    if (CaseInsensitiveEqual(attr,"color")) {
        csh.AddColorValuesToHints();
        return true;
    }
    MscStyle style(STYLE_DEFAULT, ArrowHead::NONE, true, true, true, true, false, pipe, true, true, false); //no arrow, vline solid side
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
    _ASSERT(f);
    if (f==NULL) return this;
    if (!f->valid)
        valid = false;
    if (valid && pipe) {
        if (f->style.side.first) {
            f->style.side.first = false;
            chart->Error.Error(f->file_pos.start,
                               "Attribute 'side' can only be specified in the first "
                               "element in a pipe series. Ignoring it in subsequent ones.");
        }
        if (f->style.line.radius.first) {
            f->style.line.radius.first = false;
            chart->Error.Error(f->file_pos.start,
                               "Attribute 'line.radius' can only be specified in the first "
                               "element in a pipe series. Ignoring it in subsequent ones.");
        }
    }
    //we do this even if we are valid, so that f will be properly destroyed
    f->first = this;
    MscStyle s = style;
    s += f->style;
    f->style = s;
    follow.Append(f);
    return this;
}

string ArcEmphasis::Print(int ident) const
{
    string ss;
    ss << string(ident*2, ' ');
    ss << (*src)->name << "--" << (*dst)->name;
    if (label.length()>0) ss << ": \"" << label << "\"";
    if (content)
        for (ArcList::iterator i = content->begin(); i != content->end(); i++)
            ss << "\n" << (*i)->Print(ident+1);
    return ss;
}

struct pipe_compare
{
    const Msc *chart;
    bool fromright;
    pipe_compare(const Msc *c, bool fr) : chart(c), fromright(fr) {}
    bool operator ()(const ArcEmphasis *p1, const ArcEmphasis *p2) const {
        EIterator min1 = chart->EntityMinMaxByPos(p1->src, p1->dst, fromright);
        EIterator min2 = chart->EntityMinMaxByPos(p2->src, p2->dst, fromright);
        if (min1==min2) return false; //equals are not less
        return min1 == chart->EntityMinMaxByPos(min1, min2, fromright);
    }
};

//will only be called for the first box of a multi-segment box series
void ArcEmphasis::PostParseProcess(EIterator &left, EIterator &right,
                                   Numbering &number, bool top_level)
{
    if (!valid) return;
    //Add ourselves as the first element in follow.
    follow.push_front(this);

    EIterator e1=src;      //src.pos < dst.pos guaranteed in constructor (if none is NULL)
    EIterator e2=dst;
    //OK, now expand further if there are following boxes
    for (PtrList<ArcEmphasis>::iterator i = follow.begin(); i!=follow.end(); i++) {
        //Add numbering, if needed
        (*i)->ArcLabelled::PostParseProcess(left, right, number, top_level);
        if ((*i)->content) {
            //If we have attached a number to the label of a box, set flag on number,
            //so that if we add levels inside the content of the box (before displaying any number)
            //we continue from present value
            if ((*i)->style.numbering.second && (*i)->label.length()!=0)
                number.decrementOnAddingLevels = true;
            chart->PostParseProcessArcList(*((*i)->content), false, e1, e2, number, top_level);
            number.decrementOnAddingLevels = false;
        }
    }

    if (pipe) {
        //Check that all pipe segments are fully specified, non-overlapping and sort them
        //After this follow.begin will not point to "this" but to the leftmost (fromright==true) or rightmost (==false)
        //Also set pipe_connect_back/forw flags
        //Terminology: backwards means left if "fromright" and right otherwise

        //frist sort from left to right
        pipe_compare comp(chart, true);
        follow.sort(comp);
        //Make both src and dst specified and ordered in all segments
        //The leftmost and the rightmost segment can auto-adjust to the content (e1 and e2)
        //others can snap left and right if not specified
        //Also collect thickest linewidth
        EIterator last = e1;
        EIterator next;
        double lw_max = 0;
        for (auto i = follow.begin(); i!=follow.end(); i++) {
            auto i_next = i;
            i_next++;
            next = i_next==follow.end() ? e2 : chart->EntityMinByPos((*i_next)->src, (*i_next)->dst);

            if ((*i)->src == chart->NoEntity) (*i)->src = last;
            if ((*i)->dst == chart->NoEntity) (*i)->dst = next;

            if ((*i)->src == chart->NoEntity || (*i)->dst == chart->NoEntity) {
                std::string msg = "Could not figure out the extent of this pipe";
                if (follow.size()>1) msg += " segment";
                msg += ". Specify starting and/or terminating entity.";
                chart->Error.Error((*i)->file_pos.start, msg);
                valid = false;
                return;
            }
            if ((*i)->src != chart->EntityMinByPos((*i)->src, (*i)->dst))
                swap((*i)->src, (*i)->dst);
            last = (*i)->dst;

            lw_max = std::max(lw_max, (*i)->style.line.LineWidth());
        }

        //increase the radius everywhere by the thickest lw (if it is not zero)
        if (style.line.radius.second>0) {
            const double radius = style.line.radius.second + lw_max;
            for (auto i = follow.begin(); i!=follow.end(); i++)
                (*i)->style.line.radius.second = radius;
        }
        //set e1 and e2 to real leftmost and rightmost entity affected
        e1 = chart->EntityMinByPos(e1, (*follow.begin())->src);
        e2 = chart->EntityMaxByPos(e2, (*follow.rbegin())->dst);

        //Sort according to fromright: begin() should point to the leftmost pipe if side==right, 
        //and to the rightmost if side=left
        if (style.side.second == SIDE_LEFT) {
            comp.fromright = false;
            follow.sort(comp);
        }

        //Fill in pipe_connect vlaues
        (*follow.begin())->pipe_connect_back = false;
        (*follow.rbegin())->pipe_connect_forw = false;
        for (auto i = ++follow.begin(); i!=follow.end(); i++) {
            (*i)->pipe_connect_back = (*i)->pipe_connect_forw = false;
            //Set flags if we are adjacent to previous one
            auto i_prev = i;
            i_prev--;
            if ((style.side.second == SIDE_RIGHT && (*i_prev)->dst == (*i)->src) ||
                (style.side.second == SIDE_LEFT  && (*i_prev)->src == (*i)->dst)) {
                (*i)->pipe_connect_back = true;
                (*i_prev)->pipe_connect_forw = true;
            } else {
                if ((style.side.second == SIDE_RIGHT && chart->EntityMaxByPos((*i_prev)->dst, (*i)->src) != (*i)->src) ||
                    (style.side.second == SIDE_LEFT  && chart->EntityMinByPos((*i_prev)->src, (*i)->dst) != (*i)->dst)) 
                    chart->Error.Warning((*i)->file_pos.start, "This pipe segment overlaps a negighbouring one."
                                         " It may not look so good.",
                                         "Encapsulate one in the other if you want that effect.");
                (*i)->pipe_connect_back = false;
                (*i_prev)->pipe_connect_forw = false;
            }
        }
        //Now check that segments connecting to both sides are not only spanning a single entity
        if (follow.size()>1)
            for (auto i = follow.begin(); i!=follow.end(); i++) {
                if ((*i)->src == (*i)->dst) {
                    chart->Error.Error((*i)->file_pos.start, "This pipe segment is sqeezed between two connecting neighbouring segments and will not show."
                        " Ignoring pipe.");
                    valid = false;
                    return;
            }
        }

        //set return value
        left = chart->EntityMinByPos(e1, left);
        right = chart->EntityMaxByPos(e2, right);

        //if there is no content, no need to draw a transparent cover
        //save drawing cost (and potential fallback img)
        //only first pipe can have content (which becomes the content of all pipe)
        if (!content || content->size() == 0)
            for (PtrList<ArcEmphasis>::iterator i = follow.begin(); i!=follow.end(); i++)
                (*i)->style.solid.second = 255;
    } else {
        if (src==chart->NoEntity) src = e1;
        if (dst==chart->NoEntity) dst = e2;

        //Src and dst can still be == NoEntity, if no arcs specified
        //inside the content and no enity specified at emph declaration.
        //In this case emph box spans to leftmost and rightmost entity in chart.
        //At PostParse "Entities" is already sorted by pos values
        //we only do this step if we are the first in an Emphasis box series.
        if (src==chart->NoEntity) src = ++ ++chart->Entities.begin();     //leftmost entity after Noentity and (left)
        if (dst==chart->NoEntity) dst = -- --chart->Entities.end();    //rightmost entity (before (right)

        left = chart->EntityMinByPos(chart->EntityMinByPos(left, src), dst);
        right = chart->EntityMaxByPos(chart->EntityMaxByPos(right, src), dst);
    }
}

//will only be called for the first box of a multi-segment box series
void ArcEmphasis::Width(EntityDistanceMap &distances)
{
    if (!valid) return;
    if (pipe) {
        EntityDistanceMap d, d_pipe;
        if (content)
            chart->WidthArcList(*content, d);

        //(*i)->src and dst contain the left and right end of a pipe
        //The order of the pipe segments in follow depends on style.side
        for (auto i = follow.begin(); i!=follow.end(); i++) {
            const double ilw = (*i)->style.line.LineWidth();
            const double width = (*i)->parsed_label.getTextWidthHeight().x + 2*chart->emphVGapInside;
            (*i)->left_space = d.Query((*(*i)->src)->index, DISTANCE_LEFT) + chart->emphVGapInside;
            (*i)->right_space = d.Query((*(*i)->dst)->index, DISTANCE_RIGHT) + chart->emphVGapInside;
            //Add extra space for curvature
            if (style.side.second == SIDE_RIGHT)
                (*i)->right_space += style.line.radius.second;
            else
                (*i)->left_space += style.line.radius.second;

            //The style.line.radius.second is understood to be the radius of the hole of the _outer edge_
            if ((*i)->src==(*i)->dst) {
                (*i)->left_space  = std::max(width/2, (*i)->left_space);
                (*i)->right_space = std::max(width/2, (*i)->right_space);
            } else {
                //keep a big enough space between src and dst for the text + curvature of pipe
                //the text can go out of the entity lines, all the way to the left-space
                d_pipe.Insert((*(*i)->src)->index, (*(*i)->dst)->index,
                                 width - (*i)->left_space - (*i)->right_space +
                                 style.line.radius.second);
            }
            (*i)->left_space  = ceil((*i)->left_space);
            (*i)->right_space = ceil((*i)->right_space);
            bool connect_left = style.side.second == SIDE_RIGHT ? (*i)->pipe_connect_back : (*i)->pipe_connect_forw;
            bool connect_right = style.side.second == SIDE_RIGHT ? (*i)->pipe_connect_forw : (*i)->pipe_connect_back;
            //Check if we are connecting to a neighbour pipe segment
            if (connect_left)
                (*i)->left_space = 0;
            else
                d_pipe.Insert((*(*i)->src)->index, DISTANCE_LEFT,
                                 (*i)->left_space + ilw + style.line.radius.second);
            //add shadow to the right size only if we are the rightmost entity
            double shadow_to_add = 0;
            if ((style.side.second == SIDE_RIGHT && i==--follow.end()) ||
                (style.side.second == SIDE_LEFT  && i==follow.begin()))
                shadow_to_add = (*i)->style.shadow.offset.second;
            if (connect_right)
                (*i)->right_space = 0;
            else
                d_pipe.Insert((*(*i)->dst)->index, DISTANCE_RIGHT,
                                 (*i)->right_space + ilw + style.line.radius.second +
                                 shadow_to_add);
        }
        d_pipe.CombineLeftRightToPair_Sum(chart->hscaleAutoXGap);
        distances += d_pipe;

        //Finally add the requirements of the content
        d.CombineLeftRightToPair_Max(chart->hscaleAutoXGap);
        d.CombineLeftRightToPair_Single(chart->hscaleAutoXGap);
        d.CopyBoxSideToPair(chart->hscaleAutoXGap);
        distances += d;
    } else {
        EntityDistanceMap d;
        double max_width = 0; //the widest label plus margins
        for (auto i = follow.begin(); i!=follow.end(); i++) {
            if ((*i)->content)
                chart->WidthArcList(*((*i)->content), d);
            double width = (*i)->parsed_label.getTextWidthHeight().x;
            //calculated margins (only for first segment) and save them
            if (i==follow.begin()) {
                const Area tcov = (*i)->parsed_label.Cover(0, width, style.line.LineWidth()+chart->emphVGapInside);
                DoublePair margins = style.line.CalculateTextMargin(tcov, 0);
                width += margins.first + margins.second;
                (*i)->sx_text = margins.first;
                (*i)->dx_text = margins.second;
            } else {
                (*i)->sx_text = (*i)->dx_text = style.line.LineWidth();
            }
            max_width = max(max_width, width);
        }

        //Now d contains distance requirements within this emph box series
        //And width contains the widest
        double left_space_inside = d.Query((*src)->index, DISTANCE_LEFT);
        double right_space_inside = d.Query((*dst)->index, DISTANCE_RIGHT);

        if (src==dst) {
            //ensure that text fits
            left_space_inside = std::max(max_width/2, left_space_inside);
            right_space_inside = std::max(max_width/2, right_space_inside);
        } else {
            //do a default margin and ensure that internals fit
            const double def_margin = chart->XCoord(0.25);
            left_space_inside = max(def_margin, left_space_inside);
            right_space_inside = max(def_margin, right_space_inside);
        }

        //add gap and linewidth
        max_width += 2*chart->emphVGapInside;
        left_space =  left_space_inside +  chart->emphVGapInside + style.line.LineWidth();
        right_space = right_space_inside + chart->emphVGapInside + style.line.LineWidth();

        //Check box_side requirements
        const std::pair<double, double> l_tmp = d.QueryBoxSide((*src)->index, true); //left req
        const std::pair<double, double> r_tmp = d.QueryBoxSide((*dst)->index, false); //left req
        if (left_space_inside < l_tmp.second) left_space += l_tmp.second - left_space_inside;
        if (right_space_inside < r_tmp.first) right_space += r_tmp.first - right_space_inside;
        d.InsertBoxSide((*src)->index-1, l_tmp.first, left_space);
        d.InsertBoxSide((*dst)->index, right_space, r_tmp.second);
        
        //convert the side requirements to pairwise distances
        d.CombineLeftRightToPair_Max(chart->hscaleAutoXGap);
        d.CombineLeftRightToPair_Single(chart->hscaleAutoXGap);
        d.CopyBoxSideToPair(chart->hscaleAutoXGap);

        //if we span multiple entities ensure that text fits
        if (src!=dst && max_width > left_space + right_space)
            distances.Insert((*src)->index, (*dst)->index, max_width - left_space - right_space);
        
        //Add side distances
        distances.Insert((*src)->index, DISTANCE_LEFT, left_space);
        distances.Insert((*dst)->index, DISTANCE_RIGHT, right_space);
        distances += d;
    }
}

//Will only be called for the first box of a multi-segment box/pipe series
double ArcEmphasis::Height(AreaList &cover)
{
    if (!valid) return 0;
    if (pipe) {
        //Collect cover information from labels and linewidth, so compression of content arrows can be done
        //Determine thickest line for precise pipe alignment
        double max_lineWidth = 0;
        for (auto i = follow.begin(); i!=follow.end(); i++)
            max_lineWidth = std::max(max_lineWidth, (*i)->style.line.LineWidth());
        double lowest_line_bottom =  max_lineWidth + chart->emphVGapInside;
        //Determine highest label and collect all text covers
        //Also calcualte all x positioning
        double lowest_label_bottom = 0;
        double lowest_opaque_label_bottom = 0;
        Area label_covers;
        for (auto i = follow.begin(); i!=follow.end(); i++) {
            //Variables already set (all of them rounded):
            //pipe_connect true if a segment connects to us directly
            //left_space, right_space contains how much our content expands beyond the entity line,
            (*i)->yPos = 0;
            (*i)->area.clear();
            (*i)->area_draw.clear();
            (*i)->draw_is_different = false;

            //Set pipe_block.x, sx_text, dx_text in each segment, in the meantime
            //pipe_block contains the outside of the pipe, with the exception of the curvature (since it is a rect)
            (*i)->y_text = ceil(chart->emphVGapOutside + (*i)->style.line.LineWidth() +
                           chart->emphVGapInside);
            (*i)->area.clear();
            (*i)->pipe_block.x.from = chart->XCoord((*i)->src) - (*i)->left_space; //already rounded
            (*i)->pipe_block.x.till = chart->XCoord((*i)->dst) + (*i)->right_space;
            (*i)->sx_text = (*i)->pipe_block.x.from + (*i)->style.line.LineWidth() + chart->emphVGapInside; //not rounded
            (*i)->dx_text = (*i)->pipe_block.x.till - (*i)->style.line.LineWidth() - chart->emphVGapInside;
            switch (style.side.second) {
            case SIDE_RIGHT: (*i)->dx_text -= style.line.radius.second; break;
            case SIDE_LEFT:  (*i)->sx_text += style.line.radius.second; break;
            }
            (*i)->text_cover = (*i)->parsed_label.Cover((*i)->sx_text, (*i)->dx_text, (*i)->y_text);
            // omit text cover for pipes if the pipe is fully opaque,
            // in that case content can be drawn at same position as label - opaque pipe will cover anyway
            double y = (*i)->y_text + (*i)->parsed_label.getTextWidthHeight().y;
            if (y == (*i)->y_text && !content)
                y += Label("M", chart, style.text).getTextWidthHeight().y; 
            if (style.solid.second < 255) {
                label_covers += (*i)->text_cover;
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
        if (content)
            y = ceil(chart->PlaceListUnder(content->begin(), content->end(), ceil(y),
                                           lowest_line_bottom, label_covers));
        //now y contains the bottom of the content arrows (if any),
        //adjust if an opaque pipe's label was not yet considered in y
        y = std::max(y, lowest_opaque_label_bottom);
        y += chart->emphVGapInside + max_lineWidth;
        //now y contains the bottommost pixel of the pipe itself
        total_height = y = ceil(y);
        //Now set the y coordinate in all segments
        //Also calculate the Contours that will be used for drawing
        for (auto i = follow.begin(); i!=follow.end(); i++) {
            const double ilw = (*i)->style.line.LineWidth();

            //fill in pipe_block.y (both are integer)
            (*i)->pipe_block.y.from = chart->emphVGapOutside;
            (*i)->pipe_block.y.till = y;

            XY cs((*i)->pipe_block.x.from, (*i)->pipe_block.y.MidPoint());
            XY cd((*i)->pipe_block.x.till, (*i)->pipe_block.y.MidPoint());
            const XY rad(style.line.radius.second, (*i)->pipe_block.y.Spans()/2); //we use the first pipe's line.radius
            if (style.side.second == SIDE_LEFT) std::swap(cs, cd); //use the first pipe's fromright, not (*i)->fromright
            //now cd is the one with the hole
            Contour back_end(cs, rad.x, rad.y);
            Contour forw_end(cd, rad.x, rad.y);
            //(*i)->pipe_block, back_end and forw_end are now all outer edge
            //we need to shring by ilw/2 to get the line contour and by ilw/2-line.width/2 to get the fill contour
            const double gap_for_line = -ilw/2;
            const double gap_for_fill = -ilw + (*i)->style.line.width.second/2;
            (*i)->pipe_body_fill = Block((*i)->pipe_block.x, Range((*i)->pipe_block.y).Expand(gap_for_fill));
            (*i)->pipe_body_line = Block((*i)->pipe_block.x, Range((*i)->pipe_block.y).Expand(gap_for_line));
            (*i)->pipe_shadow = (*i)->pipe_block;
            //Do back end
            if (rad.x>0) {
                (*i)->pipe_body_fill += back_end.CreateExpand(gap_for_fill);
                (*i)->pipe_body_line += back_end.CreateExpand(gap_for_line);
                (*i)->pipe_shadow += back_end;
            } else {
                //square end: chop off from fill and line
                (*i)->pipe_body_fill -= Block(Range(cs.x, cs.x), (*i)->pipe_block.y).Expand(gap_for_fill);
                (*i)->pipe_body_line -= Block(Range(cs.x, cs.x), (*i)->pipe_block.y).Expand(gap_for_line);
            }
            //Do front end, and whole_line
            if ((*i)->pipe_connect_forw) {
                (*i)->area = (*i)->pipe_shadow;
                if (content && (*i)->style.solid.second < 255)
                    (*i)->area_draw = (*i)->area.CreateExpand(chart->trackFrameWidth);
                //We take a big, unshrunken back-end out of fill: (this will be the next segments outer edge)
                if (rad.x>0) {
                    (*i)->pipe_body_fill -= forw_end;
                    (*i)->area -= forw_end;
                    //below we need to first expand forw_end before substracting it
                    //the other way is not ok: Expand fails in expanding negative arcs
                    if (content && (*i)->style.solid.second < 255) {
                        (*i)->area_draw -= forw_end.CreateExpand(-chart->trackFrameWidth);
                        (*i)->area_draw *= Contour(style.side.second == SIDE_RIGHT ? 0 : chart->total.x, cd.x,
                                                   -chart->trackFrameWidth-1, total_height+chart->trackFrameWidth+1);
                    }
                }
                //Line shall fall entirely under the next segment, so we add a small block to it
                (*i)->pipe_body_line += Block(Range(cd.x-ilw/2, cd.x+ilw/2), Range((*i)->pipe_block.y).Expand(gap_for_line));
                (*i)->pipe_whole_line = (*i)->pipe_body_line;
                //shadow need no asjustment, as it will be merged with next segment
                //we clear the holes: no need
                (*i)->pipe_hole_fill.clear();
                (*i)->pipe_hole_line.clear();
                (*i)->pipe_hole_curve = Edge(); //wont draw anything
            } else {
                //No connection, we draw this end, too
                if (rad.x>0) {
                    (*i)->pipe_body_fill -= forw_end.CreateExpand(-(*i)->style.line.width.second/2.);
                    (*i)->pipe_whole_line = (*i)->pipe_body_line + Area(forw_end.CreateExpand(gap_for_line));
                    (*i)->pipe_body_line -= forw_end.CreateExpand(gap_for_line);

                    //for shaodow we add
                    (*i)->pipe_shadow += forw_end;
                    (*i)->pipe_hole_line = forw_end.CreateExpand(gap_for_line).GetFirst();
                    (*i)->pipe_hole_fill = forw_end.CreateExpand(gap_for_fill).GetFirst();
                    (*i)->pipe_hole_curve = Edge(cd, rad.x+gap_for_line, rad.y+gap_for_line, 0,
                                                 style.side.second == SIDE_RIGHT ? 270 : 90, 
                                                 style.side.second == SIDE_RIGHT ? 90 : 270);
                } else {
                    //just chop off from fill and line
                    (*i)->pipe_body_fill -= Block(Range(cd.x, cd.x), (*i)->pipe_block.y).Expand(gap_for_fill);
                    (*i)->pipe_body_line -= Block(Range(cd.x, cd.x), (*i)->pipe_block.y).Expand(gap_for_line);
                    (*i)->pipe_whole_line = (*i)->pipe_body_line;
                    //we clear the holes: no need, body will draw the line we need
                    (*i)->pipe_hole_fill.clear();
                    (*i)->pipe_hole_line.clear();
                    (*i)->pipe_hole_curve = Edge(); //wont draw anything
                }
                (*i)->area = (*i)->pipe_shadow;
                if (content && (*i)->style.solid.second < 255)
                    (*i)->area_draw = (*i)->area.CreateExpand(chart->trackFrameWidth);
            }
            //Finalize covers
            (*i)->area.arc = *i;
            if (content && (*i)->style.solid.second < 255) {
                //Make a frame, add it to the already added label
                (*i)->area_draw -= (*i)->area;
                (*i)->area_draw += (*i)->text_cover.CreateExpand(chart->trackExpandBy);
                (*i)->area_draw.arc = *i;
                (*i)->area_draw_is_frame = true;
                (*i)->draw_is_different = true;
            }
            //now determine the cover to be used for placement
            if (style.shadow.offset.second)
                cover += (*i)->pipe_shadow + Area((*i)->pipe_shadow).Shift(XY(style.shadow.offset.second, style.shadow.offset.second));
            else
                cover += (*i)->pipe_shadow;
            //merge shadows of connected previous segment to ours
            if ((*i)->pipe_connect_back) {
                auto i_neigh = i; i_neigh--;
                (*i_neigh)->pipe_shadow = ((*i)->pipe_shadow + (*i_neigh)->pipe_shadow).GetFirst();
                (*i)->pipe_shadow.clear();
            }
        }
        cover.mainline = Range(chart->emphVGapOutside, total_height);  //totalheight includes the top emphvgapoutside for pipes
        for (auto i = follow.begin(); i!=follow.end(); i++)
            (*i)->pipe_shadow = (*i)->pipe_shadow.CreateExpand(-(*i)->style.line.width.second/2);
    } else {
        //We are a box here
        //A few explanations of the variables exact meaning
        //the upper edge of the upper line of each segment is at yPos
        //total_height includes linewidths and shadow, but not emphVGapOutside (contrary for pipes)
        //left_space and right_space includes linewidth
        //height includes the upper linewidth, emphvgapinside, content, lower emphvgapinside, but not lower lw
        //sx and dx are the inner edges of the lines of the whole box
        const double lw = style.line.LineWidth();
        const double sx = chart->XCoord(src) - left_space + lw;
        const double dx = chart->XCoord(dst) + right_space - lw;

        double y = chart->emphVGapOutside;
        for (auto i = follow.begin(); i!=follow.end(); i++) {
            (*i)->yPos = y;
            // y now points to the *top* of the line of the top edge of this box
            // if we are pipe, we draw the segment side-by side, so we reset y here

            //Advance upper line and spacing
            y += (*i)->style.line.LineWidth() + chart->emphVGapInside;
            (*i)->y_text = y;
            (*i)->sx_text = sx + (*i)->sx_text - lw + chart->emphVGapInside;  //both sx and sx_text includes a lw
            (*i)->dx_text = dx - (*i)->dx_text + lw - chart->emphVGapInside;
            //Add text cover & draw if necessary
            (*i)->text_cover = (*i)->parsed_label.Cover((*i)->sx_text, (*i)->dx_text, (*i)->y_text);
            //Advance label height
            double th = (*i)->parsed_label.getTextWidthHeight().y;
            //Position arrows if any under the label
            AreaList content_cover = (*i)->text_cover;
            if ((*i)->content) {
                Area limit = (*i)->text_cover;
                if (i==follow.begin() && style.line.corner.second != CORNER_NONE && style.line.radius.second>0) {
                    //Funnily shaped box, prevent content from hitting it
                    Block b(sx, dx, y, lw+y+style.line.radius.second*4);
                    limit += Contour(Block(sx, dx, 0, y+lw+style.line.radius.second)) -
                             style.line.CreateRectangle(b);
                    if (style.line.corner.second == CORNER_NOTE) {
                        const double r = style.line.radius.second + style.line.RadiusIncMul()*lw/2 -
                            (style.line.IsDouble() ? style.line.DoubleSpacing() : 0);
                        limit += Contour(b.x.till-r, b.y.from, b.x.till-r, b.y.from+r,
                                         b.x.till, b.y.from+r);
                    }
                }
                y = chart->PlaceListUnder((*i)->content->begin(), (*i)->content->end(),
                                           y+th, y, limit, compress, &content_cover);  //no extra margin below text
            } else {
                y += th; //no content, just add textheight
            }
            if (i==--follow.end() && style.line.corner.second != CORNER_NONE && style.line.radius.second>0) {
                //Funnily shaped box, prevent it content from hitting the bottom of the content
                Block b(sx, dx, 0, y);
                MscLineAttr limiter_line(style.line);
                limiter_line.radius.second += chart->compressGap;
                const Area bottom = Contour(Block(sx, dx, limiter_line.radius.second+1, y+1)) -
                              limiter_line.CreateRectangle(b);
                double tp;
                double off = content_cover.OffsetBelow(bottom, tp);
                if (off>0 && compress) y-=off;
                if (off<0) y-=off;
            }
            y += chart->emphVGapInside;
            //Make segment as tall as needed to accomodate curvature
            //if (style.line.radius.second>0) {
            //    double we_need_this_much_for_radius = (*i)->style.line.LineWidth();
            //    if (i==follow.begin())
            //        we_need_this_much_for_radius += style.line.radius.second;
            //    if (i==--follow.end())
            //        we_need_this_much_for_radius += style.line.radius.second;
            //    y = std::max(y, (*i)->yPos + we_need_this_much_for_radius);
            //}
            y = ceil(y);
            (*i)->height = y - (*i)->yPos;

            //Add the linewidth of the next box or the final one
            if (i==--follow.end())
                (*i)->height_w_lower_line = (*i)->height + lw;
            else
                (*i)->height_w_lower_line = (*i)->height + (*((++i)--))->style.line.LineWidth();
        } /* for cycle through segments */
        //Final advance of linewidth, the inner edge (y) is on integer
        total_height = y + lw - yPos;
        Block b(sx-lw, dx+lw, yPos, yPos + total_height);
        MscLineAttr line_for_outer_edge = style.line;
        line_for_outer_edge.radius.second += lw * style.line.RadiusIncMul();
        Area overall_box = line_for_outer_edge.CreateRectangle(b);
        // now we have all geometries correct, now calculate areas and covers
        for (auto i = follow.begin(); i!=follow.end(); i++) {
            (*i)->area = Contour(sx-lw, dx+lw, (*i)->yPos, (*i)->yPos + (*i)->height_w_lower_line) * overall_box;
            (*i)->area.arc = *i;
            if ((*i)->content) {
                //Make a frame, add it to the already added label
                (*i)->area_draw = (*i)->area.CreateExpand(chart->trackFrameWidth) - (*i)->area;
                (*i)->area_draw += (*i)->text_cover.CreateExpand(chart->trackExpandBy);
                (*i)->area_draw.arc = *i;
                (*i)->draw_is_different = true;
                (*i)->area_draw_is_frame = true;
            } else {
                (*i)->area_draw.clear();
                (*i)->draw_is_different = false;
                (*i)->area_draw_is_frame = false;
            }
        }
        overall_box.mainline = b.y;
        if (style.shadow.offset.second)
            cover += overall_box + Area(overall_box).Shift(XY(style.shadow.offset.second, style.shadow.offset.second));
        else
            cover += overall_box;
    }
    return yPos + total_height + style.shadow.offset.second + chart->emphVGapOutside;
}

void ArcEmphasis::ShiftBy(double y)
{
    if (!valid) return;
    if (y==0) return;
    for (auto i=follow.begin(); i!=follow.end(); i++) {
        if (pipe) {
            (*i)->pipe_block.Shift(XY(0,y));
            (*i)->pipe_shadow.Shift(XY(0,y));
            (*i)->pipe_body_line.Shift(XY(0,y));
            (*i)->pipe_whole_line.Shift(XY(0,y));
            (*i)->pipe_hole_line.Shift(XY(0,y));
            (*i)->pipe_body_fill.Shift(XY(0,y));
            (*i)->pipe_hole_fill.Shift(XY(0,y));
            (*i)->pipe_hole_curve.Shift(XY(0,y));
        }
        (*i)->y_text += y;
        (*i)->text_cover.Shift(XY(0,y));
        (*i)->ArcLabelled::ShiftBy(y);
        if ((*i)->content)
            chart->ShiftByArcList((*i)->content->begin(), (*i)->content->end(), y);
    }
}

//Will only be called for the first box of a multi-segment box series
void ArcEmphasis::PostPosProcess(double autoMarker)
{
    if (!valid) return;
    if (pipe) {
        //For pipes we first add those covers to chart->AllCovers that are not fully opaque,
        //then the content (only in the first segment)
        //then those segments, which are fully opaque
        //(this is because search is backwards and this arrangement fits the visual best
        for (auto i = follow.begin(); i!=follow.end(); i++)
            if ((*i)->valid && (*i)->style.solid.second < 255)
                (*i)->ArcLabelled::PostPosProcess(autoMarker);
        if (content)
            chart->PostPosProcessArcList(*content, autoMarker);
        for (auto i = follow.begin(); i!=follow.end(); i++)
            if ((*i)->valid && (*i)->style.solid.second == 255)
                (*i)->ArcLabelled::PostPosProcess(autoMarker);
        for (auto i = follow.begin(); i!=follow.end(); i++)
            chart->HideEntityLines((*i)->pipe_shadow);
    } else {
        //For boxes we always add the background cover first then the content
        //And we do this for each segment sequentially
        for (auto i = follow.begin(); i!=follow.end(); i++)
            if ((*i)->valid) {
                (*i)->ArcLabelled::PostPosProcess(autoMarker);
                if ((*i)->content)
                    chart->PostPosProcessArcList(*(*i)->content, autoMarker);
            }

        //Hide entity lines during the lines inside the box
        for (auto i = ++follow.begin(); i!=follow.end(); i++) {
            chart->HideEntityLines((*i)->text_cover);
            if ((*i)->style.line.IsDoubleOrTriple()) {
                const Range r((*i)->yPos, (*i)->yPos+(*i)->style.line.LineWidth());
                for (auto j=src; j!=++EIterator(dst); j++)
                    (*j)->status.HideRange(r);
            }
        }
        //hide the entity lines under the label of the first segment (not handled above)
        chart->HideEntityLines(text_cover);
        //hide top and bottom line if double
        if (style.line.IsDoubleOrTriple()) {
            const double lw = style.line.LineWidth();
            const double r_in = style.line.radius.second; //radius for inner edge
            const double r_out = r_in + lw * style.line.RadiusIncMul(); //radius for outer edge
            Block b(chart->XCoord(src) - left_space, chart->XCoord(dst) + right_space,
                    yPos, yPos+total_height); //The outer edge of the lines
            switch (style.line.corner.second) {
            case CORNER_BEVEL:
            case CORNER_ROUND:
                for (auto j=src; j!=++EIterator(dst); j++) {
                    const double x = chart->XCoord(j);
                    //distance from the changepoint towards the edges (=0 in edges, =r_in/out in the middle)
                    double off_in  = r_in - std::min(std::min(x-b.x.from-lw, b.x.till-lw-x), r_in);
                    double off_out = r_out- std::min(std::min(x-b.x.from,    b.x.till-x   ), r_out);
                    if (style.line.corner.second == CORNER_ROUND) {
                        off_out = r_out - sqrt(r_out*r_out - off_out*off_out);
                        off_in = r_in - sqrt(r_in*r_in - off_in*off_in);
                    }
                    off_in += lw;
                    const Range top   (b.y.from + ceil(off_out), b.y.from + ceil(off_in));
                    const Range bottom(b.y.till - ceil(off_in),  b.y.till - ceil(off_out));
                    (*j)->status.HideRange(top);
                    (*j)->status.HideRange(bottom);
                }
                break;
            case CORNER_NOTE:
                for (auto j=src; j!=++EIterator(dst); j++) {
                    const double x = chart->XCoord(j);
                    const double r = style.line.radius.second + style.line.RadiusIncMul()*lw/2 -
                        (style.line.IsDouble() ? style.line.DoubleSpacing() : 0);
                    const double off_in  = x < b.x.till-lw-r ? lw : lw+r+1;  //HACK: +1 is for EMF
                    const double off_out = r_out - std::min(b.x.till-x, r_out);
                    const Range top   (b.y.from + off_out, b.y.from + off_in);
                    const Range bottom(b.y.till - lw, b.y.till);
                    (*j)->status.HideRange(top);
                    (*j)->status.HideRange(bottom);
                }
                break;
            case CORNER_NONE:
                const Range top(b.y.from, b.y.from + lw);
                const Range bottom(b.y.till - lw, b.y.till);
                for (auto j=src; j!=++EIterator(dst); j++) {
                    (*j)->status.HideRange(top);
                    (*j)->status.HideRange(bottom);
                }
            }
        }
    }
}

//Draw a pipe, this is called for each segment
//topside is the bigger part of the pipe
//backside is the small oval visible form the back of the pipe
//this->yPos is the outer edge of the top line
//this->left_space and right_space includes linewidth
void ArcEmphasis::DrawPipe(bool topSideFill, bool topSideLine, bool backSide, bool shadow, bool text,
                           double next_lw)
{
    if (shadow) {
        //Shadow under the whole pipe
        chart->Shadow(pipe_shadow, style.shadow);
    }
    if (backSide) {
        //The back of the main pipe
        MscFillAttr fill = style.fill;
        //backside is always filled fully opaque
        fill.color.second.a = 255;
        fill.color2.second.a = 255;
        chart->Fill(pipe_body_fill, fill);
        //the back of the small ellipsis visible from the side
        if (fill.gradient.second == GRADIENT_UP)
            fill.gradient.second = GRADIENT_DOWN;
        else if (fill.gradient.second == GRADIENT_DOWN)
            fill.gradient.second = GRADIENT_UP;
        chart->Fill(pipe_hole_fill, fill);
        //Draw the backside line
        chart->Line(pipe_hole_curve, style.line);
    }
    if (topSideFill) {
        //apply the transparency of the solid attribute
        MscFillAttr fill = style.fill;
        fill.color.second.a = unsigned(style.solid.second) * unsigned(fill.color.second.a) / 255;
        chart->Fill(pipe_body_fill, fill);
    }
    if (topSideLine) {
        cairo_line_join_t t = chart->SetLineJoin(CAIRO_LINE_JOIN_BEVEL);
        const double x = style.side.second == SIDE_RIGHT ? pipe_block.x.till : pipe_block.x.from;
        Contour clip(x, style.side.second == SIDE_LEFT ? chart->total.x : 0, 0, chart->total.y);
        if (style.line.radius.second>0 && pipe_connect_forw) {
            const XY c(x, pipe_block.y.MidPoint());
            clip = (clip-Contour(c, style.line.radius.second-next_lw/2, pipe_block.y.Spans()/2.-next_lw/2)).GetFirst();
        }
        chart->Clip(clip);
        if (!style.line.IsDoubleOrTriple() || drawing_variant==0)
            chart->Line(pipe_body_line, style.line); //basic variant, in case of double & triple, lines cross
        else if (style.line.IsDouble()) {
            chart->SetLineAttr(style.line);
            if (drawing_variant==1) { //advanced: lines do not cross
                chart->singleLine(pipe_whole_line.CreateExpand(style.line.DoubleSpacing()), style.line);
                chart->singleLine(pipe_body_line.CreateExpand(-style.line.DoubleSpacing()), style.line);
                chart->singleLine(pipe_hole_line.CreateExpand(-style.line.DoubleSpacing()), style.line);
            } else { //very advanced: proper double line joint
                chart->singleLine(pipe_whole_line.CreateExpand(style.line.DoubleSpacing()), style.line); //outer
                chart->singleLine(pipe_body_line.CreateExpand(-style.line.DoubleSpacing()) -
                                  pipe_hole_line.CreateExpand(style.line.DoubleSpacing()), style.line);  //inner body
                chart->singleLine(pipe_hole_line.CreateExpand(-style.line.DoubleSpacing()), style.line); //inner hole
            }
        } else if (style.line.IsTriple()) {
            chart->SetLineAttr(style.line);
            //here variant 1 and 2 result in the same
            chart->singleLine(pipe_whole_line.CreateExpand(style.line.TripleSpacing()), style.line);  //outer
            chart->singleLine(pipe_body_line.CreateExpand(-style.line.TripleSpacing()) -
                                pipe_hole_line.CreateExpand(style.line.TripleSpacing()), style.line);  //inner body
            chart->singleLine(pipe_hole_line.CreateExpand(-style.line.TripleSpacing()), style.line);   //inner hole
            cairo_set_line_width(chart->cr, style.line.TripleMiddleWidth());
            chart->singleLine(pipe_body_line, style.line); //middle line
        }
        chart->UnClip();
        chart->SetLineJoin(t);
    }
    if (text)
        parsed_label.Draw(sx_text, dx_text, y_text);
}

void ArcEmphasis::Draw()
{
    if (!valid) return;
    if (pipe) {
        //First shadows
        for (auto i = follow.begin(); i!=follow.end(); i++)
            (*i)->DrawPipe(false, false, false, true, false, 0);  //dummy 0
        for (auto i = follow.begin(); i!=follow.end(); i++) {
            //Dont draw the topside fill
            //Draw the topside line only if pipe is fully transparent. Else we may cover the line.
            //Draw the backside in any case.
            //Do not draw text
            auto i_next = i; i_next++;
            const double next_linewidth = i_next!=follow.end() ? (*i_next)->style.line.width.second : 0;
            (*i)->DrawPipe(false, (*i)->style.solid.second == 0, true, false, false, next_linewidth);
            if (content && drawEntityLines)
                chart->DrawEntityLines(yPos, height, (*i)->src, ++EIterator((*i)->dst));
        }
        if (content)
            chart->DrawArcList(*content);
        for (auto i = follow.begin(); i!=follow.end(); i++) {
            //Draw the topside fill only if the pipe is not fully transparent.
            //Draw the topside line in any case
            //Do not draw the backside (that may content arrow lines already drawn)
            //Draw the text
            auto i_next = i; i_next++;
            const double next_linewidth = i_next!=follow.end() ? (*i_next)->style.line.width.second : 0;
            (*i)->DrawPipe((*i)->style.solid.second > 0, true, false, false, true, next_linewidth);
        }
    } else {
        //For boxes draw background for each segment, then separator lines, then bounding rectangle lines, then content
        const double lw = style.line.LineWidth();
        MscLineAttr line = style.line;  //We will vary the radius, so we need a copy
        Block r(chart->XCoord(src) - left_space, chart->XCoord(dst) + right_space,
                yPos, yPos+total_height); //The outer edge of the lines
        line.radius.second = std::min(std::min(r.y.Spans()/2 - lw, r.x.Spans()/2 - lw), line.radius.second);
        //The radius specified in style.line will be that of the inner edge of the line
        if (line.radius.second > 0)
            line.radius.second += lw * line.RadiusIncMul(); //now it is appropriate for the outer edge
        //First draw the shadow.
        r.Expand(-line.width.second/2.);
        line.radius.second -= line.width.second/2. * line.RadiusIncMul();
        chart->Shadow(r, line, style.shadow);
        //Do a clip region for the overall box (for round corners)
        //at half a linewidth from the inner edge (use the width of a single line!)
        line.radius.second -= (lw-line.width.second) * line.RadiusIncMul();
        r.Expand(-lw+line.width.second);
        chart->Clip(r, line);
        for (auto i = follow.begin(); i!=follow.end(); i++) {
            //Overall rule for background fill:
            //for single line borders we fill up to the middle of the border
            //for double line borders we fill up to the middle of the inner line of the border
            //style.line.LineWidth() gives the full width of the (double) line, width.second is just one line of it
            //for single lines style.line.LineWidth()==style.line.width.second
            double sy = (*i)->yPos + (*i)->style.line.LineWidth() - (*i)->style.line.width.second/2.;
            double dy = (*i)->yPos + (*i)->height;
            //decrease upper limit for the first one (note+triple line has areas higher than this to cover)
            //clip will cut away the not needed areas
            if (i==follow.begin())
                sy -= lw;
            //Increase the fill area downward by half of the linewidth below us
            PtrList<ArcEmphasis>::const_iterator next = i;
            next++;
            if (next==follow.end())
                dy += style.line.width.second/2.;
            else
                dy += (*next)->style.line.width.second/2.;
            //fill wider than r.x - note+triple line has wider areas to cover, clip will cut away excess
            chart->Fill(Block(r.x.from, r.x.till+lw, sy, dy), (*i)->style.fill);
            //if there are contained entities, draw entity lines, strictly from inside of line
            if ((*i)->content && (*i)->drawEntityLines)
                chart->DrawEntityLines((*i)->yPos, (*i)->height + (*i)->style.line.LineWidth(), src, ++EIterator(dst));
        }
        chart->UnClip();
        //shring to the inner edge
        r.Expand(-line.width.second/2.);
        line.radius.second -= line.width.second/2. * line.RadiusIncMul();
        //Draw box lines - Cycle only for subsequent boxes
        for (auto i = ++(follow.begin()); i!=follow.end(); i++) {
            const double y = (*i)->yPos + (*i)->style.line.LineWidth()/2;
            const XY magic(1,0);  //XXX needed in windows
            chart->Line(XY(r.x.from, y)-magic, XY(r.x.till, y), (*i)->style.line);
        }
        //Finally draw the overall line around the box, expand to midpoint of line
        r.Expand(lw/2);
        line.radius.second += (lw/2)  * line.RadiusIncMul();
        chart->Line(r, line);
        //XXX double line joints: fix it
        for (auto i = follow.begin(); i!=follow.end(); i++) {
            (*i)->parsed_label.Draw((*i)->sx_text, (*i)->dx_text, (*i)->y_text);
            if ((*i)->content)
                chart->DrawArcList(*(*i)->content);
            //if (i==follow.begin()) {
            //    const Area tcov = (*i)->parsed_label.Cover(0, (*i)->parsed_label.getTextWidthHeight().x, style.line.LineWidth()+chart->emphVGapInside);
            //    DoublePair margins = style.line.CalculateTextMargin(tcov, 0, follow.size()==1?chart:NULL);
            //}
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////

ArcDivider::ArcDivider(MscArcType t, Msc *msc) :
    ArcLabelled(t, msc, msc->Contexts.back().styles["divider"]),
        nudge(t==MSC_COMMAND_NUDGE), extra_space(t==MSC_ARC_DISCO ? msc->discoVgap : 0)
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

void ArcDivider::AttributeNames(Csh &csh, bool nudge)
{
    if (nudge) return;
    ArcLabelled::AttributeNames(csh);
    MscStyle style(STYLE_DEFAULT, ArrowHead::NONE, true, true, false, false, true, false, true, true, false); //no arrow, fill, shadow solid side
    style.AttributeNames(csh);
}

bool ArcDivider::AttributeValues(const std::string attr, Csh &csh, bool nudge)
{
    if (nudge) return false;
    MscStyle style(STYLE_DEFAULT, ArrowHead::NONE, true, true, false, false, true, false, true, true, false); //no arrow, fill, shadow solid side
    if (style.AttributeValues(attr, csh)) return true;
    if (ArcLabelled::AttributeValues(attr, csh)) return true;
    return false;
}

void ArcDivider::PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level)
{
    if (!valid) return;
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
    if (!valid) return 0;
    yPos = 0;
    if (nudge) {
        Block b(0, chart->total.x, 0, chart->nudgeSize);
        area = b;
        area.mainline=b.y;
        cover+=area;
        return chart->nudgeSize;
    }
    double y = wide ? 0 : chart->arcVGapAbove;
    y += extra_space;
    const double charheight = Label("M", chart, style.text).getTextWidthHeight().y;

    XY wh = parsed_label.getTextWidthHeight();
    if (!wh.y) wh.y = charheight;
    centerline = y+wh.y/2;
    text_margin = wide ? 0 : chart->XCoord(MARGIN*1.3);
    line_margin = chart->XCoord(MARGIN);
    text_cover = parsed_label.Cover(text_margin, chart->total.x-text_margin, y);
    area = text_cover;
    area.arc = this;

    //Add a cover block for the line, if one exists
    if (style.line.type.second != LINE_NONE && style.line.color.second.valid && style.line.color.second.a>0)
        area += Block(line_margin, chart->total.x-line_margin,
                      centerline - style.line.LineWidth()*2, centerline + style.line.LineWidth()*2);
    if (!wide)
        wh.y += chart->arcVGapAbove + chart->arcVGapBelow;
    height = wh.y + extra_space;
    //Discontinuity lines cannot be compressed much
    if (type==MSC_ARC_DISCO)
        area.mainline += Range(wide ? 0 : chart->arcVGapAbove, height- (wide ? 0 :chart->arcVGapBelow));
    else
        area.mainline += Range(centerline-charheight/2, centerline+charheight/2);
    cover += area;
    return height;
}

void ArcDivider::ShiftBy(double y)
{
    if (!valid) return;
    if (!nudge)
        text_cover.Shift(XY(0,y));
    ArcLabelled::ShiftBy(y);
}

void ArcDivider::PostPosProcess(double autoMarker)
{
    if (!valid) return;
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
    if (!valid) return;
    if (nudge) return;
    parsed_label.Draw(text_margin, chart->total.x-text_margin, yPos + (wide ? 0 : chart->arcVGapAbove));
    //determine widest extent for coverage at the centerline+-style.line.LineWidth()/2;
    const double lw2 = ceil(style.line.LineWidth()/2.);
    Block b(line_margin, chart->total.x-line_margin, yPos + centerline - lw2, yPos + centerline + lw2);
    Range r = (text_cover * b).GetBoundingBox().x;
    if (r.IsInvalid())
        chart->Line(XY(line_margin, yPos + centerline), XY(chart->total.x-line_margin, yPos + centerline), style.line);
    else {
        chart->Line(XY(line_margin, yPos + centerline), XY(r.from-chart->emphVGapInside, yPos + centerline), style.line);
        chart->Line(XY(r.till+chart->emphVGapInside, yPos + centerline), XY(chart->total.x-line_margin, yPos + centerline), style.line);
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
    if (!valid) return;
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
    d.CopyBoxSideToPair(chart->hscaleAutoXGap);
    distances += d;
}

double ArcParallel::Height(AreaList &cover)
{
    if (!valid) return 0;
    heights.clear();
    heights.reserve(blocks.size());
    double height = 0;
    for (auto i=blocks.begin(); i != blocks.end(); i++) {
        AreaList cover_block;
        //Each parallel block is compressed without regard to the others
        double h = chart->HeightArcList((*i)->begin(), (*i)->end(), cover_block);
        height = std::max(height, h);
        heights.push_back(height);
        cover += cover_block;
    }
    return height;
}

void ArcParallel::ShiftBy(double y)
{
    if (!valid) return;
    for (auto i=blocks.begin(); i!=blocks.end(); i++)
        chart->ShiftByArcList((*i)->begin(), (*i)->end(), y);
    ArcBase::ShiftBy(y);
}

void ArcParallel::PostPosProcess(double autoMarker)
{
    if (!valid) return;
    ArcBase::PostPosProcess(autoMarker);
    int n=0;
    //For automarker, give the bottom of the largest of previous blocks
    for (auto i=blocks.begin(); i!=blocks.end(); i++, n++)
        chart->PostPosProcessArcList(*(*i),
            n>0 && heights[n-1]>0 ? yPos + heights[n-1] : autoMarker);
}

void ArcParallel::Draw()
{
    if (!valid) return;
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

bool CommandEntity::AddAttribute(const Attribute &)
{
    return false;
}

void CommandEntity::AttributeNames(Csh &csh)
{
    return;
}

bool CommandEntity::AttributeValues(const std::string attr, Csh &csh)
{
    return false;
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

CommandEntity *CommandEntity::ApplyShowHide(bool show)
{
    for (auto i=entities.begin(); i!=entities.end(); i++) {
        if ((*i)->show_is_explicit) continue;
        (*i)->show.first = true;
        (*i)->show.second = show;
    }
    return this;
}


void CommandEntity::PostParseProcess(EIterator &left, EIterator &right, Numbering &number,
                                     bool top_level)
{
    if (!valid) return;
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
        left = chart->EntityMinByPos(left, (*i_def)->itr);
        right = chart->EntityMaxByPos(right, (*i_def)->itr);
        double w = (*i_def)->Width();
        if ((*(*i_def)->itr)->maxwidth < w) (*(*i_def)->itr)->maxwidth = w;
    }
}


void CommandEntity::Width(EntityDistanceMap &distances)
{
    if (!valid) return;
    //Add distances for entity heading
    //Start by creating a map in which distances are ordered by index
    std::map<unsigned, double> dist; //same distance on left and right side
    //in PostParseProcess we created an entitydef for all entities shown here. "full_heading" not even checked here
    for (auto i = entities.begin(); i!=entities.end(); i++) {
        //Take entity height into account or draw it if show=on was added
        if ((*i)->shown) {
			const double halfsize = (*(*i)->itr)->maxwidth/2;
            const unsigned index = (*(*i)->itr)->index;
            auto ii = dist.find(index);
            if (ii==dist.end() || ii->second < halfsize) 
                dist[index] = halfsize;
        }
    }
    if (dist.size()==0) return;
    //Now convert neighbouring ones to box_side distances, and add the rest as normal side distance
    distances.Insert(dist.begin()->first,  DISTANCE_LEFT, dist.begin()->second); //leftmost distance
    distances.Insert(dist.rbegin()->first, DISTANCE_RIGHT,dist.rbegin()->second); //rightmost distance
    for (auto d = dist.begin(); d!=--dist.end(); d++) {
        auto d_next = d; d_next++;
        if (d->first == d_next->first-1) //neighbours
            distances.InsertBoxSide(d->first, d->second, d_next->second);
        else {
            distances.Insert(d->first, DISTANCE_RIGHT, d->second);
            distances.Insert(d_next->first, DISTANCE_LEFT, d_next->second);
        }
    }
}

double CommandEntity::Height(AreaList &cover)
{
    if (!valid) return 0;
    height = 0;
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
    if (!valid) return;
    for (auto i = entities.begin(); i!=entities.end(); i++)
        (*i)->ShiftBy(y);
    ArcCommand::ShiftBy(y);
}

void CommandEntity::PostPosProcess(double autoMarker)
{
    if (!valid) return;
    for (auto i = entities.begin(); i!=entities.end(); i++)
        (*i)->PostPosProcess();
    ArcCommand::PostPosProcess(autoMarker);
    if (height>0) {
        if (chart->headingSize == 0) chart->headingSize = yPos + height;
        chart->headingSize = std::min(chart->headingSize, yPos + height);
    }
}

void CommandEntity::Draw()
{
    if (!valid) return;
    for (auto i = entities.begin(); i!=entities.end(); i++)
        if ((*i)->shown) 
            (*i)->Draw();
}

//////////////////////////////////////////////////////////////////////////////////////

bool CommandNewpage::AddAttribute(const Attribute &)
{
    return false;
}

void CommandNewpage::AttributeNames(Csh &csh)
{

}

bool CommandNewpage::AttributeValues(const std::string attr, Csh &csh)
{
    return false;
}

double CommandNewpage::Height(AreaList &cover)
{
    if (!valid) return 0;
    Block b(0, chart->total.x, -chart->nudgeSize/2, chart->nudgeSize/2);
    area_draw = b;
    area_draw.mainline = b.y;
    draw_is_different = true; //area is empty - never find this
    return 0;
}

void CommandNewpage::PostPosProcess(double autoMarker)
{
    if (!valid) return;
    chart->yPageStart.push_back(yPos);
}

//////////////////////////////////////////////////////////////////////////////////////

double CommandNewBackground::Height(AreaList &cover)
{
    if (!valid) return 0;
    Block b(0, chart->total.x, -chart->nudgeSize/2, chart->nudgeSize/2);
    area_draw = b;
    area_draw.mainline = b.y;
    draw_is_different = true; //area is empty - never find this
    return 0;
}

void CommandNewBackground::PostPosProcess(double autoMarker)
{
    if (!valid) return;
    chart->Background[yPos] = fill;
}

//////////////////////////////////////////////////////////////////////////////////////
void CommandNumbering::PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level)
{
    if (!valid) return;
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
    if (!valid) return 0;
    Block b(0, chart->total.x, offset-chart->nudgeSize/2, offset+chart->nudgeSize/2);
    area_draw = b;
    area_draw.mainline = b.y;
    draw_is_different = true; //area is empty - never find this
    return 0;
}

void CommandMark::ShiftBy(double y)
{
    if (!valid) return;
    ArcCommand::ShiftBy(y);
    chart->Markers[name].second = yPos+offset;
}

//////////////////////////////////////////////////////////////////////////////////////

#define EMPTY_MARGIN_X 50
#define EMPTY_MARGIN_Y 5

CommandEmpty::CommandEmpty(Msc *msc) :
    ArcCommand(MSC_COMMAND_EMPTY, msc), parsed_label(msc)
{
    StringFormat format;
    format.Default();
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
    if (!valid) return 0;
    yPos = 0;
    const XY wh = parsed_label.getTextWidthHeight();
    Contour a = Block((chart->total.x-wh.x)/2, (chart->total.x+wh.x)/2, EMPTY_MARGIN_Y, EMPTY_MARGIN_Y+wh.y);
    cover += a;
    return wh.y + EMPTY_MARGIN_Y*2;
}

void CommandEmpty::Draw()
{
    if (!valid) return;
    const double width  = parsed_label.getTextWidthHeight().x;
    const double height = parsed_label.getTextWidthHeight().y;
    MscLineAttr line;
    line.width.second = 3;
    line.corner.second = CORNER_ROUND;
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
