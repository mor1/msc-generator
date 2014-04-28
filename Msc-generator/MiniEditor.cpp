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
#include "stdafx.h"
#include "Msc-generator.h"
#include "MscGenDoc.h"
#include "MiniEditor.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CCshRichEditCtrl, CRichEditCtrl)
	ON_WM_MOUSEWHEEL() 
    ON_MESSAGE(EM_PASTESPECIAL, OnPasteSpecial)
END_MESSAGE_MAP()


CMscGenDoc *GetMscGenDocument() 
{
    CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
    ASSERT(pApp != NULL);
    if (!pApp) return NULL;
    CFrameWnd *pMainWnd = dynamic_cast<CFrameWnd*>(pApp->GetMainWnd());
    ASSERT(pMainWnd!=NULL);
    if (!pMainWnd || pMainWnd->GetActiveView() == NULL) return NULL;
    CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(pMainWnd->GetActiveView()->GetDocument());
    return pDoc;
}

CCshRichEditCtrl::CCshRichEditCtrl(CEditorBar *parent) : 
    m_csh(ArcBase::defaultDesign, NULL), m_hintsPopup(parent, this),
    m_parent(parent)
{
    m_tabsize = 4;
    m_bWasReturnKey = false;
    m_bUserRequested = false;
    m_bTillCursorOnly = false;
    m_bWasAutoComplete = false;
    m_bRedrawState = true;
}

#define LIMIT_TEXT 64000
#define LIMIT_TEXT_STR "64Kbytes"

/** Create the object. 
 * initialize the tabsize and color scheme.
 * Create our hint popup window, too. */
BOOL CCshRichEditCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	if (pApp) {
        m_tabsize = pApp->GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_TABSIZE, 4);
        m_csh.use_scheme = &pApp->m_nCshScheme;
    } 

    if (!CRichEditCtrl::Create(dwStyle, rect, pParentWnd, nID))
        return false;
    if (!m_hintsPopup.Create(IDD_POPUPLIST, this))
        return false;
    LimitText(LIMIT_TEXT);
    return true;
}

/** Return the position of the selection in line and column 
* @param [out] lStartLine The line number of the start of the selection
* @param [out] lStartCol The line number of the start of the selection
* @param [out] lEndLine The line number of the end of the selection
* @param [out] lEndCol The line number of the end of the selection
*/
void CCshRichEditCtrl::GetSelLineCol(int &lStartLine, int &lStartCol, int &lEndLine, int &lEndCol) const
{
	CHARRANGE cr;
	GetSel(cr);
	ConvertPosToLineCol(cr.cpMin, lStartLine, lStartCol);
	ConvertPosToLineCol(cr.cpMax, lEndLine, lEndCol);
}

/** Return the content of a line
 * @param [in] line Which line we want
 * @param [out] str The returned content of the line.
 * @returns the length of the line.*/
int CCshRichEditCtrl::GetLineString(int line, CString &str)
{
	int nLineLength = LineLength(LineIndex(line)) + sizeof(int) + 1;
	nLineLength = GetLine(line, str.GetBufferSetLength(nLineLength), nLineLength);
	str.Truncate(nLineLength); 
	if (str[nLineLength-1] == '\x0d')
		str.Truncate(--nLineLength); //kill terminating lineend
	return nLineLength;
}


/** Return the offset of the first non-whitespace
 * @param [in] str The string in which to search
 * @param [in] Max The maximum value to return. If negative, no limit
 * @returns -1 if all the line is whitespace (or the first Max chars)
 *          else the offset of the first non-whitespace character*/
int CCshRichEditCtrl::FirstNonWhitespaceIdent(const char *str, int Max) 
{
	int i = 0;
    if (Max <= -1) {
        while (str[i]==' ' || str[i]=='\t') i++;
        return str[i] ? i : -1;
    }
    while (i<=Max && (str[i]==' ' || str[i]=='\t')) i++;
	if (!str[i] || i>Max) return -1;
	return i;
}

/** Return the offset of the last non-whitespace
* @param [in] str The string in which to search
* @param [in] Max The maximum value to return.
* @returns -1 if all the line is whitespace (or the first Max chars)
*          else the offset of the last non-whitespace character within Max*/
int CCshRichEditCtrl::LastNonWhitespaceIdent(const char *str, int Max)
{
	if (Max == -1) 
		Max = strlen(str)-1;
	while (Max>=0 && (str[Max]==' ' || str[Max]=='\t')) Max--;
	return Max;
}

/** Return the ident of a colon-label's applicable ident.
 * The returned value is the ident of the first non-whitespace after the colon
 * (can be in a different line than the colon). If the cursor is in-between
 * the colon and the first non-whitespace, we return the ident of the colon+2.
 * @param [in] lStart The position of the cursor (assumedly inside a colon-label)
 * @param [out] line The number of the line we found the colon in.
 * @returns Return -1 if we are 1) not in a label or 2) smart ident is off 
 *          or 3) no csh or 4) the label is not preceeeded by a colon.
 *          Else return the column of the first non whitespace character 
 *          after the colon*/
int CCshRichEditCtrl::FindColonLabelIdent(long lStart, int *line) 
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    if (!pApp || !pApp->m_bSmartIdent) return -1;
	//Go through the list of colon labels
    const CshPos *pos = m_csh.IsInColonLabel(lStart);
    if (!pos)
        return -1;     //We were not inside a colon-label

    int dummy;
    //Note: pos.first_pos indexes the text file from 1
    //whereas lStart indexes the file from 0.
    //hence we substract one from first_pos
    if (line) ConvertPosToLineCol(pos->first_pos-1, *line, dummy); //the line of the colon
        
    int offset = m_tabsize;
    int nLine, nCol;
    ConvertPosToLineCol(lStart, nLine, nCol);
    CString strLine;
    int nLineLength = GetLineString(nLine, strLine);
    int ident1 = FirstNonWhitespaceIdent(strLine);
    //if the line to ident begins with a { or [, use the line of the colon
    //to align to.
    if (ident1>=0 && (strLine[ident1] == '{' || strLine[ident1] == '[')) {
        if (strLine[ident1] == '{')
            offset = 0;
        //store the line of the colon strLine and its pos in nLine, nCol
        ConvertPosToLineCol(pos->first_pos-1, nLine, nCol);
        nLineLength = GetLineString(nLine, strLine);
    } else {
        //OK, we are really identing a line of the label text
        //First see if the colon is the last char in its line
        //store the line of the colon strLine and its pos in nLine, nCol
        ConvertPosToLineCol(pos->first_pos-1, nLine, nCol);
        nLineLength = GetLineString(nLine, strLine);
        int i = nLineLength-1;
        while (i>nCol && (strLine[i]==' ' || strLine[i]=='\t'))
            i--;
        //now strLine[i] is the last non-whitespace in the line of the colon
        if (i > nCol) {
            //The colon is not the last char in the line.
            //Search the line after the colon to the first non whitespace:
            //that will be the first char of the label - use that as ident
            nCol++; //step over the colon
            while (nCol<nLineLength && (strLine[nCol]==' ' || strLine[nCol]=='\t')) 
                nCol++;
            //if there is such a non-space in the label on the same line as the colon
            if (nCol<nLineLength)
                return nCol;
            else {
                _ASSERT(0);
                return -1; //should not happen, we must have chars after the colon here
            }
        }
        //OK, here the colon is the last char in its line,
        //so the ident to return is the ident of the colon's line + m_tabsize
        //Now offset==m_tabsize and strLine holds the line of the colon.
    } 
    //All other cases use the ident of strLine plus 'offset'
    const int ident = FirstNonWhitespaceIdent(strLine);
    //..and ident one compared to that
    if (ident>=0)
        return ident + offset;
    _ASSERT(0); //should not happen, the line of the colon must not be empty
    return -1;
}

/** Find what is the proper identation for this line 
 * Consider if we are in a label, and consider '{' and '}' chars.
 * @param [in] line the line we talk about
 * @returns the number of spaces proper at the head of this line */
int  CCshRichEditCtrl::FindProperLineIdent(int line)
{
    //Check if the head of the line is in a label or not
    CString strLine;
    /*int nLineLength =*/ GetLineString(line, strLine);
    const int current_ident = FirstNonWhitespaceIdent(strLine);
    int colon_line;
    const long lStartLine = ConvertLineColToPos(line, 0);
    const int label_ident = FindColonLabelIdent(lStartLine, &colon_line);
    if (label_ident>=0)
        return label_ident;
    
    //if not, check if the line starts with a '}'
    if (current_ident>=0 && strLine[current_ident] == '}') {
        const int ident = FindIdentForClosingBrace(lStartLine);
        return ident == -1 ? 0 : ident;
    }
    
    //Ok, we are not in a label, neither start with '}'
    //Find the ident of the previous line, but ignore if part of a label
    for (--line; line>=0; line--) {
        //skip empty lines
        CString strLine;
        GetLineString(line, strLine);
        int ident = FirstNonWhitespaceIdent(strLine);
        if (ident == -1) continue;

        //OK, we have found the line above the start pos that has a non-whitespace
        //if the beginning of that line is inside a label, we have to find the beginning of the label
        //because we must ident to that line
        if (strLine[ident] != '{' && FindColonLabelIdent(ConvertLineColToPos(line, ident)) >= 0)
            continue;

        //If no label is above us, check if the line ends in an '{'
        //if yes, add extra identation
        int last_col = LastNonWhitespaceIdent(strLine);
        if (last_col>=0 && strLine[last_col] == '{')
            ident += m_tabsize;
        return ident;
    }
    //reached the beginning of the file, no whitespace above us
    return 0;
}


