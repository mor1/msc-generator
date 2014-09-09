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


/* Substitute the variable and function names.  */
#define yyparse         csh_parse
#define yylex           csh_lex
#define yyerror         csh_error
#define yydebug         csh_debug
#define yynerrs         csh_nerrs


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

#line 130 "colorsyntax.cc" /* yacc.c:339  */

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
   by #include "colorsyntax.h".  */
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
    CHAR_IF_CSH(EArcType)          arctype;
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

#line 342 "colorsyntax.cc" /* yacc.c:355  */
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

/* Copy the second part of user declarations.  */
#line 178 "language.yy" /* yacc.c:358  */

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


#line 431 "colorsyntax.cc" /* yacc.c:358  */

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
#define YYFINAL  223
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1926

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  72
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  90
/* YYNRULES -- Number of rules.  */
#define YYNRULES  437
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  479

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
       0,   242,   242,   247,   248,   257,   269,   279,   292,   310,
     325,   337,   355,   356,   365,   379,   392,   411,   419,   430,
     441,   452,   463,   473,   485,   499,   500,   511,   526,   535,
     543,   557,   558,   559,   571,   587,   587,   589,   598,   622,
     640,   648,   660,   677,   697,   720,   739,   760,   783,   814,
     838,   863,   897,   909,   917,   933,   949,   962,   975,   995,
    1014,  1028,  1035,  1042,  1051,  1064,  1078,  1094,  1112,  1128,
    1146,  1159,  1174,  1188,  1205,  1219,  1236,  1256,  1271,  1289,
    1309,  1326,  1343,  1358,  1375,  1395,  1412,  1426,  1443,  1444,
    1445,  1446,  1463,  1485,  1504,  1526,  1543,  1566,  1566,  1568,
    1569,  1595,  1617,  1634,  1635,  1646,  1657,  1657,  1659,  1671,
    1682,  1691,  1705,  1717,  1729,  1740,  1750,  1765,  1765,  1765,
    1765,  1767,  1777,  1796,  1809,  1810,  1825,  1844,  1863,  1882,
    1904,  1923,  1945,  1965,  1971,  1982,  1994,  1997,  2013,  2025,
    2041,  2054,  2072,  2087,  2105,  2121,  2122,  2132,  2145,  2193,
    2204,  2224,  2239,  2254,  2275,  2292,  2302,  2313,  2330,  2345,
    2362,  2374,  2387,  2398,  2406,  2422,  2450,  2474,  2500,  2528,
    2558,  2591,  2592,  2602,  2616,  2616,  2618,  2640,  2663,  2681,
    2698,  2722,  2746,  2757,  2758,  2769,  2783,  2800,  2814,  2831,
    2833,  2834,  2844,  2854,  2861,  2880,  2899,  2916,  2938,  2960,
    2977,  2996,  3005,  3022,  3031,  3049,  3065,  3073,  3089,  3099,
    3118,  3133,  3161,  3169,  3182,  3190,  3205,  3223,  3242,  3257,
    3279,  3290,  3306,  3328,  3329,  3338,  3338,  3340,  3354,  3365,
    3377,  3387,  3405,  3425,  3445,  3466,  3485,  3506,  3546,  3568,
    3590,  3614,  3638,  3658,  3669,  3682,  3694,  3709,  3718,  3727,
    3737,  3750,  3760,  3771,  3779,  3792,  3793,  3796,  3807,  3819,
    3837,  3845,  3854,  3869,  3884,  3884,  3884,  3886,  3899,  3909,
    3920,  3930,  3940,  3953,  3963,  3974,  3984,  3993,  4006,  4016,
    4027,  4037,  4046,  4046,  4046,  4046,  4047,  4047,  4047,  4047,
    4048,  4048,  4048,  4048,  4050,  4050,  4051,  4051,  4052,  4052,
    4054,  4064,  4075,  4086,  4100,  4110,  4121,  4132,  4146,  4156,
    4167,  4178,  4192,  4201,  4215,  4229,  4246,  4255,  4269,  4283,
    4300,  4309,  4323,  4337,  4356,  4357,  4371,  4387,  4404,  4404,
    4404,  4406,  4432,  4462,  4498,  4522,  4549,  4572,  4596,  4617,
    4635,  4643,  4657,  4680,  4699,  4719,  4743,  4750,  4757,  4765,
    4781,  4796,  4804,  4813,  4821,  4829,  4841,  4849,  4852,  4862,
    4872,  4883,  4895,  4906,  4917,  4930,  4934,  4946,  4953,  4962,
    4973,  4975,  4990,  5005,  5020,  5034,  5048,  5053,  5065,  5079,
    5089,  5096,  5097,  5104,  5105,  5105,  5105,  5106,  5106,  5106,
    5106,  5107,  5107,  5107,  5107,  5107,  5107,  5108,  5108,  5108,
    5108,  5108,  5109,  5109,  5109,  5109,  5109,  5110,  5110,  5110,
    5110,  5111,  5111,  5111,  5112,  5112,  5114,  5115,  5116,  5117,
    5118,  5119,  5120,  5121,  5122,  5123,  5124,  5125,  5126,  5134,
    5145,  5145,  5147,  5147,  5147,  5149,  5164,  5166
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

#define YYPACT_NINF -322

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-322)))

