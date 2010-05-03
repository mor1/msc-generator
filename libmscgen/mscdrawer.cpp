/*
    This file is part of Msc-generator.
	Copyright 2008,2009,2010 Zoltan Turanyi
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


MscDrawer::MscDrawer() :
    totalWidth(0), totalHeight(0), copyrightTextHeight(0),
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
    scale = 1.0;
    fallback_resolution = 300; //irrelevant for bitmaps - no fallback for bitmaps

    if (ot == PNG) {
        white_background = true;
    }
    if (ot == EMF) {
        fake_gradients = 30;
        fake_shadows = true;
        scale = 1;              //do 10 for better precision clipping
        fallback_resolution = 100; //300 is too much for on-screen work
    }
    if (ot == WMF) {
        individual_chars = true; //do this so that it is easier to convert to WMF
        use_text_path_rotated = true;
        fake_dash = true;
        fake_gradients = 30;
        fake_shadows = true;
        scale = 1;              //do 10 for better precision clipping
        fallback_resolution = 100; //300 is too much for on-screen work
    }
    if (ot == WIN) {
        individual_chars = true; //do this to look the very same as for EMF output
    }
}

//page numbering starts from 0, -1 means all of the chart (ignoring page breaks)
//This one does not take bottom banner into account
void MscDrawer::GetPagePosition(int page, XY &offset, XY &size) const
{
    offset.x = 0;
    size.x = totalWidth;
    if (page<=-1) {
        size.y = totalHeight;
        offset.y = 0;
    } else if (page>=yPageStart.size()) { //too large page
        size.y = 0; //nothing drawn
        offset.y = totalHeight;
    } else if (page==yPageStart.size()-1) { //last page
        size.y = totalHeight - yPageStart[page];
        offset.y = yPageStart[page];
    } else {
        size.y = yPageStart[page+1] - yPageStart[page];
        offset.y = yPageStart[page];
    }
}

//page numbering starts from 0
//Use this to save to a file or to determine dimensions
bool MscDrawer::SetOutput(OutputType ot, const string &fn, int page)
{
    assert(ot!=WIN);
    if (surface) CloseOutput();

    SetLowLevelParams(ot);

    XY size, origSize, offset;
    GetPagePosition(page, offset, origSize);
    size.y += copyrightTextHeight;
    offset = offset*scale;
    size = origSize*scale;

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
         ** In cairo_gstate.c when _cairo_gstate_show_text_glyphs calls
         ** _cairo_gstate_transform_glyphs_to_backend, the one but last
         ** parameter (num_transformed_glyphs) shall be set to NULL
         ** to avoid dropping glyphs outside the DC's extent.
         ** I could not make a metafile DC to have larger extent that 1600x1200
		 ** An alternate tweak is to modify cairo_win32_printing_surface_create
		 ** diretcly to use an extent bigger than the one reported by GetClipBox().
         */
        surface = cairo_win32_printing_surface_create(win32_dc);
        break;

	//case WMF:
        // Create the Enhanced Metafile
        if (fileName.length() > 0)
            win32_dc = CreateMetaFile(fileName.c_str());
        else
            win32_dc = CreateMetaFile(NULL);
        // Did you get a good metafile?
        if( win32_dc == NULL ) {
            fileName.clear();
            return false;
        }
        HRGN hrgn = CreateRectRgn(0, 0, size.x, size.y);
        SelectClipRgn(win32_dc, hrgn);
        DeleteObject(hrgn);
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
    cairo_surface_set_fallback_resolution (surface, fallback_resolution, fallback_resolution); //default is 300 DPI

    cr = cairo_create (surface);
    st = cairo_status(cr);
    if (st != CAIRO_STATUS_SUCCESS) {
        CloseOutput();
        return false;
    }

    cairo_translate(cr, -offset.x, -offset.y);
    cairo_scale(cr, scale, scale);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);

    if (totalWidth>0 && totalHeight>0) {
        //clip first to allow for banner text
        ClipRectangle(XY(0,0), origSize + XY(0, copyrightTextHeight));
        //then clip again to exclude banner text
        ClipRectangle(XY(0,0), origSize);
    }
    return true;
}

