%option header-file="language2.h"
%option reentrant noyywrap nounput
%option bison-bridge bison-locations case-insensitive

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

#include <stdio.h>
#include <string.h>
#include <iostream>
#include "msc.h"
#include "language.h"  /* Token definitions from Yacc/Bison */

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
** (copies to new memory) */
char* msc_process_colon_string(const char *s)
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
        int a = end_line-1;
        while (a>=start_line && (s[a]==' ' || s[a]=='\t')) a--;
        //now append the line (without the whitespaces)
        memcpy(ret+new_pos, s+start_line, a-start_line+1);
        new_pos += a-start_line+1;
        //if ending was a null we are done with processing all lines
        if (!ending) break;
        //append "\n" escape for msc-generator, but only if not an empty first line
        if (new_pos) {
            //add a space for empty lines
            if (a-start_line+1 == 0)
                ret[new_pos++] = ' ';
            ret[new_pos++] = '\\';
            ret[new_pos++] = 'n';
        }
        //Check for a two character CRLF, skip over the LF, too
        if (ending == 13 && s[end_line+1] == 10) end_line++;
        old_pos = end_line+1;
    }
    ret[new_pos] = 0;
    return ret;
}

/* remove heading and trailing quotation from a string (copies to new memory) */
char* msc_remove_quotes(const char *s)
{
    unsigned a=0;
    while (s[a]==' ' || s[a]=='\t') a++;
    if (strlen(s)>=2+a && s[a]=='"' && s[strlen(s)-1]=='"') {
        char * r = strdup(s+a+1);
        r[strlen(s)-2-a] = 0;
        return r;
    }
    return strdup(s);
}

%}

%%

\x0d\x0a     yylineno++;
\x0d         yylineno++;
\x0a         yylineno++;

#.*\x0d\x0a  yylineno++;   /* Ignore lines after '#' */
#.*\x0d      yylineno++;   /* Ignore lines after '#' */
#.*\x0a      yylineno++;   /* Ignore lines after '#' */


msc       yylval_param->str = strdup(yytext); return TOK_MSC;
heading   yylval_param->str = strdup(yytext); return TOK_COMMAND_HEADING;
nudge     yylval_param->str = strdup(yytext); return TOK_COMMAND_NUDGE;
defcolor  yylval_param->str = strdup(yytext); return TOK_COMMAND_DEFCOLOR;
defstyle  yylval_param->str = strdup(yytext); return TOK_COMMAND_DEFSTYLE;
defdesign yylval_param->str = strdup(yytext); return TOK_COMMAND_DEFDESIGN;
newpage   yylval_param->str = strdup(yytext); return TOK_COMMAND_NEWPAGE;
block     yylval_param->str = strdup(yytext); return TOK_COMMAND_BIG;
pipe      yylval_param->str = strdup(yytext); return TOK_COMMAND_PIPE;
mark      yylval_param->str = strdup(yytext); return TOK_COMMAND_MARK;
vertical  yylval_param->str = strdup(yytext); return TOK_VERTICAL;
at        yylval_param->str = strdup(yytext); return TOK_AT;

no        yylval_param->str = strdup(yytext); return TOK_BOOLEAN;
yes       yylval_param->str = strdup(yytext); return TOK_BOOLEAN;

