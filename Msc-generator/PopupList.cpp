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
        {
            MscCanvas canvas(MscCanvas::WIN, pDC->m_hDC, Block(0,HINT_GRAPHIC_SIZE_X, 0,HINT_GRAPHIC_SIZE_Y));
            csh.ProcessHints(canvas, &m_format, uc, filter_by_uc, pApp->m_bHintCompact);
            //Destroy canvas before the DC
        }
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
        AddString((LPCSTR)&*i);
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
    return CRect(p->ul_x-1, p->ul_y-1, p->br_x+1, p->br_y+1);
}

void CHintListBox::SetStringUnderCursor(const char *uc)
{
    if (GetCount()==0) return;
    //Go through and determine the longest matching entry
    //elements are not necessarily sorted alphabetically
    int match_index = -1;  
    unsigned match_len = 0;
    if (uc && uc[0]) {
        const unsigned uc_len = strlen(uc);
        for (int i=0; i<GetCount(); i++) {
            const CshHint *item= GetHint(i);
            if (!item->selectable) continue; 
            const char *p = item->plain.c_str(); 
            unsigned len = CaseInsensitiveCommonPrefixLen(p, uc);
            if (len==item->plain.length()) {
                if (len==uc_len) 
                    //if p == uc, we select
                    ChangeSelectionTo(i, HINT_ITEM_SELECTED);
                else 
                    //if uc is longer than p, we select half
                    ChangeSelectionTo(i, HINT_ITEM_SELECTED_HALF);
                return;
            }
            if (len <= match_len) continue;
            match_index = i;
            match_len = len;
        }
    }
    ChangeSelectionTo(match_index);
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
    const CRect rect = ConvertABCDToCRect(item);
    SetCurSel(index);
    //InvalidateRect(rect);
    //Invalidate();
}

void CHintListBox::DrawItem(LPDRAWITEMSTRUCT lpItem)
{
    //Do not draw focus rectangle (or anything else) for empty lists
    if (lpItem->itemID==-1) return;
    //We do not differentiate in appearance whether we have the focus or not,
    //so for ODA_FOCUS, nothing to do
    if (lpItem->itemAction != ODA_DRAWENTIRE && lpItem->itemAction != ODA_SELECT) return;

    /* Cairo 1.12.8 has a bug with Windows Display surfaces, when the original clip of the
    surface is nonzero. This results in the surface having a nonzero extent origin. When
    operations requiring fallback are done, the map_to_image function screws up. 
    This bug applies here as the DC we get here is clipped to the actual item.
    We work around by first creating a DIB-based DC (no clip, zero extent origin)
    and by BitBlt-ing that to the original DC.*/
    
    CDC origDC, memDC;
    origDC.Attach(lpItem->hDC);
	memDC.CreateCompatibleDC(&origDC);
	CBitmap bitmap;
	CBitmap *oldBitmap;
    bitmap.CreateCompatibleBitmap(&origDC, lpItem->rcItem.right-lpItem->rcItem.left,
                                           lpItem->rcItem.bottom - lpItem->rcItem.top);
    oldBitmap = memDC.SelectObject(&bitmap);
    memDC.FillSolidRect(0,0,lpItem->rcItem.right-lpItem->rcItem.left,
                                           lpItem->rcItem.bottom - lpItem->rcItem.top, origDC.GetBkColor());
    MscCanvas canvas(MscCanvas::WIN, memDC.m_hDC);
    const CshHint *item= (CshHint*)lpItem->itemData;
    Label label(item->decorated, canvas, m_format);
    const MscColorType black(0,0,0);
    const MscFillAttr fill(black.Lighter(0.75), GRADIENT_DOWN);
    const MscLineAttr line(LINE_SOLID, black.Lighter(0.5), 1, CORNER_ROUND, 3);
    const Block b(0,lpItem->rcItem.right-lpItem->rcItem.left,
                  0,lpItem->rcItem.bottom - lpItem->rcItem.top);
    switch (item->state) {
    case HINT_ITEM_SELECTED:
        canvas.Fill(b, line, fill);
    case HINT_ITEM_SELECTED_HALF:
        canvas.Line(b, line);
    default:
        break;
    }
    const int y = ((lpItem->rcItem.bottom - lpItem->rcItem.top) - item->y_size)/2;
    label.Draw(canvas, HINT_GRAPHIC_SIZE_X, lpItem->rcItem.right-lpItem->rcItem.left, y);
    if (item->callback) {
        const int y2 = ((lpItem->rcItem.bottom - lpItem->rcItem.top) - HINT_GRAPHIC_SIZE_Y)/2;
        cairo_translate(canvas.GetContext(), 0, y2);
        item->callback(&canvas, item->param);
        //We do not restore the context, we drop it anyway
    }

    canvas.CloseOutput();
    origDC.BitBlt(lpItem->rcItem.left, lpItem->rcItem.top,  
                  lpItem->rcItem.right-lpItem->rcItem.left,
                  lpItem->rcItem.bottom - lpItem->rcItem.top,
                  &memDC, 0, 0, SRCCOPY);
    memDC.SelectObject(oldBitmap);
    origDC.Detach();
    
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
        unsigned j=0;
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
