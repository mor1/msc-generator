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

#include "process.h"
#include <string>
#include <list>
#include <set>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COptionDlg dialog

IMPLEMENT_DYNCREATE(COptionDlg, CDialog)

COptionDlg::COptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionDlg::IDD, pParent)
	, m_Pedantic(FALSE)
	, m_Warnings(FALSE)
	, m_DefaultText(_T(""))
	, m_bNppExists(false)
{
}

COptionDlg::~COptionDlg()
{
}

void COptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_PEDANTIC, m_Pedantic);
	DDX_Check(pDX, IDC_CHECK_WARNINGS, m_Warnings);
	DDX_Text(pDX, IDC_EDIT_DEFAULT_TEXT, m_DefaultText);
	DDX_Radio(pDX, IDC_RADIO1, m_TextEditorRadioButtons);
	DDX_Text(pDX, IDC_EDIT1, m_TextEditStartCommand);
	DDX_Text(pDX, IDC_EDIT2, m_TextEditorJumpToLineCommand);
}

BOOL COptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	GetDlgItem(IDC_EDIT1)->EnableWindow(m_TextEditorRadioButtons==2);
	GetDlgItem(IDC_EDIT2)->EnableWindow(m_TextEditorRadioButtons==2);
	CFileFind finder;
	GetDlgItem(IDC_RADIO2)->EnableWindow(m_bNppExists);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COptionDlg::OnBnClickedTextEditorRadio()
{
	bool otherSet = ((CButton*)this->GetDlgItem(IDC_RADIO3))->GetCheck();
	GetDlgItem(IDC_EDIT1)->EnableWindow(otherSet);
	GetDlgItem(IDC_EDIT2)->EnableWindow(otherSet);
}

BEGIN_MESSAGE_MAP(COptionDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO1, &COptionDlg::OnBnClickedTextEditorRadio)
	ON_BN_CLICKED(IDC_RADIO2, &COptionDlg::OnBnClickedTextEditorRadio)
	ON_BN_CLICKED(IDC_RADIO3, &COptionDlg::OnBnClickedTextEditorRadio)
END_MESSAGE_MAP()

// CMscGenDoc

IMPLEMENT_DYNCREATE(CMscGenDoc, COleServerDoc)

BEGIN_MESSAGE_MAP(CMscGenDoc, COleServerDoc)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_FILE_EXPORT, OnFileExport)
	ON_COMMAND(ID_EDIT_PREFERENCES, OnEditPreferences)
	ON_COMMAND(ID_DESIGN_ZOOM, OnDesignZoom)
	ON_CBN_SELENDOK(ID_DESIGN_ZOOM, OnDesignZoom)
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
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateFileExport)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
END_MESSAGE_MAP()

CLIPFORMAT NEAR CMscGenDoc::m_cfPrivate = NULL;

// CMscGenDoc construction/destruction

CMscGenDoc::CMscGenDoc()
{
	// Use OLE compound files, may not be needed for new environment
	EnableCompoundFile();

	// one-time construction code here
	if (m_cfPrivate == NULL)
	{
		m_cfPrivate = (CLIPFORMAT)
		::RegisterClipboardFormat(_T("Msc-Generator Signalling Chart"));
	}
	if (!m_ErrorWindow.Create(IDD_DIALOG1, NULL)) {
		MessageBox(0,"Fail to create error window", "Msc-generator", MB_OK);
	}
	if (!m_ProgressWindow.Create(IDD_PROGRESSDIALOG, NULL)) {
		MessageBox(0,"Fail to create progress window", "Msc-generator", MB_OK);
	}
	//Options stored in registry
	ReadRegistryValues(false);
	m_zoom = 100;
	m_page = 0; //all

	m_EditorProcessId = 0;
	m_bModified = FALSE;
	m_hWndForEditor = NULL;

	m_charts.push_back(CChartData(m_Pedantic, m_ChartSourcePreamble, m_ChartSourcePostscript, m_CopyrightText, m_FileName));
	m_itrCurrent = m_charts.begin();
	m_itrSaved = m_charts.end(); //start as modified
}

CMscGenDoc::~CMscGenDoc()
{
}

// CMscGenDoc server implementation

//Inserts new chart after the current one and deletes redo list
void CMscGenDoc::InsertNewChart(const CChartData &data)
{
	//The mfc stl implementation changes the value of .end() at insert.
	//Re-set m_itrSaved to .end() after list manipulation, if it was .end() before
	bool saved_chart_is_deleted = m_itrSaved == m_charts.end();
	IChartData i = m_itrCurrent;
	i++;
	while (i!=m_charts.end() && !saved_chart_is_deleted) {
		if (i == m_itrSaved) saved_chart_is_deleted = true;
		i++;
	}
	i = m_itrCurrent;
	i++;
	m_charts.erase(i, m_charts.end());
	m_charts.push_back(data);
	m_itrCurrent = --m_charts.end();
	if (saved_chart_is_deleted) 
		m_itrSaved = m_charts.end();
}