/** Return the column of the first non space in the current line
 * @param [in] lStart the position of the cursor of which line we are interested in
 * @return -1 if current line has only spaces or is empty*/
int CCshRichEditCtrl::FindCurrentLineIdent(long lStart) 
{
	int line = LineFromChar(lStart);
	CString strLine;
	GetLineString(line, strLine);
	return FirstNonWhitespaceIdent(strLine);
}


/** Finds the first unmatched opening brace before the pos 
 * (used when inserting a closing brace)
 * @returns the ident of the first character in the line of the '{'
 *      or -1 if no opening brace before us or we are in a label and we know it.*/
int CCshRichEditCtrl::FindIdentForClosingBrace(int pos_to_be_inserted)
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	//if color syntax highlighting is not enabled we cannot check for label
    const bool haveSmartIdent = pApp && pApp->m_bSmartIdent;
    
    //Do not do anything in a label
    if (haveSmartIdent)
        if (m_csh.IsInColonLabel(pos_to_be_inserted) || 
            m_csh.IsInQuotedString(pos_to_be_inserted))
            return -1;
	int nLine, nCol;
	ConvertPosToLineCol(pos_to_be_inserted, nLine, nCol);
	CString strLine;

	//Find the matching brace before us
	int num_braces = 0;
	for (int line = nLine; line>=0; line--) {
		int nLineLength = GetLineString(line, strLine);
		if (line == nLine) 
			nLineLength = nCol;  //on the current line, start searching from insertion point
		//search for opening brace from the back
		while(--nLineLength>=0) {
			if (strLine[nLineLength] != '{' && strLine[nLineLength] != '}') continue;
			//if color syntax highlighting is not enabled we cannot check for label
			if (haveSmartIdent) {
				//if a brace, but in a label, ignore
                const long lPos = ConvertLineColToPos(nLine, nLineLength)+1;
                if (m_csh.IsInColonLabel(lPos) ||
                    m_csh.IsInQuotedString(lPos))
                    continue;
            }
			if (strLine[nLineLength] == '}') num_braces++;
			else if (--num_braces<0) //OK our matching opening brace found
				return FirstNonWhitespaceIdent(strLine, nLineLength);
		}
	}
	//We found no matching '{'
	return -1;
}

/** Calculates the tab stop from the current position 
 * @param [in] col The current column of the cursor (must be nonnegative)
 * @param [in] forward true if we look for a tabstop to the right, false if left
 * @param [in] smartIdent A pre-calculated smart ident position if >=0.
 *                        In this we jump to that (if in the right direction), 
 *                        else we align to prevident (so end result differs 
 *                        from prev_ident in 'm_tabsize' (Man, what does this mean?!?)
 * @param [in] prevIdent The previously existing identation of the line 
 *                       (must be nonnegative)
 * @param [in] strict If true and the actual value of prevIdent lies in the right 
 *                    direction, we jump there
 * @returns the calculated identation */
int CCshRichEditCtrl::CalcTabStop(int col, bool forward, int smartIdent, int prevIdent, bool strict)
{
	if (smartIdent>=0) {
		if ( forward && col<smartIdent) return smartIdent;
		if (!forward && col>smartIdent) return smartIdent;
	}
	if (strict) {
		if ( forward && col<prevIdent) return prevIdent;
		if (!forward && col>prevIdent) return prevIdent;
	}
	if (forward) 
		return col + ((prevIdent%m_tabsize - col%m_tabsize + m_tabsize - 1)%m_tabsize + 1);
	col -= (col%m_tabsize - prevIdent%m_tabsize + m_tabsize - 1)%m_tabsize + 1;
	return col<0 ? 0 : col;
}

/** Sets the identation of current line by adding or removing leading whitespace
 * Adds or removes spaces so that caret moves to right ident (from lStart) 
 * This assumes there is enough space in front of us if we need to remove 
 * to get to the right ident.
 * We also assume
 * @param target_ident The identation we want 
 * @param current_ident The current identation of the line
 * @param col The current cursor position in the line (lStart)
 * @param lStart The start of the selection or current cursor position
 * @param lEnd The end of the selection or current cursor position
 * @param [in] standalone If true, we disable drawing and adjust selection afterwards.
 *                        If false we assume this is called for consecutive lines
 *                        so we do not disable redraw and leave selection as garbage.
 *                        Also if we are not standalone, we update the CSH so that 
 *                        subsequent lines work well.
 *                        Also, if we are standalone, we update last_change.
 * @returns true if the text has changed */
bool CCshRichEditCtrl::SetCurrentIdentTo(int target_ident, int current_ident, int col,
                                         long lStart, long lEnd, bool standalone)
{
    if (current_ident == -1) 
        current_ident = 0; //empty line
    if (target_ident == current_ident) return false; //nothing to do
    WindowUpdateStatus state;
    if (standalone)
        state = DisableWindowUpdate();
    int line2, col2;
    ConvertPosToLineCol(lEnd, line2, col2);
    if (target_ident > current_ident) {
        SetSel(lStart-col, lStart-col);
        ReplaceSel(CString(' ', target_ident-current_ident));
    } else {
        SetSel(lStart-col, lStart-col+current_ident-target_ident);
        ReplaceSel("");
    }
    if (standalone) {
        const long lBegin = lStart - col;
        if (col>=current_ident)
            col += target_ident - current_ident; //move with idented text
        else
            col = target_ident;
        if (col2 >= current_ident)
            col2 += target_ident - current_ident; //move with idented text
        else
            col2 = target_ident;
        SetSel(lBegin+col, lBegin+col2);
        RestoreWindowUpdate(state);
    } else {
        Csh::AdjustCSHList(m_csh.CshList,    lStart-col+1, target_ident-current_ident);
        Csh::AdjustCSHList(m_csh.ColonLabels,lStart-col+1, target_ident-current_ident);
        Csh::AdjustCSHList(m_csh.CshErrors,  lStart-col+1, target_ident-current_ident);
    }
    return true;
}

LRESULT CCshRichEditCtrl::OnPasteSpecial(WPARAM wParam, LPARAM lParam)
{
    //if we do not paste CF_TEXT, we use regular paste
    if (wParam!=CF_TEXT) {
        CRichEditCtrl::Paste();
        return 0;
    }

    // Test to see if we can open the clipboard first.
    if (OpenClipboard()) {
        // Retrieve the Clipboard data (specifying that 
        // we want ANSI text (via the CF_TEXT value).
        HANDLE hClipboardData = GetClipboardData(CF_TEXT);

        // Call GlobalLock so that to retrieve a pointer
        // to the data associated with the handle returned
        // from GetClipboardData.
        char *pchData = (char*)GlobalLock(hClipboardData);

        // Set a local CString variable to the data
        // and then update the dialog with the Clipboard data
        CString strFromClipboard = pchData;
        const auto state = GetWindowUpdate();
        SetRedraw(false);
        ReplaceSel(pchData);
        RestoreWindowUpdate(state);

        // Unlock the global memory.
        GlobalUnlock(hClipboardData);

        // Finally, when finished I simply close the Clipboard
        // which has the effect of unlocking it so that other
        // applications can examine or modify its contents.
        CloseClipboard();
    }
    return 0;
}


