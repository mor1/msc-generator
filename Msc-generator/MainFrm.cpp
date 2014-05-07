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

BOOL CMscGenSplitterWnd::SplitRow(int cyBefore)
{
    return CSplitterWnd::SplitRow(cyBefore);
}

void CMscGenSplitterWnd::DeleteRow(int rowDelete)
{
    CMainFrame *pWnd = dynamic_cast<CMainFrame *>(GetParent());
    if (pWnd)
        pWnd->m_bAutoSplit = false;
    CSplitterWnd::DeleteRow(rowDelete);
}



// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;


//#define ID_VIEW_APPLOOK_WIN_2000        210
//#define ID_VIEW_APPLOOK_OFF_XP          211
//#define ID_VIEW_APPLOOK_WIN_XP          212
//#define ID_VIEW_APPLOOK_OFF_2003        213
//#define ID_VIEW_APPLOOK_VS_2005         214
//#define ID_VIEW_APPLOOK_OFF_2007_BLUE   215
//#define ID_VIEW_APPLOOK_OFF_2007_BLACK  216
//#define ID_VIEW_APPLOOK_OFF_2007_SILVER 217
//#define ID_VIEW_APPLOOK_OFF_2007_AQUA   218
//#define ID_VIEW_APPLOOK_VS_2008         219
//#define ID_VIEW_APPLOOK_WINDOWS_7       220

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_FULL_SCREEN, OnViewFullScreen)
	ON_COMMAND(ID_BUTTON_AUTO_SPLIT, OnButtonAutoSplit)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_AUTO_SPLIT, OnUpdateButtonAutoSplit)
//	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
    ON_WM_ACTIVATE()
	ON_COMMAND(ID_DESIGN_ZOOM, OnDesignZoom)
	ON_CBN_SELENDOK(ID_DESIGN_ZOOM, OnDesignZoom)
	ON_COMMAND(ID_DESIGN_PAGE, OnDesignPage)
	ON_COMMAND(ID_FULL_SCREEN_PAGE, OnFullScreenPage)
	ON_CBN_SELENDOK(ID_DESIGN_PAGE, OnDesignPage)
	ON_UPDATE_COMMAND_UI(ID_DESIGN_PAGE, OnUpdateDesignPage)
	ON_COMMAND(ID_DESIGN_PAGE_FULL_SCREEN_NEXT, OnDesignNextPage)
	ON_COMMAND(ID_DESIGN_PAGE_FULL_SCREEN_PREV, OnDesignPrevPage)
	ON_COMMAND(ID_DESIGN_DESIGN, OnDesignDesign)
	ON_CBN_SELENDOK(ID_DESIGN_DESIGN, OnDesignDesign)
	ON_UPDATE_COMMAND_UI(ID_DESIGN_DESIGN, OnUpdateDesignDesign)
	ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomin)
	ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomout)
	ON_COMMAND(ID_BUTTON_EDITINTERNAL, OnViewInternalEditor)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_EDITINTERNAL, OnUpdateViewInternalEditor)
    ON_UPDATE_COMMAND_UI(ID_BUTTON_DEFAULT_TEXT, OnUpdateButtonDefaultText)
    ON_UPDATE_COMMAND_UI(ID_EMBEDDEDOPTIONS_FALLBACK_RES, OnUpdateEmbeddedoptionsFallbackRes)
    ON_MESSAGE(WM_APP+293, OnCompilationDone)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_TRK,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame() 
{
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2005);
    m_bAutoSplit = false;
    m_at_embedded_object_category = false;
    m_has_fallback_image = false;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);

	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;
    m_wndRibbonBar.GetElementsByID(0, arButtons);
    for (unsigned i = 0; i<arButtons.GetSize(); i++) {
        const char *c = arButtons[i]->GetText();
        if (!strcmp("ObjectSize", c))
            m_labelObjectSize = dynamic_cast<CMFCRibbonLabel*>(arButtons[i]);
        else if (!strcmp("FallbackImage", c))
            m_labelFallbackImage = dynamic_cast<CMFCRibbonLabel*>(arButtons[i]);
    }
    _ASSERT(m_labelFallbackImage);
    _ASSERT(m_labelObjectSize);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	m_wndStatusBar.EnablePaneDoubleClick();
	m_wndStatusBar.SetTipText(1, _T("Toggle Tracking Mode")); //index 1 is ID_INDICATOR_TRK

	if (!m_wndOutputView.Create(_T("Errors and Warnings"), this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUT, 
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create output bar\n");
		return FALSE;      // fail to create
	}
	pApp->m_pWndOutputView = &m_wndOutputView;

	if (!m_ctrlEditor.Create(_T("Internal Editor"), this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_EDITOR, 
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create output bar\n");
		return FALSE;      // fail to create
	}
	pApp->m_pWndEditor = &m_ctrlEditor;

	m_wndOutputView.EnableDocking(CBRS_ALIGN_ANY);
	m_ctrlEditor.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutputView);
	DockPane(&m_ctrlEditor);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	if (m_ctrlEditor.IsVisible()) 
        m_ctrlEditor.SetFocus();

	EnableFullScreenMode(ID_VIEW_FULL_SCREEN);
	EnableFullScreenMainMenu(FALSE);


    //Set inital value of CSH scheme combo box
    m_wndRibbonBar.GetElementsByID(IDC_COMBO_CSH, arButtons);
    _ASSERT(arButtons.GetSize()==1);
	CMFCRibbonComboBox *c = dynamic_cast<CMFCRibbonComboBox*>(arButtons[0]);
    _ASSERT(c);
    c->SelectItem(int(pApp->m_nCshScheme));

    //Set initial value for tracking color button
    m_wndRibbonBar.GetElementsByID(ID_BUTTON_TRACK_COLOR, arButtons);
    _ASSERT(arButtons.GetSize()==1);
    CMFCRibbonColorButton *cb = dynamic_cast<CMFCRibbonColorButton*>(arButtons[0]);    
    //Make it 50% lighter - as if 50% transparent on white background
    unsigned char r = GetRValue(pApp->m_trackFillColor); 
    unsigned char g = GetGValue(pApp->m_trackFillColor); 
    unsigned char b = GetBValue(pApp->m_trackFillColor); 
    cb->SetColor(RGB(r, g, b));

    //Set initial values for thes embedded options
    m_wndRibbonBar.GetElementsByID(ID_EMBEDDEDOPTIONS_FALLBACK_RES, arButtons);
    _ASSERT(arButtons.GetSize()==1);
    CMFCRibbonSlider *s = dynamic_cast<CMFCRibbonSlider *>(arButtons[0]);
    if (s) s->SetPos(pApp->m_uFallbackResolution);

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

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
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

	//Actualize combo box values
	CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(GetActiveDocument());
	if (pDoc) {
		FillDesignComboBox(pDoc->m_itrEditing->GetDesign(), true);
		FillPageComboBox(pDoc->m_ChartShown.GetPages(), pDoc->m_ChartShown.GetPage());
		FillZoomComboBox(pDoc->m_zoom); //merely fill in zoom combo with list of zoom values
	}

	return TRUE;
}

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/) 
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    if (pApp->IsInternalEditorRunning() && pApp->m_pWndEditor->IsVisible())
		pApp->m_pWndEditor->m_ctrlEditor.SetFocus();
}

