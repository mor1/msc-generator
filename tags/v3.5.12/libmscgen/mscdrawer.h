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

/** @file mscdrawer.h The declaration classes for drawing, pagination and file formats (Canvas).
 * @ingroup libmscgen_files */

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
#include "element.h"
#include "stringparse.h"

////////////////////Helpers//////////////////////////////////

/***************************************************************************
 ***************************************************************************/

struct PageBreakData;
/** A collection holding relevant data for all the page breaks in the chart*/
typedef std::vector<PageBreakData> PBDataVector;

/** A class having only static members, providing page size conversion capabilities.*/
class PageSizeInfo
{
public:
    /** Describes fixed page output page sizes.*/
    typedef enum {
        NO_PAGE=1, ///<This value indicates no fixed page output
        A0P, A0L,
        A1P, A1L,
        A2P, A2L,
        A3P, A3L,
        A4P, A4L,
        A5P, A5L,
        A6P, A6L,
        LETTER_P, LETTER_L,
        LEGAL_P, LEGAL_L,
        LEDGER, TABLOID,
        MAX_PAGE ///<Not used, the max value.
    } EPageSize;
    /** Converts a string to a fixed page size*/
    static EPageSize ConvertPageSize(const char *);
    /** Returns the physical size of `ps` in points (1/72 inch)*/
    static XY GetPhysicalPageSize(EPageSize ps);
};


/** A class encapsulating a cairo drawing conext.
 * Besides relaying drawing calls to the cario context Canvas has the following functionality.
 * - Create and manage various target surfaces (PDF, PNG, EPS, EMF, WMF, BMP, recording surface)
 *   including writing to files.
 * - Handle capabilities missing in certain target types (dashed lines, gradient fill,
 *   some parts of transparency) to avoid fallback images as much as possible.
 *   WMF is a very important target in this respect, as OLE charts use it.
 *   See SetLowLevelParams() for a list of the capability tinkering.
 * - Provide a usable EMF->WMF conversion (does not work on XP).
 * - Provide pagination support, with header (automatic pagination headings) and
 *   footer (the copyright text). Supports two modes: when the chart size dictates
 *   the size of the pages and when the chart is drawn on a (series) of fixed-size
 *   pages (printing or -p PDF output).
 *
 * A few notes on output formats.
 * - WMF internally uses an Enhanced Metafile with limited features and 
 * converts to a MetaFile at closure. This is because a WMF DC cannot be given
 * to cairo, since it fails. For a list of tweaks for WMF see SetLowLevelParams().
 * - PRINTER is internally equal to EMF
 * 
 * The term *chart coordinates* refer to the coordinates the ArcBase::Layout() and 
 * ArcBase::Draw() and all Msc functions see. In this space the chart begins
 * at the origin and spans left-down towards growing x and y coordinates.
 * (Thus y coordinate grows downward, compatible with contour assumptions.)
 * Note however, that notes may expand into negative coordinates, thus the final
 * chart area is captured in a Block (the `total` member of Canvas).
 * If the chart is drawn on an unscaled PNG file, then one unit of chart space
 * will translate to one pixel. Similar in PDF and EPS, one unit will translate
 * to one point (1/72 inch in PostScript and PDF).
 * However, the user can ask for scaling (in Canvas::Canvas) and also, for WMF
 * we introduce other scaling mechanisms, so the output may be dawn to a 
 * space different from the chart coordinate space.
 */
