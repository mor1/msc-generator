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
	ON_COMMAND(ID_DESIGN_DESIGN, OnDesignDesign)
	ON_CBN_SELENDOK(ID_DESIGN_DESIGN, OnDesignDesign)
	ON_COMMAND(ID_VIEW_NEXTERROR, &CMscGenDoc::OnViewNexterror)
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
END_MESSAGE_MAP()

CLIPFORMAT NEAR CMscGenDoc::m_cfPrivate = NULL;

// CMscGenDoc construction/destruction

CMscGenDoc::CMscGenDoc() : m_ExternalEditor(this)
{
	// Use OLE compound files, may not be needed for new environment
	EnableCompoundFile();

	// one-time construction code here
	if (m_cfPrivate == NULL)
	{
		m_cfPrivate = (CLIPFORMAT)
		::RegisterClipboardFormat(_T("Msc-Generator Signalling Chart"));
	}
	m_ZoomMode = (EZoomMode)AfxGetApp()->GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_DEFAULTZOOMMODE, 0);
	m_zoom = 100;
	m_nTrackRectNo = 0;
	m_bTrackMode = false;
	m_last_arc = NULL;
	m_saved_charrange.cpMax = 0;
	m_saved_charrange.cpMin = 0;
	m_nTrackBottomClip = 0;
	m_page = 0; //all

	m_charts.push_back(m_ChartShown);
	m_itrEditing = m_charts.begin();
	m_itrShown = m_charts.end();
	m_itrSaved = m_charts.end(); //start as unmodified

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

void CMscGenDoc::DestroyInPlaceFrame(COleIPFrameWnd* pFrameWnd)
{
	// Stop editor
	m_ExternalEditor.Stop(STOPEDITOR_WAIT);
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
		ar << unsigned(1); //file format version
		ar << m_ChartShown.GetDesign();
		ar << m_page;
		ar << m_ChartShown.GetText();
	} else {
		CString text;
		CString design;
		ar >> design;
		if (design != NEW_VERSION_STRING) {
			//Old file format
			ar >> m_page;
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
			unsigned file_version;
			ar >> file_version;
			switch (file_version) {
				case 0:
					char *buff;
					unsigned length;
					ar >> design;
					ar >> m_page;
					ar >> length;
					buff = (char*)malloc(length+1);
					ar.Read(buff, length);
					buff[length] = 0;
					text = buff;
					free(buff);
					break;
				default:
				case 1:
					ar >> design;
					ar >> m_page;
					ar >> text;
					break;
			}
		}
		EnsureCRLF(text);
		InsertNewChart(CChartData(text, design));
	}
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
	m_itrSaved = m_charts.end(); //start as unmodified
	ShowNewChart(m_charts.begin(), true);
	SetModifiedFlag(FALSE); //start as unmodified
	if (pApp->IsInternalEditorRunning())
		pApp->m_pWndEditor->m_ctrlEditor.UpdateText(m_itrEditing->GetText(), m_itrEditing->m_sel, true);
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
		InsertNewChart(data);

	}
	//Copy text to the internal editor
	if (pApp->IsInternalEditorRunning())
		pApp->m_pWndEditor->m_ctrlEditor.UpdateText(m_itrEditing->GetText(), m_itrEditing->m_sel, true);
	//Delete all entries before the currently loaded one (no undo)
	//part after (redo) was deleted by Serialize or InsertNewChart
	m_charts.erase(m_charts.begin(), m_itrEditing);
	SetModifiedFlag(FALSE);     // back to unmodified
	m_itrSaved = m_itrEditing;
	ShowNewChart(m_itrEditing, true);

	// if the app was started only to print, don't set user control
	//Copied from COleLinkingDoc
	if (pApp->m_pCmdInfo == NULL ||
		(pApp->m_pCmdInfo->m_nShellCommand != CCommandLineInfo::FileDDE &&
		 pApp->m_pCmdInfo->m_nShellCommand != CCommandLineInfo::FilePrint))
	{
		AfxOleSetUserCtrl(TRUE);
	}
	pApp->FillDesignDesignCombo(m_itrEditing->GetDesign());
	SetZoom(); //reset toolbar, do not chnage zoom
	if (restartEditor)
		m_ExternalEditor.Start(lpszPathName);
	return TRUE;
}

