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
    3. Additional small functions (SetLineEnd, ArcBox::SetPipe, CommandEntity::ApplyPrefix, etc.)

    <parsing ends>
    <here we construct a list of active entities from AllEntities to ActiveEntities>

    5. PostParseProcess: This is called recursive to do the following.
       a) Determine the non-specified entities for boxes, pipes and arrows. Note that the EIterators 
          received as parameters ("left", "right") are iterators of AllEntities and not ActiveEntities.
       b) Add numering to labels
       c) Determine which actual entities the arc refers to. In step #1 all entities of the arc point to 
          an entity in the Msc::AllEntities list. In this step we consider collapsed entities and search
          the corresponding entity in the ActiveEntities list. After this point all entities shall point 
          to the ActiveEntities list. We have to ensure that just because of collapsing entities, 
          automatic numbering does not change. (Some numbers are skipped - those that are assigned to
          invisible arcs, such as arc between entities not shown.)
       d) For boxes we check if the box is collapsed. If so, we replace content to ArcIndicator. 
          Here we have to ensure that automatic numbering of arrows do not change as for step 4c above.
          We also have to ensure that for auto-sizing entities (e.g., " .. : label { <content> }") we
          keep the size as would be the case for a non-collapsed box. Also, content that does not show, but
          influence appearance (e.g., entity commands, page breaks, background changes, etc.) are 
          still kept as content and steps below (#6-9) shall be performed on them. See #h below.
       e) Combine CommandEntities and ArcIndicators one after the other
       f) Set up some extra variables
       g) Print error messages. 
       h) Decide on replacement. PostParseProcess have a "hide" parameter telling if this arc will be hidden 
          due to a collapsed box (and no other reason). In this case we return NULL, if the arc does not 
          want to receive #6-9 below. Else we return "this".
          An arc can also become hidden due to collapsed entities - this was determined in #4c above. If 
          the arc becomes hidden, it can get replaced to an ArcIndicator if the entity in question has its
          "indicator" in "running_style" set. Else we just retuen NULL.
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

//template class PtrList<ArcBase>;

ArcBase::ArcBase(MscArcType t, Msc *msc) :
    TrackableElement(msc), type(t), valid(true), compress(false), parallel(false)
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
        CaseInsensitiveEqual(attr,"parallel")||
        CaseInsensitiveEqual(attr,"indicator")) {
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
    if (valid) 
        TrackableElement::PostPosProcess(autoMarker);
    else if (!file_pos.IsInvalid())
        chart->AllArcs[file_pos] = this; //Do this even if we are invalid
}

ArcIndicator::ArcIndicator(Msc *chart, const MscStyle &st, const file_line_range &l) : 
    ArcBase(MSC_ARC_INDICATOR, chart), style(st)
{
    dst = src = chart->AllEntities.Find_by_Name(NONE_ENT_STR);
    AddAttributeList(NULL);
    SetLineEnd(l);
}

ArcIndicator::ArcIndicator(Msc *chart, EIterator s, const MscStyle &st, const file_line_range &l) : 
    ArcBase(MSC_ARC_INDICATOR, chart), src(s), dst(s), style(st)
{
    AddAttributeList(NULL);
    SetLineEnd(l);
}

bool ArcIndicator::IsComplete() const
{
    return *src != chart->NoEntity && *dst != chart->NoEntity;
}

bool ArcIndicator::Combine(const ArcIndicator *o)
{
    if (o==NULL) return false;
    if (src == o->src && dst == o->dst) {
        file_pos.start = std::min(file_pos.start, o->file_pos.start);
        file_pos.end =   std::max(file_pos.end,   o->file_pos.end);
        return true;
    }
    return false;
}

double ArcIndicator::GetXCoord() const 
{
    return (chart->XCoord((*src)->pos)+chart->XCoord((*dst)->pos))/2;
}

MscDirType ArcIndicator::GetToucedEntities(class EntityList &el) const
{
    if (!IsComplete()) return MSC_DIR_INDETERMINATE;
    el.push_back(*src);
    if (*src!=*dst)
        el.push_back(*dst);
    return MSC_DIR_INDETERMINATE;
}

double ArcIndicator::Height(AreaList &cover) 
{
    const double x = (chart->XCoord((*src)->pos) + chart->XCoord((*dst)->pos))/2;
    const Block b = GetIndicatorCover(XY(x, chart->emphVGapOutside));
    area = b;
    cover = area.CreateExpand(chart->compressGap/2);
    cover.mainline += b.y;
    return b.y.till + chart->emphVGapOutside;
} 

void ArcIndicator::Draw() 
{
    const double x = (chart->XCoord((*src)->pos) + chart->XCoord((*dst)->pos))/2;
    DrawIndicator(XY(x, yPos), chart->GetCanvas());
}

//////////////////////////////////////////////////////////////////////////////////////

//Take numbering style from the current context
ArcLabelled::ArcLabelled(MscArcType t, Msc *msc, const MscStyle &s) :
    ArcBase(t, msc), style(s), numberingStyle(msc->Contexts.back().numberingStyle),
    parsed_label(msc->GetCanvas()), concrete_number(-1)
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

