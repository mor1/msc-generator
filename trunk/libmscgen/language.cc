/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
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
#define YYBISON_VERSION "2.5"

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



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 7 "language.yy"

/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011,2012 Zoltan Turanyi
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
#include "arcs.h"//For AttrNames and AttrValues
#include "msc.h" //For AttrNames and AttrValues
//redefine default loc action for CSH
#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#define YYLLOC_DEFAULT(Current, Rhs, N)                       \
    do                                                        \
        if (YYID (N)) {                                       \
            (Current).first_pos = YYRHSLOC(Rhs,1).first_pos;  \
	        (Current).last_pos  = YYRHSLOC(Rhs,N).last_pos;	  \
        } else {                                              \
	        (Current).first_pos = (Current).last_pos =        \
	            YYRHSLOC (Rhs, 0).last_pos;                   \
        }                                                     \
    while (YYID (0))

/* yyerror
 *  Error handling function.  Do nothing for CSH
 */
void yyerror(YYLTYPE* /*loc*/, Csh & /*csh*/, void * /*yyscanner*/, const char * /*str*/)
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



/* Line 268 of yacc.c  */
#line 183 "language.cc"

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

/* Line 293 of yacc.c  */
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
    CHAR_IF_CSH(MscArcType)        arctype;
    CHAR_IF_CSH(EntityDef)        *entity;
    CHAR_IF_CSH(EntityDefList)    *entitylist;
    CHAR_IF_CSH(Attribute)        *attrib;
    CHAR_IF_CSH(AttributeList)    *attriblist;
    CHAR_IF_CSH(VertXPos)         *vertxpos;
    CHAR_IF_CSH(ExtVertXPos)      *extvertxpos;
    CHAR_IF_CSH(NamePair)         *namerel;
    std::list<std::string>        *stringlist;



/* Line 293 of yacc.c  */
#line 364 "language.cc"
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


/* Line 343 of yacc.c  */
#line 389 "language.cc"

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
# if defined YYENABLE_NLS && YYENABLE_NLS
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
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
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
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

