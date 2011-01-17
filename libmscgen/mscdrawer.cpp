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
 * These include opening and closing various files or destination contexts depending on target,
 * faking some effects for some targets (such as gradient or dash lines for WMF) and
 * drawing/filling complete contours plus shadowing them.
 *
 * Comments on drawing.
 * Cairo draws lines on integer coordinates halfway to both adoining pixels.
 * This should be avoided, especially for thin lines.
 * However, we do not do it here - call each Fill/Line etc function with the appropriate coordinates.
 */

MscDrawer::MscDrawer() :
    total(0,0), copyrightTextHeight(0),
    outFile(NULL), surface(NULL), cr(NULL)
#ifdef CAIRO_HAS_WIN32_SURFACE
	, win32_dc(NULL), save_hdc(NULL)
#endif
{
    current_file = Error.AddFile("[config]");
    SetLowLevelParams();
}

cairo_status_t write_func(void * closure, const unsigned char *data, unsigned length)
{
    if (closure==NULL) return CAIRO_STATUS_SUCCESS;
    if (length == fwrite(data, 1, length, (FILE*)closure))
        return CAIRO_STATUS_SUCCESS;
    else
        return CAIRO_STATUS_WRITE_ERROR;
}

void MscDrawer::SetLowLevelParams(OutputType ot)
{
    /* Set low-level parameters for default */
    white_background = false;
    use_text_path = false;
    use_text_path_rotated = false;
    individual_chars = false;
    fake_gradients = 0;
    fake_dash = false; 
    fake_shadows = false;
    fake_spaces = false;
    fake_scale = 1.0;
    fallback_resolution = 300; //irrelevant for bitmaps - no fallback for bitmaps
    needs_dots_in_corner = false;

    switch (ot) {
    case PNG:
        white_background = true;
        break;
#ifdef CAIRO_HAS_WIN32_SURFACE
    case WMF:
        individual_chars = true; //do this so that it is easier to convert to WMF
        use_text_path_rotated = true;
        fake_dash = true;
        fake_scale = 10;              //do 10 for better precision clipping
        //Fallthrough
    case EMF:
        fake_gradients = 30;
        fake_shadows = true;
        needs_dots_in_corner = true;
        //check if we run on vista or later: then cairo can do text on EMF/WMF 
        OSVERSIONINFOEX osvi;
        ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
        //majorversion of 5 is Win2000, XP and 2003, 6 is Vista, 2008 and Win7
        if(!GetVersionEx ((OSVERSIONINFO *) &osvi) || osvi.dwMajorVersion<=5) 
        {
            use_text_path = use_text_path_rotated = true;
            fake_scale=100;
        }
#endif
    }
}

//page numbering starts from 0, -1 means all of the chart (ignoring page breaks)
//This one does not take bottom banner into account
void MscDrawer::GetPagePosition(int page, XY &offset, XY &size) const
{
    offset.x = 0;
    size.x = total.x;
    if (page<=-1) {
        size.y = total.y;
        offset.y = 0;
    } else if (page>=yPageStart.size()) { //too large page
        size.y = 0; //nothing drawn
        offset.y = total.y;
    } else if (page==yPageStart.size()-1) { //last page
        size.y = total.y - yPageStart[page];
        offset.y = yPageStart[page];
    } else {
        size.y = yPageStart[page+1] - yPageStart[page];
        offset.y = yPageStart[page];
    }
}

//page numbering starts from 0
//Use this to save to a file or to determine dimensions
bool MscDrawer::SetOutput(OutputType ot, double x_scale, double y_scale, const string &fn, int page)
{
    assert(ot!=WIN);
    if (surface) CloseOutput();

    SetLowLevelParams(ot);
    scale_for_shadows = sqrt(x_scale*y_scale);

    XY size, origSize, origOffset;
    GetPagePosition(page, origOffset, origSize);
	size = origSize;
    size.y += copyrightTextHeight;
    size.x *= fake_scale*x_scale;
    size.y *= fake_scale*y_scale;

    outType = ot;
    if (fn.length()==0) {
        outFile = NULL;
        fileName.clear();
    } else {
        fileName = fn;
        if (page>=0) {
            char num[3]="00";
            num[0] = '0'+page/10;
            num[1] = '0'+page%10;
            fileName.insert(fn.find_last_of('.'), num);
        }
        if (ot==EMF || ot==WMF)
            //For metafiles, there is no need to open a FILE*
            outFile = NULL;
        else {
            outFile = fopen(fileName.c_str(), "wb");
            if (!outFile) {
                Error.Error(file_line(current_file,0), string("Failed to open output file '").append(fileName).append("'"));
                //We still do the drawing, but no file will be produced
            }
        }
    }

    switch (ot) {
    default:
        return false;
#ifdef CAIRO_HAS_PNG_FUNCTIONS
    case PNG:
        surface = cairo_image_surface_create (CAIRO_FORMAT_RGB24, size.x, size.y);
        break;
#endif
#ifdef CAIRO_HAS_PS_SURFACE
    case EPS:
        surface = cairo_ps_surface_create_for_stream(write_func, outFile, size.x, size.y);
        cairo_ps_surface_set_eps(surface, true);
        break;
#endif
#ifdef CAIRO_HAS_PDF_SURFACE
    case PDF:
        surface = cairo_pdf_surface_create_for_stream(write_func, outFile, size.x, size.y);
        break;
#endif
#ifdef CAIRO_HAS_SVG_SURFACE
    case SVG:
        surface = cairo_svg_surface_create_for_stream(write_func, outFile, size.x, size.y);
        break;
#endif
#ifdef CAIRO_HAS_WIN32_SURFACE
    case EMF:
	case WMF:
        // Create the Enhanced Metafile
        if (fileName.length() > 0)
            win32_dc = CreateEnhMetaFile(NULL, fileName.c_str(), NULL, "Msc-generator\0\0");
        else
            win32_dc = CreateEnhMetaFile(NULL, NULL, NULL, NULL);
        // Did you get a good metafile?
        if( win32_dc == NULL ) {
            fileName.clear();
            return false;
        }
        /* We need a tweak in cairo.
		 ** I modified cairo_win32_printing_surface_create diretcly to use 
		 ** an extent bigger than the one reported by GetClipBox(), if the 
		 ** DC passed is an EMF DC.
         ** This is to avoid dropping glyphs outside the DC's extent.
         ** I could not make a metafile DC to have larger extent that 1600x1200
         */
        surface = cairo_win32_printing_surface_create(win32_dc);
        break;
#endif
    }
	//OK, check surface creation
    cairo_status_t st;
    st = cairo_surface_status(surface);
    if (st != CAIRO_STATUS_SUCCESS) {
        CloseOutput();
        return false;
    }
    cairo_surface_set_fallback_resolution(surface, fallback_resolution/fake_scale, fallback_resolution/fake_scale);

    cr = cairo_create (surface);
    st = cairo_status(cr);
    if (st != CAIRO_STATUS_SUCCESS) {
        CloseOutput();
        return false;
    }

    cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);
    cairo_scale(cr, fake_scale*x_scale, fake_scale*y_scale);
    if (total.x>0 && total.y>0) {
        //clip first to allow for banner text, but not for WIN contexts
        if (ot!=WIN) 
            Clip(XY(0,0), origSize + XY(0, copyrightTextHeight));
        //then clip again to exclude banner text, but not for WIN contexts
        if (ot!=WIN) 
            Clip(XY(0,0), origSize);
    }
    cairo_translate(cr, -origOffset.x, -origOffset.y);
    return true;
}

#ifdef CAIRO_HAS_WIN32_SURFACE

