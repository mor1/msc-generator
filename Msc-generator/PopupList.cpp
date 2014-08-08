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
/** @file PopupList.cpp The implementation of the list box & its container 
 * window popping up to offer hints. 
 * @ingroup Msc_generator_files */

#include "stdafx.h"
#include "Msc-generator.h"
#include "PopupList.h"
#include "afxdialogex.h"
#include "MiniEditor.h"


BEGIN_MESSAGE_MAP(CHintListBox, CListBox)
    ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
    ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
END_MESSAGE_MAP()


/** Just initialize. Also set default hint format.*/
CHintListBox::CHintListBox() : 
    m_csh(ArcBase::defaultDesign, NULL)
{
    m_format.Default();
    m_format += "\\f(Courier New)\\mn(12)\\ms(8)\\pl";
    //m_format += "\\f(Arial)\\mn(24)\\ms(8)\\pl";
}

/** Needed to enable tooltips */
void CHintListBox::PreSubclassWindow()
{
    CListBox::PreSubclassWindow();
    EnableToolTips(true);
}

/** Tests if the mouse is in one of our items and gives the associated text.*/
int CHintListBox::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
{
    BOOL tmp = FALSE;
    const int row = ItemFromPoint(point, tmp);
    if (row == -1)
        return -1;

    //fill TOOLINFO  
    RECT rect;
    GetItemRect(row, &rect);
    pTI->rect = rect;
    pTI->hwnd = m_hWnd;
    pTI->uFlags &= ~TTF_IDISHWND;
    pTI->uId = UINT(row);
    pTI->lpszText = LPSTR_TEXTCALLBACK;
    return pTI->uId;
}


/**Handeles TTN_NEEDTEXTW and TTN_NEEDTEXTA events and returns the tooltip text.*/
BOOL CHintListBox::OnToolTipText(UINT id, NMHDR * pNMHDR, LRESULT * pResult)
{
    const CshHint *hint = GetHint(pNMHDR->idFrom);
    if (!hint) return FALSE;
    CString strTipText = hint->plain.c_str();

    //Convert to return format
    TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
    TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
#ifndef _UNICODE
    if (pNMHDR->code == TTN_NEEDTEXTA)
        lstrcpyn(pTTTA->szText, strTipText, 80);
    else
        _mbstowcsz(pTTTW->szText, strTipText, 80);
#else
    if (pNMHDR->code == TTN_NEEDTEXTA)
        _wcstombsz(pTTTA->szText, strTipText, 80);
    else
        lstrcpyn(pTTTW->szText, strTipText, 80);
#endif
    *pResult = 0;

    return TRUE;
}

/** Take a list of hints, filter & group them and copy them to us.
 * We empty 'csh' if no hinting is requested by the user preferences or
 * hints are not in HINT_READY state. Then we remove those types of
 * hints not automatically requested by the user (unless this hint session
 * is triggered by Ctrl+Space).
 * Then we call Csh::ProcessHints() to potentially filter hints not
 * matching the 'uc' and potentially to group them, like 'line.*'
 * @param csh Contains the list of hints we start from. We also return the
 *            filtered & grouped list of hints in it.
 * @param [in] uc The string under the cursor, that we are hinting on.
 * @param [in] userRequest True, if the hint session started via Ctrl+Space.
 * @param [in] afterReturnKey True, if the session started after pressing return (we may show the first line hints).
 * @returns True, if the list of hints has changed.*/
bool CHintListBox::PreprocessHints(Csh &csh, const std::string &uc, bool userRequest, bool afterReturnKey)
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    if (!pApp->m_bHints || csh.hintStatus != HINT_READY)
        csh.Hints.clear();
    else {
        //Now delete those hints not requested by the user preferences,
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
        CDC* pDC = GetDC();
        {
            Canvas canvas(Canvas::WIN, pDC->m_hDC, Block(0,HINT_GRAPHIC_SIZE_X, 0,HINT_GRAPHIC_SIZE_Y));
            csh.ProcessHints(canvas, &m_format, uc, pApp->m_bHintFilter,
                             pApp->m_bHintCompact);
            //Destroy canvas before the DC
        }
        ReleaseDC(pDC);
    }
    bool changed = csh.Hints != m_csh.Hints;
    //save so much of "csh" as is needed to draw the hints & symbols
    m_csh.Hints = csh.Hints; 
    m_csh.pShapes = csh.pShapes;
    return changed;
}