# define YYCOPY_NEEDED 1

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

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
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
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  179
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1248

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  61
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  71
/* YYNRULES -- Number of rules.  */
#define YYNRULES  328
/* YYNRULES -- Number of states.  */
#define YYNSTATES  368

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   315

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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,     9,    12,    15,    17,    20,
      22,    25,    27,    30,    34,    37,    41,    45,    48,    53,
      57,    60,    62,    65,    67,    69,    72,    75,    79,    84,
      86,    90,    92,    95,    97,   100,   102,   105,   109,   111,
     114,   118,   120,   122,   124,   127,   130,   134,   138,   143,
     145,   147,   149,   151,   154,   157,   160,   162,   165,   167,
     170,   173,   176,   180,   182,   185,   187,   189,   191,   195,
     198,   200,   203,   205,   207,   209,   211,   213,   216,   219,
     222,   224,   228,   230,   233,   236,   238,   242,   244,   246,
     248,   250,   252,   256,   259,   263,   267,   271,   275,   279,
     282,   286,   289,   293,   296,   298,   302,   305,   309,   312,
     314,   318,   321,   324,   326,   330,   333,   335,   339,   342,
     344,   348,   350,   354,   358,   364,   371,   373,   375,   379,
     382,   385,   387,   389,   393,   396,   400,   404,   408,   412,
     416,   418,   421,   423,   426,   430,   434,   437,   442,   446,
     448,   451,   454,   458,   461,   464,   466,   470,   472,   475,
     479,   481,   484,   488,   491,   494,   496,   499,   503,   507,
     512,   516,   521,   525,   530,   536,   538,   540,   542,   544,
     546,   550,   553,   556,   558,   562,   565,   568,   570,   572,
     576,   581,   585,   590,   593,   595,   597,   599,   601,   603,
     607,   610,   613,   617,   620,   624,   627,   630,   634,   637,
     641,   644,   647,   651,   654,   656,   658,   660,   662,   664,
     666,   668,   670,   672,   674,   676,   678,   680,   682,   684,
     686,   688,   690,   692,   694,   697,   699,   701,   703,   708,
     714,   718,   723,   727,   731,   734,   736,   738,   741,   746,
     749,   752,   756,   759,   761,   763,   765,   767,   770,   774,
     777,   779,   782,   786,   791,   795,   798,   802,   804,   807,
     810,   812,   816,   819,   823,   827,   831,   835,   838,   842,
     844,   846,   848,   850,   852,   854,   856,   858,   860,   862,
     864,   866,   868,   870,   872,   874,   876,   878,   880,   882,
     884,   886,   888,   890,   892,   894,   896,   898,   900,   902,
     904,   906,   908,   910,   912,   914,   916,   918,   920,   922,
     924,   926,   928,   930,   932,   934,   936,   938,   941
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      62,     0,    -1,    63,    -1,    63,    52,    -1,    -1,    16,
      66,    -1,    65,    66,    -1,    16,    -1,    16,     1,    -1,
      64,    -1,    64,     1,    -1,    67,    -1,    67,    13,    -1,
      67,    13,    64,    -1,    16,     7,    -1,    16,     7,   129,
      -1,   130,    67,   131,    -1,   130,   131,    -1,   130,    67,
       1,   131,    -1,   130,    67,     1,    -1,   130,    67,    -1,
      68,    -1,    68,    70,    -1,    70,    -1,    69,    -1,    68,
      69,    -1,    70,     9,    -1,    70,     9,    60,    -1,    70,
       1,     9,    60,    -1,     9,    -1,    70,     1,     9,    -1,
      71,    -1,    44,    71,    -1,   104,    -1,   104,   122,    -1,
      41,    -1,    41,   104,    -1,    41,   104,   122,    -1,    45,
      -1,    45,   103,    -1,    45,   103,   122,    -1,   122,    -1,
      81,    -1,    76,    -1,    76,    81,    -1,    81,     8,    -1,
      81,     8,    79,    -1,    76,    81,     8,    -1,    76,    81,
       8,    79,    -1,    77,    -1,    94,    -1,    98,    -1,    93,
      -1,    38,    85,    -1,    39,    82,    -1,    40,    87,    -1,
      35,    -1,    35,   123,    -1,    36,    -1,    36,   123,    -1,
      72,   122,    -1,    43,   129,    -1,    43,   129,   123,    -1,
      37,    -1,    37,   123,    -1,   118,    -1,   119,    -1,   120,
      -1,    54,    74,    73,    -1,    54,    74,    -1,    54,    -1,
      53,    73,    -1,    53,    -1,    58,    -1,    59,    -1,   122,
      -1,     5,    -1,     5,   122,    -1,   126,     6,    -1,     6,
     126,    -1,     6,    -1,   126,     6,   126,    -1,   126,    -1,
     126,     6,    -1,     6,   126,    -1,     6,    -1,   126,     6,
     126,    -1,    49,    -1,    48,    -1,    50,    -1,    51,    -1,
      78,    -1,    77,     8,    78,    -1,    77,     8,    -1,    77,
       8,    60,    -1,    77,     8,     1,    -1,   126,     7,    20,
      -1,   126,     7,     5,    -1,   126,     7,   129,    -1,   126,
       7,    -1,    16,     7,   129,    -1,    16,     7,    -1,    16,
      11,   129,    -1,    16,    11,    -1,    80,    -1,    79,     8,
      80,    -1,    79,     8,    -1,    79,     8,    60,    -1,   126,
     122,    -1,   126,    -1,   126,   122,    66,    -1,   126,    66,
      -1,   126,   122,    -1,   126,    -1,   126,   122,    66,    -1,
     126,    66,    -1,    83,    -1,    82,     8,    83,    -1,    84,
     123,    -1,   129,    -1,    84,     8,   129,    -1,    86,    -1,
      85,     8,    86,    -1,     3,     7,   129,    -1,     3,    88,
      89,     9,    13,    -1,     3,    88,    89,     9,     1,    13,
      -1,    12,    -1,    90,    -1,    89,     9,    90,    -1,    38,
      85,    -1,    39,    82,    -1,    91,    -1,    92,    -1,    91,
       8,    92,    -1,    91,     1,    -1,   126,     7,    20,    -1,
     126,     7,     5,    -1,   126,     7,   129,    -1,    16,     7,
     129,    -1,    16,    11,   129,    -1,    66,    -1,    93,    66,
      -1,    95,    -1,    94,    99,    -1,    94,    99,   122,    -1,
      94,    99,    66,    -1,    94,    66,    -1,    94,    99,   122,
      66,    -1,    94,   122,    66,    -1,    99,    -1,    99,   122,
      -1,    99,    66,    -1,    99,   122,    66,    -1,    42,    99,
      -1,    42,     1,    -1,    42,    -1,    42,    99,   122,    -1,
      96,    -1,    97,    99,    -1,    97,    99,   122,    -1,    97,
      -1,    97,    66,    -1,   126,    34,   126,    -1,    34,   126,
      -1,   126,    34,    -1,    34,    -1,    46,   126,    -1,    46,
     126,     5,    -1,    46,   126,     6,    -1,    46,   126,     6,
       5,    -1,    46,   126,    10,    -1,    46,   126,    10,     5,
      -1,    46,   126,    34,    -1,    46,   126,     6,   126,    -1,
      46,   126,     6,   126,     5,    -1,    46,    -1,   126,    -1,
      34,    -1,   108,    -1,   110,    -1,   126,   101,   126,    -1,
     101,   126,    -1,   126,   101,    -1,   101,    -1,   126,   109,
     126,    -1,   109,   126,    -1,   126,   109,    -1,   109,    -1,
     126,    -1,   101,   126,    60,    -1,   126,   101,   126,    60,
      -1,   109,   126,    60,    -1,   126,   109,   126,    60,    -1,
     102,   100,    -1,   102,    -1,    33,    -1,   105,    -1,   106,
      -1,   107,    -1,   126,   108,   126,    -1,   108,   126,    -1,
     126,   108,    -1,   105,   111,   126,    -1,   105,   111,    -1,
     126,   109,   126,    -1,   109,   126,    -1,   126,   109,    -1,
     106,   112,   126,    -1,   106,   112,    -1,   126,   110,   126,
      -1,   110,   126,    -1,   126,   110,    -1,   107,   113,   126,
      -1,   107,   113,    -1,    21,    -1,    24,    -1,    27,    -1,
      30,    -1,    22,    -1,    25,    -1,    28,    -1,    31,    -1,
      23,    -1,    26,    -1,    29,    -1,    32,    -1,   108,    -1,
       6,    -1,   109,    -1,     6,    -1,   110,    -1,     6,    -1,
     115,    -1,   126,    -1,    47,   100,    -1,    47,    -1,   126,
      -1,   128,    -1,    55,   116,    75,   114,    -1,    55,   116,
      75,   114,   114,    -1,    55,   116,   115,    -1,    55,   116,
     115,   114,    -1,    55,   116,    75,    -1,    55,   116,   126,
      -1,    55,   116,    -1,    55,    -1,   117,    -1,   117,   122,
      -1,    56,    46,   129,   122,    -1,    56,   122,    -1,    56,
      46,    -1,    56,    46,   122,    -1,    57,   122,    -1,    57,
      -1,    18,    -1,    17,    -1,   121,    -1,   121,   123,    -1,
     123,   121,   123,    -1,   123,   121,    -1,   123,    -1,    14,
      15,    -1,    14,   124,    15,    -1,    14,   124,     1,    15,
      -1,    14,     1,    15,    -1,    14,   124,    -1,    14,   124,
       1,    -1,    14,    -1,    14,    60,    -1,    14,     1,    -1,
     125,    -1,   124,     8,   125,    -1,   124,     8,    -1,   124,
       8,    60,    -1,   129,     7,   129,    -1,   129,     7,     5,
      -1,   129,     7,    20,    -1,   129,     7,    -1,   129,     7,
      60,    -1,   129,    -1,     4,    -1,     3,    -1,    16,    -1,
      38,    -1,    39,    -1,    40,    -1,    37,    -1,    41,    -1,
      42,    -1,    45,    -1,    44,    -1,    35,    -1,    36,    -1,
      43,    -1,    46,    -1,    47,    -1,    48,    -1,    49,    -1,
      50,    -1,    51,    -1,    52,    -1,    54,    -1,    53,    -1,
      55,    -1,    56,    -1,    57,    -1,    58,    -1,    59,    -1,
      21,    -1,    22,    -1,    23,    -1,    24,    -1,    25,    -1,
      26,    -1,    27,    -1,    28,    -1,    29,    -1,    30,    -1,
      31,    -1,    32,    -1,    33,    -1,    34,    -1,   126,    -1,
     127,    -1,   128,    -1,    19,    -1,    12,    -1,    12,    60,
      -1,    13,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   221,   221,   222,   233,   243,   254,   264,   274,   284,
     291,   304,   305,   314,   328,   340,   357,   370,   383,   399,
     411,   424,   425,   439,   455,   465,   476,   490,   491,   492,
     504,   519,   527,   550,   557,   568,   580,   595,   613,   625,
     635,   649,   660,   667,   679,   691,   703,   714,   730,   745,
     757,   764,   771,   778,   787,   796,   805,   814,   827,   836,
     849,   865,   877,   893,   902,   915,   916,   917,   918,   931,
     946,   957,   970,   986,   986,   988,   989,  1000,  1011,  1023,
    1034,  1043,  1057,  1082,  1094,  1105,  1115,  1130,  1130,  1130,
    1130,  1132,  1142,  1161,  1174,  1175,  1186,  1205,  1224,  1243,
    1263,  1282,  1302,  1321,  1342,  1348,  1358,  1370,  1373,  1389,
    1401,  1417,  1430,  1448,  1463,  1481,  1497,  1498,  1505,  1554,
    1570,  1588,  1589,  1596,  1612,  1629,  1649,  1659,  1660,  1667,
    1674,  1681,  1683,  1684,  1690,  1697,  1716,  1735,  1754,  1776,
    1800,  1809,  1821,  1829,  1838,  1852,  1861,  1871,  1885,  1902,
    1910,  1923,  1931,  1946,  1961,  1977,  1988,  2007,  2014,  2023,
    2038,  2039,  2046,  2061,  2073,  2086,  2097,  2109,  2123,  2137,
    2152,  2165,  2180,  2214,  2230,  2248,  2260,  2276,  2276,  2276,
    2278,  2292,  2303,  2315,  2324,  2338,  2349,  2361,  2370,  2383,
    2384,  2385,  2386,  2389,  2400,  2411,  2419,  2423,  2427,  2432,
    2446,  2457,  2469,  2480,  2491,  2505,  2516,  2528,  2539,  2549,
    2563,  2574,  2586,  2597,  2607,  2607,  2607,  2607,  2608,  2608,
    2608,  2608,  2609,  2609,  2609,  2609,  2611,  2611,  2612,  2612,
    2613,  2613,  2615,  2616,  2639,  2649,  2663,  2663,  2665,  2701,
    2724,  2760,  2784,  2820,  2857,  2893,  2910,  2911,  2923,  2942,
    2957,  2972,  2993,  3008,  3021,  3028,  3036,  3044,  3052,  3064,
    3072,  3075,  3085,  3095,  3106,  3117,  3128,  3139,  3152,  3156,
    3168,  3175,  3184,  3194,  3196,  3210,  3224,  3238,  3252,  3257,
    3269,  3269,  3271,  3271,  3272,  3272,  3273,  3273,  3273,  3274,
    3274,  3275,  3275,  3276,  3276,  3276,  3276,  3276,  3277,  3277,
    3277,  3278,  3278,  3278,  3279,  3279,  3280,  3280,  3282,  3283,
    3284,  3285,  3286,  3287,  3288,  3289,  3290,  3291,  3292,  3293,
    3294,  3302,  3313,  3313,  3313,  3313,  3315,  3329,  3331
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOK_STRING", "TOK_QSTRING",
  "TOK_NUMBER", "TOK_DASH", "TOK_EQUAL", "TOK_COMMA", "TOK_SEMICOLON",
  "TOK_PLUS", "TOK_PLUS_EQUAL", "TOK_OCBRACKET", "TOK_CCBRACKET",
  "TOK_OSBRACKET", "TOK_CSBRACKET", "TOK_MSC", "TOK_COLON_STRING",
  "TOK_COLON_QUOTED_STRING", "TOK_STYLE_NAME", "TOK_BOOLEAN",
  "TOK_REL_SOLID_TO", "TOK_REL_SOLID_FROM", "TOK_REL_SOLID_BIDIR",
  "TOK_REL_DOUBLE_TO", "TOK_REL_DOUBLE_FROM", "TOK_REL_DOUBLE_BIDIR",
  "TOK_REL_DASHED_TO", "TOK_REL_DASHED_FROM", "TOK_REL_DASHED_BIDIR",
  "TOK_REL_DOTTED_TO", "TOK_REL_DOTTED_FROM", "TOK_REL_DOTTED_BIDIR",
  "TOK_SPECIAL_ARC", "TOK_EMPH", "TOK_COMMAND_HEADING",
  "TOK_COMMAND_NUDGE", "TOK_COMMAND_NEWPAGE", "TOK_COMMAND_DEFCOLOR",
  "TOK_COMMAND_DEFSTYLE", "TOK_COMMAND_DEFDESIGN", "TOK_COMMAND_BIG",
  "TOK_COMMAND_PIPE", "TOK_COMMAND_MARK", "TOK_COMMAND_PARALLEL",
  "TOK_VERTICAL", "TOK_AT", "TOK_AT_POS", "TOK_SHOW", "TOK_HIDE",
  "TOK_ACTIVATE", "TOK_DEACTIVATE", "TOK_BYE", "TOK_COMMAND_VSPACE",
  "TOK_COMMAND_HSPACE", "TOK_COMMAND_SYMBOL", "TOK_COMMAND_NOTE",
  "TOK_COMMAND_COMMENT", "TOK_COMMAND_TITLE", "TOK_COMMAND_SUBTITLE",
  "TOK__NEVER__HAPPENS", "$accept", "msc_with_bye", "msc",
  "top_level_arclist", "msckey", "braced_arclist",
  "arclist_maybe_no_semicolon", "arclist", "arc_with_parallel_semicolon",
  "arc_with_parallel", "arc", "titlecommandtoken",
  "full_arcattrlist_with_label_or_number", "entityrel",
  "markerrel_no_string", "entity_command_prefixes", "optlist", "opt",
  "entitylist", "entity", "first_entity", "styledeflist", "styledef",
  "tok_stringlist", "colordeflist", "colordef", "designdef",
  "scope_open_empty", "designelementlist", "designelement",
  "designoptlist", "designopt", "parallel", "box_list", "first_box",
  "first_pipe", "pipe_list_no_content", "pipe_list", "boxrel", "vertxpos",
  "empharcrel_straight", "vertrel_no_xpos", "vertrel", "arcrel",
  "arcrel_to", "arcrel_from", "arcrel_bidir", "relation_to",
  "relation_from", "relation_bidir", "relation_to_cont",
  "relation_from_cont", "relation_bidir_cont", "extvertxpos",
  "extvertxpos_no_string", "symbol_type_string", "symbol_command_no_attr",
  "symbol_command", "note", "comment", "colon_string",
  "full_arcattrlist_with_label", "full_arcattrlist", "arcattrlist",
  "arcattr", "entity_string", "reserved_word_string", "symbol_string",
  "string", "scope_open", "scope_close", 0
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
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    61,    62,    62,    63,    63,    63,    63,    63,    63,
      63,    64,    64,    64,    65,    65,    66,    66,    66,    66,
      66,    67,    67,    67,    68,    68,    69,    69,    69,    69,
      69,    70,    70,    71,    71,    71,    71,    71,    71,    71,
      71,    71,    71,    71,    71,    71,    71,    71,    71,    71,
      71,    71,    71,    71,    71,    71,    71,    71,    71,    71,
      71,    71,    71,    71,    71,    71,    71,    71,    71,    71,
      71,    71,    71,    72,    72,    73,    73,    73,    74,    74,
      74,    74,    74,    75,    75,    75,    75,    76,    76,    76,
      76,    77,    77,    77,    77,    77,    78,    78,    78,    78,
      78,    78,    78,    78,    79,    79,    79,    79,    80,    80,
      80,    80,    81,    81,    81,    81,    82,    82,    83,    84,
      84,    85,    85,    86,    87,    87,    88,    89,    89,    90,
      90,    90,    91,    91,    91,    92,    92,    92,    92,    92,
      93,    93,    94,    94,    94,    94,    94,    94,    94,    95,
      95,    95,    95,    96,    96,    96,    96,    97,    97,    97,
      98,    98,    99,    99,    99,    99,   100,   100,   100,   100,
     100,   100,   100,   100,   100,   100,   100,   101,   101,   101,
     102,   102,   102,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   103,   103,   104,   104,   104,   104,   105,
     105,   105,   105,   105,   106,   106,   106,   106,   106,   107,
     107,   107,   107,   107,   108,   108,   108,   108,   109,   109,
     109,   109,   110,   110,   110,   110,   111,   111,   112,   112,
     113,   113,   114,   114,   115,   115,   116,   116,   117,   117,
     117,   117,   117,   117,   117,   117,   118,   118,   119,   119,
     119,   119,   120,   120,   121,   121,   122,   122,   122,   122,
     122,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     124,   124,   124,   124,   125,   125,   125,   125,   125,   125,
     126,   126,   127,   127,   127,   127,   127,   127,   127,   127,
     127,   127,   127,   127,   127,   127,   127,   127,   127,   127,
     127,   127,   127,   127,   127,   127,   127,   127,   128,   128,
     128,   128,   128,   128,   128,   128,   128,   128,   128,   128,
     128,   128,   129,   129,   129,   129,   130,   130,   131
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     0,     2,     2,     1,     2,     1,
       2,     1,     2,     3,     2,     3,     3,     2,     4,     3,
       2,     1,     2,     1,     1,     2,     2,     3,     4,     1,
       3,     1,     2,     1,     2,     1,     2,     3,     1,     2,
       3,     1,     1,     1,     2,     2,     3,     3,     4,     1,
       1,     1,     1,     2,     2,     2,     1,     2,     1,     2,
       2,     2,     3,     1,     2,     1,     1,     1,     3,     2,
       1,     2,     1,     1,     1,     1,     1,     2,     2,     2,
       1,     3,     1,     2,     2,     1,     3,     1,     1,     1,
       1,     1,     3,     2,     3,     3,     3,     3,     3,     2,
       3,     2,     3,     2,     1,     3,     2,     3,     2,     1,
       3,     2,     2,     1,     3,     2,     1,     3,     2,     1,
       3,     1,     3,     3,     5,     6,     1,     1,     3,     2,
       2,     1,     1,     3,     2,     3,     3,     3,     3,     3,
       1,     2,     1,     2,     3,     3,     2,     4,     3,     1,
       2,     2,     3,     2,     2,     1,     3,     1,     2,     3,
       1,     2,     3,     2,     2,     1,     2,     3,     3,     4,
       3,     4,     3,     4,     5,     1,     1,     1,     1,     1,
       3,     2,     2,     1,     3,     2,     2,     1,     1,     3,
       4,     3,     4,     2,     1,     1,     1,     1,     1,     3,
       2,     2,     3,     2,     3,     2,     2,     3,     2,     3,
       2,     2,     3,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     1,     1,     4,     5,
       3,     4,     3,     3,     2,     1,     1,     2,     4,     2,
       2,     3,     2,     1,     1,     1,     1,     2,     3,     2,
       1,     2,     3,     4,     3,     2,     3,     1,     2,     2,
       1,     3,     2,     3,     3,     3,     3,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       4,   281,   280,    29,   326,     0,     0,   255,   254,   214,
     218,   222,   215,   219,   223,   216,   220,   224,   217,   221,
     225,   195,   165,    56,    58,    63,     0,     0,     0,    35,
       0,     0,     0,    38,    88,    87,    89,    90,    72,    70,
     245,     0,   253,    73,    74,     0,     2,     0,     0,   140,
      11,    21,    24,     0,    31,     0,    43,    49,    91,    42,
      52,    50,   142,   157,   160,    51,   149,    33,   196,   197,
     198,     0,     0,     0,   246,    65,    66,    67,   256,    41,
     260,   113,     0,   327,   269,   261,   282,   325,   308,   309,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,   321,   291,   292,   286,   283,   284,   285,   287,   288,
     293,   290,   289,   294,   295,   296,   297,   298,   299,   300,
     302,   301,   303,   304,   305,   306,   307,   268,     0,   270,
     322,   323,   324,   279,     8,   101,   103,     5,   163,    57,
      59,    64,     0,    53,   121,    54,   116,     0,   119,     0,
      55,    36,     0,   154,   153,     0,    61,     0,    32,   177,
     183,   194,    39,   178,   187,   179,   188,    76,    71,    75,
      80,    69,    82,   244,   236,   237,   250,   249,   252,     1,
       3,    10,     6,    12,    25,     0,     0,    26,    60,    44,
     113,     0,    45,   141,   146,   143,     0,   161,   158,   151,
     150,    34,   227,   226,   203,   229,   228,   208,   231,   230,
     213,   200,   205,   210,   247,   257,   259,    99,   164,   115,
     201,   206,   211,   112,   328,     0,    17,   264,   266,   272,
     262,   277,   100,   102,     0,     0,     0,     0,   118,   126,
       0,    37,   156,    62,   101,   181,   175,   193,   176,    40,
     185,   182,   186,    77,    79,    68,    78,    85,   235,   242,
     240,   243,   251,     0,    13,    30,    27,    47,    95,    94,
      92,     0,    46,   104,   109,   145,   144,   148,   159,   152,
     202,   207,   212,   258,    97,    96,    98,   162,   199,   204,
     209,   114,    19,    16,   263,   273,   271,   275,   276,   278,
     274,   123,   122,   117,   120,     0,     0,     0,     0,   127,
       0,   132,     0,   100,   189,   166,   191,   180,   184,    81,
      84,   234,   238,   232,   233,   241,    83,   248,    28,    48,
     106,   111,   108,   147,    18,     0,     0,   129,   130,     0,
     134,     0,     0,   167,   168,   170,   172,   190,   192,   239,
      86,   107,   105,   110,   138,   139,     0,   124,   128,   133,
     136,   135,   137,   169,   173,   171,   125,   174
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,   168,   171,   259,    56,    57,    58,   272,   273,
      59,   145,   146,   147,   143,   144,   150,   240,   308,   309,
     310,   311,    60,    61,    62,    63,    64,    65,    66,   247,
     160,   161,   162,    67,    68,    69,    70,    71,    72,    73,
     204,   207,   210,   322,   323,   173,    74,    75,    76,    77,
      78,    79,    80,   128,   129,   130,   131,   132,   148,    82,
     226
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -253
static const yytype_int16 yypact[] =
{
     780,  -253,  -253,  -253,   -47,   378,    16,  -253,  -253,  -253,
    -253,  -253,  -253,  -253,  -253,  -253,  -253,  -253,  -253,  -253,
    -253,  -253,   129,    28,    28,    28,    45,   951,    56,  1161,
    1068,   951,  1008,  1145,  -253,  -253,  -253,  -253,   133,   111,
     334,   112,    77,  -253,  -253,    14,    17,    11,    69,  -253,
      71,   837,  -253,  1087,  -253,    77,   129,    78,  -253,    88,
      69,   194,  -253,  -253,    43,  -253,   219,    77,   242,   207,
     136,   129,   129,   129,    77,  -253,  -253,  -253,    28,  -253,
     149,  1188,   609,  -253,    84,  -253,  -253,  -253,  -253,  -253,
    -253,  -253,  -253,  -253,  -253,  -253,  -253,  -253,  -253,  -253,
    -253,  -253,  -253,  -253,  -253,  -253,  -253,  -253,  -253,  -253,
    -253,  -253,  -253,  -253,  -253,  -253,  -253,  -253,  -253,  -253,
    -253,  -253,  -253,  -253,  -253,  -253,  -253,  -253,   302,  -253,
    -253,  -253,  -253,    97,  -253,   894,   951,  -253,  -253,  -253,
    -253,  -253,   105,   116,  -253,   132,  -253,    23,  -253,   110,
    -253,    77,  1216,  -253,    77,   109,    28,    13,  -253,  -253,
     129,    41,    77,  -253,   129,  -253,  1202,    77,  -253,  -253,
     129,   133,   151,    15,  -253,  -253,   552,  -253,  -253,  -253,
    -253,  -253,  -253,   837,  -253,  1110,   154,   117,  -253,   164,
     219,    89,   129,  -253,  -253,   219,    69,  -253,    77,  -253,
      69,  -253,  -253,  -253,   129,  -253,  -253,   129,  -253,  -253,
     129,  -253,  -253,  -253,  -253,  -253,    28,   666,   129,  -253,
     129,   129,   129,    69,  -253,  1129,  -253,  -253,   159,   494,
    -253,   436,   172,  -253,   951,    45,   951,   951,  -253,  -253,
      72,  -253,  -253,  -253,   951,   122,   129,  -253,  -253,  -253,
     125,   129,   129,  -253,  -253,  -253,   129,   129,    41,    31,
      31,   181,  -253,    77,  -253,   130,  -253,   129,  -253,  -253,
    -253,   169,   185,  -253,   219,  -253,    69,  -253,  -253,  -253,
    -253,  -253,  -253,  -253,  -253,  -253,  -253,  -253,  -253,  -253,
    -253,  -253,   176,  -253,  -253,  -253,  -253,  -253,  -253,  -253,
    -253,  -253,  -253,  -253,  -253,   137,    45,   951,   193,  -253,
     170,  -253,   187,  -253,  -253,   103,  -253,   143,   145,  -253,
    -253,  -253,    31,  -253,  -253,  -253,   129,  -253,  -253,   185,
       6,  -253,    69,  -253,  -253,   951,   951,   116,   132,   115,
    -253,    49,   723,  -253,   196,   211,  -253,  -253,  -253,  -253,
    -253,  -253,  -253,  -253,  -253,  -253,   204,  -253,  -253,  -253,
    -253,  -253,  -253,  -253,   218,  -253,  -253,  -253
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -253,  -253,  -253,    42,  -253,    19,   142,  -253,   175,   179,
     202,  -253,    74,  -253,  -253,  -253,  -253,    52,   -18,   -80,
     197,   -52,    25,  -253,   -44,    29,  -253,  -253,  -253,   -69,
    -253,   -70,  -253,  -253,  -253,  -253,  -253,  -253,    -4,    20,
     107,  -253,  -253,   245,  -253,  -253,  -253,   -32,   -31,   -27,
    -253,  -253,  -253,  -252,   102,  -253,  -253,  -253,  -253,  -253,
     199,   114,   -20,  -253,    48,     0,  -253,   240,    10,  -253,
    -218
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -268
static const yytype_int16 yytable[] =
{
      81,   163,   164,   139,   140,   141,   165,   293,   325,     1,
       2,    -9,   181,    83,   179,   133,    -7,   134,     1,     2,
     244,   257,   138,   135,   136,   137,   154,   136,     4,   152,
     155,   237,    81,   166,     1,     2,   203,     5,   206,   172,
     174,   156,     5,   209,     1,     2,     1,     2,   142,   220,
     221,    81,     1,     2,   222,     4,   190,   195,   215,   149,
     198,   155,   258,    -9,   155,   305,   351,   182,    -7,   180,
     349,   211,   212,   213,   334,     1,     2,    22,   258,   193,
     194,     4,    81,   197,   183,   199,   191,   246,   305,   -93,
     268,     5,     1,     2,     7,     8,   192,   -93,   -93,   227,
     219,   -93,   -93,   -93,   231,   157,   -93,   -93,   343,   344,
     306,   307,   234,   345,     1,     2,   356,   170,     1,     2,
     220,   221,   239,   -93,   235,   222,     5,   238,   357,     7,
       8,   305,     1,     2,   163,   252,   243,   346,   167,   165,
     236,   -93,   208,   218,   335,   232,   233,     5,   336,   269,
       7,     8,   169,   306,   307,   177,   178,   256,   176,    11,
     245,   248,    14,   265,   250,    17,     7,     8,    20,   188,
     254,   340,   267,   261,   294,   196,   217,   266,   341,  -131,
     200,   201,   314,    81,   -15,   316,   263,   326,   214,   224,
     328,   271,   274,   330,   342,   223,   283,     1,     2,     1,
       2,   363,   339,   347,   280,   348,     4,   281,     5,   219,
     282,     7,     8,   205,   275,   277,   365,   366,   287,   279,
     288,   289,   290,   367,   225,   264,   184,   286,    22,    10,
     185,     4,    13,     5,   158,    16,     7,     8,    19,   133,
     312,   300,   291,   270,   301,   255,   315,   304,   202,   329,
     352,   317,   318,   189,   313,   338,   319,   320,   248,   324,
     324,   303,   337,     9,   302,   241,    12,   274,   242,    15,
     358,   359,    18,   251,   151,   260,   249,   296,   321,   216,
     175,   253,     0,     0,     0,   169,     0,     0,     0,     0,
     262,     0,     0,   331,     0,   333,     0,     0,     0,     0,
       0,     0,  -265,   228,   223,  -265,  -265,     0,     0,   276,
     229,  -265,   278,     0,  -265,  -265,  -265,   230,     0,  -265,
    -265,     0,   324,     0,     0,     0,   350,     0,     0,     0,
     274,     0,     0,     0,     0,     0,  -265,     1,     2,   312,
       0,   312,     0,     0,   364,   354,   355,     0,     0,     0,
       0,   353,   362,     0,  -265,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,     0,
       0,     0,     0,     0,     0,     0,     0,   327,  -267,    84,
       0,     1,     2,     0,     0,     0,  -267,  -267,   332,     0,
    -267,  -267,  -267,    85,    86,  -267,  -267,    87,     0,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,     1,
       2,   297,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,    87,   298,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   299,     1,     2,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,    87,     0,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   295,     1,     2,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     5,     0,    86,     7,
       8,    87,     0,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,     1,     2,     0,     0,     0,     0,     3,     0,
       0,     4,   224,     5,     0,   157,     7,     8,     0,     0,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,     0,     0,    34,    35,    36,
      37,     0,    38,    39,    40,    41,    42,    43,    44,     1,
       2,   284,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,    87,   285,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,     1,     2,   360,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,    87,   361,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,     1,     2,     0,     0,     0,     0,     3,
       0,     0,     4,     0,     5,     0,     6,     7,     8,     0,
       0,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,     0,     0,    34,    35,
      36,    37,     0,    38,    39,    40,    41,    42,    43,    44,
       1,     2,     0,     0,     0,     0,     3,     0,     0,     4,
       0,     5,     0,   157,     7,     8,     0,     0,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,     0,     0,    34,    35,    36,    37,     0,
      38,    39,    40,    41,    42,    43,    44,     1,     2,     0,
       0,     0,     0,     0,     0,     0,   -14,     0,     0,     0,
      86,     0,     0,    87,     0,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,     1,     2,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
      87,     0,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,     1,     2,     0,     0,     0,     0,     0,     0,     0,
       4,     0,     5,     0,   157,     7,     8,     0,     0,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,     0,    33,     0,     0,    34,    35,    36,    37,
       0,    38,    39,    40,    41,    42,    43,    44,  -155,   153,
       0,     1,     2,     0,     0,     0,  -155,  -155,     0,     0,
    -155,  -155,  -155,     0,     0,  -155,  -155,   -23,   186,     0,
     -23,   -23,     0,     0,     0,   -23,   187,     0,     0,   -23,
     -23,   -23,    22,     0,   -23,   -23,     0,     0,     0,     0,
     -22,   186,     0,   -22,   -22,     0,     0,     0,   -22,   187,
    -155,   -23,   -22,   -22,   -22,     0,     0,   -22,   -22,   -20,
     292,     0,   -20,   -20,     0,     0,     0,   -20,   -20,   -23,
       0,   -20,   224,   -20,   -22,     0,   -20,   -20,     1,     2,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   -22,   -20,     1,     2,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,     0,   159,
       0,   -20,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,   217,     0,     0,     0,     0,
       4,     0,     5,     0,     0,     7,     8,     0,     0,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,     0,   218,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,     0,   159,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-253))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       0,    33,    33,    23,    24,    25,    33,   225,   260,     3,
       4,     0,     1,    60,     0,     5,     0,     1,     3,     4,
       7,     6,    22,     7,    11,     6,    30,    11,    12,    29,
      30,     8,    32,    33,     3,     4,    68,    14,    69,    39,
      40,    31,    14,    70,     3,     4,     3,     4,     3,    81,
      81,    51,     3,     4,    81,    12,    56,    61,    78,     3,
      64,    61,    47,    52,    64,    16,    60,    48,    52,    52,
     322,    71,    72,    73,   292,     3,     4,    34,    47,    60,
      61,    12,    82,    64,    13,    66,     8,    46,    16,     0,
       1,    14,     3,     4,    17,    18,     8,     8,     9,    15,
      81,    12,    13,    14,     7,    16,    17,    18,     5,     6,
      38,    39,     7,    10,     3,     4,     1,     6,     3,     4,
     152,   152,    12,    34,     8,   152,    14,   147,    13,    17,
      18,    16,     3,     4,   166,   166,   156,    34,     5,   166,
       8,    52,     6,    34,     7,   135,   136,    14,    11,    60,
      17,    18,    38,    38,    39,    41,    42,     6,    46,    23,
     160,   161,    26,     9,   164,    29,    17,    18,    32,    55,
     170,     1,     8,   173,    15,    61,     7,    60,     8,     9,
      66,    67,    60,   183,    12,    60,   176,     6,    74,    13,
      60,   191,   192,     8,     7,    81,   216,     3,     4,     3,
       4,     5,     9,    60,   204,    60,    12,   207,    14,   190,
     210,    17,    18,     6,   195,   196,     5,    13,   218,   200,
     220,   221,   222,     5,    82,   183,    51,   217,    34,    22,
      51,    12,    25,    14,    32,    28,    17,    18,    31,   229,
     240,   231,   223,   191,   234,   171,   246,   237,     6,   267,
     330,   251,   252,    56,   244,   307,   256,   257,   258,   259,
     260,   236,   306,    21,   235,   151,    24,   267,   154,    27,
     339,   341,    30,   166,    29,   173,   162,   229,   258,    80,
      40,   167,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,
     176,    -1,    -1,   274,    -1,   276,    -1,    -1,    -1,    -1,
      -1,    -1,     0,     1,   190,     3,     4,    -1,    -1,   195,
       8,     9,   198,    -1,    12,    13,    14,    15,    -1,    17,
      18,    -1,   322,    -1,    -1,    -1,   326,    -1,    -1,    -1,
     330,    -1,    -1,    -1,    -1,    -1,    34,     3,     4,   339,
      -1,   341,    -1,    -1,   344,   335,   336,    -1,    -1,    -1,
      -1,   332,   342,    -1,    52,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   263,     0,     1,
      -1,     3,     4,    -1,    -1,    -1,     8,     9,   274,    -1,
      12,    13,    14,    15,    16,    17,    18,    19,    -1,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,     3,
       4,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,     3,     4,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      16,    -1,    -1,    19,    -1,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,     3,     4,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    14,    -1,    16,    17,
      18,    19,    -1,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,     3,     4,    -1,    -1,    -1,    -1,     9,    -1,
      -1,    12,    13,    14,    -1,    16,    17,    18,    -1,    -1,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    -1,    48,    49,    50,
      51,    -1,    53,    54,    55,    56,    57,    58,    59,     3,
       4,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,     3,     4,     5,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    16,
      -1,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,     3,     4,    -1,    -1,    -1,    -1,     9,
      -1,    -1,    12,    -1,    14,    -1,    16,    17,    18,    -1,
      -1,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    -1,    -1,    48,    49,
      50,    51,    -1,    53,    54,    55,    56,    57,    58,    59,
       3,     4,    -1,    -1,    -1,    -1,     9,    -1,    -1,    12,
      -1,    14,    -1,    16,    17,    18,    -1,    -1,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    -1,    -1,    48,    49,    50,    51,    -1,
      53,    54,    55,    56,    57,    58,    59,     3,     4,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      16,    -1,    -1,    19,    -1,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,     3,     4,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,
      19,    -1,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,     3,     4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    14,    -1,    16,    17,    18,    -1,    -1,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    -1,    45,    -1,    -1,    48,    49,    50,    51,
      -1,    53,    54,    55,    56,    57,    58,    59,     0,     1,
      -1,     3,     4,    -1,    -1,    -1,     8,     9,    -1,    -1,
      12,    13,    14,    -1,    -1,    17,    18,     0,     1,    -1,
       3,     4,    -1,    -1,    -1,     8,     9,    -1,    -1,    12,
      13,    14,    34,    -1,    17,    18,    -1,    -1,    -1,    -1,
       0,     1,    -1,     3,     4,    -1,    -1,    -1,     8,     9,
      52,    34,    12,    13,    14,    -1,    -1,    17,    18,     0,
       1,    -1,     3,     4,    -1,    -1,    -1,     8,     9,    52,
      -1,    12,    13,    14,    34,    -1,    17,    18,     3,     4,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    52,    34,     3,     4,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    34,
      -1,    52,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,     7,    -1,    -1,    -1,    -1,
      12,    -1,    14,    -1,    -1,    17,    18,    -1,    -1,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    34,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    34,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     9,    12,    14,    16,    17,    18,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    48,    49,    50,    51,    53,    54,
      55,    56,    57,    58,    59,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    76,    77,    78,    81,
      93,    94,    95,    96,    97,    98,    99,   104,   105,   106,
     107,   108,   109,   110,   117,   118,   119,   120,   121,   122,
     123,   126,   130,    60,     1,    15,    16,    19,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,   124,   125,
     126,   127,   128,   129,     1,     7,    11,    66,   126,   123,
     123,   123,     3,    85,    86,    82,    83,    84,   129,     3,
      87,   104,   126,     1,    99,   126,   129,    16,    71,    34,
     101,   102,   103,   108,   109,   110,   126,     5,    73,   122,
       6,    74,   126,   116,   126,   128,    46,   122,   122,     0,
      52,     1,    66,    13,    69,    70,     1,     9,   122,    81,
     126,     8,     8,    66,    66,    99,   122,    66,    99,    66,
     122,   122,     6,   108,   111,     6,   109,   112,     6,   110,
     113,   126,   126,   126,   122,   123,   121,     7,    34,    66,
     108,   109,   110,   122,    13,    67,   131,    15,     1,     8,
      15,     7,   129,   129,     7,     8,     8,     8,   123,    12,
      88,   122,   122,   123,     7,   126,    46,   100,   126,   122,
     126,   101,   109,   122,   126,    73,     6,     6,    47,    75,
     115,   126,   122,   129,    64,     9,    60,     8,     1,    60,
      78,   126,    79,    80,   126,    66,   122,    66,   122,    66,
     126,   126,   126,   123,     5,    20,   129,   126,   126,   126,
     126,    66,     1,   131,    15,    60,   125,     5,    20,    60,
     129,   129,    86,    83,   129,    16,    38,    39,    89,    90,
      91,    92,   126,   129,    60,   126,    60,   126,   126,   126,
     126,   100,   114,   115,   126,   114,     6,   122,    60,    79,
       8,    66,   122,    66,   131,     7,    11,    85,    82,     9,
       1,     8,     7,     5,     6,    10,    34,    60,    60,   114,
     126,    60,    80,    66,   129,   129,     1,    13,    90,    92,
       5,    20,   129,     5,   126,     5,    13,     5
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
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
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
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
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
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
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
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
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

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

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