//Draw to a DC that is either a display DC or a metafile
//Use this to display a chart, use SetOutput to save the chart to a file
bool MscDrawer::SetOutputWin32(OutputType ot, HDC hdc, double x_scale, double y_scale, int page)
{
    if (surface) CloseOutput();
    if (ot!=WIN && ot!=WMF && ot!=EMF) return false;

    outType = ot;
    outFile = NULL;
    win32_dc = NULL;
    save_hdc = hdc;
    fileName.clear();

    SetLowLevelParams(ot);
    scale_for_shadows = sqrt(x_scale*y_scale);

    XY size, origSize, origOffset;
    GetPagePosition(page, origOffset, origSize);
	size = origSize;
    size.y += copyrightTextHeight;
    size.x *= fake_scale*x_scale;
    size.y *= fake_scale*y_scale;

    switch (ot) {
    case WIN:
        surface = cairo_win32_surface_create(hdc);
        break;
    case EMF:
        surface = cairo_win32_printing_surface_create(hdc);
        break;
    case WMF:
        win32_dc = CreateEnhMetaFile(NULL, NULL, NULL, NULL);
        if( win32_dc == NULL ) return false;
        save_hdc = hdc;
        surface = cairo_win32_printing_surface_create(win32_dc);
        break;
    default:
        _ASSERT(0);
    }
    cairo_status_t st = cairo_surface_status(surface);
    if (st) goto problem;

    cairo_surface_set_fallback_resolution(surface, fallback_resolution/fake_scale, fallback_resolution/fake_scale);

	cr = cairo_create (surface);
    st = cairo_status(cr);
    if (st) goto problem;

    cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);
    cairo_scale(cr, fake_scale*x_scale, fake_scale*y_scale);
    if (total.x>0 && total.y>0) {
        //clip first to allow for banner text
        if (ot!=WIN) 
            Clip(XY(0,0), origSize + XY(0, copyrightTextHeight));
        if (needs_dots_in_corner) {
            //dots in the corner for EMF
            MscLineAttr l(MscColorType(255,255,255)); l.MakeComplete();
            Line(XY(0,origSize.y+copyrightTextHeight), origSize+ XY(0, copyrightTextHeight), l);
        }
        //then clip again to exclude banner text
        if (ot!=WIN) 
            Clip(XY(0,0), origSize);
    }
    cairo_translate(cr, -origOffset.x, -origOffset.y);
    return true;

    problem:
        const char *error = cairo_status_to_string(st);
        CloseOutput();
        return false;
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
                      LPARAM lpClientData     // optional data
                     )
{
    bool b=true;
    char buff[4096];
    if (lpMFR->rdFunction == META_EXTTEXTOUT) {
        EXTTO* pTO = ( EXTTO* )(&lpMFR->rdParm[0]);
        unsigned len = pTO->toSTRLEN;
        for (int i=0; i<len; i++)
            buff[i] = pTO->string[i] + 29;
        buff[len]=0;
        ExtTextOut(hDC, pTO->toX, pTO->toY, 0, NULL, buff, len, NULL);
    } else if (lpMFR->rdFunction == META_CREATEFONTINDIRECT) {
        LOGFONT16* plogfont = ( LOGFONT16* )(&lpMFR->rdParm[0]);
        plogfont->lfHeight = plogfont->lfHeight / 32;
        b=PlayMetaFileRecord(hDC, lpHTable, lpMFR, nObj);
    } else {
        b=PlayMetaFileRecord(hDC, lpHTable, lpMFR, nObj);
    }
    if (!b) FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,NULL,GetLastError(),0, buff,4096,NULL );
    return 1;
}

void PaintEMFonWMFdc(HENHMETAFILE hemf, HDC hdc, const RECT &r, bool applyTricks)
{
    if (hemf == NULL || hdc == NULL) return;
    HDC refDC = GetDC(NULL);
    SetMapMode(refDC, MM_ANISOTROPIC);
    SetWindowExtEx(refDC, r.right, r.bottom, NULL);
    SetViewportExtEx(refDC, r.right, r.bottom, NULL);
    unsigned size = GetWinMetaFileBits(hemf, 0, NULL, MM_ANISOTROPIC, refDC);
    BYTE *buff = (BYTE*)malloc(size);
	if (buff==NULL) {
		ReleaseDC(NULL, refDC);
		return;
	}
    size = GetWinMetaFileBits(hemf, size, buff, MM_ANISOTROPIC, refDC);
    ReleaseDC(NULL, refDC);
    HMETAFILE hwmf = SetMetaFileBitsEx(size, buff);
    free(buff);
    if (applyTricks)
        EnumMetaFile(hdc, hwmf, EnumProc, NULL);
    else
        PlayMetaFile(hdc, hwmf);
    DeleteMetaFile(hwmf);
}

HENHMETAFILE MscDrawer::CloseOutputRetainHandleEMF()
{
    if (surface==NULL) return NULL;
    cairo_status_t st = cairo_surface_status(surface);
    cairo_surface_show_page(surface);
    cairo_surface_destroy(surface);
    outFile=NULL;
    surface=NULL;
    cr=NULL;
    fileName.clear();
    switch (outType) {
    default:
        return NULL;
    case EMF:
    case WMF:
        return CloseEnhMetaFile(win32_dc);
    }
}
HMETAFILE MscDrawer::CloseOutputRetainHandleWMF()
{
    if (surface==NULL) return NULL;
    cairo_status_t st = cairo_surface_status(surface);
    cairo_surface_show_page(surface);
    cairo_surface_destroy(surface);
    outFile=NULL;
    surface=NULL;
    cr=NULL;
    fileName.clear();
    switch (outType) {
    default:
        return NULL;
    case EMF:
    case WMF:
        HENHMETAFILE hemf = CloseEnhMetaFile(win32_dc);
        RECT r;
        SetRect(&r, 0, 0, total.x, total.y);
        HDC hdc = CreateMetaFile(NULL);
        PaintEMFonWMFdc(hemf, hdc, r, true);
        DeleteEnhMetaFile(hemf);
        return CloseMetaFile(hdc);
    }
}
#endif //WIN32

void MscDrawer::PrepareForCopyrightText()
{
    if (outType != WIN)
        UnClip();
}


void MscDrawer::CloseOutput()
{
    /* Destroy our context */
    if (cr)
        cairo_destroy (cr);

    if (surface) {
        /* Output the image to the disk file in PNG format. */
        switch (outType) {
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
#ifdef CAIRO_HAS_WIN32_SURFACE
            cairo_surface_show_page(surface);
            cairo_surface_destroy (surface);
            if (outType==EMF) {
                if (win32_dc)
                    DeleteEnhMetaFile(CloseEnhMetaFile(win32_dc)); //save metafile if opened by SetOutput
            } else { //WMF
                if (save_hdc) { //Opened via SetOutputWin32()
                    HENHMETAFILE hemf = CloseEnhMetaFile(win32_dc);
                    RECT r;
                    SetRect(&r, 0, 0, total.x, total.y);
                    PaintEMFonWMFdc(hemf, save_hdc, r, true);
                    DeleteEnhMetaFile(hemf);
                    save_hdc = NULL;
                } else { //Opened via SetOutput, win32_dc is an EMF DC
                    DeleteEnhMetaFile(CloseEnhMetaFile(win32_dc));
                }
            }
            win32_dc = NULL;
            break;
            //Fallthrough if no WIN32 surface
#endif
        case WIN:
            cairo_surface_destroy (surface);
        }
    }
    outFile=NULL;
    surface=NULL;
    cr=NULL;
    fileName.clear();
}

cairo_line_join_t MscDrawer::SetLineJoin(cairo_line_join_t t)
{
    cairo_line_join_t tt = cairo_get_line_join(cr);
    cairo_set_line_join(cr, t);
    return tt;
}
cairo_line_cap_t MscDrawer::SetLineCap(cairo_line_cap_t t)
{
    cairo_line_cap_t tt = cairo_get_line_cap(cr);
    cairo_set_line_cap(cr, t);
    return tt;
}


void MscDrawer::Clip(const EllipseData &ellipse)
{
    cairo_save(cr);
    cairo_save(cr);
    ellipse.TransformForDrawing(cr);
    cairo_arc(cr, 0, 0, 1, 0, 2*M_PI);
    cairo_restore(cr);
    cairo_clip(cr);
}

void MscDrawer::Clip(const Block &b, const MscLineAttr &line)
{
    _ASSERT(line.IsComplete());
    cairo_save(cr);
    if (line.corner.second == CORNER_NOTE && line.IsDoubleOrTriple()) {
        //spacing from the line midpoint to the midpoint of the outer line of a double/triple line
        const double spacing = line.IsDouble() ? line.DoubleSpacing() : line.IsTriple() ? line.TripleSpacing() : 0;
        const double r = line.radius.second + (line.IsDouble() ? - line.width.second/2 : spacing * RadiusIncMultiplier(CORNER_NOTE)); //expand radius
        //Add main body as a closed path
        (Contour(b) - Block(b.x.till-r, b.x.till, b.y.from, b.y.from+r)).Path(cr);
        const double r1 = line.radius.second - (line.IsDouble() ? spacing *2 + line.width.second/2: spacing *(1-RadiusIncMultiplier(CORNER_NOTE))); 
        const double r2 = spacing * (line.IsDouble() ? 1./RadiusIncMultiplier(CORNER_NOTE) - 1. : - 1);
        //Add little triangle as a path
        Contour(XY(b.x.till-r1, b.y.from+r2),XY(b.x.till-r1, b.y.from+r1), XY(b.x.till-r2, b.y.from+r1)).Path(cr);
    } else {
        RectanglePath(b.x.from, b.x.till, b.y.from, b.y.till, line);
    }
    cairo_clip(cr);
}

