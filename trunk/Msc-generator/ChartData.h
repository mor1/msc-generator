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

/** @file ChartData.h The declaration of the CChartData and CDrawingChartData classes.
 * @ingroup Msc_generator_files */

#pragma once
#include <list>
#undef min
#undef max
#include "msc.h"

/** Replaces tabs to spaces in 'str'.*/
void ReplaceTAB(CString &str, unsigned tabsize); 
/** Replaces Windows-style CRLF character pairs at line ends to 
 * Unix-style single character "\n" line endings.*/
void RemoveCRLF(CString &str);
/** Replaces Unix-style single "\n" character line endings to
 * Windows-style CRLF character pairs. */
void EnsureCRLF(CString &str);

class Msc;

/** Stores a chart.
 * Includes chart text, selected design, box & entity collapse state, 
 * page size, fitting, autoheading settings. Everything needed
 * to properly layout the chart.
 * We have two uses for this class. One is that it is one stage in the undo buffer associated
 * with a document in CMscGenDoc. The other is an ancestor for an object that contains a 
 * compiled/drawn chart used for viewing and GUI manipulations, such as tracking or control boxes.*/
class CChartData {
protected:
	CString m_text;                               ///<The chart text
	CString m_ForcedDesign;                       ///<The selected design to force (empty if none)
    EntityCollapseCatalog m_ForcedEntityCollapse; ///<A set of group entity names with collapse state
    ArcSignatureCatalog   m_ForcedArcCollapse;    ///<A set of entries about boxes and their collapse state
    XY m_page_size;                               ///<Page size in pixels, if automatic pagination is requested. No automatic pagination if y==0
    bool m_addHeading;                            ///<True if automatic pagination is requested and headers shall be automatically added at auto page breaks.
    bool m_fitWidth;                              ///<True, if we shall fit the chart to the page size.
public:
    unsigned ver_a;  ///<<Major version of Msc-generator used to create this chart. (Used when serialized in from an OLE object.)
    unsigned ver_b;  ///<<Minor version of Msc-generator used to create this chart. (Used when serialized in from an OLE object.)
    unsigned ver_c;  ///<<Micro version of Msc-generator used to create this chart. (Used when serialized in from an OLE object.)
    /** @name Members relevant only when we are inside an undo buffer.
    * @{ */
    CHARRANGE m_sel; ///<Location of the cursor to undo to
    bool block_undo; ///<If true, no further changes can be appended to this state in the undo buffer
    long start;      ///<The start of the change compared to the previous state
    long ins;        ///<The length of the insertion compared to the previous state
    long del;        ///<The length of the deletion compared to the previous state
    //@}
    /** Default constructor.*/
	CChartData() : m_page_size(0, 0), m_addHeading(true), m_fitWidth(false), 
        ver_a(0), ver_b(0), ver_c(0), 
        block_undo(true), start(0), ins(0), del(0) {m_sel.cpMax = m_sel.cpMin = 0; }
    /** Initialize to a chart text (and optionally a forced design to use)*/
	explicit CChartData(const char *text, const char *design=NULL) 
		:m_text(text?text:""), m_ForcedDesign(design?design:""), 
        m_page_size(0, 0), m_addHeading(true), m_fitWidth(false), 
        ver_a(0), ver_b(0), ver_c(0), 
        block_undo(true), start(0), ins(0), del(0) {m_sel.cpMax = m_sel.cpMin = 0;}
    /** Initialize to a chart text & cursor pos (and optionally a forced design to use)*/
	CChartData(const char *text, const CHARRANGE &sel, const char *design=NULL)
		:m_text(text?text:""), m_sel(sel), m_ForcedDesign(design?design:""),
        m_page_size(0, 0), m_addHeading(true), m_fitWidth(false), ver_a(0), ver_b(0), ver_c(0), 
        block_undo(true), start(0), ins(0), del(0)  {}
    /** Copy constructor.*/
	CChartData(const CChartData&o) {operator=(o);}
    /** Move constructor */
    CChartData(CChartData &&o) { operator=(std::move(o)); }
	virtual ~CChartData() {Delete();}
    /** Swap contets with another CChartData. */
    void swap(CChartData &o);
    /** Empty all content */
	virtual void Delete(void) {m_text.Empty(); m_ForcedDesign.Empty(); m_sel.cpMax = m_sel.cpMin = 0;}
    /** Set our defined version.*/
    void SetVersion(unsigned a, unsigned b, unsigned c) {ver_a=a; ver_b=b; ver_c=c;}
    /** True, if we contain an Msc-generator version used to define us.*/
    bool HasVersion() const {return ver_a>0;}
    /** Compares (a,b,c) to our version. Returns +1 if our version is later, -1 if earlier
     * or 0 if the same or we do not contain a version.*/
    int CompareVersion(unsigned a, unsigned b, unsigned c) const;
    /** Set the page size.*/
    virtual void SetPageSize(const XY &s) {m_page_size = s;}
    /** Get the page size */
    const XY &GetPageSize() const {return m_page_size;}
    /** Set AddHeading property */
    virtual void SetAddHeading(bool b) {m_addHeading = b;}
    /** Get Addheading property */
    bool GetAddHeading() const {return m_addHeading;}
    /** Set the Fit to Width property */
    virtual void SetFitWidth(bool b) {m_fitWidth = b;}
    /** Get the Fit to Width property */
    bool GetFitWidth() const {return m_fitWidth;}
    /** Saves the chart text to a file on disk. 
     * At error, use CFileException::ReportError to report the error and returns false.*/
	BOOL Save(const CString &fileName);
    /** Loads the chart text from a file on disk.
    * At error, use CFileException::ReportError to report the error and returns false.*/
    BOOL Load(const CString &fileName, BOOL reportError = TRUE);
    /** Sets the chart text, deletes forced design and invalidate cursor pos. */
	void Set(const char *text) {Delete(); m_text=text?text:"";}
    /** Sets a forced design. Use empty string to cancel forced design.*/
    virtual void SetDesign(const char *design) { if (design) m_ForcedDesign = design; }
    /**Reads the name of the forced design, empty string if none*/
	const CString &GetDesign () const {return m_ForcedDesign;}
    /** Get the current chart text. */
	const CString &GetText() const {return m_text;}
    /** Removes spaces at line ends. Leaves cursor position in a bad state.*/
	void RemoveSpacesAtLineEnds();
    /** True if we have no text. */
	BOOL IsEmpty() const {return m_text.GetLength()==0;}
    /** Forces an entity to a collapsed/expanded state*/
    virtual bool ForceEntityCollapse(const std::string &s, bool b);
    /** Sets the entity collapse state in one go. Returns true if it effected a real change.*/
    virtual bool ForceEntityCollapse(const EntityCollapseCatalog&);
    /** Get the entity collapse state.*/
    const EntityCollapseCatalog &GetForcedEntityCollapse() const {return m_ForcedEntityCollapse;}
    /** Forces a box to be collapsed/expanded/arrow-collapsed*/
    virtual bool ForceArcCollapse(const ArcSignature &, EBoxCollapseType t);
    /** Sets the box collapse state in one go. Returns true if it effected a real change.*/
    virtual bool ForceArcCollapse(const ArcSignatureCatalog &);
    /** Get the box collapse state.*/
    const ArcSignatureCatalog &GetForcedArcCollapse() const {return m_ForcedArcCollapse;}
};

