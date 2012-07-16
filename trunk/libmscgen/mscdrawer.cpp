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
#include <algorithm>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <cassert>
#include <cairo-ps.h>
#include <cairo-pdf.h>
#include <cairo-svg.h>
#include "mscdrawer.h"

#ifndef M_PI
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


/* This is a class implementing a lot of useful helpers for drawing on top of cairo.
 * These include faking some effects for some targets (such as gradient or dash lines for WMF) and
 * drawing/filling complete contours plus shadowing them.
 *
 * Comments on drawing.
 * Cairo draws lines on integer coordinates halfway to both adoining pixels.
 * This should be avoided, especially for thin lines.
 * However, we do not do it here - call each Fill/Line etc function with the appropriate coordinates.
 */

//Use this to detemine positions/layout
MscCanvas::MscCanvas(OutputType ot) : 
    fake_dash_offset(0), outFile(NULL), surface(NULL), cr(NULL), 
    outType(ot), total(0,0,0,0), status(ERR_PARAM), candraw(false)
#ifdef CAIRO_HAS_WIN32_SURFACE
	, win32_dc(NULL), original_wmf_hdc(NULL)
#endif
{
    SetLowLevelParams(ot);
    status = CreateSurface(XY(0,0));
    if (status!=ERR_OK) return;
    status = CreateContextFromSurface(ot, XY(1.,1.), 0, 0, 0);
    if (status!=ERR_OK) CloseOutput();
}

//page numbering starts from 0
//Use this to save to a file 
MscCanvas::MscCanvas(OutputType ot, const Block &tot, double copyrightTextHeight, 
                     const string &fn, const XY &scale,
                     const std::vector<double> *yPageStart, unsigned page) :
    fake_dash_offset(0), outFile(NULL), surface(NULL), cr(NULL), outType(ot), 
    total(tot), status(ERR_PARAM), candraw(false)
#ifdef CAIRO_HAS_WIN32_SURFACE
	, win32_dc(NULL), original_wmf_hdc(NULL)
#endif
{
    _ASSERT(ot!=WIN);
    _ASSERT(fn.length()>0);
    if (fn.length()==0 || ot==WIN || ot==PRINTER) 
        return; //fail

    SetLowLevelParams(ot);
    
    double origYSize, origYOffset;
    GetPagePosition(yPageStart, page, origYOffset, origYSize);
    XY size(total.x.Spans(), origYSize);
    size.y += copyrightTextHeight;
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
        if (!outFile) return;
    }
    status = CreateSurface(size);
    if (status!=ERR_OK) return;
    status = CreateContextFromSurface(ot, scale, origYSize, origYOffset, copyrightTextHeight);
    if (status!=ERR_OK) CloseOutput();
    else candraw = true;
}

cairo_status_t write_func(void * closure, const unsigned char *data, unsigned length)
{
    if (closure==NULL) return CAIRO_STATUS_SUCCESS;
    if (length == fwrite(data, 1, length, (FILE*)closure))
        return CAIRO_STATUS_SUCCESS;
    else
        return CAIRO_STATUS_WRITE_ERROR;
}

#ifdef CAIRO_HAS_WIN32_SURFACE
int GetWindowsVersion() 
{
    static int cache = -1;
    if (cache<0) {
        OSVERSIONINFOEX osvi;
        ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
        //majorversion of 5 is Win2000, XP and 2003, 6 is Vista, 2008 and Win7
        if(GetVersionEx ((OSVERSIONINFO *) &osvi)) 
            cache = osvi.dwMajorVersion;
    }
    return cache;
}
#endif

