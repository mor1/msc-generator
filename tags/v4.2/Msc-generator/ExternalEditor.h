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
/** @file ExternalEditor.h Things needed for the external editors to run.
* @ingroup Msc_generator_files */


#pragma once

class CMscGenDoc;

/** Ways to stop the external editor. */
enum EStopEditor {
    STOPEDITOR_NOWAIT,  ///<Initiate closing of the external editor, but do not wait for completion.
    STOPEDITOR_WAIT,    ///<Initiate closing of the external editor and wait till it closes (e.g., user saves the text)
    STOPEDITOR_FORCE    ///<Kill external editor without allowing user to save
};

/** Invisible window managing external editor*/
class CExternalEditor : CWnd {
	DWORD m_ProcessId;   ///<Process id of the external editor exec.
	CString m_FileName;  ///<The name of the text file, the user is editing
	CTime m_FileLastMod; ///<Last update time of the text file
	HWND m_hWndForEditor;///<Handle for the window of the external editor (to send msgs)
	UINT_PTR m_hTimer;   ///<Timer used to periodically check for modification
	CMscGenDoc *m_pDoc;  ///<Pointer to the document

public:
    /** Initialize us */
	CExternalEditor(CMscGenDoc *pDoc);
    /** True, if creation is successful and we can launch the external editor.*/
	bool CanStart() const {return ::IsWindow(m_hWnd);}
    /** Create a hidden external editor for a document*/
	bool Init() {return Create(NULL, "hidden window for external editor", 0, CRect(), AfxGetMainWnd(), 0);}
    /** Start the external Editor */
	void Start(CString filename = "");
    /** Stop external editor as described in 'force' and start it again*/
	void Restart(EStopEditor force);
    /** True if the external editor is running */
	bool IsRunning() const {return m_ProcessId!=0;}
    /** Invoke the external editor to jump to a place*/
	void JumpToLine(unsigned line, unsigned col);
    /**Stop the external editor as described by 'force'*/
	void Stop(EStopEditor force);

	DECLARE_MESSAGE_MAP()
    /** This gets called if the external editor runs and we check if the file has
     * been updated. In that case we call m_pDoc->OnExternalEditorChange();*/
	afx_msg void OnTimer(UINT nIDEvent);
};