/*
    This file is part of Msc-generator.
	Copyright 2008,2009,2010,2011,2012 Zoltan Turanyi
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
// MscGenView.cpp : implementation of the CMscGenView class
//

#include "stdafx.h"
#include "Msc-generator.h"

#include "MscGenDoc.h"
#include "MscGenView.h"
#include "SrvrItem.h"
#include "cairo.h"
#include "cairo-win32.h"

#include <string>
#include <cmath>
#include <utility>
#include "area.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMscGenView

IMPLEMENT_DYNCREATE(CMscGenView, CScrollView)

BEGIN_MESSAGE_MAP(CMscGenView, CScrollView)
	ON_WM_MOUSEWHEEL() 
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_KEYUP()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMscGenView::OnFilePrintPreview)
	ON_COMMAND(ID_VIEW_REDRAW, OnViewRedraw)
	ON_COMMAND(ID_VIEW_RESETASPECTRATIO_INPLACE, ResetAspectRatioInPlace)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESETASPECTRATIO_INPLACE, OnUpdateResetAspectRatioInPlace)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DROPFILES()
	ON_WM_LBUTTONDOWN()
//    ON_WM_ACTIVATE()
ON_COMMAND(ID_VIEW_SHOWELEMENTCONTROLS, &CMscGenView::OnViewShowElementControls)
ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWELEMENTCONTROLS, &CMscGenView::OnUpdateViewShowElementControls)
END_MESSAGE_MAP()

// CMscGenView construction/destruction

CMscGenView::CMscGenView() : m_size(0,0)
{
	m_size.SetSize(0,0);
	// construction code here
	m_DeleteBkg = false;
	m_stretch_x = m_stretch_y = 1;
	m_FadingTimer = NULL;
	SetScrollSizes(MM_TEXT, m_size);
	m_nDropEffect = DROPEFFECT_NONE;
	m_clicked = false;
}

CMscGenView::~CMscGenView()
{
}

BOOL CMscGenView::PreCreateWindow(CREATESTRUCT& cs)
{
	// Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

// CMscGenView drawing

void CMscGenView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	CScrollView::OnPrepareDC(pDC, pInfo);
	if (SizeEmpty(m_size)) return;
	CMscGenDoc* pDoc = GetDocument();
	if (pDoc==NULL) return; 

	if (pDoc->IsInPlaceActive()) {
		pDC->SetMapMode(MM_ANISOTROPIC);
		pDC->SetWindowExt(m_size);

		CSize sizeNum, sizeDenom;
		pDoc->GetZoomFactor(&sizeNum, &sizeDenom);

		int xLogPixPerInch = pDC->GetDeviceCaps(LOGPIXELSX);
		int yLogPixPerInch = pDC->GetDeviceCaps(LOGPIXELSY);

		long xExt = (long)m_size.cx * xLogPixPerInch * sizeNum.cx;
		xExt /= 100 * (long)sizeDenom.cx;
		long yExt = (long)m_size.cy * yLogPixPerInch * sizeNum.cy;
		yExt /= 100 * (long)sizeDenom.cy;
		pDC->SetViewportExt((int)xExt, (int)yExt);
	} else {
		pDC->SetMapMode(MM_TEXT);
		pDC->SetWindowExt(ScaleSize(m_size, pDoc->m_zoom/100.0));
	}
}



void CMscGenView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_ESCAPE) {
		CMscGenDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		pDoc->SetTrackMode(false);
		//fallthrough to default handler, too
	}
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
	//If user has made modifications reflect them
	if (pDoc->m_ExternalEditor.IsRunning())
		pDoc->m_ExternalEditor.Restart(STOPEDITOR_WAIT);
	pDoc->SyncShownWithEditing("print");
    CDrawingChartData *pData = new CDrawingChartData(pDoc->m_ChartShown);
	pInfo->SetMaxPage(pData->GetPages()); //This one compiles copied chart
    pInfo->m_lpUserData = pData;
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMscGenView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// extra initialization before printing
}

void CMscGenView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
    CDrawingChartData *const pData = (CDrawingChartData *)pInfo->m_lpUserData;
    if (pData==NULL || pData->IsEmpty())
		return;

    CSize orig_size = pData->GetSize();
	double scale = double(pInfo->m_rectDraw.Width())/orig_size.cx;
    scale = std::min(scale, 10.);

    CWaitCursor wait;
	pData->SetPage(pInfo->m_nCurPage);
    pData->DrawToPrinter(pDC->m_hDC, scale, scale);

	//CRect r(0, 0, orig_size.cx*fzoom, orig_size.cy*fzoom);
	//HENHMETAFILE hemf = data.GetEMF(true);
	//ENHMETAHEADER header;
	//GetEnhMetaFileHeader(hemf, sizeof(header), &header);
	//r.SetRect(header.rclBounds.left*fzoom, header.rclBounds.top*fzoom,
	//	      header.rclBounds.right*fzoom, header.rclBounds.bottom*fzoom); 
	//PlayEnhMetaFile(pDC->m_hDC, hemf, r);
	//DeleteEnhMetaFile(hemf);
}

void CMscGenView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// cleanup after printing
}

void CMscGenView::OnRButtonUp(UINT /*nFlags*/, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMscGenView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
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