void MscCanvas::SetLowLevelParams(MscCanvas::OutputType ot)
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
    imprecise_positioning = false;
    can_and_shall_clip_total = true;
    avoid_transparency = false;
    white_background = false;
    fake_scale = 1.0;          //normally we map our coordinates 1:1 onto the canvas
    fallback_resolution = 300; //this will be irrelevant for bitmaps - they use the native resolution
    needs_dots_in_corner = false;

    //Customize as per target type
    switch (ot) {
    case MscCanvas::PNG:
        white_background = true;
        break;
#ifdef CAIRO_HAS_WIN32_SURFACE
    case WMF:
        if (total.x.Spans()>0 && total.y.Spans()>0)
            fake_scale = std::min(std::min(30000./total.x.Spans(), 30000./total.y.Spans()), 10.);  
    case PRINTER:
        individual_chars = true;        //do this so that it is easier to convert to WMF
        use_text_path_rotated = true;   //WMF has no support for this
        fake_dash = true;               //WMF has no support for this
        needs_arrow_fix = true;         //WMF does not support complex clipping it seems
        //determine scaling so that no coordinate is larger than 30K - a built-in limit of WMF
        //but scale up, so that coordinates are large, since WMF handles only integer coordinates,
        //thus fonts are bad if we do not scale up.
        //So if we have total calculated already, we select fake_scale as 10, or smaller if this would result in >30K coords.
        //Setting fake_scale higher than 10 seems to result in wrong image fallback positioning, I am not sure why.
        avoid_transparency = GetWindowsVersion()<=5; //on XP transparency happens wrong
        //Fallthrough
    case EMF:
        needs_dots_in_corner = true;
        imprecise_positioning = true;
        fake_gradients = 30;
        fake_shadows = true;
        //check if we run on vista or later: then cairo can do text on EMF/WMF 
        if(GetWindowsVersion()<=5) {
            use_text_path = true;
            use_text_path_rotated = true;
            //fallback_resolution = 100; //on XP fallback shall be small, or not?         
        }
        break;
    case WIN:
        can_and_shall_clip_total = false;
#endif
    default:
        break;
    }
}

//page numbering starts from 0, -1 means all of the chart (ignoring page breaks)
//This one does not take bottom banner into account
void MscCanvas::GetPagePosition(const std::vector<double> *yPageStart, unsigned page, double &y_offset, double &y_size) const
{
    if (page==0 || yPageStart==NULL || yPageStart->size()==0) {
        y_offset = total.y.from;
        y_size = total.y.Spans();
    } else if (page>yPageStart->size()) { //too large page
        y_offset = total.y.till;
        y_size = 0; //nothing drawn
    } else if (page==yPageStart->size()) { //last page
        y_offset = yPageStart->at(page-1);
        y_size = total.y.till - yPageStart->at(page-1);
    } else {
        y_size = yPageStart->at(page) - yPageStart->at(page-1);
        y_offset =  yPageStart->at(page-1);
    }
}


//Creates a surface of size XY (can be null)
//uses "outType" and "fileName" members
//returns true is success
MscCanvas::ErrorType MscCanvas::CreateSurface(const XY &size)
{
    const int x = int(size.x);
    const int y = int(size.y);
    switch (outType) {
    default:
        return ERR_PARAM;
#ifdef CAIRO_HAS_PNG_FUNCTIONS
    case MscCanvas::PNG:
        surface = cairo_image_surface_create (CAIRO_FORMAT_RGB24, x, y);
        break;
#endif
#ifdef CAIRO_HAS_PS_SURFACE
    case MscCanvas::EPS:
        surface = cairo_ps_surface_create_for_stream(write_func, outFile, x, y);
        cairo_ps_surface_set_eps(surface, true);
        break;
#endif
#ifdef CAIRO_HAS_PDF_SURFACE
    case MscCanvas::PDF:
        surface = cairo_pdf_surface_create_for_stream(write_func, outFile, x, y);
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,10,0)
        //Limit to PDF version 1.4 if possible (appeared in cairo 1.10)
        //since no features of higher are relevant and Texinfo doc compilation 
        //produces warnings with 1.5
        if (cairo_surface_status(surface) == CAIRO_STATUS_SUCCESS)
            cairo_pdf_surface_restrict_to_version(surface, CAIRO_PDF_VERSION_1_4);
        break;
  #endif
#endif
#ifdef CAIRO_HAS_SVG_SURFACE
    case MscCanvas::SVG:
        surface = cairo_svg_surface_create_for_stream(write_func, outFile, x, y);
        break;
#endif
#ifdef CAIRO_HAS_WIN32_SURFACE
    case MscCanvas::EMF:
        // Create the Enhanced Metafile
        if (fileName.length() > 0)
            win32_dc = CreateEnhMetaFile(NULL, fileName.c_str(), NULL, "Msc-generator\0\0");
        else
            win32_dc = CreateEnhMetaFile(NULL, NULL, NULL, NULL);
        // Did you get a good metafile?
        if( win32_dc == NULL ) 
            return ERR_CANVAS;
        /* We need a tweak in cairo.
		 ** I modified cairo_win32_printing_surface_create diretcly to use 
		 ** an extent bigger than the one reported by GetClipBox(), if the 
		 ** DC passed is an EMF DC.
         ** This is to avoid dropping glyphs outside the DC's extent.
         ** I could not make a metafile DC to have larger extent that 1600x1200
         */
        surface = cairo_win32_printing_surface_create(win32_dc);
        break;
	case MscCanvas::WMF:
        // Create the Metafile on disk
        if (fileName.length() > 0)
            original_wmf_hdc = CreateMetaFile(fileName.c_str());
        else
            original_wmf_hdc = CreateMetaFile(NULL);
        // Did you get a good metafile?
        if( original_wmf_hdc == NULL ) 
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
        return ERR_CANVAS;
    }
    cairo_surface_set_fallback_resolution(surface, fallback_resolution/fake_scale, fallback_resolution/fake_scale);
    return ERR_OK;
}