BOOL CMscGenDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	if (lpszPathName==NULL) {
		SyncShownWithEditing("update the container document", !m_bAttemptingToClose);
		return COleServerDocEx::OnSaveDocument(lpszPathName);
	}

	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	//Restart external editor only if file name changes
	bool restartEditor = lpszPathName!=GetPathName() && m_ExternalEditor.IsRunning();
	if (restartEditor) 
		m_ExternalEditor.Stop(STOPEDITOR_WAIT);
	SyncShownWithEditing("save", !m_bAttemptingToClose);
	if (!m_itrEditing->Save(lpszPathName)) {
		CFileException e;
		ReportSaveLoadException(lpszPathName, &e,
			TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
		return FALSE;
	}
	if (restartEditor) 
		m_ExternalEditor.Start(lpszPathName);
	m_itrSaved = m_itrEditing;
	SetModifiedFlag(FALSE);     // back to unmodified
	return TRUE;
}

void CMscGenDoc::OnCloseDocument()
{
	m_ExternalEditor.Stop(STOPEDITOR_FORCE);
	COleServerDocEx::OnCloseDocument();
}

void CMscGenDoc::OnUpdateFileExport(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_ChartShown.IsEmpty());
}

void CMscGenDoc::OnFileExport()
{
	SyncShownWithEditing("export", true);
	CFileDialog m_a(false);
	m_a.m_pOFN->lpstrTitle = "Export to file";
	m_a.m_pOFN->lpstrFilter =	
		"Portable Network Graphics (*.png)\0*.png\0"
		"Windows Bitmap (*.bmp)\0*.bmp\0"
		"Enhanced Metafile (*emf)\0*.emf\0"
		"Scalable Vector Graphics (*svg)\0*.svg\0"
		"Portable Document Format (*.pdf)\0*.pdf\0"
		"Encapsulated PostScript (*.eps)\0*.eps\0";
	m_a.m_pOFN->nFilterIndex = 1;
	char filename[1024] = "*.png";
	m_a.m_pOFN->lpstrFile = filename;
	m_a.m_pOFN->nMaxFile = sizeof(filename);
	if (m_a.DoModal() != IDOK)
		return;
	m_ChartShown.Draw(m_a.GetPathName());
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

	SetModifiedFlag(m_itrEditing != m_itrSaved || m_itrShown != m_itrSaved);
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
	
    SetModifiedFlag(m_itrEditing != m_itrSaved || m_itrShown != m_itrSaved);
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
	CMscGenSrvrItem *pItem = reinterpret_cast<CMscGenSrvrItem*>(COleServerDocEx::GetEmbeddedItem());
	pItem->CopyToClipboard();
}

void CMscGenDoc::OnUpdateEditPasteEntireChart(CCmdUI *pCmdUI)
{
	COleDataObject dataObject;
	dataObject.AttachClipboard();
	pCmdUI->Enable(dataObject.IsDataAvailable(m_cfPrivate) || 
                   dataObject.IsDataAvailable(CF_TEXT));
}

void CMscGenDoc::OnEditPasteEntireChart()
{
	// Paste is handled by Document
	COleDataObject dataObject;
	dataObject.AttachClipboard();
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
	}
	SetModifiedFlag();
	ShowNewChart(m_itrEditing, true);
	//Copy text to the internal editor
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	if (pApp->IsInternalEditorRunning())
		pApp->m_pWndEditor->m_ctrlEditor.UpdateText(m_itrEditing->GetText(), m_itrEditing->m_sel, true);
	if (restartEditor) 
		m_ExternalEditor.Start();
}

void CMscGenDoc::OnButtonEdittext()
{
	if (m_ExternalEditor.IsRunning())
		m_ExternalEditor.Stop(STOPEDITOR_WAIT);
	else
		m_ExternalEditor.Start();
}

void CMscGenDoc::OnUpdateButtonEdittext(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_ExternalEditor.IsRunning());
	if (!m_ExternalEditor.CanStart())
		if (!m_ExternalEditor.Init()) 
			MessageBox(0, "Fail to create external editor window", "Msc-generator", MB_OK);
	pCmdUI->Enable(m_ExternalEditor.CanStart());
}

void CMscGenDoc::OnEditUpdate()
{
	//update the View, update zoom, 
	ShowNewChart(m_itrEditing, true);
}

void CMscGenDoc::OnUdpateEditUpdate(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_itrShown != m_itrEditing);
}


void CMscGenDoc::OnButtonTrack()
{
	SetTrackMode(!m_bTrackMode);
}

