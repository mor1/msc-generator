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
/** @file parse_tools.cpp Utilities for parsing.
 * @ingroup libmscgen_files
 */

//We do not compile csh, so define the below macro as a no-op
#define CHAR_IF_CSH(A) A
#include "msc.h" //required by language.h to define YYSTYPE
#define YYMSC_RESULT_TYPE Msc
#define RESULT msc
#include "language.h"
//parse_tools.h requires YYLTYPE, this is why we must include language.h
//plus YYSTYPE is also required by language2.h
#include "parse_tools.h"
#include "language_misc.h"
#define YY_HEADER_EXPORT_START_CONDITIONS
#include "language2.h"
#include "attribute.h"  //for case insensitive compares

//define isatty - only once, so we put it into this file
#ifndef HAVE_UNISTD_H
int isatty (int) {return 0;}
#endif



/** Move `loc` to the beginning of the next line*/
void msc_jump_line(YYLTYPE *loc)
{
    loc->last_line = loc->first_line+1;
    loc->last_column=1; //yacc column numbering starts at 1
};

/** In-place removal of whitespace.
 * Remove heading and trailing whitespace, by
 * returning a new head of string and adding a '0' to the end of it.*/
char *msc_remove_head_tail_whitespace(char *s)
{
    size_t a=0;
    char *r;
    while (s[a]==' ' || s[a]=='\t') a++;
    r = s+a;
    a=strlen(r);
    while (a>0 && (r[a-1]==' ' || r[a-1]=='\t')) a--;
    r[a] = '\0';
    return r;
}

/** Preprocess a colon-initiated label.
 * We do all the following:
 * - Remove heading and trailing whitespace.
 * - Replace any internal CR or CRLF (and surrounding whitespaces) to "\n".
 * - Remove comments between # and lineend, except if # is preceeded by an odd
 *   number of backslashes.
 * - Update `loc` to point to the end of the token
 * - Insert \0x2(file,line,col) escapes where needed we changed the length of the
 *   preceeding string, so that if we generate an error to any escapes thereafter
 *   those will have the right location in the input file.
 *
 * The function copies the result to new memory and the caller shall free().*/
char* msc_process_colon_string(const char *s, YYLTYPE *loc, unsigned file_no)
{
    std::string ret;
    int old_pos = 1; //actually s begins with the colon, we skip that
    loc->last_line   = loc->first_line;
    loc->last_column = loc->first_column+1;

    while (1) {
        //the current line begins at old_pos
        int end_line   = old_pos;
        int start_line = old_pos;
        //find the end of the line
        while (s[end_line]!=0 && s[end_line]!=10 && s[end_line]!=13)
            end_line++;
        //store the ending char to later see how to proceed
        char ending = s[end_line];
        //skip over the heading whitespace at the beginning of the line
        while (s[start_line]==' ' || s[start_line]=='\t')
            start_line++;
        //find the last non-whitespace in the line
        int term_line = end_line-1;
        //term_line can be smaller than start_line here if line is empty
        while (term_line>=start_line && (s[term_line]==' ' || s[term_line]=='\t'))
            term_line--;
        //Generate a \l(file,line,col) escape and append
        FileLineCol pos(file_no, loc->last_line,
                      loc->last_column + (start_line-old_pos));
        ret += pos.Print();
        //now append the line (without the whitespaces)
        //but remove anything after a potential #
        bool wasComment = false;
        bool emptyLine = true;
        while (start_line<=term_line) {
            //count number of consecutive \s
            unsigned num_of_backsp = 0;
            while(start_line<=term_line && s[start_line] != '#') {
                if (s[start_line] == '\\') num_of_backsp++;
                else num_of_backsp = 0;
                ret += s[start_line++];
                emptyLine = false;
            }
            //if we hit a # leave rest of line only if not preceeded by even number of \s
            if (s[start_line] == '#') {
                if (num_of_backsp%2) {
                    ret += s[start_line++]; //step over escaped #
                    emptyLine = false;
                } else {
                    wasComment = true;
                    break;
                }
            }
        }
        //We may have copied spaces before the comment, we skip those
        while (ret.length()>0 && (ret[ret.length()-1]==' ' || ret[ret.length()-1]=='\t'))
            ret.erase(ret.length()-1);
        //if ending was a null we are done with processing all lines
        if (!ending) {
            loc->last_column += (end_line - old_pos) -1;
            break;
        }
        //append ESCAPE_CHAR_SOFT_NEWLINE escape for msc-generator,
        //but only if not an empty first line
        //append "\" + ESCAPE_CHAR_SOFT_NEWLINE if line ended with odd number of \s
        if (!emptyLine || loc->first_line != loc->last_line) {
            //add a space for empty lines, if line did not contain a comment
            if (emptyLine && !wasComment )
                ret += ' ';
            //test for how many \s we have at the end of the line
            int pp = (int)ret.length()-1;
            while (pp>=0 && ret[pp]=='\\') pp--;
            //if odd, we insert an extra '\' to keep lines ending with \s
            if ((ret.length()-pp)%2==0) ret += '\\';
            ret += "\\" ESCAPE_STRING_SOFT_NEWLINE;
        }
        //Check for a two character CRLF, skip over the LF, too
        if (ending == 13 && s[end_line+1] == 10) end_line++;
        old_pos = end_line+1;

        //Now advance loc
        loc->last_line++;
        loc->last_column = 1;
    }
    return strdup(ret.c_str());
}

