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
// Construction
public:
	COutputList();

// Implementation
public:
	virtual ~COutputList();

protected:
	DECLARE_MESSAGE_MAP()
};

class COutputViewBar : public CDockablePane
{
// Construction
public:
	COutputViewBar();

// Attributes
	CFont m_Font;
	COutputList m_wndOutput;
    std::list<CString> compilation_errors;
    std::vector<std::pair<int, int>> error_pos;

protected:
	void AdjusrHorzScroll(CListBox& wndListBox);

// Implementation
public:
	virtual ~COutputViewBar();
    void ShowCompilationErrors(const CDrawingChartData &chart);
    void ShowCshErrors(const std::list<CString> &errors, 
                       const std::vector<std::pair<int, int>> &err_pos);
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);


	DECLARE_MESSAGE_MAP()
};

