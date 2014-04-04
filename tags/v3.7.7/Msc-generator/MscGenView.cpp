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
// MscGenView.cpp : implementation of the CMscGenView class
//

#include "stdafx.h"
#include "Msc-generator.h"

#include "MscGenDoc.h"
#include "MscGenView.h"
#include "SrvrItem.h"
#include "MainFrm.h"
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

CMscGenView::CMscGenView() 
{
	// construction code here
	m_DeleteBkg = false;
	m_FadingTimer = NULL;
	SetScrollSizes(MM_TEXT, CSize(0,0));
	m_nDropEffect = DROPEFFECT_NONE;
    m_view_pos.SetRectEmpty();
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
	CMscGenDoc* pDoc = GetDocument();
	if (pDoc==NULL) return; 

	pDC->SetMapMode(MM_TEXT);
    pDC->SetWindowExt(ScaleSize(pDoc->m_ChartShown.GetSize(), 
                      pDoc->m_zoom/100.0));
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
    CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
    if (pMain) 
        pMain->AddToPrintPreviewCategory();
	AFXPrintPreview(this);
    if (pMain) {
        pMain->FillMargins();
        pMain->FillPageSize();
        pMain->FillScale4Pagination();
    }
}

void CMscGenView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, 
                                    POINT point, CPreviewView* pView)
{
    CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
    if (pMain) 
        pMain->DeleteFromPrintPreviewCategory();
    CScrollView::OnEndPrintPreview(pDC, pInfo, point, pView);
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
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    const XY ps = pApp->m_PhyPrinterPageSize - 
        XY(pApp->m_printer_usr_margins[0] + pApp->m_printer_usr_margins[1], 
           pApp->m_printer_usr_margins[2] + pApp->m_printer_usr_margins[3]);
    // default preparation
    if (!DoPreparePrinting(pInfo))
        return FALSE;
    pApp->UpdatePrinterData();
    //if page size has changed and we autopaginate, recompile
    const XY new_ps = pApp->m_PhyPrinterPageSize - 
        XY(pApp->m_printer_usr_margins[0] + pApp->m_printer_usr_margins[1], 
           pApp->m_printer_usr_margins[2] + pApp->m_printer_usr_margins[3]);
    if (ps != new_ps && pApp->m_bAutoPaginate) 
        pDoc->CompileEditingChart(false, false);
    pInfo->SetMaxPage(pDoc->m_ChartShown.GetPages());
    return TRUE;
}

void CMscGenView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* pInfo)
{
	CMscGenDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
    if (pDoc)
        pInfo->SetMaxPage(pDoc->m_ChartShown.GetPages());
}

void CMscGenView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	CMscGenDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    CWaitCursor wait;
    pInfo->SetMaxPage(pDoc->m_ChartShown.GetPages());
    //If we are compiling draw just diagonal lines
    if (pInfo->m_bPreview && pDoc->m_pCompilingThread) {
        CBrush brush;
        brush.CreateHatchBrush(HS_FDIAGONAL, RGB(200,200,200));
        pDC->FillRect(pInfo->m_rectDraw, &brush);
        return;
    }
    const CSize orig_size = pDoc->m_ChartShown.GetSize(pInfo->m_nCurPage);
    const XY printable = pApp->GetPrintablePaperSize(); //this is in points (1/72 inch)
    double scale; //will be the scale from chart pixels to points (=user scaling)
    if (pApp->m_iScale4Pagination<=-2) 
        scale = std::min(printable.x/orig_size.cx, printable.y/orig_size.cy);
    else if (pApp->m_iScale4Pagination==-1) 
        scale = printable.x/orig_size.cx;
    else
        scale = pApp->m_iScale4Pagination/100.0;

    const int vA = pApp->m_iPageAlignment<=-2 ? -1 : pApp->m_iPageAlignment>=2 ? +1 : 0;
    const int hA = pApp->m_iPageAlignment - vA*3;
    //We calculate the offset of the top-left corner of the chart page on the physical page
    //'off' below will be in printer pixels, starting from the top left physical margin
    //First we take the offset from the user margin in points
    XY off;
    switch (hA) {
    case -1: off.x = 0; break;
    case  0: off.x = (printable.x - scale*orig_size.cx)/2; break;
    case +1: off.x = printable.x - scale*orig_size.cx; break;
    }
    switch (vA) {
    case -1: off.y = 0; break;
    case  0: off.y = (printable.y - scale*orig_size.cy)/2; break;
    case +1: off.y = printable.y - scale*orig_size.cy; break;
    }
    //Then we calculate this from the physical margin;
    off.x += pApp->m_printer_usr_margins[0] - pApp->m_printer_phy_margins[0];
    off.y += pApp->m_printer_usr_margins[2] - pApp->m_printer_phy_margins[2];
    //Finally we apply it converted to printer pixels.
    pDC->SetViewportOrg(int(off.x*pApp->m_PrinterScale.x), int(off.y*pApp->m_PrinterScale.y));
    pDoc->m_ChartShown.DrawToDC(Canvas::PRINTER, pDC->m_hDC, scale * pApp->m_PrinterScale,
                                pInfo->m_nCurPage, false);

}

