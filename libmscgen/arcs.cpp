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
    TrackableElement(msc), valid(true), compress(false), parallel(false),
    draw_pass(DEFAULT), type(t)
{
    if (msc) compress = msc->Contexts.back().compress;
    had_add_attr_list = false;
}

inline Area ArcBase::GetCover4Compress(const Area &a) const
{
    Area ret(static_cast<const Contour &>(a).CreateExpand(chart->compressGap/2, 
                                             contour::EXPAND_MITER_SQUARE, 
                                             contour::EXPAND_MITER_SQUARE, 
                                             1, 1), a.arc);
    ret.mainline = a.mainline;
    return ret;
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

template<> const char EnumEncapsulator<ArcBase::DrawPassType>::names[][ENUM_STRING_LEN] =
    {"invalid", "before_entity_lines", "after_entity_lines", "default", "after_default", ""};


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
    if (a.Is("draw_time")) {
        if (!a.EnsureNotClear(chart->Error, STYLE_ARC)) return true;
        if (a.type == MSC_ATTR_STRING && Convert(a.value, draw_pass)) return true;
        a.InvalidValueError(CandidatesFor(draw_pass), chart->Error);
        return true;
    }
    return false;
}

void ArcBase::AttributeNames(Csh &csh)
{
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "compress", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "parallel", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "draw_time", HINT_ATTR_NAME));
}

bool ArcBase::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr,"compress")||
        CaseInsensitiveEqual(attr,"parallel")) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "yes", HINT_ATTR_VALUE));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "no", HINT_ATTR_VALUE));
        return true;
    }
    if (CaseInsensitiveEqual(attr,"draw_time")) {
        csh.AddToHints(EnumEncapsulator<DrawPassType>::names, csh.HintPrefix(COLOR_ATTRVALUE), 
                       HINT_ATTR_VALUE);
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

void ArcBase::PostPosProcess(MscCanvas &canvas, double autoMarker)
{
    _ASSERT(had_add_attr_list);
    if (valid) 
        TrackableElement::PostPosProcess(canvas, autoMarker);
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
    ArcBase(MSC_ARC_INDICATOR, chart), style(st), src(s), dst(s)
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

void ArcIndicator::Width(MscCanvas &/*canvas*/, EntityDistanceMap &distances)
{
    if (*src != *dst) return; //no width requirements for Indicators not exactly on an entity
    //If we are exactly on an entity line add left and right req for boxes potentially around us.
    const double width = indicator_style.line.LineWidth() + indicator_size.x/2;
    distances.InsertBoxSide((*src)->index-1, 0, width);
    distances.InsertBoxSide((*src)->index,   width, 0);
}

double ArcIndicator::Height(MscCanvas &/*canvas*/, AreaList &cover)
{
    yPos = chart->emphVGapOutside;
    const double x = (chart->XCoord((*src)->pos) + chart->XCoord((*dst)->pos))/2;
    const Block b = GetIndicatorCover(XY(x, chart->emphVGapOutside));
    area = b;
    Area tmp = area;
    if (style.shadow.offset.second>0)
        tmp += area.CreateShifted(XY(style.shadow.offset.second, style.shadow.offset.second));
    tmp.arc = this;
    tmp.mainline = Block(0,chart->total.x, b.y.from, b.y.till);
    cover = GetCover4Compress(std::move(tmp));
    return b.y.till + chart->emphVGapOutside;
}

void ArcIndicator::Draw(MscCanvas &canvas, DrawPassType pass) 
{
    if (pass!=draw_pass) return;
    const double x = (chart->XCoord((*src)->pos) + chart->XCoord((*dst)->pos))/2;
    DrawIndicator(XY(x, yPos), &canvas);
}

//////////////////////////////////////////////////////////////////////////////////////

//Take numbering style from the current context
ArcLabelled::ArcLabelled(MscArcType t, Msc *msc, const MscStyle &s) :
    ArcBase(t, msc), concrete_number(-1), style(s),
    numberingStyle(msc->Contexts.back().numberingStyle)
{
    SetStyleWithText();  //keep existing style, just combine with default text style, numbering and refinement
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

//set style to this name, but combine it with default text style
void ArcLabelled::SetStyleWithText(const char *style_name)
{
    const MscStyle *s = NULL;
    if (style_name)
        s = &chart->Contexts.back().styles[style_name];
    SetStyleWithText(s);
}

//set style to this name, but combine it with default text style
void ArcLabelled::SetStyleWithText(const MscStyle *style_to_use)
{
    if (style_to_use) 
        style = *style_to_use;
    //Make style.text complete using the default text formatting in the context as default
    StringFormat to_use(chart->Contexts.back().text);
    to_use += style.text;
    style.text = to_use;
    style.type = STYLE_ARC;
    //If style does not contain a numbering setting, apply the value of the
    //current chart option.
    if (!style.numbering.first) {
        style.numbering.first = true;
        style.numbering.second = chart->Contexts.back().numbering;
    }
    //Add refinement style (e.g., -> or ... or vertical++)
    const MscStyle *refinement = GetRefinementStyle(type); //virtual function for the arc
    if (refinement) style += *refinement;
}

const MscStyle *ArcLabelled::GetRefinementStyle(MscArcType t) const
{
    //refinement for all arrows, boxes and dividers
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
    case MSC_ARC_BIG_BIDIR:
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
    default:
        return NULL;
    };
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
        unsigned off = chart->Contexts.back().numberingStyle.Last().Input(a.value, num);
        //off is how many characters we could not understand at the end of a.value
        if (off == a.value.length()) {
            //No characters understood
            chart->Error.Error(a, true, "Value for 'number' must be 'yes', 'no' or a number. Ignoring attribute.");
            return true;
        }
        if (off > 0) {
            file_line l(a.linenum_value.start);
            l.col += (unsigned)a.value.length() - off;
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
    if (a.Is("draw_time")) {
        if (!a.EnsureNotClear(chart->Error, STYLE_ARC)) return true;
        if (a.type == MSC_ATTR_STRING && Convert(a.value, draw_pass)) return true;
        a.InvalidValueError(CandidatesFor(draw_pass), chart->Error);
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
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "draw_time", HINT_ATTR_NAME));
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
    if (CaseInsensitiveEqual(attr,"draw_time")) {
        csh.AddToHints(EnumEncapsulator<DrawPassType>::names, csh.HintPrefix(COLOR_ATTRVALUE), 
                       HINT_ATTR_VALUE);
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
ArcBase *ArcLabelled::PostParseProcess(MscCanvas &canvas, bool /*hide*/, EIterator &/*left*/, EIterator &/*right*/, Numbering &number, bool top_level)
{
    if (!valid) return NULL;
    //We do everything here even if we are hidden (numbering is not impacted by hide/show or collapse/expand)
    at_top_level = top_level;
    string num;
    string pre_num_post;
    if (label.length()!=0) {
        if (style.numbering.second) {
            number.SetSize((unsigned)numberingStyle.Size()); //append 1s if style has more levels
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
    parsed_label.Set(label, &canvas, style.text);
    return this;
}


/////////////////////////////////////////////////////

bool ArcArrow::AddAttribute(const Attribute &a)
{
    if (a.Is("angle")) {
        if (dynamic_cast<ArcSelfArrow*>(this)) 
            chart->Error.Warning(a, false, "Arrows pointing to the same entity cannot have an 'angle' attribute. Ignoring it.");
        else if (dynamic_cast<ArcVerticalArrow*>(this)) 
            chart->Error.Warning(a, false, "Verticals cannot have an 'angle' attribute. Ignoring it.");
        return true;
    }
    return ArcLabelled::AddAttribute(a);
}


void ArcArrow::AttributeNames(Csh &csh)
{
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "angle", HINT_ATTR_NAME));
    ArcLabelled::AttributeNames(csh);
    Design().styles["arrow"].AttributeNames(csh);
}

bool ArcArrow::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr,"angle")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", HINT_ATTR_VALUE, false));
        return true;
    }
    if (Design().styles["arrow"].AttributeValues(attr, csh)) return true;
    if (ArcLabelled::AttributeValues(attr, csh)) return true;
    return false;
}
//////////////////////////////////////////////////////////////////////////////////////

ArcSelfArrow::ArcSelfArrow(MscArcType t, const char *s, file_line_range sl,
                           Msc *msc, const MscStyle &st, double ys) :
    ArcArrow(t, msc, st), YSize(ys)
{
    src = chart->FindAllocEntity(s, sl);
}

ArcArrow * ArcSelfArrow::AddSegment(MscArcType, const char * /*m*/, file_line_range /*ml*/, file_line_range l)
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

ArcBase* ArcSelfArrow::PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right, Numbering &number, bool top_level)
{
    if (!valid) return NULL;
    if (chart->ErrorIfEntityGrouped(src, (*src)->file_pos)) return NULL;

    //Add numbering, if needed
    ArcLabelled::PostParseProcess(canvas, hide, left, right, number, top_level);

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

void ArcSelfArrow::Width(MscCanvas &/*canvas*/, EntityDistanceMap &distances)
{
    if (!valid) return;
    distances.Insert((*src)->index, DISTANCE_RIGHT, chart->XCoord(0.375)+src_act);
    distances.Insert((*src)->index, DISTANCE_LEFT, parsed_label.getTextWidthHeight().x+src_act);
}

double ArcSelfArrow::Height(MscCanvas &/*canvas*/, AreaList &cover)
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
    area.mainline = Block(0, chart->total.x, y - chart->nudgeSize/2, y + wh.y + chart->nudgeSize/2);

    cover = GetCover4Compress(area);
    return area.GetBoundingBox().y.till + chart->arcVGapBelow;
}

void ArcSelfArrow::PostPosProcess(MscCanvas &canvas, double autoMarker)
{
    if (!valid) return;
    ArcArrow::PostPosProcess(canvas, autoMarker);

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

void ArcSelfArrow::Draw(MscCanvas &canvas, DrawPassType pass)
{
    if (!valid) return;
    if (pass!=draw_pass) return;
    double y = yPos + chart->arcVGapAbove;

    parsed_label.Draw(&canvas, sx, dx-src_act, y);
    y += xy_s.y;

    if (style.line.radius.second < 0) {
        //draw an arc
        canvas.Line(Edge(XY(dx+src_act, y+YSize), wh.x, wh.y/2, 0, 270, 90), style.line);
    } else {
        //draw (part of) a rounded rectangle
        canvas.Clip(dx+src_act, chart->total.x, 0, chart->total.y);
        canvas.Line(Block(XY(0, y), XY(dx,y)+wh), style.line);
        canvas.UnClip();
    }
    //draw arrowheads
    style.arrow.Draw(XY(dx+src_act, y+2*YSize), 0, false, isBidir(), MSC_ARROW_END, style.line, style.line, &canvas);
    style.arrow.Draw(XY(dx+src_act, y        ), 0, true,  isBidir(), MSC_ARROW_START, style.line, style.line, &canvas);
}

//////////////////////////////////////////////////////////////////////////////////////

ArcDirArrow::ArcDirArrow(MscArcType t, const char *s, file_line_range sl,
                         const char *d, file_line_range dl, Msc *msc, bool fw, const MscStyle &st) :
    ArcArrow(t, msc, st), linenum_src(sl.start), linenum_dst(dl.start), specified_as_forward(fw), slant_angle(0)
{
    src = chart->FindAllocEntity(s, sl);
    dst = chart->FindAllocEntity(d, dl);
    modifyFirstLineSpacing = true;
    segment_types.push_back(t);
    if (chart) slant_angle = chart->Contexts.back().slant_angle;
};

ArcDirArrow::ArcDirArrow(const class EntityList &el, bool bidir, const ArcLabelled &al) :
    ArcArrow(bidir ? MSC_ARC_BIG_BIDIR : MSC_ARC_BIG, al), specified_as_forward(false), slant_angle(0)
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
    if (chart) slant_angle = chart->Contexts.back().slant_angle;
}

ArcArrow * ArcDirArrow::AddSegment(MscArcType t, const char *m, file_line_range ml, file_line_range /*l*/)
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
    //This will work even if we are a BigArrow
    MscStyle save(style);
    style.Empty();
    ArcArrow::AddAttributeList(l);
    for (unsigned i=0; i<segment_types.size(); i++) {
        segment_lines.push_back(save.line);
        const MscStyle * const refinement = GetRefinementStyle(segment_types[i]);
        if (refinement) 
            *segment_lines.rbegin() += refinement->line;
        *segment_lines.rbegin() += style.line;
    }
    save += style;
    style = save;
    return this;
}

bool ArcDirArrow::AddAttribute(const Attribute &a)
{
    if (a.Is("angle")) {
        if (!a.EnsureNotClear(chart->Error, STYLE_ARC)) return true;
        if (!a.CheckType(MSC_ATTR_NUMBER, chart->Error)) return true;
        if (a.number<0 || a.number>45) {
            string x;
            if (a.number<0) x = "0";
            if (a.number>45) x = "45";
            chart->Error.Error(a, true, "Using " + x + " degrees instead of the specified value.",
                "The slant angle must be between 0 and 45 degrees.");
            if (a.number<0) slant_angle = 0;
            else if (a.number>45) slant_angle = 45;
        } else
            slant_angle = a.number;
        return true;
    }
    return ArcArrow::AddAttribute(a);
}

void ArcDirArrow::AttributeNames(Csh &csh)
{
    ArcLabelled::AttributeNames(csh);
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "angle", HINT_ATTR_NAME));

}

