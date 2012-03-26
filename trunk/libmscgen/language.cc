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
     TOK_AT = 300,
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
     TOK__NEVER__HAPPENS = 311
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
#define TOK__NEVER__HAPPENS 311




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 135 "language.yy"

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
#line 356 "language.cc"
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
#line 381 "language.cc"

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
#define YYFINAL  173
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1127

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  57
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  69
/* YYNRULES -- Number of rules.  */
#define YYNRULES  315
/* YYNRULES -- Number of states.  */
#define YYNSTATES  354

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   311

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
      55,    56
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
     170,   173,   177,   179,   182,   184,   186,   190,   193,   195,
     198,   200,   202,   204,   207,   210,   213,   215,   219,   221,
     224,   227,   229,   233,   235,   237,   239,   241,   243,   247,
     250,   254,   258,   262,   266,   270,   273,   277,   280,   282,
     286,   289,   293,   296,   298,   302,   305,   308,   310,   314,
     317,   319,   323,   326,   328,   332,   334,   338,   342,   348,
     355,   357,   359,   363,   366,   369,   371,   373,   377,   380,
     384,   388,   392,   396,   398,   401,   403,   406,   410,   414,
     417,   422,   426,   428,   431,   434,   438,   441,   444,   446,
     450,   452,   455,   459,   461,   464,   468,   471,   474,   476,
     479,   483,   487,   492,   496,   501,   505,   510,   516,   518,
     520,   522,   524,   526,   530,   533,   536,   538,   542,   545,
     548,   550,   552,   556,   561,   565,   570,   573,   575,   577,
     579,   581,   583,   587,   590,   593,   597,   600,   604,   607,
     610,   614,   617,   621,   624,   627,   631,   634,   636,   638,
     640,   642,   644,   646,   648,   650,   652,   654,   656,   658,
     660,   662,   664,   666,   668,   670,   672,   674,   677,   679,
     681,   683,   688,   694,   698,   703,   707,   711,   714,   716,
     718,   721,   725,   729,   732,   734,   736,   738,   741,   745,
     748,   750,   753,   757,   762,   766,   769,   773,   775,   778,
     781,   783,   787,   790,   794,   798,   802,   806,   809,   813,
     815,   817,   819,   821,   823,   825,   827,   829,   831,   833,
     835,   837,   839,   841,   843,   845,   847,   849,   851,   853,
     855,   857,   859,   861,   863,   865,   867,   869,   871,   873,
     875,   877,   879,   881,   883,   885,   887,   889,   891,   893,
     895,   897,   899,   901,   903,   906
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      58,     0,    -1,    59,    -1,    59,    51,    -1,    -1,    15,
      62,    -1,    61,    62,    -1,    15,    -1,    15,     1,    -1,
      60,    -1,    60,     1,    -1,    63,    -1,    63,    12,    -1,
      63,    12,    60,    -1,    15,     7,    -1,    15,     7,   123,
      -1,   124,    63,   125,    -1,   124,   125,    -1,   124,    63,
       1,   125,    -1,   124,    63,     1,    -1,   124,    63,    -1,
      64,    -1,    64,    66,    -1,    66,    -1,    65,    -1,    64,
      65,    -1,    66,     9,    -1,    66,     9,    56,    -1,    66,
       1,     9,    56,    -1,     9,    -1,    66,     1,     9,    -1,
      67,    -1,    43,    67,    -1,    99,    -1,    99,   116,    -1,
      40,    -1,    40,    99,    -1,    40,    99,   116,    -1,    44,
      -1,    44,    98,    -1,    44,    98,   116,    -1,   116,    -1,
      76,    -1,    71,    -1,    71,    76,    -1,    76,     8,    -1,
      76,     8,    74,    -1,    71,    76,     8,    -1,    71,    76,
       8,    74,    -1,    72,    -1,    89,    -1,    93,    -1,    88,
      -1,    37,    80,    -1,    38,    77,    -1,    39,    82,    -1,
      34,    -1,    34,   117,    -1,    35,    -1,    35,   117,    -1,
      42,   123,    -1,    42,   123,   117,    -1,    36,    -1,    36,
     117,    -1,   113,    -1,   114,    -1,    53,    69,    68,    -1,
      53,    69,    -1,    53,    -1,    52,    68,    -1,    52,    -1,
     116,    -1,     5,    -1,     5,   116,    -1,   120,     6,    -1,
       6,   120,    -1,     6,    -1,   120,     6,   120,    -1,   120,
      -1,   120,     6,    -1,     6,   120,    -1,     6,    -1,   120,
       6,   120,    -1,    48,    -1,    47,    -1,    49,    -1,    50,
      -1,    73,    -1,    72,     8,    73,    -1,    72,     8,    -1,
      72,     8,    56,    -1,    72,     8,     1,    -1,   120,     7,
      19,    -1,   120,     7,     5,    -1,   120,     7,   123,    -1,
     120,     7,    -1,    15,     7,   123,    -1,    15,     7,    -1,
      75,    -1,    74,     8,    75,    -1,    74,     8,    -1,    74,
       8,    56,    -1,   120,   116,    -1,   120,    -1,   120,   116,
      62,    -1,   120,    62,    -1,   120,   116,    -1,   120,    -1,
     120,   116,    62,    -1,   120,    62,    -1,    78,    -1,    77,
       8,    78,    -1,    79,   117,    -1,   123,    -1,    79,     8,
     123,    -1,    81,    -1,    80,     8,    81,    -1,     3,     7,
     123,    -1,     3,    83,    84,     9,    12,    -1,     3,    83,
      84,     9,     1,    12,    -1,    11,    -1,    85,    -1,    84,
       9,    85,    -1,    37,    80,    -1,    38,    77,    -1,    86,
      -1,    87,    -1,    86,     8,    87,    -1,    86,     1,    -1,
     120,     7,    19,    -1,   120,     7,     5,    -1,   120,     7,
     123,    -1,    15,     7,   123,    -1,    62,    -1,    88,    62,
      -1,    90,    -1,    89,    94,    -1,    89,    94,   116,    -1,
      89,    94,    62,    -1,    89,    62,    -1,    89,    94,   116,
      62,    -1,    89,   116,    62,    -1,    94,    -1,    94,   116,
      -1,    94,    62,    -1,    94,   116,    62,    -1,    41,    94,
      -1,    41,     1,    -1,    41,    -1,    41,    94,   116,    -1,
      91,    -1,    92,    94,    -1,    92,    94,   116,    -1,    92,
      -1,    92,    62,    -1,   120,    33,   120,    -1,    33,   120,
      -1,   120,    33,    -1,    33,    -1,    45,   120,    -1,    45,
     120,     5,    -1,    45,   120,     6,    -1,    45,   120,     6,
       5,    -1,    45,   120,    10,    -1,    45,   120,    10,     5,
      -1,    45,   120,    33,    -1,    45,   120,     6,   120,    -1,
      45,   120,     6,   120,     5,    -1,    45,    -1,   120,    -1,
      33,    -1,   103,    -1,   105,    -1,   120,    96,   120,    -1,
      96,   120,    -1,   120,    96,    -1,    96,    -1,   120,   104,
     120,    -1,   104,   120,    -1,   120,   104,    -1,   104,    -1,
     120,    -1,    96,   120,    56,    -1,   120,    96,   120,    56,
      -1,   104,   120,    56,    -1,   120,   104,   120,    56,    -1,
      97,    95,    -1,    97,    -1,    32,    -1,   100,    -1,   101,
      -1,   102,    -1,   120,   103,   120,    -1,   103,   120,    -1,
     120,   103,    -1,   100,   106,   120,    -1,   100,   106,    -1,
     120,   104,   120,    -1,   104,   120,    -1,   120,   104,    -1,
     101,   107,   120,    -1,   101,   107,    -1,   120,   105,   120,
      -1,   105,   120,    -1,   120,   105,    -1,   102,   108,   120,
      -1,   102,   108,    -1,    20,    -1,    23,    -1,    26,    -1,
      29,    -1,    21,    -1,    24,    -1,    27,    -1,    30,    -1,
      22,    -1,    25,    -1,    28,    -1,    31,    -1,   103,    -1,
       6,    -1,   104,    -1,     6,    -1,   105,    -1,     6,    -1,
     110,    -1,   120,    -1,    46,    95,    -1,    46,    -1,   120,
      -1,   122,    -1,    54,   111,    70,   109,    -1,    54,   111,
      70,   109,   109,    -1,    54,   111,   110,    -1,    54,   111,
     110,   109,    -1,    54,   111,    70,    -1,    54,   111,   120,
      -1,    54,   111,    -1,    54,    -1,   112,    -1,   112,   116,
      -1,    55,   110,   116,    -1,    55,    95,   116,    -1,    55,
     116,    -1,    17,    -1,    16,    -1,   115,    -1,   115,   117,
      -1,   117,   115,   117,    -1,   117,   115,    -1,   117,    -1,
      13,    14,    -1,    13,   118,    14,    -1,    13,   118,     1,
      14,    -1,    13,     1,    14,    -1,    13,   118,    -1,    13,
     118,     1,    -1,    13,    -1,    13,    56,    -1,    13,     1,
      -1,   119,    -1,   118,     8,   119,    -1,   118,     8,    -1,
     118,     8,    56,    -1,   123,     7,   123,    -1,   123,     7,
       5,    -1,   123,     7,    19,    -1,   123,     7,    -1,   123,
       7,    56,    -1,   123,    -1,     4,    -1,     3,    -1,    15,
      -1,    37,    -1,    38,    -1,    39,    -1,    36,    -1,    40,
      -1,    41,    -1,    44,    -1,    43,    -1,    34,    -1,    35,
      -1,    42,    -1,    45,    -1,    46,    -1,    47,    -1,    48,
      -1,    49,    -1,    50,    -1,    51,    -1,    53,    -1,    52,
      -1,    54,    -1,    55,    -1,    20,    -1,    21,    -1,    22,
      -1,    23,    -1,    24,    -1,    25,    -1,    26,    -1,    27,
      -1,    28,    -1,    29,    -1,    30,    -1,    31,    -1,    32,
      -1,    33,    -1,   120,    -1,   121,    -1,   122,    -1,    18,
      -1,    11,    -1,    11,    56,    -1,    12,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   218,   218,   219,   230,   240,   251,   261,   271,   281,
     288,   301,   302,   311,   325,   337,   352,   365,   378,   394,
     406,   419,   420,   434,   450,   460,   471,   485,   486,   487,
     499,   514,   522,   545,   552,   563,   575,   590,   608,   620,
     630,   644,   655,   662,   674,   686,   698,   709,   725,   740,
     752,   759,   766,   773,   782,   791,   800,   809,   822,   831,
     844,   856,   872,   881,   894,   895,   896,   909,   924,   935,
     948,   964,   965,   976,   987,   999,  1010,  1019,  1033,  1058,
    1070,  1081,  1091,  1106,  1106,  1106,  1106,  1108,  1118,  1137,
    1150,  1151,  1162,  1182,  1202,  1229,  1249,  1267,  1288,  1294,
    1304,  1316,  1319,  1335,  1347,  1363,  1376,  1394,  1409,  1427,
    1443,  1444,  1451,  1477,  1493,  1511,  1512,  1519,  1535,  1555,
    1578,  1589,  1590,  1597,  1604,  1611,  1613,  1614,  1620,  1627,
    1646,  1665,  1684,  1705,  1714,  1726,  1734,  1743,  1757,  1766,
    1776,  1790,  1807,  1815,  1828,  1836,  1851,  1866,  1882,  1893,
    1912,  1919,  1928,  1943,  1944,  1951,  1966,  1978,  1991,  2002,
    2014,  2028,  2042,  2057,  2070,  2085,  2119,  2135,  2153,  2165,
    2181,  2181,  2181,  2183,  2197,  2208,  2220,  2229,  2243,  2254,
    2266,  2275,  2288,  2289,  2290,  2291,  2294,  2305,  2316,  2324,
    2328,  2332,  2337,  2351,  2362,  2374,  2385,  2396,  2410,  2421,
    2433,  2444,  2454,  2468,  2479,  2491,  2502,  2512,  2512,  2512,
    2512,  2513,  2513,  2513,  2513,  2514,  2514,  2514,  2514,  2516,
    2516,  2517,  2517,  2518,  2518,  2520,  2521,  2544,  2554,  2568,
    2568,  2570,  2606,  2629,  2665,  2689,  2725,  2762,  2798,  2815,
    2816,  2828,  2839,  2855,  2868,  2875,  2883,  2891,  2899,  2911,
    2919,  2922,  2932,  2942,  2953,  2964,  2975,  2986,  2999,  3003,
    3015,  3022,  3031,  3041,  3043,  3057,  3071,  3085,  3099,  3104,
    3116,  3116,  3118,  3118,  3119,  3119,  3120,  3120,  3120,  3121,
    3121,  3122,  3122,  3123,  3123,  3123,  3123,  3123,  3124,  3124,
    3124,  3125,  3125,  3125,  3126,  3128,  3129,  3130,  3131,  3132,
    3133,  3134,  3135,  3136,  3137,  3138,  3139,  3140,  3148,  3159,
    3159,  3159,  3159,  3161,  3175,  3177
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
  "TOK_VERTICAL", "TOK_AT", "TOK_AT_POS", "TOK_SHOW", "TOK_HIDE",
  "TOK_ACTIVATE", "TOK_DEACTIVATE", "TOK_BYE", "TOK_COMMAND_VSPACE",
  "TOK_COMMAND_HSPACE", "TOK_COMMAND_SYMBOL", "TOK_COMMAND_NOTE",
  "TOK__NEVER__HAPPENS", "$accept", "msc_with_bye", "msc",
  "top_level_arclist", "msckey", "braced_arclist",
  "arclist_maybe_no_semicolon", "arclist", "arc_with_parallel_semicolon",
  "arc_with_parallel", "arc", "full_arcattrlist_with_label_or_number",
  "entityrel", "markerrel_no_string", "entity_command_prefixes", "optlist",
  "opt", "entitylist", "entity", "first_entity", "styledeflist",
  "styledef", "tok_stringlist", "colordeflist", "colordef", "designdef",
  "scope_open_empty", "designelementlist", "designelement",
  "designoptlist", "designopt", "parallel", "box_list", "first_box",
  "first_pipe", "pipe_list_no_content", "pipe_list", "boxrel", "vertxpos",
  "empharcrel_straight", "vertrel_no_xpos", "vertrel", "arcrel",
  "arcrel_to", "arcrel_from", "arcrel_bidir", "relation_to",
  "relation_from", "relation_bidir", "relation_to_cont",
  "relation_from_cont", "relation_bidir_cont", "extvertxpos",
  "extvertxpos_no_string", "symbol_type_string", "symbol_command_no_attr",
  "symbol_command", "note", "colon_string", "full_arcattrlist_with_label",
  "full_arcattrlist", "arcattrlist", "arcattr", "entity_string",
  "reserved_word_string", "symbol_string", "string", "scope_open",
  "scope_close", 0
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
     305,   306,   307,   308,   309,   310,   311
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    57,    58,    58,    59,    59,    59,    59,    59,    59,
      59,    60,    60,    60,    61,    61,    62,    62,    62,    62,
      62,    63,    63,    63,    64,    64,    65,    65,    65,    65,
      65,    66,    66,    67,    67,    67,    67,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    68,    68,    68,    69,    69,    69,    69,    69,    70,
      70,    70,    70,    71,    71,    71,    71,    72,    72,    72,
      72,    72,    73,    73,    73,    73,    73,    73,    74,    74,
      74,    74,    75,    75,    75,    75,    76,    76,    76,    76,
      77,    77,    78,    79,    79,    80,    80,    81,    82,    82,
      83,    84,    84,    85,    85,    85,    86,    86,    86,    87,
      87,    87,    87,    88,    88,    89,    89,    89,    89,    89,
      89,    89,    90,    90,    90,    90,    91,    91,    91,    91,
      92,    92,    92,    93,    93,    94,    94,    94,    94,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      96,    96,    96,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    98,    98,    99,    99,
      99,    99,   100,   100,   100,   100,   100,   101,   101,   101,
     101,   101,   102,   102,   102,   102,   102,   103,   103,   103,
     103,   104,   104,   104,   104,   105,   105,   105,   105,   106,
     106,   107,   107,   108,   108,   109,   109,   110,   110,   111,
     111,   112,   112,   112,   112,   112,   112,   112,   112,   113,
     113,   114,   114,   114,   115,   115,   116,   116,   116,   116,
     116,   117,   117,   117,   117,   117,   117,   117,   117,   117,
     118,   118,   118,   118,   119,   119,   119,   119,   119,   119,
     120,   120,   121,   121,   121,   121,   121,   121,   121,   121,
     121,   121,   121,   121,   121,   121,   121,   121,   121,   121,
     121,   121,   121,   121,   121,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   123,
     123,   123,   123,   124,   124,   125
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
       2,     3,     1,     2,     1,     1,     3,     2,     1,     2,
       1,     1,     1,     2,     2,     2,     1,     3,     1,     2,
       2,     1,     3,     1,     1,     1,     1,     1,     3,     2,
       3,     3,     3,     3,     3,     2,     3,     2,     1,     3,
       2,     3,     2,     1,     3,     2,     2,     1,     3,     2,
       1,     3,     2,     1,     3,     1,     3,     3,     5,     6,
       1,     1,     3,     2,     2,     1,     1,     3,     2,     3,
       3,     3,     3,     1,     2,     1,     2,     3,     3,     2,
       4,     3,     1,     2,     2,     3,     2,     2,     1,     3,
       1,     2,     3,     1,     2,     3,     2,     2,     1,     2,
       3,     3,     4,     3,     4,     3,     4,     5,     1,     1,
       1,     1,     1,     3,     2,     2,     1,     3,     2,     2,
       1,     1,     3,     4,     3,     4,     2,     1,     1,     1,
       1,     1,     3,     2,     2,     3,     2,     3,     2,     2,
       3,     2,     3,     2,     2,     3,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     1,
       1,     4,     5,     3,     4,     3,     3,     2,     1,     1,
       2,     3,     3,     2,     1,     1,     1,     2,     3,     2,
       1,     2,     3,     4,     3,     2,     3,     1,     2,     2,
       1,     3,     2,     3,     3,     3,     3,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       4,   271,   270,    29,   313,     0,     0,   245,   244,   207,
     211,   215,   208,   212,   216,   209,   213,   217,   210,   214,
     218,   188,   158,    56,    58,    62,     0,     0,     0,    35,
       0,     0,     0,    38,    84,    83,    85,    86,    70,    68,
     238,     0,     0,     2,     0,     0,   133,    11,    21,    24,
       0,    31,    43,    49,    87,    42,    52,    50,   135,   150,
     153,    51,   142,    33,   189,   190,   191,     0,     0,     0,
     239,    64,    65,   246,    41,   250,   107,     0,   314,   259,
     251,   272,   312,   295,   296,   297,   298,   299,   300,   301,
     302,   303,   304,   305,   306,   307,   308,   281,   282,   276,
     273,   274,   275,   277,   278,   283,   280,   279,   284,   285,
     286,   287,   288,   289,   290,   292,   291,   293,   294,   258,
       0,   260,   309,   310,   311,   269,     8,    97,     5,   156,
      57,    59,    63,     0,    53,   115,    54,   110,     0,   113,
       0,    55,    36,     0,   147,   146,     0,    60,     0,    32,
     170,   176,   187,    39,   171,   180,   172,   181,    72,    69,
      71,    76,    67,    78,   237,   229,   230,   168,   228,     0,
       0,   243,   169,     1,     3,    10,     6,    12,    25,     0,
       0,    26,    44,   107,     0,    45,   134,   139,   136,     0,
     154,   151,   144,   143,    34,   220,   219,   196,   222,   221,
     201,   224,   223,   206,   193,   198,   203,   240,   247,   249,
      95,   157,   109,   194,   199,   204,   106,   315,     0,    17,
     254,   256,   262,   252,   267,    96,     0,     0,     0,     0,
     112,   120,     0,    37,   149,    61,    97,   174,   186,    40,
     178,   175,   179,    73,    75,    66,    74,    81,   235,   233,
     236,   159,   227,   242,   241,    13,    30,    27,    47,    91,
      90,    88,     0,    46,    98,   103,   138,   137,   141,   152,
     145,   195,   200,   205,   248,    93,    92,    94,   155,   192,
     197,   202,   108,    19,    16,   253,   263,   261,   265,   266,
     268,   264,   117,   116,   111,   114,     0,     0,     0,     0,
     121,     0,   126,     0,    96,   182,   184,   173,   177,    77,
      80,   231,   225,   226,   234,    79,   160,   161,   163,   165,
      28,    48,   100,   105,   102,   140,    18,     0,   123,   124,
       0,   128,     0,     0,   183,   185,   232,    82,   162,   166,
     164,   101,    99,   104,   132,     0,   118,   122,   127,   130,
     129,   131,   167,   119
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,   159,   162,   248,    52,    53,    54,   263,   264,    55,
     136,   137,   138,   134,   135,   141,   232,   299,   300,   301,
     302,    56,    57,    58,    59,    60,    61,    62,   169,   151,
     152,   153,    63,    64,    65,    66,    67,    68,    69,   197,
     200,   203,   311,   312,   164,    70,    71,    72,    73,    74,
      75,   120,   121,   122,   123,   124,   139,    77,   219
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -241
static const yytype_int16 yypact[] =
{
     679,  -241,  -241,  -241,   -25,   358,    10,  -241,  -241,  -241,
    -241,  -241,  -241,  -241,  -241,  -241,  -241,  -241,  -241,  -241,
    -241,  -241,    24,    29,    29,    29,    79,   838,    84,   325,
     947,   838,   891,  1057,  -241,  -241,  -241,  -241,   211,    40,
    1026,   176,    90,    48,    14,    93,  -241,    95,   732,  -241,
     966,  -241,    24,   111,  -241,   113,    93,   143,  -241,  -241,
      51,  -241,   251,   149,   175,   239,   228,    24,    24,    24,
     149,  -241,  -241,    29,  -241,    57,  1082,   520,  -241,   101,
    -241,  -241,  -241,  -241,  -241,  -241,  -241,  -241,  -241,  -241,
    -241,  -241,  -241,  -241,  -241,  -241,  -241,  -241,  -241,  -241,
    -241,  -241,  -241,  -241,  -241,  -241,  -241,  -241,  -241,  -241,
    -241,  -241,  -241,  -241,  -241,  -241,  -241,  -241,  -241,  -241,
     293,  -241,  -241,  -241,  -241,   117,  -241,   785,  -241,  -241,
    -241,  -241,  -241,   120,   124,  -241,   130,  -241,    50,  -241,
     122,  -241,   149,  1096,  -241,   149,   106,    29,   141,  -241,
    -241,    24,    33,   149,  -241,    24,  -241,  1043,   149,  -241,
    -241,    24,   211,   151,    20,  -241,  -241,    24,    33,   149,
     149,  -241,  -241,  -241,  -241,  -241,  -241,   732,  -241,   992,
     140,   127,   155,   251,    85,    24,  -241,  -241,   251,    93,
    -241,   149,  -241,    93,  -241,  -241,  -241,    24,  -241,  -241,
      24,  -241,  -241,    24,  -241,  -241,  -241,  -241,  -241,    29,
     573,    24,  -241,    24,    24,    24,    93,  -241,  1011,  -241,
    -241,   159,   466,  -241,   412,   179,   838,    79,   838,   838,
    -241,  -241,   107,  -241,  -241,  -241,   838,   135,  -241,  -241,
     138,    24,    24,  -241,  -241,  -241,    24,    24,     4,     4,
     172,   125,  -241,  -241,  -241,  -241,   150,  -241,    24,  -241,
    -241,  -241,   189,   178,  -241,   251,  -241,    93,  -241,  -241,
    -241,  -241,  -241,  -241,  -241,  -241,  -241,  -241,  -241,  -241,
    -241,  -241,  -241,   187,  -241,  -241,  -241,  -241,  -241,  -241,
    -241,  -241,  -241,  -241,  -241,  -241,   198,    79,   838,   200,
    -241,   104,  -241,   203,  -241,  -241,  -241,   162,   163,  -241,
    -241,     4,  -241,  -241,  -241,    24,  -241,   167,   215,  -241,
    -241,   178,    16,  -241,    93,  -241,  -241,   838,   124,   130,
     105,  -241,    88,   626,  -241,  -241,  -241,  -241,  -241,   218,
    -241,  -241,  -241,  -241,  -241,   213,  -241,  -241,  -241,  -241,
    -241,  -241,  -241,  -241
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -241,  -241,  -241,    49,  -241,    19,   153,  -241,   190,   191,
     205,    78,  -241,  -241,  -241,  -241,    60,    -7,   -70,   209,
     -43,    43,  -241,   -24,    45,  -241,  -241,  -241,   -55,  -241,
     -56,  -241,  -241,  -241,  -241,  -241,  -241,    23,  -134,   121,
    -241,  -241,   248,  -241,  -241,  -241,   -29,   -20,   -17,  -241,
    -241,  -241,  -240,   -35,  -241,  -241,  -241,  -241,   204,   112,
     -22,  -241,    58,     0,  -241,   243,     7,  -241,  -213
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -258
static const yytype_int16 yytable[] =
{
      76,   130,   131,   132,   154,   284,   170,     1,     2,   314,
      -7,   126,   125,   155,    -9,   175,   156,   127,   238,     1,
       2,     4,   129,     1,     2,   128,   247,     1,     2,   143,
     146,    78,    76,   157,   252,   196,     1,     2,   147,   163,
     165,   172,     5,     1,     2,   199,   161,   213,    76,   202,
     168,   208,   183,   145,     1,     2,   214,   146,   229,   215,
     146,    -7,     4,     5,   176,    -9,   168,   204,   205,   206,
     326,   336,   341,     7,     8,   186,   187,    76,   167,   190,
     188,   192,   133,   191,    22,   -89,   259,   140,     1,     2,
     173,     1,     2,   -89,   -89,   212,   -89,   -89,   -89,   174,
     148,   -89,   -89,   296,     4,   331,   345,   177,     1,     2,
       1,     2,   332,  -125,   213,   220,   230,   346,   -89,   184,
     296,   185,   296,   214,   224,   235,   215,   226,   154,   249,
     316,   317,   227,   231,   225,   318,   -89,   242,   228,   211,
     156,   260,   297,   298,   297,   298,     1,     2,   236,   256,
     160,   237,   172,   171,     4,   240,     5,   246,   319,     7,
       8,   244,     5,   258,   250,     7,     8,   251,   172,   189,
       1,     2,   338,   285,   193,   194,    22,    76,   315,     1,
       2,   195,   207,   257,   262,   265,   322,   274,   216,     5,
     -15,   305,     7,     8,   306,     9,   210,   271,    12,   217,
     272,    15,   212,   273,    18,   327,   320,   266,   268,   330,
     333,   278,   270,   279,   280,   281,   158,   277,   334,   335,
     340,   167,   168,   352,     5,   353,   255,     7,     8,   125,
     218,   291,   303,   292,   201,   282,   295,   149,   178,   179,
     245,   307,   308,   304,   261,   198,   309,   310,   313,   313,
      11,   321,   342,    14,   233,   329,    17,   234,   265,    20,
      10,   182,     4,    13,     5,   239,    16,     7,     8,    19,
     243,   294,   293,   328,   160,   347,   348,   142,   241,   209,
     287,   253,   254,   166,   323,     0,   325,     0,     0,     0,
       0,     0,     0,  -255,   221,   216,  -255,  -255,     0,     0,
     267,   222,  -255,   269,  -255,  -255,  -255,   223,     0,  -255,
    -255,   313,     0,     0,     0,   337,     0,   339,     0,     0,
       0,     0,   265,     0,     0,     0,  -255,     0,     1,     2,
     303,     0,   303,     0,   344,     0,     0,     0,     0,     0,
     351,     0,     0,   343,  -255,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,  -257,    79,
       0,     1,     2,     0,     0,     0,  -257,  -257,     0,  -257,
    -257,  -257,    80,    81,  -257,  -257,    82,   324,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,     1,     2,   288,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    81,     0,     0,
      82,   289,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   290,     1,
       2,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    81,     0,     0,    82,     0,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   286,     1,     2,     0,     0,     0,     0,     3,
       0,     4,   217,     5,     0,   148,     7,     8,     0,     0,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,     0,     0,    34,    35,    36,
      37,     0,    38,    39,    40,    41,     1,     2,   275,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    81,     0,
       0,    82,   276,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,     1,
       2,   349,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    81,     0,     0,    82,   350,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,     1,     2,     0,     0,     0,     0,     3,     0,
       4,     0,     5,     0,     6,     7,     8,     0,     0,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,     0,     0,    34,    35,    36,    37,
       0,    38,    39,    40,    41,     1,     2,     0,     0,     0,
       0,     3,     0,     4,     0,     5,     0,   148,     7,     8,
       0,     0,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,     0,     0,    34,
      35,    36,    37,     0,    38,    39,    40,    41,     1,     2,
       0,     0,     0,     0,     0,     0,   -14,     0,     0,     0,
      81,     0,     0,    82,     0,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,     1,     2,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    81,     0,     0,    82,     0,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,     1,     2,     0,     0,     0,     0,
       0,     0,     4,     0,     5,     0,   148,     7,     8,     0,
       0,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,     0,    33,     0,     0,    34,    35,
      36,    37,     0,    38,    39,    40,    41,  -148,   144,     0,
       1,     2,     0,     0,     0,  -148,  -148,     0,  -148,  -148,
    -148,     0,     0,  -148,  -148,     0,   -23,   180,     0,   -23,
     -23,     0,     0,     0,   -23,   181,     0,   -23,   -23,   -23,
      22,     0,   -23,   -23,     0,     0,     0,     0,     0,     0,
       0,     0,   -22,   180,     0,   -22,   -22,     0,  -148,   -23,
     -22,   181,     0,   -22,   -22,   -22,     0,     0,   -22,   -22,
       0,   -20,   283,     0,   -20,   -20,     0,   -23,     0,   -20,
     -20,     0,   -20,   217,   -20,   -22,     0,   -20,   -20,     1,
       2,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   -22,   -20,     0,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
       1,     2,   -20,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,     0,   150,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,   210,
     150,     0,     0,     4,     0,     5,     0,     0,     7,     8,
       0,     0,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,     0,   211,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-241))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       0,    23,    24,    25,    33,   218,    41,     3,     4,   249,
       0,     1,     5,    33,     0,     1,    33,     7,   152,     3,
       4,    11,    22,     3,     4,     6,     6,     3,     4,    29,
      30,    56,    32,    33,   168,    64,     3,     4,    31,    39,
      40,    41,    13,     3,     4,    65,     6,    76,    48,    66,
      46,    73,    52,    30,     3,     4,    76,    57,     8,    76,
      60,    51,    11,    13,    45,    51,    46,    67,    68,    69,
     283,   311,    56,    16,    17,    56,    57,    77,    45,    60,
      57,    62,     3,    60,    33,     0,     1,     3,     3,     4,
       0,     3,     4,     8,     9,    76,    11,    12,    13,    51,
      15,    16,    17,    15,    11,     1,     1,    12,     3,     4,
       3,     4,     8,     9,   143,    14,   138,    12,    33,     8,
      15,     8,    15,   143,     7,   147,   143,     7,   157,   164,
       5,     6,     8,    11,   127,    10,    51,   157,     8,    33,
     157,    56,    37,    38,    37,    38,     3,     4,     7,     9,
      38,   151,   152,    41,    11,   155,    13,     6,    33,    16,
      17,   161,    13,     8,   164,    16,    17,   167,   168,    57,
       3,     4,     5,    14,    62,    63,    33,   177,     6,     3,
       4,     6,    70,    56,   184,   185,     8,   209,    76,    13,
      11,    56,    16,    17,    56,    20,     7,   197,    23,    12,
     200,    26,   183,   203,    29,     7,    56,   188,   189,     9,
       7,   211,   193,   213,   214,   215,     5,   210,    56,    56,
       5,    45,    46,     5,    13,    12,   177,    16,    17,   222,
      77,   224,   232,   226,     6,   216,   229,    32,    48,    48,
     162,   241,   242,   236,   184,     6,   246,   247,   248,   249,
      22,   258,   322,    25,   142,   298,    28,   145,   258,    31,
      21,    52,    11,    24,    13,   153,    27,    16,    17,    30,
     158,   228,   227,   297,   162,   330,   332,    29,   157,    75,
     222,   169,   170,    40,   265,    -1,   267,    -1,    -1,    -1,
      -1,    -1,    -1,     0,     1,   183,     3,     4,    -1,    -1,
     188,     8,     9,   191,    11,    12,    13,    14,    -1,    16,
      17,   311,    -1,    -1,    -1,   315,    -1,   317,    -1,    -1,
      -1,    -1,   322,    -1,    -1,    -1,    33,    -1,     3,     4,
     330,    -1,   332,    -1,   327,    -1,    -1,    -1,    -1,    -1,
     333,    -1,    -1,   324,    51,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,     0,     1,
      -1,     3,     4,    -1,    -1,    -1,     8,     9,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,   265,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,     3,     4,     5,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,     3,
       4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    18,    -1,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,     3,     4,    -1,    -1,    -1,    -1,     9,
      -1,    11,    12,    13,    -1,    15,    16,    17,    -1,    -1,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    -1,    -1,    47,    48,    49,
      50,    -1,    52,    53,    54,    55,     3,     4,     5,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,     3,
       4,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,     3,     4,    -1,    -1,    -1,    -1,     9,    -1,
      11,    -1,    13,    -1,    15,    16,    17,    -1,    -1,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    -1,    -1,    47,    48,    49,    50,
      -1,    52,    53,    54,    55,     3,     4,    -1,    -1,    -1,
      -1,     9,    -1,    11,    -1,    13,    -1,    15,    16,    17,
      -1,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    -1,    -1,    47,
      48,    49,    50,    -1,    52,    53,    54,    55,     3,     4,
      -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,
      15,    -1,    -1,    18,    -1,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,     3,     4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    18,    -1,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,     3,     4,    -1,    -1,    -1,    -1,
      -1,    -1,    11,    -1,    13,    -1,    15,    16,    17,    -1,
      -1,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    -1,    44,    -1,    -1,    47,    48,
      49,    50,    -1,    52,    53,    54,    55,     0,     1,    -1,
       3,     4,    -1,    -1,    -1,     8,     9,    -1,    11,    12,
      13,    -1,    -1,    16,    17,    -1,     0,     1,    -1,     3,
       4,    -1,    -1,    -1,     8,     9,    -1,    11,    12,    13,
      33,    -1,    16,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     0,     1,    -1,     3,     4,    -1,    51,    33,
       8,     9,    -1,    11,    12,    13,    -1,    -1,    16,    17,
      -1,     0,     1,    -1,     3,     4,    -1,    51,    -1,     8,
       9,    -1,    11,    12,    13,    33,    -1,    16,    17,     3,
       4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    51,    33,    -1,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
       3,     4,    51,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,     7,
      33,    -1,    -1,    11,    -1,    13,    -1,    -1,    16,    17,
      -1,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     9,    11,    13,    15,    16,    17,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    47,    48,    49,    50,    52,    53,
      54,    55,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    71,    72,    73,    76,    88,    89,    90,    91,
      92,    93,    94,    99,   100,   101,   102,   103,   104,   105,
     112,   113,   114,   115,   116,   117,   120,   124,    56,     1,
      14,    15,    18,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
     118,   119,   120,   121,   122,   123,     1,     7,    62,   120,
     117,   117,   117,     3,    80,    81,    77,    78,    79,   123,
       3,    82,    99,   120,     1,    94,   120,   123,    15,    67,
      33,    96,    97,    98,   103,   104,   105,   120,     5,    68,
     116,     6,    69,   120,   111,   120,   122,    45,    46,    95,
     110,   116,   120,     0,    51,     1,    62,    12,    65,    66,
       1,     9,    76,   120,     8,     8,    62,    62,    94,   116,
      62,    94,    62,   116,   116,     6,   103,   106,     6,   104,
     107,     6,   105,   108,   120,   120,   120,   116,   117,   115,
       7,    33,    62,   103,   104,   105,   116,    12,    63,   125,
      14,     1,     8,    14,     7,   123,     7,     8,     8,     8,
     117,    11,    83,   116,   116,   117,     7,   120,    95,   116,
     120,    96,   104,   116,   120,    68,     6,     6,    70,   110,
     120,   120,    95,   116,   116,    60,     9,    56,     8,     1,
      56,    73,   120,    74,    75,   120,    62,   116,    62,   116,
      62,   120,   120,   120,   117,     5,    19,   123,   120,   120,
     120,   120,    62,     1,   125,    14,    56,   119,     5,    19,
      56,   123,   123,    81,    78,   123,    15,    37,    38,    84,
      85,    86,    87,   120,   123,    56,    56,   120,   120,   120,
     120,   109,   110,   120,   109,     6,     5,     6,    10,    33,
      56,    74,     8,    62,   116,    62,   125,     7,    80,    77,
       9,     1,     8,     7,    56,    56,   109,   120,     5,   120,
       5,    56,    75,    62,   123,     1,    12,    85,    87,     5,
      19,   123,     5,    12
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
#line 208 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 1926 "language.cc"
	break;
      case 4: /* "TOK_QSTRING" */

