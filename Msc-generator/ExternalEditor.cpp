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

    You should have received a copy of the GNU Affero General Public License    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stdafx.h"
#include "process.h"
#include "ChartData.h"
#include "ExternalEditor.h"
#include "Msc-generator.h"
#include "MscGenDoc.h"

BEGIN_MESSAGE_MAP(CExternalEditor, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()

CExternalEditor::CExternalEditor(CMscGenDoc *pDoc) {
	m_ProcessId = 0;
	m_hWndForEditor = NULL;
	m_hTimer = NULL;
	m_pDoc = pDoc;
}

void CExternalEditor::Start(CString filename)
{
	if (m_ProcessId) return;
	//Determine filename
	//Reuse existing filename, if no new name is given
	if (filename.GetLength() > 0 || m_FileName.GetLength() == 0) {
		if (filename.GetLength() == 0) {
			if (m_pDoc->IsEmbedded()) {
				if (m_pDoc->IsInPlaceActive()) {
					// In-place active embedded charts
					filename = "In-place Chart";
				} else {
					// Embedded charts opened in a separate window
					AfxGetMainWnd()->GetWindowText(filename);
					if (filename.Right(16) == " - Msc-generator")
						filename = filename.Left(filename.GetLength()-16);
					else if (filename.Right(13) == "Msc-generator")
						filename = filename.Left(filename.GetLength()-13);
				}
			} else {
				// Standalone opened charts
				filename = "Unsigned";
			}
		}
		//Remove path info, keep filename only
		int pos = filename.ReverseFind('\\');
		if (pos >0)
			filename.Delete(0, pos+1);
		//Add random number and extension and prepend TEMP dir
		m_FileName.Format("%04d - Text of ", rand()%1000);
		m_FileName.Append(filename);
		if (m_FileName.Right(11)!=".signalling")
			m_FileName.Append(".signalling");
		char buff[1024];
		GetTempPath(sizeof(buff), buff);
		m_FileName.Insert(0, buff);
	}
	//Save file, record last modification time (pretty much now)
	m_pDoc->m_itrEditing->Save(m_FileName);
	CFileStatus status;
	CFile::GetStatus(m_FileName, status);
	m_FileLastMod = status.m_mtime;

	//Fire up editor process
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	CString cmdLine = pApp->m_sStartTextEditor;
	cmdLine.Replace("%n", "\"" + CString(m_FileName) + "\"");
	char* cmdline = strdup(cmdLine);
	STARTUPINFO si;
	si.cb = sizeof(STARTUPINFO);
	si.lpDesktop = NULL;
	si.lpTitle = strdup(m_FileName.GetString());
	si.dwFlags = 0;
	si.lpReserved = NULL;
	si.lpReserved2 = NULL;
	si.cbReserved2 = NULL;
	PROCESS_INFORMATION pi;
	if (!CreateProcess(NULL, cmdline, NULL, NULL, FALSE, 0, NULL, NULL,  &si, &pi)) {
		free(cmdline);
		free(si.lpTitle);
		Stop(STOPEDITOR_FORCE); //Delete temp file and null the filename
		return;
	}
	m_ProcessId = pi.dwProcessId;
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	free(cmdline);
	free(si.lpTitle);

	m_hTimer = SetTimer(1, 100, NULL);
	if (pApp->IsInternalEditorRunning()) 
		pApp->m_pWndEditor->SetReadOnly();
}

void CExternalEditor::JumpToLine(unsigned line, unsigned col)
{
	if (line==0) return;
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	//Jump in the external editor
	if (m_FileName.GetLength()==0 || m_ProcessId==0) return;
	if (pApp->m_sJumpToLine.GetLength()==0) return;
	CString cmdLine = pApp->m_sJumpToLine;
	CString num;
	num.Format("%u", line);
	cmdLine.Replace("%l", num);
	cmdLine.Replace("%n", "\"" + CString(m_FileName) + "\"");

	char* cmdline = strdup(cmdLine);
	STARTUPINFO si;
	si.cb = sizeof(STARTUPINFO);
	si.lpDesktop = NULL;
	si.lpTitle = strdup(m_FileName.GetString());
	si.dwFlags = 0;
	si.lpReserved = NULL;
	si.lpReserved2 = NULL;
	si.cbReserved2 = NULL;
	PROCESS_INFORMATION pi;
	CreateProcess(NULL, cmdline, NULL, NULL, FALSE, 0, NULL, NULL,  &si, &pi);
	free(cmdline);
	free(si.lpTitle);
}
//Check if editor process is still running - shuts us down if not
//Check if file has been updated
//returns true if data is changed and countains the new text
void CExternalEditor::OnTimer(UINT nIDEvent)
{
	if (m_FileName.GetLength()>0) {
		CFileStatus status;
		//Query file status
		if (CFile::GetStatus(m_FileName, status)) {
			//Check if the file has been updated by the editor
			if (m_FileLastMod != status.m_mtime) {
				CChartData data;
				//Load modification
				if (data.Load(m_FileName, FALSE)) {
					m_FileLastMod = status.m_mtime;
					//OK: File successfully read, update the Document.
					data.SetDesign(m_pDoc->m_itrEditing->GetDesign());
					m_pDoc->OnExternalEditorChange(data);
					CWnd::OnTimer(nIDEvent);
					return;
				}
			}
		} else {
			//Editor file not possible to stat
			m_FileName.Empty();
		}
	}
	//File deleted! Check if editor is still running
	if (m_ProcessId) {
		HANDLE h = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, m_ProcessId);
		if (!GetPriorityClass(h)) { //this checks if process still there
			Stop(STOPEDITOR_FORCE); //remove editor file & zero out 
		}
		CloseHandle(h);
	}
	CWnd::OnTimer(nIDEvent);
}