//We need to override it to capture ESCAPE key.
//CFrameWndEx handles it in this message by shutting down full screen
//We do the same, but if we show an embedded object in fullscreen mode, we have to exit 
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	if (pApp->m_bFullScreenViewMode && pMsg->message == WM_KEYDOWN && 
		pMsg->wParam == VK_ESCAPE && IsFullScreen()) {
		//CFrameWndEx::PreTranslateMessage first checks it against a menu
		//But we have no menus in full screen mode
		//Ok, Hide window and kill it:
		CMainFrame::OnViewFullScreen();
		return TRUE;
	}
	return CFrameWndEx::PreTranslateMessage(pMsg);
}

void CMainFrame::OnViewFullScreen() 
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	//if we show an embedded object in fullscreen mode, we have to exit now.
	if (pApp->m_bFullScreenViewMode && IsFullScreen()) {
        //Here we end a session opened by the Show in Full Screen OLE action
		//Hide, so user does not see as fullscreen is first restored
		ShowWindow(SW_HIDE);
		//Exit app.
		//Instead of CFrameWndEx::OnClose we call CFrameWnd::OnClose.
		//The former does the following before calling CFrameWnd::OnClose:
		//1. It shuts down print preview (ethere is none in full screen view mode)
		//2. it kills OLE container (we are not one, there is nothing to kill); 
		//3. it restores Fullscreen (we do not want this: we just want the full screen to disapear)
		//4. saves window state by calling CWinAppEx::OnClosingMainFrame
		//5. saves window placement by calling CFrameImpl::StoreWindowPlacement.
		//The last two we dont want since a full screen windows has all the wrong state and placement
		CFrameWnd::OnClose();
	} else {
		if (!IsFullScreen()) {
            ShowFullScreen();
            AddToFullScreenToolbar();
        } else {
            //If we are cancelling full screen update the zoom combo box with current value
			CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(GetActiveDocument());
			if (pDoc) 
				pDoc->SetZoom(); 
            ShowFullScreen();
        }
	}
}

