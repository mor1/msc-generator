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

// Msc-generator.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "Msc-generator.h"
#include "MainFrm.h"

#include "IpFrame.h"
#include "MscGenDoc.h"
#include "MscGenView.h"
#include "csh.h"
#include "commandline.h" //for VersionText

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
	CMFCLinkCtrl m_btnLink_Latest;

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
    DDX_Control(pDX, IDC_BUTTON_LATEST_URL, m_btnLink_Latest);
}

BOOL CAboutDlg::OnInitDialog( ) 
{
	BOOL a = CDialog::OnInitDialog();
	m_btnLink.SetURL(_T("http://msc-generator.sourceforge.net"));
	m_btnLink.SetTooltip(_T("Visit the Msc-generator site"));
	m_btnLink.SizeToContent();
	CString text = "Msc-generator Version ";
	GetDlgItem(IDC_STATIC_VERSION)->SetWindowText(text + (VersionText()+1));

    CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	int no_a = pApp->GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_LAST_VERSION_SEEN_MAJOR, -1);
	int no_b = pApp->GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_LAST_VERSION_SEEN_MINOR, -1);
	int no_c = pApp->GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_LAST_VERSION_SEEN_SUPER_MINOR, -1);

    m_btnLink_Latest.SetWindowText(_T(""));
    m_btnLink_Latest.SetTooltip(_T("Msc-generator is up-to-date"));
    m_btnLink_Latest.SizeToContent();
    m_btnLink_Latest.EnableWindow(FALSE);

    const CString latest_url = pApp->GetProfileString(REG_SECTION_SETTINGS, REG_KEY_LAST_VERSION_SEEN_URL);
    if (no_a<=0 || no_b<0 || no_c<0 || latest_url.GetLength()==0) return a;

    if (no_a<=LIBMSCGEN_MAJOR) return a;
    if (no_a==LIBMSCGEN_MAJOR && no_b<LIBMSCGEN_MINOR) return a;
    if (no_a==LIBMSCGEN_MAJOR && no_b==LIBMSCGEN_MINOR && no_c<=LIBMSCGEN_SUPERMINOR) return a;

    m_btnLink_Latest.SetURL(latest_url);
    m_btnLink_Latest.SetWindowText(CString("Download latest version (") + VersionText(no_a, no_b, no_c) + CString(")"));
    m_btnLink_Latest.SetTooltip(latest_url);
    m_btnLink_Latest.SizeToContent();
    m_btnLink_Latest.EnableWindow();
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

	DECLARE_MESSAGE_MAP()
public:
	CString m_TextEditStartCommand;
	CString m_TextEditorJumpToLineCommand;
};


IMPLEMENT_DYNCREATE(COptionDlg, CDialog)

COptionDlg::COptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionDlg::IDD, pParent)
{
}

COptionDlg::~COptionDlg()
{
}

void COptionDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, m_TextEditStartCommand);
    DDX_Text(pDX, IDC_EDIT2, m_TextEditorJumpToLineCommand);
}


