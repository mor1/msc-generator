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

    You should have received a copy of the GNU Affero General Public License
    along with Msc-generator.  If not, see <http://www.gnu.org/licenses/>.
*/

// MscGenDoc.cpp : implementation of the CMscGenDoc class
//

#include "stdafx.h"
#include "Msc-generator.h"

#include "version.h"
#include "MscGenDoc.h"
#include "SrvrItem.h"
#include "MainFrm.h"
#include "MscGenView.h"

#include <string>
#include <list>
#include <set>
#include <map>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



class CScaleDlg : public CDialog
{
public:
	CScaleDlg();

// Dialog Data
	enum { IDD = IDD_SCALE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog( );
	afx_msg void OnBnClicked();

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
    double m_scale;
    UINT m_x;
    UINT m_y;
    UINT m_xy_x;
    UINT m_xy_y;
    CSize m_orig_size;
    double m_res_x, m_res_y;
    int m_selected;
};

BEGIN_MESSAGE_MAP(CScaleDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO_SCALING_NO, &CScaleDlg::OnBnClicked)
	ON_BN_CLICKED(IDC_RADIO_SCALING_SCALE, &CScaleDlg::OnBnClicked)
	ON_BN_CLICKED(IDC_RADIO_SCALING_X, &CScaleDlg::OnBnClicked)
	ON_BN_CLICKED(IDC_RADIO_SCALING_Y, &CScaleDlg::OnBnClicked)
	ON_BN_CLICKED(IDC_RADIO_SCALING_XY, &CScaleDlg::OnBnClicked)
END_MESSAGE_MAP()

CScaleDlg::CScaleDlg() : CDialog(CScaleDlg::IDD)
    , m_scale(0)
    , m_x(0)
    , m_y(0)
    , m_xy_x(0)
    , m_xy_y(0)
{
}

void CScaleDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_SCALE, m_scale);
    DDV_MinMaxDouble(pDX, m_scale, 0.001, 100);
    DDX_Text(pDX, IDC_EDIT_X, m_x);
	DDV_MinMaxUInt(pDX, m_x, 10, 200000);
    DDX_Text(pDX, IDC_EDIT_Y, m_y);
	DDV_MinMaxUInt(pDX, m_y, 10, 200000);
    DDX_Text(pDX, IDC_EDIT_XY_X, m_xy_x);
	DDV_MinMaxUInt(pDX, m_xy_x, 10, 200000);
    DDX_Text(pDX, IDC_EDIT_XY_Y, m_xy_y);
	DDV_MinMaxUInt(pDX, m_xy_y, 10, 200000);
}

BOOL CScaleDlg::OnInitDialog( ) 
{
    char buff[200];
    sprintf(buff, "(%d x %d)", m_orig_size.cx, m_orig_size.cy);
    GetDlgItem(IDC_STATIC_ORIGINAL_SIZE)->SetWindowText(buff);
    m_selected = AfxGetApp()->GetProfileInt(REG_SECTION_SETTINGS, "SCALING_OPTION", 0);
    m_scale = AfxGetApp()->GetProfileInt(REG_SECTION_SETTINGS, "SCALING_SCALE", 1000)/1000.;
    m_x = AfxGetApp()->GetProfileInt(REG_SECTION_SETTINGS, "SCALING_X", m_orig_size.cx);
    m_y = AfxGetApp()->GetProfileInt(REG_SECTION_SETTINGS, "SCALING_Y", m_orig_size.cy);
    m_xy_x = AfxGetApp()->GetProfileInt(REG_SECTION_SETTINGS, "SCALING_XY_X", m_orig_size.cx);
    m_xy_y = AfxGetApp()->GetProfileInt(REG_SECTION_SETTINGS, "SCALING_XY_Y", m_orig_size.cy);
    ((CButton*)GetDlgItem(IDC_RADIO_SCALING_NO))->SetCheck(m_selected==0);
    ((CButton*)GetDlgItem(IDC_RADIO_SCALING_SCALE))->SetCheck(m_selected==1);
    ((CButton*)GetDlgItem(IDC_RADIO_SCALING_X))->SetCheck(m_selected==2);
    ((CButton*)GetDlgItem(IDC_RADIO_SCALING_Y))->SetCheck(m_selected==3);
    ((CButton*)GetDlgItem(IDC_RADIO_SCALING_XY))->SetCheck(m_selected==4);
    GetDlgItem(IDC_EDIT_SCALE)->EnableWindow(m_selected==1);
    GetDlgItem(IDC_EDIT_X)->EnableWindow(m_selected==2);
    GetDlgItem(IDC_EDIT_Y)->EnableWindow(m_selected==3);
    GetDlgItem(IDC_EDIT_XY_X)->EnableWindow(m_selected==4);
    GetDlgItem(IDC_EDIT_XY_Y)->EnableWindow(m_selected==4);
	BOOL a = CDialog::OnInitDialog();
	return a;
}

void CScaleDlg::OnBnClicked()
{
    if (((CButton*)GetDlgItem(IDC_RADIO_SCALING_NO))->GetCheck())
        m_selected = 0;
    else if (((CButton*)GetDlgItem(IDC_RADIO_SCALING_SCALE))->GetCheck())
        m_selected = 1;
    else if (((CButton*)GetDlgItem(IDC_RADIO_SCALING_X))->GetCheck())
        m_selected = 2;
    else if (((CButton*)GetDlgItem(IDC_RADIO_SCALING_Y))->GetCheck())
        m_selected = 3;
    else if (((CButton*)GetDlgItem(IDC_RADIO_SCALING_XY))->GetCheck())
        m_selected = 4;
    else
        _ASSERT(0);
    GetDlgItem(IDC_EDIT_SCALE)->EnableWindow(m_selected==1);
    GetDlgItem(IDC_EDIT_X)->EnableWindow(m_selected==2);
    GetDlgItem(IDC_EDIT_Y)->EnableWindow(m_selected==3);
    GetDlgItem(IDC_EDIT_XY_X)->EnableWindow(m_selected==4);
    GetDlgItem(IDC_EDIT_XY_Y)->EnableWindow(m_selected==4);
}

AnimationElement::AnimationElement(Element *a, ElementType et, int delay, 
                       int appear, int disappear) :
    arc(a), what(et), status(APPEARING), fade_value(0), fade_delay(delay),
    appe_time(appear), disa_time(disappear)
{
    _ASSERT(et==CONTROL || et==TRACKRECT);
    _ASSERT(arc);
    if (appe_time<0) appe_time = default_appear_time[what];
    if (disa_time<0) disa_time = default_disapp_time[what];
}

AnimationElement::AnimationElement(ElementType et, int delay, 
                       int appear, int disappear) :
    arc(NULL), what(et), status(APPEARING), fade_value(0), fade_delay(delay),
    appe_time(appear), disa_time(disappear)
{
    _ASSERT(et==FALLBACK_IMAGE || et==COMPILING_GREY);
    if (appe_time<0) appe_time = default_appear_time[what];
    if (disa_time<0) disa_time = default_disapp_time[what];
}


const unsigned AnimationElement::default_appear_time[MAX_TYPE] = {100, 300, 1, 100};
const unsigned AnimationElement::default_disapp_time[MAX_TYPE] = {100, 300, 500, 100};


// CMscGenDoc

IMPLEMENT_DYNCREATE(CMscGenDoc, COleServerDocEx)

BEGIN_MESSAGE_MAP(CMscGenDoc, COleServerDocEx)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_COMMAND(ID_EDIT_REPLACE, OnEditReplace)
	ON_COMMAND(ID_EDIT_REPEAT, OnEditRepeat)
    ON_COMMAND(ID_EDIT_SELECT_ALL, &CMscGenDoc::OnEditSelectAll)
	ON_COMMAND(ID_EDIT_COPYENTIRECHART, OnEditCopyEntireChart)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPYENTIRECHART, OnUpdateFileExport)
    ON_COMMAND_RANGE(ID_COPY_PAGE1, ID_COPY_PAGE99, OnCopyPage)
	ON_COMMAND(ID_EDIT_PASETENTIRECHART, OnEditPasteEntireChart)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASETENTIRECHART, OnUpdateEditPasteEntireChart)
	ON_COMMAND(ID_EDIT_UPDATE, OnEditUpdate)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UPDATE, OnUdpateEditUpdate)
	ON_COMMAND(ID_FILE_EXPORT, OnFileExport)
	ON_LBN_SELCHANGE(IDC_OUTPUT_LIST, OnSelChange)
	ON_COMMAND(ID_VIEW_NEXTERROR, &CMscGenDoc::OnViewNexterror)
	ON_COMMAND(ID_VIEW_PREVERROR, &CMscGenDoc::OnViewPreverror)
	ON_COMMAND(ID_VIEW_ZOOMNORMALIZE, OnViewZoomnormalize)
	ON_COMMAND(ID_VIEW_ADJUSTWIDTH, OnViewAdjustwidth)
	ON_COMMAND(ID_VIEW_FITTOWIDTH, OnViewFittowidth)
	ON_COMMAND(ID_ZOOMMODE_KEEPINOVERVIEW, OnZoommodeKeepinoverview)
	ON_COMMAND(ID_ZOOMMODE_KEEPADJUSTINGWINDOWWIDTH, OnZoommodeKeepadjustingwindowwidth)
	ON_COMMAND(ID_ZOOMMODE_KEEPFITTINGTOWIDTH, OnZoommodeKeepfittingtowidth)
	ON_UPDATE_COMMAND_UI(ID_ZOOMMODE_KEEPINOVERVIEW, OnUpdateZoommodeKeepinoverview)
	ON_UPDATE_COMMAND_UI(ID_ZOOMMODE_KEEPADJUSTINGWINDOWWIDTH, OnUpdateZoommodeKeepadjustingwindowwidth)
	ON_UPDATE_COMMAND_UI(ID_ZOOMMODE_KEEPFITTINGTOWIDTH, OnUpdateZoommodeKeepfittingtowidth)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileExport)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileExport)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT, OnUpdateFileExport)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCutCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCutCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateEditCutCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACE, OnUpdateEditCutCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPEAT, OnUpdateEditCutCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditCutCopy)
	ON_COMMAND(ID_BUTTON_EDITTEXT, OnButtonEdittext)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_EDITTEXT, OnUpdateButtonEdittext)
	ON_COMMAND(ID_BUTTON_TRACK, OnButtonTrack)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_TRACK, OnUpdateButtonTrack)
	ON_COMMAND(ID_INDICATOR_TRK, OnButtonTrack)
	ON_COMMAND(ID_HELP_HELP, OnHelpHelp)
