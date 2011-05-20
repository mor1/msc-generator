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
    along with Msc-generator.  If not, see <http://www.gnu.org/licenses/>.
*/


#pragma once

class CMscGenDoc;

enum EStopEditor {STOPEDITOR_NOWAIT, STOPEDITOR_WAIT, STOPEDITOR_FORCE};

class CExternalEditor : CWnd {
	DWORD m_ProcessId; 
	CString m_FileName;
	CTime m_FileLastMod;
	HWND m_hWndForEditor;
	UINT_PTR m_hTimer;
	CMscGenDoc *m_pDoc;

public:
	CExternalEditor(CMscGenDoc *pDoc);
	bool CanStart() const {return ::IsWindow(m_hWnd);}
	bool Init() {return Create(NULL, "hidden window for external editor", 0, CRect(), AfxGetMainWnd(), 0);}
	void Start(CString filename = "");
	void Restart(EStopEditor force);
	bool IsRunning() const {return m_ProcessId!=0;}
	void JumpToLine(unsigned line, unsigned col);
	bool Check(CChartData &data);
	void Stop(EStopEditor force);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT nIDEvent);
};