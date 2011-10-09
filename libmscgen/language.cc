
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



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 7 "language.yy"

/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011 Zoltan Turanyi
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



/* Line 189 of yacc.c  */
#line 185 "language.cc"

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
     TOK__NEVER__HAPPENS = 310
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
#define TOK__NEVER__HAPPENS 310




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
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



/* Line 214 of yacc.c  */
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


/* Line 264 of yacc.c  */
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
#define YYFINAL  158
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1066

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  56
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  66
/* YYNRULES -- Number of rules.  */
#define YYNRULES  291
/* YYNRULES -- Number of states.  */
#define YYNSTATES  333

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   310

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
      55
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
     170,   173,   177,   179,   182,   184,   188,   191,   193,   196,
     198,   200,   202,   205,   208,   211,   213,   217,   220,   223,
     225,   229,   231,   233,   235,   237,   239,   243,   246,   250,
     254,   258,   262,   266,   269,   273,   276,   278,   282,   285,
     289,   292,   294,   298,   301,   304,   306,   310,   313,   315,
     319,   322,   324,   328,   330,   334,   338,   344,   351,   353,
     355,   359,   362,   365,   367,   369,   373,   376,   380,   384,
     388,   392,   394,   397,   399,   402,   406,   410,   413,   418,
     422,   424,   427,   430,   434,   437,   440,   442,   446,   448,
     451,   455,   457,   460,   464,   467,   470,   472,   475,   479,
     483,   487,   492,   494,   496,   498,   500,   504,   507,   510,
     512,   516,   519,   522,   524,   526,   530,   535,   539,   544,
     547,   549,   551,   553,   555,   557,   561,   564,   567,   571,
     574,   578,   581,   584,   588,   591,   595,   598,   601,   605,
     608,   610,   612,   614,   616,   618,   620,   622,   624,   626,
     628,   630,   632,   634,   636,   638,   640,   642,   644,   647,
     652,   658,   662,   667,   669,   672,   674,   676,   678,   681,
     685,   688,   690,   693,   697,   702,   706,   709,   713,   715,
     718,   721,   723,   727,   730,   734,   738,   742,   746,   749,
     753,   755,   757,   759,   761,   763,   765,   767,   769,   771,
     773,   775,   777,   779,   781,   783,   785,   787,   789,   791,
     793,   795,   797,   799,   801,   803,   805,   807,   809,   811,
     813,   815,   817,   819,   821,   823,   825,   827,   829,   831,
     833,   836
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      57,     0,    -1,    58,    -1,    58,    51,    -1,    -1,    15,
      61,    -1,    60,    61,    -1,    15,    -1,    15,     1,    -1,
      59,    -1,    59,     1,    -1,    62,    -1,    62,    12,    -1,
      62,    12,    59,    -1,    15,     7,    -1,    15,     7,   119,
      -1,   120,    62,   121,    -1,   120,   121,    -1,   120,    62,
       1,   121,    -1,   120,    62,     1,    -1,   120,    62,    -1,
      63,    -1,    63,    65,    -1,    65,    -1,    64,    -1,    63,
      64,    -1,    65,     9,    -1,    65,     9,    55,    -1,    65,
       1,     9,    55,    -1,     9,    -1,    65,     1,     9,    -1,
      66,    -1,    43,    66,    -1,    98,    -1,    98,   112,    -1,
      40,    -1,    40,    98,    -1,    40,    98,   112,    -1,    44,
      -1,    44,    97,    -1,    44,    97,   112,    -1,   112,    -1,
      75,    -1,    70,    -1,    70,    75,    -1,    75,     8,    -1,
      75,     8,    73,    -1,    70,    75,     8,    -1,    70,    75,
       8,    73,    -1,    71,    -1,    88,    -1,    92,    -1,    87,
      -1,    37,    79,    -1,    38,    76,    -1,    39,    81,    -1,
      34,    -1,    34,   113,    -1,    35,    -1,    35,   113,    -1,
      42,   119,    -1,    42,   119,   113,    -1,    36,    -1,    36,
     113,    -1,   110,    -1,    53,    68,    67,    -1,    53,    68,
      -1,    53,    -1,    52,    67,    -1,    52,    -1,   112,    -1,
       5,    -1,     5,   112,    -1,   116,     6,    -1,     6,   116,
      -1,     6,    -1,   116,     6,   116,    -1,   116,     6,    -1,
       6,   116,    -1,     6,    -1,   116,     6,   116,    -1,    48,
      -1,    47,    -1,    49,    -1,    50,    -1,    72,    -1,    71,
       8,    72,    -1,    71,     8,    -1,    71,     8,    55,    -1,
      71,     8,     1,    -1,   116,     7,    19,    -1,   116,     7,
       5,    -1,   116,     7,   119,    -1,   116,     7,    -1,    15,
       7,   119,    -1,    15,     7,    -1,    74,    -1,    73,     8,
      74,    -1,    73,     8,    -1,    73,     8,    55,    -1,   116,
     112,    -1,   116,    -1,   116,   112,    61,    -1,   116,    61,
      -1,   116,   112,    -1,   116,    -1,   116,   112,    61,    -1,
     116,    61,    -1,    77,    -1,    76,     8,    77,    -1,    78,
     113,    -1,   119,    -1,    78,     8,   119,    -1,    80,    -1,
      79,     8,    80,    -1,     3,     7,   119,    -1,     3,    82,
      83,     9,    12,    -1,     3,    82,    83,     9,     1,    12,
      -1,    11,    -1,    84,    -1,    83,     9,    84,    -1,    37,
      79,    -1,    38,    76,    -1,    85,    -1,    86,    -1,    85,
       8,    86,    -1,    85,     1,    -1,   116,     7,    19,    -1,
     116,     7,     5,    -1,   116,     7,   119,    -1,    15,     7,
     119,    -1,    61,    -1,    87,    61,    -1,    89,    -1,    88,
      93,    -1,    88,    93,   112,    -1,    88,    93,    61,    -1,
      88,    61,    -1,    88,    93,   112,    61,    -1,    88,   112,
      61,    -1,    93,    -1,    93,   112,    -1,    93,    61,    -1,
      93,   112,    61,    -1,    41,    93,    -1,    41,     1,    -1,
      41,    -1,    41,    93,   112,    -1,    90,    -1,    91,    93,
      -1,    91,    93,   112,    -1,    91,    -1,    91,    61,    -1,
     116,    33,   116,    -1,    33,   116,    -1,   116,    33,    -1,
      33,    -1,    45,   116,    -1,    45,   116,     6,    -1,    45,
     116,    10,    -1,    45,   116,    33,    -1,    45,   116,     6,
     116,    -1,    45,    -1,    33,    -1,   102,    -1,   104,    -1,
     116,    95,   116,    -1,    95,   116,    -1,   116,    95,    -1,
      95,    -1,   116,   103,   116,    -1,   103,   116,    -1,   116,
     103,    -1,   103,    -1,   116,    -1,    95,   116,    55,    -1,
     116,    95,   116,    55,    -1,   103,   116,    55,    -1,   116,
     103,   116,    55,    -1,    96,    94,    -1,    96,    -1,    32,
      -1,    99,    -1,   100,    -1,   101,    -1,   116,   102,   116,
      -1,   102,   116,    -1,   116,   102,    -1,    99,   105,   116,
      -1,    99,   105,    -1,   116,   103,   116,    -1,   103,   116,
      -1,   116,   103,    -1,   100,   106,   116,    -1,   100,   106,
      -1,   116,   104,   116,    -1,   104,   116,    -1,   116,   104,
      -1,   101,   107,   116,    -1,   101,   107,    -1,    20,    -1,
      23,    -1,    26,    -1,    29,    -1,    21,    -1,    24,    -1,
      27,    -1,    30,    -1,    22,    -1,    25,    -1,    28,    -1,
      31,    -1,   102,    -1,     6,    -1,   103,    -1,     6,    -1,
     104,    -1,     6,    -1,    46,    94,    -1,    54,   119,    69,
     108,    -1,    54,   119,    69,   108,   108,    -1,    54,   119,
     108,    -1,    54,   119,   108,   108,    -1,   109,    -1,   109,
     112,    -1,    17,    -1,    16,    -1,   111,    -1,   111,   113,
      -1,   113,   111,   113,    -1,   113,   111,    -1,   113,    -1,
      13,    14,    -1,    13,   114,    14,    -1,    13,   114,     1,
      14,    -1,    13,     1,    14,    -1,    13,   114,    -1,    13,
     114,     1,    -1,    13,    -1,    13,    55,    -1,    13,     1,
      -1,   115,    -1,   114,     8,   115,    -1,   114,     8,    -1,
     114,     8,    55,    -1,   119,     7,   119,    -1,   119,     7,
       5,    -1,   119,     7,    19,    -1,   119,     7,    -1,   119,
       7,    55,    -1,   119,    -1,     4,    -1,     3,    -1,    15,
      -1,    37,    -1,    38,    -1,    39,    -1,    36,    -1,    40,
      -1,    41,    -1,    44,    -1,    43,    -1,    34,    -1,    35,
      -1,    42,    -1,    45,    -1,    47,    -1,    48,    -1,    49,
      -1,    50,    -1,    51,    -1,    20,    -1,    21,    -1,    22,
      -1,    23,    -1,    24,    -1,    25,    -1,    26,    -1,    27,
      -1,    28,    -1,    29,    -1,    30,    -1,    31,    -1,    32,
      -1,    33,    -1,   116,    -1,   117,    -1,   118,    -1,    18,
      -1,    11,    -1,    11,    55,    -1,    12,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   218,   218,   219,   230,   240,   251,   261,   271,   281,
     288,   301,   302,   311,   325,   337,   352,   362,   375,   388,
     400,   413,   414,   425,   437,   446,   455,   469,   470,   471,
     483,   498,   506,   529,   536,   547,   559,   574,   592,   604,
     613,   626,   637,   644,   656,   668,   680,   691,   707,   722,
     734,   741,   748,   755,   764,   773,   782,   791,   804,   813,
     826,   838,   854,   863,   876,   877,   890,   905,   916,   929,
     945,   946,   957,   968,   979,   990,  1000,  1013,  1024,  1035,
    1045,  1059,  1059,  1059,  1059,  1061,  1070,  1088,  1101,  1102,
    1113,  1133,  1153,  1180,  1200,  1219,  1240,  1246,  1256,  1268,
    1271,  1288,  1301,  1317,  1330,  1348,  1364,  1382,  1398,  1399,
    1406,  1432,  1448,  1466,  1467,  1474,  1490,  1510,  1533,  1544,
    1545,  1552,  1559,  1566,  1568,  1569,  1575,  1582,  1601,  1620,
    1639,  1660,  1669,  1681,  1688,  1696,  1710,  1718,  1727,  1740,
    1756,  1764,  1777,  1785,  1800,  1814,  1830,  1841,  1859,  1866,
    1874,  1888,  1889,  1896,  1910,  1921,  1933,  1943,  1955,  1969,
    1982,  2016,  2032,  2046,  2046,  2046,  2048,  2062,  2073,  2085,
    2094,  2108,  2119,  2131,  2140,  2153,  2154,  2155,  2156,  2159,
    2170,  2181,  2189,  2193,  2197,  2202,  2216,  2227,  2239,  2250,
    2261,  2275,  2286,  2298,  2309,  2319,  2333,  2344,  2356,  2367,
    2377,  2377,  2377,  2377,  2378,  2378,  2378,  2378,  2379,  2379,
    2379,  2379,  2381,  2381,  2382,  2382,  2383,  2383,  2385,  2395,
    2407,  2419,  2431,  2444,  2445,  2457,  2464,  2472,  2480,  2488,
    2500,  2508,  2511,  2521,  2531,  2542,  2553,  2564,  2575,  2588,
    2592,  2604,  2611,  2620,  2630,  2632,  2646,  2660,  2674,  2688,
    2693,  2705,  2705,  2707,  2707,  2708,  2708,  2709,  2709,  2709,
    2710,  2710,  2711,  2711,  2712,  2712,  2712,  2712,  2713,  2713,
    2713,  2715,  2716,  2717,  2718,  2719,  2720,  2721,  2722,  2723,
    2724,  2725,  2726,  2727,  2735,  2746,  2746,  2746,  2746,  2748,
    2762,  2764
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
  "TOK_COMMAND_HSPACE", "TOK_COMMAND_SYMBOL", "TOK__NEVER__HAPPENS",
  "$accept", "msc_with_bye", "msc", "top_level_arclist", "msckey",
  "braced_arclist", "arclist_maybe_no_semicolon", "arclist",
  "arc_with_parallel_semicolon", "arc_with_parallel", "arc",
  "full_arcattrlist_with_label_or_number", "entityrel", "markerrel",
  "entity_command_prefixes", "optlist", "opt", "entitylist", "entity",
  "first_entity", "styledeflist", "styledef", "tok_stringlist",
  "colordeflist", "colordef", "designdef", "scope_open_empty",
  "designelementlist", "designelement", "designoptlist", "designopt",
  "parallel", "box_list", "first_box", "first_pipe",
  "pipe_list_no_content", "pipe_list", "boxrel", "vertxpos",
  "empharcrel_straight", "vertrel_no_xpos", "vertrel", "arcrel",
  "arcrel_to", "arcrel_from", "arcrel_bidir", "relation_to",
  "relation_from", "relation_bidir", "relation_to_cont",
  "relation_from_cont", "relation_bidir_cont", "extvertxpos",
  "symbol_command_no_attr", "symbol_command", "colon_string",
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
     305,   306,   307,   308,   309,   310
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    56,    57,    57,    58,    58,    58,    58,    58,    58,
      58,    59,    59,    59,    60,    60,    61,    61,    61,    61,
      61,    62,    62,    62,    63,    63,    64,    64,    64,    64,
      64,    65,    65,    66,    66,    66,    66,    66,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      67,    67,    67,    68,    68,    68,    68,    69,    69,    69,
      69,    70,    70,    70,    70,    71,    71,    71,    71,    71,
      72,    72,    72,    72,    72,    72,    73,    73,    73,    73,
      74,    74,    74,    74,    75,    75,    75,    75,    76,    76,
      77,    78,    78,    79,    79,    80,    81,    81,    82,    83,
      83,    84,    84,    84,    85,    85,    85,    86,    86,    86,
      86,    87,    87,    88,    88,    88,    88,    88,    88,    88,
      89,    89,    89,    89,    90,    90,    90,    90,    91,    91,
      91,    92,    92,    93,    93,    93,    93,    94,    94,    94,
      94,    94,    94,    95,    95,    95,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    97,
      97,    98,    98,    98,    98,    99,    99,    99,    99,    99,
     100,   100,   100,   100,   100,   101,   101,   101,   101,   101,
     102,   102,   102,   102,   103,   103,   103,   103,   104,   104,
     104,   104,   105,   105,   106,   106,   107,   107,   108,   109,
     109,   109,   109,   110,   110,   111,   111,   112,   112,   112,
     112,   112,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   114,   114,   114,   114,   115,   115,   115,   115,   115,
     115,   116,   116,   117,   117,   117,   117,   117,   117,   117,
     117,   117,   117,   117,   117,   117,   117,   117,   117,   117,
     117,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   119,   119,   119,   119,   120,
     120,   121
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
       2,     3,     1,     2,     1,     3,     2,     1,     2,     1,
       1,     1,     2,     2,     2,     1,     3,     2,     2,     1,
       3,     1,     1,     1,     1,     1,     3,     2,     3,     3,
       3,     3,     3,     2,     3,     2,     1,     3,     2,     3,
       2,     1,     3,     2,     2,     1,     3,     2,     1,     3,
       2,     1,     3,     1,     3,     3,     5,     6,     1,     1,
       3,     2,     2,     1,     1,     3,     2,     3,     3,     3,
       3,     1,     2,     1,     2,     3,     3,     2,     4,     3,
       1,     2,     2,     3,     2,     2,     1,     3,     1,     2,
       3,     1,     2,     3,     2,     2,     1,     2,     3,     3,
       3,     4,     1,     1,     1,     1,     3,     2,     2,     1,
       3,     2,     2,     1,     1,     3,     4,     3,     4,     2,
       1,     1,     1,     1,     1,     3,     2,     2,     3,     2,
       3,     2,     2,     3,     2,     3,     2,     2,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     4,
       5,     3,     4,     1,     2,     1,     1,     1,     2,     3,
       2,     1,     2,     3,     4,     3,     2,     3,     1,     2,
       2,     1,     3,     2,     3,     3,     3,     3,     2,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       4,   252,   251,    29,   289,     0,     0,   226,   225,   200,
     204,   208,   201,   205,   209,   202,   206,   210,   203,   207,
     211,   181,   156,    56,    58,    62,     0,     0,     0,    35,
       0,     0,     0,    38,    82,    81,    83,    84,    69,    67,
       0,     0,     2,     0,     0,   131,    11,    21,    24,     0,
      31,    43,    49,    85,    42,    52,    50,   133,   148,   151,
      51,   140,    33,   182,   183,   184,     0,     0,     0,   223,
      64,   227,    41,   231,   105,     0,   290,   240,   232,   253,
     288,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   282,   283,   284,   262,   263,   257,   254,   255,
     256,   258,   259,   264,   261,   260,   265,   266,   267,   268,
     269,   270,   239,     0,   241,   285,   286,   287,   250,     8,
      95,     5,   154,    57,    59,    63,     0,    53,   113,    54,
     108,     0,   111,     0,    55,    36,     0,   145,   144,     0,
      60,     0,    32,   163,   169,   180,    39,   164,   173,   165,
     174,    71,    68,    70,    75,    66,     0,     0,     1,     3,
      10,     6,    12,    25,     0,     0,    26,    44,   105,     0,
      45,   132,   137,   134,     0,   152,   149,   142,   141,    34,
     213,   212,   189,   215,   214,   194,   217,   216,   199,   186,
     191,   196,   224,   228,   230,    93,   155,   107,   187,   192,
     197,   104,   291,     0,    17,   235,   237,   243,   233,   248,
      94,     0,     0,     0,     0,   110,   118,     0,    37,   147,
      61,    95,   167,   162,   179,    40,   171,   168,   172,    72,
      74,    65,    73,    79,     0,     0,   221,     0,    13,    30,
      27,    47,    89,    88,    86,     0,    46,    96,   101,   136,
     135,   139,   150,   143,   188,   193,   198,   229,    91,    90,
      92,   153,   185,   190,   195,   106,    19,    16,   234,   244,
     242,   246,   247,   249,   245,   115,   114,   109,   112,     0,
       0,     0,     0,   119,     0,   124,     0,    94,   175,   157,
     177,   166,   170,    76,    78,   218,   219,   222,    77,    28,
      48,    98,   103,   100,   138,    18,     0,   121,   122,     0,
     126,     0,     0,   158,   159,   160,   176,   178,   220,    80,
      99,    97,   102,   130,     0,   116,   120,   125,   128,   127,
     129,   161,   117
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,   152,   155,   235,    51,    52,    53,   246,   247,    54,
     129,   130,   131,   127,   128,   134,   217,   282,   283,   284,
     285,    55,    56,    57,    58,    59,    60,    61,   224,   144,
     145,   146,    62,    63,    64,    65,    66,    67,    68,   182,
     185,   188,   236,    69,    70,    71,    72,    73,   113,   114,
     115,   116,   117,   132,    75,   204
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -232
static const yytype_int16 yypact[] =
{
     521,  -232,  -232,  -232,   -41,   322,    10,  -232,  -232,  -232,
    -232,  -232,  -232,  -232,  -232,  -232,  -232,  -232,  -232,  -232,
    -232,  -232,   124,    21,    21,    21,    22,   925,    25,  1007,
     680,   925,   625,   974,  -232,  -232,  -232,  -232,   162,   116,
     925,    31,   -15,    23,    27,  -232,    30,   573,  -232,   699,
    -232,   124,    36,  -232,    42,    27,   976,  -232,  -232,    75,
    -232,    85,    64,   145,   125,   111,   124,   124,   124,    64,
    -232,    21,  -232,   147,  1033,   469,  -232,    50,  -232,  -232,
    -232,  -232,  -232,  -232,  -232,  -232,  -232,  -232,  -232,  -232,
    -232,  -232,  -232,  -232,  -232,  -232,  -232,  -232,  -232,  -232,
    -232,  -232,  -232,  -232,  -232,  -232,  -232,  -232,  -232,  -232,
    -232,  -232,  -232,   257,  -232,  -232,  -232,  -232,    65,  -232,
     876,  -232,  -232,  -232,  -232,  -232,    76,    87,  -232,    89,
    -232,    49,  -232,    82,  -232,    64,   992,  -232,    64,    73,
      21,   104,  -232,  -232,   124,    68,    64,  -232,   124,  -232,
     742,    64,  -232,  -232,   124,   162,   115,    12,  -232,  -232,
    -232,  -232,   573,  -232,   725,   123,    83,   132,    85,   234,
     124,  -232,  -232,    85,    27,  -232,    64,  -232,    27,  -232,
    -232,  -232,   124,  -232,  -232,   124,  -232,  -232,   124,  -232,
    -232,  -232,  -232,  -232,    21,   778,   124,  -232,   124,   124,
     124,    27,  -232,   744,  -232,  -232,   127,   420,  -232,   371,
     134,   925,    22,   925,   925,  -232,  -232,    67,  -232,  -232,
    -232,   925,    88,   124,  -232,  -232,    92,   124,   124,  -232,
    -232,  -232,   124,   124,    68,   107,   107,   144,  -232,   101,
    -232,   124,  -232,  -232,  -232,   151,   153,  -232,    85,  -232,
      27,  -232,  -232,  -232,  -232,  -232,  -232,  -232,  -232,  -232,
    -232,  -232,  -232,  -232,  -232,  -232,   154,  -232,  -232,  -232,
    -232,  -232,  -232,  -232,  -232,  -232,  -232,  -232,  -232,   170,
      22,   925,   171,  -232,    18,  -232,   174,  -232,  -232,    81,
    -232,   128,   131,  -232,  -232,  -232,   107,  -232,   124,  -232,
     153,     5,  -232,    27,  -232,  -232,   925,    87,    89,   122,
    -232,    37,   827,   124,  -232,  -232,  -232,  -232,  -232,  -232,
    -232,  -232,  -232,  -232,   175,  -232,  -232,  -232,  -232,  -232,
    -232,  -232,  -232
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -232,  -232,  -232,    28,  -232,    29,   117,  -232,   148,   157,
     159,    39,  -232,  -232,  -232,  -232,    32,   -36,   -93,   158,
     -69,     2,  -232,   -67,     4,  -232,  -232,  -232,   -91,  -232,
     -92,  -232,  -232,  -232,  -232,  -232,  -232,    33,   -14,    71,
    -232,  -232,   196,  -232,  -232,  -232,   -26,   -21,   -20,  -232,
    -232,  -232,  -231,  -232,  -232,   166,    38,   -22,  -232,    24,
       0,  -232,  -232,    15,  -232,  -197
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -239
static const yytype_int16 yytable[] =
{
      74,   123,   124,   125,   296,   297,   267,   147,     1,     2,
      -7,   119,   148,   149,    76,     1,     2,   120,   233,   310,
     118,     4,   122,    -9,   160,   126,   311,  -123,   133,   136,
     139,   158,    74,   150,     5,   121,   159,   181,     4,   156,
       1,     2,   162,   184,   169,   187,   140,    74,   198,   193,
     170,   168,   279,   199,   200,   157,   139,   214,   234,   139,
     320,    -7,     5,   138,   205,   318,   189,   190,   191,   305,
       1,     2,   209,   161,    -9,    74,   153,     5,     1,     2,
       7,     8,   279,   211,   171,   172,     4,   313,   175,   173,
     177,   314,   176,   216,   174,   212,     4,   213,     5,   178,
     179,     7,     8,   197,   280,   281,   196,   192,    22,   215,
     198,   221,   201,   223,   315,   199,   200,   186,   220,     1,
       2,   232,   154,   324,   147,     1,     2,     1,     2,   228,
     149,   183,   239,    11,   325,   210,    14,   279,   240,    17,
     241,   268,    20,   288,   222,   -15,    10,   290,   226,    13,
     298,   180,    16,   234,   230,    19,   299,   237,   195,   280,
     281,   301,    74,     7,     8,     9,   202,   151,    12,   245,
     248,    15,   257,   218,    18,     5,   219,   306,     7,     8,
     309,   312,   254,   316,   225,   255,   317,   332,   256,   229,
     238,   142,   203,   153,   231,   163,   261,   197,   262,   263,
     264,   244,   249,   251,   164,   300,   201,   253,   321,   167,
     260,   250,   308,   307,   252,   277,   276,   286,   326,   327,
     295,   227,   118,   289,   274,   135,   275,   291,   292,   278,
     265,   270,   293,   294,   -87,   242,   287,     1,     2,   194,
       0,   248,   -87,   -87,     0,   -87,   -87,   -87,     0,   141,
     -87,   -87,     0,     0,     0,     0,     0,  -236,   206,     0,
    -236,  -236,     0,     0,     0,   207,  -236,   -87,  -236,  -236,
    -236,   208,     0,  -236,  -236,     0,     0,   302,     0,   304,
       0,     0,     0,     0,     0,   -87,   303,     0,     0,   243,
    -236,     0,     0,     0,     0,     0,     0,     0,   319,     0,
       0,   248,     0,     0,     0,     0,     0,     0,  -236,   286,
       0,   286,     0,   331,     0,     0,     0,     0,     0,     0,
       0,   323,  -238,    77,     0,     1,     2,   330,     0,     0,
    -238,  -238,   322,  -238,  -238,  -238,    78,    79,  -238,  -238,
      80,     0,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,     0,   107,
     108,   109,   110,   111,     1,     2,   271,   112,     0,     0,
       0,     0,     0,     0,     0,     0,    79,     0,     0,    80,
     272,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,     0,   107,   108,
     109,   110,   111,     1,     2,     0,   273,     0,     0,     0,
       0,     0,     0,     0,     0,    79,     0,     0,    80,     0,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,     0,   107,   108,   109,
     110,   111,     1,     2,     0,   269,     0,     0,     3,     0,
       4,   202,     5,     0,   141,     7,     8,     0,     0,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,     0,     0,    34,    35,    36,    37,
       0,    38,    39,    40,     1,     2,     0,     0,     0,     0,
       3,     0,     4,     0,     5,     0,     6,     7,     8,     0,
       0,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,     0,     0,    34,    35,
      36,    37,     0,    38,    39,    40,     1,     2,     0,     0,
       0,     0,     3,     0,     4,     0,     5,     0,   141,     7,
       8,     0,     0,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,     0,     0,
      34,    35,    36,    37,     0,    38,    39,    40,     1,     2,
       0,     0,     0,     0,     0,     0,     4,     0,     5,     0,
     141,     7,     8,     0,     0,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,     0,    33,
       0,     0,    34,    35,    36,    37,     0,    38,    39,    40,
    -146,   137,     0,     1,     2,     0,     0,     0,  -146,  -146,
       0,  -146,  -146,  -146,     0,     0,  -146,  -146,     0,   -23,
     165,     0,   -23,   -23,     0,     0,     0,   -23,   166,     0,
     -23,   -23,   -23,    22,     0,   -23,   -23,     0,     0,     0,
       0,     0,     0,     0,     0,   -22,   165,     0,   -22,   -22,
       0,  -146,   -23,   -22,   166,     0,   -22,   -22,   -22,     0,
       0,   -22,   -22,     0,   -20,   266,     0,   -20,   -20,     0,
     -23,     0,   -20,   -20,     0,   -20,   202,   -20,   -22,     0,
     -20,   -20,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,     0,   143,   -22,   -20,     0,     0,
       0,     1,     2,   258,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    79,     0,   -20,    80,   259,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,     0,   107,   108,   109,   110,   111,
       1,     2,   328,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    79,     0,     0,    80,   329,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,     0,   107,   108,   109,   110,   111,     1,
       2,     0,     0,     0,     0,     0,     0,   -14,     0,     0,
       0,    79,     0,     0,    80,     0,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,     0,   107,   108,   109,   110,   111,     1,     2,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      79,     0,     0,    80,     0,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,     0,   107,   108,   109,   110,   111,     1,     2,     1,
       2,     0,     0,     0,     0,     0,     0,     4,     0,     5,
       0,     0,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,     0,   143,     0,    22,
       1,     2,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,     0,     0,     0,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
     195,     0,     0,     0,     4,     0,     5,     0,     0,     7,
       8,     0,     0,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,     0,   196
};

static const yytype_int16 yycheck[] =
{
       0,    23,    24,    25,   235,   236,   203,    33,     3,     4,
       0,     1,    33,    33,    55,     3,     4,     7,     6,     1,
       5,    11,    22,     0,     1,     3,     8,     9,     3,    29,
      30,     0,    32,    33,    13,     6,    51,    63,    11,    39,
       3,     4,    12,    64,     8,    65,    31,    47,    74,    71,
       8,    51,    15,    74,    74,    40,    56,     8,    46,    59,
      55,    51,    13,    30,    14,   296,    66,    67,    68,   266,
       3,     4,     7,    44,    51,    75,    38,    13,     3,     4,
      16,    17,    15,     7,    55,    56,    11,     6,    59,    56,
      61,    10,    59,    11,    56,     8,    11,     8,    13,    61,
      62,    16,    17,    74,    37,    38,    33,    69,    33,   131,
     136,     7,    74,    45,    33,   136,   136,     6,   140,     3,
       4,     6,     6,     1,   150,     3,     4,     3,     4,   150,
     150,     6,     9,    22,    12,   120,    25,    15,    55,    28,
       8,    14,    31,    55,   144,    11,    21,    55,   148,    24,
       6,     6,    27,    46,   154,    30,    55,   157,     7,    37,
      38,     8,   162,    16,    17,    20,    12,     5,    23,   169,
     170,    26,   194,   135,    29,    13,   138,     7,    16,    17,
       9,     7,   182,    55,   146,   185,    55,    12,   188,   151,
     162,    32,    75,   155,   155,    47,   196,   168,   198,   199,
     200,   169,   173,   174,    47,   241,   168,   178,   301,    51,
     195,   173,   281,   280,   176,   213,   212,   217,   309,   311,
     234,   150,   207,   223,   209,    29,   211,   227,   228,   214,
     201,   207,   232,   233,     0,     1,   221,     3,     4,    73,
      -1,   241,     8,     9,    -1,    11,    12,    13,    -1,    15,
      16,    17,    -1,    -1,    -1,    -1,    -1,     0,     1,    -1,
       3,     4,    -1,    -1,    -1,     8,     9,    33,    11,    12,
      13,    14,    -1,    16,    17,    -1,    -1,   248,    -1,   250,
      -1,    -1,    -1,    -1,    -1,    51,   248,    -1,    -1,    55,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   298,    -1,
      -1,   301,    -1,    -1,    -1,    -1,    -1,    -1,    51,   309,
      -1,   311,    -1,   313,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   306,     0,     1,    -1,     3,     4,   312,    -1,    -1,
       8,     9,   303,    11,    12,    13,    14,    15,    16,    17,
      18,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    -1,    47,
      48,    49,    50,    51,     3,     4,     5,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    -1,    47,    48,
      49,    50,    51,     3,     4,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,    18,    -1,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    -1,    47,    48,    49,
      50,    51,     3,     4,    -1,    55,    -1,    -1,     9,    -1,
      11,    12,    13,    -1,    15,    16,    17,    -1,    -1,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    -1,    -1,    47,    48,    49,    50,
      -1,    52,    53,    54,     3,     4,    -1,    -1,    -1,    -1,
       9,    -1,    11,    -1,    13,    -1,    15,    16,    17,    -1,
      -1,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    -1,    -1,    47,    48,
      49,    50,    -1,    52,    53,    54,     3,     4,    -1,    -1,
      -1,    -1,     9,    -1,    11,    -1,    13,    -1,    15,    16,
      17,    -1,    -1,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    -1,    -1,
      47,    48,    49,    50,    -1,    52,    53,    54,     3,     4,
      -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,    13,    -1,
      15,    16,    17,    -1,    -1,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    -1,    44,
      -1,    -1,    47,    48,    49,    50,    -1,    52,    53,    54,
       0,     1,    -1,     3,     4,    -1,    -1,    -1,     8,     9,
      -1,    11,    12,    13,    -1,    -1,    16,    17,    -1,     0,
       1,    -1,     3,     4,    -1,    -1,    -1,     8,     9,    -1,
      11,    12,    13,    33,    -1,    16,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     0,     1,    -1,     3,     4,
      -1,    51,    33,     8,     9,    -1,    11,    12,    13,    -1,
      -1,    16,    17,    -1,     0,     1,    -1,     3,     4,    -1,
      51,    -1,     8,     9,    -1,    11,    12,    13,    33,    -1,
      16,    17,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    51,    33,    -1,    -1,
      -1,     3,     4,     5,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    15,    -1,    51,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    -1,    47,    48,    49,    50,    51,
       3,     4,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    -1,    47,    48,    49,    50,    51,     3,
       4,    -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,
      -1,    15,    -1,    -1,    18,    -1,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    -1,    47,    48,    49,    50,    51,     3,     4,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    18,    -1,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    -1,    47,    48,    49,    50,    51,     3,     4,     3,
       4,    -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,    13,
      -1,    -1,    16,    17,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    -1,    33,
       3,     4,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    -1,    -1,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
       7,    -1,    -1,    -1,    11,    -1,    13,    -1,    -1,    16,
      17,    -1,    -1,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     9,    11,    13,    15,    16,    17,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    47,    48,    49,    50,    52,    53,
      54,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    70,    71,    72,    75,    87,    88,    89,    90,    91,
      92,    93,    98,    99,   100,   101,   102,   103,   104,   109,
     110,   111,   112,   113,   116,   120,    55,     1,    14,    15,
      18,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    47,    48,    49,
      50,    51,    55,   114,   115,   116,   117,   118,   119,     1,
       7,    61,   116,   113,   113,   113,     3,    79,    80,    76,
      77,    78,   119,     3,    81,    98,   116,     1,    93,   116,
     119,    15,    66,    33,    95,    96,    97,   102,   103,   104,
     116,     5,    67,   112,     6,    68,   116,   119,     0,    51,
       1,    61,    12,    64,    65,     1,     9,    75,   116,     8,
       8,    61,    61,    93,   112,    61,    93,    61,   112,   112,
       6,   102,   105,     6,   103,   106,     6,   104,   107,   116,
     116,   116,   112,   113,   111,     7,    33,    61,   102,   103,
     104,   112,    12,    62,   121,    14,     1,     8,    14,     7,
     119,     7,     8,     8,     8,   113,    11,    82,   112,   112,
     113,     7,   116,    45,    94,   112,   116,    95,   103,   112,
     116,    67,     6,     6,    46,    69,   108,   116,    59,     9,
      55,     8,     1,    55,    72,   116,    73,    74,   116,    61,
     112,    61,   112,    61,   116,   116,   116,   113,     5,    19,
     119,   116,   116,   116,   116,    61,     1,   121,    14,    55,
     115,     5,    19,    55,   119,   119,    80,    77,   119,    15,
      37,    38,    83,    84,    85,    86,   116,   119,    55,   116,
      55,   116,   116,   116,   116,    94,   108,   108,     6,    55,
      73,     8,    61,   112,    61,   121,     7,    79,    76,     9,
       1,     8,     7,     6,    10,    33,    55,    55,   108,   116,
      55,    74,    61,   119,     1,    12,    84,    86,     5,    19,
     119,   116,    12
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
#line 208 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1849 "language.cc"
	break;
      case 4: /* "TOK_QSTRING" */

/* Line 1000 of yacc.c  */
#line 208 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1858 "language.cc"
	break;
      case 5: /* "TOK_NUMBER" */

/* Line 1000 of yacc.c  */
#line 213 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1867 "language.cc"
	break;
      case 15: /* "TOK_MSC" */

/* Line 1000 of yacc.c  */
#line 209 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1876 "language.cc"
	break;
      case 16: /* "TOK_COLON_STRING" */

/* Line 1000 of yacc.c  */
#line 208 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1885 "language.cc"
	break;
      case 17: /* "TOK_COLON_QUOTED_STRING" */

/* Line 1000 of yacc.c  */
#line 208 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1894 "language.cc"
	break;
      case 18: /* "TOK_STYLE_NAME" */

/* Line 1000 of yacc.c  */
#line 208 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1903 "language.cc"
	break;
      case 19: /* "TOK_BOOLEAN" */

/* Line 1000 of yacc.c  */
#line 213 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1912 "language.cc"
	break;
      case 34: /* "TOK_COMMAND_HEADING" */

/* Line 1000 of yacc.c  */
#line 211 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1921 "language.cc"
	break;
      case 35: /* "TOK_COMMAND_NUDGE" */

/* Line 1000 of yacc.c  */
#line 211 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1930 "language.cc"
	break;
      case 36: /* "TOK_COMMAND_NEWPAGE" */

/* Line 1000 of yacc.c  */
#line 211 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1939 "language.cc"
	break;
      case 37: /* "TOK_COMMAND_DEFCOLOR" */

/* Line 1000 of yacc.c  */
#line 210 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1948 "language.cc"
	break;
      case 38: /* "TOK_COMMAND_DEFSTYLE" */

/* Line 1000 of yacc.c  */
#line 210 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1957 "language.cc"
	break;
      case 39: /* "TOK_COMMAND_DEFDESIGN" */

/* Line 1000 of yacc.c  */
#line 210 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1966 "language.cc"
	break;
      case 40: /* "TOK_COMMAND_BIG" */

/* Line 1000 of yacc.c  */
#line 209 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1975 "language.cc"
	break;
      case 41: /* "TOK_COMMAND_PIPE" */

/* Line 1000 of yacc.c  */
#line 209 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1984 "language.cc"
	break;
      case 42: /* "TOK_COMMAND_MARK" */

/* Line 1000 of yacc.c  */
#line 212 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1993 "language.cc"
	break;
      case 43: /* "TOK_COMMAND_PARALLEL" */

/* Line 1000 of yacc.c  */
#line 212 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 2002 "language.cc"
	break;
      case 44: /* "TOK_VERTICAL" */

/* Line 1000 of yacc.c  */
#line 213 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 2011 "language.cc"
	break;
      case 45: /* "TOK_AT" */

/* Line 1000 of yacc.c  */
#line 213 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 2020 "language.cc"
	break;
      case 46: /* "TOK_AT_POS" */

/* Line 1000 of yacc.c  */
#line 213 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 2029 "language.cc"
	break;
      case 47: /* "TOK_SHOW" */

/* Line 1000 of yacc.c  */
#line 213 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 2038 "language.cc"
	break;
      case 48: /* "TOK_HIDE" */

/* Line 1000 of yacc.c  */
#line 213 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 2047 "language.cc"
	break;
      case 49: /* "TOK_ACTIVATE" */

/* Line 1000 of yacc.c  */
#line 213 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 2056 "language.cc"
	break;
      case 50: /* "TOK_DEACTIVATE" */

/* Line 1000 of yacc.c  */
#line 213 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 2065 "language.cc"
	break;
      case 51: /* "TOK_BYE" */

/* Line 1000 of yacc.c  */
#line 212 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 2074 "language.cc"
	break;
      case 52: /* "TOK_COMMAND_VSPACE" */

/* Line 1000 of yacc.c  */
#line 214 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 2083 "language.cc"
	break;
      case 54: /* "TOK_COMMAND_SYMBOL" */

/* Line 1000 of yacc.c  */
#line 214 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 2092 "language.cc"
	break;
      case 59: /* "top_level_arclist" */

/* Line 1000 of yacc.c  */
#line 203 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1000 of yacc.c  */
#line 2101 "language.cc"
	break;
      case 61: /* "braced_arclist" */

/* Line 1000 of yacc.c  */
#line 203 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1000 of yacc.c  */
#line 2110 "language.cc"
	break;
      case 62: /* "arclist_maybe_no_semicolon" */

/* Line 1000 of yacc.c  */
#line 203 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1000 of yacc.c  */
#line 2119 "language.cc"
	break;
      case 63: /* "arclist" */

/* Line 1000 of yacc.c  */
#line 203 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1000 of yacc.c  */
#line 2128 "language.cc"
	break;
      case 64: /* "arc_with_parallel_semicolon" */

/* Line 1000 of yacc.c  */
#line 198 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1000 of yacc.c  */
#line 2137 "language.cc"
	break;
      case 65: /* "arc_with_parallel" */

/* Line 1000 of yacc.c  */
#line 198 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1000 of yacc.c  */
#line 2146 "language.cc"
	break;
      case 66: /* "arc" */

/* Line 1000 of yacc.c  */
#line 198 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1000 of yacc.c  */
#line 2155 "language.cc"
	break;
      case 67: /* "full_arcattrlist_with_label_or_number" */

/* Line 1000 of yacc.c  */
#line 206 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1000 of yacc.c  */
#line 2164 "language.cc"
	break;
      case 71: /* "optlist" */

/* Line 1000 of yacc.c  */
#line 203 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1000 of yacc.c  */
#line 2173 "language.cc"
	break;
      case 72: /* "opt" */

/* Line 1000 of yacc.c  */
#line 198 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1000 of yacc.c  */
#line 2182 "language.cc"
	break;
      case 73: /* "entitylist" */

/* Line 1000 of yacc.c  */
#line 204 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entitylist);};

/* Line 1000 of yacc.c  */
#line 2191 "language.cc"
	break;
      case 74: /* "entity" */

/* Line 1000 of yacc.c  */
#line 204 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entitylist);};

/* Line 1000 of yacc.c  */
#line 2200 "language.cc"
	break;
      case 75: /* "first_entity" */

/* Line 1000 of yacc.c  */
#line 204 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entitylist);};

/* Line 1000 of yacc.c  */
#line 2209 "language.cc"
	break;
      case 78: /* "tok_stringlist" */

/* Line 1000 of yacc.c  */
#line 205 "language.yy"
	{if (!C_S_H) delete (yyvaluep->stringlist);};

/* Line 1000 of yacc.c  */
#line 2218 "language.cc"
	break;
      case 87: /* "parallel" */

/* Line 1000 of yacc.c  */
#line 202 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcparallel);};

/* Line 1000 of yacc.c  */
#line 2227 "language.cc"
	break;
      case 88: /* "box_list" */

/* Line 1000 of yacc.c  */
#line 201 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcboxseries);};

/* Line 1000 of yacc.c  */
#line 2236 "language.cc"
	break;
      case 89: /* "first_box" */

/* Line 1000 of yacc.c  */
#line 201 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbox);};

/* Line 1000 of yacc.c  */
#line 2245 "language.cc"
	break;
      case 90: /* "first_pipe" */

/* Line 1000 of yacc.c  */
#line 201 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcpipe);};

/* Line 1000 of yacc.c  */
#line 2254 "language.cc"
	break;
      case 91: /* "pipe_list_no_content" */

/* Line 1000 of yacc.c  */
#line 201 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcpipeseries);};

/* Line 1000 of yacc.c  */
#line 2263 "language.cc"
	break;
      case 92: /* "pipe_list" */

/* Line 1000 of yacc.c  */
#line 201 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcpipeseries);};

/* Line 1000 of yacc.c  */
#line 2272 "language.cc"
	break;
      case 93: /* "boxrel" */

/* Line 1000 of yacc.c  */
#line 201 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbox);};

/* Line 1000 of yacc.c  */
#line 2281 "language.cc"
	break;
      case 94: /* "vertxpos" */

/* Line 1000 of yacc.c  */
#line 196 "language.yy"
	{if (!C_S_H) delete (yyvaluep->vertxpos);};

/* Line 1000 of yacc.c  */
#line 2290 "language.cc"
	break;
      case 96: /* "vertrel_no_xpos" */

/* Line 1000 of yacc.c  */
#line 197 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcvertarrow);};

/* Line 1000 of yacc.c  */
#line 2299 "language.cc"
	break;
      case 97: /* "vertrel" */

/* Line 1000 of yacc.c  */
#line 198 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1000 of yacc.c  */
#line 2308 "language.cc"
	break;
      case 98: /* "arcrel" */

/* Line 1000 of yacc.c  */
#line 198 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1000 of yacc.c  */
#line 2317 "language.cc"
	break;
      case 99: /* "arcrel_to" */

/* Line 1000 of yacc.c  */
#line 200 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 1000 of yacc.c  */
#line 2326 "language.cc"
	break;
      case 100: /* "arcrel_from" */

/* Line 1000 of yacc.c  */
#line 200 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 1000 of yacc.c  */
#line 2335 "language.cc"
	break;
      case 101: /* "arcrel_bidir" */

/* Line 1000 of yacc.c  */
#line 200 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 1000 of yacc.c  */
#line 2344 "language.cc"
	break;
      case 109: /* "symbol_command_no_attr" */

/* Line 1000 of yacc.c  */
#line 199 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1000 of yacc.c  */
#line 2353 "language.cc"
	break;
      case 110: /* "symbol_command" */

/* Line 1000 of yacc.c  */
#line 199 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1000 of yacc.c  */
#line 2362 "language.cc"
	break;
      case 112: /* "full_arcattrlist_with_label" */

/* Line 1000 of yacc.c  */
#line 205 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1000 of yacc.c  */
#line 2371 "language.cc"
	break;
      case 113: /* "full_arcattrlist" */

/* Line 1000 of yacc.c  */
#line 205 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1000 of yacc.c  */
#line 2380 "language.cc"
	break;
      case 114: /* "arcattrlist" */

/* Line 1000 of yacc.c  */
#line 205 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1000 of yacc.c  */
#line 2389 "language.cc"
	break;
      case 115: /* "arcattr" */

/* Line 1000 of yacc.c  */
#line 205 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attrib);};

/* Line 1000 of yacc.c  */
#line 2398 "language.cc"
	break;
      case 116: /* "entity_string" */

/* Line 1000 of yacc.c  */
#line 207 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 2407 "language.cc"
	break;
      case 117: /* "reserved_word_string" */

/* Line 1000 of yacc.c  */
#line 207 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 2416 "language.cc"
	break;
      case 118: /* "symbol_string" */

/* Line 1000 of yacc.c  */
#line 207 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 2425 "language.cc"
	break;
      case 119: /* "string" */

/* Line 1000 of yacc.c  */
#line 207 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 2434 "language.cc"
	break;
      case 121: /* "scope_close" */

/* Line 1000 of yacc.c  */
#line 198 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1000 of yacc.c  */
#line 2443 "language.cc"
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
        case 3:

/* Line 1455 of yacc.c  */
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

/* Line 1455 of yacc.c  */
#line 230 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.hintStatus = HINT_READY;
    csh.hintType = HINT_LINE_START;
    csh.hintsForcedOnly = true;
    csh.AddLineBeginToHints();
  #else
    //no action for empty file
  #endif
}
    break;

  case 5:

/* Line 1455 of yacc.c  */
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

/* Line 1455 of yacc.c  */
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

/* Line 1455 of yacc.c  */
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

/* Line 1455 of yacc.c  */
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

/* Line 1455 of yacc.c  */
#line 282 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    msc.AddArcs((yyvsp[(1) - (1)].arclist));
  #endif
}
    break;

  case 10:

