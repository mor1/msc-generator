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


// IpFrame.cpp : implementation of the CInPlaceFrame class
//

#include "stdafx.h"
#include "Msc-generator.h"
#include "MscGenDoc.h"
#include "MscGenView.h"

#include "IpFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void CDesignBar::OnReset ()
{
	//CMFCToolBarComboBoxButton pageButton (ID_DESIGN_PAGE,
	//	GetCmdMgr ()->GetCmdImage (ID_DESIGN_PAGE, FALSE), CBS_DROPDOWNLIST, 50);
	CMFCToolBarComboBoxButton pageButton (ID_DESIGN_PAGE, 0, CBS_DROPDOWNLIST, 50);
	InsertSeparator();
	InsertButton(pageButton);

	//CMFCToolBarComboBoxButton designButton (ID_DESIGN_DESIGN,
	//	GetCmdMgr ()->GetCmdImage (ID_DESIGN_DESIGN, FALSE));
	CMFCToolBarComboBoxButton designButton (ID_DESIGN_DESIGN, 0);
	InsertSeparator();
	InsertButton(designButton);
}

// CInPlaceFrame

IMPLEMENT_DYNCREATE(CInPlaceFrame, COleIPFrameWndEx)

BEGIN_MESSAGE_MAP(CInPlaceFrame, COleIPFrameWndEx)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_VIEW_TOOLBAR, OnViewToolBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, OnUpdateViewToolBar)
	ON_COMMAND(ID_VIEW_OUTPUT, OnViewOutput)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUT, OnUpdateViewOutput)
	ON_COMMAND(ID_VIEW_EDITOR, OnViewEditor)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EDITOR, OnUpdateViewEditor)
END_MESSAGE_MAP()


// CInPlaceFrame construction/destruction

CInPlaceFrame::CInPlaceFrame()
{
}

CInPlaceFrame::~CInPlaceFrame()
{
}

int CInPlaceFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleIPFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// CResizeBar implements in-place resizing.
	if (!m_wndResizeBar.Create(this))
	{
		TRACE0("Failed to create resize bar\n");
		return -1;      // fail to create
	}

	// By default, it is a good idea to register a drop-target that does
	//  nothing with your frame window.  This prevents drops from
	//  "falling through" to a container that supports drag-drop.
	m_dropTarget.Register(this);

	return 0;
}

