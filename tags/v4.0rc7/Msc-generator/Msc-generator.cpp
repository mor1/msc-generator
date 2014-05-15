/*
    This file is part of Msc-generator.
	Copyright 2008,2009,2010,2011,2012,2013,2014 Zoltan Turanyi
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


/** @defgroup Msc-generator The Windows GUI of Msc-generator.
This part of the code compiles only on Windows and implements the GUI.
It is an OLE server object, providing an "Msc-generator siganlling chart" object
to other applications.
It is written using the Microsoft Foundation Classes (MFC) library using the 
Document/View architecture. */

/** @defgroup Msc_generator_files The files comprising of the Msc-generator GUI.*/

/** @file Msc-generator.cpp The implementation of CAboutDialog and MscGenApp classes.
 * @ingroup Msc_generator_files */

// Msc-generator.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "Msc-generator.h"
#include "MainFrm.h"

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
    text = "(using cairo v";
	GetDlgItem(IDC_STATIC_CAIRO_VERSION)->SetWindowText(text + cairo_version_string() + ")");

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
    ON_COMMAND(ID_HELP, &CMscGenApp::OnHelp)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CMscGenApp::OnFilePrintSetup)
    ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_SETUP, &CMscGenApp::OnUpdatePrintSetup)
	ON_COMMAND(ID_EDIT_PREFERENCES, OnEditPreferences)
    ON_COMMAND(ID_BUTTON_DEFAULT_TEXT, &CMscGenApp::OnButtonDefaultText)
    ON_UPDATE_COMMAND_UI(ID_BUTTON_DEFAULT_TEXT, &CMscGenApp::OnUpdateButtonDefaultText)
    ON_COMMAND(IDC_CHECK_PEDANTIC, &CMscGenApp::OnCheckPedantic)
    ON_UPDATE_COMMAND_UI(IDC_CHECK_PEDANTIC, &CMscGenApp::OnUpdateCheckPedantic)
    ON_COMMAND(IDC_CHECK_WARNINGS, &CMscGenApp::OnCheckWarnings)
    ON_UPDATE_COMMAND_UI(IDC_CHECK_WARNINGS, &CMscGenApp::OnUpdateCheckWarnings)
    ON_COMMAND(IDC_CHECK_PAGEBREAKS, &CMscGenApp::OnCheckPageBreaks)
    ON_UPDATE_COMMAND_UI(IDC_CHECK_PAGEBREAKS, &CMscGenApp::OnUpdateCheckPageBreaks)
    ON_COMMAND(IDC_CHECK_CSH, &CMscGenApp::OnCheckCsh)
    ON_COMMAND(IDC_COMBO_CSH, &CMscGenApp::OnComboCsh)
    ON_UPDATE_COMMAND_UI(IDC_CHECK_CSH, &CMscGenApp::OnUpdateCheckCsh)
    ON_UPDATE_COMMAND_UI(IDC_COMBO_CSH, &CMscGenApp::OnUpdateComboCsh)
    ON_COMMAND(IDC_CHECK_SMART_IDENT, &CMscGenApp::OnCheckSmartIdent)
    ON_COMMAND(ID_CHECK_CSH_TAB, &CMscGenApp::OnCheckTABIdents)
    ON_COMMAND(IDC_CHECK_CSH_ERROR, &CMscGenApp::OnCheckCshError)
    ON_UPDATE_COMMAND_UI(IDC_CHECK_SMART_IDENT, &CMscGenApp::OnUpdateCheckSmartIdent)
    ON_UPDATE_COMMAND_UI(ID_CHECK_CSH_TAB, &CMscGenApp::OnUpdateCheckTABIdents)
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
    ON_COMMAND(ID_EMBEDDEDOPTIONS_FALLBACK_RES, &CMscGenApp::OnEmbeddedoptionsFallbackRes)
    ON_COMMAND(ID_AUTO_PAGINATE, &CMscGenApp::OnAutoPaginate)
    ON_UPDATE_COMMAND_UI(ID_AUTO_PAGINATE, &CMscGenApp::OnUpdateAutoPaginate)
    ON_COMMAND(ID_AUTO_HEADERS, &CMscGenApp::OnAutoHeaders)
    ON_UPDATE_COMMAND_UI(ID_AUTO_HEADERS, &CMscGenApp::OnUpdateAutoHeaders)
    ON_COMMAND(ID_COMBO_SCALE, &CMscGenApp::OnComboScale)
    ON_COMMAND(ID_BUTTON_PAGES, OnButtonPages)
    ON_COMMAND(ID_COMBO_ALIGNMENT, &CMscGenApp::OnComboAlignment)
    ON_COMMAND(ID_EDIT_MARGIN_L, &CMscGenApp::OnEditMarginL)
    ON_COMMAND(ID_EDIT_MARGIN_R, &CMscGenApp::OnEditMarginR)
    ON_COMMAND(ID_EDIT_MARGIN_T, &CMscGenApp::OnEditMarginT)
    ON_COMMAND(ID_EDIT_MARGIN_B, &CMscGenApp::OnEditMarginB)
    ON_UPDATE_COMMAND_UI(ID_COMBO_SCALE, &CMscGenApp::OnUpdatePrintPreviewEdits)
    ON_UPDATE_COMMAND_UI(ID_EDIT_MARGIN_L, &CMscGenApp::OnUpdatePrintPreviewEdits)
    ON_UPDATE_COMMAND_UI(ID_EDIT_MARGIN_R, &CMscGenApp::OnUpdatePrintPreviewEdits)
    ON_UPDATE_COMMAND_UI(ID_EDIT_MARGIN_T, &CMscGenApp::OnUpdatePrintPreviewEdits)
    ON_UPDATE_COMMAND_UI(ID_EDIT_MARGIN_B, &CMscGenApp::OnUpdatePrintPreviewEdits)
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


/** Default constructor.
 * As per MFC custioms, this is not doing real initialization, just
 * sets the main fields to meaningful defaults.*/
CMscGenApp::CMscGenApp() : m_designlib_csh(Context(true), &m_Shapes)
{
	// replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
    char buff[200];
    sprintf(buff, "Msc-generator-%s", VersionText());
	SetAppID(buff);

	// Place all significant initialization in InitInstance
	m_pWndOutputView = 0;
	m_pWndEditor = 0;
	m_bFullScreenViewMode = false;
    m_bShowControls = false;
    m_bAutoPaginate = false;
    m_bAutoHeading = true;
    m_iScale4Pagination = -1; //Percentage, -1=Fit Witdth, -2=Fit Page
    m_iPageAlignment = -4;
    m_PrinterScale = XY(1,1);
    m_PhyPrinterPageSize = PageSizeInfo::GetPhysicalPageSize(PageSizeInfo::A4P);
    m_pageSize = PageSizeInfo::A4P;
    for (unsigned u=0; u<4; u++)
        m_printer_phy_margins[u] = 0;
    for (unsigned u = 0; u<4; u++)
        m_printer_usr_margins[u] = 36;  //uniform half inch margin
}


/** The one and only CMscGenApp object */
CMscGenApp theApp;
/** This identifier was generated to be statistically unique for your app
 * You may change it if you prefer to choose a specific identifier
 * {453AC3C8-F260-4D88-832A-EC957E92FDEC} */
static const CLSID clsid =
{ 0x453AC3C8, 0xF260, 0x4D88, { 0x83, 0x2A, 0xEC, 0x95, 0x7E, 0x92, 0xFD, 0xEC } };


UINT CheckVersionFreshness(LPVOID);

/** CMscGenApp initialization 
 * This is doing all the important initialization.
 * - Init the OLE libs, Taskbar, RichEdit, context menu mgr, tooltips, keyboard
 * - Register our OLE object class and document template
 * - Parse command line, act on it
 * - Obtain current printer data (margins, paper size)
 * - Show our window, enable drag and drop and focus the internal editor.
 */
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

    //Prevent some MFC bug
    //See http://www.mombu.com/microsoft/mmc/t-activation-context-error-in-mmc-sxs-when-using-windows-vista-610122.html
    //and http://www.prof-uis.com/prof-uis/tech-support/support-forum/activateactctx-and-dactivateactctx-65396.aspx
    //afxAmbientActCtx = FALSE; 


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
	pDocTemplate->SetServerInfo(IDR_SRVR_EMBEDDED); //No other parameters, we do not support in-place editing
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

    //Get Printer info (may be needed to print from command line)
    UpdatePrinterData();

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

/** Terminate us.
 * Just add AfxOleTerm() to the default method*/
int CMscGenApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}


/** App command to run the about dialog */
void CMscGenApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/** App command to run the help */
void CMscGenApp::OnHelp()
{
    char buff[2048];
    if (GetModuleFileName(NULL, buff, 2048)) {
        PathRemoveFileSpec(buff);
        CString help = buff;
        help += "\\msc-gen.chm";
        ShellExecute(NULL, NULL, help, NULL, NULL, SW_SHOWNORMAL);
    }
}

/** CMscGenApp customization load/save methods*/
void CMscGenApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

/** Do nothing*/
void CMscGenApp::LoadCustomState()
{
}

/** Do nothing*/
void CMscGenApp::SaveCustomState()
{
}

/** Convert a libmscgen color sytnax flag (defined in csh.h) to
 * that of Windows' CHARFORMAT*/
DWORD ConvertMscColorSyntaxFlagsToCHARFORMATFlags(int input)
{
	return ((input & COLOR_FLAG_BOLD     ) ? CFM_BOLD      : 0) |
		   ((input & COLOR_FLAG_ITALICS  ) ? CFM_ITALIC    : 0) |
		   ((input & COLOR_FLAG_UNDERLINE) ? CFM_UNDERLINE : 0) |
		   ((input & COLOR_FLAG_COLOR    ) ? CFM_COLOR     : 0);
}

/** Convert a libmscgen color sytnax appearance (defined in csh.h) to
 * that of Windows' CHARFORMAT*/
void ConvertMscCshAppearanceToCHARFORMAT(const ColorSyntaxAppearance &app, CHARFORMAT &cf)
{
	cf.cbSize      = sizeof(cf);
	cf.dwMask      = ConvertMscColorSyntaxFlagsToCHARFORMATFlags(app.mask);
	cf.dwEffects   = ConvertMscColorSyntaxFlagsToCHARFORMATFlags(app.effects);
	cf.crTextColor = RGB(app.r, app.g, app.b);
}

/** Reads page geometry for the default printer and stores it in MscGenApp.*/
void CMscGenApp::UpdatePrinterData()
{
    CDC dc;
	UpdatePrinterSelection(m_hDevNames == NULL); //force default if no current
    if (CreatePrinterDC(dc)) {
        //Get printer resolution (printer pixels per inch = DPI)
        const XY res(dc.GetDeviceCaps(LOGPIXELSX), dc.GetDeviceCaps(LOGPIXELSY));
        //convert to point/printer pixel scaling (1 point = 1/72 inch = msc-generator pixel)
        const XY sc = res/72;
        //Get page size in points
        const XY phy = XY(dc.GetDeviceCaps(PHYSICALWIDTH)/sc.x, dc.GetDeviceCaps(PHYSICALHEIGHT)/sc.y);
        //Get offset of printable area in point
        const XY off(dc.GetDeviceCaps(PHYSICALOFFSETX)/sc.x, dc.GetDeviceCaps(PHYSICALOFFSETY)/sc.y);
        //Get printable area in point
        const XY printable(dc.GetDeviceCaps(HORZRES)/sc.x, dc.GetDeviceCaps(VERTRES)/sc.y);

        m_PrinterScale = sc;
        m_PhyPrinterPageSize = phy;
        m_pageSize = PageSizeInfo::NO_PAGE;
        for (unsigned u = unsigned(PageSizeInfo::NO_PAGE)+1; u < unsigned(PageSizeInfo::MAX_PAGE); u++)
            if ((PageSizeInfo::GetPhysicalPageSize(PageSizeInfo::EPageSize(u)) - phy).length_sqr()<10) {
                m_pageSize = PageSizeInfo::EPageSize(u); 
                break;
            }
    
            m_printer_phy_margins[0] = off.x;
            m_printer_phy_margins[1] = phy.x -printable.x - off.x; 
            m_printer_phy_margins[2] = off.y;
            m_printer_phy_margins[3] = phy.y -printable.y - off.y; 
            _ASSERT( m_printer_phy_margins[1]>0);
            _ASSERT( m_printer_phy_margins[3]>0);
    } 
    NormalizeUserMargins();
}

/** Adjust the margins specified by the user such that
 * there is always at least 72 points of printable space among them
 * (if possible).
 * @returns FALSE, if paper is too small for this.*/
bool CMscGenApp::NormalizeUserMargins()
{
    //Normalize user margins, have at least 72 points of printable space
    const XY ps = GetPrintablePaperSize();
    if (ps.x<0) {
        m_printer_usr_margins[0] += ps.x/2;
        m_printer_usr_margins[1] += ps.x/2;
    }
    if (ps.y<0) {
        m_printer_usr_margins[2] += ps.y/2;
        m_printer_usr_margins[3] += ps.y/2;
    }
    return ps.x<0 || ps.y<0;
}

void CMscGenApp::DisplayErrors(const MscError &error, bool warnings, const char *type)
{
    if (error.GetErrorNum(warnings)==0)
        return;
    string msg = "There were errors in the ";
    msg.append(type);
    msg.append(" file.\n");
    for (unsigned u = 0; u<std::min(10U, error.GetErrorNum(warnings)); u++)
        msg.append(error.GetErrorText(u, warnings)).append("\n");
    MessageBox(0, msg.c_str(), "Msc-generator errors", MB_OK);
}


/** Read our stored settings from the registry and reads the design library.
 * @param [in] reportProblem If true, we report any problems.*/
void CMscGenApp::ReadRegistryValues(bool /*reportProblem*/) 
{
	//Load Registry values
	m_Pedantic       = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_PEDANTIC, FALSE);
	m_Warnings       = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_WARNINGS, TRUE);
	m_bPageBreaks    = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_PB_EDITING, FALSE);
    m_uFallbackResolution = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_FALLBACK_RESOLUTION, 300);
	m_bShowCsh       = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_CSHENABLED, TRUE);
	m_nCshScheme     = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_CSHSCHEME, 1);
	if (m_nCshScheme >= CSH_SCHEME_MAX) m_nCshScheme = 1;
	m_bSmartIdent    = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_SMARTIDENT, TRUE);
    m_bTABIdents = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_TABIDENTS, TRUE);;
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
    m_bAutoPaginate  = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_AUTO_PAGINATE, FALSE);
    m_bAutoHeading   = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_AUTO_HEADING, TRUE);
    m_iScale4Pagination = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_SCALE4PAGINATION, -1);
    m_iPageAlignment = GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_PAGE_ALIGNMENT, -4);
    CString val;
    val = GetProfileString(REG_SECTION_SETTINGS, REG_KEY_PAGE_MARGIN_L, "28.3464567"); //so many points per cm
    sscanf(val, "%lf", m_printer_usr_margins+0);
    val = GetProfileString(REG_SECTION_SETTINGS, REG_KEY_PAGE_MARGIN_R, "28.3464567");
    sscanf(val, "%lf", m_printer_usr_margins+1);
    val = GetProfileString(REG_SECTION_SETTINGS, REG_KEY_PAGE_MARGIN_T, "28.3464567");
    sscanf(val, "%lf", m_printer_usr_margins+2);
    val = GetProfileString(REG_SECTION_SETTINGS, REG_KEY_PAGE_MARGIN_B, "28.3464567");
    sscanf(val, "%lf", m_printer_usr_margins+3);

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
		"#with the leftmost button on the Preferences pane of the ribbon.\r\n"
		"\r\n"
		"a,b,c;\r\n"
		"b->c:trallala;\r\n"
		"a->b:new message;\r\n");

    //fills m_ChartSourcePreamble and m_SetOfDesigns
	if (1==ReadDesigns(1, "designlib.signalling")) //designlib.signalling not found in install dir
	    ReadDesigns(1, "original_designlib.signalling"); //use original_designlib.signalling 
    ReadDesigns(2, "*.signalling"); //load designs from appdata
    DisplayErrors(m_DesignErrors, true, "design library");

    //ReadShapes(1, "default.shape");
    //ReadShapes(2, "*.shape");
    //DisplayErrors(m_ShapeErrors, true, "shape library");

    m_CopyrightText = "\\md(0)\\mu(2)\\mr(0)\\mn(10)\\f(arial)\\pr\\c(150,150,150)"
		              "http://msc-generator.sourceforge.net ";
	m_CopyrightText.Append(VersionText());

	//Complie csh colors from the variables defined in csh.{cpp,h} into CHARFORMAT elements for faster CSH
	MscInitializeCshAppearanceList();
	for (int scheme=0; scheme<CSH_SCHEME_MAX; scheme++)
		for (int i=0; i<COLOR_MAX; i++) 
			ConvertMscCshAppearanceToCHARFORMAT(MscCshAppearanceList[scheme][i], m_csh_cf[scheme][i]);

}

/** Get the path to a folder.
 * 1==the folder of the executable
 * 2==appdata/mscgenerator fodler 
 * other values return the empty string*/
std::string GetFolder(unsigned folder)
{
    char buff[MAX_PATH];
    std::string dir;
    if (folder == 1) {
        GetModuleFileName(NULL, buff, MAX_PATH);
        dir = buff;
        unsigned pos = dir.find_last_of('\\');
        ASSERT(pos!=std::string::npos);
        dir.erase(pos+1);
    } else if (folder==2) {
        if (!SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, buff)))
            return dir;
        dir = buff;
        dir.append("\\Msc-generator\\");
    }
    return dir;
}

/** Read the shapes from the design library.
* Errors are collected into m_ShapeErrors
* @param [in] folder Which folder to look in (0: current, 1:executable's, 
*                    2:appdata/roaming/Msc-generator
* @param [in] fileName The name of the file to read from.
*                      Searched in the folder specified.
*                      Can contain wildcards, all matching files are loaded.
* @returns 0 if OK; 1 if no file found; 2 if file found but there were errors in it (you can probably ignore them)*/
int CMscGenApp::ReadShapes(unsigned folder, const char *fileName)
{
    CString shape_filename = GetFolder(folder).append(fileName).c_str();

    unsigned old_err = m_ShapeErrors.GetErrorNum(false);
    CFileFind finder;
    bool bFound = finder.FindFile(shape_filename);
    if (!bFound)
        return 1;
    while (bFound) {
        bFound = finder.FindNextFile();
        FILE *in = fopen(finder.GetFilePath(), "r");
        if (in) {
            char *buffer = ReadFile(in);
            if (buffer) {
                m_ShapeErrors.AddFile(string(finder.GetFileName()));
                m_Shapes.Add(buffer, m_ShapeErrors);
                free(buffer);
            }
            fclose(in);
        }
    }
    return old_err == m_ShapeErrors.GetErrorNum(false) ?  0 : 2;
}

/** Read the designs from the design library.
* Errors are collected into m_DesignErrors
* Always updates m_ChartSourcePreamble and m_SetOfDesigns (at problem they
 * may be empty or partial).
 * @param [in] folder Which folder to look for. 0=none - filename contains folder,
 *                    1=the folder of the executable, 2=the appdata folder of the user
 * @param [in] fileName The name of the file to read from. 
 *                      Searched in the executable's directory.
 *                      Can contain wildcards, all matching files are loaded.
 * @returns 0 if OK; 1 if no file found; 2 if file found but there were errors in it (you can probably ignore them)*/
int CMscGenApp::ReadDesigns(unsigned folder, const char *fileName)
{
	CString designlib_filename = GetFolder(folder).append(fileName).c_str();

	CString msg;
	CFileFind finder;
	bool errors = false;
	BOOL bFound = finder.FindFile(designlib_filename);
    if (!bFound) {
		CDrawingChartData data;
        data.CompileIfNeeded();
        m_Designs.insert(data.GetDesigns().begin(), data.GetDesigns().end()); //insert 'plain' design
        m_designlib_csh.ParseText("", 0, -1, 1);
        return 1;
    }

    CString text;
    Msc msc;
    msc.Progress.StartSection(MscProgress::PARSE);
    msc.Shapes.swap(m_Shapes);
	while (bFound) {
		bFound = finder.FindNextFile();
		CDrawingChartData data;
		if (data.Load(finder.GetFilePath(), false)) {
            msc.ParseText(data.GetText(), finder.GetFilePath());
            text.Append(data.GetText());
		}
	}
    //move the designs, shapes & errors from data to m_Designs/m_DesignErrors
    for (auto d : msc.Designs) {
        const auto i = m_Designs.find(d.first);
        if (i==m_Designs.end())
            m_Designs.emplace(d.first, std::move(d.second));
        else
            i->second += d.second;
    }
    msc.Shapes.swap(m_Shapes);
    m_DesignErrors += msc.Error;
    //create pre-parsed data for csh
    m_designlib_csh.ParseText(text, text.GetLength(), -1, 1);
    //Now clean it up. Keep only FullDesigns, PartialDesigns, 
    //the outermost context and forbidden styles
    //1.clear hinted errors, actual csh entries, hints
    m_designlib_csh.CshList.clear();
    m_designlib_csh.CshErrors.clear();
    m_designlib_csh.Hints.clear();
    //2. clear entity names, marker names, all but the top of the the context stack
    _ASSERT(m_designlib_csh.EntityNames.size()==0);
    _ASSERT(m_designlib_csh.MarkerNames.size()==0);
    _ASSERT(m_designlib_csh.Contexts.size()==1);
    m_designlib_csh.EntityNames.clear();
    m_designlib_csh.MarkerNames.clear();
    m_designlib_csh.Contexts.erase(++m_designlib_csh.Contexts.begin(), m_designlib_csh.Contexts.end());
    //3. clear shape names, too: these will be added via pShapes (in parsed form)
    m_designlib_csh.shape_names.clear();
    //4. clear all miscellaneous info that just takes up memory
    m_designlib_csh.hintAttrName.clear();
    m_designlib_csh.ForcedDesign.clear(); //will be set in CMscGenDoc::ChangeDesign()

    return errors ? 2 : 0;
}


/** Version reminder dialog */
class CVersionDlg : public CDialog
{
public:
	CVersionDlg() : CDialog(CVersionDlg::IDD), a(-1), b(-1), c(-1)
	{}

// Dialog Data
	enum { IDD = IDD_DIALOG_VERSION };
	CMFCLinkCtrl m_btnLink; ///<The link of the msc-generator home page

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    ///< DDX/DDV support
	virtual BOOL OnInitDialog( );                       ///< Main initialization
public:
	int a; ///<Major version
    int b; ///<Minor version
    int c; ///<Superminor (or micro) version
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


/** The main function of a background thread that checks
 * if our version is current. If yes, it silently returns.
 * If no, and the user has not blocked reminders for the
 * latest version available, we display a reminder message box.
 * We handle user wishes on that box. (Ignore this version, remind later)*/
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
            sprintf(buff, "User-Agent: Msc-generator/%s (Windows; U; Windows NT %u.%u%s)", VersionText()+1, 
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

/** Store the current chart text as default chart text in the registry.
 * Updates the registry.*/
void CMscGenApp::OnButtonDefaultText()
{
    m_DefaultText = GetDoc()->m_itrEditing->GetText();
    WriteProfileString(REG_SECTION_SETTINGS, REG_KEY_DEFAULTTEXT, m_DefaultText);
}

/** Enable/Disable the button if the curren text is different/same as the default.*/
void CMscGenApp::OnUpdateButtonDefaultText(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_DefaultText != GetDoc()->m_itrEditing->GetText());
}

/** Toggles the pedantic compilation flag.
 * Updates the registry.
 * Recompile if the current compiled version is up-tp-date.*/
void CMscGenApp::OnCheckPedantic()
{
    m_Pedantic = !m_Pedantic;
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_PEDANTIC, m_Pedantic);
    //recompile if it was a recently compiled
    CMscGenDoc *pDoc = GetDoc();
    if (pDoc)
        pDoc->CompileEditingChart(false, false, true);
}

/** Copy the m_Pedantic flag to the button*/
void CMscGenApp::OnUpdateCheckPedantic(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_Pedantic);
}

/** Toggles the "include warnings" flag. 
 * Updates the registry.
* Recompile if the current compiled version is up-tp-date.*/
void CMscGenApp::OnCheckWarnings()
{
    m_Warnings = !m_Warnings;
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_WARNINGS, m_Warnings);
    //recompile if it was a recently compiled
    CMscGenDoc *pDoc = GetDoc();
    if (pDoc)
        pDoc->CompileEditingChart(false, false, true);
}


/** Copy the m_Warnings flag to the button*/
void CMscGenApp::OnUpdateCheckWarnings(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_Warnings);
}


/** Toggles the "include page breaks" flag. 
 * Updates the registry.
* Recompile if the current compiled version is up-to-date and has
* multiple pages.*/
void CMscGenApp::OnCheckPageBreaks()
{
    m_bPageBreaks = !m_bPageBreaks;
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_PB_EDITING, m_bPageBreaks);
    //recompile
    CMscGenDoc *pDoc = GetDoc();
    if (pDoc && pDoc->m_ChartShown.GetPages()>1)
        pDoc->CompileEditingChart(false, false, true);
}

/** Enable the button always*/
void CMscGenApp::OnUpdateCheckPageBreaks(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_bPageBreaks);
}

/** Toggles the Color Sytnax Highlighting flag.
 * Updates the registry.
 * Updates CSH in the internal editor if any.*/
void CMscGenApp::OnCheckCsh()
{
    m_bShowCsh = !m_bShowCsh;
	WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_CSHENABLED, m_bShowCsh);
    if (IsInternalEditorRunning()) 
        m_pWndEditor->m_ctrlEditor.UpdateCSH(CCshRichEditCtrl::FORCE_CSH);
}

/** Enables/disables the button if we have an internal editor.*/
void CMscGenApp::OnUpdateCheckCsh(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_bShowCsh);
    pCmdUI->Enable(IsInternalEditorRunning());
}

/** Reads the current value of the Csh scheme combo box,
 * updates the registry
 * Updates csh info in the internal editor, if any.*/
void CMscGenApp::OnComboCsh()
{
    CMainFrame *pMainWnd = dynamic_cast<CMainFrame*>(GetMainWnd());
    if (!pMainWnd) return;
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;
    pMainWnd->m_wndRibbonBar.GetElementsByID(IDC_COMBO_CSH, arButtons);
    _ASSERT(arButtons.GetSize()==1);
	CMFCRibbonComboBox *c = dynamic_cast<CMFCRibbonComboBox*>(arButtons[0]);
    if (m_nCshScheme == c->GetCurSel()) 
        return;
    m_nCshScheme = c->GetCurSel();
	WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_CSHSCHEME, m_nCshScheme);
    if (IsInternalEditorRunning()) 
        m_pWndEditor->m_ctrlEditor.UpdateCSH(CCshRichEditCtrl::FORCE_CSH);
}


/** Enables the Csh combo box if csh is enabled and we have 
 * an internal editor*/
void CMscGenApp::OnUpdateComboCsh(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_bShowCsh && IsInternalEditorRunning());
}


/** Toggles the smart identation button.
 * Updates the registry.
 * Updates the csh info if the internal editor is running.*/
void CMscGenApp::OnCheckSmartIdent()
{
    m_bSmartIdent = !m_bSmartIdent;
	WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_SMARTIDENT, m_bSmartIdent);
    if (IsInternalEditorRunning() && m_bSmartIdent)
        m_pWndEditor->m_ctrlEditor.UpdateCSH(CCshRichEditCtrl::HINTS_AND_LABELS);
}

/** Toggles the smart identation button.
* Updates the registry.
* Updates the csh info if the internal editor is running.*/
void CMscGenApp::OnCheckTABIdents()
{
    m_bTABIdents = !m_bTABIdents;
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_TABIDENTS, m_bTABIdents);
}

/** Enables the button only if we show csh and the internal editor is running.
 * Checks it if m_bTABIdents is true.*/
void CMscGenApp::OnUpdateCheckTABIdents(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_bSmartIdent && IsInternalEditorRunning());
    pCmdUI->SetCheck(m_bTABIdents);
}

/** Enables the button only if we show csh and the internal editor is running.
* Checks it if m_bSmartIdent is true.*/
void CMscGenApp::OnUpdateCheckSmartIdent(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(IsInternalEditorRunning());
    pCmdUI->SetCheck(m_bSmartIdent);
}

/** Toggels the colorize errors checkbox.
 * Updates the registry.
 * Updates the csh info if the internal editor is running.*/
void CMscGenApp::OnCheckCshError()
{
    m_bShowCshErrors = !m_bShowCshErrors;
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_CSHERRORS, m_bShowCshErrors);
    if (IsInternalEditorRunning()) 
        m_pWndEditor->m_ctrlEditor.UpdateCSH(CCshRichEditCtrl::FORCE_CSH);
}

/** Enables the button only if we show csh and the internal editor is running.
 * Checks it if m_bShowCshErrors is true.*/
void CMscGenApp::OnUpdateCheckCshError(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(IsInternalEditorRunning());
    pCmdUI->SetCheck(m_bShowCshErrors);
}

/** Toggles whether the csh errors are listed in the error window.
 * Updates the registry.
 * Updates the csh info if the internal editor is running.*/
void CMscGenApp::OnCheckCshErrorInWindow()
{
    m_bShowCshErrorsInWindow = !m_bShowCshErrorsInWindow;
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_CSHERRORSINWINDOW, m_bShowCshErrorsInWindow); 
    if (IsInternalEditorRunning())
        m_pWndEditor->m_ctrlEditor.UpdateCSH(CCshRichEditCtrl::FORCE_CSH);
}

/** Enables the button only if we show csh and the internal editor is running.
 * Checks it if m_bShowCshErrorsInWindow is true.*/
void CMscGenApp::OnUpdateCheckCshErrorInWindow(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(IsInternalEditorRunning());
    pCmdUI->SetCheck(m_bShowCshErrorsInWindow);
}

/** Opens the external text editor preferences dialog*/
void CMscGenApp::OnEditPreferences()
{
	CMscGenDoc *pDoc = GetDoc();

	COptionDlg optionDlg;
	optionDlg.m_TextEditStartCommand = m_sStartTextEditor;
	optionDlg.m_TextEditorJumpToLineCommand = m_sJumpToLine;

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

/** Enables the button if the other external text editor is selected.*/
void CMscGenApp::OnUpdateEditPreferences(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_iTextEditorType==OTHER);
}

//C:\Windows\Notepad.exe %n;"C:\Program Files\Notepad++\notepad++.exe" -multiInst -nosession %n;
//;-n%l -nosession %n;


/** Selects the "other" external editor type. Restarts the external editor if needed*/
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

/** Selects the notepad as external editor type. 
 * Restarts the external editor if needed*/
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

/** Selects the notepad++ as external editor type. 
 * Restarts the external editor if needed*/
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


/** Always enable the button.
 * Check it if the type of external editor selected is other */
void CMscGenApp::OnUpdateCheckEeOther(CCmdUI *pCmdUI)
{
    pCmdUI->Enable();
    pCmdUI->SetCheck(m_iTextEditorType==OTHER);
}

/** Always enable the button.
 * Check it if the type of external editor selected is notepad*/
void CMscGenApp::OnUpdateCheckEeNotepad(CCmdUI *pCmdUI)
{
    pCmdUI->Enable();
    pCmdUI->SetCheck(m_iTextEditorType==NOTEPAD);
}


/** Always enable the button.
 * Check it if the type of external editor selected is notepad++*/
void CMscGenApp::OnUpdateCheckEeNotepadpp(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_NppPath.GetLength()>0);
    pCmdUI->SetCheck(m_iTextEditorType==NPP);
}

/** Toggle whether hints are provided or not.
* Updates the registry*/
void CMscGenApp::OnCheckHints()
{
    m_bHints = !m_bHints;
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_HINT, m_bHints);
}

/** Check the button if hints are provided*/
void CMscGenApp::OnUpdateCheckHints(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_bHints);
}

/** Toggle whether hints are provided or not.
* Updates the registry*/
void CMscGenApp::OnCheckSmartHintCompact()
{
    m_bHintCompact = !m_bHintCompact;
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_SHOW_CONTROLS, m_bHintCompact);
}