END_MESSAGE_MAP()

CLIPFORMAT NEAR CMscGenDoc::m_cfPrivate = NULL;

// CMscGenDoc construction/destruction

CMscGenDoc::CMscGenDoc() : m_ExternalEditor(this)
{
	// Use OLE compound files, may not be needed for new environment
	EnableCompoundFile();

	// one-time construction code here
	m_cfPrivate = (CLIPFORMAT)::RegisterClipboardFormat(_T("Msc-Generator Signalling Chart"));
	m_ZoomMode = (EZoomMode)AfxGetApp()->GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_DEFAULTZOOMMODE, 0);
	m_zoom = 100;
	m_bTrackMode = false;
	m_saved_charrange.cpMin = -1;
	m_last_arc = NULL;
	m_pViewFadingTimer = NULL;

    m_charts.push_back(CChartData());
	m_itrEditing = m_charts.begin();
	m_itrShown = m_charts.end();
	m_itrSaved = m_itrEditing; //start as unmodified
	m_bAttemptingToClose = false;
    serialize_doc_overhead = 0;
    m_page_serialized_in = -1; //no page data available from serialize
    m_highlight_fallback_images = false;
    CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    m_ChartShown.SetFallbackResolution(pApp->m_uFallbackResolution);
    m_ChartShown.SetPageBreaks(pApp->m_bPageBreaks);
    m_ChartShown.CompileIfNeeded();
    m_pCompilingThread = NULL;
    //m_animations.push_back(AnimationElement(AnimationElement::COMPILING_GREY, -1));
    //m_animations.begin()->status = AnimationElement::SHOWING;
    //m_animations.begin()->fade_value = 1;
    m_Progress = 0;
}

CMscGenDoc::~CMscGenDoc()
{
}

COleServerItem* CMscGenDoc::OnGetEmbeddedItem()
{
	// OnGetEmbeddedItem is called by the framework to get the COleServerItem
	//  that is associated with the document.  It is only called when necessary.

	CMscGenSrvrItem* pItem = new CMscGenSrvrItem(this, 0);  //Do not force any page
	ASSERT_VALID(pItem);
	return pItem;
}

COleServerItem* CMscGenDoc::OnGetLinkedItem(LPCTSTR lpszItemName)
{
    // look in current list first
	COleServerItem* pItem = COleServerDoc::OnGetLinkedItem(lpszItemName);
	if (pItem != NULL)
		return pItem;

    unsigned page = 0;
    sscanf(lpszItemName, "%u", &page);
    if (page > m_ChartShown.GetPages())
        return NULL;

	pItem = new CMscGenSrvrItem(this, page);
	return pItem;
}


CDocObjectServer *CMscGenDoc::GetDocObjectServer(LPOLEDOCUMENTSITE pDocSite)
{
	return new CDocObjectServer(this, pDocSite);
}

// CMscGenDoc diagnostics
#ifdef _DEBUG
void CMscGenDoc::AssertValid() const
{
	COleServerDocEx::AssertValid();
}

void CMscGenDoc::Dump(CDumpContext& dc) const
{
	COleServerDocEx::Dump(dc);
}
#endif //_DEBUG

// CMscGenDoc serialization
void CMscGenDoc::Serialize(CArchive& ar)
{
    unsigned page = m_ChartShown.GetPage(); 
    SerializePage(ar, page);
    if (!ar.IsStoring())
        m_page_serialized_in = page;
    m_uSavedPage = page;
}


struct stringpair
{
    string url, info;
    stringpair(const string &a, const string &b) : url(a), info(b) {}
    bool operator < (const stringpair &o) const { return url==o.url ? info < o.info : url < o.url; }
};

#define NEW_VERSION_STRING "@@@Msc-generator later than 2.3.4"

void CMscGenDoc::SerializePage(CArchive& ar, unsigned &page)
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	if (ar.IsStoring()) {
		ar << CString(NEW_VERSION_STRING); //if format starts with this string, we have file version afterwards
		ar << unsigned(6); //file format version
        ar << m_ChartShown.GetDesign();
		ar << page;
		ar << m_ChartShown.GetText();
        ar << page; //dummy
        ar << unsigned(LIBMSCGEN_MAJOR);
        ar << unsigned(LIBMSCGEN_MINOR);
        ar << unsigned(LIBMSCGEN_SUPERMINOR);
        ar << unsigned(m_ChartShown.GetForcedArcCollapse().size());
        for (auto i = m_ChartShown.GetForcedArcCollapse().begin(); 
                  i !=m_ChartShown.GetForcedArcCollapse().end(); i++) {
            ar << unsigned(i->first.file_pos.start.file);
            ar << unsigned(i->first.file_pos.start.line);
            ar << unsigned(i->first.file_pos.start.col);
            ar << unsigned(i->first.file_pos.end.file);
            ar << unsigned(i->first.file_pos.end.line);
            ar << unsigned(i->first.file_pos.end.col);
            ar << unsigned(i->second);
        }
        ar << unsigned(m_ChartShown.GetForcedEntityCollapse().size());
        for (auto i = m_ChartShown.GetForcedEntityCollapse().begin();
                   i!=m_ChartShown.GetForcedEntityCollapse().end(); i++)
            ar << CString(i->first.c_str()) << unsigned(i->second);
        ar << pApp->m_uFallbackResolution;
        ar << pApp->m_bPageBreaks;
        //List shapes used
        if (m_ChartShown.GetUsedShapes().size()) {
            std::multimap<stringpair, string> shapes;
            for (unsigned u : m_ChartShown.GetUsedShapes())
                shapes.emplace(stringpair(pApp->m_Shapes[u].GetURL(), pApp->m_Shapes[u].GetURL()),
                               pApp->m_Shapes[u].name);
            for (auto i = shapes.begin(); i!=shapes.end(); /*nope*/) {
                unsigned count = shapes.count(i->first);
                _ASSERT(count);
                ar << count;
                ar << CString(i->first.url.c_str());
                ar << CString(i->first.info.c_str());
                for (unsigned u = 0; u<count; u++, i++)
                    ar << CString(i->second.c_str());
            }
        }
        ar << 0; //terminating shapes
    } else {
        CChartData chart;
		CString text;
		CString design;
		unsigned read_page;
		ar >> design;
        unsigned file_version = 0;
		if (design != NEW_VERSION_STRING) { 
			//Old file format
			ar >> read_page;
			unsigned alloc = 16384;
			unsigned length = 0;
			char *buff = (char*)malloc(alloc);
			while (1) {
				length += ar.Read(buff+length, alloc-1-length);
				if (length == alloc-1) {
                    char * const buff2 = (char*)realloc(buff, alloc+=16384);
                    if (buff2) buff = buff2;
                    else break; //could not allocate more space. Work with truncated file.
                } else break;
			} 
			buff[length] = 0;
			text = buff;
			free(buff);
		} else {
			//New file format
			ar >> file_version;
			if (file_version ==0 ) {//since v2.3.4
				char *buff;
				unsigned length;
				ar >> design;
				ar >> read_page;
				ar >> length;
				buff = (char*)malloc(length+1);
				ar.Read(buff, length);
				buff[length] = 0;
				text = buff;
				free(buff);
            } else { //later versions use this
				ar >> design;
				ar >> read_page;
				ar >> text;
            }
		}
		EnsureCRLF(text);
		ReplaceTAB(text);
        chart.Set(text);
        chart.SetDesign(design);
        unsigned force_entity_size, force_arc_size;
        unsigned a=0, b=0, c=0;
        if (file_version >= 4) { //since 3.4.3
            ar >> a; //keep "forced_page" intact, "a" is dummy, will be overwritten below
        }
        if (file_version >= 3) { //since 3.1.3 : read version and force arc collapse
            ar >> a;
            ar >> b;
            ar >> c;
            chart.SetVersion(a,b,c);
            ar >> force_arc_size;
            for (unsigned i=0; i<force_arc_size; i++) {
                ArcSignature as;
                ar >> as.file_pos.start.file;
                ar >> as.file_pos.start.line;
                ar >> as.file_pos.start.col;
                ar >> as.file_pos.end.file;
                ar >> as.file_pos.end.line;
                ar >> as.file_pos.end.col;
                ar >> a;
                chart.ForceArcCollapse(as, EBoxCollapseType(a));
            }
        }
        if (file_version >= 2) { //since v3.1: read force entity collapse
            ar >> force_entity_size;
            for (unsigned i=0; i<force_entity_size; i++) {
                CString s; unsigned b2;
                ar >> s;
                ar >> b2;
                chart.ForceEntityCollapse(string(s), bool(b2));
            }
        }
        pApp->m_uFallbackResolution = 300; //default before 3.5.3
        if (file_version >= 5) { //since 3.5.3
            ar >> pApp->m_uFallbackResolution;
            m_uSavedFallbackResolution = pApp->m_uFallbackResolution;
            ar >> pApp->m_bPageBreaks;
            m_bSavedPageBreaks = pApp->m_bPageBreaks;
        }
        if (file_version >= 6) { // since 3.7.5
            unsigned count;
            ar >> count;
            CString message;
            bool unknown = false;
            unsigned enumerator = 1;
            while (count) {
                CString url, info;
                ar >> url;
                ar >> info;
                bool added = false;
                while (count--) {
                    CString name;
                    ar >> name;
                    if (!added && pApp->m_Shapes.GetShapeNo(string(name))<0) {
                        if (url.GetLength()+info.GetLength()>0) {
                            message.AppendFormat("%d. ", enumerator++);
                            if (info.GetLength()) {
                                message.Append(info);
                                if (url.GetLength()) {
                                    message.Append(" (");
                                    message.Append(url);
                                    message.Append(")");
                                }
                            } else //only url
                                message.Append(url);
                            message.Append("\n");
                        } else
                            unknown = true;
                        added = true;
                    }
                }
                ar >> count;
            }
            if (message.GetLength()) {
                message.Insert(0, "The embedded chart contains shapes from the following shape files not present on this machine.\n");
                if (unknown)
                    message.Append("Plus unknown shape files.\n");
                message.Append("These shapes will be missing.");
            } else if (unknown)
                message = "The embedded chart contains shapes not present on this machine.\nThese shapes will be missing.";
            if (message.GetLength())
                MessageBox(NULL, message, "Msc-generator error", MB_OK);
        }
        CMainFrame *pMainWnd = dynamic_cast<CMainFrame *>(AfxGetMainWnd());
        if (pMainWnd) {
            CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;
            pMainWnd->m_wndRibbonBar.GetElementsByID(ID_EMBEDDEDOPTIONS_FALLBACK_RES, arButtons);
            _ASSERT(arButtons.GetSize()==1);
            CMFCRibbonSlider *s = dynamic_cast<CMFCRibbonSlider *>(arButtons[0]);
            if (s) s->SetPos(pApp->m_uFallbackResolution);
        }
        InsertNewChart(chart);
	} /* not IsStoring */
}

