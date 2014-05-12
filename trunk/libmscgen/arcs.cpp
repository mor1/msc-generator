/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011,2012,2013,2014 Zoltan Turanyi
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

  # Example

  \msc
  a->*b;
  \endmsc
  
  # Terminology

  - An *entity* is something the messages go in between. They have three classes
    in libmscgen: one Entity per entity of the chart, one EntityApp for every
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
    option, such as 'compress', 'vspacing', 'numbering', 'indicator', 'slant_angle' and 'hscale'.
    They also include the current comment line and fill style, the default font
    and current numbering style.read(). Contexts are captured by class Context.
  - A *scope* in the chart file is an area enclosed between '{' and '} symbols.
    Any change you make to the context is valid only inside the scope.
  - A *style* is a set of attributes (line, vline, fill, vfill, shadow, 
    arrow, text, note, solid, side, numbering, compress, vspacing, indicator and
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
       d) For arrows that are specified as lost, we determine where the loss is happening.
       e) For boxes we check if the box is collapsed. If so, we replace content to ArcIndicator. 
          Here we have to ensure that automatic numbering of arrows do not change as for step 4c above.
          We also have to ensure that for auto-sizing entities (e.g., " .. : label { <content> }") we
          keep the size as would be the case for a non-collapsed box. Also, content that does not show, but
          influence appearance (e.g., entity commands, page breaks, background changes, etc.) are 
          still kept as content and steps below (\#6-9) shall be performed on them. See \#h below.
       f) Combine CommandEntities and ArcIndicators one after the other
       g) Set up some extra variables
       h) Print error messages. 
       i) Decide on replacement. PostParseProcess have a "hide" parameter telling if this arc will be hidden 
          due to a collapsed box (and no other reason). In this case we return NULL, if the arc does not 
          want to receive #6-9 below. Else we return "this".
          An arc can also become hidden due to collapsed entities - this was determined in #4c above. If 
          the arc becomes hidden, it can get replaced to an ArcIndicator if the entity in question has its
          "indicator" in "running_style" set. Else we just retuen NULL.
       j) If the node is kept, we move its floating notes to "Msc::FloatingNotes" via "MoveNotesToChart"
          called from "Msc::PostParseProcessArcList".
       k) For notes and comments, we decide who is the real target and attach the note/command here.
       l) For labels, we replace the remaining escapes to actual values, except for "\r()"
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

       Note that if `chart->prepare_for_tracking` is not set, we do not calculate `area` and `area_draw`.
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
       y coordinate into Msc::pageBreakData. This function can be called multiple times during
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
       Note that if `chart->prepare_for_tracking` is not set, we do not expand these.
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
   - `style` collects attribues to a common Style class and add Context and Design.
   - `mscdrawer` contains Canvas, a helper class to encapsulate a cairo
     surface and context. We add logic for pages and fallback operations here
     (e.g., for output types (like WMF) that do not support certain operations, 
     like dashed lines.)
   - `element`, `entity`, `arcs` and `commands` define the classes that represent 
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

ArcBase::ArcBase(EArcType t, MscProgress::ECategory c, Msc *msc) :
    Element(msc), had_add_attr_list(false), valid(true), 
    vspacing(0), parallel(false), overlap(false),
    keep_together(true), keep_with_next(false), height(0),
    type(t), myProgressCategory(c)
{
    vspacing = chart->Contexts.back().vspacing.second;
    chart->Progress.RegisterArc(myProgressCategory);
}

ArcBase::~ArcBase()
{
    _ASSERT(chart);
    if (chart)
        chart->Progress.UnRegisterArc(myProgressCategory);
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
void ArcBase::AddAttributeList(AttributeList *l)
{
    had_add_attr_list = true;
    if (l==NULL || !valid) return;
    for (auto pAttr : *l)
        AddAttribute(*pAttr);
    delete l;
}

bool ArcBase::AddAttribute(const Attribute &a)
{
    //In case of ArcLabelled this will not be called, for 
    //"compress" and "vspacing" attributes.
    //There the style.read().AddAtribute will process any compress attribute.
    //Then in ArcLabelled::PostParseProcess
    //we copy style.read().compress.second to the ArcBase::compress.
    if (a.Is("compress")) {
        if (!a.EnsureNotClear(chart->Error, STYLE_ARC)) return true;
        if (!a.CheckType(MSC_ATTR_BOOL, chart->Error)) return true;
        vspacing = a.yes ? DBL_MIN : 0;
        return true;
    }
    if (a.Is("vspacing")) {
        if (!a.EnsureNotClear(chart->Error, STYLE_ARC)) return true;
        if (a.type == MSC_ATTR_STRING && a.value == "compress") {
            vspacing = DBL_MIN;
            return true;
        }
        if (!a.CheckType(MSC_ATTR_NUMBER, chart->Error)) return true;
        vspacing = a.number;
        return true;
    }
    if (a.Is("parallel")) {
        if (!a.EnsureNotClear(chart->Error, STYLE_ARC)) return true;
        if (!a.CheckType(MSC_ATTR_BOOL, chart->Error)) return true;
        parallel = a.yes;
        return true;
    }
    if (a.Is("overlap")) {
        if (!a.EnsureNotClear(chart->Error, STYLE_ARC)) return true;
        if (!a.CheckType(MSC_ATTR_BOOL, chart->Error)) return true;
        overlap = a.yes;
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
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "vspacing", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "parallel", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "overlay", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "refname", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "keep_together", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "keep_with_next", HINT_ATTR_NAME));
    Element::AttributeNames(csh);
}

bool ArcBase::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr,"compress")||
        CaseInsensitiveEqual(attr,"parallel") ||
        CaseInsensitiveEqual(attr,"overlay") ||
        CaseInsensitiveEqual(attr,"keep_together") ||
        CaseInsensitiveEqual(attr,"keep_with_next")) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+"yes", HINT_ATTR_VALUE, true, CshHintGraphicCallbackForYesNo, CshHintGraphicParam(1)));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+"no", HINT_ATTR_VALUE, true, CshHintGraphicCallbackForYesNo, CshHintGraphicParam(0)));
        return true;
    }
    if (CaseInsensitiveEqual(attr, "vspacing")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", HINT_ATTR_VALUE, false));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+"compress", HINT_ATTR_VALUE, true));
        return true;
    }
    if (CaseInsensitiveEqual(attr, "refname"))
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


void ArcBase::AddEntityLineWidths(DistanceMapVertical &vdist)
{
    for (const auto &e : chart->ActiveEntities)
        if (e->running_shown.IsActive()) {
            vdist.Insert(e->index, DISTANCE_LEFT, chart->activeEntitySize);
            vdist.Insert(e->index, DISTANCE_RIGHT, chart->activeEntitySize);
        }
}

ArcBase* ArcBase::PostParseProcess(Canvas &/*canvas*/, bool /*hide*/,
                                   EIterator &/*left*/, EIterator &/*right*/,
                                   Numbering &/*number*/, Element **target,
                                   ArcBase * /*vertical_target*/)
{
    if (CanBeNoted()) *target = this;
    return this;
}

void ArcBase::FinalizeLabels(Canvas &)
{
    if (refname.length())
        chart->ReferenceNames[refname].arc = this;
}

void ArcBase::Layout(Canvas &canvas, AreaList *cover)
{
    height = 0;
    LayoutComments(canvas, cover);
}

void ArcBase::PostPosProcess(Canvas &canvas)
{
    _ASSERT(had_add_attr_list);
    if (valid) 
        Element::PostPosProcess(canvas); //also adds "this" to chart->AllArcs
    else if (!file_pos.IsInvalid())
        chart->AllArcs[file_pos] = this; //Do this even if we are invalid
}

ArcIndicator::ArcIndicator(Msc *chart, const StyleCoW &st, const FileLineColRange &l) : 
    ArcBase(MSC_ARC_INDICATOR, MscProgress::INDICATOR, chart), style(st)
{
    dst = src = chart->AllEntities.Find_by_Name(NONE_ENT_STR);
    AddAttributeList(NULL);
    SetLineEnd(l);
}

ArcIndicator::ArcIndicator(Msc *chart, EIterator s, const StyleCoW &st, const FileLineColRange &l) : 
    ArcBase(MSC_ARC_INDICATOR, MscProgress::INDICATOR, chart), style(st), src(s), dst(s)
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

EDirType ArcIndicator::GetToucedEntities(class EntityList &el) const
{
    if (!IsComplete()) return MSC_DIR_INDETERMINATE;
    el.push_back(*src);
    if (*src!=*dst)
        el.push_back(*dst);
    return MSC_DIR_INDETERMINATE;
}

void ArcIndicator::Width(Canvas &, EntityDistanceMap &distances, DistanceMapVertical &vdist)
{
    //Add a new element to vdist
    vdist.InsertElementTop(this);
    //Add activation status right away
    AddEntityLineWidths(vdist);
    vdist.InsertEntity(src);
    vdist.InsertEntity(dst);
    //Add a new element to vdist
    vdist.InsertElementBottom(this);
    //Add activation status right away
    AddEntityLineWidths(vdist);
    if (*src != *dst) return; //no width requirements for Indicators not exactly on an entity
    //If we are exactly on an entity line add left and right req for boxes potentially around us.
    const double width = indicator_style.read().line.LineWidth() + indicator_size.x/2;
    distances.InsertBoxSide((*src)->index-1, 0, width);
    distances.InsertBoxSide((*src)->index,   width, 0);
}

void ArcIndicator::Layout(Canvas &canvas, AreaList *cover)
{
    yPos = chart->boxVGapOutside;
    const double x = (chart->XCoord((*src)->pos) + chart->XCoord((*dst)->pos))/2;
    const Block b = GetIndicatorCover(XY(x, chart->boxVGapOutside));
    if (chart->prepare_for_tracking || cover) {
        area = b;
        area.mainline = Block(chart->GetDrawing().x, b.y);
    }
    area_important = b;
    chart->NoteBlockers.Append(this);
    height = b.y.till + chart->boxVGapOutside;
    //TODO add shadow to cover
    if (cover)
        *cover = GetCover4Compress(area);
    LayoutComments(canvas, cover);
}

void ArcIndicator::Draw(Canvas &canvas, EDrawPassType pass) 
{
    if (pass!=draw_pass) return;
    const double x = (chart->XCoord((*src)->pos) + chart->XCoord((*dst)->pos))/2;
    DrawIndicator(XY(x, yPos), &canvas);
}

//////////////////////////////////////////////////////////////////////////////////////

/* The regular constructor.
 * Takes numbering style from the current context. 
 * Merges the default text formatting style with `s`.*/
ArcLabelled::ArcLabelled(EArcType t, MscProgress::ECategory c, Msc *msc, const StyleCoW &s) :
    ArcBase(t, c, msc), concrete_number(-1), style(s),
    numberingStyle(msc->Contexts.back().numberingStyle)
{
    SetStyleWithText();  //keep existing style, just combine with default text style, numbering and refinement
    entityLineRange.MakeInvalid();
}

/** Custom constructor to convert arcs.
 * This is only used to convert an ArcBox to an ArcBigArrow in
 * ArcBoxSeries::PostParseProcess (when the box is collapsed to a block arrow).
 * So we assume PostParseProcess() was not called for `al`, but will be called for `this`.*/
ArcLabelled::ArcLabelled(EArcType t, MscProgress::ECategory c, const ArcLabelled &al)
    : ArcBase(t, c, al.chart), label(al.label), parsed_label(al.parsed_label),
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
    vspacing = al.vspacing;
    parallel = al.parallel;
}

//set style to this name, but combine it with default text style
void ArcLabelled::SetStyleWithText(const char *style_name)
{
    const StyleCoW *s = NULL;
    if (style_name)
        s = &chart->Contexts.back().styles[style_name];
    SetStyleWithText(s);
}

//set style to this name, but combine it with default text style
void ArcLabelled::SetStyleWithText(const StyleCoW *style_to_use)
{
    if (style_to_use)
        style = *style_to_use;
    //Make style.read().text complete using the default text formatting in the context as default
    StringFormat to_use(chart->Contexts.back().text);
    to_use += style.read().text;
    style.write().text = to_use;
    style.write().type = STYLE_ARC;
    //If style does not contain a numbering setting, apply the value of the
    //current chart option.
    if (!style.read().numbering.first) {
        style.write().numbering.first = true;
        style.write().numbering.second = chart->Contexts.back().numbering.second;
    }
    //Add refinement style (e.g., -> or ... or vertical++)
    const StyleCoW *refinement = GetRefinementStyle(type); //virtual function for the arc
    if (refinement) style += *refinement;
}

void ArcLabelled::OverflowWarning(double overflow, const string &msg, EIterator e1,  EIterator e2)
{
    if (overflow < 1) return;
    string bigger_hscale;
    bigger_hscale << ceil(chart->GetHScale()+0.5);
    chart->Error.Warning(file_pos.start, "Too little space for label - may look bad.",
        msg.length() ? msg :
        (chart->GetHScale()<=0 ? 
            "Don't use 'hscale=auto'" : 
            "Try increasing 'hscale', e.g., 'hscale="+bigger_hscale+"'") +
        (chart->IsVirtualEntity(*e1) && chart->IsVirtualEntity(*e2) ? "." :
             " or add space using 'hspace " + 
             (chart->IsVirtualEntity(*e1) ? "" : (*e1)->name) +
             "-" +
             (chart->IsVirtualEntity(*e2) ? "" : (*e2)->name) +
             "'."));
};

void ArcLabelled::CountOverflow(double space)
{
    _ASSERT(!parsed_label.IsWordWrap());
    chart->CountLabel(parsed_label.getTextWidthHeight().x > space + 1);
}


const StyleCoW *ArcLabelled::GetRefinementStyle(EArcType t) const
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
    case MSC_BOX_SOLID:
        return &chart->Contexts.back().styles["--"];
    case MSC_BOX_DASHED:
        return &chart->Contexts.back().styles["++"];
    case MSC_BOX_DOTTED:
        return &chart->Contexts.back().styles[".."];
    case MSC_BOX_DOUBLE:
        return &chart->Contexts.back().styles["=="];
    case MSC_BOX_UNDETERMINED_FOLLOW:
        return NULL; /*do nothing*/
    case MSC_ARC_DIVIDER:
        return &chart->Contexts.back().styles["---"];
    case MSC_ARC_DISCO:
        return &chart->Contexts.back().styles["..."];
    default:
        return NULL;
    };
}

void ArcLabelled::AddAttributeList(AttributeList *l)
{
    if (!valid) return;
    //Find position of label attribute (if any), prepend it via an escape
    FileLineCol label_pos;
    if (l)
        for (auto pAttr : *l)
            if (pAttr->Is("label")) 
                label_pos = pAttr->linenum_value.start;
    //Add attributest 
    ArcBase::AddAttributeList(l);
    //vspacing went to the style, copy it
    if (style.read().vspacing.first)
        vspacing = style.read().vspacing.second;
    //Then convert color and style names in labels
    if (label.length()>0) {
        StringFormat::ExpandReferences(label, chart, label_pos, &style.read().text,
                                          false, true, StringFormat::LABEL);
        //re-insert position, so that FinalizeLabels has one
        label.insert(0, label_pos.Print());
    }
}

bool ArcLabelled::AddAttribute(const Attribute &a)
{
    if (a.type == MSC_ATTR_STYLE) {
        style.write().AddAttribute(a, chart);
        return true;
    }
    string s;
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
                style.write().numbering.first = true;
                style.write().numbering.second = true;
            } else
                chart->Error.Error(a, true, "Value for 'number' must not be negative. Ignoring attribute.");
            return true;
        }
        if (a.type == MSC_ATTR_BOOL) {
            style.write().numbering.first = true;
            style.write().numbering.second = a.yes;
            return true;
        }
        if (a.type == MSC_ATTR_CLEAR) { //turn off numbering
            style.write().numbering.first = true;
            style.write().numbering.second = false;
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
            FileLineCol l(a.linenum_value.start);
            l.col += (unsigned)a.value.length() - off;
            chart->Error.Warning(l, "I could not understand number from here. Applying only '" +
                                 a.value.substr(0, a.value.length() - off) + "'.");
        }
        concrete_number = num;
        style.write().numbering.first = true;
        style.write().numbering.second = true;
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
    if (style.write().AddAttribute(a, chart))
        return true;
    if (ArcBase::AddAttribute(a)) return true;
    a.InvalidAttrError(chart->Error);
    return false;
}

void ArcLabelled::AttributeNames(Csh &csh)
{
    ArcBase::AttributeNames(csh);
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "label", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "number", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "draw_time", HINT_ATTR_NAME));
    csh.AddStylesToHints();
}

bool ArcLabelled::AttributeValues(const std::string attr, Csh &csh)
{
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
        csh.AddToHints(EnumEncapsulator<EDrawPassType>::names, csh.HintPrefix(COLOR_ATTRVALUE), 
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
//fills the "vspacing" member from the style.read().
//Strictly to be called by descendants
ArcBase *ArcLabelled::PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right, 
                                       Numbering &number, Element **target, ArcBase * vertical_target)
{
    if (!valid) return NULL;
    //We do everything here even if we are hidden (numbering is not impacted by hide/show or collapse/expand)
    if (label.length()!=0 && style.read().numbering.second) {
        number.SetSize((unsigned)numberingStyle.Size()); //append 1s if style has more levels
        if (concrete_number >= 0)
            number.Last() = concrete_number;
        number_text = numberingStyle.Print(number);
        //Now remove escapes from the number text (if any)
        //Recreate the text style at the point where the label will be inserted
        StringFormat basic = style.read().text;
        basic.Apply(label.c_str());
        //At this point the number text must be processed using StringFormat::ExpandReferences
        //to expand remaining empty \c(), \s(), etc escapes.
        //We use a dummy linenum, as we should not get ANY errors here...
        StringFormat::ExpandReferences(number_text, chart, FileLineCol(), &basic,
            false, true, StringFormat::LABEL);
        //insert us among the references if we have any name
        if (refname.length()) 
            chart->ReferenceNames[refname].number_text = number_text;
        ++number;
    }
    return ArcBase::PostParseProcess(canvas, hide, left, right, number, target, vertical_target);
}

void ArcLabelled::FinalizeLabels(Canvas &canvas) 
{
    ArcBase::FinalizeLabels(canvas);
    if (label.length()==0) return;
    string pre_num_post;
    if (label.length()!=0 && style.read().numbering.second) 
        pre_num_post = numberingStyle.pre.second + number_text + numberingStyle.post.second;
    //We add empty num and pre_num_post if numberin is turned off, to remove \N escapes
    StringFormat::AddNumbering(label, number_text, pre_num_post);
    //Next we add reference numbers to labels, and also kill off any \s or the like
    //escapes that came with numbers in pre_num_post
    //We can start with a dummy pos, since the label's pos is prepended
    //during AddAttributeList. Note that with this calling
    //(references parameter true), ExpandReferences will only emit errors
    //to missing references - all other errors were already emitted in
    //the call in AddAttributeList()
    StringFormat copy(style.read().text);
    StringFormat::ExpandReferences(label, chart, FileLineCol(), 
        &copy, true, true, StringFormat::LABEL);
    parsed_label.Set(label, canvas, style.read().text);
}

void ArcLabelled::PostPosProcess(Canvas &canvas)
{
	//If there is a vline or vfill in the current style, add that to entitylines
    if ((style.read().f_vline &&(style.read().vline.width.first || style.read().vline.type.first || style.read().vline.color.first)) ||
        (style.read().f_vfill && !style.read().vfill.IsEmpty())) 
        if (!entityLineRange.IsInvalid()) {
		    StyleCoW toadd;
            if (style.read().f_vline) toadd.write().vline = style.read().vline;
            if (style.read().f_vfill) toadd.write().vfill = style.read().vfill;
            for(auto pEntity : chart->ActiveEntities)
                if (!chart->IsVirtualEntity(pEntity))
                    pEntity->status.ApplyStyle(entityLineRange, toadd);
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
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "slant_depth", HINT_ATTR_NAME));
    ArcLabelled::AttributeNames(csh);
    defaultDesign.styles.GetStyle("arrow").read().AttributeNames(csh);
}

bool ArcArrow::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr,"angle")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number 0..45 [degrees]>", HINT_ATTR_VALUE, false));
        return true;
    }
    if (CaseInsensitiveEqual(attr, "slant_depth")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number 0..1000 [pixels]>", HINT_ATTR_VALUE, false));
        return true;
    }
    if (defaultDesign.styles.GetStyle("arrow").read().AttributeValues(attr, csh)) return true;
    if (ArcLabelled::AttributeValues(attr, csh)) return true;
    return false;
}

void ArcArrow::PostPosProcess(Canvas &canvas)
{
    if (!valid) return;
    entityLineRange = area.GetBoundingBox().y;
    ArcLabelled::PostPosProcess(canvas);
}

void ArcArrow::DrawLSym(Canvas &canvas, const XY &C, XY size)
{
    auto before = cairo_get_line_cap(canvas.GetContext());
    cairo_set_line_cap(canvas.GetContext(), CAIRO_LINE_CAP_ROUND);
    canvas.Line(C - size/2, C + size/2, style.read().lsym_line);
    size.x *= -1;
    canvas.Line(C - size/2, C + size/2, style.read().lsym_line);
    cairo_set_line_cap(canvas.GetContext(), before);
}

//////////////////////////////////////////////////////////////////////////////////////

ArcSelfArrow::ArcSelfArrow(EArcType t, const char *s, const FileLineColRange &sl,
                           Msc *msc, const StyleCoW &st, double ys) :
    ArcArrow(t, MscProgress::SELF_ARROW, msc, st), YSize(ys), XSizeUnit(0.375)
{
    src = chart->FindAllocEntity(s, sl);
}

ArcArrow *ArcSelfArrow::AddSegment(ArrowSegmentData /*data*/, const char * /*m*/, 
                                    const FileLineColRange &/*ml*/, const FileLineColRange &l)
{
    if (!valid) return this; //display error only once
    chart->Error.Error(l.start, "Cannot add further segments to an arrow pointing to the same entity. Ignoring arrow.");
    valid = false;
    return this;
}

ArcArrow *ArcSelfArrow::AddLostPos(VertXPos *pos, const FileLineColRange &l)
{
    chart->Error.Error(l.start, "Cannot add a loss position to an arrow pointing to the same entity. Ignoring 'at' clause.");
    delete pos;
    return this;
}