BEGIN_MESSAGE_MAP(COptionDlg, CDialog)
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
    ON_COMMAND(ID_BUTTON_DEFAULT_TEXT, &CMscGenApp::OnButtonDefaultText)
    ON_UPDATE_COMMAND_UI(ID_BUTTON_DEFAULT_TEXT, &CMscGenApp::OnUpdateButtonDefaultText)
    ON_COMMAND(IDC_CHECK_PEDANTIC, &CMscGenApp::OnCheckPedantic)
    ON_UPDATE_COMMAND_UI(IDC_CHECK_PEDANTIC, &CMscGenApp::OnUpdateCheckPedantic)
    ON_COMMAND(IDC_CHECK_WARNINGS, &CMscGenApp::OnCheckWarnings)
    ON_UPDATE_COMMAND_UI(IDC_CHECK_WARNINGS, &CMscGenApp::OnUpdateCheckWarnings)
    ON_COMMAND(IDC_CHECK_PB_EDITING, &CMscGenApp::OnCheckPbEditing)
    ON_UPDATE_COMMAND_UI(IDC_CHECK_PB_EDITING, &CMscGenApp::OnUpdateCheckPbEditing)
    ON_COMMAND(IDC_CHECK_PB_EMBEDDED, &CMscGenApp::OnCheckPbEmbedded)
    ON_UPDATE_COMMAND_UI(IDC_CHECK_PB_EMBEDDED, &CMscGenApp::OnUpdateCheckPbEmbedded)
    ON_COMMAND(IDC_CHECK_CSH, &CMscGenApp::OnCheckCsh)
    ON_COMMAND(IDC_COMBO_CSH, &CMscGenApp::OnComboCsh)
    ON_UPDATE_COMMAND_UI(IDC_CHECK_CSH, &CMscGenApp::OnUpdateCheckCsh)
    ON_UPDATE_COMMAND_UI(IDC_COMBO_CSH, &CMscGenApp::OnUpdateComboCsh)
    ON_COMMAND(IDC_CHECK_SMART_IDENT, &CMscGenApp::OnCheckSmartIdent)
    ON_COMMAND(IDC_CHECK_CSH_ERROR, &CMscGenApp::OnCheckCshError)
    ON_UPDATE_COMMAND_UI(IDC_CHECK_SMART_IDENT, &CMscGenApp::OnUpdateCheckSmartIdent)
    ON_UPDATE_COMMAND_UI(IDC_CHECK_CSH_ERROR, &CMscGenApp::OnUpdateCheckCshError)
    ON_COMMAND(IDC_CHECK_CSH_ERROR_IN_WINDOW, &CMscGenApp::OnCheckCshErrorInWindow)
    ON_UPDATE_COMMAND_UI(IDC_CHECK_CSH_ERROR_IN_WINDOW, &CMscGenApp::OnUpdateCheckCshErrorInWindow)
    ON_UPDATE_COMMAND_UI(ID_EDIT_PREFERENCES, &CMscGenApp::OnUpdateEditPreferences)
    ON_COMMAND(ID_CHECK_EE_OTHER, &CMscGenApp::OnCheckEeOther)
    ON_COMMAND(ID_CHECK_EE_NOTEPAD, &CMscGenApp::OnCheckEeNotepad)
    ON_COMMAND(ID_CHECK_EE_NOTEPADPP, &CMscGenApp::OnCheckEeNotepadpp)
    ON_UPDATE_COMMAND_UI(ID_CHECK_EE_OTHER, &CMscGenApp::OnUpdateCheckEeOther)
    ON_UPDATE_COMMAND_UI(ID_CHECK_EE_NOTEPAD, &CMscGenApp::OnUpdateCheckEeNotepad)
    ON_UPDATE_COMMAND_UI(ID_CHECK_EE_NOTEPADPP, &CMscGenApp::OnUpdateCheckEeNotepadpp)
    ON_COMMAND(IDC_CHECK_HINTS, &CMscGenApp::OnCheckHints)
    ON_COMMAND(IDC_CHECK_SMART_HINT_COMPACT, &CMscGenApp::OnCheckSmartHintCompact)
    ON_COMMAND(IDC_CHECK_SMART_HINT_FILTER, &CMscGenApp::OnCheckSmartHintFilter)
    ON_COMMAND(IDC_CHECK_SMART_HINT_LINE_START, &CMscGenApp::OnCheckSmartHintLineStart)
    ON_COMMAND(IDC_CHECK_SMART_HINT_ENTITY, &CMscGenApp::OnCheckSmartHintEntity)
    ON_COMMAND(IDC_CHECK_SMART_HINT_ATTR_NAME, &CMscGenApp::OnCheckSmartHintAttrName)
    ON_COMMAND(IDC_CHECK_SMART_HINT_ATTR_VALUE, &CMscGenApp::OnCheckSmartHintAttrValue)
    ON_UPDATE_COMMAND_UI(IDC_CHECK_SMART_HINT_FILTER, &CMscGenApp::OnUpdateCheckSmartHintBoxes)
    ON_UPDATE_COMMAND_UI(IDC_CHECK_SMART_HINT_COMPACT, &CMscGenApp::OnUpdateCheckSmartHintBoxes)
    ON_UPDATE_COMMAND_UI(IDC_CHECK_SMART_HINT_LINE_START, &CMscGenApp::OnUpdateCheckSmartHintBoxes)
    ON_UPDATE_COMMAND_UI(IDC_CHECK_SMART_HINT_ENTITY, &CMscGenApp::OnUpdateCheckSmartHintBoxes)
    ON_UPDATE_COMMAND_UI(IDC_CHECK_SMART_HINT_ATTR_NAME, &CMscGenApp::OnUpdateCheckSmartHintBoxes)
    ON_UPDATE_COMMAND_UI(IDC_CHECK_SMART_HINT_ATTR_VALUE, &CMscGenApp::OnUpdateCheckSmartHintBoxes)
    ON_UPDATE_COMMAND_UI(IDC_CHECK_HINTS, &CMscGenApp::OnUpdateCheckHints)
    ON_COMMAND(ID_BUTTON_TRACK_COLOR, &CMscGenApp::OnButtonTrackColor)
