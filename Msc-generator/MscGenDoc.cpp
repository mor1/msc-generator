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

    You should have received a copy of the GNU Affero General Public License
    along with Msc-generator.  If not, see <http://www.gnu.org/licenses/>.
*/

// MscGenDoc.cpp : implementation of the CMscGenDoc class
//

#include "stdafx.h"
#include "Msc-generator.h"

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
	bool a = CDialog::OnInitDialog();
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
	ON_COMMAND(ID_EDIT_COPYENTIRECHART, OnEditCopyEntireChart)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPYENTIRECHART, OnUpdateFileExport)
	ON_COMMAND(ID_EDIT_PASETENTIRECHART, OnEditPasteEntireChart)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASETENTIRECHART, OnUpdateEditPasteEntireChart)
	ON_COMMAND(ID_EDIT_UPDATE, OnEditUpdate)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UPDATE, OnUdpateEditUpdate)
	ON_COMMAND(ID_FILE_EXPORT, OnFileExport)
	ON_LBN_SELCHANGE(IDC_OUTPUT_LIST, OnSelChange)
	ON_COMMAND(ID_DESIGN_ZOOM, OnDesignZoom)
	ON_CBN_SELENDOK(ID_DESIGN_ZOOM, OnDesignZoom)
	ON_COMMAND(ID_DESIGN_PAGE, OnDesignPage)
	ON_CBN_SELENDOK(ID_DESIGN_PAGE, OnDesignPage)
	ON_UPDATE_COMMAND_UI(ID_DESIGN_PAGE, OnUpdateDesignPage)
	ON_COMMAND(ID_DESIGN_DESIGN, OnDesignDesign)
	ON_CBN_SELENDOK(ID_DESIGN_DESIGN, OnDesignDesign)
	ON_UPDATE_COMMAND_UI(ID_DESIGN_PAGE, OnUpdateDesignDesign)
	ON_COMMAND(ID_VIEW_NEXTERROR, &CMscGenDoc::OnViewNexterror)
	ON_COMMAND(ID_VIEW_PREVERROR, &CMscGenDoc::OnViewPreverror)
	ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomin)
	ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomout)
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
	m_itrDoNotSyncForThis = m_charts.end();
	m_bAttemptingToClose = false;
}

CMscGenDoc::~CMscGenDoc()
{
}

COleServerItem* CMscGenDoc::OnGetEmbeddedItem()
{
	// OnGetEmbeddedItem is called by the framework to get the COleServerItem
	//  that is associated with the document.  It is only called when necessary.

	CMscGenSrvrItem* pItem = new CMscGenSrvrItem(this);
	ASSERT_VALID(pItem);
	return pItem;
}

CDocObjectServer *CMscGenDoc::GetDocObjectServer(LPOLEDOCUMENTSITE pDocSite)
{
	return new CDocObjectServer(this, pDocSite);
}

COleIPFrameWnd *CMscGenDoc::CreateInPlaceFrame(CWnd *pParentWnd) 
{
	m_itrDoNotSyncForThis = m_charts.end();
	m_bAttemptingToClose = false;
	return COleServerDocEx::CreateInPlaceFrame(pParentWnd);
}