/* Line 1391 of yacc.c  */
#line 208 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 1935 "language.cc"
	break;
      case 5: /* "TOK_NUMBER" */

/* Line 1391 of yacc.c  */
#line 213 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 1944 "language.cc"
	break;
      case 15: /* "TOK_MSC" */

/* Line 1391 of yacc.c  */
#line 209 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 1953 "language.cc"
	break;
      case 16: /* "TOK_COLON_STRING" */

/* Line 1391 of yacc.c  */
#line 208 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 1962 "language.cc"
	break;
      case 17: /* "TOK_COLON_QUOTED_STRING" */

/* Line 1391 of yacc.c  */
#line 208 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 1971 "language.cc"
	break;
      case 18: /* "TOK_STYLE_NAME" */

/* Line 1391 of yacc.c  */
#line 208 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 1980 "language.cc"
	break;
      case 19: /* "TOK_BOOLEAN" */

/* Line 1391 of yacc.c  */
#line 213 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 1989 "language.cc"
	break;
      case 34: /* "TOK_COMMAND_HEADING" */

/* Line 1391 of yacc.c  */
#line 211 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 1998 "language.cc"
	break;
      case 35: /* "TOK_COMMAND_NUDGE" */

/* Line 1391 of yacc.c  */
#line 211 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2007 "language.cc"
	break;
      case 36: /* "TOK_COMMAND_NEWPAGE" */

