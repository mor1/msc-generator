/*
    This file is part of Msc-generator.
	Copyright 2008,2009,2010,2011,2012,2013 Zoltan Turanyi
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

void CChartData::swap(CChartData &o)
{
    std::swap(m_text, o.m_text);
    std::swap(m_ForcedDesign, o.m_ForcedDesign);
    m_ForcedEntityCollapse.swap(o.m_ForcedEntityCollapse);
    m_ForcedArcCollapse.swap(o.m_ForcedArcCollapse);
    std::swap(m_page_size, o.m_page_size);
    std::swap(m_addHeading, o.m_addHeading);
    std::swap(m_fitWidth, o.m_fitWidth);
    std::swap(ver_a, o.ver_a);
    std::swap(ver_b, o.ver_b);
    std::swap(ver_c, o.ver_c);
    std::swap(m_sel, o.m_sel);
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

CDrawingChartData::CDrawingChartData(const CChartData&o) : 
    m_msc(NULL), compiled(false), m_cacheType(CACHE_RECORDING), m_cache_EMF(NULL), 
    m_cache_rec(NULL), m_cache_rec_full_no_pb(NULL), m_wmf_size(0),
    m_fallback_resolution(300), m_page(0), m_pageBreaks(false),
    m_pedantic(false), m_designs(NULL), m_design_errors(NULL),
    m_callback(NULL), m_callback_data(NULL)
{
    operator=(o);
}

CDrawingChartData::CDrawingChartData(const CDrawingChartData&o) : 
    CChartData(o),
    m_msc(NULL), compiled(false), m_cacheType(o.m_cacheType), m_cache_EMF(NULL), 
    m_cache_rec(NULL), m_cache_rec_full_no_pb(NULL), m_wmf_size(0),
    m_fallback_resolution(o.m_fallback_resolution), m_page(o.m_page), 
    m_pageBreaks(o.m_pageBreaks), m_pedantic(o.m_pedantic),
    m_designs(o.m_designs), m_design_errors(o.m_design_errors), m_copyright(o.m_copyright),
    m_callback(o.m_callback), m_callback_data(o.m_callback_data), m_load_data(o.m_load_data)
{
}

void CDrawingChartData::Delete(void) 
{
    Invalidate(); 
    CChartData::Delete();
    m_copyright.Empty();
    m_callback=NULL; 
    m_callback_data=NULL;
    m_load_data.Empty();
}


void CDrawingChartData::swap(CDrawingChartData &o)
{
    CChartData::swap(o);
    std::swap(m_msc, o.m_msc);
    std::swap(compiled, o.compiled);
    std::swap(m_cacheType, o.m_cacheType);
    std::swap(m_cache_EMF, o.m_cache_EMF);
    std::swap(m_cache_rec, o.m_cache_rec);
    std::swap(m_cache_rec_full_no_pb, o.m_cache_rec_full_no_pb);
    m_fallback_image_places.swap(o.m_fallback_image_places);
    std::swap(m_wmf_size, o.m_wmf_size);
    std::swap(m_fallback_resolution, o.m_fallback_resolution);
    std::swap(m_page, o.m_page);
    std::swap(m_pageBreaks, o.m_pageBreaks);
    std::swap(m_pedantic, o.m_pedantic);
    std::swap(m_designs, o.m_designs);
    std::swap(m_design_errors, o.m_design_errors);
    std::swap(m_copyright, o.m_copyright);
    std::swap(m_callback, o.m_callback);
    std::swap(m_callback_data, o.m_callback_data);
    std::swap(m_load_data, o.m_load_data);
}

void CDrawingChartData::ClearCache() const
{
    if (m_cache_EMF) {
        DeleteEnhMetaFile(m_cache_EMF);
        m_cache_EMF = NULL;
    }
    if (m_cache_rec) {
        cairo_surface_destroy(m_cache_rec);
        m_cache_rec = NULL;
    }
    if (m_cache_rec_full_no_pb) {
        cairo_surface_destroy(m_cache_rec_full_no_pb);
        m_cache_rec_full_no_pb = NULL;
    }
    m_wmf_size = 0;
    m_fallback_image_places.clear();
    compiled = false;
}

void CDrawingChartData::SetDesign (const char *design)
{
	if (!design) return;
	if (GetDesign() == design) return;
	Invalidate();
	m_ForcedDesign = design;
}

 void CDrawingChartData::SetPageSize(const XY &s)
{
    XY use = (s.y<0 || s.x<0) ? XY(0,0) : s;
	if (GetPageSize() == s) return;
	Invalidate();
	m_page_size = s;
}

void CDrawingChartData::SetAddHeading(bool b)
{
	if (GetAddHeading() == b) return;
	Invalidate();
	m_addHeading = b;
}

void CDrawingChartData::SetFitWidth(bool b)
{
	if (GetFitWidth() == b) return;
	Invalidate();
	m_fitWidth = b;
}

void CDrawingChartData::SetPedantic(bool b)
{
    if (m_pedantic == b) return;
	Invalidate();
    m_pedantic = b;
}

void CDrawingChartData::SetDesigns(const std::map<std::string, Context> *p, const MscError *e)
{
    if (m_designs == p && m_design_errors == e) return;
	Invalidate();
    m_designs = p;
    m_design_errors = e;
}

void CDrawingChartData::SetCopyRightText(const char *c)
{
	if (m_copyright== c) return;
	Invalidate();
	m_copyright = c;
}


void CDrawingChartData::InvalidatePage() const
{
    if (m_cacheType==CACHE_EMF) {
        ClearCache();
    } else {
        if (m_cache_rec) {
            cairo_surface_destroy(m_cache_rec);
            m_cache_rec = NULL;
        }
    }
}

void CDrawingChartData::SetPage(unsigned page)
{
    if (compiled) {
        if (GetPages()==1) page = 0;
        else page = std::min(page, GetPages());
    }
    if (page!=m_page) InvalidatePage();
    m_page = page;
}

void CDrawingChartData::SetPageBreaks(bool pageBreaks)
{
    if (pageBreaks == m_pageBreaks) return;
    m_pageBreaks = pageBreaks;
    if (m_page==0) InvalidatePage();
}

void CDrawingChartData::SetFallbackResolution(double d)
{
    if (m_fallback_resolution == d) return ;
    m_fallback_resolution = d;
    if (m_cacheType == CACHE_EMF) InvalidatePage();
}

bool CDrawingChartData::ForceEntityCollapse(const std::string &s, bool b) 
{
	if (CChartData::ForceEntityCollapse(s, b)) {
        Invalidate();
        return true;
    }
    return false;
}

bool CDrawingChartData::ForceEntityCollapse(const std::map<std::string,bool> &o)
{
	if (CChartData::ForceEntityCollapse(o)) {
        Invalidate();
        return true;
    }
    return false;
}

bool CDrawingChartData::ForceArcCollapse(const ArcSignature &s, BoxCollapseType t) 
{
	if (CChartData::ForceArcCollapse(s, t)) {
        Invalidate();
        return true;
    }
    return false;
}

bool CDrawingChartData::ForceArcCollapse(const ArcSignatureCatalog &o)
{
	if (CChartData::ForceArcCollapse(o)) {
        Invalidate();
        return true;
    }
    return false;
}

void CDrawingChartData::Invalidate() const
{
    if (m_msc) {
        delete m_msc; 
        m_msc=NULL; 
    }
    compiled = false;
    ClearCache();
}


bool CDrawingChartData::CompileIfNeeded() const
{
    const bool did_compilation = m_msc==NULL;
	if (!m_msc) {
	    m_msc = new Msc;
        m_msc->prepare_for_tracking = false;
        if (m_callback) {
            m_msc->Progress.callback = m_callback;
            m_msc->Progress.data = m_callback_data;   
            if (m_load_data.GetLength())
                m_msc->Progress.ReadLoadData(m_load_data);
        }
        //copy pedantic flag from app settings
        m_msc->pedantic = m_pedantic;
        if (HasVersion() && CompareVersion(LIBMSCGEN_MAJOR, LIBMSCGEN_MINOR, LIBMSCGEN_SUPERMINOR)<0) {
            string msg = "This chart was created with Msc-Generator v";
            msg << ver_a << "." << ver_b;
            if (ver_c) msg << "." << ver_c;
            msg << ", which is newer than the current version. Please update Msc-generator, see Help|About...";
            m_msc->Error.Warning(FileLineCol(0, 0, 0), msg);
        }
        //add designs
        if (m_designs) {
            m_msc->Designs = *m_designs;
            m_msc->Error = *m_design_errors; 
		    if (!m_ForcedDesign.IsEmpty()) {
                ArcBase *ret=NULL;
			    if (m_msc->SetDesign(true, (const char*)m_ForcedDesign, true, &ret)) 
                    m_msc->ignore_designs = true;
                if (ret)
                    m_msc->Arcs.Append(ret);
            }
        }
        //copy forced collapse/expand entities/boxes
        m_msc->force_entity_collapse = m_ForcedEntityCollapse;
        m_msc->force_box_collapse = m_ForcedArcCollapse;
        //parse chart text
        m_msc->Progress.StartSection(MscProgress::PARSE);
	    m_msc->ParseText(m_text, "");
        //set copyright text
        m_msc->copyrightText = m_copyright;
	    //Do postparse, compile, calculate sizes and sort errors by line
        m_msc->CompleteParse(Canvas::PNG, false, m_page_size.x>0 && m_page_size.y>0, 
                             m_addHeading, m_page_size, m_fitWidth);
        //See which of the forced entity/box collapse directives remained
        //ones with no entity/box or equal state as chart were removed in Msc::PostParseProcess and
        //EntityDef::AddAttributeList
        //cast away constness, this is merely to reinterpret values
        const_cast<EntityCollapseCatalog&>(m_ForcedEntityCollapse) = m_msc->force_entity_collapse;
        const_cast<ArcSignatureCatalog&>(m_ForcedArcCollapse) = m_msc->force_box_collapse_instead;
        ClearCache();
        if (m_page > m_msc->pageBreakData.size())
            const_cast<unsigned&>(m_page) = m_msc->pageBreakData.size();
    }
    bool did_redraw = did_compilation;
    //Now regenerate the cache
    if (m_cacheType==CACHE_EMF) {
        if (m_cache_EMF==NULL) {
            m_cache_EMF = m_msc->DrawToMetaFile(Canvas::EMFWMF, m_page, m_pageBreaks, 
                m_fallback_resolution, &m_wmf_size, &m_fallback_image_places);
            did_redraw = true;
        }
    } else if (!m_cache_rec) {
        if (m_page==0 && m_pageBreaks) {
            //If we draw all of it with page breaks, redraw it
            m_cache_rec = m_msc->DrawToRecordingSurface(Canvas::SVG, true);
            did_redraw = true;
        } else {
            //If not (no page breaks or just one page), reuse the full no pb member
            if (!m_cache_rec_full_no_pb) 
                m_cache_rec_full_no_pb = m_msc->DrawToRecordingSurface(Canvas::SVG, false);            
            m_cache_rec = m_msc->ReDrawOnePage(m_cache_rec_full_no_pb, m_page);
            did_redraw = true;
        }
    }
    compiled = true;
    if (did_compilation && m_callback && m_text.GetLength()>0) {
        m_load_data = m_msc->Progress.WriteLoadData().c_str();
    }
    return did_redraw;
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

CString CDrawingChartData::GetDesignNames() const 
{
	return CString(GetMsc()->GetDesignNames(true).c_str());
}


unsigned CDrawingChartData::GetPages() const
{
    return GetMsc()->pageBreakData.size();
}

//if force_page==false, return the size of m_page (or the entire chart if m_page==0)
//else that of forced_page

//In CDrawingChartData we hide that the Msc chart does not originate in (0,0) by 
//shifting all coordinates.
//The returned page here contains autoHeaders and copyRightText
CSize CDrawingChartData::GetSize(unsigned page) const
{
    const Msc &msc = *GetMsc();
    CSize ret(int(msc.GetTotal().x.Spans()), int(msc.copyrightTextHeight));
    if (page==0) 
        ret.cy = int(msc.GetTotal().y.Spans() + msc.copyrightTextHeight);
    else if (page < msc.pageBreakData.size()) 
        ret.cy = int(msc.pageBreakData[page].y - 
                     msc.pageBreakData[page-1].y + 
                     msc.pageBreakData[page-1].autoHeadingSize +
                     msc.copyrightTextHeight);
    else if (page == msc.pageBreakData.size()) 
        ret.cy = int(msc.GetTotal().y.till - 
                     msc.pageBreakData[page-1].y + 
                     msc.pageBreakData[page-1].autoHeadingSize +
                     msc.copyrightTextHeight);
    return ret;
}

const Block &CDrawingChartData::GetMscTotal() const 
{
    return GetMsc()->GetTotal();
}

XY CDrawingChartData::GetPageOrigin(unsigned page) const
{
    const Msc &msc = *GetMsc();
    XY ret(msc.GetTotal().x.from, 0);
    if (page<=1) 
        ret.y = msc.GetTotal().y.from;
    else 
        ret.y = msc.pageBreakData[page-1].y - 
                msc.pageBreakData[page-1].autoHeadingSize;
    return ret;
}


double CDrawingChartData::GetHeadingSize() const
{
    return GetMsc()->headingSize - GetMsc()->GetTotal().x.from;
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
    Canvas::EOutputType ot;
    if (ext.CompareNoCase("png")==0) ot = Canvas::PNG;
    else if (ext.CompareNoCase("png")==0) ot = Canvas::PNG;
    else if (ext.CompareNoCase("emf")==0) ot = Canvas::EMF;
    else if (ext.CompareNoCase("svg")==0) ot = Canvas::SVG;
    else if (ext.CompareNoCase("pdf")==0) ot = Canvas::PDF;
    else if (ext.CompareNoCase("eps")==0) ot = Canvas::EPS;
    else if (ext.CompareNoCase("wmf")==0) ot = Canvas::WMF;
    else {
        ot = Canvas::PNG;
        fn += ".png";
    }
    GetMsc()->DrawToFile(ot, std::vector<XY>(1, XY(x_scale, y_scale)), fn, bPageBreaks);
}

//This one expects point in chart coordinates
Element *CDrawingChartData::GetArcByCoordinate(CPoint point) const
{
	_ASSERT(m_msc);
    const Area *area = m_msc->AllCovers.InWhichFromBack(XY(point.x, point.y));
	if (area==NULL) return NULL;
	return area->arc;
}

Element *CDrawingChartData::GetArcByLine(unsigned line, unsigned col) const
{
	_ASSERT(m_msc);
	FileLineCol linenum(m_msc->Error.Files.size()-1, line, col);
	Msc::LineToArcMapType::const_iterator itr;
	//in the map linenum_ranges are sorted by increasing length, we search the shortest first
	for (itr = m_msc->AllArcs.begin(); itr!=m_msc->AllArcs.end(); itr++)
		if (itr->first.start <= linenum && linenum <= itr->first.end) break;
	if (itr == m_msc->AllArcs.end()) return NULL;
	return itr->second;
}


//Here the "memDC" contains a bitmap as large as the visible part of the screen
//"scale" contains the magnification factor we shall display the chart with
//"clip" contains the coordinates of the visible part of the screen in scaled chart coordinates.
//E.g., if the chart is 100x100, scale is 2, and we can see 80x80 pixels in the middle of the
//chart (a small window), then the scaled chart is 200x200 and clip will be (60x60)->(140x140)
void CDrawingChartData::DrawToMemDC(CDC &memDC, double x_scale, double y_scale, const CRect &clip, bool bPageBreaks) 
{
    _ASSERT(m_msc);
    if (!m_msc) return;
    switch (m_cacheType) {
    case CACHE_EMF: {
        if (!m_cache_EMF) break;
        const CSize size = GetSize();
        const CRect full(0,0, int(size.cx*x_scale), int(size.cy*y_scale));
        memDC.SaveDC();
        memDC.SetWindowOrg(clip.left, clip.top);
        memDC.PlayMetaFile(m_cache_EMF, &full);
        memDC.RestoreDC(1);
        break;
        }
    case CACHE_RECORDING: {
        if (!m_cache_rec) break;
        cairo_surface_t *surf = cairo_win32_surface_create(memDC.m_hDC);
        cairo_t *cr = cairo_create(surf);
        cairo_scale(cr, x_scale, y_scale);
        cairo_set_source_surface (cr, m_cache_rec, -clip.left/x_scale, -clip.top/y_scale);
        cairo_paint (cr);
        cairo_destroy (cr);
        cairo_surface_destroy(surf);
        break;
        }
    }
}