void CMscGenDoc::DeleteContents()
{
    KillCompilation();
    m_charts.clear();
	m_charts.push_back(CChartData());
	m_itrEditing = m_charts.begin();
	m_itrShown = m_charts.end();
	m_itrSaved = m_charts.end(); //start as modified
	m_ChartShown.Set("");
    m_ChartShown.ForceArcCollapse(ArcSignatureCatalog());
    m_ChartShown.ForceEntityCollapse(EntityCollapseCatalog());
    m_ChartShown.CompileIfNeeded();
	COleServerDocEx::DeleteContents();
}

BOOL CMscGenDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
    KillCompilation();
	m_bAttemptingToClose = true;
	BOOL ret = COleServerDocEx::CanCloseFrame(pFrame);
	m_bAttemptingToClose = false;
	return ret;
}

BOOL CMscGenDoc::OnNewDocument()
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);

	bool restartEditor = m_ExternalEditor.IsRunning();
	if (restartEditor) 
		m_ExternalEditor.Stop(STOPEDITOR_WAIT);
	if (!COleServerDocEx::OnNewDocument())
		return FALSE;

	// (SDI documents will reuse this document)
	CChartData data;
	if (pApp)
		data.Set(pApp->m_DefaultText);
	m_charts.clear();
	m_charts.push_back(data);
	m_itrEditing = m_charts.begin();
	m_itrSaved = m_itrEditing; //start as unmodified
	if (pApp->IsInternalEditorRunning())
		pApp->m_pWndEditor->m_ctrlEditor.UpdateText(m_itrEditing->GetText(), m_itrEditing->m_sel, true);
	CompileEditingChart(true, false);
	if (restartEditor)
		m_ExternalEditor.Start("Untitled");
	return TRUE;
}

BOOL CMscGenDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	ASSERT_VALID(this);
	ASSERT(lpszPathName == NULL || AfxIsValidString(lpszPathName));

	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);

	bool restartEditor = m_ExternalEditor.IsRunning();
	if (restartEditor) 
		m_ExternalEditor.Stop(STOPEDITOR_WAIT);

	if (lpszPathName == NULL) {
		//this one will use serialize which sets m_itrCurrent to loaded entry
		if (!COleServerDocEx::OnOpenDocument(lpszPathName))
			return FALSE;
        //Copy the saved display parameters to current chart showing from where they
        //will be copied to the chart under compilation
        m_ChartShown.SetPage(m_uSavedPage);
        m_ChartShown.SetFallbackResolution(m_uSavedFallbackResolution);
        m_ChartShown.SetPageBreaks(m_bSavedPageBreaks);
	} else {
		// always register the document before opening it
		Revoke();
		if (!RegisterIfServerAttached(lpszPathName, FALSE))
		{
			// always output a trace (it is just an FYI -- not generally fatal)
			TRACE(traceOle, 0, _T("Warning: Unable to register moniker '%s' as running\n"), lpszPathName);
		}

		USES_CONVERSION;

		if (IsModified())
			TRACE(traceOle, 0, "Warning: OnOpenDocument replaces an unsaved document.\n");

		CChartData data;
		if (!data.Load(lpszPathName)) {
			CFileException e;
			ReportSaveLoadException(lpszPathName, &e,
				FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
			return FALSE;
		}
		data.SetDesign(m_itrEditing->GetDesign());
		InsertNewChart(data); //all pages visible

	}
	//Copy text to the internal editor
	if (pApp->IsInternalEditorRunning())
		pApp->m_pWndEditor->m_ctrlEditor.UpdateText(m_itrEditing->GetText(), m_itrEditing->m_sel, true);
	//Delete all entries before the currently loaded one (no undo)
	//part after (redo) was deleted by Serialize or InsertNewChart
	//Here we set all our m_itr* iterators to valid values afterwards
	m_charts.erase(m_charts.begin(), m_itrEditing);
	m_itrSaved = m_itrEditing;

	// if the app was started only to print or to display an embedded object, don't set user control
	//Copied from COleLinkingDoc
    const bool user = pApp->m_pCmdInfo == NULL ||
		(pApp->m_pCmdInfo->m_nShellCommand != CCommandLineInfo::FileDDE &&
		 pApp->m_pCmdInfo->m_nShellCommand != CCommandLineInfo::FilePrint);
	if (user)
	{
		AfxOleSetUserCtrl(TRUE);
	}
    //if the main window is invisible, do a blocking compilation
    //Needed for single thread operation when the system DLLs start our
    //application to draw an embedded object, for instance.
    CMainFrame *pWnd = m_bAttemptingToClose ? NULL : dynamic_cast<CMainFrame *>(AfxGetMainWnd());
    CompileEditingChart(true, pWnd || !pWnd->IsWindowVisible());
	if (restartEditor)
		m_ExternalEditor.Start(lpszPathName);
	return TRUE;
}

BOOL CMscGenDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    bool restartEditor = false;
	if (lpszPathName==NULL) {
		SyncShownWithEditing("update the container document");
        if (!COleServerDocEx::OnSaveDocument(lpszPathName)) 
            return FALSE;
	} else {
	    //Restart external editor only if file name changes
	    restartEditor = lpszPathName!=GetPathName() && m_ExternalEditor.IsRunning();
	    if (restartEditor) 
		    m_ExternalEditor.Stop(STOPEDITOR_WAIT);
	    SyncShownWithEditing("save");
	    if (!m_itrShown->Save(lpszPathName)) {
		    CFileException e;
		    ReportSaveLoadException(lpszPathName, &e,
			    TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
		    return FALSE;
	    }
    }
    //SyncShownWithEditing() above either made m_itrShown equal to 
    //m_itrEditing by compiling, or left them different. In the latter
    //case we effectively perform an Undo to m_itrShown.
	m_itrSaved = m_itrEditing = m_itrShown;
	if (pApp->IsInternalEditorRunning()) 
		pApp->m_pWndEditor->m_ctrlEditor.UpdateText(m_itrEditing->GetText(), m_itrEditing->m_sel, true);
    if (restartEditor) 
        m_ExternalEditor.Start(lpszPathName);
    m_uSavedFallbackResolution = pApp->m_uFallbackResolution;
    m_bSavedPageBreaks = pApp->m_bPageBreaks;
    _ASSERT(!CheckIfChanged());
	return TRUE;
}

void CMscGenDoc::OnCloseDocument()
{
	m_ExternalEditor.Stop(STOPEDITOR_FORCE);
    KillCompilation();
	COleServerDocEx::OnCloseDocument();
}


void CMscGenDoc::OnUpdateFileExport(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_ChartShown.IsEmpty() || !m_itrEditing->IsEmpty());
}