/* Line 1391 of yacc.c  */
#line 211 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2016 "language.cc"
	break;
      case 37: /* "TOK_COMMAND_DEFCOLOR" */

/* Line 1391 of yacc.c  */
#line 210 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2025 "language.cc"
	break;
      case 38: /* "TOK_COMMAND_DEFSTYLE" */

/* Line 1391 of yacc.c  */
#line 210 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2034 "language.cc"
	break;
      case 39: /* "TOK_COMMAND_DEFDESIGN" */

/* Line 1391 of yacc.c  */
#line 210 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2043 "language.cc"
	break;
      case 40: /* "TOK_COMMAND_BIG" */

/* Line 1391 of yacc.c  */
#line 209 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2052 "language.cc"
	break;
      case 41: /* "TOK_COMMAND_PIPE" */

/* Line 1391 of yacc.c  */
#line 209 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2061 "language.cc"
	break;
      case 42: /* "TOK_COMMAND_MARK" */

/* Line 1391 of yacc.c  */
#line 212 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2070 "language.cc"
	break;
      case 43: /* "TOK_COMMAND_PARALLEL" */

/* Line 1391 of yacc.c  */
#line 212 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2079 "language.cc"
	break;
      case 44: /* "TOK_VERTICAL" */

/* Line 1391 of yacc.c  */
#line 213 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2088 "language.cc"
	break;
      case 45: /* "TOK_AT" */

