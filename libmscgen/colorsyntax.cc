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
#line 191 "colorsyntax.cc"

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
     TOK_COMMAND_COMMENT = 311,
     TOK__NEVER__HAPPENS = 312
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
#define TOK_COMMAND_COMMENT 311
#define TOK__NEVER__HAPPENS 312




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
#line 366 "colorsyntax.cc"
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
#line 391 "colorsyntax.cc"

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
#define YYLAST   1190

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  58
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  70
/* YYNRULES -- Number of rules.  */
#define YYNRULES  320
/* YYNRULES -- Number of states.  */
#define YYNSTATES  358

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   312

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
      55,    56,    57
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
     170,   173,   177,   179,   182,   184,   186,   188,   192,   195,
     197,   200,   202,   204,   206,   209,   212,   215,   217,   221,
     223,   226,   229,   231,   235,   237,   239,   241,   243,   245,
     249,   252,   256,   260,   264,   268,   272,   275,   279,   282,
     284,   288,   291,   295,   298,   300,   304,   307,   310,   312,
     316,   319,   321,   325,   328,   330,   334,   336,   340,   344,
     350,   357,   359,   361,   365,   368,   371,   373,   375,   379,
     382,   386,   390,   394,   398,   400,   403,   405,   408,   412,
     416,   419,   424,   428,   430,   433,   436,   440,   443,   446,
     448,   452,   454,   457,   461,   463,   466,   470,   473,   476,
     478,   481,   485,   489,   494,   498,   503,   507,   512,   518,
     520,   522,   524,   526,   528,   532,   535,   538,   540,   544,
     547,   550,   552,   554,   558,   563,   567,   572,   575,   577,
     579,   581,   583,   585,   589,   592,   595,   599,   602,   606,
     609,   612,   616,   619,   623,   626,   629,   633,   636,   638,
     640,   642,   644,   646,   648,   650,   652,   654,   656,   658,
     660,   662,   664,   666,   668,   670,   672,   674,   676,   679,
     681,   683,   685,   690,   696,   700,   705,   709,   713,   716,
     718,   720,   723,   728,   731,   734,   738,   741,   743,   745,
     747,   749,   752,   756,   759,   761,   764,   768,   773,   777,
     780,   784,   786,   789,   792,   794,   798,   801,   805,   809,
     813,   817,   820,   824,   826,   828,   830,   832,   834,   836,
     838,   840,   842,   844,   846,   848,   850,   852,   854,   856,
     858,   860,   862,   864,   866,   868,   870,   872,   874,   876,
     878,   880,   882,   884,   886,   888,   890,   892,   894,   896,
     898,   900,   902,   904,   906,   908,   910,   912,   914,   916,
     919
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      59,     0,    -1,    60,    -1,    60,    51,    -1,    -1,    15,
      63,    -1,    62,    63,    -1,    15,    -1,    15,     1,    -1,
      61,    -1,    61,     1,    -1,    64,    -1,    64,    12,    -1,
      64,    12,    61,    -1,    15,     7,    -1,    15,     7,   125,
      -1,   126,    64,   127,    -1,   126,   127,    -1,   126,    64,
       1,   127,    -1,   126,    64,     1,    -1,   126,    64,    -1,
      65,    -1,    65,    67,    -1,    67,    -1,    66,    -1,    65,
      66,    -1,    67,     9,    -1,    67,     9,    57,    -1,    67,
       1,     9,    57,    -1,     9,    -1,    67,     1,     9,    -1,
      68,    -1,    43,    68,    -1,   100,    -1,   100,   118,    -1,
      40,    -1,    40,   100,    -1,    40,   100,   118,    -1,    44,
      -1,    44,    99,    -1,    44,    99,   118,    -1,   118,    -1,
      77,    -1,    72,    -1,    72,    77,    -1,    77,     8,    -1,
      77,     8,    75,    -1,    72,    77,     8,    -1,    72,    77,
       8,    75,    -1,    73,    -1,    90,    -1,    94,    -1,    89,
      -1,    37,    81,    -1,    38,    78,    -1,    39,    83,    -1,
      34,    -1,    34,   119,    -1,    35,    -1,    35,   119,    -1,
      42,   125,    -1,    42,   125,   119,    -1,    36,    -1,    36,
     119,    -1,   114,    -1,   115,    -1,   116,    -1,    53,    70,
      69,    -1,    53,    70,    -1,    53,    -1,    52,    69,    -1,
      52,    -1,   118,    -1,     5,    -1,     5,   118,    -1,   122,
       6,    -1,     6,   122,    -1,     6,    -1,   122,     6,   122,
      -1,   122,    -1,   122,     6,    -1,     6,   122,    -1,     6,
      -1,   122,     6,   122,    -1,    48,    -1,    47,    -1,    49,
      -1,    50,    -1,    74,    -1,    73,     8,    74,    -1,    73,
       8,    -1,    73,     8,    57,    -1,    73,     8,     1,    -1,
     122,     7,    19,    -1,   122,     7,     5,    -1,   122,     7,
     125,    -1,   122,     7,    -1,    15,     7,   125,    -1,    15,
       7,    -1,    76,    -1,    75,     8,    76,    -1,    75,     8,
      -1,    75,     8,    57,    -1,   122,   118,    -1,   122,    -1,
     122,   118,    63,    -1,   122,    63,    -1,   122,   118,    -1,
     122,    -1,   122,   118,    63,    -1,   122,    63,    -1,    79,
      -1,    78,     8,    79,    -1,    80,   119,    -1,   125,    -1,
      80,     8,   125,    -1,    82,    -1,    81,     8,    82,    -1,
       3,     7,   125,    -1,     3,    84,    85,     9,    12,    -1,
       3,    84,    85,     9,     1,    12,    -1,    11,    -1,    86,
      -1,    85,     9,    86,    -1,    37,    81,    -1,    38,    78,
      -1,    87,    -1,    88,    -1,    87,     8,    88,    -1,    87,
       1,    -1,   122,     7,    19,    -1,   122,     7,     5,    -1,
     122,     7,   125,    -1,    15,     7,   125,    -1,    63,    -1,
      89,    63,    -1,    91,    -1,    90,    95,    -1,    90,    95,
     118,    -1,    90,    95,    63,    -1,    90,    63,    -1,    90,
      95,   118,    63,    -1,    90,   118,    63,    -1,    95,    -1,
      95,   118,    -1,    95,    63,    -1,    95,   118,    63,    -1,
      41,    95,    -1,    41,     1,    -1,    41,    -1,    41,    95,
     118,    -1,    92,    -1,    93,    95,    -1,    93,    95,   118,
      -1,    93,    -1,    93,    63,    -1,   122,    33,   122,    -1,
      33,   122,    -1,   122,    33,    -1,    33,    -1,    45,   122,
      -1,    45,   122,     5,    -1,    45,   122,     6,    -1,    45,
     122,     6,     5,    -1,    45,   122,    10,    -1,    45,   122,
      10,     5,    -1,    45,   122,    33,    -1,    45,   122,     6,
     122,    -1,    45,   122,     6,   122,     5,    -1,    45,    -1,
     122,    -1,    33,    -1,   104,    -1,   106,    -1,   122,    97,
     122,    -1,    97,   122,    -1,   122,    97,    -1,    97,    -1,
     122,   105,   122,    -1,   105,   122,    -1,   122,   105,    -1,
     105,    -1,   122,    -1,    97,   122,    57,    -1,   122,    97,
     122,    57,    -1,   105,   122,    57,    -1,   122,   105,   122,
      57,    -1,    98,    96,    -1,    98,    -1,    32,    -1,   101,
      -1,   102,    -1,   103,    -1,   122,   104,   122,    -1,   104,
     122,    -1,   122,   104,    -1,   101,   107,   122,    -1,   101,
     107,    -1,   122,   105,   122,    -1,   105,   122,    -1,   122,
     105,    -1,   102,   108,   122,    -1,   102,   108,    -1,   122,
     106,   122,    -1,   106,   122,    -1,   122,   106,    -1,   103,
     109,   122,    -1,   103,   109,    -1,    20,    -1,    23,    -1,
      26,    -1,    29,    -1,    21,    -1,    24,    -1,    27,    -1,
      30,    -1,    22,    -1,    25,    -1,    28,    -1,    31,    -1,
     104,    -1,     6,    -1,   105,    -1,     6,    -1,   106,    -1,
       6,    -1,   111,    -1,   122,    -1,    46,    96,    -1,    46,
      -1,   122,    -1,   124,    -1,    54,   112,    71,   110,    -1,
      54,   112,    71,   110,   110,    -1,    54,   112,   111,    -1,
      54,   112,   111,   110,    -1,    54,   112,    71,    -1,    54,
     112,   122,    -1,    54,   112,    -1,    54,    -1,   113,    -1,
     113,   118,    -1,    55,    45,   125,   118,    -1,    55,   118,
      -1,    55,    45,    -1,    55,    45,   118,    -1,    56,   118,
      -1,    56,    -1,    17,    -1,    16,    -1,   117,    -1,   117,
     119,    -1,   119,   117,   119,    -1,   119,   117,    -1,   119,
      -1,    13,    14,    -1,    13,   120,    14,    -1,    13,   120,
       1,    14,    -1,    13,     1,    14,    -1,    13,   120,    -1,
      13,   120,     1,    -1,    13,    -1,    13,    57,    -1,    13,
       1,    -1,   121,    -1,   120,     8,   121,    -1,   120,     8,
      -1,   120,     8,    57,    -1,   125,     7,   125,    -1,   125,
       7,     5,    -1,   125,     7,    19,    -1,   125,     7,    -1,
     125,     7,    57,    -1,   125,    -1,     4,    -1,     3,    -1,
      15,    -1,    37,    -1,    38,    -1,    39,    -1,    36,    -1,
      40,    -1,    41,    -1,    44,    -1,    43,    -1,    34,    -1,
      35,    -1,    42,    -1,    45,    -1,    46,    -1,    47,    -1,
      48,    -1,    49,    -1,    50,    -1,    51,    -1,    53,    -1,
      52,    -1,    54,    -1,    55,    -1,    56,    -1,    20,    -1,
      21,    -1,    22,    -1,    23,    -1,    24,    -1,    25,    -1,
      26,    -1,    27,    -1,    28,    -1,    29,    -1,    30,    -1,
      31,    -1,    32,    -1,    33,    -1,   122,    -1,   123,    -1,
     124,    -1,    18,    -1,    11,    -1,    11,    57,    -1,    12,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   220,   220,   221,   232,   242,   253,   263,   273,   283,
     290,   303,   304,   313,   327,   339,   354,   367,   380,   396,
     408,   421,   422,   436,   452,   462,   473,   487,   488,   489,
     501,   516,   524,   547,   554,   565,   577,   592,   610,   622,
     632,   646,   657,   664,   676,   688,   700,   711,   727,   742,
     754,   761,   768,   775,   784,   793,   802,   811,   824,   833,
     846,   858,   874,   883,   896,   897,   898,   899,   912,   927,
     938,   951,   967,   968,   979,   990,  1002,  1013,  1022,  1036,
    1061,  1073,  1084,  1094,  1109,  1109,  1109,  1109,  1111,  1121,
    1140,  1153,  1154,  1165,  1184,  1203,  1222,  1242,  1259,  1280,
    1286,  1296,  1308,  1311,  1327,  1339,  1355,  1368,  1386,  1401,
    1419,  1435,  1436,  1443,  1469,  1485,  1503,  1504,  1511,  1527,
    1547,  1570,  1581,  1582,  1589,  1596,  1603,  1605,  1606,  1612,
    1619,  1638,  1657,  1676,  1697,  1706,  1718,  1726,  1735,  1749,
    1758,  1768,  1782,  1799,  1807,  1820,  1828,  1843,  1858,  1874,
    1885,  1904,  1911,  1920,  1935,  1936,  1943,  1958,  1970,  1983,
    1994,  2006,  2020,  2034,  2049,  2062,  2077,  2111,  2127,  2145,
    2157,  2173,  2173,  2173,  2175,  2189,  2200,  2212,  2221,  2235,
    2246,  2258,  2267,  2280,  2281,  2282,  2283,  2286,  2297,  2308,
    2316,  2320,  2324,  2329,  2343,  2354,  2366,  2377,  2388,  2402,
    2413,  2425,  2436,  2446,  2460,  2471,  2483,  2494,  2504,  2504,
    2504,  2504,  2505,  2505,  2505,  2505,  2506,  2506,  2506,  2506,
    2508,  2508,  2509,  2509,  2510,  2510,  2512,  2513,  2536,  2546,
    2560,  2560,  2562,  2598,  2621,  2657,  2681,  2717,  2754,  2790,
    2807,  2808,  2820,  2839,  2854,  2869,  2890,  2905,  2918,  2925,
    2933,  2941,  2949,  2961,  2969,  2972,  2982,  2992,  3003,  3014,
    3025,  3036,  3049,  3053,  3065,  3072,  3081,  3091,  3093,  3107,
    3121,  3135,  3149,  3154,  3166,  3166,  3168,  3168,  3169,  3169,
    3170,  3170,  3170,  3171,  3171,  3172,  3172,  3173,  3173,  3173,
    3173,  3173,  3174,  3174,  3174,  3175,  3175,  3175,  3176,  3176,
    3178,  3179,  3180,  3181,  3182,  3183,  3184,  3185,  3186,  3187,
    3188,  3189,  3190,  3198,  3209,  3209,  3209,  3209,  3211,  3225,
    3227
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
  "TOK_COMMAND_COMMENT", "TOK__NEVER__HAPPENS", "$accept", "msc_with_bye",
  "msc", "top_level_arclist", "msckey", "braced_arclist",
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
     305,   306,   307,   308,   309,   310,   311,   312
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    58,    59,    59,    60,    60,    60,    60,    60,    60,
      60,    61,    61,    61,    62,    62,    63,    63,    63,    63,
      63,    64,    64,    64,    65,    65,    66,    66,    66,    66,
      66,    67,    67,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    69,    69,    69,    70,    70,    70,    70,    70,
      71,    71,    71,    71,    72,    72,    72,    72,    73,    73,
      73,    73,    73,    74,    74,    74,    74,    74,    74,    75,
      75,    75,    75,    76,    76,    76,    76,    77,    77,    77,
      77,    78,    78,    79,    80,    80,    81,    81,    82,    83,
      83,    84,    85,    85,    86,    86,    86,    87,    87,    87,
      88,    88,    88,    88,    89,    89,    90,    90,    90,    90,
      90,    90,    90,    91,    91,    91,    91,    92,    92,    92,
      92,    93,    93,    93,    94,    94,    95,    95,    95,    95,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    97,    97,    97,    98,    98,    98,    98,    98,    98,
      98,    98,    98,    98,    98,    98,    98,    99,    99,   100,
     100,   100,   100,   101,   101,   101,   101,   101,   102,   102,
     102,   102,   102,   103,   103,   103,   103,   103,   104,   104,
     104,   104,   105,   105,   105,   105,   106,   106,   106,   106,
     107,   107,   108,   108,   109,   109,   110,   110,   111,   111,
     112,   112,   113,   113,   113,   113,   113,   113,   113,   113,
     114,   114,   115,   115,   115,   115,   116,   116,   117,   117,
     118,   118,   118,   118,   118,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   120,   120,   120,   120,   121,   121,
     121,   121,   121,   121,   122,   122,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   124,   124,   125,   125,   125,   125,   126,   126,
     127
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
       2,     3,     1,     2,     1,     1,     1,     3,     2,     1,
       2,     1,     1,     1,     2,     2,     2,     1,     3,     1,
       2,     2,     1,     3,     1,     1,     1,     1,     1,     3,
       2,     3,     3,     3,     3,     3,     2,     3,     2,     1,
       3,     2,     3,     2,     1,     3,     2,     2,     1,     3,
       2,     1,     3,     2,     1,     3,     1,     3,     3,     5,
       6,     1,     1,     3,     2,     2,     1,     1,     3,     2,
       3,     3,     3,     3,     1,     2,     1,     2,     3,     3,
       2,     4,     3,     1,     2,     2,     3,     2,     2,     1,
       3,     1,     2,     3,     1,     2,     3,     2,     2,     1,
       2,     3,     3,     4,     3,     4,     3,     4,     5,     1,
       1,     1,     1,     1,     3,     2,     2,     1,     3,     2,
       2,     1,     1,     3,     4,     3,     4,     2,     1,     1,
       1,     1,     1,     3,     2,     2,     3,     2,     3,     2,
       2,     3,     2,     3,     2,     2,     3,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     1,
       1,     1,     4,     5,     3,     4,     3,     3,     2,     1,
       1,     2,     4,     2,     2,     3,     2,     1,     1,     1,
       1,     2,     3,     2,     1,     2,     3,     4,     3,     2,
       3,     1,     2,     2,     1,     3,     2,     3,     3,     3,
       3,     2,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       4,   275,   274,    29,   318,     0,     0,   249,   248,   208,
     212,   216,   209,   213,   217,   210,   214,   218,   211,   215,
     219,   189,   159,    56,    58,    62,     0,     0,     0,    35,
       0,     0,     0,    38,    85,    84,    86,    87,    71,    69,
     239,     0,   247,     0,     2,     0,     0,   134,    11,    21,
      24,     0,    31,    43,    49,    88,    42,    52,    50,   136,
     151,   154,    51,   143,    33,   190,   191,   192,     0,     0,
       0,   240,    64,    65,    66,   250,    41,   254,   108,     0,
     319,   263,   255,   276,   317,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   285,
     286,   280,   277,   278,   279,   281,   282,   287,   284,   283,
     288,   289,   290,   291,   292,   293,   294,   296,   295,   297,
     298,   299,   262,     0,   264,   314,   315,   316,   273,     8,
      98,     5,   157,    57,    59,    63,     0,    53,   116,    54,
     111,     0,   114,     0,    55,    36,     0,   148,   147,     0,
      60,     0,    32,   171,   177,   188,    39,   172,   181,   173,
     182,    73,    70,    72,    77,    68,    79,   238,   230,   231,
     244,   243,   246,     1,     3,    10,     6,    12,    25,     0,
       0,    26,    44,   108,     0,    45,   135,   140,   137,     0,
     155,   152,   145,   144,    34,   221,   220,   197,   223,   222,
     202,   225,   224,   207,   194,   199,   204,   241,   251,   253,
      96,   158,   110,   195,   200,   205,   107,   320,     0,    17,
     258,   260,   266,   256,   271,    97,     0,     0,     0,     0,
     113,   121,     0,    37,   150,    61,    98,   175,   169,   187,
     170,    40,   179,   176,   180,    74,    76,    67,    75,    82,
     229,   236,   234,   237,   245,     0,    13,    30,    27,    47,
      92,    91,    89,     0,    46,    99,   104,   139,   138,   142,
     153,   146,   196,   201,   206,   252,    94,    93,    95,   156,
     193,   198,   203,   109,    19,    16,   257,   267,   265,   269,
     270,   272,   268,   118,   117,   112,   115,     0,     0,     0,
       0,   122,     0,   127,     0,    97,   183,   160,   185,   174,
     178,    78,    81,   228,   232,   226,   227,   235,    80,   242,
      28,    48,   101,   106,   103,   141,    18,     0,   124,   125,
       0,   129,     0,     0,   161,   162,   164,   166,   184,   186,
     233,    83,   102,   100,   105,   133,     0,   119,   123,   128,
     131,   130,   132,   163,   167,   165,   120,   168
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,   162,   165,   251,    53,    54,    55,   264,   265,    56,
     139,   140,   141,   137,   138,   144,   232,   300,   301,   302,
     303,    57,    58,    59,    60,    61,    62,    63,   239,   154,
     155,   156,    64,    65,    66,    67,    68,    69,    70,   197,
     200,   203,   314,   315,   167,    71,    72,    73,    74,    75,
      76,    77,   123,   124,   125,   126,   127,   142,    79,   219
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -244
static const yytype_int16 yypact[] =
{
     727,  -244,  -244,  -244,   -51,   346,    14,  -244,  -244,  -244,
    -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,
    -244,  -244,   128,    -5,    -5,    -5,    25,   889,    35,  1105,
     290,   889,   943,  1091,  -244,  -244,  -244,  -244,    67,   142,
    1060,   127,    89,    17,    -3,    23,    39,  -244,    34,   781,
    -244,  1000,  -244,   128,    55,  -244,    58,    39,   191,  -244,
    -244,    40,  -244,   214,    89,   252,   199,   249,   128,   128,
     128,    89,  -244,  -244,  -244,    -5,  -244,   136,  1131,   565,
    -244,    63,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,
    -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,
    -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,
    -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,
    -244,  -244,  -244,   100,  -244,  -244,  -244,  -244,    78,  -244,
     835,  -244,  -244,  -244,  -244,  -244,    81,    85,  -244,   110,
    -244,     5,  -244,   112,  -244,    89,  1159,  -244,    89,   103,
      -5,   121,  -244,  -244,   128,    33,    89,  -244,   128,  -244,
    1145,    89,  -244,  -244,   128,    67,   156,    91,  -244,  -244,
     511,  -244,  -244,  -244,  -244,  -244,  -244,   781,  -244,  1019,
     138,   102,   157,   214,   253,   128,  -244,  -244,   214,    39,
    -244,    89,  -244,    39,  -244,  -244,  -244,   128,  -244,  -244,
     128,  -244,  -244,   128,  -244,  -244,  -244,  -244,  -244,    -5,
     619,   128,  -244,   128,   128,   128,    39,  -244,  1045,  -244,
    -244,   159,   456,  -244,   401,   164,   889,    25,   889,   889,
    -244,  -244,   153,  -244,  -244,  -244,   889,   119,   128,  -244,
    -244,  -244,   125,   128,   128,  -244,  -244,  -244,   128,   128,
      33,    16,    16,   172,  -244,    89,  -244,   129,  -244,   128,
    -244,  -244,  -244,   173,   179,  -244,   214,  -244,    39,  -244,
    -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,
    -244,  -244,  -244,  -244,   180,  -244,  -244,  -244,  -244,  -244,
    -244,  -244,  -244,  -244,  -244,  -244,  -244,   186,    25,   889,
     187,  -244,   141,  -244,   192,  -244,  -244,    21,  -244,   162,
     171,  -244,  -244,  -244,    16,  -244,  -244,  -244,   128,  -244,
    -244,   179,     7,  -244,    39,  -244,  -244,   889,    85,   110,
     123,  -244,    95,   673,  -244,   166,   193,  -244,  -244,  -244,
    -244,  -244,  -244,  -244,  -244,  -244,   198,  -244,  -244,  -244,
    -244,  -244,  -244,  -244,   211,  -244,  -244,  -244
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -244,  -244,  -244,    57,  -244,    29,   160,  -244,   197,   218,
     204,    76,  -244,  -244,  -244,  -244,    79,   -17,   -62,   223,
     -16,    51,  -244,   -13,    60,  -244,  -244,  -244,   -48,  -244,
     -44,  -244,  -244,  -244,  -244,  -244,  -244,    61,    42,   140,
    -244,  -244,   260,  -244,  -244,  -244,   -31,   -21,   -26,  -244,
    -244,  -244,  -243,   144,  -244,  -244,  -244,  -244,  -244,   219,
      18,   -20,  -244,    83,     0,  -244,   268,    11,  -244,  -217
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -262
static const yytype_int16 yytable[] =
{
      78,   285,   157,   133,   134,   135,    80,   159,     5,   317,
       1,     2,   158,   229,    -7,   129,   128,   173,     5,     1,
       2,   130,   132,    -9,   175,     4,   334,   335,   136,   146,
     149,   336,    78,   160,   196,   131,     1,     2,   143,   166,
     168,   202,   150,     1,     2,   199,   177,   213,   174,    78,
       4,     4,   215,   183,   337,   208,   163,   214,   149,   171,
     172,   149,   250,   184,   342,    -7,   185,   326,   204,   205,
     206,   340,   161,    22,    -9,   176,   189,   220,   238,    78,
       5,   193,   194,     7,     8,   224,   186,   187,   226,   207,
     190,   148,   192,   227,     1,     2,   216,   249,     1,     2,
    -259,   221,     5,  -259,  -259,     7,     8,   212,   222,  -259,
     297,  -259,  -259,  -259,   223,   213,  -259,  -259,   228,   188,
     215,   230,   191,   231,   346,   214,     1,     2,   236,   157,
     235,     1,     2,  -259,   159,   347,   211,   250,   297,   244,
       5,   225,   331,     7,     8,     1,     2,   257,   164,   332,
    -126,  -259,     7,     8,   237,   240,     1,     2,   242,   258,
     298,   299,   248,   233,   246,   259,   234,   253,   297,     1,
       2,   353,   170,   286,   241,   -15,   306,    78,   318,   245,
     210,   255,   308,   163,   263,   266,   320,   322,   254,   275,
     298,   299,   217,   327,     1,     2,   330,   272,   355,   333,
     273,   216,     4,   274,     5,   198,   268,     7,     8,   270,
     356,   279,   212,   280,   281,   282,   357,   267,   269,   338,
      10,   278,   271,    13,    22,     4,    16,     5,   339,    19,
       7,     8,   304,   128,   256,   292,   152,   293,   307,   218,
     296,   247,   321,   309,   310,   283,   178,   305,   311,   312,
     240,   316,   316,   -90,   260,   201,     1,     2,   195,   266,
     343,   -90,   -90,   262,   -90,   -90,   -90,   179,   151,   -90,
     -90,    11,     9,   319,    14,    12,   182,    17,    15,   295,
      20,    18,   348,   329,   324,   328,   -90,   294,   349,   145,
    -149,   147,   313,     1,     2,   323,   209,   325,  -149,  -149,
     243,  -149,  -149,  -149,   -90,   288,  -149,  -149,   169,     0,
     261,   252,     0,     0,   316,     0,     0,     0,   341,     0,
       0,     0,   266,    22,     0,     0,     0,     0,     0,     0,
     304,     0,   304,     0,     0,   354,     0,     0,   345,     0,
       0,  -149,     0,     0,   352,     0,  -261,    81,     0,     1,
       2,     0,     0,   344,  -261,  -261,     0,  -261,  -261,  -261,
      82,    83,  -261,  -261,    84,     0,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,     1,     2,   289,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,     0,     0,    84,
     290,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   291,     1,
       2,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,     0,     0,    84,     0,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   287,     1,     2,     0,     0,     0,     0,
       0,     0,     0,     0,     5,     0,    83,     7,     8,    84,
       0,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,     1,     2,
       0,     0,     0,     0,     3,     0,     4,   217,     5,     0,
     151,     7,     8,     0,     0,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
       0,     0,    34,    35,    36,    37,     0,    38,    39,    40,
      41,    42,     1,     2,   276,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,     0,     0,    84,   277,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,     1,     2,   350,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,     0,
       0,    84,   351,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
       1,     2,     0,     0,     0,     0,     3,     0,     4,     0,
       5,     0,     6,     7,     8,     0,     0,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,     0,     0,    34,    35,    36,    37,     0,    38,
      39,    40,    41,    42,     1,     2,     0,     0,     0,     0,
       3,     0,     4,     0,     5,     0,   151,     7,     8,     0,
       0,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,     0,     0,    34,    35,
      36,    37,     0,    38,    39,    40,    41,    42,     1,     2,
       0,     0,     0,     0,     0,     0,   -14,     0,     0,     0,
      83,     0,     0,    84,     0,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,     1,     2,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,     0,     0,    84,     0,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,     1,     2,     0,     0,
       0,     0,     0,     0,     4,     0,     5,     0,   151,     7,
       8,     0,     0,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,     0,    33,     0,     0,
      34,    35,    36,    37,     0,    38,    39,    40,    41,    42,
     -23,   180,     0,   -23,   -23,     0,     0,     0,   -23,   181,
       0,   -23,   -23,   -23,     0,     0,   -23,   -23,     0,   -22,
     180,     0,   -22,   -22,     0,     0,     0,   -22,   181,     0,
     -22,   -22,   -22,   -23,     0,   -22,   -22,     0,     0,     0,
       0,     0,     0,     0,     0,   -20,   284,     0,   -20,   -20,
       0,   -23,   -22,   -20,   -20,     0,   -20,   217,   -20,     0,
       0,   -20,   -20,     1,     2,     0,     0,     0,     0,     0,
     -22,     0,     0,     0,     0,     0,     0,     0,   -20,     0,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,     1,     2,   -20,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     1,     2,
       0,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,     0,   153,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,   210,     0,
       0,     0,     4,     0,     5,     0,     0,     7,     8,     0,
       0,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,     0,   211,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,     0,   153,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-244))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       0,   218,    33,    23,    24,    25,    57,    33,    13,   252,
       3,     4,    33,     8,     0,     1,     5,     0,    13,     3,
       4,     7,    22,     0,     1,    11,     5,     6,     3,    29,
      30,    10,    32,    33,    65,     6,     3,     4,     3,    39,
      40,    67,    31,     3,     4,    66,    12,    78,    51,    49,
      11,    11,    78,    53,    33,    75,    38,    78,    58,    41,
      42,    61,    46,     8,    57,    51,     8,   284,    68,    69,
      70,   314,     5,    33,    51,    46,    58,    14,    45,    79,
      13,    63,    64,    16,    17,     7,    57,    58,     7,    71,
      61,    30,    63,     8,     3,     4,    78,     6,     3,     4,
       0,     1,    13,     3,     4,    16,    17,    78,     8,     9,
      15,    11,    12,    13,    14,   146,    16,    17,     8,    58,
     146,   141,    61,    11,     1,   146,     3,     4,     7,   160,
     150,     3,     4,    33,   160,    12,    33,    46,    15,   160,
      13,   130,     1,    16,    17,     3,     4,     9,     6,     8,
       9,    51,    16,    17,   154,   155,     3,     4,   158,    57,
      37,    38,     6,   145,   164,     8,   148,   167,    15,     3,
       4,     5,    45,    14,   156,    11,    57,   177,     6,   161,
       7,   170,    57,   165,   184,   185,    57,     8,   170,   209,
      37,    38,    12,     7,     3,     4,     9,   197,     5,     7,
     200,   183,    11,   203,    13,     6,   188,    16,    17,   191,
      12,   211,   183,   213,   214,   215,     5,   188,   189,    57,
      21,   210,   193,    24,    33,    11,    27,    13,    57,    30,
      16,    17,   232,   222,   177,   224,    32,   226,   238,    79,
     229,   165,   259,   243,   244,   216,    49,   236,   248,   249,
     250,   251,   252,     0,     1,     6,     3,     4,     6,   259,
     322,     8,     9,   184,    11,    12,    13,    49,    15,    16,
      17,    22,    20,   255,    25,    23,    53,    28,    26,   228,
      31,    29,   330,   299,   266,   298,    33,   227,   332,    29,
       0,     1,   250,     3,     4,   266,    77,   268,     8,     9,
     160,    11,    12,    13,    51,   222,    16,    17,    40,    -1,
      57,   167,    -1,    -1,   314,    -1,    -1,    -1,   318,    -1,
      -1,    -1,   322,    33,    -1,    -1,    -1,    -1,    -1,    -1,
     330,    -1,   332,    -1,    -1,   335,    -1,    -1,   327,    -1,
      -1,    51,    -1,    -1,   333,    -1,     0,     1,    -1,     3,
       4,    -1,    -1,   324,     8,     9,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    -1,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,     3,     4,     5,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,     3,
       4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    18,    -1,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,     3,     4,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    13,    -1,    15,    16,    17,    18,
      -1,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,     3,     4,
      -1,    -1,    -1,    -1,     9,    -1,    11,    12,    13,    -1,
      15,    16,    17,    -1,    -1,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      -1,    -1,    47,    48,    49,    50,    -1,    52,    53,    54,
      55,    56,     3,     4,     5,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,     3,     4,     5,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
       3,     4,    -1,    -1,    -1,    -1,     9,    -1,    11,    -1,
      13,    -1,    15,    16,    17,    -1,    -1,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    -1,    -1,    47,    48,    49,    50,    -1,    52,
      53,    54,    55,    56,     3,     4,    -1,    -1,    -1,    -1,
       9,    -1,    11,    -1,    13,    -1,    15,    16,    17,    -1,
      -1,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    -1,    -1,    47,    48,
      49,    50,    -1,    52,    53,    54,    55,    56,     3,     4,
      -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,
      15,    -1,    -1,    18,    -1,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,     3,     4,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    18,    -1,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,     3,     4,    -1,    -1,
      -1,    -1,    -1,    -1,    11,    -1,    13,    -1,    15,    16,
      17,    -1,    -1,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    -1,    44,    -1,    -1,
      47,    48,    49,    50,    -1,    52,    53,    54,    55,    56,
       0,     1,    -1,     3,     4,    -1,    -1,    -1,     8,     9,
      -1,    11,    12,    13,    -1,    -1,    16,    17,    -1,     0,
       1,    -1,     3,     4,    -1,    -1,    -1,     8,     9,    -1,
      11,    12,    13,    33,    -1,    16,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     0,     1,    -1,     3,     4,
      -1,    51,    33,     8,     9,    -1,    11,    12,    13,    -1,
      -1,    16,    17,     3,     4,    -1,    -1,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,     3,     4,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
      -1,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,     7,    -1,
      -1,    -1,    11,    -1,    13,    -1,    -1,    16,    17,    -1,
      -1,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    33,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     9,    11,    13,    15,    16,    17,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    47,    48,    49,    50,    52,    53,
      54,    55,    56,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    72,    73,    74,    77,    89,    90,    91,
      92,    93,    94,    95,   100,   101,   102,   103,   104,   105,
     106,   113,   114,   115,   116,   117,   118,   119,   122,   126,
      57,     1,    14,    15,    18,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,   120,   121,   122,   123,   124,   125,     1,
       7,    63,   122,   119,   119,   119,     3,    81,    82,    78,
      79,    80,   125,     3,    83,   100,   122,     1,    95,   122,
     125,    15,    68,    33,    97,    98,    99,   104,   105,   106,
     122,     5,    69,   118,     6,    70,   122,   112,   122,   124,
      45,   118,   118,     0,    51,     1,    63,    12,    66,    67,
       1,     9,    77,   122,     8,     8,    63,    63,    95,   118,
      63,    95,    63,   118,   118,     6,   104,   107,     6,   105,
     108,     6,   106,   109,   122,   122,   122,   118,   119,   117,
       7,    33,    63,   104,   105,   106,   118,    12,    64,   127,
      14,     1,     8,    14,     7,   125,     7,     8,     8,     8,
     119,    11,    84,   118,   118,   119,     7,   122,    45,    96,
     122,   118,   122,    97,   105,   118,   122,    69,     6,     6,
      46,    71,   111,   122,   118,   125,    61,     9,    57,     8,
       1,    57,    74,   122,    75,    76,   122,    63,   118,    63,
     118,    63,   122,   122,   122,   119,     5,    19,   125,   122,
     122,   122,   122,    63,     1,   127,    14,    57,   121,     5,
      19,    57,   125,   125,    82,    79,   125,    15,    37,    38,
      85,    86,    87,    88,   122,   125,    57,   122,    57,   122,
     122,   122,   122,    96,   110,   111,   122,   110,     6,   118,
      57,    75,     8,    63,   118,    63,   127,     7,    81,    78,
       9,     1,     8,     7,     5,     6,    10,    33,    57,    57,
     110,   122,    57,    76,    63,   125,     1,    12,    86,    88,
       5,    19,   125,     5,   122,     5,    12,     5
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
#line 1956 "colorsyntax.cc"
	break;
      case 4: /* "TOK_QSTRING" */