COleServerItem* CMscGenDoc::OnGetEmbeddedItem()
{
	// OnGetEmbeddedItem is called by the framework to get the COleServerItem
	//  that is associated with the document.  It is only called when necessary.

	CMscGenSrvrItem* pItem = new CMscGenSrvrItem(this);
	ASSERT_VALID(pItem);
	return pItem;
}


// CMscGenDoc Active Document server implementation

CDocObjectServer *CMscGenDoc::GetDocObjectServer(LPOLEDOCUMENTSITE pDocSite)
{
	return new CDocObjectServer(this, pDocSite);
}

// CMscGenDoc serialization
#define NEW_VERSION_STRING "@@@Msc-generator later than 2.3.4"
void CMscGenDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) {
		ar << CString(NEW_VERSION_STRING);
		ar << unsigned(0); //file format version
		ar << m_itrCurrent->GetDesign();
		ar << m_page;
		ar << m_itrCurrent->GetLength();
		ar.Write(m_itrCurrent->GetText(), m_itrCurrent->GetLength());
	} else {
		char *buff;
		unsigned length;
		CString design;
		ar >> design;
		if (design != NEW_VERSION_STRING) {
			//Old file format
			ar >> m_page;
			unsigned alloc = 16384;
			buff = (char*)malloc(alloc);
			length = 0;
			while (1) {
				length += ar.Read(buff+length, alloc-length);
				if (length == alloc)
					buff = (char*)realloc(buff, alloc+=16384);
				else break;
			} 
		} else {
			//New file format
			unsigned file_version;
			ar >> file_version;
			switch (file_version) {
				default:
				case 0:
					ar >> design;
					ar >> m_page;
					ar >> length;
					buff = (char*)malloc(length);
					ar.Read(buff, length);
					break;
			}
		}
		CChartData data(m_Pedantic, m_ChartSourcePreamble, m_ChartSourcePostscript, m_CopyrightText, m_FileName);
		data.Set(buff, length);
		data.SetDesign(design);
		InsertNewChart(data);
		free(buff);
	}
}


// CMscGenDoc diagnostics

#ifdef _DEBUG
void CMscGenDoc::AssertValid() const
{
	COleServerDoc::AssertValid();
}

void CMscGenDoc::Dump(CDumpContext& dc) const
{
	COleServerDoc::Dump(dc);
}
#endif //_DEBUG


// CMscGenDoc commands

void CMscGenDoc::DeleteContents()
{
	m_charts.clear();
	m_charts.push_back(CChartData(m_Pedantic, m_ChartSourcePreamble, m_ChartSourcePostscript, m_CopyrightText, m_FileName));
	m_itrCurrent = m_charts.begin();
	m_itrSaved = m_charts.end(); //start as modified
	COleServerDoc::DeleteContents();
}

void CMscGenDoc::OnUpdate(bool resetZoom)
{
	Msc_GetErrors(m_itrCurrent->GetMsc(), m_Warnings, m_errorText, MAX_ERROR_LENGTH);
	//Copy errors/warnings to the error window & show/hide as appropriate
	CopyErrorsToWindow(m_errorText);
	JumpToLine(Msc_GetErrorLine(m_itrCurrent->GetMsc()));
	NotifyChanged();  //for OLE
	UpdateAllViews(NULL);
	if (resetZoom) ArrangeViews();
}


BOOL CMscGenDoc::OnNewDocument()
{
	bool restartEditor = m_EditorProcessId!=0;
	StopEditor(STOPEDITOR_WAIT);
	if (!COleServerDoc::OnNewDocument())
		return FALSE;

	// (SDI documents will reuse this document)
	CChartData data(m_Pedantic, m_ChartSourcePreamble, m_ChartSourcePostscript, m_CopyrightText, m_FileName);
	data.Set(m_DefaultText, m_DefaultText.GetLength());
	m_charts.clear();
	m_charts.push_back(data);
	m_itrCurrent = m_charts.begin();
	m_itrSaved = m_charts.end(); //start as modified
	SetModifiedFlag(FALSE); //start as modified
	FillDesignDesignCombo();
	OnUpdate();
	if (restartEditor)
		StartEditor("Untitled");
	return TRUE;
}

