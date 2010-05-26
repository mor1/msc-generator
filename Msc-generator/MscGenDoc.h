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
#include "ExternalEditor.h"
#include "afxwin.h"

class CMscGenSrvrItem;

class CMscGenDoc : public COleServerDocEx
{
protected: // create from serialization only
	CMscGenDoc();
	DECLARE_DYNCREATE(CMscGenDoc)
	DECLARE_MESSAGE_MAP()
	virtual COleServerItem* OnGetEmbeddedItem();
	virtual CDocObjectServer* GetDocObjectServer(LPOLEDOCUMENTSITE pDocSite);
	virtual void DestroyInPlaceFrame(COleIPFrameWnd* pFrameWnd);
public:
	virtual ~CMscGenDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Attributes
public:
	//Actual Signalling Chart Data and various pointers to this list
	std::list<CChartData> m_charts;
	IChartData m_itrSaved; //The chart that is the one last saved. This iterator may be invalid, use only for comparison
	IChartData m_itrEditing; //The chart that is the current one in the editor
	IChartData m_itrShown; //The chart that is compiled and shown in view. Iterator may be invalid if user undoed the shown chart 
	CChartData m_ChartShown;
	unsigned m_page; //current page to show

	// Zoom related 
	int m_zoom; //In percentage. 100 is normal
	enum EZoomMode {NONE=0, OVERVIEW, WINDOW_WIDTH, ZOOM_WIDTH} m_ZoomMode;
	// Track mode related
	bool m_bTrackMode; //True if mouse is tracked over arcs
    int m_nTrackRectNo; //True if there are boxes to display
	TrackRect m_rctTrack[100];
	int m_nTrackBottomClip; //the bottom of the clip region (in MscDrawer space) to clip at (above the copyright label)
	CHARRANGE m_saved_charrange;
	void* m_last_arc; //the arc for which current track rects are valid
	//Clipboard format
	static CLIPFORMAT m_cfPrivate;
	//The external editor object
	CExternalEditor m_ExternalEditor; //A hidden window to do external editor functions
	//This indicates to OnSaveDocument if we are closing 
	bool m_bAttemptingToClose;

//Menu and toolbar functions
public:
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents(); // Destroys existing data, updates views
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();

	afx_msg void OnUpdateFileExport(CCmdUI *pCmdUI);
	afx_msg void OnFileExport();
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditCutCopy(CCmdUI *pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditFindReplace(CCmdUI *pCmdUI);
	afx_msg void OnEditFind();
	afx_msg void OnEditRepeat();
	afx_msg void OnEditReplace();
	afx_msg void OnEditCopyEntireChart();
	afx_msg void OnUpdateEditPasteEntireChart(CCmdUI *pCmdUI);
	afx_msg void OnEditPasteEntireChart();
	afx_msg void OnUpdateButtonEdittext(CCmdUI *pCmdUI);
	afx_msg void OnButtonEdittext();          //External editor button
	afx_msg void OnUdpateEditUpdate(CCmdUI *pCmdUI);
	afx_msg void OnEditUpdate();              //update chart from internal editor
	afx_msg void OnUpdateButtonTrack(CCmdUI *pCmdUI);
	afx_msg void OnButtonTrack();             //Tracking mode button
	afx_msg void OnViewNexterror();           //User advances error
	afx_msg void OnSelChange();               //Another error is selected

	afx_msg void OnDesignDesign(); //design combo box changes
	afx_msg void OnDesignPage(); //page combo box changes

	//Zoom functions
			void SetZoom(int zoom=0);                        //Update views with new zoom factor. ==0 means just reset toolbar
	afx_msg void OnViewZoomin();                             //zoom in 10%
	afx_msg void OnViewZoomout();                            //zoom out 10%
	afx_msg void OnDesignZoom();                             //user changed zoom combo
			void ArrangeViews(EZoomMode mode);               //Automatically adjust zoom and window size
			void ArrangeViews() {ArrangeViews(m_ZoomMode);}  //Automatically adjust zoom and window size using existng zoom mode (if any)
	afx_msg void OnViewZoomnormalize();                      //Automatically adjust zoom and window size: overview mode
	afx_msg void OnViewAdjustwidth();                        //Automatically adjust zoom and window size: set window width
	afx_msg void OnViewFittowidth();                         //Automatically adjust zoom and window size: set width to window
			void SwitchZoomMode(EZoomMode mode);             //Set zoom mode to specified value (and adjust zoom)
	afx_msg void OnZoommodeKeepinoverview();                 //Set zoom mode
	afx_msg void OnZoommodeKeepadjustingwindowwidth();       //Set zoom mode
	afx_msg void OnZoommodeKeepfittingtowidth();             //Set zoom mode
	afx_msg void OnUpdateZoommodeKeepinoverview(CCmdUI *pCmdUI);
	afx_msg void OnUpdateZoommodeKeepadjustingwindowwidth(CCmdUI *pCmdUI);
	afx_msg void OnUpdateZoommodeKeepfittingtowidth(CCmdUI *pCmdUI);

	        bool DispatchMouseWheel(UINT nFlags, short zDelta, CPoint pt);

public:
	void InsertNewChart(const CChartData &);             //insert a new chart into the list (destroys redo, updates iterators)
	void SyncShownWithEditing(const CString &action);    //Ask the user what to do if editing iterator != shown iterator
    void OnExternalEditorChange(const CChartData &data); //this is called by m_ExternalEditor if the text in the external editor changes
    void OnInternalEditorChange();                       //this is called by CMiniEditor if the text in the internal editor changes
    void OnInternalEditorSelChange();                    //this is called by CMiniEditor if the selection in the internal editor changes
	void ShowNewChart(IChartData, bool resetZoom);       //Call this if you wnat to show a new chart, it compiles and updates the views
	void SetTrackMode(bool on);                          //Turns tracking mode on
	void UpdateTrackRects(CPoint mouse);                 //updates tracking rectangles depending on the mouse position (position is in MscDrawer coord space)
};