void CMscGenView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
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
	//CMscGenDoc *pDoc = GetDocument();
	//ASSERT(pDoc);
	//double scale = pDoc->m_zoom / 100.; 
    //CRect r(int(b.x.from*scale-1), int(b.y.from*scale-1), int(b.x.till*scale+1), int(b.y.till*scale+1));
	//InvalidateRect(&r);
    //Invalidate the whole client area, so that we can cache it
    Invalidate();
}

void CMscGenView::ClearViewCache() 
{
    m_view.DeleteObject();
    m_view_pos.SetRectEmpty();
}

//Draw tracking rectangles and controls
//clip is understood as window surface coordinates 
//scale tells me how much to scale m_size to get surface coords.
//the DC window origin is at 0,0 here
void CMscGenView::DrawAnimation(CDC *pDC, const XY &scale, const CRect &clip)
{
	CMscGenDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_animations.size()==0 || NULL==pDC) return;
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    double grey_fade_value = 0;
    //cairo seems not thread-safe, at least win32 surface
    //so if we compile (which means we also draw), we skip animations
    //except for COMPILING_GREY
    if (pDoc->m_pCompilingThread == NULL) {
        cairo_surface_t *surf = cairo_win32_surface_create(pDC->m_hDC);
        cairo_t *cr = cairo_create(surf);
        cairo_translate(cr, -clip.left, -clip.top);
        cairo_scale(cr, scale.x, scale.y);
        cairo_translate(cr, -m_chartOrigin.x, -m_chartOrigin.y);

        cairo_save(cr);
        //draw track rects only inside the page, not on the copyright text or autoheading or outside
        cairo_rectangle(cr, m_chartPage.x.from, m_chartPage.y.from, m_chartPage.x.Spans(), m_chartPage.y.Spans());
        cairo_clip(cr);
        for (std::vector<AnimationElement>::const_iterator i = pDoc->m_animations.begin(); 
             i!=pDoc->m_animations.end(); i++) 
            switch (i->what) {
            case AnimationElement::TRACKRECT:
                cairo_set_source_rgba(cr, GetRValue(pApp->m_trackFillColor)/255., 
                                          GetGValue(pApp->m_trackFillColor)/255., 
                                          GetBValue(pApp->m_trackFillColor)/255., 
                                          GetAValue(pApp->m_trackFillColor)/255.*i->fade_value);
                i->arc->GetAreaToDraw().Fill(cr);
	            cairo_set_source_rgba(cr, GetRValue(pApp->m_trackLineColor)/255., 
                                          GetGValue(pApp->m_trackLineColor)/255., 
                                          GetBValue(pApp->m_trackLineColor)/255., 
                                          GetAValue(pApp->m_trackLineColor)/255.*i->fade_value);
                i->arc->GetAreaToDraw().Line(cr);
                break;
            case AnimationElement::FALLBACK_IMAGE:
                if (!pDoc->m_fallback_image_location.IsEmpty()) {
                    cairo_save(cr);
                    const Block &total = pDoc->m_ChartShown.GetMscTotal();
                    const double width = 0.4;
                    const double off = 0.8 - i->fade_value;
                    cairo_pattern_t *pattern = cairo_pattern_create_linear(total.x.from, total.y.from, total.x.till, total.y.till);
                    cairo_pattern_add_color_stop_rgba(pattern, off+width*0.00, 1, 1, 1, 0);
                    cairo_pattern_add_color_stop_rgba(pattern, off+width*0.25, 1, 1, 1, 1);
                    cairo_pattern_add_color_stop_rgba(pattern, off+width*0.75, 0, 0, 0, 1);
                    cairo_pattern_add_color_stop_rgba(pattern, off+width*1.00, 0, 0, 0, 0);
                    cairo_set_source(cr, pattern);
                    cairo_set_line_width(cr, 3);
                    cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
                    pDoc->m_fallback_image_location.CairoPath(cr, true);
                    cairo_stroke(cr);
                    cairo_pattern_destroy(pattern);
                    cairo_restore(cr);
                }
                break;
            case AnimationElement::CONTROL:
                break;
            case AnimationElement::COMPILING_GREY:
                grey_fade_value = i->fade_value;
                break;
            default:
                _ASSERT(0);
        }
        //If we have control rectangles, show them even outside page boundaries
        if (pApp->m_bShowControls) {
            cairo_restore(cr); //unclip
            for (std::vector<AnimationElement>::const_iterator i = pDoc->m_animations.begin(); 
                i!=pDoc->m_animations.end(); i++) 
                if (i->what == AnimationElement::CONTROL) 
                    i->arc->DrawControls(cr, i->fade_value);
        }
        cairo_destroy(cr);
        cairo_surface_destroy(surf);
    } else { //if compiling at the moment we draw only the grey stuff
        for (std::vector<AnimationElement>::const_iterator i = pDoc->m_animations.begin(); 
             i!=pDoc->m_animations.end(); i++) 
             if (i->what == AnimationElement::COMPILING_GREY) {
                 grey_fade_value = i->fade_value;
                 break;
             }
    }
    //Do grey only once and at last
    if (grey_fade_value==0) return;

	CDC srcDC;
	srcDC.CreateCompatibleDC(pDC);
	CBitmap bitmap;
	CBitmap *oldBitmap;
    bitmap.CreateCompatibleBitmap(pDC, clip.Width(), clip.Height());
    const CRect rect(0,0,clip.Width(), clip.Height());
    oldBitmap = srcDC.SelectObject(&bitmap);
    srcDC.FillSolidRect(rect, RGB(255,255,255));
    CBrush brush;
    brush.CreateHatchBrush(HS_FDIAGONAL, RGB(200,200,200));
    srcDC.FillRect(rect, &brush);
    BLENDFUNCTION blend;
    blend.BlendOp = AC_SRC_OVER;
    blend.BlendFlags = 0;
    blend.SourceConstantAlpha = BYTE(200*grey_fade_value);
    blend.AlphaFormat = 0;
    pDC->AlphaBlend(0,0,clip.Width(),clip.Height(), &srcDC, 
                    0,0,clip.Width(),clip.Height(), blend);
    srcDC.SelectObject(oldBitmap);

    if (grey_fade_value<1.0) return;
    CMainFrame *pMainWnd = (CMainFrame *)AfxGetMainWnd();
    //With a split window we do not show progress in the upper pane
    if (pMainWnd && pMainWnd->m_wndSplitter.GetRowCount()==2 && 
        this == dynamic_cast<CMscGenView *>(pMainWnd ->m_wndSplitter.GetPane(0,0)))
        return;
    CRect progress;
    const int w = clip.Width()/3;
    progress.left = w;
    progress.right = clip.Width() - w;
    const int m = (clip.bottom-clip.top)/2;
    progress.top = m-10;
    progress.bottom = m+10;
    pDC->FillSolidRect(progress, RGB(255,255,255));
    //cover with white
    pDC->FillSolidRect(progress.left, progress.top, 
                       int(progress.Width()*pDoc->m_Progress), 
                       progress.Height(),
                       RGB(0,255,0));
    //line around
    CPen penBlack;
    penBlack.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&penBlack);
    HGDIOBJ hOldBrush = pDC->SelectObject(GetStockObject(NULL_BRUSH));
    pDC->Rectangle(progress);
    pDC->SelectObject(pOldPen);
    pDC->SelectObject(hOldBrush);
}

