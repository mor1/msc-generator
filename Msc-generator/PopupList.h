/*
    This file is part of Msc-generator.
	Copyright 2008,2009,2010,2011,2012,2013 Zoltan Turanyi
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
#pragma once
#include "afxwin.h"
#include "stringparse.h"
#include "csh.h"

class CHintListBox : public CListBox 
{
public:
    std::set<CshHint> m_current_hints;
    CSize             m_current_size;
    StringFormat      m_format;
    int               m_cur_sel;
    CHintListBox();
    bool PreprocessHints(Csh &csh, const std::string &uc, bool userRequest, bool afterReturnKey);
    CSize SetHintsToCurrent();
    void SetStringUnderCursor(const char *uc);
    void UpDownKey(int offset);
    void ChangeSelectionTo(int index, EHintItemSelectionState state);
    void ChangeSelectionTo(int index);
    const CshHint* GetSelectedHint() {return GetHint(m_cur_sel);}
    const CshHint* GetHint(int i) {return (0<=i && i<GetCount()) ? (const CshHint*)GetItemData(i) : NULL;}
    virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
    virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
    virtual int CompareItem(LPCOMPAREITEMSTRUCT /*lpCompareItemStruct*/);
};


class CCshRichEditCtrl;

// CPopupList dialog

class CPopupList : public CDialog
{
	DECLARE_DYNAMIC(CPopupList)

public:
	CPopupList(CWnd* pParent, CCshRichEditCtrl* pEditCtrl);   // standard constructor
	virtual ~CPopupList();

    CCshRichEditCtrl *m_pEditCtrl;
// Dialog Data
	enum { IDD = IDD_POPUPLIST };

    void Show(bool changed, const LPCSTR uc, int x, int y);
    void Hide();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    CHintListBox m_listBox;
    bool         m_shown;
    afx_msg void OnLbnDblclkList2();
    afx_msg void OnLbnSelcancelList2();
    afx_msg void OnLbnSelchangeList2();
};
