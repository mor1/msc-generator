// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

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

class CCshRichEditCtrl : public CRichEditCtrl
{
	bool m_bCshUpdateInProgress; //if the process of setting colors for syntax is in progress
	Csh  m_csh;
	Csh  m_designlib_csh;
    bool m_bWasReturnKey;        //if the return key was pressed
    bool m_bUserRequested;       //the incarnation of the hints session was due to Ctrl+Space
    bool m_bTillCursorOnly;      //the incarnation of this hints session started at the beginning of a word
    bool m_bWasAutoComplete;     //Set to prevent entering hint mode after an auto-completion
    CPopupList m_hintsPopup;
public:
	int m_tabsize;
	CCshRichEditCtrl(CWnd *parent);
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
	int  FindPreviousLineIdent(long lStart, long *lEnd=NULL);
	int  FindCurrentLineIdent(long lStart);
	int  FindIdentForClosingBrace(int pos_to_be_inserted);
	int  CalcTabStop(int col, bool forward, int smartIdent=-1, int prevIdent=0, bool strict=false);
	void SetCurrentIdentTo(int ident);
	BOOL PreTranslateMessage(MSG* pMsg);

	//Color Syntax Highlighting functions
    void SetForcedDesign(const CString &fd) {m_designlib_csh.ForcedDesign = fd;}
	void UpdateText(const char *text, CHARRANGE &cr, bool preventNotification);
	void UpdateText(const char *text, int lStartLine, int lStartCol, int lEndLine, int lEndCol, bool preventNotification);
	bool UpdateCsh(bool force = false); //retuns true if the past and new m_csh.hintedStringPos overlap
	void CancelPartialMatch();
	bool IsCshUpdateInProgress() {return m_bCshUpdateInProgress;}
	bool NotifyDocumentOfChange(bool onlySelChange=false);

	//Mouse Wheel handling
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt); 
	        BOOL DoMouseWheel(UINT nFlags, short zDelta, CPoint pt); 

    //Hint window related
    void StartHintMode(bool setUptoCursor); //also used to update hints in popup, if already in hint mode
    bool InHintMode() const {return m_hintsPopup.m_shown;}
    void CancelHintMode();
    void CancelUserSelected() {m_bUserRequested = false;}
    void ReplaceHintedString(const char *substitute, bool endHintMode);

	DECLARE_MESSAGE_MAP()
};

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
    long m_totalLenAtPreviousTextChange;

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
