// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#pragma once
#include <list>
/////////////////////////////////////////////////////////////////////////////
// CMiniEditor window

struct CEditorUndoRecord {
	CString text;
	CHARRANGE pos;
};

class CCshRichEditCtrl : public CRichEditCtrl
{
	bool m_bCshUpdateInProgress;
public:
	CCshRichEditCtrl();
	void UpdateText(const char *text, CHARRANGE &cr);
	void UpdateCsh(bool force = false);
	long ConvertLineColToPos(unsigned line, unsigned col) {return LineIndex(line) + col;}
	void ConvertPosToLineCol(long pos, int &line, int &col) {line=LineFromChar(pos); col=pos-LineIndex(line);}
	bool IsCshUpdateInProgress() {return m_bCshUpdateInProgress;}
};

class CEditorBar : public CDockablePane
{
// Construction
public:
	CEditorBar();

// Attributes
	CFont m_Font;
	CCshRichEditCtrl m_wndEditor;
protected:

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
DECLARE_MESSAGE_MAP()
};

