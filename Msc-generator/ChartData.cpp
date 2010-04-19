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

#include "stdafx.h"
#include "ChartData.h"
#include "msc.h"

//CChartData

CChartData::CChartData(bool &Pedantic, CString &ChartSourcePreamble, CString &CopyrightText) :
    m_Pedantic(Pedantic), m_ChartSourcePreamble(ChartSourcePreamble), m_CopyrightText(CopyrightText)
{
	m_buff=NULL; 
	m_length=0; 	
	m_msc = NULL;
}


CChartData::CChartData(const CChartData& other) :
     m_Pedantic(other.m_Pedantic), m_ChartSourcePreamble(other.m_ChartSourcePreamble), m_CopyrightText(other.m_CopyrightText)
{
	m_buff = NULL;
	m_length = 0;
	m_msc = NULL;
	operator =(other);
}

CChartData & CChartData::operator = (const CChartData& other)
{
	Delete();
	m_ForcedDesign = other.m_ForcedDesign;
	if (other.IsEmpty()) 
		return *this;
	m_buff = (char*)malloc(other.m_length);
	if (m_buff==NULL) 
		return *this;
	m_length = other.m_length;
	memcpy(m_buff, other.m_buff, m_length);
	return *this;
}

void CChartData::Delete(void) 
{
	FreeMsc();
	if (m_buff) free(m_buff); 
	m_buff=NULL; 
	m_length=0; 
}

BOOL CChartData::Set(const char *text, unsigned len)
{
	char *buff=NULL;
	if (len && text) {
		buff = (char*)malloc(len);
		if (!buff) return false;
	}
	Delete();
	m_buff = buff;
	m_length = text?len:0;
	if (len && text)
		memcpy(m_buff, text, len);
	return true;
}

void CChartData::SetDesign (const char *design)
{
	if (!design) return;
	if (m_ForcedDesign == design) return;
	FreeMsc();
	m_ForcedDesign = design;
}

BOOL CChartData::Save(const CString &fileName) const
{
	TRY {
		CStdioFile outfile(fileName, CFile::modeCreate | CFile::modeWrite | CFile::typeText);
		TRY {
			if (m_buff)
				outfile.Write(m_buff, m_length);
		} CATCH(CFileException, pEx) {
			pEx->ReportError();
			outfile.Close();
			return FALSE;
		}
		END_CATCH
		outfile.Close();
	} CATCH(CFileException, pEx) {
		pEx->ReportError();
		return FALSE;
	}
	END_CATCH
	return TRUE;
}



BOOL CChartData::Load(const CString &fileName, BOOL reportError)
{
	unsigned length = 0;
	char *buff = NULL;
	if (fileName.GetLength()>0) {
		CStdioFile infile;
		CFileException Ex;
		if (!infile.Open(fileName, CFile::modeRead | CFile::typeText, &Ex)) {
			if (reportError) Ex.ReportError();
			return FALSE;
		}
		length = infile.GetLength();
		if (length>0) {
			buff = (char*)malloc(length);
			TRY {
				length = infile.Read(buff, length);
			} CATCH(CFileException, pEx) {
				infile.Close();
				free(buff);
				if (reportError) pEx->ReportError();
				return FALSE;
			}
			END_CATCH
		}
		infile.Close();
	}
	Delete();
	m_buff = buff;
	m_length = length;
	return TRUE;
}

void CChartData::FreeMsc() const
{
	if (m_msc) {
		delete m_msc;
		m_msc = NULL;
	}
}

void CChartData::CompileIfNeeded() const 
{
	//To force a recompilation, call ReCompile()
	if (m_msc) return;
	m_msc = new Msc;
	if (!m_ChartSourcePreamble.IsEmpty()) {
		m_msc->pedantic = m_Pedantic;
		m_msc->ParseText(m_ChartSourcePreamble, m_ChartSourcePreamble.GetLength(), "[designlib]");
		if (!m_ForcedDesign.IsEmpty())
			if (m_msc->SetDesign((const char*)m_ForcedDesign, true)) 
				m_msc->ignore_designs = true;
	}
	m_msc->ParseText(m_buff, m_length, "");
	m_msc->copyrightText = (const char*)m_CopyrightText;
    
	//Allocate (non-sized) output object and assign it to the chart
    //From this point on, the chart sees xy dimensions
    m_msc->SetOutput(MscDrawer::WMF);
    //Sort Entities, add numbering, fill in auto-calculated values,
    //and throw warnings for badly constructed diagrams.
    m_msc->PostParseProcess();
    m_msc->CalculateWidthHeight();
    //A final step of prcessing, checking for additional drawing warnings
    m_msc->PostHeightProcess();
    m_msc->CloseOutput();
}