#define YYTABLE_NINF -365

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     790,  -322,  -322,  -322,   -54,   448,   977,   114,  -322,  -322,
    -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,
    -322,  -322,  -322,  -322,  -322,    45,    45,    45,   180,  1606,
     994,    44,  1792,    33,   336,  1062,  -322,  -322,  1724,  -322,
    -322,  -322,  -322,   131,  1826,  1758,   300,  -322,  -322,  -322,
    -322,  -322,   362,  -322,    63,    25,    55,    61,  -322,    67,
     858,  -322,  1538,   369,  -322,   257,   180,    85,  -322,    96,
      61,   104,  -322,  -322,   261,  -322,   180,   382,   257,    53,
    1610,  1605,   402,    94,   103,   125,   180,   180,   180,   257,
    -322,  -322,   257,  -322,    45,  -322,   145,  1841,   517,  -322,
     112,  -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,
    -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,
    -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,
    -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,
    -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,
    -322,   220,  -322,  -322,  -322,  -322,   148,   150,   136,   151,
     155,    25,  1130,  1470,  -322,  -322,  -322,  -322,  -322,   165,
     174,  -322,   193,  -322,   195,  -322,    43,  -322,  -322,   201,
    -322,   257,  1893,  -322,   173,  -322,   257,  -322,    45,   568,
    -322,  -322,   180,   209,  -322,   180,   175,   257,  -322,  -322,
    -322,  1674,  1875,   257,  -322,  -322,  -322,  -322,   162,  -322,
     131,  -322,   180,  -322,    35,  -322,   307,  -322,  -322,   654,
    -322,   257,  -322,  -322,  -322,   222,  -322,    25,  -322,  -322,
     858,    13,  -322,   374,  -322,   233,   172,  -322,   238,   382,
     134,   180,  -322,  -322,   382,    61,  -322,   257,  -322,  -322,
      61,  -322,   175,  -322,  1689,  -322,   228,   180,  -322,  1607,
    -322,   243,   180,  -322,   911,  -322,   244,   180,  -322,  -322,
    -322,  -322,  -322,  -322,  -322,  -322,  -322,    45,  1198,  -322,
     180,   180,   180,   180,    61,  -322,  -322,  -322,   189,  -322,
    -322,   239,   585,  -322,   722,   206,  -322,  -322,  -322,  -322,
     254,  -322,   111,  1606,   926,   994,  1266,  -322,  -322,   127,
    -322,  -322,  -322,   256,   281,  -322,   213,  -322,  -322,   257,
    -322,   180,  -322,  -322,  -322,  -322,   180,   175,   180,    92,
      92,    35,  -322,   257,  -322,  -322,  -322,  -322,  1470,   218,
    -322,   180,  -322,  -322,  -322,   296,   297,  -322,   382,  -322,
      61,  -322,  -322,  -322,  -322,   291,  -322,  -322,   299,  -322,
    -322,   304,  -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,
    -322,  -322,  -322,    29,  -322,  -322,  -322,  -322,  -322,  -322,
    -322,  1334,  -322,  -322,  -322,   252,   319,   236,   318,  -322,
    1334,  -322,  -322,  -322,  -322,    28,  1606,   994,   329,  -322,
     324,  -322,   337,  -322,  -322,   265,   338,   180,  -322,  -322,
    -322,   275,  -322,  -322,  -322,    92,  -322,  -322,  -322,   180,
    -322,  -322,  -322,   297,    19,  -322,    61,  -322,  -322,  -322,
    -322,  -322,  -322,  -322,   341,   342,   344,  -322,   346,  -322,
    -322,  1470,  1470,   174,   195,   350,  -322,   295,  1402,  -322,
     354,  -322,   355,  -322,    92,  -322,  -322,  -322,  -322,  -322,
     357,  -322,  -322,  -322,  -322,   351,  -322,  -322,  -322,  -322,
    -322,  -322,  -322,  -322,   361,  -322,   363,   368,  -322
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       6,   381,   380,    33,   435,     0,     0,     0,   352,   351,
     282,   286,   290,   283,   287,   291,   284,   288,   292,   285,
     289,   293,   260,   226,   225,    72,    74,    86,    65,    67,
      69,    71,    42,   204,     0,    82,    35,    36,    45,   118,
     117,   119,   120,    96,    93,   339,     0,   346,   347,   348,
      97,    98,    78,   382,     0,     0,     0,     0,   201,    12,
      25,    28,    39,     0,    37,    77,    54,    60,   121,    53,
      63,    61,   203,   220,   223,    62,   230,   212,    40,   263,
     264,   265,   266,   300,   304,   308,     0,     0,     0,   340,
      88,    89,   350,    90,   353,    52,   357,   142,     0,   436,
     366,   358,   383,   434,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   384,   385,
     386,   387,   388,   389,   390,   391,   392,   393,   394,   395,
     396,   397,   398,   399,   400,   401,   402,   403,   404,   405,
     406,   407,   408,   409,   410,   411,   412,   413,   414,   415,
     365,     0,   367,   430,   431,   433,   432,   377,   301,   305,
     309,     0,   130,   132,     7,    73,    75,    87,    64,   155,
      66,   171,   179,   151,    68,   145,   149,   432,   150,     0,
      70,    43,     0,   205,     0,   217,   216,    85,    83,     0,
     379,   378,   242,   243,   259,   253,   258,    46,   248,   247,
     249,    48,   254,   104,    95,   103,   106,   107,   101,   102,
      92,    94,   110,    99,   112,   330,   338,   328,   329,   344,
     343,    81,    79,     1,     3,     4,     2,     0,    10,     8,
      13,     0,    29,     0,    38,     0,    30,    76,    55,   142,
       0,    56,   202,   209,   206,     0,   224,   221,   228,   214,
     213,    41,   262,   295,     0,   294,   312,   271,   297,     0,
     296,   316,   276,   299,     0,   298,   320,   281,   302,   306,
     310,   268,   273,   278,   341,   349,   354,   356,   128,   144,
     229,   269,   274,   279,   141,    22,   437,    24,     0,    18,
     361,   363,   369,   359,   375,     0,   303,   307,   311,     9,
     129,   131,     0,   173,   178,   147,   152,   148,   182,     0,
      44,   219,    84,   244,   231,   245,   251,   257,    49,    47,
      51,   252,   105,   100,    91,   109,   108,   327,   115,   336,
     334,   337,   345,     0,    80,     5,    11,    14,   130,    34,
      31,    58,   125,   124,   122,     0,    57,   133,   138,   208,
     207,   211,   222,   215,   261,   313,   314,   270,   317,   318,
     275,   321,   322,   280,   355,   126,   127,   227,   267,   272,
     277,   143,    21,     0,    23,    17,   360,   370,   368,   374,
     174,   373,   371,   175,   376,     0,   164,     0,     0,   172,
       0,   176,   146,   154,   153,     0,   186,   188,     0,   183,
       0,   190,     0,   246,   232,   233,   235,   226,   237,   255,
      50,   250,   111,   326,   114,   331,   324,   325,   335,   113,
     342,   129,    32,    59,   135,   140,   137,   210,   315,   319,
     323,    20,    19,   372,     0,   165,   163,   158,     0,   160,
     177,   199,   200,   185,   187,     0,   193,   192,   196,   234,
     238,   236,   239,   256,   332,   116,   136,   134,   139,   161,
     166,   159,   162,   197,   198,     0,   180,   184,   191,   194,
     195,   240,   241,   333,   167,   181,   168,   169,   170
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -322,  -322,   -52,  -322,   149,  -322,     8,   286,  -322,   325,
    -322,   326,   327,  -322,  -322,   -37,  -203,  -322,  -322,  -322,
    -322,   158,    47,   -34,   333,     6,    97,  -322,  -322,  -322,
    -286,    14,  -290,   101,  -322,  -322,  -322,   -33,  -322,   -38,
    -322,  -322,   378,  -322,  -322,  -322,   -36,    58,   -42,   211,
    -322,   215,   385,  -322,  -322,  -322,  -322,    -3,     2,    -1,
     167,   163,   159,   -22,     5,    16,  -322,  -322,  -322,  -321,
     214,  -322,  -322,  -322,  -322,  -322,  -322,   331,   -25,    23,
    -322,   133,  -322,     0,  -322,   386,     1,    -4,  -322,  -276
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    54,   226,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,   210,   204,   212,   213,   329,    66,
      67,    68,   346,   347,    69,   174,   175,   176,   168,   387,
     388,   170,   382,   171,   180,   309,   398,   399,   400,   401,
      70,    71,    72,    73,    74,    75,    76,    77,   194,   195,
     196,   197,    78,    79,    80,    81,    82,    83,    84,    85,
     256,   261,   266,    86,    87,    88,   257,   262,   267,   415,
     416,   216,    89,    90,    91,    92,    93,    94,    95,    96,
     151,   152,   201,   153,   154,   155,   177,   383,    98,   289
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      97,   157,   193,   158,   228,   160,   156,   211,   159,   418,
     221,   326,   375,   328,   391,   164,   198,    99,   205,   205,
     338,   220,     1,     2,   163,   224,   178,   222,   169,   431,
     172,   188,   182,   184,   184,   441,     1,     2,   202,   442,
     237,   206,   286,   199,   214,   217,   245,   179,   165,   166,
     167,   306,   250,   251,   200,   224,   227,     5,   187,     5,
      97,   280,    97,   223,   274,   229,   239,   275,    23,    24,
     207,   184,   284,     4,   184,   281,   248,   255,   242,   243,
     230,   265,   246,   260,   225,   249,   271,   272,   273,    53,
     456,   433,   186,   240,   454,     1,     2,   432,    97,   434,
     440,   438,   282,    53,   241,   279,   252,     1,     2,   299,
     268,  -156,   385,   283,   225,   161,     4,   276,     5,   269,
    -156,   162,     8,     9,  -156,   163,     4,   290,   419,   244,
       1,     2,   247,   473,  -123,   342,   203,     1,     2,    23,
      24,   270,  -123,  -123,   395,     5,   327,  -123,   280,     8,
       9,   231,   296,   313,   317,   294,   310,   295,   300,   301,
     281,   311,    53,     8,     9,   193,   193,   297,   396,   397,
    -156,   298,   318,   324,    53,   336,   320,   302,   322,   198,
     198,   386,   303,     1,     2,   205,   158,   282,   160,   372,
     373,   159,   314,  -123,   332,   316,   334,    53,   283,   307,
     304,   202,   286,   305,    53,   343,   199,   199,    23,    24,
     354,   312,   325,   308,   284,   333,   331,   200,   200,   350,
    -362,   291,   352,  -362,  -362,   315,   323,   192,   292,  -362,
      97,   335,  -362,  -362,  -362,   293,  -157,   436,  -362,  -362,
     345,   348,   339,   340,   356,  -157,   341,   279,   374,   437,
      53,   255,   349,   351,   376,  -362,  -362,   357,   353,   359,
     362,   260,   360,   265,     1,     2,   -16,   363,     1,     2,
     449,     5,   403,     4,   366,     8,     9,   384,   408,  -362,
     367,   368,   369,   370,   409,   413,   404,   405,   157,   422,
    -362,   406,   371,   156,   410,  -157,    23,    24,     1,     2,
     364,   178,   394,   278,   172,   424,   386,   428,   420,   402,
       1,     2,   395,   206,     5,   429,   407,    24,     8,     9,
     430,   411,   386,   426,   435,   446,   412,   439,   414,   417,
     417,    53,   447,  -189,   421,    53,  -218,   185,   445,     1,
       2,   348,   207,   451,   448,  -218,   453,   460,  -218,  -218,
     459,   465,   219,     1,     2,   462,   425,   461,   427,   471,
     472,   327,   474,   466,   475,    53,   476,   395,   477,   -27,
     235,    23,    24,   478,   -26,   235,     5,    53,   236,   337,
       8,     9,   -27,   236,   288,   232,   233,   -26,   423,   234,
     457,   396,   397,   178,     4,  -218,     5,   172,   344,   238,
       8,     9,   392,   444,   389,   450,    53,   452,   263,   468,
     443,   183,   467,   321,   192,   417,   319,   181,   264,   455,
      53,   355,   358,   361,   348,   378,    12,   277,   -27,    15,
     330,   218,    18,   -26,   458,    21,     0,   463,   464,     0,
       0,     0,     0,     0,   470,   402,     0,   402,  -364,   100,
       0,     1,     2,     0,   417,     0,  -364,  -364,     0,     0,
    -364,  -364,  -364,   101,     0,   102,  -364,  -364,   103,     0,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,  -364,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   285,    53,   150,
       1,     2,     0,     0,     0,     0,     3,     0,     0,     4,
     286,     5,     0,     6,   231,     8,     9,     0,     0,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,     0,     0,
       0,     0,    39,    40,    41,    42,   287,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,     1,     2,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   102,    23,    24,   103,     0,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,     0,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,     0,    53,   377,     1,     2,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     5,     0,
       0,   102,     8,     9,   103,     0,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
       0,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,     0,    53,     1,     2,   379,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   102,
       0,     0,   103,   380,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   381,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,     0,    53,     1,     2,     0,     0,     0,     0,     3,
       0,     0,     4,     0,     5,     0,     6,     7,     8,     9,
       0,     0,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,     0,     0,     0,     0,    39,    40,    41,    42,     0,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,     1,     2,     0,     0,     0,     0,     3,     0,     0,
       4,     0,     5,     0,     6,   231,     8,     9,     0,     0,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,     0,
       0,     0,     0,    39,    40,    41,    42,   263,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,     1,
       2,     0,     0,     0,     0,    12,     0,     0,    15,     0,
       0,    18,     0,   102,    21,     0,   103,   380,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   390,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,     0,    53,     1,     2,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,   102,     0,     0,   103,     0,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     173,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,     0,    53,     1,     2,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     5,     0,     0,   102,
       0,     0,   103,     0,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,     0,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,     0,    53,     1,     2,     0,     0,     0,     0,     0,
       0,     0,   -15,     0,     0,     0,     0,   102,     0,     0,
     103,     0,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,     0,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,     0,
      53,     1,     2,   365,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   102,     0,     0,   103,     0,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,     0,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,     0,    53,     1,
       2,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   102,     0,     0,   103,     0,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   393,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,     0,    53,     1,     2,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   102,     0,     0,   103,   380,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
       0,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,     0,    53,     1,     2,   469,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   102,
       0,     0,   103,     0,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,     0,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,     0,    53,     1,     2,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   102,     0,     0,
     103,     0,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,     0,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,     0,
      53,     1,     2,     0,     0,     0,     0,     0,     0,     0,
       4,     0,     5,     0,     6,   231,     8,     9,     0,     0,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,     0,     0,    38,     0,
       0,     0,     0,    39,    40,    41,    42,     0,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,     1,
       2,   258,     0,   258,     0,     0,   253,     0,     0,     0,
       0,   259,     0,   102,     0,     0,   254,     0,    11,     0,
      11,    14,    10,    14,    17,    13,    17,    20,    16,    20,
       0,    19,     0,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,     0,    53,     1,     2,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     5,     0,
     189,     0,     8,     9,     0,   253,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,     0,    23,
      24,    10,     0,     0,    13,     0,     0,    16,     0,     0,
      19,     0,     0,     0,     0,     0,   192,     1,     2,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     189,     0,     0,     0,    53,     0,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,     0,    23,
      24,     1,     2,     0,     0,     0,     0,     0,     0,   190,
       0,     0,     0,     0,     0,   191,   192,     0,     0,     0,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    53,     1,     2,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     6,     0,
       0,     0,     0,     0,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,   215,    53,     1,
       2,   203,   206,     0,     0,     0,     0,     0,     0,     0,
       5,     0,     0,     0,     8,     9,     0,     0,   278,     0,
       0,     0,     0,     4,     0,     5,     0,     6,     0,     8,
       9,   207,    53,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,     0,    23,    24,     0,     0,
     208,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     209,   189,     0,     0,     0,     0,    53,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,     6,
      23,    24,     0,     0,     0,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21
};