//Creates a context from the surface
MscCanvas::ErrorType MscCanvas::CreateContextFromSurface(MscCanvas::OutputType /*ot*/, const XY &scale, double origYSize, double origYOffset, double copyrightTextHeight) 
{
    cr = cairo_create (surface);
    cairo_status_t st = cairo_status(cr);
    if (st != CAIRO_STATUS_SUCCESS) {
        CloseOutput();
        return ERR_CANVAS;
    }
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);
    if (total.x.Spans() <= 0 || total.y.Spans()<=0) return ERR_OK; //OK

    scale_for_shadows = sqrt(scale.y*scale.x);
    cairo_scale(cr, fake_scale*scale.x, fake_scale*scale.y);
    if (white_background) {
        cairo_rectangle(cr, 0, 0, total.x.Spans(), origYSize+copyrightTextHeight);
        cairo_set_source_rgb(cr, 1., 1., 1.);
        cairo_fill(cr);
    }
    if (can_and_shall_clip_total) {
        //clip first to allow for banner text, but not for WIN contexts
        cairo_save(cr);
        cairo_rectangle(cr, 0, 0, total.x.Spans(), origYSize+copyrightTextHeight);
        cairo_clip(cr);
    }
    if (needs_dots_in_corner) {
        //dots in the corner for EMF
        cairo_set_source_rgb(cr, 1, 1, 1);
        cairo_move_to(cr, 0,               origYSize+copyrightTextHeight);
        cairo_line_to(cr, total.x.Spans(), origYSize+copyrightTextHeight);
        cairo_stroke(cr);
    }
    cairo_translate(cr, -total.x.from, -origYOffset);
    if (can_and_shall_clip_total) {
        //then clip again to exclude banner text, but not for WIN contexts
        cairo_save(cr);
        cairo_rectangle(cr, total.x.from, origYOffset, total.x.Spans(), origYSize);
        cairo_clip(cr);
    }
    return ERR_OK;
}

#ifdef CAIRO_HAS_WIN32_SURFACE

