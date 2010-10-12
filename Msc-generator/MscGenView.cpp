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
#include "SrvrItem.h"
#include "cairo.h"
#include "cairo-win32.h"

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
	ON_WM_ERASEBKGND()
	ON_WM_KEYUP()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_CANCEL_EDIT_SRVR, &CMscGenView::OnCancelEditSrvr)
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
END_MESSAGE_MAP()

// CMscGenView construction/destruction

CMscGenView::CMscGenView() : m_size(0,0)
{
	m_size.SetSize(0,0);
	m_cachedBitmapClip.SetRectEmpty();
	m_cachedBitmapZoom = 100;
	// construction code here
	m_DeleteBkg = false;
	m_stretch_x = m_stretch_y = 1;
	m_FadingTimer = NULL;
	SetScrollSizes(MM_TEXT, m_size);
	m_nDropEffect = DROPEFFECT_NONE;
	m_pl += Block(30,50,60,70);
	m_pl += MscPolygon(XY(50,90), XY(100,60), XY(40,20));
	m_pl += Block(130,150,60,70);
	m_pl += Block(150,170,60,70);
	m_pl += Block(150,170,70,80);
}

CMscGenView::~CMscGenView()
{
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
	pInfo->SetMaxPage(pDoc->m_ChartShown.GetPages());
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMscGenView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* pInfo)
{
	// extra initialization before printing
}

void CMscGenView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	CMscGenDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_ChartShown.IsEmpty())
		return;

	CWaitCursor wait;
	CDrawingChartData data(pDoc->m_ChartShown);
	data.SetPage(pInfo->m_nCurPage);

    CSize orig_size = data.GetSize(); //This one compiles
	double fzoom = double(pInfo->m_rectDraw.Width())/orig_size.cx;
	CRect r(0, 0, orig_size.cx*fzoom, orig_size.cy*fzoom);

	HENHMETAFILE hemf = data.GetEMF();
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
	//We comment it out as it does not handle CDockablePanes well...
	//So we'd rather not deactivate from the server
	//GetDocument()->OnDeactivateUI(FALSE);
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

BOOL CMscGenView::OnEraseBkgnd(CDC *pDC)
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
	double xFactor = pDoc->m_zoom / 100.; 
	double yFactor = pDoc->m_zoom / 100.; 

	double pageTop = pDoc->m_ChartShown.GetPageYShift();

	CPoint point = GetDeviceScrollPosition();
	//inflate by 2 pixels, as track rects cover a bigger area
	//Shift so that we are in page coordinates
	//then scale to device units
	//then shift by the scroll position
	CRect r((b.x.from-2)*xFactor - point.x, (b.y.from-pageTop-2)*yFactor - point.y, 
		    (b.x.till+4)*xFactor - point.x, (b.y.till-pageTop+4)*yFactor - point.y);
//	InvalidateRect(&r);
	Invalidate();
}

