/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
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
     TOK_MSC = 271,
     TOK_COLON_STRING = 272,
     TOK_COLON_QUOTED_STRING = 273,
     TOK_STYLE_NAME = 274,
     TOK_BOOLEAN = 275,
     TOK_REL_SOLID_TO = 276,
     TOK_REL_SOLID_FROM = 277,
     TOK_REL_SOLID_BIDIR = 278,
     TOK_REL_DOUBLE_TO = 279,
     TOK_REL_DOUBLE_FROM = 280,
     TOK_REL_DOUBLE_BIDIR = 281,
     TOK_REL_DASHED_TO = 282,
     TOK_REL_DASHED_FROM = 283,
     TOK_REL_DASHED_BIDIR = 284,
     TOK_REL_DOTTED_TO = 285,
     TOK_REL_DOTTED_FROM = 286,
     TOK_REL_DOTTED_BIDIR = 287,
     TOK_SPECIAL_ARC = 288,
     TOK_EMPH = 289,
     TOK_COMMAND_HEADING = 290,
     TOK_COMMAND_NUDGE = 291,
     TOK_COMMAND_NEWPAGE = 292,
     TOK_COMMAND_DEFCOLOR = 293,
     TOK_COMMAND_DEFSTYLE = 294,
     TOK_COMMAND_DEFDESIGN = 295,
     TOK_COMMAND_BIG = 296,
     TOK_COMMAND_PIPE = 297,
     TOK_COMMAND_MARK = 298,
     TOK_COMMAND_PARALLEL = 299,
     TOK_VERTICAL = 300,
     TOK_AT = 301,
     TOK_AT_POS = 302,
     TOK_SHOW = 303,
     TOK_HIDE = 304,
     TOK_ACTIVATE = 305,
     TOK_DEACTIVATE = 306,
     TOK_BYE = 307,
     TOK_COMMAND_VSPACE = 308,
     TOK_COMMAND_HSPACE = 309,
     TOK_COMMAND_SYMBOL = 310,
     TOK_COMMAND_NOTE = 311,
     TOK_COMMAND_COMMENT = 312,
     TOK_COMMAND_TITLE = 313,
     TOK_COMMAND_SUBTITLE = 314,
     TOK__NEVER__HAPPENS = 315
   };
#endif
/* Tokens.  */
#define TOK_STRING 258
#define TOK_QSTRING 259
#define TOK_NUMBER 260
#define TOK_DASH 261
#define TOK_EQUAL 262
#define TOK_COMMA 263
#define TOK_SEMICOLON 264
#define TOK_PLUS 265
#define TOK_PLUS_EQUAL 266
#define TOK_OCBRACKET 267
#define TOK_CCBRACKET 268
#define TOK_OSBRACKET 269
#define TOK_CSBRACKET 270
#define TOK_MSC 271
#define TOK_COLON_STRING 272
#define TOK_COLON_QUOTED_STRING 273
#define TOK_STYLE_NAME 274
#define TOK_BOOLEAN 275
#define TOK_REL_SOLID_TO 276
#define TOK_REL_SOLID_FROM 277
#define TOK_REL_SOLID_BIDIR 278
#define TOK_REL_DOUBLE_TO 279
#define TOK_REL_DOUBLE_FROM 280
#define TOK_REL_DOUBLE_BIDIR 281
#define TOK_REL_DASHED_TO 282
#define TOK_REL_DASHED_FROM 283
#define TOK_REL_DASHED_BIDIR 284
#define TOK_REL_DOTTED_TO 285
#define TOK_REL_DOTTED_FROM 286
#define TOK_REL_DOTTED_BIDIR 287
#define TOK_SPECIAL_ARC 288
#define TOK_EMPH 289
#define TOK_COMMAND_HEADING 290
#define TOK_COMMAND_NUDGE 291
#define TOK_COMMAND_NEWPAGE 292
#define TOK_COMMAND_DEFCOLOR 293
#define TOK_COMMAND_DEFSTYLE 294
#define TOK_COMMAND_DEFDESIGN 295
#define TOK_COMMAND_BIG 296
#define TOK_COMMAND_PIPE 297
#define TOK_COMMAND_MARK 298
#define TOK_COMMAND_PARALLEL 299
#define TOK_VERTICAL 300
#define TOK_AT 301
#define TOK_AT_POS 302
#define TOK_SHOW 303
#define TOK_HIDE 304
#define TOK_ACTIVATE 305
#define TOK_DEACTIVATE 306
#define TOK_BYE 307
#define TOK_COMMAND_VSPACE 308
#define TOK_COMMAND_HSPACE 309
#define TOK_COMMAND_SYMBOL 310
#define TOK_COMMAND_NOTE 311
#define TOK_COMMAND_COMMENT 312
#define TOK_COMMAND_TITLE 313
#define TOK_COMMAND_SUBTITLE 314
#define TOK__NEVER__HAPPENS 315




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 136 "language.yy"

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



/* Line 2068 of yacc.c  */
#line 195 "language.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif



#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



