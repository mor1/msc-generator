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
/** @file MiniEditor.h The internal editor: CEditorBar and CCshRichEditCtrl classes.
* @ingroup Msc_generator_files */

#pragma once
#undef min
#undef max
#include <msc.h>
#include "PopupList.h"

/////////////////////////////////////////////////////////////////////////////
// CMiniEditor window


class CEditorBar;

/** A color syntax highlighting and smart identing editor control
 * that is doing hinting.*/
class CCshRichEditCtrl : public CRichEditCtrl
{
protected:
    DECLARE_MESSAGE_MAP()
    /** Need to redefine this because default behaviour skips linefeeds
     * if they are the last character of the text to paste.*/
    LRESULT OnPasteSpecial(WPARAM wParam, LPARAM lParam);

private:
    bool m_bWasReturnKey;        ///<True if the return key was pressed. Needed to see if we shall show the line begin hints or not.
    bool m_bUserRequested;       ///<True if the the current hints session was invoked via Ctrl+Space
    bool m_bTillCursorOnly;      ///<True if the the current hints session started at the beginning of a word.
    bool m_bWasAutoComplete;     ///<True if last change was an auto-completion. Set to prevent entering hint mode again after.
    CPopupList m_hintsPopup;     ///<A List box that pops up for hint sessions.
    CEditorBar * const m_parent; ///<Our parent, an editor bar.
    CHARRANGE m_crSel_before;    ///<The selection before a change
    CString m_prev_text;         ///<The previous text, for which m_csh is relevant. Used in UpdateCSH().
    bool m_bRedrawState;         ///<Stores what did we last set SetRedraw(), since we have no GetRedraw()
    /** A type to store redraw and doc notification status.
     * `first` contains if redraw is enabled, `second` contains if doc updates are enabled.*/
    typedef std::pair<bool, bool> WindowUpdateStatus; 
public:
    /** Modes of updating CSH */
    typedef enum UpdateCSHType {
        HINTS_AND_LABELS = 0, ///<Do not update colory syntax highlighting, just parse text for hints and the location of labels (for smart identing)
        CSH = 1,              ///<Update color sytnax highlighting (and collect hints and label positions, as well)
        FORCE_CSH = 2         ///<Force an update of color sytnax highlighting (and collect hints and label positions, as well)
    };
    Csh  m_csh;               ///<The result of the last parse (csh entries, hints and label positions
    int m_tabsize;            ///<What is the tabulator size (read from registry, but not yet written)
    /**Init members*/
	CCshRichEditCtrl(CEditorBar *parent);
    /**Create Windows object and show.*/
    virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

    /** @name Generic helpers
    * @{ */
	void GetSelLineCol(int &lStartLine, int &lStartCol, int &lEndLine, int &lEndCol) const;
	int  GetLineString(int line, CString &str);
    /**Convert a line,col position to a RichEditCtrl position.*/
	long ConvertLineColToPos(unsigned line, unsigned col) const {return LineIndex(line) + col;}
    /**Convert a RichEditCtrl position to a line,col position.*/
	void ConvertPosToLineCol(long pos, int &line, int &col) const {line=LineFromChar(pos); col=pos-LineIndex(line);}
    /** Move the cart to a position */
	void JumpToLine(int line, int col);
    //@}
    /** @name Functions for intelligent identation
    * @{ */
	int  FirstNonWhitespaceIdent(const char *str, int Max=-1);
	char LastNonWhitespaceChar(const char *str, long pos) const;
	int  FindColonLabelIdent(long lStart, int *line=NULL);
    int  FindProperLineIdent(int lStart);
    int  FindCurrentLineIdent(long lStart);
	int  FindIdentForClosingBrace(int pos_to_be_inserted);
	int  CalcTabStop(int col, bool forward, int smartIdent=-1, int prevIdent=-1, bool strict=false);
    bool SetCurrentIdentTo(int target_ident, int current_ident, int col, long lStart, long lEnd, bool standalone);
	BOOL PreTranslateMessage(MSG* pMsg);
    WindowUpdateStatus GetWindowUpdate();
    WindowUpdateStatus DisableWindowUpdate();
    void RestoreWindowUpdate(const WindowUpdateStatus &s);
    //@}
    /** @name Color Syntax Highlighting functions
    * @{ */
	void UpdateText(const char *text, CHARRANGE &cr);
    void UpdateText(const char *text, int lStartLine, int lStartCol, int lEndLine, int lEndCol);
    bool UpdateCSH(UpdateCSHType updateCSH);
	void CancelPartialMatch();
    //@}
    /** @name Mouse Wheel handling
    * @{ */
    /**Called by the framework if a mouse wheel event is captured by us 
      * due to we having the focus. We call pDoc->DispatchMouseWheel(),
      * which decides if the view or us shall handle the event.*/
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt); 
    /** Change zoom. Called if the mouse wheel was rolled inside our area.*/
    BOOL DoMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    //@}
    /** @name Hint window related
      * @{ */
    /** Shows popup hint window and enters hint mode.
     * Assumes we have called UpdateCSH() just before and m_csh is up-to-date.
     * Also used to update hints in popup, if already in hint mode.
     * @param [in] setUptoCursor If true, we will consider the hinted word
     *                           to last only up to the cursor position, even
     *                           if there are alphanum chars after.
     *                           (Set when hint mode was triggered at the beginning of a word.)*/
    void StartHintMode(bool setUptoCursor); 
    /**Returns true if we are in hint mode*/
    bool InHintMode() const {return m_hintsPopup.m_shown;}
    /**Hides the hint popup and leaves hint mode. Text is not changed.*/
    void CancelHintMode();
    /** Unset the user selected nature of the hint session. Called if after an update of
     * the text the cursor has ended up in a different word.*/
    void CancelUserSelected() {m_bUserRequested = false;}
    /**Replace the hinted string of the text with the substitute.
     * Called when we select a hint from the list. If endHintMode is true, we also 
     * leave hint mode. We remain in hint mode if the user hit a '.' dot or if
     * the originator of the hint indicated so (like a grouped hint).*/
    void ReplaceHintedString(const char *substitute, bool endHintMode);
    //@}
};

class CMainFrame;

/** A custom dockable pane hosting a CCshRichEditCtrl.*/
class CEditorBar : public CDockablePane
{

public:
	CEditorBar(); ///< Construction

    CFont m_Font;                             ///<Font to use
	CCshRichEditCtrl m_ctrlEditor;            ///<The edit control
	CFindReplaceDialog* m_pFindReplaceDialog; ///<The search/replace dialog
	CString m_sLastFindString;                ///<The last string to search for
	CString m_sLastReplaceString;             ///<The last string to replace to
	bool m_bLastMatchCase;                    ///<If we have 'match case' on the last time we searched
    bool m_bLastMatchWholeWord;               ///<If we have 'whole word' on the last time we searched
	CPoint m_ptLastFindPos;                   ///<Last screen position of the text found.
	long m_lInitialSearchPos;                 ///<Text pos where we started searching from
	BOOL m_bFirstSearch;                      ///<True if this is the first search in a search/replace session
	bool m_bSuspendNotifications;             ///<True if we shall not notify the document about a change in selection (e.g., during coloring)
    long m_totalLenAtPreviousSelChange;       ///<The length of the text at the last selection change (We use this to test if we have just changed the selection or the text, as well.)
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
