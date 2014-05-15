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

/** @file mscdrawer.cpp The implementation of drawing in Msc generator.
 * @ingroup libmscgen_files 
 *
 * Use http://www.flounder.com/metafileexplorer.htm for debugging EMF output.
 */

#include <algorithm>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <cassert>
#include <cairo-ps.h>
#include <cairo-pdf.h>
#include <cairo-svg.h>
#include "msc.h"

#ifndef M_PI
/** My own definition of PI*/
#define M_PI 3.14159265358979323846
#endif

#ifndef CAIRO_HAS_PNG_FUNCTIONS
#warning Cairo misses PNG functions, this may be fatal.
#endif

#ifndef CAIRO_HAS_PDF_SURFACE
#error Cairo misses PDF functions, resulting exec will not have PDF support
#endif

#ifndef CAIRO_HAS_PS_SURFACE
#error Cairo misses PS functions, resulting exec will not have EPS support
#endif

#ifndef CAIRO_HAS_SVG_SURFACE
#error Cairo needs SVG functions, resulting exec will not have SVG support
#endif

#ifndef CAIRO_HAS_WIN32_SURFACE
#warning The Cairo lib installed here does not have WIN32 functions.
#warning Resulting exec will not have EMF support.
#endif

/** Possible values for page size*/
template<> const char EnumEncapsulator<PageSizeInfo::EPageSize>::names[][ENUM_STRING_LEN] =
    {"invalid", "none", 
    "A0P", "A0L", "A1P", "A1L", "A2P", "A2L", 
    "A3P", "A3L", "A4P", "A4L", "A5P", "A5L", 
    "A6P", "A6L", "LETTER_P", "LETTER_L", 
    "LEGAL_P", "LEGAL_L", "LEDGER", "TABLOID", ""};

const char VerbosePageSizeNames[][ENUM_STRING_LEN] =
{"invalid", "none",
"A0 portrait", "A0 landscape", "A1 portrait", "A1 landscape", "A2 portrait", "A2 landscape",
"A3 portrait", "A3 landscape", "A4 portrait", "A4 landscape", "A5 portrait", "A5 landscape",
"A6 portrait", "A6 landscape", "letter portrait", "LETTER_ landscape",
"legal portrait", "legal landscape", "ledger", "tabloid", ""};

PageSizeInfo::EPageSize PageSizeInfo::ConvertPageSize(const char *c)
{
    EPageSize ret = NO_PAGE;
    Convert(c, ret);
    return ret;
}

const char *PageSizeInfo::ConvertPageSize(EPageSize ps)
{
    return EnumEncapsulator<EPageSize>::names[ps];
}

const char *PageSizeInfo::ConvertPageSizeVerbose(EPageSize ps)
{
    return VerbosePageSizeNames[ps];
}

/** Returns the page size in points (1/72 inch)*/
XY PageSizeInfo::GetPhysicalPageSize(EPageSize ps)
{
    if (ps>=MAX_PAGE || ps<=NO_PAGE) return XY(0,0);
    static const double ISO_size[] = {118.9, 84.1, 59.4, 42.0, 29.7, 21.0, 14.8, 10.5};
    const double mul = PT_PER_CM; 
    XY ret;
    switch (ps) {
    case LETTER_P:
        ret = XY(8.5, 11)*72; break;
    case LEGAL_P:
        ret = XY(8.5, 14)*72; break;
    case LEDGER:
        ret = XY(17, 11)*72; break;
    default: //ISO sizes
        ret.x = mul*ISO_size[unsigned(ps)/2];
        ret.y = mul*ISO_size[unsigned(ps)/2 - 1];
    }
    //handle landscape
    if (unsigned(ps)%2)
        ret.SwapXY();
    return ret;
}


/** Creates an empty canvas to query font sizes 
 * Drawing on this will fire a debug assertion or
 * do nothing for a release build.*/
Canvas::Canvas(EOutputType) : 
    outFile(NULL), surface(NULL), cr(NULL), status(ERR_PARAM), candraw(false),
    fake_dash_offset(0), outType(PNG), total(0,0,0,0),
    external_surface(false), copyrightTextHeight(0),
    user_scale(1,1), raw_page_clip(0,0,0,0), h_alignment(0), v_alignment(0)
#ifdef CAIRO_HAS_WIN32_SURFACE
	, stored_metafile_size(0), win32_dc(NULL), original_hdc(NULL), external_hdc(false)
#endif
{
    SetLowLevelParams();
    status = CreateSurface(XY(0,0));
    if (status!=ERR_OK) return;
    status = CreateContext(0, 0, 0);
    if (status!=ERR_OK) CloseOutput();
}

/**Creates a canvas to draw one page or all of the chart to a (single-page) file.
  After creation you can start drawing in chart coordinates and the right
  page will show. Call PrepareForHeaderFoorter() before drawing the 
  automatic heading and copyright text.
  @param [in] ot The type of the file. Cannot be WIN or PRINTER, since
                 these are not to be written to a file. These create a 
                 Canvas in error.
  @param [in] tot The total size of the chart in the canvas coordinate space.
  @param [in] ctexth The height of the copyright text.
  @param [in] fn The filename.
  @param [in] scale The output shall be this much bigger than `tot`.
  @param [in] pageBreakData The collection of page break associated data 
                            generated during parsing. This function only
                            uses the y coordinate of the page break and
                            the size of the automatically inserted heading
                            (if any).
  @param [in] page Specifies which page we the Canvas for, the index of
                   the first page is 1. 0 means we want to treat 
                   the whole chart as one page.*/
Canvas::Canvas(EOutputType ot, const Block &tot, double ctexth, 
               const string &fn, const XY &scale,
               const PBDataVector *pageBreakData, unsigned page) :
    outFile(NULL), surface(NULL), cr(NULL), status(ERR_PARAM), candraw(false),
    fake_dash_offset(0), outType(ot), total(tot),
    external_surface(false), copyrightTextHeight(ctexth),
    user_scale(scale), raw_page_clip(0,0,0,0), h_alignment(0), v_alignment(0)
#ifdef CAIRO_HAS_WIN32_SURFACE
	, stored_metafile_size(0), win32_dc(NULL), original_hdc(NULL), external_hdc(false)
#endif
{
    _ASSERT(ot!=WIN);
    _ASSERT(fn.length()>0);
    if (fn.length()==0 || ot==WIN || ot==PRINTER) 
        return; //fail

    SetLowLevelParams();
    
    double origYSize, origYOffset, autoHeadingSize;
    GetPagePosition(pageBreakData, page, origYOffset, origYSize, autoHeadingSize);
#ifdef CAIRO_HAS_WIN32_SURFACE
    original_device_size.x = total.x.Spans();
    original_device_size.y = autoHeadingSize + origYSize + copyrightTextHeight;
    original_device_size.x *= user_scale.x;//*fake_scale;
    original_device_size.y *= user_scale.y;//*fake_scale;
#endif 
    XY size(total.x.Spans(), origYSize);
    size.y += autoHeadingSize + copyrightTextHeight;
    size.x *= fake_scale*scale.x;
    size.y *= fake_scale*scale.y;

    fileName = fn;
    if (page>0) {  //append page number if not all pages written
        char num[3]="00";
        num[0] = char('0'+page/10);
        num[1] = char('0'+page%10);
        fileName.insert(fn.find_last_of('.'), num);
    }
    if (ot==EMF || ot==WMF)
        //For metafiles, there is no need to open a FILE*
        outFile = NULL;
    else {
        outFile = fopen(fileName.c_str(), "wb");
        if (!outFile) {
            status = ERR_FILE;
            return;
        }
    }
    status = CreateSurface(size);
    if (status!=ERR_OK) return;
    status = CreateContext(origYSize, origYOffset, autoHeadingSize);
    if (status!=ERR_OK) CloseOutput();
    else candraw = true;
}

/** Creates a canvas to draw the chart to a fixed-page-size multi-page file & starts page No. 1.
  After creation you can start drawing in chart coordinates and the first page
  page will appear. Call PrepareForHeaderFoorter() before drawing the 
  automatic heading and copyright text. Then call TurnPage and repeat
  for second page and so on.
  @param [in] ot The type of the file. Currently must be PDF, since
                 this is the onl format supporting multiple pages in a single
                 file. Others create a Canvas in error.
  @param [in] tot The total size of the chart in the canvas coordinate space.
  @param [in] fn The filename.
  @param [in] scale A series of scale factors to try. We go through them
                    and see if the pages of the chart fit the fixed page size
                    with the scale factor. We try in the list until we find
                    a scale factor that fits. -1 value means "fit width",
                    -2 value means "fit page" - this one scales to fit the 
                    largest page and uses the same factor for all pages and
                    always fits. If none of the values fit, we use the last one.
  @param [in] pageSize The size of the fixed page in points/pixels.
  @param [in] margins The margin size in points/pixels. The order is
                      left,right, up,down.
  @param [in] ha The horizontal alignment -1=left, 0=center, 1=right
  @param [in] va The vertical alignment -1=up, 0=center, 1=down
  @param [in] ctexth The height of the copyright text.
  @param [in] pageBreakData The collection of page break associated data 
                            generated during parsing. This function only
                            uses the y coordinate of the page break and
                            the size of the automatically inserted heading
                            (if any.)*/
Canvas::Canvas(EOutputType ot, const Block &tot, const string &fn, const std::vector<XY>  &scale, 
                     const XY &pageSize, const double margins[4],  int ha, int va, 
                     double ctexth, const PBDataVector *pageBreakData) :
    outFile(NULL), surface(NULL), cr(NULL), status(ERR_PARAM), candraw(false),
    fake_dash_offset(0), outType(ot), total(tot),
    external_surface(false), copyrightTextHeight(ctexth),
    raw_page_clip(0,0,0,0), h_alignment(ha), v_alignment(va)
#ifdef CAIRO_HAS_WIN32_SURFACE
	, stored_metafile_size(0), win32_dc(NULL), original_hdc(NULL), external_hdc(false)