EDirType ArcSelfArrow::GetToucedEntities(class EntityList &el) const
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

ArcBase* ArcSelfArrow::PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right, 
                                        Numbering &number, Element **target, ArcBase *vertical_target)
{
    if (!valid) return NULL;
    if (chart->ErrorIfEntityGrouped(src, (*src)->file_pos)) return NULL;

    //Add numbering, if needed
    ArcLabelled::PostParseProcess(canvas, hide, left, right, number, target, vertical_target);

    const EIterator substitute = chart->FindActiveParentEntity(src);
    const bool we_disappear = src != substitute; //src is not visible -> we disappear, too
    if (we_disappear && !(*substitute)->running_style.read().indicator.second) 
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

void ArcSelfArrow::Width(Canvas &, EntityDistanceMap &distances, DistanceMapVertical &vdist)
{
    if (!valid) return;
    //Add a new element to vdist
    vdist.InsertElementTop(this);
    //Add activation status right away
    AddEntityLineWidths(vdist);
    vdist.InsertEntity(src);
    const double left = chart->XCoord(XSizeUnit)+src_act+style.read().line.LineWidth()/2;
    const double right = parsed_label.getSpaceRequired() + src_act;
    distances.Insert((*src)->index, DISTANCE_RIGHT, left);
    distances.Insert((*src)->index, DISTANCE_LEFT, right);
    vdist.Insert((*src)->index, DISTANCE_RIGHT, left);
    vdist.Insert((*src)->index, DISTANCE_LEFT, right);
    //Add a new element to vdist
    vdist.InsertElementBottom(this);
    //Add activation status right away
    AddEntityLineWidths(vdist);
}

void ArcSelfArrow::Layout(Canvas &canvas, AreaList *cover)
{
    height = 0;
    if (!valid) return;
    yPos = 0;
    xy_s = style.read().arrow.getWidthHeight(isBidir(), MSC_ARROW_START);
    xy_e = style.read().arrow.getWidthHeight(isBidir(), MSC_ARROW_END);
    xy_s.y = ceil(xy_s.y);
    xy_e.y = ceil(xy_e.y);
    wh.x = ceil(chart->XCoord(XSizeUnit));
    wh.y = ceil(2*YSize);
    dx = chart->XCoord(src);
    sx = 0;
    if (parsed_label.IsWordWrap()) {
        //find entity left of `src` that is on
        auto i = src;
        i--;
        while (*i != chart->LSide && !(*i)->status.GetStatus(yPos).IsOn())
            i--;
        //at worst we are at LSide
        sx = chart->XCoord(i);
        if ((*i)->status.GetStatus(yPos).IsActive())
            sx += chart->activeEntitySize/2;
        _ASSERT(dx - src_act - sx > 0);
        const double overflow = parsed_label.Reflow(canvas, dx - src_act - sx);
        OverflowWarning(overflow, "", i, src);
    } else {
        CountOverflow(dx - src_act - sx);
    }

    double y = chart->arcVGapAbove;
    area = parsed_label.Cover(sx, dx-src_act, y);
    area.arc = this;
    area_important = area;
    const Block arrow_box(dx+src_act, ceil(dx+src_act+wh.x), y, ceil(y+xy_s.y+wh.y+xy_e.y));
    area += arrow_box;
    area.mainline = Block(chart->GetDrawing().x, Range(y - chart->nudgeSize/2, y + wh.y + chart->nudgeSize/2));
    //Now add arrowheads to the "area_important", and a small block if they are NONE
    XY point = XY(dx+src_act, xy_s.y + chart->arcVGapAbove);
    if (style.read().arrow.GetType(isBidir(), MSC_ARROW_START) == MSC_ARROW_NONE)
        area_important += Block(point.x-chart->compressGap/2, point.x+chart->compressGap/2,
        point.y-chart->compressGap/2, point.y+chart->compressGap/2);
    else
        area_important += style.read().arrow.Cover(point, 0, true,  isBidir(), MSC_ARROW_START, style.read().line, style.read().line);
    point.y += 2*YSize;
    if (style.read().arrow.GetType(isBidir(), MSC_ARROW_END) == MSC_ARROW_NONE)
        area_important += Block(point.x-chart->compressGap/2, point.x+chart->compressGap/2,
        point.y-chart->compressGap/2, point.y+chart->compressGap/2);
    else
        area_important += style.read().arrow.Cover(point, 0, false, isBidir(), MSC_ARROW_END, style.read().line, style.read().line);
    chart->NoteBlockers.Append(this);
    height = area.GetBoundingBox().y.till + chart->arcVGapBelow;
    if (cover)
        *cover = GetCover4Compress(area);
    LayoutComments(canvas, cover);
}

void ArcSelfArrow::PostPosProcess(Canvas &canvas)
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

void ArcSelfArrow::Draw(Canvas &canvas, EDrawPassType pass)
{
    if (!valid) return;
    if (pass!=draw_pass) return;
    double y = yPos + chart->arcVGapAbove;

    parsed_label.Draw(canvas, sx, dx-src_act, y);
    y += xy_s.y;

    if (style.read().line.radius.second < 0) {
        //draw an arc
        const Contour curve(XY(dx+src_act, y+YSize), wh.x, wh.y/2, 0, 270, 90);
        curve[0][curve[0].size()-1].visible = false; //last edge is the linear segment closing it - do not draw
        canvas.Line(curve, style.read().line);
    } else {
        //draw (part of) a rounded rectangle
        canvas.Clip(dx+src_act, chart->GetDrawing().x.till, chart->GetDrawing().y.from, chart->GetDrawing().y.till);
        canvas.Line(Block(XY(0, y), XY(dx,y)+wh), style.read().line);
        canvas.UnClip();
    }
    //draw arrowheads
    style.read().arrow.Draw(XY(dx+src_act, y+2*YSize), 0, false, isBidir(), MSC_ARROW_END, style.read().line, style.read().line, &canvas);
    style.read().arrow.Draw(XY(dx+src_act, y        ), 0, true,  isBidir(), MSC_ARROW_START, style.read().line, style.read().line, &canvas);
}

//////////////////////////////////////////////////////////////////////////////////////

ArcDirArrow::ArcDirArrow(ArrowSegmentData data, const char *s, const FileLineColRange &sl,
                         const char *d, const FileLineColRange &dl, Msc *msc, bool fw, 
                         const StyleCoW &st) :
    ArcArrow(data.type, MscProgress::DIR_ARROW, msc, st), 
    linenum_src(sl.start), linenum_dst(dl.start), 
    specified_as_forward(fw), slant_angle(0), slant_depth(0),
    lost_at(-2), lost_pos(*msc)
{
    src = chart->FindAllocEntity(s, sl);
    dst = chart->FindAllocEntity(d, dl);
    segment_types.push_back(data.type);
    if (chart) 
        slant_angle = chart->Contexts.back().slant_angle.second;
    if (data.lost==EArrowLost::AT_SRC) {
        lost_at = -1;
        lost_is_forward = true;
        linenum_asterisk = data.lost_pos.CopyTo().start;
    } else if (data.lost==EArrowLost::AT_DST) {
        lost_at = 0;
        lost_is_forward = false;
        linenum_asterisk = data.lost_pos.CopyTo().start;
    } 
};

ArcDirArrow::ArcDirArrow(const EntityList &el, bool bidir, const ArcLabelled &al) :
    ArcArrow(bidir ? MSC_ARC_BIG_BIDIR : MSC_ARC_BIG, MscProgress::BLOCK_ARROW, al),
    specified_as_forward(false), slant_angle(0), slant_depth(0), 
    lost_at(-2), lost_pos(*ArcArrow::chart)
{
    src = chart->AllEntities.Find_by_Ptr(*el.begin());
    dst = chart->AllEntities.Find_by_Ptr(*el.rbegin());
    segment_types.push_back(bidir ? MSC_ARC_BIG_BIDIR : MSC_ARC_BIG);
    segment_lines.push_back(style.read().line);
    for (auto i = ++el.begin(); i!=--el.end(); i++) {
        middle.push_back(chart->AllEntities.Find_by_Ptr(*i));
        segment_types.push_back(MSC_ARC_BIG);
        segment_lines.push_back(style.read().line);
    }
    if (chart) 
        slant_angle = chart->Contexts.back().slant_angle.second;
}

ArcArrow * ArcDirArrow::AddSegment(ArrowSegmentData data, const char *m, const FileLineColRange &ml,
                                   const FileLineColRange &/*l*/)
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
        segment_types.push_back(data.type);
    } else {
        //check for this situation: <-b<-a (where a is left of b)
        if (middle.size()==0 && *dst == chart->LSide &&
            (*src)->pos > (*mid)->pos && m!=NULL)
            dst = chart->FindAllocEntity(RSIDE_ENT_STR, ml);
        middle.insert(middle.begin(), src);
        linenum_middle.insert(linenum_middle.begin(), linenum_src);
        src = mid;
        linenum_src = ml.start;
        segment_types.insert(segment_types.begin(), data.type);
        //if we have specified a message lost at an entity, 
        //we maintain the correct index
        if (lost_at > -2)
            lost_at++;
    };
    //Error if we already have a loss specified
    if (data.lost!=EArrowLost::NOT && lost_at>-2) {
        chart->Error.Error(data.lost_pos.CopyTo().start, "This arrow is already specified as lost. Ignoring this asterisk.");
    } else if (data.lost==EArrowLost::AT_SRC) {
        lost_at = specified_as_forward ? middle.size()-1 : -1;
        lost_is_forward = true;
        linenum_asterisk = data.lost_pos.CopyTo().start;
    } else if (data.lost==EArrowLost::AT_DST) {
        lost_at = specified_as_forward ? middle.size() : 0;
        lost_is_forward = false;
        linenum_asterisk = data.lost_pos.CopyTo().start;
    }
    return this;
}

ArcArrow *ArcDirArrow::AddLostPos(VertXPos *pos, const FileLineColRange &l)
{
    if (!pos || !pos->valid || pos->pos==VertXPos::POS_INVALID)
        chart->Error.Error(l.start, "The 'at' clause is invalid. Ignoring it.");
    else {
        lost_pos = *pos;
        linenum_lost_at = l.start;
    }
    delete pos;
    return this;
}


void ArcDirArrow::AddAttributeList(AttributeList *l)
{
    //Save the style, empty it, collect all modifications and apply those to segments, too
    //This will work even if we are a BigArrow
    StyleCoW save(style);
    style.write().Empty();
    ArcArrow::AddAttributeList(l);
    for (unsigned i=0; i<segment_types.size(); i++) {
        segment_lines.push_back(save.read().line);
        const StyleCoW * const refinement = GetRefinementStyle(segment_types[i]);
        if (refinement) {
            //Add the line type of the refinement style to each segment
            *segment_lines.rbegin() += refinement->read().line;
            //Add all other style elements of the refinement style to us
            save.write() += refinement->read();
        }
        *segment_lines.rbegin() += style.read().line;
    }
    save += style;
    style = save;
    //vspacing went to the style, copy it
    if (style.read().vspacing.first)
        vspacing = style.read().vspacing.second;
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
        } else {
            slant_angle = a.number;
            slant_depth = 0;
        }
        return true;
    }
    if (a.Is("slant_depth")) {
        if (!a.EnsureNotClear(chart->Error, STYLE_ARC)) return true;
        if (!a.CheckType(MSC_ATTR_NUMBER, chart->Error)) return true;
        if (a.number<0 || a.number>1000) 
            chart->Error.Error(a, true, "Slant depth must be between 0 and 1000 pixels. Ignoring this attribute.");
        else {
            slant_angle = 0;
            slant_depth = a.number;
        }
        return true;

    }
    return ArcArrow::AddAttribute(a);
}

EDirType ArcDirArrow::GetToucedEntities(class EntityList &el) const
{
    el.push_back(*src);
    el.push_back(*dst);
    for (auto i : middle)
        el.push_back(*i);
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

ArcBase *ArcDirArrow::PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right, 
                                       Numbering &number, Element **target, ArcBase *vertical_target)
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
    ArcLabelled::PostParseProcess(canvas, hide, left, right, number, target, vertical_target);

    //Update src, dst and mid
    EIterator sub;
    sub = chart->FindActiveParentEntity(src);
    src = chart->ActiveEntities.Find_by_Ptr(*sub);
    _ASSERT(src != chart->ActiveEntities.end());
    sub = chart->FindActiveParentEntity(dst);
    dst = chart->ActiveEntities.Find_by_Ptr(*sub);
    _ASSERT(dst != chart->ActiveEntities.end());
    if (src == dst && !(*src)->running_style.read().indicator.second) //We became a degenerate arrow, do not show us
        return NULL;
    //Change left and right only if they actually point to a "real entity"
    //and not (left) or (right). If they do, consider our other "end"
    if (!chart->IsVirtualEntity(*src)) {
        left = chart->EntityMinByPos(left, src);
        right = chart->EntityMaxByPos(right, src);
    }
    if (!chart->IsVirtualEntity(*dst)) {
        right = chart->EntityMaxByPos(right, dst);
        left = chart->EntityMinByPos(left, dst);
    }
    //Consider the middle elements if any of our endpoints is virtual
    //(else we are good only by checkind the endpoints above
    if (chart->IsVirtualEntity(*src) || chart->IsVirtualEntity(*dst))
        for (auto iMiddle : middle) {
            left = chart->EntityMinByPos(left, iMiddle);
            right = chart->EntityMaxByPos(right, iMiddle);
        }

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
    if (isBidir()) 
        slant_depth = slant_angle = 0;
    else if ((*src)->index > (*dst)->index) 
            slant_angle = -slant_angle;
    
    //At this point if the user specified a slant_depth of nonzero,
    //slant_angle will be zero (which is not the final slant). 
    //Nevertheless, we will proceed to calculate 'act_size' and go on
    //doing the Width() function call as if there were no slant.
    //(Luckily in this way we may only ask for *more* space than
    //with slant, so nothing will overlap nothing else.)
    //Then in Layout, when we already know the positions of the
    //entities, we calculate slant_angle, cos_slant and sin_slant
    //from slant_depth.
    sin_slant = slant_angle ? sin(slant_angle*M_PI/180.) : 0.;
    cos_slant = slant_angle ? cos(slant_angle*M_PI/180.) : 1.;

    //record what entities are active at the arrow (already consider slant)
    act_size.clear();
    act_size.reserve(2+middle.size());
    act_size.push_back(std::max(0., (*src)->GetRunningWidth(chart->activeEntitySize)/2)/cos_slant);
    for (unsigned iiii = 0; iiii<middle.size(); iiii++) 
        act_size.push_back(std::max(0., (*middle[iiii])->GetRunningWidth(chart->activeEntitySize)/2)/cos_slant);
    act_size.push_back(std::max(0., (*dst)->GetRunningWidth(chart->activeEntitySize)/2)/cos_slant);

    //If the message are lost and the user specified no 'at' clause,
    //fill in which entities it is lost between.
    //After this we rely only on 'lost_pos'
    if (!lost_pos.valid || lost_pos.pos==VertXPos::POS_INVALID) {
        if (lost_at > -2) {
            lost_pos.valid = true;
            lost_pos.offset = 0;
            lost_pos.e1line.MakeInvalid();
            lost_pos.e2line.MakeInvalid();

            lost_pos.entity1 = lost_at == -1 ? src : unsigned(lost_at) == middle.size() ? dst : middle[lost_at];
            lost_pos.entity2 = lost_pos.entity1;
            //calculate the entity that is both a) part of the arrow (src, dst or middle)
            //and b) is immediately left/right of the entity marked at lost_at.
            const int p = lost_is_forward ? lost_at + 1 : lost_at - 1;
            _ASSERT(p>=-1 && p<=int(middle.size()));
            const EIterator pair = p <= -1 ? src :
                                   unsigned(p) >= middle.size() ? dst :
                                   middle[p];

            //find next (or prev) active and showing entity. 
            //If at end (or beginning) use RSide (or LSide)
            //If the next/prev entity is part of the arrow, put the 
            //loss at one third of the distance. Else put into the middle.
            if (((*src)->index < (*dst)->index) == lost_is_forward) {
                do {
                    lost_pos.entity2++;
                } while (lost_pos.entity2 !=  chart->ActiveEntities.end() &&
                         !(*lost_pos.entity2)->running_shown.IsOn());
                if (lost_pos.entity2 ==  chart->ActiveEntities.end())
                    lost_pos.entity2 = chart->ActiveEntities.Find_by_Ptr(chart->RSide);
            } else {
                do {
                    lost_pos.entity2--;
                } while (lost_pos.entity2 !=  chart->ActiveEntities.end() &&
                    !(*lost_pos.entity2)->running_shown.IsOn());
                if (lost_pos.entity2 ==  chart->ActiveEntities.end())
                    lost_pos.entity2 = chart->ActiveEntities.Find_by_Ptr(chart->LSide);
            }
            lost_pos.pos = *lost_pos.entity2 != *pair ? VertXPos::POS_CENTER :
                           lost_is_forward ? VertXPos::POS_THIRD_LEFT : 
                                             VertXPos::POS_THIRD_RIGHT;
        } else //when lost_at==-2 
            lost_pos.valid = false; //make sure this field is false
    }
    return this;
}

void ArcDirArrow::UpdateActiveSizes() 
{
    //We take the maximum of the existing activation size and the ones in the
    //running state of entities.

    //We assume act_size is still ordered as after PostParseProcess() and 
    //before Layout(), that is {src,middle[],dst}
    auto i = act_size.begin();
    *i = std::max(*i, (*src)->GetRunningWidth(chart->activeEntitySize)/2/cos_slant);
    for (unsigned u = 1; u<=middle.size(); u++) 
        act_size[u] = std::max(act_size[u], (*middle[u-1])->GetRunningWidth(chart->activeEntitySize)/2/cos_slant);
    i = --act_size.end();
    *i = std::max(*i, (*dst)->GetRunningWidth(chart->activeEntitySize)/2/cos_slant);
}


void ArcDirArrow::FinalizeLabels(Canvas &canvas)
{
    ArcArrow::FinalizeLabels(canvas);
    if (parsed_label.getTextWidthHeight().y==0) return;
    //Insert a small extra spacing for the arrow line
    double lw_max = style.read().line.LineWidth();
    for (auto i : segment_lines)
        lw_max = std::max(lw_max, i.LineWidth());
    parsed_label.AddSpacingAfterFirstLine(ARROW_TEXT_VSPACE_ABOVE + lw_max + ARROW_TEXT_VSPACE_BELOW);
}


const double ArcDirArrow::lsym_side_by_offset = 0.5;
const double LSYM_SIZE_ADJ = 0.66;

void ArcDirArrow::Width(Canvas &canvas, EntityDistanceMap &distances, DistanceMapVertical &vdist)
{
    if (!valid) return;
    //Add a new element to vdist
    vdist.InsertElementTop(this);
    //Add activation status right away
    AddEntityLineWidths(vdist);
    vdist.InsertEntity(src);
    vdist.InsertEntity(dst);

    //Here we have a valid canvas, so we adjust act_size
    if (canvas.HasImprecisePositioning() && slant_angle==0)
        for (auto &i : act_size)
            i = floor(i);

    //we lie about us being forward (we do not check), so we know which of first/second to use
    DoublePair end = style.read().arrow.getWidths(true, isBidir(), MSC_ARROW_END, style.read().line);
    DoublePair start = style.read().arrow.getWidths(true, isBidir(), MSC_ARROW_START, style.read().line);
    distances.Insert((*src)->index, (*dst)->index,
                     (end.first + start.second + parsed_label.getSpaceRequired() +
                     *act_size.begin() + *act_size.rbegin())*cos_slant);
    //Add distances for arrowheads
    const bool fw = (*src)->index  <  (*dst)->index;
    const double s = (start.second + *act_size.begin())*cos_slant;
    const double d = (end.first    + *act_size.rbegin())*cos_slant;
    distances.Insert((*src)->index, fw ? DISTANCE_RIGHT : DISTANCE_LEFT, s);
    distances.Insert((*dst)->index, fw ? DISTANCE_LEFT : DISTANCE_RIGHT, d);
    vdist.Insert((*src)->index, fw ? DISTANCE_RIGHT : DISTANCE_LEFT, s);
    vdist.Insert((*dst)->index, fw ? DISTANCE_LEFT : DISTANCE_RIGHT, d);

    if (middle.size()) {
        EntityDistanceMap d;
        for (unsigned i = 0; i<middle.size(); i++) {
            DoublePair mid = style.read().arrow.getWidths(fw, isBidir(), MSC_ARROW_MIDDLE, style.read().line);
            const double left = (mid.first  + act_size[i+1])*cos_slant;
            const double right = (mid.second + act_size[i+1])*cos_slant;
            distances.Insert((*middle[i])->index, DISTANCE_LEFT, left);
            distances.Insert((*middle[i])->index, DISTANCE_RIGHT, right);
            vdist.Insert((*middle[i])->index, DISTANCE_LEFT, left);
            vdist.Insert((*middle[i])->index, DISTANCE_RIGHT, right);
        }
        d.CombinePairedLeftRightToPair_Sum(chart->hscaleAutoXGap);
        distances += d;
    }
    //If a lost symbol is generated, do some
    if (lost_pos.valid) {
        lsym_size.y = lsym_size.x =
            ArrowHead::baseArrowWidth * LSYM_SIZE_ADJ * ArrowHead::arrowSizePercentage[style.read().lsym_size.second]/100.;
        const double aw = lsym_side_by_offset * lsym_size.x;
        const double gap = chart->hscaleAutoXGap;
        const unsigned e1 = (*lost_pos.entity1)->index;
        const unsigned e2 = (*lost_pos.entity2)->index;
        //If the arrow symbol is specified between two neighbouring entity,
        //add some space for it
        switch (lost_pos.pos) {
        case VertXPos::POS_CENTER:
            if (abs(int(e1)-int(e2))==1)
                distances.Insert(e1, e2, distances.Query(e1, e2) + lsym_size.x + 2*gap);
            break;
        case VertXPos::POS_THIRD_LEFT:
        case VertXPos::POS_THIRD_RIGHT:
            if (abs(int(e1)-int(e2))==1)
                distances.Insert(e1, e2, distances.Query(e1, e2) + 3*lsym_size.x + 2*gap);
            break;
        case VertXPos::POS_AT:
            distances.Insert(e1, DISTANCE_LEFT, lsym_size.x/2 + gap);
            distances.Insert(e1, DISTANCE_RIGHT, lsym_size.x/2 + gap);
            vdist.Insert(e1, DISTANCE_LEFT, lsym_size.x/2 + gap);
            vdist.Insert(e1, DISTANCE_RIGHT, lsym_size.x/2 + gap);
            break;
        case VertXPos::POS_LEFT_BY:     
            distances.Insert(e1, DISTANCE_LEFT, lsym_size.x + aw + gap);
            vdist.Insert(e1, DISTANCE_LEFT, lsym_size.x + aw + gap);
            break;
        case VertXPos::POS_RIGHT_BY:    
            distances.Insert(e1, DISTANCE_RIGHT, lsym_size.x + aw + gap);
            vdist.Insert(e1, DISTANCE_RIGHT, lsym_size.x + aw + gap);
            break;
        case VertXPos::POS_LEFT_SIDE:
            distances.Insert(e1, DISTANCE_LEFT, lsym_size.x + gap);
            vdist.Insert(e1, DISTANCE_LEFT, lsym_size.x + gap);
            break;
        case VertXPos::POS_RIGHT_SIDE:
            distances.Insert(e1, DISTANCE_RIGHT, lsym_size.x + gap);
            vdist.Insert(e1, DISTANCE_RIGHT, lsym_size.x + gap);
            break;
        case VertXPos::POS_INVALID:
        default:
            _ASSERT(0);
        }
    } else 
        //if no lost position
        lsym_size.x = lsym_size.y = 0;
    //Add a new element to vdist
    vdist.InsertElementBottom(this);
    //Add activation status right away
    AddEntityLineWidths(vdist);
}

