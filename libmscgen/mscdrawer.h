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
#include "stringparse.h"

#define CAIRO_OFF (0.5)


////////////////////Helpers//////////////////////////////////

/***************************************************************************
 ***************************************************************************/

class MscCanvas 
{
public:
    //WMF does the same as EMF, but uses features only that can be in WMFs
    typedef enum {PNG, EPS, PDF, SVG, EMF, WMF, WIN} OutputType;
    typedef enum {ERR_OK=0, ERR_FILE, ERR_CANVAS, ERR_PARAM, ERR_DONE} ErrorType;
protected:
    /* Low-level options */
    bool         white_background; /* Draw a white background */
    double       fake_scale; /*final rendering should be scaled like this */
    double 	     fallback_resolution; /* for cairo WMF/EMF backends */
    bool         needs_dots_in_corner; /* Draw a dot in upperleft and lowerright corner */
    bool         use_text_path;  /* Use cairo_text_path() instead of cairo_show_text (windows metafile & truetype font problem)*/
    bool         use_text_path_rotated; /* Use text path() on rotated text */
    bool         individual_chars; /* Each character is drawn one by one */
    unsigned     fake_gradients; /* Do not use cairo gradients, mimic them with a lot of fills, #of steps if non-zero */
    double       scale_for_shadows; /* how many pixel lies below one logical pixel (needed for fine shadows) */
    bool         fake_shadows; /* Do not use alpha blending in shadows */
    bool         fake_dash; /* Do not use cairo dash, mimic them with individual dashes */
    bool         fake_spaces; /* Add space for leading & trailing spaces at text(), assuming those are skipped by it */
    bool         needs_arrow_fix; /*Cannot do the convex clipping for arrowheads */
    bool         imprecise_positioning; /*EMF/WMF has trouble doing lines on 0.5 boundaries*/
    bool         can_and_shall_clip_total;  /* true if we can clip to "total". Does not work for WIN, so we need this flag */
    bool         avoid_transparency; /* true for Win Xp WMFs, where fallback images are slow */
    
    /* Status of fake dashes */
    double       fake_dash_offset;

    /* Context data */
    string      fileName;
    FILE*       outFile;

    cairo_surface_t *surface;
    cairo_t         *cr;
    const OutputType outType;
    const XY         total; //the full chart area (in unscaled chart coordinates, all pages) excluding copyright
    ErrorType        status;
    bool             candraw;

    void SetLowLevelParams(MscCanvas::OutputType ot);
    void GetPagePosition(const std::vector<double> *yPageStart, unsigned page, XY &offset, XY &size) const;
    ErrorType CreateSurface(const XY &size); 
    ErrorType CreateContextFromSurface(OutputType, XY scale, XY origSize, XY origOffset, double copyrightTextHeight);

    void ArcPath(const contour::EllipseData &ell, double s_rad=0, double e_rad=2*M_PI, bool reverse=false);
    void ArcPath(const XY &c, double r1, double r2=0, double s_rad=0, double e_rad=2*M_PI, bool reverse=false);
    void RectanglePath(double sx, double dx, double sy, double dy);
    void RectanglePath(double sx, double dx, double sy, double dy, const MscLineAttr &line);

    void linearGradient(MscColorType from, MscColorType to, const XY &s, const XY &d, MscGradientType type);
    void fakeLinearGrad(MscColorType from, MscColorType to, const XY &s, const XY &d, bool dir_is_x, unsigned steps);
    void radialGradient(MscColorType from, MscColorType to, const XY &s,
		                double outer_radius, double inner_radius, MscGradientType type);
    void fakeRadialGrad(MscColorType from, MscColorType to, const XY &s, double outer_radius, double inner_radius,
                        unsigned steps, bool rectangle, double rad_from=0, double rad_to=2*M_PI);

    void singleLine(const Block &, const MscLineAttr &line);
    void singleLine(const Contour&, const MscLineAttr &line);


friend class StringFormat; //for all sorts of text manipulation
friend class ArcBox;  //for exotic line joints
friend class ArcPipe;  //for exotic line joints
    void SetColor(MscColorType);
    void SetLineAttr(MscLineAttr);
    void SetDash(MscLineAttr);
    void SetFontFace(const char*face, bool italics, bool bold);
    void SetFontSize(double size) {cairo_set_font_size (cr, size);}
    double textWidth(const string &s);
    void Text(XY p, const string &s, bool isRotated);

public:
    MscCanvas(OutputType ot);
    MscCanvas(OutputType, const XY &tot, double copyrightTextHeight, const string &fn, const XY &scale=XY(1.,1.),
              const std::vector<double> *yPageStart=NULL, unsigned page=0);
    ErrorType Status() const {return status;}
#ifdef CAIRO_HAS_WIN32_SURFACE
    HDC win32_dc, original_wmf_hdc;
    MscCanvas(OutputType, HDC hdc, const XY &tot=XY(0,0), double copyrightTextHeight=0, const XY &scale=XY(1.,1.),
              const std::vector<double> *yPageStart=NULL, unsigned page=0);
#endif
    void PrepareForCopyrightText();
    void CloseOutput();
    ~MscCanvas() {if (status==ERR_OK) CloseOutput();}

