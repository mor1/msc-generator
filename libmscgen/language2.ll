%option reentrant noyywrap nounput
%option bison-bridge bison-locations

%{
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
    along with Msc-generator.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef COLOR_SYNTAX_HIGHLIGHT
#define C_S_H (1)
#define C_S_H_IS_COMPILED
#else
#define C_S_H (0)
#endif

#include <stdio.h>
#include <string.h>
#include <iostream>

#ifndef HAVE_UNISTD_H
#define YY_NO_UNISTD_H
extern int isatty(int);
#endif



#ifdef C_S_H_IS_COMPILED
#include "csh.h"
#define YYMSC_RESULT_TYPE Csh
#define RESULT csh

//If we scan for color syntax highlight use this location
//yyerror is defined by bison, the other is defined for flex
#define YYLTYPE_IS_DECLARED
#define YYLTYPE CshPos
#define CHAR_IF_CSH(A) char

#include "colorsyntax.h"
#include "parserhelper.h"
#include "arcs.h" //MSC_XXX defs
#else
#include "msc.h"
#define YYMSC_RESULT_TYPE Msc
#define RESULT msc

#define CHAR_IF_CSH(A) A
#include "language.h"
#include "parserhelper.h"

//define isatty - only once, so we put inside the #ifdef
#ifndef HAVE_UNISTD_H
int isatty (int) {return 0;}
#endif


#endif

#define YY_INPUT(buffer, res, max_size)             \
do {                                                \
    parse_parm *pp = yyget_extra(yyscanner);        \
    if (pp->pos >= pp->length)                      \
        res = YY_NULL;                              \
    else                                            \
    {                                               \
        res = pp->length - pp->pos;                 \
        res > (int)max_size ? res = max_size : 0;   \
        memcpy(buffer, pp->buf + pp->pos, res);     \
        pp->pos += res;                             \
    }                                               \
} while (0)

#ifdef C_S_H_IS_COMPILED
#define YY_USER_ACTION do {                     \
    yylloc->first_pos = yylloc->last_pos+1;     \
    yylloc->last_pos = yylloc->last_pos+yyleng; \
    } while(0);

#define YYRHSLOC(Rhs, K) ((Rhs)[K])

#define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do								\
      if (YYID (N))                                             \
	{							\
	  (Current).first_pos = YYRHSLOC (Rhs, 1).first_pos;	\
	  (Current).last_pos  = YYRHSLOC (Rhs, N).last_pos;	\
	}							\
      else							\
	{							\
        (Current).first_pos = (Current).last_pos   =		\
	    YYRHSLOC (Rhs, 0).last_pos;				\
	}							\
    while (YYID (0))

#define JUMP_LINE

#else

#define YY_USER_ACTION do {                              \
    yylloc->first_line = yylloc->last_line;              \
    yylloc->first_column = yylloc->last_column+1;        \
    yylloc->last_column = yylloc->first_column+yyleng-1; \
    } while(0);

#define JUMP_LINE do {\
    yylineno++;                                \
    yylloc->last_line = yylloc->first_line+1;  \
    yylloc->last_column=0;                     \
    } while(0)

/* in-place removal of whitespace. Returns new head */
char *msc_remove_head_tail_whitespace(char *s)
{
    int a=0;
    char *r;
    while (s[a]==' ' || s[a]=='\t') a++;
    r = s+a;
    a=strlen(r);
    while (a>0 && (r[a-1]==' ' || r[a-1]=='\t')) a--;
    r[a] = '\0';
    return r;
}

