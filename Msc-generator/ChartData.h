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
#include <vector>
#include <list>
#include "csh.h"

void RemoveCRLF(CString &str);
void EnsureCRLF(CString &str);

class Msc;
enum Msc_DrawType {DRAW_DIRECT=0, DRAW_EMF=1, DRAW_WMF=2};

class CChartData {
protected:
	CString m_text;
	mutable Msc *m_msc;
	mutable Msc *m_msc_for_csh;
	//References are to the MscGenDoc members
	CString m_ForcedDesign;
	Msc *GetMsc() const {CompileIfNeeded(); return m_msc;}
public:
	CChartData() : m_msc(NULL), m_msc_for_csh(NULL) {};
	CChartData(const char *text) : m_msc(NULL), m_msc_for_csh(NULL), m_text(text?text:"") {}
	CChartData(const char *text, const char *design) : m_msc(NULL), m_msc_for_csh(NULL), m_text(text?text:""), m_ForcedDesign(design?design:"") {}
	CChartData(const CChartData&o) : m_msc(NULL), m_msc_for_csh(NULL) {operator=(o);}
	~CChartData() {Delete();}
	CChartData & operator = (const CChartData&);
	void Delete(void) {FreeMsc(); FreeCsh(); m_text = "";}
	BOOL Save(const CString &fileName) const;
	BOOL Load(const CString &fileName,  BOOL reportError=TRUE);
	void Set(const char *text) {Delete(); m_text=text?text:"";}
	void SetDesign (const char *design);
	CString GetDesign () {return m_ForcedDesign;}
	const CString &GetText() const {return m_text;}
//Compilation related
	void FreeMsc() const {if (m_msc) {delete m_msc; m_msc=NULL;}}
	void CompileIfNeeded() const;
	void Recompile() const {FreeMsc(); CompileIfNeeded();}
	BOOL IsEmpty() const {return m_text.GetLength()==0;}
	BOOL IsCompiled() const {return m_msc!=NULL;}
//Error related
	unsigned GetErrorNum(bool oWarnings) const;
	unsigned GetErrorLine(unsigned num, bool oWarnings) const;
	unsigned GetErrorCol(unsigned num, bool oWarnings) const;
	CString GetErrorText(unsigned num, bool oWarnings) const;
	CString GetDesigns() const;
//Drawing related
	unsigned GetPages() const;
	CSize GetSize(unsigned page) const;
	void Draw(HDC hdc, Msc_DrawType type, double zoom, unsigned page, bool pageBreaks);
	void Draw(const char* fileName);
//Cover related
	void *GetArcByCoordinate(CPoint point) const;
	void *GetArcByLine(unsigned line, unsigned col) const;
	bool GetLineByArc(void*arc, unsigned &start_line, unsigned &start_col, unsigned &end_line, unsigned &end_col) const;
	unsigned GetCoversByArc(void *arc, RECT *result, int max_size, double xScale, double yScale) const;
//Color Syntax Highlighting related
	const MscCshListType &GetCsh() const;
	void FreeCsh() const {if (m_msc_for_csh) {delete m_msc_for_csh; m_msc_for_csh=NULL;}}
};

typedef std::list<CChartData>::iterator IChartData;