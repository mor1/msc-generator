/*
    This file is part of Msc-generator.
	Copyright 2008,2009,2010,2011,2012,2013 Zoltan Turanyi
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
END_MESSAGE_MAP()



CCshRichEditCtrl::CCshRichEditCtrl(CEditorBar *parent) : 
    m_csh(ArcBase::defaultDesign, NULL), m_hintsPopup(parent, this),
    m_parent(parent)
{
    m_tabsize = 4;
	m_bCshUpdateInProgress = false;  
    m_bWasReturnKey = false;
    m_bUserRequested = false;
    m_bTillCursorOnly = false;
    m_bWasAutoComplete = false;
    m_csh_index = -3;
    last_change.Clear();
}

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
 * @param [in] Max The maximum value to return.
 * @returns -1 if all the line is whitespace (or the first Max chars)
 *          else the offset of the first non-whitespace character*/
int CCshRichEditCtrl::FirstNonWhitespaceIdent(const char *str, int Max) 
{
	int i = 0;
	if (Max == -1) 
		while (str[i]==' ' || str[i]=='\t') i++;
	else 
		while (i<Max && (str[i]==' ' || str[i]=='\t')) i++;
	if (!str[i] || i==Max) return -1;
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
 * @param [in] lStart The position of the cursor (assumedly inside a label)
 * @param [out] line The number of the line we found the colon in.
 * @returns Return -1 if we are 1) not in a label or 2) smart ident is off 
 *          or 3) no csh or 4) the label is not preceeeded by a colon.
 *          Else return the column of the first non whitespace character 
 *          after the colon*/
int CCshRichEditCtrl::FindColonLabelIdent(long lStart, int *line) 
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    if (!pApp || !pApp->m_bSmartIdent || !pApp->m_bDoCshProcessing) return -1;
	//Go through the list of color syntax highlight entries
	for (auto  i = m_csh.CshList.begin(); i!=m_csh.CshList.end(); i++) {
		//if we do not fall into a label skip
		if (i->color != COLOR_LABEL_TEXT || i->first_pos>=lStart || i->last_pos<lStart ) continue;
		//if yes and is preceeded by a colon...
		auto j = i;
		j--;
		if (j->color != COLOR_COLON) continue;
		//...then find the first non-space in the label
		int nLine, nCol;
		ConvertPosToLineCol(i->first_pos, nLine, nCol);
		CString strLine;
		int nLineLength = GetLineString(nLine, strLine);
		int pos = nCol;
		while (pos>0 && pos<nLineLength && (strLine[pos]==' ' || strLine[pos]=='\t')) pos++;
		//if there is such a non-space in the label on the same line as the colon
		if (pos>0 && pos<nLineLength) {
			int dummy;
			if (line) ConvertPosToLineCol(j->first_pos, *line, dummy); //the line of the colon
			return pos;
		}
	}
	return -1;
}