void CMscGenDoc::OnUpdateButtonTrack(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bTrackMode);
}


void CMscGenDoc::OnViewNexterror()
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	COutputViewBar *pOutputView = pApp->m_pWndOutputView;
	if (!pOutputView) return;

	//Move to next error
	int maxsel = pOutputView->m_wndOutput.GetCount();
	if (maxsel == LB_ERR || maxsel==0) return; //no error
	int cursel = pOutputView->m_wndOutput.GetCurSel();
	cursel = (cursel+1) % maxsel;
	pOutputView->m_wndOutput.SetCurSel(cursel);
	//Jump to that pos in editor
	int line = m_ChartShown.GetErrorLine(cursel, pApp->m_Warnings);
	int col = m_ChartShown.GetErrorCol(cursel, pApp->m_Warnings);
	//Jump in the internal and external editors
	if (pApp->IsInternalEditorRunning()) 
		pApp->m_pWndEditor->m_ctrlEditor.JumpToLine(line, col);
	if (m_ExternalEditor.IsRunning())
		m_ExternalEditor.JumpToLine(line, col);

	//Turn tracking off
	SetTrackMode(false);
	//Show tracking boxes for the error
	bool wasboxes = m_nTrackRectNo!=0;
	void *arc = m_ChartShown.GetArcByLine(line, col);
	m_nTrackRectNo = m_ChartShown.GetCoversByArc(arc, int(m_page)-1, m_rctTrack, sizeof(m_rctTrack)/sizeof(RECT), m_nTrackBottomClip);
	if (wasboxes || m_nTrackRectNo) {
		POSITION pos = GetFirstViewPosition();
		while (pos) {
			CMscGenView* pView = (CMscGenView*)GetNextView(pos);
			pView->Invalidate();
		}
	}
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
	SyncShownWithEditing("change the design", true);
	m_itrEditing->SetDesign(new_forcedDesign);
	ShowNewChart(m_itrEditing, false);  //Do not change zoom, juts update views
	if (IsEmbedded())
		SetModifiedFlag();
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
	unsigned oldPage = m_page;
	m_page = index;
	if (m_page == oldPage)
		return;
	//We save forceddesign only in an embedded doc, so changing that counts as modified
	if (IsEmbedded())
		SetModifiedFlag(); 
	NotifyChanged();  //for OLE
	UpdateAllViews(NULL);
}

