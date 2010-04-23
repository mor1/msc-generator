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
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;
	COutputViewBar *m_pWndOutputView;
	CEditorBar     *m_pWndEditor;
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();
	//bool CommandLineMain(CString cmd);

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMscGenApp theApp;

//Registry data keys
#define REG_SECTION_SETTINGS "Settings"
#define REG_KEY_PEDANTIC "Pedantic"
#define REG_KEY_WARNINGS "Warnings"
#define REG_KEY_TEXTEDITORTYPE "TextEditorType"
#define REG_KEY_STARTTEXTEDITOR "TextEditorCommand"
#define REG_KEY_JUMPTOLINE "JumpToLineCommand"
#define REG_KEY_DEFAULTTEXT "DefaultText"
#define REG_KEY_DEFAULTZOOMMODE "DefaultZoomMode"
