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

// Msc-generator.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "Msc-generator.h"
#include "MainFrm.h"

#include "IpFrame.h"
#include "MscGenDoc.h"
#include "MscGenView.h"
#include "version.h"

#include <sstream>
#include <vector>
#include <list>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };
	CMFCLinkCtrl m_btnLink;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog( );

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_URL, m_btnLink);
}

BOOL CAboutDlg::OnInitDialog( ) 
{
	bool a = CDialog::OnInitDialog();
	m_btnLink.SetURL(_T("http://msc-generator.sourcforge.net"));
	m_btnLink.SetTooltip(_T("Visit the Msc-generator site"));
	m_btnLink.SizeToContent();
	return a;
}


// COptionDlg dialog

// COptionDlg dialog

class COptionDlg : public CDialog
{
	DECLARE_DYNCREATE(COptionDlg)

public:
	COptionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COptionDlg();
// Overrides

// Dialog Data
	enum { IDD = IDD_DIALOG_OPTIONS};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClicked();

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_Pedantic;
	BOOL m_Warnings;
	CString m_DefaultText;
	int m_TextEditorRadioButtons;
	CString m_TextEditStartCommand;
	CString m_TextEditorJumpToLineCommand;
	bool m_bNppExists;
	BOOL m_bPB_Editing;
	BOOL m_bPB_Embedded;
	BOOL m_bAlwaysOpen;
	BOOL m_bCsh;
	int m_nCshScheme;
	BOOL m_bSmartIdent;
};


IMPLEMENT_DYNCREATE(COptionDlg, CDialog)

COptionDlg::COptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionDlg::IDD, pParent)
	, m_Pedantic(FALSE)
	, m_Warnings(FALSE)
	, m_DefaultText(_T(""))
	, m_bNppExists(false)
	, m_bPB_Editing(FALSE)
	, m_bPB_Embedded(FALSE)
	, m_bAlwaysOpen(FALSE)
	, m_bCsh(FALSE)
	, m_bSmartIdent(FALSE)
	, m_nCshScheme(0)
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
	DDX_Check(pDX, IDC_CHECK_PB_EDITING, m_bPB_Editing);
	DDX_Check(pDX, IDC_CHECK_PB_EMBEDDED, m_bPB_Embedded);
	DDX_Check(pDX, IDC_CHECK_CSH, m_bCsh);
	DDX_Check(pDX, IDC_CHECK_SMART_IDENT, m_bSmartIdent);
	DDX_Check(pDX, IDC_CHECK_ALWAYSOPEN, m_bAlwaysOpen);
	DDX_CBIndex(pDX, IDC_COMBO_CSH, m_nCshScheme);
	
}

BOOL COptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	GetDlgItem(IDC_EDIT1)->EnableWindow(m_TextEditorRadioButtons==2);
	GetDlgItem(IDC_EDIT2)->EnableWindow(m_TextEditorRadioButtons==2);
	GetDlgItem(IDC_RADIO2)->EnableWindow(m_bNppExists);
	GetDlgItem(IDC_COMBO_CSH)->EnableWindow(m_bCsh);
	GetDlgItem(IDC_CHECK_SMART_IDENT)->EnableWindow(m_bCsh);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COptionDlg::OnBnClicked()
{
	bool otherSet = ((CButton*)this->GetDlgItem(IDC_RADIO3))->GetCheck();
	GetDlgItem(IDC_EDIT1)->EnableWindow(otherSet);
	GetDlgItem(IDC_EDIT2)->EnableWindow(otherSet);
	bool cshSet = ((CButton*)this->GetDlgItem(IDC_CHECK_CSH))->GetCheck();
	GetDlgItem(IDC_COMBO_CSH)->EnableWindow(cshSet);
	GetDlgItem(IDC_CHECK_SMART_IDENT)->EnableWindow(cshSet);
}

BEGIN_MESSAGE_MAP(COptionDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO1, &COptionDlg::OnBnClicked)
	ON_BN_CLICKED(IDC_RADIO2, &COptionDlg::OnBnClicked)
	ON_BN_CLICKED(IDC_RADIO3, &COptionDlg::OnBnClicked)
	ON_BN_CLICKED(IDC_CHECK_CSH, &COptionDlg::OnBnClicked)
END_MESSAGE_MAP()

// CMscGenApp

BEGIN_MESSAGE_MAP(CMscGenApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CMscGenApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_EDIT_PREFERENCES, OnEditPreferences)
END_MESSAGE_MAP()


// CMscGenApp construction

CMscGenApp::CMscGenApp()
{
	m_bHiColorIcons = TRUE;

	// Place all significant initialization in InitInstance
	m_pWndOutputView = 0;
	m_pWndEditor = 0;
	m_bFullScreenViewMode = false;
}


// The one and only CMscGenApp object

CMscGenApp theApp;
// This identifier was generated to be statistically unique for your app
// You may change it if you prefer to choose a specific identifier
// {453AC3C8-F260-4D88-832A-EC957E92FDEC}
static const CLSID clsid =
{ 0x453AC3C8, 0xF260, 0x4D88, { 0x83, 0x2A, 0xEC, 0x95, 0x7E, 0x92, 0xFD, 0xEC } };


UINT CheckVersionFreshness(LPVOID);