END_MESSAGE_MAP()


// CMscGenApp construction

inline CMscGenDoc *CMscGenApp::GetDoc(void)  
{
	//Get pDoc. I have no clue how to do it for in-place active state: then we have no main window.
	//So in that case we skip external editor restart and recompilation
    CMscGenDoc *pDoc = NULL;
    CMainFrame *pMainWnd = dynamic_cast<CMainFrame*>(GetMainWnd());
    if (pMainWnd!=NULL && pMainWnd->GetActiveView() != NULL) 
		pDoc = dynamic_cast<CMscGenDoc *>(pMainWnd->GetActiveView()->GetDocument());
    _ASSERT(pDoc);
    return pDoc;
}


CMscGenApp::CMscGenApp()
{
	m_bHiColorIcons = TRUE;

	// replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
    char buff[200];
    sprintf(buff, "Msc-generator-%s", VersionText());
	SetAppID(buff);

	// Place all significant initialization in InitInstance
	m_pWndOutputView = 0;
	m_pWndEditor = 0;
	m_bFullScreenViewMode = false;
    m_cacheType = CChartCache::CACHE_NONE;
    m_bShowControls = false;
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

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
    
    EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	AfxInitRichEdit2();
    
    // Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
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

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	//Read options from the registry
	ReadRegistryValues(false);
	//Start process that checks if we are the latest version
	/*CWinThread* check_thread =*/ AfxBeginThread(&CheckVersionFreshness, NULL);

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

    //Set focus to Internal editor, if any
    if (IsInternalEditorRunning() && m_pWndEditor->IsVisible())
		m_pWndEditor->m_ctrlEditor.SetFocus();

	return TRUE;
}

int CMscGenApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
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

DWORD ConvertMscColorSyntaxFlagsToCHARFORMATFlags(int input)
{
	return ((input & COLOR_FLAG_BOLD     ) ? CFM_BOLD      : 0) |
		   ((input & COLOR_FLAG_ITALICS  ) ? CFM_ITALIC    : 0) |
		   ((input & COLOR_FLAG_UNDERLINE) ? CFM_UNDERLINE : 0) |
		   ((input & COLOR_FLAG_COLOR    ) ? CFM_COLOR     : 0);
}

void ConvertMscCshAppearanceToCHARFORMAT(const MscColorSyntaxAppearance &app, CHARFORMAT &cf)
{
	cf.cbSize      = sizeof(cf);
	cf.dwMask      = ConvertMscColorSyntaxFlagsToCHARFORMATFlags(app.mask);
	cf.dwEffects   = ConvertMscColorSyntaxFlagsToCHARFORMATFlags(app.effects);
	cf.crTextColor = RGB(app.r, app.g, app.b);
}

