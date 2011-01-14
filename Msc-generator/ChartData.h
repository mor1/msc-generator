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
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/


#pragma once
#include <list>
#undef min
#undef max
#include "trackable.h"

void ReplaceTAB(CString &str, unsigned tabsize=6); //6 seems to be default for RichEditCtrl
void RemoveCRLF(CString &str);
void EnsureCRLF(CString &str);

class Msc;

class CChartData {
protected:
	CString m_text;
	CString m_ForcedDesign;
	unsigned m_page;
public:
	CHARRANGE m_sel;
	bool m_wasDrawn;
	CChartData() : m_page(0), m_wasDrawn(false) {m_sel.cpMax = m_sel.cpMin = 0;}
	CChartData(const char *text, const char *design=NULL, unsigned page = 0) 
		:m_text(text?text:""), m_ForcedDesign(design?design:""), m_page(page), 
		 m_wasDrawn(false) {m_sel.cpMax = m_sel.cpMin = 0;}
	CChartData(const char *text, const CHARRANGE &sel, const char *design=NULL, unsigned page = 0) 
		:m_text(text?text:""), m_sel(sel), m_ForcedDesign(design?design:""), m_page(page), m_wasDrawn(false) {}
	CChartData(const CChartData&o) {operator=(o);}
	virtual ~CChartData() {Delete();}
	virtual void Delete(void) {m_text.Empty(); m_wasDrawn = false; m_page=0; m_ForcedDesign.Empty(); m_sel.cpMax = m_sel.cpMin = 0;}
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
};

class CDrawingChartData : public CChartData {
protected:
	mutable Msc *m_msc;
	mutable HENHMETAFILE m_hemf;
	mutable bool m_hemf_is_true_emf;
	Msc *GetMsc() const {CompileIfNeeded(m_hemf_is_true_emf); return m_msc;}
public:
	CDrawingChartData() : m_msc(NULL), m_hemf(NULL), m_hemf_is_true_emf(true) {}
	CDrawingChartData(const CChartData&o) : m_msc(NULL), m_hemf(NULL), m_hemf_is_true_emf(true) {operator=(o);}
	CDrawingChartData & operator = (const CChartData& o) {FreeMsc(); CChartData::operator =(o); return *this;}
	virtual void Delete(void) {CChartData::Delete(); FreeMsc();}
	virtual void SetDesign (const char *design);
	virtual void SetPage(unsigned page) {if (m_page==page) return; m_page=page; if (m_msc) Recompile();}
	unsigned GetPage() const {return m_page;}
//Compilation related
	void FreeMsc() const {if (m_msc) {delete m_msc; m_msc=NULL; DeleteEnhMetaFile(m_hemf); m_hemf=NULL;}}
	void CompileIfNeeded(bool doEMF) const;
	void CompileIfNeeded() const {CompileIfNeeded(m_hemf_is_true_emf);}
	void Recompile() const {FreeMsc(); CompileIfNeeded();}
	BOOL IsCompiled() const {return m_msc!=NULL;}
	HENHMETAFILE GetEMF(bool trueEMF) const {CompileIfNeeded(trueEMF); return m_hemf;}
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
	void Draw(HDC hdc, bool doEMF, bool pageBreaks) const;
	void Draw(const char* fileName) const;
//Cover related
	TrackableElement *GetArcByCoordinate(CPoint point) const;
	TrackableElement *GetArcByLine(unsigned line, unsigned col) const;
	//unsigned GetCoversByArc(void *arc, int page_shown, TrackRect *result, int max_size, int &bottom_clip) const;
};

typedef std::list<CChartData>::iterator IChartData;
typedef std::list<CChartData>::const_iterator IChartData_const;