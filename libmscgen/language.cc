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
    TOK_REL_TO = 277,
    TOK_REL_FROM = 278,
    TOK_REL_BIDIR = 279,
    TOK_REL_X = 280,
    TOK_SPECIAL_ARC = 281,
    TOK_EMPH = 282,
    TOK_EMPH_PLUS_PLUS = 283,
    TOK_COMMAND_HEADING = 284,
    TOK_COMMAND_NUDGE = 285,
    TOK_COMMAND_NEWPAGE = 286,
    TOK_COMMAND_DEFSHAPE = 287,
    TOK_COMMAND_DEFCOLOR = 288,
    TOK_COMMAND_DEFSTYLE = 289,
    TOK_COMMAND_DEFDESIGN = 290,
    TOK_COMMAND_BIG = 291,
    TOK_COMMAND_BOX = 292,
    TOK_COMMAND_PIPE = 293,
    TOK_COMMAND_MARK = 294,
    TOK_COMMAND_PARALLEL = 295,
    TOK_COMMAND_OVERLAP = 296,
    TOK_VERTICAL = 297,
    TOK_VERTICAL_SHAPE = 298,
    TOK_AT = 299,
    TOK_LOST = 300,
    TOK_AT_POS = 301,
    TOK_SHOW = 302,
    TOK_HIDE = 303,
    TOK_ACTIVATE = 304,
    TOK_DEACTIVATE = 305,
    TOK_BYE = 306,
    TOK_COMMAND_VSPACE = 307,
    TOK_COMMAND_HSPACE = 308,
    TOK_COMMAND_SYMBOL = 309,
    TOK_COMMAND_NOTE = 310,
    TOK_COMMAND_COMMENT = 311,
    TOK_COMMAND_ENDNOTE = 312,
    TOK_COMMAND_FOOTNOTE = 313,
    TOK_COMMAND_TITLE = 314,
    TOK_COMMAND_SUBTITLE = 315,
    TOK_COMMAND_TEXT = 316,
    TOK_SHAPE_COMMAND = 317,
    TOK_MSCGEN_RBOX = 318,
    TOK_MSCGEN_ABOX = 319,
    TOK__NEVER__HAPPENS = 320
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
#define TOK_SPECIAL_ARC 281
#define TOK_EMPH 282
#define TOK_EMPH_PLUS_PLUS 283
#define TOK_COMMAND_HEADING 284
#define TOK_COMMAND_NUDGE 285
#define TOK_COMMAND_NEWPAGE 286
#define TOK_COMMAND_DEFSHAPE 287
#define TOK_COMMAND_DEFCOLOR 288
#define TOK_COMMAND_DEFSTYLE 289
#define TOK_COMMAND_DEFDESIGN 290
#define TOK_COMMAND_BIG 291
#define TOK_COMMAND_BOX 292
#define TOK_COMMAND_PIPE 293
#define TOK_COMMAND_MARK 294
#define TOK_COMMAND_PARALLEL 295
#define TOK_COMMAND_OVERLAP 296
#define TOK_VERTICAL 297
#define TOK_VERTICAL_SHAPE 298
#define TOK_AT 299
#define TOK_LOST 300
#define TOK_AT_POS 301
#define TOK_SHOW 302
#define TOK_HIDE 303
#define TOK_ACTIVATE 304
#define TOK_DEACTIVATE 305
#define TOK_BYE 306
#define TOK_COMMAND_VSPACE 307
#define TOK_COMMAND_HSPACE 308
#define TOK_COMMAND_SYMBOL 309
#define TOK_COMMAND_NOTE 310
#define TOK_COMMAND_COMMENT 311
#define TOK_COMMAND_ENDNOTE 312
#define TOK_COMMAND_FOOTNOTE 313
#define TOK_COMMAND_TITLE 314
#define TOK_COMMAND_SUBTITLE 315
#define TOK_COMMAND_TEXT 316
#define TOK_SHAPE_COMMAND 317
#define TOK_MSCGEN_RBOX 318
#define TOK_MSCGEN_ABOX 319
#define TOK__NEVER__HAPPENS 320

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 92 "language.yy" /* yacc.c:355  */

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

#line 324 "language.cc" /* yacc.c:355  */
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
#line 173 "language.yy" /* yacc.c:358  */

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


#line 413 "language.cc" /* yacc.c:358  */

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
#define YYFINAL  209
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1772

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  66
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  89
/* YYNRULES -- Number of rules.  */
#define YYNRULES  437
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  481

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   320

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
      65
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   237,   237,   242,   243,   252,   264,   274,   292,   307,
     319,   337,   338,   347,   361,   373,   386,   405,   413,   424,
     435,   446,   457,   467,   479,   493,   494,   505,   520,   529,
     537,   551,   552,   553,   565,   581,   581,   583,   592,   616,
     634,   643,   656,   673,   693,   716,   735,   756,   779,   810,
     834,   859,   893,   911,   923,   938,   958,   973,   981,   997,
    1013,  1026,  1039,  1059,  1078,  1089,  1096,  1103,  1112,  1125,
    1139,  1155,  1173,  1189,  1207,  1220,  1235,  1249,  1266,  1280,
    1297,  1317,  1332,  1350,  1370,  1387,  1404,  1419,  1436,  1456,
    1473,  1487,  1504,  1505,  1506,  1507,  1524,  1546,  1565,  1587,
    1604,  1627,  1627,  1629,  1630,  1656,  1678,  1695,  1696,  1707,
    1718,  1718,  1720,  1732,  1743,  1752,  1766,  1778,  1790,  1801,
    1811,  1826,  1826,  1826,  1826,  1828,  1838,  1857,  1870,  1871,
    1886,  1905,  1924,  1943,  1965,  1984,  2006,  2026,  2032,  2043,
    2055,  2058,  2074,  2086,  2102,  2115,  2133,  2148,  2166,  2182,
    2183,  2193,  2206,  2254,  2265,  2285,  2300,  2315,  2336,  2353,
    2363,  2374,  2391,  2406,  2423,  2435,  2448,  2459,  2467,  2483,
    2511,  2535,  2561,  2589,  2619,  2652,  2653,  2663,  2677,  2677,
    2679,  2701,  2724,  2742,  2759,  2783,  2807,  2818,  2819,  2830,
    2844,  2861,  2875,  2892,  2894,  2895,  2905,  2915,  2922,  2941,
    2960,  2977,  2999,  3021,  3038,  3057,  3066,  3082,  3093,  3110,
    3132,  3141,  3159,  3175,  3183,  3199,  3209,  3228,  3243,  3268,
    3279,  3297,  3305,  3318,  3326,  3341,  3359,  3378,  3393,  3415,
    3426,  3442,  3464,  3465,  3474,  3474,  3476,  3490,  3501,  3513,
    3523,  3523,  3523,  3523,  3525,  3552,  3570,  3590,  3610,  3631,
    3650,  3671,  3711,  3733,  3755,  3779,  3803,  3823,  3834,  3847,
    3859,  3874,  3883,  3892,  3902,  3915,  3925,  3936,  3944,  3957,
    3958,  3961,  3972,  3987,  4005,  4013,  4022,  4037,  4052,  4052,
    4052,  4053,  4096,  4143,  4165,  4188,  4201,  4211,  4222,  4232,
    4242,  4255,  4265,  4276,  4286,  4295,  4308,  4318,  4329,  4339,
    4348,  4348,  4349,  4349,  4350,  4350,  4352,  4362,  4373,  4384,
    4398,  4408,  4419,  4430,  4444,  4454,  4465,  4476,  4490,  4499,
    4513,  4527,  4544,  4553,  4567,  4581,  4598,  4607,  4621,  4635,
    4654,  4655,  4669,  4685,  4702,  4702,  4702,  4704,  4730,  4760,
    4796,  4820,  4847,  4870,  4894,  4915,  4933,  4941,  4955,  4978,
    4997,  5017,  5041,  5048,  5055,  5063,  5079,  5094,  5102,  5111,
    5119,  5127,  5139,  5147,  5150,  5160,  5170,  5181,  5193,  5204,
    5215,  5228,  5232,  5244,  5251,  5260,  5271,  5273,  5288,  5303,
    5318,  5332,  5346,  5351,  5363,  5377,  5387,  5394,  5395,  5396,
    5403,  5404,  5404,  5404,  5405,  5405,  5405,  5405,  5406,  5406,
    5406,  5406,  5406,  5406,  5407,  5407,  5407,  5407,  5407,  5408,
    5408,  5408,  5408,  5408,  5409,  5409,  5409,  5409,  5410,  5410,
    5410,  5411,  5411,  5412,  5412,  5414,  5424,  5435,  5445,  5453,
    5464,  5464,  5466,  5466,  5466,  5468,  5483,  5485
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
  "TOK_COLORDEF", "TOK_REL_TO", "TOK_REL_FROM", "TOK_REL_BIDIR",
  "TOK_REL_X", "TOK_SPECIAL_ARC", "TOK_EMPH", "TOK_EMPH_PLUS_PLUS",
  "TOK_COMMAND_HEADING", "TOK_COMMAND_NUDGE", "TOK_COMMAND_NEWPAGE",
  "TOK_COMMAND_DEFSHAPE", "TOK_COMMAND_DEFCOLOR", "TOK_COMMAND_DEFSTYLE",
  "TOK_COMMAND_DEFDESIGN", "TOK_COMMAND_BIG", "TOK_COMMAND_BOX",
  "TOK_COMMAND_PIPE", "TOK_COMMAND_MARK", "TOK_COMMAND_PARALLEL",
  "TOK_COMMAND_OVERLAP", "TOK_VERTICAL", "TOK_VERTICAL_SHAPE", "TOK_AT",
  "TOK_LOST", "TOK_AT_POS", "TOK_SHOW", "TOK_HIDE", "TOK_ACTIVATE",
  "TOK_DEACTIVATE", "TOK_BYE", "TOK_COMMAND_VSPACE", "TOK_COMMAND_HSPACE",
  "TOK_COMMAND_SYMBOL", "TOK_COMMAND_NOTE", "TOK_COMMAND_COMMENT",
  "TOK_COMMAND_ENDNOTE", "TOK_COMMAND_FOOTNOTE", "TOK_COMMAND_TITLE",
  "TOK_COMMAND_SUBTITLE", "TOK_COMMAND_TEXT", "TOK_SHAPE_COMMAND",
  "TOK_MSCGEN_RBOX", "TOK_MSCGEN_ABOX", "TOK__NEVER__HAPPENS", "$accept",
  "msc_with_bye", "eof", "msc", "top_level_arclist", "msckey",
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
  "pipe_list_no_content", "pipe_list", "emphrel", "boxrel",
  "mscgen_emphrel", "mscgen_boxrel", "vertxpos", "empharcrel_straight",
  "vertrel_no_xpos", "vertrel", "arcrel", "arcrel_arrow", "arcrel_to",
  "arcrel_from", "arcrel_bidir", "relation_to_cont_no_loss",
  "relation_from_cont_no_loss", "relation_bidir_cont_no_loss",
  "relation_to", "relation_from", "relation_bidir", "relation_to_cont",
  "relation_from_cont", "relation_bidir_cont", "extvertxpos",
  "extvertxpos_no_string", "symbol_type_string", "symbol_command_no_attr",
  "symbol_command", "note", "comment_command", "comment", "colon_string",
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
     315,   316,   317,   318,   319,   320
};
# endif

#define YYPACT_NINF -322

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-322)))

