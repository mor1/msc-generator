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
    TOK_BOOLEAN = 277,
    TOK_REL_SOLID_TO = 278,
    TOK_REL_SOLID_FROM = 279,
    TOK_REL_SOLID_BIDIR = 280,
    TOK_REL_DOUBLE_TO = 281,
    TOK_REL_DOUBLE_FROM = 282,
    TOK_REL_DOUBLE_BIDIR = 283,
    TOK_REL_DASHED_TO = 284,
    TOK_REL_DASHED_FROM = 285,
    TOK_REL_DASHED_BIDIR = 286,
    TOK_REL_DOTTED_TO = 287,
    TOK_REL_DOTTED_FROM = 288,
    TOK_REL_DOTTED_BIDIR = 289,
    TOK_SPECIAL_ARC = 290,
    TOK_EMPH = 291,
    TOK_EMPH_PLUS_PLUS = 292,
    TOK_COMMAND_HEADING = 293,
    TOK_COMMAND_NUDGE = 294,
    TOK_COMMAND_NEWPAGE = 295,
    TOK_COMMAND_DEFSHAPE = 296,
    TOK_COMMAND_DEFCOLOR = 297,
    TOK_COMMAND_DEFSTYLE = 298,
    TOK_COMMAND_DEFDESIGN = 299,
    TOK_COMMAND_BIG = 300,
    TOK_COMMAND_BOX = 301,
    TOK_COMMAND_PIPE = 302,
    TOK_COMMAND_MARK = 303,
    TOK_COMMAND_PARALLEL = 304,
    TOK_COMMAND_OVERLAP = 305,
    TOK_VERTICAL = 306,
    TOK_VERTICAL_SHAPE = 307,
    TOK_AT = 308,
    TOK_LOST = 309,
    TOK_AT_POS = 310,
    TOK_SHOW = 311,
    TOK_HIDE = 312,
    TOK_ACTIVATE = 313,
    TOK_DEACTIVATE = 314,
    TOK_BYE = 315,
    TOK_COMMAND_VSPACE = 316,
    TOK_COMMAND_HSPACE = 317,
    TOK_COMMAND_SYMBOL = 318,
    TOK_COMMAND_NOTE = 319,
    TOK_COMMAND_COMMENT = 320,
    TOK_COMMAND_ENDNOTE = 321,
    TOK_COMMAND_FOOTNOTE = 322,
    TOK_COMMAND_TITLE = 323,
    TOK_COMMAND_SUBTITLE = 324,
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
#define TOK_BOOLEAN 277
#define TOK_REL_SOLID_TO 278
#define TOK_REL_SOLID_FROM 279
#define TOK_REL_SOLID_BIDIR 280
#define TOK_REL_DOUBLE_TO 281
#define TOK_REL_DOUBLE_FROM 282
#define TOK_REL_DOUBLE_BIDIR 283
#define TOK_REL_DASHED_TO 284
#define TOK_REL_DASHED_FROM 285
#define TOK_REL_DASHED_BIDIR 286
#define TOK_REL_DOTTED_TO 287
#define TOK_REL_DOTTED_FROM 288
#define TOK_REL_DOTTED_BIDIR 289
#define TOK_SPECIAL_ARC 290
#define TOK_EMPH 291
#define TOK_EMPH_PLUS_PLUS 292
#define TOK_COMMAND_HEADING 293
#define TOK_COMMAND_NUDGE 294
#define TOK_COMMAND_NEWPAGE 295
#define TOK_COMMAND_DEFSHAPE 296
#define TOK_COMMAND_DEFCOLOR 297
#define TOK_COMMAND_DEFSTYLE 298
#define TOK_COMMAND_DEFDESIGN 299
#define TOK_COMMAND_BIG 300
#define TOK_COMMAND_BOX 301
#define TOK_COMMAND_PIPE 302
#define TOK_COMMAND_MARK 303
#define TOK_COMMAND_PARALLEL 304
#define TOK_COMMAND_OVERLAP 305
#define TOK_VERTICAL 306
#define TOK_VERTICAL_SHAPE 307
#define TOK_AT 308
#define TOK_LOST 309
#define TOK_AT_POS 310
#define TOK_SHOW 311
#define TOK_HIDE 312
#define TOK_ACTIVATE 313
#define TOK_DEACTIVATE 314
#define TOK_BYE 315
#define TOK_COMMAND_VSPACE 316
#define TOK_COMMAND_HSPACE 317
#define TOK_COMMAND_SYMBOL 318
#define TOK_COMMAND_NOTE 319
#define TOK_COMMAND_COMMENT 320
#define TOK_COMMAND_ENDNOTE 321
#define TOK_COMMAND_FOOTNOTE 322
#define TOK_COMMAND_TITLE 323
#define TOK_COMMAND_SUBTITLE 324
#define TOK_SHAPE_COMMAND 325
#define TOK__NEVER__HAPPENS 326

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 95 "language.yy" /* yacc.c:355  */

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

#line 336 "language.cc" /* yacc.c:355  */
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
#line 179 "language.yy" /* yacc.c:358  */

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


#line 425 "language.cc" /* yacc.c:358  */

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
#define YYFINAL  213
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1767

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  72
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  89
/* YYNRULES -- Number of rules.  */
#define YYNRULES  418
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  464

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
       0,   243,   243,   248,   249,   258,   270,   280,   289,   298,
     309,   317,   331,   332,   341,   355,   367,   385,   393,   404,
     415,   426,   437,   447,   459,   473,   474,   485,   500,   509,
     517,   531,   532,   533,   545,   561,   561,   563,   572,   597,
     616,   624,   636,   648,   663,   681,   698,   711,   725,   744,
     761,   779,   802,   814,   822,   834,   846,   859,   871,   887,
     901,   915,   922,   929,   938,   947,   956,   971,   980,   995,
    1004,  1015,  1025,  1039,  1049,  1063,  1080,  1093,  1110,  1120,
    1134,  1135,  1136,  1137,  1151,  1166,  1180,  1194,  1210,  1210,
    1212,  1213,  1239,  1257,  1274,  1275,  1286,  1297,  1309,  1320,
    1329,  1343,  1368,  1380,  1391,  1401,  1416,  1416,  1416,  1416,
    1418,  1428,  1447,  1460,  1461,  1472,  1491,  1510,  1529,  1549,
    1568,  1588,  1607,  1628,  1634,  1644,  1656,  1659,  1675,  1687,
    1703,  1716,  1734,  1749,  1767,  1783,  1784,  1791,  1839,  1850,
    1870,  1885,  1900,  1921,  1938,  1948,  1959,  1976,  1991,  2008,
    2018,  2030,  2039,  2047,  2063,  2091,  2115,  2141,  2169,  2199,
    2232,  2233,  2243,  2257,  2257,  2259,  2281,  2304,  2322,  2339,
    2355,  2374,  2385,  2386,  2393,  2400,  2414,  2425,  2439,  2441,
    2442,  2448,  2455,  2474,  2493,  2512,  2534,  2558,  2567,  2579,
    2586,  2597,  2608,  2616,  2629,  2637,  2646,  2659,  2675,  2683,
    2696,  2704,  2719,  2734,  2750,  2761,  2780,  2790,  2804,  2824,
    2825,  2832,  2832,  2834,  2848,  2859,  2871,  2881,  2893,  2907,
    2921,  2936,  2949,  2964,  2998,  3014,  3032,  3046,  3057,  3069,
    3081,  3095,  3104,  3113,  3123,  3136,  3146,  3157,  3165,  3178,
    3179,  3182,  3193,  3201,  3220,  3228,  3237,  3252,  3258,  3258,
    3258,  3260,  3273,  3283,  3294,  3304,  3314,  3327,  3337,  3348,
    3358,  3367,  3380,  3390,  3401,  3411,  3420,  3420,  3420,  3420,
    3421,  3421,  3421,  3421,  3422,  3422,  3422,  3422,  3424,  3424,
    3425,  3425,  3426,  3426,  3428,  3438,  3449,  3460,  3474,  3484,
    3495,  3506,  3520,  3530,  3541,  3552,  3566,  3575,  3589,  3603,
    3620,  3629,  3643,  3657,  3674,  3683,  3697,  3711,  3729,  3730,
    3753,  3763,  3777,  3777,  3779,  3815,  3838,  3874,  3898,  3934,
    3971,  4007,  4024,  4031,  4044,  4062,  4076,  4091,  4111,  4118,
    4125,  4133,  4146,  4158,  4165,  4174,  4182,  4190,  4202,  4210,
    4213,  4223,  4233,  4244,  4255,  4266,  4277,  4290,  4294,  4306,
    4313,  4322,  4332,  4334,  4349,  4364,  4379,  4393,  4407,  4421,
    4426,  4438,  4452,  4462,  4469,  4470,  4477,  4477,  4478,  4478,
    4479,  4479,  4479,  4480,  4481,  4481,  4481,  4482,  4482,  4483,
    4483,  4483,  4483,  4483,  4484,  4484,  4484,  4485,  4485,  4485,
    4486,  4486,  4486,  4487,  4487,  4487,  4488,  4490,  4491,  4492,
    4493,  4494,  4495,  4496,  4497,  4498,  4499,  4500,  4501,  4502,
    4510,  4521,  4521,  4523,  4523,  4523,  4525,  4540,  4542
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
  "TOK_COLORDEF", "TOK_BOOLEAN", "TOK_REL_SOLID_TO", "TOK_REL_SOLID_FROM",
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
  "TOK_SHAPE_COMMAND", "TOK__NEVER__HAPPENS", "$accept", "msc_with_bye",
  "eof", "msc", "top_level_arclist", "msckey", "braced_arclist",
  "arclist_maybe_no_semicolon", "arclist", "arc_with_parallel_semicolon",
  "overlap_or_parallel", "arc_with_parallel", "arc", "titlecommandtoken",
  "hspace_location", "full_arcattrlist_with_label_or_number", "entityrel",
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

#define YYPACT_NINF -352

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-352)))

#define YYTABLE_NINF -347

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     714,  -352,  -352,  -352,   -53,   437,   968,   341,  -352,  -352,
    -352,  -352,  -352,  -352,  -352,  -352,  -352,  -352,  -352,  -352,
    -352,  -352,  -352,  -352,  -352,    21,    21,    21,   225,  1530,
    1122,    34,   347,    59,   276,  1394,  -352,  -352,  1633,  -352,
    -352,  -352,  -352,   279,    47,  1684,   247,  -352,  -352,  -352,
    -352,  -352,  -352,    22,   114,    19,    36,  -352,    55,   782,
    -352,  1462,   254,  -352,    38,   225,    66,  -352,    93,    36,
    1636,  -352,  -352,   353,  -352,   225,   289,    38,    57,  1532,
    1529,   385,   106,   120,   127,   225,   225,   225,    38,  -352,
    -352,    38,  -352,    21,  -352,   102,  1714,   509,  -352,   116,
    -352,  -352,  -352,  -352,  -352,  -352,  -352,  -352,  -352,  -352,
    -352,  -352,  -352,  -352,  -352,  -352,  -352,  -352,  -352,  -352,
    -352,  -352,  -352,  -352,  -352,  -352,  -352,  -352,  -352,  -352,
    -352,  -352,  -352,  -352,  -352,  -352,  -352,  -352,  -352,  -352,
    -352,  -352,  -352,  -352,  -352,  -352,  -352,  -352,   325,  -352,
    -352,  -352,  -352,   141,   150,   147,   167,   173,   114,  1190,
    1394,  -352,  -352,  -352,  -352,  -352,   182,   191,  -352,   194,
    -352,   204,  -352,    58,  -352,  -352,   203,  -352,    38,  1733,
    -352,   145,  -352,    38,    21,   559,  -352,  -352,   225,   200,
    -352,   225,   166,    38,  -352,  -352,  -352,  1598,  1668,    38,
    -352,  -352,   225,   156,  -352,   279,  -352,   216,    43,  -352,
    -352,   850,  -352,  -352,  -352,   215,  -352,   114,  -352,  -352,
     782,     3,  -352,   309,  -352,   223,   165,  -352,   230,   289,
     115,   225,  -352,  -352,   289,    36,  -352,    38,  -352,  -352,
      36,  -352,   166,  -352,  1531,  -352,   218,   225,  -352,   398,
    -352,   224,   225,  -352,   387,  -352,   234,   225,  -352,  -352,
    -352,  -352,  -352,  -352,  -352,  -352,  -352,    21,   918,  -352,
     225,   225,   225,   225,    36,  -352,  -352,  -352,    23,  -352,
    -352,   236,   577,  -352,   646,   185,  -352,  -352,  -352,  -352,
     246,  -352,   133,  1530,   986,  1122,  1258,  -352,  -352,   136,
    -352,  -352,  -352,   253,   208,  -352,   219,  -352,  -352,    38,
    -352,   225,  -352,  -352,  -352,  -352,   225,   225,   166,   155,
     155,   256,  -352,    38,  -352,  -352,  -352,  1394,   231,  -352,
     225,  -352,  -352,  -352,   267,   296,  -352,   289,  -352,    36,
    -352,  -352,  -352,  -352,   259,  -352,  -352,   271,  -352,  -352,
     290,  -352,  -352,  -352,  -352,  -352,  -352,  -352,  -352,  -352,
    -352,  -352,  -352,    16,  -352,  -352,  -352,  -352,  -352,  -352,
    -352,  -352,  1326,  -352,  -352,  -352,   235,   310,   226,  -352,
    -352,  1326,  -352,  -352,  -352,  -352,   153,  1530,  1122,   318,
    -352,   323,  -352,   334,  -352,  -352,   122,   349,  -352,  -352,
    -352,   284,  -352,  -352,  -352,   155,  -352,  -352,  -352,   225,
    -352,  -352,  -352,   296,    39,  -352,    36,  -352,  -352,  -352,
    -352,  -352,  -352,  -352,  -352,   354,   345,  -352,  -352,  -352,
    1394,  1394,   191,   204,   134,  -352,   163,  1054,  -352,   355,
    -352,  -352,  -352,  -352,  -352,  -352,  -352,   359,  -352,  -352,
    -352,   370,  -352,  -352,  -352,  -352,  -352,  -352,  -352,   361,
    -352,   362,   363,  -352
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       6,   364,   363,    33,   416,     0,     0,     0,   334,   333,
     266,   270,   274,   267,   271,   275,   268,   272,   276,   269,
     273,   277,   244,   212,   211,    71,    73,    78,     0,    66,
      68,    70,    42,   190,     0,     0,    35,    36,    45,   107,
     106,   108,   109,    87,    85,   321,     0,   328,   329,   330,
      88,    89,   365,     0,     0,     0,     0,   187,    12,    25,
      28,    39,     0,    37,     0,    54,    60,   110,    53,    63,
      61,   189,   206,   209,    62,   216,   198,    40,   247,   248,
     249,   250,   284,   288,   292,     0,     0,     0,   322,    80,
      81,   332,    82,   335,    52,   339,   132,     0,   417,   348,
     340,   366,   415,   397,   398,   399,   400,   401,   402,   403,
     404,   405,   406,   407,   408,   409,   410,   377,   378,   370,
     367,   368,   369,   371,   372,   373,   379,   375,   376,   374,
     396,   380,   381,   382,   383,   384,   385,   386,   388,   387,
     389,   390,   391,   392,   393,   394,   395,   347,     0,   349,
     411,   412,   414,   413,   360,   285,   289,   293,     0,   120,
     122,     7,    72,    74,    79,    64,   144,    65,   160,   168,
     140,    67,   135,   138,   413,   139,     0,    69,    43,     0,
     191,     0,   203,   202,    76,     0,   362,   361,   226,   227,
     243,   237,   242,    46,   232,   231,   233,    48,   238,    95,
      86,    94,    99,    92,    93,    84,    90,   101,   320,   312,
     313,   326,   325,     1,     3,     4,     2,     0,    10,     8,
      13,     0,    29,     0,    38,     0,    30,    75,    55,   132,
       0,    56,   188,   195,   192,     0,   210,   207,   214,   200,
     199,    41,   246,   279,     0,   278,   296,   255,   281,     0,
     280,   300,   260,   283,     0,   282,   304,   265,   286,   290,
     294,   252,   257,   262,   323,   331,   336,   338,   118,   134,
     215,   253,   258,   263,   131,    22,   418,    24,     0,    18,
     343,   345,   351,   341,   358,     0,   287,   291,   295,     9,
     119,   121,     0,   162,   167,     0,   141,   137,   171,     0,
      44,   205,    77,   228,   217,   229,   235,   241,    49,    47,
      51,   236,    96,    98,    91,    83,    97,   104,   311,   318,
     316,   319,   327,     0,     5,    11,    14,   120,    34,    31,
      58,   114,   113,   111,     0,    57,   123,   128,   194,   193,
     197,   208,   201,   245,   297,   298,   254,   301,   302,   259,
     305,   306,   264,   337,   116,   115,   117,   213,   251,   256,
     261,   133,    21,     0,    23,    17,   342,   352,   350,   356,
     163,   357,   355,   353,   164,   359,     0,   153,     0,   149,
     161,     0,   165,   136,   143,   142,     0,   175,   177,     0,
     172,     0,   179,     0,   230,   218,   219,   221,   223,   239,
      50,   234,   100,   103,   310,   314,   308,   309,   317,   102,
     324,   119,    32,    59,   125,   130,   127,   196,   299,   303,
     307,    20,    19,   354,   150,   154,   152,   147,   151,   166,
       0,     0,   174,   176,     0,   181,     0,     0,   220,   224,
     222,   240,   315,   105,   126,   124,   129,   155,   148,   185,
     186,     0,   169,   173,   180,   183,   182,   184,   225,   156,
     170,   157,   158,   159
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -352,  -352,   -52,  -352,   172,  -352,     8,   297,  -352,   327,
    -352,   328,   336,  -352,  -352,   193,  -352,  -352,  -352,  -352,
     169,    70,   -12,   338,    18,   112,  -352,  -352,  -352,  -351,
      24,  -282,   137,  -352,  -352,  -352,   -26,  -352,   -16,  -352,
    -352,   396,  -352,  -352,  -352,   -36,    71,  -188,   237,  -352,
     242,   400,  -352,  -352,  -352,  -352,    15,    11,    -1,   198,
     195,   189,   -29,   -27,   -25,  -352,  -352,  -352,  -313,   239,
    -352,  -352,  -352,  -352,  -352,  -352,   358,    12,    14,  -352,
     176,  -352,     0,  -352,   403,     1,    -4,  -352,  -270
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    53,   216,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,   205,   200,   206,   319,    65,    66,
      67,   335,   336,    68,   171,   172,   173,   165,   378,   379,
     167,   373,   168,   177,   299,   389,   390,   391,   392,    69,
      70,    71,    72,    73,    74,    75,    76,   190,   191,   192,
     193,    77,    78,    79,    80,    81,    82,    83,    84,   246,
     251,   256,    85,    86,    87,   247,   252,   257,   405,   406,
     208,    88,    89,    90,    91,    92,    93,    94,    95,   148,
     149,   197,   150,   151,   152,   174,   374,    97,   279
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      96,   154,   189,   218,   307,   157,   153,   408,   365,   194,
     327,   195,   382,   196,   160,   161,   421,   156,    98,   214,
     217,   155,   213,   362,   363,   424,   175,   428,   166,   276,
     169,   184,   179,   181,   181,     5,   276,   176,   198,   162,
     163,   164,     1,     2,   207,   209,     1,     2,     4,   317,
       1,     2,     5,   202,   343,   201,     8,     9,   212,    96,
     270,    96,     1,     2,   219,   229,   296,   271,   220,   272,
     181,   273,     5,   181,   230,   238,   227,   232,   233,   215,
     255,   236,   235,   364,   239,   261,   262,   263,   240,   241,
     423,   250,   442,   422,   245,    23,    24,    96,   318,   429,
     264,   231,   203,   265,   269,   183,   289,   266,   274,    52,
     444,   242,   204,    52,   214,  -112,   331,    52,     1,     2,
       8,     9,   258,  -112,  -112,     1,     2,   438,  -112,    52,
     404,   280,   221,  -145,   376,   451,   259,     1,     2,     1,
       2,   234,  -145,   260,   237,   270,  -145,   452,   284,   303,
     271,   386,   272,   386,   273,   290,   291,   285,     1,     2,
     430,   189,   189,   286,   431,   325,     1,     2,   194,   194,
     195,   195,   196,   196,   215,  -112,   387,   388,   387,   388,
     386,    23,    24,   287,   157,    52,   332,   297,   304,   288,
     300,   306,    52,  -145,   292,   301,   156,   198,   302,   293,
     155,   294,   313,   377,    52,   308,    52,   323,   321,   310,
     318,   312,   295,   395,   396,   298,   305,   201,   397,   188,
      96,   314,   316,   322,   324,    52,  -146,   426,     1,     2,
     334,   337,   328,    52,   345,  -146,   329,   269,   330,   427,
     348,   274,   338,   340,    23,    24,   339,   346,   342,   341,
     351,   366,   349,   255,   -27,   225,   375,   352,   -16,   245,
     250,     5,   409,   226,   356,     8,     9,   -27,   398,   394,
     357,   358,   359,   360,   268,   418,  -204,   182,   154,     1,
       2,   353,   361,   153,   199,  -204,  -146,   419,  -204,  -204,
     399,   175,   385,     5,   169,    52,   377,     8,     9,   393,
     211,     4,   412,     5,   414,   377,   420,     8,     9,   -26,
     225,   401,    23,    24,   -27,   425,   402,   403,   226,   407,
     407,   400,   -26,   411,   435,  -344,   281,   434,  -344,  -344,
     337,   436,  -178,   282,  -344,   410,  -204,  -344,  -344,  -344,
     283,   437,   158,  -344,  -344,   415,    52,   417,   159,   416,
       1,     2,   160,     4,   440,   441,     1,     2,   448,   447,
     458,  -344,  -344,     6,   459,     4,   461,   462,   463,   -26,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,   460,   175,  -344,   222,   223,   169,    23,
      24,   253,   326,   253,   278,  -344,   439,   224,   315,   333,
     413,   254,   445,   228,   248,   407,   433,   383,   453,   443,
      12,   432,    12,    15,   337,    15,    18,    52,    18,    21,
     454,    21,    11,    52,   446,    14,   449,   450,    17,   180,
     380,    20,   178,   457,   393,   311,   393,  -346,    99,   309,
       1,     2,   344,   350,   347,  -346,  -346,   320,   210,  -346,
    -346,  -346,   100,   267,   101,  -346,  -346,   102,   368,     0,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,  -346,   117,   118,   119,     0,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,     0,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    52,   147,   275,
       0,     0,     1,     2,     0,     0,     0,     0,     3,     0,
       0,     4,   276,     5,     0,     6,   221,     8,     9,     0,
       0,     0,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,     0,     0,     0,     0,    39,    40,    41,    42,   277,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
       1,     2,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,   101,    23,    24,   102,     0,     0,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,     0,   117,   118,   119,     0,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,     0,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    52,   367,     1,
       2,   369,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   370,   371,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   372,   117,   118,   119,     0,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
       0,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    52,     1,     2,     0,
       0,     0,     0,     3,     0,     0,     4,     0,     5,     0,
       6,     7,     8,     9,     0,     0,     0,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,     0,     0,     0,     0,
      39,    40,    41,    42,     0,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,     1,     2,     0,     0,     0,
       0,     3,     0,     0,     4,     0,     5,     0,     6,   221,
       8,     9,     0,     0,     0,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,     0,     0,     0,     0,    39,    40,
      41,    42,     0,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,     1,     2,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     5,     0,     0,   101,     8,     9,
     102,     0,     0,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,     0,   117,   118,
     119,     0,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,     0,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      52,     1,     2,   354,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,     0,
     355,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,     0,   117,   118,   119,     0,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,     0,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    52,     1,
       2,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,   101,     0,     0,   102,   370,     0,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   381,   117,   118,   119,     0,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
       0,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    52,     1,     2,   455,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,     0,   456,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,     0,   117,   118,   119,     0,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,     0,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    52,     1,     2,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   101,
       0,     0,   102,     0,     0,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   170,
     117,   118,   119,     0,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,     0,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    52,     1,     2,     0,     0,     0,     0,     0,
       0,     0,   -15,     0,     0,     0,     0,   101,     0,     0,
     102,     0,     0,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,     0,   117,   118,
     119,     0,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,     0,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      52,     1,     2,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,     0,
       0,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   384,   117,   118,   119,     0,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,     0,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    52,     1,
       2,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   370,     0,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,     0,   117,   118,   119,     0,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
       0,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    52,     1,     2,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,     0,     0,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,     0,   117,   118,   119,     0,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,     0,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    52,     1,     2,     0,     0,     0,
       0,     0,     0,     0,     4,     0,     5,     0,     6,   221,
       8,     9,     0,     0,     0,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,     0,     0,    38,     0,     0,     0,     0,    39,    40,
      41,    42,     0,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,     1,     2,   248,     0,   243,   243,     0,
       0,     0,     0,     0,     0,   249,     0,   101,   244,     0,
       0,     0,     0,    11,    10,    10,    14,    13,    13,    17,
      16,    16,    20,    19,    19,     0,     0,     0,   117,   118,
     119,     0,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,     0,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      52,     1,     2,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     5,     0,   185,     0,     8,     9,     0,     0,
       0,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,     0,    23,    24,     1,     2,     0,     1,
       2,     0,     0,     0,     0,     0,     0,     0,     4,   185,
       5,   188,     0,     0,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    52,    23,
      24,     0,    23,    24,     0,     0,     0,     0,     0,   186,
       0,     0,     0,     0,   185,   187,   188,     1,     2,     0,
       0,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    52,    23,    24,    52,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   268,     0,     0,     0,     0,     4,     0,     5,     0,
       6,     0,     8,     9,     0,     0,     0,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,     6,
      23,    24,     0,     0,    52,     0,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21
};