BOOL CMscGenDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	ASSERT_VALID(this);
	ASSERT(lpszPathName == NULL || AfxIsValidString(lpszPathName));

	CMscGenApp *pApp = (CMscGenApp *)::AfxGetApp();
	ASSERT(pApp != NULL);

	bool restartEditor = m_EditorProcessId!=0;
	StopEditor(STOPEDITOR_WAIT);

	if (lpszPathName == NULL) {
		//Will use serialize which sets m_itrCurrent to loaded entry
		if (!COleServerDoc::OnOpenDocument(lpszPathName))
			return FALSE;
		m_FileName = "Embedded";
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

		CString fn(lpszPathName);
		m_FileName = fn.Mid(fn.ReverseFind('\\')+1);
		CChartData data(m_Pedantic, m_ChartSourcePreamble, m_ChartSourcePostscript, m_CopyrightText, m_FileName);
		if (!data.Load(lpszPathName)) {
			CFileException e;
			ReportSaveLoadException(lpszPathName, &e,
				FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
			return FALSE;
		}
		data.SetDesign(m_itrCurrent->GetDesign());
		InsertNewChart(data);

	};
	//Delete all entries before the currently loaded one (no undo)
	//part after (redo) was deleted by Serialize or InsertNewChart
	m_charts.erase(m_charts.begin(), m_itrCurrent);
	m_itrSaved = m_itrCurrent;

	// if the app was started only to print, don't set user control
	//Copied from COleLinkingDoc
	if (pApp->m_pCmdInfo == NULL ||
		(pApp->m_pCmdInfo->m_nShellCommand != CCommandLineInfo::FileDDE &&
		 pApp->m_pCmdInfo->m_nShellCommand != CCommandLineInfo::FilePrint))
	{
		AfxOleSetUserCtrl(TRUE);
	}
	FillDesignDesignCombo();
	if (restartEditor)
		StartEditor(lpszPathName);
	SetModifiedFlag(FALSE);     // back to unmodified}
	OnUpdate();
	return TRUE;
}

BOOL CMscGenDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	if (lpszPathName==NULL) 
		return COleServerDoc::OnSaveDocument(lpszPathName);

	bool restartEditor = (lpszPathName!=GetPathName() && m_EditorProcessId!=0);
	if (restartEditor) 
		StopEditor(STOPEDITOR_WAIT);
	if (!m_itrCurrent->Save(lpszPathName)) {
		CFileException e;
		ReportSaveLoadException(lpszPathName, &e,
			TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
		return FALSE;
	}
	if (restartEditor) 
		StartEditor(lpszPathName);
	m_itrSaved = m_itrCurrent;
	SetModifiedFlag(FALSE);     // back to unmodified}
	return TRUE;
}

void CMscGenDoc::OnCloseDocument()
{
	StopEditor(STOPEDITOR_FORCE);
	COleServerDoc::OnCloseDocument();
}

void CMscGenDoc::OnUpdateFileExport(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_itrCurrent->IsEmpty());
}


void CMscGenDoc::OnFileExport()
{
	if (m_itrCurrent->IsEmpty()) 
		return;
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
	Msc_Draw_to_File(m_itrCurrent->GetMsc(), m_a.GetPathName());
}

void CMscGenDoc::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_itrCurrent != m_charts.begin());
}

void CMscGenDoc::OnEditUndo()
{
	if (m_itrCurrent == m_charts.begin()) return;
	bool editor = false;
	if (m_EditorProcessId) {
		StopEditor(STOPEDITOR_WAIT);
		editor = true;
	}
	m_itrCurrent--;
	//if (!m_itrCurrent->IsDrawn()) 
	StartDrawingProgress();
	m_itrCurrent->CompileIfNeeded();
    SetModifiedFlag(m_itrCurrent != m_itrSaved);
	OnUpdate();
	if (editor)	StartEditor();
	StopDrawingProgress();
}

void CMscGenDoc::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_itrCurrent != --m_charts.end());
}
void CMscGenDoc::OnEditRedo()
{
	if (m_itrCurrent == --m_charts.end()) return;
	bool editor = false;
	if (m_EditorProcessId) {
		StopEditor(STOPEDITOR_WAIT);
		editor = true;
	}
	m_itrCurrent++;
	//if (!m_itrCurrent->IsDrawn()) 
	StartDrawingProgress();
	m_itrCurrent->CompileIfNeeded();
    SetModifiedFlag(m_itrCurrent != m_itrSaved);
	OnUpdate();
	if (editor)	StartEditor();
	StopDrawingProgress();
}
void CMscGenDoc::OnEditCopy()
{
	//Copy is handled by SrvItem
	CMscGenSrvrItem *pItem= GetEmbeddedItem();
	pItem->CopyToClipboard();
}

void CMscGenDoc::OnSetItemRects(LPCRECT lpPosRect , LPCRECT lpClipRect)
{
	// call base class to change the size of the window
	COleServerDoc::OnSetItemRects(lpPosRect, lpClipRect);

	// notify first view that scroll info should change
//	POSITION pos = GetFirstViewPosition();
//	CMscGenView* pView = (CMscGenView*)GetNextView(pos);
//	pView->ResyncScrollSizes();
}