//Draw to a DC that is either a display/printer DC or a metafile
//Use this to display a chart, use other constructors without a DC to save the chart to a file
MscCanvas::MscCanvas(OutputType ot, HDC hdc, const Block &tot, double copyrightTextHeight, 
                     const XY &scale, const std::vector<double> *yPageStart, unsigned page) :
    fake_dash_offset(0), outFile(NULL), surface(NULL), cr(NULL), outType(ot), 
    total(tot), status(ERR_PARAM), candraw(false), win32_dc(NULL), original_wmf_hdc(NULL)
{
    if (ot!=WIN && ot!=WMF && ot!=EMF && ot!=PRINTER) 
        return;

    SetLowLevelParams(ot);

    double origYSize, origYOffset;
    GetPagePosition(yPageStart, page, origYOffset, origYSize);
    XY size(total.x.Spans(), origYSize);
    size.y += copyrightTextHeight;
    size.x *= fake_scale*scale.x;
    size.y *= fake_scale*scale.y;
    RECT r;

    switch (ot) {
    case MscCanvas::WIN:
        surface = cairo_win32_surface_create(hdc);
        break;
    case MscCanvas::EMF:
        surface = cairo_win32_printing_surface_create(hdc);
        break;
    case MscCanvas::WMF:
    case MscCanvas::PRINTER:
        //Save the extent (including this page, the copyright, scale & fake_scale)
        //SetRect(&r,0,0,(int)size.x, (int)size.y);
        //win32_dc = CreateEnhMetaFile(NULL, NULL, &r, NULL);
        win32_dc = CreateEnhMetaFile(NULL, NULL, NULL, NULL);
        if( win32_dc == NULL ) return;
        original_wmf_hdc = hdc;
        original_size_for_printing = size;
        surface = cairo_win32_printing_surface_create(win32_dc);
        break;
    default:
        _ASSERT(0);
    }
    cairo_surface_set_fallback_resolution(surface, fallback_resolution/fake_scale, fallback_resolution/fake_scale);
    status = CreateContextFromSurface(ot, scale, origYSize, origYOffset, copyrightTextHeight);
    if (status!=ERR_OK) CloseOutput();
    else candraw = true;
}

typedef struct tagEXTTO { // lf
    _int16 toY;
    _int16 toX;
    _int16 toSTRLEN;
    _int16 toFWOPTS;
    char   string[1];
} EXTTO;

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

int CALLBACK EnumProc(HDC hDC,                // handle to DC
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
        ::MoveToEx(hDC, pTO->toX, pTO->toY, NULL);
        ::LineTo(hDC, pTO->toX, pTO->toY+10);
    } else 
    if (lpMFR->rdFunction == META_CREATEFONTINDIRECT) {
        LOGFONT16* const plogfont = ( LOGFONT16* )(&lpMFR->rdParm[0]);
        plogfont->lfHeight = plogfont->lfHeight / 32;   //When this is negative character height is meant, when positive cell height (cell_h = character_h + lead). Keep sign!
        b=PlayMetaFileRecord(hDC, lpHTable, lpMFR, nObj);
    } else {
        b=PlayMetaFileRecord(hDC, lpHTable, lpMFR, nObj);
    }
    //if (!b) FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,NULL,GetLastError(),0, buff,4096,NULL );
    return 1;
}

void PaintEMFonWMFdc(HENHMETAFILE hemf, HDC hdc, const RECT &r, bool applyTricks)
{
    if (hemf == NULL || hdc == NULL) return;
    HDC refDC = GetDC(NULL);
    SetMapMode(refDC, MM_ANISOTROPIC);
    SetWindowExtEx(refDC, r.right, r.bottom, NULL);
    SetViewportExtEx(refDC, r.right, r.bottom, NULL);
    unsigned size = GetWinMetaFileBits(hemf, 0, NULL, MM_ANISOTROPIC, hdc);
    BYTE *buff = (BYTE*)malloc(size);
	if (buff==NULL) {
		ReleaseDC(NULL, refDC);
		return;
	}
    size = GetWinMetaFileBits(hemf, size, buff, MM_ANISOTROPIC, hdc);
    ReleaseDC(NULL, refDC);
    HMETAFILE hwmf = SetMetaFileBitsEx(size, buff);
    free(buff);
    if (applyTricks)
        EnumMetaFile(hdc, hwmf, EnumProc, NULL);
    else
        PlayMetaFile(hdc, hwmf);
    DeleteMetaFile(hwmf);
}