#endif
{
    _ASSERT(ot==PDF);
    _ASSERT(fn.length()>0);

    SetLowLevelParams();
    _ASSERT(fake_scale==1); //We cannot increase scale with fixed paper sizes...
    fileName = fn;
    outFile = fopen(fileName.c_str(), "wb");
    if (!outFile) {
        status = ERR_FILE;
        return;
    }
    status = CreateSurface(pageSize);
    if (status!=ERR_OK) return;

    //Determine net page size (physical size - margins)
    raw_page_clip = Block(XY(0,0), pageSize);
    if (margins) {
        raw_page_clip.x.from += margins[0];
        raw_page_clip.x.till -= margins[1];
        raw_page_clip.y.from += margins[2];
        raw_page_clip.y.till -= margins[3];
    }
    if (raw_page_clip.x.from >= raw_page_clip.x.till || 
        raw_page_clip.y.from >= raw_page_clip.y.till) {
        status = ERR_MARGIN;
        return; //too big margins
    }
    //Determine scaling
    double origYSize, origYOffset, autoHeadingSize;
    for (unsigned s=0; s<scale.size(); s++) {
        if (scale[s].x<=0) { //auto or width
            _ASSERT(scale[s].x==-2 || scale[s].x==-1);
            _ASSERT(scale[s].y==scale[s].x);
            //Set scale to what is dictated by width
            user_scale.x = raw_page_clip.x.Spans()/total.x.Spans();
            if (scale[s].x==-2) { //auto
                //check if we need to reduce scale for all page length to fit, as well
                for (unsigned p = 1; p<=pageBreakData->size(); p++) {
                    GetPagePosition(pageBreakData, p, origYOffset, origYSize, autoHeadingSize);
                    user_scale.x = std::min(user_scale.x, raw_page_clip.y.Spans()/(autoHeadingSize+origYSize+copyrightTextHeight));
                }
                user_scale.y = user_scale.x;
                break; //auto always fits
            }
            user_scale.y = user_scale.x;
        } else 
            user_scale = scale[s];
        //Check if we fit
        bool oversize = raw_page_clip.x.Spans() < total.x.Spans()*user_scale.x;
        if (!oversize) 
            for (unsigned p = 1; p<=pageBreakData->size(); p++) {
                GetPagePosition(pageBreakData, p, origYOffset, origYSize, autoHeadingSize);
                oversize = raw_page_clip.x.Spans() < (autoHeadingSize+origYSize+copyrightTextHeight)*user_scale.y;
                if (oversize) break;
            }
        if (!oversize) break; //if all pages fit, we stick to this user_scale
    }
    //Create context for first page
    GetPagePosition(pageBreakData, 1, origYOffset, origYSize, autoHeadingSize);
#ifdef CAIRO_HAS_WIN32_SURFACE
    original_device_size.x = total.x.Spans();
    original_device_size.y = autoHeadingSize + origYSize + copyrightTextHeight;
    original_device_size.x *= user_scale.x;//*fake_scale;
    original_device_size.y *= user_scale.y;//*fake_scale;
#endif 
    status = CreateContext(origYSize, origYOffset, autoHeadingSize);
    if (status!=ERR_OK) {
        CloseOutput();
        return;
    }
    candraw = true;
}

/** Prepares a multi-page Canvas to print a new page.
  We flush the previous page and change the translation matrix for the 
  new page. After this call you can start drawing in chart coordinates 
  and page `next_page` page will appear. Call PrepareForHeaderFoorter() 
  before drawing the automatic heading and copyright text for `next_page`. 
  @param [in] pageBreakData The collection of page break associated data 
                            generated during parsing. This function only
                            uses the y coordinate of the page break and
                            the size of the automatically inserted heading
                            (if any).
  @param [in] next_page Specifies which page the next page will be.
                        The index of the first page is 1. 0 means we want 
                        to treat the whole chart as one page.
                        In principle you can print a page multiple times
                        or in whatever order or print the whole chart as
                        one of the pages - but why would you do so?
  @param error If the height next page does not fit the output page size
               This is where we emit a warning (if not NULL).
  @returns False if an error (but not warning) has been encountered, 
           irrespective of the value of `error` parameter.
*/
 bool Canvas::TurnPage(const PBDataVector *pageBreakData, unsigned next_page, 
                         MscError *error) 
{
    if (status!=ERR_OK) 
        return false;
    //Complete the old page
    cairo_destroy(cr);
    cairo_surface_show_page(surface);
    //Cretate context for next page
    double origYSize, origYOffset, autoHeadingSize;
    GetPagePosition(pageBreakData, next_page, origYOffset, origYSize, autoHeadingSize);
#ifdef CAIRO_HAS_WIN32_SURFACE
    original_device_size.x = total.x.Spans();
    original_device_size.y = autoHeadingSize + origYSize + copyrightTextHeight;
    original_device_size.x *= user_scale.x;//*fake_scale;
    original_device_size.y *= user_scale.y;//*fake_scale;
#endif 
    status = CreateContext(origYSize, origYOffset, autoHeadingSize);
    if (status!=ERR_OK) {
        CloseOutput();
        ErrorAfterCreation(error, pageBreakData, false);
        candraw = false;
        return false;
    }
    //Complain if this page is too 
    if (error && raw_page_clip.y.Spans() < (origYSize+copyrightTextHeight)*user_scale.y) {
        string err = "Page #";
        err << next_page << " is ";
        err << ceil((origYSize+copyrightTextHeight)*user_scale.y*100./raw_page_clip.y.Spans()-100);
        error->Warning(FileLineCol(0, 0), err + "% longer than the paper size and will be cropped."); 
    }
    candraw = true;
    return true;
}

/** Records the errors/warnings happened during any constructor.
  Possible errors we record are
  - Could not open file
  - Internal error
  - Could not open canvas (cairo error)
  - Too big margins 
  - And a warning if the page with is higher than a fixed page width (minus margins)
  - And a warining if for multi-page output the first page is taller than 
    the paper height (minus margins).
  @param error This is where the errors are reported.
  @param [in] pageBreakData The collection of page break associated data 
                            generated during parsing. This function only
                            uses the y coordinate of the page break and
                            the size of the automatically inserted heading
                            (if any).
  @param [in] fatal If true then the file open and margin errors will be reported
                    as fatal.*/
bool Canvas::ErrorAfterCreation(MscError *error,  const PBDataVector *pageBreakData, 
                                bool fatal)
{
    if (error==NULL) return status!=ERR_OK;
    typedef void (MscError::*TFunc)(FileLineCol , const std::string &, const std::string &);
    const TFunc func = fatal ? &MscError::FatalError : (TFunc)&MscError::Error;
    switch (status) {
    default: _ASSERT(0); return true;
    case Canvas::ERR_FILE: error->FatalError(FileLineCol(0, 0), "Could not open file '" + fileName + "'."); return true;
    case Canvas::ERR_PARAM: (error->*func)(FileLineCol(0, 0), "Internal param problem when opening canvas.", ""); return true;
    case Canvas::ERR_CANVAS: (error->*func)(FileLineCol(0, 0), "Could not open canvas.", ""); return true;
    case Canvas::ERR_CANVAS_MEM: 
        {
            std::string s = "Out of memory - too big output image"; 
            if (surface_size_x && surface_size_y) 
                s << " (" << surface_size_x << "x" << surface_size_y << ")";
            (error->*func)(FileLineCol(0, 0), s + ".", ""); return true;
        }
    case Canvas::ERR_MARGIN: error->FatalError(FileLineCol(0, 0), "Too big margins - no print area left!", ""); return true;
    case Canvas::ERR_DONE: (error->*func)(FileLineCol(0, 0), "Whops, internal error.", ""); return true;
    case Canvas::ERR_OK: 
        if (raw_page_clip.x.till==0 || raw_page_clip.y.till==0) return false;
        if (raw_page_clip.x.Spans() < total.x.Spans()*user_scale.x) {
            string err = "Chart is ";
            err << ceil(total.x.Spans()*user_scale.x*100./raw_page_clip.x.Spans()-100);
            error->Warning(FileLineCol(0, 0), err + "% wider than the paper size and will be cropped."); 
        }
        double origYSize, origYOffset, autoHeadingSize;
        GetPagePosition(pageBreakData, 1, origYOffset, origYSize, autoHeadingSize);
        if (raw_page_clip.y.Spans() <origYSize*user_scale.y) {
            string err = "First page is ";
            err << ceil(origYSize*user_scale.y*100./raw_page_clip.y.Spans()-100);
            error->Warning(FileLineCol(0, 0), err + "% longer than the paper size and will be cropped."); 
        }
        return false;
    }
}

/**Creates a canvas to draw one page or all of the chart to a cairo recording surface.*

  All chart elements will be recorded in an area `tot*fake_scale` 
  (for showing all the chart). (But `fake_scale` is one for all output types
  other than WMF.)
  Preparation for heading and copyright text is also added.
  After Canvas creation, draw chart in chart coordinates, call
  PrepareForHeaderFoorter() and draw automatic heading and copyright text.

  @param [in] ot The type of the output - Drawing fakes and tweaks will be applied 
                 as per this. Does not really influence output target - that 
                 is a cairo recording surface here.
  @param surf The cairo recording surface to draw onto (in fact, can be any surface).
  @param [in] tot The total size of the chart in the canvas coordinate space.
  @param [in] ctexth The height of the copyright text.
  @param [in] scale The scaling applied before we draw to the surface.
  @param [in] pageBreakData The collection of page break associated data 
                            generated during parsing. This function only
                            uses the y coordinate of the page break and
                            the size of the automatically inserted heading
                            (if any.)
  @param [in] page Specifies which page we the Canvas for, the index of
                   the first page is 1. 0 means we want to treat 
                   the whole chart as one page.*/
