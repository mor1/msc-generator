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

    You should have received a copy of the GNU Affero General Public License    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stdafx.h"
#include "Msc-generator.h"
#include "ChartData.h"
#include "msc.h"


void RemoveCRLF(CString &str)
{
	int pos = str.Find("\r");
	while (pos >= 0) {
		str.Delete(pos);
		pos = str.Find("\r", pos);
	}
}

void EnsureCRLF(CString &str)
{
	int pos = str.Find("\n");
	while (pos >= 0) {
		if (pos == 0 || str[pos-1]!='\r') {
			str.Insert(pos, "\r");
			pos++;
		}
		pos = str.Find("\n", pos+1);
	}
}


//CChartData

CChartData & CChartData::operator = (const CChartData& other)
{
	Delete();
	m_ForcedDesign = other.m_ForcedDesign;
	if (other.IsEmpty()) 
		return *this;
	m_text = other.m_text;
	return *this;
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
			if (m_text.GetLength()>0)
				outfile.Write(m_text, m_text.GetLength());
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
			buff = (char*)malloc(length+1);
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
	buff[length] = 0;
	Delete();
	m_text = buff;
	free(buff);
	return TRUE;
}

void CChartData::CompileIfNeeded() const 
{
	//To force a recompilation, call ReCompile()
	if (m_msc) return;
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp);
	m_msc = new Msc;
	if (pApp) 
		m_msc->pedantic = pApp->m_Pedantic;
	if (pApp && !pApp->m_ChartSourcePreamble.IsEmpty()) {
		m_msc->ParseText(pApp->m_ChartSourcePreamble, "[designlib]");
		if (!m_ForcedDesign.IsEmpty())
			if (m_msc->SetDesign((const char*)m_ForcedDesign, true)) 
				m_msc->ignore_designs = true;
	}
	m_msc->ParseText(m_text, "");
	if (pApp) 
		m_msc->copyrightText = (const char*)pApp->m_CopyrightText;
	//Do postparse, compile, calculate sizes and sort errors by line    
	m_msc->CompleteParse(MscDrawer::WMF, true);
}

unsigned CChartData::GetErrorNum(bool oWarnings) const {
	return GetMsc()->Error.GetErrorNum(oWarnings);
}

unsigned CChartData::GetErrorLine(unsigned num, bool oWarnings) const 
{
	return GetMsc()->Error.GetErrorLoc(num, oWarnings).line;
}

unsigned CChartData::GetErrorCol(unsigned num, bool oWarnings) const 
{
	return GetMsc()->Error.GetErrorLoc(num, oWarnings).col;
}

CString CChartData::GetErrorText(unsigned num, bool oWarnings) const 
{
	return CString(GetMsc()->Error.GetErrorText(num, oWarnings));
}

CString CChartData::GetDesigns() const 
{
	return CString(GetMsc()->GetDesigns().c_str());
}


unsigned CChartData::GetPages() const
{
	CompileIfNeeded();
    return m_msc->yPageStart.size();
}

CSize CChartData::GetSize(unsigned page) const
{
	CompileIfNeeded();
	XY offset, size;
	m_msc->GetPagePosition(int(page)-1, offset, size);
    return CSize(m_msc->totalWidth*m_msc->scale, size.y*m_msc->scale + m_msc->copyrightTextHeight*m_msc->scale);
}

void CChartData::Draw(HDC hdc, Msc_DrawType type, double zoom, unsigned page, bool pageBreaks)
{
	CompileIfNeeded();
	MscDrawer::OutputType ot;
	switch (type) {
	case ::DRAW_EMF: ot = MscDrawer::EMF; break;
	case ::DRAW_WMF: ot = MscDrawer::WMF; break;
	case ::DRAW_DIRECT: ot = MscDrawer::WIN; break;
        }
    if (!m_msc->SetOutputWin32(ot, hdc, zoom, int(page)-1))
        return;
	//draw page breaks only if requested and not drawing a single page only
    m_msc->Draw(pageBreaks && page==0);
	m_msc->UnClip(); //Unclip the banner text exclusion clipped in SetOutputWin32()
	m_msc->DrawCopyrightText(int(page)-1);
    m_msc->CloseOutput();
}

void CChartData::Draw(const char* fileName)
{
	CompileIfNeeded();
	string fn(fileName?fileName:"Untitled");
    size_t pos = fn.find_last_of('.');
    if (pos==string::npos) {
        pos = fn.length();
        fn += ".png";
    }
    string ext = fn.substr(pos+1);
    MscDrawer::OutputType ot;
    if (ext == "png") ot = MscDrawer::PNG;
    else if (ext == "png") ot = MscDrawer::PNG;
    else if (ext == "emf") ot = MscDrawer::EMF;
    else if (ext == "svg") ot = MscDrawer::SVG;
    else if (ext == "pdf") ot = MscDrawer::PDF;
    else if (ext == "eps") ot = MscDrawer::EPS;
    else {
        ot = MscDrawer::PNG;
        fn += ".png";
    }
	//Ignore useTextPaths
    m_msc->DrawToOutput(ot, fn);
}

void *CChartData::GetArcByCoordinate(CPoint point) const
{
	CompileIfNeeded();
	const Block *block = m_msc->AllCovers.InWhich(XY(point.x, point.y));
	if (block==NULL) return NULL;
	return block->arc;
}

void *CChartData::GetArcByLine(unsigned line, unsigned col) const
{
	CompileIfNeeded();
	file_line linenum(m_msc->Error.Files.size()-1, line, col);
	std::map<file_line, ArcBase*>::const_iterator itr = --m_msc->AllArcs.upper_bound(linenum);
	if (itr == m_msc->AllArcs.end()) return NULL;
	//OK now itr points to the arc with start pos just before the cursor
	//Now see that the end pos of the arc is after the cursor
	if (itr->second->line_end.line<line) return NULL;
	if (itr->second->line_end.line==line && itr->second->line_end.col < col)  return NULL;
	return  itr->second;
}


bool CChartData::GetLineByArc(void*arc, unsigned &start_line, unsigned &start_col, unsigned &end_line, unsigned &end_col) const
{
	if (arc==NULL) return false;
	CompileIfNeeded();
	start_line = static_cast<ArcBase*>(arc)->line_start.line;
	start_col = static_cast<ArcBase*>(arc)->line_start.col;
	end_line = static_cast<ArcBase*>(arc)->line_end.line;
	end_col = static_cast<ArcBase*>(arc)->line_end.col;
	return true;
}

unsigned CChartData::GetCoversByArc(void *arc, TrackRect *result, int max_size, double xScale, double yScale) const
{
	if (arc==NULL) return false;
	CompileIfNeeded();
	unsigned count=0;
	for(std::set<Block>::const_iterator i=static_cast<ArcBase*>(arc)->GetGeometry().cover.begin(); 
		    i!=static_cast<ArcBase*>(arc)->GetGeometry().cover.end() && count<max_size; i++) {
		if (i->drawType == Block::DRAW_NONE) continue;
		result[count].r.left = i->x.from * xScale;
		result[count].r.right = i->x.till * xScale;
		result[count].r.top = i->y.from * yScale;
		result[count].r.bottom = i->y.till * yScale;
		result[count].frame_only = i->drawType == Block::DRAW_FRAME;
		count++;
	}
	return count;
}

const MscCshListType &CChartData::GetCsh() const
{
	if (!m_msc_for_csh) {
		m_msc_for_csh = new Msc;
		m_msc_for_csh->ParseForCSH(m_text, m_text.GetLength());
	}
	return m_msc_for_csh->CshList;
}
