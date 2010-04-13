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


//CChartData

CChartData::CChartData(bool &Pedantic, CString &ChartSourcePreamble, CString &ChartSourcePostscript, CString &CopyrightText, CString &FileName) :
    m_Pedantic(Pedantic), m_ChartSourcePreamble(ChartSourcePreamble), m_ChartSourcePostscript(ChartSourcePostscript), 
	m_CopyrightText(CopyrightText), m_FileName(FileName)
{
	m_buff=NULL; 
	m_length=0; 	
	m_msc = NULL;
}


CChartData::CChartData(const CChartData& other) :
     m_Pedantic(other.m_Pedantic), m_ChartSourcePreamble(other.m_ChartSourcePreamble), m_ChartSourcePostscript(other.m_ChartSourcePostscript), 
	 m_CopyrightText(other.m_CopyrightText), m_FileName(other.m_FileName)
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
	if (m_buff) free(m_buff); 
	m_buff=NULL; 
	if (m_msc) Msc_Destroy(m_msc); 
	m_msc=NULL; 
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
	m_ForcedDesign = design;
	if (m_msc) {
		Msc_Destroy(m_msc); 
		m_msc = NULL;
	}
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
	if (m_msc) {
		Msc_Destroy(m_msc); 
		m_msc = NULL;
	}
	return TRUE;
}

void CChartData::CompileIfNeeded() const 
{
	//To force a recompilation, call ReCompile()
	if (m_msc) return;
	if (!m_ChartSourcePreamble.IsEmpty()) {
		m_msc = Msc_ParseText(m_ChartSourcePreamble, m_ChartSourcePreamble.GetLength(), "[designlib]", m_msc, m_Pedantic);
		if (!m_ForcedDesign.IsEmpty())
			Msc_ForceDesign(m_msc, m_ForcedDesign);
	}
	m_msc = Msc_ParseText(m_buff, m_length, m_FileName, m_msc, m_Pedantic);
	Msc_SetCopyrightText(m_msc, m_CopyrightText);
	if (!m_ChartSourcePostscript.IsEmpty()) 
		m_msc = Msc_ParseText(m_ChartSourcePostscript, m_ChartSourcePostscript.GetLength(), "[postscript]", m_msc, m_Pedantic);
	Msc_PostParse(m_msc);
}