static const yytype_int16 yycheck[] =
{
       0,     5,    38,    55,   192,     6,     5,   320,   278,    38,
       7,    38,   294,    38,    11,     7,     0,     6,    71,     0,
       1,     6,     0,     0,     1,   376,    30,   378,    28,    13,
      29,    35,    32,    33,    34,    14,    13,     3,    38,    25,
      26,    27,     3,     4,    44,    45,     3,     4,    12,     6,
       3,     4,    14,     6,   242,    43,    18,    19,    46,    59,
      96,    61,     3,     4,    56,    65,     8,    96,    13,    96,
      70,    96,    14,    73,     8,    75,    64,    69,    70,    60,
      81,    73,    70,    60,    76,    85,    86,    87,    76,    77,
     372,    80,   405,   363,    79,    36,    37,    97,    55,   381,
      88,     8,    55,    91,    96,    34,   158,    93,    96,    70,
      71,    54,    65,    70,     0,     0,     1,    70,     3,     4,
      18,    19,    16,     8,     9,     3,     4,     5,    13,    70,
     318,    15,    17,     0,     1,     1,    16,     3,     4,     3,
       4,    70,     9,    16,    73,   181,    13,    13,     7,   185,
     179,    17,   179,    17,   179,   159,   160,     7,     3,     4,
       7,   197,   198,    16,    11,   217,     3,     4,   197,   198,
     197,   198,   197,   198,    60,    60,    42,    43,    42,    43,
      17,    36,    37,    16,   185,    70,    71,   173,   188,    16,
     178,   191,    70,    60,    12,   183,   185,   197,   184,     8,
     185,     7,   202,    70,    70,   193,    70,   211,   208,   197,
      55,   199,     8,     5,     6,    12,    16,   205,    10,    53,
     220,    65,     6,   211,     9,    70,     0,     1,     3,     4,
     230,   231,     9,    70,    16,     9,    71,   229,     8,    13,
      16,   229,   234,   235,    36,    37,   234,   247,   240,   237,
      16,    15,   252,   254,     0,     1,    71,   257,    12,   244,
     249,    14,     6,     9,   268,    18,    19,    13,   304,    16,
     270,   271,   272,   273,     7,    16,     0,     1,   282,     3,
       4,   267,   274,   282,     5,     9,    60,    16,    12,    13,
      71,   295,   296,    14,   293,    70,    70,    18,    19,   299,
      53,    12,    71,    14,     8,    70,    16,    18,    19,     0,
       1,   311,    36,    37,    60,     5,   316,   317,     9,   319,
     320,   309,    13,   327,     1,     0,     1,     9,     3,     4,
     330,     8,     9,     8,     9,   323,    60,    12,    13,    14,
      15,     7,     1,    18,    19,   337,    70,   339,     7,   337,
       3,     4,    11,    12,     5,    71,     3,     4,    13,     5,
       5,    36,    37,    16,     5,    12,     5,     5,     5,    60,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    13,   388,    60,    59,    59,   387,    36,
      37,     6,   220,     6,    97,    70,   396,    61,   205,   230,
     330,    16,   414,    65,     6,   405,   388,   295,   434,   409,
      25,   387,    25,    28,   414,    28,    31,    70,    31,    34,
     436,    34,    24,    70,   416,    27,   430,   431,    30,    33,
     293,    33,    32,   437,   434,   198,   436,     0,     1,   197,
       3,     4,   244,   254,   249,     8,     9,   208,    45,    12,
      13,    14,    15,    95,    17,    18,    19,    20,   282,    -1,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    -1,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    -1,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,     0,
      -1,    -1,     3,     4,    -1,    -1,    -1,    -1,     9,    -1,
      -1,    12,    13,    14,    -1,    16,    17,    18,    19,    -1,
      -1,    -1,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    -1,    -1,    -1,    -1,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
       3,     4,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    17,    36,    37,    20,    -1,    -1,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    -1,    38,    39,    40,    -1,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    -1,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,     3,
       4,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      -1,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,     3,     4,    -1,
      -1,    -1,    -1,     9,    -1,    -1,    12,    -1,    14,    -1,
      16,    17,    18,    19,    -1,    -1,    -1,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    -1,    -1,    -1,    -1,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,     3,     4,    -1,    -1,    -1,
      -1,     9,    -1,    -1,    12,    -1,    14,    -1,    16,    17,
      18,    19,    -1,    -1,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    -1,    -1,    -1,    -1,    56,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,     3,     4,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    19,
      20,    -1,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      40,    -1,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,     3,     4,     5,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    40,    -1,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    -1,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,     3,
       4,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    17,    -1,    -1,    20,    21,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      -1,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,     3,     4,     5,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    -1,    38,    39,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    -1,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,     3,     4,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    20,    -1,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    -1,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    -1,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,     3,     4,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      20,    -1,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      40,    -1,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,     3,     4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    20,    -1,
      -1,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    -1,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    -1,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,     3,
       4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    20,    21,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    -1,    38,    39,    40,    -1,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      -1,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,     3,     4,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    20,    -1,    -1,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    -1,    38,    39,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    -1,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,     3,     4,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    14,    -1,    16,    17,
      18,    19,    -1,    -1,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    -1,    -1,    51,    -1,    -1,    -1,    -1,    56,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,     3,     4,     6,    -1,     6,     6,    -1,
      -1,    -1,    -1,    -1,    -1,    16,    -1,    17,    16,    -1,
      -1,    -1,    -1,    24,    23,    23,    27,    26,    26,    30,
      29,    29,    33,    32,    32,    -1,    -1,    -1,    38,    39,
      40,    -1,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,     3,     4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    -1,    16,    -1,    18,    19,    -1,    -1,
      -1,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    -1,    36,    37,     3,     4,    -1,     3,
       4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    16,
      14,    53,    -1,    -1,    18,    19,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    70,    36,
      37,    -1,    36,    37,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    16,    52,    53,     3,     4,    -1,
      -1,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    70,    36,    37,    70,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,     7,    -1,    -1,    -1,    -1,    12,    -1,    14,    -1,
      16,    -1,    18,    19,    -1,    -1,    -1,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    16,
      36,    37,    -1,    -1,    70,    -1,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     9,    12,    14,    16,    17,    18,    19,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    56,
      57,    58,    59,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    73,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    90,    91,    92,    95,   111,
     112,   113,   114,   115,   116,   117,   118,   123,   124,   125,
     126,   127,   128,   129,   130,   134,   135,   136,   143,   144,
     145,   146,   147,   148,   149,   150,   154,   159,    71,     1,
      15,    17,    20,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    38,    39,    40,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    71,   151,   152,
     154,   155,   156,   157,   158,   128,   129,   130,     1,     7,
      11,    78,   150,   150,   150,    99,   154,   102,   104,   157,
      37,    96,    97,    98,   157,   158,     3,   105,   123,   154,
     113,   154,     1,   118,   158,    16,    46,    52,    53,   117,
     119,   120,   121,   122,   134,   135,   136,   153,   154,     5,
      87,   149,     6,    55,    65,    86,    88,   154,   142,   154,
     156,    53,   149,     0,     0,    60,    74,     1,    74,    78,
      13,    17,    81,    83,    84,     1,     9,   149,    95,   154,
       8,     8,    78,    78,   118,   149,    78,   118,   154,    78,
     149,   149,    54,     6,    16,   128,   131,   137,     6,    16,
     129,   132,   138,     6,    16,   130,   133,   139,    16,    16,
      16,   154,   154,   154,   149,   149,   150,   148,     7,    78,
     117,   134,   135,   136,   149,     0,    13,    60,    79,   160,
      15,     1,     8,    15,     7,     7,    16,    16,    16,    74,
     158,   158,    12,     8,     7,     8,     8,   150,    12,   106,
     149,   149,   150,   117,   154,    16,   154,   119,   149,   122,
     149,   120,   149,   154,    65,    87,     6,     6,    55,    89,
     141,   154,   149,   158,     9,    74,    76,     7,     9,    71,
       8,     1,    71,    92,   154,    93,    94,   154,    78,   149,
      78,   149,    78,   119,   131,    16,   154,   132,    16,   154,
     133,    16,   154,   150,     5,    22,   158,   154,   154,   154,
     154,    78,     0,     1,    60,   160,    15,    71,   152,     5,
      21,    22,    37,   103,   158,    71,     1,    70,   100,   101,
     104,    37,   103,    97,    37,   158,    17,    42,    43,   107,
     108,   109,   110,   154,    16,     5,     6,    10,   117,    71,
     149,   154,   154,   154,   119,   140,   141,   154,   140,     6,
     149,   158,    71,    93,     8,    78,   149,    78,    16,    16,
      16,     0,   160,   103,   101,     5,     1,    13,   101,   103,
       7,    11,   102,    96,     9,     1,     8,     7,     5,   154,
       5,    71,   140,   154,    71,    94,    78,     5,    13,   158,
     158,     1,    13,   108,   110,     5,    22,   158,     5,     5,
      13,     5,     5,     5
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
      84,    84,    84,    84,    84,    84,    84,    84,    85,    85,
      86,    86,    86,    86,    87,    87,    87,    88,    88,    88,
      88,    88,    89,    89,    89,    89,    90,    90,    90,    90,
      91,    91,    91,    91,    91,    92,    92,    92,    92,    92,
      92,    92,    92,    93,    93,    93,    93,    94,    94,    94,
      94,    95,    95,    95,    95,    96,    96,    97,    97,    98,
      98,    98,    98,    98,    99,    99,    99,    99,    99,   100,
     100,   100,   100,   101,   101,   101,   101,   101,   101,   101,
     102,   102,   102,   103,   103,   104,   104,   104,   104,   105,
     105,   106,   107,   107,   108,   108,   108,   108,   108,   109,
     109,   109,   110,   110,   110,   110,   110,   111,   111,   112,
     112,   112,   112,   112,   112,   112,   112,   112,   113,   113,
     113,   113,   114,   114,   114,   114,   115,   115,   115,   116,
     116,   117,   117,   118,   118,   118,   118,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   120,   120,   120,
     120,   120,   120,   120,   121,   121,   121,   121,   121,   121,
     121,   122,   122,   122,   123,   123,   123,   123,   124,   124,
     124,   125,   125,   125,   125,   125,   126,   126,   126,   126,
     126,   127,   127,   127,   127,   127,   128,   128,   128,   128,
     129,   129,   129,   129,   130,   130,   130,   130,   131,   131,
     132,   132,   133,   133,   134,   134,   134,   134,   135,   135,
     135,   135,   136,   136,   136,   136,   137,   137,   137,   137,
     138,   138,   138,   138,   139,   139,   139,   139,   140,   140,
     141,   141,   142,   142,   143,   143,   143,   143,   143,   143,
     143,   143,   144,   144,   145,   145,   145,   145,   146,   146,
     146,   147,   147,   148,   148,   149,   149,   149,   149,   149,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   151,
     151,   151,   151,   152,   152,   152,   152,   152,   152,   152,
     152,   153,   153,   154,   154,   154,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   157,   157,   158,   158,   158,   159,   159,   160
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
       1,     1,     1,     1,     2,     2,     1,     2,     1,     2,
       1,     1,     2,     1,     2,     2,     2,     3,     1,     2,
       1,     1,     1,     3,     2,     1,     2,     1,     1,     1,
       1,     2,     1,     1,     1,     1,     2,     2,     2,     1,
       3,     1,     2,     2,     1,     3,     1,     1,     1,     1,
       1,     3,     2,     3,     3,     3,     3,     3,     2,     3,
       2,     3,     2,     1,     3,     2,     3,     2,     1,     3,
       2,     2,     1,     3,     2,     1,     3,     2,     1,     1,
       1,     2,     3,     3,     1,     2,     3,     4,     5,     1,
       2,     2,     2,     1,     2,     3,     4,     5,     6,     7,
       1,     3,     2,     1,     1,     3,     4,     2,     1,     5,
       6,     1,     1,     3,     2,     1,     2,     1,     1,     1,
       3,     2,     3,     3,     3,     3,     3,     1,     2,     1,
       1,     2,     2,     3,     3,     2,     4,     3,     1,     2,
       2,     3,     2,     2,     1,     3,     1,     2,     3,     1,
       2,     1,     1,     3,     2,     2,     1,     2,     3,     3,
       4,     3,     4,     3,     4,     5,     1,     1,     2,     2,
       3,     1,     1,     1,     3,     2,     2,     1,     1,     3,
       4,     2,     1,     1,     1,     3,     2,     1,     1,     1,
       1,     3,     2,     2,     3,     2,     3,     2,     2,     3,
       2,     3,     2,     2,     3,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     3,     1,     2,
       2,     3,     1,     2,     2,     3,     1,     2,     2,     3,
       1,     2,     2,     3,     1,     2,     2,     3,     1,     1,
       2,     1,     1,     1,     4,     5,     3,     4,     3,     3,
       2,     1,     1,     2,     4,     2,     2,     3,     1,     1,
       1,     2,     1,     1,     1,     1,     2,     3,     2,     1,
       2,     3,     4,     3,     2,     3,     1,     2,     2,     1,
       3,     2,     3,     3,     4,     3,     3,     3,     2,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     1
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
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 1997 "language.cc" /* yacc.c:1257  */
        break;

    case 4: /* TOK_QSTRING  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2003 "language.cc" /* yacc.c:1257  */
        break;

    case 5: /* TOK_NUMBER  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2009 "language.cc" /* yacc.c:1257  */
        break;

    case 17: /* TOK_MSC  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2015 "language.cc" /* yacc.c:1257  */
        break;

    case 18: /* TOK_COLON_STRING  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2021 "language.cc" /* yacc.c:1257  */
        break;

    case 19: /* TOK_COLON_QUOTED_STRING  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2027 "language.cc" /* yacc.c:1257  */
        break;

    case 20: /* TOK_STYLE_NAME  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2033 "language.cc" /* yacc.c:1257  */
        break;

    case 21: /* TOK_COLORDEF  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2039 "language.cc" /* yacc.c:1257  */
        break;

    case 22: /* TOK_BOOLEAN  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2045 "language.cc" /* yacc.c:1257  */
        break;

    case 38: /* TOK_COMMAND_HEADING  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2051 "language.cc" /* yacc.c:1257  */
        break;

    case 39: /* TOK_COMMAND_NUDGE  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2057 "language.cc" /* yacc.c:1257  */
        break;

    case 40: /* TOK_COMMAND_NEWPAGE  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2063 "language.cc" /* yacc.c:1257  */
        break;

    case 41: /* TOK_COMMAND_DEFSHAPE  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2069 "language.cc" /* yacc.c:1257  */
        break;

    case 42: /* TOK_COMMAND_DEFCOLOR  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2075 "language.cc" /* yacc.c:1257  */
        break;

    case 43: /* TOK_COMMAND_DEFSTYLE  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2081 "language.cc" /* yacc.c:1257  */
        break;

    case 44: /* TOK_COMMAND_DEFDESIGN  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2087 "language.cc" /* yacc.c:1257  */
        break;

    case 45: /* TOK_COMMAND_BIG  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2093 "language.cc" /* yacc.c:1257  */
        break;

    case 46: /* TOK_COMMAND_BOX  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2099 "language.cc" /* yacc.c:1257  */
        break;

    case 47: /* TOK_COMMAND_PIPE  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2105 "language.cc" /* yacc.c:1257  */
        break;

    case 48: /* TOK_COMMAND_MARK  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2111 "language.cc" /* yacc.c:1257  */
        break;

    case 49: /* TOK_COMMAND_PARALLEL  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2117 "language.cc" /* yacc.c:1257  */
        break;

    case 50: /* TOK_COMMAND_OVERLAP  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2123 "language.cc" /* yacc.c:1257  */
        break;

    case 51: /* TOK_VERTICAL  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2129 "language.cc" /* yacc.c:1257  */
        break;

    case 52: /* TOK_VERTICAL_SHAPE  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2135 "language.cc" /* yacc.c:1257  */
        break;

    case 53: /* TOK_AT  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2141 "language.cc" /* yacc.c:1257  */
        break;

    case 54: /* TOK_LOST  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2147 "language.cc" /* yacc.c:1257  */
        break;

    case 55: /* TOK_AT_POS  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2153 "language.cc" /* yacc.c:1257  */
        break;

    case 56: /* TOK_SHOW  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2159 "language.cc" /* yacc.c:1257  */
        break;

    case 57: /* TOK_HIDE  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2165 "language.cc" /* yacc.c:1257  */
        break;

    case 58: /* TOK_ACTIVATE  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2171 "language.cc" /* yacc.c:1257  */
        break;

    case 59: /* TOK_DEACTIVATE  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2177 "language.cc" /* yacc.c:1257  */
        break;

    case 60: /* TOK_BYE  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2183 "language.cc" /* yacc.c:1257  */
        break;

    case 61: /* TOK_COMMAND_VSPACE  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2189 "language.cc" /* yacc.c:1257  */
        break;

    case 62: /* TOK_COMMAND_HSPACE  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2195 "language.cc" /* yacc.c:1257  */
        break;

    case 63: /* TOK_COMMAND_SYMBOL  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2201 "language.cc" /* yacc.c:1257  */
        break;

    case 64: /* TOK_COMMAND_NOTE  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2207 "language.cc" /* yacc.c:1257  */
        break;

    case 65: /* TOK_COMMAND_COMMENT  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2213 "language.cc" /* yacc.c:1257  */
        break;

    case 66: /* TOK_COMMAND_ENDNOTE  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2219 "language.cc" /* yacc.c:1257  */
        break;

    case 67: /* TOK_COMMAND_FOOTNOTE  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2225 "language.cc" /* yacc.c:1257  */
        break;

    case 68: /* TOK_COMMAND_TITLE  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2231 "language.cc" /* yacc.c:1257  */
        break;

    case 69: /* TOK_COMMAND_SUBTITLE  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2237 "language.cc" /* yacc.c:1257  */
        break;

    case 76: /* top_level_arclist  */
#line 173 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arclist);}
#line 2243 "language.cc" /* yacc.c:1257  */
        break;

    case 78: /* braced_arclist  */
#line 173 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arclist);}
#line 2249 "language.cc" /* yacc.c:1257  */
        break;

    case 79: /* arclist_maybe_no_semicolon  */
#line 173 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arclist);}
#line 2255 "language.cc" /* yacc.c:1257  */
        break;

    case 80: /* arclist  */
#line 173 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arclist);}
#line 2261 "language.cc" /* yacc.c:1257  */
        break;

    case 81: /* arc_with_parallel_semicolon  */
#line 173 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2267 "language.cc" /* yacc.c:1257  */
        break;

    case 82: /* overlap_or_parallel  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2273 "language.cc" /* yacc.c:1257  */
        break;

    case 83: /* arc_with_parallel  */
#line 173 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2279 "language.cc" /* yacc.c:1257  */
        break;

    case 84: /* arc  */
#line 173 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2285 "language.cc" /* yacc.c:1257  */
        break;

    case 85: /* titlecommandtoken  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2291 "language.cc" /* yacc.c:1257  */
        break;

    case 86: /* hspace_location  */
#line 176 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).namerel);}
#line 2297 "language.cc" /* yacc.c:1257  */
        break;

    case 87: /* full_arcattrlist_with_label_or_number  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).attriblist);}
#line 2303 "language.cc" /* yacc.c:1257  */
        break;

    case 88: /* entityrel  */
#line 176 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).namerel);}
#line 2309 "language.cc" /* yacc.c:1257  */
        break;

    case 89: /* markerrel_no_string  */
#line 176 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).namerel);}
#line 2315 "language.cc" /* yacc.c:1257  */
        break;

    case 90: /* entity_command_prefixes  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2321 "language.cc" /* yacc.c:1257  */
        break;

    case 91: /* optlist  */
#line 173 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arclist);}
#line 2327 "language.cc" /* yacc.c:1257  */
        break;

    case 92: /* opt  */
#line 173 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2333 "language.cc" /* yacc.c:1257  */
        break;

    case 93: /* entitylist  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).entitylist);}
#line 2339 "language.cc" /* yacc.c:1257  */
        break;

    case 94: /* entity  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).entitylist);}
#line 2345 "language.cc" /* yacc.c:1257  */
        break;

    case 95: /* first_entity  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).entitylist);}
#line 2351 "language.cc" /* yacc.c:1257  */
        break;

    case 98: /* stylenamelist  */
#line 176 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).stringlist);}
#line 2357 "language.cc" /* yacc.c:1257  */
        break;

    case 103: /* color_string  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2363 "language.cc" /* yacc.c:1257  */
        break;

    case 111: /* parallel  */
#line 174 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcparallel);}
#line 2369 "language.cc" /* yacc.c:1257  */
        break;

    case 112: /* box_list  */
#line 174 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcboxseries);}
#line 2375 "language.cc" /* yacc.c:1257  */
        break;

    case 113: /* first_box  */
#line 174 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbox);}
#line 2381 "language.cc" /* yacc.c:1257  */
        break;

    case 114: /* first_pipe  */
#line 174 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcpipe);}
#line 2387 "language.cc" /* yacc.c:1257  */
        break;

    case 115: /* pipe_list_no_content  */
#line 174 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcpipeseries);}
#line 2393 "language.cc" /* yacc.c:1257  */
        break;

    case 116: /* pipe_list  */
#line 174 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcpipeseries);}
#line 2399 "language.cc" /* yacc.c:1257  */
        break;

    case 118: /* boxrel  */
#line 174 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbox);}
#line 2405 "language.cc" /* yacc.c:1257  */
        break;

    case 119: /* vertxpos  */
#line 176 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).vertxpos);}
#line 2411 "language.cc" /* yacc.c:1257  */
        break;

    case 121: /* vertrel_no_xpos  */
#line 173 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcvertarrow);}
#line 2417 "language.cc" /* yacc.c:1257  */
        break;

    case 122: /* vertrel  */
#line 173 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcvertarrow);}
#line 2423 "language.cc" /* yacc.c:1257  */
        break;

    case 123: /* arcrel  */
#line 173 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2429 "language.cc" /* yacc.c:1257  */
        break;

    case 124: /* arcrel_arrow  */
#line 173 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcarrow);}
#line 2435 "language.cc" /* yacc.c:1257  */
        break;

    case 125: /* arcrel_to  */
#line 173 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcarrow);}
#line 2441 "language.cc" /* yacc.c:1257  */
        break;

    case 126: /* arcrel_from  */
#line 173 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcarrow);}
#line 2447 "language.cc" /* yacc.c:1257  */
        break;

    case 127: /* arcrel_bidir  */
#line 173 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcarrow);}
#line 2453 "language.cc" /* yacc.c:1257  */
        break;

    case 140: /* extvertxpos  */
#line 176 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).extvertxpos);}
#line 2459 "language.cc" /* yacc.c:1257  */
        break;

    case 141: /* extvertxpos_no_string  */
#line 176 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).extvertxpos);}
#line 2465 "language.cc" /* yacc.c:1257  */
        break;

    case 142: /* symbol_type_string  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2471 "language.cc" /* yacc.c:1257  */
        break;

    case 143: /* symbol_command_no_attr  */
#line 173 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2477 "language.cc" /* yacc.c:1257  */
        break;

    case 144: /* symbol_command  */
#line 173 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2483 "language.cc" /* yacc.c:1257  */
        break;

    case 145: /* note  */
#line 173 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2489 "language.cc" /* yacc.c:1257  */
        break;

    case 147: /* comment  */
#line 173 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2495 "language.cc" /* yacc.c:1257  */
        break;

    case 148: /* colon_string  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2501 "language.cc" /* yacc.c:1257  */
        break;

    case 149: /* full_arcattrlist_with_label  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).attriblist);}
#line 2507 "language.cc" /* yacc.c:1257  */
        break;

    case 150: /* full_arcattrlist  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).attriblist);}
#line 2513 "language.cc" /* yacc.c:1257  */
        break;

    case 151: /* arcattrlist  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).attriblist);}
#line 2519 "language.cc" /* yacc.c:1257  */
        break;

    case 152: /* arcattr  */
#line 175 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).attrib);}
#line 2525 "language.cc" /* yacc.c:1257  */
        break;

    case 154: /* entity_string  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2531 "language.cc" /* yacc.c:1257  */
        break;

    case 155: /* reserved_word_string  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2537 "language.cc" /* yacc.c:1257  */
        break;

    case 156: /* symbol_string  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2543 "language.cc" /* yacc.c:1257  */
        break;

    case 157: /* alpha_string  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2549 "language.cc" /* yacc.c:1257  */
        break;

    case 158: /* string  */
#line 177 "language.yy" /* yacc.c:1257  */
      {free(((*yyvaluep).str));}
#line 2555 "language.cc" /* yacc.c:1257  */
        break;

    case 160: /* scope_close  */
#line 173 "language.yy" /* yacc.c:1257  */
      {if (!C_S_H) delete ((*yyvaluep).arcbase);}
#line 2561 "language.cc" /* yacc.c:1257  */
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

#line 2677 "language.cc" /* yacc.c:1429  */
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
#line 244 "language.yy" /* yacc.c:1646  */
    {
	YYACCEPT;
}
#line 2868 "language.cc" /* yacc.c:1646  */
    break;

  case 4:
#line 250 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
	csh.AddCSH_AllCommentBeyond((yylsp[0]));
  #else
  #endif
    free((yyvsp[0].str));
}
#line 2881 "language.cc" /* yacc.c:1646  */
    break;

  case 5:
#line 259 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[0]), COLOR_SEMICOLON);
	csh.AddCSH_AllCommentBeyond((yylsp[0]));
  #else
  #endif
    free((yyvsp[-1].str));
}
#line 2895 "language.cc" /* yacc.c:1646  */
    break;

  case 6:
#line 270 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddLineBeginToHints();
    csh.hintStatus = HINT_READY;
    csh.hintType = HINT_LINE_START;
    csh.hintsForcedOnly = true;
  #else
    //no action for empty file
  #endif
}
#line 2910 "language.cc" /* yacc.c:1646  */
    break;

  case 7:
#line 281 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
  #else
    msc.AddArcs((yyvsp[0].arclist));
  #endif
    free((yyvsp[-1].str));
}
#line 2923 "language.cc" /* yacc.c:1646  */
    break;

  case 8:
#line 290 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[-1])))
        csh.AddDesignsToHints(true);
  #else
    msc.AddArcs((yyvsp[0].arclist));
  #endif
}
#line 2936 "language.cc" /* yacc.c:1646  */
    break;

  case 9:
#line 299 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.AddCSH_Error((yylsp[-1]), "Missing an equal sign or a list of elements between braces ('{' and '}').");
  #else
    msc.Error.Error(MSC_POS((yylsp[-1])).start, "Missing an equal sign or a list of elements between braces ('{' and '}').");
  #endif
    free((yyvsp[-2].str));
	YYACCEPT; //We should noty parse further in msc_with_bye as we may have something beyond BYE (in eof)
}
#line 2951 "language.cc" /* yacc.c:1646  */
    break;

  case 10:
#line 310 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    msc.AddArcs((yyvsp[-1].arclist));
  #endif
	YYACCEPT; //We should noty parse further in msc_with_bye as we may have something beyond BYE (in eof)
}
#line 2963 "language.cc" /* yacc.c:1646  */
    break;

  case 11:
#line 318 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
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
#line 2980 "language.cc" /* yacc.c:1646  */
    break;

  case 13:
#line 333 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[0]), "Closing brace missing its opening pair.");
  #else
    (yyval.arclist) = (yyvsp[-1].arclist);
    msc.Error.Error(MSC_POS((yylsp[0])).start, "Unexpected '}'.");
  #endif
}
#line 2993 "language.cc" /* yacc.c:1646  */
    break;

  case 14:
#line 342 "language.yy" /* yacc.c:1646  */
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
#line 3009 "language.cc" /* yacc.c:1646  */
    break;

  case 15:
#line 356 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[0]), COLOR_EQUAL);
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing a design name.");
    csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, HINT_ATTR_VALUE, "msc");
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing design name.");
  #endif
    free((yyvsp[-1].str));
}
#line 3025 "language.cc" /* yacc.c:1646  */
    break;

  case 16:
#line 368 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[-1]), COLOR_EQUAL);
    csh.AddCSH((yylsp[0]), COLOR_DESIGNNAME);
    csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), HINT_ATTR_VALUE, "msc");
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
#line 3046 "language.cc" /* yacc.c:1646  */
    break;

  case 17:
#line 386 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    if ((yyvsp[0].arcbase)) ((yyvsp[-1].arclist))->Append((yyvsp[0].arcbase)); //Append any potential CommandNumbering
    (yyval.arclist) = (yyvsp[-1].arclist);
  #endif
}
#line 3058 "language.cc" /* yacc.c:1646  */
    break;

  case 18:
#line 394 "language.yy" /* yacc.c:1646  */
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
#line 3073 "language.cc" /* yacc.c:1646  */
    break;

  case 19:
#line 405 "language.yy" /* yacc.c:1646  */
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
#line 3088 "language.cc" /* yacc.c:1646  */
    break;

  case 20:
#line 416 "language.yy" /* yacc.c:1646  */
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
#line 3103 "language.cc" /* yacc.c:1646  */
    break;

  case 21:
#line 427 "language.yy" /* yacc.c:1646  */
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
#line 3118 "language.cc" /* yacc.c:1646  */
    break;

  case 22:
#line 438 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ErrorAfter((yylsp[-1]), "Missing a closing brace ('}').");
  #else
    (yyval.arclist) = NULL;
    //Do not pop context, as the missing scope_close would have done
    msc.Error.Error(MSC_POS((yylsp[-1])).end.NextChar(), "Missing a corresponding '}'.");
  #endif
}
#line 3132 "language.cc" /* yacc.c:1646  */
    break;

  case 23:
#line 448 "language.yy" /* yacc.c:1646  */
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
#line 3148 "language.cc" /* yacc.c:1646  */
    break;

  case 24:
#line 460 "language.yy" /* yacc.c:1646  */
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
#line 3163 "language.cc" /* yacc.c:1646  */
    break;

  case 26:
#line 475 "language.yy" /* yacc.c:1646  */
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
#line 3178 "language.cc" /* yacc.c:1646  */
    break;

  case 27:
#line 486 "language.yy" /* yacc.c:1646  */
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
#line 3195 "language.cc" /* yacc.c:1646  */
    break;

  case 28:
#line 501 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[0].arcbase))
        (yyval.arclist) = (new ArcList)->Append((yyvsp[0].arcbase)); /* New list */
    else
        (yyval.arclist) = new ArcList;
  #endif
}
#line 3208 "language.cc" /* yacc.c:1646  */
    break;

  case 29:
#line 510 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[0].arcbase)) ((yyvsp[-1].arclist))->Append((yyvsp[0].arcbase));     /* Add to existing list */
    (yyval.arclist) = ((yyvsp[-1].arclist));
  #endif
}
#line 3219 "language.cc" /* yacc.c:1646  */
    break;

  case 30:
#line 518 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SEMICOLON);
    if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, HINT_LINE_START)) {
       csh.AddLineBeginToHints();
       csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.arcbase)=(yyvsp[-1].arcbase);
  #endif
}
#line 3235 "language.cc" /* yacc.c:1646  */
    break;

  case 33:
#line 534 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SEMICOLON);
    if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, HINT_LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.arcbase)=NULL;
  #endif
}
#line 3251 "language.cc" /* yacc.c:1646  */
    break;

  case 34:
#line 546 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SEMICOLON);
    csh.AddCSH_Error((yylsp[-1]), "I am not sure what is coming here.");
    if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, HINT_LINE_START)) {
       csh.AddLineBeginToHints();
       csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.arcbase)=(yyvsp[-2].arcbase);
    msc.Error.Error(MSC_POS((yylsp[-2])).end.NextChar(), "Missing a semicolon ';'.");
    msc.Error.Error(MSC_POS((yylsp[-2])).start, MSC_POS((yylsp[-2])).end.NextChar(), "Here is the beginning of the command as I understood it.");
  #endif
}
#line 3270 "language.cc" /* yacc.c:1646  */
    break;

  case 37:
#line 564 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    if ((yyvsp[0].arcbase)) 
		((yyvsp[0].arcbase))->SetLineEnd(MSC_POS((yyloc)));
    (yyval.arcbase)=(yyvsp[0].arcbase);
  #endif
}
#line 3283 "language.cc" /* yacc.c:1646  */
    break;

  case 38:
#line 573 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[-1]), HINT_LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), HINT_LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
        csh.hintType = HINT_LINE_START;
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
#line 3312 "language.cc" /* yacc.c:1646  */
    break;

  case 39:
#line 598 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[0]), HINT_LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF, HINT_LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
        csh.hintType = HINT_LINE_START;
        csh.hintsForcedOnly = true;
    }
  #else
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[0].str));
}
#line 3334 "language.cc" /* yacc.c:1646  */
    break;

  case 40:
#line 617 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    ((yyvsp[0].arcbase))->AddAttributeList(NULL);
    (yyval.arcbase)=((yyvsp[0].arcbase));
  #endif
}
#line 3346 "language.cc" /* yacc.c:1646  */
    break;

  case 41:
#line 625 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[0])))
        ArcArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[0])))
        ArcArrow::AttributeValues(csh.hintAttrName, csh);
  #else
    ((yyvsp[-1].arcbase))->AddAttributeList((yyvsp[0].attriblist));
    (yyval.arcbase) = ((yyvsp[-1].arcbase));
  #endif
}
#line 3362 "language.cc" /* yacc.c:1646  */
    break;

  case 42:
#line 637 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    csh.CheckEntityHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF);
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing an arrow specification.");
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing an arrow specification.");
  #endif
    free((yyvsp[0].str));
    (yyval.arcbase) = NULL;
}
#line 3378 "language.cc" /* yacc.c:1646  */
    break;

  case 43:
#line 649 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
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
#line 3397 "language.cc" /* yacc.c:1646  */
    break;

  case 44:
#line 664 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[-2]), (yylsp[-1]));
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[0])))
        ArcBigArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[0])))
        ArcBigArrow::AttributeValues(csh.hintAttrName, csh);
  #else
    //Returns NULL, if BIG is before a self-pointing arrow
    ArcBigArrow *arrow = msc.CreateArcBigArrow((yyvsp[-1].arcbase));
    if (arrow) arrow->AddAttributeList((yyvsp[0].attriblist));
    (yyval.arcbase) = arrow;
    delete (yyvsp[-1].arcbase);
  #endif
    free((yyvsp[-2].str));
}
#line 3419 "language.cc" /* yacc.c:1646  */
    break;

  case 45:
#line 682 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF, HINT_MARKER)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "brace", HINT_KEYWORD));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "bracket", HINT_KEYWORD));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "range", HINT_KEYWORD));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "box", HINT_KEYWORD));
    }
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing a marker or one of 'brace', 'bracket', 'range', 'box' or an arrow or box symbol, such as '->' or '--'.");
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing a marker or one of 'brace', 'bracket', 'range', 'box' or an arrow or box symbol, such as '->' or '--'.");
  #endif
    free((yyvsp[0].str));
    (yyval.arcbase) = NULL;
}
#line 3440 "language.cc" /* yacc.c:1646  */
    break;

  case 46:
#line 699 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
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
#line 3457 "language.cc" /* yacc.c:1646  */
    break;

  case 47:
#line 712 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
  #else
    if ((yyvsp[0].arcvertarrow)) {
      ((yyvsp[0].arcvertarrow))->SetVerticalShape((yyvsp[-1].vshape));
      ((yyvsp[0].arcvertarrow))->AddAttributeList(NULL);
      (yyval.arcbase) = ((yyvsp[0].arcvertarrow));
    } else (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[-2].str));
}
#line 3475 "language.cc" /* yacc.c:1646  */
    break;

  case 48:
#line 726 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
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
#line 3498 "language.cc" /* yacc.c:1646  */
    break;

  case 49:
#line 745 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[0])))
        ArcVerticalArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[0])))
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
#line 3519 "language.cc" /* yacc.c:1646  */
    break;

  case 50:
#line 762 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-3]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[0])))
        ArcVerticalArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[0])))
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
#line 3541 "language.cc" /* yacc.c:1646  */
    break;

  case 51:
#line 780 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[0])))
        ArcVerticalArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[0])))
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
#line 3568 "language.cc" /* yacc.c:1646  */
    break;

  case 52:
#line 803 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[0])))
        ArcDivider::AttributeNames(csh, false, false);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[0])))
        ArcDivider::AttributeValues(csh.hintAttrName, csh, false, false);
  #else
    (yyval.arcbase) = (new ArcDivider(MSC_ARC_VSPACE, &msc));
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
  #endif
}
#line 3584 "language.cc" /* yacc.c:1646  */
    break;

  case 53:
#line 815 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (new CommandEntity((yyvsp[0].entitylist), &msc, false));
    ((yyval.arcbase))->AddAttributeList(NULL);
  #endif
}
#line 3596 "language.cc" /* yacc.c:1646  */
    break;

  case 54:
#line 823 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    csh.CheckEntityHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF);
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing an entity.");
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing an entity.");
  #endif
    free((yyvsp[0].str));
    (yyval.arcbase) = NULL;
}
#line 3612 "language.cc" /* yacc.c:1646  */
    break;

  case 55:
#line 835 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[-1]), (yylsp[0]));
  #else
    CommandEntity *ce = new CommandEntity((yyvsp[0].entitylist), &msc, false);
    ce->AddAttributeList(NULL);
    (yyval.arcbase) = ce->ApplyPrefix((yyvsp[-1].str));
  #endif
    free((yyvsp[-1].str));
}
#line 3628 "language.cc" /* yacc.c:1646  */
    break;

  case 56:
#line 847 "language.yy" /* yacc.c:1646  */
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
#line 3645 "language.cc" /* yacc.c:1646  */
    break;

  case 57:
#line 860 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_COMMA);
  #else
    ((yyvsp[0].entitylist))->Prepend((yyvsp[-2].entitylist));
    CommandEntity *ce = new CommandEntity((yyvsp[0].entitylist), &msc, false);
    delete ((yyvsp[-2].entitylist));
    ce->AddAttributeList(NULL);
    (yyval.arcbase) = ce;
  #endif
}
#line 3661 "language.cc" /* yacc.c:1646  */
    break;

  case 58:
#line 872 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[0]), COLOR_COMMA);
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
#line 3681 "language.cc" /* yacc.c:1646  */
    break;

  case 59:
#line 888 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-3]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[-1]), COLOR_COMMA);
  #else
    ((yyvsp[0].entitylist))->Prepend((yyvsp[-2].entitylist));
    CommandEntity *ce = new CommandEntity((yyvsp[0].entitylist), &msc, false);
    delete ((yyvsp[-2].entitylist));
    ce->AddAttributeList(NULL);
    (yyval.arcbase) = ce->ApplyPrefix((yyvsp[-3].str));
  #endif
    free((yyvsp[-3].str));
}
#line 3699 "language.cc" /* yacc.c:1646  */
    break;

  case 60:
#line 902 "language.yy" /* yacc.c:1646  */
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
#line 3717 "language.cc" /* yacc.c:1646  */
    break;

  case 61:
#line 916 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (yyvsp[0].arcboxseries); //to remove warning for downcast
  #endif
}
#line 3728 "language.cc" /* yacc.c:1646  */
    break;

  case 62:
#line 923 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (yyvsp[0].arcpipeseries); //to remove warning for downcast
  #endif
}
#line 3739 "language.cc" /* yacc.c:1646  */
    break;

  case 63:
#line 930 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = ((yyvsp[0].arcparallel));
	if ((yyvsp[0].arcparallel))
        ((yyval.arcbase))->AddAttributeList(NULL);
  #endif
}
#line 3752 "language.cc" /* yacc.c:1646  */
    break;

  case 64:
#line 939 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[-1].str));
}
#line 3765 "language.cc" /* yacc.c:1646  */
    break;

  case 65:
#line 948 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[-1].str));
}
#line 3778 "language.cc" /* yacc.c:1646  */
    break;

  case 66:
#line 957 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, HINT_ATTR_VALUE)) {
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
#line 3797 "language.cc" /* yacc.c:1646  */
    break;

  case 67:
#line 972 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[-1].str));
}
#line 3810 "language.cc" /* yacc.c:1646  */
    break;

  case 68:
#line 981 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, HINT_ATTR_VALUE)) {
        csh.AddStylesToHints(true);
        csh.hintStatus = HINT_READY;
	}
	csh.AddCSH_ErrorAfter((yyloc), "Missing style name to (re-)define.");
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing a style name to (re-)define.");
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[0].str));
}
#line 3829 "language.cc" /* yacc.c:1646  */
    break;

  case 69:
#line 996 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[-1].str));
}
#line 3842 "language.cc" /* yacc.c:1646  */
    break;

  case 70:
#line 1005 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
	csh.AddCSH_ErrorAfter((yyloc), "Missing design name to (re-)define.");
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing a design name to (re-)define.");
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[0].str));
}
#line 3857 "language.cc" /* yacc.c:1646  */
    break;

  case 71:
#line 1016 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = (new CommandEntity(NULL, &msc, false));
    ((yyval.arcbase))->AddAttributeList(NULL);
  #endif
    free((yyvsp[0].str));
}
#line 3871 "language.cc" /* yacc.c:1646  */
    break;

  case 72:
#line 1026 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[0])))
        CommandEntity::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[0])))
        CommandEntity::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcbase) = (new CommandEntity(NULL, &msc, false));
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
  #endif
    free((yyvsp[-1].str));
}
#line 3889 "language.cc" /* yacc.c:1646  */
    break;

  case 73:
#line 1040 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = (new ArcDivider(MSC_COMMAND_NUDGE, &msc));
    ((yyval.arcbase))->AddAttributeList(NULL);
  #endif
    free((yyvsp[0].str));
}
#line 3903 "language.cc" /* yacc.c:1646  */
    break;

  case 74:
#line 1050 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[0])))
        ArcDivider::AttributeNames(csh, true, false);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[0])))
        ArcDivider::AttributeValues(csh.hintAttrName, csh, true, false);
  #else
    (yyval.arcbase) = (new ArcDivider(MSC_COMMAND_NUDGE, &msc));
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
  #endif
    free((yyvsp[-1].str));
}
#line 3921 "language.cc" /* yacc.c:1646  */
    break;

  case 75:
#line 1064 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[0])))
        ArcDivider::AttributeNames(csh, false, true);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[0])))
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
#line 3942 "language.cc" /* yacc.c:1646  */
    break;

  case 76:
#line 1081 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[0]), COLOR_MARKERNAME);
    csh.MarkerNames.insert((yyvsp[0].str));
  #else
    (yyval.arcbase) = (new CommandMark((yyvsp[0].str), MSC_POS((yyloc)), &msc));
    ((yyval.arcbase))->AddAttributeList(NULL);
  #endif
    free((yyvsp[-1].str));
    free((yyvsp[0].str));
}
#line 3959 "language.cc" /* yacc.c:1646  */
    break;

  case 77:
#line 1094 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[-1]), COLOR_MARKERNAME);
    csh.MarkerNames.insert((yyvsp[-1].str));
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[-1])))
        CommandMark::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[-1])))
        CommandMark::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcbase) = (new CommandMark((yyvsp[-1].str), MSC_POS((yyloc)), &msc));
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[-1].str));
}
#line 3980 "language.cc" /* yacc.c:1646  */
    break;

  case 78:
#line 1111 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = (new CommandNewpage(&msc, true));
    ((yyval.arcbase))->AddAttributeList(NULL);
  #endif
    free((yyvsp[0].str));
}
#line 3994 "language.cc" /* yacc.c:1646  */
    break;

  case 79:
#line 1121 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[0])))
        CommandNewpage::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[0])))
        CommandNewpage::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcbase) = (new CommandNewpage(&msc, true));
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
  #endif
    free((yyvsp[-1].str));
}
#line 4012 "language.cc" /* yacc.c:1646  */
    break;

  case 83:
#line 1138 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[-1])))
        CommandHSpace::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[-1])))
        CommandHSpace::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcbase) = (new CommandHSpace(&msc, (yyvsp[-1].namerel)));
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
  #endif
    free((yyvsp[-2].str));
}
#line 4030 "language.cc" /* yacc.c:1646  */
    break;

  case 84:
#line 1152 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", HINT_KEYWORD, false));
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<label>", HINT_KEYWORD, false));
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_ErrorAfter((yylsp[-1]), "Missing either a number or a label.");
  #else
    (yyval.arcbase) = new CommandHSpace(&msc, (yyvsp[0].namerel)); //Will trigger an error: either label or space attr is needed
  #endif
    free((yyvsp[-1].str));
}
#line 4049 "language.cc" /* yacc.c:1646  */
    break;

  case 85:
#line 1167 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckEntityHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "left comment", HINT_KEYWORD));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "right comment", HINT_KEYWORD));
    }
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing an entity.");
  #else
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[0].str));
}
#line 4067 "language.cc" /* yacc.c:1646  */
    break;

  case 86:
#line 1181 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[0])))
        CommandHSpace::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[0])))
        CommandHSpace::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcbase) = (new CommandVSpace(&msc));
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
  #endif
    free((yyvsp[-1].str));
}
#line 4085 "language.cc" /* yacc.c:1646  */
    break;

  case 87:
#line 1195 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", HINT_KEYWORD, false));
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<label>", HINT_KEYWORD, false));
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing either a number or a label.");
  #else
    (yyval.arcbase) = new CommandVSpace(&msc); //will result in an error, since label or space attribute is needed
  #endif
    free((yyvsp[0].str));
}
#line 4104 "language.cc" /* yacc.c:1646  */
    break;

  case 91:
#line 1214 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (CaseInsensitiveEqual((yyvsp[-1].str), "left") || CaseInsensitiveEqual((yyvsp[-1].str), "right"))
        csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    else
        csh.AddCSH_Error((yylsp[-1]), "Use either `left` or `right` to specify which comment column to size.");
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckEntityHintAt((yylsp[-1]))) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "left", HINT_KEYWORD));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "right", HINT_KEYWORD));
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
#line 4134 "language.cc" /* yacc.c:1646  */
    break;

  case 92:
#line 1240 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (CaseInsensitiveEqual((yyvsp[0].str), "left") || CaseInsensitiveEqual((yyvsp[0].str), "right"))
        csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    else
        csh.AddCSH_Error((yylsp[0]), "Use either `left comment` or `right comment` to specify which comment column to size.");
    if (csh.CheckEntityHintAt((yylsp[0]))) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "left comment", HINT_KEYWORD));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "right comment", HINT_KEYWORD));
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).start, "Use <entity>-<entity>, `left comment`, `right comment` to specify horizontal spacing. Ignoring command.");
    (yyval.namerel) = NULL;
  #endif
    free((yyvsp[0].str));
}
#line 4156 "language.cc" /* yacc.c:1646  */
    break;

  case 93:
#line 1258 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckEntityHintAt((yylsp[0]))) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "left comment", HINT_KEYWORD));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "right comment", HINT_KEYWORD));
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).start, "Use `left comment` or `right comment` to specify horizontal spacing for comments. Ignoring command.");
    (yyval.namerel) = NULL;
  #endif
    free((yyvsp[0].str));
}
#line 4175 "language.cc" /* yacc.c:1646  */
    break;

  case 95:
#line 1276 "language.yy" /* yacc.c:1646  */
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
#line 4190 "language.cc" /* yacc.c:1646  */
    break;

  case 96:
#line 1287 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_ATTRVALUE);
  #else
    ((yyvsp[0].attriblist))->Append(new Attribute("space", atof((yyvsp[-1].str)), MSC_POS((yylsp[-1])), MSC_POS((yylsp[-1])), (yyvsp[-1].str)));
    (yyval.attriblist) = (yyvsp[0].attriblist);
  #endif
    free((yyvsp[-1].str));
}
#line 4204 "language.cc" /* yacc.c:1646  */
    break;

  case 97:
#line 1298 "language.yy" /* yacc.c:1646  */
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
#line 4220 "language.cc" /* yacc.c:1646  */
    break;

  case 98:
#line 1310 "language.yy" /* yacc.c:1646  */
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
#line 4235 "language.cc" /* yacc.c:1646  */
    break;

  case 99:
#line 1321 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.namerel) = NULL;
  #endif
}
#line 4248 "language.cc" /* yacc.c:1646  */
    break;

  case 100:
#line 1330 "language.yy" /* yacc.c:1646  */
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
#line 4266 "language.cc" /* yacc.c:1646  */
    break;

  case 101:
#line 1344 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
    csh.CheckEntityHintAt((yylsp[0]));
  #else
    (yyval.namerel) = new NamePair((yyvsp[0].str), MSC_POS((yylsp[0])), NULL, FileLineColRange());
  #endif
    free((yyvsp[0].str));
}
#line 4280 "language.cc" /* yacc.c:1646  */
    break;

  case 102:
#line 1369 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_MARKERNAME);
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    csh.CheckHintAt((yylsp[-1]), HINT_MARKER);
    csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    (yyval.namerel) = new NamePair((yyvsp[-1].str), MSC_POS((yylsp[-1])), NULL, FileLineColRange());
  #endif
    free((yyvsp[-1].str));
}
#line 4296 "language.cc" /* yacc.c:1646  */
    break;

  case 103:
#line 1381 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    csh.AddCSH((yylsp[0]), COLOR_MARKERNAME);
    csh.CheckHintAt((yylsp[0]), HINT_MARKER);
  #else
    (yyval.namerel) = new NamePair(NULL, FileLineColRange(), (yyvsp[0].str), MSC_POS((yylsp[0])));
  #endif
    free((yyvsp[0].str));
}
#line 4311 "language.cc" /* yacc.c:1646  */
    break;

  case 104:
#line 1392 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    csh.CheckHintAt((yylsp[0]), HINT_MARKER);
    csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    (yyval.namerel) = NULL;
  #endif
}
#line 4325 "language.cc" /* yacc.c:1646  */
    break;

  case 105:
#line 1402 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_MARKERNAME);
    csh.CheckHintAt((yylsp[-2]), HINT_MARKER);
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    csh.AddCSH((yylsp[0]), COLOR_MARKERNAME);
    csh.CheckHintAt((yylsp[0]), HINT_MARKER);
  #else
    (yyval.namerel) = new NamePair((yyvsp[-2].str), MSC_POS((yylsp[-2])), (yyvsp[0].str), MSC_POS((yylsp[0])));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 4343 "language.cc" /* yacc.c:1646  */
    break;

  case 110:
#line 1419 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[0].arcbase)) {
        (yyval.arclist) = (new ArcList)->Append((yyvsp[0].arcbase)); /* New list */
        //($1)->MakeMeLastNotable(); Do not make chart options notable
    } else
        (yyval.arclist) = NULL;
  #endif
}
#line 4357 "language.cc" /* yacc.c:1646  */
    break;

  case 111:
#line 1429 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_COMMA);
    if (csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), HINT_ATTR_NAME)) {
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
#line 4380 "language.cc" /* yacc.c:1646  */
    break;

  case 112:
#line 1448 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_COMMA);
    if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, HINT_ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.arclist) = (yyvsp[-1].arclist);
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Expecting an option here.");
  #endif
}
#line 4397 "language.cc" /* yacc.c:1646  */
    break;

  case 114:
#line 1462 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_COMMA);
    csh.AddCSH_Error((yylsp[0]), "An option expected here.");
  #else
    (yyval.arclist) = (yyvsp[-2].arclist);
  #endif
}
#line 4410 "language.cc" /* yacc.c:1646  */
    break;

  case 115:
#line 1473 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName((yylsp[-2]), (yyvsp[-2].str), COLOR_OPTIONNAME);
    csh.AddCSH((yylsp[-1]), COLOR_EQUAL);
    csh.AddCSH((yylsp[0]), COLOR_ATTRVALUE);
    if (csh.CheckHintAt((yylsp[-2]), HINT_ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), HINT_ATTR_VALUE, (yyvsp[-2].str))) {
        Msc::AttributeValues((yyvsp[-2].str), csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.arcbase) = msc.AddAttribute(Attribute((yyvsp[-2].str), str2bool((yyvsp[0].str)), MSC_POS((yylsp[-2])), MSC_POS((yylsp[0])), (yyvsp[0].str)));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 4433 "language.cc" /* yacc.c:1646  */
    break;

  case 116:
#line 1492 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName((yylsp[-2]), (yyvsp[-2].str), COLOR_OPTIONNAME);
    csh.AddCSH((yylsp[-1]), COLOR_EQUAL);
    csh.AddCSH((yylsp[0]), COLOR_ATTRVALUE);
    if (csh.CheckHintAt((yylsp[-2]), HINT_ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), HINT_ATTR_VALUE, (yyvsp[-2].str))) {
        Msc::AttributeValues((yyvsp[-2].str), csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.arcbase) = msc.AddAttribute(Attribute((yyvsp[-2].str), atof((yyvsp[0].str)), MSC_POS((yylsp[-2])), MSC_POS((yylsp[0])), (yyvsp[0].str)));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 4456 "language.cc" /* yacc.c:1646  */
    break;

  case 117:
#line 1511 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName((yylsp[-2]), (yyvsp[-2].str), COLOR_OPTIONNAME);
    csh.AddCSH((yylsp[-1]), COLOR_EQUAL);
    csh.AddCSH_AttrValue((yylsp[0]), (yyvsp[0].str), (yyvsp[-2].str));
    if (csh.CheckHintAt((yylsp[-2]), HINT_ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), HINT_ATTR_VALUE, (yyvsp[-2].str))) {
        Msc::AttributeValues((yyvsp[-2].str), csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.arcbase) = msc.AddAttribute(Attribute((yyvsp[-2].str), (yyvsp[0].str), MSC_POS((yylsp[-2])), MSC_POS((yylsp[0]))));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 4479 "language.cc" /* yacc.c:1646  */
    break;

  case 118:
#line 1530 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName((yylsp[-1]), (yyvsp[-1].str), COLOR_OPTIONNAME);
    csh.AddCSH((yylsp[0]), COLOR_EQUAL);
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing option value.");
    if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, HINT_ATTR_VALUE, (yyvsp[-1].str))) {
        Msc::AttributeValues((yyvsp[-1].str), csh);
        csh.hintStatus = HINT_READY;
    }
    if (csh.CheckHintAt((yylsp[-1]), HINT_ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing option value.");
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[-1].str));
}
#line 4503 "language.cc" /* yacc.c:1646  */
    break;

  case 119:
#line 1550 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[-1]), COLOR_EQUAL);
        csh.AddCSH((yylsp[0]), COLOR_DESIGNNAME);
        if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), HINT_ATTR_VALUE, "msc")) {
            csh.AddDesignsToHints(true);
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
#line 4526 "language.cc" /* yacc.c:1646  */
    break;

  case 120:
#line 1569 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[0]), COLOR_EQUAL);
        csh.AddCSH_ErrorAfter((yylsp[0]), "Missing option value.");
        if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, HINT_ATTR_VALUE, "msc")) {
            csh.AddDesignsToHints(true);
            csh.hintStatus = HINT_READY;
        }
        if (csh.CheckHintAt((yylsp[-1]), HINT_ATTR_NAME)) {
            csh.AddOptionsToHints();
            csh.hintStatus = HINT_READY;
        }
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing option value.");
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[-1].str));
}
#line 4550 "language.cc" /* yacc.c:1646  */
    break;

  case 121:
#line 1589 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[-1]), COLOR_EQUAL);
        csh.AddCSH((yylsp[0]), COLOR_DESIGNNAME);
        if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), HINT_ATTR_VALUE, "msc+")) {
            csh.AddDesignsToHints(false);
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
#line 4573 "language.cc" /* yacc.c:1646  */
    break;

  case 122:
#line 1608 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[0]), COLOR_EQUAL);
        csh.AddCSH_ErrorAfter((yylsp[0]), "Missing option value.");
        if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, HINT_ATTR_VALUE, "msc+")) {
            csh.AddDesignsToHints(false);
            csh.hintStatus = HINT_READY;
        }
        if (csh.CheckHintAt((yylsp[-1]), HINT_ATTR_NAME)) {
            csh.AddOptionsToHints();
            csh.hintStatus = HINT_READY;
        }
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing option value.");
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[-1].str));
}
#line 4597 "language.cc" /* yacc.c:1646  */
    break;

  case 123:
#line 1629 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.entitylist) = ((yyvsp[0].entitylist));
  #endif
}
#line 4607 "language.cc" /* yacc.c:1646  */
    break;

  case 124:
#line 1635 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_COMMA);
  #else
    ((yyvsp[0].entitylist))->Prepend((yyvsp[-2].entitylist));
    (yyval.entitylist) = (yyvsp[0].entitylist);
    delete ((yyvsp[-2].entitylist));
  #endif
}
#line 4621 "language.cc" /* yacc.c:1646  */
    break;

  case 125:
#line 1645 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_COMMA);
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.entitylist) = ((yyvsp[-1].entitylist));
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Expecting an entity here.");
  #endif
}
#line 4635 "language.cc" /* yacc.c:1646  */
    break;

  case 127:
#line 1660 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[-1]));
    csh.AddCSH_EntityName((yylsp[-1]), (yyvsp[-1].str));
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[0])))
        EntityApp::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[0])))
        EntityApp::AttributeValues(csh.hintAttrName, csh);
  #else
    EntityApp *ed = new EntityApp((yyvsp[-1].str), &msc);
    ed->SetLineEnd(MSC_POS((yyloc)));
    (yyval.entitylist) = ed->AddAttributeList((yyvsp[0].attriblist), NULL, FileLineCol());
  #endif
    free((yyvsp[-1].str));
}
#line 4655 "language.cc" /* yacc.c:1646  */
    break;

  case 128:
#line 1676 "language.yy" /* yacc.c:1646  */
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
#line 4671 "language.cc" /* yacc.c:1646  */
    break;

  case 129:
#line 1688 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[-2]));
    csh.AddCSH_EntityName((yylsp[-2]), (yyvsp[-2].str));
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[-1])))
        EntityApp::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[-1])))
        EntityApp::AttributeValues(csh.hintAttrName, csh);
  #else
    EntityApp *ed = new EntityApp((yyvsp[-2].str), &msc);
    ed->SetLineEnd(MSC_POS2((yylsp[-2]), (yylsp[-1])));
    (yyval.entitylist) = ed->AddAttributeList((yyvsp[-1].attriblist), (yyvsp[0].arclist), MSC_POS((yylsp[0])).start);
  #endif
    free((yyvsp[-2].str));
}
#line 4691 "language.cc" /* yacc.c:1646  */
    break;

  case 130:
#line 1704 "language.yy" /* yacc.c:1646  */
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
#line 4707 "language.cc" /* yacc.c:1646  */
    break;

  case 131:
#line 1717 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintAt((yylsp[-1]), HINT_LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[0])))
        EntityApp::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[0])))
        EntityApp::AttributeValues(csh.hintAttrName, csh);
    csh.AddCSH_KeywordOrEntity((yylsp[-1]), (yyvsp[-1].str));  //Do it after AddLineBeginToHints so this one is not included
  #else
    EntityApp *ed = new EntityApp((yyvsp[-1].str), &msc);
    ed->SetLineEnd(MSC_POS((yyloc)));
    (yyval.entitylist) = ed->AddAttributeList((yyvsp[0].attriblist), NULL, FileLineCol());
  #endif
    free((yyvsp[-1].str));
}
#line 4729 "language.cc" /* yacc.c:1646  */
    break;

  case 132:
#line 1735 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
      if (csh.CheckHintAt((yylsp[0]), HINT_LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_KeywordOrEntity((yylsp[0]), (yyvsp[0].str));   //Do it after AddLineBeginToHints so this one is not included
  #else
    EntityApp *ed = new EntityApp((yyvsp[0].str), &msc);
    ed->SetLineEnd(MSC_POS((yyloc)));
    (yyval.entitylist) = ed->AddAttributeList(NULL, NULL, FileLineCol());
  #endif
    free((yyvsp[0].str));
}
#line 4748 "language.cc" /* yacc.c:1646  */
    break;

  case 133:
#line 1750 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintAt((yylsp[-2]), HINT_LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[-1])))
        EntityApp::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[-1])))
        EntityApp::AttributeValues(csh.hintAttrName, csh);
    csh.AddCSH_KeywordOrEntity((yylsp[-2]), (yyvsp[-2].str));  //Do it after AddLineBeginToHints so this one is not included
  #else
    EntityApp *ed = new EntityApp((yyvsp[-2].str), &msc);
    ed->SetLineEnd(MSC_POS2((yylsp[-2]), (yylsp[-1])));
    (yyval.entitylist) = ed->AddAttributeList((yyvsp[-1].attriblist), (yyvsp[0].arclist), MSC_POS((yylsp[0])).start);
  #endif
    free((yyvsp[-2].str));
}
#line 4770 "language.cc" /* yacc.c:1646  */
    break;

  case 134:
#line 1768 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
      if (csh.CheckHintAt((yylsp[-1]), HINT_LINE_START)) {
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
#line 4789 "language.cc" /* yacc.c:1646  */
    break;

  case 136:
#line 1785 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_COMMA);
  #endif
}
#line 4799 "language.cc" /* yacc.c:1646  */
    break;

  case 137:
#line 1792 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    for (auto &str : *((yyvsp[-1].stringlist)))
        if (csh.ForbiddenStyles.find(str) == csh.ForbiddenStyles.end())
            csh.Contexts.back().StyleNames.insert(str);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[0])))
        MscStyle().AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[0])))
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
#line 4851 "language.cc" /* yacc.c:1646  */
    break;

  case 138:
#line 1840 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
	csh.AddCSH_ErrorAfter((yyloc), "Missing attribute definitons in brackets ('[' and ']').");
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing attribute definitons in brackets ('[' and ']').");
  #endif
    delete((yyvsp[0].stringlist));
}
#line 4864 "language.cc" /* yacc.c:1646  */
    break;

  case 139:
#line 1851 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_STYLENAME);
    (yyval.stringlist) = new std::list<string>;
    if (strcmp((yyvsp[0].str), "emphasis")==0)
        ((yyval.stringlist))->push_back("box");
    else if (strcmp((yyvsp[0].str), "emptyemphasis")==0)
        ((yyval.stringlist))->push_back("emptybox");
    else ((yyval.stringlist))->push_back((yyvsp[0].str));
    if (csh.CheckHintAt((yylsp[0]), HINT_ATTR_VALUE)) {
		csh.AddStylesToHints(true);
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.stringlist) = new std::list<string>;
    ((yyval.stringlist))->push_back(ConvertEmphasisToBox((yyvsp[0].str), &(yylsp[0]), msc));
  #endif
    free((yyvsp[0].str));
}
#line 4888 "language.cc" /* yacc.c:1646  */
    break;

  case 140:
#line 1871 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_STYLENAME);
    (yyval.stringlist) = new std::list<string>;
	((yyval.stringlist))->push_back("++");
    if (csh.CheckHintAt((yylsp[0]), HINT_ATTR_VALUE)) {
		csh.AddStylesToHints(true);
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.stringlist) = new std::list<string>;
    ((yyval.stringlist))->push_back(ConvertEmphasisToBox("++", &(yylsp[0]), msc));
  #endif
}
#line 4907 "language.cc" /* yacc.c:1646  */
    break;

  case 141:
#line 1886 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_COMMA);
	csh.AddCSH_ErrorAfter((yylsp[0]), "Missing a style name to (re-)define.");
    if (csh.CheckHintAfterPlusOne((yylsp[-1]), yylloc, yychar==YYEOF, HINT_ATTR_VALUE)) {
		csh.AddStylesToHints(true);
        csh.hintStatus = HINT_READY;
    }
	(yyval.stringlist) = (yyvsp[-1].stringlist);
  #else
    (yyval.stringlist) = (yyvsp[-1].stringlist);
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing a style name to (re-)define.");
  #endif
}
#line 4926 "language.cc" /* yacc.c:1646  */
    break;

  case 142:
#line 1901 "language.yy" /* yacc.c:1646  */
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
    if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), HINT_ATTR_VALUE)) {
		csh.AddStylesToHints(true);
        csh.hintStatus = HINT_READY;
    }
  #else
    ((yyvsp[-2].stringlist))->push_back(ConvertEmphasisToBox((yyvsp[0].str), &(yylsp[0]), msc));
    (yyval.stringlist) = (yyvsp[-2].stringlist);
  #endif
    free((yyvsp[0].str));
}
#line 4951 "language.cc" /* yacc.c:1646  */
    break;

  case 143:
#line 1922 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_COMMA);
    csh.AddCSH((yylsp[0]), COLOR_STYLENAME);
    (yyval.stringlist) = (yyvsp[-2].stringlist);
	((yyval.stringlist))->push_back("++");
    if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), HINT_ATTR_VALUE)) {
		csh.AddStylesToHints(true);
        csh.hintStatus = HINT_READY;
    }
  #else
    ((yyvsp[-2].stringlist))->push_back(ConvertEmphasisToBox("++", &(yylsp[0]), msc));
    (yyval.stringlist) = (yyvsp[-2].stringlist);
  #endif
}
#line 4971 "language.cc" /* yacc.c:1646  */
    break;

  case 144:
#line 1939 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_ATTRVALUE);
    csh.AddCSH_ErrorAfter((yyloc), ("Here should come a shape definition beginning with '{'. Ignoring this malformed shape definition for '"+string((yyvsp[0].str) ? (yyvsp[0].str) : "") +"'.").c_str());
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Here should come a shape definition beginning with '{'. Ignoring this malformed shape definition for '"+string((yyvsp[0].str) ? (yyvsp[0].str) : "") +"'.");
  #endif	
  free((yyvsp[0].str));
}
#line 4985 "language.cc" /* yacc.c:1646  */
    break;

  case 145:
#line 1949 "language.yy" /* yacc.c:1646  */
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
#line 5000 "language.cc" /* yacc.c:1646  */
    break;

  case 146:
#line 1960 "language.yy" /* yacc.c:1646  */
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
#line 5021 "language.cc" /* yacc.c:1646  */
    break;

  case 147:
#line 1977 "language.yy" /* yacc.c:1646  */
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
#line 5040 "language.cc" /* yacc.c:1646  */
    break;

  case 148:
#line 1992 "language.yy" /* yacc.c:1646  */
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
#line 5060 "language.cc" /* yacc.c:1646  */
    break;

  case 149:
#line 2009 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.shape) = new Shape;
	if ((yyvsp[0].shapeelement)) {
		((yyval.shape))->Add(std::move(*((yyvsp[0].shapeelement))));
		delete (yyvsp[0].shapeelement);
	}
  #endif	
}
#line 5074 "language.cc" /* yacc.c:1646  */
    break;

  case 150:
#line 2019 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[-1]), "I do not understand this.");
  #else
    (yyval.shape) = new Shape;
	if ((yyvsp[0].shapeelement)) {
		((yyval.shape))->Add(std::move(*((yyvsp[0].shapeelement))));
		delete (yyvsp[0].shapeelement);
	}
  #endif	
}
#line 5090 "language.cc" /* yacc.c:1646  */
    break;

  case 151:
#line 2031 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
	if ((yyvsp[0].shapeelement)) {
		((yyvsp[-1].shape))->Add(std::move(*((yyvsp[0].shapeelement))));
		delete (yyvsp[0].shapeelement);
	}
  #endif	
}
#line 5103 "language.cc" /* yacc.c:1646  */
    break;

  case 152:
#line 2040 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[0]), "Only numbers can come after shape commands.");
  #else
  #endif	
}
#line 5114 "language.cc" /* yacc.c:1646  */
    break;

  case 153:
#line 2048 "language.yy" /* yacc.c:1646  */
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
#line 5134 "language.cc" /* yacc.c:1646  */
    break;

  case 154:
#line 2064 "language.yy" /* yacc.c:1646  */
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
#line 5166 "language.cc" /* yacc.c:1646  */
    break;

  case 155:
#line 2092 "language.yy" /* yacc.c:1646  */
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
#line 5194 "language.cc" /* yacc.c:1646  */
    break;

  case 156:
#line 2116 "language.yy" /* yacc.c:1646  */
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
#line 5224 "language.cc" /* yacc.c:1646  */
    break;

  case 157:
#line 2142 "language.yy" /* yacc.c:1646  */
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
#line 5256 "language.cc" /* yacc.c:1646  */
    break;

  case 158:
#line 2170 "language.yy" /* yacc.c:1646  */
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
#line 5290 "language.cc" /* yacc.c:1646  */
    break;

  case 159:
#line 2200 "language.yy" /* yacc.c:1646  */
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
#line 5326 "language.cc" /* yacc.c:1646  */
    break;

  case 161:
#line 2234 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_COMMA);
    if (csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), HINT_ATTR_VALUE)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
	}
  #endif
}
#line 5340 "language.cc" /* yacc.c:1646  */
    break;

  case 162:
#line 2244 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_COMMA);
    if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, HINT_ATTR_VALUE)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
	}
	csh.AddCSH_ErrorAfter((yyloc), "Missing color name to (re-)define.");
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing a color name to (re-)define.");
  #endif
}
#line 5357 "language.cc" /* yacc.c:1646  */
    break;

  case 165:
#line 2260 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_COLORNAME);
    csh.AddCSH((yylsp[-1]), COLOR_EQUAL);
    csh.AddCSH((yylsp[0]), COLOR_COLORDEF);
    ColorType color = csh.Contexts.back().Colors.GetColor((yyvsp[0].str));
    if (color.type!=ColorType::INVALID)
        csh.Contexts.back().Colors[(yyvsp[-2].str)] = color;
    if (csh.CheckHintAt((yylsp[-2]), HINT_ATTR_VALUE)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), HINT_ATTR_VALUE)) {
        csh.AddColorValuesToHints(false);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Contexts.back().colors.AddColor((yyvsp[-2].str), (yyvsp[0].str), msc.Error, MSC_POS((yyloc)));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 5383 "language.cc" /* yacc.c:1646  */
    break;

  case 166:
#line 2282 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-3]), COLOR_COLORNAME);
    csh.AddCSH((yylsp[-2]), COLOR_EQUAL);
    csh.AddCSH((yylsp[-1]), COLOR_COLORDEF);
    csh.AddCSH((yylsp[0]), COLOR_COLORDEF);
    ColorType color = csh.Contexts.back().Colors.GetColor("++"+string((yyvsp[0].str)));
    if (color.type!=ColorType::INVALID)
        csh.Contexts.back().Colors[(yyvsp[-3].str)] = color;
    if (csh.CheckHintAt((yylsp[-3]), HINT_ATTR_VALUE)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-2]), (yylsp[0]), HINT_ATTR_VALUE)) {
        csh.AddColorValuesToHints(false);
        csh.hintStatus = HINT_READY;
    }
#else
    msc.Contexts.back().colors.AddColor((yyvsp[-3].str), "++"+string((yyvsp[0].str)), msc.Error, MSC_POS((yyloc)));
  #endif
    free((yyvsp[-3].str));
    free((yyvsp[0].str));
}
#line 5410 "language.cc" /* yacc.c:1646  */
    break;

  case 167:
#line 2305 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_COLORNAME);
    csh.AddCSH((yylsp[0]), COLOR_EQUAL);
    if (csh.CheckHintAt((yylsp[-1]), HINT_ATTR_VALUE)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, HINT_ATTR_VALUE)) {
        csh.AddColorValuesToHints(false);
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_ErrorAfter((yyloc), "Missing color definition.");
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing color definition.");
  #endif
    free((yyvsp[-1].str));
}
#line 5432 "language.cc" /* yacc.c:1646  */
    break;

  case 168:
#line 2323 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_COLORNAME);
    if (csh.CheckHintAt((yylsp[0]), HINT_ATTR_VALUE)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
	}
	csh.AddCSH_ErrorAfter((yyloc), "Missing equal sign ('=') and a color definition.");
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing equal sign ('=') and a color definition.");
  #endif
    free((yyvsp[0].str));
}
#line 5450 "language.cc" /* yacc.c:1646  */
    break;

  case 169:
#line 2340 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-4]), COLOR_DESIGNNAME);
    csh.AddCSH((yylsp[-1]), COLOR_SEMICOLON);
    csh.AddCSH((yylsp[0]), COLOR_BRACE);
    (csh.Contexts.back().full ? csh.FullDesigns : csh.PartialDesigns)[(yyvsp[-4].str)] += csh.Contexts.back();
    csh.PopContext();
  #else
    //cope_open_empty pushed an empty color & style set onto the stack
    //then designelementlist added color & style definitions, now we harvest those
    msc.Designs[(yyvsp[-4].str)] += msc.Contexts.back();
    msc.PopContext();
  #endif
    free((yyvsp[-4].str));
}
#line 5470 "language.cc" /* yacc.c:1646  */
    break;

  case 170:
#line 2356 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-5]), COLOR_DESIGNNAME);
    csh.AddCSH((yylsp[-2]), COLOR_SEMICOLON);
    csh.AddCSH_Error((yylsp[-1]), "Could not recognize this as part of a design definition.");
    csh.AddCSH((yylsp[0]), COLOR_BRACE);
    (csh.Contexts.back().full ? csh.FullDesigns : csh.PartialDesigns)[(yyvsp[-5].str)] = csh.Contexts.back();
    csh.PopContext();
  #else
    //if closing brace missing, still do the design definition
    //cope_open_empty pushed an empty color & style set onto the stack
    //then designelementlist added color & style definitions, now we harvest those
    msc.Designs[(yyvsp[-5].str)] += msc.Contexts.back();
    msc.PopContext();
  #endif
}
#line 5491 "language.cc" /* yacc.c:1646  */
    break;

  case 171:
#line 2375 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_BRACE);
    csh.PushContext(true);
  #else
    //push empty color & style sets for design definition
    msc.PushContext(true);
  #endif
}
#line 5505 "language.cc" /* yacc.c:1646  */
    break;

  case 173:
#line 2387 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_SEMICOLON);
  #endif
}
#line 5515 "language.cc" /* yacc.c:1646  */
    break;

  case 174:
#line 2394 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
  #endif
    free((yyvsp[-1].str));
}
#line 5526 "language.cc" /* yacc.c:1646  */
    break;

  case 175:
#line 2401 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, HINT_ATTR_VALUE)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
	}
	csh.AddCSH_ErrorAfter((yyloc), "Missing color name to (re-)define.");
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing a color name to (re-)define.");
  #endif
    free((yyvsp[0].str));
}
#line 5544 "language.cc" /* yacc.c:1646  */
    break;

  case 176:
#line 2415 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintBetweenPlusOne((yylsp[-1]), (yylsp[0]), HINT_ATTR_NAME)) {
        csh.AddStylesToHints(true);
        csh.hintStatus = HINT_READY;
	}
  #endif
    free((yyvsp[-1].str));
}
#line 5559 "language.cc" /* yacc.c:1646  */
    break;

  case 177:
#line 2426 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, HINT_ATTR_VALUE)) {
        csh.AddStylesToHints(true);
        csh.hintStatus = HINT_READY;
	}
	csh.AddCSH_ErrorAfter((yyloc), "Missing style name to (re-)define.");
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing a style name to (re-)define.");
  #endif
    free((yyvsp[0].str));
}
#line 5577 "language.cc" /* yacc.c:1646  */
    break;

  case 180:
#line 2443 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_COMMA);
  #endif
}
#line 5587 "language.cc" /* yacc.c:1646  */
    break;

  case 181:
#line 2449 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[0]), "Extra stuff after design options. Maybe missing a comma?");
  #endif
}
#line 5597 "language.cc" /* yacc.c:1646  */
    break;

  case 182:
#line 2456 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName((yylsp[-2]), (yyvsp[-2].str), COLOR_OPTIONNAME);
    csh.AddCSH((yylsp[-1]), COLOR_EQUAL);
    csh.AddCSH((yylsp[0]), COLOR_ATTRVALUE);
    if (csh.CheckHintAt((yylsp[-2]), HINT_ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), HINT_ATTR_VALUE, (yyvsp[-2].str))) {
        Msc::AttributeValues((yyvsp[-2].str), csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.AddDesignAttribute(Attribute((yyvsp[-2].str), str2bool((yyvsp[0].str)), MSC_POS((yyloc)), MSC_POS((yylsp[0])), (yyvsp[0].str)));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 5620 "language.cc" /* yacc.c:1646  */
    break;

  case 183:
#line 2475 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName((yylsp[-2]), (yyvsp[-2].str), COLOR_OPTIONNAME);
    csh.AddCSH((yylsp[-1]), COLOR_EQUAL);
    csh.AddCSH((yylsp[0]), COLOR_ATTRVALUE);
    if (csh.CheckHintAt((yylsp[-2]), HINT_ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), HINT_ATTR_VALUE, (yyvsp[-2].str))) {
        Msc::AttributeValues((yyvsp[-2].str), csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.AddDesignAttribute(Attribute((yyvsp[-2].str), atof((yyvsp[0].str)), MSC_POS((yyloc)), MSC_POS((yylsp[0])), (yyvsp[0].str)));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 5643 "language.cc" /* yacc.c:1646  */
    break;

  case 184:
#line 2494 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName((yylsp[-2]), (yyvsp[-2].str), COLOR_OPTIONNAME);
    csh.AddCSH((yylsp[-1]), COLOR_EQUAL);
    csh.AddCSH((yylsp[0]), COLOR_ATTRVALUE);
    if (csh.CheckHintAt((yylsp[-2]), HINT_ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), HINT_ATTR_VALUE, (yyvsp[-2].str))) {
        Msc::AttributeValues((yyvsp[-2].str), csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.AddDesignAttribute(Attribute((yyvsp[-2].str), (yyvsp[0].str), MSC_POS((yylsp[-2])), MSC_POS((yylsp[0]))));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 5666 "language.cc" /* yacc.c:1646  */
    break;

  case 185:
#line 2513 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[-1]), COLOR_EQUAL);
    csh.AddCSH((yylsp[0]), COLOR_DESIGNNAME);
    if (csh.CheckHintAt((yylsp[-2]), HINT_ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), HINT_ATTR_VALUE, (yyvsp[-2].str))) {
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
#line 5692 "language.cc" /* yacc.c:1646  */
    break;

  case 186:
#line 2535 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[-1]), COLOR_EQUAL);
    csh.AddCSH((yylsp[0]), COLOR_DESIGNNAME);
    if (csh.CheckHintAt((yylsp[-2]), HINT_ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), HINT_ATTR_VALUE, (yyvsp[-2].str))) {
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
#line 5718 "language.cc" /* yacc.c:1646  */
    break;

  case 187:
#line 2559 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[0].arclist))
        (yyval.arcparallel) = new ArcParallel(&msc, (yyvsp[0].arclist));
    else
        (yyval.arcparallel) = NULL;
  #endif
}
#line 5731 "language.cc" /* yacc.c:1646  */
    break;

  case 188:
#line 2568 "language.yy" /* yacc.c:1646  */
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
#line 5746 "language.cc" /* yacc.c:1646  */
    break;

  case 189:
#line 2580 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[0].arcbox))->SetLineEnd(MSC_POS((yyloc)));
    (yyval.arcboxseries) = new ArcBoxSeries((yyvsp[0].arcbox));
  #endif
}
#line 5757 "language.cc" /* yacc.c:1646  */
    break;

  case 190:
#line 2587 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    csh.CheckEntityHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcboxseries) = NULL;
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "The keyword '" + string((yyvsp[0].str)) +"' should be followed by an entity, or '--', '..', '++' or '=='.");
  #endif
    free((yyvsp[0].str));
}
#line 5772 "language.cc" /* yacc.c:1646  */
    break;

  case 191:
#line 2598 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);   
  #else
    ((yyvsp[0].arcbox))->ExpandLineEnd(MSC_POS((yyloc)));
    (yyval.arcboxseries) = new ArcBoxSeries((yyvsp[0].arcbox));
  #endif
  free((yyvsp[-1].str));
}
#line 5786 "language.cc" /* yacc.c:1646  */
    break;

  case 192:
#line 2609 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[0].arcbox))->SetLineEnd(MSC_POS((yylsp[0])));
    (yyval.arcboxseries) = ((yyvsp[-1].arcboxseries))->AddFollow((yyvsp[0].arcbox));
    ((yyvsp[0].arcbox))->AddAttributeList(NULL); //should come after AddFollow
  #endif
}
#line 5798 "language.cc" /* yacc.c:1646  */
    break;

  case 193:
#line 2617 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[0])))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[0])))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
  #else
    ((yyvsp[-1].arcbox))->SetLineEnd(MSC_POS2((yylsp[-1]), (yylsp[0])));
    (yyval.arcboxseries) = ((yyvsp[-2].arcboxseries))->AddFollow((yyvsp[-1].arcbox));
    ((yyvsp[-1].arcbox))->AddAttributeList((yyvsp[0].attriblist)); //should come after AddFollow
  #endif
}
#line 5815 "language.cc" /* yacc.c:1646  */
    break;

  case 194:
#line 2630 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[-1].arcbox))->AddArcList((yyvsp[0].arclist))->SetLineEnd(MSC_POS((yylsp[-1])));
    (yyval.arcboxseries) = ((yyvsp[-2].arcboxseries))->AddFollow((yyvsp[-1].arcbox));
    ((yyvsp[-1].arcbox))->AddAttributeList(NULL); //should come after AddFollow
  #endif
}
#line 5827 "language.cc" /* yacc.c:1646  */
    break;

  case 195:
#line 2638 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    ArcBox *temp = new ArcBox(MSC_BOX_UNDETERMINED_FOLLOW, NULL, MSC_POS((yylsp[-1])), NULL, MSC_POS((yylsp[-1])), &msc);
    temp->AddArcList((yyvsp[0].arclist));
    (yyval.arcboxseries) = ((yyvsp[-1].arcboxseries))->AddFollow(temp);
    temp->AddAttributeList(NULL); //should come after AddFollow
  #endif
}
#line 5840 "language.cc" /* yacc.c:1646  */
    break;

  case 196:
#line 2647 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[-2])))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[-2])))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
  #else
    ((yyvsp[-2].arcbox))->AddArcList((yyvsp[0].arclist))->SetLineEnd(MSC_POS2((yylsp[-2]), (yylsp[-1])));
    (yyval.arcboxseries) = ((yyvsp[-3].arcboxseries))->AddFollow((yyvsp[-2].arcbox));
    ((yyvsp[-2].arcbox))->AddAttributeList((yyvsp[-1].attriblist)); //should come after AddFollow
  #endif
}
#line 5857 "language.cc" /* yacc.c:1646  */
    break;

  case 197:
#line 2660 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[-1])))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[-1])))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
  #else
    ArcBox *temp = new ArcBox(MSC_BOX_UNDETERMINED_FOLLOW, NULL, MSC_POS((yylsp[-2])), NULL, MSC_POS((yylsp[-2])), &msc);
    temp->AddArcList((yyvsp[0].arclist))->SetLineEnd(MSC_POS((yylsp[-1])));
    (yyval.arcboxseries) = ((yyvsp[-2].arcboxseries))->AddFollow(temp);
    temp->AddAttributeList((yyvsp[-1].attriblist)); //should come after AddFollow
  #endif
}
#line 5875 "language.cc" /* yacc.c:1646  */
    break;

  case 198:
#line 2676 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[0].arcbox))->AddAttributeList(NULL);
    ((yyvsp[0].arcbox))->SetLineEnd(MSC_POS((yyloc)), false);
    (yyval.arcbox) = (yyvsp[0].arcbox);
  #endif
}
#line 5887 "language.cc" /* yacc.c:1646  */
    break;

  case 199:
#line 2684 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[0])))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[0])))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
  #else
    ((yyvsp[-1].arcbox))->AddAttributeList((yyvsp[0].attriblist));
    ((yyvsp[-1].arcbox))->SetLineEnd(MSC_POS((yyloc)), false);
    (yyval.arcbox) = ((yyvsp[-1].arcbox));
  #endif
}
#line 5904 "language.cc" /* yacc.c:1646  */
    break;

  case 200:
#line 2697 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[-1].arcbox))->AddAttributeList(NULL);
    ((yyvsp[-1].arcbox))->SetLineEnd(MSC_POS((yylsp[-1])), false);
    (yyval.arcbox) = ((yyvsp[-1].arcbox))->AddArcList((yyvsp[0].arclist));
  #endif
}
#line 5916 "language.cc" /* yacc.c:1646  */
    break;

  case 201:
#line 2705 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[-1])))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[-1])))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
  #else
    ((yyvsp[-2].arcbox))->SetLineEnd(MSC_POS2((yylsp[-2]), (yylsp[-1])), false);
    ((yyvsp[-2].arcbox))->AddArcList((yyvsp[0].arclist))->AddAttributeList((yyvsp[-1].attriblist));
    (yyval.arcbox) = ((yyvsp[-2].arcbox));
  #endif
}
#line 5933 "language.cc" /* yacc.c:1646  */
    break;

  case 202:
#line 2720 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintBetweenPlusOne((yylsp[-1]), (yylsp[0]), HINT_ENTITY)) {
        csh.hintStatus = HINT_READY;
        csh.AddEntitiesToHints();
    }
  #else
    (yyval.arcpipe) = new ArcPipe((yyvsp[0].arcbox));
    ((yyval.arcpipe))->AddAttributeList(NULL);
    ((yyval.arcpipe))->SetLineEnd(MSC_POS((yyloc)));
  #endif
    free((yyvsp[-1].str));
}
#line 5952 "language.cc" /* yacc.c:1646  */
    break;

  case 203:
#line 2735 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    csh.AddCSH_EntityName((yylsp[-1]), (yyvsp[-1].str));
    if (csh.CheckHintAtAndBeforePlusOne((yylsp[-1]), (yylsp[0]), HINT_ENTITY)) {
        csh.hintStatus = HINT_READY;
        csh.AddEntitiesToHints();
    }
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing a box symbol.");
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing a box symbol. Ignoring pipe.");
    (yyval.arcpipe) = NULL;
  #endif
    free((yyvsp[-1].str));
}
#line 5972 "language.cc" /* yacc.c:1646  */
    break;

  case 204:
#line 2751 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    csh.CheckEntityHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcpipe) = NULL;
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "The keyword '" + string((yyvsp[0].str)) +"' should be followed by an entity, or '--', '..', '++' or '=='.");
  #endif
    free((yyvsp[0].str));
}
#line 5987 "language.cc" /* yacc.c:1646  */
    break;

  case 205:
#line 2762 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    if (csh.CheckHintBetweenPlusOne((yylsp[-2]), (yylsp[-1]), HINT_ENTITY)) {
        csh.hintStatus = HINT_READY;
        csh.AddEntitiesToHints();
    } else if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[0])))
        ArcPipe::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[0])))
        ArcPipe::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcpipe) = new ArcPipe((yyvsp[-1].arcbox));
    ((yyval.arcpipe))->AddAttributeList((yyvsp[0].attriblist));
    ((yyval.arcpipe))->SetLineEnd(MSC_POS((yyloc)));
  #endif
    free((yyvsp[-2].str));
}
#line 6009 "language.cc" /* yacc.c:1646  */
    break;

  case 206:
#line 2781 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    if ((yyvsp[0].arcpipe))
      (yyval.arcpipeseries) = new ArcPipeSeries((yyvsp[0].arcpipe));
    else
      (yyval.arcpipeseries) = NULL;
  #endif
}
#line 6023 "language.cc" /* yacc.c:1646  */
    break;

  case 207:
#line 2791 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
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
#line 6041 "language.cc" /* yacc.c:1646  */
    break;

  case 208:
#line 2805 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[0])))
        ArcPipe::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[0])))
        ArcPipe::AttributeValues(csh.hintAttrName, csh);
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
#line 6064 "language.cc" /* yacc.c:1646  */
    break;

  case 210:
#line 2826 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.arcpipeseries) = ((yyvsp[-1].arcpipeseries))->AddArcList((yyvsp[0].arclist));
  #endif
}
#line 6074 "language.cc" /* yacc.c:1646  */
    break;

  case 213:
#line 2835 "language.yy" /* yacc.c:1646  */
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
#line 6092 "language.cc" /* yacc.c:1646  */
    break;

  case 214:
#line 2849 "language.yy" /* yacc.c:1646  */
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
#line 6107 "language.cc" /* yacc.c:1646  */
    break;

  case 215:
#line 2860 "language.yy" /* yacc.c:1646  */
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
#line 6123 "language.cc" /* yacc.c:1646  */
    break;

  case 216:
#line 2872 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcbox) = new ArcBox((yyvsp[0].arctype), NULL, MSC_POS((yylsp[0])), NULL, MSC_POS((yylsp[0])), &msc);
  #endif
}
#line 6136 "language.cc" /* yacc.c:1646  */
    break;

  case 217:
#line 2882 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[0].str), MSC_POS((yylsp[0])));
  #endif
    free((yyvsp[-1].str));
    free((yyvsp[0].str));
}
#line 6152 "language.cc" /* yacc.c:1646  */
    break;

  case 218:
#line 2894 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[-2]), (yylsp[-1]));
    csh.AddCSH_EntityName((yylsp[-1]), (yyvsp[-1].str));
    csh.AddCSH((yylsp[0]), COLOR_ATTRVALUE);
  #else
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[-1].str), MSC_POS((yylsp[-1])), VertXPos::POS_AT, atof((yyvsp[0].str)));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[-1].str));
    free((yyvsp[0].str));
}
#line 6170 "language.cc" /* yacc.c:1646  */
    break;

  case 219:
#line 2908 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[-2]), (yylsp[-1]));
    csh.AddCSH_EntityName((yylsp[-1]), (yyvsp[-1].str));
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[-1].str), MSC_POS((yylsp[-1])), VertXPos::POS_LEFT_SIDE);
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[-1].str));
}
#line 6188 "language.cc" /* yacc.c:1646  */
    break;

  case 220:
#line 2922 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-3]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[-3]), (yylsp[-2]));
    csh.AddCSH_EntityName((yylsp[-2]), (yyvsp[-2].str));
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    csh.AddCSH((yylsp[0]), COLOR_ATTRVALUE);
  #else
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[-2].str), MSC_POS((yylsp[-2])), VertXPos::POS_LEFT_SIDE, -atof((yyvsp[0].str)));
  #endif
    free((yyvsp[-3].str));
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 6207 "language.cc" /* yacc.c:1646  */
    break;

  case 221:
#line 2937 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[-2]), (yylsp[-1]));
    csh.AddCSH_EntityName((yylsp[-1]), (yyvsp[-1].str));
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
  #else
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[-1].str), MSC_POS((yylsp[-1])), VertXPos::POS_RIGHT_SIDE);
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[-1].str));
}
#line 6224 "language.cc" /* yacc.c:1646  */
    break;

  case 222:
#line 2950 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-3]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[-3]), (yylsp[-2]));
    csh.AddCSH_EntityName((yylsp[-2]), (yyvsp[-2].str));
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    csh.AddCSH((yylsp[0]), COLOR_ATTRVALUE);
  #else
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[-2].str), MSC_POS((yylsp[-2])), VertXPos::POS_RIGHT_SIDE, atof((yyvsp[0].str)));
  #endif
    free((yyvsp[-3].str));
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 6243 "language.cc" /* yacc.c:1646  */
    break;

  case 223:
#line 2965 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[-2]), (yylsp[-1]));
    csh.AddCSH_EntityName((yylsp[-1]), (yyvsp[-1].str));
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
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
#line 6281 "language.cc" /* yacc.c:1646  */
    break;

  case 224:
#line 2999 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-3]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[-3]), (yylsp[-2]));
    csh.AddCSH_EntityName((yylsp[-2]), (yyvsp[-2].str));
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[-2].str), MSC_POS((yylsp[-2])), (yyvsp[0].str), MSC_POS((yylsp[0])));
  #endif
    free((yyvsp[-3].str));
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 6301 "language.cc" /* yacc.c:1646  */
    break;

  case 225:
#line 3015 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-4]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[-4]), (yylsp[-3]));
    csh.AddCSH_EntityName((yylsp[-3]), (yyvsp[-3].str));
    csh.AddCSH((yylsp[-2]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[-2]), (yylsp[-1]));
    csh.AddCSH_EntityName((yylsp[-1]), (yyvsp[-1].str));
    csh.AddCSH((yylsp[0]), COLOR_ATTRVALUE);
  #else
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[-3].str), MSC_POS((yylsp[-3])), (yyvsp[-1].str), MSC_POS((yylsp[-1])), atof((yyvsp[0].str)));
  #endif
    free((yyvsp[-4].str));
    free((yyvsp[-3].str));
    free((yyvsp[-1].str));
    free((yyvsp[0].str));
}
#line 6323 "language.cc" /* yacc.c:1646  */
    break;

  case 226:
#line 3033 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    csh.CheckEntityHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF);
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing an entity name.");
  #else
    (yyval.vertxpos) = NULL;
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing an entity name.");
  #endif
    free((yyvsp[0].str));
}
#line 6339 "language.cc" /* yacc.c:1646  */
    break;

  case 227:
#line 3047 "language.yy" /* yacc.c:1646  */
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
#line 6354 "language.cc" /* yacc.c:1646  */
    break;

  case 228:
#line 3058 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_SYMBOL);
	(yyval.arctypeplusdir) = 0; //dummy to supress warning	
  #else
    ((yyval.arctypeplusdir)).arc.type = (yyvsp[0].arctype);
	((yyval.arctypeplusdir)).arc.lost = EArrowLost::AT_SRC;
    ((yyval.arctypeplusdir)).arc.lost_pos.SetFrom(MSC_POS((yylsp[-1])));
	((yyval.arctypeplusdir)).dir = MSC_DIR_INDETERMINATE;
  #endif
}
#line 6370 "language.cc" /* yacc.c:1646  */
    break;

  case 229:
#line 3070 "language.yy" /* yacc.c:1646  */
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
#line 6386 "language.cc" /* yacc.c:1646  */
    break;

  case 230:
#line 3082 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_SYMBOL);
    csh.AddCSH_Error((yylsp[0]), MULTIPLE_ASTERISK_ERROR_MSG);
	(yyval.arctypeplusdir) = 0; //dummy to supress warning	
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).start, "Only one loss can be specified. Ignoring second asterisk ('*').");
    ((yyval.arctypeplusdir)).arc.type = (yyvsp[-1].arctype);
    ((yyval.arctypeplusdir)).arc.lost = EArrowLost::AT_SRC;
    ((yyval.arctypeplusdir)).arc.lost_pos.SetFrom(MSC_POS((yylsp[-2])));
	((yyval.arctypeplusdir)).dir = MSC_DIR_INDETERMINATE;
  #endif
}
#line 6404 "language.cc" /* yacc.c:1646  */
    break;

  case 231:
#line 3096 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
	(yyval.arctypeplusdir) = 0; //dummy to supress warning
  #else
    ((yyval.arctypeplusdir)).arc = (yyvsp[0].arcsegdata);
	((yyval.arctypeplusdir)).dir = MSC_DIR_LEFT;
  #endif
}
#line 6417 "language.cc" /* yacc.c:1646  */
    break;

  case 232:
#line 3105 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
	(yyval.arctypeplusdir) = 0; //dummy to supress warning
  #else
    ((yyval.arctypeplusdir)).arc = (yyvsp[0].arcsegdata);
	((yyval.arctypeplusdir)).dir = MSC_DIR_RIGHT;
  #endif
}
#line 6430 "language.cc" /* yacc.c:1646  */
    break;

  case 233:
#line 3114 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
	(yyval.arctypeplusdir) = 0; //dummy to supress warning
  #else
    ((yyval.arctypeplusdir)).arc = (yyvsp[0].arcsegdata);
	((yyval.arctypeplusdir)).dir = MSC_DIR_BIDIR;
  #endif
}
#line 6443 "language.cc" /* yacc.c:1646  */
    break;

  case 234:
#line 3124 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_MARKERNAME);
    csh.AddCSH((yylsp[0]), COLOR_MARKERNAME);
    csh.CheckHintAt((yylsp[-2]), HINT_MARKER);
    csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), HINT_MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[-1].arctypeplusdir), (yyvsp[-2].str), (yyvsp[0].str), &msc);
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 6460 "language.cc" /* yacc.c:1646  */
    break;

  case 235:
#line 3137 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_MARKERNAME);
    csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), HINT_MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[-1].arctypeplusdir), MARKER_HERE_STR, (yyvsp[0].str), &msc);
  #endif
    free((yyvsp[0].str));
}
#line 6474 "language.cc" /* yacc.c:1646  */
    break;

  case 236:
#line 3147 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_MARKERNAME);
    csh.CheckHintAt((yylsp[-1]), HINT_MARKER);
    csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[0].arctypeplusdir), (yyvsp[-1].str), MARKER_HERE_STR, &msc);
  #endif
    free((yyvsp[-1].str));
}
#line 6489 "language.cc" /* yacc.c:1646  */
    break;

  case 237:
#line 3158 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[0].arctypeplusdir), MARKER_HERE_STR, MARKER_HERE_STR, &msc);
  #endif
}
#line 6501 "language.cc" /* yacc.c:1646  */
    break;

  case 238:
#line 3166 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_MARKERNAME);
    csh.CheckHintAt((yylsp[0]), HINT_MARKER);
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing a box or arrow symbol.");
  #else
    (yyval.arcvertarrow) = NULL;
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing a box or arrow symbol.");
  #endif
    free((yyvsp[0].str));
}
#line 6517 "language.cc" /* yacc.c:1646  */
    break;

  case 239:
#line 3178 "language.yy" /* yacc.c:1646  */
    {free((yyvsp[-1].str)); (yyval.arcvertarrow)=NULL;}
#line 6523 "language.cc" /* yacc.c:1646  */
    break;

  case 240:
#line 3179 "language.yy" /* yacc.c:1646  */
    {free((yyvsp[-3].str)); free((yyvsp[-1].str)); (yyval.arcvertarrow)=NULL;}
#line 6529 "language.cc" /* yacc.c:1646  */
    break;

  case 241:
#line 3183 "language.yy" /* yacc.c:1646  */
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
#line 6544 "language.cc" /* yacc.c:1646  */
    break;

  case 242:
#line 3194 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
	VertXPos vxp(msc);
	(yyval.arcvertarrow) = ((yyvsp[0].arcvertarrow))->AddXpos(&vxp);
  #endif
}
#line 6556 "language.cc" /* yacc.c:1646  */
    break;

  case 243:
#line 3202 "language.yy" /* yacc.c:1646  */
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
#line 6578 "language.cc" /* yacc.c:1646  */
    break;

  case 244:
#line 3221 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_SYMBOL);
  #else
    (yyval.arcbase) = new ArcDivider((yyvsp[0].arctype), &msc);
  #endif
}
#line 6590 "language.cc" /* yacc.c:1646  */
    break;

  case 245:
#line 3229 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = ((yyvsp[-2].arcarrow))->AddLostPos((yyvsp[0].vertxpos), MSC_POS2((yylsp[-1]), (yylsp[0])));
  #endif
   free((yyvsp[-1].str));
}
#line 6603 "language.cc" /* yacc.c:1646  */
    break;

  case 246:
#line 3238 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing 'at' clause.");
    if (csh.CheckHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF, HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "at", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).end, "Missing 'at' clause.");
    (yyval.arcbase) = ((yyvsp[-1].arcarrow));
  #endif
   free((yyvsp[0].str));
}
#line 6622 "language.cc" /* yacc.c:1646  */
    break;

  case 247:
#line 3253 "language.yy" /* yacc.c:1646  */
    {
    //explicit copy here to suppress bison warning (since the two types are different)
    (yyval.arcbase) = (yyvsp[0].arcarrow);
}
#line 6631 "language.cc" /* yacc.c:1646  */
    break;

  case 251:
#line 3261 "language.yy" /* yacc.c:1646  */
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
#line 6648 "language.cc" /* yacc.c:1646  */
    break;

  case 252:
#line 3274 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[-1].arcsegdata), LSIDE_ENT_STR, MSC_POS((yylsp[-1])), (yyvsp[0].str), true, MSC_POS((yylsp[0])));
  #endif
    free((yyvsp[0].str));
}
#line 6662 "language.cc" /* yacc.c:1646  */
    break;

  case 253:
#line 3284 "language.yy" /* yacc.c:1646  */
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
#line 6677 "language.cc" /* yacc.c:1646  */
    break;

  case 254:
#line 3295 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcarrow) = ((yyvsp[-2].arcarrow))->AddSegment((yyvsp[-1].arcsegdata), (yyvsp[0].str), MSC_POS((yylsp[0])), MSC_POS2((yylsp[-1]), (yylsp[0])));
  #endif
    free((yyvsp[0].str));
}
#line 6691 "language.cc" /* yacc.c:1646  */
    break;

  case 255:
#line 3305 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((yylsp[-1]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[-1].arcarrow))->AddSegment((yyvsp[0].arcsegdata), NULL, MSC_POS((yylsp[0])), MSC_POS((yylsp[0])));
  #endif
}
#line 6703 "language.cc" /* yacc.c:1646  */
    break;

  case 256:
#line 3315 "language.yy" /* yacc.c:1646  */
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
#line 6720 "language.cc" /* yacc.c:1646  */
    break;

  case 257:
#line 3328 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[-1].arcsegdata), (yyvsp[0].str), MSC_POS((yylsp[0])), LSIDE_ENT_STR, false, MSC_POS((yylsp[-1])));
  #endif
    free((yyvsp[0].str));
}
#line 6734 "language.cc" /* yacc.c:1646  */
    break;

  case 258:
#line 3338 "language.yy" /* yacc.c:1646  */
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
#line 6749 "language.cc" /* yacc.c:1646  */
    break;

  case 259:
#line 3349 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcarrow) = ((yyvsp[-2].arcarrow))->AddSegment((yyvsp[-1].arcsegdata), (yyvsp[0].str), MSC_POS((yylsp[0])), MSC_POS2((yylsp[-1]), (yylsp[0])));
  #endif
    free((yyvsp[0].str));
}
#line 6763 "language.cc" /* yacc.c:1646  */
    break;

  case 260:
#line 3359 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[-1].arcarrow))->AddSegment((yyvsp[0].arcsegdata), NULL, MSC_POS((yylsp[0])), MSC_POS((yylsp[0])));
  #endif
}
#line 6775 "language.cc" /* yacc.c:1646  */
    break;

  case 261:
#line 3368 "language.yy" /* yacc.c:1646  */
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
#line 6792 "language.cc" /* yacc.c:1646  */
    break;

  case 262:
#line 3381 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[-1].arcsegdata), LSIDE_ENT_STR, MSC_POS((yylsp[-1])), (yyvsp[0].str), true, MSC_POS((yylsp[0])));
  #endif
    free((yyvsp[0].str));
}
#line 6806 "language.cc" /* yacc.c:1646  */
    break;

  case 263:
#line 3391 "language.yy" /* yacc.c:1646  */
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
#line 6821 "language.cc" /* yacc.c:1646  */
    break;

  case 264:
#line 3402 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((yylsp[-1]), (yylsp[0]));
    csh.AddCSH_EntityName((yylsp[0]), (yyvsp[0].str));
  #else
    (yyval.arcarrow) = ((yyvsp[-2].arcarrow))->AddSegment((yyvsp[-1].arcsegdata), (yyvsp[0].str), MSC_POS((yylsp[0])), MSC_POS2((yylsp[-1]), (yylsp[0])));
  #endif
    free((yyvsp[0].str));
}
#line 6835 "language.cc" /* yacc.c:1646  */
    break;

  case 265:
#line 3412 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((yylsp[0]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[-1].arcarrow))->AddSegment((yyvsp[0].arcsegdata), NULL, MSC_POS((yylsp[0])), MSC_POS((yylsp[0])));
  #endif
}
#line 6847 "language.cc" /* yacc.c:1646  */
    break;

  case 279:
#line 3424 "language.yy" /* yacc.c:1646  */
    {(yyval.arctype)=MSC_ARC_UNDETERMINED_SEGMENT;}
#line 6853 "language.cc" /* yacc.c:1646  */
    break;

  case 281:
#line 3425 "language.yy" /* yacc.c:1646  */
    {(yyval.arctype)=MSC_ARC_UNDETERMINED_SEGMENT;}
#line 6859 "language.cc" /* yacc.c:1646  */
    break;

  case 283:
#line 3426 "language.yy" /* yacc.c:1646  */
    {(yyval.arctype)=MSC_ARC_UNDETERMINED_SEGMENT;}
#line 6865 "language.cc" /* yacc.c:1646  */
    break;

  case 284:
#line 3429 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 0;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
#line 6879 "language.cc" /* yacc.c:1646  */
    break;

  case 285:
#line 3439 "language.yy" /* yacc.c:1646  */
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
#line 6894 "language.cc" /* yacc.c:1646  */
    break;

  case 286:
#line 3450 "language.yy" /* yacc.c:1646  */
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
#line 6909 "language.cc" /* yacc.c:1646  */
    break;

  case 287:
#line 3461 "language.yy" /* yacc.c:1646  */
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
#line 6926 "language.cc" /* yacc.c:1646  */
    break;

  case 288:
#line 3475 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
#line 6940 "language.cc" /* yacc.c:1646  */
    break;

  case 289:
#line 3485 "language.yy" /* yacc.c:1646  */
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
#line 6955 "language.cc" /* yacc.c:1646  */
    break;

  case 290:
#line 3496 "language.yy" /* yacc.c:1646  */
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
#line 6970 "language.cc" /* yacc.c:1646  */
    break;

  case 291:
#line 3507 "language.yy" /* yacc.c:1646  */
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
#line 6987 "language.cc" /* yacc.c:1646  */
    break;

  case 292:
#line 3521 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 0;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
#line 7001 "language.cc" /* yacc.c:1646  */
    break;

  case 293:
#line 3531 "language.yy" /* yacc.c:1646  */
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
#line 7016 "language.cc" /* yacc.c:1646  */
    break;

  case 294:
#line 3542 "language.yy" /* yacc.c:1646  */
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
#line 7031 "language.cc" /* yacc.c:1646  */
    break;

  case 295:
#line 3553 "language.yy" /* yacc.c:1646  */
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
#line 7048 "language.cc" /* yacc.c:1646  */
    break;

  case 296:
#line 3567 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
#line 7061 "language.cc" /* yacc.c:1646  */
    break;

  case 297:
#line 3576 "language.yy" /* yacc.c:1646  */
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
#line 7079 "language.cc" /* yacc.c:1646  */
    break;

  case 298:
#line 3590 "language.yy" /* yacc.c:1646  */
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
#line 7097 "language.cc" /* yacc.c:1646  */
    break;

  case 299:
#line 3604 "language.yy" /* yacc.c:1646  */
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
#line 7117 "language.cc" /* yacc.c:1646  */
    break;

  case 300:
#line 3621 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
#line 7130 "language.cc" /* yacc.c:1646  */
    break;

  case 301:
#line 3630 "language.yy" /* yacc.c:1646  */
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
#line 7148 "language.cc" /* yacc.c:1646  */
    break;

  case 302:
#line 3644 "language.yy" /* yacc.c:1646  */
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
#line 7166 "language.cc" /* yacc.c:1646  */
    break;

  case 303:
#line 3658 "language.yy" /* yacc.c:1646  */
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
#line 7186 "language.cc" /* yacc.c:1646  */
    break;

  case 304:
#line 3675 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[0].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
#line 7199 "language.cc" /* yacc.c:1646  */
    break;

  case 305:
#line 3684 "language.yy" /* yacc.c:1646  */
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
#line 7217 "language.cc" /* yacc.c:1646  */
    break;

  case 306:
#line 3698 "language.yy" /* yacc.c:1646  */
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
#line 7235 "language.cc" /* yacc.c:1646  */
    break;

  case 307:
#line 3712 "language.yy" /* yacc.c:1646  */
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
#line 7255 "language.cc" /* yacc.c:1646  */
    break;

  case 309:
#line 3731 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ExtvxposDesignatorName((yylsp[0]), (yyvsp[0].str));
    if (csh.hintStatus != HINT_READY &&
        csh.CheckHintAt((yylsp[0]), HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "left", HINT_KEYWORD, true,
                               CshHintGraphicCallbackForTextIdent,
                               CshHintGraphicParam(MSC_IDENT_LEFT)));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "center", HINT_KEYWORD, true,
                               CshHintGraphicCallbackForTextIdent,
                               CshHintGraphicParam(MSC_IDENT_CENTER)));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "right", HINT_KEYWORD, true,
                               CshHintGraphicCallbackForTextIdent,
                               CshHintGraphicParam(MSC_IDENT_RIGHT)));
        csh.hintStatus = HINT_FILLING;
    }
  #else
    (yyval.extvertxpos) = NULL;
  #endif
    free((yyvsp[0].str));
}
#line 7281 "language.cc" /* yacc.c:1646  */
    break;

  case 310:
#line 3754 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ExtvxposDesignatorName((yylsp[-1]), (yyvsp[-1].str));

  #else
    (yyval.extvertxpos) = new ExtVertXPos((yyvsp[-1].str), MSC_POS((yylsp[-1])), (yyvsp[0].vertxpos));
  #endif
    free((yyvsp[-1].str));
}
#line 7295 "language.cc" /* yacc.c:1646  */
    break;

  case 311:
#line 3764 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ExtvxposDesignatorName((yylsp[0]), (yyvsp[0].str));
    if (csh.CheckHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF, HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "at", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.extvertxpos) = NULL;
  #endif
    free((yyvsp[0].str));
}
#line 7312 "language.cc" /* yacc.c:1646  */
    break;

  case 314:
#line 3780 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-3]), COLOR_KEYWORD);
    if (CaseInsensitiveEqual((yyvsp[-2].str), "arc") || CaseInsensitiveEqual((yyvsp[-2].str), "rectangle") ||
        CaseInsensitiveEqual((yyvsp[-2].str), "..."))
        csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    else if (CaseInsensitiveBeginsWith("arc", (yyvsp[-2].str))==0 &&
             CaseInsensitiveBeginsWith("rectangle", (yyvsp[-2].str))==0 &&
             CaseInsensitiveBeginsWith("...", (yyvsp[-2].str))==0)
        csh.AddCSH_Error((yylsp[-2]), "Bad symbol name. Use 'arc', '...' or 'rectangle'.");
    if (csh.CheckHintAtAndBefore((yylsp[-3]), (yylsp[-2]), HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
    if (csh.hintStatus != HINT_READY &&
        csh.CheckHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF, HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "left", HINT_KEYWORD, true,
                               CshHintGraphicCallbackForTextIdent,
                               CshHintGraphicParam(MSC_IDENT_LEFT)));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "center", HINT_KEYWORD, true,
                               CshHintGraphicCallbackForTextIdent,
                               CshHintGraphicParam(MSC_IDENT_CENTER)));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "right", HINT_KEYWORD, true,
                               CshHintGraphicCallbackForTextIdent,
                               CshHintGraphicParam(MSC_IDENT_RIGHT)));
        csh.hintStatus = HINT_FILLING;
    }
  #else
    (yyval.arcbase) = new CommandSymbol(&msc, (yyvsp[-2].str), (yyvsp[-1].namerel), (yyvsp[0].extvertxpos), NULL);
  #endif
    free((yyvsp[-3].str));
    free((yyvsp[-2].str));
}
#line 7352 "language.cc" /* yacc.c:1646  */
    break;

  case 315:
#line 3816 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-4]), COLOR_KEYWORD);
    if (CaseInsensitiveEqual((yyvsp[-3].str), "arc") || CaseInsensitiveEqual((yyvsp[-3].str), "rectangle") ||
        CaseInsensitiveEqual((yyvsp[-3].str), "..."))
        csh.AddCSH((yylsp[-3]), COLOR_KEYWORD);
    else if (CaseInsensitiveBeginsWith("arc", (yyvsp[-3].str))==0 &&
             CaseInsensitiveBeginsWith("rectangle", (yyvsp[-3].str))==0 &&
             CaseInsensitiveBeginsWith("...", (yyvsp[-3].str))==0)
        csh.AddCSH_Error((yylsp[-3]), "Bad symbol name. Use 'arc', '...' or 'rectangle'.");
    if (csh.CheckHintAtAndBefore((yylsp[-4]), (yylsp[-3]), HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.arcbase) = new CommandSymbol(&msc, (yyvsp[-3].str), (yyvsp[-2].namerel), (yyvsp[-1].extvertxpos), (yyvsp[0].extvertxpos));
  #endif
    free((yyvsp[-4].str));
    free((yyvsp[-3].str));
}
#line 7379 "language.cc" /* yacc.c:1646  */
    break;

  case 316:
#line 3839 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    if (CaseInsensitiveEqual((yyvsp[-1].str), "arc") || CaseInsensitiveEqual((yyvsp[-1].str), "rectangle") ||
        CaseInsensitiveEqual((yyvsp[-1].str), "..."))
        csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    else if (CaseInsensitiveBeginsWith("arc", (yyvsp[-1].str))==0 &&
             CaseInsensitiveBeginsWith("rectangle", (yyvsp[-1].str))==0 &&
             CaseInsensitiveBeginsWith("...", (yyvsp[-1].str))==0)
        csh.AddCSH_Error((yylsp[-1]), "Bad symbol name. Use 'arc', '...' or 'rectangle'.");
    if (csh.CheckHintAtAndBefore((yylsp[-2]), (yylsp[-1]), HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
    if (csh.hintStatus != HINT_READY &&
        csh.CheckHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF, HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "left", HINT_KEYWORD, true,
                               CshHintGraphicCallbackForTextIdent,
                               CshHintGraphicParam(MSC_IDENT_LEFT)));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "center", HINT_KEYWORD, true,
                               CshHintGraphicCallbackForTextIdent,
                               CshHintGraphicParam(MSC_IDENT_CENTER)));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "right", HINT_KEYWORD, true,
                               CshHintGraphicCallbackForTextIdent,
                               CshHintGraphicParam(MSC_IDENT_RIGHT)));
        csh.hintStatus = HINT_FILLING;
    }
  #else
    (yyval.arcbase) = new CommandSymbol(&msc, (yyvsp[-1].str), NULL, (yyvsp[0].extvertxpos), NULL);
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[-1].str));
}
#line 7419 "language.cc" /* yacc.c:1646  */
    break;

  case 317:
#line 3875 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-3]), COLOR_KEYWORD);
    if (CaseInsensitiveEqual((yyvsp[-2].str), "arc") || CaseInsensitiveEqual((yyvsp[-2].str), "rectangle") ||
        CaseInsensitiveEqual((yyvsp[-2].str), "..."))
        csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    else if (CaseInsensitiveBeginsWith("arc", (yyvsp[-2].str))==0 &&
             CaseInsensitiveBeginsWith("rectangle", (yyvsp[-2].str))==0 &&
             CaseInsensitiveBeginsWith("...", (yyvsp[-2].str))==0)
        csh.AddCSH_Error((yylsp[-2]), "Bad symbol name. Use 'arc', '...' or 'rectangle'.");
    if (csh.CheckHintAtAndBefore((yylsp[-3]), (yylsp[-2]), HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
    csh.CheckHintBetween((yylsp[-2]), (yylsp[-1]), HINT_MARKER);
  #else
    (yyval.arcbase) = new CommandSymbol(&msc, (yyvsp[-2].str), NULL, (yyvsp[-1].extvertxpos), (yyvsp[0].extvertxpos));
  #endif
    free((yyvsp[-3].str));
    free((yyvsp[-2].str));
}
#line 7447 "language.cc" /* yacc.c:1646  */
    break;

  case 318:
#line 3899 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    if (CaseInsensitiveEqual((yyvsp[-1].str), "arc") || CaseInsensitiveEqual((yyvsp[-1].str), "rectangle") ||
        CaseInsensitiveEqual((yyvsp[-1].str), "..."))
        csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    else if (CaseInsensitiveBeginsWith("arc", (yyvsp[-1].str))==0 &&
             CaseInsensitiveBeginsWith("rectangle", (yyvsp[-1].str))==0 &&
             CaseInsensitiveBeginsWith("...", (yyvsp[-1].str))==0)
        csh.AddCSH_Error((yylsp[-1]), "Bad symbol name. Use 'arc', '...' or 'rectangle'.");
    if (csh.CheckHintAtAndBefore((yylsp[-2]), (yylsp[-1]), HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
    if (csh.hintStatus != HINT_READY &&
        csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "left", HINT_KEYWORD, true,
                               CshHintGraphicCallbackForTextIdent,
                               CshHintGraphicParam(MSC_IDENT_LEFT)));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "center", HINT_KEYWORD, true,
                               CshHintGraphicCallbackForTextIdent,
                               CshHintGraphicParam(MSC_IDENT_CENTER)));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "right", HINT_KEYWORD, true,
                               CshHintGraphicCallbackForTextIdent,
                               CshHintGraphicParam(MSC_IDENT_RIGHT)));
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.arcbase) = new CommandSymbol(&msc, (yyvsp[-1].str), (yyvsp[0].namerel), NULL, NULL);
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[-1].str));
}
#line 7487 "language.cc" /* yacc.c:1646  */
    break;

  case 319:
#line 3935 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    if (CaseInsensitiveEqual((yyvsp[-1].str), "arc") || CaseInsensitiveEqual((yyvsp[-1].str), "rectangle") ||
        CaseInsensitiveEqual((yyvsp[-1].str), "..."))
        csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    else if (CaseInsensitiveBeginsWith("arc", (yyvsp[-1].str))==0 &&
             CaseInsensitiveBeginsWith("rectangle", (yyvsp[-1].str))==0 &&
             CaseInsensitiveBeginsWith("...", (yyvsp[-1].str))==0)
        csh.AddCSH_Error((yylsp[-1]), "Bad symbol name. Use 'arc', '...' or 'rectangle'.");
    if (csh.CheckHintAtAndBefore((yylsp[-2]), (yylsp[-1]), HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
    else if (csh.hintStatus != HINT_READY &&
        csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), HINT_MARKER)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "left", HINT_KEYWORD, true,
                               CshHintGraphicCallbackForTextIdent,
                               CshHintGraphicParam(MSC_IDENT_LEFT)));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "center", HINT_KEYWORD, true,
                               CshHintGraphicCallbackForTextIdent,
                               CshHintGraphicParam(MSC_IDENT_CENTER)));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "right", HINT_KEYWORD, true,
                               CshHintGraphicCallbackForTextIdent,
                               CshHintGraphicParam(MSC_IDENT_RIGHT)));
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.arcbase) = new CommandSymbol(&msc, (yyvsp[-1].str), NULL, NULL, NULL);
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[-1].str));
    free((yyvsp[0].str));
}
#line 7528 "language.cc" /* yacc.c:1646  */
    break;

  case 320:
#line 3972 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (CaseInsensitiveEqual((yyvsp[0].str), "arc") || CaseInsensitiveEqual((yyvsp[0].str), "rectangle") ||
        CaseInsensitiveEqual((yyvsp[0].str), "..."))
        csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    else if (CaseInsensitiveBeginsWith("arc", (yyvsp[0].str))==0 &&
             CaseInsensitiveBeginsWith("rectangle", (yyvsp[0].str))==0 &&
             CaseInsensitiveBeginsWith("...", (yyvsp[0].str))==0)
        csh.AddCSH_Error((yylsp[0]), "Bad symbol name. Use 'arc', '...' or 'rectangle'.");
    if (csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
    else if (csh.CheckHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF, HINT_MARKER)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "left", HINT_KEYWORD, true,
                               CshHintGraphicCallbackForTextIdent,
                               CshHintGraphicParam(MSC_IDENT_LEFT)));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "center", HINT_KEYWORD, true,
                               CshHintGraphicCallbackForTextIdent,
                               CshHintGraphicParam(MSC_IDENT_CENTER)));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "right", HINT_KEYWORD, true,
                               CshHintGraphicCallbackForTextIdent,
                               CshHintGraphicParam(MSC_IDENT_RIGHT)));
        csh.hintStatus = HINT_READY;

    }
  #else
    (yyval.arcbase) = new CommandSymbol(&msc, (yyvsp[0].str), NULL, NULL, NULL);
  #endif
    free((yyvsp[-1].str));
    free((yyvsp[0].str));
}
#line 7568 "language.cc" /* yacc.c:1646  */
    break;

  case 321:
#line 4008 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    if (csh.CheckHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF, HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Error.Error(MSC_POS((yylsp[0])).end, "Missing a symbol type.", "Use 'arc', '...' or 'rectangle'.");
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[0].str));
}
#line 7588 "language.cc" /* yacc.c:1646  */
    break;

  case 322:
#line 4025 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[0].arcbase))->AddAttributeList(NULL);
    (yyval.arcbase) = ((yyvsp[0].arcbase));
  #endif
}
#line 7599 "language.cc" /* yacc.c:1646  */
    break;

  case 323:
#line 4032 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[0])))
        CommandSymbol::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[0])))
        CommandSymbol::AttributeValues(csh.hintAttrName, csh);
  #else
    ((yyvsp[-1].arcbase))->AddAttributeList((yyvsp[0].attriblist));
    (yyval.arcbase) = ((yyvsp[-1].arcbase));
  #endif
}
#line 7615 "language.cc" /* yacc.c:1646  */
    break;

  case 324:
#line 4045 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-3]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.AddCSH_EntityOrMarkerName((yylsp[-1]), (yyvsp[-1].str));
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[0])))
        CommandNote::AttributeNames(csh, true);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[0])))
        CommandNote::AttributeValues(csh.hintAttrName, csh, true);
  #else
    (yyval.arcbase) = new CommandNote(&msc, (yyvsp[-1].str), MSC_POS((yylsp[-1])));
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
  #endif
    free((yyvsp[-3].str));
    free((yyvsp[-2].str));
    free((yyvsp[-1].str));
}
#line 7637 "language.cc" /* yacc.c:1646  */
    break;

  case 325:
#line 4063 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[0])))
        CommandNote::AttributeNames(csh, true);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[0])))
        CommandNote::AttributeValues(csh.hintAttrName, csh, true);
  #else
    (yyval.arcbase) = new CommandNote(&msc);
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
  #endif
    free((yyvsp[-1].str));
}
#line 7655 "language.cc" /* yacc.c:1646  */
    break;

  case 326:
#line 4077 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    csh.AddCSH_ErrorAfter((yylsp[0]), "Missing an entity or marker name.");
    csh.AddCSH_ErrorAfter((yylsp[0]), "Notes need a label.");
    if (csh.CheckEntityHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF))
        csh.addMarkersAtEnd = true;
  #else
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[-1].str));
    free((yyvsp[0].str));
}
#line 7674 "language.cc" /* yacc.c:1646  */
    break;

  case 327:
#line 4092 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-2]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    csh.AddCSH_ErrorAfter((yylsp[-1]), "Missing an entity or marker name.");
    if (csh.CheckEntityHintAfterPlusOne((yylsp[-1]), yylloc, yychar==YYEOF))
        csh.addMarkersAtEnd = true;
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[0])))
        CommandNote::AttributeNames(csh, true);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[0])))
        CommandNote::AttributeValues(csh.hintAttrName, csh, true);
  #else
    (yyval.arcbase) = new CommandNote(&msc);
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[-1].str));
}
#line 7697 "language.cc" /* yacc.c:1646  */
    break;

  case 328:
#line 4112 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.eside)= ESide::LEFT;
  #endif
    free((yyvsp[0].str));
}
#line 7708 "language.cc" /* yacc.c:1646  */
    break;

  case 329:
#line 4119 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.eside)= ESide::END;
  #endif
    free((yyvsp[0].str));
}
#line 7719 "language.cc" /* yacc.c:1646  */
    break;

  case 330:
#line 4126 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.eside)= ESide::END;
  #endif
    free((yyvsp[0].str));
}
#line 7730 "language.cc" /* yacc.c:1646  */
    break;

  case 331:
#line 4134 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[0])))
        CommandNote::AttributeNames(csh, false);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[0])))
        CommandNote::AttributeValues(csh.hintAttrName, csh, false);
  #else
    (yyval.arcbase) = new CommandNote(&msc, (yyvsp[-1].eside));
    ((yyval.arcbase))->AddAttributeList((yyvsp[0].attriblist));
  #endif
}
#line 7747 "language.cc" /* yacc.c:1646  */
    break;

  case 332:
#line 4147 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_KEYWORD);
    csh.AddCSH_ErrorAfter((yylsp[0]), "Comments and notes need a label.");
    if (csh.CheckEntityHintAfterPlusOne((yylsp[0]), yylloc, yychar==YYEOF))
        csh.addMarkersAtEnd = true;
  #else
    (yyval.arcbase) = NULL;
  #endif
}
#line 7762 "language.cc" /* yacc.c:1646  */
    break;

  case 333:
#line 4159 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ColonString((yylsp[0]), (yyvsp[0].str), false);
  #endif
    (yyval.str) = (yyvsp[0].str);
}
#line 7773 "language.cc" /* yacc.c:1646  */
    break;

  case 334:
#line 4166 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ColonString((yylsp[0]), (yyvsp[0].str), true);
	csh.AddColonLabel((yylsp[0]));
  #endif
    (yyval.str) = (yyvsp[0].str);
}
#line 7785 "language.cc" /* yacc.c:1646  */
    break;

  case 335:
#line 4175 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = (new AttributeList)->Append(new Attribute("label", (yyvsp[0].str), MSC_POS((yyloc)), MSC_POS((yyloc)).IncStartCol()));
  #endif
    free((yyvsp[0].str));
}
#line 7797 "language.cc" /* yacc.c:1646  */
    break;

  case 336:
#line 4183 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = ((yyvsp[0].attriblist))->Prepend(new Attribute("label", (yyvsp[-1].str), MSC_POS((yylsp[-1])), MSC_POS((yylsp[-1])).IncStartCol()));
  #endif
    free((yyvsp[-1].str));
}
#line 7809 "language.cc" /* yacc.c:1646  */
    break;

  case 337:
#line 4191 "language.yy" /* yacc.c:1646  */
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
#line 7825 "language.cc" /* yacc.c:1646  */
    break;

  case 338:
#line 4203 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = ((yyvsp[-1].attriblist))->Append(new Attribute("label", (yyvsp[0].str), MSC_POS((yylsp[0])), MSC_POS((yylsp[0])).IncStartCol()));
  #endif
    free((yyvsp[0].str));
}
#line 7837 "language.cc" /* yacc.c:1646  */
    break;

  case 340:
#line 4214 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[-1]), COLOR_BRACKET);
        csh.AddCSH((yylsp[0]), COLOR_BRACKET);
        csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), HINT_ATTR_NAME);
  #else
    (yyval.attriblist) = new AttributeList;
  #endif
}
#line 7851 "language.cc" /* yacc.c:1646  */
    break;

  case 341:
#line 4224 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[-2]), COLOR_BRACKET);
        csh.AddCSH((yylsp[0]), COLOR_BRACKET);
        csh.CheckHintBetween((yylsp[-2]), (yylsp[-1]), HINT_ATTR_NAME);
  #else
    (yyval.attriblist) = (yyvsp[-1].attriblist);
  #endif
}
#line 7865 "language.cc" /* yacc.c:1646  */
    break;

  case 342:
#line 4234 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[-3]), COLOR_BRACKET);
        csh.AddCSH_Error((yylsp[-1]), "Extra stuff after an attribute list. Maybe missing a comma?");
        csh.AddCSH((yylsp[0]), COLOR_BRACKET);
        csh.CheckHintBetween((yylsp[-3]), (yylsp[-2]), HINT_ATTR_NAME);
  #else
    (yyval.attriblist) = (yyvsp[-2].attriblist);
  #endif
}
#line 7880 "language.cc" /* yacc.c:1646  */
    break;

  case 343:
#line 4245 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[-2]), COLOR_BRACKET);
        csh.AddCSH_Error((yylsp[-1]), "Could not recognize this as an attribute.");
        csh.AddCSH((yylsp[0]), COLOR_BRACKET);
        csh.CheckHintBetween((yylsp[-2]), (yylsp[-1]), HINT_ATTR_NAME);
  #else
    (yyval.attriblist) = new AttributeList;
  #endif
}
#line 7895 "language.cc" /* yacc.c:1646  */
    break;

  case 344:
#line 4256 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[-1]), COLOR_BRACKET);
        csh.AddCSH_ErrorAfter((yylsp[0]), "Missing a square bracket (']').");
        csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), HINT_ATTR_NAME);
  #else
    (yyval.attriblist) = (yyvsp[0].attriblist);
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing ']'.");
  #endif
}
#line 7910 "language.cc" /* yacc.c:1646  */
    break;

  case 345:
#line 4267 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[-2]), COLOR_BRACKET);
        csh.AddCSH_Error((yylsp[0]), "Missing a ']'.");
        csh.CheckHintBetween((yylsp[-2]), (yylsp[-1]), HINT_ATTR_NAME);
  #else
    (yyval.attriblist) = (yyvsp[-1].attriblist);
    msc.Error.Error(MSC_POS((yylsp[0])).start, "Missing ']'.");
  #endif
}
#line 7925 "language.cc" /* yacc.c:1646  */
    break;

  case 346:
#line 4278 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[0]), COLOR_BRACKET);
        csh.AddCSH_ErrorAfter((yylsp[0]), "Missing a square bracket (']').");
        csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, HINT_ATTR_NAME);
  #else
    (yyval.attriblist) = new AttributeList;
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Missing ']'.");
  #endif
}
#line 7940 "language.cc" /* yacc.c:1646  */
    break;

  case 347:
#line 4291 "language.yy" /* yacc.c:1646  */
    {
    (yyval.attriblist) = NULL;
}
#line 7948 "language.cc" /* yacc.c:1646  */
    break;

  case 348:
#line 4295 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[-1]), COLOR_BRACKET);
        csh.AddCSH_Error((yylsp[0]), "Missing a ']'.");
        csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), HINT_ATTR_NAME);
  #else
    (yyval.attriblist) = new AttributeList;
    msc.Error.Error(MSC_POS((yylsp[0])).start, "Missing ']'.");
  #endif
}
#line 7963 "language.cc" /* yacc.c:1646  */
    break;

  case 349:
#line 4307 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.attriblist) = (new AttributeList)->Append((yyvsp[0].attrib));
  #endif
}
#line 7974 "language.cc" /* yacc.c:1646  */
    break;

  case 350:
#line 4314 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[-1]), COLOR_COMMA);
    csh.CheckHintBetween((yylsp[-1]), (yylsp[0]), HINT_ATTR_NAME);
  #else
    (yyval.attriblist) = ((yyvsp[-2].attriblist))->Append((yyvsp[0].attrib));
  #endif
}
#line 7987 "language.cc" /* yacc.c:1646  */
    break;

  case 351:
#line 4323 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_COMMA);
    csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, HINT_ATTR_NAME);
  #else
    (yyval.attriblist) = (yyvsp[-1].attriblist);
    msc.Error.Error(MSC_POS((yylsp[0])).end.NextChar(), "Expecting an entity here.");
  #endif
}
#line 8001 "language.cc" /* yacc.c:1646  */
    break;

  case 353:
#line 4335 "language.yy" /* yacc.c:1646  */
    {
  //string=string
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((yylsp[-2]), (yyvsp[-2].str), COLOR_ATTRNAME);
        csh.AddCSH((yylsp[-1]), COLOR_EQUAL);
        csh.AddCSH_AttrValue((yylsp[0]), (yyvsp[0].str), (yyvsp[-2].str));
        csh.CheckHintAt((yylsp[-2]), HINT_ATTR_NAME);
        csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), HINT_ATTR_VALUE, (yyvsp[-2].str));
  #else
        (yyval.attrib) = new Attribute((yyvsp[-2].str), (yyvsp[0].str), MSC_POS((yylsp[-2])), MSC_POS((yylsp[0])));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 8020 "language.cc" /* yacc.c:1646  */
    break;

  case 354:
#line 4350 "language.yy" /* yacc.c:1646  */
    {
  //string=string
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((yylsp[-3]), (yyvsp[-3].str), COLOR_ATTRNAME);
        csh.AddCSH((yylsp[-2]), COLOR_EQUAL);
        csh.AddCSH_AttrValue((yylsp[-1])+(yylsp[0]), (string("++")+(yyvsp[0].str)).c_str(), (yyvsp[-3].str));
        csh.CheckHintAt((yylsp[-3]), HINT_ATTR_NAME);
        csh.CheckHintAtAndBefore((yylsp[-2]), (yylsp[-1])+(yylsp[0]), HINT_ATTR_VALUE, (yyvsp[-3].str));
  #else
        (yyval.attrib) = new Attribute((yyvsp[-3].str), string("++")+(yyvsp[0].str), MSC_POS((yylsp[-3])), MSC_POS2((yylsp[-1]),(yylsp[0])));
  #endif
    free((yyvsp[-3].str));
    free((yyvsp[0].str));
}
#line 8039 "language.cc" /* yacc.c:1646  */
    break;

  case 355:
#line 4365 "language.yy" /* yacc.c:1646  */
    {
  //string=string
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((yylsp[-2]), (yyvsp[-2].str), COLOR_ATTRNAME);
        csh.AddCSH((yylsp[-1]), COLOR_EQUAL);
        csh.AddCSH_AttrValue((yylsp[0]), "++", (yyvsp[-2].str));
		csh.AddCSH_ErrorAfter((yylsp[0]), "Continue with a color name or definition.");
        csh.CheckHintAt((yylsp[-2]), HINT_ATTR_NAME);
        csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), HINT_ATTR_VALUE, (yyvsp[-2].str));
  #else
        (yyval.attrib) = new Attribute((yyvsp[-2].str), "++", MSC_POS((yylsp[-2])), MSC_POS((yylsp[0])));
  #endif
    free((yyvsp[-2].str));
}
#line 8058 "language.cc" /* yacc.c:1646  */
    break;

  case 356:
#line 4380 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((yylsp[-2]), (yyvsp[-2].str), COLOR_ATTRNAME);
        csh.AddCSH((yylsp[-1]), COLOR_EQUAL);
        csh.AddCSH((yylsp[0]), COLOR_ATTRVALUE);
        csh.CheckHintAt((yylsp[-2]), HINT_ATTR_NAME);
        csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), HINT_ATTR_VALUE, (yyvsp[-2].str));
  #else
        (yyval.attrib) = new Attribute((yyvsp[-2].str), atof((yyvsp[0].str)), MSC_POS((yyloc)), MSC_POS((yylsp[0])), (yyvsp[0].str));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 8076 "language.cc" /* yacc.c:1646  */
    break;

  case 357:
#line 4394 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((yylsp[-2]), (yyvsp[-2].str), COLOR_ATTRNAME);
        csh.AddCSH((yylsp[-1]), COLOR_EQUAL);
        csh.AddCSH((yylsp[0]), COLOR_ATTRVALUE);
        csh.CheckHintAt((yylsp[-2]), HINT_ATTR_NAME);
        csh.CheckHintAtAndBefore((yylsp[-1]), (yylsp[0]), HINT_ATTR_VALUE, (yyvsp[-2].str));
  #else
        (yyval.attrib) = new Attribute((yyvsp[-2].str), str2bool((yyvsp[0].str)), MSC_POS((yyloc)), MSC_POS((yylsp[0])), (yyvsp[0].str));
  #endif
    free((yyvsp[-2].str));
    free((yyvsp[0].str));
}
#line 8094 "language.cc" /* yacc.c:1646  */
    break;

  case 358:
#line 4408 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((yylsp[-1]), (yyvsp[-1].str), COLOR_ATTRNAME);
        csh.AddCSH((yylsp[0]), COLOR_EQUAL);
        csh.CheckHintAt((yylsp[-1]), HINT_ATTR_NAME);
        csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, HINT_ATTR_VALUE, (yyvsp[-1].str));
  #else
        (yyval.attrib) = new Attribute((yyvsp[-1].str), (char*)NULL, MSC_POS((yyloc)), MSC_POS((yyloc)));
  #endif
    free((yyvsp[-1].str));
}
#line 8110 "language.cc" /* yacc.c:1646  */
    break;

  case 359:
#line 4422 "language.yy" /* yacc.c:1646  */
    {
    (yyval.attrib) = NULL;
    free((yyvsp[-2].str));
}
#line 8119 "language.cc" /* yacc.c:1646  */
    break;

  case 360:
#line 4427 "language.yy" /* yacc.c:1646  */
    {
  //here we accept non alpha strings for "->" and similar style names
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_StyleOrAttrName((yylsp[0]), (yyvsp[0].str));
        csh.CheckHintAt((yylsp[0]), HINT_ATTR_NAME);
  #else
        (yyval.attrib) = new Attribute((yyvsp[0].str), MSC_POS((yyloc)));
  #endif
    free((yyvsp[0].str));
}
#line 8134 "language.cc" /* yacc.c:1646  */
    break;

  case 361:
#line 4439 "language.yy" /* yacc.c:1646  */
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
#line 8152 "language.cc" /* yacc.c:1646  */
    break;

  case 362:
#line 4453 "language.yy" /* yacc.c:1646  */
    {
  #ifndef C_S_H_IS_COMPILED
	(yyval.vshape) = ArcVerticalArrow::BOX;
  #endif
  free((yyvsp[0].str));
}
#line 8163 "language.cc" /* yacc.c:1646  */
    break;

  case 363:
#line 4463 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
	csh.AddQuotedString((yylsp[0]));
  #endif
  (yyval.str) = (yyvsp[0].str);
}
#line 8174 "language.cc" /* yacc.c:1646  */
    break;

  case 365:
#line 4471 "language.yy" /* yacc.c:1646  */
    {
	(yyval.str) = (char*)malloc(2);
	((yyval.str))[0] = ShapeElement::act_code[(yyvsp[0].shapecommand)];
	((yyval.str))[1] = 0;
}
#line 8184 "language.cc" /* yacc.c:1646  */
    break;

  case 397:
#line 4490 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("->");}
#line 8190 "language.cc" /* yacc.c:1646  */
    break;

  case 398:
#line 4491 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("<-");}
#line 8196 "language.cc" /* yacc.c:1646  */
    break;

  case 399:
#line 4492 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("<->");}
#line 8202 "language.cc" /* yacc.c:1646  */
    break;

  case 400:
#line 4493 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("=>");}
#line 8208 "language.cc" /* yacc.c:1646  */
    break;

  case 401:
#line 4494 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("<=");}
#line 8214 "language.cc" /* yacc.c:1646  */
    break;

  case 402:
#line 4495 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("<=>");}
#line 8220 "language.cc" /* yacc.c:1646  */
    break;

  case 403:
#line 4496 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup(">>");}
#line 8226 "language.cc" /* yacc.c:1646  */
    break;

  case 404:
#line 4497 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("<<");}
#line 8232 "language.cc" /* yacc.c:1646  */
    break;

  case 405:
#line 4498 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("<<>>");}
#line 8238 "language.cc" /* yacc.c:1646  */
    break;

  case 406:
#line 4499 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup(">");}
#line 8244 "language.cc" /* yacc.c:1646  */
    break;

  case 407:
#line 4500 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("<");}
#line 8250 "language.cc" /* yacc.c:1646  */
    break;

  case 408:
#line 4501 "language.yy" /* yacc.c:1646  */
    {(yyval.str) = strdup("<>");}
#line 8256 "language.cc" /* yacc.c:1646  */
    break;

  case 409:
#line 4503 "language.yy" /* yacc.c:1646  */
    {
    switch ((yyvsp[0].arctype)) {
    case MSC_ARC_DIVIDER:  (yyval.str) = strdup("---"); break;
    case MSC_ARC_DISCO:    (yyval.str) = strdup("..."); break;
    default: _ASSERT(0);
    }
}
#line 8268 "language.cc" /* yacc.c:1646  */
    break;

  case 410:
#line 4511 "language.yy" /* yacc.c:1646  */
    {
    switch ((yyvsp[0].arctype)) {
    case MSC_BOX_SOLID:  (yyval.str) = strdup("--"); break;
    case MSC_BOX_DASHED: (yyval.str) = strdup("++"); break; //will likely not happen
    case MSC_BOX_DOTTED: (yyval.str) = strdup(".."); break;
    case MSC_BOX_DOUBLE: (yyval.str) = strdup("=="); break;
    default: _ASSERT(0);
    }
}
#line 8282 "language.cc" /* yacc.c:1646  */
    break;

  case 416:
#line 4526 "language.yy" /* yacc.c:1646  */
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[0]), COLOR_BRACE);
    csh.PushContext();
    if (csh.CheckHintAfter((yylsp[0]), yylloc, yychar==YYEOF, HINT_LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.PushContext();
  #endif
}
#line 8299 "language.cc" /* yacc.c:1646  */
    break;

  case 418:
#line 4543 "language.yy" /* yacc.c:1646  */
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
#line 8316 "language.cc" /* yacc.c:1646  */
    break;


#line 8320 "language.cc" /* yacc.c:1646  */
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
#line 4557 "language.yy" /* yacc.c:1906  */



/* END OF FILE */
