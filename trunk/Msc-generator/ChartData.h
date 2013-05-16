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


#pragma once
#include <list>
#undef min
#undef max
#include "msc.h"

void ReplaceTAB(CString &str, unsigned tabsize=6); //6 seems to be default for RichEditCtrl
void RemoveCRLF(CString &str);
void EnsureCRLF(CString &str);

class Msc;

class CChartData {
protected:
	CString m_text;
	CString m_ForcedDesign;
    EntityCollapseCatalog m_ForcedEntityCollapse; 
    ArcSignatureCatalog   m_ForcedArcCollapse; 
    XY m_page_size; //no automatic pagination if y==0
    bool m_addHeading;
    bool m_fitWidth;
public:
    unsigned ver_a, ver_b, ver_c;
	CHARRANGE m_sel;
	CChartData() : m_page_size(0, 0), m_addHeading(true), m_fitWidth(false), 
        ver_a(0), ver_b(0), ver_c(0) {m_sel.cpMax = m_sel.cpMin = 0;}
	CChartData(const char *text, const char *design=NULL) 
		:m_text(text?text:""), m_ForcedDesign(design?design:""), 
        m_page_size(0, 0), m_addHeading(true), m_fitWidth(false), 
        ver_a(0), ver_b(0), ver_c(0) {m_sel.cpMax = m_sel.cpMin = 0;}
	CChartData(const char *text, const CHARRANGE &sel, const char *design=NULL)
		:m_text(text?text:""), m_sel(sel), m_ForcedDesign(design?design:""),
        m_page_size(0, 0), m_addHeading(true), m_fitWidth(false), ver_a(0), ver_b(0), ver_c(0)  {}
	CChartData(const CChartData&o) {operator=(o);}
	virtual ~CChartData() {Delete();}
    void swap(CChartData &o);
	virtual void Delete(void) {m_text.Empty(); m_ForcedDesign.Empty(); m_sel.cpMax = m_sel.cpMin = 0;}
    void SetVersion(unsigned a, unsigned b, unsigned c) {ver_a=a; ver_b=b; ver_c=c;}
    bool HasVersion() const {return ver_a>0;}
    int CompareVersion(unsigned a, unsigned b, unsigned c) const;
    virtual void SetPageSize(const XY &s) {m_page_size = s;}
    const XY &GetPageSize() const {return m_page_size;}
    virtual void SetAddHeading(bool b) {m_addHeading = b;}
    bool GetAddHeading() const {return m_addHeading;}
    virtual void SetFitWidth(bool b) {m_fitWidth = b;}
    bool GetFitWidth() const {return m_fitWidth;}
	BOOL Save(const CString &fileName);
	BOOL Load(const CString &fileName,  BOOL reportError=TRUE);
	void Set(const char *text) {Delete(); m_text=text?text:"";}
	virtual void SetDesign (const char *design);
	CString GetDesign () const {return m_ForcedDesign;}
	const CString &GetText() const {return m_text;}
	void RemoveSpacesAtLineEnds();
	BOOL IsEmpty() const {return m_text.GetLength()==0;}
    virtual bool ForceEntityCollapse(const std::string &s, bool b);
    virtual bool ForceEntityCollapse(const EntityCollapseCatalog&);
    const EntityCollapseCatalog &GetForcedEntityCollapse() const {return m_ForcedEntityCollapse;}
    virtual bool ForceArcCollapse(const ArcSignature &, EBoxCollapseType t);
    virtual bool ForceArcCollapse(const ArcSignatureCatalog &);
    const ArcSignatureCatalog &GetForcedArcCollapse() const {return m_ForcedArcCollapse;}
};

class CDrawingChartData : public CChartData {
public:
    enum ECacheType {CACHE_EMF, CACHE_RECORDING};
protected:
	mutable Msc         *m_msc;
    mutable bool         compiled;
	Msc *GetMsc() const {_ASSERT(m_msc); return m_msc;}

    //Caching related
    ECacheType               m_cacheType;
	mutable HENHMETAFILE     m_cache_EMF;
    mutable cairo_surface_t *m_cache_rec;
    mutable cairo_surface_t *m_cache_rec_full_no_pb;
    //Returning EMF data
    mutable Contour          m_fallback_image_places;
    mutable size_t           m_wmf_size;
    //Parameters for drawing the cache
    double                   m_fallback_resolution;
    unsigned                 m_page;
    bool                     m_pageBreaks;
    bool                     m_pedantic;       
    const std::map<std::string, Context> *m_designs;
    const MscError          *m_design_errors;
    CString                  m_copyright;