bool ArcDirArrow::AttributeValues(const std::string attr, Csh &csh)
{
    if (ArcLabelled::AttributeValues(attr, csh)) return true;
    if (CaseInsensitiveEqual(attr,"angle")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number 0..45>", HINT_ATTR_VALUE, false));
        return true;
    }
    return false;
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

ArcBase *ArcDirArrow::PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right, Numbering &number, bool top_level)
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
        for (unsigned f = 0; f<middle.size(); f++)
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
    ArcLabelled::PostParseProcess(canvas, hide, left, right, number, top_level);

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
    for (unsigned ii = 0; ii<middle.size(); /*nope*/) {
        sub = chart->FindActiveParentEntity(middle[ii]);
        if (middle[ii] != sub) {
            //if the replacement parent equals to an end, delete it
            //sub now is an iterator to AllEntities, src and dst is to ActiveEntities
            if (*sub == *src || *sub == *dst) {
                erase:
                middle.erase(middle.begin()+ii);
                continue;
            }
            for (unsigned jj=0; jj<ii; jj++) 
                if (middle[jj] == sub) goto erase;
            middle[ii] = sub;
        }
        ii++;
    }
    //Replace middle[] values to point to ActiveEntities
    for (unsigned iii = 0; iii<middle.size(); iii++) {
        middle[iii] = chart->ActiveEntities.Find_by_Ptr(*middle[iii]);
        _ASSERT(middle[iii] != chart->ActiveEntities.end());
    }

    //set angle to zero if a bidirectional arrow and negative if a<-b
    if (isBidir()) slant_angle = 0;
    else if ((*src)->index > (*dst)->index) 
            slant_angle = -slant_angle;

    sin_slant = slant_angle ? sin(slant_angle*M_PI/180.) : 0.;
    cos_slant = slant_angle ? cos(slant_angle*M_PI/180.) : 1.;

    //record what entities are active at the arrow (already consider slant)
    act_size.clear();
    act_size.reserve(2+middle.size());
    act_size.push_back(std::max(0., (*src)->GetRunningWidth(chart->activeEntitySize)/2)/cos_slant);
    for (unsigned iiii = 0; iiii<middle.size(); iiii++) 
        act_size.push_back(std::max(0., cos_slant*(*middle[iiii])->GetRunningWidth(chart->activeEntitySize)/2)/cos_slant);
    act_size.push_back(std::max(0., cos_slant*(*dst)->GetRunningWidth(chart->activeEntitySize)/2)/cos_slant);
    //Insert a small extra spacing for the arrow line
    if (parsed_label.getTextWidthHeight().y && modifyFirstLineSpacing)
        parsed_label.AddSpacing(0, style.line.LineWidth()+
                                ARROW_TEXT_VSPACE_ABOVE+ARROW_TEXT_VSPACE_BELOW);
    return this;
}

void ArcDirArrow::Width(MscCanvas &canvas, EntityDistanceMap &distances)
{
    if (!valid) return;

    //Here we have a valid canvas, so we adjust act_size
    if (canvas.HasImprecisePositioning() && slant_angle==0)
        for (auto i = act_size.begin(); i!=act_size.end(); i++)
            *i = floor(*i);

    //we lie about us being forward (we do not check), so we know which of first/second to use
    DoublePair end = style.arrow.getWidths(true, isBidir(), MSC_ARROW_END, style.line);
    DoublePair start = style.arrow.getWidths(true, isBidir(), MSC_ARROW_START, style.line);
    distances.Insert((*src)->index, (*dst)->index,
                     (end.first + start.second + parsed_label.getTextWidthHeight().x +
                     *act_size.begin() + *act_size.rbegin())*cos_slant);
    //Add distances for arrowheads
    const bool fw = (*src)->index  <  (*dst)->index;
    distances.Insert((*src)->index, fw ? DISTANCE_RIGHT : DISTANCE_LEFT, (start.second + *act_size.begin())*cos_slant);
    distances.Insert((*dst)->index, fw ? DISTANCE_LEFT : DISTANCE_RIGHT, (end.first    + *act_size.rbegin())*cos_slant);

    if (middle.size()==0) return;
    EntityDistanceMap d;
    for (unsigned i=0; i<middle.size(); i++) {
        DoublePair mid = style.arrow.getWidths(fw, isBidir(), MSC_ARROW_MIDDLE, style.line);
        distances.Insert((*middle[i])->index, DISTANCE_LEFT,  (mid.first  + act_size[i+1])*cos_slant);
        distances.Insert((*middle[i])->index, DISTANCE_RIGHT, (mid.second + act_size[i+1])*cos_slant);
    }
    d.CombineLeftRightToPair_Sum(chart->hscaleAutoXGap);
    distances += d;
}

MscArrowEnd ArcDirArrow::WhichArrow(unsigned i)
{
    //in xPos (may not be filled up yet, index 0 will be the src and indes xpos.size()-1 will be dest
    //in between there will be middle.size() number of middle arrows.
    //"i" here is an index that will be used for xPos.
    if (i>0 && i<middle.size()+1) return MSC_ARROW_MIDDLE;
    if ((i==0) == ((*src)->index  <  (*dst)->index)) return MSC_ARROW_START;
    return MSC_ARROW_END;
}

double ArcDirArrow::Height(MscCanvas &/*canvas*/, AreaList &cover)
{
    if (!valid) return 0;
    yPos = 0;
    area.clear();
    sx = chart->XCoord(src);
    dx = chart->XCoord(dst);
    //convert dx to transformed space
    if (slant_angle)
        dx = sx + (dx-sx)/cos_slant;

    double lw_max = style.line.LineWidth();
    for (unsigned i=0; i<segment_lines.size(); i++)
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

        //Determine coverage for the text (or draw it if we already draw)
        if (sx<dx) {
            sx_text = sx + xy_s.x;
            dx_text = dx - xy_e.x;
        } else {
            sx_text = dx + xy_e.x;
            dx_text = sx - xy_s.x;
        }
        cx_text = (sx+dx)/2;
        area = text_cover = parsed_label.Cover(sx_text, dx_text, y, cx_text);
        //determine top edge position of arrow midline
        y += std::max(aH, firstLineHeight+ARROW_TEXT_VSPACE_ABOVE);
    } else {
        /* no text */
        y += aH;
    }
    centerline = y = ceil(y) + lw_max/2;
    //Note: When the angle is slanted, we rotate the space around "sx, centerline+yPos"

    //prepare xPos and margins
    xPos.clear(); xPos.reserve(2+middle.size());
    margins.clear(); margins.reserve(2+middle.size());
    xPos.push_back(sx);
    margins.push_back(style.arrow.getWidths(sx<dx, isBidir(), MSC_ARROW_START, style.line));
    for (unsigned i=0; i<middle.size(); i++) {
        xPos.push_back(sx + (chart->XCoord(middle[i])-sx)/cos_slant);
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
    Block total(sx + (sx<dx ? s_act : -s_act), dx - (sx<dx ? d_act : -d_act), 0, y+2*lw_max);
    clip_area  = style.arrow.ClipForLine(XY(sx, y), s_act, sx<dx, isBidir(), MSC_ARROW_START,
                                         total, *segment_lines.begin(), *segment_lines.begin());
    clip_area *= style.arrow.ClipForLine(XY(dx, y), d_act, sx<dx, isBidir(), MSC_ARROW_END,
                                         total, *segment_lines.rbegin(), *segment_lines.rbegin());
    for (unsigned i=1; i<xPos.size()-1; i++)
        clip_area *= style.arrow.ClipForLine(XY(xPos[i], y), act_size[i], sx<dx, isBidir(), MSC_ARROW_MIDDLE, 
                                             total, segment_lines[i-1], segment_lines[i]);

    //Add arrowheads and line segments to cover
    for (unsigned i=0; i<xPos.size(); i++)
        area += style.arrow.Cover(XY(xPos[i], y), act_size[i], sx<dx, isBidir(), WhichArrow(i),
        segment_lines[i - (i==0 ? 0 : 1)], segment_lines[i - (i==xPos.size()-1 ? 1 : 0)]);
    for (unsigned i=0; i<xPos.size()-1; i++) {
        const double lw2 = ceil(segment_lines[i].LineWidth()/2);
        //x coordinates below are not integer- but this will be merged with other contours - so they disappear
        area += clip_area * Block(xPos[i]+margins[i].second, xPos[i+1]-margins[i+1].first, y-lw2, y+lw2);
    }
    CalculateMainline(std::max(lw_max, chart->nudgeSize+1.0));
    if (slant_angle != 0) {
        //OK: all of sx, dx, sx_text, dx_text, cx_text, xPos, act_size, margins
        //are in the transformed space
        //Now we transfrom "area" and "text_cover", too
        const XY c(sx, yPos+centerline);
        area.RotateAround(c, slant_angle);
        text_cover.RotateAround(c, slant_angle); //not used for bigarrows so is empty
        clip_area.RotateAround(c, slant_angle); //not used for bigarrows so is empty
    }
    cover = GetCover4Compress(area);
    if (slant_angle==0)
        return std::max(y+max(aH, lw_max/2), chart->arcVGapAbove + text_wh.y) + chart->arcVGapBelow;
    return area.GetBoundingBox().y.till;
}


void ArcDirArrow::CalculateMainline(double thickness)
{
    if (slant_angle == 0) 
        area.mainline = Block(0, chart->total.x, yPos+centerline - thickness/2, yPos+centerline + thickness/2);
    else {
        thickness /= cos_slant;
        const double src_y = yPos+centerline;
        const double dst_y = sin_slant*(dx-sx) + src_y;
        const double real_dx = sx + cos_slant*(dx-sx);
        if (sx<dx) {
            const XY ml[] = {XY(0,   src_y-thickness/2), XY(sx, src_y-thickness/2),
                             XY(real_dx, dst_y-thickness/2), XY(chart->total.x, dst_y-thickness/2),
                             XY(chart->total.x, dst_y+thickness/2), XY(real_dx, dst_y+thickness/2),
                             XY(sx, src_y+thickness/2), XY(0, src_y+thickness/2)};
            area.mainline.assign_dont_check(ml);
        } else {
            const XY ml[] = {XY(0, dst_y-thickness/2), XY(real_dx, dst_y-thickness/2),
                             XY(sx, src_y-thickness/2), XY(chart->total.x, src_y-thickness/2),
                             XY(chart->total.x, src_y+thickness/2), XY(sx, src_y+thickness/2),
                             XY(real_dx, dst_y+thickness/2), XY(0, dst_y+thickness/2)};
            area.mainline.assign_dont_check(ml);
        }
    }
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
    for (unsigned i = 0; i<temp.size(); i++)
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
            for(unsigned i=0; i<ss.size()-2; i++)
                sss << ss[i] << ", ";
            sss << ss[ss.size()-2] << " and " << ss[ss.size()-1] << " are";
        }
        sss << " turned off, but referenced here.";
        chart->Error.Warning(linenum_temp[earliest], sss, "It will look strange.");
    }
}


void ArcDirArrow::PostPosProcess(MscCanvas &canvas, double autoMarker)
{
    if (!valid) return;
    ArcArrow::PostPosProcess(canvas, autoMarker);
    CheckSegmentOrder(yPos+centerline);
    //Exclude the areas covered by the text from entity lines
    chart->HideEntityLines(text_cover);
    //Exclude the areas covered by the arrow heads from entity lines
    const XY c(sx, yPos+centerline);
    Contour tmp;
    tmp = style.arrow.EntityLineCover(XY(sx, yPos+centerline), sx<dx, isBidir(), MSC_ARROW_START);
    tmp.RotateAround(c, slant_angle);
    chart->HideEntityLines(tmp);
    tmp = style.arrow.EntityLineCover(XY(dx, yPos+centerline), sx<dx, isBidir(), MSC_ARROW_END);
    tmp.RotateAround(c, slant_angle);
    chart->HideEntityLines(tmp);
    //for multi-segment arrows
    for (unsigned i=1; i<xPos.size()-1; i++) {
        tmp = style.arrow.EntityLineCover(XY(xPos[i], yPos+centerline), sx<dx, isBidir(), MSC_ARROW_MIDDLE);
        tmp.RotateAround(c, slant_angle);
        chart->HideEntityLines(tmp);
    }
}

void ArcDirArrow::Draw(MscCanvas &canvas, DrawPassType pass)
{
    if (!valid) return;
    if (pass!=draw_pass) return;
    const double y = yPos+centerline;  //should be integer
    if (parsed_label.getTextWidthHeight().y) {
        if (slant_angle)
            style.arrow.TransformCanvasForAngle(slant_angle, canvas, sx, y);
        parsed_label.Draw(&canvas, sx_text, dx_text, yPos + chart->arcVGapAbove, cx_text, slant_angle!=0);
        if (slant_angle)
            style.arrow.UnTransformCanvas(canvas);
    }
    /* Draw the line */
    //all the entities this (potentially multi-segment arrow visits)
    const int mul = (sx<dx) ? 1 : -1;
    if (!canvas.NeedsArrowFix()) canvas.Clip(clip_area);
    for (unsigned i=0; i<xPos.size()-1; i++) {
        double sta_x = xPos[i];
        double end_x = xPos[i+1];
        if (canvas.NeedsArrowFix()) {
            sta_x += +margins[i  ].second+act_size[i  ]*mul;
            end_x += -margins[i+1].first -act_size[i+1]*mul;
        }
        const XY sta(sx + (sta_x-sx)*cos_slant, y + (sta_x-sx)*sin_slant);
        const XY end(sx + (end_x-sx)*cos_slant, y + (end_x-sx)*sin_slant);
        canvas.Line(sta, end, segment_lines[i]);
    }
    if (!canvas.NeedsArrowFix()) canvas.UnClip();
    /* Now the arrow heads */
    if (slant_angle)
        style.arrow.TransformCanvasForAngle(slant_angle, canvas, sx, y);
    for (unsigned i=0; i<xPos.size(); i++)
        style.arrow.Draw(XY(xPos[i], y), act_size[i], sx<dx, isBidir(), WhichArrow(i), 
                         segment_lines[i - (i==0 ? 0 : 1)], segment_lines[i - (i==xPos.size()-1 ? 1 : 0)],
                         &canvas);
    if (slant_angle)
        style.arrow.UnTransformCanvas(canvas);
}

//////////////////////////////////////////////////////////////////////////////////////

ArcBigArrow::ArcBigArrow(const ArcDirArrow &dirarrow, const MscStyle &s) : 
 ArcDirArrow(dirarrow), sig(NULL)
{
    SetStyleWithText(&s);
    modifyFirstLineSpacing = false;
}

//This invocation is from ArcBoxSeries::PostParseProcess, so PPP will not be called
//on this arrow again. (But ArcLabelled::PPP was already called)
ArcBigArrow::ArcBigArrow(const EntityList &el, bool bidir, const ArcLabelled &al,
    const ArcSignature *s)
    : ArcDirArrow(el, bidir, al), sig(s)
{
    modifyFirstLineSpacing = false;
    slant_angle = 0;
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
    default:
        return NULL;
    }
}


void ArcBigArrow::AttributeNames(Csh &csh)
{
    ArcLabelled::AttributeNames(csh);
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "angle", HINT_ATTR_NAME));
    Design().styles["blockarrow"].AttributeNames(csh);
}

bool ArcBigArrow::AttributeValues(const std::string attr, Csh &csh)
{
    if (Design().styles["blockarrow"].AttributeValues(attr, csh)) return true;
    if (ArcLabelled::AttributeValues(attr, csh)) return true;
    if (CaseInsensitiveEqual(attr,"angle")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number 0..45>", HINT_ATTR_VALUE, false));
        return true;
    }
    return false;
}

string ArcBigArrow::Print(int ident) const
{
    return ArcDirArrow::Print(ident);
}