void CMscGenDoc::DestroyInPlaceFrame(COleIPFrameWnd* pFrameWnd)
{
	// Stop editor
	m_bAttemptingToClose = true;
	m_ExternalEditor.Stop(STOPEDITOR_WAIT);
	SyncShownWithEditing("exit in-place editing");
	//If the user selected not to sync, we save this itrEditing and will not ask again
	//This is to avoid popping up a sync question window long after the user has 
	//exited in-place editing in the container app
	if (m_itrEditing != m_itrShown) 
		m_itrDoNotSyncForThis = m_itrEditing;
	COleServerDocEx::DestroyInPlaceFrame(pFrameWnd);
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
#define NEW_VERSION_STRING "@@@Msc-generator later than 2.3.4"
void CMscGenDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) {
		ar << CString(NEW_VERSION_STRING);
		ar << unsigned(2); //file format version
		ar << m_ChartShown.GetDesign();
		ar << m_ChartShown.GetPage();
		ar << m_ChartShown.GetText();
        ar << unsigned(m_ChartShown.GetForcedEntityCollapse().size());
        for (auto i = m_ChartShown.GetForcedEntityCollapse().begin(); i!=m_ChartShown.GetForcedEntityCollapse().end(); i++)
            ar << CString(i->first.c_str()) << unsigned(i->second);
	} else {
		CString text;
		CString design;
		unsigned page;
		ar >> design;
        unsigned file_version = -1;
		if (design != NEW_VERSION_STRING) {
			//Old file format
			ar >> page;
			unsigned alloc = 16384;
			unsigned length = 0;
			char *buff = (char*)malloc(alloc);
			while (1) {
				length += ar.Read(buff+length, alloc-1-length);
				if (length == alloc-1)
					buff = (char*)realloc(buff, alloc+=16384);
				else break;
			} 
			buff[length] = 0;
			text = buff;
			free(buff);
		} else {
			//New file format
			ar >> file_version;
			switch (file_version) {
				case 0:  //since v2.3.4
					char *buff;
					unsigned length;
					ar >> design;
					ar >> page;
					ar >> length;
					buff = (char*)malloc(length+1);
					ar.Read(buff, length);
					buff[length] = 0;
					text = buff;
					free(buff);
					break;
				case 1:  //since ???
				case 2:  //since v3.1
				default: //any future version
					ar >> design;
					ar >> page;
					ar >> text;
					break;
			}
		}
		EnsureCRLF(text);
		ReplaceTAB(text);
		CChartData chart(text, design, page);
        unsigned force_entity_size;
        switch (file_version) {
        case 0:
        case 1: 
            break; //nothing to read besides design, page and text
        default: //any future version 
        case 2:  //since v3.1
            ar >> force_entity_size;
            for (int i=0; i<force_entity_size; i++) {
                CString s; unsigned b;
                ar >> s;
                ar >> b;
                chart.ForceEntityCollapse(string(s), bool(b));
            }
            break;
        }
        InsertNewChart(chart);
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
	bool ret = COleServerDocEx::CanCloseFrame(pFrame);
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
	CheckIfChanged(); 
	if (pApp->IsInternalEditorRunning())
		pApp->m_pWndEditor->m_ctrlEditor.UpdateText(m_itrEditing->GetText(), m_itrEditing->m_sel, true);
	pApp->FillDesignDesignCombo(m_itrEditing->GetDesign(), true);
	pApp->FillDesignPageCombo(m_ChartShown.GetPages(), m_ChartShown.GetPage());
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
	m_itrDoNotSyncForThis = m_charts.end();
	CheckIfChanged();     

	// if the app was started only to print, don't set user control
	//Copied from COleLinkingDoc
	if (pApp->m_pCmdInfo == NULL ||
		(pApp->m_pCmdInfo->m_nShellCommand != CCommandLineInfo::FileDDE &&
		 pApp->m_pCmdInfo->m_nShellCommand != CCommandLineInfo::FilePrint))
	{
		AfxOleSetUserCtrl(TRUE);
	}
	pApp->FillDesignDesignCombo(m_itrEditing->GetDesign(), true);
	pApp->FillDesignPageCombo(m_ChartShown.GetPages(), m_ChartShown.GetPage());
	SetZoom(); //reset toolbar, do not chnage zoom
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
    CString name;
    bool bitmap;
    double x_scale=1, y_scale=1;
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
        name = GetPathName();
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
        if (ext.CompareNoCase(".png")==0 || ext.CompareNoCase(".bmp")==0 || 
            ext.CompareNoCase(".emf")==0 || ext.CompareNoCase(".svg")==0 || 
            ext.CompareNoCase(".pdf")==0 || ext.CompareNoCase(".eps")==0) {
            strcpy_s(filename, name);
            PathRemoveExtension(filename);
            name = filename;
        }
        switch(dialog.m_pOFN->nFilterIndex) {
        case 1: name += ".png"; bitmap=true; break;
        case 2: name += ".bmp"; bitmap=true; break;
        case 3: name += ".emf"; bitmap=false;break;
        case 4: name += ".svg"; bitmap=false;break;
        case 5: name += ".pdf"; bitmap=false;break;
        case 6: name += ".eps"; bitmap=false;break;
        default: _ASSERT(0);
        }
        if (PathFileExists(name)) 
            if (IDNO == MessageBox(0, "File " + name + " exists. Do you want to overwrite?", "Msc-generator", MB_YESNO))
                continue;
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
        AfxGetApp()->WriteProfileInt(REG_SECTION_SETTINGS, "SCALING_SCALE", scale.m_scale*1000);
        AfxGetApp()->WriteProfileInt(REG_SECTION_SETTINGS, "SCALING_X", scale.m_x);
        AfxGetApp()->WriteProfileInt(REG_SECTION_SETTINGS, "SCALING_Y", scale.m_y);
        AfxGetApp()->WriteProfileInt(REG_SECTION_SETTINGS, "SCALING_XY_X", scale.m_xy_x);
        AfxGetApp()->WriteProfileInt(REG_SECTION_SETTINGS, "SCALING_XY_Y", scale.m_xy_y);
    }
    m_ChartShown.DrawToFile(name, x_scale, y_scale);
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
	CMscGenSrvrItem *pItem = reinterpret_cast<CMscGenSrvrItem*>(COleServerDocEx::GetEmbeddedItem());
	pItem->CopyToClipboard();
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
	CheckIfChanged();     
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
			MessageBox(0, "Fail to create external editor window", "Msc-generator", MB_OK);
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
	int maxsel = pOutputView->m_wndOutput.GetCount();
	if (maxsel == LB_ERR || maxsel==0) return; //no error
	int cursel = pOutputView->m_wndOutput.GetCurSel();
	cursel = (cursel + (next ? 1 : -1) + maxsel) % maxsel;
	pOutputView->m_wndOutput.SetCurSel(cursel);

    //Turn tracking off
	if (m_bTrackMode) SetTrackMode(false);
	else StartFadingAll();

    //exit if we do not know the location of the error
    if (cursel >= pOutputView->error_pos.size() || cursel<0) return;
    if (pOutputView->error_pos[cursel].first < 0) return;

    //Jump to that pos in editor
    int line = pOutputView->error_pos[cursel].first;
    int col = pOutputView->error_pos[cursel].second;
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
	int sel = pOutputView->m_wndOutput.GetCurSel();
	if (sel == LB_ERR) return;
	int line = m_ChartShown.GetErrorLine(sel, pApp->m_Warnings);
	int col =  m_ChartShown.GetErrorCol(sel, pApp->m_Warnings);
	if (pApp->IsInternalEditorRunning())
		pApp->m_pWndEditor->m_ctrlEditor.JumpToLine(line, col);
	if (m_ExternalEditor.IsRunning())
		m_ExternalEditor.JumpToLine(line, col);
}

void CMscGenDoc::OnDesignDesign() 
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	CObList list;
	CMFCToolBar::GetCommandButtons(ID_DESIGN_DESIGN, list);
	POSITION p = list.GetHeadPosition();
	if (!p) return;
	CMFCToolBarComboBoxButton *combo = static_cast<CMFCToolBarComboBoxButton*>(list.GetNext(p));
	unsigned index = combo->GetCurSel();
	//Set this setting on oll remaining ones
	while (p) {
		CMFCToolBarComboBoxButton *combo = static_cast<CMFCToolBarComboBoxButton*>(list.GetNext(p));
		combo->SelectItem(index, TRUE);
	}
	CString new_forcedDesign; 
	if (index > 0)
		new_forcedDesign = combo->GetItem(index);
	if (new_forcedDesign == m_itrEditing->GetDesign()) return;
	InsertNewChart(CChartData(*m_itrEditing)); //duplicate current chart, loose undo, keep page shown
	m_itrEditing->SetDesign(new_forcedDesign);
    pApp->m_pWndEditor->m_ctrlEditor.SetForcedDesign(new_forcedDesign);
	ShowEditingChart(true);
	CheckIfChanged();     
}

void CMscGenDoc::OnUpdateDesignDesign(CCmdUI *pCmdUI)
{
    CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    pCmdUI->Enable(pApp->m_SetOfDesigns.GetLength()>0);
}

void CMscGenDoc::OnDesignPage()
{
	CObList list;
	CMFCToolBar::GetCommandButtons(ID_DESIGN_PAGE, list);

	POSITION p = list.GetHeadPosition();
	if (!p) return;
	CMFCToolBarComboBoxButton *combo = static_cast<CMFCToolBarComboBoxButton*>(list.GetNext(p));
	int index = combo->GetCurSel();
	//Set this setting on oll remaining ones
	while (p) {
		CMFCToolBarComboBoxButton *combo = static_cast<CMFCToolBarComboBoxButton*>(list.GetNext(p));
		combo->SelectItem(index);
	}
	if (index == m_ChartShown.GetPage())
		return;
	InsertNewChart(CChartData(*m_itrEditing)); //duplicate current chart, loose undo
	m_itrEditing->SetPage(index);  //set new page
	ShowEditingChart(true);
	CheckIfChanged();     
}

void CMscGenDoc::OnUpdateDesignPage(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_ChartShown.GetPages()>1);
}

void CMscGenDoc::SetZoom(int zoom)
{
	//No zooming by ourselves during in-place editing
	if (IsInPlaceActive()) {
		m_zoom = 100;
		return;
	}

	if (zoom < 1) zoom = m_zoom;
	if (zoom > 10000) zoom = 10000;
	if (zoom < 10) zoom = 10;

	CString text;
	text.Format("%u%%", zoom);
	CObList list;
	CMFCToolBar::GetCommandButtons(ID_DESIGN_ZOOM, list);
	POSITION p = list.GetHeadPosition();
	while (p) {
		static unsigned zoom_values[] = {400, 300, 200, 150, 100, 75, 50, 40, 30, 20, 10, 0};
		CMFCToolBarComboBoxButton *combo = static_cast<CMFCToolBarComboBoxButton*>(list.GetNext(p));
		if (combo->GetCount()!=sizeof(zoom_values)/sizeof(unsigned)-1) {
			for (int i=0; zoom_values[i]>0; i++) {
				CString text;
				text.Format("%u%%", zoom_values[i]);
				combo->AddItem(text, zoom_values[i]);
			}
		}
		combo->SetText(text);
	}
	if (zoom == m_zoom) return;
	m_zoom = zoom;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL) {
        CMscGenView* pView = static_cast<CMscGenView*>(GetNextView(pos));
  	    if (pView->IsKindOf(RUNTIME_CLASS(CMscGenView))) {
	        pView->ResyncScrollSize();
			pView->Invalidate();
		}
	}
    CMainFrame *pWnd = dynamic_cast<CMainFrame *>(AfxGetMainWnd());
    if (pWnd)
        pWnd->SetSplitSize(m_ChartShown.GetHeadingSize()*double(m_zoom)/100.);
}

void CMscGenDoc::OnViewZoomin()
{
	SetZoom(m_zoom*1.1);
}

void CMscGenDoc::OnViewZoomout()
{
	SetZoom(m_zoom/1.1);
}

void CMscGenDoc::OnDesignZoom()
{
	CObList list;
	CMFCToolBar::GetCommandButtons(ID_DESIGN_ZOOM, list);

	POSITION p = list.GetHeadPosition();
	if(!p) return;
	CMFCToolBarComboBoxButton *combo = static_cast<CMFCToolBarComboBoxButton*>(list.GetNext(p));
	CString text = combo->GetText();
	if (!text.CompareNoCase("overview") || !text.CompareNoCase("auto")) {
		ArrangeViews(OVERVIEW);
	} else {
		SetZoom(atoi(text));
	}
}


#define SCREEN_MARGIN 50 

