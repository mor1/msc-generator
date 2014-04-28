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
/** @file element.cpp The implementation of class Element.
 * @ingroup libmscgen_files */

#include "msc.h"
using namespace std;

const Context Element::defaultDesign(true);

Element::Element(Msc *m) : chart(m), 
    hidden(false), linenum_final(false), yPos(0), 
    draw_is_different(false), area_draw_is_frame(false),
    comments(false), comment_height(0),
    indicator_style(m->Contexts.back().styles["indicator"]),
    draw_pass(DRAW_DEFAULT) 
{
    area.arc = this;
    control_location.MakeInvalid();
}

Element::~Element() 
{
    if (chart) 
        chart->InvalidateNotesToThisTarget(this);
}


Element::Element(const Element&o) :
    chart(o.chart), hidden(o.hidden), linenum_final(o.linenum_final),
    yPos(o.yPos), area(o.area), area_draw(o.area_draw),
    draw_is_different(o.draw_is_different), area_draw_is_frame(o.area_draw_is_frame), 
    area_to_note(o.area_to_note),
    comments(false), comment_height(0),//we do not copy comments!!!
    area_important(o.area_important), 
    controls(o.controls), control_location(o.control_location),
    indicator_style(o.indicator_style),
    draw_pass(o.draw_pass), file_pos(o.file_pos)
{
    area.arc = this;
}

/** Record the location of the element in the input file
 * @param [in] l The range the element occupies in the input file to record.
 * @param [in] f If true, the recording is final - any more calls to SetLineEnd() will be ignored.*/
void Element::SetLineEnd(FileLineColRange l, bool f)
{
    if (linenum_final) return;
    linenum_final = f;
    file_pos = l;
}

/** Attach a comment to us.
 * The caller will remain responsible do delete `cn`, when the time comes.*/
void Element::AttachComment(CommandNote *cn)
{
    _ASSERT(cn);
    _ASSERT(!cn->is_float);
    comments.Append(cn);
}

/** Move the comments from `te` to us.
 * Comments are moved in order after our comments (if any).*/
void Element::CombineComments(Element *te)
{
    _ASSERT(te);
    if (te)
        comments.splice(comments.end(), te->comments);
}

/** Textual representation of drawing passes*/
template<> const char EnumEncapsulator<EDrawPassType>::names[][ENUM_STRING_LEN] =
    {"invalid", "before_entity_lines", "after_entity_lines", "default", "after_default", 
     "note", "after_note", ""};


/** Applies an Attribute to us.
 * We only accept `draw_time` attribute.
 * @param [in] a The attribute to add.
 * @return true if we recognized the attribute as ours.*/
bool Element::AddAttribute(const Attribute &a)
{
    if (a.Is("draw_time")) {
        if (!a.EnsureNotClear(chart->Error, STYLE_ARC)) return true;
        if (a.type == MSC_ATTR_STRING && Convert(a.value, draw_pass)) return true;
        a.InvalidValueError(CandidatesFor(draw_pass), chart->Error);
        return true;
    }
    return false;
}


/** Add the attribute names we take to `csh`.*/
void Element::AttributeNames(Csh &csh)
{
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "draw_time", HINT_ATTR_NAME));
}

/** Add a list of possible attribute value names to `csh` for attribute `attr`.*/
bool Element::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr,"draw_time")) {
        csh.AddToHints(EnumEncapsulator<EDrawPassType>::names, csh.HintPrefix(COLOR_ATTRVALUE), 
                       HINT_ATTR_VALUE);
        return true;
    }
    return false;
}

/** Shift an elememt up or down 
 * Shifts all `area_*` members, `yPos`, `control_location` and
 * all our comments;
 */
void Element::ShiftBy(double y)
{
    if (y==0) return;
    area.Shift(XY(0, y));
    area_draw.Shift(XY(0, y));
    area_to_note.Shift(XY(0,y));
    area_to_note2.Shift(XY(0,y));
    area_important.Shift(XY(0,y)); 
    yPos+=y;
    control_location.y += y;
    for (auto c : comments)
        c->ShiftCommentBy(y);
}

