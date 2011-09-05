// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "Msc-generator.h"
#include "OutputView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputViewBar::COutputViewBar()
{
}

COutputViewBar::~COutputViewBar()
{
}

BEGIN_MESSAGE_MAP(COutputViewBar, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

int COutputViewBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_Font.CreateFont(16, 0, 0, 0, FW_NORMAL, false, false, false, ANSI_CHARSET, 
		              OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
					  FIXED_PITCH|FF_MODERN, NULL);

	CRect rectClient(0, 0, lpCreateStruct->cx, lpCreateStruct->cy);

	// Create output pane:
	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | LBS_NOTIFY;

	if (!m_wndOutput.Create(dwStyle, rectClient, this, IDC_OUTPUT_LIST))
	{
		TRACE0("Failed to create output window\n");
		return -1;      // fail to create
	}

	m_wndOutput.SetFont(&m_Font);

	return 0;
}

void COutputViewBar::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	CRect rc;
	GetClientRect(rc);

	m_wndOutput.SetWindowPos(NULL, rc.left + 1, rc.top + 1, rc.Width() - 2, rc.Height() - 2, SWP_NOACTIVATE | SWP_NOZORDER );

	AdjusrHorzScroll(m_wndOutput);
}

void COutputViewBar::AdjusrHorzScroll(CListBox& wndListBox)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&m_Font);

	int cxExtentMax = 0;

	for (int i = 0; i < wndListBox.GetCount(); i ++)
	{
		CString strItem;
		wndListBox.GetText(i, strItem);

		if (cxExtentMax < dc.GetTextExtent(strItem).cx)
			cxExtentMax = dc.GetTextExtent(strItem).cx;
	}

	wndListBox.SetHorizontalExtent(cxExtentMax);
	dc.SelectObject(pOldFont);
}

/////////////////////////////////////////////////////////////////////////////
// COutputList1

COutputList::COutputList()
{
}

COutputList::~COutputList()
{
}

BEGIN_MESSAGE_MAP(COutputList, CListBox)
	ON_WM_CONTEXTMENU()
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList message handlers

void COutputViewBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndOutput.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void COutputViewBar::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndOutput.SetFocus();
}


void COutputViewBar::ShowCompilationErrors(const CDrawingChartData &chart) 
{
    if (!::IsWindow(m_wndOutput)) return;
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	m_wndOutput.ResetContent();
    compilation_errors.clear();
	unsigned num = chart.GetErrorNum(pApp->m_Warnings);
    error_pos.resize(num);
	for (unsigned i=0; i<num; i++) {
        compilation_errors.push_back(chart.GetErrorText(i, pApp->m_Warnings));
        m_wndOutput.AddString(*compilation_errors.rbegin());
        if (chart.IsErrorInFile(i, pApp->m_Warnings)) {
            error_pos[i].first = chart.GetErrorLine(i, pApp->m_Warnings);
            error_pos[i].second = chart.GetErrorCol(i, pApp->m_Warnings);
        } else {
            error_pos[i].first = -1;
        }
    }
    //turn off only if m_bShowCshErrorsInWindow is false && no errors
    ShowPane(num>0 || (pApp->m_bShowCshErrorsInWindow && IsVisible()), false, true);
}


void COutputViewBar::ShowCshErrors(const std::list<CString> &errors, 
                                   const std::vector<std::pair<int, int>> &err_pos) 
{
    if (!::IsWindow(m_wndOutput)) return;
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    m_wndOutput.SetRedraw(false);
	m_wndOutput.ResetContent();
	for (auto i=compilation_errors.begin(); !(i==compilation_errors.end()); i++) 
        m_wndOutput.AddString(*i);
    for (auto i=errors.begin(); i!=errors.end(); i++) 
        m_wndOutput.AddString(*i);
    error_pos.resize(compilation_errors.size());
    error_pos.insert(error_pos.end(), err_pos.begin(), err_pos.end());
    //ShowPane(true, true, false);  //do not do this! If user wants them, let him turn errors on
    m_wndOutput.SetRedraw(true);
    //turn off only if m_bShowCshErrorsInWindow is false && no errors
	ShowPane(error_pos.size()>0 || (pApp->m_bShowCshErrorsInWindow && IsVisible()), false, true);
}