// CMscGenApp initialization

BOOL CMscGenApp::InitInstance()
{
	srand((unsigned)time(NULL)); 
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();
	AfxInitRichEdit();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Zoltan Turanyi"));
	LoadStdProfileSettings(9);  // Load standard INI file options (including MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMscGenDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CMscGenView));
	if (!pDocTemplate)
		return FALSE;
	pDocTemplate->SetServerInfo(
		IDR_SRVR_EMBEDDED, IDR_SRVR_INPLACE,
		RUNTIME_CLASS(CInPlaceFrame));
	AddDocTemplate(pDocTemplate);
	// Connect the COleTemplateServer to the document template
	//  The COleTemplateServer creates new documents on behalf
	//  of requesting OLE containers by using information
	//  specified in the document template
	m_server.ConnectTemplate(clsid, pDocTemplate, TRUE);
		// Note: SDI applications register server objects only if /Embedding
		//   or /Automation is present on the command line

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	//Read options from the registry
	ReadRegistryValues(false);
	//Start process that checks if we are the latest version
	CWinThread* check_thread  = AfxBeginThread(&CheckVersionFreshness, NULL);

	// App was launched with /Embedding or /Automation switch.
	// Run app as automation server.
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
	#ifdef _DEBUG
		MessageBox(0, "In Msc-generator::Appinit", "aa", 0);
	#endif
		// Register all OLE server factories as running.  This enables the
		//  OLE libraries to create objects from other applications
		COleTemplateServer::RegisterAll();

		// Don't show the main window
		return TRUE;
	}
	// App was launched with /Unregserver or /Unregister switch.  Unregister
	// typelibrary.  Other unregistration occurs in ProcessShellCommand().
	else if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
	{
		UnregisterShellFileTypes();
		m_server.UpdateRegistry(OAT_DOC_OBJECT_SERVER, NULL, NULL, FALSE);
	}
	// App was launched standalone or with other switches (e.g. /Register
	// or /Regserver).  Update registry entries, including typelibrary.
	else if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppRegister)
	{
		m_server.UpdateRegistry(OAT_DOC_OBJECT_SERVER);
	}
	// App started with other switches or standalone, see if there is -T to do a command line run
	//else if (!CommandLineMain(m_lpCmdLine))
	//	return FALSE;

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	return TRUE;
}


// App command to run the dialog
void CMscGenApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMscGenApp customization load/save methods

void CMscGenApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CMscGenApp::LoadCustomState()
{
}

void CMscGenApp::SaveCustomState()
{
}

bool CMscGenApp::FillDesignDesignCombo(const char *current, bool updateComboContent) {
	//ok now designs contains a set of of designs, preable contains a concatenation of designlib text
	//Add the list of designs to the combo boxes
	CObList list;
	CMFCToolBar::GetCommandButtons(ID_DESIGN_DESIGN, list);
	POSITION p = list.GetHeadPosition();
	bool ret=true;
	while (p) {
		CMFCToolBarComboBoxButton *combo = static_cast<CMFCToolBarComboBoxButton*>(list.GetNext(p));
		if (updateComboContent || combo->GetCount()==0) {
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
		}
		//restore the selection to the given style if it can be found
		int index = (current==NULL || current[0]==0) ? 0 : combo->FindItem(current);
		ret = (index != CB_ERR);
		combo->SelectItem(ret?index:0);
	}
	return ret;
}

void CMscGenApp::FillDesignPageCombo(int no_pages, int page)
{
	CObList list;
	CMFCToolBar::GetCommandButtons(ID_DESIGN_PAGE, list);

	POSITION p = list.GetHeadPosition();
	while(p) {
		CMFCToolBarComboBoxButton *combo = static_cast<CMFCToolBarComboBoxButton*>(list.GetNext(p));
		if (!combo) continue;
		if (no_pages == 1 && combo->GetCount() == 1) continue;
		//If the combo shows different number of pages then we have, update the combo
		if (no_pages+1 != combo->GetCount()) {
			combo->RemoveAllItems();
			//Fill combo list with the appropriate number of pages
			combo->AddItem("(all)", 0);
			CString str;
			if (no_pages > 1)
				for (int i=1; i<=no_pages; i++) {
					str.Format("%d", i);
					combo->AddItem(str, i);
				}
			combo->SetDropDownHeight(250);
		}
		//Set the index to the current page
		combo->SelectItem(page, TRUE);
	} 
}

