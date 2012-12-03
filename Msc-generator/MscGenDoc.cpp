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

TrackedArc::TrackedArc(TrackableElement *a, ElementType et, int delay, 
                       int appear, int disappear) :
    arc(a), what(et), status(APPEARING), fade_value(0), fade_delay(delay),
    appe_time(appear), disa_time(disappear)
{
}


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
	ON_COMMAND(ID_HELP_HELP, &CMscGenDoc::OnHelpHelp)
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

	m_charts.push_back(m_ChartShown);
	m_itrEditing = m_charts.begin();
	m_itrShown = m_charts.end();
	m_itrSaved = m_itrEditing; //start as unmodified
	m_bAttemptingToClose = false;
    serialize_doc_overhead = 0;
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
    unsigned forced_page = m_ChartShown.GetPage(); //dummy
    SerializePage(ar, forced_page, false);  //do not use forced_page
}

void CMscGenDoc::SerializePage(CArchive& ar, unsigned &forced_page, bool force_page)
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	if (ar.IsStoring()) {
        SerializeHelper(ar, m_ChartShown, forced_page, force_page);
    } else {
        CChartData chart;
        SerializeHelper(ar, chart, forced_page, force_page);
        InsertNewChart(chart);
	} /* not IsStoring */
}


//if "force_page" is true, we force a certain page (or all) stored in "forced_page" (only at store)
//else we use the page in "chart"
#define NEW_VERSION_STRING "@@@Msc-generator later than 2.3.4"
void CMscGenDoc::SerializeHelper(CArchive& ar, CChartData &chart, unsigned &forced_page, bool force_page) 
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	if (ar.IsStoring()) {
		ar << CString(NEW_VERSION_STRING); //if format starts with this string, we have file version afterwards
		ar << unsigned(5); //file format version
		ar << chart.GetDesign();
		ar << (force_page ? forced_page : chart.GetPage());
		ar << chart.GetText();
        ar << (force_page ? forced_page : chart.GetPage()); //dummy
        ar << unsigned(LIBMSCGEN_MAJOR);
        ar << unsigned(LIBMSCGEN_MINOR);
        ar << unsigned(LIBMSCGEN_SUPERMINOR);
        ar << unsigned(chart.GetForcedArcCollapse().size());
        for (auto i = chart.GetForcedArcCollapse().begin(); i!=chart.GetForcedArcCollapse().end(); i++) {
            ar << unsigned(i->first.file_pos.start.file);
            ar << unsigned(i->first.file_pos.start.line);
            ar << unsigned(i->first.file_pos.start.col);
            ar << unsigned(i->first.file_pos.end.file);
            ar << unsigned(i->first.file_pos.end.line);
            ar << unsigned(i->first.file_pos.end.col);
            ar << unsigned(i->second);
        }
        ar << unsigned(chart.GetForcedEntityCollapse().size());
        for (auto i = chart.GetForcedEntityCollapse().begin(); i!=chart.GetForcedEntityCollapse().end(); i++)
            ar << CString(i->first.c_str()) << unsigned(i->second);
        ar << pApp->m_uFallbackResolution;
        ar << pApp->m_bPageBreaks;
	} else {
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
        chart.SetPage(read_page);
        unsigned force_entity_size, force_arc_size;
        unsigned a=0, b=0, c=0;
        if (file_version >= 4) { //since 3.4.3
            if (force_page)
                ar >> forced_page; //read forced page for links (unused, quite much)
            else 
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
                chart.ForceArcCollapse(as, BoxCollapseType(a));
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
        if (file_version >= 5) { //since 3.5.3
            ar >> pApp->m_uFallbackResolution;
            m_uSavedFallbackResolution = pApp->m_uFallbackResolution;
            ar >> pApp->m_bPageBreaks;
            m_bSavedPageBreaks = pApp->m_bPageBreaks;
            CMainFrame *pMainWnd = dynamic_cast<CMainFrame *>(AfxGetMainWnd());
            if (pMainWnd) {
                CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;
                pMainWnd->m_wndRibbonBar.GetElementsByID(ID_EMBEDDEDOPTIONS_FALLBACK_RES, arButtons);
                _ASSERT(arButtons.GetSize()==1);
                CMFCRibbonSlider *s = dynamic_cast<CMFCRibbonSlider *>(arButtons[0]);
                if (s) s->SetPos(pApp->m_uFallbackResolution);
                
                //pMainWnd->m_wndRibbonBar.GetElementsByID(IDC_CHECK_PB_EMBEDDED, arButtons);
                //_ASSERT(arButtons.GetSize()==1);
                //CMFCButton *b = dynamic_cast<CMFCButton *>(arButtons[0]);
                //if (b) b->SetCheck(pApp->m_bPB_Embedded);

                //pMainWnd->m_wndRibbonBar.GetElementsByID(IDC_CHECK_PB_EDITING, arButtons);
                //_ASSERT(arButtons.GetSize()==1);
                //b = dynamic_cast<CMFCButton *>(arButtons[0]);
                //if (b) b->SetCheck(pApp->m_bPB_Editing);
            }
        }
	} /* not IsStoring */
}

