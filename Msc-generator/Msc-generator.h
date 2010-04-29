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

// Msc-generator.h : main header file for the Msc-generator application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "OutputView.h"
#include "MiniEditor.h"
#include "csh.h"


// CMscGenApp:
// See Msc-generator.cpp for the implementation of this class
//

#define CSH_SCHEME_MAX 4

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

//Options
	bool m_Pedantic;
	BOOL m_Warnings;
	BOOL m_bPB_Editing;
	BOOL m_bPB_Embedded;
	CString m_ChartSourcePreamble;
	CString m_SetOfDesigns;
	CString m_CopyrightText;
	BOOL m_bAlwaysOpen;
	BOOL m_bCsh;
	int m_nCshScheme;
	CHARFORMAT m_csh_cf[CSH_SCHEME_MAX][COLOR_MAX];

	//Editor related
	enum EEditorType {NOTEPAD=0, NPP=1, OTHER=2, INTERNAL=3} m_iTextEditorType;
	CString m_sStartTextEditor;
	CString m_sJumpToLine;
	CString m_DefaultText;
	CString m_NppPath;


	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();
	//bool CommandLineMain(CString cmd);

	void ReadRegistryValues(bool reportProblem);
	bool ReadDesigns(bool reportProblem=false, const char *fileName="designlib.signalling");
	bool FillDesignDesignCombo(const char *current, bool updateComboContent=false);
	void FillDesignPageCombo(int no_pages, int page);

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