/* Line 1391 of yacc.c  */
#line 210 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 1965 "colorsyntax.cc"
	break;
      case 5: /* "TOK_NUMBER" */

/* Line 1391 of yacc.c  */
#line 215 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 1974 "colorsyntax.cc"
	break;
      case 15: /* "TOK_MSC" */

/* Line 1391 of yacc.c  */
#line 211 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 1983 "colorsyntax.cc"
	break;
      case 16: /* "TOK_COLON_STRING" */

/* Line 1391 of yacc.c  */
#line 210 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 1992 "colorsyntax.cc"
	break;
      case 17: /* "TOK_COLON_QUOTED_STRING" */

/* Line 1391 of yacc.c  */
#line 210 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2001 "colorsyntax.cc"
	break;
      case 18: /* "TOK_STYLE_NAME" */

/* Line 1391 of yacc.c  */
#line 210 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2010 "colorsyntax.cc"
	break;
      case 19: /* "TOK_BOOLEAN" */

/* Line 1391 of yacc.c  */
#line 215 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2019 "colorsyntax.cc"
	break;
      case 34: /* "TOK_COMMAND_HEADING" */

/* Line 1391 of yacc.c  */
#line 213 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2028 "colorsyntax.cc"
	break;
      case 35: /* "TOK_COMMAND_NUDGE" */