#ifdef CAIRO_HAS_WIN32_SURFACE

//Draw to a DC that is either a display DC or a metafile
//Use this to display a chart, use SetOutput to save the chart to a file
bool MscDrawer::SetOutputWin32(OutputType ot, HDC hdc, double zoom, int page)
{
    if (surface) CloseOutput();
    if (ot!=WIN && ot!=WMF && ot!=EMF) return false;

    outType = ot;
    outFile = NULL;
    win32_dc = NULL;
    save_hdc = hdc;
    fileName.clear();

    SetLowLevelParams(ot);

    XY origSize, size, offset;
    GetPagePosition(page, offset, origSize);
    size.y += copyrightTextHeight;
    offset = offset*(scale*zoom/100.);
    size = origSize*(scale*zoom/100.);

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
    }

    cr = cairo_create (surface);

    cairo_status_t st = cairo_surface_status(surface);
    if (st) goto problem;
    st = cairo_status(cr);
    if (st) goto problem;

    cairo_translate(cr, -offset.x, -offset.y);
    cairo_scale(cr, scale*zoom/100., scale*zoom/100.);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);

    if (totalWidth>0 && totalHeight>0) {
        //clip first to allow for banner text
        ClipRectangle(XY(0,0), origSize + XY(0, copyrightTextHeight));
        if (ot == EMF || ot == WMF) {
            //dots in the corner for EMF
            MscLineAttr l(MscColorType(255,255,255));
            line(XY(0,origSize.y+copyrightTextHeight), origSize+ XY(0, copyrightTextHeight), l);
        }
        //then clip again to exclude banner text
        ClipRectangle(XY(0,0), origSize);
    }
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
        SetRect(&r, 0, 0, totalWidth, totalHeight);
        HDC hdc = CreateMetaFile(NULL);
        PaintEMFonWMFdc(hemf, hdc, r, true);
        DeleteEnhMetaFile(hemf);
        return CloseMetaFile(hdc);
    }
}
#endif //WIN32

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
                    SetRect(&r, 0, 0, totalWidth, totalHeight);
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

