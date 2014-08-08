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

/** @file Msc-generator.h Main header file for the Msc-generator application
 * @ingroup Msc_generator_files */

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "ChartData.h"
#include "OutputView.h"
#include "MiniEditor.h"
#include "MscGenDoc.h"
#include "csh.h"
#include "mscdrawer.h"

/** Gets Alpha channel value. 
 * Complementing WinGDI macros (like GetRValue())
 * for trackrects color fill and line*/
#define GetAValue(rgb)      (LOBYTE((rgb)>>24))
/** Coverts a color having alpha channel to a 32-bit COLORREF */
#define RGBA(r,g,b,a)       ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)|(((DWORD)(BYTE)(a))<<24)))

/** The main application class.
 * This is an SDI application, so only one Document per app.*/
class CMscGenApp : public CWinAppEx
{
    /** Get our one and only document */
    CMscGenDoc *GetDoc(void);
public:
	CMscGenApp();

// Overrides
public:
	virtual BOOL InitInstance();
    int ExitInstance();

// Implementation
	COleTemplateServer m_server;                 ///<Server object for document creation
	UINT               m_nAppLook;               ///<The visual style of the Ribbon and other MFC components
	COutputViewBar    *m_pWndOutputView;         ///<Shortcut to the error window
	CEditorBar        *m_pWndEditor;             ///<Shortcut to the internal editor
    PageSizeInfo::EPageSize m_pageSize;          ///<Raw printer page size, NO_PAGE means a custom size
    XY                 m_PhyPrinterPageSize;     ///<raw page size in points
    double             m_printer_phy_margins[4]; ///<left,right, up, down physical margins of the printer in points
    double             m_printer_usr_margins[4]; ///<left,right, up,down margins specified by the user in points
    XY                 m_PrinterScale;           ///<The scale to apply so that we get from points to printer pixels

    ///< Return printable area in points (physical page - user margins)
    XY GetPrintablePaperSize() const {return m_PhyPrinterPageSize - XY(m_printer_usr_margins[0] + m_printer_usr_margins[1], m_printer_usr_margins[2] + m_printer_usr_margins[3]);}

//Status 
	bool m_bFullScreenViewMode; ///<True if we were opened by "ViewFullScreen OLE verb"

    /** @name User selected preferences
     * These can be set on the preferences category of the ribbon. */
    /** @{ */
	bool m_Pedantic;                     ///<True if pedantic compilation should be used
	bool m_Warnings;                     ///<True if warnings are to be displayed
	bool m_bPageBreaks;                  ///<True if page breaks are to be displayed
	unsigned m_uFallbackResolution;      ///<The resolution of fallback images (OLE objects)
    std::map<string, Context> m_Designs; ///<The list of designs in the design library.
    ShapeCollection m_Shapes;            ///<The list of shapes in the design/shape library
    MscError m_ShapeErrors;              ///<Errors collected during the compilation of the shape files.
    MscError m_DesignErrors;             ///<Errors collected during the compilation of the design lib.
    Csh m_designlib_csh;                 ///<The color & other info collected during design library. Used to seed the Csh of the internal editor.
	CString m_CopyrightText;             ///<The copyright text to display at the bottom of the charts.
	bool m_bShowCsh;                     ///<True if we show CSH markings in internal editor.
    bool m_bShowCshErrors;               ///<True if we show CSH discovered parse errors
    bool m_bShowCshErrorsInWindow;       ///<True if we list the CSH discovered parse errors in the output window.
	unsigned m_nCshScheme;               ///<Which color shceme do we use for CSH
    bool m_bSmartIndent;                 ///<Whether we do smart indent or not. If true m_csh.ColonLabels are kep up-to-date at every change
    bool m_bTABIndents;                  ///<Whether TAB indents the line (true) or inserts a tab
    CHARFORMAT m_csh_cf[CSH_SCHEME_MAX][COLOR_MAX]; ///<The colors used for each color scheme
	COLORREF m_trackFillColor;           ///<The color used to fill element tracking outlines
	COLORREF m_trackLineColor;           ///<The color used to draw element tracking outlines
    bool m_bHints;                       ///<True if hinting is turned on
    bool m_bHintLineStart;               ///<true if we provide hints at the beginning of a line
    bool m_bHintEntity;                  ///<True if we provide hints for entity names
    bool m_bHintAttrName;                ///<True if we provide hints for attribute names
    bool m_bHintAttrValue;               ///<True if we provide hints for attribute values
    bool m_bHintFilter;                  ///<True if we filter the hint list to possible candidates
    bool m_bHintCompact;                 ///<True if we compact the hint list hiding hints with same prefix
    bool m_bShowControls;                ///<True if we show element controls (collapse/expand)
    bool m_bAutoPaginate;                ///<True if we auto-paginate the chart
    bool m_bAutoHeading;                 ///<True if we add an auto heading for each automatic page break
    int m_iScale4Pagination;             ///<The scale used for auto-pagination. In percentage, -1=Fit Witdth -2=Fit Page
    int m_iPageAlignment;                ///<Alignment of chart in the fix-size page: -1/0/+1 left/center/right + -3/0/+3 bottom/center/top