/* Line 1391 of yacc.c  */
#line 213 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2037 "colorsyntax.cc"
	break;
      case 36: /* "TOK_COMMAND_NEWPAGE" */

/* Line 1391 of yacc.c  */
#line 213 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2046 "colorsyntax.cc"
	break;
      case 37: /* "TOK_COMMAND_DEFCOLOR" */

/* Line 1391 of yacc.c  */
#line 212 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2055 "colorsyntax.cc"
	break;
      case 38: /* "TOK_COMMAND_DEFSTYLE" */

/* Line 1391 of yacc.c  */
#line 212 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2064 "colorsyntax.cc"
	break;
      case 39: /* "TOK_COMMAND_DEFDESIGN" */

/* Line 1391 of yacc.c  */
#line 212 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2073 "colorsyntax.cc"
	break;
      case 40: /* "TOK_COMMAND_BIG" */

/* Line 1391 of yacc.c  */
#line 211 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2082 "colorsyntax.cc"
	break;
      case 41: /* "TOK_COMMAND_PIPE" */

/* Line 1391 of yacc.c  */
#line 211 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2091 "colorsyntax.cc"
	break;
      case 42: /* "TOK_COMMAND_MARK" */

/* Line 1391 of yacc.c  */
#line 214 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2100 "colorsyntax.cc"
	break;
      case 43: /* "TOK_COMMAND_PARALLEL" */

/* Line 1391 of yacc.c  */
#line 214 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2109 "colorsyntax.cc"
	break;
      case 44: /* "TOK_VERTICAL" */

/* Line 1391 of yacc.c  */
#line 215 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2118 "colorsyntax.cc"
	break;
      case 45: /* "TOK_AT" */

/* Line 1391 of yacc.c  */
#line 215 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2127 "colorsyntax.cc"
	break;
      case 46: /* "TOK_AT_POS" */

/* Line 1391 of yacc.c  */
#line 215 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2136 "colorsyntax.cc"
	break;
      case 47: /* "TOK_SHOW" */

/* Line 1391 of yacc.c  */
#line 215 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2145 "colorsyntax.cc"
	break;
      case 48: /* "TOK_HIDE" */

/* Line 1391 of yacc.c  */
#line 215 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2154 "colorsyntax.cc"
	break;
      case 49: /* "TOK_ACTIVATE" */

/* Line 1391 of yacc.c  */
#line 215 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2163 "colorsyntax.cc"
	break;
      case 50: /* "TOK_DEACTIVATE" */

/* Line 1391 of yacc.c  */
#line 215 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2172 "colorsyntax.cc"
	break;
      case 51: /* "TOK_BYE" */

/* Line 1391 of yacc.c  */
#line 214 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2181 "colorsyntax.cc"
	break;
      case 52: /* "TOK_COMMAND_VSPACE" */

