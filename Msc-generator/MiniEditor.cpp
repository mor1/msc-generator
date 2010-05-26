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
#include "msc.h"
#include "MiniEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CCshRichEditCtrl, CRichEditCtrl)
	ON_WM_MOUSEWHEEL() 
END_MESSAGE_MAP()


CCshRichEditCtrl::CCshRichEditCtrl()
{
	m_bCshUpdateInProgress = false;
}

#define TABSTOP 4
//We assume no TAB characters...
BOOL CCshRichEditCtrl::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message != WM_KEYDOWN) return CRichEditCtrl::PreTranslateMessage(pMsg);
	if (pMsg->wParam == VK_TAB) {
		bool shift = GetKeyState(VK_SHIFT) & 0x8000;
		long lStart, lEnd;
		GetSel(lStart, lEnd);
		//if no selection, just insert/remove spaces up to the next/prev tab stop
		if (lStart == lEnd) {
			int line, col;
			ConvertPosToLineCol(lStart, line, col);
			if (shift) { //Shift+TAB
				CString strLine;
				int nLineLength = LineLength(line);
				int nBuffLen = std::max<int>(sizeof(int), nLineLength);
				GetLine(line, strLine.GetBufferSetLength(nBuffLen + 1), nBuffLen);
				int del = 0;
				while (del < TABSTOP && col > del && (strLine[col-del-1] == ' ' || strLine[col-del-1] == '\t')) del++;
				if (del>0) {
					SetSel(lStart-del, lEnd);
					ReplaceSel("", FALSE);
				}
			} else { //regular TAB
				col = TABSTOP - col%TABSTOP;
				char spaces[] = "                        ";
				spaces[col] = 0;
				ReplaceSel(spaces, FALSE);
			}
		} else { //OK, a full selection, do identation
			if (m_bCshUpdateInProgress) return TRUE; //If this happens we cannot ident
			NotifyDocumentOfChange(); //We indicate content change, yet there is none. This stores current selection into the undo buffer
			//If there is a selection perform identation
			int startLine = LineFromChar(lStart);
			int endLine, endCol;
			ConvertPosToLineCol(lEnd, endLine, endCol);
			//If last line is not really selected, skip it
			if (endCol==0) {
				endLine--;
			}
			m_bCshUpdateInProgress = true;
			SetRedraw(false);
			char spaces[] = "                        ";
			spaces[TABSTOP] = 0;
			for (int line = startLine; line<=endLine; line++) {
				long index = LineIndex(line);
				if (shift) {
					SetSel(index,index+TABSTOP);
					GetSelText(spaces);
					int del = 0;
					while (del<TABSTOP && (spaces[del] == ' ' || spaces[del] == '\t')) del++;
					if (del != TABSTOP)
						SetSel(index,index+del);
					ReplaceSel("", FALSE);
					lEnd -= del;
					if (index < lStart) { //can happen only at the first line, if selection does not start at the beginning of line
						if (lStart < index+del) 
							lStart = index; //beginning of line
						else 
							lStart -= del;
					}
				} else {
					SetSel(index,index);
					ReplaceSel(spaces);
					lEnd += TABSTOP;
					if (index < lStart)  //can happen only at the first line, if selection does not start at the beginning of line
						lStart += TABSTOP;
				}
			}
			SetSel(lStart, lEnd);
			m_bCshUpdateInProgress = false;
			UpdateCsh(); //will set redraw back to true
			NotifyDocumentOfChange();
		}
		return TRUE;
	}
	if (pMsg->wParam == VK_RETURN) {
		long lStart, lEnd;
		GetSel(lStart, lEnd);
		int nLine = LineFromChar(lStart);
		//Find a line (current or above) with a non-whitespace char
		//ident to that char
		do {
			CString strLine;
			int nLineLength = LineLength(nLine);
			nLineLength = GetLine(nLine, strLine.GetBufferSetLength(nLineLength + 1), nLineLength);
			int i=0;
			while (i<nLineLength && (strLine[i]==' ' || strLine[i]=='\t')) i++;
			if (i<nLineLength) {
				strLine.Truncate(i);
				strLine.Insert(0, '\n');
				ReplaceSel(strLine, FALSE);
				return TRUE;
			}
			nLine--;
		} while (nLine>=0);
		//There were only empty lines above, just insert a return
		return FALSE;
	}
	if (pMsg->wParam == VK_BACK) {
		long lStart, lEnd;
		GetSel(lStart, lEnd);
		int nLine, nCol;
		ConvertPosToLineCol(lStart, nLine, nCol);
		if (nCol==0) return FALSE;
		CString strLine;
		int nLineLength = LineLength(nLine);
		int nBuffLen = std::max<int>(sizeof(int), nLineLength);
		GetLine(nLine, strLine.GetBufferSetLength(nBuffLen + 1), nBuffLen);
		int i=0;
		while (i<nLineLength && (strLine[i]==' ' || strLine[i]=='\t')) i++;
		//If there are non-whitespace chars before current position in line, do regular stuff
		if (i<nCol) return FALSE;
		nCol = (nCol-1)%TABSTOP + 1; //number of chars to remove 1..TABSTOP
		//Extend beginning of selection by this many char
		SetSel(lStart-nCol, lEnd);
		return FALSE;
	}
	//Do nothing for escape
	if (pMsg->wParam == VK_ESCAPE) {
		NotifyDocumentOfChange(); //This kills tracking mode and stores the selection onto the undo buffer
		return TRUE;
	}
	return FALSE;
}