LRESULT CMainFrame::OnCompilationDone(WPARAM wParam, LPARAM lParam)
{
    CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(GetActiveDocument());
    if (pDoc) 
        pDoc->CompleteCompilingEditingChart();
    return 0; //OK, we processed it
}

CPaneFrameWnd *CMainFrame::FindFullScreenToolBarFrameWnd()
{
    CString strCaption;
    strCaption.LoadString(IDS_AFXBARRES_FULLSCREEN);
    for (CWnd *i = GetWindow(GW_HWNDFIRST); i; i=i->GetWindow(GW_HWNDNEXT)) {
        CString s;
        i->GetWindowText(s);
        if (s != strCaption) continue;
        if (!i->IsKindOf(RUNTIME_CLASS(CPaneFrameWnd))) continue;
        //OK, we found the right paneframewnd
        return dynamic_cast<CPaneFrameWnd *>(i);
    }
    return NULL;
}


CMFCToolBar *CMainFrame::FindFullScreenToolBar(CPaneFrameWnd *i)
{
    if (!i)
        i = FindFullScreenToolBarFrameWnd();
    if (!i)
        return NULL;
    CString strCaption;
    strCaption.LoadString(IDS_AFXBARRES_FULLSCREEN);
    for (CWnd *j = i->GetWindow(GW_CHILD); j; j=j->GetWindow(GW_HWNDNEXT)) {
        CString s;
        j->GetWindowText(s);
        if (s != strCaption) continue;
        if (!j->IsKindOf(RUNTIME_CLASS(CMFCToolBar))) continue;
        //OK, we found the toolbar in it
        return dynamic_cast<CMFCToolBar *>(j);
    }
    return NULL;
}

bool CMainFrame::AddToFullScreenToolbar() //finds the toolbar and adds our buttons to it
{
    CPaneFrameWnd *f = FindFullScreenToolBarFrameWnd();
    if (!f) return false;
    CMFCToolBar *p = FindFullScreenToolBar(f);
    if (!p) return false;
    p->InsertSeparator();
    CSize size = p->GetButtonSize();
    //Add autoSplit Button
    int buttonBitMap = CMFCToolBar::GetDefaultImage(ID_BUTTON_AUTO_SPLIT);
    CMFCToolBarButton button(ID_BUTTON_AUTO_SPLIT, buttonBitMap, "AutoSplit", TRUE, TRUE);
    p->InsertButton(button);
    //Add page combo, if more than one page
    CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(GetActiveDocument());
    CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
    if (pDoc && pApp && pDoc->m_ChartShown.GetPages()>1) {
        CMFCToolBarButton nextButton (ID_DESIGN_PAGE_FULL_SCREEN_NEXT,
            GetCmdMgr ()->GetCmdImage (ID_DESIGN_PAGE_FULL_SCREEN_NEXT, FALSE), "Next");
        CMFCToolBarButton prevButton (ID_DESIGN_PAGE_FULL_SCREEN_PREV,
            GetCmdMgr ()->GetCmdImage (ID_DESIGN_PAGE_FULL_SCREEN_PREV, FALSE), "Prev");
        CMFCToolBarEditBoxButton edit (ID_FULL_SCREEN_PAGE, 
            GetCmdMgr ()->GetCmdImage (ID_FULL_SCREEN_PAGE, FALSE), 
            ES_NUMBER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_RIGHT, 50);
        p->InsertSeparator();
        p->InsertButton(prevButton);
        p->InsertButton(edit);
        p->InsertButton(nextButton);
        FillPageComboBox(pDoc->m_ChartShown.GetPages(), pDoc->m_ChartShown.GetPage());
    }
    //Arrange size
    p->StretchPane(32000, false);
    f->SizeToContent();
    return true;
}


