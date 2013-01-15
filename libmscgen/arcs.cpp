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

/** @defgroup libmscgen The engine of Msc-generator.
 
  The mscgen library contains functions to 
  - parse signalling charts 
  - to lay out charts
  - to draw the chart onto carious cairo surfaces
  - to parse the chart for color syntax highlighting purposes, 
    collect and organize hints; and
  - to help a Windows client with auxiliary functions (controls, grouping state,
    tracking rectangle generation, header size calculation for autosplit, etc.)

  # Terminology

  - An *entity* is something the messages go in between. They have three classes
    in libmscgen: one Entity per entity of the chart, one EntityDef for every
    occurence of that entity (definition, change of status, etc.) and an
    CommandEntity for each group of entities mentioned together.
  - An entity is a *group entity* if it contains other entities. We call these
    *child* and the group entity the *parent*.
  - A group entity is *collapsed* if its 'collapsed' attribute says so,
    and thus its child entities are not shown.
  - An entity is *active* if it shows on the chart. It is not active if
    it is a child of a collapsed group entity.
  - An *arc* is a command in the chart terminated by a semicolon. An arc
    can be an arrow, an entity definition, a box, an option, etc.
    All arcs are descendants of ArcBase.
  - The chart is represented by an Msc object. It incorporates all arcs,
    entities and settings.
  - A *context* is a set of settings valid at a given place in the file during 
    parse. It includes the current definition of colors, styles and a few global
    option, such as 'compress', 'numbering', 'indicator', 'slant_angle' and 'hscale'.
    They also include the current comment line and fill style, the default font
    and current numbering style. Contexts are captured by class Context.
  - A *scope* in the chart file is an area enclosed between '{' and '} symbols.
    Any change you make to the context is valid only inside the scope.
  - A *style* is a set of attributes (line, vline, fill, vfill, shadow, 
    arrow, text, note, solid, side, numbering, compress, indicator and
    makeroom). Not all attributes have to be set (making a style *incomplete*)
    and in some cases not all of them *can be set*. Styles are capture by
    class Style.
  - The *coverage* or *area* of an element is the shape it covers on the 2D
    canvas of the chart. It is described by a Contour object or by an Area
    object. The latter contains a pointer back to the element and a mainline.
  - The *mainline* of an element is mostly a horizontal stripe of the chart
    area as wide as the chart itself. It denotes how much elements below
    a given element can be compressed (shifted upwards). In general, an element
    can be shifted upwards as high as it hits an element above it. This, 
    however could result in rearranging element orders, if a later element
    is shifted higher besides an element before it. E.g., if we have 5 
    entities and two arrows: a->b followed by d->e. In this case d->e must
    come later than a->b even with compress, thus it cannot be shifted higher
    than a->b. This is prevented by the mainline. The mainline of a->b is a 
    thin horizontal rectangle, covering the line of the a->b arrow. Likewise
    the mainline of d->e. At laying out d->e with compress we ensure that 
    it does not overlap with any other element (such as a->b, which it would
    not since it is besides a->b), but also that the mainlines do not overlap.
    This will make d->e always being somewhat lower than a->b. If, however,
    a->b has a long multiline label, d->e can be shifted upwards besides this 
    label, but not higher than the arrow of a->b. In this sense the mainline
    captures the vertical position of the core of the element.
    For slanted arrows, the mainline is no longer just a horizontal stripe
    of the chart. It his so left and right from the arrow, but along the arrow
    it slants with it.

  # Parsing

  Parsing is defined in language.yy and language2.ll yacc and lex files.
  There are two parsing processes (MscParse and CshParse), one for
  generating the drawable Msc class and another to collect color syntax
  highlighting and hinting data into class Csh. Both parsing process
  is described in the same yacc and lex file, but those are compiled twice
  with different options and type definitions one for Msc one for Csh.
  The former are language.h, language.cc, language2.h, language2.cc; whereas
  the latter result in colorsyntax.h, colorsyntax.cc, colorsyntax2.h and
  colorsyntax2.cc. Using the same yacc and lex sources enables easy
  consistency between drawn charts and CSH.

  The parsing for Msc uses a location type (YYLTYPE) that consists of a line and col
  number, since this is how we display error messages. Whereas the csh parsing
  uses a location type that contains the number of bytes since the beginning 
  of the file, since the Windows RichEdit control uses such location values.

  During both parsing type we maintain a stack of contexts. For Msc it is a
  stack of Context classes, for csh it is a stack of CshContext classes.
  The latter one is simpler, it merely contains the colors and style names,
  because only these are needed for autocompletion.

  When parsing for Csh, we also collect potential extensions of a fragment
  under the cursor. We call these 'Hints'.

  When parsing for Msc, we collect Entities and Arcs. Further layout and drawing
  operations (all called from a member of Msc) work on the list (or rather tree)
  of arcs. Tree, because arcs may contain other arcs, as in case of a box, for
  example.

  # The lifecycle of an Arc.

    @<parsing starts>
    1. Construction: Only basic initialization is done here. For arcs with style (descendants of ArcLabelled)
       we fetch the appropriate style from Msc::Contexts.back()
       We also look up entities the arc refers to (and create them if needed), so after this point
       we have EIterators pointing to the Msc::AllEntities list.
       When a CommandNote is constructed, it is attached to the last element that can be 
       attached to (by calling Msc::last_notable_arc->AttachNote()). By this the ownership of the
       CommandNote object falls to its targeted arc.
    2. AddAttributeList: Add attributes to arc. This function must always be called (with NULL if no attributes)
       We have to do this before we can create a list of Active Entities, since attributes can 
       result in the implicit definition of new entities.
    3. Additional small functions (SetLineEnd, ArcBox::SetPipe, CommandEntity::ApplyPrefix, etc.)

    @<parsing ends>
    @<here we construct a list of active entities from AllEntities to ActiveEntities>

    4. PostParseProcess: This is called (also recursively for children arcs in the tree) to do the following.
       a) Determine the non-specified entities for boxes, pipes and arrows. Note that the EIterators 
          received as parameters ("left", "right") are iterators of AllEntities and not ActiveEntities.
       b) Calculate numering for labels. Also, if the entity has a name, store the arc in 
          Msc::ReferenceNames;
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
          still kept as content and steps below (\#6-9) shall be performed on them. See \#h below.
       e) Combine CommandEntities and ArcIndicators one after the other
       f) Set up some extra variables
       g) Print error messages. 
       h) Decide on replacement. PostParseProcess have a "hide" parameter telling if this arc will be hidden 
          due to a collapsed box (and no other reason). In this case we return NULL, if the arc does not 
          want to receive #6-9 below. Else we return "this".
          An arc can also become hidden due to collapsed entities - this was determined in #4c above. If 
          the arc becomes hidden, it can get replaced to an ArcIndicator if the entity in question has its
          "indicator" in "running_style" set. Else we just retuen NULL.
       i) If the node is kept, we move its floating notes to "Msc::FloatingNotes" via "MoveNotesToChart"
          called from "Msc::PostParseProcessArcList".
       j) For notes and comments, we decide who is the real target and attach the note/command here.
       k) For labels, we replace the remaining escapes to actual values, except for "\r()"
          element references. Those will be done in FinalizeLabels().
       This function can only be called once, as it changes arcs (e.g., you do not want to
       increment numbering twice). Often the arc needs to be changed to a different one, in this case the
       return pointer shall be used. If the return pointer == this, the arc shall not be replaced.
    5. FinalizeLabels: This is called recursive to fill in all escapes in labels with their
       actual values. Here we substitute number, color, style references and so on. This fills
       the "parsed_label".
    6. Width: This is also called recursively. Here each arc can place the distance requirements between
       entities. Using the data Msc::CalculateWithAndHeight() can place entities dynamically if hscale==auto.
       If hcale!=auto, entities have fixed positions, but this function is still called (so as it can be used
       to calculate cached values).

    @<here we calculate the Entity::pos for all entities in ActiveEntities in Msc::CalculateWidthHeight>

    7. Layout: This is a key function, returning the vertical space an element(/arc) occupies. It also places
       the contour of the element in its "cover" parameter. The former (height) is used when compress is off and
       the latter (contour) if compress is on. In the latter case the entity will be placed just below entities
       abover such that their contours just touch but do not overlap. Also fills in
       Element::area and area_draw with contours that will be used to detect if a mouse pointer is
       inside the arc or not and to draw a tracking contour over the arc. Observe
       - contour returned in cover is used for placement and should contain shadows
       - area is used to detect if the mouse is within, should not contain shadows
       - area_draw is used to draw, it should be a frame for boxes and pipes with content, not the contour of the box.
       Layout can also store some pre-computed values and contours to make drawing faster.
       Layout always places the element at the vertical position=0. Any contour should assume that.
       Finally, Layout() also fills in "area_important", containing the 'important' part of the
       element's cover (text, arrowheads, symbols). The Note layout engine will use this, to avoid
       covering these areas. Also, "def_note_target" is filled in, this is where a note points to
       by default.
    7. ShiftBy: During the placement process this is called to shift an entity in the vertical direction
       (usually downwards). This should update area, area_draw and any other cached variable.
       ArcBase::yPos contains the sum of these shifts. This function can be called multiple times.
    8. CollectPageBreaks: This walks the tree of elements and each CommandPageBreak places its
       y coordinate into Msc::yPageStart. This function can be called multiple times during
       automatic pagination, but only once if no automatic pagination is done.
    9. PageBreak: This is called by an automatic pagination process (if any), when the element is
       cut in half by a page break. The element can rearrange itself to accomodate the page break, by 
       shifting half of it down or can indicate that it cannot rearrange itself and shall be fully 
       shifted to the next page. Elements with `keep_together` set to false are not called, those are
       simply cut in half abruptly.
       This function can be called multiple times, if the element spans multiple page breaks. In the
       second and subsequent calls only the last chunk shall be split.
    10. PlaceWithMarkers: By now all positions and height values are final, except for notes & verticals. 
       (Comments are also placed with their target.) We go through the tree and calculate position & cover for
       verticals. This is needed as a separate run, just to do it before placing notes.
    
    <here we place floating notes in Msc::PlaceFloatingNotes>

   11. PostPosProcess: Called after the last call to ShiftBy. Here all x and y positions of all elements are set.
       Here entity lines are hidden behind text and warnings/errors are generated which require vertical position 
       to decide. We also expand all "area" and "area_draw" members, so that contours look better in tracking mode.
       No error messages shall be printed after this function by arc objects. (Msc will print some, if
       page sizes do not fit, but that is under control there.)
   12. Draw: This function actually draws the chart to the "canvas" parameter. This function can rely on cached 
       values in the elements. It can be called several times and should not change state of the element 
       including the cached values.
   13. Destructor.

    All the above functions are called from the Msc object. #1-#3 are called from Msc::ParseText, whereas
    the remainder from the Msc:: memeber functions of similar names, with the exception of ShiftBy, which is
    called from Msc::Layout and Msc::PlaceListUnder.

    Color Syntax Highlighting support also has functions in Arcs.
    1. AttributeNames: A static function that inserts the attributes valid for this type of arc into a Csh object.
    2. AttributeValues: A static function that inserts the possible values for a given attribute into a Csh object.
    Both of the above calls include a callback function to draw the small pictograms for hints. These functions
    are in fact only used for the hinting process and not for actual syntax highlighting (which requires no
    support from Arc objects, since those are not created during a Csh parse.
*/

