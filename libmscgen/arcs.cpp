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

    <parsing starts>

    1. Construction: Only basic initialization is done here. For arcs with style (descendants of ArcLabelled)
       we fetch the appropriate style from Msc::Contexts.back()
       We also look up entities the arc refers to (and create them if needed), so after this point
       we have EIterators pointing to the AllEntities list.
    2. AddAttributeList: Add attributes to arc. This function must be called (with NULL if no attributes)
       We have to do this before we can create a list of Active Entities, since attributes can 
       result in the implicit definition of new entities.
    3. Additional small functions (SetLineEnd, ArcEmphasis::SetPipe, CommandEntity::ApplyPrefix, etc.)

    <parsing ends>
    <here we construct a list of active entities from AllEntities to ActiveEntities>

    5. PostParseProcess: This is called recursive to do the following.
       a) Determine the non-specified entities for boxes. Note that the EIterators received as parameters
          are iterators of AllEntities and not ActiveEntities.
       b) Add numering to labels
       c) Determine which actual entities the arc refers to. In step #1 all entities of the arc point to 
          an entity in the Msc::AllEntities list. In this step we consider collapsed entities and search
          the corresponding entity in the ActiveEntities list. After this point all entities shall point 
          to the ActiveEntities list. We have to ensure that just because of collapsing entities, 
          automatic numbering does not change. (Some numbers are skipped - those that are assigned to
          invisible arcs, such as arc between entities not shown.)
       d) For boxes we check if the box is collapsed. If so, we replace content to CommandEmptyBox. 
          Here we have to ensure that automatic numbering of arrows do not change as for step 4c above.
          We also have to ensure that for auto-sizing entities (e.g., " .. : label { <content> }") we
          keep the size as would be the case for a non-collapsed box.
       e) Combine CommandEntities one after the other
       f) Set up some extra variables
       g) Print error messages. 
       This function can only be called once, as it changes arcs (e.g., you do not want to
       add numbering twice). Often the arc needs to be changed to a different one, in this case the
       return pointer shall be used. If the return pointer == this, the arc shall not be replaced.
    5. Width: This is also called recursively. Here each arc can place the distance requirements between
       entities. Using the data Msc::CalculateWithAndHeight() can place entities dynamically if hscale==auto.
       If hcale!=auto, entities have fixed positions, but this function is still called (so as it can be used
       to calculate cached values).

    <here we calculate the Entity::pos for all entities in ActiveEntities>

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
    const MscStyle *refinement = GetRefinementStyle(type);
    if (refinement) style += *refinement;
}

const MscStyle *ArcLabelled::GetRefinementStyle(MscArcType t) const
{
    switch(t) {
    case MSC_ARC_SOLID:
    case MSC_ARC_SOLID_BIDIR:
        return &chart->Contexts.back().styles["->"];
    case MSC_ARC_DOTTED:
    case MSC_ARC_DOTTED_BIDIR:
        return &chart->Contexts.back().styles[">"];
    case MSC_ARC_DASHED:
    case MSC_ARC_DASHED_BIDIR:
        return &chart->Contexts.back().styles[">>"];
    case MSC_ARC_DOUBLE:
    case MSC_ARC_DOUBLE_BIDIR:
        return &chart->Contexts.back().styles["=>"];
    case MSC_ARC_UNDETERMINED_SEGMENT:
        return NULL; /*Do nothing */
    case MSC_EMPH_SOLID:
        return &chart->Contexts.back().styles["--"];
    case MSC_EMPH_DASHED:
        return &chart->Contexts.back().styles["++"];
    case MSC_EMPH_DOTTED:
        return &chart->Contexts.back().styles[".."];
    case MSC_EMPH_DOUBLE:
        return &chart->Contexts.back().styles["=="];
    case MSC_EMPH_UNDETERMINED_FOLLOW:
        return NULL; /*do nothing*/
    case MSC_ARC_DIVIDER:
        return &chart->Contexts.back().styles["---"];
    case MSC_ARC_DISCO:
        return &chart->Contexts.back().styles["..."];
    };
    return NULL;
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

//This assigns a running number to the label and 
//fills the "compress" member from the style.
//Strictly to be called by descendants
ArcBase *ArcLabelled::PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level)
{
    if (!valid) return NULL;
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
    return this;
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

ArcArrow * ArcSelfArrow::AddSegment(MscArcType t, const char *m, file_line_range ml, file_line_range l)
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

ArcBase* ArcSelfArrow::PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level)
{
    if (!valid) return NULL;
    if (chart->ErrorIfEntityGrouped(src, (*src)->file_pos)) return NULL;

    //Add numbering, if needed
    ArcLabelled::PostParseProcess(left, right, number, top_level);

    EIterator substitute = chart->FindActiveParentEntity(src);
    if (src != substitute) return NULL; //src is not visible -> we disappear, too

    left = chart->EntityMinByPos(left, src);
    right = chart->EntityMaxByPos(right, src);

    //update src to point to ActiveEntities
    src = chart->ActiveEntities.Find_by_Ptr(*src);
    _ASSERT(src != chart->ActiveEntities.end());
    return this;
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
                         const char *d, file_line_range dl, Msc *msc, bool fw, const MscStyle &st) :
    ArcArrow(t, msc, st), linenum_src(sl.start), linenum_dst(dl.start), specified_as_forward(fw)
{
    src = chart->FindAllocEntity(s, sl, &valid);
    dst = chart->FindAllocEntity(d, dl, &valid);
    modifyFirstLineSpacing = true;
    segment_types.push_back(t);
};

ArcArrow * ArcDirArrow::AddSegment(MscArcType t, const char *m, file_line_range ml, file_line_range l)
{
    if (!valid) return this;
    EIterator mid;
    if (m==NULL) {
        if (((*src)->pos < (*dst)->pos) ^ specified_as_forward)
            // b->a-> and b<-a<-
            mid = chart->FindAllocEntity(LSIDE_ENT_STR, ml);
        else
            // a->b-> and a<-b<-
            mid = chart->FindAllocEntity(RSIDE_ENT_STR, ml);
    } else
        mid = chart->FindAllocEntity(m, ml);
    _ASSERT(*mid != chart->NoEntity);
    if (specified_as_forward) {
        //check for this situation: ->b->a (where a is left of b)
        if (middle.size()==0 && (*src)->name ==LSIDE_ENT_STR &&
            (*dst)->pos > (*mid)->pos && m!=NULL)
            src = chart->FindAllocEntity(RSIDE_ENT_STR, ml);
        middle.push_back(dst);
        linenum_middle.push_back(linenum_dst);
        dst = mid;
        linenum_dst = ml.start;
        segment_types.push_back(t);
    } else {
        //check for this situation: <-b<-a (where a is left of b)
        if (middle.size()==0 && (*dst)->name ==LSIDE_ENT_STR &&
            (*src)->pos > (*mid)->pos && m!=NULL)
            dst = chart->FindAllocEntity(RSIDE_ENT_STR, ml);
        middle.insert(middle.begin(), src);
        linenum_middle.insert(linenum_middle.begin(), linenum_src);
        src = mid;
        linenum_src = ml.start;
        segment_types.insert(segment_types.begin(), t);
    };
    return this;
}