void CMscGenDoc::OnFileExport()
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	SyncShownWithEditing("export");
    CString name = GetPathName();
    double x_scale=1, y_scale=1;
    bool bitmap;
    do {  //must not call DoModal twice for the same instance of CFileDialog
        CFileDialog dialog(false);
        dialog.m_pOFN->Flags &= ~OFN_OVERWRITEPROMPT & ~OFN_NOTESTFILECREATE;
        dialog.m_pOFN->lpstrTitle = "Export to file";
        dialog.m_pOFN->lpstrFilter =	
            "Portable Network Graphics (*.png)\0*.png\0"
            "Windows Bitmap (*.bmp)\0*.bmp\0"
            "Enhanced Metafile (*emf)\0*.emf\0"
            "Scalable Vector Graphics (*svg)\0*.svg\0"
            "Portable Document Format (*.pdf)\0*.pdf\0"
            "Encapsulated PostScript (*.eps)\0*.eps\0";
        dialog.m_pOFN->nFilterIndex = pApp->GetProfileInt(REG_SECTION_SETTINGS, "ExportFileType", 1);
        if (dialog.m_pOFN->nFilterIndex>6 || dialog.m_pOFN->nFilterIndex<1)
            dialog.m_pOFN->nFilterIndex = 1;
        char filename[1024];
        strcpy_s(filename, name);
        if (PathFindExtension(filename) == CString(".signalling"))
            PathRemoveExtension(filename);
        dialog.m_pOFN->lpstrFile = filename;  
        dialog.m_pOFN->nMaxFile = sizeof(filename);
        //dialog.ApplyOFNToShellDialog();
        if (dialog.DoModal() != IDOK)
            return;
        pApp->WriteProfileInt(REG_SECTION_SETTINGS, "ExportFileType", dialog.m_pOFN->nFilterIndex);
        //dialog.UpdateOFNFromShellDialog();
        name = dialog.GetPathName();
        CString ext = PathFindExtension(name);
        //if we do not recognize the extension typed by the user, we add one from the selected type
        if (ext.CompareNoCase(".png")!=0 && ext.CompareNoCase(".bmp")!=0 && 
            ext.CompareNoCase(".emf")!=0 && ext.CompareNoCase(".svg")!=0 && 
            ext.CompareNoCase(".pdf")!=0 && ext.CompareNoCase(".eps")!=0 &&
            ext.CompareNoCase(".wmf")!=0) { //undocumented wmf export
            switch(dialog.m_pOFN->nFilterIndex) {
            case 1: name += ".png"; break;
            case 2: name += ".bmp"; break;
            case 3: name += ".emf"; break;
            case 4: name += ".svg"; break;
            case 5: name += ".pdf"; break;
            case 6: name += ".eps"; break;
            default: _ASSERT(0);
            }
        }
        if (PathFileExists(name)) 
            if (IDNO == AfxMessageBox("File " + name + " exists. Do you want to overwrite?", MB_YESNO))
                continue;
        ext = PathFindExtension(name);
        bitmap = ext.CompareNoCase(".png")==0 || ext.CompareNoCase(".bmp")==0;
        break;
    } while(1);
    if (bitmap) { 
        CScaleDlg scale;
        scale.m_orig_size = m_ChartShown.GetSize();
        if (IDCANCEL == scale.DoModal())
            return;
        switch (scale.m_selected) {
        default: _ASSERT(0); //no break, release edition falls through to no scaling
        case 0: x_scale = y_scale = 1; break;
        case 1: x_scale = y_scale = scale.m_scale; break;
        case 2: x_scale = y_scale = double(scale.m_x)/scale.m_orig_size.cx; break;
        case 3: x_scale = y_scale = double(scale.m_y)/scale.m_orig_size.cy; break;
        case 4: 
            x_scale = double(scale.m_x)/scale.m_orig_size.cx; 
            y_scale = double(scale.m_y)/scale.m_orig_size.cy; 
            break;
        }
        AfxGetApp()->WriteProfileInt(REG_SECTION_SETTINGS, "SCALING_OPTION", scale.m_selected);
        AfxGetApp()->WriteProfileInt(REG_SECTION_SETTINGS, "SCALING_SCALE", int(scale.m_scale*1000));
        AfxGetApp()->WriteProfileInt(REG_SECTION_SETTINGS, "SCALING_X", scale.m_x);
        AfxGetApp()->WriteProfileInt(REG_SECTION_SETTINGS, "SCALING_Y", scale.m_y);
        AfxGetApp()->WriteProfileInt(REG_SECTION_SETTINGS, "SCALING_XY_X", scale.m_xy_x);
        AfxGetApp()->WriteProfileInt(REG_SECTION_SETTINGS, "SCALING_XY_Y", scale.m_xy_y);
    }
    m_ChartShown.DrawToFile(name, false, x_scale, y_scale);
}

void CMscGenDoc::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_itrEditing != m_charts.begin());
}

void CMscGenDoc::OnEditUndo()
{
	if (m_itrEditing == m_charts.begin()) return;
	bool restartEditor = m_ExternalEditor.IsRunning();
	if (restartEditor) 
		m_ExternalEditor.Stop(STOPEDITOR_WAIT);
	//Step back and update the internal editor
	m_itrEditing--;
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	if (pApp->IsInternalEditorRunning()) 
		pApp->m_pWndEditor->m_ctrlEditor.UpdateText(m_itrEditing->GetText(), m_itrEditing->m_sel, true);

	CheckIfChanged();     
	if (restartEditor)
		m_ExternalEditor.Start();
}

void CMscGenDoc::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_itrEditing != --m_charts.end());
}

void CMscGenDoc::OnEditRedo()
{
	if (m_itrEditing == --m_charts.end()) return;
	bool restartEditor = m_ExternalEditor.IsRunning();
	if (restartEditor) 
		m_ExternalEditor.Stop(STOPEDITOR_WAIT);
	m_itrEditing++;
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	if (pApp->IsInternalEditorRunning()) 
		pApp->m_pWndEditor->m_ctrlEditor.UpdateText(m_itrEditing->GetText(), m_itrEditing->m_sel, true);
	
	CheckIfChanged();     
	if (restartEditor)
		m_ExternalEditor.Start();
}

void CMscGenDoc::OnUpdateEditCutCopy(CCmdUI *pCmdUI)
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	pCmdUI->Enable(pApp->IsInternalEditorRunning());
}

void CMscGenDoc::OnEditCut()
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	if (!pApp->IsInternalEditorRunning()) return;
	pApp->m_pWndEditor->m_ctrlEditor.Cut();
}

void CMscGenDoc::OnEditCopy()
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	if (!pApp->IsInternalEditorRunning()) return;
	pApp->m_pWndEditor->m_ctrlEditor.Copy();
}

void CMscGenDoc::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	COleDataObject dataObject;
	dataObject.AttachClipboard();
	pCmdUI->Enable(dataObject.IsDataAvailable(CF_TEXT) && pApp->IsInternalEditorRunning());
}

void CMscGenDoc::OnEditPaste()
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	if (!pApp->IsInternalEditorRunning()) return;
	COleDataObject dataObject;
	dataObject.AttachClipboard();
	if (!dataObject.IsDataAvailable(CF_TEXT)) return;
	pApp->m_pWndEditor->m_ctrlEditor.PasteSpecial(CF_TEXT);
}

void CMscGenDoc::OnEditFind()
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	if (pApp->IsInternalEditorRunning()) pApp->m_pWndEditor->OnEditFindReplace(true);
}

void CMscGenDoc::OnEditRepeat()
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	if (pApp->IsInternalEditorRunning()) pApp->m_pWndEditor->OnEditRepeat();
}

void CMscGenDoc::OnEditReplace()
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	if (pApp->IsInternalEditorRunning()) pApp->m_pWndEditor->OnEditFindReplace(false);
}


void CMscGenDoc::OnEditCopyEntireChart()
{
	//Copy is handled by SrvItem
    SyncShownWithEditing("copy the chart to the clipboard");
	CMscGenSrvrItem *pItem = dynamic_cast<CMscGenSrvrItem*>(COleServerDocEx::GetEmbeddedItem());
    TRY 
    {
        pItem->CopyToClipboard(TRUE);
    }   
    CATCH_ALL(e)
    {
        AfxMessageBox("Copy to clipboard failed");
    }
    END_CATCH_ALL
}

void CMscGenDoc::OnCopyPage(UINT id)
{
    const unsigned page = id - ID_COPY_PAGE1 + 1;
    char buff[300];
    sprintf(buff, "copy page #%u to the clipboard", page);
    SyncShownWithEditing(buff);
    if (m_ChartShown.GetPages() < page) {
        sprintf(buff, "Sorry, I no longer have page #%u, "
                      "the entire chart consists of only %u page(s). "
                      "I did not change the clipboard.", 
                      page, m_ChartShown.GetPages());
        AfxMessageBox(buff);
    } else {
        TRY 
        {
            sprintf(buff, "%u", page);
            //Copy is handled by SrvItem
            COleServerItem *pItem = OnGetLinkedItem(buff);
            pItem->CopyToClipboard(TRUE);
        }   
        CATCH_ALL(e)
        {
            AfxMessageBox("Copy to clipboard failed");
        }
        END_CATCH_ALL
    }
}


void CMscGenDoc::OnUpdateEditPasteEntireChart(CCmdUI *pCmdUI)
{
	COleDataObject dataObject;
	dataObject.AttachClipboard();
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	pCmdUI->Enable(!pApp->m_bFullScreenViewMode && (dataObject.IsDataAvailable(m_cfPrivate) || 
						        					dataObject.IsDataAvailable(CF_TEXT)));
}

void CMscGenDoc::DoPasteData(COleDataObject &dataObject) 
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	//Do not insert when in FullScreenViewMode
	if (pApp->m_bFullScreenViewMode) return;
	bool restartEditor = m_ExternalEditor.IsRunning();
	if (dataObject.IsDataAvailable(m_cfPrivate)) {
		// get file refering to clipboard data
		CFile *pFile = dataObject.GetFileData(m_cfPrivate);
		if (pFile == NULL) return;
		if (restartEditor) 
			m_ExternalEditor.Stop(STOPEDITOR_FORCE);
		// connect the file to the archive and read the contents
		CArchive ar(pFile, CArchive::load);
		ar.m_pDocument = this; // for COleClientItem serialize
		Serialize(ar); //Kills redo part
		ar.Close();
		delete pFile;
	} else if (dataObject.IsDataAvailable(CF_TEXT)) {
		HGLOBAL hGlobal = dataObject.GetGlobalData(CF_TEXT);
		unsigned length = GlobalSize(hGlobal);
		void* v = GlobalLock(hGlobal);
		if (v==NULL || length==0) {
			GlobalUnlock(hGlobal);
			GlobalFree(hGlobal);
			return;
		}
		CString text;
		text.SetString((char*)v, length);
		GlobalUnlock(hGlobal);
		GlobalFree(hGlobal);
		if (restartEditor) m_ExternalEditor.Stop(STOPEDITOR_FORCE);
		InsertNewChart(CChartData(text, m_itrEditing->GetDesign())); 
        m_page_serialized_in = 0; //all pages visible
	}
	CompileEditingChart(true, false);
    m_page_serialized_in = -1;
	//Copy text to the internal editor
	if (pApp->IsInternalEditorRunning())
		pApp->m_pWndEditor->m_ctrlEditor.UpdateText(m_itrEditing->GetText(), m_itrEditing->m_sel, true);
	if (restartEditor) 
		m_ExternalEditor.Start();
};

void CMscGenDoc::OnEditPasteEntireChart()
{
	COleDataObject dataObject;
	dataObject.AttachClipboard();
	DoPasteData(dataObject);
}

void CMscGenDoc::OnEditSelectAll()
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	if (pApp->IsInternalEditorRunning()) pApp->m_pWndEditor->SelectAll();
}


void CMscGenDoc::OnButtonEdittext()
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	if (m_ExternalEditor.IsRunning())
		m_ExternalEditor.Stop(STOPEDITOR_WAIT);
	else if (!pApp->m_bFullScreenViewMode)
		m_ExternalEditor.Start();
}