ArcBase* ArcBigArrow::PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right, Numbering &number, bool top_level)
{
    if (!valid) return NULL;
    //Determine src and dst entity, check validity of multi-segment ones, add numbering, etc
    ArcBase *ret = ArcDirArrow::PostParseProcess(canvas, hide, left, right, number, top_level);
    //Finally copy the line attribute to the arrow, as well (arrow.line.* attributes are annulled here)
    style.arrow.line = style.line;
    return ret;
}

void ArcBigArrow::Width(MscCanvas &canvas, EntityDistanceMap &distances)
{
    if (!valid) return;

    //fill an "indexes" and "act_size" array
    const bool fw = (*src)->index < (*dst)->index;
    std::vector<unsigned> indexes;
    indexes.reserve(2+middle.size());
    indexes.push_back((*src)->index);
    for (unsigned i=0; i<middle.size(); i++) 
        indexes.push_back((*middle[i])->index);
    indexes.push_back((*dst)->index);
    //Sort to make them be from left to right
    if (!fw) {
        std::reverse(indexes.begin(), indexes.end());
        std::reverse(act_size.begin(), act_size.end());
        std::reverse(segment_lines.begin(), segment_lines.end());
        std::reverse(segment_types.begin(), segment_types.end());
    }

    //Determine if there are multiple segments, 
    //if so add text into the appropriate one
    if (middle.size()>0 && !MSC_ARROW_IS_SYMMETRIC(style.arrow.midType.second)) {
        switch (style.text.GetIdent()) {
		default:
        case MSC_IDENT_LEFT:   stext = 0; break;
        case MSC_IDENT_CENTER: stext = (unsigned)indexes.size()/2-1; break;
        case MSC_IDENT_RIGHT:  stext = (unsigned)indexes.size()-2; break;
        }
        dtext = stext+1;
    } else {
        //if no segments, then span the whole arrow.
        stext = 0;
        dtext = (unsigned)indexes.size()-1;
    }

    //Set sy and dy
    //sy and dy are at the outer line of the line around the body.
    //We ensure that the outer edge of the body falls on an integer value
    const double aH = ceil(style.arrow.bigYExtent(isBidir(), fw, &segment_lines));
    XY twh = parsed_label.getTextWidthHeight();
    ind_off = twh.y + chart->emphVGapInside;
    if (sig && style.indicator.second) {
        twh.y += GetIndiactorSize().y + chart->emphVGapInside;
        twh.x = std::max(twh.x, GetIndiactorSize().x + 2*chart->emphVGapInside);
    } 
    twh.y = std::max(twh.y, Label("M", &canvas, style.text).getTextWidthHeight().y);
    sy = chart->arcVGapAbove + aH;
    dy = ceil(sy + twh.y + chart->emphVGapInside*2 + 2*segment_lines[stext].LineWidth());

    const MscArrowEnd e_left  = fw ? MSC_ARROW_START : MSC_ARROW_END;
    const MscArrowType t_left_end  = style.arrow.GetType(isBidir(), e_left);
    const MscArrowEnd e_right = fw ? MSC_ARROW_END : MSC_ARROW_START;
    const MscArrowType t_right_end = style.arrow.GetType(isBidir(), e_right);

    const double sp_left_end = style.arrow.getBigWidthsForSpace(
                                    isBidir(), t_left_end, e_left, dy-sy, 
                                    *act_size.begin(), *segment_lines.begin());
    const double sp_right_end = style.arrow.getBigWidthsForSpace(
                                    isBidir(), t_right_end, e_right, dy-sy, 
                                    *act_size.rbegin(), *segment_lines.rbegin());

    distances.Insert(*indexes.begin(), DISTANCE_RIGHT, sp_left_end*cos_slant);
    distances.Insert(*indexes.rbegin(), DISTANCE_LEFT, sp_right_end*cos_slant);

    //Collect iterators and distances into arrays
    margins.reserve(2+middle.size()); margins.clear();
    margins.push_back(DoublePair(0, sp_left_end));
    for (unsigned i=0; i<middle.size(); i++) {
        DoublePair mid;
        const MscArrowType t_left  = style.arrow.GetType(fw, isBidir(), MSC_ARROW_MIDDLE, true);
        const MscArrowType t_right = style.arrow.GetType(fw, isBidir(), MSC_ARROW_MIDDLE, false);
        mid.first = style.arrow.getBigWidthsForSpace(
                                    isBidir(), t_left, MSC_ARROW_MIDDLE, dy-sy, 
                                    act_size[i+1], segment_lines[i]);
        mid.second = style.arrow.getBigWidthsForSpace(
                                    isBidir(), t_right, MSC_ARROW_MIDDLE, dy-sy, 
                                    act_size[i+1], segment_lines[i+1]);
        margins.push_back(mid);
    }
    margins.push_back(DoublePair(sp_right_end, 0));

    for (unsigned i=0; i<indexes.size()-1; i++) {
        //if neighbours
        if (indexes[i] + 1 == indexes[i+1]) {
            distances.Insert(indexes[i], indexes[i+1],
                             (margins[i].second + margins[i+1].first + chart->compressGap)*cos_slant);
        } else {
            distances.Insert(indexes[i], indexes[i] + 1,
                             (margins[i].second + chart->compressGap)*cos_slant);
            distances.Insert(indexes[i+1]-1, indexes[i+1],
                             (margins[i+1].first + chart->compressGap)*cos_slant);
        }
    }

    //calculate text margin. segment_lines is now ordered from smaller x to larger x
    const Contour tcov = parsed_label.Cover(0, twh.x, sy + segment_lines[stext].LineWidth() + chart->emphVGapInside);
    const MscArrowType s_type = style.arrow.GetType(fw, isBidir(), WhichArrow(stext), false);
    const MscArrowType d_type = style.arrow.GetType(fw, isBidir(), WhichArrow(dtext), true);
    _ASSERT(s_type!=MSC_ARROW_INVALID && d_type!=MSC_ARROW_INVALID);

    sm = style.arrow.getBigMargin(tcov, sy, dy, true, isBidir(), s_type,
                                  segment_lines[stext]);
    dm = style.arrow.getBigMargin(tcov, sy, dy, false, isBidir(), d_type,
                                  segment_lines[stext]);
    distances.Insert(indexes[stext], indexes[dtext], 
        (sm + twh.x + dm + act_size[stext] +act_size[dtext])*cos_slant);
}


double ArcBigArrow::Height(MscCanvas &/*canvas*/, AreaList &cover)
{
    if (!valid) return 0;
    yPos = 0;

    //Reuse sy and dy set in Width()
    centerline = (sy+dy)/2; //Note that we rotate around (sx, yPos+centerline)

    //set sx and dx
    sx = chart->XCoord(src);
    dx = chart->XCoord(dst);
    //convert dx to transformed space
    if (slant_angle)
        dx = sx + (dx-sx)/cos_slant;

    //prepare xPos (margins were already done in Width)
    xPos.clear(); xPos.reserve(2+middle.size());
    xPos.push_back(sx);
    for (unsigned i=0; i<middle.size(); i++)
        xPos.push_back(sx + (chart->XCoord(middle[i])-sx)/cos_slant);
    xPos.push_back(dx);
    if (sx >= dx) 
        std::reverse(xPos.begin(), xPos.end());
    //calculate text margings
    sx_text = xPos[stext] + sm + act_size[stext];
    dx_text = xPos[dtext] - dm - act_size[dtext];
    cx_text = (xPos[stext] + xPos[dtext])/2;
    //text_cover = parsed_label.Cover(sx_text, dx_text, sy+style.line.LineWidth()/2 + chart->emphVGapInside, cx_text);
    area = style.arrow.BigContour(xPos, act_size, sy, dy, sx<dx, isBidir(), &segment_lines, outer_contours);
    area.arc = this;
    //due to thick lines we can extend above y==0. Shift down to avoid it
    if (area.GetBoundingBox().y.from < chart->arcVGapAbove)
        ShiftBy(-area.GetBoundingBox().y.from + chart->arcVGapAbove);

    CalculateMainline(chart->nudgeSize);
    if (slant_angle != 0) {
        //OK: all of sx, dx, sx_text, dx_text, cx_text, xPos, act_size, margins
        //are in the transformed space
        //Now we transfrom "area" and "text_cover", too
        const XY c(sx, yPos+centerline);
        area.RotateAround(c, slant_angle);
        text_cover.RotateAround(c, slant_angle); 
    }
    cover = GetCover4Compress(area);
    return area.GetBoundingBox().y.till + chart->arcVGapBelow + style.shadow.offset.second;
}

void ArcBigArrow::ShiftBy(double y)
{
    if (!valid) return;
    sy += y;
    dy += y;
    for (auto i = outer_contours.begin(); i!=outer_contours.end(); i++)
        i->Shift(XY(0,y));
    ArcDirArrow::ShiftBy(y); //This shifts clip_area, too, but that shall be empty anyway
}

void ArcBigArrow::PostPosProcess(MscCanvas &canvas, double autoMarker)
{
    if (!valid) return;
    CheckSegmentOrder(yPos + centerline);
    if (!valid) return;
    if (sig) {
        controls.push_back(MSC_CONTROL_EXPAND);        
        controls.push_back(MSC_CONTROL_COLLAPSE);        
    }
    ArcArrow::PostPosProcess(canvas, autoMarker); //Skip ArcDirArrow
    const XY c(sx, yPos+centerline);
    for (auto i = outer_contours.begin(); i!=outer_contours.end(); i++) {
        Contour tmp(*i);
        tmp.RotateAround(c, slant_angle);
        chart->HideEntityLines(tmp);
    }
}

void ArcBigArrow::Draw(MscCanvas &canvas, DrawPassType pass)
{
    if (!valid) return;
    if (pass!=draw_pass) return;
    if (slant_angle)
        style.arrow.TransformCanvasForAngle(slant_angle, canvas, sx, yPos+centerline);
    style.arrow.BigDrawFromContour(outer_contours, &segment_lines, style.fill, style.shadow, canvas, slant_angle*M_PI/180.);
    parsed_label.Draw(&canvas, sx_text, dx_text, sy+segment_lines[stext].LineWidth() + chart->emphVGapInside, cx_text);
    if (sig && style.indicator.second)
        DrawIndicator(XY(cx_text, sy+segment_lines[stext].LineWidth() + chart->emphVGapInside+ind_off), &canvas);
    if (slant_angle)
        style.arrow.UnTransformCanvas(canvas);
}

//////////////////////////////////////////////////////////////////////////////////////

VertXPos::VertXPos(Msc&m, const char *e1, file_line_range e1l,
                   const char *e2, file_line_range e2l, postype p, double off)
{
    valid = true;
    pos = p;
    offset = off;
    entity1 = m.FindAllocEntity(e1, e1l);
    e1line = e1l;
    if (pos == POS_CENTER) {
        entity2 = m.FindAllocEntity(e2, e2l);
        e2line = e2l;
    } else {
        entity2 = m.AllEntities.Find_by_Ptr(m.NoEntity);
        _ASSERT(entity2 != m.AllEntities.end());
    }
}

VertXPos::VertXPos(Msc&m, const char *e1, file_line_range e1l, postype p, double off)
{
    valid = true;
    pos = p;
    offset = off;
    entity1 = m.FindAllocEntity(e1, e1l);
    e1line = e1l;
    entity2 = m.AllEntities.Find_by_Ptr(m.NoEntity);
    _ASSERT(entity2 != m.AllEntities.end());
}

VertXPos::VertXPos(Msc&m)
{
    valid = true;
    pos = POS_INVALID;
    offset = 0;
    entity1 = m.AllEntities.Find_by_Ptr(m.NoEntity);
    _ASSERT(entity1 != m.AllEntities.end());
    entity2 = m.AllEntities.Find_by_Ptr(m.NoEntity);
    _ASSERT(entity2 != m.AllEntities.end());
}

double VertXPos::CalculatePos(Msc &chart, double width, double aw) const
{
    double xpos = chart.XCoord(entity1);
    const double gap = chart.hscaleAutoXGap;
    if (aw<0) aw = gap;
    switch (pos) {
    default:
    case VertXPos::POS_AT: break;
    case VertXPos::POS_CENTER:      xpos = (xpos + chart.XCoord(entity2))/2; break;
    case VertXPos::POS_LEFT_BY:     xpos -= width/2 + aw + gap; break;
    case VertXPos::POS_RIGHT_BY:    xpos += width/2 + aw + gap; break;
    case VertXPos::POS_LEFT_SIDE:   xpos -= width/2 + gap; break;
    case VertXPos::POS_RIGHT_SIDE:  xpos += width/2 + gap; break;
    };
    return xpos + offset;
}


ArcVerticalArrow::ArcVerticalArrow(MscArcType t, const char *s, const char *d, Msc *msc) :
    ArcArrow(t, msc, msc->Contexts.back().styles["vertical"]), pos(*msc), ypos(2)
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
    SetStyleWithText("vertical");
    return this;
}

ArcArrow *ArcVerticalArrow::AddSegment(MscArcType, const char * /*m*/, file_line_range /*ml*/, file_line_range l)
{
    if (!valid) return this; //display error only once
    chart->Error.Error(l.start, "Cannot add further segments to vertical arrow. Ignoring it.");
    valid = false;
    return this;
}

const MscStyle *ArcVerticalArrow::GetRefinementStyle(MscArcType t) const
{
    switch(type) {
    case MSC_ARC_SOLID:
    case MSC_ARC_SOLID_BIDIR:
        return &chart->Contexts.back().styles["vertical->"];
    case MSC_ARC_DOTTED:
    case MSC_ARC_DOTTED_BIDIR:
        return &chart->Contexts.back().styles["vertical>"];
    case MSC_ARC_DASHED:
    case MSC_ARC_DASHED_BIDIR:
        return &chart->Contexts.back().styles["vertical>>"];
    case MSC_ARC_DOUBLE:
    case MSC_ARC_DOUBLE_BIDIR:
        return &chart->Contexts.back().styles["vertical=>"];
    case MSC_EMPH_SOLID:
        return &chart->Contexts.back().styles["vertical--"];
    case MSC_EMPH_DASHED:
        return &chart->Contexts.back().styles["vertical++"];
    case MSC_EMPH_DOTTED:
        return &chart->Contexts.back().styles["vertical.."];
    case MSC_EMPH_DOUBLE:
        return &chart->Contexts.back().styles["vertical=="];
    default:
        return NULL;
    }
}

