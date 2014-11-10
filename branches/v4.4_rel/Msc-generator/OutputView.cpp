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

/** @file OutputView.cpp The implementation of the view showing error lists.
* @ingroup Msc_generator_files */

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

/** Called by the framework at events reflected back to us.
 * We handle double-clicks and notify the document that an error was selected.*/
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

/** Create the windows objects.*/
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

	m_Font.CreateFont(16, 0, 0, 0, FW_NORMAL, false, false, false, ANSI_CHARSET, 
		              OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
					  FIXED_PITCH|FF_MODERN, NULL);
    //m_wndOutput.SetFont(&afxGlobalData.fontRegular);
    //m_wndOutputHints.SetFont(&afxGlobalData.fontRegular);
    m_wndOutput.SetFont(&m_Font);
    m_wndOutputHints.SetFont(&m_Font);

	m_wndTabs.AddTab(&m_wndOutput, "Compilation Errors");
	m_wndTabs.AddTab(&m_wndOutputHints, "Error Hints");

	return 0;
}

/** The framework calls this when the pane changes size.
 * We adjust scroll sizes and the size of the tab control.*/
void COutputViewBar::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

	AdjusrHorzScroll(m_wndOutput);
	AdjusrHorzScroll(m_wndOutputHints);
}

/** Set the horizontal size of the virtual canvas in wndListBox.*/
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

/** Called by the framework to repaint.
 * We draw a bit of a border.*/
void COutputViewBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndOutput.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

/** Called by the framework when we get the focus.
 * We pass the focus to the compilation list.*/
void COutputViewBar::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndOutput.SetFocus();
}


/**Copy the compilation errors from 'chart' to the compilation error listbox.
 * Also store the line/col values so we can jump there.
 * We also activate the compilation error pane, if there were errors.
 * If no errors we entirely hide us.*/
void COutputViewBar::ShowCompilationErrors(const CDrawingChartData &chart) 
{
    if (!::IsWindow(m_wndOutput)) return;
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
    if (!pApp) return;
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


/**Copy the hinted errors from 'errors' and 'err_pos' to the compilation error listbox.
* Also store the line/col values so we can jump there.
* We also activate the hints error pane, if there were errors.
* If no errors we entirely hide us.*/
void COutputViewBar::ShowCshErrors(const std::list<CString> &errors,
                                   const std::vector<std::pair<int, int>> &err_pos) 
{
    if (!::IsWindow(m_wndOutputHints)) return;
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
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
    if (error_pos.size()>0 || (pApp && pApp->m_bShowCshErrorsInWindow && error_pos_hint.size()>0))
        ShowPane(true, false, true);
}

/** We jump to the next/prev error (in the currently showing list).
 * @param [in] next True if we shall step forward, false if backward.
 * @returns true if we ended up at a new error, which has a valid line number.*/
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

/** Gets the line and column of the current error.
 * @param [out] line Returns the line number.
 * @param [out] col Returns the column number
 * @returns true if we have returned a valid line/col. */
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



