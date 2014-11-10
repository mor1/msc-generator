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

/** @file MainFrm.h The interface for the main window classes.
* @ingroup Msc_generator_files */

#pragma once
#include "OutputView.h"
#include "MiniEditor.h"

/** Small extension of CSplitterWnd to capture when user kills splitting.*/
class CMscGenSplitterWnd : public CSplitterWnd
{
public:
    virtual void DeleteRow(int rowDelete);
};

/** Number of the mscgen compatibility pane on the statusbar*/
#define NUM_STATUS_BAR_MSCGEN_COMPAT 1
/** Number of the tracking mode pane on the statusbar*/
#define NUM_STATUS_BAR_TACKING 2

/** The main window class.*/
class CMainFrame : public CFrameWndEx
{
	
protected: 
    /** Just init fields. Not called from external sources.
     * We get created from serialization only.*/
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CMscGenSplitterWnd m_wndSplitter;        ///<The splitter window used to show two panes of the chart
	CMFCRibbonBar      m_wndRibbonBar;       ///<Our ribbon
	CMFCStatusBar      m_wndStatusBar;       ///<Our status bar
    CMFCRibbonLabel   *m_labelObjectSize;    ///<Pointer to the 'Object Size' label on the Embedded Chart panel category.
    CMFCRibbonLabel   *m_labelFallbackImage; ///<Pointer to the 'Fallback Image' label on the Embedded Chart panel category.

protected:  
    friend class CMscGenSplitterWnd;
	CMFCRibbonApplicationButton m_MainButton;///<The main button of the Ribbon
	CMFCToolBarImages m_PanelImages;         ///<The panel images for the ribbon
	COutputViewBar    m_wndOutputView;       ///<The panel hosting the error lists.
	CEditorBar        m_ctrlEditor;          ///<The panel hosting the internal editor.
public:
    bool m_bAutoSplit;                       ///<True if autosplit mode is on
    bool m_at_embedded_object_category;      ///<True if the Embedded Object ribbon category is currently showing
    bool m_has_fallback_image;               ///<True if the object has a fallback image


protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnButtonAutoSplit();             //AutpSplit mode button
    afx_msg void OnUpdateButtonAutoSplit(CCmdUI *pCmdUI);
    DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	        BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnViewFullScreen();
    afx_msg LRESULT OnCompilationDone(WPARAM wParam, LPARAM lParam);
    /** True if we are in autosplit mode.*/
    bool IsInAutoSplitMode() const {return m_bAutoSplit;}
    void SetSplitSize(unsigned coord);
    bool AddToFullScreenToolbar(); //finds the fulls screen toolbar and adds our buttons to it
    void AddToPrintPreviewCategory();
    void DeleteFromPrintPreviewCategory();
// Implementation
public:
    virtual ~CMainFrame() {}
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

    bool FillDesignComboBox(const char *current, bool updateComboContent);
    afx_msg void OnDesignDesign(); //design combo box changes
    afx_msg void OnUpdateDesignDesign(CCmdUI *pCmdUI);
    void FillPageComboBox(int no_pages, int page);
	afx_msg void OnDesignPage(); //page combo box changes
	afx_msg void OnFullScreenPage(); //page combo box changes
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
};