void CMscGenApp::ReadRegistryValues(bool reportProblem) 
{
	//Load Registry values
	m_Pedantic = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_PEDANTIC, FALSE);
	m_Warnings = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_WARNINGS, TRUE);
	m_bPB_Editing     = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_PB_EDITING, FALSE);
	m_bPB_Embedded    = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_PB_EMBEDDED, FALSE);
	m_bAlwaysOpen     = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_ALWAYSOPEN, TRUE);
	m_bShowCsh        = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_CSHENABLED, TRUE);
	m_nCshScheme      = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_CSHSCHEME, 1);
	if (m_nCshScheme >= CSH_SCHEME_MAX) m_nCshScheme = 1;
	m_bSmartIdent    = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_SMARTIDENT, TRUE);
	m_bShowCshErrors = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_CSHERRORS, TRUE);
	m_bShowCshErrorsInWindow = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_CSHERRORSINWINDOW, FALSE);
	m_trackLineColor = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_TRACKLINERGBA, RGBA(128, 0, 0, 255));
	m_trackFillColor = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_TRACKFILLRGBA, RGBA(255, 0, 0, 128));
    m_bHints         = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_HINT, TRUE);
    m_bHintLineStart = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_HINT_LINESTART, TRUE);
    m_bHintEntity    = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_HINT_ENTITY, TRUE);
    m_bHintAttrName  = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_HINT_ATTRNAME, TRUE);
    m_bHintAttrValue = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_HINT_ATTRVALUE, TRUE);
    m_bHintFilter    = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_HINT_FILTER, TRUE);
    m_bHintCompact   = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_HINT_COMPACT, TRUE);
    m_bShowControls  = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_SHOW_CONTROLS, TRUE);

	m_bDoCshProcessing = m_bShowCsh || m_bHints;

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
		"#This is the default signalling chart\r\n"
		"#Edit and press Ctrl+W to see the result\r\n"
		"#You can change the default chart\r\n"
		"#in the Edit|Preferences... menu.\r\n"
		"\r\n"
		"a,b,c;\r\n"
		"b->c:trallala;\r\n"
		"a->b:new message;\r\n");

    //fills m_ChartSourcePreamble and m_SetOfDesigns
	if (1==ReadDesigns(reportProblem, "designlib.signalling")) //designlib.signalling not found in install dir
	    ReadDesigns(reportProblem, "original_designlib.signalling"); //use original_designlib.signalling 
	//FillDesignDesignCombo("", true);
	m_CopyrightText = "\\md(0)\\mu(2)\\mr(0)\\mn(10)\\f(arial)\\pr\\c(150,150,150)"
		              "http://msc-generator.sourceforge.net ";
	m_CopyrightText.Append(VersionText());

	//Complie csh colors from the variables defined in csh.{cpp,h} into CHARFORMAT elements for faster CSH
	MscInitializeCshAppearanceList();
	for (int scheme=0; scheme<CSH_SCHEME_MAX; scheme++)
		for (int i=0; i<COLOR_MAX; i++) 
			ConvertMscCshAppearanceToCHARFORMAT(MscCshAppearanceList[scheme][i], m_csh_cf[scheme][i]);

    m_cacheType = CChartCache::CACHE_BMP; //This works on all platforms
    OSVERSIONINFOEX osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    //majorversion of 5 is Win2000, XP and 2003, 6 is Vista, 2008 and Win7
    if(GetVersionEx((OSVERSIONINFO*)&osvi) && osvi.dwMajorVersion >= 6) 
        m_cacheType = CChartCache::CACHE_EMF;
}

//Read the designs from m_DesignDir, display a modal dialog if there is a problem.
//Filename can contain wildcards, all matching files are loaded
//Always updates m_ChartSourcePreamble and m_SetOfDesigns (at problem they may be empty or partial)
//returns 0 if OK
//returns 1 if no file found
//returns 2 if file found but there were errors in it (you can probably ignore them)

