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
/** @file PopupList.h The interface for the list box & its container 
 * window popping up to offer hints. 
 * @ingroup Msc_generator_files */
#pragma once
#include "afxwin.h"
#include "stringparse.h"
#include "csh.h"

struct HintListBoxItemCacheElement
{
    string label;
    EHintItemSelectionState state;
    CshHintGraphicCallback callback;
    CshHintGraphicParam param;
    HintListBoxItemCacheElement(const string &l, EHintItemSelectionState s,
                                CshHintGraphicCallback c, CshHintGraphicParam p) :
                                label(l), state(s), callback(c), param(p) {}
    bool operator < (const HintListBoxItemCacheElement &o) const {
        return state < o.state ? true : state  > o.state ? false :
               callback < o.callback ? true : callback > o.callback ? false :
               param < o.param ? true : param > o.param ? false :
               label < o.label;
    }
};

typedef std::map<HintListBoxItemCacheElement, std::auto_ptr<CBitmap>> HintListBoxItemCache;


/** An owner-drawn list box containing a list of hints.*/
class CHintListBox : public CListBox 
{
protected:
    DECLARE_MESSAGE_MAP()
    BOOL OnToolTipText(UINT id, NMHDR * pNMHDR, LRESULT * pResult);
public:
    CSize             m_current_size; ///<The current geometry of us.
    StringFormat      m_format;       ///<Default format for hints
    int               m_cur_sel;      ///<The index of the currently selected item
    Csh               m_csh;          ///<Stores the current set of hints and shapes
    CMFCToolTipCtrl   m_tooltip;      ///<The tooltip showing hint descriptors
    HintListBoxItemCache m_cache;       ///<We store pre-drawn box items here.
    CHintListBox();
    
    enum EHintProcResult {
        HINTS_REPLACE,     ///<After chain forwarding, we determine, we need to replace the string under cursor (only one result and user pressed Ctrl+Space)
        HINTS_CHANGED,     ///<The list of hints changed
        HINTS_NOT_CHANGED, ///<The list of hints remained as before
    };

    EHintProcResult PreprocessHints(Csh &csh, const std::string &uc, bool userRequest, bool afterReturnKey, bool inhintmode);
    CSize SetHintsToCurrent();
    void SetStringUnderCursor(const char *uc);
    void UpDownKey(int offset);
    void ChangeSelectionTo(int index, EHintItemSelectionState state);
    void ChangeSelectionTo(int index);
    /** Return a pointer to the hint currently selected. NULL if nothing selected.*/
    const CshHint* GetSelectedHint() const {return GetHint(m_cur_sel);}
    /** Return a pointer to the i_th hint. NULL if i is out of bounds.*/
    const CshHint* GetHint(int i) const { return (0<=i && i<GetCount()) ? GetHint((const char*)GetItemData(i)) : NULL; }
    const CshHint* GetHint(const char *plain) const;
    virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
    virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
    virtual int CompareItem(LPCOMPAREITEMSTRUCT /*lpCompareItemStruct*/);
};

class CCshRichEditCtrl;

// CPopupList dialog

/** A dialog window having a single list.*/
class CPopupList : public CDialog
{
	DECLARE_DYNAMIC(CPopupList)

public:
	CPopupList(CWnd* pParent, CCshRichEditCtrl* pEditCtrl);   // standard constructor
    virtual ~CPopupList() {}

    CCshRichEditCtrl *m_pEditCtrl;
// Dialog Data
	enum { IDD = IDD_POPUPLIST };

    void Show(const LPCSTR uc, int x, int y);
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