class Canvas 
{
public:
    /** Describes graphics output type */
    typedef enum {
        PNG,     ///<Portable Network Graphics bitmap output type
        EPS,     ///<Encapsulated PostScript vector output type
        PDF,     ///<Portable Document Format vector output type
        SVG,     ///<Scalable Vector Graphics vector output type
        EMF,     ///<Windows Enhanced Metafile vector output type
        WMF,     ///<Old-format Windows Metafile vector output type
        EMFWMF,  ///<EMF output, but only uses features compatible with a WMF
        WIN,     ///<Bitmap output to a Windows window DC
        PRINTER  ///<Output to a Windows printer DC
    } EOutputType;
    typedef enum {ERR_OK=0, ERR_FILE, ERR_CANVAS, ERR_PARAM, ERR_MARGIN, ERR_DONE} EErrorType;
protected:
    /** @name Low-level compatibility options 
     * @{ */
    bool     white_background;        /** Draw a white background before chart drawing.*/
    double   fake_scale;              /** Final rendering should be scaled like this. Used for WMF, since WMF coordinates are 16-bit integers. We are better off if the chart is scaeld up (less rounding), but not more than 32K.*/
    double 	 fallback_resolution;     /** The resolution of the fallback images for vector surfaces.*/
    bool     needs_dots_in_corner;    /** Draw a dot in upperleft and lowerright corner. Needed for EMF/WMF for the output to include the white space around the chart.*/
    bool     use_text_path;           /** Use cairo_text_path() instead of cairo_show_text(). I am not sure we have any backend needing this.*/
    bool     use_text_path_rotated;   /** Use cairo_text_path() instead of cairo_show_text() for rotated text. WMF cannot display text if the transformation matrix is rotated. */
    bool     individual_chars;        /** Each character is drawn one by one, and not in a single call to cairo_show_text(). WMF does not scale text nicely, this is a workaround. */
    unsigned fake_gradients;          /** If nonzero then do not use cairo gradients, mimic them with a lot of fills. This is the number of steps. */
    double   scale_for_shadows;       /** How many pixel lies below one logical pixel (needed for fine shadows) */
    bool     fake_shadows;            /** Do not use alpha blending in shadows, use solid color as if on white background. (This avoids fallback image for EMF/WMF, which support no transparency.) */
    bool     fake_dash;               /** Do not use cairo dash, mimic them with individual dashes. WMF supports no dash, EMF supports no dash-offset. */
    bool     fake_spaces;             /** Add space for leading & trailing spaces at cairo_show_text(), assuming those are skipped by it. I do not remember, why we need it. */
    bool     needs_arrow_fix;         /** Cannot do the convex clipping for arrowheads, use a simpler method. */
    bool     avoid_linewidth_1;       /** Never draw lines of width exactly 1.0. EMF with cairo 1.12.2 needs to have wider lines like 1.01. */
    bool     imprecise_positioning;   /** EMF/WMF has trouble doing lines on 0.5 boundaries, avoid this.*/
    bool     can_and_shall_clip_total;/** True if we can clip to the total chart area. Does not work for WIN, so we need this flag */
    bool     avoid_transparency;      /** Pipes and gradient fills will be flattened before output. */
    /** @} */

    /** @name Context data 
     * @{ */
    string           fileName;         /** The name of the output file (if any)*/
    FILE*            outFile;          /** The opened output file (if opened in constructor)*/
    cairo_surface_t *surface;          /** The cairo surface we draw to */
    cairo_t         *cr;               /** The cairo context we draw to */
    EErrorType       status;           /** The status of the Canvas */
    bool             candraw;          /** False, if Canvas cannot be drawn on. Used only in debug asserts.*/
    double           fake_dash_offset; /** Status of fake dashes */
    /** @}*/

    /** @name Values recorded at construction 
     * @{ */
    const EOutputType outType;            ///<The output type
    const Block       total;              ///<The full chart area (in unscaled chart coordinates, all pages) excluding copyright, but including notes and comments
    const bool        external_surface;   ///<True if we got the surface in the constructor externally (no need to destroy it at closure)
    const double      copyrightTextHeight;///<The height of the copyright text
    XY                user_scale;         ///<Stores the scale user specified at construction
    Block             raw_page_clip;      ///<Used with fix-page outoput only. Contains the raw page size in points (pixels) minus margins. All zero if not a fixed page output.
    const int         h_alignment;        ///<Used with fix-page outoput only. Contains -1/0/+1 for left/center/right page alignment.
    const int         v_alignment;        ///<Used with fix-page outoput only. Contains -1/0/+1 for up/center/down page alignment.
    /** @} */