/** Find what is the proper identation for this line 
 * Consider if we are in a label, and consider '{' and '}' chars.
 * @param [in] lStart the line we talk about
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
        int colon_line;
        if (FindColonLabelIdent(ConvertLineColToPos(line, ident), &colon_line) >= 0)
            return FindCurrentLineIdent(LineIndex(colon_line));

        //If no label is above us, check if the line ends in an '{'
        //if yes, add extra identation
        int last_col = LastNonWhitespaceIdent(strLine);
        if (last_col>0 && strLine[last_col] == '{')
            ident += m_tabsize;
        return ident;
    }
    //reached the beginning of the file, no whitespace above us
    return 0;
}


/** Return the column of the first non space in the current line
 * @param [in] lStart the position of the cursor of which line we are interested in
 * @retun -1 if current line has only spaces or is empty*/
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
    const bool haveSmartIdent = pApp && pApp->m_bSmartIdent && pApp->m_bDoCshProcessing;
	if (haveSmartIdent) {
		EColorSyntaxType csh_color = m_csh.GetCshAt(pos_to_be_inserted+1);
		//Dont do anything in a label
		if (csh_color == COLOR_LABEL_TEXT || csh_color == COLOR_LABEL_ESCAPE) return -1;
	}
	int nLine, nCol;
	ConvertPosToLineCol(pos_to_be_inserted, nLine, nCol);
	CString strLine;

	//Find the matching brace before us
	int num_braces = 0;
	for (int line = nLine; line>=0; line--) {
		int nLineLength = GetLineString(nLine, strLine);
		if (line == nLine) 
			nLineLength = nCol;  //on the current line, start searching from insertion point
		//search for opening brace from the back
		while(--nLineLength>=0) {
			if (strLine[nLineLength] != '{' && strLine[nLineLength] != '}') continue;
			//if color syntax highlighting is not enabled we cannot check for label
			if (haveSmartIdent) {
				//if a brace, but in a label, ignore
				EColorSyntaxType csh_color = m_csh.GetCshAt(ConvertLineColToPos(nLine, nLineLength)+1);
				if (csh_color == COLOR_LABEL_TEXT || csh_color == COLOR_LABEL_ESCAPE) continue;
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
 *                        from prev_ident in <int>*m_tabsize
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
 * @param col The current cursor position in the line
 * @param lStart The current cursor position
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
    if (current_ident == -1) return false; //empty line
    if (target_ident == current_ident) return false; //nothing to do
    if (standalone)
        SetRedraw(false);
    if (target_ident > current_ident) {
        SetSel(lStart-col, lStart-col);
        if (standalone) {
            _ASSERT(last_change.ins==0 && last_change.del==0);
            last_change.start = lStart-col;
            last_change.ins = target_ident-current_ident;
            last_change.del = 0;
        }
        ReplaceSel(CString(' ', target_ident-current_ident));
    } else {
        SetSel(lStart-col, lStart-col+current_ident-target_ident);
        if (standalone) {
            _ASSERT(last_change.ins==0 && last_change.del==0);
            last_change.start = lStart-col;
            last_change.ins = 0;
            last_change.del = current_ident-target_ident;
        }
        ReplaceSel("");
    }
    if (standalone) {
        if (col>=current_ident)
            SetSel(lStart-current_ident+target_ident, lEnd-current_ident+target_ident);
        else
            SetSel(lStart, lEnd);
        SetRedraw(true);
    } else {
        m_csh.AdjustCSH(lStart-col, target_ident-current_ident);
    }
    return true;
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
    if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_CHAR)
        last_change.Clear();
    else    
        return CRichEditCtrl::PreTranslateMessage(pMsg);
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
                    ////Update CSH as it does not happen if we return TRUE
                    //UpdateCsh(); //replacehintedstring has filled in last_change
                    return TRUE;
                }
                //if nothing selected let it run further as if not in hint mode
            }
        } else if (pMsg->message == WM_CHAR) {
            if (isalnum(pMsg->wParam) || pMsg->wParam == '_' || pMsg->wParam == VK_BACK)
                //characters that can make up a hinted keyword - 
                //we insert them and re-calc hints afterwards in OnCommand
                goto insert_char;
            //do nothing if no item is selected
            const CshHint *item = m_hintsPopup.m_listBox.GetSelectedHint();
            if (!item) goto insert_char;
            //Expand hint under cursor, if it is full match or prefix
            if (item->state != HINT_ITEM_SELECTED) goto insert_char;
            if (pMsg->wParam == '.') {
                //expand only till the next dot in the hint
                unsigned pos = m_csh.hintedStringPos.last_pos - m_csh.hintedStringPos.first_pos;
                pos = item->plain.find_first_of('.', pos);
                if (pos != string::npos) {
                    ReplaceHintedString(item->plain.substr(0, pos).c_str(), false);
                    last_change.ins++; //as we let windows insert the '.'
                    return FALSE;
                }
            }
            ReplaceHintedString(item->plain.c_str(), !item->keep);
            last_change.ins++; //as we let windows insert whatever char is in the msg
            return FALSE;
        }
    }

    //show hints for ctrl+space
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE 
        && GetKeyState(VK_CONTROL) & 0x8000) {
            m_bUserRequested = true;
            UpdateCsh();
            //see if we are at the beginning of a word
            long s, e;
            GetSel(s, e);
            bool till_cursor_only = false;
            if (s==0) till_cursor_only = true;
            else {
                CString str;
                GetTextRange(s-1,s, str);
                if (str[0]!='_' && !isalnum(str[0])) till_cursor_only = true;
            }
            //if so, set the hint mode such that we only consider the word under the curson
            //up to the cursor (and cut away the remainder)
            m_bWasAutoComplete = false; //do not prevent hint mode on this basis
            StartHintMode(till_cursor_only); 
            //Update CSH as it does not happen if we return TRUE
            //if (last_change.ins>0 ||last_change.del>0)
            //    UpdateCsh(); //if replacehintedstring has filled in last_change
            return TRUE;
    }

    //Do nothing for escape
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) {
        NotifyDocumentOfChange(); //This kills tracking mode and stores the selection onto the undo buffer
        return TRUE;
	}
    //call parent if not keys relevant for smart identing
    if (pMsg->message == WM_KEYDOWN) {
        if (pMsg->wParam == VK_INSERT) goto insert_clipboard;
        if (pMsg->wParam == VK_DELETE) {
            long end;
            GetSel(last_change.start, end);
            last_change.del = std::max(1L, end-last_change.start);
            last_change.ins = 0;
            return FALSE;
        }
        if (pMsg->wParam != VK_TAB && pMsg->wParam != VK_RETURN && pMsg->wParam != VK_BACK)
            return CRichEditCtrl::PreTranslateMessage(pMsg);
    }
    if (pMsg->message == WM_CHAR && pMsg->wParam != '}')
        goto insert_char;

    long lStart, lEnd;
	GetSel(lStart, lEnd);
	int line, col;
	ConvertPosToLineCol(lStart, line, col);
	const int current_ident = FindCurrentLineIdent(lStart);

	if (pMsg->wParam == '}') {
        //if we are in a label, do nothing
        if (FindColonLabelIdent(lStart)>-1) goto insert_char;
        if (current_ident!=-1 && current_ident<col) goto insert_char; //not in leading whitespace
		int brace_ident = FindIdentForClosingBrace(lStart);
		if (brace_ident == -1) brace_ident = 0; //beginning of line if no matching opening brace
		SetCurrentIdentTo(brace_ident, current_ident, col, lStart, lEnd, true);
		goto insert_char; //let RicheditCtrl insert the }
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
        CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
        _ASSERT(pApp);
        const bool shift = bool(GetKeyState(VK_SHIFT) & 0x8000);
        bool changed = false;
        if (pApp->m_bTABIdents && !m_bCshUpdateInProgress) { //TAB mode
            int eLine, eCol;
            ConvertPosToLineCol(lEnd, eLine, eCol);
            if (line == eLine) {
                const int ident = FindProperLineIdent(line);
                changed = SetCurrentIdentTo(ident, current_ident, col, lStart, lEnd, true);
            } else {
                //leave last change as if nothing happened
                //We go on and adjust csh all the fime in SetCurrentIdentTo(..., false).
                SetRedraw(false);
                m_bCshUpdateInProgress = true;
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
                m_bCshUpdateInProgress = false;
                SetRedraw(true);
            }
        } else if (!m_bCshUpdateInProgress) { //smart ident, but no tab mode
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
                    if (ident<col) {
                        SetRedraw(false);
                        SetSel(lStart+(ident-col), lStart);
                        ReplaceSel("");
                        SetRedraw(true);
                        last_change.start = lStart+(ident-col);
                        last_change.del = col-ident;
                    } else {
                        ReplaceSel(CString(' ', ident-col));
                        last_change.start = lStart;
                        last_change.ins = ident-col;
                    }
                    changed = true;
                }
            } else {
                //a full selection, do identation
                //First fill in last change as if we deleted the whole selection
                //and inserted something - we do not track where do we change what
                last_change.start = lStart;
                last_change.del = lEnd-lStart;
                last_change.ins = -1;
                last_change.length_before = GetTextLength();

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
                m_bCshUpdateInProgress = true;
                SetRedraw(false);
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
                SetRedraw(true);
                m_bCshUpdateInProgress = false;
                changed = true;
            }
        }
        if (changed) {
            UpdateCsh();
            NotifyDocumentOfChange();
        }
        return TRUE;
	}
	if (pMsg->wParam == VK_RETURN) {
        m_bWasReturnKey=true;
        const int ident = FindProperLineIdent(line);
		//If the determined ident is zero, just let the system handle the RETURN
		if (ident==0) 
			goto insert_char;
		//insert that many spaces as ident after the newline
		CString spaces(' ', ident);
		spaces.Insert(0, "\n");
        last_change.start = lStart;
        last_change.del = lEnd-lStart;
        last_change.ins = ident+1;
        ReplaceSel(spaces, FALSE);
        ////Update CSH as it does not happen if we return TRUE
        //UpdateCsh(); 
		return TRUE;
	}
	if (pMsg->wParam == VK_BACK) {
		if (col==0) return FALSE;
		//if not in leading whitespace or at beginning of line
        if ((col==0) || (current_ident!=-1 && current_ident<col)) {
            if (lStart!=lEnd) {
                last_change.start = lStart;
                last_change.del = lEnd-lStart;
            } else if (lStart>0) {
                last_change.start = lStart-1;
                last_change.del = 1;
            }
            return FALSE;
        }
		//in leading whitespace, consider smart ident and/or previous line indent
		const int smartIdent = FindProperLineIdent(line);
		int ident = CalcTabStop(col, false, smartIdent, current_ident, true);
        SetCurrentIdentTo(ident, current_ident, col, lStart, lEnd, true);
        ////Update CSH as it does not happen if we return TRUE
        //UpdateCsh(); //setcurrentidentto(...true) has filled in last_change
        return TRUE;
	}
	return FALSE;