Canvas::Canvas(EOutputType ot, cairo_surface_t *surf, const Block &tot, double ctexth, 
               const XY &scale, const PBDataVector *pageBreakData, unsigned page) :

    outFile(NULL), surface(surf), cr(NULL), status(ERR_PARAM), candraw(false),
    fake_dash_offset(0), outType(ot), total(tot),
    external_surface(true), copyrightTextHeight(ctexth),
    user_scale(scale), raw_page_clip(0,0,0,0), h_alignment(0), v_alignment(0)
#ifdef CAIRO_HAS_WIN32_SURFACE
	, stored_metafile_size(0), win32_dc(NULL), original_hdc(NULL), external_hdc(false)
#endif
{
    if (CAIRO_STATUS_SUCCESS != cairo_surface_status(surf)) return; //nodraw state
    surface_size_x = surface_size_y = 0;
    SetLowLevelParams();
    double origYSize, origYOffset, autoHeadingSize;
    GetPagePosition(pageBreakData, page, origYOffset, origYSize, autoHeadingSize);
    cairo_surface_set_fallback_resolution(surface, fallback_resolution/fake_scale, fallback_resolution/fake_scale);
    status = CreateContext(origYSize, origYOffset, autoHeadingSize);
    if (status!=ERR_OK) CloseOutput();
    else candraw = true;
}


/** A callback to write data to a FILE 
  @param closure The FILE* to write to.
  @param [in] data The data to write
  @param [in] length The length of the data
  @returns Either CAIRO_STATUS_WRITE_ERROR or CAIRO_STATUS_SUCCESS.*/
cairo_status_t write_func(void * closure, const unsigned char *data, unsigned length)
{
    if (closure==NULL) return CAIRO_STATUS_SUCCESS;
    if (length == fwrite(data, 1, length, (FILE*)closure))
        return CAIRO_STATUS_SUCCESS;
    else
        return CAIRO_STATUS_WRITE_ERROR;
}

/** Set the low-level compatibility options according to our output type.*/
void Canvas::SetLowLevelParams()
{
    /* Set defaults */
    use_text_path = false;
    use_text_path_rotated = false;
    individual_chars = false;
    fake_gradients = 0;
    fake_dash = false;
    fake_shadows = false;
    fake_spaces = false;
    needs_arrow_fix = false;
    avoid_linewidth_1 = false;
    imprecise_positioning = false;
    needs_strokepath_rclboundsfix = false;
    can_and_shall_clip_total = true;
    avoid_transparency = false;
    white_background = false;
    fake_scale = 1.0;          //normally we map our coordinates 1:1 onto the canvas
    fallback_resolution = 300; //this will be irrelevant for bitmaps - they use the native resolution
    needs_dots_in_corner = false;

    //Customize as per target type
    switch (outType) {
    case Canvas::PNG:
        white_background = true;
        break;
#ifdef CAIRO_HAS_WIN32_SURFACE
    case WMF:
    case EMFWMF:
        //determine scaling so that no coordinate is larger than 30K - a built-in limit of WMF
        //but scale up, so that coordinates are large, since WMF handles only integer coordinates,
        //thus fonts are bad if we do not scale up.
        //So if we have total calculated already, we select fake_scale as 10, or smaller if this would result in >30K coords.
        //Setting fake_scale higher than 10 seems to result in wrong image fallback positioning, I am not sure why.
        if (total.x.Spans()>0 && total.y.Spans()>0)
            fake_scale = std::min(std::min(30000./total.x.Spans(), 30000./total.y.Spans()), 10.);  
        individual_chars = true;        //do this so that it is easier to convert to WMF
        use_text_path_rotated = true;   //WMF has no support for this
        fake_dash = true;               //WMF has no support for this
        needs_arrow_fix = true;         //WMF does not support complex clipping it seems
        //Fallthrough
    case EMF:
    case PRINTER:
        if (outType==PRINTER) 
            fallback_resolution = 50;
        avoid_linewidth_1 = true;       //EMF needs wider than 1 horizontal lines to avoid clipping them too much
        needs_dots_in_corner = true;
        imprecise_positioning = true;
        needs_strokepath_rclboundsfix = true;
        fake_gradients = 30;
        fake_shadows = true;
        break;
    case WIN:
        //(Highest quality) Default image surface settings will do
        //no need to clear bkg, as we always do that when drawing to the client area
#endif
    default:
        break;
    }
}

/** Return position information about a page of the chart
 * All values returned are in chart coordinates.
 * @param [in] pageBreakData The collection of page break associated data 
 *                           generated during parsing. This function only
 *                           uses the y coordinate of the page break and
 *                           the size of the automatically inserted heading
 *                           (if any).
 * @param [in] page Specifies which page we want data for, the index of
 *                  the first page is zero. -1 means we want to treat 
 *                  the whole chart as one page.
 * @param [out] y_offset The y coordinate of the top of the page.
 * @param [out] y_size The length of the page (not including the automatically
 *                     inserted heading (if any) and the copyright text.
 * @param [out] autoHeadingSize The size of the automatically inserted heading
 *                              at the top of the page (if any). These headings
 *                              are not included in the chart space. */
void Canvas::GetPagePosition(const PBDataVector *pageBreakData, unsigned page, double &y_offset, 
                                double &y_size, double &autoHeadingSize) const
{
    if (page==0 || pageBreakData==NULL || pageBreakData->size()==0) {
        y_offset = total.y.from;
        y_size = total.y.Spans();
        autoHeadingSize = 0;
    } else if (page>pageBreakData->size()) { //too large page
        y_offset = total.y.till;
        y_size = 0; //nothing drawn
        autoHeadingSize = 0;
    } else if (page==pageBreakData->size()) { //last page
        y_offset = pageBreakData->at(page-1).y;
        y_size = total.y.till - pageBreakData->at(page-1).y;
        autoHeadingSize = pageBreakData->at(page-1).autoHeadingSize;
    } else {
        y_size = pageBreakData->at(page).y - pageBreakData->at(page-1).y;
        y_offset =  pageBreakData->at(page-1).y;
        autoHeadingSize = pageBreakData->at(page-1).autoHeadingSize;
    }
}


/** Creates a surface of a given size.
 * Uses the `outType` member to determine what type of surface to create.
 * Uses the `fileName` members to open an EMF file, if not empty.
 * Uses the 'outFile' memeber to direct EPS and SVG output into it.
 * @param [in] size The native size of the surface (can be 0,0), that is 
 *                  if the user specifies a scale of 2, this shall be twice the
 *                  chart (page) size.
 * @returns Any error or ERR_OK.*/
Canvas::EErrorType Canvas::CreateSurface(const XY &size)
{
    const int x = abs(int(size.x));
    const int y = abs(int(size.y));
    surface_size_x = x;
    surface_size_y = y;
    switch (outType) {
    default:
        return ERR_PARAM;
#ifdef CAIRO_HAS_PNG_FUNCTIONS
    case Canvas::PNG:
        surface = cairo_image_surface_create (CAIRO_FORMAT_RGB24, x, y);
        break;
#endif
#ifdef CAIRO_HAS_PS_SURFACE
    case Canvas::EPS:
        surface = cairo_ps_surface_create_for_stream(write_func, outFile, x, y);
        cairo_ps_surface_set_eps(surface, true);
        break;
#endif
#ifdef CAIRO_HAS_PDF_SURFACE
    case Canvas::PDF:
        surface = cairo_pdf_surface_create_for_stream(write_func, outFile, x, y);
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,10,0)
        //Limit to PDF version 1.4 if possible (appeared in cairo 1.10)
        //since no features of higher are relevant and Texinfo doc compilation 
        //produces warnings with 1.5
        if (cairo_surface_status(surface) == CAIRO_STATUS_SUCCESS)
            cairo_pdf_surface_restrict_to_version(surface, CAIRO_PDF_VERSION_1_4);
#endif
        break;
#endif
#ifdef CAIRO_HAS_SVG_SURFACE
    case Canvas::SVG:
        surface = cairo_svg_surface_create_for_stream(write_func, outFile, x, y);
        break;
#endif
#ifdef CAIRO_HAS_WIN32_SURFACE
    case Canvas::EMF:
    case Canvas::EMFWMF:
        // Create the Enhanced Metafile
        if (fileName.length() > 0) 
            original_hdc = CreateEnhMetaFile(NULL, fileName.c_str(), NULL, "Msc-generator\0\0");
        else {
            _ASSERT(0);
            return ERR_PARAM; //this can only be called with a filename
        }
        // Did you get a good metafile?
        if( original_hdc == NULL ) 
            return ERR_CANVAS;
        /* We need a tweak in cairo.
		 ** I modified cairo_win32_printing_surface_create diretcly to use 
		 ** an extent bigger than the one reported by GetClipBox(), if the 
		 ** DC passed is an EMF DC.
         ** This is to avoid dropping glyphs outside the DC's extent.
         ** I could not make a metafile DC to have larger extent than
         ** the current screen size.
         */
        win32_dc = CreateEnhMetaFile(NULL, NULL, NULL, NULL);
        // Did you get a good metafile?
        if( win32_dc == NULL ) 
            return ERR_CANVAS;
        surface = cairo_win32_printing_surface_create(win32_dc);
        break;
	case Canvas::WMF:
        // Create the Metafile on disk
        if (fileName.length() > 0)
            original_hdc = CreateMetaFile(fileName.c_str());
        else {
            _ASSERT(0);
            return ERR_PARAM; //this can only be called with a filename
        }
        // Did you get a good metafile?
        if( original_hdc == NULL ) 
            return ERR_CANVAS;
        // Create the Enhanced Metafile
        win32_dc = CreateEnhMetaFile(NULL, NULL, NULL, NULL);
        // Did you get a good metafile?
        if( win32_dc == NULL ) 
            return ERR_CANVAS;
        surface = cairo_win32_printing_surface_create(win32_dc);
#endif
    }
    cairo_status_t st = cairo_surface_status(surface);
    if (st != CAIRO_STATUS_SUCCESS) {
        CloseOutput();
        //We get CAIRO_STATUS_INVALID_SIZE if the bitmap image is larger
        //than 32767 (for cairo 12.8), which is because they send 
        //coordinates to pixman in 16.16 fixpoint format.
        return (st == CAIRO_STATUS_NO_MEMORY || st == CAIRO_STATUS_INVALID_SIZE) ? 
            ERR_CANVAS_MEM : ERR_CANVAS;
    }
    cairo_surface_set_fallback_resolution(surface, fallback_resolution/fake_scale, fallback_resolution/fake_scale);
    return ERR_OK;
}