	//Editor related
    /** Types of External editor */
	enum EEditorType {
        NOTEPAD=0, ///<Windows notepad
        NPP=1,     ///<Notepad++ (supports jump into file)
        OTHER=2    ///<Some other external editor
    } m_iTextEditorType; ///<Which external editor to use
	CString m_sStartTextEditor; ///<Command line to start the "other" external editor
	CString m_sJumpToLine;      ///<Command line to jump to a specific line in the "other" external editor
	CString m_NppPath;          ///<The path of the Notepad++ program, empty if not installed here.
	CString m_DefaultText;      ///<The default text of a new chart
    /** @} */

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

    void UpdatePrinterData();
    bool NormalizeUserMargins();
    void DisplayErrors(const MscError &error, bool warnings, const char *type);
	void ReadRegistryValues(bool reportProblem);
    int ReadDesigns(unsigned folder, const char *fileName);
    int ReadShapes(unsigned folder, const char *fileName);
    /** True if the internal editor is running */
	bool IsInternalEditorRunning() const {
		return m_pWndEditor && IsWindow(m_pWndEditor->m_hWnd) /* && m_pWndEditor->IsVisible()*/;}
    
	DECLARE_MESSAGE_MAP()
    /** @name GUI control functions
     * These functions are called when a control is invoked on the GUI. */
    /** @{ */
	afx_msg void OnAppAbout();
    afx_msg void OnHelp();
    afx_msg void OnFilePrintSetup();
    afx_msg void OnUpdatePrintSetup(CCmdUI *pCmdUI);
	afx_msg void OnEditPreferences();
    afx_msg void OnButtonDefaultText();
    afx_msg void OnUpdateButtonDefaultText(CCmdUI *pCmdUI);
    afx_msg void OnCheckPedantic();
    afx_msg void OnUpdateCheckPedantic(CCmdUI *pCmdUI);
    afx_msg void OnCheckWarnings();
    afx_msg void OnUpdateCheckWarnings(CCmdUI *pCmdUI);
    afx_msg void OnCheckPageBreaks();
    afx_msg void OnUpdateCheckPageBreaks(CCmdUI *pCmdUI);
    afx_msg void OnCheckCsh();
    afx_msg void OnComboCsh();
    afx_msg void OnUpdateCheckCsh(CCmdUI *pCmdUI);
    afx_msg void OnUpdateComboCsh(CCmdUI *pCmdUI);
    afx_msg void OnCheckSmartIndent();
    afx_msg void OnCheckTABIndents();
    afx_msg void OnUpdateCheckSmartIndent(CCmdUI *pCmdUI);
    afx_msg void OnCheckCshError();
    afx_msg void OnUpdateCheckTABIndents(CCmdUI *pCmdUI);
    afx_msg void OnUpdateCheckCshError(CCmdUI *pCmdUI);
    afx_msg void OnCheckCshErrorInWindow();
    afx_msg void OnUpdateCheckCshErrorInWindow(CCmdUI *pCmdUI);
    afx_msg void OnUpdateEditPreferences(CCmdUI *pCmdUI);
    afx_msg void OnCheckEeOther();
    afx_msg void OnCheckEeNotepad();
    afx_msg void OnCheckEeNotepadpp();
    afx_msg void OnUpdateCheckEeOther(CCmdUI *pCmdUI);
    afx_msg void OnUpdateCheckEeNotepad(CCmdUI *pCmdUI);
    afx_msg void OnUpdateCheckEeNotepadpp(CCmdUI *pCmdUI);
    afx_msg void OnCheckHints();
    afx_msg void OnCheckSmartHintCompact();
    afx_msg void OnCheckSmartHintFilter();
    afx_msg void OnCheckSmartHintLineStart();
    afx_msg void OnCheckSmartHintEntity();
    afx_msg void OnCheckSmartHintAttrName();
    afx_msg void OnCheckSmartHintAttrValue();
    afx_msg void OnUpdateCheckSmartHintBoxes(CCmdUI *pCmdUI);
    afx_msg void OnUpdateCheckHints(CCmdUI *pCmdUI);
    afx_msg void OnButtonTrackColor();
    afx_msg void OnEmbeddedoptionsFallbackRes();
    afx_msg void OnAutoPaginate();
    afx_msg void OnUpdateAutoPaginate(CCmdUI *pCmdUI);
    afx_msg void OnAutoHeaders();
    afx_msg void OnUpdateAutoHeaders(CCmdUI *pCmdUI);
    afx_msg void OnComboScale();
    afx_msg void OnButtonPages();
    afx_msg void OnComboAlignment();
    afx_msg void OnEditMarginL() { DoEditMargin(ID_EDIT_MARGIN_L); }
    afx_msg void OnEditMarginR() {DoEditMargin(ID_EDIT_MARGIN_R);}
    afx_msg void OnEditMarginT() {DoEditMargin(ID_EDIT_MARGIN_T);}
    afx_msg void OnEditMarginB() {DoEditMargin(ID_EDIT_MARGIN_B);}
            void DoEditMargin(UINT id); 
    afx_msg void OnUpdatePrintPreviewEdits(CCmdUI *pCmdUI);
    /** @} */

};