void CMscGenDoc::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	COleDataObject dataObject;
	dataObject.AttachClipboard();
	pCmdUI->Enable(dataObject.IsDataAvailable(m_cfPrivate) || 
                   dataObject.IsDataAvailable(CF_TEXT));
}

void CMscGenDoc::OnEditPaste()
{
	// Paste is handled by Document
	COleDataObject dataObject;
	dataObject.AttachClipboard();
	bool editor_running = m_EditorProcessId != 0;
	if (dataObject.IsDataAvailable(m_cfPrivate)) {
		// get file refering to clipboard data
		CFile *pFile = dataObject.GetFileData(m_cfPrivate);
		if (pFile == NULL) return;
		StartDrawingProgress();
		if (editor_running) StopEditor(STOPEDITOR_FORCE);
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
		StartDrawingProgress();
		if (editor_running) StopEditor(STOPEDITOR_FORCE);
		CChartData data(m_Pedantic, m_ChartSourcePreamble, m_ChartSourcePostscript, m_CopyrightText, m_FileName);
		data.Set((char*)v, length);
		data.SetDesign(m_itrCurrent->GetDesign());
		GlobalUnlock(hGlobal);
		GlobalFree(hGlobal);
		InsertNewChart(data);
	}
	SetModifiedFlag(TRUE);
	OnUpdate();
	if (editor_running) StartEditor();
	StopDrawingProgress();
}

void CMscGenDoc::OnEditPreferences()
{
	CMscGenApp *pApp = (CMscGenApp *)::AfxGetApp();
	COptionDlg optionDlg;
	optionDlg.m_Pedantic = m_Pedantic;
	optionDlg.m_Warnings = m_Warnings;
	optionDlg.m_DefaultText = m_DefaultText;
	optionDlg.m_DefaultText.Replace("\n", "\x0d\x0a");
	optionDlg.m_TextEditStartCommand = m_sStartTextEditor;
	optionDlg.m_TextEditorJumpToLineCommand = m_sJumpToLine;
	optionDlg.m_bNppExists = m_NppPath.GetLength()>0;

	switch (m_iTextEditorType) {
		default:
		case NOTEPAD: optionDlg.m_TextEditorRadioButtons = 0; break;
		case NPP: optionDlg.m_TextEditorRadioButtons = m_NppPath.GetLength()?1:0; break;
		case OTHER: optionDlg.m_TextEditorRadioButtons = 2; break;
	}
	if (optionDlg.DoModal() == IDOK) {
		if (m_Pedantic != (bool)optionDlg.m_Pedantic) {
			m_Pedantic = optionDlg.m_Pedantic;
			pApp->WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_PEDANTIC, m_Pedantic);
			StartDrawingProgress(); 
			for (IChartData i = m_charts.begin(); i!=m_charts.end(); i++)
				i->FreeMsc();
			m_itrCurrent->CompileIfNeeded();  
			OnUpdate(false);     //Do not change zoom, merely re-issue errors
			StopDrawingProgress();
		}
		if (m_Warnings != optionDlg.m_Warnings) {
			m_Warnings = optionDlg.m_Warnings;
			pApp->WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_WARNINGS, m_Warnings);
		}

		bool bRestartEditor=false;
		EEditorType temp;
		switch (optionDlg.m_TextEditorRadioButtons) {
			default:
			case 0: temp = NOTEPAD; break;
			case 1: temp = m_NppPath.GetLength()?NPP:NOTEPAD; break;
			case 2: temp = OTHER; break;
		}
		if (m_sStartTextEditor != optionDlg.m_TextEditStartCommand) {
			m_sStartTextEditor = optionDlg.m_TextEditStartCommand;
			pApp->WriteProfileString(REG_SECTION_SETTINGS, REG_KEY_STARTTEXTEDITOR, m_sStartTextEditor);
			if (temp==OTHER) bRestartEditor=true;
		}
		if (m_sJumpToLine != optionDlg.m_TextEditorJumpToLineCommand) {
			m_sJumpToLine = optionDlg.m_TextEditorJumpToLineCommand;
			pApp->WriteProfileString(REG_SECTION_SETTINGS, REG_KEY_JUMPTOLINE, m_sJumpToLine);
		}
		if (temp != m_iTextEditorType) {
			m_iTextEditorType = temp;
			pApp->WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_TEXTEDITORTYPE, m_iTextEditorType);
			bRestartEditor=true;
			switch (m_iTextEditorType) {
				case NPP:
					m_sStartTextEditor = "\"" +m_NppPath+ "\" -multiInst -nosession %n";
					m_sJumpToLine = "\"" +m_NppPath+ "\" -n%l -nosession %n";
					break;
				case NOTEPAD:
				default:
					m_sStartTextEditor = "Notepad.exe %n";
					m_sJumpToLine = "";
					break;
				case OTHER:
					break; //values already set by user
			}
		}
		if (bRestartEditor && m_EditorProcessId) {
			StopEditor(STOPEDITOR_WAIT);
			StartEditor();
		}
		if (optionDlg.m_DefaultText != m_DefaultText) {
			m_DefaultText = optionDlg.m_DefaultText;
			int rep = m_DefaultText.Replace("\x0a", "");
			pApp->WriteProfileString(REG_SECTION_SETTINGS, REG_KEY_DEFAULTTEXT, m_DefaultText);
		}
	}
}