/** Creates a cairo context for an already created surface.
 * We not only create a surface, but set the transformation matrix
 * to reflect page alignment, scaling pagination and low-level 
 * compatibility options.
 * We use cairo_save twice to create two drawing situations.
 * - The topmost (available on return) will have clipping for the page content
 *   That is drawing the objects of the chart in chart coordinates will 
 *   send the content of the page specified by the input parameters of this
 *   function to the output.
 * - The second situation will contain a larger clip area allowing to 
 *   draw the automatic heading (if any) at the top of the page and the
 *   copyright notice at the bottom.
 *
 * If `raw_page_clip` contains something, we clip the native surface and 
 * translates accorindg to `h_alignment` and `v_alignment`.
 * We also draw the white background and the dots in the corner, if needed.
 * @param [in] origYSize The height of the page in chart coordinates.
 * @param [in] origYOffset The top of the page in chart coordinates.
 * @param [in] autoHeadingSize The height of the automatic header at the
 *                             top of the page (if any).*/
Canvas::EErrorType Canvas::CreateContext(double origYSize, double origYOffset, 
                                         double autoHeadingSize) 
{
    cr = cairo_create (surface);
    cairo_status_t st = cairo_status(cr);
    if (st != CAIRO_STATUS_SUCCESS) {
        CloseOutput();
        return st == CAIRO_STATUS_NO_MEMORY ? ERR_CANVAS_MEM : ERR_CANVAS;
    }
    //Set line cap to butts
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);
    //Turn font metric hints off. This way scaling the fonts will not change layout.
    cairo_font_options_t *fopt = cairo_font_options_create();
    cairo_get_font_options(cr, fopt);
    cairo_font_options_set_hint_metrics(fopt, CAIRO_HINT_METRICS_OFF);
    cairo_set_font_options(cr, fopt);
    cairo_font_options_destroy(fopt);

    scale_for_shadows = sqrt(user_scale.y*user_scale.x);
    if (total.x.Spans() <= 0 || total.y.Spans()<=0) return ERR_OK; //OK

    if (raw_page_clip.y.till>0) {
        cairo_rectangle(cr, raw_page_clip.x.from, raw_page_clip.y.from, raw_page_clip.x.Spans(), raw_page_clip.y.Spans());
        cairo_clip(cr);
        XY off = raw_page_clip.UpperLeft();
        switch (v_alignment) {
        case +1: //bottom
            off.y += raw_page_clip.y.Spans() - (autoHeadingSize+origYSize+copyrightTextHeight)*user_scale.y; break;
        case 0: //center
            off.y += (raw_page_clip.y.Spans() - (autoHeadingSize+origYSize+copyrightTextHeight)*user_scale.y)/2; break;
        case -1://top
            break;
        default: _ASSERT(0);
        }
        switch (h_alignment) {
        case 1: //right
            off.x += raw_page_clip.x.Spans() - total.x.Spans()*user_scale.x; break;
        case 0:  //center
            off.x += (raw_page_clip.x.Spans() - total.x.Spans()*user_scale.x)/2; break;
        case -1: //left
            break;
        default: _ASSERT(0);
        }
        cairo_translate(cr, off.x, off.y);
    }

    cairo_scale(cr, fake_scale*user_scale.x, fake_scale*user_scale.y);
    cairo_save(cr);
    if (can_and_shall_clip_total) {
        //clip first to allow for banner text, but not for WIN contexts
        cairo_rectangle(cr, 0, 0, total.x.Spans(), autoHeadingSize+origYSize+copyrightTextHeight);
        cairo_clip(cr);
    }
    if (white_background) {
        cairo_rectangle(cr, 0, 0, total.x.Spans(), autoHeadingSize+origYSize+copyrightTextHeight);
        cairo_set_source_rgb(cr, 1., 1., 1.);
        cairo_fill(cr);
    } else if (needs_dots_in_corner) {
        //Draw small marks in corners, so EMF an WMF spans correctly
        //(for some reason stroke do not work as intended, so we use fill()s)
        cairo_set_source_rgb(cr, 1, 1, 1);
        cairo_rectangle(cr, 0, 0, 1, 1);
        cairo_fill(cr);
        cairo_rectangle(cr, total.x.Spans()-1, autoHeadingSize+origYSize+copyrightTextHeight-1, 1, 1);
        cairo_fill(cr);
    }
    cairo_translate(cr, -total.x.from, autoHeadingSize-origYOffset);
    cairo_save(cr);
    if (can_and_shall_clip_total) {
        //then clip again to exclude banner text, but not for WIN contexts
        cairo_rectangle(cr, total.x.from, origYOffset, total.x.Spans(), origYSize);
        cairo_clip(cr);
    }
    return ERR_OK;
}

#ifdef CAIRO_HAS_WIN32_SURFACE

/** Create a Canvas to a Windows Device Conetxt that is either a display/printer DC or a metafile.
  After Canvas creation, draw chart in chart coordinates, call
  PrepareForHeaderFoorter() and draw automatic heading and copyright text.

  @param [in] ot The type of the output - WIN, WMF, EMF, EMFWMF and PRINTER.
  @param [in] tot The total size of the chart in the canvas coordinate space.
  @param [in] ctexth The height of the copyright text.
  @param [in] scale Scales the output to be this much bigger
  @param [in] pageBreakData The collection of page break associated data 
                            generated during parsing. This function only
                            uses the y coordinate of the page break and
                            the size of the automatically inserted heading
                            (if any.)
  @param [in] page Specifies which page we the Canvas for, the index of
                   the first page is 1. 0 means we want to treat 
                   the whole chart as one page.*/
//Use this to display a chart, use other constructors without a DC to save the chart to a file
Canvas::Canvas(EOutputType ot, HDC hdc, const Block &tot, double ctexth, 
                     const XY &scale, const PBDataVector *pageBreakData, unsigned page) :
    fake_dash_offset(0), outFile(NULL), surface(NULL), cr(NULL), outType(ot),
    total(tot), status(ERR_PARAM), candraw(false), 
    external_surface(false), copyrightTextHeight(ctexth),
    raw_page_clip(0,0,0,0), user_scale(scale), h_alignment(0), v_alignment(0),
    stored_metafile_size(0), win32_dc(NULL), original_hdc(NULL), external_hdc(true)
{
    if (ot!=WIN && ot!=WMF && ot!=EMF && ot!=EMFWMF && ot!=PRINTER) 
        return;

    SetLowLevelParams();

    double origYSize, origYOffset, autoHeadingSize;
    GetPagePosition(pageBreakData, page, origYOffset, origYSize, autoHeadingSize);
    original_device_size.x = total.x.Spans();
    original_device_size.y = autoHeadingSize + origYSize + copyrightTextHeight;
    original_device_size.x *= scale.x;//*fake_scale;
    original_device_size.y *= scale.y;//*fake_scale;

    switch (ot) {
    case WIN:
        surface = cairo_win32_surface_create(hdc);
        break;
    case EMF:
    case WMF:
    case EMFWMF:
    case PRINTER:
        win32_dc = CreateEnhMetaFile(NULL, NULL, NULL, NULL);
        if( win32_dc == NULL ) return;
        original_hdc = hdc;
        surface = cairo_win32_printing_surface_create(win32_dc);
        break;
    default:
        _ASSERT(0);
    }
    cairo_surface_set_fallback_resolution(surface, fallback_resolution/fake_scale, fallback_resolution/fake_scale);
    status = CreateContext(origYSize, origYOffset, autoHeadingSize);
    if (status!=ERR_OK) CloseOutput();
    else candraw = true;
}

/** Metafile record for text output*/
typedef struct tagEXTTO { // lf
    _int16 toY;
    _int16 toX;
    _int16 toSTRLEN;
    _int16 toFWOPTS;
    char   string[1];
} EXTTO;

/** Metafile record for changing font options*/
typedef struct tagLOGFONT16 { // lf
    _int16 lfHeight;
    _int16 lfWidth;
    _int16 lfEscapement;
    _int16 lfOrientation;
    _int16 lfWeight;
    BYTE   lfItalic;
    BYTE   lfUnderline;
    BYTE   lfStrikeOut;
    BYTE   lfCharSet;
    BYTE   lfOutPrecision;
    BYTE   lfClipPrecision;
    BYTE   lfQuality;
    BYTE   lfPitchAndFamily;
    char   lfFaceName[LF_FACESIZE];
} LOGFONT16;

/** Callback to process a WMF record.
  I use this callback to adjust erroneous WMF records
  which are badly converted from EMF.
  @param hDC handle to DC
  @param lpHTable metafile handle table
  @param lpMFR metafile record
  @param nObj count of objects
  @returns 1 if we shall continue processing records*/
int CALLBACK WMFRecordFixProc(HDC hDC,                // handle to DC
                              HANDLETABLE *lpHTable,  // metafile handle table
                              METARECORD *lpMFR,      // metafile record
                              int nObj,               // count of objects
                              LPARAM /*lpClientData*/ // optional data
                             )
{
    bool b=true;
    if (lpMFR->rdFunction == META_EXTTEXTOUT) {
        const EXTTO * const pTO = ( EXTTO* )(&lpMFR->rdParm[0]);
        const unsigned len = pTO->toSTRLEN;
        
        //**Below code is needed for cairo 10.8, but not for 12.2
        //char buff[4096];
        //for (unsigned i=0; i<len; i++)
        //    buff[i] = pTO->string[i] + 29; 
        //buff[len]=0;
        //ExtTextOut(hDC, pTO->toX, pTO->toY, 0, NULL, buff, len, NULL);
        ExtTextOut(hDC, pTO->toX, pTO->toY, 0, NULL, pTO->string, len, NULL);
    } else 
    if (lpMFR->rdFunction == META_CREATEFONTINDIRECT) {
        LOGFONT16* const plogfont = ( LOGFONT16* )(&lpMFR->rdParm[0]);
        plogfont->lfHeight = plogfont->lfHeight / 32;   //When this is negative character height is meant, when positive cell height (cell_h = character_h + lead). Keep sign!
        b=PlayMetaFileRecord(hDC, lpHTable, lpMFR, nObj);
    } else {
        b=PlayMetaFileRecord(hDC, lpHTable, lpMFR, nObj);
    }
    return 1;
}