//This is to handle TAB, SHIFT+TAB, RETURN, BACKSPACE, and "}" chars
//We assume no TAB characters in the file, we replace everything to spaces
/* TAB rules
**   SINGLE line: 
**      In leading whitespace: Align to next/prev tab position
**           (which is: if in a label: the first char of the label
**            
**            else: <int>*m_tabsize + ident of the first char on previous line)
**      After leading whitespace: align to next <int>*m_tabsize
**                                Shift+Tab: align to previous, if enough whitespace
**      Or if TAB mode is on, we simply ident the line
**   Multiple lines: align each line to next/prev tab position
**           (which is: if line is in label, the first char of the label)
**            else <int>*m_tabsize + ident of the first char on the line before the selected block)
** ENTER rule
**   if in label, align to label first char
**   else if if the char just after the insertion point is a }, search matching {
**   else align to previous first indent (potentially same line)
**      except: add + m_tabsize if char just before enter is a {
** BACKSPACE rule
**   if in leading whitespace, do as shift+tab
**   else: do as normally
** "}" rule
**   if in leading whitespace, find matching { and force ident to that
**   else do as normal
*/
BOOL CCshRichEditCtrl::PreTranslateMessage(MSG* pMsg)
{
    m_bWasReturnKey = false;
    if (pMsg->message != WM_KEYDOWN && pMsg->message != WM_CHAR)
        return CRichEditCtrl::PreTranslateMessage(pMsg);
    static std::list<std::pair<UINT, WPARAM>> msg;
    msg.emplace_back(pMsg->message, pMsg->wParam);
    GetSel(m_crSel_before);
    long lStart = m_crSel_before.cpMin;
    long lEnd = m_crSel_before.cpMax;
    if (InHintMode()) {
        if (pMsg->message == WM_KEYDOWN) { 
            if (pMsg->wParam == VK_ESCAPE) {CancelHintMode(); return TRUE;}
            if (pMsg->wParam == VK_UP)     {m_hintsPopup.m_listBox.UpDownKey(-1); return TRUE;}
            if (pMsg->wParam == VK_DOWN)   {m_hintsPopup.m_listBox.UpDownKey(+1); return TRUE;}
            if (pMsg->wParam == VK_PRIOR)  {m_hintsPopup.m_listBox.UpDownKey(-2); return TRUE;}
            if (pMsg->wParam == VK_NEXT)   {m_hintsPopup.m_listBox.UpDownKey(+2); return TRUE;}
            if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_TAB) {
                const CshHint *item = m_hintsPopup.m_listBox.GetSelectedHint();
                if (item && item->state == HINT_ITEM_SELECTED) {
                    ReplaceHintedString(item->plain.c_str(), !item->keep);
                    return TRUE;
                }
                //if nothing selected let it run further as if not in hint mode
            }
        } else if (pMsg->message == WM_CHAR) {
            if (isalnum(pMsg->wParam) || pMsg->wParam == '_' || pMsg->wParam<=31 || pMsg->wParam>=127)
                //characters that can make up a hinted keyword -
                //or are control characters
                //we insert them and re-calc hints afterwards in OnCommand
                return CRichEditCtrl::PreTranslateMessage(pMsg);
            //do nothing if no item is selected
            const CshHint *item = m_hintsPopup.m_listBox.GetSelectedHint();
            if (!item) return CRichEditCtrl::PreTranslateMessage(pMsg);
            //Expand hint under cursor, if it is full match or prefix
            if (item->state != HINT_ITEM_SELECTED) return CRichEditCtrl::PreTranslateMessage(pMsg);
            if (pMsg->wParam == '.') {
                //expand only till the next dot in the hint
                unsigned pos = m_csh.hintedStringPos.last_pos - m_csh.hintedStringPos.first_pos;
                pos = item->plain.find_first_of('.', pos);
                if (pos != string::npos) {
                    ReplaceHintedString(item->plain.substr(0, pos).c_str(), false);
                    return CRichEditCtrl::PreTranslateMessage(pMsg);
                }
            }
            ReplaceHintedString(item->plain.c_str(), !item->keep);
            return CRichEditCtrl::PreTranslateMessage(pMsg);
        }
    }
    //Handle a batch of WM_KEYDOWN events
    if (pMsg->message == WM_KEYDOWN) {
        //show hints for ctrl+space
        if (pMsg->wParam == VK_SPACE && GetKeyState(VK_CONTROL) & 0x8000) {
            m_bUserRequested = true;
            //just update CSH records for hints
            UpdateCSH(HINTS_AND_LABELS);
            //see if we are at the beginning of a word
            bool till_cursor_only = false;
            if (lStart==0) till_cursor_only = true;
            else {
                CString str;
                GetTextRange(lStart-1, lStart, str);
                if (str[0]!='_' && !isalnum(str[0])) till_cursor_only = true;
            }
            //if so, set the hint mode such that we only consider the word under the curson
            //up to the cursor (and cut away the remainder)
            m_bWasAutoComplete = false; //do not prevent hint mode on this basis
            StartHintMode(till_cursor_only);
            return TRUE;
        }

        //Do nothing for escape
        if (pMsg->wParam == VK_ESCAPE) {
            CFrameWnd *pMainWnd = dynamic_cast<CFrameWnd*>(AfxGetMainWnd());
            if (pMainWnd && pMainWnd->GetActiveView()) {
                CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(pMainWnd->GetActiveView()->GetDocument());
                //kill tracking mode or any highlight
                if (pDoc->m_bTrackMode)
                    pDoc->SetTrackMode(false);
                else
                    pDoc->StartFadingAll();
            }
            return TRUE;
        }
        //call parent if not keys relevant for smart identing
        if (pMsg->wParam != VK_TAB && pMsg->wParam != VK_RETURN && pMsg->wParam != VK_BACK)
            return CRichEditCtrl::PreTranslateMessage(pMsg);
    }

    //Do nothing for characters other than braces and opening square bracket
    if (pMsg->message == WM_CHAR && pMsg->wParam != '}' && 
        pMsg->wParam != '{' && pMsg->wParam != '[')
        return CRichEditCtrl::PreTranslateMessage(pMsg);

    //At this point we have either WM_CHAR and a brace/bracket
    //or WM_KEYDOWN and TAB, ENTER OR BKSPACE

    CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
    ASSERT(pApp != NULL);

	int line, col;
	ConvertPosToLineCol(lStart, line, col);
	const int current_ident = FindCurrentLineIdent(lStart);

    if (pMsg->wParam == '}') {
        if (!pApp->m_bSmartIdent) return CRichEditCtrl::PreTranslateMessage(pMsg);
        //if we are in a label, do nothing
        if (FindColonLabelIdent(lStart)>-1) return CRichEditCtrl::PreTranslateMessage(pMsg);
        //if we are not at the beginning of a line do nothing
        if (current_ident!=-1 && current_ident<col) return CRichEditCtrl::PreTranslateMessage(pMsg); //not in leading whitespace
        //else insert and auto-ident
        const auto state = DisableWindowUpdate();
        ReplaceSel("}", FALSE);
        GetSel(lStart, lEnd);
        const int ident = FindProperLineIdent(line);
        //If the determined ident is zero, just let the system handle the RETURN
        const long line_begin = ConvertLineColToPos(line, 0);
        const int new_current_ident = FindCurrentLineIdent(lStart);
        if (new_current_ident>ident) {
            SetSel(line_begin, line_begin + new_current_ident - ident);
            ReplaceSel("", FALSE);
        } else if (new_current_ident<ident) {
            //insert that many spaces as ident at the beginning of the line
            SetSel(line_begin, line_begin);
            CString spaces(' ', ident-new_current_ident);
            ReplaceSel(spaces, FALSE);
        }
        SetSel(lStart + ident - new_current_ident, lEnd + ident - new_current_ident);
        UpdateCSH(CSH);
        RestoreWindowUpdate(state);
        return TRUE;
	}
    WindowUpdateStatus state;
    if (pMsg->wParam == '{' || pMsg->wParam == '[') {
        if (!pApp->m_bSmartIdent) return CRichEditCtrl::PreTranslateMessage(pMsg);
        //if we are not at the beginning of a line do nothing
        if (current_ident!=-1 && current_ident<col) return CRichEditCtrl::PreTranslateMessage(pMsg); //not in leading whitespace
        state = DisableWindowUpdate();
        ReplaceSel(pMsg->wParam=='{' ? "{" : "[", FALSE);
    apply_ident:
        UpdateCSH(HINTS_AND_LABELS);
        GetSel(lStart, lEnd);
        ConvertPosToLineCol(lStart, line, col);
        int ident = FindProperLineIdent(line);
        const int current_ident2 = FindCurrentLineIdent(lStart);
        const bool changed = SetCurrentIdentTo(ident, current_ident2, col, lStart, lEnd, true);
        //if we have inserted a char, set cursor after it
        if (pMsg->wParam != VK_RETURN)
            ident++;
        lStart = ConvertLineColToPos(line, ident);
        SetSel(lStart, lStart);
        UpdateCSH(CSH);
        RestoreWindowUpdate(state);
        return TRUE;
    }

	if (pMsg->wParam == VK_RETURN) {
        m_bWasReturnKey=true;
        if (!pApp->m_bSmartIdent) return CRichEditCtrl::PreTranslateMessage(pMsg);
        state = DisableWindowUpdate();
        ReplaceSel("\n", FALSE);
        goto apply_ident;
	}
    if (pMsg->wParam == VK_BACK) {
        //in case of ctrl+backspace we do not do
        //any smart ident. (Why not??)
        if (bool(GetKeyState(VK_CONTROL) & 0x8000)) 
            return CRichEditCtrl::PreTranslateMessage(pMsg);
		//if not in leading whitespace or at beginning of line
        if ((col==0) || (current_ident!=-1 && current_ident<col)) 
            return CRichEditCtrl::PreTranslateMessage(pMsg);
		//in leading whitespace, consider smart ident and/or previous line indent
        if (!pApp->m_bSmartIdent) return CRichEditCtrl::PreTranslateMessage(pMsg);
        const int smartIdent = FindProperLineIdent(line);
		int ident = CalcTabStop(col, false, smartIdent, current_ident, true);
        SetCurrentIdentTo(ident, current_ident, col, lStart, lEnd, true);
        return TRUE;
	}
    //Here are the rules for TAB
    //1. if TAB mode is on, we ident all lines in selection correctly.
    //2. if TAB mode is off
    //    - if the selection is a cursor
    //        * if we are in the leading whitespace 
    //             we ident the line correctly and jump to the beginning of the line
    //        * else we insert as many spaces to fall to int multiple of m_tabsize
    //    - if the selection is a range, we delete it and fall to int multiple of m_tabsize
    if (pMsg->wParam == VK_TAB) {
        const bool shift = bool(GetKeyState(VK_SHIFT) & 0x8000);
        if (pApp->m_bTABIdents && pApp->m_bSmartIdent) { //TAB mode
            int eLine, eCol;
            ConvertPosToLineCol(lEnd, eLine, eCol);
            const auto state = DisableWindowUpdate();
            bool changed = false;
            if (line == eLine) {
                const int ident = FindProperLineIdent(line);
                changed = SetCurrentIdentTo(ident, current_ident, col, lStart, lEnd, true);
            } else { 
                //We need to save CSH info, since it will be modified by SetCurrentIdent(..., false)
                //and will become out-of-sync with m_prev_text.
                //No need to save m_csh.ColonLabels - that is not synced to m_prev_text
                CshListType saved_csh_list;
                CshErrorList saved_csh_error_list;
                saved_csh_list = m_csh.CshList;
                saved_csh_error_list = m_csh.CshErrors;
                POINT scroll_pos;
                ::SendMessage(m_hWnd, EM_GETSCROLLPOS, 0, (LPARAM)&scroll_pos);

                CString strLine;
                for (int i = line; i<=eLine; i++) {
                    //if the last line and not really selected, skip
                    if (i==eLine && eCol==0) {
                        lEnd = ConvertLineColToPos(eLine, eCol);
                        break;
                    }
                    GetLineString(i, strLine);
                    const int old_ident = FirstNonWhitespaceIdent(strLine);
                    const int new_ident = FindProperLineIdent(i);
                    changed |= SetCurrentIdentTo(new_ident, old_ident, 0, ConvertLineColToPos(i, 0), 0L, false);
                    if (i==line && col>=old_ident)
                        lStart += new_ident-old_ident;
                    else if (i==eLine) {
                        if (eCol>=old_ident)
                            lEnd = ConvertLineColToPos(eLine, eCol+new_ident-old_ident);
                        else
                            lEnd = ConvertLineColToPos(eLine, eCol);
                    }
                }
                SetSel(lStart, lEnd);
                ::SendMessage(m_hWnd, EM_SETSCROLLPOS, 0, (LPARAM)&scroll_pos);
                //restore csh info
                m_csh.CshList = std::move(saved_csh_list);
                m_csh.CshErrors = std::move(saved_csh_error_list);
            }
            if (changed)
                UpdateCSH(CSH);
            RestoreWindowUpdate(state);
        } else { //no smart ident or no tab mode
            //if no selection:
            if (lStart == lEnd) {
                int ident;
                //just insert/remove spaces up to the next/prev tab stop
                //if not in leading whitespace
                if (current_ident!=-1 && current_ident<col) {
                    ident = CalcTabStop(col, true);
                } else {
                    //in leading whitespace, consider smart ident and/or previous line indent
                    const int smartIdent = FindProperLineIdent(line);
                    ident = CalcTabStop(col, !shift, smartIdent, current_ident, true);
                }
                if (ident != col) {
                    const auto state2 = DisableWindowUpdate();
                    if (ident<col) {
                        SetSel(lStart+(ident-col), lStart);
                        ReplaceSel("");
                    } else {
                        ReplaceSel(CString(' ', ident-col));
                    }
                    UpdateCSH(CSH);
                    RestoreWindowUpdate(state2);
                }
            } else {
                //a full selection, do identation
                //Leave last_change as if we deleted the whole selection
                //and inserted something - we do not track where do we change what

                int startLine = line;
                int endLine, endCol;
                ConvertPosToLineCol(lEnd, endLine, endCol);
                //If last line is not really selected, skip it
                if (endCol==0) {
                    endLine--;
                }
                //Calculate the amount of space to add/remove based on the first non-empty line in the selection
                int headLine;
                int headCol = -1;
                for (headLine = startLine; headLine<endLine; headLine++) {
                    headCol = FindCurrentLineIdent(LineIndex(headLine));
                    if (headCol >= 0) break;
                }
                //if all lines empty, nothing to do 
                if (headCol == -1) return TRUE;
                const int smartIdent = FindProperLineIdent(headLine);
                const int offset = CalcTabStop(headCol, !shift, smartIdent, headCol, true) - headCol;

                //Insert/remove "offset" amount of space from the beginning of each line
                const auto state = DisableWindowUpdate();
                POINT scroll_pos;
                ::SendMessage(m_hWnd, EM_GETSCROLLPOS, 0, (LPARAM)&scroll_pos);
                CString spaces(' ', std::max(0, offset)); //empty string if we remove, otherwise as many spaces as we insert
                for (int l = startLine; l<=endLine; l++) {
                    const long lLineBegin = LineIndex(l);
                    const int current_ident2 = FindCurrentLineIdent(lLineBegin);
                    const int adjusted_offset = -std::min(current_ident2, -offset);  //reduce potential remove if not so many spaces at the beginning
                    //empty line - do nothing
                    if (current_ident2==-1) continue;
                    SetSel(lLineBegin, lLineBegin + std::max(0, -adjusted_offset)); //empty selection if we insert, otherwise the spaces to remove
                    ReplaceSel(spaces);

                    //Adjust lStart and lEnd
                    if (l == startLine && adjusted_offset<0) { //can happen only once: at the first line
                        lStart += adjusted_offset;
                        if (lStart < lLineBegin) lStart = lLineBegin;
                    }
                    lEnd += adjusted_offset;
                }
                SetSel(lStart, lEnd);
                ::SendMessage(m_hWnd, EM_SETSCROLLPOS, 0, (LPARAM)&scroll_pos);
                UpdateCSH(CSH);
                RestoreWindowUpdate(state);
            }
        }
        return TRUE;
    }
    return CRichEditCtrl::PreTranslateMessage(pMsg);
}