void CMscGenDoc::DeleteContents()
{
	m_charts.clear();
	m_ChartShown = CChartData();
	m_charts.push_back(m_ChartShown);
	m_itrEditing = m_charts.begin();
	m_itrShown = m_charts.end();
	m_itrSaved = m_charts.end(); //start as modified
	COleServerDocEx::DeleteContents();
}

BOOL CMscGenDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
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
	ShowEditingChart(true);
	if (pApp->IsInternalEditorRunning())
		pApp->m_pWndEditor->m_ctrlEditor.UpdateText(m_itrEditing->GetText(), m_itrEditing->m_sel, true);
    CMainFrame *pWnd = dynamic_cast<CMainFrame *>(AfxGetMainWnd());
    if (pWnd) {
        pWnd->FillDesignComboBox(m_itrEditing->GetDesign(), true);
        pWnd->FillPageComboBox(m_ChartShown.GetPages(), m_ChartShown.GetPage());
        pWnd->FillZoomComboBox(m_zoom);
    }
	SetZoom(); //reset toolbar
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
	ShowEditingChart(true);

	// if the app was started only to print, don't set user control
	//Copied from COleLinkingDoc
	if (pApp->m_pCmdInfo == NULL ||
		(pApp->m_pCmdInfo->m_nShellCommand != CCommandLineInfo::FileDDE &&
		 pApp->m_pCmdInfo->m_nShellCommand != CCommandLineInfo::FilePrint))
	{
		AfxOleSetUserCtrl(TRUE);
	}
	CMainFrame *pWnd = dynamic_cast<CMainFrame *>(AfxGetMainWnd());
    if (pWnd) {
        pWnd->FillDesignComboBox(m_itrEditing->GetDesign(), true);
        pWnd->FillPageComboBox(m_ChartShown.GetPages(), m_ChartShown.GetPage());
        pWnd->FillZoomComboBox(m_zoom); //reset toolbar, do not chnage zoom
    }
	if (restartEditor)
		m_ExternalEditor.Start(lpszPathName);
	return TRUE;
}

BOOL CMscGenDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	if (lpszPathName==NULL) {
		SyncShownWithEditing("update the container document");
		return COleServerDocEx::OnSaveDocument(lpszPathName);
	}

	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	//Restart external editor only if file name changes
	bool restartEditor = lpszPathName!=GetPathName() && m_ExternalEditor.IsRunning();
	if (restartEditor) 
		m_ExternalEditor.Stop(STOPEDITOR_WAIT);
	SyncShownWithEditing("save");
	if (!m_itrEditing->Save(lpszPathName)) {
		CFileException e;
		ReportSaveLoadException(lpszPathName, &e,
			TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
		return FALSE;
	}
	if (restartEditor) 
		m_ExternalEditor.Start(lpszPathName);
	m_itrSaved = m_itrEditing;
	CheckIfChanged();     
	return TRUE;
}