//rotates such that a 
void MscDrawer::Transform_Rotate90(double s, double d, bool clockwise)
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

void MscDrawer::Transform_SwapXY()
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

void MscDrawer::Transform_FlipHorizontal(double y)
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

void MscDrawer::SetColor(MscColorType pen)
{
	if (pen.valid)
		cairo_set_source_rgba(cr, pen.r/255.0, pen.g/255.0, pen.b/255.0, pen.a/255.0);
}

void MscDrawer::SetLineAttr(MscLineAttr line)
{
	if (line.color.first && line.color.second.valid)
        SetColor(line.color.second);
    if (line.width.first)
        cairo_set_line_width(cr, line.width.second);
    if (line.type.first)
        SetDash(line, 0);
}

void MscDrawer::SetDash(MscLineAttr line, double pattern_offset)
{
    if (fake_dash) {
        cairo_set_dash(cr, NULL, 0, 0); 
        fake_dash_offset = pattern_offset;
    } else if (line.type.first) {
        int num;
        const double * const pattern = line.DashPattern(num);
        cairo_set_dash(cr, pattern, num, pattern_offset); 
    }
}

void MscDrawer::SetFontFace(const char*face, bool italics, bool bold)
{
    cairo_select_font_face (cr, face,
                            italics?CAIRO_FONT_SLANT_ITALIC:CAIRO_FONT_SLANT_NORMAL,
                            bold?CAIRO_FONT_WEIGHT_BOLD:CAIRO_FONT_WEIGHT_NORMAL);
}

void MscDrawer::Text(XY xy, const string &s, bool isRotated)
{
    cairo_move_to (cr, xy.x, xy.y);
    if (use_text_path || (isRotated && use_text_path_rotated)) {
        cairo_text_path(cr, s.c_str());
        cairo_fill(cr);
        return;
    }
    if (individual_chars) {
        char tmp_stirng[2] = "a";
        for (int i=0; i<s.length(); i++) {
            tmp_stirng[0] = s[i];
            cairo_show_text(cr, tmp_stirng);
        }
    } else {
        cairo_show_text (cr, s.c_str());
    }
}

void MscDrawer::ArcPath(const EllipseData &ell, double s_rad, double e_rad, bool reverse)
{
    cairo_save (cr);
    ell.TransformForDrawing(cr);
    if (reverse)
        cairo_arc_negative(cr, 0., 0., 1., s_rad, e_rad);
    else
        cairo_arc(cr, 0., 0., 1., s_rad, e_rad);
    cairo_restore (cr);
}

void MscDrawer::ArcPath(const XY &c, double r1, double r2, double s_rad, double e_rad, bool reverse)
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

void MscDrawer::RectanglePath(double sx, double dx, double sy, double dy)
{
    cairo_new_path(cr);
    cairo_move_to(cr, sx, sy);
    cairo_line_to(cr, dx, sy);
    cairo_line_to(cr, dx, dy);
    cairo_line_to(cr, sx, dy);
    cairo_close_path(cr);
}

//for CORNER_NOTE it draws the outer edge
void MscDrawer::RectanglePath(double sx, double dx, double sy, double dy, const MscLineAttr &line)
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
        cairo_new_path(cr);
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
        cairo_new_path(cr);
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


//Draw dashed/dotted line
//assume color and linewidth are set correctly. We also assume cairo dash is set to continuous: we fake dash here
//The pattern contains lengths of alternating on/off segments, num contains the number of elements in pattern
//pos shows which pattern element we shall start at, offset shows at which point. We return updated versions of these
void MscDrawer::fakeDashedLine(const XY &s, const XY &d, const double pattern[], int num, int &pos, double &offset)
{
    _ASSERT(num);
    _ASSERT(offset<pattern[pos]);
    const double len = sqrt((d.x-s.x)*(d.x-s.x) + (d.y-s.y)*(d.y-s.y));
    const double ddx = (d.x-s.x)/len;
    const double ddy = (d.y-s.y)/len;
    double processed = 0;
    double x = s.x, y = s.y;  
    cairo_new_path(cr);
    if (offset) {
        if (pattern[pos]-offset > len) { //remaining segment is shorter than the entire length
            if (pos%2==0) {//start with drawn
                cairo_move_to(cr, s.x, s.y);
                cairo_line_to(cr, d.x, d.y);
                cairo_stroke(cr);
            }
            offset += len;
            return;
        }
        if (pos%2==0) cairo_move_to(cr, x, y);
        x += (pattern[pos]-offset)*ddx;
        y += (pattern[pos]-offset)*ddy;
        if (pos%2==0) cairo_line_to(cr, x, y);
        processed += pattern[pos]-offset;
        pos = (pos+1)%num;
    }

    while(processed+pattern[pos] <= len) {
        if (pos%2==0) cairo_move_to(cr, x, y);
        x += pattern[pos]*ddx;
        y += pattern[pos]*ddy;
        if (pos%2==0) cairo_line_to(cr, x, y);
        processed += pattern[pos];
        pos = (pos+1)%num;
    }

    offset = len - processed;
    if (pos%2==0 && !test_zero(offset)) {
        cairo_move_to(cr, x, y);
        cairo_line_to(cr, d.x, d.y); 
    }
    cairo_stroke(cr);
}

//Draw dashed/dotted line
//assume color and linewidth are set correctly. We also assume cairo dash is set to continuous: we fake dash here
//The pattern contains lengths of alternating on/off segments, num contains the number of elements in pattern
//pos shows which pattern element we shall start at, offset shows at which point. We return updated versions of these
void MscDrawer::fakeDashedLine(const XY &c, double r1, double r2, double tilt, double s, double e, 
                               const double pattern[], int num, int &pos, double &offset, bool reverse)
{
    _ASSERT(num);
    _ASSERT(offset<pattern[pos]);
    if (r2<=0 || r1<=0) return;

    cairo_new_path(cr);
    cairo_save(cr);
    cairo_translate(cr, c.x, c.y);
    if (tilt)
        cairo_rotate(cr, tilt);
    cairo_scale(cr, r1, r2);

    if (reverse) {
        if (s<e) s += 2*M_PI; //ensure s is larger than e (assume e and s between [0, 2PI])
    } else {
        if (e<s) e += 2*M_PI; //ensure e is larger than s
    }
    //TODO Do proper ellipse arc length calculations
    const double avg_r = (r1+r2)/2;
    const double len = fabs(s-e)*avg_r; 
    const double inc_s = s<e ? 1/avg_r : -1/avg_r;
    double processed = 0;
    if (offset) {
        if (pattern[pos]-offset > len) { //remaining segment is shorter than the entire length
            offset += len;
            if (pos%2==0) {//start with drawn
                cairo_new_sub_path(cr);
                if (reverse) cairo_arc_negative(cr, 0, 0, 1., s, e);
                else cairo_arc(cr, 0, 0, 1., s, e);
                cairo_restore(cr);
                cairo_stroke(cr);
            } else
                cairo_restore(cr);
            return;
        }
        const double new_s = s + inc_s*(pattern[pos]-offset);
        if (pos%2==0) {
            cairo_new_sub_path(cr);
            if (reverse) cairo_arc_negative(cr, 0, 0, 1., s, new_s);
            else cairo_arc(cr, 0, 0, 1., s, new_s);
        }
        s = new_s;
        processed += pattern[pos]-offset;
        pos = (pos+1)%num;
    }

    while (processed+pattern[pos] <= len) {
        const double new_s = s + inc_s*pattern[pos];
        if (pos%2==0) {
            cairo_new_sub_path(cr);
            if (reverse) cairo_arc_negative(cr, 0, 0, 1., s, new_s);
            else cairo_arc(cr, 0, 0, 1., s, new_s);
        }
        s = new_s;
        processed += pattern[pos];
        pos = (pos+1)%num;
    }

    offset = len - processed;
    if (pos%2==0 && !test_zero(offset)) {
        cairo_new_sub_path(cr);
        if (reverse) cairo_arc_negative(cr, 0, 0, 1., s, e);
        else cairo_arc(cr, 0, 0, 1., s, e);
    }
    cairo_restore(cr);
    cairo_stroke(cr);
}

//Draw a line, but ignore if a double line is proscribed by line.
//However, if the surface requires faking dashes, do them. 
//Assume Color and Width (and Dash if we do not fake it) are already set in cairo context
//line and arc version honour fake_dash_offset
void MscDrawer::singleLine(const XY &s, const XY &d, const MscLineAttr &line)
{
    if (line.IsContinuous() || !fake_dash) {
        cairo_new_path(cr);
        cairo_move_to(cr, s.x, s.y); 
        cairo_line_to(cr, d.x, d.y);
        cairo_stroke(cr);
    } else {
        int num, pos=0;
        const double *pattern;
        double offset;
        pattern = line.DashPattern(num);
        offset = fmod(fake_dash_offset, pattern[num]);
        while (offset>=pattern[pos]) offset -= pattern[pos++];
        fakeDashedLine(s, d, pattern, num, pos, offset);
        fake_dash_offset = offset;
        while(num>0) fake_dash_offset += pattern[--num];
    }
}