#define YYTABLE_NINF -371

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    1407,  -322,  -322,  -322,   -12,   474,    51,   341,  -322,  -322,
      76,   104,   107,  -322,  -322,  -322,  -322,    86,    86,    86,
     301,  1345,   849,   108,  1648,  1508,   307,   911,  -322,  -322,
    1613,  -322,  -322,  -322,  -322,   113,  1640,  1657,    95,  -322,
    -322,  -322,  -322,  -322,   103,  -322,   146,     5,    32,   151,
    -322,   165,  1467,  -322,  1527,   129,  -322,   139,   301,   179,
    -322,   181,    17,   265,  -322,  -322,   208,  -322,   301,   248,
     139,   139,   114,   188,   128,    11,   301,   301,   301,   139,
    -322,  -322,   139,  -322,    86,   331,  1708,   540,  -322,   177,
    -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,
    -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,
    -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,
    -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,  -322,
    -322,  -322,  -322,   417,  -322,  -322,  -322,  -322,   201,   211,
     209,   216,   221,     5,   973,  1283,  -322,  -322,  -322,  -322,
    -322,  -322,  -322,   227,   232,  -322,   234,  -322,   235,  -322,
      36,  -322,  -322,   236,  -322,   139,   378,  -322,   202,  -322,
     139,  -322,    86,   443,  -322,  -322,   301,   230,  -322,   301,
     219,   139,  -322,  -322,  -322,  1587,   435,   139,  -322,    86,
    -322,    84,  -322,  -322,   196,  -322,   113,  -322,   301,  -322,
      13,  -322,  1593,  -322,  -322,   663,  -322,   139,  -322,  -322,
    -322,   255,  -322,     5,  -322,  -322,  1467,   166,  -322,   300,
    -322,   262,   215,  -322,   274,   248,   372,   301,  -322,   151,
    -322,   248,   151,  -322,   139,  -322,  -322,   151,  -322,  -322,
     219,  -322,    75,  -322,   271,   301,  -322,    63,  -322,   273,
     301,  -322,    35,  -322,   280,   301,  -322,  -322,  -322,  -322,
    -322,  -322,  -322,    86,   278,  1035,   292,  -322,  -322,  -322,
    -322,  -322,   301,   301,   301,   301,   301,   151,  -322,  -322,
    -322,    38,  -322,  -322,   291,   600,  -322,   725,   247,  -322,
    -322,  -322,  -322,   306,  -322,    48,  1345,   787,   849,  1097,
    -322,  -322,   337,  -322,  -322,  -322,   312,   385,  -322,   266,
    -322,  -322,   139,  -322,   301,  -322,  -322,    86,  -322,  -322,
    -322,   301,   219,   301,   203,   203,    13,  -322,   139,  -322,
    -322,  -322,  -322,  1283,   279,  -322,   301,  -322,  -322,  -322,
     323,   330,  -322,   248,  -322,  -322,   151,  -322,  -322,  -322,
    -322,   339,  -322,  -322,   340,  -322,  -322,   344,  -322,  -322,
    -322,   301,  -322,  -322,   301,  -322,  -322,  -322,  -322,  -322,
    -322,  -322,    21,  -322,  -322,  -322,  -322,  -322,  -322,  -322,
    1159,  -322,  -322,  -322,   284,   342,   124,   357,  -322,  1159,
    -322,  -322,  -322,  -322,   184,  1345,   849,   358,  -322,   147,
    -322,   367,  -322,  -322,   253,   373,   301,  -322,  -322,  -322,
     314,  -322,  -322,  -322,  -322,   203,  -322,  -322,  -322,   301,
    -322,  -322,  -322,   330,    39,  -322,   151,  -322,  -322,  -322,
    -322,  -322,  -322,  -322,  -322,  -322,   379,   377,   374,  -322,
     384,  -322,  -322,  1283,  1283,   232,   235,   180,  -322,   157,
    1221,  -322,   393,  -322,   400,  -322,   203,  -322,  -322,  -322,
    -322,  -322,   402,  -322,  -322,  -322,  -322,   395,  -322,  -322,
    -322,  -322,  -322,  -322,  -322,  -322,   404,  -322,   405,   406,
    -322
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       6,   387,   386,    33,   435,     0,     0,     0,   358,   357,
     306,   310,   314,   388,   274,   235,   234,    76,    78,    90,
      69,    71,    73,    75,    42,   211,     0,    86,    35,    36,
      45,   122,   121,   123,   124,   100,    97,   345,     0,   352,
     353,   354,   101,   102,    82,   389,     0,     0,     0,     0,
     205,    11,    25,    28,    39,     0,    37,    81,    58,    64,
     125,    57,    67,    65,   210,   229,   232,    66,   239,   221,
     219,    40,   277,   278,   279,   280,     0,     0,     0,   346,
      92,    93,   356,    94,    53,    52,   146,     0,   436,   372,
     364,   390,   434,   425,   426,   427,   428,   429,   391,   392,
     393,   394,   395,   396,   397,   398,   399,   400,   401,   402,
     403,   404,   405,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   415,   416,   417,   418,   419,   420,   421,   422,
     423,   424,   371,     0,   373,   430,   431,   433,   432,   383,
     307,   311,   315,     0,   134,   136,   308,   312,   316,    77,
      79,    91,    68,   159,    70,   175,   183,   155,    72,   149,
     153,   432,   154,     0,    74,    43,     0,   212,     0,   226,
     225,    89,    87,     0,   385,   384,   256,   257,   273,   267,
     272,    46,   262,   261,   263,    48,   268,   108,    99,   359,
     107,   363,   110,   111,   105,   106,    96,    98,   114,   103,
     116,   336,   344,   334,   335,   350,   349,    85,    83,     1,
       3,     4,     2,     0,     9,     7,    12,     0,    29,     0,
      38,     0,    30,    80,    59,   146,     0,    60,   207,   209,
     216,   213,     0,   233,   230,   237,   223,   222,   220,    41,
     276,   301,     0,   300,   318,   289,   303,     0,   302,   322,
     294,   305,     0,   304,   326,   299,   286,   291,   296,   347,
     355,    54,   206,    56,     0,   132,     0,   242,   243,   240,
     241,   148,   238,     0,   287,   292,   297,   145,    22,   437,
      24,     0,    18,   367,   369,   375,   365,   381,     0,   309,
     313,   317,     8,   133,   135,     0,   177,   182,   151,   156,
     152,   186,     0,    44,   228,    88,   258,   245,   259,   265,
     271,    49,    47,    51,   266,   109,   360,   362,   104,    95,
     113,   112,   333,   119,   342,   340,   343,   351,     0,    84,
       5,    10,    13,   134,    34,    31,    62,   129,   128,   126,
       0,    61,   137,   142,   208,   215,   214,   218,   231,   224,
     275,   319,   320,   288,   323,   324,   293,   327,   328,   298,
      55,   283,   130,   131,   284,   236,   244,   285,   290,   295,
     147,    21,     0,    23,    17,   366,   376,   374,   380,   178,
     379,   377,   179,   382,     0,   168,     0,     0,   176,     0,
     180,   150,   158,   157,     0,   190,   192,     0,   187,     0,
     194,     0,   260,   246,   247,   249,   235,   251,   269,    50,
     264,   361,   115,   332,   118,   337,   330,   331,   341,   117,
     348,   133,    32,    63,   139,   144,   141,   217,   321,   325,
     329,   281,   282,    20,    19,   378,     0,   169,   167,   162,
       0,   164,   181,   203,   204,   189,   191,     0,   197,   196,
     200,   248,   252,   250,   253,   270,   338,   120,   140,   138,
     143,   165,   170,   163,   166,   201,   202,     0,   184,   188,
     195,   198,   199,   254,   255,   339,   171,   185,   172,   173,
     174
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -322,  -322,   -45,  -322,   198,  -322,    22,   329,  -322,   370,
    -322,   375,   387,  -322,  -322,   -34,  -190,  -322,  -322,  -322,
    -322,   207,    92,    14,   392,    47,   154,  -322,  -322,  -322,
    -258,    58,  -284,   158,  -322,  -322,  -322,    25,  -322,     6,
    -322,  -322,   436,  -322,  -322,  -322,   -19,    53,  -322,  -322,
     -37,   283,  -322,   275,   449,  -322,  -322,  -322,  -322,   239,
     229,   233,   -21,   -16,   -14,  -322,  -322,  -322,  -321,   282,
    -322,  -322,  -322,  -322,  -322,  -322,     8,   152,    28,  -322,
     205,  -322,     0,  -322,   498,     1,    -4,  -322,  -266
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    46,   212,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,   196,   188,   198,   199,   324,    58,
      59,    60,   341,   342,    61,   158,   159,   160,   152,   386,
     387,   154,   381,   155,   164,   302,   397,   398,   399,   400,
      62,    63,    64,    65,    66,    67,    68,    69,   273,    70,
     178,   179,   180,   181,    71,    72,    73,    74,    75,   244,
     249,   254,    76,    77,    78,   245,   250,   255,   415,   416,
     202,    79,    80,    81,    82,    83,   189,   190,   191,   133,
     134,   185,   135,   136,   137,   161,   382,    87,   282
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      86,   139,   197,   214,   418,   210,   138,   207,    84,   182,
     321,   177,   323,   390,   183,   374,   184,   251,   162,   192,
     153,   433,   156,   172,   166,   168,   168,   252,    85,     4,
     186,     5,   210,   213,   279,   253,   200,   203,   371,   372,
     193,   251,     1,     2,   299,   149,   150,   151,  -160,   384,
       5,   279,    86,    88,    86,   171,   211,  -160,   225,   253,
      84,  -160,    84,   168,    13,   274,   168,   272,   235,   246,
     275,   215,   276,   140,   141,   142,   256,   257,   258,   170,
      85,   241,    85,   211,   228,   230,   248,    86,   233,   373,
     229,   236,   146,   263,   456,    84,   435,   243,   292,  -160,
       5,    45,     8,     9,   458,   442,   434,   262,   271,     5,
     385,   163,   261,     8,     9,    85,   231,     5,   187,   234,
     147,     8,     9,   148,  -161,   438,   436,     5,   440,   -27,
     221,     8,     9,  -161,   246,   475,   419,   439,   222,   205,
     293,   294,   -27,   310,   247,   274,   209,   176,   448,   272,
     275,   248,   276,     5,   306,   449,  -193,     8,     9,   240,
       1,     2,   319,     4,   182,   182,   177,   177,   331,   183,
     183,   184,   184,   333,   394,  -161,   307,   145,   216,   309,
     -27,   467,    13,     1,     2,   186,   385,   226,   300,   227,
     206,   443,   283,   468,   241,   444,   208,   394,   320,   317,
     305,   328,   326,   350,   242,    13,     1,     2,   287,   223,
     243,     1,     2,   395,   396,   232,    86,   316,   288,    45,
       4,   237,   238,   239,    84,   289,   340,   343,    13,    15,
      16,   259,   290,    13,   260,    15,    16,   291,   277,   295,
     296,   297,    45,   298,    85,   353,   308,   271,   301,   322,
     356,   344,   318,   345,   347,   359,     1,     2,   451,   349,
       4,   363,     5,   176,   330,    45,     8,     9,     1,     2,
      45,   334,   365,   366,   367,   368,   369,     4,    13,     5,
     335,   139,   336,     8,     9,   413,   138,   352,   407,   355,
      13,   360,    15,    16,   162,   393,   358,   156,   364,   370,
     -26,   221,   401,   361,     1,     2,   375,  -227,   169,   222,
       1,     2,   383,   -26,   410,    45,  -227,   303,   -16,  -227,
    -227,   412,   304,   414,   417,   417,    13,    45,   402,   421,
     265,   408,    13,   311,    15,    16,   343,   313,   424,   315,
       1,     2,   143,     4,   422,   411,   385,   437,   144,     8,
       9,   -26,   145,   -14,   394,   428,   429,   327,  -227,   329,
     430,   431,    13,    45,   432,   425,   441,   447,   427,    45,
     395,   396,  -127,   337,   450,     1,     2,   277,   453,   455,
    -127,  -127,   462,   346,   264,  -127,   348,   463,   461,   217,
     403,   404,   162,   464,     6,   405,   156,    13,   473,    45,
      10,    11,    12,   266,   452,   474,   454,   476,   477,   478,
     479,   480,   406,    16,   332,   417,   281,  -368,   284,   457,
    -368,  -368,   218,  -127,   343,   285,  -368,   219,   423,  -368,
    -368,  -368,   286,   339,    45,  -368,  -368,   338,   459,   465,
     466,   220,  -368,   446,  -368,  -368,   472,   401,   460,   401,
     224,   173,   391,   445,   388,   470,   417,    10,    11,    12,
     312,   167,    15,    16,   409,   140,   141,   142,  -368,   314,
      15,    16,   469,   165,  -370,    89,   354,     1,     2,  -368,
     420,   351,  -370,  -370,   325,   357,  -370,  -370,  -370,    90,
     377,    91,  -370,  -370,    92,   426,    93,    94,    95,    13,
      96,    97,  -370,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   204,    45,   130,   131,   132,
     278,     0,     0,     1,     2,     0,     0,     0,     0,     3,
       0,     0,     4,   279,     5,     0,     6,   217,     8,     9,
       0,     0,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,     0,     0,     0,     0,    31,    32,    33,
      34,   280,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,     1,     2,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
      92,     0,    93,    94,    95,    13,    96,    97,     0,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,     0,    45,   130,   131,   376,     1,     2,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     5,     0,     0,
      91,     8,     9,    92,     0,    93,    94,    95,    13,    96,
      97,     0,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,     0,    45,   130,   131,     1,     2,
     378,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,     0,     0,    92,   379,    93,    94,    95,
      13,    96,    97,   380,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,     0,    45,   130,   131,
       1,     2,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,    92,   379,    93,
      94,    95,    13,    96,    97,   389,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,     0,    45,
     130,   131,     1,     2,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    91,     0,     0,    92,
       0,    93,    94,    95,    13,    96,    97,   157,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
       0,    45,   130,   131,     1,     2,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     5,     0,     0,    91,     0,
       0,    92,     0,    93,    94,    95,    13,    96,    97,     0,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,     0,    45,   130,   131,     1,     2,     0,     0,
       0,     0,     0,     0,     0,   -15,     0,     0,     0,     0,
      91,     0,     0,    92,     0,    93,    94,    95,    13,    96,
      97,     0,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,     0,    45,   130,   131,     1,     2,
     362,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,     0,     0,    92,     0,    93,    94,    95,
      13,    96,    97,     0,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,     0,    45,   130,   131,
       1,     2,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,    92,     0,    93,
      94,    95,    13,    96,    97,   392,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,     0,    45,
     130,   131,     1,     2,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    91,     0,     0,    92,
     379,    93,    94,    95,    13,    96,    97,     0,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
       0,    45,   130,   131,     1,     2,   471,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,    92,     0,    93,    94,    95,    13,    96,    97,     0,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,     0,    45,   130,   131,     1,     2,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      91,     0,     0,    92,     0,    93,    94,    95,    13,    96,
      97,     0,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,     0,    45,   130,   131,     1,     2,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
      13,     0,     0,     0,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,     0,    45,   130,   131,
       1,     2,     0,     0,     0,     0,     3,     0,     0,     4,
       0,     5,     0,     6,     7,     8,     9,     0,     0,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
       0,     0,     0,     0,    31,    32,    33,    34,     0,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
       1,     2,     0,     0,     0,     0,     3,     0,     0,     4,
       0,     5,     0,     6,   217,     8,     9,     0,     0,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
       0,     1,     2,     0,    31,    32,    33,    34,     0,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
       1,     2,     0,    13,     0,    15,    16,     0,     0,     4,
       0,     5,     0,     6,   217,     8,     9,     0,     0,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,     0,    30,
      45,     0,     0,     0,    31,    32,    33,    34,     0,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
       1,     2,     0,     0,     0,     0,     1,     2,     0,   192,
       0,     5,     0,   173,     0,     8,     9,     0,     0,    10,
      11,    12,    13,     0,    15,    16,     1,     2,    13,     0,
     193,     0,     0,     0,     0,     0,     0,     0,     0,   173,
       0,   176,     0,     0,     0,    10,    11,    12,    13,   322,
      15,    16,     0,     1,     2,   187,   192,     0,     0,    45,
     174,     1,     2,     0,     5,    45,   175,   176,     8,     9,
       1,     2,     0,     0,     6,    13,     0,   193,     0,     0,
      10,    11,    12,    13,    14,    45,     0,     0,     0,    93,
      94,    95,    13,    96,    97,     0,   194,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   195,     0,     0,     0,
       0,     0,    45,     0,     0,     0,     0,     0,     0,     0,
      45,     0,     0,     0,   264,   265,     0,     0,   201,    45,
       4,     0,     5,     0,     6,     0,     8,     9,     0,     0,
      10,    11,    12,   266,     0,    15,    16,     0,     0,     0,
       0,     0,     0,     0,     0,   267,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   268,     0,     0,     0,     0,     0,     0,
       0,   269,   270
};

