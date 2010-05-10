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

CCshRichEditCtrl::CCshRichEditCtrl()
{
	m_bCshUpdateInProgress = false;
	m_UndoList.push_back(CEditorUndoRecord());
	m_itrCurrent = --m_UndoList.end();
}

void CCshRichEditCtrl::UpdateText(const char *text)
{
	CEditorUndoRecord undoRec;
	undoRec.text = text;
	EnsureCRLF(undoRec.text);
	SetRedraw(false);
	GetSel(undoRec.pos);
	SetWindowText(undoRec.text);
	SetSel(undoRec.pos);
	GetSel(undoRec.pos);
	UpdateCsh();
	m_UndoList.erase(++m_itrCurrent, m_UndoList.end());
	m_UndoList.push_back(undoRec);
	m_itrCurrent = --m_UndoList.end();
}

void CCshRichEditCtrl::UpdateCsh(bool force)
{
	if (m_bCshUpdateInProgress) return;
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	//Check if Color Syntax Highlghting is enabled
	if (!pApp->m_bCsh && !force) return;
	CString text;
	GetWindowText(text);
	RemoveCRLF(text);
	CChartData data(text);
	const MscCshListType v(data.GetCsh());

	SetRedraw(false);
	//long eventMask = GetEventMask();
	//SetEventMask(0);
	m_bCshUpdateInProgress = true;

	POINT scroll_pos;
	::SendMessage(m_hWnd, EM_GETSCROLLPOS, 0, (LPARAM)&scroll_pos);
	CHARRANGE cr;
	GetSel(cr);
	CHARFORMAT *const scheme = pApp->m_csh_cf[pApp->m_nCshScheme];
	const DWORD effects = scheme[COLOR_NORMAL].dwEffects;
	const COLORREF color = scheme[COLOR_NORMAL].crTextColor;

	SetSel(0,-1);
	SetSelectionCharFormat(scheme[COLOR_NORMAL]);
	if (pApp->m_bCsh)
		for (MscCshListType::const_iterator i=v.begin(); i!=v.end(); i++) 
			if (scheme[i->color].dwEffects != effects || scheme[i->color].crTextColor != color) {
				SetSel(i->first_pos-1, i->last_pos);
				SetSelectionCharFormat(scheme[i->color]);
			}
	SetSel(cr);
	::SendMessage(m_hWnd, EM_SETSCROLLPOS, 0, (LPARAM)&scroll_pos);

	m_bCshUpdateInProgress = false;
	SetRedraw(true);
	Invalidate();
	//SetEventMask(eventMask);
}

void CCshRichEditCtrl::StoreUndo()
{
	CEditorUndoRecord undoRec;
	GetWindowText(undoRec.text);
	if (m_itrCurrent->text == undoRec.text) return;
	m_UndoList.erase(++m_itrCurrent, m_UndoList.end());
	GetSel(undoRec.pos);
	m_UndoList.push_back(undoRec);
	m_itrCurrent = --m_UndoList.end();
}

void CCshRichEditCtrl::ClearUndo()
{
	m_UndoList.clear();
	m_UndoList.push_back(CEditorUndoRecord());
	m_itrCurrent = --m_UndoList.end();
	GetWindowText(m_itrCurrent->text);
	GetSel(m_itrCurrent->pos);
}

void CCshRichEditCtrl::MyUndo()
{
	if (m_UndoList.begin() == m_itrCurrent) return;
	m_itrCurrent--;
	SetRedraw(false);
	SetWindowText(m_itrCurrent->text);
	SetSel(m_itrCurrent->pos);
}

void CCshRichEditCtrl::MyRedo()
{
	if (--m_UndoList.end() == m_itrCurrent) return;
	m_itrCurrent++;
	SetRedraw(false);
	SetWindowText(m_itrCurrent->text);
	SetSel(m_itrCurrent->pos);
}


/////////////////////////////////////////////////////////////////////////////
// COutputBar

CEditorBar::CEditorBar()
{
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

	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	CFrameWnd *pMainWnd = dynamic_cast<CFrameWnd*>(pApp->GetMainWnd());
	ASSERT(pMainWnd!=NULL);
	if (pMainWnd->GetActiveView() != NULL) {
		CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(pMainWnd->GetActiveView()->GetDocument());
		if (pDoc != NULL && pDoc->IsExternalEditorRunning())
			SetReadOnly();
	}

	return 0;
}

#define READONLY_STRING " (Read-only)"
void CEditorBar::SetReadOnly(bool readonly)
{
	if (!m_wndEditor.SetReadOnly(readonly)) return;
	CString text;
	GetWindowText(text);
	if (text.Right(12).CompareNoCase(READONLY_STRING)==0) {
		if (!readonly)
			SetWindowText(text.Left(text.GetLength()-12));
	} else if (readonly)
		SetWindowText(text + READONLY_STRING);
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


BOOL CEditorBar::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UINT nID = LOWORD(wParam);
	HWND hWndCtrl = (HWND)lParam;
	int nCode = HIWORD(wParam);

	if (hWndCtrl != m_wndEditor) return CDockablePane::OnCommand(wParam, lParam);
	if (nCode != EN_CHANGE) return CDockablePane::OnCommand(wParam, lParam);
	if (!m_wndEditor.IsCshUpdateInProgress()) {
		m_wndEditor.UpdateCsh();
		m_wndEditor.StoreUndo();
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
		if (pDoc != NULL)
			pDoc->OnInternalEditorSelChange();
	}
	*result = 0;
}
