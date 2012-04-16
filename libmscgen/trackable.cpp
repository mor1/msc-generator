/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011,2012 Zoltan Turanyi
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
using namespace std;

//Generate a plain design with all the default styles (and colors)
//will be used by ::AddAttributeNames to see which style components an element has
//will also be used by CshContext::SetPlain to take default style and color names/defs
const Context TrackableElement::defaultDesign(true);

TrackableElement::TrackableElement(Msc *m) : chart(m), 
    hidden(false), linenum_final(false),  draw_pass(DRAW_DEFAULT), yPos(0),
    draw_is_different(false), area_draw_is_frame(false),
    comments(false),
    indicator_style(m->Contexts.back().styles["indicator"])
{
    area.arc = this;
    control_location.MakeInvalid();
}

TrackableElement::~TrackableElement() 
{
    if (chart) 
        chart->InvalidateNotesToThisTarget(this);
}

//This does not copy comments
TrackableElement::TrackableElement(const TrackableElement&o) :
    chart(o.chart), hidden(o.hidden), linenum_final(o.linenum_final),
    draw_pass(o.draw_pass),
    area(o.area), yPos(o.yPos), area_draw(o.area_draw),
    draw_is_different(o.draw_is_different), area_draw_is_frame(o.area_draw_is_frame), 
    area_important(o.area_important), 
    controls(o.controls), control_location(o.control_location)
{
    area.arc = this;
}

void TrackableElement::SetLineEnd(file_line_range l, bool f)
{
    if (linenum_final) return;
    linenum_final = f;
    file_pos = l;
}

void TrackableElement::AttachComment(CommandNote *cn)
{
    _ASSERT(cn);
    _ASSERT(!cn->is_float);
    comments.Append(cn);
}

//move comments to us    
void TrackableElement::CombineComments(TrackableElement *te)
{
    _ASSERT(te);
    if (te)
        comments.splice(comments.end(), te->comments);
}

template<> const char EnumEncapsulator<DrawPassType>::names[][ENUM_STRING_LEN] =
    {"invalid", "before_entity_lines", "after_entity_lines", "default", "after_default", 
     "note", "after_note", ""};


bool TrackableElement::AddAttribute(const Attribute &a)
{
    if (a.Is("draw_time")) {
        if (!a.EnsureNotClear(chart->Error, STYLE_ARC)) return true;
        if (a.type == MSC_ATTR_STRING && Convert(a.value, draw_pass)) return true;
        a.InvalidValueError(CandidatesFor(draw_pass), chart->Error);
        return true;
    }
    return false;
}

void TrackableElement::AttributeNames(Csh &csh)
{
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "draw_time", HINT_ATTR_NAME));
}

bool TrackableElement::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr,"draw_time")) {
        csh.AddToHints(EnumEncapsulator<DrawPassType>::names, csh.HintPrefix(COLOR_ATTRVALUE), 
                       HINT_ATTR_VALUE);
        return true;
    }
    return false;
}

void TrackableElement::ShiftBy(double y)
{
    if (y==0) return;
    area.Shift(XY(0, y));
    area_draw.Shift(XY(0, y));
    area_to_note.Shift(XY(0,y));
    area_important.Shift(XY(0,y)); 
    yPos+=y;
    control_location.y += y;
    for (auto n = comments.begin(); n!=comments.end(); n++)
        (*n)->ShiftCommentBy(y);
}

//Here we add to "cover", do not overwrite it
double TrackableElement::NoteHeightHelper(MscCanvas &canvas, AreaList &cover, double &l, double &r)
{
    for (auto n = comments.begin(); n!=comments.end(); n++)
        (*n)->PlaceSideTo(canvas, cover, (*n)->GetStyle().side.second == SIDE_LEFT ? l : r);
    return std::max(l, r);
}


void TrackableElement::PostPosProcess(MscCanvas &/*canvas*/)
{
    if (!area.IsEmpty()&& !hidden) {
        //TODO: Pipe segments suck here, so if expand cannot do it,
        //we still keep the original stuff.
        Area expanded_area = area.CreateExpand(chart->trackExpandBy, 
                                               contour::EXPAND_MITER_ROUND, 
                                               contour::EXPAND_MITER_ROUND, 
                                               2, 2);
        if (!expanded_area.IsEmpty())
            area = expanded_area;
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


const XY TrackableElement::control_size = XY(25, 25);

void TrackableElement::DrawControls(MscCanvas*canvas, double size)
{
    if (size<0.01 || size>1 || controls.size()==0 || canvas==NULL) return;
    cairo_t *cr = canvas->GetContext();
    cairo_save(cr);
    XY center = control_location.UpperLeft() + control_size/2;
    cairo_translate(cr, center.x, center.y);
    cairo_scale(cr, size, size);
    MscLineAttr l_rect(LINE_SOLID, MscColorType(0,0,0), 2, CORNER_ROUND, (control_size.x+control_size.y)/10);
    MscFillAttr f_rect(MscColorType(0,0,0), MscColorType(64,64,64), GRADIENT_DOWN);
    MscShadowAttr s_rect(MscColorType(0,0,0));
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
            Area arrow = Contour(-control_size.x*0.25, -control_size.x*0.1,
                                 -control_size.y*0.1,  control_size.y*0.1);
            arrow += Contour(XY(-control_size.x*0.1, -control_size.y*0.25),
                             XY(-control_size.x*0.1,  control_size.y*0.25),
                             XY(control_size.x*0.25, 0));
            MscLineAttr line(LINE_SOLID, MscColorType(0,0,0), 1, CORNER_NONE, 0);
            MscFillAttr fill(MscColorType(0,128,0), GRADIENT_UP);
            canvas->Fill(arrow, fill);
            canvas->Line(arrow, line);
            break;
        }
        //move down to next control location
        cairo_translate(cr, 0, control_size.y/size);
    }
    cairo_restore(cr);
}

MscControlType TrackableElement::WhichControl(const XY &xy)
{
    if (!inside(control_location.IsWithin(xy))) return MSC_CONTROL_INVALID;
    return controls[unsigned((xy.y - control_location.y.from)/control_size.y)];
}


const XY TrackableElement::indicator_size = XY(25, 10);

//The outer Edge of indicators
Block TrackableElement::GetIndicatorCover(const XY &pos)
{
    Block b(pos.x-indicator_size.x/2, pos.x+indicator_size.x/2,
            pos.y, pos.y+indicator_size.y);
    return b.Expand(indicator_style.line.LineWidth());
}

void TrackableElement::DrawIndicator(XY pos, MscCanvas *canvas)
{
    if (canvas==NULL) return;

    const Block area = GetIndicatorCover(pos);
    canvas->Shadow(area, indicator_style.line, indicator_style.shadow);
    canvas->Fill(area, indicator_style.line, indicator_style.fill);
    canvas->Line(area, indicator_style.line);

    cairo_save(canvas->GetContext());
    cairo_set_line_cap(canvas->GetContext(), CAIRO_LINE_CAP_ROUND);
    MscLineAttr line(indicator_style.line);
    line.width.second = area.y.Spans()/4;
    line.type.second = LINE_SOLID;
    pos.y += indicator_size.y/2;
    canvas->Line(pos, pos, line);
    const XY offset(area.x.Spans()/4, 0);
    canvas->Line(pos-offset, pos-offset, line);
    canvas->Line(pos+offset, pos+offset, line);
    cairo_restore(canvas->GetContext());
}