void CCshRichEditCtrl::UpdateText(const char *text, CHARRANGE &cr, bool preventNotification)
{
	CEditorUndoRecord undoRec;
	undoRec.text = text;
	EnsureCRLF(undoRec.text);
	SetRedraw(false);
	m_bCshUpdateInProgress = preventNotification; 
	SetWindowText(undoRec.text);
	m_bCshUpdateInProgress = false;
	if (preventNotification) UpdateCsh(); //This was done, if notifications were on during SetWindowText()
	SetSel(cr);
	SetFocus();
}

void CCshRichEditCtrl::UpdateCsh(bool force)
{
	if (m_bCshUpdateInProgress) return;
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	//Keep running if color syntax highlighting is not enabled, but we are forced to reset csh to normal
	if (!pApp->m_bCsh && !force) return;
	CHARFORMAT *const scheme = pApp->m_csh_cf[pApp->m_nCshScheme];

	SetRedraw(false);
	//long eventMask = GetEventMask();
	//SetEventMask(0);
	m_bCshUpdateInProgress = true;

	POINT scroll_pos;
	::SendMessage(m_hWnd, EM_GETSCROLLPOS, 0, (LPARAM)&scroll_pos);
	CHARRANGE cr;
	GetSel(cr);

	SetSel(0,-1); //select all
	SetSelectionCharFormat(scheme[COLOR_NORMAL]); //set formatting to neutral
	if (pApp->m_bCsh) {
		const DWORD effects = scheme[COLOR_NORMAL].dwEffects;
		const COLORREF color = scheme[COLOR_NORMAL].crTextColor;
		CString text;
		GetWindowText(text);
		RemoveCRLF(text);
		Msc csh;
		csh.ParseForCSH(text, text.GetLength());
		for (MscCshListType::const_iterator i=csh.CshList.begin(); i!=csh.CshList.end(); i++) 
			if (scheme[i->color].dwEffects != effects || scheme[i->color].crTextColor != color) {
				SetSel(i->first_pos-1, i->last_pos);
				SetSelectionCharFormat(scheme[i->color]);
			}
	}
	SetSel(cr);
	::SendMessage(m_hWnd, EM_SETSCROLLPOS, 0, (LPARAM)&scroll_pos);

	m_bCshUpdateInProgress = false;
	SetRedraw(true);
	Invalidate();
	//SetEventMask(eventMask);
}

void CCshRichEditCtrl::JumpToLine(int line, int col) {
	long index = ConvertLineColToPos(line-1, col?col-1:0);
	SetSel(index, index);
	SetFocus();
}

bool CCshRichEditCtrl::NotifyDocumentOfChange(bool onlySelChange) 
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	CFrameWnd *pMainWnd = dynamic_cast<CFrameWnd*>(pApp->GetMainWnd());
	ASSERT(pMainWnd!=NULL);
	if (pMainWnd->GetActiveView() == NULL) return false;
	CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(pMainWnd->GetActiveView()->GetDocument());
	if (pDoc == NULL) return false;
	if (onlySelChange) pDoc->OnInternalEditorSelChange();
	else pDoc->OnInternalEditorChange();

	SetFocus();
	return true;
}

BOOL CCshRichEditCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	CFrameWnd *pMainWnd = dynamic_cast<CFrameWnd*>(pApp->GetMainWnd());
	ASSERT(pMainWnd!=NULL);
	if (pMainWnd->GetActiveView() != NULL) {
		CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(pMainWnd->GetActiveView()->GetDocument());
		if (pDoc != NULL)
			pDoc->DispatchMouseWheel(nFlags, zDelta, pt);
	}
	return TRUE;
}