/** Converts and paints an EMF on a WMF DC.
  @param [in] hemf the Enhanced Metafile
  @param hdc The WMF DC to draw on
  @param [in] r The size of the enhanced metafile
  @param [in] applyTricks Whether we shall apply my convesion trciks (adviseable)
  @returns The size of the resulting WMF in bytes.*/
size_t PaintEMFonWMFdc(HENHMETAFILE hemf, HDC hdc, const RECT &r, bool applyTricks)
{
    if (hemf == NULL || hdc == NULL) return 0;
    HDC refDC = GetDC(NULL);
    SetMapMode(refDC, MM_ANISOTROPIC);
    SetWindowExtEx(refDC, r.right, r.bottom, NULL);
    SetViewportExtEx(refDC, r.right, r.bottom, NULL);
    size_t size = GetWinMetaFileBits(hemf, 0, NULL, MM_ANISOTROPIC, refDC);
    BYTE *buff = (BYTE*)malloc(size);
	if (buff==NULL) {
		ReleaseDC(NULL, refDC);
		return 0;
	}
    size = GetWinMetaFileBits(hemf, size, buff, MM_ANISOTROPIC, refDC);
    ReleaseDC(NULL, refDC);
    HMETAFILE hwmf = SetMetaFileBitsEx(size, buff);
    free(buff);
    if (applyTricks)
        EnumMetaFile(hdc, hwmf, WMFRecordFixProc, NULL);
    else
        PlayMetaFile(hdc, hwmf);
    size = GetMetaFileBitsEx(hwmf, 0, NULL);
    DeleteMetaFile(hwmf);
    return size;
}

/** Callback to process an EMF record.
  I use this to collect fallback image (StetchDIBits records) location*/
int CALLBACK ExtractStretcDIBitsRecords( HDC hDC,
                                         HANDLETABLE *lpHTable,
                                         const ENHMETARECORD *lpEMFR,
                                         int nObj,
                                         LPARAM lpData)
{
    switch (lpEMFR->iType) {
        //operations modifying the transformation matrix
    case EMR_MODIFYWORLDTRANSFORM:
    case EMR_SETWORLDTRANSFORM:
    case EMR_SETVIEWPORTEXTEX:
    case EMR_SETVIEWPORTORGEX:
    case EMR_SETWINDOWEXTEX:
    case EMR_SETWINDOWORGEX:
    case EMR_SAVEDC:
    case EMR_RESTOREDC:
    case EMR_SCALEVIEWPORTEXTEX:
    case EMR_SCALEWINDOWEXTEX:
        PlayEnhMetaFileRecord(hDC, lpHTable, lpEMFR, nObj);
        break;
    case EMR_STRETCHDIBITS:
        Block b;
        const EMRSTRETCHDIBITS * const s = reinterpret_cast<const EMRSTRETCHDIBITS*>(lpEMFR);
        b.x.from = s->xDest;
        b.x.till = b.x.from + s->cxDest;
        b.y.from = s->yDest;
        b.y.till = b.y.from + s->cyDest;

        //Apply the world transform to block b
        //See http://msdn.microsoft.com/en-us/library/windows/desktop/dd145228(v=vs.85).aspx
        XFORM xForm;
        GetWorldTransform(hDC, &xForm);
        //We omit page to device translations now.
        //For those, see http://msdn.microsoft.com/en-us/library/windows/desktop/dd145045(v=vs.85).aspx
        XY points[4] = {
            b.UpperLeft().Transform(xForm.eM11, xForm.eM12, xForm.eM21, xForm.eM22, xForm.eDx, xForm.eDy),
            b.UpperRight().Transform(xForm.eM11, xForm.eM12, xForm.eM21, xForm.eM22, xForm.eDx, xForm.eDy),
            b.LowerRight().Transform(xForm.eM11, xForm.eM12, xForm.eM21, xForm.eM22, xForm.eDx, xForm.eDy),
            b.LowerLeft().Transform(xForm.eM11, xForm.eM12, xForm.eM21, xForm.eM22, xForm.eDx, xForm.eDy)
        };
        reinterpret_cast<Contour*>(lpData)->operator+=(Contour(points));
    } 
    return 1;
}

/** Determine the location of fallback images in an enhanced metafile.
  @param [in] hemf The enhanced metafile.
  @param [in] lpRECT The total size of the chart. If set correctly
                     the resulting image locations will be in chart coordinates.*/
Contour Canvas::FallbackImages(HENHMETAFILE hemf, LPRECT lpRECT)
{
    Contour c;
    HDC hDC = CreateEnhMetaFile(NULL, NULL, NULL, NULL);
    EnumEnhMetaFile(hDC, hemf, ExtractStretcDIBitsRecords, &c, lpRECT);
    DeleteObject(CloseEnhMetaFile(hDC));
    return c;
}


/** Callback to process an EMF record.
  I use this to expand the rect of Strokepath records. */
int CALLBACK ExpandStorkePathRect( HDC hDC,
                                   HANDLETABLE *lpHTable,
                                   const ENHMETARECORD *lpEMFR,
                                   int nObj,
                                   LPARAM lpData)
{
    switch (lpEMFR->iType) {
        //operations modifying the transformation matrix
    default:
        PlayEnhMetaFileRecord(hDC, lpHTable, lpEMFR, nObj);
        break;
    case EMR_STROKEPATH:
        EMRSTROKEPATH s;
        memcpy(&s, lpEMFR, sizeof(s));
        const unsigned amount = (unsigned)lpData;
        s.rclBounds.left   -= amount;
        s.rclBounds.right  += amount;
        s.rclBounds.top    -= amount;
        s.rclBounds.bottom += amount;
        //PlayEnhMetaFileRecord(hDC, lpHTable, lpEMFR, nObj);
        BOOL b =PlayEnhMetaFileRecord(hDC, lpHTable, (ENHMETARECORD*)&s, nObj);
        if (!b) {
            b =PlayEnhMetaFileRecord(hDC, lpHTable, lpEMFR, nObj);
            s.rclBounds.bottom += amount;

        }
    } 
    return 1;
}

/** Expand the clip rectangle of StrokePath records.
  @param [in] hemf The enhanced metafile.
  @param [in] lpRECT The total size of the chart. */
void Canvas::ExpandStrokePathRects(HDC hDC, HENHMETAFILE hemf, LPRECT lpRECT, unsigned amount)
{
    EnumEnhMetaFile(hDC, hemf, ExpandStorkePathRect, (LPVOID)amount, lpRECT);
}

/** Close the output and return the resulting emf file.
 * Valid only for EMF and EMFWMF outputs.
 * A complete replacement for CloseOutput().*/
HENHMETAFILE Canvas::CloseAndGetEMF()
{
    _ASSERT(outType == EMF || outType==EMFWMF || outType==PRINTER);
    _ASSERT(original_hdc == NULL);
    _ASSERT(surface);
    _ASSERT(!external_surface);

    if (status!=ERR_OK) 
        return NULL;
    /* Destroy our context */
    if (cr)
        cairo_destroy (cr);
    cairo_surface_show_page(surface);
    cairo_surface_destroy (surface);
    RECT r;
    SetRect(&r, 0, 0, int(original_device_size.x), int(original_device_size.y));
    HENHMETAFILE hemf = CloseEnhMetaFile(win32_dc);
    stored_fallback_image_places = FallbackImages(hemf, &r);                
    stored_metafile_size = GetEnhMetaFileBits(hemf, 0, NULL);
    stored_fallback_image_places.Scale(1/(user_scale.x/*fake_scale*/)); //TODO: assumes user_scale.x==user_scale.y
    win32_dc = NULL;
    outFile=NULL;
    surface=NULL;
    cr=NULL;
    fileName.clear();
    if (status==ERR_OK) status = ERR_DONE; //preserve other errors
    candraw = false;
    return hemf;
}

#endif //WIN32

void Canvas::PrepareForHeaderFoorter()
{
    if (status==ERR_OK)
        cairo_restore(cr);
}