    /** @name Logic 
     * @{ */
    void SetLowLevelParams();
    void GetPagePosition(const PBDataVector *pageBreakData, unsigned page, double &y_offset, double &y_size, double &autoHeadingSize) const;
    EErrorType CreateSurface(const XY &size); 
    EErrorType CreateContext(double origYSize, double origYOffset, double autoHeadingSize);
    /** @} */

friend class StringFormat; //for all sorts of text manipulation
friend class ArcBox;  //for exotic line joints
friend class ArcPipe;  //for exotic line joints
     /** @name Cairo context attribute handing */
    /** Set color, line with and dash */
    void SetLineAttr(LineAttr);
    /** Set dash */
    void SetDash(LineAttr);
    /** Set the font face and bold/italics */
    void SetFontFace(const char*face, bool italics, bool bold);
    /** Set the size of the font */
    void SetFontSize(double size) {cairo_set_font_size (cr, size);}
    /** Query the length of a text string with the currently set font face and size.*/
    double textWidth(const string &s);
    /** Set the source to a single color */
    void SetColor(ColorType); 
    /** Set the source to a linear gradient.*/
    void linearGradient(ColorType from, ColorType to, const XY &s, const XY &d, EGradientType type);
    /** Set the source to a radial gradient.*/
    void radialGradient(ColorType from, ColorType to, const XY &s,
		                double outer_radius, double inner_radius, EGradientType type);
    /** @} */

    /** @name Helpers for drawing operations.
     * @{ */
    /** Draw text with upper-left coordinates `p`. If isRotated is true the transformation matrix is rotated*/
    void Text(XY p, const string &s, bool isRotated);
    /** Create an arc path in cairo*/
    void ArcPath(const contour::EllipseData &ell, double s_rad=0, double e_rad=2*M_PI, bool reverse=false);
    /** Create an arc path in cairo*/
    void ArcPath(const XY &c, double r1, double r2=0, double s_rad=0, double e_rad=2*M_PI, bool reverse=false);
    /** Create a rectangle path in cairo*/
    void RectanglePath(double sx, double dx, double sy, double dy);
    /** Create a rectangle path in cairo, with special corners considering `line.corner` and 'line.radius'*/
    void RectanglePath(double sx, double dx, double sy, double dy, const LineAttr &line);

