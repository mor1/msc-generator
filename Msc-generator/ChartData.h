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

class Msc;
enum Msc_DrawType {DRAW_DIRECT=0, DRAW_EMF=1, DRAW_WMF=2};

class CChartData {
protected:
	char *m_buff;
	size_t m_length;
	mutable Msc *m_msc;
	//References are to the MscGenDoc members
	CString &m_ChartSourcePreamble;
	CString &m_CopyrightText;
	bool    &m_Pedantic;
	CString m_ForcedDesign;
	Msc *GetMsc() const {CompileIfNeeded(); return m_msc;}
public:
	CChartData(bool &Pedantic, CString &ChartSourcePreamble, CString &CopyrightText);
	CChartData(const CChartData&);
	~CChartData() {Delete();}
	CChartData & operator = (const CChartData&);
	void Delete(void); 
	BOOL Save(const CString &fileName) const;
	BOOL Load(const CString &fileName,  BOOL reportError=TRUE);
	BOOL Set(const char *text, unsigned len);
	void SetDesign (const char *design);
	CString GetDesign () {return m_ForcedDesign;}
	const char *GetText() const {return m_buff;}
	unsigned GetLength() const {return m_length;}
	//Compilation related
	void FreeMsc() const;
	void CompileIfNeeded() const;
	void Recompile() const {FreeMsc(); CompileIfNeeded();}
	BOOL IsEmpty() const {return m_length==0 || m_buff==NULL;}
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
	void Draw(HDC hdc, Msc_DrawType type, double zoom, unsigned page);
	void Draw(const char* fileName);

};

typedef std::list<CChartData>::iterator IChartData;