//line and arc version honour fake_dash_offset
void MscDrawer::singleLine(const XY &c, double r1, double r2, double tilt, 
                           double s, double e, const MscLineAttr &line, bool reverse)
{
    if (line.IsContinuous() || !fake_dash) {
        cairo_new_path(cr);
        cairo_save (cr);
        cairo_translate (cr, c.x, c.y);
        if (tilt)
            cairo_rotate(cr, tilt);
        cairo_scale (cr, r1, r2);
        if (reverse)
            cairo_arc_negative(cr, 0., 0., 1., s, e);
        else
            cairo_arc(cr, 0., 0., 1., s, e);
        cairo_restore (cr);
        cairo_stroke(cr);
    } else {
        int num, pos = 0;
        const double *pattern;
        double offset;
        pattern = line.DashPattern(num);
        offset = fmod(fake_dash_offset, pattern[num]);
        while (offset>=pattern[pos]) offset -= pattern[pos++];
        fakeDashedLine(c, r1, r2, tilt, s, e, pattern, num, pos, offset, reverse);
        fake_dash_offset = offset;
        while(num>0) fake_dash_offset += pattern[--num];
    }
}

void MscDrawer::singleLine(const Block &b, const MscLineAttr &line)
{
    if (line.IsContinuous() || !fake_dash) {
        RectanglePath(b.x.from, b.x.till, b.y.from, b.y.till, line);
        cairo_stroke(cr);
    } else {
        int num, pos = 0;
        const double *pattern;
        double offset = 0;
        pattern = line.DashPattern(num);
        //r1 is used for the upper left corner, r2 for the other three
        const double r1 = (line.corner.second == CORNER_ROUND ||
                           line.corner.second == CORNER_BEVEL ||
                           line.corner.second == CORNER_NOTE) ?
                           std::min(std::min(fabs(b.x.Spans())/2, fabs(b.y.Spans())/2), line.radius.second) : 0;
        const double r2 = line.corner.second == CORNER_NOTE ? 0 : r1;
        //upper left corner
        if (r2>0) switch (line.corner.second) {
        case CORNER_ROUND: fakeDashedLine(XY(b.x.from+r2, b.y.from+r2), r2, r2, 0, 2.*M_PI/2., 3.*M_PI/2., 
                               pattern, num, pos, offset, false); break;
        case CORNER_BEVEL: fakeDashedLine(XY(b.x.from, b.y.from-r2), XY(b.x.from+r2, b.y.from), 
                               pattern, num, pos, offset); break;
        }
        fakeDashedLine(XY(b.x.from+r2, b.y.from), XY(b.x.till-r1, b.y.from), pattern, num, pos, offset);
        //upper right corner
        if (r1>0) switch (line.corner.second) {
        case CORNER_ROUND: fakeDashedLine(XY(b.x.till-r1, b.y.from+r1), r1, r1, 0, 3.*M_PI/2., 4.*M_PI/2., 
                               pattern, num, pos, offset, false); break;
        case CORNER_NOTE: 
        case CORNER_BEVEL: fakeDashedLine(XY(b.x.till-r1, b.y.from), XY(b.x.till, b.y.from+r1), 
                               pattern, num, pos, offset); break;
        }
        fakeDashedLine(XY(b.x.till, b.y.from+r1), XY(b.x.till, b.y.till-r2), pattern, num, pos, offset);
        //lower right corner
        if (r2>0) switch (line.corner.second) {
        case CORNER_ROUND: fakeDashedLine(XY(b.x.till-r2, b.y.till-r2), r2, r2, 0, 0.*M_PI/2., 1.*M_PI/2., 
                               pattern, num, pos, offset, false); break;
        case CORNER_BEVEL: fakeDashedLine(XY(b.x.till, b.y.till-r2), XY(b.x.till-r2, b.y.till), 
                               pattern, num, pos, offset); break;
        }
        fakeDashedLine(XY(b.x.till-r2, b.y.till), XY(b.x.from+r2, b.y.till), pattern, num, pos, offset);
        //lower left corner
        if (r2>0) switch (line.corner.second) {
        case CORNER_ROUND: fakeDashedLine(XY(b.x.from+r2, b.y.till-r2), r2, r2, 0, 1.*M_PI/2., 2.*M_PI/2., 
                               pattern, num, pos, offset, false);
        case CORNER_BEVEL: fakeDashedLine(XY(b.x.from-r2, b.y.till), XY(b.x.from, b.y.till-r2), 
                               pattern, num, pos, offset); break;
        }
        fakeDashedLine(XY(b.x.from, b.y.till-r2), XY(b.x.from, b.y.from+r2), pattern, num, pos, offset);
    }
}

void MscDrawer::singleLine(const Contour &c, const MscLineAttr &line, bool open)
{
    if (line.IsContinuous() || !fake_dash) {
        if (open)
            c.PathOpen(cr);
        else
            c.Path(cr);
        cairo_stroke(cr);
    } else {
        int num, pos = 0;
        const double *pattern;
        double offset;
        pattern = line.DashPattern(num);
        for (int i=0; i<c.size()-(open?1:0); i++) 
            if (c.GetEdge(i).IsStraight())
                singleLine(c.GetEdge(i).GetStart(), c.GetEdge((i+1)%c.size()).GetStart(), line);
            else {
                const Edge &edge = c.GetEdge(i);
                singleLine(edge.GetEllipseData().GetCenter(), edge.GetEllipseData().GetRadius1(), edge.GetEllipseData().GetRadius2(), 
                    edge.GetEllipseData().GetTilt(), edge.GetRadianS(), edge.GetRadianE(), line, !edge.GetClockWise());
            }
    }
}

void MscDrawer::singleLine(const ContourList&cl, const MscLineAttr &line)
{
    for (auto i=cl.begin(); i!=cl.end(); i++) {
        singleLine(static_cast<const Contour&>(*i), line, false);
        if (i->HasHoles()) singleLine(i->GetHoles(), line);
    }
}

void MscDrawer::singleLine(const Area&cl, const MscLineAttr &line)
{
    for (auto i=cl.begin(); i!=cl.end(); i++) {
        singleLine(static_cast<const Contour&>(*i), line, false);
        if (i->HasHoles()) singleLine(i->GetHoles(), line);
    }
}

void MscDrawer::Line(const Edge& edge, const MscLineAttr &line)
{
    _ASSERT(line.IsComplete());
    if (edge.IsStraight()) return;
	if (line.type.second == LINE_NONE || !line.color.second.valid || line.color.second.a==0) return;
    SetLineAttr(line);
    const double spacing = line.IsDouble() ? line.DoubleSpacing() : line.IsTriple() ? line.TripleSpacing() : 0;
    const EllipseData &ell = edge.GetEllipseData();
    if (line.IsDoubleOrTriple()) {
        singleLine(ell.GetCenter(), ell.GetRadius1()-spacing, ell.GetRadius2()-spacing, 
            ell.GetTilt(), edge.GetRadianS(), edge.GetRadianE(), line, !edge.GetClockWise());
        singleLine(ell.GetCenter(), ell.GetRadius1()+spacing, ell.GetRadius2()+spacing, 
            ell.GetTilt(), edge.GetRadianS(), edge.GetRadianE(), line, !edge.GetClockWise());
    } 
    if (line.IsTriple()) cairo_set_line_width(cr,  line.TripleMiddleWidth());
    if (!line.IsDouble()) 
        singleLine(ell.GetCenter(), ell.GetRadius1(), ell.GetRadius2(), 
            ell.GetTilt(), edge.GetRadianS(), edge.GetRadianE(), line, !edge.GetClockWise());
}

void MscDrawer::Line(const XY &s, const XY &d, const MscLineAttr &line, double pattern_offset)
{
    _ASSERT(line.IsComplete());
	if (line.type.second == LINE_NONE || !line.color.second.valid || line.color.second.a==0) return;
    SetLineAttr(line);
    const double spacing = line.IsDouble() ? line.DoubleSpacing() : line.IsTriple() ? line.TripleSpacing() : 0;
    if (line.IsDoubleOrTriple()) {
        const double len = sqrt((d.x-s.x)*(d.x-s.x) + (d.y-s.y)*(d.y-s.y));
        const double DX = (s.y-d.y)/len*spacing;
        const double DY = (s.x-d.x)/len*spacing;
        SetDash(line, pattern_offset);
        singleLine(s+XY(DX, -DY), d+XY(DX, -DY), line);
        SetDash(line, pattern_offset);
        singleLine(s-XY(DX, -DY), d-XY(DX, -DY), line);
    } 
    if (line.IsTriple()) cairo_set_line_width(cr,  line.TripleMiddleWidth());
    if (!line.IsDouble())  {
        SetDash(line, pattern_offset);
        singleLine(s, d, line);
    }
}

