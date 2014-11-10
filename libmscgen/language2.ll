%option reentrant noyywrap nounput stack
%option bison-bridge bison-locations
%s LEX_STATE_MSCGEN_COMPAT

%{
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

#include "commands.h" //MSC_* defs and CommandNote and Shapes in entity.h
#include "colorsyntax.h"
#include "language_misc.h"

#else

#include "msc.h"
#define YYMSC_RESULT_TYPE Msc
#define RESULT msc

#define CHAR_IF_CSH(A) A
#include "language.h"
#include "language_misc.h"

#endif

#ifdef C_S_H_IS_COMPILED

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

#define YY_USER_ACTION do {                     \
    yylloc->first_pos = yylloc->last_pos+1;     \
    yylloc->last_pos = yylloc->last_pos+yyleng; \
    /* Set initial condition according to our compatibility mode*/ \
    /* (usually changes only once after the 'msc {...}' is detected or at the beginning) */ \
    BEGIN(yyget_extra(yyscanner)->csh->mscgen_compat == EMscgenCompat::FORCE_MSCGEN ? LEX_STATE_MSCGEN_COMPAT : INITIAL); \
    } while(0);

#define YYRHSLOC(Rhs, K) ((Rhs)[K])

#define YYLLOC_DEFAULT(Current, Rhs, N)                  \
    do                                                   \
      if (YYID (N))                                      \
    {                                                    \
      (Current).first_pos = YYRHSLOC (Rhs, 1).first_pos; \
      (Current).last_pos  = YYRHSLOC (Rhs, N).last_pos;  \
    }                                                    \
      else                                               \
    {                                                    \
        (Current).first_pos = (Current).last_pos   =     \
        YYRHSLOC (Rhs, 0).last_pos;                      \
    }                                                    \
    while (YYID (0))

#else

//Read in 512-byte chunks
//We use this low value to make progress reporint more frequent
#define YY_READ_BUF_SIZE 512

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
        pp->msc->Progress.DoneItem(MscProgress::PARSE, res); \
        pp->pos += res;                             \
    }                                               \
} while (0)

#define YY_USER_ACTION do {                              \
    yylloc->first_line = yylloc->last_line;              \
    yylloc->first_column = yylloc->last_column+1;        \
    yylloc->last_column = yylloc->first_column+yyleng-1; \
    /* Set initial condition according to our compatibility mode*/ \
    /* (usually changes only once after the 'msc {...}' is detected or at the beginning) */ \
    BEGIN(yyget_extra(yyscanner)->msc->mscgen_compat == EMscgenCompat::FORCE_MSCGEN ? LEX_STATE_MSCGEN_COMPAT : INITIAL); \
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

[ \t]+    /* ignore whitespace */;

 /* These shape definition keywords are case sensitive */
M	yylval_param->shapecommand = ShapeElement::MOVE_TO; return TOK_SHAPE_COMMAND;
L	yylval_param->shapecommand = ShapeElement::LINE_TO; return TOK_SHAPE_COMMAND;
C	yylval_param->shapecommand = ShapeElement::CURVE_TO; return TOK_SHAPE_COMMAND;
E	yylval_param->shapecommand = ShapeElement::CLOSE_PATH; return TOK_SHAPE_COMMAND;
S	yylval_param->shapecommand = ShapeElement::SECTION_BG; return TOK_SHAPE_COMMAND;
T	yylval_param->shapecommand = ShapeElement::TEXT_AREA; return TOK_SHAPE_COMMAND;
H	yylval_param->shapecommand = ShapeElement::HINT_AREA; return TOK_SHAPE_COMMAND;

 /* This is used for mscgen style loss arrow symbol -x and x-*/
X|x yylval_param->str = strdup(yytext);  return TOK_REL_X;

 /* These keywords are case insensitive */
(?i:msc)       yylval_param->str = strdup(yytext); return TOK_MSC;
(?i:heading)   yylval_param->str = strdup(yytext); return TOK_COMMAND_HEADING;
(?i:nudge)     yylval_param->str = strdup(yytext); return TOK_COMMAND_NUDGE;
(?i:defshape)  yylval_param->str = strdup(yytext); return TOK_COMMAND_DEFSHAPE;
(?i:defcolor)  yylval_param->str = strdup(yytext); return TOK_COMMAND_DEFCOLOR;
(?i:defstyle)  yylval_param->str = strdup(yytext); return TOK_COMMAND_DEFSTYLE;
(?i:defdesign) yylval_param->str = strdup(yytext); return TOK_COMMAND_DEFDESIGN;
(?i:newpage)   yylval_param->str = strdup(yytext); return TOK_COMMAND_NEWPAGE;
(?i:block)     yylval_param->str = strdup(yytext); return TOK_COMMAND_BIG;
(?i:box)       yylval_param->str = strdup(yytext); return TOK_COMMAND_BOX;
(?i:pipe)      yylval_param->str = strdup(yytext); return TOK_COMMAND_PIPE;
(?i:mark)      yylval_param->str = strdup(yytext); return TOK_COMMAND_MARK;
(?i:parallel)  yylval_param->str = strdup(yytext); return TOK_COMMAND_PARALLEL;
(?i:overlap)   yylval_param->str = strdup(yytext); return TOK_COMMAND_OVERLAP;
(?i:vertical)  yylval_param->str = strdup(yytext); return TOK_VERTICAL;
(?i:at)        yylval_param->str = strdup(yytext); return TOK_AT;
(?i:lost)      yylval_param->str = strdup(yytext); return TOK_LOST;
(?i:left)      yylval_param->str = strdup(yytext); return TOK_AT_POS;
(?i:center)    yylval_param->str = strdup(yytext); return TOK_AT_POS;
(?i:right)     yylval_param->str = strdup(yytext); return TOK_AT_POS;
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
(?i:endnote)   yylval_param->str = strdup(yytext); return TOK_COMMAND_ENDNOTE;
(?i:footnote)  yylval_param->str = strdup(yytext); return TOK_COMMAND_FOOTNOTE;
(?i:title)     yylval_param->str = strdup(yytext); return TOK_COMMAND_TITLE;
(?i:subtitle)  yylval_param->str = strdup(yytext); return TOK_COMMAND_SUBTITLE;
(?i:text)      yylval_param->str = strdup(yytext); return TOK_COMMAND_TEXT;
(?i:brace)     yylval_param->str = strdup(yytext); return TOK_VERTICAL_SHAPE;
(?i:bracket)   yylval_param->str = strdup(yytext); return TOK_VERTICAL_SHAPE;
(?i:range)     yylval_param->str = strdup(yytext); return TOK_VERTICAL_SHAPE;
(?i:pointer)   yylval_param->str = strdup(yytext); return TOK_VERTICAL_SHAPE;
(?i:rbox)      yylval_param->str = strdup(yytext); return TOK_MSCGEN_RBOX;
(?i:abox)      yylval_param->str = strdup(yytext); return TOK_MSCGEN_ABOX;