BOOL CCshRichEditCtrl::DoMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CRect view;
	GetClientRect(&view);
	ScreenToClient(&pt);
	//Process message only if within our view
	if (!view.PtInRect(pt)) return FALSE;
	CWnd::OnMouseWheel(nFlags, zDelta, pt);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// COutputBar

static UINT nFindReplaceDialogMessage = ::RegisterWindowMessage(FINDMSGSTRING);

BEGIN_MESSAGE_MAP(CEditorBar, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_NOTIFY(EN_SELCHANGE, IDC_INTERNAL_EDITOR, OnSelChange)
	ON_REGISTERED_MESSAGE(nFindReplaceDialogMessage, OnFindReplaceMessage)
END_MESSAGE_MAP()

CEditorBar::CEditorBar() : m_pFindReplaceDialog( NULL )
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	m_bSuspendNotifications = FALSE;
	m_bLastMatchCase = pApp->GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_FINDMATCHCASE, 1);; 
	m_bLastMatchWholeWord = false;
	m_ptLastFindPos.x = pApp->GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_FINDWINPOS_X, 0);
	m_ptLastFindPos.y = pApp->GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_FINDWINPOS_Y, 0);
	CDC dc;
	dc.CreateCompatibleDC(NULL);
	CRect r;
	dc.GetClipBox(&r);
	r.bottom -= 20;
	r.right -=20;
	if (!r.PtInRect(m_ptLastFindPos)) m_ptLastFindPos.x = m_ptLastFindPos.y = 0;
}

CEditorBar::~CEditorBar()
{
	if (m_ptLastFindPos != CPoint(0,0)) {
		CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
		ASSERT(pApp != NULL);
		pApp->WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_FINDWINPOS_X, m_ptLastFindPos.x);
		pApp->WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_FINDWINPOS_Y, m_ptLastFindPos.y);
		pApp->WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_FINDMATCHCASE, m_bLastMatchCase);
	}
}

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

	if (!m_ctrlEditor.Create(dwStyle, rectClient, this, IDC_INTERNAL_EDITOR))
	{
		TRACE0("Failed to create output window\n");
		return -1;      // fail to create
	}
	m_ctrlEditor.SetTextMode(TM_SINGLECODEPAGE | TM_MULTILEVELUNDO | TM_RICHTEXT);
	m_ctrlEditor.SetFont(&m_Font);
	m_ctrlEditor.SetFocus();
	m_ctrlEditor.SetEventMask(m_ctrlEditor.GetEventMask() | ENM_CHANGE | ENM_SELCHANGE);

	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	CFrameWnd *pMainWnd = dynamic_cast<CFrameWnd*>(pApp->GetMainWnd());
	ASSERT(pMainWnd!=NULL);
	if (pMainWnd->GetActiveView() != NULL) {
		CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(pMainWnd->GetActiveView()->GetDocument());
		if (pDoc != NULL && pDoc->m_ExternalEditor.IsRunning())
			SetReadOnly();
	}

	return 0;
}

#define READONLY_STRING " (Read-only)"
void CEditorBar::SetReadOnly(bool readonly)
{
	if (!m_ctrlEditor.SetReadOnly(readonly)) return;
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

	m_ctrlEditor.SetWindowPos(NULL, rc.left + 1, rc.top + 1, rc.Width() - 2, rc.Height() - 2, SWP_NOACTIVATE | SWP_NOZORDER );
}

/////////////////////////////////////////////////////////////////////////////
// CEditorBar message handlers

void CEditorBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_ctrlEditor.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CEditorBar::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_ctrlEditor.SetFocus();
}


BOOL CEditorBar::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UINT nID = LOWORD(wParam);
	HWND hWndCtrl = (HWND)lParam;
	int nCode = HIWORD(wParam);

	if (hWndCtrl != m_ctrlEditor) return CDockablePane::OnCommand(wParam, lParam);
	if (nCode != EN_CHANGE) return CDockablePane::OnCommand(wParam, lParam);
	if (m_ctrlEditor.IsCshUpdateInProgress() || m_bSuspendNotifications) return TRUE;

	m_ctrlEditor.UpdateCsh();
	m_ctrlEditor.NotifyDocumentOfChange();
	return TRUE;
}

void CEditorBar::OnSelChange(NMHDR *pNotifyStruct, LRESULT *result) 
{
	if (m_bSuspendNotifications) return;
	m_ctrlEditor.NotifyDocumentOfChange(true);
	*result = 0;
}

//Find replace related