//HENHMETAFILE MscCanvas::CloseOutputRetainHandleEMF()
//{
//    if (status!=ERR_OK) return NULL;
//    cairo_destroy(cr);
//    outFile=NULL;
//    cr=NULL;
//    fileName.clear();
//    if (surface==NULL) return NULL;
//    cairo_status_t st = cairo_surface_status(surface);
//    cairo_surface_show_page(surface);
//    cairo_surface_destroy(surface);
//    surface=NULL;
//    if (outType == EMF || outType == WMF) 
//        return CloseEnhMetaFile(win32_dc);
//    return NULL;
//}
//HMETAFILE MscCanvas::CloseOutputRetainHandleWMF()
//{
//    if (status!=ERR_OK) return NULL;
//    cairo_destroy(cr);
//    outFile=NULL;
//    cr=NULL;
//    fileName.clear();
//    if (surface==NULL) return NULL;
//    cairo_status_t st = cairo_surface_status(surface);
//    cairo_surface_show_page(surface);
//    cairo_surface_destroy(surface);
//    surface=NULL;
//    if (outType == EMF || outType == WMF) {
//        HENHMETAFILE hemf = CloseEnhMetaFile(win32_dc);
//        RECT r;
//        SetRect(&r, 0, 0, int(total.x), int(total.y));
//        HDC hdc = CreateMetaFile(NULL);
//        PaintEMFonWMFdc(hemf, hdc, r, true); //true means we fiddle with fonts & textout
//        DeleteEnhMetaFile(hemf);
//        return CloseMetaFile(hdc);
//    }
//    return NULL;
//}
#endif //WIN32

void MscCanvas::PrepareForCopyrightText()
{
    if (status==ERR_OK && can_and_shall_clip_total)
        UnClip();
}