\.\.\.   yylval_param->arcsymbol = EArcSymbol::DIV_DISCO;       return TOK_SPECIAL_ARC;// ...
---      yylval_param->arcsymbol = EArcSymbol::DIV_DIVIDER;     return TOK_SPECIAL_ARC;// ---
\|\|\|   yylval_param->arcsymbol = EArcSymbol::DIV_VSPACE;      return TOK_SPECIAL_ARC;// |||
-\>      yylval_param->arcsymbol = EArcSymbol::ARC_SOLID;       return TOK_REL_TO;     // ->
\<-      yylval_param->arcsymbol = EArcSymbol::ARC_SOLID;       return TOK_REL_FROM;   // <-
\<-\>    yylval_param->arcsymbol = EArcSymbol::ARC_SOLID_BIDIR; return TOK_REL_BIDIR;  // <->
=\>      yylval_param->arcsymbol = EArcSymbol::ARC_DOUBLE;      return TOK_REL_TO;     // =>
\<=      yylval_param->arcsymbol = EArcSymbol::ARC_DOUBLE;      return TOK_REL_FROM;   // <= 
\<=\>    yylval_param->arcsymbol = EArcSymbol::ARC_DOUBLE_BIDIR;return TOK_REL_BIDIR;  // <=>
\>\>     yylval_param->arcsymbol = EArcSymbol::ARC_DASHED;      return TOK_REL_TO;     // >>
\<\<     yylval_param->arcsymbol = EArcSymbol::ARC_DASHED;      return TOK_REL_FROM;   // <<
\<\<\>\> yylval_param->arcsymbol = EArcSymbol::ARC_DASHED_BIDIR;return TOK_REL_BIDIR;  // <<>>
\>       yylval_param->arcsymbol = EArcSymbol::ARC_DOTTED;      return TOK_REL_TO;     // >
\<       yylval_param->arcsymbol = EArcSymbol::ARC_DOTTED;      return TOK_REL_FROM;   // <
\<\>     yylval_param->arcsymbol = EArcSymbol::ARC_DOTTED_BIDIR;return TOK_REL_BIDIR;  // <>
=\>\>    yylval_param->arcsymbol = EArcSymbol::ARC_DBLDBL;      return TOK_REL_TO;     // =>>
\<\<=    yylval_param->arcsymbol = EArcSymbol::ARC_DBLDBL;      return TOK_REL_FROM;   // <<=
\<\<=\>\> yylval_param->arcsymbol = EArcSymbol::ARC_DBLDBL_BIDIR;return TOK_REL_BIDIR; // <<=>>
==\>     yylval_param->arcsymbol = EArcSymbol::ARC_DOUBLE2;      return TOK_REL_TO;    // ==>
\<==     yylval_param->arcsymbol = EArcSymbol::ARC_DOUBLE2;      return TOK_REL_FROM;  // <==
\<==\>   yylval_param->arcsymbol = EArcSymbol::ARC_DOUBLE2_BIDIR;return TOK_REL_BIDIR; // <==>

<INITIAL>{
    --       yylval_param->arcsymbol = EArcSymbol::BOX_SOLID;       return TOK_EMPH;       // --
    \+\+     yylval_param->arcsymbol = EArcSymbol::BOX_DASHED;      return TOK_EMPH_PLUS_PLUS;// ++
    \.\.     yylval_param->arcsymbol = EArcSymbol::BOX_DOTTED;      return TOK_EMPH;       // ..
    ==       yylval_param->arcsymbol = EArcSymbol::BOX_DOUBLE;      return TOK_EMPH;       // == 
}
<LEX_STATE_MSCGEN_COMPAT>{
    --       yylval_param->arcsymbol = EArcSymbol::ARC_SOLID_BIDIR;  return TOK_REL_MSCGEN; // --
    \:\:     yylval_param->arcsymbol = EArcSymbol::ARC_DOUBLE2_BIDIR;return TOK_REL_MSCGEN; // ::
    \.\.     yylval_param->arcsymbol = EArcSymbol::ARC_DOTTED_BIDIR; return TOK_REL_MSCGEN; // ..
    ==       yylval_param->arcsymbol = EArcSymbol::ARC_DOUBLE_BIDIR; return TOK_REL_MSCGEN; // == 
    \:\>     yylval_param->arcsymbol = EArcSymbol::ARC_DOUBLE2;      return TOK_REL_TO;     // :>
    \<\:     yylval_param->arcsymbol = EArcSymbol::ARC_DOUBLE2;      return TOK_REL_FROM;   // <:
    \<\:\>   yylval_param->arcsymbol = EArcSymbol::ARC_DOUBLE2_BIDIR;return TOK_REL_BIDIR;  // <:>
}

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
\*       return TOK_ASTERISK;

 /* We need to list only those style names, which are not conforming to
  * TOK_STRING above. */
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


 /* This is a colon-quoted string, finished by a quotation mark
 ** : "<string>"
 ** <string> can contain escaped quotation marks, hashmarks, but no line breaks
 */