/* remove heading and trailing whitespace from a string
** remove any internal CR or CRLF (and surrounding whitespaces) & replace to \n
** Remove comments between # and lineend, except if # is preceeded by even number of \s
** (copies to new memory) */
char* msc_process_colon_string(const char *s, YYLTYPE *loc)
{
    char *ret = (char*)malloc(strlen(s)*2+2); //max characters needed
    int old_pos = 0;
    int new_pos = 0;
    while (1) {
        //the next line begins at old_pos
        int end_line=old_pos;
        //find the end of the line
        while (s[end_line]!=0 && s[end_line]!=10 && s[end_line]!=13) end_line++;
        //store the ending char to later see how to proceed
        char ending = s[end_line];
        //skip over the heading whitespace at the beginning of the line
        int start_line = old_pos;
        while (s[start_line]==' ' || s[start_line]=='\t') start_line++;
        //find the last non-whitespace in the line
        int a = end_line-1; //a can be smaller than start_line here if line is empty
        while (a>=start_line && (s[a]==' ' || s[a]=='\t')) a--;
        //now append the line (without the whitespaces)
        //but remove anything after a potential #
        bool wasComment = false;
        bool emptyLine = true;
        while (start_line<=a) {
            //count number of consecutive \s
            unsigned num_of_backsp = 0;
            while(start_line<=a && s[start_line] != '#') {
                if (s[start_line] == '\\') num_of_backsp++;
                else num_of_backsp = 0;
                ret[new_pos++] = s[start_line++];
                emptyLine = false;
            }
            //if we hit a # leave rest of line only if not preceeded by even number of \s
            if (s[start_line] == '#') {
                if (num_of_backsp%2) {
                    ret[new_pos++] = s[start_line++]; //step over escaped #
                    emptyLine = false;
                } else {
                    wasComment = true;
                    break;
                }
            }
        }
        //We may have copied spaces before the comment, we skip those
        while (new_pos>0 && (ret[new_pos-1]==' ' || ret[new_pos-1]=='\t')) new_pos--;
        //if ending was a null we are done with processing all lines
        if (!ending) break;
        //append "\n" escape for msc-generator, but only if not an empty first line
        //append "\\n" if line ended with odd number of \s
        if (new_pos) {
            //add a space for empty lines, if line did not contain a comment
            if (emptyLine && !wasComment )
                ret[new_pos++] = ' ';
            //test for how many \s we have
            int pp = new_pos-1;
            while (pp>=0 && ret[pp]=='\\') pp--;
            //if odd, we insert an extra '\' to keep lines ending with \s
            if ((new_pos-pp)%2==0) ret[new_pos++] = '\\';
            ret[new_pos++] = '\\';
            ret[new_pos++] = 'n';
        }
        //Check for a two character CRLF, skip over the LF, too
        if (ending == 13 && s[end_line+1] == 10) end_line++;
        old_pos = end_line+1;
    }
    ret[new_pos] = 0;

    //OK, now adjust yylloc
    old_pos = 0;
    new_pos = 0; //start of line
    while (1) {
        while (s[old_pos]!=0 && s[old_pos]!=10 && s[old_pos]!=13) old_pos++;
        if (s[old_pos]==0) {
            if (loc->last_column==0) loc->last_column = old_pos-new_pos;
            break;
        }
        if (s[old_pos] == 13 && s[old_pos+1] == 10) old_pos++;
        old_pos++;
        new_pos = old_pos;
        loc->last_line++;
        loc->last_column = 0;
    }
    return ret;
}

#endif /* C_S_H_IS_COMPILED */

%}

%%

 /* Newline characters in all forms accepted */
\x0d\x0a     JUMP_LINE;
\x0d         JUMP_LINE;
\x0a         JUMP_LINE;

 /* # starts a comment last until end of line */
#[^\x0d\x0a]* %{
  #ifdef C_S_H_IS_COMPILED
    yyget_extra(yyscanner)->csh->AddCSH(*yylloc, COLOR_COMMENT);
  #endif
%}


 /* This is a colon-quoted string, finished by a quotation mark
 ** : "<string>"
 ** <string> can contain escaped quotation marks, but no line breaks
 */
\:[ \t]*\"([^\"\x0d\x0a]*(\\\")*)*\" %{
  #ifdef C_S_H_IS_COMPILED
    yylval_param->str = strdup(yytext);
  #else
    /* after whitespaces we are guaranteed to have a tailing and heading quot */
    yylval_param->str = strdup(msc_remove_head_tail_whitespace(yytext+1)+1);
    yylval_param->str[strlen(yylval_param->str) - 1] = '\0';
  #endif
    return TOK_COLON_QUOTED_STRING;
%}

 /* This is a colon-quoted string, finished by a newline (trailing context)
 ** : "<string>$
 ** <string> can contain escaped quotation marks, but no line breaks
 */