/** A helper placing comments.
 * @param canvas The canvas used to determine comment geometry
 * @param cover We add the cover of the placed comments to this, if not NULL.
 * @param l At call says where we shall start laying out left comments from, at return the height of he comments on the left.
 * @param r At call says where we shall start laying out right comments from, at return the height of he comments on the right.
 */
void Element::LayoutCommentsHelper(Canvas &canvas, AreaList *cover, double &l, double &r)
{
    for (auto c : comments)
        c->PlaceSideTo(canvas, cover, c->GetStyle().read().side.second == ESide::LEFT ? l : r);
    comment_height = std::max(l, r);
}


/** Do processing after our positioning on the chart is known.
 * We expand `area` and `area_draw` by `chart->trackExpandBy`
 * if we show; set `control_location` and register us in
 * chart->AllArcs.
 * We do nothing if `chart->prepare_for_tracking` is false.*/
void Element::PostPosProcess(Canvas &/*canvas*/)
{
    if (!chart->prepare_for_tracking) 
        return;
    if (!area.IsEmpty() && !hidden) {
        //TODO: Pipe segments suck here, so if expand cannot do it,
        //we still keep the original stuff.
        Area expanded_area = area.CreateExpand(chart->trackExpandBy, 
                                               contour::EXPAND_MITER_ROUND, 
                                               contour::EXPAND_MITER_ROUND, 
                                               2, 2);
        if (!expanded_area.IsEmpty())
            area = expanded_area;
        else {
            _ASSERT(0);
        }
        area.arc = this;
        chart->AllCovers += area;
        //Determine, where the controls shall be shown
        control_location.x = Range(area.GetBoundingBox().x.till, 
                                   area.GetBoundingBox().x.till + control_size.x);
        control_location.y = Range(area.GetBoundingBox().y.from, 
                                   area.GetBoundingBox().y.from + control_size.y*controls.size());
    } else {
        //remove controls if we cannot pinpoint a location for them
        controls.clear();
    }
    if (!hidden && draw_is_different && !area_draw.IsEmpty() && !area_draw_is_frame)
        area_draw = area_draw.CreateExpand(chart->trackExpandBy, 
                                           contour::EXPAND_MITER_ROUND, 
                                           contour::EXPAND_MITER_ROUND, 
                                           2, 2);
    if (!file_pos.IsInvalid())
        chart->AllArcs[file_pos] = this;
}


const XY Element::control_size = XY(25, 25);

/** Draw GUI controls (if any)
 * @param cr The cairo context to draw the controls to. 
 *           The transformation matrix shall be so that we can draw on chart space.
 * @param [in] size A number between [0.01..1] indicating a scale factor. */