/** @defgroup libmscgen_files The files of the mscgen library.
 * @ingroup libmscgen
 *
 * The files are organized in the following logical hierarchy.
   - `version`, `error` and `color` Provide basic utilities for line numbering, errors 
   and color.
   - `attribute`, `arrowhead`, `stringparse` and `numbering` provide element 
     attribute handling for lines, fills, shadows, arrows, text formatting and
     numbering.
   - `style` collects attribues to a common Style class and add Context and
     Design.
   - `mscdrawer` contains MscCanvas, a helper class to encapsulate a cairo
     surface and context. We add logic for pages and fallback operations here
     (e.g., for output types (like WMF) that do not support certain operations, 
     like dashed lines.)
   - `trackable`, `entity`, `arcs` and `commands` define the classes that represent 
     the elements of the chart. Element as root, entities and related
     classes in `entity` and various chart elements, all descendants of ArcBase
     (`arcs` and `commands` are loose groups of chart commands and various arcs, 
     boxes and the like).
   - `language` and `language2` are files generated by yacc and flex from
     `language.yy` and `language2.ll` respectively. They contain the actual
     parsing routine that we encapsulate in MscParse(). `parse_tools` provide
     (usually string manipulation) helpers to parsing. During parsing we allocate
     classes for commands, arc and entities and store them in an Msc object.
   - `msc` brings together everything including parsing, layout and drawing by
     defining the main class Msc.
   - `csh`, `colorsyntax` and `colorsyntax2` implement Color Syntax Highlighting
     and hinting (auto-completion) functionality. `csh` implements the main 
     class for this (Csh) which collects color syntax highlighting info and
     hints. The other two files are generated by yacc and flex from
     `language.yy` and `language2.ll` respectively, similar to 
     `language` and `language2`, but with different preprocessor directives in place.
     I use the same yacc and lex files so as to keep the grammar for
     actual chart generation and coloring the same. This pollutes the
     grammar definition, though.

 * @file arcs.cpp The basic definitions or arcs that are not commands.
 * @ingroup libmscgen_files */

#include <math.h>
#include <cassert>
#include <iostream>
#include <algorithm>
#include "msc.h"

using namespace std;

template class PtrList<ArcBase>;

ArcBase::ArcBase(MscArcType t, Msc *msc) :
    Element(msc), valid(true), compress(false), parallel(false),
    keep_together(true), keep_with_next(false),
    type(t)
{
    if (msc) 
        compress = msc->Contexts.back().compress.second;
    had_add_attr_list = false;
}

Area ArcBase::GetCover4Compress(const Area &a) const
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
    if (a.Is("refname")) {
        if (!a.EnsureNotClear(chart->Error, STYLE_ARC)) return true;
        auto i = chart->ReferenceNames.find(a.value);
        if (i==chart->ReferenceNames.end()) {
            chart->ReferenceNames[a.value].linenum = a.linenum_value.start;
            refname = a.value;
            return true;
        }
        chart->Error.Error(a, true, "The reference name '" + a.value + "' is already assigned. Ignoring it.");
        chart->Error.Error(i->second.linenum, a.linenum_value.start, "This is the location of the previous assignment.");
        return true;
    }
    if (a.Is("keep_together")) {
        if (!a.EnsureNotClear(chart->Error, STYLE_ARC)) return true;
        if (!a.CheckType(MSC_ATTR_BOOL, chart->Error)) return true;
        keep_together = a.yes;
        return true;
    }
    if (a.Is("keep_with_next")) {
        if (!a.EnsureNotClear(chart->Error, STYLE_ARC)) return true;
        if (!a.CheckType(MSC_ATTR_BOOL, chart->Error)) return true;
        keep_with_next = a.yes;
        return true;
    }
    return Element::AddAttribute(a);
}

void ArcBase::AttributeNames(Csh &csh)
{
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "compress", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "parallel", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "refname", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "keep_together", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "keep_with_next", HINT_ATTR_NAME));
    Element::AttributeNames(csh);
}

bool ArcBase::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr,"compress")||
        CaseInsensitiveEqual(attr,"parallel") ||
        CaseInsensitiveEqual(attr,"keep_together") ||
        CaseInsensitiveEqual(attr,"keep_with_next")) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+"yes", HINT_ATTR_VALUE, true, CshHintGraphicCallbackForYesNo, CshHintGraphicParam(1)));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+"no", HINT_ATTR_VALUE, true, CshHintGraphicCallbackForYesNo, CshHintGraphicParam(0)));
        return true;
    }
    if (CaseInsensitiveEqual(attr,"refname")) 
        return true;
    if (Element::AttributeValues(attr, csh)) return true;
    return false;
}

string ArcBase::PrintType(void) const
{
    static const char arcnames[][25] = {
        "invalid_arc_type",
        "arc_solid", "arc_solid_bidir", "arc_dotted", "arc_dotted_bidir",
        "arc_dashed", "arc_dashed_bidir", "arc_double", "arc_double_bidir",
        "emph_solid", "emph_dotted", "emph_double", "arc_parallel",
        "arc_disco", "arc_divider", "arc_label",
        "command_heading", "nudge", "command_entity",
        "hspace", "vspace", "symbol", "note"};
    return arcnames[int(type)-1];
}

ArcBase* ArcBase::PostParseProcess(MscCanvas &/*canvas*/, bool /*hide*/,
                                   EIterator &/*left*/, EIterator &/*right*/,
                                   Numbering &/*number*/, bool top_level, 
                                   Element **target)
{
    at_top_level = top_level;
    if (CanBeNoted()) *target = this;
    return this;
}

void ArcBase::FinalizeLabels(MscCanvas &)
{
    if (refname.length())
        chart->ReferenceNames[refname].arc = this;
}

void ArcBase::Layout(MscCanvas &canvas, AreaList &cover)
{
    height = 0;
    CommentHeight(canvas, cover);
}

void ArcBase::PostPosProcess(MscCanvas &canvas)
{
    _ASSERT(had_add_attr_list);
    if (valid) 
        Element::PostPosProcess(canvas); //also adds "this" to chart->AllArcs
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

void ArcIndicator::Width(MscCanvas &, EntityDistanceMap &distances)
{
    if (*src != *dst) return; //no width requirements for Indicators not exactly on an entity
    //If we are exactly on an entity line add left and right req for boxes potentially around us.
    const double width = indicator_style.line.LineWidth() + indicator_size.x/2;
    distances.InsertBoxSide((*src)->index-1, 0, width);
    distances.InsertBoxSide((*src)->index,   width, 0);
}

void ArcIndicator::Layout(MscCanvas &canvas, AreaList &cover)
{
    yPos = chart->emphVGapOutside;
    const double x = (chart->XCoord((*src)->pos) + chart->XCoord((*dst)->pos))/2;
    const Block b = GetIndicatorCover(XY(x, chart->emphVGapOutside));
    area = b;
    area.mainline = Block(chart->GetDrawing().x, b.y);
    area_important = b;
    chart->NoteBlockers.Append(this);
    height = b.y.till + chart->emphVGapOutside;
    //TODO add shadow to cover
    cover = GetCover4Compress(area);
    CommentHeight(canvas, cover);
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
    //Element members
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
        style.numbering.second = chart->Contexts.back().numbering.second;
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
    //Find position of label attribute (if any), prepend it via an escape
    if (l)
        for (AttributeList::iterator i = l->begin(); i!=l->end(); i++)
            if ((*i)->Is("label")) 
                (*i)->value.insert(0, (*i)->linenum_value.start.Print());
    //Add attributest 
    ArcBase::AddAttributeList(l);
    //compress went to the style, copy it
    if (style.compress.first)
        compress = style.compress.second;
    //Then convert color and style names in labels
    if (label.length()>0)
        //we can start with a dummy pos, since the label's pos is prepended
        //during parse for colon labels and during AddAttributeList for others
        StringFormat::ExpandReferences(label, chart, file_line(), &style.text,
                                          false, true, StringFormat::LABEL);
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
    if (CaseInsensitiveEqual(attr,"label") || CaseInsensitiveEqual(attr,"refname")) {
        return true;
    }
    if (CaseInsensitiveEqual(attr,"number")) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+"yes", HINT_ATTR_VALUE, true, CshHintGraphicCallbackForYesNo, CshHintGraphicParam(1)));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+"no", HINT_ATTR_VALUE, true, CshHintGraphicCallbackForYesNo, CshHintGraphicParam(0)));
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
ArcBase *ArcLabelled::PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right, 
                                       Numbering &number, bool /*top_level*/, Element **target)
{
    if (!valid) return NULL;
    //We do everything here even if we are hidden (numbering is not impacted by hide/show or collapse/expand)
    if (label.length()!=0 && style.numbering.second) {
        number.SetSize((unsigned)numberingStyle.Size()); //append 1s if style has more levels
        if (concrete_number >= 0)
            number.Last() = concrete_number;
        number_text = numberingStyle.Print(number);
        //Now remove escapes from the number text (if any)
        //Recreate the text style at the point where the label will be inserted
        StringFormat basic = style.text;
        basic.Apply(label.c_str());
        //At this point the number text must be processed using StringFormat::ExpandReferences
        //to expand remaining empty \c(), \s(), etc escapes.
        //We use a dummy linenum, as we should not get ANY errors here...
        StringFormat::ExpandReferences(number_text, chart, file_line(), &basic,
            false, true, StringFormat::LABEL);
        //insert us among the references if we have any name
        if (refname.length()) 
            chart->ReferenceNames[refname].number_text = number_text;
        ++number;
    }
    return ArcBase::PostParseProcess(canvas, hide, left, right, number, at_top_level, target);
}