static const yytype_int16 yycheck[] =
{
       0,     5,    38,     6,    56,     6,     5,    44,     6,   330,
      52,   214,   288,   216,   304,     7,    38,    71,    43,    44,
       7,    46,     3,     4,    11,     0,    30,    52,    28,     0,
      29,    35,    32,    33,    34,     7,     3,     4,    38,    11,
      65,     6,    13,    38,    44,    45,    71,     3,    25,    26,
      27,     8,    77,    78,    38,     0,     1,    14,    35,    14,
      60,    97,    62,     0,    89,    57,    66,    92,    35,    36,
      35,    71,    97,    12,    74,    97,    76,    80,    70,    71,
      13,    82,    74,    81,    59,    77,    86,    87,    88,    70,
      71,   381,    34,     8,   415,     3,     4,   373,    98,   385,
     390,   387,    97,    70,     8,    97,    53,     3,     4,   161,
      16,     0,     1,    97,    59,     1,    12,    94,    14,    16,
       9,     7,    18,    19,    13,    11,    12,    15,   331,    71,
       3,     4,    74,   454,     0,     1,     5,     3,     4,    35,
      36,    16,     8,     9,    17,    14,    54,    13,   184,    18,
      19,    17,    16,   189,   196,     7,   181,     7,   162,   163,
     182,   186,    70,    18,    19,   201,   202,    16,    41,    42,
      59,    16,   197,   210,    70,   227,   201,    12,   203,   201,
     202,    70,     8,     3,     4,   210,   189,   182,   189,     0,
       1,   189,   192,    59,   219,   195,   221,    70,   182,   176,
       7,   201,    13,     8,    70,    71,   201,   202,    35,    36,
     252,   188,   212,    12,   239,   219,   216,   201,   202,   244,
       0,     1,   247,     3,     4,    16,    64,    52,     8,     9,
     230,     9,    12,    13,    14,    15,     0,     1,    18,    19,
     240,   241,     9,    71,    16,     9,     8,   239,    59,    13,
      70,   254,   244,   245,    15,    35,    36,   257,   250,    16,
      16,   259,   262,   264,     3,     4,    12,   267,     3,     4,
       5,    14,    16,    12,   278,    18,    19,    71,   314,    59,
     280,   281,   282,   283,    71,   327,     5,     6,   292,    71,
      70,    10,   284,   292,   319,    59,    35,    36,     3,     4,
     277,   305,   306,     7,   303,     8,    70,    16,   333,   309,
       3,     4,    17,     6,    14,    16,    35,    36,    18,    19,
      16,   321,    70,   348,     5,     1,   326,     9,   328,   329,
     330,    70,     8,     9,   338,    70,     0,     1,     9,     3,
       4,   341,    35,     5,     7,     9,    71,     5,    12,    13,
       9,     1,    52,     3,     4,     9,   348,    13,   350,     5,
       5,    54,     5,    13,    13,    70,     5,    17,     5,     0,
       1,    35,    36,     5,     0,     1,    14,    70,     9,   230,
      18,    19,    13,     9,    98,    60,    60,    13,   341,    62,
     424,    41,    42,   397,    12,    59,    14,   396,   240,    66,
      18,    19,   305,   397,   303,   405,    70,   407,     6,   447,
     396,    33,   445,   202,    52,   415,   201,    32,    16,   419,
      70,   254,   259,   264,   424,   292,    24,    96,    59,    27,
     216,    45,    30,    59,   426,    33,    -1,   441,   442,    -1,
      -1,    -1,    -1,    -1,   448,   445,    -1,   447,     0,     1,
      -1,     3,     4,    -1,   454,    -1,     8,     9,    -1,    -1,
      12,    13,    14,    15,    -1,    17,    18,    19,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,     0,    70,    71,
       3,     4,    -1,    -1,    -1,    -1,     9,    -1,    -1,    12,
      13,    14,    -1,    16,    17,    18,    19,    -1,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    -1,    -1,
      -1,    -1,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,     3,     4,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    17,    35,    36,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    -1,    70,    71,     3,     4,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    19,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    -1,    70,     3,     4,     5,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    -1,    70,     3,     4,    -1,    -1,    -1,    -1,     9,
      -1,    -1,    12,    -1,    14,    -1,    16,    17,    18,    19,
      -1,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    -1,    -1,    -1,    -1,    55,    56,    57,    58,    -1,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,     3,     4,    -1,    -1,    -1,    -1,     9,    -1,    -1,
      12,    -1,    14,    -1,    16,    17,    18,    19,    -1,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    -1,
      -1,    -1,    -1,    55,    56,    57,    58,     6,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,     3,
       4,    -1,    -1,    -1,    -1,    24,    -1,    -1,    27,    -1,
      -1,    30,    -1,    17,    33,    -1,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,     3,     4,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    17,    -1,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    -1,    70,     3,     4,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    14,    -1,    -1,    17,
      -1,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    -1,    70,     3,     4,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    -1,
      70,     3,     4,     5,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,     3,
       4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,     3,     4,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    -1,    70,     3,     4,     5,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    -1,    70,     3,     4,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    -1,
      70,     3,     4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    14,    -1,    16,    17,    18,    19,    -1,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    55,    56,    57,    58,    -1,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,     3,
       4,     6,    -1,     6,    -1,    -1,     6,    -1,    -1,    -1,
      -1,    16,    -1,    17,    -1,    -1,    16,    -1,    23,    -1,
      23,    26,    22,    26,    29,    25,    29,    32,    28,    32,
      -1,    31,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,     3,     4,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    -1,
      16,    -1,    18,    19,    -1,     6,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    -1,    35,
      36,    22,    -1,    -1,    25,    -1,    -1,    28,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    52,     3,     4,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      16,    -1,    -1,    -1,    70,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    -1,    35,
      36,     3,     4,    -1,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    -1,    -1,    -1,    51,    52,    -1,    -1,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    70,     3,     4,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    16,    -1,
      -1,    -1,    -1,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    69,    70,     3,
       4,     5,     6,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      14,    -1,    -1,    -1,    18,    19,    -1,    -1,     7,    -1,
      -1,    -1,    -1,    12,    -1,    14,    -1,    16,    -1,    18,
      19,    35,    70,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    -1,    35,    36,    -1,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    16,    -1,    -1,    -1,    -1,    70,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    16,
      35,    36,    -1,    -1,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33
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
     145,   146,   147,   148,   149,   150,   151,   155,   160,    71,
       1,    15,    17,    20,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      71,   152,   153,   155,   156,   157,   158,   159,   129,   130,
     131,     1,     7,    11,    78,   151,   151,   151,   100,   155,
     103,   105,   158,    36,    97,    98,    99,   158,   159,     3,
     106,   124,   155,   114,   155,     1,   119,   151,   159,    16,
      45,    51,    52,   118,   120,   121,   122,   123,   135,   136,
     137,   154,   155,     5,    87,   150,     6,    35,    54,    64,
      86,    87,    88,    89,   155,    69,   143,   155,   157,    52,
     150,   120,   150,     0,     0,    59,    74,     1,    74,    78,
      13,    17,    81,    83,    84,     1,     9,   150,    96,   155,
       8,     8,    78,    78,   119,   150,    78,   119,   155,    78,
     150,   150,    53,     6,    16,   129,   132,   138,     6,    16,
     130,   133,   139,     6,    16,   131,   134,   140,    16,    16,
      16,   155,   155,   155,   150,   150,   151,   149,     7,    78,
     118,   135,   136,   137,   150,     0,    13,    59,    79,   161,
      15,     1,     8,    15,     7,     7,    16,    16,    16,    74,
     159,   159,    12,     8,     7,     8,     8,   151,    12,   107,
     150,   150,   151,   118,   155,    16,   155,   120,   150,   123,
     150,   121,   150,    64,    87,   155,    88,    54,    88,    90,
     142,   155,   150,   159,   150,     9,    74,    76,     7,     9,
      71,     8,     1,    71,    93,   155,    94,    95,   155,    78,
     150,    78,   150,    78,   120,   132,    16,   155,   133,    16,
     155,   134,    16,   155,   151,     5,   159,   155,   155,   155,
     155,    78,     0,     1,    59,   161,    15,    71,   153,     5,
      21,    36,   104,   159,    71,     1,    70,   101,   102,   105,
      36,   104,    98,    36,   159,    17,    41,    42,   108,   109,
     110,   111,   155,    16,     5,     6,    10,    35,   118,    71,
     150,   155,   155,   120,   155,   141,   142,   155,   141,    88,
     150,   159,    71,    94,     8,    78,   150,    78,    16,    16,
      16,     0,   161,   104,   102,     5,     1,    13,   102,     9,
     104,     7,    11,   103,    97,     9,     1,     8,     7,     5,
     155,     5,   155,    71,   141,   155,    71,    95,    78,     9,
       5,    13,     9,   159,   159,     1,    13,   109,   111,     5,
     159,     5,     5,   141,     5,    13,     5,     5,     5
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
      84,    84,    84,    84,    84,    84,    84,    85,    85,    86,
      86,    86,    86,    87,    87,    87,    88,    88,    89,    89,
      89,    89,    89,    90,    90,    90,    90,    91,    91,    91,
      91,    92,    92,    92,    92,    92,    93,    93,    93,    93,
      93,    93,    93,    94,    94,    94,    94,    95,    95,    95,
      95,    96,    96,    96,    96,    97,    97,    97,    98,    98,
      99,    99,    99,    99,    99,   100,   100,   100,   100,   100,
     101,   101,   101,   101,   102,   102,   102,   102,   102,   102,
     102,   103,   103,   103,   104,   104,   105,   105,   105,   105,
     106,   106,   107,   108,   108,   109,   109,   109,   109,   109,
     110,   110,   110,   110,   111,   111,   111,   111,   111,   111,
     111,   112,   112,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   114,   114,   114,   114,   115,   115,   115,   115,
     116,   116,   116,   117,   117,   118,   118,   119,   119,   119,
     119,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   121,   121,   121,   121,   121,   121,   121,
     122,   122,   122,   122,   122,   122,   122,   123,   123,   123,
     124,   124,   124,   124,   125,   125,   125,   126,   126,   126,
     126,   126,   127,   127,   127,   127,   127,   128,   128,   128,
     128,   128,   129,   129,   129,   129,   130,   130,   130,   130,
     131,   131,   131,   131,   132,   132,   133,   133,   134,   134,
     135,   135,   135,   135,   136,   136,   136,   136,   137,   137,
     137,   137,   138,   138,   138,   138,   139,   139,   139,   139,
     140,   140,   140,   140,   141,   141,   142,   142,   143,   143,
     143,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     145,   145,   146,   146,   146,   146,   147,   147,   147,   148,
     148,   149,   149,   150,   150,   150,   150,   150,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   152,   152,   152,
     152,   153,   153,   153,   153,   153,   153,   153,   154,   154,
     155,   155,   155,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     158,   158,   159,   159,   159,   160,   160,   161
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     1,     2,     0,     2,     2,     3,
       2,     3,     1,     2,     3,     2,     3,     3,     2,     4,
       4,     3,     2,     3,     2,     1,     2,     1,     1,     2,
       2,     3,     4,     1,     3,     1,     1,     1,     2,     1,
       1,     2,     1,     2,     3,     1,     2,     3,     2,     3,
       4,     3,     1,     1,     1,     2,     2,     3,     3,     4,
       1,     1,     1,     1,     2,     1,     2,     1,     2,     1,
       2,     1,     1,     2,     1,     2,     2,     1,     1,     2,
       3,     2,     1,     2,     3,     2,     1,     2,     1,     1,
       1,     3,     2,     1,     2,     2,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     2,     1,     1,     2,     2,
       1,     3,     1,     2,     2,     1,     3,     1,     1,     1,
       1,     1,     3,     2,     3,     3,     3,     3,     2,     3,
       2,     3,     2,     1,     3,     2,     3,     2,     1,     3,
       2,     2,     1,     3,     2,     1,     3,     2,     2,     1,
       1,     1,     2,     3,     3,     1,     2,     3,     4,     5,
       2,     3,     3,     2,     1,     2,     3,     4,     5,     6,
       7,     1,     3,     2,     1,     1,     3,     4,     2,     1,
       5,     6,     1,     1,     3,     2,     1,     2,     1,     1,
       1,     3,     2,     2,     3,     3,     2,     3,     3,     2,
       2,     1,     2,     1,     1,     2,     2,     3,     3,     2,
       4,     3,     1,     2,     2,     3,     2,     2,     1,     3,
       1,     2,     3,     1,     2,     1,     1,     3,     2,     2,
       1,     2,     3,     3,     4,     3,     4,     3,     4,     4,
       5,     5,     1,     1,     2,     2,     3,     1,     1,     1,
       3,     2,     2,     1,     1,     3,     4,     2,     1,     1,
       1,     3,     2,     1,     1,     1,     1,     3,     2,     2,
       3,     2,     3,     2,     2,     3,     2,     3,     2,     2,
       3,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     3,     1,     2,     2,     3,     1,     2,
       2,     3,     1,     2,     2,     3,     1,     2,     2,     3,
       1,     2,     2,     3,     1,     1,     2,     1,     1,     1,
       1,     4,     5,     6,     3,     4,     3,     3,     2,     1,
       1,     2,     4,     2,     2,     3,     1,     1,     1,     2,
       1,     1,     1,     1,     2,     3,     2,     1,     2,     3,
       4,     3,     2,     3,     1,     2,     2,     1,     3,     2,
       3,     3,     4,     3,     3,     2,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     1
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
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2045 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 4: /* TOK_QSTRING  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2051 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 5: /* TOK_NUMBER  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2057 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 17: /* TOK_MSC  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2063 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 18: /* TOK_COLON_STRING  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2069 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 19: /* TOK_COLON_QUOTED_STRING  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2075 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 20: /* TOK_STYLE_NAME  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2081 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 21: /* TOK_COLORDEF  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2087 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 37: /* TOK_COMMAND_HEADING  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2093 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 38: /* TOK_COMMAND_NUDGE  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2099 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 39: /* TOK_COMMAND_NEWPAGE  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2105 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 40: /* TOK_COMMAND_DEFSHAPE  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2111 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 41: /* TOK_COMMAND_DEFCOLOR  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2117 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 42: /* TOK_COMMAND_DEFSTYLE  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2123 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 43: /* TOK_COMMAND_DEFDESIGN  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2129 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 44: /* TOK_COMMAND_BIG  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2135 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 45: /* TOK_COMMAND_BOX  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2141 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 46: /* TOK_COMMAND_PIPE  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2147 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 47: /* TOK_COMMAND_MARK  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2153 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 48: /* TOK_COMMAND_PARALLEL  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2159 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 49: /* TOK_COMMAND_OVERLAP  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2165 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 50: /* TOK_VERTICAL  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2171 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 51: /* TOK_VERTICAL_SHAPE  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2177 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 52: /* TOK_AT  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2183 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 53: /* TOK_LOST  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2189 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 54: /* TOK_AT_POS  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2195 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 55: /* TOK_SHOW  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2201 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 56: /* TOK_HIDE  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2207 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 57: /* TOK_ACTIVATE  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2213 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 58: /* TOK_DEACTIVATE  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2219 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 59: /* TOK_BYE  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2225 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 60: /* TOK_COMMAND_VSPACE  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2231 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 61: /* TOK_COMMAND_HSPACE  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2237 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 62: /* TOK_COMMAND_SYMBOL  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2243 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 63: /* TOK_COMMAND_NOTE  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2249 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 64: /* TOK_COMMAND_COMMENT  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2255 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 65: /* TOK_COMMAND_ENDNOTE  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2261 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 66: /* TOK_COMMAND_FOOTNOTE  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2267 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 67: /* TOK_COMMAND_TITLE  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2273 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 68: /* TOK_COMMAND_SUBTITLE  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2279 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 69: /* TOK_COMMAND_TEXT  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2285 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 76: /* top_level_arclist  */
#line 172 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arclist);}
#line 2291 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 78: /* braced_arclist  */
#line 172 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arclist);}
#line 2297 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 79: /* arclist_maybe_no_semicolon  */
#line 172 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arclist);}
#line 2303 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 80: /* arclist  */
#line 172 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arclist);}
#line 2309 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 81: /* arc_with_parallel_semicolon  */
#line 172 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2315 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 82: /* overlap_or_parallel  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2321 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 83: /* arc_with_parallel  */
#line 172 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2327 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 84: /* arc  */
#line 172 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2333 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 85: /* titlecommandtoken  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2339 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 86: /* hspace_location  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).namerel);}
#line 2345 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 87: /* full_arcattrlist_with_label_or_number  */
#line 174 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).attriblist);}
#line 2351 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 89: /* entityrel  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).namerel);}
#line 2357 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 90: /* markerrel_no_string  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).namerel);}
#line 2363 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 91: /* entity_command_prefixes  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2369 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 92: /* optlist  */
#line 172 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arclist);}
#line 2375 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 93: /* opt  */
#line 172 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2381 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 94: /* entitylist  */
#line 174 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).entitylist);}
#line 2387 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 95: /* entity  */
#line 174 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).entitylist);}
#line 2393 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 96: /* first_entity  */
#line 174 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).entitylist);}
#line 2399 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 99: /* stylenamelist  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).stringlist);}
#line 2405 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 104: /* color_string  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2411 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 112: /* parallel  */
#line 173 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcparallel);}
#line 2417 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 113: /* box_list  */
#line 173 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcboxseries);}
#line 2423 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 114: /* first_box  */
#line 173 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbox);}
#line 2429 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 115: /* first_pipe  */
#line 173 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcpipe);}
#line 2435 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 116: /* pipe_list_no_content  */
#line 173 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcpipeseries);}
#line 2441 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 117: /* pipe_list  */
#line 173 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcpipeseries);}
#line 2447 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 119: /* boxrel  */
#line 173 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbox);}
#line 2453 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 120: /* vertxpos  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).vertxpos);}
#line 2459 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 122: /* vertrel_no_xpos  */
#line 172 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcvertarrow);}
#line 2465 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 123: /* vertrel  */
#line 172 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcvertarrow);}
#line 2471 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 124: /* arcrel  */
#line 172 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2477 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 125: /* arcrel_arrow  */
#line 172 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcarrow);}
#line 2483 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 126: /* arcrel_to  */
#line 172 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcarrow);}
#line 2489 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 127: /* arcrel_from  */
#line 172 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcarrow);}
#line 2495 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 128: /* arcrel_bidir  */
#line 172 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcarrow);}
#line 2501 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 141: /* extvertxpos  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).extvertxpos);}
#line 2507 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 142: /* extvertxpos_no_string  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).extvertxpos);}
#line 2513 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 143: /* symbol_type_string  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2519 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 144: /* symbol_command_no_attr  */
#line 172 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2525 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 145: /* symbol_command  */
#line 172 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2531 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 146: /* note  */
#line 172 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2537 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 148: /* comment  */
#line 172 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2543 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 149: /* colon_string  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2549 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 150: /* full_arcattrlist_with_label  */
#line 174 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).attriblist);}
#line 2555 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 151: /* full_arcattrlist  */
#line 174 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).attriblist);}
#line 2561 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 152: /* arcattrlist  */
#line 174 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).attriblist);}
#line 2567 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 153: /* arcattr  */
#line 174 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).attrib);}
#line 2573 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 155: /* entity_string  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2579 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 156: /* reserved_word_string  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2585 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 157: /* symbol_string  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2591 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 158: /* alpha_string  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2597 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 159: /* string  */
#line 176 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2603 "colorsyntax.cc" /* yacc.c:1257  */
        break;

    case 161: /* scope_close  */
#line 172 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2609 "colorsyntax.cc" /* yacc.c:1257  */
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

#line 2725 "colorsyntax.cc" /* yacc.c:1429  */
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
#line 243 "language.yy" /* yacc.c:1646  */
    {
	YYACCEPT;
}
#line 2916 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 4:
#line 249 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
	csh.AddCSH_AllCommentBeyond((yylsp[0]));
  #else
  #endif
    free((yyvsp[0].str));
}
#line 2929 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 5:
#line 258 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[0]), COLOR_SEMICOLON);
	csh.AddCSH_AllCommentBeyond((yylsp[0]));
  #else
  #endif
    free((yyvsp[-1].str));
}
#line 2943 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 6:
#line 269 "language.yy" /* yacc.c:1646  */
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
#line 2958 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 7:
#line 280 "language.yy" /* yacc.c:1646  */
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
#line 2975 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 8:
#line 293 "language.yy" /* yacc.c:1646  */
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
#line 2997 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 9:
#line 311 "language.yy" /* yacc.c:1646  */
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
#line 3016 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 10:
#line 326 "language.yy" /* yacc.c:1646  */
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
#line 3032 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 11:
#line 338 "language.yy" /* yacc.c:1646  */
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
#line 3053 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 13:
#line 357 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[0]), "Closing brace missing its opening pair.");
  #else
    (yyval.arclist) = (yyvsp[-1].arclist);
    msc.Error.Error(MSC_POS((yylsp[0])).start, "Unexpected '}'.");
  #endif
}
#line 3066 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 14:
#line 366 "language.yy" /* yacc.c:1646  */
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
#line 3082 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 15:
#line 380 "language.yy" /* yacc.c:1646  */
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
#line 3099 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 16:
#line 393 "language.yy" /* yacc.c:1646  */
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
#line 3121 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 17:
#line 412 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    if ((yyvsp[0].arcbase)) ((yyvsp[-1].arclist))->Append((yyvsp[0].arcbase)); //Append any potential CommandNumbering
    (yyval.arclist) = (yyvsp[-1].arclist);
  #endif
}
#line 3133 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 18:
#line 420 "language.yy" /* yacc.c:1646  */
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
#line 3148 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 19:
#line 431 "language.yy" /* yacc.c:1646  */
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
#line 3163 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 20:
#line 442 "language.yy" /* yacc.c:1646  */
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
#line 3178 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 21:
#line 453 "language.yy" /* yacc.c:1646  */
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
#line 3193 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 22:
#line 464 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ErrorAfter((yylsp[-1]), "Missing a closing brace ('}').");
  #else
    (yyval.arclist) = NULL;
    //Do not pop context, as the missing scope_close would have done
    msc.Error.Error(MSC_POS((yylsp[-1])).end.NextChar(), "Missing a corresponding '}'.");
  #endif
}
#line 3207 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 23:
#line 474 "language.yy" /* yacc.c:1646  */
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
#line 3223 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 24:
#line 486 "language.yy" /* yacc.c:1646  */
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
#line 3238 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 26:
#line 501 "language.yy" /* yacc.c:1646  */
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
#line 3253 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 27:
#line 512 "language.yy" /* yacc.c:1646  */
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
#line 3270 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 28:
#line 527 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[0].arcbase))
        (yyval.arclist) = (new ArcList)->Append((yyvsp[0].arcbase)); /* New list */
    else
        (yyval.arclist) = new ArcList;
  #endif
}
#line 3283 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 29:
#line 536 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[0].arcbase)) ((yyvsp[-1].arclist))->Append((yyvsp[0].arcbase));     /* Add to existing list */
    (yyval.arclist) = ((yyvsp[-1].arclist));
  #endif
}
#line 3294 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 30:
#line 544 "language.yy" /* yacc.c:1646  */
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
#line 3310 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 33:
#line 560 "language.yy" /* yacc.c:1646  */
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
#line 3326 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 34:
#line 572 "language.yy" /* yacc.c:1646  */
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
#line 3345 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 37:
#line 590 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    if ((yyvsp[0].arcbase)) 
		((yyvsp[0].arcbase))->SetLineEnd(MSC_POS((yyloc)));
    (yyval.arcbase)=(yyvsp[0].arcbase);
  #endif
}
#line 3358 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 38:
#line 599 "language.yy" /* yacc.c:1646  */
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
#line 3386 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 39:
#line 623 "language.yy" /* yacc.c:1646  */
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
#line 3407 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 40:
#line 641 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    ((yyvsp[0].arcbase))->AddAttributeList(NULL);
    (yyval.arcbase)=((yyvsp[0].arcbase));
  #endif
}
#line 3419 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 41:
#line 649 "language.yy" /* yacc.c:1646  */
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
#line 3435 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 42:
#line 661 "language.yy" /* yacc.c:1646  */
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
#line 3456 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 43:
#line 678 "language.yy" /* yacc.c:1646  */
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
#line 3480 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 44:
#line 698 "language.yy" /* yacc.c:1646  */
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
#line 3507 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 45:
#line 721 "language.yy" /* yacc.c:1646  */
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
#line 3530 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 46:
#line 740 "language.yy" /* yacc.c:1646  */
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
#line 3555 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 47:
#line 761 "language.yy" /* yacc.c:1646  */
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
#line 3582 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 48:
#line 784 "language.yy" /* yacc.c:1646  */
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
	typeplusdir.arc.type = MSC_ARC_SOLID;
	typeplusdir.arc.lost = EArrowLost::NOT;
	typeplusdir.dir = MSC_DIR_RIGHT;
	ArcVerticalArrow *ava = new ArcVerticalArrow(typeplusdir, MARKER_HERE_STR, MARKER_HERE_STR, &msc);
	VertXPos vxp(msc);
	ava->AddXpos(&vxp);
    ava->SetVerticalShape((yyvsp[0].vshape));
    ava->AddAttributeList(NULL);
	(yyval.arcbase) = ava;
  #endif
    free((yyvsp[-1].str));
}
#line 3617 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 49:
#line 815 "language.yy" /* yacc.c:1646  */
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
#line 3645 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 50:
#line 839 "language.yy" /* yacc.c:1646  */
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
#line 3674 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 51:
#line 864 "language.yy" /* yacc.c:1646  */
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
	typeplusdir.arc.type = MSC_ARC_SOLID;
	typeplusdir.arc.lost = EArrowLost::NOT;
	typeplusdir.dir = MSC_DIR_RIGHT;
	ArcVerticalArrow *ava = new ArcVerticalArrow(typeplusdir, MARKER_HERE_STR, MARKER_HERE_STR, &msc);
	VertXPos vxp(msc);
	ava->AddXpos(&vxp);
    ava->SetVerticalShape((yyvsp[-1].vshape));
    ava->AddAttributeList((yyvsp[0].attriblist));
	(yyval.arcbase) = ava;
  #endif
    free((yyvsp[-2].str));
}
#line 3712 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 52:
#line 898 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0])))
        ArcDivider::AttributeNames(csh, false, false);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0])))
        ArcDivider::AttributeValues(csh.hintAttrName, csh, false, false);
  #else
    (yyval.arcbase) = (new ArcDivider(MSC_ARC_VSPACE, &msc));
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
  #endif
}
#line 3728 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 53:
#line 910 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (new CommandEntity((yyvsp[0].entitylist), &msc, false));
    ((yyval.arcbase))->AddAttributeList(NULL);
  #endif
}
#line 3740 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 54:
#line 918 "language.yy" /* yacc.c:1646  */
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
#line 3760 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 55:
#line 934 "language.yy" /* yacc.c:1646  */
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
#line 3780 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 56:
#line 950 "language.yy" /* yacc.c:1646  */
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
#line 3797 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 57:
#line 963 "language.yy" /* yacc.c:1646  */
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
#line 3814 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 58:
#line 976 "language.yy" /* yacc.c:1646  */
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
#line 3838 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 59:
#line 996 "language.yy" /* yacc.c:1646  */
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
#line 3861 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 60:
#line 1015 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    /* If there were arcs defined by the options (e.g., background)
     * enclose them in an "CommandArcList" element used only for this. */
    if ((yyvsp[0].arclist)) {
        (yyval.arcbase) = (new CommandArcList(&msc, (yyvsp[0].arclist)));
        ((yyval.arcbase))->AddAttributeList(NULL);
		delete ((yyvsp[0].arclist));
    } else
        (yyval.arcbase) = NULL;
  #endif
}
#line 3879 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 61:
#line 1029 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (yyvsp[0].arcboxseries); //to remove warning for downcast
  #endif
}
#line 3890 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 62:
#line 1036 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (yyvsp[0].arcpipeseries); //to remove warning for downcast
  #endif
}
#line 3901 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 63:
#line 1043 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = ((yyvsp[0].arcparallel));
	if ((yyvsp[0].arcparallel))
        ((yyval.arcbase))->AddAttributeList(NULL);
  #endif
}
#line 3914 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 64:
#line 1052 "language.yy" /* yacc.c:1646  */
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
#line 3931 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 65:
#line 1065 "language.yy" /* yacc.c:1646  */
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
#line 3949 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 66:
#line 1079 "language.yy" /* yacc.c:1646  */
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
#line 3969 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 67:
#line 1095 "language.yy" /* yacc.c:1646  */
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
#line 3991 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 68:
#line 1113 "language.yy" /* yacc.c:1646  */
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
#line 4011 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 69:
#line 1129 "language.yy" /* yacc.c:1646  */
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
#line 4033 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 70:
#line 1147 "language.yy" /* yacc.c:1646  */
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
#line 4050 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 71:
#line 1160 "language.yy" /* yacc.c:1646  */
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
#line 4069 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 72:
#line 1175 "language.yy" /* yacc.c:1646  */
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
#line 4087 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 73:
#line 1189 "language.yy" /* yacc.c:1646  */
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
#line 4108 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 74:
#line 1206 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } 
  #else
    (yyval.arcbase) = (new ArcDivider(MSC_COMMAND_NUDGE, &msc));
    ((yyval.arcbase))->AddAttributeList(NULL);
  #endif
    free((yyvsp[0].str));
}
#line 4126 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 75:
#line 1220 "language.yy" /* yacc.c:1646  */
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
    (yyval.arcbase) = (new ArcDivider(MSC_COMMAND_NUDGE, &msc));
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
  #endif
    free((yyvsp[-1].str));
}
#line 4147 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 76:
#line 1237 "language.yy" /* yacc.c:1646  */
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
    const EArcType t = CaseInsensitiveEqual("title", (yyvsp[-1].str)) ? MSC_COMMAND_TITLE :
                         CaseInsensitiveEqual("subtitle", (yyvsp[-1].str)) ? MSC_COMMAND_SUBTITLE :
                         MSC_ARC_INVALID;
    (yyval.arcbase) = (new ArcDivider(t, &msc));
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
  #endif
    free((yyvsp[-1].str));
}
#line 4171 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 77:
#line 1257 "language.yy" /* yacc.c:1646  */
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
#line 4190 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 78:
#line 1272 "language.yy" /* yacc.c:1646  */
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
#line 4212 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 79:
#line 1290 "language.yy" /* yacc.c:1646  */
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
#line 4236 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 80:
#line 1310 "language.yy" /* yacc.c:1646  */
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
#line 4257 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 81:
#line 1327 "language.yy" /* yacc.c:1646  */
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
#line 4278 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 82:
#line 1344 "language.yy" /* yacc.c:1646  */
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
#line 4297 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 83:
#line 1359 "language.yy" /* yacc.c:1646  */
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
#line 4318 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 84:
#line 1376 "language.yy" /* yacc.c:1646  */
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
#line 4342 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 85:
#line 1396 "language.yy" /* yacc.c:1646  */
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
#line 4363 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 86:
#line 1413 "language.yy" /* yacc.c:1646  */
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
#line 4381 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 87:
#line 1427 "language.yy" /* yacc.c:1646  */
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
#line 4402 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 91:
#line 1447 "language.yy" /* yacc.c:1646  */
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
#line 4423 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 92:
#line 1464 "language.yy" /* yacc.c:1646  */
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
#line 4449 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 93:
#line 1486 "language.yy" /* yacc.c:1646  */
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
#line 4472 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 94:
#line 1505 "language.yy" /* yacc.c:1646  */
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
#line 4498 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 95:
#line 1527 "language.yy" /* yacc.c:1646  */
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
#line 4519 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 96:
#line 1544 "language.yy" /* yacc.c:1646  */
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
#line 4545 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 100:
#line 1570 "language.yy" /* yacc.c:1646  */
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
#line 4575 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 101:
#line 1596 "language.yy" /* yacc.c:1646  */
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
#line 4601 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 102:
#line 1618 "language.yy" /* yacc.c:1646  */
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
#line 4620 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 104:
#line 1636 "language.yy" /* yacc.c:1646  */
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
#line 4635 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 105:
#line 1647 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_ATTRVALUE);
  #else
    ((yyvsp[0].attriblist))->Append(new Attribute("space", atof((yyvsp[-1].str)), MSC_POS((yylsp[-1])), MSC_POS((yylsp[-1])), (yyvsp[-1].str)));
    (yyval.attriblist) = (yyvsp[0].attriblist);
  #endif
    free((yyvsp[-1].str));
}
#line 4649 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 108:
#line 1660 "language.yy" /* yacc.c:1646  */
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
#line 4665 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 109:
#line 1672 "language.yy" /* yacc.c:1646  */
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
#line 4680 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 110:
#line 1683 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.namerel) = NULL;
  #endif
}
#line 4693 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 111:
#line 1692 "language.yy" /* yacc.c:1646  */
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
#line 4711 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 112:
#line 1706 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
    csh.CheckEntityHintAt((yylsp[0]));
  #else
    (yyval.namerel) = new NamePair((yyvsp[0].str), MSC_POS((yylsp[0])), NULL, FileLineColRange());
  #endif
    free((yyvsp[0].str));
}
#line 4725 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 113:
#line 1718 "language.yy" /* yacc.c:1646  */
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
#line 4741 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 114:
#line 1730 "language.yy" /* yacc.c:1646  */
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
#line 4756 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 115:
#line 1741 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    csh.CheckHintAt((yylsp[0]), EHintSourceType::MARKER);
    csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::MARKER);
  #else
    (yyval.namerel) = NULL;
  #endif
}
#line 4770 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 116:
#line 1751 "language.yy" /* yacc.c:1646  */
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
#line 4788 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 121:
#line 1768 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[0].arcbase)) {
        (yyval.arclist) = (new ArcList)->Append((yyvsp[0].arcbase)); /* New list */
        //($1)->MakeMeLastNotable(); Do not make chart options notable
    } else
        (yyval.arclist) = NULL;
  #endif
}
#line 4802 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 122:
#line 1778 "language.yy" /* yacc.c:1646  */
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
#line 4825 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 123:
#line 1797 "language.yy" /* yacc.c:1646  */
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
#line 4842 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 125:
#line 1811 "language.yy" /* yacc.c:1646  */
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
#line 4859 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 126:
#line 1826 "language.yy" /* yacc.c:1646  */
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
#line 4882 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 127:
#line 1845 "language.yy" /* yacc.c:1646  */
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
#line 4905 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 128:
#line 1864 "language.yy" /* yacc.c:1646  */
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
#line 4928 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 129:
#line 1883 "language.yy" /* yacc.c:1646  */
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
#line 4954 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 130:
#line 1905 "language.yy" /* yacc.c:1646  */
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
#line 4977 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 131:
#line 1924 "language.yy" /* yacc.c:1646  */
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
#line 5003 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 132:
#line 1946 "language.yy" /* yacc.c:1646  */
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
#line 5026 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 133:
#line 1966 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.entitylist) = ((yyvsp[0].entitylist));
  #endif
}
#line 5036 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 134:
#line 1972 "language.yy" /* yacc.c:1646  */
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
#line 5051 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 135:
#line 1983 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_COMMA);
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.entitylist) = ((yyvsp[-1].entitylist));
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Expecting an entity here.");
  #endif
}
#line 5065 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 137:
#line 1998 "language.yy" /* yacc.c:1646  */
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
#line 5085 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 138:
#line 2014 "language.yy" /* yacc.c:1646  */
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
#line 5101 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 139:
#line 2026 "language.yy" /* yacc.c:1646  */
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
#line 5121 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 140:
#line 2042 "language.yy" /* yacc.c:1646  */
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
#line 5137 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 141:
#line 2055 "language.yy" /* yacc.c:1646  */
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
#line 5159 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 142:
#line 2073 "language.yy" /* yacc.c:1646  */
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
#line 5178 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 143:
#line 2088 "language.yy" /* yacc.c:1646  */
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
#line 5200 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 144:
#line 2106 "language.yy" /* yacc.c:1646  */
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
#line 5219 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 146:
#line 2123 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_COMMA);
    if (csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_VALUE)) {
        csh.AddStylesToHints(true, true);
        csh.hintStatus = HINT_READY;
    }
  #endif
}
#line 5233 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 147:
#line 2133 "language.yy" /* yacc.c:1646  */
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
#line 5249 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 148:
#line 2146 "language.yy" /* yacc.c:1646  */
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
#line 5301 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 149:
#line 2194 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
	csh.AddCSH_ErrorAfter((yyloc), "Missing attribute definitons in square brackets ('[' and ']').");
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing attribute definitons in square brackets ('[' and ']').");
  #endif
    delete((yyvsp[0].stringlist));
}
#line 5314 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 150:
#line 2205 "language.yy" /* yacc.c:1646  */
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
#line 5338 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 151:
#line 2225 "language.yy" /* yacc.c:1646  */
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
#line 5357 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 152:
#line 2240 "language.yy" /* yacc.c:1646  */
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
#line 5376 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 153:
#line 2255 "language.yy" /* yacc.c:1646  */
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
#line 5401 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 154:
#line 2276 "language.yy" /* yacc.c:1646  */
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
#line 5421 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 155:
#line 2293 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_ATTRVALUE);
    csh.AddCSH_ErrorAfter((yyloc), ("Here should come a shape definition beginning with '{'. Ignoring this malformed shape definition for '"+string((yyvsp[0].str) ? (yyvsp[0].str) : "") +"'.").c_str());
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Here should come a shape definition beginning with '{'. Ignoring this malformed shape definition for '"+string((yyvsp[0].str) ? (yyvsp[0].str) : "") +"'.");
  #endif	
  free((yyvsp[0].str));
}
#line 5435 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 156:
#line 2303 "language.yy" /* yacc.c:1646  */
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
#line 5450 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 157:
#line 2314 "language.yy" /* yacc.c:1646  */
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
#line 5471 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 158:
#line 2331 "language.yy" /* yacc.c:1646  */
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
#line 5490 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 159:
#line 2346 "language.yy" /* yacc.c:1646  */
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
#line 5510 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 160:
#line 2363 "language.yy" /* yacc.c:1646  */
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
#line 5526 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 161:
#line 2375 "language.yy" /* yacc.c:1646  */
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
#line 5543 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 162:
#line 2388 "language.yy" /* yacc.c:1646  */
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
#line 5558 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 163:
#line 2399 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[0]), "Only numbers can come after shape commands.");
  #else
  #endif	
}
#line 5569 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 164:
#line 2407 "language.yy" /* yacc.c:1646  */
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
#line 5589 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 165:
#line 2423 "language.yy" /* yacc.c:1646  */
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
#line 5621 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 166:
#line 2451 "language.yy" /* yacc.c:1646  */
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
#line 5649 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 167:
#line 2475 "language.yy" /* yacc.c:1646  */
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
#line 5679 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 168:
#line 2501 "language.yy" /* yacc.c:1646  */
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
#line 5711 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 169:
#line 2529 "language.yy" /* yacc.c:1646  */
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
#line 5745 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 170:
#line 2559 "language.yy" /* yacc.c:1646  */
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
#line 5781 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 172:
#line 2593 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_COMMA);
    if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_NAME)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
	}
  #endif
}
#line 5795 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 173:
#line 2603 "language.yy" /* yacc.c:1646  */
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
#line 5812 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 176:
#line 2619 "language.yy" /* yacc.c:1646  */
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
#line 5838 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 177:
#line 2641 "language.yy" /* yacc.c:1646  */
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
#line 5865 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 178:
#line 2664 "language.yy" /* yacc.c:1646  */
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
#line 5887 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 179:
#line 2682 "language.yy" /* yacc.c:1646  */
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
#line 5905 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 180:
#line 2699 "language.yy" /* yacc.c:1646  */
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
#line 5933 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 181:
#line 2723 "language.yy" /* yacc.c:1646  */
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
#line 5959 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 182:
#line 2747 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_BRACE);
    csh.PushContext(true);
  #else
    //push empty color & style sets for design definition
    msc.PushContext(true);
  #endif
}
#line 5973 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 184:
#line 2759 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_SEMICOLON);
    if (csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddDesignLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
#endif
}
#line 5987 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 185:
#line 2770 "language.yy" /* yacc.c:1646  */
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
#line 6005 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 186:
#line 2784 "language.yy" /* yacc.c:1646  */
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
#line 6026 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 187:
#line 2801 "language.yy" /* yacc.c:1646  */
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
#line 6044 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 188:
#line 2815 "language.yy" /* yacc.c:1646  */
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
#line 6065 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 191:
#line 2835 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_COMMA);
    if (csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    }   
  #endif
}
#line 6079 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 192:
#line 2845 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_COMMA);
    if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    }   
  #endif
}
#line 6093 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 193:
#line 2855 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[0]), "Extra stuff after design options. Maybe missing a comma?");
  #endif
}
#line 6103 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 194:
#line 2862 "language.yy" /* yacc.c:1646  */
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
#line 6126 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 195:
#line 2881 "language.yy" /* yacc.c:1646  */
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
#line 6149 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 196:
#line 2900 "language.yy" /* yacc.c:1646  */
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
#line 6170 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 197:
#line 2917 "language.yy" /* yacc.c:1646  */
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
#line 6196 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 198:
#line 2939 "language.yy" /* yacc.c:1646  */
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
#line 6222 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 199:
#line 2961 "language.yy" /* yacc.c:1646  */
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
#line 6243 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 200:
#line 2978 "language.yy" /* yacc.c:1646  */
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
#line 6264 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 201:
#line 2997 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[0].arclist))
        (yyval.arcparallel) = new ArcParallel(&msc, (yyvsp[0].arclist));
    else
        (yyval.arcparallel) = NULL;
  #endif
}
#line 6277 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 202:
#line 3006 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[0].arclist)==NULL)
        (yyval.arcparallel) = (yyvsp[-1].arcparallel);
    else if ((yyvsp[-1].arcparallel))
        (yyval.arcparallel) = ((yyvsp[-1].arcparallel))->AddArcList((yyvsp[0].arclist));
    else
        (yyval.arcparallel) = new ArcParallel(&msc, (yyvsp[0].arclist));
  #endif
}
#line 6292 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 203:
#line 3023 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    ((yyvsp[0].arcbox))->SetLineEnd(MSC_POS((yyloc)));
    (yyval.arcboxseries) = new ArcBoxSeries((yyvsp[0].arcbox));
  #endif
}
#line 6305 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 204:
#line 3032 "language.yy" /* yacc.c:1646  */
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
#line 6327 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 205:
#line 3050 "language.yy" /* yacc.c:1646  */
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
#line 6346 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 206:
#line 3066 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[0].arcbox))->SetLineEnd(MSC_POS((yylsp[0])));
    (yyval.arcboxseries) = ((yyvsp[-1].arcboxseries))->AddBox((yyvsp[0].arcbox));
    ((yyvsp[0].arcbox))->AddAttributeList(NULL); //should come after AddBox
  #endif
}
#line 6358 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 207:
#line 3074 "language.yy" /* yacc.c:1646  */
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
#line 6378 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 208:
#line 3090 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    ((yyvsp[-1].arcbox))->AddArcList((yyvsp[0].arclist))->SetLineEnd(MSC_POS((yylsp[-1])));
    (yyval.arcboxseries) = ((yyvsp[-2].arcboxseries))->AddBox((yyvsp[-1].arcbox));
    ((yyvsp[-1].arcbox))->AddAttributeList(NULL); //should come after AddBox
  #endif
}
#line 6392 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 209:
#line 3100 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    if ((yyvsp[-1].arcboxseries)) {
        ArcBox *temp = new ArcBox(MSC_BOX_UNDETERMINED_FOLLOW, NULL, MSC_POS((yylsp[-1])), NULL, MSC_POS((yylsp[-1])), &msc);
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
#line 6415 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 210:
#line 3119 "language.yy" /* yacc.c:1646  */
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
#line 6434 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 211:
#line 3134 "language.yy" /* yacc.c:1646  */
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
        ArcBox *temp = new ArcBox(MSC_BOX_UNDETERMINED_FOLLOW, NULL, MSC_POS((yylsp[-2])), NULL, MSC_POS((yylsp[-2])), &msc);
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
#line 6464 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 212:
#line 3162 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[0].arcbox))->AddAttributeList(NULL);
    ((yyvsp[0].arcbox))->SetLineEnd(MSC_POS((yyloc)), false);
    (yyval.arcbox) = (yyvsp[0].arcbox);
  #endif
}
#line 6476 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 213:
#line 3170 "language.yy" /* yacc.c:1646  */
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
#line 6493 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 214:
#line 3183 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[-1].arcbox))->AddAttributeList(NULL);
    ((yyvsp[-1].arcbox))->SetLineEnd(MSC_POS((yylsp[-1])), false);
    (yyval.arcbox) = ((yyvsp[-1].arcbox))->AddArcList((yyvsp[0].arclist));
  #endif
}
#line 6505 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 215:
#line 3191 "language.yy" /* yacc.c:1646  */
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
#line 6522 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 216:
#line 3206 "language.yy" /* yacc.c:1646  */
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
#line 6544 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 217:
#line 3224 "language.yy" /* yacc.c:1646  */
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
#line 6567 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 218:
#line 3243 "language.yy" /* yacc.c:1646  */
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
#line 6586 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 219:
#line 3258 "language.yy" /* yacc.c:1646  */
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
#line 6611 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 220:
#line 3280 "language.yy" /* yacc.c:1646  */
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
#line 6626 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 221:
#line 3291 "language.yy" /* yacc.c:1646  */
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
#line 6646 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 222:
#line 3307 "language.yy" /* yacc.c:1646  */
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
#line 6671 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 224:
#line 3330 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintBetween((yylsp[-1]), (yylsp[0]));
  #else
    (yyval.arcpipeseries) = ((yyvsp[-1].arcpipeseries))->AddArcList((yyvsp[0].arclist));
  #endif
}
#line 6683 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 227:
#line 3341 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[-2]));
    csh.AddCSH_EntityName((yylsp[-2]), (yyvsp[-2].str));
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcbox) = new ArcBox((yyvsp[-1].arctype), (yyvsp[-2].str), MSC_POS((yylsp[-2])), (yyvsp[0].str), MSC_POS((yylsp[0])), &msc);
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 6701 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 228:
#line 3355 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcbox) = new ArcBox((yyvsp[-1].arctype), NULL, MSC_POS((yylsp[-1])), (yyvsp[0].str), MSC_POS((yylsp[0])), &msc);
  #endif
    free((yyvsp[0].str));
}
#line 6716 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 229:
#line 3366 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[-1]));
    csh.AddCSH_EntityName((yylsp[-1]), (yyvsp[-1].str));
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcbox) = new ArcBox((yyvsp[0].arctype), (yyvsp[-1].str), MSC_POS((yylsp[-1])), NULL, MSC_POS((yylsp[0])), &msc);
  #endif
    free((yyvsp[-1].str));
}
#line 6732 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 230:
#line 3378 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcbox) = new ArcBox((yyvsp[0].arctype), NULL, MSC_POS((yylsp[0])), NULL, MSC_POS((yylsp[0])), &msc);
  #endif
}
#line 6745 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 231:
#line 3388 "language.yy" /* yacc.c:1646  */
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
#line 6767 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 232:
#line 3406 "language.yy" /* yacc.c:1646  */
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
#line 6791 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 233:
#line 3426 "language.yy" /* yacc.c:1646  */
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
#line 6815 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 234:
#line 3446 "language.yy" /* yacc.c:1646  */
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
#line 6840 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 235:
#line 3467 "language.yy" /* yacc.c:1646  */
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
#line 6863 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 236:
#line 3486 "language.yy" /* yacc.c:1646  */
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
#line 6888 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 237:
#line 3507 "language.yy" /* yacc.c:1646  */
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
    switch ((yyvsp[0].arctype)) {
    default:
        _ASSERT(0);
    case MSC_BOX_SOLID:
        (yyval.vertxpos) = new VertXPos(msc, (yyvsp[-1].str), MSC_POS((yylsp[-1])), VertXPos::POS_LEFT_BY);
        break;
    case MSC_BOX_DASHED:
        (yyval.vertxpos) = new VertXPos(msc, (yyvsp[-1].str), MSC_POS((yylsp[-1])), VertXPos::POS_RIGHT_BY);
        break;
    case MSC_BOX_DOTTED:
        msc.Error.Error(MSC_POS((yylsp[0])).start,
                        "unexpected '..', expected '-', '--', '+' or '++'."
                        " Ignoring vertical."); 
        (yyval.vertxpos) = NULL;
        break;
    case MSC_BOX_DOUBLE:
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
#line 6932 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 238:
#line 3547 "language.yy" /* yacc.c:1646  */
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
#line 6958 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 239:
#line 3569 "language.yy" /* yacc.c:1646  */
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
#line 6984 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 240:
#line 3591 "language.yy" /* yacc.c:1646  */
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
#line 7012 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 241:
#line 3615 "language.yy" /* yacc.c:1646  */
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
#line 7040 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 242:
#line 3639 "language.yy" /* yacc.c:1646  */
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
#line 7062 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 243:
#line 3659 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
	(yyval.arctypeplusdir) = 0; //dummy to supress warning
  #else
    ((yyval.arctypeplusdir)).arc.type = (yyvsp[0].arctype);
	((yyval.arctypeplusdir)).arc.lost = EArrowLost::NOT;
	((yyval.arctypeplusdir)).dir = MSC_DIR_INDETERMINATE;
  #endif
}
#line 7077 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 244:
#line 3670 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    csh.CheckEntityHintBetween((yylsp[-1]), (yylsp[0]));
	(yyval.arctypeplusdir) = 0; //dummy to supress warning	
  #else
    ((yyval.arctypeplusdir)).arc.type = (yyvsp[0].arctype);
	((yyval.arctypeplusdir)).arc.lost = EArrowLost::AT_SRC;
    ((yyval.arctypeplusdir)).arc.lost_pos.SetFrom(MSC_POS((yylsp[-1])));
	((yyval.arctypeplusdir)).dir = MSC_DIR_INDETERMINATE;
  #endif
}
#line 7094 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 245:
#line 3683 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
	(yyval.arctypeplusdir) = 0; //dummy to supress warning	
  #else
    ((yyval.arctypeplusdir)).arc.type = (yyvsp[-1].arctype);
	((yyval.arctypeplusdir)).arc.lost = EArrowLost::AT_SRC;
    ((yyval.arctypeplusdir)).arc.lost_pos.SetFrom(MSC_POS((yylsp[0])));
	((yyval.arctypeplusdir)).dir = MSC_DIR_INDETERMINATE;
  #endif
}
#line 7110 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 246:
#line 3695 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_SYMBOL);
    csh.AddCSH_Error((yylsp[0]), MULTIPLE_ASTERISK_ERROR_MSG);
    csh.CheckEntityHintBetween((yylsp[-2]), (yylsp[-1]));
    (yyval.arctypeplusdir) = 0; //dummy to supress warning	
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).start, "Only one loss can be specified. Ignoring second asterisk ('*').");
    ((yyval.arctypeplusdir)).arc.type = (yyvsp[-1].arctype);
    ((yyval.arctypeplusdir)).arc.lost = EArrowLost::AT_SRC;
    ((yyval.arctypeplusdir)).arc.lost_pos.SetFrom(MSC_POS((yylsp[-2])));
	((yyval.arctypeplusdir)).dir = MSC_DIR_INDETERMINATE;
  #endif
}
#line 7129 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 247:
#line 3710 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
	(yyval.arctypeplusdir) = 0; //dummy to supress warning
  #else
    ((yyval.arctypeplusdir)).arc = (yyvsp[0].arcsegdata);
	((yyval.arctypeplusdir)).dir = MSC_DIR_LEFT;
  #endif
}
#line 7142 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 248:
#line 3719 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
	(yyval.arctypeplusdir) = 0; //dummy to supress warning
  #else
    ((yyval.arctypeplusdir)).arc = (yyvsp[0].arcsegdata);
	((yyval.arctypeplusdir)).dir = MSC_DIR_RIGHT;
  #endif
}
#line 7155 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 249:
#line 3728 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
	(yyval.arctypeplusdir) = 0; //dummy to supress warning
  #else
    ((yyval.arctypeplusdir)).arc = (yyvsp[0].arcsegdata);
	((yyval.arctypeplusdir)).dir = MSC_DIR_BIDIR;
  #endif
}
#line 7168 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 250:
#line 3738 "language.yy" /* yacc.c:1646  */
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
#line 7185 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 251:
#line 3751 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_MARKERNAME);
    csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), EHintSourceType::MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[-1].arctypeplusdir), MARKER_HERE_STR, (yyvsp[0].str), &msc);
  #endif
    free((yyvsp[0].str));
}
#line 7199 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 252:
#line 3761 "language.yy" /* yacc.c:1646  */
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
#line 7214 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 253:
#line 3772 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[0].arctypeplusdir), MARKER_HERE_STR, MARKER_HERE_STR, &msc);
  #endif
}
#line 7226 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 254:
#line 3780 "language.yy" /* yacc.c:1646  */
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
#line 7242 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 255:
#line 3792 "language.yy" /* yacc.c:1646  */
    {free((yyvsp[-1].str)); (yyval.arcvertarrow)=NULL;}
