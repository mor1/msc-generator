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
    bool         needs_dots_in_corner; /* Draw a dot in upperleft and lowerright corner */

    cairo_surface_t *surface;
    cairo_t *cr;

    void SetLowLevelParams(OutputType ot=PNG);

    void ArcPath(const contour::Ellipse &ell, double s_rad=0, double e_rad=2*M_PI, bool reverse=false);
    void ArcPath(const XY &c, double r1, double r2=0, double s_rad=0, double e_rad=2*M_PI, bool reverse=false);
    void RectanglePath(double sx, double dx, double sy, double dy);
    void RectanglePath(double sx, double dx, double sy, double dy, const MscLineAttr &line);

    void fakeDashedLine(const XY &s, const XY &d, 
                        const double pattern[], int num, int &pos, double &offset);
    void fakeDashedLine(const XY &c, double r1, double r2, double tilt, double s, double e, 
                        const double pattern[], int num, int &pos, double &offset, bool reverse);
    void singleLine(const XY &s, const XY &d, const MscLineAttr &line);
    void singleLine(const XY &c, double r1, double r2, double tilt, double s, double e, const MscLineAttr &line, bool reverse=false);
    void singleLine(const Block &, const MscLineAttr &line);
    void singleLine(const Contour &, const MscLineAttr &line, bool open);
    void singleLine(const ContourList&, const MscLineAttr &line);
    void singleLine(const Area&, const MscLineAttr &line);


    void linearGradient(MscColorType from, MscColorType to, const XY &s, const XY &d, MscGradientType type);
    void fakeLinearGrad(MscColorType from, MscColorType to, const XY &s, const XY &d, bool dir_is_x, unsigned steps);
    void radialGradient(MscColorType from, MscColorType to, const XY &s,
		                double outer_radius, double inner_radius, MscGradientType type);
    void fakeRadialGrad(MscColorType from, MscColorType to, const XY &s, double outer_radius, double inner_radius,
                        unsigned steps, bool rectangle, double rad_from=0, double rad_to=2*M_PI);

friend class StringFormat;
    void SetColor(MscColorType);
    void SetLineAttr(MscLineAttr);
    void SetFontFace(const char*face, bool italics, bool bold);
    void SetFontSize(double size) {cairo_set_font_size (cr, size);}
    double textWidth(const string &s);
    void Text(XY p, const string &s, bool isRotated);

  public:
    MscDrawer();
    void GetPagePosition(int page, XY &offset, XY &size) const;
    bool SetOutput(OutputType, const string &fn=string(), int page=-1);
#ifdef CAIRO_HAS_WIN32_SURFACE
    HDC win32_dc, save_hdc;
    bool SetOutputWin32(OutputType, HDC hdc, int page=-1);
    HENHMETAFILE CloseOutputRetainHandleEMF();
    HMETAFILE CloseOutputRetainHandleWMF();