//This is only used to convert an ArcBox to an ArcBigArrow in
//ArcBoxSeries::PostParseProcess (when the box is collapsed to a block arrow)
//So PostPProcess was not called for "al", but will be called for this
ArcLabelled::ArcLabelled(MscArcType t, const ArcLabelled &al)
    : ArcBase(t, al.chart), label(al.label), parsed_label(al.parsed_label),
    concrete_number(al.concrete_number), style(al.style), numberingStyle(al.numberingStyle)
{
    //TrackableElement members
    hidden = al.hidden;
    linenum_final = al.linenum_final;
    file_pos = al.file_pos;
    //other members, such as area and yPos and controls will happen only later

    //ArcBase members
    ArcBase::AddAttributeList(NULL); //to kill error
    valid = true;
    at_top_level = al.at_top_level;
    compress = al.compress;
    parallel = al.parallel;
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
    case MSC_ARC_BIG:
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
    //Find position of label attribute (if any)
    file_line linenum_label;
    if (l)
        for (AttributeList::iterator i = l->begin(); i!=l->end(); i++)
            if ((*i)->Is("label")) linenum_label = (*i)->linenum_value.start;
    //Add attributest 
    ArcBase::AddAttributeList(l);
    //compress went to the style, copy it
    if (style.compress.first)
        compress = style.compress.second;
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
ArcBase *ArcLabelled::PostParseProcess(bool hide, EIterator &left, EIterator &right, Numbering &number, bool top_level)
{
    if (!valid) return NULL;
    //We do everything here even if we are hidden (numbering is not impacted by hide/show or collapse/expand)
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
    parsed_label.Set(label, chart->GetCanvas(), style.text);
    return this;
}

void ArcArrow::AttributeNames(Csh &csh)
{
    ArcLabelled::AttributeNames(csh);
    Design().styles["arrow"].AttributeNames(csh);
}

bool ArcArrow::AttributeValues(const std::string attr, Csh &csh)
{
    if (Design().styles["arrow"].AttributeValues(attr, csh)) return true;
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

MscDirType ArcSelfArrow::GetToucedEntities(class EntityList &el) const
{
    el.push_back(*src);
    return MSC_DIR_INDETERMINATE;
}

string ArcSelfArrow::Print(int ident) const
{
    string ss;
    ss << string(ident*2, ' ');
    ss << (*src)->name << (isBidir()?"<->":"->") << (*src)->name;
    if (label.length()>0) ss << ": \"" << label << "\"";
    return ss;
};

ArcBase* ArcSelfArrow::PostParseProcess(bool hide, EIterator &left, EIterator &right, Numbering &number, bool top_level)
{
    if (!valid) return NULL;
    if (chart->ErrorIfEntityGrouped(src, (*src)->file_pos)) return NULL;

    //Add numbering, if needed
    ArcLabelled::PostParseProcess(hide, left, right, number, top_level);

    const EIterator substitute = chart->FindActiveParentEntity(src);
    const bool we_disappear = src != substitute; //src is not visible -> we disappear, too
    if (we_disappear && !(*substitute)->running_style.indicator.second) 
        return NULL;

    //Even if we disappear or shall be hidden, we update left/right
    left = chart->EntityMinByPos(left, substitute);
    right = chart->EntityMaxByPos(right, substitute);
    //update src to point to ActiveEntities
    src = chart->ActiveEntities.Find_by_Ptr(*substitute);
    _ASSERT(src != chart->ActiveEntities.end());

    if (hide) return NULL;
    if (we_disappear) //now indicator must be true, see above
        return new ArcIndicator(chart, src, indicator_style, file_pos);
    //We do not disappear and need not hide
    src_act = (*src)->running_shown.IsActive() ? chart->activeEntitySize/2 : 0;
    return this;
}

void ArcSelfArrow::Width(EntityDistanceMap &distances)
{
    if (!valid) return;
    distances.Insert((*src)->index, DISTANCE_RIGHT, chart->XCoord(0.375)+src_act);
    distances.Insert((*src)->index, DISTANCE_LEFT, parsed_label.getTextWidthHeight().x+src_act);
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
    area = parsed_label.Cover(sx, dx-src_act, y);
    area += Block(dx+src_act, ceil(dx+src_act+wh.x), y, ceil(y+xy_s.y+wh.y+xy_e.y));
    area.mainline = Range(y - chart->nudgeSize/2, y + wh.y + chart->nudgeSize/2);

    cover = area.CreateExpand(chart->compressGap/2);
    return area.GetBoundingBox().y.till + chart->arcVGapBelow;
}

void ArcSelfArrow::PostPosProcess(double autoMarker)
{
    if (!valid) return;
    ArcArrow::PostPosProcess(autoMarker);

    //Check if the entity involved is actually turned on.
    if (!(*src)->status.GetStatus(yPos).IsOn()) {
        string sss;
        sss << "Entity '" << (*src)->name << "' is";
        sss << " turned off, but referenced here.";
        chart->Error.Warning(file_pos.start, sss, "It will look strange.");
    }
    if ((src_act>0) != (*src)->status.GetStatus(yPos).IsActive()) {
        string sss;
        sss << "Entity '" << (*src)->name << "' is";
        sss << " activated/deactivated in a parallel block causing conflict here.";
        chart->Error.Warning(file_pos.start, sss, "It will look strange.");
    }
}

void ArcSelfArrow::Draw()
{
    if (!valid) return;
    double y = yPos + chart->arcVGapAbove;

    parsed_label.Draw(chart->GetCanvas(), sx, dx-src_act, y);
    y += xy_s.y;

    if (style.line.radius.second < 0) {
        //draw an arc
        chart->GetCanvas()->Line(Edge(XY(dx+src_act, y+YSize), wh.x, wh.y/2, 0, 270, 90), style.line);
    } else {
        //draw (part of) a rounded rectangle
        chart->GetCanvas()->Clip(dx+src_act, chart->total.x, 0, chart->total.y);
        chart->GetCanvas()->Line(Block(XY(0, y), XY(dx,y)+wh), style.line);
        chart->GetCanvas()->UnClip();
    }
    //draw arrowheads
    style.arrow.Draw(XY(dx+src_act, y+2*YSize), 0, false, isBidir(), MSC_ARROW_END, chart->GetCanvas());
    style.arrow.Draw(XY(dx+src_act, y        ), 0, true,  isBidir(), MSC_ARROW_START, chart->GetCanvas());
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

ArcDirArrow::ArcDirArrow(const class EntityList &el, bool bidir, const ArcLabelled &al) :
    ArcArrow(bidir ? MSC_ARC_BIG_BIDIR : MSC_ARC_BIG, al), specified_as_forward(false)
{
    src = chart->AllEntities.Find_by_Ptr(*el.begin());
    dst = chart->AllEntities.Find_by_Ptr(*el.rbegin());
    segment_types.push_back(bidir ? MSC_ARC_BIG_BIDIR : MSC_ARC_BIG);
    segment_lines.push_back(style.line);
    for (auto i = ++el.begin(); i!=--el.end(); i++) {
        middle.push_back(chart->AllEntities.Find_by_Ptr(*i));
        segment_types.push_back(MSC_ARC_BIG);
        segment_lines.push_back(style.line);
    }
}

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


MscDirType ArcDirArrow::GetToucedEntities(class EntityList &el) const
{
    el.push_back(*src);
    el.push_back(*dst);
    for (auto i = middle.begin(); i!=middle.end(); i++)
        el.push_back(**i);
    if (isBidir()) return MSC_DIR_BIDIR;
    if ((*src)->pos < (*dst)->pos) return MSC_DIR_RIGHT;
    return MSC_DIR_LEFT;
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

ArcBase *ArcDirArrow::PostParseProcess(bool hide, EIterator &left, EIterator &right, Numbering &number, bool top_level)
{
    if (!valid) return NULL;
    bool error = false;
    error |= chart->ErrorIfEntityGrouped(src, file_pos.start);
    error |= chart->ErrorIfEntityGrouped(dst, file_pos.start);
    for (auto i = middle.begin(); i!=middle.end(); i++)
        error |= chart->ErrorIfEntityGrouped(*i, file_pos.start);
    if (error) return NULL;

    //OK, now we check if arrow will display correctly
    if (middle.size()) {
        const bool dir = (*src)->pos_exp < (*dst)->pos_exp; //true for ->, false for <-
        if (dir != ((*src)->pos_exp < ((*middle[0])->pos_exp))) goto problem;
        for (unsigned i=0; i<middle.size()-1; i++)
            if (dir != ((*middle[i])->pos_exp < (*middle[i+1])->pos_exp)) goto problem;
        if (dir != (((*middle[middle.size()-1])->pos_exp < (*dst)->pos_exp))) goto problem;
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
        e.SortByPosExp();
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

    //Add numbering, if needed
    ArcLabelled::PostParseProcess(hide, left, right, number, top_level);

    //Save our left and right (as specified by the user)
    const EIterator our_left =  chart->EntityMinByPos(src, dst);
    const EIterator our_right = chart->EntityMaxByPos(src, dst);

    //Update src, dst and mid
    EIterator sub;
    sub = chart->FindActiveParentEntity(src);
    src = chart->ActiveEntities.Find_by_Ptr(*sub);
    _ASSERT(src != chart->ActiveEntities.end());
    sub = chart->FindActiveParentEntity(dst);
    dst = chart->ActiveEntities.Find_by_Ptr(*sub);
    _ASSERT(dst != chart->ActiveEntities.end());
    if (src == dst && !(*src)->running_style.indicator.second) //We became a degenerate arrow, do not show us
        return NULL;
    //Update left and right using our original left and right
    const EIterator _left = ++chart->AllEntities.begin(); // leftmost entity;
    const EIterator _right = --chart->AllEntities.end();  // rightmost entiry
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

    if (hide) return NULL;
    if (src == dst) //We became a degenerate arrow, do not show us, but an indicator
        return new ArcIndicator(chart, src, indicator_style, file_pos); 

    //Find the visible parent of each middle point and remove it if equals to
    //an end or to any other middle visible parent
    for (int ii = 0; ii<middle.size(); ii++) {
        sub = chart->FindActiveParentEntity(middle[ii]);
        if (middle[ii] == sub) continue;
        //if the replacement parent equals to an end, delete it
        //sub now is an iterator to AllEntities, src and dst is to ActiveEntities
        if (*sub == *src || *sub == *dst) {
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

    //record what entities are active at the arrow
    act_size.clear();
    act_size.reserve(2+middle.size());
    act_size.push_back((*src)->running_shown.IsActive() ? chart->activeEntitySize/2 : 0);
    for (int iiii = 0; iiii<middle.size(); iiii++) 
        act_size.push_back((*middle[iiii])->running_shown.IsActive() ? chart->activeEntitySize/2 : 0);
    act_size.push_back((*dst)->running_shown.IsActive() ? chart->activeEntitySize/2 : 0);
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
                     end.first + start.second + parsed_label.getTextWidthHeight().x +
                     *act_size.begin() + *act_size.rbegin());
    //Add distances for arrowheads
    const bool fw = (*src)->index  <  (*dst)->index;
    distances.Insert((*src)->index, fw ? DISTANCE_RIGHT : DISTANCE_LEFT, start.second + *act_size.begin());
    distances.Insert((*dst)->index, fw ? DISTANCE_LEFT : DISTANCE_RIGHT, end.first    + *act_size.rbegin());

    if (middle.size()==0) return;
    EntityDistanceMap d;
    for (int i=0; i<middle.size(); i++) {
        DoublePair mid = style.arrow.getWidths(fw, isBidir(), MSC_ARROW_MIDDLE, style.line);
        distances.Insert((*middle[i])->index, DISTANCE_LEFT,  mid.first  + act_size[i+1]);
        distances.Insert((*middle[i])->index, DISTANCE_RIGHT, mid.second + act_size[i+1]);
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
    const double s_act = *act_size.begin();
    const double d_act = *act_size.rbegin();
    if (sx>=dx) {
        std::reverse(xPos.begin(), xPos.end());
        std::reverse(margins.begin(), margins.end());
        std::reverse(segment_lines.begin(), segment_lines.end());
        std::reverse(segment_types.begin(), segment_types.end());
        std::reverse(act_size.begin(), act_size.end());
    }
    //prepare clip_area
    Block total(sx+s_act, dx+d_act, 0, y+lw_max);
    clip_area  = style.arrow.ClipForLine(XY(sx, y), s_act, sx<dx, isBidir(), MSC_ARROW_START,
                                         total, *segment_lines.begin(), *segment_lines.begin());
    clip_area *= style.arrow.ClipForLine(XY(dx, y), d_act, sx<dx, isBidir(), MSC_ARROW_END,
                                         total, *segment_lines.rbegin(), *segment_lines.rbegin());
    for (unsigned i=0; i<middle.size(); i++)
        clip_area *= style.arrow.ClipForLine(XY(chart->XCoord(middle[i]), y), act_size[i+1],
                                             sx<dx, isBidir(), MSC_ARROW_MIDDLE, total, segment_lines[i], segment_lines[i+1]);

    //Add arrowheads and line segments to cover
    for (int i=0; i<xPos.size(); i++)
        area += style.arrow.Cover(XY(xPos[i], y), act_size[i], sx<dx, isBidir(), WhichArrow(i));
    for (int i=0; i<xPos.size()-1; i++) {
        const double lw2 = ceil(segment_lines[i].LineWidth()/2);
        //x coordinates below are not integer- but this will be merged with other contours - so they disappear
        area += clip_area * Block(xPos[i]+margins[i].second, xPos[i+1]-margins[i+1].first, y-lw2, y+lw2);
    }
    lw_max = std::max(lw_max, chart->nudgeSize+1.0);
    //set mainline - not much dependent on main line with
    area.mainline = Range(y - lw_max/2, y + lw_max/2);
    cover = area.CreateExpand(chart->compressGap/2);
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
        if (!(*temp[i])->status.GetStatus(y).IsOn() &&
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
        parsed_label.Draw(chart->GetCanvas(), sx_text, dx_text, yPos + chart->arcVGapAbove, cx_text);
    /* Draw the line */
    //all the entities this (potentially multi-segment arrow visits)
    const double y = yPos+centerline;  //should be integer
    const int mul = (sx<dx) ? 1 : -1;
    if (chart->GetCanvas()->NeedsArrowFix()) {
        for (unsigned i=0; i<xPos.size()-1; i++)
            chart->GetCanvas()->Line(XY(xPos[i  ]+margins[i  ].second+act_size[i  ]*mul, y), 
                                     XY(xPos[i+1]-margins[i+1].first -act_size[i+1]*mul, y), 
                                     segment_lines[i]);
    } else {
        chart->GetCanvas()->Clip(clip_area);
        for (unsigned i=0; i<xPos.size()-1; i++)
            chart->GetCanvas()->Line(XY(xPos[i], y), XY(xPos[i+1], y), segment_lines[i]);
            //chart->Line(XY(xPos[i]+margins[i].second, y), XY(xPos[i+1]-margins[i+1].first, y), style.line);
        chart->GetCanvas()->UnClip();
    }
    /* Now the arrow heads */
    for (unsigned i=0; i<xPos.size(); i++)
        style.arrow.Draw(XY(xPos[i], y), act_size[i], sx<dx, isBidir(), WhichArrow(i), chart->GetCanvas());
}

//////////////////////////////////////////////////////////////////////////////////////

ArcBigArrow::ArcBigArrow(const ArcDirArrow &dirarrow, const MscStyle &s) : 
 ArcDirArrow(dirarrow), sig(NULL)
{
    style = s;
    const MscStyle *refinement = GetRefinementStyle(type);
    if (refinement)
        style += *refinement;
    modifyFirstLineSpacing = false;
}

//This invocation is from ArcBoxSeries::PostParseProcess, so PPP will not be called
//on this arrow again. (But ArcLabelled::PPP was already called)
ArcBigArrow::ArcBigArrow(const EntityList &el, bool bidir, const ArcLabelled &al,
    const ArcSignature *s)
    : ArcDirArrow(el, bidir, al), sig(s)
{
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
    Design().styles["blockarrow"].AttributeNames(csh);
}

bool ArcBigArrow::AttributeValues(const std::string attr, Csh &csh)
{
    if (Design().styles["blockarrow"].AttributeValues(attr, csh)) return true;
    if (ArcLabelled::AttributeValues(attr, csh)) return true;
    return false;
}

string ArcBigArrow::Print(int ident) const
{
    return ArcDirArrow::Print(ident);
}

ArcBase* ArcBigArrow::PostParseProcess(bool hide, EIterator &left, EIterator &right, Numbering &number, bool top_level)
{
    if (!valid) return NULL;
    //Determine src and dst entity, check validity of multi-segment ones, add numbering, etc
    ArcBase *ret = ArcDirArrow::PostParseProcess(hide, left, right, number, top_level);
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
    ind_off = twh.y;
    if (sig && style.indicator.second) {
        twh.y += indicator_size.y;
        twh.x = std::max(twh.x, indicator_size.x);
    } 
    twh.y = std::max(twh.y, Label("M", chart->GetCanvas(), style.text).getTextWidthHeight().y);
    sy = chart->arcVGapAbove + aH + max_lw/2;
    dy = ceil(sy + max_lw/2 + twh.y + chart->emphVGapInside*2 + max_lw) - max_lw/2;

    Area tcov = parsed_label.Cover(0, twh.x, max_lw/2);
    const bool fw = (*src)->index < (*dst)->index;
    //kill entity activate indication from entities where this arrow does not segment (e.g., dot)
    if (style.arrow.bigDoesSegment(isBidir(), MSC_ARROW_START)) act_size[0] = 0;
    if (style.arrow.bigDoesSegment(isBidir(), MSC_ARROW_END)) *act_size.rbegin() = 0;
    if (style.arrow.bigDoesSegment(isBidir(), MSC_ARROW_MIDDLE)) 
        for (int i=1; i<act_size.size()-1; i++)
            act_size[i] = 0;

    const DoublePair start = style.arrow.getBigWidthsForSpace(fw, isBidir(), MSC_ARROW_START, dy-sy);
    const DoublePair end   = style.arrow.getBigWidthsForSpace(fw, isBidir(), MSC_ARROW_END, dy-sy);
    //entity line thickness (due to activate) at src and dst
    const double src_act = *act_size.begin();
    const double dst_act = *act_size.rbegin();

    distances.Insert((fw ? *src : *dst)->index, DISTANCE_LEFT, start.first + (fw ? src_act : dst_act));
    distances.Insert((fw ? *dst : *src)->index, DISTANCE_RIGHT, end.second + (fw ? dst_act : src_act));

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
        std::reverse(act_size.begin(), act_size.end());
    }
    for (int i=0; i<iterators.size()-1; i++) {
        //if neighbours
        if ((*iterators[i])->index + 1 == (*iterators[i+1])->index) {
            distances.Insert((*iterators[i])->index, (*iterators[i+1])->index,
                             margins[i].second+margins[i+1].first + 3 +
                             act_size[i  ] + act_size[i+1]);
        } else {
            distances.Insert((*iterators[i  ])->index,   (*iterators[i  ])->index+1,
                             margins[i].second + 3 + act_size[i]);
            distances.Insert((*iterators[i+1])->index-1, (*iterators[i+1])->index  ,
                             margins[i+1].first + 3 + act_size[i+1]);
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
    distances.Insert((*iterators[stext])->index, (*iterators[dtext])->index, 
        sm + twh.x + dm + act_size[stext] +act_size[dtext]);
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
        sx_text = xPos[stext] + sm + act_size[stext];
        dx_text = xPos[dtext] - dm - act_size[dtext];
        cx_text = (xPos[stext] + xPos[dtext])/2;
    } else {
        sx_text = xPos[xPos.size()-1-stext] + sm + act_size[xPos.size()-1-stext];
        dx_text = xPos[xPos.size()-1-dtext] - dm - act_size[xPos.size()-1-dtext];
        cx_text = (xPos[xPos.size()-1-stext] + xPos[xPos.size()-1-dtext])/2;
    }
    label_cover = parsed_label.Cover(sx_text, dx_text, sy+style.line.LineWidth()/2 + chart->emphVGapInside, cx_text);
    //use += to keep arc and other params of area
    area = style.arrow.BigCover(xPos, act_size, sy, dy, isBidir());
    area.arc = this;
    //set mainline - not much dependent on main line with
    area.mainline = Range(centerline - chart->nudgeSize/2, centerline + chart->nudgeSize/2);
    cover = area.CreateExpand(chart->compressGap/2);
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
    if (sig) {
        controls.push_back(MSC_CONTROL_EXPAND);        
        controls.push_back(MSC_CONTROL_COLLAPSE);        
    }
    ArcArrow::PostPosProcess(autoMarker); //Skip ArcDirArrow
    chart->HideEntityLines(style.arrow.BigEntityLineCover(xPos, act_size, sy, dy, isBidir(), &segment_lines,
        Block(XY(0,0), chart->total)));
}

void ArcBigArrow::Draw()
{
    if (!valid) return;
    style.arrow.BigDraw(xPos, act_size, sy, dy, isBidir(), style.shadow, style.fill, &segment_lines, chart->GetCanvas(), &label_cover);
    parsed_label.Draw(chart->GetCanvas(), sx_text, dx_text, sy+style.line.LineWidth()/2 + chart->emphVGapInside, cx_text);
    if (sig && style.indicator.second)
        DrawIndicator(XY(cx_text, sy+style.line.LineWidth()/2 + chart->emphVGapInside+ind_off), chart->GetCanvas());
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

ArcVerticalArrow* ArcVerticalArrow::AddXpos(VertXPos *p)
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
    Design().styles["vertical"].AttributeNames(csh);
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
    if (Design().styles["vertical"].AttributeValues(attr, csh)) return true;
    if (ArcLabelled::AttributeValues(attr, csh)) return true;
    return false;
}


ArcBase* ArcVerticalArrow::PostParseProcess(bool hide, EIterator &left, EIterator &right,
                                        Numbering &number, bool top_level)
{
    if (!valid) return NULL; 
    //Ignore hide: we show verticals even if they may be hidden
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
    ArcLabelled::PostParseProcess(hide, left, right, number, top_level);

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
        width = Label("M", chart->GetCanvas(), style.text).getTextWidthHeight().y;
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
        width = Label("M", chart->GetCanvas(), style.text).getTextWidthHeight().y;
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
    std::vector<double> act_size(2, 0);
    area = style.arrow.BigCover(ypos_tmp, act_size, xpos-width/2, xpos+width/2, isBidir());
    area.SwapXY();
    //Expand area and add us to chart's all covers list
    ArcArrow::PostPosProcess(autoMarker);
}


void ArcVerticalArrow::Draw()
{
    if (!valid) return;
    if (style.side.second == SIDE_LEFT)
        chart->GetCanvas()->Transform_Rotate90(xpos-width/2, xpos+width/2, false);
    else
        chart->GetCanvas()->Transform_Rotate90(ypos[0], ypos[1], true);
    //Draw background
    const Area lab = parsed_label.Cover(min(ypos[0], ypos[1]), max(ypos[0], ypos[1]),
                                        xpos-width/2+style.line.LineWidth()/2+chart->emphVGapInside,
                                        -1, true);
    std::vector<double> act_size(2,0);
    style.arrow.BigDraw(ypos, act_size, xpos-width/2, xpos+width/2, isBidir(), style.shadow, style.fill,
                        NULL, chart->GetCanvas(), &lab,
                        style.side.second==SIDE_RIGHT, style.side.second==SIDE_LEFT);
    parsed_label.Draw(chart->GetCanvas(), min(ypos[0], ypos[1]), max(ypos[0], ypos[1]),
                      xpos-width/2+style.line.LineWidth()/2+chart->emphVGapInside, -1, true);
    chart->GetCanvas()->UnTransform();
}

//////////////////////////////////////////////////////////////////////////////////////

template<> const char EnumEncapsulator<BoxCollapseType>::names[][ENUM_STRING_LEN] =
    {"invalid", "no", "yes", "arrow", ""};


ArcSignatureCatalog::iterator ArcSignature::WhichIsSimilar(ArcSignatureCatalog &cat) const
{
    for (auto i = cat.begin(); cat.end()!=i; i++)
        if (i->first.file_pos == file_pos)
            return i;
    return cat.end();
}

bool ArcSignature::operator < (const ArcSignature&o) const
{
    return file_pos < o.file_pos;
}

bool ArcSignature::operator == (const ArcSignature&o) const
{
    return file_pos == o.file_pos;
}


ArcBox::ArcBox(MscArcType t, const char *s, file_line_range sl,
                         const char *d, file_line_range dl, Msc *msc) :
    ArcLabelled(t, msc, msc->Contexts.back().styles["emptybox"]),
    content(NULL), drawEntityLines(true), collapsed(BOX_COLLAPSE_EXPAND)
{
    src = chart->FindAllocEntity(s, sl, &valid);
    dst = chart->FindAllocEntity(d, dl, &valid);

    //If both src and dst specified, order them
    if (*src!=chart->NoEntity && *dst!=chart->NoEntity)
        if ((*src)->pos > (*dst)->pos) 
            std::swap(src, dst);
}

const ArcSignature* ArcBox::GetSignature() const
{
    ArcSignature *s = new ArcSignature;
    s->file_pos = file_pos;
    return s;
}

ArcBoxSeries::ArcBoxSeries(ArcBox *first) : 
    ArcBase(MSC_EMPH_SOLID, first->chart), drawing_variant(1), series(true)
{
    series.Append(first);
}


ArcBox* ArcBox::AddArcList(ArcList*l)
{
    if (!valid) return this;
    if (l!=NULL && l->size()>0) {
        content.insert(content.end(), l->begin(), l->end());
        l->clear(); //so that l's constructor does not delete Arcs in arclist
        delete l;
    }
    style = chart->Contexts.back().styles["box"];
    const MscStyle *refinement = GetRefinementStyle(type);
    if (refinement) style += *refinement;
    return this;
}

ArcBase *ArcBox::AddAttributeList(AttributeList *l) 
{
    //First check for a collapse attribute to see what state we are of
    if (l) 
        for (auto i = l->begin(); i!=l->end(); /*nope*/) {
            if ((*i)->Is("collapsed")) {
                AddAttribute(**i);
                //Delete any collapsed attribute
                l->erase(i++);
            } else
                i++; //cycle to next attribute
        }

    //Search for instructions (from GUI) if we should collapse or expand this box
    //these override any attribute setting
    const ArcSignature *s = GetSignature();
    auto force = s->WhichIsSimilar(chart->force_box_collapse);
    if (force != chart->force_box_collapse.end() && collapsed != force->second) {
        collapsed = force->second;
        chart->force_box_collapse_instead[*s] = force->second;
    }
    delete s;
    //If collapsed, use emptybox style
    const MscStyle *refinement = GetRefinementStyle(type);
    if (collapsed==BOX_COLLAPSE_COLLAPSE) {
        style = chart->Contexts.back().styles["box_collapsed"];
        if (refinement) style += *refinement;
    }
    else if (collapsed==BOX_COLLAPSE_BLOCKARROW) {
        style = chart->Contexts.back().styles["box_collapsed_arrow"];
        if (refinement) style += *refinement;
    }
    ArcLabelled::AddAttributeList(l);
    return this;
}

bool ArcBox::AddAttribute(const Attribute &a)
{
    if (a.type == MSC_ATTR_STYLE) return ArcLabelled::AddAttribute(a);
    if (a.Is("color")) {
        return style.fill.AddAttribute(a, chart, style.type);
    }
    if (a.Is("collapsed")) {
        if (content.size()==0) {
            chart->Error.Error(a, false, "The 'collapsed' attribute can only be specified for boxes with content.",
                               "Ignoring attribute.");
            return true;
        }
        if (!a.EnsureNotClear(chart->Error, STYLE_ARC))
            return true;
        if (a.type == MSC_ATTR_BOOL) {
            collapsed = a.yes ? BOX_COLLAPSE_COLLAPSE : BOX_COLLAPSE_EXPAND;
            return true;
        }
        if (a.type == MSC_ATTR_STRING && Convert(a.value, collapsed)) 
            return true;
        a.InvalidValueError(CandidatesFor(collapsed), chart->Error);
        return true;
    }
    return ArcLabelled::AddAttribute(a);
}

void ArcBox::AttributeNames(Csh &csh)
{
    ArcLabelled::AttributeNames(csh);
    Design().styles["box"].AttributeNames(csh);
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"collapsed", HINT_ATTR_NAME));
}

bool CshHintGraphicCallbackForBoxCollapsed(MscCanvas *canvas, CshHintGraphicParam p)
{
    if (!canvas) return false;
    BoxCollapseType t = (BoxCollapseType)(int)p;
    switch (t) {
    case BOX_COLLAPSE_COLLAPSE:
        return CshHintGraphicCallbackForYesNo(canvas, CshHintGraphicParam(1));
    case BOX_COLLAPSE_EXPAND:
        return CshHintGraphicCallbackForYesNo(canvas, CshHintGraphicParam(0));
    case BOX_COLLAPSE_BLOCKARROW:
        return CshHintGraphicCallbackForSide(canvas, CshHintGraphicParam(int(SIDE_LEFT)));
    } 
    return true;
}

bool ArcBox::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr,"color")) {
        csh.AddColorValuesToHints();
        return true;
    }
    if (CaseInsensitiveEqual(attr, "collapsed")) {
        csh.AddToHints(EnumEncapsulator<BoxCollapseType>::names, csh.HintPrefix(COLOR_ATTRVALUE), 
                       HINT_ATTR_VALUE, CshHintGraphicCallbackForBoxCollapsed); 
        return true;
    }
    if (Design().styles["box"].AttributeValues(attr, csh)) return true;
    if (ArcLabelled::AttributeValues(attr, csh)) return true;
    return false;
}

//This should be called after adding content to "f', but before adding any attributes
ArcBoxSeries* ArcBoxSeries::AddFollow(ArcBox *f)
{
    _ASSERT(f);
    if (f==NULL) return this;
    if (f->valid) {
        //Use the style of the first box in the series as a base
        MscStyle s = (*series.begin())->style;
        //Override with the line type specified (if any)
        if (f->type != MSC_EMPH_UNDETERMINED_FOLLOW)
            s += *f->GetRefinementStyle(f->type);
        f->style = s;
        //AddAttributeList will be called for "f" after this function
    } else
        valid = false;
    series.Append(f);
    return this;
}

string ArcBox::Print(int ident) const
{
    string ss;
    ss << string(ident*2, ' ');
    ss << (*src)->name << "--" << (*dst)->name;
    if (label.length()>0) ss << ": \"" << label << "\"";
    for (auto i = content.begin(); i != content.end(); i++)
        ss << "\n" << (*i)->Print(ident+1);
    return ss;
}

MscDirType ArcBoxSeries::GetToucedEntities(class EntityList &el) const
{
    el.push_back(*(*series.begin())->src);
    el.push_back(*(*series.begin())->dst);
    return MSC_DIR_INDETERMINATE;
}

string ArcBoxSeries::Print(int ident) const
{
    string ss(ident*2, ' '); 
    ss << "Box series";
    for (auto i = series.begin(); i!=series.end(); i++)
        ss << "\n" << (*i)->Print(ident+1);
    return ss;
}

ArcBase* ArcBox::PostParseProcess(bool hide, EIterator &left, EIterator &right,
                                  Numbering &number, bool top_level)
{
    if (collapsed == BOX_COLLAPSE_BLOCKARROW) {
        EntityList el(false);
        //replace us with a block arrow - here we are sure to be alone in the series
        el.Append(*src);
        el.Append(*dst);
        MscDirType dir = chart->GetTouchedEntitiesArcList(content, el);
        if (dir == MSC_DIR_INDETERMINATE) dir = MSC_DIR_BIDIR;
        //remove NoEntity and side ones from el
        for (auto i = el.begin(); i!=el.end(); /*nope*/)
            if ((*i)->name == NONE_ENT_STR || (*i)->name == LSIDE_ENT_STR || (*i)->name == RSIDE_ENT_STR)
                el.erase(i++);
            else 
                i++;
        el.SortByPosExp();
        if (dir == MSC_DIR_LEFT)
            std::reverse(el.begin(), el.end());
        ArcBigArrow *ret = new ArcBigArrow(el, dir == MSC_DIR_BIDIR, *this, GetSignature());
        ret->ArcArrow::AddAttributeList(NULL); //skip copying line segment styles
        return ret->PostParseProcess(hide, left, right, number, top_level);
    }
    ArcLabelled::PostParseProcess(hide, left, right, number, top_level);
    //Add numbering, if needed
    EIterator left_content = chart->AllEntities.Find_by_Name(NONE_ENT_STR);
    EIterator right_content = left_content;
    if (content.size()) {
        //If we have attached a number to the label of a box, set flag on number,
        //so that if we add levels inside the content of the box (before displaying any number)
        //we continue from present value
        if (style.numbering.second && label.length()!=0)
            number.decrementOnAddingLevels = true;
        const bool hide_i = hide || (collapsed!=BOX_COLLAPSE_EXPAND);
        chart->PostParseProcessArcList(hide_i, content, false, 
                          left_content, right_content, number, top_level);
        //If we are collapsed, but not hidden and "indicator" attribute is set, 
        //then add an indicator to the end of the list (which will have only elements
        //with zero height here, the rest removed themselves due to hide_i==true
        if (collapsed!=BOX_COLLAPSE_EXPAND && style.indicator.second && !hide) 
            //src and dst of the ArcIndicator will be set in ArcBoxSeries::PostParseProcess
            content.Append(new ArcIndicator(chart, indicator_style, file_pos));  
        number.decrementOnAddingLevels = false;
    } 
    //left & right will not expand if src and dst is unspecified
    left = chart->EntityMinByPos(left, src);
    right = chart->EntityMaxByPos(right, dst);
    if (*src == chart->NoEntity) src = left_content;
    if (*dst == chart->NoEntity) dst = right_content;
    return this;
}

ArcBase* ArcBoxSeries::PostParseProcess(bool hide, EIterator &left, EIterator &right,
                                        Numbering &number, bool top_level)
{
    if (!valid || series.size()==0) return NULL;
    //If first segment is compressed or parallel, copy that to full series
    compress = (*series.begin())->compress;
//    parallel = (*series.begin())->parallel;

    ArcBase *ret;
    EIterator src, dst;
    dst = src = chart->AllEntities.Find_by_Name(NONE_ENT_STR);
    for (auto i = series.begin(); i!=series.end(); i++) {
        if (i!=series.begin() && (*i)->parallel) {
            chart->Error.Error((*i)->file_pos.start,
                               "Attribute 'parallel' can only be specified in the first "
                               "element in a pipe series. Ignoring it in subsequent ones.");
        }
        if ((*i)->content.size()) {
            if ((*i)->collapsed == BOX_COLLAPSE_BLOCKARROW && series.size()>1) {
                chart->Error.Error((*i)->file_pos.start, "Only single boxes (and not box series) can be collapsed to a block arrow.", 
                    "Collapsing to a box instead.");
                (*i)->collapsed = BOX_COLLAPSE_COLLAPSE;
            } 
        } else if ((*i)->collapsed != BOX_COLLAPSE_EXPAND) {
            chart->Error.Warning(file_pos.start, "Cannot collapse an empty box.", 
                                    "Ignoring 'collapsed' attribute.");
            (*i)->collapsed = BOX_COLLAPSE_EXPAND;
        }
        //Add numbering, do content, add NULL for indicators to "content", adjust src/dst,
        //and collect left and right if needed
        ret = (*i)->PostParseProcess(hide, src, dst, number, top_level); //ret is an arcblockarrow if we need to collapse
    }
    //parallel flag can be either on the series or on the first element
    parallel |= (*series.begin())->parallel;
    //src and dst can be NoEntity here if none of the series specified a left or a right entity
    //Go through and use content to adjust to content
    if (*src==chart->NoEntity) 
        for (auto i = series.begin(); i!=series.end(); i++) 
            src = chart->EntityMinByPos(src, (*i)->src);
    if (*dst==chart->NoEntity) 
        for (auto i = series.begin(); i!=series.end(); i++) 
            dst = chart->EntityMaxByPos(dst, (*i)->dst);

    //Src and dst can still be == NoEntity, if no arcs specified
    //inside the content and no enity specified at box declaration.
    //In this case emph box spans to leftmost and rightmost entity in chart.
    //At PostParse chart->AllEntities is already sorted by pos values
    //we only do this step if we are the first in a box series.
    if (*src==chart->NoEntity) src = ++ ++chart->AllEntities.begin();  //leftmost entity after Noentity and (left)
    if (*dst==chart->NoEntity) dst = -- --chart->AllEntities.end();    //rightmost entity (before (right)

    //Now see how entities change due to entity collapse
    EIterator sub1 = chart->FindWhoIsShowingInsteadOf(src, true);
    EIterator sub2 = chart->FindWhoIsShowingInsteadOf(dst, false);

    //if box spans a single entity and both ends have changed, 
    //we kill this box 
    const bool we_diappear = *sub1==*sub2 && sub1!=src && sub2!=dst;
    if (we_diappear && !(*sub1)->running_style.indicator.second) 
        return NULL;
        
    //Now convert src and dst to an iterator pointing to ActiveEntities
    src = chart->ActiveEntities.Find_by_Ptr(*sub1); 
    dst = chart->ActiveEntities.Find_by_Ptr(*sub2);
    _ASSERT(src != chart->ActiveEntities.end()); 
    _ASSERT(dst != chart->ActiveEntities.end()); 

    //Now copy src, dst and indicator to all boxes
    //And to indicators if any
    for (auto i = series.begin(); i!=series.end(); i++) {
        if ((*i)->content.size()) {
            ArcIndicator*ai = dynamic_cast<ArcIndicator*>(*(*i)->content.rbegin());
            if (ai && !ai->IsComplete()) //if ai is complete it may come from an entity disapearing due to entity collapse
                ai->SetEntities(src, dst);
        }
        (*i)->src = src;
        (*i)->dst = dst;
    }
    //Note that src and dst now point to ActiveEntities: use subX to 
    //keep left and right to point to AllEntities
    left = chart->EntityMinByPos(chart->EntityMinByPos(left, sub1), sub2);
    right = chart->EntityMaxByPos(chart->EntityMaxByPos(right, sub1), sub2);
    if (hide) return NULL;  
    if (we_diappear) //we disappear, but leave an indicator: left & right shall be updated
        return new ArcIndicator(chart, src, indicator_style, file_pos);

    //Check if we are collapsed to a block arrow
    if (series.size()==1 && (*series.begin())->collapsed == BOX_COLLAPSE_BLOCKARROW) 
        return ret;
    return this;
}

//will only be called for the first box of a multi-segment box series
void ArcBoxSeries::Width(EntityDistanceMap &distances)
{
    if (!valid) return;
    const MscStyle &overall_style = (*series.begin())->style;
    const EIterator src = (*series.begin())->src;
    const EIterator dst = (*series.begin())->dst;

    EntityDistanceMap d;
    double max_width = 0; //the widest label plus margins
    for (auto i = series.begin(); i!=series.end(); i++) {
        if ((*i)->content.size())
            chart->WidthArcList(((*i)->content), d);
        double width = (*i)->parsed_label.getTextWidthHeight().x;
        //calculated margins (only for first segment) and save them
        if (i==series.begin()) {
            const Area tcov = (*i)->parsed_label.Cover(0, width, overall_style.line.LineWidth()+chart->emphVGapInside);
            DoublePair margins = overall_style.line.CalculateTextMargin(tcov, 0);
            width += margins.first + margins.second;
            (*i)->sx_text = margins.first;
            (*i)->dx_text = margins.second;
        } else {
            (*i)->sx_text = (*i)->dx_text = overall_style.line.LineWidth();
        }
        max_width = max(max_width, width);
    }

    //Now d contains distance requirements within this emph box series
    //And "max_width" contains the widest
    double left_space_inside = d.Query((*src)->index, DISTANCE_LEFT);
    double right_space_inside = d.Query((*dst)->index, DISTANCE_RIGHT);

    if (src == dst) {
        //ensure that text fits
        left_space_inside = std::max(max_width/2, left_space_inside);
        right_space_inside = std::max(max_width/2, right_space_inside);
    } else {
        //do a default margin and ensure that internals fit
        const double def_margin = chart->XCoord(0.25);
        left_space_inside = max(def_margin, left_space_inside);
        right_space_inside = max(def_margin, right_space_inside);
    }

    //Check box_side requirements
    const std::pair<double, double> l_tmp = d.QueryBoxSide((*src)->index-1, false); 
    const std::pair<double, double> r_tmp = d.QueryBoxSide((*dst)->index,   true); 
    left_space_inside  = std::max(left_space_inside,  l_tmp.second);
    right_space_inside = std::max(right_space_inside, r_tmp.first);

    //add gap and linewidth
    max_width += 2*chart->emphVGapInside;
    left_space =  left_space_inside +  chart->emphVGapInside + overall_style.line.LineWidth();
    right_space = right_space_inside + chart->emphVGapInside + overall_style.line.LineWidth();

    //convert the side requirements to pairwise distances
    d.CombineLeftRightToPair_Max(chart->hscaleAutoXGap, chart->activeEntitySize/2);
    d.CombineLeftRightToPair_Single(chart->hscaleAutoXGap);
    d.CopyBoxSideToPair(chart->hscaleAutoXGap);

    //if we span multiple entities ensure that text fits
    if (src!=dst && max_width > left_space + right_space)
        distances.Insert((*src)->index, (*dst)->index, max_width - left_space - right_space);

    //Add side distances
    distances.Insert((*src)->index, DISTANCE_LEFT, left_space);
    distances.Insert((*dst)->index, DISTANCE_RIGHT, right_space);
    distances.InsertBoxSide((*src)->index-1, l_tmp.first, left_space);
    distances.InsertBoxSide((*dst)->index, right_space, r_tmp.second);
    distances += d;
}

double ArcBoxSeries::Height(AreaList &cover)
{
    if (!valid) return 0;
    //A few explanations of the variables exact meaning
    //the upper edge of the upper line of each segment is at yPos
    //total_height includes linewidths and shadow, but not emphVGapOutside (contrary for pipes)
    //left_space and right_space includes linewidth
    //height includes the upper linewidth, emphvgapinside, content, lower emphvgapinside, but not lower lw
    //sx and dx are the inner edges of the lines of the whole box
    const MscStyle &main_style = (*series.begin())->style;
    const double lw = main_style.line.LineWidth();
    const double sx = chart->XCoord((*series.begin())->src) - left_space + lw;
    const double dx = chart->XCoord((*series.begin())->dst) + right_space - lw;

    double y = chart->emphVGapOutside;
    for (auto i = series.begin(); i!=series.end(); i++) {
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
        if ((*i)->content.size()) {
            const double &radius = main_style.line.radius.second;
            Area limit = (*i)->text_cover;
            if (i==series.begin() && radius != CORNER_NONE && radius>0) {
                //Funnily shaped box, prevent content from hitting it
                Block b(sx, dx, y, lw+y+radius*4);
                limit += Contour(Block(sx, dx, 0, y+lw+main_style.line.radius.second)) -
                            main_style.line.CreateRectangle(b);
                if (main_style.line.corner.second == CORNER_NOTE) {
                    const double r = radius + main_style.line.RadiusIncMul()*lw/2 -
                        (main_style.line.IsDouble() ? main_style.line.DoubleSpacing() : 0);
                    limit += Contour(b.x.till-r, b.y.from, b.x.till-r, b.y.from+r,
                                        b.x.till, b.y.from+r);
                }
            }
            y = chart->PlaceListUnder((*i)->content.begin(), (*i)->content.end(),
                                        y+th, y, limit, compress, &content_cover);  //no extra margin below text
        } else {
            y += th; //no content, just add textheight
        }
        if (i==--series.end() && main_style.line.corner.second != CORNER_NONE && main_style.line.radius.second>0) {
            //Funnily shaped box, prevent it content from hitting the bottom of the content
            Block b(sx, dx, 0, y);
            MscLineAttr limiter_line(main_style.line);
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
        if (i==--series.end())
            (*i)->height_w_lower_line = (*i)->height + lw;
        else
            (*i)->height_w_lower_line = (*i)->height + (*((++i)--))->style.line.LineWidth();
    } /* for cycle through segments */
    //Final advance of linewidth, the inner edge (y) is on integer
    total_height = y + lw - yPos;
    Block b(sx-lw, dx+lw, yPos, yPos + total_height);
    MscLineAttr line_for_outer_edge = main_style.line;
    line_for_outer_edge.radius.second += lw * main_style.line.RadiusIncMul();
    Area overall_box = line_for_outer_edge.CreateRectangle(b);
    // now we have all geometries correct, now calculate areas and covers
    for (auto i = series.begin(); i!=series.end(); i++) {
        (*i)->area = Contour(sx-lw, dx+lw, (*i)->yPos, (*i)->yPos + (*i)->height_w_lower_line) * overall_box;
        (*i)->area.arc = *i;
        if ((*i)->content.size() && (*i)->collapsed==BOX_COLLAPSE_EXPAND) {
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
    const double &offset = main_style.shadow.offset.second;
    if (offset)
        cover = overall_box + overall_box.CreateShifted(XY(offset, offset));
    else
        cover = overall_box;
    cover = cover.CreateExpand(chart->compressGap/2);
    return yPos + total_height + offset + chart->emphVGapOutside;
}

void ArcBox::ShiftBy(double y)
{
    if (!valid) return;
    if (y==0) return;
    y_text += y;
    text_cover.Shift(XY(0,y));
    ArcLabelled::ShiftBy(y);
    if (content.size())
        chart->ShiftByArcList(content.begin(), content.end(), y);
}

void ArcBoxSeries::ShiftBy(double y)
{
    if (!valid) return;
    if (y==0) return;
    for (auto i=series.begin(); i!=series.end(); i++) 
        (*i)->ShiftBy(y);    
    ArcBase::ShiftBy(y);
}

void ArcBoxSeries::PostPosProcess(double autoMarker)
{
    if (!valid) return;
    //For boxes we always add the background cover first then the content
    //And we do this for each segment sequentially
    for (auto i = series.begin(); i!=series.end(); i++)
        if ((*i)->valid) {
            if ((*i)->content.size()) 
                switch ((*i)->collapsed) {
                case BOX_COLLAPSE_EXPAND:
                    (*i)->controls.push_back(MSC_CONTROL_COLLAPSE);        
                    if (series.size()==1) 
                        (*i)->controls.push_back(MSC_CONTROL_ARROW);        
                    break;
                case BOX_COLLAPSE_COLLAPSE:
                    (*i)->controls.push_back(MSC_CONTROL_EXPAND);        
                    if (series.size()==1) 
                        (*i)->controls.push_back(MSC_CONTROL_ARROW);        
                    break;
                case BOX_COLLAPSE_BLOCKARROW:
                default:
                    _ASSERT(0); //should not happen here
                    break;
                }
            (*i)->ArcLabelled::PostPosProcess(autoMarker);
            if ((*i)->content.size()) 
                chart->PostPosProcessArcList((*i)->content, autoMarker);
        }

    //Hide entity lines during the lines inside the box
    const MscStyle &main_style = (*series.begin())->style;
    const double lw = main_style.line.LineWidth();
    const double src_x = chart->XCoord((*series.begin())->src);
    const double dst_x = chart->XCoord((*series.begin())->dst);
    const double sx = src_x - left_space + lw;
    const double dx = dst_x + right_space - lw;
    for (auto i = ++series.begin(); i!=series.end(); i++) 
        if ((*i)->style.line.IsDoubleOrTriple()) {
            const Block r(sx, dx, (*i)->yPos, (*i)->yPos+(*i)->style.line.LineWidth());
            chart->HideEntityLines(r);
        }
    
    //hide the entity lines under the labels
    for (auto i = series.begin(); i!=series.end(); i++) 
        chart->HideEntityLines((*i)->text_cover);
    //hide top and bottom line if double
    if (main_style.line.IsDoubleOrTriple()) {
        Block b(src_x - left_space, dst_x + right_space,
                yPos, yPos+total_height); //The outer edge of the lines
        MscLineAttr line = main_style.line;  //We will vary the radius, so we need a copy
        line.radius.second = std::min(std::min(b.y.Spans()/2 - lw, b.x.Spans()/2 - lw), line.radius.second);
        //The radius specified in style.line will be that of the inner edge of the line
        if (line.radius.second > 0)
            line.radius.second += lw * line.RadiusIncMul(); //now it is appropriate for the outer edge
        Area hide = line.CreateRectangle(b);
        line.radius.second -= lw * line.RadiusIncMul(); //now it is appropriate for the inner edge
        hide -= line.CreateRectangle(b.Expand(-lw));

        if (main_style.line.corner.second == CORNER_NOTE) {
            const double r = main_style.line.radius.second + main_style.line.RadiusIncMul()*lw/2 -
                                (main_style.line.IsDouble() ? main_style.line.DoubleSpacing() : 0);
            hide += Contour(b.UpperRight()+XY(0,r), b.UpperRight()+XY(-r,0), b.UpperRight()+XY(-r,r));
        }
        chart->HideEntityLines(hide);
    }
}

void ArcBoxSeries::Draw()
{
    if (!valid) return;
    //For boxes draw background for each segment, then separator lines, then bounding rectangle lines, then content
    const MscStyle &main_style = (*series.begin())->style;
    const double lw = main_style.line.LineWidth();
    const double src_x = chart->XCoord((*series.begin())->src);
    const double dst_x = chart->XCoord((*series.begin())->dst);
    MscLineAttr line = main_style.line;  //We will vary the radius, so we need a copy
    Block r(src_x - left_space, dst_x + right_space, yPos, yPos+total_height); //The outer edge of the lines
    line.radius.second = std::min(std::min(r.y.Spans()/2 - lw, r.x.Spans()/2 - lw), line.radius.second);
    //The radius specified in style.line will be that of the inner edge of the line
    if (line.radius.second > 0)
        line.radius.second += lw * line.RadiusIncMul(); //now it is appropriate for the outer edge
    //First draw the shadow.
    r.Expand(-line.width.second/2.);
    line.radius.second -= line.width.second/2. * line.RadiusIncMul();
    chart->GetCanvas()->Shadow(r, line, main_style.shadow);
    //Do a clip region for the overall box (for round corners)
    //at half a linewidth from the inner edge (use the width of a single line!)
    line.radius.second -= (lw-line.width.second) * line.RadiusIncMul();
    r.Expand(-lw+line.width.second);
    chart->GetCanvas()->Clip(r, line);
    for (auto i = series.begin(); i!=series.end(); i++) {
        //Overall rule for background fill:
        //for single line borders we fill up to the middle of the border
        //for double line borders we fill up to the middle of the inner line of the border
        //style.line.LineWidth() gives the full width of the (double) line, width.second is just one line of it
        //for single lines style.line.LineWidth()==style.line.width.second
        double sy = (*i)->yPos + (*i)->style.line.LineWidth() - (*i)->style.line.width.second/2.;
        double dy = (*i)->yPos + (*i)->height;
        //decrease upper limit for the first one (note+triple line has areas higher than this to cover)
        //clip will cut away the not needed areas
        if (i==series.begin())
            sy -= lw;
        //Increase the fill area downward by half of the linewidth below us
        PtrList<ArcBox>::const_iterator next = i;
        next++;
        if (next==series.end())
            dy += main_style.line.width.second/2.;
        else
            dy += (*next)->style.line.width.second/2.;
        //fill wider than r.x - note+triple line has wider areas to cover, clip will cut away excess
        chart->GetCanvas()->Fill(Block(r.x.from, r.x.till+lw, sy, dy), (*i)->style.fill);
        //if there are contained entities, draw entity lines, strictly from inside of line
        if ((*i)->content.size() && (*i)->collapsed==BOX_COLLAPSE_EXPAND && (*i)->drawEntityLines)
            chart->DrawEntityLines((*i)->yPos, (*i)->height + (*i)->style.line.LineWidth(), (*i)->src, ++EIterator((*i)->dst));
    }
    chart->GetCanvas()->UnClip();
    //shring to the inner edge
    r.Expand(-line.width.second/2.);
    line.radius.second -= line.width.second/2. * line.RadiusIncMul();
    //Draw box lines - Cycle only for subsequent boxes
    for (auto i = ++series.begin(); i!=series.end(); i++) {
        const double y = (*i)->yPos + (*i)->style.line.LineWidth()/2;
        const XY magic(1,0);  //XXX needed in windows
        chart->GetCanvas()->Line(XY(r.x.from, y)-magic, XY(r.x.till, y), (*i)->style.line);
    }
    //Finally draw the overall line around the box, expand to midpoint of line
    r.Expand(lw/2);
    line.radius.second += (lw/2)  * line.RadiusIncMul();
    chart->GetCanvas()->Line(r, line);
    //XXX double line joints: fix it
    for (auto i = series.begin(); i!=series.end(); i++) {
        (*i)->parsed_label.Draw(chart->GetCanvas(), (*i)->sx_text, (*i)->dx_text, (*i)->y_text);
        if ((*i)->content.size())
            chart->DrawArcList((*i)->content);
        //if (i==follow.begin()) {
        //    const Area tcov = (*i)->parsed_label.Cover(0, (*i)->parsed_label.getTextWidthHeight().x, style.line.LineWidth()+chart->emphVGapInside);
        //    DoublePair margins = style.line.CalculateTextMargin(tcov, 0, follow.size()==1?chart:NULL);
        //}
    }
}

/////////////////////////////////////////////////////////////////

ArcPipe::ArcPipe(ArcBox *box) :
    ArcLabelled(box->type, box->chart, box->chart->Contexts.back().styles["pipe"]),
    src(box->src), dst(box->dst), drawEntityLines(false)
{
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
}

ArcPipeSeries::ArcPipeSeries(ArcPipe *first) :
    ArcBase(MSC_EMPH_SOLID, first->chart), drawing_variant(1), series(true)
{
    series.Append(first);
}

ArcPipeSeries* ArcPipeSeries::AddArcList(ArcList*l)
{
    if (!valid) return this;
    if (l!=NULL && l->size()>0) {
        content.insert(content.end(), l->begin(), l->end());
        l->clear(); //so that l's constructor does not delete Arcs in arclist
        delete l;
    }
    return this;
}

bool ArcPipe::AddAttribute(const Attribute &a)
{
    if (a.type == MSC_ATTR_STYLE) return ArcLabelled::AddAttribute(a);
    if (a.Is("color")) {
        return style.fill.AddAttribute(a, chart, style.type);
    }
    return ArcLabelled::AddAttribute(a);
}

void ArcPipe::AttributeNames(Csh &csh)
{
    ArcLabelled::AttributeNames(csh);
    Design().styles["pipe"].AttributeNames(csh);
}

bool ArcPipe::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr,"color")) {
        csh.AddColorValuesToHints();
        return true;
    }
    if (Design().styles["pipe"].AttributeValues(attr, csh)) return true;
    if (ArcLabelled::AttributeValues(attr, csh)) return true;
    return false;
}

//Should be called before AddAttributeList for "f" is called
ArcPipeSeries* ArcPipeSeries::AddFollowWithAttributes(ArcPipe*f, AttributeList *l)
{
    _ASSERT(f);
    if (f==NULL) return this;
    if (f->valid) {
        f->style.Empty();
        f->AddAttributeList(l);
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
        if (f->parallel) {
            chart->Error.Error(f->file_pos.start,
                               "Attribute 'parallel' can only be specified in the first "
                               "element in a pipe series. Ignoring it in subsequent ones.");
        }
        //Use the style of the first box in the series as a base
        MscStyle s = (*series.begin())->style;
        //Override with the line type specified (if any)
        _ASSERT(f->type != MSC_EMPH_UNDETERMINED_FOLLOW);
        s += *f->GetRefinementStyle(f->type);
        s += f->style; //add the result of attributes
        f->style = s;
        //AddAttributeList will NOT be called for "f" after this function
    } else
        valid = false;
    series.Append(f);
    return this;
}

string ArcPipe::Print(int ident) const
{
    string ss;
    ss << string(ident*2, ' ');
    ss << (*src)->name << "--" << (*dst)->name;
    if (label.length()>0) ss << ": \"" << label << "\"";
    return ss;
}

MscDirType ArcPipeSeries::GetToucedEntities(EntityList &el) const
{
    if (content.size()) 
        return chart->GetTouchedEntitiesArcList(content, el);
    //If no content, add leftmost and rightmost
    el.push_back(*chart->EntityMinByPos((*series.begin())->src, (*series.rbegin())->src));
    el.push_back(*chart->EntityMaxByPos((*series.begin())->dst, (*series.rbegin())->dst));
    return MSC_DIR_INDETERMINATE;
}

string ArcPipeSeries::Print(int ident) const
{
    string ss(ident*2, ' '); 
    ss << "Pipe series";
    for (auto i = series.begin(); i!=series.end(); i++)
        ss << "\n" << (*i)->Print(ident+1);
    for (auto i = content.begin(); i != content.end(); i++)
        ss << "\n" << (*i)->Print(ident+2);
    return ss;
}

struct pipe_compare
{
    const Msc *chart;
    bool fromright;
    pipe_compare(const Msc *c, bool fr) : chart(c), fromright(fr) {}
    bool operator ()(const ArcPipe *p1, const ArcPipe *p2) const {
        EIterator min1 = chart->EntityMinMaxByPos(p1->src, p1->dst, fromright);
        EIterator min2 = chart->EntityMinMaxByPos(p2->src, p2->dst, fromright);
        if (min1==min2) return false; //equals are not less
        return min1 == chart->EntityMinMaxByPos(min1, min2, fromright);
    }
};

ArcBase* ArcPipeSeries::PostParseProcess(bool hide, EIterator &left, EIterator &right,
                                        Numbering &number, bool top_level)
{
    if (!valid) return NULL;

    //Add numbering, if needed 
    for (auto i = series.begin(); i!=series.end(); i++) 
        (*i)->PostParseProcess(hide, left, right, number, top_level);
    //Postparse the content;
    EIterator content_left, content_right;
    content_right = content_left = chart->AllEntities.Find_by_Name(NONE_ENT_STR);
    chart->PostParseProcessArcList(hide, content, false, content_left, content_right, number, top_level);

    //parallel flag can be either on the series or on the first element
    parallel |= (*series.begin())->parallel;

    //Check that all pipe segments are fully specified, non-overlapping and sort them

    //frist sort from left to right
    pipe_compare comp(chart, true);
    series.sort(comp);
    //Make both src and dst specified and ordered in all segments
    //The leftmost and the rightmost segment can auto-adjust to the content 
    //others can snap left and right if not specified
    if (*(*series.begin())->src == chart->NoEntity) 
        (*series.begin())->src = content_left;
    //If the left side is still unspecified, we had no content, return an error
    if (*(*series.begin())->src == chart->NoEntity) {
        chart->Error.Error((*series.begin())->file_pos.start, "The left side of the leftmost pipe segment in an empty pipe must be specified.",
                           "Ignoring this pipe.");
        return NULL;
    }
    if (*(*series.begin())->src == chart->NoEntity) 
        (*series.begin())->src = content_left;
    //If the left side is still unspecified, we had no content, return an error
    if (*(*series.begin())->src == chart->NoEntity) {
        chart->Error.Error((*series.begin())->file_pos.start, "The right side of the rightmost pipe segment in an empty pipe must be specified.",
                           "Ignoring this pipe.");
        return NULL;
    }
    if (series.size()>1) {
        for (auto i = ++series.begin(); i!=--series.end(); i++) {
            auto i_next = i; i_next++;

            if (*(*i_next)->src == chart->NoEntity) (*i_next)->src = (*i)->dst;
            if (*(*i)->dst      == chart->NoEntity) (*i)->dst      = (*i_next)->src;

            if (*(*i)->dst == chart->NoEntity || *(*i_next)->src == chart->NoEntity) {
                chart->Error.Error((*i)->file_pos.start, "Could not figure out the extent of this pipe segment. Specify right entity.");
                return NULL;
            }
        }

        //Now check that segments sanity
        if (series.size()>1) 
            for (auto i = series.begin(); i!=series.end(); /*nope*/) {
                const EIterator loc_src = chart->FindLeftRightDescendant((*i)->src, true, false);
                const EIterator loc_dst = chart->FindLeftRightDescendant((*i)->dst, false, false);
                if (i != --series.end()) {
                    auto i_next = i; i_next++;
                    const EIterator next_src = chart->FindLeftRightDescendant((*i_next)->src, true, false);
                    if (loc_src == loc_dst && loc_dst == next_src) {
                        chart->Error.Error((*i)->file_pos.start, "This pipe segment is attaches to the next segment but spans only a single entity."
                            "Segment will not be shown.");
                        series.erase(i++);
                        continue;
                    }
                }
                if (i != series.begin()) {
                    auto i_prev = i; i_prev--; 
                    const EIterator prev_dst = chart->FindLeftRightDescendant((*i_prev)->dst, false, false);
                    if (loc_src == loc_dst && loc_src == prev_dst) {
                        chart->Error.Error((*i)->file_pos.start, "This pipe segment is attaches to the previous segment but spans only a single entity."
                            "Segment will not be shown.");
                        series.erase(i++);
                        continue;
                    }
                    if (chart->EntityMaxByPos(prev_dst, loc_src) != loc_src) 
                        chart->Error.Warning((*i)->file_pos.start, "This pipe segment overlaps the previousl one. It may not look so good.",
                        "Encapsulate one in the other if you want that effect.");
                }
                i++;
            }
    }
    if (series.size()==0) return NULL;

    //All the above operations were checked on AllEntities. We have accepted the pipe
    //as valid here and should not complain no matter what entities are collapsed or not

    //Now change src:s and dst:s to active entities.
    //First look up their active parent in AllEntities (or if they are
    //visible group entities find, the left/right-most children)
    //Then convert to ActiveEntities iterators
    //If may happen that a pipe segment disappears.
    //If the first one disappears (==this), we need to do a replacement
    //If all disappear, we just return the content in an ArcParallel
    //Do one pass of checking
    EIterator ei_if_disappear = chart->AllEntities.Find_by_Name(NONE_ENT_STR);
    for (auto i = series.begin(); i!=series.end(); /*none*/) {
        EIterator sub1 = chart->FindWhoIsShowingInsteadOf((*i)->src, true);
        EIterator sub2 = chart->FindWhoIsShowingInsteadOf((*i)->dst, false);

        //if pipe segment spans a single entity and both ends have changed, 
        //we kill this segment
        if (sub1==sub2 && sub1!=(*i)->src && sub2!=(*i)->dst) {
            delete *i;
            series.erase(i++);
            ei_if_disappear = sub1;
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
    for (auto i = series.begin(); i!=series.end(); /*nope*/) {
        if ((*i)->src == (*i)->dst) {
            auto j = i;
            auto k = i;
            if ((i!=series.begin() && (*--j)->dst == (*i)->src) ||
                (i!=--series.end() && (*++k)->src == (*i)->dst)) {
                    delete *i;
                    series.erase(i++);
                    continue;
            } 
        }
        lw_max = std::max(lw_max, (*i)->style.line.LineWidth());
        i++;
    }

    //see if we have any segments left, if not return only content
    if (series.size()==0) {
        if (content.size()) {
            ArcParallel *p = new ArcParallel(chart);
            p->AddAttributeList(NULL);
            ArcList *al = new ArcList;
            al->swap(content);
            p->AddArcList(al);  //this will do a "delete al;"
            return p;
        }
        //We completely disappear due to entity collapses and have no content
        if ((*ei_if_disappear)->running_style.indicator.second) {
            //leave an indicator, but update left and right
            left = chart->EntityMinByPos(ei_if_disappear, left);
            right = chart->EntityMaxByPos(ei_if_disappear, right);
            ei_if_disappear = chart->FindWhoIsShowingInsteadOf(ei_if_disappear, true); //result points to ActiveEntities
            return new ArcIndicator(chart, ei_if_disappear, indicator_style, file_pos);  
        } else 
            return NULL;
    }

    //increase the radius everywhere by the thickest lw (if it is not zero)
    if ((*series.begin())->style.line.radius.second>0) {
        const double radius = (*series.begin())->style.line.radius.second + lw_max;
        for (auto i = series.begin(); i!=series.end(); i++)
            (*i)->style.line.radius.second = radius;
    }

    //Sort according to fromright: begin() should point to the leftmost pipe if side==right,
    //and to the rightmost if side=left
    if ((*series.begin())->style.side.second == SIDE_LEFT) {
        comp.fromright = false;
        series.sort(comp);
    }
    //Now series.begin does not point to the leftmost, but to the one showing the hole
    //Also set pipe_connect_back/forw flags
    //Terminology: backwards means left if "fromright" and right otherwise

    //Fill in pipe_connect vlaues
    (*series.begin())->pipe_connect_back = false;
    (*series.rbegin())->pipe_connect_forw = false;
    for (auto i = ++series.begin(); i!=series.end(); i++) {
        (*i)->pipe_connect_back = (*i)->pipe_connect_forw = false;
        //Set flags if we are adjacent to previous one
        auto i_prev = i;
        i_prev--;
        if (((*series.begin())->style.side.second == SIDE_RIGHT && (*i_prev)->dst == (*i)->src) ||
            ((*series.begin())->style.side.second == SIDE_LEFT  && (*i_prev)->src == (*i)->dst)) {
            (*i)->pipe_connect_back = true;
            (*i_prev)->pipe_connect_forw = true;
        } else {
            (*i)->pipe_connect_back = false;
            (*i_prev)->pipe_connect_forw = false;
        }
    }

    //set return value
    left = chart->EntityMinByPos(left,  (*series.begin())->src);
    left = chart->EntityMinByPos(left,  content_left);
    right= chart->EntityMaxByPos(right, (*series.rbegin())->dst);
    right= chart->EntityMaxByPos(right, content_right);

    if (hide) return NULL;
    //if there is no content, no need to draw a transparent cover
    //save drawing cost (and potential fallback img)
    //only first pipe can have content (which becomes the content of all pipe)
    if (content.size() == 0)
        for (auto i = series.begin(); i!=series.end(); i++)
            (*i)->style.solid.second = 255;
    return this;
}

//will only be called for the first box of a multi-segment box series
void ArcPipeSeries::Width(EntityDistanceMap &distances)
{
    if (!valid) return;
    EntityDistanceMap d, d_pipe;
    if (content.size())
        chart->WidthArcList(content, d);

    const MscSideType side = (*series.begin())->style.side.second;
    const double radius = (*series.begin())->style.line.radius.second;

    //(*i)->src and dst contain the left and right end of a pipe
    //The order of the pipe segments in follow depends on style.side
    for (auto i = series.begin(); i!=series.end(); i++) {
        const double ilw = (*i)->style.line.LineWidth();
        const double width = (*i)->parsed_label.getTextWidthHeight().x + 2*chart->emphVGapInside;
        (*i)->left_space = d.Query((*(*i)->src)->index, DISTANCE_LEFT) + chart->emphVGapInside;
        (*i)->right_space = d.Query((*(*i)->dst)->index, DISTANCE_RIGHT) + chart->emphVGapInside;
        //Add extra space for curvature
        if (side == SIDE_RIGHT)
            (*i)->right_space += radius;
        else
            (*i)->left_space += radius;

        //The style.line.radius.second is understood to be the radius of the hole of the _outer edge_
        if ((*i)->src==(*i)->dst) {
            (*i)->left_space  = std::max(width/2, (*i)->left_space);
            (*i)->right_space = std::max(width/2, (*i)->right_space);
        } else {
            //keep a big enough space between src and dst for the text + curvature of pipe
            //the text can go out of the entity lines, all the way to the left-space
            d_pipe.Insert((*(*i)->src)->index, (*(*i)->dst)->index,
                                width - (*i)->left_space - (*i)->right_space +
                                radius);
        }
        (*i)->left_space  = ceil((*i)->left_space);
        (*i)->right_space = ceil((*i)->right_space);
        bool connect_left = side == SIDE_RIGHT ? (*i)->pipe_connect_back : (*i)->pipe_connect_forw;
        bool connect_right = side == SIDE_RIGHT ? (*i)->pipe_connect_forw : (*i)->pipe_connect_back;
        //Check if we are connecting to a neighbour pipe segment
        if (connect_left)
            (*i)->left_space = 0;
        else
            d_pipe.Insert((*(*i)->src)->index, DISTANCE_LEFT,
                                (*i)->left_space + ilw + radius);
        //add shadow to the right size only if we are the rightmost entity
        double shadow_to_add = 0;
        if ((side == SIDE_RIGHT && i==--series.end()) ||
            (side == SIDE_LEFT  && i==series.begin()))
            shadow_to_add = (*i)->style.shadow.offset.second;
        if (connect_right)
            (*i)->right_space = 0;
        else
            d_pipe.Insert((*(*i)->dst)->index, DISTANCE_RIGHT,
                                (*i)->right_space + ilw + radius + shadow_to_add);
    }
    d_pipe.CombineLeftRightToPair_Sum(chart->hscaleAutoXGap);
    distances += d_pipe;

    //Finally add the requirements of the content
    d.CombineLeftRightToPair_Max(chart->hscaleAutoXGap, chart->activeEntitySize/2);
    d.CombineLeftRightToPair_Single(chart->hscaleAutoXGap);
    d.CopyBoxSideToPair(chart->hscaleAutoXGap);
    distances += d;
}

double ArcPipeSeries::Height(AreaList &cover)
{
    if (!valid) return 0;
    //Collect cover information from labels and linewidth, so compression of content arrows can be done
    //Determine thickest line for precise pipe alignment
    double max_lineWidth = 0;
    for (auto i = series.begin(); i!=series.end(); i++)
        max_lineWidth = std::max(max_lineWidth, (*i)->style.line.LineWidth());
    double lowest_line_bottom =  max_lineWidth + chart->emphVGapInside;
    //Determine highest label and collect all text covers
    //Also calcualte all x positioning
    double lowest_label_bottom = 0;
    double lowest_opaque_label_bottom = 0;
    Area label_covers;
    //A few shortcuts. "side" and "radius" must be the same in any pipe element, so we take the first
    const MscSideType side = (*series.begin())->style.side.second;
    const double radius = (*series.begin())->style.line.radius.second;
    for (auto i = series.begin(); i!=series.end(); i++) {
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
        switch (side) {
        case SIDE_RIGHT: (*i)->dx_text -= radius; break;
        case SIDE_LEFT:  (*i)->sx_text += radius; break;
        }
        (*i)->text_cover = (*i)->parsed_label.Cover((*i)->sx_text, (*i)->dx_text, (*i)->y_text);
        // omit text cover for pipes if the pipe is fully opaque,
        // in that case content can be drawn at same position as label - opaque pipe will cover anyway
        double y = (*i)->y_text + (*i)->parsed_label.getTextWidthHeight().y;
        if (y == (*i)->y_text && content.size()==0)
            y += Label("M", chart->GetCanvas(), (*i)->style.text).getTextWidthHeight().y;
        if ((*i)->style.solid.second < 255) {
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
    AreaList content_cover;
    if (content.size())
        y = ceil(chart->PlaceListUnder(content.begin(), content.end(), ceil(y),
                                        lowest_line_bottom, label_covers, false, &content_cover));
    //now y contains the bottom of the content arrows (if any),
    //adjust if an opaque pipe's label was not yet considered in y
    y = std::max(y, lowest_opaque_label_bottom);
    y += chart->emphVGapInside + max_lineWidth;
    //now y contains the bottommost pixel of the pipe itself
    total_height = y = ceil(y);
    //Now set the y coordinate in all segments
    //Also calculate the Contours that will be used for drawing
    double max_offset = 0;
    for (auto i = series.begin(); i!=series.end(); i++) {
        const double ilw = (*i)->style.line.LineWidth();

        //fill in pipe_block.y (both are integer)
        (*i)->pipe_block.y.from = chart->emphVGapOutside;
        (*i)->pipe_block.y.till = y;

        XY cs((*i)->pipe_block.x.from, (*i)->pipe_block.y.MidPoint());
        XY cd((*i)->pipe_block.x.till, (*i)->pipe_block.y.MidPoint());
        const XY rad(radius, (*i)->pipe_block.y.Spans()/2); //we use the first pipe's line.radius
        if (side == SIDE_LEFT) std::swap(cs, cd); //use the first pipe's fromright, not (*i)->fromright
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
            if (content.size() && (*i)->style.solid.second < 255)
                (*i)->area_draw = (*i)->area.CreateExpand(chart->trackFrameWidth);
            //We take a big, unshrunken back-end out of fill: (this will be the next segments outer edge)
            if (rad.x>0) {
                (*i)->pipe_body_fill -= forw_end;
                (*i)->area -= forw_end;
                //below we need to first expand forw_end before substracting it
                //the other way is not ok: Expand fails in expanding negative arcs
                if (content.size() && (*i)->style.solid.second < 255) {
                    (*i)->area_draw -= forw_end.CreateExpand(-chart->trackFrameWidth);
                    (*i)->area_draw *= Contour(side == SIDE_RIGHT ? 0 : chart->total.x, cd.x,
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
                                                side == SIDE_RIGHT ? 270 : 90,
                                                side == SIDE_RIGHT ? 90 : 270);
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
            if (content.size() && (*i)->style.solid.second < 255)
                (*i)->area_draw = (*i)->area.CreateExpand(chart->trackFrameWidth);
        }
        //Finalize covers
        (*i)->area.arc = *i;
        if (content.size() && (*i)->style.solid.second < 255) {
            //Make a frame, add it to the already added label
            (*i)->area_draw -= (*i)->area;
            (*i)->area_draw += (*i)->text_cover.CreateExpand(chart->trackExpandBy);
            (*i)->area_draw.arc = *i;
            (*i)->area_draw_is_frame = true;
            (*i)->draw_is_different = true;
        }
        //now determine the cover to be used for placement
        const double offset = (*i)->style.shadow.offset.second;
        if (offset)
            cover += (*i)->pipe_shadow + (*i)->pipe_shadow.CreateShifted(XY(offset, offset));
        else
            cover += (*i)->pipe_shadow;
        max_offset = std::max(max_offset, offset);
        //merge shadows of connected previous segment to ours
        if ((*i)->pipe_connect_back) {
            auto i_neigh = i; i_neigh--;
            (*i_neigh)->pipe_shadow = ((*i)->pipe_shadow + (*i_neigh)->pipe_shadow).GetFirst();
            (*i)->pipe_shadow.clear();
        }
    }
    for (auto i = series.begin(); i!=series.end(); i++)
        (*i)->pipe_shadow = (*i)->pipe_shadow.CreateExpand(-(*i)->style.line.width.second/2);
    //Expand cover, but not content
    cover = cover.CreateExpand(chart->compressGap/2);
    //Add content to cover (may "come out" from pipe)
    cover += content_cover;
    //If we have valid content, set mainline to that of the content
    if (content.size() && !content_cover.mainline.IsInvalid()) 
        cover.mainline += content_cover.mainline;
    else  //else use the top and bottom of the pipe
        cover.mainline = Range(chart->emphVGapOutside, total_height);  //totalheight includes the top emphvgapoutside 
    return yPos + total_height + max_offset + chart->emphVGapOutside;
}


void ArcPipe::ShiftBy(double y)
{
    if (!valid) return;
    if (y==0) return;
    ArcLabelled::ShiftBy(y);
    y_text += y;
    text_cover.Shift(XY(0,y));
    pipe_block.Shift(XY(0,y));
    pipe_shadow.Shift(XY(0,y));
    pipe_body_line.Shift(XY(0,y));
    pipe_whole_line.Shift(XY(0,y));
    pipe_hole_line.Shift(XY(0,y));
    pipe_body_fill.Shift(XY(0,y));
    pipe_hole_fill.Shift(XY(0,y));
    pipe_hole_curve.Shift(XY(0,y));
}

void ArcPipeSeries::ShiftBy(double y)
{
    if (!valid) return;
    if (y==0) return;
    for (auto i=series.begin(); i!=series.end(); i++) 
        (*i)->ShiftBy(y);    
    if (content.size())
        chart->ShiftByArcList(content.begin(), content.end(), y);
    ArcBase::ShiftBy(y);
}

void ArcPipeSeries::PostPosProcess(double autoMarker)
{
    if (!valid) return;
    //For pipes we first add those covers to chart->AllCovers that are not fully opaque,
    //then the content (only in the first segment)
    //then those segments, which are fully opaque
    //(this is because search is backwards and this arrangement fits the visual best
    for (auto i = series.begin(); i!=series.end(); i++)
        if ((*i)->valid && (*i)->style.solid.second < 255)
            (*i)->ArcLabelled::PostPosProcess(autoMarker);
    if (content.size())
        chart->PostPosProcessArcList(content, autoMarker);
    for (auto i = series.begin(); i!=series.end(); i++)
        if ((*i)->valid && (*i)->style.solid.second == 255)
            (*i)->ArcLabelled::PostPosProcess(autoMarker);
    for (auto i = series.begin(); i!=series.end(); i++)
        chart->HideEntityLines((*i)->pipe_shadow);
}

//Draw a pipe, this is called for each segment, bool params dictate which part
//topside is the bigger part of the pipe
//backside is the small oval visible form the back of the pipe
//this->yPos is the outer edge of the top line
//this->left_space and right_space includes linewidth
void ArcPipe::DrawPipe(bool topSideFill, bool topSideLine, bool backSide, 
                       bool shadow, bool text, double next_lw, 
                       int drawing_variant)
{
    if (shadow) {
        //Shadow under the whole pipe
        chart->GetCanvas()->Shadow(pipe_shadow, style.shadow);
    }
    if (backSide) {
        //The back of the main pipe
        MscFillAttr fill = style.fill;
        //backside is always filled fully opaque
        fill.color.second.a = 255;
        fill.color2.second.a = 255;
        chart->GetCanvas()->Fill(pipe_body_fill, fill);
        //the back of the small ellipsis visible from the side
        if (fill.gradient.second == GRADIENT_UP)
            fill.gradient.second = GRADIENT_DOWN;
        else if (fill.gradient.second == GRADIENT_DOWN)
            fill.gradient.second = GRADIENT_UP;
        chart->GetCanvas()->Fill(pipe_hole_fill, fill);
        //Draw the backside line
        chart->GetCanvas()->Line(pipe_hole_curve, style.line);
    }
    if (topSideFill) {
        //apply the transparency of the solid attribute
        MscFillAttr fill = style.fill;
        fill.color.second.a = unsigned(style.solid.second) * unsigned(fill.color.second.a) / 255;
        chart->GetCanvas()->Fill(pipe_body_fill, fill);
    }
    if (topSideLine) {
        cairo_line_join_t t = chart->GetCanvas()->SetLineJoin(CAIRO_LINE_JOIN_BEVEL);
        const double x = style.side.second == SIDE_RIGHT ? pipe_block.x.till : pipe_block.x.from;
        Contour clip(x, style.side.second == SIDE_LEFT ? chart->total.x : 0, 0, chart->total.y);
        if (style.line.radius.second>0 && pipe_connect_forw) {
            const XY c(x, pipe_block.y.MidPoint());
            clip = (clip-Contour(c, style.line.radius.second-next_lw/2, pipe_block.y.Spans()/2.-next_lw/2)).GetFirst();
        }
        chart->GetCanvas()->Clip(clip);
        if (!style.line.IsDoubleOrTriple() || drawing_variant==0)
            chart->GetCanvas()->Line(pipe_body_line, style.line); //basic variant, in case of double & triple, lines cross
        else if (style.line.IsDouble()) {
            chart->GetCanvas()->SetLineAttr(style.line);
            if (drawing_variant==1) { //advanced: lines do not cross
                chart->GetCanvas()->singleLine(pipe_whole_line.CreateExpand(style.line.DoubleSpacing()), style.line);
                chart->GetCanvas()->singleLine(pipe_body_line.CreateExpand(-style.line.DoubleSpacing()), style.line);
                chart->GetCanvas()->singleLine(pipe_hole_line.CreateExpand(-style.line.DoubleSpacing()), style.line);
            } else { //very advanced: proper double line joint
                chart->GetCanvas()->singleLine(pipe_whole_line.CreateExpand(style.line.DoubleSpacing()), style.line); //outer
                chart->GetCanvas()->singleLine(pipe_body_line.CreateExpand(-style.line.DoubleSpacing()) -
                                  pipe_hole_line.CreateExpand(style.line.DoubleSpacing()), style.line);  //inner body
                chart->GetCanvas()->singleLine(pipe_hole_line.CreateExpand(-style.line.DoubleSpacing()), style.line); //inner hole
            }
        } else if (style.line.IsTriple()) {
            chart->GetCanvas()->SetLineAttr(style.line);
            //here variant 1 and 2 result in the same
            chart->GetCanvas()->singleLine(pipe_whole_line.CreateExpand(style.line.TripleSpacing()), style.line);  //outer
            chart->GetCanvas()->singleLine(pipe_body_line.CreateExpand(-style.line.TripleSpacing()) -
                                pipe_hole_line.CreateExpand(style.line.TripleSpacing()), style.line);  //inner body
            chart->GetCanvas()->singleLine(pipe_hole_line.CreateExpand(-style.line.TripleSpacing()), style.line);   //inner hole
            cairo_set_line_width(chart->GetCanvas()->GetContext(), style.line.TripleMiddleWidth());
            chart->GetCanvas()->singleLine(pipe_body_line, style.line); //middle line
        }
        chart->GetCanvas()->UnClip();
        chart->GetCanvas()->SetLineJoin(t);
    }
    if (text)
        parsed_label.Draw(chart->GetCanvas(), sx_text, dx_text, y_text);
}

void ArcPipeSeries::Draw()
{
    if (!valid) return;
    //First shadows
    for (auto i = series.begin(); i!=series.end(); i++)
        (*i)->DrawPipe(false, false, false, true, false, 0, drawing_variant);  //dummy 0
    for (auto i = series.begin(); i!=series.end(); i++) {
        //Dont draw the topside fill
        //Draw the topside line only if pipe is fully transparent. Else we may cover the line.
        //Draw the backside in any case.
        //Do not draw text
        auto i_next = i; i_next++;
        const double next_linewidth = i_next!=series.end() ? (*i_next)->style.line.width.second : 0;
        (*i)->DrawPipe(false, (*i)->style.solid.second == 0, true, false, false, next_linewidth, drawing_variant);
        if (content.size() && (*i)->drawEntityLines)
            chart->DrawEntityLines(yPos, total_height, (*i)->src, ++EIterator((*i)->dst));
    }
    if (content.size())
        chart->DrawArcList(content);
    for (auto i = series.begin(); i!=series.end(); i++) {
        //Draw the topside fill only if the pipe is not fully transparent.
        //Draw the topside line in any case
        //Do not draw the backside (that may content arrow lines already drawn)
        //Draw the text
        auto i_next = i; i_next++;
        const double next_linewidth = i_next!=series.end() ? (*i_next)->style.line.width.second : 0;
        (*i)->DrawPipe((*i)->style.solid.second > 0, true, false, false, true, next_linewidth, drawing_variant);
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
    Design().styles["divider"].AttributeNames(csh);
}

bool ArcDivider::AttributeValues(const std::string attr, Csh &csh, bool nudge)
{
    if (nudge) return false;
    if (Design().styles["divider"].AttributeValues(attr, csh)) return true;
    if (ArcLabelled::AttributeValues(attr, csh)) return true;
    return false;
}

ArcBase* ArcDivider::PostParseProcess(bool hide, EIterator &left, EIterator &right, Numbering &number, bool top_level)
{
    if (!valid) return NULL;
    //Add numbering, if needed
    ArcLabelled::PostParseProcess(hide, left, right, number, top_level);

    if (!top_level && (type==MSC_ARC_DISCO || type==MSC_ARC_DIVIDER)) {
        string ss;
        ss << (type==MSC_ARC_DISCO ? "'...'" : "'---'") << " is specified inside a parallel block.";
        chart->Error.Warning(file_pos.start, ss, "May display incorrectly.");
    }
    if (hide) return NULL;
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
    const double charheight = Label("M", chart->GetCanvas(), style.text).getTextWidthHeight().y;

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
    cover = area.CreateExpand(chart->compressGap/2);
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
    parsed_label.Draw(chart->GetCanvas(), text_margin, chart->total.x-text_margin, yPos + (wide ? 0 : chart->arcVGapAbove));
    //determine widest extent for coverage at the centerline+-style.line.LineWidth()/2;
    const double lw2 = ceil(style.line.LineWidth()/2.);
    Block b(line_margin, chart->total.x-line_margin, yPos + centerline - lw2, yPos + centerline + lw2);
    Range r = (text_cover * b).GetBoundingBox().x;
    if (r.IsInvalid())
        chart->GetCanvas()->Line(XY(line_margin, yPos + centerline), XY(chart->total.x-line_margin, yPos + centerline), style.line);
    else {
        chart->GetCanvas()->Line(XY(line_margin, yPos + centerline), XY(r.from-chart->emphVGapInside, yPos + centerline), style.line);
        chart->GetCanvas()->Line(XY(r.till+chart->emphVGapInside, yPos + centerline), XY(chart->total.x-line_margin, yPos + centerline), style.line);
    }
}

//////////////////////////////////////////////////////////////////////////////////////

MscDirType ArcParallel::GetToucedEntities(class EntityList &el) const
{
    MscDirType dir = MSC_DIR_INDETERMINATE;
    for (auto i = blocks.begin(); i!=blocks.end(); i++)
        dir = chart->GetTouchedEntitiesArcList(**i, el, dir);
    return dir;
}

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

ArcBase* ArcParallel::PostParseProcess(bool hide, EIterator &left, EIterator &right, Numbering &number, bool top_level)
{
    if (!valid) return NULL;
    at_top_level = top_level;
    for (PtrList<ArcList>::iterator i=blocks.begin(); i != blocks.end(); i++)
        chart->PostParseProcessArcList(hide, **i, false, left, right, number, false);
    if (hide) return NULL;
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
    //Do not expand cover, it has already been expanded
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

//This is called only from "Combine" and Msc::PostParseProcess(), so definitely
//After AddAttrLits and before PostParseProcess, so here are the members
//with comments on how to merge a later entityDef into a former one.
//name                 //This is const, shall be the same
//label;               //This can only be set once: keep the former
//linenum_label_value; //pos of label text, irrelevant in later one
//pos;                 //THis is used only in AddAttributeList
//rel;                 //THis is used only in AddAttributeList
//collapsed;           //THis is used only in AddAttributeList
//show;                //the latter shall overwrite the former one
//active;              //the latter shall overwrite the former one
//show_is_explicit;    //ignore, This is only used in ApplyPrefix which is only called during parse
//itr;                 //this is set during PostParse, ignore
//style;               //this is finalized during PostParse, combine latter into former
//parsed_label;        //will be set during PostParse, ignore
//defining;            //keep former
//shown;               //ignore, will be set in PostParse
void CommandEntity::AppendToEntities(const EntityDefList &e)
{
    for (auto i = e.begin(); i!=e.end(); i++) {
        auto i2 = entities.begin();
        for (/*nope*/; i2!=entities.end(); i2++)
            if ((*i2)->name == (*i)->name) break;
        if (i2 == entities.end()) {
            entities.Append(*i);
        } else {
            (*i2)->style += (*i)->style;
            if ((*i)->show.first) 
                (*i2)->show = (*i)->show;
            if ((*i)->active.first) 
                (*i2)->active = (*i)->active;
        }
    }
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
		} else if (CaseInsensitiveEqual(prefix, "activate") || CaseInsensitiveEqual(prefix, "deactivate")) {
            if ((*i)->active_is_explicit) continue;
			(*i)->active.first = true;	
			(*i)->active.second = CaseInsensitiveEqual(prefix, "activate");
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

ArcBase* CommandEntity::PostParseProcess(bool hide, EIterator &left, EIterator &right, Numbering &number,
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
                    ed->file_pos = (*i_def)->file_pos; 
                    ed->itr = jj_ent;
                    ed->style = (*jj_ent)->running_style;
                    ed->parsed_label.Set((*jj_ent)->label, chart->GetCanvas(), ed->style.text);
                    entities.Append(ed);
                }
            }
            (*i_def)->show.first = false;
        }
    }
    //Next apply the style changes of this command to the running style of the entities
    for (auto i_def = entities.begin(); i_def != entities.end(); i_def++) {
        const EIterator j_ent = (*i_def)->itr;
        //Make the style of the entitydef fully specified using the accumulated style info in Entity
        (*j_ent)->running_style += (*i_def)->style;  //(*i)->style is a partial style here specified by the user
        if ((*i_def)->active.first)
            (*j_ent)->running_shown.Activate((*i_def)->active.second);
    }
    //Now remove grouped entities (we have handled style and show for them)
    //and calculate shown status for non-grouped ones
    for (auto i_def = entities.begin(); i_def != entities.end(); /*nope*/) 
        if ((*(*i_def)->itr)->children_names.size()) {
            entities.erase(i_def++);
        } else {
            EIterator j_ent = (*i_def)->itr;
            //Decide, if this entitydef will show an entity or not
            //It can get drawn because we 1) said show=yes, or
            //2) because it is on, we mention it (without show=yes) and it is
            //a full heading.
            (*i_def)->shown = ((*i_def)->show.second && (*i_def)->show.first) 
                              || (full_heading && (*j_ent)->running_shown.IsOn());
            //Adjust the running status of the entity, this is valid *after* this command. 
            //This is just for the Height process knwos whch entity is on/off
            if ((*i_def)->show.first)
                (*j_ent)->running_shown.Show((*i_def)->show.second);
            if ((*i_def)->shown) {
                explicitly_listed.insert(*(*i_def)->itr);
                //Update the style of the entitydef
                (*i_def)->style = (*j_ent)->running_style;	 //(*i)->style now become the full style to use from this point
                (*i_def)->parsed_label.Set((*j_ent)->label, chart->GetCanvas(), (*i_def)->style.text);
            }
            i_def++; //cycle to next in for-cycle
        }

    //A "heading" command, we have to draw all entities that are on
    //for these we create additional EntityDefs and append them to entities
    //Only do this for children (non-grouped entities)
    if (full_heading)
        for (auto i = chart->AllEntities.begin(); i!=chart->AllEntities.end(); i++) {
            if (!(*i)->running_shown.IsOn()) continue;
            if ((*i)->children_names.size()) continue;
            if (explicitly_listed.find(*i) != explicitly_listed.end()) continue;
            EntityDef *e = new EntityDef((*i)->name.c_str(), chart);
            //fill in all values necessary
            e->itr = i;
            e->file_pos = file_pos; //use the file_pos of the CommandEntity
            e->style = (*i)->running_style;
            e->parsed_label.Set((*i)->label, chart->GetCanvas(), e->style.text);
            e->shown = true;
            entities.Append(e);
        }
    //At this point the list contains all non-grouped entities with show=yes
    //Collapse is not considered yet, we will do it below

    //Now add parents to the list for those children that are visible
    //Maintain reverse ordering: parents later, children earlier in the list
    std::list<pair<string, file_line_range>> sl;
    for (auto i_def = entities.begin(); i_def != entities.end(); i_def++) {
        if (!(*i_def)->shown) continue;
        const string &myparent = (*(*i_def)->itr)->parent_name;
        if (myparent.length()==0) continue;
        auto si = sl.begin();
        for(/*nope*/; si!=sl.end(); si++) 
            if (si->first == myparent) break;
        if (si != sl.end()) continue;
        si = sl.begin();
        while (si != sl.end() && !chart->IsMyParentEntity(myparent, si->first)) si++;
        sl.insert(si, pair<string, file_line_range>(myparent, (*i_def)->file_pos));
    }
    //Now add an entitydef for each parent (whith shown=yes) and reverse order
    for (auto i = sl.begin(); i!=sl.end(); i++) {
        EntityDef *e = new EntityDef(i->first.c_str(), chart);
        //fill in all values necessary
        e->itr = chart->AllEntities.Find_by_Name(i->first);
        e->file_pos = i->second;
        e->style = (*e->itr)->running_style;
        e->parsed_label.Set((*e->itr)->label, chart->GetCanvas(), e->style.text);
        e->shown = true;
        entities.Prepend(e);
    }

    //Finally prune the list: remove those that shall not be displayed due to collapse
    for (auto i_def = entities.begin(); i_def != entities.end(); /*nope*/) 
        if (chart->FindActiveParentEntity((*i_def)->itr) != (*i_def)->itr) 
            entities.erase(i_def++);
        else
            i_def++;
    //Now we have all entities among "entities" that will show here
    //Go through them and update left, right and the entities' maxwidth
    for (auto i_def = entities.begin(); i_def != entities.end(); i_def++) {
        if (!(*i_def)->shown) continue;
        left =  chart->EntityMinByPos(left,  chart->FindWhoIsShowingInsteadOf((*i_def)->itr, true));
        right = chart->EntityMaxByPos(right, chart->FindWhoIsShowingInsteadOf((*i_def)->itr, false));
        const EIterator ei = (*i_def)->itr;
        double w = (*i_def)->Width();
        if ((*ei)->maxwidth < w) (*(*i_def)->itr)->maxwidth = w;
    }
    hidden = hide;
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
    if (!valid || hidden) return;
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
            //and expand us by linewidth and space
            const EIterator j_ent = (*i)->itr;
            double expand = chart->emphVGapInside + (*i)->style.line.LineWidth();
            (*i)->left_ent = chart->FindWhoIsShowingInsteadOf(j_ent, true);
            (*i)->right_ent= chart->FindWhoIsShowingInsteadOf(j_ent, false);
            (*i)->left_offset = dist[(*(*i)->left_ent)->index].first += expand; 
            (*i)->right_offset = dist[(*(*i)->right_ent)->index].second += expand; 
            //If this is a group entity containing only one element, ensure its label fit
            if ((*i)->left_ent == (*i)->right_ent) {
                if (dist[(*(*i)->left_ent)->index].first < (*(*i)->itr)->maxwidth/2) {
                    dist[(*(*i)->left_ent)->index].first = (*(*i)->itr)->maxwidth/2;
                    (*i)->left_offset = (*(*i)->itr)->maxwidth/2;
                }
                if (dist[(*(*i)->right_ent)->index].second < (*(*i)->itr)->maxwidth/2) {
                    dist[(*(*i)->right_ent)->index].second = (*(*i)->itr)->maxwidth/2;
                    (*i)->right_offset = (*(*i)->itr)->maxwidth/2;
                }
            } else {
                //Insert a requirement between left_ent and right_ent, so that our width will fit (e.g., long text)
                distances.Insert((*(*i)->left_ent)->index, (*(*i)->right_ent)->index,
                                 (*(*i)->itr)->maxwidth - (*i)->left_offset - (*i)->right_offset);
            }
        }
    }
    if (dist.size()) {
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
    //Now add some distances for activation (only for non-grouped or collapsed entities)
    for (auto i = entities.begin(); i!=entities.end(); i++) {
        if ((*(*i)->itr)->children_names.size() == 0 || (*(*i)->itr)->collapsed) {
            if ((*i)->show.first) 
                (*(*i)->itr)->running_shown.Show((*i)->show.second);
            if ((*i)->active.first) 
                (*(*i)->itr)->running_shown.Activate((*i)->active.second);
            if ((*(*i)->itr)->running_shown == EEntityStatus::SHOW_ACTIVE_ON) 
                distances.was_activated.insert((*(*i)->itr)->index);
        }
    }
}

double CommandEntity::Height(AreaList &cover)
{
    if (!valid || hidden) return height=0;
    Range hei(0,0);
    //Those entities explicitly listed will have their own EntityDef for this line.
    //Thus their area will be stored there and not in CommandEntity->area
    //But, still put those into "cover" so they can be considered for placement
    //There are other entities shown here, those triggered by a heading command.
    //They have no line info and they do not add
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
        //EntityDef::Height places children entities to yPos==0
        //Grouped entities may start at negative yPos.
        //We collect here the maximum extent
        //Note: Height() also adds the cover to the entitydef's area
        hei += (*i)->Height(cover, edl); 
        area += (*i)->GetAreaToSearch();
    }
    if (!hei.Spans()) return height = 0; //if no headings show
    //Ensure overall startpos is zero
    ShiftBy(-hei.from + chart->headingVGapAbove);
    cover.Shift(XY(0,-hei.from + chart->headingVGapAbove));
    cover = cover.CreateExpand(chart->compressGap/2);
    return height = chart->headingVGapAbove + hei.Spans() + chart->headingVGapBelow;
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
    ArcCommand::PostPosProcess(autoMarker);
    for (auto i = entities.begin(); i!=entities.end(); i++)
        (*i)->PostPosProcess(autoMarker);
    if (height>0 && !hidden) {
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
ArcBase* CommandNumbering::PostParseProcess(bool hide, EIterator &left, EIterator &right, Numbering &number, bool top_level)
{
    if (!valid) return NULL;
    if (hide) hidden = true;
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
    ArcCommand(MSC_COMMAND_EMPTY, msc), parsed_label(msc->GetCanvas())
{
    StringFormat format;
    format.Default();
    format.Apply("\\pc\\mu(10)\\md(10)\\ml(10)\\mr(10)\\c(255,255,255)\\b\\i");
    parsed_label.Set(string("\\i\\bEmpty chart"), chart->GetCanvas(), format);
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
    Area a = Area(Block((chart->total.x-wh.x)/2, (chart->total.x+wh.x)/2, EMPTY_MARGIN_Y, EMPTY_MARGIN_Y+wh.y));
    cover = a.CreateExpand(chart->compressGap/2);
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

    chart->GetCanvas()->Shadow(b, line, shadow);
    chart->GetCanvas()->Fill(b, line, fill);
    chart->GetCanvas()->Line(b, line);
}
