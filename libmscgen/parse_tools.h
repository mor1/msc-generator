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

/** @file parse_tools.h Utilities for parsing.
 * @ingroup libmscgen_files 
 * This file is used if we do parsing for language and not for Color Syntax Highlight.
 * Need to include this for the right YYLTYPE */

#include "msc.h"

#ifndef CHAR_IF_CSH
#define CHAR_IF_CSH(A) A
#endif

#include "language.h"

/** A macro creating a file_line_range both starting and ending at `A`*/
#define MSC_POS(A) MSC_POS2(A,A) 
/** A macro creating a file_line_range starting at `A` and ending at `B`.*/
// yacc column numbers start with 1 - we substract one here
#define MSC_POS2(A, B) file_line_range(                                                          \
            file_line(msc.current_file, (A).first_line, (A).first_column - 1), \
            file_line(msc.current_file, (B).last_line,  (B).last_column  - 1))

void msc_jump_line(YYLTYPE *loc);
char *msc_remove_head_tail_whitespace(char *s);
char* msc_process_colon_string(const char *s, YYLTYPE *loc, unsigned file_no);

std::string ConvertEmphasisToBox(const std::string &style, const YYLTYPE *loc, Msc &msc);

/** Converts 'yes' and 'no' to a bool value.*/
inline bool str2bool(const char*s)
{
   return (s[0]!='n' && s[0]!='N') || (s[1]!='o' && s[1]!='O');
}

void yyerror(YYLTYPE*loc, Msc &msc, void *yyscanner, const char *str);