/* Line 1391 of yacc.c  */
#line 210 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 1974 "language.cc"
	break;
      case 4: /* "TOK_QSTRING" */

/* Line 1391 of yacc.c  */
#line 210 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 1983 "language.cc"
	break;
      case 5: /* "TOK_NUMBER" */

/* Line 1391 of yacc.c  */
#line 215 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 1992 "language.cc"
	break;
      case 16: /* "TOK_MSC" */

/* Line 1391 of yacc.c  */
#line 211 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2001 "language.cc"
	break;
      case 17: /* "TOK_COLON_STRING" */

/* Line 1391 of yacc.c  */
#line 210 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2010 "language.cc"
	break;
      case 18: /* "TOK_COLON_QUOTED_STRING" */

/* Line 1391 of yacc.c  */
#line 210 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2019 "language.cc"
	break;
      case 19: /* "TOK_STYLE_NAME" */

/* Line 1391 of yacc.c  */
#line 210 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2028 "language.cc"
	break;
      case 20: /* "TOK_BOOLEAN" */

/* Line 1391 of yacc.c  */
#line 215 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2037 "language.cc"
	break;
      case 35: /* "TOK_COMMAND_HEADING" */

/* Line 1391 of yacc.c  */
#line 213 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2046 "language.cc"
	break;
      case 36: /* "TOK_COMMAND_NUDGE" */

/* Line 1391 of yacc.c  */
#line 213 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2055 "language.cc"
	break;
      case 37: /* "TOK_COMMAND_NEWPAGE" */

/* Line 1391 of yacc.c  */
#line 213 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2064 "language.cc"
	break;
      case 38: /* "TOK_COMMAND_DEFCOLOR" */

/* Line 1391 of yacc.c  */
#line 212 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2073 "language.cc"
	break;
      case 39: /* "TOK_COMMAND_DEFSTYLE" */

/* Line 1391 of yacc.c  */
#line 212 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2082 "language.cc"
	break;
      case 40: /* "TOK_COMMAND_DEFDESIGN" */

/* Line 1391 of yacc.c  */
#line 212 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2091 "language.cc"
	break;
      case 41: /* "TOK_COMMAND_BIG" */

/* Line 1391 of yacc.c  */
#line 211 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2100 "language.cc"
	break;
      case 42: /* "TOK_COMMAND_PIPE" */

/* Line 1391 of yacc.c  */
#line 211 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2109 "language.cc"
	break;
      case 43: /* "TOK_COMMAND_MARK" */

/* Line 1391 of yacc.c  */
#line 214 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2118 "language.cc"
	break;
      case 44: /* "TOK_COMMAND_PARALLEL" */

/* Line 1391 of yacc.c  */
#line 214 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2127 "language.cc"
	break;
      case 45: /* "TOK_VERTICAL" */

/* Line 1391 of yacc.c  */
#line 215 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2136 "language.cc"
	break;
      case 46: /* "TOK_AT" */

/* Line 1391 of yacc.c  */
#line 215 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2145 "language.cc"
	break;
      case 47: /* "TOK_AT_POS" */

/* Line 1391 of yacc.c  */
#line 215 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2154 "language.cc"
	break;
      case 48: /* "TOK_SHOW" */

/* Line 1391 of yacc.c  */
#line 215 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2163 "language.cc"
	break;
      case 49: /* "TOK_HIDE" */

/* Line 1391 of yacc.c  */
#line 215 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2172 "language.cc"
	break;
      case 50: /* "TOK_ACTIVATE" */

/* Line 1391 of yacc.c  */
#line 216 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2181 "language.cc"
	break;
      case 51: /* "TOK_DEACTIVATE" */

/* Line 1391 of yacc.c  */
#line 216 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2190 "language.cc"
	break;
      case 52: /* "TOK_BYE" */

/* Line 1391 of yacc.c  */
#line 214 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2199 "language.cc"
	break;
      case 53: /* "TOK_COMMAND_VSPACE" */

/* Line 1391 of yacc.c  */
#line 216 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2208 "language.cc"
	break;
      case 55: /* "TOK_COMMAND_SYMBOL" */

/* Line 1391 of yacc.c  */
#line 216 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2217 "language.cc"
	break;
      case 56: /* "TOK_COMMAND_NOTE" */

/* Line 1391 of yacc.c  */
#line 217 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2226 "language.cc"
	break;
      case 57: /* "TOK_COMMAND_COMMENT" */

/* Line 1391 of yacc.c  */
#line 217 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2235 "language.cc"
	break;
      case 58: /* "TOK_COMMAND_TITLE" */

/* Line 1391 of yacc.c  */
#line 217 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2244 "language.cc"
	break;
      case 59: /* "TOK_COMMAND_SUBTITLE" */

/* Line 1391 of yacc.c  */
#line 217 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2253 "language.cc"
	break;
      case 64: /* "top_level_arclist" */

/* Line 1391 of yacc.c  */
#line 205 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1391 of yacc.c  */
#line 2262 "language.cc"
	break;
      case 66: /* "braced_arclist" */

/* Line 1391 of yacc.c  */
#line 205 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1391 of yacc.c  */
#line 2271 "language.cc"
	break;
      case 67: /* "arclist_maybe_no_semicolon" */

/* Line 1391 of yacc.c  */
#line 205 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1391 of yacc.c  */
#line 2280 "language.cc"
	break;
      case 68: /* "arclist" */

/* Line 1391 of yacc.c  */
#line 205 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1391 of yacc.c  */
#line 2289 "language.cc"
	break;
      case 69: /* "arc_with_parallel_semicolon" */

/* Line 1391 of yacc.c  */
#line 200 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2298 "language.cc"
	break;
      case 70: /* "arc_with_parallel" */

/* Line 1391 of yacc.c  */
#line 200 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2307 "language.cc"
	break;
      case 71: /* "arc" */

/* Line 1391 of yacc.c  */
#line 200 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2316 "language.cc"
	break;
      case 73: /* "full_arcattrlist_with_label_or_number" */

/* Line 1391 of yacc.c  */
#line 208 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1391 of yacc.c  */
#line 2325 "language.cc"
	break;
      case 77: /* "optlist" */

/* Line 1391 of yacc.c  */
#line 205 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1391 of yacc.c  */
#line 2334 "language.cc"
	break;
      case 78: /* "opt" */

/* Line 1391 of yacc.c  */
#line 200 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2343 "language.cc"
	break;
      case 79: /* "entitylist" */

/* Line 1391 of yacc.c  */
#line 206 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entitylist);};

/* Line 1391 of yacc.c  */
#line 2352 "language.cc"
	break;
      case 80: /* "entity" */

/* Line 1391 of yacc.c  */
#line 206 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entitylist);};

/* Line 1391 of yacc.c  */
#line 2361 "language.cc"
	break;
      case 81: /* "first_entity" */

/* Line 1391 of yacc.c  */
#line 206 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entitylist);};

/* Line 1391 of yacc.c  */
#line 2370 "language.cc"
	break;
      case 84: /* "tok_stringlist" */

/* Line 1391 of yacc.c  */
#line 207 "language.yy"
	{if (!C_S_H) delete (yyvaluep->stringlist);};

/* Line 1391 of yacc.c  */
#line 2379 "language.cc"
	break;
      case 93: /* "parallel" */

/* Line 1391 of yacc.c  */
#line 204 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcparallel);};

/* Line 1391 of yacc.c  */
#line 2388 "language.cc"
	break;
      case 94: /* "box_list" */

/* Line 1391 of yacc.c  */
#line 203 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcboxseries);};

/* Line 1391 of yacc.c  */
#line 2397 "language.cc"
	break;
      case 95: /* "first_box" */

/* Line 1391 of yacc.c  */
#line 203 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbox);};

/* Line 1391 of yacc.c  */
#line 2406 "language.cc"
	break;
      case 96: /* "first_pipe" */

/* Line 1391 of yacc.c  */
#line 203 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcpipe);};

/* Line 1391 of yacc.c  */
#line 2415 "language.cc"
	break;
      case 97: /* "pipe_list_no_content" */

/* Line 1391 of yacc.c  */
#line 203 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcpipeseries);};

/* Line 1391 of yacc.c  */
#line 2424 "language.cc"
	break;
      case 98: /* "pipe_list" */

/* Line 1391 of yacc.c  */
#line 203 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcpipeseries);};

/* Line 1391 of yacc.c  */
#line 2433 "language.cc"
	break;
      case 99: /* "boxrel" */

/* Line 1391 of yacc.c  */
#line 203 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbox);};

/* Line 1391 of yacc.c  */
#line 2442 "language.cc"
	break;
      case 100: /* "vertxpos" */

/* Line 1391 of yacc.c  */
#line 198 "language.yy"
	{if (!C_S_H) delete (yyvaluep->vertxpos);};

/* Line 1391 of yacc.c  */
#line 2451 "language.cc"
	break;
      case 102: /* "vertrel_no_xpos" */

/* Line 1391 of yacc.c  */
#line 199 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcvertarrow);};

/* Line 1391 of yacc.c  */
#line 2460 "language.cc"
	break;
      case 103: /* "vertrel" */

/* Line 1391 of yacc.c  */
#line 200 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2469 "language.cc"
	break;
      case 104: /* "arcrel" */

/* Line 1391 of yacc.c  */
#line 200 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2478 "language.cc"
	break;
      case 105: /* "arcrel_to" */

/* Line 1391 of yacc.c  */
#line 202 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 1391 of yacc.c  */
#line 2487 "language.cc"
	break;
      case 106: /* "arcrel_from" */

/* Line 1391 of yacc.c  */
#line 202 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 1391 of yacc.c  */
#line 2496 "language.cc"
	break;
      case 107: /* "arcrel_bidir" */

/* Line 1391 of yacc.c  */
#line 202 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 1391 of yacc.c  */
#line 2505 "language.cc"
	break;
      case 116: /* "symbol_type_string" */

/* Line 1391 of yacc.c  */
#line 209 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2514 "language.cc"
	break;
      case 117: /* "symbol_command_no_attr" */

/* Line 1391 of yacc.c  */
#line 201 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2523 "language.cc"
	break;
      case 118: /* "symbol_command" */

/* Line 1391 of yacc.c  */
#line 201 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2532 "language.cc"
	break;
      case 119: /* "note" */

/* Line 1391 of yacc.c  */
#line 201 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2541 "language.cc"
	break;
      case 120: /* "comment" */

/* Line 1391 of yacc.c  */
#line 201 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2550 "language.cc"
	break;
      case 121: /* "colon_string" */

/* Line 1391 of yacc.c  */
#line 209 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2559 "language.cc"
	break;
      case 122: /* "full_arcattrlist_with_label" */

/* Line 1391 of yacc.c  */
#line 207 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1391 of yacc.c  */
#line 2568 "language.cc"
	break;
      case 123: /* "full_arcattrlist" */

/* Line 1391 of yacc.c  */
#line 207 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1391 of yacc.c  */
#line 2577 "language.cc"
	break;
      case 124: /* "arcattrlist" */

/* Line 1391 of yacc.c  */
#line 207 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1391 of yacc.c  */
#line 2586 "language.cc"
	break;
      case 125: /* "arcattr" */

/* Line 1391 of yacc.c  */
#line 207 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attrib);};

/* Line 1391 of yacc.c  */
#line 2595 "language.cc"
	break;
      case 126: /* "entity_string" */

/* Line 1391 of yacc.c  */
#line 209 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2604 "language.cc"
	break;
      case 127: /* "reserved_word_string" */

/* Line 1391 of yacc.c  */
#line 209 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2613 "language.cc"
	break;
      case 128: /* "symbol_string" */

/* Line 1391 of yacc.c  */
#line 209 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2622 "language.cc"
	break;
      case 129: /* "string" */

/* Line 1391 of yacc.c  */
#line 209 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2631 "language.cc"
	break;
      case 131: /* "scope_close" */

/* Line 1391 of yacc.c  */
#line 200 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2640 "language.cc"
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


/*----------.
| yyparse.  |
`----------*/

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
    YYLTYPE yyerror_range[3];

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

#if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
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
  if (yypact_value_is_default (yyn))
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
        case 3:

/* Line 1806 of yacc.c  */
#line 223 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_KEYWORD);
  #else
  #endif
    free((yyvsp[(2) - (2)].str));
    YYACCEPT; //ignore anything after bye
}
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 233 "language.yy"
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
    break;

  case 5:

/* Line 1806 of yacc.c  */
#line 244 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
    msc.AddArcs((yyvsp[(2) - (2)].arclist));
  #endif
    free((yyvsp[(1) - (2)].str));

    YYACCEPT;
}
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 255 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(1) - (2)])))
        csh.AddDesignsToHints(true);
  #else
    msc.AddArcs((yyvsp[(2) - (2)].arclist));
  #endif
    YYACCEPT;
}
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 265 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    csh.AddCSH_ErrorAfter((yylsp[(1) - (1)]), "Missing an equal sign or a list of elements between braces ('{' and '}').");
  #else
    msc.Error.Error(MSC_POS((yylsp[(1) - (1)])).end.NextChar(), "Missing an equal sign or a list of elements between braces ('{' and '}').");
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 275 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    csh.AddCSH_Error((yylsp[(2) - (2)]), "Missing an equal sign or a list of elements between braces ('{' and '}').");
  #else
    msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).start, "Missing an equal sign or a list of elements between braces ('{' and '}').");
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 285 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    msc.AddArcs((yyvsp[(1) - (1)].arclist));
  #endif
}
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 292 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (((yylsp[(1) - (2)])).last_pos >= ((yylsp[(2) - (2)])).first_pos)
        ((yylsp[(2) - (2)])).first_pos = ((yylsp[(1) - (2)])).last_pos;
    csh.AddCSH_Error((yylsp[(2) - (2)]), "Could not recognize this as a valid line.");
  #else
    msc.AddArcs((yyvsp[(1) - (2)].arclist));
    msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).start, "Could not recognize this as a valid line.");
  #endif
  YYACCEPT;
}
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 306 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[(2) - (2)]), "Closing brace missing its opening pair.");
  #else
    (yyval.arclist) = (yyvsp[(1) - (2)].arclist);
    msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).start, "Unexpected '}'.");
  #endif
}
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 315 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[(2) - (3)]), "Closing brace missing its opening pair.");
  #else
    //Merge $3 into $1
    ((yyvsp[(1) - (3)].arclist))->splice(((yyvsp[(1) - (3)].arclist))->end(), *((yyvsp[(3) - (3)].arclist)));
    delete ((yyvsp[(3) - (3)].arclist));
    (yyval.arclist) = (yyvsp[(1) - (3)].arclist);
    msc.Error.Error(MSC_POS((yylsp[(3) - (3)])).start, "Unexpected '}'.");
  #endif
}
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 329 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_EQUAL);
    csh.AddCSH_ErrorAfter((yylsp[(2) - (2)]), "Missing a design name.");
    csh.CheckHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF, HINT_ATTR_VALUE, "msc");
  #else
    msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).end.NextChar(), "Missing design name.");
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 341 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
    csh.AddCSH((yylsp[(3) - (3)]), COLOR_DESIGNNAME);
    csh.CheckHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_VALUE, "msc");
    std::string msg = csh.SetDesignTo((yyvsp[(3) - (3)].str), true);
    if (msg.length())
        csh.AddCSH_Error((yylsp[(3) - (3)]), msg.c_str());
  #else
    msc.AddAttribute(Attribute("msc", (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(1) - (3)])), MSC_POS((yylsp[(3) - (3)]))));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 358 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_BRACE);
    csh.AddCSH((yylsp[(3) - (3)]), COLOR_BRACE);
  #else
    if ((yyvsp[(3) - (3)].arcbase)) {
        ((yyvsp[(2) - (3)].arclist))->Append((yyvsp[(3) - (3)].arcbase)); //Append any potential CommandNumbering
        ((yyvsp[(3) - (3)].arcbase))->MakeMeLastNotable();
    }
    (yyval.arclist) = (yyvsp[(2) - (3)].arclist);
  #endif
}
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 371 "language.yy"
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

  case 18:

/* Line 1806 of yacc.c  */
#line 384 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (4)]), COLOR_BRACE);
    csh.AddCSH_Error((yylsp[(3) - (4)]), "Could not recognize this as a valid line.");
    csh.AddCSH((yylsp[(4) - (4)]), COLOR_BRACE);
  #else
    if ((yyvsp[(4) - (4)].arcbase)) {
        ((yyvsp[(2) - (4)].arclist))->Append((yyvsp[(4) - (4)].arcbase)); //Append any potential CommandNumbering
        ((yyvsp[(4) - (4)].arcbase))->MakeMeLastNotable();
    }
    (yyval.arclist) = (yyvsp[(2) - (4)].arclist);
    msc.Error.Error(MSC_POS((yylsp[(3) - (4)])).start, "Syntax error.");
  #endif
    yyerrok;
}
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 400 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_BRACE);
    csh.AddCSH_Error((yylsp[(3) - (3)]), "Could not recognize this as a valid line.");
  #else
    (yyval.arclist) = (yyvsp[(2) - (3)].arclist);
    //Do not pop context, as the missing scope_close would have done
    msc.Error.Error(MSC_POS((yylsp[(3) - (3)])).start, "Missing '}'.");
    msc.Error.Error(MSC_POS((yylsp[(1) - (3)])).start, MSC_POS((yylsp[(3) - (3)])).start, "Here is the corresponding '{'.");
  #endif
}
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 412 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_BRACE);
    csh.AddCSH_ErrorAfter((yylsp[(2) - (2)]), "Missing a closing brace ('}').");
  #else
    (yyval.arclist) = (yyvsp[(2) - (2)].arclist);
    //Do not pop context, as the missing scope_close would have done
    msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).end.NextChar(), "Missing '}'.");
    msc.Error.Error(MSC_POS((yylsp[(1) - (2)])).start, MSC_POS((yylsp[(2) - (2)])).end.NextChar(), "Here is the corresponding '{'.");
  #endif
}
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 426 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ErrorAfter((yylsp[(2) - (2)]), "Missing a semicolon (';').");
  #else
    if ((yyvsp[(2) - (2)].arcbase)) {
        ((yyvsp[(1) - (2)].arclist))->Append((yyvsp[(2) - (2)].arcbase));
        ((yyvsp[(2) - (2)].arcbase))->MakeMeLastNotable();
    }
    (yyval.arclist) = (yyvsp[(1) - (2)].arclist);
    msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).end.NextChar(), "Missing ';'.");
    msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).start, MSC_POS((yylsp[(2) - (2)])).end.NextChar(), "Here is the beginning of the command as I understood it.");
  #endif
}
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 440 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ErrorAfter((yylsp[(1) - (1)]), "Missing a semicolon (';').");
  #else
    if ((yyvsp[(1) - (1)].arcbase)) {
        (yyval.arclist) = (new ArcList)->Append((yyvsp[(1) - (1)].arcbase)); /* New list */
        ((yyvsp[(1) - (1)].arcbase))->MakeMeLastNotable();
    } else
        (yyval.arclist) = new ArcList;
    msc.Error.Error(MSC_POS((yylsp[(1) - (1)])).end.NextChar(), "Missing ';'.");
    msc.Error.Error(MSC_POS((yylsp[(1) - (1)])).start, MSC_POS((yylsp[(1) - (1)])).end.NextChar(), "Here is the beginning of the command as I understood it.");
  #endif
}
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 456 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(1) - (1)].arcbase)) {
        (yyval.arclist) = (new ArcList)->Append((yyvsp[(1) - (1)].arcbase)); /* New list */
        ((yyvsp[(1) - (1)].arcbase))->MakeMeLastNotable();
    } else
        (yyval.arclist) = new ArcList;
  #endif
}
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 466 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(2) - (2)].arcbase)) {
        ((yyvsp[(1) - (2)].arclist))->Append((yyvsp[(2) - (2)].arcbase));     /* Add to existing list */
        ((yyvsp[(2) - (2)].arcbase))->MakeMeLastNotable();
    }
    (yyval.arclist) = ((yyvsp[(1) - (2)].arclist));
  #endif
}
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 477 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SEMICOLON);
    if (csh.CheckHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF, HINT_LINE_START)) {
       csh.AddLineBeginToHints();
       csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.arcbase)=(yyvsp[(1) - (2)].arcbase);
  #endif
}
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 493 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_SEMICOLON);
    if (csh.CheckHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF, HINT_LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.arcbase)=NULL;
  #endif
}
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 505 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(3) - (3)]), COLOR_SEMICOLON);
    csh.AddCSH_Error((yylsp[(2) - (3)]), "I am not sure what is coming here.");
    if (csh.CheckHintAfter((yylsp[(3) - (3)]), yylloc, yychar==YYEOF, HINT_LINE_START)) {
       csh.AddLineBeginToHints();
       csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.arcbase)=(yyvsp[(1) - (3)].arcbase);
    msc.Error.Error(MSC_POS((yylsp[(1) - (3)])).end.NextChar(), "Expecting a keyword, entity name, a valid arrow, box or divider symbol or an opening brace.");
  #endif
}
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 520 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        if ((yyvsp[(1) - (1)].arcbase)) ((yyvsp[(1) - (1)].arcbase))->SetLineEnd(MSC_POS((yyloc)));
        (yyval.arcbase)=(yyvsp[(1) - (1)].arcbase);
  #endif
}
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 528 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[(1) - (2)]), HINT_LINE_START)) {
	    csh.AddLineBeginToHints(false);
	    csh.hintStatus = HINT_READY;
	} else if (csh.CheckHintBetween((yylsp[(1) - (2)]), (yylsp[(2) - (2)]), HINT_LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
        csh.hintType = HINT_LINE_START;
        csh.hintsForcedOnly = true;
    }
  #else
    if ((yyvsp[(2) - (2)].arcbase)) {
        ((yyvsp[(2) - (2)].arcbase))->SetLineEnd(MSC_POS((yyloc)));
        ((yyvsp[(2) - (2)].arcbase))->SetParallel();
    }
    (yyval.arcbase) = (yyvsp[(2) - (2)].arcbase);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 551 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.arcbase)=((yyvsp[(1) - (1)].arcbase))->AddAttributeList(NULL);
  #endif
}
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 558 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (2)])))
        ArcArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (2)])))
        ArcArrow::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcbase) = ((yyvsp[(1) - (2)].arcbase))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
  #endif
}
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 569 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    csh.CheckEntityHintAfterPlusOne((yylsp[(1) - (1)]), yylloc, yychar==YYEOF);
    csh.AddCSH_ErrorAfter((yylsp[(1) - (1)]), "Missing an arrow specification.");
  #else
    msc.Error.Error(MSC_POS((yylsp[(1) - (1)])).end.NextChar(), "Missing an arrow specification.");
  #endif
    free((yyvsp[(1) - (1)].str));
    (yyval.arcbase) = NULL;
}
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 581 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
  #else
    //Returns NULL, if BIG is before a self-pointing arrow
    ArcBase *arc = msc.CreateArcBigArrow((yyvsp[(2) - (2)].arcbase));
    if (arc)
        arc->AddAttributeList(NULL);
    delete (yyvsp[(2) - (2)].arcbase);
    (yyval.arcbase) = arc;
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 596 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[(1) - (3)]), (yylsp[(2) - (3)]));
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(3) - (3)])))
        ArcBigArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(3) - (3)])))
        ArcBigArrow::AttributeValues(csh.hintAttrName, csh);
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

  case 38:

/* Line 1806 of yacc.c  */
#line 614 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    csh.CheckHintAfterPlusOne((yylsp[(1) - (1)]), yylloc, yychar==YYEOF, HINT_MARKER);
    csh.AddCSH_ErrorAfter((yylsp[(1) - (1)]), "Missing a marker.");
  #else
    msc.Error.Error(MSC_POS((yylsp[(1) - (1)])).end.NextChar(), "Missing a marker.");
  #endif
    free((yyvsp[(1) - (1)].str));
    (yyval.arcbase) = NULL;
}
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 626 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
    if ((yyvsp[(2) - (2)].arcbase)) (yyval.arcbase) = ((yyvsp[(2) - (2)].arcbase))->AddAttributeList(NULL);
    else (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 636 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(3) - (3)])))
        ArcVerticalArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(3) - (3)])))
        ArcVerticalArrow::AttributeValues(csh.hintAttrName, csh);
  #else
    if ((yyvsp[(2) - (3)].arcbase)) (yyval.arcbase) = ((yyvsp[(2) - (3)].arcbase))->AddAttributeList((yyvsp[(3) - (3)].attriblist));
    else (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 650 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(1) - (1)])))
        ArcDivider::AttributeNames(csh, false, false);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(1) - (1)])))
        ArcDivider::AttributeValues(csh.hintAttrName, csh, false, false);
  #else
    (yyval.arcbase) = (new ArcDivider(MSC_ARC_VSPACE, &msc))->AddAttributeList((yyvsp[(1) - (1)].attriblist));
  #endif
}
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 661 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (new CommandEntity((yyvsp[(1) - (1)].entitylist), &msc, false))->AddAttributeList(NULL);
  #endif
}
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 668 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    csh.CheckEntityHintAfterPlusOne((yylsp[(1) - (1)]), yylloc, yychar==YYEOF);
    csh.AddCSH_ErrorAfter((yylsp[(1) - (1)]), "Missing an entity.");
  #else
    msc.Error.Error(MSC_POS((yylsp[(1) - (1)])).end.NextChar(), "Missing an entity.");
  #endif
    free((yyvsp[(1) - (1)].str));
    (yyval.arcbase) = NULL;
}
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 680 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
  #else
    CommandEntity *ce = new CommandEntity((yyvsp[(2) - (2)].entitylist), &msc, false);
    ce->AddAttributeList(NULL);
	(yyval.arcbase) = ce->ApplyPrefix((yyvsp[(1) - (2)].str));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 692 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_COMMA);
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
    csh.AddCSH_ErrorAfter((yylsp[(2) - (2)]), "Missing an entity.");
  #else
    CommandEntity *ce = new CommandEntity((yyvsp[(1) - (2)].entitylist), &msc, false);
    (yyval.arcbase) = ce->AddAttributeList(NULL);
    msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).end.NextChar(), "Missing an entity.");
  #endif
}
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 704 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    csh.CheckEntityHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]));
  #else
    CommandEntity *ce = new CommandEntity(((yyvsp[(3) - (3)].entitylist))->Prepend((yyvsp[(1) - (3)].entitylist)), &msc, false);
	delete ((yyvsp[(1) - (3)].entitylist));
    (yyval.arcbase) = ce->AddAttributeList(NULL);
  #endif
}
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 715 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[(3) - (3)]), COLOR_COMMA);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[(1) - (3)]), (yylsp[(2) - (3)]));
    csh.CheckEntityHintAfter((yylsp[(3) - (3)]), yylloc, yychar==YYEOF);
    csh.AddCSH_ErrorAfter((yylsp[(3) - (3)]), "Missing an entity.");
  #else
    CommandEntity *ce = new CommandEntity((yyvsp[(2) - (3)].entitylist), &msc, false);
    ce->AddAttributeList(NULL);
	(yyval.arcbase) = ce->ApplyPrefix((yyvsp[(1) - (3)].str));
    msc.Error.Error(MSC_POS((yylsp[(3) - (3)])).end.NextChar(), "Missing an entity.");
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 731 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (4)]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[(3) - (4)]), COLOR_COMMA);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[(1) - (4)]), (yylsp[(2) - (4)]));
    csh.CheckEntityHintAtAndBefore((yylsp[(3) - (4)]), (yylsp[(4) - (4)]));
  #else
    CommandEntity *ce = new CommandEntity(((yyvsp[(4) - (4)].entitylist))->Prepend((yyvsp[(2) - (4)].entitylist)), &msc, false);
	delete ((yyvsp[(2) - (4)].entitylist));
    ce->AddAttributeList(NULL);
    (yyval.arcbase) = ce->ApplyPrefix((yyvsp[(1) - (4)].str));
  #endif
    free((yyvsp[(1) - (4)].str));
}
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 746 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    /* If there were arcs defined by the options (e.g., background)
     * enclose them in an "CommandArcList" element used only for this. */
    if ((yyvsp[(1) - (1)].arclist)) {
        (yyval.arcbase) = (new CommandArcList(&msc, (yyvsp[(1) - (1)].arclist)))->AddAttributeList(NULL);
    } else
        (yyval.arcbase) = NULL;
  #endif
}
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 758 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcboxseries); //to remove warning for downcast
  #endif
}
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 765 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcpipeseries); //to remove warning for downcast
  #endif
}
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 772 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = ((yyvsp[(1) - (1)].arcparallel))->AddAttributeList(NULL);
  #endif
}
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 779 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 788 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 797 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 806 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = (new CommandEntity(NULL, &msc, false))->AddAttributeList(NULL);
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 815 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (2)])))
        CommandEntity::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (2)])))
        CommandEntity::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcbase) = (new CommandEntity(NULL, &msc, false))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 828 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = (new ArcDivider(MSC_COMMAND_NUDGE, &msc))->AddAttributeList(NULL);
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 837 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (2)])))
        ArcDivider::AttributeNames(csh, true, false);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (2)])))
        ArcDivider::AttributeValues(csh.hintAttrName, csh, true, false);
  #else
    (yyval.arcbase) = (new ArcDivider(MSC_COMMAND_NUDGE, &msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 850 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (2)])))
        ArcDivider::AttributeNames(csh, false, true);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (2)])))
        ArcDivider::AttributeValues(csh.hintAttrName, csh, false, true);
  #else
    const MscArcType t = CaseInsensitiveEqual("title", (yyvsp[(1) - (2)].str)) ? MSC_COMMAND_TITLE :
                         CaseInsensitiveEqual("subtitle", (yyvsp[(1) - (2)].str)) ? MSC_COMMAND_SUBTITLE :
                         MSC_ARC_INVALID;
    (yyval.arcbase) = (new ArcDivider(t, &msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 866 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_MARKERNAME);
    csh.MarkerNames.insert((yyvsp[(2) - (2)].str));
  #else
    (yyval.arcbase) = (new CommandMark((yyvsp[(2) - (2)].str), MSC_POS((yyloc)), &msc))->AddAttributeList(NULL);
  #endif
    free((yyvsp[(1) - (2)].str));
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 878 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_MARKERNAME);
    csh.MarkerNames.insert((yyvsp[(2) - (3)].str));
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (3)])))
        CommandMark::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (3)])))
        CommandMark::AttributeValues(csh.hintAttrName, csh);
  #else
        (yyval.arcbase) = (new CommandMark((yyvsp[(2) - (3)].str), MSC_POS((yyloc)), &msc))->AddAttributeList((yyvsp[(3) - (3)].attriblist));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 894 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
  #else
        (yyval.arcbase) = (new CommandNewpage(&msc))->AddAttributeList(NULL);
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 903 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (2)])))
        CommandNewpage::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (2)])))
        CommandNewpage::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcbase) = (new CommandNewpage(&msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 919 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (3)])))
        CommandHSpace::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (3)])))
        CommandHSpace::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcbase) = (new CommandHSpace(&msc, (yyvsp[(2) - (3)].namerel)))->AddAttributeList((yyvsp[(3) - (3)].attriblist));
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 932 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    if (csh.CheckHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF, HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", HINT_KEYWORD, false));
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<label>", HINT_KEYWORD, false));
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_ErrorAfter((yylsp[(1) - (2)]), "Missing either a number or a label.");
  #else
    (yyval.arcbase) = new CommandHSpace(&msc, (yyvsp[(2) - (2)].namerel)); //Will trigger an error: either label or space attr is needed
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 947 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    csh.CheckEntityHintAfterPlusOne((yylsp[(1) - (1)]), yylloc, yychar==YYEOF);
    csh.AddCSH_ErrorAfter((yylsp[(1) - (1)]), "Missing an entity.");
  #else
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 958 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (2)])))
        CommandHSpace::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (2)])))
        CommandHSpace::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcbase) = (new CommandVSpace(&msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 971 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    if (csh.CheckHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF, HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", HINT_KEYWORD, false));
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<label>", HINT_KEYWORD, false));
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_ErrorAfter((yylsp[(1) - (1)]), "Missing either a number or a label.");
  #else
    (yyval.arcbase) = new CommandVSpace(&msc); //will result in an error, since label or space attribute is needed
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 990 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_ATTRVALUE);
  #else
    AttributeList *al = new AttributeList;
    al->Append(new Attribute("space", atof((yyvsp[(1) - (1)].str)), MSC_POS((yylsp[(1) - (1)])), MSC_POS((yylsp[(1) - (1)])), (yyvsp[(1) - (1)].str)));
    (yyval.attriblist) = al;
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 1001 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_ATTRVALUE);
  #else
    ((yyvsp[(2) - (2)].attriblist))->Append(new Attribute("space", atof((yyvsp[(1) - (2)].str)), MSC_POS((yylsp[(1) - (2)])), MSC_POS((yylsp[(1) - (2)])), (yyvsp[(1) - (2)].str)));
    (yyval.attriblist) = (yyvsp[(2) - (2)].attriblist);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 1012 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_EntityName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
    csh.CheckEntityHintAt((yylsp[(1) - (2)]));
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.namerel) = new NamePair((yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), NULL, file_line_range());
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 1024 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
    csh.AddCSH_EntityName((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
    csh.CheckEntityHintAt((yylsp[(2) - (2)]));
  #else
    (yyval.namerel) = new NamePair(NULL, file_line_range(), (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])));
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 1035 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF);
  #else
    (yyval.namerel) = NULL;
  #endif
}
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 1044 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_EntityName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));
    csh.CheckEntityHintAt((yylsp[(1) - (3)]));
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    csh.AddCSH_EntityName((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
    csh.CheckEntityHintAt((yylsp[(3) - (3)]));
  #else
    (yyval.namerel) = new NamePair((yyvsp[(1) - (3)].str), MSC_POS((yylsp[(1) - (3)])), (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(3) - (3)])));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 1058 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_EntityName((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str));
    csh.CheckEntityHintAt((yylsp[(1) - (1)]));
  #else
    (yyval.namerel) = new NamePair((yyvsp[(1) - (1)].str), MSC_POS((yylsp[(1) - (1)])), NULL, file_line_range());
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 1083 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_MARKERNAME);
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckHintAt((yylsp[(1) - (2)]), HINT_MARKER);
    csh.CheckHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    (yyval.namerel) = new NamePair((yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), NULL, file_line_range());
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 1095 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_MARKERNAME);
    csh.CheckHintAt((yylsp[(2) - (2)]), HINT_MARKER);
  #else
    (yyval.namerel) = new NamePair(NULL, file_line_range(), (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])));
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 1106 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
    csh.CheckHintAt((yylsp[(1) - (1)]), HINT_MARKER);
    csh.CheckHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    (yyval.namerel) = NULL;
  #endif
}
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 1116 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_MARKERNAME);
    csh.CheckHintAt((yylsp[(1) - (3)]), HINT_MARKER);
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    csh.AddCSH((yylsp[(3) - (3)]), COLOR_MARKERNAME);
    csh.CheckHintAt((yylsp[(3) - (3)]), HINT_MARKER);
  #else
    (yyval.namerel) = new NamePair((yyvsp[(1) - (3)].str), MSC_POS((yylsp[(1) - (3)])), (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(3) - (3)])));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 1133 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(1) - (1)].arcbase)) {
        (yyval.arclist) = (new ArcList)->Append((yyvsp[(1) - (1)].arcbase)); /* New list */
        //($1)->MakeMeLastNotable(); Do not make chart options notable
    } else
        (yyval.arclist) = NULL;
  #endif
}
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 1143 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    if (csh.CheckHintBetween((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    if ((yyvsp[(3) - (3)].arcbase)) {
        if ((yyvsp[(1) - (3)].arclist))
            (yyval.arclist) = ((yyvsp[(1) - (3)].arclist))->Append((yyvsp[(3) - (3)].arcbase));     /* Add to existing list */
        else
            (yyval.arclist) = (new ArcList)->Append((yyvsp[(3) - (3)].arcbase)); /* New list */
        //($3)->MakeMeLastNotable(); Do not make chart options notable
    } else
        (yyval.arclist) = (yyvsp[(1) - (3)].arclist);
  #endif
}
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 1162 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_COMMA);
    if (csh.CheckHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF, HINT_ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.arclist) = (yyvsp[(1) - (2)].arclist);
    msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).end.NextChar(), "Expecting an option here.");
  #endif
}
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 1176 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    csh.AddCSH_Error((yylsp[(3) - (3)]), "An option expected here.");
  #else
    (yyval.arclist) = (yyvsp[(1) - (3)].arclist);
  #endif
}
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 1187 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_OPTIONNAME);
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
    csh.AddCSH((yylsp[(3) - (3)]), COLOR_ATTRVALUE);
    if (csh.CheckHintAt((yylsp[(1) - (3)]), HINT_ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_VALUE, (yyvsp[(1) - (3)].str))) {
        Msc::AttributeValues((yyvsp[(1) - (3)].str), csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.arcbase) = msc.AddAttribute(Attribute((yyvsp[(1) - (3)].str), str2bool((yyvsp[(3) - (3)].str)), MSC_POS((yylsp[(1) - (3)])), MSC_POS((yylsp[(3) - (3)])), (yyvsp[(3) - (3)].str)));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 1206 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_OPTIONNAME);
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
    csh.AddCSH((yylsp[(3) - (3)]), COLOR_ATTRVALUE);
    if (csh.CheckHintAt((yylsp[(1) - (3)]), HINT_ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_VALUE, (yyvsp[(1) - (3)].str))) {
        Msc::AttributeValues((yyvsp[(1) - (3)].str), csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.arcbase) = msc.AddAttribute(Attribute((yyvsp[(1) - (3)].str), atof((yyvsp[(3) - (3)].str)), MSC_POS((yylsp[(1) - (3)])), MSC_POS((yylsp[(3) - (3)])), (yyvsp[(3) - (3)].str)));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 1225 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_OPTIONNAME);
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
    csh.AddCSH_AttrValue((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str), (yyvsp[(1) - (3)].str));
    if (csh.CheckHintAt((yylsp[(1) - (3)]), HINT_ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_VALUE, (yyvsp[(1) - (3)].str))) {
        Msc::AttributeValues((yyvsp[(1) - (3)].str), csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.arcbase) = msc.AddAttribute(Attribute((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(1) - (3)])), MSC_POS((yylsp[(3) - (3)]))));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 1244 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str), COLOR_OPTIONNAME);
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_EQUAL);
    csh.AddCSH_ErrorAfter((yylsp[(2) - (2)]), "Missing option value.");
    if (csh.CheckHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF, HINT_ATTR_VALUE, (yyvsp[(1) - (2)].str))) {
        Msc::AttributeValues((yyvsp[(1) - (2)].str), csh);
        csh.hintStatus = HINT_READY;
    }
    if (csh.CheckHintAt((yylsp[(1) - (2)]), HINT_ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).end.NextChar(), "Missing option value.");
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 1264 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_DESIGNNAME);
        if (csh.CheckHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_VALUE, "msc")) {
            csh.AddDesignsToHints(true);
            csh.hintStatus = HINT_READY;
        }
        std::string msg = csh.SetDesignTo((yyvsp[(3) - (3)].str), true);
        if (msg.length())
            csh.AddCSH_Error((yylsp[(3) - (3)]), msg.c_str());
  #else
        (yyval.arcbase) = msc.AddAttribute(Attribute("msc", (yyvsp[(3) - (3)].str), MSC_POS((yyloc)), MSC_POS((yylsp[(3) - (3)]))));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 1283 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_EQUAL);
        csh.AddCSH_ErrorAfter((yylsp[(2) - (2)]), "Missing option value.");
        if (csh.CheckHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF, HINT_ATTR_VALUE, "msc")) {
            csh.AddDesignsToHints(true);
            csh.hintStatus = HINT_READY;
        }
        if (csh.CheckHintAt((yylsp[(1) - (2)]), HINT_ATTR_NAME)) {
            csh.AddOptionsToHints();
            csh.hintStatus = HINT_READY;
        }
  #else
    msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).end.NextChar(), "Missing option value.");
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 1303 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_DESIGNNAME);
        if (csh.CheckHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_VALUE, "msc+")) {
            csh.AddDesignsToHints(false);
            csh.hintStatus = HINT_READY;
        }
        std::string msg = csh.SetDesignTo((yyvsp[(3) - (3)].str), false);
        if (msg.length())
            csh.AddCSH_Error((yylsp[(3) - (3)]), msg.c_str());
  #else
        (yyval.arcbase) = msc.AddAttribute(Attribute("msc+", (yyvsp[(3) - (3)].str), MSC_POS((yyloc)), MSC_POS((yylsp[(3) - (3)]))));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 1322 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_EQUAL);
        csh.AddCSH_ErrorAfter((yylsp[(2) - (2)]), "Missing option value.");
        if (csh.CheckHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF, HINT_ATTR_VALUE, "msc+")) {
            csh.AddDesignsToHints(false);
            csh.hintStatus = HINT_READY;
        }
        if (csh.CheckHintAt((yylsp[(1) - (2)]), HINT_ATTR_NAME)) {
            csh.AddOptionsToHints();
            csh.hintStatus = HINT_READY;
        }
  #else
    msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).end.NextChar(), "Missing option value.");
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 1343 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.entitylist) = (EntityDefList*)((yyvsp[(1) - (1)].entitylist));
  #endif
}
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 1349 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    csh.CheckEntityHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]));
  #else
    (yyval.entitylist) = (EntityDefList*)(((yyvsp[(1) - (3)].entitylist))->Append((yyvsp[(3) - (3)].entitylist)));
	delete ((yyvsp[(3) - (3)].entitylist));
  #endif
}
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 1359 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_COMMA);
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.entitylist) = (EntityDefList*)((yyvsp[(1) - (2)].entitylist));
    msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).end.NextChar(), "Expecting an entity here.");
  #endif
}
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 1374 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (2)]));
    csh.AddCSH_EntityName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (2)])))
        EntityDef::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (2)])))
        EntityDef::AttributeValues(csh.hintAttrName, csh);
  #else
    EntityDef *ed = new EntityDef((yyvsp[(1) - (2)].str), &msc);
    ed->SetLineEnd(MSC_POS((yyloc)));
    (yyval.entitylist) = ed->AddAttributeList((yyvsp[(2) - (2)].attriblist), NULL, file_line());
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 1390 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (1)]));
    csh.AddCSH_EntityName((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str));
  #else
    EntityDef *ed = new EntityDef((yyvsp[(1) - (1)].str), &msc);
    ed->SetLineEnd(MSC_POS((yyloc)));
    (yyval.entitylist) = ed->AddAttributeList(NULL, NULL, file_line());
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 1402 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (3)]));
    csh.AddCSH_EntityName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (3)])))
        EntityDef::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (3)])))
        EntityDef::AttributeValues(csh.hintAttrName, csh);
  #else
    EntityDef *ed = new EntityDef((yyvsp[(1) - (3)].str), &msc);
    ed->SetLineEnd(MSC_POS((yyloc)));
    (yyval.entitylist) = ed->AddAttributeList((yyvsp[(2) - (3)].attriblist), (yyvsp[(3) - (3)].arclist), MSC_POS((yylsp[(3) - (3)])).start);
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 1418 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (2)]));
    csh.AddCSH_EntityName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
  #else
    EntityDef *ed = new EntityDef((yyvsp[(1) - (2)].str), &msc);
    ed->SetLineEnd(MSC_POS((yyloc)));
    (yyval.entitylist) = ed->AddAttributeList(NULL, (yyvsp[(2) - (2)].arclist), MSC_POS((yylsp[(2) - (2)])).start);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 1431 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintAt((yylsp[(1) - (2)]), HINT_LINE_START)) {
	    csh.AddLineBeginToHints();
	    csh.hintStatus = HINT_READY;
	} else if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (2)])))
        EntityDef::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (2)])))
        EntityDef::AttributeValues(csh.hintAttrName, csh);
    csh.AddCSH_KeywordOrEntity((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));  //Do it after AddLineBeginToHints so this one is not included
  #else
    EntityDef *ed = new EntityDef((yyvsp[(1) - (2)].str), &msc);
    ed->SetLineEnd(MSC_POS((yyloc)));
    (yyval.entitylist) = ed->AddAttributeList((yyvsp[(2) - (2)].attriblist), NULL, file_line());
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 1449 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  	if (csh.CheckHintAt((yylsp[(1) - (1)]), HINT_LINE_START)) {
	    csh.AddLineBeginToHints();
	    csh.hintStatus = HINT_READY;
	}
    csh.AddCSH_KeywordOrEntity((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str));   //Do it after AddLineBeginToHints so this one is not included
  #else
    EntityDef *ed = new EntityDef((yyvsp[(1) - (1)].str), &msc);
    ed->SetLineEnd(MSC_POS((yyloc)));
    (yyval.entitylist) = ed->AddAttributeList(NULL, NULL, file_line());
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 1464 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintAt((yylsp[(1) - (3)]), HINT_LINE_START)) {
	    csh.AddLineBeginToHints();
	    csh.hintStatus = HINT_READY;
	} else if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (3)])))
        EntityDef::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (3)])))
        EntityDef::AttributeValues(csh.hintAttrName, csh);
    csh.AddCSH_KeywordOrEntity((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));  //Do it after AddLineBeginToHints so this one is not included
  #else
    EntityDef *ed = new EntityDef((yyvsp[(1) - (3)].str), &msc);
    ed->SetLineEnd(MSC_POS((yyloc)));
    (yyval.entitylist) = ed->AddAttributeList((yyvsp[(2) - (3)].attriblist), (yyvsp[(3) - (3)].arclist), MSC_POS((yylsp[(3) - (3)])).start);
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 1482 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  	if (csh.CheckHintAt((yylsp[(1) - (2)]), HINT_LINE_START)) {
	    csh.AddLineBeginToHints();
	    csh.hintStatus = HINT_READY;
	}
    csh.AddCSH_KeywordOrEntity((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));   //Do it after AddLineBeginToHints so this one is not included
  #else
    EntityDef *ed = new EntityDef((yyvsp[(1) - (2)].str), &msc);
    ed->SetLineEnd(MSC_POS((yyloc)));
    (yyval.entitylist) = ed->AddAttributeList(NULL, (yyvsp[(2) - (2)].arclist), MSC_POS((yylsp[(2) - (2)])).start);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 1499 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #endif
}
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 1506 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    for (std::list<std::string>::iterator i = ((yyvsp[(1) - (2)].stringlist))->begin(); i!=((yyvsp[(1) - (2)].stringlist))->end(); i++)
        if (csh.ForbiddenStyles.find(*i) != csh.ForbiddenStyles.end())
            csh.Contexts.back().StyleNames.insert(string(*i));
	if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (2)])))
        MscStyle().AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (2)])))
        MscStyle().AttributeValues(csh.hintAttrName, csh);
  #else
    for (auto a=((yyvsp[(2) - (2)].attriblist))->begin(); a!=((yyvsp[(2) - (2)].attriblist))->end(); a++) {
        std::list<string> problem;
        bool had_generic = false;
        for (auto s = ((yyvsp[(1) - (2)].stringlist))->begin(); s!=((yyvsp[(1) - (2)].stringlist))->end(); s++) {
            MscStyle style = msc.Contexts.back().styles.GetStyle(*s); //may be default style
            if (style.AddAttribute(**a, &msc))
               msc.Contexts.back().styles[*s] = style;
            else {
               problem.push_back(*s);
               had_generic |= (style.type == STYLE_STYLE);
            }
        }
        if (problem.size()==0) continue;
        string msg;
        if (problem.size()==1) {
            if (had_generic)
                msg = "Attribute '" + (*a)->name + "' is not applicable to styles. Ignoring it.";
            else
                msg = "Attribute '" + (*a)->name + "' is not applicable to style '" + *problem.begin() + "'. Ignoring it.";
        } else if (problem.size() == ((yyvsp[(2) - (2)].attriblist))->size()) {
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
    delete((yyvsp[(1) - (2)].stringlist));
    delete((yyvsp[(2) - (2)].attriblist));
  #endif
}
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 1555 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (1)]), COLOR_STYLENAME);
        (yyval.stringlist) = new std::list<string>;
        if (strcmp((yyvsp[(1) - (1)].str), "emphasis")==0)
            ((yyval.stringlist))->push_back("box");
        else if (strcmp((yyvsp[(1) - (1)].str), "emptyemphasis")==0)
            ((yyval.stringlist))->push_back("emptybox");
        else ((yyval.stringlist))->push_back((yyvsp[(1) - (1)].str));
  #else
        (yyval.stringlist) = new std::list<string>;
        ((yyval.stringlist))->push_back(ConvertEmphasisToBox((yyvsp[(1) - (1)].str), &(yylsp[(1) - (1)]), msc));
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 1571 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_STYLENAME);
        (yyval.stringlist) = (yyvsp[(1) - (3)].stringlist);
        if (strcmp((yyvsp[(3) - (3)].str), "emphasis")==0)
            ((yyval.stringlist))->push_back("box");
        else if (strcmp((yyvsp[(3) - (3)].str), "emptyemphasis")==0)
            ((yyval.stringlist))->push_back("emptybox");
        else ((yyval.stringlist))->push_back((yyvsp[(3) - (3)].str));
  #else
        ((yyvsp[(1) - (3)].stringlist))->push_back(ConvertEmphasisToBox((yyvsp[(3) - (3)].str), &(yylsp[(3) - (3)]), msc));
        (yyval.stringlist) = (yyvsp[(1) - (3)].stringlist);
  #endif
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 1590 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #endif
}
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 1597 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_COLORNAME);
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
    csh.AddCSH((yylsp[(3) - (3)]), COLOR_COLORDEF);
    MscColorType color = csh.Contexts.back().Colors.GetColor((yyvsp[(3) - (3)].str));
    if (color.valid)
        csh.Contexts.back().Colors[(yyvsp[(1) - (3)].str)] = color;
  #else
    msc.Contexts.back().colors.AddColor((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), msc.Error, MSC_POS((yyloc)));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 1613 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (5)]), COLOR_DESIGNNAME);
    csh.AddCSH((yylsp[(2) - (5)]), COLOR_BRACE);
    csh.AddCSH((yylsp[(4) - (5)]), COLOR_SEMICOLON);
    csh.AddCSH((yylsp[(5) - (5)]), COLOR_BRACE);
    (csh.Contexts.back().full ? csh.FullDesigns : csh.PartialDesigns)[(yyvsp[(1) - (5)].str)] += csh.Contexts.back();
    csh.PopContext();
  #else
    //cope_open_empty pushed an empty color & style set onto the stack
    //then designelementlist added color & style definitions, now we harvest those
    msc.Designs[(yyvsp[(1) - (5)].str)] += msc.Contexts.back();
    msc.PopContext();
  #endif
    free((yyvsp[(1) - (5)].str));
}
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 1630 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (6)]), COLOR_DESIGNNAME);
    csh.AddCSH((yylsp[(2) - (6)]), COLOR_BRACE);
    csh.AddCSH((yylsp[(4) - (6)]), COLOR_SEMICOLON);
    csh.AddCSH_Error((yylsp[(5) - (6)]), "Could not recognize this as part of a design definition.");
    csh.AddCSH((yylsp[(6) - (6)]), COLOR_BRACE);
    (csh.Contexts.back().full ? csh.FullDesigns : csh.PartialDesigns)[(yyvsp[(1) - (6)].str)] = csh.Contexts.back();
    csh.PopContext();
  #else
    //if closing brace missing, still do the design definition
    //cope_open_empty pushed an empty color & style set onto the stack
    //then designelementlist added color & style definitions, now we harvest those
    msc.Designs[(yyvsp[(1) - (6)].str)] += msc.Contexts.back();
    msc.PopContext();
  #endif
}
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 1650 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.PushContext(true);
  #else
    //push empty color & style sets for design definition
    msc.PushContext(true);
  #endif
}
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 1661 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SEMICOLON);
  #endif
}
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 1668 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 1675 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 1685 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #endif
}
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 1691 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[(2) - (2)]), "Extra stuff after design options. Maybe missing a comma?");
  #endif
}
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 1698 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_OPTIONNAME);
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
    csh.AddCSH((yylsp[(3) - (3)]), COLOR_ATTRVALUE);
    if (csh.CheckHintAt((yylsp[(1) - (3)]), HINT_ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_VALUE, (yyvsp[(1) - (3)].str))) {
        Msc::AttributeValues((yyvsp[(1) - (3)].str), csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.AddDesignAttribute(Attribute((yyvsp[(1) - (3)].str), str2bool((yyvsp[(3) - (3)].str)), MSC_POS((yyloc)), MSC_POS((yylsp[(3) - (3)])), (yyvsp[(3) - (3)].str)));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 1717 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_OPTIONNAME);
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
    csh.AddCSH((yylsp[(3) - (3)]), COLOR_ATTRVALUE);
    if (csh.CheckHintAt((yylsp[(1) - (3)]), HINT_ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_VALUE, (yyvsp[(1) - (3)].str))) {
        Msc::AttributeValues((yyvsp[(1) - (3)].str), csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.AddDesignAttribute(Attribute((yyvsp[(1) - (3)].str), atof((yyvsp[(3) - (3)].str)), MSC_POS((yyloc)), MSC_POS((yylsp[(3) - (3)])), (yyvsp[(3) - (3)].str)));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 1736 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_OPTIONNAME);
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
    csh.AddCSH((yylsp[(3) - (3)]), COLOR_ATTRVALUE);
    if (csh.CheckHintAt((yylsp[(1) - (3)]), HINT_ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_VALUE, (yyvsp[(1) - (3)].str))) {
        Msc::AttributeValues((yyvsp[(1) - (3)].str), csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.AddDesignAttribute(Attribute((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(1) - (3)])), MSC_POS((yylsp[(3) - (3)]))));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 1755 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
    csh.AddCSH((yylsp[(3) - (3)]), COLOR_DESIGNNAME);
    if (csh.CheckHintAt((yylsp[(1) - (3)]), HINT_ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_VALUE, (yyvsp[(1) - (3)].str))) {
        Msc::AttributeValues("msc", csh);
        csh.hintStatus = HINT_READY;
    }
    std::string msg = csh.SetDesignTo((yyvsp[(3) - (3)].str), true);
    if (msg.length())
        csh.AddCSH_Error((yylsp[(3) - (3)]), msg.c_str());
  #else
    msc.AddDesignAttribute(Attribute("msc", (yyvsp[(3) - (3)].str), MSC_POS((yyloc)), MSC_POS((yylsp[(3) - (3)]))));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 1777 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
    csh.AddCSH((yylsp[(3) - (3)]), COLOR_DESIGNNAME);
    if (csh.CheckHintAt((yylsp[(1) - (3)]), HINT_ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_VALUE, (yyvsp[(1) - (3)].str))) {
        Msc::AttributeValues("msc+", csh);
        csh.hintStatus = HINT_READY;
    }
    std::string msg = csh.SetDesignTo((yyvsp[(3) - (3)].str), false);
    if (msg.length())
        csh.AddCSH_Error((yylsp[(3) - (3)]), msg.c_str());
  #else
    msc.AddDesignAttribute(Attribute("msc+", (yyvsp[(3) - (3)].str), MSC_POS((yyloc)), MSC_POS((yylsp[(3) - (3)]))));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 1801 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(1) - (1)].arclist))
        (yyval.arcparallel) = (new ArcParallel(&msc))->AddArcList((yyvsp[(1) - (1)].arclist));
    else
        (yyval.arcparallel) = NULL;
  #endif
}
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 1810 "language.yy"
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

  case 142:

/* Line 1806 of yacc.c  */
#line 1822 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.arcboxseries) = new ArcBoxSeries((yyvsp[(1) - (1)].arcbox));
    ((yyvsp[(1) - (1)].arcbox))->MakeMeLastNotable();
  #endif
}
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 1830 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(2) - (2)].arcbox))->SetLineEnd(MSC_POS((yylsp[(2) - (2)])));
    (yyval.arcboxseries) = ((yyvsp[(1) - (2)].arcboxseries))->AddFollow((yyvsp[(2) - (2)].arcbox));
    ((yyvsp[(2) - (2)].arcbox))->AddAttributeList(NULL); //should come after AddFollow
    ((yyvsp[(1) - (2)].arcboxseries))->MakeMeLastNotable();
  #endif
}
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 1839 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(3) - (3)])))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(3) - (3)])))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
  #else
    ((yyvsp[(2) - (3)].arcbox))->SetLineEnd(MSC_POS2((yylsp[(2) - (3)]), (yylsp[(3) - (3)])));
    (yyval.arcboxseries) = ((yyvsp[(1) - (3)].arcboxseries))->AddFollow((yyvsp[(2) - (3)].arcbox));
    ((yyvsp[(2) - (3)].arcbox))->AddAttributeList((yyvsp[(3) - (3)].attriblist)); //should come after AddFollow
    ((yyvsp[(1) - (3)].arcboxseries))->MakeMeLastNotable();
  #endif
}
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 1853 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(2) - (3)].arcbox))->AddArcList((yyvsp[(3) - (3)].arclist))->SetLineEnd(MSC_POS((yylsp[(2) - (3)])));
    (yyval.arcboxseries) = ((yyvsp[(1) - (3)].arcboxseries))->AddFollow((yyvsp[(2) - (3)].arcbox));
    ((yyvsp[(2) - (3)].arcbox))->AddAttributeList(NULL); //should come after AddFollow
    ((yyvsp[(1) - (3)].arcboxseries))->MakeMeLastNotable();
  #endif
}
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 1862 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ArcBox *temp = new ArcBox(MSC_EMPH_UNDETERMINED_FOLLOW, NULL, MSC_POS((yylsp[(1) - (2)])), NULL, MSC_POS((yylsp[(1) - (2)])), &msc);
    temp->AddArcList((yyvsp[(2) - (2)].arclist));
    (yyval.arcboxseries) = ((yyvsp[(1) - (2)].arcboxseries))->AddFollow(temp);
    temp->AddAttributeList(NULL); //should come after AddFollow
    ((yyvsp[(1) - (2)].arcboxseries))->MakeMeLastNotable();
  #endif
}
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 1872 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (4)])))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (4)])))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
  #else
    ((yyvsp[(2) - (4)].arcbox))->AddArcList((yyvsp[(4) - (4)].arclist))->SetLineEnd(MSC_POS2((yylsp[(2) - (4)]), (yylsp[(3) - (4)])));
    (yyval.arcboxseries) = ((yyvsp[(1) - (4)].arcboxseries))->AddFollow((yyvsp[(2) - (4)].arcbox));
    ((yyvsp[(2) - (4)].arcbox))->AddAttributeList((yyvsp[(3) - (4)].attriblist)); //should come after AddFollow
    ((yyvsp[(1) - (4)].arcboxseries))->MakeMeLastNotable();
  #endif
}
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 1886 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (3)])))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (3)])))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
  #else
    ArcBox *temp = new ArcBox(MSC_EMPH_UNDETERMINED_FOLLOW, NULL, MSC_POS((yylsp[(1) - (3)])), NULL, MSC_POS((yylsp[(1) - (3)])), &msc);
    temp->AddArcList((yyvsp[(3) - (3)].arclist))->SetLineEnd(MSC_POS((yylsp[(2) - (3)])));
    (yyval.arcboxseries) = ((yyvsp[(1) - (3)].arcboxseries))->AddFollow(temp);
    temp->AddAttributeList((yyvsp[(2) - (3)].attriblist)); //should come after AddFollow
    ((yyvsp[(1) - (3)].arcboxseries))->MakeMeLastNotable();
  #endif
}
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 1903 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(1) - (1)].arcbox))->AddAttributeList(NULL);
    ((yyvsp[(1) - (1)].arcbox))->SetLineEnd(MSC_POS((yyloc)));
    (yyval.arcbox) = (yyvsp[(1) - (1)].arcbox);
  #endif
}
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 1911 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (2)])))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (2)])))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
  #else
    ((yyvsp[(1) - (2)].arcbox))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
    ((yyvsp[(1) - (2)].arcbox))->SetLineEnd(MSC_POS((yyloc)));
    (yyval.arcbox) = ((yyvsp[(1) - (2)].arcbox));
  #endif
}
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 1924 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(1) - (2)].arcbox))->AddAttributeList(NULL);
    ((yyvsp[(1) - (2)].arcbox))->SetLineEnd(MSC_POS((yylsp[(1) - (2)])));
    (yyval.arcbox) = ((yyvsp[(1) - (2)].arcbox))->AddArcList((yyvsp[(2) - (2)].arclist));
  #endif
}
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 1932 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (3)])))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (3)])))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
  #else
    ((yyvsp[(1) - (3)].arcbox))->SetLineEnd(MSC_POS2((yylsp[(1) - (3)]), (yylsp[(2) - (3)])));
    ((yyvsp[(1) - (3)].arcbox))->AddArcList((yyvsp[(3) - (3)].arclist))->AddAttributeList((yyvsp[(2) - (3)].attriblist));
    (yyval.arcbox) = ((yyvsp[(1) - (3)].arcbox));
  #endif
}
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 1947 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    if (csh.CheckHintBetweenPlusOne((yylsp[(1) - (2)]), (yylsp[(2) - (2)]), HINT_ENTITY)) {
        csh.hintStatus = HINT_READY;
        csh.AddEntitiesToHints();
    }
  #else
    (yyval.arcpipe) = new ArcPipe((yyvsp[(2) - (2)].arcbox));
    ((yyval.arcpipe))->AddAttributeList(NULL)->SetLineEnd(MSC_POS((yyloc)));
    ((yyval.arcpipe))->MakeMeLastNotable();
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 1962 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    csh.AddCSH_EntityName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
    if (csh.CheckHintAtAndBeforePlusOne((yylsp[(1) - (2)]), (yylsp[(2) - (2)]), HINT_ENTITY)) {
        csh.hintStatus = HINT_READY;
        csh.AddEntitiesToHints();
    }
    csh.AddCSH_ErrorAfter((yylsp[(2) - (2)]), "Missing a box symbol.");
  #else
    msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).end.NextChar(), "Missing a box symbol. Ignoring pipe.");
    (yyval.arcpipe) = NULL;
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 1978 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    csh.CheckEntityHintAfterPlusOne((yylsp[(1) - (1)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcpipe) = NULL;
    msc.Error.Error(MSC_POS((yylsp[(1) - (1)])).end.NextChar(), "The keyword '" + string((yyvsp[(1) - (1)].str)) +"' should be followed by an entity, or '--', '..', '++' or '=='.");
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 1989 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    if (csh.CheckHintBetweenPlusOne((yylsp[(1) - (3)]), (yylsp[(2) - (3)]), HINT_ENTITY)) {
        csh.hintStatus = HINT_READY;
        csh.AddEntitiesToHints();
    } else if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(3) - (3)])))
        ArcPipe::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(3) - (3)])))
        ArcPipe::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcpipe) = new ArcPipe((yyvsp[(2) - (3)].arcbox));
    ((yyval.arcpipe))->AddAttributeList((yyvsp[(3) - (3)].attriblist))->SetLineEnd(MSC_POS((yyloc)));
    ((yyval.arcpipe))->MakeMeLastNotable();
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 2008 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcpipeseries) = new ArcPipeSeries((yyvsp[(1) - (1)].arcpipe));
  #endif
}
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 2015 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ArcPipe *ap = new ArcPipe((yyvsp[(2) - (2)].arcbox));
    ap->MakeMeLastNotable();
    ap->SetLineEnd(MSC_POS((yylsp[(2) - (2)])));
    (yyval.arcpipeseries) = ((yyvsp[(1) - (2)].arcpipeseries))->AddFollowWithAttributes(ap, NULL);
  #endif
}
    break;

  case 159:

/* Line 1806 of yacc.c  */
#line 2024 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(3) - (3)])))
        ArcPipe::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(3) - (3)])))
        ArcPipe::AttributeValues(csh.hintAttrName, csh);
  #else
    ArcPipe *ap = new ArcPipe((yyvsp[(2) - (3)].arcbox));
    ap->MakeMeLastNotable();
    ap->SetLineEnd(MSC_POS((yylsp[(2) - (3)])));
    (yyval.arcpipeseries) = ((yyvsp[(1) - (3)].arcpipeseries))->AddFollowWithAttributes(ap, (yyvsp[(3) - (3)].attriblist));
  #endif
}
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 2040 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.arcpipeseries) = ((yyvsp[(1) - (2)].arcpipeseries))->AddArcList((yyvsp[(2) - (2)].arclist));
  #endif
}
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 2047 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (3)]));
    csh.AddCSH_EntityName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]));
    csh.AddCSH_EntityName((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
  #else
    (yyval.arcbox) = new ArcBox((yyvsp[(2) - (3)].arctype), (yyvsp[(1) - (3)].str), MSC_POS((yylsp[(1) - (3)])), (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(3) - (3)])), &msc);
    ((yyval.arcbox))->MakeMeLastNotable();
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 2062 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
    csh.AddCSH_EntityName((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
  #else
    (yyval.arcbox) = new ArcBox((yyvsp[(1) - (2)].arctype), NULL, MSC_POS((yylsp[(1) - (2)])), (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])), &msc);
    ((yyval.arcbox))->MakeMeLastNotable();
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 164:

/* Line 1806 of yacc.c  */
#line 2074 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (2)]));
    csh.AddCSH_EntityName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcbox) = new ArcBox((yyvsp[(2) - (2)].arctype), (yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), NULL, MSC_POS((yylsp[(2) - (2)])), &msc);
    ((yyval.arcbox))->MakeMeLastNotable();
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 165:

/* Line 1806 of yacc.c  */
#line 2087 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcbox) = new ArcBox((yyvsp[(1) - (1)].arctype), NULL, MSC_POS((yylsp[(1) - (1)])), NULL, MSC_POS((yylsp[(1) - (1)])), &msc);
    ((yyval.arcbox))->MakeMeLastNotable();
  #endif
}
    break;

  case 166:

/* Line 1806 of yacc.c  */
#line 2098 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
    csh.AddCSH_EntityName((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
  #else
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])));
  #endif
    free((yyvsp[(1) - (2)].str));
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 167:

/* Line 1806 of yacc.c  */
#line 2110 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[(1) - (3)]), (yylsp[(2) - (3)]));
    csh.AddCSH_EntityName((yylsp[(2) - (3)]), (yyvsp[(2) - (3)].str));
    csh.AddCSH((yylsp[(3) - (3)]), COLOR_ATTRVALUE);
  #else
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[(2) - (3)].str), MSC_POS((yylsp[(2) - (3)])), VertXPos::POS_AT, atof((yyvsp[(3) - (3)].str)));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 168:

/* Line 1806 of yacc.c  */
#line 2124 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[(1) - (3)]), (yylsp[(2) - (3)]));
    csh.AddCSH_EntityName((yylsp[(2) - (3)]), (yyvsp[(2) - (3)].str));
    csh.AddCSH((yylsp[(3) - (3)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(3) - (3)]), yylloc, yychar==YYEOF);
  #else
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[(2) - (3)].str), MSC_POS((yylsp[(2) - (3)])), VertXPos::POS_LEFT_SIDE);
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 169:

/* Line 1806 of yacc.c  */
#line 2138 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (4)]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[(1) - (4)]), (yylsp[(2) - (4)]));
    csh.AddCSH_EntityName((yylsp[(2) - (4)]), (yyvsp[(2) - (4)].str));
    csh.AddCSH((yylsp[(3) - (4)]), COLOR_SYMBOL);
    csh.AddCSH((yylsp[(4) - (4)]), COLOR_ATTRVALUE);
  #else
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[(2) - (4)].str), MSC_POS((yylsp[(2) - (4)])), VertXPos::POS_LEFT_SIDE, -atof((yyvsp[(4) - (4)].str)));
  #endif
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(2) - (4)].str));
    free((yyvsp[(4) - (4)].str));
}
    break;

  case 170:

/* Line 1806 of yacc.c  */
#line 2153 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[(1) - (3)]), (yylsp[(2) - (3)]));
    csh.AddCSH_EntityName((yylsp[(2) - (3)]), (yyvsp[(2) - (3)].str));
    csh.AddCSH((yylsp[(3) - (3)]), COLOR_SYMBOL);
  #else
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[(2) - (3)].str), MSC_POS((yylsp[(2) - (3)])), VertXPos::POS_RIGHT_SIDE);
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 171:

/* Line 1806 of yacc.c  */
#line 2166 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (4)]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[(1) - (4)]), (yylsp[(2) - (4)]));
    csh.AddCSH_EntityName((yylsp[(2) - (4)]), (yyvsp[(2) - (4)].str));
    csh.AddCSH((yylsp[(3) - (4)]), COLOR_SYMBOL);
    csh.AddCSH((yylsp[(4) - (4)]), COLOR_ATTRVALUE);
  #else
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[(2) - (4)].str), MSC_POS((yylsp[(2) - (4)])), VertXPos::POS_RIGHT_SIDE, atof((yyvsp[(4) - (4)].str)));
  #endif
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(2) - (4)].str));
    free((yyvsp[(4) - (4)].str));
}
    break;

  case 172:

/* Line 1806 of yacc.c  */
#line 2181 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[(1) - (3)]), (yylsp[(2) - (3)]));
    csh.AddCSH_EntityName((yylsp[(2) - (3)]), (yyvsp[(2) - (3)].str));
    csh.AddCSH((yylsp[(3) - (3)]), COLOR_SYMBOL);
  #else
    switch ((yyvsp[(3) - (3)].arctype)) {
    default:
        _ASSERT(0);
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

  case 173:

/* Line 1806 of yacc.c  */
#line 2215 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (4)]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[(1) - (4)]), (yylsp[(2) - (4)]));
    csh.AddCSH_EntityName((yylsp[(2) - (4)]), (yyvsp[(2) - (4)].str));
    csh.AddCSH((yylsp[(3) - (4)]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[(3) - (4)]), (yylsp[(4) - (4)]));
    csh.AddCSH_EntityName((yylsp[(4) - (4)]), (yyvsp[(4) - (4)].str));
  #else
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[(2) - (4)].str), MSC_POS((yylsp[(2) - (4)])), (yyvsp[(4) - (4)].str), MSC_POS((yylsp[(4) - (4)])), VertXPos::POS_CENTER);
  #endif
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(2) - (4)].str));
    free((yyvsp[(4) - (4)].str));
}
    break;

  case 174:

/* Line 1806 of yacc.c  */
#line 2231 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (5)]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[(1) - (5)]), (yylsp[(2) - (5)]));
    csh.AddCSH_EntityName((yylsp[(2) - (5)]), (yyvsp[(2) - (5)].str));
    csh.AddCSH((yylsp[(3) - (5)]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[(3) - (5)]), (yylsp[(4) - (5)]));
    csh.AddCSH_EntityName((yylsp[(4) - (5)]), (yyvsp[(4) - (5)].str));
    csh.AddCSH((yylsp[(5) - (5)]), COLOR_ATTRVALUE);
  #else
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[(2) - (5)].str), MSC_POS((yylsp[(2) - (5)])), (yyvsp[(4) - (5)].str), MSC_POS((yylsp[(4) - (5)])), VertXPos::POS_CENTER, atof((yyvsp[(5) - (5)].str)));
  #endif
    free((yyvsp[(1) - (5)].str));
    free((yyvsp[(2) - (5)].str));
    free((yyvsp[(4) - (5)].str));
    free((yyvsp[(5) - (5)].str));
}
    break;

  case 175:

/* Line 1806 of yacc.c  */
#line 2249 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    csh.CheckEntityHintAfterPlusOne((yylsp[(1) - (1)]), yylloc, yychar==YYEOF);
    csh.AddCSH_ErrorAfter((yylsp[(1) - (1)]), "Missing an entity name.");
  #else
    (yyval.vertxpos) = NULL;
    msc.Error.Error(MSC_POS((yylsp[(1) - (1)])).end.NextChar(), "Missing an entity name.");
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 176:

/* Line 1806 of yacc.c  */
#line 2261 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (CaseInsensitiveBeginsWith("at", (yyvsp[(1) - (1)].str)) && csh.cursor_pos == ((yylsp[(1) - (1)])).last_pos)
        csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD_PARTIAL);
    if (csh.CheckHintAt((yylsp[(1) - (1)]), HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "at", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.vertxpos) = NULL;
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 180:

/* Line 1806 of yacc.c  */
#line 2279 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_MARKERNAME);
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    csh.AddCSH((yylsp[(3) - (3)]), COLOR_MARKERNAME);
    csh.CheckHintAt((yylsp[(1) - (3)]), HINT_MARKER);
    csh.CheckHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[(2) - (3)].arctype), (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), &msc);
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 2293 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_MARKERNAME);
    csh.CheckHintAtAndBefore((yylsp[(1) - (2)]), (yylsp[(2) - (2)]), HINT_MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[(1) - (2)].arctype), MARKER_HERE_STR, (yyvsp[(2) - (2)].str), &msc);
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 182:

/* Line 1806 of yacc.c  */
#line 2304 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_MARKERNAME);
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckHintAt((yylsp[(1) - (2)]), HINT_MARKER);
    csh.CheckHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[(2) - (2)].arctype), (yyvsp[(1) - (2)].str), MARKER_HERE_STR, &msc);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 2316 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
    csh.CheckHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[(1) - (1)].arctype), MARKER_HERE_STR, MARKER_PREV_PARALLEL_STR, &msc);
  #endif
}
    break;

  case 184:

/* Line 1806 of yacc.c  */
#line 2325 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_MARKERNAME);
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    csh.AddCSH((yylsp[(3) - (3)]), COLOR_MARKERNAME);
    csh.CheckHintAt((yylsp[(1) - (3)]), HINT_MARKER);
    csh.CheckHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[(2) - (3)].arctype), (yyvsp[(3) - (3)].str), (yyvsp[(1) - (3)].str), &msc);
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 185:

/* Line 1806 of yacc.c  */
#line 2339 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_MARKERNAME);
    csh.CheckHintAtAndBefore((yylsp[(1) - (2)]), (yylsp[(2) - (2)]), HINT_MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[(1) - (2)].arctype), (yyvsp[(2) - (2)].str), MARKER_HERE_STR, &msc);
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 186:

/* Line 1806 of yacc.c  */
#line 2350 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_MARKERNAME);
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckHintAt((yylsp[(1) - (2)]), HINT_MARKER);
    csh.CheckHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[(2) - (2)].arctype), MARKER_HERE_STR, (yyvsp[(1) - (2)].str), &msc);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 187:

/* Line 1806 of yacc.c  */
#line 2362 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
    csh.CheckHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[(1) - (1)].arctype), MARKER_PREV_PARALLEL_STR, MARKER_HERE_STR, &msc);
  #endif
}
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 2371 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_MARKERNAME);
    csh.CheckHintAt((yylsp[(1) - (1)]), HINT_MARKER);
    csh.AddCSH_ErrorAfter((yylsp[(1) - (1)]), "Missing a box or arrow symbol.");
  #else
    (yyval.arcvertarrow) = NULL;
    msc.Error.Error(MSC_POS((yylsp[(1) - (1)])).end.NextChar(), "Missing a box or arrow symbol.");
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 2383 "language.yy"
    {free((yyvsp[(2) - (3)].str)); (yyval.arcvertarrow)=NULL;}
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 2384 "language.yy"
    {free((yyvsp[(1) - (4)].str)); free((yyvsp[(3) - (4)].str)); (yyval.arcvertarrow)=NULL;}
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 2385 "language.yy"
    {free((yyvsp[(2) - (3)].str)); (yyval.arcvertarrow)=NULL;}
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 2386 "language.yy"
    {free((yyvsp[(1) - (4)].str)); free((yyvsp[(3) - (4)].str)); (yyval.arcvertarrow)=NULL;}
    break;

  case 193:

/* Line 1806 of yacc.c  */
#line 2390 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    if ((yyvsp[(1) - (2)].arcvertarrow))
        (yyval.arcbase) = ((yyvsp[(1) - (2)].arcvertarrow))->AddXpos((yyvsp[(2) - (2)].vertxpos));
    else
        (yyval.arcbase) = NULL;
    delete (yyvsp[(2) - (2)].vertxpos);
  #endif
}
    break;

  case 194:

/* Line 1806 of yacc.c  */
#line 2401 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ErrorAfter((yylsp[(1) - (1)]), "Missing the 'at' keyword.");
  #else
    (yyval.arcbase) = NULL;
    msc.Error.Error(MSC_POS((yylsp[(1) - (1)])).end.NextChar(), "Missing the 'at' keyword.");
    if ((yyvsp[(1) - (1)].arcvertarrow)) delete (yyvsp[(1) - (1)].arcvertarrow);
  #endif
}
    break;

  case 195:

/* Line 1806 of yacc.c  */
#line 2412 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
  #else
    (yyval.arcbase) = new ArcDivider((yyvsp[(1) - (1)].arctype), &msc);
  #endif
}
    break;

  case 196:

/* Line 1806 of yacc.c  */
#line 2420 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 197:

/* Line 1806 of yacc.c  */
#line 2424 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 2428 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 199:

/* Line 1806 of yacc.c  */
#line 2433 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (3)]));
    csh.AddCSH_EntityName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]));
    csh.AddCSH_EntityName((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (3)].arctype), (yyvsp[(1) - (3)].str), MSC_POS((yylsp[(1) - (3)])), (yyvsp[(3) - (3)].str), true, MSC_POS((yylsp[(3) - (3)])));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 2447 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
    csh.AddCSH_EntityName((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(1) - (2)].arctype), LSIDE_ENT_STR, MSC_POS((yylsp[(1) - (2)])), (yyvsp[(2) - (2)].str), true, MSC_POS((yylsp[(2) - (2)])));
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 201:

/* Line 1806 of yacc.c  */
#line 2458 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (2)]));
    csh.AddCSH_EntityName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (2)].arctype), (yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), RSIDE_ENT_STR, true, MSC_POS((yylsp[(2) - (2)])));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 2470 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    csh.CheckEntityHintAt((yylsp[(3) - (3)]));
    csh.AddCSH_EntityName((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (3)].arcarrow))->AddSegment((yyvsp[(2) - (3)].arctype), (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(3) - (3)])), MSC_POS2((yylsp[(2) - (3)]), (yylsp[(3) - (3)])));
  #endif
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 2481 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(1) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment((yyvsp[(2) - (2)].arctype), NULL, MSC_POS((yylsp[(2) - (2)])), MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 204:

/* Line 1806 of yacc.c  */
#line 2492 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (3)]));
    csh.AddCSH_EntityName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]));
    csh.AddCSH_EntityName((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (3)].arctype), (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(3) - (3)])), (yyvsp[(1) - (3)].str), false, MSC_POS((yylsp[(1) - (3)])));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 205:

/* Line 1806 of yacc.c  */
#line 2506 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
    csh.AddCSH_EntityName((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(1) - (2)].arctype), (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])), LSIDE_ENT_STR, false, MSC_POS((yylsp[(1) - (2)])));
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 2517 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (2)]));
    csh.AddCSH_EntityName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (2)].arctype), RSIDE_ENT_STR, MSC_POS((yylsp[(2) - (2)])), (yyvsp[(1) - (2)].str), false, MSC_POS((yylsp[(1) - (2)])));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 2529 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]));
    csh.AddCSH_EntityName((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (3)].arcarrow))->AddSegment((yyvsp[(2) - (3)].arctype), (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(3) - (3)])), MSC_POS2((yylsp[(2) - (3)]), (yylsp[(3) - (3)])));
  #endif
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 208:

/* Line 1806 of yacc.c  */
#line 2540 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment((yyvsp[(2) - (2)].arctype), NULL, MSC_POS((yylsp[(2) - (2)])), MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 209:

/* Line 1806 of yacc.c  */
#line 2550 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (3)]));
    csh.AddCSH_EntityName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]));
    csh.AddCSH_EntityName((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (3)].arctype), (yyvsp[(1) - (3)].str), MSC_POS((yylsp[(1) - (3)])), (yyvsp[(3) - (3)].str), true, MSC_POS((yylsp[(3) - (3)])));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 210:

/* Line 1806 of yacc.c  */
#line 2564 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
    csh.AddCSH_EntityName((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(1) - (2)].arctype), LSIDE_ENT_STR, MSC_POS((yylsp[(1) - (2)])), (yyvsp[(2) - (2)].str), true, MSC_POS((yylsp[(2) - (2)])));
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 211:

/* Line 1806 of yacc.c  */
#line 2575 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (2)]));
    csh.AddCSH_EntityName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==EOF);
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (2)].arctype), (yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), RSIDE_ENT_STR, true, MSC_POS((yylsp[(2) - (2)])));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 212:

/* Line 1806 of yacc.c  */
#line 2587 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]));
    csh.AddCSH_EntityName((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (3)].arcarrow))->AddSegment((yyvsp[(2) - (3)].arctype), (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(3) - (3)])), MSC_POS2((yylsp[(2) - (3)]), (yylsp[(3) - (3)])));
  #endif
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 213:

/* Line 1806 of yacc.c  */
#line 2598 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment((yyvsp[(2) - (2)].arctype), NULL, MSC_POS((yylsp[(2) - (2)])), MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 227:

/* Line 1806 of yacc.c  */
#line 2611 "language.yy"
    {(yyval.arctype)=MSC_ARC_UNDETERMINED_SEGMENT;}
    break;

  case 229:

/* Line 1806 of yacc.c  */
#line 2612 "language.yy"
    {(yyval.arctype)=MSC_ARC_UNDETERMINED_SEGMENT;}
    break;

  case 231:

/* Line 1806 of yacc.c  */
#line 2613 "language.yy"
    {(yyval.arctype)=MSC_ARC_UNDETERMINED_SEGMENT;}
    break;

  case 233:

/* Line 1806 of yacc.c  */
#line 2617 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ExtvxposDesignatorName((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str));
    if (csh.hintStatus != HINT_READY &&
        csh.CheckHintAt((yylsp[(1) - (1)]), HINT_KEYWORD)) {
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
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 234:

/* Line 1806 of yacc.c  */
#line 2640 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ExtvxposDesignatorName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));

  #else
    (yyval.extvertxpos) = new ExtVertXPos((yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), (yyvsp[(2) - (2)].vertxpos));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 235:

/* Line 1806 of yacc.c  */
#line 2650 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ExtvxposDesignatorName((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str));
    if (csh.CheckHintAfterPlusOne((yylsp[(1) - (1)]), yylloc, yychar==YYEOF, HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "at", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.extvertxpos) = NULL;
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 238:

/* Line 1806 of yacc.c  */
#line 2666 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (4)]), COLOR_KEYWORD);
    if (CaseInsensitiveEqual((yyvsp[(2) - (4)].str), "arc") || CaseInsensitiveEqual((yyvsp[(2) - (4)].str), "rectangle") ||
        CaseInsensitiveEqual((yyvsp[(2) - (4)].str), "..."))
        csh.AddCSH((yylsp[(2) - (4)]), COLOR_KEYWORD);
    else if (CaseInsensitiveBeginsWith("arc", (yyvsp[(2) - (4)].str))==0 &&
             CaseInsensitiveBeginsWith("rectangle", (yyvsp[(2) - (4)].str))==0 &&
             CaseInsensitiveBeginsWith("...", (yyvsp[(2) - (4)].str))==0)
        csh.AddCSH_Error((yylsp[(2) - (4)]), "Bad symbol name. Use 'arc', '...' or 'rectangle'.");
    if (csh.CheckHintAtAndBefore((yylsp[(1) - (4)]), (yylsp[(2) - (4)]), HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
    if (csh.hintStatus != HINT_READY &&
        csh.CheckHintAfterPlusOne((yylsp[(4) - (4)]), yylloc, yychar==YYEOF, HINT_KEYWORD)) {
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
    (yyval.arcbase) = new CommandSymbol(&msc, (yyvsp[(2) - (4)].str), (yyvsp[(3) - (4)].namerel), (yyvsp[(4) - (4)].extvertxpos), NULL);
  #endif
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(2) - (4)].str));
}
    break;

  case 239:

/* Line 1806 of yacc.c  */
#line 2702 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (5)]), COLOR_KEYWORD);
    if (CaseInsensitiveEqual((yyvsp[(2) - (5)].str), "arc") || CaseInsensitiveEqual((yyvsp[(2) - (5)].str), "rectangle") ||
        CaseInsensitiveEqual((yyvsp[(2) - (5)].str), "..."))
        csh.AddCSH((yylsp[(2) - (5)]), COLOR_KEYWORD);
    else if (CaseInsensitiveBeginsWith("arc", (yyvsp[(2) - (5)].str))==0 &&
             CaseInsensitiveBeginsWith("rectangle", (yyvsp[(2) - (5)].str))==0 &&
             CaseInsensitiveBeginsWith("...", (yyvsp[(2) - (5)].str))==0)
        csh.AddCSH_Error((yylsp[(2) - (5)]), "Bad symbol name. Use 'arc', '...' or 'rectangle'.");
    if (csh.CheckHintAtAndBefore((yylsp[(1) - (5)]), (yylsp[(2) - (5)]), HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.arcbase) = new CommandSymbol(&msc, (yyvsp[(2) - (5)].str), (yyvsp[(3) - (5)].namerel), (yyvsp[(4) - (5)].extvertxpos), (yyvsp[(5) - (5)].extvertxpos));
  #endif
    free((yyvsp[(1) - (5)].str));
    free((yyvsp[(2) - (5)].str));
}
    break;

  case 240:

/* Line 1806 of yacc.c  */
#line 2725 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    if (CaseInsensitiveEqual((yyvsp[(2) - (3)].str), "arc") || CaseInsensitiveEqual((yyvsp[(2) - (3)].str), "rectangle") ||
        CaseInsensitiveEqual((yyvsp[(2) - (3)].str), "..."))
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_KEYWORD);
    else if (CaseInsensitiveBeginsWith("arc", (yyvsp[(2) - (3)].str))==0 &&
             CaseInsensitiveBeginsWith("rectangle", (yyvsp[(2) - (3)].str))==0 &&
             CaseInsensitiveBeginsWith("...", (yyvsp[(2) - (3)].str))==0)
        csh.AddCSH_Error((yylsp[(2) - (3)]), "Bad symbol name. Use 'arc', '...' or 'rectangle'.");
    if (csh.CheckHintAtAndBefore((yylsp[(1) - (3)]), (yylsp[(2) - (3)]), HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
    if (csh.hintStatus != HINT_READY &&
        csh.CheckHintAfterPlusOne((yylsp[(3) - (3)]), yylloc, yychar==YYEOF, HINT_KEYWORD)) {
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
    (yyval.arcbase) = new CommandSymbol(&msc, (yyvsp[(2) - (3)].str), NULL, (yyvsp[(3) - (3)].extvertxpos), NULL);
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 241:

/* Line 1806 of yacc.c  */
#line 2761 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (4)]), COLOR_KEYWORD);
    if (CaseInsensitiveEqual((yyvsp[(2) - (4)].str), "arc") || CaseInsensitiveEqual((yyvsp[(2) - (4)].str), "rectangle") ||
        CaseInsensitiveEqual((yyvsp[(2) - (4)].str), "..."))
        csh.AddCSH((yylsp[(2) - (4)]), COLOR_KEYWORD);
    else if (CaseInsensitiveBeginsWith("arc", (yyvsp[(2) - (4)].str))==0 &&
             CaseInsensitiveBeginsWith("rectangle", (yyvsp[(2) - (4)].str))==0 &&
             CaseInsensitiveBeginsWith("...", (yyvsp[(2) - (4)].str))==0)
        csh.AddCSH_Error((yylsp[(2) - (4)]), "Bad symbol name. Use 'arc', '...' or 'rectangle'.");
    if (csh.CheckHintAtAndBefore((yylsp[(1) - (4)]), (yylsp[(2) - (4)]), HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
    csh.CheckHintBetween((yylsp[(2) - (4)]), (yylsp[(3) - (4)]), HINT_MARKER);
  #else
    (yyval.arcbase) = new CommandSymbol(&msc, (yyvsp[(2) - (4)].str), NULL, (yyvsp[(3) - (4)].extvertxpos), (yyvsp[(4) - (4)].extvertxpos));
  #endif
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(2) - (4)].str));
}
    break;

  case 242:

/* Line 1806 of yacc.c  */
#line 2785 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    if (CaseInsensitiveEqual((yyvsp[(2) - (3)].str), "arc") || CaseInsensitiveEqual((yyvsp[(2) - (3)].str), "rectangle") ||
        CaseInsensitiveEqual((yyvsp[(2) - (3)].str), "..."))
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_KEYWORD);
    else if (CaseInsensitiveBeginsWith("arc", (yyvsp[(2) - (3)].str))==0 &&
             CaseInsensitiveBeginsWith("rectangle", (yyvsp[(2) - (3)].str))==0 &&
             CaseInsensitiveBeginsWith("...", (yyvsp[(2) - (3)].str))==0)
        csh.AddCSH_Error((yylsp[(2) - (3)]), "Bad symbol name. Use 'arc', '...' or 'rectangle'.");
    if (csh.CheckHintAtAndBefore((yylsp[(1) - (3)]), (yylsp[(2) - (3)]), HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
    if (csh.hintStatus != HINT_READY &&
        csh.CheckHintAfter((yylsp[(3) - (3)]), yylloc, yychar==YYEOF, HINT_KEYWORD)) {
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
    (yyval.arcbase) = new CommandSymbol(&msc, (yyvsp[(2) - (3)].str), (yyvsp[(3) - (3)].namerel), NULL, NULL);
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 243:

/* Line 1806 of yacc.c  */
#line 2821 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    if (CaseInsensitiveEqual((yyvsp[(2) - (3)].str), "arc") || CaseInsensitiveEqual((yyvsp[(2) - (3)].str), "rectangle") ||
        CaseInsensitiveEqual((yyvsp[(2) - (3)].str), "..."))
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_KEYWORD);
    else if (CaseInsensitiveBeginsWith("arc", (yyvsp[(2) - (3)].str))==0 &&
             CaseInsensitiveBeginsWith("rectangle", (yyvsp[(2) - (3)].str))==0 &&
             CaseInsensitiveBeginsWith("...", (yyvsp[(2) - (3)].str))==0)
        csh.AddCSH_Error((yylsp[(2) - (3)]), "Bad symbol name. Use 'arc', '...' or 'rectangle'.");
    if (csh.CheckHintAtAndBefore((yylsp[(1) - (3)]), (yylsp[(2) - (3)]), HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
    else if (csh.hintStatus != HINT_READY &&
        csh.CheckHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_MARKER)) {
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
    (yyval.arcbase) = new CommandSymbol(&msc, (yyvsp[(2) - (3)].str), NULL, NULL, NULL);
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 244:

/* Line 1806 of yacc.c  */
#line 2858 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    if (CaseInsensitiveEqual((yyvsp[(2) - (2)].str), "arc") || CaseInsensitiveEqual((yyvsp[(2) - (2)].str), "rectangle") ||
        CaseInsensitiveEqual((yyvsp[(2) - (2)].str), "..."))
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_KEYWORD);
    else if (CaseInsensitiveBeginsWith("arc", (yyvsp[(2) - (2)].str))==0 &&
             CaseInsensitiveBeginsWith("rectangle", (yyvsp[(2) - (2)].str))==0 &&
             CaseInsensitiveBeginsWith("...", (yyvsp[(2) - (2)].str))==0)
        csh.AddCSH_Error((yylsp[(2) - (2)]), "Bad symbol name. Use 'arc', '...' or 'rectangle'.");
    if (csh.CheckHintAtAndBefore((yylsp[(1) - (2)]), (yylsp[(2) - (2)]), HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
    else if (csh.CheckHintAfterPlusOne((yylsp[(2) - (2)]), yylloc, yychar==YYEOF, HINT_MARKER)) {
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
    (yyval.arcbase) = new CommandSymbol(&msc, (yyvsp[(2) - (2)].str), NULL, NULL, NULL);
  #endif
    free((yyvsp[(1) - (2)].str));
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 245:

/* Line 1806 of yacc.c  */
#line 2894 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    if (csh.CheckHintAfterPlusOne((yylsp[(1) - (1)]), yylloc, yychar==YYEOF, HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Error.Error(MSC_POS((yylsp[(1) - (1)])).end, "Missing a symbol type.", "Use 'arc', '...' or 'rectangle'.");
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 247:

/* Line 1806 of yacc.c  */
#line 2912 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (2)])))
        CommandSymbol::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (2)])))
        CommandSymbol::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcbase) = ((yyvsp[(1) - (2)].arcbase))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
  #endif
}
    break;

  case 248:

/* Line 1806 of yacc.c  */
#line 2924 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (4)]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[(2) - (4)]), COLOR_KEYWORD);
    csh.AddCSH_EntityOrMarkerName((yylsp[(3) - (4)]), (yyvsp[(3) - (4)].str));
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(4) - (4)])))
        CommandNote::AttributeNames(csh, true);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(4) - (4)])))
        CommandNote::AttributeValues(csh.hintAttrName, csh, true);
  #else
    ArcBase *a = new CommandNote(&msc, true, MSC_POS((yyloc)), (yyvsp[(3) - (4)].str), MSC_POS((yylsp[(3) - (4)])), (yyvsp[(4) - (4)].attriblist)); //This attaches itself to the target of the note
    if (!a->IsValid()) delete a; //if attachment not successful, drop it
    (yyval.arcbase) = NULL; //no need to add to arclist
  #endif
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(2) - (4)].str));
    free((yyvsp[(3) - (4)].str));
}
    break;

  case 249:

/* Line 1806 of yacc.c  */
#line 2943 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (2)])))
        CommandNote::AttributeNames(csh, true);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (2)])))
        CommandNote::AttributeValues(csh.hintAttrName, csh, true);
  #else
    ArcBase *a = new CommandNote(&msc, true, MSC_POS((yyloc)), NULL, file_line_range(), (yyvsp[(2) - (2)].attriblist)); //This attaches itself to the target of the note
    if (!a->IsValid()) delete a; //if attachment not successful, drop it
    (yyval.arcbase) = NULL; //no need to add to arclist
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 250:

/* Line 1806 of yacc.c  */
#line 2958 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_KEYWORD);
    csh.AddCSH_ErrorAfter((yylsp[(2) - (2)]), "Missing an entity or marker name.");
    csh.AddCSH_ErrorAfter((yylsp[(2) - (2)]), "Notes need a label.");
    if (csh.CheckEntityHintAfterPlusOne((yylsp[(2) - (2)]), yylloc, yychar==YYEOF))
        csh.addMarkersAtEnd = true;
  #else
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (2)].str));
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 251:

/* Line 1806 of yacc.c  */
#line 2973 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_KEYWORD);
    csh.AddCSH_ErrorAfter((yylsp[(2) - (3)]), "Missing an entity or marker name.");
    if (csh.CheckEntityHintAfterPlusOne((yylsp[(2) - (3)]), yylloc, yychar==YYEOF))
        csh.addMarkersAtEnd = true;
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(3) - (3)])))
        CommandNote::AttributeNames(csh, true);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(3) - (3)])))
        CommandNote::AttributeValues(csh.hintAttrName, csh, true);
  #else
    ArcBase *a = new CommandNote(&msc, true, MSC_POS((yyloc)), NULL, file_line_range(), (yyvsp[(3) - (3)].attriblist)); //This attaches itself to the target of the note
    if (!a->IsValid()) delete a; //if attachment not successful, drop it
    (yyval.arcbase) = NULL; //no need to add to arclist
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 252:

/* Line 1806 of yacc.c  */
#line 2994 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (2)])))
        CommandNote::AttributeNames(csh, false);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (2)])))
        CommandNote::AttributeValues(csh.hintAttrName, csh, false);
  #else
    ArcBase *a = new CommandNote(&msc, false, MSC_POS((yyloc)), NULL, file_line_range(), (yyvsp[(2) - (2)].attriblist)); //This attaches itself to the target of the note
    if (!a->IsValid()) delete a; //if attachment not successful, drop it
    (yyval.arcbase) = NULL; //no need to add to arclist
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 253:

/* Line 1806 of yacc.c  */
#line 3009 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    csh.AddCSH_ErrorAfter((yylsp[(1) - (1)]), "Comments need a label.");
    if (csh.CheckEntityHintAfterPlusOne((yylsp[(1) - (1)]), yylloc, yychar==YYEOF))
        csh.addMarkersAtEnd = true;
  #else
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 254:

/* Line 1806 of yacc.c  */
#line 3022 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_ColonString((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str), false);
  #endif
    (yyval.str) = (yyvsp[(1) - (1)].str);
}
    break;

  case 255:

/* Line 1806 of yacc.c  */
#line 3029 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_ColonString((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str), true);
  #endif
    (yyval.str) = (yyvsp[(1) - (1)].str);
}
    break;

  case 256:

/* Line 1806 of yacc.c  */
#line 3037 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = (new AttributeList)->Append(new Attribute("label", (yyvsp[(1) - (1)].str), MSC_POS((yyloc)), MSC_POS((yyloc)).IncStartCol()));
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 257:

/* Line 1806 of yacc.c  */
#line 3045 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = ((yyvsp[(2) - (2)].attriblist))->Prepend(new Attribute("label", (yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), MSC_POS((yylsp[(1) - (2)])).IncStartCol()));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 258:

/* Line 1806 of yacc.c  */
#line 3053 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = ((yyvsp[(1) - (3)].attriblist))->Append(new Attribute("label", (yyvsp[(2) - (3)].str), MSC_POS((yylsp[(2) - (3)])), MSC_POS((yylsp[(2) - (3)])).IncStartCol()));
        //Merge $3 at the end of $1
        ((yyvsp[(1) - (3)].attriblist))->splice(((yyvsp[(1) - (3)].attriblist))->end(), *((yyvsp[(3) - (3)].attriblist)));
        delete ((yyvsp[(3) - (3)].attriblist)); //empty list now
        (yyval.attriblist) = (yyvsp[(1) - (3)].attriblist);
  #endif
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 259:

/* Line 1806 of yacc.c  */
#line 3065 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = ((yyvsp[(1) - (2)].attriblist))->Append(new Attribute("label", (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])), MSC_POS((yylsp[(2) - (2)])).IncStartCol()));
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 261:

/* Line 1806 of yacc.c  */
#line 3076 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_BRACKET);
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_BRACKET);
        csh.CheckHintBetween((yylsp[(1) - (2)]), (yylsp[(2) - (2)]), HINT_ATTR_NAME);
  #else
    (yyval.attriblist) = new AttributeList;
  #endif
}
    break;

  case 262:

/* Line 1806 of yacc.c  */
#line 3086 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_BRACKET);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_BRACKET);
        csh.CheckHintBetween((yylsp[(1) - (3)]), (yylsp[(2) - (3)]), HINT_ATTR_NAME);
  #else
    (yyval.attriblist) = (yyvsp[(2) - (3)].attriblist);
  #endif
}
    break;

  case 263:

/* Line 1806 of yacc.c  */
#line 3096 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (4)]), COLOR_BRACKET);
        csh.AddCSH_Error((yylsp[(3) - (4)]), "Extra stuff after an attribute list. Maybe missing a comma?");
        csh.AddCSH((yylsp[(4) - (4)]), COLOR_BRACKET);
        csh.CheckHintBetween((yylsp[(1) - (4)]), (yylsp[(2) - (4)]), HINT_ATTR_NAME);
  #else
    (yyval.attriblist) = (yyvsp[(2) - (4)].attriblist);
  #endif
}
    break;

  case 264:

/* Line 1806 of yacc.c  */
#line 3107 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_BRACKET);
        csh.AddCSH_Error((yylsp[(2) - (3)]), "Could not recognize this as an attribute.");
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_BRACKET);
        csh.CheckHintBetween((yylsp[(1) - (3)]), (yylsp[(2) - (3)]), HINT_ATTR_NAME);
  #else
    (yyval.attriblist) = new AttributeList;
  #endif
}
    break;

  case 265:

/* Line 1806 of yacc.c  */
#line 3118 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_BRACKET);
        csh.AddCSH_ErrorAfter((yylsp[(2) - (2)]), "Missing a square bracket (']').");
        csh.CheckHintBetween((yylsp[(1) - (2)]), (yylsp[(2) - (2)]), HINT_ATTR_NAME);
  #else
    (yyval.attriblist) = (yyvsp[(2) - (2)].attriblist);
    msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).end.NextChar(), "Missing ']'.");
  #endif
}
    break;

  case 266:

/* Line 1806 of yacc.c  */
#line 3129 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_BRACKET);
        csh.AddCSH_Error((yylsp[(3) - (3)]), "Missing a ']'.");
        csh.CheckHintBetween((yylsp[(1) - (3)]), (yylsp[(2) - (3)]), HINT_ATTR_NAME);
  #else
    (yyval.attriblist) = (yyvsp[(2) - (3)].attriblist);
    msc.Error.Error(MSC_POS((yylsp[(3) - (3)])).start, "Missing ']'.");
  #endif
}
    break;

  case 267:

/* Line 1806 of yacc.c  */
#line 3140 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (1)]), COLOR_BRACKET);
        csh.AddCSH_ErrorAfter((yylsp[(1) - (1)]), "Missing a square bracket (']').");
        csh.CheckHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF, HINT_ATTR_NAME);
  #else
    (yyval.attriblist) = new AttributeList;
    msc.Error.Error(MSC_POS((yylsp[(1) - (1)])).end.NextChar(), "Missing ']'.");
  #endif
}
    break;

  case 268:

/* Line 1806 of yacc.c  */
#line 3153 "language.yy"
    {
    (yyval.attriblist) = NULL;
}
    break;

  case 269:

/* Line 1806 of yacc.c  */
#line 3157 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_BRACKET);
        csh.AddCSH_Error((yylsp[(2) - (2)]), "Missing a ']'.");
        csh.CheckHintBetween((yylsp[(1) - (2)]), (yylsp[(2) - (2)]), HINT_ATTR_NAME);
  #else
    (yyval.attriblist) = new AttributeList;
    msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).start, "Missing ']'.");
  #endif
}
    break;

  case 270:

/* Line 1806 of yacc.c  */
#line 3169 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.attriblist) = (new AttributeList)->Append((yyvsp[(1) - (1)].attrib));
  #endif
}
    break;

  case 271:

/* Line 1806 of yacc.c  */
#line 3176 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    csh.CheckHintBetween((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_NAME);
  #else
    (yyval.attriblist) = ((yyvsp[(1) - (3)].attriblist))->Append((yyvsp[(3) - (3)].attrib));
  #endif
}
    break;

  case 272:

/* Line 1806 of yacc.c  */
#line 3185 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_COMMA);
    csh.CheckHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF, HINT_ATTR_NAME);
  #else
    (yyval.attriblist) = (yyvsp[(1) - (2)].attriblist);
    msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).end.NextChar(), "Expecting an entity here.");
  #endif
}
    break;

  case 274:

/* Line 1806 of yacc.c  */
#line 3197 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_ATTRNAME);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        csh.AddCSH_AttrValue((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str), (yyvsp[(1) - (3)].str));
        csh.CheckHintAt((yylsp[(1) - (3)]), HINT_ATTR_NAME);
        csh.CheckHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_VALUE, (yyvsp[(1) - (3)].str));
  #else
        (yyval.attrib) = new Attribute((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(1) - (3)])), MSC_POS((yylsp[(3) - (3)])));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 275:

/* Line 1806 of yacc.c  */
#line 3211 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_ATTRNAME);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_ATTRVALUE);
        csh.CheckHintAt((yylsp[(1) - (3)]), HINT_ATTR_NAME);
        csh.CheckHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_VALUE, (yyvsp[(1) - (3)].str));
  #else
        (yyval.attrib) = new Attribute((yyvsp[(1) - (3)].str), atof((yyvsp[(3) - (3)].str)), MSC_POS((yyloc)), MSC_POS((yylsp[(3) - (3)])), (yyvsp[(3) - (3)].str));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 276:

/* Line 1806 of yacc.c  */
#line 3225 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_ATTRNAME);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_ATTRVALUE);
        csh.CheckHintAt((yylsp[(1) - (3)]), HINT_ATTR_NAME);
        csh.CheckHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_VALUE, (yyvsp[(1) - (3)].str));
  #else
        (yyval.attrib) = new Attribute((yyvsp[(1) - (3)].str), str2bool((yyvsp[(3) - (3)].str)), MSC_POS((yyloc)), MSC_POS((yylsp[(3) - (3)])), (yyvsp[(3) - (3)].str));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 277:

/* Line 1806 of yacc.c  */
#line 3239 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str), COLOR_ATTRNAME);
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_EQUAL);
        csh.CheckHintAt((yylsp[(1) - (2)]), HINT_ATTR_NAME);
        csh.CheckHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF, HINT_ATTR_VALUE, (yyvsp[(1) - (2)].str));
  #else
        (yyval.attrib) = new Attribute((yyvsp[(1) - (2)].str), (char*)NULL, MSC_POS((yyloc)), MSC_POS((yyloc)));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 278:

/* Line 1806 of yacc.c  */
#line 3253 "language.yy"
    {
    (yyval.attrib) = NULL;
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 279:

/* Line 1806 of yacc.c  */
#line 3258 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_StyleOrAttrName((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str));
        csh.CheckHintAt((yylsp[(1) - (1)]), HINT_ATTR_NAME);
  #else
        (yyval.attrib) = new Attribute((yyvsp[(1) - (1)].str), MSC_POS((yyloc)));
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 308:

/* Line 1806 of yacc.c  */
#line 3282 "language.yy"
    {(yyval.str) = strdup("->");}
    break;

  case 309:

/* Line 1806 of yacc.c  */
#line 3283 "language.yy"
    {(yyval.str) = strdup("<-");}
    break;

  case 310:

/* Line 1806 of yacc.c  */
#line 3284 "language.yy"
    {(yyval.str) = strdup("<->");}
    break;

  case 311:

/* Line 1806 of yacc.c  */
#line 3285 "language.yy"
    {(yyval.str) = strdup("=>");}
    break;

  case 312:

/* Line 1806 of yacc.c  */
#line 3286 "language.yy"
    {(yyval.str) = strdup("<=");}
    break;

  case 313:

/* Line 1806 of yacc.c  */
#line 3287 "language.yy"
    {(yyval.str) = strdup("<=>");}
    break;

  case 314:

/* Line 1806 of yacc.c  */
#line 3288 "language.yy"
    {(yyval.str) = strdup(">>");}
    break;

  case 315:

/* Line 1806 of yacc.c  */
#line 3289 "language.yy"
    {(yyval.str) = strdup("<<");}
    break;

  case 316:

/* Line 1806 of yacc.c  */
#line 3290 "language.yy"
    {(yyval.str) = strdup("<<>>");}
    break;

  case 317:

/* Line 1806 of yacc.c  */
#line 3291 "language.yy"
    {(yyval.str) = strdup(">");}
    break;

  case 318:

/* Line 1806 of yacc.c  */
#line 3292 "language.yy"
    {(yyval.str) = strdup("<");}
    break;

  case 319:

/* Line 1806 of yacc.c  */
#line 3293 "language.yy"
    {(yyval.str) = strdup("<>");}
    break;

  case 320:

/* Line 1806 of yacc.c  */
#line 3295 "language.yy"
    {
    switch ((yyvsp[(1) - (1)].arctype)) {
    case MSC_ARC_DIVIDER:  (yyval.str) = strdup("---"); break;
    case MSC_ARC_DISCO:    (yyval.str) = strdup("..."); break;
    default: _ASSERT(0);
    }
}
    break;

  case 321:

/* Line 1806 of yacc.c  */
#line 3303 "language.yy"
    {
    switch ((yyvsp[(1) - (1)].arctype)) {
    case MSC_EMPH_SOLID:  (yyval.str) = strdup("--"); break;
    case MSC_EMPH_DASHED: (yyval.str) = strdup("++"); break;
    case MSC_EMPH_DOTTED: (yyval.str) = strdup(".."); break;
    case MSC_EMPH_DOUBLE: (yyval.str) = strdup("=="); break;
    default: _ASSERT(0);
    }
}
    break;

  case 326:

/* Line 1806 of yacc.c  */
#line 3316 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.PushContext();
    if (csh.CheckHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF, HINT_LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.PushContext();
  #endif
}
    break;

  case 328:

/* Line 1806 of yacc.c  */
#line 3332 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    (yyval.arcbase) = NULL;
    csh.PopContext();
  #else
    std::pair<bool, double> hscale = msc.Contexts.back().hscale;
    (yyval.arcbase) = msc.PopContext();
    if (hscale.first)
        msc.Contexts.back().hscale = hscale;
  #endif
}
    break;



/* Line 1806 of yacc.c  */
#line 7201 "language.cc"
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

  *++yyvsp = yylval;

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
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, RESULT, yyscanner);
    }
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



/* Line 2067 of yacc.c  */
#line 3345 "language.yy"



/* END OF FILE */