\:[ \t]*\"([^\"\x0d\x0a]*(\\\")*)*/[\x0d\x0a] %{
  #ifdef C_S_H_IS_COMPILED
    yylval_param->str = strdup(yytext);
  #else
    {
    /* after whitespaces we are guaranteed to have a heading quot */
    const char *s = msc_remove_head_tail_whitespace(yytext+1);
    // s now points to heading quotation mark
    yylval_param->str = strdup(s+1);
    file_line pos(yyget_extra(yyscanner)->msc->current_file,
                 yylloc->first_line, yylloc->first_column + (s - yytext));
    yyget_extra(yyscanner)->msc->Error.Error(pos,
         "This opening quotation mark misses its closing pair. "
         "Assuming string termination at line-end.",
         "Quoted strings cannot have line breaks. Use \'\\n\' to insert a line break.");
    }
  #endif
    return TOK_COLON_QUOTED_STRING;
%}

 /* This is a non quoted colon-string
 ** : <string>
 ** terminated by any of: quot mark, backslash [ { or ;
 ** Honors escaping of the above via a backslash
 */
\:([^\"\;\[\{\\]*(\\.)*)*  %{
  #ifdef C_S_H_IS_COMPILED
    yylval_param->str = strdup(yytext);
  #else
    yylval_param->str = msc_process_colon_string(yytext+1, yylloc);
  #endif
    return TOK_COLON_STRING;
%}

 /* A simple quoted string. */
\"[^\"\x0d\x0a]*\" %{
  #ifdef C_S_H_IS_COMPILED
    yylval_param->str = strdup(yytext);
  #else
    yylval_param->str = strdup(yytext+1);
    yylval_param->str[strlen(yylval_param->str) - 1] = '\0';
  #endif
    return TOK_QSTRING;
%}

 /* A simple quoted string, missing a closing quotation mark */
\"[^\"\x0d\x0a]*/\x0d\x0a %{
  #ifdef C_S_H_IS_COMPILED
    yylval_param->str = strdup(yytext);
  #else
    {
    yylval_param->str = strdup(yytext+1);
    file_line pos(yyget_extra(yyscanner)->msc->current_file,
                 yylloc->first_line, yylloc->first_column);
    yyget_extra(yyscanner)->msc->Error.Error(pos,
         "This opening quotation mark misses its closing pair. "
         "Assuming string termination at line-end.",
         "Quoted strings cannot have line breaks. Use \'\\n\' to insert a line break.");
    }
  #endif
    return TOK_QSTRING;
%}

 /* These keywords are case insensitive */
(?i:msc)       yylval_param->str = strdup(yytext); return TOK_MSC;
(?i:heading)   yylval_param->str = strdup(yytext); return TOK_COMMAND_HEADING;
(?i:nudge)     yylval_param->str = strdup(yytext); return TOK_COMMAND_NUDGE;
(?i:defcolor)  yylval_param->str = strdup(yytext); return TOK_COMMAND_DEFCOLOR;
(?i:defstyle)  yylval_param->str = strdup(yytext); return TOK_COMMAND_DEFSTYLE;
(?i:defdesign) yylval_param->str = strdup(yytext); return TOK_COMMAND_DEFDESIGN;
(?i:newpage)   yylval_param->str = strdup(yytext); return TOK_COMMAND_NEWPAGE;
(?i:block)     yylval_param->str = strdup(yytext); return TOK_COMMAND_BIG;
(?i:pipe)      yylval_param->str = strdup(yytext); return TOK_COMMAND_PIPE;
(?i:mark)      yylval_param->str = strdup(yytext); return TOK_COMMAND_MARK;
(?i:parallel)  yylval_param->str = strdup(yytext); return TOK_COMMAND_PARALLEL;
(?i:vertical)  yylval_param->str = strdup(yytext); return TOK_VERTICAL;
(?i:at)        yylval_param->str = strdup(yytext); return TOK_AT;
(?i:no)        yylval_param->str = strdup(yytext); return TOK_BOOLEAN;
(?i:yes)       yylval_param->str = strdup(yytext); return TOK_BOOLEAN;