//C:\Windows\Notepad.exe %n;"C:\Program Files\Notepad++\notepad++.exe" -multiInst -nosession %n;
//;-n%l -nosession %n;


void CMscGenDoc::ReadRegistryValues(bool reportProblem) 
{
	CMscGenApp *pApp = (CMscGenApp *)::AfxGetApp();
	//Load Registry values
	m_Pedantic = pApp->GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_PEDANTIC, FALSE);
	m_Warnings = pApp->GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_WARNINGS, TRUE);
	m_iTextEditorType = EEditorType(pApp->GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_TEXTEDITORTYPE, NOTEPAD));

	m_NppPath = "C:\\Program Files\\Notepad++\\notepad++.exe";
	CFileFind finder;
	if (!finder.FindFile(m_NppPath))
		m_NppPath.Empty(); //empty value means no NPP on this system

	switch (m_iTextEditorType) {
		case NPP:
			if (m_NppPath.GetLength()) {
				m_sStartTextEditor = "\"" +m_NppPath+ "\" -multiInst -nosession %n";
				m_sJumpToLine = "\"" +m_NppPath+ "\" -n%l -nosession %n";
				break;
			}
			//fallback if notepad++ not found
		case NOTEPAD:
		default:
			m_sStartTextEditor = "Notepad.exe %n";
			m_sJumpToLine = "";
			break;
		case OTHER:
			m_sStartTextEditor = pApp->GetProfileString(REG_SECTION_SETTINGS, REG_KEY_STARTTEXTEDITOR, "Notepad.exe %n");
			m_sJumpToLine = pApp->GetProfileString(REG_SECTION_SETTINGS, REG_KEY_JUMPTOLINE, "");
			break;
	}
	m_DefaultText= pApp->GetProfileString(REG_SECTION_SETTINGS, REG_KEY_DEFAULTTEXT, 
		"#Default signalling chart\r\na,b,c;\r\nb->c:trallala;\r\na->b: new;\r\n");

	m_ZoomMode = (EZoomMode)pApp->GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_DEFAULTZOOMMODE, 0);

	ReadDesigns(reportProblem); //fills m_ChartSourcePreamble appropriately, default filename applies
	FillDesignDesignCombo();
	m_ChartSourcePostscript = "";
	m_CopyrightText = "\\md(0)\\mu(2)\\mr(0)\\mn(10)\\f(arial)\\pr\\c(150,150,150)"
		              "http://msc-generator.sourceforge.net v2.4.0pre1";
	//m_CopyrightText.AppendFormat(" v%d.%d.%d", LIBMSCGEN_MAJOR, LIBMSCGEN_MINOR, LIBMSCGEN_SUPERMINOR);
}

//Read the designs from m_DesignDir, display a modal dialog if there is a problem.
//If no problem, update m_ChartSourcePreamble and return true
bool CMscGenDoc::ReadDesigns(bool reportProblem, const char *fileName)
{
	char buff[1024]; 
	GetModuleFileName(NULL, buff, 1024);
	std::string dir(buff);
	int pos = dir.find_last_of('\\');
	ASSERT(pos!=std::string::npos);
	CString designlib_filename = dir.substr(0,pos).append("\\").append(fileName).c_str();

	CString preamble;
	CString msg;
	CFileFind finder;
	bool errors = false;
	bool bFound = finder.FindFile(designlib_filename);
	if (bFound) m_SetOfDesigns.Empty();
	while (bFound) {
		bFound = finder.FindNextFile();
		bool designlib_pedantic = true;
		CString designlib_strings;
		CString designlib_filename("[designlib]");
		CChartData data(designlib_pedantic, designlib_strings, designlib_strings, designlib_strings, designlib_filename);
		if (data.Load(finder.GetFilePath(), false)) {
			char buff[4096]; //needed later for designs, too
			Msc_GetErrors(data.GetMsc(), true, buff, sizeof(buff));
			if (strlen(buff)) {
				msg.Append("Errors in design file: ");
				msg.Append(finder.GetFileName());
				msg.Append("\n");
				msg.Append(buff);
				errors = true;
			}
			char *heyho = (char*)malloc(data.GetLength()+1);
			memcpy(heyho, data.GetText(), data.GetLength());
			heyho[data.GetLength()] = 0;
			preamble.Append(heyho);
			free(heyho);
			Msc_GetDesigns(data.GetMsc(), buff, sizeof(buff));
			if (m_SetOfDesigns.GetLength()>0) m_SetOfDesigns.Append(" ");
			m_SetOfDesigns.Append(buff);
		}
	}
	if (msg.GetLength()>0 && reportProblem)
		MessageBox(NULL, msg, "Msc-generator", MB_OK);
	if (preamble.GetLength()>0) 
		m_ChartSourcePreamble = preamble;
	else 
		m_ChartSourcePreamble = ";\n";
	return !errors;
}

