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
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

// Msc-generator.h : main header file for the Msc-generator application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "ChartData.h"
#include "OutputView.h"
#include "MiniEditor.h"
#include "csh.h"


//Extending WinGDI definitions for trackrects color fill and line
#define GetAValue(rgb)      (LOBYTE((rgb)>>24))
#define RGBA(r,g,b,a)       ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)|(((DWORD)(BYTE)(a))<<24)))

// CMscGenApp:
// See Msc-generator.cpp for the implementation of this class
//

class CMscGenApp : public CWinAppEx
{
public:
	CMscGenApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	COleTemplateServer m_server; // Server object for document creation
	UINT               m_nAppLook;
	BOOL			   m_bHiColorIcons;
	COutputViewBar    *m_pWndOutputView;
	CEditorBar        *m_pWndEditor;

//Status 
	bool m_bFullScreenViewMode; //we were opened by "ViewFullScreen OLE verb"

//Options
	bool m_Pedantic;
	BOOL m_Warnings;
	BOOL m_bPB_Editing;
	BOOL m_bPB_Embedded;
	CString m_ChartSourcePreamble;
	CString m_SetOfDesigns;
	CString m_CopyrightText;
	BOOL m_bAlwaysOpen;
    BOOL m_bDoCshProcessing;
	BOOL m_bShowCsh;
    BOOL m_bShowCshErrors;
    BOOL m_bShowCshErrorsInWindow;
	int m_nCshScheme;
	BOOL m_bSmartIdent;
	CHARFORMAT m_csh_cf[CSH_SCHEME_MAX][COLOR_MAX];
	COLORREF m_trackFillColor;
	COLORREF m_trackLineColor;
    bool m_bHints;
    bool m_bHintLineStart;
    bool m_bHintEntity;
    bool m_bHintAttrName;
    bool m_bHintAttrValue;
    bool m_bHintFilter;
    bool m_bHintCompact;
    CDrawingChartData::ECacheType m_cacheType;

	//Editor related
	enum EEditorType {NOTEPAD=0, NPP=1, OTHER=2} m_iTextEditorType;
	CString m_sStartTextEditor;
	CString m_sJumpToLine;
	CString m_DefaultText;
	CString m_NppPath;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	void ReadRegistryValues(bool reportProblem);
	bool ReadDesigns(bool reportProblem=false, const char *fileName="designlib.signalling");
	bool FillDesignDesignCombo(const char *current, bool updateComboContent=false);
	void FillDesignPageCombo(int no_pages, int page);
	bool IsInternalEditorRunning() const {
		return m_pWndEditor && IsWindow(m_pWndEditor->m_hWnd) /* && m_pWndEditor->IsVisible()*/;}

	afx_msg void OnAppAbout();
	afx_msg void OnEditPreferences();
	DECLARE_MESSAGE_MAP()
};

extern CMscGenApp theApp;

//Registry data keys
#define REG_SECTION_SETTINGS "Settings"
#define REG_KEY_PEDANTIC "Pedantic"
#define REG_KEY_WARNINGS "Warnings"
#define REG_KEY_PB_EDITING "PageBreak_Editing"
#define REG_KEY_PB_EMBEDDED "PageBreak_Embedded"
#define REG_KEY_ALWAYSOPEN "AlwaysOpen"
#define REG_KEY_TEXTEDITORTYPE "TextEditorType"
#define REG_KEY_STARTTEXTEDITOR "TextEditorCommand"
#define REG_KEY_JUMPTOLINE "JumpToLineCommand"
#define REG_KEY_DEFAULTTEXT "DefaultText"
#define REG_KEY_DEFAULTZOOMMODE "DefaultZoomMode"
#define REG_KEY_CSHENABLED "CshEnabled"
#define REG_KEY_CSHSCHEME "CshScheme"
#define REG_KEY_SMARTIDENT "SmartIdent"
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
#define REG_KEY_TABSIZE "TabSize"
#define REG_KEY_HINT "DoHints"
#define REG_KEY_HINT_LINESTART "HintLineStart"
#define REG_KEY_HINT_ENTITY "HintEntity"
#define REG_KEY_HINT_ATTRNAME "HintAttrName"
#define REG_KEY_HINT_ATTRVALUE "HintAttrValue"
#define REG_KEY_HINT_COMPACT "HintCompact"
#define REG_KEY_HINT_FILTER "HintFilter"

