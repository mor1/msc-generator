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

/** @file language_misc.h Interface between parser and us.
 * @ingroup libmscgen_files 
 * This file is used both for color syntax and language compilation */
#if !defined(PARSERHELPER_H)
#define PARSERHELPER_H

#ifndef HAVE_UNISTD_H
#define YY_NO_UNISTD_H
extern int isatty(int);
#endif


#ifndef YYMSC_RESULT_TYPE
#error You must set YYMSC_RESULT_TYPE before you include parserhelper.h
#endif

//Parser and lexer related stuff
typedef struct
{
    void              *yyscanner;
    char              *buf;
    int                pos;
    int                length;
    YYMSC_RESULT_TYPE *RESULT;
} parse_parm;

#define YY_EXTRA_TYPE   parse_parm *

int     yylex(YYSTYPE *, YYLTYPE *, void *);
int     yylex_init(void **);
int     yylex_destroy(void *);
void    yyset_extra(YY_EXTRA_TYPE, void *);
int     yyparse(YYMSC_RESULT_TYPE&, void *);
int     yyget_lineno(void* scanner);

#endif //PARSERHELPER_H