BOOL CMscGenView::OnEraseBkgnd(CDC * /*pDC*/)
{
	if (!m_DeleteBkg) return false;
	m_DeleteBkg = false;
	return true;
}

void CMscGenView::InvalidateBlock(const Block &b) 
{
	CMscGenDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	//pDoc->GetZoomFactor(&sizeNum, &sizeDenom);
	double scale = pDoc->m_zoom / 100.; 

	//double pageTop = pDoc->m_ChartShown.GetPageYShift();

	//CPoint point = GetDeviceScrollPosition();
	////inflate by 2 pixels, as track rects cover a bigger area
	////Shift so that we are in page coordinates
	////then scale to device units
	////then shift by the scroll position
    CRect r(int(b.x.from*scale-1), int(b.y.from*scale-1), int(b.x.till*scale+1), int(b.y.till*scale+1));
	//InvalidateRect(&r);
    Invalidate();
}

//Draw tracking rectangles and controls
//clip is understood as surface coordinates. scale tells me how much to scale m_size to get surface coords.
void CMscGenView::DrawTrackRects(CDC* pDC, CRect clip, double x_scale, double y_scale)
{
	CMscGenDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_trackArcs.size()==0 || pDC==NULL) return;
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	//Adjust clip for pDoc->m_nTrackBottomClip. We do not draw a tackrect onto the copyright line.
    clip.bottom = std::min(clip.bottom, (LONG)ceil(y_scale*pDoc->m_ChartShown.GetBottomWithoutCopyright()));
	//This is the destination surface
    MscCanvas canvas(MscCanvas::WIN, pDC->m_hDC, pDoc->m_ChartShown.GetMscTotal(), 0, XY(x_scale, y_scale));
    cairo_set_line_width(canvas.GetContext(), 1);
	for (auto i = pDoc->m_trackArcs.begin(); i!=pDoc->m_trackArcs.end(); i++) {
        if (i->what == TrackedArc::TRACKRECT) {
            cairo_set_source_rgba(canvas.GetContext(), GetRValue(pApp->m_trackFillColor)/255., 
                                                       GetGValue(pApp->m_trackFillColor)/255., 
                                                       GetBValue(pApp->m_trackFillColor)/255., 
                                                       GetAValue(pApp->m_trackFillColor)/255.*i->fade_value);
            i->arc->GetAreaToDraw().Fill(canvas.GetContext());
	        cairo_set_source_rgba(canvas.GetContext(), GetRValue(pApp->m_trackLineColor)/255., 
                                                       GetGValue(pApp->m_trackLineColor)/255., 
                                                       GetBValue(pApp->m_trackLineColor)/255., 
                                                       GetAValue(pApp->m_trackLineColor)/255.*i->fade_value);
            i->arc->GetAreaToDraw().Line(canvas.GetContext());
        } else if (i->what == TrackedArc::CONTROL && pApp->m_bShowControls) 
            i->arc->DrawControls(&canvas, i->fade_value);
    }
}