void CMscGenDoc::OnCloseDocument()
{
	m_ExternalEditor.Stop(STOPEDITOR_FORCE);
	COleServerDocEx::OnCloseDocument();
}


void CMscGenDoc::OnUpdateFileExport(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_ChartShown.IsEmpty() || !m_itrEditing->IsEmpty());
}

void CMscGenDoc::OnFileExport()
{
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
	if (m_itrEditing->m_wasDrawn)
		ShowEditingChart(true);
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
	if (m_itrEditing->m_wasDrawn)
		ShowEditingChart(true);
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
		InsertNewChart(CChartData(text, m_itrEditing->GetDesign())); //all pages visible
	}
	ShowEditingChart(true);
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
	ShowEditingChart(true);
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
	if (!IsInPlaceActive() && pWnd) 
		pWnd->m_wndStatusBar.SetPaneTextColor(1, m_bTrackMode?RGB(0,0,0):RGB(100,100,100));
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
	else StartFadingAll(NULL);

    //Jump in the internal and external editors
    if (pApp->IsInternalEditorRunning()) 
        pApp->m_pWndEditor->m_ctrlEditor.JumpToLine(line, col);
    if (m_ExternalEditor.IsRunning())
        m_ExternalEditor.JumpToLine(line, col);

    //Show tracking boxes for the error
    AddTrackArc(m_ChartShown.GetArcByLine(line, col), TrackedArc::TRACKRECT);
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
    pApp->m_pWndEditor->m_ctrlEditor.SetForcedDesign(design);
	ShowEditingChart(true);
}

void CMscGenDoc::ChangePage(unsigned page)
{
	if (page == m_ChartShown.GetPage())
		return;
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    //if we are not viewing read-only, then duplicate current chart, loose redo
    if (pApp && !pApp->m_bFullScreenViewMode) 
        InsertNewChart(CChartData(*m_itrEditing)); 
	m_itrEditing->SetPage(page);  //set new page
	ShowEditingChart(true);
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
	//No zooming by ourselves during in-place editing
	if (IsInPlaceActive()) {
		zoom = m_zoom = 100;
		return;
	}

	if (zoom < 1) zoom = m_zoom;
	if (zoom > 10000) zoom = 10000;
	if (zoom < 10) zoom = 10;

	if (zoom == m_zoom) return;
	m_zoom = zoom;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL) {
        CMscGenView* pView = dynamic_cast<CMscGenView*>(GetNextView(pos));
  	    if (pView) {
	        pView->ResyncScrollSize();
			pView->Invalidate();
		}
	}
	CMainFrame *pWnd = dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	if (pWnd)
        pWnd->FillZoomComboBox(m_zoom);
    return;
}


#define SCREEN_MARGIN 50 

void CMscGenDoc::ArrangeViews(EZoomMode mode)
{
	if (mode == NONE) return;
	if (IsInPlaceActive()) return;
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
				//now xy contains is the required client size, 
				//next, we adjust for minimum size
				if (x < 550) x = 580;
				if (y < 300) y = 300;
				if (window.left + x > screen.right - SCREEN_MARGIN)
					window.left = screen.right - SCREEN_MARGIN - x;
				if (window.top + y > screen.bottom - SCREEN_MARGIN)
					window.top = screen.bottom - SCREEN_MARGIN - y;
				pWnd->SetWindowPos(NULL, window.left, window.top, x, y,  SWP_NOZORDER | SWP_NOACTIVATE);
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
}

void CMscGenDoc::OnUpdateZoommodeKeepadjustingwindowwidth(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_ZoomMode == WINDOW_WIDTH);
}