void Canvas::CloseOutput()
{
    if (status!=ERR_OK) 
        return;
    /* Destroy our context */
    if (cr)
        cairo_destroy (cr);

    //Flush and destroy surface, but preserve external ones
    if (surface && !external_surface) {
        /* Output the image to the disk file in PNG format. */
        switch (outType) {
        case ISMAP:
            _ASSERT(0);
            break;
#ifdef CAIRO_HAS_PNG_FUNCTIONS
        case PNG:
            if (outFile)
                cairo_surface_write_to_png_stream (surface, write_func, outFile);
            //fallthrough
#endif
        case EPS:
        case PDF:
        case SVG:
            cairo_surface_destroy (surface);
            if (outFile)
                fclose(outFile);
            break;
        case EMF:
        case WMF:
        case EMFWMF:
        case PRINTER:
#ifdef CAIRO_HAS_WIN32_SURFACE
            cairo_surface_show_page(surface);
            cairo_surface_destroy (surface);
            RECT r;
            SetRect(&r, 0, 0, int(original_device_size.x), int(original_device_size.y));
            if (original_hdc) { 
                //Opened via either 
                //1. with an existing WMF HDC (OutType==WMF)
                //2. with an existing EMF HDC (outType==EMF or EMFWMF);
                //3. with a WMF file (OutType==WMF), the DC was created by "this"
                //4. with a EMF file (OutType==EMF or EMFWMF), the DC was created by "this"
                //5. with an existing printing DC (OutType==PRINTER)
                //For 1&3, we need to convert from EMF to WMF, but otherwise can close 
                //"original_hdc" the same way. 
                //For #5, we just copy. (This is a fix, we cannot draw onto a printer DC
                //with cairo directly, so we use an EMF in-between.)
                //win32_dc is an EMF DC opened by "this"
                //"original_dc" contains the original target to draw on
                HENHMETAFILE hemf = CloseEnhMetaFile(win32_dc);
                stored_fallback_image_places = FallbackImages(hemf, &r);                
                if (outType==WMF) {
                    stored_metafile_size = PaintEMFonWMFdc(hemf, original_hdc, r, true); 
                    //This will leave the file on disk in case of #3
                    if (!external_hdc)
                        DeleteMetaFile(CloseMetaFile(original_hdc)); 
                } else { //EMF, EMFWMF or PRINTER
                    //PlayEnhMetaFile(original_hdc, hemf, &r);
                    ExpandStrokePathRects(original_hdc, hemf, &r, 20);
                    stored_metafile_size = GetEnhMetaFileBits(hemf, 0, NULL);
                    //This will leave the file on disk in case of #4
                    if (!external_hdc)
                        DeleteEnhMetaFile(CloseEnhMetaFile(original_hdc)); 
                }
                DeleteEnhMetaFile(hemf);
                original_hdc = NULL;
                win32_dc = NULL;
                //Scale the fallback image places back to chart space (but with page origin at y==0)
                stored_fallback_image_places.Scale(1/(user_scale.x*fake_scale)); //TODO: assumes user_scale.x==user_scale.y
            } else 
                _ASSERT(0);
            break;
            //Fallthrough if cairo has no WIN32 surface
#endif
        case WIN:
            cairo_surface_destroy (surface);
        }
    }
    outFile=NULL;
    surface=NULL;
    cr=NULL;
    fileName.clear();
    if (status==ERR_OK) status = ERR_DONE; //preserve other errors
    candraw = false;
}


cairo_line_join_t Canvas::SetLineJoin(cairo_line_join_t t)
{
    cairo_line_join_t tt = cairo_get_line_join(cr);
    cairo_set_line_join(cr, t);
    return tt;
}
cairo_line_cap_t Canvas::SetLineCap(cairo_line_cap_t t)
{
    cairo_line_cap_t tt = cairo_get_line_cap(cr);
    cairo_set_line_cap(cr, t);
    return tt;
}


void Canvas::Clip(const Block &b, const LineAttr &line)
{
    _ASSERT(line.IsComplete());
    cairo_save(cr);
    if (line.corner.second == CORNER_NOTE && line.IsDoubleOrTriple()) 
        line.CreateRectangle_ForFill(b).CairoPath(cr, true);  //notefill shrinks by Spacing()
    else
        RectanglePath(b.x.from, b.x.till, b.y.from, b.y.till, line);
    cairo_clip(cr);
}

void Canvas::ClipInverse(const Contour &area)
{
    cairo_save(cr);
    Block outer(total);
    outer += area.GetBoundingBox();
    outer.Expand(1);
    cairo_rectangle(cr, outer.x.from, outer.y.from, outer.x.Spans(), outer.y.Spans());
    cairo_new_sub_path(cr);
    area.CairoPath(cr, true);
    cairo_fill_rule_t old = cairo_get_fill_rule(cr);
    cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
    cairo_clip(cr);
    cairo_set_fill_rule(cr, old);
}

/** Rotates the canvas to arbitrary degree around 'center'
 * Any arrow drawn from left to right will show up as rotated clockwise
 * with y coordinate growing downwards*/
void Canvas::Transform_Rotate(const XY &center, double radian)
{
    cairo_save(cr);
    cairo_translate(cr, center.x, center.y);
    cairo_rotate(cr, radian);
    cairo_translate(cr, -center.x, -center.y);
}
                                        
/** Rotate the canvas by 90 degrees.
  @param [in] clockwise True if the rotation is clockwise. That is, 
                        if after transformation you draw
                        a horizontal line to the right (like this ->) , 
                        then it will appear on the canvas as going up.
  @param [in] s This is an y coordinate for clockwise rotation. Points 
                with this y coordinate will have this x coordinate 
                after the translation. For counterclockwise swap x and y.
  @param [in] d This is an y coordinate for clockwise rotation. Points 
                with this y coordinate will have this x coordinate 
                after the translation. For counterclockwise swap x and y.

  I admit I do not get this either.
 */
void Canvas::Transform_Rotate90(double s, double d, bool clockwise)
{
    cairo_save(cr);
    cairo_matrix_t matrix;
    matrix.xx = 0;
    matrix.yy = 0;
    if (clockwise) {
        matrix.xy = 1;
        matrix.x0 = 0;
        matrix.yx = -1;
        matrix.y0 = s+d;
    } else {
        matrix.yx = 1;
        matrix.y0 = 0;
        matrix.xy = -1;
        matrix.x0 = s+d;
    }
    cairo_transform(cr, &matrix);
}

void Canvas::Transform_SwapXY()
{
    cairo_save(cr);
    cairo_matrix_t matrix;
    matrix.xy = 1;
    matrix.yx = 1;
    matrix.xx = 0;
    matrix.yy = 0;
    matrix.x0 = 0;
    matrix.y0 = 0;
    cairo_transform(cr, &matrix);
}

void Canvas::Transform_FlipHorizontal(double y)
{
    cairo_save(cr);
    cairo_matrix_t matrix;
    matrix.xx = 1;
    matrix.xy = 0;
    matrix.x0 = 0;
    matrix.yy = -1;
    matrix.yx = 0;
    matrix.y0 = 2*y;
    cairo_transform(cr, &matrix);
}

void Canvas::SetColor(ColorType pen)
{
	if (pen.type!=ColorType::INVALID) {
        _ASSERT(pen.type==ColorType::COMPLETE);
        if (AvoidTransparency())
            pen = pen.FlattenAlpha();
        cairo_set_source_rgba(cr, pen.r/255.0, pen.g/255.0, pen.b/255.0, pen.a/255.0);
    }
}

void Canvas::SetLineAttr(const LineAttr &line)
{
	if (line.color.first && line.color.second.type!=ColorType::INVALID)
        SetColor(line.color.second);
    if (line.width.first)
        cairo_set_line_width(cr, line.width.second + (avoid_linewidth_1 ? 0.01 : 0.0));
    if (line.type.first)
        SetDash(line);
}

void Canvas::SetDash(const LineAttr &line)
{
    unsigned num;
    const double * const pattern = line.DashPattern(num);
    if (fake_dash) 
        cairo_set_dash(cr, NULL, 0, 0); 
    else if (line.type.first && !fake_dash) 
        cairo_set_dash(cr, pattern, num, 0); 
}

void Canvas::SetFontFace(const char*face, bool italics, bool bold)
{
    cairo_select_font_face (cr, face,
                            italics?CAIRO_FONT_SLANT_ITALIC:CAIRO_FONT_SLANT_NORMAL,
                            bold?CAIRO_FONT_WEIGHT_BOLD:CAIRO_FONT_WEIGHT_NORMAL);
}

void Canvas::Text(XY xy, const string &s, bool isRotated)
{
    _ASSERT(candraw);
    cairo_move_to (cr, xy.x, xy.y);
    if (use_text_path || (isRotated && use_text_path_rotated)) {
        cairo_text_path(cr, s.c_str());
        cairo_fill(cr);
        return;
    }
    if (individual_chars) {
        char tmp_stirng[2] = "a";
        for (unsigned i=0; i<s.length(); i++) {
            tmp_stirng[0] = s[i];
            cairo_show_text(cr, tmp_stirng);
        }
    } else {
        cairo_show_text (cr, s.c_str());
    }
}

void Canvas::ArcPath(const XY &c, double r1, double r2, double s_rad, double e_rad, bool reverse)
{
    cairo_save(cr);
    cairo_translate(cr, c.x, c.y);
    cairo_scale(cr, r1, r2 ? r2 : r1);
    if (reverse)
        cairo_arc_negative(cr, 0., 0., 1., s_rad, e_rad);
    else
        cairo_arc(cr, 0., 0., 1., s_rad, e_rad);
    cairo_restore (cr);
}

void Canvas::RectanglePath(double sx, double dx, double sy, double dy)
{
    cairo_new_path(cr);
    cairo_move_to(cr, sx, sy);
    cairo_line_to(cr, dx, sy);
    cairo_line_to(cr, dx, dy);
    cairo_line_to(cr, sx, dy);
    cairo_close_path(cr);
}

//for CORNER_NOTE it draws the outer edge
void Canvas::RectanglePath(double sx, double dx, double sy, double dy, const LineAttr &line)
{
    cairo_new_path(cr);
    if (!line.radius.first || line.radius.second==0 ||
        !line.corner.first) {
        RectanglePath(sx, dx, sy, dy);
        return;
    }
    const double r = std::min(std::min(fabs(sx-dx)/2, fabs(sy-dy)/2), line.radius.second);
    switch (line.corner.second) {
    default:
        RectanglePath(sx, dx, sy, dy);
        return;
    case CORNER_ROUND:
        ArcPath(XY(sx + r, sy + r), r, r, 2.*M_PI/2., 3.*M_PI/2.);
        ArcPath(XY(dx - r, sy + r), r, r, 3.*M_PI/2., 4.*M_PI/2.);
        ArcPath(XY(dx - r, dy - r), r, r, 0.*M_PI/2., 1.*M_PI/2.);
        ArcPath(XY(sx + r, dy - r), r, r, 1.*M_PI/2., 2.*M_PI/2.);
        cairo_close_path(cr);
        break;
    case CORNER_BEVEL:
        cairo_move_to(cr, sx+r, sy);
        cairo_line_to(cr, dx-r, sy);
        cairo_line_to(cr, dx, sy+r);
        cairo_line_to(cr, dx, dy-r);
        cairo_line_to(cr, dx-r, dy);
        cairo_line_to(cr, sx+r, dy);
        cairo_line_to(cr, sx, dy-r);
        cairo_line_to(cr, sx, sy+r);
        cairo_close_path(cr);
        break;
    case CORNER_NOTE:
        cairo_move_to(cr, sx, sy);
        cairo_line_to(cr, dx-r, sy);
        cairo_line_to(cr, dx, sy+r);
        cairo_line_to(cr, dx, dy);
        cairo_line_to(cr, sx, dy);
        cairo_close_path(cr);
        break;
    }
}