//Draw covers to the dest surface
//only draw normal ones, skip frames
void AddTrackRectsToSurface(const Geometry &geometry, double alpha, cairo_t *cr_dest, 
							double xScale, double yScale, COLORREF lineColor, COLORREF fillColor)
{
	//Calculate line width
	int lwx = floor(xScale+0.5);
	if (lwx<1) lwx = 1;
	int lwy = floor(yScale+0.5);
	if (lwy<1) lwy = 1;
	Block b;
	geometry.GetBoundingBox(b);
	b.x.from = (b.x.from-2) * xScale;
	b.x.till = (b.x.till+4) * xScale;
	b.y.from = (b.y.from-2) * yScale;
	b.y.till = (b.y.till+4) * yScale;
	//create fill surface
	//We make it lwx, lwy larger at top and left, so xor operations are not truncated there
	cairo_surface_t *surface_fill = cairo_image_surface_create(CAIRO_FORMAT_A8, b.x.Spans()+lwx, b.y.Spans()+lwy);
	cairo_t *cr_fill = cairo_create(surface_fill);
	cairo_set_source_rgba(cr_fill, 1, 1, 1, 1);
	
	for (std::set<Block>::const_iterator j = geometry.GetCover().begin(); j!=geometry.GetCover().end(); j++) {
		CRect rr;
		rr.left   = j->x.from * xScale - 2*lwx;
		rr.right  = j->x.till * xScale + 2*lwx;
		rr.top    = j->y.from * yScale - 2*lwy;
		rr.bottom = j->y.till * yScale + 2*lwy;
		cairo_rectangle(cr_fill, rr.left - b.x.from + lwx, rr.top - b.y.from + lwy, rr.Width(), rr.Height());
		//Here j->drawType can only be NORMAL. FFRAMEs are handled in CMscGenView::DrawTrackRects
		cairo_fill(cr_fill);
	}	
	cairo_destroy(cr_fill);

	//create outline surface  (we make it lwx, lwy larger...)
	cairo_surface_t *surface_line = cairo_image_surface_create(CAIRO_FORMAT_A8, b.x.Spans()+lwx, b.y.Spans()+lwy);
	cairo_t *cr_line = cairo_create(surface_line);
	cairo_set_operator(cr_line, CAIRO_OPERATOR_XOR);
	cairo_set_source_rgba(cr_line, 1, 1, 1, 1);
	cairo_mask_surface(cr_line, surface_fill, 0, 0);
	cairo_mask_surface(cr_line, surface_fill, lwx, lwy);
	cairo_destroy(cr_line);

	//Copy stuff to the destination surface
	cairo_set_source_rgba(cr_dest, GetRValue(fillColor)/255., GetGValue(fillColor)/255., 
		                           GetBValue(fillColor)/255., GetAValue(fillColor)/255.*alpha);
	cairo_mask_surface(cr_dest, surface_fill, b.x.from, b.y.from);
	cairo_set_source_rgba(cr_dest, GetRValue(lineColor)/255., GetGValue(lineColor)/255., 
		                           GetBValue(lineColor)/255., GetAValue(lineColor)/255.*alpha);
	cairo_mask_surface(cr_dest, surface_line, b.x.from-lwx, b.y.from-lwy);
	cairo_surface_destroy(surface_line);
	cairo_surface_destroy(surface_fill);
}

//Draw a frame to the dest surface, assume b is a frame
void AddFrameToSurface(const Block &block, double alpha, cairo_t *cr_dest, 
					   double xScale, double yScale, COLORREF lineColor, COLORREF fillColor)
{
	//Calculate line width
	int lwx = floor(xScale+0.5);
	if (lwx<1) lwx = 1;
	int lwy = floor(yScale+0.5);
	if (lwy<1) lwy = 1;
	Block b;
	b.x.from = (block.x.from) * xScale;
	b.x.till = (block.x.till) * xScale;
	b.y.from = (block.y.from) * yScale;
	b.y.till = (block.y.till) * yScale;
	cairo_rectangle(cr_dest, b.x.from,         b.y.from,         b.x.Spans(),         b.y.Spans());
	cairo_rectangle(cr_dest, b.x.from - 3*lwx, b.y.from - 3*lwy, b.x.Spans() + 6*lwx, b.y.Spans() + 6*lwy);
	cairo_set_fill_rule(cr_dest, CAIRO_FILL_RULE_EVEN_ODD);
	cairo_set_source_rgba(cr_dest, GetRValue(fillColor)/255., GetGValue(fillColor)/255., 
		                           GetBValue(fillColor)/255., GetAValue(fillColor)/255.*alpha);
	cairo_fill(cr_dest);

	cairo_set_source_rgba(cr_dest, GetRValue(lineColor)/255., GetGValue(lineColor)/255., 
		                           GetBValue(lineColor)/255., GetAValue(lineColor)/255.*alpha);
	cairo_rectangle(cr_dest, b.x.from+0.5,         b.y.from+0.5,         b.x.Spans(),         b.y.Spans());
	cairo_rectangle(cr_dest, b.x.from+0.5 - 3*lwx, b.y.from+0.5 - 3*lwy, b.x.Spans() + 6*lwx, b.y.Spans() + 6*lwy);
	cairo_set_line_width(cr_dest, (lwx+lwy)/2);
	cairo_stroke(cr_dest);
}