insert_char:
    long end;
    GetSel(last_change.start, end);
    last_change.del = end-last_change.start;
    last_change.ins = 1;
    return FALSE;

insert_clipboard:
    end;
    GetSel(last_change.start, end);
    last_change.del = end-last_change.start;
    last_change.ins = -1;
    last_change.length_before = GetTextLength();
    return FALSE;
}

void CCshRichEditCtrl::UpdateText(const char *text, int lStartLine, int lStartCol, int lEndLine, int lEndCol, bool preventNotification)
{
	CEditorUndoRecord undoRec;
	undoRec.text = text;
	EnsureCRLF(undoRec.text);
	SetRedraw(false);
	m_bCshUpdateInProgress = preventNotification; 
    m_csh.CshList.clear();
    m_csh.CshErrors.clear();
    SetWindowText(undoRec.text);
	m_bCshUpdateInProgress = false;
	if (preventNotification) UpdateCsh(); //This was done, if notifications were on during SetWindowText()
	SetSel(ConvertLineColToPos(lStartLine, lStartCol), ConvertLineColToPos(lEndLine, lEndCol));
    SetRedraw(true);
	SetFocus();
}

void CCshRichEditCtrl::UpdateText(const char *text, CHARRANGE &cr, bool preventNotification)
{
	CEditorUndoRecord undoRec;
	undoRec.text = text;
	EnsureCRLF(undoRec.text);
	SetRedraw(false);
	m_bCshUpdateInProgress = preventNotification; 
    m_csh.CshList.clear();
    m_csh.CshErrors.clear();
	SetWindowText(undoRec.text);
	SetSel(cr);
	m_bCshUpdateInProgress = false;
	if (preventNotification) UpdateCsh(); //This was done, if notifications were on during SetWindowText()
    SetRedraw(true);
	SetFocus();
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
        if (o->first_pos == n->first_pos) {
            if (o->last_pos != n->last_pos) {
                if (o->last_pos > n->last_pos) {//if old csh is longer, we add its neutral version
                    delta.AddToBack(*o);
                    delta.back().color = neutral;
                }
                delta.AddToBack(*n);
            }
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


bool CCshRichEditCtrl::UpdateCsh(bool force)
{
    if (m_bCshUpdateInProgress) return false;
    CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
    ASSERT(pApp != NULL);
    //Keep running if color syntax highlighting is not enabled, but we are forced to reset csh to normal
    if (pApp && !pApp->m_bDoCshProcessing && !force) return false;
    CHARFORMAT *const scheme = pApp->m_csh_cf[pApp->m_nCshScheme];

    CHARRANGE cr;
    GetSel(cr);
    //if we do not do CSH processing, but force is true, we clear all formatting now
    if (!pApp->m_bDoCshProcessing && force) {
        SetRedraw(false);
        m_bCshUpdateInProgress = true;
        POINT scroll_pos;
        ::SendMessage(m_hWnd, EM_GETSCROLLPOS, 0, (LPARAM)&scroll_pos);
        SetSel(0, -1); //select all
        SetSelectionCharFormat(scheme[COLOR_NORMAL]); //set formatting to neutral
        SetSel(cr);
        ::SendMessage(m_hWnd, EM_SETSCROLLPOS, 0, (LPARAM)&scroll_pos);

        m_bCshUpdateInProgress = false;
        SetRedraw(true);
        Invalidate();
        UpdateWindow();
        m_csh_index = -3;
        return false;
    }

    //here pApp->m_bDoCshProcessing is true - Parse the text for CSH

    //Take text we display and remove CRLF
    CString text;
    GetWindowText(text);
    RemoveCRLF(text);

    //if we had csh entries before, consider last_change
    if (m_csh.CshList.size()) {
        if (last_change.ins == -1)
            last_change.ins = text.GetLength() - last_change.length_before + last_change.del;
        m_csh.AdjustCSH(last_change.start, last_change.ins-last_change.del);
        //if we inserted, mark inserted text as "COLOR_MAX" so that we
        //mark it properly
        if (last_change.ins) 
            //search for the place to insert
            for (unsigned u = 0; u<m_csh.CshList.size(); u++)
                if (m_csh.CshList[u].first_pos > last_change.start) {
                    m_csh.CshList.insert(m_csh.CshList.begin()+u, CshEntry());
                    m_csh.CshList[u].first_pos = last_change.start;
                    m_csh.CshList[u].last_pos = last_change.start + last_change.ins;
                    m_csh.CshList[u].color = COLOR_MAX;
                    break;
                }
        last_change.Clear();
    }


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
    bool ret = m_csh.hintedStringPos.first_pos <= old_uc.last_pos && old_uc.first_pos<=m_csh.hintedStringPos.last_pos;

    if (!pApp->m_bShowCsh) return ret;

    //Apply the color syntax to the text in the editor
    if (pApp->m_bShowCshErrorsInWindow && pApp->m_pWndOutputView) {
        MscError Error;
        Error.AddFile("Hint");
        std::list<CString> errors;
        std::vector<std::pair<int, int>> error_pos;
        for (auto i = m_csh.CshErrors.begin(); i!=m_csh.CshErrors.end(); i++) {
            int line, col;
            ConvertPosToLineCol(i->first_pos, line, col);
            line++; col++; //XXX WTF This is needed
            error_pos.push_back(std::pair<int, int>(line, col));
            errors.push_back(Error.FormulateElement(FileLineCol(0, line, col), FileLineCol(0, line, col), true, false, i->text).text.c_str());
        }
        pApp->m_pWndOutputView->ShowCshErrors(errors, error_pos);
    }
    //create a diff in delta
    //csh_list is unfortunately not always sorted by first_pos, but close.
    std::sort(m_csh.CshList.begin(), m_csh.CshList.end(),
        [](CshEntry const & a, CshEntry const &b) {
        return a.first_pos==b.first_pos ? a.last_pos < b.last_pos : a.first_pos < b.first_pos;
    });
    std::sort(m_csh.CshErrors.begin(), m_csh.CshErrors.end(),
        [](CshError const & a, CshError const &b) {
        return a.first_pos==b.first_pos ? a.last_pos < b.last_pos : a.first_pos < b.first_pos;
    });
    csh_delta = Diff(old_csh_list, m_csh.CshList, COLOR_NORMAL);
    csh_error_delta = Diff(old_csh_error_list, m_csh.CshErrors, COLOR_NO_ERROR);

    //Do first batch of csh 
    m_csh_index = -1;
    CopyCsh();
    return ret;
}

void CCshRichEditCtrl::CopyCsh()
{
    const unsigned ONE_BATCH = 100; //# of lines
    const int PRE = 20;
    long startfrom;
    bool shall_invalidate = true; //XXX: speedup to set it false by default
    bool shall_continue = false;
    switch (m_csh_index) {
    case -3: return;
    case -2: return;
    case -1: startfrom = std::max(0, GetFirstVisibleLine()-PRE); shall_invalidate = true; break;
    default: startfrom = m_csh_index; 
    }
    long dotill = ConvertLineColToPos(startfrom + ONE_BATCH, 0);
    if (dotill < 0) 
        dotill = GetTextLength()+1;
    startfrom = ConvertLineColToPos(startfrom, 0);
    if (startfrom < 0) {
        m_csh_index = -3;
        return;
    }
    
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
	//Keep running if color syntax highlighting is not enabled, but we are forced to reset csh to normal
	if (!pApp) return;
	CHARFORMAT *const scheme = pApp->m_csh_cf[pApp->m_nCshScheme];

    SetRedraw(false);
	//long eventMask = GetEventMask();
	//SetEventMask(0);
	m_bCshUpdateInProgress = true;

	POINT scroll_pos;
	::SendMessage(m_hWnd, EM_GETSCROLLPOS, 0, (LPARAM)&scroll_pos);
	CHARRANGE cr;
	GetSel(cr);

    //if (m_csh_index == -1) { //if we start anew
    //    SetSel(0,-1); //select all
    //    SetSelectionCharFormat(scheme[COLOR_NORMAL]); //set formatting to neutral
    //}
    const DWORD effects = scheme[COLOR_NORMAL].dwEffects;
    const COLORREF color = scheme[COLOR_NORMAL].crTextColor;
    //Go backwards, since overlaid entries are added later
    for (auto i = csh_delta.rbegin(); !(i==csh_delta.rend()); i++)
        if ( (i->first_pos <= dotill) && 
             (i->last_pos >= startfrom) &&
             (i->color<COLOR_MAX) ) {
             //&&
             //( scheme[i->color].dwEffects != effects || 
             //  scheme[i->color].crTextColor != color || 
             //  (MscCshAppearanceList[pApp->m_nCshScheme][i->color].mask & COLOR_FLAG_DIFFERENT_DRAW) ) ) {
			SetSel(i->first_pos-1, i->last_pos);
			SetSelectionCharFormat(scheme[i->color]);
		}

    if (dotill >= GetTextLength()) {
        if (pApp->m_bShowCshErrors) 
            for (auto i = csh_error_delta.rbegin(); !(i==csh_error_delta.rend()); i++) {
                SetSel(i->first_pos-1, i->last_pos);
                SetSelectionCharFormat(scheme[i->color]);
            }
        m_csh_index = -3; // We are done
    } else {
        if (m_csh_index == -1) 
            m_csh_index = 0;
        else 
            m_csh_index += ONE_BATCH;
        shall_continue = true;
    }

    SetSel(cr);
	::SendMessage(m_hWnd, EM_SETSCROLLPOS, 0, (LPARAM)&scroll_pos);

	m_bCshUpdateInProgress = false;
	SetRedraw(true);
	if (shall_invalidate) {
        Invalidate();  //Invalidate only if we have shown the visible part
        UpdateWindow();
    }
	//SetEventMask(eventMask);
    m_parent->StartStopTimer(shall_continue);
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

	SetRedraw(false);
	m_bCshUpdateInProgress = true;

    //Set the partial match to the color previously proscribed in Csh
	SetSel(m_csh.partial_at_cursor_pos.first_pos-1, m_csh.partial_at_cursor_pos.last_pos); //select all
	SetSelectionCharFormat(pApp->m_csh_cf[pApp->m_nCshScheme][m_csh.partial_at_cursor_pos.color]); 

	SetSel(cr);

	m_bCshUpdateInProgress = false;
	SetRedraw(true);

	Invalidate();
}

void CCshRichEditCtrl::JumpToLine(int line, int col) {
	long index = ConvertLineColToPos(line-1, col?col-1:0);
	SetSel(index, index);
	SetFocus();
}

bool CCshRichEditCtrl::NotifyDocumentOfChange(bool onlySelChange) 
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT(pApp != NULL);
    if (!pApp) return false;
	CFrameWnd *pMainWnd = dynamic_cast<CFrameWnd*>(pApp->GetMainWnd());
	ASSERT(pMainWnd!=NULL);
	if (!pMainWnd || pMainWnd->GetActiveView() == NULL) return false;
	CMscGenDoc *pDoc = dynamic_cast<CMscGenDoc *>(pMainWnd->GetActiveView()->GetDocument());
	if (pDoc == NULL) return false;
	if (onlySelChange) pDoc->OnInternalEditorSelChange();
	else pDoc->OnInternalEditorChange();

	SetFocus();
	return true;
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

//Assumes we have called UpdateCsh just before and m_csh is up-to-date
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
    SetRedraw(false);
    SetSel(pos.first_pos, pos.last_pos);
    last_change.start = pos.first_pos;
    last_change.del = pos.last_pos-pos.first_pos;
    last_change.ins = subst.GetLength();
    ReplaceSel(subst); //this will trigger an UpdateCsh
    SetRedraw(true);
}

/////////////////////////////////////////////////////////////////////////////
// COutputBar

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
    m_timer = NULL;
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
    m_totalLenAtPreviousTextChange = 0;
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
            pApp->m_pWndEditor->m_ctrlEditor.SetRedraw(false);
            pApp->m_pWndEditor->m_ctrlEditor.SetSel(0,-1);
            pApp->m_pWndEditor->m_ctrlEditor.SetRedraw(true);
            pApp->m_pWndEditor->m_ctrlEditor.ReplaceSel(pDoc->m_itrEditing->GetText());
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


//Timer for csh.
//This cannot be in CCshRichEditCtrl, for in that case mouse wheel scrolling
//does not work mysteriously.
void CEditorBar::StartStopTimer(bool shall_continue)
{
    const unsigned TIME_DLEAY = 30; //milliseconds
    if (shall_continue == (m_timer!=NULL))
        return;
    else if (shall_continue)
        m_timer = SetTimer(1, TIME_DLEAY, NULL);
    else {
        KillTimer(m_timer);
        m_timer = NULL;
    }

}
void CEditorBar::OnTimer(UINT_PTR nEventID)
{
    if (nEventID == m_timer)
        m_ctrlEditor.CopyCsh();
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
	if (m_ctrlEditor.IsCshUpdateInProgress() || m_bSuspendNotifications) return TRUE;

	const bool at_same_hint = m_ctrlEditor.UpdateCsh();
    int len = m_ctrlEditor.GetTextLength();
    //Update hints if we are in hint mode
    if (m_ctrlEditor.InHintMode()) {
        //Kill the user requested nature
        if (!at_same_hint)
            m_ctrlEditor.CancelUserSelected();
        m_ctrlEditor.StartHintMode(false); //false == do not change m_bTillCursorOnly
    } else {
        long s, e;
        m_ctrlEditor.GetSel(s,e);
        //Enter hint mode only if we typed a character
        if (m_totalLenAtPreviousTextChange < len && s==e) {
            bool till_cursor_only = false;
            if (s<=1) till_cursor_only = true;
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
    m_totalLenAtPreviousTextChange = len;
	m_ctrlEditor.NotifyDocumentOfChange();
	return TRUE;
}

void CEditorBar::OnSelChange(NMHDR * /*pNotifyStruct*/, LRESULT *result) 
{
    if (m_ctrlEditor.IsCshUpdateInProgress() || m_bSuspendNotifications) {
        m_totalLenAtPreviousSelChange = m_ctrlEditor.GetTextLength();
        return;
    }
    //if we have just moved, not inserted or deleted cancel partial match and hints
    //(otherwise we will get an EN_CHANGE right after this and we redo
    //csh and hints all the same. This is to prevent hint window flicker.)
    if (m_totalLenAtPreviousSelChange == m_ctrlEditor.GetTextLength()) {
        m_ctrlEditor.CancelPartialMatch();
        m_ctrlEditor.NotifyDocumentOfChange(true);
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
		m_bSuspendNotifications = true;
		m_ctrlEditor.HideSelection(TRUE, FALSE);
		do {
			m_ctrlEditor.ReplaceSel(pFindReplace->GetReplaceString());
		} while (FindText(pFindReplace->GetFindString(), pFindReplace->MatchCase(), pFindReplace->MatchWholeWord()));
		m_ctrlEditor.HideSelection(FALSE, FALSE);
		m_bSuspendNotifications = false;
		m_ctrlEditor.UpdateCsh(); 
		m_ctrlEditor.NotifyDocumentOfChange();
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