////////////////////// Line routines

void Canvas::singleLine(const Block &b, const LineAttr &line)
{
    _ASSERT(candraw);
    unsigned num=0;
    if (line.IsContinuous() || !fake_dash) 
        RectanglePath(b.x.from, b.x.till, b.y.from, b.y.till, line);
    else {
        const double *pattern = line.DashPattern(num);
        line.CreateRectangle_Midline(b).CairoPathDashed(cr, pattern, num, true);
    }
    cairo_stroke(cr);
}

void Canvas::singleLine(const Contour&cl, const LineAttr &line)
{
    _ASSERT(candraw);
    unsigned num=0;
    if (line.IsContinuous() || !fake_dash) 
        cl.CairoPath(cr, false);
    else {
        const double *pattern = line.DashPattern(num);
        cl.CairoPathDashed(cr, pattern, num, false);
    }
    cairo_stroke(cr);
}

void Canvas::Line(const Edge& edge, const LineAttr &line)
{
    _ASSERT(candraw);
    _ASSERT(line.IsComplete());
    if (line.type.second == LINE_NONE || 
        line.color.second.type==ColorType::INVALID || 
        line.color.second.a==0) return;
    SetLineAttr(line);
    const double spacing = line.Spacing();
    if (line.IsDoubleOrTriple()) {
        std::list<Edge> edges;
        XY d1, d2;
        edge.CreateExpand(spacing, edges,d1,d2);
        if (edges.size()) {
            cairo_move_to(cr, edges.front().GetStart().x, edges.front().GetStart().y);
            for (const auto &e: edges)
                e.PathTo(cr);
            cairo_stroke(cr);
            edges.clear(); 
        }
        edge.CreateExpand(-spacing, edges,d1,d2);
        if (edges.size()) {
            cairo_move_to(cr, edges.front().GetStart().x, edges.front().GetStart().y);
            for (const auto &e: edges)
                e.PathTo(cr);
            cairo_stroke(cr);
        }
        if (line.IsDouble()) 
            return;
        //triple line 
        cairo_set_line_width(cr,  line.TripleMiddleWidth());
        //fallthrougfh
    } 
    if (line.IsContinuous()) {
        cairo_move_to(cr, edge.GetStart().x, edge.GetStart().y);
        edge.PathTo(cr);
        cairo_stroke(cr);
        return; //triple line will exit here as only single lines can be dashed
    } 
    //Some dashed line
    unsigned num=0;
    const double *pattern = line.DashPattern(num);
    int pos = 0;
    double offset=0;
    edge.PathDashed(cr, pattern, num, pos, offset);
    cairo_stroke(cr);
}

void Canvas::Line(const Block &b, const LineAttr &line)
{
    _ASSERT(candraw);
    _ASSERT(line.IsComplete());
	if (line.type.second == LINE_NONE || 
        line.color.second.type==ColorType::INVALID || 
        line.color.second.a==0) return;
    if (b.IsInvalid()) return;
    SetLineAttr(line);
    const double spacing = line.Spacing();
    const double r = line.SaneRadius(b);
    LineAttr line2(line);
    line2.radius.second = r;
    if (line.corner.second!=CORNER_NOTE || r==0) {
        if (line.IsDoubleOrTriple()) {
            line2.Expand(spacing);
            singleLine(b.CreateExpand(spacing), line2);
            line2.radius.second = r;
            line2.Expand(-spacing);
            singleLine(b.CreateExpand(-spacing), line2);
        } 
        if (line.IsTriple()) {
            cairo_set_line_width(cr,  line.TripleMiddleWidth());
            line2.radius.second = r;
        }
        if (!line.IsDouble()) 
            singleLine(b, line2);
    } else {
        //Draw note
        if (line.IsTriple()) {
            //draw inner line
            singleLine(line2.CreateRectangle_ForFill(b)[0], line2);
            //draw outer line
            line2.Expand(spacing);
            singleLine(line2.CreateRectangle_Midline(b.CreateExpand(spacing)), line2);
            //prepare for middle line
            line2.radius.second = r;
            cairo_set_line_width(cr,  line.TripleMiddleWidth());
        } 
        if (line.IsDouble()) {
            //draw inner line + triangle
            singleLine(line2.CreateRectangle_ForFill(b), line2);
            //draw outer line
            line2.radius.second = r;
            line2.Expand(spacing);
            singleLine(b.CreateExpand(spacing), line2);
        } else {
            //for triple and single line
            singleLine(line2.CreateRectangle_Midline(b), line2);
            const Contour tri(b.x.till-r, b.y.from, b.x.till, b.y.from+r, b.x.till-r, b.y.from+r);
            tri[0][0].visible = false;
            singleLine(tri, line);
        }
    }
}

void Canvas::Line(const Contour &c, const LineAttr &line)
{
    _ASSERT(candraw);
    _ASSERT(line.IsComplete());
	if (line.type.second == LINE_NONE || 
        line.color.second.type==ColorType::INVALID || 
        line.color.second.a==0) return;
    if (c.IsEmpty()) return;
    SetLineAttr(line);
    const double spacing = line.Spacing();
    if (line.IsDoubleOrTriple()) {
        singleLine(c.CreateExpand(spacing), line);
        singleLine(c.CreateExpand(-spacing), line);
    } 
    if (line.IsTriple()) cairo_set_line_width(cr,  line.TripleMiddleWidth());
    if (!line.IsDouble()) 
        singleLine(c, line);
}

////The first element in the list is the overall contour, the remaining ones are disjoint ones
////that collectively cover the same area as the first one.
////This is especially useful for double/triple lines
//void MscDrawer::LineWithJoints(const std::list<Contour> &clist, const LineAttr &line)
//{
//    if (clist.size()==0) return;
//    if (clist.size()==1) {
//        Line(*clist.begin(), line);
//        return;
//    }
//    _ASSERT(line.IsComplete());
//	if (line.type.second == LINE_NONE || !line.color.second.valid || line.color.second.a==0) return;
//    SetLineAttr(line);
//    const double spacing = line.IsDouble() ? line.DoubleSpacing() : line.IsTriple() ? line.TripleSpacing() : 0;
//    if (line.IsDoubleOrTriple()) {
//        singleLine(clist.begin()->CreateExpand(spacing), line);
//        for (auto i = ++clist.begin(); i !=clist.end(); i++) 
//            singleLine(i->CreateExpand(-spacing), line);
//    } 
//    if (line.IsTriple()) cairo_set_line_width(cr,  line.TripleMiddleWidth());
//    if (!line.IsDouble()) 
//        for (auto i=clist.begin(); i!=clist.end(); i++)
//            singleLine(*i, line);
//
//}
//


////////////////////// Fill routines


inline void _add_color_stop(cairo_pattern_t *pattern, double offset, ColorType color)
{
    cairo_pattern_add_color_stop_rgba(pattern, offset,
                                      color.r/255., color.g/255.,
                                      color.b/255., color.a/255.);
}

void Canvas::linearGradient(ColorType from, ColorType to, const XY &s, const XY &d, EGradientType type)
{
    cairo_pattern_t *pattern;
    switch(type) {
    case GRADIENT_OUT:
    case GRADIENT_IN:
        //*Should not happen here*/
        assert(0);
        return;
    case GRADIENT_DOWN:
        pattern = cairo_pattern_create_linear(s.x, s.y, s.x, d.y);
        _add_color_stop(pattern, 0, from);
        _add_color_stop(pattern, 1, to);
        break;
    case GRADIENT_UP:
        pattern = cairo_pattern_create_linear(s.x, s.y, s.x, d.y);
        _add_color_stop(pattern, 0, to);
        _add_color_stop(pattern, 1, from);
        break;
    case GRADIENT_LEFT:
        pattern = cairo_pattern_create_linear(s.x, s.y, d.x, s.y);
        _add_color_stop(pattern, 0, to);
        _add_color_stop(pattern, 1, from);
        break;
    case GRADIENT_RIGHT:
        pattern = cairo_pattern_create_linear(s.x, s.y, d.x, s.y);
        _add_color_stop(pattern, 0, from);
        _add_color_stop(pattern, 1, to);
        break;
    case GRADIENT_BUTTON:
        pattern = cairo_pattern_create_linear(s.x, s.y, s.x, d.y);
        _add_color_stop(pattern, 0,    to.Lighter(0.8));
        _add_color_stop(pattern, 0.43, to);
        _add_color_stop(pattern, 0.5,  to.Darker(0.1));
        _add_color_stop(pattern, 1,    to);
        break;
    case GRADIENT_NONE:
    default:
        pattern = cairo_pattern_create_rgba(from.r/255., from.g/255., from.b/255., from.a/255.);
        break;
    }
    cairo_set_source(cr, pattern);
    cairo_pattern_destroy(pattern);
}

