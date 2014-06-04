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

/** @file MscGenDoc.h The main document class interface.
* @ingroup Msc_generator_files */


#pragma once
#include "ChartData.h"
#include "ExternalEditor.h"
#include "afxwin.h"

class CMscGenSrvrItem;

/** Structure describing one piece of animated content.
 * It can be a tracking rectangle/shape, an element control (for
 * collapse/expand) or a contour of a fallback image on the embedded
 * object view or the compilation progress bar (coverin the whole view).*/
struct AnimationElement {
	const Element *arc;  ///<If we are a tracking rect or control, this is the corresponding arc
    /** Types of animation elements */
    enum ElementType {
        CONTROL = 0,    ///<A collapse/expand control
        TRACKRECT,      ///<A tracking rectangle (or rather shape)
        FALLBACK_IMAGE, ///<A contour of fallback images
        COMPILING_GREY, ///<The compiling progress bar
        MAX_TYPE        ///<Maximum value for AnimationElement::ElementType
    } what;             ///<What sort of animation is this.
    static const unsigned default_appear_time[MAX_TYPE]; ///<Length of time for these animations to appear
    static const unsigned default_disapp_time[MAX_TYPE]; ///<Length of time for these animations to disappear
    /** States an animation can be in. */
    enum {
        APPEARING, ///<The animation is in the process of appearing
        SHOWING,   ///<The animation is fully shown (does not change)
        FADING,    ///<The animation is is in the process of disappearing
        OFF        ///<The animation has completely disappeared (and can be deleted)
    } status;  ///<The current status of this animation
	int    fade_delay;  ///<After appear this much long has to pass before we automatically start disappearing. If <0 we do not automatically start fading.
    int    appe_time;   ///<This much time left from appearing.
    int    disa_time;   ///<This much time left from disappearing.
	double fade_value;  ///<How much are we showing. 0 is not shown, 1 is fully shown.
    /** Initialize as a tracking rectange/shape or control*/
    AnimationElement(Element *a, ElementType et, int delay = -1, 
                     int appear=-1, int disappear=-1);
    /** Initialize as a fallback image or compiling progress bar.*/
	AnimationElement(ElementType et, int delay = -1, 
                     int appear=-1, int disappear=-1);
};

/** The main document object.
 * It actually handles rendering to a cache (in CChartDrawingData), as well. 
 * This is because in case of split view, we have two CMscGenView objects, each one 
 * having the same zoom, cached rendered content, tracking animations. Those common
 * data is then stored/processed in this document object.*/
class CMscGenDoc : public COleServerDocEx
{
protected: 
	CMscGenDoc(); ///<Protected constructor: create from serialization only
	DECLARE_DYNCREATE(CMscGenDoc)
	DECLARE_MESSAGE_MAP()
	virtual COleServerItem* OnGetEmbeddedItem();
    virtual COleServerItem* OnGetLinkedItem(LPCTSTR lpszItemName);
	virtual CDocObjectServer* GetDocObjectServer(LPOLEDOCUMENTSITE pDocSite);
public:
    virtual ~CMscGenDoc() {}