int CMscGenApp::ReadDesigns(bool reportProblem, const char *fileName)
{
	char buff[1024]; 
	GetModuleFileName(NULL, buff, 1024);
	std::string dir(buff);
	unsigned pos = dir.find_last_of('\\');
	ASSERT(pos!=std::string::npos);
	CString designlib_filename = dir.substr(0,pos).append("\\").append(fileName).c_str();

	//clear existing designs
    m_SetOfDesigns.Empty();
    m_ChartSourcePreamble = ";\n";

    CString preamble;
	CString msg;
	CFileFind finder;
	bool errors = false;
	BOOL bFound = finder.FindFile(designlib_filename);
    if (!bFound) return 1;
	while (bFound) {
		bFound = finder.FindNextFile();
		CDrawingChartData data;
		if (data.Load(finder.GetFilePath(), false)) {
			unsigned num = data.GetErrorNum(true);
			if (num) {
				msg.Append("Problems in design file: ");
				msg.Append(finder.GetFileName());
				msg.Append("\n");
				for (unsigned i=0; i<num; i++) {
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
	return errors?2:0;
}


//Version reminder dialog
class CVersionDlg : public CDialog
{
public:
	CVersionDlg() : CDialog(CVersionDlg::IDD), a(-1), b(-1), c(-1)
	{}

// Dialog Data
	enum { IDD = IDD_DIALOG_VERSION };
	CMFCLinkCtrl m_btnLink;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog( );
public:
	int a, b, c;
};

void CVersionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_URL2, m_btnLink);
}

BOOL CVersionDlg::OnInitDialog( ) 
{
	BOOL ret = CDialog::OnInitDialog();
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    const CString latest_url = pApp->GetProfileString(REG_SECTION_SETTINGS, REG_KEY_LAST_VERSION_SEEN_URL);
	m_btnLink.SetURL(latest_url);
	m_btnLink.SetTooltip(latest_url);
	m_btnLink.SizeToContent();
 	CString text = "Currently installed version: ";
	GetDlgItem(IDC_STATIC_CURRENT_VERSION)->SetWindowText(text + VersionText());
 	text = "Latest version available: ";
	GetDlgItem(IDC_STATIC_LATEST_VERSION)->SetWindowText(text + VersionText(a, b, c));
	return ret;
}



UINT CheckVersionFreshness(LPVOID)
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    CString latest_version_download_url;
    int latest_version_a=0, latest_version_b=0, latest_version_c=0;
	TRY {
		CInternetSession session("Msc-generator", 1, 0, NULL, NULL, INTERNET_FLAG_DONT_CACHE);
		CHttpConnection *httpconn = session.GetHttpConnection("msc-generator.sourceforge.net", 
			INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE, INTERNET_PORT(80));
		CHttpFile *file = httpconn->OpenRequest("GET", "version", NULL, 1, NULL, NULL, 
			INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE);
        wchar_t locale[LOCALE_NAME_MAX_LENGTH] = L"en-US";
        //Locale name only available in Vista!!
        //if (0<GetUserDefaultLocaleName(locale, LOCALE_NAME_MAX_LENGTH))
        //    file->AddRequestHeaders(CString("Accept-Language: ")+locale, HTTP_ADDREQ_FLAG_ADD_IF_NEW);
        OSVERSIONINFOEX osvi;
        ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
        //majorversion of 5 is Win2000, XP and 2003, 6 is Vista, 2008 and Win7
        if(GetVersionEx ((OSVERSIONINFO *) &osvi)) {
            char loc[LOCALE_NAME_MAX_LENGTH+2] = "; ";
            int i, j=2;
            for (i=0; i<LOCALE_NAME_MAX_LENGTH && locale[i]!=0; i++)
                if (locale[i]<128)
                    loc[j++] = (char)(locale[i]);
            loc[j]=0;
            if (i==2) loc[0]=0;
            char buff[200];
            //Mozilla/5.0 (Windows; U; Windows NT 6.0; en-US)
            sprintf(buff, "User-Agent: Msc-generator/%s (Windows; U; Windows NT %d.%d%s)", VersionText()+1, 
                osvi.dwMajorVersion, osvi.dwMinorVersion, loc);
            file->AddRequestHeaders(buff, HTTP_ADDREQ_FLAG_ADD_IF_NEW);
        }
		if (!file->SendRequest()) return false;
        CString latest_version;
		if (!file->ReadString(latest_version)) return false;
        if (latest_version.GetLength()==0) return false;
        if (0==sscanf(latest_version, "v%d.%d.%d", &latest_version_a, &latest_version_b, &latest_version_c)) return false;
        pApp->WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_LAST_VERSION_SEEN_MAJOR, latest_version_a);
        pApp->WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_LAST_VERSION_SEEN_MINOR, latest_version_b);
        pApp->WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_LAST_VERSION_SEEN_SUPER_MINOR, latest_version_c);
        if (file->ReadString(latest_version_download_url) && latest_version_download_url.GetLength()>0)
            pApp->WriteProfileString(REG_SECTION_SETTINGS, REG_KEY_LAST_VERSION_SEEN_URL, latest_version_download_url);
	} CATCH(CInternetException, pEx) {
        return false;
	}
	END_CATCH
        
	//If we are the latest version (or erroneous string read from web), exit
	if (latest_version_a<LIBMSCGEN_MAJOR) return false;
	if (latest_version_a==LIBMSCGEN_MAJOR && latest_version_b<LIBMSCGEN_MINOR) return false;
	if (latest_version_a==LIBMSCGEN_MAJOR && latest_version_b==LIBMSCGEN_MINOR && latest_version_c<=LIBMSCGEN_SUPERMINOR) return false;	

	//If user do not want to get reminded for the new version, exit
	int no_a = pApp->GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_NOREMIND_VERSION_MAJOR, -1);
	int no_b = pApp->GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_NOREMIND_VERSION_MINOR, -1);
	int no_c = pApp->GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_NOREMIND_VERSION_SUPER_MINOR, -1);
	if (no_a == latest_version_a && no_b == latest_version_b && no_c == latest_version_c) return false;
	
    CVersionDlg dlg;
    dlg.a = latest_version_a;
    dlg.b = latest_version_b;
    dlg.c = latest_version_c;

    if (dlg.DoModal() == IDOK) {
		pApp->WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_NOREMIND_VERSION_MAJOR, dlg.a);
		pApp->WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_NOREMIND_VERSION_MINOR, dlg.b);
		pApp->WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_NOREMIND_VERSION_SUPER_MINOR, dlg.c);
	}
	return true;
}


