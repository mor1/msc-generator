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

// MscGenDoc.h : interface of the CMscGenDoc class
//


#pragma once
#include "ChartData.h"
#include "afxwin.h"

class CMscGenSrvrItem;



enum EStopEditor {STOPEDITOR_NOWAIT, STOPEDITOR_WAIT, STOPEDITOR_FORCE};

class CMscGenDoc : public COleServerDocEx
{
protected: // create from serialization only
	CMscGenDoc();
	DECLARE_DYNCREATE(CMscGenDoc)

// Attributes
public:
	CMscGenSrvrItem* GetEmbeddedItem()
		{ return reinterpret_cast<CMscGenSrvrItem*>(COleServerDocEx::GetEmbeddedItem()); }
	//Actual Signalling Chart Data and undo list
	std::list<CChartData> m_charts;
	IChartData m_itrSaved; //The chart that is the one last saved
	IChartData m_itrCurrent; //The chart that is the current one
	unsigned m_page;
	unsigned m_pages;
	// View related 
	int m_zoom; //In percentage. 100 is normal
	enum EZoomMode {NONE=0, OVERVIEW, WINDOW_WIDTH, ZOOM_WIDTH} m_ZoomMode;

	//Clipboard format
	static CLIPFORMAT m_cfPrivate;
	//The non-modal windows
	CDialog m_ProgressWindow;
	int m_ProgressWindowCount;
	//Text editor spawned process related
	DWORD m_EditorProcessId;
	CString m_EditorFileName;
	CTime m_EditorFileLastMod;
	HWND m_hWndForEditor;

// Operations
public:
	void InsertNewChart(const CChartData &);
	//Editor functions
	void OnUpdate(bool resetZoom=true, bool updateInternalEditor=true);
	void StartEditor(CString = "");
	void RestartEditor(EStopEditor force);
	bool IsEditorRunning() const;
	bool IsCorrectEditorRunning() const;
	void JumpToLine(unsigned line, unsigned col);
	bool CheckExternalEditorAndFile(void);
	void StopEditor(EStopEditor force, bool);
	void StartDrawingProgress();
	void StopDrawingProgress();

// Overrides
protected:
	virtual COleServerItem* OnGetEmbeddedItem();
	virtual void OnSetItemRects(LPCRECT lpPosRect , LPCRECT lpClipRect);
public:
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents(); // Destroys existing data, updates views
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	afx_msg void OnFileExport();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditCopyEntireChart();
	afx_msg void OnEditPasteEntireChart();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditCutCopy(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditPasteEntireChart(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileExport(CCmdUI *pCmdUI);
	afx_msg void OnSelChange();

	//Zoom functions
			void SetZoom(int zoom);
	afx_msg void OnViewZoomin();
	afx_msg void OnViewZoomout();
	afx_msg void OnDesignZoom();
			void ArrangeViews(EZoomMode mode);
			void ArrangeViews() {ArrangeViews(m_ZoomMode);}
			void SwitchZoomMode(EZoomMode mode);
	afx_msg void OnViewZoomnormalize();
	afx_msg void OnViewAdjustwidth();
	afx_msg void OnViewFittowidth();
	afx_msg void OnZoommodeKeepinoverview();
	afx_msg void OnZoommodeKeepadjustingwindowwidth();
	afx_msg void OnZoommodeKeepfittingtowidth();
	afx_msg void OnUpdateZoommodeKeepinoverview(CCmdUI *pCmdUI);
	afx_msg void OnUpdateZoommodeKeepadjustingwindowwidth(CCmdUI *pCmdUI);
	afx_msg void OnUpdateZoommodeKeepfittingtowidth(CCmdUI *pCmdUI);

// Implementation
public:
	virtual ~CMscGenDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual CDocObjectServer* GetDocObjectServer(LPOLEDOCUMENTSITE pDocSite);
	virtual void DestroyInPlaceFrame(COleIPFrameWnd* pFrameWnd);
	virtual COleIPFrameWnd* CreateInPlaceFrame(CWnd* pParentWnd);

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEditUpdate();
	afx_msg void OnUdpateEditUpdate(CCmdUI *pCmdUI);
	afx_msg void OnViewNexterror();
};