/* Line 1391 of yacc.c  */
#line 213 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2097 "language.cc"
	break;
      case 46: /* "TOK_AT_POS" */

/* Line 1391 of yacc.c  */
#line 213 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2106 "language.cc"
	break;
      case 47: /* "TOK_SHOW" */

/* Line 1391 of yacc.c  */
#line 213 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2115 "language.cc"
	break;
      case 48: /* "TOK_HIDE" */

/* Line 1391 of yacc.c  */
#line 213 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2124 "language.cc"
	break;
      case 49: /* "TOK_ACTIVATE" */

/* Line 1391 of yacc.c  */
#line 213 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2133 "language.cc"
	break;
      case 50: /* "TOK_DEACTIVATE" */

/* Line 1391 of yacc.c  */
#line 213 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2142 "language.cc"
	break;
      case 51: /* "TOK_BYE" */

/* Line 1391 of yacc.c  */
#line 212 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2151 "language.cc"
	break;
      case 52: /* "TOK_COMMAND_VSPACE" */

/* Line 1391 of yacc.c  */
#line 214 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2160 "language.cc"
	break;
      case 54: /* "TOK_COMMAND_SYMBOL" */

/* Line 1391 of yacc.c  */
#line 214 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2169 "language.cc"
	break;
      case 55: /* "TOK_COMMAND_NOTE" */