/** Toggle whether hints are filtered or not.
* Updates the registry*/
void CMscGenApp::OnCheckSmartHintFilter()
{
    m_bHintFilter = !m_bHintFilter;
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_HINT_FILTER, m_bHintFilter);
}


/** Toggle whether hints are provided at the beginning of a line or not.
* Updates the registry*/
void CMscGenApp::OnCheckSmartHintLineStart()
{
    m_bHintLineStart = !m_bHintLineStart;
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_HINT_LINESTART, m_bHintLineStart);
}


/** Toggle whether hints are provided for entities or not.
* Updates the registry*/
void CMscGenApp::OnCheckSmartHintEntity()
{
    m_bHintEntity = !m_bHintEntity;
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_HINT_ENTITY, m_bHintEntity);
}


/** Toggle whether hints are provided for attribute names or not.
* Updates the registry*/
void CMscGenApp::OnCheckSmartHintAttrName()
{
    m_bHintAttrName = !m_bHintAttrName;
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_HINT_ATTRNAME, m_bHintAttrName);
}


/** Toggle whether hints are provided for attribute values or not.
* Updates the registry*/
void CMscGenApp::OnCheckSmartHintAttrValue()
{
    m_bHintAttrValue = !m_bHintAttrValue;
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_HINT_ATTRVALUE, m_bHintAttrValue);
}


/** Enables the button if we provide hints.
 * Sets it according to its current value.*/
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



/** Updates the tracking color after a change of the respective button on the ribbon.
 * Updates the registry*/
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
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_TRACKLINERGBA, m_trackLineColor);
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_TRACKFILLRGBA, m_trackFillColor);
}

/** Updates the fallback image resolution after a change on the slider.
 * Updates the registry*/
void CMscGenApp::OnEmbeddedoptionsFallbackRes()
{
    CMainFrame *pMainWnd = dynamic_cast<CMainFrame*>(GetMainWnd());
    if (!pMainWnd) return;
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;
    pMainWnd->m_wndRibbonBar.GetElementsByID(ID_EMBEDDEDOPTIONS_FALLBACK_RES, arButtons);
    _ASSERT(arButtons.GetSize()==1);
    CMFCRibbonSlider *s = dynamic_cast<CMFCRibbonSlider *>(arButtons[0]);
    if (!s) return;
    if (m_uFallbackResolution == s->GetPos()) return; //nothing to do
    m_uFallbackResolution = s->GetPos();

    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_FALLBACK_RESOLUTION, m_uFallbackResolution);
    //recompile if it is embdedded
    CMscGenDoc *pDoc = GetDoc();
    if (pDoc && pDoc->IsEmbedded()) {
        pDoc->m_ChartShown.SetFallbackResolution(m_uFallbackResolution);
        pDoc->ReDrawEMF();
        pDoc->CheckIfChanged();
    }
}


/** Toggles autopaginate 
 * Updates registry. Recompiles if no changes since last compilation.*/
void CMscGenApp::OnAutoPaginate()
{
    m_bAutoPaginate = !m_bAutoPaginate;
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_AUTO_PAGINATE, m_bAutoPaginate);
	CMscGenDoc *pDoc = GetDoc();
    if (pDoc) 
        pDoc->CompileEditingChart(false, false, true);
}

/** Checks the button if autopaginate is enabled.*/
void CMscGenApp::OnUpdateAutoPaginate(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_bAutoPaginate);
}


/** Toggles autoheaders.
 * Updates registry. Recompiles if no changes since last compilation.*/
void CMscGenApp::OnAutoHeaders()
{
    m_bAutoHeading = !m_bAutoHeading;
    if (!m_bAutoPaginate) return;
	CMscGenDoc *pDoc = GetDoc();
    if (pDoc && m_bAutoPaginate)
        pDoc->CompileEditingChart(false, false, true);
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_AUTO_HEADING, m_bAutoHeading);
}

/** Enables the button if automatic pagination is on, checks it if auto headings
 * are generated.*/
void CMscGenApp::OnUpdateAutoHeaders(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_bAutoHeading);
    pCmdUI->Enable(m_bAutoPaginate);
}

/** Updates the scaling value after a change of it on the GUI.
 * Updates the registry and recompiles if automatic pagination is on.*/
void CMscGenApp::OnComboScale()
{
    CMainFrame *pMainWnd = dynamic_cast<CMainFrame*>(GetMainWnd());
    if (!pMainWnd) return;
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;
    pMainWnd->m_wndRibbonBar.GetElementsByID(ID_COMBO_SCALE, arButtons);
    _ASSERT(arButtons.GetSize()==1);
	CMFCRibbonEdit *c = dynamic_cast<CMFCRibbonEdit*>(arButtons[0]);
	CMscGenDoc *pDoc = GetDoc();
    if (!pDoc) return;
    const CString val = c->GetEditText();
    int new_val = m_iScale4Pagination;
    if (1!=sscanf(val, "%i", &new_val)) {
        if (val.CompareNoCase("Fit Width")==0) new_val = -1;
        else if (val.CompareNoCase("Fit Page")==0) new_val = -2;
    }
    if (m_iScale4Pagination==new_val) 
        return;
    m_iScale4Pagination = new_val;
    //m_iScale4Pagination may have been left unchanged, but we normalize text
    pMainWnd->FillScale4Pagination();
    WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_SCALE4PAGINATION, m_iScale4Pagination);

    if (m_bAutoPaginate) 
        pDoc->CompileEditingChart(false, false, true);
    pDoc->UpdateAllViews(NULL);
}