    /** Fill an area with faked gradient */
    void fakeLinearGrad(ColorType from, ColorType to, const XY &s, const XY &d, bool dir_is_x, unsigned steps);
    /** Fill an area with faked gradient */
    void fakeRadialGrad(ColorType from, ColorType to, const XY &s, double outer_radius, double inner_radius,
                        unsigned steps, bool rectangle, double rad_from=0, double rad_to=2*M_PI);
    /** Stroke a single (non-doble or triple), which may be dashed - consider `fake_dash`*/
    void singleLine(const Block &, const LineAttr &line);
    /** Stroke a single (non-doble or triple), which may be dashed - consider `fake_dash`*/
    void singleLine(const Contour&, const LineAttr &line);
    /** @} */

public:
    Canvas(EOutputType ot);
    Canvas(EOutputType, const Block &tot, double copyrightTextHeight, const string &fn, 
              const XY &scale=XY(1.,1.), 
              const PBDataVector *pageBreakData=NULL, unsigned page=0);
    Canvas(EOutputType, const Block &tot, const string &fn, 
              const std::vector<XY> &scale, const XY &pageSize, 
              const double margins[4],  int ha, int va, 
              double copyrightTextHeight, const PBDataVector *pageBreakData);
    Canvas(EOutputType ot, cairo_surface_t *surf, const Block &tot, 
              double copyrightTextHeight, const XY &scale=XY(1.,1.),
              const PBDataVector *pageBreakData=NULL, unsigned page=0);
    /** Returns the state of the Canvas*/
    EErrorType Status() const {return status;}
    bool TurnPage(const PBDataVector *pageBreakData, unsigned next_page, 
                  MscError *error=NULL);
    bool ErrorAfterCreation(MscError *error,  const PBDataVector *pageBreakData, bool fatal);
#ifdef CAIRO_HAS_WIN32_SURFACE
protected:
    /** @name Windows-related members
     * @{ */
    HDC win32_dc;                        ///<The windows DC we created in the constructor.
    HDC original_hdc;                    ///<The windows DC the user supplied.
    XY original_device_size;             ///<The size of the output `(header+page height+copyright)*fake_scale`
    size_t stored_metafile_size;         ///<When rendering on WMF or EMF, store the size of the resulting file in CloseOutput()
    Contour stored_fallback_image_places;///<When rendering on WMF or EMF, store the location of fallback images in chart space in CloseOutput()
    /** @} */
public:
    Canvas(EOutputType, HDC hdc, const Block &tot=Block(0,0,0,0), double copyrightTextHeight=0, const XY &scale=XY(1.,1.),
              const PBDataVector *pageBreakData=NULL, unsigned page=0);
    /** When rendering on WMF or EMF, return the size of the resulting file. Valid only after CloseOutput().*/
    size_t GetMetaFileSize() const {_ASSERT(outType==WMF||outType==EMF||outType==EMFWMF); return stored_metafile_size;} 
    /** When rendering on WMF or EMF, return the location of fallback images in chart space. Valid only after in CloseOutput()*/
    Contour &GetFallbackImagePlaces() {_ASSERT(outType==WMF||outType==EMF||outType==EMFWMF); return stored_fallback_image_places;} 
    static Contour FallbackImages(HENHMETAFILE hemf, LPRECT lpRECT);
    HENHMETAFILE CloseAndGetEMF();
#endif
public:
    /** Set the fallback image resolution.*/
    void SetFallbackImageResolution(double res) {if (status==ERR_OK) cairo_surface_set_fallback_resolution(surface, res/fake_scale, res/fake_scale);}
    /** Switches to the second drawing situation allowing to draw header (inserted by e.g., automatic pagination) and footer (copyrgight text).*/
    void PrepareForHeaderFoorter();
    /** Flush output, write to file and close canvas.*/
    void CloseOutput();
    ~Canvas() {if (status==ERR_OK) CloseOutput();}

    /** Return our output type*/
    EOutputType GetOutputType() const {return outType;}
    /** Return the current cairo context for direct manipulation*/
    cairo_t *GetContext() const {return cr;}
    /** Our current total size*/
    const Block &GetSize() const {return total;}

    /** True if the target does not support complex clipping */
    bool NeedsArrowFix() const {return needs_arrow_fix;}
    /** True if the target has imprecise positioning */
    bool HasImprecisePositioning() const {return imprecise_positioning;}
    /** True if we should avoid transparency when drawing */
    bool AvoidTransparency() const {return avoid_transparency;}

    /** Set the line join style & return previous one.*/
    cairo_line_join_t SetLineJoin(cairo_line_join_t t);
    /** Set the line cap style & return previous one.*/
    cairo_line_cap_t SetLineCap(cairo_line_cap_t t);
    /** Clip to a rectangle */
    void Clip(double sx, double dx, double sy, double dy);
    /** Clip to a rectangle with corners, see `line.corner` and `line.radius`*/
    void Clip(double sx, double dx, double sy, double dy, const LineAttr &line);
    /** Clip to a rectangle */
    void Clip(const XY &s, const XY &d);
    /** Clip to a rectangle with corners, see `line.corner` and `line.radius`*/
    void Clip(const XY &s, const XY &d, const LineAttr &line);
    /** Clip to a rectangle */
    void Clip(const Block &b);
    /** Clip to a rectangle with corners, see `line.corner` and `line.radius`*/
    void Clip(const Block &b, const LineAttr &line);
    /** Clip to an ellipse */
    void Clip(const contour::EllipseData &ellipse);
    /** Clip to a shape */
    void Clip(const Contour &area);
    /** Clip to the inverse of a rectangle (anything outside `area` but inside `total`*/
    void ClipInverse(const Contour &area);
    /** Undo a previous clip operation*/
    void UnClip() {cairo_restore(cr);}
    void Transform_Rotate90(double s, double d, bool clockwise);
    /** Transform the canvas by swapping x and y coordinates*/
    void Transform_SwapXY();
    /** Transform the canvas by flipping it along the horizontal line at y. */
    void Transform_FlipHorizontal(double y);
    void UnTransform() {cairo_restore(cr);}

