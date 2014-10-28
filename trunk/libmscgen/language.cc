/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 14 "language.yy" /* yacc.c:339  */

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

//if yyerror is defined, we compile with a prefix other than yy (csh_error)
//thus we compile the colorsyntax highlighting version
#ifdef yyerror
#define C_S_H (1)
#define C_S_H_IS_COMPILED
#else
#define C_S_H (0)
#endif

#ifdef C_S_H_IS_COMPILED

#define YYMSC_RESULT_TYPE Csh
#define RESULT csh

#include "csh.h"
#include "entity.h"//For Shapes

//If we scan for color syntax highlight use this location
//yyerror is defined by bison, the other is defined for flex
#define YYLTYPE_IS_DECLARED
#define YYLTYPE CshPos
#define CHAR_IF_CSH(A) char

#else

#define YYMSC_RESULT_TYPE Msc
#define RESULT msc

#define YYMSC_RESULT_TYPE Msc
#define RESULT msc

#define CHAR_IF_CSH(A) A
#endif

#include "arcs.h"//For AttrNames and AttrValues
#include "msc.h" //For AttrNames and AttrValues

#line 124 "language.cc" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "language.h".  */
#ifndef YY_YY_LANGUAGE_H_INCLUDED
# define YY_YY_LANGUAGE_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
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
    TOK_EMPH_PLUS_PLUS = 291,
    TOK_COMMAND_HEADING = 292,
    TOK_COMMAND_NUDGE = 293,
    TOK_COMMAND_NEWPAGE = 294,
    TOK_COMMAND_DEFSHAPE = 295,
    TOK_COMMAND_DEFCOLOR = 296,
    TOK_COMMAND_DEFSTYLE = 297,
    TOK_COMMAND_DEFDESIGN = 298,
    TOK_COMMAND_BIG = 299,
    TOK_COMMAND_BOX = 300,
    TOK_COMMAND_PIPE = 301,
    TOK_COMMAND_MARK = 302,
    TOK_COMMAND_PARALLEL = 303,
    TOK_COMMAND_OVERLAP = 304,
    TOK_VERTICAL = 305,
    TOK_VERTICAL_SHAPE = 306,
    TOK_AT = 307,
    TOK_LOST = 308,
    TOK_AT_POS = 309,
    TOK_SHOW = 310,
    TOK_HIDE = 311,
    TOK_ACTIVATE = 312,
    TOK_DEACTIVATE = 313,
    TOK_BYE = 314,
    TOK_COMMAND_VSPACE = 315,
    TOK_COMMAND_HSPACE = 316,
    TOK_COMMAND_SYMBOL = 317,
    TOK_COMMAND_NOTE = 318,
    TOK_COMMAND_COMMENT = 319,
    TOK_COMMAND_ENDNOTE = 320,
    TOK_COMMAND_FOOTNOTE = 321,
    TOK_COMMAND_TITLE = 322,
    TOK_COMMAND_SUBTITLE = 323,
    TOK_COMMAND_TEXT = 324,
    TOK_SHAPE_COMMAND = 325,
    TOK__NEVER__HAPPENS = 326
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
#define TOK_REL_SOLID_TO 277
#define TOK_REL_SOLID_FROM 278
#define TOK_REL_SOLID_BIDIR 279
#define TOK_REL_DOUBLE_TO 280
#define TOK_REL_DOUBLE_FROM 281
#define TOK_REL_DOUBLE_BIDIR 282
#define TOK_REL_DASHED_TO 283
#define TOK_REL_DASHED_FROM 284
#define TOK_REL_DASHED_BIDIR 285
#define TOK_REL_DOTTED_TO 286
#define TOK_REL_DOTTED_FROM 287
#define TOK_REL_DOTTED_BIDIR 288
#define TOK_SPECIAL_ARC 289
#define TOK_EMPH 290
#define TOK_EMPH_PLUS_PLUS 291
#define TOK_COMMAND_HEADING 292
#define TOK_COMMAND_NUDGE 293
#define TOK_COMMAND_NEWPAGE 294
#define TOK_COMMAND_DEFSHAPE 295
#define TOK_COMMAND_DEFCOLOR 296
#define TOK_COMMAND_DEFSTYLE 297
#define TOK_COMMAND_DEFDESIGN 298
#define TOK_COMMAND_BIG 299
#define TOK_COMMAND_BOX 300
#define TOK_COMMAND_PIPE 301
#define TOK_COMMAND_MARK 302
#define TOK_COMMAND_PARALLEL 303
#define TOK_COMMAND_OVERLAP 304
#define TOK_VERTICAL 305
#define TOK_VERTICAL_SHAPE 306
#define TOK_AT 307
#define TOK_LOST 308
#define TOK_AT_POS 309
#define TOK_SHOW 310
#define TOK_HIDE 311
#define TOK_ACTIVATE 312
#define TOK_DEACTIVATE 313
#define TOK_BYE 314
#define TOK_COMMAND_VSPACE 315
#define TOK_COMMAND_HSPACE 316
#define TOK_COMMAND_SYMBOL 317
#define TOK_COMMAND_NOTE 318
#define TOK_COMMAND_COMMENT 319
#define TOK_COMMAND_ENDNOTE 320
#define TOK_COMMAND_FOOTNOTE 321
#define TOK_COMMAND_TITLE 322
#define TOK_COMMAND_SUBTITLE 323
#define TOK_COMMAND_TEXT 324
#define TOK_SHAPE_COMMAND 325
#define TOK__NEVER__HAPPENS 326

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 94 "language.yy" /* yacc.c:355  */

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
    EArrowSymbol                   arrowsymbol;
    EBoxSymbol                     boxsymbol;
    EDividerSymbol                 dividersymbol;
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

#line 338 "language.cc" /* yacc.c:355  */
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



int yyparse (YYMSC_RESULT_TYPE &RESULT, void *yyscanner);

#endif /* !YY_YY_LANGUAGE_H_INCLUDED  */

/* Copy the second part of user declarations.  */
#line 181 "language.yy" /* yacc.c:358  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <iostream>

#ifdef C_S_H_IS_COMPILED

//#include "colorsyntax.h"
#include "language_misc.h"
#include "colorsyntax2.h"

//redefine default loc action for CSH
#define YYLLOC_DEFAULT(Current, Rhs, N)                     \
    do                                                      \
        if (N) {                                            \
            (Current).first_pos = YYRHSLOC(Rhs,1).first_pos;\
            (Current).last_pos  = YYRHSLOC(Rhs,N).last_pos; \
        } else {                                            \
            (Current).first_pos = (Current).last_pos =      \
                YYRHSLOC (Rhs, 0).last_pos;                 \
        }                                                   \
    while (0)

/* yyerror
 *  Error handling function.  Do nothing for CSH
 */
void yyerror(YYLTYPE* /*loc*/, Csh & /*csh*/, void * /*yyscanner*/, const char * /*str*/)
{}

#else

//#include "language.h"
#include "language_misc.h"
#include "language2.h"

/* Use verbose error reporting such that the expected token names are dumped */
//#define YYERROR_VERBOSE

#include "parse_tools.h"
#endif

#ifdef C_S_H_IS_COMPILED
void CshParse(YYMSC_RESULT_TYPE &RESULT, const char *buff, unsigned len)
#else
void MscParse(YYMSC_RESULT_TYPE &RESULT, const char *buff, unsigned len)
#endif
{
    parse_parm  pp;
    pp.buf = const_cast<char*>(buff);
    pp.length = len;
    pp.pos = 0;
    pp.RESULT = &RESULT;
    yylex_init(&pp.yyscanner);
    yyset_extra(&pp, pp.yyscanner);
    yyparse(RESULT, pp.yyscanner);
    yylex_destroy(pp.yyscanner);
}


#line 427 "language.cc" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  224
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1944

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  72
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  90
/* YYNRULES -- Number of rules.  */
#define YYNRULES  444
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  486

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   326

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   245,   245,   250,   251,   260,   272,   282,   295,   313,
     328,   340,   358,   359,   368,   382,   395,   414,   422,   433,
     444,   455,   466,   476,   488,   502,   503,   514,   529,   538,
     546,   560,   561,   562,   574,   590,   590,   592,   601,   625,
     643,   651,   663,   680,   700,   723,   742,   763,   786,   817,
     841,   866,   900,   918,   930,   945,   965,   980,   988,  1004,
    1020,  1033,  1046,  1066,  1085,  1096,  1103,  1110,  1119,  1132,
    1146,  1162,  1180,  1196,  1214,  1227,  1242,  1256,  1273,  1287,
    1304,  1324,  1339,  1357,  1377,  1394,  1411,  1426,  1443,  1463,
    1480,  1494,  1511,  1512,  1513,  1514,  1531,  1553,  1572,  1594,
    1611,  1634,  1634,  1636,  1637,  1663,  1685,  1702,  1703,  1714,
    1725,  1725,  1727,  1739,  1750,  1759,  1773,  1785,  1797,  1808,
    1818,  1833,  1833,  1833,  1833,  1835,  1845,  1864,  1877,  1878,
    1893,  1912,  1931,  1950,  1972,  1991,  2013,  2033,  2039,  2050,
    2062,  2065,  2081,  2093,  2109,  2122,  2140,  2155,  2173,  2189,
    2190,  2200,  2213,  2261,  2272,  2292,  2307,  2322,  2343,  2360,
    2370,  2381,  2398,  2413,  2430,  2442,  2455,  2466,  2474,  2490,
    2518,  2542,  2568,  2596,  2626,  2659,  2660,  2670,  2684,  2684,
    2686,  2708,  2731,  2749,  2766,  2790,  2814,  2825,  2826,  2837,
    2851,  2868,  2882,  2899,  2901,  2902,  2912,  2922,  2929,  2948,
    2967,  2984,  3006,  3028,  3045,  3064,  3073,  3089,  3100,  3117,
    3139,  3148,  3166,  3182,  3190,  3206,  3216,  3235,  3250,  3278,
    3286,  3299,  3307,  3322,  3340,  3359,  3374,  3396,  3407,  3423,
    3445,  3446,  3455,  3455,  3457,  3471,  3482,  3494,  3504,  3522,
    3542,  3562,  3583,  3602,  3623,  3663,  3685,  3707,  3731,  3755,
    3775,  3786,  3799,  3811,  3826,  3835,  3844,  3854,  3867,  3877,
    3888,  3896,  3909,  3910,  3913,  3924,  3939,  3957,  3965,  3974,
    3989,  4004,  4004,  4004,  4006,  4019,  4029,  4040,  4050,  4060,
    4073,  4083,  4094,  4104,  4113,  4126,  4136,  4147,  4157,  4166,
    4166,  4166,  4166,  4167,  4167,  4167,  4167,  4168,  4168,  4168,
    4168,  4170,  4170,  4171,  4171,  4172,  4172,  4174,  4184,  4195,
    4206,  4220,  4230,  4241,  4252,  4266,  4276,  4287,  4298,  4312,
    4321,  4335,  4349,  4366,  4375,  4389,  4403,  4420,  4429,  4443,
    4457,  4476,  4477,  4491,  4507,  4524,  4524,  4524,  4526,  4552,
    4582,  4618,  4642,  4669,  4692,  4716,  4737,  4755,  4763,  4777,
    4800,  4819,  4839,  4863,  4870,  4877,  4885,  4901,  4916,  4924,
    4933,  4941,  4949,  4961,  4969,  4972,  4982,  4992,  5003,  5015,
    5026,  5037,  5050,  5054,  5066,  5073,  5082,  5093,  5095,  5110,
    5125,  5140,  5154,  5168,  5173,  5185,  5199,  5209,  5216,  5217,
    5224,  5225,  5225,  5225,  5226,  5226,  5226,  5226,  5227,  5227,
    5227,  5227,  5227,  5227,  5228,  5228,  5228,  5228,  5228,  5229,
    5229,  5229,  5229,  5229,  5230,  5230,  5230,  5230,  5231,  5231,
    5231,  5232,  5232,  5234,  5235,  5236,  5237,  5238,  5239,  5240,
    5241,  5242,  5243,  5244,  5245,  5246,  5254,  5265,  5265,  5267,
    5267,  5267,  5269,  5284,  5286
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "TOK_EOF", "error", "$undefined", "TOK_STRING", "TOK_QSTRING",
  "TOK_NUMBER", "TOK_DASH", "TOK_EQUAL", "TOK_COMMA", "TOK_SEMICOLON",
  "TOK_PLUS", "TOK_PLUS_EQUAL", "TOK_OCBRACKET", "TOK_CCBRACKET",
  "TOK_OSBRACKET", "TOK_CSBRACKET", "TOK_ASTERISK", "TOK_MSC",
  "TOK_COLON_STRING", "TOK_COLON_QUOTED_STRING", "TOK_STYLE_NAME",
  "TOK_COLORDEF", "TOK_REL_SOLID_TO", "TOK_REL_SOLID_FROM",
  "TOK_REL_SOLID_BIDIR", "TOK_REL_DOUBLE_TO", "TOK_REL_DOUBLE_FROM",
  "TOK_REL_DOUBLE_BIDIR", "TOK_REL_DASHED_TO", "TOK_REL_DASHED_FROM",
  "TOK_REL_DASHED_BIDIR", "TOK_REL_DOTTED_TO", "TOK_REL_DOTTED_FROM",
  "TOK_REL_DOTTED_BIDIR", "TOK_SPECIAL_ARC", "TOK_EMPH",
  "TOK_EMPH_PLUS_PLUS", "TOK_COMMAND_HEADING", "TOK_COMMAND_NUDGE",
  "TOK_COMMAND_NEWPAGE", "TOK_COMMAND_DEFSHAPE", "TOK_COMMAND_DEFCOLOR",
  "TOK_COMMAND_DEFSTYLE", "TOK_COMMAND_DEFDESIGN", "TOK_COMMAND_BIG",
  "TOK_COMMAND_BOX", "TOK_COMMAND_PIPE", "TOK_COMMAND_MARK",
  "TOK_COMMAND_PARALLEL", "TOK_COMMAND_OVERLAP", "TOK_VERTICAL",
  "TOK_VERTICAL_SHAPE", "TOK_AT", "TOK_LOST", "TOK_AT_POS", "TOK_SHOW",
  "TOK_HIDE", "TOK_ACTIVATE", "TOK_DEACTIVATE", "TOK_BYE",
  "TOK_COMMAND_VSPACE", "TOK_COMMAND_HSPACE", "TOK_COMMAND_SYMBOL",
  "TOK_COMMAND_NOTE", "TOK_COMMAND_COMMENT", "TOK_COMMAND_ENDNOTE",
  "TOK_COMMAND_FOOTNOTE", "TOK_COMMAND_TITLE", "TOK_COMMAND_SUBTITLE",
  "TOK_COMMAND_TEXT", "TOK_SHAPE_COMMAND", "TOK__NEVER__HAPPENS",
  "$accept", "msc_with_bye", "eof", "msc", "top_level_arclist", "msckey",
  "braced_arclist", "arclist_maybe_no_semicolon", "arclist",
  "arc_with_parallel_semicolon", "overlap_or_parallel",
  "arc_with_parallel", "arc", "titlecommandtoken", "hspace_location",
  "full_arcattrlist_with_label_or_number", "dash_or_dashdash", "entityrel",
  "markerrel_no_string", "entity_command_prefixes", "optlist", "opt",
  "entitylist", "entity", "first_entity", "styledeflist", "styledef",
  "stylenamelist", "shapedef", "shapedeflist", "shapeline", "colordeflist",
  "color_string", "colordef", "designdef", "scope_open_empty",
  "designelementlist", "designelement", "designoptlist", "designopt",
  "parallel", "box_list", "first_box", "first_pipe",
  "pipe_list_no_content", "pipe_list", "emphrel", "boxrel", "vertxpos",
  "empharcrel_straight", "vertrel_no_xpos", "vertrel", "arcrel",
  "arcrel_arrow", "arcrel_to", "arcrel_from", "arcrel_bidir",
  "relation_to_no_loss", "relation_from_no_loss", "relation_bidir_no_loss",
  "relation_to_cont_no_loss", "relation_from_cont_no_loss",
  "relation_bidir_cont_no_loss", "relation_to", "relation_from",
  "relation_bidir", "relation_to_cont", "relation_from_cont",
  "relation_bidir_cont", "extvertxpos", "extvertxpos_no_string",
  "symbol_type_string", "symbol_command_no_attr", "symbol_command", "note",
  "comment_command", "comment", "colon_string",
  "full_arcattrlist_with_label", "full_arcattrlist", "arcattrlist",
  "arcattr", "vertical_shape", "entity_string", "reserved_word_string",
  "symbol_string", "alpha_string", "string", "scope_open", "scope_close", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326
};
# endif

#define YYPACT_NINF -380

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-380)))

#define YYTABLE_NINF -372

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     821,  -380,  -380,  -380,   -23,   479,   940,   308,  -380,  -380,
    -380,  -380,  -380,  -380,  -380,  -380,  -380,  -380,  -380,  -380,
    -380,  -380,  -380,  -380,  -380,    45,    45,    45,    48,  1637,
    1025,    61,  1823,    90,   119,  1093,  -380,  -380,  1755,  -380,
    -380,  -380,  -380,   178,   411,  1789,   215,  -380,  -380,  -380,
    -380,  -380,   256,  -380,    70,    53,    36,    68,  -380,    98,
     889,  -380,  1569,   289,  -380,   274,    48,   109,  -380,   113,
     161,  1859,  -380,  -380,   211,  -380,    48,   226,   274,    76,
    1639,  1636,   433,   118,   122,   125,    48,    48,    48,   274,
    -380,  -380,   274,  -380,    45,   333,  1884,   548,  -380,   136,
    -380,  -380,  -380,  -380,  -380,  -380,  -380,  -380,  -380,  -380,
    -380,  -380,  -380,  -380,  -380,  -380,  -380,  -380,  -380,  -380,
    -380,  -380,  -380,  -380,  -380,  -380,  -380,  -380,  -380,  -380,
    -380,  -380,  -380,  -380,  -380,  -380,  -380,  -380,  -380,  -380,
    -380,  -380,  -380,  -380,  -380,  -380,  -380,  -380,  -380,  -380,
     362,  -380,  -380,  -380,  -380,   149,   152,   145,   154,   163,
      53,  1161,  1501,  -380,  -380,  -380,  -380,  -380,   176,   187,
    -380,   195,  -380,   199,  -380,    21,  -380,  -380,   196,  -380,
     274,  1723,  -380,   150,  -380,   274,  -380,    45,   599,  -380,
    -380,    48,   194,  -380,    48,   159,   274,  -380,  -380,  -380,
    1705,  1908,   274,  -380,    45,  -380,    73,  -380,  -380,   155,
    -380,   178,  -380,    48,  -380,     7,  -380,    14,  -380,  -380,
     685,  -380,   274,  -380,  -380,  -380,   213,  -380,    53,  -380,
    -380,   889,    32,  -380,   427,  -380,   240,   164,  -380,   243,
     226,   313,    48,  -380,    68,  -380,   226,    68,  -380,   274,
    -380,  -380,    68,  -380,   159,  -380,   121,  -380,   245,    48,
    -380,   441,  -380,   252,    48,  -380,   270,  -380,   255,    48,
    -380,  -380,  -380,  -380,  -380,  -380,  -380,  -380,  -380,  -380,
      45,  1229,  -380,    48,    48,    48,    48,    68,  -380,  -380,
    -380,    56,  -380,  -380,   258,   616,  -380,   753,   209,  -380,
    -380,  -380,  -380,   287,  -380,    54,  1637,   957,  1025,  1297,
    -380,  -380,   401,  -380,  -380,  -380,   285,   222,  -380,   239,
    -380,  -380,   274,  -380,    48,  -380,  -380,    45,  -380,  -380,
    -380,    48,   159,    48,   202,   202,     7,  -380,   274,  -380,
    -380,  -380,  -380,  1501,   247,  -380,    48,  -380,  -380,  -380,
     304,   315,  -380,   226,  -380,  -380,    68,  -380,  -380,  -380,
    -380,   309,  -380,  -380,   312,  -380,  -380,   326,  -380,  -380,
    -380,  -380,  -380,  -380,  -380,  -380,  -380,  -380,  -380,    27,
    -380,  -380,  -380,  -380,  -380,  -380,  -380,  1365,  -380,  -380,
    -380,   283,   339,   167,   347,  -380,  1365,  -380,  -380,  -380,
    -380,    39,  1637,  1025,   348,  -380,   328,  -380,   351,  -380,
    -380,    19,   354,    48,  -380,  -380,  -380,   297,  -380,  -380,
    -380,  -380,   202,  -380,  -380,  -380,    48,  -380,  -380,  -380,
     315,    69,  -380,    68,  -380,  -380,  -380,  -380,  -380,  -380,
    -380,   364,   355,   369,  -380,   376,  -380,  -380,  1501,  1501,
     187,   199,   337,  -380,   160,  1433,  -380,   381,  -380,   383,
    -380,   202,  -380,  -380,  -380,  -380,  -380,   384,  -380,  -380,
    -380,  -380,   377,  -380,  -380,  -380,  -380,  -380,  -380,  -380,
    -380,   386,  -380,   387,   389,  -380
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       6,   388,   387,    33,   442,     0,     0,     0,   359,   358,
     289,   293,   297,   290,   294,   298,   291,   295,   299,   292,
     296,   300,   267,   233,   232,    76,    78,    90,    69,    71,
      73,    75,    42,   211,     0,    86,    35,    36,    45,   122,
     121,   123,   124,   100,    97,   346,     0,   353,   354,   355,
     101,   102,    82,   389,     0,     0,     0,     0,   205,    12,
      25,    28,    39,     0,    37,    81,    58,    64,   125,    57,
      67,    65,   210,   227,   230,    66,   237,   219,    40,   270,
     271,   272,   273,   307,   311,   315,     0,     0,     0,   347,
      92,    93,   357,    94,    53,    52,   146,     0,   443,   373,
     365,   390,   441,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   391,   392,   393,
     394,   395,   396,   397,   398,   399,   400,   401,   402,   403,
     404,   405,   406,   407,   408,   409,   410,   411,   412,   413,
     414,   415,   416,   417,   418,   419,   420,   421,   422,   372,
       0,   374,   437,   438,   440,   439,   384,   308,   312,   316,
       0,   134,   136,     7,    77,    79,    91,    68,   159,    70,
     175,   183,   155,    72,   149,   153,   439,   154,     0,    74,
      43,     0,   212,     0,   224,   223,    89,    87,     0,   386,
     385,   249,   250,   266,   260,   265,    46,   255,   254,   256,
      48,   261,   108,    99,   360,   107,   364,   110,   111,   105,
     106,    96,    98,   114,   103,   116,   337,   345,   335,   336,
     351,   350,    85,    83,     1,     3,     4,     2,     0,    10,
       8,    13,     0,    29,     0,    38,     0,    30,    80,    59,
     146,     0,    60,   207,   209,   216,   213,     0,   231,   228,
     235,   221,   220,    41,   269,   302,     0,   301,   319,   278,
     304,     0,   303,   323,   283,   306,     0,   305,   327,   288,
     309,   313,   317,   275,   280,   285,   348,   356,    54,   206,
      56,   132,   148,   236,   276,   281,   286,   145,    22,   444,
      24,     0,    18,   368,   370,   376,   366,   382,     0,   310,
     314,   318,     9,   133,   135,     0,   177,   182,   151,   156,
     152,   186,     0,    44,   226,    88,   251,   238,   252,   258,
     264,    49,    47,    51,   259,   109,   361,   363,   104,    95,
     113,   112,   334,   119,   343,   341,   344,   352,     0,    84,
       5,    11,    14,   134,    34,    31,    62,   129,   128,   126,
       0,    61,   137,   142,   208,   215,   214,   218,   229,   222,
     268,   320,   321,   277,   324,   325,   282,   328,   329,   287,
      55,   130,   131,   234,   274,   279,   284,   147,    21,     0,
      23,    17,   367,   377,   375,   381,   178,   380,   378,   179,
     383,     0,   168,     0,     0,   176,     0,   180,   150,   158,
     157,     0,   190,   192,     0,   187,     0,   194,     0,   253,
     239,   240,   242,   233,   244,   262,    50,   257,   362,   115,
     333,   118,   338,   331,   332,   342,   117,   349,   133,    32,
      63,   139,   144,   141,   217,   322,   326,   330,    20,    19,
     379,     0,   169,   167,   162,     0,   164,   181,   203,   204,
     189,   191,     0,   197,   196,   200,   241,   245,   243,   246,
     263,   339,   120,   140,   138,   143,   165,   170,   163,   166,
     201,   202,     0,   184,   188,   195,   198,   199,   247,   248,
     340,   171,   185,   172,   173,   174
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -380,  -380,   -54,  -380,   169,  -380,     8,   298,  -380,   341,
    -380,   346,   350,  -380,  -380,   -40,  -206,  -380,  -380,  -380,
    -380,   168,    64,   -12,   342,    17,   115,  -380,  -380,  -380,
    -379,    22,  -282,   127,  -380,  -380,  -380,   -18,  -380,   -17,
    -380,  -380,   402,  -380,  -380,  -380,   -35,    62,   -45,   237,
    -380,   248,   418,  -380,  -380,  -380,  -380,    10,     2,    -1,
     197,   198,   189,   -19,     3,    20,  -380,  -380,  -380,  -314,
     241,  -380,  -380,  -380,  -380,  -380,  -380,    47,   147,    75,
    -380,   173,  -380,     0,  -380,   417,     1,    -4,  -380,  -281
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    54,   227,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,   211,   203,   213,   214,   334,    66,
      67,    68,   351,   352,    69,   173,   174,   175,   167,   393,
     394,   169,   388,   170,   179,   312,   404,   405,   406,   407,
      70,    71,    72,    73,    74,    75,    76,    77,   193,   194,
     195,   196,    78,    79,    80,    81,    82,    83,    84,    85,
     258,   263,   268,    86,    87,    88,   259,   264,   269,   422,
     423,   217,    89,    90,    91,    92,    93,   204,   205,   206,
     150,   151,   200,   152,   153,   154,   176,   389,    97,   292
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      96,   156,   229,   192,   212,   159,   155,   222,   158,   331,
     381,   333,   441,   207,   445,   163,   157,     1,     2,   197,
     207,   425,     1,     2,   456,   397,   177,   438,   168,   309,
     171,   187,   181,   183,   183,     5,   225,   228,   201,   343,
     289,   198,   208,   162,   215,   218,   448,    94,    98,   208,
     449,     1,     2,   225,  -160,   391,   378,   379,   199,     5,
      96,   283,    96,  -160,   178,   230,   240,  -160,   332,   289,
     224,   183,     1,     2,   183,    95,   250,   284,   243,   245,
       4,   267,   248,   262,    53,   251,   273,   274,   275,    53,
     257,     8,     9,     1,     2,   226,   185,    96,   439,   285,
     164,   165,   166,   279,   282,   440,   302,    94,   461,    94,
     186,   231,   226,  -160,   447,   380,   286,   241,    53,  -225,
     184,   242,     1,     2,   392,    23,    24,   255,  -225,   254,
     426,  -225,  -225,   246,   270,    95,   249,    95,   271,    53,
     463,   272,   280,    10,    94,   244,    13,   480,   283,    16,
     320,   293,    19,   316,    23,    24,   297,   303,   304,   298,
      53,   299,   284,     1,     2,   192,   192,  -161,   443,   278,
     300,   329,    95,     4,   341,     5,  -161,   401,  -225,   301,
     444,   197,   197,   202,   285,    23,    24,   159,   305,    53,
     158,   317,     5,   221,   319,   306,     8,     9,   157,   223,
     201,   286,   307,   198,   198,     1,     2,   308,   311,   360,
     318,   191,   238,   330,     1,     2,   338,   336,   247,   328,
     199,   199,   340,     4,   252,   253,  -161,   410,   411,     5,
      53,    96,   412,     8,     9,   345,   276,   392,     4,   277,
       5,   350,   353,   287,     8,     9,    23,    24,   282,   344,
     310,   346,   354,   327,   355,   357,   332,   413,    24,   363,
     359,   362,   315,   262,   366,   267,   257,   220,   365,   369,
       5,   368,    53,   382,     8,     9,   265,   372,    94,   326,
     390,    53,   414,   373,   374,   375,   376,   420,     5,   -27,
     236,   156,     8,     9,    12,   377,   155,    15,   237,   -16,
      18,   409,   -27,    21,   177,   400,    95,   171,   191,   160,
     415,   281,   408,  -127,   347,   161,     1,     2,   429,   162,
       4,  -127,  -127,   431,   417,   435,  -127,   313,   436,   453,
     232,   419,   314,   421,   424,   424,   454,  -193,   472,   428,
       1,     2,   437,   321,   442,     4,   353,   323,   -27,   325,
     473,     8,     9,   392,   401,   370,   446,   452,   455,   458,
     467,   432,  -369,   294,   434,  -369,  -369,   337,   460,   339,
     295,  -369,  -127,   466,  -369,  -369,  -369,   296,   402,   403,
    -369,  -369,   468,    53,   348,   469,   478,   287,   479,   481,
     482,   483,   484,   356,   485,   291,   358,  -369,  -369,   177,
     342,   233,   418,   171,     1,     2,   234,    53,   239,   349,
     430,   457,   235,   459,     1,     2,   202,   207,   401,   464,
     451,  -369,   424,   398,   450,     5,   462,   -26,   236,     8,
       9,   353,  -369,   395,   474,   182,   237,   475,   324,   265,
     -26,   465,   402,   403,   470,   471,   208,   260,   322,   266,
     180,   477,   408,   361,   408,   367,     0,    12,   335,   364,
      15,   424,   219,    18,    11,   209,    21,    14,   384,   416,
      17,    53,     0,    20,     0,   210,     0,     0,     0,  -371,
      99,    53,     1,     2,     0,   427,   -26,  -371,  -371,     0,
       0,  -371,  -371,  -371,   100,     0,   101,  -371,  -371,   102,
     433,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,  -371,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   288,    53,
     149,     1,     2,     0,     0,     0,     0,     3,     0,     0,
       4,   289,     5,     0,     6,   232,     8,     9,     0,     0,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,     0,
       0,     0,     0,    39,    40,    41,    42,   290,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,     1,
       2,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,   101,    23,    24,   102,     0,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,     0,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,     0,    53,   383,     1,     2,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     5,
       0,     0,   101,     8,     9,   102,     0,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,     0,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,     0,    53,     1,     2,   385,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,   386,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   387,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,     0,    53,     1,     2,     0,     0,     0,     0,
       3,     0,     0,     4,     0,     5,     0,     6,     7,     8,
       9,     0,     0,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,     0,     0,     0,     0,    39,    40,    41,    42,
       0,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,     1,     2,     0,     0,     0,     0,     3,     0,
       0,     4,     0,     5,     0,     6,   232,     8,     9,     0,
       0,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
       0,     0,     0,     0,    39,    40,    41,    42,     0,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
       1,     2,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,   101,     0,     0,   102,   386,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   396,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,     0,    53,     1,     2,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,     0,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   172,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,     0,    53,     1,     2,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     5,     0,     0,
     101,     0,     0,   102,     0,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,     0,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,     0,    53,     1,     2,     0,     0,     0,     0,
       0,     0,     0,   -15,     0,     0,     0,     0,   101,     0,
       0,   102,     0,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,     0,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
       0,    53,     1,     2,   371,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
       0,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,     0,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,     0,    53,
       1,     2,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   101,     0,     0,   102,     0,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   399,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,     0,    53,     1,     2,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   386,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,     0,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,     0,    53,     1,     2,   476,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,     0,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,     0,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,     0,    53,     1,     2,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   102,     0,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,     0,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
       0,    53,     1,     2,     0,     0,     0,     0,     0,     0,
       0,     4,     0,     5,     0,     6,   232,     8,     9,     0,
       0,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,     0,     0,    38,
       0,     0,     0,     0,    39,    40,    41,    42,     0,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
       1,     2,   260,     0,     0,   255,     0,     0,     0,     0,
       0,     0,   261,     0,   101,   256,     0,     0,     0,    11,
       0,    10,    14,     0,    13,    17,     0,    16,    20,     0,
      19,     0,     0,     0,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,     0,    53,     1,     2,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     5,
       0,   188,     0,     8,     9,     0,     0,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,     6,
      23,    24,     0,     0,     0,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,   191,     1,     2,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   188,     0,     0,     0,    53,     0,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,     0,
      23,    24,     1,     2,     0,     0,     0,     0,     0,     0,
     189,     0,     0,     0,     0,     0,   190,   191,     0,     0,
       0,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,    53,     1,     2,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     6,
       0,     0,     0,     0,     0,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,   216,    53,
       0,     0,     1,     2,     0,     0,     0,     0,     0,     0,
       0,     4,     0,     5,     0,     0,     0,     8,     9,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   281,     0,    53,    23,    24,     4,     0,     5,     0,
       6,     0,     8,     9,     0,     0,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,     0,    23,
      24,     0,     0,     0,   188,     0,     0,     0,     0,    53,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,     0,    23,    24
};