bool ArcVerticalArrow::AddAttribute(const Attribute &a)
{
    if (a.Is("ofsset")) {
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
    //csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"offset", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"makeroom", HINT_ATTR_NAME));
}

bool ArcVerticalArrow::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr,"offset")) {
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


ArcBase* ArcVerticalArrow::PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right,
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
    ArcLabelled::PostParseProcess(canvas, hide, left, right, number, top_level);

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

void ArcVerticalArrow::Width(MscCanvas &canvas, EntityDistanceMap &distances)
{
    if (!valid) return;
    //No extra space requirement
    if (!makeroom) return;
    //The offset is ignored during the process of setting space requirements
    const XY twh = parsed_label.getTextWidthHeight();
    const double lw = style.line.LineWidth();
    double width = twh.y;
    if (width==0)
        width = Label("M", &canvas, style.text).getTextWidthHeight().y;
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
double ArcVerticalArrow::Height(MscCanvas &/*canvas*/, AreaList &)
{
    return 0;
}

void ArcVerticalArrow::ShiftBy(double y)
{
    yPos += y;
}

void ArcVerticalArrow::PostPosProcess(MscCanvas &canvas, double autoMarker)
{
    if (!valid) return;
    ArcArrow::PostPosProcess(canvas, autoMarker);
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

    const bool forward = ypos[0] < ypos[1];

    //if (style.side.second == SIDE_RIGHT)
    //    swap(ypos[0], ypos[1]);
    if (!forward)
        swap(ypos[0], ypos[1]);

    const double lw = style.line.LineWidth();
    const XY twh = parsed_label.getTextWidthHeight();
    const Contour text_cover = parsed_label.Cover(0, twh.x, lw);

    const double sm = style.arrow.getBigMargin(text_cover, 0, twh.y+2*lw, style.side.second == SIDE_LEFT,  
                                               isBidir(), style.arrow.startType.second, style.line);
    const double dm = style.arrow.getBigMargin(text_cover, 0, twh.y+2*lw, style.side.second != SIDE_LEFT, 
                                               isBidir(), style.arrow.endType.second, style.line);
    const double ss = style.arrow.getBigWidthsForSpace(isBidir(), style.arrow.startType.second, MSC_ARROW_START, 
                                                       twh.y+2*lw, 0, style.line);
    const double ds = style.arrow.getBigWidthsForSpace(isBidir(), style.arrow.endType.second, MSC_ARROW_END, 
                                                       twh.y+2*lw, 0, style.line);

    if (sm + twh.x + dm  > ypos[1]-ypos[0])
        chart->Error.Warning(file_pos.start, "Size of vertical element is smaller than needed for text.",
                                 "May look strange.");
    if (ss+ds+chart->compressGap > ypos[1]-ypos[0]) {
        chart->Error.Warning(file_pos.start, "Size of vertical element is too small to draw arrowheads. Ignoring it.");
        valid = false;
    }

    //calculate xpos and width
    width = twh.y;
    if (width==0)
        width = Label("M", &canvas, style.text).getTextWidthHeight().y;
    width = ceil(width + 2*lw + 2*chart->emphVGapInside);
    width += fmod_negative_safe(width, 2.); //width is even integer now: the distance from outer edge to outer edge

    const double aw = style.arrow.bigYExtent(isBidir(), false)/2;
    xpos = pos.CalculatePos(*chart, width, aw);
    xpos = floor(xpos + offset + 0.5); //xpos is integer now: the centerline of arrow
    width -= lw; //not necessarily integer, the distance from midline to midline

    if (style.side.second == SIDE_LEFT) {
        sy_text = ypos[0] + (forward ? sm : dm);
        dy_text = ypos[1] - (forward ? dm : sm);
    } else {
        sy_text = ypos[1] - (forward ? sm : dm);
        dy_text = ypos[0] + (forward ? dm : sm);
    }

	//Generate area
    std::vector<double> act_size(2, 0);
    //use inverse of forward, swapXY will do the job
    area = style.arrow.BigContour(ypos, act_size, xpos-width/2, xpos+width/2, 
                                  forward, isBidir(), NULL, outer_contours);
    area.SwapXY();
    for (auto i = outer_contours.begin(); i!=outer_contours.end(); i++)
        i->SwapXY();
    //Expand area and add us to chart's all covers list
    ArcArrow::PostPosProcess(canvas, autoMarker);
}


void ArcVerticalArrow::Draw(MscCanvas &canvas, DrawPassType pass)
{
    if (!valid) return;
    if (pass!=draw_pass) return;

    //Draw background
    std::vector<double> act_size(2,0);
    style.arrow.BigDrawFromContour(outer_contours, NULL, style.fill, style.shadow, canvas);

    if (style.side.second == SIDE_LEFT)
        canvas.Transform_Rotate90(xpos-width/2, xpos+width/2, false);
    else
        canvas.Transform_Rotate90(ypos[0], ypos[1], true);
    const Contour lab = parsed_label.Cover(ypos[0], ypos[1],
                                        xpos-width/2+style.line.LineWidth()/2+chart->emphVGapInside,
                                        -1, true);
    //We skip BigDrawEmptyMid. as there can not be mid-stops
    parsed_label.Draw(&canvas, min(sy_text, dy_text), max(sy_text, dy_text),
                      xpos-width/2+style.line.LineWidth()/2+chart->emphVGapInside, -1, true);
    canvas.UnTransform();
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
    collapsed(BOX_COLLAPSE_EXPAND), drawEntityLines(true), content(NULL)
{
    src = chart->FindAllocEntity(s, sl);
    dst = chart->FindAllocEntity(d, dl);

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
    ArcBase(MSC_EMPH_SOLID, first->chart), series(true), drawing_variant(1)
{
    series.Append(first);
    draw_pass = first->draw_pass;
}


ArcBox* ArcBox::AddArcList(ArcList*l)
{
    if (!valid) return this;
    if (l!=NULL && l->size()>0) {
        content.insert(content.end(), l->begin(), l->end());
        l->clear(); //so that l's constructor does not delete Arcs in arclist
        delete l;
    }
    SetStyleWithText("box");
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
    if (collapsed==BOX_COLLAPSE_COLLAPSE) 
        SetStyleWithText("box_collapsed");
    else if (collapsed==BOX_COLLAPSE_BLOCKARROW) 
        SetStyleWithText("box_collapsed_arrow");
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
    default:
        _ASSERT(0);
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

ArcBase* ArcBox::PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right,
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
        return ret->PostParseProcess(canvas, hide, left, right, number, top_level);
    }
    ArcLabelled::PostParseProcess(canvas, hide, left, right, number, top_level);
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
        chart->PostParseProcessArcList(canvas, hide_i, content, false, 
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

ArcBase* ArcBoxSeries::PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right,
                                        Numbering &number, bool top_level)
{
    if (!valid || series.size()==0) return NULL;
    //If first segment is compressed or parallel, copy that to full series
    compress = (*series.begin())->compress;
//    parallel = (*series.begin())->parallel;

    ArcBase *ret = this;
    EIterator src, dst;
    dst = src = chart->AllEntities.Find_by_Name(NONE_ENT_STR);
    for (auto i = series.begin(); i!=series.end(); i++) {
        if (i!=series.begin() && (*i)->parallel) {
            chart->Error.Error((*i)->file_pos.start,
                               "Attribute 'parallel' can only be specified in the first "
                               "element in a box series. Ignoring it in subsequent ones.");
        }
        if (i!=series.begin() && (*i)->draw_pass!=DEFAULT) {
            chart->Error.Error((*i)->file_pos.start,
                               "Attribute 'draw_time' can only be specified in the first "
                               "element in a box series. Ignoring it in subsequent ones.");
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
        ret = (*i)->PostParseProcess(canvas, hide, src, dst, number, top_level); //ret is an arcblockarrow if we need to collapse
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
void ArcBoxSeries::Width(MscCanvas &canvas, EntityDistanceMap &distances)
{
    if (!valid) return;
    const MscStyle &overall_style = (*series.begin())->style;
    const EIterator src = (*series.begin())->src;
    const EIterator dst = (*series.begin())->dst;

    EntityDistanceMap d;
    double max_width = 0; //the widest label plus margins
    for (auto i = series.begin(); i!=series.end(); i++) {
        if ((*i)->content.size())
            chart->WidthArcList(canvas, ((*i)->content), d);
        double width = (*i)->parsed_label.getTextWidthHeight().x;
        //calculated margins (only for first segment) and save them
        if (i==series.begin()) {
            const Contour tcov = (*i)->parsed_label.Cover(0, width, overall_style.line.LineWidth()+chart->emphVGapInside);
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

double ArcBoxSeries::Height(MscCanvas &canvas, AreaList &cover)
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
    yPos = y;
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
        AreaList content_cover = Area((*i)->text_cover);
        if ((*i)->content.size()) {
            const double &radius = main_style.line.radius.second;
            Contour limit = (*i)->text_cover;
            if (i==series.begin() && main_style.line.corner.second != CORNER_NONE && radius>0) {
                //Funnily shaped box, prevent content from hitting it
                const Block b(sx-lw/2, dx+lw/2, y+lw/2, y + std::max(lw+radius*4, dx-sx)); //at midpoint of line
                limit += Contour(sx-lw/2, dx+lw/2, 0, y+lw+radius) - 
                         main_style.line.CreateRectangle_InnerEdge(b);
            }
            y = chart->PlaceListUnder(canvas, (*i)->content.begin(), (*i)->content.end(),
                                        y+th, y, Area(std::move(limit)), compress, &content_cover);  //no extra margin below text
        } else {
            y += th; //no content, just add textheight
        }
        if (i==--series.end() && main_style.line.corner.second != CORNER_NONE && main_style.line.radius.second>0) {
            //Funnily shaped box, prevent it content from hitting the bottom of the content
            MscLineAttr limiter_line(main_style.line);
            limiter_line.radius.second += chart->compressGap;
            const Block b(sx-lw/2, dx+lw/2, -limiter_line.radius.second*2, y);
            const Contour bottom = Contour(sx-lw/2, dx+lw/2, limiter_line.radius.second+1, y+1) -
                                   limiter_line.CreateRectangle_InnerEdge(b);
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

    Block b(sx-lw/2, dx+lw/2, yPos+lw/2, yPos + total_height - lw/2);  //The midpoint of us
    //update the style so that it's radius is not bigger than it can be
    const_cast<double&>(main_style.line.radius.second) = main_style.line.SaneRadius(b);
    
    Area overall_box(main_style.line.CreateRectangle_OuterEdge(b), this);
    // now we have all geometries correct, now calculate areas and covers
    for (auto i = series.begin(); i!=series.end(); i++) {
        (*i)->area = Contour(sx-lw, dx+lw, (*i)->yPos, (*i)->yPos + (*i)->height_w_lower_line) * overall_box;
        (*i)->area.arc = *i;
        if ((*i)->content.size() && (*i)->collapsed==BOX_COLLAPSE_EXPAND) {
            //Make a frame, add it to the already added label
            (*i)->area_draw = (*i)->area.CreateExpand(chart->trackFrameWidth) - (*i)->area;
            (*i)->area_draw += (*i)->text_cover.CreateExpand(chart->trackExpandBy);
            (*i)->draw_is_different = true;
            (*i)->area_draw_is_frame = true;
        } else {
            (*i)->area_draw.clear();
            (*i)->draw_is_different = false;
            (*i)->area_draw_is_frame = false;
        }
    }
    const double &offset = main_style.shadow.offset.second;
    if (offset)
        overall_box += overall_box.CreateShifted(XY(offset, offset));
    overall_box.mainline = Block(0, chart->total.x, b.y.from, b.y.till);
    cover = GetCover4Compress(overall_box);
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

void ArcBoxSeries::PostPosProcess(MscCanvas &canvas, double autoMarker)
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
            (*i)->ArcLabelled::PostPosProcess(canvas, autoMarker);
            if ((*i)->content.size()) 
                chart->PostPosProcessArcList(canvas, (*i)->content, autoMarker);
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
        Block b(src_x - left_space + lw/2, dst_x + right_space - lw/2,
                yPos + lw/2, yPos+total_height - lw/2); //The midpoint of the lines
        //The radius specified in style.line will be that of the midpoint of the line
        chart->HideEntityLines(main_style.line.CreateRectangle_OuterEdge(b) - 
                               main_style.line.CreateRectangle_InnerEdge(b));
    }
}

void ArcBoxSeries::Draw(MscCanvas &canvas, DrawPassType pass)
{
    if (!valid) return;
    if (pass!=draw_pass) return;
    //For boxes draw background for each segment, then separator lines, then bounding rectangle lines, then content
    const MscStyle &main_style = (*series.begin())->style;
    const double lw = main_style.line.LineWidth();
    const double src_x = chart->XCoord((*series.begin())->src);
    const double dst_x = chart->XCoord((*series.begin())->dst);
    //The midpoint of the lines
    const Block r(src_x - left_space + lw/2, dst_x + right_space - lw/2, 
                  yPos + lw/2, yPos+total_height - lw/2); 
    //The radius specified in main_style.line will be that of the midpoint of the line
    //First draw the shadow.
    canvas.Shadow(r, main_style.line, main_style.shadow);
    //Do a clip region for the overall box (for round/bevel/note corners)
    //at half a linewidth from the inner edge (use the width of a single line!)
    canvas.Clip(main_style.line.CreateRectangle_ForFill(r));
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
        canvas.Fill(Block(r.x.from, r.x.till+lw, sy, dy), (*i)->style.fill);
        //if there are contained entities, draw entity lines, strictly from inside of line
        if ((*i)->content.size()) {
            chart->DrawArcList(canvas, (*i)->content, BEFORE_ENTITY_LINES);
            if ((*i)->collapsed==BOX_COLLAPSE_EXPAND && (*i)->drawEntityLines)
                chart->DrawEntityLines(canvas, (*i)->yPos, (*i)->height + (*i)->style.line.LineWidth(), (*i)->src, ++EIterator((*i)->dst));
            chart->DrawArcList(canvas, (*i)->content, AFTER_ENTITY_LINES);
        }
    }
    canvas.UnClip();
    //Draw box lines - Cycle only for subsequent boxes
    for (auto i = ++series.begin(); i!=series.end(); i++) {
        const double y = (*i)->yPos + (*i)->style.line.LineWidth()/2;
        const XY magic(1,0);  //XXX needed in windows
        canvas.Line(XY(r.x.from, y)-magic, XY(r.x.till, y), (*i)->style.line);
    }
    //Finally draw the overall line around the box
    canvas.Line(r, main_style.line);
    //XXX double line joints: fix it
    for (auto i = series.begin(); i!=series.end(); i++) {
        (*i)->parsed_label.Draw(&canvas, (*i)->sx_text, (*i)->dx_text, (*i)->y_text, r.x.MidPoint());
        if ((*i)->content.size()) 
            chart->DrawArcList(canvas, (*i)->content, DEFAULT);
        //if (i==follow.begin()) {
        //    const Area tcov = (*i)->parsed_label.Cover(0, (*i)->parsed_label.getTextWidthHeight().x, style.line.LineWidth()+chart->emphVGapInside);
        //    DoublePair margins = style.line.CalculateTextMargin(tcov, 0, follow.size()==1?chart:NULL);
        //}
    }
    for (auto i = series.begin(); i!=series.end(); i++) 
        if ((*i)->content.size()) 
            chart->DrawArcList(canvas, (*i)->content, AFTER_DEFAULT);
}

/////////////////////////////////////////////////////////////////

ArcPipe::ArcPipe(ArcBox *box) :
    ArcLabelled(box->type, box->chart, box->chart->Contexts.back().styles["pipe"]),
    src(box->src), dst(box->dst), drawEntityLines(false)
{
    delete box;
    switch (type) {
    case MSC_EMPH_SOLID:
        style += chart->Contexts.back().styles["pipe--"]; break;
    case MSC_EMPH_DASHED:
        style += chart->Contexts.back().styles["pipe++"]; break;
    case MSC_EMPH_DOTTED:
        style += chart->Contexts.back().styles["pipe.."]; break;
    case MSC_EMPH_DOUBLE:
        style += chart->Contexts.back().styles["pipe=="]; break;
    default:
        _ASSERT(0);
    }
}

ArcPipeSeries::ArcPipeSeries(ArcPipe *first) :
    ArcBase(MSC_EMPH_SOLID, first->chart), series(true), drawing_variant(1)
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

ArcBase* ArcPipeSeries::PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right,
                                        Numbering &number, bool top_level)
{
    if (!valid) return NULL;

    //Add numbering, if needed 
    for (auto i = series.begin(); i!=series.end(); i++) 
        (*i)->PostParseProcess(canvas, hide, left, right, number, top_level);
    //Postparse the content;
    EIterator content_left, content_right;
    content_right = content_left = chart->AllEntities.Find_by_Name(NONE_ENT_STR);
    chart->PostParseProcessArcList(canvas, hide, content, false, content_left, content_right, number, top_level);

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
        for (auto i = series.begin(); i!=--series.end(); i++) {
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
                if (i!=series.begin() && (*i)->draw_pass!=DEFAULT) {
                    chart->Error.Error((*i)->file_pos.start,
                        "Attribute 'draw_time' can only be specified in the first "
                        "element in a pipe series. Ignoring it in subsequent ones.");
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
    else if (canvas.HasLowPerformaceTransparency())
        for (auto i = series.begin(); i!=series.end(); i++)
            //disallow transparency if too low power
            if ((*i)->style.solid.second!=255) 
                (*i)->style.solid.second = 0;
    return this;
}

//will only be called for the first box of a multi-segment box series
void ArcPipeSeries::Width(MscCanvas &canvas, EntityDistanceMap &distances)
{
    if (!valid) return;
    EntityDistanceMap d, d_pipe;
    if (content.size())
        chart->WidthArcList(canvas, content, d);

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

double ArcPipeSeries::Height(MscCanvas &canvas, AreaList &cover)
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
    Area label_covers(this);
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
        default: _ASSERT(0);
        }
        (*i)->text_cover = (*i)->parsed_label.Cover((*i)->sx_text, (*i)->dx_text, (*i)->y_text);
        // omit text cover for pipes if the pipe is fully opaque,
        // in that case content can be drawn at same position as label - opaque pipe will cover anyway
        double y = (*i)->y_text + (*i)->parsed_label.getTextWidthHeight().y;
        if (y == (*i)->y_text && content.size()==0)
            y += Label("M", &canvas, (*i)->style.text).getTextWidthHeight().y;
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
        y = ceil(chart->PlaceListUnder(canvas, content.begin(), content.end(), ceil(y),
                                        lowest_line_bottom, label_covers, false, &content_cover));
    //now y contains the bottom of the content arrows (if any),
    //adjust if an opaque pipe's label was not yet considered in y
    y = std::max(y, lowest_opaque_label_bottom);
    y += chart->emphVGapInside + max_lineWidth;
    //now y contains the bottommost pixel of the pipe itself
    total_height = y = ceil(y);
    //Now set the y coordinate in all segments
    //Also calculate the Contour that will be used for drawing
    double max_offset = 0;
    Area pipe_body_cover(this);
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
            (*i)->pipe_hole_curve.clear();
        } else {
            //No connection, we draw this end, too
            if (rad.x>0) {
                (*i)->pipe_body_fill -= forw_end.CreateExpand(-(*i)->style.line.width.second/2.);
                (*i)->pipe_whole_line = (*i)->pipe_body_line + forw_end.CreateExpand(gap_for_line);
                (*i)->pipe_body_line -= forw_end.CreateExpand(gap_for_line);

                //for shaodow we add
                (*i)->pipe_shadow += forw_end;
                (*i)->pipe_hole_line = forw_end.CreateExpand(gap_for_line);
                (*i)->pipe_hole_fill = forw_end.CreateExpand(gap_for_fill);
                const Edge hole_line[2] = {Edge(cd, rad.x+gap_for_line, rad.y+gap_for_line, 0, 90, 270), 
                                           Edge(cd, rad.x+gap_for_line, rad.y+gap_for_line, 0, 270, 90)};
                (*i)->pipe_hole_curve.assign_dont_check(hole_line);
                //this is only half of the hole ellipsos
                (*i)->pipe_hole_curve[0][side == SIDE_RIGHT ? 0 : 1].visible = false;
            } else {
                //just chop off from fill and line
                (*i)->pipe_body_fill -= Block(Range(cd.x, cd.x), (*i)->pipe_block.y).Expand(gap_for_fill);
                (*i)->pipe_body_line -= Block(Range(cd.x, cd.x), (*i)->pipe_block.y).Expand(gap_for_line);
                (*i)->pipe_whole_line = (*i)->pipe_body_line;
                //we clear the holes: no need, body will draw the line we need
                (*i)->pipe_hole_fill.clear();
                (*i)->pipe_hole_line.clear();
                (*i)->pipe_hole_curve.clear(); //wont draw anything
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
            (*i)->area_draw_is_frame = true;
            (*i)->draw_is_different = true;
        }
        //now determine the cover to be used for placement
        const double offset = (*i)->style.shadow.offset.second;
        if (offset)
            pipe_body_cover += (*i)->pipe_shadow + (*i)->pipe_shadow.CreateShifted(XY(offset, offset));
        else
            pipe_body_cover += (*i)->pipe_shadow;
        max_offset = std::max(max_offset, offset);
        //merge shadows of connected previous segment to ours
        if ((*i)->pipe_connect_back) {
            auto i_neigh = i; i_neigh--;
            (*i_neigh)->pipe_shadow = ((*i)->pipe_shadow + (*i_neigh)->pipe_shadow);
            (*i)->pipe_shadow.clear();
        }
    }
    for (auto i = series.begin(); i!=series.end(); i++)
        (*i)->pipe_shadow = (*i)->pipe_shadow.CreateExpand(-(*i)->style.line.width.second/2);
    //Add content to cover (may "come out" from pipe)
    cover += content_cover;
    //If we have no valid content, set mainline to that of pipe, else the content's mainline will be used
    if (content_cover.mainline.IsEmpty()) 
        pipe_body_cover.mainline = Block(0, chart->total.x, chart->emphVGapOutside, total_height);  //totalheight includes the top emphvgapoutside 
    //Expand cover, but not content (that is already expanded)
    cover += GetCover4Compress(pipe_body_cover);
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

void ArcPipeSeries::PostPosProcess(MscCanvas &canvas, double autoMarker)
{
    if (!valid) return;
    //For pipes we first add those covers to chart->AllCovers that are not fully opaque,
    //then the content (only in the first segment)
    //then those segments, which are fully opaque
    //(this is because search is backwards and this arrangement fits the visual best
    for (auto i = series.begin(); i!=series.end(); i++)
        if ((*i)->valid && (*i)->style.solid.second < 255)
            (*i)->ArcLabelled::PostPosProcess(canvas, autoMarker);
    if (content.size())
        chart->PostPosProcessArcList(canvas, content, autoMarker);
    for (auto i = series.begin(); i!=series.end(); i++)
        if ((*i)->valid && (*i)->style.solid.second == 255)
            (*i)->ArcLabelled::PostPosProcess(canvas, autoMarker);
    for (auto i = series.begin(); i!=series.end(); i++)
        chart->HideEntityLines((*i)->pipe_shadow);
}

//Draw a pipe, this is called for each segment, bool params dictate which part
//topside is the bigger part of the pipe
//backside is the small oval visible form the back of the pipe
//this->yPos is the outer edge of the top line
//this->left_space and right_space includes linewidth
void ArcPipe::DrawPipe(MscCanvas &canvas, bool topSideFill, bool topSideLine, bool backSide, 
                       bool shadow, bool text, double next_lw, 
                       int drawing_variant)
{
    if (shadow) {
        //Shadow under the whole pipe
        canvas.Shadow(pipe_shadow, style.shadow);
    }
    if (backSide) {
        //The back of the main pipe
        MscFillAttr fill = style.fill;
        //backside is always filled fully opaque
        fill.color.second.a = 255;
        fill.color2.second.a = 255;
        canvas.Fill(pipe_body_fill, fill);
        //the back of the small ellipsis visible from the side
        if (fill.gradient.second == GRADIENT_UP)
            fill.gradient.second = GRADIENT_DOWN;
        else if (fill.gradient.second == GRADIENT_DOWN)
            fill.gradient.second = GRADIENT_UP;
        canvas.Fill(pipe_hole_fill, fill);
        //Draw the backside line
        canvas.Line(pipe_hole_curve, style.line);
    }
    if (topSideFill) {
        //apply the transparency of the solid attribute
        MscFillAttr fill = style.fill;
        fill.color.second.a = unsigned(style.solid.second) * unsigned(fill.color.second.a) / 255;
        canvas.Fill(pipe_body_fill, fill);
    }
    if (topSideLine) {
        cairo_line_join_t t = canvas.SetLineJoin(CAIRO_LINE_JOIN_BEVEL);
        const double x = style.side.second == SIDE_RIGHT ? pipe_block.x.till : pipe_block.x.from;
        Contour clip(x, style.side.second == SIDE_LEFT ? chart->total.x : 0, 0, chart->total.y);
        if (style.line.radius.second>0 && pipe_connect_forw) {
            const XY c(x, pipe_block.y.MidPoint());
            clip -= Contour(c, style.line.radius.second-next_lw/2, pipe_block.y.Spans()/2.-next_lw/2);
        }
        canvas.Clip(clip);
        const double spacing = style.line.Spacing();
        if (!style.line.IsDoubleOrTriple() || drawing_variant==0)
            canvas.Line(pipe_body_line, style.line); //basic variant, in case of double & triple, lines cross
        else if (style.line.IsDouble()) {
            canvas.SetLineAttr(style.line);
            if (drawing_variant==1) { //advanced: lines do not cross
                canvas.singleLine(pipe_whole_line.CreateExpand(spacing), style.line);
                canvas.singleLine(pipe_hole_line.CreateExpand(-spacing), style.line);
                canvas.SetLineJoin(CAIRO_LINE_JOIN_MITER);
                canvas.singleLine(pipe_body_line.CreateExpand(-spacing), style.line);
            } else { //very advanced: proper double line joint
                canvas.singleLine(pipe_whole_line.CreateExpand(spacing), style.line); //outer
                canvas.SetLineJoin(CAIRO_LINE_JOIN_MITER);
                canvas.singleLine(pipe_hole_line.CreateExpand(-spacing), style.line); //inner hole
                canvas.singleLine(pipe_body_line.CreateExpand(-spacing) -
                                  pipe_hole_line.CreateExpand(spacing), style.line);  //inner body
            }
        } else if (style.line.IsTriple()) {
            canvas.SetLineAttr(style.line);
            //here variant 1 and 2 result in the same
            canvas.singleLine(pipe_whole_line.CreateExpand(spacing), style.line);  //outer
            canvas.SetLineJoin(CAIRO_LINE_JOIN_MITER);
            canvas.singleLine(pipe_body_line.CreateExpand(-spacing) -
                                pipe_hole_line.CreateExpand(spacing), style.line);  //inner body
            canvas.SetLineJoin(CAIRO_LINE_JOIN_BEVEL);
            canvas.singleLine(pipe_hole_line.CreateExpand(-spacing), style.line);   //inner hole
            cairo_set_line_width(canvas.GetContext(), style.line.TripleMiddleWidth());
            canvas.singleLine(pipe_body_line, style.line); //middle line
        }
        canvas.UnClip();
        canvas.SetLineJoin(t);
    }
    if (text)
        parsed_label.Draw(&canvas, sx_text, dx_text, y_text);
}

void ArcPipeSeries::Draw(MscCanvas &canvas, DrawPassType pass)
{
    if (!valid) return;
    if (pass!=draw_pass) return;
    //First shadows
    for (auto i = series.begin(); i!=series.end(); i++)
        (*i)->DrawPipe(canvas, false, false, false, true, false, 0, drawing_variant);  //dummy 0
    for (auto i = series.begin(); i!=series.end(); i++) {
        //Dont draw the topside fill
        //Draw the topside line only if pipe is fully transparent. Else we may cover the line.
        //Draw the backside in any case.
        //Do not draw text
        auto i_next = i; i_next++;
        const double next_linewidth = i_next!=series.end() ? (*i_next)->style.line.width.second : 0;
        (*i)->DrawPipe(canvas, false, (*i)->style.solid.second == 0, true, false, false, next_linewidth, drawing_variant);
    }
    if (content.size()) {
        chart->DrawArcList(canvas, content, BEFORE_ENTITY_LINES);
        for (auto i = series.begin(); i!=series.end(); i++) 
            if ((*i)->drawEntityLines)
                chart->DrawEntityLines(canvas, yPos, total_height, (*i)->src, ++EIterator((*i)->dst));
        chart->DrawArcList(canvas, content, AFTER_ENTITY_LINES);
        chart->DrawArcList(canvas, content, DEFAULT);
        chart->DrawArcList(canvas, content, AFTER_DEFAULT);
    }
    for (auto i = series.begin(); i!=series.end(); i++) {
        //Draw the topside fill only if the pipe is not fully transparent.
        //Draw the topside line in any case
        //Do not draw the backside (that may content arrow lines already drawn)
        //Draw the text
        auto i_next = i; i_next++;
        const double next_linewidth = i_next!=series.end() ? (*i_next)->style.line.width.second : 0;
        (*i)->DrawPipe(canvas, (*i)->style.solid.second > 0, true, false, false, true, next_linewidth, drawing_variant);
    }
}

//////////////////////////////////////////////////////////////////////////////////////

ArcDivider::ArcDivider(MscArcType t, Msc *msc) :
    ArcLabelled(t, msc, msc->Contexts.back().styles["divider"]),
    nudge(t==MSC_COMMAND_NUDGE), wide(false),
    extra_space(t==MSC_ARC_DISCO ? msc->discoVgap : 0)
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

ArcBase* ArcDivider::PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right, Numbering &number, bool top_level)
{
    if (!valid) return NULL;
    //Add numbering, if needed
    ArcLabelled::PostParseProcess(canvas, hide, left, right, number, top_level);

    if (!top_level && (type==MSC_ARC_DISCO || type==MSC_ARC_DIVIDER)) {
        string ss;
        ss << (type==MSC_ARC_DISCO ? "'...'" : "'---'") << " is specified inside a parallel block.";
        chart->Error.Warning(file_pos.start, ss, "May display incorrectly.");
    }
    if (hide) return NULL;
    return this;
}

void ArcDivider::Width(MscCanvas &/*canvas*/, EntityDistanceMap &distances)
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

double ArcDivider::Height(MscCanvas &canvas, AreaList &cover)
{
    if (!valid) return 0;
    yPos = 0;
    if (nudge) {
        Block b(0, chart->total.x, 0, chart->nudgeSize);
        area.mainline = area = b;
        cover = GetCover4Compress(area);
        return chart->nudgeSize;
    }
    double y = wide ? 0 : chart->arcVGapAbove;
    y += extra_space;
    const double charheight = Label("M", &canvas, style.text).getTextWidthHeight().y;

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
        area.mainline = Block(0, chart->total.x, wide ? 0 : chart->arcVGapAbove, height- (wide ? 0 :chart->arcVGapBelow));
    else
        area.mainline = Block(0, chart->total.x, centerline-charheight/2, centerline+charheight/2);
    cover = GetCover4Compress(area);
    return height;
}

void ArcDivider::ShiftBy(double y)
{
    if (!valid) return;
    if (!nudge)
        text_cover.Shift(XY(0,y));
    ArcLabelled::ShiftBy(y);
}

void ArcDivider::PostPosProcess(MscCanvas &canvas, double autoMarker)
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
    ArcLabelled::PostPosProcess(canvas, autoMarker);
}

void ArcDivider::Draw(MscCanvas &canvas, DrawPassType pass)
{
    if (!valid) return;
    if (pass!=draw_pass) return;
    if (nudge) return;
    parsed_label.Draw(&canvas, text_margin, chart->total.x-text_margin, yPos + (wide ? 0 : chart->arcVGapAbove+extra_space));
    //determine widest extent for coverage at the centerline+-style.line.LineWidth()/2;
    const double lw2 = ceil(style.line.LineWidth()/2.);
    Block b(line_margin, chart->total.x-line_margin, yPos + centerline - lw2, yPos + centerline + lw2);
    Range r = (text_cover * b).GetBoundingBox().x;
    if (r.IsInvalid())
        canvas.Line(XY(line_margin, yPos + centerline), XY(chart->total.x-line_margin, yPos + centerline), style.line);
    else {
        canvas.Line(XY(line_margin, yPos + centerline), XY(r.from-chart->emphVGapInside, yPos + centerline), style.line);
        canvas.Line(XY(r.till+chart->emphVGapInside, yPos + centerline), XY(chart->total.x-line_margin, yPos + centerline), style.line);
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

ArcBase* ArcParallel::PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right, Numbering &number, bool top_level)
{
    if (!valid) return NULL;
    at_top_level = top_level;
    for (PtrList<ArcList>::iterator i=blocks.begin(); i != blocks.end(); i++)
        chart->PostParseProcessArcList(canvas, hide, **i, false, left, right, number, false);
    if (hide) return NULL;
    return this;
}

void ArcParallel::Width(MscCanvas &canvas, EntityDistanceMap &distances)
{
    if (!valid) return;
    EntityDistanceMap d;
    for (PtrList<ArcList>::iterator i=blocks.begin(); i != blocks.end(); i++)
        chart->WidthArcList(canvas, **i, d);
    d.CombineLeftRightToPair_Sum(chart->hscaleAutoXGap);
    d.CombineLeftRightToPair_Single(chart->hscaleAutoXGap);
    d.CopyBoxSideToPair(chart->hscaleAutoXGap);
    distances += d;
}

double ArcParallel::Height(MscCanvas &canvas, AreaList &cover)
{
    if (!valid) return 0;
    heights.clear();
    heights.reserve(blocks.size());
    double height = 0;
    for (auto i=blocks.begin(); i != blocks.end(); i++) {
        AreaList cover_block;
        //Each parallel block is compressed without regard to the others
        double h = chart->HeightArcList(canvas, (*i)->begin(), (*i)->end(), cover_block);
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

void ArcParallel::PostPosProcess(MscCanvas &canvas, double autoMarker)
{
    if (!valid) return;
    ArcBase::PostPosProcess(canvas, autoMarker);
    int n=0;
    //For automarker, give the bottom of the largest of previous blocks
    for (auto i=blocks.begin(); i!=blocks.end(); i++, n++)
        chart->PostPosProcessArcList(canvas, *(*i),
            n>0 && heights[n-1]>0 ? yPos + heights[n-1] : autoMarker);
}

void ArcParallel::Draw(MscCanvas &canvas, DrawPassType pass)
{
    if (!valid) return;
    for (auto i=blocks.begin(); i != blocks.end(); i++)
        chart->DrawArcList(canvas, *(*i), pass);
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

void CommandEntity::AttributeNames(Csh &)
{
    return;
}

bool CommandEntity::AttributeValues(const std::string, Csh &)
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
		} else if (CaseInsensitiveEqual(prefix, "activate") || CaseInsensitiveEqual(prefix, "deactivate")) {
            if ((*i)->active_is_explicit) continue;
			(*i)->active.first = true;	
			(*i)->active.second = CaseInsensitiveEqual(prefix, "activate");
            (*i)->active.third = (*i)->file_pos.start;
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


//Adds and entitydef for "entity" uses running_show and running_style
EntityDef* CommandEntity::FindAddEntityDefForEntity(const string &entity, const file_line_range &l)
{
    //find if already have a def for this
    for (auto i_def = entities.begin(); i_def != entities.end(); i_def++) 
        if ((*i_def)->name == entity) return *i_def;
    const EIterator jj_ent = chart->AllEntities.Find_by_Name(entity);
    _ASSERT(*jj_ent != chart->NoEntity);
    EntityDef *ed = new EntityDef(entity.c_str(), chart);
    ed->itr = jj_ent;
    ed->style = (*jj_ent)->running_style;
    ed->file_pos = l;
    ed->show.first = ed->active.first = false;
    entities.Append(ed);
    return ed;
}

//return active if any of the children are active and ON if any of them are ON
//go recursive all the way deep - ignore the "running_shown" of parents
EEntityStatus CommandEntity::GetCombinedStatus(const std::set<string>& children) const
{
    EEntityStatus ret = EEntityStatus::SHOW_OFF;
    for (auto s = children.begin(); s!=children.end(); s++) {
        auto i = chart->AllEntities.Find_by_Name(*s);
        _ASSERT(*i != chart->NoEntity);
        EEntityStatus es;
        if ((*i)->children_names.size()) es = GetCombinedStatus((*i)->children_names);
        else es = (*i)->running_shown;
        if (es.IsActive()) ret.Activate(true);
        if (es.IsOn()) ret.Show(true);
    }
    return ret;
}

/* The following rules apply.
 * Each entity we can possibly have is represented in chart->AllEntities and is of
 * class Entity. 
 * Each time we name an entity in an entity command, we allocate an EntityDef object.
 * If the "defining" member is true this mention of the entity was used to define the
 * entity. If it is false, this mention of the entity is a subsequent one.
 * An entity can have zero or one EntityDefs with defining=true (zero if the entity
 * was implicitly defined via e.g., an arrow definition).
 * When an entity is mentioned in an entity command any (or all) of the three things
 * can be done: turn it on/off, activate/deactivate it or change the style. An entity
 * that is on can be turned on again, in which case we draw an entity heading of it.
 * Similar, the heading command draws an entity heading for all entities that are currently
 * on. This is emulated by adding EntityDefs for such entities with show set to on.
 *
 * During the PostParse process we keep up-to date the "running_show" and "running_style"
 * members of the entities. (Memmbers of class "Entity".) These are used by arrows, etc.
 * to find out if certain entities are on/off or active, etc.
 * In addition, we store the "running_style" and "running_show" in every EntityDef, as well;
 * and then in the PosPos process, when x and y coordinates are already set, we copy them
 * to Entity::status, so that when we draw entity lines we know what to draw at what 
 * coordinate. The entity headings are drawn based on the EntityDef::style member.
 *
 * The EntityDef::draw_heading member tells if a heading should be drawn for this heading
 * at this point. The "show" and "active" members tell if there was "show" or "active"
 * attributes set (perhaps via show/hide/activate/deactivate commands). If so, the
 * "show_is_explicit"/"active_is_explicit" are set. (This is used to prevent overwriting
 * an explicitly written [show=yes/no] with a "show" command. Thus 'show aaa [show=no]' 
 * will result in show=false. Otherwise any newly defined entitydef
 * has show set to true and active set to false - the default for newly defined entities.

 * The algorithm here is the following.
 *
 * 0. Merge entitydefs corresponding to the same entity. (They are in the same order 
 *    as in the source file, so copying merging "active", "show" and "style" is enough,
 *    since all other attributes can only be used at definition, which can be the first 
 *    one only.)
 * 1. Apply the style elements specified by the user to the entity's "running_style"
 * 2. If a grouped entity is listed with show=yes/no, we copy this attribute to all of its
 *    listed children (unless they also have an explicit show attribute, which overrides that
 *    of the group). We also prepend any unlisted children and copy the attribute.
 *    At the same time, we remove the show attribute from the grouped entities.
 *    This will result in that if a grouped entity is turned on or off all of its 
 *    children (and their children, too) will get turned on or off, as well.
 * 3. Apply all listed entities show and active attributes to "running_show" & "draw_heading"
 * 4. Update the "running_shown" of all parents based on the status of leaf children,
 *    if any change, add a corresponding entitydef (if needed)
 * 5. If we are a heading command, add entities where running_show indicates shown
 * 6. Order the listed entities such that parents come first and their children after
 * 7. Set "draw_heading" of the children of collapsed parents to false
 * 8. For those that draw a heading, update "left" and "right", process label & record max width
 */

//TODO: What if multiple entitydefs are present for the same entity? We should merge them
ArcBase* CommandEntity::PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right, Numbering &,
                                     bool top_level)
{
    if (!valid) return NULL;
    at_top_level = top_level;
    if (full_heading && !top_level)
        chart->Error.Warning(file_pos.start, "The command 'heading' is specified "
                             "inside a parallel block. May display incorrectly.");

    //0. First merge entitydefs of the same entity, so that we have at most one for each entity.
    for (auto i_def = entities.begin(); i_def != entities.end(); /*nope*/) {
        //find first entity of this name
        EntityDef *ed = FindAddEntityDefForEntity((*i_def)->name, file_pos); //second param dummy, we never add here
        if (ed == *i_def) i_def++; 
        else {
            //OK, "ed" is an EntityDef before i_def, combine them.
            _ASSERT(!(*i_def)->defining);
            //show_is_explicit and active_is_explicit makes no role beyond this, so ignore
            if ((*i_def)->show.first) 
                ed->show = (*i_def)->show;
            if ((*i_def)->active.first) 
                ed->active = (*i_def)->active;
            ed->style += (*i_def)->style;
            //Ok, delete i_def
            entities.erase(i_def++);
        }
    }
    
    //1. Then apply the style changes of this command to the running style of the entities
    for (auto i_def = entities.begin(); i_def != entities.end(); i_def++) {
        const EIterator j_ent = chart->AllEntities.Find_by_Name((*i_def)->name);
        (*i_def)->itr = j_ent;
        //Make the style of the entitydef fully specified using the accumulated style info in Entity
        (*j_ent)->running_style += (*i_def)->style;  //(*i)->style is a partial style here specified by the user
    }
    //2. Copy show on/off attribute from grouped entities to their children
    for (auto i_def = entities.begin(); i_def != entities.end(); i_def++) {
        const Entity *ent = *(*i_def)->itr;
        //if the entity is a grouped entity with a show/hide attribute, 
        //add an entitydef to our list for those children, who have no entitidefs
        //yet in the list. For those children, who have, just set show attribute
        //if not set yet
        //new entitydefs are added to the end of the list - and get processed for further 
        //children
        if ((*i_def)->show.first && ent->children_names.size()) 
            for (auto ss = ent->children_names.begin(); ss!=ent->children_names.end(); ss++) 
                FindAddEntityDefForEntity(*ss, (*i_def)->file_pos)->show = (*i_def)->show;
    }
    //3. 
    for (auto i_def = entities.begin(); i_def != entities.end(); i_def++) {
        Entity *ent = *(*i_def)->itr;
        //Decide, if this entitydef will draw a heading or not
        //It can get drawn because we 1) said show=yes, or
        //2) because it is on, we mention it (without show=yes) and it is
        //a full heading.
        (*i_def)->draw_heading = ((*i_def)->show.second && (*i_def)->show.first) 
                                 || (full_heading && ent->running_shown.IsOn());
        //Adjust the running status of the entity, this is valid *after* this command. 
        //This is just for the Height process knwos whch entity is on/off
        if ((*i_def)->show.first)
            ent->running_shown.Show((*i_def)->show.second);
        //Update the style of the entitydef
        (*i_def)->style = ent->running_style;	 //(*i)->style now become the full style to use from this point
        //reflect any "active" attribute in the running_shown variable 
        if ((*i_def)->active.first) 
            ent->running_shown.Activate((*i_def)->active.second);
    }
    //4. Now we are guaranteed to have all leaf children's runnin_shown status right.
    //However, we can have parents, whose child(ren) changed status and we need to update that
    //We will need to add EntityDefs here for such parents (so they update status in PostPos, too)
    for (auto j_ent = chart->AllEntities.begin(); j_ent != chart->AllEntities.end(); j_ent++) {
        if ((*j_ent)->children_names.size()==0) continue;
        EEntityStatus es_new = GetCombinedStatus((*j_ent)->children_names);
        EEntityStatus es_old = (*j_ent)->running_shown;
        if (es_old == es_new) continue;
        //ok, shown status has changed, add/lookup entitydef
        EntityDef *ed = FindAddEntityDefForEntity((*j_ent)->name, this->file_pos);
        if (es_new.IsOn() != es_old.IsOn()) {
            ed->show.first = true;
            ed->show.second = es_new.IsOn();
        }
        if (es_new.IsActive() != es_old.IsActive()) {
            ed->active.first = true;
            ed->active.second = es_new.IsActive();
        }
        (*j_ent)->running_shown = es_new;
    }

    //5. A "heading" command, we have to draw all entities that are on
    //for these we create additional EntityDefs and append them to entities
    //Only do this for children (non-grouped entities)
    if (full_heading)
        for (auto i = chart->AllEntities.begin(); i!=chart->AllEntities.end(); i++) {
            if (!(*i)->running_shown.IsOn()) continue;
            if ((*i)->children_names.size()) continue;
            EntityDef *ed = FindAddEntityDefForEntity((*i)->name, this->file_pos); //use the file_pos of the CommandEntity
            ed->draw_heading = true;
        }

    //6. Order the list (lousy bubblesort)
    //any descendant should come after any of their anscestors, but we can only compare
    //direct parent-child, so we go through the list until we see change
    bool changed;
    do {
        changed = false;
        for (auto i_def = ++entities.begin(); i_def != entities.end(); i_def++) 
            for (auto i_def2 = entities.begin(); i_def2 != i_def; i_def2++) 
                if ((*(*i_def2)->itr)->parent_name == (*i_def)->name) {
                    std::swap(*i_def, *i_def2);
                    changed = true;
                }
    } while (changed);

    //At this point the list contains all entities that 
    //- is mentioned by the user (style or status change)
    //- shall be shown a heading due to heading command
    //- has a parent (ancestor) or children (descendant) in the above mentioned two categories
    //Collapse is not considered yet, we will do it below

    //7. Finally prune the list: do not show those that shall not be displayed due to collapse
    for (auto i_def = entities.begin(); i_def != entities.end(); i_def++) 
        if (chart->FindActiveParentEntity((*i_def)->itr) != (*i_def)->itr) 
            (*i_def)->draw_heading = false;

    //8. At last we have all entities among "entities" that will show here/change status or style
    //Go through them and update left, right and the entities' maxwidth
    for (auto i_def = entities.begin(); i_def != entities.end(); i_def++) {
        if (!(*i_def)->draw_heading) continue;
        const EIterator ei = (*i_def)->itr;
        left =  chart->EntityMinByPos(left,  chart->FindWhoIsShowingInsteadOf(ei, true));
        right = chart->EntityMaxByPos(right, chart->FindWhoIsShowingInsteadOf(ei, false));
        (*i_def)->parsed_label.Set((*ei)->label, &canvas, (*ei)->running_style.text);
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
void CommandEntity::Width(MscCanvas &/*canvas*/, EntityDistanceMap &distances)
{
    if (!valid || hidden) return;
    //Add distances for entity heading
    //Start by creating a map in which distances are ordered by index
    std::map<int, pair<double, double>> dist; //map the index of an active entity to spaces left & right
    //in PostParseProcess we created an entitydef for all entities shown here. 
    //"full_heading" not even checked here
    for (auto i = entities.rbegin(); !(i==entities.rend()); i++) {
        //Take entity height into account or draw it if show=on was added
        if (!(*i)->draw_heading) continue;
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

double CommandEntity::Height(MscCanvas &/*canvas*/, AreaList &cover)
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
        if (!(*i)->draw_heading) continue;
        //Collect who is my children in this list
        EntityDefList edl(false);
        for (auto ii = entities.rbegin(); !(ii==entities.rend()); ii++) 
            if ((*ii)->draw_heading && *ii != *i && chart->IsMyParentEntity((*ii)->name, (*i)->name))
                edl.Append(*ii);
        //EntityDef::Height places children entities to yPos==0
        //Grouped entities may start at negative yPos.
        //We collect here the maximum extent
        //Note: Height() also adds the cover to the entitydef's area
        Area entity_cover;
        hei += (*i)->Height(entity_cover, edl); 
        cover += GetCover4Compress(entity_cover);
        area += (*i)->GetAreaToSearch();
    }
    if (!hei.Spans()) return height = 0; //if no headings show
    //Ensure overall startpos is zero
    ShiftBy(-hei.from + chart->headingVGapAbove);
    cover.Shift(XY(0,-hei.from + chart->headingVGapAbove));
    return height = chart->headingVGapAbove + hei.Spans() + chart->headingVGapBelow;
}

void CommandEntity::ShiftBy(double y)
{
    if (!valid) return;
    for (auto i = entities.begin(); i!=entities.end(); i++)
        (*i)->ShiftBy(y);
    ArcCommand::ShiftBy(y);
}

void CommandEntity::PostPosProcess(MscCanvas &canvas, double autoMarker)
{
    if (!valid) return;
    ArcCommand::PostPosProcess(canvas, autoMarker);
    for (auto i = entities.begin(); i!=entities.end(); i++)
        (*i)->PostPosProcess(canvas, autoMarker);
    if (height>0 && !hidden) {
        if (chart->headingSize == 0) chart->headingSize = yPos + height;
        chart->headingSize = std::min(chart->headingSize, yPos + height);
    }
}

void CommandEntity::Draw(MscCanvas &canvas, DrawPassType pass)
{
    if (!valid) return;
    if (pass!=draw_pass) return;
    for (auto i = entities.begin(); i!=entities.end(); i++)
        if ((*i)->draw_heading)
            (*i)->Draw(canvas);
}

//////////////////////////////////////////////////////////////////////////////////////

bool CommandNewpage::AddAttribute(const Attribute &)
{
    return false;
}

void CommandNewpage::AttributeNames(Csh &)
{

}

bool CommandNewpage::AttributeValues(const std::string, Csh &)
{
    return false;
}

double CommandNewpage::Height(MscCanvas &/*canvas*/, AreaList &)
{
    if (!valid) return 0;
    Block b(0, chart->total.x, -chart->nudgeSize/2, chart->nudgeSize/2);
    area_draw = b;
    draw_is_different = true; //area is empty - never find this
    return 0;
}

void CommandNewpage::PostPosProcess(MscCanvas &/*canvas*/, double)
{
    if (!valid) return;
    chart->yPageStart.push_back(yPos);
}

//////////////////////////////////////////////////////////////////////////////////////

double CommandNewBackground::Height(MscCanvas &/*canvas*/, AreaList &)
{
    if (!valid) return 0;
    Block b(0, chart->total.x, -chart->nudgeSize/2, chart->nudgeSize/2);
    area_draw = b;
    draw_is_different = true; //area is empty - never find this
    return 0;
}

void CommandNewBackground::PostPosProcess(MscCanvas &/*canvas*/, double)
{
    if (!valid) return;
    chart->Background[yPos] = fill;
}

//////////////////////////////////////////////////////////////////////////////////////
ArcBase* CommandNumbering::PostParseProcess(MscCanvas &/*canvas*/, bool hide, EIterator &/*left*/, EIterator &/*right*/, Numbering &number, bool /*top_level*/)
{
    if (!valid) return NULL;
    if (hide) hidden = true;
    if ((action & SIZE) && length)
        number.SetSize(unsigned(length));
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
		chart->Error.Error(ml.start, "Marker '"+name+"' has already been defined. Keeping old definition.");
        chart->Error.Error(i->second.first,  ml.start, "Location of previous definition.");
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

double CommandMark::Height(MscCanvas &/*canvas*/, AreaList &)
{
    if (!valid) return 0;
    Block b(0, chart->total.x, offset-chart->nudgeSize/2, offset+chart->nudgeSize/2);
    area_draw = b;
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

CommandEmpty::CommandEmpty(Msc *msc) : ArcCommand(MSC_COMMAND_EMPTY, msc)
{
}

void CommandEmpty::Width(MscCanvas &canvas, EntityDistanceMap &distances)
{
    if (!valid) return;
    StringFormat format;
    format.Default();
    format.Apply("\\pc\\mu(10)\\md(10)\\ml(10)\\mr(10)\\c(255,255,255)\\b\\i");
    parsed_label.Set(string("\\i\\bEmpty chart"), &canvas, format);
    const unsigned lside_index = (*chart->ActiveEntities.Find_by_Name(LSIDE_ENT_STR))->index;
    const unsigned rside_index = (*chart->ActiveEntities.Find_by_Name(RSIDE_ENT_STR))->index;
    const double width = parsed_label.getTextWidthHeight().x + 2*EMPTY_MARGIN_X;
    distances.Insert(lside_index, rside_index, width);
}

double CommandEmpty::Height(MscCanvas &/*canvas*/, AreaList &cover)
{
    if (!valid) return 0;
    yPos = 0;
    const XY wh = parsed_label.getTextWidthHeight();
    const Area a(Block((chart->total.x-wh.x)/2, (chart->total.x+wh.x)/2, EMPTY_MARGIN_Y, EMPTY_MARGIN_Y+wh.y), this);
    cover = GetCover4Compress(a);
    return wh.y + EMPTY_MARGIN_Y*2;
}

void CommandEmpty::Draw(MscCanvas &canvas, DrawPassType pass)
{
    if (!valid) return;
    if (pass!=draw_pass) return;
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

    canvas.Shadow(b, line, shadow);
    canvas.Fill(b, line, fill);
    canvas.Line(b, line);
}

/////////////////////////////////////////////////////////////////
CommandHSpace::CommandHSpace(Msc*msc, const NamePair*enp) :
    ArcCommand(MSC_COMMAND_HSPACE, msc), format(msc->Contexts.back().text),
    label(false, string()), space(false, 0)
{
    if (enp==NULL) {
        valid=false;
        return;
    }
    if (enp->src.length()) {
        src = chart->FindAllocEntity(enp->src.c_str(), enp->sline);
        sline = enp->sline;
    } else
        src = chart->AllEntities.Find_by_Name(LSIDE_ENT_STR);
    if (enp->dst.length()) {
        dst = chart->FindAllocEntity(enp->dst.c_str(), enp->dline);
        dline = enp->dline;
    } else
        dst = chart->AllEntities.Find_by_Name(RSIDE_ENT_STR);
    delete enp;
}

bool CommandHSpace::AddAttribute(const Attribute &a)
{
    if (a.Is("label")) {
        if (!a.CheckType(MSC_ATTR_STRING, chart->Error)) return true;
        //MSC_ATTR_CLEAR is OK above with value = ""
        label.first = true;
        label.second = a.value;
        return true;
    }
    if (a.Is("space")) {
        if (!a.CheckType(MSC_ATTR_NUMBER, chart->Error)) return true;
        space.first = true;
        space.second = a.number;
        return true;
    }
    if (format.AddAttribute(a, chart, STYLE_ARC)) return true;
    return ArcCommand::AddAttribute(a);
}

void CommandHSpace::AttributeNames(Csh &csh)
{
    ArcCommand::AttributeNames(csh);
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "label", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "space", HINT_ATTR_NAME));
    StringFormat::AttributeNames(csh);
}

bool CommandHSpace::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr,"label")) {
        return true;
    }
    if (CaseInsensitiveEqual(attr,"space")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", HINT_ATTR_VALUE, false));
        return true;
    }
    if (StringFormat::AttributeValues(attr, csh)) return true;
    if (ArcCommand::AttributeValues(attr, csh)) return true;
    return false;
}

ArcBase* CommandHSpace::PostParseProcess(MscCanvas &/*canvas*/, bool /*hide*/,
    EIterator &/*left*/, EIterator &/*right*/, Numbering &/*number*/, bool /*top_level*/)
{
    if (!valid) return NULL;
    if (!label.first && !space.first) {
        chart->Error.Error(file_pos.start, "You must specify either a numeric space or a lable for the hspace command.");
        return NULL;
    }
    //Give error if user specified groupe entities
    if (chart->ErrorIfEntityGrouped(src, sline.start)) return NULL;
    if (chart->ErrorIfEntityGrouped(dst, sline.start)) return NULL;
    //check if src and dst has disappeared, also make src&dst point to
    //chart->AllActiveEntities
    src = chart->ActiveEntities.Find_by_Ptr(*chart->FindActiveParentEntity(src));
    dst = chart->ActiveEntities.Find_by_Ptr(*chart->FindActiveParentEntity(dst));
    //we keep ourselves even if src/dst has disappeared
    return this; 
}


void CommandHSpace::Width(MscCanvas &canvas, EntityDistanceMap &distances)
{
    if (!valid) return;
    double dist = space.second; //0 if not specified by user
    if (label.second.length())
        dist += Label(label.second, &canvas, format).getTextWidthHeight().x;
    if (dist<0)
        chart->Error.Error(file_pos.start, "The horizontal space specified is negative. Ignoring it.");
    else
        distances.Insert((*src)->index, (*dst)->index, dist);
}


//////////////////////////////////////////////////////////////////////////////////

CommandVSpace::CommandVSpace(Msc*msc)  : ArcCommand(MSC_COMMAND_VSPACE, msc),
    format(msc->Contexts.back().text), label(false, string()),
    space(false, 0), compressable(false)
{
}

bool CommandVSpace::AddAttribute(const Attribute &a)
{
    if (a.Is("label")) {
        if (!a.CheckType(MSC_ATTR_STRING, chart->Error)) return true;
        //MSC_ATTR_CLEAR is OK above with value = ""
        label.first = true;
        label.second = a.value;
        return true;
    }
    if (a.Is("space")) {
        if (!a.CheckType(MSC_ATTR_NUMBER, chart->Error)) return true;
        space.first = true;
        space.second = a.number;
        return true;
    }
    if (a.Is("compressable")) {
        if (!a.CheckType(MSC_ATTR_BOOL, chart->Error)) return true;
        compressable = a.yes;
        return true;
    }
    if (format.AddAttribute(a, chart, STYLE_ARC)) return true;
    return ArcCommand::AddAttribute(a);
}

void CommandVSpace::AttributeNames(Csh &csh)
{
    ArcCommand::AttributeNames(csh);
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "label", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "space", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "compressable", HINT_ATTR_NAME));
    StringFormat::AttributeNames(csh);
}

bool CommandVSpace::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr,"label")) {
        return true;
    }
    if (CaseInsensitiveEqual(attr,"space")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", HINT_ATTR_VALUE, false));
        return true;
    }
    if (CaseInsensitiveEqual(attr,"compressable")) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+"yes", HINT_ATTR_VALUE, true, CshHintGraphicCallbackForYesNo, CshHintGraphicParam(1)));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+"no", HINT_ATTR_VALUE, true, CshHintGraphicCallbackForYesNo, CshHintGraphicParam(0)));
        return true;
    }
    if (StringFormat::AttributeValues(attr, csh)) return true;
    if (ArcCommand::AttributeValues(attr, csh)) return true;
    return false;
}