\:[ \t]*\"([^\"\x0d\x0a]|(\\\"))*\" %{
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
    FileLineCol pos(yyget_extra(yyscanner)->msc->current_file,
                 yylloc->first_line, yylloc->first_column + unsigned(s+1 - yytext));
    yylval_param->str = strdup((pos.Print() + str).c_str());
    }
  #endif
    return TOK_COLON_QUOTED_STRING;
%}

 /* This is a colon-quoted string, finished by a newline (trailing context)
 ** : "<string>$
 ** <string> can contain escaped quotation marks, hashmarks, but no line breaks
 */
\:[ \t]*\"([^\"\x0d\x0a]|(\\\"))*  %{
  #ifdef C_S_H_IS_COMPILED
    yylval_param->str = strdup(yytext);
    yyget_extra(yyscanner)->csh->AddCSH_ErrorAfter(*yylloc, "Missing closing quotation mark.");
  #else
    {
    /* after whitespaces we are guaranteed to have a heading quot */
    const char *s = msc_remove_head_tail_whitespace(yytext+1);
    // s now points to heading quotation mark
    FileLineCol pos(yyget_extra(yyscanner)->msc->current_file,
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
 ** terminated by any of: [ { or ;
 ** Honors escaping of the above via a backslash
 ** Can contain quotation marks (escaped or unescaped), but can not start with it
 ** If it contains a hashmark, unescaped [ { or ; is allowed till the end of the line
 ** (representing a commented section inside a label)
 ** Not available in mscgen compatibility mode. There we use the one below
 *  \:[\t]*(((#[^\x0d\x0a]*)|[^\"\;\[\{\\]|(\\.))((#[^\x0d\x0a]*)|[^\;\[\{\\]|(\\.))*(\\)?|\\)  
 * \:[ \t]*((#[^\0xd\0xa]*|[^\"\;\{\[\\#\ \t]|(\\[^0xd0xa])))((#[^0xd0xa]*|[^\;\{\[\\#]|(\\[^0xd0xa])))*
 */
<INITIAL>\:[ \t]*((#[^\0xd\0xa]*|[^\"\;\{\[\\#\ \t]|(\\[^0xd0xa])))((#[^0xd0xa]*|[^\;\{\[\\#]|(\\[^0xd0xa])))*\\? %{
  #ifdef C_S_H_IS_COMPILED
    yylval_param->str = strdup(yytext);
  #else
    yylval_param->str = msc_process_colon_string(yytext, yylloc,
                        yyget_extra(yyscanner)->msc->current_file);
  #endif
    return TOK_COLON_STRING;
%}

 /* This is a non quoted colon-string, where no '>' or ':' follows the initial colon
 ** : <string>
 ** terminated by any of: [ { or ;
 ** Honors escaping of the above via a backslash
 ** Can contain quotation marks (escaped or unescaped), but can not start with it
 ** If it contains a hashmark, unescaped [ { or ; is allowed till the end of the line
 ** (representing a commented section inside a label)
 ** Used only in mscgen compatibility mode to separate from :> and :: arrow symbols.
 *  \:[\t]*(((#[^\x0d\x0a]*)|[^\"\;\[\{\\]|(\\.))((#[^\x0d\x0a]*)|[^\;\[\{\\]|(\\.))*(\\)?|\\)  
 * \:[ \t]*((#[^\0xd\0xa]*|[^\"\;\{\[\\#\ \t]|(\\[^0xd0xa])))((#[^0xd0xa]*|[^\;\{\[\\#]|(\\[^0xd0xa])))*
 */
<LEX_STATE_MSCGEN_COMPAT>\:(([ \t]*#[^\0xd\0xa]*|[ \t]+[^\"\;\{\[\\#\ \t]|[ \t]*(\\[^0xd0xa])|[^\"\;\{\[\\#\ \t\>\:]))((#[^0xd0xa]*|[^\;\{\[\\#]|(\\[^0xd0xa])))*\\? %{
  #ifdef C_S_H_IS_COMPILED
    yylval_param->str = strdup(yytext);
  #else
    yylval_param->str = msc_process_colon_string(yytext, yylloc,
                        yyget_extra(yyscanner)->msc->current_file);
  #endif
    return TOK_COLON_STRING;
%}

 /* This is a degenerate non quoted colon-string
 ** a colon followed by a solo escape or just a colon
 */
\:[ \t]*\\? %{
  #ifdef C_S_H_IS_COMPILED
    yylval_param->str = strdup(yytext);
   #else
    yylval_param->str = msc_process_colon_string(yytext, yylloc,
                        yyget_extra(yyscanner)->msc->current_file);
  #endif
    return TOK_COLON_STRING;
%}


 /* A simple quoted string, that can have escaped quotation marks inside.*/
\"([^\"\x0d\x0a]|(\\\"))*\" %{
    yylval_param->str = strdup(yytext+1);
    yylval_param->str[strlen(yylval_param->str) - 1] = '\0';
    return TOK_QSTRING;
%}

 /* A simple quoted string, missing a closing quotation mark */
\"([^\"\x0d\x0a]|(\\\"))* %{
  #ifdef C_S_H_IS_COMPILED
    yylval_param->str = strdup(yytext+1);
    yyget_extra(yyscanner)->csh->AddCSH_ErrorAfter(*yylloc, "Missing closing quotation mark.");
#else
    {
    yylval_param->str = strdup(yytext+1);
    FileLineCol pos(yyget_extra(yyscanner)->msc->current_file,
                 yylloc->first_line, yylloc->first_column);
    yyget_extra(yyscanner)->msc->Error.Error(pos,
         "This opening quotation mark misses its closing pair. "
         "Assuming string termination at line-end.",
         "Quoted strings cannot have line breaks. Use \'\\n\' to insert a line break.");
    }
  #endif
    return TOK_QSTRING;
%}

 /* A simple quoted string with line breaks inside 
  *  Just for msc-gen compatibility */
<LEX_STATE_MSCGEN_COMPAT>\"([^\"]|(\\\"))*\" %{
  #ifdef C_S_H_IS_COMPILED
    yylval_param->str = strdup(yytext+1);
    yylval_param->str[strlen(yylval_param->str) - 1] = '\0';
  #else
    yylval_param->str = msc_process_multiline_qstring(yytext, yylloc,
                        yyget_extra(yyscanner)->msc->current_file);
  #endif
    return TOK_QSTRING;
%}


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

 /* Strings ending with a dot, not followed by a second dot 
  * Two dots one after another shall be parsed a '..' into TOK_EMPH*/
[A-Za-z_]([A-Za-z0-9_\.]?[A-Za-z0-9_])*\./[^\.] %{
    yylval_param->str = strdup(yytext);
    return TOK_STRING;
%}

 /* Color definitions */
 /* string+-number[,number]*/
[A-Za-z_]([A-Za-z0-9_\.]?[A-Za-z0-9_])*(\.)?[\+\-][0-9]+(\.[0-9]*)?(\,[0-9]+(\.[0-9]*)?)? %{
    yylval_param->str = strdup(yytext);
    return TOK_COLORDEF;
%}

 /* string,number[+-number]*/
[A-Za-z_]([A-Za-z0-9_\.]?[A-Za-z0-9_])*(\.)?\,[0-9]+(\.[0-9]*)?([\+\-][0-9]+(\.[0-9]*)?)? %{
    yylval_param->str = strdup(yytext);
    return TOK_COLORDEF;
%}

 /* number,number,number[,number] */
[0-9]+(\.[0-9]*)?\,[0-9]+(\.[0-9]*)?\,[0-9]+(\.[0-9]*)?(\,[0-9]+(\.[0-9]*)?)? %{
    yylval_param->str = strdup(yytext);
    return TOK_COLORDEF;
%}

<<EOF>> return TOK_EOF;


%%

/* END OF FILE */