/* Line 1391 of yacc.c  */
#line 216 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2190 "colorsyntax.cc"
	break;
      case 54: /* "TOK_COMMAND_SYMBOL" */

/* Line 1391 of yacc.c  */
#line 216 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2199 "colorsyntax.cc"
	break;
      case 55: /* "TOK_COMMAND_NOTE" */

/* Line 1391 of yacc.c  */
#line 216 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2208 "colorsyntax.cc"
	break;
      case 56: /* "TOK_COMMAND_COMMENT" */

/* Line 1391 of yacc.c  */
#line 216 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2217 "colorsyntax.cc"
	break;
      case 61: /* "top_level_arclist" */

/* Line 1391 of yacc.c  */
#line 205 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1391 of yacc.c  */
#line 2226 "colorsyntax.cc"
	break;
      case 63: /* "braced_arclist" */

/* Line 1391 of yacc.c  */
#line 205 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1391 of yacc.c  */
#line 2235 "colorsyntax.cc"
	break;
      case 64: /* "arclist_maybe_no_semicolon" */

/* Line 1391 of yacc.c  */
#line 205 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1391 of yacc.c  */
#line 2244 "colorsyntax.cc"
	break;
      case 65: /* "arclist" */

/* Line 1391 of yacc.c  */
#line 205 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1391 of yacc.c  */
#line 2253 "colorsyntax.cc"
	break;
      case 66: /* "arc_with_parallel_semicolon" */

/* Line 1391 of yacc.c  */
#line 200 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2262 "colorsyntax.cc"
	break;
      case 67: /* "arc_with_parallel" */

/* Line 1391 of yacc.c  */
#line 200 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2271 "colorsyntax.cc"
	break;
      case 68: /* "arc" */

/* Line 1391 of yacc.c  */
#line 200 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2280 "colorsyntax.cc"
	break;
      case 69: /* "full_arcattrlist_with_label_or_number" */

/* Line 1391 of yacc.c  */
#line 208 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1391 of yacc.c  */
#line 2289 "colorsyntax.cc"
	break;
      case 73: /* "optlist" */

/* Line 1391 of yacc.c  */
#line 205 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1391 of yacc.c  */
#line 2298 "colorsyntax.cc"
	break;
      case 74: /* "opt" */

/* Line 1391 of yacc.c  */
#line 200 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2307 "colorsyntax.cc"
	break;
      case 75: /* "entitylist" */

/* Line 1391 of yacc.c  */
#line 206 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entitylist);};

/* Line 1391 of yacc.c  */
#line 2316 "colorsyntax.cc"
	break;
      case 76: /* "entity" */

/* Line 1391 of yacc.c  */
#line 206 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entitylist);};

/* Line 1391 of yacc.c  */
#line 2325 "colorsyntax.cc"
	break;
      case 77: /* "first_entity" */

/* Line 1391 of yacc.c  */
#line 206 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entitylist);};

/* Line 1391 of yacc.c  */
#line 2334 "colorsyntax.cc"
	break;
      case 80: /* "tok_stringlist" */

/* Line 1391 of yacc.c  */
#line 207 "language.yy"
	{if (!C_S_H) delete (yyvaluep->stringlist);};

/* Line 1391 of yacc.c  */
#line 2343 "colorsyntax.cc"
	break;
      case 89: /* "parallel" */

/* Line 1391 of yacc.c  */
#line 204 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcparallel);};

/* Line 1391 of yacc.c  */
#line 2352 "colorsyntax.cc"
	break;
      case 90: /* "box_list" */

/* Line 1391 of yacc.c  */
#line 203 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcboxseries);};

/* Line 1391 of yacc.c  */
#line 2361 "colorsyntax.cc"
	break;
      case 91: /* "first_box" */

/* Line 1391 of yacc.c  */
#line 203 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbox);};

/* Line 1391 of yacc.c  */
#line 2370 "colorsyntax.cc"
	break;
      case 92: /* "first_pipe" */

/* Line 1391 of yacc.c  */
#line 203 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcpipe);};

/* Line 1391 of yacc.c  */
#line 2379 "colorsyntax.cc"
	break;
      case 93: /* "pipe_list_no_content" */

/* Line 1391 of yacc.c  */
#line 203 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcpipeseries);};

/* Line 1391 of yacc.c  */
#line 2388 "colorsyntax.cc"
	break;
      case 94: /* "pipe_list" */

/* Line 1391 of yacc.c  */
#line 203 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcpipeseries);};

/* Line 1391 of yacc.c  */
#line 2397 "colorsyntax.cc"
	break;
      case 95: /* "boxrel" */

/* Line 1391 of yacc.c  */
#line 203 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbox);};

/* Line 1391 of yacc.c  */
#line 2406 "colorsyntax.cc"
	break;
      case 96: /* "vertxpos" */

/* Line 1391 of yacc.c  */
#line 198 "language.yy"
	{if (!C_S_H) delete (yyvaluep->vertxpos);};

/* Line 1391 of yacc.c  */
#line 2415 "colorsyntax.cc"
	break;
      case 98: /* "vertrel_no_xpos" */

/* Line 1391 of yacc.c  */
#line 199 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcvertarrow);};

/* Line 1391 of yacc.c  */
#line 2424 "colorsyntax.cc"
	break;
      case 99: /* "vertrel" */

/* Line 1391 of yacc.c  */
#line 200 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2433 "colorsyntax.cc"
	break;
      case 100: /* "arcrel" */

/* Line 1391 of yacc.c  */
#line 200 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2442 "colorsyntax.cc"
	break;
      case 101: /* "arcrel_to" */

/* Line 1391 of yacc.c  */
#line 202 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 1391 of yacc.c  */
#line 2451 "colorsyntax.cc"
	break;
      case 102: /* "arcrel_from" */

/* Line 1391 of yacc.c  */
#line 202 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 1391 of yacc.c  */
#line 2460 "colorsyntax.cc"
	break;
      case 103: /* "arcrel_bidir" */