//We never notify the document object about a change in this function,
//as it is called by the document object only.
void CCshRichEditCtrl::UpdateText(const char *text, int lStartLine, int lStartCol, int lEndLine, int lEndCol)
{
    CHARRANGE cr;
    cr.cpMin = ConvertLineColToPos(lStartLine, lStartCol);
    cr.cpMax = ConvertLineColToPos(lEndLine, lEndCol);
    UpdateText(text, cr);
}

//We never notify the document object about a change in this function,
//as it is called by the document object only.
void CCshRichEditCtrl::UpdateText(const char *text, CHARRANGE &cr)
{
    CString t = text;
    EnsureCRLF(t);

    const auto state = DisableWindowUpdate();
    SetWindowText(t);
    SetSel(cr);
    UpdateCSH(FORCE_CSH);
    RestoreWindowUpdate(state);
    SetFocus();

    if (t.GetLength()>LIMIT_TEXT && m_parent) {
        m_parent->SetReadOnly();
        MessageBox("The internal editor can only handle files up to " LIMIT_TEXT_STR 
                   ". Use an external editor to edit.",
                   "Msc-generator", MB_OK | MB_ICONERROR);
    }
}

template<class EntryList>
CshListType Diff(const EntryList &old_list,
                 const EntryList &new_list, 
                 EColorSyntaxType neutral)
{
    CshListType delta;
    auto o = old_list.begin();
    auto n = new_list.begin();
    while (o!=old_list.end() && n!= new_list.end()) {
        //ignore NULL values in the old list (which can happen due to delete)
        if (o->first_pos > o->last_pos) {
            o++;
            continue;
        }
        if (o->first_pos == n->first_pos) {
            if (o->last_pos != n->last_pos) {
                if (o->last_pos > n->last_pos) {//if old csh is longer, we add its neutral version
                    delta.AddToBack(*o);
                    delta.back().color = neutral;
                }
                delta.AddToBack(*n);
            } else if (o->color != n->color) //same position, but different color: keep new version
                delta.AddToBack(*n);
            o++;
            n++;
        } else if (o->first_pos < n->first_pos) { //a removed item: paint with normal color
            delta.AddToBack(*o);
            delta.back().color = neutral;
            o++;
        } else {//n->first_pos < o->first_pos
            delta.AddToBack(*n);
            n++;
        }
    }
    while (o!=old_list.end()) {
        delta.AddToBack(*o);
        delta.back().color = neutral;
        o++;
    }
    while (n!=new_list.end()) {
        delta.AddToBack(*n);
        n++;
    }
    return delta;
}

/** Updates CSH in the editor window and notifies the document.
@param [in] updateCSH Tells us what to update. 
                      HINTS_AND_COLON_LABELS re-parses, updates hints & colon_labels
                      but does not re-color and leaves m_prev_text, m_csh.CshList and m_csh.CshErrors 
                      intact. Used when Ctrl+Space has been 
                      pressed or during indentation. 
                      CSH updates both hints and recolor the text
                      (only if m_bShowCSH are set).
                      FORCE_CSH forces a full recoloring (discarding delta).
@returns In hint node we return true if we keep hinting the same string. Outside
         hint mode we return true if the user typed (so that we may need to enter
         hint mode).

About position values.
CSH entries start with the first character of the file indexed as 1.
Thus, if a csh entry corresponds to the first character (only) of a file,
it has 'first_pos = 1' and 'last_pos = 1'. For the first three letter 
of the file these values are 1 and 3.
In contrast, RichEditCtrl values start being indexed from zero.
Thus if the caret is before the first character, we get lStart=lEnd = 0.
*/

