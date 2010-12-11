
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 1

/* Substitute the variable and function names.  */
#define yyparse         csh_parse
#define yylex           csh_lex
#define yyerror         csh_error
#define yylval          csh_lval
#define yychar          csh_char
#define yydebug         csh_debug
#define yynerrs         csh_nerrs
#define yylloc          csh_lloc

/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 7 "language.yy"

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

//if yyerror is defined, we compile with a prefix other than yy (csh_error)
//thus we compile the colorsyntax highlighting version
#ifdef yyerror
#define C_S_H (1)
#define C_S_H_IS_COMPILED
#else
#define C_S_H (0)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <iostream>

#ifdef C_S_H_IS_COMPILED

#define YYMSC_RESULT_TYPE Csh
#define RESULT csh

#include "csh.h"

//If we scan for color syntax highlight use this location
//yyerror is defined by bison, the other is defined for flex
#define YYLTYPE_IS_DECLARED
#define YYLTYPE CshPos
#define CHAR_IF_CSH(A) char

#include "colorsyntax.h"
#include "language_misc.h"
#include "colorsyntax2.h"
#include "arcs.h"

//redefine default loc action for CSH
#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do								\
      if (YYID (N))                                             \
	{							\
        (Current).first_pos = YYRHSLOC (Rhs, 1).first_pos;	\
	  (Current).last_pos  = YYRHSLOC (Rhs, N).last_pos;	\
	}						        \
      else							\
	{							\
	  (Current).first_pos = (Current).last_pos   =		\
	    YYRHSLOC (Rhs, 0).last_pos;				\
	}							\
    while (YYID (0))

/* yyerror
 *  Error handling function.  Do nothing for CSH
 */
void yyerror(YYLTYPE*loc, Csh &csh, void *yyscanner, const char *str)
{}

#else

#define YYMSC_RESULT_TYPE Msc
#define RESULT msc

#include "msc.h"
#define YYMSC_RESULT_TYPE Msc
#define RESULT msc

#define CHAR_IF_CSH(A) A
#include "language.h"
#include "language_misc.h"
#include "language2.h"

/* Use verbose error reporting such that the expected token names are dumped */
#define YYERROR_VERBOSE

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



/* Line 189 of yacc.c  */
#line 196 "colorsyntax.cc"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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
     TOK_OCBRACKET = 266,
     TOK_CCBRACKET = 267,
     TOK_OSBRACKET = 268,
     TOK_CSBRACKET = 269,
     TOK_MSC = 270,
     TOK_COLON_STRING = 271,
     TOK_COLON_QUOTED_STRING = 272,
     TOK_STYLE_NAME = 273,
     TOK_BOOLEAN = 274,
     TOK_REL_SOLID_TO = 275,
     TOK_REL_SOLID_FROM = 276,
     TOK_REL_SOLID_BIDIR = 277,
     TOK_REL_DOUBLE_TO = 278,
     TOK_REL_DOUBLE_FROM = 279,
     TOK_REL_DOUBLE_BIDIR = 280,
     TOK_REL_DASHED_TO = 281,
     TOK_REL_DASHED_FROM = 282,
     TOK_REL_DASHED_BIDIR = 283,
     TOK_REL_DOTTED_TO = 284,
     TOK_REL_DOTTED_FROM = 285,
     TOK_REL_DOTTED_BIDIR = 286,
     TOK_SPECIAL_ARC = 287,
     TOK_EMPH = 288,
     TOK_COMMAND_HEADING = 289,
     TOK_COMMAND_NUDGE = 290,
     TOK_COMMAND_NEWPAGE = 291,
     TOK_COMMAND_DEFCOLOR = 292,
     TOK_COMMAND_DEFSTYLE = 293,
     TOK_COMMAND_DEFDESIGN = 294,
     TOK_COMMAND_BIG = 295,
     TOK_COMMAND_PIPE = 296,
     TOK_COMMAND_MARK = 297,
     TOK_COMMAND_PARALLEL = 298,
     TOK_VERTICAL = 299,
     TOK_AT = 300
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
#define TOK_OCBRACKET 266
#define TOK_CCBRACKET 267
#define TOK_OSBRACKET 268
#define TOK_CSBRACKET 269
#define TOK_MSC 270
#define TOK_COLON_STRING 271
#define TOK_COLON_QUOTED_STRING 272
#define TOK_STYLE_NAME 273
#define TOK_BOOLEAN 274
#define TOK_REL_SOLID_TO 275
#define TOK_REL_SOLID_FROM 276
#define TOK_REL_SOLID_BIDIR 277
#define TOK_REL_DOUBLE_TO 278
#define TOK_REL_DOUBLE_FROM 279
#define TOK_REL_DOUBLE_BIDIR 280
#define TOK_REL_DASHED_TO 281
#define TOK_REL_DASHED_FROM 282
#define TOK_REL_DASHED_BIDIR 283
#define TOK_REL_DOTTED_TO 284
#define TOK_REL_DOTTED_FROM 285
#define TOK_REL_DOTTED_BIDIR 286
#define TOK_SPECIAL_ARC 287
#define TOK_EMPH 288
#define TOK_COMMAND_HEADING 289
#define TOK_COMMAND_NUDGE 290
#define TOK_COMMAND_NEWPAGE 291
#define TOK_COMMAND_DEFCOLOR 292
#define TOK_COMMAND_DEFSTYLE 293
#define TOK_COMMAND_DEFDESIGN 294
#define TOK_COMMAND_BIG 295
#define TOK_COMMAND_PIPE 296
#define TOK_COMMAND_MARK 297
#define TOK_COMMAND_PARALLEL 298
#define TOK_VERTICAL 299
#define TOK_AT 300




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 136 "language.yy"

    char                          *str;
    CHAR_IF_CSH(Msc)              *msc;
    CHAR_IF_CSH(ArcBase)          *arcbase;
    CHAR_IF_CSH(ArcList)          *arclist;
    CHAR_IF_CSH(ArcArrow)         *arcarrow;
    CHAR_IF_CSH(ArcEmphasis)      *arcemph;
    CHAR_IF_CSH(ArcParallel)      *arcparallel;
    CHAR_IF_CSH(MscArcType)        arctype;
    CHAR_IF_CSH(EntityDef)        *entity;
    CHAR_IF_CSH(EntityDefList)    *entitylist;
    CHAR_IF_CSH(Attribute)        *attrib;
    CHAR_IF_CSH(AttributeList)    *attriblist;
    CHAR_IF_CSH(VertXPos)         *vertxpos;
    std::list<std::string>        *stringlist;