#line 7248 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 256:
#line 3793 "language.yy" /* yacc.c:1646  */
    {free((yyvsp[-3].str)); free((yyvsp[-1].str)); (yyval.arcvertarrow)=NULL;}
#line 7254 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 257:
#line 3797 "language.yy" /* yacc.c:1646  */
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
#line 7269 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 258:
#line 3808 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    }
#else
	VertXPos vxp(msc);
	(yyval.arcvertarrow) = ((yyvsp[0].arcvertarrow))->AddXpos(&vxp);
  #endif
}
#line 7285 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 259:
#line 3820 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    if ((yyvsp[0].vertxpos)) {
		ArcTypePlusDir typeplusdir;
		typeplusdir.arc.type = MSC_ARC_SOLID;
		typeplusdir.arc.lost = EArrowLost::NOT;
		typeplusdir.dir = MSC_DIR_RIGHT;
		ArcVerticalArrow *ava = new ArcVerticalArrow(typeplusdir, MARKER_HERE_STR, MARKER_HERE_STR, &msc);
		ava->AddXpos((yyvsp[0].vertxpos));
		(yyval.arcvertarrow) = ava;
		delete (yyvsp[0].vertxpos);
	} else
	    (yyval.arcvertarrow) = NULL;
  #endif
}
#line 7306 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 260:
#line 3838 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
  #else
    (yyval.arcbase) = new ArcDivider((yyvsp[0].arctype), &msc);
  #endif
}
#line 7318 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 261:
#line 3846 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = ((yyvsp[-2].arcarrow))->AddLostPos((yyvsp[0].vertxpos), MSC_POS2((yylsp[-1]), (yylsp[0])));
  #endif
   free((yyvsp[-1].str));
}
#line 7331 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 262:
#line 3855 "language.yy" /* yacc.c:1646  */
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
#line 7350 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 263:
#line 3870 "language.yy" /* yacc.c:1646  */
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
#line 7368 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 267:
#line 3887 "language.yy" /* yacc.c:1646  */
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
#line 7385 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 268:
#line 3900 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[-1].arcsegdata), LSIDE_ENT_STR, MSC_POS((yylsp[-1])), (yyvsp[0].str), true, MSC_POS((yylsp[0])));
  #endif
    free((yyvsp[0].str));
}
#line 7399 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 269:
#line 3910 "language.yy" /* yacc.c:1646  */
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
#line 7414 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 270:
#line 3921 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcarrow) = ((yyvsp[-2].arcarrow))->AddSegment((yyvsp[-1].arcsegdata), (yyvsp[0].str), MSC_POS((yylsp[0])), MSC_POS2((yylsp[-1]), (yylsp[0])));
  #endif
    free((yyvsp[0].str));
}
#line 7428 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 271:
#line 3931 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((yylsp[-1]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[-1].arcarrow))->AddSegment((yyvsp[0].arcsegdata), NULL, MSC_POS((yylsp[0])), MSC_POS((yylsp[0])));
  #endif
}
#line 7440 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 272:
#line 3941 "language.yy" /* yacc.c:1646  */
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
#line 7457 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 273:
#line 3954 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[-1].arcsegdata), (yyvsp[0].str), MSC_POS((yylsp[0])), LSIDE_ENT_STR, false, MSC_POS((yylsp[-1])));
  #endif
    free((yyvsp[0].str));
}
#line 7471 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 274:
#line 3964 "language.yy" /* yacc.c:1646  */
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
#line 7486 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 275:
#line 3975 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcarrow) = ((yyvsp[-2].arcarrow))->AddSegment((yyvsp[-1].arcsegdata), (yyvsp[0].str), MSC_POS((yylsp[0])), MSC_POS2((yylsp[-1]), (yylsp[0])));
  #endif
    free((yyvsp[0].str));
}
#line 7500 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 276:
#line 3985 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[-1].arcarrow))->AddSegment((yyvsp[0].arcsegdata), NULL, MSC_POS((yylsp[0])), MSC_POS((yylsp[0])));
  #endif
}
#line 7512 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 277:
#line 3994 "language.yy" /* yacc.c:1646  */
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
#line 7529 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 278:
#line 4007 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[-1].arcsegdata), LSIDE_ENT_STR, MSC_POS((yylsp[-1])), (yyvsp[0].str), true, MSC_POS((yylsp[0])));
  #endif
    free((yyvsp[0].str));
}
#line 7543 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 279:
#line 4017 "language.yy" /* yacc.c:1646  */
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
#line 7558 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 280:
#line 4028 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcarrow) = ((yyvsp[-2].arcarrow))->AddSegment((yyvsp[-1].arcsegdata), (yyvsp[0].str), MSC_POS((yylsp[0])), MSC_POS2((yylsp[-1]), (yylsp[0])));
  #endif
    free((yyvsp[0].str));
}
#line 7572 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 281:
#line 4038 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[-1].arcarrow))->AddSegment((yyvsp[0].arcsegdata), NULL, MSC_POS((yylsp[0])), MSC_POS((yylsp[0])));
  #endif
}
#line 7584 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 295:
#line 4050 "language.yy" /* yacc.c:1646  */
    {(yyval.arctype)=MSC_ARC_UNDETERMINED_SEGMENT;}