void CMscGenDoc::FillDesignDesignCombo(void) {
	//ok now designs contains a set of of designs, preable contains a concatenation of designlib text
	//Add the list of designs to the combo boxes
	CObList list;
	CMFCToolBar::GetCommandButtons(ID_DESIGN_DESIGN, list);
	POSITION p = list.GetHeadPosition();
	while (p) {
		CMFCToolBarComboBoxButton *combo = static_cast<CMFCToolBarComboBoxButton*>(list.GetNext(p));
		combo->ClearData();
		combo->AddItem("(use chart-defined)");
		combo->AddItem("plain");
		int pos = 0;
		while (m_SetOfDesigns.GetLength()>pos) {
			int pos2 = m_SetOfDesigns.Find(' ', pos);
			if (pos2==-1) pos2 = m_SetOfDesigns.GetLength();
			if (pos2>pos && m_SetOfDesigns.Mid(pos, pos2-pos).CompareNoCase("plain")!=0)
				combo->AddItem(m_SetOfDesigns.Mid(pos, pos2-pos));
			pos = pos2+1;
		}	
		//restore the selection to the saved style if it can be found
		int index = combo->FindItem(m_itrCurrent->GetDesign());
		if (index == CB_ERR)
			combo->SelectItem(0);
		else 
			combo->SelectItem(index);
	}
};

void CMscGenDoc::CopyErrorsToWindow(char *text) 
{
	CEdit *pEdit = (CEdit*)m_ErrorWindow.GetDlgItem(IDC_EDIT1);
	CString errors(text);
	int pos = 0;
	do {
		pos = errors.Find('\n', pos);
		if (pos>=0) {
			errors.Insert(pos, '\r');
			pos+=2;
		}
	} while (pos>=0);
	pEdit->SetWindowText(errors);
	if (strlen(text))
		m_ErrorWindow.ShowWindow(SW_SHOW); //Activate
	else
		m_ErrorWindow.ShowWindow(SW_HIDE); //Activate
}


void CMscGenDoc::StartEditor(CString filename)
{
	if (m_EditorProcessId) return;

	//Determine filename
	//Reuse existing filename, if no new name is given
	if (filename.GetLength() > 0 || m_EditorFileName.GetLength() == 0) {
		if (filename.GetLength() == 0) {
			if (IsEmbedded()) {
				if (IsInPlaceActive()) {
					// In-place active embedded charts
					filename = "In-place Chart";
				} else {
					// Embedded charts opened in a separate window
					AfxGetMainWnd()->GetWindowText(filename);
					if (filename.Right(16) == " - Msc-generator")
						filename = filename.Left(filename.GetLength()-16);
					else if (filename.Right(13) == "Msc-generator")
						filename = filename.Left(filename.GetLength()-13);
				}
			} else {
				// Standalone opened charts
				filename = "Unsigned";
			}
		}
		//Remove path info, keep filename only
		int pos = filename.ReverseFind('\\');
		if (pos >0)
			filename.Delete(0, pos+1);
		//Add random number and extension and prepend TEMP dir
		m_EditorFileName.Format("%04d - Text of ", rand()%1000);
		m_EditorFileName.Append(filename);
		if (m_EditorFileName.Right(11)!=".signalling")
			m_EditorFileName.Append(".signalling");
		char buff[1024];
		GetTempPath(sizeof(buff), buff);
		m_EditorFileName.Insert(0, buff);
	}
	//Save file, record last modification time (pretty much now)
	m_itrCurrent->Save(m_EditorFileName);
	CFileStatus status;
	CFile::GetStatus(m_EditorFileName, status);
	m_EditorFileLastMod = status.m_mtime;

	//Fire up editor process
	CString cmdLine = m_sStartTextEditor;
	cmdLine.Replace("%n", "\"" + CString(m_EditorFileName) + "\"");
	char* cmdline = strdup(cmdLine);
	STARTUPINFO si;
	si.cb = sizeof(STARTUPINFO);
	si.lpDesktop = NULL;
	si.lpTitle = strdup(m_EditorFileName.GetString());
	si.dwFlags = 0;
	si.lpReserved = NULL;
	si.lpReserved2 = NULL;
	si.cbReserved2 = NULL;
	PROCESS_INFORMATION pi;
	if (!CreateProcess(NULL, cmdline, NULL, NULL, FALSE, 0, NULL, NULL,  &si, &pi)) {
		free(cmdline);
		free(si.lpTitle);
		StopEditor(STOPEDITOR_FORCE); //Delete temp file and null the filename
		return;
	}
	m_EditorProcessId = pi.dwProcessId;
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	free(cmdline);
	free(si.lpTitle);

	//OK, now start a timer in all views (so it is still up when one is killed)
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL) {
        CMscGenView* pView = static_cast<CMscGenView*>(GetNextView(pos));
  	    if (pView->IsKindOf(RUNTIME_CLASS(CMscGenView))) {
			pView->m_hTimer = pView->SetTimer(1, 100, NULL);
		}
	}
}