bool CCshRichEditCtrl::UpdateCSH(UpdateCSHType updateCSH)
{
    bool ret = false;
    CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
    ASSERT(pApp != NULL);
    if (!pApp) return false; //not even notify the doc - it does not exists
    switch (updateCSH) {
    default:
        break;
    case CSH:
        if (pApp->m_bShowCsh || pApp->m_bShowCshErrors ||
            pApp->m_bShowCshErrorsInWindow)
            break;
        //fallthrough
    case HINTS_AND_LABELS:
        //nothing to do if we do not hint or do smart ident
        if (pApp->m_bSmartIdent || pApp->m_bHints)
            break;
        return false;
    }
    CString text;
    GetWindowText(text);
    //Now remove CR ('\r') characters, keep only LF ('\n')
    RemoveCRLF(text);

    //record cursor position 
    CHARRANGE cr;
    GetSel(cr);

    //if we only refresh Hints ColonLabels and QuotedStrings
    if (updateCSH==HINTS_AND_LABELS) {
        //Save old CSH entries
        Csh save = std::move(m_csh);
        //Parse the text
        m_csh = pApp->m_designlib_csh;
        m_csh.use_scheme = &pApp->m_nCshScheme;
        CshPos old_uc = m_csh.hintedStringPos;
        m_csh.ParseText(text, text.GetLength(), cr.cpMax == cr.cpMin ? cr.cpMin : -1, pApp->m_nCshScheme);
        //restore old csh and error list - the ones matching the (now unchanged) m_prev_text
        m_csh.CshList.swap(save.CshList);
        m_csh.CshErrors.swap(save.CshErrors);
        //If we consider the hinted string only up to the cursor, trim the returned pos
        if (m_bTillCursorOnly && m_csh.hintedStringPos.last_pos>cr.cpMin)
            m_csh.hintedStringPos.last_pos = cr.cpMin;

        //return true if the past and new m_csh.hintedStringPos overlap
        return m_csh.hintedStringPos.first_pos <= old_uc.last_pos && old_uc.first_pos<=m_csh.hintedStringPos.last_pos;
        //Here we do not update the doc. HINTS_AND_LABELS are
        //only used, when 1) user pressed Ctrl+Space (no change to text)
        //or 2) when we do smart ident (will call this function again with CSH,
        //when we are done).
    }

    //now updateCSH is either CSH or FORCE_CSH
    /* Compute diff to last text */
    long start, ins, del;
    const size_t l1 = text.GetLength();
    const size_t l2 = m_prev_text.GetLength();
    const size_t lmin = std::min(l1, l2);
    const int ldiff = (int)l2-(int)l1;
    size_t first_diff, last_diff;
    for (first_diff = 0; first_diff<lmin; first_diff++)
        if (text[first_diff]!=m_prev_text[first_diff])
            break;
    if (first_diff == lmin) {
        //one text is prefix of the other
        start = lmin;
        ins = ldiff>=0 ? 0 : -ldiff;
    } else {
        //search for the end of the differences
        for (last_diff = l1; last_diff>std::max(0, -ldiff); last_diff--)
            if (text[last_diff-1]!=m_prev_text[last_diff-1+ldiff])
                break;
        //last_diff is the index of the last different char plus one
        if (last_diff==0) {
            //the new file is a postfix of the old one
            _ASSERT(ldiff>0);
            start = 0;
            ins = first_diff;
        } else if (last_diff<=first_diff) {
            //same text from front and beginning overlap
            //last_diff and first diff actually point to chars that are the same
            start = last_diff - std::max(0, -ldiff);
            ins = first_diff - start + std::max(0, -ldiff);
        } else {
            start = first_diff;
            ins = last_diff - first_diff;
        }
    }
    del = ins + ldiff;
    if (del<0) {
        ins -= del;
        del = 0;
    }
    _ASSERT(ins>=0 && del>=0);
    //string a;
    //a << first_diff << " " <<last_diff << ", ";
    //a << "c " << start << " " << ins << " " << del;
    //MessageBox(a.c_str());
    //Now expand the changed range to include the selection before the action.
    //This is needed for te (unformatted or badly formatted) 
    //text which is equal to the text already there, the above algorithm will not 
    //detect the change and coloring will remain as pasted.
    //Note that start is now in RichEditCtrl space: if we insert at the beginning
    //of the file, start is zero.
    if (m_crSel_before.cpMin < m_crSel_before.cpMax) {
        if (m_crSel_before.cpMin < start) {
            ins += start - m_crSel_before.cpMin;
            del += start - m_crSel_before.cpMin;
            start = m_crSel_before.cpMin;
        }
        if (m_crSel_before.cpMax > start + del) {
            ins += m_crSel_before.cpMax - start - del;
            del = m_crSel_before.cpMax - start;
        }
    }
    m_prev_text = text;

    const auto state = GetWindowUpdate();
    CHARFORMAT *const scheme = pApp->m_csh_cf[pApp->m_nCshScheme];

    if (updateCSH == FORCE_CSH) {
        //if we force a csh update (e.g., due to switching csh schemes)
        //we force a fill update.
        //(however, we have correctly calculated ins,del,start above, since
        //those may be needed by MscGenDoc to decide on grouping undo)
        m_csh.CshList.clear();
        m_csh.CshErrors.clear();
    }
    //if we had csh entries before, adjust them according to ins/del
    if (m_csh.CshList.size()) {
        //Since start is as if the first char in the file is indexed
        //at zero, we add one (csh entries are indexed from 1).
        Csh::AdjustCSHList(m_csh.CshList,   start+1, ins-del);
        Csh::AdjustCSHList(m_csh.CshErrors, start+1, ins-del);
        //We do not adjust ColonLabels here, since that is not needed
        //for calculating coloring diffs, only to smart ident.
        //If we inserted, destroy any marking overlapping with the insertion.
        if (ins) {
            for (auto &csh : m_csh.CshList)
                if (csh.first_pos <= start+1+ins &&
                    start+1 <= csh.last_pos)
                    csh.color = COLOR_NORMAL;
            for (auto &csh : m_csh.CshErrors)
                if (csh.first_pos <= start+1+ins &&
                    start+1 <= csh.last_pos)
                    csh.color = COLOR_MAX;
        }
    }
    //PARSE the text for color syntax
    //Take the last hinted string from m_csh (before overwriting it by m_designlib_csh)
    CshPos old_uc = m_csh.hintedStringPos;
    //move the last list to csh_delta
    CshListType old_csh_list;
    old_csh_list.swap(m_csh.CshList);
    CshErrorList old_csh_error_list;
    old_csh_error_list.swap(m_csh.CshErrors);
    //Take the design, color and style definitions from the designlib
    m_csh = pApp->m_designlib_csh;
    m_csh.use_scheme = &pApp->m_nCshScheme;
    m_csh.ParseText(text, text.GetLength(), cr.cpMax == cr.cpMin ? cr.cpMin : -1, pApp->m_nCshScheme);
    //If we consider the hinted string only up to the cursor, trim the returned pos
    if (m_bTillCursorOnly && m_csh.hintedStringPos.last_pos>cr.cpMin)
        m_csh.hintedStringPos.last_pos = cr.cpMin;

    //retune true if the past and new m_csh.hintedStringPos overlap
    ret = m_csh.hintedStringPos.first_pos <= old_uc.last_pos && old_uc.first_pos<=m_csh.hintedStringPos.last_pos;
    //if we are not in hint mode, we return true if we should enter one
    if (!InHintMode() && ins-del==1)
        ret = true;

    //List the parse errors in the error window
    if ((pApp->m_bShowCshErrorsInWindow || updateCSH == FORCE_CSH) && pApp->m_pWndOutputView) {
        MscError Error;
        Error.AddFile("Hint");
        std::list<CString> errors;
        std::vector<std::pair<int, int>> error_pos;
        //populate list only if we need to show the errors and not judt FORCE_CSH
        if (pApp->m_bShowCshErrorsInWindow) 
            for (const auto &e : m_csh.CshErrors) {
                int line, col;
                ConvertPosToLineCol(e.first_pos, line, col);
                line++; col++; //Needed as errors are indexed from one, positions from zero
                error_pos.push_back(std::pair<int, int>(line, col));
                errors.push_back(Error.FormulateElement(FileLineCol(0, line, col), FileLineCol(0, line, col), true, false, e.text).text.c_str());
            }
        pApp->m_pWndOutputView->ShowCshErrors(errors, error_pos);
        SetFocus(); //back to editor
    }
    //See if we may need to touch the screen
    if (((pApp->m_bShowCsh || pApp->m_bShowCshErrors) && updateCSH >= CSH) || updateCSH == FORCE_CSH) {
        //create a diff 
        //csh_list is unfortunately not always sorted by first_pos, but close.
        std::sort(m_csh.CshList.begin(), m_csh.CshList.end(),
            [](CshEntry const & a, CshEntry const &b) {
            return a.first_pos==b.first_pos ? a.last_pos < b.last_pos : a.first_pos < b.first_pos;
        });
        std::sort(m_csh.CshErrors.begin(), m_csh.CshErrors.end(),
            [](CshError const & a, CshError const &b) {
            return a.first_pos==b.first_pos ? a.last_pos < b.last_pos : a.first_pos < b.first_pos;
        });
        CshListType csh_delta, csh_error_delta;
        if (updateCSH == FORCE_CSH) {
            csh_delta = m_csh.CshList;
            csh_error_delta.reserve(m_csh.CshErrors.size());
            for (const auto &e : m_csh.CshErrors)
                csh_error_delta.push_back(e); //copy only the CshEntry part of e (which is of type CshError)
        } else {
            csh_error_delta = Diff(old_csh_error_list, m_csh.CshErrors, COLOR_MAX);
            //kill any entries overlapping with a no_error - so they are forced to be refreshed
            for (auto &err : csh_error_delta)
                if (err.color == COLOR_MAX)
                    for (auto &csh : old_csh_list)
                        if (csh.first_pos <= err.last_pos && err.first_pos <= csh.last_pos)
                            csh.color = COLOR_MAX;
            csh_delta = Diff(old_csh_list, m_csh.CshList, COLOR_NORMAL);
        }


        //See if we actually need to touch the screen
        //Note: if we have inserted, we need to make that text COLOR_NORMAL even if csh_delta is
        //empty.
        if (ins || csh_delta.size() || csh_error_delta.size() || updateCSH==FORCE_CSH) {

            //Ok now copy the delta to the editor window
            //record scroll position 
            POINT scroll_pos;
            ::SendMessage(m_hWnd, EM_GETSCROLLPOS, 0, (LPARAM)&scroll_pos);

            //freeze screen, prevent visual updates
            DisableWindowUpdate();
            //Erase all formatting on a full update (deltas contain all entries in this case)
            if (updateCSH == FORCE_CSH) {
                SetSel(0, -1); //select all
                SetSelectionCharFormat(scheme[COLOR_NORMAL]); //set formatting to neutral
            } else if (ins) {
                SetSel(start, start+ins); //select inserted text
                SetSelectionCharFormat(scheme[COLOR_NORMAL]); //set formatting to neutral
            }

            //First erase formatting from parts, where errors ceased to exist
            for (auto &e : csh_error_delta)
                if (e.color==COLOR_MAX) {
                    SetSel(e.first_pos-1, e.last_pos);
                    SetSelectionCharFormat(scheme[COLOR_NORMAL]);
                }

            //Now add coloring to parts where it changed 
            if (pApp->m_bShowCsh)
                for (auto &e : csh_delta)
                    if (e.color<COLOR_MAX) {
                        SetSel(e.first_pos-1, e.last_pos);
                        SetSelectionCharFormat(scheme[e.color]);
                    }
            //Now add errors, if we show them
            if (pApp->m_bShowCshErrors)
                for (auto &e : csh_error_delta)
                    if (e.color<COLOR_MAX) {
                        SetSel(e.first_pos-1, e.last_pos);
                        SetSelectionCharFormat(scheme[e.color]);
                    }

            //restore cursor and scroll position
            SetSel(cr);
            ::SendMessage(m_hWnd, EM_SETSCROLLPOS, 0, (LPARAM)&scroll_pos);
        }
    }
    //Ok, notify the document about the change
    if (GetMscGenDocument()) 
        GetMscGenDocument()->OnInternalEditorChange(start, ins, del, m_crSel_before);
    RestoreWindowUpdate(state);
    return ret;
}

