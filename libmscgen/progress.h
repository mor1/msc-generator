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

/** @file progress.h The declaration of progress reporting functions
 * @ingroup libmscgen_files */

#if !defined(PROGRESS_H)
#define PROGRESS_H

#include <vector>
#include <ctime>

class MscProgress
{
public:
	typedef bool (*ProgressCallback)(double percent, void*data); //Callback function to report progress at;
    typedef enum {
		NO_BULK_SECTION = 0, ///< The invalid value
        STARTUP,
        PARSE,
		AUTOPAGINATE,   ///<Automatic pagination
        MAX_BULK_SECTION
    } EBulkSection;
	typedef enum {
		NO_ARC_SECTION = 0, ///< The invalid value
		POST_PARSE,     ///<PostParseProcess
		FINALIZE_LABELS,///<FinalizeLabels
		WIDTH,          ///<Width calculation, including entity spacing
		LAYOUT,         ///<Layout calculation
		PLACEWITHMARKERS,///Msc::PlaceWithMarkers
		NOTES,          ///<Note placement
		POST_POS,       ///<Msc::PostPosProcess
		DRAW,           ///<Drawing 
		MAX_ARC_SECTION///<Not used, the max value
	} EArcSection;
	typedef enum {
		NO_CATEGORY = 0,
		INDICATOR,
		SELF_ARROW,
		DIR_ARROW,
		BLOCK_ARROW,
		VERTICAL,
		BOX,
		BOX_SERIES,
		PIPE,
		PIPE_SERIES,
		DIVIDER,
		PARALLEL,
		ENTITY,
		NEWPAGE,
		BACKGROUND,
		NUMBERING,
		MARKER,
		EMPTY,
		HSPACE,
		VSPACE,
		SYMBOL,
		COMMENT,
		NOTE,
		LIST,
        REMAINDER,
        MAX_CATEGORY
	} ECategory;
protected:
    /** Shows the ticks needed to do one section for one arc of a given category 
     * (factory default)*/
    static const double default_arc_ticks[MAX_CATEGORY][MAX_ARC_SECTION]; 
    /** Shows the ticks needed to do one section for one arc of a given category 
     * (loaded from file)*/
    std::vector<std::vector<double>> loaded_arc_ticks;
    /** How many arcs were used to generate the info in `loaded_arc_ticks` */
    std::vector<unsigned> loaded_arc_number;
    /** During progress, counts how many ticks were spent on doing a section 
     * of a category of arcs. Counting is done cumulatively for all arcs of 
     * that category.*/
    double total_loaded_arc_ticks;
    double total_done_arc_ticks;
    std::vector<std::vector<clock_t>> counted_arc_ticks;

    /** Shows the relative effort of processing arc sections (inde of zero) 
     * and bulk sections (index of 1..n). These values sum up to 1. 
     * (factory default)*/
    static const double default_relative_effort[MAX_BULK_SECTION];
    /** Shows the relative effort of processing arc sections (inde of zero) 
     * and bulk sections (index of 1..n). These values sum up to 1. 
     * (loaded from file)*/
    std::vector<double> loaded_relative_effort;
    /** During progress, counts the number ticks spent in doing bulk 
     * sections (cumulative)*/
    std::vector<clock_t> counted_relative_effort;

    /** The number of arcs registered per category */
	std::vector<unsigned> arc_items_regsitered;
    /** The number of arcs already done in this section per category*/
	std::vector<unsigned> arc_items_done_in_current_section;
    /** The number of bulk items registered per section */
	std::vector<unsigned> bulk_items_regsitered;
    /** The number of items processed in the current bulk section*/
    std::vector<unsigned> bulk_items_done;
    /** The current arc section. Either this is set or `current_bulk_section`*/
	EArcSection current_arc_section;
    /** The current bulk section. Either this is set or `current_arc_section`*/
	EBulkSection current_bulk_section;
    /** When the whole processing has started */
	const clock_t started;
    /** When the current section has started */
	clock_t section_started;
    /** When the current item has started */
    clock_t item_started;


    double total_done_progress;
    void ReCalcSectionDoneLoad();

    double last_reported; //Last percentage value reported
    void Callback();      //Report `total_done_progress` if granularity demands it

public:
    /** In percentage, the granularity of calling the callback.*/
    const double granularity;
    /** The callback function to call */
    ProgressCallback callback;
    /** The supplemental value to send to the callback */
    void * data;
	explicit MscProgress(ProgressCallback cb = NULL, void*data=NULL, double g=1);
	void RegisterBulk(EBulkSection section, unsigned len);
	void RegisterArc(ECategory, unsigned num=1); ///<Register an item to be done in the future
	void UnRegisterArc(ECategory, unsigned num=1); ///<Register an item to be done in the future

    void RecalcLoad();

    void CloseSection();
    void StartSection(EArcSection section) {CloseSection(); current_arc_section = section; arc_items_done_in_current_section.assign(MAX_CATEGORY, 0);}
	void StartSection(EBulkSection section) {CloseSection(); current_bulk_section = section;}
    void DoneItem(EArcSection section, ECategory category, unsigned number=1);
    void DoneItem(EBulkSection section, unsigned number);
    void Done();

    std::vector<std::vector<double>> &GetArcTicks() {return loaded_arc_ticks;}
    std::vector<double>              &GetRelLoad() {return loaded_relative_effort;}
    std::vector<unsigned>            &GetArcNum() {return loaded_arc_number;}

    /** Write `loaded_relative_effort` and `loaded_arc_ticks` into file*/
    bool Write(const char *fn);
    /** Load `loaded_relative_effort` and `loaded_arc_ticks` from file,
     * our use their default values to fill.*/
    bool Read(const char *fn);
};

#endif //PROGRESS_H