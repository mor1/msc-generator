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


// CMscGenApp

BEGIN_MESSAGE_MAP(CMscGenApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CMscGenApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CMscGenApp construction

CMscGenApp::CMscGenApp()
{
	m_bHiColorIcons = TRUE;

	// Place all significant initialization in InitInstance
}


// The one and only CMscGenApp object

CMscGenApp theApp;
// This identifier was generated to be statistically unique for your app
// You may change it if you prefer to choose a specific identifier
// {453AC3C8-F260-4D88-832A-EC957E92FDEC}
static const CLSID clsid =
{ 0x453AC3C8, 0xF260, 0x4D88, { 0x83, 0x2A, 0xEC, 0x95, 0x7E, 0x92, 0xFD, 0xEC } };


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

#ifdef _DEBUG
	MessageBox(0, "In Msc-generator::Appinit", "aa", 0);
#endif

	// App was launched with /Embedding or /Automation switch.
	// Run app as automation server.
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
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

// CMscGenApp message handlers

//
//// Command-line operation
//const char * usage()
//{
//    return 
//"Usage: mscgen [infile]\n"
//"       mscgen -T <type> [-o <file>] [<infile>] [-q] \n"
//"       mscgen -l\n"
//"       mscgen /register\n"
//"\n"
//"Where:\n"
//" -T <type>   Invokes command-line mode and specifies the output file type,\n"
//"             which maybe one of 'png', 'eps', 'pdf', 'svg' or 'wmf'.\n"
//"             The absence of -T, -l and /registed makes Msc-generator to start\n"
//"             as a windowed program.\n"
//" -o <file>   Write output to the named file.  If omitted the input filename\n"
//"             will be appended by .png or .ps. If neither input nor output \n"
//"             file is given, mscgen_out.{png,eps,svg,pdf,wmf} will be used.\n"
//" <infile>    The file from which to read input.  If omitted or specified as\n"
//"             '-', input will be read from stdin.\n"
//" -q          Quiet: Errors and warnings will not display in a dialog.\n"
//" -l          Display program licence and exit.\n"
//"/register    Run this once as administrator, from the installed location to\n"
//"             perform OLE registration.\n"
//"\n"
//"Msc-generator version 2.3, Copyright (C) 2008-10 Zoltan Turanyi,\n"
//"Msc-generator comes with ABSOLUTELY NO WARRANTY.  This is free software, and you are\n"
//"welcome to redistribute it under certain conditions; type `mscgen -l' for\n"
//"details.\n";
//}
//
///** Print program licence and return.
// */
//const char * licence()
//{
//return 
//"Msc-generator, a message sequence chart renderer.\n"
//"Copyright (C) 2008-2009-2010 Zoltan Turanyi\n"
//"Distributed under GNU Affero General Public License.\n"
//"\n"
//"This program is free software; you can redistribute it and/or modify\n"
//"it under the terms of the GNU Affero General Public License as published by\n"
//"the Free Software Foundation; either version 3 of the License, or\n"
//"(at your option) any later version.\n"
//"\n"
//"This program is distributed in the hope that it will be useful,\n"
//"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
//"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
//"GNU General Public License for more details.\n"
//"\n"
//"You should have received a copy of the GNU Affero General Public License\n"
//"along with this program; if not, see <http://www.gnu.org/licenses/>.\n";
//}
//
//CString GetNextArg(CString &cmd) 
//{
//	while (cmd.GetLength()>0) {
//		int pos = cmd.Find(' ');
//		if (pos == -1) pos = cmd.GetLength();
//		CString arg = cmd.Left(pos);
//		cmd.Delete(0,pos+1);
//		if (arg.GetLength()>0) return arg;
//	}
//	return CString();
//}
//