void CMscGenApp::OnEditPreferences()
{
	CMainFrame *pMainWnd = dynamic_cast<CMainFrame*>(GetMainWnd());
	ASSERT(pMainWnd!=NULL);
	ASSERT(pMainWnd->GetActiveView() != NULL);
	CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(pMainWnd->GetActiveView()->GetDocument());
	ASSERT(pDoc != NULL);

	COptionDlg optionDlg;
	optionDlg.m_Pedantic = m_Pedantic;
	optionDlg.m_Warnings = m_Warnings;
	optionDlg.m_bPB_Editing  = m_bPB_Editing;
	optionDlg.m_bPB_Embedded = m_bPB_Embedded;
	optionDlg.m_bAlwaysOpen  = m_bAlwaysOpen;
	optionDlg.m_bCsh		 = m_bCsh;
	optionDlg.m_nCshScheme   = m_nCshScheme;
	optionDlg.m_bSmartIdent  = m_bSmartIdent;

	EnsureCRLF(m_DefaultText);
	optionDlg.m_DefaultText = m_DefaultText;
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
		if (optionDlg.m_DefaultText != m_DefaultText) {
			m_DefaultText = optionDlg.m_DefaultText;
			EnsureCRLF(m_DefaultText);
			WriteProfileString(REG_SECTION_SETTINGS, REG_KEY_DEFAULTTEXT, m_DefaultText);
		}
		if (optionDlg.m_bSmartIdent != m_bSmartIdent) 
			WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_SMARTIDENT, m_bSmartIdent = optionDlg.m_bSmartIdent);

		bool recompile = (m_Pedantic != (bool)optionDlg.m_Pedantic) ||
			(m_bPB_Editing != optionDlg.m_bPB_Editing) ||
			(m_bPB_Embedded != optionDlg.m_bPB_Embedded);
		bool updateCSH = (optionDlg.m_bCsh != m_bCsh) || (m_bCsh &&	optionDlg.m_nCshScheme != m_nCshScheme);

		WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_PEDANTIC, m_Pedantic = optionDlg.m_Pedantic);
		WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_WARNINGS, m_Warnings = optionDlg.m_Warnings);
		WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_PB_EDITING, m_bPB_Editing = optionDlg.m_bPB_Editing);
		WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_PB_EMBEDDED, m_bPB_Embedded = optionDlg.m_bPB_Embedded);
		WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_ALWAYSOPEN, m_bAlwaysOpen = optionDlg.m_bAlwaysOpen);
		WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_CSHENABLED, m_bCsh = optionDlg.m_bCsh);
		WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_CSHSCHEME, m_nCshScheme = optionDlg.m_nCshScheme);

		bool bRestartEditor = false;
		EEditorType temp;
		switch (optionDlg.m_TextEditorRadioButtons) {
			default:
			case 0: temp = NOTEPAD; break;
			case 1: temp = m_NppPath.GetLength()?NPP:NOTEPAD; break;
			case 2: temp = OTHER; break;
		}
		if (m_sStartTextEditor != optionDlg.m_TextEditStartCommand) {
			m_sStartTextEditor = optionDlg.m_TextEditStartCommand;
			WriteProfileString(REG_SECTION_SETTINGS, REG_KEY_STARTTEXTEDITOR, m_sStartTextEditor);
			if (temp==OTHER && pDoc->m_ExternalEditor.IsRunning()) 
				bRestartEditor=true;
		}
		if (m_sJumpToLine != optionDlg.m_TextEditorJumpToLineCommand) {
			m_sJumpToLine = optionDlg.m_TextEditorJumpToLineCommand;
			WriteProfileString(REG_SECTION_SETTINGS, REG_KEY_JUMPTOLINE, m_sJumpToLine);
		}
		if (temp != m_iTextEditorType) {
			m_iTextEditorType = temp;
			WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_TEXTEDITORTYPE, m_iTextEditorType);
			bRestartEditor=true;
			switch (m_iTextEditorType) {
				case NPP:
					m_sStartTextEditor = "\"" +m_NppPath+ "\" -multiInst -nosession %n";
					m_sJumpToLine = "\"" +m_NppPath+ "\" -n%l -nosession %n";
					break;
				default:
				case NOTEPAD:
					m_sStartTextEditor = "Notepad.exe %n";
					m_sJumpToLine = "";
					break;
				case OTHER:
					break; //values already set by user
			}
		}
		if (bRestartEditor) 
			pDoc->m_ExternalEditor.Restart(STOPEDITOR_WAIT);

		if (recompile) 
			pDoc->ShowEditingChart(false);     //Do not change zoom, merely recompile & re-issue 
		if (updateCSH && IsInternalEditorRunning())
			m_pWndEditor->m_ctrlEditor.UpdateCsh(true);
	}
}

//C:\Windows\Notepad.exe %n;"C:\Program Files\Notepad++\notepad++.exe" -multiInst -nosession %n;
//;-n%l -nosession %n;