void CMscGenApp::OnButtonPages()
{
    CMainFrame *pMainWnd = dynamic_cast<CMainFrame*>(GetMainWnd());
    if (!pMainWnd) return;
    CPreviewView *pView = dynamic_cast<CPreviewView *>(pMainWnd->GetActiveView());
    if (!pView) return;
    //TODO: Change page size in print prview
    OnFilePrintSetup();
    pMainWnd->FillPageSize();
}

void CMscGenApp::OnComboAlignment()
{
    CMainFrame *pMainWnd = dynamic_cast<CMainFrame*>(GetMainWnd());
    if (!pMainWnd) return;
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;
    pMainWnd->m_wndRibbonBar.GetElementsByID(ID_COMBO_ALIGNMENT, arButtons);
    _ASSERT(arButtons.GetSize()==1);
    CMFCRibbonComboBox *c = dynamic_cast<CMFCRibbonComboBox*>(arButtons[0]);
    CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
    ASSERT(pApp != NULL);
    pApp->m_iPageAlignment = c->GetCurSel() - 4;
    pApp->WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_PAGE_ALIGNMENT, pApp->m_iPageAlignment);
    CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(pMainWnd->GetActiveDocument());
    if (pDoc)
        pDoc->UpdateAllViews(NULL);
}




/** Update our user specified margins after a change of them on the GUI.
 * Update the registry and recompile if needed.*/
void CMscGenApp::DoEditMargin(UINT id)
{
    _ASSERT(id == ID_EDIT_MARGIN_L || id == ID_EDIT_MARGIN_R ||
            id == ID_EDIT_MARGIN_T || id == ID_EDIT_MARGIN_B);
    CMainFrame *pMainWnd = dynamic_cast<CMainFrame*>(GetMainWnd());
    if (!pMainWnd) return;
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;
    pMainWnd->m_wndRibbonBar.GetElementsByID(id, arButtons);
    _ASSERT(arButtons.GetSize()==1);
	CMFCRibbonEdit *c = dynamic_cast<CMFCRibbonEdit*>(arButtons[0]);
	CMscGenDoc *pDoc = GetDoc();
    if (!pDoc || !c) return;
    const CString val = c->GetEditText();
    bool change = false;
    double cm;
    if (1==sscanf(val, "%lf", &cm)) {
        switch (id) {
        case ID_EDIT_MARGIN_L: m_printer_usr_margins[0] = cm * PT_PER_CM; break;
        case ID_EDIT_MARGIN_R: m_printer_usr_margins[1] = cm * PT_PER_CM; break;
        case ID_EDIT_MARGIN_T: m_printer_usr_margins[2] = cm * PT_PER_CM; break;
        case ID_EDIT_MARGIN_B: m_printer_usr_margins[3] = cm * PT_PER_CM; break;
        }
        NormalizeUserMargins();
        change = true;
    }
    //m_printer_usr_margins may have been left unchanged, but we normalize text
    pMainWnd->FillMargins();
    if (!change) return;

    CString val2;
    val2.Format("%lf", m_printer_usr_margins[0]);
    WriteProfileString(REG_SECTION_SETTINGS, REG_KEY_PAGE_MARGIN_L, val2);
    val2.Format("%lf", m_printer_usr_margins[1]);
    WriteProfileString(REG_SECTION_SETTINGS, REG_KEY_PAGE_MARGIN_R, val2); 
    val2.Format("%lf", m_printer_usr_margins[2]);
    WriteProfileString(REG_SECTION_SETTINGS, REG_KEY_PAGE_MARGIN_T, val2);
    val2.Format("%lf", m_printer_usr_margins[3]);
    WriteProfileString(REG_SECTION_SETTINGS, REG_KEY_PAGE_MARGIN_B, val2);

    if (m_bAutoPaginate) 
        pDoc->CompileEditingChart(false, false, true);
    pDoc->UpdateAllViews(NULL);
}

/** Enable these edit boxes if we are in print preview*/
void CMscGenApp::OnUpdatePrintPreviewEdits(CCmdUI *pCmdUI) 
{
    CMainFrame *pWnd = dynamic_cast<CMainFrame *>(GetMainWnd());
    pCmdUI->Enable(pWnd ? pWnd->IsPrintPreview() : false);
}

/** Open the print setup dialog, read the page size after and
 * update our internal values. Recompile if needed.*/
void CMscGenApp::OnFilePrintSetup()
{
    CMainFrame *pMainWnd= dynamic_cast<CMainFrame *>(GetMainWnd());
    if (!pMainWnd) return;
    XY ps = m_PhyPrinterPageSize;
    CWinAppEx::OnFilePrintSetup();
    UpdatePrinterData();
    if (m_PhyPrinterPageSize == ps) return;
    pMainWnd->FillPageSize();
    pMainWnd->FillMargins();

	CMscGenDoc *pDoc = GetDoc();
    if (!pDoc) return;
    if (m_bAutoPaginate) 
        pDoc->CompileEditingChart(false, false, true);
    pDoc->UpdateAllViews(NULL);
}

/** Enable these edit boxes if we are in print preview*/
void CMscGenApp::OnUpdatePrintSetup(CCmdUI *pCmdUI) 
{
    CMainFrame *pWnd = dynamic_cast<CMainFrame *>(GetMainWnd());
    pCmdUI->Enable(pWnd ? !pWnd->IsPrintPreview() : false);
}