void CMscGenDoc::OnUpdateZoommodeKeepfittingtowidth(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_ZoomMode == ZOOM_WIDTH);
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
		ShowEditingChart(true);
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
			if (m_itrSaved->GetPage() != m_itrEditing->GetPage()) goto modified;
			if (m_itrSaved->GetDesign() != m_itrEditing->GetDesign()) goto modified;
            if (!(m_itrSaved->GetForcedEntityCollapse() == m_itrEditing->GetForcedEntityCollapse())) goto modified;
            if (!(m_itrSaved->GetForcedArcCollapse() == m_itrEditing->GetForcedArcCollapse())) goto modified;
		}
	}
not_modified:
	SetModifiedFlag(FALSE);
	return false;
modified:
    m_uSavedFallbackResolution = pApp->m_uFallbackResolution;
    m_bSavedPageBreaks = pApp->m_bPageBreaks;
	SetModifiedFlag(TRUE);
    return true;
}

void CMscGenDoc::OnExternalEditorChange(const CChartData &data) 
{
	SetTrackMode(false);
	int current_page = m_itrEditing->GetPage();
	InsertNewChart(data);
	m_itrEditing->SetPage(current_page);
	ShowEditingChart(true);

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
		StartFadingAll(NULL);
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
		int page = m_itrEditing->GetPage();
        CChartData chart(text, cr, m_itrEditing->GetDesign(), page);
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
	StartFadingAll(NULL);
	AddTrackArc(m_ChartShown.GetArcByLine(line+1, col+1), TrackedArc::TRACKRECT); 
}

void CMscGenDoc::ShowEditingChart(bool resetZoom)
{
	CWaitCursor wait;
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);

	m_itrEditing->RemoveSpacesAtLineEnds();
	if (pApp->IsInternalEditorRunning()) {
		//Adjust text in the internal editor
		int lStartLine, lStartCol, lEndLine, lEndCol;
		//Save selection in terms of line:column values
		pApp->m_pWndEditor->m_ctrlEditor.GetSelLineCol(lStartLine, lStartCol, lEndLine, lEndCol);
		//Apply selection in those terms
		pApp->m_pWndEditor->m_ctrlEditor.UpdateText(m_itrEditing->GetText(), lStartLine, lStartCol, lEndLine, lEndCol, true);
		//Save new selection in character index terms
		pApp->m_pWndEditor->m_ctrlEditor.GetSel(m_itrEditing->m_sel);
	}

    //Change showing only if not viewing in full screen mode
    if (!pApp->m_bFullScreenViewMode || m_charts.size()==1) {
        m_itrShown = m_itrEditing;
        m_itrShown->m_wasDrawn = true;
        m_ChartShown = *m_itrEditing;
    }

	int max_page = m_ChartShown.GetPages(); //This GetPages compiles
	if (max_page == 1) max_page=0;

	if (max_page<m_ChartShown.GetPage()) {
		m_ChartShown.SetPage(max_page);
		m_itrEditing->SetPage(max_page);
	}
	
	//Display error messages
	if (pApp->m_pWndOutputView && !m_bAttemptingToClose) 
        pApp->m_pWndOutputView->ShowCompilationErrors(m_ChartShown);

	if (!m_bAttemptingToClose) {
        CMainFrame *pWnd = dynamic_cast<CMainFrame *>(AfxGetMainWnd());
        if (pWnd) {
            //See if we have the (potentially) new ForcedDesign verified & copied to the combo box of DesignBar            
            if (!pWnd->FillDesignComboBox(m_ChartShown.GetDesign(), true)) {
                m_ChartShown.SetDesign("");
                m_itrShown->SetDesign("");
            }
            //Update page controls and variables
            pWnd->FillPageComboBox(m_ChartShown.GetPages(), m_ChartShown.GetPage());
		}
	}

	//Abruptly delete all tracking rectangles
    CSingleLock lock(&m_SectionTrackingMembers);
    lock.Lock();
    m_trackArcs.clear();
    lock.Unlock();
	SetTrackMode(false);
    if (CheckIfChanged()) {
        NotifyChanged();
        if (IsEmbedded())
            m_itrSaved = m_itrShown;
    }

	if (!m_bAttemptingToClose) {
		UpdateAllViews(NULL);
		if (resetZoom) ArrangeViews();
		else SetZoom(); //just make sure combo shows right value

		//If there is an internal editor, reset focus to it.
		if (pApp->IsInternalEditorRunning()) pApp->m_pWndEditor->m_ctrlEditor.SetFocus();
	}
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