/** This number of hints are visible in the hint box.*/
#define MAX_HINT_LISTBOX_LEN 10
/** Copies the hints from 'm_csh' to the list box itself.
 * @returns the resulting pixel size of the list box.*/
CSize CHintListBox::SetHintsToCurrent()
{
    m_current_size.SetSize(0,0);
    if (m_csh.Hints.size()==0) {
        m_cur_sel = -1;
        ResetContent();
        return m_current_size;
    }
    SetRedraw(false);
    //Copy hints to the listbox and resize
    ResetContent();
    int added = 0;
    for (const auto &h : m_csh.Hints) {
        AddString(h.plain.c_str());
        if (h.x_size > m_current_size.cx)
            m_current_size.cx = h.x_size;
        if (added<MAX_HINT_LISTBOX_LEN)
            m_current_size.cy += std::max(h.y_size, HINT_GRAPHIC_SIZE_Y);
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

/** Return the box of 'p' as a CRect.*/
inline CRect ConvertABCDToCRect(const CshHint *p) 
{
    return CRect(p->ul_x-1, p->ul_y-1, p->br_x+1, p->br_y+1);
}

/** Select the right item based on what is under the cursor,
 * that is, the longest matching item.*/
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
            if (len <= match_len) continue;
            match_index = i;
            match_len = len;
        }
        if (match_len==uc_len) 
            //if string under cursor is a prefix of the longest mathcing hint, 
            //we select fully (if the hint is selectable)
            ChangeSelectionTo(match_index);
        else 
            //if the string under the cursor differs by its end from the hint,
            //we selet conly halfway.
            ChangeSelectionTo(match_index, HINT_ITEM_SELECTED_HALF);
    } else
        //no string under cursor: select nothing
        ChangeSelectionTo(-1, HINT_ITEM_NOT_SELECTED); //second param is dummy
}

/** Step uwards or downwards in the list.
  * @param [in] offset +-1 for up/down arrow, +-2 for pgup/dn. */
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

/** Change the selection to 'index'. 
 * Range safe, if index is out of bounds, we deselect.
 * Also, we take into account if that index is selectable or not and 
 * apply 'half' selection if not.*/
void CHintListBox::ChangeSelectionTo(int index)
{
    if (index>=0 && index<GetCount()) 
        ChangeSelectionTo(index, GetHint(index)->selectable ? HINT_ITEM_SELECTED : HINT_ITEM_SELECTED_HALF);
    else 
        ChangeSelectionTo(index, HINT_ITEM_NOT_SELECTED); //second param is dummy
}

/** Change the selection to 'index', with state 'state'.
* Range safe, if index is out of bounds, we deselect.*/
void CHintListBox::ChangeSelectionTo(int index, EHintItemSelectionState state)
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


/** Search the current list of hints in 'm_csh' and return the one
 * equal to 'plain'.*/
const CshHint* CHintListBox::GetHint(const char *plain) const
{
    auto i = m_csh.Hints.begin();
    while (i!=m_csh.Hints.end())
        if (!strcmp(i->plain.c_str(), plain))
            break;
        else
            i++;
    _ASSERT(i!=m_csh.Hints.end());
    if (i==m_csh.Hints.end()) return NULL;
    return &*i;
}

