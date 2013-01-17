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
// MscGenView.h : interface of the CMscGenView class
//

#pragma once

#include "MscGenDoc.h"

#define FADE_TIMER 30

inline CSize ScaleSize(const CSize &s, double z) {return CSize(int(s.cx*z), int(s.cy*z));}
inline bool SizeEmpty(const CSize &s) {return s.cx==0 || s.cy==0;}

class CMscGenView : public CScrollView
{
protected: // create from serialization only
	CMscGenView();
	DECLARE_DYNCREATE(CMscGenView)

// Attributes
public:
	bool m_DeleteBkg;  //Set to true if we need to erase bkg on next draw
	// Drawn chart
    XY           m_chartOrigin;  //The upper left corner of the chart in chart coordinate space
    CChartCache  m_cache;        //pre-drawn (nonscaled) version of chart (page)
    CBitmap      m_view;         //Last rendered bitmap of (scaled) chart (page)
    CRect        m_view_pos;     //origin and size of bitamp above. (empty if invalid)
	UINT_PTR     m_FadingTimer;  //Handle of Fading Timer
    bool m_recalc_embeddded_object_data; //pass trigger from OnUpdate() to OnDraw()
    bool m_highlight_fallback_images;
	//Drag and Drop 
	COleDropTarget m_DropTarget;
	DROPEFFECT     m_nDropEffect;
	//Draging
	CPoint         m_DragPoint;
	CPoint         m_DragStartPoint;

// Operations
public:
	CMscGenDoc* GetDocument() const;

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
			void InvalidateBlock(const Block &);                 //Invalidate this block (block is in Msc page space)
            void ClearViewCache(); //Clear the bitmap cache
			void DrawTrackRects(CDC *pDC, double x_scale, double y_scale, CRect clip);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	afx_msg void OnViewRedraw();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
protected:
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	afx_msg void OnFilePrintPreview();
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	afx_msg void OnCancelEditSrvr();

// Implementation
public:
	virtual ~CMscGenView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

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

#ifndef _DEBUG  // debug version in MscGenView.cpp
inline CMscGenDoc* CMscGenView::GetDocument() const
   { return reinterpret_cast<CMscGenDoc*>(m_pDocument); }
#endif

