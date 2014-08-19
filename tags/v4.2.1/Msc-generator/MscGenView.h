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

/** @file MscGenView.h The interface of the view of a chart.
* @ingroup Msc_generator_files */

#pragma once

#include "MscGenDoc.h"

/** Animation callback resolution in ms. */
#define FADE_TIMER 30

/** Multiply both coordinates of a CSize object. */
inline CSize ScaleSize(const CSize &s, double z) {return CSize(int(s.cx*z), int(s.cy*z));}
/** Test if both coordinates of a CSize object are zero.*/
inline bool SizeEmpty(const CSize &s) {return s.cx==0 || s.cy==0;}

/** The view class.
 * It holds very little functions, only to re-draw from the rendering 
 * cache in CMscGenDoc (well, in CDrawingChartData), dispatching of mouse
 * and drag&drop events, handling timers for animation, panning.
 * We contain a bitmap, which is as large as the view area and holds
 * the chart without any animation. At animations we use this bitmap
 * to draw the animations on top of. This reduces re-draw time when nothing
 * changes, just the animations play. If the user pans or zooms, we need
 * to re-create this bitmap from the rendering cache of CDrawingChartData.
 * If the user changes something that needs a recompile, we have to update
 * even that rendering cache (and our bitmap from that).*/
class CMscGenView : public CScrollView
{
protected: 
	CMscGenView();
	DECLARE_DYNCREATE(CMscGenView)

public:
	bool m_DeleteBkg;            ///<Set to true if we need to erase bkg on next draw
    UINT_PTR     m_FadingTimer;  ///<Handle of Fading Timer
    /** @name Information on the drawn chart
    * @{ */
    XY           m_chartOrigin;  ///<The upper left corner of the chart in chart coordinate space (e.g., page origin)
    Block        m_chartPage;    ///<The page we show in chart coordinates
    CBitmap      m_view;         ///<Last rendered bitmap of (scaled) chart (page)
    CRect        m_view_pos;     ///<Origin and size of bitamp above. (empty if invalid)
    //@}
    /** @name Drag and drop related
    * @{ */
	COleDropTarget m_DropTarget;  ///<Our drop target
	DROPEFFECT     m_nDropEffect; ///<Here we store in OnDragEnter() how we would react if the user dropped that object to us. Kind of a cached value to be used in OnDragOver().
    //@}
    /** @name Dragging (mouse-based panning) related
    * @{ */
    CPoint         m_DragPoint;      ///<During drag-panning this was the last point where we have refreshed the screen.
	CPoint         m_DragStartPoint; ///<Where did we start dragging.
    //@}

// Operations
public:
    /**Get the corresponding document object.*/
    CMscGenDoc* GetDocument() const { return dynamic_cast<CMscGenDoc*>(m_pDocument); }

// Overrides
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
			void InvalidateBlock(const Block &);                 //Invalidate this block (block is in Msc page space)
            void ClearViewCache(); //Clear the bitmap cache
			void DrawAnimation(CDC *pDC, const XY &scale, const CRect &clip);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
            void ForceRepaint() { CScrollView::OnPaint(); } ///<Force an immediate (blocking, synchronous) repaint 
	afx_msg void OnViewRedraw();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
protected:
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	afx_msg void OnFilePrintPreview();
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

// Implementation
public:
    virtual ~CMscGenView() {}

// Generated message map functions
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt); 
	        BOOL DoMouseWheel(UINT nFlags, short zDelta, CPoint pt); 
			void ResyncScrollSize(void);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

			void StartFadingTimer();
	afx_msg void OnTimer(UINT_PTR);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
//    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    afx_msg void OnViewShowElementControls();
    afx_msg void OnUpdateViewShowElementControls(CCmdUI *pCmdUI);
};