static const yytype_int16 yycheck[] =
{
       0,     5,    56,    38,    44,     6,     5,    52,     6,   215,
     291,   217,   391,     6,   393,     7,     6,     3,     4,    38,
       6,   335,     3,     4,     5,   307,    30,     0,    28,     8,
      29,    35,    32,    33,    34,    14,     0,     1,    38,     7,
      13,    38,    35,    11,    44,    45,     7,     0,    71,    35,
      11,     3,     4,     0,     0,     1,     0,     1,    38,    14,
      60,    96,    62,     9,     3,    57,    66,    13,    54,    13,
       0,    71,     3,     4,    74,     0,    76,    96,    70,    71,
      12,    82,    74,    81,    70,    77,    86,    87,    88,    70,
      80,    18,    19,     3,     4,    59,    34,    97,   379,    96,
      25,    26,    27,    95,    96,   387,   160,    60,   422,    62,
      35,    13,    59,    59,   396,    59,    96,     8,    70,     0,
       1,     8,     3,     4,    70,    35,    36,     6,     9,    53,
     336,    12,    13,    71,    16,    60,    74,    62,    16,    70,
      71,    16,    95,    22,    97,    70,    25,   461,   183,    28,
     195,    15,    31,   188,    35,    36,     7,   161,   162,     7,
      70,    16,   181,     3,     4,   200,   201,     0,     1,    94,
      16,   211,    97,    12,   228,    14,     9,    17,    59,    16,
      13,   200,   201,     5,   181,    35,    36,   188,    12,    70,
     188,   191,    14,    46,   194,     8,    18,    19,   188,    52,
     200,   181,     7,   200,   201,     3,     4,     8,    12,   254,
      16,    52,    65,   213,     3,     4,   220,   217,    71,    64,
     200,   201,     9,    12,    77,    78,    59,     5,     6,    14,
      70,   231,    10,    18,    19,    71,    89,    70,    12,    92,
      14,   241,   242,    96,    18,    19,    35,    36,   240,     9,
     175,     8,   244,   206,   246,   247,    54,    35,    36,   259,
     252,    16,   187,   261,   264,   266,   256,    52,    16,   269,
      14,    16,    70,    15,    18,    19,     6,   281,   231,   204,
      71,    70,   317,   283,   284,   285,   286,   332,    14,     0,
       1,   295,    18,    19,    24,   287,   295,    27,     9,    12,
      30,    16,    13,    33,   308,   309,   231,   306,    52,     1,
      71,     7,   312,     0,     1,     7,     3,     4,    71,    11,
      12,     8,     9,     8,   324,    16,    13,   180,    16,     1,
      17,   331,   185,   333,   334,   335,     8,     9,     1,   343,
       3,     4,    16,   196,     5,    12,   346,   200,    59,   202,
      13,    18,    19,    70,    17,   280,     9,     9,     7,     5,
       5,   353,     0,     1,   356,     3,     4,   220,    71,   222,
       8,     9,    59,     9,    12,    13,    14,    15,    41,    42,
      18,    19,    13,    70,    71,     9,     5,   240,     5,     5,
      13,     5,     5,   246,     5,    97,   249,    35,    36,   403,
     231,    60,   327,   402,     3,     4,    60,    70,    66,   241,
     346,   411,    62,   413,     3,     4,     5,     6,    17,   431,
     403,    59,   422,   308,   402,    14,   426,     0,     1,    18,
      19,   431,    70,   306,   452,    33,     9,   454,   201,     6,
      13,   433,    41,    42,   448,   449,    35,     6,   200,    16,
      32,   455,   452,   256,   454,   266,    -1,    24,   217,   261,
      27,   461,    45,    30,    23,    54,    33,    26,   295,   322,
      29,    70,    -1,    32,    -1,    64,    -1,    -1,    -1,     0,
       1,    70,     3,     4,    -1,   338,    59,     8,     9,    -1,
      -1,    12,    13,    14,    15,    -1,    17,    18,    19,    20,
     353,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,     0,    70,
      71,     3,     4,    -1,    -1,    -1,    -1,     9,    -1,    -1,
      12,    13,    14,    -1,    16,    17,    18,    19,    -1,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    -1,
      -1,    -1,    -1,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,     3,
       4,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    17,    35,    36,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,    71,     3,     4,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    19,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    -1,    70,     3,     4,     5,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,     3,     4,    -1,    -1,    -1,    -1,
       9,    -1,    -1,    12,    -1,    14,    -1,    16,    17,    18,
      19,    -1,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    -1,    -1,    -1,    -1,    55,    56,    57,    58,
      -1,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,     3,     4,    -1,    -1,    -1,    -1,     9,    -1,
      -1,    12,    -1,    14,    -1,    16,    17,    18,    19,    -1,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      -1,    -1,    -1,    -1,    55,    56,    57,    58,    -1,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
       3,     4,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    17,    -1,    -1,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    -1,    70,     3,     4,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    -1,    70,     3,     4,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    -1,    -1,
      17,    -1,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,     3,     4,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,     3,     4,     5,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    -1,    70,
       3,     4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    -1,    70,     3,     4,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    -1,    70,     3,     4,     5,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,     3,     4,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,     3,     4,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    14,    -1,    16,    17,    18,    19,    -1,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    55,    56,    57,    58,    -1,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
       3,     4,     6,    -1,    -1,     6,    -1,    -1,    -1,    -1,
      -1,    -1,    16,    -1,    17,    16,    -1,    -1,    -1,    23,
      -1,    22,    26,    -1,    25,    29,    -1,    28,    32,    -1,
      31,    -1,    -1,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    -1,    70,     3,     4,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,
      -1,    16,    -1,    18,    19,    -1,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    16,
      35,    36,    -1,    -1,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    52,     3,     4,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    16,    -1,    -1,    -1,    70,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    -1,
      35,    36,     3,     4,    -1,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    -1,    -1,    -1,    51,    52,    -1,    -1,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    70,     3,     4,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    16,
      -1,    -1,    -1,    -1,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    69,    70,
      -1,    -1,     3,     4,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    14,    -1,    -1,    -1,    18,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     7,    -1,    70,    35,    36,    12,    -1,    14,    -1,
      16,    -1,    18,    19,    -1,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    -1,    35,
      36,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,    70,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    -1,    35,    36
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     9,    12,    14,    16,    17,    18,    19,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    55,
      56,    57,    58,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    73,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    91,    92,    93,    96,
     112,   113,   114,   115,   116,   117,   118,   119,   124,   125,
     126,   127,   128,   129,   130,   131,   135,   136,   137,   144,
     145,   146,   147,   148,   149,   151,   155,   160,    71,     1,
      15,    17,    20,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    71,
     152,   153,   155,   156,   157,   158,   159,   129,   130,   131,
       1,     7,    11,    78,   151,   151,   151,   100,   155,   103,
     105,   158,    36,    97,    98,    99,   158,   159,     3,   106,
     124,   155,   114,   155,     1,   119,   151,   159,    16,    45,
      51,    52,   118,   120,   121,   122,   123,   135,   136,   137,
     154,   155,     5,    87,   149,   150,   151,     6,    35,    54,
      64,    86,    87,    88,    89,   155,    69,   143,   155,   157,
      52,   150,   120,   150,     0,     0,    59,    74,     1,    74,
      78,    13,    17,    81,    83,    84,     1,     9,   150,    96,
     155,     8,     8,    78,   151,    78,   119,   150,    78,   119,
     155,    78,   150,   150,    53,     6,    16,   129,   132,   138,
       6,    16,   130,   133,   139,     6,    16,   131,   134,   140,
      16,    16,    16,   155,   155,   155,   150,   150,   151,    78,
     149,     7,    78,   118,   135,   136,   137,   150,     0,    13,
      59,    79,   161,    15,     1,     8,    15,     7,     7,    16,
      16,    16,    74,   159,   159,    12,     8,     7,     8,     8,
     151,    12,   107,   150,   150,   151,   118,   155,    16,   155,
     120,   150,   123,   150,   121,   150,   151,   149,    64,    87,
     155,    88,    54,    88,    90,   142,   155,   150,   159,   150,
       9,    74,    76,     7,     9,    71,     8,     1,    71,    93,
     155,    94,    95,   155,    78,    78,   150,    78,   150,    78,
     120,   132,    16,   155,   133,    16,   155,   134,    16,   155,
     151,     5,   159,   155,   155,   155,   155,    78,     0,     1,
      59,   161,    15,    71,   153,     5,    21,    36,   104,   159,
      71,     1,    70,   101,   102,   105,    36,   104,    98,    36,
     159,    17,    41,    42,   108,   109,   110,   111,   155,    16,
       5,     6,    10,    35,   118,    71,   150,   155,   151,   155,
     120,   155,   141,   142,   155,   141,    88,   150,   159,    71,
      94,     8,    78,   150,    78,    16,    16,    16,     0,   161,
     104,   102,     5,     1,    13,   102,     9,   104,     7,    11,
     103,    97,     9,     1,     8,     7,     5,   155,     5,   155,
      71,   141,   155,    71,    95,    78,     9,     5,    13,     9,
     159,   159,     1,    13,   109,   111,     5,   159,     5,     5,
     141,     5,    13,     5,     5,     5
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    72,    73,    74,    74,    74,    75,    75,    75,    75,
      75,    75,    76,    76,    76,    77,    77,    78,    78,    78,
      78,    78,    78,    78,    78,    79,    79,    79,    80,    80,
      81,    81,    81,    81,    81,    82,    82,    83,    83,    83,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    85,    85,    86,    86,    86,    86,    87,    87,    87,
      88,    88,    89,    89,    89,    89,    89,    90,    90,    90,
      90,    91,    91,    91,    91,    92,    92,    92,    92,    92,
      93,    93,    93,    93,    93,    93,    93,    94,    94,    94,
      94,    95,    95,    95,    95,    96,    96,    96,    96,    97,
      97,    97,    98,    98,    99,    99,    99,    99,    99,   100,
     100,   100,   100,   100,   101,   101,   101,   101,   102,   102,
     102,   102,   102,   102,   102,   103,   103,   103,   104,   104,
     105,   105,   105,   105,   106,   106,   107,   108,   108,   109,
     109,   109,   109,   109,   110,   110,   110,   110,   111,   111,
     111,   111,   111,   111,   111,   112,   112,   112,   112,   112,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   114,
     114,   114,   114,   115,   115,   115,   115,   116,   116,   116,
     117,   117,   118,   118,   119,   119,   119,   119,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     121,   121,   121,   121,   121,   121,   121,   122,   122,   122,
     122,   122,   122,   122,   123,   123,   123,   124,   124,   124,
     124,   125,   125,   125,   126,   126,   126,   126,   126,   127,
     127,   127,   127,   127,   128,   128,   128,   128,   128,   129,
     129,   129,   129,   130,   130,   130,   130,   131,   131,   131,
     131,   132,   132,   133,   133,   134,   134,   135,   135,   135,
     135,   136,   136,   136,   136,   137,   137,   137,   137,   138,
     138,   138,   138,   139,   139,   139,   139,   140,   140,   140,
     140,   141,   141,   142,   142,   143,   143,   143,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   145,   145,   146,
     146,   146,   146,   147,   147,   147,   148,   148,   149,   149,
     150,   150,   150,   150,   150,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   152,   152,   152,   152,   153,   153,
     153,   153,   153,   153,   153,   154,   154,   155,   155,   155,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   158,   158,   159,
     159,   159,   160,   160,   161
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     1,     2,     0,     2,     2,     3,
       2,     3,     1,     2,     3,     2,     3,     3,     2,     4,
       4,     3,     2,     3,     2,     1,     2,     1,     1,     2,
       2,     3,     4,     1,     3,     1,     1,     1,     2,     1,
       1,     2,     1,     2,     3,     1,     2,     3,     2,     3,
       4,     3,     1,     1,     2,     3,     2,     1,     1,     2,
       2,     3,     3,     4,     1,     1,     1,     1,     2,     1,
       2,     1,     2,     1,     2,     1,     1,     2,     1,     2,
       2,     1,     1,     2,     3,     2,     1,     2,     3,     2,
       1,     2,     1,     1,     1,     3,     2,     1,     2,     2,
       1,     1,     1,     1,     2,     1,     1,     1,     1,     2,
       1,     1,     2,     2,     1,     3,     1,     2,     2,     1,
       3,     1,     1,     1,     1,     1,     3,     2,     3,     3,
       3,     3,     2,     3,     2,     3,     2,     1,     3,     2,
       3,     2,     1,     3,     2,     2,     1,     3,     2,     1,
       3,     2,     2,     1,     1,     1,     2,     3,     3,     1,
       2,     3,     4,     5,     2,     3,     3,     2,     1,     2,
       3,     4,     5,     6,     7,     1,     3,     2,     1,     1,
       3,     4,     2,     1,     5,     6,     1,     1,     3,     2,
       1,     2,     1,     1,     1,     3,     2,     2,     3,     3,
       2,     3,     3,     2,     2,     1,     2,     2,     3,     2,
       1,     1,     2,     2,     3,     3,     2,     4,     3,     1,
       2,     2,     3,     2,     2,     1,     3,     1,     2,     3,
       1,     2,     1,     1,     3,     2,     2,     1,     2,     3,
       3,     4,     3,     4,     3,     4,     4,     5,     5,     1,
       1,     2,     2,     3,     1,     1,     1,     3,     2,     2,
       1,     1,     3,     4,     2,     1,     1,     1,     3,     2,
       1,     1,     1,     1,     3,     2,     2,     3,     2,     3,
       2,     2,     3,     2,     3,     2,     2,     3,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       3,     1,     2,     2,     3,     1,     2,     2,     3,     1,
       2,     2,     3,     1,     2,     2,     3,     1,     2,     2,
       3,     1,     1,     2,     1,     1,     1,     1,     4,     5,
       6,     3,     4,     3,     3,     2,     1,     1,     2,     4,
       2,     2,     3,     1,     1,     1,     2,     1,     1,     1,
       1,     2,     3,     2,     1,     2,     3,     4,     3,     2,
       3,     1,     2,     2,     1,     3,     2,     3,     3,     4,
       3,     3,     2,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (&yylloc, RESULT, yyscanner, YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location, RESULT, yyscanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, YYMSC_RESULT_TYPE &RESULT, void *yyscanner)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  YYUSE (RESULT);
  YYUSE (yyscanner);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, YYMSC_RESULT_TYPE &RESULT, void *yyscanner)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, RESULT, yyscanner);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, YYMSC_RESULT_TYPE &RESULT, void *yyscanner)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , RESULT, yyscanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, RESULT, yyscanner); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, YYMSC_RESULT_TYPE &RESULT, void *yyscanner)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (RESULT);
  YYUSE (yyscanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yytype)
    {
          case 3: /* TOK_STRING  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2051 "language.cc" /* yacc.c:1257  */
        break;

    case 4: /* TOK_QSTRING  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2057 "language.cc" /* yacc.c:1257  */
        break;

    case 5: /* TOK_NUMBER  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2063 "language.cc" /* yacc.c:1257  */
        break;

    case 17: /* TOK_MSC  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2069 "language.cc" /* yacc.c:1257  */
        break;

    case 18: /* TOK_COLON_STRING  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2075 "language.cc" /* yacc.c:1257  */
        break;

    case 19: /* TOK_COLON_QUOTED_STRING  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2081 "language.cc" /* yacc.c:1257  */
        break;

    case 20: /* TOK_STYLE_NAME  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2087 "language.cc" /* yacc.c:1257  */
        break;

    case 21: /* TOK_COLORDEF  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2093 "language.cc" /* yacc.c:1257  */
        break;

    case 37: /* TOK_COMMAND_HEADING  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2099 "language.cc" /* yacc.c:1257  */
        break;

    case 38: /* TOK_COMMAND_NUDGE  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2105 "language.cc" /* yacc.c:1257  */
        break;

    case 39: /* TOK_COMMAND_NEWPAGE  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2111 "language.cc" /* yacc.c:1257  */
        break;

    case 40: /* TOK_COMMAND_DEFSHAPE  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2117 "language.cc" /* yacc.c:1257  */
        break;

    case 41: /* TOK_COMMAND_DEFCOLOR  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2123 "language.cc" /* yacc.c:1257  */
        break;

    case 42: /* TOK_COMMAND_DEFSTYLE  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2129 "language.cc" /* yacc.c:1257  */
        break;

    case 43: /* TOK_COMMAND_DEFDESIGN  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2135 "language.cc" /* yacc.c:1257  */
        break;

    case 44: /* TOK_COMMAND_BIG  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2141 "language.cc" /* yacc.c:1257  */
        break;

    case 45: /* TOK_COMMAND_BOX  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2147 "language.cc" /* yacc.c:1257  */
        break;

    case 46: /* TOK_COMMAND_PIPE  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2153 "language.cc" /* yacc.c:1257  */
        break;

    case 47: /* TOK_COMMAND_MARK  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2159 "language.cc" /* yacc.c:1257  */
        break;

    case 48: /* TOK_COMMAND_PARALLEL  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2165 "language.cc" /* yacc.c:1257  */
        break;

    case 49: /* TOK_COMMAND_OVERLAP  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2171 "language.cc" /* yacc.c:1257  */
        break;

    case 50: /* TOK_VERTICAL  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2177 "language.cc" /* yacc.c:1257  */
        break;

    case 51: /* TOK_VERTICAL_SHAPE  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2183 "language.cc" /* yacc.c:1257  */
        break;

    case 52: /* TOK_AT  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2189 "language.cc" /* yacc.c:1257  */
        break;

    case 53: /* TOK_LOST  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2195 "language.cc" /* yacc.c:1257  */
        break;

    case 54: /* TOK_AT_POS  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2201 "language.cc" /* yacc.c:1257  */
        break;

    case 55: /* TOK_SHOW  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2207 "language.cc" /* yacc.c:1257  */
        break;

    case 56: /* TOK_HIDE  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2213 "language.cc" /* yacc.c:1257  */
        break;

    case 57: /* TOK_ACTIVATE  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2219 "language.cc" /* yacc.c:1257  */
        break;

    case 58: /* TOK_DEACTIVATE  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2225 "language.cc" /* yacc.c:1257  */
        break;

    case 59: /* TOK_BYE  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2231 "language.cc" /* yacc.c:1257  */
        break;

    case 60: /* TOK_COMMAND_VSPACE  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2237 "language.cc" /* yacc.c:1257  */
        break;

    case 61: /* TOK_COMMAND_HSPACE  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2243 "language.cc" /* yacc.c:1257  */
        break;

    case 62: /* TOK_COMMAND_SYMBOL  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2249 "language.cc" /* yacc.c:1257  */
        break;

    case 63: /* TOK_COMMAND_NOTE  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2255 "language.cc" /* yacc.c:1257  */
        break;

    case 64: /* TOK_COMMAND_COMMENT  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2261 "language.cc" /* yacc.c:1257  */
        break;

    case 65: /* TOK_COMMAND_ENDNOTE  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2267 "language.cc" /* yacc.c:1257  */
        break;

    case 66: /* TOK_COMMAND_FOOTNOTE  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2273 "language.cc" /* yacc.c:1257  */
        break;

    case 67: /* TOK_COMMAND_TITLE  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2279 "language.cc" /* yacc.c:1257  */
        break;

    case 68: /* TOK_COMMAND_SUBTITLE  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2285 "language.cc" /* yacc.c:1257  */
        break;

    case 69: /* TOK_COMMAND_TEXT  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2291 "language.cc" /* yacc.c:1257  */
        break;

    case 76: /* top_level_arclist  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arclist);}
#line 2297 "language.cc" /* yacc.c:1257  */
        break;

    case 78: /* braced_arclist  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arclist);}
#line 2303 "language.cc" /* yacc.c:1257  */
        break;

    case 79: /* arclist_maybe_no_semicolon  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arclist);}
#line 2309 "language.cc" /* yacc.c:1257  */
        break;

    case 80: /* arclist  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arclist);}
#line 2315 "language.cc" /* yacc.c:1257  */
        break;

    case 81: /* arc_with_parallel_semicolon  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2321 "language.cc" /* yacc.c:1257  */
        break;

    case 82: /* overlap_or_parallel  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2327 "language.cc" /* yacc.c:1257  */
        break;

    case 83: /* arc_with_parallel  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2333 "language.cc" /* yacc.c:1257  */
        break;

    case 84: /* arc  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2339 "language.cc" /* yacc.c:1257  */
        break;

    case 85: /* titlecommandtoken  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2345 "language.cc" /* yacc.c:1257  */
        break;

    case 86: /* hspace_location  */
#line 178 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).namerel);}
#line 2351 "language.cc" /* yacc.c:1257  */
        break;

    case 87: /* full_arcattrlist_with_label_or_number  */
#line 177 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).attriblist);}
#line 2357 "language.cc" /* yacc.c:1257  */
        break;

    case 89: /* entityrel  */
#line 178 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).namerel);}
#line 2363 "language.cc" /* yacc.c:1257  */
        break;

    case 90: /* markerrel_no_string  */
#line 178 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).namerel);}
#line 2369 "language.cc" /* yacc.c:1257  */
        break;

    case 91: /* entity_command_prefixes  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2375 "language.cc" /* yacc.c:1257  */
        break;

    case 92: /* optlist  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arclist);}
#line 2381 "language.cc" /* yacc.c:1257  */
        break;

    case 93: /* opt  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2387 "language.cc" /* yacc.c:1257  */
        break;

    case 94: /* entitylist  */
#line 177 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).entitylist);}
#line 2393 "language.cc" /* yacc.c:1257  */
        break;

    case 95: /* entity  */
#line 177 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).entitylist);}
#line 2399 "language.cc" /* yacc.c:1257  */
        break;

    case 96: /* first_entity  */
#line 177 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).entitylist);}
#line 2405 "language.cc" /* yacc.c:1257  */
        break;

    case 99: /* stylenamelist  */
#line 178 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).stringlist);}
#line 2411 "language.cc" /* yacc.c:1257  */
        break;

    case 104: /* color_string  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2417 "language.cc" /* yacc.c:1257  */
        break;

    case 112: /* parallel  */
#line 176 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcparallel);}
#line 2423 "language.cc" /* yacc.c:1257  */
        break;

    case 113: /* box_list  */
#line 176 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcboxseries);}
#line 2429 "language.cc" /* yacc.c:1257  */
        break;

    case 114: /* first_box  */
#line 176 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbox);}
#line 2435 "language.cc" /* yacc.c:1257  */
        break;

    case 115: /* first_pipe  */
#line 176 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcpipe);}
#line 2441 "language.cc" /* yacc.c:1257  */
        break;

    case 116: /* pipe_list_no_content  */
#line 176 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcpipeseries);}
#line 2447 "language.cc" /* yacc.c:1257  */
        break;

    case 117: /* pipe_list  */
#line 176 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcpipeseries);}
#line 2453 "language.cc" /* yacc.c:1257  */
        break;

    case 119: /* boxrel  */
#line 176 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbox);}
#line 2459 "language.cc" /* yacc.c:1257  */
        break;

    case 120: /* vertxpos  */
#line 178 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).vertxpos);}
#line 2465 "language.cc" /* yacc.c:1257  */
        break;

    case 122: /* vertrel_no_xpos  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcvertarrow);}
#line 2471 "language.cc" /* yacc.c:1257  */
        break;

    case 123: /* vertrel  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcvertarrow);}
#line 2477 "language.cc" /* yacc.c:1257  */
        break;

    case 124: /* arcrel  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2483 "language.cc" /* yacc.c:1257  */
        break;

    case 125: /* arcrel_arrow  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcarrow);}
#line 2489 "language.cc" /* yacc.c:1257  */
        break;

    case 126: /* arcrel_to  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcarrow);}
#line 2495 "language.cc" /* yacc.c:1257  */
        break;

    case 127: /* arcrel_from  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcarrow);}
#line 2501 "language.cc" /* yacc.c:1257  */
        break;

    case 128: /* arcrel_bidir  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcarrow);}
#line 2507 "language.cc" /* yacc.c:1257  */
        break;

    case 141: /* extvertxpos  */
#line 178 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).extvertxpos);}
#line 2513 "language.cc" /* yacc.c:1257  */
        break;

    case 142: /* extvertxpos_no_string  */
#line 178 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).extvertxpos);}
#line 2519 "language.cc" /* yacc.c:1257  */
        break;

    case 143: /* symbol_type_string  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2525 "language.cc" /* yacc.c:1257  */
        break;

    case 144: /* symbol_command_no_attr  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2531 "language.cc" /* yacc.c:1257  */
        break;

    case 145: /* symbol_command  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2537 "language.cc" /* yacc.c:1257  */
        break;

    case 146: /* note  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2543 "language.cc" /* yacc.c:1257  */
        break;

    case 148: /* comment  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2549 "language.cc" /* yacc.c:1257  */
        break;

    case 149: /* colon_string  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2555 "language.cc" /* yacc.c:1257  */
        break;

    case 150: /* full_arcattrlist_with_label  */
#line 177 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).attriblist);}
#line 2561 "language.cc" /* yacc.c:1257  */
        break;

    case 151: /* full_arcattrlist  */
#line 177 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).attriblist);}
#line 2567 "language.cc" /* yacc.c:1257  */
        break;

    case 152: /* arcattrlist  */
#line 177 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).attriblist);}
#line 2573 "language.cc" /* yacc.c:1257  */
        break;

    case 153: /* arcattr  */
#line 177 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).attrib);}
#line 2579 "language.cc" /* yacc.c:1257  */
        break;

    case 155: /* entity_string  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2585 "language.cc" /* yacc.c:1257  */
        break;

    case 156: /* reserved_word_string  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2591 "language.cc" /* yacc.c:1257  */
        break;

    case 157: /* symbol_string  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2597 "language.cc" /* yacc.c:1257  */
        break;

    case 158: /* alpha_string  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2603 "language.cc" /* yacc.c:1257  */
        break;

    case 159: /* string  */
#line 179 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2609 "language.cc" /* yacc.c:1257  */
        break;

    case 161: /* scope_close  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2615 "language.cc" /* yacc.c:1257  */
        break;


      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (YYMSC_RESULT_TYPE &RESULT, void *yyscanner)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

/* User initialization code.  */
#line 7 "language.yy" /* yacc.c:1429  */
{
  #ifdef C_S_H_IS_COMPILED
    yylloc.first_pos = 0;
    yylloc.last_pos = 0;
  #endif
}

#line 2731 "language.cc" /* yacc.c:1429  */
  yylsp[0] = yylloc;
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex (&yylval, &yylloc, yyscanner);
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 246 "language.yy" /* yacc.c:1646  */
    {
	YYACCEPT;
}
#line 2922 "language.cc" /* yacc.c:1646  */
    break;

  case 4:
#line 252 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
	csh.AddCSH_AllCommentBeyond((yylsp[0]));
  #else
  #endif
    free((yyvsp[0].str));
}
#line 2935 "language.cc" /* yacc.c:1646  */
    break;

  case 5:
#line 261 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[0]), COLOR_SEMICOLON);
	csh.AddCSH_AllCommentBeyond((yylsp[0]));
  #else
  #endif
    free((yyvsp[-1].str));
}
#line 2949 "language.cc" /* yacc.c:1646  */
    break;

  case 6:
#line 272 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddLineBeginToHints();
    csh.hintStatus = HINT_READY;
    csh.hintSource = EHintSourceType::LINE_START;
    csh.hintsForcedOnly = true;
  #else
    //no action for empty file
  #endif
}
#line 2964 "language.cc" /* yacc.c:1646  */
    break;

  case 7:
#line 283 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckLineStartHintBefore((yylsp[-1])) || csh.CheckLineStartHintAt((yylsp[-1]))) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
  #else
    msc.AddArcs((yyvsp[0].arclist));
  #endif
    free((yyvsp[-1].str));
}
#line 2981 "language.cc" /* yacc.c:1646  */
    break;

  case 8:
#line 296 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckLineStartHintBefore((yylsp[-1]))) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[-1])))
        csh.AddDesignsToHints(true);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[-1])))
        csh.AddOptionsToHints();
    else if (csh.CheckHintLocated(EHintSourceType::LINE_START, (yylsp[-1]))) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.AddArcs((yyvsp[0].arclist));
  #endif
}
#line 3003 "language.cc" /* yacc.c:1646  */
    break;

  case 9:
#line 314 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckLineStartHintBefore((yylsp[-2])) || csh.CheckLineStartHintAt((yylsp[-2]))) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.AddCSH_Error((yylsp[-1]), "Missing an equal sign or a list of elements between braces ('{' and '}').");
  #else
    msc.Error.Error(MSC_POS((yylsp[-1])).start, "Missing an equal sign or a list of elements between braces ('{' and '}').");
  #endif
    free((yyvsp[-2].str));
	YYACCEPT; //We should noty parse further in msc_with_bye as we may have something beyond BYE (in eof)
}
#line 3022 "language.cc" /* yacc.c:1646  */
    break;

  case 10:
#line 329 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckLineStartHintBefore((yylsp[-1]))) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
#else
    msc.AddArcs((yyvsp[-1].arclist));
  #endif
	YYACCEPT; //We should noty parse further in msc_with_bye as we may have something beyond BYE (in eof)
}
#line 3038 "language.cc" /* yacc.c:1646  */
    break;

  case 11:
#line 341 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckLineStartHintBefore((yylsp[-2]))) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
    CshPos pos = (yylsp[-1]);
    if (((yylsp[-2])).last_pos >= ((yylsp[-1])).first_pos)
        pos.first_pos = ((yylsp[-2])).last_pos;
    csh.AddCSH_Error(pos, "Could not recognize this as a valid line.");
  #else
    msc.AddArcs((yyvsp[-2].arclist));
    msc.Error.Error(MSC_POS((yylsp[-1])).start, "Could not recognize this as a valid line.");
  #endif
	YYACCEPT; //We should noty parse further in msc_with_bye as we may have something beyond BYE (in eof)
}
#line 3059 "language.cc" /* yacc.c:1646  */
    break;

  case 13:
#line 360 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[0]), "Closing brace missing its opening pair.");
  #else
    (yyval.arclist) = (yyvsp[-1].arclist);
    msc.Error.Error(MSC_POS((yylsp[0])).start, "Unexpected '}'.");
  #endif
}
#line 3072 "language.cc" /* yacc.c:1646  */
    break;

  case 14:
#line 369 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[-1]), "Closing brace missing its opening pair.");
  #else
    //Merge $3 into $1
    ((yyvsp[-2].arclist))->splice(((yyvsp[-2].arclist))->end(), *((yyvsp[0].arclist)));
    delete ((yyvsp[0].arclist));
    (yyval.arclist) = (yyvsp[-2].arclist);
    msc.Error.Error(MSC_POS((yylsp[0])).start, "Unexpected '}'.");
  #endif
}
#line 3088 "language.cc" /* yacc.c:1646  */
    break;

  case 15:
#line 383 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[0]), COLOR_EQUAL);
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing a design name.");
    csh.CheckHintAt((yylsp[-1]), EHintSourceType::ATTR_NAME);
    csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::ATTR_VALUE, "msc");
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing design name.");
  #endif
    free((yyvsp[-1].str));
}
#line 3105 "language.cc" /* yacc.c:1646  */
    break;

  case 16:
#line 396 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[-1]), COLOR_EQUAL);
    csh.AddCSH((yylsp[0]), COLOR_DESIGNNAME);
    csh.CheckHintAt((yylsp[-2]), EHintSourceType::ATTR_NAME);
    csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_VALUE, "msc");
    std::string msg = csh.SetDesignTo((yyvsp[0].str), true);
    if (msg.length())
        csh.AddCSH_Error((yylsp[0]), msg.c_str());
  #else
    ArcBase *dummy = msc.AddAttribute(Attribute("msc", (yyvsp[0].str), MSC_POS((yylsp[-2])), MSC_POS((yylsp[0]))));
    if (dummy) delete dummy;
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 3127 "language.cc" /* yacc.c:1646  */
    break;

  case 17:
#line 415 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    if ((yyvsp[0].arcbase)) ((yyvsp[-1].arclist))->Append((yyvsp[0].arcbase)); //Append any potential CommandNumbering
    (yyval.arclist) = (yyvsp[-1].arclist);
  #endif
}
#line 3139 "language.cc" /* yacc.c:1646  */
    break;

  case 18:
#line 423 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arclist) = new ArcList;
    //scope_close should not return here with a CommandNumbering
    //but just in case
    if ((yyvsp[0].arcbase))
        delete((yyvsp[0].arcbase));
  #endif
}
#line 3154 "language.cc" /* yacc.c:1646  */
    break;

  case 19:
#line 434 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[-1]), "Could not recognize this as a valid line.");
  #else
    if ((yyvsp[0].arcbase)) ((yyvsp[-2].arclist))->Append((yyvsp[0].arcbase)); //Append any potential CommandNumbering
    (yyval.arclist) = (yyvsp[-2].arclist);
    msc.Error.Error(MSC_POS((yylsp[-1])).start, "Syntax error.");
  #endif
    yyerrok;
}
#line 3169 "language.cc" /* yacc.c:1646  */
    break;

  case 20:
#line 445 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[-1]), "Could not recognize this as a valid line.");
  #else
    (yyval.arclist) = (yyvsp[-2].arclist);
    //Do not pop context, as the missing scope_close would have done
    msc.Error.Error(MSC_POS((yylsp[-1])).start, "Missing '}'.");
    msc.Error.Error(MSC_POS((yylsp[-3])).start, MSC_POS((yylsp[-1])).start, "Here is the corresponding '{'.");
  #endif
}
#line 3184 "language.cc" /* yacc.c:1646  */
    break;

  case 21:
#line 456 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ErrorAfter((yylsp[-1]), "Missing a closing brace ('}').");
  #else
    (yyval.arclist) = (yyvsp[-1].arclist);
    //Do not pop context, as the missing scope_close would have done
    msc.Error.Error(MSC_POS((yylsp[-1])).end.NextChar(), "Missing '}'.");
    msc.Error.Error(MSC_POS((yylsp[-2])).start, MSC_POS((yylsp[-1])).end.NextChar(), "Here is the corresponding '{'.");
  #endif
}
#line 3199 "language.cc" /* yacc.c:1646  */
    break;

  case 22:
#line 467 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ErrorAfter((yylsp[-1]), "Missing a closing brace ('}').");
  #else
    (yyval.arclist) = NULL;
    //Do not pop context, as the missing scope_close would have done
    msc.Error.Error(MSC_POS((yylsp[-1])).end.NextChar(), "Missing a corresponding '}'.");
  #endif
}
#line 3213 "language.cc" /* yacc.c:1646  */
    break;

  case 23:
#line 477 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[0]), "The command 'bye' can only be used at the top level.");
  #else
    (yyval.arclist) = (yyvsp[-1].arclist);
    //Do not pop context, as the missing scope_close would have done
    msc.Error.Error(MSC_POS((yylsp[0])).start, "The command 'bye' can not be used between curly braces '{' and '}'.");
    msc.Error.Error(MSC_POS((yylsp[-2])).start, MSC_POS((yylsp[0])).start, "Here is the opening '{'.");
  #endif
  free((yyvsp[0].str));
}
#line 3229 "language.cc" /* yacc.c:1646  */
    break;

  case 24:
#line 489 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[0]), "The command 'bye' can only be used at the top level and not inside curly braces '{' and '}'.");
  #else
    (yyval.arclist) = NULL;
    //Do not pop context, as the missing scope_close would have done
    msc.Error.Error(MSC_POS((yylsp[0])).start, "The command 'bye' can not be used between curly braces '{' and '}'.");
  #endif
  free((yyvsp[0].str));
}
#line 3244 "language.cc" /* yacc.c:1646  */
    break;

  case 26:
#line 504 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing a semicolon (';').");
  #else
    if ((yyvsp[0].arcbase)) ((yyvsp[-1].arclist))->Append((yyvsp[0].arcbase));
    (yyval.arclist) = (yyvsp[-1].arclist);
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing a semicolon ';'.");
    msc.Error.Error(MSC_POS((yylsp[0])).start, MSC_POS((yylsp[0])).end.NextChar(), "Here is the beginning of the command as I understood it.");
  #endif
}
#line 3259 "language.cc" /* yacc.c:1646  */
    break;

  case 27:
#line 515 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing a semicolon (';').");
  #else
    if ((yyvsp[0].arcbase))
        (yyval.arclist) = (new ArcList)->Append((yyvsp[0].arcbase)); /* New list */
    else
        (yyval.arclist) = new ArcList;
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing a semicolon ';'.");
    msc.Error.Error(MSC_POS((yylsp[0])).start, MSC_POS((yylsp[0])).end.NextChar(), "Here is the beginning of the command as I understood it.");
  #endif
}
#line 3276 "language.cc" /* yacc.c:1646  */
    break;

  case 28:
#line 530 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[0].arcbase))
        (yyval.arclist) = (new ArcList)->Append((yyvsp[0].arcbase)); /* New list */
    else
        (yyval.arclist) = new ArcList;
  #endif
}
#line 3289 "language.cc" /* yacc.c:1646  */
    break;

  case 29:
#line 539 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[0].arcbase)) ((yyvsp[-1].arclist))->Append((yyvsp[0].arcbase));     /* Add to existing list */
    (yyval.arclist) = ((yyvsp[-1].arclist));
  #endif
}
#line 3300 "language.cc" /* yacc.c:1646  */
    break;

  case 30:
#line 547 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SEMICOLON);
    if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::LINE_START)) {
       csh.AddLineBeginToHints();
       csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.arcbase)=(yyvsp[-1].arcbase);
  #endif
}
#line 3316 "language.cc" /* yacc.c:1646  */
    break;

  case 33:
#line 563 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SEMICOLON);
    if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.arcbase)=NULL;
  #endif
}
#line 3332 "language.cc" /* yacc.c:1646  */
    break;

  case 34:
#line 575 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SEMICOLON);
    csh.AddCSH_Error((yylsp[-1]), "I am not sure what is coming here.");
    if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::LINE_START)) {
       csh.AddLineBeginToHints();
       csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.arcbase)=(yyvsp[-2].arcbase);
    msc.Error.Error(MSC_POS((yylsp[-2])).end.NextChar(), "Missing a semicolon ';'.");
    msc.Error.Error(MSC_POS((yylsp[-2])).start, MSC_POS((yylsp[-2])).end.NextChar(), "Here is the beginning of the command as I understood it.");
  #endif
}
#line 3351 "language.cc" /* yacc.c:1646  */
    break;

  case 37:
#line 593 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    if ((yyvsp[0].arcbase)) 
		((yyvsp[0].arcbase))->SetLineEnd(MSC_POS((yyloc)));
    (yyval.arcbase)=(yyvsp[0].arcbase);
  #endif
}
#line 3364 "language.cc" /* yacc.c:1646  */
    break;

  case 38:
#line 602 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    }
  #else
    if ((yyvsp[0].arcbase)) {
        ((yyvsp[0].arcbase))->SetLineEnd(MSC_POS((yyloc)));
        if (CaseInsensitiveEqual((yyvsp[-1].str), "parallel"))
            ((yyvsp[0].arcbase))->SetParallel();
        else 
            ((yyvsp[0].arcbase))->SetOverlap();
    }
    (yyval.arcbase) = (yyvsp[0].arcbase);
  #endif
    free((yyvsp[-1].str));
}
#line 3392 "language.cc" /* yacc.c:1646  */
    break;

  case 39:
#line 626 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    }
  #else
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[0].str));
}
#line 3413 "language.cc" /* yacc.c:1646  */
    break;

  case 40:
#line 644 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    ((yyvsp[0].arcbase))->AddAttributeList(NULL);
    (yyval.arcbase)=((yyvsp[0].arcbase));
  #endif
}
#line 3425 "language.cc" /* yacc.c:1646  */
    break;

  case 41:
#line 652 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0])))
        ArcArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0])))
        ArcArrow::AttributeValues(csh.hintAttrName, csh);
  #else
    ((yyvsp[-1].arcbase))->AddAttributeList((yyvsp[0].attriblist));
    (yyval.arcbase) = ((yyvsp[-1].arcbase));
  #endif
}
#line 3441 "language.cc" /* yacc.c:1646  */
    break;

  case 42:
#line 664 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    } else
        csh.CheckEntityHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF);
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing an arrow specification.");
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing an arrow specification.");
  #endif
    free((yyvsp[0].str));
    (yyval.arcbase) = NULL;
}
#line 3462 "language.cc" /* yacc.c:1646  */
    break;

  case 43:
#line 681 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    } else
        csh.CheckEntityHintAtAndBeforePlusOne((yylsp[-1]), (yylsp[0]));
  #else
    //Returns NULL, if BIG is before a self-pointing arrow
    ArcBase *arc = msc.CreateArcBigArrow((yyvsp[0].arcbase));
    if (arc)
        arc->AddAttributeList(NULL);
    delete (yyvsp[0].arcbase);
    (yyval.arcbase) = arc;
  #endif
    free((yyvsp[-1].str));
}
#line 3486 "language.cc" /* yacc.c:1646  */
    break;

  case 44:
#line 701 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-2]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0])))
        ArcBigArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0])))
        ArcBigArrow::AttributeValues(csh.hintAttrName, csh);
    else 
        csh.CheckEntityHintAtAndBeforePlusOne((yylsp[-2]), (yylsp[-1]));
  #else
    //Returns NULL, if BIG is before a self-pointing arrow
    ArcBigArrow *arrow = msc.CreateArcBigArrow((yyvsp[-1].arcbase));
    if (arrow) arrow->AddAttributeList((yyvsp[0].attriblist));
    (yyval.arcbase) = arrow;
    delete (yyvsp[-1].arcbase);
  #endif
    free((yyvsp[-2].str));
}
#line 3513 "language.cc" /* yacc.c:1646  */
    break;

  case 45:
#line 724 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    } else if (csh.CheckHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddVerticalTypesToHints();
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing a marker or one of 'brace', 'bracket', 'range', 'box' or an arrow or box symbol, such as '->' or '--'.");
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing a marker or one of 'brace', 'bracket', 'range', 'box' or an arrow or box symbol, such as '->' or '--'.");
  #endif
    free((yyvsp[0].str));
    (yyval.arcbase) = NULL;
}
#line 3536 "language.cc" /* yacc.c:1646  */
    break;

  case 46:
#line 743 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    } else if (csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::KEYWORD)) {
        csh.AddVerticalTypesToHints();
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
  #else
    if ((yyvsp[0].arcvertarrow)) {
      ((yyvsp[0].arcvertarrow))->SetVerticalShape(ArcVerticalArrow::ARROW_OR_BOX);
      ((yyvsp[0].arcvertarrow))->AddAttributeList(NULL);
      (yyval.arcbase) = ((yyvsp[0].arcvertarrow));
    } else (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[-1].str));
}
#line 3561 "language.cc" /* yacc.c:1646  */
    break;

  case 47:
#line 764 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-2]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    } else if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::KEYWORD)) {
        csh.AddVerticalTypesToHints();
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    }
#else
    if ((yyvsp[0].arcvertarrow)) {
      ((yyvsp[0].arcvertarrow))->SetVerticalShape((yyvsp[-1].vshape));
      ((yyvsp[0].arcvertarrow))->AddAttributeList(NULL);
      (yyval.arcbase) = ((yyvsp[0].arcvertarrow));
    } else (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[-2].str));
}
#line 3588 "language.cc" /* yacc.c:1646  */
    break;

  case 48:
#line 787 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    } else if (csh.CheckHintAt((yylsp[0]), EHintSourceType::KEYWORD)) {
        csh.AddVerticalTypesToHints();
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    } else if (csh.CheckHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::MARKER)) {
        csh.hintStatus = HINT_FILLING;
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
    }
#else
	ArcTypePlusDir typeplusdir;
	typeplusdir.arc.type = EArrowSymbol::SOLID;
	typeplusdir.arc.lost = EArrowLost::NOT;
	typeplusdir.dir = EDirType::RIGHT;
	ArcVerticalArrow *ava = new ArcVerticalArrow(typeplusdir, MARKER_HERE_STR, MARKER_HERE_STR, &msc);
	VertXPos vxp(msc);
	ava->AddXpos(&vxp);
    ava->SetVerticalShape((yyvsp[0].vshape));
    ava->AddAttributeList(NULL);
	(yyval.arcbase) = ava;
  #endif
    free((yyvsp[-1].str));
}
#line 3623 "language.cc" /* yacc.c:1646  */
    break;

  case 49:
#line 818 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-2]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    } else if (csh.CheckHintBetween((yylsp[-2]), (yylsp[-1]), EHintSourceType::KEYWORD)) {
        csh.AddVerticalTypesToHints();
        csh.hintStatus = HINT_READY;
    } if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0])))
        ArcVerticalArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0])))
        ArcVerticalArrow::AttributeValues(csh.hintAttrName, csh);
  #else
    if ((yyvsp[-1].arcvertarrow)) {
      ((yyvsp[-1].arcvertarrow))->SetVerticalShape(ArcVerticalArrow::ARROW_OR_BOX);
      ((yyvsp[-1].arcvertarrow))->AddAttributeList((yyvsp[0].attriblist));
      (yyval.arcbase) = ((yyvsp[-1].arcvertarrow));
    } else (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[-2].str));
}
#line 3651 "language.cc" /* yacc.c:1646  */
    break;

  case 50:
#line 842 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-3]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-3]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAt((yylsp[-2]), EHintSourceType::KEYWORD)) {
        csh.AddVerticalTypesToHints();
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0])))
        ArcVerticalArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0])))
        ArcVerticalArrow::AttributeValues(csh.hintAttrName, csh);
  #else
    if ((yyvsp[-1].arcvertarrow)) {
      ((yyvsp[-1].arcvertarrow))->SetVerticalShape((yyvsp[-2].vshape));
      ((yyvsp[-1].arcvertarrow))->AddAttributeList((yyvsp[0].attriblist));
      (yyval.arcbase) = ((yyvsp[-1].arcvertarrow));
    } else (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[-3].str));
}
#line 3680 "language.cc" /* yacc.c:1646  */
    break;

  case 51:
#line 867 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-2]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    } else if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::KEYWORD)) {
        csh.AddVerticalTypesToHints();
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    } else if (csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::MARKER)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_FILLING;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0])))
        ArcVerticalArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0])))
        ArcVerticalArrow::AttributeValues(csh.hintAttrName, csh);
  #else
	ArcTypePlusDir typeplusdir;
    typeplusdir.arc.type = EArrowSymbol::SOLID;
	typeplusdir.arc.lost = EArrowLost::NOT;
	typeplusdir.dir = EDirType::RIGHT;
	ArcVerticalArrow *ava = new ArcVerticalArrow(typeplusdir, MARKER_HERE_STR, MARKER_HERE_STR, &msc);
	VertXPos vxp(msc);
	ava->AddXpos(&vxp);
    ava->SetVerticalShape((yyvsp[-1].vshape));
    ava->AddAttributeList((yyvsp[0].attriblist));
	(yyval.arcbase) = ava;
  #endif
    free((yyvsp[-2].str));
}
#line 3718 "language.cc" /* yacc.c:1646  */
    break;

  case 52:
#line 901 "language.yy" /* yacc.c:1646  */
    {
    //Here we have no label and may continue as a parallel block
    //->offer parallel attributes, as well...
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0]))) {
        ArcDivider::AttributeNames(csh, false, false);
        ArcParallel::AttributeNames(csh, true);
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0]))) {
        ArcDivider::AttributeValues(csh.hintAttrName, csh, false, false);
        ArcParallel::AttributeValues(csh.hintAttrName, csh, true);
    }
  #else
    //... but due to the lack of curly brace we are a divider
    (yyval.arcbase) = new ArcDivider(EDividerSymbol::VSPACE, &msc);
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
  #endif
}
#line 3740 "language.cc" /* yacc.c:1646  */
    break;

  case 53:
#line 919 "language.yy" /* yacc.c:1646  */
    {
    //Here we have a colon label added: this is a divider
  #ifdef C_S_H_IS_COMPILED
  #else
    AttributeList *al = new AttributeList;
    al->Append(new Attribute("label", (yyvsp[0].str), MSC_POS((yyloc)), MSC_POS((yyloc)).IncStartCol()));
    (yyval.arcbase) = new ArcDivider(EDividerSymbol::VSPACE, &msc);
    ((yyval.arcbase))->AddAttributeList(al);
  #endif
    free((yyvsp[0].str));
}
#line 3756 "language.cc" /* yacc.c:1646  */
    break;

  case 54:
#line 931 "language.yy" /* yacc.c:1646  */
    {
    //Here we have a colon label added: this is a divider
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0]))) 
        ArcDivider::AttributeNames(csh, false, false);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0]))) 
        ArcDivider::AttributeValues(csh.hintAttrName, csh, false, false);
  #else
    ((yyvsp[0].attriblist))->Prepend(new Attribute("label", (yyvsp[-1].str), MSC_POS((yylsp[-1])), MSC_POS((yylsp[-1])).IncStartCol()));
    (yyval.arcbase) = new ArcDivider(EDividerSymbol::VSPACE, &msc);
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
#endif
    free((yyvsp[-1].str));
}
#line 3775 "language.cc" /* yacc.c:1646  */
    break;

  case 55:
#line 946 "language.yy" /* yacc.c:1646  */
    {
    //Here we have a colon label added: this is a divider
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[-2])) || 
        csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0])))
        ArcDivider::AttributeNames(csh, false, false);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[-2])) || 
             csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0]))) 
        ArcDivider::AttributeValues(csh.hintAttrName, csh, false, false);
  #else
    ((yyvsp[-2].attriblist))->Append(new Attribute("label", (yyvsp[-1].str), MSC_POS((yylsp[-1])), MSC_POS((yylsp[-1])).IncStartCol()));
    //Merge $3 at the end of $1 (after the colon label, so ordering is kept)
    ((yyvsp[-2].attriblist))->splice(((yyvsp[-2].attriblist))->end(), *((yyvsp[0].attriblist)));
    delete ((yyvsp[0].attriblist)); //empty list now
    (yyval.arcbase) = new ArcDivider(EDividerSymbol::VSPACE, &msc);
    ((yyval.arcbase))->AddAttributeList((yyvsp[-2].attriblist));
#endif
    free((yyvsp[-1].str));
}
#line 3799 "language.cc" /* yacc.c:1646  */
    break;

  case 56:
#line 966 "language.yy" /* yacc.c:1646  */
    {
    //Here we have a colon label added: this is a divider
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[-1]))) 
        ArcDivider::AttributeNames(csh, false, false);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[-1]))) 
        ArcDivider::AttributeValues(csh.hintAttrName, csh, false, false);
  #else
    ((yyvsp[-1].attriblist))->Append(new Attribute("label", (yyvsp[0].str), MSC_POS((yylsp[0])), MSC_POS((yylsp[0])).IncStartCol())); 
    (yyval.arcbase) = new ArcDivider(EDividerSymbol::VSPACE, &msc);
    ((yyval.arcbase))->AddAttributeList((yyvsp[-1].attriblist));    
  #endif
    free((yyvsp[0].str));
}
#line 3818 "language.cc" /* yacc.c:1646  */
    break;

  case 57:
#line 981 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (new CommandEntity((yyvsp[0].entitylist), &msc, false));
    ((yyval.arcbase))->AddAttributeList(NULL);
  #endif
}
#line 3830 "language.cc" /* yacc.c:1646  */
    break;

  case 58:
#line 989 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else
        csh.CheckEntityHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF);
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing an entity.");
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing an entity.");
  #endif
    free((yyvsp[0].str));
    (yyval.arcbase) = NULL;
}
#line 3850 "language.cc" /* yacc.c:1646  */
    break;

  case 59:
#line 1005 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else
        csh.CheckEntityHintAtAndBeforePlusOne((yylsp[-1]), (yylsp[0]));
  #else
    CommandEntity *ce = new CommandEntity((yyvsp[0].entitylist), &msc, false);
    ce->AddAttributeList(NULL);
    (yyval.arcbase) = ce->ApplyPrefix((yyvsp[-1].str));
  #endif
    free((yyvsp[-1].str));
}
#line 3870 "language.cc" /* yacc.c:1646  */
    break;

  case 60:
#line 1021 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_COMMA);
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing an entity.");
  #else
    CommandEntity *ce = new CommandEntity((yyvsp[-1].entitylist), &msc, false);
    ce->AddAttributeList(NULL);
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing an entity.");
    (yyval.arcbase) = ce;
  #endif
}
#line 3887 "language.cc" /* yacc.c:1646  */
    break;

  case 61:
#line 1034 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_COMMA);
    csh.CheckEntityHintBetween((yylsp[-1]), (yylsp[0]));
  #else
    ((yyvsp[0].entitylist))->Prepend((yyvsp[-2].entitylist));
    CommandEntity *ce = new CommandEntity((yyvsp[0].entitylist), &msc, false);
    delete ((yyvsp[-2].entitylist));
    ce->AddAttributeList(NULL);
    (yyval.arcbase) = ce;
  #endif
}
#line 3904 "language.cc" /* yacc.c:1646  */
    break;

  case 62:
#line 1047 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[0]), COLOR_COMMA);
    if (csh.CheckHintAt((yylsp[-2]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else
        csh.CheckEntityHintAtAndBeforePlusOne((yylsp[-2]), (yylsp[-1]));
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing an entity.");
  #else
    CommandEntity *ce = new CommandEntity((yyvsp[-1].entitylist), &msc, false);
    ce->AddAttributeList(NULL);
    (yyval.arcbase) = ce->ApplyPrefix((yyvsp[-2].str));
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing an entity.");
  #endif
    free((yyvsp[-2].str));
}
#line 3928 "language.cc" /* yacc.c:1646  */
    break;

  case 63:
#line 1067 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-3]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[-1]), COLOR_COMMA);
    if (csh.CheckHintAt((yylsp[-3]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else 
        csh.CheckEntityHintBetween((yylsp[-1]), (yylsp[0]));
#else
    ((yyvsp[0].entitylist))->Prepend((yyvsp[-2].entitylist));
    CommandEntity *ce = new CommandEntity((yyvsp[0].entitylist), &msc, false);
    delete ((yyvsp[-2].entitylist));
    ce->AddAttributeList(NULL);
    (yyval.arcbase) = ce->ApplyPrefix((yyvsp[-3].str));
  #endif
    free((yyvsp[-3].str));
}
#line 3951 "language.cc" /* yacc.c:1646  */
    break;

  case 64:
#line 1086 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    /* If there were arcs defined by the options (e.g., background)
     * enclose them in an "ArcParallel" element used only for this. 
     * This will be an internally defined ArcParallel that will
     * get unrolled in Msc::PostParseArcList()*/
    (yyval.arcbase) = ((yyvsp[0].arclist)) ? new ArcParallel(&msc, (yyvsp[0].arclist)) : NULL;
  #endif
}
#line 3966 "language.cc" /* yacc.c:1646  */
    break;

  case 65:
#line 1097 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (yyvsp[0].arcboxseries); //to remove warning for downcast
  #endif
}
#line 3977 "language.cc" /* yacc.c:1646  */
    break;

  case 66:
#line 1104 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (yyvsp[0].arcpipeseries); //to remove warning for downcast
  #endif
}
#line 3988 "language.cc" /* yacc.c:1646  */
    break;

  case 67:
#line 1111 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = ((yyvsp[0].arcparallel));
	if ((yyvsp[0].arcparallel))
        ((yyval.arcbase))->AddAttributeList(NULL);
  #endif
}
#line 4001 "language.cc" /* yacc.c:1646  */
    break;

  case 68:
#line 1120 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } 
  #else
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[-1].str));
}
#line 4018 "language.cc" /* yacc.c:1646  */
    break;

  case 69:
#line 1133 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } 
    csh.AddCSH_ErrorAfter((yyloc), "Missing shape name and definition.");
#else
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[0].str));
}
#line 4036 "language.cc" /* yacc.c:1646  */
    break;

  case 70:
#line 1147 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_VALUE)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[-1].str));
}
#line 4056 "language.cc" /* yacc.c:1646  */
    break;

  case 71:
#line 1163 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::ATTR_VALUE)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
	}
	csh.AddCSH_ErrorAfter((yyloc), "Missing color name to (re-)define.");
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing a color name to (re-)define.");
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[0].str));
}
#line 4078 "language.cc" /* yacc.c:1646  */
    break;

  case 72:
#line 1181 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_VALUE)) {
        csh.AddStylesToHints(true, true);
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[-1].str));
}
#line 4098 "language.cc" /* yacc.c:1646  */
    break;

  case 73:
#line 1197 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::ATTR_VALUE)) {
        csh.AddStylesToHints(true, true);
        csh.hintStatus = HINT_READY;
	}
	csh.AddCSH_ErrorAfter((yyloc), "Missing style name to (re-)define.");
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing a style name to (re-)define.");
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[0].str));
}
#line 4120 "language.cc" /* yacc.c:1646  */
    break;

  case 74:
#line 1215 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } 
  #else
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[-1].str));
}
#line 4137 "language.cc" /* yacc.c:1646  */
    break;

  case 75:
#line 1228 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } 
    csh.AddCSH_ErrorAfter((yyloc), "Missing design name to (re-)define.");
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing a design name to (re-)define.");
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[0].str));
}
#line 4156 "language.cc" /* yacc.c:1646  */
    break;

  case 76:
#line 1243 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } 
  #else
    (yyval.arcbase) = (new CommandEntity(NULL, &msc, false));
    ((yyval.arcbase))->AddAttributeList(NULL);
  #endif
    free((yyvsp[0].str));
}
#line 4174 "language.cc" /* yacc.c:1646  */
    break;

  case 77:
#line 1257 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0])))
        CommandEntity::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0])))
        CommandEntity::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcbase) = (new CommandEntity(NULL, &msc, false));
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
  #endif
    free((yyvsp[-1].str));
}
#line 4195 "language.cc" /* yacc.c:1646  */
    break;

  case 78:
#line 1274 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } 
  #else
    (yyval.arcbase) = (new ArcDivider(EDividerSymbol::VSPACE, &msc));
    ((yyval.arcbase))->AddAttributeList(NULL);
  #endif
    free((yyvsp[0].str));
}
#line 4213 "language.cc" /* yacc.c:1646  */
    break;

  case 79:
#line 1288 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0])))
        ArcDivider::AttributeNames(csh, true, false);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0])))
        ArcDivider::AttributeValues(csh.hintAttrName, csh, true, false);
  #else
    (yyval.arcbase) = (new ArcDivider(EDividerSymbol::VSPACE, &msc));
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
  #endif
    free((yyvsp[-1].str));
}
#line 4234 "language.cc" /* yacc.c:1646  */
    break;

  case 80:
#line 1305 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0])))
        ArcDivider::AttributeNames(csh, false, true);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0])))
        ArcDivider::AttributeValues(csh.hintAttrName, csh, false, true);
  #else
    const EDividerSymbol t = CaseInsensitiveEqual("title", (yyvsp[-1].str)) ? EDividerSymbol::TITLE :
                             CaseInsensitiveEqual("subtitle", (yyvsp[-1].str)) ? EDividerSymbol::SUBTITLE :
                             EDividerSymbol::INVALID;
    (yyval.arcbase) = (new ArcDivider(t, &msc));
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
  #endif
    free((yyvsp[-1].str));
}
#line 4258 "language.cc" /* yacc.c:1646  */
    break;

  case 81:
#line 1325 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } 
    csh.AddCSH_ErrorAfter((yyloc), "Missing label.");
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing label. Ignoring (sub)title.", "Titles and subtitles must have a label.");
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[0].str));
}
#line 4277 "language.cc" /* yacc.c:1646  */
    break;

  case 82:
#line 1340 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing 'at' keyword.");
  #else
    (yyval.arcbase) = NULL;
    msc.Error.Error(MSC_POS((yylsp[0])).end, "Missing 'at' clause.");
#endif
    free((yyvsp[0].str));
}
#line 4299 "language.cc" /* yacc.c:1646  */
    break;

  case 83:
#line 1358 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_ErrorAfter((yylsp[-1]), "Missing 'at' keyword.");
  #else
    (yyval.arcbase) = NULL;
    msc.Error.Error(MSC_POS((yylsp[-1])).end, "Missing 'at' clause. Ignoring this.");
    if ((yyvsp[0].attriblist))
        delete (yyvsp[0].attriblist);
  #endif
    free((yyvsp[-1].str));
}
#line 4323 "language.cc" /* yacc.c:1646  */
    break;

  case 84:
#line 1378 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-2]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } 
#else
    CommandSymbol *s = new CommandSymbol(&msc, (yyvsp[-1].vertxpos), MSC_POS((yylsp[-1])));
    s->AddAttributeList((yyvsp[0].attriblist));
    (yyval.arcbase) = s;
    if ((yyvsp[-1].vertxpos))
        delete (yyvsp[-1].vertxpos);
  #endif
    free((yyvsp[-2].str));
}
#line 4344 "language.cc" /* yacc.c:1646  */
    break;

  case 85:
#line 1395 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } 
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing label.");
  #else
    if ((yyvsp[0].vertxpos))
        delete (yyvsp[0].vertxpos);
    msc.Error.Error(MSC_POS((yylsp[0])).end, "Missing actual text - specify a label. Ignoring this.");
    (yyval.arcbase) = NULL;
#endif
    free((yyvsp[-1].str));
}
#line 4365 "language.cc" /* yacc.c:1646  */
    break;

  case 86:
#line 1412 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing marker name.");
  #else
    (yyval.arcbase) = NULL;
    msc.Error.Error(MSC_POS((yylsp[0])).end, "Missing marker name. Ignoring this.", "You need to supply a name which then can be used to refer to this vertical position you are marking here.");
  #endif
    free((yyvsp[0].str));
}
#line 4384 "language.cc" /* yacc.c:1646  */
    break;

  case 87:
#line 1427 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH((yylsp[0]), COLOR_MARKERNAME);
    csh.MarkerNames.insert((yyvsp[0].str));
  #else
    (yyval.arcbase) = (new CommandMark((yyvsp[0].str), MSC_POS((yyloc)), &msc));
    ((yyval.arcbase))->AddAttributeList(NULL);
  #endif
    free((yyvsp[-1].str));
    free((yyvsp[0].str));
}
#line 4405 "language.cc" /* yacc.c:1646  */
    break;

  case 88:
#line 1444 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[-1]), COLOR_MARKERNAME);
    csh.MarkerNames.insert((yyvsp[-1].str));
    if (csh.CheckHintAt((yylsp[-2]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[-1])))
        CommandMark::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[-1])))
        CommandMark::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcbase) = (new CommandMark((yyvsp[-1].str), MSC_POS((yyloc)), &msc));
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[-1].str));
}
#line 4429 "language.cc" /* yacc.c:1646  */
    break;

  case 89:
#line 1464 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_ErrorAfter((yylsp[-1]), "Missing marker name.");
  #else
    (yyval.arcbase) = NULL;
    msc.Error.Error(MSC_POS((yylsp[-1])).end, "Missing marker name. Ignoring this.", "You need to supply a name which then can be used to refer to this vertical position you are marking here.");
    if ((yyvsp[0].attriblist))
        delete (yyvsp[0].attriblist);
#endif
    free((yyvsp[-1].str));
}
#line 4450 "language.cc" /* yacc.c:1646  */
    break;

  case 90:
#line 1481 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
#else
    (yyval.arcbase) = (new CommandNewpage(&msc, true));
    ((yyval.arcbase))->AddAttributeList(NULL);
  #endif
    free((yyvsp[0].str));
}
#line 4468 "language.cc" /* yacc.c:1646  */
    break;

  case 91:
#line 1495 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0])))
        CommandNewpage::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0])))
        CommandNewpage::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcbase) = (new CommandNewpage(&msc, true));
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
  #endif
    free((yyvsp[-1].str));
}
#line 4489 "language.cc" /* yacc.c:1646  */
    break;

  case 95:
#line 1515 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-2]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0])))
        CommandHSpace::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0])))
        CommandHSpace::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcbase) = (new CommandHSpace(&msc, (yyvsp[-1].namerel)));
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
  #endif
    free((yyvsp[-2].str));
}
#line 4510 "language.cc" /* yacc.c:1646  */
    break;

  case 96:
#line 1532 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", 
            "Enter a number in pixels to set horizontal spacing.", 
            EHintType::KEYWORD, false));
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<label>", 
            "Enter some text the width of which will be used as horizontal spacing.", 
            EHintType::KEYWORD, false));
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_ErrorAfter((yylsp[-1]), "Missing either a number or a label.");
  #else
    (yyval.arcbase) = new CommandHSpace(&msc, (yyvsp[0].namerel)); //Will trigger an error: either label or space attr is needed
  #endif
    free((yyvsp[-1].str));
}
#line 4536 "language.cc" /* yacc.c:1646  */
    break;

  case 97:
#line 1554 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::ENTITY)) {
        csh.AddEntitiesToHints();
        csh.AddLeftRightHSpaceToHints();
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing an entity, 'left comment' or 'right comment'.");
  #else
    (yyval.arcbase) = NULL;
    msc.Error.Error(MSC_POS((yylsp[0])).end, "Missing an entity, 'left comment' or 'right comment'. Ignoring this command.");
  #endif
    free((yyvsp[0].str));
}
#line 4559 "language.cc" /* yacc.c:1646  */
    break;

  case 98:
#line 1573 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::ENTITY)) {
        csh.AddEntitiesToHints();
        csh.AddLeftRightHSpaceToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0])))
        CommandHSpace::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0])))
        CommandHSpace::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcbase) = NULL;
    msc.Error.Error(MSC_POS((yylsp[-1])).end, "Missing an entity, 'left comment' or 'right comment'. Ignoring this command.");
    delete (yyvsp[0].attriblist);
  #endif
    free((yyvsp[-1].str));
}
#line 4585 "language.cc" /* yacc.c:1646  */
    break;

  case 99:
#line 1595 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0])))
        CommandHSpace::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0])))
        CommandHSpace::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcbase) = (new CommandVSpace(&msc));
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
  #endif
    free((yyvsp[-1].str));
}
#line 4606 "language.cc" /* yacc.c:1646  */
    break;

  case 100:
#line 1612 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", 
            "Enter a number in pixels to add that much empty vertical spacing.", 
            EHintType::KEYWORD, false));
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<label>", 
            "Enter some text the height of which will be added as vertical empty space.", 
            EHintType::KEYWORD, false));
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing either a number or a label.");
  #else
    (yyval.arcbase) = new CommandVSpace(&msc); //will result in an error, since label or space attribute is needed
  #endif
    free((yyvsp[0].str));
}
#line 4632 "language.cc" /* yacc.c:1646  */
    break;

  case 104:
#line 1638 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (CaseInsensitiveEqual((yyvsp[-1].str), "left") || CaseInsensitiveEqual((yyvsp[-1].str), "right"))
        csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    else
        csh.AddCSH_Error((yylsp[-1]), "Use either `left comment` or `right comment` to specify which comment column to size.");
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::ENTITY)) {
        csh.AddEntitiesToHints();
        csh.AddLeftRightHSpaceToHints();
        csh.hintStatus = HINT_READY;
    } 
  #else
    if (CaseInsensitiveEqual((yyvsp[-1].str), "left"))
        (yyval.namerel) = new NamePair(LNOTE_ENT_STR, MSC_POS((yylsp[-1])), NULL, MSC_POS((yylsp[-1])));
    else if (CaseInsensitiveEqual((yyvsp[-1].str), "right"))
        (yyval.namerel) = new NamePair(RNOTE_ENT_STR, MSC_POS((yylsp[-1])), NULL, MSC_POS((yylsp[-1])));
    else {
        msc.Error.Error(MSC_POS((yylsp[-1])).start, "Use either `left` or `right` to specify which comment column to size. Ignoring command.");
        (yyval.namerel) = NULL;
    }
  #endif
    free((yyvsp[-1].str));
    free((yyvsp[0].str));
}
#line 4662 "language.cc" /* yacc.c:1646  */
    break;

  case 105:
#line 1664 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (CaseInsensitiveEqual((yyvsp[0].str), "left") || CaseInsensitiveEqual((yyvsp[0].str), "right")) {
        csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
        if (csh.CheckHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
            csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "comment", NULL, EHintType::KEYWORD, true));
            csh.hintStatus = HINT_READY;
        }
    }  else
        csh.AddCSH_Error((yylsp[0]), "Use either `left comment` or `right comment` to specify which comment column to size.");
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::ENTITY)) {
        csh.AddEntitiesToHints();
        csh.AddLeftRightHSpaceToHints();
        csh.hintStatus = HINT_READY;
    } 
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).start, "Use <entity>-<entity>, `left comment`, `right comment` to specify horizontal spacing. Ignoring command.");
    (yyval.namerel) = NULL;
  #endif
    free((yyvsp[0].str));
}
#line 4688 "language.cc" /* yacc.c:1646  */
    break;

  case 106:
#line 1686 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::ENTITY)) {
        csh.AddEntitiesToHints();
        csh.AddLeftRightHSpaceToHints();
        csh.hintStatus = HINT_READY;
    } 
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).start, "Use `left comment` or `right comment` to specify horizontal spacing for comments. Ignoring command.");
    (yyval.namerel) = NULL;
  #endif
    free((yyvsp[0].str));
}
#line 4707 "language.cc" /* yacc.c:1646  */
    break;

  case 108:
#line 1704 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_ATTRVALUE);
  #else
    AttributeList *al = new AttributeList;
    al->Append(new Attribute("space", atof((yyvsp[0].str)), MSC_POS((yylsp[0])), MSC_POS((yylsp[0])), (yyvsp[0].str)));
    (yyval.attriblist) = al;
  #endif
    free((yyvsp[0].str));
}
#line 4722 "language.cc" /* yacc.c:1646  */
    break;

  case 109:
#line 1715 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_ATTRVALUE);
  #else
    ((yyvsp[0].attriblist))->Append(new Attribute("space", atof((yyvsp[-1].str)), MSC_POS((yylsp[-1])), MSC_POS((yylsp[-1])), (yyvsp[-1].str)));
    (yyval.attriblist) = (yyvsp[0].attriblist);
  #endif
    free((yyvsp[-1].str));
}
#line 4736 "language.cc" /* yacc.c:1646  */
    break;

  case 112:
#line 1728 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_EntityName((yylsp[-1]), (yyvsp[-1].str));
    csh.CheckEntityHintAt((yylsp[-1]));
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.namerel) = new NamePair((yyvsp[-1].str), MSC_POS((yylsp[-1])), NULL, FileLineColRange());
  #endif
    free((yyvsp[-1].str));
}
#line 4752 "language.cc" /* yacc.c:1646  */
    break;

  case 113:
#line 1740 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
    csh.CheckEntityHintAt((yylsp[0]));
  #else
    (yyval.namerel) = new NamePair(NULL, FileLineColRange(), (yyvsp[0].str), MSC_POS((yylsp[0])));
  #endif
    free((yyvsp[0].str));
}
#line 4767 "language.cc" /* yacc.c:1646  */
    break;

  case 114:
#line 1751 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.namerel) = NULL;
  #endif
}
#line 4780 "language.cc" /* yacc.c:1646  */
    break;

  case 115:
#line 1760 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_EntityName((yylsp[-2]), (yyvsp[-2].str));
    csh.CheckEntityHintAt((yylsp[-2]));
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
    csh.CheckEntityHintAt((yylsp[0]));
  #else
    (yyval.namerel) = new NamePair((yyvsp[-2].str), MSC_POS((yylsp[-2])), (yyvsp[0].str), MSC_POS((yylsp[0])));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 4798 "language.cc" /* yacc.c:1646  */
    break;

  case 116:
#line 1774 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
    csh.CheckEntityHintAt((yylsp[0]));
  #else
    (yyval.namerel) = new NamePair((yyvsp[0].str), MSC_POS((yylsp[0])), NULL, FileLineColRange());
  #endif
    free((yyvsp[0].str));
}
#line 4812 "language.cc" /* yacc.c:1646  */
    break;

  case 117:
#line 1786 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_MARKERNAME);
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    csh.CheckHintAt((yylsp[-1]), EHintSourceType::MARKER);
    csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::MARKER);
  #else
    (yyval.namerel) = new NamePair((yyvsp[-1].str), MSC_POS((yylsp[-1])), NULL, FileLineColRange());
  #endif
    free((yyvsp[-1].str));
}
#line 4828 "language.cc" /* yacc.c:1646  */
    break;

  case 118:
#line 1798 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    csh.AddCSH((yylsp[0]), COLOR_MARKERNAME);
    csh.CheckHintAt((yylsp[0]), EHintSourceType::MARKER);
  #else
    (yyval.namerel) = new NamePair(NULL, FileLineColRange(), (yyvsp[0].str), MSC_POS((yylsp[0])));
  #endif
    free((yyvsp[0].str));
}
#line 4843 "language.cc" /* yacc.c:1646  */
    break;

  case 119:
#line 1809 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    csh.CheckHintAt((yylsp[0]), EHintSourceType::MARKER);
    csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::MARKER);
  #else
    (yyval.namerel) = NULL;
  #endif
}
#line 4857 "language.cc" /* yacc.c:1646  */
    break;

  case 120:
#line 1819 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_MARKERNAME);
    csh.CheckHintAt((yylsp[-2]), EHintSourceType::MARKER);
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    csh.AddCSH((yylsp[0]), COLOR_MARKERNAME);
    csh.CheckHintAt((yylsp[0]), EHintSourceType::MARKER);
  #else
    (yyval.namerel) = new NamePair((yyvsp[-2].str), MSC_POS((yylsp[-2])), (yyvsp[0].str), MSC_POS((yylsp[0])));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 4875 "language.cc" /* yacc.c:1646  */
    break;

  case 125:
#line 1836 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[0].arcbase)) {
        (yyval.arclist) = (new ArcList)->Append((yyvsp[0].arcbase)); /* New list */
        //($1)->MakeMeLastNotable(); Do not make chart options notable
    } else
        (yyval.arclist) = NULL;
  #endif
}
#line 4889 "language.cc" /* yacc.c:1646  */
    break;

  case 126:
#line 1846 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_COMMA);
    if (csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    if ((yyvsp[0].arcbase)) {
        if ((yyvsp[-2].arclist))
            (yyval.arclist) = ((yyvsp[-2].arclist))->Append((yyvsp[0].arcbase));     /* Add to existing list */
        else
            (yyval.arclist) = (new ArcList)->Append((yyvsp[0].arcbase)); /* New list */
        //($3)->MakeMeLastNotable(); Do not make chart options notable
    } else
        (yyval.arclist) = (yyvsp[-2].arclist);
  #endif
}
#line 4912 "language.cc" /* yacc.c:1646  */
    break;

  case 127:
#line 1865 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_COMMA);
    if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.arclist) = (yyvsp[-1].arclist);
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Expecting an option here.");
  #endif
}
#line 4929 "language.cc" /* yacc.c:1646  */
    break;

  case 129:
#line 1879 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_COMMA);
    if (csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_Error((yylsp[0]), "An option expected here.");
  #else
    (yyval.arclist) = (yyvsp[-2].arclist);
  #endif
}
#line 4946 "language.cc" /* yacc.c:1646  */
    break;

  case 130:
#line 1894 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName((yylsp[-2]), (yyvsp[-2].str), COLOR_OPTIONNAME);
    csh.AddCSH((yylsp[-1]), COLOR_EQUAL);
    csh.AddCSH((yylsp[0]), COLOR_ATTRVALUE);
    if (csh.CheckHintAt((yylsp[-2]), EHintSourceType::ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_VALUE, (yyvsp[-2].str))) {
        Msc::AttributeValues((yyvsp[-2].str), csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.arcbase) = msc.AddAttribute(Attribute((yyvsp[-2].str), atof((yyvsp[0].str)), MSC_POS((yylsp[-2])), MSC_POS((yylsp[0])), (yyvsp[0].str)));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 4969 "language.cc" /* yacc.c:1646  */
    break;

  case 131:
#line 1913 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName((yylsp[-2]), (yyvsp[-2].str), COLOR_OPTIONNAME);
    csh.AddCSH((yylsp[-1]), COLOR_EQUAL);
    csh.AddCSH_AttrValue_CheckAndAddEscapeHint((yylsp[0]), (yyvsp[0].str), (yyvsp[-2].str));
    if (csh.CheckHintAt((yylsp[-2]), EHintSourceType::ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_VALUE, (yyvsp[-2].str))) {
        Msc::AttributeValues((yyvsp[-2].str), csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.arcbase) = msc.AddAttribute(Attribute((yyvsp[-2].str), (yyvsp[0].str), MSC_POS((yylsp[-2])), MSC_POS((yylsp[0]))));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 4992 "language.cc" /* yacc.c:1646  */
    break;

  case 132:
#line 1932 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName((yylsp[-1]), (yyvsp[-1].str), COLOR_OPTIONNAME);
    csh.AddCSH((yylsp[0]), COLOR_EQUAL);
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing option value.");
    if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::ATTR_VALUE, (yyvsp[-1].str))) {
        Msc::AttributeValues((yyvsp[-1].str), csh);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing option value.");
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[-1].str));
}
#line 5015 "language.cc" /* yacc.c:1646  */
    break;

  case 133:
#line 1951 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[-1]), COLOR_EQUAL);
    csh.AddCSH((yylsp[0]), COLOR_DESIGNNAME);
    if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_VALUE, "msc")) {
        csh.AddDesignsToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAt((yylsp[-2]), EHintSourceType::ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    }
    std::string msg = csh.SetDesignTo((yyvsp[0].str), true);
    if (msg.length())
        csh.AddCSH_Error((yylsp[0]), msg.c_str());
  #else
    (yyval.arcbase) = msc.AddAttribute(Attribute("msc", (yyvsp[0].str), MSC_POS((yyloc)), MSC_POS((yylsp[0]))));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 5041 "language.cc" /* yacc.c:1646  */
    break;

  case 134:
#line 1973 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[0]), COLOR_EQUAL);
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing option value.");
    if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::ATTR_VALUE, "msc")) {
        csh.AddDesignsToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing option value.");
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[-1].str));
}
#line 5064 "language.cc" /* yacc.c:1646  */
    break;

  case 135:
#line 1992 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[-1]), COLOR_EQUAL);
    csh.AddCSH((yylsp[0]), COLOR_DESIGNNAME);
    if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_VALUE, "msc+")) {
        csh.AddDesignsToHints(false);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAt((yylsp[-2]), EHintSourceType::ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    }
    std::string msg = csh.SetDesignTo((yyvsp[0].str), false);
    if (msg.length())
        csh.AddCSH_Error((yylsp[0]), msg.c_str());
  #else
        (yyval.arcbase) = msc.AddAttribute(Attribute("msc+", (yyvsp[0].str), MSC_POS((yyloc)), MSC_POS((yylsp[0]))));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 5090 "language.cc" /* yacc.c:1646  */
    break;

  case 136:
#line 2014 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[0]), COLOR_EQUAL);
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing option value.");
    if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::ATTR_VALUE, "msc+")) {
        csh.AddDesignsToHints(false);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing option value.");
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[-1].str));
}
#line 5113 "language.cc" /* yacc.c:1646  */
    break;

  case 137:
#line 2034 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.entitylist) = ((yyvsp[0].entitylist));
  #endif
}
#line 5123 "language.cc" /* yacc.c:1646  */
    break;

  case 138:
#line 2040 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_COMMA);
    csh.CheckEntityHintBetween((yylsp[-1]), (yylsp[0]));
#else
    ((yyvsp[0].entitylist))->Prepend((yyvsp[-2].entitylist));
    (yyval.entitylist) = (yyvsp[0].entitylist);
    delete ((yyvsp[-2].entitylist));
  #endif
}
#line 5138 "language.cc" /* yacc.c:1646  */
    break;

  case 139:
#line 2051 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_COMMA);
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.entitylist) = ((yyvsp[-1].entitylist));
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Expecting an entity here.");
  #endif
}
#line 5152 "language.cc" /* yacc.c:1646  */
    break;

  case 141:
#line 2066 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[-1]));
    csh.AddCSH_EntityName((yylsp[-1]), (yyvsp[-1].str));
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0])))
        EntityApp::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0])))
        EntityApp::AttributeValues(csh.hintAttrName, csh);
  #else
    EntityApp *ed = new EntityApp((yyvsp[-1].str), &msc);
    ed->SetLineEnd(MSC_POS((yyloc)));
    (yyval.entitylist) = ed->AddAttributeList((yyvsp[0].attriblist), NULL, FileLineCol());
  #endif
    free((yyvsp[-1].str));
}
#line 5172 "language.cc" /* yacc.c:1646  */
    break;

  case 142:
#line 2082 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    EntityApp *ed = new EntityApp((yyvsp[0].str), &msc);
    ed->SetLineEnd(MSC_POS((yyloc)));
    (yyval.entitylist) = ed->AddAttributeList(NULL, NULL, FileLineCol());
  #endif
    free((yyvsp[0].str));
}
#line 5188 "language.cc" /* yacc.c:1646  */
    break;

  case 143:
#line 2094 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[-2]));
    csh.AddCSH_EntityName((yylsp[-2]), (yyvsp[-2].str));
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[-1])))
        EntityApp::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[-1])))
        EntityApp::AttributeValues(csh.hintAttrName, csh);
  #else
    EntityApp *ed = new EntityApp((yyvsp[-2].str), &msc);
    ed->SetLineEnd(MSC_POS2((yylsp[-2]), (yylsp[-1])));
    (yyval.entitylist) = ed->AddAttributeList((yyvsp[-1].attriblist), (yyvsp[0].arclist), MSC_POS((yylsp[0])).start);
  #endif
    free((yyvsp[-2].str));
}
#line 5208 "language.cc" /* yacc.c:1646  */
    break;

  case 144:
#line 2110 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[-1]));
    csh.AddCSH_EntityName((yylsp[-1]), (yyvsp[-1].str));
  #else
    EntityApp *ed = new EntityApp((yyvsp[-1].str), &msc);
    ed->SetLineEnd(MSC_POS((yylsp[-1])));
    (yyval.entitylist) = ed->AddAttributeList(NULL, (yyvsp[0].arclist), MSC_POS((yylsp[0])).start);
  #endif
    free((yyvsp[-1].str));
}
#line 5224 "language.cc" /* yacc.c:1646  */
    break;

  case 145:
#line 2123 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0])))
        EntityApp::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0])))
        EntityApp::AttributeValues(csh.hintAttrName, csh);
    csh.AddCSH_KeywordOrEntity((yylsp[-1]), (yyvsp[-1].str));  //Do it after AddLineBeginToHints so if this is a newly defined entity it one is not included among the hints
  #else
    EntityApp *ed = new EntityApp((yyvsp[-1].str), &msc);
    ed->SetLineEnd(MSC_POS((yyloc)));
    (yyval.entitylist) = ed->AddAttributeList((yyvsp[0].attriblist), NULL, FileLineCol());
  #endif
    free((yyvsp[-1].str));
}
#line 5246 "language.cc" /* yacc.c:1646  */
    break;

  case 146:
#line 2141 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_KeywordOrEntity((yylsp[0]), (yyvsp[0].str));   //Do it after AddLineBeginToHints so if this is a newly defined entity it one is not included among the hints
  #else
    EntityApp *ed = new EntityApp((yyvsp[0].str), &msc);
    ed->SetLineEnd(MSC_POS((yyloc)));
    (yyval.entitylist) = ed->AddAttributeList(NULL, NULL, FileLineCol());
  #endif
    free((yyvsp[0].str));
}
#line 5265 "language.cc" /* yacc.c:1646  */
    break;

  case 147:
#line 2156 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintAt((yylsp[-2]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[-1])))
        EntityApp::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[-1])))
        EntityApp::AttributeValues(csh.hintAttrName, csh);
    csh.AddCSH_KeywordOrEntity((yylsp[-2]), (yyvsp[-2].str));  //Do it after AddLineBeginToHints so this one is not included
  #else
    EntityApp *ed = new EntityApp((yyvsp[-2].str), &msc);
    ed->SetLineEnd(MSC_POS2((yylsp[-2]), (yylsp[-1])));
    (yyval.entitylist) = ed->AddAttributeList((yyvsp[-1].attriblist), (yyvsp[0].arclist), MSC_POS((yylsp[0])).start);
  #endif
    free((yyvsp[-2].str));
}
#line 5287 "language.cc" /* yacc.c:1646  */
    break;

  case 148:
#line 2174 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_KeywordOrEntity((yylsp[-1]), (yyvsp[-1].str));   //Do it after AddLineBeginToHints so this one is not included
  #else
    EntityApp *ed = new EntityApp((yyvsp[-1].str), &msc);
    ed->SetLineEnd(MSC_POS((yylsp[-1])));
    (yyval.entitylist) = ed->AddAttributeList(NULL, (yyvsp[0].arclist), MSC_POS((yylsp[0])).start);
  #endif
    free((yyvsp[-1].str));
}
#line 5306 "language.cc" /* yacc.c:1646  */
    break;

  case 150:
#line 2191 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_COMMA);
    if (csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_VALUE)) {
        csh.AddStylesToHints(true, true);
        csh.hintStatus = HINT_READY;
    }
  #endif
}
#line 5320 "language.cc" /* yacc.c:1646  */
    break;

  case 151:
#line 2201 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_COMMA);
    if (csh.CheckHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::ATTR_VALUE)) {
        csh.AddStylesToHints(true, true);
        csh.hintStatus = HINT_READY;
    }
  #else 
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing style definition here.", "Try just removing the comma.");
#endif
}
#line 5336 "language.cc" /* yacc.c:1646  */
    break;

  case 152:
#line 2214 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    for (auto &str : *((yyvsp[-1].stringlist)))
        if (csh.ForbiddenStyles.find(str) == csh.ForbiddenStyles.end())
            csh.Contexts.back().StyleNames.insert(str);
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0])))
        MscStyle().AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0])))
        MscStyle().AttributeValues(csh.hintAttrName, csh);
  #else
    for (auto a=((yyvsp[0].attriblist))->begin(); a!=((yyvsp[0].attriblist))->end(); a++) {
        std::list<string> problem;
        bool had_generic = false;
        for (auto s = ((yyvsp[-1].stringlist))->begin(); s!=((yyvsp[-1].stringlist))->end(); s++) {
            StyleCoW style = msc.Contexts.back().styles.GetStyle(*s); //may be default style
            if (style.write().AddAttribute(**a, &msc))
               msc.Contexts.back().styles[*s] = style;
            else {
               problem.push_back(*s);
               had_generic |= (style.read().type == STYLE_STYLE);
            }
        }
        if (problem.size()==0) continue;
        string msg;
        if (problem.size()==1) {
            if (had_generic)
                msg = "Attribute '" + (*a)->name + "' is not applicable to styles. Ignoring it.";
            else
                msg = "Attribute '" + (*a)->name + "' is not applicable to style '" + *problem.begin() + "'. Ignoring it.";
        } else if (problem.size() == ((yyvsp[0].attriblist))->size()) {
            if (had_generic)
                msg = "Attribute '" + (*a)->name + "' is not applicable to styles. Ignoring it.";
            else
                msg = "Attribute '" + (*a)->name + "' is not applicable to any of these styles. Ignoring it.";
        } else {
            msg = *problem.begin();
            for (auto p = ++problem.begin(); p!=--problem.end(); p++)
                msg.append("', '").append(*p);
            msg.append("' and '").append(*--problem.end());
            _ASSERT(!had_generic);
        }
        msc.Error.Error(**a, false, msg);
    }
    delete((yyvsp[0].attriblist));
  #endif
    delete((yyvsp[-1].stringlist));
}
#line 5388 "language.cc" /* yacc.c:1646  */
    break;

  case 153:
#line 2262 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
	csh.AddCSH_ErrorAfter((yyloc), "Missing attribute definitons in square brackets ('[' and ']').");
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing attribute definitons in square brackets ('[' and ']').");
  #endif
    delete((yyvsp[0].stringlist));
}
#line 5401 "language.cc" /* yacc.c:1646  */
    break;

  case 154:
#line 2273 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_STYLENAME);
    (yyval.stringlist) = new std::list<string>;
    if (strcmp((yyvsp[0].str), "emphasis")==0)
        ((yyval.stringlist))->push_back("box");
    else if (strcmp((yyvsp[0].str), "emptyemphasis")==0)
        ((yyval.stringlist))->push_back("emptybox");
    else ((yyval.stringlist))->push_back((yyvsp[0].str));
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::ATTR_NAME)) {
		csh.AddStylesToHints(true, true);
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.stringlist) = new std::list<string>;
    ((yyval.stringlist))->push_back(ConvertEmphasisToBox((yyvsp[0].str), &(yylsp[0]), msc));
  #endif
    free((yyvsp[0].str));
}
#line 5425 "language.cc" /* yacc.c:1646  */
    break;

  case 155:
#line 2293 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_STYLENAME);
    (yyval.stringlist) = new std::list<string>;
	((yyval.stringlist))->push_back("++");
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::ATTR_NAME)) {
		csh.AddStylesToHints(true, true);
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.stringlist) = new std::list<string>;
    ((yyval.stringlist))->push_back(ConvertEmphasisToBox("++", &(yylsp[0]), msc));
  #endif
}
#line 5444 "language.cc" /* yacc.c:1646  */
    break;

  case 156:
#line 2308 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_COMMA);
	csh.AddCSH_ErrorAfter((yylsp[0]), "Missing a style name to (re-)define.");
    if (csh.CheckHintAfterPlusOne((yylsp[-1]), yylloc, yychar==YYEOF, EHintSourceType::ATTR_NAME)) {
		csh.AddStylesToHints(true, true);
        csh.hintStatus = HINT_READY;
    }
	(yyval.stringlist) = (yyvsp[-1].stringlist);
  #else
    (yyval.stringlist) = (yyvsp[-1].stringlist);
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing a style name to (re-)define.");
  #endif
}
#line 5463 "language.cc" /* yacc.c:1646  */
    break;

  case 157:
#line 2323 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_COMMA);
    csh.AddCSH((yylsp[0]), COLOR_STYLENAME);
    (yyval.stringlist) = (yyvsp[-2].stringlist);
    if (strcmp((yyvsp[0].str), "emphasis")==0)
        ((yyval.stringlist))->push_back("box");
    else if (strcmp((yyvsp[0].str), "emptyemphasis")==0)
        ((yyval.stringlist))->push_back("emptybox");
    else ((yyval.stringlist))->push_back((yyvsp[0].str));
    if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_NAME)) {
		csh.AddStylesToHints(true, true);
        csh.hintStatus = HINT_READY;
    }
  #else
    ((yyvsp[-2].stringlist))->push_back(ConvertEmphasisToBox((yyvsp[0].str), &(yylsp[0]), msc));
    (yyval.stringlist) = (yyvsp[-2].stringlist);
  #endif
    free((yyvsp[0].str));
}
#line 5488 "language.cc" /* yacc.c:1646  */
    break;

  case 158:
#line 2344 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_COMMA);
    csh.AddCSH((yylsp[0]), COLOR_STYLENAME);
    (yyval.stringlist) = (yyvsp[-2].stringlist);
	((yyval.stringlist))->push_back("++");
    if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_NAME)) {
		csh.AddStylesToHints(true, true);
        csh.hintStatus = HINT_READY;
    }
  #else
    ((yyvsp[-2].stringlist))->push_back(ConvertEmphasisToBox("++", &(yylsp[0]), msc));
    (yyval.stringlist) = (yyvsp[-2].stringlist);
  #endif
}
#line 5508 "language.cc" /* yacc.c:1646  */
    break;

  case 159:
#line 2361 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_ATTRVALUE);
    csh.AddCSH_ErrorAfter((yyloc), ("Here should come a shape definition beginning with '{'. Ignoring this malformed shape definition for '"+string((yyvsp[0].str) ? (yyvsp[0].str) : "") +"'.").c_str());
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Here should come a shape definition beginning with '{'. Ignoring this malformed shape definition for '"+string((yyvsp[0].str) ? (yyvsp[0].str) : "") +"'.");
  #endif	
  free((yyvsp[0].str));
}
#line 5522 "language.cc" /* yacc.c:1646  */
    break;

  case 160:
#line 2371 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_ATTRVALUE);
    csh.AddCSH((yylsp[0]), COLOR_BRACE);
    csh.AddCSH_ErrorAfter((yyloc), ("Here should come a shape definition beginning with 'T', 'H', 'M', 'L', 'C', 'S' or 'E'. Ignoring this malformed shape definition for '"+string((yyvsp[-1].str) ? (yyvsp[-1].str): "") +"'.").c_str());
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Here should come a shape definition beginning with 'T', 'H', 'M', 'L', 'C', 'S' or 'E'. Ignoring this malformed shape definition for '"+string((yyvsp[-1].str) ? (yyvsp[-1].str): "") +"'.");
  #endif	
  free((yyvsp[-1].str));
}
#line 5537 "language.cc" /* yacc.c:1646  */
    break;

  case 161:
#line 2382 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_ATTRVALUE);
    csh.AddCSH((yylsp[-1]), COLOR_BRACE);
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing a closing brace ('}').");
	csh.AddShapeName((yyvsp[-2].str));
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing '}'.");
    msc.Error.Error(MSC_POS((yylsp[-1])).start, MSC_POS((yylsp[0])).end.NextChar(), "Here is the corresponding '{'.");
	if ((yyvsp[0].shape)) {
		msc.Shapes.Add(std::string((yyvsp[-2].str)), MSC_POS((yylsp[-2])).start, msc.file_url, msc.file_info, (yyvsp[0].shape), msc.Error);
		delete (yyvsp[0].shape);
	}
  #endif	
  free((yyvsp[-2].str));
}
#line 5558 "language.cc" /* yacc.c:1646  */
    break;

  case 162:
#line 2399 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-3]), COLOR_ATTRVALUE);
    csh.AddCSH((yylsp[-2]), COLOR_BRACE);
    csh.AddCSH((yylsp[0]), COLOR_BRACE);
	csh.AddShapeName((yyvsp[-3].str));
  #else
	if ((yyvsp[-1].shape)) {
		msc.Shapes.Add(std::string((yyvsp[-3].str)), MSC_POS((yylsp[-3])).start, msc.file_url, msc.file_info, (yyvsp[-1].shape), msc.Error);
		delete (yyvsp[-1].shape);
	}
  #endif	
  free((yyvsp[-3].str));
}
#line 5577 "language.cc" /* yacc.c:1646  */
    break;

  case 163:
#line 2414 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-4]), COLOR_ATTRVALUE);
    csh.AddCSH((yylsp[-3]), COLOR_BRACE);
    csh.AddCSH((yylsp[0]), COLOR_BRACE);
	csh.AddShapeName((yyvsp[-4].str));
    csh.AddCSH_Error((yylsp[-1]), "Only numbers can come after shape commands.");
  #else
	if ((yyvsp[-2].shape)) {
		msc.Shapes.Add(std::string((yyvsp[-4].str)), MSC_POS((yylsp[-4])).start, msc.file_url, msc.file_info, (yyvsp[-2].shape), msc.Error);
		delete (yyvsp[-2].shape);
	}
  #endif	
  free((yyvsp[-4].str));
}
#line 5597 "language.cc" /* yacc.c:1646  */
    break;

  case 164:
#line 2431 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SEMICOLON);
  #else
    (yyval.shape) = new Shape;
	if ((yyvsp[-1].shapeelement)) {
		((yyval.shape))->Add(std::move(*((yyvsp[-1].shapeelement))));
		delete (yyvsp[-1].shapeelement);
	}
  #endif	
}
#line 5613 "language.cc" /* yacc.c:1646  */
    break;

  case 165:
#line 2443 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[-2]), "I do not understand this.");
    csh.AddCSH((yylsp[0]), COLOR_SEMICOLON);
#else
    (yyval.shape) = new Shape;
	if ((yyvsp[-1].shapeelement)) {
		((yyval.shape))->Add(std::move(*((yyvsp[-1].shapeelement))));
		delete (yyvsp[-1].shapeelement);
	}
  #endif	
}
#line 5630 "language.cc" /* yacc.c:1646  */
    break;

  case 166:
#line 2456 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SEMICOLON);
  #else
	if ((yyvsp[-1].shapeelement)) {
		((yyvsp[-2].shape))->Add(std::move(*((yyvsp[-1].shapeelement))));
		delete (yyvsp[-1].shapeelement);
	}
  #endif	
}
#line 5645 "language.cc" /* yacc.c:1646  */
    break;

  case 167:
#line 2467 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[0]), "Only numbers can come after shape commands.");
  #else
  #endif	
}
#line 5656 "language.cc" /* yacc.c:1646  */
    break;

  case 168:
#line 2475 "language.yy" /* yacc.c:1646  */
    {
    const int num_args = 0;
	const int should_args = ShapeElement::GetNumArgs((yyvsp[0].shapecommand));
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
	if (should_args != num_args)
		csh.AddCSH_ErrorAfter((yyloc), ShapeElement::ErrorMsg((yyvsp[0].shapecommand), num_args));
  #else
	(yyval.shapeelement) = NULL;
	if (should_args != num_args)
		msc.Error.Error(MSC_POS((yyloc)).end, ShapeElement::ErrorMsg((yyvsp[0].shapecommand), num_args).append(" Ignoring line."));
	else 
	    (yyval.shapeelement) = new ShapeElement((yyvsp[0].shapecommand));
  #endif	
}
#line 5676 "language.cc" /* yacc.c:1646  */
    break;

  case 169:
#line 2491 "language.yy" /* yacc.c:1646  */
    {
    const int num_args = 1;
	const int should_args = ShapeElement::GetNumArgs((yyvsp[-1].shapecommand));
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
	if (should_args != num_args) {
		const string msg = ShapeElement::ErrorMsg((yyvsp[-1].shapecommand), num_args);
	    switch (ShapeElement::GetNumArgs((yyvsp[-1].shapecommand))) {
		case 0:  csh.AddCSH_Error((yylsp[0]), msg); break;
		default: csh.AddCSH_ErrorAfter((yyloc), msg); break;
		}
	} else if ((yyvsp[-1].shapecommand)>=ShapeElement::SECTION_BG && (((yyvsp[0].str))[0]<'0' || ((yyvsp[0].str))[0]>'2' || ((yyvsp[0].str))[1]!=0))
		csh.AddCSH_Error((yylsp[0]), "S (section) commands require an integer between 0 and 2.");
  #else
	(yyval.shapeelement) = NULL;
	const double a = atof((yyvsp[0].str));
	if (should_args > num_args)
		msc.Error.Error(MSC_POS((yylsp[0])).end, ShapeElement::ErrorMsg((yyvsp[-1].shapecommand), num_args).append(" Ignoring line."));
	else if ((yyvsp[-1].shapecommand)>=ShapeElement::SECTION_BG && (a!=0 && a!=1 && a!=2))
		msc.Error.Error(MSC_POS((yylsp[0])).start, "S (section) commands require an integer between 0 and 2. Ignoring line.");
	else if ((yyvsp[-1].shapecommand)>=ShapeElement::SECTION_BG)
	    (yyval.shapeelement) = new ShapeElement(ShapeElement::Type((yyvsp[-1].shapecommand) + unsigned(a)));
	else 
		(yyval.shapeelement) = new ShapeElement((yyvsp[-1].shapecommand), a);
  #endif	
  free((yyvsp[0].str));
}
#line 5708 "language.cc" /* yacc.c:1646  */
    break;

  case 170:
#line 2519 "language.yy" /* yacc.c:1646  */
    {
    const int num_args = 2;
	const int should_args = ShapeElement::GetNumArgs((yyvsp[-2].shapecommand));
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
	if (should_args != num_args) {
		const string msg = ShapeElement::ErrorMsg((yyvsp[-2].shapecommand), num_args);
	    switch (ShapeElement::GetNumArgs((yyvsp[-2].shapecommand))) {
		case 0:  csh.AddCSH_Error((yylsp[-1]) + (yylsp[0]), msg); break;
		case 1:  csh.AddCSH_Error((yylsp[0]), msg); break;
		default: csh.AddCSH_ErrorAfter((yyloc), msg); break;
		}
	}
  #else
	(yyval.shapeelement) = NULL;
	if (should_args > num_args)
		msc.Error.Error(MSC_POS((yyloc)).end, ShapeElement::ErrorMsg((yyvsp[-2].shapecommand), num_args).append(" Ignoring line."));
	else 
		(yyval.shapeelement) = new ShapeElement((yyvsp[-2].shapecommand), atof((yyvsp[-1].str)), atof((yyvsp[0].str)));
  #endif	
  free((yyvsp[-1].str));
  free((yyvsp[0].str));
}
#line 5736 "language.cc" /* yacc.c:1646  */
    break;

  case 171:
#line 2543 "language.yy" /* yacc.c:1646  */
    {
    const int num_args = 3;
	const int should_args = ShapeElement::GetNumArgs((yyvsp[-3].shapecommand));
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-3]), COLOR_KEYWORD);
	if (should_args != num_args) {
		const string msg = ShapeElement::ErrorMsg((yyvsp[-3].shapecommand), num_args);
	    switch (ShapeElement::GetNumArgs((yyvsp[-3].shapecommand))) {
		case 0:  csh.AddCSH_Error((yylsp[-2]) + (yylsp[0]), msg); break;
		case 1:  csh.AddCSH_Error((yylsp[-1]) + (yylsp[0]), msg); break;
		case 2:  csh.AddCSH_Error((yylsp[0]), msg); break;
		default: csh.AddCSH_ErrorAfter((yyloc), msg); break;
		}
	}
  #else
	(yyval.shapeelement) = NULL;
	if (should_args > num_args)
		msc.Error.Error(MSC_POS((yyloc)).end, ShapeElement::ErrorMsg((yyvsp[-3].shapecommand), num_args).append(" Ignoring line."));
	else 
		(yyval.shapeelement) = new ShapeElement((yyvsp[-3].shapecommand), atof((yyvsp[-2].str)), atof((yyvsp[-1].str)), atof((yyvsp[0].str)));
  #endif	
  free((yyvsp[-2].str));
  free((yyvsp[-1].str));
  free((yyvsp[0].str));
}
#line 5766 "language.cc" /* yacc.c:1646  */
    break;

  case 172:
#line 2569 "language.yy" /* yacc.c:1646  */
    {
    const int num_args = 4;
	const int should_args = ShapeElement::GetNumArgs((yyvsp[-4].shapecommand));
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-4]), COLOR_KEYWORD);
	if (should_args != num_args) {
		const string msg = ShapeElement::ErrorMsg((yyvsp[-4].shapecommand), num_args);
	    switch (ShapeElement::GetNumArgs((yyvsp[-4].shapecommand))) {
		case 0:  csh.AddCSH_Error((yylsp[-3]) + (yylsp[0]), msg); break;
		case 1:  csh.AddCSH_Error((yylsp[-2]) + (yylsp[0]), msg); break;
		case 2:  csh.AddCSH_Error((yylsp[-1]) + (yylsp[0]), msg); break;
		case 3:  csh.AddCSH_Error((yylsp[0]), msg); break;
		default: csh.AddCSH_ErrorAfter((yyloc), msg); break;
		}
	}
  #else
	(yyval.shapeelement) = NULL;
	if (should_args > num_args)
		msc.Error.Error(MSC_POS((yyloc)).end, ShapeElement::ErrorMsg((yyvsp[-4].shapecommand), num_args).append(" Ignoring line."));
	else 
		(yyval.shapeelement) = new ShapeElement((yyvsp[-4].shapecommand), atof((yyvsp[-3].str)), atof((yyvsp[-2].str)), atof((yyvsp[-1].str)), atof((yyvsp[0].str)));
  #endif	
  free((yyvsp[-3].str));
  free((yyvsp[-2].str));
  free((yyvsp[-1].str));
  free((yyvsp[0].str));
}
#line 5798 "language.cc" /* yacc.c:1646  */
    break;

  case 173:
#line 2597 "language.yy" /* yacc.c:1646  */
    {
    const int num_args = 5;
	const int should_args = ShapeElement::GetNumArgs((yyvsp[-5].shapecommand));
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-5]), COLOR_KEYWORD);
	if (should_args != num_args) {
		const string msg = ShapeElement::ErrorMsg((yyvsp[-5].shapecommand), num_args);
	    switch (ShapeElement::GetNumArgs((yyvsp[-5].shapecommand))) {
		case 0:  csh.AddCSH_Error((yylsp[-4]) + (yylsp[0]), msg); break;
		case 1:  csh.AddCSH_Error((yylsp[-3]) + (yylsp[0]), msg); break;
		case 2:  csh.AddCSH_Error((yylsp[-2]) + (yylsp[0]), msg); break;
		case 3:  csh.AddCSH_Error((yylsp[-1]) + (yylsp[0]), msg); break;
		case 4:  csh.AddCSH_Error((yylsp[0]), msg); break;
		default: csh.AddCSH_ErrorAfter((yyloc), msg); break;
		}
	}
  #else
	(yyval.shapeelement) = NULL;
	if (should_args > num_args)
		msc.Error.Error(MSC_POS((yyloc)).end, ShapeElement::ErrorMsg((yyvsp[-5].shapecommand), num_args).append(" Ignoring line."));
	else 
		(yyval.shapeelement) = new ShapeElement((yyvsp[-5].shapecommand), atof((yyvsp[-4].str)), atof((yyvsp[-3].str)), atof((yyvsp[-2].str)), atof((yyvsp[-1].str)), atof((yyvsp[0].str)));
  #endif	
  free((yyvsp[-4].str));
  free((yyvsp[-3].str));
  free((yyvsp[-2].str));
  free((yyvsp[-1].str));
  free((yyvsp[0].str));
}
#line 5832 "language.cc" /* yacc.c:1646  */
    break;

  case 174:
#line 2627 "language.yy" /* yacc.c:1646  */
    {
    const int num_args = 6;
	const int should_args = ShapeElement::GetNumArgs((yyvsp[-6].shapecommand));
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-6]), COLOR_KEYWORD);
	if (should_args != num_args) {
		const string msg = ShapeElement::ErrorMsg((yyvsp[-6].shapecommand), num_args);
	    switch (ShapeElement::GetNumArgs((yyvsp[-6].shapecommand))) {
		case 0:  csh.AddCSH_Error((yylsp[-5]) + (yylsp[0]), msg); break;
		case 1:  csh.AddCSH_Error((yylsp[-4]) + (yylsp[0]), msg); break;
		case 2:  csh.AddCSH_Error((yylsp[-3]) + (yylsp[0]), msg); break;
		case 3:  csh.AddCSH_Error((yylsp[-2]) + (yylsp[0]), msg); break;
		case 4:  csh.AddCSH_Error((yylsp[-1]) + (yylsp[0]), msg); break;
		case 5:  csh.AddCSH_Error((yylsp[0]), msg); break;
		default: csh.AddCSH_ErrorAfter((yyloc), msg); break;
		}
	}
  #else
	(yyval.shapeelement) = NULL;
	if (should_args > num_args)
		msc.Error.Error(MSC_POS((yyloc)).end, ShapeElement::ErrorMsg((yyvsp[-6].shapecommand), num_args).append(" Ignoring line."));
	else 
		(yyval.shapeelement) = new ShapeElement((yyvsp[-6].shapecommand), atof((yyvsp[-5].str)), atof((yyvsp[-4].str)), atof((yyvsp[-3].str)), atof((yyvsp[-2].str)), atof((yyvsp[-1].str)), atof((yyvsp[0].str)));
  #endif	
  free((yyvsp[-5].str));
  free((yyvsp[-4].str));
  free((yyvsp[-3].str));
  free((yyvsp[-2].str));
  free((yyvsp[-1].str));
  free((yyvsp[0].str));
}
#line 5868 "language.cc" /* yacc.c:1646  */
    break;

  case 176:
#line 2661 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_COMMA);
    if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_NAME)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
	}
  #endif
}
#line 5882 "language.cc" /* yacc.c:1646  */
    break;

  case 177:
#line 2671 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_COMMA);
    if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::ATTR_NAME)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
	}
	csh.AddCSH_ErrorAfter((yyloc), "Missing color name to (re-)define.");
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing a color name to (re-)define.");
  #endif
}
#line 5899 "language.cc" /* yacc.c:1646  */
    break;

  case 180:
#line 2687 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_COLORNAME);
    csh.AddCSH((yylsp[-1]), COLOR_EQUAL);
    csh.AddCSH((yylsp[0]), COLOR_COLORDEF);
    ColorType color = csh.Contexts.back().Colors.GetColor((yyvsp[0].str));
    if (color.type!=ColorType::INVALID)
        csh.Contexts.back().Colors[(yyvsp[-2].str)] = color;
    if (csh.CheckHintAt((yylsp[-2]), EHintSourceType::ATTR_NAME)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_VALUE)) {
        csh.AddColorValuesToHints(false);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Contexts.back().colors.AddColor((yyvsp[-2].str), (yyvsp[0].str), msc.Error, MSC_POS((yyloc)));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 5925 "language.cc" /* yacc.c:1646  */
    break;

  case 181:
#line 2709 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-3]), COLOR_COLORNAME);
    csh.AddCSH((yylsp[-2]), COLOR_EQUAL);
    csh.AddCSH((yylsp[-1]), COLOR_COLORDEF);
    csh.AddCSH((yylsp[0]), COLOR_COLORDEF);
    ColorType color = csh.Contexts.back().Colors.GetColor("++"+string((yyvsp[0].str)));
    if (color.type!=ColorType::INVALID)
        csh.Contexts.back().Colors[(yyvsp[-3].str)] = color;
    if (csh.CheckHintAt((yylsp[-3]), EHintSourceType::ATTR_NAME)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-2]), (yylsp[0]), EHintSourceType::ATTR_VALUE)) {
        csh.AddColorValuesToHints(false);
        csh.hintStatus = HINT_READY;
    }
#else
    msc.Contexts.back().colors.AddColor((yyvsp[-3].str), "++"+string((yyvsp[0].str)), msc.Error, MSC_POS((yyloc)));
  #endif
    free((yyvsp[-3].str));
    free((yyvsp[0].str));
}
#line 5952 "language.cc" /* yacc.c:1646  */
    break;

  case 182:
#line 2732 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_COLORNAME);
    csh.AddCSH((yylsp[0]), COLOR_EQUAL);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::ATTR_NAME)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::ATTR_VALUE)) {
        csh.AddColorValuesToHints(false);
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_ErrorAfter((yyloc), "Missing color definition.");
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing color definition.");
  #endif
    free((yyvsp[-1].str));
}
#line 5974 "language.cc" /* yacc.c:1646  */
    break;

  case 183:
#line 2750 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_COLORNAME);
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::ATTR_NAME)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
	}
	csh.AddCSH_ErrorAfter((yyloc), "Missing equal sign ('=') and a color definition.");
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing equal sign ('=') and a color definition.");
  #endif
    free((yyvsp[0].str));
}
#line 5992 "language.cc" /* yacc.c:1646  */
    break;

  case 184:
#line 2767 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-4]), COLOR_DESIGNNAME);
    csh.AddCSH((yylsp[-1]), COLOR_SEMICOLON);
    csh.AddCSH((yylsp[0]), COLOR_BRACE);
    (csh.Contexts.back().full ? csh.FullDesigns : csh.PartialDesigns)[(yyvsp[-4].str)] += csh.Contexts.back();
    csh.PopContext();
    if (csh.CheckHintBetween((yylsp[-3]), (yylsp[-2]), EHintSourceType::LINE_START) || 
        csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddDesignLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    //cope_open_empty pushed an empty color & style set onto the stack
    //then designelementlist added color & style definitions, now we harvest those
    if (msc.Contexts.back().is_full)
        msc.Designs[(yyvsp[-4].str)] = msc.Contexts.back();
    else
        msc.Designs[(yyvsp[-4].str)] += msc.Contexts.back();
    msc.PopContext();
  #endif
    free((yyvsp[-4].str));
}
#line 6020 "language.cc" /* yacc.c:1646  */
    break;

  case 185:
#line 2791 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-5]), COLOR_DESIGNNAME);
    csh.AddCSH((yylsp[-2]), COLOR_SEMICOLON);
    csh.AddCSH_Error((yylsp[-1]), "Could not recognize this as part of a design definition.");
    csh.AddCSH((yylsp[0]), COLOR_BRACE);
    (csh.Contexts.back().full ? csh.FullDesigns : csh.PartialDesigns)[(yyvsp[-5].str)] = csh.Contexts.back();
    csh.PopContext();
    if (csh.CheckHintBetween((yylsp[-4]), (yylsp[-3]), EHintSourceType::LINE_START) ||
        csh.CheckHintBetween((yylsp[-2]), (yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddDesignLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
#else
    //if closing brace missing, still do the design definition
    //cope_open_empty pushed an empty color & style set onto the stack
    //then designelementlist added color & style definitions, now we harvest those
    msc.Designs[(yyvsp[-5].str)] += msc.Contexts.back();
    msc.PopContext();
  #endif
}
#line 6046 "language.cc" /* yacc.c:1646  */
    break;

  case 186:
#line 2815 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_BRACE);
    csh.PushContext(true);
  #else
    //push empty color & style sets for design definition
    msc.PushContext(true);
  #endif
}
#line 6060 "language.cc" /* yacc.c:1646  */
    break;

  case 188:
#line 2827 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_SEMICOLON);
    if (csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddDesignLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
#endif
}
#line 6074 "language.cc" /* yacc.c:1646  */
    break;

  case 189:
#line 2838 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddDesignLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_NAME)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
    }
  #endif
    free((yyvsp[-1].str));
}
#line 6092 "language.cc" /* yacc.c:1646  */
    break;

  case 190:
#line 2852 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddDesignLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::ATTR_NAME)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
	}
	csh.AddCSH_ErrorAfter((yyloc), "Missing color name to (re-)define.");
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing a color name to (re-)define.");
  #endif
    free((yyvsp[0].str));
}
#line 6113 "language.cc" /* yacc.c:1646  */
    break;

  case 191:
#line 2869 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddDesignLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintBetweenPlusOne((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_NAME)) {
        csh.AddStylesToHints(true, true);
        csh.hintStatus = HINT_READY;
	}
  #endif
    free((yyvsp[-1].str));
}
#line 6131 "language.cc" /* yacc.c:1646  */
    break;

  case 192:
#line 2883 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddDesignLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::ATTR_NAME)) {
        csh.AddStylesToHints(true, true);
        csh.hintStatus = HINT_READY;
	}
	csh.AddCSH_ErrorAfter((yyloc), "Missing style name to (re-)define.");
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing a style name to (re-)define.");
  #endif
    free((yyvsp[0].str));
}
#line 6152 "language.cc" /* yacc.c:1646  */
    break;

  case 195:
#line 2903 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_COMMA);
    if (csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    }   
  #endif
}
#line 6166 "language.cc" /* yacc.c:1646  */
    break;

  case 196:
#line 2913 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_COMMA);
    if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    }   
  #endif
}
#line 6180 "language.cc" /* yacc.c:1646  */
    break;

  case 197:
#line 2923 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[0]), "Extra stuff after design options. Maybe missing a comma?");
  #endif
}
#line 6190 "language.cc" /* yacc.c:1646  */
    break;

  case 198:
#line 2930 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName((yylsp[-2]), (yyvsp[-2].str), COLOR_OPTIONNAME);
    csh.AddCSH((yylsp[-1]), COLOR_EQUAL);
    csh.AddCSH((yylsp[0]), COLOR_ATTRVALUE);
    if (csh.CheckHintAt((yylsp[-2]), EHintSourceType::ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_VALUE, (yyvsp[-2].str))) {
        Msc::AttributeValues((yyvsp[-2].str), csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.AddDesignAttribute(Attribute((yyvsp[-2].str), atof((yyvsp[0].str)), MSC_POS((yyloc)), MSC_POS((yylsp[0])), (yyvsp[0].str)));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 6213 "language.cc" /* yacc.c:1646  */
    break;

  case 199:
#line 2949 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName((yylsp[-2]), (yyvsp[-2].str), COLOR_OPTIONNAME);
    csh.AddCSH((yylsp[-1]), COLOR_EQUAL);
    csh.AddCSH((yylsp[0]), COLOR_ATTRVALUE);
    if (csh.CheckHintAt((yylsp[-2]), EHintSourceType::ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_VALUE, (yyvsp[-2].str))) {
        Msc::AttributeValues((yyvsp[-2].str), csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.AddDesignAttribute(Attribute((yyvsp[-2].str), (yyvsp[0].str), MSC_POS((yylsp[-2])), MSC_POS((yylsp[0]))));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 6236 "language.cc" /* yacc.c:1646  */
    break;

  case 200:
#line 2968 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName((yylsp[-1]), (yyvsp[-1].str), COLOR_OPTIONNAME);
    csh.AddCSH((yylsp[0]), COLOR_EQUAL);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::ATTR_VALUE, (yyvsp[-1].str))) {
        Msc::AttributeValues((yyvsp[-1].str), csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing option value. Ignoring this.");
#endif
    free((yyvsp[-1].str));
}
#line 6257 "language.cc" /* yacc.c:1646  */
    break;

  case 201:
#line 2985 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[-1]), COLOR_EQUAL);
    csh.AddCSH((yylsp[0]), COLOR_DESIGNNAME);
    if (csh.CheckHintAt((yylsp[-2]), EHintSourceType::ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_VALUE, (yyvsp[-2].str))) {
        Msc::AttributeValues("msc", csh);
        csh.hintStatus = HINT_READY;
    }
    std::string msg = csh.SetDesignTo((yyvsp[0].str), true);
    if (msg.length())
        csh.AddCSH_Error((yylsp[0]), msg.c_str());
  #else
    msc.AddDesignAttribute(Attribute("msc", (yyvsp[0].str), MSC_POS((yyloc)), MSC_POS((yylsp[0]))));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 6283 "language.cc" /* yacc.c:1646  */
    break;

  case 202:
#line 3007 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[-1]), COLOR_EQUAL);
    csh.AddCSH((yylsp[0]), COLOR_DESIGNNAME);
    if (csh.CheckHintAt((yylsp[-2]), EHintSourceType::ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_VALUE, (yyvsp[-2].str))) {
        Msc::AttributeValues("msc+", csh);
        csh.hintStatus = HINT_READY;
    }
    std::string msg = csh.SetDesignTo((yyvsp[0].str), false);
    if (msg.length())
        csh.AddCSH_Error((yylsp[0]), msg.c_str());
  #else
    msc.AddDesignAttribute(Attribute("msc+", (yyvsp[0].str), MSC_POS((yyloc)), MSC_POS((yylsp[0]))));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 6309 "language.cc" /* yacc.c:1646  */
    break;

  case 203:
#line 3029 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName((yylsp[-1]), (yyvsp[-1].str), COLOR_OPTIONNAME);
    csh.AddCSH((yylsp[0]), COLOR_EQUAL);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::ATTR_VALUE, "msc")) {
        Msc::AttributeValues((yyvsp[-1].str), csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing design name. Ignoring this.");
#endif
    free((yyvsp[-1].str));
}
#line 6330 "language.cc" /* yacc.c:1646  */
    break;

  case 204:
#line 3046 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName((yylsp[-1]), (yyvsp[-1].str), COLOR_OPTIONNAME);
    csh.AddCSH((yylsp[0]), COLOR_EQUAL);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::ATTR_VALUE, "msc+")) {
        Msc::AttributeValues((yyvsp[-1].str), csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing design name. Ignoring this.");
#endif
    free((yyvsp[-1].str));
}
#line 6351 "language.cc" /* yacc.c:1646  */
    break;

  case 205:
#line 3065 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[0].arclist))
        (yyval.arcparallel) = new ArcParallel(&msc, (yyvsp[0].arclist), NULL);
    else
        (yyval.arcparallel) = NULL;
  #endif
}
#line 6364 "language.cc" /* yacc.c:1646  */
    break;

  case 206:
#line 3074 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[-1])))
        ArcParallel::AttributeNames(csh, true);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[-1])))
        ArcParallel::AttributeValues(csh.hintAttrName, csh, true);
  #else 
    if ((yyvsp[0].arclist)) {
        (yyval.arcparallel) = new ArcParallel(&msc, (yyvsp[0].arclist), (yyvsp[-1].attriblist));
    } else {
        (yyval.arcparallel) = NULL;
        if ((yyvsp[-1].attriblist)) delete (yyvsp[-1].attriblist);
    }
  #endif
}
#line 6384 "language.cc" /* yacc.c:1646  */
    break;

  case 207:
#line 3090 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[0].arclist)==NULL)
        (yyval.arcparallel) = (yyvsp[-1].arcparallel);
    else if ((yyvsp[-1].arcparallel))
        (yyval.arcparallel) = ((yyvsp[-1].arcparallel))->AddArcList((yyvsp[0].arclist), NULL);
    else
        (yyval.arcparallel) = new ArcParallel(&msc, (yyvsp[0].arclist), NULL);
  #endif
}
#line 6399 "language.cc" /* yacc.c:1646  */
    break;

  case 208:
#line 3101 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[-1])))
        ArcParallel::AttributeNames(csh, false);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[-1])))
        ArcParallel::AttributeValues(csh.hintAttrName, csh, false);
  #else 
    if ((yyvsp[0].arclist)==NULL) {
        (yyval.arcparallel) = (yyvsp[-2].arcparallel);
        if ((yyvsp[-1].attriblist)) delete (yyvsp[-1].attriblist);
    } else if ((yyvsp[-2].arcparallel))
        (yyval.arcparallel) = ((yyvsp[-2].arcparallel))->AddArcList((yyvsp[0].arclist), (yyvsp[-1].attriblist));
    else
        (yyval.arcparallel) = new ArcParallel(&msc, (yyvsp[0].arclist), (yyvsp[-1].attriblist));
  #endif
}
#line 6420 "language.cc" /* yacc.c:1646  */
    break;

  case 209:
#line 3118 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0])))
        ArcParallel::AttributeNames(csh, false);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0])))
        ArcParallel::AttributeValues(csh.hintAttrName, csh, false);
    csh.AddCSH_ErrorAfter((yylsp[0]), 
        "Need an additional parallel block enclosed between '{' and '}'.");
  #else 
    (yyval.arcparallel) = (yyvsp[-1].arcparallel);
    if ((yyvsp[0].attriblist)) delete (yyvsp[0].attriblist);
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(),
        "Missing an additional parallel block enclosed between '{' and '}' after the attributes.");
  #endif
}
#line 6440 "language.cc" /* yacc.c:1646  */
    break;

  case 210:
#line 3140 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    ((yyvsp[0].arcbox))->SetLineEnd(MSC_POS((yyloc)));
    (yyval.arcboxseries) = new ArcBoxSeries((yyvsp[0].arcbox));
  #endif
}
#line 6453 "language.cc" /* yacc.c:1646  */
    break;

  case 211:
#line 3149 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else
        csh.CheckEntityHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcboxseries) = NULL;
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), 
        "The keyword '" + string((yyvsp[0].str)) +
        "' should be followed by an entity, or '--', '..', '++' or '=='. "
        "Ignoring this box (series).");
  #endif
    free((yyvsp[0].str));
}
#line 6475 "language.cc" /* yacc.c:1646  */
    break;

  case 212:
#line 3167 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);   
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else
        csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    ((yyvsp[0].arcbox))->ExpandLineEnd(MSC_POS((yyloc)));
    (yyval.arcboxseries) = new ArcBoxSeries((yyvsp[0].arcbox));
  #endif
  free((yyvsp[-1].str));
}
#line 6494 "language.cc" /* yacc.c:1646  */
    break;

  case 213:
#line 3183 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[0].arcbox))->SetLineEnd(MSC_POS((yylsp[0])));
    (yyval.arcboxseries) = ((yyvsp[-1].arcboxseries))->AddBox((yyvsp[0].arcbox));
    ((yyvsp[0].arcbox))->AddAttributeList(NULL); //should come after AddBox
  #endif
}
#line 6506 "language.cc" /* yacc.c:1646  */
    break;

  case 214:
#line 3191 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0])))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0])))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
    else 
        csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    ((yyvsp[-1].arcbox))->SetLineEnd(MSC_POS2((yylsp[-1]), (yylsp[0])));

    (yyval.arcboxseries) = ((yyvsp[-2].arcboxseries))->AddBox((yyvsp[-1].arcbox));
    ((yyvsp[-1].arcbox))->AddAttributeList((yyvsp[0].attriblist)); //should come after AddBox
  #endif
}
#line 6526 "language.cc" /* yacc.c:1646  */
    break;

  case 215:
#line 3207 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    ((yyvsp[-1].arcbox))->AddArcList((yyvsp[0].arclist))->SetLineEnd(MSC_POS((yylsp[-1])));
    (yyval.arcboxseries) = ((yyvsp[-2].arcboxseries))->AddBox((yyvsp[-1].arcbox));
    ((yyvsp[-1].arcbox))->AddAttributeList(NULL); //should come after AddBox
  #endif
}
#line 6540 "language.cc" /* yacc.c:1646  */
    break;

  case 216:
#line 3217 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    if ((yyvsp[-1].arcboxseries)) {
        ArcBox *temp = new ArcBox(EBoxSymbol::UNDETERMINED_FOLLOW, NULL, MSC_POS((yylsp[-1])), NULL, MSC_POS((yylsp[-1])), &msc);
        temp->AddArcList((yyvsp[0].arclist));
        (yyval.arcboxseries) = ((yyvsp[-1].arcboxseries))->AddBox(temp);
        temp->AddAttributeList(NULL); //should come after AddBox
    } else {
        //We have emitted an error message when returning a NULL 
        //ArcBoxSeries for 'box_list', so we delete 'temp' and ignore
        //this box series entirely.
        (yyval.arcboxseries) = NULL;
        delete ((yyvsp[0].arclist));
    }
  #endif
}
#line 6563 "language.cc" /* yacc.c:1646  */
    break;

  case 217:
#line 3236 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[-2])))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[-2])))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
    else 
        csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    ((yyvsp[-2].arcbox))->AddArcList((yyvsp[0].arclist))->SetLineEnd(MSC_POS2((yylsp[-2]), (yylsp[-1])));
    (yyval.arcboxseries) = ((yyvsp[-3].arcboxseries))->AddBox((yyvsp[-2].arcbox));
    ((yyvsp[-2].arcbox))->AddAttributeList((yyvsp[-1].attriblist)); //should come after AddBox
  #endif
}
#line 6582 "language.cc" /* yacc.c:1646  */
    break;

  case 218:
#line 3251 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[-1])))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[-1])))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
    else     
        csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    if ((yyvsp[-2].arcboxseries)) {
        ArcBox *temp = new ArcBox(EBoxSymbol::UNDETERMINED_FOLLOW, NULL, MSC_POS((yylsp[-2])), NULL, MSC_POS((yylsp[-2])), &msc);
        temp->AddArcList((yyvsp[0].arclist))->SetLineEnd(MSC_POS((yylsp[-1])));
        (yyval.arcboxseries) = ((yyvsp[-2].arcboxseries))->AddBox(temp);
        temp->AddAttributeList((yyvsp[-1].attriblist)); //should come after AddBox
    } else {
        //We have emitted an error message when returning a NULL 
        //ArcBoxSeries for 'box_list', so we delete 'temp' and ignore
        //this box series entirely.
        (yyval.arcboxseries) = NULL;
        delete ((yyvsp[-1].attriblist));
        delete ((yyvsp[0].arclist));
    }

  #endif
}
#line 6612 "language.cc" /* yacc.c:1646  */
    break;

  case 219:
#line 3279 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[0].arcbox))->AddAttributeList(NULL);
    ((yyvsp[0].arcbox))->SetLineEnd(MSC_POS((yyloc)), false);
    (yyval.arcbox) = (yyvsp[0].arcbox);
  #endif
}
#line 6624 "language.cc" /* yacc.c:1646  */
    break;

  case 220:
#line 3287 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0])))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0])))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
  #else
    ((yyvsp[-1].arcbox))->AddAttributeList((yyvsp[0].attriblist));
    ((yyvsp[-1].arcbox))->SetLineEnd(MSC_POS((yyloc)), false);
    (yyval.arcbox) = ((yyvsp[-1].arcbox));
  #endif
}
#line 6641 "language.cc" /* yacc.c:1646  */
    break;

  case 221:
#line 3300 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[-1].arcbox))->AddAttributeList(NULL);
    ((yyvsp[-1].arcbox))->SetLineEnd(MSC_POS((yylsp[-1])), false);
    (yyval.arcbox) = ((yyvsp[-1].arcbox))->AddArcList((yyvsp[0].arclist));
  #endif
}
#line 6653 "language.cc" /* yacc.c:1646  */
    break;

  case 222:
#line 3308 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[-1])))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[-1])))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
  #else
    ((yyvsp[-2].arcbox))->SetLineEnd(MSC_POS2((yylsp[-2]), (yylsp[-1])), false);
    ((yyvsp[-2].arcbox))->AddArcList((yyvsp[0].arclist))->AddAttributeList((yyvsp[-1].attriblist));
    (yyval.arcbox) = ((yyvsp[-2].arcbox));
  #endif
}
#line 6670 "language.cc" /* yacc.c:1646  */
    break;

  case 223:
#line 3323 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintBetweenPlusOne((yylsp[-1]), (yylsp[0]), EHintSourceType::ENTITY)) {
        csh.AddEntitiesToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.arcpipe) = new ArcPipe((yyvsp[0].arcbox));
    ((yyval.arcpipe))->AddAttributeList(NULL);
    ((yyval.arcpipe))->SetLineEnd(MSC_POS((yyloc)));
  #endif
    free((yyvsp[-1].str));
}
#line 6692 "language.cc" /* yacc.c:1646  */
    break;

  case 224:
#line 3341 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    csh.AddCSH_EntityName((yylsp[-1]), (yyvsp[-1].str));
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBeforePlusOne((yylsp[-1]), (yylsp[0]), EHintSourceType::ENTITY)) {
        csh.AddEntitiesToHints();
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing a box symbol.");
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing a box symbol. Ignoring pipe.");
    (yyval.arcpipe) = NULL;
  #endif
    free((yyvsp[-1].str));
}
#line 6715 "language.cc" /* yacc.c:1646  */
    break;

  case 225:
#line 3360 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else
        csh.CheckEntityHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcpipe) = NULL;
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "The keyword '" + string((yyvsp[0].str)) +"' should be followed by an entity, or '--', '..', '++' or '=='.");
  #endif
    free((yyvsp[0].str));
}
#line 6734 "language.cc" /* yacc.c:1646  */
    break;

  case 226:
#line 3375 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-2]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintBetweenPlusOne((yylsp[-2]), (yylsp[-1]), EHintSourceType::ENTITY)) {
        csh.AddEntitiesToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0])))
        ArcPipe::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0])))
        ArcPipe::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcpipe) = new ArcPipe((yyvsp[-1].arcbox));
    ((yyval.arcpipe))->AddAttributeList((yyvsp[0].attriblist));
    ((yyval.arcpipe))->SetLineEnd(MSC_POS((yyloc)));
  #endif
    free((yyvsp[-2].str));
}
#line 6759 "language.cc" /* yacc.c:1646  */
    break;

  case 227:
#line 3397 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((yyloc), yylloc, yychar==YYEOF);
  #else
    if ((yyvsp[0].arcpipe))
      (yyval.arcpipeseries) = new ArcPipeSeries((yyvsp[0].arcpipe));
    else
      (yyval.arcpipeseries) = NULL;
  #endif
}
#line 6774 "language.cc" /* yacc.c:1646  */
    break;

  case 228:
#line 3408 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((yyloc), yylloc, yychar==YYEOF);
#else
    //($2) is never NULL: "boxrel" always return a value (except oo memory)
    ArcPipe *ap = new ArcPipe((yyvsp[0].arcbox));
    ap->SetLineEnd(MSC_POS((yylsp[0])));
    if ((yyvsp[-1].arcpipeseries))
      (yyval.arcpipeseries) = ((yyvsp[-1].arcpipeseries))->AddFollowWithAttributes(ap, NULL);
    else {
      ap->AddAttributeList(NULL);
      (yyval.arcpipeseries) = new ArcPipeSeries(ap);
    }
  #endif
}
#line 6794 "language.cc" /* yacc.c:1646  */
    break;

  case 229:
#line 3424 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0])))
        ArcPipe::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0])))
        ArcPipe::AttributeValues(csh.hintAttrName, csh);
    else
        csh.CheckEntityHintAfter((yyloc), yylloc, yychar==YYEOF);
  #else
    //($2) is never NULL: "boxrel" always return a value (except oo memory)
    ArcPipe *ap = new ArcPipe((yyvsp[-1].arcbox));
    ap->SetLineEnd(MSC_POS((yylsp[-1])));
    if ((yyvsp[-2].arcpipeseries))
      (yyval.arcpipeseries) = ((yyvsp[-2].arcpipeseries))->AddFollowWithAttributes(ap, (yyvsp[0].attriblist));
    else {
      ap->AddAttributeList((yyvsp[0].attriblist));
      (yyval.arcpipeseries) = new ArcPipeSeries(ap);
    }
  #endif
}
#line 6819 "language.cc" /* yacc.c:1646  */
    break;

  case 231:
#line 3447 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintBetween((yylsp[-1]), (yylsp[0]));
  #else
    (yyval.arcpipeseries) = ((yyvsp[-1].arcpipeseries))->AddArcList((yyvsp[0].arclist));
  #endif
}
#line 6831 "language.cc" /* yacc.c:1646  */
    break;

  case 234:
#line 3458 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[-2]));
    csh.AddCSH_EntityName((yylsp[-2]), (yyvsp[-2].str));
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcbox) = new ArcBox((yyvsp[-1].boxsymbol), (yyvsp[-2].str), MSC_POS((yylsp[-2])), (yyvsp[0].str), MSC_POS((yylsp[0])), &msc);
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 6849 "language.cc" /* yacc.c:1646  */
    break;

  case 235:
#line 3472 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcbox) = new ArcBox((yyvsp[-1].boxsymbol), NULL, MSC_POS((yylsp[-1])), (yyvsp[0].str), MSC_POS((yylsp[0])), &msc);
  #endif
    free((yyvsp[0].str));
}
#line 6864 "language.cc" /* yacc.c:1646  */
    break;

  case 236:
#line 3483 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[-1]));
    csh.AddCSH_EntityName((yylsp[-1]), (yyvsp[-1].str));
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcbox) = new ArcBox((yyvsp[0].boxsymbol), (yyvsp[-1].str), MSC_POS((yylsp[-1])), NULL, MSC_POS((yylsp[0])), &msc);
  #endif
    free((yyvsp[-1].str));
}
#line 6880 "language.cc" /* yacc.c:1646  */
    break;

  case 237:
#line 3495 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcbox) = new ArcBox((yyvsp[0].boxsymbol), NULL, MSC_POS((yylsp[0])), NULL, MSC_POS((yylsp[0])), &msc);
  #endif
}
#line 6893 "language.cc" /* yacc.c:1646  */
    break;

  case 238:
#line 3505 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckEntityHintAtAndBeforePlusOne((yylsp[-1]), (yylsp[0]))) {
        csh.AddVertXPosSyntaxNonSelectableToHints(false);
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[0].str), MSC_POS((yylsp[0])));
  #endif
    free((yyvsp[-1].str));
    free((yyvsp[0].str));
}
#line 6915 "language.cc" /* yacc.c:1646  */
    break;

  case 239:
#line 3523 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-2]), EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckEntityHintAtAndBeforePlusOne((yylsp[-2]), (yylsp[-1]))) {
        csh.AddVertXPosSyntaxNonSelectableToHints(false);
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_EntityName((yylsp[-1]), (yyvsp[-1].str));
    csh.AddCSH((yylsp[0]), COLOR_ATTRVALUE);
  #else
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[-1].str), MSC_POS((yylsp[-1])), VertXPos::POS_AT, atof((yyvsp[0].str)));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[-1].str));
    free((yyvsp[0].str));
}
#line 6939 "language.cc" /* yacc.c:1646  */
    break;

  case 240:
#line 3543 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.AddCSH_EntityName((yylsp[-1]), (yyvsp[-1].str));
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    if (csh.CheckHintAt((yylsp[-2]), EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckEntityHintAtAndBeforePlusOne((yylsp[-2]), (yylsp[-1]))) {
        csh.AddVertXPosSyntaxNonSelectableToHints(false);
        csh.hintStatus = HINT_READY;
    } else
        csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[-1].str), MSC_POS((yylsp[-1])), VertXPos::POS_LEFT_SIDE);
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[-1].str));
}
#line 6963 "language.cc" /* yacc.c:1646  */
    break;

  case 241:
#line 3563 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-3]), COLOR_KEYWORD);
    csh.AddCSH_EntityName((yylsp[-2]), (yyvsp[-2].str));
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    csh.AddCSH((yylsp[0]), COLOR_ATTRVALUE);
    if (csh.CheckHintAt((yylsp[-3]), EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckEntityHintAtAndBeforePlusOne((yylsp[-3]), (yylsp[-2]))) {
        csh.AddVertXPosSyntaxNonSelectableToHints(false);
        csh.hintStatus = HINT_READY;
    } 
  #else
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[-2].str), MSC_POS((yylsp[-2])), VertXPos::POS_LEFT_SIDE, atof((yyvsp[0].str)));
  #endif
    free((yyvsp[-3].str));
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 6988 "language.cc" /* yacc.c:1646  */
    break;

  case 242:
#line 3584 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.AddCSH_EntityName((yylsp[-1]), (yyvsp[-1].str));
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    if (csh.CheckHintAt((yylsp[-2]), EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckEntityHintAtAndBeforePlusOne((yylsp[-2]), (yylsp[-1]))) {
        csh.AddVertXPosSyntaxNonSelectableToHints(false);
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[-1].str), MSC_POS((yylsp[-1])), VertXPos::POS_RIGHT_SIDE);
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[-1].str));
}
#line 7011 "language.cc" /* yacc.c:1646  */
    break;

  case 243:
#line 3603 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-3]), COLOR_KEYWORD);
    csh.AddCSH_EntityName((yylsp[-2]), (yyvsp[-2].str));
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    csh.AddCSH((yylsp[0]), COLOR_ATTRVALUE);
    if (csh.CheckHintAt((yylsp[-3]), EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckEntityHintAtAndBeforePlusOne((yylsp[-3]), (yylsp[-2]))) {
        csh.AddVertXPosSyntaxNonSelectableToHints(false);
        csh.hintStatus = HINT_READY;
    }
#else
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[-2].str), MSC_POS((yylsp[-2])), VertXPos::POS_RIGHT_SIDE, atof((yyvsp[0].str)));
  #endif
    free((yyvsp[-3].str));
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 7036 "language.cc" /* yacc.c:1646  */
    break;

  case 244:
#line 3624 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.AddCSH_EntityName((yylsp[-1]), (yyvsp[-1].str));
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    if (csh.CheckHintAt((yylsp[-2]), EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckEntityHintAtAndBeforePlusOne((yylsp[-2]), (yylsp[-1]))) {
        csh.AddVertXPosSyntaxNonSelectableToHints(false);
        csh.hintStatus = HINT_READY;
    }
  #else
    switch ((yyvsp[0].boxsymbol)) {
    default:
        _ASSERT(0);
    case EBoxSymbol::SOLID:
        (yyval.vertxpos) = new VertXPos(msc, (yyvsp[-1].str), MSC_POS((yylsp[-1])), VertXPos::POS_LEFT_BY);
        break;
    case EBoxSymbol::DASHED:
        (yyval.vertxpos) = new VertXPos(msc, (yyvsp[-1].str), MSC_POS((yylsp[-1])), VertXPos::POS_RIGHT_BY);
        break;
    case EBoxSymbol::DOTTED:
        msc.Error.Error(MSC_POS((yylsp[0])).start,
                        "unexpected '..', expected '-', '--', '+' or '++'."
                        " Ignoring vertical."); 
        (yyval.vertxpos) = NULL;
        break;
    case EBoxSymbol::DOUBLE:
        msc.Error.Error(MSC_POS((yylsp[0])).start,
                        "unexpected '==', expected '-', '--', '+' or '++'."
                        " Ignoring vertical."); 
        (yyval.vertxpos) = NULL;
        break;
    }
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[-1].str));
}
#line 7080 "language.cc" /* yacc.c:1646  */
    break;

  case 245:
#line 3664 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-3]), COLOR_KEYWORD);
    csh.AddCSH_EntityName((yylsp[-2]), (yyvsp[-2].str));
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
    if (csh.CheckHintAt((yylsp[-3]), EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckEntityHintAtAndBeforePlusOne((yylsp[-3]), (yylsp[-2]))) {
        csh.AddVertXPosSyntaxNonSelectableToHints(false);
        csh.hintStatus = HINT_READY;
    } else
        csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
  #else
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[-2].str), MSC_POS((yylsp[-2])), (yyvsp[0].str), MSC_POS((yylsp[0])));
  #endif
    free((yyvsp[-3].str));
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 7106 "language.cc" /* yacc.c:1646  */
    break;

  case 246:
#line 3686 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-3]), COLOR_KEYWORD);
    csh.AddCSH_EntityName((yylsp[-2]), (yyvsp[-2].str));
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
    if (csh.CheckHintAt((yylsp[-3]), EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckEntityHintAtAndBeforePlusOne((yylsp[-3]), (yylsp[-2]))) {
        csh.AddVertXPosSyntaxNonSelectableToHints(false);
        csh.hintStatus = HINT_READY;
    } else
        csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
  #else
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[-2].str), MSC_POS((yylsp[-2])), (yyvsp[0].str), MSC_POS((yylsp[0])));
  #endif
    free((yyvsp[-3].str));
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 7132 "language.cc" /* yacc.c:1646  */
    break;

  case 247:
#line 3708 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-4]), COLOR_KEYWORD);
    csh.AddCSH_EntityName((yylsp[-3]), (yyvsp[-3].str));
    csh.AddCSH((yylsp[-2]), COLOR_SYMBOL);
    csh.AddCSH_EntityName((yylsp[-1]), (yyvsp[-1].str));
    csh.AddCSH((yylsp[0]), COLOR_ATTRVALUE);
    if (csh.CheckHintAt((yylsp[-4]), EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckEntityHintAtAndBeforePlusOne((yylsp[-4]), (yylsp[-3]))) {
        csh.AddVertXPosSyntaxNonSelectableToHints(false);
        csh.hintStatus = HINT_READY;
    } else
        csh.CheckEntityHintAtAndBefore((yylsp[-2]), (yylsp[-1]));
  #else
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[-3].str), MSC_POS((yylsp[-3])), (yyvsp[-1].str), MSC_POS((yylsp[-1])), atof((yyvsp[0].str)));
  #endif
    free((yyvsp[-4].str));
    free((yyvsp[-3].str));
    free((yyvsp[-1].str));
    free((yyvsp[0].str));
}
#line 7160 "language.cc" /* yacc.c:1646  */
    break;

  case 248:
#line 3732 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-4]), COLOR_KEYWORD);
    csh.AddCSH_EntityName((yylsp[-3]), (yyvsp[-3].str));
    csh.AddCSH((yylsp[-2]), COLOR_SYMBOL);
    csh.AddCSH_EntityName((yylsp[-1]), (yyvsp[-1].str));
    csh.AddCSH((yylsp[0]), COLOR_ATTRVALUE);
    if (csh.CheckHintAt((yylsp[-4]), EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckEntityHintAtAndBeforePlusOne((yylsp[-4]), (yylsp[-3]))) {
        csh.AddVertXPosSyntaxNonSelectableToHints(false);
        csh.hintStatus = HINT_READY;
    } else
        csh.CheckEntityHintAtAndBefore((yylsp[-2]), (yylsp[-1]));
  #else
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[-3].str), MSC_POS((yylsp[-3])), (yyvsp[-1].str), MSC_POS((yylsp[-1])), atof((yyvsp[0].str)));
  #endif
    free((yyvsp[-4].str));
    free((yyvsp[-3].str));
    free((yyvsp[-1].str));
    free((yyvsp[0].str));
}
#line 7188 "language.cc" /* yacc.c:1646  */
    break;

  case 249:
#line 3756 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckEntityHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(false);
        csh.hintStatus = HINT_READY;
    } 
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing an entity name.");
  #else
    (yyval.vertxpos) = NULL;
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing an entity name.");
  #endif
    free((yyvsp[0].str));
}
#line 7210 "language.cc" /* yacc.c:1646  */
    break;

  case 250:
#line 3776 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
	(yyval.arctypeplusdir) = 0; //dummy to supress warning
  #else
    ((yyval.arctypeplusdir)).arc.type = (yyvsp[0].boxsymbol);
	((yyval.arctypeplusdir)).arc.lost = EArrowLost::NOT;
	((yyval.arctypeplusdir)).dir = EDirType::INDETERMINATE;
  #endif
}
#line 7225 "language.cc" /* yacc.c:1646  */
    break;

  case 251:
#line 3787 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    csh.CheckEntityHintBetween((yylsp[-1]), (yylsp[0]));
	(yyval.arctypeplusdir) = 0; //dummy to supress warning	
  #else
    ((yyval.arctypeplusdir)).arc.type = (yyvsp[0].boxsymbol);
	((yyval.arctypeplusdir)).arc.lost = EArrowLost::AT_SRC;
    ((yyval.arctypeplusdir)).arc.lost_pos.SetFrom(MSC_POS((yylsp[-1])));
	((yyval.arctypeplusdir)).dir = EDirType::INDETERMINATE;
  #endif
}
#line 7242 "language.cc" /* yacc.c:1646  */
    break;

  case 252:
#line 3800 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
	(yyval.arctypeplusdir) = 0; //dummy to supress warning	
  #else
    ((yyval.arctypeplusdir)).arc.type = (yyvsp[-1].boxsymbol);
	((yyval.arctypeplusdir)).arc.lost = EArrowLost::AT_SRC;
    ((yyval.arctypeplusdir)).arc.lost_pos.SetFrom(MSC_POS((yylsp[0])));
	((yyval.arctypeplusdir)).dir = EDirType::INDETERMINATE;
  #endif
}
#line 7258 "language.cc" /* yacc.c:1646  */
    break;

  case 253:
#line 3812 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_SYMBOL);
    csh.AddCSH_Error((yylsp[0]), MULTIPLE_ASTERISK_ERROR_MSG);
    csh.CheckEntityHintBetween((yylsp[-2]), (yylsp[-1]));
    (yyval.arctypeplusdir) = 0; //dummy to supress warning	
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).start, "Only one loss can be specified. Ignoring second asterisk ('*').");
    ((yyval.arctypeplusdir)).arc.type = (yyvsp[-1].boxsymbol);
    ((yyval.arctypeplusdir)).arc.lost = EArrowLost::AT_SRC;
    ((yyval.arctypeplusdir)).arc.lost_pos.SetFrom(MSC_POS((yylsp[-2])));
	((yyval.arctypeplusdir)).dir = EDirType::INDETERMINATE;
  #endif
}
#line 7277 "language.cc" /* yacc.c:1646  */
    break;

  case 254:
#line 3827 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
	(yyval.arctypeplusdir) = 0; //dummy to supress warning
  #else
    ((yyval.arctypeplusdir)).arc = (yyvsp[0].arcsegdata);
	((yyval.arctypeplusdir)).dir = EDirType::LEFT;
  #endif
}
#line 7290 "language.cc" /* yacc.c:1646  */
    break;

  case 255:
#line 3836 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
	(yyval.arctypeplusdir) = 0; //dummy to supress warning
  #else
    ((yyval.arctypeplusdir)).arc = (yyvsp[0].arcsegdata);
	((yyval.arctypeplusdir)).dir = EDirType::RIGHT;
  #endif
}
#line 7303 "language.cc" /* yacc.c:1646  */
    break;

  case 256:
#line 3845 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
	(yyval.arctypeplusdir) = 0; //dummy to supress warning
  #else
    ((yyval.arctypeplusdir)).arc = (yyvsp[0].arcsegdata);
	((yyval.arctypeplusdir)).dir = EDirType::BIDIR;
  #endif
}
#line 7316 "language.cc" /* yacc.c:1646  */
    break;

  case 257:
#line 3855 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_MARKERNAME);
    csh.AddCSH((yylsp[0]), COLOR_MARKERNAME);
    csh.CheckHintAt((yylsp[-2]), EHintSourceType::MARKER);
    csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), EHintSourceType::MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[-1].arctypeplusdir), (yyvsp[-2].str), (yyvsp[0].str), &msc);
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 7333 "language.cc" /* yacc.c:1646  */
    break;

  case 258:
#line 3868 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_MARKERNAME);
    csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), EHintSourceType::MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[-1].arctypeplusdir), MARKER_HERE_STR, (yyvsp[0].str), &msc);
  #endif
    free((yyvsp[0].str));
}
#line 7347 "language.cc" /* yacc.c:1646  */
    break;

  case 259:
#line 3878 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_MARKERNAME);
    csh.CheckHintAt((yylsp[-1]), EHintSourceType::MARKER);
    csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[0].arctypeplusdir), (yyvsp[-1].str), MARKER_HERE_STR, &msc);
  #endif
    free((yyvsp[-1].str));
}
#line 7362 "language.cc" /* yacc.c:1646  */
    break;

  case 260:
#line 3889 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[0].arctypeplusdir), MARKER_HERE_STR, MARKER_HERE_STR, &msc);
  #endif
}
#line 7374 "language.cc" /* yacc.c:1646  */
    break;

  case 261:
#line 3897 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_MARKERNAME);
    csh.CheckHintAt((yylsp[0]), EHintSourceType::MARKER);
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing a box or arrow symbol.");
  #else
    (yyval.arcvertarrow) = NULL;
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing a box or arrow symbol.");
  #endif
    free((yyvsp[0].str));
}
#line 7390 "language.cc" /* yacc.c:1646  */
    break;

  case 262:
#line 3909 "language.yy" /* yacc.c:1646  */
    {free((yyvsp[-1].str)); (yyval.arcvertarrow)=NULL;}
#line 7396 "language.cc" /* yacc.c:1646  */
    break;

  case 263:
#line 3910 "language.yy" /* yacc.c:1646  */
    {free((yyvsp[-3].str)); free((yyvsp[-1].str)); (yyval.arcvertarrow)=NULL;}
#line 7402 "language.cc" /* yacc.c:1646  */
    break;

  case 264:
#line 3914 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    if ((yyvsp[-1].arcvertarrow))
        (yyval.arcvertarrow) = ((yyvsp[-1].arcvertarrow))->AddXpos((yyvsp[0].vertxpos));
    else
        (yyval.arcvertarrow) = NULL;
    delete (yyvsp[0].vertxpos);
  #endif
}
#line 7417 "language.cc" /* yacc.c:1646  */
    break;

  case 265:
#line 3925 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    }
#else
    if ((yyvsp[0].arcvertarrow)) {
        VertXPos vxp(msc);
        (yyval.arcvertarrow) = ((yyvsp[0].arcvertarrow))->AddXpos(&vxp);
    } else
        (yyval.arcvertarrow) = NULL;
  #endif
}
#line 7436 "language.cc" /* yacc.c:1646  */
    break;

  case 266:
#line 3940 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    if ((yyvsp[0].vertxpos)) {
		ArcTypePlusDir typeplusdir;
		typeplusdir.arc.type = EArrowSymbol::SOLID;
		typeplusdir.arc.lost = EArrowLost::NOT;
		typeplusdir.dir = EDirType::RIGHT;
		ArcVerticalArrow *ava = new ArcVerticalArrow(typeplusdir, MARKER_HERE_STR, MARKER_HERE_STR, &msc);
		ava->AddXpos((yyvsp[0].vertxpos));
		(yyval.arcvertarrow) = ava;
		delete (yyvsp[0].vertxpos);
	} else
	    (yyval.arcvertarrow) = NULL;
  #endif
}
#line 7457 "language.cc" /* yacc.c:1646  */
    break;

  case 267:
#line 3958 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
  #else
    (yyval.arcbase) = new ArcDivider((yyvsp[0].dividersymbol), &msc);
  #endif
}
#line 7469 "language.cc" /* yacc.c:1646  */
    break;

  case 268:
#line 3966 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = ((yyvsp[-2].arcarrow))->AddLostPos((yyvsp[0].vertxpos), MSC_POS2((yylsp[-1]), (yylsp[0])));
  #endif
   free((yyvsp[-1].str));
}
#line 7482 "language.cc" /* yacc.c:1646  */
    break;

  case 269:
#line 3975 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing 'at' clause.");
    if (csh.CheckHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).end, "Missing 'at' clause.");
    (yyval.arcbase) = ((yyvsp[-1].arcarrow));
  #endif
   free((yyvsp[0].str));
}
#line 7501 "language.cc" /* yacc.c:1646  */
    break;

  case 270:
#line 3990 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "lost at", 
            "Use the 'lost at' keyword to indicate that the message has been lost.", 
            EHintType::KEYWORD, true)); ///XXX Other can come?
        csh.hintStatus = HINT_READY;
    }
  #endif
    //explicit copy here to suppress bison warning (since the two types are different)
    (yyval.arcbase) = (yyvsp[0].arcarrow);

}
#line 7519 "language.cc" /* yacc.c:1646  */
    break;

  case 274:
#line 4007 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[-2]));
    csh.AddCSH_EntityName((yylsp[-2]), (yyvsp[-2].str));
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[-1].arcsegdata), (yyvsp[-2].str), MSC_POS((yylsp[-2])), (yyvsp[0].str), true, MSC_POS((yylsp[0])));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 7536 "language.cc" /* yacc.c:1646  */
    break;

  case 275:
#line 4020 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[-1].arcsegdata), LSIDE_ENT_STR, MSC_POS((yylsp[-1])), (yyvsp[0].str), true, MSC_POS((yylsp[0])));
  #endif
    free((yyvsp[0].str));
}
#line 7550 "language.cc" /* yacc.c:1646  */
    break;

  case 276:
#line 4030 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[-1]));
    csh.AddCSH_EntityName((yylsp[-1]), (yyvsp[-1].str));
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[0].arcsegdata), (yyvsp[-1].str), MSC_POS((yylsp[-1])), RSIDE_ENT_STR, true, MSC_POS((yylsp[0])));
  #endif
    free((yyvsp[-1].str));
}
#line 7565 "language.cc" /* yacc.c:1646  */
    break;

  case 277:
#line 4041 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcarrow) = ((yyvsp[-2].arcarrow))->AddSegment((yyvsp[-1].arcsegdata), (yyvsp[0].str), MSC_POS((yylsp[0])), MSC_POS2((yylsp[-1]), (yylsp[0])));
  #endif
    free((yyvsp[0].str));
}
#line 7579 "language.cc" /* yacc.c:1646  */
    break;

  case 278:
#line 4051 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((yylsp[-1]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[-1].arcarrow))->AddSegment((yyvsp[0].arcsegdata), NULL, MSC_POS((yylsp[0])), MSC_POS((yylsp[0])));
  #endif
}
#line 7591 "language.cc" /* yacc.c:1646  */
    break;

  case 279:
#line 4061 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[-2]));
    csh.AddCSH_EntityName((yylsp[-2]), (yyvsp[-2].str));
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[-1].arcsegdata), (yyvsp[0].str), MSC_POS((yylsp[0])), (yyvsp[-2].str), false, MSC_POS((yylsp[-2])));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 7608 "language.cc" /* yacc.c:1646  */
    break;

  case 280:
#line 4074 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[-1].arcsegdata), (yyvsp[0].str), MSC_POS((yylsp[0])), LSIDE_ENT_STR, false, MSC_POS((yylsp[-1])));
  #endif
    free((yyvsp[0].str));
}
#line 7622 "language.cc" /* yacc.c:1646  */
    break;

  case 281:
#line 4084 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[-1]));
    csh.AddCSH_EntityName((yylsp[-1]), (yyvsp[-1].str));
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[0].arcsegdata), RSIDE_ENT_STR, MSC_POS((yylsp[0])), (yyvsp[-1].str), false, MSC_POS((yylsp[-1])));
  #endif
    free((yyvsp[-1].str));
}
#line 7637 "language.cc" /* yacc.c:1646  */
    break;

  case 282:
#line 4095 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcarrow) = ((yyvsp[-2].arcarrow))->AddSegment((yyvsp[-1].arcsegdata), (yyvsp[0].str), MSC_POS((yylsp[0])), MSC_POS2((yylsp[-1]), (yylsp[0])));
  #endif
    free((yyvsp[0].str));
}
#line 7651 "language.cc" /* yacc.c:1646  */
    break;

  case 283:
#line 4105 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[-1].arcarrow))->AddSegment((yyvsp[0].arcsegdata), NULL, MSC_POS((yylsp[0])), MSC_POS((yylsp[0])));
  #endif
}
#line 7663 "language.cc" /* yacc.c:1646  */
    break;

  case 284:
#line 4114 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[-2]));
    csh.AddCSH_EntityName((yylsp[-2]), (yyvsp[-2].str));
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[-1].arcsegdata), (yyvsp[-2].str), MSC_POS((yylsp[-2])), (yyvsp[0].str), true, MSC_POS((yylsp[0])));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 7680 "language.cc" /* yacc.c:1646  */
    break;

  case 285:
#line 4127 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[-1].arcsegdata), LSIDE_ENT_STR, MSC_POS((yylsp[-1])), (yyvsp[0].str), true, MSC_POS((yylsp[0])));
  #endif
    free((yyvsp[0].str));
}
#line 7694 "language.cc" /* yacc.c:1646  */
    break;

  case 286:
#line 4137 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[-1]));
    csh.AddCSH_EntityName((yylsp[-1]), (yyvsp[-1].str));
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==EOF);
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[0].arcsegdata), (yyvsp[-1].str), MSC_POS((yylsp[-1])), RSIDE_ENT_STR, true, MSC_POS((yylsp[0])));
  #endif
    free((yyvsp[-1].str));
}
#line 7709 "language.cc" /* yacc.c:1646  */
    break;

  case 287:
#line 4148 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcarrow) = ((yyvsp[-2].arcarrow))->AddSegment((yyvsp[-1].arcsegdata), (yyvsp[0].str), MSC_POS((yylsp[0])), MSC_POS2((yylsp[-1]), (yylsp[0])));
  #endif
    free((yyvsp[0].str));
}
#line 7723 "language.cc" /* yacc.c:1646  */
    break;

  case 288:
#line 4158 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[-1].arcarrow))->AddSegment((yyvsp[0].arcsegdata), NULL, MSC_POS((yylsp[0])), MSC_POS((yylsp[0])));
  #endif
}
#line 7735 "language.cc" /* yacc.c:1646  */
    break;

  case 302:
#line 4170 "language.yy" /* yacc.c:1646  */
    {(yyval.arrowsymbol)=EArrowSymbol::UNDETERMINED_SEGMENT;}
#line 7741 "language.cc" /* yacc.c:1646  */
    break;

  case 304:
#line 4171 "language.yy" /* yacc.c:1646  */
    {(yyval.arrowsymbol)=EArrowSymbol::UNDETERMINED_SEGMENT;}
#line 7747 "language.cc" /* yacc.c:1646  */
    break;

  case 306:
#line 4172 "language.yy" /* yacc.c:1646  */
    {(yyval.arrowsymbol)=EArrowSymbol::UNDETERMINED_SEGMENT;}
#line 7753 "language.cc" /* yacc.c:1646  */
    break;

  case 307:
#line 4175 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 0;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arrowsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
#line 7767 "language.cc" /* yacc.c:1646  */
    break;

  case 308:
#line 4185 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arrowsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-1])));
  #endif
}
#line 7782 "language.cc" /* yacc.c:1646  */
    break;

  case 309:
#line 4196 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[-1].arrowsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[0])));
  #endif
}
#line 7797 "language.cc" /* yacc.c:1646  */
    break;

  case 310:
#line 4207 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]) + (yylsp[-1]), COLOR_SYMBOL);
    csh.AddCSH_Error((yylsp[0]), MULTIPLE_ASTERISK_ERROR_MSG);
    csh.asteriskNo = 2;
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).start, "One arrow may be lost only once. Ignoring subsequent asterisks ('*').");
    ((yyval.arcsegdata)).type = (yyvsp[-1].arrowsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-2])));
  #endif
}
#line 7814 "language.cc" /* yacc.c:1646  */
    break;

  case 311:
#line 4221 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arrowsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
#line 7828 "language.cc" /* yacc.c:1646  */
    break;

  case 312:
#line 4231 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arrowsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-1])));
  #endif
}
#line 7843 "language.cc" /* yacc.c:1646  */
    break;

  case 313:
#line 4242 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[-1].arrowsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[0])));
  #endif
}
#line 7858 "language.cc" /* yacc.c:1646  */
    break;

  case 314:
#line 4253 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]) + (yylsp[-1]), COLOR_SYMBOL);
    csh.AddCSH_Error((yylsp[0]), MULTIPLE_ASTERISK_ERROR_MSG);
    csh.asteriskNo = 2;
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).start, "One arrow may be lost only once. Ignoring subsequent asterisks ('*').");
    ((yyval.arcsegdata)).type = (yyvsp[-1].arrowsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-2])));
  #endif
}
#line 7875 "language.cc" /* yacc.c:1646  */
    break;

  case 315:
#line 4267 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 0;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arrowsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
#line 7889 "language.cc" /* yacc.c:1646  */
    break;

  case 316:
#line 4277 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arrowsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-1])));
  #endif
}
#line 7904 "language.cc" /* yacc.c:1646  */
    break;

  case 317:
#line 4288 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[-1].arrowsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[0])));
  #endif
}
#line 7919 "language.cc" /* yacc.c:1646  */
    break;

  case 318:
#line 4299 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]) + (yylsp[-1]), COLOR_SYMBOL);
    csh.AddCSH_Error((yylsp[0]), MULTIPLE_ASTERISK_ERROR_MSG);
    csh.asteriskNo = 2;
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).start, "One arrow may be lost only once. Ignoring subsequent asterisks ('*').");
    ((yyval.arcsegdata)).type = (yyvsp[-1].arrowsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-2])));
  #endif
}
#line 7936 "language.cc" /* yacc.c:1646  */
    break;

  case 319:
#line 4313 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arrowsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
#line 7949 "language.cc" /* yacc.c:1646  */
    break;

  case 320:
#line 4322 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[-1]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arrowsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-1])));
  #endif
}
#line 7967 "language.cc" /* yacc.c:1646  */
    break;

  case 321:
#line 4336 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[0]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[-1].arrowsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[0])));
  #endif
}
#line 7985 "language.cc" /* yacc.c:1646  */
    break;

  case 322:
#line 4350 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[-2]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[-2]), MULTIPLE_ASTERISK_ERROR_MSG);
    csh.AddCSH_Error((yylsp[0]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).start, "One arrow may be lost only once. Ignoring this asterisk ('*').");
    ((yyval.arcsegdata)).type = (yyvsp[-1].arrowsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-2])));
  #endif
}
#line 8005 "language.cc" /* yacc.c:1646  */
    break;

  case 323:
#line 4367 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arrowsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
#line 8018 "language.cc" /* yacc.c:1646  */
    break;

  case 324:
#line 4376 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[-1]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arrowsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-1])));
  #endif
}
#line 8036 "language.cc" /* yacc.c:1646  */
    break;

  case 325:
#line 4390 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[0]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[-1].arrowsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[0])));
  #endif
}
#line 8054 "language.cc" /* yacc.c:1646  */
    break;

  case 326:
#line 4404 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[-2]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[-2]), MULTIPLE_ASTERISK_ERROR_MSG);
    csh.AddCSH_Error((yylsp[0]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).start, "One arrow may be lost only once. Ignoring this asterisk ('*').");
    ((yyval.arcsegdata)).type = (yyvsp[-1].arrowsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-2])));
  #endif
}
#line 8074 "language.cc" /* yacc.c:1646  */
    break;

  case 327:
#line 4421 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arrowsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
#line 8087 "language.cc" /* yacc.c:1646  */
    break;

  case 328:
#line 4430 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[-1]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arrowsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-1])));
  #endif
}
#line 8105 "language.cc" /* yacc.c:1646  */
    break;

  case 329:
#line 4444 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[0]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[-1].arrowsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[0])));
  #endif
}
#line 8123 "language.cc" /* yacc.c:1646  */
    break;

  case 330:
#line 4458 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[-2]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[-2]), MULTIPLE_ASTERISK_ERROR_MSG);
    csh.AddCSH_Error((yylsp[0]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).start, "One arrow may be lost only once. Ignoring this asterisk ('*').");
    ((yyval.arcsegdata)).type = (yyvsp[-1].arrowsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-2])));
  #endif
}
#line 8143 "language.cc" /* yacc.c:1646  */
    break;

  case 332:
#line 4478 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ExtvxposDesignatorName((yylsp[0]), (yyvsp[0].str));
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_FILLING;
    }
  #else
    (yyval.extvertxpos) = NULL;
  #endif
    free((yyvsp[0].str));
}
#line 8160 "language.cc" /* yacc.c:1646  */
    break;

  case 333:
#line 4492 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ExtvxposDesignatorName((yylsp[-1]), (yyvsp[-1].str));
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_FILLING;
    } else if (csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.extvertxpos) = new ExtVertXPos((yyvsp[-1].str), MSC_POS((yylsp[-1])), (yyvsp[0].vertxpos));
  #endif
    free((yyvsp[-1].str));
}
#line 8180 "language.cc" /* yacc.c:1646  */
    break;

  case 334:
#line 4508 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ExtvxposDesignatorName((yylsp[0]), (yyvsp[0].str));
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_FILLING;
    } else if (csh.CheckHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.extvertxpos) = NULL;
  #endif
    free((yyvsp[0].str));
}
#line 8200 "language.cc" /* yacc.c:1646  */
    break;

  case 338:
#line 4527 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-3]), COLOR_KEYWORD);
    csh.AddCSH_SymbolName((yylsp[-2]), (yyvsp[-2].str));
    if (csh.CheckHintAt((yylsp[-3]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-3]), (yylsp[-2]), EHintSourceType::KEYWORD)) {
        csh.AddSymbolTypesToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-2]), (yylsp[-1]), EHintSourceType::MARKER)) {
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_FILLING;
    }
  #else
    (yyval.arcbase) = new CommandSymbol(&msc, (yyvsp[-2].str), (yyvsp[-1].namerel), (yyvsp[0].extvertxpos), NULL);
  #endif
    free((yyvsp[-3].str));
    free((yyvsp[-2].str));
}
#line 8230 "language.cc" /* yacc.c:1646  */
    break;

  case 339:
#line 4553 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-4]), COLOR_KEYWORD);
    csh.AddCSH_SymbolName((yylsp[-3]), (yyvsp[-3].str));
    if (csh.CheckHintAt((yylsp[-4]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-4]), (yylsp[-3]), EHintSourceType::KEYWORD)) {
        csh.AddSymbolTypesToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-3]), (yylsp[-2]), EHintSourceType::MARKER)) {
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-2]), (yylsp[-1]), EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_FILLING;
    }
#else
    (yyval.arcbase) = new CommandSymbol(&msc, (yyvsp[-3].str), (yyvsp[-2].namerel), (yyvsp[-1].extvertxpos), (yyvsp[0].extvertxpos));
  #endif
    free((yyvsp[-4].str));
    free((yyvsp[-3].str));
}
#line 8263 "language.cc" /* yacc.c:1646  */
    break;

  case 340:
#line 4583 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-5]), COLOR_KEYWORD);
    csh.AddCSH_SymbolName((yylsp[-4]), (yyvsp[-4].str));
    if (csh.CheckHintAt((yylsp[-5]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-5]), (yylsp[-4]), EHintSourceType::KEYWORD)) {
        csh.AddSymbolTypesToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-4]), (yylsp[-3]), EHintSourceType::MARKER)) {
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-3]), (yylsp[-2]), EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-2]), (yylsp[-1]), EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-2]), (yylsp[0]), EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_FILLING;
    }
    csh.AddCSH_Error((yylsp[0]), "Too many left/center/right specifiers here, at most two can be given.");
#else
    (yyval.arcbase) = new CommandSymbol(&msc, (yyvsp[-4].str), (yyvsp[-3].namerel), (yyvsp[-2].extvertxpos), (yyvsp[-1].extvertxpos));
    if ((yyvsp[0].extvertxpos))
        delete (yyvsp[0].extvertxpos);
    msc.Error.Error(MSC_POS((yylsp[0])).start, "Too many specifiers here, ignoring last one.", "At most two can be given.");
  #endif
    free((yyvsp[-5].str));
    free((yyvsp[-4].str));
}
#line 8303 "language.cc" /* yacc.c:1646  */
    break;

  case 341:
#line 4619 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.AddCSH_SymbolName((yylsp[-1]), (yyvsp[-1].str));
    if (csh.CheckHintAt((yylsp[-2]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-2]), (yylsp[-1]), EHintSourceType::KEYWORD)) {
        csh.AddSymbolTypesToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::MARKER)) {
        csh.AddLeftRightCenterToHints(); //markers, plus left/center/right
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_FILLING;
    }
  #else
    (yyval.arcbase) = new CommandSymbol(&msc, (yyvsp[-1].str), NULL, (yyvsp[0].extvertxpos), NULL);
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[-1].str));
}
#line 8331 "language.cc" /* yacc.c:1646  */
    break;

  case 342:
#line 4643 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-3]), COLOR_KEYWORD);
    csh.AddCSH_SymbolName((yylsp[-2]), (yyvsp[-2].str));
    if (csh.CheckHintAt((yylsp[-3]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-3]), (yylsp[-2]), EHintSourceType::KEYWORD)) {
        csh.AddSymbolTypesToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintBetween((yylsp[-2]), (yylsp[-1]), EHintSourceType::MARKER)) {
        csh.AddLeftRightCenterToHints(); //markers, plus left/center/right
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_FILLING;
    }
#else
    (yyval.arcbase) = new CommandSymbol(&msc, (yyvsp[-2].str), NULL, (yyvsp[-1].extvertxpos), (yyvsp[0].extvertxpos));
  #endif
    free((yyvsp[-3].str));
    free((yyvsp[-2].str));
}
#line 8362 "language.cc" /* yacc.c:1646  */
    break;

  case 343:
#line 4670 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.AddCSH_SymbolName((yylsp[-1]), (yyvsp[-1].str));
    if (csh.CheckHintAt((yylsp[-2]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-2]), (yylsp[-1]), EHintSourceType::KEYWORD)) {
        csh.AddSymbolTypesToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), EHintSourceType::MARKER)) {
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_FILLING;
    }
#else
    (yyval.arcbase) = new CommandSymbol(&msc, (yyvsp[-1].str), (yyvsp[0].namerel), NULL, NULL);
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[-1].str));
}
#line 8389 "language.cc" /* yacc.c:1646  */
    break;

  case 344:
#line 4693 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.AddCSH_SymbolName((yylsp[-1]), (yyvsp[-1].str));
    csh.AddCSH_LeftRightCenterMarker((yylsp[0]), (yyvsp[0].str));
    if (csh.CheckHintAt((yylsp[-2]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-2]), (yylsp[-1]), EHintSourceType::KEYWORD)) {
        csh.AddSymbolTypesToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), EHintSourceType::MARKER)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.arcbase) = new CommandSymbol(&msc, (yyvsp[-1].str), NULL, NULL, NULL);
    msc.Error.Error(MSC_POS((yylsp[0])).start, "Expecting 'left', 'right', 'center' or a marker name followed by a dash. Ignoring this piece.");
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[-1].str));
    free((yyvsp[0].str));
}
#line 8417 "language.cc" /* yacc.c:1646  */
    break;

  case 345:
#line 4717 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    csh.AddCSH_SymbolName((yylsp[0]), (yyvsp[0].str));
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), EHintSourceType::KEYWORD)) {
        csh.AddSymbolTypesToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::MARKER)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.arcbase) = new CommandSymbol(&msc, (yyvsp[0].str), NULL, NULL, NULL);
  #endif
    free((yyvsp[-1].str));
    free((yyvsp[0].str));
}
#line 8442 "language.cc" /* yacc.c:1646  */
    break;

  case 346:
#line 4738 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddSymbolTypesToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).end, "Missing a symbol type.", "Use 'arc', '...', 'text' or 'rectangle'.");
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[0].str));
}
#line 8463 "language.cc" /* yacc.c:1646  */
    break;

  case 347:
#line 4756 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[0].arcbase))
        ((yyvsp[0].arcbase))->AddAttributeList(NULL);
    (yyval.arcbase) = ((yyvsp[0].arcbase));
  #endif
}
#line 8475 "language.cc" /* yacc.c:1646  */
    break;

  case 348:
#line 4764 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0])))
        CommandSymbol::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0])))
        CommandSymbol::AttributeValues(csh.hintAttrName, csh);
  #else
    if ((yyvsp[-1].arcbase))
        ((yyvsp[-1].arcbase))->AddAttributeList((yyvsp[0].attriblist));
    (yyval.arcbase) = ((yyvsp[-1].arcbase));
  #endif
}
#line 8492 "language.cc" /* yacc.c:1646  */
    break;

  case 349:
#line 4778 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-3]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.AddCSH_EntityOrMarkerName((yylsp[-1]), (yyvsp[-1].str));
    if (csh.CheckHintAt((yylsp[-3]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0])))
        CommandNote::AttributeNames(csh, true);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0])))
        CommandNote::AttributeValues(csh.hintAttrName, csh, true);
    else if (csh.CheckEntityHintAtAndBefore((yylsp[-2]), (yylsp[-1])))
        csh.addMarkersAtEnd = true; //after 'at' a marker or an entity may come
  #else
    (yyval.arcbase) = new CommandNote(&msc, (yyvsp[-1].str), MSC_POS((yylsp[-1])));
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
  #endif
    free((yyvsp[-3].str));
    free((yyvsp[-2].str));
    free((yyvsp[-1].str));
}
#line 8519 "language.cc" /* yacc.c:1646  */
    break;

  case 350:
#line 4801 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "at", NULL, EHintType::KEYWORD, true)); //XXX Other can come
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0])))
        CommandNote::AttributeNames(csh, true);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0])))
        CommandNote::AttributeValues(csh.hintAttrName, csh, true);
  #else
    (yyval.arcbase) = new CommandNote(&msc);
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
  #endif
    free((yyvsp[-1].str));
}
#line 8542 "language.cc" /* yacc.c:1646  */
    break;

  case 351:
#line 4820 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing an entity or marker name.");
    csh.AddCSH_ErrorAfter((yylsp[0]), "Notes need a label.");
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckEntityHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF)) 
        csh.addMarkersAtEnd = true; //after 'at' a marker or an entity may come
  #else
    (yyval.arcbase) = NULL;
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing an entity or marker name after 'at'.");
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing a label, ignoring note.", "Notes and comments must have text, specify a label.");
#endif
    free((yyvsp[-1].str));
    free((yyvsp[0].str));
}
#line 8566 "language.cc" /* yacc.c:1646  */
    break;

  case 352:
#line 4840 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    csh.AddCSH_ErrorAfter((yylsp[-1]), "Missing an entity or marker name.");
    if (csh.CheckHintAt((yylsp[-2]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckEntityHintAfterPlusOne((yylsp[-1]), yylloc, yychar==YYEOF))
        csh.addMarkersAtEnd = true;
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0])))
        CommandNote::AttributeNames(csh, true);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0])))
        CommandNote::AttributeValues(csh.hintAttrName, csh, true);
  #else
    msc.Error.Error(MSC_POS((yylsp[-1])).end.NextChar(), "Missing an entity or marker name after 'at'. Ignoring 'at' clause.");
    (yyval.arcbase) = new CommandNote(&msc);
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[-1].str));
}
#line 8593 "language.cc" /* yacc.c:1646  */
    break;

  case 353:
#line 4864 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.eside)= ESide::LEFT;
  #endif
    free((yyvsp[0].str));
}
#line 8604 "language.cc" /* yacc.c:1646  */
    break;

  case 354:
#line 4871 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.eside)= ESide::END;
  #endif
    free((yyvsp[0].str));
}
#line 8615 "language.cc" /* yacc.c:1646  */
    break;

  case 355:
#line 4878 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.eside)= ESide::END;
  #endif
    free((yyvsp[0].str));
}
#line 8626 "language.cc" /* yacc.c:1646  */
    break;

  case 356:
#line 4886 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-1]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0])))
        CommandNote::AttributeNames(csh, false);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0])))
        CommandNote::AttributeValues(csh.hintAttrName, csh, false);
  #else
    (yyval.arcbase) = new CommandNote(&msc, (yyvsp[-1].eside));
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
  #endif
}
#line 8646 "language.cc" /* yacc.c:1646  */
    break;

  case 357:
#line 4902 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    csh.AddCSH_ErrorAfter((yylsp[0]), "Comments and notes need a label.");
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } 
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing a label, ignoring comment/note.", "Notes and comments must have text, specify a label.");
    (yyval.arcbase) = NULL;
  #endif
}
#line 8664 "language.cc" /* yacc.c:1646  */
    break;

  case 358:
#line 4917 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ColonString_CheckAndAddEscapeHint((yylsp[0]), (yyvsp[0].str), false);
    csh.AddColonLabel((yylsp[0]));
#endif
    (yyval.str) = (yyvsp[0].str);
}
#line 8676 "language.cc" /* yacc.c:1646  */
    break;

  case 359:
#line 4925 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ColonString_CheckAndAddEscapeHint((yylsp[0]), (yyvsp[0].str), true);
	csh.AddColonLabel((yylsp[0]));
  #endif
    (yyval.str) = (yyvsp[0].str);
}
#line 8688 "language.cc" /* yacc.c:1646  */
    break;

  case 360:
#line 4934 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = (new AttributeList)->Append(new Attribute("label", (yyvsp[0].str), MSC_POS((yyloc)), MSC_POS((yyloc)).IncStartCol()));
  #endif
    free((yyvsp[0].str));
}
#line 8700 "language.cc" /* yacc.c:1646  */
    break;

  case 361:
#line 4942 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = ((yyvsp[0].attriblist))->Prepend(new Attribute("label", (yyvsp[-1].str), MSC_POS((yylsp[-1])), MSC_POS((yylsp[-1])).IncStartCol()));
  #endif
    free((yyvsp[-1].str));
}
#line 8712 "language.cc" /* yacc.c:1646  */
    break;

  case 362:
#line 4950 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = ((yyvsp[-2].attriblist))->Append(new Attribute("label", (yyvsp[-1].str), MSC_POS((yylsp[-1])), MSC_POS((yylsp[-1])).IncStartCol()));
        //Merge $3 at the end of $1
        ((yyvsp[-2].attriblist))->splice(((yyvsp[-2].attriblist))->end(), *((yyvsp[0].attriblist)));
        delete ((yyvsp[0].attriblist)); //empty list now
        (yyval.attriblist) = (yyvsp[-2].attriblist);
  #endif
    free((yyvsp[-1].str));
}
#line 8728 "language.cc" /* yacc.c:1646  */
    break;

  case 363:
#line 4962 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = ((yyvsp[-1].attriblist))->Append(new Attribute("label", (yyvsp[0].str), MSC_POS((yylsp[0])), MSC_POS((yylsp[0])).IncStartCol()));
  #endif
    free((yyvsp[0].str));
}
#line 8740 "language.cc" /* yacc.c:1646  */
    break;

  case 365:
#line 4973 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[-1]), COLOR_BRACKET);
        csh.AddCSH((yylsp[0]), COLOR_BRACKET);
        csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_NAME);
  #else
    (yyval.attriblist) = new AttributeList;
  #endif
}
#line 8754 "language.cc" /* yacc.c:1646  */
    break;

  case 366:
#line 4983 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[-2]), COLOR_BRACKET);
        csh.AddCSH((yylsp[0]), COLOR_BRACKET);
        csh.CheckHintBetween((yylsp[-2]), (yylsp[-1]), EHintSourceType::ATTR_NAME);
  #else
    (yyval.attriblist) = (yyvsp[-1].attriblist);
  #endif
}
#line 8768 "language.cc" /* yacc.c:1646  */
    break;

  case 367:
#line 4993 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[-3]), COLOR_BRACKET);
        csh.AddCSH_Error((yylsp[-1]), "Extra stuff after an attribute list. Maybe missing a comma?");
        csh.AddCSH((yylsp[0]), COLOR_BRACKET);
        csh.CheckHintBetween((yylsp[-3]), (yylsp[-2]), EHintSourceType::ATTR_NAME);
  #else
    (yyval.attriblist) = (yyvsp[-2].attriblist);
  #endif
}
#line 8783 "language.cc" /* yacc.c:1646  */
    break;

  case 368:
#line 5004 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[-2]), COLOR_BRACKET);
        csh.AddCSH_Error((yylsp[-1]), "Could not recognize this as an attribute or style name.");
        csh.AddCSH((yylsp[0]), COLOR_BRACKET);
        csh.CheckHintBetween((yylsp[-2]), (yylsp[-1]), EHintSourceType::ATTR_NAME);
  #else
    (yyval.attriblist) = new AttributeList;
    msc.Error.Error(MSC_POS((yylsp[-1])).start, "Expecting an attribute or style name. Ignoring all until the closing square bracket (']').");
#endif
}
#line 8799 "language.cc" /* yacc.c:1646  */
    break;

  case 369:
#line 5016 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[-1]), COLOR_BRACKET);
        csh.AddCSH_ErrorAfter((yylsp[0]), "Missing a square bracket (']').");
        csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_NAME);
  #else
    (yyval.attriblist) = (yyvsp[0].attriblist);
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing ']'.");
  #endif
}
#line 8814 "language.cc" /* yacc.c:1646  */
    break;

  case 370:
#line 5027 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[-2]), COLOR_BRACKET);
        csh.AddCSH_Error((yylsp[0]), "Missing a ']'.");
        csh.CheckHintBetween((yylsp[-2]), (yylsp[-1]), EHintSourceType::ATTR_NAME);
  #else
    (yyval.attriblist) = (yyvsp[-1].attriblist);
    msc.Error.Error(MSC_POS((yylsp[0])).start, "Missing ']'.");
  #endif
}
#line 8829 "language.cc" /* yacc.c:1646  */
    break;

  case 371:
#line 5038 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[0]), COLOR_BRACKET);
        csh.AddCSH_ErrorAfter((yylsp[0]), "Missing a square bracket (']').");
        csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::ATTR_NAME);
  #else
    (yyval.attriblist) = new AttributeList;
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing ']'.");
  #endif
}
#line 8844 "language.cc" /* yacc.c:1646  */
    break;

  case 372:
#line 5051 "language.yy" /* yacc.c:1646  */
    {
    (yyval.attriblist) = NULL;
}
#line 8852 "language.cc" /* yacc.c:1646  */
    break;

  case 373:
#line 5055 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[-1]), COLOR_BRACKET);
        csh.AddCSH_Error((yylsp[0]), "Missing a ']'.");
        csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_NAME);
  #else
    (yyval.attriblist) = new AttributeList;
    msc.Error.Error(MSC_POS((yylsp[0])).start, "Missing ']'.");
  #endif
}
#line 8867 "language.cc" /* yacc.c:1646  */
    break;

  case 374:
#line 5067 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.attriblist) = (new AttributeList)->Append((yyvsp[0].attrib));
  #endif
}
#line 8878 "language.cc" /* yacc.c:1646  */
    break;

  case 375:
#line 5074 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_COMMA);
    csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_NAME);
  #else
    (yyval.attriblist) = ((yyvsp[-2].attriblist))->Append((yyvsp[0].attrib));
  #endif
}
#line 8891 "language.cc" /* yacc.c:1646  */
    break;

  case 376:
#line 5083 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_COMMA);
    csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::ATTR_NAME);
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing attribute or style name.");
  #else
    (yyval.attriblist) = (yyvsp[-1].attriblist);
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Expecting an attribute or style name here.");
  #endif
}
#line 8906 "language.cc" /* yacc.c:1646  */
    break;

  case 378:
#line 5096 "language.yy" /* yacc.c:1646  */
    {
  //string=string
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((yylsp[-2]), (yyvsp[-2].str), COLOR_ATTRNAME);
        csh.AddCSH((yylsp[-1]), COLOR_EQUAL);
        csh.AddCSH_AttrValue_CheckAndAddEscapeHint((yylsp[0]), (yyvsp[0].str), (yyvsp[-2].str));
        csh.CheckHintAt((yylsp[-2]), EHintSourceType::ATTR_NAME);
        csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_VALUE, (yyvsp[-2].str));
  #else
        (yyval.attrib) = new Attribute((yyvsp[-2].str), (yyvsp[0].str), MSC_POS((yylsp[-2])), MSC_POS((yylsp[0])));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 8925 "language.cc" /* yacc.c:1646  */
    break;

  case 379:
#line 5111 "language.yy" /* yacc.c:1646  */
    {
  //string=string
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((yylsp[-3]), (yyvsp[-3].str), COLOR_ATTRNAME);
        csh.AddCSH((yylsp[-2]), COLOR_EQUAL);
        csh.AddCSH_AttrValue_CheckAndAddEscapeHint((yylsp[-1])+(yylsp[0]), (string("++")+(yyvsp[0].str)).c_str(), (yyvsp[-3].str));
        csh.CheckHintAt((yylsp[-3]), EHintSourceType::ATTR_NAME);
        csh.CheckHintAtAndBefore((yylsp[-2]), (yylsp[-1])+(yylsp[0]), EHintSourceType::ATTR_VALUE, (yyvsp[-3].str));
  #else
        (yyval.attrib) = new Attribute((yyvsp[-3].str), string("++")+(yyvsp[0].str), MSC_POS((yylsp[-3])), MSC_POS2((yylsp[-1]),(yylsp[0])));
  #endif
    free((yyvsp[-3].str));
    free((yyvsp[0].str));
}
#line 8944 "language.cc" /* yacc.c:1646  */
    break;

  case 380:
#line 5126 "language.yy" /* yacc.c:1646  */
    {
  //string=string
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((yylsp[-2]), (yyvsp[-2].str), COLOR_ATTRNAME);
        csh.AddCSH((yylsp[-1]), COLOR_EQUAL);
        csh.AddCSH_AttrValue_CheckAndAddEscapeHint((yylsp[0]), "++", (yyvsp[-2].str));
		csh.AddCSH_ErrorAfter((yylsp[0]), "Continue with a color name or definition.");
        csh.CheckHintAt((yylsp[-2]), EHintSourceType::ATTR_NAME);
        csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_VALUE, (yyvsp[-2].str));
  #else
        (yyval.attrib) = new Attribute((yyvsp[-2].str), "++", MSC_POS((yylsp[-2])), MSC_POS((yylsp[0])));
  #endif
    free((yyvsp[-2].str));
}
#line 8963 "language.cc" /* yacc.c:1646  */
    break;

  case 381:
#line 5141 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((yylsp[-2]), (yyvsp[-2].str), COLOR_ATTRNAME);
        csh.AddCSH((yylsp[-1]), COLOR_EQUAL);
        csh.AddCSH((yylsp[0]), COLOR_ATTRVALUE);
        csh.CheckHintAt((yylsp[-2]), EHintSourceType::ATTR_NAME);
        csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_VALUE, (yyvsp[-2].str));
  #else
        (yyval.attrib) = new Attribute((yyvsp[-2].str), atof((yyvsp[0].str)), MSC_POS((yyloc)), MSC_POS((yylsp[0])), (yyvsp[0].str));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 8981 "language.cc" /* yacc.c:1646  */
    break;

  case 382:
#line 5155 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((yylsp[-1]), (yyvsp[-1].str), COLOR_ATTRNAME);
        csh.AddCSH((yylsp[0]), COLOR_EQUAL);
        csh.CheckHintAt((yylsp[-1]), EHintSourceType::ATTR_NAME);
        csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::ATTR_VALUE, (yyvsp[-1].str));
  #else
        (yyval.attrib) = new Attribute((yyvsp[-1].str), (char*)NULL, MSC_POS((yyloc)), MSC_POS((yyloc)));
  #endif
    free((yyvsp[-1].str));
}
#line 8997 "language.cc" /* yacc.c:1646  */
    break;

  case 383:
#line 5169 "language.yy" /* yacc.c:1646  */
    {
    (yyval.attrib) = NULL;
    free((yyvsp[-2].str));
}
#line 9006 "language.cc" /* yacc.c:1646  */
    break;

  case 384:
#line 5174 "language.yy" /* yacc.c:1646  */
    {
  //here we accept non alpha strings for "->" and similar style names
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_StyleOrAttrName((yylsp[0]), (yyvsp[0].str));
        csh.CheckHintAt((yylsp[0]), EHintSourceType::ATTR_NAME);
  #else
        (yyval.attrib) = new Attribute((yyvsp[0].str), MSC_POS((yyloc)));
  #endif
    free((yyvsp[0].str));
}
#line 9021 "language.cc" /* yacc.c:1646  */
    break;

  case 385:
#line 5186 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    if (CaseInsensitiveEqual((yyvsp[0].str), "brace")) (yyval.vshape) = ArcVerticalArrow::BRACE;
    else if (CaseInsensitiveEqual((yyvsp[0].str), "bracket")) (yyval.vshape) = ArcVerticalArrow::BRACKET;
    else if (CaseInsensitiveEqual((yyvsp[0].str), "range")) (yyval.vshape) = ArcVerticalArrow::RANGE;
    else if (CaseInsensitiveEqual((yyvsp[0].str), "pointer")) (yyval.vshape) = ArcVerticalArrow::POINTER; 
    else {
        (yyval.vshape) = ArcVerticalArrow::ARROW_OR_BOX;
        _ASSERT(0);
    }
  #endif
  free((yyvsp[0].str));
}
#line 9039 "language.cc" /* yacc.c:1646  */
    break;

  case 386:
#line 5200 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
	(yyval.vshape) = ArcVerticalArrow::ARROW_OR_BOX;
  #endif
  free((yyvsp[0].str));
}
#line 9050 "language.cc" /* yacc.c:1646  */
    break;

  case 387:
#line 5210 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
	csh.AddQuotedString((yylsp[0]));
  #endif
  (yyval.str) = (yyvsp[0].str);
}
#line 9061 "language.cc" /* yacc.c:1646  */
    break;

  case 389:
#line 5218 "language.yy" /* yacc.c:1646  */
    {
	(yyval.str) = (char*)malloc(2);
	((yyval.str))[0] = ShapeElement::act_code[(yyvsp[0].shapecommand)];
	((yyval.str))[1] = 0;
}
#line 9071 "language.cc" /* yacc.c:1646  */
    break;

  case 423:
#line 5234 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("->");}
#line 9077 "language.cc" /* yacc.c:1646  */
    break;

  case 424:
#line 5235 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("<-");}
#line 9083 "language.cc" /* yacc.c:1646  */
    break;

  case 425:
#line 5236 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("<->");}
#line 9089 "language.cc" /* yacc.c:1646  */
    break;

  case 426:
#line 5237 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("=>");}
#line 9095 "language.cc" /* yacc.c:1646  */
    break;

  case 427:
#line 5238 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("<=");}
#line 9101 "language.cc" /* yacc.c:1646  */
    break;

  case 428:
#line 5239 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("<=>");}
#line 9107 "language.cc" /* yacc.c:1646  */
    break;

  case 429:
#line 5240 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup(">>");}
#line 9113 "language.cc" /* yacc.c:1646  */
    break;

  case 430:
#line 5241 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("<<");}
#line 9119 "language.cc" /* yacc.c:1646  */
    break;

  case 431:
#line 5242 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("<<>>");}
#line 9125 "language.cc" /* yacc.c:1646  */
    break;

  case 432:
#line 5243 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup(">");}
#line 9131 "language.cc" /* yacc.c:1646  */
    break;

  case 433:
#line 5244 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("<");}
#line 9137 "language.cc" /* yacc.c:1646  */
    break;

  case 434:
#line 5245 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("<>");}
#line 9143 "language.cc" /* yacc.c:1646  */
    break;

  case 435:
#line 5247 "language.yy" /* yacc.c:1646  */
    {
    switch ((yyvsp[0].dividersymbol)) {
    case EDividerSymbol::DIVIDER:  (yyval.str) = strdup("---"); break;
    case EDividerSymbol::DISCO:    (yyval.str) = strdup("..."); break;
    default: _ASSERT(0);
    }
}
#line 9155 "language.cc" /* yacc.c:1646  */
    break;

  case 436:
#line 5255 "language.yy" /* yacc.c:1646  */
    {
    switch ((yyvsp[0].boxsymbol)) {
    case EBoxSymbol::SOLID:  (yyval.str) = strdup("--"); break;
    case EBoxSymbol::DASHED: (yyval.str) = strdup("++"); break; //will likely not happen due to special handling in TOK_COLORDEF
    case EBoxSymbol::DOTTED: (yyval.str) = strdup(".."); break;
    case EBoxSymbol::DOUBLE: (yyval.str) = strdup("=="); break;
    default: _ASSERT(0);
    }
}
#line 9169 "language.cc" /* yacc.c:1646  */
    break;

  case 442:
#line 5270 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_BRACE);
    csh.PushContext();
    if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.PushContext();
  #endif
}
#line 9186 "language.cc" /* yacc.c:1646  */
    break;

  case 444:
#line 5287 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    (yyval.arcbase) = NULL;
    csh.PopContext();
    csh.AddCSH((yylsp[0]), COLOR_BRACE);
  #else
    std::pair<bool, double> hscale = msc.Contexts.back().hscale;
    (yyval.arcbase) = msc.PopContext();
    if (hscale.first)
        msc.Contexts.back().hscale = hscale;
  #endif
}
#line 9203 "language.cc" /* yacc.c:1646  */
    break;


#line 9207 "language.cc" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, RESULT, yyscanner, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (&yylloc, RESULT, yyscanner, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, RESULT, yyscanner);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp, RESULT, yyscanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, RESULT, yyscanner, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, RESULT, yyscanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp, RESULT, yyscanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 5301 "language.yy" /* yacc.c:1906  */



/* END OF FILE */