ArcBase *ArcDirArrow::AddAttributeList(AttributeList *l)
{
    //Save the style, empty it, collect all modifications and apply those to segments, too
    MscStyle save(style);
    style.Empty();
    ArcArrow::AddAttributeList(l);
    save += style;
    for (int i=0; i<segment_types.size(); i++) {
        const MscStyle *refinement = GetRefinementStyle(segment_types[i]);
        if (refinement) {
            segment_lines.push_back(chart->Contexts.back().styles["arrow"].line);
            *segment_lines.rbegin() += refinement->line;
            *segment_lines.rbegin() += style.line;
        } else {
            segment_lines.push_back(save.line);
        }
    }
    style = save;
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

ArcBase *ArcDirArrow::PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level)
{
    if (!valid) return NULL;
    bool error = false;
    error |= chart->ErrorIfEntityGrouped(src, file_pos.start);
    error |= chart->ErrorIfEntityGrouped(dst, file_pos.start);
    for (auto i = middle.begin(); i!=middle.end(); i++)
        error |= chart->ErrorIfEntityGrouped(*i, file_pos.start);
    if (error) return NULL;

    //Add numbering, if needed
    ArcLabelled::PostParseProcess(left, right, number, top_level);

    //OK, now we check if arrow will display correctly
    if (middle.size()) {
        const bool dir = (*src)->pos < (*dst)->pos; //true for ->, false for <-
        if (dir != ((*src)->pos < ((*middle[0])->pos))) goto problem;
        for (unsigned i=0; i<middle.size()-1; i++)
            if (dir != ((*middle[i])->pos < (*middle[i+1])->pos)) goto problem;
        if (dir != (((*middle[middle.size()-1])->pos < (*dst)->pos))) goto problem;
        goto no_problem;

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
        return NULL; //Remove this arrow.
    }
    no_problem:

    //Update left and right 
    const EIterator _left = ++chart->AllEntities.begin(); // leftmost entity;
    const EIterator _right = --chart->AllEntities.end();  // rightmost entiry
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

    //Update src, dst and mid
    EIterator sub;
    sub = chart->FindActiveParentEntity(src);
    if (sub!=src) {
        src = chart->ActiveEntities.Find_by_Ptr(*sub);
        _ASSERT(src != chart->ActiveEntities.end());
    }
    sub = chart->FindActiveParentEntity(dst);
    if (sub!=dst) {
        dst = chart->ActiveEntities.Find_by_Ptr(*sub);
        _ASSERT(dst != chart->ActiveEntities.end());
    }
    if (src == dst) return NULL; //We became a degenerate arrow, do not show us

    //Find the visible parent of each middle point and remove it if equals to
    //an end or to any other middle visible parent
    for (int ii = 0; ii<middle.size(); ii++) {
        sub = chart->FindActiveParentEntity(middle[ii]);
        if (middle[ii] == sub) continue;
        //if the replacement parent equals to an end, delete it
        if (sub == src || sub == dst) {
            erase:
            middle.erase(middle.begin()+ii);
            ii++;
            continue;
        }
        for (int jj=0; jj<ii; jj++) 
            if (middle[jj] == sub) goto erase;
        middle[ii] = sub;
    }
    //Replace middle[] values to point to ActiveEntities
    for (int iii = 0; iii<middle.size(); iii++) {
        middle[iii] = chart->ActiveEntities.Find_by_Ptr(*middle[iii]);
        _ASSERT(middle[iii] != chart->ActiveEntities.end());
    }

    //Insert a small extra spacing for the arrow line
    if (parsed_label.getTextWidthHeight().y && modifyFirstLineSpacing)
        parsed_label.AddSpacing(0, style.line.LineWidth()+
                                ARROW_TEXT_VSPACE_ABOVE+ARROW_TEXT_VSPACE_BELOW);
    return this;
}

void ArcDirArrow::Width(EntityDistanceMap &distances)
{
    if (!valid) return;
    //we lie about us being forward (we do not check), so we know which of first/second to use
    DoublePair end = style.arrow.getWidths(true, isBidir(), MSC_ARROW_END, style.line);
    DoublePair start = style.arrow.getWidths(true, isBidir(), MSC_ARROW_START, style.line);
    distances.Insert((*src)->index, (*dst)->index,
                     end.first + start.second + parsed_label.getTextWidthHeight().x);
    //Add distances for arrowheads
    const bool fw = (*src)->index  <  (*dst)->index;
    distances.Insert((*src)->index, fw ? DISTANCE_RIGHT : DISTANCE_LEFT, start.second);
    distances.Insert((*dst)->index, fw ? DISTANCE_LEFT : DISTANCE_RIGHT, end.first);

    if (middle.size()==0) return;
    EntityDistanceMap d;
    for (int i=0; i<middle.size(); i++) {
        DoublePair mid = style.arrow.getWidths(fw, isBidir(), MSC_ARROW_MIDDLE, style.line);
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

    double lw_max = style.line.LineWidth();
    for (int i=0; i<segment_lines.size(); i++)
        lw_max = std::max(lw_max, segment_lines[i].LineWidth());
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
        cx_text = (sx+dx)/2;
        area += text_cover = parsed_label.Cover(sx_text, dx_text, y, cx_text);
        //determine top edge position of arrow midline
        y += std::max(aH, firstLineHeight+ARROW_TEXT_VSPACE_ABOVE);
    } else {
        /* no text */
        y += aH;
    }
    centerline = y = ceil(y) + lw_max/2;
    //prepare xPos and margins
    xPos.clear(); xPos.reserve(2+middle.size());
    margins.clear(); margins.reserve(2+middle.size());
    xPos.push_back(sx);
    margins.push_back(style.arrow.getWidths(sx<dx, isBidir(), MSC_ARROW_START, style.line));
    for (int i=0; i<middle.size(); i++) {
        xPos.push_back(chart->XCoord(middle[i]));
        margins.push_back(style.arrow.getWidths(sx<dx, isBidir(), MSC_ARROW_MIDDLE, style.line));
    }
    xPos.push_back(dx);
    margins.push_back(style.arrow.getWidths(sx<dx, isBidir(), MSC_ARROW_END, style.line));
    if (sx>=dx) {
        std::reverse(xPos.begin(), xPos.end());
        std::reverse(margins.begin(), margins.end());
        std::reverse(segment_lines.begin(), segment_lines.end());
        std::reverse(segment_types.begin(), segment_types.end());
    }
    //prepare clip_area
    Block total(sx, dx, 0, y+lw_max);
    clip_area  = style.arrow.ClipForLine(XY(sx, y), sx<dx, isBidir(), MSC_ARROW_START,
                                         total, *segment_lines.begin(), *segment_lines.begin());
    clip_area *= style.arrow.ClipForLine(XY(dx, y), sx<dx, isBidir(), MSC_ARROW_END,
                                         total, *segment_lines.rbegin(), *segment_lines.rbegin());
    for (unsigned i=0; i<middle.size(); i++)
        clip_area *= style.arrow.ClipForLine(XY(chart->XCoord(middle[i]), y), sx<dx, isBidir(), MSC_ARROW_MIDDLE,
                                             total, segment_lines[i], segment_lines[i+1]);

    //Add arrowheads and line segments to cover
    for (int i=0; i<xPos.size(); i++)
        area += style.arrow.Cover(XY(xPos[i], y), sx<dx, isBidir(), WhichArrow(i));
    for (int i=0; i<xPos.size()-1; i++) {
        const double lw2 = ceil(segment_lines[i].LineWidth()/2);
        //x coordinates below are not integer- but this will be merged with other contours - so they disappear
        area += clip_area * Block(xPos[i]+margins[i].second, xPos[i+1]-margins[i+1].first, y-lw2, y+lw2);
    }
    lw_max = std::max(lw_max, chart->nudgeSize+1.0);
    //set mainline - not much dependent on main line with
    area.mainline = Range(y - lw_max/2, y + lw_max/2);
    cover = area;
    return std::max(y+max(aH, lw_max/2), chart->arcVGapAbove + text_wh.y) + chart->arcVGapBelow;
}