//return false if no fading rect remained
bool CMscGenDoc::DoFading()
{
    Block bounding; bounding.MakeInvalid();
    CSingleLock lock(&m_SectionTrackingMembers);
    lock.Lock();
	for (auto ta = m_trackArcs.begin(); ta != m_trackArcs.end(); /*nope*/) {
        const Contour &draw_area = ta->arc ? ta->arc->GetAreaToDraw() : m_fallback_image_location;
        switch (ta->status) {
            case TrackedArc::SHOWING:
                if (ta->fade_delay>0) 
                    ta->fade_delay = std::max(0, ta->fade_delay-FADE_TIMER);
                if (ta->fade_delay==0) 
                    ta->status = TrackedArc::FADING;
                ta++;
                break;
            case TrackedArc::APPEARING:
                bounding += draw_area.GetBoundingBox();
                ta->fade_value += double(FADE_TIMER)/ta->appe_time;
                if (ta->fade_value >= 1) {
                    ta->fade_value = 1;
                    ta->status = TrackedArc::SHOWING;
                }
                ta++;
                break;
            case TrackedArc::FADING:
                bounding += draw_area.GetBoundingBox();
                ta->fade_value -= double(FADE_TIMER)/ta->disa_time;
                if (ta->fade_value > 0) {
                    ta++;
                    break;
                }
                /*Fallthrough*/
            case TrackedArc::OFF:
                //if a control, remove from m_controlsShowing
                if (ta->what==TrackedArc::CONTROL) 
                    for (auto ii=m_controlsShowing.begin(); ii!=m_controlsShowing.end(); /*nope*/)
                        if (ii->second == ta->arc) m_controlsShowing.erase(ii++);
                        else ii++;
                ta = m_trackArcs.erase(ta);
                break;
		}
    }
    if (bounding.IsInvalid()) //nothing to update
        return m_trackArcs.size()>0;  //return true if we need to keep the timer
    lock.Unlock();
	POSITION pos = GetFirstViewPosition();
	while(pos) {
		CMscGenView* pView = dynamic_cast<CMscGenView*>(GetNextView(pos));
		if (pView) pView->InvalidateBlock(bounding);
	}
	return true;
}