/** Stores a drawn chart.
 * Includes a drawing cache, so that redraws or page changes are faster.*/
class CDrawingChartData : public CChartData {
public:
    /** What kind of cache do we use.*/
    enum ECacheType {
        /** Use an in-memory metafile as cache. 
         * Used when showing an embedded object with the embedded object panel.
         * Using this mode the chart shows exactly as it would in the container object,
         * including fallback image quality. (Which is the purpose in that view.)*/
        CACHE_EMF,
        CACHE_RECORDING ///<Using a cairo recording surface as cache. Higher quality and allows re-use at page changes.
    };
protected:
	mutable Msc         *m_msc;    ///<The chart we have drawn.
    mutable bool         compiled; ///<True if chart is compiled and can draw.
	Msc *GetMsc() const {_ASSERT(m_msc); return m_msc;} ///<Return the chart object.

    /** @name Cache related members.
     * @{ */
    ECacheType               m_cacheType;            ///<What type of cache do we use?
	mutable HENHMETAFILE     m_cache_EMF;            ///<The cache of the current page (or all if all pages are selected) if we use EMF type.
    mutable cairo_surface_t *m_cache_rec;            ///<The cache of the current page (or all if all pages are selected) if we use a recording surface
    mutable cairo_surface_t *m_cache_rec_full_no_pb; ///<The full chart drawn without page break indicators - used only if either we show one page of many or if we show all but does not show page break indicators. We re-draw pages from this at page changes.
    //@}
    /** @name Data collected during the population of the EMF cache
    * @{ */
    mutable Contour          m_fallback_image_places;///<The location of the fallback images in m_msc coordinates.
    mutable size_t           m_wmf_size;             ///<The size of the resulted WMF metafile
    //@}
    /** @name Parameters for drawing the cache
    * @{ */
    double                   m_fallback_resolution; ///<The fallback resolution to use for EMF
    unsigned                 m_page;                ///<The selected page to draw (0=all)
    bool                     m_pageBreaks;          ///<If we shall draw page break indicators if m_page=0
    bool                     m_pedantic;            ///<If compilation shall be pedantic or not
    const std::map<std::string, Context> *m_designs;///<The designs to use at compilation (from designlib)
    const ShapeCollection   *m_shapes;              ///<The shapes the user can select from (from designlib)
    const MscError          *m_design_errors;       ///<The error messages happened during designlib compilation
    CString                  m_copyright;           ///<The copyright text used at the bottom of each page 
    //@}