void ArcLabelled::FinalizeLabels(MscCanvas &canvas) 
{
    ArcBase::FinalizeLabels(canvas);
    if (label.length()==0) return;
    string pre_num_post;
    if (label.length()!=0 && style.numbering.second) 
        pre_num_post = numberingStyle.pre.second + number_text + numberingStyle.post.second;
    //We add empty num and pre_num_post if numberin is turned off, to remove \N escapes
    StringFormat::AddNumbering(label, number_text, pre_num_post);
    //Next we add reference numbers to labels, and also kill off any \s or the like
    //escapes that came with numbers
    //we can start with a dummy pos, since the label's pos is prepended
    //during parse for colon labels and during AddAttributeList for others
    StringFormat copy(style.text);
    StringFormat::ExpandReferences(pre_num_post, chart, file_line(), 
        &copy, true, true, StringFormat::LABEL);
    parsed_label.Set(label, canvas, style.text);
}

void ArcLabelled::PostPosProcess(MscCanvas &canvas)
{
	//If there is a vline or vfill in the current style, add that to entitylines
    if ((style.f_vline &&(style.vline.width.first || style.vline.type.first || style.vline.color.first)) ||
        (style.f_vfill && !style.vfill.IsEmpty())) {
		MscStyle toadd;
        if (style.f_vline) toadd.vline = style.vline;
        if (style.f_vfill) toadd.vfill = style.vfill;
        for(EIterator i = chart->ActiveEntities.begin(); i!=chart->ActiveEntities.end(); i++)
            if (!chart->IsVirtualEntity(*i))
                (*i)->status.ApplyStyle(area.GetBoundingBox().y, toadd);
	}
    ArcBase::PostPosProcess(canvas);
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
    defaultDesign.styles.GetStyle("arrow").AttributeNames(csh);
}

bool ArcArrow::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr,"angle")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", HINT_ATTR_VALUE, false));
        return true;
    }
    if (defaultDesign.styles.GetStyle("arrow").AttributeValues(attr, csh)) return true;
    if (ArcLabelled::AttributeValues(attr, csh)) return true;
    return false;
}
//////////////////////////////////////////////////////////////////////////////////////

ArcSelfArrow::ArcSelfArrow(MscArcType t, const char *s, file_line_range sl,
                           Msc *msc, const MscStyle &st, double ys) :
    ArcArrow(t, msc, st), YSize(ys), XSizeUnit(0.375)
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

