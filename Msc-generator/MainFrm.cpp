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

// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Msc-generator.h"

#include "MainFrm.h"
#include "MscGenDoc.h"
#include "MscGenView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, OnViewCustomize)
//	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, OnToolbarReset)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SIZE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2005);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,
		                       CRect(1,1,1,1), theApp.m_bHiColorIcons ? IDR_TOOLBAR_MAINFRAME_256 : IDR_TOOLBAR_MAINFRAME) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_TOOLBAR_MAINFRAME_256 : IDR_TOOLBAR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	if (!m_wndDesignBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,
		                         CRect(1,1,1,1), IDR_DESIGNBAR) ||
		!m_wndDesignBar.LoadToolBar(IDR_DESIGNBAR))
	{
		TRACE0("Failed to create designbar\n");
		return -1;      // fail to create
	}
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_DESIGN);
	ASSERT(bNameValid);
	m_wndDesignBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	m_wndDesignBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// Allow user-defined toolbars operations:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	if (!m_wndOutputView.Create(_T("Errors and Warnings"), this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUT, 
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create output bar\n");
		return FALSE;      // fail to create
	}
	static_cast<CMscGenApp*>(AfxGetApp())->m_pWndOutputView = &m_wndOutputView;

	if (!m_wndEditor.Create(_T("Chart Text"), this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_EDITOR, 
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create output bar\n");
		return FALSE;      // fail to create
	}
	static_cast<CMscGenApp*>(AfxGetApp())->m_pWndEditor = &m_wndEditor;

	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndDesignBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndOutputView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndEditor.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);
	DockPane(&m_wndDesignBar);
	DockPane(&m_wndOutputView);
	DockPane(&m_wndEditor);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	// enable menu personalization (most-recently used commands)
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_FILE_EXPORT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_DESIGN_PAGE);
	lstBasicCommands.AddTail(ID_DESIGN_ZOOM);
	lstBasicCommands.AddTail(ID_DESIGN_DESIGN);
	lstBasicCommands.AddTail(ID_BUTTON_EDITTEXT);              
	lstBasicCommands.AddTail(ID_EDIT_PREFERENCES);             
	lstBasicCommands.AddTail(ID_VIEW_REDRAW);                  
	lstBasicCommands.AddTail(ID_VIEW_ZOOMIN);                  
	lstBasicCommands.AddTail(ID_VIEW_ZOOMOUT);                 
	lstBasicCommands.AddTail(ID_VIEW_ZOOMNORMALIZE);           
	lstBasicCommands.AddTail(ID_VIEW_ADJUSTWIDTH);             
	lstBasicCommands.AddTail(ID_VIEW_FITTOWIDTH);              
	lstBasicCommands.AddTail(ID_ZOOMMODE_KEEPINOVERVIEW);      
	lstBasicCommands.AddTail(ID_ZOOMMODE_KEEPADJUSTINGWINDOWWIDTH); 
	lstBasicCommands.AddTail(ID_ZOOMMODE_KEEPFITTINGTOWIDTH);  

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	SetWindowPos(NULL, 0, 0, 550, 300,  SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	if (m_wndEditor.IsVisible()) m_wndEditor.m_wndEditor.SetFocus();

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	return m_wndSplitter.Create(this,
		2, 1,               // TODO: adjust the number of rows, columns
		CSize(10, 10),      // TODO: adjust the minimum pane size
		pContext,
		WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | SPLS_DYNAMIC_SPLIT);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnViewCustomize()
{
	//------------------------------------
	// Create a customize toolbars dialog:
	//------------------------------------
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog (this,
		TRUE /* Automatic menus scaning */
		);
	pDlgCust->EnableUserDefinedToolbars();

	CMFCToolBarComboBoxButton pageButton (ID_DESIGN_PAGE,
		GetCmdMgr ()->GetCmdImage (ID_DESIGN_PAGE, FALSE), CBS_DROPDOWNLIST, 50);
	pDlgCust->ReplaceButton(ID_DESIGN_PAGE, pageButton);

	CMFCToolBarComboBoxButton zoomButton (ID_DESIGN_ZOOM,
		GetCmdMgr ()->GetCmdImage (ID_DESIGN_ZOOM, FALSE), CBS_DROPDOWN, 50);
	pDlgCust->ReplaceButton(ID_DESIGN_ZOOM, zoomButton);

	CMFCToolBarComboBoxButton designButton (ID_DESIGN_DESIGN,
		GetCmdMgr ()->GetCmdImage (ID_DESIGN_DESIGN, FALSE));
	pDlgCust->ReplaceButton(ID_DESIGN_DESIGN, designButton);

	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

LRESULT CMainFrame::OnToolbarReset(WPARAM wp, LPARAM)
{
	UINT uiToolBarId = (UINT) wp;  
 
	if (uiToolBarId==IDR_DESIGNBAR) {
		CMFCToolBarComboBoxButton pageButton (ID_DESIGN_PAGE,
			GetCmdMgr ()->GetCmdImage (ID_DESIGN_PAGE, FALSE), CBS_DROPDOWNLIST, 50);
		m_wndDesignBar.ReplaceButton(ID_DESIGN_PAGE, pageButton);

		CMFCToolBarComboBoxButton zoomButton (ID_DESIGN_ZOOM,
			GetCmdMgr ()->GetCmdImage (ID_DESIGN_ZOOM, FALSE), CBS_DROPDOWN, 50);
		m_wndDesignBar.ReplaceButton(ID_DESIGN_ZOOM, zoomButton);

		CMFCToolBarComboBoxButton designButton (ID_DESIGN_DESIGN,
			GetCmdMgr ()->GetCmdImage (ID_DESIGN_DESIGN, FALSE));
		m_wndDesignBar.ReplaceButton(ID_DESIGN_DESIGN, designButton);

		CMscGenDoc *pDoc = static_cast<CMscGenDoc *>(GetActiveDocument());
		if (pDoc) {
			CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
			ASSERT(pApp != NULL);
			pApp->FillDesignDesignCombo(pDoc->m_itrCurrent->GetDesign(), true);
			pApp->FillDesignPageCombo(pDoc->m_pages, pDoc->m_page);
			pDoc->SetZoom(pDoc->m_zoom); //reinforce value - it merely fills in zoom combo with list of zoom values
		}
	}	  

	return 0; 	
}


void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// base class does the real work

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// enable customization button for all user toolbars
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}



void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWndEx::OnSize(nType, cx, cy);
	CMscGenDoc *pDoc = static_cast<CMscGenDoc *>(GetActiveDocument());
	if (pDoc && pDoc->m_ZoomMode == CMscGenDoc::ZOOM_WIDTH) 
		pDoc->ArrangeViews(pDoc->m_ZoomMode);
}