void CMscGenView::OnDraw(CDC* pDC)
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT_VALID(pApp);
	CMscGenDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (SizeEmpty(m_size)) return;
    CRect total;
    CRect clip;
    double x_scale, y_scale;
	if (pDoc->IsInPlaceActive()) {
        x_scale = double(pDC->GetViewportExt().cx)/m_size.cx;              
        y_scale = double(pDC->GetViewportExt().cy)/m_size.cy;              
        total.SetRect(CPoint(0, 0), CPoint(pDC->GetViewportExt()));
        clip = total;
	} else {
        x_scale = y_scale = pDoc->m_zoom/100.0;
		total = CRect(CPoint(0, 0), CPoint(int(m_size.cx*x_scale), int(m_size.cy*y_scale)));
		pDC->GetClipBox(&clip);
    }
    CPoint upper = GetScrollPosition();
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bitmap;
	CBitmap *oldBitmap;
    bitmap.CreateCompatibleBitmap(pDC, clip.Width(), clip.Height());
    oldBitmap = memDC.SelectObject(&bitmap);
    memDC.SetWindowOrg(clip.left, clip.top);
    memDC.FillSolidRect(clip, pDC->GetBkColor());
    m_cache.DrawToWindow(memDC.m_hDC, x_scale, y_scale, clip, pApp->m_bPB_Editing);
	DrawTrackRects(&memDC, clip, x_scale, y_scale);
    //pDC->SetMapMode(MM_TEXT);
    pDC->BitBlt(clip.left, clip.top, clip.Width(), clip.Height(), &memDC, clip.left, clip.top, SRCCOPY);   
	memDC.SelectObject(oldBitmap);
}

void CMscGenView::OnViewRedraw()
{
	CMscGenDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	pDoc->UpdateAllViews(NULL);
}

void CMscGenView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	//m_pDropTarget is a CWnd memeber and holds the droptarget object
	//If we are already registered, do not register again.
	if (!m_pDropTarget) m_DropTarget.Register(this);
}

void CMscGenView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	CMscGenDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT_VALID(pApp);

    m_cache.SetCacheType(pApp->m_cacheType);
    m_cache.SetData(&pDoc->m_ChartShown);

	//Delete the cached bitmap
	if (pDoc->m_ChartShown.IsEmpty()) {
		m_size.cx = m_size.cy = 0;
		m_DeleteBkg = true;
		Invalidate();
		return;
	}

	//Check if some of the background becomes visible (only if not in-place)
	CSize new_size = pDoc->m_ChartShown.GetSize();
	if (m_size.cx > new_size.cx || m_size.cy > new_size.cy)
		if (!pDoc->IsInPlaceActive())
			m_DeleteBkg = true;
	m_size = new_size;

	//readjust size if inplace active
	if (pDoc->IsInPlaceActive() && !SizeEmpty(m_size)) {
		CRect pos;
		pDoc->GetItemPosition(&pos);
		pos.right = pos.left + int(m_size.cx*m_stretch_x);
		pos.bottom = pos.top + int(m_size.cy*m_stretch_y);
		pDoc->RequestPositionChange(pos);
	}
	Invalidate();
	ResyncScrollSize();
}

//Zoom functions
BOOL CMscGenView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CMscGenDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	if (pDoc->DispatchMouseWheel(nFlags, zDelta, pt)) return TRUE;
	if (pDoc->IsInPlaceActive()) 
		return CView::OnMouseWheel(nFlags, zDelta, pt);
	return TRUE;
}