ArcBase* CommandVSpace::PostParseProcess(MscCanvas &/*canvas*/, bool /*hide*/,
    EIterator &/*left*/, EIterator &/*right*/, Numbering &/*number*/, bool /*top_level*/)
{
    if (!valid) return NULL;
    if (!label.first && !space.first) {
        chart->Error.Error(file_pos.start, "You must specify either a numeric space or a lable for the vspace command.");
        return NULL;
    }
    return this;
}

double CommandVSpace::Height(MscCanvas &canvas, AreaList &cover)
{
    double dist = space.second;
    if (label.second.length())
        dist += Label(label.second, &canvas, format).getTextWidthHeight().y;
    if (dist<0)
        chart->Error.Error(file_pos.start, "The vertical space specified is negative. Ignoring it.");
    if (dist<=0)
        return 0;
    if (!compressable) {
        area = Block(0, chart->total.x, 0, dist);
        cover = GetCover4Compress(area);
    }
    return dist;
}


//////////////////////////////////////////////////////////////////////////////////

ExtVertXPos::ExtVertXPos(const char *s, const file_line_range &sl, const VertXPos *p) :
VertXPos(*p), side_line(sl)
{
    if (!valid) {
        side = BAD_SIDE;
        return;
    }
    if (CaseInsensitiveEqual(s, "left"))
        side = LEFT;
    else if (CaseInsensitiveEqual(s, "center"))
        side = CENTER;
    else if (CaseInsensitiveEqual(s, "right"))
        side = RIGHT;
    else
        side = BAD_SIDE;
}