// OnCreateControlBars is called by the framework to create control bars on the
//  container application's windows.  pWndFrame is the top level frame window of
//  the container and is always non-NULL.  pWndDoc is the doc level frame window
//  and will be NULL when the container is an SDI application.  A server
//  application can place MFC control bars on either window.
BOOL CInPlaceFrame::OnCreateControlBars(CFrameWnd* pWndFrame, CFrameWnd* pWndDoc)
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);

	//Does nothing, but ensures that m_pMainFrame is of type COleCntrFrameWndEx, 
	//so that we can dock MFC extension pack panes with DockPane below
	COleIPFrameWndEx::OnCreateControlBars(pWndFrame, pWndDoc);

	//The previous call may have deleted pWndFrame, so use m_pMainFrame from here on
	COleCntrFrameWndEx* pFrame = DYNAMIC_DOWNCAST (COleCntrFrameWndEx, m_pMainFrame);
	if (pFrame == NULL) 
	{
		return FALSE;
	}
	ASSERT_VALID (pFrame);

	// Create toolbar on client's frame window
	if (!m_wndToolBar.CreateEx(pFrame, TBSTYLE_FLAT,WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC | CBRS_BORDER_3D,
		                       CRect(1,1,1,1), IDR_TOOLBAR_SRVR_INPLACE) ||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR_SRVR_INPLACE))
	{
		TRACE0("Failed to create toolbar\n");
		return FALSE;
	}
	CString strToolBarName;
	bool bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	if (!m_wndOutputView.Create(_T("Errors and Warnings"), pFrame, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUT, 
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create output bar\n");
		return FALSE;      // fail to create
	}
	pApp->m_pWndOutputView = &m_wndOutputView;

	//Before creating the editor, query pFrame size
	CRect r;
	pFrame->GetClientRect(&r);
	if (r.right < 500) r.right = 100;
	else if (r.right > 2000) r.right = 400;
	else r.right /= 5;
	if (r.bottom < 500) r.bottom = 100;
	else if (r.bottom > 2000) r.bottom = 400;
	else r.bottom /= 5;

	if (!m_ctrlEditor.Create(_T("Internal Editor"), pFrame, r, TRUE, ID_VIEW_EDITOR, 
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create editor\n");
		return FALSE;      // fail to create
	}
	pApp->m_pWndEditor = &m_ctrlEditor;
	
	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, "Customize...", ID_VIEW_TOOLBAR);

	//Make bars dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndOutputView.EnableDocking(CBRS_ALIGN_ANY);
	m_ctrlEditor.EnableDocking(CBRS_ALIGN_ANY);
	pFrame->EnableDocking(CBRS_ALIGN_ANY);
	pFrame->DockPane(&m_wndToolBar);
	pFrame->DockPane(&m_wndOutputView);
	pFrame->DockPane(&m_ctrlEditor);
	m_wndOutputView.ShowPane(false,false,false);

	// Set owner to this window, so messages are delivered to correct app
	m_wndToolBar.SetOwner(this);
	m_wndOutputView.SetOwner(this);
	m_ctrlEditor.SetOwner(this);

	CMscGenDoc *pDoc = static_cast<CMscGenDoc *>(GetActiveDocument());
	ASSERT(pDoc != NULL);
	ASSERT_KINDOF(CMscGenDoc, pDoc);

	pApp->FillDesignDesignCombo(pDoc->m_itrEditing->GetDesign(), true);
	pApp->FillDesignPageCombo(pDoc->m_ChartShown.GetPages(), pDoc->m_ChartShown.GetPage());
	pDoc->SetZoom(); //Adjust combo

	//Set the text of the chart in the editor
	m_ctrlEditor.m_ctrlEditor.UpdateText(pDoc->m_itrEditing->GetText(), pDoc->m_itrEditing->m_sel, true);

	//Show error window only if there are errors, actually. At this point we have pDoc all set up.
	m_wndOutputView.ShowPane(pDoc->m_ChartShown.GetErrorNum(pApp->m_Warnings), TRUE, FALSE); 
	return TRUE;
}

void CInPlaceFrame::OnDestroy()
{
    m_wndToolBar.DestroyWindow();
	CMscGenDoc *pDoc = static_cast<CMscGenDoc *>(GetActiveDocument());
	if (pDoc) {
		ASSERT_KINDOF(CMscGenDoc, pDoc);
		pDoc->m_ExternalEditor.Stop(STOPEDITOR_WAIT);
	}
    COleIPFrameWndEx::OnDestroy();
}


// CInPlaceFrame diagnostics

#ifdef _DEBUG
void CInPlaceFrame::AssertValid() const
{
	COleIPFrameWndEx::AssertValid();
}

void CInPlaceFrame::Dump(CDumpContext& dc) const
{
	COleIPFrameWndEx::Dump(dc);
}
#endif //_DEBUG

void CInPlaceFrame::OnViewToolBar()
{
	m_wndToolBar.ShowPane(!m_wndToolBar.IsVisible(), FALSE, FALSE);
}

void CInPlaceFrame::OnUpdateViewToolBar(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndToolBar.IsVisible());
}

void CInPlaceFrame::OnViewOutput()
{
	m_wndOutputView.ShowPane(!m_wndOutputView.IsVisible(), FALSE, FALSE);
}

void CInPlaceFrame::OnUpdateViewOutput(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndOutputView.IsVisible());
}

void CInPlaceFrame::OnViewEditor()
{
	m_ctrlEditor.ShowPane(!m_ctrlEditor.IsVisible(), FALSE, FALSE);
}

void CInPlaceFrame::OnUpdateViewEditor(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_ctrlEditor.IsVisible());
}