/* Line 1391 of yacc.c  */
#line 214 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2178 "language.cc"
	break;
      case 60: /* "top_level_arclist" */

/* Line 1391 of yacc.c  */
#line 203 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1391 of yacc.c  */
#line 2187 "language.cc"
	break;
      case 62: /* "braced_arclist" */

/* Line 1391 of yacc.c  */
#line 203 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1391 of yacc.c  */
#line 2196 "language.cc"
	break;
      case 63: /* "arclist_maybe_no_semicolon" */

/* Line 1391 of yacc.c  */
#line 203 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1391 of yacc.c  */
#line 2205 "language.cc"
	break;
      case 64: /* "arclist" */

/* Line 1391 of yacc.c  */
#line 203 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1391 of yacc.c  */
#line 2214 "language.cc"
	break;
      case 65: /* "arc_with_parallel_semicolon" */

/* Line 1391 of yacc.c  */
#line 198 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2223 "language.cc"
	break;
      case 66: /* "arc_with_parallel" */

/* Line 1391 of yacc.c  */
#line 198 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2232 "language.cc"
	break;
      case 67: /* "arc" */

/* Line 1391 of yacc.c  */
#line 198 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2241 "language.cc"
	break;
      case 68: /* "full_arcattrlist_with_label_or_number" */

/* Line 1391 of yacc.c  */
#line 206 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1391 of yacc.c  */
#line 2250 "language.cc"
	break;
      case 72: /* "optlist" */

/* Line 1391 of yacc.c  */
#line 203 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1391 of yacc.c  */
#line 2259 "language.cc"
	break;
      case 73: /* "opt" */

/* Line 1391 of yacc.c  */
#line 198 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2268 "language.cc"
	break;
      case 74: /* "entitylist" */

/* Line 1391 of yacc.c  */
#line 204 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entitylist);};

/* Line 1391 of yacc.c  */
#line 2277 "language.cc"
	break;
      case 75: /* "entity" */

/* Line 1391 of yacc.c  */
#line 204 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entitylist);};

/* Line 1391 of yacc.c  */
#line 2286 "language.cc"
	break;
      case 76: /* "first_entity" */

/* Line 1391 of yacc.c  */
#line 204 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entitylist);};

/* Line 1391 of yacc.c  */
#line 2295 "language.cc"
	break;
      case 79: /* "tok_stringlist" */

/* Line 1391 of yacc.c  */
#line 205 "language.yy"
	{if (!C_S_H) delete (yyvaluep->stringlist);};

/* Line 1391 of yacc.c  */
#line 2304 "language.cc"
	break;
      case 88: /* "parallel" */

/* Line 1391 of yacc.c  */
#line 202 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcparallel);};

/* Line 1391 of yacc.c  */
#line 2313 "language.cc"
	break;
      case 89: /* "box_list" */

/* Line 1391 of yacc.c  */
#line 201 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcboxseries);};

/* Line 1391 of yacc.c  */
#line 2322 "language.cc"
	break;
      case 90: /* "first_box" */

/* Line 1391 of yacc.c  */
#line 201 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbox);};

/* Line 1391 of yacc.c  */
#line 2331 "language.cc"
	break;
      case 91: /* "first_pipe" */

/* Line 1391 of yacc.c  */
#line 201 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcpipe);};

/* Line 1391 of yacc.c  */
#line 2340 "language.cc"
	break;
      case 92: /* "pipe_list_no_content" */

/* Line 1391 of yacc.c  */
#line 201 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcpipeseries);};

/* Line 1391 of yacc.c  */
#line 2349 "language.cc"
	break;
      case 93: /* "pipe_list" */

/* Line 1391 of yacc.c  */
#line 201 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcpipeseries);};

/* Line 1391 of yacc.c  */
#line 2358 "language.cc"
	break;
      case 94: /* "boxrel" */

/* Line 1391 of yacc.c  */
#line 201 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbox);};

/* Line 1391 of yacc.c  */
#line 2367 "language.cc"
	break;
      case 95: /* "vertxpos" */

/* Line 1391 of yacc.c  */
#line 196 "language.yy"
	{if (!C_S_H) delete (yyvaluep->vertxpos);};

/* Line 1391 of yacc.c  */
#line 2376 "language.cc"
	break;
      case 97: /* "vertrel_no_xpos" */

/* Line 1391 of yacc.c  */
#line 197 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcvertarrow);};

/* Line 1391 of yacc.c  */
#line 2385 "language.cc"
	break;
      case 98: /* "vertrel" */

/* Line 1391 of yacc.c  */
#line 198 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2394 "language.cc"
	break;
      case 99: /* "arcrel" */

/* Line 1391 of yacc.c  */
#line 198 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2403 "language.cc"
	break;
      case 100: /* "arcrel_to" */

/* Line 1391 of yacc.c  */
#line 200 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 1391 of yacc.c  */
#line 2412 "language.cc"
	break;
      case 101: /* "arcrel_from" */

/* Line 1391 of yacc.c  */
#line 200 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 1391 of yacc.c  */
#line 2421 "language.cc"
	break;
      case 102: /* "arcrel_bidir" */

/* Line 1391 of yacc.c  */
#line 200 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 1391 of yacc.c  */
#line 2430 "language.cc"
	break;
      case 111: /* "symbol_type_string" */

/* Line 1391 of yacc.c  */
#line 207 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2439 "language.cc"
	break;
      case 112: /* "symbol_command_no_attr" */

/* Line 1391 of yacc.c  */
#line 199 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2448 "language.cc"
	break;
      case 113: /* "symbol_command" */

/* Line 1391 of yacc.c  */
#line 199 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2457 "language.cc"
	break;
      case 114: /* "note" */

/* Line 1391 of yacc.c  */
#line 199 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2466 "language.cc"
	break;
      case 115: /* "colon_string" */

/* Line 1391 of yacc.c  */
#line 207 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2475 "language.cc"
	break;
      case 116: /* "full_arcattrlist_with_label" */

/* Line 1391 of yacc.c  */
#line 205 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1391 of yacc.c  */
#line 2484 "language.cc"
	break;
      case 117: /* "full_arcattrlist" */

/* Line 1391 of yacc.c  */
#line 205 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1391 of yacc.c  */
#line 2493 "language.cc"
	break;
      case 118: /* "arcattrlist" */

/* Line 1391 of yacc.c  */
#line 205 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1391 of yacc.c  */
#line 2502 "language.cc"
	break;
      case 119: /* "arcattr" */

/* Line 1391 of yacc.c  */
#line 205 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attrib);};

/* Line 1391 of yacc.c  */
#line 2511 "language.cc"
	break;
      case 120: /* "entity_string" */

/* Line 1391 of yacc.c  */
#line 207 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2520 "language.cc"
	break;
      case 121: /* "reserved_word_string" */

/* Line 1391 of yacc.c  */
#line 207 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2529 "language.cc"
	break;
      case 122: /* "symbol_string" */

/* Line 1391 of yacc.c  */
#line 207 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2538 "language.cc"
	break;
      case 123: /* "string" */

/* Line 1391 of yacc.c  */
#line 207 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2547 "language.cc"
	break;
      case 125: /* "scope_close" */