#line 7590 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 297:
#line 4051 "language.yy" /* yacc.c:1646  */
    {(yyval.arctype)=MSC_ARC_UNDETERMINED_SEGMENT;}
#line 7596 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 299:
#line 4052 "language.yy" /* yacc.c:1646  */
    {(yyval.arctype)=MSC_ARC_UNDETERMINED_SEGMENT;}
#line 7602 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 300:
#line 4055 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 0;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
#line 7616 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 301:
#line 4065 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-1])));
  #endif
}
#line 7631 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 302:
#line 4076 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[-1].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[0])));
  #endif
}
#line 7646 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 303:
#line 4087 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]) + (yylsp[-1]), COLOR_SYMBOL);
    csh.AddCSH_Error((yylsp[0]), MULTIPLE_ASTERISK_ERROR_MSG);
    csh.asteriskNo = 2;
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).start, "One arrow may be lost only once. Ignoring subsequent asterisks ('*').");
    ((yyval.arcsegdata)).type = (yyvsp[-1].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-2])));
  #endif
}
#line 7663 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 304:
#line 4101 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
#line 7677 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 305:
#line 4111 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-1])));
  #endif
}
#line 7692 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 306:
#line 4122 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[-1].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[0])));
  #endif
}
#line 7707 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 307:
#line 4133 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]) + (yylsp[-1]), COLOR_SYMBOL);
    csh.AddCSH_Error((yylsp[0]), MULTIPLE_ASTERISK_ERROR_MSG);
    csh.asteriskNo = 2;
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).start, "One arrow may be lost only once. Ignoring subsequent asterisks ('*').");
    ((yyval.arcsegdata)).type = (yyvsp[-1].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-2])));
  #endif
}
#line 7724 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 308:
#line 4147 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 0;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
#line 7738 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 309:
#line 4157 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-1])));
  #endif
}
#line 7753 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 310:
#line 4168 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[-1].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[0])));
  #endif
}
#line 7768 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 311:
#line 4179 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]) + (yylsp[-1]), COLOR_SYMBOL);
    csh.AddCSH_Error((yylsp[0]), MULTIPLE_ASTERISK_ERROR_MSG);
    csh.asteriskNo = 2;
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).start, "One arrow may be lost only once. Ignoring subsequent asterisks ('*').");
    ((yyval.arcsegdata)).type = (yyvsp[-1].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-2])));
  #endif
}
#line 7785 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 312:
#line 4193 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
#line 7798 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 313:
#line 4202 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[-1]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-1])));
  #endif
}
#line 7816 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 314:
#line 4216 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[0]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[-1].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[0])));
  #endif
}
#line 7834 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 315:
#line 4230 "language.yy" /* yacc.c:1646  */
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
    ((yyval.arcsegdata)).type = (yyvsp[-1].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-2])));
  #endif
}
#line 7854 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 316:
#line 4247 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
#line 7867 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 317:
#line 4256 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[-1]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-1])));
  #endif
}
#line 7885 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 318:
#line 4270 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[0]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[-1].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[0])));
  #endif
}
#line 7903 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 319:
#line 4284 "language.yy" /* yacc.c:1646  */
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
    ((yyval.arcsegdata)).type = (yyvsp[-1].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-2])));
  #endif
}
#line 7923 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 320:
#line 4301 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
#line 7936 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 321:
#line 4310 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[-1]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-1])));
  #endif
}
#line 7954 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 322:
#line 4324 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[0]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[-1].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[0])));
  #endif
}
#line 7972 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 323:
#line 4338 "language.yy" /* yacc.c:1646  */
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
    ((yyval.arcsegdata)).type = (yyvsp[-1].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-2])));
  #endif
}
#line 7992 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 325:
#line 4358 "language.yy" /* yacc.c:1646  */
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
#line 8009 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 326:
#line 4372 "language.yy" /* yacc.c:1646  */
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
#line 8029 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 327:
#line 4388 "language.yy" /* yacc.c:1646  */
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
#line 8049 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 331:
#line 4407 "language.yy" /* yacc.c:1646  */
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
#line 8079 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 332:
#line 4433 "language.yy" /* yacc.c:1646  */
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
#line 8112 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 333:
#line 4463 "language.yy" /* yacc.c:1646  */
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
#line 8152 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 334:
#line 4499 "language.yy" /* yacc.c:1646  */
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
#line 8180 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 335:
#line 4523 "language.yy" /* yacc.c:1646  */
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
#line 8211 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 336:
#line 4550 "language.yy" /* yacc.c:1646  */
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
#line 8238 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 337:
#line 4573 "language.yy" /* yacc.c:1646  */
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
#line 8266 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 338:
#line 4597 "language.yy" /* yacc.c:1646  */
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
#line 8291 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 339:
#line 4618 "language.yy" /* yacc.c:1646  */
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
#line 8312 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 340:
#line 4636 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[0].arcbase))
        ((yyvsp[0].arcbase))->AddAttributeList(NULL);
    (yyval.arcbase) = ((yyvsp[0].arcbase));
  #endif
}
#line 8324 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 341:
#line 4644 "language.yy" /* yacc.c:1646  */
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
#line 8341 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 342:
#line 4658 "language.yy" /* yacc.c:1646  */
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
#line 8368 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 343:
#line 4681 "language.yy" /* yacc.c:1646  */
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
#line 8391 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 344:
#line 4700 "language.yy" /* yacc.c:1646  */
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
#line 8415 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 345:
#line 4720 "language.yy" /* yacc.c:1646  */
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
#line 8442 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 346:
#line 4744 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.eside)= ESide::LEFT;
  #endif
    free((yyvsp[0].str));
}
#line 8453 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 347:
#line 4751 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.eside)= ESide::END;
  #endif
    free((yyvsp[0].str));
}
#line 8464 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 348:
#line 4758 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.eside)= ESide::END;
  #endif
    free((yyvsp[0].str));
}
#line 8475 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 349:
#line 4766 "language.yy" /* yacc.c:1646  */
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
#line 8495 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 350:
#line 4782 "language.yy" /* yacc.c:1646  */
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
#line 8513 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 351:
#line 4797 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ColonString_CheckAndAddEscapeHint((yylsp[0]), (yyvsp[0].str), false);
    csh.AddColonLabel((yylsp[0]));