EArrowEnd ArcDirArrow::WhichArrow(unsigned i)
{
    //in xPos (may not be filled up yet, index 0 will be the src and indes xpos.size()-1 will be dest
    //in between there will be middle.size() number of middle arrows.
    //"i" here is an index that will be used for xPos.
    if (i>0 && i<middle.size()+1) return MSC_ARROW_MIDDLE;
    if ((i==0) == ((*src)->index  <  (*dst)->index)) return MSC_ARROW_START;
    return MSC_ARROW_END;
}

void ArcDirArrow::Layout(Canvas &canvas, AreaList *cover)
{
    height = 0;
    if (!valid) return;
    yPos = 0;
    area.clear();
    sx = chart->XCoord(src);
    dx = chart->XCoord(dst);
    //If the user specified slant_depth, we shall re-calculate 
    //slant_angle, cos_slant and sin_slant.
    if (slant_depth) {
        _ASSERT(slant_angle==0);
        //slant_angle shall be negative if dx < sx;
        const double rad = atan(slant_depth/(dx-sx));
        sin_slant = sin(rad);
        cos_slant = cos(rad);
        slant_angle = rad/M_PI*180.;
    }
    //Check if the loss happens in-between (untransformed) sx and dx
    if (lost_pos.valid) {
        cx_lsym = lost_pos.CalculatePos(*chart, lsym_size.x, lsym_size.x*lsym_side_by_offset);
        if (cx_lsym < std::min(sx, dx) || std::max(sx, dx) < cx_lsym) {
            chart->Error.Error(linenum_lost_at.IsInvalid() ? this->file_pos.start : linenum_lost_at,
                "The position of the loss is " +
                std::string(cx_lsym < std::min(sx, dx) ? "left" : "right") +
                " of the arrow. Ignoring it.");
            lost_pos.valid = false;
        }
    }
    //convert dx and cx_lsym to transformed space
    if (slant_angle) {
        dx = sx + (dx-sx)/cos_slant;
        cx_lsym = sx + (cx_lsym-sx)/cos_slant;
    }

    double lw_max = style.read().line.LineWidth();
    for (auto i : segment_lines)
        lw_max = std::max(lw_max, i.LineWidth());
    const XY xy_e = style.read().arrow.getWidthHeight(isBidir(), MSC_ARROW_END);
    const XY xy_s = style.read().arrow.getWidthHeight(isBidir(), MSC_ARROW_START);
    //If there are middle arrows, make aH be the highest of endType/startType
    //and midType arrows.
    //If not use endType/startType only
    //aH.y is _half_ the height of the arrowhead (the height above/below the centerline)
    //aH.x is the width on one side on the entity line only.
    double aH = std::max(std::max(xy_e.y, xy_s.y), lsym_size.y/2);
    if (middle.size()>0)
        aH = max(aH, style.read().arrow.getWidthHeight(isBidir(), MSC_ARROW_MIDDLE).y);

    double y = chart->arcVGapAbove;
    XY text_wh = parsed_label.getTextWidthHeight();
    if (text_wh.y) {
        //Determine space for the text, reflow if needed and calculate cover
        if (sx<dx) {
            sx_text = sx + xy_s.x + *act_size.begin();
            dx_text = dx - xy_e.x - *act_size.rbegin();
        } else {
            sx_text = dx + xy_e.x + *act_size.rbegin();
            dx_text = sx - xy_s.x - *act_size.begin();
        }
        if (parsed_label.IsWordWrap()) {
            const double overflow = parsed_label.Reflow(canvas, dx_text - sx_text);
            OverflowWarning(overflow, "", sx<dx ? src : dst, sx<dx ? dst : src);
            text_wh = parsed_label.getTextWidthHeight(); //refresh
        } else {
            CountOverflow(dx_text - sx_text);
        }
        cx_text = (sx+dx)/2;
        area = text_cover = parsed_label.Cover(sx_text, dx_text, y, cx_text);
        area.arc = this;
        //determine top edge position of arrow midline
        const double firstLineHeight = parsed_label.getTextWidthHeight(0).y;
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
    margins.push_back(style.read().arrow.getWidths(sx<dx, isBidir(), MSC_ARROW_START, style.read().line));
    for (auto i : middle) {
        xPos.push_back(sx + (chart->XCoord(i)-sx)/cos_slant);
        margins.push_back(style.read().arrow.getWidths(sx<dx, isBidir(), MSC_ARROW_MIDDLE, style.read().line));
    }
    xPos.push_back(dx);
    margins.push_back(style.read().arrow.getWidths(sx<dx, isBidir(), MSC_ARROW_END, style.read().line));
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
    clip_area  = style.read().arrow.ClipForLine(XY(sx, y), s_act, sx<dx, isBidir(), MSC_ARROW_START,
                                         total, *segment_lines.begin(), *segment_lines.begin());
    clip_area *= style.read().arrow.ClipForLine(XY(dx, y), d_act, sx<dx, isBidir(), MSC_ARROW_END,
                                         total, *segment_lines.rbegin(), *segment_lines.rbegin());
    for (unsigned i=1; i<xPos.size()-1; i++)
        clip_area *= style.read().arrow.ClipForLine(XY(xPos[i], y), act_size[i], sx<dx, isBidir(), MSC_ARROW_MIDDLE, 
                                             total, segment_lines[i-1], segment_lines[i]);

    //Add arrowheads and line segments to cover
    for (unsigned i=0; i<xPos.size(); i++)
        area += style.read().arrow.Cover(XY(xPos[i], y), act_size[i], sx<dx, isBidir(), WhichArrow(i),
        segment_lines[i - (i==0 ? 0 : 1)], segment_lines[i - (i==xPos.size()-1 ? 1 : 0)]);
    //Add the loss symbol
    if (lost_pos.valid)
        area += Block(XY(cx_lsym, centerline)-lsym_size/2, XY(cx_lsym, centerline)+lsym_size/2);
    area_important = area; //the text and arrowheads and lost symbol
    //Add small blocks if there is no end or start arrowhead
    if (style.read().arrow.GetType(isBidir(), MSC_ARROW_START) == MSC_ARROW_NONE)
        area_important += Block(sx-chart->compressGap/2, sx+chart->compressGap/2,
                                centerline-chart->compressGap/2, centerline+chart->compressGap/2);
    if (style.read().arrow.GetType(isBidir(), MSC_ARROW_START) == MSC_ARROW_NONE)
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
    if (cover) 
        *cover = GetCover4Compress(area);
    if (slant_angle==0)
        height = std::max(y+max(aH, lw_max/2), chart->arcVGapAbove + text_wh.y) + chart->arcVGapBelow;
    else
        height = area.GetBoundingBox().y.till;
    LayoutComments(canvas, cover);
}

double ArcDirArrow::GetCenterline(double x) const
{
    if (!slant_angle) 
        return centerline;

    //Calculate the x coordinate of the tip of the arrow
    //Here act_size is sorted from left to right and not from src->dst
    const double _sx = sx + cos_slant*(sx<dx ? *act_size.begin() : -*act_size.rbegin());
    const double _dx = sx + (dx-sx)*cos_slant + 
                            cos_slant*(sx<dx ? -*act_size.rbegin() : *act_size.begin());

    //if we are left of a -> arrow or right from a <- arrow
    //(Latter is an example of below, 'x' is at the + sign)
    //        o---+--------
    //       /
    //_____|/_
    //return the top line
    if ( (sx<dx && x<=_sx) || (sx>dx && x>=_sx) ) 
        return centerline;
    //if we are on the other side return the bottom line
    if ( (sx<dx && x>=_dx) || (sx>dx && x<=_dx) ) 
        return sin_slant*(_dx-_sx) + centerline;
    //We are on the arrow
    return sin_slant*(x-_sx) + centerline;
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
    vector<FileLineCol> linenum_temp = linenum_middle;
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


void ArcDirArrow::PostPosProcess(Canvas &canvas)
{
    if (!valid) return;
    ArcArrow::PostPosProcess(canvas);
    CheckSegmentOrder(yPos+centerline);
    //Exclude the areas covered by the text from entity lines
    chart->HideEntityLines(text_cover);
    //Exclude the areas covered by the arrow heads from entity lines
    const XY c(sx, yPos+centerline);
    Contour tmp;
    tmp = style.read().arrow.EntityLineCover(XY(sx, yPos+centerline), sx<dx, isBidir(), MSC_ARROW_START);
    if (!tmp.IsEmpty()) {
        tmp.RotateAround(c, slant_angle);
        chart->HideEntityLines(tmp);
    }
    tmp = style.read().arrow.EntityLineCover(XY(dx, yPos+centerline), sx<dx, isBidir(), MSC_ARROW_END);
    if (!tmp.IsEmpty()) {
        tmp.RotateAround(c, slant_angle);
        chart->HideEntityLines(tmp);
    }
    //for multi-segment arrows
    for (unsigned i=1; i<xPos.size()-1; i++) {
        tmp = style.read().arrow.EntityLineCover(XY(xPos[i], yPos+centerline), sx<dx, isBidir(), MSC_ARROW_MIDDLE);
        if (!tmp.IsEmpty()) {
            tmp.RotateAround(c, slant_angle);
            chart->HideEntityLines(tmp);
        }
    }
}

/** Draws an actual arrow using the parameters passed, not considering message loss. 
 * Used twice to draw a lost arrow. */
void ArcDirArrow::DrawArrow(Canvas &canvas, const Label &loc_parsed_label, 
                            const std::vector<LineAttr>& loc_segment_lines,
                            const ArrowHead &loc_arrow)
{
    const double y = yPos+centerline;  //should be integer
    if (loc_parsed_label.getTextWidthHeight().y) {
        if (slant_angle)
            loc_arrow.TransformCanvasForAngle(slant_angle, canvas, sx, y);
        loc_parsed_label.Draw(canvas, sx_text, dx_text, yPos + chart->arcVGapAbove, cx_text, slant_angle!=0);
        if (slant_angle)
            loc_arrow.UnTransformCanvas(canvas);
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
        canvas.Line(sta, end, loc_segment_lines[i]);
    }
    if (!canvas.NeedsArrowFix()) canvas.UnClip();
    /* Now the arrow heads */
    if (slant_angle)
        loc_arrow.TransformCanvasForAngle(slant_angle, canvas, sx, y);
    for (unsigned i=0; i<xPos.size(); i++)
        loc_arrow.Draw(XY(xPos[i], y), act_size[i], sx<dx, isBidir(), WhichArrow(i), 
                       loc_segment_lines[i - (i==0 ? 0 : 1)], loc_segment_lines[i - (i==xPos.size()-1 ? 1 : 0)],
                       &canvas);
    if (slant_angle)
        loc_arrow.UnTransformCanvas(canvas);
}

void ArcDirArrow::Draw(Canvas &canvas, EDrawPassType pass)
{
    if (!valid) return;
    if (pass!=draw_pass) return;
    const Block sb(Range(chart->GetDrawing().x.from, cx_lsym),
                   chart->GetDrawing().y);
    const Block db(Range(cx_lsym, chart->GetDrawing().x.till),
                   chart->GetDrawing().y);
    if (lost_pos.valid) 
        canvas.Clip(sx<dx ? sb : db);
    DrawArrow(canvas, parsed_label, segment_lines, style.read().arrow);
    if (lost_pos.valid) {
        canvas.UnClip();
        canvas.Clip(sx<dx ? db : sb);
        Label loc_parsed_label = parsed_label;
        loc_parsed_label.ApplyStyle(style.read().lost_text);
        std::vector<LineAttr> loc_segment_lines = segment_lines;
        for (auto &segment : loc_segment_lines)
            segment += style.read().lost_line;
        ArrowHead loc_arrow = style.read().arrow;
        loc_arrow += style.read().lost_arrow;
        DrawArrow(canvas, loc_parsed_label, loc_segment_lines, loc_arrow);
        canvas.UnClip();
        if (slant_angle)
            style.read().arrow.TransformCanvasForAngle(slant_angle, canvas, sx, yPos+centerline);
        DrawLSym(canvas, XY(cx_lsym, yPos+centerline), lsym_size);
        if (slant_angle)
            style.read().arrow.UnTransformCanvas(canvas);
    }
}
//////////////////////////////////////////////////////////////////////////////////////

ArcBigArrow::ArcBigArrow(const ArcDirArrow &dirarrow, const StyleCoW &s) : 
 ArcDirArrow(dirarrow), sig(NULL)
{
    SetStyleWithText(&s);
    //No need to unregister a DIR_ARROW: above copy constructor did not register    
    const_cast<MscProgress::ECategory&>(myProgressCategory) = MscProgress::BLOCK_ARROW;
    chart->Progress.RegisterArc(MscProgress::BLOCK_ARROW);
}

//This invocation is from ArcBoxSeries::PostParseProcess
ArcBigArrow::ArcBigArrow(const EntityList &el, bool bidir, const ArcLabelled &al,
    const ArcSignature *s)
    : ArcDirArrow(el, bidir, al), sig(s) //this sets myProgressCategory to BLOCK_ARROW
{
    slant_angle = 0;
}


const StyleCoW *ArcBigArrow::GetRefinementStyle(EArcType t) const
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
    defaultDesign.styles.GetStyle("blockarrow").read().AttributeNames(csh);
}

bool ArcBigArrow::AttributeValues(const std::string attr, Csh &csh)
{
    if (defaultDesign.styles.GetStyle("blockarrow").read().AttributeValues(attr, csh)) return true;
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

ArcBase* ArcBigArrow::PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                       Numbering &number, Element **target, ArcBase *vertical_target)
{
    if (!valid) return NULL;
    //Check that we have no loss specified
    if (lost_at > -2)
        chart->Error.Error(linenum_asterisk, "No support for message loss with block arrows. Ignoring asterisk.");
    if (lost_pos.valid && lost_pos.pos != VertXPos::POS_INVALID)
        chart->Error.Error(linenum_lost_at, "No support for message loss with block arrows. Ignoring 'lost at' clause.");
    lost_pos.valid = false;
    lost_at = -2;
    //Determine src and dst entity, check validity of multi-segment ones, add numbering, etc
    ArcBase *ret = ArcDirArrow::PostParseProcess(canvas, hide, left, right, number, target, vertical_target);
    //Finally copy the line attribute to the arrow, as well (arrow.line.* attributes are annulled here)
    style.write().arrow.line = style.read().line;
    return ret;
}

void ArcBigArrow::FinalizeLabels(Canvas &canvas)
{
    //Skip the part in ArcDirArrow adding first line spacing
    ArcArrow::FinalizeLabels(canvas);
}


void ArcBigArrow::Width(Canvas &canvas, EntityDistanceMap &distances, DistanceMapVertical &vdist)
{
    if (!valid) return;
    //Add a new element to vdist
    vdist.InsertElementTop(this);
    //Add activation status right away
    AddEntityLineWidths(vdist);
    vdist.InsertEntity(src);
    vdist.InsertEntity(dst);

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
    if (middle.size()>0 && !MSC_ARROW_IS_SYMMETRIC(style.read().arrow.midType.second)) {
        switch (style.read().text.GetIdent()) {
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
    const double aH = ceil(style.read().arrow.bigYExtent(isBidir(), fw, &segment_lines));
    XY twh = parsed_label.getTextWidthHeight();
    ind_off = twh.y + chart->boxVGapInside;
    if (sig && style.read().indicator.second) {
        twh.y += GetIndiactorSize().y + chart->boxVGapInside;
        twh.x = std::max(twh.x, GetIndiactorSize().x + 2*chart->boxVGapInside);
    } 
    twh.y = std::max(twh.y, style.read().text.getCharHeight(canvas));
    sy = chart->arcVGapAbove + aH;
    dy = ceil(sy + twh.y + chart->boxVGapInside*2 + 2*segment_lines[stext].LineWidth());

    const DoublePair sp_left_end = style.read().arrow.getBigWidthsForSpace(
            fw, isBidir(), fw ? MSC_ARROW_START : MSC_ARROW_END, 
            dy-sy, act_size.front(), segment_lines.front());
    const DoublePair sp_right_end = style.read().arrow.getBigWidthsForSpace(
            fw, isBidir(), fw ? MSC_ARROW_END : MSC_ARROW_START,
            dy-sy, act_size.back(), segment_lines.front());

    //add the distance requirement to the left and right side of src and dst
    distances.Insert(indexes.front(), DISTANCE_LEFT,  sp_left_end.first  *cos_slant);
    distances.Insert(indexes.front(), DISTANCE_RIGHT, sp_left_end.second *cos_slant);
    distances.Insert(indexes.back(),  DISTANCE_LEFT,  sp_right_end.first *cos_slant);
    distances.Insert(indexes.back(),  DISTANCE_RIGHT, sp_right_end.second*cos_slant);

    //add the distances to keep for verticals only on the outer side of src and dst
    vdist.Insert(indexes.front(), DISTANCE_LEFT,  sp_left_end.first  *cos_slant);
    vdist.Insert(indexes.back(),  DISTANCE_RIGHT, sp_right_end.second*cos_slant);

    //Collect iterators and distances into arrays
    margins.reserve(2+middle.size()); margins.clear();
    margins.push_back(sp_left_end);
    for (unsigned i=0; i<middle.size(); i++) 
        margins.push_back(style.read().arrow.getBigWidthsForSpace(
                             fw, isBidir(), MSC_ARROW_MIDDLE, 
                             dy-sy, act_size[i+1], segment_lines[i]));
    margins.push_back(sp_right_end);

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
    //If label is word wrap, we shall perhaps use something else below than twh.x
    const Contour tcov = parsed_label.Cover(0, twh.x, sy + segment_lines[stext].LineWidth() + chart->boxVGapInside);
    const EArrowType s_type = style.read().arrow.GetType(fw, isBidir(), WhichArrow(stext), false);
    const EArrowType d_type = style.read().arrow.GetType(fw, isBidir(), WhichArrow(dtext), true);
    _ASSERT(s_type!=MSC_ARROW_INVALID && d_type!=MSC_ARROW_INVALID);

    sm = style.read().arrow.getBigMargin(tcov, sy, dy, true, isBidir(), s_type,
                                  segment_lines[stext]);
    dm = style.read().arrow.getBigMargin(tcov, sy, dy, false, isBidir(), d_type,
                                  segment_lines[stext]);
    distances.Insert(indexes[stext], indexes[dtext], 
        (sm + parsed_label.getSpaceRequired() + dm + act_size[stext] +act_size[dtext])*cos_slant);
    //Add a new element to vdist
    vdist.InsertElementBottom(this);
    //Add activation status right away
    AddEntityLineWidths(vdist);
}


void ArcBigArrow::Layout(Canvas &canvas, AreaList *cover)
{
    if (!valid) return;
    yPos = 0;

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

    //reflow text if needed
    if (parsed_label.IsWordWrap()) {
        const double overflow = parsed_label.Reflow(canvas, dx_text - sx_text);
        OverflowWarning(overflow, "", sx<dx ? src : dst, sx<dx ? dst : src);
        //recalculate dy: reuse sy set in Width()
        dy = ceil(sy + parsed_label.getTextWidthHeight().y + 
                  chart->boxVGapInside*2 + 2*segment_lines[stext].LineWidth());
    } else {
        CountOverflow(dx_text - sx_text);
    }
    //reuse sy dy set in Width()
    centerline = (sy+dy)/2; //Note that we rotate around (sx, yPos+centerline)

    //text_cover = parsed_label.Cover(sx_text, dx_text, sy+style.read().line.LineWidth()/2 + chart->boxVGapInside, cx_text);
    area = style.read().arrow.BigContour(xPos, act_size, sy, dy, sx<dx, isBidir(), &segment_lines, outer_contours);
    area.arc = this;
    area_important = style.read().arrow.BigHeadContour(xPos, act_size, sy, dy, sx<dx, isBidir(), &segment_lines, chart->compressGap);
    area_important += parsed_label.Cover(sx_text, dx_text, sy+segment_lines[stext].LineWidth() + chart->boxVGapInside, cx_text);
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
    if (cover)
        *cover = GetCover4Compress(area);

    height = area.GetBoundingBox().y.till + chart->arcVGapBelow + style.read().shadow.offset.second;
    LayoutComments(canvas, cover);
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

void ArcBigArrow::PostPosProcess(Canvas &canvas)
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

void ArcBigArrow::Draw(Canvas &canvas, EDrawPassType pass)
{
    if (!valid) return;
    if (pass!=draw_pass) return;
    if (slant_angle)
        style.read().arrow.TransformCanvasForAngle(slant_angle, canvas, sx, yPos+centerline);
    style.read().arrow.BigDrawFromContour(outer_contours, &segment_lines, style.read().fill, style.read().shadow, canvas, slant_angle*M_PI/180.);
    parsed_label.Draw(canvas, sx_text, dx_text, sy+segment_lines[stext].LineWidth() + chart->boxVGapInside, cx_text);
    if (sig && style.read().indicator.second)
        DrawIndicator(XY(cx_text, sy+segment_lines[stext].LineWidth() + chart->boxVGapInside+ind_off), &canvas);
    if (slant_angle)
        style.read().arrow.UnTransformCanvas(canvas);
}

//////////////////////////////////////////////////////////////////////////////////////

VertXPos::VertXPos(Msc&m, const char *e1, const FileLineColRange &e1l,
                   const char *e2, const FileLineColRange &e2l, 
                   double off, EPosType p)
{
    //Sanitize p
    switch (p) {
    case POS_INVALID:
    case POS_AT:
    default:
        _ASSERT(0);
        p = POS_CENTER;
        break;
    case POS_LEFT_BY:
    case POS_LEFT_SIDE:
        _ASSERT(0);
        p = POS_THIRD_LEFT;
        break;
    case POS_RIGHT_BY:
    case POS_RIGHT_SIDE:
        _ASSERT(0);
        p = POS_THIRD_RIGHT;
        break;
    case POS_THIRD_LEFT:
    case POS_THIRD_RIGHT:
    case POS_CENTER:
        break;

    }
    valid = true;
    pos = p;
    entity1 = m.FindAllocEntity(e1, e1l);
    e1line = e1l;
    entity2 = m.FindAllocEntity(e2, e2l);
    e2line = e2l;
    offset = off;
}

VertXPos::VertXPos(Msc&m, const char *e1, const FileLineColRange &e1l, EPosType p, double off)
{
    valid = p != POS_CENTER;
    pos = p;
    offset = off;
    entity1 = m.FindAllocEntity(e1, e1l);
    e1line = e1l;
    entity2 = m.AllEntities.Find_by_Ptr(m.NoEntity);
    _ASSERT(entity2 != m.AllEntities.end());
    _ASSERT(p != POS_CENTER);
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
    const double a = std::min(xpos, chart.XCoord(entity2));
    const double b = std::max(xpos, chart.XCoord(entity2));
    switch (pos) {
    default: _ASSERT(0); //fallthrough
    case VertXPos::POS_AT: break;
    case VertXPos::POS_CENTER:      xpos = (  a +   b)/2; break;
    case VertXPos::POS_THIRD_LEFT:  xpos = (2*a +   b)/3; break;
    case VertXPos::POS_THIRD_RIGHT: xpos = (  a + 2*b)/3; break;
    case VertXPos::POS_LEFT_BY:     xpos -= width/2 + aw + gap; break;
    case VertXPos::POS_RIGHT_BY:    xpos += width/2 + aw + gap; break;
    case VertXPos::POS_LEFT_SIDE:   xpos -= width/2 + gap; break;
    case VertXPos::POS_RIGHT_SIDE:  xpos += width/2 + gap; break;
    };
    return xpos + offset;
}


ArcVerticalArrow::ArcVerticalArrow(ArcTypePlusDir t, const char *s, const char *d, Msc *msc) :
    ArcArrow(t.arc.type, MscProgress::VERTICAL, msc, msc->Contexts.back().styles["vertical"]), 
    src(s ? s : ""), dst(d ? d : ""), lost(t.arc.lost!=EArrowLost::NOT), 
    lost_pos(t.arc.lost_pos.CopyTo().start), shape(ARROW_OR_BOX), pos(*msc), prev_arc(NULL), ypos(2)
{
    //If we are defined via a backwards arrow, swap source and dst
    if (t.dir==MSC_DIR_LEFT)
        src.swap(dst);
    width_attr.Empty();
    valid = false; //without an x pos we are invalid
}

ArcVerticalArrow* ArcVerticalArrow::AddXpos(VertXPos *p)
{
    if (!p || !p->valid) return this;
    valid = true;
    //Note p->pos may be POS_INVALID if the user specified no 'at' clause.
    pos = *p;
    return this;
}

ArcArrow *ArcVerticalArrow::AddSegment(ArrowSegmentData /*data*/, const char * /*m*/, 
                                       const FileLineColRange &/*ml*/, 
                                       const FileLineColRange &l)
{
    if (!valid) return this; //display error only once
    chart->Error.Error(l.start, "Cannot add further segments to vertical arrow. Ignoring it.");
    valid = false;
    return this;
}

ArcArrow *ArcVerticalArrow::AddLostPos(VertXPos *pos, const FileLineColRange &l)
{
    _ASSERT(0);
    chart->Error.Error(l.start, "Internal error. Ignoring 'at' clause.");
    delete pos;
    return this;
}

void ArcVerticalArrow::SetVerticalShape(EVerticalShape sh)
{
    shape = sh;
    switch (shape) {
    default:
        _ASSERT(0); //fallthrough
    case ARROW_OR_BOX:
    case BOX:
        SetStyleWithText("vertical");
        break;
    case BRACE:
        SetStyleWithText("vertical_brace");
        break;
    case BRACKET:
        SetStyleWithText("vertical_bracket");
        break;
    case RANGE:
        SetStyleWithText("vertical_range");
        break;
    case POINTER:
        SetStyleWithText("vertical_pointer");
        break;
    }
    //re-do the automatic setting of "side"
    switch (pos.pos) {
    case VertXPos::POS_LEFT_BY:
    case VertXPos::POS_LEFT_SIDE:
    case VertXPos::POS_THIRD_RIGHT:
        if (!style.read().side.first) {
            style.write().side.first = true;
            style.write().side.second = ESide::RIGHT;
        }
        left = true;
        break;
    default:
        if (!style.read().side.first) {
            style.write().side.first = true;
            style.write().side.second = ESide::LEFT;
        }
        left = false;
        break;
    }
}


const StyleCoW *ArcVerticalArrow::GetRefinementStyle(EArcType t) const
{
    EArcType tt;
    if (shape==BOX) 
        switch (t) {
        case MSC_ARC_SOLID:
        case MSC_ARC_SOLID_BIDIR:
            tt = MSC_BOX_SOLID; break;
        case MSC_ARC_DOTTED:
        case MSC_ARC_DOTTED_BIDIR:
            tt = MSC_BOX_DOTTED; break;
        case MSC_ARC_DASHED:
        case MSC_ARC_DASHED_BIDIR:
            tt = MSC_BOX_DASHED; break;
        case MSC_ARC_DOUBLE:
        case MSC_ARC_DOUBLE_BIDIR:
            tt = MSC_BOX_DOUBLE; break;
        default: 
            tt = t;
    } else
        tt = t;

    switch(tt) {
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
    case MSC_BOX_SOLID:
        return &chart->Contexts.back().styles["vertical--"];
    case MSC_BOX_DASHED:
        return &chart->Contexts.back().styles["vertical++"];
    case MSC_BOX_DOTTED:
        return &chart->Contexts.back().styles["vertical.."];
    case MSC_BOX_DOUBLE:
        return &chart->Contexts.back().styles["vertical=="];
    default:
        return NULL;
    }
}

bool ArcVerticalArrow::AddAttribute(const Attribute &a)
{
    if (a.Is("offset")) {
        if (!a.EnsureNotClear(chart->Error, STYLE_ARC)) return true;
        if (!a.CheckType(MSC_ATTR_NUMBER, chart->Error)) return true;
        //By this time `pos` is filled in, so we can add offset there
        pos.offset += a.number;
        return true;
    }
    if (a.Is("text.width"))
        return width_attr.AddAttribute(a, chart, STYLE_ARC);
    return ArcArrow::AddAttribute(a);
}

void ArcVerticalArrow::AttributeNames(Csh &csh)
{
    ArcLabelled::AttributeNames(csh);
    defaultDesign.styles.GetStyle("vertical").read().AttributeNames(csh);
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"text.width", HINT_ATTR_NAME));
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
    if (CaseInsensitiveEqual(attr, "text.width") && WidthAttr::AttributeValues(attr, csh)) return true;
    if (defaultDesign.styles.GetStyle("vertical").read().AttributeValues(attr, csh)) return true;
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


ArcBase* ArcVerticalArrow::PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                            Numbering &number, Element **target, ArcBase *vertical_target)
{
    if (!valid) return NULL;
    if (lost) switch (shape) {
    case ARROW_OR_BOX: chart->Error.Error(lost_pos, "Box or block arrow verticals cannot indicate loss.", "Ignoring loss symbol."); break;
    case BOX:          chart->Error.Error(lost_pos, "Box verticals cannot indicate loss.", "Ignoring loss symbol."); break;
    case BRACE:        chart->Error.Error(lost_pos, "Brace verticals cannot indicate loss.", "Ignoring loss symbol."); break;
    case BRACKET:      chart->Error.Error(lost_pos, "Bracket verticals cannot indicate loss.", "Ignoring loss symbol."); break;
    case RANGE:        chart->Error.Error(lost_pos, "Range verticals cannot indicate loss.", "Ignoring loss symbol."); break;
    case POINTER: break;
    default: _ASSERT(0);
    }
    //Ignore hide: we show verticals even if they may be hidden
    if (src == MARKER_HERE_STR && dst == MARKER_HERE_STR) {
        //Try to see if we have a prior element to use
        if (vertical_target == NULL) {
            valid = false;
            chart->Error.Error(file_pos.start, "This vertical has no markers and no prior element to align to. Ignoring it.",
                "Either specify markers to set the top and bottom of the vertical or "
                "specify it directly after another element to align to.");
            return NULL;
        } else 
            prev_arc = vertical_target;
    }
    if (src != MARKER_HERE_STR) {
        std::map<string, Msc::MarkerData>::const_iterator i = chart->Markers.find(src);
        if (i == chart->Markers.end()) {
            chart->Error.Error(file_pos.start, "Cannot find marker '" + src + "'."
                               " Ignoring vertical.");
            valid=false;
            return NULL;
        }
    }

    if (dst != MARKER_HERE_STR) {
        std::map<string, Msc::MarkerData>::const_iterator i = chart->Markers.find(dst);
        if (i == chart->Markers.end()) {
            chart->Error.Error(file_pos.start, "Cannot find marker '" + dst + "'."
                               " Ignoring vertical.");
            valid=false;
            return NULL;
        }
    }

    bool error = chart->ErrorIfEntityGrouped(pos.entity1, file_pos.start);
    error |= chart->ErrorIfEntityGrouped(pos.entity2, file_pos.start);
    if (error) return NULL;

    //Add numbering, if needed
    ArcLabelled::PostParseProcess(canvas, hide, left, right, number, target, vertical_target);

    left = chart->EntityMinByPos(left, pos.entity1);
    right = chart->EntityMaxByPos(right, pos.entity1);
    left = chart->EntityMinByPos(left, pos.entity2);
    right = chart->EntityMaxByPos(right, pos.entity2);

    if (hide) 
        return NULL;

    //Now change entities in vertxPos to point to ActiveEntities
    //If the pos is invalid, it marks a missing 'at' clause. 
    //In this case we calculate the pos later in Width().
    if (pos.pos != VertXPos::POS_INVALID) {
        pos.entity1 = chart->ActiveEntities.Find_by_Ptr(*chart->FindActiveParentEntity(pos.entity1));
        pos.entity2 = chart->ActiveEntities.Find_by_Ptr(*chart->FindActiveParentEntity(pos.entity2));
        _ASSERT(pos.entity1 != chart->ActiveEntities.end());
        _ASSERT(pos.entity2 != chart->ActiveEntities.end());
        if (pos.pos == VertXPos::POS_CENTER && pos.entity1 == pos.entity2)
            pos.pos = VertXPos::POS_AT;
    }

    //If we do a RANGE, remove arrowheads for box symbols
    if (shape == RANGE) 
        if (type==MSC_BOX_DASHED || type == MSC_BOX_DOTTED || type==MSC_BOX_DOUBLE || type == MSC_BOX_SOLID) 
            style.write().arrow.startType.second = 
                style.write().arrow.endType.second = MSC_ARROW_NONE;

    //If we do a block arrow copy line attribute to arrow
    if (shape == ARROW_OR_BOX)
        style.write().arrow.line = style.read().line;

    //Re-adjust gradients so that they look what the user wanted 
    //even after rotation
    static const EGradientType readfrom_left_gardient[] = {
        GRADIENT_INVALID, GRADIENT_NONE, GRADIENT_OUT, GRADIENT_IN,
        GRADIENT_RIGHT, GRADIENT_LEFT, GRADIENT_DOWN, GRADIENT_UP,
        GRADIENT_LEFT};
    static const EGradientType readfrom_right_gardient[] = {
        GRADIENT_INVALID, GRADIENT_NONE, GRADIENT_OUT, GRADIENT_IN,
        GRADIENT_LEFT, GRADIENT_RIGHT, GRADIENT_UP, GRADIENT_DOWN,
        GRADIENT_RIGHT};

    if (style.read().fill.gradient.first) {
        if (style.read().side.second == ESide::LEFT)
            style.write().fill.gradient.second = readfrom_left_gardient[style.read().fill.gradient.second];
        else
            style.write().fill.gradient.second = readfrom_right_gardient[style.read().fill.gradient.second];
    }
    return this;
}

void ArcVerticalArrow::Width(Canvas &canvas, EntityDistanceMap &distances, DistanceMapVertical &vdist)
{
    const double text_distance = 0; //chart->hscaleAutoXGap
    if (!valid) return;
    //reflow if needed
    if (style.read().side.second==ESide::END && parsed_label.IsWordWrap()) {
        //This works only for side==END really
        //for rotated text we cannot go back and increase space req...
        const double w = width_attr.GetWidth(canvas, style.read().text);
        if (w>0) {
            const double overflow = parsed_label.Reflow(canvas, w);
            OverflowWarning(overflow, "Use the 'text.width' attribute to increase text width.");
        } else 
            chart->Error.Warning(file_pos.start, "Word wrapping is on, but no meaningful width is specified.", "Ignoring word wrapping. Try setting 'text.width'.");
    }
    //The offset is ignored during the process of setting space requirements
    const XY twh = parsed_label.getTextWidthHeight();
    const double lw = style.read().line.LineWidth();
    radius = std::max(0., style.read().line.radius.second);
    if (style.read().side.second == ESide::END)
        width = twh.x;
    else 
        width = twh.y;
    switch (shape) {
    default:
        _ASSERT(0);
    case ARROW_OR_BOX:
    case BOX:
        //Here body width is the width of the box or arrow body
        if (width==0)
            width = style.read().text.getCharHeight(canvas);
        width += 2*lw + 2*chart->boxVGapInside;
        width += fmod_negative_safe(width, 2.); //width is even integer now: the distance from outer edge to outer edge
        //ext width is the tip of the arrowhead, if any
        ext_width = style.read().arrow.bigYExtent(isBidir(), false); 
        break;
    case RANGE:  {
        //Here body with is all the width of the range
        double w = lw/2;
        //getWidthHeight().y is _half_ the height of the arrowhead 
        //(the height above/below the centerline)
        w = std::max(w, style.read().arrow.getWidthHeight(isBidir(), MSC_ARROW_END).y);
        w = std::max(w, style.read().arrow.getWidthHeight(isBidir(), MSC_ARROW_START).y);
        width = std::max(width + text_distance + lw/2, //space between text and range
                         style.read().line.radius.second);
        width = std::max(width, w);
        _ASSERT(style.read().line.IsComplete());
        //we set vline_off here so that it becomes easier to calculate it later
        vline_off = std::max(w, style.read().line.radius.second);
        width += vline_off;
        //no ext width
        ext_width = radius;
        }
        break;
    case BRACKET:
    case POINTER:
        lsym_size.y = lsym_size.x =
            ArrowHead::baseArrowWidth * LSYM_SIZE_ADJ * ArrowHead::arrowSizePercentage[style.read().lsym_size.second]/100.;
        //Here body with is all the width of the brace/bracket, plus text
        if (width)
            width += text_distance; //space between text and brace
        _ASSERT(style.read().line.IsComplete());
        if (lost) {
            width = std::max(width, std::max(lw/2, lsym_size.x));
            //This is the distance from the edge of the object to the midline
            //of the horizontal arrow.
            vline_off = std::max(std::max(lsym_size.x, radius), lw/2);
            width += vline_off;
        } else {
            width += radius + lw;
            vline_off = radius + lw/2;
        }
        ext_width = radius;
        break;
    case BRACE:
        //Here body with is all the width of the brace/bracket, plus text
        if (width)
            width += text_distance; //space between text and brace
        _ASSERT(style.read().line.IsComplete());
        vline_off = radius + lw;
        width += vline_off + radius;
        ext_width = radius;
        break;
    }

    const bool both_none = (src==MARKER_HERE_STR && dst == MARKER_HERE_STR);
    const auto si = both_none ? vdist.GetIterator(prev_arc, true) :
                      src==MARKER_HERE_STR ? vdist.GetIteratorEnd() :
                           vdist.GetIterator(src);
    const auto di = both_none ? vdist.GetIterator(prev_arc, false) :
                      dst==MARKER_HERE_STR ? vdist.GetIteratorEnd() :
                           vdist.GetIterator(dst);
    //If pos.pos is invalid, we select a location.
    if (pos.pos==VertXPos::POS_INVALID) {
        const DistanceMapVerticalElement elem = vdist.Get(si, di);
        const bool left = style.read().side.second == ESide::RIGHT;
        const unsigned i = left ? elem.QueryLeftEntity() : elem.QueryRightEntity();
        pos.pos = left ? VertXPos::POS_LEFT_SIDE
            : VertXPos::POS_RIGHT_SIDE;
        if (i==DistanceMapVerticalElement::NO_LEFT_ENTITY)
            pos.entity1 = chart->ActiveEntities.Find_by_Ptr(chart->LSide);
        else if (i==DistanceMapVerticalElement::NO_RIGHT_ENTITY)
            pos.entity1 = chart->ActiveEntities.Find_by_Ptr(chart->RSide);
        else
            pos.entity1 = chart->ActiveEntities.Find_by_Index(i);
        pos.offset = 0;
    }
    _ASSERT(pos.entity1 != chart->ActiveEntities.end());

    const unsigned index = (*pos.entity1)->index;
    double displace;
    switch (pos.pos) {
    case VertXPos::POS_LEFT_BY:
    case VertXPos::POS_LEFT_SIDE:
        displace = -vdist.Get(si, di).Query(index, DISTANCE_LEFT);
        break;
    case VertXPos::POS_RIGHT_BY:
    case VertXPos::POS_RIGHT_SIDE:
        displace = vdist.Get(si, di).Query(index, DISTANCE_RIGHT);
        break;
    default:
        displace = 0;
        break;
    };

    //If we are a pointer, make room for the arrowheads
    if (shape == POINTER) {
        const double ah_width = std::max(
            style.read().arrow.getWidths(true, isBidir(), MSC_ARROW_START, 
                  style.read().line).second,
            style.read().arrow.getWidths(true, isBidir(), MSC_ARROW_END, 
                  style.read().line).first);
        displace = std::max(fabs(displace), ah_width); //positive
        //We do not move pos.offset here, instead, we enlarge the vertical's width
        width += displace;
        vline_off += displace;
        //Finally, move so that arrowhead touches the entity line
        //(VertXPos::CalculatePos() leaves a small gap)
        switch (pos.pos) {
        case VertXPos::POS_LEFT_BY:
        case VertXPos::POS_LEFT_SIDE:
            pos.offset += chart->hscaleAutoXGap;
            break;
        case VertXPos::POS_RIGHT_BY:
        case VertXPos::POS_RIGHT_SIDE:
            pos.offset -= chart->hscaleAutoXGap;
            break;
        default:
            break;
        }
    } else //for others make room for what is already there
        pos.offset += displace;

    //Make vline_off signed
    if (!left)
        vline_off *= -1;

    //No extra space requirement -> exit here
    if (!style.read().makeroom.second) return;

    switch (pos.pos) {
    default:
        _ASSERT(0); //fallthrough
    case VertXPos::POS_AT:
        distances.Insert(index, DISTANCE_LEFT, (width+ext_width)/2 - pos.offset);
        distances.Insert(index, DISTANCE_RIGHT, (width+ext_width)/2 + pos.offset);
        vdist.Insert(index, DISTANCE_LEFT, (width+ext_width)/2 - pos.offset, si, di);
        vdist.Insert(index, DISTANCE_RIGHT, (width+ext_width)/2 + pos.offset, si, di);
        break;
    case VertXPos::POS_THIRD_LEFT:
    case VertXPos::POS_THIRD_RIGHT:
        _ASSERT(0); //fallthrough, these should not appear here, used only for loss
    case VertXPos::POS_CENTER:
        distances.Insert(index, (*pos.entity2)->index, width+ext_width + pos.offset);
        break;
    case VertXPos::POS_LEFT_BY:
        distances.Insert(index, DISTANCE_LEFT, width + ext_width - pos.offset);
        vdist.Insert(index, DISTANCE_LEFT, width + ext_width - pos.offset, si, di);
        break;
    case VertXPos::POS_RIGHT_BY:
        distances.Insert(index, DISTANCE_RIGHT, width + ext_width + pos.offset);
        vdist.Insert(index, DISTANCE_RIGHT, width + ext_width + pos.offset, si, di);
        break;

    case VertXPos::POS_LEFT_SIDE:
        distances.Insert(index, DISTANCE_LEFT, width - pos.offset);
        vdist.Insert(index, DISTANCE_LEFT, width - pos.offset, di, si);
        break;
    case VertXPos::POS_RIGHT_SIDE:
        distances.Insert(index, DISTANCE_RIGHT, width + pos.offset);
        vdist.Insert(index, DISTANCE_RIGHT, width + pos.offset, si, di);
        break;
    };
}

//Height and parameters of this can only be calculated in PostPosProcess, when all other edges are set
//So here we do nothing. yPos is not used for this
void ArcVerticalArrow::Layout(Canvas &/*canvas*/, AreaList * /*cover*/)
{
    //We will not have notes or comments, so no need to call CommentHeight()
    _ASSERT(comments.size()==0);
    //Set height to zero not to disturb layouting and automatic pagination
    height = 0;
}

void ArcVerticalArrow::ShiftBy(double y)
{
    yPos += y;
}

/** Creates a quarter of a circle's belt 
 * @param [in] x The center
 * @param [in] y The center
 * @param [in] r The radius of the midline of the belt
 * @param [in] lw The width of the belt
 * @param [in] q Which quarter: 0:lower-right, 1:lower-left, 2: upper-left, 3: upper-right
 * @returns a contour.*/
Contour BeltQuarter(double x, double y, double r, double lw, unsigned q)
{
    _ASSERT(q<=3);
    if (r<=0 || lw<=0) return Contour();
    std::vector<Edge> edges;
    Edge::GenerateEllipse(edges, XY(x, y), r+lw/2, 0, 0, 90*q, 90*(q+1));
    const size_t end1 = edges.size();
    edges.emplace_back();
    Edge::GenerateEllipse(edges, XY(x, y), r-lw/2, 0, 0, 90*(q+1), 90*q, false);
    edges.emplace_back();
    edges[end1] = Edge(edges[end1-1].GetEnd(), edges[end1+1].GetStart());
    edges.back() = Edge(edges[edges.size()-2].GetEnd(), edges[0].GetStart());
    return edges;
}

void ArcVerticalArrow::PlaceWithMarkers(Canvas &/*canvas*/)
{
    if (!valid) return;
    if (src!=MARKER_HERE_STR || dst!=MARKER_HERE_STR) {
        //Here we are sure markers are OK
        //all below are integers. yPos is such, in general. "Markers" are yPos of the markers
        ypos[0] = src == MARKER_HERE_STR ? yPos : chart->Markers.find(src)->second.y;
        ypos[1] = dst == MARKER_HERE_STR ? yPos : chart->Markers.find(dst)->second.y;

    } else if (prev_arc) {
        ypos[0] = prev_arc->GetYExtent().from;
        ypos[1] = prev_arc->GetYExtent().till;
    } else {
        _ASSERT(0); // this should have been caught in PostParseProcess()
        valid = false;
        return;
    }

    if (ypos[0] == ypos[1]) {
        chart->Error.Error(file_pos.start, "This vertical is of zero size. Ignoring it.");
        valid = false;
        return;
    }
    forward = ypos[0] < ypos[1];
    if (!forward)
        swap(ypos[0], ypos[1]); //ypos[0] now < ypos[1]

    const double lw = style.read().line.LineWidth();
    const double lw2 = lw/2;
    XY twh = parsed_label.getTextWidthHeight();
    const Contour text_cover = parsed_label.Cover(0, twh.x, lw);
    double sm, dm; //up/down margins
    if (style.read().side.second== ESide::END) {
        dm = sm = lw;
    } else switch (shape) {
        case ARROW_OR_BOX:
        case BOX:
            sm = style.read().arrow.getBigMargin(text_cover, 0, twh.y+2*lw, style.read().side.second == ESide::LEFT,
                                                 isBidir(), style.read().arrow.startType.second, style.read().line);
            dm = style.read().arrow.getBigMargin(text_cover, 0, twh.y+2*lw, style.read().side.second != ESide::LEFT,
                                                 isBidir(), style.read().arrow.endType.second, style.read().line);
            break;
        case RANGE:
            sm = lw + std::max(lw/2, style.read().arrow.getWidthHeight(isBidir(), MSC_ARROW_START).x);
            dm = lw + std::max(lw/2, style.read().arrow.getWidthHeight(isBidir(), MSC_ARROW_END).x);
            break;
        case POINTER:
            sm = 0;
            dm = lost ? lsym_size.y + (style.read().line.corner.second == CORNER_NONE ?
                                       0 : radius) : 0;
            break;
        default:
            dm = sm = 0;
    }
    //calculate y coordinates of text boundaries
    switch (style.read().side.second) {
    case ESide::LEFT:
        s_text = ypos[0] + (forward ? sm : dm);
        d_text = ypos[1] - (forward ? dm : sm);
        break;
    case ESide::RIGHT:
        s_text = ypos[1] - (forward ? sm : dm);
        d_text = ypos[0] + (forward ? dm : sm);
        break;
    case ESide::END:
        t_text = (ypos[0] + ypos[1] - twh.y)/2;
        break;
    default:
        _ASSERT(0);
        break;
    }
    //Warn if label does not fit
    if ((style.read().side.second==ESide::END && ypos[1]-ypos[0] < twh.y) || 
        (style.read().side.second!=ESide::END && ypos[1]-ypos[0] < fabs(s_text-d_text)))
        chart->Error.Warning(file_pos.start, "Too little vertical space for label - may look bad.");

    xpos = pos.CalculatePos(*chart, width, ext_width/2);
    //xpos is now the middle of the vertical object.
    //This is good for arrow/box, but not for others, since
    //for bracket/brace/pointer/range it should be the main vertical line 
    //We will adjust xpos below.
    //Plus we calculate the x coordinates of the text
    switch (shape) {
    default:
        _ASSERT(0); //fallthrough
    case ARROW_OR_BOX:
    case BOX:
        //adjust xpos and width
        width -= lw; //not necessarily integer, the distance from midline to midline
        xpos = floor(xpos + 0.5); //xpos is integer now: the centerline of arrow if arrow
        switch (style.read().side.second) {
        case ESide::LEFT:
            t_text = xpos + twh.y/2; break;
        case ESide::RIGHT:
            t_text = xpos - twh.y/2; break;
        case ESide::END:
            s_text = xpos - twh.x/2;
            d_text = xpos + twh.x/2;
            break;
        default:
            _ASSERT(0);
            break;
        }
        break;
    case POINTER:
    case BRACKET:
    case BRACE:
    case RANGE:
        switch (style.read().side.second) {
        case ESide::LEFT:
            if (left) t_text = xpos - width/2 + twh.y;
            else t_text = xpos + width/2;
            break;
        case ESide::RIGHT:
            if (left) t_text = xpos - width/2;
            else t_text = xpos + width/2 - twh.y;
            break;
        case ESide::END:
            if (left) {
                s_text = xpos - width/2;
                d_text = s_text + twh.x;
            } else {
                d_text = xpos + width/2;
                s_text = d_text - twh.x;
            }
            break;
        default:
            _ASSERT(0);
            break;
        }
        xpos += (left ? 1 : -1) * (width/2) - vline_off;
        break;
    }
    //'xpos' is now the middle of the arrow/box;
    //for bracket/brace/pointer/range it is the main vertical line 
    //All of 's_text', 'd_text', 't_text' is filled
    
    //Copy text cover to 'area_important', re-used below
    area_important = parsed_label.Cover(s_text, d_text, t_text, style.read().side.second); 

    //Now set 'area'
    switch (shape) {
    default:
        _ASSERT(0); //fallthrough
    case ARROW_OR_BOX:
    case BOX:
        {
            const double ss = style.read().arrow.getBigWidthsForSpace(
                                  forward, isBidir(), forward ? MSC_ARROW_START : MSC_ARROW_END,
                                  twh.y+2*lw, 0, style.read().line).first;
            const double ds = style.read().arrow.getBigWidthsForSpace(
                                  forward, isBidir(), forward ? MSC_ARROW_END: MSC_ARROW_START,
                                  twh.y+2*lw, 0, style.read().line).second;
            if (ss+ds+chart->compressGap > ypos[1]-ypos[0]) {
                chart->Error.Warning(file_pos.start, "Size of vertical element is too small to draw arrowheads. Ignoring it.");
                valid = false;
                return;
            }

	        //Generate area
            std::vector<double> act_size(2, 0);
            //use inverse of forward, swapXY will do the job
            area = style.read().arrow.BigContour(ypos, act_size, xpos-width/2, xpos+width/2, 
                                                 forward, isBidir(), NULL, outer_contours);
            area.SwapXY();
            for (auto i = outer_contours.begin(); i!=outer_contours.end(); i++)
                i->SwapXY();
            break; //ARROW_OR_BOX
        }
    case RANGE: 
        {
            area = area_important;
            area += Block(xpos-lw2, xpos+lw2, ypos[0], ypos[1]);
            area += Block(xpos-radius-lw2, xpos+radius+lw2, ypos[0], ypos[0]+lw);
            area += Block(xpos-radius-lw2, xpos+radius+lw2, ypos[1], ypos[1]-lw);
            //Now add arrowhead contours
            const XY cs(xpos, ypos[0]+lw/2);
            const XY cd(xpos, ypos[1]-lw/2);
            area += style.read().arrow.Cover(cs, 0, forward, isBidir(), 
                forward ? MSC_ARROW_START : MSC_ARROW_END, style.read().line, style.read().line
                ).RotateAround(cs, 90);
            area += style.read().arrow.Cover(cd, 0, forward, isBidir(), 
                forward ? MSC_ARROW_END : MSC_ARROW_START, style.read().line, style.read().line
                ).RotateAround(cd, 90);
        }
        break;
    case POINTER:
    case BRACKET: {
        area = area_important;
        const Block rect(xpos, xpos+2*vline_off, ypos[0]+lw2, ypos[1]-lw2);
        const Block total(xpos-vline_off, xpos+vline_off, ypos[0], ypos[1]);
        clip_line = total;
        if (shape==POINTER) {
            const XY cs(xpos+vline_off, ypos[0]);
            const XY cd(xpos+vline_off, ypos[1]);
            const Block tot_up(total.x, Range(total.y.from, total.y.MidPoint()));
            const Block tot_dn(total.x, Range(total.y.MidPoint(), total.y.till));
            //Adjust clip for arrowheads
            clip_line *= style.read().arrow.ClipForLine(cs, 0, forward != left,
                           isBidir(), forward ? MSC_ARROW_START : MSC_ARROW_END, tot_up,
                           style.read().line, style.read().line)
                         +tot_dn;
            clip_line *= style.read().arrow.ClipForLine(cd, 0, forward == left,
                            isBidir(), forward ? MSC_ARROW_END : MSC_ARROW_START, tot_dn,
                            style.read().line, style.read().line)
                            +tot_up;
            //Add arrowheads to area
            area += style.read().arrow.Cover(cs, 0, forward != left, 
                isBidir(), forward ? MSC_ARROW_START : MSC_ARROW_END,
                style.read().line, style.read().line);
            area += style.read().arrow.Cover(cd, 0, forward == left,
                isBidir(), forward ? MSC_ARROW_END : MSC_ARROW_START,
                style.read().line, style.read().line);
        }
        area += clip_line * (style.read().line.CreateRectangle_OuterEdge(rect) - 
                             style.read().line.CreateRectangle_InnerEdge(rect));
        if (shape==POINTER && lost) 
            area += Block(xpos - lsym_size.x/2, xpos + lsym_size.x/2,
                ypos[1] - lsym_size.y/2, ypos[1] +  lsym_size.y/2);
        }
        break;
    case BRACE: 
        area = area_important;
        //Limit radius to what fits the height
        //(We can only do so here, for all other width etc values
        //were needed to be consistently calculated with the original radius)
        radius = std::max(0., std::min(radius, (ypos[1]-ypos[0]-2*lw)/4));
        const double ymid = (ypos[0]+ypos[1])/2;
        const double xoff = left ? radius : -radius;
        const double lw2off = left ? lw2 : -lw2;
        area += Block(xpos+xoff, xpos+xoff+lw2off, ypos[0], ypos[0]+lw);
        area += BeltQuarter(xpos+xoff, ypos[0]+radius+lw/2, radius, lw, left ? 2 : 3);
        area += Block(xpos-lw/2, xpos+lw/2, ypos[0]+radius+lw/2, ymid-radius);
        area += BeltQuarter(xpos-xoff, ymid-radius, radius, lw, left ? 0 : 1);
        area += BeltQuarter(xpos-xoff, ymid+radius, radius, lw, left ? 3 : 2);
        area += Block(xpos-lw/2, xpos+lw/2, ymid+radius, ypos[1]-radius-lw/2);
        area += BeltQuarter(xpos+xoff, ypos[1]-radius-lw/2, radius, lw, left ? 1 : 0);
        area += Block(xpos+xoff, xpos+xoff+lw2off, ypos[1], ypos[1]-lw);
        area += Block(xpos-xoff, xpos-xoff-lw2off, ymid-lw2, ymid+lw2);
    }

    area.arc = this;
    area_to_note2 = Block(xpos, xpos, ypos[0], ypos[1]).Expand(0.5);
    chart->NoteBlockers.Append(this);
    //set yPos and height so that GetYExtent returns the right range
    //to determine if this element needs to be drawn on a particular page
    yPos = ypos[0];
    height = ypos[1]-ypos[0];
}

void ArcVerticalArrow::PostPosProcess(Canvas &canvas)
{
    if (!valid) return;
    //Expand area and add us to chart's all covers list
    ArcArrow::PostPosProcess(canvas);
}

/** Draws the line and arrowhead of a brace, pointer or lost_pointer */
void ArcVerticalArrow::DrawBraceLostPointer(Canvas &canvas, const LineAttr &line, 
                                            const ArrowHead &arrow)
{
    const double lw = line.LineWidth();
    const Block rect(xpos, xpos+2*vline_off, ypos[0]+lw/2, ypos[1]-lw/2);

    canvas.Clip(clip_line);
    canvas.Line(rect, line); //This will have rounde edges, if so set
    canvas.UnClip();
    if (shape==POINTER) {
        const XY cs(xpos+vline_off, ypos[0]);
        const XY cd(xpos+vline_off, ypos[1]);
        arrow.Draw(cs, 0, forward != left,
            isBidir(), forward ? MSC_ARROW_START : MSC_ARROW_END,
            style.read().line, style.read().line, &canvas);
        arrow.Draw(cd, 0, forward == left,
            isBidir(), forward ? MSC_ARROW_END : MSC_ARROW_START,
            style.read().line, style.read().line, &canvas);
    }
}

void ArcVerticalArrow::Draw(Canvas &canvas, EDrawPassType pass)
{
    if (!valid) return;
    if (pass!=draw_pass) return;

    const double lw = style.read().line.LineWidth();
    //Draw shape 
    switch (shape) {
    case ARROW_OR_BOX:
    case BOX:
        style.read().arrow.BigDrawFromContour(outer_contours, NULL, style.read().fill, style.read().shadow, canvas);
        break;
    case RANGE: {
        //small tips
        LineAttr solid = style.read().line;
        solid.type.second = LINE_SOLID;
        canvas.Line(XY(xpos-radius-lw/2, ypos[0]+lw/2), XY(xpos+radius+lw/2, ypos[0]+lw/2),
                    solid);
        canvas.Line(XY(xpos-radius-lw/2, ypos[1]-lw/2), XY(xpos+radius+lw/2, ypos[1]-lw/2),
                    solid);
        
        //now the line. Clip first.
        const XY cs(xpos, ypos[0]+lw/2);
        const XY cd(xpos, ypos[1]-lw/2);
        Contour clip_area = style.read().arrow.ClipForLine(cs, 0, forward, isBidir(), 
            forward ? MSC_ARROW_START : MSC_ARROW_END, 
            Block(xpos, xpos + ypos[1]-ypos[0], ypos[0]-vline_off, ypos[0]+vline_off),
            style.read().line, style.read().line
            ).RotateAround(cs, 90);
        Contour ca = style.read().arrow.ClipForLine(cd, 0, forward, isBidir(), 
            forward ? MSC_ARROW_END : MSC_ARROW_START,
            Block(xpos-(ypos[1]-ypos[0]), xpos, ypos[1]-vline_off, ypos[1]+vline_off),
            style.read().line, style.read().line
            ).RotateAround(cd, 90);
        clip_area *= ca;
        canvas.Clip(clip_area);
        canvas.Line(XY(xpos, ypos[0]), XY(xpos, ypos[1]),
            style.read().line);
        canvas.UnClip();           
        //Now arrowheads
        style.read().arrow.TransformCanvasForAngle(90, canvas, cs.x, cs.y);
        style.read().arrow.Draw(cs, 0, forward, isBidir(), 
            forward ? MSC_ARROW_START : MSC_ARROW_END, 
            style.read().line, style.read().line, &canvas);
        style.read().arrow.UnTransformCanvas(canvas);
        style.read().arrow.TransformCanvasForAngle(90, canvas, cd.x, cd.y);
        style.read().arrow.Draw(cd, 0, forward, isBidir(), 
            forward ? MSC_ARROW_END : MSC_ARROW_START,
            style.read().line, style.read().line, &canvas);
        style.read().arrow.UnTransformCanvas(canvas);
    }
        break;
    case BRACKET:
        DrawBraceLostPointer(canvas, style.read().line, style.read().arrow);
        break;
    case POINTER:
        if (lost) {
            Contour clip_up = Block(xpos-vline_off/2, xpos+vline_off/2, ypos[0], ypos[1]-lw);
            clip_up += Block(xpos+vline_off/2, xpos+3*vline_off/2, ypos[0]-100, (ypos[0]+ypos[1])/2);
            canvas.Clip(clip_up);
            //temorarily increase size to shift corner out of the clip range
            ypos[1] += radius;
            DrawBraceLostPointer(canvas, style.read().line, style.read().arrow);
            ypos[1] -= radius;
            canvas.UnClip();
            Contour clip_dn = Block(xpos-vline_off/2, xpos+vline_off/2, ypos[1]-lw, ypos[1]);
            clip_dn += Block(xpos+vline_off/2, xpos+3*vline_off/2, (ypos[0]+ypos[1])/2, ypos[1]+100);
            canvas.Clip(clip_dn);
            LineAttr line = style.read().line;
            line += style.read().lost_line;
            line.corner.second = CORNER_NONE;
            ArrowHead arrow = style.read().arrow;
            arrow += style.read().lost_arrow;
            DrawBraceLostPointer(canvas, line, arrow);
            canvas.UnClip();
            DrawLSym(canvas, XY(xpos, ypos[1]), lsym_size);
       } else //Not lost 
            DrawBraceLostPointer(canvas, style.read().line, style.read().arrow);
        break;
    case BRACE:
        const double ymid = (ypos[0]+ypos[1])/2;
        const double xoff = left ? radius : -radius;
        const double lw2off = left ? lw/2 : -lw/2;
        std::vector<Edge> edges;
        edges.reserve(18);
        edges.emplace_back(XY(xpos+xoff+lw2off, ypos[0]+lw/2), XY(xpos+xoff, ypos[0]+lw/2));
        Edge::GenerateEllipse(edges, XY(xpos+xoff, ypos[0]+radius+lw/2), radius, 0, 0, left ? 270 : 270, left ? 180 : 360, !left);
        edges.emplace_back(XY(xpos, ypos[0]+radius+lw/2), XY(xpos, ymid-radius));
        Edge::GenerateEllipse(edges, XY(xpos-xoff, ymid-radius), radius, 0, 0, left ? 0 : 180, left ? 90 : 90, left);
        Edge::GenerateEllipse(edges, XY(xpos-xoff, ymid+radius), radius, 0, 0, left ? 270 : 270, left ? 360 : 180, left);
        edges.emplace_back(XY(xpos, ymid+radius), XY(xpos, ypos[1]-radius-lw/2));
        Edge::GenerateEllipse(edges, XY(xpos+xoff, ypos[1]-radius-lw/2), radius, 0, 0, left ? 180 : 0, left ? 90 : 90, !left);
        edges.emplace_back(XY(xpos+xoff, ypos[1]-lw/2), XY(xpos+xoff+lw2off, ypos[1]-lw/2));
        edges.emplace_back(XY(xpos+xoff+lw2off, ypos[1]-lw/2), XY(xpos+xoff+lw2off, ypos[0]+lw/2), false);
        Contour brace;
        brace.assign_dont_check(std::move(edges));
        canvas.Line(brace, style.read().line);
    };
    //draw text
    parsed_label.Draw(canvas, s_text, d_text, t_text, style.read().side.second);
}

//////////////////////////////////////////////////////////////////////////////////////

template<> const char EnumEncapsulator<EBoxCollapseType>::names[][ENUM_STRING_LEN] =
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


ArcBox::ArcBox(EArcType t, const char *s, const FileLineColRange &sl,
                         const char *d, const FileLineColRange &dl, Msc *msc) :
    ArcLabelled(t, MscProgress::BOX, msc, msc->Contexts.back().styles["emptybox"]),
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
    ArcBase(MSC_BOX_SOLID, MscProgress::BOX_SERIES, first->chart), 
    series(true), drawing_variant(1)
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
    SetStyleWithText("box"); //change our default style from "emptybox" to box
    keep_together = false;
    return this;
}

void ArcBox::AddAttributeList(AttributeList *l) 
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
    //Now the 'collapsed' member is final, but no (other) attribute has been set.

    //If collapsed, use appropriate style instead of "box", which was set 
    //in AddArcList()
    if (collapsed==BOX_COLLAPSE_COLLAPSE) 
        SetStyleWithText("box_collapsed");
    else if (collapsed==BOX_COLLAPSE_BLOCKARROW) 
        SetStyleWithText("box_collapsed_arrow");
    
    //Now set the attributes
    ArcLabelled::AddAttributeList(l);
}