/** Onwer-draw a list item.*/
void CHintListBox::DrawItem(LPDRAWITEMSTRUCT lpItem)
{
    //Do not draw focus rectangle (or anything else) for empty lists
    if (lpItem->itemID==-1) return;
    //We do not differentiate in appearance whether we have the focus or not,
    //so for ODA_FOCUS, nothing to do
    if (lpItem->itemAction != ODA_DRAWENTIRE && lpItem->itemAction != ODA_SELECT) return;
    
    const CshHint * const item = GetHint((char*)lpItem->itemData);
    if (item==NULL) return;

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
    Canvas canvas(Canvas::WIN, memDC.m_hDC);
    Label label(item->decorated, canvas, m_format);
    const ColorType black(0,0,0);
    const FillAttr fill(black.Lighter(0.75), GRADIENT_DOWN);
    const LineAttr line(LINE_SOLID, black.Lighter(0.5), 1, CORNER_ROUND, 3);
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
        item->callback(&canvas, item->param, m_csh);
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


/** Return the geometry of one item.*/
void CHintListBox::MeasureItem(LPMEASUREITEMSTRUCT lpItem)
{
    const CshHint * const item = GetHint((char*)lpItem->itemData);
    if (item) {
        lpItem->itemWidth = item->x_size;
        lpItem->itemHeight = item->y_size;
    }
}

/** Called by the framework to sort the items.
 * Selectable items always come first, then we sort according to 'plain'.
 * @return -1, if item 1 sorts before item 2; 0, if item 1 and item 2 sort 
 * the same; and 1, if item 1 sorts after item 2.*/
int CHintListBox::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct)
{
    // return -1 = item 1 sorts before item 2
    // return 0 = item 1 and item 2 sort the same
    // return 1 = item 1 sorts after item 2
    //find the hint in m_csh
    const CshHint * const item1 = GetHint((char*)lpCompareItemStruct->itemData1);
    if (!item1) return 0;
    const CshHint * const item2 = GetHint((char*)lpCompareItemStruct->itemData2);
    if (!item2) return 0;

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

/** Just initialize.
 * We take a pointer to the internal editor, to be able to move
 * focus back to it and to be able to order the insertion of the
 * selected hint text into the editor.*/
CPopupList::CPopupList(CWnd* pParent, CCshRichEditCtrl* pEditCtrl)
	: CDialog(CPopupList::IDD, pParent)
{
    m_pEditCtrl = pEditCtrl;
    m_shown=false;
}

/** Standard dialog stuff. */
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


/** Show the hints. 
 * Copy the hints from the list box's m_csh to the listbox,
 * select the appropriate one for 'uc' and position the window.
 * @param [in] uc The text in the editor, under the cursor, we are hintin based on.
 * @param [in] x The coordinate where we should appear.
 * @param [in] y The coordinate where we should appear.*/
void CPopupList::Show(const LPCSTR uc, int x, int y)
{
    if (m_listBox.m_csh.Hints.size()==0) {
        Hide();
        return;
    }
    CSize size = m_listBox.SetHintsToCurrent();
    SetWindowPos(&CWnd::wndTop, x, y, size.cx+3, size.cy+3, SWP_NOOWNERZORDER | SWP_SHOWWINDOW);
    m_listBox.SetStringUnderCursor(uc);
    m_shown=true;
}

/** Hide the hints, move focus to the internal editor.*/
void CPopupList::Hide() 
{
    if (m_shown) {
        ShowWindow(SW_HIDE); 
        m_listBox.ResetContent();
        m_listBox.m_csh.Hints.clear();
        m_shown=false; 
    }
    m_pEditCtrl->SetFocus();
}

/** Called by the framework if the user double-clicks an item.
 * We insert that to the chart text via CCshRichEditCtrl::ReplaceHintedString()*/
void CPopupList::OnLbnDblclkList2()
{
    const CshHint *item = m_listBox.GetSelectedHint();
    if (item && item->state == HINT_ITEM_SELECTED) 
        m_pEditCtrl->ReplaceHintedString(item->plain.c_str(), !item->keep);
}


/** Called by the framework if the user cancels a selection.*/
void CPopupList::OnLbnSelcancelList2()
{
    m_listBox.ChangeSelectionTo(-1);
}


/** Called by the framework if the user changes the selection.*/
void CPopupList::OnLbnSelchangeList2()
{
    m_listBox.ChangeSelectionTo(m_listBox.GetCurSel());
}