void CMscGenDoc::OnUpdateButtonEdittext(CCmdUI *pCmdUI)
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	pCmdUI->SetCheck(m_ExternalEditor.IsRunning());
	if (!m_ExternalEditor.CanStart())
		if (!m_ExternalEditor.Init()) 
			AfxMessageBox("Fail to create external editor window");
	pCmdUI->Enable(m_ExternalEditor.CanStart() && !pApp->m_bFullScreenViewMode);
}

void CMscGenDoc::OnEditUpdate()
{
	//update the View, update zoom, 
	CompileEditingChart(true, false);
}

void CMscGenDoc::OnUdpateEditUpdate(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_itrShown != m_itrEditing);

    //call one of MainFrame's update proc to detect when we switch away from
    //the embedded objects category on the ribbon
    CMainFrame *pMain = dynamic_cast<CMainFrame *>(AfxGetMainWnd());
    if (pMain)
        pMain->TriggerIfRibbonCategoryChange();
}


void CMscGenDoc::OnButtonTrack()
{
    if (m_pCompilingThread && !m_bTrackMode) return;  //Do not turn TrackMode, when compiling
	SetTrackMode(!m_bTrackMode);
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	//This is called if the user pressed the toolbar button, selected the menu or pressed ctrl-T
	//Highlight the rect shown by the editor
	if (m_bTrackMode && pApp->IsInternalEditorRunning())
		OnInternalEditorSelChange();
}

void CMscGenDoc::OnUpdateButtonTrack(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bTrackMode);
	CMainFrame *pWnd = dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	if (pWnd) 
		pWnd->m_wndStatusBar.SetPaneTextColor(1, m_bTrackMode?RGB(0,0,0):RGB(100,100,100));
    pCmdUI->Enable(m_pCompilingThread==NULL);
}

void CMscGenDoc::OnViewNexterror()
{
	DoViewNexterror(true);
}

void CMscGenDoc::OnViewPreverror()
{
	DoViewNexterror(false);
}

void CMscGenDoc::DoViewNexterror(bool next)
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	COutputViewBar *pOutputView = pApp->m_pWndOutputView;
	if (!pOutputView) return;

	//Move to next error
    int line, col;
    if (!pOutputView->NextError(next)) return;
    if (!pOutputView->GetCurrentErrorLine(line, col)) return;

    //Turn tracking off
	if (m_bTrackMode) SetTrackMode(false);
	else StartFadingAll();

    //Jump in the internal and external editors
    if (pApp->IsInternalEditorRunning()) 
        pApp->m_pWndEditor->m_ctrlEditor.JumpToLine(line, col);
    if (m_ExternalEditor.IsRunning())
        m_ExternalEditor.JumpToLine(line, col);

    //Show tracking boxes for the error
    AddAnimationElement(AnimationElement::TRACKRECT,
                        m_ChartShown.GetArcByLine(line, col));
}

//Selection in the error window changes
void CMscGenDoc::OnSelChange()
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	COutputViewBar *pOutputView = pApp->m_pWndOutputView;
	if (!pOutputView) return;
    int line, col;
    if (!pOutputView->GetCurrentErrorLine(line, col)) return;
	if (pApp->IsInternalEditorRunning())
		pApp->m_pWndEditor->m_ctrlEditor.JumpToLine(line, col);
	if (m_ExternalEditor.IsRunning())
		m_ExternalEditor.JumpToLine(line, col);
}

void CMscGenDoc::ChangeDesign(const char *design) 
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);

	if (CString(design) == m_itrEditing->GetDesign()) return;
	InsertNewChart(CChartData(*m_itrEditing)); //duplicate current chart, loose undo, keep page shown
	m_itrEditing->SetDesign(design);
    pApp->m_designlib_csh.ForcedDesign = design;
	CompileEditingChart(true, false);
}

void CMscGenDoc::ChangePage(unsigned page)
{
    //skip if compiling
    if (m_pCompilingThread) return;
    m_ChartShown.SetPage(page);
    CMainFrame *pWnd = dynamic_cast<CMainFrame *>(AfxGetMainWnd());
    if (pWnd && pWnd->m_bAutoSplit) 
        pWnd->SetSplitSize(unsigned(m_ChartShown.GetHeadingSize()*m_zoom/100.));
    UpdateAllViews(NULL);
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    if (pApp->IsInternalEditorRunning()) 
        pApp->m_pWndEditor->m_ctrlEditor.SetFocus();
    if (pWnd)
        pWnd->FillPageComboBox(m_ChartShown.GetPages(), page);
}

void CMscGenDoc::StepPage(signed int step)
{
	if (step == 0 || m_ChartShown.GetPages()<=1)
		return;
    int page = m_ChartShown.GetPage() + step;
    if (page > m_ChartShown.GetPages()) page = m_ChartShown.GetPages();
    if (page < 0 ) page = 0;
    if (page == m_ChartShown.GetPage()) return;
    ChangePage(page);
}


//true if actual change happened
void CMscGenDoc::SetZoom(int zoom)
{
    if (m_pCompilingThread) return; //skip if compiling
	if (zoom < 1) zoom = m_zoom;
	if (zoom > 10000) zoom = 10000;
	if (zoom < 10) zoom = 10;

	if (zoom == m_zoom) return;
	m_zoom = zoom;
    UpdateAllViews(NULL);
	CMainFrame *pWnd = dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	if (pWnd)
        pWnd->FillZoomComboBox(m_zoom);
    return;
}


#define SCREEN_MARGIN 50 

void CMscGenDoc::ArrangeViews(EZoomMode mode)
{
    if (m_pCompilingThread) return; //skip if compiling
	if (mode == NONE) return;
	POSITION pos = GetFirstViewPosition();
	if (pos == NULL) return;
    CMscGenView* pView = dynamic_cast<CMscGenView*>(GetNextView(pos));
  	if (!pView) return;
    const CSize size = m_ChartShown.GetSize();
	if (size.cx==0 || size.cy==0) return;
	CMainFrame *pWnd = dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	if (!pWnd) return;

	//Query size of the view, the main window and the screen
	RECT view, window, screen;
	pView->GetClientRect(&view);
	pWnd->GetWindowRect(&window); 
	CClientDC dc(NULL);
	dc.GetClipBox(&screen);
	//calculate the space available to a view on the screen
	unsigned x, y;
	x = screen.right-screen.left;
	y = screen.bottom-screen.top;
	//For a full screen window, the entire screen is there
	//If not full screen, we reduce the sizes with a MARGIN and the elements of the
	//main window outside the view (toolbars, title, menus, border, docking windows, etc.)
	if (!pWnd->IsFullScreen()) {
		x -= (window.right-window.left) - (view.right-view.left) + SCREEN_MARGIN;
		y -= (window.bottom-window.top) - (view.bottom-view.top) + SCREEN_MARGIN; 
	}
	int zoom;

	//OK, we have a sane View, with some drawing in it and we are not in place with a sane main window
	switch (mode) {
		case OVERVIEW:
			//Kill all, but one view
			for (int i = 1; i<pWnd->m_wndSplitter.GetRowCount(); i++)
				pWnd->m_wndSplitter.DeleteRow(i);
            //HACK: Update this
			//Re-query view size		
			pos = GetFirstViewPosition();
			if (pos == NULL) return;
			pView = dynamic_cast<CMscGenView*>(GetNextView(pos));
  			if (!pView) return;
			pView->GetClientRect(&view);

            if (pWnd->GetStyle() & WS_MAXIMIZE) {
                //Here we are maximized and do not adjust window size
			    //See which dimension is limiting
                if (double(view.bottom - view.top)/double(view.right - view.left) > double(size.cy)/double(size.cx)) 
				    zoom = unsigned(double(view.right - view.left)/double(size.cx)*100.);
			    else 
				    zoom = unsigned(double(view.bottom - view.top)/double(size.cy)*100.);
			    if (zoom > 100) zoom = 100;
			    SetZoom(zoom);
            } else {
                //Here we fit to screen size and also change windows size
			    //See which dimension is limiting
			    if (double(y)/double(x) > double(size.cy)/double(size.cx)) 
				    zoom = unsigned(double(x)/double(size.cx)*100.);
			    else 
				    zoom = unsigned(double(y)/double(size.cy)*100.);
			    if (zoom > 100) zoom = 100;
			    SetZoom(zoom);
			    //If not fullscreen, adjust window size, too
			    if (!pWnd->IsFullScreen()) {
				    x = zoom*size.cx/100 + 1;
				    y = zoom*size.cy/100 + 1;
				    //now xy contains is the required client size
				    x += (window.right-window.left) - (view.right-view.left);
				    y += (window.bottom-window.top) - (view.bottom-view.top);
				    //now xy contains is the required client size, plus any panes
				    //next, we adjust for minimum size
				    if (x < 550) x = 580;
				    if (y < 300) y = 300;
				    if (window.left + x > screen.right - SCREEN_MARGIN)
					    window.left = screen.right - SCREEN_MARGIN - x;
				    if (window.top + y > screen.bottom - SCREEN_MARGIN)
					    window.top = screen.bottom - SCREEN_MARGIN - y;
				    pWnd->SetWindowPos(NULL, window.left, window.top, x, y,  SWP_NOZORDER | SWP_NOACTIVATE);
			    }
            }
			break;
		case CMscGenDoc::WINDOW_WIDTH:
			//Try fit real size
			zoom = 100;
			//if window is big enough do nothing
			if (view.right-view.left > zoom*size.cx/100 + 1) break;
			//adjust zoom if there is not enough space
			if (zoom*size.cx/100 + 1 > x)
				zoom = unsigned((x-1)*100./size.cx);
			SetZoom(zoom);
			//If not fullscreen, adjust window size, too
			if (!pWnd->IsFullScreen()) {
				x = zoom*size.cx/100 + 1;
                x += (window.right-window.left) - (view.right-view.left);
                //now xy contains is the required client size, plus any panes
				if (x < 550) x = 580;
				if (window.left + x > screen.right - SCREEN_MARGIN)
					window.left = screen.right - SCREEN_MARGIN - x;
				pWnd->SetWindowPos(NULL, window.left, window.top, x, (window.bottom-window.top),  SWP_NOZORDER | SWP_NOACTIVATE);
			}
			break;
		case CMscGenDoc::ZOOM_WIDTH:
			zoom = unsigned((view.right-view.left)*100./size.cx);
			if (zoom>150) zoom = 150;
			SetZoom(zoom);
			break;
	}
    UpdateAllViews(NULL);
}