extern CMscGenApp theApp;

//Registry data keys
#define REG_SECTION_SETTINGS "Settings"
#define REG_KEY_PEDANTIC "Pedantic"
#define REG_KEY_WARNINGS "Warnings"
#define REG_KEY_PB_EDITING "PageBreak_Editing"
#define REG_KEY_FALLBACK_RESOLUTION "Default_Fallback_Resolution"
#define REG_KEY_ALWAYSOPEN "AlwaysOpen"
#define REG_KEY_TEXTEDITORTYPE "TextEditorType"
#define REG_KEY_STARTTEXTEDITOR "TextEditorCommand"
#define REG_KEY_JUMPTOLINE "JumpToLineCommand"
#define REG_KEY_DEFAULTTEXT "DefaultText"
#define REG_KEY_DEFAULTZOOMMODE "DefaultZoomMode"
#define REG_KEY_CSHENABLED "CshEnabled"
#define REG_KEY_CSHSCHEME "CshScheme"
#define REG_KEY_SMARTINDENT "SmartIdent"
#define REG_KEY_TABINDENTS "TABIdents"
#define REG_KEY_CSHERRORS "CshErrorsEnabled"
#define REG_KEY_CSHERRORSINWINDOW "CshErrorsInWindow"
#define REG_KEY_FINDWINPOS_X "FindReplaceWindowXPos"
#define REG_KEY_FINDWINPOS_Y "FindReplaceWindowYPos"
#define REG_KEY_FINDMATCHCASE "FindReplaceMatchCase"
#define REG_KEY_TRACKFILLRGBA "TrackFillRGBA"
#define REG_KEY_TRACKLINERGBA "TrackLineRGBA"
#define REG_KEY_NOREMIND_VERSION_MAJOR "NoRemindVersionMajor"
#define REG_KEY_NOREMIND_VERSION_MINOR "NoRemindVersionMinor"
#define REG_KEY_NOREMIND_VERSION_SUPER_MINOR "NoRemindVersionSuperMinor"
#define REG_KEY_LAST_VERSION_SEEN_MAJOR "NoLastVersionSeenMajor"
#define REG_KEY_LAST_VERSION_SEEN_MINOR "NoLastVersionSeenMinor"
#define REG_KEY_LAST_VERSION_SEEN_SUPER_MINOR "NoLastVersionSeenSuperMinor"
#define REG_KEY_LAST_VERSION_SEEN_URL "NoLastVersionSeenSuperURL"
#define REG_KEY_TABSIZE "TabSize"
#define REG_KEY_HINT "DoHints"
#define REG_KEY_HINT_LINESTART "HintLineStart"
#define REG_KEY_HINT_ENTITY "HintEntity"
#define REG_KEY_HINT_ATTRNAME "HintAttrName"
#define REG_KEY_HINT_ATTRVALUE "HintAttrValue"
#define REG_KEY_HINT_COMPACT "HintCompact"
#define REG_KEY_HINT_FILTER "HintFilter"
#define REG_KEY_SHOW_CONTROLS "ShowElementControls"
#define REG_KEY_LOAD_DATA "LoadData"
#define REG_KEY_AUTO_PAGINATE "AutoPaginate"
#define REG_KEY_AUTO_HEADING "AutoHeading"
#define REG_KEY_SCALE4PAGINATION "Scale4Pagination"
#define REG_KEY_PAGE_ALIGNMENT "PageAlignment"
#define REG_KEY_PAGE_MARGIN_L "PageMarginL"
#define REG_KEY_PAGE_MARGIN_R "PageMarginR"
#define REG_KEY_PAGE_MARGIN_T "PageMarginT"
#define REG_KEY_PAGE_MARGIN_B "PageMarginB"