//returns true if point is within our rectangle. Does nothing otherwise
BOOL CMscGenView::DoMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CRect view;
	GetClientRect(&view);
	ScreenToClient(&pt);
	//Process message only if within our view
	if (!view.PtInRect(pt)) return FALSE;
	CMscGenDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	//if we fall within, and in-place active, do default action
	if (pDoc->IsInPlaceActive()) 
		return CView::OnMouseWheel(nFlags, zDelta, pt);
	//If no control, just scroll
	if (!(nFlags & MK_CONTROL)) {
		CRect client;
		GetClientRect(client);
		//scoll 10% of the view height (zDelta can be negative for up)
		const int amount = int(client.Height()*0.1*zDelta/WHEEL_DELTA);
		CPoint pos = GetScrollPosition();
		pos.y -= amount;
		if (pos.y<0) pos.y = 0;
		if (pos.y + client.bottom > m_size.cy*pDoc->m_zoom/100)
			pos.y = m_size.cy*pDoc->m_zoom/100 - client.bottom;
		//See that we do not go out of the map
		ScrollToPosition(pos);
		return TRUE;
	}

	//Change zoom, but keep point under mouse stationary
	unsigned zoom = unsigned(pDoc->m_zoom*(1+float(zDelta)/WHEEL_DELTA/10));  //10% per wheel tick
	if (zoom < 10) zoom = 10;
	if (zoom > 10000) zoom = 10000;
	if (zoom == pDoc->m_zoom) 
		return TRUE;
	CPoint pos = GetDeviceScrollPosition() + pt;
	pos.x = int(double(pos.x)/pDoc->m_zoom * zoom);
	pos.y = int(double(pos.y)/pDoc->m_zoom * zoom);
	pos -= pt;
	if (pos.x < 0) pos.x = 0;
	if (pos.y < 0) pos.y = 0;
	SetRedraw(false);
	pDoc->SetZoom(zoom);
	ScrollToDevicePosition(pos);
	SetRedraw(true);
	Invalidate();
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

void CMscGenView::OnSize(UINT /*nType*/, int cx, int cy)
{ 
	CMscGenDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	//Calculate stretch (used only for in-place editing)
	if (!SizeEmpty(m_size)) {
		m_stretch_x = double(cx)/m_size.cx;
		m_stretch_y = double(cy)/m_size.cy;
	} else 
		m_stretch_x = m_stretch_y = 1;
	if (!pDoc->IsInPlaceActive() && pDoc->m_ZoomMode == CMscGenDoc::ZOOM_WIDTH)
		pDoc->ArrangeViews();
	else 
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
	sizeNew.cx *= int(scale/2);
	sizeNew.cy *= int(scale/2);
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

void CMscGenView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CScrollView::OnLButtonDown(nFlags, point);
	CMscGenDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	if (1) {
		if (pDoc->IsInPlaceActive()) return;
		//Mouse drag scrolling
		m_DragPoint = point;
		m_DragStartPoint = point;
		SetCapture();
	} else {
		//This is what we had to do for drag and drop
		//Copy is handled by SrvItem
		CMscGenSrvrItem *pItem = reinterpret_cast<CMscGenSrvrItem*>(pDoc->GetEmbeddedItem());
		TRY {
			COleDataSource *pDataSource = pItem->OnGetClipboardData(FALSE, NULL, NULL);
			if (pDataSource) pDataSource->DoDragDrop(DROPEFFECT_COPY);
		} CATCH_ALL(e) {
			//Do nothing if error happened in GetDataSource
		}
		END_CATCH_ALL
	}
}

void CMscGenView::OnMouseMove(UINT nFlags, CPoint point)
{
	CMscGenDoc *pDoc = GetDocument();
	if (pDoc == NULL) return CScrollView::OnMouseMove(nFlags, point);
	//If we are in drag mode
	if (GetCapture() == this) { 
		//allow only horizontal or vertical scrolling if SHIFT is down
		if (nFlags & MK_SHIFT) {
			//Keep that coordinate at start value that has changed less
			if (abs(point.x - m_DragStartPoint.x) > abs(point.y - m_DragStartPoint.y))
				point.y = m_DragStartPoint.y;
			else 
				point.x = m_DragStartPoint.x;
		}
		CPoint pos = GetScrollPosition();
		pos -= point - m_DragPoint;
		m_DragPoint = point;
		if (pos.x<0) pos.x = 0;
		if (pos.y<0) pos.y = 0;
		CRect client;
		GetClientRect(client);
		if (pos.x + client.right > m_size.cx*pDoc->m_zoom/100)
			pos.x = m_size.cx*pDoc->m_zoom/100 - client.right;
		if (pos.y + client.bottom > m_size.cy*pDoc->m_zoom/100)
			pos.y = m_size.cy*pDoc->m_zoom/100 - client.bottom;
		ScrollToPosition(pos);
		//Now apply the same x scroll for the other view(s)
		POSITION p = pDoc->GetFirstViewPosition();
		while (p != NULL) {
			CMscGenView* pView = static_cast<CMscGenView*>(pDoc->GetNextView(p));
			if (pView != this) pView->Invalidate();
		}
	} else {
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_HOVER;
		tme.dwHoverTime = 10;
		TrackMouseEvent(&tme);
	}
	CScrollView::OnMouseMove(nFlags, point);
} 