void ArcDirArrow::ShiftBy(double y)
{
    if (!valid) return;
    if (y) {
        text_cover.Shift(XY(0, y));
        clip_area.Shift(XY(0, y));
    }
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
    chart->HideEntityLines(style.arrow.EntityLineCover(XY(sx, yPos+centerline), sx<dx, isBidir(), MSC_ARROW_START));
    chart->HideEntityLines(style.arrow.EntityLineCover(XY(dx, yPos+centerline), sx<dx, isBidir(), MSC_ARROW_END));
    //for multi-segment arrows
    for (unsigned i=0; i<middle.size(); i++) {
        const double mx = chart->XCoord(middle[i]);
        chart->HideEntityLines(style.arrow.EntityLineCover(XY(mx, yPos+centerline), sx<dx, isBidir(), MSC_ARROW_MIDDLE));
    }
}

void ArcDirArrow::Draw()
{
    if (!valid) return;
    if (parsed_label.getTextWidthHeight().y)
        parsed_label.Draw(sx_text, dx_text, yPos + chart->arcVGapAbove, cx_text);
    /* Draw the line */
    //all the entities this (potentially multi-segment arrow visits)
    const double y = yPos+centerline;  //should be integer
    chart->Clip(clip_area);
    for (unsigned i=0; i<xPos.size()-1; i++)
        chart->Line(XY(xPos[i], y), XY(xPos[i+1], y), segment_lines[i]);
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
    const MscStyle *refinement = GetRefinementStyle(type);
    if (refinement)
        style += *refinement;
    modifyFirstLineSpacing = false;
}

const MscStyle *ArcBigArrow::GetRefinementStyle(MscArcType t) const
{
    switch(t) {
    case MSC_ARC_SOLID:
    case MSC_ARC_SOLID_BIDIR:
        return &chart->Contexts.back().styles["block->"];
    case MSC_ARC_DOTTED:
    case MSC_ARC_DOTTED_BIDIR:
        return &chart->Contexts.back().styles["block>"];
    case MSC_ARC_DASHED:
    case MSC_ARC_DASHED_BIDIR:
        return &chart->Contexts.back().styles["block>>"];
    case MSC_ARC_DOUBLE:
    case MSC_ARC_DOUBLE_BIDIR:
        return &chart->Contexts.back().styles["block=>"];
    }
    return NULL;
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

ArcBase* ArcBigArrow::PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level)
{
    if (!valid) return NULL;
    //Determine src and dst entity, check validity of multi-segment ones, add numbering, etc
    ArcBase *ret = ArcDirArrow::PostParseProcess(left, right, number, top_level);
    //Finally copy the line attribute to the arrow, as well (arrow.line.* attributes are annulled here)
    style.arrow.line = style.line;
    return ret;
}

void ArcBigArrow::Width(EntityDistanceMap &distances)
{
    if (!valid) return;
    //Set sy and dy
    //sy and dy are at the midline of the line around the body.
    //We ensure that the outer edge of the body falls on an integer value
    double max_lw = style.line.LineWidth();
    for (int i=0; i<segment_lines.size(); i++)
        max_lw = std::max(max_lw, segment_lines[i].LineWidth());
    const double aH = ceil(style.arrow.bigYExtent(isBidir(), middle.size()>0));
    XY twh = parsed_label.getTextWidthHeight();
    if (twh.y == 0)
        twh.y = Label("M", chart, style.text).getTextWidthHeight().y;
    sy = chart->arcVGapAbove + aH + max_lw/2;
    dy = ceil(sy + max_lw/2 + twh.y + chart->emphVGapInside*2 + max_lw) - max_lw/2;

    Area tcov = parsed_label.Cover(0, twh.x, max_lw/2);
    const bool fw = (*src)->index < (*dst)->index;
    const DoublePair start = style.arrow.getBigWidthsForSpace(fw, isBidir(), MSC_ARROW_START, dy-sy);
    const DoublePair end   = style.arrow.getBigWidthsForSpace(fw, isBidir(), MSC_ARROW_END, dy-sy);

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
        margins.push_back(style.arrow.getBigWidthsForSpace(fw, isBidir(), MSC_ARROW_MIDDLE, dy-sy));
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
    //calculate text margin. segment_lines is now ordered from smaller x to larger x
    sm = style.arrow.getBigMargin(tcov, 0, dy-sy, true, fw, isBidir(), WhichArrow(stext)) +
         segment_lines[stext].LineWidth();
    dm = style.arrow.getBigMargin(tcov, 0, dy-sy, false, fw, isBidir(), WhichArrow(dtext)) +
         segment_lines[dtext-1].LineWidth();
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
        cx_text = (xPos[stext] + xPos[dtext])/2;
    } else {
        sx_text = xPos[xPos.size()-1-stext] + sm;
        dx_text = xPos[xPos.size()-1-dtext] - dm;
        cx_text = (xPos[xPos.size()-1-stext] + xPos[xPos.size()-1-dtext])/2;
    }
    //use += to keep arc and other params of area
    area = style.arrow.BigCover(xPos, sy, dy, isBidir());
    area.arc = this;
    //set mainline - not much dependent on main line with
    area.mainline = Range(centerline - chart->nudgeSize/2, centerline + chart->nudgeSize/2);
    cover = area;
    label_cover = parsed_label.Cover(sx_text, dx_text, sy+style.line.LineWidth()/2 + chart->emphVGapInside, cx_text);
    return centerline*2 - chart->arcVGapAbove + chart->arcVGapBelow + style.shadow.offset.second;
}