    // Attributes
public:
    /** @name Actual Signalling Chart Data and various pointers
     * The undo buffer is essentially a list of CChartData objects (each encapsulating full chart text and attributes)
     * We also store two compiled/rendered charts (CDrawingChartData, encapsulating an Msc object), one that is 
     * currently shown and anothe that is being compiled.
    * @{ */
	std::list<CChartData> m_charts;     ///<The undo buffer containing chart texts
	IChartData m_itrSaved;              ///<The chart that is last saved. This iterator may be invalid (==m_charts.end()), use only for comparison
	IChartData m_itrEditing;            ///<The chart that is the current one in the internal or external editor
	IChartData m_itrShown;              ///<The chart that is compiled and shown in view. Iterator may be invalid (==m_charts.end()) if user undoed the shown chart.
	CDrawingChartData m_ChartShown;     ///<The chart that is currently showing, including a cached rendering of it.
    CDrawingChartData m_ChartCompiling; ///<The chart that is currently compiling/rendering.
    //@}
    /** @name Misc data for embedded objects
    * @{ */
    int      m_page_serialized_in;       ///<The page read by last Serialize() - we store it since we have no other way to get it out of Serialize()
    unsigned m_uSavedFallbackResolution; ///<The fallback resolution saved for embedded charts 
    bool     m_bSavedPageBreaks;         ///<The page break indicator flag saved for embedded charts 
    unsigned m_uSavedPage;               ///<The page we have embedded
    size_t   serialize_doc_overhead;     ///<For embedded objects this stores the calcualted serialization overhead (so we can display Object Size correctly)
    //@}
    /** @name Compilation related 
     * We may do compilation in a separate thread. The separate thread works on m_ChartCompiling.
     * We have a lock here to protect that, a handle to a process and ways to communicate with it.
    * @{ */
    CCriticalSection m_SectionCompiling;   ///<A critical section to protect m_ChartCompiling, during compilation
    CWinThread *m_pCompilingThread;        ///<The handle of the compilation thread 
    double m_Progress;                     ///<The progress during compilation - updated by the compiling thread
    bool m_killCompilation;                ///<The main thread may set this causing the compilation thread to abort 
    HWND m_hMainWndToSignalCompilationEnd; ///<If the compilation thread suceeds, it sends a WM_APP+293 message to this window (should be our main window)
    bool m_hadArrangeViews;                ///<If we shall arrange views after compilation 
    bool m_highlight_fallback_images;      ///<if we shall highlight fallback image loc after compilation
    //@}
    /** @name Zoom related stuff
     * Since we do actual rendering in this object, we must have zoom related functions here.
    * @{ */
    unsigned m_zoom;     ///<Current zoom in percentage. 100 is normal
    /** What zoom modes we may have. 
     * A zoom mode is an automatic zoom setting applied after compilation.*/
	enum EZoomMode {
        NONE=0,         ///<No automatic zoom setting after compilation - keep what zoom we had before.
        OVERVIEW,       ///<Zoom such that the whole chart (or its current page) fits the view.
        ORIGSIZE,       ///<Zoom back to 100%
        ZOOM_FITTOWIDTH ///<Zoom such that the whole chart (or its current page) fits the *width* view.
    } m_ZoomMode; ///<What is the current zoom mode selected by the user.
    //@}
    /** @name Animations and Tracking mode related members
    * @{ */
    bool m_bTrackMode;                           ///<True if tracking mode is on - if the mouse is tracked over arcs
    Contour m_fallback_image_location;           ///<The location of the fallback images extracted during rendering to an EMF cache
	std::vector<AnimationElement> m_animations;  ///<The currently visible list of animations
    std::map<Block, Element*> m_controlsShowing; ///<Controls currently appearing
	CHARRANGE m_saved_charrange;                 ///<The character range we had before the tracking mode started - this is re-applied in those moments when we do not point to any element during tracking mode
	Element *m_last_arc;                         ///<During tracking the arc highlighted in the editor
	CView *m_pViewFadingTimer;                   ///<Our view in which track rectangles show
	//@}
	static CLIPFORMAT m_cfPrivate;    ///<Clipboard format for chart objects
	CExternalEditor m_ExternalEditor; ///<A hidden window to do external editor functions
	bool m_bAttemptingToClose;        ///<This flag indicates to compilation functions if we are closing, set during CanCloseFrame()

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
            void DoExport(bool pdfOnly);
    afx_msg void OnPreviewExport();
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
    afx_msg void OnErrorSelected(unsigned line, unsigned col); //Another error is selected

	void ChangeDesign(const char *design);
    void ChangePage(unsigned page);
    void StepPage(signed int step);

	//Zoom functions
			bool SetZoom(int zoom=0);                        //Update views with new zoom factor. ==0 means just reset toolbar, Returns true if we have actually changed zoom .
			bool ArrangeViews(EZoomMode mode);               //Automatically adjust zoom. Returns true if we have done something.
			bool ArrangeViews() {return ArrangeViews(m_ZoomMode);}  ///<Automatically adjust zoom using existng zoom mode (if any) Returns true if we have done something.
	afx_msg void OnViewZoomnormalize();                      //Automatically adjust zoom: overview mode
	afx_msg void OnView100Percent();                         //Automatically adjust zoom: set to 100%
	afx_msg void OnViewFittowidth();                         //Automatically adjust zoom: set width to window
    afx_msg void OnUpdateViewZoom(CCmdUI *pCmdUI);
            void SwitchZoomMode(EZoomMode mode);             //Set zoom mode to specified value (and adjust zoom)
	afx_msg void OnZoommodeKeepinoverview();                 //Set zoom mode
	afx_msg void OnZoommodeKeep100Percent();       //Set zoom mode
	afx_msg void OnZoommodeKeepfittingtowidth();             //Set zoom mode
	afx_msg void OnUpdateZoommodeKeepinoverview(CCmdUI *pCmdUI);
    afx_msg void OnZoommodeKeep100Percent(CCmdUI *pCmdUI);
	afx_msg void OnUpdateZoommodeKeepfittingtowidth(CCmdUI *pCmdUI);

	        bool DispatchMouseWheel(UINT nFlags, short zDelta, CPoint pt);

public:
	void InsertNewChart(const CChartData &);             //insert a new chart into the list (destroys redo, updates iterators)
	bool CheckIfChanged();                               //Check if we have changed and updates SetModifiedFlag
    void OnExternalEditorChange(const CChartData &data); //this is called by m_ExternalEditor if the text in the external editor changes
    void OnInternalEditorChange(long start, long ins, long del, CHARRANGE sel_before);//this is called by CMiniEditor if the text in the internal editor changes
    void OnInternalEditorSelChange();                    //this is called by CMiniEditor if the selection in the internal editor changes
	void CompileEditingChart(bool resetZoom, bool force_block, bool force_compile);//Call this to show the currently edited chart, it compiles and updates the views, calls NotifyChanged()
    void CompleteCompilingEditingChart();                //This will be called once the compilation is complete
    void KillCompilation();                              //Abort the compilation
    void WaitForCompilationToEnd();                      //Wait till compilation ends

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
    afx_msg void OnEditSelectAll();
};

CMscGenDoc *GetMscGenDocument();