void CMscGenView::OnMouseHover(UINT nFlags, CPoint point)
{
	CMscGenDoc *pDoc = GetDocument();
	if (pDoc == NULL) return CScrollView::OnMouseHover(nFlags, point);
	//updateTrackRects expects the point to be in the native chart coordinate space as used by class MscDrawer.
	//Distort for embedded objects, for windowed ones (MM_TEXT) cater for scrolling
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);
	//take zooming into account.
	point.x = int(point.x*100./pDoc->m_zoom);
	point.y = int(point.y*100./pDoc->m_zoom);
	pDoc->UpdateTrackRects(point);
	m_hoverPoint = point;
}

void CMscGenView::OnLButtonUp(UINT nFlags, CPoint point)
{
	const double delay_before_fade = 500; //in ms
	CMscGenDoc *pDoc = GetDocument();
	if (pDoc == NULL) return;
	//If we were doing dragging stop it
	if (GetCapture() == this) {
		ReleaseCapture();
		//If we did actually drag, stop here do not highlight arc below cursor
		if (point != m_DragStartPoint)
			return;
	}

	//updateTrackRects expects the point to be in the native chart coordinate space as used by class MscDrawer.
	//Distort for embedded objects, for windowed ones (MM_TEXT) cater for scrolling
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);
	//then take zooming into account.
	point.x = int(point.x*100./pDoc->m_zoom);
	point.y = int(point.y*100./pDoc->m_zoom);
    for (auto i = pDoc->m_controlsShowing.begin(); i!=pDoc->m_controlsShowing.end(); i++)
        if (inside(i->first.IsWithin(XY(point.x, point.y))))
            if (i->second)
                if (pDoc->OnControlClicked(i->second, i->second->WhichControl(XY(point.x, point.y))))
                    return; //Chart and "i" is no longer valid pDoc->m_trackArc and m_controlsShown are empty
	if (pDoc->m_bTrackMode) {
		m_clicked=true;
		pDoc->UpdateTrackRects(point);
	} else {
		TrackableElement *arc = pDoc->m_ChartShown.GetArcByCoordinate(point);
		if (arc) {
			pDoc->StartFadingAll(NULL);
			pDoc->AddTrackArc(arc, TrackedArc::TRACKRECT, int(delay_before_fade));
			pDoc->HighLightArc(arc);
			CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
			ASSERT(pApp != NULL);
			if (pApp->IsInternalEditorRunning()) {
				pApp->m_pWndEditor->SetFocus();
                if (pDoc->m_saved_charrange.cpMin==-1)
                    pApp->m_pWndEditor->m_ctrlEditor.GetSel(pDoc->m_saved_charrange);
            }
		} else
			CScrollView::OnLButtonUp(nFlags, point);  //This sets focus to view
	}
}