\:[ \t]*\"[^\"]*\"                      yylval_param->str = msc_remove_quotes(yytext+1); return TOK_COLON_STRING;
\:[ \t]*[^ \t\"\;\[\{]?[^\;\[\{]*       yylval_param->str = msc_process_colon_string(yytext+1); return TOK_COLON_STRING;
[+\-]?[0-9]+\.?[0-9]*                   yylval_param->str = strdup(yytext); return TOK_NUMBER;
[A-Za-z_]([A-Za-z0-9_\.]?[A-Za-z0-9_])* yylval_param->str = strdup(yytext); return TOK_STRING;
\"[^\"]*\"                              yylval_param->str = strdup(yytext + 1); yylval_param->str[strlen(yylval_param->str) - 1] = '\0'; return TOK_QSTRING;

\.\.\.    yylval_param->arctype = MSC_ARC_DISCO;    return TOK_SPECIAL_ARC;        /* ... */
---       yylval_param->arctype = MSC_ARC_DIVIDER;  return TOK_SPECIAL_ARC;        /* --- */
-\>       yylval_param->arctype = MSC_ARC_SOLID;    return TOK_REL_SOLID_TO;         /* -> */
\<-       yylval_param->arctype = MSC_ARC_SOLID;    return TOK_REL_SOLID_FROM;       /* <- */
\<-\>     yylval_param->arctype = MSC_ARC_SOLID_BIDIR;    return TOK_REL_SOLID_BIDIR;      /* <-> */
=\>       yylval_param->arctype = MSC_ARC_DOUBLE;   return TOK_REL_DOUBLE_TO;      /* => */
\<=       yylval_param->arctype = MSC_ARC_DOUBLE;   return TOK_REL_DOUBLE_FROM;    /* <= */
\<=\>     yylval_param->arctype = MSC_ARC_DOUBLE_BIDIR;   return TOK_REL_DOUBLE_BIDIR;   /* <=> */
\>\>      yylval_param->arctype = MSC_ARC_DASHED;   return TOK_REL_DASHED_TO;      /* >> */
\<\<      yylval_param->arctype = MSC_ARC_DASHED;   return TOK_REL_DASHED_FROM;    /* << */
\<\<\>\>  yylval_param->arctype = MSC_ARC_DASHED_BIDIR;   return TOK_REL_DASHED_BIDIR;   /* <<>> */
\>        yylval_param->arctype = MSC_ARC_DOTTED;   return TOK_REL_DOTTED_TO;    /* > */
\<        yylval_param->arctype = MSC_ARC_DOTTED;   return TOK_REL_DOTTED_FROM;  /* < */
\<\>      yylval_param->arctype = MSC_ARC_DOTTED_BIDIR;   return TOK_REL_DOTTED_BIDIR;  /* <> */
--        yylval_param->arctype = MSC_EMPH_SOLID;   return TOK_EMPH;
\+\+      yylval_param->arctype = MSC_EMPH_DASHED;  return TOK_EMPH;
\.\.      yylval_param->arctype = MSC_EMPH_DOTTED;  return TOK_EMPH;
==        yylval_param->arctype = MSC_EMPH_DOUBLE;  return TOK_EMPH;
-         yylval_param->linenum = yylineno;  return TOK_DASH;
=         yylval_param->linenum = yylineno;  return TOK_EQUAL;
,         yylval_param->linenum = yylineno;  return TOK_COMMA;
\;        yylval_param->linenum = yylineno;  return TOK_SEMICOLON;
\{        yylval_param->linenum = yylineno;  return TOK_OCBRACKET;
\}        yylval_param->linenum = yylineno;  return TOK_CCBRACKET;
\[        yylval_param->linenum = yylineno;  return TOK_OSBRACKET;
\]        yylval_param->linenum = yylineno;  return TOK_CSBRACKET;

pipe--    yylval_param->str = strdup(yytext); return TOK_STYLE_NAME;
pipe\+\+  yylval_param->str = strdup(yytext); return TOK_STYLE_NAME;
pipe==    yylval_param->str = strdup(yytext); return TOK_STYLE_NAME;
pipe\.\.  yylval_param->str = strdup(yytext); return TOK_STYLE_NAME;
block-\>  yylval_param->str = strdup(yytext); return TOK_STYLE_NAME;
block\>   yylval_param->str = strdup(yytext); return TOK_STYLE_NAME;
block\>\> yylval_param->str = strdup(yytext); return TOK_STYLE_NAME;
block=\>  yylval_param->str = strdup(yytext); return TOK_STYLE_NAME;
vertical--    yylval_param->str = strdup(yytext); return TOK_STYLE_NAME;
vertical\+\+  yylval_param->str = strdup(yytext); return TOK_STYLE_NAME;
vertical==    yylval_param->str = strdup(yytext); return TOK_STYLE_NAME;
vertical\.\.  yylval_param->str = strdup(yytext); return TOK_STYLE_NAME;
vertical-\>   yylval_param->str = strdup(yytext); return TOK_STYLE_NAME;
vertical\>\>  yylval_param->str = strdup(yytext); return TOK_STYLE_NAME;
vertical\>    yylval_param->str = strdup(yytext); return TOK_STYLE_NAME;
vertical=\>   yylval_param->str = strdup(yytext); return TOK_STYLE_NAME;

[ \t]+    /* ignore whitespace */;
%%

/* END OF FILE */