void CMscGenDoc::ArrangeViews(EZoomMode mode)
{
	if (mode == NONE) return;
	if (IsInPlaceActive()) return;
	POSITION pos = GetFirstViewPosition();
	if (pos == NULL) return;
    CMscGenView* pView = static_cast<CMscGenView*>(GetNextView(pos));
  	if (!pView->IsKindOf(RUNTIME_CLASS(CMscGenView))) return;
	if (pView->m_size.cx==0 || pView->m_size.cy==0) return;
	CMainFrame *pWnd = static_cast<CMainFrame *>(AfxGetMainWnd());
	if (!pWnd->IsKindOf(RUNTIME_CLASS(CMainFrame))) return;

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
			pView = static_cast<CMscGenView*>(GetNextView(pos));
  			if (!pView->IsKindOf(RUNTIME_CLASS(CMscGenView))) return;
			pView->GetClientRect(&view);

			//See which dimension is limiting
			if (double(y)/double(x) > double(pView->m_size.cy)/double(pView->m_size.cx)) 
				zoom = double(x)/double(pView->m_size.cx)*100.;
			else 
				zoom = double(y)/double(pView->m_size.cy)*100.;
			if (zoom > 100) zoom = 100;
			SetZoom(zoom);
			//If not fullscreen, adjust window size, too
			if (!pWnd->IsFullScreen()) {
				x = zoom*pView->m_size.cx/100 + 1;
				y = zoom*pView->m_size.cy/100 + 1;
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
			if (view.right-view.left > zoom*pView->m_size.cx/100 + 1) break;
			//adjust zoom if there is not enough space
			if (zoom*pView->m_size.cx/100 + 1 > x)
				zoom = (x-1)*100./pView->m_size.cx;
			SetZoom(zoom);
			//If not fullscreen, adjust window size, too
			if (!pWnd->IsFullScreen()) {
				x = zoom*pView->m_size.cx/100 + 1;
				if (x < 550) x = 580;
				if (window.left + x > screen.right - SCREEN_MARGIN)
					window.left = screen.right - SCREEN_MARGIN - x;
				pWnd->SetWindowPos(NULL, window.left, window.top, x, (window.bottom-window.top),  SWP_NOZORDER | SWP_NOACTIVATE);
			}
			break;
		case CMscGenDoc::ZOOM_WIDTH:
			zoom = (view.right-view.left)*100./pView->m_size.cx;
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
        CMscGenView* pView = static_cast<CMscGenView*>(GetNextView(pos));
  	    if (pView->IsKindOf(RUNTIME_CLASS(CMscGenView))) {
			//Each view handles this message only if pt is within its area
	        if (pView->DoMouseWheel(nFlags, zDelta, pt)) return true;
		}
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
	bool donot_chart_is_deleted = m_itrDoNotSyncForThis == m_charts.end();
	IChartData i;
	for (i = ++IChartData(m_itrEditing); i!=m_charts.end(); i++) {
		if (i == m_itrSaved) saved_chart_is_deleted = true;
		if (i == m_itrShown) shown_chart_is_deleted = true;
		if (i == m_itrDoNotSyncForThis) donot_chart_is_deleted = true;
	}
	m_charts.erase(++IChartData(m_itrEditing), m_charts.end());
	m_charts.push_back(data);
	m_itrEditing = --m_charts.end();
	if (saved_chart_is_deleted) 
		m_itrSaved = m_charts.end();
	if (shown_chart_is_deleted) 
		m_itrShown = m_charts.end();
	if (donot_chart_is_deleted) 
		m_itrDoNotSyncForThis = m_charts.end();
}

void CMscGenDoc::SyncShownWithEditing(const CString &action) 
{
	if (m_itrEditing == m_itrShown) return;
	if (m_itrEditing == m_itrDoNotSyncForThis) return;
	CString message = "I want to " + action + ", but you have made changes in the text editor.\n";
	if (m_bAttemptingToClose) 
		message.Append("Do you want to include the changes (or permanently loose them)?"); 
	else 
		message.Append("Do you want to include the changes and redraw the chart before I " + action + "?\n");

	if (IDYES == MessageBox(NULL, message, "Msc-generator", MB_ICONQUESTION | MB_YESNO)) 
		ShowEditingChart(true);
}

void CMscGenDoc::CheckIfChanged()
{
	if (m_itrSaved == m_charts.end()) goto modified;
	if (m_itrSaved != m_itrEditing) {
		if (m_itrSaved->GetText() != m_itrEditing->GetText()) goto modified;
		if (IsEmbedded()) {
			if (m_itrSaved->GetPage() != m_itrEditing->GetPage()) goto modified;
			if (m_itrSaved->GetDesign() != m_itrEditing->GetDesign()) goto modified;
		}
	}
	SetModifiedFlag(FALSE);
	return;
modified:
	SetModifiedFlag(TRUE);
}

void CMscGenDoc::OnExternalEditorChange(const CChartData &data) 
{
	SetTrackMode(false);
	int current_page = m_itrEditing->GetPage();
	InsertNewChart(data);
	m_itrEditing->SetPage(current_page);
	ShowEditingChart(true);
	CheckIfChanged();     

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
	StartFadingAll();
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

	m_itrShown = m_itrEditing;
	m_itrShown->m_wasDrawn = true;
	m_ChartShown = *m_itrEditing;

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
		//Update page controls and variables
		pApp->FillDesignPageCombo(m_ChartShown.GetPages(), m_ChartShown.GetPage());

		//See if we have the (potentially) new ForcedDesign verified & copied to the combo box of DesignBar
		if (!pApp->FillDesignDesignCombo(m_ChartShown.GetDesign())) {
			m_ChartShown.SetDesign("");
			m_itrShown->SetDesign("");
		}
	}

	//Abruptly delete all tracking rectangles
	m_trackArcs.clear();
	SetTrackMode(false);
	NotifyChanged(); 

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
	for (int i = 0; i<m_trackArcs.size(); i++) {
        TrackedArc &ta = m_trackArcs[i];
        switch (ta.status) {
            case TrackedArc::SHOWING:
                if (ta.fade_delay>0) 
                    ta.fade_delay = std::max(0, ta.fade_delay-FADE_TIMER);
                if (ta.fade_delay==0) 
                    ta.status = TrackedArc::FADING;
                break;
            case TrackedArc::APPEARING:
                bounding += ta.arc->GetAreaToDraw().GetBoundingBox();
                ta.fade_value += double(FADE_TIMER)/ta.appe_time;
                if (ta.fade_value >= 1) {
                    ta.fade_value = 1;
                    ta.status = TrackedArc::SHOWING;
                }
                break;
            case TrackedArc::FADING:
                bounding += ta.arc->GetAreaToDraw().GetBoundingBox();
                ta.fade_value -= double(FADE_TIMER)/ta.disa_time;
                if (ta.fade_value > 0) 
                    break;
                /*Fallthrough*/
            case TrackedArc::OFF:
                //if a control, remove from m_controlsShowing
                if (ta.what==TrackedArc::CONTROL) 
                    for (auto ii=m_controlsShowing.begin(); ii!=m_controlsShowing.end(); /*nope*/)
                        if (ii->second == ta.arc) m_controlsShowing.erase(ii++);
                        else ii++;
                m_trackArcs.erase(m_trackArcs.begin()+i);
                i--;
                break;
		}
	}
    if (bounding.IsInvalid()) //nothing to update
        return m_trackArcs.size()>0;  //return true if we need to keep the timer
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
	if (arc==NULL) return false;
	//Do not add if it has no visual element
    if (type == TrackedArc::TRACKRECT && arc->GetAreaToDraw().IsEmpty()) 
		return false;
	bool found = false;
    Block b; b.MakeInvalid();
	//Look for this arc. If already on list and still fully visible return false - no need to update
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
        b += i->arc->GetAreaToDraw().GetBoundingBox();
    }
    //We always redraw all the tracked rectangles, to look better
	if (!found) {
        const int appear = type == TrackedArc::TRACKRECT ? 300 : 100;
        const int disapp = type == TrackedArc::TRACKRECT ? 300 : 100;
		m_trackArcs.push_back(TrackedArc(arc, type, delay, appear, disapp));
        if (type == TrackedArc::CONTROL) 
            m_controlsShowing[arc->GetControlLocation()] = arc;
        b += arc->GetControlLocation();
    } 
    if (b.IsInvalid()) 
        return true;
	POSITION pos = GetFirstViewPosition();
	while(pos) {
		CMscGenView* pView = dynamic_cast<CMscGenView*>(GetNextView(pos));
		if (pView) pView->InvalidateBlock(b);
	}
	return true;
}