/* Line 1391 of yacc.c  */
#line 202 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 1391 of yacc.c  */
#line 2469 "colorsyntax.cc"
	break;
      case 112: /* "symbol_type_string" */

/* Line 1391 of yacc.c  */
#line 209 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2478 "colorsyntax.cc"
	break;
      case 113: /* "symbol_command_no_attr" */

/* Line 1391 of yacc.c  */
#line 201 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2487 "colorsyntax.cc"
	break;
      case 114: /* "symbol_command" */

/* Line 1391 of yacc.c  */
#line 201 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2496 "colorsyntax.cc"
	break;
      case 115: /* "note" */

/* Line 1391 of yacc.c  */
#line 201 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2505 "colorsyntax.cc"
	break;
      case 116: /* "comment" */

/* Line 1391 of yacc.c  */
#line 201 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2514 "colorsyntax.cc"
	break;
      case 117: /* "colon_string" */

/* Line 1391 of yacc.c  */
#line 209 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2523 "colorsyntax.cc"
	break;
      case 118: /* "full_arcattrlist_with_label" */

/* Line 1391 of yacc.c  */
#line 207 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1391 of yacc.c  */
#line 2532 "colorsyntax.cc"
	break;
      case 119: /* "full_arcattrlist" */

/* Line 1391 of yacc.c  */
#line 207 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1391 of yacc.c  */
#line 2541 "colorsyntax.cc"
	break;
      case 120: /* "arcattrlist" */

/* Line 1391 of yacc.c  */
#line 207 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1391 of yacc.c  */
#line 2550 "colorsyntax.cc"
	break;
      case 121: /* "arcattr" */

/* Line 1391 of yacc.c  */
#line 207 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attrib);};

/* Line 1391 of yacc.c  */
#line 2559 "colorsyntax.cc"
	break;
      case 122: /* "entity_string" */

/* Line 1391 of yacc.c  */
#line 209 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2568 "colorsyntax.cc"
	break;
      case 123: /* "reserved_word_string" */

/* Line 1391 of yacc.c  */
#line 209 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2577 "colorsyntax.cc"
	break;
      case 124: /* "symbol_string" */

/* Line 1391 of yacc.c  */
#line 209 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2586 "colorsyntax.cc"
	break;
      case 125: /* "string" */

/* Line 1391 of yacc.c  */
#line 209 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2595 "colorsyntax.cc"
	break;
      case 127: /* "scope_close" */

/* Line 1391 of yacc.c  */
#line 200 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2604 "colorsyntax.cc"
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
#line 222 "language.yy"
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
#line 232 "language.yy"
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
#line 243 "language.yy"
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
#line 254 "language.yy"
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
#line 264 "language.yy"
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
#line 274 "language.yy"
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
#line 284 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    msc.AddArcs((yyvsp[(1) - (1)].arclist));
  #endif
}
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 291 "language.yy"
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
#line 305 "language.yy"
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
#line 314 "language.yy"
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
#line 328 "language.yy"
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
#line 340 "language.yy"
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
#line 355 "language.yy"
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
#line 368 "language.yy"
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
#line 381 "language.yy"
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
#line 397 "language.yy"
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
#line 409 "language.yy"
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
#line 423 "language.yy"
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
#line 437 "language.yy"
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
#line 453 "language.yy"
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
#line 463 "language.yy"
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
#line 474 "language.yy"
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
#line 490 "language.yy"
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
#line 502 "language.yy"
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
#line 517 "language.yy"
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
#line 525 "language.yy"
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
#line 548 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.arcbase)=((yyvsp[(1) - (1)].arcbase))->AddAttributeList(NULL);
  #endif
}
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 555 "language.yy"
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
#line 566 "language.yy"
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
#line 578 "language.yy"
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
#line 593 "language.yy"
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
#line 611 "language.yy"
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
#line 623 "language.yy"
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
#line 633 "language.yy"
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
#line 647 "language.yy"
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
#line 658 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (new CommandEntity((yyvsp[(1) - (1)].entitylist), &msc))->AddAttributeList(NULL);
  #endif
}
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 665 "language.yy"
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
#line 677 "language.yy"
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
#line 689 "language.yy"
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
#line 701 "language.yy"
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
#line 712 "language.yy"
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
#line 728 "language.yy"
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
#line 743 "language.yy"
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
#line 755 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcboxseries); //to remove warning for downcast
  #endif
}
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 762 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcpipeseries); //to remove warning for downcast
  #endif
}
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 769 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = ((yyvsp[(1) - (1)].arcparallel))->AddAttributeList(NULL);
  #endif
}
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 776 "language.yy"
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
#line 785 "language.yy"
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
#line 794 "language.yy"
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
#line 803 "language.yy"
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
#line 812 "language.yy"
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
#line 825 "language.yy"
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
#line 834 "language.yy"
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
#line 847 "language.yy"
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
#line 859 "language.yy"
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
#line 875 "language.yy"
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
#line 884 "language.yy"
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

  case 67:

/* Line 1806 of yacc.c  */
#line 900 "language.yy"
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

  case 68:

/* Line 1806 of yacc.c  */
#line 913 "language.yy"
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

  case 69:

/* Line 1806 of yacc.c  */
#line 928 "language.yy"
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

  case 70:

/* Line 1806 of yacc.c  */
#line 939 "language.yy"
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

  case 71:

/* Line 1806 of yacc.c  */
#line 952 "language.yy"
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

  case 73:

/* Line 1806 of yacc.c  */
#line 969 "language.yy"
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

  case 74:

/* Line 1806 of yacc.c  */
#line 980 "language.yy"
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

  case 75:

/* Line 1806 of yacc.c  */
#line 991 "language.yy"
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

  case 76:

/* Line 1806 of yacc.c  */
#line 1003 "language.yy"
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

  case 77:

/* Line 1806 of yacc.c  */
#line 1014 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF);
  #else
    (yyval.namerel) = NULL;
  #endif
}
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 1023 "language.yy"
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

  case 79:

/* Line 1806 of yacc.c  */
#line 1037 "language.yy"
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

  case 80:

/* Line 1806 of yacc.c  */
#line 1062 "language.yy"
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

  case 81:

/* Line 1806 of yacc.c  */
#line 1074 "language.yy"
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

  case 82:

/* Line 1806 of yacc.c  */
#line 1085 "language.yy"
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

  case 83:

/* Line 1806 of yacc.c  */
#line 1095 "language.yy"
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

  case 88:

/* Line 1806 of yacc.c  */
#line 1112 "language.yy"
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

  case 89:

/* Line 1806 of yacc.c  */
#line 1122 "language.yy"
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

  case 90:

/* Line 1806 of yacc.c  */
#line 1141 "language.yy"
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

  case 92:

/* Line 1806 of yacc.c  */
#line 1155 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    csh.AddCSH_Error((yylsp[(3) - (3)]), "An option expected here.");
  #else
    (yyval.arclist) = (yyvsp[(1) - (3)].arclist);
  #endif
}
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 1166 "language.yy"
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

  case 94:

/* Line 1806 of yacc.c  */
#line 1185 "language.yy"
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

  case 95:

/* Line 1806 of yacc.c  */
#line 1204 "language.yy"
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

  case 96:

/* Line 1806 of yacc.c  */
#line 1223 "language.yy"
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

  case 97:

/* Line 1806 of yacc.c  */
#line 1243 "language.yy"
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
        (yyval.arcbase) = msc.AddAttribute(Attribute("msc", (yyvsp[(3) - (3)].str), MSC_POS((yyloc)), MSC_POS((yylsp[(3) - (3)]))));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 1260 "language.yy"
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

  case 99:

/* Line 1806 of yacc.c  */
#line 1281 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.entitylist) = (EntityDefList*)((yyvsp[(1) - (1)].entitylist));
  #endif
}
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 1287 "language.yy"
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

  case 101:

/* Line 1806 of yacc.c  */
#line 1297 "language.yy"
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

  case 103:

/* Line 1806 of yacc.c  */
#line 1312 "language.yy"
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

  case 104:

/* Line 1806 of yacc.c  */
#line 1328 "language.yy"
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

  case 105:

/* Line 1806 of yacc.c  */
#line 1340 "language.yy"
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

  case 106:

/* Line 1806 of yacc.c  */
#line 1356 "language.yy"
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

  case 107:

/* Line 1806 of yacc.c  */
#line 1369 "language.yy"
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

  case 108:

/* Line 1806 of yacc.c  */
#line 1387 "language.yy"
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

  case 109:

/* Line 1806 of yacc.c  */
#line 1402 "language.yy"
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

  case 110:

/* Line 1806 of yacc.c  */
#line 1420 "language.yy"
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

  case 112:

/* Line 1806 of yacc.c  */
#line 1437 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #endif
}
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 1444 "language.yy"
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

  case 114:

/* Line 1806 of yacc.c  */
#line 1470 "language.yy"
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

  case 115:

/* Line 1806 of yacc.c  */
#line 1486 "language.yy"
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

  case 117:

/* Line 1806 of yacc.c  */
#line 1505 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #endif
}
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 1512 "language.yy"
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

  case 119:

/* Line 1806 of yacc.c  */
#line 1528 "language.yy"
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

  case 120:

/* Line 1806 of yacc.c  */
#line 1548 "language.yy"
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

  case 121:

/* Line 1806 of yacc.c  */
#line 1571 "language.yy"
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

  case 123:

/* Line 1806 of yacc.c  */
#line 1583 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SEMICOLON);
  #endif
}
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 1590 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 1597 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 1607 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #endif
}
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 1613 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[(2) - (2)]), "Extra stuff after design options. Maybe missing a comma?");
  #endif
}
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 1620 "language.yy"
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

  case 131:

/* Line 1806 of yacc.c  */
#line 1639 "language.yy"
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

  case 132:

/* Line 1806 of yacc.c  */
#line 1658 "language.yy"
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

  case 133:

/* Line 1806 of yacc.c  */
#line 1677 "language.yy"
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

  case 134:

/* Line 1806 of yacc.c  */
#line 1698 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(1) - (1)].arclist))
        (yyval.arcparallel) = (new ArcParallel(&msc))->AddArcList((yyvsp[(1) - (1)].arclist));
    else
        (yyval.arcparallel) = NULL;
  #endif
}
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 1707 "language.yy"
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

  case 136:

/* Line 1806 of yacc.c  */
#line 1719 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.arcboxseries) = new ArcBoxSeries((yyvsp[(1) - (1)].arcbox));
    ((yyvsp[(1) - (1)].arcbox))->MakeMeLastNotable();
  #endif
}
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 1727 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(2) - (2)].arcbox))->SetLineEnd(MSC_POS((yylsp[(2) - (2)])));
    (yyval.arcboxseries) = ((yyvsp[(1) - (2)].arcboxseries))->AddFollow((yyvsp[(2) - (2)].arcbox));
    ((yyvsp[(2) - (2)].arcbox))->AddAttributeList(NULL); //should come after AddFollow
    ((yyvsp[(1) - (2)].arcboxseries))->MakeMeLastNotable();
  #endif
}
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 1736 "language.yy"
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

  case 139:

/* Line 1806 of yacc.c  */
#line 1750 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(2) - (3)].arcbox))->AddArcList((yyvsp[(3) - (3)].arclist))->SetLineEnd(MSC_POS((yylsp[(2) - (3)])));
    (yyval.arcboxseries) = ((yyvsp[(1) - (3)].arcboxseries))->AddFollow((yyvsp[(2) - (3)].arcbox));
    ((yyvsp[(2) - (3)].arcbox))->AddAttributeList(NULL); //should come after AddFollow
    ((yyvsp[(1) - (3)].arcboxseries))->MakeMeLastNotable();
  #endif
}
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 1759 "language.yy"
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

  case 141:

/* Line 1806 of yacc.c  */
#line 1769 "language.yy"
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

  case 142:

/* Line 1806 of yacc.c  */
#line 1783 "language.yy"
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

  case 143:

/* Line 1806 of yacc.c  */
#line 1800 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(1) - (1)].arcbox))->AddAttributeList(NULL);
    ((yyvsp[(1) - (1)].arcbox))->SetLineEnd(MSC_POS((yyloc)));
    (yyval.arcbox) = (yyvsp[(1) - (1)].arcbox);
  #endif
}
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 1808 "language.yy"
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

  case 145:

/* Line 1806 of yacc.c  */
#line 1821 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(1) - (2)].arcbox))->AddAttributeList(NULL);
    ((yyvsp[(1) - (2)].arcbox))->SetLineEnd(MSC_POS((yylsp[(1) - (2)])));
    (yyval.arcbox) = ((yyvsp[(1) - (2)].arcbox))->AddArcList((yyvsp[(2) - (2)].arclist));
  #endif
}
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 1829 "language.yy"
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

  case 147:

/* Line 1806 of yacc.c  */
#line 1844 "language.yy"
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

  case 148:

/* Line 1806 of yacc.c  */
#line 1859 "language.yy"
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

  case 149:

/* Line 1806 of yacc.c  */
#line 1875 "language.yy"
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

  case 150:

/* Line 1806 of yacc.c  */
#line 1886 "language.yy"
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

  case 151:

/* Line 1806 of yacc.c  */
#line 1905 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcpipeseries) = new ArcPipeSeries((yyvsp[(1) - (1)].arcpipe));
  #endif
}
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 1912 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ArcPipe *ap = new ArcPipe((yyvsp[(2) - (2)].arcbox));
    ap->MakeMeLastNotable();
    ap->SetLineEnd(MSC_POS((yylsp[(2) - (2)])));
    (yyval.arcpipeseries) = ((yyvsp[(1) - (2)].arcpipeseries))->AddFollowWithAttributes(ap, NULL);
  #endif
}
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 1921 "language.yy"
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

  case 155:

/* Line 1806 of yacc.c  */
#line 1937 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.arcpipeseries) = ((yyvsp[(1) - (2)].arcpipeseries))->AddArcList((yyvsp[(2) - (2)].arclist));
  #endif
}
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 1944 "language.yy"
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

  case 157:

/* Line 1806 of yacc.c  */
#line 1959 "language.yy"
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

  case 158:

/* Line 1806 of yacc.c  */
#line 1971 "language.yy"
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

  case 159:

/* Line 1806 of yacc.c  */
#line 1984 "language.yy"
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

  case 160:

/* Line 1806 of yacc.c  */
#line 1995 "language.yy"
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

  case 161:

/* Line 1806 of yacc.c  */
#line 2007 "language.yy"
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

  case 162:

/* Line 1806 of yacc.c  */
#line 2021 "language.yy"
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

  case 163:

/* Line 1806 of yacc.c  */
#line 2035 "language.yy"
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

  case 164:

/* Line 1806 of yacc.c  */
#line 2050 "language.yy"
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

  case 165:

/* Line 1806 of yacc.c  */
#line 2063 "language.yy"
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

  case 166:

/* Line 1806 of yacc.c  */
#line 2078 "language.yy"
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

  case 167:

/* Line 1806 of yacc.c  */
#line 2112 "language.yy"
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

  case 168:

/* Line 1806 of yacc.c  */
#line 2128 "language.yy"
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

  case 169:

/* Line 1806 of yacc.c  */
#line 2146 "language.yy"
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

  case 170:

/* Line 1806 of yacc.c  */
#line 2158 "language.yy"
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

  case 174:

/* Line 1806 of yacc.c  */
#line 2176 "language.yy"
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

  case 175:

/* Line 1806 of yacc.c  */
#line 2190 "language.yy"
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

  case 176:

/* Line 1806 of yacc.c  */
#line 2201 "language.yy"
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

  case 177:

/* Line 1806 of yacc.c  */
#line 2213 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
    csh.CheckHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[(1) - (1)].arctype), MARKER_HERE_STR, MARKER_PREV_PARALLEL_STR, &msc);
  #endif
}
    break;

  case 178:

/* Line 1806 of yacc.c  */
#line 2222 "language.yy"
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

  case 179:

/* Line 1806 of yacc.c  */
#line 2236 "language.yy"
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

  case 180:

/* Line 1806 of yacc.c  */
#line 2247 "language.yy"
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

  case 181:

/* Line 1806 of yacc.c  */
#line 2259 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
    csh.CheckHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[(1) - (1)].arctype), MARKER_PREV_PARALLEL_STR, MARKER_HERE_STR, &msc);
  #endif
}
    break;

  case 182:

/* Line 1806 of yacc.c  */
#line 2268 "language.yy"
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

  case 183:

/* Line 1806 of yacc.c  */
#line 2280 "language.yy"
    {free((yyvsp[(2) - (3)].str)); (yyval.arcvertarrow)=NULL;}
    break;

  case 184:

/* Line 1806 of yacc.c  */
#line 2281 "language.yy"
    {free((yyvsp[(1) - (4)].str)); free((yyvsp[(3) - (4)].str)); (yyval.arcvertarrow)=NULL;}
    break;

  case 185:

/* Line 1806 of yacc.c  */
#line 2282 "language.yy"
    {free((yyvsp[(2) - (3)].str)); (yyval.arcvertarrow)=NULL;}
    break;

  case 186:

/* Line 1806 of yacc.c  */
#line 2283 "language.yy"
    {free((yyvsp[(1) - (4)].str)); free((yyvsp[(3) - (4)].str)); (yyval.arcvertarrow)=NULL;}
    break;

  case 187:

/* Line 1806 of yacc.c  */
#line 2287 "language.yy"
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

  case 188:

/* Line 1806 of yacc.c  */
#line 2298 "language.yy"
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

  case 189:

/* Line 1806 of yacc.c  */
#line 2309 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
  #else
    (yyval.arcbase) = new ArcDivider((yyvsp[(1) - (1)].arctype), &msc);
  #endif
}
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 2317 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 2321 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 2325 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 193:

/* Line 1806 of yacc.c  */
#line 2330 "language.yy"
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

  case 194:

/* Line 1806 of yacc.c  */
#line 2344 "language.yy"
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

  case 195:

/* Line 1806 of yacc.c  */
#line 2355 "language.yy"
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

  case 196:

/* Line 1806 of yacc.c  */
#line 2367 "language.yy"
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

  case 197:

/* Line 1806 of yacc.c  */
#line 2378 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(1) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment((yyvsp[(2) - (2)].arctype), NULL, MSC_POS((yylsp[(2) - (2)])), MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 2389 "language.yy"
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

  case 199:

/* Line 1806 of yacc.c  */
#line 2403 "language.yy"
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

  case 200:

/* Line 1806 of yacc.c  */
#line 2414 "language.yy"
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

  case 201:

/* Line 1806 of yacc.c  */
#line 2426 "language.yy"
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

  case 202:

/* Line 1806 of yacc.c  */
#line 2437 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment((yyvsp[(2) - (2)].arctype), NULL, MSC_POS((yylsp[(2) - (2)])), MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 2447 "language.yy"
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

  case 204:

/* Line 1806 of yacc.c  */
#line 2461 "language.yy"
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

  case 205:

/* Line 1806 of yacc.c  */
#line 2472 "language.yy"
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

  case 206:

/* Line 1806 of yacc.c  */
#line 2484 "language.yy"
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

  case 207:

/* Line 1806 of yacc.c  */
#line 2495 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment((yyvsp[(2) - (2)].arctype), NULL, MSC_POS((yylsp[(2) - (2)])), MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 221:

/* Line 1806 of yacc.c  */
#line 2508 "language.yy"
    {(yyval.arctype)=MSC_ARC_UNDETERMINED_SEGMENT;}
    break;

  case 223:

/* Line 1806 of yacc.c  */
#line 2509 "language.yy"
    {(yyval.arctype)=MSC_ARC_UNDETERMINED_SEGMENT;}
    break;

  case 225:

/* Line 1806 of yacc.c  */
#line 2510 "language.yy"
    {(yyval.arctype)=MSC_ARC_UNDETERMINED_SEGMENT;}
    break;

  case 227:

/* Line 1806 of yacc.c  */
#line 2514 "language.yy"
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

  case 228:

/* Line 1806 of yacc.c  */
#line 2537 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ExtvxposDesignatorName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));

  #else
    (yyval.extvertxpos) = new ExtVertXPos((yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), (yyvsp[(2) - (2)].vertxpos));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 229:

/* Line 1806 of yacc.c  */
#line 2547 "language.yy"
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

  case 232:

/* Line 1806 of yacc.c  */
#line 2563 "language.yy"
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

  case 233:

/* Line 1806 of yacc.c  */
#line 2599 "language.yy"
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

  case 234:

/* Line 1806 of yacc.c  */
#line 2622 "language.yy"
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

  case 235:

/* Line 1806 of yacc.c  */
#line 2658 "language.yy"
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

  case 236:

/* Line 1806 of yacc.c  */
#line 2682 "language.yy"
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

  case 237:

/* Line 1806 of yacc.c  */
#line 2718 "language.yy"
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

  case 238:

/* Line 1806 of yacc.c  */
#line 2755 "language.yy"
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

  case 239:

/* Line 1806 of yacc.c  */
#line 2791 "language.yy"
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

  case 241:

/* Line 1806 of yacc.c  */
#line 2809 "language.yy"
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

  case 242:

/* Line 1806 of yacc.c  */
#line 2821 "language.yy"
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

  case 243:

/* Line 1806 of yacc.c  */
#line 2840 "language.yy"
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

  case 244:

/* Line 1806 of yacc.c  */
#line 2855 "language.yy"
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

  case 245:

/* Line 1806 of yacc.c  */
#line 2870 "language.yy"
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

  case 246:

/* Line 1806 of yacc.c  */
#line 2891 "language.yy"
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

  case 247:

/* Line 1806 of yacc.c  */
#line 2906 "language.yy"
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

  case 248:

/* Line 1806 of yacc.c  */
#line 2919 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_ColonString((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str), false);
  #endif
    (yyval.str) = (yyvsp[(1) - (1)].str);
}
    break;

  case 249:

/* Line 1806 of yacc.c  */
#line 2926 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_ColonString((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str), true);
  #endif
    (yyval.str) = (yyvsp[(1) - (1)].str);
}
    break;

  case 250:

/* Line 1806 of yacc.c  */
#line 2934 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = (new AttributeList)->Append(new Attribute("label", (yyvsp[(1) - (1)].str), MSC_POS((yyloc)), MSC_POS((yyloc)).IncStartCol()));
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 251:

/* Line 1806 of yacc.c  */
#line 2942 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = ((yyvsp[(2) - (2)].attriblist))->Prepend(new Attribute("label", (yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), MSC_POS((yylsp[(1) - (2)])).IncStartCol()));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 252:

/* Line 1806 of yacc.c  */
#line 2950 "language.yy"
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

  case 253:

/* Line 1806 of yacc.c  */
#line 2962 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = ((yyvsp[(1) - (2)].attriblist))->Append(new Attribute("label", (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])), MSC_POS((yylsp[(2) - (2)])).IncStartCol()));
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 255:

/* Line 1806 of yacc.c  */
#line 2973 "language.yy"
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

  case 256:

/* Line 1806 of yacc.c  */
#line 2983 "language.yy"
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

  case 257:

/* Line 1806 of yacc.c  */
#line 2993 "language.yy"
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

  case 258:

/* Line 1806 of yacc.c  */
#line 3004 "language.yy"
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

  case 259:

/* Line 1806 of yacc.c  */
#line 3015 "language.yy"
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

  case 260:

/* Line 1806 of yacc.c  */
#line 3026 "language.yy"
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

  case 261:

/* Line 1806 of yacc.c  */
#line 3037 "language.yy"
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

  case 262:

/* Line 1806 of yacc.c  */
#line 3050 "language.yy"
    {
    (yyval.attriblist) = NULL;
}
    break;

  case 263:

/* Line 1806 of yacc.c  */
#line 3054 "language.yy"
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

  case 264:

/* Line 1806 of yacc.c  */
#line 3066 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.attriblist) = (new AttributeList)->Append((yyvsp[(1) - (1)].attrib));
  #endif
}
    break;

  case 265:

/* Line 1806 of yacc.c  */
#line 3073 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    csh.CheckHintBetween((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_NAME);
  #else
    (yyval.attriblist) = ((yyvsp[(1) - (3)].attriblist))->Append((yyvsp[(3) - (3)].attrib));
  #endif
}
    break;

  case 266:

/* Line 1806 of yacc.c  */
#line 3082 "language.yy"
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

  case 268:

/* Line 1806 of yacc.c  */
#line 3094 "language.yy"
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

  case 269:

/* Line 1806 of yacc.c  */
#line 3108 "language.yy"
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

  case 270:

/* Line 1806 of yacc.c  */
#line 3122 "language.yy"
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

  case 271:

/* Line 1806 of yacc.c  */
#line 3136 "language.yy"
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

  case 272:

/* Line 1806 of yacc.c  */
#line 3150 "language.yy"
    {
    (yyval.attrib) = NULL;
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 273:

/* Line 1806 of yacc.c  */
#line 3155 "language.yy"
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

  case 300:

/* Line 1806 of yacc.c  */
#line 3178 "language.yy"
    {(yyval.str) = strdup("->");}
    break;

  case 301:

/* Line 1806 of yacc.c  */
#line 3179 "language.yy"
    {(yyval.str) = strdup("<-");}
    break;

  case 302:

/* Line 1806 of yacc.c  */
#line 3180 "language.yy"
    {(yyval.str) = strdup("<->");}
    break;

  case 303:

/* Line 1806 of yacc.c  */
#line 3181 "language.yy"
    {(yyval.str) = strdup("=>");}
    break;

  case 304:

/* Line 1806 of yacc.c  */
#line 3182 "language.yy"
    {(yyval.str) = strdup("<=");}
    break;

  case 305:

/* Line 1806 of yacc.c  */
#line 3183 "language.yy"
    {(yyval.str) = strdup("<=>");}
    break;

  case 306:

/* Line 1806 of yacc.c  */
#line 3184 "language.yy"
    {(yyval.str) = strdup(">>");}
    break;

  case 307:

/* Line 1806 of yacc.c  */
#line 3185 "language.yy"
    {(yyval.str) = strdup("<<");}
    break;

  case 308:

/* Line 1806 of yacc.c  */
#line 3186 "language.yy"
    {(yyval.str) = strdup("<<>>");}
    break;

  case 309:

/* Line 1806 of yacc.c  */
#line 3187 "language.yy"
    {(yyval.str) = strdup(">");}
    break;

  case 310:

/* Line 1806 of yacc.c  */
#line 3188 "language.yy"
    {(yyval.str) = strdup("<");}
    break;

  case 311:

/* Line 1806 of yacc.c  */
#line 3189 "language.yy"
    {(yyval.str) = strdup("<>");}
    break;

  case 312:

/* Line 1806 of yacc.c  */
#line 3191 "language.yy"
    {
    switch ((yyvsp[(1) - (1)].arctype)) {
    case MSC_ARC_DIVIDER:  (yyval.str) = strdup("---"); break;
    case MSC_ARC_DISCO:    (yyval.str) = strdup("..."); break;
    default: _ASSERT(0);
    }
}
    break;

  case 313:

/* Line 1806 of yacc.c  */
#line 3199 "language.yy"
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

  case 318:

/* Line 1806 of yacc.c  */
#line 3212 "language.yy"
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

  case 320:

/* Line 1806 of yacc.c  */
#line 3228 "language.yy"
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
#line 7042 "colorsyntax.cc"
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
#line 3238 "language.yy"



/* END OF FILE */