bool ArcBox::AddAttribute(const Attribute &a)
{
    if (a.type == MSC_ATTR_STYLE) return ArcLabelled::AddAttribute(a);
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
    defaultDesign.styles.GetStyle("box").read().AttributeNames(csh);
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"collapsed", HINT_ATTR_NAME));
}

bool CshHintGraphicCallbackForBoxCollapsed(Canvas *canvas, CshHintGraphicParam p, Csh &csh)
{
    if (!canvas) return false;
    EBoxCollapseType t = (EBoxCollapseType)(int)p;
    switch (t) {
    case BOX_COLLAPSE_COLLAPSE:
        return CshHintGraphicCallbackForYesNo(canvas, CshHintGraphicParam(1), csh);
    case BOX_COLLAPSE_EXPAND:
        return CshHintGraphicCallbackForYesNo(canvas, CshHintGraphicParam(0), csh);
    case BOX_COLLAPSE_BLOCKARROW:
        return CshHintGraphicCallbackForSide(canvas, CshHintGraphicParam(int(ESide::LEFT)), csh);
    default:
        _ASSERT(0);
    } 
    return true;
}

bool ArcBox::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr, "collapsed")) {
        csh.AddToHints(EnumEncapsulator<EBoxCollapseType>::names, csh.HintPrefix(COLOR_ATTRVALUE), 
                       HINT_ATTR_VALUE, CshHintGraphicCallbackForBoxCollapsed); 
        return true;
    }
    if (defaultDesign.styles.GetStyle("box").read().AttributeValues(attr, csh)) return true;
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
        StyleCoW s = (*series.begin())->style;
        //Override with the line type specified (if any)
        if (f->type != MSC_BOX_UNDETERMINED_FOLLOW)
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