//Add a tracking element to the list. Updates Views if needed & rets ture if so
bool CMscGenDoc::AddTrackArc(TrackableElement *arc, TrackedArc::ElementType type, int delay)
{
    _ASSERT (type==TrackedArc::FALLBACK_IMAGE || arc);
	//Do not add if it has no visual element
    const Contour &draw = type==TrackedArc::FALLBACK_IMAGE ? m_fallback_image_location : arc->GetAreaToDraw();
    if (type != TrackedArc::CONTROL && draw.IsEmpty()) 
		return false;
	bool found = false;
    Block b; b.MakeInvalid();
	//Look for this arc. If already on list and still fully visible return false - no need to update
    CSingleLock lock(&m_SectionTrackingMembers);
    lock.Lock();
	for (auto i = m_trackArcs.begin(); i!=m_trackArcs.end(); i++) {
		if (i->arc == arc && i->what == type) {
			i->fade_delay = delay;
			if (i->status == TrackedArc::SHOWING) 
				return false; //we found and it is fully highlighted
			else {
				i->status = TrackedArc::APPEARING;
				found = true;
			}
		} 

        b += (i->what == TrackedArc::FALLBACK_IMAGE ? m_fallback_image_location : i->arc->GetAreaToDraw()).GetBoundingBox();
    }
    //We always redraw all the tracked rectangles, to look better
	if (!found) {
        const int appear = type == TrackedArc::TRACKRECT ? 300 : type == TrackedArc::FALLBACK_IMAGE ? 1 : 100;
        const int disapp = type == TrackedArc::TRACKRECT ? 300 : type == TrackedArc::FALLBACK_IMAGE ? 1500 : 100;
		m_trackArcs.push_back(TrackedArc(arc, type, delay, appear, disapp));
        if (type == TrackedArc::CONTROL) {
            _ASSERT(arc);
            m_controlsShowing[arc->GetControlLocation()] = arc;
            b += arc->GetControlLocation();
        } else
            b += draw.GetBoundingBox();
    } 
    if (b.IsInvalid()) 
        return true;
    lock.Unlock();
    POSITION pos = GetFirstViewPosition();
	while(pos) {
		CMscGenView* pView = dynamic_cast<CMscGenView*>(GetNextView(pos));
		if (pView) pView->InvalidateBlock(b);
	}
	return true;
}

void CMscGenDoc::StartTrackFallbackImageLocations(const Contour &c)
{
    m_fallback_image_location = c;
    AddTrackArc(NULL, TrackedArc::FALLBACK_IMAGE, 0);
    StartFadingTimer();
}


//Start the fading process for all rectangles (even for delay<0)
void CMscGenDoc::StartFadingAll(const TrackableElement *except) 
{
    CSingleLock lock(&m_SectionTrackingMembers);
    lock.Lock();
    for (auto i = m_trackArcs.begin(); i!=m_trackArcs.end(); i++) 
        if (i->arc != except)
            i->status = TrackedArc::FADING;
    lock.Unlock();
	//If fading in progress we exit
	POSITION pos = GetFirstViewPosition();
	while(pos) 
		if (m_pViewFadingTimer == GetNextView(pos)) return;
	//else we start fading immediately
	DoFading();
	StartFadingTimer();
}


void CMscGenDoc::SetTrackMode(bool on)
{
	if (on == m_bTrackMode) return;
	m_bTrackMode = on;
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	StartFadingAll(NULL); //Delete trackrects from screen (even if turned on)
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
    //Start fading all controls and track rectangles, except the one under cursor
	TrackableElement *arc = m_ChartShown.GetArcByCoordinate(mouse);
	StartFadingAll(arc); 
    //re-add those controls which are under mouse
    for (auto i = m_controlsShowing.begin(); i!=m_controlsShowing.end(); i++)
        if (inside(i->first.IsWithin(XY(mouse.x, mouse.y))))
            AddTrackArc(i->second, TrackedArc::CONTROL);
    if (arc && arc->GetControls().size()) 
        AddTrackArc(arc, TrackedArc::CONTROL);
    //if we do not display tracking rectangles, exit
	if (!m_bTrackMode) return;
    //Re-add tracking rectangle under cursor
	AddTrackArc(arc, TrackedArc::TRACKRECT);
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

void CMscGenDoc::HighLightArc(const TrackableElement *arc)
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

bool CMscGenDoc::OnControlClicked(TrackableElement *arc, MscControlType t)
{
    if (arc==NULL || t==MSC_CONTROL_INVALID) return false;
    CChartData chart(*m_itrEditing);
    EntityDef *ed = dynamic_cast<EntityDef*>(arc);
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
    CSingleLock lock(&m_SectionTrackingMembers);
    lock.Lock();
    m_trackArcs.clear();
    m_controlsShowing.clear();
    lock.Unlock();
	InsertNewChart(chart);
    ShowEditingChart(true);
    return true;
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