void CMainFrame::AddToPrintPreviewCategory()
{
    //find print preview category
    const int count = m_wndRibbonBar.GetCategoryCount();
    CMFCRibbonCategory* pRC = NULL;
    const CString name = "Print Preview";
    for (int i = 0; i<count; i++) {
        CMFCRibbonCategory* p = m_wndRibbonBar.GetCategory(i);
        if (name.CompareNoCase(p->GetName())) continue;
        pRC = p;
        break;
    }
    if (!pRC || 3 < pRC->GetPanelCount()) return;

	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);

    //Add new panel
    CMFCRibbonPanel *pRP = pRC->AddPanel("Pagination");
    pRP->SetCenterColumnVert();
    pRP->SetJustifyColumns();

    //Add panel content
    CMFCRibbonCheckBox *pRButt = new CMFCRibbonCheckBox(ID_AUTO_PAGINATE, "Auto Paginate");
    pRP->Add(pRButt);
    pRButt = new CMFCRibbonCheckBox(ID_AUTO_HEADERS, "Auto Heading");
    pRP->Add(pRButt);
    CMFCRibbonComboBox *pRCB = new CMFCRibbonComboBox(ID_COMBO_SCALE, true, 60, "Scale");
    pRCB->EnableDropDownListResize();
    pRCB->AddItem("25%");
    pRCB->AddItem("50%");
    pRCB->AddItem("75%");
    pRCB->AddItem("100%");
    pRCB->AddItem("125%");
    pRCB->AddItem("150%");
    pRCB->AddItem("Fit width");
    pRCB->AddItem("Fit page");
    if (pApp->m_iScale4Pagination<=-2)
        pRCB->SelectItem(8);
    else if (pApp->m_iScale4Pagination==-1)
        pRCB->SelectItem(7);
    else {
        CString val;
        val.Format("%u%%", pApp->m_iScale4Pagination);
        pRCB->SetEditText(val);
    }
    pRP->Add(pRCB);

    pRP->AddSeparator();

    //pRCB = new CMFCRibbonComboBox(ID_COMBO_PAGES, false, 70, "Page Size");
    //pRCB->AddItem("none");    
    //pRCB->AddItem("A0 portrait");    
    //pRCB->AddItem("A0 landscape");    
    //pRCB->AddItem("A1 portrait");    
    //pRCB->AddItem("A1 landscape");    
    //pRCB->AddItem("A2 portrait");    
    //pRCB->AddItem("A2 landscape");    
    //pRCB->AddItem("A3 portrait");   
    //pRCB->AddItem("A3 landscape");  
    //pRCB->AddItem("A4 portrait");   
    //pRCB->AddItem("A4 landscape");  
    //pRCB->AddItem("A5 portrait");   
    //pRCB->AddItem("A5 landscape"); 
    //pRCB->AddItem("A6 portrait");  
    //pRCB->AddItem("A6 landscape");   
    //pRCB->AddItem("Letter portrait"); 
    //pRCB->AddItem("Letter landscape"); 
    //pRCB->AddItem("Legal portrait");   
    //pRCB->AddItem("Legal landscape"); 
    //pRCB->AddItem("Ledger");  
    //pRCB->AddItem("Tabloid");
    //pRCB->SelectItem(PageSizeInfo::EPageSize(pApp->m_pageSize));
    //pRP->Add(pRCB);
    
    CMFCRibbonEdit* pRE = new CMFCRibbonEdit(ID_EDIT_PAGES, 90, "Page size:");
    pRE->SetEditText(PageSizeInfo::ConvertPageSizeVerbose(pApp->m_pageSize));
    pRP->Add(pRE);

    CMFCRibbonButton *pRB = new CMFCRibbonButton(ID_BUTTON_PAGES, "Page setup");
    pRP->Add(pRB);

    pRCB = new CMFCRibbonComboBox(ID_COMBO_ALIGNMENT, false, 90, "Alignment:");
    pRCB->EnableDropDownListResize();
    pRCB->AddItem("Top - Left");
    pRCB->AddItem("Top - Center");
    pRCB->AddItem("Top - Right");
    pRCB->AddItem("Middle - Left");
    pRCB->AddItem("Middle - Center");
    pRCB->AddItem("Middle - Right");
    pRCB->AddItem("Bottom - Left");
    pRCB->AddItem("Bottom - Center");
    pRCB->AddItem("Bottom - Right");
    pRCB->SelectItem(pApp->m_iPageAlignment + 4);
    pRP->Add(pRCB);

    pRP->AddSeparator();
    pRP->Add(new CMFCRibbonLabel("Margins (cm)"));

    pRE = new CMFCRibbonEdit(ID_EDIT_MARGIN_L, 35, "Left");
    CString val;
    val.Format("%lg", pApp->m_printer_usr_margins[0]/PT_PER_CM); 
    pRE->SetEditText(val);
    pRP->Add(pRE);

    pRE = new CMFCRibbonEdit(ID_EDIT_MARGIN_R, 35, "Right");
    val.Format("%lg", pApp->m_printer_usr_margins[1]/PT_PER_CM); //cm to points, see http://www.asknumbers.com/CentimetersToPointsConversion.aspx
    pRE->SetEditText(val);
    pRP->Add(pRE);
    
    pRP->Add(new CMFCRibbonLabel(":"));

    pRE = new CMFCRibbonEdit(ID_EDIT_MARGIN_T, 35, "Top");
    val.Format("%lg", pApp->m_printer_usr_margins[2]/PT_PER_CM); //cm to points, see http://www.asknumbers.com/CentimetersToPointsConversion.aspx
    pRE->SetEditText(val);
    pRP->Add(pRE);

    pRE = new CMFCRibbonEdit(ID_EDIT_MARGIN_B, 35, "Bottom");
    val.Format("%lg", pApp->m_printer_usr_margins[3]/PT_PER_CM); //cm to points, see http://www.asknumbers.com/CentimetersToPointsConversion.aspx
    pRE->SetEditText(val);
    pRP->Add(pRE);

    m_wndRibbonBar.RecalcLayout();
}