void CMscGenDoc::JumpToLine(unsigned line)
{
	if (m_EditorFileName.GetLength()==0 || m_EditorProcessId==0) return;
	if (m_sJumpToLine.GetLength()==0 || line==0) return;
	CString cmdLine = m_sJumpToLine;
	CString num;
	num.Format("%u", line);
	cmdLine.Replace("%l", num);
	cmdLine.Replace("%n", "\"" + CString(m_EditorFileName) + "\"");

	char* cmdline = strdup(cmdLine);
	STARTUPINFO si;
	si.cb = sizeof(STARTUPINFO);
	si.lpDesktop = NULL;
	si.lpTitle = strdup(m_EditorFileName.GetString());
	si.dwFlags = 0;
	si.lpReserved = NULL;
	si.lpReserved2 = NULL;
	si.cbReserved2 = NULL;
	PROCESS_INFORMATION pi;
	CreateProcess(NULL, cmdline, NULL, NULL, FALSE, 0, NULL, NULL,  &si, &pi);
	free(cmdline);
	free(si.lpTitle);
}

bool CMscGenDoc::CheckEditorAndFile()
{
	if (m_EditorFileName.GetLength()>0) {
		CFileStatus status;
		//Query file status
		if (CFile::GetStatus(m_EditorFileName, status)) {
			//Check if the file has been updated by the editor
			if (m_EditorFileLastMod != status.m_mtime) {
				//Load modification
				CChartData data(m_Pedantic, m_ChartSourcePreamble, m_ChartSourcePostscript, m_CopyrightText, m_FileName);
				if (data.Load(m_EditorFileName, FALSE)) {
					StartDrawingProgress();
					m_EditorFileLastMod = status.m_mtime;
					//OK: File successfully read, update the Document.
					data.SetDesign(m_itrCurrent->GetDesign());
					InsertNewChart(data);
					SetModifiedFlag();
					//Now update the View
					OnUpdate();
					StopDrawingProgress();
					return true;
				}
			}
		} else {
			//Editor file not possible to stat
			m_EditorFileName.Empty();
		}
	}
	//Check if editor is still running
	if (m_EditorProcessId) {
		HANDLE h = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, m_EditorProcessId);
		if (!GetPriorityClass(h)) { //this checks if process still there
			StopEditor(STOPEDITOR_FORCE); //remove editor file & zero out 
		}
		CloseHandle(h);
	}
	return false;
}


struct window_search {
	DWORD processId;
	std::list<HWND> hWnd_result;
};

BOOL CALLBACK EnumWindowsProcFindTopWindowByProcess(HWND hwnd, LPARAM lParam)
{
	DWORD processId;
	GetWindowThreadProcessId(hwnd, &processId);
	if (processId == reinterpret_cast<struct window_search*>(lParam)->processId)
		reinterpret_cast<window_search*>(lParam)->hWnd_result.push_back(hwnd);
	return true; //continue iteration
}