/* Line 214 of yacc.c  */
#line 341 "colorsyntax.cc"
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


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 366 "colorsyntax.cc"

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
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
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
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  131
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   901

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  57
/* YYNRULES -- Number of rules.  */
#define YYNRULES  219
/* YYNRULES -- Number of states.  */
#define YYNSTATES  281

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   300

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
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
      45
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     6,     8,    11,    14,    18,    22,
      25,    30,    34,    36,    39,    41,    44,    49,    53,    59,
      64,    66,    69,    73,    75,    78,    81,    85,    87,    90,
      92,    95,    98,   102,   105,   109,   111,   113,   117,   119,
     121,   123,   126,   129,   132,   134,   137,   139,   142,   145,
     149,   151,   154,   156,   160,   164,   168,   172,   176,   178,
     182,   185,   187,   190,   192,   194,   198,   201,   203,   207,
     209,   213,   217,   223,   230,   232,   234,   238,   241,   244,
     246,   248,   252,   255,   259,   263,   267,   271,   273,   276,
     278,   280,   283,   287,   291,   294,   299,   303,   305,   308,
     311,   315,   317,   320,   323,   326,   330,   332,   335,   339,
     342,   345,   347,   350,   354,   358,   362,   367,   369,   371,
     373,   378,   382,   386,   389,   394,   398,   402,   405,   407,
     409,   411,   413,   417,   420,   423,   427,   430,   434,   437,
     440,   444,   447,   451,   454,   457,   461,   464,   466,   468,
     470,   472,   474,   476,   478,   480,   482,   484,   486,   488,
     490,   492,   494,   496,   498,   500,   502,   504,   506,   509,
     512,   516,   518,   521,   525,   530,   534,   538,   541,   543,
     547,   551,   555,   559,   562,   564,   566,   568,   570,   572,
     574,   576,   578,   580,   582,   584,   586,   588,   590,   592,
     594,   596,   598,   600,   602,   604,   606,   608,   610,   612,
     614,   616,   618,   620,   622,   624,   626,   628,   630,   632
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      47,     0,    -1,    -1,    48,    -1,    50,    -1,    50,     1,
      -1,    50,    12,    -1,    50,    12,    48,    -1,   101,    50,
     102,    -1,   101,   102,    -1,   101,    50,     1,   102,    -1,
     101,    50,     1,    -1,    53,    -1,    50,    53,    -1,    51,
      -1,    50,    51,    -1,    52,    11,    50,    12,    -1,    52,
      11,    12,    -1,    52,    11,    50,     1,    12,    -1,    52,
      11,    50,     1,    -1,    15,    -1,    15,     7,    -1,    15,
       7,   100,    -1,     9,    -1,     1,     9,    -1,    54,     9,
      -1,    54,     1,     9,    -1,    55,    -1,    43,    55,    -1,
      82,    -1,    82,    93,    -1,    40,    82,    -1,    40,    82,
      93,    -1,    44,    81,    -1,    44,    81,    93,    -1,    93,
      -1,    60,    -1,    60,     8,    58,    -1,    56,    -1,    73,
      -1,    72,    -1,    37,    64,    -1,    38,    61,    -1,    39,
      66,    -1,    34,    -1,    34,    94,    -1,    35,    -1,    35,
      94,    -1,    42,   100,    -1,    42,   100,    94,    -1,    36,
      -1,    36,    94,    -1,    57,    -1,    56,     8,    57,    -1,
      97,     7,    19,    -1,    97,     7,     5,    -1,    97,     7,
     100,    -1,    15,     7,   100,    -1,    59,    -1,    58,     8,
      59,    -1,    97,    93,    -1,    97,    -1,    97,    93,    -1,
      97,    -1,    62,    -1,    61,     8,    62,    -1,    63,    94,
      -1,   100,    -1,    63,     8,   100,    -1,    65,    -1,    64,
       8,    65,    -1,     3,     7,   100,    -1,     3,    67,    68,
       9,    12,    -1,     3,    67,    68,     9,     1,    12,    -1,
      11,    -1,    69,    -1,    68,     9,    69,    -1,    37,    64,
      -1,    38,    61,    -1,    70,    -1,    71,    -1,    70,     8,
      71,    -1,    70,     1,    -1,    97,     7,    19,    -1,    97,
       7,     5,    -1,    97,     7,   100,    -1,    15,     7,   100,
      -1,    49,    -1,    72,    49,    -1,    74,    -1,    77,    -1,
      73,    78,    -1,    73,    78,    93,    -1,    73,    78,    49,
      -1,    73,    49,    -1,    73,    78,    93,    49,    -1,    73,
      93,    49,    -1,    78,    -1,    78,    93,    -1,    78,    49,
      -1,    78,    93,    49,    -1,    78,    -1,    78,    93,    -1,
      41,    75,    -1,    76,    75,    -1,    76,    41,    75,    -1,
      76,    -1,    76,    49,    -1,    97,    33,    97,    -1,    33,
      97,    -1,    97,    33,    -1,    33,    -1,    45,    97,    -1,
      45,    97,     6,    -1,    45,    97,    10,    -1,    45,    97,
      33,    -1,    45,    97,     6,    97,    -1,    33,    -1,    86,
      -1,    88,    -1,    97,    80,    97,    79,    -1,    80,    97,
      79,    -1,    97,    80,    79,    -1,    80,    79,    -1,    97,
      87,    97,    79,    -1,    87,    97,    79,    -1,    97,    87,
      79,    -1,    87,    79,    -1,    32,    -1,    83,    -1,    84,
      -1,    85,    -1,    97,    86,    97,    -1,    86,    97,    -1,
      97,    86,    -1,    83,    89,    97,    -1,    83,    89,    -1,
      97,    87,    97,    -1,    87,    97,    -1,    97,    87,    -1,
      84,    90,    97,    -1,    84,    90,    -1,    97,    88,    97,
      -1,    88,    97,    -1,    97,    88,    -1,    85,    91,    97,
      -1,    85,    91,    -1,    20,    -1,    23,    -1,    26,    -1,
      29,    -1,    21,    -1,    24,    -1,    27,    -1,    30,    -1,
      22,    -1,    25,    -1,    28,    -1,    31,    -1,    86,    -1,
       6,    -1,    87,    -1,     6,    -1,    88,    -1,     6,    -1,
      17,    -1,    16,    -1,    92,    -1,    92,    94,    -1,    94,
      92,    -1,    94,    92,    94,    -1,    94,    -1,    13,    14,
      -1,    13,    95,    14,    -1,    13,    95,     1,    14,    -1,
      13,     1,    14,    -1,    13,    95,     1,    -1,    13,     1,
      -1,    96,    -1,    95,     8,    96,    -1,   100,     7,   100,
      -1,   100,     7,     5,    -1,   100,     7,    19,    -1,   100,
       7,    -1,   100,    -1,     4,    -1,     3,    -1,    15,    -1,
      37,    -1,    38,    -1,    39,    -1,    36,    -1,    40,    -1,
      41,    -1,    44,    -1,    43,    -1,    34,    -1,    35,    -1,
      42,    -1,    45,    -1,    20,    -1,    21,    -1,    22,    -1,
      23,    -1,    24,    -1,    25,    -1,    26,    -1,    27,    -1,
      28,    -1,    29,    -1,    30,    -1,    31,    -1,    32,    -1,
      33,    -1,    97,    -1,    98,    -1,    99,    -1,    18,    -1,
      11,    -1,    12,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   198,   198,   201,   209,   210,   218,   227,   241,   251,
     264,   276,   288,   297,   305,   311,   324,   341,   358,   377,
     395,   403,   415,   434,   442,   451,   460,   472,   473,   484,
     485,   501,   512,   534,   543,   561,   577,   583,   591,   602,
     608,   614,   623,   632,   641,   650,   659,   668,   677,   688,
     699,   708,   718,   727,   742,   755,   768,   788,   805,   811,
     820,   830,   841,   851,   864,   865,   872,   891,   907,   925,
     926,   933,   947,   967,   990,  1001,  1002,  1009,  1016,  1023,
    1025,  1026,  1032,  1039,  1051,  1063,  1075,  1089,  1098,  1110,
    1116,  1121,  1128,  1136,  1143,  1151,  1159,  1170,  1177,  1185,
    1192,  1201,  1207,  1215,  1225,  1232,  1244,  1245,  1252,  1264,
    1274,  1284,  1293,  1304,  1316,  1328,  1359,  1374,  1374,  1374,
    1376,  1389,  1400,  1411,  1420,  1433,  1444,  1455,  1466,  1474,
    1478,  1482,  1487,  1499,  1509,  1519,  1529,  1539,  1551,  1561,
    1571,  1581,  1590,  1602,  1612,  1622,  1632,  1641,  1641,  1641,
    1641,  1642,  1642,  1642,  1642,  1643,  1643,  1643,  1643,  1645,
    1645,  1646,  1646,  1647,  1647,  1650,  1657,  1665,  1673,  1681,
    1689,  1701,  1704,  1715,  1734,  1754,  1766,  1785,  1797,  1804,
    1823,  1844,  1865,  1886,  1903,  1918,  1918,  1920,  1920,  1921,
    1921,  1922,  1922,  1922,  1923,  1923,  1924,  1924,  1925,  1925,
    1927,  1928,  1929,  1930,  1931,  1932,  1933,  1934,  1935,  1936,
    1937,  1938,  1939,  1946,  1956,  1956,  1956,  1956,  1958,  1967
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOK_STRING", "TOK_QSTRING",
  "TOK_NUMBER", "TOK_DASH", "TOK_EQUAL", "TOK_COMMA", "TOK_SEMICOLON",
  "TOK_PLUS", "TOK_OCBRACKET", "TOK_CCBRACKET", "TOK_OSBRACKET",
  "TOK_CSBRACKET", "TOK_MSC", "TOK_COLON_STRING",
  "TOK_COLON_QUOTED_STRING", "TOK_STYLE_NAME", "TOK_BOOLEAN",
  "TOK_REL_SOLID_TO", "TOK_REL_SOLID_FROM", "TOK_REL_SOLID_BIDIR",
  "TOK_REL_DOUBLE_TO", "TOK_REL_DOUBLE_FROM", "TOK_REL_DOUBLE_BIDIR",
  "TOK_REL_DASHED_TO", "TOK_REL_DASHED_FROM", "TOK_REL_DASHED_BIDIR",
  "TOK_REL_DOTTED_TO", "TOK_REL_DOTTED_FROM", "TOK_REL_DOTTED_BIDIR",
  "TOK_SPECIAL_ARC", "TOK_EMPH", "TOK_COMMAND_HEADING",
  "TOK_COMMAND_NUDGE", "TOK_COMMAND_NEWPAGE", "TOK_COMMAND_DEFCOLOR",
  "TOK_COMMAND_DEFSTYLE", "TOK_COMMAND_DEFDESIGN", "TOK_COMMAND_BIG",
  "TOK_COMMAND_PIPE", "TOK_COMMAND_MARK", "TOK_COMMAND_PARALLEL",
  "TOK_VERTICAL", "TOK_AT", "$accept", "msc", "top_level_arclist",
  "braced_arclist", "arclist", "mscenclosed", "msckey", "complete_arc",
  "arc_with_parallel", "arc", "optlist", "opt", "entitylist", "entity",
  "first_entity", "styledeflist", "styledef", "tok_stringlist",
  "colordeflist", "colordef", "designdef", "scope_open_empty",
  "designelementlist", "designelement", "designoptlist", "designopt",
  "parallel", "emphasis_list", "first_emphasis", "pipe_def",
  "pipe_def_list", "pipe_emphasis", "emphrel", "vertxpos",
  "empharcrel_straight", "vertrel", "arcrel", "arcrel_to", "arcrel_from",
  "arcrel_bidir", "relation_to", "relation_from", "relation_bidir",
  "relation_to_cont", "relation_from_cont", "relation_bidir_cont",
  "colon_string", "full_arcattrlist_with_label", "full_arcattrlist",
  "arcattrlist", "arcattr", "entity_string", "reserved_word_string",
  "symbol_string", "string", "scope_open", "scope_close", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    46,    47,    47,    48,    48,    48,    48,    49,    49,
      49,    49,    50,    50,    50,    50,    51,    51,    51,    51,
      52,    52,    52,    53,    53,    53,    53,    54,    54,    55,
      55,    55,    55,    55,    55,    55,    55,    55,    55,    55,
      55,    55,    55,    55,    55,    55,    55,    55,    55,    55,
      55,    55,    56,    56,    57,    57,    57,    57,    58,    58,
      59,    59,    60,    60,    61,    61,    62,    63,    63,    64,
      64,    65,    66,    66,    67,    68,    68,    69,    69,    69,
      70,    70,    70,    71,    71,    71,    71,    72,    72,    73,
      73,    73,    73,    73,    73,    73,    73,    74,    74,    74,
      74,    75,    75,    76,    76,    76,    77,    77,    78,    78,
      78,    78,    79,    79,    79,    79,    79,    80,    80,    80,
      81,    81,    81,    81,    81,    81,    81,    81,    82,    82,
      82,    82,    83,    83,    83,    83,    83,    84,    84,    84,
      84,    84,    85,    85,    85,    85,    85,    86,    86,    86,
      86,    87,    87,    87,    87,    88,    88,    88,    88,    89,
      89,    90,    90,    91,    91,    92,    92,    93,    93,    93,
      93,    93,    94,    94,    94,    94,    94,    94,    95,    95,
      96,    96,    96,    96,    96,    97,    97,    98,    98,    98,
      98,    98,    98,    98,    98,    98,    98,    98,    98,    98,
      99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,   100,   100,   100,   100,   101,   102
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     1,     2,     2,     3,     3,     2,
       4,     3,     1,     2,     1,     2,     4,     3,     5,     4,
       1,     2,     3,     1,     2,     2,     3,     1,     2,     1,
       2,     2,     3,     2,     3,     1,     1,     3,     1,     1,
       1,     2,     2,     2,     1,     2,     1,     2,     2,     3,
       1,     2,     1,     3,     3,     3,     3,     3,     1,     3,
       2,     1,     2,     1,     1,     3,     2,     1,     3,     1,
       3,     3,     5,     6,     1,     1,     3,     2,     2,     1,
       1,     3,     2,     3,     3,     3,     3,     1,     2,     1,
       1,     2,     3,     3,     2,     4,     3,     1,     2,     2,
       3,     1,     2,     2,     2,     3,     1,     2,     3,     2,
       2,     1,     2,     3,     3,     3,     4,     1,     1,     1,
       4,     3,     3,     2,     4,     3,     3,     2,     1,     1,
       1,     1,     3,     2,     2,     3,     2,     3,     2,     2,
       3,     2,     3,     2,     2,     3,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       3,     1,     2,     3,     4,     3,     3,     2,     1,     3,
       3,     3,     3,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,   186,   185,    23,   218,     0,    20,   166,   165,
     147,   151,   155,   148,   152,   156,   149,   153,   157,   150,
     154,   158,   128,   111,    44,    46,    50,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     3,    87,     0,    14,
       0,    12,     0,    27,    38,    52,    36,    40,    39,    89,
     106,    90,    97,    29,   129,   130,   131,     0,     0,     0,
     167,    35,   171,    63,     0,    24,   177,   172,   187,   217,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   196,   197,   191,   188,   189,   190,
     192,   193,   198,   195,   194,   199,     0,   178,   214,   215,
     216,   184,    21,   109,    45,    47,    51,     0,    41,    69,
      42,    64,     0,    67,     0,    43,    31,     0,   103,   101,
       0,    48,     0,    28,   117,     0,    33,   118,     0,   119,
       0,     1,     5,     0,    15,    13,     0,     0,    25,     0,
       0,    88,    94,    91,     0,     0,   107,   104,    99,    98,
      30,   160,   159,   136,   162,   161,   141,   164,   163,   146,
     133,   138,   143,   168,   169,     0,   110,   134,   139,   144,
      62,   219,     0,     9,   175,   176,     0,   173,   183,    57,
       0,     0,     0,     0,    66,    74,     0,    32,   102,    49,
       0,     0,   123,     0,    34,   127,     0,     0,     0,     7,
      17,     0,    26,    53,     0,    37,    58,    61,    93,    92,
      96,   105,   100,   135,   140,   145,   170,    55,    54,    56,
     108,   132,   137,   142,    11,     8,   174,   179,   181,   182,
     180,    71,    70,    65,    68,     0,     0,     0,     0,    75,
       0,    80,     0,    57,   112,   121,   125,   122,     0,   126,
       0,    19,    16,     0,    60,    95,    10,     0,    77,    78,
       0,    82,     0,     0,   113,   114,   115,   120,   124,    18,
      59,    86,     0,    72,    76,    81,    84,    83,    85,   116,
      73
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,   205,   206,    46,   110,   111,   112,   108,   109,
     115,   186,   238,   239,   240,   241,    47,    48,    49,   118,
      50,    51,    52,   192,   125,   126,    53,    54,    55,    56,
      57,    58,    59,   153,   156,   159,    60,    61,    62,    96,
      97,    98,    99,   100,   113,    64,   173
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -163
static const yytype_int16 yypact[] =
{
     265,     3,  -163,  -163,  -163,  -163,   354,    30,  -163,  -163,
    -163,  -163,  -163,  -163,  -163,  -163,  -163,  -163,  -163,  -163,
    -163,  -163,  -163,   107,     9,     9,     9,    58,   702,    60,
     820,    75,   702,   745,   787,    49,  -163,  -163,   199,  -163,
      65,  -163,    20,  -163,    61,  -163,    94,    95,   789,  -163,
      41,  -163,   141,   114,    67,   859,   833,   107,   107,   107,
       9,  -163,   121,   846,   399,  -163,   100,  -163,  -163,  -163,
    -163,  -163,  -163,  -163,  -163,  -163,  -163,  -163,  -163,  -163,
    -163,  -163,  -163,  -163,  -163,  -163,  -163,  -163,  -163,  -163,
    -163,  -163,  -163,  -163,  -163,  -163,   112,  -163,  -163,  -163,
    -163,   109,   702,  -163,  -163,  -163,  -163,   136,   113,  -163,
     140,  -163,    72,  -163,   133,  -163,   114,   870,  -163,   114,
     118,     9,   155,  -163,  -163,    15,   114,  -163,    15,  -163,
     805,  -163,     3,   310,  -163,  -163,   443,   154,  -163,    38,
     107,  -163,  -163,   141,    95,    75,  -163,  -163,  -163,    95,
    -163,  -163,  -163,   107,  -163,  -163,   107,  -163,  -163,   107,
    -163,  -163,  -163,  -163,     9,   573,   107,   107,   107,   107,
    -163,  -163,   487,  -163,  -163,   150,   702,  -163,   616,   163,
     702,    58,   702,   702,  -163,  -163,   146,  -163,  -163,  -163,
     702,   107,  -163,   132,  -163,  -163,   132,    15,    15,  -163,
    -163,   531,  -163,  -163,   173,   174,  -163,   114,  -163,    95,
    -163,  -163,  -163,  -163,  -163,  -163,  -163,  -163,  -163,  -163,
    -163,  -163,  -163,  -163,    77,  -163,  -163,  -163,  -163,  -163,
    -163,  -163,  -163,  -163,  -163,   180,    58,   702,   172,  -163,
      46,  -163,   181,  -163,    33,  -163,  -163,  -163,   132,  -163,
     132,    92,  -163,   107,  -163,  -163,  -163,   702,   113,   140,
      80,  -163,   131,   659,   107,  -163,  -163,  -163,  -163,  -163,
    -163,  -163,   177,  -163,  -163,  -163,  -163,  -163,  -163,  -163,
    -163
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -163,  -163,    57,   -44,   -59,   -31,  -163,   -25,  -163,   159,
    -163,    54,  -163,   -58,  -163,   -43,    14,  -163,   -32,    24,
    -163,  -163,  -163,   -54,  -163,   -55,  -163,  -163,  -163,   -36,
    -163,  -163,   -22,  -126,    79,  -163,   183,  -163,  -163,  -163,
       2,   -23,    12,  -163,  -163,  -163,   156,   -28,    -9,  -163,
      68,     0,  -163,  -163,    -5,  -163,  -162
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -80
static const yytype_int16 yytable[] =
{
      63,   101,   195,   141,   142,   172,   146,   134,   148,   119,
     225,   128,    65,   135,   147,   104,   105,   106,     2,     3,
     144,   137,     6,   103,   149,   150,   143,   121,   119,   138,
     117,   120,   155,    63,   130,   170,   127,   102,    63,   264,
     168,     2,     3,   265,     2,     3,   129,   261,   120,   131,
     120,   163,     5,   122,   262,   -79,   152,   160,   161,   162,
     191,   107,   256,   114,    63,   167,   266,   245,   158,   139,
     246,   247,   249,   151,    23,   169,   136,   201,     2,     3,
     183,   272,   145,     2,     3,     6,    65,    10,   187,   171,
      13,   188,   273,    16,   168,   235,    19,   179,   194,   208,
     210,    65,   140,   184,   269,   212,     5,   198,    23,   211,
       2,     3,   189,   175,   174,   209,   178,   236,   237,   167,
     176,   181,   267,   119,   268,   193,   177,     6,   196,   169,
       8,     9,   127,    63,     2,     3,    63,     8,     9,   204,
     207,   134,   129,   180,   185,   120,   235,   135,   182,     2,
       3,   166,     5,   213,     6,   216,   214,     8,     9,   215,
     219,   235,   190,   202,   226,   255,   220,   221,   222,   223,
     134,   101,    63,   230,   -22,   231,   135,   191,   234,   254,
     165,   260,   253,   236,   237,   243,   242,   257,   263,   280,
     199,   244,   123,   203,   259,   270,   233,   248,   250,    -4,
     132,    63,     2,     3,   258,   232,   274,   275,     4,   197,
       5,   133,     6,   116,     7,     8,     9,     0,   164,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,   227,     0,     0,     0,     0,     0,
       0,     0,   271,   207,     0,     0,     0,     0,   278,     0,
     242,     0,   242,     0,   279,    -2,     1,     0,     2,     3,
       0,     0,     0,     0,     4,     0,     5,     0,     6,     0,
       7,     8,     9,     0,     0,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      -6,     1,     0,     2,     3,     0,     0,     0,     0,     4,
       0,     5,     0,     6,     0,     7,     8,     9,     0,     0,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    66,     0,     2,     3,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,    68,
       0,     0,    69,     0,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
       1,     0,     2,     3,     0,     0,     0,     0,     4,     0,
       5,   171,     6,     0,     7,     8,     9,     0,     0,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,     1,     0,     2,     3,     0,     0,
       0,     0,     4,     0,     5,   200,     6,     0,     7,     8,
       9,     0,     0,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,   224,     0,
       2,     3,     0,     0,     0,     0,     4,     0,     5,   171,
       6,     0,     7,     8,     9,     0,     0,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,   251,     0,     2,     3,     0,     0,     0,     0,
       4,     0,     5,   252,     6,     0,     7,     8,     9,     0,
       0,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,     2,     3,   217,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
       0,    69,   218,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,     2,
       3,   228,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,     0,    69,   229,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,     2,     3,   276,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,     0,    69,   277,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,     2,     3,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,     0,
      69,     0,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,     2,     3,
       0,     0,     0,     0,     0,     0,     5,     0,     6,     0,
     122,     8,     9,     0,     0,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,     0,    34,
       2,     3,     2,     3,     0,     0,     0,     0,     0,     0,
       5,     0,     6,     0,     0,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,     0,
     124,     0,    23,     2,     3,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,     0,   124,   157,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,   165,     0,    12,     0,     0,    15,     6,
       0,    18,     8,     9,    21,   154,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,     0,   166,
      11,     0,     0,    14,     0,     0,    17,     0,     0,    20,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21
};

static const yytype_int16 yycheck[] =
{
       0,     6,   128,    47,    48,    64,    50,    38,    52,    31,
     172,    34,     9,    38,    50,    24,    25,    26,     3,     4,
      48,     1,    13,    23,    52,    53,    48,    32,    50,     9,
      30,    31,    55,    33,    34,    63,    34,     7,    38,     6,
      63,     3,     4,    10,     3,     4,    34,     1,    48,     0,
      50,    60,    11,    15,     8,     9,    54,    57,    58,    59,
      45,     3,   224,     3,    64,    63,    33,   193,    56,     8,
     196,   197,   198,     6,    33,    63,    11,   136,     3,     4,
       8,     1,    41,     3,     4,    13,     9,    20,   116,    12,
      23,   119,    12,    26,   117,    15,    29,   102,   126,   143,
     144,     9,     8,   112,    12,   149,    11,   130,    33,   145,
       3,     4,   121,     1,    14,   143,     7,    37,    38,   117,
       8,     8,   248,   145,   250,   125,    14,    13,   128,   117,
      16,    17,   130,   133,     3,     4,   136,    16,    17,   139,
     140,   172,   130,     7,    11,   145,    15,   172,     8,     3,
       4,    33,    11,   153,    13,   164,   156,    16,    17,   159,
     165,    15,     7,     9,    14,   209,   166,   167,   168,   169,
     201,   176,   172,   178,    11,   180,   201,    45,   183,   207,
       7,     9,     8,    37,    38,   190,   186,     7,     7,    12,
     133,   191,    33,   139,   237,   253,   182,   197,   198,     0,
       1,   201,     3,     4,   236,   181,   260,   262,     9,   130,
      11,    12,    13,    30,    15,    16,    17,    -1,    62,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,   176,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   257,   253,    -1,    -1,    -1,    -1,   263,    -1,
     260,    -1,   262,    -1,   264,     0,     1,    -1,     3,     4,
      -1,    -1,    -1,    -1,     9,    -1,    11,    -1,    13,    -1,
      15,    16,    17,    -1,    -1,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
       0,     1,    -1,     3,     4,    -1,    -1,    -1,    -1,     9,
      -1,    11,    -1,    13,    -1,    15,    16,    17,    -1,    -1,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,     1,    -1,     3,     4,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,
      -1,    -1,    18,    -1,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
       1,    -1,     3,     4,    -1,    -1,    -1,    -1,     9,    -1,
      11,    12,    13,    -1,    15,    16,    17,    -1,    -1,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,     1,    -1,     3,     4,    -1,    -1,
      -1,    -1,     9,    -1,    11,    12,    13,    -1,    15,    16,
      17,    -1,    -1,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,     1,    -1,
       3,     4,    -1,    -1,    -1,    -1,     9,    -1,    11,    12,
      13,    -1,    15,    16,    17,    -1,    -1,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,     1,    -1,     3,     4,    -1,    -1,    -1,    -1,
       9,    -1,    11,    12,    13,    -1,    15,    16,    17,    -1,
      -1,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,     3,     4,     5,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,     3,
       4,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,     3,     4,     5,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,     3,     4,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      18,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,     3,     4,
      -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,    13,    -1,
      15,    16,    17,    -1,    -1,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    -1,    44,
       3,     4,     3,     4,    -1,    -1,    -1,    -1,    -1,    -1,
      11,    -1,    13,    -1,    -1,    16,    17,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    -1,    33,     3,     4,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    33,     6,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,     7,    -1,    22,    -1,    -1,    25,    13,
      -1,    28,    16,    17,    31,     6,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      21,    -1,    -1,    24,    -1,    -1,    27,    -1,    -1,    30,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,     4,     9,    11,    13,    15,    16,    17,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    60,    72,    73,    74,
      76,    77,    78,    82,    83,    84,    85,    86,    87,    88,
      92,    93,    94,    97,   101,     9,     1,    14,    15,    18,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    95,    96,    97,    98,
      99,   100,     7,    97,    94,    94,    94,     3,    64,    65,
      61,    62,    63,   100,     3,    66,    82,    97,    75,    78,
      97,   100,    15,    55,    33,    80,    81,    86,    87,    88,
      97,     0,     1,    12,    51,    53,    11,     1,     9,     8,
       8,    49,    49,    78,    93,    41,    49,    75,    49,    93,
      93,     6,    86,    89,     6,    87,    90,     6,    88,    91,
      97,    97,    97,    94,    92,     7,    33,    86,    87,    88,
      93,    12,    50,   102,    14,     1,     8,    14,     7,   100,
       7,     8,     8,     8,    94,    11,    67,    93,    93,    94,
       7,    45,    79,    97,    93,    79,    97,    80,    87,    48,
      12,    50,     9,    57,    97,    58,    59,    97,    49,    93,
      49,    75,    49,    97,    97,    97,    94,     5,    19,   100,
      97,    97,    97,    97,     1,   102,    14,    96,     5,    19,
     100,   100,    65,    62,   100,    15,    37,    38,    68,    69,
      70,    71,    97,   100,    97,    79,    79,    79,    97,    79,
      97,     1,    12,     8,    93,    49,   102,     7,    64,    61,
       9,     1,     8,     7,     6,    10,    33,    79,    79,    12,
      59,   100,     1,    12,    69,    71,     5,    19,   100,    97,
      12
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (&yylloc, RESULT, yyscanner, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, &yylloc, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, &yylloc, yyscanner)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location, RESULT, yyscanner); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, YYMSC_RESULT_TYPE &RESULT, void *yyscanner)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, RESULT, yyscanner)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    YYMSC_RESULT_TYPE &RESULT;
    void *yyscanner;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
  YYUSE (RESULT);
  YYUSE (yyscanner);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, YYMSC_RESULT_TYPE &RESULT, void *yyscanner)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp, RESULT, yyscanner)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    YYMSC_RESULT_TYPE &RESULT;
    void *yyscanner;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, RESULT, yyscanner);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, YYMSC_RESULT_TYPE &RESULT, void *yyscanner)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule, RESULT, yyscanner)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
    YYMSC_RESULT_TYPE &RESULT;
    void *yyscanner;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       , RESULT, yyscanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule, RESULT, yyscanner); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, YYMSC_RESULT_TYPE &RESULT, void *yyscanner)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp, RESULT, yyscanner)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
    YYMSC_RESULT_TYPE &RESULT;
    void *yyscanner;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (RESULT);
  YYUSE (yyscanner);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {
      case 3: /* "TOK_STRING" */

/* Line 1000 of yacc.c  */
#line 189 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1724 "colorsyntax.cc"
	break;
      case 4: /* "TOK_QSTRING" */

/* Line 1000 of yacc.c  */
#line 189 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1733 "colorsyntax.cc"
	break;
      case 5: /* "TOK_NUMBER" */

/* Line 1000 of yacc.c  */
#line 193 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1742 "colorsyntax.cc"
	break;
      case 15: /* "TOK_MSC" */

/* Line 1000 of yacc.c  */
#line 190 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1751 "colorsyntax.cc"
	break;
      case 16: /* "TOK_COLON_STRING" */

/* Line 1000 of yacc.c  */
#line 189 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1760 "colorsyntax.cc"
	break;
      case 17: /* "TOK_COLON_QUOTED_STRING" */

/* Line 1000 of yacc.c  */
#line 189 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1769 "colorsyntax.cc"
	break;
      case 18: /* "TOK_STYLE_NAME" */

/* Line 1000 of yacc.c  */
#line 189 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1778 "colorsyntax.cc"
	break;
      case 19: /* "TOK_BOOLEAN" */

/* Line 1000 of yacc.c  */
#line 193 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1787 "colorsyntax.cc"
	break;
      case 34: /* "TOK_COMMAND_HEADING" */

/* Line 1000 of yacc.c  */
#line 192 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1796 "colorsyntax.cc"
	break;
      case 35: /* "TOK_COMMAND_NUDGE" */

/* Line 1000 of yacc.c  */
#line 192 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1805 "colorsyntax.cc"
	break;
      case 36: /* "TOK_COMMAND_NEWPAGE" */

/* Line 1000 of yacc.c  */
#line 192 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1814 "colorsyntax.cc"
	break;
      case 37: /* "TOK_COMMAND_DEFCOLOR" */

/* Line 1000 of yacc.c  */
#line 191 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1823 "colorsyntax.cc"
	break;
      case 38: /* "TOK_COMMAND_DEFSTYLE" */

/* Line 1000 of yacc.c  */
#line 191 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1832 "colorsyntax.cc"
	break;
      case 39: /* "TOK_COMMAND_DEFDESIGN" */

/* Line 1000 of yacc.c  */
#line 191 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1841 "colorsyntax.cc"
	break;
      case 40: /* "TOK_COMMAND_BIG" */

/* Line 1000 of yacc.c  */
#line 190 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1850 "colorsyntax.cc"
	break;
      case 41: /* "TOK_COMMAND_PIPE" */

/* Line 1000 of yacc.c  */
#line 190 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1859 "colorsyntax.cc"
	break;
      case 42: /* "TOK_COMMAND_MARK" */

/* Line 1000 of yacc.c  */
#line 193 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1868 "colorsyntax.cc"
	break;
      case 43: /* "TOK_COMMAND_PARALLEL" */

/* Line 1000 of yacc.c  */
#line 190 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1877 "colorsyntax.cc"
	break;
      case 48: /* "top_level_arclist" */

/* Line 1000 of yacc.c  */
#line 185 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1000 of yacc.c  */
#line 1886 "colorsyntax.cc"
	break;
      case 49: /* "braced_arclist" */

/* Line 1000 of yacc.c  */
#line 185 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1000 of yacc.c  */
#line 1895 "colorsyntax.cc"
	break;
      case 50: /* "arclist" */

/* Line 1000 of yacc.c  */
#line 185 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1000 of yacc.c  */
#line 1904 "colorsyntax.cc"
	break;
      case 51: /* "mscenclosed" */

/* Line 1000 of yacc.c  */
#line 185 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1000 of yacc.c  */
#line 1913 "colorsyntax.cc"
	break;
      case 53: /* "complete_arc" */

/* Line 1000 of yacc.c  */
#line 181 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1000 of yacc.c  */
#line 1922 "colorsyntax.cc"
	break;
      case 54: /* "arc_with_parallel" */

/* Line 1000 of yacc.c  */
#line 181 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1000 of yacc.c  */
#line 1931 "colorsyntax.cc"
	break;
      case 55: /* "arc" */

/* Line 1000 of yacc.c  */
#line 181 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1000 of yacc.c  */
#line 1940 "colorsyntax.cc"
	break;
      case 56: /* "optlist" */

/* Line 1000 of yacc.c  */
#line 185 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1000 of yacc.c  */
#line 1949 "colorsyntax.cc"
	break;
      case 57: /* "opt" */

/* Line 1000 of yacc.c  */
#line 181 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1000 of yacc.c  */
#line 1958 "colorsyntax.cc"
	break;
      case 58: /* "entitylist" */

/* Line 1000 of yacc.c  */
#line 186 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entitylist);};

/* Line 1000 of yacc.c  */
#line 1967 "colorsyntax.cc"
	break;
      case 59: /* "entity" */

/* Line 1000 of yacc.c  */
#line 186 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entity);};

/* Line 1000 of yacc.c  */
#line 1976 "colorsyntax.cc"
	break;
      case 60: /* "first_entity" */

/* Line 1000 of yacc.c  */
#line 186 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entity);};

/* Line 1000 of yacc.c  */
#line 1985 "colorsyntax.cc"
	break;
      case 63: /* "tok_stringlist" */

/* Line 1000 of yacc.c  */
#line 187 "language.yy"
	{if (!C_S_H) delete (yyvaluep->stringlist);};

/* Line 1000 of yacc.c  */
#line 1994 "colorsyntax.cc"
	break;
      case 72: /* "parallel" */

/* Line 1000 of yacc.c  */
#line 184 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcparallel);};

/* Line 1000 of yacc.c  */
#line 2003 "colorsyntax.cc"
	break;
      case 73: /* "emphasis_list" */

/* Line 1000 of yacc.c  */
#line 183 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};

/* Line 1000 of yacc.c  */
#line 2012 "colorsyntax.cc"
	break;
      case 74: /* "first_emphasis" */

/* Line 1000 of yacc.c  */
#line 183 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};

/* Line 1000 of yacc.c  */
#line 2021 "colorsyntax.cc"
	break;
      case 75: /* "pipe_def" */

/* Line 1000 of yacc.c  */
#line 183 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};

/* Line 1000 of yacc.c  */
#line 2030 "colorsyntax.cc"
	break;
      case 76: /* "pipe_def_list" */

/* Line 1000 of yacc.c  */
#line 183 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};

/* Line 1000 of yacc.c  */
#line 2039 "colorsyntax.cc"
	break;
      case 77: /* "pipe_emphasis" */

/* Line 1000 of yacc.c  */
#line 183 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};

/* Line 1000 of yacc.c  */
#line 2048 "colorsyntax.cc"
	break;
      case 78: /* "emphrel" */

/* Line 1000 of yacc.c  */
#line 183 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};

/* Line 1000 of yacc.c  */
#line 2057 "colorsyntax.cc"
	break;
      case 79: /* "vertxpos" */

/* Line 1000 of yacc.c  */
#line 180 "language.yy"
	{if (!C_S_H) delete (yyvaluep->vertxpos);};

/* Line 1000 of yacc.c  */
#line 2066 "colorsyntax.cc"
	break;
      case 81: /* "vertrel" */

/* Line 1000 of yacc.c  */
#line 181 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1000 of yacc.c  */
#line 2075 "colorsyntax.cc"
	break;
      case 82: /* "arcrel" */

/* Line 1000 of yacc.c  */
#line 181 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1000 of yacc.c  */
#line 2084 "colorsyntax.cc"
	break;
      case 83: /* "arcrel_to" */

/* Line 1000 of yacc.c  */
#line 182 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 1000 of yacc.c  */
#line 2093 "colorsyntax.cc"
	break;
      case 84: /* "arcrel_from" */

/* Line 1000 of yacc.c  */
#line 182 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 1000 of yacc.c  */
#line 2102 "colorsyntax.cc"
	break;
      case 85: /* "arcrel_bidir" */

/* Line 1000 of yacc.c  */
#line 182 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 1000 of yacc.c  */
#line 2111 "colorsyntax.cc"
	break;
      case 93: /* "full_arcattrlist_with_label" */

/* Line 1000 of yacc.c  */
#line 187 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1000 of yacc.c  */
#line 2120 "colorsyntax.cc"
	break;
      case 94: /* "full_arcattrlist" */

/* Line 1000 of yacc.c  */
#line 187 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1000 of yacc.c  */
#line 2129 "colorsyntax.cc"
	break;
      case 95: /* "arcattrlist" */

/* Line 1000 of yacc.c  */
#line 187 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1000 of yacc.c  */
#line 2138 "colorsyntax.cc"
	break;
      case 96: /* "arcattr" */

/* Line 1000 of yacc.c  */
#line 187 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attrib);};

/* Line 1000 of yacc.c  */
#line 2147 "colorsyntax.cc"
	break;
      case 97: /* "entity_string" */

/* Line 1000 of yacc.c  */
#line 188 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 2156 "colorsyntax.cc"
	break;
      case 98: /* "reserved_word_string" */

/* Line 1000 of yacc.c  */
#line 188 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 2165 "colorsyntax.cc"
	break;
      case 99: /* "symbol_string" */

/* Line 1000 of yacc.c  */
#line 188 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 2174 "colorsyntax.cc"
	break;
      case 100: /* "string" */

/* Line 1000 of yacc.c  */
#line 188 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 2183 "colorsyntax.cc"
	break;
      case 102: /* "scope_close" */

/* Line 1000 of yacc.c  */
#line 181 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1000 of yacc.c  */
#line 2192 "colorsyntax.cc"
	break;

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (YYMSC_RESULT_TYPE &RESULT, void *yyscanner);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */





/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (YYMSC_RESULT_TYPE &RESULT, void *yyscanner)
#else
int
yyparse (RESULT, yyscanner)
    YYMSC_RESULT_TYPE &RESULT;
    void *yyscanner;
#endif
#endif
{
/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.
       `yyls': related to locations.

       Refer to the stacks thru separate pointers, to allow yyoverflow
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
    YYLTYPE yyerror_range[2];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
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

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;

#if YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 1;
#endif

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
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
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
  *++yyvsp = yylval;
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
     `$$ = $1'.

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

/* Line 1455 of yacc.c  */
#line 198 "language.yy"
    {
    //no action for empty file
}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 202 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        msc.AddArcs((yyvsp[(1) - (1)].arclist));
  #endif
}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 211 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_ERROR);
  #else
        (yyval.arclist) = (yyvsp[(1) - (2)].arclist);
  #endif
}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 219 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_ERROR);
  #else
        (yyval.arclist) = (yyvsp[(1) - (2)].arclist);
        msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).start, "Unexpected '}'.");
  #endif
}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 228 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_ERROR);
  #else
        //Merge $3 into $1
        ((yyvsp[(1) - (3)].arclist))->splice(((yyvsp[(1) - (3)].arclist))->end(), *((yyvsp[(3) - (3)].arclist)));
        delete ((yyvsp[(3) - (3)].arclist));
        (yyval.arclist) = (yyvsp[(1) - (3)].arclist);
        msc.Error.Error(MSC_POS((yylsp[(3) - (3)])).start, "Unexpected '}'.");
  #endif
}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 242 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_BRACE);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_BRACE);
  #else
        if ((yyvsp[(3) - (3)].arcbase)) ((yyvsp[(2) - (3)].arclist))->Append((yyvsp[(3) - (3)].arcbase)); //Append any potential CommandNumbering
        (yyval.arclist) = (yyvsp[(2) - (3)].arclist);
  #endif
}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 252 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_BRACE);
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_BRACE);
  #else
         (yyval.arclist) = new ArcList;
         //scope_close should not return here with a CommandNumbering
         //but just in case
         if ((yyvsp[(2) - (2)].arcbase))
            delete((yyvsp[(2) - (2)].arcbase));
  #endif
}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 265 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (4)]), COLOR_BRACE);
        csh.AddCSH((yylsp[(3) - (4)]), COLOR_ERROR);
        csh.AddCSH((yylsp[(4) - (4)]), COLOR_BRACE);
  #else
        if ((yyvsp[(4) - (4)].arcbase)) ((yyvsp[(2) - (4)].arclist))->Append((yyvsp[(4) - (4)].arcbase)); //Append any potential CommandNumbering
        (yyval.arclist) = (yyvsp[(2) - (4)].arclist);
  #endif
    yyerrok;
}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 277 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_BRACE);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_ERROR);
  #else
        (yyval.arclist) = (yyvsp[(2) - (3)].arclist);
        //Do not pop context, as the missing scope_close would have done
  #endif
}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 289 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(1) - (1)].arcbase))
        (yyval.arclist) = (new ArcList)->Append((yyvsp[(1) - (1)].arcbase)); /* New list */
    else
        (yyval.arclist) = new ArcList;
  #endif
}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 298 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(2) - (2)].arcbase))
        ((yyvsp[(1) - (2)].arclist))->Append((yyvsp[(2) - (2)].arcbase));     /* Add to existing list */
    (yyval.arclist) = ((yyvsp[(1) - (2)].arclist));
  #endif
}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 306 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.arclist) = (yyvsp[(1) - (1)].arclist);
  #endif
}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 312 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    //Merge $2 into $1
    ((yyvsp[(1) - (2)].arclist))->splice(((yyvsp[(1) - (2)].arclist))->end(), *((yyvsp[(2) - (2)].arclist)));
    delete ((yyvsp[(2) - (2)].arclist));
    (yyval.arclist) = (yyvsp[(1) - (2)].arclist);
  #endif
}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 325 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (4)]), COLOR_BRACE);
        csh.AddCSH((yylsp[(4) - (4)]), COLOR_BRACE);
        if (csh.hintStatus == HINT_ATTR_VALUE_CAN_COME) {
            if (csh.CursorIn(((yylsp[(1) - (4)])).last_pos,((yylsp[(2) - (4)])).first_pos)) {
                csh.hintStatus = HINT_READY;
                for (auto i = csh.Designs.begin(); i!=csh.Designs.end(); i++)
                    csh.Hints.insert(i->first);
            } else
                csh.hintStatus = HINT_NONE;
        }
  #else
        (yyval.arclist) = (yyvsp[(3) - (4)].arclist);
  #endif
}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 342 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_BRACE);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_BRACE);
        if (csh.hintStatus == HINT_ATTR_VALUE_CAN_COME) {
            if (csh.CursorIn(((yylsp[(1) - (3)])).last_pos,((yylsp[(2) - (3)])).first_pos)) {
                csh.hintStatus = HINT_READY;
                for (auto i = csh.Designs.begin(); i!=csh.Designs.end(); i++)
                    csh.Hints.insert(i->first);
            } else
                csh.hintStatus = HINT_NONE;
        }
  #else
        (yyval.arclist) = new ArcList;
  #endif
}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 359 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (5)]), COLOR_BRACE);
        csh.AddCSH((yylsp[(4) - (5)]), COLOR_ERROR);
        csh.AddCSH((yylsp[(5) - (5)]), COLOR_BRACE);
        if (csh.hintStatus == HINT_ATTR_VALUE_CAN_COME) {
            if (csh.CursorIn(((yylsp[(1) - (5)])).last_pos,((yylsp[(2) - (5)])).first_pos)) {
                csh.hintStatus = HINT_READY;
                for (auto i = csh.Designs.begin(); i!=csh.Designs.end(); i++)
                    csh.Hints.insert(i->first);
            } else
                csh.hintStatus = HINT_NONE;
        }
  #else
        (yyval.arclist) = (yyvsp[(3) - (5)].arclist);
  #endif
    yyerrok;
}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 378 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (4)]), COLOR_BRACE);
        csh.AddCSH((yylsp[(4) - (4)]), COLOR_ERROR);
        if (csh.hintStatus == HINT_ATTR_VALUE_CAN_COME) {
            if (csh.CursorIn(((yylsp[(1) - (4)])).last_pos,((yylsp[(2) - (4)])).first_pos)) {
                csh.hintStatus = HINT_READY;
                for (auto i = csh.Designs.begin(); i!=csh.Designs.end(); i++)
                    csh.Hints.insert(i->first);
            } else
                csh.hintStatus = HINT_NONE;
        }
  #else
        (yyval.arclist) = (yyvsp[(3) - (4)].arclist);
  #endif
}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 396 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
  #else
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 404 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_EQUAL);
        csh.hintStatus = HINT_ATTR_VALUE_CAN_COME;

  #else
        msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).end, "Missing design name.");
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 416 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_DESIGNNAME);
        if (csh.CursorIn(((yylsp[(2) - (3)])).last_pos,((yylsp[(3) - (3)])).last_pos)) {
            csh.hintStatus = HINT_READY;
            csh.hintedStringPos = (yylsp[(3) - (3)]);
            for (auto i = csh.Designs.begin(); i!=csh.Designs.end(); i++)
                csh.Hints.insert(i->first);
        }
  #else
        msc.AddAttribute(Attribute("msc", (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(1) - (3)])), MSC_POS((yylsp[(3) - (3)]))));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 435 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (1)]), COLOR_SEMICOLON);
  #else
        (yyval.arcbase)=NULL;
  #endif
}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 443 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_ERROR);
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_SEMICOLON);
  #else
        (yyval.arcbase)=NULL;
  #endif
}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 452 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_SEMICOLON);
  #else
        if ((yyvsp[(1) - (2)].arcbase)) ((yyvsp[(1) - (2)].arcbase))->SetLineEnd(MSC_POS((yyloc)));
        (yyval.arcbase)=(yyvsp[(1) - (2)].arcbase);
  #endif
}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 461 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_ERROR);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_SEMICOLON);
  #else
        if ((yyvsp[(1) - (3)].arcbase)) ((yyvsp[(1) - (3)].arcbase))->SetLineEnd(MSC_POS((yyloc)));
        (yyval.arcbase)=(yyvsp[(1) - (3)].arcbase);
  #endif
    yyerrok;
}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 474 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
        ((yyvsp[(2) - (2)].arcbase))->SetParallel();
        (yyval.arcbase) = (yyvsp[(2) - (2)].arcbase);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 486 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        if (csh.hintStatus==HINT_ATTR_NAME) {
            const_char_vector_t v;
            ArcArrow::AttributeNames(v, csh);
            csh.SetHintsReady(std::move(v));
        } else if (csh.hintStatus==HINT_ATTR_VALUE) {
            const_char_vector_t v;
            ArcArrow::AttributeValues(csh.hintAttrName, v, csh);
            csh.SetHintsReady(std::move(v));
        }
  #else
    (yyval.arcbase) = ((yyvsp[(1) - (2)].arcbase))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
  #endif
}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 502 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
        //Returns NULL, if BIG is before a self-pointing arrow
        (yyval.arcbase) = msc.CreateArcBigArrow((yyvsp[(2) - (2)].arcbase));
        delete (yyvsp[(2) - (2)].arcbase);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 513 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        if (csh.hintStatus==HINT_ATTR_NAME) {
            const_char_vector_t v;
            ArcBigArrow::AttributeNames(v, csh);
            csh.SetHintsReady(std::move(v));
        } else if (csh.hintStatus==HINT_ATTR_VALUE) {
            const_char_vector_t v;
            ArcBigArrow::AttributeValues(csh.hintAttrName, v, csh);
            csh.SetHintsReady(std::move(v));
        }
  #else
        //Returns NULL, if BIG is before a self-pointing arrow
        ArcBigArrow *arrow = msc.CreateArcBigArrow((yyvsp[(2) - (3)].arcbase));
        if (arrow) arrow->AddAttributeList((yyvsp[(3) - (3)].attriblist));
        (yyval.arcbase) = arrow;
        delete (yyvsp[(2) - (3)].arcbase);
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 535 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
        (yyval.arcbase) = (yyvsp[(2) - (2)].arcbase);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 544 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        if (csh.hintStatus==HINT_ATTR_NAME) {
            const_char_vector_t v;
            ArcVerticalArrow::AttributeNames(v, csh);
            csh.SetHintsReady(std::move(v));
        } else if (csh.hintStatus==HINT_ATTR_VALUE) {
            const_char_vector_t v;
            ArcVerticalArrow::AttributeValues(csh.hintAttrName, v, csh);
            csh.SetHintsReady(std::move(v));
        }
  #else
        (yyval.arcbase) = ((yyvsp[(2) - (3)].arcbase))->AddAttributeList((yyvsp[(3) - (3)].attriblist));
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 562 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        if (csh.hintStatus==HINT_ATTR_NAME) {
            const_char_vector_t v;
            ArcDivider::AttributeNames(v, csh);
            csh.SetHintsReady(std::move(v));
        } else if (csh.hintStatus==HINT_ATTR_VALUE) {
            const_char_vector_t v;
            ArcDivider::AttributeValues(csh.hintAttrName, v, csh);
            csh.SetHintsReady(std::move(v));
        }
  #else
    (yyval.arcbase) = (new ArcDivider(MSC_ARC_VSPACE, &msc))->AddAttributeList((yyvsp[(1) - (1)].attriblist));
  #endif
}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 578 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.arcbase) = new CommandEntity((new EntityDefList)->Append((yyvsp[(1) - (1)].entity)), &msc);
  #endif
}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 584 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #else
    (yyval.arcbase) = new CommandEntity(((yyvsp[(3) - (3)].entitylist))->Prepend((yyvsp[(1) - (3)].entity)), &msc);
  #endif
}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 592 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    /* If there were arcs defined by the options (e.g., background)
     * enclose them in a single item parallel element. */
    if ((yyvsp[(1) - (1)].arclist)) {
        (yyval.arcbase) = (new ArcParallel(&msc))->AddArcList((yyvsp[(1) - (1)].arclist));
    } else
        (yyval.arcbase) = NULL;
  #endif
}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 603 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcemph); //to remove warning for downcast
  #endif
}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 609 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcparallel);
  #endif
}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 615 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
        (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 624 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
        (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 633 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
        (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 642 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
  #else
        (yyval.arcbase) = new CommandEntity(NULL, &msc);
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 651 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
        (yyval.arcbase) = (new CommandEntity(NULL, &msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 660 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
  #else
        (yyval.arcbase) = new ArcDivider(MSC_COMMAND_NUDGE, &msc);
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 669 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
        (yyval.arcbase) = (new ArcDivider(MSC_COMMAND_NUDGE, &msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 678 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_MARKERNAME);
  #else
        (yyval.arcbase) = new CommandMark((yyvsp[(2) - (2)].str), MSC_POS((yyloc)), &msc);
  #endif
    free((yyvsp[(1) - (2)].str));
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 689 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_MARKERNAME);
  #else
        (yyval.arcbase) = (new CommandMark((yyvsp[(2) - (3)].str), MSC_POS((yyloc)), &msc))->AddAttributeList((yyvsp[(3) - (3)].attriblist));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 700 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
  #else
        (yyval.arcbase) = new CommandNewpage(&msc);
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 709 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
        (yyval.arcbase) = (new CommandNewpage(&msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 719 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(1) - (1)].arcbase))
        (yyval.arclist) = (new ArcList)->Append((yyvsp[(1) - (1)].arcbase)); /* New list */
    else
        (yyval.arclist) = NULL;
  #endif
}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 728 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #else
    if ((yyvsp[(3) - (3)].arcbase)) {
        if ((yyvsp[(1) - (3)].arclist))
            (yyval.arclist) = ((yyvsp[(1) - (3)].arclist))->Append((yyvsp[(3) - (3)].arcbase));     /* Add to existing list */
        else
            (yyval.arclist) = (new ArcList)->Append((yyvsp[(3) - (3)].arcbase)); /* New list */
    }
    (yyval.arclist) = (yyvsp[(1) - (3)].arclist);
  #endif
}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 743 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_OPTIONNAME);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_ATTRVALUE);
  #else
        msc.AddAttribute(Attribute((yyvsp[(1) - (3)].str), str2bool((yyvsp[(3) - (3)].str)), MSC_POS((yylsp[(1) - (3)])), MSC_POS((yylsp[(3) - (3)])), (yyvsp[(3) - (3)].str)));
        (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 756 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_OPTIONNAME);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_ATTRVALUE);
  #else
       msc.AddAttribute(Attribute((yyvsp[(1) - (3)].str), atof((yyvsp[(3) - (3)].str)), MSC_POS((yylsp[(1) - (3)])), MSC_POS((yylsp[(3) - (3)])), (yyvsp[(3) - (3)].str)));
       (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 769 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_OPTIONNAME);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        csh.AddCSH_AttrValue((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str), (yyvsp[(1) - (3)].str));
  #else
        Attribute a((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(1) - (3)])), MSC_POS((yylsp[(3) - (3)])));
        MscFillAttr fill;
        fill.Empty();
        if (a.StartsWith("background") && fill.AddAttribute(a, &msc, STYLE_OPTION)) {
            (yyval.arcbase) = new CommandNewBackground(&msc, fill);
        } else {
            msc.AddAttribute(a);
            (yyval.arcbase) = NULL;
        }
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 789 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_ATTRVALUE);
        if (csh.Designs.find((yyvsp[(1) - (3)].str)) != csh.Designs.end())
            for (auto i = csh.Contexts.begin(); i!=csh.Contexts.end(); i++)
                *i += csh.Designs[(yyvsp[(1) - (3)].str)];
  #else
        msc.AddAttribute(Attribute("msc", (yyvsp[(3) - (3)].str), MSC_POS((yyloc)), MSC_POS((yylsp[(3) - (3)]))));
        (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 806 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.entitylist) = (EntityDefList*)((new EntityDefList)->Append((yyvsp[(1) - (1)].entity)));
  #endif
}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 812 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #else
    (yyval.entitylist) = (EntityDefList*)(((yyvsp[(1) - (3)].entitylist))->Append((yyvsp[(3) - (3)].entity)));
  #endif
}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 821 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_EntityName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
  #else
        (yyval.entity) = (new EntityDef((yyvsp[(1) - (2)].str), &msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist), &msc);
        ((yyval.entity))->SetLineEnd(MSC_POS((yyloc)));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 831 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_EntityName((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str));
  #else
        (yyval.entity) = (new EntityDef((yyvsp[(1) - (1)].str), &msc))->AddAttributeList(NULL, &msc);
        ((yyval.entity))->SetLineEnd(MSC_POS((yyloc)));
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 842 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_KeywordOrEntity((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
  #else
        (yyval.entity) = (new EntityDef((yyvsp[(1) - (2)].str), &msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist), &msc);
        ((yyval.entity))->SetLineEnd(MSC_POS((yyloc)));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 852 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_KeywordOrEntity((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str));
  #else
        (yyval.entity) = (new EntityDef((yyvsp[(1) - (1)].str), &msc))->AddAttributeList(NULL, &msc);
        ((yyval.entity))->SetLineEnd(MSC_POS((yyloc)));
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 866 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #endif
}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 873 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    for (std::list<std::string>::iterator i = ((yyvsp[(1) - (2)].stringlist))->begin(); i!=((yyvsp[(1) - (2)].stringlist))->end(); i++)
        if (csh.ForbiddenStyles.find(*i) != csh.ForbiddenStyles.end())
            csh.Contexts.back().StyleNames.insert(string(*i));
  #else
    for (std::list<std::string>::iterator i = ((yyvsp[(1) - (2)].stringlist))->begin(); i!=((yyvsp[(1) - (2)].stringlist))->end(); i++) {
        MscStyle style = msc.Contexts.back().styles.GetStyle(*i);
        AttributeList::iterator j=((yyvsp[(2) - (2)].attriblist))->begin();
        while (j!=((yyvsp[(2) - (2)].attriblist))->end())
           style.AddAttribute(**(j++), &msc);
        msc.Contexts.back().styles[*i] = style;
    }
    delete((yyvsp[(1) - (2)].stringlist));
    delete((yyvsp[(2) - (2)].attriblist));
  #endif
}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 892 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (1)]), COLOR_STYLENAME);
        (yyval.stringlist) = new std::list<string>;
        if (((yyvsp[(1) - (1)].str)) == "emphasis")
            ((yyval.stringlist))->push_back("box");
        else if (((yyvsp[(1) - (1)].str)) == "emptyemphasis")
            ((yyval.stringlist))->push_back("emptybox");
        else ((yyval.stringlist))->push_back((yyvsp[(1) - (1)].str));
  #else
        (yyval.stringlist) = new std::list<string>;
        ((yyval.stringlist))->push_back(ConvertEmphasisToBox((yyvsp[(1) - (1)].str), &(yylsp[(1) - (1)]), msc));
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 908 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_STYLENAME);
        (yyval.stringlist) = (yyvsp[(1) - (3)].stringlist);
        if (((yyvsp[(3) - (3)].str)) == "emphasis")
            ((yyval.stringlist))->push_back("box");
        else if (((yyvsp[(3) - (3)].str)) == "emptyemphasis")
            ((yyval.stringlist))->push_back("emptybox");
        else ((yyval.stringlist))->push_back((yyvsp[(3) - (3)].str));
  #else
        ((yyvsp[(1) - (3)].stringlist))->push_back(ConvertEmphasisToBox((yyvsp[(3) - (3)].str), &(yylsp[(3) - (3)]), msc));
        (yyval.stringlist) = (yyvsp[(1) - (3)].stringlist);
  #endif
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 927 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #endif
}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 934 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_COLORNAME);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_COLORDEF);
        csh.Contexts.back().ColorNames.insert((yyvsp[(1) - (3)].str));
  #else
        msc.Contexts.back().colors.AddColor((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), msc.Error, MSC_POS((yyloc)));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 948 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (5)]), COLOR_DESIGNNAME);
        csh.AddCSH((yylsp[(2) - (5)]), COLOR_BRACE);
        csh.AddCSH((yylsp[(4) - (5)]), COLOR_SEMICOLON);
        csh.AddCSH((yylsp[(5) - (5)]), COLOR_BRACE);
        csh.Designs[(yyvsp[(1) - (5)].str)] = csh.Contexts.back();
        csh.PopContext();
  #else
    //cope_open_empty pushed an empty color & style set onto the stack
    //then designelementlist added color & style definitions, now we harvest those
    Design &design = msc.Designs[(yyvsp[(1) - (5)].str)];
    static_cast<Context&>(design) = msc.Contexts.back();
    design.hscale = msc.hscale;
    msc.hscale = msc.saved_hscale;
    msc.PopContext();
  #endif
    free((yyvsp[(1) - (5)].str));
}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 968 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (6)]), COLOR_DESIGNNAME);
        csh.AddCSH((yylsp[(2) - (6)]), COLOR_BRACE);
        csh.AddCSH((yylsp[(4) - (6)]), COLOR_SEMICOLON);
        csh.AddCSH((yylsp[(5) - (6)]), COLOR_ERROR);
        csh.AddCSH((yylsp[(6) - (6)]), COLOR_BRACE);
        csh.Designs[(yyvsp[(1) - (6)].str)] = csh.Contexts.back();
        csh.PopContext();
  #else
    //if closing brace missing, still do the design definition
    //cope_open_empty pushed an empty color & style set onto the stack
    //then designelementlist added color & style definitions, now we harvest those
    Design &design = msc.Designs[(yyvsp[(1) - (6)].str)];
    static_cast<Context&>(design) = msc.Contexts.back();
    design.hscale = msc.hscale;
    msc.hscale = msc.saved_hscale;
    msc.PopContext();
  #endif
}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 991 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.PushContext(true);
  #else
    //push empty color & style sets for design definition
    msc.PushContext(true);
    msc.saved_hscale = msc.hscale;
  #endif
}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 1003 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_SEMICOLON);
  #endif
}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 1010 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 1017 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 1027 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #endif
}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 1033 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_ERROR);
  #endif
}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 1040 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_OPTIONNAME);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_ATTRVALUE);
  #else
        msc.AddDesignAttribute(Attribute((yyvsp[(1) - (3)].str), str2bool((yyvsp[(3) - (3)].str)), MSC_POS((yyloc)), MSC_POS((yylsp[(3) - (3)])), (yyvsp[(3) - (3)].str)));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 1052 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_OPTIONNAME);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_ATTRVALUE);
  #else
        msc.AddDesignAttribute(Attribute((yyvsp[(1) - (3)].str), atof((yyvsp[(3) - (3)].str)), MSC_POS((yyloc)), MSC_POS((yylsp[(3) - (3)])), (yyvsp[(3) - (3)].str)));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 1064 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_OPTIONNAME);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_ATTRVALUE);
  #else
        msc.AddDesignAttribute(Attribute((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(1) - (3)])), MSC_POS((yylsp[(3) - (3)]))));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 1076 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_ATTRVALUE);
  #else
        msc.AddDesignAttribute(Attribute("msc", (yyvsp[(3) - (3)].str), MSC_POS((yyloc)), MSC_POS((yylsp[(3) - (3)]))));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 1090 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(1) - (1)].arclist))
        (yyval.arcparallel) = (new ArcParallel(&msc))->AddArcList((yyvsp[(1) - (1)].arclist));
    else
        (yyval.arcparallel) = NULL;
  #endif
}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 1099 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(2) - (2)].arclist)==NULL)
        (yyval.arcparallel) = (yyvsp[(1) - (2)].arcparallel);
    else if ((yyvsp[(1) - (2)].arcparallel))
        (yyval.arcparallel) = ((yyvsp[(1) - (2)].arcparallel))->AddArcList((yyvsp[(2) - (2)].arclist));
    else
        (yyval.arcparallel) = (new ArcParallel(&msc))->AddArcList((yyvsp[(2) - (2)].arclist));
  #endif
}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 1111 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.arcemph) = (yyvsp[(1) - (1)].arcemph);
  #endif
}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 1117 "language.yy"
    {
    (yyval.arcemph) = (yyvsp[(1) - (1)].arcemph);
}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 1122 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(2) - (2)].arcemph))->SetLineEnd(MSC_POS((yylsp[(2) - (2)])));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddFollow(((yyvsp[(2) - (2)].arcemph))->ChangeStyleForFollow());
  #endif
}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 1129 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(2) - (3)].arcemph))->ChangeStyleForFollow()->AddAttributeList((yyvsp[(3) - (3)].attriblist));
    ((yyvsp[(2) - (3)].arcemph))->SetLineEnd(MSC_POS2((yylsp[(2) - (3)]), (yylsp[(3) - (3)])));
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph))->AddFollow((yyvsp[(2) - (3)].arcemph));
  #endif
}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 1137 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(2) - (3)].arcemph))->SetLineEnd(MSC_POS((yylsp[(2) - (3)])));
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph))->AddFollow(((yyvsp[(2) - (3)].arcemph))->AddArcList((yyvsp[(3) - (3)].arclist))->ChangeStyleForFollow());
  #endif
}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 1144 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ArcEmphasis *temp = new ArcEmphasis(MSC_EMPH_UNDETERMINED_FOLLOW, NULL, MSC_POS((yylsp[(1) - (2)])), NULL, MSC_POS((yylsp[(1) - (2)])), &msc);
    temp->AddArcList((yyvsp[(2) - (2)].arclist))->ChangeStyleForFollow((yyvsp[(1) - (2)].arcemph));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddFollow(temp);
  #endif
}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 1152 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(2) - (4)].arcemph))->SetLineEnd(MSC_POS2((yylsp[(2) - (4)]), (yylsp[(3) - (4)])));
    ((yyvsp[(2) - (4)].arcemph))->AddArcList((yyvsp[(4) - (4)].arclist))->ChangeStyleForFollow()->AddAttributeList((yyvsp[(3) - (4)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (4)].arcemph))->AddFollow((yyvsp[(2) - (4)].arcemph));
  #endif
}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 1160 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ArcEmphasis *temp = new ArcEmphasis(MSC_EMPH_UNDETERMINED_FOLLOW, NULL, MSC_POS((yylsp[(1) - (3)])), NULL, MSC_POS((yylsp[(1) - (3)])), &msc);
    temp->SetLineEnd(MSC_POS((yylsp[(2) - (3)])));
    temp->AddArcList((yyvsp[(3) - (3)].arclist))->ChangeStyleForFollow((yyvsp[(1) - (3)].arcemph))->AddAttributeList((yyvsp[(2) - (3)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph))->AddFollow(temp);
  #endif
}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 1171 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(1) - (1)].arcemph))->SetLineEnd(MSC_POS((yyloc)));
    (yyval.arcemph) = (yyvsp[(1) - (1)].arcemph);
  #endif
}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 1178 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(1) - (2)].arcemph))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
    ((yyvsp[(1) - (2)].arcemph))->SetLineEnd(MSC_POS((yyloc)));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph));
  #endif
}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 1186 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(1) - (2)].arcemph))->SetLineEnd(MSC_POS((yylsp[(1) - (2)])));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddArcList((yyvsp[(2) - (2)].arclist));
  #endif
}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 1193 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(1) - (3)].arcemph))->SetLineEnd(MSC_POS2((yylsp[(1) - (3)]), (yylsp[(2) - (3)])));
    ((yyvsp[(1) - (3)].arcemph))->AddArcList((yyvsp[(3) - (3)].arclist))->AddAttributeList((yyvsp[(2) - (3)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph));
  #endif
}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 1202 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.arcemph) = ((yyvsp[(1) - (1)].arcemph))->SetPipe();
  #endif
}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 1208 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(1) - (2)].arcemph))->SetPipe()->AddAttributeList((yyvsp[(2) - (2)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph));
  #endif
}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 1216 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
        ((yyvsp[(2) - (2)].arcemph))->SetLineEnd(MSC_POS((yyloc)));
        (yyval.arcemph) = (yyvsp[(2) - (2)].arcemph);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 1226 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(2) - (2)].arcemph))->SetLineEnd(MSC_POS((yylsp[(2) - (2)])));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddFollow((yyvsp[(2) - (2)].arcemph));
  #endif
}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 1233 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_KEYWORD);
  #else
        ((yyvsp[(3) - (3)].arcemph))->SetLineEnd(MSC_POS2((yylsp[(2) - (3)]), (yylsp[(3) - (3)])));
        (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph))->AddFollow((yyvsp[(3) - (3)].arcemph));
  #endif
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 1246 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddArcList((yyvsp[(2) - (2)].arclist));
  #endif
}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 1253 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_EntityName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
        csh.AddCSH_EntityName((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
  #else
        (yyval.arcemph) = new ArcEmphasis((yyvsp[(2) - (3)].arctype), (yyvsp[(1) - (3)].str), MSC_POS((yylsp[(1) - (3)])), (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(3) - (3)])), &msc);
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 1265 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
        csh.AddCSH_EntityName((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
  #else
        (yyval.arcemph) = new ArcEmphasis((yyvsp[(1) - (2)].arctype), NULL, MSC_POS((yylsp[(1) - (2)])), (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])), &msc);
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 1275 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_EntityName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
  #else
        (yyval.arcemph) = new ArcEmphasis((yyvsp[(2) - (2)].arctype), (yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), NULL, MSC_POS((yylsp[(2) - (2)])), &msc);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 1285 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
  #else
        (yyval.arcemph) = new ArcEmphasis((yyvsp[(1) - (1)].arctype), NULL, MSC_POS((yylsp[(1) - (1)])), NULL, MSC_POS((yylsp[(1) - (1)])), &msc);
  #endif
}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 1294 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
        csh.AddCSH_EntityName((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
  #else
        (yyval.vertxpos) = new VertXPos(msc, (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])));
  #endif
    free((yyvsp[(1) - (2)].str));
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 1305 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        csh.AddCSH_EntityName((yylsp[(2) - (3)]), (yyvsp[(2) - (3)].str));
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_SYMBOL);
  #else
        (yyval.vertxpos) = new VertXPos(msc, (yyvsp[(2) - (3)].str), MSC_POS((yylsp[(2) - (3)])), VertXPos::POS_LEFT_SIDE);
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 1317 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        csh.AddCSH_EntityName((yylsp[(2) - (3)]), (yyvsp[(2) - (3)].str));
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_SYMBOL);
  #else
        (yyval.vertxpos) = new VertXPos(msc, (yyvsp[(2) - (3)].str), MSC_POS((yylsp[(2) - (3)])), VertXPos::POS_RIGHT_SIDE);
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 1329 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        csh.AddCSH_EntityName((yylsp[(2) - (3)]), (yyvsp[(2) - (3)].str));
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_SYMBOL);
  #else
        switch ((yyvsp[(3) - (3)].arctype)) {
        case MSC_EMPH_SOLID:
            (yyval.vertxpos) = new VertXPos(msc, (yyvsp[(2) - (3)].str), MSC_POS((yylsp[(2) - (3)])), VertXPos::POS_LEFT_BY);
            break;
        case MSC_EMPH_DASHED:
            (yyval.vertxpos) = new VertXPos(msc, (yyvsp[(2) - (3)].str), MSC_POS((yylsp[(2) - (3)])), VertXPos::POS_RIGHT_BY);
            break;
        case MSC_EMPH_DOTTED:
            msc.Error.Error(MSC_POS((yylsp[(3) - (3)])).start,
                            "unexpected '..', expected '-', '--', '+' or '++'."
                            " Ignoring vertical."); break;
            (yyval.vertxpos) = NULL;
            break;
        case MSC_EMPH_DOUBLE:
            msc.Error.Error(MSC_POS((yylsp[(3) - (3)])).start,
                            "unexpected '==', expected '-', '--', '+' or '++'."
                            " Ignoring vertical."); break;
            (yyval.vertxpos) = NULL;
            break;
        }
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 1360 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (4)]), COLOR_KEYWORD);
        csh.AddCSH_EntityName((yylsp[(2) - (4)]), (yyvsp[(2) - (4)].str));
        csh.AddCSH((yylsp[(3) - (4)]), COLOR_SYMBOL);
        csh.AddCSH_EntityName((yylsp[(4) - (4)]), (yyvsp[(4) - (4)].str));
  #else
        (yyval.vertxpos) = new VertXPos(msc, (yyvsp[(2) - (4)].str), MSC_POS((yylsp[(2) - (4)])), (yyvsp[(4) - (4)].str), MSC_POS((yylsp[(4) - (4)])), VertXPos::POS_CENTER);
  #endif
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(2) - (4)].str));
    free((yyvsp[(4) - (4)].str));
}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 1377 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (4)]), COLOR_MARKERNAME);
        csh.AddCSH((yylsp[(2) - (4)]), COLOR_SYMBOL);
        csh.AddCSH((yylsp[(3) - (4)]), COLOR_MARKERNAME);
  #else
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(2) - (4)].arctype), (yyvsp[(1) - (4)].str), (yyvsp[(3) - (4)].str), (yyvsp[(4) - (4)].vertxpos), &msc);
        delete (yyvsp[(4) - (4)].vertxpos);
  #endif
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(3) - (4)].str));
}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 1390 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_SYMBOL);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_MARKERNAME);
  #else
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(1) - (3)].arctype), MARKER_HERE_STR, (yyvsp[(2) - (3)].str), (yyvsp[(3) - (3)].vertxpos), &msc);
        delete (yyvsp[(3) - (3)].vertxpos);
  #endif
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 1401 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_MARKERNAME);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
  #else
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(2) - (3)].arctype), (yyvsp[(1) - (3)].str), MARKER_HERE_STR, (yyvsp[(3) - (3)].vertxpos), &msc);
        delete (yyvsp[(3) - (3)].vertxpos);
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 1412 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
  #else
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(1) - (2)].arctype), MARKER_HERE_STR, MARKER_PREV_PARALLEL_STR, (yyvsp[(2) - (2)].vertxpos), &msc);
        delete (yyvsp[(2) - (2)].vertxpos);
  #endif
}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 1421 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (4)]), COLOR_MARKERNAME);
        csh.AddCSH((yylsp[(2) - (4)]), COLOR_SYMBOL);
        csh.AddCSH((yylsp[(3) - (4)]), COLOR_MARKERNAME);
  #else
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(2) - (4)].arctype), (yyvsp[(3) - (4)].str), (yyvsp[(1) - (4)].str), (yyvsp[(4) - (4)].vertxpos), &msc);
        delete (yyvsp[(4) - (4)].vertxpos);
  #endif
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(3) - (4)].str));
}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 1434 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_SYMBOL);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_MARKERNAME);
  #else
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(1) - (3)].arctype), (yyvsp[(2) - (3)].str), MARKER_HERE_STR, (yyvsp[(3) - (3)].vertxpos), &msc);
        delete (yyvsp[(3) - (3)].vertxpos);
  #endif
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 1445 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_MARKERNAME);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
  #else
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(2) - (3)].arctype), MARKER_HERE_STR, (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].vertxpos), &msc);
        delete (yyvsp[(3) - (3)].vertxpos);
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 1456 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
  #else
    (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(1) - (2)].arctype), MARKER_PREV_PARALLEL_STR, MARKER_HERE_STR, (yyvsp[(2) - (2)].vertxpos), &msc);
    delete (yyvsp[(2) - (2)].vertxpos);
  #endif
}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 1467 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
  #else
    (yyval.arcbase) = new ArcDivider((yyvsp[(1) - (1)].arctype), &msc);
  #endif
}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 1475 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 1479 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 1483 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 1488 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_EntityName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
        csh.AddCSH_EntityName((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
  #else
        (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (3)].arctype), (yyvsp[(1) - (3)].str), MSC_POS((yylsp[(1) - (3)])), (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(3) - (3)])));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 1500 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
        csh.AddCSH_EntityName((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
  #else
        (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(1) - (2)].arctype), LSIDE_ENT_STR, MSC_POS((yylsp[(1) - (2)])), (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])));
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 1510 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_EntityName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
  #else
        (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (2)].arctype), (yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), RSIDE_ENT_STR, MSC_POS((yylsp[(2) - (2)])));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 1520 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
        csh.AddCSH_EntityName((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
  #else
        (yyval.arcarrow) = ((yyvsp[(1) - (3)].arcarrow))->AddSegment((yyvsp[(3) - (3)].str), MSC_POS((yylsp[(3) - (3)])), true, MSC_POS2((yylsp[(2) - (3)]), (yylsp[(3) - (3)])));
  #endif
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 1530 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment(NULL, MSC_POS((yylsp[(2) - (2)])), true, MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 1540 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_EntityName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
        csh.AddCSH_EntityName((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
  #else
        (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (3)].arctype), (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(3) - (3)])), (yyvsp[(1) - (3)].str), MSC_POS((yylsp[(1) - (3)])));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 1552 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
        csh.AddCSH_EntityName((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
  #else
        (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(1) - (2)].arctype), (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])), LSIDE_ENT_STR, MSC_POS((yylsp[(1) - (2)])));
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 1562 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_EntityName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
  #else
        (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (2)].arctype), RSIDE_ENT_STR, MSC_POS((yylsp[(2) - (2)])), (yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 1572 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
        csh.AddCSH_EntityName((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
  #else
        (yyval.arcarrow) = ((yyvsp[(1) - (3)].arcarrow))->AddSegment((yyvsp[(3) - (3)].str), MSC_POS((yylsp[(3) - (3)])), false, MSC_POS2((yylsp[(2) - (3)]), (yylsp[(3) - (3)])));
  #endif
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 1582 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment(NULL, MSC_POS((yylsp[(2) - (2)])), false, MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 1591 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_EntityName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
        csh.AddCSH_EntityName((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
  #else
        (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (3)].arctype), (yyvsp[(1) - (3)].str), MSC_POS((yylsp[(1) - (3)])), (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(3) - (3)])));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 1603 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
        csh.AddCSH_EntityName((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
  #else
        (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(1) - (2)].arctype), LSIDE_ENT_STR, MSC_POS((yylsp[(1) - (2)])), (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])));
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 1613 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_EntityName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
  #else
        (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (2)].arctype), (yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), RSIDE_ENT_STR, MSC_POS((yylsp[(2) - (2)])));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 1623 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
        csh.AddCSH_EntityName((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
  #else
        (yyval.arcarrow) = ((yyvsp[(1) - (3)].arcarrow))->AddSegment((yyvsp[(3) - (3)].str), MSC_POS((yylsp[(3) - (3)])), true, MSC_POS2((yylsp[(2) - (3)]), (yylsp[(3) - (3)])));
  #endif
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 1633 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment(NULL, MSC_POS((yylsp[(2) - (2)])), true, MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 1651 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_ColonString((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str), false);
  #endif
    (yyval.str) = (yyvsp[(1) - (1)].str);
}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 1658 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_ColonString((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str), true);
  #endif
    (yyval.str) = (yyvsp[(1) - (1)].str);
}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1666 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = (new AttributeList)->Append(new Attribute("label", (yyvsp[(1) - (1)].str), MSC_POS((yyloc)), MSC_POS((yyloc)).IncStartCol()));
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1674 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = ((yyvsp[(2) - (2)].attriblist))->Prepend(new Attribute("label", (yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), MSC_POS((yylsp[(1) - (2)])).IncStartCol()));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1682 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = ((yyvsp[(1) - (2)].attriblist))->Append(new Attribute("label", (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])), MSC_POS((yylsp[(2) - (2)])).IncStartCol()));
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1690 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        ((yyvsp[(1) - (3)].attriblist))->Append(new Attribute("label", (yyvsp[(2) - (3)].str), MSC_POS((yylsp[(2) - (3)])), MSC_POS((yylsp[(2) - (3)])).IncStartCol()));
        //Merge $3 at the end of $1
        ((yyvsp[(1) - (3)].attriblist))->splice(((yyvsp[(1) - (3)].attriblist))->end(), *((yyvsp[(3) - (3)].attriblist)));
        delete ((yyvsp[(3) - (3)].attriblist)); //empty list now
        (yyval.attriblist) = (yyvsp[(1) - (3)].attriblist);
  #endif
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1705 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_BRACKET);
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_BRACKET);
        if (csh.CursorIn(((yylsp[(1) - (2)])).last_pos,((yylsp[(2) - (2)])).first_pos))
            csh.hintStatus = HINT_ATTR_NAME;
  #else
    (yyval.attriblist) = new AttributeList;
  #endif
}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1716 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_BRACKET);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_BRACKET);
        if (csh.CursorIn(((yylsp[(1) - (3)])).last_pos,((yylsp[(2) - (3)])).first_pos))
            csh.hintStatus = HINT_ATTR_NAME;
        if (csh.hintStatus == HINT_ATTR_VALUE_CAN_COME) {
            if (csh.CursorIn(((yylsp[(2) - (3)])).last_pos,((yylsp[(3) - (3)])).first_pos))
                csh.hintStatus = HINT_ATTR_VALUE;
            else {
                csh.hintStatus = HINT_NONE;
                csh.hintAttrName.clear();
            }
        }
  #else
    (yyval.attriblist) = (yyvsp[(2) - (3)].attriblist);
  #endif
}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1735 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (4)]), COLOR_BRACKET);
        csh.AddCSH((yylsp[(3) - (4)]), COLOR_ERROR);
        csh.AddCSH((yylsp[(4) - (4)]), COLOR_BRACKET);
        if (csh.CursorIn(((yylsp[(1) - (4)])).last_pos,((yylsp[(2) - (4)])).first_pos))
            csh.hintStatus = HINT_ATTR_NAME;
        if (csh.hintStatus == HINT_ATTR_VALUE_CAN_COME) {
            if (csh.CursorIn(((yylsp[(2) - (4)])).last_pos,((yylsp[(4) - (4)])).first_pos))
                csh.hintStatus = HINT_ATTR_VALUE;
            else {
                csh.hintStatus = HINT_NONE;
                csh.hintAttrName.clear();
            }
        }
  #else
    (yyval.attriblist) = (yyvsp[(2) - (4)].attriblist);
  #endif
}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1755 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_BRACKET);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_ERROR);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_BRACKET);
        if (csh.CursorIn(((yylsp[(1) - (3)])).last_pos,((yylsp[(3) - (3)])).first_pos))
            csh.hintStatus = HINT_ATTR_NAME;
  #else
    (yyval.attriblist) = new AttributeList;
  #endif
}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1767 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_BRACKET);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_ERROR);
        if (csh.CursorIn(((yylsp[(1) - (3)])).last_pos,((yylsp[(2) - (3)])).first_pos))
            csh.hintStatus = HINT_ATTR_NAME;
        if (csh.hintStatus == HINT_ATTR_VALUE_CAN_COME) {
            if (csh.CursorIn(((yylsp[(2) - (3)])).last_pos,((yylsp[(3) - (3)])).last_pos))
                csh.hintStatus = HINT_ATTR_VALUE;
            else {
                csh.hintStatus = HINT_NONE;
                csh.hintAttrName.clear();
            }
        }
  #else
    (yyval.attriblist) = (yyvsp[(2) - (3)].attriblist);
  #endif
}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1786 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_BRACKET);
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_ERROR);
        if (csh.CursorIn(((yylsp[(1) - (2)])).last_pos,((yylsp[(2) - (2)])).first_pos))
            csh.hintStatus = HINT_ATTR_NAME;
  #else
    (yyval.attriblist) = new AttributeList;
  #endif
}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1798 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.attriblist) = (new AttributeList)->Append((yyvsp[(1) - (1)].attrib));
  #endif
}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1805 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
        if (csh.CursorIn(((yylsp[(2) - (3)])).last_pos,((yylsp[(3) - (3)])).first_pos))
            csh.hintStatus = HINT_ATTR_NAME;
        if (csh.hintStatus == HINT_ATTR_VALUE_CAN_COME) {
            if (csh.CursorIn(((yylsp[(1) - (3)])).last_pos,((yylsp[(2) - (3)])).first_pos))
                csh.hintStatus = HINT_ATTR_VALUE;
            else {
                csh.hintStatus = HINT_ATTR_VALUE;
                csh.hintAttrName.clear();
            }
        }
  #else
    (yyval.attriblist) = ((yyvsp[(1) - (3)].attriblist))->Append((yyvsp[(3) - (3)].attrib));
  #endif
}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1824 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_ATTRNAME);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        csh.AddCSH_AttrValue((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str), (yyvsp[(1) - (3)].str));
        if (csh.CursorIn((yylsp[(1) - (3)]))) {
            csh.hintStatus = HINT_ATTR_NAME;
            csh.hintedStringPos = (yylsp[(1) - (3)]);
        }
        if (csh.CursorIn(((yylsp[(2) - (3)])).last_pos, ((yylsp[(3) - (3)])).last_pos)) {
            csh.hintStatus = HINT_ATTR_VALUE;
            csh.hintedStringPos = (yylsp[(3) - (3)]);
            csh.hintAttrName = (yyvsp[(1) - (3)].str);
        }
  #else
        (yyval.attrib) = new Attribute((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(1) - (3)])), MSC_POS((yylsp[(3) - (3)])));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1845 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_ATTRNAME);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_ATTRVALUE);
        if (csh.CursorIn((yylsp[(1) - (3)]))) {
            csh.hintStatus = HINT_ATTR_NAME;
            csh.hintedStringPos = (yylsp[(1) - (3)]);
        }
        if (csh.CursorIn(((yylsp[(2) - (3)])).last_pos, ((yylsp[(3) - (3)])).last_pos)) {
            csh.hintStatus = HINT_ATTR_VALUE;
            csh.hintedStringPos = (yylsp[(3) - (3)]);
            csh.hintAttrName = (yyvsp[(1) - (3)].str);
        }
  #else
        (yyval.attrib) = new Attribute((yyvsp[(1) - (3)].str), atof((yyvsp[(3) - (3)].str)), MSC_POS((yyloc)), MSC_POS((yylsp[(3) - (3)])), (yyvsp[(3) - (3)].str));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1866 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_ATTRNAME);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_ATTRVALUE);
        if (csh.CursorIn((yylsp[(1) - (3)]))) {
            csh.hintStatus = HINT_ATTR_NAME;
            csh.hintedStringPos = (yylsp[(1) - (3)]);
        }
        if (csh.CursorIn(((yylsp[(2) - (3)])).last_pos, ((yylsp[(3) - (3)])).last_pos)) {
            csh.hintStatus = HINT_ATTR_VALUE;
            csh.hintedStringPos = (yylsp[(3) - (3)]);
            csh.hintAttrName = (yyvsp[(1) - (3)].str);
        }
  #else
        (yyval.attrib) = new Attribute((yyvsp[(1) - (3)].str), str2bool((yyvsp[(3) - (3)].str)), MSC_POS((yyloc)), MSC_POS((yylsp[(3) - (3)])), (yyvsp[(3) - (3)].str));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1887 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str), COLOR_ATTRNAME);
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_EQUAL);
        if (csh.CursorIn((yylsp[(1) - (2)]))) {
            csh.hintStatus = HINT_ATTR_NAME;
            csh.hintedStringPos = (yylsp[(1) - (2)]);
        } else {
            csh.hintStatus = HINT_ATTR_VALUE_CAN_COME;
            csh.hintAttrName = (yyvsp[(1) - (2)].str);
        }
  #else
        (yyval.attrib) = new Attribute((yyvsp[(1) - (2)].str), (char*)NULL, MSC_POS((yyloc)), MSC_POS((yyloc)));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1904 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_StyleOrAttrName((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str));
        if (csh.CursorIn((yylsp[(1) - (1)]))) {
            csh.hintStatus = HINT_ATTR_NAME;
            csh.hintedStringPos = (yylsp[(1) - (1)]);
        }
  #else
        (yyval.attrib) = new Attribute((yyvsp[(1) - (1)].str), MSC_POS((yyloc)));
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1927 "language.yy"
    {(yyval.str) = strdup("->");}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1928 "language.yy"
    {(yyval.str) = strdup("<-");}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1929 "language.yy"
    {(yyval.str) = strdup("<->");}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1930 "language.yy"
    {(yyval.str) = strdup("=>");}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1931 "language.yy"
    {(yyval.str) = strdup("<=");}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1932 "language.yy"
    {(yyval.str) = strdup("<=>");}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1933 "language.yy"
    {(yyval.str) = strdup(">>");}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1934 "language.yy"
    {(yyval.str) = strdup("<<");}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1935 "language.yy"
    {(yyval.str) = strdup("<<>>");}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1936 "language.yy"
    {(yyval.str) = strdup(">");}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1937 "language.yy"
    {(yyval.str) = strdup("<");}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1938 "language.yy"
    {(yyval.str) = strdup("<>");}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1940 "language.yy"
    {
    switch ((yyvsp[(1) - (1)].arctype)) {
    case MSC_ARC_DIVIDER:  (yyval.str) = strdup("---"); break;
    case MSC_ARC_DISCO:    (yyval.str) = strdup("..."); break;
    }
}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1947 "language.yy"
    {
    switch ((yyvsp[(1) - (1)].arctype)) {
    case MSC_EMPH_SOLID:  (yyval.str) = strdup("--"); break;
    case MSC_EMPH_DASHED: (yyval.str) = strdup("++"); break;
    case MSC_EMPH_DOTTED: (yyval.str) = strdup(".."); break;
    case MSC_EMPH_DOUBLE: (yyval.str) = strdup("=="); break;
    }
}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1959 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.PushContext();
  #else
    msc.PushContext();
  #endif
}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1968 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    (yyval.arcbase) = NULL;
    csh.PopContext();
  #else
    (yyval.arcbase) = msc.PopContext();
  #endif
}
    break;



/* Line 1455 of yacc.c  */
#line 5062 "colorsyntax.cc"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, RESULT, yyscanner, YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (&yylloc, RESULT, yyscanner, yymsg);
	  }
	else
	  {
	    yyerror (&yylloc, RESULT, yyscanner, YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }

  yyerror_range[0] = yylloc;

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

  yyerror_range[0] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
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

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp, RESULT, yyscanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
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

#if !defined(yyoverflow) || YYERROR_VERBOSE
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
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, &yylloc, RESULT, yyscanner);
  /* Do not reclaim the symbols of the rule which action triggered
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 1978 "language.yy"



/* END OF FILE */