void CMscGenView::OnDraw(CDC* pDC)
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT_VALID(pApp);
	CMscGenDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
    CRect clip;
    const double scale = pDoc->m_zoom/100.0;
	pDC->GetClipBox(&clip);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bitmap;
	CBitmap *oldBitmap;
    bitmap.CreateCompatibleBitmap(pDC, clip.Width(), clip.Height());
    oldBitmap = memDC.SelectObject(&bitmap);
    if (clip != m_view_pos) {
        CDC memoryDC; 
        memoryDC.CreateCompatibleDC(pDC);
        m_view.DeleteObject();
        m_view.CreateCompatibleBitmap(pDC, clip.Width(), clip.Height());
        CBitmap *oldBitmap2 = memoryDC.SelectObject(&m_view);
        memoryDC.FillSolidRect(0,0, clip.Width(), clip.Height(), pDC->GetBkColor());
        pDoc->m_ChartShown.DrawToMemDC(memoryDC, scale, scale, clip, pApp->m_bPageBreaks);
        memoryDC.SelectObject(oldBitmap2);
        m_view_pos = clip;
    }
    //Copy bitmap to memDC
    CDC memoryDC;
    memoryDC.CreateCompatibleDC(pDC);
    CBitmap *oldBitmap2 = memoryDC.SelectObject(&m_view);
    memDC.BitBlt(0,0, clip.Width(), clip.Height(), &memoryDC, 0, 0, SRCCOPY);   
    memoryDC.SelectObject(oldBitmap2);
    //Draw track records onto it
    DrawAnimation(&memDC, XY(scale, scale),  clip);
    //Copy to client area
    pDC->BitBlt(clip.left, clip.top, clip.Width(), clip.Height(), &memDC, 0, 0, SRCCOPY);   
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
	ResyncScrollSize();
	CScrollView::OnInitialUpdate();
	//m_pDropTarget is a CWnd memeber and holds the droptarget object
	//If we are already registered, do not register again.
	if (!m_pDropTarget) m_DropTarget.Register(this);
}