void CMscGenApp::OnButtonDefaultText()
{
    m_DefaultText = GetDoc()->m_itrEditing->GetText();
    WriteProfileString(REG_SECTION_SETTINGS, REG_KEY_DEFAULTTEXT, m_DefaultText);
}


void CMscGenApp::OnUpdateButtonDefaultText(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_DefaultText != GetDoc()->m_itrEditing->GetText());
}


void CMscGenApp::OnCheckPedantic()
{
    m_Pedantic = !m_Pedantic;
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_PEDANTIC, m_Pedantic);
    //recompile if it was a recently compiled
    CMscGenDoc *pDoc = GetDoc();
    if (pDoc->m_itrShown == pDoc->m_itrEditing)
        pDoc->ShowEditingChart(false);
}


void CMscGenApp::OnUpdateCheckPedantic(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_Pedantic);
}


void CMscGenApp::OnCheckWarnings()
{
    m_Warnings = !m_Warnings;
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_WARNINGS, m_Warnings);
    //recompile if it was a recently compiled
    CMscGenDoc *pDoc = GetDoc();
    if (pDoc->m_itrShown == pDoc->m_itrEditing)
        pDoc->ShowEditingChart(false);
}


void CMscGenApp::OnUpdateCheckWarnings(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_Warnings);
}


void CMscGenApp::OnCheckPbEditing()
{
    m_bPB_Editing = !m_bPB_Editing;
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_PB_EDITING, m_bPB_Editing);
    //recompile if it was a recently compiled and is showing all pages
    CMscGenDoc *pDoc = GetDoc();
    if (pDoc->m_itrShown == pDoc->m_itrEditing)
        pDoc->ShowEditingChart(false);
}


void CMscGenApp::OnUpdateCheckPbEditing(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_bPB_Editing);
}


void CMscGenApp::OnCheckPbEmbedded()
{
    m_bPB_Embedded = !m_bPB_Embedded;
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_PB_EMBEDDED, m_bPB_Embedded);
    //recompile if it is embdedded and was recently compiled and is showing all pages
    CMscGenDoc *pDoc = GetDoc();
    if (pDoc->IsEmbedded() && pDoc->m_itrShown == pDoc->m_itrEditing && pDoc->m_itrEditing->GetPage()>0)
        pDoc->ShowEditingChart(false);
}


void CMscGenApp::OnUpdateCheckPbEmbedded(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_bPB_Embedded);
}


void CMscGenApp::OnCheckCsh()
{
    m_bShowCsh = !m_bShowCsh;
    m_bDoCshProcessing = m_bShowCsh || m_bHints;
	WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_CSHENABLED, m_bShowCsh);
    if (IsInternalEditorRunning())
        m_pWndEditor->m_ctrlEditor.UpdateCsh(true);
}


void CMscGenApp::OnUpdateCheckCsh(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_bShowCsh);
    pCmdUI->Enable(IsInternalEditorRunning());
}

void CMscGenApp::OnComboCsh()
{
    CMainFrame *pMainWnd = dynamic_cast<CMainFrame*>(GetMainWnd());
    if (!pMainWnd) return;
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;
    pMainWnd->m_wndRibbonBar.GetElementsByID(IDC_COMBO_CSH, arButtons);
    _ASSERT(arButtons.GetSize()==1);
	CMFCRibbonComboBox *c = dynamic_cast<CMFCRibbonComboBox*>(arButtons[0]);
    m_nCshScheme = c->GetCurSel();
	WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_CSHSCHEME, m_nCshScheme);
    if (IsInternalEditorRunning())
        m_pWndEditor->m_ctrlEditor.UpdateCsh(true);
}



void CMscGenApp::OnUpdateComboCsh(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_bShowCsh && IsInternalEditorRunning());
}


