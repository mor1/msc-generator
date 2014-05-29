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

/** @file progress.cpp The implementation of progress reporting functions
 * @ingroup libmscgen_files */

#include <sstream>
#include "progress.h"

#ifndef _ASSERT
#define  _ASSERT(A)
#endif

MscProgress::MscProgress(ProgressCallback cb, void *d, double g) : 
    loaded_arc_ticks(MAX_CATEGORY, std::vector<double>(MAX_ARC_SECTION, 0)),
    loaded_arc_number(MAX_CATEGORY, 0),
    total_loaded_arc_ticks(0),
    counted_arc_ticks(MAX_CATEGORY, std::vector<clock_t>(MAX_ARC_SECTION, 0)),
    loaded_relative_effort(MAX_BULK_SECTION, 1.0/MAX_BULK_SECTION),
    counted_relative_effort(MAX_BULK_SECTION, 0),
    arc_items_regsitered(MAX_CATEGORY, 0), 
    arc_items_done_in_current_section(MAX_CATEGORY, 0),
    arc_sections_completed(MAX_ARC_SECTION, false),
    bulk_items_regsitered(MAX_BULK_SECTION, 0),
    bulk_items_done(MAX_BULK_SECTION, 0),
    current_arc_section(NO_ARC_SECTION), 
    current_bulk_section(STARTUP),
    started(clock()), 
    section_started(started), 
    item_started(started),
    last_reported(-101),
    granularity(g), 
    callback(cb), 
    data(d)
{
    for (unsigned u = 1; u<MAX_ARC_SECTION; u++) {
        arc_items_regsitered[REMAINDER] = 1;
        total_loaded_arc_ticks += loaded_arc_ticks[REMAINDER][u];
    }
    loaded_relative_effort[0] = 0.90;  //Arc processing
    loaded_relative_effort[STARTUP] = 0.07;
    loaded_relative_effort[PARSE] = 0.025;
    loaded_relative_effort[AUTOPAGINATE] = 0.005;
    Callback();
};    


void MscProgress::RegisterBulk(EBulkSection section, unsigned len)
{
    bulk_items_regsitered[section] += len;
}

void MscProgress::RegisterArc(ECategory category)
{
    arc_items_regsitered[category]++;
    for (unsigned u = 1; u<MAX_ARC_SECTION; u++)
        total_loaded_arc_ticks += loaded_arc_ticks[category][u];
}


void MscProgress::UnRegisterArc(ECategory category)
{
    _ASSERT(arc_items_regsitered[category] > 0);
    arc_items_regsitered[category]--;
    for (unsigned u = 1; u<MAX_ARC_SECTION; u++)
        total_loaded_arc_ticks -= loaded_arc_ticks[category][u];
}

void MscProgress::DoneItem(EArcSection section, ECategory category) 
{
    const clock_t now = clock();
    //We may call draw multiple times
    //We may delete arcs (content of ArcBox) already processed, due to gouped entity hiding
    //Notes is also exempted. Cant remember, why.
    //Plus we only check if we report something in the current section
    //(At autopaginate we may add autoheaders and do width, etc., when we
    //are already beyond WIDTH section.)
    if (section == current_arc_section && current_arc_section != DRAW && 
        current_arc_section != POST_PARSE && current_arc_section != NOTES) {
        _ASSERT(arc_items_done_in_current_section[category] < arc_items_regsitered[category]);
    }
    if (arc_items_done_in_current_section[category] == arc_items_regsitered[category]) {
        //We have already reported the processing of all registered items.
        //Do nothing now, the time spent processing this extra item will go
        //unaccounted for.
        item_started = now;
        return;
    } else if (arc_items_done_in_current_section[category] < arc_items_regsitered[category]) {
        arc_items_done_in_current_section[category]++;
        counted_arc_ticks[category][section] += now - item_started;
    } else {
        arc_items_done_in_current_section[category] = arc_items_regsitered[category];
    }
    item_started = now;
    Callback();
}

void MscProgress::DoneItem(EBulkSection section, unsigned number) 
{
    //Bulk item reporting is usually precise, thus we do
    //do not cater for overreporting here.
    const clock_t now = clock();
    _ASSERT(section == current_bulk_section);
    bulk_items_done[section] += number;
    _ASSERT(bulk_items_done[section] <= bulk_items_regsitered[section]);
    counted_relative_effort[section] = now - section_started;
    Callback();
}



void MscProgress::CloseSection()
{
    const clock_t now = clock();
    //Close previous section (if any)
    if (current_arc_section != NO_ARC_SECTION) {
        DoneItem(current_arc_section, REMAINDER);
        for (unsigned u=1; u<MAX_CATEGORY; u++) {
            if (current_arc_section!=DRAW  && current_arc_section!=POST_PARSE && current_arc_section != NOTES)
                if (u != NOTE) {
                    _ASSERT(arc_items_done_in_current_section[u] == arc_items_regsitered[u]);
                }
        }
        arc_sections_completed[current_arc_section] = true;
        current_arc_section = NO_ARC_SECTION;
    }
    if (current_bulk_section != NO_BULK_SECTION) {
//        _ASSERT(bulk_items_done[current_bulk_section] == bulk_items_regsitered[current_bulk_section]);
        //bye command causes we do not parse all of the text
        counted_relative_effort[current_bulk_section] += now - section_started;
        current_bulk_section = NO_BULK_SECTION;
    }
    section_started = now;
    item_started = now;
    Callback();
}


