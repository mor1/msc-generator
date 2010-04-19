/*
    This file is part of Msc-generator.
	Copyright 2008,2009,2010 Zoltan Turanyi
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
// MscGenView.cpp : implementation of the CMscGenView class
//

#include "stdafx.h"
#include "Msc-generator.h"

#include "MscGenDoc.h"
#include "MscGenView.h"

#include <string>
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMscGenView

IMPLEMENT_DYNCREATE(CMscGenView, CScrollView)

BEGIN_MESSAGE_MAP(CMscGenView, CScrollView)
	ON_WM_MOUSEWHEEL() 
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_KEYUP()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_CANCEL_EDIT_SRVR, &CMscGenView::OnCancelEditSrvr)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMscGenView::OnFilePrintPreview)
	ON_COMMAND(ID_DESIGN_PAGE, OnDesignPage)
	ON_CBN_SELENDOK(ID_DESIGN_PAGE, OnDesignPage)
	ON_COMMAND(ID_DESIGN_DESIGN, OnDesignDesign)
	ON_CBN_SELENDOK(ID_DESIGN_DESIGN, OnDesignDesign)
	ON_COMMAND(ID_VIEW_REDRAW, OnViewRedraw)
	ON_COMMAND(ID_VIEW_RESETASPECTRATIO_INPLACE, ResetAspectRatioInPlace)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESETASPECTRATIO_INPLACE, OnUpdateResetAspectRatioInPlace)
	ON_COMMAND(ID_BUTTON_EDITTEXT, OnButtonEdittext)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_EDITTEXT, OnUpdateButtonEdittext)
END_MESSAGE_MAP()

// CMscGenView construction/destruction

CMscGenView::CMscGenView()
{
	CMscGenApp *pApp = (CMscGenApp *)::AfxGetApp();
	// construction code here
	m_hemf = NULL;
	m_DeleteBkg = false;
	m_hTimer = NULL;
	m_size.SetSize(0,0);
	SetScrollSizes(MM_TEXT, CSize(0,0));
}

CMscGenView::~CMscGenView()
{
	if (m_hemf)
		DeleteEnhMetaFile(m_hemf);
}

BOOL CMscGenView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

// CMscGenView drawing

void CMscGenView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	CMscGenDoc* pDoc = GetDocument();
	CScrollView::OnPrepareDC(pDC, pInfo);

	pDC->SetMapMode(MM_ANISOTROPIC);
	CSize sizeDoc(ScaleSize(m_size, pDoc->m_zoom/100.0));
	pDC->SetWindowExt(sizeDoc);

	CSize sizeNum, sizeDenom;
	pDoc->GetZoomFactor(&sizeNum, &sizeDenom);

	int xLogPixPerInch = pDC->GetDeviceCaps(LOGPIXELSX);
	int yLogPixPerInch = pDC->GetDeviceCaps(LOGPIXELSY);

	long xExt = (long)sizeDoc.cx * xLogPixPerInch * sizeNum.cx;
	xExt /= 100 * (long)sizeDenom.cx;
	long yExt = (long)sizeDoc.cy * yLogPixPerInch * sizeNum.cy;
	yExt /= 100 * (long)sizeDenom.cy;
	pDC->SetViewportExt((int)xExt, (int)yExt);
}


BOOL CMscGenView::OnEraseBkgnd(CDC *pDC)
{
	CMscGenDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->IsInPlaceActive())
		return TRUE;
	if (m_DeleteBkg) {
		m_DeleteBkg = false;
		return TRUE;
	}
	return FALSE;
}

void CMscGenView::OnDraw(CDC* pDC)
{
	CMscGenDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (m_hemf==NULL) return;
	CRect clip;
	pDC->GetClipBox(&clip);
	//m_zoom is always 100% when in place
	if (pDoc->IsInPlaceActive()) {
		pDC->FillSolidRect(clip, pDC->GetBkColor());
		CRect r(CPoint(0, 0), m_size);
		pDC->SetMapMode(MM_ANISOTROPIC);
		//pDC->SetViewportExt(m_size());
		//pDC->SetWindowExt(m_size());
		PlayEnhMetaFile(pDC->m_hDC, m_hemf, r);
	} else {
		CRect r(CPoint(0, 0), ScaleSize(m_size, pDoc->m_zoom/100.0));
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);
		CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(pDC, clip.Width(), clip.Height());
		memDC.SelectObject(&bitmap);
		memDC.SetWindowOrg(clip.left, clip.top);
		memDC.FillSolidRect(clip, pDC->GetBkColor());
		PlayEnhMetaFile(memDC.m_hDC, m_hemf, r);
		pDC->BitBlt(clip.left, clip.top, clip.Width(),  clip.Height(),
			        &memDC, clip.left, clip.top, SRCCOPY);   
	}
	return;
	//if (m_hwmf) {
	//  CSize size(r.right, r.bottom);
	//	pDC->SetMapMode(MM_ANISOTROPIC);
	//	pDC->SetWindowExt(size);
	//	pDC->SetViewportExt(size);
	//	PlayMetaFile(pDC->m_hDC, m_hwmf);
	//}
}

void CMscGenView::OnViewRedraw()
{
	CMscGenDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	pDoc->UpdateAllViews(NULL);
}

void CMscGenView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar) {
		case VK_HOME:  OnScroll(SB_TOP*256       + SB_ENDSCROLL, 0); break;
		case VK_END:   OnScroll(SB_BOTTOM*256    + SB_ENDSCROLL, 0); break;
		case VK_PRIOR: OnScroll(SB_PAGEUP*256    + SB_ENDSCROLL, 0); break;
		case VK_NEXT:  OnScroll(SB_PAGEDOWN*256  + SB_ENDSCROLL, 0); break;
		case VK_UP:    OnScroll(SB_LINEUP*256    + SB_ENDSCROLL, 0); break;
		case VK_DOWN:  OnScroll(SB_LINEDOWN*256  + SB_ENDSCROLL, 0); break;
		case VK_LEFT:  OnScroll(SB_ENDSCROLL*256 + SB_LINELEFT,  0); break;
		case VK_RIGHT: OnScroll(SB_ENDSCROLL*256 + SB_LINERIGHT, 0); break;
		default: CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
	}
}
// CMscGenView printing


void CMscGenView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CMscGenView::OnPreparePrinting(CPrintInfo* pInfo)
{
	CMscGenDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pInfo->SetMaxPage(pDoc->m_pages);
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMscGenView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* pInfo)
{
	// extra initialization before printing
	CMscGenDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_itrCurrent->IsEmpty())
		return;
}

void CMscGenView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	CMscGenDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_itrCurrent->IsEmpty())
		return;

    CSize orig_size = pDoc->m_itrCurrent->GetSize(pInfo->m_nCurPage);
	double fzoom = double(pInfo->m_rectDraw.Width())/orig_size.cx;
	CRect r(0, 0, orig_size.cx*fzoom, orig_size.cy*fzoom);

	HDC hdc = CreateEnhMetaFile(NULL, NULL, NULL, NULL);
	pDoc->m_itrCurrent->Draw(hdc, DRAW_EMF, 100, pInfo->m_nCurPage);
	HENHMETAFILE hemf = CloseEnhMetaFile(hdc);
	ENHMETAHEADER header;
	GetEnhMetaFileHeader(hemf, sizeof(header), &header);
	r.SetRect(header.rclBounds.left*fzoom, header.rclBounds.top*fzoom,
		      header.rclBounds.right*fzoom, header.rclBounds.bottom*fzoom); 
	PlayEnhMetaFile(pDC->m_hDC, hemf, r);
	DeleteEnhMetaFile(hemf);
}

void CMscGenView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// cleanup after printing
}

void CMscGenView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMscGenView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}

// OLE Server support

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.  Here,
//  the server (not the container) causes the deactivation
void CMscGenView::OnCancelEditSrvr()
{
	GetDocument()->OnDeactivateUI(FALSE);
}


// CMscGenView diagnostics

#ifdef _DEBUG
void CMscGenView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMscGenView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMscGenDoc* CMscGenView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMscGenDoc)));
	return (CMscGenDoc*)m_pDocument;
}
#endif //_DEBUG

// CMscGenView message handlers

void CMscGenView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	CMscGenDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (m_hemf) {
		DeleteEnhMetaFile(m_hemf);
		m_hemf = NULL;
	}

    HDC hdc = CreateEnhMetaFile(NULL, NULL, NULL, NULL);
	pDoc->m_itrCurrent->Draw(hdc, DRAW_EMF, 100, pDoc->m_page);
	m_hemf = CloseEnhMetaFile(hdc);

	//Check if some of the background becomes visible
	CSize new_size = pDoc->m_itrCurrent->GetSize(pDoc->m_page);
	if (m_size.cx > new_size.cx || m_size.cy > new_size.cy)
		m_DeleteBkg = true;

	//readjust size if inplace active
	if (pDoc->IsInPlaceActive() && !SizeEmpty(new_size)) {
		if (!SizeEmpty(m_size)) {
			CRect oldPos;
			pDoc->GetItemPosition(&oldPos);
			CSize sizeOld = oldPos.Size();
			CSize sizeNew(sizeOld.cx*new_size.cx/m_size.cy, sizeOld.cy*new_size.cy/m_size.cy);
			CRect newPos(oldPos.TopLeft(), oldPos.TopLeft()+sizeNew);
			pDoc->RequestPositionChange(newPos);
		}
	}
	m_size = new_size;
	Invalidate();
	ResyncScrollSize();
}

//Zoom functions
BOOL CMscGenView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CMscGenDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	if (pDoc->IsInPlaceActive()) 
		return CView::OnMouseWheel(nFlags, zDelta, pt);
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL) {
        CMscGenView* pView = static_cast<CMscGenView*>(pDoc->GetNextView(pos));
  	    if (pView->IsKindOf(RUNTIME_CLASS(CMscGenView))) {
			//Each view handles this message only if pt is within its area
	        pView->DoMouseWheel(nFlags, zDelta, pt);
		}
	}
	return TRUE;
}


BOOL CMscGenView::DoMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CMscGenDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	CRect view;
	GetClientRect(&view);
	ScreenToClient(&pt);
	//Process message only if within our view
	if (!view.PtInRect(pt)) return TRUE;
	if (!(nFlags & MK_CONTROL)) {
		if (pt.x > view.right || pt.x < 0 || 
			pt.y > view.bottom || pt.y <0) return TRUE; //Do nothing if outside client area

		if (zDelta>0) OnScroll(SB_PAGEUP*256    + SB_ENDSCROLL, 0); 
		if (zDelta<0) OnScroll(SB_PAGEDOWN*256  + SB_ENDSCROLL, 0);
		return TRUE;
	}
	unsigned zoom = pDoc->m_zoom*(1+float(zDelta)/WHEEL_DELTA/10);  //10% per wheel tick
	if (zoom < 10) zoom = 10;
	if (zoom > 10000) zoom = 10000;
	if (zoom == pDoc->m_zoom) 
		return TRUE;
	CPoint pos = GetDeviceScrollPosition() + pt;
	pos.x = double(pos.x)/pDoc->m_zoom * zoom;
	pos.y = double(pos.y)/pDoc->m_zoom * zoom;
	pos -= pt;
	if (pos.x < 0) pos.x = 0;
	if (pos.y < 0) pos.y = 0;
	pDoc->SetZoom(zoom);
	ScrollToDevicePosition(pos);
	return TRUE;
}

void CMscGenView::ResyncScrollSize(void)
{
	CMscGenDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	if (SizeEmpty(m_size)) {
		SetScrollSizes(MM_TEXT, CSize(0,0));
		return;
	}
	if (pDoc->IsInPlaceActive()) {
		//Set the doc size equal to the view size: no scrollbars
		CRect posView;
		pDoc->GetItemPosition(&posView);
		SetScrollSizes(MM_TEXT, posView.Size());
	} else {
		SetScrollSizes(MM_TEXT, ScaleSize(m_size, pDoc->m_zoom/100.0));
	}
}

void CMscGenView::OnSize(UINT nType, int cx, int cy)
{
	ResyncScrollSize(); 
}

void CMscGenView::ResetAspectRatioInPlace(void)
{
	CMscGenDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	if (!pDoc->IsInPlaceActive() || SizeEmpty(m_size)) return;
	CRect oldPos;
	pDoc->GetItemPosition(&oldPos);
	CSize sizeOld = oldPos.Size();
	CSize sizeNew(m_size);
	CClientDC dc(this);
	dc.LPtoDP(&sizeNew);
	dc.LPtoDP(&sizeOld);
	double oldArea = double(sizeOld.cx) * double(sizeOld.cy);
	double newArea =  double(sizeNew.cx) * double(sizeNew.cy);
	double scale = sqrt(oldArea / newArea); //Existing scale from Chart area to Client Area
	sizeNew.cx *= scale/2;
	sizeNew.cy *= scale/2;
	//Keep centerpoint
	CRect newPos(oldPos.CenterPoint()-sizeNew, oldPos.CenterPoint()+sizeNew);
	pDoc->RequestPositionChange(newPos);
	Invalidate();
}

void CMscGenView::OnUpdateResetAspectRatioInPlace(CCmdUI *pCmdUI)
{
	CMscGenDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	pCmdUI->Enable(pDoc->IsInPlaceActive() && !SizeEmpty(m_size));
}

//Page functions
void CMscGenView::OnDesignPage()
{
	CObList list;
	CMFCToolBar::GetCommandButtons(ID_DESIGN_PAGE, list);

	POSITION p = list.GetHeadPosition();
	if (!p) return;
	CMFCToolBarComboBoxButton *combo = static_cast<CMFCToolBarComboBoxButton*>(list.GetNext(p));
	int index = combo->GetCurSel();
	//Set this setting on oll remaining ones
	while (p) {
		CMFCToolBarComboBoxButton *combo = static_cast<CMFCToolBarComboBoxButton*>(list.GetNext(p));
		combo->SelectItem(index);
	}
	CMscGenDoc *pDoc = GetDocument();
	if (pDoc == NULL) return;
	unsigned oldPage = pDoc->m_page;
	pDoc->m_page = index;
	if (pDoc->m_page == oldPage)
		return;
	//We save forceddesign only in an embedded doc, so changing that counts as modified
	if (pDoc->IsEmbedded())
		pDoc->SetModifiedFlag(); 
	pDoc->NotifyChanged();  //for OLE
	pDoc->UpdateAllViews(NULL);
}

void CMscGenView::OnTimer(UINT nIDEvent)
{
	CMscGenDoc *pDoc = GetDocument();
	ASSERT(pDoc);

	//Check for a modified file (if editor saved file & closed)
	//and for a missing editor process
	//Timer is shut down by MscGenDoc::StopEditor, called if editor process is gone
	pDoc->CheckEditorAndFile();
}

void CMscGenView::OnButtonEdittext()
{
	CMscGenDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	if (pDoc->m_EditorProcessId)
		pDoc->StopEditor(STOPEDITOR_NOWAIT);
	else
		pDoc->StartEditor();
}

void CMscGenView::OnUpdateButtonEdittext(CCmdUI *pCmdUI)
{
	CMscGenDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	pCmdUI->SetCheck(pDoc->m_EditorProcessId != 0);
}

//Design combo functions
void CMscGenView::OnDesignDesign() 
{
	CObList list;
	CMFCToolBar::GetCommandButtons(ID_DESIGN_DESIGN, list);
	POSITION p = list.GetHeadPosition();
	if (!p) return;
	CMFCToolBarComboBoxButton *combo = static_cast<CMFCToolBarComboBoxButton*>(list.GetNext(p));
	unsigned index = combo->GetCurSel();
	//Set this setting on oll remaining ones
	while (p) {
		CMFCToolBarComboBoxButton *combo = static_cast<CMFCToolBarComboBoxButton*>(list.GetNext(p));
		combo->SelectItem(index, TRUE);
	}
	CMscGenDoc *pDoc = GetDocument();
	if (pDoc == NULL) return;
	CString old_forcedDesign(pDoc->m_itrCurrent->GetDesign());
	CString new_forcedDesign; 
	if (index > 0)
		new_forcedDesign = combo->GetItem(index);
	if (new_forcedDesign == old_forcedDesign)
		return;
	pDoc->StartDrawingProgress();
	if (pDoc->IsEmbedded())
		pDoc->SetModifiedFlag();
	pDoc->m_itrCurrent->SetDesign(new_forcedDesign);
	pDoc->OnUpdate(false);  //Do not change zoom, juts update views
	pDoc->StopDrawingProgress();
}