void CMainFrame::DeleteFromPrintPreviewCategory()
{
    const int count = m_wndRibbonBar.GetCategoryCount();
    CMFCRibbonCategory* pRC = NULL;
    const CString name = "Print Preview";
    for (int i = 0; i<count; i++) {
        CMFCRibbonCategory* p = m_wndRibbonBar.GetCategory(i);
        if (name.CompareNoCase(p->GetName())) continue;
        pRC = p;
        break;
    }
    if (!pRC || pRC->GetPanelCount()<=3) return;
    pRC->RemovePanel(3);
}


void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    CFrameWndEx::OnActivate(nState, pWndOther, bMinimized);

    //Set focus to internal editor
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    if (nState != WA_INACTIVE)
        if (pApp->IsInternalEditorRunning() && pApp->m_pWndEditor->IsVisible())
            pApp->m_pWndEditor->SetFocus();
}


void CMainFrame::OnButtonAutoSplit()
{
    if (!m_bAutoSplit) {
        CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(GetActiveDocument());
        m_bAutoSplit = true;
        if (pDoc)
            SetSplitSize(unsigned(pDoc->m_ChartShown.GetHeadingSize()*pDoc->m_zoom/100.));
    } else {
        //turn it off
        switch (m_wndSplitter.GetRowCount()) {
        default: _ASSERT(0);
        case 1: break;
        case 2: 
            int Cur, Min;
            m_wndSplitter.GetRowInfo(0, Cur, Min);
            m_wndSplitter.DeleteRow(0); 
            CMscGenView *pView = dynamic_cast<CMscGenView *>(m_wndSplitter.GetPane(0,0));
            if (pView) {
		        CPoint pos = pView->GetScrollPosition();
                pos.y -= Cur;
                if (pos.y<0) pos.y=0;
		        pView->ScrollToPosition(pos);
            }
            break;
        }
        m_bAutoSplit = false;
    }
}

void CMainFrame::OnUpdateButtonAutoSplit(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bAutoSplit);
}


void CMainFrame::SetSplitSize(unsigned coord)
{
    if (!m_bAutoSplit) return;
    //CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(GetActiveDocument());
    //if (pDoc) coord = double(coord)*pDoc->m_zoom/100.;
    int prev=0, dummy;
    CSize orig_pos(0,0);
    switch (m_wndSplitter.GetRowCount()) {
    case 1: 
        {
        CMscGenView *pView = dynamic_cast<CMscGenView *>(m_wndSplitter.GetPane(0,0));
        if (pView) orig_pos = pView->GetScrollPosition();
        m_wndSplitter.SplitRow(coord); 
        }
        break;
    case 2: 
        {
        CMscGenView *pView = dynamic_cast<CMscGenView *>(m_wndSplitter.GetPane(1,0));
        if (pView) orig_pos = pView->GetScrollPosition();
        }
        m_wndSplitter.GetRowInfo(0, prev, dummy); 
        m_wndSplitter.SetRowInfo(0, coord, dummy); 
        m_wndSplitter.RecalcLayout();
        break;
    default: _ASSERT(0);
    }
    int now;
    m_wndSplitter.GetRowInfo(0, now, dummy); 
    //Now prev contains what was the size of pane(0) before and now is after
    CMscGenView *pView = dynamic_cast<CMscGenView *>(m_wndSplitter.GetPane(0,0));
    if (pView) {
		CPoint pos = pView->GetScrollPosition();
        pos.x = orig_pos.cx;
        pos.y = 0;
		pView->ScrollToPosition(pos);
    }
    pView = dynamic_cast<CMscGenView *>(m_wndSplitter.GetPane(1,0));
    if (pView) {
		CPoint pos = pView->GetScrollPosition();
        pos.x = orig_pos.cx;
        pos.y = orig_pos.cy + now-prev; 
        if (pos.y < coord) pos.y = coord;
		pView->ScrollToPosition(pos);
    }
}


