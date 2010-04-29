// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CMiniEditor window

class CEditorBar : public CDockablePane
{
// Construction
public:
	CEditorBar();

// Attributes
	CFont m_Font;
	CRichEditCtrl m_wndEditor;
	bool m_bCshUpdateInProgress;
	bool m_modified;
protected:

// Implementation
public:
	virtual ~CEditorBar();
	void UpdateText(const char *text);
	void UpdateCsh(bool force = false);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	BOOL ShowWindow(int nCmdShow);
};