const double CommandSymbol::ellipsis_space_ratio = 2.0/3.0;


CommandSymbol::CommandSymbol(Msc*msc, const char *symbol, const NamePair *enp,
                const ExtVertXPos *vxpos1, const ExtVertXPos *vxpos2) :
    ArcCommand(MSC_COMMAND_SYMBOL, msc),
    style(chart->Contexts.back().styles["symbol"]),
    hpos1(vxpos1 ? *vxpos1 : ExtVertXPos(*msc)),
    hpos2(vxpos2 ? *vxpos2 : ExtVertXPos(*msc)),
    vpos(enp ? *enp : NamePair(NULL, file_line_range(), NULL, file_line_range())),
    xsize(false, 0), ysize(false, 0), size(MSC_ARROW_SMALL)
{
    if (CaseInsensitiveEqual(symbol, "arc"))
        symbol_type = ARC;
    else if (CaseInsensitiveEqual(symbol, "rectangle"))
        symbol_type = RECTANGLE;
    else if (CaseInsensitiveEqual(symbol, "..."))
        symbol_type = ELLIPSIS;
    else {
        valid = false;
        return;
    }
    if (symbol_type == ELLIPSIS) {
        if (hpos1.side != ExtVertXPos::NONE && hpos2.side != ExtVertXPos::NONE) {
            chart->Error.Error(hpos2.e1line.start, "Symbol '...' can only have one vertical position indicated. Ignoring second one.");
            hpos2.side = ExtVertXPos::NONE;
        }
        if (vpos.dst.length()>0 && vpos.src.length()>0) {
            chart->Error.Error(vpos.dline.start, "Symbol '...' can only have one horizontal position indicated. Ignoring second one.");
            vpos.src.clear();
        }
        style.fill.color.second = style.line.color.second;
    }
}