bool CMainFrame::FillDesignComboBox(const char *current, bool updateComboContent) 
{
	//ok now designs contains a set of of designs, preable contains a concatenation of designlib text
	//Add the list of designs to the combo boxes
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;
    m_wndRibbonBar.GetElementsByID(ID_DESIGN_DESIGN, arButtons);
    _ASSERT(arButtons.GetSize()==1);
	CMFCRibbonComboBox *c = dynamic_cast<CMFCRibbonComboBox*>(arButtons[0]);
    
	if (updateComboContent || c->GetCount()==0) {
		c->RemoveAllItems();
        CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
        ASSERT(pApp != NULL);
        if (pApp->m_Designs.size()) {
			c->AddItem("(use chart-defined)");
			c->AddItem("plain"); //place 'plain' as first
			int pos = 0;
            for (auto i=pApp->m_Designs.begin(); i!=pApp->m_Designs.end(); i++) 
                if (i->first != "plain" && i->second.is_full)
                    c->AddItem(i->first.c_str());
		} else 
			c->AddItem("-(only plain is available)-");
	}
	//restore the selection to the given style if it can be found
	int index = (current==NULL || current[0]==0) ? 0 : c->FindItem(current);
	bool ret = (index >= 0);
	c->SelectItem(ret?index:0);
    return ret;
}

void CMainFrame::OnDesignDesign() 
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;
    m_wndRibbonBar.GetElementsByID(ID_DESIGN_DESIGN, arButtons);
    _ASSERT(arButtons.GetSize()==1);
	CMFCRibbonComboBox *c = dynamic_cast<CMFCRibbonComboBox*>(arButtons[0]);
	int index = c->GetCurSel();
	CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(GetActiveDocument());
    if (pDoc && index>=0) 
        pDoc->ChangeDesign(index<=0 ? "" : c->GetItem(index));
}

void CMainFrame::OnUpdateDesignDesign(CCmdUI *pCmdUI)
{
    CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    pCmdUI->Enable(pApp->m_Designs.size()>1);
}


void CMainFrame::FillPageComboBox(int no_pages, int page)
{
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;
    m_wndRibbonBar.GetElementsByID(ID_DESIGN_PAGE, arButtons);
    _ASSERT(arButtons.GetSize()==1);
	CMFCRibbonEdit *c = dynamic_cast<CMFCRibbonEdit*>(arButtons[0]);

    CMFCToolBarEditBoxButton  *c2 = CMFCToolBarEditBoxButton::GetByCmd(ID_FULL_SCREEN_PAGE);

    if (no_pages == 1) {
        c->EnableSpinButtons(0, 0);
        c->SetEditText("1/1");
        if (c2) c2->GetEditBox()->SetWindowText("1/1");
    } else {
        c->EnableSpinButtons(0, no_pages);
        CString str;
        if (page==0) 
            str.Format("(all)/%d", no_pages);
        else 
            str.Format("%d/%d", page, no_pages);
        c->SetEditText(str);
        if (c2) c2->GetEditBox()->SetWindowText(str);
    }

    //Update the Copy Entire Chart button, as well
    arButtons.RemoveAll();
    m_wndRibbonBar.GetElementsByID(ID_EDIT_COPYENTIRECHART, arButtons);
    _ASSERT(arButtons.GetSize()==1);
    CMFCRibbonButton *button = dynamic_cast<CMFCRibbonButton*>(arButtons[0]);
    unsigned old_size = button->GetSubItems().GetSize();
    unsigned new_size = no_pages;
    if (new_size == 1) new_size = 0;
    if (old_size == new_size) return;
    while (old_size>new_size)
        button->RemoveSubItem(--old_size);
    while (old_size<new_size) {
        char buff[100];
        sprintf(buff, "Copy Page #%u", old_size+1);
        button->AddSubItem(new CMFCRibbonButton(ID_COPY_PAGE1+old_size, buff));
        old_size++;
    }
}