//Start the fading process for all rectangles (even for delay<0)
void CMscGenDoc::StartFadingAll() 
{
	for (std::vector<TrackedArc>::iterator i = m_trackArcs.begin(); i!=m_trackArcs.end(); i++) 
		i->status = TrackedArc::FADING;
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
    //Start fading all controls and track rectangles
	StartFadingAll();
    //re-add those controls which are under mouse
    for (auto i = m_controlsShowing.begin(); i!=m_controlsShowing.end(); i++)
        if (i->first.IsWithin(XY(mouse.x, mouse.y)) == WI_INSIDE)
            AddTrackArc(i->second, TrackedArc::CONTROL);
    //re-add any control for elements under mouse
	TrackableElement *arc = m_ChartShown.GetArcByCoordinate(mouse);
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
    if (ed != NULL) {
        if (t==MSC_CONTROL_COLLAPSE) chart.ForceEntityCollapse(ed->name, true);
        else if (t==MSC_CONTROL_EXPAND) chart.ForceEntityCollapse(ed->name, false);
        else return false;
    }
    m_trackArcs.clear();
    m_controlsShowing.clear();
	InsertNewChart(chart);
	CheckIfChanged();     
    ShowEditingChart(true);
    return true;
}


void CMscGenDoc::OnHelpHelp()
{
	char buff[1024]; 
	GetModuleFileName(NULL, buff, 1024);
	std::string dir(buff);
	int pos = dir.find_last_of('\\');
	ASSERT(pos!=std::string::npos);
	string cmdLine = "\"" + dir.substr(0,pos) + "\\msc-gen.chm\"";

	ShellExecute(NULL, NULL, cmdLine.c_str(), NULL, NULL, SW_SHOW); 
}