void CMscGenView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CMscGenDoc *pDoc = GetDocument();
	if (pDoc == NULL) return;
    //First transform the point to the chart coordinate space
	//updateTrackRects expects the point to be in the native chart coordinate space as used by class MscDrawer.
	//Distort for embedded objects, for windowed ones (MM_TEXT) cater for scrolling
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);
	//then take zooming into account.
	point.x = int(point.x*100./pDoc->m_zoom);
	point.y = int(point.y*100./pDoc->m_zoom);

    //If the click is on an element that has controls, activate the first one
	TrackableElement *arc = pDoc->m_ChartShown.GetArcByCoordinate(point);
    if (arc && arc->GetControls().size()) {
        pDoc->OnControlClicked(arc, *arc->GetControls().begin());
        return; //Whole chart will be redrawn
    }
    //Dblclick not on an arc with controls=> enter tracking mode
	pDoc->SetTrackMode(true);
	pDoc->UpdateTrackRects(point);
	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CMscGenView::StartFadingTimer() 
{
	CMscGenDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	pDoc->m_pViewFadingTimer = this;
	if (m_FadingTimer) return;
	m_FadingTimer = SetTimer(1, FADE_TIMER, NULL);
}

void CMscGenView::OnTimer(UINT_PTR)
{
	CMscGenDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	if (pDoc->DoFading()) return;
	//If no more currently fading rects, kill timer
	KillTimer(1);
	pDoc->m_pViewFadingTimer = NULL;
	m_FadingTimer = NULL;
}

int CMscGenView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	// Accept files dropped on the view
	//Do not accept dropped files when in FullScreenViewMode
	if (!pApp->m_bFullScreenViewMode) 
		DragAcceptFiles(true);

	return 0;
}

void CMscGenView::OnDropFiles(HDROP hDropInfo)
{
	int nFiles = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);
	switch (nFiles) {
		default:
			AfxMessageBox("Sorry, I cannot handle multiple files dropped on me. Drop just one.");
			break;
		case 0:
			break; //do nothing
		case 1:
			CString s;
			LPTSTR pFileName = s.GetBufferSetLength(MAX_PATH);
			::DragQueryFile(hDropInfo, 0, pFileName, MAX_PATH);
			AfxGetApp()->OpenDocumentFile(s);
			break;
	}
	::DragFinish(hDropInfo);

	CScrollView::OnDropFiles(hDropInfo);
}

DROPEFFECT CMscGenView::OnDragEnter(COleDataObject* pDataObject, DWORD /*dwKeyState*/, CPoint /*point*/)
{
	CMscGenDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	ASSERT(pDataObject);
	if (pDataObject->IsDataAvailable(CF_TEXT) || 
		pDataObject->IsDataAvailable(pDoc->m_cfPrivate)) {
		m_nDropEffect = DROPEFFECT_COPY;
	} else
		m_nDropEffect = DROPEFFECT_NONE;
	return m_nDropEffect;
}

DROPEFFECT CMscGenView::OnDragOver(COleDataObject* /*pDataObject*/, DWORD /*dwKeyState*/, CPoint /*point*/)
{
	return m_nDropEffect;
}

void CMscGenView::OnDragLeave()
{
	m_nDropEffect = DROPEFFECT_NONE;
}

BOOL CMscGenView::OnDrop(COleDataObject* pDataObject, DROPEFFECT /*dropEffect*/, CPoint /*point*/)
{
	CMscGenDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	ASSERT(pDataObject);
	if (m_nDropEffect == DROPEFFECT_NONE) 
		return FALSE;
	pDoc->DoPasteData(*pDataObject);
	return TRUE;
}

//void CMscGenView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
//{
//    CScrollView::OnActivate(nState, pWndOther, bMinimized);
//
//    //Set focus to internal editor
//	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
//	ASSERT(pApp != NULL);
//    if (nState != WA_INACTIVE)
//        if (pApp->IsInternalEditorRunning() && pApp->m_pWndEditor->IsVisible())
//            pApp->m_pWndEditor->SetFocus();
//}


void CMscGenView::OnViewShowElementControls()
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    pApp->m_bShowControls = !pApp->m_bShowControls;
    Invalidate();
    pApp->WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_SHOW_CONTROLS, pApp->m_bShowControls);
}


void CMscGenView::OnUpdateViewShowElementControls(CCmdUI *pCmdUI)
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    pCmdUI->SetCheck(pApp->m_bShowControls);
}