/** Preprocess multiline quoted strings.
 * We do all the following:
 * - skip heading and trailing quotation mark and whitespace inside
 * - Replace any internal CR or CRLF (and surrounding whitespaces) to "\n".
 * - Update `loc` to point to the end of the token
 * - Insert \0x2(file,line,col) escapes where needed we changed the length of the
 *   preceeding string, so that if we generate an error to any escapes thereafter
 *   those will have the right location in the input file.
 *
 * The function copies the result to new memory and the caller shall free().*/
char* msc_process_multiline_qstring(const char *s, YYLTYPE *loc, unsigned file_no)
{
    std::string ret;
    int old_pos = 1; //actually s begins with the quotation mark, we skip that
    loc->last_line = loc->first_line;
    loc->last_column = loc->first_column+1;

    while (1) {
        //the current line begins at old_pos
        int end_line = old_pos;
        int start_line = old_pos;
        //find the end of the line
        while (s[end_line]!=0 && s[end_line]!=10 && s[end_line]!=13)
            end_line++;
        //store the ending char to later see how to proceed
        char ending = s[end_line];
        //skip over the heading whitespace at the beginning of the line
        while (s[start_line]==' ' || s[start_line]=='\t')
            start_line++;
        //find the last non-whitespace in the line
        int term_line = end_line-1;
        //if we are at the very end, ignore trailing quotation mark
        if (ending==0) term_line--;
        //term_line can be smaller than start_line here if line is empty
        while (term_line>=start_line && (s[term_line]==' ' || s[term_line]=='\t'))
            term_line--;
        //Generate a \l(file,line,col) escape and append
        FileLineCol pos(file_no, loc->last_line,
            loc->last_column + (start_line-old_pos));
        ret += pos.Print();
        //now append the line (without the whitespaces)
        ret.append(s+start_line, s+term_line+1);
        //if ending was a null we are done with processing all lines
        if (!ending) {
            loc->last_column += (end_line - old_pos) -1;
            break;
        }
        //append ESCAPE_CHAR_SOFT_NEWLINE escape for msc-generator,
        //but only if not an empty first line
        //append "\" + ESCAPE_CHAR_SOFT_NEWLINE if line ended with odd number of \s
        if (start_line<=term_line || loc->first_line != loc->last_line) {
            //add a space for empty lines, if line did not contain a comment
            if (start_line>term_line)
                ret += ' ';
            //test for how many \s we have at the end of the line
            int pp = (int)ret.length()-1;
            while (pp>=0 && ret[pp]=='\\') pp--;
            //if odd, we insert an extra '\' to keep lines ending with \s
            if ((ret.length()-pp)%2==0) ret += '\\';
            ret += "\\" ESCAPE_STRING_SOFT_NEWLINE;
        }
        //Check for a two character CRLF, skip over the LF, too
        if (ending == 13 && s[end_line+1] == 10) end_line++;
        old_pos = end_line+1;

        //Now advance loc
        loc->last_line++;
        loc->last_column = 1;
    }
    return strdup(ret.c_str());
}