bool CommandSymbol::AddAttribute(const Attribute &a)
{
    if (a.Is("xsize")) 
        switch (symbol_type) {
        case ELLIPSIS:
            chart->Error.Error(a, false, "Attribute 'xsize' is valid only for arc and rectangle symbols.", "Try using just 'size' instead.");
            return true;
        default:
            if (!a.CheckType(MSC_ATTR_NUMBER, chart->Error)) return true;
            xsize.first = true;
            xsize.second = a.number;
            return true;
        }
    if (a.Is("ysize")) 
        switch (symbol_type) {
        case ELLIPSIS:
            chart->Error.Error(a, false, "Attribute 'ysize' is valid only for arc and rectangle symbols.", "Try using just 'size' instead.");
            return true;
        default:
            if (!a.CheckType(MSC_ATTR_NUMBER, chart->Error)) return true;
            ysize.first = true;
            ysize.second = a.number;
            return true;
        }
    if (a.Is("size")) 
        switch (symbol_type) {
        case ELLIPSIS: 
            {
                ArrowHead ah; //use this type to decode 'small', 'normal', etc...
                ah.Empty();
                ah.AddAttribute(a, chart, STYLE_ARC);
                if (ah.size.first) size = ah.size.second;
            }
            return true;
        default:
            chart->Error.Error(a, false, "Attribute 'size' is valid only for '...' symbols.", "Try using 'xsize' and/or 'ysize' instead.");
            return true;
        }
    if (a.Is("draw_time")) {  //We add this even though it is in ArcBase::AddAttribute, but ArcCommand::AddAttribute does not call that
        if (!a.EnsureNotClear(chart->Error, STYLE_ARC)) return true;
        if (a.type == MSC_ATTR_STRING && Convert(a.value, draw_pass)) return true;
        a.InvalidValueError(CandidatesFor(draw_pass), chart->Error);
        return true;
    }
    if (style.AddAttribute(a, chart)) return true;
    return ArcCommand::AddAttribute(a);
}