void CMainFrame::OnDesignPage()
{
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;
    m_wndRibbonBar.GetElementsByID(ID_DESIGN_PAGE, arButtons);
    _ASSERT(arButtons.GetSize()==1);
	CMFCRibbonEdit *c = dynamic_cast<CMFCRibbonEdit*>(arButtons[0]);
	CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(GetActiveDocument());
    if (!pDoc) return;
    unsigned page = pDoc->m_ChartShown.GetPage();
    const CString val = c->GetEditText();
    if (1==sscanf(val, "%u", &page)) 
        pDoc->ChangePage(page);
}

void CMainFrame::OnFullScreenPage()
{
	CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(GetActiveDocument());
    if (!pDoc) return;
    unsigned page = pDoc->m_ChartShown.GetPage();
    CString val = CMFCToolBarEditBoxButton::GetContentsAll(ID_FULL_SCREEN_PAGE);
    if (1==sscanf(val, "%u", &page)) 
        pDoc->ChangePage(page);
}

void CMainFrame::OnUpdateDesignPage(CCmdUI *pCmdUI)
{
	CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(GetActiveDocument());
    if (pDoc) 
        pCmdUI->Enable(pDoc->m_ChartShown.GetPages()>1);
}

void CMainFrame::OnDesignNextPage()
{
	CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(GetActiveDocument());
    if (pDoc) 
        pDoc->StepPage(+1);
}

void CMainFrame::OnDesignPrevPage()
{
	CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(GetActiveDocument());
    if (pDoc) 
        pDoc->StepPage(-1);
}

void CMainFrame::FillZoomComboBox(int zoom)
{
	CString text;
	text.Format("%d%%", zoom);
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;
    m_wndRibbonBar.GetElementsByID(ID_DESIGN_ZOOM, arButtons);
    _ASSERT(arButtons.GetSize()==1);
	CMFCRibbonComboBox *c = dynamic_cast<CMFCRibbonComboBox*>(arButtons[0]);
    static unsigned zoom_values[] = {400, 300, 200, 150, 100, 75, 50, 40, 30, 20, 10, 0};
    if (c->GetCount()!=sizeof(zoom_values)/sizeof(unsigned)-1) {
        for (int i=0; zoom_values[i]>0; i++) {
            CString text2;
            text2.Format("%u%%", zoom_values[i]);
            c->AddItem(text2, zoom_values[i]);
        }
    }
    c->SetEditText(text);
	CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(GetActiveDocument());
    if (pDoc)
        SetSplitSize(unsigned(pDoc->m_ChartShown.GetHeadingSize()*double(zoom)/100.));
}



void CMainFrame::OnViewZoomin()
{
	CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(GetActiveDocument());
    if (pDoc) 
        pDoc->SetZoom(int(pDoc->m_zoom*1.1));
}

void CMainFrame::OnViewZoomout()
{
	CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(GetActiveDocument());
    if (pDoc)
        pDoc->SetZoom(int(pDoc->m_zoom/1.1));
}

void CMainFrame::OnDesignZoom()
{
	CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(GetActiveDocument());
    if (!pDoc) return;
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;
    m_wndRibbonBar.GetElementsByID(ID_DESIGN_ZOOM, arButtons);
    _ASSERT(arButtons.GetSize()==1);
	CMFCRibbonComboBox *c = dynamic_cast<CMFCRibbonComboBox*>(arButtons[0]);
	CString text = c->GetEditText();
	if (!text.CompareNoCase("overview") || !text.CompareNoCase("auto")) {
		pDoc->ArrangeViews(CMscGenDoc::OVERVIEW);
	} else {
		pDoc->SetZoom(atoi(text));
	}
}

void CMainFrame::FillEmbeddedPanel(size_t size, double percent)
{
    m_wndRibbonBar.ShowContextCategories(ID_CONTEXT_EMBEDDEDOPTIONS);
    if (m_labelObjectSize) {
        CString sizeText = "Object size: ";
        if (size>=1536*1024) 
            sizeText.AppendFormat("%.2f MB", size/1024./1024.);
        else if (size>=1536)
            sizeText.AppendFormat("%.2f KB", size/1024.);
        else if (size>0)
            sizeText.AppendFormat("%u bytes", size);
        m_labelObjectSize->SetText(sizeText);
    }
    if (m_labelFallbackImage) {
        m_has_fallback_image = bool(percent);
        if (m_has_fallback_image) {
            CString fi;
            fi.AppendFormat("%u%% of chart area is fallback image", abs(int(floor(percent+0.5))));
            m_labelFallbackImage->SetText(fi);
        } else
            m_labelFallbackImage->SetText("No fallback images in this chart.");
    }
    if (m_labelFallbackImage || m_labelObjectSize) 
        m_wndRibbonBar.ForceRecalcLayout();
}