#endif
    void CloseOutput();

    MscError     Error;
    unsigned     current_file;  /* The number of the file under parsing, plus the error location */
    /** The total width & height of the drawing and the height of the bottom copyright text (all to be calculated) */
    XY total;
    double copyrightTextHeight;
	/** The starting ypos of each page, one for each page. yPageStart[0] is always 0. */
    std::vector<double> yPageStart;

    cairo_line_join_t SetLineJoin(cairo_line_join_t t);
    cairo_line_cap_t SetLineCap(cairo_line_cap_t t);

    cairo_t *GetContext() const {return cr;}
    void Clip(double sx, double dx, double sy, double dy);
    void Clip(double sx, double dx, double sy, double dy, const MscLineAttr &line);
    void Clip(const XY &s, const XY &d);
    void Clip(const XY &s, const XY &d, const MscLineAttr &line);
    void Clip(const Block &b);
    void Clip(const Block &b, const MscLineAttr &line);
    void Clip(const contour::Ellipse &ellipse);
    void Clip(const Area &area);
    void UnClip() {cairo_restore(cr);}
    void Transform_Rotate90(double s, double d, bool clockwise);
    void Transform_SwapXY();
    void Transform_FlipHorizontal(double y);
    void UnTransform() {cairo_restore(cr);}

    void Line(const Edge& edge, const MscLineAttr &line);          
    void Line(const XY &s, const XY &d, const MscLineAttr &line);                
    void Line(const Block &b, const MscLineAttr &line);
    void Line(const Contour &contour, const MscLineAttr &line);    
    void Line(const Area &area, const MscLineAttr &line);          
    void LineOpen(const Contour &contour, const MscLineAttr &line);  //an arbitrary contour, but not its last edge
    void LineWithJoints(const std::list<Contour> &c, const MscLineAttr &line); 
    void Fill(const XY &s, const XY &d, const MscFillAttr &fill);
    void Fill(const XY &s, const XY &d, const MscLineAttr &line, const MscFillAttr &fill);
    void Fill(const Block &b, const MscFillAttr &fill);
    void Fill(const Block &b, const MscLineAttr &line, const MscFillAttr &fill);
    void Fill(const contour::Ellipse &ellipse, const MscFillAttr &fill);
    void Fill(const Contour &contour, const MscFillAttr &fill);
    void Fill(const Area &area, const MscFillAttr &fill);
    void Shadow(const Block &b, const MscShadowAttr &shadow, bool clip=false) {Shadow(b, MscLineAttr(), shadow, clip);}
    void Shadow(const Block &b, const MscLineAttr &line, const MscShadowAttr &shadow, bool clip=false);
    void Shadow(const Area &area, const MscShadowAttr &shadow, bool clip=false);
};



inline void MscDrawer::Clip(double sx, double dx, double sy, double dy) {cairo_save(cr); RectanglePath(sx, dx, sy, dy); cairo_clip(cr);}
inline void MscDrawer::Clip(double sx, double dx, double sy, double dy, const MscLineAttr &line) {cairo_save(cr); RectanglePath(sx, dx, sy, dy, line); cairo_clip(cr);}
inline void MscDrawer::Clip(const XY &s, const XY &d) {cairo_save(cr); RectanglePath(s.x, d.x, s.y, d.y); cairo_clip(cr);}
inline void MscDrawer::Clip(const XY &s, const XY &d, const MscLineAttr &line) {cairo_save(cr); RectanglePath(s.x, d.x, s.y, d.y, line); cairo_clip(cr);}
inline void MscDrawer::Clip(const Block &b) {cairo_save(cr); RectanglePath(b.x.from, b.x.till, b.y.from, b.y.till); cairo_clip(cr);}
inline void MscDrawer::Clip(const Block &b, const MscLineAttr &line) {cairo_save(cr); RectanglePath(b.x.from, b.x.till, b.y.from, b.y.till, line); cairo_clip(cr);}
//void Clip(const contour::Ellipse &ellipse); not inline
inline void MscDrawer::Clip(const Area &area) {cairo_save(cr); area.Path(cr); cairo_clip(cr);}


inline void MscDrawer::Fill(const XY &s, const XY &d, const MscFillAttr &fill) {Fill(Block(s, d), fill);}
inline void MscDrawer::Fill(const XY &s, const XY &d, const MscLineAttr &line, const MscFillAttr &fill) {Fill(Block(s, d), fill);}
//void MscDrawer:: Fill(const Block &b, const MscFillAttr &fill); //Not inline
inline void MscDrawer::Fill(const Block &b, const MscLineAttr &line, const MscFillAttr &fill) {Clip(b, line); Fill(b, fill); UnClip();}
//void MscDrawer::Fill(const contour::Ellipse &ellipse, const MscFillAttr &fill); //Not inline
inline void MscDrawer::Fill(const Contour &c, const MscFillAttr &fill) {Clip(c); Fill(c.GetBoundingBox(), fill); UnClip();}
inline void MscDrawer::Fill(const Area &area, const MscFillAttr &fill) {Clip(area); Fill(area.GetBoundingBox(), fill); UnClip();}


#endif