    MscProgress::ProgressCallback m_callback;       ///<The callback function to call during compilation 
    void *m_callback_data;                          ///<The parameter to pass to the progress callback

    /** @name Compilation related helpers
    * @{ */
    void Invalidate() const;     ///<Completely invalidate compiled stuff - requires full re-compilation and drawing (e.g., at text or forced design change)
    void ClearCache() const;     ///<Invalidate the cache - does not require full re-compilation just a re-draw using Msc::Draw() (e.g., at page break indicator on/off change)
    void InvalidatePage() const; ///<Invaludate the current page. Same as ClearCache for EMF cache, but for cairo, we may re-use the recording surface to replay just one page.
    //@}

public:
    mutable CString m_load_data; ///<Load data: approximations on how much time each operation takes during compilation
    /** Default constructor */
    CDrawingChartData() : m_msc(NULL), compiled(false), m_cacheType(CACHE_RECORDING), m_cache_EMF(NULL), 
                    m_cache_rec(NULL), m_cache_rec_full_no_pb(NULL), m_wmf_size(0),
                    m_fallback_resolution(300), m_page(0), m_pageBreaks(false), m_pedantic(false),
                    m_designs(NULL), m_shapes(NULL), m_design_errors(NULL),
                    m_callback(NULL), m_callback_data(NULL) {}
    /** Initialize from a ChartData - but no compilation yet.*/
	CDrawingChartData(const CChartData&o);
    /** Copy Constructor */
    CDrawingChartData(const CDrawingChartData&o);
    /** Move Constructor */
    CDrawingChartData(CDrawingChartData &&o);
    /** Initialize from a ChartData - but no compilation yet.*/
    CDrawingChartData & operator = (const CChartData& o) { Invalidate(); CChartData::operator =(o); return *this; }
    /** Empty all content */
	virtual void Delete(void);
    /** Swap with another CDrawingCahrtData */
    void swap(CDrawingChartData &o);
    /** Set the progress callback */
    void SetProgressCallback(MscProgress::ProgressCallback cb=NULL, void *d=NULL) {m_callback = cb; m_callback_data = d;}
    /** @name Set properties - these require recompile, which we do not do
    * @{ */
    /** Set the forced design */
	virtual void SetDesign (const char *design);
    /** Set if we want auto heading for auto page breaks.*/
    virtual void SetAddHeading(bool b);
    /** Set if we fit to width. */
    virtual void SetFitWidth(bool b);
    /** Set if we compuile pedantic*/
    virtual void SetPedantic(bool b);
    bool ForceEntityCollapse(const std::string &s, bool b);
    bool ForceEntityCollapse(const EntityCollapseCatalog &);
    bool ForceArcCollapse(const ArcSignature &, EBoxCollapseType t);
    bool ForceArcCollapse(const ArcSignatureCatalog &);
    //@}
    /** @name Set other properties - these require some level of redraw only
    * @{ */
    void SetCacheType(ECacheType t) { if (m_cacheType!=t) { ClearCache(); m_cacheType = t; } }
    ECacheType GetCacheType() const {return m_cacheType;}
    void SetPage(unsigned page);
    unsigned GetPage() const {return m_page;}
    void SetPageBreaks(bool pageBreaks);
    bool GetPageBreaks() const {return m_pageBreaks;}
    void SetFallbackResolution(double d);
    double GetFallbackResolution() const {return m_fallback_resolution;}
    void SetPageSize(const XY &s);
    void SetDesignsShapes(const std::map<std::string, Context> *, const ShapeCollection *, const MscError *);
    void SetCopyRightText(const char *);
    const std::map<std::string, Context> &GetDesigns() const {return GetMsc()->Designs;}
    //@}
    /** @name Compilation members
    * @{ */
    bool IsCompiled() const { return compiled; }
    /** Compiles the chart & return true if recompiled or at least re-drawn the cache */
	bool CompileIfNeeded() const;
    //@}
    /** @name Members related to resulting error messages
    * @{ */
    /** Returns true if the error 'num' (interpreted according to 'oWarnings') is
     *  in the last file used at compilation (=in the chart text).*/
    bool IsErrorInFile(unsigned num, bool oWarnings) const;
    /** Get number of compilation errors. if oWarnings true, we add warnings, as well. */
    unsigned GetErrorNum(bool oWarnings) const;
    /** Get line number of error 'num'. 'num' interpreted based on oWarnings.*/
	unsigned GetErrorLine(unsigned num, bool oWarnings) const;
    /** Get column of error 'num'. 'num' interpreted based on oWarnings.*/
    unsigned GetErrorCol(unsigned num, bool oWarnings) const;
    /** Get error text of error 'num'. 'num' interpreted based on oWarnings.*/
    CString GetErrorText(unsigned num, bool oWarnings) const;
    /** Get list of designs available after compilation.*/
    CString GetDesignNames() const;
    /** Get the id number of the shapes we used (for saving them in OLE objects)*/
    const std::set<unsigned> &GetUsedShapes() const { return GetMsc()->used_shapes; }
    //@}
    /** @name Members related to drawing.
    * @{ */
    /** Get the number of pages in the chart*/
    unsigned GetPages() const;
    /** Get the size of the current page (or specify your own)*/
	CSize GetSize(unsigned forced_page=0) const;
    /**Get the total size of the chart in Msc coordinates.*/
    const Block &GetMscTotal() const;
    /** Get the coordiante of the nth page top-left corner in Msc coordinates*/
	XY GetPageOrigin(unsigned page) const;
    /** Get the page area of the current page without autoheader and coyright text in Msc coordinates*/
    Block GetNetPageBlock() const; 
    /** Get the height of the heading on the current page (for autoheading)*/
    double GetHeadingSize() const;
    /** Draws the chart to one or more files. 
     * @param [in] fileName The name of the file. If empty "Untitled" is used.
     *                      The extension is used to determine type, if not recognized
     *                      'png' is added and used. In case of multiple pages, a number
     *                      is also added to the end of the name (before the extension).
     * @param [in] bPageBreaks If true, the chart is multi-page and we draw all pages
     *                         in the same file (ignore_pagebreaks is true), this one
     *                         decides if we indicate page breaks or not.
     * @param [in] ignore_pagebreaks If set we draw all of a multi-paged chart in the same
     *                               single file.
     * @param [in] x_scale Scaling in x direction.
     * @param [in] y_scale Scaling in y direction.
     * @param [in] pageSize If non-zero, a fixed page size is used and a single multi-page
     *                      file will be created (only with PDF).
     * @param [in] margins For fixed size pages, these are the margins
     * @param [in] ha The horizontal alignment for fix size pages (-1: left, 0: center, +1:right)
     * @param [in] va The vertical alignment for fix size pages (-1:up, 0: center, +1:bottom) */
     void DrawToFile(const char* fileName, bool bPageBreaks, bool ignore_pagebreaks,
                    double x_scale=1.0, double y_scale=1.0, 
                    const XY &pageSize = XY(0, 0), const double margins[4] = NULL,
                    int ha = 0, int va = 0) const;
    /** Draw the chart to a Windows Device Context (DC), metafile or a printer DC.
     * @param [in] ot The format of output. Determines what type of metafile we use.
     *                can only be WMF, EMF, EMFWMF or PRINTER.
     * @param hdc The DC to draw onto.
     * @param [in] scale Scale the chart by this amount
     * @param [in] page The page to draw, zero for the whole chart.
     * @param [in] bPageBreaks If true and we draw the whole chart and the chart has multiple
     *                         pages, draw dashed lines for page breaks.
     * @param [in] fallback_image_resolution Draw fallback images at this resolution.
     * @param [in] generateErrors If true and we cannot avoid overfill, we geenrate a warining.
     *                            Errors are also generated on file creation and similar hard errors.
     * @returns The size of the metafile or zero at error.*/
    size_t DrawToDC(Canvas::EOutputType ot, HDC hdc, const XY &scale,
                  unsigned page, bool bPageBreaks,
                  double fallback_image_resolution=-1, 
                  bool generateErrors=false) const
    {return GetMsc()->DrawToDC(ot, hdc, scale, page, bPageBreaks, fallback_image_resolution, generateErrors);}
    /** Draws the chart (from cache) to a memory Device Context, representing a bitmap.
     * Used to do flicker-free scrolling and zooming.
     * @param memDC Target DC. Contains a bitmap as large as the visible part of the screen.
     * @param [in] x_scale Magnification factor we shall display the chart with.
     * @param [in] y_scale Magnification factor we shall display the chart with.
     * @param [in] clip Contains the coordinates of the visible part of the screen in scaled 
     *                  chart coordinates. E.g., if the chart is 100x100, scale is 2, and we 
     *                  can see 80x80 pixels in the middle of the chart (a small window), then 
     *                  the scaled chart is 200x200 and clip will be (60x60)->(140x140)*/
    void DrawToMemDC(CDC &memDC, double x_scale, double y_scale, const CRect &clip);

    /** Returns the size of the drawn WMF (with EMF caching only).*/
    size_t GetWMFSize() const {_ASSERT(m_cacheType==CACHE_EMF && m_cache_EMF); return m_wmf_size;}
    /** Returns the location of the fallback images in Msc coordinates (with EMF caching only).*/
    const Contour &GetWMFFallbackImagePos() const {_ASSERT(m_cacheType==CACHE_EMF && m_cache_EMF); return m_fallback_image_places;}
    //@}
    /** @name Members related to getting the coverage of arc elements.
    * @{ */
    /** Return which element is topmost at `point` (in Msc coordinates).*/
    Element *GetArcByCoordinate(CPoint point) const;
    /** Return which element is at (line, col) in the chart text.*/
	Element *GetArcByLine(unsigned line, unsigned col) const;
    //@}
};

/** An iterator pointing to a state in the undo buffer.*/
typedef std::list<CChartData>::iterator IChartData;
/** An iterator pointing to a state in the undo buffer.*/
typedef std::list<CChartData>::const_iterator IChartData_const;