void CMscGenView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	CMscGenDoc* pDoc = GetDocument();
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
    CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
    _ASSERT(pDoc && pApp && pMain);
    if (!pDoc || !pApp || !pMain) return;

    m_chartOrigin = pDoc->m_ChartShown.GetPageOrigin(pDoc->m_ChartShown.GetPage());

    m_chartPage = pDoc->m_ChartShown.GetNetPageBlock();

    ClearViewCache();

	//Delete the cached bitmap
	if (pDoc->m_ChartShown.IsEmpty()) {
		m_DeleteBkg = true;
		Invalidate();
		return;
	}
	ResyncScrollSize();
    Invalidate();
}

//Zoom functions
BOOL CMscGenView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CMscGenDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	if (pDoc->DispatchMouseWheel(nFlags, zDelta, pt)) return TRUE;
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
	//if we fall within, and compiling, do nothing, but return true
	if (pDoc->m_pCompilingThread) 
		return true;
	//If no control, just scroll
	if (!(nFlags & MK_CONTROL)) {
		CRect client;
		GetClientRect(client);
        const double ysize = pDoc->m_ChartShown.GetSize().cy*pDoc->m_zoom/100;
        if (client.Height() > ysize) 
            return TRUE;
		//scoll 10% of the view height (zDelta can be negative for up)
		const int amount = int(client.Height()*0.1*zDelta/WHEEL_DELTA);
		CPoint pos = GetScrollPosition();
		pos.y -= amount;
		if (pos.y<0) pos.y = 0;
		if (pos.y > ysize - client.bottom)
			pos.y = int(ysize - client.bottom);
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
    ClearViewCache();
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
    if (pDoc->m_pCompilingThread) 
        EnableScrollBarCtrl(SB_BOTH, FALSE); //disable scroll bars if compiling
    else
        SetScrollSizes(MM_TEXT, ScaleSize(pDoc->m_ChartShown.GetSize(), pDoc->m_zoom/100.0));
}