#endif
    (yyval.str) = (yyvsp[0].str);
}
#line 8525 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 352:
#line 4805 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ColonString_CheckAndAddEscapeHint((yylsp[0]), (yyvsp[0].str), true);
	csh.AddColonLabel((yylsp[0]));
  #endif
    (yyval.str) = (yyvsp[0].str);
}
#line 8537 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 353:
#line 4814 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = (new AttributeList)->Append(new Attribute("label", (yyvsp[0].str), MSC_POS((yyloc)), MSC_POS((yyloc)).IncStartCol()));
  #endif
    free((yyvsp[0].str));
}
#line 8549 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 354:
#line 4822 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = ((yyvsp[0].attriblist))->Prepend(new Attribute("label", (yyvsp[-1].str), MSC_POS((yylsp[-1])), MSC_POS((yylsp[-1])).IncStartCol()));
  #endif
    free((yyvsp[-1].str));
}
#line 8561 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 355:
#line 4830 "language.yy" /* yacc.c:1646  */
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
#line 8577 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 356:
#line 4842 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = ((yyvsp[-1].attriblist))->Append(new Attribute("label", (yyvsp[0].str), MSC_POS((yylsp[0])), MSC_POS((yylsp[0])).IncStartCol()));
  #endif
    free((yyvsp[0].str));
}
#line 8589 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 358:
#line 4853 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[-1]), COLOR_BRACKET);
        csh.AddCSH((yylsp[0]), COLOR_BRACKET);
        csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_NAME);
  #else
    (yyval.attriblist) = new AttributeList;
  #endif
}
#line 8603 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 359:
#line 4863 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[-2]), COLOR_BRACKET);
        csh.AddCSH((yylsp[0]), COLOR_BRACKET);
        csh.CheckHintBetween((yylsp[-2]), (yylsp[-1]), EHintSourceType::ATTR_NAME);
  #else
    (yyval.attriblist) = (yyvsp[-1].attriblist);
  #endif
}
#line 8617 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 360:
#line 4873 "language.yy" /* yacc.c:1646  */
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
#line 8632 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 361:
#line 4884 "language.yy" /* yacc.c:1646  */
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
#line 8648 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 362:
#line 4896 "language.yy" /* yacc.c:1646  */
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
#line 8663 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 363:
#line 4907 "language.yy" /* yacc.c:1646  */
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
#line 8678 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 364:
#line 4918 "language.yy" /* yacc.c:1646  */
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
#line 8693 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 365:
#line 4931 "language.yy" /* yacc.c:1646  */
    {
    (yyval.attriblist) = NULL;
}
#line 8701 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 366:
#line 4935 "language.yy" /* yacc.c:1646  */
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
#line 8716 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 367:
#line 4947 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.attriblist) = (new AttributeList)->Append((yyvsp[0].attrib));
  #endif
}
#line 8727 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 368:
#line 4954 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_COMMA);
    csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_NAME);
  #else
    (yyval.attriblist) = ((yyvsp[-2].attriblist))->Append((yyvsp[0].attrib));
  #endif
}
#line 8740 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 369:
#line 4963 "language.yy" /* yacc.c:1646  */
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
#line 8755 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 371:
#line 4976 "language.yy" /* yacc.c:1646  */
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
#line 8774 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 372:
#line 4991 "language.yy" /* yacc.c:1646  */
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
#line 8793 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 373:
#line 5006 "language.yy" /* yacc.c:1646  */
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
#line 8812 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 374:
#line 5021 "language.yy" /* yacc.c:1646  */
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
#line 8830 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 375:
#line 5035 "language.yy" /* yacc.c:1646  */
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
#line 8846 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 376:
#line 5049 "language.yy" /* yacc.c:1646  */
    {
    (yyval.attrib) = NULL;
    free((yyvsp[-2].str));
}
#line 8855 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 377:
#line 5054 "language.yy" /* yacc.c:1646  */
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
#line 8870 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 378:
#line 5066 "language.yy" /* yacc.c:1646  */
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
#line 8888 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 379:
#line 5080 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
	(yyval.vshape) = ArcVerticalArrow::BOX;
  #endif
  free((yyvsp[0].str));
}
#line 8899 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 380:
#line 5090 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
	csh.AddQuotedString((yylsp[0]));
  #endif
  (yyval.str) = (yyvsp[0].str);
}
#line 8910 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 382:
#line 5098 "language.yy" /* yacc.c:1646  */
    {
	(yyval.str) = (char*)malloc(2);
	((yyval.str))[0] = ShapeElement::act_code[(yyvsp[0].shapecommand)];
	((yyval.str))[1] = 0;
}
#line 8920 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 416:
#line 5114 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("->");}
#line 8926 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 417:
#line 5115 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("<-");}
#line 8932 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 418:
#line 5116 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("<->");}
#line 8938 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 419:
#line 5117 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("=>");}
#line 8944 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 420:
#line 5118 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("<=");}
#line 8950 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 421:
#line 5119 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("<=>");}
#line 8956 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 422:
#line 5120 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup(">>");}
#line 8962 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 423:
#line 5121 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("<<");}
#line 8968 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 424:
#line 5122 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("<<>>");}
#line 8974 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 425:
#line 5123 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup(">");}
#line 8980 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 426:
#line 5124 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("<");}
#line 8986 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 427:
#line 5125 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("<>");}
#line 8992 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 428:
#line 5127 "language.yy" /* yacc.c:1646  */
    {
    switch ((yyvsp[0].arctype)) {
    case MSC_ARC_DIVIDER:  (yyval.str) = strdup("---"); break;
    case MSC_ARC_DISCO:    (yyval.str) = strdup("..."); break;
    default: _ASSERT(0);
    }
}
#line 9004 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 429:
#line 5135 "language.yy" /* yacc.c:1646  */
    {
    switch ((yyvsp[0].arctype)) {
    case MSC_BOX_SOLID:  (yyval.str) = strdup("--"); break;
    case MSC_BOX_DASHED: (yyval.str) = strdup("++"); break; //will likely not happen due to special handling in TOK_COLORDEF
    case MSC_BOX_DOTTED: (yyval.str) = strdup(".."); break;
    case MSC_BOX_DOUBLE: (yyval.str) = strdup("=="); break;
    default: _ASSERT(0);
    }
}
#line 9018 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 435:
#line 5150 "language.yy" /* yacc.c:1646  */
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
#line 9035 "colorsyntax.cc" /* yacc.c:1646  */
    break;

  case 437:
#line 5167 "language.yy" /* yacc.c:1646  */
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
#line 9052 "colorsyntax.cc" /* yacc.c:1646  */
    break;


#line 9056 "colorsyntax.cc" /* yacc.c:1646  */
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
#line 5181 "language.yy" /* yacc.c:1906  */



/* END OF FILE */
