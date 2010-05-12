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
// MscGenView.h : interface of the CMscGenView class
//

#pragma once

#include "MscGenDoc.h"

inline CSize ScaleSize(const CSize &s, double z) {return CSize(s.cx*z, s.cy*z);}
inline bool SizeEmpty(const CSize &s) {return s.cx==0 || s.cy==0;}

class CMscGenView : public CScrollView
{
protected: // create from serialization only
	CMscGenView();
	DECLARE_DYNCREATE(CMscGenView)

// Attributes
public:
	bool m_DeleteBkg;
	// Drawn chart
	HENHMETAFILE m_hemf;
	CSize        m_size;
	//stretch for in-place editing
	double m_stretch_x;
	double m_stretch_y;
	UINT_PTR m_hTimer;

// Operations
public:
	CMscGenDoc* GetDocument() const;

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
		    void DrawTrackRects(CDC* pDC, const CRect &clip);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	afx_msg void OnViewRedraw();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

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
	afx_msg void ResetAspectRatioInPlace(void);
	afx_msg void OnUpdateResetAspectRatioInPlace(CCmdUI *pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
protected:
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in MscGenView.cpp
inline CMscGenDoc* CMscGenView::GetDocument() const
   { return reinterpret_cast<CMscGenDoc*>(m_pDocument); }
#endif

