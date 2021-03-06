/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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

#ifndef YY_CSH_COLORSYNTAX_H_INCLUDED
# define YY_CSH_COLORSYNTAX_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int csh_debug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOK_EOF = 0,
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
    TOK_COLORDEF = 276,
    TOK_REL_TO = 277,
    TOK_REL_FROM = 278,
    TOK_REL_BIDIR = 279,
    TOK_REL_X = 280,
    TOK_REL_MSCGEN = 281,
    TOK_SPECIAL_ARC = 282,
    TOK_EMPH = 283,
    TOK_EMPH_PLUS_PLUS = 284,
    TOK_COMMAND_HEADING = 285,
    TOK_COMMAND_NUDGE = 286,
    TOK_COMMAND_NEWPAGE = 287,
    TOK_COMMAND_DEFSHAPE = 288,
    TOK_COMMAND_DEFCOLOR = 289,
    TOK_COMMAND_DEFSTYLE = 290,
    TOK_COMMAND_DEFDESIGN = 291,
    TOK_COMMAND_BIG = 292,
    TOK_COMMAND_BOX = 293,
    TOK_COMMAND_PIPE = 294,
    TOK_COMMAND_MARK = 295,
    TOK_COMMAND_PARALLEL = 296,
    TOK_COMMAND_OVERLAP = 297,
    TOK_VERTICAL = 298,
    TOK_VERTICAL_SHAPE = 299,
    TOK_AT = 300,
    TOK_LOST = 301,
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
    TOK_COMMAND_ENDNOTE = 313,
    TOK_COMMAND_FOOTNOTE = 314,
    TOK_COMMAND_TITLE = 315,
    TOK_COMMAND_SUBTITLE = 316,
    TOK_COMMAND_TEXT = 317,
    TOK_SHAPE_COMMAND = 318,
    TOK_MSCGEN_RBOX = 319,
    TOK_MSCGEN_ABOX = 320,
    TOK__NEVER__HAPPENS = 321
  };
#endif
/* Tokens.  */
#define TOK_EOF 0
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
#define TOK_ASTERISK 271
#define TOK_MSC 272
#define TOK_COLON_STRING 273
#define TOK_COLON_QUOTED_STRING 274
#define TOK_STYLE_NAME 275
#define TOK_COLORDEF 276
#define TOK_REL_TO 277
#define TOK_REL_FROM 278
#define TOK_REL_BIDIR 279
#define TOK_REL_X 280
#define TOK_REL_MSCGEN 281
#define TOK_SPECIAL_ARC 282
#define TOK_EMPH 283
#define TOK_EMPH_PLUS_PLUS 284
#define TOK_COMMAND_HEADING 285
#define TOK_COMMAND_NUDGE 286
#define TOK_COMMAND_NEWPAGE 287
#define TOK_COMMAND_DEFSHAPE 288
#define TOK_COMMAND_DEFCOLOR 289
#define TOK_COMMAND_DEFSTYLE 290
#define TOK_COMMAND_DEFDESIGN 291
#define TOK_COMMAND_BIG 292
#define TOK_COMMAND_BOX 293
#define TOK_COMMAND_PIPE 294
#define TOK_COMMAND_MARK 295
#define TOK_COMMAND_PARALLEL 296
#define TOK_COMMAND_OVERLAP 297
#define TOK_VERTICAL 298
#define TOK_VERTICAL_SHAPE 299
#define TOK_AT 300
#define TOK_LOST 301
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
#define TOK_COMMAND_ENDNOTE 313
#define TOK_COMMAND_FOOTNOTE 314
#define TOK_COMMAND_TITLE 315
#define TOK_COMMAND_SUBTITLE 316
#define TOK_COMMAND_TEXT 317
#define TOK_SHAPE_COMMAND 318
#define TOK_MSCGEN_RBOX 319
#define TOK_MSCGEN_ABOX 320
#define TOK__NEVER__HAPPENS 321

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 92 "language.yy" /* yacc.c:1909  */

    char                          *str;
    CHAR_IF_CSH(ArcBase)          *arcbase;
    CHAR_IF_CSH(ArcList)          *arclist;
    CHAR_IF_CSH(ArcArrow)         *arcarrow;
    CHAR_IF_CSH(ArcVerticalArrow) *arcvertarrow;
    CHAR_IF_CSH(ArcBox)           *arcbox;
    CHAR_IF_CSH(ArcPipe)          *arcpipe;
    CHAR_IF_CSH(ArcBoxSeries)     *arcboxseries;
    CHAR_IF_CSH(ArcPipeSeries)    *arcpipeseries;
    CHAR_IF_CSH(ArcParallel)      *arcparallel;
    EArcSymbol                     arcsymbol;
    CHAR_IF_CSH(EntityAppHelper)  *entitylist;
    CHAR_IF_CSH(Attribute)        *attrib;
    CHAR_IF_CSH(AttributeList)    *attriblist;
    CHAR_IF_CSH(VertXPos)         *vertxpos;
    CHAR_IF_CSH(ExtVertXPos)      *extvertxpos;
    CHAR_IF_CSH(NamePair)         *namerel;
    std::list<std::string>        *stringlist;
    CHAR_IF_CSH(ESide)            eside;
    CHAR_IF_CSH(ArrowSegmentData) arcsegdata;
    CHAR_IF_CSH(ArcVerticalArrow::EVerticalShape)   vshape;
	CHAR_IF_CSH(ArcTypePlusDir)   arctypeplusdir;
	ShapeElement::Type            shapecommand;
	CHAR_IF_CSH(Shape)            *shape;
	CHAR_IF_CSH(ShapeElement)     *shapeelement;

#line 216 "colorsyntax.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



int csh_parse (YYMSC_RESULT_TYPE &RESULT, void *yyscanner);

#endif /* !YY_CSH_COLORSYNTAX_H_INCLUDED  */
