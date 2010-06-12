// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#pragma once
#undef min
#undef max
#include <msc.h>
/////////////////////////////////////////////////////////////////////////////
// CMiniEditor window

struct CEditorUndoRecord {
	CString text;
	CHARRANGE pos;
};

class CCshRichEditCtrl : public CRichEditCtrl
{
	bool m_bCshUpdateInProgress;
	Csh  m_csh;
public:
	CCshRichEditCtrl();
	int  FindColonLabelIdent(long lStart);
	int  FirstNonWhitespaceIdent(const char *str, int Max=-1);
	BOOL PreTranslateMessage(MSG* pMsg);
	void UpdateText(const char *text, CHARRANGE &cr, bool preventNotification);
	void UpdateCsh(bool force = false);
	long ConvertLineColToPos(unsigned line, unsigned col) {return LineIndex(line) + col;}
	void ConvertPosToLineCol(long pos, int &line, int &col) {line=LineFromChar(pos); col=pos-LineIndex(line);}
	void JumpToLine(int line, int col);
	bool IsCshUpdateInProgress() {return m_bCshUpdateInProgress;}
	bool NotifyDocumentOfChange(bool onlySelChange=false);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt); 
	        BOOL DoMouseWheel(UINT nFlags, short zDelta, CPoint pt); 

	DECLARE_MESSAGE_MAP()
};

class CEditorBar : public CDockablePane
{
// Construction
public:
	CEditorBar();

// Attributes
	CFont m_Font;
	CCshRichEditCtrl m_ctrlEditor;
	CFindReplaceDialog* m_pFindReplaceDialog;
	CString m_sLastFindString;
	CString m_sLastReplaceString;
	bool m_bLastMatchCase;
	bool m_bLastMatchWholeWord;
	CPoint m_ptLastFindPos;
	long m_lInitialSearchPos;
	UINT m_nPasteType;
	BOOL m_bFirstSearch;
	bool m_bSuspendNotifications;

// Implementation
public:
	virtual ~CEditorBar();
	void SetReadOnly(bool=true);
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    afx_msg void OnSelChange(NMHDR*pNotifyStruct, LRESULT*result);
	LRESULT OnFindReplaceMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEditFindReplace(bool findOnly);
	afx_msg void OnEditRepeat();

	bool UpdateLastFindStringFromSelection();
	BOOL SameAsSelected(LPCTSTR lpszCompare, BOOL bCase);
	void AdjustDialogPosition(CDialog* pDlg);
	BOOL FindText(LPCTSTR lpszFind, BOOL bCase = TRUE, BOOL bWord = TRUE, BOOL bNext = TRUE);
	long FindAndSelect(DWORD dwFlags, FINDTEXTEX& ft);
	void TextNotFound(LPCTSTR lpszFind);
};