/* Line 1455 of yacc.c  */
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

/* Line 1455 of yacc.c  */
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

/* Line 1455 of yacc.c  */
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

/* Line 1455 of yacc.c  */
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

/* Line 1455 of yacc.c  */
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

/* Line 1455 of yacc.c  */
#line 353 "language.yy"
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

  case 17:

/* Line 1455 of yacc.c  */
#line 363 "language.yy"
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

/* Line 1455 of yacc.c  */
#line 376 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (4)]), COLOR_BRACE);
    csh.AddCSH_Error((yylsp[(3) - (4)]), "Could not recognize this as a valid line.");
    csh.AddCSH((yylsp[(4) - (4)]), COLOR_BRACE);
  #else
    if ((yyvsp[(4) - (4)].arcbase)) ((yyvsp[(2) - (4)].arclist))->Append((yyvsp[(4) - (4)].arcbase)); //Append any potential CommandNumbering
    (yyval.arclist) = (yyvsp[(2) - (4)].arclist);
    msc.Error.Error(MSC_POS((yylsp[(3) - (4)])).start, "Syntax error.");
  #endif
    yyerrok;
}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 389 "language.yy"
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

/* Line 1455 of yacc.c  */
#line 401 "language.yy"
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

/* Line 1455 of yacc.c  */
#line 415 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ErrorAfter((yylsp[(2) - (2)]), "Missing a semicolon (';').");
  #else
    if ((yyvsp[(2) - (2)].arcbase)) ((yyvsp[(1) - (2)].arclist))->Append((yyvsp[(2) - (2)].arcbase));
    (yyval.arclist) = (yyvsp[(1) - (2)].arclist);
    msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).end.NextChar(), "Missing ';'.");
    msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).start, MSC_POS((yylsp[(2) - (2)])).end.NextChar(), "Here is the beginning of the command as I understood it.");
  #endif
}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 426 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ErrorAfter((yylsp[(1) - (1)]), "Missing a semicolon (';').");
  #else
    (yyval.arclist) = (new ArcList)->Append((yyvsp[(1) - (1)].arcbase)); /* New list */
    msc.Error.Error(MSC_POS((yylsp[(1) - (1)])).end.NextChar(), "Missing ';'.");
    msc.Error.Error(MSC_POS((yylsp[(1) - (1)])).start, MSC_POS((yylsp[(1) - (1)])).end.NextChar(), "Here is the beginning of the command as I understood it.");
  #endif
}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 438 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(1) - (1)].arcbase))
        (yyval.arclist) = (new ArcList)->Append((yyvsp[(1) - (1)].arcbase)); /* New list */
    else
        (yyval.arclist) = new ArcList;
  #endif
}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 447 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(2) - (2)].arcbase))
        ((yyvsp[(1) - (2)].arclist))->Append((yyvsp[(2) - (2)].arcbase));     /* Add to existing list */
    (yyval.arclist) = ((yyvsp[(1) - (2)].arclist));
  #endif
}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 456 "language.yy"
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

