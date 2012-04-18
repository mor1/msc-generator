/*
    This file is part of Msc-generator.
	Copyright 2008,2009,2010,2011,2012 Zoltan Turanyi
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
#include "version.h"

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

//rets -1 if "this" has newer version, 0 if equal, +1 if "this" has older
//If we have no version or the other is invalid, return 0
int CChartData::CompareVersion(unsigned a, unsigned b, unsigned c) const 
{
    if (!HasVersion() || ver_a==0) return 0;
    if (a<ver_a) return -1;
    if (a>ver_a) return +1;
    if (b<ver_b) return -1;
    if (b>ver_b) return +1;
    if (c<ver_c) return -1;
    if (c>ver_c) return +1;
    return 0;
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
	length = unsigned(infile.GetLength());
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
    ver_a = ver_b = ver_c = 0;
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

bool CChartData::ForceEntityCollapse(const std::string &s, bool b) 
{
    auto itr = m_ForcedEntityCollapse.find(s);
    bool changed = itr == m_ForcedEntityCollapse.end() || b != itr->second;
    if (changed) 
        m_ForcedEntityCollapse[s] = b; 
    return changed;
}

bool CChartData::ForceEntityCollapse(const EntityCollapseCatalog &o)
{
    bool changed = m_ForcedEntityCollapse == o;
    m_ForcedEntityCollapse = o;
    return changed;
}

bool CChartData::ForceArcCollapse(const ArcSignature &s, BoxCollapseType t) 
{
    auto itr = m_ForcedArcCollapse.find(s);
    bool changed = itr == m_ForcedArcCollapse.end() || t != itr->second;
    if (changed) 
        m_ForcedArcCollapse[s] = t; 
    return changed;
}

bool CChartData::ForceArcCollapse(const ArcSignatureCatalog &o)
{
    if (m_ForcedArcCollapse == o) return false;
    m_ForcedArcCollapse = o;
    return true;
}

CDrawingChartData::CDrawingChartData(const CChartData&o) : m_msc(NULL)
{
    operator=(o);
}

CDrawingChartData::CDrawingChartData(const CDrawingChartData&o) : m_msc(NULL)
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

bool CDrawingChartData::ForceEntityCollapse(const std::string &s, bool b) 
{
	if (CChartData::ForceEntityCollapse(s, b)) {
        FreeMsc();
        return true;
    }
    return false;
}

bool CDrawingChartData::ForceEntityCollapse(const std::map<std::string,bool> &o)
{
	if (CChartData::ForceEntityCollapse(o)) {
        FreeMsc();
        return true;
    }
    return false;
}

bool CDrawingChartData::ForceArcCollapse(const ArcSignature &s, BoxCollapseType t) 
{
	if (CChartData::ForceArcCollapse(s, t)) {
        FreeMsc();
        return true;
    }
    return false;
}

bool CDrawingChartData::ForceArcCollapse(const ArcSignatureCatalog &o)
{
	if (CChartData::ForceArcCollapse(o)) {
        FreeMsc();
        return true;
    }
    return false;
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
        //copy pedantic flag from app settings
	    if (pApp) 
		    m_msc->pedantic = pApp->m_Pedantic;
        if (HasVersion() && CompareVersion(LIBMSCGEN_MAJOR, LIBMSCGEN_MINOR, LIBMSCGEN_SUPERMINOR)<0) {
            string msg = "This chart was created with Msc-Generator v";
            msg << ver_a << "." << ver_b;
            if (ver_c) msg << "." << ver_c;
            msg << ", which is newer than the current version. Please update Msc-generator, see Help|About...";
            m_msc->Error.Warning(file_line(0, 0, 0), msg);
        }
        //compile preamble and set forced design
	    if (pApp && !pApp->m_ChartSourcePreamble.IsEmpty()) {
		    m_msc->ParseText(pApp->m_ChartSourcePreamble, "[designlib]");
		    if (!m_ForcedDesign.IsEmpty()) {
                ArcBase *ret;
			    if (m_msc->SetDesign(true, (const char*)m_ForcedDesign, true, &ret)) 
				    m_msc->ignore_designs = true;
                m_msc->Arcs.Append(ret);
            }
	    }
        //copy forced collapse/expand entities/boxes
        m_msc->force_entity_collapse = m_ForcedEntityCollapse;
        m_msc->force_box_collapse = m_ForcedArcCollapse;

        //parse chart text
	    m_msc->ParseText(m_text, "");
        //set copyright text
	    if (pApp) 
		    m_msc->copyrightText = (const char*)pApp->m_CopyrightText;
	    //Do postparse, compile, calculate sizes and sort errors by line
	    m_msc->CompleteParse(MscCanvas::EMF, true);
        //See which of the forced entity/box collapse directives remained
        //ones with no entity/box or equal state as chart were removed in Msc::PostParseProcess and
        //EntityDef::AddAttributeList
        //cast away constness, this is merely to reinterpret values
        const_cast<EntityCollapseCatalog&>(m_ForcedEntityCollapse) = m_msc->force_entity_collapse;
        const_cast<ArcSignatureCatalog&>(m_ForcedArcCollapse) = m_msc->force_box_collapse_instead;
    }
}

unsigned CDrawingChartData::GetErrorNum(bool oWarnings) const {
	return GetMsc()->Error.GetErrorNum(oWarnings);
}

bool CDrawingChartData::IsErrorInFile(unsigned num, bool oWarnings) const 
{
    if (GetErrorNum(oWarnings)>num && num>=0)
        return GetMsc()->Error.GetErrorLoc(num, oWarnings).file == int(GetMsc()->Error.Files.size())-1;
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
	return CString(GetMsc()->GetDesigns(true).c_str());
}


unsigned CDrawingChartData::GetPages() const
{
    return GetMsc()->yPageStart.size();
}

//if force_page==false, return the size of m_page (or the entire chart if m_page==0)
//else that of forced_page

//In CDrawingChartData we hide that the Msc chart does not originate in (0,0) by 
//shifting all coordinates.
CSize CDrawingChartData::GetSize(bool force_page, unsigned forced_page) const
{
    const unsigned page_to_measure = force_page ? forced_page : m_page;
    const Msc &msc = *GetMsc();
    CSize ret(int(msc.GetTotal().x.Spans()), int(msc.copyrightTextHeight));
    if (page_to_measure==0) 
        ret.cy = int(msc.GetTotal().y.Spans() + msc.copyrightTextHeight);
    else if (page_to_measure < msc.yPageStart.size()) 
        ret.cy = int(msc.yPageStart[page_to_measure] - msc.yPageStart[page_to_measure-1] + msc.copyrightTextHeight);
    else if (page_to_measure == msc.yPageStart.size()) 
        ret.cy = int(msc.GetTotal().y.till - msc.yPageStart[page_to_measure-1] + msc.copyrightTextHeight);
    return ret;
}

const Block &CDrawingChartData::GetMscTotal() const 
{
    return GetMsc()->GetTotal();
}


double CDrawingChartData::GetPageYShift() const
{
    const Msc &msc = *GetMsc();
    if (m_page==0) return 0;
    if (m_page <= msc.yPageStart.size()) return msc.yPageStart[m_page-1]-msc.GetTotal().y.from;
    return msc.GetTotal().y.Spans();
}


double CDrawingChartData::GetBottomWithoutCopyright() const
{
    return GetSize().cy - GetMsc()->copyrightTextHeight;
}


double CDrawingChartData::GetHeadingSize() const
{
    return GetMsc()->headingSize - GetMsc()->GetTotal().x.from;
}

void CDrawingChartData::DrawToWindow(HDC hdc, bool bPageBreaks, double x_scale, double y_scale) const
{
    MscCanvas canvas(MscCanvas::WIN, hdc, GetMsc()->GetTotal(), GetMsc()->copyrightTextHeight, 
                     XY(x_scale, y_scale), &GetMsc()->yPageStart, m_page);
    if (canvas.Status()==MscCanvas::ERR_OK) {
        //draw page breaks only if requested and not drawing a single page only
        m_msc->Draw(canvas, bPageBreaks && m_page==0);
        canvas.PrepareForCopyrightText(); //Unclip the banner text exclusion clipped in SetOutputWin32()
        m_msc->DrawCopyrightText(canvas, m_page);
    }
}

//here force_page==0 means we do not force a particular page, use m_page
void CDrawingChartData::DrawToMetafile(HDC hdc, bool isEMF, bool pageBreaks, bool force_page, unsigned forced_page) const
{
    const unsigned page_to_draw = force_page ? forced_page : m_page;
    MscCanvas canvas(isEMF ? MscCanvas::EMF : MscCanvas::WMF, hdc, GetMsc()->GetTotal(), GetMsc()->copyrightTextHeight, 
                     XY(1., 1.), &GetMsc()->yPageStart, page_to_draw);
    if (canvas.Status()==MscCanvas::ERR_OK) {
        //draw page breaks only if requested and not drawing a single page only
        m_msc->Draw(canvas, pageBreaks && page_to_draw==0);
        canvas.PrepareForCopyrightText(); //Unclip the banner text exclusion clipped in SetOutputWin32()
        m_msc->DrawCopyrightText(canvas, page_to_draw);
    }
}

void CDrawingChartData::DrawToFile(const char* fileName, bool bPageBreaks, double x_scale, double y_scale) const
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
    else if (ext.CompareNoCase("wmf")==0) ot = MscCanvas::WMF;
    else {
        ot = MscCanvas::PNG;
        fn += ".png";
    }
	//Ignore useTextPaths
    GetMsc()->DrawToOutput(ot, XY(x_scale, y_scale), fn, bPageBreaks);
}

TrackableElement *CDrawingChartData::GetArcByCoordinate(CPoint point) const
{
	CompileIfNeeded();
	if (m_page>0)
		point.y += LONG(m_msc->yPageStart[m_page-1]);
    const XY point_msc(point.x + m_msc->GetTotal().x.from, point.y + m_msc->GetTotal().y.from);
    const Area *area = m_msc->AllCovers.InWhichFromBack(point_msc);
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

void CChartCache::DrawToWindow(HDC hdc, double x_scale, double y_scale, const CRect &clip, bool bPageBreaks) 
{
    if (!m_data) return;
    m_data->CompileIfNeeded();
    if (!m_data->m_msc) return;
    switch (m_cacheType) {
    case CACHE_NONE:
        m_data->DrawToWindow(hdc, x_scale, y_scale);
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
            m_data->DrawToWindow(memDC.m_hDC, x_scale, y_scale);
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
            m_data->DrawToMetafile(hdc, true, bPageBreaks);
            m_cache_EMF = CloseEnhMetaFile(hdc);
        }
        CRect full(0,0, int(m_data->GetSize().cx*x_scale), int(m_data->GetSize().cy*y_scale));
        PlayEnhMetaFile(hdc, m_cache_EMF, &full);
        break;
    }
}