EDirType ArcBoxSeries::GetToucedEntities(class EntityList &el) const
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

ArcBase* ArcBox::PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                  Numbering &number, Element **target, ArcBase *vertical_target)
{
    ArcBase *ret = this;
    if (collapsed == BOX_COLLAPSE_BLOCKARROW) {
        EntityList el(false);
        //replace us with a block arrow - here we are sure to be alone in the series
        el.Append(*src);
        el.Append(*dst);
        EDirType dir = chart->GetTouchedEntitiesArcList(content, el);
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
        ret = aba->PostParseProcess(canvas, hide, left, right, number, target, vertical_target);
        if (old_target != *target && ret == NULL)
            *target = DELETE_NOTE;
    } else 
        ret = ArcLabelled::PostParseProcess(canvas, hide, left, right, number, target, vertical_target);
    //Add numbering, if needed
    EIterator left_content = chart->AllEntities.Find_by_Name(NONE_ENT_STR);
    EIterator right_content = left_content;
    if (content.size()) {
        //If we have attached a number to the label of a box, set flag on number,
        //so that if we add levels inside the content of the box (before displaying any number)
        //we continue from present value
        if (style.read().numbering.second && label.length()!=0)
            number.decrementOnAddingLevels = true;
        const bool hide_i = hide || (collapsed!=BOX_COLLAPSE_EXPAND);
        chart->PostParseProcessArcList(canvas, hide_i, content, false, 
                                       left_content, right_content, number, target);
        //If we are collapsed (but not to blockarrow), but not hidden and "indicator" attribute is set, 
        //then add an indicator to the end of the list (which will have only elements
        //with zero height here, the rest removed themselves due to hide_i==true
        if (collapsed==BOX_COLLAPSE_COLLAPSE && style.read().indicator.second && !hide) {
            //src and dst of the ArcIndicator will be set in ArcBoxSeries::PostParseProcess
            content.Append(new ArcIndicator(chart, indicator_style, file_pos)); 
            chart->Progress.DoneItem(MscProgress::POST_PARSE, MscProgress::INDICATOR);
        }
        number.decrementOnAddingLevels = false;
    } 
    //left & right will not expand if src and dst is unspecified
    left = chart->EntityMinByPos(left, src);
    right = chart->EntityMaxByPos(right, dst);
    if (*src == chart->NoEntity) src = left_content;
    if (*dst == chart->NoEntity) dst = right_content;
    return ret;
}