void MscCanvas::CloseOutput()
{
    if (status!=ERR_OK) 
        return;
    /* Destroy our context */
    if (cr)
        cairo_destroy (cr);

    if (surface) {
        /* Output the image to the disk file in PNG format. */
        switch (outType) {
#ifdef CAIRO_HAS_PNG_FUNCTIONS
        case MscCanvas::PNG:
            if (outFile)
                cairo_surface_write_to_png_stream (surface, write_func, outFile);
            //fallthrough
#endif
        case MscCanvas::EPS:
        case MscCanvas::PDF:
        case MscCanvas::SVG:
            cairo_surface_destroy (surface);
            if (outFile)
                fclose(outFile);
            break;
        case MscCanvas::EMF:
        case MscCanvas::WMF:
        case MscCanvas::PRINTER:
#ifdef CAIRO_HAS_WIN32_SURFACE
            cairo_surface_show_page(surface);
            cairo_surface_destroy (surface);
            if (outType==MscCanvas::EMF) {
                if (win32_dc)
                    DeleteEnhMetaFile(CloseEnhMetaFile(win32_dc)); //this does not delete the metafile on disk, if so
            } else { //WMF or PRINTER
                if (original_wmf_hdc) { //Opened via MscCanvas() with an existing HDC
                    HENHMETAFILE hemf = CloseEnhMetaFile(win32_dc);
                    //Get saved extent (includes scale & fake_scale)
                    //ENHMETAHEADER emh;
                    //GetEnhMetaFileHeader(hemf, sizeof(emh), &emh);
                    RECT r;
                    //SetRect(&r, emh.rclFrame.left, emh.rclFrame.top, emh.rclFrame.right, emh.rclFrame.bottom);
                    //SetRect(&r, 0, 0, int(total.x.Spans()), int(total.y.Spans()));
                    SetRect(&r, 0, 0, int(original_size_for_printing.x), int(original_size_for_printing.y));
                    HDC hDCmeta = CreateEnhMetaFile(NULL, NULL, NULL, NULL);
                    PaintEMFonWMFdc(hemf, hDCmeta, r, true); 
                    HENHMETAFILE hemf2 = CloseEnhMetaFile(hDCmeta);
                    PlayEnhMetaFile(original_wmf_hdc, hemf2, &r);
                    DeleteEnhMetaFile(hemf2);
                    ::MoveToEx(original_wmf_hdc, r.left, r.top, NULL);
                    ::LineTo(original_wmf_hdc, r.right, r.bottom);
                    DeleteEnhMetaFile(hemf);
                    original_wmf_hdc = NULL;
                } else { //Opened via MscCanvas() with a filename, win32_dc is an EMF DC
                    DeleteEnhMetaFile(CloseEnhMetaFile(win32_dc));
                }
            }
            win32_dc = NULL;
            break;
            //Fallthrough if no WIN32 surface
#endif
        case MscCanvas::WIN:
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


cairo_line_join_t MscCanvas::SetLineJoin(cairo_line_join_t t)
{
    cairo_line_join_t tt = cairo_get_line_join(cr);
    cairo_set_line_join(cr, t);
    return tt;
}
cairo_line_cap_t MscCanvas::SetLineCap(cairo_line_cap_t t)
{
    cairo_line_cap_t tt = cairo_get_line_cap(cr);
    cairo_set_line_cap(cr, t);
    return tt;
}


void MscCanvas::Clip(const contour::EllipseData &ellipse)
{
    cairo_save(cr);
    cairo_save(cr);
    ellipse.TransformForDrawing(cr);
    cairo_arc(cr, 0, 0, 1, 0, 2*M_PI);
    cairo_restore(cr);
    cairo_clip(cr);
}

void MscCanvas::Clip(const Block &b, const MscLineAttr &line)
{
    _ASSERT(line.IsComplete());
    cairo_save(cr);
    if (line.corner.second == CORNER_NOTE && line.IsDoubleOrTriple()) 
        line.CreateRectangle_ForFill(b).Path(cr, true);  //notefill shrinks by Spacing()
    else
        RectanglePath(b.x.from, b.x.till, b.y.from, b.y.till, line);
    cairo_clip(cr);
}

void MscCanvas::ClipInverse(const Contour &area)
{
    cairo_save(cr);
    Block outer(total);
    outer += area.GetBoundingBox();
    outer.Expand(1);
    cairo_rectangle(cr, outer.x.from, outer.y.from, outer.x.Spans(), outer.y.Spans());
    cairo_new_sub_path(cr);
    area.Contour::Path(cr, true);
    cairo_fill_rule_t old = cairo_get_fill_rule(cr);
    cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
    cairo_clip(cr);
    cairo_set_fill_rule(cr, old);
}

//rotates such that a 
void MscCanvas::Transform_Rotate90(double s, double d, bool clockwise)
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

void MscCanvas::Transform_SwapXY()
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

void MscCanvas::Transform_FlipHorizontal(double y)
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

void MscCanvas::SetColor(MscColorType pen)
{
	if (pen.valid) {
        if (AvoidTransparency())
            pen = pen.FlattenAlpha();
        cairo_set_source_rgba(cr, pen.r/255.0, pen.g/255.0, pen.b/255.0, pen.a/255.0);
    }
}

void MscCanvas::SetLineAttr(MscLineAttr line)
{
	if (line.color.first && line.color.second.valid)
        SetColor(line.color.second);
    if (line.width.first)
        cairo_set_line_width(cr, line.width.second);
    if (line.type.first)
        SetDash(line);
}

void MscCanvas::SetDash(MscLineAttr line)
{
    unsigned num;
    const double * const pattern = line.DashPattern(num);
    if (fake_dash) 
        cairo_set_dash(cr, NULL, 0, 0); 
    else if (line.type.first && !fake_dash) 
        cairo_set_dash(cr, pattern, num, 0); 
}

void MscCanvas::SetFontFace(const char*face, bool italics, bool bold)
{
    cairo_select_font_face (cr, face,
                            italics?CAIRO_FONT_SLANT_ITALIC:CAIRO_FONT_SLANT_NORMAL,
                            bold?CAIRO_FONT_WEIGHT_BOLD:CAIRO_FONT_WEIGHT_NORMAL);
}

void MscCanvas::Text(XY xy, const string &s, bool isRotated)
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

void MscCanvas::ArcPath(const contour::EllipseData &ell, double s_rad, double e_rad, bool reverse)
{
    cairo_save (cr);
    ell.TransformForDrawing(cr);
    if (reverse)
        cairo_arc_negative(cr, 0., 0., 1., s_rad, e_rad);
    else
        cairo_arc(cr, 0., 0., 1., s_rad, e_rad);
    cairo_restore (cr);
}

void MscCanvas::ArcPath(const XY &c, double r1, double r2, double s_rad, double e_rad, bool reverse)
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

void MscCanvas::RectanglePath(double sx, double dx, double sy, double dy)
{
    cairo_new_path(cr);
    cairo_move_to(cr, sx, sy);
    cairo_line_to(cr, dx, sy);
    cairo_line_to(cr, dx, dy);
    cairo_line_to(cr, sx, dy);
    cairo_close_path(cr);
}

//for CORNER_NOTE it draws the outer edge
void MscCanvas::RectanglePath(double sx, double dx, double sy, double dy, const MscLineAttr &line)
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

void MscCanvas::singleLine(const Block &b, const MscLineAttr &line)
{
    _ASSERT(candraw);
    unsigned num=0;
    if (line.IsContinuous() || !fake_dash) 
        RectanglePath(b.x.from, b.x.till, b.y.from, b.y.till, line);
    else {
        const double *pattern = line.DashPattern(num);
        line.CreateRectangle_Midline(b).PathDashed(cr, pattern, num, true);
    }
    cairo_stroke(cr);
}

void MscCanvas::singleLine(const Contour&cl, const MscLineAttr &line)
{
    _ASSERT(candraw);
    unsigned num=0;
    if (line.IsContinuous() || !fake_dash) 
        cl.Path(cr, false);
    else {
        const double *pattern = line.DashPattern(num);
        cl.PathDashed(cr, pattern, num, false);
    }
    cairo_stroke(cr);
}

void MscCanvas::Line(const Edge& edge, const MscLineAttr &line)
{
    _ASSERT(candraw);
    _ASSERT(line.IsComplete());
	if (line.type.second == LINE_NONE || !line.color.second.valid || line.color.second.a==0) return;
    SetLineAttr(line);
    const double spacing = line.Spacing();
    if (line.IsDoubleOrTriple()) {
        Edge e = edge;
        e.Expand(spacing);
        cairo_move_to(cr, e.GetStart().x, e.GetStart().y);
        e.PathTo(cr);
        cairo_stroke(cr);
        e.Expand(-2*spacing);
        cairo_move_to(cr, e.GetStart().x, e.GetStart().y);
        e.PathTo(cr);
        cairo_stroke(cr);
        if (line.IsDouble()) 
            return;
        //triple line 
        cairo_set_line_width(cr,  line.TripleMiddleWidth());
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

void MscCanvas::Line(const Block &b, const MscLineAttr &line)
{
    _ASSERT(candraw);
    _ASSERT(line.IsComplete());
	if (line.type.second == LINE_NONE || !line.color.second.valid || line.color.second.a==0) return;
    if (b.IsInvalid()) return;
    SetLineAttr(line);
    const double spacing = line.Spacing();
    const double r = line.SaneRadius(b);
    MscLineAttr line2(line);
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

void MscCanvas::Line(const Contour &c, const MscLineAttr &line)
{
    _ASSERT(candraw);
    _ASSERT(line.IsComplete());
	if (line.type.second == LINE_NONE || !line.color.second.valid || line.color.second.a==0) return;
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
//void MscDrawer::LineWithJoints(const std::list<Contour> &clist, const MscLineAttr &line)
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


void _add_color_stop(cairo_pattern_t *pattern, double offset, MscColorType color)
{
    cairo_pattern_add_color_stop_rgba(pattern, offset,
                                      color.r/255., color.g/255.,
                                      color.b/255., color.a/255.);
}

void MscCanvas::linearGradient(MscColorType from, MscColorType to, const XY &s, const XY &d, MscGradientType type)
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
}

void MscCanvas::fakeLinearGrad(MscColorType from, MscColorType to, const XY &s, const XY &d, 
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

void MscCanvas::radialGradient(MscColorType from, MscColorType to, const XY &s, double outer_radius, double inner_radius, MscGradientType type)
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
void MscCanvas::fakeRadialGrad(MscColorType from, MscColorType to, const XY &s, double outer_radius, double inner_radius,
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

void MscCanvas::Fill(const Block &b, const MscFillAttr &fill)
{
    _ASSERT(candraw);
    _ASSERT(fill.IsComplete());
    if (!fill.color.second.valid || fill.color.second.a==0) return;
	const double max_extent = std::max(fabs(b.x.till-b.x.from), fabs(b.y.till-b.y.from));
	//If gradients are to be drawn, we fake them only if no alpha is present
	//If we draw somewhat transparent, we will fall back to images anyway,
	//so let us not fake gradients either
    if (fill.gradient.second!=GRADIENT_NONE && fake_gradients && fill.color.second.a==255) {
        Clip(b);
        MscColorType color = fill.color.second;
        MscColorType color2 = fill.color2.first ? fill.color2.second : fill.color.second.Lighter(0.8);
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
        MscColorType to = fill.color.second, from;
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
void MscCanvas::Shadow(const Contour &area, const MscShadowAttr &shadow, double angle_radian)
{
    _ASSERT(candraw);
    _ASSERT(shadow.IsComplete());
    if (shadow.offset.second==0 || area.IsEmpty()) return;
    //Clip out the actual shape we are the shadow of
    //if (clip) {
    //    area.Path(cr);
    //    cairo_new_sub_path(cr);
    //    cairo_rectangle(cr, area.GetBoundingBox().x.from-1, area.GetBoundingBox().y.from-1,
    //                        area.GetBoundingBox().x.Spans()+2+shadow.offset.second, area.GetBoundingBox().y.Spans()+2+shadow.offset.second);
    //    cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
    //    cairo_clip(cr);
    //}
    const Contour &substract = area;//.CreateExpand(-0.5);
    Contour outer(area), inner;
    XY off(shadow.offset.second, shadow.offset.second);
    off.Rotate(cos(-angle_radian), sin(-angle_radian));
    outer.Shift(off);
    MscColorType color = shadow.color.second;
    if (shadow.blur.second>0) {
        const unsigned steps = unsigned(std::min(shadow.blur.second, shadow.offset.second)*scale_for_shadows + 0.5);
        const double transp_step = double(color.a)/(steps+1);
        double alpha = 0;
        for (unsigned i=0; i<steps; i++) {
            inner = outer.CreateExpand(-1/scale_for_shadows);
            if (inner.IsEmpty() && outer.GetBoundingBox().x.Spans()>2 && outer.GetBoundingBox().y.Spans()>2) {
                _ASSERT(0);
            }

            outer -= inner;
            alpha += transp_step;
            color.a = (unsigned char)alpha;
            if (fake_shadows)
                SetColor(MscColorType(color).FlattenAlpha());
            else
                SetColor(color);
            //since clip is not working so well, we substract the original area
            (outer-=substract).Fill(cr);
            std::swap(inner, outer);
        }
    }
    if (shadow.blur.second <= shadow.offset.second) {
        if (fake_shadows)  //we still use a blurred shadow color
            SetColor(MscColorType(shadow.color.second).FlattenAlpha());
        else
            SetColor(shadow.color.second);
    }
    (outer-=substract).Fill(cr);
    //if (clip)
    //    cairo_restore(cr);
}

/* Set clip, if the rectangle of which this is the shadow of is not opaque */
void MscCanvas::Shadow(const Block &b, const MscLineAttr &line, const MscShadowAttr &shadow, double angle_radian)
{
    _ASSERT(candraw);
    _ASSERT(shadow.IsComplete() && line.radius.first);
    if (shadow.offset.second==0) return;
	if (!shadow.color.second.valid || shadow.color.second.a==0) return;
    //For now just call the other Shadow Routine
    Contour c = line.CreateRectangle_OuterEdge(b);
    c.Expand(-line.width.second);
    Shadow(std::move(c), shadow, angle_radian);
    return;
}

/* END OF FILE */