ArcBase* ArcSelfArrow::PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right, 
                                        Numbering &number, bool top_level, Element **target)
{
    if (!valid) return NULL;
    if (chart->ErrorIfEntityGrouped(src, (*src)->file_pos)) return NULL;

    //Add numbering, if needed
    ArcLabelled::PostParseProcess(canvas, hide, left, right, number, top_level, target);

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

void ArcSelfArrow::Width(MscCanvas &, EntityDistanceMap &distances)
{
    if (!valid) return;
    distances.Insert((*src)->index, DISTANCE_RIGHT, chart->XCoord(XSizeUnit)+src_act+style.line.LineWidth()/2);
    distances.Insert((*src)->index, DISTANCE_LEFT, parsed_label.getTextWidthHeight().x+src_act);
}

void ArcSelfArrow::Layout(MscCanvas &canvas, AreaList &cover)
{
    height = 0;
    if (!valid) return;
    yPos = 0;
    xy_s = style.arrow.getWidthHeight(isBidir(), MSC_ARROW_START);
    xy_e = style.arrow.getWidthHeight(isBidir(), MSC_ARROW_END);
    xy_s.y = ceil(xy_s.y);
    xy_e.y = ceil(xy_e.y);
    wh.x = ceil(chart->XCoord(XSizeUnit));
    wh.y = ceil(2*YSize);
    dx = chart->XCoord(src);
    sx = 0;

    double y = chart->arcVGapAbove;
    area = parsed_label.Cover(sx, dx-src_act, y);
    area.arc = this;
    area_important = area;
    const Block arrow_box(dx+src_act, ceil(dx+src_act+wh.x), y, ceil(y+xy_s.y+wh.y+xy_e.y));
    area += arrow_box;
    area.mainline = Block(chart->GetDrawing().x, Range(y - chart->nudgeSize/2, y + wh.y + chart->nudgeSize/2));
    //Now add arrowheads to the "area_important", and a small block if they are NONE
    XY point = XY(dx+src_act, xy_s.y + chart->arcVGapAbove);
    if (style.arrow.GetType(isBidir(), MSC_ARROW_START) == MSC_ARROW_NONE)
        area_important += Block(point.x-chart->compressGap/2, point.x+chart->compressGap/2,
        point.y-chart->compressGap/2, point.y+chart->compressGap/2);
    else
        area_important += style.arrow.Cover(point, 0, true,  isBidir(), MSC_ARROW_START, style.line, style.line);
    point.y += 2*YSize;
    if (style.arrow.GetType(isBidir(), MSC_ARROW_END) == MSC_ARROW_NONE)
        area_important += Block(point.x-chart->compressGap/2, point.x+chart->compressGap/2,
        point.y-chart->compressGap/2, point.y+chart->compressGap/2);
    else
        area_important += style.arrow.Cover(point, 0, false, isBidir(), MSC_ARROW_END, style.line, style.line);
    chart->NoteBlockers.Append(this);
    height = area.GetBoundingBox().y.till + chart->arcVGapBelow;
    cover = GetCover4Compress(area);
    CommentHeight(canvas, cover);
}

void ArcSelfArrow::PostPosProcess(MscCanvas &canvas)
{
    if (!valid) return;
    ArcArrow::PostPosProcess(canvas);

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

    parsed_label.Draw(canvas, sx, dx-src_act, y);
    y += xy_s.y;

    if (style.line.radius.second < 0) {
        //draw an arc
        canvas.Line(Edge(XY(dx+src_act, y+YSize), wh.x, wh.y/2, 0, 270, 90), style.line);
    } else {
        //draw (part of) a rounded rectangle
        canvas.Clip(dx+src_act, chart->GetDrawing().x.till, chart->GetDrawing().y.from, chart->GetDrawing().y.till);
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
    segment_types.push_back(t);
    if (chart) slant_angle = chart->Contexts.back().slant_angle.second;
};

ArcDirArrow::ArcDirArrow(const EntityList &el, bool bidir, const ArcLabelled &al) :
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
    if (chart) slant_angle = chart->Contexts.back().slant_angle.second;
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
        if (middle.size()==0 && *src == chart->LSide &&
            (*dst)->pos > (*mid)->pos && m!=NULL)
            src = chart->FindAllocEntity(RSIDE_ENT_STR, ml);
        middle.push_back(dst);
        linenum_middle.push_back(linenum_dst);
        dst = mid;
        linenum_dst = ml.start;
        segment_types.push_back(t);
    } else {
        //check for this situation: <-b<-a (where a is left of b)
        if (middle.size()==0 && *dst == chart->LSide &&
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

ArcBase *ArcDirArrow::PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right, 
                                       Numbering &number, bool top_level, Element **target)
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
    ArcLabelled::PostParseProcess(canvas, hide, left, right, number, top_level, target);

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
    const EIterator _left = chart->AllEntities.Find_by_Ptr(chart->LSide); // leftmost entity;
    const EIterator _right = chart->AllEntities.Find_by_Ptr(chart->RSide);  // rightmost entiry
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
    return this;
}

void ArcDirArrow::FinalizeLabels(MscCanvas &canvas)
{
    ArcArrow::FinalizeLabels(canvas);
    if (parsed_label.getTextWidthHeight().y==0) return;
    //Insert a small extra spacing for the arrow line
    double lw_max = style.line.LineWidth();
    for (unsigned i=0; i<segment_lines.size(); i++)
        lw_max = std::max(lw_max, segment_lines[i].LineWidth());
    parsed_label.AddSpacing(0, ARROW_TEXT_VSPACE_ABOVE + lw_max + ARROW_TEXT_VSPACE_BELOW);
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

void ArcDirArrow::Layout(MscCanvas &canvas, AreaList &cover)
{
    height = 0;
    if (!valid) return;
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
    //aH.y is _half_ the height of the arrowhead (the height above/below the centerline)
    //aH.x is the width on one side on the entity line only.
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
        area.arc = this;
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
    const Block total(sx + (sx<dx ? s_act : -s_act), dx - (sx<dx ? d_act : -d_act), 0, y+2*lw_max+aH);
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
    area_important = area; //the text and arrowheads
    //Add small blocks if there is no end or start arrowhead
    if (style.arrow.GetType(isBidir(), MSC_ARROW_START) == MSC_ARROW_NONE)
        area_important += Block(sx-chart->compressGap/2, sx+chart->compressGap/2,
                                centerline-chart->compressGap/2, centerline+chart->compressGap/2);
    if (style.arrow.GetType(isBidir(), MSC_ARROW_START) == MSC_ARROW_NONE)
        area_important += Block(sx-chart->compressGap/2, sx+chart->compressGap/2,
                                centerline-chart->compressGap/2, centerline+chart->compressGap/2);
    for (unsigned i=0; i<xPos.size()-1; i++) {
        const double lw2 = ceil(segment_lines[i].LineWidth()/2);
        //x coordinates below are not integer- but this will be merged with other contours - so they disappear
        area += clip_area * Block(xPos[i]+margins[i].second, xPos[i+1]-margins[i+1].first, y-lw2, y+lw2);
    }
    //Add a horizontal line to area to bridge the gap across activated entities
    area_to_note2 = Block(sx, dx, centerline, centerline).Expand(0.5);
    CalculateMainline(std::max(lw_max, chart->nudgeSize+1.0));
    if (slant_angle != 0) {
        //OK: all of sx, dx, sx_text, dx_text, cx_text, xPos, act_size, margins
        //are in the transformed space
        //Now we transfrom "area" and "text_cover", too
        const XY c(sx, yPos+centerline);
        area.RotateAround(c, slant_angle);
        area_important.RotateAround(c, slant_angle);
        area_to_note2.RotateAround(c, slant_angle);
        text_cover.RotateAround(c, slant_angle); 
        clip_area.RotateAround(c, slant_angle); 
    }
    chart->NoteBlockers.Append(this);
    cover = GetCover4Compress(area);
    if (slant_angle==0)
        height = std::max(y+max(aH, lw_max/2), chart->arcVGapAbove + text_wh.y) + chart->arcVGapBelow;
    else
        height = area.GetBoundingBox().y.till;
    CommentHeight(canvas, cover);
}


void ArcDirArrow::CalculateMainline(double thickness)
{
    if (slant_angle == 0) 
        area.mainline = Block(chart->GetDrawing().x, Range(yPos+centerline - thickness/2, yPos+centerline + thickness/2));
    else {
        thickness /= cos_slant;
        const double src_y = yPos+centerline;
        const double dst_y = sin_slant*(dx-sx) + src_y;
        const double real_dx = sx + cos_slant*(dx-sx);
        if (sx<dx) {
            const XY ml[] = {XY(chart->GetDrawing().x.from,   src_y-thickness/2), XY(sx, src_y-thickness/2),
                             XY(real_dx, dst_y-thickness/2), XY(chart->GetDrawing().x.till, dst_y-thickness/2),
                             XY(chart->GetDrawing().x.till, dst_y+thickness/2), XY(real_dx, dst_y+thickness/2),
                             XY(sx, src_y+thickness/2), XY(chart->GetDrawing().x.from, src_y+thickness/2)};
            area.mainline.assign_dont_check(ml);
        } else {
            const XY ml[] = {XY(chart->GetDrawing().x.from, dst_y-thickness/2), XY(real_dx, dst_y-thickness/2),
                             XY(sx, src_y-thickness/2), XY(chart->GetDrawing().x.till, src_y-thickness/2),
                             XY(chart->GetDrawing().x.till, src_y+thickness/2), XY(sx, src_y+thickness/2),
                             XY(real_dx, dst_y+thickness/2), XY(chart->GetDrawing().x.from, dst_y+thickness/2)};
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
    //During the Layout() process, and can be considered complete only here
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
            !chart->IsVirtualEntity(*temp[i])) {
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


void ArcDirArrow::PostPosProcess(MscCanvas &canvas)
{
    if (!valid) return;
    ArcArrow::PostPosProcess(canvas);
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
        parsed_label.Draw(canvas, sx_text, dx_text, yPos + chart->arcVGapAbove, cx_text, slant_angle!=0);
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
}

//This invocation is from ArcBoxSeries::PostParseProcess
ArcBigArrow::ArcBigArrow(const EntityList &el, bool bidir, const ArcLabelled &al,
    const ArcSignature *s)
    : ArcDirArrow(el, bidir, al), sig(s)
{
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
    defaultDesign.styles.GetStyle("blockarrow").AttributeNames(csh);
}

bool ArcBigArrow::AttributeValues(const std::string attr, Csh &csh)
{
    if (defaultDesign.styles.GetStyle("blockarrow").AttributeValues(attr, csh)) return true;
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

ArcBase* ArcBigArrow::PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right,
                                       Numbering &number, bool top_level, Element **target)
{
    if (!valid) return NULL;
    //Determine src and dst entity, check validity of multi-segment ones, add numbering, etc
    ArcBase *ret = ArcDirArrow::PostParseProcess(canvas, hide, left, right, number, top_level, target);
    //Finally copy the line attribute to the arrow, as well (arrow.line.* attributes are annulled here)
    style.arrow.line = style.line;
    return ret;
}

void ArcBigArrow::FinalizeLabels(MscCanvas &canvas)
{
    //Skip the part in ArcDirArrow adding first line spacing
    ArcArrow::FinalizeLabels(canvas);
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
    twh.y = std::max(twh.y, style.text.getCharHeight(canvas));
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


void ArcBigArrow::Layout(MscCanvas &canvas, AreaList &cover)
{
    if (!valid) return;
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
    area_important = style.arrow.BigHeadContour(xPos, act_size, sy, dy, sx<dx, isBidir(), &segment_lines, chart->compressGap);
    area_important += parsed_label.Cover(sx_text, dx_text, sy+segment_lines[stext].LineWidth() + chart->emphVGapInside, cx_text);
    area_to_note2 = Block(sx, dx, (sy+dy)/2, (sy+dy)/2).Expand(0.5);
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
        area_important.RotateAround(c, slant_angle); 
        area_to_note2.RotateAround(c, slant_angle);
    }
    chart->NoteBlockers.Append(this);
    cover = GetCover4Compress(area);

    height = area.GetBoundingBox().y.till + chart->arcVGapBelow + style.shadow.offset.second;
    CommentHeight(canvas, cover);
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

void ArcBigArrow::PostPosProcess(MscCanvas &canvas)
{
    if (!valid) return;
    CheckSegmentOrder(yPos + centerline);
    if (!valid) return;
    if (sig) {
        controls.push_back(MSC_CONTROL_EXPAND);        
        controls.push_back(MSC_CONTROL_COLLAPSE);        
    }
    ArcArrow::PostPosProcess(canvas); //Skip ArcDirArrow
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
    parsed_label.Draw(canvas, sx_text, dx_text, sy+segment_lines[stext].LineWidth() + chart->emphVGapInside, cx_text);
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
    switch(t) {
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
    return ArcArrow::AddAttribute(a);
}

void ArcVerticalArrow::AttributeNames(Csh &csh)
{
    ArcLabelled::AttributeNames(csh);
    defaultDesign.styles.GetStyle("vertical").AttributeNames(csh);
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
    if (defaultDesign.styles.GetStyle("vertical").AttributeValues(attr, csh)) return true;
    if (ArcLabelled::AttributeValues(attr, csh)) return true;
    return false;
}


Element* ArcVerticalArrow::AttachNote(CommandNote *note)
{
    _ASSERT(note);
    chart->Error.Error(note->file_pos.start, "Notes cannot be attached to Verticals. Ignoring note.");
    chart->Error.Error(file_pos.start, note->file_pos.start, "Here is the vertical this note is attached to.");
    delete note;
    return NULL;
}


ArcBase* ArcVerticalArrow::PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right,
                                            Numbering &number, bool top_level, Element **target)
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
    ArcLabelled::PostParseProcess(canvas, hide, left, right, number, top_level, target);

    left = chart->EntityMinByPos(left, pos.entity1);
    right = chart->EntityMaxByPos(right, pos.entity1);
    left = chart->EntityMinByPos(left, pos.entity2);
    right = chart->EntityMaxByPos(right, pos.entity2);

    if (hide) 
        return NULL;

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
    if (!style.makeroom.second) return;
    //The offset is ignored during the process of setting space requirements
    const XY twh = parsed_label.getTextWidthHeight();
    const double lw = style.line.LineWidth();
    double width = twh.y;
    if (width==0)
        width = style.text.getCharHeight(canvas);
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
void ArcVerticalArrow::Layout(MscCanvas &canvas, AreaList &cover)
{
    //We will not have notes or comments, so no need to call CommentHeight()
    height = 0;
    _ASSERT(comments.size()==0);
}

void ArcVerticalArrow::ShiftBy(double y)
{
    yPos += y;
}

void ArcVerticalArrow::PlaceWithMarkers(MscCanvas &canvas, double autoMarker)
{
    if (!valid) return;
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
        width = style.text.getCharHeight(canvas);
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
    area.arc = this;
    area.SwapXY();
    area_important = parsed_label.Cover(min(sy_text, dy_text), max(sy_text, dy_text),
                      xpos-width/2+style.line.LineWidth()/2+chart->emphVGapInside);
    area_important.SwapXY();
    area_to_note2 = Block(xpos, xpos, ypos[0], ypos[1]).Expand(0.5);
    for (auto i = outer_contours.begin(); i!=outer_contours.end(); i++)
        i->SwapXY();
    chart->NoteBlockers.Append(this);
}

void ArcVerticalArrow::PostPosProcess(MscCanvas &canvas)
{
    if (!valid) return;
    //Expand area and add us to chart's all covers list
    ArcArrow::PostPosProcess(canvas);
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
    //We skip BigDrawEmptyMid. as there can not be mid-stops
    parsed_label.Draw(canvas, min(sy_text, dy_text), max(sy_text, dy_text),
                      xpos-width/2+style.line.LineWidth()/2+chart->emphVGapInside, -CONTOUR_INFINITY, true);
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
    collapsed(BOX_COLLAPSE_EXPAND), drawEntityLines(true)
{
    src = chart->FindAllocEntity(s, sl);
    dst = chart->FindAllocEntity(d, dl);

    //If both src and dst specified, order them
    if (*src!=chart->NoEntity && *dst!=chart->NoEntity)
        if ((*src)->pos > (*dst)->pos) 
            std::swap(src, dst);
    keep_together = true;
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
    keep_together = true; //we will do our own pagination if needed, we shall not be cut in half by Msc::ArcHeightList()
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
    keep_together = false;
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
    defaultDesign.styles.GetStyle("box").AttributeNames(csh);
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
    if (defaultDesign.styles.GetStyle("box").AttributeValues(attr, csh)) return true;
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
                                  Numbering &number, bool top_level, Element **target)
{
    ArcBase *ret = this;
    if (collapsed == BOX_COLLAPSE_BLOCKARROW) {
        EntityList el(false);
        //replace us with a block arrow - here we are sure to be alone in the series
        el.Append(*src);
        el.Append(*dst);
        MscDirType dir = chart->GetTouchedEntitiesArcList(content, el);
        if (dir == MSC_DIR_INDETERMINATE) dir = MSC_DIR_BIDIR;
        //remove NoEntity and side ones from el
        for (auto i = el.begin(); i!=el.end(); /*nope*/)
            if (chart->IsVirtualEntity(*i))
                el.erase(i++);
            else 
                i++;
        el.SortByPosExp();
        if (dir == MSC_DIR_LEFT)
            std::reverse(el.begin(), el.end());
        ArcBigArrow *aba = new ArcBigArrow(el, dir == MSC_DIR_BIDIR, *this, GetSignature());
        aba->ArcArrow::AddAttributeList(NULL); //skip copying line segment styles
        aba->CombineComments(this); //we pass on our notes to the block arrow
        Element *const old_target = *target;
        ret = aba->PostParseProcess(canvas, hide, left, right, number, top_level, target);
        if (old_target != *target && ret == NULL)
            *target = DELETE_NOTE;
    } else 
        ret = ArcLabelled::PostParseProcess(canvas, hide, left, right, number, top_level, target);
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
                                       left_content, right_content, number, top_level, target);
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
    return ret;
}

void ArcBox::FinalizeLabels(MscCanvas &canvas)
{
    chart->FinalizeLabelsArcList(content, canvas);
    ArcLabelled::FinalizeLabels(canvas);
}

ArcBase* ArcBoxSeries::PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right,
                                        Numbering &number, bool top_level, Element **target)
{
    if (!valid || series.size()==0) return NULL;
    //If first segment is compressed or parallel, copy that to full series
    compress = (*series.begin())->compress;
    //parallel = (*series.begin())->parallel;
    keep_with_next = (*series.rbegin())->keep_with_next;

    ArcBase *ret = this;
    EIterator src, dst;
    dst = src = chart->AllEntities.Find_by_Name(NONE_ENT_STR);
    for (auto i = series.begin(); i!=series.end(); i++) {
        if (i!=series.begin() && (*i)->parallel) {
            chart->Error.Error((*i)->file_pos.start,
                               "Attribute 'parallel' can only be specified in the first "
                               "element in a box series. Ignoring it in subsequent ones.");
        }
        if (i!=series.begin() && (*i)->draw_pass!=DRAW_DEFAULT) {
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
        //set the target to the ArcBox (if a first note comes in the content)
        *target = *i;
        //Add numbering, do content, add NULL for indicators to "content", adjust src/dst,
        //and collect left and right if needed
        ret = (*i)->PostParseProcess(canvas, hide, src, dst, number, top_level, target); //ret is an arcblockarrow if we need to collapse
		//Check if we are collapsed to a block arrow
		if ((*i)->collapsed == BOX_COLLAPSE_BLOCKARROW) {
			_ASSERT(series.size()==1);
			if (ret == NULL) *target = DELETE_NOTE;
			else if (ret->CanBeNoted()) *target = ret;
			else *target = DELETE_NOTE; //ArcBox can be noted, so if replacement cannot, we shall silently delete note
			return ret;
		}
		_ASSERT(*i==ret);
    }
    //parallel flag can be either on the series or on the first element
    parallel |= (*series.begin())->parallel;
    //Set the target to the last ArcBox (for comments coming afterwards)
    *target = *series.rbegin();
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
        return new ArcIndicator(chart, src, indicator_style, file_pos); //notes deleted
    return this;
}

void ArcBoxSeries::FinalizeLabels(MscCanvas &canvas)
{
    for (auto i=series.begin(); i!=series.end(); i++)
        (*i)->FinalizeLabels(canvas);
    ArcBase::FinalizeLabels(canvas);
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
        (*i)->ArcLabelled::Width(canvas, distances); //To process notes
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

void ArcBoxSeries::Layout(MscCanvas &canvas, AreaList &cover)
{
    height = 0;
    if (!valid) return;
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
    double comment_end=y;
    for (auto i = series.begin(); i!=series.end(); i++) {
        (*i)->yPos = y; //"y" now points to the *top* of the line of the top edge of this box
        
        //Place side comments. This will update "cover" and thus force the content
        //downward if the content also has side notes
        double l=y, r=y;
        (*i)->CommentHeightHelper(canvas, cover, l, r);
        comment_end = (*i)->comment_height;

        //Advance upper line and spacing
        y += (*i)->style.line.LineWidth() + chart->emphVGapInside;
        (*i)->y_text = y;
        (*i)->sx_text = sx + (*i)->sx_text - lw + chart->emphVGapInside;  //both sx and sx_text includes a lw
        (*i)->dx_text = dx - (*i)->dx_text + lw - chart->emphVGapInside;
        //Add text cover & draw if necessary
        (*i)->text_cover = (*i)->parsed_label.Cover((*i)->sx_text, (*i)->dx_text, (*i)->y_text);
        //Advance label height
        const double th = (*i)->parsed_label.getTextWidthHeight().y;
        //Position arrows if any under the label
        AreaList content_cover = Area((*i)->text_cover);
        if ((*i)->content.size()) {
            Area limit = (*i)->text_cover;
            if (i==series.begin() && main_style.line.corner.second != CORNER_NONE && main_style.line.radius.second>0) {
                //Funnily shaped box, prevent content from hitting it
                MscLineAttr limiter_line(main_style.line);
                limiter_line.radius.second += chart->compressGap;
                const Block b(sx-lw/2, dx+lw/2, y+lw/2, y + std::max(lw+limiter_line.radius.second*4, dx-sx)); //at midpoint of line
                limit += Contour(sx-lw/2, dx+lw/2, 0, y+lw+limiter_line.radius.second) - 
                         limiter_line.CreateRectangle_InnerEdge(b);
            }
            const double content_y = chart->PlaceListUnder(canvas, (*i)->content, y+th, 
                                                           y, limit, compress, 
                                                           &content_cover);  //no extra margin below text
            y = std::max(y+th, content_y);
        } else {
            y += th; //no content, just add textheight
        }
        if (i==--series.end() && main_style.line.corner.second != CORNER_NONE && main_style.line.radius.second>0) {
            //Funnily shaped box, prevent it content from hitting the bottom of the content
            MscLineAttr limiter_line(main_style.line);
            limiter_line.radius.second += chart->compressGap;
            const Block b(sx-lw/2, dx+lw/2, -limiter_line.radius.second*2, y);
            const Contour bottom = Contour(sx-lw/2, dx+lw/2, y-limiter_line.radius.second, y+1) -
                                   limiter_line.CreateRectangle_InnerEdge(b);
            double tp;
            double off = content_cover.OffsetBelow(bottom, tp);
            if (off>0 && compress) y-=off;
            if (off<0) y-=off;
        }
        y += chart->emphVGapInside;
        //increase the size of the box by the side notes, except for the last box
        if (i!=--series.end()) y = std::max(y, comment_end);
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
        (*i)->height = y - (*i)->yPos;  //for boxes "height is meant without the lower line

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
        (*i)->area_important = (*i)->text_cover;
        chart->NoteBlockers.Append(*i);
    }
    const double &offset = main_style.shadow.offset.second;
    if (offset)
        overall_box += overall_box.CreateShifted(XY(offset, offset));
    overall_box.mainline = Block(chart->GetDrawing().x, b.y);
    cover = GetCover4Compress(overall_box);
    height = yPos + total_height + offset + chart->emphVGapOutside;
    //We do not call CommentHeight for "this" since a box series cannot take notes, only its
    //box elements do and those were handled above
    comment_height = comment_end;
}

void ArcBox::ShiftBy(double y)
{
    if (!valid) return;
    if (y==0) return;
    y_text += y;
    text_cover.Shift(XY(0,y));
    ArcLabelled::ShiftBy(y);
    if (content.size())
        chart->ShiftByArcList(content, y);
}

void ArcBoxSeries::ShiftBy(double y)
{
    if (!valid) return;
    if (y==0) return;
    for (auto i=series.begin(); i!=series.end(); i++) 
        (*i)->ShiftBy(y);    
    ArcBase::ShiftBy(y);
}

void ArcBoxSeries::CollectPageBreak(double /*hSize*/) 
{
    if (!valid) return;
    for (auto i=series.begin(); i!=series.end(); i++) 
        chart->CollectPageBreakArcList((*i)->content);
}

double ArcBoxSeries::SplitByPageBreak(MscCanvas &canvas, double netPrevPageSize,
                                       double pageBreak, bool &addCommandNewpage, 
                                       bool addHeading, ArcList &res)
{
    if (series.size()==0) return -1; //we cannot split if no content
    if (pageBreak <= yPos || pageBreak >= yPos + GetHeight()) return -1; //pb does not cut us
    //if pageBreak goes through a label (or is above) we cannot split there
    auto kwn_from = series.end();
    for (auto i = series.begin(); i!=series.end(); i++) {
        if (pageBreak >= (*i)->GetPos()+(*i)->GetHeight()) {
            if ((*i)->IsKeepWithNext()) {
                if (kwn_from == series.end())
                    kwn_from = i;
            } else
                kwn_from = series.end();
            continue;
        }
        //We need to break in 'i'
        //if we have content and the pageBreak goes through the content
        //(top: if we have a label, use the bottom of that, if not then y_text)
        //(bottom: height contains height without the lower line)
        if ((*i)->content.size() && !(*i)->keep_together && 
            ((*i)->text_cover.IsEmpty() ? (*i)->y_text : (*i)->text_cover.GetBoundingBox().y.till) <= pageBreak &&
            ((*i)->yPos + (*i)->height) >= pageBreak) {
            //break the list
            const double ret = chart->PageBreakArcList(canvas, (*i)->content, netPrevPageSize,
                                                       pageBreak, addCommandNewpage, addHeading, false);
            //enlarge us
            (*i)->height_w_lower_line += ret;
            (*i)->height += ret;
            //shift the remaining
            while (++i!=series.end())
                (*i)->ShiftBy(ret);
            height += ret;
            total_height += ret;
            return ret;
        }
        //we cannot break inside 'i'
        //break just before 'kwn_from' or 'i'
        if (kwn_from == series.end()) kwn_from = i;
        if (kwn_from == series.begin()) return -1; //we cannot break
        //move the remainder of the series to a new object
        ArcBoxSeries *abs = new ArcBoxSeries(*kwn_from);
        series.erase(kwn_from++);
        abs->series.splice(abs->series.end(), series, kwn_from, series.end());
        //compute shift. ArcBox::yPos points to the "top" of the upper line
        //"height" is meant without the lower line
        const double shift_top = (*series.begin())->style.line.LineWidth();
        const double increase_top = shift_top - (*abs->series.begin())->style.line.LineWidth();
        const double shift_rest = shift_top + increase_top;
        //copy line style to the first one
        (*abs->series.begin())->style.line = (*series.begin())->style.line;
        (*--series.end())->height_w_lower_line += increase_top; 
        (*abs->series.begin())->height += increase_top;
        (*abs->series.begin())->height_w_lower_line += increase_top;
        (*abs->series.begin())->ShiftBy(shift_rest);
        (*abs->series.begin())->yPos -= shift_top;
        abs->yPos = (*abs->series.begin())->yPos;
        abs->AddAttributeList(NULL);
        abs->at_top_level = at_top_level;
        abs->compress = compress;
        abs->parallel = parallel;
        abs->keep_together = keep_together;
        abs->keep_with_next = keep_with_next;
        abs->left_space = left_space;
        abs->right_space = right_space;
        abs->drawing_variant = drawing_variant;

        total_height = shift_top;
        for (auto ii = series.begin(); ii!=series.end(); ii++)
            total_height += (*ii)->height;
        abs->total_height = shift_top;
        for (auto ii = abs->series.begin(); ii!=abs->series.end(); ii++) 
            abs->total_height += (*ii)->height;
        for (auto ii = ++abs->series.begin(); ii!=abs->series.end(); ii++) 
            (*ii)->ShiftBy(shift_rest);
        res.Append(abs);
        return shift_rest;
    }
    //if we got here, the pb did not go through any member
    //we must be at the very end...
    return -1;
}




void ArcBoxSeries::PlaceWithMarkers(MscCanvas &canvas, double autoMarker)
{
    for (auto i = series.begin(); i!=series.end(); i++)
        if ((*i)->valid && (*i)->content.size()) 
            chart->PlaceWithMarkersArcList(canvas, (*i)->content, autoMarker);
}


void ArcBoxSeries::PostPosProcess(MscCanvas &canvas)
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
            (*i)->ArcLabelled::PostPosProcess(canvas);
            if ((*i)->content.size()) 
                chart->PostPosProcessArcList(canvas, (*i)->content);
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
    if (pass==draw_pass) 
        canvas.Shadow(r, main_style.line, main_style.shadow);
    //Do a clip region for the overall box (for round/bevel/note corners)
    //at half a linewidth from the inner edge (use the width of a single line!)
    const Contour clip = main_style.line.CreateRectangle_ForFill(r);
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
        canvas.Clip(clip);
        //fill wider than r.x - note+triple line has wider areas to cover, clip will cut away excess
        if (pass==draw_pass) 
            canvas.Fill(Block(r.x.from, r.x.till+lw, sy, dy), (*i)->style.fill);
        //if there are contained entities, draw entity lines, strictly from inside of line
        if ((*i)->content.size()) {
            if (pass==draw_pass && (*i)->drawEntityLines &&
                (*i)->collapsed==BOX_COLLAPSE_EXPAND)
                chart->DrawEntityLines(canvas, (*i)->yPos, (*i)->height + (*i)->style.line.LineWidth(), (*i)->src, ++EIterator((*i)->dst));
            canvas.UnClip();
            chart->DrawArcList(canvas, (*i)->content, pass);
        } else
            canvas.UnClip();
    }
    if (pass!=draw_pass) return;
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
        (*i)->parsed_label.Draw(canvas, (*i)->sx_text, (*i)->dx_text, (*i)->y_text, r.x.MidPoint());
    }
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
    keep_together = false; //we can be cut in half
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
    defaultDesign.styles.GetStyle("pipe").AttributeNames(csh);
}

bool ArcPipe::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr,"color")) {
        csh.AddColorValuesToHints();
        return true;
    }
    if (defaultDesign.styles.GetStyle("pipe").AttributeValues(attr, csh)) return true;
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
                                        Numbering &number, bool top_level, Element **target)
{
    if (!valid) return NULL;

    //Add numbering, if needed 
    for (auto i = series.begin(); i!=series.end(); i++) 
        (*i)->PostParseProcess(canvas, hide, left, right, number, top_level, target);
    //Postparse the content;
    EIterator content_left, content_right;
    content_right = content_left = chart->AllEntities.Find_by_Name(NONE_ENT_STR);
    //set the first element as the note target (first in the {})
    *target = *series.begin();
    chart->PostParseProcessArcList(canvas, hide, content, false, content_left, content_right, number, top_level, target);

    //parallel flag can be either on the series or on the first element
    parallel |= (*series.begin())->parallel;
    //copy keep_with_next to Series
    for (auto i = series.begin(); i!=series.end(); i++)
        keep_with_next |= (*i)->keep_with_next;
    //set the last element as a note target (coming after us)
    *target = *series.rbegin();

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
                if (i!=series.begin() && (*i)->draw_pass!=DRAW_DEFAULT) {
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
            ArcList *al = new ArcList;
            al->swap(content);
            CommandArcList *p = new CommandArcList(chart, al); //this will do a "delete al;"
            p->AddAttributeList(NULL);
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
    else if (canvas.AvoidTransparency())
        for (auto i = series.begin(); i!=series.end(); i++)
            //disallow transparency if too low power
            if ((*i)->style.solid.second!=255) 
                (*i)->style.solid.second = 0;

    return this;
}

void ArcPipeSeries::FinalizeLabels(MscCanvas &canvas)
{
    for (auto i=series.begin(); i!=series.end(); i++)
        (*i)->FinalizeLabels(canvas);
    chart->FinalizeLabelsArcList(content, canvas);
    ArcBase::FinalizeLabels(canvas);
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
        (*i)->ArcLabelled::Width(canvas, distances); //To process notes
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

//Takes the pipe_body member of each pipe and calculates their contours used for drawing.
//returns the total body cover if not NULL
void ArcPipeSeries::CalculateContours(Area *pipe_body_cover)
{
    //A few shortcuts. "side" and "radius" must be the same in any pipe element, so we take the first
    const MscSideType side = (*series.begin())->style.side.second;
    const double radius = (*series.begin())->style.line.radius.second;
    //the largest of the shadow offsets
    double max_offset = 0;
    for (auto i = series.begin(); i!=series.end(); i++) {
        //No need to clean up. If any of the pipe_* or area, area_draw, area_important
        //has values here, they will get simply overwritten
        const double ilw = (*i)->style.line.LineWidth();

        XY cs((*i)->pipe_block.x.from, (*i)->pipe_block.y.MidPoint());
        XY cd((*i)->pipe_block.x.till, (*i)->pipe_block.y.MidPoint());
        const XY rad(radius, (*i)->pipe_block.y.Spans()/2); //we use the first pipe's line.radius
        if (side == SIDE_LEFT) std::swap(cs, cd); //use the first pipe's fromright, not (*i)->fromright
        //now cd is the one with the hole
        const Contour back_end(cs, rad.x, rad.y);
        const Contour forw_end(cd, rad.x, rad.y);
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
                    (*i)->area_draw *= Contour(side == SIDE_RIGHT ? chart->GetDrawing().x.from : chart->GetDrawing().x.till, cd.x,
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
                (*i)->pipe_body_fill -= forw_end.CreateExpand(-(*i)->style.line.width.second/2);
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
        if (pipe_body_cover) {
            //now determine the cover to be used for placement
            const double offset = (*i)->style.shadow.offset.second;
            if (offset)
                *pipe_body_cover += (*i)->pipe_shadow + (*i)->pipe_shadow.CreateShifted(XY(offset, offset));
            else
                *pipe_body_cover += (*i)->pipe_shadow;
        }
        //merge shadows of connected previous segment to ours
        if ((*i)->pipe_connect_back) {
            auto i_neigh = i; i_neigh--;
            (*i_neigh)->pipe_shadow = ((*i)->pipe_shadow + (*i_neigh)->pipe_shadow);
            (*i)->pipe_shadow.clear();
        }
        (*i)->area_important = (*i)->text_cover;
        (*i)->area_important += forw_end;
        (*i)->area_important += back_end;
    }
    for (auto i = series.begin(); i!=series.end(); i++)
        (*i)->pipe_shadow = (*i)->pipe_shadow.CreateExpand(-(*i)->style.line.width.second/2);
}

void ArcPipeSeries::Layout(MscCanvas &canvas, AreaList &cover)
{
    height = 0;
    if (!valid) return;
    //Collect cover information from labels and linewidth, so compression of content arrows can be done
    //Determine thickest line for precise pipe alignment
    double max_lineWidth = 0;
    for (auto i = series.begin(); i!=series.end(); i++)
        max_lineWidth = std::max(max_lineWidth, (*i)->style.line.LineWidth());
    double lowest_line_bottom =  max_lineWidth + chart->emphVGapInside;
    //Determine highest label and collect all text covers
    //Also calcualte all x positioning
    double lowest_label_on_transculent_bottom = lowest_line_bottom;
    double lowest_label_on_opaque_segments_bottom = lowest_line_bottom;
    Area label_covers(this);
    //A few shortcuts. "side" and "radius" must be the same in any pipe element, so we take the first
    const MscSideType side = (*series.begin())->style.side.second;
    const double radius = (*series.begin())->style.line.radius.second;
    double note_l=0, note_r=0;
    for (auto i = series.begin(); i!=series.end(); i++) {
        //Variables already set (all of them rounded):
        //pipe_connect true if a segment connects to us directly
        //left_space, right_space contains how much our content expands beyond the entity line,
        (*i)->yPos = 0;
        (*i)->area.clear();
        (*i)->area_draw.clear();
        (*i)->draw_is_different = false;

        //Place side comments. This will update "cover" and thus force the content
        //downward if the content also has side notes
        (*i)->CommentHeightHelper(canvas, cover, note_l, note_r);

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
            y += (*i)->style.text.getCharHeight(canvas);
        if ((*i)->style.solid.second < 255) {
            label_covers += (*i)->text_cover;
            lowest_label_on_transculent_bottom = std::max(lowest_label_on_transculent_bottom, y);
        } else {
            //collect the highest label of opaque segments for later use
            lowest_label_on_opaque_segments_bottom = std::max(lowest_label_on_opaque_segments_bottom, y);
        }
    }
    double y = lowest_label_on_transculent_bottom;
    //Calculate the Height of the content
    AreaList content_cover;
    if (content.size())
        y = ceil(chart->PlaceListUnder(canvas, content, ceil(y), lowest_line_bottom, 
                                       label_covers, false, &content_cover));
    //now y contains the bottom of the content arrows (if any),
    //adjust if an opaque pipe's label was not yet considered in y
    y = std::max(y, lowest_label_on_opaque_segments_bottom);
    y += chart->emphVGapInside + max_lineWidth;
    //now y contains the bottommost pixel of the pipe itself
    total_height = y = ceil(y);
    //Now set the y coordinate in all segments
    double max_shadow_offset = 0;
    for (auto i = series.begin(); i!=series.end(); i++) {
        //fill in pipe_block.y (both are integer)
        (*i)->pipe_block.y.from = chart->emphVGapOutside;
        (*i)->pipe_block.y.till = y;
        chart->NoteBlockers.Append(*i);
        max_shadow_offset = std::max(max_shadow_offset, (*i)->style.shadow.offset.second);
    }
    //Calculate contours in all pipes from pipe_block
    Area pipe_body_cover(this);
    CalculateContours(&pipe_body_cover);
    //Add content to cover (may "come out" from pipe)
    cover += content_cover;
    //If we have no valid content, set mainline to that of pipe, else the content's mainline will be used
    if (content_cover.mainline.IsEmpty()) 
        pipe_body_cover.mainline = Block(chart->GetDrawing().x.from, chart->GetDrawing().x.till, chart->emphVGapOutside, total_height);  //totalheight includes the top emphvgapoutside 
    //Expand cover, but not content (that is already expanded)
    cover += GetCover4Compress(pipe_body_cover);
    height = yPos + total_height + max_shadow_offset + chart->emphVGapOutside;
    //We do not call NoteHeight here as a PipeSeries will not have notes, only its elements
    comment_height = std::max(note_l, note_r);
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
        chart->ShiftByArcList(content, y);
    ArcBase::ShiftBy(y);
}

void ArcPipeSeries::CollectPageBreak(double /*hSize*/) 
{
    chart->CollectPageBreakArcList(content);
}

double ArcPipeSeries::SplitByPageBreak(MscCanvas &canvas, double netPrevPageSize,
                                       double pageBreak, bool &addCommandNewpage, 
                                       bool addHeading, ArcList &/*res*/)
{
    if (content.size()==0 || keep_together) return -1; //we cannot split if no content
    //if pageBreak goes through a label (or is above) we cannot split there
    for (auto i = series.begin(); i!=series.end(); i++)
        if ((*i)->text_cover.GetBoundingBox().y.till > pageBreak || (*i)->keep_together)
            return -1;
    const double ret = chart->PageBreakArcList(canvas, content, netPrevPageSize,
                                               pageBreak, addCommandNewpage, addHeading, false);
    height += ret;
    total_height += ret;
    for (auto i=series.begin(); i!=series.end(); i++)
        (*i)->pipe_block.y.till += ret;
    CalculateContours();
    return ret;
}


void ArcPipeSeries::PlaceWithMarkers(MscCanvas &canvas, double autoMarker)
{
    if (content.size())
        chart->PlaceWithMarkersArcList(canvas, content, autoMarker);
}

void ArcPipeSeries::PostPosProcess(MscCanvas &canvas)
{
    if (!valid) return;
    //For pipes we first add those covers to chart->AllCovers that are not fully opaque,
    //then the content (only in the first segment)
    //then those segments, which are fully opaque
    //(this is because search is backwards and this arrangement fits the visual best
    for (auto i = series.begin(); i!=series.end(); i++)
        if ((*i)->valid && (*i)->style.solid.second < 255)
            (*i)->ArcLabelled::PostPosProcess(canvas);
    if (content.size())
        chart->PostPosProcessArcList(canvas, content);
    for (auto i = series.begin(); i!=series.end(); i++)
        if ((*i)->valid && (*i)->style.solid.second == 255)
            (*i)->ArcLabelled::PostPosProcess(canvas);
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
        Contour clip(x, style.side.second == SIDE_LEFT ? chart->GetDrawing().x.till : chart->GetDrawing().x.from,
                     chart->GetDrawing().y.from, chart->GetDrawing().y.till);
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
        parsed_label.Draw(canvas, sx_text, dx_text, y_text);
}

void ArcPipeSeries::Draw(MscCanvas &canvas, DrawPassType pass)
{
    if (!valid) return;
    //First shadows
    if (pass==draw_pass) {
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
    }
    if (content.size()) {
        if (pass==DRAW_AFTER_ENTITY_LINES)
            for (auto i = series.begin(); i!=series.end(); i++) 
                if ((*i)->drawEntityLines)
                    chart->DrawEntityLines(canvas, yPos, total_height, (*i)->src, ++EIterator((*i)->dst));
        chart->DrawArcList(canvas, content, pass);
    }
    if (pass==draw_pass) 
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
    ArcLabelled(t, msc, msc->Contexts.back().styles[MyStyleName(t)]),
    nudge(t==MSC_COMMAND_NUDGE),
    title(t==MSC_COMMAND_TITLE || t==MSC_COMMAND_SUBTITLE),
    wide(false),
    extra_space(t==MSC_ARC_DISCO ? msc->discoVgap :
                t==MSC_COMMAND_TITLE ? msc->titleVgap :
                t==MSC_COMMAND_SUBTITLE ? msc->subtitleVgap :
                0)
{
}

const char *ArcDivider::MyStyleName(MscArcType t)
{
    switch(t) {
    default:
        _ASSERT(0);
        //these styles will be differentiated by refinement styles
        //in ArcLabelled::GetRefinementStyle
    case MSC_ARC_DISCO:
    case MSC_ARC_DIVIDER:
    case MSC_ARC_VSPACE:
    case MSC_COMMAND_NUDGE:
        return "divider";
    case MSC_COMMAND_TITLE:
        return "title";
    case MSC_COMMAND_SUBTITLE:
        return "subtitle";
    }
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

void ArcDivider::AttributeNames(Csh &csh, bool nudge, bool title)
{
    if (nudge) return;
    ArcLabelled::AttributeNames(csh);
    defaultDesign.styles.GetStyle(title ? "title" : "divider").AttributeNames(csh);
}

bool ArcDivider::AttributeValues(const std::string attr, Csh &csh, bool nudge, bool title)
{
    if (nudge) return false;
    if (defaultDesign.styles.GetStyle(title ? "title" : "divider").AttributeValues(attr, csh)) return true;
    if (ArcLabelled::AttributeValues(attr, csh)) return true;
    return false;
}

ArcBase* ArcDivider::PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right, 
                                      Numbering &number, bool top_level, Element **target)
{
    if (!valid) return NULL;
    string ss;
    switch (type) {
    case MSC_ARC_VSPACE:       break;
    case MSC_ARC_DISCO:        ss = "'...'"; break;
    case MSC_ARC_DIVIDER:      ss = "'---'"; break;
    case MSC_COMMAND_TITLE:    ss = "Titles"; break;
    case MSC_COMMAND_SUBTITLE: ss = "Subtitles"; break;
    case MSC_COMMAND_NUDGE:    ss = "Nudges"; break;
    default: _ASSERT(0); break;
    }

    if (title && label.length()==0) {
        chart->Error.Error(file_pos.start, ss + " must have a label. Ignoring this command.");
        return NULL;
    }

    //Add numbering, if needed
    ArcLabelled::PostParseProcess(canvas, hide, left, right, number, top_level, target);

    if (!top_level && (type==MSC_ARC_DISCO || type==MSC_ARC_DIVIDER || 
                       type==MSC_COMMAND_TITLE || type==MSC_COMMAND_SUBTITLE)) {
        chart->Error.Warning(file_pos.start, ss + " is specified inside a parallel block.",
            "May display incorrectly.");
    }
    if (hide) return NULL;
    return this;
}

void ArcDivider::Width(MscCanvas &, EntityDistanceMap &distances)
{
    if (!valid) return;
    if (nudge || !valid || parsed_label.getTextWidthHeight().y==0)
        return;
    //Get marging from lside and rside
    text_margin = wide ? 0 : chart->XCoord((chart->GetHScale()>=0 ? MARGIN : MARGIN_HSCALE_AUTO)/2);
    if (title) 
        text_margin += style.line.LineWidth();
    //calculate space requirement between lside and rside
    const double width = 2*text_margin + parsed_label.getTextWidthHeight().x;
    if (width>0)
        distances.Insert(chart->LSide->index, chart->RSide->index, width);
}

void ArcDivider::Layout(MscCanvas &canvas, AreaList &cover)
{
    height = 0;
    if (!valid) return;
    yPos = 0;
    if (nudge) {
        Block b(chart->GetDrawing().x.from, chart->GetDrawing().x.till, 0, chart->nudgeSize);
        area.mainline = area = b;
        cover = GetCover4Compress(area);
        height = chart->nudgeSize;
        CommentHeight(canvas, cover);
        return;
    }
    double y = wide ? 0 : chart->arcVGapAbove;
    y += extra_space;
    if (title)
        y += style.line.LineWidth();
    const double charheight = style.text.getCharHeight(canvas);
    XY wh = parsed_label.getTextWidthHeight();
    if (!wh.y) wh.y = charheight;
    centerline = y+wh.y/2;
    line_margin = chart->XCoord(MARGIN);
    text_cover = parsed_label.Cover(chart->GetDrawing().x.from + text_margin, chart->GetDrawing().x.till-text_margin, y);
    area = text_cover;
    area_important = area;
    const double lw = style.line.LineWidth();
    //Add a cover block for the line, if one exists
    if (!title && style.line.type.second != LINE_NONE && style.line.color.second.valid && style.line.color.second.a>0)
        area += Block(chart->GetDrawing().x.from + line_margin, chart->GetDrawing().x.till - line_margin,
                      centerline - style.line.LineWidth()*2, centerline + style.line.LineWidth()*2);
    else if (title && (style.line.type.second != LINE_NONE || style.fill.color.second.valid))
        area += Block(chart->GetDrawing().x.from + text_margin-lw, chart->GetDrawing().x.till - text_margin+lw,
                      y-lw, y+wh.y+lw);
    else if (area.IsEmpty())
        area = Block(chart->GetDrawing().x.from, chart->GetDrawing().x.till, 0, chart->nudgeSize);
    area.arc = this;

    if (!wide)
        wh.y += chart->arcVGapBelow;
    height = wh.y + 2*extra_space;
    if (title)
        height += 2*style.line.LineWidth() + style.shadow.offset.second;
    //Discontinuity lines cannot be compressed much
    if (type==MSC_ARC_DISCO || title)
        area.mainline = Block(chart->GetDrawing().x.from, chart->GetDrawing().x.till, 
                              wide ? 0 : chart->arcVGapAbove, height- (wide ? 0 :chart->arcVGapBelow));
    else
        area.mainline = Block(chart->GetDrawing().x.from, chart->GetDrawing().x.till, 
                              centerline-charheight/2, centerline+charheight/2);
    chart->NoteBlockers.Append(this);
    cover = GetCover4Compress(area);
    CommentHeight(canvas, cover);
}

void ArcDivider::ShiftBy(double y)
{
    if (!valid) return;
    if (!nudge)
        text_cover.Shift(XY(0,y));
    ArcLabelled::ShiftBy(y);
}

void ArcDivider::PostPosProcess(MscCanvas &canvas)
{
    if (!valid) return;
    if (!nudge)
        chart->HideEntityLines(text_cover);
    ArcLabelled::PostPosProcess(canvas);
}

void ArcDivider::Draw(MscCanvas &canvas, DrawPassType pass)
{
    if (!valid) return;
    if (pass!=draw_pass) return;
    if (nudge) return;
    if (title) {
        const Block outer(chart->GetDrawing().x.from + text_margin, chart->GetDrawing().x.till - text_margin,
                            yPos+extra_space, yPos+height-style.shadow.offset.second-extra_space);
        canvas.Shadow(outer, style.line, style.shadow);
        canvas.Fill(outer.CreateExpand(-style.line.LineWidth()/2-style.line.Spacing()), style.line, style.fill);
        canvas.Line(outer.CreateExpand(-style.line.LineWidth()/2), style.line);
    }
    parsed_label.Draw(canvas, chart->GetDrawing().x.from + text_margin, chart->GetDrawing().x.till - text_margin, 
                      yPos + (wide ? 0 : chart->arcVGapAbove+extra_space));
    if (title) return;
    //determine widest extent for coverage at the centerline+-style.line.LineWidth()/2;
    const double lw2 = ceil(style.line.LineWidth()/2.);
    Block b(chart->GetDrawing().x.from+line_margin, chart->GetDrawing().x.till-line_margin, 
            yPos + centerline - lw2, yPos + centerline + lw2);
    Range r = (text_cover * b).GetBoundingBox().x;
    if (r.IsInvalid())
        canvas.Line(XY(chart->GetDrawing().x.from+line_margin, yPos + centerline), XY(chart->GetDrawing().x.till-line_margin, yPos + centerline), style.line);
    else {
        canvas.Line(XY(chart->GetDrawing().x.from+line_margin, yPos + centerline), XY(r.from-chart->emphVGapInside, yPos + centerline), style.line);
        canvas.Line(XY(r.till+chart->emphVGapInside, yPos + centerline), XY(chart->GetDrawing().x.till-line_margin, yPos + centerline), style.line);
    }
}

//////////////////////////////////////////////////////////////////////////////////////

MscDirType ArcParallel::GetToucedEntities(class EntityList &el) const
{
    MscDirType dir = MSC_DIR_INDETERMINATE;
    for (auto i = blocks.begin(); i!=blocks.end(); i++)
        dir = chart->GetTouchedEntitiesArcList(*i, el, dir);
    return dir;
}

string ArcParallel::Print(int ident) const
{
    string ss;
    ss << string(ident*2, ' ');
    ss << PrintType() << "\n";
    for (auto i = blocks.begin(); i!=blocks.end(); i++) {
        if (i!=blocks.begin())
            ss << string(ident*2+2, ' ') << "---\n";
        ss << i->Print(ident+2);
        if (i!=blocks.end())
            ss << "\n";
    }
    return ss;
};

ArcBase* ArcParallel::PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right, 
                      Numbering &number, bool top_level, Element **target)
{
    if (!valid) return NULL;
    at_top_level = top_level;
    for (auto i=blocks.begin(); i != blocks.end(); i++)
        chart->PostParseProcessArcList(canvas, hide, *i, false, left, right, number, false, target);
    if (hide) return NULL;
    return this;
}

void ArcParallel::FinalizeLabels(MscCanvas &canvas)
{
    for (auto i=blocks.begin(); i != blocks.end(); i++)
        chart->FinalizeLabelsArcList(*i, canvas);
    ArcBase::FinalizeLabels(canvas);
}

void ArcParallel::Width(MscCanvas &canvas, EntityDistanceMap &distances)
{
    if (!valid) return;
    EntityDistanceMap d;
    for (auto i=blocks.begin(); i != blocks.end(); i++)
        chart->WidthArcList(canvas, *i, d);
    d.CombineLeftRightToPair_Sum(chart->hscaleAutoXGap);
    d.CombineLeftRightToPair_Single(chart->hscaleAutoXGap);
    d.CopyBoxSideToPair(chart->hscaleAutoXGap);
    distances += d;
}

void ArcParallel::Layout(MscCanvas &canvas, AreaList &cover)
{
    height = 0;
    if (!valid) return;
    if (chart->simple_arc_parallel_layout) {
        for (auto i=blocks.begin(); i != blocks.end(); i++) {
            AreaList cover_block;
            //Each parallel block is compressed without regard to the others
            double h = chart->LayoutArcList(canvas, *i, cover_block);
            height = std::max(height, h);
            cover += cover_block;
        }
    } else {
        std::vector<double> heights = chart->LayoutArcLists(canvas, blocks, cover);
        for (unsigned u = 0; u<blocks.size(); u++)
            height = std::max(height, heights[u]);
    }
    //Do not expand cover, it has already been expanded
    CommentHeight(canvas, cover);
}

void ArcParallel::ShiftBy(double y)
{
    if (!valid) return;
    for (auto i=blocks.begin(); i!=blocks.end(); i++)
        chart->ShiftByArcList(*i, y);
    ArcBase::ShiftBy(y);
}

void ArcParallel::CollectPageBreak(double /*hSize*/)
{
    if (!valid) return;
    for (auto i=blocks.begin(); i!=blocks.end(); i++)
        chart->CollectPageBreakArcList(*i);
}


//step with 'e' as long as we hit an element of nonzero height
//if w hit the end of the list, we splice the whole list to
//result
void FindFirstNonZero(ArcList::iterator &e, ArcList *list, ArcList &result) 
{
    while (e != list->end() && (*e)->GetHeight() == 0)
        e++;
    if (e == list->end()) 
        result.Append(list);
}


double ArcParallel::SplitByPageBreak(MscCanvas &canvas, double netPrevPageSize,
                                    double pageBreak, bool &addCommandNewpage, 
                                    bool addHeading, ArcList &/*res*/)
{
    if (keep_together) return -1;
    //First merge our content to a single list
    if (blocks.size()>1) {
        //We cannot use simple merge here, because in case of compress
        //a zero-height element between two compressed non-zero
        //may start later (at the touchpoint) than the subsequent element.
        //The rule here is to move zero-height elements along with the
        //subsequent non-zero element after them. If they are at the end
        //of a block, then move them with the non-zero element before them.
        for (auto b=++blocks.begin(); b!=blocks.end(); b++) {
            ArcList result;
            ArcList *list[2] = {&blocks[0], &*b};
            ArcList::iterator e[2] = {list[0]->begin(), list[1]->begin()};
            FindFirstNonZero(e[0], list[0], result);
            FindFirstNonZero(e[1], list[1], result);
            while (list[0]->size() >0 && list[0]->size() > 0) {
                unsigned u = (*e[0])->GetPos() > (*e[1])->GetPos();
                result.splice(result.end(), *list[u], list[u]->begin(), ++e[u]);
                FindFirstNonZero(e[u], list[u], result);
            }
            if (list[0]->size()) result.splice(result.end(), *list[0]);
            if (list[1]->size()) result.splice(result.end(), *list[1]);
            blocks[0].swap(result); //move the result to "block[0]"
        }
        blocks.resize(1);
    }
    return chart->PageBreakArcList(canvas, blocks[0], netPrevPageSize, 
                                   pageBreak, addCommandNewpage, addHeading, false);
}

void ArcParallel::PlaceWithMarkers(MscCanvas &canvas, double autoMarker)
{
    if (!valid) return;
    int n=0;
    //For automarker, give the bottom of the largest blocks
    for (auto i=blocks.begin(); i!=blocks.end(); i++, n++)
        chart->PlaceWithMarkersArcList(canvas, *i,
            n>0 ? yPos + height : autoMarker);
}

void ArcParallel::PostPosProcess(MscCanvas &canvas)
{
    if (!valid) return;
    ArcBase::PostPosProcess(canvas);
    for (auto i=blocks.begin(); i!=blocks.end(); i++)
        chart->PostPosProcessArcList(canvas, *i);
}

void ArcParallel::Draw(MscCanvas &canvas, DrawPassType pass)
{
    if (!valid) return;
    for (auto i=blocks.begin(); i != blocks.end(); i++)
        chart->DrawArcList(canvas, *i, pass);
}