void CCshRichEditCtrl::CancelPartialMatch()
{
	CHARRANGE cr;
	GetSel(cr);

    //First check if hint is on and we have left the hinted word
    if (InHintMode() && !m_csh.hintedStringPos.IsWithin(cr.cpMin))
        m_hintsPopup.Hide();

    if (!m_csh.was_partial) return;
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	m_csh.was_partial = false;
    if (!pApp || !pApp->m_bShowCsh) return;

    UpdateCSH(CSH);
}

void CCshRichEditCtrl::JumpToLine(int line, int col) {
	long index = ConvertLineColToPos(line-1, col?col-1:0);
	SetSel(index, index);
	SetFocus();
}


BOOL CCshRichEditCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    if (!pApp) return FALSE;
	CFrameWnd *pMainWnd = dynamic_cast<CFrameWnd*>(pApp->GetMainWnd());
	ASSERT(pMainWnd!=NULL);
	if (pMainWnd && pMainWnd->GetActiveView() != NULL) {
		CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(pMainWnd->GetActiveView()->GetDocument());
		if (pDoc != NULL)
			pDoc->DispatchMouseWheel(nFlags, zDelta, pt);
	}
	return TRUE;
}

BOOL CCshRichEditCtrl::DoMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    CPoint pt2 = pt;
	CRect view;
	GetClientRect(&view);
	ScreenToClient(&pt2);
	//Process message only if within our view
	if (!view.PtInRect(pt2)) return FALSE;
	CWnd::OnMouseWheel(nFlags, zDelta, pt2);
    if (InHintMode())
        CancelHintMode();
	return TRUE;
}

//Assumes we have called UpdateCSH just before and m_csh is up-to-date
//if setUptoCursor is true, we set m_bTillCursorOnly to true
//if false, we leave the value unchanged (=start with a false value)
void CCshRichEditCtrl::StartHintMode(bool setUptoCursor)
{
    long start, end;
    GetSel(start, end);
    //cancel hints if
    //1. multiple characters selected
    //2. no hints collected
    //3. If this StartHintMode is the result of a previous autocompletion (initiated from this function below)
    //4. the cursor stands at the beginning of a real nonzero len hinted word 
    //   and the user did not press Ctrl+Space
    const CshPos &p = m_csh.hintedStringPos;
    if (start!=end || m_csh.Hints.size()==0 || m_bWasAutoComplete ||
        (p.first_pos != p.last_pos && !m_bUserRequested && p.first_pos == start)) {
        CancelHintMode();
        return;
    }
    CPoint pt;
    pt.x = PosFromChar(m_csh.hintedStringPos.first_pos).x;
    int line = LineFromChar(m_csh.hintedStringPos.first_pos);
    pt.y = PosFromChar(LineIndex(line+1)).y;
    if (pt.y==0) { //last line
        if (line==0) //empty file
            pt.y=20;
        else
            pt.y = 2*PosFromChar(LineIndex(line)).y-PosFromChar(LineIndex(line-1)).y;
    }
    ClientToScreen(&pt);
    CString text;
    long s,e;
    GetSel(s,e);
    if (setUptoCursor) {
        m_bTillCursorOnly = true;
        if (s<m_csh.hintedStringPos.last_pos)
            m_csh.hintedStringPos.last_pos = s;
    }
    if (m_csh.hintedStringPos.first_pos>=0) 
        GetTextRange(m_csh.hintedStringPos.first_pos, m_csh.hintedStringPos.last_pos, text);
    
    bool changed = m_hintsPopup.m_listBox.PreprocessHints(m_csh, (const char *)text, m_bUserRequested, m_bWasReturnKey);
    //See how many of the remaining hits fit the word under the cursor 
    auto hit = m_csh.Hints.end(), i = m_csh.Hints.begin();
    for (; i!=m_csh.Hints.end(); i++) {
        //find a non-selectable item or one that the text under cursor fits 
        if (!i->selectable)
            continue;
        else if (text == i->plain.substr(0, text.GetLength()).c_str()) {
            if (hit == m_csh.Hints.end())
                hit = i;
            else 
                break; //this is the second such item, let us show the hint window
        }
    }
    bool onlyOne = i==m_csh.Hints.end() && hit!=m_csh.Hints.end();
    //if there is only one hit and it is equal to the word under cursor, cancel hit mode
    if (onlyOne && hit->plain.c_str() == text) {
        CancelHintMode();
        return;
    }
    //If we are about to start hint mode due to a Ctrl+Space and we are at the end of the word under cursor, 
    //then check how many hints do we fit on and if there is only one, auto complete without
    //popping up the hint list
    if (onlyOne && !InHintMode() && m_bUserRequested && !m_bTillCursorOnly && m_csh.hintedStringPos.last_pos==s) {
        ReplaceHintedString(hit->plain.c_str(), true);
        return;
    } 
    //Show the window (or keep it shown & update the list)
    m_hintsPopup.Show(changed, text, pt.x, pt.y);
    SetFocus();
}

void CCshRichEditCtrl::CancelHintMode()
{
    m_hintsPopup.Hide();
    m_bUserRequested = false;
    m_bTillCursorOnly = false; 
    m_bWasAutoComplete = false;
}

/**Replace the part of the text pointed by m_csh.hintedStringPos to substitute.*/
void CCshRichEditCtrl::ReplaceHintedString(const char *substitute, bool endHintMode)
{
    CshPos pos = m_csh.hintedStringPos;
    CString subst(substitute); //CancelHintMode will likely destroy "substitute"
    //Need to call before CancelHintMode before ReplaceSel, since after replace that results a call 
    //to OnCommand and we get confused, if we are still in Hint mode then
    if (endHintMode) {
        CancelHintMode(); 
        //Set flag So that we do not re-enter hint mode as a result of change we do below
        m_bWasAutoComplete = true; 
    }
    const auto state = DisableWindowUpdate();
    SetSel(pos.first_pos, pos.last_pos);
    ReplaceSel(subst); 
    UpdateCSH(CSH);
    RestoreWindowUpdate(state);
}