void CMainFrame::FillScale4Pagination() 
{
    if (!IsPrintPreview()) return;
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    if (pApp->m_iScale4Pagination<-2) return;
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;
    m_wndRibbonBar.GetElementsByID(ID_COMBO_SCALE, arButtons);
    if (arButtons.GetSize()==0) return;
    _ASSERT(arButtons.GetSize()==1);
 	CMFCRibbonEdit *c = dynamic_cast<CMFCRibbonEdit*>(arButtons[0]);
    if (!c) return;
    if (pApp->m_iScale4Pagination==-2) 
        c->SetEditText("Fit Page");
    else if (pApp->m_iScale4Pagination==-1) 
        c->SetEditText("Fit Width");
    else {
        CString val;
        val.Format("%u%%", pApp->m_iScale4Pagination);
        c->SetEditText(val);
    }
}

void CMainFrame::FillPageSize() 
{
    if (!IsPrintPreview()) return;
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;
    m_wndRibbonBar.GetElementsByID(ID_EDIT_PAGES, arButtons);
    if (arButtons.GetSize()==0) return;
    _ASSERT(arButtons.GetSize()==1);
    CMFCRibbonEdit *c = dynamic_cast<CMFCRibbonEdit*>(arButtons[0]);
    if (c)
        c->SetEditText(PageSizeInfo::ConvertPageSizeVerbose(pApp->m_pageSize));
}

void CMainFrame::FillMargins() 
{
    if (!IsPrintPreview()) return;
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    if (pApp->m_iScale4Pagination<-2) return;
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;
    m_wndRibbonBar.GetElementsByID(ID_EDIT_MARGIN_L, arButtons);
    if (arButtons.GetSize()==0) return;
    _ASSERT(arButtons.GetSize()==1);
    CMFCRibbonEdit *c = dynamic_cast<CMFCRibbonEdit*>(arButtons[0]);
    if (!c) return;
    CString val;
    val.Format("%lg", pApp->m_printer_usr_margins[0]/PT_PER_CM);
    c->SetEditText(val);

    m_wndRibbonBar.GetElementsByID(ID_EDIT_MARGIN_R, arButtons);
    _ASSERT(arButtons.GetSize()==1);
    c = dynamic_cast<CMFCRibbonEdit*>(arButtons[0]);
    val.Format("%lg", pApp->m_printer_usr_margins[1]/PT_PER_CM);
    c->SetEditText(val);

    m_wndRibbonBar.GetElementsByID(ID_EDIT_MARGIN_T, arButtons);
    _ASSERT(arButtons.GetSize()==1);
    c = dynamic_cast<CMFCRibbonEdit*>(arButtons[0]);
    val.Format("%lg", pApp->m_printer_usr_margins[2]/PT_PER_CM);
    c->SetEditText(val);

    m_wndRibbonBar.GetElementsByID(ID_EDIT_MARGIN_B, arButtons);
    _ASSERT(arButtons.GetSize()==1);
    c = dynamic_cast<CMFCRibbonEdit*>(arButtons[0]);
    val.Format("%lg", pApp->m_printer_usr_margins[3]/PT_PER_CM);
    c->SetEditText(val);
}

void CMainFrame::OnViewInternalEditor()
{
	m_ctrlEditor.ShowPane(!m_ctrlEditor.IsVisible(), FALSE, TRUE);
}

void CMainFrame::OnUpdateViewInternalEditor(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_ctrlEditor.IsVisible());
}


void CMainFrame::TriggerIfRibbonCategoryChange()
{
    const bool category_is_embedded = !strcmp("Embedded Object", m_wndRibbonBar.GetActiveCategory()->GetName());
    if (category_is_embedded == m_at_embedded_object_category) return;
    m_at_embedded_object_category = category_is_embedded;
    CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(GetActiveDocument());
    if (pDoc) 
        pDoc->OnChangeRibbonCategory(category_is_embedded);
}


void CMainFrame::OnUpdateButtonDefaultText(CCmdUI *pCmdUI)
{
    //This is just used to test if we have switched to "Embedded Object" category
    //This is only called if this category is visible
    TriggerIfRibbonCategoryChange();
}

void CMainFrame::OnUpdateEmbeddedoptionsFallbackRes(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_has_fallback_image);
    //This is just used to test if we have switched to "Embedded Object" category
    //This is only called if this category is visible
    TriggerIfRibbonCategoryChange();
}
