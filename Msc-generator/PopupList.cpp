/*
    This file is part of Msc-generator.
	Copyright 2008,2009,2010,2011 Zoltan Turanyi
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
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/
// PopupList.cpp : implementation file
//

#include "stdafx.h"
#include "Msc-generator.h"
#include "PopupList.h"
#include "afxdialogex.h"
#include "MiniEditor.h"

CHintListBox::CHintListBox()
{
    m_format.Default();
    m_format += "\\f(Courier New)\\mn(12)\\ms(8)\\pl";
    //m_format += "\\f(Arial)\\mn(24)\\ms(8)\\pl";
}

bool CHintListBox::PreprocessHints(Csh &csh, const std::string &uc, bool userRequest, bool afterReturnKey)
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    if (!pApp->m_bHints || csh.hintStatus != HINT_READY)
        csh.Hints.clear();
    else {
        //Now delete those not requested by the user preferences,
        //but only if we do hints not in response to a Ctrl+Space
        //Keep first line ones only if after the user pressed newline
        if (csh.hintType == HINT_LINE_START && !userRequest) 
            if (!pApp->m_bHintLineStart || (!afterReturnKey && uc.length()==0))
                csh.Hints.clear();
        if (!userRequest)
            if ((!pApp->m_bHintAttrName && csh.hintType==HINT_ATTR_NAME) ||
                (!pApp->m_bHintAttrValue && csh.hintType==HINT_ATTR_VALUE) ||
                (!pApp->m_bHintEntity && csh.hintType==HINT_ENTITY))
                csh.Hints.clear();
    }    
    //Now process the list of hints: fill extra fields, compute size, filter by uc and compact
    if (csh.Hints.size()) {
        //Display all hints (not only the ones matching the string under cursor) if Ctrl+Space
        const bool filter_by_uc = pApp->m_bHintFilter && !userRequest;
        CDC* pDC = GetDC();
        MscDrawer mscdrawer;
        mscdrawer.SetOutputWin32(MscDrawer::WIN, pDC->m_hAttribDC);
        mscdrawer.total.x = HINT_GRAPHIC_SIZE_X;
        mscdrawer.total.y = HINT_GRAPHIC_SIZE_Y;
        csh.ProcessHints(&mscdrawer, &m_format, uc, filter_by_uc, pApp->m_bHintCompact);
        mscdrawer.CloseOutput();
        ReleaseDC(pDC);
    }

    if (csh.Hints.size()==0) {
        if (m_current_hints.size()==0) return false;
        m_current_hints.clear();
        return true;
    }
    if (m_current_hints == csh.Hints)
        return false; //no change
    m_current_hints = csh.Hints;
    return true;
}


#define MAX_HINT_LISTBOX_LEN 10
CSize CHintListBox::SetHintsToCurrent()
{
    m_current_size.SetSize(0,0);
    if (m_current_hints.size()==0) {
        m_cur_sel = -1;
        ResetContent();
        return m_current_size;
    }
    SetRedraw(false);
    //Copy hints to the listbox and resize
    ResetContent();
    int added = 0;
    for (auto i=m_current_hints.begin(); i!=m_current_hints.end(); i++) {
        int index = AddString((LPCSTR)&*i);

        if (i->x_size > m_current_size.cx)
            m_current_size.cx = i->x_size;
        if (added<MAX_HINT_LISTBOX_LEN)
            m_current_size.cy += std::max(i->y_size, HINT_GRAPHIC_SIZE_Y);
        added++;
    }
    //Adjust magic numbers
    m_current_size.cy += 5;
    m_current_size.cx += 7;
    //Make it at least this big
    if (m_current_size.cx < 50) m_current_size.cx = 50;
    //Add space for graphic part
    m_current_size.cx += HINT_GRAPHIC_SIZE_X;
    if (added>MAX_HINT_LISTBOX_LEN) 
        m_current_size.cx += ::GetSystemMetrics( SM_CXVSCROLL ); //scrollbar will pop up
    SetWindowPos(NULL, 0, 0, m_current_size.cx, m_current_size.cy, SWP_NOOWNERZORDER | SWP_SHOWWINDOW);
    SetRedraw(true);
    m_cur_sel = GetCurSel();
    return m_current_size;
}

inline CRect ConvertABCDToCRect(const CshHint *p) 
{
    return CRect(p->ul_x, p->ul_y, p->br_x, p->br_y);
}

void CHintListBox::SetStringUnderCursor(const char *uc)
{
    if (GetCount()==0) return;
    int i;
    CshHintItemSelectionState sel_type;
    int smaller = -1;  //the last one that is still smaller and selectable
    if (uc && uc[0]) {
        for (i=0; i<GetCount(); i++) {
            const CshHint *item= GetHint(i);
            if (!item->selectable) continue; 
            const char *p = item->plain.c_str(); 
            int j=0;
            while (p[j]&&uc[j]&&toupper(p[j])==toupper(uc[j])) j++;
            //if exact or prefix match select this
            if (p[j]==uc[j] || uc[j]==0) {
                ChangeSelectionTo(i, HINT_ITEM_SELECTED);
                return;
            }
            //if we went beyond, select only half
            if (toupper(p[j]) > toupper(uc[j])) {
                if (smaller < 0) ChangeSelectionTo(i, HINT_ITEM_SELECTED_HALF);
                else ChangeSelectionTo(smaller, HINT_ITEM_SELECTED_HALF);
                return;
            }
            smaller = i;
        }
        //We did not find any selectable, do not select anything
    }
    ChangeSelectionTo(-1);
    return;
}

//offset +-1 for up/down arrow, +-2 for pgup/dn
void CHintListBox::UpDownKey(int offset)
{
    _ASSERT(abs(offset)<=2);
    if (abs(offset)==2)
        offset = (offset<0) ? -MAX_HINT_LISTBOX_LEN : MAX_HINT_LISTBOX_LEN;
    const int count = GetCount();
    int sel = m_cur_sel;
    if (sel==-1) 
        sel = (offset<-1) ? count-1 : 0;
    else {
        sel += offset;
        if (sel<0) sel = 0;
        if (sel>=count) sel = count-1;
    }
    ChangeSelectionTo(sel);
}

void CHintListBox::ChangeSelectionTo(int index)
{
    if (index>=0 && index<GetCount()) 
        ChangeSelectionTo(index, GetHint(index)->selectable ? HINT_ITEM_SELECTED : HINT_ITEM_SELECTED_HALF);
    else 
        ChangeSelectionTo(index, HINT_ITEM_NOT_SELECTED); //second param is dummy
}

void CHintListBox::ChangeSelectionTo(int index, CshHintItemSelectionState state)
{
    if (index == m_cur_sel) return;
    const CshHint *item_orig = (m_cur_sel>=0) ? GetHint(m_cur_sel) : NULL;
    if (item_orig) {
        item_orig->state = HINT_ITEM_NOT_SELECTED;
        InvalidateRect(ConvertABCDToCRect(item_orig));
    }
    if (index<0 || index>=GetCount()) index=-1;
    m_cur_sel = index;
    if (index==-1) return;
    const CshHint *item= GetHint(index);
    item->state = state;
    SetCurSel(index);
    InvalidateRect(ConvertABCDToCRect(item));
}

void CHintListBox::DrawItem(LPDRAWITEMSTRUCT lpItem)
{
    if (lpItem->itemID==-1) return;
    MscDrawer mscdrawer;
    mscdrawer.SetOutputWin32(MscDrawer::WIN, lpItem->hDC);
    CshHint *item= (CshHint*)lpItem->itemData;
    Label label(item->decorated, &mscdrawer, m_format);
    XY wh = label.getTextWidthHeight();
    Block b(lpItem->rcItem.left+1, lpItem->rcItem.right-1, lpItem->rcItem.top, lpItem->rcItem.bottom-1);
    MscColorType black(0,0,0);
    MscFillAttr fill(black.Lighter(0.75), GRADIENT_DOWN);
    MscLineAttr line(LINE_SOLID, black.Lighter(0.5), 1, CORNER_ROUND, 3);
    switch (item->state) {
    case HINT_ITEM_SELECTED:
        mscdrawer.Fill(b, line, fill);
    case HINT_ITEM_SELECTED_HALF:
        mscdrawer.Line(b, line);
    }
    int y = ((lpItem->rcItem.bottom - lpItem->rcItem.top) - item->y_size)/2;
    label.Draw(lpItem->rcItem.left+ HINT_GRAPHIC_SIZE_X, lpItem->rcItem.right, lpItem->rcItem.top + y);
    if (item->callback) {
        int y = ((lpItem->rcItem.bottom - lpItem->rcItem.top) - HINT_GRAPHIC_SIZE_Y)/2;
        cairo_translate(mscdrawer.GetContext(), lpItem->rcItem.left, lpItem->rcItem.top + y);
        item->callback(&mscdrawer, item->param);
        //We do not restore the context, we drop it anyway
    }
    mscdrawer.CloseOutput();
    item->ul_x = lpItem->rcItem.left;
    item->ul_y = lpItem->rcItem.top;
    item->br_x = lpItem->rcItem.right;
    item->br_y = lpItem->rcItem.bottom;
}


void CHintListBox::MeasureItem(LPMEASUREITEMSTRUCT lpItem)
{
    lpItem->itemWidth  = ((CshHint*)lpItem->itemData)->x_size;
    lpItem->itemHeight = ((CshHint*)lpItem->itemData)->y_size;
}

int CHintListBox::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct)
{
    // return -1 = item 1 sorts before item 2
    // return 0 = item 1 and item 2 sort the same
    // return 1 = item 1 sorts after item 2
    CshHint *item1 = (CshHint*)lpCompareItemStruct->itemData1;
    CshHint *item2 = (CshHint*)lpCompareItemStruct->itemData2;
    //selectable items come first
    if (item1->selectable == item2->selectable) {
        int j=0;
        while (j<item1->plain.length() && j<item2->plain.length() && 
               toupper(item1->plain[j]) == toupper(item2->plain[j])) j++;
        if (j==item1->plain.length() && j==item2->plain.length()) 
            return 0; //equal 
        if (j==item1->plain.length()) return -1;
        if (j==item2->plain.length()) return +1;
        return (toupper(item1->plain[j]) < toupper(item2->plain[j])) ? -1 : 1;
    }
    if (item1->selectable) 
        return -1;
    return 1;
}

// CPopupList dialog

IMPLEMENT_DYNAMIC(CPopupList, CDialogEx)

CPopupList::CPopupList(CWnd* pParent, CCshRichEditCtrl* pEditCtrl)
	: CDialog(CPopupList::IDD, pParent)
{
    m_pEditCtrl = pEditCtrl;
    m_shown=false;
}

CPopupList::~CPopupList()
{
}

void CPopupList::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST2, m_listBox);
}


BEGIN_MESSAGE_MAP(CPopupList, CDialog)
    ON_LBN_DBLCLK(IDC_LIST2, &CPopupList::OnLbnDblclkList2)
    ON_LBN_SELCANCEL(IDC_LIST2, &CPopupList::OnLbnSelcancelList2)
    ON_LBN_SELCHANGE(IDC_LIST2, &CPopupList::OnLbnSelchangeList2)
END_MESSAGE_MAP()


// CPopupList message handlers
void CPopupList::Show(bool changed, const LPCSTR uc, int x, int y)
{
    if (changed) {
        if (m_listBox.m_current_hints.size()==0) {
            Hide();
            return;
        }
        CSize size = m_listBox.SetHintsToCurrent();
        SetWindowPos(&CWnd::wndTop, x, y, size.cx+3, size.cy+3, SWP_NOOWNERZORDER | SWP_SHOWWINDOW);
    } else {
        //not changed
        if (m_listBox.m_current_hints.size()==0) {
            _ASSERT(!m_shown);
            return;
        }
        //just update window pos (should not be needed!)
        SetWindowPos(&CWnd::wndTop, x, y, 0, 0, SWP_NOOWNERZORDER | SWP_SHOWWINDOW | SWP_NOSIZE);
    } 
    m_listBox.SetStringUnderCursor(uc);
    m_shown=true;
}

void CPopupList::Hide() 
{
    if (m_shown) {
        ShowWindow(SW_HIDE); 
        m_listBox.ResetContent();
        m_listBox.m_current_hints.clear();
        m_shown=false; 
    }
    m_pEditCtrl->SetFocus();
}



void CPopupList::OnLbnDblclkList2()
{
    const CshHint *item = m_listBox.GetSelectedHint();
    if (item && item->state == HINT_ITEM_SELECTED) 
        m_pEditCtrl->ReplaceHintedString(item->plain.c_str(), !item->keep);
}


void CPopupList::OnLbnSelcancelList2()
{
    m_listBox.ChangeSelectionTo(-1);
}


void CPopupList::OnLbnSelchangeList2()
{
    m_listBox.ChangeSelectionTo(m_listBox.GetCurSel());
}
