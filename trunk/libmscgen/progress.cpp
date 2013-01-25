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

/** @file progress.cpp The implementation of progress reporting functions
 * @ingroup libmscgen_files */

#include <fstream>
#include "progress.h"

const double MscProgress::default_arc_ticks[MAX_CATEGORY][MAX_ARC_SECTION] =
{
//   POPA FILA  WID  LAY MARK  NOTE POPO DRAW
    {   0,   0,   0,   0,   0,    0,   0,   0},  /* NO_CATEGORY */
    {   0,   0,   1,   1,   0,    0,   0,   1},  /* INDICATOR */
    {   2,   3,   3,   3,   0,    0,   1,   3},  /* SELF_ARROW */
    {  10,   3,  11,  21,   0,    0,  10,  21},  /* DIR_ARROW */
    {  10,   3,  21,  31,   0,    0,  20,  41},  /* BLOCK_ARROW */
    {  10,   3,  21,   0,  31,    0,  20,  41},  /* VERTICAL */
    {  10,   3,   0,   0,   0,    0,   0,   0},  /* BOX */
    {  10,   0,  30,  30,   0,    0,  40,  40},  /* BOX_SERIES */
    {  10,   3,   0,   1,   0,    0,   0,   1},  /* PIPE */
    {  10,   0,  20,  50,   0,    0,  30,  50},  /* PIPE_SERIES */
    {  10,   3,   5,   5,   0,    0,   5,   5},  /* DIVIDER */
    {   0,   0,   0,   0,   0,    0,   0,   0},  /* PARALLEL */
    {  20,   0,  11,  11,   0,    0,  10,  11},  /* ENTITY */
    {   5,   0,   5,   5,   0,    0,   5,   0},  /* NEWPAGE */
    {   0,   0,   0,   0,   0,    0,   1,   0},  /* BACKGROUND */
    {   0,   0,   0,   0,   0,    0,   0,   0},  /* NUMBERING */
    {   0,   0,   0,   1,   0,    0,   0,   0},  /* MARKER */
    {   5,   0,   5,   3,   0,    0,   2,   3},  /* EMPTY */
    {   0,   0,   0,   0,   0,    0,   0,   0},  /* HSPACE */
    {   0,   0,   0,   0,   0,    0,   0,   0},  /* VSPACE */
    {   5,   3,   5,   3,   0,    0,   0,   2},  /* SYMBOL */
    {   5,   3,   0,   0,   0,    0,   0,   2},  /* COMMENT */
    {   1,   3,   0,   0,   0,  200,  10,  11},  /* NOTE */
    {   0,   0,   0,   0,   0,    0,   0,   0},  /* LIST */
    {   0,   0,   0,   0,   0,    0,   0,   0}   /* REMAINDER */
};

const double MscProgress::default_relative_effort[MAX_BULK_SECTION] =
{ };

MscProgress::MscProgress(ProgressCallback cb, void *d, double g) : 
    loaded_arc_ticks(MAX_CATEGORY, std::vector<double>(MAX_ARC_SECTION, 0)),
    counted_arc_ticks(MAX_CATEGORY, std::vector<clock_t>(MAX_ARC_SECTION, 0)),
    loaded_arc_number(MAX_CATEGORY, 0),
    total_loaded_arc_ticks(0),
    total_done_arc_ticks(0),
    loaded_relative_effort(MAX_BULK_SECTION, 0),
    counted_relative_effort(MAX_BULK_SECTION, 0),
    arc_items_regsitered(MAX_CATEGORY, 0), 
    arc_items_done_in_current_section(MAX_CATEGORY, 0),
    bulk_items_regsitered(MAX_BULK_SECTION, 0),
    bulk_items_done(MAX_BULK_SECTION, 0),
    current_arc_section(NO_ARC_SECTION), 
    current_bulk_section(STARTUP),
    started(clock()), 
    section_started(started), 
    item_started(started),
    total_done_progress(0),
    last_reported(-101),
    granularity(g), 
    callback(cb), 
    data(d)
{
    Read("load.txt");
    for (unsigned u = 1; u<MAX_ARC_SECTION; u++) 
        arc_items_regsitered[REMAINDER] = 1;
    Callback();
};    


void MscProgress::RegisterBulk(EBulkSection section, unsigned len)
{
    bulk_items_regsitered[section] += len;
}