void CMscGenView::OnSize(UINT /*nType*/, int cx, int cy)
{ 
	CMscGenDoc *pDoc = GetDocument();
	ASSERT(pDoc);
    //Do not adjust zoom here for ZOOM_WIDTH, since zooming 
    //while split window will trigger this - and destroy the zoom command
    //if ZOOM_WIDTH is on.
	//if (pDoc->m_ZoomMode == CMscGenDoc::ZOOM_WIDTH)
	//	pDoc->ArrangeViews();
	//else 
    ResyncScrollSize(); 
}

void CMscGenView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CScrollView::OnLButtonDown(nFlags, point);
	CMscGenDoc *pDoc = GetDocument();
	ASSERT(pDoc);
    if (pDoc->m_pCompilingThread) return; //skip if compiling
	if (1) {
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
    if (pDoc->m_pCompilingThread) return; //skip if compiling
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
        const CSize size = pDoc->m_ChartShown.GetSize();
		if (pos.x + client.right > size.cx*pDoc->m_zoom/100)
			pos.x = size.cx*pDoc->m_zoom/100 - client.right;
		if (pos.y + client.bottom > size.cy*pDoc->m_zoom/100)
			pos.y = size.cy*pDoc->m_zoom/100 - client.bottom;
		ScrollToPosition(pos);
		//Now apply the same x scroll for the other view(s)
		POSITION p = pDoc->GetFirstViewPosition();
		while (p != NULL) {
			CMscGenView* pView = dynamic_cast<CMscGenView*>(pDoc->GetNextView(p));
			if (pView && pView != this) pView->Invalidate();
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
	point.x = int(point.x*100./pDoc->m_zoom + m_chartOrigin.x);
	point.y = int(point.y*100./pDoc->m_zoom + m_chartOrigin.y);
	pDoc->UpdateTrackRects(point);
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
    if (pDoc->m_pCompilingThread) return; //skip if compiling

	//updateTrackRects expects the point to be in the native chart coordinate space as used by class MscDrawer.
	//Distort for embedded objects, for windowed ones (MM_TEXT) cater for scrolling
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);
	//then take zooming into account.
	point.x = int(point.x*100./pDoc->m_zoom + m_chartOrigin.x);
	point.y = int(point.y*100./pDoc->m_zoom + m_chartOrigin.y);
    for (auto i = pDoc->m_controlsShowing.begin(); i!=pDoc->m_controlsShowing.end(); i++)
        if (inside(i->first.IsWithin(XY(point.x, point.y))))
            if (i->second)
                if (pDoc->OnControlClicked(i->second, i->second->WhichControl(XY(point.x, point.y))))
                    return; //Chart and "i" is no longer valid pDoc->m_trackArc and m_controlsShown are empty
	if (pDoc->m_bTrackMode) {
		pDoc->UpdateTrackRects(point);
	} else {
        Element *arc = pDoc->m_ChartShown.GetArcByCoordinate(point);
		if (arc) {
			pDoc->StartFadingAll();
			pDoc->AddAnimationElement(AnimationElement::TRACKRECT, arc, int(delay_before_fade));
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
    if (pDoc->m_pCompilingThread) return; //skip if compiling
    //First transform the point to the chart coordinate space
	//updateTrackRects expects the point to be in the native chart coordinate space as used by class MscDrawer.
	//Distort for embedded objects, for windowed ones (MM_TEXT) cater for scrolling
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);
	//then take zooming into account.
	point.x = int(point.x*100./pDoc->m_zoom + m_chartOrigin.x);
	point.y = int(point.y*100./pDoc->m_zoom + m_chartOrigin.y);

    //If the click is on an element that has controls, activate the first one
	Element *arc = pDoc->m_ChartShown.GetArcByCoordinate(point);
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