struct window_search {
	DWORD processId;
	std::list<HWND> hWnd_result;
};

BOOL CALLBACK EnumWindowsProcFindTopWindowByProcess(HWND hwnd, LPARAM lParam)
{
	if (!(GetWindowLong(hwnd,GWL_STYLE) & WS_VISIBLE)) return true; //only consider visible windows
	DWORD processId;
	GetWindowThreadProcessId(hwnd, &processId);
	if (processId == reinterpret_cast<struct window_search*>(lParam)->processId)
		reinterpret_cast<window_search*>(lParam)->hWnd_result.push_back(hwnd);
	return true; //continue iteration
}

void CExternalEditor::Stop(EStopEditor force)
{
	if (m_ProcessId) {
		switch (force) {
		case STOPEDITOR_FORCE:
			{
			HANDLE h = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_ProcessId);
			TerminateProcess(h, 0);
			CloseHandle(h);
			//continue with removing editor file and view timers
			break;
			}
		case STOPEDITOR_NOWAIT:
		case STOPEDITOR_WAIT:
			struct window_search search;
			search.processId = m_ProcessId;
			EnumWindows(EnumWindowsProcFindTopWindowByProcess, (LPARAM)&search);
			int ii = search.hWnd_result.size();
			for (std::list<HWND>::iterator i=search.hWnd_result.begin(); i!=search.hWnd_result.end(); i++) 
				if (::IsWindow(*i)) {
					char buff[1024];
					int len = ::GetWindowText(*i, buff, sizeof(buff)); 
					if (strstr(buff, "signalling")!=NULL) {
						::SetForegroundWindow(*i);
						::SendMessage(*i, WM_CLOSE, 0, 0); 
					}
			}
			if (force == STOPEDITOR_NOWAIT)
				//Okay let us wait till the editor actually closes
				//CView::OnTimer will come back to us in CheckEditorAndFile and when editor is gone
				//MscGenDoc::StopExternalEditor will be called with STOPEDITOR_FORCE, which will remove timers and editor file
				return;
			//If we are forced to close, wait till editor process closes
			bool wait = true;
			while (wait) { 
				HANDLE h = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, m_ProcessId);
				wait = GetPriorityClass(h);  //this checks if process still there
				CloseHandle(h);
				Sleep(300);
			}
		}
		//Indicate that the editor process is gone
		m_ProcessId = 0; 
	}
	//update the document if editor has made last minute edits
	OnTimer(1); 
	//Remove timer
	if (::IsWindow(m_hWnd)) KillTimer(m_hTimer);
	m_hTimer = NULL;
	//Remove file, if still there
	if (m_FileName.GetLength()>0) {
		TRY {
			CFile::Remove(m_FileName);
		} CATCH(CFileException, pEx) {
		}
		END_CATCH
		m_FileName.Empty();
	}
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	if (pApp->IsInternalEditorRunning()) 
		pApp->m_pWndEditor->SetReadOnly(false);
}

//Restarts editor, if it is running
void CExternalEditor::Restart(EStopEditor force)
{
	if (!IsRunning()) return;
	Stop(force);
	Start();
}