\.\.\.   yylval_param->arctype=MSC_ARC_DISCO;       return TOK_SPECIAL_ARC;      // ...
---      yylval_param->arctype=MSC_ARC_DIVIDER;     return TOK_SPECIAL_ARC;      // ---
-\>      yylval_param->arctype=MSC_ARC_SOLID;       return TOK_REL_SOLID_TO;     // ->
\<-      yylval_param->arctype=MSC_ARC_SOLID;       return TOK_REL_SOLID_FROM;   // <-
\<-\>    yylval_param->arctype=MSC_ARC_SOLID_BIDIR; return TOK_REL_SOLID_BIDIR;  // <->
=\>      yylval_param->arctype=MSC_ARC_DOUBLE;      return TOK_REL_DOUBLE_TO;    // =>
\<=      yylval_param->arctype=MSC_ARC_DOUBLE;      return TOK_REL_DOUBLE_FROM;  // <=
\<=\>    yylval_param->arctype=MSC_ARC_DOUBLE_BIDIR;return TOK_REL_DOUBLE_BIDIR; // <=>
\>\>     yylval_param->arctype=MSC_ARC_DASHED;      return TOK_REL_DASHED_TO;    // >>
\<\<     yylval_param->arctype=MSC_ARC_DASHED;      return TOK_REL_DASHED_FROM;  // <<
\<\<\>\> yylval_param->arctype=MSC_ARC_DASHED_BIDIR;return TOK_REL_DASHED_BIDIR; // <<>>
\>       yylval_param->arctype=MSC_ARC_DOTTED;      return TOK_REL_DOTTED_TO;    // >
\<       yylval_param->arctype=MSC_ARC_DOTTED;      return TOK_REL_DOTTED_FROM;  // <
\<\>     yylval_param->arctype=MSC_ARC_DOTTED_BIDIR;return TOK_REL_DOTTED_BIDIR; // <>
--       yylval_param->arctype=MSC_EMPH_SOLID;      return TOK_EMPH;             // --
\+\+     yylval_param->arctype=MSC_EMPH_DASHED;     return TOK_EMPH;             // ++
\.\.     yylval_param->arctype=MSC_EMPH_DOTTED;     return TOK_EMPH;             // ..
==       yylval_param->arctype=MSC_EMPH_DOUBLE;     return TOK_EMPH;             // ==
-        return TOK_DASH;
=        return TOK_EQUAL;
,        return TOK_COMMA;
\;       return TOK_SEMICOLON;
\{       return TOK_OCBRACKET;
\}       return TOK_CCBRACKET;
\[       return TOK_OSBRACKET;
\]       return TOK_CSBRACKET;

pipe--        yylval_param->str=strdup(yytext); return TOK_STYLE_NAME;
pipe\+\+      yylval_param->str=strdup(yytext); return TOK_STYLE_NAME;
pipe==        yylval_param->str=strdup(yytext); return TOK_STYLE_NAME;
pipe\.\.      yylval_param->str=strdup(yytext); return TOK_STYLE_NAME;
block-\>      yylval_param->str=strdup(yytext); return TOK_STYLE_NAME;
block\>       yylval_param->str=strdup(yytext); return TOK_STYLE_NAME;
block\>\>     yylval_param->str=strdup(yytext); return TOK_STYLE_NAME;
block=\>      yylval_param->str=strdup(yytext); return TOK_STYLE_NAME;
vertical--    yylval_param->str=strdup(yytext); return TOK_STYLE_NAME;
vertical\+\+  yylval_param->str=strdup(yytext); return TOK_STYLE_NAME;
vertical==    yylval_param->str=strdup(yytext); return TOK_STYLE_NAME;
vertical\.\.  yylval_param->str=strdup(yytext); return TOK_STYLE_NAME;
vertical-\>   yylval_param->str=strdup(yytext); return TOK_STYLE_NAME;
vertical\>\>  yylval_param->str=strdup(yytext); return TOK_STYLE_NAME;
vertical\>    yylval_param->str=strdup(yytext); return TOK_STYLE_NAME;
vertical=\>   yylval_param->str=strdup(yytext); return TOK_STYLE_NAME;

[+\-]?[0-9]+\.?[0-9]*                   yylval_param->str = strdup(yytext); return TOK_NUMBER;
[A-Za-z_]([A-Za-z0-9_\.]?[A-Za-z0-9_])* yylval_param->str = strdup(yytext); return TOK_STRING;

[ \t]+    /* ignore whitespace */;
%%

/* END OF FILE */
