/* A Bison parser, made by GNU Bison 2.5.  */

/* Skeleton interface for Bison GLR parsers in C
   
      Copyright (C) 2002-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOK_STRING = 258,
     TOK_QSTRING = 259,
     TOK_NUMBER = 260,
     TOK_DASH = 261,
     TOK_EQUAL = 262,
     TOK_COMMA = 263,
     TOK_SEMICOLON = 264,
     TOK_PLUS = 265,
     TOK_PLUS_EQUAL = 266,
     TOK_OCBRACKET = 267,
     TOK_CCBRACKET = 268,
     TOK_OSBRACKET = 269,
     TOK_CSBRACKET = 270,
     TOK_ASTERISK = 271,
     TOK_MSC = 272,
     TOK_COLON_STRING = 273,
     TOK_COLON_QUOTED_STRING = 274,
     TOK_STYLE_NAME = 275,
     TOK_BOOLEAN = 276,
     TOK_REL_SOLID_TO = 277,
     TOK_REL_SOLID_FROM = 278,
     TOK_REL_SOLID_BIDIR = 279,
     TOK_REL_DOUBLE_TO = 280,
     TOK_REL_DOUBLE_FROM = 281,
     TOK_REL_DOUBLE_BIDIR = 282,
     TOK_REL_DASHED_TO = 283,
     TOK_REL_DASHED_FROM = 284,
     TOK_REL_DASHED_BIDIR = 285,
     TOK_REL_DOTTED_TO = 286,
     TOK_REL_DOTTED_FROM = 287,
     TOK_REL_DOTTED_BIDIR = 288,
     TOK_SPECIAL_ARC = 289,
     TOK_EMPH = 290,
     TOK_COMMAND_HEADING = 291,
     TOK_COMMAND_NUDGE = 292,
     TOK_COMMAND_NEWPAGE = 293,
     TOK_COMMAND_DEFCOLOR = 294,
     TOK_COMMAND_DEFSTYLE = 295,
     TOK_COMMAND_DEFDESIGN = 296,
     TOK_COMMAND_BIG = 297,
     TOK_COMMAND_PIPE = 298,
     TOK_COMMAND_MARK = 299,
     TOK_COMMAND_PARALLEL = 300,
     TOK_VERTICAL = 301,
     TOK_AT = 302,
     TOK_LOST = 303,
     TOK_AT_POS = 304,
     TOK_SHOW = 305,
     TOK_HIDE = 306,
     TOK_ACTIVATE = 307,
     TOK_DEACTIVATE = 308,
     TOK_BYE = 309,
     TOK_COMMAND_VSPACE = 310,
     TOK_COMMAND_HSPACE = 311,
     TOK_COMMAND_SYMBOL = 312,
     TOK_COMMAND_NOTE = 313,
     TOK_COMMAND_COMMENT = 314,
     TOK_COMMAND_ENDNOTE = 315,
     TOK_COMMAND_FOOTNOTE = 316,
     TOK_COMMAND_TITLE = 317,
     TOK_COMMAND_SUBTITLE = 318,
     TOK__NEVER__HAPPENS = 319
   };
#endif


#ifndef YYSTYPE
typedef union YYSTYPE
{

/* Line 2663 of glr.c  */
#line 93 "language.yy"

    char                          *str;
    CHAR_IF_CSH(Msc)              *msc;
    CHAR_IF_CSH(ArcBase)          *arcbase;
    CHAR_IF_CSH(ArcList)          *arclist;
    CHAR_IF_CSH(ArcArrow)         *arcarrow;
    CHAR_IF_CSH(ArcVerticalArrow) *arcvertarrow;
    CHAR_IF_CSH(ArcBox)           *arcbox;
    CHAR_IF_CSH(ArcPipe)          *arcpipe;
    CHAR_IF_CSH(ArcBoxSeries)     *arcboxseries;
    CHAR_IF_CSH(ArcPipeSeries)    *arcpipeseries;
    CHAR_IF_CSH(ArcParallel)      *arcparallel;
    CHAR_IF_CSH(EArcType)          arctype;
    CHAR_IF_CSH(EntityApp)        *entity;
    CHAR_IF_CSH(EntityAppHelper)  *entitylist;
    CHAR_IF_CSH(Attribute)        *attrib;
    CHAR_IF_CSH(AttributeList)    *attriblist;
    CHAR_IF_CSH(VertXPos)         *vertxpos;
    CHAR_IF_CSH(ExtVertXPos)      *extvertxpos;
    CHAR_IF_CSH(NamePair)         *namerel;
    std::list<std::string>        *stringlist;
    CHAR_IF_CSH(ESide)            eside;
	CHAR_IF_CSH(ArrowSegmentData) arcsegdata;



/* Line 2663 of glr.c  */
#line 140 "language.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{

  int first_line;
  int first_column;
  int last_line;
  int last_column;

} YYLTYPE;
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif








