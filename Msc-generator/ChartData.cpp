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

    You should have received a copy of the GNU Affero General Public License    along with Msc-generator.  If not, see <http://www.gnu.org/licenses/>.
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

CDrawingChartData::CDrawingChartData(const CChartData&o) : m_msc(NULL), m_bPageBreaks(false)
{
    operator=(o);
}

CDrawingChartData::CDrawingChartData(const CDrawingChartData&o) : m_msc(NULL), m_bPageBreaks(m_bPageBreaks)
{
    operator=(static_cast<const CChartData&>(o));
}


void CDrawingChartData::SetDesign (const char *design)
{
	if (!design) return;
	if (GetDesign() == design) return;
	FreeMsc();
	m_ForcedDesign = design;
}

void CDrawingChartData::FreeMsc() const 
{
    if (m_msc) {
        delete m_msc; 
        m_msc=NULL; 
    }
}


void CDrawingChartData::CompileIfNeeded() const 
{
	//To force a recompilation, call ReCompile()
	if (!m_msc) {
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
	    m_msc->CompleteParse(MscCanvas::EMF, true);
    }
}

unsigned CDrawingChartData::GetErrorNum(bool oWarnings) const {
	return GetMsc()->Error.GetErrorNum(oWarnings);
}

bool CDrawingChartData::IsErrorInFile(unsigned num, bool oWarnings) const 
{
    if (GetErrorNum(oWarnings)>num && num>=0)
        return GetMsc()->Error.GetErrorLoc(num, oWarnings).file == GetMsc()->Error.Files.size()-1;
    else
        return false;
}

unsigned CDrawingChartData::GetErrorLine(unsigned num, bool oWarnings) const 
{
    if (GetErrorNum(oWarnings)>num && num>=0 )
        return GetMsc()->Error.GetErrorLoc(num, oWarnings).line;
    else
        return 0;
}

unsigned CDrawingChartData::GetErrorCol(unsigned num, bool oWarnings) const 
{
    if (GetErrorNum(oWarnings)>num && num>=0)
        return GetMsc()->Error.GetErrorLoc(num, oWarnings).col;
    return 0;
}

CString CDrawingChartData::GetErrorText(unsigned num, bool oWarnings) const 
{
    if (GetErrorNum(oWarnings)>num && num>=0)
        return CString(GetMsc()->Error.GetErrorText(num, oWarnings));
    else 
        return "";
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
    return CSize(GetMsc()->total.x, size.y + GetMsc()->copyrightTextHeight);
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


double CDrawingChartData::GetHeadingSize() const
{
    return GetMsc()->headingSize;
}

void CDrawingChartData::DrawToWindow(HDC hdc, double x_scale, double y_scale, const CRect &clip) const
{
    CompileIfNeeded();
    if (m_msc->SetOutputWin32(MscCanvas::WIN, hdc, x_scale, y_scale, int(m_page)-1)) {
        //draw page breaks only if requested and not drawing a single page only
        m_msc->Draw(m_bPageBreaks && m_page==0);
        m_msc->PrepareForCopyrightText(); //Unclip the banner text exclusion clipped in SetOutputWin32()
        m_msc->DrawCopyrightText(int(m_page)-1);
        //MscLineAttr line(LINE_SOLID, MscColorType(0,0,255), 4, CORNER_NONE, 0);
        //m_msc->Line(XY(0,0), XY(GetSize().cx*scale, GetSize().cy*scale), line);
        m_msc->CloseOutput();
    }

}

void CDrawingChartData::DrawToWMF(HDC hdc, bool pageBreaks) const
{
    if (!GetMsc()->SetOutputWin32(MscCanvas::WMF, hdc, 1.0, 1.0, int(m_page)-1)) return;
	//draw page breaks only if requested and not drawing a single page only
    m_msc->Draw(pageBreaks && m_page==0);
	m_msc->PrepareForCopyrightText(); //Unclip the banner text exclusion clipped in SetOutputWin32()
	m_msc->DrawCopyrightText(int(m_page)-1);
    m_msc->CloseOutput();
}

void CDrawingChartData::DrawToFile(const char* fileName, double x_scale, double y_scale) const
{
	string fn(fileName?fileName:"Untitled");
    size_t pos = fn.find_last_of('.');
    if (pos==string::npos) {
        pos = fn.length();
        fn += ".png";
    }
    CString ext = fn.substr(pos+1).c_str();
    MscCanvas::OutputType ot;
    if (ext.CompareNoCase("png")==0) ot = MscCanvas::PNG;
    else if (ext.CompareNoCase("png")==0) ot = MscCanvas::PNG;
    else if (ext.CompareNoCase("emf")==0) ot = MscCanvas::EMF;
    else if (ext.CompareNoCase("svg")==0) ot = MscCanvas::SVG;
    else if (ext.CompareNoCase("pdf")==0) ot = MscCanvas::PDF;
    else if (ext.CompareNoCase("eps")==0) ot = MscCanvas::EPS;
    else {
        ot = MscCanvas::PNG;
        fn += ".png";
    }
	//Ignore useTextPaths
    GetMsc()->DrawToOutput(ot, x_scale, y_scale, fn);
}

TrackableElement *CDrawingChartData::GetArcByCoordinate(CPoint point) const
{
	CompileIfNeeded();
	if (m_page>0)
		point.y += m_msc->yPageStart[m_page];
    const Area *area = m_msc->AllCovers.InWhichFromBack(XY(point.x, point.y));
	if (area==NULL) return NULL;
	return area->arc;
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

CChartCache::CChartCache() :
m_cacheType(CACHE_EMF), m_cache_EMF(NULL), m_cache_BMP_x_scale(0), m_cache_BMP_y_scale(0)
{
}

void CChartCache::ClearCache() 
{
    if (m_cache_EMF) {
        DeleteEnhMetaFile(m_cache_EMF);
        m_cache_EMF = NULL;
    }
    if (m_cache_BMP_x_scale) {
        m_cache_BMP_x_scale = 0;
        m_cache_BMP.DeleteObject();
    }
}

void CChartCache::DrawToWindow(HDC hdc, double x_scale, double y_scale, const CRect &clip) 
{
    if (!m_data) return;
    m_data->CompileIfNeeded();
    if (!m_data->m_msc) return;
    switch (m_cacheType) {
    case CACHE_NONE:
        if (m_data->m_msc->SetOutputWin32(MscCanvas::WIN, hdc, x_scale, y_scale, int(m_data->m_page)-1)) {
            //draw page breaks only if requested and not drawing a single page only
            m_data->m_msc->Draw(m_data->m_bPageBreaks && m_data->m_page==0);
            m_data->m_msc->PrepareForCopyrightText(); //Unclip the banner text exclusion clipped in SetOutputWin32()
            m_data->m_msc->DrawCopyrightText(int(m_data->m_page)-1);
            //MscLineAttr line(LINE_SOLID, MscColorType(0,0,255), 4, CORNER_NONE, 0);
            //m_msc->Line(XY(0,0), XY(GetSize().cx*scale, GetSize().cy*scale), line);
            m_data->m_msc->CloseOutput();
        }
        break;
    case CACHE_BMP:
        //GetClipBox(hdc, &clip);
        if (m_cache_BMP_x_scale != x_scale || m_cache_BMP_y_scale != y_scale || m_cache_BMP_clip != clip) {
            //Cache not ok, regenerate
            if (m_cache_BMP_x_scale) 
                m_cache_BMP.DeleteObject(); //Delete old cached object
            m_cache_BMP_clip = clip;
            m_cache_BMP_x_scale = x_scale;
            m_cache_BMP_y_scale = y_scale;
            CDC targetDC;
            targetDC.Attach(hdc);
	        CDC memDC;
	        memDC.CreateCompatibleDC(&targetDC);
            m_cache_BMP.CreateCompatibleBitmap(&targetDC, clip.Width(), clip.Height());
            CBitmap *oldBitmap = memDC.SelectObject(&m_cache_BMP);
            memDC.SetWindowOrg(clip.left, clip.top);
            memDC.FillSolidRect(clip, targetDC.GetBkColor());
            if (m_data->m_msc->SetOutputWin32(MscCanvas::WIN, memDC.m_hDC, x_scale, y_scale, int(m_data->m_page)-1)) {
                //draw page breaks only if requested and not drawing a single page only
                m_data->m_msc->Draw(m_data->m_bPageBreaks && m_data->m_page==0);
                m_data->m_msc->PrepareForCopyrightText(); //Unclip the banner text exclusion clipped in SetOutputWin32()
                m_data->m_msc->DrawCopyrightText(int(m_data->m_page)-1);
                m_data->m_msc->CloseOutput();
            }
            targetDC.BitBlt(clip.left, clip.top, clip.Width(), clip.Height(), &memDC, clip.left, clip.top, SRCCOPY);   
            memDC.SelectObject(oldBitmap); //select our cached bitmap out from the temp context
            targetDC.Detach();
        } else {
            //Copy cache to target
            CDC targetDC;
            targetDC.Attach(hdc);
	        CDC memDC;
	        memDC.CreateCompatibleDC(&targetDC);
	        CBitmap *oldBitmap = memDC.SelectObject(&m_cache_BMP);
            targetDC.BitBlt(clip.left, clip.top, clip.Width(), clip.Height(), &memDC, 0, 0, SRCCOPY);   
            memDC.SelectObject(oldBitmap); //select our cached bitmap out from the temp context
            targetDC.Detach();
        }
        break;
    case CACHE_EMF:
        if (!m_cache_EMF) {
            //cache not OK, regenerate
            HDC hdc = CreateEnhMetaFile(NULL, NULL, NULL, NULL);
            if (!m_data->m_msc->SetOutputWin32(MscCanvas::EMF, hdc, 1., 1., int(m_data->m_page)-1)) return;
            //draw page breaks only if requested and not drawing a single page only
            m_data->m_msc->Draw(m_data->m_bPageBreaks && m_data->m_page==0);
            m_data->m_msc->PrepareForCopyrightText(); //Unclip the banner text exclusion clipped in SetOutputWin32()
            m_data->m_msc->DrawCopyrightText(int(m_data->m_page)-1);
            m_data->m_msc->CloseOutput();
            m_cache_EMF = CloseEnhMetaFile(hdc);
        }
        CRect full(0,0, m_data->GetSize().cx*x_scale, m_data->GetSize().cy*y_scale);
        PlayEnhMetaFile(hdc, m_cache_EMF, &full);
        break;
    }
}