CCshRichEditCtrl::WindowUpdateStatus CCshRichEditCtrl::GetWindowUpdate()
{
    WindowUpdateStatus ret;
    ret.first = m_bRedrawState;
    ret.second = m_parent->m_bSuspendNotifications;
    return ret;
}

CCshRichEditCtrl::WindowUpdateStatus CCshRichEditCtrl::DisableWindowUpdate()
{
    WindowUpdateStatus ret;
    ret.first = m_bRedrawState;
    ret.second = m_parent->m_bSuspendNotifications;
    m_bRedrawState = false;
    m_parent->m_bSuspendNotifications = true;
    if (ret.first)
        SetRedraw(false);
    return ret;
}

void CCshRichEditCtrl::RestoreWindowUpdate(const WindowUpdateStatus &s)
{
    m_parent->m_bSuspendNotifications = s.second;
    if (s.first != m_bRedrawState) {
        m_bRedrawState = s.first;
        SetRedraw(m_bRedrawState);        if (m_bRedrawState) {            Invalidate();            UpdateWindow();        }    }}


/////////////////////////////////////////////////////////////////////////////
// CEditorBar

static UINT nFindReplaceDialogMessage = ::RegisterWindowMessage(FINDMSGSTRING);

BEGIN_MESSAGE_MAP(CEditorBar, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
    ON_WM_TIMER()
	ON_NOTIFY(EN_SELCHANGE, IDC_INTERNAL_EDITOR, OnSelChange)
	ON_REGISTERED_MESSAGE(nFindReplaceDialogMessage, OnFindReplaceMessage)
END_MESSAGE_MAP()

CEditorBar::CEditorBar() : m_ctrlEditor(this), m_pFindReplaceDialog(NULL)
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    m_lInitialSearchPos = 0;
    m_bFirstSearch = true;
	m_bSuspendNotifications = FALSE;
	m_bLastMatchCase = pApp ? pApp->GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_FINDMATCHCASE, 1) : 1; 
	m_bLastMatchWholeWord = false;
	m_ptLastFindPos.x = pApp ? pApp->GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_FINDWINPOS_X, 0) : 0;
	m_ptLastFindPos.y = pApp ? pApp->GetProfileInt(REG_SECTION_SETTINGS, REG_KEY_FINDWINPOS_Y, 0) : 0;
	CDC dc;
	dc.CreateCompatibleDC(NULL);
	CRect r;
	dc.GetClipBox(&r);
	r.bottom -= 20;
	r.right -=20;
	if (!r.PtInRect(m_ptLastFindPos)) m_ptLastFindPos.x = m_ptLastFindPos.y = 0;
    m_totalLenAtPreviousSelChange = 0;
}

CEditorBar::~CEditorBar()
{
	if (m_ptLastFindPos != CPoint(0,0)) {
		CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
		ASSERT(pApp != NULL);
        if (pApp) {
            pApp->WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_FINDWINPOS_X, m_ptLastFindPos.x);
            pApp->WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_FINDWINPOS_Y, m_ptLastFindPos.y);
            pApp->WriteProfileInt(REG_SECTION_SETTINGS, REG_KEY_FINDMATCHCASE, m_bLastMatchCase);
        }
    }
}

int CEditorBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	//m_Font.CreateStockObject(DEFAULT_GUI_FONT);
	m_Font.CreateFont(16, 0, 0, 0, FW_NORMAL, false, false, false, ANSI_CHARSET, 
		              OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
					  FIXED_PITCH|FF_MODERN, NULL);
	LOGFONT logFont;
	m_Font.GetLogFont(&logFont);

	CRect rectClient(0, 0, lpCreateStruct->cx, lpCreateStruct->cy);

	// Create output pane:
	const DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | 
						  ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_NOHIDESEL;

	if (!m_ctrlEditor.Create(dwStyle, rectClient, this, IDC_INTERNAL_EDITOR))
	{
		TRACE0("Failed to create output window\n");
		return -1;      // fail to create
	}
	m_ctrlEditor.SetTextMode(TM_SINGLECODEPAGE | TM_MULTILEVELUNDO | TM_RICHTEXT);
	m_ctrlEditor.SetFont(&m_Font);
	m_ctrlEditor.SetFocus();
	m_ctrlEditor.SetEventMask(m_ctrlEditor.GetEventMask() | ENM_CHANGE | ENM_SELCHANGE);

	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	CFrameWnd *pMainWnd = pApp ? dynamic_cast<CFrameWnd*>(pApp->GetMainWnd()) : NULL;
	ASSERT(pMainWnd!=NULL);
	if (pMainWnd->GetActiveView() != NULL) {
		CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(pMainWnd->GetActiveView()->GetDocument());
		if (pDoc != NULL) {
            if (pDoc->m_ExternalEditor.IsRunning())
                SetReadOnly();
            const auto state = pApp->m_pWndEditor->m_ctrlEditor.DisableWindowUpdate();
            pApp->m_pWndEditor->m_ctrlEditor.SetSel(0,-1);
            pApp->m_pWndEditor->m_ctrlEditor.ReplaceSel(pDoc->m_itrEditing->GetText());
            pApp->m_pWndEditor->m_ctrlEditor.RestoreWindowUpdate(state);
        }
	}

	return 0;
}

#define READONLY_STRING " (Read-only)"
void CEditorBar::SetReadOnly(bool readonly)
{
	if (!m_ctrlEditor.SetReadOnly(readonly)) return;
	CString text;
	GetWindowText(text);
	if (text.Right(12).CompareNoCase(READONLY_STRING)==0) {
		if (!readonly)
			SetWindowText(text.Left(text.GetLength()-12));
	} else if (readonly)
		SetWindowText(text + READONLY_STRING);
}


void CEditorBar::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	CRect rc;
	GetClientRect(rc);

	m_ctrlEditor.SetWindowPos(NULL, rc.left + 1, rc.top + 1, rc.Width() - 2, rc.Height() - 2, SWP_NOACTIVATE | SWP_NOZORDER );
}

/////////////////////////////////////////////////////////////////////////////
// CEditorBar message handlers

void CEditorBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_ctrlEditor.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CEditorBar::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_ctrlEditor.SetFocus();
}


BOOL CEditorBar::OnCommand(WPARAM wParam, LPARAM lParam)
{
	HWND hWndCtrl = (HWND)lParam;
	int nCode = HIWORD(wParam);
	if (hWndCtrl != m_ctrlEditor) return CDockablePane::OnCommand(wParam, lParam);
	if (nCode != EN_CHANGE) return CDockablePane::OnCommand(wParam, lParam);
	if (m_bSuspendNotifications) return TRUE;

    const bool hint = m_ctrlEditor.UpdateCSH(CCshRichEditCtrl::CSH);
    //Update hints if we are in hint mode
    if (m_ctrlEditor.InHintMode()) {
        //Kill the user requested nature
        if (!hint) //We have switched to a different string to hint
            m_ctrlEditor.CancelUserSelected();
        m_ctrlEditor.StartHintMode(false); //false == do not change m_bTillCursorOnly
    } else {
        long s, e;
        m_ctrlEditor.GetSel(s,e);
        //Enter hint mode only if we typed a character (outside hint mode
        //this is what UpdateCSH returns)
        if (hint && s==e) {
            bool till_cursor_only = false;
            if (s<=1) 
                till_cursor_only = true;
            else {
                CString str;
                m_ctrlEditor.GetTextRange(s-2,s-1, str);
                //Non-ANSI characters make isalnum to throw an assert
                if (str[0]<0 || (str[0]!='_' && !isalnum(str[0]))) 
                    till_cursor_only = true;
            }
            m_ctrlEditor.StartHintMode(till_cursor_only);
        }
    }        
	return TRUE;
}

void CEditorBar::OnSelChange(NMHDR * /*pNotifyStruct*/, LRESULT *result) 
{
    if (m_bSuspendNotifications) {
        m_totalLenAtPreviousSelChange = m_ctrlEditor.GetTextLength();
        return;
    }
    //if we have just moved, not inserted or deleted cancel partial match and hints
    //(otherwise we will get an EN_CHANGE right after this and we redo
    //csh and hints all the same. This is to prevent hint window flicker.)
    if (m_totalLenAtPreviousSelChange == m_ctrlEditor.GetTextLength()) {
        m_ctrlEditor.CancelPartialMatch();
        CMscGenDoc *pDoc = GetMscGenDocument();
        if (pDoc)
            pDoc->OnInternalEditorSelChange();
    } else 
        m_totalLenAtPreviousSelChange = m_ctrlEditor.GetTextLength();
	*result = 0;
}


//Find replace related

bool CEditorBar::UpdateLastFindStringFromSelection()
{
	//See if there is a selection and if it is within a single line. If so, copy that to the search box
	long lStart, lEnd;
	m_ctrlEditor.GetSel(lStart, lEnd);
	if (lStart == lEnd) return false;
	if (m_ctrlEditor.LineFromChar(lStart) != m_ctrlEditor.LineFromChar(lEnd)) return false;
	m_sLastFindString = m_ctrlEditor.GetSelText();
	return true;
}

