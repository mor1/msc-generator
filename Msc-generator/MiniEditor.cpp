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
#include "MscGenDoc.h"
#include "ChartData.h"
#include "MiniEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

CEditorBar::CEditorBar()
{
	m_bCshUpdateInProgress = false;
	m_modified = false;
}

CEditorBar::~CEditorBar()
{
}

BEGIN_MESSAGE_MAP(CEditorBar, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_NOTIFY(EN_SELCHANGE, IDC_INTERNAL_EDITOR, OnSelChange)
END_MESSAGE_MAP()

int CEditorBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	//m_Font.CreateStockObject(DEFAULT_GUI_FONT);
	m_Font.CreateFont(16, 0, 0, 0, FW_NORMAL, false, false, false, ANSI_CHARSET, 
		              OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
					  FIXED_PITCH|FF_MODERN, NULL);

	CRect rectClient(0, 0, lpCreateStruct->cx, lpCreateStruct->cy);

	// Create output pane:
	const DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | 
						  ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_NOHIDESEL;

	if (!m_wndEditor.Create(dwStyle, rectClient, this, IDC_INTERNAL_EDITOR))
	{
		TRACE0("Failed to create output window\n");
		return -1;      // fail to create
	}
	m_wndEditor.SetTextMode(TM_SINGLECODEPAGE | TM_MULTILEVELUNDO | TM_RICHTEXT);
	m_wndEditor.SetFont(&m_Font);
	m_wndEditor.SetFocus();
	m_wndEditor.SetEventMask(m_wndEditor.GetEventMask() | ENM_CHANGE | ENM_SELCHANGE);

	return 0;
}

void CEditorBar::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	CRect rc;
	GetClientRect(rc);

	m_wndEditor.SetWindowPos(NULL, rc.left + 1, rc.top + 1, rc.Width() - 2, rc.Height() - 2, SWP_NOACTIVATE | SWP_NOZORDER );
}

/////////////////////////////////////////////////////////////////////////////
// CEditorBar message handlers

void CEditorBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndEditor.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CEditorBar::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndEditor.SetFocus();
}
void CEditorBar::UpdateText(const char *text)
{
	CString t(text);
	EnsureCRLF(t);
	m_wndEditor.SetRedraw(false);
	m_wndEditor.SetWindowText(t);
	m_modified = false;
	UpdateCsh();
	m_wndEditor.SetRedraw(true);
}

void CEditorBar::UpdateCsh(bool force)
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	//Check if Color Syntax Highlghting is enabled
	if (!pApp->m_bCsh && !force) return;
	CString text;
	m_wndEditor.GetWindowText(text);
	RemoveCRLF(text);
	CChartData data(text);
	const MscCshListType v(data.GetCsh());

	m_wndEditor.SetRedraw(false);
	//long eventMask = m_wndEditor.GetEventMask();
	//m_wndEditor.SetEventMask(0);
	m_bCshUpdateInProgress = true;

	POINT scroll_pos;
	::SendMessage(m_wndEditor.m_hWnd, EM_GETSCROLLPOS, 0, (LPARAM)&scroll_pos);
	CHARRANGE cr;
	m_wndEditor.GetSel(cr);
	CHARFORMAT *const scheme = pApp->m_csh_cf[pApp->m_nCshScheme];
	const DWORD effects = scheme[COLOR_NORMAL].dwEffects;
	const COLORREF color = scheme[COLOR_NORMAL].crTextColor;

	m_wndEditor.SetSel(0,-1);
	m_wndEditor.SetSelectionCharFormat(scheme[COLOR_NORMAL]);
	if (pApp->m_bCsh)
		for (MscCshListType::const_iterator i=v.begin(); i!=v.end(); i++) 
			if (scheme[i->color].dwEffects != effects || scheme[i->color].crTextColor != color) {
				m_wndEditor.SetSel(i->first_pos-1, i->last_pos);
				m_wndEditor.SetSelectionCharFormat(scheme[i->color]);
			}
	m_wndEditor.SetSel(cr);
	::SendMessage(m_wndEditor.m_hWnd, EM_SETSCROLLPOS, 0, (LPARAM)&scroll_pos);

	m_bCshUpdateInProgress = false;
	m_wndEditor.SetRedraw(true);
	m_wndEditor.Invalidate();
	//m_wndEditor.SetEventMask(eventMask);
}

BOOL CEditorBar::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UINT nID = LOWORD(wParam);
	HWND hWndCtrl = (HWND)lParam;
	int nCode = HIWORD(wParam);

	if (hWndCtrl != m_wndEditor) return CDockablePane::OnCommand(wParam, lParam);
	if (nCode != EN_CHANGE) return CDockablePane::OnCommand(wParam, lParam);
	if (!m_bCshUpdateInProgress) {
		UpdateCsh();
		m_modified = true;
		CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
		ASSERT(pApp != NULL);
		CFrameWnd *pMainWnd = dynamic_cast<CFrameWnd*>(pApp->GetMainWnd());
		ASSERT(pMainWnd!=NULL);
		if (pMainWnd->GetActiveView() != NULL) {
			CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(pMainWnd->GetActiveView()->GetDocument());
			if (pDoc != NULL);
				pDoc->OnInternalEditorChange();
		}
	}
	return TRUE;
}

void CEditorBar::OnSelChange(NMHDR *pNotifyStruct, LRESULT *result) 
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	CFrameWnd *pMainWnd = dynamic_cast<CFrameWnd*>(pApp->GetMainWnd());
	ASSERT(pMainWnd!=NULL);
	if (pMainWnd->GetActiveView() != NULL) {
		CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(pMainWnd->GetActiveView()->GetDocument());
		if (pDoc != NULL);
			pDoc->OnInternalEditorSelChange();
	}
	*result = 0;
}

BOOL CEditorBar::ShowWindow(int nCmdShow)
{
	return CDockablePane::ShowWindow(nCmdShow);
}