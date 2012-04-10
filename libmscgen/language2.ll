%option reentrant noyywrap nounput
%option bison-bridge bison-locations

%{
/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011,2012 Zoltan Turanyi
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
#include "language_misc.h"
#include "commands.h" //MSC_* defs and CommandNote
#else
#include "msc.h"
#define YYMSC_RESULT_TYPE Msc
#define RESULT msc

#define CHAR_IF_CSH(A) A
#include "language.h"
#include "language_misc.h"
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

#else

#define YY_USER_ACTION do {                              \
    yylloc->first_line = yylloc->last_line;              \
    yylloc->first_column = yylloc->last_column+1;        \
    yylloc->last_column = yylloc->first_column+yyleng-1; \
    } while(0);

#include "parse_tools.h"

#endif /* C_S_H_IS_COMPILED */

%}

%%

 /* Newline characters in all forms accepted */
\x0d\x0a     %{
  #ifndef C_S_H_IS_COMPILED
    msc_jump_line(yylloc);
  #endif
%}

\x0a         %{
  #ifndef C_S_H_IS_COMPILED
    msc_jump_line(yylloc);
  #endif
%}

\x0d         %{
  #ifndef C_S_H_IS_COMPILED
    msc_jump_line(yylloc);
  #endif
%}

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
    {
    /* after whitespaces we are guaranteed to have a tailing and heading quot */
    char *s = msc_remove_head_tail_whitespace(yytext+1);
    /* s now points to the heading quotation marks.
    ** Now get rid of both quotation marks */
    std::string str(s+1);
    str.erase(str.length()-1);
    /* Calculate the position of the string and prepend a location escape */
    file_line pos(yyget_extra(yyscanner)->msc->current_file,
                 yylloc->first_line, yylloc->first_column + unsigned(s+1 - yytext));
    yylval_param->str = strdup((pos.Print() + str).c_str());
    }
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
    file_line pos(yyget_extra(yyscanner)->msc->current_file,
                 yylloc->first_line, yylloc->first_column + unsigned(s - yytext));
    yyget_extra(yyscanner)->msc->Error.Error(pos,
         "This opening quotation mark misses its closing pair. "
         "Assuming string termination at line-end.",
         "Quoted strings cannot have line breaks. Use \'\\n\' to insert a line break.");
    /* Advance pos beyond the leading quotation mark */
    pos.col++;
    yylval_param->str = strdup((pos.Print() + (s+1)).c_str());
    }
  #endif
    return TOK_COLON_QUOTED_STRING;
%}

 /* This is a non quoted colon-string
 ** : <string>
 ** terminated by any of: quot mark, backslash [ { or ;
 ** Honors escaping of the above via a backslash
 ** Can contain quotation marks (escaped or unescaped), but can not start with it
 */
\:[ \t]*([^\"\;\[\{\\]|(\\.))([^\;\[\{\\]*(\\.)*)*  %{
  #ifdef C_S_H_IS_COMPILED
    yylval_param->str = strdup(yytext);
  #else
    yylval_param->str = msc_process_colon_string(yytext, yylloc,
                        yyget_extra(yyscanner)->msc->current_file);
  #endif
    return TOK_COLON_STRING;
%}

 /* A simple quoted string. */
\"[^\"\x0d\x0a]*\" %{
    yylval_param->str = strdup(yytext+1);
    yylval_param->str[strlen(yylval_param->str) - 1] = '\0';
    return TOK_QSTRING;
%}

 /* A simple quoted string, missing a closing quotation mark */
\"[^\"\x0d\x0a]*/\x0d\x0a %{
  #ifdef C_S_H_IS_COMPILED
    yylval_param->str = strdup(yytext+1);
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
(?i:left)      yylval_param->str = strdup(yytext); return TOK_AT_POS;
(?i:center)    yylval_param->str = strdup(yytext); return TOK_AT_POS;
(?i:right)     yylval_param->str = strdup(yytext); return TOK_AT_POS;
(?i:no)        yylval_param->str = strdup(yytext); return TOK_BOOLEAN;
(?i:yes)       yylval_param->str = strdup(yytext); return TOK_BOOLEAN;
(?i:show)      yylval_param->str = strdup(yytext); return TOK_SHOW;
(?i:hide)      yylval_param->str = strdup(yytext); return TOK_HIDE;
(?i:activate)  yylval_param->str = strdup(yytext); return TOK_ACTIVATE;
(?i:deactivate) yylval_param->str= strdup(yytext); return TOK_DEACTIVATE;
(?i:bye)       yylval_param->str = strdup(yytext); return TOK_BYE;
(?i:vspace)    yylval_param->str = strdup(yytext); return TOK_COMMAND_VSPACE;
(?i:hspace)    yylval_param->str = strdup(yytext); return TOK_COMMAND_HSPACE;
(?i:symbol)    yylval_param->str = strdup(yytext); return TOK_COMMAND_SYMBOL;
(?i:note)      yylval_param->str = strdup(yytext); return TOK_COMMAND_NOTE;
(?i:comment)   yylval_param->str = strdup(yytext); return TOK_COMMAND_COMMENT;

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
\+=      return TOK_PLUS_EQUAL;
-        return TOK_DASH;
\+       return TOK_PLUS;
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

 /* Numbers */
[+\-]?[0-9]+\.?[0-9]*  %{
    yylval_param->str = strdup(yytext);
    return TOK_NUMBER;
%}

 /* Strings not ending with a dot */
[A-Za-z_]([A-Za-z0-9_\.]?[A-Za-z0-9_])* %{
    yylval_param->str = strdup(yytext);
    return TOK_STRING;
%}

 /* Strings ending with a dot, not followed by a second dot */
[A-Za-z_]([A-Za-z0-9_\.]?[A-Za-z0-9_])*\./[^\.] %{
    yylval_param->str = strdup(yytext);
    return TOK_STRING;
%}

[ \t]+    /* ignore whitespace */;
%%

/* END OF FILE */