//clip is understood as surface coordinates. scale tells me how much to scale m_size to get surface coords.
void CMscGenView::DrawTrackRects(CDC* pDC, CRect clip, double xScale, double yScale)
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	CMscGenDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_trackArcs.size()==0 || pDC==NULL) return;
	//Adjust clip for pDoc->m_nTrackBottomClip. We do not draw a tackrect onto the copyright line.
	if (clip.bottom > yScale*pDoc->m_ChartShown.GetBottomWithoutCopyright());
		clip.bottom = yScale*pDoc->m_ChartShown.GetBottomWithoutCopyright();
	//This is the destination surface
	cairo_surface_t *surface_dest = cairo_win32_surface_create(*pDC);
	cairo_t *cr_dest = cairo_create(surface_dest);
	for (std::vector<TrackedArc>::const_iterator i = pDoc->m_trackArcs.begin(); i!=pDoc->m_trackArcs.end(); i++) {
		//Draw normal covers, leave out FRAME
		Geometry geometry;
		for (std::set<Block>::const_iterator j = i->arc->geometry.GetCover().begin(); j!=i->arc->geometry.GetCover().end(); j++) 
			if (j->drawType == Block::DRAW_NORMAL)
				geometry += *j;
		AddTrackRectsToSurface(geometry, i->alpha/255., cr_dest, xScale, yScale, 
			                   pApp->m_trackLineColor, pApp->m_trackFillColor);
		//Do the frames
		for (std::set<Block>::const_iterator j = i->arc->geometry.GetCover().begin(); j!=i->arc->geometry.GetCover().end(); j++) 
			if (j->drawType == Block::DRAW_FRAME) 
				AddFrameToSurface(*j, i->alpha/255., cr_dest, xScale, yScale, 
				                  pApp->m_trackLineColor, pApp->m_trackFillColor);
	}

	if (m_pl.IsWithin(XY(m_hoverPoint.x, m_hoverPoint.y)))
		m_pl.Fill(cr_dest);
	else
		m_pl.Line(cr_dest); //XXX
	//Cleanup
	cairo_destroy(cr_dest);
	cairo_surface_destroy(surface_dest);
}


void CMscGenView::OnDraw(CDC* pDC)
{
	CMscGenDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (SizeEmpty(m_size)) return;
	//m_zoom is always 100% when in place
	if (pDoc->IsInPlaceActive()) {
		//physical size of in place frame
		CRect viewPort(CPoint(0, 0), pDC->GetViewportExt());
		pDC->SetMapMode(MM_TEXT);
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);
		CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(pDC, viewPort.Width(), viewPort.Height());
		CBitmap *oldBitmap = memDC.SelectObject(&bitmap);
		memDC.FillSolidRect(viewPort, pDC->GetBkColor());
		PlayEnhMetaFile(memDC.m_hDC, pDoc->m_ChartShown.GetEMF(), viewPort);
		DrawTrackRects(&memDC, viewPort, viewPort.Width()/double(m_size.cx), viewPort.Height()/double(m_size.cy));
		pDC->BitBlt(0, 0, viewPort.Width(), viewPort.Height(), &memDC, 0, 0, SRCCOPY);   
		memDC.SelectObject(oldBitmap);
	} else {
		CRect clip;
		pDC->GetClipBox(&clip);
		CRect r(CPoint(0, 0), ScaleSize(m_size, pDoc->m_zoom/100.0));
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);
		CBitmap *oldBitmap;
		//See if the cached bitmap is OK (same zoom & clip falls entirely within the bitmap)
		if (m_cachedBitmapZoom == pDoc->m_zoom && ((m_cachedBitmapClip | clip) == m_cachedBitmapClip)) {
			//Yes, select it into memDC
			oldBitmap = memDC.SelectObject(&m_cachedBitmap);
			memDC.SetWindowOrg(m_cachedBitmapClip.left, m_cachedBitmapClip.top);
		} else {
			//No, discard and regenerate bitmap
			//A null m_cachedBitmapClp indicates m_cachedBitmap is invalid;
			if (!m_cachedBitmapClip.IsRectNull()) m_cachedBitmap.DeleteObject();
			m_cachedBitmap.CreateCompatibleBitmap(pDC, clip.Width(), clip.Height());
			oldBitmap = memDC.SelectObject(&m_cachedBitmap);
			memDC.SetWindowOrg(clip.left, clip.top);
			memDC.FillSolidRect(clip, pDC->GetBkColor());
			PlayEnhMetaFile(memDC.m_hDC, pDoc->m_ChartShown.GetEMF(), r);
			m_cachedBitmapClip = clip;
			m_cachedBitmapZoom = pDoc->m_zoom;
		}

		//See if we have to draw trackrects
		if (pDoc->m_trackArcs.size()>0) {
			//Define second bitmap, copy chart there and add darw trackrects on top
			CDC memDC2;
			memDC2.CreateCompatibleDC(pDC);
			CBitmap bitmap;
			bitmap.CreateCompatibleBitmap(pDC, clip.Width(), clip.Height());
			CBitmap *oldBitmap2 = memDC2.SelectObject(&bitmap);
			memDC2.SetWindowOrg(clip.left, clip.top);
			memDC2.BitBlt(clip.left, clip.top, clip.Width(), clip.Height(),
						  &memDC, clip.left, clip.top, SRCCOPY);   
			DrawTrackRects(&memDC2, clip, pDoc->m_zoom/100., pDoc->m_zoom/100.);
			//Finally copy to output DC
			pDC->BitBlt(clip.left, clip.top, clip.Width(), clip.Height(),
						&memDC2, clip.left, clip.top, SRCCOPY);   
			memDC2.SelectObject(oldBitmap2);
			//clip.DeflateRect(1,1);
			//pDC->DPtoLP(clip);
			//pDC->Rectangle(clip);
		} else {
			//Else just use the cached bitmap
			pDC->BitBlt(clip.left, clip.top, clip.Width(), clip.Height(),
						&memDC, clip.left, clip.top, SRCCOPY);   
		}
		memDC.SelectObject(oldBitmap);
	}
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