void MscProgress::RegisterArc(ECategory category, unsigned num)
{
    arc_items_regsitered[category] += num;
    for (unsigned u = 1; u<MAX_ARC_SECTION; u++)
        total_loaded_arc_ticks += num*loaded_arc_ticks[category][u];
}


void MscProgress::UnRegisterArc(ECategory category, unsigned num)
{
    _ASSERT(arc_items_regsitered[category] > 0);
    arc_items_regsitered[category] -= num;
    for (unsigned u = 1; u<MAX_ARC_SECTION; u++)
        total_loaded_arc_ticks -= num*loaded_arc_ticks[category][u];
}

void MscProgress::DoneItem(EArcSection section, ECategory category, unsigned number) 
{
    const clock_t now = clock();
    _ASSERT(section == current_arc_section);
    arc_items_done_in_current_section[category] += number;
    if (current_arc_section != DRAW)
        _ASSERT(arc_items_done_in_current_section[category] <= arc_items_regsitered[category]);
    counted_arc_ticks[category][section] += now - item_started;
    item_started = now;
    total_done_arc_ticks += number*loaded_arc_ticks[category][section];
    total_done_progress += number*loaded_arc_ticks[category][section]*loaded_relative_effort[0]/total_loaded_arc_ticks;
    Callback();
}

void MscProgress::DoneItem(EBulkSection section, unsigned number) 
{
    const clock_t now = clock();
    _ASSERT(section == current_bulk_section);
    bulk_items_done[section] += number;
    _ASSERT(bulk_items_done[section] <= bulk_items_regsitered[section]);
    counted_relative_effort[section] = now - section_started;
    ReCalcSectionDoneLoad();
    Callback();
}

void MscProgress::ReCalcSectionDoneLoad()
{
    double r = total_done_arc_ticks * loaded_relative_effort[0];
    for (unsigned u = 1; u<MAX_BULK_SECTION; u++)
        r += double(bulk_items_done[u])/bulk_items_regsitered[u] * 
                 loaded_relative_effort[u];
    total_done_progress = r;
}

void MscProgress::CloseSection()
{
    const clock_t now = clock();
    //Close previous section (if any)
    if (current_arc_section != NO_ARC_SECTION) {
        DoneItem(current_arc_section, REMAINDER);
        if (current_arc_section!=NOTES && current_arc_section!=DRAW)
            for (unsigned u=1; u<MAX_CATEGORY; u++)
                _ASSERT(arc_items_done_in_current_section[u] == arc_items_regsitered[u]);
        current_arc_section = NO_ARC_SECTION;
    }
    if (current_bulk_section != NO_BULK_SECTION) {
        _ASSERT(bulk_items_done[current_bulk_section] == bulk_items_regsitered[current_bulk_section]);
        counted_relative_effort[current_bulk_section] += now - section_started;
        current_bulk_section = NO_BULK_SECTION;
    }
    section_started = now;
    item_started = now;
}


void MscProgress::Callback()
{
    if (!callback) return;
    if (total_done_progress*100 < last_reported+granularity) return;
    callback(last_reported = total_done_progress*100, data);
}

void MscProgress::Done() 
{
    StartSection(MAX_BULK_SECTION);

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
    _ASSERT(section_started - started == total_ticks);
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


bool MscProgress::Write(const char *fn)
{
    std::ofstream out(fn);
    if (out.fail()) return false;
    for (unsigned u = 0; u<MAX_BULK_SECTION; u++) 
        out << loaded_relative_effort[u] << " ";
    out << std::endl;
    for (unsigned u=0; u<MAX_CATEGORY; u++) {
        for (unsigned v=0; v<MAX_ARC_SECTION; v++)
            out << loaded_arc_ticks[u][v] << " ";
        out << loaded_arc_number[u] << std::endl;
    }
    if (out.fail()) return false;
    return true;
}
bool MscProgress::Read(const char *fn)
{
    std::ifstream in(fn);
    if (in.fail()) return false;
    for (unsigned u = 0; u<MAX_BULK_SECTION; u++) 
        in >> loaded_relative_effort[u];
    for (unsigned u=0; u<MAX_CATEGORY; u++) {
        for (unsigned v=0; v<MAX_ARC_SECTION; v++)
            in >> loaded_arc_ticks[u][v];
        in >> loaded_arc_number[u];
    }
    if (in.fail()) return false;
    return true;
}
