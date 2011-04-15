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
#include "msc.h"
using namespace std;

TrackableElement::TrackableElement(Msc *m) : chart(m), 
    hidden(false), linenum_final(false),  yPos(0),
    draw_is_different(false), area_draw_is_frame(false),
    indicator(false), indicator_style(m->Contexts.back().styles["indicator"])
{
    area_draw.arc = area.arc = this;
    control_location.MakeInvalid();
    if (m && m->Contexts.size()) 
        indicator = m->Contexts.back().indicator;
}


TrackableElement::TrackableElement(const TrackableElement&o) :
    hidden(o.hidden), linenum_final(o.linenum_final), area(o.area), yPos(o.yPos),
    area_draw(o.area_draw), draw_is_different(o.draw_is_different),
    area_draw_is_frame(o.area_draw_is_frame), chart(o.chart),
    controls(o.controls), control_location(o.control_location),
    indicator(o.indicator)
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
    control_location.y += y;
}

void TrackableElement::PostPosProcess(double)
{
    if (!area.IsEmpty()&& !hidden) {
        //TODO: Pipe segments suck here, so if expand cannot do it,
        //we still keep the original stuff.
        Area expanded_area = area.CreateExpand(chart->trackExpandBy);
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
        area_draw = area_draw.CreateExpand(chart->trackExpandBy);
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
        Area rect = l_rect.CreateRectangle(-control_size.x/2.1, control_size.x/2.1, 
                                                -control_size.y/2.1, control_size.y/2.1);
        cairo_set_source_rgb(cr, 1,1,1);
        rect.Fill(cr);
        cairo_set_source_rgb(cr, 0,0,0);
        cairo_set_line_width(cr, 2);
        rect.Line(cr);
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
        //move down to next control location
        cairo_translate(cr, 0, control_size.y/size);
    }
    cairo_restore(cr);
}

MscControlType TrackableElement::WhichControl(const XY &xy)
{
    if (control_location.IsWithin(xy)!=WI_INSIDE) return MSC_CONTROL_INVALID;
    return controls[(xy.y - control_location.y.from)/control_size.y];
}


const XY TrackableElement::indicator_size = XY(25, 10);

//The outer Edge of indicators
Block TrackableElement::GetIndicatorCover(const XY &pos)
{
    Block b(pos.x-indicator_size.x/2, pos.x+indicator_size.x/2,
            pos.y, pos.y+indicator_size.y);
    return b.Expand(indicator_style.line.LineWidth()/2);
}

void TrackableElement::DrawIndicator(XY pos, MscCanvas *canvas)
{
    if (canvas==NULL) return;

    MscLineAttr line = indicator_style.line;
    Block area = GetIndicatorCover(pos);
    canvas->Shadow(area, line, indicator_style.shadow);
    area.Expand(-line.LineWidth()+line.width.second/2);
    line.radius.second -= line.LineWidth()-line.width.second/2;
    canvas->Fill(area, line, indicator_style.fill);
    area.Expand(line.LineWidth()/2-line.width.second/2);
    line.radius.second += line.LineWidth()/2-line.width.second/2;
    canvas->Line(area, line);

    cairo_save(canvas->GetContext());
    cairo_set_line_cap(canvas->GetContext(), CAIRO_LINE_CAP_ROUND);
    line.width.second = area.y.Spans()/4;
    line.type.second = LINE_SOLID;
    pos.y += indicator_size.y/2;
    canvas->Line(pos, pos, line);
    const XY offset(area.x.Spans()/4, 0);
    canvas->Line(pos-offset, pos-offset, line);
    canvas->Line(pos+offset, pos+offset, line);
}