void MscDrawer::Rotate90(double s, double d, bool clockwise)
{
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

void MscDrawer::SetColor(MscColorType pen)
{
    cairo_set_source_rgba(cr, pen.r/255.0, pen.g/255.0, pen.b/255.0, pen.a/255.0);
}

#define MSC_DASH_DOTTED 2
#define MSC_DASH_DASHED 6

void MscDrawer::SetLineAttr(MscLineAttr line)
{
    static const double dash_dotted[2]={MSC_DASH_DOTTED, MSC_DASH_DOTTED};
    static const double dash_dashed[2]={MSC_DASH_DASHED, MSC_DASH_DASHED};
    if (line.type.first)
        switch (line.type.second) {
        case LINE_DOTTED:
            cairo_set_dash(cr, dash_dotted,2,0); break;
        case LINE_DASHED:
            cairo_set_dash(cr, dash_dashed,2,0); break;
        case LINE_DOUBLE:
        case LINE_SOLID:
            cairo_set_dash(cr, NULL,0,0); break;
        };
    if (line.color.first)
        SetColor(line.color.second);
    if (line.width.first)
        cairo_set_line_width(cr, line.width.second);

}

void _add_color_stop(cairo_pattern_t *pattern, double offset, MscColorType color)
{
    cairo_pattern_add_color_stop_rgba(pattern, offset,
                                      color.r/255., color.g/255.,
                                      color.b/255., color.a/255.);
}

void MscDrawer::_set_linear_gradient(MscColorType from, MscColorType to, XY s, XY d, MscGradientType type)
{
    cairo_pattern_t *pattern;
    switch(type) {
    case GRADIENT_OUT:
    case GRADIENT_IN:
        //*Should not happen here*/
        assert(0);
        break;
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
        _add_color_stop(pattern, 0,    from.Lighter(0.8));
        _add_color_stop(pattern, 0.43, from);
        _add_color_stop(pattern, 0.5,  from.Darker(0.1));
        _add_color_stop(pattern, 1,    from);
        break;
    case GRADIENT_NONE:
    default:
        pattern = cairo_pattern_create_rgba(from.r/255., from.g/255., from.b/255., from.a/255.);
        break;
    }
    cairo_set_source(cr, pattern);
}

void MscDrawer::_set_radial_gradient(MscColorType from, MscColorType to, XY s, double outer_radius, double inner_radius, MscGradientType type)
{
    cairo_pattern_t *pattern;
    switch(type) {
    default:
        assert(0);
        break;
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

void MscDrawer::SetFontFace(const char*face, bool italics, bool bold)
{
    cairo_select_font_face (cr, face,
                            italics?CAIRO_FONT_SLANT_ITALIC:CAIRO_FONT_SLANT_NORMAL,
                            bold?CAIRO_FONT_WEIGHT_BOLD:CAIRO_FONT_WEIGHT_NORMAL);
}

void MscDrawer::_line_path(double sx, double sy, double dx, double dy, double wider, MscLineType type)
{
    const double len = sqrt((dx-sx)*(dx-sx) + (dy-sy)*(dy-sy));
    if ((type == LINE_DASHED || type == LINE_DOTTED) && fake_dash) {
        cairo_set_dash(cr, NULL,0,0);
        const double dash = (type == LINE_DOTTED) ? MSC_DASH_DOTTED : MSC_DASH_DASHED;
        const double ddx = dash*(dx-sx)/len;
        const double ddy = dash*(dy-sy)/len;
        unsigned count = unsigned(len/2/dash);
        double x = sx+CAIRO_OFF, y = sy+CAIRO_OFF;
        const double to_x = dx+CAIRO_OFF-2*ddx, to_y = dy+CAIRO_OFF-2*ddy;
        while(count>0) {
            cairo_move_to(cr, x, y);
            cairo_line_to(cr, x+ddx, y+ddy);
            x += 2*ddx;
            y += 2*ddy;
            count--;
        }
        cairo_move_to(cr, x, y);
        cairo_line_to(cr, dx+CAIRO_OFF, dy+CAIRO_OFF);
    } else if (type == LINE_DOUBLE) {
        const double DX = (sy-dy)/len*wider;
        const double DY = (sx-dx)/len*wider;
        cairo_move_to(cr, sx+DX+CAIRO_OFF, sy-DY+CAIRO_OFF);
        cairo_line_to(cr, dx+DX+CAIRO_OFF, dy-DY+CAIRO_OFF);
        cairo_move_to(cr, sx-DX+CAIRO_OFF, sy+DY+CAIRO_OFF);
        cairo_line_to(cr, dx-DX+CAIRO_OFF, dy+DY+CAIRO_OFF);
    } else {
        if (fabs(sx - save_x)>1e-4 || fabs(sy - save_y)>1e-4)
            cairo_move_to(cr, sx+CAIRO_OFF, sy+CAIRO_OFF);
        cairo_line_to(cr, dx+CAIRO_OFF, dy+CAIRO_OFF);
        save_x = dx;
        save_y = dy;
    }
}

void MscDrawer::line(XY s, XY d, MscLineAttr line)
{
    line = (MscLineAttr() += line);
    if (line.type.second == LINE_NONE || !line.color.second.valid) return;
    SetLineAttr(line);
    _line_path(s, d, line.width.second, line.type.second);
    cairo_stroke(cr);
    _new_path();
}

void MscDrawer::text(XY xy, const string &s, bool isRotated)
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
    _new_path();
}

void MscDrawer::_arc_path(XY c, XY wh, double s, double e, double wider, MscLineType type, bool reverse)
{
    //Handle degenerate case - draw line
    if (wh.x == 0 || wh.y == 0) {
        if ((s-e)/360 == floor((s-e)/360)) {
            s=0;
            e=180;
        }
        //We do not handle complex cases just what is needed for pipe drawing
        XY src = XY(cos(s*(M_PI/180.))*wh.x/2, sin(s*(M_PI/180.))*wh.y/2) + c;
        XY dst = XY(cos(e*(M_PI/180.))*wh.x/2, sin(e*(M_PI/180.))*wh.y/2) + c;
        if (save_x!=-1)
            _line_path(save_x, save_y, src.x, src.y, 0, type);
        _line_path(src.x, src.y, dst.x, dst.y, 0, type);
        save_x = dst.x;
        save_y = dst.y;
        return;
    }
    if ((type == LINE_DASHED || type == LINE_DOTTED) && fake_dash) {
        cairo_set_dash(cr, NULL,0,0);
        double dash = MSC_DASH_DASHED;
        if (type == LINE_DOTTED)
            dash = MSC_DASH_DOTTED;
        cairo_save (cr);
        cairo_translate (cr, c.x+CAIRO_OFF, c.y+CAIRO_OFF);
        cairo_scale (cr, wh.x / 2., wh.y / 2.);
        double rot = dash/(wh.x+wh.y)*2*2;  //dash/radius
        double ss = s*(M_PI/180.);
        double ee = e*(M_PI/180.);
        if (ee<ss) ee += 2*M_PI;
        while(ss<ee-2*rot) {
            cairo_new_sub_path(cr);
            cairo_arc (cr, 0., 0., 1., ss, ss+rot);
            ss += 2*rot;
        }
        cairo_new_sub_path(cr);
        cairo_arc (cr, 0., 0., 1., ss, ee);
        cairo_restore (cr);
    } else if (type == LINE_DOUBLE) {
        cairo_save (cr);
        cairo_translate (cr, c.x+CAIRO_OFF, c.y+CAIRO_OFF);
        cairo_scale (cr, (wh.x-wider) / 2., (wh.y-wider) / 2.);
        cairo_new_sub_path(cr);
        cairo_arc (cr, 0., 0., 1., s*(M_PI/180.), e*(M_PI/180.));
        cairo_restore (cr);
        cairo_save (cr);
        cairo_translate (cr, c.x+CAIRO_OFF, c.y+CAIRO_OFF);
        cairo_scale (cr, (wh.x+wider) / 2., (wh.y+wider) / 2.);
        cairo_new_sub_path(cr);
        cairo_arc (cr, 0., 0., 1., s*(M_PI/180.), e*(M_PI/180.));
        cairo_restore (cr);
    } else {
        cairo_save (cr);
        cairo_translate (cr, c.x+CAIRO_OFF, c.y+CAIRO_OFF);
        cairo_scale (cr, wh.x / 2., wh.y / 2.);
        if (reverse)
            cairo_arc_negative(cr, 0., 0., 1., s*(M_PI/180.), e*(M_PI/180.));
        else
            cairo_arc(cr, 0., 0., 1., s*(M_PI/180.), e*(M_PI/180.));
        cairo_restore (cr);
    }
    save_x = c.x + wh.x*cos(M_PI/180.*e);
    save_y = c.y + wh.y*sin(M_PI/180.*e);
}

void MscDrawer::arc(XY c, XY wh, double s, double e,
                    MscLineAttr line)
{
    line = (MscLineAttr() += line);
    if (line.type.second == LINE_NONE || !line.color.second.valid) return;
    SetLineAttr(line);
    _arc_path(c, wh, s, e, line.width.second, line.type.second);
    cairo_stroke(cr);
    _new_path();
}

//wider is only used if type is DOUBLE
void MscDrawer::_rectangle_line_path(XY s, XY d, double offset, double wider,
                                     double radius, MscLineType type)
{
    if ((type == LINE_DASHED || type == LINE_DOTTED) && fake_dash) {
        _line_path(XY(s.x+radius, s.y), XY(d.x-radius, s.y), wider, type);
        _line_path(XY(d.x, s.y+radius), XY(d.x, d.y-radius), wider, type);
        _line_path(XY(s.x+radius, d.y), XY(d.x-radius, d.y), wider, type);
        _line_path(XY(s.x, s.y+radius), XY(s.x, d.y-radius), wider, type);
        if (radius>0) {
            XY rr(2*radius, 2*radius);
            _arc_path(XY(s.x+radius, s.y+radius), rr, 180, 270, wider, type);
            _arc_path(XY(d.x-radius, s.y+radius), rr, 270, 360, wider, type);
            _arc_path(XY(d.x-radius, d.y-radius), rr,   0,  90, wider, type);
            _arc_path(XY(s.x+radius, d.y-radius), rr,  90, 180, wider, type);
        }
    } else if (type == LINE_DOUBLE) {
        rectangle_path(s, d, CAIRO_OFF, wider, radius);
        rectangle_path(s, d, CAIRO_OFF, -wider, radius);
    } else {
        rectangle_path(s, d, CAIRO_OFF, 0, radius);
    }
}

void MscDrawer::rectangle_path(XY s, XY d,
                               double offset, double wider, double radius)
{
    //Normalize radius
    if (radius<0) radius=0;
    const double max_radius = std::min(fabs(s.x-d.x), fabs(s.y-d.y))/2-1;
    if (radius > max_radius) radius = max_radius;

	if (radius==0) {
        cairo_rectangle(cr, s.x-wider+offset, s.y-wider+offset,
                        d.x-s.x + 2*wider, d.y-s.y + 2*wider);
    } else {
        double r = radius+wider;
        cairo_move_to(cr, s.x-wider+r+offset, s.y-wider+offset);
        cairo_arc(cr, d.x+wider-r+offset, s.y-wider+r+offset,
                  r, 270*(M_PI/180.), 0*(M_PI/180.));
        cairo_arc(cr, d.x+wider-r+offset, d.y+wider-r+offset,
                  r, 0*(M_PI/180.), 90*(M_PI/180.));
        cairo_arc(cr, s.x-wider+r+offset, d.y+wider-r+offset,
                  r, 90*(M_PI/180.), 180*(M_PI/180.));
        cairo_arc(cr, s.x-wider+r+offset, s.y-wider+r+offset,
                  r, 180*(M_PI/180.), 270*(M_PI/180.));
    }
    cairo_close_path(cr);
}

void MscDrawer::rectangle(XY s, XY d, MscLineAttr line)
{
    line = (MscLineAttr() += line);
    if (line.type.second == LINE_NONE || !line.color.second.valid) return;
    SetLineAttr(line);
    _rectangle_line_path(s, d, CAIRO_OFF, line.width.second,
		                 line.radius.second, line.type.second);
    cairo_stroke(cr);
    _new_path();
}

void _fakelinear(cairo_t *cr, XY s, XY d, MscColorType from, MscColorType to,
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

//from is from the outside, caller must apply clipping!!!
void _fakeradial(cairo_t *cr, XY s,MscColorType from, MscColorType to, double outer_radius, double inner_radius,
                 unsigned steps, bool rectangle, double arc_from=0, double arc_to=360)
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
        cairo_arc (cr, s.x, s.y, current, arc_from*(M_PI/180.), arc_to*(M_PI/180.));
        cairo_set_source_rgba(cr, r, g, b, a);
        cairo_fill(cr);
        current -= advance;
        r+=dr; g+=dg; b+=db; a+=da;
    }
}

/* Set clip, if the rectangle of which this is the shadow of is not opaque */
void MscDrawer::shadow(XY s, XY d, MscShadowAttr shadow, int radius, bool clip)
{
    shadow = (MscShadowAttr() += shadow);
    if (shadow.offset.second==0) return;
    MscColorType inner = shadow.color.second;
    d.x++; d.y++;

    //Clip out the actual rectange we are the shadow of
    if (clip) {
        rectangle_path(s, d, -CAIRO_OFF, 0, radius);
        cairo_new_sub_path(cr);
        cairo_rectangle(cr, 0, 0, totalWidth, totalHeight);
        cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
        Clip();
    }

    //Normalize radius
    if (radius<0) radius=0;
    const double max_radius = std::min(fabs(s.x-d.x), fabs(s.y-d.y))/2-1;
    if (radius > max_radius) radius = max_radius;
    const double blur = std::min(max_radius, shadow.blur.second);
    const double blur_radius = std::max((double)radius, blur);
    const XY xy_off(shadow.offset.second, shadow.offset.second);
    const XY xy_blur(blur, blur);
    const XY ss = s+xy_off;
    const XY dd = d+xy_off;

    //Draw the blurred edges first
    if (shadow.blur.second > 0) {
        const XY mm = d-s-XY(2*blur_radius, 2*blur_radius);
        MscColorType outer = shadow.color.second;   outer.a = 0; /* Fully transparent */
        if (fake_shadows) {
            /* Should avoid both alpha and gradients */
            inner = inner.FlattenAlpha();
            outer = outer.FlattenAlpha();
            _fakelinear(cr, XY(ss.x+blur_radius, ss.y),      XY(dd.x-blur_radius, ss.y+blur),
                        outer, inner, false, fake_gradients);
            _fakelinear(cr, XY(ss.x+blur_radius, dd.y-blur), XY(dd.x-blur_radius, dd.y),
                        inner, outer, false, fake_gradients);
            _fakelinear(cr, XY(ss.x, ss.y+blur_radius),      XY(ss.x+blur, dd.y-blur_radius),
                        outer, inner, true, fake_gradients);
            _fakelinear(cr, XY(dd.x-blur, ss.y+blur_radius), XY(dd.x, dd.y-blur_radius),
                        inner, outer, true, fake_gradients);
            //Draw corners
            XY c;
            c = XY(ss.x+blur_radius, ss.y+blur_radius);
            _fakeradial(cr, c, outer, inner, blur_radius, blur_radius-blur,
                        fake_gradients, false, 180, 270);
            c = XY(dd.x-blur_radius, ss.y+blur_radius);
            _fakeradial(cr, c, outer, inner, blur_radius, blur_radius-blur,
                        fake_gradients, false, 270, 360);
            c = XY(dd.x-blur_radius, dd.y-blur_radius);
            _fakeradial(cr, c, outer, inner, blur_radius, blur_radius-blur,
                        fake_gradients, false, 0, 90);
            c = XY(ss.x+blur_radius, dd.y-blur_radius);
            _fakeradial(cr, c, outer, inner, blur_radius, blur_radius-blur,
                        fake_gradients, false, 90, 180);
        } else {
            //Draw sides
            _set_linear_gradient(inner, outer, ss, ss+xy_blur, GRADIENT_UP);
            cairo_rectangle(cr, ss.x+blur_radius, ss.y, mm.x, blur);
            cairo_fill(cr);
            _set_linear_gradient(inner, outer, dd-xy_blur, dd, GRADIENT_DOWN);
            cairo_rectangle(cr, ss.x+blur_radius, dd.y-blur, mm.x, blur);
            cairo_fill(cr);
            _set_linear_gradient(inner, outer, ss, ss+xy_blur, GRADIENT_LEFT);
            cairo_rectangle(cr, ss.x, ss.y+blur_radius, blur, mm.y);
            cairo_fill(cr);
            _set_linear_gradient(inner, outer, dd-xy_blur, dd, GRADIENT_RIGHT);
            cairo_rectangle(cr, dd.x-blur, ss.y+blur_radius, blur, mm.y);
            cairo_fill(cr);
            //Draw corners
            XY c;
            c = XY(ss.x+blur_radius, ss.y+blur_radius);
            _set_radial_gradient(inner, outer, c, blur_radius, blur_radius-blur, GRADIENT_OUT);
            cairo_move_to(cr, c.x, c.y);
            cairo_arc(cr, c.x, c.y, blur_radius, 180*(M_PI/180.), 270*(M_PI/180.));
            cairo_fill(cr);
            c = XY(dd.x-blur_radius, ss.y+blur_radius);
            _set_radial_gradient(inner, outer, c, blur_radius, blur_radius-blur, GRADIENT_OUT);
            cairo_move_to(cr, c.x, c.y);
            cairo_arc(cr, c.x, c.y, blur_radius, 270*(M_PI/180.), 0*(M_PI/180.));
            cairo_fill(cr);
            c = XY(dd.x-blur_radius, dd.y-blur_radius);
            _set_radial_gradient(inner, outer, c, blur_radius, blur_radius-blur, GRADIENT_OUT);
            cairo_move_to(cr, c.x, c.y);
            cairo_arc(cr, c.x, c.y, blur_radius, 0*(M_PI/180.), 90*(M_PI/180.));
            cairo_fill(cr);
            c = XY(ss.x+blur_radius, dd.y-blur_radius);
            _set_radial_gradient(inner, outer, c, blur_radius, blur_radius-blur, GRADIENT_OUT);
            cairo_move_to(cr, c.x, c.y);
            cairo_arc(cr, c.x, c.y, blur_radius, 90*(M_PI/180.), 180*(M_PI/180.));
            cairo_fill(cr);
        }
    }
    //Draw the inner of the shadow, if any of this is visible
    radius -= blur;
    if (radius<0) radius = 0;
    rectangle_path(ss+xy_blur, dd-xy_blur, 0, 0, radius);
    SetColor(inner);
    cairo_fill(cr);
    if (clip)
        UnClip();
    _new_path();
}

void MscDrawer::filledRectangle(XY s, XY d, MscFillAttr fill, int radius)
{
    fill = (MscFillAttr() += fill);
    if (!fill.color.second.valid || fill.color.second.a==0) return;
    d.x++; d.y++;
	const double max_extent = std::max(fabs(d.x-s.x), fabs(d.y-s.y));
	//If gradients are to be drawn, we fake them only if no alpha is present
	//If we draw somewhat transparent, we will fall back to images anyway,
	//so let us not fake gradients either
    if (fill.gradient.second!=GRADIENT_NONE && fake_gradients && fill.color.second.a==255) {
        ClipRectangle(s, d-XY(1,1), radius);
        MscColorType color = fill.color.second;
        switch(fill.gradient.second) {
        case GRADIENT_OUT:
            _fakeradial(cr, XY((s.x+d.x)/2, (s.y+d.y)/2),
                        color, color.Lighter(0.8),
                        max_extent, 0, fake_gradients, true);
            break;
        case GRADIENT_IN:
            _fakeradial(cr, XY((s.x+d.x)/2, (s.y+d.y)/2),
                        color.Lighter(0.8), color,
                        max_extent, 0, fake_gradients, true);
            break;
        case GRADIENT_DOWN:
            _fakelinear(cr, s, d, color.Lighter(0.8), color, false, fake_gradients);
            break;
        case GRADIENT_UP:
            _fakelinear(cr, s, d, color, color.Lighter(0.8), false, fake_gradients);
            break;
        case GRADIENT_RIGHT:
            _fakelinear(cr, s, d, color.Lighter(0.8), color, true, fake_gradients);
            break;
        case GRADIENT_LEFT:
            _fakelinear(cr, s, d, color, color.Lighter(0.8), true, fake_gradients);
            break;
        case GRADIENT_BUTTON:
            double step = (d.y-s.y)/100;
            _fakelinear(cr, s,                d-XY(0, step* 66), color.Lighter(0.66), color.Lighter(0.35), false, fake_gradients*0.34);
            _fakelinear(cr, s+XY(0, step*34), d                , color.Lighter(0.16), color,               false, fake_gradients*0.66);
            break;
        }
        UnClip();
    } else {
        MscColorType to = fill.color.second, from;
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
        if (fill.gradient.second ==GRADIENT_IN || fill.gradient.second == GRADIENT_OUT) {
            XY c((s.x+d.x)/2, (s.y+d.y)/2);
            _set_radial_gradient(from, to, c, max_extent, 0, fill.gradient.second);
        } else {
            _set_linear_gradient(from, to, s, d, fill.gradient.second);
        }
        rectangle_path(s, d, 0, 0, radius);
        cairo_fill(cr);
    }
    _new_path();
}


/* END OF FILE */