    /** Draw a line along `edge` using the line style in `line`*/
    void Line(const Edge& edge, const LineAttr &line);
    /** Draw a straight line from `s` to `d` using the line style in `line`*/
    void Line(const XY &s, const XY &d, const LineAttr &line) {Line(Edge(s, d), line);}
    /** Draw a rectangel line (perhaps with corners) using the line & corner style in `line`*/
    void Line(const Block &b, const LineAttr &line);
    /** Draw a shape using the line style in `line`*/
    void Line(const Contour &area, const LineAttr &line);
    /** Fill a rectangle with `fill`*/
    void Fill(const XY &s, const XY &d, const FillAttr &fill);
    /** Fill a rectangle with corners specified in 'line.corner` with `fill`*/
    void Fill(const XY &s, const XY &d, const LineAttr &line, const FillAttr &fill);
    /** Fill a rectangle with `fill`*/
    void Fill(const Block &b, const FillAttr &fill);
    /** Fill a rectangle with corners specified in 'line.corner` with `fill`*/
    void Fill(const Block &b, const LineAttr &line, const FillAttr &fill);
    /** Fill a shape with `fill`*/
    void Fill(const Contour &area, const FillAttr &fill);
    /**Draw shadow to a rectangle. `angle_radian` is the clockwise rotation*/
    void Shadow(const Block &b, const ShadowAttr &shadow, double angle_radian=0) {Shadow(b, LineAttr(), shadow, angle_radian);}
    /**Draw shadow to a rectangle with corners. `angle_radian` is the clockwise rotation*/
    void Shadow(const Block &b, const LineAttr &line, const ShadowAttr &shadow, double angle_radian=0);
    /**Draw shadow to a shape. `angle_radian` is the clockwise rotation*/
    void Shadow(const Contour &area, const ShadowAttr &shadow, double angle_radian=0);
};

//A number, which is larger than any chart, but small enough for contour to make no mistakes
#define MSC_BIG_COORD 1e5

inline void Canvas::Clip(double sx, double dx, double sy, double dy) {cairo_save(cr); RectanglePath(sx, dx, sy, dy); cairo_clip(cr);}
inline void Canvas::Clip(double sx, double dx, double sy, double dy, const LineAttr &line) {cairo_save(cr); RectanglePath(sx, dx, sy, dy, line); cairo_clip(cr);}
inline void Canvas::Clip(const XY &s, const XY &d) {cairo_save(cr); RectanglePath(s.x, d.x, s.y, d.y); cairo_clip(cr);}
inline void Canvas::Clip(const XY &s, const XY &d, const LineAttr &line) {cairo_save(cr); RectanglePath(s.x, d.x, s.y, d.y, line); cairo_clip(cr);}
inline void Canvas::Clip(const Block &b) {cairo_save(cr); RectanglePath(b.x.from, b.x.till, b.y.from, b.y.till); cairo_clip(cr);}
//inline void Canvas::Clip(const Block &b, const LineAttr &line); not inline
//void Clip(const EllipseData &ellipse); not inline
inline void Canvas::Clip(const Contour &area) {cairo_save(cr); area.Path(cr, true); cairo_clip(cr);}


inline void Canvas::Fill(const XY &s, const XY &d, const FillAttr &fill) {Fill(Block(s, d), fill);}
inline void Canvas::Fill(const XY &s, const XY &d, const LineAttr &line, const FillAttr &fill) {Fill(line.CreateRectangle_ForFill(s, d), fill);}
//void Canvas:: Fill(const Block &b, const FillAttr &fill); //Not inline
inline void Canvas::Fill(const Block &b, const LineAttr &line, const FillAttr &fill) {Clip(b, line); Fill(b, fill); UnClip();}
//void Canvas::Fill(const EllipseData &ellipse, const FillAttr &fill); //Not inline
inline void Canvas::Fill(const Contour &area, const FillAttr &fill) {Clip(area); Fill(area.GetBoundingBox(), fill); UnClip();}


#endif