void ArcBigArrow::ShiftBy(double y)
{
    if (!valid) return;
    sy += y;
    dy += y;
    label_cover.Shift(XY(0,y));
    ArcArrow::ShiftBy(y); //Skip ArcDirArrow
}

void ArcBigArrow::PostPosProcess(double autoMarker)
{
    if (!valid) return;
    CheckSegmentOrder(yPos + centerline);
    if (!valid) return;
    ArcArrow::PostPosProcess(autoMarker); //Skip ArcDirArrow
    chart->HideEntityLines(style.arrow.BigEntityLineCover(xPos, sy, dy, isBidir(), &segment_lines,
        Block(XY(0,0), chart->total)));
}

void ArcBigArrow::Draw()
{
    if (!valid) return;
    style.arrow.BigDraw(xPos, sy, dy, isBidir(), style.shadow, style.fill, &segment_lines, chart, &label_cover);
    parsed_label.Draw(sx_text, dx_text, sy+style.line.LineWidth()/2 + chart->emphVGapInside, cx_text);
}

//////////////////////////////////////////////////////////////////////////////////////

VertXPos::VertXPos(Msc&m, const char *e1, file_line_range e1l,
                   const char *e2, file_line_range e2l, postype p)
{
    valid = true;
    pos = p;
    entity1 = m.FindAllocEntity(e1, e1l, &valid);
    if (pos == POS_CENTER) entity2 = m.FindAllocEntity(e2, e2l, &valid);
    else {
        entity2 = m.AllEntities.Find_by_Ptr(m.NoEntity);
        _ASSERT(entity2 != m.AllEntities.end());
    }
}

VertXPos::VertXPos(Msc&m, const char *e1, file_line_range e1l, postype p)
{
    valid = true;
    pos = p;
    entity1 = m.FindAllocEntity(e1, e1l, &valid);
    entity2 = m.AllEntities.Find_by_Ptr(m.NoEntity);
    _ASSERT(entity2 != m.AllEntities.end());
}

VertXPos::VertXPos(Msc&m, postype p)
{
    valid = true;
    entity1 = m.AllEntities.Find_by_Ptr(m.NoEntity);
    _ASSERT(entity1 != m.AllEntities.end());
    entity2 = m.AllEntities.Find_by_Ptr(m.NoEntity);
    _ASSERT(entity2 != m.AllEntities.end());
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


ArcArrow *ArcVerticalArrow::AddSegment(MscArcType t, const char *m, file_line_range ml, file_line_range l)
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
    MscStyle style(STYLE_DEFAULT, ArrowHead::BIGARROW, true, true, true, true, false, false, true, true, true);  //no vline solid
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
    MscStyle style(STYLE_DEFAULT, ArrowHead::BIGARROW, true, true, true, true, false, false, true, true, true);  //no vline solid
    if (style.AttributeValues(attr, csh)) return true;
    if (ArcLabelled::AttributeValues(attr, csh)) return true;
    return false;
}