    MscProgress::ProgressCallback m_callback;
    void *m_callback_data;

//Compilation related
    void Invalidate() const;
    void ClearCache() const;
    void InvalidatePage() const;

public:
    mutable CString m_load_data;
    CDrawingChartData() : m_msc(NULL), compiled(false), m_cacheType(CACHE_RECORDING), m_cache_EMF(NULL), 
                    m_cache_rec(NULL), m_cache_rec_full_no_pb(NULL), m_wmf_size(0),
                    m_fallback_resolution(300), m_page(0), m_pageBreaks(false), m_pedantic(false),
                    m_designs(NULL), m_design_errors(NULL),
                    m_callback(NULL), m_callback_data(NULL) {}
	CDrawingChartData(const CChartData&o);
	CDrawingChartData(const CDrawingChartData&o);
	CDrawingChartData & operator = (const CChartData& o) {Invalidate(); CChartData::operator =(o); return *this;}
	virtual void Delete(void);
    void swap(CDrawingChartData &o);
    void SetProgressCallback(MscProgress::ProgressCallback cb=NULL, void *d=NULL) {m_callback = cb; m_callback_data = d;}
//Set parameters - these require recompile
	virtual void SetDesign (const char *design);
    virtual void SetAddHeading(bool b);
    virtual void SetFitWidth(bool b);
    virtual void SetPedantic(bool b);
    bool ForceEntityCollapse(const std::string &s, bool b);
    bool ForceEntityCollapse(const EntityCollapseCatalog &);
    bool ForceArcCollapse(const ArcSignature &, EBoxCollapseType t);
    bool ForceArcCollapse(const ArcSignatureCatalog &);
//Set Parameters, these only require some level of redraw only
    void SetCacheType(ECacheType t) {if (m_cacheType!=t) {ClearCache(); m_cacheType=t;}}
    ECacheType GetCacheType() const {return m_cacheType;}
    void SetPage(unsigned page);
    unsigned GetPage() const {return m_page;}
    void SetPageBreaks(bool pageBreaks);
    bool GetPageBreaks() const {return m_pageBreaks;}
    void SetFallbackResolution(double d);
    double GetFallbackResolution() const {return m_fallback_resolution;}
    void SetPageSize(const XY &s);
    void SetDesigns(const std::map<std::string, Context> *, const MscError *);
    void SetCopyRightText(const char *);
    const std::map<std::string, Context> &GetDesigns() const {return GetMsc()->Designs;}
//Compilation
    bool IsCompiled() const {return compiled;}
    //true if something changed
	bool CompileIfNeeded() const;

//Error related
	unsigned GetErrorNum(bool oWarnings) const;
    bool     IsErrorInFile(unsigned num, bool oWarnings) const;
	unsigned GetErrorLine(unsigned num, bool oWarnings) const;
	unsigned GetErrorCol(unsigned num, bool oWarnings) const;
	CString GetErrorText(unsigned num, bool oWarnings) const;
	CString GetDesignNames() const;
//Drawing related
	unsigned GetPages() const;
	CSize GetSize(unsigned forced_page=0) const;
    const Block &GetMscTotal() const;
	XY GetPageOrigin(unsigned page) const;
    Block GetNetPageBlock() const; //the page coordinates without autoheader and coyright text
    double GetHeadingSize() const;
	void DrawToFile(const char* fileName, bool bPageBreaks, double x_scale=1.0, double y_scale=1.0) const;
    /** Returns the size of the metafile or zero if error.*/
    size_t DrawToDC(Canvas::EOutputType ot, HDC hdc, const XY &scale,
                  unsigned page, bool bPageBreaks,
                  double fallback_image_resolution=-1, 
                  bool generateErrors=false) const
    {return GetMsc()->DrawToDC(ot, hdc, scale, page, bPageBreaks, fallback_image_resolution, generateErrors);}
    void DrawToMemDC(CDC &memDC, double x_scale, double y_scale, const CRect &clip, bool bPageBreaks);

    size_t GetWMFSize() const {_ASSERT(m_cacheType==CACHE_EMF && m_cache_EMF); return m_wmf_size;}
    const Contour &GetWMFFallbackImagePos() const {_ASSERT(m_cacheType==CACHE_EMF && m_cache_EMF); return m_fallback_image_places;}
//Cover related
	Element *GetArcByCoordinate(CPoint point) const;
	Element *GetArcByLine(unsigned line, unsigned col) const;
};

typedef std::list<CChartData>::iterator IChartData;
typedef std::list<CChartData>::const_iterator IChartData_const;

