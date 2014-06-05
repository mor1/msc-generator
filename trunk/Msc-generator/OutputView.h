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

/** @file OutputView.h The interface to the view showing error lists.
* @ingroup Msc_generator_files */

#pragma once
#include <vector>
#include "ChartData.h"

/////////////////////////////////////////////////////////////////////////////
// COutputList window

/** The list box used to display errors.
 * The only thin why we specialize CListBox, is to capture double-click events.*/
class COutputList : public CListBox
{
public:
    virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};

/** The dockable pane of the error messages.
 * It has a tab control with two pages eaxch containing one COutputList objects.
 * The two lists correspond to compilation errors or hinted errors.*/
class COutputViewBar : public CDockablePane
{

// Attributes
	CFont m_Font;                                     ///<The font we use to display errors.
protected:
	CMFCTabCtrl	m_wndTabs;                            ///<A tab control hosing the two lists
	COutputList m_wndOutput;                          ///<The list control for compilation errors.
    COutputList m_wndOutputHints;                     ///<The list control for hinted errors.
    std::list<CString> compilation_errors;            ///<A list of compilation error texts (with line num and file name included)
    std::vector<std::pair<int, int>> error_pos;       ///<A (line,col) values for compilation errors.
    std::list<CString> compilation_errors_hint;       ///<A list of hinted error texts (with line num included)
    std::vector<std::pair<int, int>> error_pos_hint;  ///<A (line,col) values for hinted errors.

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

	DECLARE_MESSAGE_MAP()
};