/* Line 1455 of yacc.c  */
#line 472 "language.yy"
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

/* Line 1455 of yacc.c  */
#line 484 "language.yy"
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

/* Line 1455 of yacc.c  */
#line 499 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        if ((yyvsp[(1) - (1)].arcbase)) ((yyvsp[(1) - (1)].arcbase))->SetLineEnd(MSC_POS((yyloc)));
        (yyval.arcbase)=(yyvsp[(1) - (1)].arcbase);
  #endif
}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 507 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[(1) - (2)]), HINT_LINE_START)) {
	    csh.AddLineBeginToHints(false);
	    csh.hintStatus = HINT_READY;
	} else if (csh.CheckHintBetween((yylsp[(1) - (2)]), (yylsp[(2) - (2)]), HINT_LINE_START)) {
        csh.hintStatus = HINT_READY;
        csh.hintType = HINT_LINE_START;
        csh.hintsForcedOnly = true;
        csh.AddLineBeginToHints(false);
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

/* Line 1455 of yacc.c  */
#line 530 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.arcbase)=((yyvsp[(1) - (1)].arcbase))->AddAttributeList(NULL);
  #endif
}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 537 "language.yy"
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

/* Line 1455 of yacc.c  */
#line 548 "language.yy"
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

/* Line 1455 of yacc.c  */
#line 560 "language.yy"
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