static const yytype_int16 yycheck[] =
{
       0,     5,    36,    48,   325,     0,     5,    44,     0,    30,
     200,    30,   202,   297,    30,   281,    30,     6,    22,     6,
      20,     0,    21,    27,    24,    25,    26,    16,     0,    12,
      30,    14,     0,     1,    13,    24,    36,    37,     0,     1,
      27,     6,     3,     4,     8,    17,    18,    19,     0,     1,
      14,    13,    52,    65,    54,    27,    51,     9,    58,    24,
      52,    13,    54,    63,    25,    86,    66,    86,    68,     6,
      86,    49,    86,    22,    23,    24,    76,    77,    78,    26,
      52,     6,    54,    51,    62,    63,    23,    87,    66,    51,
      62,    69,    16,    85,   415,    87,   380,    22,   143,    51,
      14,    62,    18,    19,    65,   389,   372,    85,    86,    14,
      62,     3,    84,    18,    19,    87,    63,    14,     5,    66,
      16,    18,    19,    16,     0,     1,   384,    14,   386,     0,
       1,    18,    19,     9,     6,   456,   326,    13,     9,    44,
     144,   145,    13,   180,    16,   166,     0,    44,     1,   168,
     166,    23,   166,    14,   173,     8,     9,    18,    19,    45,
       3,     4,   196,    12,   185,   186,   185,   186,   213,   185,
     186,   185,   186,     7,    17,    51,   176,    11,    13,   179,
      51,     1,    25,     3,     4,   185,    62,     8,   160,     8,
      38,     7,    15,    13,     6,    11,    44,    17,   198,   191,
     172,   205,   202,   240,    16,    25,     3,     4,     7,    57,
      22,     3,     4,    33,    34,    63,   216,   189,     7,    62,
      12,    69,    70,    71,   216,    16,   226,   227,    25,    27,
      28,    79,    16,    25,    82,    27,    28,    16,    86,    12,
       8,     7,    62,     8,   216,   245,    16,   225,    12,    46,
     250,   229,    56,   231,   232,   255,     3,     4,     5,   237,
      12,   265,    14,    44,     9,    62,    18,    19,     3,     4,
      62,     9,   272,   273,   274,   275,   276,    12,    25,    14,
      65,   285,     8,    18,    19,   322,   285,    16,   307,    16,
      25,   263,    27,    28,   298,   299,    16,   296,     6,   277,
       0,     1,   302,    25,     3,     4,    15,     0,     1,     9,
       3,     4,    65,    13,   314,    62,     9,   165,    12,    12,
      13,   321,   170,   323,   324,   325,    25,    62,    16,   333,
       7,    65,    25,   181,    27,    28,   336,   185,     8,   187,
       3,     4,     1,    12,    65,   317,    62,     5,     7,    18,
      19,    51,    11,    12,    17,    16,    16,   205,    51,   207,
      16,   361,    25,    62,   364,   343,     9,     9,   346,    62,
      33,    34,     0,     1,     7,     3,     4,   225,     5,    65,
       8,     9,     5,   231,     6,    13,   234,    13,     9,    17,
       5,     6,   396,     9,    16,    10,   395,    25,     5,    62,
      22,    23,    24,    25,   404,     5,   406,     5,    13,     5,
       5,     5,    27,    28,   216,   415,    87,     0,     1,   419,
       3,     4,    52,    51,   424,     8,     9,    52,   336,    12,
      13,    14,    15,   226,    62,    18,    19,    65,   424,   443,
     444,    54,    25,   396,    27,    28,   450,   447,   426,   449,
      58,    16,   298,   395,   296,   449,   456,    22,    23,    24,
     185,    25,    27,    28,   312,    22,    23,    24,    51,   186,
      27,    28,   447,    24,     0,     1,   247,     3,     4,    62,
     328,   242,     8,     9,   202,   252,    12,    13,    14,    15,
     285,    17,    18,    19,    20,   343,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    37,    62,    63,    64,    65,
       0,    -1,    -1,     3,     4,    -1,    -1,    -1,    -1,     9,
      -1,    -1,    12,    13,    14,    -1,    16,    17,    18,    19,
      -1,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    -1,    -1,    -1,    -1,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,     3,     4,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,     3,     4,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    19,    20,    -1,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,     3,     4,
       5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
       3,     4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,     3,     4,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,     3,     4,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    14,    -1,    -1,    17,    -1,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,     3,     4,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,     3,     4,
       5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    -1,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
       3,     4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,     3,     4,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,     3,     4,     5,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,     3,     4,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,     3,     4,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
       3,     4,    -1,    -1,    -1,    -1,     9,    -1,    -1,    12,
      -1,    14,    -1,    16,    17,    18,    19,    -1,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      -1,    -1,    -1,    -1,    47,    48,    49,    50,    -1,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
       3,     4,    -1,    -1,    -1,    -1,     9,    -1,    -1,    12,
      -1,    14,    -1,    16,    17,    18,    19,    -1,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      -1,     3,     4,    -1,    47,    48,    49,    50,    -1,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
       3,     4,    -1,    25,    -1,    27,    28,    -1,    -1,    12,
      -1,    14,    -1,    16,    17,    18,    19,    -1,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      62,    -1,    -1,    -1,    47,    48,    49,    50,    -1,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
       3,     4,    -1,    -1,    -1,    -1,     3,     4,    -1,     6,
      -1,    14,    -1,    16,    -1,    18,    19,    -1,    -1,    22,
      23,    24,    25,    -1,    27,    28,     3,     4,    25,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    16,
      -1,    44,    -1,    -1,    -1,    22,    23,    24,    25,    46,
      27,    28,    -1,     3,     4,     5,     6,    -1,    -1,    62,
      37,     3,     4,    -1,    14,    62,    43,    44,    18,    19,
       3,     4,    -1,    -1,    16,    25,    -1,    27,    -1,    -1,
      22,    23,    24,    25,    26,    62,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    27,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,
      -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    -1,    -1,    -1,     6,     7,    -1,    -1,    61,    62,
      12,    -1,    14,    -1,    16,    -1,    18,    19,    -1,    -1,
      22,    23,    24,    25,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    64
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     9,    12,    14,    16,    17,    18,    19,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    47,    48,    49,    50,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    67,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    85,    86,
      87,    90,   106,   107,   108,   109,   110,   111,   112,   113,
     115,   120,   121,   122,   123,   124,   128,   129,   130,   137,
     138,   139,   140,   141,   142,   144,   148,   153,    65,     1,
      15,    17,    20,    22,    23,    24,    26,    27,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      63,    64,    65,   145,   146,   148,   149,   150,   151,   152,
      22,    23,    24,     1,     7,    11,    16,    16,    16,   144,
     144,   144,    94,   148,    97,    99,   151,    28,    91,    92,
      93,   151,   152,     3,   100,   120,   148,   108,   148,     1,
     113,   144,   152,    16,    37,    43,    44,   112,   116,   117,
     118,   119,   128,   129,   130,   147,   148,     5,    81,   142,
     143,   144,     6,    27,    46,    56,    80,    81,    82,    83,
     148,    61,   136,   148,   150,    44,   143,   116,   143,     0,
       0,    51,    68,     1,    68,    72,    13,    17,    75,    77,
      78,     1,     9,   143,    90,   148,     8,     8,    72,   144,
      72,   113,   143,    72,   113,   148,    72,   143,   143,   143,
      45,     6,    16,    22,   125,   131,     6,    16,    23,   126,
     132,     6,    16,    24,   127,   133,   148,   148,   148,   143,
     143,   144,    72,   142,     6,     7,    25,    37,    55,    63,
      64,    72,   112,   114,   128,   129,   130,   143,     0,    13,
      51,    73,   154,    15,     1,     8,    15,     7,     7,    16,
      16,    16,    68,   152,   152,    12,     8,     7,     8,     8,
     144,    12,   101,   143,   143,   144,   112,   148,    16,   148,
     116,   143,   119,   143,   117,   143,   144,   142,    56,    81,
     148,    82,    46,    82,    84,   135,   148,   143,   152,   143,
       9,    68,    70,     7,     9,    65,     8,     1,    65,    87,
     148,    88,    89,   148,    72,    72,   143,    72,   143,    72,
     116,   125,    16,   148,   126,    16,   148,   127,    16,   148,
     144,    25,     5,   152,     6,   148,   148,   148,   148,   148,
      72,     0,     1,    51,   154,    15,    65,   146,     5,    21,
      28,    98,   152,    65,     1,    62,    95,    96,    99,    28,
      98,    92,    28,   152,    17,    33,    34,   102,   103,   104,
     105,   148,    16,     5,     6,    10,    27,   112,    65,   143,
     148,   144,   148,   116,   148,   134,   135,   148,   134,    82,
     143,   152,    65,    88,     8,    72,   143,    72,    16,    16,
      16,   148,   148,     0,   154,    98,    96,     5,     1,    13,
      96,     9,    98,     7,    11,    97,    91,     9,     1,     8,
       7,     5,   148,     5,   148,    65,   134,   148,    65,    89,
      72,     9,     5,    13,     9,   152,   152,     1,    13,   103,
     105,     5,   152,     5,     5,   134,     5,    13,     5,     5,
       5
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    66,    67,    68,    68,    68,    69,    69,    69,    69,
      69,    70,    70,    70,    71,    71,    71,    72,    72,    72,
      72,    72,    72,    72,    72,    73,    73,    73,    74,    74,
      75,    75,    75,    75,    75,    76,    76,    77,    77,    77,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    79,    79,    80,    80,    80,    80,    81,    81,    81,
      82,    82,    83,    83,    83,    83,    83,    84,    84,    84,
      84,    85,    85,    85,    85,    86,    86,    86,    86,    86,
      87,    87,    87,    87,    87,    87,    87,    88,    88,    88,
      88,    89,    89,    89,    89,    90,    90,    90,    90,    91,
      91,    91,    92,    92,    93,    93,    93,    93,    93,    94,
      94,    94,    94,    94,    95,    95,    95,    95,    96,    96,
      96,    96,    96,    96,    96,    97,    97,    97,    98,    98,
      99,    99,    99,    99,   100,   100,   101,   102,   102,   103,
     103,   103,   103,   103,   104,   104,   104,   104,   105,   105,
     105,   105,   105,   105,   105,   106,   106,   106,   106,   106,
     107,   107,   107,   107,   107,   107,   107,   107,   107,   107,
     107,   108,   108,   108,   108,   109,   109,   109,   109,   110,
     110,   110,   111,   111,   112,   112,   113,   113,   113,   113,
     114,   114,   114,   114,   115,   116,   116,   116,   116,   116,
     116,   116,   116,   116,   116,   116,   116,   117,   117,   117,
     117,   117,   117,   117,   118,   118,   118,   118,   118,   118,
     118,   119,   119,   119,   120,   120,   120,   120,   121,   121,
     121,   121,   121,   121,   121,   122,   122,   122,   122,   122,
     123,   123,   123,   123,   123,   124,   124,   124,   124,   124,
     125,   125,   126,   126,   127,   127,   128,   128,   128,   128,
     129,   129,   129,   129,   130,   130,   130,   130,   131,   131,
     131,   131,   132,   132,   132,   132,   133,   133,   133,   133,
     134,   134,   135,   135,   136,   136,   136,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   138,   138,   139,   139,
     139,   139,   140,   140,   140,   141,   141,   142,   142,   143,
     143,   143,   143,   143,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   145,   145,   145,   145,   146,   146,   146,
     146,   146,   146,   146,   147,   147,   148,   148,   148,   148,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   150,   150,   150,   150,   150,
     151,   151,   152,   152,   152,   153,   153,   154
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     1,     2,     0,     2,     3,     2,
       3,     1,     2,     3,     1,     2,     3,     3,     2,     4,
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
       2,     1,     2,     2,     3,     2,     2,     1,     3,     1,
       2,     3,     1,     2,     1,     1,     3,     2,     2,     1,
       1,     1,     1,     1,     3,     2,     3,     3,     4,     3,
       4,     3,     4,     4,     5,     5,     1,     1,     2,     2,
       3,     1,     1,     1,     3,     2,     2,     1,     1,     3,
       4,     2,     1,     1,     1,     3,     2,     1,     1,     1,
       1,     4,     4,     3,     3,     3,     2,     2,     3,     2,
       3,     2,     2,     3,     2,     3,     2,     2,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     3,
       1,     2,     2,     3,     1,     2,     2,     3,     1,     2,
       2,     3,     1,     2,     2,     3,     1,     2,     2,     3,
       1,     1,     2,     1,     1,     1,     1,     4,     5,     6,
       3,     4,     3,     3,     2,     1,     1,     2,     4,     2,
       2,     3,     1,     1,     1,     2,     1,     1,     1,     1,
       2,     3,     2,     1,     2,     3,     4,     3,     2,     3,
       1,     2,     2,     1,     3,     2,     3,     3,     4,     3,
       3,     2,     3,     1,     1,     1,     1,     1,     1,     1,
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
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 1995 "language.cc" /* yacc.c:1257  */
        break;

    case 4: /* TOK_QSTRING  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2001 "language.cc" /* yacc.c:1257  */
        break;

    case 5: /* TOK_NUMBER  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2007 "language.cc" /* yacc.c:1257  */
        break;

    case 17: /* TOK_MSC  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2013 "language.cc" /* yacc.c:1257  */
        break;

    case 18: /* TOK_COLON_STRING  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2019 "language.cc" /* yacc.c:1257  */
        break;

    case 19: /* TOK_COLON_QUOTED_STRING  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2025 "language.cc" /* yacc.c:1257  */
        break;

    case 20: /* TOK_STYLE_NAME  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2031 "language.cc" /* yacc.c:1257  */
        break;

    case 21: /* TOK_COLORDEF  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2037 "language.cc" /* yacc.c:1257  */
        break;

    case 25: /* TOK_REL_X  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2043 "language.cc" /* yacc.c:1257  */
        break;

    case 29: /* TOK_COMMAND_HEADING  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2049 "language.cc" /* yacc.c:1257  */
        break;

    case 30: /* TOK_COMMAND_NUDGE  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2055 "language.cc" /* yacc.c:1257  */
        break;

    case 31: /* TOK_COMMAND_NEWPAGE  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2061 "language.cc" /* yacc.c:1257  */
        break;

    case 32: /* TOK_COMMAND_DEFSHAPE  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2067 "language.cc" /* yacc.c:1257  */
        break;

    case 33: /* TOK_COMMAND_DEFCOLOR  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2073 "language.cc" /* yacc.c:1257  */
        break;

    case 34: /* TOK_COMMAND_DEFSTYLE  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2079 "language.cc" /* yacc.c:1257  */
        break;

    case 35: /* TOK_COMMAND_DEFDESIGN  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2085 "language.cc" /* yacc.c:1257  */
        break;

    case 36: /* TOK_COMMAND_BIG  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2091 "language.cc" /* yacc.c:1257  */
        break;

    case 37: /* TOK_COMMAND_BOX  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2097 "language.cc" /* yacc.c:1257  */
        break;

    case 38: /* TOK_COMMAND_PIPE  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2103 "language.cc" /* yacc.c:1257  */
        break;

    case 39: /* TOK_COMMAND_MARK  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2109 "language.cc" /* yacc.c:1257  */
        break;

    case 40: /* TOK_COMMAND_PARALLEL  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2115 "language.cc" /* yacc.c:1257  */
        break;

    case 41: /* TOK_COMMAND_OVERLAP  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2121 "language.cc" /* yacc.c:1257  */
        break;

    case 42: /* TOK_VERTICAL  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2127 "language.cc" /* yacc.c:1257  */
        break;

    case 43: /* TOK_VERTICAL_SHAPE  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2133 "language.cc" /* yacc.c:1257  */
        break;

    case 44: /* TOK_AT  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2139 "language.cc" /* yacc.c:1257  */
        break;

    case 45: /* TOK_LOST  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2145 "language.cc" /* yacc.c:1257  */
        break;

    case 46: /* TOK_AT_POS  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2151 "language.cc" /* yacc.c:1257  */
        break;

    case 47: /* TOK_SHOW  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2157 "language.cc" /* yacc.c:1257  */
        break;

    case 48: /* TOK_HIDE  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2163 "language.cc" /* yacc.c:1257  */
        break;

    case 49: /* TOK_ACTIVATE  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2169 "language.cc" /* yacc.c:1257  */
        break;

    case 50: /* TOK_DEACTIVATE  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2175 "language.cc" /* yacc.c:1257  */
        break;

    case 51: /* TOK_BYE  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2181 "language.cc" /* yacc.c:1257  */
        break;

    case 52: /* TOK_COMMAND_VSPACE  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2187 "language.cc" /* yacc.c:1257  */
        break;

    case 53: /* TOK_COMMAND_HSPACE  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2193 "language.cc" /* yacc.c:1257  */
        break;

    case 54: /* TOK_COMMAND_SYMBOL  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2199 "language.cc" /* yacc.c:1257  */
        break;

    case 55: /* TOK_COMMAND_NOTE  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2205 "language.cc" /* yacc.c:1257  */
        break;

    case 56: /* TOK_COMMAND_COMMENT  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2211 "language.cc" /* yacc.c:1257  */
        break;

    case 57: /* TOK_COMMAND_ENDNOTE  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2217 "language.cc" /* yacc.c:1257  */
        break;

    case 58: /* TOK_COMMAND_FOOTNOTE  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2223 "language.cc" /* yacc.c:1257  */
        break;

    case 59: /* TOK_COMMAND_TITLE  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2229 "language.cc" /* yacc.c:1257  */
        break;

    case 60: /* TOK_COMMAND_SUBTITLE  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2235 "language.cc" /* yacc.c:1257  */
        break;

    case 61: /* TOK_COMMAND_TEXT  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2241 "language.cc" /* yacc.c:1257  */
        break;

    case 63: /* TOK_MSCGEN_RBOX  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2247 "language.cc" /* yacc.c:1257  */
        break;

    case 64: /* TOK_MSCGEN_ABOX  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2253 "language.cc" /* yacc.c:1257  */
        break;

    case 70: /* top_level_arclist  */
#line 167 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arclist);}
#line 2259 "language.cc" /* yacc.c:1257  */
        break;

    case 72: /* braced_arclist  */
#line 167 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arclist);}
#line 2265 "language.cc" /* yacc.c:1257  */
        break;

    case 73: /* arclist_maybe_no_semicolon  */
#line 167 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arclist);}
#line 2271 "language.cc" /* yacc.c:1257  */
        break;

    case 74: /* arclist  */
#line 167 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arclist);}
#line 2277 "language.cc" /* yacc.c:1257  */
        break;

    case 75: /* arc_with_parallel_semicolon  */
#line 167 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2283 "language.cc" /* yacc.c:1257  */
        break;

    case 76: /* overlap_or_parallel  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2289 "language.cc" /* yacc.c:1257  */
        break;

    case 77: /* arc_with_parallel  */
#line 167 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2295 "language.cc" /* yacc.c:1257  */
        break;

    case 78: /* arc  */
#line 167 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2301 "language.cc" /* yacc.c:1257  */
        break;

    case 79: /* titlecommandtoken  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2307 "language.cc" /* yacc.c:1257  */
        break;

    case 80: /* hspace_location  */
#line 170 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).namerel);}
#line 2313 "language.cc" /* yacc.c:1257  */
        break;

    case 81: /* full_arcattrlist_with_label_or_number  */
#line 169 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).attriblist);}
#line 2319 "language.cc" /* yacc.c:1257  */
        break;

    case 83: /* entityrel  */
#line 170 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).namerel);}
#line 2325 "language.cc" /* yacc.c:1257  */
        break;

    case 84: /* markerrel_no_string  */
#line 170 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).namerel);}
#line 2331 "language.cc" /* yacc.c:1257  */
        break;

    case 85: /* entity_command_prefixes  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2337 "language.cc" /* yacc.c:1257  */
        break;

    case 86: /* optlist  */
#line 167 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arclist);}
#line 2343 "language.cc" /* yacc.c:1257  */
        break;

    case 87: /* opt  */
#line 167 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2349 "language.cc" /* yacc.c:1257  */
        break;

    case 88: /* entitylist  */
#line 169 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).entitylist);}
#line 2355 "language.cc" /* yacc.c:1257  */
        break;

    case 89: /* entity  */
#line 169 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).entitylist);}
#line 2361 "language.cc" /* yacc.c:1257  */
        break;

    case 90: /* first_entity  */
#line 169 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).entitylist);}
#line 2367 "language.cc" /* yacc.c:1257  */
        break;

    case 93: /* stylenamelist  */
#line 170 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).stringlist);}
#line 2373 "language.cc" /* yacc.c:1257  */
        break;

    case 98: /* color_string  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2379 "language.cc" /* yacc.c:1257  */
        break;

    case 106: /* parallel  */
#line 168 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcparallel);}
#line 2385 "language.cc" /* yacc.c:1257  */
        break;

    case 107: /* box_list  */
#line 168 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcboxseries);}
#line 2391 "language.cc" /* yacc.c:1257  */
        break;

    case 108: /* first_box  */
#line 168 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbox);}
#line 2397 "language.cc" /* yacc.c:1257  */
        break;

    case 109: /* first_pipe  */
#line 168 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcpipe);}
#line 2403 "language.cc" /* yacc.c:1257  */
        break;

    case 110: /* pipe_list_no_content  */
#line 168 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcpipeseries);}
#line 2409 "language.cc" /* yacc.c:1257  */
        break;

    case 111: /* pipe_list  */
#line 168 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcpipeseries);}
#line 2415 "language.cc" /* yacc.c:1257  */
        break;

    case 113: /* boxrel  */
#line 168 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbox);}
#line 2421 "language.cc" /* yacc.c:1257  */
        break;

    case 114: /* mscgen_emphrel  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2427 "language.cc" /* yacc.c:1257  */
        break;

    case 115: /* mscgen_boxrel  */
#line 168 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbox);}
#line 2433 "language.cc" /* yacc.c:1257  */
        break;

    case 116: /* vertxpos  */
#line 170 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).vertxpos);}
#line 2439 "language.cc" /* yacc.c:1257  */
        break;

    case 118: /* vertrel_no_xpos  */
#line 167 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcvertarrow);}
#line 2445 "language.cc" /* yacc.c:1257  */
        break;

    case 119: /* vertrel  */
#line 167 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcvertarrow);}
#line 2451 "language.cc" /* yacc.c:1257  */
        break;

    case 120: /* arcrel  */
#line 167 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2457 "language.cc" /* yacc.c:1257  */
        break;

    case 121: /* arcrel_arrow  */
#line 167 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcarrow);}
#line 2463 "language.cc" /* yacc.c:1257  */
        break;

    case 122: /* arcrel_to  */
#line 167 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcarrow);}
#line 2469 "language.cc" /* yacc.c:1257  */
        break;

    case 123: /* arcrel_from  */
#line 167 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcarrow);}
#line 2475 "language.cc" /* yacc.c:1257  */
        break;

    case 124: /* arcrel_bidir  */
#line 167 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcarrow);}
#line 2481 "language.cc" /* yacc.c:1257  */
        break;

    case 134: /* extvertxpos  */
#line 170 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).extvertxpos);}
#line 2487 "language.cc" /* yacc.c:1257  */
        break;

    case 135: /* extvertxpos_no_string  */
#line 170 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).extvertxpos);}
#line 2493 "language.cc" /* yacc.c:1257  */
        break;

    case 136: /* symbol_type_string  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2499 "language.cc" /* yacc.c:1257  */
        break;

    case 137: /* symbol_command_no_attr  */
#line 167 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2505 "language.cc" /* yacc.c:1257  */
        break;

    case 138: /* symbol_command  */
#line 167 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2511 "language.cc" /* yacc.c:1257  */
        break;

    case 139: /* note  */
#line 167 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2517 "language.cc" /* yacc.c:1257  */
        break;

    case 141: /* comment  */
#line 167 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2523 "language.cc" /* yacc.c:1257  */
        break;

    case 142: /* colon_string  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2529 "language.cc" /* yacc.c:1257  */
        break;

    case 143: /* full_arcattrlist_with_label  */
#line 169 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).attriblist);}
#line 2535 "language.cc" /* yacc.c:1257  */
        break;

    case 144: /* full_arcattrlist  */
#line 169 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).attriblist);}
#line 2541 "language.cc" /* yacc.c:1257  */
        break;

    case 145: /* arcattrlist  */
#line 169 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).attriblist);}
#line 2547 "language.cc" /* yacc.c:1257  */
        break;

    case 146: /* arcattr  */
#line 169 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).attrib);}
#line 2553 "language.cc" /* yacc.c:1257  */
        break;

    case 148: /* entity_string  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2559 "language.cc" /* yacc.c:1257  */
        break;

    case 149: /* reserved_word_string  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2565 "language.cc" /* yacc.c:1257  */
        break;

    case 150: /* symbol_string  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2571 "language.cc" /* yacc.c:1257  */
        break;

    case 151: /* alpha_string  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2577 "language.cc" /* yacc.c:1257  */
        break;

    case 152: /* string  */
#line 171 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2583 "language.cc" /* yacc.c:1257  */
        break;

    case 154: /* scope_close  */
#line 167 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2589 "language.cc" /* yacc.c:1257  */
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

#line 2705 "language.cc" /* yacc.c:1429  */
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
#line 238 "language.yy" /* yacc.c:1646  */
    {
	YYACCEPT;
}
#line 2896 "language.cc" /* yacc.c:1646  */
    break;

  case 4:
#line 244 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
	csh.AddCSH_AllCommentBeyond((yylsp[0]));
  #else
  #endif
    free((yyvsp[0].str));
}
#line 2909 "language.cc" /* yacc.c:1646  */
    break;

  case 5:
#line 253 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[0]), COLOR_SEMICOLON);
	csh.AddCSH_AllCommentBeyond((yylsp[0]));
  #else
  #endif
    free((yyvsp[-1].str));
}
#line 2923 "language.cc" /* yacc.c:1646  */
    break;

  case 6:
#line 264 "language.yy" /* yacc.c:1646  */
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
#line 2938 "language.cc" /* yacc.c:1646  */
    break;

  case 7:
#line 275 "language.yy" /* yacc.c:1646  */
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
#line 2960 "language.cc" /* yacc.c:1646  */
    break;

  case 8:
#line 293 "language.yy" /* yacc.c:1646  */
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
#line 2979 "language.cc" /* yacc.c:1646  */
    break;

  case 9:
#line 308 "language.yy" /* yacc.c:1646  */
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
#line 2995 "language.cc" /* yacc.c:1646  */
    break;

  case 10:
#line 320 "language.yy" /* yacc.c:1646  */
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
#line 3016 "language.cc" /* yacc.c:1646  */
    break;

  case 12:
#line 339 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[0]), "Closing brace missing its opening pair.");
  #else
    (yyval.arclist) = (yyvsp[-1].arclist);
    msc.Error.Error(MSC_POS((yylsp[0])).start, "Unexpected '}'.");
  #endif
}
#line 3029 "language.cc" /* yacc.c:1646  */
    break;

  case 13:
#line 348 "language.yy" /* yacc.c:1646  */
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
#line 3045 "language.cc" /* yacc.c:1646  */
    break;

  case 14:
#line 362 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.mscgen_compat == EMscgenCompat::AUTODETECT)
        csh.mscgen_compat = EMscgenCompat::FORCE_MSCGEN;
  #else
    if (msc.mscgen_compat == EMscgenCompat::AUTODETECT)
        msc.mscgen_compat = EMscgenCompat::FORCE_MSCGEN;
  #endif
    free((yyvsp[0].str));
}
#line 3061 "language.cc" /* yacc.c:1646  */
    break;

  case 15:
#line 374 "language.yy" /* yacc.c:1646  */
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
#line 3078 "language.cc" /* yacc.c:1646  */
    break;

  case 16:
#line 387 "language.yy" /* yacc.c:1646  */
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
#line 3100 "language.cc" /* yacc.c:1646  */
    break;

  case 17:
#line 406 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    if ((yyvsp[0].arcbase)) ((yyvsp[-1].arclist))->Append((yyvsp[0].arcbase)); //Append any potential CommandNumbering
    (yyval.arclist) = (yyvsp[-1].arclist);
  #endif
}
#line 3112 "language.cc" /* yacc.c:1646  */
    break;

  case 18:
#line 414 "language.yy" /* yacc.c:1646  */
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
#line 3127 "language.cc" /* yacc.c:1646  */
    break;

  case 19:
#line 425 "language.yy" /* yacc.c:1646  */
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
#line 3142 "language.cc" /* yacc.c:1646  */
    break;

  case 20:
#line 436 "language.yy" /* yacc.c:1646  */
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
#line 3157 "language.cc" /* yacc.c:1646  */
    break;

  case 21:
#line 447 "language.yy" /* yacc.c:1646  */
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
#line 3172 "language.cc" /* yacc.c:1646  */
    break;

  case 22:
#line 458 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ErrorAfter((yylsp[-1]), "Missing a closing brace ('}').");
  #else
    (yyval.arclist) = NULL;
    //Do not pop context, as the missing scope_close would have done
    msc.Error.Error(MSC_POS((yylsp[-1])).end.NextChar(), "Missing a corresponding '}'.");
  #endif
}
#line 3186 "language.cc" /* yacc.c:1646  */
    break;

  case 23:
#line 468 "language.yy" /* yacc.c:1646  */
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
#line 3202 "language.cc" /* yacc.c:1646  */
    break;

  case 24:
#line 480 "language.yy" /* yacc.c:1646  */
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
#line 3217 "language.cc" /* yacc.c:1646  */
    break;

  case 26:
#line 495 "language.yy" /* yacc.c:1646  */
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
#line 3232 "language.cc" /* yacc.c:1646  */
    break;

  case 27:
#line 506 "language.yy" /* yacc.c:1646  */
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
#line 3249 "language.cc" /* yacc.c:1646  */
    break;

  case 28:
#line 521 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[0].arcbase))
        (yyval.arclist) = (new ArcList)->Append((yyvsp[0].arcbase)); /* New list */
    else
        (yyval.arclist) = new ArcList;
  #endif
}
#line 3262 "language.cc" /* yacc.c:1646  */
    break;

  case 29:
#line 530 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[0].arcbase)) ((yyvsp[-1].arclist))->Append((yyvsp[0].arcbase));     /* Add to existing list */
    (yyval.arclist) = ((yyvsp[-1].arclist));
  #endif
}
#line 3273 "language.cc" /* yacc.c:1646  */
    break;

  case 30:
#line 538 "language.yy" /* yacc.c:1646  */
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
#line 3289 "language.cc" /* yacc.c:1646  */
    break;

  case 33:
#line 554 "language.yy" /* yacc.c:1646  */
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
#line 3305 "language.cc" /* yacc.c:1646  */
    break;

  case 34:
#line 566 "language.yy" /* yacc.c:1646  */
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
#line 3324 "language.cc" /* yacc.c:1646  */
    break;

  case 37:
#line 584 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    if ((yyvsp[0].arcbase)) 
		((yyvsp[0].arcbase))->SetLineEnd(MSC_POS((yyloc)));
    (yyval.arcbase)=(yyvsp[0].arcbase);
  #endif
}
#line 3337 "language.cc" /* yacc.c:1646  */
    break;

  case 38:
#line 593 "language.yy" /* yacc.c:1646  */
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
#line 3365 "language.cc" /* yacc.c:1646  */
    break;

  case 39:
#line 617 "language.yy" /* yacc.c:1646  */
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
#line 3386 "language.cc" /* yacc.c:1646  */
    break;

  case 40:
#line 635 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    if ((yyvsp[0].arcbase))
        ((yyvsp[0].arcbase))->AddAttributeList(NULL);
    (yyval.arcbase)=((yyvsp[0].arcbase));
  #endif
}
#line 3399 "language.cc" /* yacc.c:1646  */
    break;

  case 41:
#line 644 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0])))
        ArcArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0])))
        ArcArrow::AttributeValues(csh.hintAttrName, csh);
  #else
    if ((yyvsp[-1].arcbase))
        ((yyvsp[-1].arcbase))->AddAttributeList((yyvsp[0].attriblist));
    (yyval.arcbase) = ((yyvsp[-1].arcbase));
  #endif
}
#line 3416 "language.cc" /* yacc.c:1646  */
    break;

  case 42:
#line 657 "language.yy" /* yacc.c:1646  */
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
#line 3437 "language.cc" /* yacc.c:1646  */
    break;

  case 43:
#line 674 "language.yy" /* yacc.c:1646  */
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
#line 3461 "language.cc" /* yacc.c:1646  */
    break;

  case 44:
#line 694 "language.yy" /* yacc.c:1646  */
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
#line 3488 "language.cc" /* yacc.c:1646  */
    break;

  case 45:
#line 717 "language.yy" /* yacc.c:1646  */
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
#line 3511 "language.cc" /* yacc.c:1646  */
    break;

  case 46:
#line 736 "language.yy" /* yacc.c:1646  */
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
#line 3536 "language.cc" /* yacc.c:1646  */
    break;

  case 47:
#line 757 "language.yy" /* yacc.c:1646  */
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
#line 3563 "language.cc" /* yacc.c:1646  */
    break;

  case 48:
#line 780 "language.yy" /* yacc.c:1646  */
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
	typeplusdir.arc.type = EArcSymbol::ARC_SOLID;
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
#line 3598 "language.cc" /* yacc.c:1646  */
    break;

  case 49:
#line 811 "language.yy" /* yacc.c:1646  */
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
#line 3626 "language.cc" /* yacc.c:1646  */
    break;

  case 50:
#line 835 "language.yy" /* yacc.c:1646  */
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
#line 3655 "language.cc" /* yacc.c:1646  */
    break;

  case 51:
#line 860 "language.yy" /* yacc.c:1646  */
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
    typeplusdir.arc.type = EArcSymbol::ARC_SOLID;
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
#line 3693 "language.cc" /* yacc.c:1646  */
    break;

  case 52:
#line 894 "language.yy" /* yacc.c:1646  */
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
    (yyval.arcbase) = new ArcDivider(EArcSymbol::DIV_VSPACE, &msc);
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
  #endif
}
#line 3715 "language.cc" /* yacc.c:1646  */
    break;

  case 53:
#line 912 "language.yy" /* yacc.c:1646  */
    {
    //Here we have a colon label added: this is a divider
  #ifdef C_S_H_IS_COMPILED
  #else
    AttributeList *al = new AttributeList;
    al->Append(new Attribute("label", (yyvsp[0].str), MSC_POS((yyloc)), MSC_POS((yyloc)).IncStartCol()));
    (yyval.arcbase) = new ArcDivider(EArcSymbol::DIV_VSPACE, &msc);
    ((yyval.arcbase))->AddAttributeList(al);
  #endif
    free((yyvsp[0].str));
}
#line 3731 "language.cc" /* yacc.c:1646  */
    break;

  case 54:
#line 924 "language.yy" /* yacc.c:1646  */
    {
    //Here we have a colon label added: this is a divider
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0]))) 
        ArcDivider::AttributeNames(csh, false, false);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0]))) 
        ArcDivider::AttributeValues(csh.hintAttrName, csh, false, false);
  #else
    ((yyvsp[0].attriblist))->Prepend(new Attribute("label", (yyvsp[-1].str), MSC_POS((yylsp[-1])), MSC_POS((yylsp[-1])).IncStartCol()));
    (yyval.arcbase) = new ArcDivider(EArcSymbol::DIV_VSPACE, &msc);
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
#endif
    free((yyvsp[-1].str));
}
#line 3750 "language.cc" /* yacc.c:1646  */
    break;

  case 55:
#line 939 "language.yy" /* yacc.c:1646  */
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
    (yyval.arcbase) = new ArcDivider(EArcSymbol::DIV_VSPACE, &msc);
    ((yyval.arcbase))->AddAttributeList((yyvsp[-2].attriblist));
#endif
    free((yyvsp[-1].str));
}
#line 3774 "language.cc" /* yacc.c:1646  */
    break;

  case 56:
#line 959 "language.yy" /* yacc.c:1646  */
    {
    //Here we have a colon label added: this is a divider
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[-1]))) 
        ArcDivider::AttributeNames(csh, false, false);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[-1]))) 
        ArcDivider::AttributeValues(csh.hintAttrName, csh, false, false);
  #else
    ((yyvsp[-1].attriblist))->Append(new Attribute("label", (yyvsp[0].str), MSC_POS((yylsp[0])), MSC_POS((yylsp[0])).IncStartCol())); 
    (yyval.arcbase) = new ArcDivider(EArcSymbol::DIV_VSPACE, &msc);
    ((yyval.arcbase))->AddAttributeList((yyvsp[-1].attriblist));    
  #endif
    free((yyvsp[0].str));
}
#line 3793 "language.cc" /* yacc.c:1646  */
    break;

  case 57:
#line 974 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (new CommandEntity((yyvsp[0].entitylist), &msc, false));
    ((yyval.arcbase))->AddAttributeList(NULL);
  #endif
}
#line 3805 "language.cc" /* yacc.c:1646  */
    break;

  case 58:
#line 982 "language.yy" /* yacc.c:1646  */
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
#line 3825 "language.cc" /* yacc.c:1646  */
    break;

  case 59:
#line 998 "language.yy" /* yacc.c:1646  */
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
#line 3845 "language.cc" /* yacc.c:1646  */
    break;

  case 60:
#line 1014 "language.yy" /* yacc.c:1646  */
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
#line 3862 "language.cc" /* yacc.c:1646  */
    break;

  case 61:
#line 1027 "language.yy" /* yacc.c:1646  */
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
#line 3879 "language.cc" /* yacc.c:1646  */
    break;

  case 62:
#line 1040 "language.yy" /* yacc.c:1646  */
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
#line 3903 "language.cc" /* yacc.c:1646  */
    break;

  case 63:
#line 1060 "language.yy" /* yacc.c:1646  */
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
#line 3926 "language.cc" /* yacc.c:1646  */
    break;

  case 64:
#line 1079 "language.yy" /* yacc.c:1646  */
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
#line 3941 "language.cc" /* yacc.c:1646  */
    break;

  case 65:
#line 1090 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (yyvsp[0].arcboxseries); //to remove warning for downcast
  #endif
}
#line 3952 "language.cc" /* yacc.c:1646  */
    break;

  case 66:
#line 1097 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (yyvsp[0].arcpipeseries); //to remove warning for downcast
  #endif
}
#line 3963 "language.cc" /* yacc.c:1646  */
    break;

  case 67:
#line 1104 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = ((yyvsp[0].arcparallel));
	if ((yyvsp[0].arcparallel))
        ((yyval.arcbase))->AddAttributeList(NULL);
  #endif
}
#line 3976 "language.cc" /* yacc.c:1646  */
    break;

  case 68:
#line 1113 "language.yy" /* yacc.c:1646  */
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
#line 3993 "language.cc" /* yacc.c:1646  */
    break;

  case 69:
#line 1126 "language.yy" /* yacc.c:1646  */
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
#line 4011 "language.cc" /* yacc.c:1646  */
    break;

  case 70:
#line 1140 "language.yy" /* yacc.c:1646  */
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
#line 4031 "language.cc" /* yacc.c:1646  */
    break;

  case 71:
#line 1156 "language.yy" /* yacc.c:1646  */
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
#line 4053 "language.cc" /* yacc.c:1646  */
    break;

  case 72:
#line 1174 "language.yy" /* yacc.c:1646  */
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
#line 4073 "language.cc" /* yacc.c:1646  */
    break;

  case 73:
#line 1190 "language.yy" /* yacc.c:1646  */
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
#line 4095 "language.cc" /* yacc.c:1646  */
    break;

  case 74:
#line 1208 "language.yy" /* yacc.c:1646  */
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
#line 4112 "language.cc" /* yacc.c:1646  */
    break;

  case 75:
#line 1221 "language.yy" /* yacc.c:1646  */
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
#line 4131 "language.cc" /* yacc.c:1646  */
    break;

  case 76:
#line 1236 "language.yy" /* yacc.c:1646  */
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
#line 4149 "language.cc" /* yacc.c:1646  */
    break;

  case 77:
#line 1250 "language.yy" /* yacc.c:1646  */
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
#line 4170 "language.cc" /* yacc.c:1646  */
    break;

  case 78:
#line 1267 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } 
  #else
    (yyval.arcbase) = (new ArcDivider(EArcSymbol::DIV_VSPACE, &msc));
    ((yyval.arcbase))->AddAttributeList(NULL);
  #endif
    free((yyvsp[0].str));
}
#line 4188 "language.cc" /* yacc.c:1646  */
    break;

  case 79:
#line 1281 "language.yy" /* yacc.c:1646  */
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
    (yyval.arcbase) = (new ArcDivider(EArcSymbol::DIV_VSPACE, &msc));
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
  #endif
    free((yyvsp[-1].str));
}
#line 4209 "language.cc" /* yacc.c:1646  */
    break;

  case 80:
#line 1298 "language.yy" /* yacc.c:1646  */
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
    const EArcSymbol t = CaseInsensitiveEqual("title", (yyvsp[-1].str)) ? EArcSymbol::DIV_TITLE :
                         CaseInsensitiveEqual("subtitle", (yyvsp[-1].str)) ? EArcSymbol::DIV_SUBTITLE :
                         EArcSymbol::INVALID;
    (yyval.arcbase) = (new ArcDivider(t, &msc));
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
  #endif
    free((yyvsp[-1].str));
}
#line 4233 "language.cc" /* yacc.c:1646  */
    break;

  case 81:
#line 1318 "language.yy" /* yacc.c:1646  */
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
#line 4252 "language.cc" /* yacc.c:1646  */
    break;

  case 82:
#line 1333 "language.yy" /* yacc.c:1646  */
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
#line 4274 "language.cc" /* yacc.c:1646  */
    break;

  case 83:
#line 1351 "language.yy" /* yacc.c:1646  */
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
#line 4298 "language.cc" /* yacc.c:1646  */
    break;

  case 84:
#line 1371 "language.yy" /* yacc.c:1646  */
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
#line 4319 "language.cc" /* yacc.c:1646  */
    break;

  case 85:
#line 1388 "language.yy" /* yacc.c:1646  */
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
#line 4340 "language.cc" /* yacc.c:1646  */
    break;

  case 86:
#line 1405 "language.yy" /* yacc.c:1646  */
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
#line 4359 "language.cc" /* yacc.c:1646  */
    break;

  case 87:
#line 1420 "language.yy" /* yacc.c:1646  */
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
#line 4380 "language.cc" /* yacc.c:1646  */
    break;

  case 88:
#line 1437 "language.yy" /* yacc.c:1646  */
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
#line 4404 "language.cc" /* yacc.c:1646  */
    break;

  case 89:
#line 1457 "language.yy" /* yacc.c:1646  */
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
#line 4425 "language.cc" /* yacc.c:1646  */
    break;

  case 90:
#line 1474 "language.yy" /* yacc.c:1646  */
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
#line 4443 "language.cc" /* yacc.c:1646  */
    break;

  case 91:
#line 1488 "language.yy" /* yacc.c:1646  */
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
#line 4464 "language.cc" /* yacc.c:1646  */
    break;

  case 95:
#line 1508 "language.yy" /* yacc.c:1646  */
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
#line 4485 "language.cc" /* yacc.c:1646  */
    break;

  case 96:
#line 1525 "language.yy" /* yacc.c:1646  */
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
#line 4511 "language.cc" /* yacc.c:1646  */
    break;

  case 97:
#line 1547 "language.yy" /* yacc.c:1646  */
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
#line 4534 "language.cc" /* yacc.c:1646  */
    break;

  case 98:
#line 1566 "language.yy" /* yacc.c:1646  */
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
#line 4560 "language.cc" /* yacc.c:1646  */
    break;

  case 99:
#line 1588 "language.yy" /* yacc.c:1646  */
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
#line 4581 "language.cc" /* yacc.c:1646  */
    break;

  case 100:
#line 1605 "language.yy" /* yacc.c:1646  */
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
#line 4607 "language.cc" /* yacc.c:1646  */
    break;

  case 104:
#line 1631 "language.yy" /* yacc.c:1646  */
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
#line 4637 "language.cc" /* yacc.c:1646  */
    break;

  case 105:
#line 1657 "language.yy" /* yacc.c:1646  */
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
#line 4663 "language.cc" /* yacc.c:1646  */
    break;

  case 106:
#line 1679 "language.yy" /* yacc.c:1646  */
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
#line 4682 "language.cc" /* yacc.c:1646  */
    break;

  case 108:
#line 1697 "language.yy" /* yacc.c:1646  */
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
#line 4697 "language.cc" /* yacc.c:1646  */
    break;

  case 109:
#line 1708 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_ATTRVALUE);
  #else
    ((yyvsp[0].attriblist))->Append(new Attribute("space", atof((yyvsp[-1].str)), MSC_POS((yylsp[-1])), MSC_POS((yylsp[-1])), (yyvsp[-1].str)));
    (yyval.attriblist) = (yyvsp[0].attriblist);
  #endif
    free((yyvsp[-1].str));
}
#line 4711 "language.cc" /* yacc.c:1646  */
    break;

  case 112:
#line 1721 "language.yy" /* yacc.c:1646  */
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
#line 4727 "language.cc" /* yacc.c:1646  */
    break;

  case 113:
#line 1733 "language.yy" /* yacc.c:1646  */
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
#line 4742 "language.cc" /* yacc.c:1646  */
    break;

  case 114:
#line 1744 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.namerel) = NULL;
  #endif
}
#line 4755 "language.cc" /* yacc.c:1646  */
    break;

  case 115:
#line 1753 "language.yy" /* yacc.c:1646  */
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
#line 4773 "language.cc" /* yacc.c:1646  */
    break;

  case 116:
#line 1767 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
    csh.CheckEntityHintAt((yylsp[0]));
  #else
    (yyval.namerel) = new NamePair((yyvsp[0].str), MSC_POS((yylsp[0])), NULL, FileLineColRange());
  #endif
    free((yyvsp[0].str));
}
#line 4787 "language.cc" /* yacc.c:1646  */
    break;

  case 117:
#line 1779 "language.yy" /* yacc.c:1646  */
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
#line 4803 "language.cc" /* yacc.c:1646  */
    break;

  case 118:
#line 1791 "language.yy" /* yacc.c:1646  */
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
#line 4818 "language.cc" /* yacc.c:1646  */
    break;

  case 119:
#line 1802 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    csh.CheckHintAt((yylsp[0]), EHintSourceType::MARKER);
    csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::MARKER);
  #else
    (yyval.namerel) = NULL;
  #endif
}
#line 4832 "language.cc" /* yacc.c:1646  */
    break;

  case 120:
#line 1812 "language.yy" /* yacc.c:1646  */
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
#line 4850 "language.cc" /* yacc.c:1646  */
    break;

  case 125:
#line 1829 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[0].arcbase)) {
        (yyval.arclist) = (new ArcList)->Append((yyvsp[0].arcbase)); /* New list */
        //($1)->MakeMeLastNotable(); Do not make chart options notable
    } else
        (yyval.arclist) = NULL;
  #endif
}
#line 4864 "language.cc" /* yacc.c:1646  */
    break;

  case 126:
#line 1839 "language.yy" /* yacc.c:1646  */
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
#line 4887 "language.cc" /* yacc.c:1646  */
    break;

  case 127:
#line 1858 "language.yy" /* yacc.c:1646  */
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
#line 4904 "language.cc" /* yacc.c:1646  */
    break;

  case 129:
#line 1872 "language.yy" /* yacc.c:1646  */
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
#line 4921 "language.cc" /* yacc.c:1646  */
    break;

  case 130:
#line 1887 "language.yy" /* yacc.c:1646  */
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
#line 4944 "language.cc" /* yacc.c:1646  */
    break;

  case 131:
#line 1906 "language.yy" /* yacc.c:1646  */
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
#line 4967 "language.cc" /* yacc.c:1646  */
    break;

  case 132:
#line 1925 "language.yy" /* yacc.c:1646  */
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
#line 4990 "language.cc" /* yacc.c:1646  */
    break;

  case 133:
#line 1944 "language.yy" /* yacc.c:1646  */
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
#line 5016 "language.cc" /* yacc.c:1646  */
    break;

  case 134:
#line 1966 "language.yy" /* yacc.c:1646  */
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
#line 5039 "language.cc" /* yacc.c:1646  */
    break;

  case 135:
#line 1985 "language.yy" /* yacc.c:1646  */
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
#line 5065 "language.cc" /* yacc.c:1646  */
    break;

  case 136:
#line 2007 "language.yy" /* yacc.c:1646  */
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
#line 5088 "language.cc" /* yacc.c:1646  */
    break;

  case 137:
#line 2027 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.entitylist) = ((yyvsp[0].entitylist));
  #endif
}
#line 5098 "language.cc" /* yacc.c:1646  */
    break;

  case 138:
#line 2033 "language.yy" /* yacc.c:1646  */
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
#line 5113 "language.cc" /* yacc.c:1646  */
    break;

  case 139:
#line 2044 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_COMMA);
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.entitylist) = ((yyvsp[-1].entitylist));
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Expecting an entity here.");
  #endif
}
#line 5127 "language.cc" /* yacc.c:1646  */
    break;

  case 141:
#line 2059 "language.yy" /* yacc.c:1646  */
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
#line 5147 "language.cc" /* yacc.c:1646  */
    break;

  case 142:
#line 2075 "language.yy" /* yacc.c:1646  */
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
#line 5163 "language.cc" /* yacc.c:1646  */
    break;

  case 143:
#line 2087 "language.yy" /* yacc.c:1646  */
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
#line 5183 "language.cc" /* yacc.c:1646  */
    break;

  case 144:
#line 2103 "language.yy" /* yacc.c:1646  */
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
#line 5199 "language.cc" /* yacc.c:1646  */
    break;

  case 145:
#line 2116 "language.yy" /* yacc.c:1646  */
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
#line 5221 "language.cc" /* yacc.c:1646  */
    break;

  case 146:
#line 2134 "language.yy" /* yacc.c:1646  */
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
#line 5240 "language.cc" /* yacc.c:1646  */
    break;

  case 147:
#line 2149 "language.yy" /* yacc.c:1646  */
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
#line 5262 "language.cc" /* yacc.c:1646  */
    break;

  case 148:
#line 2167 "language.yy" /* yacc.c:1646  */
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
#line 5281 "language.cc" /* yacc.c:1646  */
    break;

  case 150:
#line 2184 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_COMMA);
    if (csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_VALUE)) {
        csh.AddStylesToHints(true, true);
        csh.hintStatus = HINT_READY;
    }
  #endif
}
#line 5295 "language.cc" /* yacc.c:1646  */
    break;

  case 151:
#line 2194 "language.yy" /* yacc.c:1646  */
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
#line 5311 "language.cc" /* yacc.c:1646  */
    break;

  case 152:
#line 2207 "language.yy" /* yacc.c:1646  */
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
#line 5363 "language.cc" /* yacc.c:1646  */
    break;

  case 153:
#line 2255 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
	csh.AddCSH_ErrorAfter((yyloc), "Missing attribute definitons in square brackets ('[' and ']').");
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing attribute definitons in square brackets ('[' and ']').");
  #endif
    delete((yyvsp[0].stringlist));
}
#line 5376 "language.cc" /* yacc.c:1646  */
    break;

  case 154:
#line 2266 "language.yy" /* yacc.c:1646  */
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
#line 5400 "language.cc" /* yacc.c:1646  */
    break;

  case 155:
#line 2286 "language.yy" /* yacc.c:1646  */
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
#line 5419 "language.cc" /* yacc.c:1646  */
    break;

  case 156:
#line 2301 "language.yy" /* yacc.c:1646  */
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
#line 5438 "language.cc" /* yacc.c:1646  */
    break;

  case 157:
#line 2316 "language.yy" /* yacc.c:1646  */
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
#line 5463 "language.cc" /* yacc.c:1646  */
    break;

  case 158:
#line 2337 "language.yy" /* yacc.c:1646  */
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
#line 5483 "language.cc" /* yacc.c:1646  */
    break;

  case 159:
#line 2354 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_ATTRVALUE);
    csh.AddCSH_ErrorAfter((yyloc), ("Here should come a shape definition beginning with '{'. Ignoring this malformed shape definition for '"+string((yyvsp[0].str) ? (yyvsp[0].str) : "") +"'.").c_str());
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Here should come a shape definition beginning with '{'. Ignoring this malformed shape definition for '"+string((yyvsp[0].str) ? (yyvsp[0].str) : "") +"'.");
  #endif	
  free((yyvsp[0].str));
}
#line 5497 "language.cc" /* yacc.c:1646  */
    break;

  case 160:
#line 2364 "language.yy" /* yacc.c:1646  */
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
#line 5512 "language.cc" /* yacc.c:1646  */
    break;

  case 161:
#line 2375 "language.yy" /* yacc.c:1646  */
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
#line 5533 "language.cc" /* yacc.c:1646  */
    break;

  case 162:
#line 2392 "language.yy" /* yacc.c:1646  */
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
#line 5552 "language.cc" /* yacc.c:1646  */
    break;

  case 163:
#line 2407 "language.yy" /* yacc.c:1646  */
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
#line 5572 "language.cc" /* yacc.c:1646  */
    break;

  case 164:
#line 2424 "language.yy" /* yacc.c:1646  */
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
#line 5588 "language.cc" /* yacc.c:1646  */
    break;

  case 165:
#line 2436 "language.yy" /* yacc.c:1646  */
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
#line 5605 "language.cc" /* yacc.c:1646  */
    break;

  case 166:
#line 2449 "language.yy" /* yacc.c:1646  */
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
#line 5620 "language.cc" /* yacc.c:1646  */
    break;

  case 167:
#line 2460 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[0]), "Only numbers can come after shape commands.");
  #else
  #endif	
}
#line 5631 "language.cc" /* yacc.c:1646  */
    break;

  case 168:
#line 2468 "language.yy" /* yacc.c:1646  */
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
#line 5651 "language.cc" /* yacc.c:1646  */
    break;

  case 169:
#line 2484 "language.yy" /* yacc.c:1646  */
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
#line 5683 "language.cc" /* yacc.c:1646  */
    break;

  case 170:
#line 2512 "language.yy" /* yacc.c:1646  */
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
#line 5711 "language.cc" /* yacc.c:1646  */
    break;

  case 171:
#line 2536 "language.yy" /* yacc.c:1646  */
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
#line 5741 "language.cc" /* yacc.c:1646  */
    break;

  case 172:
#line 2562 "language.yy" /* yacc.c:1646  */
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
#line 5773 "language.cc" /* yacc.c:1646  */
    break;

  case 173:
#line 2590 "language.yy" /* yacc.c:1646  */
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
#line 5807 "language.cc" /* yacc.c:1646  */
    break;

  case 174:
#line 2620 "language.yy" /* yacc.c:1646  */
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
#line 5843 "language.cc" /* yacc.c:1646  */
    break;

  case 176:
#line 2654 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_COMMA);
    if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_NAME)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
	}
  #endif
}
#line 5857 "language.cc" /* yacc.c:1646  */
    break;

  case 177:
#line 2664 "language.yy" /* yacc.c:1646  */
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
#line 5874 "language.cc" /* yacc.c:1646  */
    break;

  case 180:
#line 2680 "language.yy" /* yacc.c:1646  */
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
#line 5900 "language.cc" /* yacc.c:1646  */
    break;

  case 181:
#line 2702 "language.yy" /* yacc.c:1646  */
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
#line 5927 "language.cc" /* yacc.c:1646  */
    break;

  case 182:
#line 2725 "language.yy" /* yacc.c:1646  */
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
#line 5949 "language.cc" /* yacc.c:1646  */
    break;

  case 183:
#line 2743 "language.yy" /* yacc.c:1646  */
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
#line 5967 "language.cc" /* yacc.c:1646  */
    break;

  case 184:
#line 2760 "language.yy" /* yacc.c:1646  */
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
#line 5995 "language.cc" /* yacc.c:1646  */
    break;

  case 185:
#line 2784 "language.yy" /* yacc.c:1646  */
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
#line 6021 "language.cc" /* yacc.c:1646  */
    break;

  case 186:
#line 2808 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_BRACE);
    csh.PushContext(true);
  #else
    //push empty color & style sets for design definition
    msc.PushContext(true);
  #endif
}
#line 6035 "language.cc" /* yacc.c:1646  */
    break;

  case 188:
#line 2820 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_SEMICOLON);
    if (csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::LINE_START)) {
        csh.AddDesignLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
#endif
}
#line 6049 "language.cc" /* yacc.c:1646  */
    break;

  case 189:
#line 2831 "language.yy" /* yacc.c:1646  */
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
#line 6067 "language.cc" /* yacc.c:1646  */
    break;

  case 190:
#line 2845 "language.yy" /* yacc.c:1646  */
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
#line 6088 "language.cc" /* yacc.c:1646  */
    break;

  case 191:
#line 2862 "language.yy" /* yacc.c:1646  */
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
#line 6106 "language.cc" /* yacc.c:1646  */
    break;

  case 192:
#line 2876 "language.yy" /* yacc.c:1646  */
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
#line 6127 "language.cc" /* yacc.c:1646  */
    break;

  case 195:
#line 2896 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_COMMA);
    if (csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    }   
  #endif
}
#line 6141 "language.cc" /* yacc.c:1646  */
    break;

  case 196:
#line 2906 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_COMMA);
    if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    }   
  #endif
}
#line 6155 "language.cc" /* yacc.c:1646  */
    break;

  case 197:
#line 2916 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[0]), "Extra stuff after design options. Maybe missing a comma?");
  #endif
}
#line 6165 "language.cc" /* yacc.c:1646  */
    break;

  case 198:
#line 2923 "language.yy" /* yacc.c:1646  */
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
#line 6188 "language.cc" /* yacc.c:1646  */
    break;

  case 199:
#line 2942 "language.yy" /* yacc.c:1646  */
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
#line 6211 "language.cc" /* yacc.c:1646  */
    break;

  case 200:
#line 2961 "language.yy" /* yacc.c:1646  */
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
#line 6232 "language.cc" /* yacc.c:1646  */
    break;

  case 201:
#line 2978 "language.yy" /* yacc.c:1646  */
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
#line 6258 "language.cc" /* yacc.c:1646  */
    break;

  case 202:
#line 3000 "language.yy" /* yacc.c:1646  */
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
#line 6284 "language.cc" /* yacc.c:1646  */
    break;

  case 203:
#line 3022 "language.yy" /* yacc.c:1646  */
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
#line 6305 "language.cc" /* yacc.c:1646  */
    break;

  case 204:
#line 3039 "language.yy" /* yacc.c:1646  */
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
#line 6326 "language.cc" /* yacc.c:1646  */
    break;

  case 205:
#line 3058 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[0].arclist))
        (yyval.arcparallel) = new ArcParallel(&msc, (yyvsp[0].arclist), NULL);
    else
        (yyval.arcparallel) = NULL;
  #endif
}
#line 6339 "language.cc" /* yacc.c:1646  */
    break;

  case 206:
#line 3067 "language.yy" /* yacc.c:1646  */
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
#line 6359 "language.cc" /* yacc.c:1646  */
    break;

  case 207:
#line 3083 "language.yy" /* yacc.c:1646  */
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
#line 6374 "language.cc" /* yacc.c:1646  */
    break;

  case 208:
#line 3094 "language.yy" /* yacc.c:1646  */
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
#line 6395 "language.cc" /* yacc.c:1646  */
    break;

  case 209:
#line 3111 "language.yy" /* yacc.c:1646  */
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
#line 6415 "language.cc" /* yacc.c:1646  */
    break;

  case 210:
#line 3133 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    ((yyvsp[0].arcbox))->SetLineEnd(MSC_POS((yyloc)));
    (yyval.arcboxseries) = new ArcBoxSeries((yyvsp[0].arcbox));
  #endif
}
#line 6428 "language.cc" /* yacc.c:1646  */
    break;

  case 211:
#line 3142 "language.yy" /* yacc.c:1646  */
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
#line 6450 "language.cc" /* yacc.c:1646  */
    break;

  case 212:
#line 3160 "language.yy" /* yacc.c:1646  */
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
#line 6469 "language.cc" /* yacc.c:1646  */
    break;

  case 213:
#line 3176 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[0].arcbox))->SetLineEnd(MSC_POS((yylsp[0])));
    (yyval.arcboxseries) = ((yyvsp[-1].arcboxseries))->AddBox((yyvsp[0].arcbox));
    ((yyvsp[0].arcbox))->AddAttributeList(NULL); //should come after AddBox
  #endif
}
#line 6481 "language.cc" /* yacc.c:1646  */
    break;

  case 214:
#line 3184 "language.yy" /* yacc.c:1646  */
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
#line 6501 "language.cc" /* yacc.c:1646  */
    break;

  case 215:
#line 3200 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    ((yyvsp[-1].arcbox))->AddArcList((yyvsp[0].arclist))->SetLineEnd(MSC_POS((yylsp[-1])));
    (yyval.arcboxseries) = ((yyvsp[-2].arcboxseries))->AddBox((yyvsp[-1].arcbox));
    ((yyvsp[-1].arcbox))->AddAttributeList(NULL); //should come after AddBox
  #endif
}
#line 6515 "language.cc" /* yacc.c:1646  */
    break;

  case 216:
#line 3210 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    if ((yyvsp[-1].arcboxseries)) {
        ArcBox *temp = new ArcBox(EArcSymbol::BOX_UNDETERMINED_FOLLOW, NULL, MSC_POS((yylsp[-1])), NULL, MSC_POS((yylsp[-1])), &msc);
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
#line 6538 "language.cc" /* yacc.c:1646  */
    break;

  case 217:
#line 3229 "language.yy" /* yacc.c:1646  */
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
#line 6557 "language.cc" /* yacc.c:1646  */
    break;

  case 218:
#line 3244 "language.yy" /* yacc.c:1646  */
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
        ArcBox *temp = new ArcBox(EArcSymbol::BOX_UNDETERMINED_FOLLOW, NULL, MSC_POS((yylsp[-2])), NULL, MSC_POS((yylsp[-2])), &msc);
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
#line 6586 "language.cc" /* yacc.c:1646  */
    break;

  case 219:
#line 3269 "language.yy" /* yacc.c:1646  */
    {
#ifndef C_S_H_IS_COMPILED
    if ((yyvsp[0].arcbox)) {
        ((yyvsp[0].arcbox))->AddAttributeList(NULL);
        ((yyvsp[0].arcbox))->SetLineEnd(MSC_POS((yyloc)));
        (yyval.arcboxseries) = new ArcBoxSeries((yyvsp[0].arcbox));
    } else
        (yyval.arcboxseries) = NULL;
#endif
}
#line 6601 "language.cc" /* yacc.c:1646  */
    break;

  case 220:
#line 3280 "language.yy" /* yacc.c:1646  */
    {
#ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, (yylsp[0])))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, (yylsp[0])))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
#else
    if ((yyvsp[-1].arcbox)) {
        ((yyvsp[-1].arcbox))->AddAttributeList((yyvsp[0].attriblist));
        ((yyvsp[-1].arcbox))->SetLineEnd(MSC_POS((yyloc)));
        (yyval.arcboxseries) = new ArcBoxSeries((yyvsp[-1].arcbox));
    } else
        (yyval.arcboxseries) = NULL;
#endif
}
#line 6621 "language.cc" /* yacc.c:1646  */
    break;

  case 221:
#line 3298 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[0].arcbox))->AddAttributeList(NULL);
    ((yyvsp[0].arcbox))->SetLineEnd(MSC_POS((yyloc)), false);
    (yyval.arcbox) = (yyvsp[0].arcbox);
  #endif
}
#line 6633 "language.cc" /* yacc.c:1646  */
    break;

  case 222:
#line 3306 "language.yy" /* yacc.c:1646  */
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
#line 6650 "language.cc" /* yacc.c:1646  */
    break;

  case 223:
#line 3319 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[-1].arcbox))->AddAttributeList(NULL);
    ((yyvsp[-1].arcbox))->SetLineEnd(MSC_POS((yylsp[-1])), false);
    (yyval.arcbox) = ((yyvsp[-1].arcbox))->AddArcList((yyvsp[0].arclist));
  #endif
}
#line 6662 "language.cc" /* yacc.c:1646  */
    break;

  case 224:
#line 3327 "language.yy" /* yacc.c:1646  */
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
#line 6679 "language.cc" /* yacc.c:1646  */
    break;

  case 225:
#line 3342 "language.yy" /* yacc.c:1646  */
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
#line 6701 "language.cc" /* yacc.c:1646  */
    break;

  case 226:
#line 3360 "language.yy" /* yacc.c:1646  */
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
#line 6724 "language.cc" /* yacc.c:1646  */
    break;

  case 227:
#line 3379 "language.yy" /* yacc.c:1646  */
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
#line 6743 "language.cc" /* yacc.c:1646  */
    break;

  case 228:
#line 3394 "language.yy" /* yacc.c:1646  */
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
#line 6768 "language.cc" /* yacc.c:1646  */
    break;

  case 229:
#line 3416 "language.yy" /* yacc.c:1646  */
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
#line 6783 "language.cc" /* yacc.c:1646  */
    break;

  case 230:
#line 3427 "language.yy" /* yacc.c:1646  */
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
#line 6803 "language.cc" /* yacc.c:1646  */
    break;

  case 231:
#line 3443 "language.yy" /* yacc.c:1646  */
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
#line 6828 "language.cc" /* yacc.c:1646  */
    break;

  case 233:
#line 3466 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintBetween((yylsp[-1]), (yylsp[0]));
  #else
    (yyval.arcpipeseries) = ((yyvsp[-1].arcpipeseries))->AddArcList((yyvsp[0].arclist));
  #endif
}
#line 6840 "language.cc" /* yacc.c:1646  */
    break;

  case 236:
#line 3477 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[-2]));
    csh.AddCSH_EntityName((yylsp[-2]), (yyvsp[-2].str));
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcbox) = new ArcBox((yyvsp[-1].arcsymbol), (yyvsp[-2].str), MSC_POS((yylsp[-2])), (yyvsp[0].str), MSC_POS((yylsp[0])), &msc);
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 6858 "language.cc" /* yacc.c:1646  */
    break;

  case 237:
#line 3491 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcbox) = new ArcBox((yyvsp[-1].arcsymbol), NULL, MSC_POS((yylsp[-1])), (yyvsp[0].str), MSC_POS((yylsp[0])), &msc);
  #endif
    free((yyvsp[0].str));
}
#line 6873 "language.cc" /* yacc.c:1646  */
    break;

  case 238:
#line 3502 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[-1]));
    csh.AddCSH_EntityName((yylsp[-1]), (yyvsp[-1].str));
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcbox) = new ArcBox((yyvsp[0].arcsymbol), (yyvsp[-1].str), MSC_POS((yylsp[-1])), NULL, MSC_POS((yylsp[0])), &msc);
  #endif
    free((yyvsp[-1].str));
}
#line 6889 "language.cc" /* yacc.c:1646  */
    break;

  case 239:
#line 3514 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcbox) = new ArcBox((yyvsp[0].arcsymbol), NULL, MSC_POS((yylsp[0])), NULL, MSC_POS((yylsp[0])), &msc);
  #endif
}
#line 6902 "language.cc" /* yacc.c:1646  */
    break;

  case 244:
#line 3526 "language.yy" /* yacc.c:1646  */
    {
    //mscgen compatibility: a box with no content or 
#ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[-2]));
    csh.AddCSH_EntityName((yylsp[-2]), (yyvsp[-2].str));
    if (csh.mscgen_compat == EMscgenCompat::FORCE_MSCGEN)
        csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    else
        csh.AddCSH_Error_Mscgen((yylsp[-1]));
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
#else
    ArcBox::Emscgen_compat c = ArcBox::MSCGEN_COMPAT_NONE;
    if (CaseInsensitiveEqual((yyvsp[-1].str), "rbox")) c = ArcBox::MSCGEN_COMPAT_RBOX;
    else if (CaseInsensitiveEqual((yyvsp[-1].str), "abox")) c = ArcBox::MSCGEN_COMPAT_ABOX;
    else if (CaseInsensitiveEqual((yyvsp[-1].str), "box")) c = ArcBox::MSCGEN_COMPAT_BOX;
    else if (CaseInsensitiveEqual((yyvsp[-1].str), "note")) c = ArcBox::MSCGEN_COMPAT_NOTE;
    (yyval.arcbox) = new ArcBox(c, (yyvsp[-2].str), MSC_POS((yylsp[-2])), (yyvsp[0].str), MSC_POS((yylsp[0])), &msc);
    if (msc.mscgen_compat != EMscgenCompat::FORCE_MSCGEN)
        msc.Error.WarnMscgen(MSC_POS((yylsp[-1])).start, "Unrecognized box symbol.");
#endif
    free((yyvsp[-2].str));
    free((yyvsp[-1].str));
    free((yyvsp[0].str));
}
#line 6932 "language.cc" /* yacc.c:1646  */
    break;

  case 245:
#line 3553 "language.yy" /* yacc.c:1646  */
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
#line 6954 "language.cc" /* yacc.c:1646  */
    break;

  case 246:
#line 3571 "language.yy" /* yacc.c:1646  */
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
#line 6978 "language.cc" /* yacc.c:1646  */
    break;

  case 247:
#line 3591 "language.yy" /* yacc.c:1646  */
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
#line 7002 "language.cc" /* yacc.c:1646  */
    break;

  case 248:
#line 3611 "language.yy" /* yacc.c:1646  */
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
#line 7027 "language.cc" /* yacc.c:1646  */
    break;

  case 249:
#line 3632 "language.yy" /* yacc.c:1646  */
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
#line 7050 "language.cc" /* yacc.c:1646  */
    break;

  case 250:
#line 3651 "language.yy" /* yacc.c:1646  */
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
#line 7075 "language.cc" /* yacc.c:1646  */
    break;

  case 251:
#line 3672 "language.yy" /* yacc.c:1646  */
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
    switch ((yyvsp[0].arcsymbol)) {
    default:
        _ASSERT(0);
    case EArcSymbol::BOX_SOLID:
        (yyval.vertxpos) = new VertXPos(msc, (yyvsp[-1].str), MSC_POS((yylsp[-1])), VertXPos::POS_LEFT_BY);
        break;
    case EArcSymbol::BOX_DASHED:
        (yyval.vertxpos) = new VertXPos(msc, (yyvsp[-1].str), MSC_POS((yylsp[-1])), VertXPos::POS_RIGHT_BY);
        break;
    case EArcSymbol::BOX_DOTTED:
        msc.Error.Error(MSC_POS((yylsp[0])).start,
                        "unexpected '..', expected '-', '--', '+' or '++'."
                        " Ignoring vertical."); 
        (yyval.vertxpos) = NULL;
        break;
    case EArcSymbol::BOX_DOUBLE:
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
#line 7119 "language.cc" /* yacc.c:1646  */
    break;

  case 252:
#line 3712 "language.yy" /* yacc.c:1646  */
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
#line 7145 "language.cc" /* yacc.c:1646  */
    break;

  case 253:
#line 3734 "language.yy" /* yacc.c:1646  */
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
#line 7171 "language.cc" /* yacc.c:1646  */
    break;

  case 254:
#line 3756 "language.yy" /* yacc.c:1646  */
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
#line 7199 "language.cc" /* yacc.c:1646  */
    break;

  case 255:
#line 3780 "language.yy" /* yacc.c:1646  */
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
#line 7227 "language.cc" /* yacc.c:1646  */
    break;

  case 256:
#line 3804 "language.yy" /* yacc.c:1646  */
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
#line 7249 "language.cc" /* yacc.c:1646  */
    break;

  case 257:
#line 3824 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
	(yyval.arctypeplusdir) = 0; //dummy to supress warning
  #else
    ((yyval.arctypeplusdir)).arc.type = (yyvsp[0].arcsymbol);
	((yyval.arctypeplusdir)).arc.lost = EArrowLost::NOT;
	((yyval.arctypeplusdir)).dir = EDirType::INDETERMINATE;
  #endif
}
#line 7264 "language.cc" /* yacc.c:1646  */
    break;

  case 258:
#line 3835 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    csh.CheckEntityHintBetween((yylsp[-1]), (yylsp[0]));
	(yyval.arctypeplusdir) = 0; //dummy to supress warning	
  #else
    ((yyval.arctypeplusdir)).arc.type = (yyvsp[0].arcsymbol);
	((yyval.arctypeplusdir)).arc.lost = EArrowLost::AT_SRC;
    ((yyval.arctypeplusdir)).arc.lost_pos.SetFrom(MSC_POS((yylsp[-1])));
	((yyval.arctypeplusdir)).dir = EDirType::INDETERMINATE;
  #endif
}
#line 7281 "language.cc" /* yacc.c:1646  */
    break;

  case 259:
#line 3848 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
	(yyval.arctypeplusdir) = 0; //dummy to supress warning	
  #else
    ((yyval.arctypeplusdir)).arc.type = (yyvsp[-1].arcsymbol);
	((yyval.arctypeplusdir)).arc.lost = EArrowLost::AT_SRC;
    ((yyval.arctypeplusdir)).arc.lost_pos.SetFrom(MSC_POS((yylsp[0])));
	((yyval.arctypeplusdir)).dir = EDirType::INDETERMINATE;
  #endif
}
#line 7297 "language.cc" /* yacc.c:1646  */
    break;

  case 260:
#line 3860 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_SYMBOL);
    csh.AddCSH_Error((yylsp[0]), MULTIPLE_ASTERISK_ERROR_MSG);
    csh.CheckEntityHintBetween((yylsp[-2]), (yylsp[-1]));
    (yyval.arctypeplusdir) = 0; //dummy to supress warning	
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).start, "Only one loss can be specified. Ignoring second asterisk ('*').");
    ((yyval.arctypeplusdir)).arc.type = (yyvsp[-1].arcsymbol);
    ((yyval.arctypeplusdir)).arc.lost = EArrowLost::AT_SRC;
    ((yyval.arctypeplusdir)).arc.lost_pos.SetFrom(MSC_POS((yylsp[-2])));
	((yyval.arctypeplusdir)).dir = EDirType::INDETERMINATE;
  #endif
}
#line 7316 "language.cc" /* yacc.c:1646  */
    break;

  case 261:
#line 3875 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
	(yyval.arctypeplusdir) = 0; //dummy to supress warning
  #else
    ((yyval.arctypeplusdir)).arc = (yyvsp[0].arcsegdata);
	((yyval.arctypeplusdir)).dir = EDirType::LEFT;
  #endif
}
#line 7329 "language.cc" /* yacc.c:1646  */
    break;

  case 262:
#line 3884 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
	(yyval.arctypeplusdir) = 0; //dummy to supress warning
  #else
    ((yyval.arctypeplusdir)).arc = (yyvsp[0].arcsegdata);
	((yyval.arctypeplusdir)).dir = EDirType::RIGHT;
  #endif
}
#line 7342 "language.cc" /* yacc.c:1646  */
    break;

  case 263:
#line 3893 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
	(yyval.arctypeplusdir) = 0; //dummy to supress warning
  #else
    ((yyval.arctypeplusdir)).arc = (yyvsp[0].arcsegdata);
	((yyval.arctypeplusdir)).dir = EDirType::BIDIR;
  #endif
}
#line 7355 "language.cc" /* yacc.c:1646  */
    break;

  case 264:
#line 3903 "language.yy" /* yacc.c:1646  */
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
#line 7372 "language.cc" /* yacc.c:1646  */
    break;

  case 265:
#line 3916 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_MARKERNAME);
    csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), EHintSourceType::MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[-1].arctypeplusdir), MARKER_HERE_STR, (yyvsp[0].str), &msc);
  #endif
    free((yyvsp[0].str));
}
#line 7386 "language.cc" /* yacc.c:1646  */
    break;

  case 266:
#line 3926 "language.yy" /* yacc.c:1646  */
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
#line 7401 "language.cc" /* yacc.c:1646  */
    break;

  case 267:
#line 3937 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, EHintSourceType::MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[0].arctypeplusdir), MARKER_HERE_STR, MARKER_HERE_STR, &msc);
  #endif
}
#line 7413 "language.cc" /* yacc.c:1646  */
    break;

  case 268:
#line 3945 "language.yy" /* yacc.c:1646  */
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
#line 7429 "language.cc" /* yacc.c:1646  */
    break;

  case 269:
#line 3957 "language.yy" /* yacc.c:1646  */
    {free((yyvsp[-1].str)); (yyval.arcvertarrow)=NULL;}
#line 7435 "language.cc" /* yacc.c:1646  */
    break;

  case 270:
#line 3958 "language.yy" /* yacc.c:1646  */
    {free((yyvsp[-3].str)); free((yyvsp[-1].str)); (yyval.arcvertarrow)=NULL;}
#line 7441 "language.cc" /* yacc.c:1646  */
    break;

  case 271:
#line 3962 "language.yy" /* yacc.c:1646  */
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
#line 7456 "language.cc" /* yacc.c:1646  */
    break;

  case 272:
#line 3973 "language.yy" /* yacc.c:1646  */
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
#line 7475 "language.cc" /* yacc.c:1646  */
    break;

  case 273:
#line 3988 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    if ((yyvsp[0].vertxpos)) {
		ArcTypePlusDir typeplusdir;
		typeplusdir.arc.type = EArcSymbol::ARC_SOLID;
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
#line 7496 "language.cc" /* yacc.c:1646  */
    break;

  case 274:
#line 4006 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
  #else
    (yyval.arcbase) = new ArcDivider((yyvsp[0].arcsymbol), &msc);
  #endif
}
#line 7508 "language.cc" /* yacc.c:1646  */
    break;

  case 275:
#line 4014 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = ((yyvsp[-2].arcarrow))->AddLostPos((yyvsp[0].vertxpos), MSC_POS2((yylsp[-1]), (yylsp[0])));
  #endif
   free((yyvsp[-1].str));
}
#line 7521 "language.cc" /* yacc.c:1646  */
    break;

  case 276:
#line 4023 "language.yy" /* yacc.c:1646  */
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
#line 7540 "language.cc" /* yacc.c:1646  */
    break;

  case 277:
#line 4038 "language.yy" /* yacc.c:1646  */
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
#line 7558 "language.cc" /* yacc.c:1646  */
    break;

  case 281:
#line 4054 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[-3]));
    csh.AddCSH_EntityName((yylsp[-3]), (yyvsp[-3].str));
    if (csh.mscgen_compat == EMscgenCompat::FORCE_MSCGEN) {
        if (((yylsp[-2])).last_pos+1 == ((yylsp[-1])).first_pos) {
            csh.AddCSH((yylsp[-2]) + (yylsp[-1]), COLOR_SYMBOL);
            csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
        } else {
            csh.AddCSH_Error((yylsp[-2]) + (yylsp[0]), "Not supported arrow symbol ('-').");
        }
    } else {
        csh.AddCSH_Error((yylsp[-2]) + (yylsp[0]), "Not supported arrow symbol ('-').");
    }
  #else
    if (msc.mscgen_compat == EMscgenCompat::FORCE_MSCGEN) {
        if (MSC_POS((yylsp[-2])).end.NextChar() == MSC_POS((yylsp[-1])).start) {
            ArrowSegmentData data;
            data.type = EArcSymbol::ARC_SOLID;
            data.lost = EArrowLost::AT_DST;
            data.lost_pos.SetFrom(MSC_POS((yylsp[-1])));
            (yyval.arcarrow) = msc.CreateArcArrow(data, (yyvsp[-3].str), MSC_POS((yylsp[-3])), (yyvsp[0].str), true, MSC_POS((yylsp[0])));
        } else {
            msc.Error.Error(MSC_POS((yylsp[-2])).start, "Unsupported arrow symbol '-'.",
                "Perhaps remove the whitespace after?");
            (yyval.arcarrow) = NULL;
        }
    } else {
        if (MSC_POS((yylsp[-2])).end.NextChar() == MSC_POS((yylsp[-1])).start) {
            msc.Error.Error(MSC_POS((yylsp[-2])).start, "This arrow symbol ('-x') is supported only in mscgen compatibility mode.",
                "Use an asterisk '*' instead to express a lost message (like '-> *').");
            (yyval.arcarrow) = NULL;
        } else {
            msc.Error.Error(MSC_POS((yylsp[-2])).start, "Unsupported arrow symbol '-'.");
            (yyval.arcarrow) = NULL;
        }
    }
  #endif
    free((yyvsp[-3].str));
    free((yyvsp[-1].str));
    free((yyvsp[0].str));
}
#line 7605 "language.cc" /* yacc.c:1646  */
    break;

  case 282:
#line 4097 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[-3]));
    csh.AddCSH_EntityName((yylsp[-3]), (yyvsp[-3].str));
    if (csh.mscgen_compat == EMscgenCompat::FORCE_MSCGEN) {
        if (((yylsp[-2])).last_pos+1 == ((yylsp[-1])).first_pos) {
            csh.AddCSH((yylsp[-2]) + (yylsp[-1]), COLOR_SYMBOL);
            csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
        } else {
            csh.AddCSH_Error((yylsp[-2]) + (yylsp[0]), "Missing arrow symbol before 'x'.");
        }
    } else {
        if (((yylsp[-2])).last_pos+1 == ((yylsp[-1])).first_pos) {
            csh.AddCSH_Error((yylsp[-2]) + (yylsp[0]), "Not supported arrow symbol ('x-').");
        } else {
            csh.AddCSH_Error((yylsp[-2]) + (yylsp[0]), "Missing arrow symbol before 'x'.");
        }
    }
  #else
    if (msc.mscgen_compat == EMscgenCompat::FORCE_MSCGEN) {
        if (MSC_POS((yylsp[-2])).end.NextChar() == MSC_POS((yylsp[-1])).start) {
            ArrowSegmentData data;
            data.type = EArcSymbol::ARC_SOLID;
            data.lost = EArrowLost::AT_DST;
            data.lost_pos.SetFrom(MSC_POS((yylsp[-2])));
            (yyval.arcarrow) = msc.CreateArcArrow(data, (yyvsp[0].str), MSC_POS((yylsp[0])), (yyvsp[-3].str), false, MSC_POS((yylsp[-3])));
        } else {
            msc.Error.Error(MSC_POS((yylsp[-2])).start, "Missing arrow symbol before 'x'.",
                "Perhaps remove the whitespace after?");
            (yyval.arcarrow) = NULL;
        }
    } else {
        if (MSC_POS((yylsp[-2])).end.NextChar() == MSC_POS((yylsp[-1])).start) {
            msc.Error.Error(MSC_POS((yylsp[-2])).start, "This arrow symbol ('x-') is supported only in mscgen compatibility mode.",
                "Use an asterisk '*' instead to express a lost message (like '-> *').");
            (yyval.arcarrow) = NULL;
        } else {
            msc.Error.Error(MSC_POS((yylsp[-2])).start, "Missing arrow symbol before 'x'.");
            (yyval.arcarrow) = NULL;
        }
    }
  #endif
    free((yyvsp[-3].str));
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 7656 "language.cc" /* yacc.c:1646  */
    break;

  case 283:
#line 4144 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[-2]));
    csh.AddCSH_EntityName((yylsp[-2]), (yyvsp[-2].str));
    if (csh.mscgen_compat == EMscgenCompat::FORCE_MSCGEN &&
        ((yylsp[-1])).last_pos+1 == ((yylsp[0])).first_pos) {
        csh.AddCSH((yylsp[-1]) + (yylsp[0]), COLOR_SYMBOL);
        csh.AddCSH_ErrorAfter((yylsp[-1]), "Missing an entity name.");
    } else {
        csh.AddCSH_Error((yylsp[-1]), "Not supported arrow symbol '-'.");
    }
  #else
    if (msc.mscgen_compat == EMscgenCompat::FORCE_MSCGEN && MSC_POS((yylsp[-1])).end.NextChar() == MSC_POS((yylsp[0])).start)
         msc.Error.Error(MSC_POS((yylsp[0])).end, "Missing an entity name after the loss arrow symbol.");
    else
        msc.Error.Error(MSC_POS((yylsp[-1])).start, "Unsupported arrow symbol '-'.");
    (yyval.arcarrow) = NULL;
#endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 7682 "language.cc" /* yacc.c:1646  */
    break;

  case 284:
#line 4166 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[-2]));
    csh.AddCSH_EntityName((yylsp[-2]), (yyvsp[-2].str));
    if (csh.mscgen_compat == EMscgenCompat::FORCE_MSCGEN && ((yylsp[-1])).last_pos+1 == ((yylsp[0])).first_pos) {
        csh.AddCSH((yylsp[-1]) + (yylsp[0]), COLOR_SYMBOL);
        csh.AddCSH_ErrorAfter((yylsp[-1]), "Missing an entity name.");
    } else {
        csh.AddCSH_Error((yylsp[-1]), "Missing arrow symbol before 'x'.");
    }
  #else
    if (msc.mscgen_compat == EMscgenCompat::FORCE_MSCGEN && MSC_POS((yylsp[-1])).end.NextChar() == MSC_POS((yylsp[0])).start)
         msc.Error.Error(MSC_POS((yylsp[0])).end, "Missing an entity name after the loss arrow symbol.");
    else
        msc.Error.Error(MSC_POS((yylsp[-1])).start, "Missing arrow symbol before 'x'.");
    (yyval.arcarrow) = NULL;
#endif
    free((yyvsp[-2].str));
    free((yyvsp[-1].str));
}
#line 7707 "language.cc" /* yacc.c:1646  */
    break;

  case 285:
#line 4189 "language.yy" /* yacc.c:1646  */
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
#line 7724 "language.cc" /* yacc.c:1646  */
    break;

  case 286:
#line 4202 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[-1].arcsegdata), LSIDE_ENT_STR, MSC_POS((yylsp[-1])), (yyvsp[0].str), true, MSC_POS((yylsp[0])));
  #endif
    free((yyvsp[0].str));
}
#line 7738 "language.cc" /* yacc.c:1646  */
    break;

  case 287:
#line 4212 "language.yy" /* yacc.c:1646  */
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
#line 7753 "language.cc" /* yacc.c:1646  */
    break;

  case 288:
#line 4223 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcarrow) = ((yyvsp[-2].arcarrow))->AddSegment((yyvsp[-1].arcsegdata), (yyvsp[0].str), MSC_POS((yylsp[0])), MSC_POS2((yylsp[-1]), (yylsp[0])));
  #endif
    free((yyvsp[0].str));
}
#line 7767 "language.cc" /* yacc.c:1646  */
    break;

  case 289:
#line 4233 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((yylsp[-1]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[-1].arcarrow))->AddSegment((yyvsp[0].arcsegdata), NULL, MSC_POS((yylsp[0])), MSC_POS((yylsp[0])));
  #endif
}
#line 7779 "language.cc" /* yacc.c:1646  */
    break;

  case 290:
#line 4243 "language.yy" /* yacc.c:1646  */
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
#line 7796 "language.cc" /* yacc.c:1646  */
    break;

  case 291:
#line 4256 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[-1].arcsegdata), (yyvsp[0].str), MSC_POS((yylsp[0])), LSIDE_ENT_STR, false, MSC_POS((yylsp[-1])));
  #endif
    free((yyvsp[0].str));
}
#line 7810 "language.cc" /* yacc.c:1646  */
    break;

  case 292:
#line 4266 "language.yy" /* yacc.c:1646  */
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
#line 7825 "language.cc" /* yacc.c:1646  */
    break;

  case 293:
#line 4277 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcarrow) = ((yyvsp[-2].arcarrow))->AddSegment((yyvsp[-1].arcsegdata), (yyvsp[0].str), MSC_POS((yylsp[0])), MSC_POS2((yylsp[-1]), (yylsp[0])));
  #endif
    free((yyvsp[0].str));
}
#line 7839 "language.cc" /* yacc.c:1646  */
    break;

  case 294:
#line 4287 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[-1].arcarrow))->AddSegment((yyvsp[0].arcsegdata), NULL, MSC_POS((yylsp[0])), MSC_POS((yylsp[0])));
  #endif
}
#line 7851 "language.cc" /* yacc.c:1646  */
    break;

  case 295:
#line 4296 "language.yy" /* yacc.c:1646  */
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
#line 7868 "language.cc" /* yacc.c:1646  */
    break;

  case 296:
#line 4309 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[-1].arcsegdata), LSIDE_ENT_STR, MSC_POS((yylsp[-1])), (yyvsp[0].str), true, MSC_POS((yylsp[0])));
  #endif
    free((yyvsp[0].str));
}
#line 7882 "language.cc" /* yacc.c:1646  */
    break;

  case 297:
#line 4319 "language.yy" /* yacc.c:1646  */
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
#line 7897 "language.cc" /* yacc.c:1646  */
    break;

  case 298:
#line 4330 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcarrow) = ((yyvsp[-2].arcarrow))->AddSegment((yyvsp[-1].arcsegdata), (yyvsp[0].str), MSC_POS((yylsp[0])), MSC_POS2((yylsp[-1]), (yylsp[0])));
  #endif
    free((yyvsp[0].str));
}
#line 7911 "language.cc" /* yacc.c:1646  */
    break;

  case 299:
#line 4340 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[-1].arcarrow))->AddSegment((yyvsp[0].arcsegdata), NULL, MSC_POS((yylsp[0])), MSC_POS((yylsp[0])));
  #endif
}
#line 7923 "language.cc" /* yacc.c:1646  */
    break;

  case 301:
#line 4348 "language.yy" /* yacc.c:1646  */
    {(yyval.arcsymbol)=EArcSymbol::ARC_UNDETERMINED_SEGMENT;}
#line 7929 "language.cc" /* yacc.c:1646  */
    break;

  case 303:
#line 4349 "language.yy" /* yacc.c:1646  */
    {(yyval.arcsymbol)=EArcSymbol::ARC_UNDETERMINED_SEGMENT;}
#line 7935 "language.cc" /* yacc.c:1646  */
    break;

  case 305:
#line 4350 "language.yy" /* yacc.c:1646  */
    {(yyval.arcsymbol)=EArcSymbol::ARC_UNDETERMINED_SEGMENT;}
#line 7941 "language.cc" /* yacc.c:1646  */
    break;

  case 306:
#line 4353 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 0;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arcsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
#line 7955 "language.cc" /* yacc.c:1646  */
    break;

  case 307:
#line 4363 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arcsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-1])));
  #endif
}
#line 7970 "language.cc" /* yacc.c:1646  */
    break;

  case 308:
#line 4374 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[-1].arcsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[0])));
  #endif
}
#line 7985 "language.cc" /* yacc.c:1646  */
    break;

  case 309:
#line 4385 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]) + (yylsp[-1]), COLOR_SYMBOL);
    csh.AddCSH_Error((yylsp[0]), MULTIPLE_ASTERISK_ERROR_MSG);
    csh.asteriskNo = 2;
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).start, "One arrow may be lost only once. Ignoring subsequent asterisks ('*').");
    ((yyval.arcsegdata)).type = (yyvsp[-1].arcsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-2])));
  #endif
}
#line 8002 "language.cc" /* yacc.c:1646  */
    break;

  case 310:
#line 4399 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arcsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
#line 8016 "language.cc" /* yacc.c:1646  */
    break;

  case 311:
#line 4409 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arcsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-1])));
  #endif
}
#line 8031 "language.cc" /* yacc.c:1646  */
    break;

  case 312:
#line 4420 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[-1].arcsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[0])));
  #endif
}
#line 8046 "language.cc" /* yacc.c:1646  */
    break;

  case 313:
#line 4431 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]) + (yylsp[-1]), COLOR_SYMBOL);
    csh.AddCSH_Error((yylsp[0]), MULTIPLE_ASTERISK_ERROR_MSG);
    csh.asteriskNo = 2;
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).start, "One arrow may be lost only once. Ignoring subsequent asterisks ('*').");
    ((yyval.arcsegdata)).type = (yyvsp[-1].arcsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-2])));
  #endif
}
#line 8063 "language.cc" /* yacc.c:1646  */
    break;

  case 314:
#line 4445 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 0;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arcsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
#line 8077 "language.cc" /* yacc.c:1646  */
    break;

  case 315:
#line 4455 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arcsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-1])));
  #endif
}
#line 8092 "language.cc" /* yacc.c:1646  */
    break;

  case 316:
#line 4466 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[-1].arcsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[0])));
  #endif
}
#line 8107 "language.cc" /* yacc.c:1646  */
    break;

  case 317:
#line 4477 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]) + (yylsp[-1]), COLOR_SYMBOL);
    csh.AddCSH_Error((yylsp[0]), MULTIPLE_ASTERISK_ERROR_MSG);
    csh.asteriskNo = 2;
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).start, "One arrow may be lost only once. Ignoring subsequent asterisks ('*').");
    ((yyval.arcsegdata)).type = (yyvsp[-1].arcsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-2])));
  #endif
}
#line 8124 "language.cc" /* yacc.c:1646  */
    break;

  case 318:
#line 4491 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arcsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
#line 8137 "language.cc" /* yacc.c:1646  */
    break;

  case 319:
#line 4500 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[-1]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arcsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-1])));
  #endif
}
#line 8155 "language.cc" /* yacc.c:1646  */
    break;

  case 320:
#line 4514 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[0]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[-1].arcsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[0])));
  #endif
}
#line 8173 "language.cc" /* yacc.c:1646  */
    break;

  case 321:
#line 4528 "language.yy" /* yacc.c:1646  */
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
    ((yyval.arcsegdata)).type = (yyvsp[-1].arcsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-2])));
  #endif
}
#line 8193 "language.cc" /* yacc.c:1646  */
    break;

  case 322:
#line 4545 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arcsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
#line 8206 "language.cc" /* yacc.c:1646  */
    break;

  case 323:
#line 4554 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[-1]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arcsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-1])));
  #endif
}
#line 8224 "language.cc" /* yacc.c:1646  */
    break;

  case 324:
#line 4568 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[0]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[-1].arcsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[0])));
  #endif
}
#line 8242 "language.cc" /* yacc.c:1646  */
    break;

  case 325:
#line 4582 "language.yy" /* yacc.c:1646  */
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
    ((yyval.arcsegdata)).type = (yyvsp[-1].arcsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-2])));
  #endif
}
#line 8262 "language.cc" /* yacc.c:1646  */
    break;

  case 326:
#line 4599 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arcsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
#line 8275 "language.cc" /* yacc.c:1646  */
    break;

  case 327:
#line 4608 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[-1]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arcsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-1])));
  #endif
}
#line 8293 "language.cc" /* yacc.c:1646  */
    break;

  case 328:
#line 4622 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[0]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[-1].arcsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[0])));
  #endif
}
#line 8311 "language.cc" /* yacc.c:1646  */
    break;

  case 329:
#line 4636 "language.yy" /* yacc.c:1646  */
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
    ((yyval.arcsegdata)).type = (yyvsp[-1].arcsymbol);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[-2])));
  #endif
}
#line 8331 "language.cc" /* yacc.c:1646  */
    break;

  case 331:
#line 4656 "language.yy" /* yacc.c:1646  */
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
#line 8348 "language.cc" /* yacc.c:1646  */
    break;

  case 332:
#line 4670 "language.yy" /* yacc.c:1646  */
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
#line 8368 "language.cc" /* yacc.c:1646  */
    break;

  case 333:
#line 4686 "language.yy" /* yacc.c:1646  */
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
#line 8388 "language.cc" /* yacc.c:1646  */
    break;

  case 337:
#line 4705 "language.yy" /* yacc.c:1646  */
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
#line 8418 "language.cc" /* yacc.c:1646  */
    break;

  case 338:
#line 4731 "language.yy" /* yacc.c:1646  */
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
#line 8451 "language.cc" /* yacc.c:1646  */
    break;

  case 339:
#line 4761 "language.yy" /* yacc.c:1646  */
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
#line 8491 "language.cc" /* yacc.c:1646  */
    break;

  case 340:
#line 4797 "language.yy" /* yacc.c:1646  */
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
#line 8519 "language.cc" /* yacc.c:1646  */
    break;

  case 341:
#line 4821 "language.yy" /* yacc.c:1646  */
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
#line 8550 "language.cc" /* yacc.c:1646  */
    break;

  case 342:
#line 4848 "language.yy" /* yacc.c:1646  */
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
#line 8577 "language.cc" /* yacc.c:1646  */
    break;

  case 343:
#line 4871 "language.yy" /* yacc.c:1646  */
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
#line 8605 "language.cc" /* yacc.c:1646  */
    break;

  case 344:
#line 4895 "language.yy" /* yacc.c:1646  */
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
#line 8630 "language.cc" /* yacc.c:1646  */
    break;

  case 345:
#line 4916 "language.yy" /* yacc.c:1646  */
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
#line 8651 "language.cc" /* yacc.c:1646  */
    break;

  case 346:
#line 4934 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[0].arcbase))
        ((yyvsp[0].arcbase))->AddAttributeList(NULL);
    (yyval.arcbase) = ((yyvsp[0].arcbase));
  #endif
}
#line 8663 "language.cc" /* yacc.c:1646  */
    break;

  case 347:
#line 4942 "language.yy" /* yacc.c:1646  */
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
#line 8680 "language.cc" /* yacc.c:1646  */
    break;

  case 348:
#line 4956 "language.yy" /* yacc.c:1646  */
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
#line 8707 "language.cc" /* yacc.c:1646  */
    break;

  case 349:
#line 4979 "language.yy" /* yacc.c:1646  */
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
#line 8730 "language.cc" /* yacc.c:1646  */
    break;

  case 350:
#line 4998 "language.yy" /* yacc.c:1646  */
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
#line 8754 "language.cc" /* yacc.c:1646  */
    break;

  case 351:
#line 5018 "language.yy" /* yacc.c:1646  */
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
#line 8781 "language.cc" /* yacc.c:1646  */
    break;

  case 352:
#line 5042 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.eside)= ESide::LEFT;
  #endif
    free((yyvsp[0].str));
}
#line 8792 "language.cc" /* yacc.c:1646  */
    break;

  case 353:
#line 5049 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.eside)= ESide::END;
  #endif
    free((yyvsp[0].str));
}
#line 8803 "language.cc" /* yacc.c:1646  */
    break;

  case 354:
#line 5056 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.eside)= ESide::END;
  #endif
    free((yyvsp[0].str));
}
#line 8814 "language.cc" /* yacc.c:1646  */
    break;

  case 355:
#line 5064 "language.yy" /* yacc.c:1646  */
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
#line 8834 "language.cc" /* yacc.c:1646  */
    break;

  case 356:
#line 5080 "language.yy" /* yacc.c:1646  */
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
#line 8852 "language.cc" /* yacc.c:1646  */
    break;

  case 357:
#line 5095 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ColonString_CheckAndAddEscapeHint((yylsp[0]), (yyvsp[0].str), false);
    csh.AddColonLabel((yylsp[0]));
#endif
    (yyval.str) = (yyvsp[0].str);
}
#line 8864 "language.cc" /* yacc.c:1646  */
    break;

  case 358:
#line 5103 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ColonString_CheckAndAddEscapeHint((yylsp[0]), (yyvsp[0].str), true);
	csh.AddColonLabel((yylsp[0]));
  #endif
    (yyval.str) = (yyvsp[0].str);
}
#line 8876 "language.cc" /* yacc.c:1646  */
    break;

  case 359:
#line 5112 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = (new AttributeList)->Append(new Attribute("label", (yyvsp[0].str), MSC_POS((yyloc)), MSC_POS((yyloc)).IncStartCol()));
  #endif
    free((yyvsp[0].str));
}
#line 8888 "language.cc" /* yacc.c:1646  */
    break;

  case 360:
#line 5120 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = ((yyvsp[0].attriblist))->Prepend(new Attribute("label", (yyvsp[-1].str), MSC_POS((yylsp[-1])), MSC_POS((yylsp[-1])).IncStartCol()));
  #endif
    free((yyvsp[-1].str));
}
#line 8900 "language.cc" /* yacc.c:1646  */
    break;

  case 361:
#line 5128 "language.yy" /* yacc.c:1646  */
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
#line 8916 "language.cc" /* yacc.c:1646  */
    break;

  case 362:
#line 5140 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = ((yyvsp[-1].attriblist))->Append(new Attribute("label", (yyvsp[0].str), MSC_POS((yylsp[0])), MSC_POS((yylsp[0])).IncStartCol()));
  #endif
    free((yyvsp[0].str));
}
#line 8928 "language.cc" /* yacc.c:1646  */
    break;

  case 364:
#line 5151 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[-1]), COLOR_BRACKET);
        csh.AddCSH((yylsp[0]), COLOR_BRACKET);
        csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_NAME);
  #else
    (yyval.attriblist) = new AttributeList;
  #endif
}
#line 8942 "language.cc" /* yacc.c:1646  */
    break;

  case 365:
#line 5161 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[-2]), COLOR_BRACKET);
        csh.AddCSH((yylsp[0]), COLOR_BRACKET);
        csh.CheckHintBetween((yylsp[-2]), (yylsp[-1]), EHintSourceType::ATTR_NAME);
  #else
    (yyval.attriblist) = (yyvsp[-1].attriblist);
  #endif
}
#line 8956 "language.cc" /* yacc.c:1646  */
    break;

  case 366:
#line 5171 "language.yy" /* yacc.c:1646  */
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
#line 8971 "language.cc" /* yacc.c:1646  */
    break;

  case 367:
#line 5182 "language.yy" /* yacc.c:1646  */
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
#line 8987 "language.cc" /* yacc.c:1646  */
    break;

  case 368:
#line 5194 "language.yy" /* yacc.c:1646  */
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
#line 9002 "language.cc" /* yacc.c:1646  */
    break;

  case 369:
#line 5205 "language.yy" /* yacc.c:1646  */
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
#line 9017 "language.cc" /* yacc.c:1646  */
    break;

  case 370:
#line 5216 "language.yy" /* yacc.c:1646  */
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
#line 9032 "language.cc" /* yacc.c:1646  */
    break;

  case 371:
#line 5229 "language.yy" /* yacc.c:1646  */
    {
    (yyval.attriblist) = NULL;
}
#line 9040 "language.cc" /* yacc.c:1646  */
    break;

  case 372:
#line 5233 "language.yy" /* yacc.c:1646  */
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
#line 9055 "language.cc" /* yacc.c:1646  */
    break;

  case 373:
#line 5245 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.attriblist) = (new AttributeList)->Append((yyvsp[0].attrib));
  #endif
}
#line 9066 "language.cc" /* yacc.c:1646  */
    break;

  case 374:
#line 5252 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_COMMA);
    csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), EHintSourceType::ATTR_NAME);
  #else
    (yyval.attriblist) = ((yyvsp[-2].attriblist))->Append((yyvsp[0].attrib));
  #endif
}
#line 9079 "language.cc" /* yacc.c:1646  */
    break;

  case 375:
#line 5261 "language.yy" /* yacc.c:1646  */
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
#line 9094 "language.cc" /* yacc.c:1646  */
    break;

  case 377:
#line 5274 "language.yy" /* yacc.c:1646  */
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
#line 9113 "language.cc" /* yacc.c:1646  */
    break;

  case 378:
#line 5289 "language.yy" /* yacc.c:1646  */
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
#line 9132 "language.cc" /* yacc.c:1646  */
    break;

  case 379:
#line 5304 "language.yy" /* yacc.c:1646  */
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
#line 9151 "language.cc" /* yacc.c:1646  */
    break;

  case 380:
#line 5319 "language.yy" /* yacc.c:1646  */
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
#line 9169 "language.cc" /* yacc.c:1646  */
    break;

  case 381:
#line 5333 "language.yy" /* yacc.c:1646  */
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
#line 9185 "language.cc" /* yacc.c:1646  */
    break;

  case 382:
#line 5347 "language.yy" /* yacc.c:1646  */
    {
    (yyval.attrib) = NULL;
    free((yyvsp[-2].str));
}
#line 9194 "language.cc" /* yacc.c:1646  */
    break;

  case 383:
#line 5352 "language.yy" /* yacc.c:1646  */
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
#line 9209 "language.cc" /* yacc.c:1646  */
    break;

  case 384:
#line 5364 "language.yy" /* yacc.c:1646  */
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
#line 9227 "language.cc" /* yacc.c:1646  */
    break;

  case 385:
#line 5378 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
	(yyval.vshape) = ArcVerticalArrow::ARROW_OR_BOX;
  #endif
  free((yyvsp[0].str));
}
#line 9238 "language.cc" /* yacc.c:1646  */
    break;

  case 386:
#line 5388 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
	csh.AddQuotedString((yylsp[0]));
  #endif
  (yyval.str) = (yyvsp[0].str);
}
#line 9249 "language.cc" /* yacc.c:1646  */
    break;

  case 389:
#line 5397 "language.yy" /* yacc.c:1646  */
    {
	(yyval.str) = (char*)malloc(2);
	((yyval.str))[0] = ShapeElement::act_code[(yyvsp[0].shapecommand)];
	((yyval.str))[1] = 0;
}
#line 9259 "language.cc" /* yacc.c:1646  */
    break;

  case 425:
#line 5415 "language.yy" /* yacc.c:1646  */
    {
    switch ((yyvsp[0].arcsymbol)) {
    case EArcSymbol::ARC_DOTTED: (yyval.str) = strdup(">"); break;
    case EArcSymbol::ARC_DASHED: (yyval.str) = strdup(">>"); break;
    case EArcSymbol::ARC_SOLID:  (yyval.str) = strdup("->"); break;
    case EArcSymbol::ARC_DOUBLE: (yyval.str) = strdup("=>"); break;
    default: _ASSERT(0);
    }
}
#line 9273 "language.cc" /* yacc.c:1646  */
    break;

  case 426:
#line 5425 "language.yy" /* yacc.c:1646  */
    {
    switch ((yyvsp[0].arcsymbol)) {
    case EArcSymbol::ARC_DOTTED: (yyval.str) = strdup("<"); break;
    case EArcSymbol::ARC_DASHED: (yyval.str) = strdup("<<"); break;
    case EArcSymbol::ARC_SOLID:  (yyval.str) = strdup("<-"); break;
    case EArcSymbol::ARC_DOUBLE: (yyval.str) = strdup("<="); break;
    default: _ASSERT(0);
    }
}
#line 9287 "language.cc" /* yacc.c:1646  */
    break;

  case 427:
#line 5436 "language.yy" /* yacc.c:1646  */
    {
    switch ((yyvsp[0].arcsymbol)) {
    case EArcSymbol::ARC_DOTTED: (yyval.str) = strdup("<>"); break;
    case EArcSymbol::ARC_DASHED: (yyval.str) = strdup("<<>>"); break;
    case EArcSymbol::ARC_SOLID:  (yyval.str) = strdup("<->"); break;
    case EArcSymbol::ARC_DOUBLE: (yyval.str) = strdup("<=>"); break;
    default: _ASSERT(0);
    }
}
#line 9301 "language.cc" /* yacc.c:1646  */
    break;

  case 428:
#line 5446 "language.yy" /* yacc.c:1646  */
    {
    switch ((yyvsp[0].arcsymbol)) {
    case EArcSymbol::DIV_DIVIDER:  (yyval.str) = strdup("---"); break;
    case EArcSymbol::DIV_DISCO:    (yyval.str) = strdup("..."); break;
    default: _ASSERT(0);
    }
}
#line 9313 "language.cc" /* yacc.c:1646  */
    break;

  case 429:
#line 5454 "language.yy" /* yacc.c:1646  */
    {
    switch ((yyvsp[0].arcsymbol)) {
    case EArcSymbol::BOX_SOLID:  (yyval.str) = strdup("--"); break;
    case EArcSymbol::BOX_DASHED: (yyval.str) = strdup("++"); break; //will likely not happen due to special handling in TOK_COLORDEF
    case EArcSymbol::BOX_DOTTED: (yyval.str) = strdup(".."); break;
    case EArcSymbol::BOX_DOUBLE: (yyval.str) = strdup("=="); break;
    default: _ASSERT(0);
    }
}
#line 9327 "language.cc" /* yacc.c:1646  */
    break;

  case 435:
#line 5469 "language.yy" /* yacc.c:1646  */
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
#line 9344 "language.cc" /* yacc.c:1646  */
    break;

  case 437:
#line 5486 "language.yy" /* yacc.c:1646  */
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
#line 9361 "language.cc" /* yacc.c:1646  */
    break;


#line 9365 "language.cc" /* yacc.c:1646  */
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
#line 5500 "language.yy" /* yacc.c:1906  */



/* END OF FILE */