void MscDrawer::Line(const Block &b, const MscLineAttr &line)
{
    _ASSERT(line.IsComplete());
	if (line.type.second == LINE_NONE || !line.color.second.valid || line.color.second.a==0) return;
    if (b.IsInvalid()) return;
    SetLineAttr(line);
    const double spacing = line.IsDouble() ? line.DoubleSpacing() : line.IsTriple() ? line.TripleSpacing() : 0;
    if (line.corner.second!=CORNER_NOTE || line.radius.second==0) {
        if (line.IsDoubleOrTriple()) {
            Block bb(b);
            MscLineAttr line2(line);
            bb.Expand(spacing);
            if (line2.radius.second>0) line2.radius.second += spacing * line2.RadiusIncMul();
            singleLine(bb, line2);
            bb.Expand(-2*spacing);
            line2.radius.second -= 2*spacing * line2.RadiusIncMul(); //negative radius is handled as 0 by singleLine
            singleLine(bb, line2);
        } 
        if (line.IsTriple()) cairo_set_line_width(cr,  line.TripleMiddleWidth());
        if (!line.IsDouble()) 
            singleLine(b, line);
    } else {
        //Draw note
        if (line.IsDoubleOrTriple()) {
            Block bb(b);
            MscLineAttr line2(line);
            bb.Expand(spacing);
            line2.radius.second += spacing * (line.IsDouble() ? -RadiusIncMultiplier(CORNER_NOTE) : RadiusIncMultiplier(CORNER_NOTE));
            singleLine(bb, line2);
            bb.Expand(-2*spacing);
            const double r = line.radius.second - spacing * (line.IsDouble() ? 1 : 0); 
            Area inner = Contour(bb) - Contour(bb.x.till-r, bb.x.till, bb.y.from, bb.y.from+r);
            singleLine(inner, line);
        } else {
            const double r = line.radius.second;
            singleLine(b, line);
            LineOpen(Contour(XY(b.x.till, b.y.from+r), XY(b.x.till-r, b.y.from+r), XY(b.x.till-r, b.y.from)), line);
        }
        if (line.IsDouble()) {
            const double r1 = line.radius.second - 2*spacing;
            const double r2 = spacing / RadiusIncMultiplier(CORNER_NOTE);
            singleLine(Contour(XY(b.x.till-r1, b.y.from+r2),XY(b.x.till-r1, b.y.from+r1), XY(b.x.till-r2, b.y.from+r1)), line);
        } else if (line.IsTriple()) {
            MscLineAttr line2(line);
            line2.type.second = LINE_SOLID;
            if (line.type.second == LINE_TRIPLE_THICK)
                line2.width.second *= 2;
            Line(b, line2);
        }
    }
}

void MscDrawer::Line(const Contour &c, const MscLineAttr &line)
{
    _ASSERT(line.IsComplete());
	if (line.type.second == LINE_NONE || !line.color.second.valid || line.color.second.a==0) return;
    if (c.IsEmpty()) return;
    SetLineAttr(line);
    const double spacing = line.IsDouble() ? line.DoubleSpacing() : line.IsTriple() ? line.TripleSpacing() : 0;
    if (line.IsDoubleOrTriple()) {
        singleLine(c.CreateExpand(spacing), line);
        singleLine(c.CreateExpand(-spacing), line);
    } 
    if (line.IsTriple()) cairo_set_line_width(cr,  line.TripleMiddleWidth());
    if (!line.IsDouble()) 
        singleLine(c, line);
}

void MscDrawer::Line(const Area &area, const MscLineAttr &line)
{
    _ASSERT(line.IsComplete());
	if (line.type.second == LINE_NONE || !line.color.second.valid || line.color.second.a==0) return;
    if (area.IsEmpty()) return;
    SetLineAttr(line);
    const double spacing = line.IsDouble() ? line.DoubleSpacing() : line.IsTriple() ? line.TripleSpacing() : 0;
    if (line.IsDoubleOrTriple()) {
        singleLine(area.CreateExpand(spacing), line);
        singleLine(area.CreateExpand(-spacing), line);
    } 
    if (line.IsTriple()) cairo_set_line_width(cr,  line.TripleMiddleWidth());
    if (!line.IsDouble()) 
        singleLine(area, line);
}

