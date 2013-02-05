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

// MscGenDoc.h : interface of the CMscGenDoc class
//


#pragma once
#include "ChartData.h"
#include "ExternalEditor.h"
#include "afxwin.h"

class CMscGenSrvrItem;

struct AnimationElement {
	const Element *arc;
    enum ElementType {
        CONTROL = 0, 
        TRACKRECT, 
        FALLBACK_IMAGE, 
        COMPILING_GREY,
        MAX_TYPE
    } what;
    static const unsigned default_appear_time[MAX_TYPE];
    static const unsigned default_disapp_time[MAX_TYPE];
    enum {APPEARING, SHOWING, FADING, OFF} status;
	int    fade_delay;  //After appear this much delay is there to fade in ms. <0 never fade
    int    appe_time;
    int    disa_time;
	double fade_value;  //0 is not shown, 1 is fully shown
	AnimationElement(Element *a, ElementType et, int delay = -1, 
                     int appear=-1, int disappear=-1);
	AnimationElement(ElementType et, int delay = -1, 
                     int appear=-1, int disappear=-1);
};

class CMscGenDoc : public COleServerDocEx
{
protected: // create from serialization only
	CMscGenDoc();
	DECLARE_DYNCREATE(CMscGenDoc)
	DECLARE_MESSAGE_MAP()
	virtual COleServerItem* OnGetEmbeddedItem();
    virtual COleServerItem* OnGetLinkedItem(LPCTSTR lpszItemName);
	virtual CDocObjectServer* GetDocObjectServer(LPOLEDOCUMENTSITE pDocSite);
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
	CDrawingChartData m_ChartShown; //The chart that is currently showing
    CDrawingChartData m_ChartCompiling; //The chart that is compiling.
    int m_page_serialized_in; //the page read by last Serialize();
    bool m_hadArrangeViews; //If we shall arrange views after compilation 
    bool m_highlight_fallback_images; //if we shall highlight fallback image loc after compilation
    CCriticalSection m_SectionCompiling; //To protect m_ChartCompiling
    CWinThread *m_pCompilingThread;
    double m_Progress;
    bool m_killCompilation; 
    HWND m_hMainWndToSignalCompilationEnd; 

    unsigned m_uSavedFallbackResolution; //for embedded charts 
    bool     m_bSavedPageBreaks;          //for embedded charts 
    unsigned m_uSavedPage; //The page we have embedded

	// Zoom related 
	unsigned m_zoom; //In percentage. 100 is normal
	enum EZoomMode {NONE=0, OVERVIEW, WINDOW_WIDTH, ZOOM_WIDTH} m_ZoomMode;
	// Track mode related
	bool m_bTrackMode; //True if mouse is tracked over arcs
    Contour m_fallback_image_location;
	std::vector<AnimationElement> m_animations;  //arcs to track currently
    std::map<Block, Element*> m_controlsShowing; //Controls currently appearing
	CHARRANGE m_saved_charrange;
	Element *m_last_arc; //During tracking the arc highlighted in the editor
	CView *m_pViewFadingTimer;    //Our view in which track rectangles show
	//Clipboard format
	static CLIPFORMAT m_cfPrivate;
	//The external editor object
	CExternalEditor m_ExternalEditor; //A hidden window to do external editor functions
	//Status flags
	bool m_bAttemptingToClose; //This indicates to OnSaveDocument if we are closing 
    size_t serialize_doc_overhead;  //for embedded objects this calcualtes serialization overhead

//Menu and toolbar functions
public:
	virtual void Serialize(CArchive& ar);
            void SerializePage(CArchive& ar, unsigned &page);
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
    afx_msg void OnCopyPage(UINT id); //starts from ID_COPY_PAGE1
	afx_msg void OnUpdateEditPasteEntireChart(CCmdUI *pCmdUI);
			void DoPasteData(COleDataObject &dataObject);
	afx_msg void OnEditPasteEntireChart();
	afx_msg void OnUpdateButtonEdittext(CCmdUI *pCmdUI);
	afx_msg void OnButtonEdittext();          //External editor button
	afx_msg void OnUdpateEditUpdate(CCmdUI *pCmdUI);
	afx_msg void OnEditUpdate();              //update chart from internal editor
	afx_msg void OnUpdateButtonTrack(CCmdUI *pCmdUI);
	afx_msg void OnButtonTrack();             //Tracking mode button
			void DoViewNexterror(bool next);  //This does the stepping
	afx_msg void OnViewNexterror();           //User advances error
	afx_msg void OnViewPreverror();           //User steps back in errors
	afx_msg void OnSelChange();               //Another error is selected

	void ChangeDesign(const char *design);
    void ChangePage(unsigned page);
    void StepPage(signed int step);

	//Zoom functions
			void SetZoom(int zoom=0);                        //Update views with new zoom factor. ==0 means just reset toolbar
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
	bool CheckIfChanged();                               //Check if we have changed and updates SetModifiedFlag
    void OnExternalEditorChange(const CChartData &data); //this is called by m_ExternalEditor if the text in the external editor changes
    void OnInternalEditorChange();                       //this is called by CMiniEditor if the text in the internal editor changes
    void OnInternalEditorSelChange();                    //this is called by CMiniEditor if the selection in the internal editor changes
	void CompileEditingChart(bool resetZoom, bool block);//Call this to show the currently edited chart, it compiles and updates the views, calls NotifyChanged()
    void CompleteCompilingEditingChart();                //This will be called once the compilation is complete
    void KillCompilation();                              //Abort the compilation

	void StartFadingTimer();                             //Ensure that one and only one View runs a fading timer;
	bool DoFading();                                     //Do one step fading. Return true if there are still elements in the process of fading
	bool AddAnimationElement(AnimationElement::ElementType, Element *arc=NULL, 
                             int delay=-1); //Add a tracking element to the list. Updates Views if needed & rets ture if so
    void StartTrackFallbackImageLocations(const Contour &);
    void StartFadingAll(AnimationElement::ElementType type=AnimationElement::TRACKRECT, const Element *except=NULL); //Start the fading process for all rectangles (even for delay<0, except one)
	void SetTrackMode(bool on);                          //Turns tracking mode on
	void UpdateTrackRects(CPoint mouse);                 //updates tracking rectangles depending on the mouse position (position is in MscDrawer coord space)
	void HighLightArc(const Element *arc);      //Select in internal editor
    bool OnControlClicked(Element *arc, EGUIControlType t); //Do what is needed if a control is clicked. Ture if chart invalidated.
    void OnChangeRibbonCategory(bool embedded);
    void ReDrawEMF();
	afx_msg void OnHelpHelp();
    afx_msg void OnEditSelectAll();
};


