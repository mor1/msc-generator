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
    along with Msc-generator.  If not, see <http://www.gnu.org/licenses/>.
*/

// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "OutputView.h"
#include "MiniEditor.h"

class CMscGenSplitterWnd : public CSplitterWnd
{
public:
    virtual BOOL SplitRow(int cyBefore);
    virtual void DeleteRow(int rowDelete);
};


class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CMscGenSplitterWnd m_wndSplitter;
	CMFCStatusBar     m_wndStatusBar;
protected:  
    friend class CMscGenSplitterWnd;
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCToolBar       m_wndDesignBar;
	CMFCToolBarImages m_UserImages;
	COutputViewBar    m_wndOutputView;
	CEditorBar        m_ctrlEditor;
    bool m_bAutoSplit;         //True if autosplit mode is on


protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnToolbarReset(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonAutoSplit(CCmdUI *pCmdUI);
	afx_msg void OnButtonAutoSplit();             //AutpSplit mode button
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	        void OnUpdateFrameMenu(HMENU hMenuAlt);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	        BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnViewFullScreen();

    bool IsInAutoSplitMode() const {return m_bAutoSplit;}
    void SetSplitSize(unsigned coord);
    bool AddToFullScreenToolbar(); //finds the fulls screen toolbar and adds our buttons to it

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};