bool CEditorBar::UpdateLastFindStringFromSelection()
{
	//See if there is a selection and if it is within a single line. If so, copy that to the search box
	long lStart, lEnd;
	m_ctrlEditor.GetSel(lStart, lEnd);
	if (lStart == lEnd) return false;
	if (m_ctrlEditor.LineFromChar(lStart) != m_ctrlEditor.LineFromChar(lEnd)) return false;
	m_sLastFindString = m_ctrlEditor.GetSelText();
	return true;
}

void CEditorBar::OnEditFindReplace(bool findOnly)
{
	//If window is up, store location and destroy
	if (m_pFindReplaceDialog) {
		CRect r;
		m_pFindReplaceDialog->GetWindowRect(r);
		m_ptLastFindPos = r.TopLeft();
		delete m_pFindReplaceDialog;
	}
	UpdateLastFindStringFromSelection();
	
	m_pFindReplaceDialog = new CFindReplaceDialog( );
	const DWORD dwFlags = FR_HIDEUPDOWN | (m_bLastMatchCase?FR_MATCHCASE:0) | (m_bLastMatchWholeWord?FR_WHOLEWORD:0);
	m_pFindReplaceDialog->Create(findOnly, m_sLastFindString, m_sLastReplaceString, dwFlags, this );
	m_pFindReplaceDialog->SetActiveWindow( );
	m_pFindReplaceDialog->ShowWindow( TRUE );
	if (m_ptLastFindPos != CPoint(0,0))
		m_pFindReplaceDialog->SetWindowPos(NULL, m_ptLastFindPos.x, m_ptLastFindPos.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void CEditorBar::OnEditRepeat()
{
	//If the find window is not there use the selection as search string
	if (!m_pFindReplaceDialog || !::IsWindow(*m_pFindReplaceDialog) || !m_pFindReplaceDialog->IsWindowVisible())
			UpdateLastFindStringFromSelection();
	//If there is nothing to search or we do not find it, beep
	if (m_sLastFindString.IsEmpty() || 
		!FindText(m_sLastFindString, m_bLastMatchCase, m_bLastMatchWholeWord))
		TextNotFound(m_sLastFindString);
}

LRESULT CEditorBar::OnFindReplaceMessage(WPARAM wParam, LPARAM lParam)
{

	CFindReplaceDialog* pFindReplace = CFindReplaceDialog::GetNotifier(lParam);
    ASSERT(pFindReplace != NULL);

	//Store values for next opening or OnEditRepeat
	m_sLastFindString = pFindReplace->GetFindString();
	m_sLastReplaceString = pFindReplace->GetReplaceString();
	m_bLastMatchCase = pFindReplace->MatchCase();
	m_bLastMatchWholeWord = pFindReplace->MatchWholeWord();
	CRect r;
	pFindReplace->GetWindowRect(r);
	m_ptLastFindPos = r.TopLeft();

	if (pFindReplace->IsTerminating()) {
		m_pFindReplaceDialog = NULL;
		return 0;
    } 

	if (pFindReplace->ReplaceAll()) {
		CWaitCursor wait;
		if (!SameAsSelected(pFindReplace->GetFindString(), pFindReplace->MatchCase())) {
			if (!FindText(pFindReplace->GetFindString(), pFindReplace->MatchCase(), pFindReplace->MatchWholeWord())) {
				TextNotFound(pFindReplace->GetFindString());
				return 0;
			}
		}
		m_bSuspendNotifications = true;
		m_ctrlEditor.HideSelection(TRUE, FALSE);
		do {
			m_ctrlEditor.ReplaceSel(pFindReplace->GetReplaceString());
		} while (FindText(pFindReplace->GetFindString(), pFindReplace->MatchCase(), pFindReplace->MatchWholeWord()));
		m_ctrlEditor.HideSelection(FALSE, FALSE);
		m_bSuspendNotifications = false;
		m_ctrlEditor.UpdateCsh(); 
		m_ctrlEditor.NotifyDocumentOfChange();
		return 0;
	}
	bool findnext = pFindReplace->FindNext();
	if (pFindReplace->ReplaceCurrent()) {
		long start, end;
		m_ctrlEditor.GetSel(start, end);
		//if nothing is selected, skip replace, just go to find next
		if (start!=end) 
			m_ctrlEditor.ReplaceSel(pFindReplace->GetReplaceString(), FALSE);
		findnext = true;
	}
	//Find next for both FindNext and ReplaceCurrent
	if (findnext) {
		if (FindText(pFindReplace->GetFindString(), pFindReplace->MatchCase(), pFindReplace->MatchWholeWord())) {
			AdjustDialogPosition(pFindReplace);
			CRect r;
			pFindReplace->GetWindowRect(r);
			m_ptLastFindPos = r.TopLeft();
		} else
			TextNotFound(pFindReplace->GetFindString());
	} 
	return 0;
}


BOOL CEditorBar::SameAsSelected(LPCTSTR lpszCompare, BOOL bCase)
{
	// check length first
	size_t nLen = lstrlen(lpszCompare);
	long lStartChar, lEndChar;
	m_ctrlEditor.GetSel(lStartChar, lEndChar);
	if (nLen != (size_t)(lEndChar - lStartChar))
		return FALSE;

	// length is the same, check contents
	CString strSelect = m_ctrlEditor.GetSelText();
	return (bCase && lstrcmp(lpszCompare, strSelect) == 0) ||
		(!bCase && lstrcmpi(lpszCompare, strSelect) == 0);
}

void CEditorBar::AdjustDialogPosition(CDialog* pDlg)
{
	ASSERT(pDlg != NULL);
	long lStart, lEnd;
	m_ctrlEditor.GetSel(lStart, lEnd);
	CPoint point = m_ctrlEditor.GetCharPos(lStart);
	ClientToScreen(&point);
	CRect rectDlg;
	pDlg->GetWindowRect(&rectDlg);
	if (rectDlg.PtInRect(point))
	{
		if (point.y > rectDlg.Height())
			rectDlg.OffsetRect(0, point.y - rectDlg.bottom - 20);
		else
		{
			int nVertExt = GetSystemMetrics(SM_CYSCREEN);
			if (point.y + rectDlg.Height() < nVertExt)
				rectDlg.OffsetRect(0, 40 + point.y - rectDlg.top);
		}
		pDlg->MoveWindow(&rectDlg);
	}
}


BOOL CEditorBar::FindText(LPCTSTR lpszFind, BOOL bCase, BOOL bWord, BOOL bNext /* = TRUE */)
{
	ASSERT_VALID(this);
	ASSERT(lpszFind != NULL);
	FINDTEXTEX ft;

	m_ctrlEditor.GetSel(ft.chrg);
	if (m_bFirstSearch) {
		if (bNext)
			m_lInitialSearchPos = ft.chrg.cpMin;
		else
			m_lInitialSearchPos = ft.chrg.cpMax;
		m_bFirstSearch = FALSE;
	}

	// lpstrText should be const
	ft.lpstrText = (LPTSTR) lpszFind;

	if (ft.chrg.cpMin != ft.chrg.cpMax) {// i.e. there is a selection
		if (bNext)
			ft.chrg.cpMin++;
		else {
			// won't wraparound backwards
			if (ft.chrg.cpMin<0) ft.chrg.cpMin = 0;
		}
	}

	DWORD dwFlags = bCase ? FR_MATCHCASE : 0;
	dwFlags |= bWord ? FR_WHOLEWORD : 0;

	ft.chrg.cpMax = m_ctrlEditor.GetTextLength() + m_lInitialSearchPos;

	if (bNext) {
		if (m_lInitialSearchPos >= 0)
			ft.chrg.cpMax = m_ctrlEditor.GetTextLength();

		dwFlags |= FR_DOWN;
		ASSERT(ft.chrg.cpMax >= ft.chrg.cpMin);
	} else {
		if (m_lInitialSearchPos >= 0)
			ft.chrg.cpMax = 0;

		dwFlags &= ~FR_DOWN;
		ASSERT(ft.chrg.cpMax <= ft.chrg.cpMin);
	}

	// if we find the text return TRUE
	if (FindAndSelect(dwFlags, ft) != -1)
		return TRUE;
	// if the original starting point was not the beginning of the buffer
	// and we haven't already been here
	else if (m_lInitialSearchPos > 0) {
		if (bNext) {
			ft.chrg.cpMin = 0;
			ft.chrg.cpMax = m_lInitialSearchPos;
		} else {
			ft.chrg.cpMin = m_ctrlEditor.GetTextLength();
			ft.chrg.cpMax = m_lInitialSearchPos;
		}
		m_lInitialSearchPos = m_lInitialSearchPos - m_ctrlEditor.GetTextLength();
		return FindAndSelect(dwFlags, ft) != -1;
	}
	// not found
	else
		return FALSE;
}

long CEditorBar::FindAndSelect(DWORD dwFlags, FINDTEXTEX& ft)
{
	long index = m_ctrlEditor.FindText(dwFlags, &ft);
	if (index != -1) // i.e. we found something
		m_ctrlEditor.SetSel(ft.chrgText);
	return index;
}

void CEditorBar::TextNotFound(LPCTSTR lpszFind)
{
	ASSERT_VALID(this);
	m_bFirstSearch = TRUE;
	MessageBeep(MB_ICONHAND);
}