/* Line 1391 of yacc.c  */
#line 198 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2556 "language.cc"
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
#line 220 "language.yy"
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
#line 230 "language.yy"
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
#line 241 "language.yy"
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
#line 252 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(1) - (2)])))
        csh.AddDesignsToHints();
  #else
    msc.AddArcs((yyvsp[(2) - (2)].arclist));
  #endif
    YYACCEPT;
}
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 262 "language.yy"
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
#line 272 "language.yy"
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
#line 282 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    msc.AddArcs((yyvsp[(1) - (1)].arclist));
  #endif
}
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 289 "language.yy"
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
#line 303 "language.yy"
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
#line 312 "language.yy"
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
#line 326 "language.yy"
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
#line 338 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
    csh.AddCSH((yylsp[(3) - (3)]), COLOR_DESIGNNAME);
    csh.CheckHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_VALUE, "msc");
    csh.SetDesignTo((yyvsp[(1) - (3)].str));
  #else
    msc.AddAttribute(Attribute("msc", (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(1) - (3)])), MSC_POS((yylsp[(3) - (3)]))));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 353 "language.yy"
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
#line 366 "language.yy"
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
#line 379 "language.yy"
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
#line 395 "language.yy"
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
#line 407 "language.yy"
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
#line 421 "language.yy"
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
#line 435 "language.yy"
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
#line 451 "language.yy"
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
#line 461 "language.yy"
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
#line 472 "language.yy"
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
#line 488 "language.yy"
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
#line 500 "language.yy"
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
#line 515 "language.yy"
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
#line 523 "language.yy"
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
#line 546 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.arcbase)=((yyvsp[(1) - (1)].arcbase))->AddAttributeList(NULL);
  #endif
}
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 553 "language.yy"
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
#line 564 "language.yy"
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
#line 576 "language.yy"
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
#line 591 "language.yy"
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
#line 609 "language.yy"
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
#line 621 "language.yy"
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
#line 631 "language.yy"
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
#line 645 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(1) - (1)])))
        ArcDivider::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(1) - (1)])))
        ArcDivider::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcbase) = (new ArcDivider(MSC_ARC_VSPACE, &msc))->AddAttributeList((yyvsp[(1) - (1)].attriblist));
  #endif
}
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 656 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (new CommandEntity((yyvsp[(1) - (1)].entitylist), &msc))->AddAttributeList(NULL);
  #endif
}
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 663 "language.yy"
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
#line 675 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
  #else
    CommandEntity *ce = new CommandEntity((yyvsp[(2) - (2)].entitylist), &msc);
    ce->AddAttributeList(NULL);
	(yyval.arcbase) = ce->ApplyPrefix((yyvsp[(1) - (2)].str));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 687 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_COMMA);
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
    csh.AddCSH_ErrorAfter((yylsp[(2) - (2)]), "Missing an entity.");
  #else
    CommandEntity *ce = new CommandEntity((yyvsp[(1) - (2)].entitylist), &msc);
    (yyval.arcbase) = ce->AddAttributeList(NULL);
    msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).end.NextChar(), "Missing an entity.");
  #endif
}
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 699 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    csh.CheckEntityHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]));
  #else
    CommandEntity *ce = new CommandEntity(((yyvsp[(3) - (3)].entitylist))->Prepend((yyvsp[(1) - (3)].entitylist)), &msc);
	delete ((yyvsp[(1) - (3)].entitylist));
    (yyval.arcbase) = ce->AddAttributeList(NULL);
  #endif
}
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 710 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[(3) - (3)]), COLOR_COMMA);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[(1) - (3)]), (yylsp[(2) - (3)]));
    csh.CheckEntityHintAfter((yylsp[(3) - (3)]), yylloc, yychar==YYEOF);
    csh.AddCSH_ErrorAfter((yylsp[(3) - (3)]), "Missing an entity.");
  #else
    CommandEntity *ce = new CommandEntity((yyvsp[(2) - (3)].entitylist), &msc);
    ce->AddAttributeList(NULL);
	(yyval.arcbase) = ce->ApplyPrefix((yyvsp[(1) - (3)].str));
    msc.Error.Error(MSC_POS((yylsp[(3) - (3)])).end.NextChar(), "Missing an entity.");
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 726 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (4)]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[(3) - (4)]), COLOR_COMMA);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[(1) - (4)]), (yylsp[(2) - (4)]));
    csh.CheckEntityHintAtAndBefore((yylsp[(3) - (4)]), (yylsp[(4) - (4)]));
  #else
    CommandEntity *ce = new CommandEntity(((yyvsp[(4) - (4)].entitylist))->Prepend((yyvsp[(2) - (4)].entitylist)), &msc);
	delete ((yyvsp[(2) - (4)].entitylist));
    ce->AddAttributeList(NULL);
    (yyval.arcbase) = ce->ApplyPrefix((yyvsp[(1) - (4)].str));
  #endif
    free((yyvsp[(1) - (4)].str));
}
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 741 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    /* If there were arcs defined by the options (e.g., background)
     * enclose them in a single item parallel element. */
    if ((yyvsp[(1) - (1)].arclist)) {
        (yyval.arcbase) = (new ArcParallel(&msc))->AddArcList((yyvsp[(1) - (1)].arclist))->AddAttributeList(NULL);
    } else
        (yyval.arcbase) = NULL;
  #endif
}
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 753 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcboxseries); //to remove warning for downcast
  #endif
}
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 760 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcpipeseries); //to remove warning for downcast
  #endif
}
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 767 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = ((yyvsp[(1) - (1)].arcparallel))->AddAttributeList(NULL);
  #endif
}
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 774 "language.yy"
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
#line 783 "language.yy"
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
#line 792 "language.yy"
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
#line 801 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = (new CommandEntity(NULL, &msc))->AddAttributeList(NULL);
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 810 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (2)])))
        CommandEntity::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (2)])))
        CommandEntity::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcbase) = (new CommandEntity(NULL, &msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 823 "language.yy"
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
#line 832 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (2)])))
        ArcDivider::AttributeNames(csh, true);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (2)])))
        ArcDivider::AttributeValues(csh.hintAttrName, csh, true);
  #else
    (yyval.arcbase) = (new ArcDivider(MSC_COMMAND_NUDGE, &msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 845 "language.yy"
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

  case 61:

/* Line 1806 of yacc.c  */
#line 857 "language.yy"
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

  case 62:

/* Line 1806 of yacc.c  */
#line 873 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
  #else
        (yyval.arcbase) = (new CommandNewpage(&msc))->AddAttributeList(NULL);
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 882 "language.yy"
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

  case 66:

/* Line 1806 of yacc.c  */
#line 897 "language.yy"
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

  case 67:

/* Line 1806 of yacc.c  */
#line 910 "language.yy"
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

  case 68:

/* Line 1806 of yacc.c  */
#line 925 "language.yy"
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

  case 69:

/* Line 1806 of yacc.c  */
#line 936 "language.yy"
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

  case 70:

/* Line 1806 of yacc.c  */
#line 949 "language.yy"
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

  case 72:

/* Line 1806 of yacc.c  */
#line 966 "language.yy"
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

  case 73:

/* Line 1806 of yacc.c  */
#line 977 "language.yy"
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

  case 74:

/* Line 1806 of yacc.c  */
#line 988 "language.yy"
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

  case 75:

/* Line 1806 of yacc.c  */
#line 1000 "language.yy"
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

  case 76:

/* Line 1806 of yacc.c  */
#line 1011 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF);
  #else
    (yyval.namerel) = NULL;
  #endif
}
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 1020 "language.yy"
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

  case 78:

/* Line 1806 of yacc.c  */
#line 1034 "language.yy"
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

  case 79:

/* Line 1806 of yacc.c  */
#line 1059 "language.yy"
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

  case 80:

/* Line 1806 of yacc.c  */
#line 1071 "language.yy"
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

  case 81:

/* Line 1806 of yacc.c  */
#line 1082 "language.yy"
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

  case 82:

/* Line 1806 of yacc.c  */
#line 1092 "language.yy"
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

  case 87:

/* Line 1806 of yacc.c  */
#line 1109 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(1) - (1)].arcbase)) {
        (yyval.arclist) = (new ArcList)->Append((yyvsp[(1) - (1)].arcbase)); /* New list */
        ((yyvsp[(1) - (1)].arcbase))->MakeMeLastNotable();
    } else
        (yyval.arclist) = NULL;
  #endif
}
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 1119 "language.yy"
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
        ((yyvsp[(3) - (3)].arcbase))->MakeMeLastNotable();
    } else
        (yyval.arclist) = (yyvsp[(1) - (3)].arclist);
  #endif
}
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 1138 "language.yy"
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

  case 91:

/* Line 1806 of yacc.c  */
#line 1152 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    csh.AddCSH_Error((yylsp[(3) - (3)]), "An option expected here.");
  #else
    (yyval.arclist) = (yyvsp[(1) - (3)].arclist);
  #endif
}
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 1163 "language.yy"
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
    msc.AddAttribute(Attribute((yyvsp[(1) - (3)].str), str2bool((yyvsp[(3) - (3)].str)), MSC_POS((yylsp[(1) - (3)])), MSC_POS((yylsp[(3) - (3)])), (yyvsp[(3) - (3)].str)));
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 1183 "language.yy"
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
    msc.AddAttribute(Attribute((yyvsp[(1) - (3)].str), atof((yyvsp[(3) - (3)].str)), MSC_POS((yylsp[(1) - (3)])), MSC_POS((yylsp[(3) - (3)])), (yyvsp[(3) - (3)].str)));
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 1203 "language.yy"
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
    Attribute a((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(1) - (3)])), MSC_POS((yylsp[(3) - (3)])));
    MscFillAttr fill;
    fill.Empty();
    if (a.StartsWith("background") && fill.AddAttribute(a, &msc, STYLE_OPTION)) {
        (yyval.arcbase) = (new CommandNewBackground(&msc, fill))->AddAttributeList(NULL);
    } else {
        msc.AddAttribute(a);
        (yyval.arcbase) = NULL;
    }
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 1230 "language.yy"
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

  case 96:

/* Line 1806 of yacc.c  */
#line 1250 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_DESIGNNAME);
        if (csh.CheckHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_VALUE, "msc")) {
            csh.AddDesignsToHints();
            csh.hintStatus = HINT_READY;
        }
        csh.SetDesignTo((yyvsp[(3) - (3)].str));
  #else
        msc.AddAttribute(Attribute("msc", (yyvsp[(3) - (3)].str), MSC_POS((yyloc)), MSC_POS((yylsp[(3) - (3)]))));
        (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 1268 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_EQUAL);
        csh.AddCSH_ErrorAfter((yylsp[(2) - (2)]), "Missing option value.");
        if (csh.CheckHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF, HINT_ATTR_VALUE, "msc")) {
            csh.AddDesignsToHints();
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

  case 98:

/* Line 1806 of yacc.c  */
#line 1289 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.entitylist) = (EntityDefList*)((yyvsp[(1) - (1)].entitylist));
  #endif
}
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 1295 "language.yy"
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

  case 100:

/* Line 1806 of yacc.c  */
#line 1305 "language.yy"
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

  case 102:

/* Line 1806 of yacc.c  */
#line 1320 "language.yy"
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

  case 103:

/* Line 1806 of yacc.c  */
#line 1336 "language.yy"
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

  case 104:

/* Line 1806 of yacc.c  */
#line 1348 "language.yy"
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

  case 105:

/* Line 1806 of yacc.c  */
#line 1364 "language.yy"
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

  case 106:

/* Line 1806 of yacc.c  */
#line 1377 "language.yy"
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

  case 107:

/* Line 1806 of yacc.c  */
#line 1395 "language.yy"
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

  case 108:

/* Line 1806 of yacc.c  */
#line 1410 "language.yy"
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

  case 109:

/* Line 1806 of yacc.c  */
#line 1428 "language.yy"
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

  case 111:

/* Line 1806 of yacc.c  */
#line 1445 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #endif
}
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 1452 "language.yy"
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
    for (std::list<std::string>::iterator i = ((yyvsp[(1) - (2)].stringlist))->begin(); i!=((yyvsp[(1) - (2)].stringlist))->end(); i++) {
        MscStyle style = msc.Contexts.back().styles.GetStyle(*i);
        AttributeList::iterator j=((yyvsp[(2) - (2)].attriblist))->begin();
        while (j!=((yyvsp[(2) - (2)].attriblist))->end()) {
           if (!style.AddAttribute(**j, &msc))
               msc.Error.Error(**j, false, "Attribute '" + (*j)->name + "' is not applicable to styles. Ignoring it.");
           j++;
        }
        msc.Contexts.back().styles[*i] = style;
    }
    delete((yyvsp[(1) - (2)].stringlist));
    delete((yyvsp[(2) - (2)].attriblist));
  #endif
}
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 1478 "language.yy"
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

  case 114:

/* Line 1806 of yacc.c  */
#line 1494 "language.yy"
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

  case 116:

/* Line 1806 of yacc.c  */
#line 1513 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #endif
}
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 1520 "language.yy"
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

  case 118:

/* Line 1806 of yacc.c  */
#line 1536 "language.yy"
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

  case 119:

/* Line 1806 of yacc.c  */
#line 1556 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (6)]), COLOR_DESIGNNAME);
    csh.AddCSH((yylsp[(2) - (6)]), COLOR_BRACE);
    csh.AddCSH((yylsp[(4) - (6)]), COLOR_SEMICOLON);
    csh.AddCSH_Error((yylsp[(5) - (6)]), "Could not recognize this as part of a design definition.");
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

  case 120:

/* Line 1806 of yacc.c  */
#line 1579 "language.yy"
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

  case 122:

/* Line 1806 of yacc.c  */
#line 1591 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SEMICOLON);
  #endif
}
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 1598 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 1605 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 1615 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #endif
}
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 1621 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[(2) - (2)]), "Extra stuff after design options. Maybe missing a comma?");
  #endif
}
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 1628 "language.yy"
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

  case 130:

/* Line 1806 of yacc.c  */
#line 1647 "language.yy"
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

  case 131:

/* Line 1806 of yacc.c  */
#line 1666 "language.yy"
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

  case 132:

/* Line 1806 of yacc.c  */
#line 1685 "language.yy"
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
  #else
    msc.AddDesignAttribute(Attribute("msc", (yyvsp[(3) - (3)].str), MSC_POS((yyloc)), MSC_POS((yylsp[(3) - (3)]))));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 1706 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(1) - (1)].arclist))
        (yyval.arcparallel) = (new ArcParallel(&msc))->AddArcList((yyvsp[(1) - (1)].arclist));
    else
        (yyval.arcparallel) = NULL;
  #endif
}
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 1715 "language.yy"
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

  case 135:

/* Line 1806 of yacc.c  */
#line 1727 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.arcboxseries) = new ArcBoxSeries((yyvsp[(1) - (1)].arcbox));
    ((yyvsp[(1) - (1)].arcbox))->MakeMeLastNotable();
  #endif
}
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 1735 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(2) - (2)].arcbox))->SetLineEnd(MSC_POS((yylsp[(2) - (2)])));
    (yyval.arcboxseries) = ((yyvsp[(1) - (2)].arcboxseries))->AddFollow((yyvsp[(2) - (2)].arcbox));
    ((yyvsp[(2) - (2)].arcbox))->AddAttributeList(NULL); //should come after AddFollow
    ((yyvsp[(1) - (2)].arcboxseries))->MakeMeLastNotable();
  #endif
}
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 1744 "language.yy"
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

  case 138:

/* Line 1806 of yacc.c  */
#line 1758 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(2) - (3)].arcbox))->AddArcList((yyvsp[(3) - (3)].arclist))->SetLineEnd(MSC_POS((yylsp[(2) - (3)])));
    (yyval.arcboxseries) = ((yyvsp[(1) - (3)].arcboxseries))->AddFollow((yyvsp[(2) - (3)].arcbox));
    ((yyvsp[(2) - (3)].arcbox))->AddAttributeList(NULL); //should come after AddFollow
    ((yyvsp[(1) - (3)].arcboxseries))->MakeMeLastNotable();
  #endif
}
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 1767 "language.yy"
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

  case 140:

/* Line 1806 of yacc.c  */
#line 1777 "language.yy"
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

  case 141:

/* Line 1806 of yacc.c  */
#line 1791 "language.yy"
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

  case 142:

/* Line 1806 of yacc.c  */
#line 1808 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(1) - (1)].arcbox))->AddAttributeList(NULL);
    ((yyvsp[(1) - (1)].arcbox))->SetLineEnd(MSC_POS((yyloc)));
    (yyval.arcbox) = (yyvsp[(1) - (1)].arcbox);
  #endif
}
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 1816 "language.yy"
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

  case 144:

/* Line 1806 of yacc.c  */
#line 1829 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(1) - (2)].arcbox))->AddAttributeList(NULL);
    ((yyvsp[(1) - (2)].arcbox))->SetLineEnd(MSC_POS((yylsp[(1) - (2)])));
    (yyval.arcbox) = ((yyvsp[(1) - (2)].arcbox))->AddArcList((yyvsp[(2) - (2)].arclist));
  #endif
}
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 1837 "language.yy"
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

  case 146:

/* Line 1806 of yacc.c  */
#line 1852 "language.yy"
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

  case 147:

/* Line 1806 of yacc.c  */
#line 1867 "language.yy"
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

  case 148:

/* Line 1806 of yacc.c  */
#line 1883 "language.yy"
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

  case 149:

/* Line 1806 of yacc.c  */
#line 1894 "language.yy"
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

  case 150:

/* Line 1806 of yacc.c  */
#line 1913 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcpipeseries) = new ArcPipeSeries((yyvsp[(1) - (1)].arcpipe));
  #endif
}
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 1920 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ArcPipe *ap = new ArcPipe((yyvsp[(2) - (2)].arcbox));
    ap->MakeMeLastNotable();
    ap->SetLineEnd(MSC_POS((yylsp[(2) - (2)])));
    (yyval.arcpipeseries) = ((yyvsp[(1) - (2)].arcpipeseries))->AddFollowWithAttributes(ap, NULL);
  #endif
}
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 1929 "language.yy"
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

  case 154:

/* Line 1806 of yacc.c  */
#line 1945 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.arcpipeseries) = ((yyvsp[(1) - (2)].arcpipeseries))->AddArcList((yyvsp[(2) - (2)].arclist));
  #endif
}
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 1952 "language.yy"
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

  case 156:

/* Line 1806 of yacc.c  */
#line 1967 "language.yy"
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

  case 157:

/* Line 1806 of yacc.c  */
#line 1979 "language.yy"
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

  case 158:

/* Line 1806 of yacc.c  */
#line 1992 "language.yy"
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

  case 159:

/* Line 1806 of yacc.c  */
#line 2003 "language.yy"
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

  case 160:

/* Line 1806 of yacc.c  */
#line 2015 "language.yy"
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

  case 161:

/* Line 1806 of yacc.c  */
#line 2029 "language.yy"
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

  case 162:

/* Line 1806 of yacc.c  */
#line 2043 "language.yy"
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

  case 163:

/* Line 1806 of yacc.c  */
#line 2058 "language.yy"
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

  case 164:

/* Line 1806 of yacc.c  */
#line 2071 "language.yy"
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

  case 165:

/* Line 1806 of yacc.c  */
#line 2086 "language.yy"
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

  case 166:

/* Line 1806 of yacc.c  */
#line 2120 "language.yy"
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

  case 167:

/* Line 1806 of yacc.c  */
#line 2136 "language.yy"
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

  case 168:

/* Line 1806 of yacc.c  */
#line 2154 "language.yy"
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

  case 169:

/* Line 1806 of yacc.c  */
#line 2166 "language.yy"
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

  case 173:

/* Line 1806 of yacc.c  */
#line 2184 "language.yy"
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

  case 174:

/* Line 1806 of yacc.c  */
#line 2198 "language.yy"
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

  case 175:

/* Line 1806 of yacc.c  */
#line 2209 "language.yy"
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

  case 176:

/* Line 1806 of yacc.c  */
#line 2221 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
    csh.CheckHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[(1) - (1)].arctype), MARKER_HERE_STR, MARKER_PREV_PARALLEL_STR, &msc);
  #endif
}
    break;

  case 177:

/* Line 1806 of yacc.c  */
#line 2230 "language.yy"
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

  case 178:

/* Line 1806 of yacc.c  */
#line 2244 "language.yy"
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

  case 179:

/* Line 1806 of yacc.c  */
#line 2255 "language.yy"
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

  case 180:

/* Line 1806 of yacc.c  */
#line 2267 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
    csh.CheckHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[(1) - (1)].arctype), MARKER_PREV_PARALLEL_STR, MARKER_HERE_STR, &msc);
  #endif
}
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 2276 "language.yy"
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

  case 182:

/* Line 1806 of yacc.c  */
#line 2288 "language.yy"
    {free((yyvsp[(2) - (3)].str)); (yyval.arcvertarrow)=NULL;}
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 2289 "language.yy"
    {free((yyvsp[(1) - (4)].str)); free((yyvsp[(3) - (4)].str)); (yyval.arcvertarrow)=NULL;}
    break;

  case 184:

/* Line 1806 of yacc.c  */
#line 2290 "language.yy"
    {free((yyvsp[(2) - (3)].str)); (yyval.arcvertarrow)=NULL;}
    break;

  case 185:

/* Line 1806 of yacc.c  */
#line 2291 "language.yy"
    {free((yyvsp[(1) - (4)].str)); free((yyvsp[(3) - (4)].str)); (yyval.arcvertarrow)=NULL;}
    break;

  case 186:

/* Line 1806 of yacc.c  */
#line 2295 "language.yy"
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

  case 187:

/* Line 1806 of yacc.c  */
#line 2306 "language.yy"
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

  case 188:

/* Line 1806 of yacc.c  */
#line 2317 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
  #else
    (yyval.arcbase) = new ArcDivider((yyvsp[(1) - (1)].arctype), &msc);
  #endif
}
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 2325 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 2329 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 2333 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 2338 "language.yy"
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

  case 193:

/* Line 1806 of yacc.c  */
#line 2352 "language.yy"
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

  case 194:

/* Line 1806 of yacc.c  */
#line 2363 "language.yy"
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

  case 195:

/* Line 1806 of yacc.c  */
#line 2375 "language.yy"
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

  case 196:

/* Line 1806 of yacc.c  */
#line 2386 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(1) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment((yyvsp[(2) - (2)].arctype), NULL, MSC_POS((yylsp[(2) - (2)])), MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 197:

/* Line 1806 of yacc.c  */
#line 2397 "language.yy"
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

  case 198:

/* Line 1806 of yacc.c  */
#line 2411 "language.yy"
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

  case 199:

/* Line 1806 of yacc.c  */
#line 2422 "language.yy"
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

  case 200:

/* Line 1806 of yacc.c  */
#line 2434 "language.yy"
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

  case 201:

/* Line 1806 of yacc.c  */
#line 2445 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment((yyvsp[(2) - (2)].arctype), NULL, MSC_POS((yylsp[(2) - (2)])), MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 2455 "language.yy"
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

  case 203:

/* Line 1806 of yacc.c  */
#line 2469 "language.yy"
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

  case 204:

/* Line 1806 of yacc.c  */
#line 2480 "language.yy"
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

  case 205:

/* Line 1806 of yacc.c  */
#line 2492 "language.yy"
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

  case 206:

/* Line 1806 of yacc.c  */
#line 2503 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment((yyvsp[(2) - (2)].arctype), NULL, MSC_POS((yylsp[(2) - (2)])), MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 220:

/* Line 1806 of yacc.c  */
#line 2516 "language.yy"
    {(yyval.arctype)=MSC_ARC_UNDETERMINED_SEGMENT;}
    break;

  case 222:

/* Line 1806 of yacc.c  */
#line 2517 "language.yy"
    {(yyval.arctype)=MSC_ARC_UNDETERMINED_SEGMENT;}
    break;

  case 224:

/* Line 1806 of yacc.c  */
#line 2518 "language.yy"
    {(yyval.arctype)=MSC_ARC_UNDETERMINED_SEGMENT;}
    break;

  case 226:

/* Line 1806 of yacc.c  */
#line 2522 "language.yy"
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

  case 227:

/* Line 1806 of yacc.c  */
#line 2545 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ExtvxposDesignatorName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));

  #else
    (yyval.extvertxpos) = new ExtVertXPos((yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), (yyvsp[(2) - (2)].vertxpos));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 228:

/* Line 1806 of yacc.c  */
#line 2555 "language.yy"
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

  case 231:

/* Line 1806 of yacc.c  */
#line 2571 "language.yy"
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

  case 232:

/* Line 1806 of yacc.c  */
#line 2607 "language.yy"
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

  case 233:

/* Line 1806 of yacc.c  */
#line 2630 "language.yy"
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

  case 234:

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
    csh.CheckHintBetween((yylsp[(2) - (4)]), (yylsp[(3) - (4)]), HINT_MARKER);
  #else
    (yyval.arcbase) = new CommandSymbol(&msc, (yyvsp[(2) - (4)].str), NULL, (yyvsp[(3) - (4)].extvertxpos), (yyvsp[(4) - (4)].extvertxpos));
  #endif
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(2) - (4)].str));
}
    break;

  case 235:

/* Line 1806 of yacc.c  */
#line 2690 "language.yy"
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

  case 236:

/* Line 1806 of yacc.c  */
#line 2726 "language.yy"
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

  case 237:

/* Line 1806 of yacc.c  */
#line 2763 "language.yy"
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

  case 238:

/* Line 1806 of yacc.c  */
#line 2799 "language.yy"
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

  case 240:

/* Line 1806 of yacc.c  */
#line 2817 "language.yy"
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

  case 241:

/* Line 1806 of yacc.c  */
#line 2829 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
  #else
    ArcBase *a = new CommandNote(&msc, (yyvsp[(2) - (3)].extvertxpos), (yyvsp[(3) - (3)].attriblist)); //This attaches itself to the target of the note
    if (!a->IsValid()) delete a; //if attachment not successful, drop it
    (yyval.arcbase) = NULL; //no need to add to arclist
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 242:

/* Line 1806 of yacc.c  */
#line 2840 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
  #else
    ArcBase *a;
    if ((yyvsp[(2) - (3)].vertxpos)) {
        const ExtVertXPos extvertxpos((yyvsp[(2) - (3)].vertxpos));
        a = new CommandNote(&msc, &extvertxpos, (yyvsp[(3) - (3)].attriblist)); //This attaches itself to the target of the note
    } else
        a = new CommandNote(&msc, NULL, (yyvsp[(3) - (3)].attriblist));
    if (!a->IsValid()) delete a; //if attachment not successful, drop it
    (yyval.arcbase) = NULL; //no need to add to arclist
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 243:

/* Line 1806 of yacc.c  */
#line 2856 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
    ArcBase *a = new CommandNote(&msc, NULL, (yyvsp[(2) - (2)].attriblist)); //This attaches itself to the target of the note
    if (!a->IsValid()) delete a; //if attachment not successful, drop it
    (yyval.arcbase) = NULL; //no need to add to arclist
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 244:

/* Line 1806 of yacc.c  */
#line 2869 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_ColonString((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str), false);
  #endif
    (yyval.str) = (yyvsp[(1) - (1)].str);
}
    break;

  case 245:

/* Line 1806 of yacc.c  */
#line 2876 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_ColonString((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str), true);
  #endif
    (yyval.str) = (yyvsp[(1) - (1)].str);
}
    break;

  case 246:

/* Line 1806 of yacc.c  */
#line 2884 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = (new AttributeList)->Append(new Attribute("label", (yyvsp[(1) - (1)].str), MSC_POS((yyloc)), MSC_POS((yyloc)).IncStartCol()));
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 247:

/* Line 1806 of yacc.c  */
#line 2892 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = ((yyvsp[(2) - (2)].attriblist))->Prepend(new Attribute("label", (yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), MSC_POS((yylsp[(1) - (2)])).IncStartCol()));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 248:

/* Line 1806 of yacc.c  */
#line 2900 "language.yy"
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

  case 249:

/* Line 1806 of yacc.c  */
#line 2912 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = ((yyvsp[(1) - (2)].attriblist))->Append(new Attribute("label", (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])), MSC_POS((yylsp[(2) - (2)])).IncStartCol()));
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 251:

/* Line 1806 of yacc.c  */
#line 2923 "language.yy"
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

  case 252:

/* Line 1806 of yacc.c  */
#line 2933 "language.yy"
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

  case 253:

/* Line 1806 of yacc.c  */
#line 2943 "language.yy"
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

  case 254:

/* Line 1806 of yacc.c  */
#line 2954 "language.yy"
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

  case 255:

/* Line 1806 of yacc.c  */
#line 2965 "language.yy"
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

  case 256:

/* Line 1806 of yacc.c  */
#line 2976 "language.yy"
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

  case 257:

/* Line 1806 of yacc.c  */
#line 2987 "language.yy"
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

  case 258:

/* Line 1806 of yacc.c  */
#line 3000 "language.yy"
    {
    (yyval.attriblist) = NULL;
}
    break;

  case 259:

/* Line 1806 of yacc.c  */
#line 3004 "language.yy"
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

  case 260:

/* Line 1806 of yacc.c  */
#line 3016 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.attriblist) = (new AttributeList)->Append((yyvsp[(1) - (1)].attrib));
  #endif
}
    break;

  case 261:

/* Line 1806 of yacc.c  */
#line 3023 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    csh.CheckHintBetween((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_NAME);
  #else
    (yyval.attriblist) = ((yyvsp[(1) - (3)].attriblist))->Append((yyvsp[(3) - (3)].attrib));
  #endif
}
    break;

  case 262:

/* Line 1806 of yacc.c  */
#line 3032 "language.yy"
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

  case 264:

/* Line 1806 of yacc.c  */
#line 3044 "language.yy"
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

  case 265:

/* Line 1806 of yacc.c  */
#line 3058 "language.yy"
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

  case 266:

/* Line 1806 of yacc.c  */
#line 3072 "language.yy"
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

  case 267:

/* Line 1806 of yacc.c  */
#line 3086 "language.yy"
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

  case 268:

/* Line 1806 of yacc.c  */
#line 3100 "language.yy"
    {
    (yyval.attrib) = NULL;
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 269:

/* Line 1806 of yacc.c  */
#line 3105 "language.yy"
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

  case 295:

/* Line 1806 of yacc.c  */
#line 3128 "language.yy"
    {(yyval.str) = strdup("->");}
    break;

  case 296:

/* Line 1806 of yacc.c  */
#line 3129 "language.yy"
    {(yyval.str) = strdup("<-");}
    break;

  case 297:

/* Line 1806 of yacc.c  */
#line 3130 "language.yy"
    {(yyval.str) = strdup("<->");}
    break;

  case 298:

/* Line 1806 of yacc.c  */
#line 3131 "language.yy"
    {(yyval.str) = strdup("=>");}
    break;

  case 299:

/* Line 1806 of yacc.c  */
#line 3132 "language.yy"
    {(yyval.str) = strdup("<=");}
    break;

  case 300:

/* Line 1806 of yacc.c  */
#line 3133 "language.yy"
    {(yyval.str) = strdup("<=>");}
    break;

  case 301:

/* Line 1806 of yacc.c  */
#line 3134 "language.yy"
    {(yyval.str) = strdup(">>");}
    break;

  case 302:

/* Line 1806 of yacc.c  */
#line 3135 "language.yy"
    {(yyval.str) = strdup("<<");}
    break;

  case 303:

/* Line 1806 of yacc.c  */
#line 3136 "language.yy"
    {(yyval.str) = strdup("<<>>");}
    break;

  case 304:

/* Line 1806 of yacc.c  */
#line 3137 "language.yy"
    {(yyval.str) = strdup(">");}
    break;

  case 305:

/* Line 1806 of yacc.c  */
#line 3138 "language.yy"
    {(yyval.str) = strdup("<");}
    break;

  case 306:

/* Line 1806 of yacc.c  */
#line 3139 "language.yy"
    {(yyval.str) = strdup("<>");}
    break;

  case 307:

/* Line 1806 of yacc.c  */
#line 3141 "language.yy"
    {
    switch ((yyvsp[(1) - (1)].arctype)) {
    case MSC_ARC_DIVIDER:  (yyval.str) = strdup("---"); break;
    case MSC_ARC_DISCO:    (yyval.str) = strdup("..."); break;
    default: _ASSERT(0);
    }
}
    break;

  case 308:

/* Line 1806 of yacc.c  */
#line 3149 "language.yy"
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

  case 313:

/* Line 1806 of yacc.c  */
#line 3162 "language.yy"
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

  case 315:

/* Line 1806 of yacc.c  */
#line 3178 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    (yyval.arcbase) = NULL;
    csh.PopContext();
  #else
    (yyval.arcbase) = msc.PopContext();
  #endif
}
    break;



/* Line 1806 of yacc.c  */
#line 6932 "language.cc"
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
#line 3188 "language.yy"



/* END OF FILE */