void CMscGenApp::ReadRegistryValues(bool reportProblem) 
{
	//Load Registry values
	m_Pedantic = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_PEDANTIC, FALSE);
	m_Warnings = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_WARNINGS, TRUE);
	m_bPB_Editing  = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_PB_EDITING, FALSE);
	m_bPB_Embedded = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_PB_EMBEDDED, FALSE);
	m_bAlwaysOpen  = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_ALWAYSOPEN, FALSE);
	m_bCsh         = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_CSHENABLED, TRUE);
	m_nCshScheme   = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_CSHSCHEME, 1);
	if (m_nCshScheme >= CSH_SCHEME_MAX) m_nCshScheme = 1;
	m_bSmartIdent  = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_SMARTIDENT, TRUE);
	m_trackLineColor = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_TRACKLINERGBA, RGBA(128, 0, 0, 255));
	m_trackFillColor = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_TRACKFILLRGBA, RGBA(255, 0, 0, 128));

	m_NppPath = "C:\\Program Files\\Notepad++\\notepad++.exe";
	CFileFind finder;
	if (!finder.FindFile(m_NppPath))
		m_NppPath.Empty(); //empty value means no NPP on this system

	switch (GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_TEXTEDITORTYPE, NPP)) {
		case 1:
			if (m_NppPath.GetLength()) {
				m_sStartTextEditor = "\"" +m_NppPath+ "\" -multiInst -nosession %n";
				m_sJumpToLine = "\"" +m_NppPath+ "\" -n%l -nosession %n";
				m_iTextEditorType = NPP;
				break;
			}
			//fallback if notepad++ not found
		default:
		case 0:
			m_sStartTextEditor = "Notepad.exe %n";
			m_sJumpToLine = "";
			m_iTextEditorType = NOTEPAD;
			break;
		case 2:
			m_sStartTextEditor = GetProfileString(REG_SECTION_SETTINGS, REG_KEY_STARTTEXTEDITOR, "Notepad.exe %n");
			m_sJumpToLine = GetProfileString(REG_SECTION_SETTINGS, REG_KEY_JUMPTOLINE, "");
			m_iTextEditorType = OTHER;
			break;
	}
	m_DefaultText= GetProfileString(REG_SECTION_SETTINGS, REG_KEY_DEFAULTTEXT, 
		"#Default signalling chart\r\n"
		": You can change the default chart\r\n"
		"  in the \f(courier new)Edit|Preferences...\f() menu.;\r\n"
		"a,b,c;\r\n"
		"b->c:trallala;\r\n"
		"a->b: new;\r\n");

	ReadDesigns(reportProblem); //fills m_ChartSourcePreamble appropriately, default filename applies
	FillDesignDesignCombo("", true);
	m_CopyrightText = "\\md(0)\\mu(2)\\mr(0)\\mn(10)\\f(arial)\\pr\\c(150,150,150)"
		              "http://msc-generator.sourceforge.net";
	m_CopyrightText.AppendFormat(" v%d.%d.%d", LIBMSCGEN_MAJOR, LIBMSCGEN_MINOR, LIBMSCGEN_SUPERMINOR);

	CHARFORMAT cf;
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_UNDERLINE|CFM_ITALIC|CFM_BOLD|CFM_COLOR;
	cf.dwEffects = 0;
	cf.crTextColor = RGB(0,0,0);
	for (int scheme=0; scheme<CSH_SCHEME_MAX; scheme++)
		for (int i=0; i<COLOR_MAX; i++)
			m_csh_cf[scheme][i] = cf;

	//CSH_SCHEME ==0 is the Minimal one
	m_csh_cf[0][COLOR_KEYWORD].crTextColor =           RGB(  0,  0,  0); m_csh_cf[0][COLOR_KEYWORD].dwEffects = CFM_BOLD;
	m_csh_cf[0][COLOR_ATTRNAME].crTextColor =          RGB(  0,  0,  0); m_csh_cf[0][COLOR_ATTRNAME].dwEffects = CFM_BOLD;
	m_csh_cf[0][COLOR_ATTRNAME_PARTIAL].crTextColor =  RGB(  0,  0,  0); m_csh_cf[0][COLOR_ATTRNAME_PARTIAL].dwEffects = 0;
	m_csh_cf[0][COLOR_OPTIONNAME].crTextColor =        RGB(  0,  0,  0); m_csh_cf[0][COLOR_OPTIONNAME].dwEffects = CFM_BOLD;
	m_csh_cf[0][COLOR_OPTIONNAME_PARTIAL].crTextColor =RGB(  0,  0,  0); m_csh_cf[0][COLOR_OPTIONNAME_PARTIAL].dwEffects = 0;
	m_csh_cf[0][COLOR_EQUAL].crTextColor =             RGB(  0,  0,  0); m_csh_cf[0][COLOR_EQUAL].dwEffects = 0;
	m_csh_cf[0][COLOR_SEMICOLON].crTextColor =         RGB(  0,  0,  0); m_csh_cf[0][COLOR_SEMICOLON].dwEffects = 0;
	m_csh_cf[0][COLOR_COLON].crTextColor =             RGB(  0,  0,  0); m_csh_cf[0][COLOR_COLON].dwEffects = 0;
	m_csh_cf[0][COLOR_BRACE].crTextColor =             RGB(  0,  0,  0); m_csh_cf[0][COLOR_BRACE].dwEffects = 0;
	m_csh_cf[0][COLOR_BRACKET].crTextColor =           RGB(  0,  0,  0); m_csh_cf[0][COLOR_BRACKET].dwEffects = 0;
	m_csh_cf[0][COLOR_SYMBOL].crTextColor =            RGB( 20, 20,  0); m_csh_cf[0][COLOR_SYMBOL].dwEffects = CFM_BOLD;
	m_csh_cf[0][COLOR_DESIGNNAME].crTextColor =        RGB( 50,  0,  0); m_csh_cf[0][COLOR_DESIGNNAME].dwEffects = 0;
	m_csh_cf[0][COLOR_STYLENAME].crTextColor =         RGB( 50,  0,  0); m_csh_cf[0][COLOR_STYLENAME].dwEffects = 0;
	m_csh_cf[0][COLOR_COLORNAME].crTextColor =         RGB( 50,  0,  0); m_csh_cf[0][COLOR_COLORNAME].dwEffects = 0;
	m_csh_cf[0][COLOR_ENTITYNAME].crTextColor =        RGB(  0, 50,  0); m_csh_cf[0][COLOR_ENTITYNAME].dwEffects = CFM_BOLD;
	m_csh_cf[0][COLOR_ENTITYNAME_FIRST].crTextColor =  RGB(  0,  0,  0); m_csh_cf[0][COLOR_ENTITYNAME_FIRST].dwEffects = CFM_BOLD;
	m_csh_cf[0][COLOR_MARKERNAME].crTextColor =        RGB(  0,  0, 50); m_csh_cf[0][COLOR_MARKERNAME].dwEffects = CFM_BOLD;
	m_csh_cf[0][COLOR_ATTRVALUE].crTextColor =         RGB(  0,  0,  0); m_csh_cf[0][COLOR_ATTRVALUE].dwEffects = 0;
	m_csh_cf[0][COLOR_COLORDEF].crTextColor =          RGB(  0,  0,  0); m_csh_cf[0][COLOR_COLORDEF].dwEffects = 0;
	m_csh_cf[0][COLOR_LABEL_TEXT].crTextColor =        RGB(  0,  0,  0); m_csh_cf[0][COLOR_LABEL_TEXT].dwEffects = 0;
	m_csh_cf[0][COLOR_LABEL_ESCAPE].crTextColor =      RGB(  0,  0,  0); m_csh_cf[0][COLOR_LABEL_ESCAPE].dwEffects = CFM_BOLD;
	m_csh_cf[0][COLOR_ERROR].crTextColor =             RGB( 50, 50, 50); m_csh_cf[0][COLOR_ERROR].dwEffects = 0;
	m_csh_cf[0][COLOR_COMMENT].crTextColor =           RGB(100,100,100); m_csh_cf[0][COLOR_COMMENT].dwEffects = CFM_ITALIC;
	m_csh_cf[0][COLOR_ACTIVE_ERROR].dwMask = CFM_UNDERLINE;              m_csh_cf[0][COLOR_ACTIVE_ERROR].dwEffects = CFM_UNDERLINE;

	//CSH_SCHEME ==1 is the Standard one
	m_csh_cf[1][COLOR_KEYWORD].crTextColor =           RGB(128,128,  0); m_csh_cf[1][COLOR_KEYWORD].dwEffects = CFM_BOLD;
	m_csh_cf[1][COLOR_ATTRNAME].crTextColor =          RGB(128,128,  0); m_csh_cf[1][COLOR_ATTRNAME].dwEffects = CFM_BOLD;
	m_csh_cf[1][COLOR_ATTRNAME_PARTIAL].crTextColor =  RGB(128,128,  0); m_csh_cf[1][COLOR_ATTRNAME_PARTIAL].dwEffects = 0;
	m_csh_cf[1][COLOR_OPTIONNAME].crTextColor =        RGB(128,128,  0); m_csh_cf[1][COLOR_OPTIONNAME].dwEffects = CFM_BOLD;
	m_csh_cf[1][COLOR_OPTIONNAME_PARTIAL].crTextColor =RGB(128,128,  0); m_csh_cf[1][COLOR_OPTIONNAME_PARTIAL].dwEffects = 0;
	m_csh_cf[1][COLOR_EQUAL].crTextColor =             RGB(  0,  0,  0); m_csh_cf[1][COLOR_EQUAL].dwEffects = 0;
	m_csh_cf[1][COLOR_SEMICOLON].crTextColor =         RGB(  0,  0,  0); m_csh_cf[1][COLOR_SEMICOLON].dwEffects = 0;
	m_csh_cf[1][COLOR_COLON].crTextColor =             RGB(  0,  0,  0); m_csh_cf[1][COLOR_COLON].dwEffects = 0;
	m_csh_cf[1][COLOR_BRACE].crTextColor =             RGB(  0,  0,  0); m_csh_cf[1][COLOR_BRACE].dwEffects = 0;
	m_csh_cf[1][COLOR_BRACKET].crTextColor =           RGB(  0,  0,  0); m_csh_cf[1][COLOR_BRACKET].dwEffects = 0;
	m_csh_cf[1][COLOR_SYMBOL].crTextColor =            RGB(255,  0,  0); m_csh_cf[1][COLOR_SYMBOL].dwEffects = CFM_BOLD;
	m_csh_cf[1][COLOR_DESIGNNAME].crTextColor =        RGB(  0,  0,  0); m_csh_cf[1][COLOR_DESIGNNAME].dwEffects = 0;
	m_csh_cf[1][COLOR_STYLENAME].crTextColor =         RGB(  0,  0,  0); m_csh_cf[1][COLOR_STYLENAME].dwEffects = 0;
	m_csh_cf[1][COLOR_COLORNAME].crTextColor =         RGB(  0,  0,  0); m_csh_cf[1][COLOR_COLORNAME].dwEffects = 0;
	m_csh_cf[1][COLOR_ENTITYNAME].crTextColor =        RGB(200,  0,  0); m_csh_cf[1][COLOR_ENTITYNAME].dwEffects = CFM_BOLD;
	m_csh_cf[1][COLOR_ENTITYNAME_FIRST].crTextColor =  RGB(200,  0,  0); m_csh_cf[1][COLOR_ENTITYNAME_FIRST].dwEffects = CFM_BOLD|CFM_UNDERLINE;
	m_csh_cf[1][COLOR_MARKERNAME].crTextColor =        RGB(  0,200,  0); m_csh_cf[1][COLOR_MARKERNAME].dwEffects = CFM_BOLD;
	m_csh_cf[1][COLOR_ATTRVALUE].crTextColor =         RGB(  0,  0,200); m_csh_cf[1][COLOR_ATTRVALUE].dwEffects = 0;
	m_csh_cf[1][COLOR_COLORDEF].crTextColor =          RGB(  0,  0,200); m_csh_cf[1][COLOR_COLORDEF].dwEffects = 0;
	m_csh_cf[1][COLOR_LABEL_TEXT].crTextColor =        RGB(  0,  0,  0); m_csh_cf[1][COLOR_LABEL_TEXT].dwEffects = 0;
	m_csh_cf[1][COLOR_LABEL_ESCAPE].crTextColor =      RGB(  0,150,  0); m_csh_cf[1][COLOR_LABEL_ESCAPE].dwEffects = CFM_BOLD;
	m_csh_cf[1][COLOR_ERROR].crTextColor =             RGB( 50, 50, 50); m_csh_cf[1][COLOR_ERROR].dwEffects = 0;
	m_csh_cf[1][COLOR_COMMENT].crTextColor =           RGB(100,100,100); m_csh_cf[1][COLOR_COMMENT].dwEffects = CFM_ITALIC;
	m_csh_cf[1][COLOR_ACTIVE_ERROR].dwMask = CFM_UNDERLINE;              m_csh_cf[1][COLOR_ACTIVE_ERROR].dwEffects = CFM_UNDERLINE;

	//CSH_SCHEME ==2 is the Colorful one
	m_csh_cf[2][COLOR_KEYWORD].crTextColor =           RGB(128,128,  0); m_csh_cf[2][COLOR_KEYWORD].dwEffects = CFM_BOLD;
	m_csh_cf[2][COLOR_ATTRNAME].crTextColor =          RGB(128,128,  0); m_csh_cf[2][COLOR_ATTRNAME].dwEffects = CFM_BOLD;
	m_csh_cf[2][COLOR_ATTRNAME_PARTIAL].crTextColor =  RGB(128,128,  0); m_csh_cf[2][COLOR_ATTRNAME_PARTIAL].dwEffects = 0;
	m_csh_cf[2][COLOR_OPTIONNAME].crTextColor =        RGB(128,128,  0); m_csh_cf[2][COLOR_OPTIONNAME].dwEffects = CFM_BOLD;
	m_csh_cf[2][COLOR_OPTIONNAME_PARTIAL].crTextColor =RGB(128,128,  0); m_csh_cf[2][COLOR_OPTIONNAME_PARTIAL].dwEffects = 0;
	m_csh_cf[2][COLOR_EQUAL].crTextColor =             RGB(  0,  0,  0); m_csh_cf[2][COLOR_EQUAL].dwEffects = 0;
	m_csh_cf[2][COLOR_SEMICOLON].crTextColor =         RGB(  0,  0,  0); m_csh_cf[2][COLOR_SEMICOLON].dwEffects = 0;
	m_csh_cf[2][COLOR_COLON].crTextColor =             RGB(  0,  0,  0); m_csh_cf[2][COLOR_COLON].dwEffects = 0;
	m_csh_cf[2][COLOR_BRACE].crTextColor =             RGB(  0,  0,  0); m_csh_cf[2][COLOR_BRACE].dwEffects = 0;
	m_csh_cf[2][COLOR_BRACKET].crTextColor =           RGB(  0,  0,  0); m_csh_cf[2][COLOR_BRACKET].dwEffects = 0;
	m_csh_cf[2][COLOR_SYMBOL].crTextColor =            RGB(  0,128,128); m_csh_cf[2][COLOR_SYMBOL].dwEffects = CFM_BOLD;
	m_csh_cf[2][COLOR_DESIGNNAME].crTextColor =        RGB(128,  0,  0); m_csh_cf[2][COLOR_DESIGNNAME].dwEffects = 0;
	m_csh_cf[2][COLOR_STYLENAME].crTextColor =         RGB(128,  0,  0); m_csh_cf[2][COLOR_STYLENAME].dwEffects = 0;
	m_csh_cf[2][COLOR_COLORNAME].crTextColor =         RGB(128,  0,  0); m_csh_cf[2][COLOR_COLORNAME].dwEffects = 0;
	m_csh_cf[2][COLOR_ENTITYNAME].crTextColor =        RGB(200,  0,  0); m_csh_cf[2][COLOR_ENTITYNAME].dwEffects = CFM_BOLD;
	m_csh_cf[2][COLOR_ENTITYNAME_FIRST].crTextColor =  RGB(200,  0,  0); m_csh_cf[2][COLOR_ENTITYNAME_FIRST].dwEffects = CFM_BOLD|CFM_UNDERLINE;
	m_csh_cf[3][COLOR_MARKERNAME].crTextColor =        RGB(  0,255,  0); m_csh_cf[2][COLOR_MARKERNAME].dwEffects = CFM_BOLD;
	m_csh_cf[2][COLOR_ATTRVALUE].crTextColor =         RGB(  0,  0,255); m_csh_cf[2][COLOR_ATTRVALUE].dwEffects = 0;
	m_csh_cf[2][COLOR_COLORDEF].crTextColor =          RGB(  0,  0,255); m_csh_cf[2][COLOR_COLORDEF].dwEffects = 0;
	m_csh_cf[2][COLOR_LABEL_TEXT].crTextColor =        RGB(  0,200,  0); m_csh_cf[2][COLOR_LABEL_TEXT].dwEffects = 0;
	m_csh_cf[2][COLOR_LABEL_ESCAPE].crTextColor =      RGB(255,  0,  0); m_csh_cf[2][COLOR_LABEL_ESCAPE].dwEffects = CFM_BOLD;
	m_csh_cf[2][COLOR_ERROR].crTextColor =             RGB( 50, 50, 50); m_csh_cf[2][COLOR_ERROR].dwEffects = 0;
	m_csh_cf[2][COLOR_COMMENT].crTextColor =           RGB(100,100,100); m_csh_cf[2][COLOR_COMMENT].dwEffects = CFM_ITALIC;
	m_csh_cf[2][COLOR_ACTIVE_ERROR].dwMask = CFM_UNDERLINE;              m_csh_cf[2][COLOR_ACTIVE_ERROR].dwEffects = CFM_UNDERLINE;

	//CSH_SCHEME ==3 is the Error oriented one
	m_csh_cf[3][COLOR_KEYWORD].crTextColor =           RGB(  0,  0,  0); m_csh_cf[3][COLOR_KEYWORD].dwEffects = CFM_BOLD;
	m_csh_cf[3][COLOR_ATTRNAME].crTextColor =          RGB(  0,  0,  0); m_csh_cf[3][COLOR_ATTRNAME].dwEffects = CFM_BOLD;
	m_csh_cf[3][COLOR_ATTRNAME_PARTIAL].crTextColor =  RGB(  0,  0,  0); m_csh_cf[3][COLOR_ATTRNAME_PARTIAL].dwEffects = 0;
	m_csh_cf[3][COLOR_OPTIONNAME].crTextColor =        RGB(  0,  0,  0); m_csh_cf[3][COLOR_OPTIONNAME].dwEffects = CFM_BOLD;
	m_csh_cf[3][COLOR_OPTIONNAME_PARTIAL].crTextColor =RGB(  0,  0,  0); m_csh_cf[2][COLOR_OPTIONNAME_PARTIAL].dwEffects = 0;
	m_csh_cf[3][COLOR_EQUAL].crTextColor =             RGB(  0,  0,  0); m_csh_cf[3][COLOR_EQUAL].dwEffects = 0;
	m_csh_cf[3][COLOR_SEMICOLON].crTextColor =         RGB(  0,  0,  0); m_csh_cf[3][COLOR_SEMICOLON].dwEffects = 0;
	m_csh_cf[3][COLOR_COLON].crTextColor =             RGB(  0,  0,  0); m_csh_cf[3][COLOR_COLON].dwEffects = 0;
	m_csh_cf[3][COLOR_BRACE].crTextColor =             RGB(  0,  0,  0); m_csh_cf[3][COLOR_BRACE].dwEffects = 0;
	m_csh_cf[3][COLOR_BRACKET].crTextColor =           RGB(  0,  0,  0); m_csh_cf[3][COLOR_BRACKET].dwEffects = 0;
	m_csh_cf[3][COLOR_SYMBOL].crTextColor =            RGB( 20, 20,  0); m_csh_cf[3][COLOR_SYMBOL].dwEffects = CFM_BOLD;
	m_csh_cf[3][COLOR_DESIGNNAME].crTextColor =        RGB( 50,  0,  0); m_csh_cf[3][COLOR_DESIGNNAME].dwEffects = 0;
	m_csh_cf[3][COLOR_STYLENAME].crTextColor =         RGB( 50,  0,  0); m_csh_cf[3][COLOR_STYLENAME].dwEffects = 0;
	m_csh_cf[3][COLOR_COLORNAME].crTextColor =         RGB( 50,  0,  0); m_csh_cf[3][COLOR_COLORNAME].dwEffects = 0;
	m_csh_cf[3][COLOR_ENTITYNAME].crTextColor =        RGB(  0, 50,  0); m_csh_cf[3][COLOR_ENTITYNAME].dwEffects = CFM_BOLD;
	m_csh_cf[3][COLOR_ENTITYNAME_FIRST].crTextColor =  RGB(  0,  0,  0); m_csh_cf[3][COLOR_ENTITYNAME_FIRST].dwEffects = CFM_BOLD|CFM_UNDERLINE;
	m_csh_cf[3][COLOR_MARKERNAME].crTextColor =        RGB(  0,  0, 50); m_csh_cf[3][COLOR_MARKERNAME].dwEffects = CFM_BOLD;
	m_csh_cf[3][COLOR_ATTRVALUE].crTextColor =         RGB(  0,  0,  0); m_csh_cf[3][COLOR_ATTRVALUE].dwEffects = 0;
	m_csh_cf[3][COLOR_COLORDEF].crTextColor =          RGB(  0,  0,  0); m_csh_cf[3][COLOR_COLORDEF].dwEffects = 0;
	m_csh_cf[3][COLOR_LABEL_TEXT].crTextColor =        RGB(  0,  0,  0); m_csh_cf[3][COLOR_LABEL_TEXT].dwEffects = 0;
	m_csh_cf[3][COLOR_LABEL_ESCAPE].crTextColor =      RGB(  0,  0,  0); m_csh_cf[3][COLOR_LABEL_ESCAPE].dwEffects = CFM_BOLD;
	m_csh_cf[3][COLOR_ERROR].crTextColor =             RGB(255,  0,  0); m_csh_cf[3][COLOR_ERROR].dwEffects = CFM_ITALIC;
	m_csh_cf[3][COLOR_COMMENT].crTextColor =           RGB(100,100,100); m_csh_cf[3][COLOR_COMMENT].dwEffects = CFM_ITALIC;
	m_csh_cf[3][COLOR_ACTIVE_ERROR].crTextColor =      RGB(255,  0,  0); m_csh_cf[3][COLOR_ACTIVE_ERROR].dwEffects = CFM_UNDERLINE;
}