void CMscGenView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	CMscGenDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT_VALID(pApp);

	//Delete the cached bitmap
	if (!m_cachedBitmapClip.IsRectNull()) 
		m_cachedBitmap.DeleteObject();
	m_cachedBitmapClip.SetRectEmpty();

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
		pos.right = pos.left + m_size.cx*m_stretch_x;
		pos.bottom = pos.top + m_size.cy*m_stretch_y;
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
		const int amount = client.Height()*0.1*zDelta/WHEEL_DELTA;
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

void CMscGenView::OnSize(UINT nType, int cx, int cy)
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
	} else if (pDoc->m_bTrackMode) {
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
	if (pDoc == NULL || !pDoc->m_bTrackMode) return CScrollView::OnMouseHover(nFlags, point);
	//updateTrackRects expects the point to be in the native chart coordinate space as used by class MscDrawer.
	//Distort for embedded objects, for windowed ones (MM_TEXT) cater for scrolling
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);
	//take zooming into account.
	point.x = point.x*100./pDoc->m_zoom;
	point.y = point.y*100./pDoc->m_zoom;
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
	point.x = point.x*100./pDoc->m_zoom;
	point.y = point.y*100./pDoc->m_zoom;
	if (pDoc->m_bTrackMode) 
		pDoc->UpdateTrackRects(point);
	else {
		TrackableElement *arc = pDoc->m_ChartShown.GetArcByCoordinate(point);
		if (arc) {
			pDoc->StartFadingAll();
			pDoc->AddTrackArc(arc, delay_before_fade/FADE_TIMER);
			pDoc->HighLightArc(arc);
			CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
			ASSERT(pApp != NULL);
			if (pApp->IsInternalEditorRunning()) 
				pApp->m_pWndEditor->SetFocus();
		} else
			CScrollView::OnLButtonUp(nFlags, point);  //This sets focus to view
	}
}

void CMscGenView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CMscGenDoc *pDoc = GetDocument();
	if (pDoc == NULL) return;
	pDoc->SetTrackMode(true);
	//updateTrackRects expects the point to be in the native chart coordinate space as used by class MscDrawer.
	//Distort for embedded objects, for windowed ones (MM_TEXT) cater for scrolling
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);
	//then take zooming into account.
	point.x = point.x*100./pDoc->m_zoom;
	point.y = point.y*100./pDoc->m_zoom;
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
			MessageBox("Sorry, I cannot handle multiple files dropped on me. Drop just one.", 
				       "Msc-generator", MB_OK);
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

DROPEFFECT CMscGenView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
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

DROPEFFECT CMscGenView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	return m_nDropEffect;
}

void CMscGenView::OnDragLeave()
{
	m_nDropEffect = DROPEFFECT_NONE;
}

BOOL CMscGenView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	CMscGenDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	ASSERT(pDataObject);
	if (m_nDropEffect == DROPEFFECT_NONE) 
		return FALSE;
	pDoc->DoPasteData(*pDataObject);
	return TRUE;
}