void MscDrawer::LineOpen(const Contour &c, const MscLineAttr &line)
{
    _ASSERT(line.IsComplete());
	if (line.type.second == LINE_NONE || !line.color.second.valid || line.color.second.a==0) return;
    if (c.IsEmpty()) return;
    SetLineAttr(line);
    const double spacing = line.IsDouble() ? line.DoubleSpacing() : line.IsTriple() ? line.TripleSpacing() : 0;
    if (line.IsDoubleOrTriple()) {
        ContourList cl;
        c.Expand(spacing, cl);
        if (!cl.IsEmpty())
            singleLine(*const_cast<const ContourList*>(&cl)->begin(), line, true);
        cl.clear();
        c.Expand(-spacing, cl);
        if (!cl.IsEmpty())
            singleLine(*const_cast<const ContourList*>(&cl)->begin(), line, true);
    } 
    if (line.IsTriple()) cairo_set_line_width(cr,  line.TripleMiddleWidth());
    if (!line.IsDouble()) 
        singleLine(c, line, true);
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

void MscDrawer::linearGradient(MscColorType from, MscColorType to, const XY &s, const XY &d, MscGradientType type)
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

void MscDrawer::fakeLinearGrad(MscColorType from, MscColorType to, const XY &s, const XY &d, 
                               bool dir_is_x, unsigned steps)
{
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

void MscDrawer::radialGradient(MscColorType from, MscColorType to, const XY &s, double outer_radius, double inner_radius, MscGradientType type)
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
void MscDrawer::fakeRadialGrad(MscColorType from, MscColorType to, const XY &s, double outer_radius, double inner_radius,
                 unsigned steps, bool rectangle, double rad_from, double rad_to)
{
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

void MscDrawer::Fill(const Block &b, const MscFillAttr &fill)
{
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
        switch(fill.gradient.second) {
        case GRADIENT_OUT:
            fakeRadialGrad(color, color2, b.CenterPoint(), max_extent, 0, fake_gradients, true);
            break;
        case GRADIENT_IN:
            fakeRadialGrad(color2, color, b.CenterPoint(), max_extent, 0, fake_gradients, true);
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
                b.UpperLeft(), b.LowerRight()-XY(0, step*66), false, fake_gradients*0.34);
            fakeLinearGrad(color.Lighter(0.16), color, 
                b.UpperLeft()+XY(0, step*34), b.LowerRight(), false, fake_gradients*0.66);
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
        if (fill.gradient.second ==GRADIENT_IN || fill.gradient.second == GRADIENT_OUT) 
            radialGradient(from, to, b.CenterPoint(), max_extent, 0, fill.gradient.second);
         else 
            linearGradient(from, to, b.UpperLeft(), b.LowerRight(), fill.gradient.second);
        RectanglePath(b.x.from, b.x.till, b.y.from, b.y.till);
        cairo_fill(cr);
    }
}

void MscDrawer::Fill(const EllipseData &ellipse, const MscFillAttr &fill)
{
    Clip(ellipse);
    XY dummy;
    Block b(ellipse.GetExtreme(0, dummy), ellipse.GetExtreme(1, dummy),
            ellipse.GetExtreme(2, dummy), ellipse.GetExtreme(3, dummy));
    Fill(b, fill);
    UnClip();
}

////////////////////// Shadow routines

void MscDrawer::Shadow(const Area &area, const MscShadowAttr &shadow)
{
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
    Area substract = area.CreateExpand(-0.5);
    Area outer(area), inner;
    outer.Shift(XY(shadow.offset.second, shadow.offset.second));
    MscColorType color = shadow.color.second;
    if (shadow.blur.second>0) {
        const int steps = floor(std::min(shadow.blur.second, shadow.offset.second)*scale_for_shadows + 0.5);
        const double transp_step = double(color.a)/(steps+1);
        color.a = transp_step;
        for (int i=0; i<steps; i++) {
            inner = outer.CreateExpand(-1/scale_for_shadows);
            outer -= inner;
            if (fake_shadows)
                SetColor(color.FlattenAlpha());
            else
                SetColor(color);
            //since clip is not working so well, we substract the original area
            (outer-=substract).Fill(cr);
            swap(inner, outer);
            color.a += transp_step;
        }
    }
    if (shadow.blur.second > shadow.offset.second) { //we still use a blurred shadow color
        if (fake_shadows)
            SetColor(color.FlattenAlpha());
        else
            SetColor(color);
    } else
        SetColor(shadow.color.second);
    (outer-=substract).Fill(cr);
    //if (clip)
    //    cairo_restore(cr);
}

/* Set clip, if the rectangle of which this is the shadow of is not opaque */
void MscDrawer::Shadow(const Block &b, const MscLineAttr &line, const MscShadowAttr &shadow)
{
    _ASSERT(shadow.IsComplete() && line.radius.first);
    if (shadow.offset.second==0) return;
	if (!shadow.color.second.valid || shadow.color.second.a==0) return;
    //For now just call the other Shadow Routine
    Shadow(line.CreateRectangle(b), shadow);
    return;

    //// chopped corners needed to be added here
    //MscColorType inner = shadow.color.second;

    ////Clip out the actual rectange we are the shadow of
    ////if (clip) {
    ////    cairo_save(cr);
    ////    RectanglePath(b.x.from, b.x.till, b.y.from, b.y.till, line);
    ////    cairo_new_sub_path(cr);
    ////    cairo_rectangle(cr, b.x.from-1, b.y.from-1, b.x.Spans()+2, b.y.Spans()+2);
    ////    cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
    ////    cairo_clip(cr);
    ////}

    ////Normalize radius
    //double radius = line.radius.second < 0 ? 0 : line.radius.second;
    //const double max_radius = std::min(b.x.Spans(), b.y.Spans())/2-1;
    //if (radius > max_radius) radius = max_radius;
    //const double blur = std::min(max_radius, shadow.blur.second);
    //const double blur_radius = std::max((double)radius, blur);
    //const XY xy_off(shadow.offset.second, shadow.offset.second);
    //const XY xy_blur(blur, blur);
    //const XY ss = b.UpperLeft()+xy_off;
    //const XY dd = b.LowerRight()+xy_off;

    ////Draw the blurred edges first
    //if (shadow.blur.second > 0) {
    //    const XY mm = b.LowerRight()-b.UpperLeft()-XY(2*blur_radius, 2*blur_radius);
    //    MscColorType outer = shadow.color.second;   outer.a = 0; /* Fully transparent */
    //    if (fake_shadows) {
    //        /* Should avoid both alpha and gradients */
    //        inner = inner.FlattenAlpha();
    //        outer = outer.FlattenAlpha();
    //        fakeLinearGrad(outer, inner, XY(ss.x+blur_radius, ss.y),      XY(dd.x-blur_radius, ss.y+blur), false, fake_gradients);
    //        fakeLinearGrad(inner, outer, XY(ss.x+blur_radius, dd.y-blur), XY(dd.x-blur_radius, dd.y),      false, fake_gradients);
    //        fakeLinearGrad(outer, inner, XY(ss.x, ss.y+blur_radius),      XY(ss.x+blur, dd.y-blur_radius), true, fake_gradients);
    //        fakeLinearGrad(inner, outer, XY(dd.x-blur, ss.y+blur_radius), XY(dd.x, dd.y-blur_radius),      true, fake_gradients);
    //        //Draw corners
    //        XY c;
    //        c = XY(ss.x+blur_radius, ss.y+blur_radius);
    //        fakeRadialGrad(outer, inner, c, blur_radius, blur_radius-blur, fake_gradients, false, M_PI, 3*M_PI/2);
    //        c = XY(dd.x-blur_radius, ss.y+blur_radius);
    //        fakeRadialGrad(outer, inner, c, blur_radius, blur_radius-blur, fake_gradients, false, 3*M_PI/2, 2*M_PI);
    //        c = XY(dd.x-blur_radius, dd.y-blur_radius);
    //        fakeRadialGrad(outer, inner, c, blur_radius, blur_radius-blur, fake_gradients, false, 0, M_PI/2);
    //        c = XY(ss.x+blur_radius, dd.y-blur_radius);
    //        fakeRadialGrad(outer, inner, c, blur_radius, blur_radius-blur, fake_gradients, false, M_PI/2, M_PI);
    //    } else {
    //        //Draw sides
    //        linearGradient(inner, outer, ss, ss+xy_blur, GRADIENT_UP);
    //        cairo_rectangle(cr, ss.x+blur_radius, ss.y, mm.x, blur);
    //        cairo_fill(cr);
    //        linearGradient(inner, outer, dd-xy_blur, dd, GRADIENT_DOWN);
    //        cairo_rectangle(cr, ss.x+blur_radius, dd.y-blur, mm.x, blur);
    //        cairo_fill(cr);
    //        linearGradient(inner, outer, ss, ss+xy_blur, GRADIENT_LEFT);
    //        cairo_rectangle(cr, ss.x, ss.y+blur_radius, blur, mm.y);
    //        cairo_fill(cr);
    //        linearGradient(inner, outer, dd-xy_blur, dd, GRADIENT_RIGHT);
    //        cairo_rectangle(cr, dd.x-blur, ss.y+blur_radius, blur, mm.y);
    //        cairo_fill(cr);
    //        //Draw corners
    //        XY c;
    //        c = XY(ss.x+blur_radius, ss.y+blur_radius);
    //        radialGradient(inner, outer, c, blur_radius, blur_radius-blur, GRADIENT_OUT);
    //        cairo_move_to(cr, c.x, c.y);
    //        cairo_arc(cr, c.x, c.y, blur_radius, deg2rad(180), deg2rad(270));
    //        cairo_fill(cr);
    //        c = XY(dd.x-blur_radius, ss.y+blur_radius);
    //        radialGradient(inner, outer, c, blur_radius, blur_radius-blur, GRADIENT_OUT);
    //        cairo_move_to(cr, c.x, c.y);
    //        cairo_arc(cr, c.x, c.y, blur_radius, deg2rad(270), 0);
    //        cairo_fill(cr);
    //        c = XY(dd.x-blur_radius, dd.y-blur_radius);
    //        radialGradient(inner, outer, c, blur_radius, blur_radius-blur, GRADIENT_OUT);
    //        cairo_move_to(cr, c.x, c.y);
    //        cairo_arc(cr, c.x, c.y, blur_radius, 0, deg2rad(90));
    //        cairo_fill(cr);
    //        c = XY(ss.x+blur_radius, dd.y-blur_radius);
    //        radialGradient(inner, outer, c, blur_radius, blur_radius-blur, GRADIENT_OUT);
    //        cairo_move_to(cr, c.x, c.y);
    //        cairo_arc(cr, c.x, c.y, blur_radius, deg2rad(90), deg2rad(180));
    //        cairo_fill(cr);
    //    }
    //}
    ////Draw the inner of the shadow, if any of this is visible
    //radius -= blur;
    //if (radius<0) radius = 0;
    //MscLineAttr line2;
    //line2.radius.second = radius;
    //RectanglePath(ss.x+blur, dd.x-blur, ss.y+blur, dd.y-blur, line);
    //SetColor(inner);
    //cairo_fill(cr);
    ////if (clip)
    ////    UnClip();
}

//void MscDrawer::_line_path(double sx, double sy, double dx, double dy, double wider, MscLineType type)
//{
//    const double len = sqrt((dx-sx)*(dx-sx) + (dy-sy)*(dy-sy));
//    if ((type == LINE_DASHED || type == LINE_DOTTED) && fake_dash) {
//        cairo_set_dash(cr, NULL,0,0);
//        const double dash = (type == LINE_DOTTED) ? MSC_DASH_DOTTED : MSC_DASH_DASHED;
//        const double ddx = dash*(dx-sx)/len;
//        const double ddy = dash*(dy-sy)/len;
//        unsigned count = unsigned(len/2/dash);
//        double x = sx+CAIRO_OFF, y = sy+CAIRO_OFF;
//        const double to_x = dx+CAIRO_OFF-2*ddx, to_y = dy+CAIRO_OFF-2*ddy;
//        while(count>0) {
//            cairo_move_to(cr, x, y);
//            cairo_line_to(cr, x+ddx, y+ddy);
//            x += 2*ddx;
//            y += 2*ddy;
//            count--;
//        }
//        cairo_move_to(cr, x, y);
//        cairo_line_to(cr, dx+CAIRO_OFF, dy+CAIRO_OFF);
//    } else if (type == LINE_DOUBLE) {
//        const double DX = (sy-dy)/len*wider;
//        const double DY = (sx-dx)/len*wider;
//        cairo_move_to(cr, sx+DX+CAIRO_OFF, sy-DY+CAIRO_OFF);
//        cairo_line_to(cr, dx+DX+CAIRO_OFF, dy-DY+CAIRO_OFF);
//        cairo_move_to(cr, sx-DX+CAIRO_OFF, sy+DY+CAIRO_OFF);
//        cairo_line_to(cr, dx-DX+CAIRO_OFF, dy+DY+CAIRO_OFF);
//    } else {
//        if (fabs(sx - save_x)>1e-4 || fabs(sy - save_y)>1e-4)
//            cairo_move_to(cr, sx+CAIRO_OFF, sy+CAIRO_OFF);
//        cairo_line_to(cr, dx+CAIRO_OFF, dy+CAIRO_OFF);
//        save_x = dx;
//        save_y = dy;
//    }
//}
//
//void MscDrawer::_arc_path(XY c, XY wh, double s, double e, double wider, MscLineType type, bool reverse)
//{
//    //Handle degenerate case - draw line
//    if (wh.x == 0 || wh.y == 0) {
//        if ((s-e)/360 == floor((s-e)/360)) {
//            s=0;
//            e=180;
//        }
//        //We do not handle complex cases just what is needed for pipe drawing
//        XY src = XY(cos(s*(M_PI/180.))*wh.x/2, sin(s*(M_PI/180.))*wh.y/2) + c;
//        XY dst = XY(cos(e*(M_PI/180.))*wh.x/2, sin(e*(M_PI/180.))*wh.y/2) + c;
//        if (save_x!=-1)
//            _line_path(save_x, save_y, src.x, src.y, 0, type);
//        _line_path(src.x, src.y, dst.x, dst.y, 0, type);
//        save_x = dst.x;
//        save_y = dst.y;
//        return;
//    }
//    if ((type == LINE_DASHED || type == LINE_DOTTED) && fake_dash) {
//        cairo_set_dash(cr, NULL,0,0);
//        double dash = MSC_DASH_DASHED;
//        if (type == LINE_DOTTED)
//            dash = MSC_DASH_DOTTED;
//        cairo_save (cr);
//        cairo_translate (cr, c.x+CAIRO_OFF, c.y+CAIRO_OFF);
//        cairo_scale (cr, wh.x / 2., wh.y / 2.);
//        double rot = dash/(wh.x+wh.y)*2*2;  //dash/radius
//        double ss = s*(M_PI/180.);
//        double ee = e*(M_PI/180.);
//        if (ee<ss) ee += 2*M_PI;
//        while(ss<ee-2*rot) {
//            cairo_new_sub_path(cr);
//            cairo_arc (cr, 0., 0., 1., ss, ss+rot);
//            ss += 2*rot;
//        }
//        cairo_new_sub_path(cr);
//        cairo_arc (cr, 0., 0., 1., ss, ee);
//        cairo_restore (cr);
//    } else if (type == LINE_DOUBLE) {
//        cairo_new_sub_path(cr);
//		_arc_path(c, wh+XY(0, 2*wider), s, e, 0, LINE_SOLID, reverse);
//        cairo_new_sub_path(cr);
//		//ensure that radius adjusted by wider is always nonzero
//		XY w(2*wider, 2*wider);
//		if (wh.x<wider) w.x = wh.x;
//		if (wh.y<wider) w.y = wh.y;
//		_arc_path(c, wh-w, s, e, 0, LINE_SOLID, reverse);
//    } else {
//        cairo_save (cr);
//        cairo_translate (cr, c.x+CAIRO_OFF, c.y+CAIRO_OFF);
//        cairo_scale (cr, wh.x / 2., wh.y / 2.);
//        if (reverse)
//            cairo_arc_negative(cr, 0., 0., 1., s*(M_PI/180.), e*(M_PI/180.));
//        else
//            cairo_arc(cr, 0., 0., 1., s*(M_PI/180.), e*(M_PI/180.));
//        cairo_restore (cr);
//    }
//    save_x = c.x + wh.x*cos(M_PI/180.*e);
//    save_y = c.y + wh.y*sin(M_PI/180.*e);
//}
//
////wider is only used if type is DOUBLE
//void MscDrawer::_rectangle_line_path(XY s, XY d, double offset, double wider,
//                                     double radius, MscLineType type)
//{
//    if ((type == LINE_DASHED || type == LINE_DOTTED) && fake_dash) {
//        _line_path(XY(s.x+radius, s.y), XY(d.x-radius, s.y), wider, type);
//        _line_path(XY(d.x, s.y+radius), XY(d.x, d.y-radius), wider, type);
//        _line_path(XY(s.x+radius, d.y), XY(d.x-radius, d.y), wider, type);
//        _line_path(XY(s.x, s.y+radius), XY(s.x, d.y-radius), wider, type);
//        if (radius>0) {
//            XY rr(2*radius, 2*radius);
//            _arc_path(XY(s.x+radius, s.y+radius), rr, 180, 270, wider, type);
//            _arc_path(XY(d.x-radius, s.y+radius), rr, 270, 360, wider, type);
//            _arc_path(XY(d.x-radius, d.y-radius), rr,   0,  90, wider, type);
//            _arc_path(XY(s.x+radius, d.y-radius), rr,  90, 180, wider, type);
//        }
//    } else if (type == LINE_DOUBLE) {
//        rectangle_path(s, d, CAIRO_OFF, wider, radius);
//        rectangle_path(s, d, CAIRO_OFF, -wider, radius);
//    } else {
//        rectangle_path(s, d, CAIRO_OFF, 0, radius);
//    }
//}
//
//void MscDrawer::rectangle_path(XY s, XY d,
//                               double offset, double wider, double radius)
//{
//    //Normalize radius
//    if (radius<0) radius=0;
//    const double max_radius = std::min(fabs(s.x-d.x), fabs(s.y-d.y))/2-1;
//    if (radius > max_radius) radius = max_radius;
//
//	if (radius==0) {
//        cairo_rectangle(cr, s.x-wider+offset, s.y-wider+offset,
//                        d.x-s.x + 2*wider, d.y-s.y + 2*wider);
//    } else {
//        double r = radius+wider;
//        cairo_move_to(cr, s.x-wider+r+offset, s.y-wider+offset);
//        cairo_arc(cr, d.x+wider-r+offset, s.y-wider+r+offset,
//                  r, 270*(M_PI/180.), 0*(M_PI/180.));
//        cairo_arc(cr, d.x+wider-r+offset, d.y+wider-r+offset,
//                  r, 0*(M_PI/180.), 90*(M_PI/180.));
//        cairo_arc(cr, s.x-wider+r+offset, d.y+wider-r+offset,
//                  r, 90*(M_PI/180.), 180*(M_PI/180.));
//        cairo_arc(cr, s.x-wider+r+offset, s.y-wider+r+offset,
//                  r, 180*(M_PI/180.), 270*(M_PI/180.));
//    }
//    cairo_close_path(cr);
//}
//
//void MscDrawer::rectangle(XY s, XY d, MscLineAttr line)
//{
//    line = (MscLineAttr() += line);
//	if (line.type.second == LINE_NONE || !line.color.second.valid || line.color.second.a==0) return;
//    SetLineAttr(line);
//    _rectangle_line_path(s, d, CAIRO_OFF, line.width.second,
//		                 line.radius.second, line.type.second);
//    cairo_stroke(cr);
//    _new_path();
//}
//
//void MscDrawer::filledRectangle(XY s, XY d, MscFillAttr fill, int radius)
//{
//    fill = (MscFillAttr() += fill);
//    if (!fill.color.second.valid || fill.color.second.a==0) return;
//    d.x++; d.y++;
//	const double max_extent = std::max(fabs(d.x-s.x), fabs(d.y-s.y));
//	//If gradients are to be drawn, we fake them only if no alpha is present
//	//If we draw somewhat transparent, we will fall back to images anyway,
//	//so let us not fake gradients either
//    if (fill.gradient.second!=GRADIENT_NONE && fake_gradients && fill.color.second.a==255) {
//        ClipRectangle(s, d-XY(1,1), radius);
//        MscColorType color = fill.color.second;
//        MscColorType color2 = fill.color2.first ? fill.color2.second : fill.color.second.Lighter(0.8);
//        switch(fill.gradient.second) {
//        case GRADIENT_OUT:
//            _fakeradial(cr, XY((s.x+d.x)/2, (s.y+d.y)/2),
//                        color, color2,
//                        max_extent, 0, fake_gradients, true);
//            break;
//        case GRADIENT_IN:
//            _fakeradial(cr, XY((s.x+d.x)/2, (s.y+d.y)/2),
//                        color2, color,
//                        max_extent, 0, fake_gradients, true);
//            break;
//        case GRADIENT_DOWN:
//            _fakelinear(cr, s, d, color2, color, false, fake_gradients);
//            break;
//        case GRADIENT_UP:
//            _fakelinear(cr, s, d, color, color2, false, fake_gradients);
//            break;
//        case GRADIENT_RIGHT:
//            _fakelinear(cr, s, d, color2, color, true, fake_gradients);
//            break;
//        case GRADIENT_LEFT:
//            _fakelinear(cr, s, d, color, color2, true, fake_gradients);
//            break;
//        case GRADIENT_BUTTON:
//            double step = (d.y-s.y)/100;
//            _fakelinear(cr, s,                d-XY(0, step* 66), color.Lighter(0.66), color.Lighter(0.35), false, fake_gradients*0.34);
//            _fakelinear(cr, s+XY(0, step*34), d                , color.Lighter(0.16), color,               false, fake_gradients*0.66);
//            break;
//        }
//        UnClip();
//    } else {
//        MscColorType to = fill.color.second, from;
//        if (!fill.color2.first) 
//            switch(fill.gradient.second) {
//            case GRADIENT_OUT:
//            case GRADIENT_IN:
//                from = to.Lighter(0.6);
//                break;
//            case GRADIENT_DOWN:
//            case GRADIENT_UP:
//            case GRADIENT_LEFT:
//            case GRADIENT_RIGHT:
//                from = to.Lighter(0.8);
//                break;
//            default:
//                from = to; //to is not used for BUTTON and NONE
//            }
//        else 
//            from = fill.color2.second;
//        if (fill.gradient.second ==GRADIENT_IN || fill.gradient.second == GRADIENT_OUT) {
//            XY c((s.x+d.x)/2, (s.y+d.y)/2);
//            _set_radial_gradient(from, to, c, max_extent, 0, fill.gradient.second);
//        } else {
//            _set_linear_gradient(from, to, s, d, fill.gradient.second);
//        }
//        rectangle_path(s, d, 0, 0, radius);
//        cairo_fill(cr);
//    }
//    _new_path();
//}
//
//
///* Set clip, if the rectangle of which this is the shadow of is not opaque */
//void MscDrawer::shadow(XY s, XY d, MscShadowAttr shadow, int radius, bool clip)
//{
//    shadow = (MscShadowAttr() += shadow);
//    if (shadow.offset.second==0) return;
//	if (!shadow.color.second.valid || shadow.color.second.a==0) return;
//    MscColorType inner = shadow.color.second;
//    d.x++; d.y++;
//
//    //Clip out the actual rectange we are the shadow of
//    if (clip) {
//        rectangle_path(s, d, -CAIRO_OFF, 0, radius);
//        cairo_new_sub_path(cr);
//        cairo_rectangle(cr, 0, 0, total.x, total.y);
//        cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
//        Clip();
//    }
//
//    //Normalize radius
//    if (radius<0) radius=0;
//    const double max_radius = std::min(fabs(s.x-d.x), fabs(s.y-d.y))/2-1;
//    if (radius > max_radius) radius = max_radius;
//    const double blur = std::min(max_radius, shadow.blur.second);
//    const double blur_radius = std::max((double)radius, blur);
//    const XY xy_off(shadow.offset.second, shadow.offset.second);
//    const XY xy_blur(blur, blur);
//    const XY ss = s+xy_off;
//    const XY dd = d+xy_off;
//
//    //Draw the blurred edges first
//    if (shadow.blur.second > 0) {
//        const XY mm = d-s-XY(2*blur_radius, 2*blur_radius);
//        MscColorType outer = shadow.color.second;   outer.a = 0; /* Fully transparent */
//        if (fake_shadows) {
//            /* Should avoid both alpha and gradients */
//            inner = inner.FlattenAlpha();
//            outer = outer.FlattenAlpha();
//            _fakelinear(cr, XY(ss.x+blur_radius, ss.y),      XY(dd.x-blur_radius, ss.y+blur),
//                        outer, inner, false, fake_gradients);
//            _fakelinear(cr, XY(ss.x+blur_radius, dd.y-blur), XY(dd.x-blur_radius, dd.y),
//                        inner, outer, false, fake_gradients);
//            _fakelinear(cr, XY(ss.x, ss.y+blur_radius),      XY(ss.x+blur, dd.y-blur_radius),
//                        outer, inner, true, fake_gradients);
//            _fakelinear(cr, XY(dd.x-blur, ss.y+blur_radius), XY(dd.x, dd.y-blur_radius),
//                        inner, outer, true, fake_gradients);
//            //Draw corners
//            XY c;
//            c = XY(ss.x+blur_radius, ss.y+blur_radius);
//            _fakeradial(cr, c, outer, inner, blur_radius, blur_radius-blur,
//                        fake_gradients, false, 180, 270);
//            c = XY(dd.x-blur_radius, ss.y+blur_radius);
//            _fakeradial(cr, c, outer, inner, blur_radius, blur_radius-blur,
//                        fake_gradients, false, 270, 360);
//            c = XY(dd.x-blur_radius, dd.y-blur_radius);
//            _fakeradial(cr, c, outer, inner, blur_radius, blur_radius-blur,
//                        fake_gradients, false, 0, 90);
//            c = XY(ss.x+blur_radius, dd.y-blur_radius);
//            _fakeradial(cr, c, outer, inner, blur_radius, blur_radius-blur,
//                        fake_gradients, false, 90, 180);
//        } else {
//            //Draw sides
//            _set_linear_gradient(inner, outer, ss, ss+xy_blur, GRADIENT_UP);
//            cairo_rectangle(cr, ss.x+blur_radius, ss.y, mm.x, blur);
//            cairo_fill(cr);
//            _set_linear_gradient(inner, outer, dd-xy_blur, dd, GRADIENT_DOWN);
//            cairo_rectangle(cr, ss.x+blur_radius, dd.y-blur, mm.x, blur);
//            cairo_fill(cr);
//            _set_linear_gradient(inner, outer, ss, ss+xy_blur, GRADIENT_LEFT);
//            cairo_rectangle(cr, ss.x, ss.y+blur_radius, blur, mm.y);
//            cairo_fill(cr);
//            _set_linear_gradient(inner, outer, dd-xy_blur, dd, GRADIENT_RIGHT);
//            cairo_rectangle(cr, dd.x-blur, ss.y+blur_radius, blur, mm.y);
//            cairo_fill(cr);
//            //Draw corners
//            XY c;
//            c = XY(ss.x+blur_radius, ss.y+blur_radius);
//            _set_radial_gradient(inner, outer, c, blur_radius, blur_radius-blur, GRADIENT_OUT);
//            cairo_move_to(cr, c.x, c.y);
//            cairo_arc(cr, c.x, c.y, blur_radius, 180*(M_PI/180.), 270*(M_PI/180.));
//            cairo_fill(cr);
//            c = XY(dd.x-blur_radius, ss.y+blur_radius);
//            _set_radial_gradient(inner, outer, c, blur_radius, blur_radius-blur, GRADIENT_OUT);
//            cairo_move_to(cr, c.x, c.y);
//            cairo_arc(cr, c.x, c.y, blur_radius, 270*(M_PI/180.), 0*(M_PI/180.));
//            cairo_fill(cr);
//            c = XY(dd.x-blur_radius, dd.y-blur_radius);
//            _set_radial_gradient(inner, outer, c, blur_radius, blur_radius-blur, GRADIENT_OUT);
//            cairo_move_to(cr, c.x, c.y);
//            cairo_arc(cr, c.x, c.y, blur_radius, 0*(M_PI/180.), 90*(M_PI/180.));
//            cairo_fill(cr);
//            c = XY(ss.x+blur_radius, dd.y-blur_radius);
//            _set_radial_gradient(inner, outer, c, blur_radius, blur_radius-blur, GRADIENT_OUT);
//            cairo_move_to(cr, c.x, c.y);
//            cairo_arc(cr, c.x, c.y, blur_radius, 90*(M_PI/180.), 180*(M_PI/180.));
//            cairo_fill(cr);
//        }
//    }
//    //Draw the inner of the shadow, if any of this is visible
//    radius -= blur;
//    if (radius<0) radius = 0;
//    rectangle_path(ss+xy_blur, dd-xy_blur, 0, 0, radius);
//    SetColor(inner);
//    cairo_fill(cr);
//    if (clip)
//        UnClip();
//    _new_path();
//}
//

//void MscDrawer::ClipAndMap(const Block &before, const Block &after)
//{
//    cairo_save(cr);
//    cairo_translate(cr, before.UpperLeft().x, before.UpperLeft().y);
//    cairo_scale(cr, before.x.Spans()/after.x.Spans(), before.y.Spans()/after.y.Spans());
//    cairo_translate(cr, -after.UpperLeft().x, -after.UpperLeft().y);
//}



/* END OF FILE */