//Read the designs from m_DesignDir, display a modal dialog if there is a problem.
//If no problem, update m_ChartSourcePreamble and return true
bool CMscGenApp::ReadDesigns(bool reportProblem, const char *fileName)
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
		CDrawingChartData data;
		if (data.Load(finder.GetFilePath(), false)) {
			unsigned num = data.GetErrorNum(true);
			if (num) {
				msg.Append("Problems in design file: ");
				msg.Append(finder.GetFileName());
				msg.Append("\n");
				for (int i=0; i<num; i++) {
					msg.Append(data.GetErrorText(i, true));
					msg.Append("\n");
				}
				errors = true;
			}
			preamble.Append(data.GetText());
			if (m_SetOfDesigns.GetLength()>0) m_SetOfDesigns.Append(" ");
			m_SetOfDesigns.Append(data.GetDesigns());
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


//Version reminder dialog
class CVersionDlg : public CDialog
{
public:
	CVersionDlg() : CDialog(CVersionDlg::IDD) , m_sCurrentVersion(_T(""))
		, m_sLatestVersion(_T(""))
	{}

// Dialog Data
	enum { IDD = IDD_DIALOG_VERSION };
	CMFCLinkCtrl m_btnLink;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog( );

public:
	CString m_sCurrentVersion;
	CString m_sLatestVersion;
};

void CVersionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_URL2, m_btnLink);
	DDX_Text(pDX, IDC_EDIT2, m_sCurrentVersion);
	DDX_Text(pDX, IDC_EDIT3, m_sLatestVersion);
}