/** Converts 'emphasis' (a deprecated style name) to 'box' and emits warning if doing so.
 * We return 'box' or 'emptybox' if the input style name was 'emphasis' or
 * 'emptyemphasis' and generate a warning. Else we return `style`.*/
string ConvertEmphasisToBox(const string &style, const YYLTYPE *loc, Msc &msc)
{
    if (style != "emphasis" && style != "emptyemphasis") 
        return style;
    const char *newname = style == "emphasis" ? "box" : "emptybox";
    msc.Error.Warning(MSC_POS(*loc).start,
        "Stylename '" + style + "' is deprecated, using " + newname + " instead.");
    return newname;
}

/** The error handling function required by yacc.
 * The TOK_XXX names are substituted for more
 *  understandable values that make more sense to the user and
 * the error is added to `msc`.
 * @param [in] loc The location of the error.
 * @param msc The chart to add the error to.
 * @param yyscanner The state of the parser
 * @param [in] str The message we need to beautify and emit into `msc`.*/
void yyerror(YYLTYPE*loc, Msc &msc, void *yyscanner, const char *str)
{
    static const std::pair<string, string> tokens[] = {
    std::pair<string,string>("unexpected", "Unexpected"),
    std::pair<string,string>("TOK_REL_SOLID_TO", "'->'"),
    std::pair<string,string>("TOK_REL_SOLID_FROM", "'<-'"),
    std::pair<string,string>("TOK_REL_SOLID_BIDIR", "'<->'"),
    std::pair<string,string>("TOK_REL_DOUBLE_TO", "'=>'"),
    std::pair<string,string>("TOK_REL_DOUBLE_FROM", "'<='"),
    std::pair<string,string>("TOK_REL_DOUBLE_BIDIR", "'<=>'"),
    std::pair<string,string>("TOK_REL_DASHED_TO", "'>>'"),
    std::pair<string,string>("TOK_REL_DASHED_FROM", "'<<'"),
    std::pair<string,string>("TOK_REL_DASHED_BIDIR", "'<<>>'"),
    std::pair<string,string>("TOK_REL_DOTTED_TO", "'>'"),
    std::pair<string,string>("TOK_REL_DOTTED_FROM", "'<'"),
    std::pair<string,string>("TOK_REL_DOTTED_BIDIR", "'<>'"),
    std::pair<string,string>("TOK_EMPH", "'..', '--', '=='"),
    std::pair<string,string>("TOK_SPECIAL_ARC", "'...', '---'"),
    std::pair<string,string>("syntax error, ", ""),
    std::pair<string,string>(", expecting $end", ""),
    std::pair<string,string>("$end", "end of input"),
    std::pair<string,string>("TOK_OCBRACKET", "'{'"),
    std::pair<string,string>("TOK_CCBRACKET", "'}'"),
    std::pair<string,string>("TOK_OSBRACKET", "'['"),
    std::pair<string,string>("TOK_CSBRACKET", "']'"),
    std::pair<string,string>("TOK_EQUAL", "'='"),
    std::pair<string,string>("TOK_DASH", "'-'"),
    std::pair<string,string>("TOK_PLUS", "'+'"),
    std::pair<string,string>("TOK_COMMA", "','"),
    std::pair<string,string>("TOK_SEMICOLON", "';'"),
    std::pair<string,string>("TOK_MSC", "'msc'"),
    std::pair<string,string>("TOK_BOOLEAN", "'yes', 'no'"),
    std::pair<string,string>("TOK_COMMAND_HEADING", "'heading'"),
    std::pair<string,string>("TOK_COMMAND_NUDGE", "'nudge'"),
    std::pair<string,string>("TOK_COMMAND_NEWPAGE", "'newpage'"),
    std::pair<string,string>("TOK_COMMAND_DEFCOLOR", "'defcolor'"),
    std::pair<string,string>("TOK_COMMAND_DEFSTYLE", "'defstyle'"),
    std::pair<string,string>("TOK_COMMAND_DEFDESIGN", "'defdesign'"),
    std::pair<string,string>("TOK_COMMAND_BIG", "'block'"),
    std::pair<string,string>("TOK_COMMAND_PIPE", "'pipe'"),
    std::pair<string,string>("TOK_COMMAND_MARK", "'mark'"),
    std::pair<string,string>("TOK_COMMAND_PARALLEL", "'parallel'"),
    std::pair<string,string>("TOK_VERTICAL", "'vertical'"),
    std::pair<string,string>("TOK_AT", "'at'"),
    std::pair<string,string>("TOK_AT_POS", "'left', 'right' or 'center'"),
    std::pair<string,string>("TOK_SHOW", "'show'"),
    std::pair<string,string>("TOK_HIDE", "'hide'"),
    std::pair<string,string>("TOK_BYE", "'bye'"),
    std::pair<string,string>("TOK_COMMAND_VSPACE", "'vspace'"),
    std::pair<string,string>("TOK_COMMAND_HSPACE", "'hspace'"),
    std::pair<string,string>("TOK_COMMAND_SYMBOL", "'symbol'"),
    std::pair<string,string>("TOK_COMMAND_NOTE", "'note'"),
    std::pair<string,string>("TOK_COMMAND_COMMENT", "'comment'"),
    std::pair<string,string>("TOK_COLON_STRING", "':'"),  //just say colon to the user
    std::pair<string,string>("TOK_COLON_QUOTED_STRING", "':'"),  //just say colon to the user
    std::pair<string,string>("TOK_NUMBER", "number"),
    std::pair<string,string>("TOK_STRING", "string"),
    std::pair<string,string>("TOK_STYLE_NAME", "style name"),
    std::pair<string,string>("TOK_QSTRING", "quoted string")};

    #define tokArrayLen (sizeof(tokens) / sizeof(std::pair<string,string>))
    string msg(str);

    string once_msg;

    //replace tokens in string. We assume
    //-each toke appears only once
    //-replaced strings will not be mistaken for a a token
    for (unsigned i=0; i<tokArrayLen; i++) {
        string::size_type pos = 0;
        //Search for the current token
        pos = msg.find(tokens[i].first, pos);
        //continue if not found
        if (pos == string::npos) continue;
        //if msg continues with an uppercase letter or _ we are not matching
        char next = msg.c_str()[pos+tokens[i].first.length()];
        if ((next<='Z' && next>='A') || next == '_') continue;

        //Ok, token found, create substitution
        string ins = tokens[i].second;
        //special comment for unexpected symbols
        //special handling for numbers and strings
        if (i>tokArrayLen-5) {
            string::size_type exppos = msg.find("expecting");
            //If we replace what was unexpected, use actual token text
            if (pos < exppos) {
                if (i==tokArrayLen-1)
                    ins += ": " + string(yyget_text(yyscanner));
                else
                    ins += ": '" + string(yyget_text(yyscanner)) + "'";
                if (i==tokArrayLen-2) {
                    string hint(yyget_text(yyscanner));
                    string::size_type pos2 = hint.find_first_not_of("abcdefghijklmnopqrstuvwxyz");
                    hint.insert(pos2," ");
                    once_msg = "Try splitting it with a space: '"+hint+"'.";
                }
            } else {
                ins = "a <" + ins + ">";
            }
        }
        //replace token
        msg.replace(pos, tokens[i].first.length(), ins);
    }
    string::size_type pos = msg.rfind("', '");
    if (pos != string::npos) {
        msg.erase(pos+1, 1);
        msg.insert(pos+1, " or");
    }
    msg.append(".");
    msc.Error.Error(MSC_POS(*loc).start, msg, once_msg);
};