void CMscGenDoc::SetZoom(int zoom)
{
	//No zooming by ourselves during in-place editing
	if (IsInPlaceActive()) return;

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
	x -= (window.right-window.left) - (view.right-view.left) + SCREEN_MARGIN;
	y = screen.bottom-screen.top;
	y -= (window.bottom-window.top) - (view.bottom-view.top) + SCREEN_MARGIN; 
	int zoom;

	//OK, we have a sane View, with some drawing in it and we are not in place with a sane main window
	switch (mode) {
		case OVERVIEW:
			//Kill all, but one view
			for (int i = 1; i<pWnd->m_wndSplitter.GetRowCount(); i++)
				pWnd->m_wndSplitter.DeleteRow(i);
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
			x = zoom*pView->m_size.cx/100 + 1;
			y = zoom*pView->m_size.cy/100 + 1;
			//now result is the client size
			x += (window.right-window.left) - (view.right-view.left);
			y += (window.bottom-window.top) - (view.bottom-view.top);
			//now result is the window size, now adjust for minimum size
			if (x < 550) x = 580;
			if (y < 300) y = 300;
			if (window.left + x > screen.right - SCREEN_MARGIN)
				window.left = screen.right - SCREEN_MARGIN - x;
			if (window.top + y > screen.bottom - SCREEN_MARGIN)
				window.top = screen.bottom - SCREEN_MARGIN - y;
			SetZoom(zoom);
			pWnd->SetWindowPos(NULL, window.left, window.top, x, y,  SWP_NOZORDER | SWP_NOACTIVATE);
			break;
		case CMscGenDoc::WINDOW_WIDTH:
			//Try fit real size
			zoom = 100;
			//if window is big enough do nothing
			if (view.right-view.left > zoom*pView->m_size.cx/100 + 1) break;
			//adjust zoom if there is not enough space
			if (zoom*pView->m_size.cx/100 + 1 > x)
				zoom = (x-1)*100./pView->m_size.cx;
			x = zoom*pView->m_size.cx/100 + 1;
			if (x < 550) x = 580;
			if (window.left + x > screen.right - SCREEN_MARGIN)
				window.left = screen.right - SCREEN_MARGIN - x;
			SetZoom(zoom);
			pWnd->SetWindowPos(NULL, window.left, window.top, x, (window.bottom-window.top),  SWP_NOZORDER | SWP_NOACTIVATE);
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
	//Re-set m_itrSaved to .end() after list manipulation, if it was .end() before
	bool saved_chart_is_deleted = m_itrSaved == m_charts.end();
	bool shown_chart_is_deleted = m_itrShown == m_charts.end();
	IChartData i = m_itrEditing;
	i++;
	while (i!=m_charts.end() && !saved_chart_is_deleted) {
		if (i == m_itrSaved) saved_chart_is_deleted = true;
		if (i == m_itrShown) shown_chart_is_deleted = true;
		i++;
	}
	i = m_itrEditing;
	i++;
	m_charts.erase(i, m_charts.end());
	m_charts.push_back(data);
	m_itrEditing = --m_charts.end();
	if (saved_chart_is_deleted) 
		m_itrSaved = m_charts.end();
	if (shown_chart_is_deleted) 
		m_itrShown = m_charts.end();
}

void CMscGenDoc::SyncShownWithEditing(const CString &action, bool redoPossible) 
{
	if (m_itrEditing == m_itrShown) return;
	CString message = "I want to " + action + ", but you have made changes in the text editor.\n";
	if (redoPossible) 
		message.Append("Do you want to include the changes and redraw the chart before I " + action + "?\n" 
					   "(Even if you click No your changes will be accessible afterwards using " +
					   (m_itrShown == m_charts.begin()?"Undo.)":"Redo.)"));
	else 
		message.Append("Do you want to include the changes (or permanently loose them)?"); 

	if (IDYES == MessageBox(NULL, message, "Msc-generator", MB_ICONQUESTION | MB_YESNO)) {
		//Draw new text and update views
		ShowNewChart(m_itrEditing, true);
	} else {
		if (m_itrShown == m_charts.end()) {
			//User has already undoed the shown chart away
			InsertNewChart(m_ChartShown);
			ShowNewChart(m_itrEditing, false);
		} else {
			//Keep items between itrshowm and itrediting in the redo buffer
			m_itrEditing = m_itrShown;
		}
		//Copy the old text to internal editor
		CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
		ASSERT(pApp != NULL);
		if (pApp->IsInternalEditorRunning()) 
			pApp->m_pWndEditor->m_ctrlEditor.UpdateText(m_itrEditing->GetText(), m_itrEditing->m_sel, true);
	}
	//Now m_itrEditing == m_itrShown
}

void CMscGenDoc::OnExternalEditorChange(const CChartData &data) 
{
	SetTrackMode(false);
	InsertNewChart(data);
	SetModifiedFlag();
	//Now update the View
	ShowNewChart(m_itrEditing, true);
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
	if (m_bTrackMode) {
		SetTrackMode(false);
	} else if (m_nTrackRectNo!=0) {
		m_nTrackRectNo = 0;
		POSITION pos = GetFirstViewPosition();
		while (pos) {
			CMscGenView* pView = (CMscGenView*)GetNextView(pos);
			pView->Invalidate();
		}
	}
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
		InsertNewChart(CChartData(text, cr, m_itrEditing->GetDesign()));
		SetModifiedFlag();
	}
}

void CMscGenDoc::OnInternalEditorSelChange()
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	if (!m_bTrackMode || !pApp->IsInternalEditorRunning()) return;

	long start, end;
	pApp->m_pWndEditor->m_ctrlEditor.GetSel(start, end);
	int line, col;
	pApp->m_pWndEditor->m_ctrlEditor.ConvertPosToLineCol(start, line, col);
	bool wasboxes = m_nTrackRectNo!=0;
	void *arc = m_ChartShown.GetArcByLine(line+1, col+1);
	m_nTrackRectNo = m_ChartShown.GetCoversByArc(arc, int(m_page)-1, m_rctTrack, sizeof(m_rctTrack)/sizeof(RECT), m_nTrackBottomClip);
	if (wasboxes || m_nTrackRectNo) {
		POSITION pos = GetFirstViewPosition();
		while (pos) {
			CMscGenView* pView = (CMscGenView*)GetNextView(pos);
			pView->Invalidate();
		}
	}
}

