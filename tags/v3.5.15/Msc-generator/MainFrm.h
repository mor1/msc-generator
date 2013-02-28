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
	CMFCRibbonBar      m_wndRibbonBar;
	CMFCStatusBar      m_wndStatusBar;
    CMFCRibbonLabel   *m_labelObjectSize, *m_labelFallbackImage;

protected:  
    friend class CMscGenSplitterWnd;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	//CMFCRibbonStatusBar  m_wndStatusBar;
	COutputViewBar    m_wndOutputView;
	CEditorBar        m_ctrlEditor;
    bool m_bAutoSplit;         //True if autosplit mode is on
public:
    bool m_at_embedded_object_category;
    bool m_has_fallback_image;


protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonAutoSplit(CCmdUI *pCmdUI);
	afx_msg void OnButtonAutoSplit();             //AutpSplit mode button
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	        BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnViewFullScreen();
    afx_msg LRESULT OnCompilationDone(WPARAM wParam, LPARAM lParam);
    bool IsInAutoSplitMode() const {return m_bAutoSplit;}
    void SetSplitSize(unsigned coord);
    bool AddToFullScreenToolbar(); //finds the fulls screen toolbar and adds our buttons to it
    void AddToPrintPreviewCategory();
    void DeleteFromPrintPreviewCategory();
// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

    bool FillDesignComboBox(const char *current, bool updateComboContent);
    afx_msg void OnDesignDesign(); //design combo box changes
    afx_msg void OnUpdateDesignDesign(CCmdUI *pCmdUI);
    void FillPageComboBox(int no_pages, int page);
	afx_msg void OnDesignPage(); //page combo box changes
    afx_msg void OnUpdateDesignPage(CCmdUI *pCmdUI);
	afx_msg void OnDesignNextPage(); //page combo box changes
	afx_msg void OnDesignPrevPage(); //page combo box changes
    void FillZoomComboBox(int zoom);
	afx_msg void OnDesignZoom();                             //user changed zoom combo
    afx_msg void OnViewZoomin();                             //zoom in 10%
	afx_msg void OnViewZoomout();                            //zoom out 10%
	afx_msg void OnViewInternalEditor();                     //Toggles internal editor
    afx_msg void OnUpdateViewInternalEditor(CCmdUI *pCmdUI); //Shows if internal editor is on
    void FillEmbeddedPanel(size_t size, double percent);     //Set Embedded Object panel Labels
    void FillScale4Pagination();
    void FillPageSize();
    void FillMargins();
    void TriggerIfRibbonCategoryChange();
    afx_msg void OnUpdateButtonDefaultText(CCmdUI *pCmdUI);
    afx_msg void OnUpdateEmbeddedoptionsFallbackRes(CCmdUI *pCmdUI);
    afx_msg void OnComboAlignment();
};