void CMscGenDoc::OnViewZoomnormalize()
{
	ArrangeViews(OVERVIEW);
}

void CMscGenDoc::OnViewAdjustwidth()
{
	ArrangeViews(WINDOW_WIDTH);
}

void CMscGenDoc::OnViewFittowidth()
{
	ArrangeViews(ZOOM_WIDTH);
}


void CMscGenDoc::SwitchZoomMode(EZoomMode mode) 
{
	if (m_ZoomMode == mode)
		m_ZoomMode = NONE;
	else 
		m_ZoomMode = mode;
	ArrangeViews(m_ZoomMode);
	AfxGetApp()->WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_DEFAULTZOOMMODE, m_ZoomMode);
}

void CMscGenDoc::OnZoommodeKeepinoverview()
{
	SwitchZoomMode(OVERVIEW);
}

void CMscGenDoc::OnZoommodeKeepadjustingwindowwidth()
{
	SwitchZoomMode(WINDOW_WIDTH);
}

void CMscGenDoc::OnZoommodeKeepfittingtowidth()
{
	SwitchZoomMode(ZOOM_WIDTH);
}

void CMscGenDoc::OnUpdateZoommodeKeepinoverview(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_ZoomMode == OVERVIEW);
    pCmdUI->Enable(m_pCompilingThread==NULL);
}

void CMscGenDoc::OnUpdateZoommodeKeepadjustingwindowwidth(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_ZoomMode == WINDOW_WIDTH);
    pCmdUI->Enable(m_pCompilingThread==NULL);
}

void CMscGenDoc::OnUpdateZoommodeKeepfittingtowidth(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_ZoomMode == ZOOM_WIDTH);
    pCmdUI->Enable(m_pCompilingThread==NULL);
}

//Check where is the mouse (which view or the internal editor) and call that function's DoMouseWheel
//return true if the event is handled
bool CMscGenDoc::DispatchMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL) {
        CMscGenView* pView = dynamic_cast<CMscGenView*>(GetNextView(pos));
  	    if (pView)
			//Each view handles this message only if pt is within its area
	        if (pView->DoMouseWheel(nFlags, zDelta, pt)) return true;
	}
	//None of the views had it, try the internal editor
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	if (pApp->IsInternalEditorRunning())
		return pApp->m_pWndEditor->m_ctrlEditor.DoMouseWheel(nFlags, zDelta, pt);
	return false;
}


//Inserts new chart after the current one and deletes redo list
void CMscGenDoc::InsertNewChart(const CChartData &data)
{
	//The mfc stl implementation changes the value of .end() at insert.
	//Re-set m_itr* to .end() after list manipulation, if it was .end() before
	//or if we delete it now
	bool saved_chart_is_deleted = m_itrSaved == m_charts.end();
	bool shown_chart_is_deleted = m_itrShown == m_charts.end();
	IChartData i;
	for (i = ++IChartData(m_itrEditing); i!=m_charts.end(); i++) {
		if (i == m_itrSaved) saved_chart_is_deleted = true;
		if (i == m_itrShown) shown_chart_is_deleted = true;
	}
	m_charts.erase(++IChartData(m_itrEditing), m_charts.end());
	m_charts.push_back(data);
	m_itrEditing = --m_charts.end();
	if (saved_chart_is_deleted) 
		m_itrSaved = m_charts.end();
	if (shown_chart_is_deleted) 
		m_itrShown = m_charts.end();
}

void CMscGenDoc::SyncShownWithEditing(const CString &action) 
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    if (pApp->m_bFullScreenViewMode) return;
	if (m_itrEditing == m_itrShown) return;
	CString message = "I want to " + action + ", but you have made changes in the text editor.\n";
	if (m_bAttemptingToClose) 
		message.Append("Do you want to include the changes (or permanently loose them)?"); 
	else 
		message.Append("Do you want to include the changes and redraw the chart before I " + action + "?\n");

	if (IDYES == AfxMessageBox(message, MB_ICONQUESTION | MB_YESNO)) 
		CompileEditingChart(true, true);
}

bool CMscGenDoc::CheckIfChanged()
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    if (pApp->m_bFullScreenViewMode) goto not_modified;
	if (m_itrSaved == m_charts.end()) goto modified;
	if (IsEmbedded()) {
        if (m_uSavedFallbackResolution != pApp->m_uFallbackResolution) goto modified;
        if (m_bSavedPageBreaks != pApp->m_bPageBreaks) goto modified;
    }
	if (m_itrSaved != m_itrEditing) {
		if (m_itrSaved->GetText() != m_itrEditing->GetText()) goto modified;
		if (IsEmbedded()) {
			if (m_uSavedPage != m_ChartShown.GetPage()) goto modified;
			if (m_itrSaved->GetDesign() != m_itrEditing->GetDesign()) goto modified;
            if (!(m_itrSaved->GetForcedEntityCollapse() == m_itrEditing->GetForcedEntityCollapse())) goto modified;
            if (!(m_itrSaved->GetForcedArcCollapse() == m_itrEditing->GetForcedArcCollapse())) goto modified;
		}
	}
not_modified:
	SetModifiedFlag(FALSE);
	return false;
modified:
	SetModifiedFlag(TRUE);
    return true;
}

void CMscGenDoc::OnExternalEditorChange(const CChartData &data) 
{
	SetTrackMode(false);
	InsertNewChart(data);
	CompileEditingChart(true, false);

	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	if (pApp->IsInternalEditorRunning())
		pApp->m_pWndEditor->m_ctrlEditor.UpdateText(m_itrEditing->GetText(), m_itrEditing->m_sel, true);
}

void CMscGenDoc::OnInternalEditorChange()
{
	//SetTrackMode invalidates if tracking is turned off
	//It may be that tracking was already off, but we ahd track rects shown (e.g., due to displaying a warning or error)
	//In that case we meed to invalidate (clear the retcs) here.
	if (m_bTrackMode) 
		SetTrackMode(false);
	else 
		StartFadingAll();
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	ASSERT(pApp->IsInternalEditorRunning());
	CString text;
	pApp->m_pWndEditor->m_ctrlEditor.GetWindowText(text);
	CHARRANGE cr;
	pApp->m_pWndEditor->m_ctrlEditor.GetSel(cr);
	//Store new stuff only if either text or selection changes
	//Usually no notification of this sort arrives when only sel changes
	//But when it does (e.g., at multi-line TAB identation) we store a new version just for the selection
	if (text != m_itrEditing->GetText() || cr.cpMax != m_itrEditing->m_sel.cpMax || cr.cpMin != m_itrEditing->m_sel.cpMin) {
        CChartData chart(text, cr, m_itrEditing->GetDesign());
        chart.ForceEntityCollapse(m_itrEditing->GetForcedEntityCollapse());
		InsertNewChart(chart);
		CheckIfChanged();     
	}
}

//User changes selection in internal editor
//This is not called if a mouse move changes the selection during tack mode
void CMscGenDoc::OnInternalEditorSelChange()
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    m_saved_charrange.cpMin = -1;
	if (!m_bTrackMode || !pApp->IsInternalEditorRunning()) return;
	long start, end;
	pApp->m_pWndEditor->m_ctrlEditor.GetSel(start, end);
	int line, col;
	pApp->m_pWndEditor->m_ctrlEditor.ConvertPosToLineCol(start, line, col);
	//Add new track arc and update the views if there is a change
	StartFadingAll();
    AddAnimationElement(AnimationElement::TRACKRECT,
                        m_ChartShown.GetArcByLine(line+1, col+1)); 
}

class AbortCompilingException : public std::exception
{
};


UINT CompileThread( LPVOID pParam )
{
    CMscGenDoc *pDoc = (CMscGenDoc*)pParam;
    pDoc->m_SectionCompiling.Lock();
    try {
        pDoc->m_ChartCompiling.CompileIfNeeded();
    } catch (AbortCompilingException) {
        pDoc->m_ChartCompiling.Delete();
        pDoc->m_killCompilation = false;
        pDoc->m_SectionCompiling.Unlock();
        return 1;

    }
    pDoc->m_SectionCompiling.Unlock();
    if (pDoc->m_hMainWndToSignalCompilationEnd)
        PostMessage(pDoc->m_hMainWndToSignalCompilationEnd, WM_APP+293, 0, 0);
    return 0;
}

bool ProgressCallbackBlocking(double percent, void *data) throw(AbortCompilingException)
{
    CMscGenDoc *pDoc = (CMscGenDoc *)data;
    pDoc->m_Progress = percent/100;
    if (pDoc->m_killCompilation) 
        throw (AbortCompilingException());
    pDoc->DoFading();
	POSITION pos = pDoc->GetFirstViewPosition();
	while(pos) {
		CMscGenView* pView = dynamic_cast<CMscGenView*>(pDoc->GetNextView(pos));
        if (pView) {
            pView->Invalidate();
            pView->ForceRepaint();
        }
	}

    return true;
}

bool ProgressCallbackNonBlocking(double percent, void *data) throw(AbortCompilingException)
{
    CMscGenDoc *pDoc = (CMscGenDoc *)data;
    pDoc->m_Progress = percent/100;
    if (pDoc->m_killCompilation) 
        throw (AbortCompilingException());
    return true;
}

