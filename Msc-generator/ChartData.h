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


#pragma once
#include <list>
#undef min
#undef max
#include "arcs.h"

void ReplaceTAB(CString &str, unsigned tabsize=6); //6 seems to be default for RichEditCtrl
void RemoveCRLF(CString &str);
void EnsureCRLF(CString &str);

class Msc;

class CChartData {
protected:
	CString m_text;
	CString m_ForcedDesign;
	unsigned m_page;
    EntityCollapseCatalog m_ForcedEntityCollapse; 
    ArcSignatureCatalog   m_ForcedArcCollapse; 
public:
    unsigned ver_a, ver_b, ver_c;
	CHARRANGE m_sel;
	bool m_wasDrawn;
	CChartData() : m_page(0), m_wasDrawn(false), ver_a(0), ver_b(0), ver_c(0) {m_sel.cpMax = m_sel.cpMin = 0;}
	CChartData(const char *text, const char *design=NULL, unsigned page = 0) 
		:m_text(text?text:""), m_ForcedDesign(design?design:""), m_page(page), 
		 m_wasDrawn(false), ver_a(0), ver_b(0), ver_c(0) {m_sel.cpMax = m_sel.cpMin = 0;}
	CChartData(const char *text, const CHARRANGE &sel, const char *design=NULL, unsigned page = 0) 
		:m_text(text?text:""), m_sel(sel), m_ForcedDesign(design?design:""), m_page(page), m_wasDrawn(false), 
        ver_a(0), ver_b(0), ver_c(0)  {}
	CChartData(const CChartData&o) {operator=(o);}
	virtual ~CChartData() {Delete();}
	virtual void Delete(void) {m_text.Empty(); m_wasDrawn = false; m_page=0; m_ForcedDesign.Empty(); m_sel.cpMax = m_sel.cpMin = 0;}
    void SetVersion(unsigned a, unsigned b, unsigned c) {ver_a=a; ver_b=b; ver_c=c;}
    bool HasVersion() const {return ver_a>0;}
    int CompareVersion(unsigned a, unsigned b, unsigned c) const;
	BOOL Save(const CString &fileName);
	BOOL Load(const CString &fileName,  BOOL reportError=TRUE);
	void Set(const char *text) {Delete(); m_text=text?text:"";}
	virtual void SetDesign (const char *design);
	CString GetDesign () {return m_ForcedDesign;}
	const CString &GetText() const {return m_text;}
	void RemoveSpacesAtLineEnds();
	BOOL IsEmpty() const {return m_text.GetLength()==0;}
	virtual void SetPage(unsigned page) {m_page=page;}
	unsigned GetPage() const {return m_page;}
    virtual bool ForceEntityCollapse(const std::string &s, bool b);
    virtual bool ForceEntityCollapse(const EntityCollapseCatalog&);
    const EntityCollapseCatalog &GetForcedEntityCollapse() const {return m_ForcedEntityCollapse;}
    virtual bool ForceArcCollapse(const ArcSignature &, BoxCollapseType t);
    virtual bool ForceArcCollapse(const ArcSignatureCatalog &);
    const ArcSignatureCatalog &GetForcedArcCollapse() const {return m_ForcedArcCollapse;}
};

class CDrawingChartData : public CChartData {
public:
protected:
    friend class CChartCache;
	mutable Msc         *m_msc;
	Msc *GetMsc() const {CompileIfNeeded(); return m_msc;}
public:
    CDrawingChartData() : m_msc(NULL) {}
	CDrawingChartData(const CChartData&o);
	CDrawingChartData(const CDrawingChartData&o);
	CDrawingChartData & operator = (const CChartData& o) {FreeMsc(); CChartData::operator =(o); return *this;}
	virtual void Delete(void) {CChartData::Delete(); FreeMsc();}
	virtual void SetDesign (const char *design);
	virtual void SetPage(unsigned page) {if (m_page==page) return; m_page=page; FreeMsc();}
	unsigned GetPage() const {return m_page;}
    bool ForceEntityCollapse(const std::string &s, bool b);
    bool ForceEntityCollapse(const EntityCollapseCatalog &);
    bool ForceArcCollapse(const ArcSignature &, BoxCollapseType t);
    bool ForceArcCollapse(const ArcSignatureCatalog &);
//Compilation related
    void FreeMsc() const;
	void CompileIfNeeded() const;
	void Recompile() const {FreeMsc(); CompileIfNeeded();}
	BOOL IsCompiled() const {return m_msc!=NULL;}
//Error related
	unsigned GetErrorNum(bool oWarnings) const;
    bool     IsErrorInFile(unsigned num, bool oWarnings) const;
	unsigned GetErrorLine(unsigned num, bool oWarnings) const;
	unsigned GetErrorCol(unsigned num, bool oWarnings) const;
	CString GetErrorText(unsigned num, bool oWarnings) const;
	CString GetDesigns() const;
//Drawing related
	unsigned GetPages() const;
	CSize GetSize() const;
	double GetPageYShift() const;
	double GetBottomWithoutCopyright() const;
    double GetHeadingSize() const;
    void DrawToWindow(HDC hdc, bool bPageBreaks, double x_scale=1.0, double y_scale=1.0) const;
	void DrawToMetafile(HDC hdc, bool isEMF, bool pageBreaks) const;
	void DrawToFile(const char* fileName, bool bPageBreaks, double x_scale=1.0, double y_scale=1.0) const;
//Cover related
	TrackableElement *GetArcByCoordinate(CPoint point) const;
	TrackableElement *GetArcByLine(unsigned line, unsigned col) const;
	//unsigned GetCoversByArc(void *arc, int page_shown, TrackRect *result, int max_size, int &bottom_clip) const;
};

typedef std::list<CChartData>::iterator IChartData;
typedef std::list<CChartData>::const_iterator IChartData_const;

class CChartCache 
{
public:
    typedef enum {CACHE_NONE, CACHE_EMF, CACHE_BMP} ECacheType;
protected:
    CDrawingChartData   *m_data;
    ECacheType   m_cacheType;
	HENHMETAFILE m_cache_EMF;
    CBitmap      m_cache_BMP;
    double       m_cache_BMP_x_scale;
    double       m_cache_BMP_y_scale;
    CRect        m_cache_BMP_clip;
public:
    CChartCache();
    void ClearCache();
    void SetData(CDrawingChartData *data) {ClearCache(); m_data = data;}
    const CDrawingChartData *GetChartData() const {return m_data;}
	void DrawToWindow(HDC hdc, double x_scale, double y_scale, const CRect &clip, bool bPageBreaks);
    void SetCacheType(ECacheType t) {if (m_cacheType!=t) {ClearCache(); m_cacheType=t;}}
};