void Element::DrawControls(cairo_t *cr, double size) const
{
    if (size<0.01 || size>1 || controls.size()==0 || cr==NULL) return;
    cairo_save(cr);
    XY center = control_location.UpperLeft() + control_size/2;
    cairo_translate(cr, center.x, center.y);
    cairo_scale(cr, size, size);
    LineAttr l_rect(LINE_SOLID, ColorType(0,0,0), 2, CORNER_ROUND, (control_size.x+control_size.y)/10);
    FillAttr f_rect(ColorType(0,0,0), ColorType(64,64,64), GRADIENT_DOWN);
    ShadowAttr s_rect(ColorType(0,0,0));
    s_rect.offset.first = s_rect.blur.first = true;
    s_rect.offset.second = 5;
    s_rect.blur.second = 5;
    for (auto j = controls.begin(); j!=controls.end(); j++) {
        Contour rect = l_rect.CreateRectangle_Midline(-control_size.x/2.1, control_size.x/2.1, 
                                                      -control_size.y/2.1, control_size.y/2.1);
        cairo_set_source_rgb(cr, 1,1,1);
        rect.Fill(cr);
        cairo_set_source_rgb(cr, 0,0,0);
        cairo_set_line_width(cr, 2);
        rect.Line(cr);
        switch (*j) {
        default:
            _ASSERT(0);
            break;
        case MSC_CONTROL_EXPAND:
        case MSC_CONTROL_COLLAPSE:
            cairo_set_line_width(cr, (control_size.x+control_size.y)/10);
            cairo_set_source_rgb(cr, 1,0,0);
            cairo_move_to(cr, -control_size.x*0.25, 0);
            cairo_line_to(cr, +control_size.x*0.25, 0);
            if (*j == MSC_CONTROL_EXPAND) {
                cairo_new_sub_path(cr);
                cairo_move_to(cr, 0, -control_size.x*0.25);
                cairo_line_to(cr, 0, +control_size.x*0.25);
            }
            cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
            cairo_stroke(cr);
            break;
        case MSC_CONTROL_ARROW:
            XY points[] = {XY(-control_size.x*0.25, -control_size.y*0.10),
                           XY(-control_size.x*0.10, -control_size.y*0.10),
                           XY(-control_size.x*0.10, -control_size.y*0.25),
                           XY( control_size.x*0.25, 0),
                           XY(-control_size.x*0.10,  control_size.y*0.25),
                           XY(-control_size.x*0.10,  control_size.y*0.10),
                           XY(-control_size.x*0.25,  control_size.y*0.10)  
                          };
            Contour arrow;
            arrow.assign_dont_check(points);
            //LineAttr line(LINE_SOLID, ColorType(0,0,0), 1, CORNER_NONE, 0);
            //FillAttr fill(ColorType(0,128,0), GRADIENT_UP);
            cairo_set_source_rgb(cr, 0.2, 0.5, 0.2);
            arrow.Fill(cr);
            cairo_set_source_rgb(cr, 0, 0, 0);
            arrow.Line(cr);
            break;
        }
        //move down to next control location
        cairo_translate(cr, 0, control_size.y/size);
    }
    cairo_restore(cr);
}

/** Return the type of GUI control `xy` points to.
 * @param [in] xy The coordinates are in chart space.
 * @returns The type of control or MSC_CONTROL_INVALID, if xy does not point to any control.*/
EGUIControlType Element::WhichControl(const XY &xy)
{
    if (!control_location.IsWithinBool(xy)) return MSC_CONTROL_INVALID;
    return controls[unsigned((xy.y - control_location.y.from)/control_size.y)];
}


const XY Element::indicator_size = XY(25, 10);

/** Return the outer Edge of indicators.
 * @param [in] pos The middle of the top edge of the indicator shall be here.
 * @returns The outer edge.*/
Block Element::GetIndicatorCover(const XY &pos)
{
    Block b(pos.x-indicator_size.x/2, pos.x+indicator_size.x/2,
            pos.y, pos.y+indicator_size.y);
    return b.Expand(indicator_style.read().line.LineWidth());
}

/** Draw an indicator.
 * @param [in] pos The middle of the top edge of the indicator shall be here.
 * @param canvas The canvas to draw on.*/
void Element::DrawIndicator(XY pos, Canvas *canvas)
{
    if (canvas==NULL) return;

    const Block area = GetIndicatorCover(pos);
    canvas->Shadow(area, indicator_style.read().line, indicator_style.read().shadow);
    canvas->Fill(area, indicator_style.read().line, indicator_style.read().fill);
    canvas->Line(area, indicator_style.read().line);

    cairo_save(canvas->GetContext());
    cairo_set_line_cap(canvas->GetContext(), CAIRO_LINE_CAP_ROUND);
    LineAttr line(indicator_style.read().line);
    line.width.second = area.y.Spans()/4;
    line.type.second = LINE_SOLID;
    pos.y += indicator_size.y/2;
    canvas->Line(pos, pos, line);
    const XY offset(area.x.Spans()/4, 0);
    canvas->Line(pos-offset, pos-offset, line);
    canvas->Line(pos+offset, pos+offset, line);
    cairo_restore(canvas->GetContext());
}