void CMscGenDoc::CompileEditingChart(bool resetZoom, bool block)
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);

    //Change showing only if not viewing in full screen mode
    if (pApp->m_bFullScreenViewMode && m_charts.size()>1) return;

    m_itrEditing->RemoveSpacesAtLineEnds();
	if (pApp->IsInternalEditorRunning()) {
		//Adjust text in the internal editor
		int lStartLine, lStartCol, lEndLine, lEndCol;
		//Save selection in terms of line:column values
		//pApp->m_pWndEditor->m_ctrlEditor.GetSelLineCol(lStartLine, lStartCol, lEndLine, lEndCol);
		//Apply selection in those terms
		//pApp->m_pWndEditor->m_ctrlEditor.UpdateText(m_itrEditing->GetText(), lStartLine, lStartCol, lEndLine, lEndCol, true);
		//Save new selection in character index terms
		pApp->m_pWndEditor->m_ctrlEditor.GetSel(m_itrEditing->m_sel);
	}

    //This can be nil if we are shutting down...
    CMainFrame *pWnd = m_bAttemptingToClose ? NULL : dynamic_cast<CMainFrame *>(AfxGetMainWnd());

    //Prepare m_ChartCompiling
    m_itrShown = m_itrEditing;
    if (pWnd) {
        //See if we have the (potentially) new ForcedDesign verified & copied to the combo box of DesignBar            
        if (!pWnd->FillDesignComboBox(m_itrShown->GetDesign(), true)) 
            m_itrShown->SetDesign("");
    }

    KillCompilation();

    m_ChartCompiling = *m_itrShown;
    if (m_page_serialized_in>=0) 
        m_ChartCompiling.SetPage(m_page_serialized_in);
    else 
        m_ChartCompiling.SetPage(m_ChartShown.GetPage());
    m_page_serialized_in = -1;
    m_ChartCompiling.SetFallbackResolution(m_ChartShown.GetFallbackResolution());
    m_ChartCompiling.SetPageBreaks(m_ChartShown.GetPageBreaks());
    if (pWnd) {
        const CDrawingChartData::ECacheType should_be = pWnd->m_at_embedded_object_category ? 
                        CDrawingChartData::CACHE_EMF : CDrawingChartData::CACHE_RECORDING;
        if (pWnd && pWnd->m_at_embedded_object_category && 
            m_ChartShown.GetCacheType()!=CDrawingChartData::CACHE_EMF)
            m_highlight_fallback_images = true;
        m_ChartCompiling.SetCacheType(should_be);
    }
    m_ChartCompiling.SetProgressCallback(block ? ProgressCallbackBlocking : ProgressCallbackNonBlocking, this);
    m_ChartCompiling.SetPedantic(pApp->m_Pedantic);
    m_ChartCompiling.SetDesignsShapes(&pApp->m_Designs, &pApp->m_Shapes, 
                                      &pApp->m_DesignErrors);
    m_ChartCompiling.SetCopyRightText(pApp->m_CopyrightText);
    if (!pApp->m_bAutoPaginate)
        m_ChartCompiling.SetPageSize(XY(0,0));
    else if (pApp->m_iScale4Pagination<=0) 
        m_ChartCompiling.SetPageSize(pApp->GetPrintablePaperSize());
    else 
        m_ChartCompiling.SetPageSize(pApp->GetPrintablePaperSize()*(100./pApp->m_iScale4Pagination));
    m_ChartCompiling.SetAddHeading(pApp->m_bAutoHeading);
    m_ChartCompiling.SetFitWidth(pApp->m_iScale4Pagination == -1);
    m_ChartCompiling.m_load_data = pApp->GetProfileString(REG_SECTION_SETTINGS, REG_KEY_LOAD_DATA);

	//All tracking rectangles, but add grey area
	SetTrackMode(false);
    m_animations.clear();
    m_controlsShowing.clear();
    AddAnimationElement(AnimationElement::COMPILING_GREY);
    
    m_hadArrangeViews = resetZoom;    
    m_killCompilation = false;
    m_Progress = 0;
    if (block)
        m_hMainWndToSignalCompilationEnd = NULL;
    else
        m_hMainWndToSignalCompilationEnd = *pWnd;
    
    //Disable scroll bars
	POSITION pos = GetFirstViewPosition();
	while(pos) {
		CMscGenView* pView = dynamic_cast<CMscGenView*>(GetNextView(pos));
        if (pView) {
            pView->EnableScrollBarCtrl(SB_BOTH, FALSE);
            pView->UpdateWindow();
        }
	}

    if (!block) {
        //Fire up compilation thread
        m_pCompilingThread = AfxBeginThread(CompileThread, (LPVOID)this);

        //Wait till compilation truly begins
        while (m_Progress==0 && !m_ChartCompiling.IsCompiled())
            Sleep(10); 

        //If there is an internal editor, reset focus to it.
        if (pApp->IsInternalEditorRunning()) 
            pApp->m_pWndEditor->m_ctrlEditor.SetFocus();
    } else {
        CWaitCursor wait;
        m_ChartCompiling.CompileIfNeeded();    
        CompleteCompilingEditingChart();
    }
}



void CMscGenDoc::CompleteCompilingEditingChart()
{
    m_SectionCompiling.Lock();
    _ASSERT(m_ChartCompiling.IsCompiled());
    if (m_ChartCompiling.m_load_data.GetLength())
        AfxGetApp()->WriteProfileString(REG_SECTION_SETTINGS, REG_KEY_LOAD_DATA, 
                                        m_ChartCompiling.m_load_data);
    m_ChartShown.swap(m_ChartCompiling);
    m_ChartCompiling.Delete();
    m_pCompilingThread = NULL;
    m_killCompilation = false;
    m_SectionCompiling.Unlock();

    if (CheckIfChanged()) {
        NotifyChanged();
        if (IsEmbedded())
            m_itrSaved = m_itrShown;
    }

    if (m_animations.size()!=1 || m_animations.begin()->what!=AnimationElement::COMPILING_GREY)
        _ASSERT(0);
    if (m_bAttemptingToClose) return;
    StartFadingAll(); //start fading the grey area
    CMainFrame *pWnd = dynamic_cast<CMainFrame *>(AfxGetMainWnd());
    if (pWnd) {
        pWnd->FillDesignComboBox(m_ChartShown.GetDesign(), true);
        pWnd->FillPageComboBox(m_ChartShown.GetPages(), m_ChartShown.GetPage());
        pWnd->FillZoomComboBox(m_zoom);
        //If we show the WMF, let us update the embedded object size shown in the ribbon
        if (m_ChartShown.GetCacheType() == CDrawingChartData::CACHE_EMF
            && pWnd->m_at_embedded_object_category) {
                const size_t size = m_ChartShown.GetWMFSize() + serialize_doc_overhead + 
                                    m_ChartShown.GetText().GetLength();
                const double total = double(m_ChartShown.GetSize().cx)*m_ChartShown.GetSize().cy;
                pWnd->FillEmbeddedPanel(size, 100*m_ChartShown.GetWMFFallbackImagePos().GetArea() / total);
        }
        if (m_highlight_fallback_images) {
            StartTrackFallbackImageLocations(m_ChartShown.GetWMFFallbackImagePos());
            m_highlight_fallback_images = false;
        }
    }
	//SetZoom(); //reset toolbar
    if (m_hadArrangeViews)
        ArrangeViews();
    UpdateAllViews(NULL);
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	//Display error messages
	if (pApp->m_pWndOutputView) 
        pApp->m_pWndOutputView->ShowCompilationErrors(m_ChartShown);
    //If there is an internal editor, reset focus to it.
    if (pApp->IsInternalEditorRunning()) 
        pApp->m_pWndEditor->m_ctrlEditor.SetFocus();
}


//Kill any ongoing compilation - a blocking call.
void CMscGenDoc::KillCompilation()
{
    m_killCompilation = true; //this causes the thread to stop
    m_SectionCompiling.Lock(); //wait for thread to exit
    m_ChartCompiling.Delete();
    m_pCompilingThread = NULL;
    m_killCompilation = false;
    m_SectionCompiling.Unlock();
    if (m_bAttemptingToClose) return;
    StartFadingAll(); //start fading the grey area
    //If there is an internal editor, reset focus to it.
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    if (pApp->IsInternalEditorRunning()) 
        pApp->m_pWndEditor->m_ctrlEditor.SetFocus();
}

void CMscGenDoc::StartFadingTimer() 
{
	POSITION pos = GetFirstViewPosition();
	while(pos) 
		if (m_pViewFadingTimer == GetNextView(pos)) return;
	//No timer
	pos = GetFirstViewPosition();
	CMscGenView *pView = dynamic_cast<CMscGenView *>(GetNextView(pos));
	if (pView)
		pView->StartFadingTimer();
}

//return false if no fading rect remained and we need to maintain timer
bool CMscGenDoc::DoFading()
{
    if (m_animations.size()==0) 
        return false;
    for (auto ta = m_animations.begin(); ta != m_animations.end(); /*nope*/) {
        switch (ta->status) {
            case AnimationElement::SHOWING:
                if (ta->fade_delay>0) 
                    ta->fade_delay = std::max(0, ta->fade_delay-FADE_TIMER);
                if (ta->fade_delay==0) 
                    ta->status = AnimationElement::FADING;
                ta++;
                break;
            case AnimationElement::APPEARING:
                ta->fade_value += double(FADE_TIMER)/ta->appe_time;
                if (ta->fade_value >= 1) {
                    ta->fade_value = 1;
                    ta->status = AnimationElement::SHOWING;
                }
                ta++;
                break;
            case AnimationElement::FADING:
                ta->fade_value -= double(FADE_TIMER)/ta->disa_time;
                if (ta->fade_value > 0) {
                    ta++;
                    break;
                }
                /*Fallthrough*/
            case AnimationElement::OFF:
                //if a control, remove from m_controlsShowing
                if (ta->what==AnimationElement::CONTROL) 
                    for (auto ii=m_controlsShowing.begin(); ii!=m_controlsShowing.end(); /*nope*/)
                        if (ii->second == ta->arc) m_controlsShowing.erase(ii++);
                        else ii++;
                ta = m_animations.erase(ta);
                break;
		}
    }
	POSITION pos = GetFirstViewPosition();
	while(pos) {
		CMscGenView* pView = dynamic_cast<CMscGenView*>(GetNextView(pos));
		if (pView) pView->Invalidate();
	}
	return m_animations.size()>0;
}


