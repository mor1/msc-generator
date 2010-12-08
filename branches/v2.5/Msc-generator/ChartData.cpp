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
#include "csh.h"

void ReplaceTAB(CString &str, unsigned tabsize)
{
	CString spaces(' ', tabsize); 
	int pos = str.Find("\t");
	while (pos >= 0) {
		str.Delete(pos);
		str.Insert(pos, spaces);
		pos = str.Find("\t", pos);
	}
}


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

void CChartData::SetDesign (const char *design)
{
	if (!design) return;
	if (m_ForcedDesign == design) return;
	m_ForcedDesign = design;
}

BOOL CChartData::Save(const CString &fileName) 
{
	RemoveSpacesAtLineEnds();
	TRY {
		CStdioFile outfile(fileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
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
	if (fileName.GetLength() == 0) return FALSE;
	CStdioFile infile;
	CFileException Ex;
	if (!infile.Open(fileName, CFile::modeRead | CFile::typeText, &Ex)) {
		if (reportError) Ex.ReportError();
		return FALSE;
	}
	length = infile.GetLength();
	if (length>0) {
		buff = (char*)malloc(length+1);
		if (buff == NULL) return FALSE;
		TRY {
			length = infile.Read(buff, length);
		} CATCH(CFileException, pEx) {
			infile.Close();
			free(buff);
			if (reportError) pEx->ReportError();
			return FALSE;
		}
		END_CATCH
		buff[length] = 0;
	}
	infile.Close();
	Delete();
	m_text = buff;
	free(buff);
	RemoveSpacesAtLineEnds();
	EnsureCRLF(m_text);
	ReplaceTAB(m_text);
	return TRUE;
}

void CChartData::RemoveSpacesAtLineEnds()
{
	int pos = m_text.Find("\x0d");  //any \x0a comes *after* \x0d
	while (pos >= 0) {
		int spaces = 0;
		while (pos>spaces+1  && (m_text[pos-spaces-1] == ' ' || m_text[pos-1-spaces] == '\t')) spaces++;
		if (spaces) {
			m_text.Delete(pos-spaces, spaces);
			pos -= spaces;
		}
		pos = m_text.Find("\x0d", pos+1);
	}
}

void CDrawingChartData::SetDesign (const char *design)
{
	if (!design) return;
	if (GetDesign() == design) return;
	FreeMsc();
	m_ForcedDesign = design;
}

void CDrawingChartData::CompileIfNeeded(bool doEMF) const 
{
	if (doEMF != m_hemf_is_true_emf) FreeMsc();
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
	m_msc->CompleteParse(doEMF ? MscDrawer::WMF : MscDrawer::WMF, true);

	HDC hdc = CreateEnhMetaFile(NULL, NULL, NULL, NULL);
	Draw(hdc, doEMF, pApp->m_bPB_Editing);
	m_hemf = CloseEnhMetaFile(hdc);
	m_hemf_is_true_emf = doEMF;
}

unsigned CDrawingChartData::GetErrorNum(bool oWarnings) const {
	return GetMsc()->Error.GetErrorNum(oWarnings);
}

unsigned CDrawingChartData::GetErrorLine(unsigned num, bool oWarnings) const 
{
	return GetMsc()->Error.GetErrorLoc(num, oWarnings).line;
}

unsigned CDrawingChartData::GetErrorCol(unsigned num, bool oWarnings) const 
{
	return GetMsc()->Error.GetErrorLoc(num, oWarnings).col;
}

CString CDrawingChartData::GetErrorText(unsigned num, bool oWarnings) const 
{
	return CString(GetMsc()->Error.GetErrorText(num, oWarnings));
}

CString CDrawingChartData::GetDesigns() const 
{
	return CString(GetMsc()->GetDesigns().c_str());
}


unsigned CDrawingChartData::GetPages() const
{
    return GetMsc()->yPageStart.size();
}

CSize CDrawingChartData::GetSize() const
{
	XY offset, size;
	GetMsc()->GetPagePosition(int(m_page)-1, offset, size);
    return CSize(GetMsc()->totalWidth, size.y + GetMsc()->copyrightTextHeight);
}

double CDrawingChartData::GetPageYShift() const
{
	XY offset, size;
	GetMsc()->GetPagePosition(int(m_page)-1, offset, size);
    return offset.y;
}


double CDrawingChartData::GetBottomWithoutCopyright() const
{
	XY offset, size;
	GetMsc()->GetPagePosition(int(m_page)-1, offset, size);
    return size.y;
}

void CDrawingChartData::Draw(HDC hdc, bool doEMF, bool pageBreaks) const
{
	CompileIfNeeded(doEMF);
	MscDrawer::OutputType ot = doEMF ? MscDrawer::EMF : MscDrawer::WMF;
    if (!m_msc->SetOutputWin32(ot, hdc, int(m_page)-1)) return;
	//draw page breaks only if requested and not drawing a single page only
    m_msc->Draw(pageBreaks && m_page==0);
	m_msc->UnClip(); //Unclip the banner text exclusion clipped in SetOutputWin32()
	m_msc->DrawCopyrightText(int(m_page)-1);
    m_msc->CloseOutput();
}

void CDrawingChartData::Draw(const char* fileName) const
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

TrackableElement *CDrawingChartData::GetArcByCoordinate(CPoint point) const
{
	CompileIfNeeded();
	if (m_page>0)
		point.y += m_msc->yPageStart[m_page];
	const Block *block = InWhich(m_msc->AllCovers, XY(point.x, point.y));
	if (block==NULL) return NULL;
	return block->arc;
}

TrackableElement *CDrawingChartData::GetArcByLine(unsigned line, unsigned col) const
{
	CompileIfNeeded();
	file_line linenum(m_msc->Error.Files.size()-1, line, col);
	Msc::LineToArcMapType::const_iterator itr;
	//in the map linenum_ranges are sorted by increasing length, we search the shortest first
	for (itr = m_msc->AllArcs.begin(); itr!=m_msc->AllArcs.end(); itr++)
		if (itr->first.start <= linenum && linenum <= itr->first.end) break;
	if (itr == m_msc->AllArcs.end()) return NULL;
	return itr->second;
}

/*unsigned CDrawingChartData::GetCoversByArc(void *arc, int page_shown, TrackRect *result, int max_size, int &bottom_clip) const
{
	if (arc==NULL) return 0;
	CompileIfNeeded();
	XY size, offset;
    m_msc->GetPagePosition(page_shown, offset, size);
	unsigned count=0;
	for(std::set<Block>::const_iterator i=static_cast<ArcBase*>(arc)->GetGeometry().cover.begin(); 
		    i!=static_cast<ArcBase*>(arc)->GetGeometry().cover.end() && count<max_size; i++) {
		if (i->drawType == Block::DRAW_NONE) continue;
		//If it lies entirely off the page, do not generate a record
		if (i->y.from > offset.y + size.y) continue;
		if (i->y.till < offset.y) continue;

		result[count].r.left = i->x.from;
		result[count].r.right = i->x.till;
		result[count].r.top = i->y.from - offset.y; //shift tops by page start
		result[count].r.bottom = i->y.till - offset.y;
		result[count].frame_only = i->drawType == Block::DRAW_FRAME;
		count++;
	}
	bottom_clip = size.y;
	return count;
}
*/