void CMscGenApp::OnCheckSmartIdent()
{
    m_bSmartIdent = !m_bSmartIdent;
	WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_SMARTIDENT, m_bSmartIdent);
    if (IsInternalEditorRunning())
        m_pWndEditor->m_ctrlEditor.UpdateCsh(true);
}


void CMscGenApp::OnUpdateCheckSmartIdent(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_bShowCsh && IsInternalEditorRunning());
    pCmdUI->SetCheck(m_bSmartIdent);
}

void CMscGenApp::OnCheckCshError()
{
    m_bShowCshErrors = !m_bShowCshErrors;
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_CSHERRORS, m_bShowCshErrors);
    if (IsInternalEditorRunning())
        m_pWndEditor->m_ctrlEditor.UpdateCsh(true);
}



void CMscGenApp::OnUpdateCheckCshError(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_bShowCsh && IsInternalEditorRunning());
    pCmdUI->SetCheck(m_bShowCshErrors);
}


void CMscGenApp::OnCheckCshErrorInWindow()
{
    m_bShowCshErrorsInWindow = !m_bShowCshErrorsInWindow;
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_CSHERRORSINWINDOW, m_bShowCshErrorsInWindow); 
    if (IsInternalEditorRunning())
        m_pWndEditor->m_ctrlEditor.UpdateCsh(true);
}


void CMscGenApp::OnUpdateCheckCshErrorInWindow(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_bShowCsh && IsInternalEditorRunning());
    pCmdUI->SetCheck(m_bShowCshErrorsInWindow);
}

void CMscGenApp::OnEditPreferences()
{
	CMscGenDoc *pDoc = GetDoc();

	COptionDlg optionDlg;
	optionDlg.m_TextEditStartCommand = m_sStartTextEditor;
	optionDlg.m_TextEditorJumpToLineCommand = m_sJumpToLine;

    bool bRestartEditor = false;
    if (optionDlg.DoModal() == IDOK) {
        if (m_sStartTextEditor != optionDlg.m_TextEditStartCommand) {
			m_sStartTextEditor = optionDlg.m_TextEditStartCommand;
			WriteProfileString(REG_SECTION_SETTINGS, REG_KEY_STARTTEXTEDITOR, m_sStartTextEditor);
			if (pDoc && pDoc->m_ExternalEditor.IsRunning()) 
                pDoc->m_ExternalEditor.Restart(STOPEDITOR_WAIT);				
		}
		if (m_sJumpToLine != optionDlg.m_TextEditorJumpToLineCommand) {
			m_sJumpToLine = optionDlg.m_TextEditorJumpToLineCommand;
			WriteProfileString(REG_SECTION_SETTINGS, REG_KEY_JUMPTOLINE, m_sJumpToLine);
		}
	}
}

void CMscGenApp::OnUpdateEditPreferences(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_iTextEditorType==OTHER);
}

//C:\Windows\Notepad.exe %n;"C:\Program Files\Notepad++\notepad++.exe" -multiInst -nosession %n;
//;-n%l -nosession %n;


void CMscGenApp::OnCheckEeOther()
{
    if (m_iTextEditorType == OTHER) return;
    m_iTextEditorType = OTHER;
    m_sStartTextEditor = GetProfileString(REG_SECTION_SETTINGS, REG_KEY_STARTTEXTEDITOR, "Notepad.exe %n");
    m_sJumpToLine = GetProfileString(REG_SECTION_SETTINGS, REG_KEY_JUMPTOLINE, "");
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_TEXTEDITORTYPE, m_iTextEditorType);
	CMscGenDoc *pDoc = GetDoc();
    if (pDoc && pDoc->m_ExternalEditor.IsRunning()) 
        pDoc->m_ExternalEditor.Restart(STOPEDITOR_WAIT);				
}


void CMscGenApp::OnCheckEeNotepad()
{
    if (m_iTextEditorType == NOTEPAD) return;
    m_iTextEditorType = NOTEPAD;
    m_sStartTextEditor = "C:\\Windows\\Notepad.exe %n";
    m_sJumpToLine = "";
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_TEXTEDITORTYPE, m_iTextEditorType);
	CMscGenDoc *pDoc = GetDoc();
    if (pDoc && pDoc->m_ExternalEditor.IsRunning()) 
        pDoc->m_ExternalEditor.Restart(STOPEDITOR_WAIT);				
}