//Add a tracking element to the list. Updates Views if needed & rets ture if so
bool CMscGenDoc::AddAnimationElement(AnimationElement::ElementType type, 
                                     Element *arc, int delay)
{
    if (m_pCompilingThread) return false;
    //sanitize input & find drawing area
    const Contour *draw;
    switch (type) {
    case AnimationElement::COMPILING_GREY:
        _ASSERT(arc==NULL);
        arc = NULL;
        draw = NULL; 
        break;
    case AnimationElement::FALLBACK_IMAGE:
        _ASSERT(arc==NULL);
        arc = NULL;
        if (m_fallback_image_location.IsEmpty()) 
            return false;
        draw = &m_fallback_image_location; 
        break;
    case AnimationElement::CONTROL:
    case AnimationElement::TRACKRECT:
        //Do not add if it has no visual element
        if (arc==NULL) return false;
        draw = &arc->GetAreaToDraw(); 
        if (draw->IsEmpty()) 
            return false;
        break;
    default:
        _ASSERT(0);
    }

    //Look for this arc. If already on list and still fully visible return false - no need to update
	bool found = false;
	for (auto i = m_animations.begin(); i!=m_animations.end(); i++) 
        if (i->what == type && i->arc == arc) {
			i->fade_delay = delay;
			if (i->status == AnimationElement::SHOWING) 
				return false; //we found and it is fully highlighted
			else {
				i->status = AnimationElement::APPEARING;
				found = true;
			}
		} 
 
    //We always redraw all the tracked rectangles, to look better
	if (!found) {
        if (arc)
            m_animations.push_back(AnimationElement(arc, type, delay));
        else {
            m_animations.push_back(AnimationElement(type, delay));
            if (type==AnimationElement::FALLBACK_IMAGE)
                m_animations.rbegin()->fade_value = 1;
        }
        if (type == AnimationElement::CONTROL) 
            m_controlsShowing[arc->GetControlLocation()] = arc;
    } 
    POSITION pos = GetFirstViewPosition();
	while(pos) {
		CMscGenView* pView = dynamic_cast<CMscGenView*>(GetNextView(pos));
		if (pView) pView->Invalidate();
	}
    StartFadingTimer();
	return true;
}

void CMscGenDoc::StartTrackFallbackImageLocations(const Contour &c)
{
    m_fallback_image_location = c;
    AddAnimationElement(AnimationElement::FALLBACK_IMAGE, 0);
    StartFadingAll();
}


//Start the fading process for all rectangles (even for delay<0)
void CMscGenDoc::StartFadingAll(AnimationElement::ElementType type, const Element *except) 
{
    if (m_pCompilingThread) return;
    for (auto i = m_animations.begin(); i!=m_animations.end(); i++) 
        if (type == i->what && i->arc == except) 
            continue;
        else
            i->status = AnimationElement::FADING;
	//If fading in progress we exit
	POSITION pos = GetFirstViewPosition();
	while(pos) 
		if (m_pViewFadingTimer == GetNextView(pos)) return;
	//else we start fading immediately
	if (DoFading())
        StartFadingTimer();
}


void CMscGenDoc::SetTrackMode(bool on)
{
	if (on == m_bTrackMode) return;
    if (m_pCompilingThread && on) return;
	m_bTrackMode = on;
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	StartFadingAll(); //Delete trackrects from screen (even if turned on)
	if (on) {
		SyncShownWithEditing("turn tracking on");
		//We have already saved the internal editor selection state into m_saved_charrange in MscGenView::OnLButtonUp
	} else if (pApp->IsInternalEditorRunning()) {
        //Disable selection change events - we are causing selection change
        //and we do not want to be notified (and add more track rects)
        DWORD eventmask = pApp->m_pWndEditor->m_ctrlEditor.GetEventMask();
        pApp->m_pWndEditor->m_ctrlEditor.SetEventMask(eventmask & ~ENM_SELCHANGE);
        pApp->m_pWndEditor->m_ctrlEditor.SetSel(m_saved_charrange);
        pApp->m_pWndEditor->m_ctrlEditor.SetEventMask(eventmask);
        m_saved_charrange.cpMin = -1;
        pApp->m_pWndEditor->SetFocus();
    }
}

//Expects the coordinates in Msc space (MscBase::totalWidth & Height, 
//except local to the current page)
//If m_bTrackMode is on, we do tracking rectangles
//if not, we only check for controls
void CMscGenDoc::UpdateTrackRects(CPoint mouse)
{
    if (m_pCompilingThread) return; //skip if compiling
    //Start fading all controls and track rectangles, except the one under cursor
	Element *arc = m_ChartShown.GetArcByCoordinate(mouse);
	StartFadingAll(AnimationElement::TRACKRECT, arc); 
    //re-add those controls which are under mouse
    for (auto i = m_controlsShowing.begin(); i!=m_controlsShowing.end(); i++)
        if (inside(i->first.IsWithin(XY(mouse.x, mouse.y))))
            AddAnimationElement(AnimationElement::CONTROL, i->second);
    if (arc && arc->GetControls().size()) 
        AddAnimationElement(AnimationElement::CONTROL, arc);
    //if we do not display tracking rectangles, exit
	if (!m_bTrackMode) return;
    //Re-add tracking rectangle under cursor
    if (arc) 
        AddAnimationElement(AnimationElement::TRACKRECT, arc);
	//If arc has not changed, do nothing
	if (arc == m_last_arc) 
		return;
	m_last_arc = arc;
	//Now update selection in internal editor
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	if (!pApp->IsInternalEditorRunning()) return;
	CCshRichEditCtrl &editor = pApp->m_pWndEditor->m_ctrlEditor;
	DWORD eventmask = editor.GetEventMask();
	//Disable selection change events - we are causing selection change
	//and we do not want to be notified (and add more track rects)
	editor.SetEventMask(eventmask & ~ENM_SELCHANGE);
	if (arc) {
		//Store selection if there was no previous saved selection
        if (m_saved_charrange.cpMin==-1) editor.GetSel(m_saved_charrange);
		HighLightArc(arc);
	} else {
		//restore selection to the one before tracking was initiated
		editor.SetSel(m_saved_charrange);
        m_saved_charrange.cpMin=-1;
	}
	editor.SetEventMask(eventmask);
}

void CMscGenDoc::HighLightArc(const Element *arc)
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	if (!pApp->IsInternalEditorRunning()) return;
	CCshRichEditCtrl &editor = pApp->m_pWndEditor->m_ctrlEditor;
	long start = editor.ConvertLineColToPos(arc->file_pos.start.line-1, arc->file_pos.start.col-1);
	long end = editor.ConvertLineColToPos(arc->file_pos.end.line-1, arc->file_pos.end.col);
	editor.SetRedraw(false);
	editor.SetSel(start, start);
	POINT scroll_pos;
	::SendMessage(editor.m_hWnd, EM_GETSCROLLPOS, 0, (LPARAM)&scroll_pos);
	editor.SetSel(start, end);
	::SendMessage(editor.m_hWnd, EM_SETSCROLLPOS, 0, (LPARAM)&scroll_pos);
	editor.SetRedraw(true);
	editor.Invalidate();
}

bool CMscGenDoc::OnControlClicked(Element *arc, EGUIControlType t)
{
    if (arc==NULL || t==MSC_CONTROL_INVALID) return false;
    if (m_pCompilingThread) return false; //skip if compiling
    CChartData chart(*m_itrEditing);
    EntityApp *ed = dynamic_cast<EntityApp*>(arc);
    bool changed = false;
    if (ed) {
        if (t==MSC_CONTROL_COLLAPSE) changed |= chart.ForceEntityCollapse(ed->name, true);
        else if (t==MSC_CONTROL_EXPAND) changed |= chart.ForceEntityCollapse(ed->name, false);
    } 
    ArcBase *ab = dynamic_cast<ArcBase *>(arc);
    if (ab) {
        const ArcSignature *sig = ab->GetSignature();
        if (sig) switch (t) {
        case MSC_CONTROL_COLLAPSE:
            changed |= chart.ForceArcCollapse(*sig, BOX_COLLAPSE_COLLAPSE);
            break;
        case MSC_CONTROL_EXPAND:
            changed |= chart.ForceArcCollapse(*sig, BOX_COLLAPSE_EXPAND);
            break;
        case MSC_CONTROL_ARROW:
            changed |= chart.ForceArcCollapse(*sig, BOX_COLLAPSE_BLOCKARROW);
            break;
        }
    }
    if (!changed) return false;
	InsertNewChart(chart);
    CompileEditingChart(true,false);
    return true;
}

void CMscGenDoc::OnChangeRibbonCategory(bool embedded)
{
    if (m_pCompilingThread) return;
    if (embedded) {
        m_ChartShown.SetCacheType(CDrawingChartData::CACHE_EMF);
        ReDrawEMF();
    } else {
        m_ChartShown.SetCacheType(CDrawingChartData::CACHE_RECORDING);
        m_ChartShown.CompileIfNeeded(); //just redraws recording
        UpdateAllViews(NULL);
    }
}

void CMscGenDoc::ReDrawEMF()
{
    if (!m_ChartShown.CompileIfNeeded()) //just redraws EMF
        return; //if no redraw happened
    if (m_ChartShown.GetCacheType()!=CDrawingChartData::CACHE_EMF)
        return;
    StartTrackFallbackImageLocations(m_ChartShown.GetWMFFallbackImagePos());
    CMainFrame *pWnd = dynamic_cast<CMainFrame *>(AfxGetMainWnd());
    if (pWnd) {
        const size_t size = m_ChartShown.GetWMFSize() + serialize_doc_overhead + 
                            m_ChartShown.GetText().GetLength();
        const double total = double(m_ChartShown.GetSize().cx)*m_ChartShown.GetSize().cy;
        pWnd->FillEmbeddedPanel(size, 100*m_ChartShown.GetWMFFallbackImagePos().GetArea() / total);
    }
    UpdateAllViews(NULL);
    NotifyChanged();
}


void CMscGenDoc::OnHelpHelp()
{
	char buff[1024]; 
	GetModuleFileName(NULL, buff, 1024);
	std::string dir(buff);
	unsigned pos = dir.find_last_of('\\');
	ASSERT(pos!=std::string::npos);
	string cmdLine = "\"" + dir.substr(0,pos) + "\\msc-gen.chm\"";

	ShellExecute(NULL, NULL, cmdLine.c_str(), NULL, NULL, SW_SHOW); 
}