/* Line 1455 of yacc.c  */
#line 575 "language.yy"
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

/* Line 1455 of yacc.c  */
#line 593 "language.yy"
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

/* Line 1455 of yacc.c  */
#line 605 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
        (yyval.arcbase) = ((yyvsp[(2) - (2)].arcbase))->AddAttributeList(NULL);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 614 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(3) - (3)])))
        ArcVerticalArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(3) - (3)])))
        ArcVerticalArrow::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcbase) = ((yyvsp[(2) - (3)].arcbase))->AddAttributeList((yyvsp[(3) - (3)].attriblist));
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 627 "language.yy"
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

/* Line 1455 of yacc.c  */
#line 638 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (new CommandEntity((yyvsp[(1) - (1)].entitylist), &msc))->AddAttributeList(NULL);
  #endif
}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 645 "language.yy"
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

/* Line 1455 of yacc.c  */
#line 657 "language.yy"
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

/* Line 1455 of yacc.c  */
#line 669 "language.yy"
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

/* Line 1455 of yacc.c  */
#line 681 "language.yy"
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

/* Line 1455 of yacc.c  */
#line 692 "language.yy"
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

/* Line 1455 of yacc.c  */
#line 708 "language.yy"
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

/* Line 1455 of yacc.c  */
#line 723 "language.yy"
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