void CommandSymbol::AttributeNames(Csh &csh)
{
    ArcCommand::AttributeNames(csh);
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "xsize", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "ysize", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "size", HINT_ATTR_NAME));
    Design().styles["symbol"].AttributeNames(csh);
}

bool CommandSymbol::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr,"xsize")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", HINT_ATTR_VALUE, false));
        return true;
    }
    if (CaseInsensitiveEqual(attr,"ysize")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", HINT_ATTR_VALUE, false));
        return true;
    }
    if (CaseInsensitiveEqual(attr,"size")) {
        ArrowHead::AttributeValues(attr, csh, ArrowHead::ANY);
        return true;
    }
    if (ArcCommand::AttributeValues(attr, csh)) return true;
    if (Design().styles["symbol"].AttributeValues(attr, csh)) return true;
    return false;
}

ArcBase* CommandSymbol::PostParseProcess(MscCanvas &/*canvas*/, bool hide, EIterator &/*left*/, EIterator &/*right*/, Numbering &/*number*/, bool /*top_level*/)
{
    if (!valid) return NULL;
    if (vpos.src.length()) {
        std::map<string, Msc::MarkerType>::const_iterator i = chart->Markers.find(vpos.src);
        if (i == chart->Markers.end()) {
            chart->Error.Error(file_pos.start, "Cannot find marker '" + vpos.src + "'."
                " Ignoring symbol.");
            return NULL;
        }
    }
    if (vpos.dst.length()) {
        std::map<string, Msc::MarkerType>::const_iterator i = chart->Markers.find(vpos.dst);
        if (i == chart->Markers.end()) {
            chart->Error.Error(file_pos.start, "Cannot find marker '" + vpos.dst + "'."
                " Ignoring symbol.");
            return NULL;
        }
    }
    if (hpos1.side == ExtVertXPos::BAD_SIDE) {
        chart->Error.Error(hpos1.side_line.start, "Invalid horizontal position designator. Ignoring symbol.", "Use 'left', 'right' or 'center'.");
        return NULL;
    }
    if (hpos2.side == ExtVertXPos::BAD_SIDE) {
        chart->Error.Error(hpos2.side_line.start, "Invalid horizontal position designator. Ignoring symbol.", "Use 'left', 'right' or 'center'.");
        return NULL;
    }
    if (hpos1.side == ExtVertXPos::NONE) {
        chart->Error.Error(hpos1.side_line.start, "You need to specify at least one horizontal position designator. Ignoring symbol.", "Use 'left at', 'right at' or 'center at'.");
        return NULL;
    }
    if (hpos1.side == hpos2.side) {
        chart->Error.Error(hpos2.side_line.start, "You cannot specify the same horizontal position designator twice. Ignoring symbol.");
        chart->Error.Error(hpos1.side_line.start, hpos2.side_line.start, "Here is the first designator.");
        return NULL;
    }
    if (chart->ErrorIfEntityGrouped(hpos1.entity1, hpos1.e1line.start)) return NULL;
    if (chart->ErrorIfEntityGrouped(hpos1.entity2, hpos1.e2line.start)) return NULL;
    if (chart->ErrorIfEntityGrouped(hpos2.entity1, hpos2.e1line.start)) return NULL;
    if (chart->ErrorIfEntityGrouped(hpos2.entity2, hpos2.e2line.start)) return NULL;
    if (hide) return NULL;

    hpos1.entity1 = chart->ActiveEntities.Find_by_Ptr(*chart->FindActiveParentEntity(hpos1.entity1));
    hpos1.entity2 = chart->ActiveEntities.Find_by_Ptr(*chart->FindActiveParentEntity(hpos1.entity2));
    hpos2.entity1 = chart->ActiveEntities.Find_by_Ptr(*chart->FindActiveParentEntity(hpos2.entity1));
    hpos2.entity2 = chart->ActiveEntities.Find_by_Ptr(*chart->FindActiveParentEntity(hpos2.entity2));

    if (hpos1.side > hpos2.side) std::swap(hpos1, hpos2);
    switch (symbol_type) {
    case ARC:
    case RECTANGLE:
        if (hpos2.side == ExtVertXPos::NONE && xsize.first == false) {
            xsize.first = true;
            xsize.second = 10; //default size;
        }
        if (!(vpos.dst.length() || vpos.src.length()) && ysize.first == false) {
            ysize.first = true;
            ysize.second = 10; //default size;
        }
        break;
    case ELLIPSIS:
        static const double ellipsis_sizes[] = {0, 1.5, 3, 5, 8, 15}; //INVALID, TINY, SMALL, NORMAL, LARGE, HUGE
        xsize.first = ysize.first = true;
        xsize.second = ellipsis_sizes[size];
        ysize.second = xsize.second*(3+2*ellipsis_space_ratio);
    }

    return this;
}

void CommandSymbol::Width(MscCanvas &/*canvas*/, EntityDistanceMap &/*distances*/)
{
}

double CommandSymbol::Height(MscCanvas &/*canvas*/, AreaList &cover)
{
    //Calculate x positions
    const double lw = style.line.LineWidth();
    double x1 = hpos1.CalculatePos(*chart);
    switch (hpos2.side) {
    case ExtVertXPos::NONE:
        switch (hpos1.side) {
        case ExtVertXPos::LEFT:
            outer_edge.x.from = x1;
            outer_edge.x.till = x1 + xsize.second;
            break;
        case ExtVertXPos::RIGHT:
            outer_edge.x.from = x1 - xsize.second;
            outer_edge.x.till = x1;
            break;
        case ExtVertXPos::CENTER:
            outer_edge.x.from = x1 - xsize.second/2;
            outer_edge.x.till = x1 + xsize.second/2;
            break;
        default:
            _ASSERT(0);
            break;
        }
        break;
    case ExtVertXPos::RIGHT:
        outer_edge.x.till = hpos2.CalculatePos(*chart);
        if (hpos1.side == ExtVertXPos::LEFT)
            outer_edge.x.from = x1;
        else //can only be center
            outer_edge.x.from = 2*x1 - outer_edge.x.till;
        break;
    case ExtVertXPos::CENTER:
        //here hpos1 can only be LEFT
        outer_edge.x.from = x1;
        outer_edge.x.till = 2*hpos2.CalculatePos(*chart) - x1;
        break;
    default:
        _ASSERT(0);
        break;
    }
    outer_edge.x.Expand(lw/2);
    //if no markers were specified, we draw it here and assign room
    //else we are done here
    if (vpos.src.length() || vpos.dst.length()) {
        outer_edge.y.MakeInvalid();
        return 0;
    }
    outer_edge.y.from = 0;
    outer_edge.y.till = lw + ysize.second;

    CalculateAreaFromOuterEdge();

    if (style.shadow.offset.second)
        cover = area + area.CreateShifted(XY(style.shadow.offset.second, style.shadow.offset.second));
    else
        cover = area;
    return outer_edge.y.till + style.shadow.offset.second;
}

void CommandSymbol::ShiftBy(double y)
{
    ArcCommand::ShiftBy(y);
    if (outer_edge.y.IsInvalid()) return;
    outer_edge.y.Shift(y);
}

void CommandSymbol::PostPosProcess(MscCanvas &/*cover*/, double /*autoMarker*/)
{
    if (!outer_edge.y.IsInvalid()) return;
    //We used markers, caculate "area" and "outer_edge.y" now
    if (vpos.src.length())
        outer_edge.y.from = chart->Markers.find(vpos.src)->second.second;
    if (vpos.dst.length())
        outer_edge.y.till = chart->Markers.find(vpos.dst)->second.second;

    if (vpos.src.length()==0)
        outer_edge.y.from = outer_edge.y.till - ysize.second;
    if (vpos.dst.length()==0)
        outer_edge.y.till = outer_edge.y.from + ysize.second;

    if (outer_edge.y.from > outer_edge.y.till)
        std::swap(outer_edge.y.from, outer_edge.y.till);
    else if (outer_edge.y.from == outer_edge.y.till)
        outer_edge.y.Expand(ysize.second/2);

    outer_edge.y.Expand(style.line.LineWidth()/2);

    CalculateAreaFromOuterEdge();
}

void CommandSymbol::CalculateAreaFromOuterEdge()
{
    switch (symbol_type) {
        case ARC:
            area = Contour(outer_edge.CenterPoint(), outer_edge.x.Spans()/2,
                           outer_edge.y.Spans()/2);
            break;
        case RECTANGLE:
            area = style.line.CreateRectangle_OuterEdge(outer_edge);
            break;
        case ELLIPSIS:
            const double r = outer_edge.x.Spans()/2;
            XY c(outer_edge.x.MidPoint(), outer_edge.y.from + r);
            area = Contour(c, r, r);
            area += Contour(c + XY(0, (2+2*ellipsis_space_ratio)*r), r, r);
            area += Contour(c + XY(0, (4+4*ellipsis_space_ratio)*r), r, r);
            break;
    }
    area.arc = this;
    area.mainline = Block(Range(0, chart->total.x), area.GetBoundingBox().y);
}

void CommandSymbol::Draw(MscCanvas &canvas, DrawPassType pass)
{
    if (pass!=draw_pass) return;
    switch (symbol_type) {
        case ARC:
        case ELLIPSIS:
            canvas.Shadow(area, style.shadow);
            canvas.Fill(area.CreateExpand(-style.line.LineWidth()/2-style.line.Spacing()),
                        style.fill);
            canvas.Line(area.CreateExpand(-style.line.LineWidth()/2), style.line);
            break;
        case RECTANGLE:
            //canvas operations on blocks take the midpoint
            const Block mid = outer_edge.CreateExpand(-style.line.LineWidth()/2);
            canvas.Shadow(mid, style.line, style.shadow);
            canvas.Fill(mid, style.line, style.fill);
            canvas.Line(mid, style.line);
            break;
    }
}