void CMscGenDoc::StopEditor(EStopEditor force)
{
	if (m_EditorProcessId) {
		switch (force) {
		case STOPEDITOR_FORCE:
			{
			HANDLE h = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_EditorProcessId);
			TerminateProcess(h, 0);
			CloseHandle(h);
			//continue with removing editor file and view timers
			break;
			}
		case STOPEDITOR_NOWAIT:
		case STOPEDITOR_WAIT:
			struct window_search search;
			search.processId = m_EditorProcessId;
			EnumWindows(EnumWindowsProcFindTopWindowByProcess, (LPARAM)&search);
			int ii = search.hWnd_result.size();
			for (std::list<HWND>::iterator i=search.hWnd_result.begin(); i!=search.hWnd_result.end(); i++) {
				char buff[1024];
				int len = GetWindowText(*i, buff, sizeof(buff)); 
				if (strstr(buff, "signalling")!=NULL) {
					SetForegroundWindow(*i);
					SendMessage(*i, WM_CLOSE, 0, 0); 
				}
			}
			if (force == STOPEDITOR_NOWAIT)
				//Okay let us wait till the editor actually closes
				//CView::OnTimer will come back to us in CheckEditorAndFile and when editor is gone
				//MscGenDoc::StopEditor will be called with STOPEDITOR_FORCE, which will remove timers and editor file
				return;
			//If we are forced to close, wait till editor process closes
			bool wait = true;
			while (wait) { 
				HANDLE h = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, m_EditorProcessId);
				wait = GetPriorityClass(h);  //this checks if process still there
				CloseHandle(h);
				Sleep(300);
			}
		}
		//Indicate that the editor process is gone
		m_EditorProcessId = 0; 
	}
	CheckEditorAndFile(); //update the document if editor has made last minute edits
	//Remove timers from views
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL) {
        CMscGenView* pView = static_cast<CMscGenView*>(GetNextView(pos));
  	    if (pView->IsKindOf(RUNTIME_CLASS(CMscGenView))) {
			pView->KillTimer(pView->m_hTimer);
			pView->m_hTimer = NULL;
		}
	}
	//Remove file, if still there
	if (m_EditorFileName.GetLength()>0) {
		TRY {
			CFile::Remove(m_EditorFileName);
		} CATCH(CFileException, pEx) {
		}
		END_CATCH
		m_EditorFileName.Empty();
	}

}

COleIPFrameWnd* CMscGenDoc::CreateInPlaceFrame(CWnd* pParentWnd)
{
	COleIPFrameWnd* ret = COleServerDoc::CreateInPlaceFrame(pParentWnd);
	// Now we are sure to have a view 
	//Start Editor (needs a view for timer)
	StartEditor();
	return ret;
}

void CMscGenDoc::DestroyInPlaceFrame(COleIPFrameWnd* pFrameWnd)
{
	// Stop editor
	StopEditor(STOPEDITOR_WAIT);
	COleServerDoc::DestroyInPlaceFrame(pFrameWnd);
}

void CMscGenDoc::StartDrawingProgress() 
{
	m_ProgressWindow.ShowWindow(SW_SHOW); //Activate
}

void CMscGenDoc::StopDrawingProgress()
{
	m_ProgressWindow.ShowWindow(SW_HIDE); 
}

void CMscGenDoc::SetZoom(int zoom)
{
	//No zooming by ourselves during in-place editing
	if (IsInPlaceActive()) return;

	if (zoom < 1) return; //0 or -1 are errors probably
	if (zoom > 10000) zoom = 10000;
	if (zoom < 10) zoom = 10;

	CString text;
	text.Format("%u%%", zoom);
	CObList list;
	CMFCToolBar::GetCommandButtons(ID_DESIGN_ZOOM, list);
	POSITION p = list.GetHeadPosition();
	while (p) {
		CMFCToolBarComboBoxButton *combo = static_cast<CMFCToolBarComboBoxButton*>(list.GetNext(p));
		if (combo->GetCount()==0) {
			static unsigned zoom_values[] = {400, 300, 200, 150, 100, 75, 50, 40, 30, 20, 10, 0};
			for (int i=0; zoom_values[i]>0; i++) {
				CString text;
				text.Format("%u%%", zoom_values[i]);
				combo->AddItem(text, zoom_values[i]);
			}
		}
		if (zoom != m_zoom) combo->SetText(text);
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
	if (pView->m_xSize==0 || pView->m_ySize==0) return;
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
			if (double(y)/double(x) > double(pView->m_ySize)/double(pView->m_xSize)) 
				zoom = double(x)/double(pView->m_xSize)*100.;
			else 
				zoom = double(y)/double(pView->m_ySize)*100.;
			if (zoom > 100) zoom = 100;
			x = zoom*pView->m_xSize/100 + 1;
			y = zoom*pView->m_ySize/100 + 1;
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
			if (view.right-view.left > zoom*pView->m_xSize/100 + 1) break;
			//adjust zoom if there is not enough space
			if (zoom*pView->m_xSize/100 + 1 > x)
				zoom = (x-1)*100./pView->m_xSize;
			x = zoom*pView->m_xSize/100 + 1;
			if (x < 550) x = 580;
			if (window.left + x > screen.right - SCREEN_MARGIN)
				window.left = screen.right - SCREEN_MARGIN - x;
			SetZoom(zoom);
			pWnd->SetWindowPos(NULL, window.left, window.top, x, (window.bottom-window.top),  SWP_NOZORDER | SWP_NOACTIVATE);
			break;
		case CMscGenDoc::ZOOM_WIDTH:
			zoom = (view.right-view.left)*100./pView->m_xSize;
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