void Canvas::fakeLinearGrad(ColorType from, ColorType to, const XY &s, const XY &d, 
                               bool dir_is_x, unsigned steps)
{
    _ASSERT(candraw);
    if (steps==0) steps = 1;
    double advance, current, till;
    if (dir_is_x) {
        current = s.x; till = d.x;
    } else {
        current = s.y; till = d.y;
    }
    //if (till-current < steps) steps = till-current; //Not finer than 1.
    advance = (till-current)/steps;

    double r = from.r/255.0, dr = (double(to.r)-double(from.r))/steps/255.0;
    double g = from.g/255.0, dg = (double(to.g)-double(from.g))/steps/255.0;
    double b = from.b/255.0, db = (double(to.b)-double(from.b))/steps/255.0;
    double a = from.a/255.0, da = (double(to.a)-double(from.a))/steps/255.0;

    while (current < till) {
        if (dir_is_x)
            cairo_rectangle(cr, current, s.y, std::min(till-current, advance), d.y-s.y);
        else
            cairo_rectangle(cr, s.x, current, d.x-s.x, std::min(till-current, advance));
        cairo_set_source_rgba(cr, r, g, b, a);
        cairo_fill(cr);
        current += advance;
        r+=dr; g+=dg; b+=db; a+=da;
    }
}

void Canvas::radialGradient(ColorType from, ColorType to, const XY &s, double outer_radius, double inner_radius, EGradientType type)
{
    cairo_pattern_t *pattern;
    switch(type) {
    default:
        assert(0);
        return;
    case GRADIENT_OUT:
        pattern = cairo_pattern_create_radial(s.x, s.y, inner_radius, s.x, s.y, outer_radius);
        _add_color_stop(pattern, 0, from);
        _add_color_stop(pattern, 1, to);
        break;
    case GRADIENT_IN:
        pattern = cairo_pattern_create_radial(s.x, s.y, inner_radius, s.x, s.y, outer_radius);
        _add_color_stop(pattern, 0, to);
        _add_color_stop(pattern, 1, from);
        break;
    case GRADIENT_NONE:
        pattern = cairo_pattern_create_rgba(from.r/255., from.g/255., from.b/255., from.a/255.);
        break;
    }
    cairo_set_source(cr, pattern);
}

//from is from the outside, caller must apply clipping!!!
void Canvas::fakeRadialGrad(ColorType from, ColorType to, const XY &s, double outer_radius, double inner_radius,
                 unsigned steps, bool rectangle, double rad_from, double rad_to)
{
    _ASSERT(candraw);
    //Normalize radiuses
    if (outer_radius < inner_radius)
        std::swap(inner_radius, outer_radius);
    if (outer_radius==0) return;
    if (outer_radius == inner_radius)
        outer_radius++;

    //Initial full-color bkground
    if (rectangle) {
        cairo_rectangle(cr, s.x-outer_radius, s.y-outer_radius, s.x+outer_radius, s.y+outer_radius);
        cairo_set_source_rgba(cr, from.r/255., from.g/255., from.b/255., from.a/255.);
        cairo_fill(cr);
    }
    if (steps==0) steps = 1;
    //if (outer_radius-inner_radius < steps) steps = outer_radius-inner_radius; //Not finer than 1.
    double advance = double(outer_radius-inner_radius)/steps;
    double current = outer_radius;

    double r = from.r/255.0, dr = (double(to.r)-double(from.r))/steps/255.0;
    double g = from.g/255.0, dg = (double(to.g)-double(from.g))/steps/255.0;
    double b = from.b/255.0, db = (double(to.b)-double(from.b))/steps/255.0;
    double a = from.a/255.0, da = (double(to.a)-double(from.a))/steps/255.0;

    while (current > inner_radius) {
        cairo_move_to(cr, s.x, s.y);
        cairo_arc (cr, s.x, s.y, current, rad_from, rad_to);
        cairo_set_source_rgba(cr, r, g, b, a);
        cairo_fill(cr);
        current -= advance;
        r+=dr; g+=dg; b+=db; a+=da;
    }
}

void Canvas::Fill(const Block &b, const FillAttr &fill)
{
    _ASSERT(candraw);
    _ASSERT(fill.IsComplete());
    if (fill.color.second.type==ColorType::INVALID || 
        fill.color.second.a==0) return;
	const double max_extent = std::max(fabs(b.x.till-b.x.from), fabs(b.y.till-b.y.from));
	//If gradients are to be drawn, we fake them only if no alpha is present
	//If we draw somewhat transparent, we will fall back to images anyway,
	//so let us not fake gradients either
    if (fill.gradient.second!=GRADIENT_NONE && fake_gradients && fill.color.second.a==255) {
        Clip(b);
        ColorType color = fill.color.second;
        ColorType color2 = fill.color2.first ? fill.color2.second : fill.color.second.Lighter(0.8);
        if (AvoidTransparency()) {
            color.FlattenAlpha();
            color2.FlattenAlpha();
        }
        switch(fill.gradient.second) {
        default:
            _ASSERT(0);
            break;
        case GRADIENT_OUT:
            fakeRadialGrad(color, color2, b.Centroid(), max_extent, 0, fake_gradients, true);
            break;
        case GRADIENT_IN:
            fakeRadialGrad(color2, color, b.Centroid(), max_extent, 0, fake_gradients, true);
            break;
        case GRADIENT_DOWN:
            fakeLinearGrad(color2, color, b.UpperLeft(), b.LowerRight(), false, fake_gradients);
            break;
        case GRADIENT_UP:
            fakeLinearGrad(color, color2, b.UpperLeft(), b.LowerRight(), false, fake_gradients);
            break;
        case GRADIENT_RIGHT:
            fakeLinearGrad(color2, color, b.UpperLeft(), b.LowerRight(), true, fake_gradients);
            break;
        case GRADIENT_LEFT:
            fakeLinearGrad(color2, color2, b.UpperLeft(), b.LowerRight(), true, fake_gradients);
            break;
        case GRADIENT_BUTTON:
            double step = (b.y.till-b.y.from)/100;
            fakeLinearGrad(color.Lighter(0.66), color.Lighter(0.35), 
                b.UpperLeft(), b.LowerRight()-XY(0, step*66), false, unsigned(fake_gradients*0.34));
            fakeLinearGrad(color.Lighter(0.16), color, 
                b.UpperLeft()+XY(0, step*34), b.LowerRight(), false, unsigned(fake_gradients*0.66));
            break;
        }
        UnClip();
    } else {
        ColorType to = fill.color.second, from;
        if (!fill.color2.first) 
            switch(fill.gradient.second) {
            case GRADIENT_OUT:
            case GRADIENT_IN:
                from = to.Lighter(0.6);
                break;
            case GRADIENT_DOWN:
            case GRADIENT_UP:
            case GRADIENT_LEFT:
            case GRADIENT_RIGHT:
                from = to.Lighter(0.8);
                break;
            default:
                from = to; //to is not used for BUTTON and NONE
            }
        else 
            from = fill.color2.second;
        if (AvoidTransparency()) {
            from.FlattenAlpha();
            to.FlattenAlpha();
        }
        if (fill.gradient.second ==GRADIENT_IN || fill.gradient.second == GRADIENT_OUT) 
            radialGradient(from, to, b.Centroid(), max_extent, 0, fill.gradient.second);
         else 
            linearGradient(from, to, b.UpperLeft(), b.LowerRight(), fill.gradient.second);
        RectanglePath(b.x.from, b.x.till, b.y.from, b.y.till);
        cairo_fill(cr);
    }
}

////////////////////// Shadow routines

//Angle degree is 0 then shadow is offset +off in both y and x dir
//angle grows clockwise (y grows downward) and means the rotation of the object. 
//From that we need to calculate the offset of the shadow assuming that the space is already rotated.
void Canvas::Shadow(const Contour &area, const ShadowAttr &shadow, double angle_radian)
{
    _ASSERT(candraw);
    _ASSERT(shadow.IsComplete());
    if (shadow.offset.second==0 || area.IsEmpty()) return;
    const bool clip = false;
    //Clip out the actual shape we are the shadow of
    if (clip) {
        area.CairoPath(cr, true);
        cairo_new_sub_path(cr);
        cairo_rectangle(cr, area.GetBoundingBox().x.from-1, area.GetBoundingBox().y.from-1,
                            area.GetBoundingBox().x.Spans()+2+shadow.offset.second, area.GetBoundingBox().y.Spans()+2+shadow.offset.second);
        cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
        cairo_clip(cr);
    }
    const Contour &substract = area;//.CreateExpand(-0.5);
    Contour outer(area);
    XY off(shadow.offset.second, shadow.offset.second);
    off.Rotate(cos(-angle_radian), sin(-angle_radian));
    outer.Shift(off);
    ColorType color = shadow.color.second;
    if (shadow.blur.second>0) {
        const Contour original(outer);
        const unsigned steps = unsigned(std::min(shadow.blur.second, shadow.offset.second)*scale_for_shadows + 0.5);
        const double transp_step = double(color.a)/(steps+1);
        double alpha = 0;
        for (unsigned i=0; i<steps; i++) {
            const double ex = double(i+1)/-scale_for_shadows;
            Contour inner = original.CreateExpand(ex);
            if (inner.IsEmpty())
                break;

            alpha += transp_step;
            color.a = (unsigned char)alpha;
            if (fake_shadows)
                SetColor(ColorType(color).FlattenAlpha());
            else
                SetColor(color);
            //since clip is not working so well, we substract the original area
            ((outer-inner)-substract).Fill(cr);
            std::swap(inner, outer);
        }
    }
    if (shadow.blur.second <= shadow.offset.second) {
        if (fake_shadows)  //we still use a blurred shadow color
            SetColor(ColorType(shadow.color.second).FlattenAlpha());
        else
            SetColor(shadow.color.second);
    }
    (outer-substract).Fill(cr);
    if (clip)
        cairo_restore(cr);
}

/* Set clip, if the rectangle of which this is the shadow of is not opaque */
void Canvas::Shadow(const Block &b, const LineAttr &line, const ShadowAttr &shadow, double angle_radian)
{
    _ASSERT(candraw);
    _ASSERT(shadow.IsComplete() && line.radius.first);
    if (shadow.offset.second==0) return;
    if (shadow.color.second.IsFullyTransparent()) return;
    //For now just call the other Shadow Routine
    Contour c = line.CreateRectangle_OuterEdge(b);
    c.Expand(-line.width.second);
    Shadow(std::move(c), shadow, angle_radian);
    return;
}

/* END OF FILE */