void CMscGenDoc::ShowNewChart(IChartData itrNew, bool resetZoom)
{
	if (itrNew == m_charts.end()) return;
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);

	m_itrShown = itrNew;
	m_ChartShown = *itrNew;

	CWaitCursor wait;

	//Display error messages
	COutputViewBar *pOutputView = pApp->m_pWndOutputView;
	if (pOutputView && ::IsWindow(pOutputView->m_wndOutput)) {
		pOutputView->m_wndOutput.ResetContent();
		unsigned num = m_ChartShown.GetErrorNum(pApp->m_Warnings);
		for (int i=0; i<num; i++) 
			pOutputView->m_wndOutput.AddString(m_ChartShown.GetErrorText(i, pApp->m_Warnings));
		pOutputView->ShowPane(num>0, false, true);
	}

	//Update page controls and variables
	if (m_page >= m_ChartShown.GetPages())
		m_page = m_ChartShown.GetPages()-1;
	pApp->FillDesignPageCombo(m_ChartShown.GetPages(), m_page);

	//See if we have the (potentially) new ForcedDesign verified & copied to the combo box of DesignBar
	if (!pApp->FillDesignDesignCombo(m_ChartShown.GetDesign())) {
		m_ChartShown.SetDesign("");
		m_itrShown->SetDesign("");
	}

	NotifyChanged();  //for OLE
	UpdateAllViews(NULL);
	if (resetZoom) ArrangeViews();
	else SetZoom(); //just make sure combo shows right value

	//If there is an internal editor, reset focus to it.
	if (pApp->IsInternalEditorRunning()) pApp->m_pWndEditor->m_ctrlEditor.SetFocus();
}

void CMscGenDoc::SetTrackMode(bool on)
{
	if (on == m_bTrackMode) return;
	m_last_arc = NULL;
	m_bTrackMode = on;
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	if (on) {
		if (pApp->IsInternalEditorRunning()) pApp->m_pWndEditor->m_ctrlEditor.GetSel(m_saved_charrange);
		//Highlight rect for current text editor pos
		OnInternalEditorSelChange();
	} else if (m_nTrackRectNo) {
		//Delete trackrects from screen
		m_nTrackRectNo = 0;
		POSITION pos = GetFirstViewPosition();
		while (pos) {
			CMscGenView* pView = (CMscGenView*)GetNextView(pos);
			pView->Invalidate();
		}
	}
}

//Expects the coordinates in MscDrawer space (MscDrawer::totalWidth & Height)
void CMscGenDoc::UpdateTrackRects(CPoint mouse)
{
	if (!m_bTrackMode) return;
	bool wasTrackRect = m_nTrackRectNo!=0;
	void *arc = m_ChartShown.GetArcByCoordinate(mouse, int(m_page)-1);
	//If arc has not changed, do nothing
	if (arc == m_last_arc) return;
	m_last_arc = arc;
	m_nTrackRectNo = m_ChartShown.GetCoversByArc(arc, int(m_page)-1, m_rctTrack, sizeof(m_rctTrack)/sizeof(RECT), m_nTrackBottomClip);
	POSITION pos = GetFirstViewPosition();
	while (pos) {
		CMscGenView* pView = (CMscGenView*)GetNextView(pos);
		pView->Invalidate();
	}
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	if (!pApp->IsInternalEditorRunning()) return;
	CCshRichEditCtrl &editor = pApp->m_pWndEditor->m_ctrlEditor;
	if (m_nTrackRectNo) {
		unsigned start_line, start_col;
		unsigned end_line, end_col;
		if (m_ChartShown.GetLineByArc(arc, start_line, start_col, end_line, end_col)) {
			if (!wasTrackRect) editor.GetSel(m_saved_charrange);
			long start = editor.ConvertLineColToPos(start_line-1, start_col-1);
			long end = editor.ConvertLineColToPos(end_line-1, end_col);
			editor.SetRedraw(false);
			editor.SetSel(start, start);
			POINT scroll_pos;
			::SendMessage(editor.m_hWnd, EM_GETSCROLLPOS, 0, (LPARAM)&scroll_pos);
			editor.SetSel(start, end);
			::SendMessage(editor.m_hWnd, EM_SETSCROLLPOS, 0, (LPARAM)&scroll_pos);
			editor.SetRedraw(true);
			editor.Invalidate();
		}
	} else {
		editor.SetSel(m_saved_charrange);
	}
}