BOOL CVersionDlg::OnInitDialog( ) 
{
	bool a = CDialog::OnInitDialog();
	m_btnLink.SetURL(_T("https://sourceforge.net/projects/msc-generator/"));
	m_btnLink.SetTooltip(_T("Download from SourceForge"));
	m_btnLink.SizeToContent();
	return a;
}



UINT CheckVersionFreshness(LPVOID)
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	CString latest_version;
	TRY {
		CInternetSession session("Msc-generator", 1, 0, NULL, NULL, INTERNET_FLAG_DONT_CACHE);
		CHttpConnection *httpconn = session.GetHttpConnection("msc-generator.sourceforge.net", 
			INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE, INTERNET_PORT(80));
		CHttpFile *file = httpconn->OpenRequest("GET", "version", NULL, 1, NULL, NULL, 
			INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE);
		if (!file->SendRequest()) return false;
		if (!file->ReadString(latest_version)) return false;
	} CATCH(CInternetException, pEx) {
		return false;
	}
	END_CATCH

	if (latest_version.GetLength()==0) return false;
	int a=-1,b=-1,c=-1;
	sscanf(latest_version, "v%d.%d.%d", &a, &b, &c);
	//MessageBox(0, "This is what I read: " + latest_version, "Msc-generator", MB_OK);

	//If we are the latest version (or erroneous string read from web), exit
	if (a<LIBMSCGEN_MAJOR) return false;
	if (a==LIBMSCGEN_MAJOR && b<LIBMSCGEN_MINOR) return false;
	if (a==LIBMSCGEN_MAJOR && b==LIBMSCGEN_MINOR && c<=LIBMSCGEN_SUPERMINOR) return false;	

	//If user do not want to get reminded for the new version, exit
	int no_a = pApp->GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_NOREMIND_VERSION_MAJOR, -1);
	int no_b = pApp->GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_NOREMIND_VERSION_MINOR, -1);
	int no_c = pApp->GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_NOREMIND_VERSION_SUPER_MINOR, -1);

	if (no_a == a && no_b == b && no_c == c) return false;
	
	char current[100]; sprintf(current, "v%d.%d.%d", LIBMSCGEN_MAJOR, LIBMSCGEN_MINOR, LIBMSCGEN_SUPERMINOR);
	char latest[100];  sprintf(latest,  "v%d.%d.%d", a, b, c);

	CVersionDlg dlg;
	dlg.m_sCurrentVersion = current;
	dlg.m_sLatestVersion = latest;
	if (dlg.DoModal() == IDOK) {
		pApp->WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_NOREMIND_VERSION_MAJOR, a);
		pApp->WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_NOREMIND_VERSION_MINOR, b);
		pApp->WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_NOREMIND_VERSION_SUPER_MINOR, c);
	}
	return true;
}