    OutputType GetOutputType() const {return outType;}
    cairo_t *GetContext() const {return cr;}
    const XY &GetSize() const {return total;}
    bool NeedsArrowFix() const {return needs_arrow_fix;}
    bool HasImprecisePositioning() const {return imprecise_positioning;}
    bool AvoidTransparency() const {return avoid_transparency;}

    cairo_line_join_t SetLineJoin(cairo_line_join_t t);
    cairo_line_cap_t SetLineCap(cairo_line_cap_t t);
    void Clip(double sx, double dx, double sy, double dy);
    void Clip(double sx, double dx, double sy, double dy, const MscLineAttr &line);
    void Clip(const XY &s, const XY &d);
    void Clip(const XY &s, const XY &d, const MscLineAttr &line);
    void Clip(const Block &b);
    void Clip(const Block &b, const MscLineAttr &line);
    void Clip(const contour::EllipseData &ellipse);
    void Clip(const Contour &area);
    void ClipInverse(const Contour &area);
    void UnClip() {cairo_restore(cr);}
    void Transform_Rotate90(double s, double d, bool clockwise);
    void Transform_SwapXY();
    void Transform_FlipHorizontal(double y);
    void UnTransform() {cairo_restore(cr);}

    void Line(const Edge& edge, const MscLineAttr &line);
    void Line(const XY &s, const XY &d, const MscLineAttr &line) {Line(Edge(s, d), line);}
    void Line(const Block &b, const MscLineAttr &line);
    void Line(const Contour &area, const MscLineAttr &line);
    void Fill(const XY &s, const XY &d, const MscFillAttr &fill);
    void Fill(const XY &s, const XY &d, const MscLineAttr &line, const MscFillAttr &fill);
    void Fill(const Block &b, const MscFillAttr &fill);
    void Fill(const Block &b, const MscLineAttr &line, const MscFillAttr &fill);
    void Fill(const Contour &area, const MscFillAttr &fill);
    void Shadow(const Block &b, const MscShadowAttr &shadow, double angle_radian=0) {Shadow(b, MscLineAttr(), shadow, angle_radian);}
    void Shadow(const Block &b, const MscLineAttr &line, const MscShadowAttr &shadow, double angle_radian=0);
    void Shadow(const Contour &area, const MscShadowAttr &shadow, double angle_radian=0);
};

//A number, which is larger than any chart, but small enough for contour to make no mistakes
#define MSC_BIG_COORD 1e5

inline void MscCanvas::Clip(double sx, double dx, double sy, double dy) {cairo_save(cr); RectanglePath(sx, dx, sy, dy); cairo_clip(cr);}
inline void MscCanvas::Clip(double sx, double dx, double sy, double dy, const MscLineAttr &line) {cairo_save(cr); RectanglePath(sx, dx, sy, dy, line); cairo_clip(cr);}
inline void MscCanvas::Clip(const XY &s, const XY &d) {cairo_save(cr); RectanglePath(s.x, d.x, s.y, d.y); cairo_clip(cr);}
inline void MscCanvas::Clip(const XY &s, const XY &d, const MscLineAttr &line) {cairo_save(cr); RectanglePath(s.x, d.x, s.y, d.y, line); cairo_clip(cr);}
inline void MscCanvas::Clip(const Block &b) {cairo_save(cr); RectanglePath(b.x.from, b.x.till, b.y.from, b.y.till); cairo_clip(cr);}
//inline void MscCanvas::Clip(const Block &b, const MscLineAttr &line); not inline
//void Clip(const EllipseData &ellipse); not inline
inline void MscCanvas::Clip(const Contour &area) {cairo_save(cr); area.Path(cr, true); cairo_clip(cr);}


inline void MscCanvas::Fill(const XY &s, const XY &d, const MscFillAttr &fill) {Fill(Block(s, d), fill);}
inline void MscCanvas::Fill(const XY &s, const XY &d, const MscLineAttr &line, const MscFillAttr &fill) {Fill(line.CreateRectangle_ForFill(s, d), fill);}
//void MscCanvas:: Fill(const Block &b, const MscFillAttr &fill); //Not inline
inline void MscCanvas::Fill(const Block &b, const MscLineAttr &line, const MscFillAttr &fill) {Clip(b, line); Fill(b, fill); UnClip();}
//void MscCanvas::Fill(const EllipseData &ellipse, const MscFillAttr &fill); //Not inline
inline void MscCanvas::Fill(const Contour &area, const MscFillAttr &fill) {Clip(area); Fill(area.GetBoundingBox(), fill); UnClip();}


#endif