void CEditorBar::OnEditFindReplace(bool findOnly)
{
	//If window is up, store location and destroy
	if (m_pFindReplaceDialog) {
		CRect r;
		m_pFindReplaceDialog->GetWindowRect(r);
		m_ptLastFindPos = r.TopLeft();
		delete m_pFindReplaceDialog;
	}
	UpdateLastFindStringFromSelection();
	
	m_pFindReplaceDialog = new CFindReplaceDialog( );
	const DWORD dwFlags = FR_HIDEUPDOWN | (m_bLastMatchCase?FR_MATCHCASE:0) | (m_bLastMatchWholeWord?FR_WHOLEWORD:0);
	m_pFindReplaceDialog->Create(findOnly, m_sLastFindString, m_sLastReplaceString, dwFlags, this );
	m_pFindReplaceDialog->SetActiveWindow( );
	m_pFindReplaceDialog->ShowWindow( TRUE );
	if (m_ptLastFindPos != CPoint(0,0))
		m_pFindReplaceDialog->SetWindowPos(NULL, m_ptLastFindPos.x, m_ptLastFindPos.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void CEditorBar::OnEditRepeat()
{
	//If the find window is not there use the selection as search string
	if (!m_pFindReplaceDialog || !::IsWindow(*m_pFindReplaceDialog) || !m_pFindReplaceDialog->IsWindowVisible())
			UpdateLastFindStringFromSelection();
	//If there is nothing to search or we do not find it, beep
	if (m_sLastFindString.IsEmpty() || 
		!FindText(m_sLastFindString, m_bLastMatchCase, m_bLastMatchWholeWord))
		TextNotFound(m_sLastFindString);
}

LRESULT CEditorBar::OnFindReplaceMessage(WPARAM /*wParam*/, LPARAM lParam)
{

	CFindReplaceDialog* pFindReplace = CFindReplaceDialog::GetNotifier(lParam);
    ASSERT(pFindReplace != NULL);
    if (!pFindReplace) return 0;

	//Store values for next opening or OnEditRepeat
	m_sLastFindString = pFindReplace->GetFindString();
	m_sLastReplaceString = pFindReplace->GetReplaceString();
	m_bLastMatchCase = pFindReplace->MatchCase();
	m_bLastMatchWholeWord = pFindReplace->MatchWholeWord();
	CRect r;
	pFindReplace->GetWindowRect(r);
	m_ptLastFindPos = r.TopLeft();

	if (pFindReplace->IsTerminating()) {
		m_pFindReplaceDialog = NULL;
		return 0;
    } 

	if (pFindReplace->ReplaceAll()) {
		CWaitCursor wait;
		if (!SameAsSelected(pFindReplace->GetFindString(), pFindReplace->MatchCase())) {
			if (!FindText(pFindReplace->GetFindString(), pFindReplace->MatchCase(), pFindReplace->MatchWholeWord())) {
				TextNotFound(pFindReplace->GetFindString());
				return 0;
			}
		}
        const bool saved_notification = m_bSuspendNotifications;
        m_bSuspendNotifications = true;
        do {
			m_ctrlEditor.ReplaceSel(pFindReplace->GetReplaceString());
		} while (FindText(pFindReplace->GetFindString(), pFindReplace->MatchCase(), pFindReplace->MatchWholeWord()));
		m_ctrlEditor.HideSelection(FALSE, FALSE);
		m_bSuspendNotifications = saved_notification;
        m_ctrlEditor.UpdateCSH(CCshRichEditCtrl::CSH);
		return 0;
	}
	bool findnext = pFindReplace->FindNext();
	if (pFindReplace->ReplaceCurrent()) {
		long start, end;
		m_ctrlEditor.GetSel(start, end);
		//if nothing is selected, skip replace, just go to find next
		if (start!=end) 
			m_ctrlEditor.ReplaceSel(pFindReplace->GetReplaceString(), FALSE);
		findnext = true;
	}
	//Find next for both FindNext and ReplaceCurrent
	if (findnext) {
		if (FindText(pFindReplace->GetFindString(), pFindReplace->MatchCase(), pFindReplace->MatchWholeWord())) {
			AdjustDialogPosition(pFindReplace);
			CRect rect;
			pFindReplace->GetWindowRect(rect);
			m_ptLastFindPos = rect.TopLeft();
		} else
			TextNotFound(pFindReplace->GetFindString());
	} 
	return 0;
}


BOOL CEditorBar::SameAsSelected(LPCTSTR lpszCompare, BOOL bCase)
{
	// check length first
	size_t nLen = lstrlen(lpszCompare);
	long lStartChar, lEndChar;
	m_ctrlEditor.GetSel(lStartChar, lEndChar);
	if (nLen != (size_t)(lEndChar - lStartChar))
		return FALSE;

	// length is the same, check contents
	CString strSelect = m_ctrlEditor.GetSelText();
	return (bCase && lstrcmp(lpszCompare, strSelect) == 0) ||
		(!bCase && lstrcmpi(lpszCompare, strSelect) == 0);
}

void CEditorBar::AdjustDialogPosition(CDialog* pDlg)
{
	ASSERT(pDlg != NULL);
	long lStart, lEnd;
	m_ctrlEditor.GetSel(lStart, lEnd);
	CPoint point = m_ctrlEditor.GetCharPos(lStart);
	ClientToScreen(&point);
	CRect rectDlg;
	pDlg->GetWindowRect(&rectDlg);
	if (rectDlg.PtInRect(point))
	{
		if (point.y > rectDlg.Height())
			rectDlg.OffsetRect(0, point.y - rectDlg.bottom - 20);
		else
		{
			int nVertExt = GetSystemMetrics(SM_CYSCREEN);
			if (point.y + rectDlg.Height() < nVertExt)
				rectDlg.OffsetRect(0, 40 + point.y - rectDlg.top);
		}
		pDlg->MoveWindow(&rectDlg);
	}
}


BOOL CEditorBar::FindText(LPCTSTR lpszFind, BOOL bCase, BOOL bWord, BOOL bNext /* = TRUE */)
{
	ASSERT_VALID(this);
	ASSERT(lpszFind != NULL);
	FINDTEXTEX ft;

	m_ctrlEditor.GetSel(ft.chrg);
	if (m_bFirstSearch) {
		if (bNext)
			m_lInitialSearchPos = ft.chrg.cpMin;
		else
			m_lInitialSearchPos = ft.chrg.cpMax;
		m_bFirstSearch = FALSE;
	}

	// lpstrText should be const
	ft.lpstrText = (LPTSTR) lpszFind;

	if (ft.chrg.cpMin != ft.chrg.cpMax) {// i.e. there is a selection
		if (bNext)
			ft.chrg.cpMin++;
		else {
			// won't wraparound backwards
			if (ft.chrg.cpMin<0) ft.chrg.cpMin = 0;
		}
	}

	DWORD dwFlags = bCase ? FR_MATCHCASE : 0;
	dwFlags |= bWord ? FR_WHOLEWORD : 0;

	ft.chrg.cpMax = m_ctrlEditor.GetTextLength() + m_lInitialSearchPos;

	if (bNext) {
		if (m_lInitialSearchPos >= 0)
			ft.chrg.cpMax = m_ctrlEditor.GetTextLength();

		dwFlags |= FR_DOWN;
		ASSERT(ft.chrg.cpMax >= ft.chrg.cpMin);
	} else {
		if (m_lInitialSearchPos >= 0)
			ft.chrg.cpMax = 0;

		dwFlags &= ~FR_DOWN;
		ASSERT(ft.chrg.cpMax <= ft.chrg.cpMin);
	}

	// if we find the text return TRUE
	if (FindAndSelect(dwFlags, ft) != -1)
		return TRUE;
	// if the original starting point was not the beginning of the buffer
	// and we haven't already been here
	else if (m_lInitialSearchPos > 0) {
		if (bNext) {
			ft.chrg.cpMin = 0;
			ft.chrg.cpMax = m_lInitialSearchPos;
		} else {
			ft.chrg.cpMin = m_ctrlEditor.GetTextLength();
			ft.chrg.cpMax = m_lInitialSearchPos;
		}
		m_lInitialSearchPos = m_lInitialSearchPos - m_ctrlEditor.GetTextLength();
		return FindAndSelect(dwFlags, ft) != -1;
	}
	// not found
	else
		return FALSE;
}

long CEditorBar::FindAndSelect(DWORD dwFlags, FINDTEXTEX& ft)
{
	long index = m_ctrlEditor.FindText(dwFlags, &ft);
	if (index != -1) // i.e. we found something
		m_ctrlEditor.SetSel(ft.chrgText);
	return index;
}

void CEditorBar::TextNotFound(LPCTSTR /*lpszFind*/)
{
	ASSERT_VALID(this);
	m_bFirstSearch = TRUE;
	MessageBeep(MB_ICONHAND);
}


void CEditorBar::SelectAll()
{
    CHARRANGE cr;
    cr.cpMin = 0;
    cr.cpMax = m_ctrlEditor.GetTextLength();
	m_ctrlEditor.SetSel(cr);
}
