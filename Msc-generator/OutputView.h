// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#pragma once
#include <vector>
#include "ChartData.h"

/////////////////////////////////////////////////////////////////////////////
// COutputList window

class COutputList : public CListBox
{
public:
    virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};

class COutputViewBar : public CDockablePane
{

// Attributes
	//CFont m_Font;
protected:
	CMFCTabCtrl	m_wndTabs;

	COutputList m_wndOutput;
	COutputList m_wndOutputHints;
    std::list<CString> compilation_errors;
    std::vector<std::pair<int, int>> error_pos;
    std::list<CString> compilation_errors_hint;
    std::vector<std::pair<int, int>> error_pos_hint;

protected:
	void AdjusrHorzScroll(CListBox& wndListBox);

// Implementation
public:
    void ShowCompilationErrors(const CDrawingChartData &chart);
    void ShowCshErrors(const std::list<CString> &errors, 
                       const std::vector<std::pair<int, int>> &err_pos);
    bool NextError(bool next);
    bool GetCurrentErrorLine(int &line, int &col);
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnDblClick() {}

	DECLARE_MESSAGE_MAP()
};