void MscProgress::Callback()
{
    if (!callback) return;
    //recalculate total_done_arc_ticks
    register double p = 0, q = 0;
    for (register unsigned u=1; u<MAX_CATEGORY; u++) 
        for (register unsigned v=1; v<MAX_ARC_SECTION; v++) {
            if (v==current_arc_section) 
                p += loaded_arc_ticks[u][v] * arc_items_done_in_current_section[u];
            else if (arc_sections_completed[v])
                p += loaded_arc_ticks[u][v] * arc_items_regsitered[u];
            q += loaded_arc_ticks[u][v] * arc_items_regsitered[u];
        }

    //p is now the total load_tick for arcs weighted by the number
    //of registered arc categories
    p /= q; //now p is the ratio of work done in arc sections
    //now calculate the total progress
    q = loaded_relative_effort[0];
    for (unsigned u = 1; u<MAX_BULK_SECTION; u++)
        if (bulk_items_regsitered[u]) 
            q += loaded_relative_effort[u];
    //q is now the total weight to distribute
    p *= loaded_relative_effort[0]; //weight each section by the relative effort
    for (unsigned u = 1; u<MAX_BULK_SECTION; u++)
        if (bulk_items_regsitered[u]) 
            p += loaded_relative_effort[u] * bulk_items_done[u] / 
                 bulk_items_regsitered[u];
    p*=100/q;
    if (p > last_reported+granularity) 
        callback(last_reported = p, data);
}

void MscProgress::Done() 
{
    CloseSection();

    const double ewma = 0.1;
    //normalize and merge arc_ticks
    double sum_arc_ticks = 0;
    for (unsigned u=0; u<MAX_CATEGORY; u++)
        for (unsigned v=0; v<MAX_ARC_SECTION; v++) {
            sum_arc_ticks += counted_arc_ticks[u][v];
            if (arc_items_regsitered[u]) {
                const double tot = loaded_arc_number[u] + arc_items_regsitered[u];
                loaded_arc_ticks[u][v] = 
                    (loaded_arc_number[u]   /tot) * loaded_arc_ticks[u][v] + 
                    (arc_items_regsitered[u]/tot) * double(counted_arc_ticks[u][v])/arc_items_regsitered[u];
                loaded_arc_number[u] += arc_items_regsitered[u];
            } else {
                _ASSERT(counted_arc_ticks[u][v]==0);
            }
        }
    //calculate load relation between bulk and arc loads & merge
    double total_ticks = sum_arc_ticks;
    for (unsigned u = 1; u<MAX_BULK_SECTION; u++) 
        total_ticks += counted_relative_effort[u];
    double sum = 
        loaded_relative_effort[0] = 
            (1-ewma) * loaded_relative_effort[0] +
               ewma  * sum_arc_ticks/total_ticks;
    for (unsigned u = 1; u<MAX_BULK_SECTION; u++) 
        sum += 
            loaded_relative_effort[u] = 
                (1-ewma) * loaded_relative_effort[u] + 
                   ewma  * counted_relative_effort[u]/total_ticks;
    for (unsigned u = 0; u<MAX_BULK_SECTION; u++) 
        loaded_relative_effort[u] /= sum;
}


std::string MscProgress::WriteLoadData() const
{
    std::stringstream out;
    out << unsigned(0) << " "; //version
    for (unsigned u = 0; u<MAX_BULK_SECTION; u++) 
        out << loaded_relative_effort[u] << " ";
    out << std::endl;
    for (unsigned u=0; u<MAX_CATEGORY; u++) {
        for (unsigned v=0; v<MAX_ARC_SECTION; v++)
            out << loaded_arc_ticks[u][v] << " ";
        out << loaded_arc_number[u] << std::endl;
    }
    return out.str();
}
bool MscProgress::ReadLoadData(const char *inp)
{
    std::stringstream in(inp);
    unsigned ver;
    if (!in.good()) goto fail;
    in >> ver;
    if (ver > 0) goto fail; //too advanced version
    for (unsigned u = 0; u<MAX_BULK_SECTION; u++) 
        if (in.good())
            in >> loaded_relative_effort[u];
        else
            goto fail;
    for (unsigned u=0; u<MAX_CATEGORY; u++) {
        for (unsigned v=0; v<MAX_ARC_SECTION; v++)
            if (in.good())
                in >> loaded_arc_ticks[u][v];
            else 
                goto fail;
        if (in.good())
            in >> loaded_arc_number[u];
        else 
            goto fail;
    }
    return true;
fail:

    return false;
}