void ArcBox::FinalizeLabels(Canvas &canvas)
{
    chart->FinalizeLabelsArcList(content, canvas);
    ArcLabelled::FinalizeLabels(canvas);
}

ArcBase* ArcBoxSeries::PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                        Numbering &number, Element **target, ArcBase *vertical_target)
{
    if (!valid || series.size()==0) return NULL;
    //If first segment is compressed or parallel, copy that to full series
    vspacing = (*series.begin())->vspacing;
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
        ret = (*i)->PostParseProcess(canvas, hide, src, dst, number, target, vertical_target); //ret is an arcblockarrow if we need to collapse
		//Check if we are collapsed to a block arrow
		if ((*i)->collapsed == BOX_COLLAPSE_BLOCKARROW) {
			_ASSERT(series.size()==1);
			if (ret == NULL) *target = DELETE_NOTE;
			else if (ret->CanBeNoted()) *target = ret;
			else *target = DELETE_NOTE; //ArcBox can be noted, so if replacement cannot, we shall silently delete note
			return ret;
		}
        chart->Progress.DoneItem(MscProgress::POST_PARSE, MscProgress::BOX);
		_ASSERT(*i==ret);
    }
    //parallel flag can be either on the series or on the first element
    parallel |= (*series.begin())->parallel;
    //Set the target to the last ArcBox (for comments coming afterwards)
    *target = *series.rbegin();
    //src and dst can be NoEntity here if none of the series specified a left or a right entity
    //Go through and use content to adjust to content
    if (*src==chart->NoEntity) 
        for (auto pBox : series) 
            src = chart->EntityMinByPos(src, pBox->src);
    if (*dst==chart->NoEntity) 
        for (auto pBox : series) 
            dst = chart->EntityMaxByPos(dst, pBox->dst);

    //Src and dst can still be == NoEntity, if no arcs specified
    //inside the content and no enity specified at box declaration.
    //In this case emph box spans to leftmost and rightmost entity in chart.
    //At PostParse chart->AllEntities is already sorted by pos values
    //we only do this step if we are the first in a box series.
    if (*src==chart->NoEntity) {
        for (src = chart->AllEntities.begin(); src!=chart->AllEntities.end(); src++)
            if (!chart->IsVirtualEntity(*src)) break;  //leftmost non-virtual entity
        //If we end up in end() it means we only have virtual entities.
        //In this case an automatic box has no meaning, we drop it
        if (src==chart->AllEntities.end()) {
            chart->Error.Error(file_pos.start, "The chart has no entities I can adjust the size of this box to. Ignoring the box.");
            src=dst; //do not let this to point to no entity. dst must == NoEntity here.
            return NULL;
        }
    }
    if (*dst==chart->NoEntity) {
        for (dst = -- chart->AllEntities.end(); dst!=chart->AllEntities.end(); dst--)
            if (!chart->IsVirtualEntity(*dst)) break;  //rightmost non-virtual entity
        _ASSERT(dst!=chart->AllEntities.end()); //No need to check dst validity, we have ensured we have valid entities above.
    }
    //Now see how entities change due to entity collapse
    EIterator sub1 = chart->FindWhoIsShowingInsteadOf(src, true);
    EIterator sub2 = chart->FindWhoIsShowingInsteadOf(dst, false);

    //if box spans a single entity and both ends have changed, 
    //we kill this box 
    const bool we_diappear = *sub1==*sub2 && sub1!=src && sub2!=dst;
    if (we_diappear && !(*sub1)->running_style.read().indicator.second) 
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

void ArcBoxSeries::FinalizeLabels(Canvas &canvas)
{
    for (auto i=series.begin(); i!=series.end(); i++) {
        (*i)->FinalizeLabels(canvas);
        chart->Progress.DoneItem(MscProgress::FINALIZE_LABELS, MscProgress::BOX);
    }
    ArcBase::FinalizeLabels(canvas);
}