/* Line 1455 of yacc.c  */
#line 735 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcboxseries); //to remove warning for downcast
  #endif
}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 742 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcpipeseries); //to remove warning for downcast
  #endif
}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 749 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = ((yyvsp[(1) - (1)].arcparallel))->AddAttributeList(NULL);
  #endif
}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 756 "language.yy"
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

/* Line 1455 of yacc.c  */
#line 765 "language.yy"
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

/* Line 1455 of yacc.c  */
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

  case 56:

/* Line 1455 of yacc.c  */
#line 783 "language.yy"
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

/* Line 1455 of yacc.c  */
#line 792 "language.yy"
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

/* Line 1455 of yacc.c  */
#line 805 "language.yy"
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

/* Line 1455 of yacc.c  */
#line 814 "language.yy"
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

/* Line 1455 of yacc.c  */
#line 827 "language.yy"
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

/* Line 1455 of yacc.c  */
#line 839 "language.yy"
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

/* Line 1455 of yacc.c  */
#line 855 "language.yy"
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

/* Line 1455 of yacc.c  */
#line 864 "language.yy"
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

  case 65:

/* Line 1455 of yacc.c  */
#line 878 "language.yy"
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

  case 66:

/* Line 1455 of yacc.c  */
#line 891 "language.yy"
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

  case 67:

/* Line 1455 of yacc.c  */
#line 906 "language.yy"
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

  case 68:

/* Line 1455 of yacc.c  */
#line 917 "language.yy"
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

  case 69:

/* Line 1455 of yacc.c  */
#line 930 "language.yy"
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

  case 71:

/* Line 1455 of yacc.c  */
#line 947 "language.yy"
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

  case 72:

/* Line 1455 of yacc.c  */
#line 958 "language.yy"
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

  case 73:

/* Line 1455 of yacc.c  */
#line 969 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_ENTITYNAME);
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.namerel) = new NamePair((yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), NULL, file_line_range());
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 980 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_ENTITYNAME);
    csh.CheckEntityHintAt((yylsp[(1) - (2)]));
  #else
    (yyval.namerel) = new NamePair(NULL, file_line_range(), (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])));
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 991 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
    csh.CheckEntityHintAt((yylsp[(1) - (1)]));
    csh.CheckEntityHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF);
  #else
    (yyval.namerel) = NULL;
  #endif
}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 1001 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_ENTITYNAME);
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    csh.AddCSH((yylsp[(3) - (3)]), COLOR_ENTITYNAME);
  #else
    (yyval.namerel) = new NamePair((yyvsp[(1) - (3)].str), MSC_POS((yylsp[(1) - (3)])), (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(3) - (3)])));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 1014 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_MARKERNAME);
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    (yyval.namerel) = new NamePair((yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), NULL, file_line_range());
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 1025 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_MARKERNAME);
    csh.CheckHintAt((yylsp[(1) - (2)]), HINT_MARKER);
  #else
    (yyval.namerel) = new NamePair(NULL, file_line_range(), (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])));
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 1036 "language.yy"
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

  case 80:

/* Line 1455 of yacc.c  */
#line 1046 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_MARKERNAME);
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    csh.AddCSH((yylsp[(3) - (3)]), COLOR_MARKERNAME);
  #else
    (yyval.namerel) = new NamePair((yyvsp[(1) - (3)].str), MSC_POS((yylsp[(1) - (3)])), (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(3) - (3)])));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 1062 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(1) - (1)].arcbase))
        (yyval.arclist) = (new ArcList)->Append((yyvsp[(1) - (1)].arcbase)); /* New list */
    else
        (yyval.arclist) = NULL;
  #endif
}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 1071 "language.yy"
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
    }
    (yyval.arclist) = (yyvsp[(1) - (3)].arclist);
  #endif
}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 1089 "language.yy"
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

  case 89:

/* Line 1455 of yacc.c  */
#line 1103 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    csh.AddCSH_Error((yylsp[(3) - (3)]), "An option expected here.");
  #else
    (yyval.arclist) = (yyvsp[(1) - (3)].arclist);
  #endif
}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 1114 "language.yy"
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

  case 91:

/* Line 1455 of yacc.c  */
#line 1134 "language.yy"
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

  case 92:

/* Line 1455 of yacc.c  */
#line 1154 "language.yy"
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

  case 93:

/* Line 1455 of yacc.c  */
#line 1181 "language.yy"
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

  case 94:

/* Line 1455 of yacc.c  */
#line 1201 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_DESIGNNAME);
        if (csh.CheckHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_VALUE, "msc")) {
            csh.AddDesignsToHints();
            csh.hintStatus = HINT_READY;
        } else
            csh.CheckHintAt((yylsp[(1) - (3)]), HINT_ATTR_NAME);
        csh.SetDesignTo((yyvsp[(1) - (3)].str));
  #else
        msc.AddAttribute(Attribute("msc", (yyvsp[(3) - (3)].str), MSC_POS((yyloc)), MSC_POS((yylsp[(3) - (3)]))));
        (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 1220 "language.yy"
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

  case 96:

/* Line 1455 of yacc.c  */
#line 1241 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.entitylist) = (EntityDefList*)((yyvsp[(1) - (1)].entitylist));
  #endif
}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 1247 "language.yy"
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

  case 98:

/* Line 1455 of yacc.c  */
#line 1257 "language.yy"
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

  case 100:

/* Line 1455 of yacc.c  */
#line 1272 "language.yy"
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
    ed->AddAttributeList((yyvsp[(2) - (2)].attriblist), NULL, file_line());
    (yyval.entitylist) = (EntityDefList*)((new EntityDefList)->Append(ed));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 1289 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (1)]));
    csh.AddCSH_EntityName((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str));
  #else
    EntityDef *ed = new EntityDef((yyvsp[(1) - (1)].str), &msc);
    ed->SetLineEnd(MSC_POS((yyloc)));
    ed->AddAttributeList(NULL, NULL, file_line());
    (yyval.entitylist) = (EntityDefList*)((new EntityDefList)->Append(ed));
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 1302 "language.yy"
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

  case 103:

/* Line 1455 of yacc.c  */
#line 1318 "language.yy"
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

  case 104:

/* Line 1455 of yacc.c  */
#line 1331 "language.yy"
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

  case 105:

/* Line 1455 of yacc.c  */
#line 1349 "language.yy"
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
    ed->AddAttributeList(NULL, NULL, file_line());
    (yyval.entitylist) = (EntityDefList*)((new EntityDefList)->Append(ed));
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 1365 "language.yy"
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

  case 107:

/* Line 1455 of yacc.c  */
#line 1383 "language.yy"
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

  case 109:

/* Line 1455 of yacc.c  */
#line 1400 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #endif
}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 1407 "language.yy"
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

  case 111:

/* Line 1455 of yacc.c  */
#line 1433 "language.yy"
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

  case 112:

/* Line 1455 of yacc.c  */
#line 1449 "language.yy"
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

  case 114:

/* Line 1455 of yacc.c  */
#line 1468 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #endif
}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 1475 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_COLORNAME);
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
    csh.AddCSH((yylsp[(3) - (3)]), COLOR_COLORDEF);
    MscColorType color((yyvsp[(3) - (3)].str));
    if (color.valid)
        csh.Contexts.back().Colors[(yyvsp[(1) - (3)].str)] = color;
  #else
    msc.Contexts.back().colors.AddColor((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), msc.Error, MSC_POS((yyloc)));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 1491 "language.yy"
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

  case 117:

/* Line 1455 of yacc.c  */
#line 1511 "language.yy"
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

  case 118:

/* Line 1455 of yacc.c  */
#line 1534 "language.yy"
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

  case 120:

/* Line 1455 of yacc.c  */
#line 1546 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SEMICOLON);
  #endif
}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 1553 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 1560 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 1570 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #endif
}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 1576 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[(2) - (2)]), "Extra stuff after design options. Maybe missing a comma?");
  #endif
}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 1583 "language.yy"
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

  case 128:

/* Line 1455 of yacc.c  */
#line 1602 "language.yy"
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

  case 129:

/* Line 1455 of yacc.c  */
#line 1621 "language.yy"
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

  case 130:

/* Line 1455 of yacc.c  */
#line 1640 "language.yy"
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

  case 131:

/* Line 1455 of yacc.c  */
#line 1661 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(1) - (1)].arclist))
        (yyval.arcparallel) = (new ArcParallel(&msc))->AddArcList((yyvsp[(1) - (1)].arclist));
    else
        (yyval.arcparallel) = NULL;
  #endif
}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 1670 "language.yy"
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

  case 133:

/* Line 1455 of yacc.c  */
#line 1682 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.arcboxseries) = new ArcBoxSeries((yyvsp[(1) - (1)].arcbox));
  #endif
}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 1689 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(2) - (2)].arcbox))->SetLineEnd(MSC_POS((yylsp[(2) - (2)])));
    (yyval.arcboxseries) = ((yyvsp[(1) - (2)].arcboxseries))->AddFollow((yyvsp[(2) - (2)].arcbox));
    ((yyvsp[(2) - (2)].arcbox))->AddAttributeList(NULL); //should come after AddFollow
  #endif
}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 1697 "language.yy"
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

  #endif
}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 1711 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(2) - (3)].arcbox))->AddArcList((yyvsp[(3) - (3)].arclist))->SetLineEnd(MSC_POS((yylsp[(2) - (3)])));
    (yyval.arcboxseries) = ((yyvsp[(1) - (3)].arcboxseries))->AddFollow((yyvsp[(2) - (3)].arcbox));
    ((yyvsp[(2) - (3)].arcbox))->AddAttributeList(NULL); //should come after AddFollow
  #endif
}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 1719 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ArcBox *temp = new ArcBox(MSC_EMPH_UNDETERMINED_FOLLOW, NULL, MSC_POS((yylsp[(1) - (2)])), NULL, MSC_POS((yylsp[(1) - (2)])), &msc);
    temp->AddArcList((yyvsp[(2) - (2)].arclist));
    (yyval.arcboxseries) = ((yyvsp[(1) - (2)].arcboxseries))->AddFollow(temp);
    temp->AddAttributeList(NULL); //should come after AddFollow
  #endif
}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 1728 "language.yy"
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
  #endif
}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 1741 "language.yy"
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
  #endif
}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 1757 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(1) - (1)].arcbox))->AddAttributeList(NULL);
    ((yyvsp[(1) - (1)].arcbox))->SetLineEnd(MSC_POS((yyloc)));
    (yyval.arcbox) = (yyvsp[(1) - (1)].arcbox);
  #endif
}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 1765 "language.yy"
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

  case 142:

/* Line 1455 of yacc.c  */
#line 1778 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(1) - (2)].arcbox))->AddAttributeList(NULL);
    ((yyvsp[(1) - (2)].arcbox))->SetLineEnd(MSC_POS((yylsp[(1) - (2)])));
    (yyval.arcbox) = ((yyvsp[(1) - (2)].arcbox))->AddArcList((yyvsp[(2) - (2)].arclist));
  #endif
}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 1786 "language.yy"
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

  case 144:

/* Line 1455 of yacc.c  */
#line 1801 "language.yy"
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
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 1815 "language.yy"
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

  case 146:

/* Line 1455 of yacc.c  */
#line 1831 "language.yy"
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

  case 147:

/* Line 1455 of yacc.c  */
#line 1842 "language.yy"
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
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 1860 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcpipeseries) = new ArcPipeSeries((yyvsp[(1) - (1)].arcpipe));
  #endif
}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 1867 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ArcPipe *ap = new ArcPipe((yyvsp[(2) - (2)].arcbox));
    ap->SetLineEnd(MSC_POS((yylsp[(2) - (2)])));
    (yyval.arcpipeseries) = ((yyvsp[(1) - (2)].arcpipeseries))->AddFollowWithAttributes(ap, NULL);
  #endif
}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 1875 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(3) - (3)])))
        ArcPipe::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(3) - (3)])))
        ArcPipe::AttributeValues(csh.hintAttrName, csh);
  #else
    ArcPipe *ap = new ArcPipe((yyvsp[(2) - (3)].arcbox));
    ap->SetLineEnd(MSC_POS((yylsp[(2) - (3)])));
    (yyval.arcpipeseries) = ((yyvsp[(1) - (3)].arcpipeseries))->AddFollowWithAttributes(ap, (yyvsp[(3) - (3)].attriblist));
  #endif
}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 1890 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.arcpipeseries) = ((yyvsp[(1) - (2)].arcpipeseries))->AddArcList((yyvsp[(2) - (2)].arclist));
  #endif
}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 1897 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (3)]));
    csh.AddCSH_EntityName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]));
    csh.AddCSH_EntityName((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
  #else
    (yyval.arcbox) = new ArcBox((yyvsp[(2) - (3)].arctype), (yyvsp[(1) - (3)].str), MSC_POS((yylsp[(1) - (3)])), (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(3) - (3)])), &msc);
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 1911 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
    csh.AddCSH_EntityName((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
  #else
    (yyval.arcbox) = new ArcBox((yyvsp[(1) - (2)].arctype), NULL, MSC_POS((yylsp[(1) - (2)])), (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])), &msc);
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 1922 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (2)]));
    csh.AddCSH_EntityName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcbox) = new ArcBox((yyvsp[(2) - (2)].arctype), (yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), NULL, MSC_POS((yylsp[(2) - (2)])), &msc);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 1934 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcbox) = new ArcBox((yyvsp[(1) - (1)].arctype), NULL, MSC_POS((yylsp[(1) - (1)])), NULL, MSC_POS((yylsp[(1) - (1)])), &msc);
  #endif
}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 1944 "language.yy"
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

  case 158:

/* Line 1455 of yacc.c  */
#line 1956 "language.yy"
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

  case 159:

/* Line 1455 of yacc.c  */
#line 1970 "language.yy"
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

  case 160:

/* Line 1455 of yacc.c  */
#line 1983 "language.yy"
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

  case 161:

/* Line 1455 of yacc.c  */
#line 2017 "language.yy"
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

  case 162:

/* Line 1455 of yacc.c  */
#line 2033 "language.yy"
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

  case 166:

/* Line 1455 of yacc.c  */
#line 2049 "language.yy"
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

  case 167:

/* Line 1455 of yacc.c  */
#line 2063 "language.yy"
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

  case 168:

/* Line 1455 of yacc.c  */
#line 2074 "language.yy"
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

  case 169:

/* Line 1455 of yacc.c  */
#line 2086 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
    csh.CheckHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[(1) - (1)].arctype), MARKER_HERE_STR, MARKER_PREV_PARALLEL_STR, &msc);
  #endif
}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 2095 "language.yy"
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

  case 171:

/* Line 1455 of yacc.c  */
#line 2109 "language.yy"
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

  case 172:

/* Line 1455 of yacc.c  */
#line 2120 "language.yy"
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

  case 173:

/* Line 1455 of yacc.c  */
#line 2132 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
    csh.CheckHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[(1) - (1)].arctype), MARKER_PREV_PARALLEL_STR, MARKER_HERE_STR, &msc);
  #endif
}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 2141 "language.yy"
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

  case 175:

/* Line 1455 of yacc.c  */
#line 2153 "language.yy"
    {free((yyvsp[(2) - (3)].str)); (yyval.arcvertarrow)=NULL;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 2154 "language.yy"
    {free((yyvsp[(1) - (4)].str)); free((yyvsp[(3) - (4)].str)); (yyval.arcvertarrow)=NULL;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 2155 "language.yy"
    {free((yyvsp[(2) - (3)].str)); (yyval.arcvertarrow)=NULL;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 2156 "language.yy"
    {free((yyvsp[(1) - (4)].str)); free((yyvsp[(3) - (4)].str)); (yyval.arcvertarrow)=NULL;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 2160 "language.yy"
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

  case 180:

/* Line 1455 of yacc.c  */
#line 2171 "language.yy"
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

  case 181:

/* Line 1455 of yacc.c  */
#line 2182 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
  #else
    (yyval.arcbase) = new ArcDivider((yyvsp[(1) - (1)].arctype), &msc);
  #endif
}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 2190 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 2194 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 2198 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 2203 "language.yy"
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

  case 186:

/* Line 1455 of yacc.c  */
#line 2217 "language.yy"
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

  case 187:

/* Line 1455 of yacc.c  */
#line 2228 "language.yy"
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

  case 188:

/* Line 1455 of yacc.c  */
#line 2240 "language.yy"
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

  case 189:

/* Line 1455 of yacc.c  */
#line 2251 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(1) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment((yyvsp[(2) - (2)].arctype), NULL, MSC_POS((yylsp[(2) - (2)])), MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 2262 "language.yy"
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

  case 191:

/* Line 1455 of yacc.c  */
#line 2276 "language.yy"
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

  case 192:

/* Line 1455 of yacc.c  */
#line 2287 "language.yy"
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

  case 193:

/* Line 1455 of yacc.c  */
#line 2299 "language.yy"
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

  case 194:

/* Line 1455 of yacc.c  */
#line 2310 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment((yyvsp[(2) - (2)].arctype), NULL, MSC_POS((yylsp[(2) - (2)])), MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 2320 "language.yy"
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

  case 196:

/* Line 1455 of yacc.c  */
#line 2334 "language.yy"
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

  case 197:

/* Line 1455 of yacc.c  */
#line 2345 "language.yy"
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

  case 198:

/* Line 1455 of yacc.c  */
#line 2357 "language.yy"
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

  case 199:

/* Line 1455 of yacc.c  */
#line 2368 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment((yyvsp[(2) - (2)].arctype), NULL, MSC_POS((yylsp[(2) - (2)])), MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 2381 "language.yy"
    {(yyval.arctype)=MSC_ARC_UNDETERMINED_SEGMENT;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 2382 "language.yy"
    {(yyval.arctype)=MSC_ARC_UNDETERMINED_SEGMENT;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 2383 "language.yy"
    {(yyval.arctype)=MSC_ARC_UNDETERMINED_SEGMENT;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 2386 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
    (yyval.extvertxpos) = new ExtVertXPos((yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), (yyvsp[(2) - (2)].vertxpos));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 2396 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (4)]), COLOR_KEYWORD);
    if (CaseInsensitiveEqual((yyvsp[(2) - (4)].str), "arc") || CaseInsensitiveEqual((yyvsp[(2) - (4)].str), "rectangle"))
        csh.AddCSH((yylsp[(2) - (4)]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = new CommandSymbol(&msc, (yyvsp[(2) - (4)].str), (yyvsp[(3) - (4)].namerel), (yyvsp[(4) - (4)].extvertxpos), NULL);
  #endif
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(2) - (4)].str));
}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 2408 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (5)]), COLOR_KEYWORD);
    if (CaseInsensitiveEqual((yyvsp[(2) - (5)].str), "arc") || CaseInsensitiveEqual((yyvsp[(2) - (5)].str), "rectangle"))
        csh.AddCSH((yylsp[(2) - (5)]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = new CommandSymbol(&msc, (yyvsp[(2) - (5)].str), (yyvsp[(3) - (5)].namerel), (yyvsp[(4) - (5)].extvertxpos), (yyvsp[(5) - (5)].extvertxpos));
  #endif
    free((yyvsp[(1) - (5)].str));
    free((yyvsp[(2) - (5)].str));
}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 2420 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    if (CaseInsensitiveEqual((yyvsp[(2) - (3)].str), "arc") || CaseInsensitiveEqual((yyvsp[(2) - (3)].str), "rectangle"))
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = new CommandSymbol(&msc, (yyvsp[(2) - (3)].str), NULL, (yyvsp[(3) - (3)].extvertxpos), NULL);
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 2432 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (4)]), COLOR_KEYWORD);
    if (CaseInsensitiveEqual((yyvsp[(2) - (4)].str), "arc") || CaseInsensitiveEqual((yyvsp[(2) - (4)].str), "rectangle"))
        csh.AddCSH((yylsp[(2) - (4)]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = new CommandSymbol(&msc, (yyvsp[(2) - (4)].str), NULL, (yyvsp[(3) - (4)].extvertxpos), (yyvsp[(4) - (4)].extvertxpos));
  #endif
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(2) - (4)].str));
}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 2446 "language.yy"
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

  case 225:

/* Line 1455 of yacc.c  */
#line 2458 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_ColonString((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str), false);
  #endif
    (yyval.str) = (yyvsp[(1) - (1)].str);
}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 2465 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_ColonString((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str), true);
  #endif
    (yyval.str) = (yyvsp[(1) - (1)].str);
}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 2473 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = (new AttributeList)->Append(new Attribute("label", (yyvsp[(1) - (1)].str), MSC_POS((yyloc)), MSC_POS((yyloc)).IncStartCol()));
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 2481 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = ((yyvsp[(2) - (2)].attriblist))->Prepend(new Attribute("label", (yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), MSC_POS((yylsp[(1) - (2)])).IncStartCol()));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 2489 "language.yy"
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

  case 230:

/* Line 1455 of yacc.c  */
#line 2501 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = ((yyvsp[(1) - (2)].attriblist))->Append(new Attribute("label", (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])), MSC_POS((yylsp[(2) - (2)])).IncStartCol()));
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 2512 "language.yy"
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

  case 233:

/* Line 1455 of yacc.c  */
#line 2522 "language.yy"
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

  case 234:

/* Line 1455 of yacc.c  */
#line 2532 "language.yy"
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

  case 235:

/* Line 1455 of yacc.c  */
#line 2543 "language.yy"
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

  case 236:

/* Line 1455 of yacc.c  */
#line 2554 "language.yy"
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

  case 237:

/* Line 1455 of yacc.c  */
#line 2565 "language.yy"
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

  case 238:

/* Line 1455 of yacc.c  */
#line 2576 "language.yy"
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

  case 239:

/* Line 1455 of yacc.c  */
#line 2589 "language.yy"
    {
    (yyval.attriblist) = NULL;
}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 2593 "language.yy"
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

  case 241:

/* Line 1455 of yacc.c  */
#line 2605 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.attriblist) = (new AttributeList)->Append((yyvsp[(1) - (1)].attrib));
  #endif
}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 2612 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    csh.CheckHintBetween((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_NAME);
  #else
    (yyval.attriblist) = ((yyvsp[(1) - (3)].attriblist))->Append((yyvsp[(3) - (3)].attrib));
  #endif
}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 2621 "language.yy"
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

  case 245:

/* Line 1455 of yacc.c  */
#line 2633 "language.yy"
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

  case 246:

/* Line 1455 of yacc.c  */
#line 2647 "language.yy"
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

  case 247:

/* Line 1455 of yacc.c  */
#line 2661 "language.yy"
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

  case 248:

/* Line 1455 of yacc.c  */
#line 2675 "language.yy"
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

  case 249:

/* Line 1455 of yacc.c  */
#line 2689 "language.yy"
    {
    (yyval.attrib) = NULL;
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 2694 "language.yy"
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

  case 271:

/* Line 1455 of yacc.c  */
#line 2715 "language.yy"
    {(yyval.str) = strdup("->");}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 2716 "language.yy"
    {(yyval.str) = strdup("<-");}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 2717 "language.yy"
    {(yyval.str) = strdup("<->");}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 2718 "language.yy"
    {(yyval.str) = strdup("=>");}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 2719 "language.yy"
    {(yyval.str) = strdup("<=");}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 2720 "language.yy"
    {(yyval.str) = strdup("<=>");}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 2721 "language.yy"
    {(yyval.str) = strdup(">>");}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 2722 "language.yy"
    {(yyval.str) = strdup("<<");}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 2723 "language.yy"
    {(yyval.str) = strdup("<<>>");}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 2724 "language.yy"
    {(yyval.str) = strdup(">");}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 2725 "language.yy"
    {(yyval.str) = strdup("<");}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 2726 "language.yy"
    {(yyval.str) = strdup("<>");}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 2728 "language.yy"
    {
    switch ((yyvsp[(1) - (1)].arctype)) {
    case MSC_ARC_DIVIDER:  (yyval.str) = strdup("---"); break;
    case MSC_ARC_DISCO:    (yyval.str) = strdup("..."); break;
    default: _ASSERT(0);
    }
}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 2736 "language.yy"
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

  case 289:

/* Line 1455 of yacc.c  */
#line 2749 "language.yy"
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

  case 291:

/* Line 1455 of yacc.c  */
#line 2765 "language.yy"
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
#line 6355 "language.cc"
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
#line 2775 "language.yy"



/* END OF FILE */

