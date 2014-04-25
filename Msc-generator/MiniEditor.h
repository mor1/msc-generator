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

#pragma once
#undef min
#undef max
#include <msc.h>
#include "PopupList.h"

/////////////////////////////////////////////////////////////////////////////
// CMiniEditor window


struct CEditorUndoRecord {
	CString text;
	CHARRANGE pos;
};

class CEditorBar;

class CCshRichEditCtrl : public CRichEditCtrl
{
protected:
    DECLARE_MESSAGE_MAP()
    LRESULT OnPasteSpecial(WPARAM wParam, LPARAM lParam);

private:
    bool m_bWasReturnKey;        ///<True if the return key was pressed. Needed to se if we shall show the line begin hints or not.
    bool m_bUserRequested;       //the incarnation of the hints session was due to Ctrl+Space
    bool m_bTillCursorOnly;      //the incarnation of this hints session started at the beginning of a word
    bool m_bWasAutoComplete;     //Set to prevent entering hint mode after an auto-completion
    CPopupList m_hintsPopup;
    CEditorBar * const m_parent;
    CHARRANGE m_crSel_before;    //The selection before a change
    CString m_prev_text;         //The previous text (for which m_csh) is relevant. Used in UpdateCSH().
    typedef std::pair<bool, bool> WindowUpdateStatus;
    bool m_bRedrawState;         //Store what is the redraw State, since we have no GetRedraw()
public:
    typedef enum UpdateCSHType {
        HINTS_AND_COLON_LABELS = 0,
        CSH = 1,
        FORCE_CSH = 2
    };
    Csh  m_csh;
    int m_tabsize;
	CCshRichEditCtrl(CEditorBar *parent);
    virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	//Generic helpers
	void GetSelLineCol(int &lStartLine, int &lStartCol, int &lEndLine, int &lEndCol) const;
	int  GetLineString(int line, CString &str);
	long ConvertLineColToPos(unsigned line, unsigned col) const {return LineIndex(line) + col;}
	void ConvertPosToLineCol(long pos, int &line, int &col) const {line=LineFromChar(pos); col=pos-LineIndex(line);}
	void JumpToLine(int line, int col);

	//Functions for intelligent identation
	int  FirstNonWhitespaceIdent(const char *str, int Max=-1);
	int  LastNonWhitespaceIdent(const char *str, int Max=-1);
	int  FindColonLabelIdent(long lStart, int *line=NULL);
    int  FindProperLineIdent(int lStart);
    int  FindCurrentLineIdent(long lStart);
	int  FindIdentForClosingBrace(int pos_to_be_inserted);
	int  CalcTabStop(int col, bool forward, int smartIdent=-1, int prevIdent=0, bool strict=false);
    bool SetCurrentIdentTo(int target_ident, int current_ident, int col, long lStart, long lEnd, bool standalone);
	BOOL PreTranslateMessage(MSG* pMsg);
    WindowUpdateStatus GetWindowUpdate();
    WindowUpdateStatus DisableWindowUpdate();
    void RestoreWindowUpdate(const WindowUpdateStatus &s);

	//Color Syntax Highlighting functions
	void UpdateText(const char *text, CHARRANGE &cr);
    void UpdateText(const char *text, int lStartLine, int lStartCol, int lEndLine, int lEndCol);
    bool UpdateCSH(UpdateCSHType updateCSH);
	void CancelPartialMatch();
	

	//Mouse Wheel handling
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt); 
	        BOOL DoMouseWheel(UINT nFlags, short zDelta, CPoint pt); 
    //Hint window related
    void StartHintMode(bool setUptoCursor); //also used to update hints in popup, if already in hint mode
    bool InHintMode() const {return m_hintsPopup.m_shown;}
    void CancelHintMode();
    void CancelUserSelected() {m_bUserRequested = false;}
    void ReplaceHintedString(const char *substitute, bool endHintMode);
};

class CMainFrame;

class CEditorBar : public CDockablePane
{
// Construction
public:
	CEditorBar();

// Attributes
    CFont m_Font;
	CCshRichEditCtrl m_ctrlEditor;
	CFindReplaceDialog* m_pFindReplaceDialog;
	CString m_sLastFindString;
	CString m_sLastReplaceString;
	bool m_bLastMatchCase;
	bool m_bLastMatchWholeWord;
	CPoint m_ptLastFindPos;
	long m_lInitialSearchPos;
	UINT m_nPasteType;
	BOOL m_bFirstSearch;
	bool m_bSuspendNotifications;
    long m_totalLenAtPreviousSelChange;

// Implementation
public:
	virtual ~CEditorBar();
	void SetReadOnly(bool=true);
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    afx_msg void OnSelChange(NMHDR*pNotifyStruct, LRESULT*result);
	LRESULT OnFindReplaceMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEditFindReplace(bool findOnly);
	afx_msg void OnEditRepeat();

	bool UpdateLastFindStringFromSelection();
	BOOL SameAsSelected(LPCTSTR lpszCompare, BOOL bCase);
	void AdjustDialogPosition(CDialog* pDlg);
	BOOL FindText(LPCTSTR lpszFind, BOOL bCase = TRUE, BOOL bWord = TRUE, BOOL bNext = TRUE);
	long FindAndSelect(DWORD dwFlags, FINDTEXTEX& ft);
	void TextNotFound(LPCTSTR lpszFind);
    void SelectAll(); 
};