ArcBase* ArcVerticalArrow::PostParseProcess(EIterator &left, EIterator &right,
                                        Numbering &number, bool top_level)
{
    if (!valid) return NULL;
    if (src == MARKER_HERE_STR || src == MARKER_PREV_PARALLEL_STR)
        if (dst == MARKER_HERE_STR || dst == MARKER_PREV_PARALLEL_STR)
            if (top_level) {
                chart->Error.Error(file_pos.start, "Need at least one marker specified."
                                   " Ignoring vertical arrow.",
                                   "Only verticals inside a parallel block can omit both markers.");
                valid = false;
                return NULL;
            }

    if (src != MARKER_HERE_STR && src != MARKER_PREV_PARALLEL_STR) {
        std::map<string, Msc::MarkerType>::const_iterator i = chart->Markers.find(src);
        if (i == chart->Markers.end()) {
            chart->Error.Error(file_pos.start, "Cannot find marker '" + src + "'."
                               " Ignoring vertical arrow.");
            valid=false;
            return NULL;
        }
    }

    if (dst != MARKER_HERE_STR && dst != MARKER_PREV_PARALLEL_STR) {
        std::map<string, Msc::MarkerType>::const_iterator i = chart->Markers.find(dst);
        if (i == chart->Markers.end()) {
            chart->Error.Error(file_pos.start, "Cannot find marker '" + dst + "'."
                               " Ignoring vertical arrow.");
            valid=false;
            return NULL;
        }
    }

    bool error = chart->ErrorIfEntityGrouped(pos.entity1, file_pos.start);
    error |= chart->ErrorIfEntityGrouped(pos.entity2, file_pos.start);
    if (error) return NULL;

    //Add numbering, if needed
    ArcLabelled::PostParseProcess(left, right, number, top_level);

    left = chart->EntityMinByPos(left, pos.entity1);
    right = chart->EntityMaxByPos(right, pos.entity1);
    left = chart->EntityMinByPos(left, pos.entity2);
    right = chart->EntityMaxByPos(right, pos.entity2);

    //Now change entities in vertxPos to point to ActiveEntities
    pos.entity1 = chart->ActiveEntities.Find_by_Ptr(*chart->FindActiveParentEntity(pos.entity1));
    pos.entity2 = chart->ActiveEntities.Find_by_Ptr(*chart->FindActiveParentEntity(pos.entity2));
    _ASSERT(pos.entity1 != chart->ActiveEntities.end());
    _ASSERT(pos.entity2 != chart->ActiveEntities.end());
    if (pos.pos == VertXPos::POS_CENTER && pos.entity1 == pos.entity2)
        pos.pos = VertXPos::POS_AT;

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
    return this;
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
    const Area lab = parsed_label.Cover(min(ypos[0], ypos[1]), max(ypos[0], ypos[1]),
                                        xpos-width/2+style.line.LineWidth()/2+chart->emphVGapInside,
                                        -1, true);
    style.arrow.BigDraw(ypos, xpos-width/2, xpos+width/2, isBidir(), style.shadow, style.fill,
                        NULL, chart, &lab,
                        style.side.second==SIDE_RIGHT, style.side.second==SIDE_LEFT);
    parsed_label.Draw(min(ypos[0], ypos[1]), max(ypos[0], ypos[1]),
                      xpos-width/2+style.line.LineWidth()/2+chart->emphVGapInside, -1, true);
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
    if (*src!=chart->NoEntity && *dst!=chart->NoEntity)
        if ((*src)->pos > (*dst)->pos) 
            std::swap(src, dst);
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
ArcBase* ArcEmphasis::PostParseProcess(EIterator &left, EIterator &right,
                                       Numbering &number, bool top_level)
{
    if (!valid) return NULL;
    //Add ourselves as the first element in follow.
    follow.push_front(this);

    EIterator e1=src;      //src.pos < dst.pos guaranteed in constructor (if none is NULL)
    EIterator e2=dst;
    //OK, now expand further if there are following boxes
    for (auto i = follow.begin(); i!=follow.end(); i++) {
        //Add numbering, if needed
        (*i)->ArcLabelled::PostParseProcess(left, right, number, top_level);
        if ((*i)->content) {
            //If we have attached a number to the label of a box, set flag on number,
            //so that if we add levels inside the content of the box (before displaying any number)
            //we continue from present value
            if ((*i)->style.numbering.second && (*i)->label.length()!=0)
                number.decrementOnAddingLevels = true;
            chart->PostParseProcessArcList(*((*i)->content), false, e1, e2, number, top_level);
            //Check if content has disappeared (entity collapsing may cause this)
            if ((*i)->content->size() == 0) {
                delete (*i)->content;
                (*i)->content = NULL;
            } 
            number.decrementOnAddingLevels = false;
        }
    }

    if (!pipe) { /* We are a box */
        if (*src==chart->NoEntity) src = e1;
        if (*dst==chart->NoEntity) dst = e2;

        //Src and dst can still be == NoEntity, if no arcs specified
        //inside the content and no enity specified at emph declaration.
        //In this case emph box spans to leftmost and rightmost entity in chart.
        //At PostParse "Entities" is already sorted by pos values
        //we only do this step if we are the first in an Emphasis box series.
        if (*src==chart->NoEntity) src = ++ ++chart->AllEntities.begin();  //leftmost entity after Noentity and (left)
        if (*dst==chart->NoEntity) dst = -- --chart->AllEntities.end();    //rightmost entity (before (right)

        //Now convert src and dst to an iterator pointing to ActiveEntities
        EIterator sub1 = chart->FindActiveParentEntity(src);
        EIterator sub2 = chart->FindActiveParentEntity(dst);
        //if src (or dst) is visible, look up their leftmost/rightmost children (if any)
        if (sub1 == src) sub1 = chart->FindLeftRightmostChildren(sub1, true);
        if (sub2 == dst) sub2 = chart->FindLeftRightmostChildren(sub2, false);

        //if pipe segment spans a single entity and both ends have changed, 
        //we kill this box but not content
        if (sub1==sub2 && sub1!=src && sub2!=dst) {
            ArcList *al = new ArcList;
            for (auto i = follow.begin(); i!=follow.end(); i++) 
                al->Append((*i)->content); //NULL is handled, content is emptied
            ArcParallel *p = new ArcParallel(chart);
            p->AddArcList(al); //ownership of "al" is taken
            return p;
        } 
        src = chart->ActiveEntities.Find_by_Ptr(*sub1); 
        dst = chart->ActiveEntities.Find_by_Ptr(*sub2);
        _ASSERT(src != chart->ActiveEntities.end()); 
        _ASSERT(dst != chart->ActiveEntities.end()); 

        left = chart->EntityMinByPos(chart->EntityMinByPos(left, src), dst);
        right = chart->EntityMaxByPos(chart->EntityMaxByPos(right, src), dst);
        return this;
    }

    /* We are pipe */
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
    EIterator last = e1;
    EIterator next;
    for (auto i = follow.begin(); i!=follow.end(); i++) {
        auto i_next = i;
        i_next++;
        next = i_next==follow.end() ? e2 : chart->EntityMinByPos((*i_next)->src, (*i_next)->dst);

        if (*(*i)->src == chart->NoEntity) (*i)->src = last;
        if (*(*i)->dst == chart->NoEntity) (*i)->dst = next;

        if (*(*i)->src == chart->NoEntity || *(*i)->dst == chart->NoEntity) {
            std::string msg = "Could not figure out the extent of this pipe";
            if (follow.size()>1) msg += " segment";
            msg += ". Specify starting and/or terminating entity.";
            chart->Error.Error((*i)->file_pos.start, msg);
            valid = false;
            return NULL;
        }
        if ((*i)->src != chart->EntityMinByPos((*i)->src, (*i)->dst))
            swap((*i)->src, (*i)->dst);
        last = (*i)->dst;

    }

    //Now check that segments sanity
    for (auto i = follow.begin(); i!=follow.end(); i++) {
        auto i_prev = i, i_next = i;
        if (i!=follow.begin()) i_prev--; 
        if (i!=--follow.end()) i_next++;
        const EIterator loc_src = chart->FindLeftRightmostChildren((*i)->src, true);
        const EIterator loc_dst = chart->FindLeftRightmostChildren((*i)->dst, false);
        const EIterator next_src = chart->FindLeftRightmostChildren(i_next!=follow.end() ? (*i_next)->src : (*i)->src, true);
        const EIterator prev_dst = chart->FindLeftRightmostChildren(i_prev!=follow.end() ? (*i_prev)->dst : (*i)->dst, false);

        if (loc_src == loc_dst && (
              (i_prev!=follow.end() && prev_dst != loc_src) ||
              (i_next!=follow.end() && next_src != loc_dst)
              )) {
            chart->Error.Error((*i)->file_pos.start, "This pipe segment is attaches to a neighbouring segments but spans only a single entity."
                " Segment cannot be shown. Ignoring pipe.");
            valid = false;
            return NULL;
        }
        if ((i_prev!=follow.end() && chart->EntityMaxByPos(prev_dst, loc_src) != loc_src) ||
            (i_next!=follow.end() && chart->EntityMinByPos(next_src, loc_dst) != loc_dst))
                chart->Error.Warning((*i)->file_pos.start, "This pipe segment overlaps a negighbouring one."
                " It may not look so good.",
                "Encapsulate one in the other if you want that effect.");
    }
    //All the above operations were checked on AllEntities. We have accepted the pipe
    //as valid here and should not complain no matter what entities are collapsed or not

    bool killed_this = false;
    //Now change src:s and dst:s to active entities.
    //First look up their active parent in AllEntities (or if they are
    //visible group entities find, the left/right-most children)
    //Then convert to ActiveEntities iterators
    //If may happen that a pipe segment disappears.
    //If the first one disappears (==this), we need to do a replacement
    //If all disappear, we just return the content in an ArcParallel
    //Do one pass of checking
    for (auto i = follow.begin(); i!=follow.end(); /*none*/) {
        EIterator sub1 = chart->FindActiveParentEntity((*i)->src);
        EIterator sub2 = chart->FindActiveParentEntity((*i)->dst);
        //if src (or dst) is visible, look up their leftmost/rightmost children (if any)
        if (sub1 == (*i)->src) sub1 = chart->FindLeftRightmostChildren(sub1, true);
        if (sub2 == (*i)->dst) sub2 = chart->FindLeftRightmostChildren(sub2, true);

        //if pipe segment spans a single entity and both ends have changed, 
        //we kill this segment
        if (sub1==sub2 && sub1!=(*i)->src && sub2!=(*i)->dst) {
            if (*i != this) delete *i;
            else killed_this = true;
            follow.erase(i++);
        } else {
            (*i)->src = chart->ActiveEntities.Find_by_Ptr(*sub1); 
            (*i)->dst = chart->ActiveEntities.Find_by_Ptr(*sub2);
            _ASSERT((*i)->src != chart->ActiveEntities.end()); 
            _ASSERT((*i)->dst != chart->ActiveEntities.end()); 
            i++;
        }
    }
    //Do second pass: a single entity segment is kept only if it does not 
    //connect to the previous or next segments
    //Also collect thickest linewidth of remaining segments
    double lw_max = 0;
    for (auto i = follow.begin(); i!=follow.end(); i++) {
        if ((*i)->src == (*i)->dst) {
            auto j = i;
            auto k = i;
            if ((i!=follow.begin() && (*--j)->dst == (*i)->src) ||
                (i!=--follow.end() && (*++k)->src == (*i)->dst)) {
                    if (*i != this) delete *i;
                    else killed_this = true;
                    follow.erase(i++);
                    i--;
                    continue;
            }
        }
        lw_max = std::max(lw_max, (*i)->style.line.LineWidth());
    }
    
    //see if we have any segments left, if not return only content
    if (follow.size()==0) {
        if (content && content->size()) {
            ArcParallel *p = new ArcParallel(chart);
            p->AddArcList(content);
            return p;
        }
        return NULL;
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
            (*i)->pipe_connect_back = false;
            (*i_prev)->pipe_connect_forw = false;
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

    //if we have killed "this" copy "follow", "content" to first guy and replace us
    if (killed_this) {
        ArcEmphasis *ret = *follow.begin();
        ret->follow = follow;
        follow.clear();
        ret->first = NULL;
        ret->content = content;
        if (content) {
            content->clear();
            delete content;
            content = NULL;
        }
        return ret;
    }
    return this;
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
                cover += (*i)->pipe_shadow + (*i)->pipe_shadow.CreateShifted(XY(style.shadow.offset.second, style.shadow.offset.second));
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
            cover += overall_box + overall_box.CreateShifted(XY(style.shadow.offset.second, style.shadow.offset.second));
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
        const double lw = style.line.LineWidth();
        const double sx = chart->XCoord(src) - left_space + lw;
        const double dx = chart->XCoord(dst) + right_space - lw;
        for (auto i = ++follow.begin(); i!=follow.end(); i++) {
            chart->HideEntityLines((*i)->text_cover);
            if ((*i)->style.line.IsDoubleOrTriple()) {
                const Block r(sx, dx, (*i)->yPos, (*i)->yPos+(*i)->style.line.LineWidth());
                chart->HideEntityLines(r);
            }
        }
        //hide the entity lines under the label of the first segment (not handled above)
        chart->HideEntityLines(text_cover);
        //hide top and bottom line if double
        if (style.line.IsDoubleOrTriple()) {
            const double lw = style.line.LineWidth();
            Block b(chart->XCoord(src) - left_space, chart->XCoord(dst) + right_space,
                    yPos, yPos+total_height); //The outer edge of the lines
            MscLineAttr line = style.line;  //We will vary the radius, so we need a copy
            line.radius.second = std::min(std::min(b.y.Spans()/2 - lw, b.x.Spans()/2 - lw), line.radius.second);
            //The radius specified in style.line will be that of the inner edge of the line
            if (line.radius.second > 0)
                line.radius.second += lw * line.RadiusIncMul(); //now it is appropriate for the outer edge
            Area hide = line.CreateRectangle(b);
            line.radius.second -= lw * line.RadiusIncMul(); //now it is appropriate for the inner edge
            hide -= line.CreateRectangle(b.Expand(-lw));

            if (style.line.corner.second == CORNER_NOTE) {
                const double r = style.line.radius.second + style.line.RadiusIncMul()*lw/2 -
                                 (style.line.IsDouble() ? style.line.DoubleSpacing() : 0);
                hide += Contour(b.UpperRight()+XY(0,r), b.UpperRight()+XY(-r,0), b.UpperRight()+XY(-r,r));
            }
            chart->HideEntityLines(hide);
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
    nudge(t==MSC_COMMAND_NUDGE), extra_space(t==MSC_ARC_DISCO ? msc->discoVgap : 0),
    wide(false)
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

ArcBase* ArcDivider::PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level)
{
    if (!valid) return NULL;
    //Add numbering, if needed
    ArcLabelled::PostParseProcess(left, right, number, top_level);

    if (!top_level && (type==MSC_ARC_DISCO || type==MSC_ARC_DIVIDER)) {
        string ss;
        ss << (type==MSC_ARC_DISCO ? "'...'" : "'---'") << " is specified inside a parallel block.";
        chart->Error.Warning(file_pos.start, ss, "May display incorrectly.");
    }
    return this;
}

void ArcDivider::Width(EntityDistanceMap &distances)
{
    if (!valid) return;
    if (nudge || !valid || parsed_label.getTextWidthHeight().y==0)
        return;
    const unsigned lside_index = (*chart->ActiveEntities.Find_by_Name(LSIDE_ENT_STR))->index;
    const unsigned rside_index = (*chart->ActiveEntities.Find_by_Name(RSIDE_ENT_STR))->index;
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
        wh.y += chart->arcVGapBelow;
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
        for(EIterator i = chart->ActiveEntities.begin(); i!=chart->ActiveEntities.end(); i++)
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

ArcBase* ArcParallel::PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level)
{
    if (!valid) return NULL;
    at_top_level = top_level;
    for (PtrList<ArcList>::iterator i=blocks.begin(); i != blocks.end(); i++)
        chart->PostParseProcessArcList(**i, false, left, right, number, false);
    return this;
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

CommandEntity *CommandEntity::ApplyPrefix(const char *prefix)
{
    for (auto i=entities.begin(); i!=entities.end(); i++) {
		if (CaseInsensitiveEqual(prefix, "show") || CaseInsensitiveEqual(prefix, "hide")) {
			if ((*i)->show_is_explicit) continue;	
			(*i)->show.first = true;	
			(*i)->show.second = CaseInsensitiveEqual(prefix, "show");
            (*i)->show_is_explicit = true; //prefix of a grouped entity will not impact inner ones with prefix
		}
    }
    return this;
}

void CommandEntity::ApplyShowToChildren(const string &name, bool show)
{
    EIterator j_ent = chart->AllEntities.Find_by_Name(name);
    if ((*j_ent)->children_names.size()==0) {
        for (auto i_def = entities.begin(); i_def != entities.end(); i_def++) 
            if ((*i_def)->name == name) {
                (*i_def)->show.first = true;
                (*i_def)->show.second = show;
                return;
            }
        EntityDef *ed = new EntityDef(name.c_str(), chart);
        ed->AddAttribute(Attribute("show", show, file_line_range(), file_line_range(), NULL));
        entities.Append(ed);
    } else {
        for (auto s = (*j_ent)->children_names.begin(); s != (*j_ent)->children_names.end(); s++) 
            ApplyShowToChildren(*s, show);
    }
}

ArcBase* CommandEntity::PostParseProcess(EIterator &left, EIterator &right, Numbering &number,
                                     bool top_level)
{
    if (!valid) return NULL;
    at_top_level = top_level;
    if (full_heading && !top_level)
        chart->Error.Warning(file_pos.start, "The command 'heading' is specified "
                             "inside a parallel block. May display incorrectly.");
    set<Entity*> explicitly_listed;
    //Remove show on/off attribute from grouped entities
    for (auto i_def = entities.begin(); i_def != entities.end(); i_def++) {
        EIterator j_ent = chart->AllEntities.Find_by_Name((*i_def)->name);
        (*i_def)->itr = j_ent;
        //if the entity is a grouped entity with a show/hide attribute, 
        //add an entitydef to our list for those children, who have no entitidefs
        //yet in the list. For those children, who have, just set show attribute
        //if not set yet
        if ((*i_def)->show.first && (*j_ent)->children_names.size()) { 
            for (auto ss = (*j_ent)->children_names.begin(); ss!=(*j_ent)->children_names.end(); ss++) {
                auto ii_def = entities.begin();
                for (/*none*/ ; ii_def != entities.end(); ii_def++) 
                    if ((*ii_def)->name == *ss) {
                        if (!(*ii_def)->show.first) 
                            (*ii_def)->show = (*i_def)->show;
                        break;
                    }
                if (ii_def == entities.end()) {
                    EIterator jj_ent = chart->AllEntities.Find_by_Name((*i_def)->name);
                    EntityDef *ed = new EntityDef(ss->c_str(), chart);
                    ed->show = (*i_def)->show;
                    ed->implicit = true;
                    ed->itr = jj_ent;
                    ed->style = (*jj_ent)->running_style;
                    ed->parsed_label.Set((*jj_ent)->label, ed->style.text);
                    entities.Append(ed);
                }
            }
            (*i_def)->show.first = false;
        }
    }
    //Next apply the style changes of this command to the running style of the entities
    for (auto i_def = entities.begin(); i_def != entities.end(); i_def++) {
        EIterator j_ent = (*i_def)->itr;
        //Make the style of the entitydef fully specified using the accumulated style info in Entity
        (*j_ent)->running_style += (*i_def)->style;  //(*i)->style is a partial style here specified by the user
    }
    //Now remove grouped entities (we have handled style and show for them)
    //and calculate shown status for non-grouped ones
    for (auto i_def = entities.begin(); i_def != entities.end(); i_def++) 
        if ((*(*i_def)->itr)->children_names.size()) {
            entities.erase(i_def++);
            i_def--;
        } else {
            EIterator j_ent = (*i_def)->itr;
            //Decide, if this entitydef will show an entity or not
            //It can get drawn because we 1) said show=yes, or
            //2) because it is on, we mention it (without show=yes) and it is
            //a full heading.
            (*i_def)->shown = ((*i_def)->show.second && (*i_def)->show.first) || (full_heading && (*j_ent)->shown);
            //Adjust the running status of the entity, this is valid *after* this command. 
            //This is just for the Height process knwos whch entity is on/off
            if ((*i_def)->show.first)
                (*j_ent)->shown = (*i_def)->show.second;
            //remove entitydef if not shown
            if (!(*i_def)->shown) {
                entities.erase(i_def++);
                i_def--;
            } else { 
                explicitly_listed.insert(*(*i_def)->itr);
                //Update the style of the entitydef
                (*i_def)->style = (*j_ent)->running_style;	 //(*i)->style now become the full style to use from this point
                (*i_def)->parsed_label.Set((*j_ent)->label, (*i_def)->style.text);
            }
        }

    //A "heading" command, we have to draw all entities that are on
    //for these we create additional EntityDefs and append them to entities
    //Only do this for children (non-grouped entities)
    if (full_heading)
        for (auto i = chart->AllEntities.begin(); i!=chart->AllEntities.end(); i++) {
            if (!(*i)->shown) continue;
            if ((*i)->children_names.size()) continue;
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
    //At this point the list contains all non-grouped entities with show=yes
    //Collapse is not considered yet, we will do it below

    //Now add parents to the list for those children that are visible
    //Maintain reverse ordering: parents later, children earlier in the list
    std::list<string> sl;
    for (auto i_def = entities.begin(); i_def != entities.end(); i_def++) {
        if (!(*i_def)->shown) continue;
        const string &myparent = (*(*i_def)->itr)->parent_name;
        if (myparent.length()==0) continue;
        if (std::find(sl.begin(), sl.end(), myparent) != sl.end()) continue;
        auto si = sl.begin();
        while (si != sl.end() && !chart->IsMyParentEntity(myparent, *si)) si++;
        sl.insert(si, myparent);
    }
    //Now add an entitydef for each parent (whith shown=yes) and reverse order
    for (auto i = sl.begin(); i!=sl.end(); i++) {
        EntityDef *e = new EntityDef(i->c_str(), chart);
        //fill in all values necessary
        e->implicit = true;
        e->itr = chart->AllEntities.Find_by_Name(*i);
        e->style = (*e->itr)->running_style;
        e->parsed_label.Set((*e->itr)->label, e->style.text);
        e->shown = true;
        entities.Prepend(e);
    }

    //Finally prune the list: remove those that shall not be displayed due to collapse
    for (auto i_def = entities.begin(); i_def != entities.end(); i_def++) 
        if (chart->FindActiveParentEntity((*i_def)->itr) != (*i_def)->itr) {
            entities.erase(i_def++);
            i_def--;
        }

    //Now we have all entities among "entities" that will show here
    //Go through them and update left, right and the entities' maxwidth
    for (auto i_def = entities.begin(); i_def != entities.end(); i_def++) {
        if (!(*i_def)->shown) continue;
        if ((*(*i_def)->itr)->children_names.size() && !(*(*i_def)->itr)->collapsed) {
            EIterator j_ent = (*i_def)->itr;
            while ((*j_ent)->children_names.size() && !(*j_ent)->collapsed) 
                j_ent = chart->FindLeftRightmostChildren(j_ent, true);
            left = chart->EntityMinByPos(left, j_ent);
            j_ent = (*i_def)->itr;
            while ((*j_ent)->children_names.size() && !(*j_ent)->collapsed) 
                j_ent = chart->FindLeftRightmostChildren(j_ent, false);
            right = chart->EntityMaxByPos(right, j_ent);
        } else {
            //for non-grouped or collapsed entities
            left = chart->EntityMinByPos(left, (*i_def)->itr);
            right = chart->EntityMaxByPos(right, (*i_def)->itr);
        }
        double w = (*i_def)->Width();
        if ((*(*i_def)->itr)->maxwidth < w) (*(*i_def)->itr)->maxwidth = w;
    }
    return this;
}

//Here we have EIterators in entitydefs that point to AllEntities (in contrast to
//all other arcs, where PostParseProcess will convert to iterators in AllActiveEntities)
//If an entity we list here is not collapsed and have children, then it will
//be drawn as containing other entities.
//Since parents are in the beginning of the list, we will go and add distances from the back
//and use the added distances later in the cycle when processing parents
void CommandEntity::Width(EntityDistanceMap &distances)
{
    //TODO: Fix here, to calculate width of grouped entities, too
    if (!valid) return;
    //Add distances for entity heading
    //Start by creating a map in which distances are ordered by index
    std::map<int, pair<double, double>> dist; //map the index of an active entity to spaces left & right
    //in PostParseProcess we created an entitydef for all entities shown here. 
    //"full_heading" not even checked here
    for (auto i = entities.rbegin(); !(i==entities.rend()); i++) {
        //Take entity height into account or draw it if show=on was added
        if (!(*i)->shown) continue;
        if ((*(*i)->itr)->children_names.size() == 0 || (*(*i)->itr)->collapsed) {
            const double halfsize = (*(*i)->itr)->maxwidth/2;
            const unsigned index = (*(*i)->itr)->index;
            dist[index] = pair<double, double>(halfsize, halfsize);
            (*i)->right_ent = (*i)->left_ent = (*i)->itr;
            (*i)->right_offset = (*i)->left_offset = 0;
        } else {
            //grouped entity, which is not collapsed
            //find leftmost and rightmost active entity 
            EIterator j_ent = (*i)->itr;
            while ((*j_ent)->children_names.size() && !(*j_ent)->collapsed) 
                j_ent = chart->FindLeftRightmostChildren(j_ent, true);
            (*i)->left_ent = j_ent;
            (*i)->left_offset = dist[(*j_ent)->index].first += 10; //Todo: Fix
            j_ent = (*i)->itr;
            while ((*j_ent)->children_names.size() && !(*j_ent)->collapsed) 
                j_ent = chart->FindLeftRightmostChildren(j_ent, false);
            (*i)->right_ent= j_ent;
            (*i)->right_offset = dist[(*j_ent)->index].second += 10; //Todo: Fix

            //Insert a requirement between left_ent and right_ent, so that our width will fit (e.g., long text)
            distances.Insert((*(*i)->left_ent)->index, (*(*i)->right_ent)->index,
                             (*(*i)->itr)->maxwidth - (*i)->left_offset - (*i)->right_offset);
        }
    }
    //Now convert neighbouring ones to box_side distances, and add the rest as normal side distance
    distances.Insert(dist.begin()->first,  DISTANCE_LEFT, dist.begin()->second.first); //leftmost distance
    distances.Insert(dist.rbegin()->first, DISTANCE_RIGHT,dist.rbegin()->second.second); //rightmost distance
    for (auto d = dist.begin(); d!=--dist.end(); d++) {
        auto d_next = d; d_next++;
        if (d->first == d_next->first-1) //neighbours
            distances.InsertBoxSide(d->first, d->second.second, d_next->second.first);
        else {
            distances.Insert(d->first, DISTANCE_RIGHT, d->second.second);
            distances.Insert(d_next->first, DISTANCE_LEFT, d_next->second.first);
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

    //We go backwards, so that contained entities get calculated first
    for (auto i = entities.rbegin(); !(i==entities.rend()); i++) {
        if (!(*i)->shown) continue;
        //Collect who is my children in this list
        EntityDefList edl(false);
        for (auto ii = entities.rbegin(); !(ii==entities.rend()); ii++) 
            if (*ii != *i && chart->IsMyParentEntity((*ii)->name, (*i)->name))
                edl.Append(*ii);
        const double h = (*i)->Height(cover, edl); //this also adds the cover to the entitydef's area
        if (height <h) height = h;
        if ((*i)->implicit)
            area += (*i)->GetAreaToSearch();
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
ArcBase* CommandNumbering::PostParseProcess(EIterator &left, EIterator &right, Numbering &number, bool top_level)
{
    if (!valid) return NULL;
    if ((action & SIZE) && length)
        number.SetSize(length);
    if (action & INCREMENT)
        ++number;
    if (action & DECREMENT)
        --number;
    return this;
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
    const unsigned lside_index = (*chart->ActiveEntities.Find_by_Name(LSIDE_ENT_STR))->index;
    const unsigned rside_index = (*chart->ActiveEntities.Find_by_Name(RSIDE_ENT_STR))->index;
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
