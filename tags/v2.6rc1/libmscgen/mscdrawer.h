#if !defined(MSCDRAW_H)
#define MSCDRAW_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <set>
#include "cairo.h"
#ifdef CAIRO_HAS_WIN32_SURFACE
#include "cairo-win32.h"
#undef max
#undef min
#endif
#include "error.h"
#include "attribute.h"
#include "trackable.h"

#define CAIRO_OFF (0.5)


////////////////////Helpers//////////////////////////////////

/***************************************************************************
 ***************************************************************************/

// Base class for Mscs that can draw themselves
class MscDrawer
{
  public:
    //WMF does the same as EMF, but uses features only that can be in WMFs
    typedef enum {PNG, EPS, PDF, SVG, EMF, WMF, WIN} OutputType;
  protected:
    string      fileName;
    OutputType  outType;
    FILE*       outFile;

    cairo_surface_t *surface;
    cairo_t *cr;
    double save_x, save_y;

    void _line_path(XY s, XY d, double wider, MscLineType type)
        {_line_path(s.x, s.y, d.x, d.y, wider, type);}
    void _rectangle_line_path(XY s, XY, double offset, double wider, double radius, MscLineType type);
    void rectangle_path(XY s, XY, double offset=0, double wider=0, double radius=0);
    void _set_linear_gradient(MscColorType from, MscColorType to, XY s, XY d, MscGradientType type);
    void _set_radial_gradient(MscColorType from, MscColorType to, XY s,
		                      double outer_radius, double inner_radius, MscGradientType type);
  public:
    MscError     Error;
    unsigned     current_file;  /* The number of the file under parsing, plus the error location */
    /** The total width & height of the drawing and the height of the bottom copyright text (all to be calculated) */
    double totalWidth, totalHeight;
    double copyrightTextHeight;
	/** The starting ypos of each page. Its number of elements specify the number of pages yPageStart[0] is always 0. */
    std::vector<double> yPageStart;

    /* Low-level drawing options */
    bool         white_background; /* Draw a white background */
    bool         use_text_path;  /* Use cairo_text_path() instead of cairo_show_text (windows metafile & truetype font problem)*/
    bool         use_text_path_rotated; /* Use text path() on rotated text */
    bool         individual_chars; /* Each character is drawn one by one */
    unsigned     fake_gradients; /* Do not use cairo gradients, mimic them with a lot of fills, #of steps if non-zero */
    bool         fake_shadows; /* Do not use alpha blending in shadows */
    bool         fake_dash; /* Do not use cairo dash, mimic them with individual dashes */
	bool         fake_spaces; /* Add space for leading & trailing spaces at text(), assuming those are skipped by it */
    double       scale; /*final rendering should be scaled like this */
    unsigned	 fallback_resolution; /* for cairo WMF backends */

    MscDrawer();
    void SetLowLevelParams(OutputType ot=PNG);
    void GetPagePosition(int page, XY &offset, XY &size) const;
    bool SetOutput(OutputType, const string &fn=string(), int page=-1);
#ifdef CAIRO_HAS_WIN32_SURFACE
    HDC win32_dc, save_hdc;
    bool SetOutputWin32(OutputType, HDC hdc, int page=-1);
    HENHMETAFILE CloseOutputRetainHandleEMF();
    HMETAFILE CloseOutputRetainHandleWMF();
#endif
    void CloseOutput();

    cairo_t *GetContext() const {return cr;}
    void Clip() {cairo_save(cr); cairo_clip(cr);}
    void ClipAndMap(const Block &before, const Block &after);
    void ClipRectangle(XY s, XY d, int radius=0)
       {rectangle_path(s, d+XY(1,1), 0, 0, radius); Clip();}
    void UnClip() {cairo_restore(cr);}
    void Rotate90(double s, double d, bool clockwise=true);

    void SetColor(MscColorType);
    void SetLineAttr(MscLineAttr);
    void SetFontFace(const char*face, bool italics, bool bold);
    void SetFontSize(double size) {cairo_set_font_size (cr, size);}

    void text(XY p, const string &s, bool isRotated);
    double textWidth(const string &s);

    void arc(XY c, XY wh, double s, double e, MscLineAttr line);

    void line(XY s, XY d, MscLineAttr line);
    void rectangle(XY s, XY d, MscLineAttr line);
    void filledRectangle(XY s, XY d, MscFillAttr fill, int radius=0);
    void shadow(XY s, XY d, MscShadowAttr shadow, int radius=0, bool clip=false);

    void _new_path() {save_x = save_y = -1; cairo_new_sub_path(cr);}
    void _line_path(double sx, double sy, double dx, double dy, double wider, MscLineType type);
    void _line_path_to(double dx, double dy, double wider, MscLineType type)
        {_line_path(save_x, save_y, dx, dy, wider, type);}
    void _arc_path(XY c, XY wh, double s, double e, double wider, MscLineType type, bool reverse=false);
};

#endif