void CMscGenApp::OnCheckEeNotepadpp()
{
    if (m_iTextEditorType == NPP) return;
    m_iTextEditorType = NPP;
    m_sStartTextEditor = "\"" +m_NppPath+ "\" -multiInst -nosession %n";
    m_sJumpToLine = "\"" +m_NppPath+ "\" -n%l -nosession %n";
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_TEXTEDITORTYPE, m_iTextEditorType);
	CMscGenDoc *pDoc = GetDoc();
    if (pDoc && pDoc->m_ExternalEditor.IsRunning()) 
        pDoc->m_ExternalEditor.Restart(STOPEDITOR_WAIT);				
}


void CMscGenApp::OnUpdateCheckEeOther(CCmdUI *pCmdUI)
{
    pCmdUI->Enable();
    pCmdUI->SetCheck(m_iTextEditorType==OTHER);
}


void CMscGenApp::OnUpdateCheckEeNotepad(CCmdUI *pCmdUI)
{
    pCmdUI->Enable();
    pCmdUI->SetCheck(m_iTextEditorType==NOTEPAD);
}


void CMscGenApp::OnUpdateCheckEeNotepadpp(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_NppPath.GetLength()>0);
    pCmdUI->SetCheck(m_iTextEditorType==NPP);
}


void CMscGenApp::OnCheckHints()
{
    m_bHints = !m_bHints;
    m_bDoCshProcessing = m_bShowCsh || m_bHints;
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_HINT, m_bHints);
}

void CMscGenApp::OnUpdateCheckHints(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_bHints);
}

void CMscGenApp::OnCheckSmartHintCompact()
{
    m_bHintCompact = !m_bHintCompact;
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_SHOW_CONTROLS, m_bHintCompact);
}


void CMscGenApp::OnCheckSmartHintFilter()
{
    m_bHintFilter = !m_bHintFilter;
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_HINT_FILTER, m_bHintFilter);
}


void CMscGenApp::OnCheckSmartHintLineStart()
{
    m_bHintLineStart = !m_bHintLineStart;
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_HINT_LINESTART, m_bHintLineStart);
}


void CMscGenApp::OnCheckSmartHintEntity()
{
    m_bHintEntity = !m_bHintEntity;
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_HINT_ENTITY, m_bHintEntity);
}


void CMscGenApp::OnCheckSmartHintAttrName()
{
    m_bHintAttrName = !m_bHintAttrName;
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_HINT_ATTRNAME, m_bHintAttrName);
}


void CMscGenApp::OnCheckSmartHintAttrValue()
{
    m_bHintAttrValue = !m_bHintAttrValue;
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_HINT_ATTRVALUE, m_bHintAttrValue);
}


void CMscGenApp::OnUpdateCheckSmartHintBoxes(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_bHints);
    bool set = false;
    switch (pCmdUI->m_nID) {
    case IDC_CHECK_SMART_HINT_FILTER: set = m_bHintFilter; break;
    case IDC_CHECK_SMART_HINT_COMPACT: set = m_bHintCompact; break;
    case IDC_CHECK_SMART_HINT_LINE_START: set = m_bHintLineStart; break;
    case IDC_CHECK_SMART_HINT_ENTITY: set = m_bHintEntity; break;
    case IDC_CHECK_SMART_HINT_ATTR_NAME: set = m_bHintAttrName; break;
    case IDC_CHECK_SMART_HINT_ATTR_VALUE: set = m_bHintAttrValue; break;
    default: _ASSERT(0);
    }
    pCmdUI->SetCheck(set);
}




void CMscGenApp::OnButtonTrackColor()
{
    CMainFrame *pMainWnd = dynamic_cast<CMainFrame*>(GetMainWnd());
    if (!pMainWnd) return;
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;
    pMainWnd->m_wndRibbonBar.GetElementsByID(ID_BUTTON_TRACK_COLOR, arButtons);
    _ASSERT(arButtons.GetSize()==1);
    CMFCRibbonColorButton *cb = dynamic_cast<CMFCRibbonColorButton*>(arButtons[0]);
    COLORREF c = cb->GetColor();
    const unsigned char r = GetRValue(c); 
    const unsigned char g = GetGValue(c); 
    const unsigned char b = GetBValue(c); 
    m_trackFillColor = RGBA(r, g, b, 128); //50% transparent
    m_trackLineColor = RGBA(r/2, g/2, b/2, 255); //50% darker, fully opaque
}
