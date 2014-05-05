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
// COutputList1

BOOL COutputList::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    if (message==WM_COMMAND) 
        switch (HIWORD(wParam)) {
        case LBN_DBLCLK:
            CWnd *pParent = GetParent();
            _ASSERT(pParent);
            if (pParent) {
                COutputViewBar *pBar = dynamic_cast<COutputViewBar*>(pParent->GetParent());
                CMscGenDoc *pDoc = GetMscGenDocument();
                _ASSERT(pBar && pDoc);
                if (pBar && pDoc) {
                    int line, col;
                    pBar->GetCurrentErrorLine(line, col);
                    pDoc->OnErrorSelected(line, col);
                    return true;
                }
            }
    }
    return CListBox::OnChildNotify(message, wParam, lParam, pResult); 
}

/////////////////////////////////////////////////////////////////////////////
// COutputBar

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

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create tabs window:
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("Failed to create output tab\n");
		return -1;      // fail to create
	}
    m_wndTabs.HideNoTabs();

	// Create error lists:
	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | 
                          WS_HSCROLL | WS_VSCROLL | LBS_NOTIFY;

	if (!m_wndOutput.Create(dwStyle, rectDummy, &m_wndTabs, IDC_ERROR_LIST) ||
        !m_wndOutputHints.Create(dwStyle, rectDummy, &m_wndTabs, IDC_HINT_ERROR_LIST))
	{
		TRACE0("Failed to create error lists\n");
		return -1;      // fail to create
	}

	//m_Font.CreateFont(16, 0, 0, 0, FW_NORMAL, false, false, false, ANSI_CHARSET, 
	//	              OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	//				  FIXED_PITCH|FF_MODERN, NULL);
	m_wndOutput.SetFont(&afxGlobalData.fontRegular);
	m_wndOutputHints.SetFont(&afxGlobalData.fontRegular);

	m_wndTabs.AddTab(&m_wndOutput, "Compilation Errors");
	m_wndTabs.AddTab(&m_wndOutputHints, "Error Hints");

	return 0;
}

void COutputViewBar::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

	AdjusrHorzScroll(m_wndOutput);
	AdjusrHorzScroll(m_wndOutputHints);
}

void COutputViewBar::AdjusrHorzScroll(CListBox& wndListBox)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

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
    if (error_pos.size()>0) 
        m_wndTabs.SetActiveTab(0);
    //turn off only if m_bShowCshErrorsInWindow is false && no errors
    ShowPane(error_pos.size()>0 || (pApp->m_bShowCshErrorsInWindow && IsVisible()), false, true);
}


void COutputViewBar::ShowCshErrors(const std::list<CString> &errors, 
                                   const std::vector<std::pair<int, int>> &err_pos) 
{
    if (!::IsWindow(m_wndOutputHints)) return;
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    const bool had_errors_before = error_pos_hint.size()>0;
    m_wndOutputHints.SetRedraw(false);
	m_wndOutputHints.ResetContent();
    for (auto i=errors.begin(); i!=errors.end(); i++) 
        m_wndOutputHints.AddString(*i);
    error_pos_hint = err_pos;
    //ShowPane(true, true, false);  //do not do this! If user wants them, let him turn errors on
    m_wndOutputHints.SetRedraw(true);
    //turn off only if m_bShowCshErrorsInWindow is false && no errors
    if (error_pos.size()==0 && error_pos_hint.size()>0) 
        m_wndTabs.SetActiveTab(1); //bring us forth if new syntax errors appear and there are no comp errors
    if (error_pos.size()>0 || (pApp->m_bShowCshErrorsInWindow && error_pos_hint.size()>0))
        ShowPane(true, false, true);
}

bool COutputViewBar::NextError(bool next)
{
    const int tab = m_wndTabs.GetActiveTab();
    if (tab<0) return false;
    COutputList *const ol = tab==0 ? &m_wndOutput : &m_wndOutputHints;
	int maxsel = ol->GetCount();
	if (maxsel == LB_ERR || maxsel==0) return false; //no error
	int cursel = ol->GetCurSel();
	cursel = (cursel + (next ? 1 : -1) + maxsel) % maxsel;
	ol->SetCurSel(cursel);

    const std::vector<std::pair<int, int>> &e = tab==0 ? error_pos : error_pos_hint;
    //exit if we do not know the location of the error
    if (cursel >= int(e.size()) || cursel<0) return false;
    if (e[cursel].first < 0) return false;
    return true;
}

bool COutputViewBar::GetCurrentErrorLine(int &line, int &col)
{
    const int tab = m_wndTabs.GetActiveTab();
    if (tab<0) return false;
    COutputList *const ol = tab==0 ? &m_wndOutput : &m_wndOutputHints;
	int maxsel = ol->GetCount();
	if (maxsel == LB_ERR || maxsel==0) return false; //no error
	int cursel = ol->GetCurSel();

    const std::vector<std::pair<int, int>> &e = tab==0 ? error_pos : error_pos_hint;
    //exit if we do not know the location of the error
    if (cursel >= int(e.size()) || cursel<0) return false;
    if (e[cursel].first < 0) return false;

    //Jump to that pos in editor
    line = e[cursel].first;
    col = e[cursel].second;
    return true;
}