//will only be called for the first box of a multi-segment box series
void ArcBoxSeries::Width(Canvas &canvas, EntityDistanceMap &distances, 
                         DistanceMapVertical &vdist)
{
    if (!valid) return;
    const StyleCoW &overall_style = (*series.begin())->style;
    const EIterator src = (*series.begin())->src;
    const EIterator dst = (*series.begin())->dst;

    //Add a new element to vdist
    vdist.InsertElementTop(this);
    //Add activation status right away
    AddEntityLineWidths(vdist);
    vdist.InsertEntity(src);
    vdist.InsertEntity(dst);

    const auto last_marker = vdist.GetIteratorLast();
    EntityDistanceMap d;
    double max_width = 0; //the widest label plus margins
    for (auto pBox : series) {
        //Add a new element to vdist
        vdist.InsertElementTop(pBox);
        //Add activation status right away
        AddEntityLineWidths(vdist);
        vdist.InsertEntity(src);
        vdist.InsertEntity(dst);

        if (pBox->content.size())
            chart->WidthArcList(canvas, (pBox->content), d, vdist);
        //Add a new element to vdist
        vdist.InsertElementBottom(pBox);
        //Add activation status right away
        AddEntityLineWidths(vdist);

        double width = pBox->parsed_label.getSpaceRequired(chart->XCoord(0.95));
        //calculated margins (only for first segment) and save them
        if (pBox==*series.begin()) {
            const Contour tcov = pBox->parsed_label.Cover(0, width, overall_style.read().line.LineWidth()+chart->boxVGapInside);
            DoublePair margins = overall_style.read().line.CalculateTextMargin(tcov, 0);
            width += margins.first + margins.second;
            pBox->sx_text = margins.first;
            pBox->dx_text = margins.second;
        } else {
            pBox->sx_text = pBox->dx_text = overall_style.read().line.LineWidth();
        }
        max_width = max(max_width, width);
        chart->Progress.DoneItem(MscProgress::WIDTH, MscProgress::BOX);
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
    max_width += 2*chart->boxVGapInside;
    left_space =  left_space_inside +  chart->boxVGapInside + overall_style.read().line.LineWidth();
    right_space = right_space_inside + chart->boxVGapInside + overall_style.read().line.LineWidth();

    //convert the side requirements to pairwise distances
    d.CombineLeftRightToPair_Max(chart->hscaleAutoXGap, chart->activeEntitySize/2);
    d.CombineBoxSideToPair(chart->hscaleAutoXGap);

    //if we span multiple entities ensure that text fits
    if (src!=dst && max_width > left_space + right_space)
        distances.Insert((*src)->index, (*dst)->index, max_width - left_space - right_space);

    //Add side distances
    distances.Insert((*src)->index, DISTANCE_LEFT, left_space);
    distances.Insert((*dst)->index, DISTANCE_RIGHT, right_space);
    distances.InsertBoxSide((*src)->index-1, l_tmp.first, left_space);
    distances.InsertBoxSide((*dst)->index, right_space, r_tmp.second);
    distances += d;
    vdist.Insert((*src)->index, DISTANCE_LEFT, left_space, last_marker);
    vdist.Insert((*dst)->index, DISTANCE_RIGHT, right_space, last_marker);

    //Add a new element to vdist
    vdist.InsertElementBottom(this);
    //Add activation status right away
    AddEntityLineWidths(vdist);
}

void ArcBoxSeries::Layout(Canvas &canvas, AreaList *cover)
{
    height = 0;
    if (!valid) return;
    //A few explanations of the variables exact meaning
    //the upper edge of the upper line of each segment is at yPos
    //total_height includes linewidths and shadow, but not boxVGapOutside (contrary for pipes)
    //left_space and right_space includes linewidth
    //height includes the upper linewidth, emphvgapinside, content, lower emphvgapinside, but not lower lw
    //sx and dx are the inner edges of the lines of the whole box
    StyleCoW &main_style = (*series.begin())->style;
    const double lw = main_style.read().line.LineWidth();
    const double sx = chart->XCoord((*series.begin())->src) - left_space + lw;
    const double dx = chart->XCoord((*series.begin())->dst) + right_space - lw;

    double y = chart->boxVGapOutside;
    yPos = y;
    double comment_end=y;
    AreaList combined_content_cover;
    for (auto i = series.begin(); i!=series.end(); i++) {
        (*i)->yPos = y; //"y" now points to the *top* of the line of the top edge of this box
        
        //Place side comments. This will update "cover" and thus force the content
        //downward if the content also has side notes
        double l=y, r=y;
        (*i)->LayoutCommentsHelper(canvas, cover, l, r);
        comment_end = (*i)->comment_height;

        //Advance upper line and spacing
        y += (*i)->style.read().line.LineWidth() + chart->boxVGapInside;
        (*i)->y_text = y;
        (*i)->sx_text = sx + (*i)->sx_text - lw + chart->boxVGapInside;  //both sx and sx_text includes a lw
        (*i)->dx_text = dx - (*i)->dx_text + lw - chart->boxVGapInside;
        //reflow label if necessary
        if ((*i)->parsed_label.IsWordWrap()) {
            const double overflow = (*i)->parsed_label.Reflow(canvas, (*i)->dx_text - (*i)->sx_text);
            (*i)->OverflowWarning(overflow, "", (*series.begin())->src, (*series.begin())->dst);
        } else {
            (*i)->CountOverflow((*i)->dx_text - (*i)->sx_text);
        }
        //Calculate text cover
        (*i)->text_cover = (*i)->parsed_label.Cover((*i)->sx_text, (*i)->dx_text, (*i)->y_text);
        //Advance label height
        const double th = (*i)->parsed_label.getTextWidthHeight().y;
        //Position arrows if any under the label
        AreaList content_cover = Area((*i)->text_cover, *i);
        if ((*i)->content.size()) {
            Area limit((*i)->text_cover, *i);
            if (i==series.begin() && main_style.read().line.corner.second != CORNER_NONE && main_style.read().line.radius.second>0) {
                //Funnily shaped box, prevent content from hitting it
                LineAttr limiter_line(main_style.read().line);
                limiter_line.radius.second += chart->compressGap;
                const Block b(sx-lw/2, dx+lw/2, y+lw/2, y + std::max(lw+limiter_line.radius.second*4, dx-sx)); //at midpoint of line
                limit += Contour(sx-lw/2, dx+lw/2, 0, y+lw+limiter_line.radius.second) - 
                         limiter_line.CreateRectangle_InnerEdge(b);
            }
            const double content_y = chart->PlaceListUnder(canvas, (*i)->content, y+th, 
                                                           y, limit, IsCompressed(), 
                                                           &content_cover);  //no extra margin below text
            y = std::max(y+th, content_y);
        } else {
            y += th; //no content, just add textheight
        }
        if (i==--series.end() && main_style.read().line.corner.second != CORNER_NONE && main_style.read().line.radius.second>0) {
            //Funnily shaped box, prevent it content from hitting the bottom of the content
            LineAttr limiter_line(main_style.read().line);
            limiter_line.radius.second += chart->compressGap;
            const Block b(sx-lw/2, dx+lw/2, -limiter_line.radius.second*2, y);
            const Contour bottom = Contour(sx-lw/2, dx+lw/2, y-limiter_line.radius.second, y+1) -
                                   limiter_line.CreateRectangle_InnerEdge(b);
            double tp;
            double off = content_cover.OffsetBelow(bottom, tp);
            if (off>0 && IsCompressed()) y-=off;
            if (off<0) y-=off;
        }
        y += chart->boxVGapInside;
        //increase the size of the box by the side notes, except for the last box
        if (i!=--series.end()) y = std::max(y, comment_end);
        //Make segment as tall as needed to accomodate curvature
        //if (style.read().line.radius.second>0) {
        //    double we_need_this_much_for_radius = (*i)->style.read().line.LineWidth();
        //    if (i==follow.begin())
        //        we_need_this_much_for_radius += style.read().line.radius.second;
        //    if (i==--follow.end())
        //        we_need_this_much_for_radius += style.read().line.radius.second;
        //    y = std::max(y, (*i)->yPos + we_need_this_much_for_radius);
        //}
        y = ceil(y);
        (*i)->height = y - (*i)->yPos;  //for boxes "height is meant without the lower line

        //Add the linewidth of the next box or the final one
        if (i==--series.end())
            (*i)->height_w_lower_line = (*i)->height + lw;
        else
            (*i)->height_w_lower_line = (*i)->height + (*((++i)--))->style.read().line.LineWidth();
        if (cover)
            combined_content_cover += std::move(content_cover);
        chart->Progress.DoneItem(MscProgress::LAYOUT, MscProgress::BOX);
    } /* for cycle through segments */
    //Final advance of linewidth, the inner edge (y) is on integer
    total_height = y + lw - yPos;

    Block b(sx-lw/2, dx+lw/2, yPos+lw/2, yPos + total_height - lw/2);  //The midpoint of us
    //update the style so that it's radius is not bigger than it can be
    const double sr = main_style.read().line.SaneRadius(b);
    if (main_style.read().line.radius.second > sr)
        main_style.write().line.radius.second = sr;
    
    Area overall_box(main_style.read().line.CreateRectangle_OuterEdge(b), this);
    // now we have all geometries correct, now calculate areas and covers
    for (auto box : series) {
        box->area = Contour(sx-lw, dx+lw, box->yPos, box->yPos + box->height_w_lower_line) * overall_box;
        box->area.arc = box;
        if (box->content.size() && box->collapsed==BOX_COLLAPSE_EXPAND) {
            //Make a frame, add it to the already added label
            box->area_draw = box->area.CreateExpand(chart->trackFrameWidth) - box->area;
            box->area_draw += box->text_cover.CreateExpand(chart->trackExpandBy);
            box->draw_is_different = true;
            box->area_draw_is_frame = true;
        } else {
            box->area_draw.clear();
            box->draw_is_different = false;
            box->area_draw_is_frame = false;
        }
        box->area_important = box->text_cover;
        chart->NoteBlockers.Append(box);
    }
    const double &offset = main_style.read().shadow.offset.second;
    if (offset)
        overall_box += overall_box.CreateShifted(XY(offset, offset));
    overall_box.mainline = Block(chart->GetDrawing().x, b.y);
    if (cover) {
        *cover += GetCover4Compress(overall_box);
        //See if some of the content lies outside this box series - then add it to cover
        std::list<Area> alist = combined_content_cover.EmptyToList(); //convert to list so that we can move elements out                     
        for (auto area : alist)
            if (!overall_box.GetBoundingBox().IsWithinBool(area.GetBoundingBox().UpperLeft()) ||
                !overall_box.GetBoundingBox().IsWithinBool(area.GetBoundingBox().LowerRight()))
                *cover += std::move(area);
    }
    height = yPos + total_height + offset + chart->boxVGapOutside;
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

double ArcBoxSeries::SplitByPageBreak(Canvas &canvas, double netPrevPageSize,
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
            //break the list, but do not shift all of it to the next page
            const double ret = chart->PageBreakArcList(canvas, (*i)->content, netPrevPageSize,
                                                       pageBreak, addCommandNewpage, addHeading,
                                                       false, true);
            if (ret>=0) {
                //if we did split the list somewhere in its middle
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
            //else we cannot break somewhere in `i`
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
        const double shift_top = (*series.begin())->style.read().line.LineWidth();
        const double increase_top = shift_top - (*abs->series.begin())->style.read().line.LineWidth();
        const double shift_rest = shift_top + increase_top;
        //copy line style to the first one
        (*abs->series.begin())->style.write().line = (*series.begin())->style.read().line;
        (*--series.end())->height_w_lower_line += increase_top;
        (*abs->series.begin())->height += increase_top;
        (*abs->series.begin())->height_w_lower_line += increase_top;
        (*abs->series.begin())->ShiftBy(shift_rest);
        (*abs->series.begin())->yPos -= shift_top;
        abs->yPos = (*abs->series.begin())->yPos;
        abs->AddAttributeList(NULL);
        abs->vspacing = vspacing;
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




void ArcBoxSeries::PlaceWithMarkers(Canvas &canvas)
{
    for (auto i = series.begin(); i!=series.end(); i++) {
        chart->Progress.DoneItem(MscProgress::PLACEWITHMARKERS, MscProgress::BOX);
        if ((*i)->valid && (*i)->content.size()) 
            chart->PlaceWithMarkersArcList(canvas, (*i)->content);
    }
}


void ArcBoxSeries::PostPosProcess(Canvas &canvas)
{
    if (!valid) return;
    //For boxes we always add the background cover first then the content
    //And we do this for each segment sequentially
    for (auto pBox : series)
        if (pBox->valid) {
            if (pBox->content.size()) 
                switch (pBox->collapsed) {
                case BOX_COLLAPSE_EXPAND:
                    pBox->controls.push_back(MSC_CONTROL_COLLAPSE);        
                    if (series.size()==1) 
                        pBox->controls.push_back(MSC_CONTROL_ARROW);        
                    break;
                case BOX_COLLAPSE_COLLAPSE:
                    pBox->controls.push_back(MSC_CONTROL_EXPAND);        
                    if (series.size()==1) 
                        pBox->controls.push_back(MSC_CONTROL_ARROW);        
                    break;
                case BOX_COLLAPSE_BLOCKARROW:
                default:
                    _ASSERT(0); //should not happen here
                    break;
                }
            pBox->entityLineRange = pBox->area.GetBoundingBox().y;
            pBox->ArcLabelled::PostPosProcess(canvas);
            chart->Progress.DoneItem(MscProgress::POST_POS, MscProgress::BOX);
            if (pBox->content.size()) 
                chart->PostPosProcessArcList(canvas, pBox->content);
        }

    //Hide entity lines during the lines inside the box
    const StyleCoW &main_style = (*series.begin())->style;
    const double lw = main_style.read().line.LineWidth();
    const double src_x = chart->XCoord((*series.begin())->src);
    const double dst_x = chart->XCoord((*series.begin())->dst);
    const double sx = src_x - left_space + lw;
    const double dx = dst_x + right_space - lw;
    for (auto i = ++series.begin(); i!=series.end(); i++) 
        if ((*i)->style.read().line.IsDoubleOrTriple()) {
            const Block r(sx, dx, (*i)->yPos, (*i)->yPos+(*i)->style.read().line.LineWidth());
            chart->HideEntityLines(r);
        }
    
    //hide the entity lines under the labels
    for (auto i = series.begin(); i!=series.end(); i++) 
        chart->HideEntityLines((*i)->text_cover);
    //hide top and bottom line if double
    if (main_style.read().line.IsDoubleOrTriple()) {
        Block b(src_x - left_space + lw/2, dst_x + right_space - lw/2,
                yPos + lw/2, yPos+total_height - lw/2); //The midpoint of the lines
        //The radius specified in style.read().line will be that of the midpoint of the line
        chart->HideEntityLines(main_style.read().line.CreateRectangle_OuterEdge(b) - 
                               main_style.read().line.CreateRectangle_InnerEdge(b));
    }
}

void ArcBoxSeries::Draw(Canvas &canvas, EDrawPassType pass)
{
    if (!valid || series.size()==0) return;
    //For boxes draw background for each segment, then separator lines, then bounding rectangle lines, then content
    const StyleCoW &main_style = (*series.begin())->style;
    const double lw = main_style.read().line.LineWidth();
    const double src_x = chart->XCoord((*series.begin())->src);
    const double dst_x = chart->XCoord((*series.begin())->dst);
    //The midpoint of the lines
    const Block r(src_x - left_space + lw/2, dst_x + right_space - lw/2, 
                  yPos + lw/2, yPos+total_height - lw/2); 
    //The radius specified in main_style.line will be that of the midpoint of the line
    //First draw the shadow.
    if (pass==series.front()->draw_pass) 
        canvas.Shadow(r, main_style.read().line, main_style.read().shadow);
    //Do a clip region for the overall box (for round/bevel/note corners)
    //at half a linewidth from the inner edge (use the width of a single line!)
    const Contour clip = main_style.read().line.CreateRectangle_ForFill(r);
    for (auto i = series.begin(); i!=series.end(); i++) {
        //Overall rule for background fill:
        //for single line borders we fill up to the middle of the border
        //for double line borders we fill up to the middle of the inner line of the border
        //style.read().line.LineWidth() gives the full width of the (double) line, width.second is just one line of it
        //for single lines style.read().line.LineWidth()==style.read().line.width.second
        double sy = (*i)->yPos + (*i)->style.read().line.LineWidth() - (*i)->style.read().line.width.second/2.;
        double dy = (*i)->yPos + (*i)->height;
        //decrease upper limit for the first one (note+triple line has areas higher than this to cover)
        //clip will cut away the not needed areas
        if (i==series.begin())
            sy -= lw;
        //Increase the fill area downward by half of the linewidth below us
        PtrList<ArcBox>::const_iterator next = i;
        next++;
        if (next==series.end())
            dy += main_style.read().line.width.second/2.;
        else
            dy += (*next)->style.read().line.width.second/2.;
        canvas.Clip(clip);
        //fill wider than r.x - note+triple line has wider areas to cover, clip will cut away excess
        if (pass==(*i)->draw_pass) 
            canvas.Fill(Block(r.x.from, r.x.till+lw, sy, dy), (*i)->style.read().fill);
        //if there are contained entities, draw entity lines, strictly from inside of line
        if ((*i)->content.size()) {
            if (pass==(*i)->draw_pass && (*i)->drawEntityLines &&
                (*i)->collapsed==BOX_COLLAPSE_EXPAND)
                chart->DrawEntityLines(canvas, (*i)->yPos, (*i)->height + (*i)->style.read().line.LineWidth(), (*i)->src, ++EIterator((*i)->dst));
            canvas.UnClip();
            chart->DrawArcList(canvas, (*i)->content, chart->GetTotal().y, pass);
        } else
            canvas.UnClip();
    }
    //Draw box lines - Cycle only for subsequent boxes
    for (auto i = ++series.begin(); i!=series.end(); i++) {
        if (pass!=(*i)->draw_pass) continue;
        const double y = (*i)->yPos + (*i)->style.read().line.LineWidth()/2;
        const XY magic(1,0);  //XXX needed in windows
        canvas.Line(XY(r.x.from, y)-magic, XY(r.x.till, y), (*i)->style.read().line);
    }
    //Finally draw the overall line around the box
    if (pass==series.front()->draw_pass)
        canvas.Line(r, main_style.read().line);
    //XXX double line joints: fix it
    for (auto i = series.begin(); i!=series.end(); i++) {
        if (pass==(*i)->draw_pass) 
            (*i)->parsed_label.Draw(canvas, (*i)->sx_text, (*i)->dx_text, (*i)->y_text, r.x.MidPoint());
    }
}

/////////////////////////////////////////////////////////////////

ArcPipe::ArcPipe(ArcBox *box) :
    ArcLabelled(box->type, MscProgress::PIPE, box->chart, 
                box->chart->Contexts.back().styles["pipe"]),
    src(box->src), dst(box->dst), drawEntityLines(false)
{
    delete box;
    switch (type) {
    case MSC_BOX_SOLID:
        style += chart->Contexts.back().styles["pipe--"]; break;
    case MSC_BOX_DASHED:
        style += chart->Contexts.back().styles["pipe++"]; break;
    case MSC_BOX_DOTTED:
        style += chart->Contexts.back().styles["pipe.."]; break;
    case MSC_BOX_DOUBLE:
        style += chart->Contexts.back().styles["pipe=="]; break;
    default:
        _ASSERT(0);
    }
}

ArcPipeSeries::ArcPipeSeries(ArcPipe *first) :
    ArcBase(MSC_BOX_SOLID, MscProgress::PIPE_SERIES, first->chart), 
    series(true), drawing_variant(1)
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
    return ArcLabelled::AddAttribute(a);
}

void ArcPipe::AttributeNames(Csh &csh)
{
    ArcLabelled::AttributeNames(csh);
    defaultDesign.styles.GetStyle("pipe").read().AttributeNames(csh);
}

bool ArcPipe::AttributeValues(const std::string attr, Csh &csh)
{
    if (defaultDesign.styles.GetStyle("pipe").read().AttributeValues(attr, csh)) return true;
    if (ArcLabelled::AttributeValues(attr, csh)) return true;
    return false;
}

//Should be called before AddAttributeList for "f" is called
ArcPipeSeries* ArcPipeSeries::AddFollowWithAttributes(ArcPipe*f, AttributeList *l)
{
    _ASSERT(f);
    if (f==NULL) return this;
    if (f->valid) {
        f->style.write().Empty();
        f->AddAttributeList(l);
        if (f->style.read().side.first) {
            f->style.write().side.first = false;
            chart->Error.Error(f->file_pos.start,
                               "Attribute 'side' can only be specified in the first "
                               "element in a pipe series. Ignoring it in subsequent ones.");
        }
        if (f->style.read().line.radius.first) {
            f->style.write().line.radius.first = false;
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
        StyleCoW s = (*series.begin())->style;
        //Override with the line type specified (if any)
        _ASSERT(f->type != MSC_BOX_UNDETERMINED_FOLLOW);
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

EDirType ArcPipeSeries::GetToucedEntities(EntityList &el) const
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

ArcBase* ArcPipeSeries::PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right,
                                        Numbering &number, Element **target, ArcBase *vertical_target)
{
    if (!valid) return NULL;

    //Add numbering, if needed 
    for (auto i = series.begin(); i!=series.end(); i++) {
        (*i)->PostParseProcess(canvas, hide, left, right, number, target, vertical_target);
        chart->Progress.DoneItem(MscProgress::POST_PARSE, MscProgress::PIPE);
    }
    //Postparse the content;
    EIterator content_left, content_right;
    content_right = content_left = chart->AllEntities.Find_by_Name(NONE_ENT_STR);
    //set the first element as the note target (first in the {})
    *target = *series.begin();
    chart->PostParseProcessArcList(canvas, hide, content, false, content_left, content_right, number, target);

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
        lw_max = std::max(lw_max, (*i)->style.read().line.LineWidth());
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
        if ((*ei_if_disappear)->running_style.read().indicator.second) {
            //leave an indicator, but update left and right
            left = chart->EntityMinByPos(ei_if_disappear, left);
            right = chart->EntityMaxByPos(ei_if_disappear, right);
            ei_if_disappear = chart->FindWhoIsShowingInsteadOf(ei_if_disappear, true); //result points to ActiveEntities
            return new ArcIndicator(chart, ei_if_disappear, indicator_style, file_pos);  
        } else 
            return NULL;
    }

    //increase the radius everywhere by the thickest lw (if it is not zero)
    if ((*series.begin())->style.read().line.radius.second>0) {
        const double radius = (*series.begin())->style.read().line.radius.second + lw_max;
        for (auto i = series.begin(); i!=series.end(); i++)
            (*i)->style.write().line.radius.second = radius;
    }

    //Sort according to fromright: begin() should point to the leftmost pipe if side==right,
    //and to the rightmost if side=left
    if ((*series.begin())->style.read().side.second == ESide::LEFT) {
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
        if (((*series.begin())->style.read().side.second == ESide::RIGHT && (*i_prev)->dst == (*i)->src) ||
            ((*series.begin())->style.read().side.second == ESide::LEFT  && (*i_prev)->src == (*i)->dst)) {
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
            (*i)->style.write().solid.second = 255;
    else if (canvas.AvoidTransparency())
        for (auto i = series.begin(); i!=series.end(); i++)
            //disallow transparency if too low power
            if ((*i)->style.read().solid.second!=255) 
                (*i)->style.write().solid.second = 0;

    return this;
}

void ArcPipeSeries::FinalizeLabels(Canvas &canvas)
{
    for (auto i=series.begin(); i!=series.end(); i++) {
        (*i)->FinalizeLabels(canvas);
        chart->Progress.DoneItem(MscProgress::FINALIZE_LABELS, MscProgress::PIPE);
    }
    chart->FinalizeLabelsArcList(content, canvas);
    ArcBase::FinalizeLabels(canvas);
}

//will only be called for the first box of a multi-segment box series
void ArcPipeSeries::Width(Canvas &canvas, EntityDistanceMap &distances, DistanceMapVertical &vdist)
{
    if (!valid) return;
    //Add a new element to vdist
    vdist.InsertElementTop(this);
    //Add activation status right away
    AddEntityLineWidths(vdist);
    for (auto pPipe : series) {
        //Add a new element to vdist
        vdist.InsertElementTop(pPipe);
        //Add activation status right away
        AddEntityLineWidths(vdist);
        vdist.InsertEntity(pPipe->src);
        vdist.InsertEntity(pPipe->dst);
    }
    EntityDistanceMap d, d_pipe;
    if (content.size())
        chart->WidthArcList(canvas, content, d, vdist);

    const ESide side = (*series.begin())->style.read().side.second;
    const double radius = (*series.begin())->style.read().line.radius.second;

    //(*i)->src and dst contain the left and right end of a pipe
    //The order of the pipe segments in follow depends on style.read().side
    for (auto pPipe : series) {
        const double ilw = pPipe->style.read().line.LineWidth();
        const double width = pPipe->parsed_label.getSpaceRequired() + 2*chart->boxVGapInside;
        pPipe->left_space = d.Query((*pPipe->src)->index, DISTANCE_LEFT) + chart->boxVGapInside;
        pPipe->right_space = d.Query((*pPipe->dst)->index, DISTANCE_RIGHT) + chart->boxVGapInside;
        //Add extra space for curvature
        if (side == ESide::RIGHT)
            pPipe->right_space += radius;
        else
            pPipe->left_space += radius;

        //The style.read().line.radius.second is understood to be the radius of the hole of the _outer edge_
        if (pPipe->src==pPipe->dst) {
            pPipe->left_space  = std::max(width/2, pPipe->left_space);
            pPipe->right_space = std::max(width/2, pPipe->right_space);
        } else {
            //keep a big enough space between src and dst for the text + curvature of pipe
            //the text can go out of the entity lines, all the way to the left-space
            d_pipe.Insert((*pPipe->src)->index, (*pPipe->dst)->index,
                                width - pPipe->left_space - pPipe->right_space +
                                radius);
        }
        pPipe->left_space  = ceil(pPipe->left_space);
        pPipe->right_space = ceil(pPipe->right_space);
        const bool connect_left = side == ESide::RIGHT ? pPipe->pipe_connect_back : pPipe->pipe_connect_forw;
        const bool connect_right = side == ESide::RIGHT ? pPipe->pipe_connect_forw : pPipe->pipe_connect_back;
        //Check if we are connecting to a neighbour pipe segment
        if (connect_left)
            pPipe->left_space = 0;
        else {
            d_pipe.Insert((*pPipe->src)->index, DISTANCE_LEFT,
                          pPipe->left_space + ilw + radius);
            vdist.Insert((*pPipe->src)->index, DISTANCE_LEFT,
                         pPipe->left_space + ilw + radius);
        }
        //add shadow to the right size only if we are the rightmost entity
        double shadow_to_add = 0;
        if ((side == ESide::RIGHT && pPipe==*series.rbegin()) ||
            (side == ESide::LEFT  && pPipe==*series.begin()))
            shadow_to_add = pPipe->style.read().shadow.offset.second;
        if (connect_right)
            pPipe->right_space = 0;
        else {
            d_pipe.Insert((*pPipe->dst)->index, DISTANCE_RIGHT,
                          pPipe->right_space + ilw + radius + shadow_to_add);
            vdist.Insert((*pPipe->dst)->index, DISTANCE_RIGHT,
                          pPipe->right_space + ilw + radius + shadow_to_add);
        }
        chart->Progress.DoneItem(MscProgress::WIDTH, MscProgress::PIPE);
    }
    d_pipe.CombinePairedLeftRightToPair_Sum(chart->hscaleAutoXGap);
    distances += d_pipe;

    //Finally add the requirements of the content
    d.CombineLeftRightToPair_Max(chart->hscaleAutoXGap, chart->activeEntitySize/2);
    d.CombineBoxSideToPair(chart->hscaleAutoXGap);
    distances += d;

    for (auto pPipe : series) {
        //Add a new element to vdist
        vdist.InsertElementBottom(pPipe);
        //Add activation status right away
        AddEntityLineWidths(vdist);
    }
    //Add a new element to vdist
    vdist.InsertElementBottom(this);
    //Add activation status right away
    AddEntityLineWidths(vdist);
}

//Takes the pipe_body member of each pipe and calculates their contours used for drawing.
//returns the total body cover if not NULL
void ArcPipeSeries::CalculateContours(Area *pipe_body_cover)
{
    //A few shortcuts. "side" and "radius" must be the same in any pipe element, so we take the first
    const ESide side = (*series.begin())->style.read().side.second;
    const double radius = (*series.begin())->style.read().line.radius.second;
    //the largest of the shadow offsets
    for (auto i = series.begin(); i!=series.end(); i++) {
        //No need to clean up. If any of the pipe_* or area, area_draw, area_important
        //has values here, they will get simply overwritten
        const double ilw = (*i)->style.read().line.LineWidth();

        XY cs((*i)->pipe_block.x.from, (*i)->pipe_block.y.MidPoint());
        XY cd((*i)->pipe_block.x.till, (*i)->pipe_block.y.MidPoint());
        const XY rad(radius, (*i)->pipe_block.y.Spans()/2); //we use the first pipe's line.radius
        if (side == ESide::LEFT) std::swap(cs, cd); //use the first pipe's fromright, not (*i)->fromright
        //now cd is the one with the hole
        const Contour back_end(cs, rad.x, rad.y);
        const Contour forw_end(cd, rad.x, rad.y);
        //(*i)->pipe_block, back_end and forw_end are now all outer edge
        //we need to shring by ilw/2 to get the line contour and by ilw/2-line.width/2 to get the fill contour
        const double gap_for_line = -ilw/2;
        const double gap_for_fill = -ilw + (*i)->style.read().line.width.second/2;
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
            if (content.size() && (*i)->style.read().solid.second < 255)
                (*i)->area_draw = (*i)->area.CreateExpand(chart->trackFrameWidth);
            //We take a big, unshrunken back-end out of fill: (this will be the next segments outer edge)
            if (rad.x>0) {
                (*i)->pipe_body_fill -= forw_end;
                (*i)->area -= forw_end;
                //below we need to first expand forw_end before substracting it
                //the other way is not ok: Expand fails in expanding negative arcs
                if (content.size() && (*i)->style.read().solid.second < 255) {
                    (*i)->area_draw -= forw_end.CreateExpand(-chart->trackFrameWidth);
                    (*i)->area_draw *= Contour(side == ESide::RIGHT ? chart->GetDrawing().x.from : chart->GetDrawing().x.till, cd.x,
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
                (*i)->pipe_body_fill -= forw_end.CreateExpand(-(*i)->style.read().line.width.second/2);
                (*i)->pipe_whole_line = (*i)->pipe_body_line + forw_end.CreateExpand(gap_for_line);
                (*i)->pipe_body_line -= forw_end.CreateExpand(gap_for_line);

                //for shaodow we add
                (*i)->pipe_shadow += forw_end;
                (*i)->pipe_hole_line = forw_end.CreateExpand(gap_for_line);
                (*i)->pipe_hole_fill = forw_end.CreateExpand(gap_for_fill);
                //const Edge hole_line[2] = {Edge(cd, rad.x+gap_for_line, rad.y+gap_for_line, 0, 90, 270), 
                //                           Edge(cd, rad.x+gap_for_line, rad.y+gap_for_line, 0, 270, 90)};
                //(*i)->pipe_hole_curve.assign_dont_check(hole_line);
                ////this is only half of the hole ellipsos
                //(*i)->pipe_hole_curve[0][side == ESide::RIGHT ? 0 : 1].visible = false;
                std::vector<Edge> tmp;
                tmp.reserve(5);
                Edge::GenerateEllipse(tmp, cd, rad.x+gap_for_line, rad.y+gap_for_line,
                                                side == ESide::RIGHT ? 0 : 180, 270, 90);
                const XY s = tmp.front().GetStart();
                const XY e = tmp.back().GetEnd();
                const XY off(side == ESide::RIGHT ? -rad.x : rad.x, 0);
                tmp.emplace_back(e, e + off, false);
                tmp.emplace_back(e + off, s + off, false);
                tmp.emplace_back(s + off, s, false);
                (*i)->pipe_hole_curve.assign_dont_check(tmp);
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
            if (content.size() && (*i)->style.read().solid.second < 255)
                (*i)->area_draw = (*i)->area.CreateExpand(chart->trackFrameWidth);
        }
        //Finalize covers
        (*i)->area.arc = *i;
        if (content.size() && (*i)->style.read().solid.second < 255) {
            //Make a frame, add it to the already added label
            (*i)->area_draw -= (*i)->area;
            (*i)->area_draw += (*i)->text_cover.CreateExpand(chart->trackExpandBy);
            (*i)->area_draw_is_frame = true;
            (*i)->draw_is_different = true;
        }
        if (pipe_body_cover) {
            //now determine the cover to be used for placement
            const double offset = (*i)->style.read().shadow.offset.second;
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
        (*i)->pipe_shadow = (*i)->pipe_shadow.CreateExpand(-(*i)->style.read().line.width.second/2);
}

void ArcPipeSeries::Layout(Canvas &canvas, AreaList *cover)
{
    height = 0;
    if (!valid) return;
    //Collect cover information from labels and linewidth, so compression of content arrows can be done
    //Determine thickest line for precise pipe alignment
    double max_lineWidth = 0;
    for (auto i = series.begin(); i!=series.end(); i++)
        max_lineWidth = std::max(max_lineWidth, (*i)->style.read().line.LineWidth());
    double lowest_line_bottom =  max_lineWidth + chart->boxVGapInside;
    //Determine highest label and collect all text covers
    //Also calcualte all x positioning
    double lowest_label_on_transculent_bottom = lowest_line_bottom;
    double lowest_label_on_opaque_segments_bottom = lowest_line_bottom;
    Area label_covers(this);
    //A few shortcuts. "side" and "radius" must be the same in any pipe element, so we take the first
    const ESide side = (*series.begin())->style.read().side.second;
    const double radius = (*series.begin())->style.read().line.radius.second;
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
        (*i)->LayoutCommentsHelper(canvas, cover, note_l, note_r);

        //Set pipe_block.x, sx_text, dx_text in each segment, in the meantime
        //pipe_block contains the outside of the pipe, with the exception of the curvature (since it is a rect)
        (*i)->y_text = ceil(chart->boxVGapOutside + (*i)->style.read().line.LineWidth() +
                        chart->boxVGapInside);
        (*i)->area.clear();
        (*i)->pipe_block.x.from = chart->XCoord((*i)->src) - (*i)->left_space; //already rounded
        (*i)->pipe_block.x.till = chart->XCoord((*i)->dst) + (*i)->right_space;
        (*i)->sx_text = (*i)->pipe_block.x.from + (*i)->style.read().line.LineWidth() + chart->boxVGapInside; //not rounded
        (*i)->dx_text = (*i)->pipe_block.x.till - (*i)->style.read().line.LineWidth() - chart->boxVGapInside;
        switch (side) {
        case ESide::RIGHT: (*i)->dx_text -= radius; break;
        case ESide::LEFT:  (*i)->sx_text += radius; break;
        default: _ASSERT(0);
        }
        //reflow label if necessary
        if ((*i)->parsed_label.IsWordWrap()) {
            const double overflow = (*i)->parsed_label.Reflow(canvas, (*i)->dx_text - (*i)->sx_text);
            (*i)->OverflowWarning(overflow, "", (*series.begin())->src, (*series.begin())->dst);
        } else {
            (*i)->CountOverflow((*i)->dx_text - (*i)->sx_text);
        }
        (*i)->text_cover = (*i)->parsed_label.Cover((*i)->sx_text, (*i)->dx_text, (*i)->y_text);
        // omit text cover for pipes if the pipe is fully opaque,
        // in that case content can be drawn at same position as label - opaque pipe will cover anyway
        double y = (*i)->y_text + (*i)->parsed_label.getTextWidthHeight().y;
        if (y == (*i)->y_text && content.size()==0)
            y += (*i)->style.read().text.getCharHeight(canvas);
        if ((*i)->style.read().solid.second < 255) {
            label_covers += (*i)->text_cover;
            lowest_label_on_transculent_bottom = std::max(lowest_label_on_transculent_bottom, y);
        } else {
            //collect the highest label of opaque segments for later use
            lowest_label_on_opaque_segments_bottom = std::max(lowest_label_on_opaque_segments_bottom, y);
        }
        chart->Progress.DoneItem(MscProgress::LAYOUT, MscProgress::PIPE);
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
    y += chart->boxVGapInside + max_lineWidth;
    //now y contains the bottommost pixel of the pipe itself
    total_height = y = ceil(y);
    //Now set the y coordinate in all segments
    double max_shadow_offset = 0;
    for (auto &pPipe : series) {
        //fill in pipe_block.y (both are integer)
        pPipe->pipe_block.y.from = chart->boxVGapOutside;
        pPipe->pipe_block.y.till = y;
        chart->NoteBlockers.Append(pPipe);
        max_shadow_offset = std::max(max_shadow_offset, pPipe->style.read().shadow.offset.second);
    }
    //Calculate contours in all pipes from pipe_block
    Area pipe_body_cover(this);
    CalculateContours(&pipe_body_cover);
    //Add content to cover (may "come out" from pipe)
    if (cover)
        *cover += content_cover;
    //If we have no valid content, set mainline to that of pipe, else the content's mainline will be used
    if (content_cover.mainline.IsEmpty()) 
        pipe_body_cover.mainline = Block(chart->GetDrawing().x.from, chart->GetDrawing().x.till, chart->boxVGapOutside, total_height);  //totalheight includes the top emphvgapoutside 
    //Expand cover, but not content (that is already expanded)
    if (cover)
        *cover += GetCover4Compress(pipe_body_cover);
    height = yPos + total_height + max_shadow_offset + chart->boxVGapOutside;
    for (auto &pPipe : series)
        pPipe->height = height; //so that if a vertical is following pPipe we get the right height
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

double ArcPipeSeries::SplitByPageBreak(Canvas &canvas, double netPrevPageSize,
                                       double pageBreak, bool &addCommandNewpage,
                                       bool addHeading, ArcList &/*res*/)
{
    if (content.size()==0 || keep_together) return -1; //we cannot split if no content
    //if pageBreak goes through a label (or is above) we cannot split there
    for (auto i = series.begin(); i!=series.end(); i++)
        if ((*i)->text_cover.GetBoundingBox().y.till > pageBreak || (*i)->keep_together)
            return -1;
    const double ret = chart->PageBreakArcList(canvas, content, netPrevPageSize,
                                               pageBreak, addCommandNewpage, addHeading,
                                               false, true);
    //if pb makes *all* of the list go to the next page, just shift the
    //whole pipe series to the next page
    if (ret<0) return -1;
    height += ret;
    total_height += ret;
    for (auto i=series.begin(); i!=series.end(); i++)
        (*i)->pipe_block.y.till += ret;
    CalculateContours();
    return ret;
}


void ArcPipeSeries::PlaceWithMarkers(Canvas &canvas)
{
    if (content.size())
        chart->PlaceWithMarkersArcList(canvas, content);
    for (unsigned u=0; u<series.size(); u++)
    chart->Progress.DoneItem(MscProgress::PLACEWITHMARKERS, MscProgress::PIPE);
}

void ArcPipeSeries::PostPosProcess(Canvas &canvas)
{
    if (!valid) return;
    //For pipes we first add those covers to chart->AllCovers that are not fully opaque,
    //then the content (only in the first segment)
    //then those segments, which are fully opaque
    //(this is because search is backwards and this arrangement fits the visual best
    for (auto i = series.begin(); i!=series.end(); i++) 
        if ((*i)->valid && (*i)->style.read().solid.second < 255) {
            (*i)->entityLineRange = (*i)->area.GetBoundingBox().y;
            (*i)->ArcLabelled::PostPosProcess(canvas);
            chart->Progress.DoneItem(MscProgress::POST_POS, MscProgress::PIPE);
        }
    if (content.size())
        chart->PostPosProcessArcList(canvas, content);
    for (auto i = series.begin(); i!=series.end(); i++)
        if ((*i)->valid && (*i)->style.read().solid.second == 255) {
            (*i)->entityLineRange = (*i)->area.GetBoundingBox().y;
            (*i)->ArcLabelled::PostPosProcess(canvas);
            chart->Progress.DoneItem(MscProgress::POST_POS, MscProgress::PIPE);
        }
    for (auto i = series.begin(); i!=series.end(); i++)
        if ((*i)->draw_pass != DRAW_BEFORE_ENTITY_LINES)
            chart->HideEntityLines((*i)->pipe_shadow);
}

//Draw a pipe, this is called for each segment, bool params dictate which part
//topside is the bigger part of the pipe
//backside is the small oval visible form the back of the pipe
//this->yPos is the outer edge of the top line
//this->left_space and right_space includes linewidth
void ArcPipe::DrawPipe(Canvas &canvas, EDrawPassType pass, 
                       bool topSideFill, bool topSideLine, bool backSide,
                       bool shadow, bool text, double next_lw, 
                       int drawing_variant)
{
    if (pass!=draw_pass)
        return;
    if (shadow) {
        //Shadow under the whole pipe
        canvas.Shadow(pipe_shadow, style.read().shadow);
    }
    if (backSide) {
        //The back of the main pipe
        FillAttr fill = style.read().fill;
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
        canvas.Line(pipe_hole_curve, style.read().line);
    }
    if (topSideFill) {
        //apply the transparency of the solid attribute
        FillAttr fill = style.read().fill;
        fill.color.second.a = unsigned(style.read().solid.second) * unsigned(fill.color.second.a) / 255;
        canvas.Fill(pipe_body_fill, fill);
    }
    if (topSideLine) {
        cairo_line_join_t t = canvas.SetLineJoin(CAIRO_LINE_JOIN_BEVEL);
        const double x = style.read().side.second == ESide::RIGHT ? pipe_block.x.till : pipe_block.x.from;
        Contour clip(x, style.read().side.second == ESide::LEFT ? chart->GetDrawing().x.till : chart->GetDrawing().x.from,
                     chart->GetDrawing().y.from, chart->GetDrawing().y.till);
        if (style.read().line.radius.second>0 && pipe_connect_forw) {
            const XY c(x, pipe_block.y.MidPoint());
            clip -= Contour(c, style.read().line.radius.second-next_lw/2, pipe_block.y.Spans()/2.-next_lw/2);
        }
        canvas.Clip(clip);
        const double spacing = style.read().line.Spacing();
        if (!style.read().line.IsDoubleOrTriple() || drawing_variant==0)
            canvas.Line(pipe_body_line, style.read().line); //basic variant, in case of double & triple, lines cross
        else if (style.read().line.IsDouble()) {
            canvas.SetLineAttr(style.read().line);
            if (drawing_variant==1) { //advanced: lines do not cross
                canvas.singleLine(pipe_whole_line.CreateExpand(spacing), style.read().line);
                canvas.singleLine(pipe_hole_line.CreateExpand(-spacing), style.read().line);
                canvas.SetLineJoin(CAIRO_LINE_JOIN_MITER);
                canvas.singleLine(pipe_body_line.CreateExpand(-spacing), style.read().line);
            } else { //very advanced: proper double line joint
                canvas.singleLine(pipe_whole_line.CreateExpand(spacing), style.read().line); //outer
                canvas.SetLineJoin(CAIRO_LINE_JOIN_MITER);
                canvas.singleLine(pipe_hole_line.CreateExpand(-spacing), style.read().line); //inner hole
                canvas.singleLine(pipe_body_line.CreateExpand(-spacing) -
                                  pipe_hole_line.CreateExpand(spacing), style.read().line);  //inner body
            }
        } else if (style.read().line.IsTriple()) {
            canvas.SetLineAttr(style.read().line);
            //here variant 1 and 2 result in the same
            canvas.singleLine(pipe_whole_line.CreateExpand(spacing), style.read().line);  //outer
            canvas.SetLineJoin(CAIRO_LINE_JOIN_MITER);
            canvas.singleLine(pipe_body_line.CreateExpand(-spacing), style.read().line);  //inner body  
                                //pipe_hole_line.CreateExpand(spacing), style.read().line);  //inner body
            canvas.SetLineJoin(CAIRO_LINE_JOIN_BEVEL);
            canvas.singleLine(pipe_hole_line.CreateExpand(-spacing), style.read().line);   //inner hole
            cairo_set_line_width(canvas.GetContext(), style.read().line.TripleMiddleWidth());
            canvas.singleLine(pipe_body_line, style.read().line); //middle line
        }
        canvas.UnClip();
        canvas.SetLineJoin(t);
    }
    if (text)
        parsed_label.Draw(canvas, sx_text, dx_text, y_text);
}

void ArcPipeSeries::Draw(Canvas &canvas, EDrawPassType pass)
{
    if (!valid) return;
    //First shadows
    for (auto i = series.begin(); i!=series.end(); i++)
        (*i)->DrawPipe(canvas, pass, false, false, false, true, false, 0, drawing_variant);  //dummy 0
    for (auto i = series.begin(); i!=series.end(); i++) {
        //Dont draw the topside fill
        //Draw the topside line only if pipe is fully transparent. Else we may cover the line.
        //Draw the backside in any case.
        //Do not draw text
        auto i_next = i; i_next++;
        const double next_linewidth = i_next!=series.end() ? (*i_next)->style.read().line.width.second : 0;
        (*i)->DrawPipe(canvas, pass, false, (*i)->style.read().solid.second == 0, true, false, false, next_linewidth, drawing_variant);
    }
    if (content.size()) {
        if (pass==DRAW_AFTER_ENTITY_LINES)
            for (auto i = series.begin(); i!=series.end(); i++) 
                if ((*i)->drawEntityLines)
                    chart->DrawEntityLines(canvas, yPos, total_height, (*i)->src, ++EIterator((*i)->dst));
        chart->DrawArcList(canvas, content, chart->GetTotal().y, pass);
    }
    for (auto i = series.begin(); i!=series.end(); i++) {
        //Draw the topside fill only if the pipe is not fully transparent.
        //Draw the topside line in any case
        //Do not draw the backside (that may content arrow lines already drawn)
        //Draw the text
        auto i_next = i; i_next++;
        const double next_linewidth = i_next!=series.end() ? (*i_next)->style.read().line.width.second : 0;
        (*i)->DrawPipe(canvas, pass, (*i)->style.read().solid.second > 0, true, false, false, true, next_linewidth, drawing_variant);
    }
}

//////////////////////////////////////////////////////////////////////////////////////

ArcDivider::ArcDivider(EArcType t, Msc *msc) :
    ArcLabelled(t, MscProgress::DIVIDER, msc, msc->Contexts.back().styles[MyStyleName(t)]),
    nudge(t==MSC_COMMAND_NUDGE),
    title(t==MSC_COMMAND_TITLE || t==MSC_COMMAND_SUBTITLE),
    wide(false),
    extra_space(t==MSC_ARC_DISCO ? msc->discoVgap :
                t==MSC_COMMAND_TITLE ? msc->titleVgap :
                t==MSC_COMMAND_SUBTITLE ? msc->subtitleVgap :
                0)
{
}

const char *ArcDivider::MyStyleName(EArcType t)
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
    defaultDesign.styles.GetStyle(title ? "title" : "divider").read().AttributeNames(csh);
}

bool ArcDivider::AttributeValues(const std::string attr, Csh &csh, bool nudge, bool title)
{
    if (nudge) return false;
    if (defaultDesign.styles.GetStyle(title ? "title" : "divider").read().AttributeValues(attr, csh)) return true;
    if (ArcLabelled::AttributeValues(attr, csh)) return true;
    return false;
}

ArcBase* ArcDivider::PostParseProcess(Canvas &canvas, bool hide, EIterator &left, EIterator &right, 
                                      Numbering &number, Element **target, ArcBase *vertical_target)
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
    ArcLabelled::PostParseProcess(canvas, hide, left, right, number, target, vertical_target);

    if (hide) return NULL;
    return this;
}

void ArcDivider::Width(Canvas &, EntityDistanceMap &distances, DistanceMapVertical &vdist)
{
    if (!valid) return;
    //Add a new element to vdist
    vdist.InsertElementTop(this);
    //Add activation status right away
    AddEntityLineWidths(vdist);
    vdist.InsertEntity(chart->LSide->index);
    vdist.InsertEntity(chart->RSide->index);
    //Add a new element to vdist
    vdist.InsertElementBottom(this);
    //Add activation status right away
    AddEntityLineWidths(vdist);
    if (nudge || !valid || parsed_label.getTextWidthHeight().y==0)
        return;
    //Get marging from lside and rside
    text_margin = wide ? 0 : chart->XCoord(MARGIN)*2;
    if (title) 
        text_margin += style.read().line.LineWidth();
    //calculate space requirement between lside and rside
    const double width = 2*text_margin + parsed_label.getSpaceRequired();
    if (width>0)
        distances.Insert(chart->LSide->index, chart->RSide->index, width);
}

//Dividers that are there just for vspace (without text or line or disco)
//do not have any cover - only mainline. This will make them
//allocate space correctly (even with compress) even if used inside a parallel
//block (there the mainlines affect only elements in the same column, whereas
//covers affect all - using the new parallel layout implemented in
//Msc::PlaceArcLists().
void ArcDivider::Layout(Canvas &canvas, AreaList *cover)
{
    height = 0;
    if (!valid) return;
    yPos = 0;
    if (nudge) {
        Block b(chart->GetDrawing().x.from, chart->GetDrawing().x.till, 0, chart->nudgeSize);
        area.mainline = area = b;
        area.arc = this;
        if (cover) {
            Area cover_area(this);
            cover_area.mainline = b;
            //nothing in "cover_area" just a mainline
            *cover =  cover_area;
        }
        height = chart->nudgeSize;
        LayoutComments(canvas, cover);
        return;
    }
    double y = wide ? 0 : chart->arcVGapAbove;
    y += extra_space;
    if (title)
        y += style.read().line.LineWidth();
    const double charheight = style.read().text.getCharHeight(canvas);
    //reflow text if needed
    if (parsed_label.IsWordWrap()) {
        const double overflow = parsed_label.Reflow(canvas, chart->GetDrawing().x.Spans() - 2*text_margin);
        OverflowWarning(overflow, "", chart->AllEntities.begin(), chart->AllEntities.begin());
    } else {
        CountOverflow(chart->GetDrawing().x.Spans() - 2*text_margin);
    }
    XY wh = parsed_label.getTextWidthHeight();
    if (!wh.y) wh.y = charheight;
    centerline = y+wh.y/2;
    line_margin = chart->XCoord(MARGIN);
    text_cover = parsed_label.Cover(chart->GetDrawing().x.from + text_margin, chart->GetDrawing().x.till-text_margin, y);
    area = text_cover;
    area_important = area;
    const double lw = style.read().line.LineWidth();
    //Add a cover block for the line, if one exists
    if (!title && style.read().line.type.second != LINE_NONE && 
        style.read().line.color.second.type!=ColorType::INVALID && 
        style.read().line.color.second.a>0)
        area += Block(chart->GetDrawing().x.from + line_margin, chart->GetDrawing().x.till - line_margin,
                      centerline - style.read().line.LineWidth()*2, centerline + style.read().line.LineWidth()*2);
    else if (title && (style.read().line.type.second != LINE_NONE || style.read().fill.color.second.type!=ColorType::INVALID))
        area += Block(chart->GetDrawing().x.from + text_margin-lw, chart->GetDrawing().x.till - text_margin+lw,
                      y-lw, y+wh.y+lw);
    else if (area.IsEmpty() && type!=MSC_ARC_VSPACE)
        //for VSPACE with no text and line (and thus empty 'area' so far) we only add a mainline
        area = Block(chart->GetDrawing().x.from, chart->GetDrawing().x.till, 
                     centerline-chart->nudgeSize/2, centerline+chart->nudgeSize/2);
    area.arc = this;

    height = wh.y + 2*extra_space + (wide ? 0 : chart->arcVGapAbove+chart->arcVGapBelow) ;
    if (title)
        height += 2*style.read().line.LineWidth() + style.read().shadow.offset.second;
    //Discontinuity lines cannot be compressed much
    if (type==MSC_ARC_DISCO || title)
        area.mainline = Block(chart->GetDrawing().x.from, chart->GetDrawing().x.till, 
                              wide ? 0 : chart->arcVGapAbove, height- (wide ? 0 :chart->arcVGapBelow));
    else
        area.mainline = Block(chart->GetDrawing().x.from, chart->GetDrawing().x.till, 
                              centerline-charheight/2, centerline+charheight/2);
    chart->NoteBlockers.Append(this);
    if (cover)
        *cover = GetCover4Compress(area);
    LayoutComments(canvas, cover);
}

void ArcDivider::ShiftBy(double y)
{
    if (!valid) return;
    if (!nudge)
        text_cover.Shift(XY(0,y));
    ArcLabelled::ShiftBy(y);
}

void ArcDivider::PostPosProcess(Canvas &canvas)
{
    if (!valid) return;
    if (!nudge)
        chart->HideEntityLines(text_cover);
    if (type == MSC_ARC_DISCO) 
        entityLineRange = area.mainline.GetBoundingBox().y; //will never be compressed away
    else
        entityLineRange = area.GetBoundingBox().y;
    ArcLabelled::PostPosProcess(canvas);
}

void ArcDivider::Draw(Canvas &canvas, EDrawPassType pass)
{
    if (!valid) return;
    if (pass!=draw_pass) return;
    if (nudge) return;
    if (title) {
        const Block outer(chart->GetDrawing().x.from + text_margin, chart->GetDrawing().x.till - text_margin,
                            yPos+extra_space, yPos+height-style.read().shadow.offset.second-extra_space);
        canvas.Shadow(outer, style.read().line, style.read().shadow);
        canvas.Fill(outer.CreateExpand(-style.read().line.LineWidth()/2-style.read().line.Spacing()), style.read().line, style.read().fill);
        canvas.Line(outer.CreateExpand(-style.read().line.LineWidth()/2), style.read().line);
    }
    parsed_label.Draw(canvas, chart->GetDrawing().x.from + text_margin, chart->GetDrawing().x.till - text_margin, 
                      yPos + (wide ? 0 : chart->arcVGapAbove+extra_space));
    if (title) return;
    //determine widest extent for coverage at the centerline+-style.read().line.LineWidth()/2;
    const double lw2 = ceil(style.read().line.LineWidth()/2.);
    Block b(chart->GetDrawing().x.from+line_margin, chart->GetDrawing().x.till-line_margin, 
            yPos + centerline - lw2, yPos + centerline + lw2);
    Range r = (text_cover * b).GetBoundingBox().x;
    if (r.IsInvalid())
        canvas.Line(XY(chart->GetDrawing().x.from+line_margin, yPos + centerline), XY(chart->GetDrawing().x.till-line_margin, yPos + centerline), style.read().line);
    else {
        canvas.Line(XY(chart->GetDrawing().x.from+line_margin, yPos + centerline), XY(r.from-chart->boxVGapInside, yPos + centerline), style.read().line);
        canvas.Line(XY(r.till+chart->boxVGapInside, yPos + centerline), XY(chart->GetDrawing().x.till-line_margin, yPos + centerline), style.read().line);
    }
}

//////////////////////////////////////////////////////////////////////////////////////

ArcParallel* ArcParallel::AddArcList(ArcList*l) 
{
    if (l) {
        //If the container grows we cannot simply use push_back
        //As ArcList does not support copy of if this array is responsible
        //and not empty (as none of the arcs do)
        //So we work around to prevent destroying ArcLists with content
        if (blocks.size() == blocks.capacity()) {
            std::vector<ArcList> tmp;
            tmp.reserve(blocks.size()*2+1);
            for (auto &arcList : blocks) {
                tmp.push_back(ArcList(arcList.responsible));
                tmp.rbegin()->splice(tmp.rbegin()->begin(), arcList);
                arcList.clear(); //free the arclist without deleting the arcs
            }
            blocks.swap(tmp);
        }
        blocks.push_back(ArcList(l->responsible)); 
        blocks.rbegin()->splice(blocks.rbegin()->begin(), *l);
        delete l; 
        keep_together = false;
    } 
    return this;
}

EDirType ArcParallel::GetToucedEntities(class EntityList &el) const
{
    EDirType dir = MSC_DIR_INDETERMINATE;
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

ArcBase* ArcParallel::PostParseProcess(Canvas &canvas, bool hide, 
                                       EIterator &left, EIterator &right, 
                                       Numbering &number, Element **target,
                                       ArcBase * /*vertical_target*/)
{
    if (!valid) return NULL;
    for (auto &block : blocks) 
        chart->PostParseProcessArcList(canvas, hide, block, false, 
                                       left, right, number, target);
    if (hide) return NULL;
    //prune empty blocks
    for (size_t i=0; i < blocks.size(); /*nope*/)
        if (blocks[i].size())
            i++;
        else 
            blocks.erase(blocks.begin()+i);
    return blocks.size() ? this : NULL;
}

void ArcParallel::FinalizeLabels(Canvas &canvas)
{
    for (auto i=blocks.begin(); i != blocks.end(); i++)
        chart->FinalizeLabelsArcList(*i, canvas);
    ArcBase::FinalizeLabels(canvas);
}

void ArcParallel::Width(Canvas &canvas, EntityDistanceMap &distances, DistanceMapVertical &vdist)
{
    if (!valid) return;
    //Add a new element to vdist
    vdist.InsertElementTop(this);
    //Add activation status right away
    AddEntityLineWidths(vdist);
    EntityDistanceMap d;
    for (auto i=blocks.begin(); i != blocks.end(); i++)
        chart->WidthArcList(canvas, *i, d, vdist);
    d.CombinePairedLeftRightToPair_Sum(chart->hscaleAutoXGap);
    d.CombineUnPairedLeftRightToPair(chart->hscaleAutoXGap);
    d.CombineBoxSideToPair(chart->hscaleAutoXGap);
    distances += d;
    //Add a new element to vdist
    vdist.InsertElementBottom(this);
    //Add activation status right away
    AddEntityLineWidths(vdist);
}

void ArcParallel::Layout(Canvas &canvas, AreaList *cover)
{
    height = 0;
    if (!valid) return;
    if (chart->simple_arc_parallel_layout) {
        for (auto i=blocks.begin(); i != blocks.end(); i++) {
            //Each parallel block is compressed without regard to the others
            double h = chart->LayoutArcList(canvas, *i, cover);
            height = std::max(height, h);
        }
    } else {
        std::vector<double> heights = chart->LayoutArcLists(canvas, blocks, cover);
        for (unsigned u = 0; u<blocks.size(); u++)
            height = std::max(height, heights[u]);
    }
    //Do not expand cover, it has already been expanded
    LayoutComments(canvas, cover);
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


double ArcParallel::SplitByPageBreak(Canvas &canvas, double netPrevPageSize,
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
            while (list[0]->size() >0 && list[1]->size() > 0) {
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
    //We may return -1 if all of us would move to the next page
    return chart->PageBreakArcList(canvas, blocks[0], netPrevPageSize,
                                   pageBreak, addCommandNewpage, addHeading,
                                   false, true);
}

void ArcParallel::PlaceWithMarkers(Canvas &canvas)
{
    if (!valid) return;
    int n=0;
    //For automarker, give the bottom of the largest blocks
    for (auto i=blocks.begin(); i!=blocks.end(); i++, n++)
        chart->PlaceWithMarkersArcList(canvas, *i);
}

void ArcParallel::PostPosProcess(Canvas &canvas)
{
    if (!valid) return;
    ArcBase::PostPosProcess(canvas);
    for (auto i=blocks.begin(); i!=blocks.end(); i++)
        chart->PostPosProcessArcList(canvas, *i);
}

void ArcParallel::Draw(Canvas &canvas, EDrawPassType pass)
{
    if (!valid) return;
    for (auto i=blocks.begin(); i != blocks.end(); i++)
        chart->DrawArcList(canvas, *i, chart->GetTotal().y, pass);
}

